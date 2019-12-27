//ScanTrac Side View Source File
// NewImageDisplayDialog.cpp : implementation file
//

#include "stdafx.h"
#include "ScanTrac.h"
#include "NewImageDisplayDialog.h"
#include "NoticeDialog.h"
#include "YesNoDialog.h"
#include "AlphaEntryDialog.h"
#include "NumericEntryDialog.h"
#include "PasswordEntryDialog.h"
#include "SelectItemDialog.h"
#include "SerialPort.h"
#include "ImageAquisitionThread.h"
#include "SelectInspectionDialog.h"
#include "SelectMultipleItemDialog.h"
#include "ConfigureProductContaminantProcessesDialog.h"
#include "HistogramDisplayDialog.h"
#include "HelpDialog.h"
#include "AutoSetupImageDisplayDialog.h"
#include "UndoAddToLearnDialog.h"
#include "ProcessQuickLearnImagesThread.h"

//#include <afxdlgs.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewImageDisplayDialog dialog

extern BYTE vGlobalNormalLearnDilateTimes;
extern BYTE vGlobalQuickLearnDilateTimes;
//extern bool vGlobalUseHalconLibrary;
extern CString vGlobalTopString;
extern CString vGlobalBottomString;
extern WORD vGlobalPasswordTimeOut;
extern BYTE vGlobalLearnState;
extern bool vGlobalEvenSecond;
//extern BYTE vGlobalMouseMoveCounter;
extern WORD vGlobalHideRemoteControlIndicator;
//extern WORD vGlobalUnderRemoteControl;
//extern bool vGlobalRemoteControlIndicatorEnabled;
extern CProcessQuickLearnImagesThread *vGlobalProcessQuickLearnImagesThread;
extern CString vGlobalCurrentDirectory;
extern BYTE vGlobalUseCommaForDecimalPoint;
extern BYTE vGlobalShiftKeyDown;
extern CSerialPort *vGlobaluCSerialPort;
extern CImageAquisitionThread *vGlobalImageAquisitionThread;
extern HCURSOR vGlobalCursorSave;
extern CProduct *vGlobalCurrentProduct;
extern int vGlobalPasswordLevel;
extern unsigned long vGlobalDialogTitleColor;

CNewImageDisplayDialog::CNewImageDisplayDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CNewImageDisplayDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewImageDisplayDialog)
	//}}AFX_DATA_INIT

	vSensitivity = 0xFF;
	vOldAddToLearnCount = 0;
	vHideRedIndicator = false;
	vProcessedRedImage = NULL;
	vRemoveJarPunt = 0;
	vImageBufferForDisplay = NULL;
	vPlotLine = 0;
	//vOldUnderRemoteControl = vGlobalUnderRemoteControl;
#ifdef UseIFCDLLs
	Erflag TempErrorFlag;
	vBlobParameters = mvt_blob_create_params(&TempErrorFlag);
	if (TempErrorFlag != IM_OK)
		ReportErrorMessage("MVTools Error 1", cEMailInspx,32000);
	else
	{
		vBlobParameters->calc_results = 0;
		vBlobParameters->min_area = 10;
		vBlobParameters->connected = CONNECT8;
		vBlobResults = mvt_blob_create_results(20, true, &TempErrorFlag);
		if (TempErrorFlag != IM_OK)
			ReportErrorMessage("MVTools Error 2", cEMailInspx,32000);
	}
#endif
	vNeedToDeleteDisplayingImageData = false;
	//vSaveTypesOfRejectsToView = 0xFFFF;
	//vRequire2InARow = 0;
	//vPreSelectDarkSpotsInImage = 0;
	vPreSmoothImage = 0;
	vShowBackground = false;
	vAllRejectsHidden = false;
	vEditROIBoundaries = false;
	vFirstInspectionToView = 0;
	vOldGoodCountTotal = 0xFFFF;
	vOldImproveLearnTotalContainers = 0;
	vOldImproveLearnCount = 0;
	vWindowInitialized = false;
	vOldLearnCount = 0;
	vGaveNoRejectsToShowMessage = false;
	vTweakingThreshold = false;
	vShowingInspectionNumberOfRedPixels = 0;
	vLastCalculateThreshold = -1;
	vDarkestValueInROI = 255;
	vLightestValueInROI = 0;
	vSizeOfAllContaminants = 0;
	vSizeOfLargestContaminant = 0;
	vNumberOfContaminants = 0;
	vHaveAnImageToShow = false;
	vImageDisplayOptions = false;
	vEditWhileRunning = false;
	for (WORD TempLoop = 0; TempLoop < 8; TempLoop++)
		vOldCounter[TempLoop] = 2147483647;  //big number so must update right away

	vRejectsImageDisplay = true;
	vAnalysisImageDisplay = false;
	vOldGlobalPasswordOK = false;
	vAverageDensity = 0;
	vCurrentDensity = 0;
	vMakeExitButtonGreen = false;
	vChangeMade = false;
	vShowDilationCount = 0;
	vShowErodeCount = 0;
	vShowErodeCountAtStart = 0;
	vShowDilationCountAtEnd = 0;
	vShowErodeCountAtEnd = 0;
	vShowBluredImage = 0;
	vIPContaminant = 0;
	vShowTopHatImage = 0;
	vIntensityAdjust = 0;
	vOldFindGoodIntensity = false;
	vOldCollectingDensityAverage = false;
	vOldDisplayLastMinimumGoodMaximumBad = 0;
	vNumberOfRejectsInLocalBuffer = 0;
	vHaveRejects = false;
	vOldAverageEnabled = false;
	vOldCalibrateBodyTriggerReferenceEnabled = false;
	vOldStructureEnabled = false;
	vOldTakingQuickLearnImages = false;
	vInspectionShowing = NULL;
	vInspectionNumberShowing = 0;
	vProductPasswordOK = false;
	vProcessNextImage = false;
	vShowDetectorEdgeLines = false;
	vSaveImagesToFiles = 0;
	vSaveImagesToFilesName = "";
	vWaitForScreenUpdate = false;
	vShowType = cShowTypeContaminant;
	vDisplayChanges = false;
	vShowAlternateImage = false;
	vOldSystemRunMode = 0;
	vShowingProcess = 0;
	vDisplayingImageData = NULL;
	vOnScreenImageData = NULL;
	vProcessData = NULL;
	vMouseState = 0;		//1 is left mouse button clicked down, 2 is left mouse button transistioned up, 3 is double clicked left mouse button
	vRecievedButtonPress = false;

	ClearAllDisplayOptions(true);

	vShowOriginalImageFactor = 0;
	vShowEnhancedImageFactor = 0;
	vShowAverage = false;
	vShowHistogram = false;
	vShowStructure = false;
	//vShowGlassStructure = false;
	//vShowGlassHistogram = false;
	vOldXRayOnNotice = false;
	MinimumEnhanceROIIntensity = 0;
	MaximumEnhanceROIIntensity = 0;
	MinimumROIIntensity = 0;
	MaximumROIIntensity = 0;
	vDisplayEnhancedImage = 0;
	vWhiteOutAnythingTouchingTheTop = 0;
	vLocalSystemData = NULL;
	vShowingLiveImages = false;
	vUpdateDisplayTimerHandle = 56;
	vOneSecondTimerHandle = 55;
	vWaitForScreenUpdateTimerHandle = 57;
	vFinishLoadingFileTimerHandle = 58;
	vCursorUpdateTimerHandle = 59;
	vMouseDownRepeatTimerHandle = 60;
	vShowNextFrame = false;
	vThreshold =-1;
	vMaximumThreshold = 255;
	vShowThresholdMenu = false;
	vImproveLearnMenu = false;
	vShowingUpperThreshold = true;
	for (WORD TempLoop = 0; TempLoop < 256; TempLoop++)
		vHistogramData[TempLoop] = 0;
	
	vFunction2ButtonEnable = true;
	vFunction3ButtonEnable = false;
	vFunction4ButtonEnable = false;
	vFunction5ButtonEnable = false;
	vSubFunction1ButtonEnable = true;
	vSubFunction2ButtonEnable = true;
	vSubFunction3ButtonEnable = true;
	vSubFunction4ButtonEnable = true;
	vSubFunction5ButtonEnable = true;
	vSubFunction6ButtonEnable = true;
	vSubFunction7ButtonEnable = true;
	vSubFunction8ButtonEnable = true;

	vLocalCWndCollection.Add(this);
	vLocalCWndCollection.Add(&m_CurrentInspection);
	vLocalCWndCollection.Add(&m_SubFunction8Display);
	vLocalCWndCollection.Add(&m_SubFunction7Display);
	vLocalCWndCollection.Add(&m_SubFunction6Display);
	vLocalCWndCollection.Add(&m_SubFunction4Display);
	vLocalCWndCollection.Add(&m_SubFunction3Display);
	vLocalCWndCollection.Add(&m_SubFunction2Display);
	vLocalCWndCollection.Add(&m_SubFunction1Display);
	vLocalCWndCollection.Add(&m_SubFunction5Display);
	vLocalCWndCollection.Add(&m_InspectionThreshold);
	vLocalCWndCollection.Add(&m_XRaysOn);
	vLocalCWndCollection.Add(&m_ImageStatus);
	vLocalCWndCollection.Add(&m_SubFunction8Button);
	vLocalCWndCollection.Add(&m_SubFunction7Button);
	vLocalCWndCollection.Add(&m_SubFunction6Button);
	vLocalCWndCollection.Add(&m_SubFunction5Button);
	vLocalCWndCollection.Add(&m_SubFunction4Button);
	vLocalCWndCollection.Add(&m_SubFunction3Button);
	vLocalCWndCollection.Add(&m_SubFunction2Button);
	vLocalCWndCollection.Add(&m_SubFunction1Button);
	vLocalCWndCollection.Add(&m_Function4Button);
	vLocalCWndCollection.Add(&m_Function5Button);
	vLocalCWndCollection.Add(&m_Function3Button);
	vLocalCWndCollection.Add(&m_Function2Button);
	vLocalCWndCollection.Add(&m_Function1Button);
	vLocalCWndCollection.Add(&m_DialogTitleStaticText);
	vLocalCWndCollection.Add(&m_ErrorMessage);
	vLocalCWndCollection.Add(&m_Background);
	vLocalCWndCollection.Add(&m_Sensitivity);
	vLocalCWndCollection.Add(&m_RejectReason);
	vLocalCWndCollection.Add(&m_MainStatusLine);
}

void CNewImageDisplayDialog::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	// TODO: Add your message handler code here
	
	vSensitivityLeft = (WORD)ResizeXCoor(3);
	vSensitivityBottom = (WORD)ResizeYCoor(125);
	vSensitivityHeight = (WORD)ResizeYCoor(10);
	vTop = vSensitivityBottom - vSensitivityHeight;
	vSensitivityWidth = (WORD)ResizeXCoor(100);
	vRight = vSensitivityLeft + vSensitivityWidth;
	vSensitivityRight = vSensitivityLeft + vSensitivityWidth;
	vLearnSensitivityBottom = (WORD)ResizeYCoor(215);
	vSensitivityTop = vSensitivityBottom - vSensitivityHeight;

	//now show image of Aux Detecto ejects aux detectors
	//vTotalNumberOfInspectionsToDisplay = vLocalSystemData->vTotalNumberOfInspections - vGlobalCurrentProduct->vNumberOfAuxiliaryDetectors;
	vTotalNumberOfInspectionsToDisplay = vLocalSystemData->vTotalNumberOfInspections;

	WORD TempMask = 0xFFFF;
	TempMask = TempMask << vTotalNumberOfInspectionsToDisplay;

	if (ThereIsEnoughMemory(vLocalSystemData->vITIPCDig->vOriginalBufferSize, "Rejects Red Buffer"))
	{
		vProcessedRedImage = (BYTE *)malloc(vLocalSystemData->vITIPCDig->vOriginalBufferSize);
	}

	vOldSystemRunMode = vLocalSystemData->vSystemRunMode;
	vMaximumUnderfillByWeightThreshold = vLocalSystemData->vITIPCDig->vOriginalBufferSizeX * vLocalSystemData->vITIPCDig->vOriginalBufferSizeY * 255;

	//enum {cDefectsInNoColor, cDefectsInPseudoColor, cDefectsInRed, cDefectInRedBox, cDefectsInRedBoxes};
	//if (vLocalConfigurationData->vDefectDisplayMode < cDefectsInPseudoColor)
	//	vLocalConfigurationData->vDefectDisplayMode = cDefectInRedBox;
	//if (vLocalConfigurationData->vDefectDisplayMode > cDefectsInRedBoxes)
	//	vLocalConfigurationData->vDefectDisplayMode = cDefectInRedBox;

	InitializeLocalLastRejectsBuffer(true);
	ShowOrHideDisplays();
	SetDlgItemText(IDC_InspectionThreshold, _T(""));

	vSaveImagesToFilesName = vLocalSystemData->vCurrentProductName;
 	vLocalSystemData->vTweakReferenceAdjust = 0;
	vLocalSystemData->vTweakTopReferenceAdjust = 0;
	vLocalSystemData->vITIPCDig->vRejectBufferShowing = -1;
	if (vLocalConfigurationData->vProductSetupPassword == "")
		vProductPasswordOK = true;

	CString TempText("");

	if (vAnalysisImageDisplay)
		vDialogTitleString.LoadString(IDS_ImageAnalysis);
	else
		vDialogTitleString.LoadString(IDS_RejectDisplay);

	SetDlgItemText(IDC_DialogTitleStaticText1,vDialogTitleString);
	TempText.LoadString(IDS_MainMenu);
	SetDlgItemText(IDC_Function1Button,TempText);
	TempText.LoadString(IDS_Scale);
	CString TempText1("");

	TempText.LoadString(IDS_Void);
	SetDlgItemText(IDC_LookingForVoids,TempText);

	TempText = "";
	SetDlgItemText(IDC_ImageStatus,TempText);

	TempText.LoadString(IDS_XRAYON);
	SetDlgItemText(IDC_XRaysOn,TempText);

	if (!PasswordOK(cTemporaryInspxPassword,false))
	{
		m_SubFunction2Button.ShowWindow(SW_HIDE);
		m_SubFunction3Button.ShowWindow(SW_HIDE);
		m_SubFunction4Button.ShowWindow(SW_HIDE);
		m_SubFunction6Button.ShowWindow(SW_HIDE);
		m_SubFunction7Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction2Button.GetRectToInvalidateArrow());
		this->InvalidateRect(&m_SubFunction3Button.GetRectToInvalidateArrow());
		this->InvalidateRect(&m_SubFunction4Button.GetRectToInvalidateArrow());
		this->InvalidateRect(&m_SubFunction6Button.GetRectToInvalidateArrow());
		this->InvalidateRect(&m_SubFunction7Button.GetRectToInvalidateArrow());
	}
	
	CWnd *TempWindow = CWnd::GetDlgItem(IDC_Background);

	if (TempWindow)
		vLocalSystemData->vITIPCDig->vWindowHandle = TempWindow->m_hWnd;

	//vLocalSystemData->vITIPCDig->vDisplayImageSizeX = 416;
	//416 is normal background so can see buttons
	//768 is too big to fit screen, but one pixel per 2 feet
	//632 fits full screen
	vLocalSystemData->vITIPCDig->vDisplayImageSizeY = (WORD) ResizeYCoor(360);
	//338 is auto setup image display
	//360 is normal background so can see buttons
	//512 is too big to fit screen, but one pixel per camera bit
	//476 fits full screen
	vLocalSystemData->vITIPCDig->SetupImageBuffer(
		vLocalSystemData->vITIPCDig->vDisplayImageSizeX, 
		vLocalSystemData->vITIPCDig->vDisplayImageSizeY);

	vLocalSystemData->vITIPCDig->vRejectBufferShowing = -1;

	bool TempCompiledForPCDig = false;
#ifdef UsePCDigBoard
	TempCompiledForPCDig = true;
#endif
	if (!TempCompiledForPCDig)
	{
		SetDlgItemText(IDC_ErrorMessage, _T("Demo Version, Compiled without Frame Grabber"));
	}
	else
	if (vLocalSystemData->vITIPCDig->vCamera == NULL)
	{
		SetDlgItemText(IDC_ErrorMessage, _T("Error, No Frame Grabber In System"));
		ReportErrorMessage("Frame Grabber not connected, Call Service",cError,0);
	}
	
	if (!vLocalSystemData->vITIPCDig->vImage)
	if (vLocalSystemData->vITIPCDig->vImageBuffer)
	if (ThereIsEnoughMemory(sizeof(CImgConn), "Frame Grabber Interface3"))
		vLocalSystemData->vITIPCDig->vImage = new CImgConn(
			vLocalSystemData->vITIPCDig->vImageBuffer,
			(WORD)vLocalSystemData->vITIPCDig->vDisplayImageSizeX,
			(WORD)vLocalSystemData->vITIPCDig->vDisplayImageSizeY,
			(WORD)vLocalSystemData->vITIPCDig->vBitsPerPixel, 
			vLocalSystemData->vITIPCDig->vWindowHandle);
		
	if (!vMainWindowPointer->HaveRejects(PasswordOK(cTemporaryInspxPassword,false)))//false))
	for (DWORD TempLoop = 0; TempLoop < cMaximumNumberOfInspections; TempLoop++)
	if (vGlobalCurrentProduct->vInspection[TempLoop])
	{
		if (!vInspectionShowing)
			SetInspectionShowing(vGlobalCurrentProduct->vInspection[TempLoop]);
	}
	if (!vInspectionShowing)
	if (vGlobalCurrentProduct->vInspection[0])
		SetInspectionShowing(vGlobalCurrentProduct->vInspection[0]);

	vFromSetupWindow = false;
	TempWindow = FindWindow(NULL, _T("Setup"));
	if (TempWindow) 
		vFromSetupWindow = true;

//if you have a window called setup, then from setup also
	if (((vLocalSystemData->vInAutoSetup) || (vFromSetupWindow)) &&
		(vLocalSystemData->vSystemRunMode != cCalibratingDetectorOffsetMode) &&
		(vLocalSystemData->vSystemRunMode != cCalibratingDetectorEdgesMode))
	{
		vMakeExitButtonGreen = true;
		m_Function1Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		SetDlgItemText(IDC_Function1Button, _T("Return to Auto Setup"));
	}
	else
	{
		CWnd *TempWindow = NULL;
		TempWindow = FindWindow(NULL, _T("Help"));
		if (TempWindow) 
		{
			vMakeExitButtonGreen = true;
			m_Function1Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
			SetDlgItemText(IDC_Function1Button, _T("Exit Diagnostics"));
		}
	}

	OneSecondUpdate();
	UpdateDisplayIndicators();
	if (vMainWindowPointer->HaveRejects(PasswordOK(cTemporaryInspxPassword,false)))//false))
	{
		vHaveAnImageToShow = true;
		m_CurrentInspection.ShowWindow(SW_SHOW);
		m_Background.ShowWindow(SW_HIDE);
		m_ErrorMessage.ShowWindow(SW_HIDE);
		vWindowInitialized = true;
		ShowNextReject(false, false);

		//if not showing a reject, show image on main menu.  If a reject, select that inspection
		if (vLocalSystemData->vITIPCDig->vRejectBufferShowing == -1)
		if (vLocalSystemData->vITIPCDig->vOriginalImage)
		if (vLocalSystemData->vITIPCDig->vOriginalImage->vContainerEjectInspection < 255)
			SetInspectionShowing(vGlobalCurrentProduct->vInspection[vLocalSystemData->vITIPCDig->vOriginalImage->vContainerEjectInspection]);

		vHaveRejects = true;
		UpdateButtons();
	}
	else
	if ((vLocalSystemData->vITIPCDig->vOriginalImage) && (vLocalSystemData->vITIPCDig->vOriginalImage->vTop) && (vLocalSystemData->vITIPCDig->vOriginalImage->vTop <= vLocalSystemData->vITIPCDig->vOriginalBufferSizeX))
	{
		vHaveAnImageToShow = true;
		m_CurrentInspection.ShowWindow(SW_SHOW);
		m_Background.ShowWindow(SW_HIDE);
		m_ErrorMessage.ShowWindow(SW_HIDE);
		ShowNewImageOnDisplay();
	}
	else
		m_CurrentInspection.ShowWindow(SW_HIDE);
	UpdateButtons();
	int TimerResult = SetTimer(vOneSecondTimerHandle,1000,NULL);
	if (!TimerResult)
		ReportErrorMessage("Error-Second Timer Failed",cEMailInspx,32000);
	this->SetWindowText(_T("Image Display"));
	vWindowInitialized = true;
	//vGlobalMouseMoveCounter = 0;

	if (!vProcessedRedImage)
		OnFunction1Button();
	::SetupMenu(vLocalCWndCollection);
}

void CNewImageDisplayDialog::OnPaint() 
{
	double TempStartTime = CIFCOS::GetSystimeMicrosecs();
	CPaintDC dc(this); // device context for painting
	
	WORD TempOffsetX = (WORD) ResizeXCoor(109);
	WORD TempOffsetY = (WORD) ResizeYCoor(43);
	if ((dc.m_ps.rcPaint.top <= vLocalSystemData->vITIPCDig->vDisplayImageSizeY + TempOffsetY) && (dc.m_ps.rcPaint.bottom >= TempOffsetY) &&
		(dc.m_ps.rcPaint.left <= vLocalSystemData->vITIPCDig->vDisplayImageSizeX + TempOffsetX) && (dc.m_ps.rcPaint.right >= TempOffsetX))
	if ((vHaveAnImageToShow) || (vShowAlternateImage))// || (vShowClipboard))
	if (vLocalSystemData->vITIPCDig->vImageBuffer)
	{
		if (vLocalSystemData->vLogFile.vLogSerialData)
			vLocalSystemData->vLogFile.WriteToLogFile("NIDD-OP-Start",cDebugMessage);

		CDC TempCompatableDeviceContext;
		TempCompatableDeviceContext.CreateCompatibleDC(&dc);
		HBITMAP TempBitmapOfReject = CreateCompatibleBitmap ( dc, vLocalSystemData->vITIPCDig->vDisplayImageSizeX, vLocalSystemData->vITIPCDig->vDisplayImageSizeY );
		BITMAPINFO TempBitmapInfo = {0};		//http://stackoverflow.com/questions/17137100/c-changing-the-values-in-a-bitmap
		TempBitmapInfo.bmiHeader.biSize = sizeof(TempBitmapInfo.bmiHeader);
		int TempBitmapColorArraySize = vLocalSystemData->vITIPCDig->vDisplayImageSizeX * vLocalSystemData->vITIPCDig->vDisplayImageSizeY * 4;
		int TempIterator = TempBitmapColorArraySize;

		if(0 == GetDIBits(dc, TempBitmapOfReject, 0, 0, NULL, &TempBitmapInfo, DIB_RGB_COLORS))  //get just bitmap info header
		{
			int r = 0;
		}
		TempBitmapInfo.bmiHeader.biBitCount = 32;  
		TempBitmapInfo.bmiHeader.biCompression = BI_RGB;  

		BYTE* TempBitmapColorArray = new BYTE[TempBitmapColorArraySize];
		GetDIBits(dc, TempBitmapOfReject, 0, TempBitmapInfo.bmiHeader.biHeight, (LPVOID)TempBitmapColorArray, &TempBitmapInfo, DIB_RGB_COLORS);

		BYTE *TempDataPoint = vLocalSystemData->vITIPCDig->vImageBuffer;
		{
			RECT TempRectangle;
			TempRectangle.left = TempOffsetX - 1;
			TempRectangle.top = TempOffsetY - 1;
			TempRectangle.right = TempOffsetX + vLocalSystemData->vITIPCDig->vDisplayImageSizeX + 1;
			TempRectangle.bottom = TempOffsetY + vLocalSystemData->vITIPCDig->vDisplayImageSizeY + 1;
			CBrush TempBrush(cWhite);
			dc.FrameRect(&TempRectangle,&TempBrush);
			TempRectangle.left = TempOffsetX - 2;
			TempRectangle.top = TempOffsetY - 2;
			TempRectangle.right = TempOffsetX + vLocalSystemData->vITIPCDig->vDisplayImageSizeX + 2;
			TempRectangle.bottom = TempOffsetY + vLocalSystemData->vITIPCDig->vDisplayImageSizeY + 2;
			dc.FrameRect(&TempRectangle,&TempBrush);
		}
		if (vLocalConfigurationData->vImageShowLines)
		if (!vRejectsImageDisplay)
		if ((!vLocalConfigurationData->vContinuousFeedContainerRate) && (!vLocalConfigurationData->vBulkProductMode)) //don't draw purple line in bulk mode
		{
			{
				//vDrawPurpleLine = false;
				WORD TempTop = (WORD)(( vGlobalCurrentProduct->vEdgeLocationBottom + vGlobalCurrentProduct->vEdgeLocationHeight) * vGlobalPixelsPerUnitInHeight);
				TempTop = TempTop * vLocalSystemData->vITIPCDig->vDisplayImageSizeY / vLocalSystemData->vITIPCDig->vOriginalBufferSizeX;
				if (vLocalSystemData->vITIPCDig->vDisplayImageSizeY > TempTop)
					TempTop = vLocalSystemData->vITIPCDig->vDisplayImageSizeY - TempTop;
				else
					TempTop = 0;

				WORD TempBottom = (WORD)(vGlobalCurrentProduct->vEdgeLocationBottom * vGlobalPixelsPerUnitInHeight);
				WORD TempHeight = (WORD)((vGlobalCurrentProduct->vEdgeLocationHeight) * vGlobalPixelsPerUnitInHeight);
				TempHeight = TempHeight * vLocalSystemData->vITIPCDig->vDisplayImageSizeY / vLocalSystemData->vITIPCDig->vOriginalBufferSizeX;
				WORD TempHeightIndex = TempHeight * vLocalSystemData->vITIPCDig->vDisplayImageSizeX;
				int TempResizedX3 = ResizeXCoor(3);

				for(WORD TempLoopY = 1; TempLoopY < TempHeight; TempLoopY++)
				{
					//Sleep(0);
					for(WORD TempLoopX = 1; TempLoopX < TempResizedX3; TempLoopX++)
					{
						//if ((TempOffsetX - TempLoopX <= dc.m_ps.rcPaint.right) &&
						//	(TempOffsetX - TempLoopX >= dc.m_ps.rcPaint.left) &&
						//	(TempOffsetY + TempLoopY + TempTop >= dc.m_ps.rcPaint.top) &&
						//	(TempOffsetY + TempLoopY + TempTop <= dc.m_ps.rcPaint.bottom))
						
				//		TempBitmapColorArray[TempHeightIndex + TempLoopY*vLocalSystemData->vITIPCDig->vDisplayImageSizeX * 4 + TempLoopX] = 80;//blue
				//		TempBitmapColorArray[TempHeightIndex + TempLoopY*vLocalSystemData->vITIPCDig->vDisplayImageSizeX * 4 + TempLoopX] = 30;//green
				//		TempBitmapColorArray[TempHeightIndex + TempLoopY*vLocalSystemData->vITIPCDig->vDisplayImageSizeX * 4 + TempLoopX] = 90;//red
				//		TempBitmapColorArray[TempHeightIndex + TempLoopY*vLocalSystemData->vITIPCDig->vDisplayImageSizeX * 4 + TempLoopX] = 255;//alpha
						
						dc.SetPixelV(TempOffsetX - TempLoopX,TempOffsetY + TempLoopY + TempTop, cPurple); //draw pixel in purple
					}
				}
			}
		}
		
		bool TempCanShowInspectionInRed = true;

		if (vShowingProcess)
		if (!vShowThreshold)
			TempCanShowInspectionInRed = false;

		if (vShowAlternateImage)
			TempCanShowInspectionInRed = false;
		if (vHideRedIndicator)
			TempCanShowInspectionInRed = false;

		if (vInspectionShowing)
		{
			if ((vInspectionShowing->vInspectionType == cDensityInspection) ||
				(vInspectionShowing->vInspectionType == cCheckWeighInspection) ||
				(vInspectionShowing->vInspectionType == cMissingLidInspection) ||
				(vInspectionShowing->vInspectionType == cUnderfillInspection) ||
				(vInspectionShowing->vInspectionType == cUnderfillByWeightInspection) ||
				(vInspectionShowing->vInspectionType == cOverfillByWeightInspection) ||
				(vInspectionShowing->vInspectionType == cOverfillInspection) ||
				(vInspectionShowing->vInspectionType == cStandardDeviationInspection))
				if (!vShowThreshold)  //don't add image processings red view if showing processing steps
					TempCanShowInspectionInRed = false;

			//don't show red from a different inspection unless editing the threshold
			if (vDisplayingImageData)
			if (vDisplayingImageData->vContainerEjectInspection < vGlobalCurrentProduct->vNumberOfInspections)
			//if (vGlobalCurrentProduct->vInspection[vDisplayingImageData->vContainerEjectInspection] != vInspectionShowing)
			if (!vDisplayingImageData->vRejectedForThisInspection[vInspectionNumberShowing])  //if this image was rejected for the inspection that is showing
			if (!vShowThreshold)
				TempCanShowInspectionInRed = false;

			if ((vLocalConfigurationData->vRedIndicatorToShowOnMainMenu == cDefectInRedBox) || (vLocalConfigurationData->vRedIndicatorToShowOnMainMenu == cDefectsInRedBoxes)) //if not showing red on all images, then don't show on this one as not correct image
				TempCanShowInspectionInRed = true;
		}
		else
			TempCanShowInspectionInRed = false;

		DWORD TempNumberOfRedPixels = 0;

		if ((vShowSubtractAverage) || (vShowAddStructure) || (vFillInLightSpots) || (vDisplayEnhancedImage))  
			//don't add image processings red view if showing processing steps
		if (!vShowThreshold)  //don't add image processings red view if showing processing steps
				TempCanShowInspectionInRed = false;

		if ((vShowDeltaCurrentPreviewStructure) && (!vHideRedIndicator))
		{ //colors for current to improve learn preview delta
			//0 - black. 1-10 - blue. 11-20 - green. 21-30 - yellow. 31-50 - orange. 51 and up - white
			//TempText = TempText + " 0=bla,1-5=blu,6-10=g,11-20=y,21-40=o,41-255=w,larger=brighter";
			TempIterator = TempBitmapColorArraySize - vLocalSystemData->vITIPCDig->vDisplayImageSizeX * 4;
			for (WORD TempLoopY = 0; TempLoopY < vLocalSystemData->vITIPCDig->vDisplayImageSizeY; TempLoopY++)
			{
				for (WORD TempLoopX = 0; TempLoopX < vLocalSystemData->vITIPCDig->vDisplayImageSizeX; TempLoopX++)
				{
					if ((TempOffsetX + TempLoopX <= dc.m_ps.rcPaint.right) &&
						(TempOffsetX + TempLoopX >= dc.m_ps.rcPaint.left) &&
						(TempOffsetY + TempLoopY >= dc.m_ps.rcPaint.top) &&
						(TempOffsetY + TempLoopY <= dc.m_ps.rcPaint.bottom))
					{
						if (*TempDataPoint < 2) //black
						{
							TempBitmapColorArray[TempIterator] = 0;//blUE
							TempBitmapColorArray[TempIterator + 1] = 0;//green
							TempBitmapColorArray[TempIterator + 2] = 0;//red
							TempBitmapColorArray[TempIterator + 3] = 255;//alpha
						//	dc.SetPixelV(TempOffsetX + TempLoopX,TempOffsetY + TempLoopY, 
							//	RGB(0,0,0)); //draw pixel in black
						}
						else
						if (*TempDataPoint > 40) //white
						{
							TempBitmapColorArray[TempIterator] = 255;//blUE
							TempBitmapColorArray[TempIterator + 1] = 255;//green
							TempBitmapColorArray[TempIterator + 2] = 255;//red
							TempBitmapColorArray[TempIterator + 3] = 255;//alpha
//							dc.SetPixelV(TempOffsetX + TempLoopX,TempOffsetY + TempLoopY, 
	//							RGB(0xFF,0xFF,0xFF)); //draw pixel in white
						}
						else
						if (*TempDataPoint < 6) //light-5, dark-1 for blue
						{
							TempBitmapColorArray[TempIterator] = 255;//blUE
							TempBitmapColorArray[TempIterator + 1] = 0;//green
							TempBitmapColorArray[TempIterator + 2] = 0;//red
							TempBitmapColorArray[TempIterator + 3] = 255;//alpha
					//		dc.SetPixelV(TempOffsetX + TempLoopX,TempOffsetY + TempLoopY, 
						//		RGB(0,0,255));//draw pixel in blue
						}
						else
						if (*TempDataPoint < 11) //light-10, dark-6 for green
						{
							TempBitmapColorArray[TempIterator] = 0;//blUE
							TempBitmapColorArray[TempIterator + 1] = 255;//green
							TempBitmapColorArray[TempIterator + 2] = 0;//red
							TempBitmapColorArray[TempIterator + 3] = 255;//alpha
					//		dc.SetPixelV(TempOffsetX + TempLoopX,TempOffsetY + TempLoopY, 
						//		RGB(0,255,0));//draw pixel in green
						}
						else
						if (*TempDataPoint < 21) //light-20, dark-11 for yellow
						{
							TempBitmapColorArray[TempIterator] = 0;//blUE
							TempBitmapColorArray[TempIterator + 1] = 255;//green
							TempBitmapColorArray[TempIterator + 2] = 255;//red
							TempBitmapColorArray[TempIterator + 3] = 255;//alpha
						//	dc.SetPixelV(TempOffsetX + TempLoopX,TempOffsetY + TempLoopY, 
							//	RGB(0xFF,0xFF,0)); //draw pixel yellow
						}
						else
						{ //light-40, dark-21
							TempBitmapColorArray[TempIterator] = 0;//blUE
							TempBitmapColorArray[TempIterator + 1] = 0;//green
							TempBitmapColorArray[TempIterator + 2] = 255;//red
							TempBitmapColorArray[TempIterator + 3] = 255;//alpha
					//		dc.SetPixelV(TempOffsetX + TempLoopX,TempOffsetY + TempLoopY, 
						//		RGB(255,0,0)); //draw pixel in orange
						}
					}
					TempDataPoint++;
					TempIterator = TempIterator + 4;
				}
				TempIterator = TempIterator - vLocalSystemData->vITIPCDig->vDisplayImageSizeX * 8;
			}
		}
		else
		if (((((vShowStructure) || (vShowOriginalStructure) || (vShowImproveLearnPreview)) && (vShowType != cShowTypeContaminant2) && (vShowType != cShowTypeContaminant2NoDilation)) ||
			(vShowDeltaOriginalToCurrentStructure)) && (!vHideRedIndicator))
		{//show colors for original structure
			TempIterator = TempBitmapColorArraySize - vLocalSystemData->vITIPCDig->vDisplayImageSizeX * 4;
			for (WORD TempLoopY = 0; TempLoopY < vLocalSystemData->vITIPCDig->vDisplayImageSizeY; TempLoopY++)
			{
				for (WORD TempLoopX = 0; TempLoopX < vLocalSystemData->vITIPCDig->vDisplayImageSizeX; TempLoopX++)
				{
					if ((TempOffsetX + TempLoopX <= dc.m_ps.rcPaint.right) && (TempOffsetX + TempLoopX >= dc.m_ps.rcPaint.left) &&
						(TempOffsetY + TempLoopY >= dc.m_ps.rcPaint.top) && (TempOffsetY + TempLoopY <= dc.m_ps.rcPaint.bottom))
					{
						if (*TempDataPoint < 2)
						{	
							TempBitmapColorArray[TempIterator] = 0;//blUE
							TempBitmapColorArray[TempIterator + 1] = 0;//green
							TempBitmapColorArray[TempIterator + 2] = 0;//red
							TempBitmapColorArray[TempIterator + 3] = 255;//alpha
							
							//	dc.SetPixelV(TempOffsetX + TempLoopX,TempOffsetY + TempLoopY, 
							//	RGB(0,0,0)); //draw pixel in black
						}
						else
						if (*TempDataPoint > 126)
						{
							TempBitmapColorArray[TempIterator] = 255;//blUE
							TempBitmapColorArray[TempIterator + 1] = 255;//green
							TempBitmapColorArray[TempIterator + 2] = 255;//red
							TempBitmapColorArray[TempIterator + 3] = 255;//alpha
						//	dc.SetPixelV(TempOffsetX + TempLoopX,TempOffsetY + TempLoopY, 
							//	RGB(0xFF,0xFF,0xFF)); //draw pixel in white
						}
						else
						if (*TempDataPoint < 21) //light-20, dark-1 for blue
						{
							BYTE TempColor = 255 - ((20 - *TempDataPoint) * 2); //want light-255, dark-210
							BYTE TempColor1 = *TempDataPoint * 10;  //want light-200, dark-0
							
							TempBitmapColorArray[TempIterator] = TempColor;//blUE
							TempBitmapColorArray[TempIterator + 1] = TempColor1;//green
							TempBitmapColorArray[TempIterator + 2] = TempColor1;//red
							TempBitmapColorArray[TempIterator + 3] = 255;//alpha
						//	dc.SetPixelV(TempOffsetX + TempLoopX,TempOffsetY + TempLoopY, 
							//	RGB(TempColor1,TempColor1,TempColor));//draw pixel in blue
						}
						else
						if (*TempDataPoint < 31) //light-30, dark-21 for green
						{
							BYTE TempColor = (*TempDataPoint * 15) - 300; //want light-150, dark-0
							BYTE TempColor1 = (*TempDataPoint * 10) - 50;  //want light-255, dark-150
							TempBitmapColorArray[TempIterator] = TempColor;//blUE
							TempBitmapColorArray[TempIterator + 1] = TempColor1;//green
							TempBitmapColorArray[TempIterator + 2] = TempColor;//red
							TempBitmapColorArray[TempIterator + 3] = 255;//alpha
						//	dc.SetPixelV(TempOffsetX + TempLoopX,TempOffsetY + TempLoopY, 
							//	RGB(TempColor,TempColor1,TempColor));//draw pixel in green
						}
						else
						if (*TempDataPoint < 51) //light-50, dark-31 for yellow
						{
							BYTE TempColor = (*TempDataPoint * 9) - 275;  //want light-175, dark 0

							TempBitmapColorArray[TempIterator] = TempColor;//blUE
							TempBitmapColorArray[TempIterator + 1] = 255;//green
							TempBitmapColorArray[TempIterator + 2] = 255;//red
							TempBitmapColorArray[TempIterator + 3] = 255;//alpha
	//						dc.SetPixelV(TempOffsetX + TempLoopX,TempOffsetY + TempLoopY, 
		//						RGB(0xFF,0xFF,TempColor)); //draw pixel yellow
						}
						else
						{ //light-126, dark-51
							BYTE TempColor = (*TempDataPoint / 2) + 195;  //want light-255, dark 220
							BYTE TempColor1 = (*TempDataPoint) + 40;  //want light-150, dark 100
							BYTE TempColor2 = (*TempDataPoint) - 50;  //want light-50, dark 0

							TempBitmapColorArray[TempIterator] = TempColor2;//blUE
							TempBitmapColorArray[TempIterator + 1] = TempColor1;//green
							TempBitmapColorArray[TempIterator + 2] = TempColor;//red
							TempBitmapColorArray[TempIterator + 3] = 255;//alpha
//							dc.SetPixelV(TempOffsetX + TempLoopX,TempOffsetY + TempLoopY, 
	//							RGB(TempColor,TempColor1,TempColor2)); //draw pixel in orange
						}
					}
					TempDataPoint++;
					TempIterator = TempIterator + 4;
				}
				TempIterator = TempIterator - vLocalSystemData->vITIPCDig->vDisplayImageSizeX * 8;
			}
		}
		else
		if (((vShowStructure) || (vShowOriginalStructure) || (vShowImproveLearnPreview)) && ((vShowType == cShowTypeContaminant2) || (vShowType == cShowTypeContaminant2NoDilation)) && (!vHideRedIndicator))
		{  //show colors for contaminant 2 structure (new type)
			//0 = black, 1-45 = blue, 46-90 = green, 91-139 = yellow, 140-254 = orange, 255 = white
			TempIterator = TempBitmapColorArraySize - vLocalSystemData->vITIPCDig->vDisplayImageSizeX * 4;
			for (WORD TempLoopY = 0; TempLoopY < vLocalSystemData->vITIPCDig->vDisplayImageSizeY; TempLoopY++)
			{
				for (WORD TempLoopX = 0; TempLoopX < vLocalSystemData->vITIPCDig->vDisplayImageSizeX; TempLoopX++)
				{
					if ((TempOffsetX + TempLoopX <= dc.m_ps.rcPaint.right) &&
						(TempOffsetX + TempLoopX >= dc.m_ps.rcPaint.left) &&
						(TempOffsetY + TempLoopY >= dc.m_ps.rcPaint.top) &&
						(TempOffsetY + TempLoopY <= dc.m_ps.rcPaint.bottom))
					{
						if (*TempDataPoint < 2)
						{
							TempBitmapColorArray[TempIterator] = 0;//blUE
							TempBitmapColorArray[TempIterator + 1] = 0;//green
							TempBitmapColorArray[TempIterator + 2] = 0;//red
							TempBitmapColorArray[TempIterator + 3] = 255;//alpha
							//dc.SetPixelV(TempOffsetX + TempLoopX,TempOffsetY + TempLoopY, RGB(0,0,0)); //draw pixel in black
						}
						else
						if (*TempDataPoint >= 253)
						{
							TempBitmapColorArray[TempIterator] = 255;//blUE
							TempBitmapColorArray[TempIterator + 1] = 255;//green
							TempBitmapColorArray[TempIterator + 2] = 255;//red
							TempBitmapColorArray[TempIterator + 3] = 255;//alpha
						//	dc.SetPixelV(TempOffsetX + TempLoopX,TempOffsetY + TempLoopY, RGB(0xFF,0xFF,0xFF)); //draw pixel in white
						}
						else
						if (*TempDataPoint < 46) //light-45, dark-1 for blue
						{
							BYTE TempColor = 230 - ((45 - *TempDataPoint)); //want light-255, dark-210
							BYTE TempColor1 = *TempDataPoint * 5;  //want light-200, dark-0

							TempBitmapColorArray[TempIterator] = TempColor;//blUE
							TempBitmapColorArray[TempIterator + 1] = TempColor1;//green
							TempBitmapColorArray[TempIterator + 2] = TempColor1;//red
							TempBitmapColorArray[TempIterator + 3] = 255;//alpha
						//	dc.SetPixelV(TempOffsetX + TempLoopX,TempOffsetY + TempLoopY, RGB(TempColor1,TempColor1,TempColor));//draw pixel in blue
						}
						else
						if (*TempDataPoint < 91) //90-light green, 46-dark green
						{
							BYTE TempColor = (*TempDataPoint * 3) - 120; //want light-150, dark-0
							BYTE TempColor1 = (*TempDataPoint * 2) + 60;  //want light-255, dark-150
							
							TempBitmapColorArray[TempIterator] = TempColor;//blUE
							TempBitmapColorArray[TempIterator + 1] = TempColor1;//green
							TempBitmapColorArray[TempIterator + 2] = TempColor;//red
							TempBitmapColorArray[TempIterator + 3] = 255;//alpha
						//	dc.SetPixelV(TempOffsetX + TempLoopX,TempOffsetY + TempLoopY, RGB(TempColor,TempColor1,TempColor));//draw pixel in green
								//RGB(0xFF,0xFF,0xFF));//draw pixel in green
						}
						else
						if (*TempDataPoint < 140) //light-139, dark-91 for yellow				
						{
							BYTE TempColor = (*TempDataPoint * 3) - 270;  //want light-175, dark 0

							TempBitmapColorArray[TempIterator] = TempColor;//blUE
							TempBitmapColorArray[TempIterator + 1] = 255;//green
							TempBitmapColorArray[TempIterator + 2] = 255;//red
							TempBitmapColorArray[TempIterator + 3] = 255;//alpha
//							dc.SetPixelV(TempOffsetX + TempLoopX,TempOffsetY + TempLoopY, RGB(0xFF,0xFF,TempColor)); //draw pixel yellow
						}
						else
						{  //light-254, dark-136									//light-126, dark-51
							BYTE TempColor = (*TempDataPoint / 3) + 170;  //want light-255, dark 220
							BYTE TempColor1 = (*TempDataPoint / 2) + 25;  //want light-150, dark 100
							BYTE TempColor2 = (*TempDataPoint / 2) - 65;  //want light-50, dark 0

							TempBitmapColorArray[TempIterator] = TempColor2;//blUE
							TempBitmapColorArray[TempIterator + 1] = TempColor1;//green
							TempBitmapColorArray[TempIterator + 2] = TempColor;//red
							TempBitmapColorArray[TempIterator + 3] = 255;//alpha
					//		dc.SetPixelV(TempOffsetX + TempLoopX,TempOffsetY + TempLoopY, RGB(TempColor,TempColor1,TempColor2)); //draw pixel in orange
						}
					}
					TempDataPoint++;
					TempIterator = TempIterator + 4;
				}
				TempIterator = TempIterator - vLocalSystemData->vITIPCDig->vDisplayImageSizeX * 8;
			}
		}
		else
/*
		if (((vShowStructure) || (vShowOriginalStructure) || (vShowImproveLearnPreview)) && (vShowType == cShowTypeContaminant2) && 
			(!vHideRedIndicator))
		{  //show colors for contaminant 2 structure (new type)
			for (WORD TempLoopY = 0; TempLoopY < vLocalSystemData->vITIPCDig->vDisplayImageSizeY; TempLoopY++)
			{
				for (WORD TempLoopX = 0; TempLoopX < vLocalSystemData->vITIPCDig->vDisplayImageSizeX; TempLoopX++)
				{
					if ((TempOffsetX + TempLoopX <= dc.m_ps.rcPaint.right) &&
						(TempOffsetX + TempLoopX >= dc.m_ps.rcPaint.left) &&
						(TempOffsetY + TempLoopY >= dc.m_ps.rcPaint.top) &&
						(TempOffsetY + TempLoopY <= dc.m_ps.rcPaint.bottom))
					{
						if (*TempDataPoint < 2)
							dc.SetPixelV(TempOffsetX + TempLoopX,TempOffsetY + TempLoopY, 
								RGB(0,0,0)); //draw pixel in black
						else
						if (*TempDataPoint > 126)
							dc.SetPixelV(TempOffsetX + TempLoopX,TempOffsetY + TempLoopY, 
								RGB(0xFF,0xFF,0xFF)); //draw pixel in white
						else
						if (*TempDataPoint < 21) //light-20, dark-1 for blue
						{
							BYTE TempColor = 255 - ((20 - *TempDataPoint) * 2); //want light-255, dark-210
							BYTE TempColor1 = *TempDataPoint * 10;  //want light-200, dark-0
							dc.SetPixelV(TempOffsetX + TempLoopX,TempOffsetY + TempLoopY, 
								RGB(TempColor1,TempColor1,TempColor));//draw pixel in blue
						}
						else
						if (*TempDataPoint < 31) //light-30, dark-21 for green
						{
							BYTE TempColor = (*TempDataPoint * 15) - 300; //want light-150, dark-0
							BYTE TempColor1 = (*TempDataPoint * 10) - 50;  //want light-255, dark-150
							dc.SetPixelV(TempOffsetX + TempLoopX,TempOffsetY + TempLoopY, 
								RGB(TempColor,TempColor1,TempColor));//draw pixel in green
								//RGB(0xFF,0xFF,0xFF));//draw pixel in green
						}
						else
						if (*TempDataPoint < 51) //light-50, dark-31 for yellow
						{
							BYTE TempColor = (*TempDataPoint * 9) - 275;  //want light-175, dark 0
							dc.SetPixelV(TempOffsetX + TempLoopX,TempOffsetY + TempLoopY, 
								RGB(0xFF,0xFF,TempColor)); //draw pixel yellow
						}
						else
						{ //light-126, dark-51
							BYTE TempColor = (*TempDataPoint / 2) + 195;  //want light-255, dark 220
							BYTE TempColor1 = (*TempDataPoint) + 40;  //want light-150, dark 100
							BYTE TempColor2 = (*TempDataPoint) - 50;  //want light-50, dark 0
							dc.SetPixelV(TempOffsetX + TempLoopX,TempOffsetY + TempLoopY, 
								RGB(TempColor,TempColor1,TempColor2)); //draw pixel in orange
						}
					}
					TempDataPoint++;
				}
			}
		}
		else
*/
		//enum {cDefectsInNoColor, cDefectsInPseudoColor, cDefectsInRed, cDefectInRedBox, cDefectsInRedBoxes};
		//draw red pixels in image
		if (((vLocalConfigurationData->vRedIndicatorToShowOnMainMenu == cDefectsInRed) || 
			(vLocalConfigurationData->vRedIndicatorToShowOnMainMenu == cDefectInRedBox) ||
			(vLocalConfigurationData->vRedIndicatorToShowOnMainMenu == cDefectsInRedBoxes) ||// (vShowThreshold) ||
			((vShowThreshold) && (vLocalConfigurationData->vRedIndicatorToShowOnMainMenu != cDefectsInPseudoColor))) && 
			(((TempCanShowInspectionInRed) && (!vShowAlternateImage))))
		{
			TempIterator = TempBitmapColorArraySize - vLocalSystemData->vITIPCDig->vDisplayImageSizeX * 4;
			for (WORD TempLoopY = 0; TempLoopY < vLocalSystemData->vITIPCDig->vDisplayImageSizeY; TempLoopY++)
			{
				//Sleep(0);
				for (WORD TempLoopX = 0; TempLoopX < vLocalSystemData->vITIPCDig->vDisplayImageSizeX; TempLoopX++)
				{
					//if showing defects in red as long as not showing thresholding, 
					//change normal image zero to one so will not be red
					//
					if (*TempDataPoint == 0)
					{
						if ((TempOffsetX + TempLoopX <= dc.m_ps.rcPaint.right) &&
							(TempOffsetX + TempLoopX >= dc.m_ps.rcPaint.left) &&
							(TempOffsetY + TempLoopY >= dc.m_ps.rcPaint.top) &&
							(TempOffsetY + TempLoopY <= dc.m_ps.rcPaint.bottom))
						{
							TempBitmapColorArray[TempIterator] = 0;//blUE
							TempBitmapColorArray[TempIterator + 1] = 0;//green
							TempBitmapColorArray[TempIterator + 2] = 255;//red
							TempBitmapColorArray[TempIterator + 3] = 255;//alpha
						//	dc.SetPixelV(TempOffsetX + TempLoopX,TempOffsetY + TempLoopY, RGB(0xFF,0,0)); //draw pixel in red
							TempNumberOfRedPixels++;
						}
					}
					else
					if (*TempDataPoint == 1)
					{
						if ((TempOffsetX + TempLoopX <= dc.m_ps.rcPaint.right) &&
							(TempOffsetX + TempLoopX >= dc.m_ps.rcPaint.left) &&
							(TempOffsetY + TempLoopY >= dc.m_ps.rcPaint.top) &&
							(TempOffsetY + TempLoopY <= dc.m_ps.rcPaint.bottom))
						{
							TempBitmapColorArray[TempIterator] = 255;//blUE
							TempBitmapColorArray[TempIterator + 1] = 0;//green
							TempBitmapColorArray[TempIterator + 2] = 0;//red
							TempBitmapColorArray[TempIterator + 3] = 255;//alpha
							TempNumberOfRedPixels++;
						}
					}
					else
					{
						if ((TempOffsetX + TempLoopX <= dc.m_ps.rcPaint.right) &&
							(TempOffsetX + TempLoopX >= dc.m_ps.rcPaint.left) &&
							(TempOffsetY + TempLoopY >= dc.m_ps.rcPaint.top) &&
							(TempOffsetY + TempLoopY <= dc.m_ps.rcPaint.bottom))
						{
							TempBitmapColorArray[TempIterator] = *TempDataPoint;//blUE
							TempBitmapColorArray[TempIterator + 1] = *TempDataPoint;//green
							TempBitmapColorArray[TempIterator + 2] = *TempDataPoint;//red
							TempBitmapColorArray[TempIterator + 3] = 255;//alpha
						//	dc.SetPixelV(TempOffsetX + TempLoopX,TempOffsetY + TempLoopY, 
							//	RGB(*TempDataPoint,*TempDataPoint,*TempDataPoint)); //draw pixel in intensity
						}
					}
					TempDataPoint++;
					TempIterator = TempIterator + 4;
				}
				TempIterator = TempIterator - vLocalSystemData->vITIPCDig->vDisplayImageSizeX * 8;  //multiply by 8, 4 points per pixel, go back two rows and forward one
			}
		}
		else
		{ 
			if ((vShowThreshold) && (vLocalConfigurationData->vRedIndicatorToShowOnMainMenu != cDefectsInPseudoColor) && (!vHideRedIndicator))
			{ //blank old grey image
				TempIterator = TempBitmapColorArraySize - vLocalSystemData->vITIPCDig->vDisplayImageSizeX * 4;
				for (WORD TempLoopY = 0; TempLoopY < vLocalSystemData->vITIPCDig->vDisplayImageSizeY; TempLoopY++)
				{
					for (WORD TempLoopX = 0; TempLoopX < vLocalSystemData->vITIPCDig->vDisplayImageSizeX; TempLoopX++)
					{
						if ((TempOffsetX + TempLoopX <= dc.m_ps.rcPaint.right) &&
							(TempOffsetX + TempLoopX >= dc.m_ps.rcPaint.left) &&
							(TempOffsetY + TempLoopY >= dc.m_ps.rcPaint.top) &&
							(TempOffsetY + TempLoopY <= dc.m_ps.rcPaint.bottom))
						if (*TempDataPoint) //if pixel was a line
						{
							TempBitmapColorArray[TempIterator] = 255;//blUE
							TempBitmapColorArray[TempIterator + 1] = 255;//green
							TempBitmapColorArray[TempIterator + 2] = 255;//red
							TempBitmapColorArray[TempIterator + 3] = 255;//alpha
					//		dc.SetPixelV(TempOffsetX + TempLoopX,TempOffsetY + TempLoopY,0x00FFFFFF); 
						}
						else
						{
							TempBitmapColorArray[TempIterator] = 0;//blUE
							TempBitmapColorArray[TempIterator + 1] = 0;//green
							TempBitmapColorArray[TempIterator + 2] = 0;//red
							TempBitmapColorArray[TempIterator + 3] = 255;//alpha
					//		dc.SetPixelV(TempOffsetX + TempLoopX,TempOffsetY + TempLoopY,0x00000000); 
						}
						TempDataPoint++;
						TempIterator = TempIterator + 4;
					}
					TempIterator = TempIterator - vLocalSystemData->vITIPCDig->vDisplayImageSizeX * 8;
				}
			}
			else
			{//draw grey image
				TempIterator = TempBitmapColorArraySize - vLocalSystemData->vITIPCDig->vDisplayImageSizeX * 4;
				for (WORD TempLoopY = 0; TempLoopY < vLocalSystemData->vITIPCDig->vDisplayImageSizeY; TempLoopY++)
				{
					for (WORD TempLoopX = 0; TempLoopX < vLocalSystemData->vITIPCDig->vDisplayImageSizeX; TempLoopX++)
					{
						if ((TempOffsetX + TempLoopX <= dc.m_ps.rcPaint.right) &&
							(TempOffsetX + TempLoopX >= dc.m_ps.rcPaint.left) &&
							(TempOffsetY + TempLoopY >= dc.m_ps.rcPaint.top) &&
							(TempOffsetY + TempLoopY <= dc.m_ps.rcPaint.bottom))
						{
							TempBitmapColorArray[TempIterator] = *TempDataPoint;//blUE
							TempBitmapColorArray[TempIterator + 1] = *TempDataPoint;//green
							TempBitmapColorArray[TempIterator + 2] = *TempDataPoint;//red
							TempBitmapColorArray[TempIterator + 3] = 255;//alpha
					//		dc.SetPixelV(TempOffsetX + TempLoopX,TempOffsetY + TempLoopY, 
						//		RGB(*TempDataPoint,*TempDataPoint,*TempDataPoint)); //draw pixel in intensity
						}
						//	TempNumberOfRedPixels++;
						TempDataPoint++;
						TempIterator = TempIterator + 4;
					}
					TempIterator = TempIterator - vLocalSystemData->vITIPCDig->vDisplayImageSizeX * 8;
				}
			}
			//draw defects in pseudo color
			//enum {cDefectsInNoColor, cDefectsInPseudoColor, cDefectsInRed, cDefectInRedBox, cDefectsInRedBoxes};
			if (((vLocalConfigurationData->vRedIndicatorToShowOnMainMenu == cDefectsInPseudoColor) ||
				((vShowingProcess) && ((vLocalConfigurationData->vRedIndicatorToShowOnMainMenu == cDefectInRedBox) ||
				(vLocalConfigurationData->vRedIndicatorToShowOnMainMenu == cDefectsInRedBoxes)))) && 
				(TempCanShowInspectionInRed) && (!vHideRedIndicator))
			if ((vDisplayingImageData) && (vDisplayingImageData->vRedImage))
			if ((vDisplayingImageData->vContainerEjectInspection < vGlobalCurrentProduct->vNumberOfInspections) || (vShowThreshold))
			if (!vShowAlternateImage)
			//if (vShowingProcess == 0)
			{
				double TempScaleX = ((double)vLocalSystemData->vITIPCDig->vOriginalBufferSizeY / 
					(double)vLocalSystemData->vITIPCDig->vDisplayImageSizeX);
				double TempScaleY = ((double)vLocalSystemData->vITIPCDig->vOriginalBufferSizeX / 
					(double)vLocalSystemData->vITIPCDig->vDisplayImageSizeY);

				long TempDelta = 0;
				BYTE TempThreshold = 254;
				if (vShowThreshold)
					TempThreshold = (BYTE)vThreshold;
				else
				if (vInspectionShowing)
					TempThreshold = vInspectionShowing->vThreshold;
				//BYTE TempThresholdAddAmount = 255 - TempThreshold - 1;

				int TempSideReferenceAmount = vDisplayingImageData->vLeftReferenceAdjust;
				if (vShowingProcess)
					TempSideReferenceAmount = 0;

				//BYTE *TempDataPoint = NULL;
				BYTE *TempRedImage = vDisplayingImageData->vRedImage; //show red from when container processed and ejected
				if (vShowThreshold)
					TempRedImage = vProcessedRedImage;  //must show Reject Menu processed image for pseudo color
				TempIterator = TempBitmapColorArraySize - vLocalSystemData->vITIPCDig->vDisplayImageSizeX * 4;
				for (WORD TempLoopY = 0; TempLoopY < vLocalSystemData->vITIPCDig->vDisplayImageSizeY; TempLoopY++)
				{
					for (WORD TempLoopX = 0; TempLoopX < vLocalSystemData->vITIPCDig->vDisplayImageSizeX; TempLoopX++)
					{
						//	if (vDisplayingImageData->vLeftReferenceAdjust < TempLoopY)
						TempDelta = ((DWORD)(((double)TempScaleY *  
							(double)(vLocalSystemData->vITIPCDig->vDisplayImageSizeY - 1 - TempLoopY))) + //number of pixels up this line
							(vLocalSystemData->vITIPCDig->vOriginalBufferSizeX * 
							(DWORD)(((double)((TempLoopX) * 
							(double)TempScaleX) - TempSideReferenceAmount)))); //number of lines up the frame

						//if (TempDelta >= vLocalSystemData->vITIPCDig->vOriginalBufferSize)
						//{
						//	TempDelta = vLocalSystemData->vITIPCDig->vOriginalBufferSize - 1;
							//ReportErrorMessage("Error-Scaling Problem 1", cEMailInspx,32000);
						//}
						TempDataPoint = TempRedImage + TempDelta;

						if (TempDelta > 0)
						if (TempDelta < (long)vLocalSystemData->vITIPCDig->vOriginalBufferSize)
						if (*TempDataPoint < TempThreshold)
						{
							BYTE TempAmountPixelBelowThreshold = TempThreshold - *TempDataPoint;

							if ((TempOffsetX + TempLoopX <= dc.m_ps.rcPaint.right) &&
									(TempOffsetX + TempLoopX >= dc.m_ps.rcPaint.left) &&
									(TempOffsetY + TempLoopY >= dc.m_ps.rcPaint.top) &&
									(TempOffsetY + TempLoopY <= dc.m_ps.rcPaint.bottom))
							{
								if (TempAmountPixelBelowThreshold < 16)
								{	
									TempBitmapColorArray[TempIterator] = 255;//blUE
									TempBitmapColorArray[TempIterator + 1] = 0;//green
									TempBitmapColorArray[TempIterator + 2] = 0;//red
									TempBitmapColorArray[TempIterator + 3] = 255;//alpha
								//	dc.SetPixelV(TempOffsetX + TempLoopX, TempOffsetY + TempLoopY, cBlue); //draw pixel in blue
								}
								else
								if (TempAmountPixelBelowThreshold < 36)
								{
									TempBitmapColorArray[TempIterator] = 0;//blUE
									TempBitmapColorArray[TempIterator + 1] = 255;//green
									TempBitmapColorArray[TempIterator + 2] = 0;//red
									TempBitmapColorArray[TempIterator + 3] = 255;//alpha
								//	dc.SetPixelV(TempOffsetX + TempLoopX, TempOffsetY + TempLoopY, cGreen); //draw pixel in green
								}
								else
								if (TempAmountPixelBelowThreshold < 66)
								{
									TempBitmapColorArray[TempIterator] = 0;//blUE
									TempBitmapColorArray[TempIterator + 1] = 128;//green
									TempBitmapColorArray[TempIterator + 2] = 255;//red
									TempBitmapColorArray[TempIterator + 3] = 255;//alpha
								//	dc.SetPixelV(TempOffsetX + TempLoopX, TempOffsetY + TempLoopY, cOrange); //draw pixel in yellow
								}
								else
								{//if (TempAmountPixelBelowThreshold < 255)
									TempBitmapColorArray[TempIterator] = 0;//blUE
									TempBitmapColorArray[TempIterator + 1] = 0;//green
									TempBitmapColorArray[TempIterator + 2] = 255;//red
									TempBitmapColorArray[TempIterator + 3] = 255;//alpha
									//dc.SetPixelV(TempOffsetX + TempLoopX, TempOffsetY + TempLoopY, cRed); //draw pixel in red
								}
							}
						}
						TempIterator = TempIterator + 4;
					}
					TempIterator = TempIterator - vLocalSystemData->vITIPCDig->vDisplayImageSizeX * 8;
				}
			}
		}
		if (vLocalSystemData->vLogFile.vLogSerialData)
			vLocalSystemData->vLogFile.WriteToLogFile("NIDD-OP-End",cDebugMessage);
		SetDIBits(dc, TempBitmapOfReject, 0, TempBitmapInfo.bmiHeader.biHeight, (LPVOID)TempBitmapColorArray, &TempBitmapInfo, DIB_RGB_COLORS);
		
		TempCompatableDeviceContext.SelectObject(TempBitmapOfReject);
		dc.BitBlt(TempOffsetX, TempOffsetY, TempBitmapInfo.bmiHeader.biWidth, TempBitmapInfo.bmiHeader.biHeight, &TempCompatableDeviceContext, 0, 0, SRCCOPY);
		
		free(TempBitmapColorArray);
		DeleteObject(TempCompatableDeviceContext);
		DeleteObject(TempBitmapOfReject);
		//Paint sensitivities 
		//blank area on screen
		if ((vSensitivity <= 100) && (vShowThresholdMenu))
			dc.SelectObject(vLocalSystemData->vGreyPen);
		else
			dc.SelectObject(vLocalSystemData->vLightGreyPen);

		for (BYTE TempLoop = 0; TempLoop < vSensitivityHeight; TempLoop++) //draw background of threshold sensitivity
		{
			dc.MoveTo(vSensitivityLeft, vSensitivityBottom - TempLoop);
			dc.LineTo(vSensitivityRight, vSensitivityBottom - TempLoop); //draw background
		}

		if (vLocalSystemData->vLearnSensitivities[vInspectionNumberShowing] <= 100)
		{
			if ((vEditROIBoundaries) || (vShowThresholdMenu))
				dc.SelectObject(vLocalSystemData->vGreyPen);
			else
				dc.SelectObject(vLocalSystemData->vLightGreyPen);

			for (BYTE TempLoop = 0; TempLoop < vSensitivityHeight; TempLoop++) //draw background of learn sensitivity
			{
				dc.MoveTo(vSensitivityLeft, vLearnSensitivityBottom - TempLoop);
				dc.LineTo(vSensitivityRight, vLearnSensitivityBottom - TempLoop); //draw background
			}
		}

		if ((vGlobalCurrentProduct) && ((vShowThresholdMenu) || (vEditROIBoundaries)))
		{
			//draw inspection sensitivity 
			if (vInspectionShowing)
			{
				//draw threshold sensitivity
				if (vSensitivity < 0xFF)
				if (vShowThresholdMenu)
				{
					if (vSensitivity >= 61)
						dc.SelectObject(vLocalSystemData->vMediumDarkGreenPen);
					else
					if (vSensitivity >= 31)
						dc.SelectObject(vLocalSystemData->vYellowPen);
					else
						dc.SelectObject(vLocalSystemData->vRedPen);

					WORD TempBarHeight = (WORD)ResizeXCoor((int)vSensitivity);
					BYTE Temp15XResized = (BYTE)ResizeXCoor(15);
					WORD Temp100XResized = (WORD)ResizeXCoor(100);
					if (TempBarHeight < Temp15XResized) //don't have the bar less than 6 pixels in height so can be easily seen
						TempBarHeight = Temp15XResized;
					if ((vThreshold == 0) || (vThreshold == 255)) //if threshold is zero, make the bar the full height
						TempBarHeight = Temp100XResized;
				//	if ((vGlobalCurrentProduct->vInspection[vInspectionNumberShowing]->vThreshold == 0) || (vGlobalCurrentProduct->vInspection[vInspectionNumberShowing]->vThreshold == 255)) //if threshold is zero, make the bar the full height
				//		TempBarHeight = Temp100XResized;
				//	if (vThreshold == 0) //if threshold is zero, make the bar the full height
					//	TempBarHeight = Temp100XResized;

					for (BYTE TempLoop = 0; TempLoop < vSensitivityHeight; TempLoop++)
					{
						dc.MoveTo(vSensitivityLeft, vSensitivityBottom - TempLoop);
						dc.LineTo(vSensitivityLeft + (int)TempBarHeight, vSensitivityBottom - TempLoop); //draw sensitivity
					}
				}

				//draw learn sensitivity
				if (vLocalSystemData->vLearnSensitivities[vInspectionNumberShowing] < 0xFF)
				{
					if (vLocalSystemData->vLearnSensitivities[vInspectionNumberShowing] >= 61)
						dc.SelectObject(vLocalSystemData->vMediumDarkGreenPen);
					else
					if (vLocalSystemData->vLearnSensitivities[vInspectionNumberShowing] >= 31)
						dc.SelectObject(vLocalSystemData->vYellowPen);
					else
						dc.SelectObject(vLocalSystemData->vRedPen);

					for (BYTE TempLoop = 0; TempLoop < vSensitivityHeight; TempLoop++)
					{
						dc.MoveTo(vSensitivityLeft, vLearnSensitivityBottom - TempLoop);
						dc.LineTo(vSensitivityLeft + (int)vLocalSystemData->vLearnSensitivities[vInspectionNumberShowing], vLearnSensitivityBottom - TempLoop); //draw sensitivity
					}
				}
			}
		}
		if (vLocalConfigurationData->vImageShowLines)
		if (!vRejectsImageDisplay)
		if ((!vLocalConfigurationData->vContinuousFeedContainerRate) && (!vLocalConfigurationData->vBulkProductMode) && (!vGlobalCurrentProduct->vBulkProductMode)) //don't draw purple line in bulk mode
		{
			//if ((dc.m_ps.rcPaint.top <= vLocalSystemData->vITIPCDig->vMainDisplayImageSizeY + TempOffsetY) && (dc.m_ps.rcPaint.bottom >= TempOffsetY) &&
			//	(dc.m_ps.rcPaint.left <= TempOffsetX) && (dc.m_ps.rcPaint.right >= TempOffsetX - 7))
			{
				//vDrawPurpleLine = false;
				WORD TempTop = (WORD)(( vGlobalCurrentProduct->vEdgeLocationBottom + vGlobalCurrentProduct->vEdgeLocationHeight) * vGlobalPixelsPerUnitInHeight);
				TempTop = TempTop * vLocalSystemData->vITIPCDig->vDisplayImageSizeY / vLocalSystemData->vITIPCDig->vOriginalBufferSizeX;
				if (vLocalSystemData->vITIPCDig->vDisplayImageSizeY > TempTop)
					TempTop = vLocalSystemData->vITIPCDig->vDisplayImageSizeY - TempTop;
				else
					TempTop = 0;

				WORD TempBottom = (WORD)(vGlobalCurrentProduct->vEdgeLocationBottom * vGlobalPixelsPerUnitInHeight);
				WORD TempHeight = (WORD)((vGlobalCurrentProduct->vEdgeLocationHeight) * vGlobalPixelsPerUnitInHeight);
				TempHeight = TempHeight * vLocalSystemData->vITIPCDig->vDisplayImageSizeY / vLocalSystemData->vITIPCDig->vOriginalBufferSizeX;
				for (WORD TempLoopY = 1; TempLoopY < TempHeight; TempLoopY++)
				{
					for (WORD TempLoopX = 1; TempLoopX < 3; TempLoopX++)
					{
						//if ((TempOffsetX - TempLoopX <= dc.m_ps.rcPaint.right) && (TempOffsetX - TempLoopX >= dc.m_ps.rcPaint.left) &&
						//	(TempOffsetY + TempLoopY + TempTop >= dc.m_ps.rcPaint.top) && (TempOffsetY + TempLoopY + TempTop <= dc.m_ps.rcPaint.bottom))
						dc.SetPixelV(TempOffsetX - TempLoopX, TempOffsetY + TempLoopY + TempTop, cPurple); //draw pixel in purple
					}
				}
			}
			//draw bottom purple line
			if (vLocalSystemData->vITIPCDig->vDoBottomReferencing)
			if (vGlobalCurrentProduct->vBottomLocationLengthPixel)
			{
				WORD TempLeft = (WORD)(vGlobalCurrentProduct->vBottomLocationLeftPixel);
				TempLeft = TempLeft * vLocalSystemData->vITIPCDig->vDisplayImageSizeX / vLocalSystemData->vITIPCDig->vOriginalBufferSizeY;

				//purple line is referenced to left edge of container
				if ((vDisplayingImageData) && (vDisplayingImageData->vLeft))
					TempLeft = TempLeft + (vDisplayingImageData->vLeft * vLocalSystemData->vITIPCDig->vDisplayImageSizeX / vLocalSystemData->vITIPCDig->vOriginalBufferSizeY);

				if (vLocalSystemData->vITIPCDig->vDisplayImageSizeX < TempLeft)
					TempLeft = vLocalSystemData->vITIPCDig->vDisplayImageSizeX;

				WORD TempRight = (WORD)(vGlobalCurrentProduct->vBottomLocationLeftPixel + vGlobalCurrentProduct->vBottomLocationLengthPixel);
				TempRight = TempRight * vLocalSystemData->vITIPCDig->vDisplayImageSizeX / vLocalSystemData->vITIPCDig->vOriginalBufferSizeY;

				//purple line is referenced to left edge of container
				if ((vDisplayingImageData) && (vDisplayingImageData->vLeft))
					TempRight = TempRight + (vDisplayingImageData->vLeft * vLocalSystemData->vITIPCDig->vDisplayImageSizeX / vLocalSystemData->vITIPCDig->vOriginalBufferSizeY);

				if (vLocalSystemData->vITIPCDig->vDisplayImageSizeX < TempRight)
					TempRight = vLocalSystemData->vITIPCDig->vDisplayImageSizeX;

				WORD TempHeight = TempRight - TempLeft;

				WORD TempStartX = TempOffsetX;
				WORD TempStartY = TempOffsetY + vLocalSystemData->vITIPCDig->vDisplayImageSizeY - 3;

				//for (WORD TempLoopX = 0; TempLoopX < vLocalSystemData->vITIPCDig->vDisplayImageSizeX; TempLoopX++)
				//for (WORD TempLoopY = 1; TempLoopY < 7; TempLoopY++)
				//{
				//	//if ((TempOffsetX - TempLoopX <= dc.m_ps.rcPaint.right) && (TempOffsetX - TempLoopX >= dc.m_ps.rcPaint.left) &&
				//	//	(TempOffsetY + TempLoopY >= dc.m_ps.rcPaint.top) && (TempOffsetY + TempLoopY <= dc.m_ps.rcPaint.bottom))
				//	dc.SetPixelV(TempStartX + TempLoopX, TempStartY + TempLoopY, cLightGray); //draw pixel in Light Grey
				//}
				for (WORD TempLoopX = 0; TempLoopX <= TempHeight; TempLoopX++)
				for (WORD TempLoopY = 1; TempLoopY < 3; TempLoopY++)
				{
					//if ((TempOffsetX - TempLoopX <= dc.m_ps.rcPaint.right) && (TempOffsetX - TempLoopX >= dc.m_ps.rcPaint.left) &&
					//	(TempOffsetY + TempLoopY + TempTop >= dc.m_ps.rcPaint.top) && (TempOffsetY + TempLoopY + TempTop <= dc.m_ps.rcPaint.bottom))
					dc.SetPixelV(TempStartX + TempLoopX + TempLeft, TempStartY + TempLoopY, cPurple); //draw pixel in purple
				}
			}
		}
	}

	// Do not call CDialog::OnPaint() for painting messages
	double TempEndTime = CIFCOS::GetSystimeMicrosecs();
	vLocalSystemData->vITIPCDig->vLastScreenPaintTime = TempEndTime - TempStartTime;
}

void CNewImageDisplayDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewImageDisplayDialog)
	DDX_Control(pDX, IDC_CurrentInspection, m_CurrentInspection);
	DDX_Control(pDX, IDC_SubFunction8Display, m_SubFunction8Display);
	DDX_Control(pDX, IDC_SubFunction7Display, m_SubFunction7Display);
	DDX_Control(pDX, IDC_SubFunction6Display, m_SubFunction6Display);
	DDX_Control(pDX, IDC_SubFunction4Display, m_SubFunction4Display);
	DDX_Control(pDX, IDC_SubFunction3Display, m_SubFunction3Display);
	DDX_Control(pDX, IDC_SubFunction2Display, m_SubFunction2Display);
	DDX_Control(pDX, IDC_SubFunction1Display, m_SubFunction1Display);
	DDX_Control(pDX, IDC_SubFunction5Display, m_SubFunction5Display);
	DDX_Control(pDX, IDC_InspectionThreshold, m_InspectionThreshold);
	DDX_Control(pDX, IDC_XRaysOn, m_XRaysOn);
	DDX_Control(pDX, IDC_ImageStatus, m_ImageStatus);
	DDX_Control(pDX, IDC_SubFunction8Button, m_SubFunction8Button);
	DDX_Control(pDX, IDC_SubFunction7Button, m_SubFunction7Button);
	DDX_Control(pDX, IDC_SubFunction6Button, m_SubFunction6Button);
	DDX_Control(pDX, IDC_SubFunction5Button, m_SubFunction5Button);
	DDX_Control(pDX, IDC_SubFunction4Button, m_SubFunction4Button);
	DDX_Control(pDX, IDC_SubFunction3Button, m_SubFunction3Button);
	DDX_Control(pDX, IDC_SubFunction2Button, m_SubFunction2Button);
	DDX_Control(pDX, IDC_SubFunction1Button, m_SubFunction1Button);
	DDX_Control(pDX, IDC_Function4Button, m_Function4Button);
	DDX_Control(pDX, IDC_Function5Button, m_Function5Button);
	DDX_Control(pDX, IDC_Function3Button, m_Function3Button);
	DDX_Control(pDX, IDC_Function2Button, m_Function2Button);
	DDX_Control(pDX, IDC_Function1Button, m_Function1Button);
	DDX_Control(pDX, IDC_DialogTitleStaticText1, m_DialogTitleStaticText);
	DDX_Control(pDX, IDC_ErrorMessage, m_ErrorMessage);
	DDX_Control(pDX, IDC_Background, m_Background);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_Sensitivity, m_Sensitivity);
	DDX_Control(pDX, IDC_RejectReason, m_RejectReason);
	DDX_Control(pDX, IDC_MainStatusLine, m_MainStatusLine);
}


BEGIN_MESSAGE_MAP(CNewImageDisplayDialog, CDialog)
	//{{AFX_MSG_MAP(CNewImageDisplayDialog)
	ON_BN_CLICKED(IDC_Function1Button, OnFunction1Button)
	ON_BN_CLICKED(IDC_Function2Button, OnFunction2Button)
	ON_BN_CLICKED(IDC_Function3Button, OnFunction3Button)
	ON_BN_CLICKED(IDC_Function4Button, OnFunction4Button)
	ON_BN_CLICKED(IDC_Function5Button, OnFunction5Button)
	ON_BN_CLICKED(IDC_SubFunction1Button, OnSubFunction1Button)
	ON_BN_CLICKED(IDC_SubFunction2Button, OnSubFunction2Button)
	ON_BN_CLICKED(IDC_SubFunction3Button, OnSubFunction3Button)
	ON_BN_CLICKED(IDC_SubFunction4Button, OnSubFunction4Button)
	ON_BN_CLICKED(IDC_SubFunction5Button, OnSubFunction5Button)
	ON_BN_CLICKED(IDC_SubFunction6Button, OnSubFunction6Button)
	ON_BN_CLICKED(IDC_SubFunction7Button, OnSubFunction7Button)
	ON_BN_CLICKED(IDC_SubFunction8Button, OnSubFunction8Button)
	ON_WM_SHOWWINDOW()
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_DialogTitleStaticText, OnDialogTitleStaticText)
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
	ON_STN_CLICKED(IDC_DialogTitleStaticText1, &CNewImageDisplayDialog::OnDialogtitlestatictext1)
	ON_STN_DBLCLK(IDC_SubFunction1Button, &CNewImageDisplayDialog::OnSubFunction1ButtonDoubleClicked)
	ON_STN_DBLCLK(IDC_SubFunction2Button, &CNewImageDisplayDialog::OnSubFunction2ButtonDoubleClicked)
	ON_STN_DBLCLK(IDC_SubFunction3Button, &CNewImageDisplayDialog::OnSubFunction3ButtonDoubleClicked)
	ON_STN_DBLCLK(IDC_SubFunction4Button, &CNewImageDisplayDialog::OnSubFunction4ButtonDoubleClicked)
	ON_STN_DBLCLK(IDC_SubFunction5Button, &CNewImageDisplayDialog::OnSubFunction5ButtonDoubleClicked)
	ON_STN_DBLCLK(IDC_SubFunction6Button, &CNewImageDisplayDialog::OnSubFunction6ButtonDoubleClicked)
	ON_STN_DBLCLK(IDC_SubFunction7Button, &CNewImageDisplayDialog::OnSubFunction7ButtonDoubleClicked)
	ON_STN_DBLCLK(IDC_SubFunction8Button, &CNewImageDisplayDialog::OnSubFunction8ButtonDoubleClicked)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewImageDisplayDialog message handlers

void CNewImageDisplayDialog::OnFunction1Button() 
{  //return to rejects display from image analysis display
	//vGlobalMouseMoveCounter = 0;
	if (vEditROIBoundaries)
	{  //back to rejects
		bool TempOKToExit = true;
		if (vChangeMade)
		{
			CYesNoDialog TempYesNoDialog;
			CString TempString("");
			TempString = "";
			TempString.LoadString(IDS_TheROIhasbeenchangedExitwithoutsavingchanges);
			//TempString = "\n\nThe ROI has been changed.\nExit without saving changes?";
			TempYesNoDialog.vNoticeText = TempString;
			TempYesNoDialog.vQuestionType = cConfirmAbandon;
			int TempResult = TempYesNoDialog.DoModal();
			if (TempResult != IDOK)
				TempOKToExit = false;
		}
		if (TempOKToExit)
		{
			if (vChangeMade)
				RestoreOriginalROIValues();
			PrepareAndExit(true);
		}
	}
	else
	if ((vAnalysisImageDisplay) || (vShowThresholdMenu))
	{  //leave the image analysis display and return to the last rejects display
		bool TempOKToExit = true;
		if (vShowThresholdMenu)
		{
			if (vChangeMade)
			{
				CYesNoDialog TempYesNoDialog;
				CString TempString("");
				TempString = "";
				TempString.LoadString(IDS_TheThresholdhasbeenchangedExitwithoutsavingchanges);
				//TempString = "\n\nThe Threshold has been changed.\nExit without saving changes?";
				TempYesNoDialog.vNoticeText = TempString;
				TempYesNoDialog.vQuestionType = cConfirmAbandon;
				int TempResult = TempYesNoDialog.DoModal();
				if (TempResult != IDOK)
					TempOKToExit = false;
			}
		}
		if (TempOKToExit)
		{ //go back to rejects display
			vChangeMade = false;
			vAnalysisImageDisplay = false;
			vRejectsImageDisplay = true;
			this->SetWindowText(_T("Rejects"));
			vShowThresholdMenu = false;
			ClearSensitivity();

			ClearAllDisplayOptions(true);

			UpdateRejectCounts();
			vDialogTitleString.LoadString(IDS_RejectDisplay);
			SetDlgItemText(IDC_DialogTitleStaticText1,vDialogTitleString);
			UpdateButtons();
			UpdateDisplayIndicators();
			UpdateDisplay(vDisplayingImageData);
			ShowOrHideDisplays();
			m_InspectionThreshold.ShowWindow(SW_HIDE);
			DisplayLocationEdge();
			WaitThenDrawImageOnScreen();
		}
	}
	else
	// User clicked Main Menu Button to exit 
	//if (TempOKToExit)
	{
		PrepareAndExit(true);
	}
}

void CNewImageDisplayDialog::OnFunction2Button() 
{
	//vGlobalMouseMoveCounter = 0;
	if (vEditROIBoundaries)
	{  //back to rejects
		bool TempOKToExit = true;
		if (vChangeMade)
		{
			CYesNoDialog TempYesNoDialog;
			CString TempString("");
			TempString = "";
			TempString.LoadString(IDS_TheROIhasbeenchangedExitwithoutsavingchanges);
			//TempString = "\n\nThe ROI has been changed.\nExit without saving changes?";
			TempYesNoDialog.vNoticeText = TempString;
			TempYesNoDialog.vQuestionType = cConfirmAbandon;
			int TempResult = TempYesNoDialog.DoModal();
			if (TempResult != IDOK)
				TempOKToExit = false;
		}
		if (TempOKToExit)
		{
			if (vChangeMade)
				RestoreOriginalROIValues();
			vEditROIBoundaries = false;
			ClearSensitivity();
			vAnalysisImageDisplay = true;
			this->SetWindowText(_T("Rejects Analysis"));
			m_SubFunction1Button.ShowNavigationArrow(NULL);
			m_SubFunction2Button.ShowNavigationArrow(NULL);
			m_SubFunction3Button.ShowNavigationArrow(NULL);
			m_SubFunction4Button.ShowNavigationArrow(NULL);
			m_SubFunction5Button.ShowNavigationArrow(NULL);
			m_SubFunction6Button.ShowNavigationArrow(NULL);
			m_SubFunction7Button.ShowNavigationArrow(NULL);
			m_SubFunction8Button.ShowNavigationArrow(NULL);
			vDialogTitleString.LoadString(IDS_ImageAnalysis);
			SetDlgItemText(IDC_DialogTitleStaticText1,vDialogTitleString);
			UpdateButtons();
			UpdateDisplay(vDisplayingImageData);
			WaitThenDrawImageOnScreen();
			//UpdateDisplayIndicators();
			vChangeMade = false;
		}
	}
	else
	if (vShowThresholdMenu)
	{
		bool TempOKToExit = true;
		if (vChangeMade)
		{
			CYesNoDialog TempYesNoDialog;
			CString TempString("");
			TempString = "";
			TempString.LoadString(IDS_TheThresholdhasbeenchangedExitwithoutsavingchanges);
			//TempString = "\n\nThe Threshold has been changed.\nExit without saving changes?";
			TempYesNoDialog.vNoticeText = TempString;
			TempYesNoDialog.vQuestionType = cConfirmAbandon;
			int TempResult = TempYesNoDialog.DoModal();
			if (TempResult != IDOK)
				TempOKToExit = false;
		}

		if (TempOKToExit)
		{
			while (vShowingProcess)
			{
				ShowNextProcess(true);
			}
			vShowThreshold = false;

			if (vInspectionShowing)
			if ((vInspectionShowing->vInspectionType == cUnderfillByWeightInspection) ||
				(vInspectionShowing->vInspectionType == cCheckWeighInspection))
			{
				vThreshold = vInspectionShowing->vDensityThresholdUpper;
				vMaximumThreshold = vMaximumUnderfillByWeightThreshold;
					vShowingUpperThreshold = true;
			}
			else
			if ((vInspectionShowing->vInspectionType == cOverfillByWeightInspection) ||
				(vInspectionShowing->vInspectionType == cCheckWeighInspection))
			{
				vThreshold = vInspectionShowing->vDensityThresholdLower;
				vMaximumThreshold = vMaximumUnderfillByWeightThreshold;
				vShowingUpperThreshold = false;
			}
			else
			if ((vInspectionShowing->vInspectionType == cDensityInspection) ||
				//(vInspectionShowing->vInspectionType == cCheckWeighInspection) ||
				(vInspectionShowing->vInspectionType == cMissingLidInspection) ||
				(vInspectionShowing->vInspectionType == cUnderfillInspection) ||
				(vInspectionShowing->vInspectionType == cOverfillInspection) ||
				(vInspectionShowing->vInspectionType == cStandardDeviationInspection))
			{
				vMaximumThreshold = 255;
				if (vInspectionShowing->vHasDensityUpperThreshold)
				{
					vThreshold = vInspectionShowing->vDensityThresholdUpper;
					vShowingUpperThreshold = true;
				}
				else
				{
					vThreshold = vInspectionShowing->vDensityThresholdLower;
				}
			}
			else
			{
				vThreshold = vInspectionShowing->vThreshold;
				vMaximumThreshold = 255;
			}

			vShowThresholdMenu = false;
			ClearSensitivity();
			vAnalysisImageDisplay = true;
			this->SetWindowText(_T("Rejects Analysis"));
			vDialogTitleString.LoadString(IDS_ImageAnalysis);
			SetDlgItemText(IDC_DialogTitleStaticText1,vDialogTitleString);
			m_SubFunction5Display.ShowWindow(SW_HIDE);
			UpdateButtons();
			WaitThenDrawImageOnScreen();
			vChangeMade = false;
		}
		UpdateDisplay(vDisplayingImageData);
	}
	else
	if (vImproveLearnMenu)
	{  //exit Improve Learn Menu and go back to image analysis
		ClearAllDisplayOptions(true);
		//vGlobalCurrentProduct->vTypesOfRejectsToView = vSaveTypesOfRejectsToView;
		vImproveLearnMenu = false;
		vAnalysisImageDisplay = true;
		this->SetWindowText(_T("Rejects Analysis"));
		vDialogTitleString.LoadString(IDS_ImageAnalysis);
		SetDlgItemText(IDC_DialogTitleStaticText1,vDialogTitleString);
		UpdateButtons();

		SetInspectionShowing(vGlobalCurrentProduct->vInspection[0]);
		//should it set current inspection to reject reason for current image?
		vLocalSystemData->vITIPCDig->vRejectBufferShowing = -1;
		ShowNextReject(true, false);
		UpdateDisplay(vDisplayingImageData);
	}
	else
	if (vImageDisplayOptions)
	{
		vImageDisplayOptions = false;
		vAnalysisImageDisplay = true;
		this->SetWindowText(_T("Rejects Analysis"));
		vDialogTitleString.LoadString(IDS_ImageAnalysis);
		SetDlgItemText(IDC_DialogTitleStaticText1,vDialogTitleString);
		UpdateButtons();
	}
	else
	if (vEditWhileRunning)
	{
		vEditWhileRunning = false;
		vAnalysisImageDisplay = true;
		this->SetWindowText(_T("Rejects Analysis"));
		vDialogTitleString.LoadString(IDS_ImageAnalysis);
		SetDlgItemText(IDC_DialogTitleStaticText1,vDialogTitleString);
		UpdateButtons();
		vChangeMade = false;
		m_InspectionThreshold.ShowWindow(SW_SHOW);
		m_SubFunction3Display.ShowWindow(SW_HIDE);
		m_SubFunction4Display.ShowWindow(SW_HIDE);
		UpdateThresholdDisplay();
	}
	else
	if (vAnalysisImageDisplay)
	{  //go to Improve Learn Menu
		if (PasswordOK(cAllansPassword,false))
		{
			vAnalysisImageDisplay = false;
			vImproveLearnMenu = true;
			this->SetWindowText(_T("Rejects Improve"));
			vDialogTitleString.LoadString(IDS_ImproveLearn);
			SetDlgItemText(IDC_DialogTitleStaticText1,vDialogTitleString);
			SetDlgItemText(IDC_InspectionThreshold, _T(""));
			vShowType = cShowTypeContaminant;
			UpdateThresholdDisplay();

			ClearAllDisplayOptions(true);
			vShowDeltaCurrentPreviewStructure = true;
			WaitThenDrawImageOnScreen();
			UpdateButtons();
			UpdateDisplayIndicators();
			vOldImproveLearnTotalContainers = vLocalSystemData->vImproveLearnTotalContainers;
			if (vOldImproveLearnTotalContainers)
				SetDlgItemText(IDC_CurrentInspection,dtoa(vLocalSystemData->vITIPCDig->vImproveLearnCount,0) + " of " + dtoa(vOldImproveLearnTotalContainers,0) + "\n" +
						dtoa((double)vLocalSystemData->vITIPCDig->vImproveLearnCount / (double)vOldImproveLearnTotalContainers * 100.0,3) + "%");

			//SetForegroundWindow();
			this->SetFocus();
		}
		else
		{
			// Live Images Button Pressed
			if (vLocalSystemData->vSystemRunMode == cStoppedSystemMode)
			{
				CString TempText("");
				CNoticeDialog TempNoticeDialog;
				TempText.LoadString(IDS_SystemMustBeRunningToShowImages);
				TempNoticeDialog.vNoticeText = TempText;
				TempNoticeDialog.vType = cErrorMessage;
				TempNoticeDialog.DoModal();
			}
			else
			{
				vShowNextFrame = false;
				if (vShowingLiveImages)
					StopShowingLiveImages();
				else
					StartShowingLiveImages();
				UpdateButtons();
			}
		}
	}
	else
	if (vRejectsImageDisplay)
	{  //goto the image analysis display
		if (PasswordOK(cSetupPassword, true))
		{
			vAnalysisImageDisplay = true;
			this->SetWindowText(_T("Rejects Analysis"));
			vRejectsImageDisplay = false;
			vDialogTitleString.LoadString(IDS_ImageAnalysis);
			SetDlgItemText(IDC_DialogTitleStaticText1,vDialogTitleString);
			m_InspectionThreshold.ShowWindow(SW_SHOW);
			UpdateButtons();
			UpdateDisplayIndicators();
			UpdateDisplay(vDisplayingImageData);
			m_SubFunction1Display.ShowWindow(SW_HIDE);
			m_SubFunction2Display.ShowWindow(SW_HIDE);
			m_SubFunction3Display.ShowWindow(SW_HIDE);
			m_SubFunction4Display.ShowWindow(SW_HIDE);
			m_SubFunction5Display.ShowWindow(SW_HIDE);
			m_SubFunction6Display.ShowWindow(SW_HIDE);
			m_SubFunction7Display.ShowWindow(SW_HIDE);
			//m_SubFunction8Display.ShowWindow(SW_HIDE);
			DisplayLocationEdge();
			WaitThenDrawImageOnScreen();
		}
	}
	else if (vFunction2ButtonEnable == false)
		ShowNoMouseClickDialogMessage();
}

void CNewImageDisplayDialog::OnFunction3Button() 
{
	//vGlobalMouseMoveCounter = 0;
	if (vImproveLearnMenu)
	{
		//show original learn
		if (!vLocalSystemData->vITIPCDig->HaveLearnImageFile("=Process 2o"))
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\n\nNo Original Learn To Show";
			TempNoticeDialog.vType = cInformationMessage;
			TempNoticeDialog.DoModal();
		}
		else
		if (vShowOriginalStructure)
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\n\nAlready showing this image";
			TempNoticeDialog.vType = cInformationMessage;
			TempNoticeDialog.DoModal();
		}
		else
		{
			BYTE TempShowType = vShowType;
			ClearAllDisplayOptions(true);
			vShowType = TempShowType;

			vShowOriginalStructure = true;
			UpdateButtons();
			DrawImageOnScreen();
		}
	}
	else
	{
		//vGlobalMouseMoveCounter = 0;
		if (vHaveAnImageToShow)
			ShowImageDisplayInformation(vDisplayingImageData);
		else
			ShowNeedImageDialogMessage();
	}
}

void CNewImageDisplayDialog::OnFunction4Button() 
{
	//vGlobalMouseMoveCounter = 0;
	if (vImproveLearnMenu)
	{
		//show current learn
		if (!vLocalSystemData->vITIPCDig->vProductStructureImage)
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\n\nNo Current Learn To Show";
			TempNoticeDialog.vType = cInformationMessage;
			TempNoticeDialog.DoModal();
		}
		else
		if (vShowStructure)
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\n\nAlready showing this image";
			TempNoticeDialog.vType = cInformationMessage;
			TempNoticeDialog.DoModal();
		}
		else
		{
			BYTE TempShowType = vShowType;
			ClearAllDisplayOptions(true);
			vShowType = TempShowType;

			vShowStructure = true;
			UpdateButtons();
			DrawImageOnScreen();
		}
	}
	else
	if (vEditROIBoundaries)
	{  //save NEW ROI in product
		if (vInspectionShowing)
		{
			CYesNoDialog TempYesNoDialog;
			CString TempString("");
			TempString = "";
			TempString = *vGlobalCurrentProduct->GetProductName();
			TempString = "\nSave ROI Boundaries?\nProduct: " +
				TempString + "\nInspection: " + vInspectionShowing->vName;
			TempYesNoDialog.vNoticeText = TempString;
			TempYesNoDialog.vQuestionType = cConfirmQuestion;
			int TempResult = TempYesNoDialog.DoModal();
			if (TempResult == IDOK)
			{
				vChangeMade = false;
				vInspectionShowing->SetROITop(vInspectionShowing->vROITop);
				vInspectionShowing->SetROIBottom(vInspectionShowing->vROIBottom);
				vInspectionShowing->SetROILeft(vInspectionShowing->vROILeft, vGlobalCurrentProduct->vOverScanMultiplier);
				vInspectionShowing->SetROIRight(vInspectionShowing->vROIRight, vGlobalCurrentProduct->vOverScanMultiplier);
				SaveOriginalROIValues();
				CWnd *TempWindow = CWnd::GetDlgItem(IDC_Function2Button);
				if (TempWindow)
					TempWindow->Invalidate(false);
				TempWindow = CWnd::GetDlgItem(IDC_Function4Button);
				if (TempWindow)
					TempWindow->Invalidate(false);
				ReportErrorMessage("Changed " + vInspectionShowing->vName + " ROI by operator", cUserChanges,0);

				vGlobalCurrentProduct->MatchInspectionsForWeightTrending(); //if ROI of a Weight Trending inspection changed, must update any that point to it
				//goto edit while running menu
				vEditROIBoundaries = false;
				ClearSensitivity();
				vEditWhileRunning = true;
				this->SetWindowText(_T("Rejects Edit"));
				vDialogTitleString.LoadString(IDS_EditInspection);
				SetDlgItemText(IDC_DialogTitleStaticText1,vDialogTitleString);

				SetInspectionShowing(vInspectionShowing);  //update settings on screen display
				m_SubFunction3Display.ShowWindow(SW_SHOW);
				m_SubFunction4Display.ShowWindow(SW_SHOW);
				m_SubFunction1Button.ShowNavigationArrow(NULL);
				m_SubFunction2Button.ShowNavigationArrow(NULL);
				m_SubFunction3Button.ShowNavigationArrow(NULL);
				m_SubFunction4Button.ShowNavigationArrow(NULL);
				m_SubFunction5Button.ShowNavigationArrow(NULL);
				m_SubFunction6Button.ShowNavigationArrow(NULL);
				m_SubFunction7Button.ShowNavigationArrow(NULL);
				m_SubFunction8Button.ShowNavigationArrow(NULL);
				m_InspectionThreshold.ShowWindow(SW_HIDE);
				UpdateButtons();
				UpdateThresholdDisplay();
			}
		}
	}
	else
	if (vImageDisplayOptions)
	{
	}
	else
	if (vEditWhileRunning)
	{ 
	}
	else
	{
		ShowNextReject(true, true);
		if ((!vHaveAnImageToShow) || ((vNumberOfRejectsInLocalBuffer == 0) && (vAllRejectsHidden)))
			ShowNeedImageDialogMessage();
	}
}

void CNewImageDisplayDialog::OnFunction5Button() 
{
	//vGlobalMouseMoveCounter = 0;
	if (vImproveLearnMenu)
	{
		//show Improve learn Preview
		if (!vLocalSystemData->vITIPCDig->vImproveLearnCount)
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\n\nNo Improve Learn To Preview";
			TempNoticeDialog.vType = cInformationMessage;
			TempNoticeDialog.DoModal();
		}
		else
		if (vShowImproveLearnPreview)
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\n\nAlready showing this image";
			TempNoticeDialog.vType = cInformationMessage;
			TempNoticeDialog.DoModal();
		}
		else
		{
			BYTE TempShowType = vShowType;
			ClearAllDisplayOptions(true);
			vShowType = TempShowType;

			vShowImproveLearnPreview = true;
			UpdateButtons();
			DrawImageOnScreen();
		}
	}
	else
	if (vEditROIBoundaries)
	{
	}
	else
	if (vImageDisplayOptions)
	{
		if (PasswordOK(cTemporaryInspxPassword,false))
			SaveImageToFile(true, true);
	}
	else
	if (vEditWhileRunning)
	{ 
		CWnd *TempWindow = NULL;
		TempWindow = FindWindow(NULL, _T("AutoSetup"));
		/*
		if (vLocalSystemData->vSystemRunMode != cRunningSystemMode)
		{
			CNoticeDialog TempNoticeDialog;
			CString TempText("");
			TempText.LoadString(IDS_SystemMustBeRunningToShowImages);
			TempNoticeDialog.vNoticeText = TempText;
			TempNoticeDialog.vType = cErrorMessage;
			TempNoticeDialog.DoModal();
		}
		else
		*/
		if (TempWindow) 
			OnFunction1Button();
		else
		if (vInspectionShowing)
		{
			//Evaluate Product button was pressed
			CAutoSetupImageDisplayDialog IAutoSetupImageDisplayDialog;
			IAutoSetupImageDisplayDialog.vLocalConfigurationData = vLocalConfigurationData;
			IAutoSetupImageDisplayDialog.vLocalSystemData = vLocalSystemData;
			IAutoSetupImageDisplayDialog.vMainWindowPointer = vMainWindowPointer;
			IAutoSetupImageDisplayDialog.vShowReturnToEditMenu = true;
			IAutoSetupImageDisplayDialog.vAutoSetupType = cEvaluateAutoSetup;
			//when go into auto setup, it will go into evaluate inspection
			if (vInspectionShowing->vInspectionType == cStandardDeviationInspection)
				IAutoSetupImageDisplayDialog.vMode = cSetDensityThreshold;
			else
			if ((vInspectionShowing->vInspectionType == cDensityInspection) ||
				(vInspectionShowing->vInspectionType == cCheckWeighInspection) ||
				(vInspectionShowing->vInspectionType == cMissingLidInspection) ||
				(vInspectionShowing->vInspectionType == cUnderfillByWeightInspection) ||
				(vInspectionShowing->vInspectionType == cOverfillByWeightInspection) ||
				(vInspectionShowing->vInspectionType == cUnderfillInspection) ||
				(vInspectionShowing->vInspectionType == cOverfillInspection))
				IAutoSetupImageDisplayDialog.vMode = cSetDensityThreshold;
			else
			if (vInspectionShowing->vInspectionType == cVoidInspection)
				IAutoSetupImageDisplayDialog.vMode = cSetVoidThreshold;
			else
			if ((vInspectionShowing->vInspectionType == cItemCountInspection) || (vInspectionShowing->vInspectionType == cSizeInspection) || (vInspectionShowing->vInspectionType == cMissingLidInspectionOnSize))
				IAutoSetupImageDisplayDialog.vMode = cSetImageSize;
			else
				IAutoSetupImageDisplayDialog.vMode = cSetContaminantThreshold;
			IAutoSetupImageDisplayDialog.vPickOneToEvaluate = true;

			//6/9/04
			CString TempInspectionName("");
			if (vInspectionShowing)
				TempInspectionName = vInspectionShowing->vName;

			IAutoSetupImageDisplayDialog.vInspectionEditing = vInspectionShowing;
			IAutoSetupImageDisplayDialog.vLocalProductCollection = vLocalProductCollection;
			BYTE TempEnableEjectors = vLocalConfigurationData->vEnableEjectors;
			vInspectionShowing = NULL;
			int nResponse = IAutoSetupImageDisplayDialog.DoModal();

			//if ((nResponse == IDOK) || (nResponse == 20))
			{
				// TODO: Place code here to handle when the dialog is accepted
				if (vLocalSystemData->vCurrentProductChanged)
				{
					/*
					if (vLocalSystemData->vSystemRunMode == cStoppedSystemMode)
					{
						vMainWindowPointer->SetupProduct(vGlobalCurrentProduct, false);
						vMainWindowPointer->ResetCounters();
						vLocalSystemData->vInspectionNameChanged = false;
						vLocalSystemData->vInspectionNameMoved = false;
					}
					else
					*/
					{
						if (vLocalSystemData->vInspectionNameChanged)
						{
							vInspectionShowing = NULL;
							vInspectionNumberShowing = 0;
							if (vGlobalCurrentProduct->vNumberOfInspections)
								vInspectionShowing = vGlobalCurrentProduct->vInspection[0];
							vLocalSystemData->vInspectionNameChanged = false;
							vMainWindowPointer->SetupRejectReasonNames();
							if (vLocalSystemData->vInspectionNameMoved)
							{
								vLocalSystemData->vInspectionNameMoved = false;
								vMainWindowPointer->MakeProductionReportIfNeeded();
								vMainWindowPointer->ResetCounters();
							}
						}
					}
				}
				if (TempEnableEjectors != vLocalConfigurationData->vEnableEjectors)
				{
					CString TempProductName = *vGlobalCurrentProduct->GetProductName();
					CYesNoDialog TempYesNoDialog;
					CString TempText("");//("\n\nEjectors were disabled when entered Edit While Running\nRe-enable Ejectors Now?");
					TempText.LoadString(IDS_EjectorsweredisabledwhenenteredEditWhileRunning);
					TempYesNoDialog.vNoticeText = TempText;
					CString TempText1("");
					TempText1.LoadString(IDS_EnableEjectors);
					TempYesNoDialog.vYesButtonText = TempText1;
					TempText1.LoadString(IDS_KeepDisabled);
					TempYesNoDialog.vNoButtonText = TempText1;
					TempYesNoDialog.vQuestionType = cConfirmQuestion;
					int TempResult = TempYesNoDialog.DoModal();
					if (TempResult == IDOK)
					{
						//vLocalConfigurationData->vEnableEjectors = true;
						//vGlobaluCSerialPort->EnableDisableEjectors(vLocalConfigurationData->vEnableEjectors);
						//vMainWindowPointer->UpdateDisableEjectorsButton();
						vMainWindowPointer->EnableDisableEjectors(true);
						ReportErrorMessage("Ejectors enabled by operator leaving Setup", cUserChanges,0);
					}
				}
			}
			//if (vGlobalCurrentProduct->HaveInspection(vInspectionShowing))
			//	SetInspectionShowing(vGlobalCurrentProduct->vInspection[
			//	vGlobalCurrentProduct->InspectionNumber(vInspectionShowing)]);
			//6/9/04
			vInspectionShowing = vGlobalCurrentProduct->vInspection[0];
			vInspectionNumberShowing = 0;
			if (TempInspectionName != "")
			{
				for (BYTE TempLoop = 1; TempLoop < vGlobalCurrentProduct->vNumberOfInspections; TempLoop++)
				if (vGlobalCurrentProduct->vInspection[TempLoop])
				if (vGlobalCurrentProduct->vInspection[TempLoop]->vName == TempInspectionName)
				{
					vInspectionShowing = vGlobalCurrentProduct->vInspection[TempLoop];
					vInspectionNumberShowing = TempLoop;
				}
			}
			if (nResponse == 10)
			{
				//Main Menu button pressed
				Sleep(1); //is the equivelent of a yeild statement;
				PrepareAndExit(10);
			}
			SetInspectionShowing(vInspectionShowing);
			WaitThenDrawImageOnScreen();
		}
	}
	else
	{
		ShowNextReject(true, false);
		if ((!vHaveAnImageToShow) || ((vNumberOfRejectsInLocalBuffer == 0) && (vAllRejectsHidden)))
			ShowNeedImageDialogMessage();
	}
}

void CNewImageDisplayDialog::OnSubFunction1Button() 
{
	//vGlobalMouseMoveCounter = 0;
	if (vImproveLearnMenu)
	{
		//clear improve learn
		if (!vLocalSystemData->vITIPCDig->vImproveLearnCount)
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\n\nNo Improve Learn To Clear";
			TempNoticeDialog.vType = cInformationMessage;
			TempNoticeDialog.DoModal();
		}
		else
		{
			vLocalSystemData->vITIPCDig->ClearImproveLearn(false);
			vLocalSystemData->vImproveLearnTotalContainers = 0;

			//return to Rejects Menu
			vImproveLearnMenu = false;
			vRejectsImageDisplay = true;
			this->SetWindowText(_T("Rejects"));
			vDialogTitleString.LoadString(IDS_RejectDisplay);
			SetDlgItemText(IDC_DialogTitleStaticText1,vDialogTitleString);
			UpdateButtons();
			UpdateDisplayIndicators();
			//causes crash UpdateDisplay(vDisplayingImageData);
			UpdateRejectCounts();
			ShowOrHideDisplays();
			m_InspectionThreshold.ShowWindow(SW_HIDE);

			SetInspectionShowing(vGlobalCurrentProduct->vInspection[0]);
			//should it set current inspection to reject reason for current image?
			vLocalSystemData->vITIPCDig->vRejectBufferShowing = -1;
			DisplayLocationEdge();
			ShowNextReject(true, true);
			if ((!vHaveAnImageToShow) || ((vNumberOfRejectsInLocalBuffer == 0) && (vAllRejectsHidden)))
				ShowNeedImageDialogMessage();
		}
	}
	else
	if ((vEditROIBoundaries) && (vSubFunction1ButtonEnable == true))
	{
		if (!vRecievedButtonPress)	//do not start repeat timer if function called from keyboard press not mouse press
		{
			if ((vMouseState == 0) || ((vMouseState > 8) && (vMouseState < 17)))	//if mouse is idle or in double click toggle on mode
			{
				vMouseState = 1;	//set mouse to on while pressed down
				KillTimer(vMouseDownRepeatTimerHandle);
				int TimerResult = SetTimer(vMouseDownRepeatTimerHandle,200,NULL);
				if (!TimerResult)
					ReportErrorMessage("Error-Second Timer Failed",cEMailInspx,32000);
			}
		}
		if (vInspectionShowing)
		if (vInspectionShowing->vROIPixelTop < vLocalSystemData->vITIPCDig->vOriginalBufferSizeX)
		{
			vInspectionShowing->vROIPixelTop++; 
			SetROILines();
		}
	}
	else
	if ((vShowThresholdMenu) && (vSubFunction1ButtonEnable == true))
	{	//Add 10 to Threshold
		if (!vRecievedButtonPress)
		{
			if ((vMouseState == 0) || ((vMouseState > 8) && (vMouseState < 17)))	//if mouse is idle or in double click toggle on mode
			{
				vMouseState = 1;	//set mouse to on while pressed down
				int TimerResult = SetTimer(vMouseDownRepeatTimerHandle,200,NULL);
				if (!TimerResult)
					ReportErrorMessage("Error-Second Timer Failed",cEMailInspx,32000);
			}
		}
		vTweakingThreshold = true;

		vThreshold = vInspectionShowing->ConvertToWeight(vThreshold);

		if (vThreshold < vMaximumThreshold - 9)
			vThreshold = vThreshold + 10;
		else
			vThreshold = vMaximumThreshold;

		vThreshold = vInspectionShowing->ConvertWeightToBrightness(vThreshold);
		//vThreshold = ((vThreshold - vInspectionShowing->vCheckWeighBFactor) / vInspectionShowing->vCheckWeighMFactor);

		vChangeMade = true;
		DisplayThreshold();
	}
	else
	if ((vRejectsImageDisplay) && (vSubFunction1ButtonEnable == true))
	{  //Filter 1 Button was pressed
		ToggleFilter(1 + vFirstInspectionToView);
	}
	else
	if ((vImageDisplayOptions) && (vSubFunction1ButtonEnable == true))
	{
		if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)
		if (PasswordOK(cTemporaryInspxPassword,false))
		{
			CNumericEntryDialog INumericEntryDialog;  
			//Set dialog box data titles and number value
			INumericEntryDialog.vEditString = dtoa(vSaveImagesToFiles, 0);
			INumericEntryDialog.m_DialogTitleStaticText1 = "Save next # of images, that are shown ";
			INumericEntryDialog.m_DialogTitleStaticText2 = "in the Image Display, to files.";
			INumericEntryDialog.m_DialogTitleStaticText3 = "Zero aborts, must Show Images to save them.";
			INumericEntryDialog.m_UnitsString = "Leaving Image Display will abort.";
			INumericEntryDialog.vMaxValue = 10000;
			INumericEntryDialog.vIntegerOnly = true;
			//Pass control to dialog box and display
			int nResponse = INumericEntryDialog.DoModal();
			//dialog box is now closed, if user pressed select do this
			//if user pressed cancel, do nothing
			if (nResponse == IDOK)
			{
				WORD TempSaveImagesToFiles = _wtoi(INumericEntryDialog.vEditString);
				if (TempSaveImagesToFiles)
				{
					CAlphaEntryDialog IAlphaEntryDialog;  
					IAlphaEntryDialog.vLocalSystemData = vLocalSystemData;
					//Set dialog box data titles and number value
					IAlphaEntryDialog.vMainWindowPointer = vMainWindowPointer;
					CTime TempTime = CTime::GetCurrentTime();
					vSaveImagesToFilesName = vSaveImagesToFilesName + TempTime.Format(" %Y-%m-%d %H-%M-%S Image Series ");
					IAlphaEntryDialog.vEditString = vSaveImagesToFilesName;

					CString TempText("Base part of file names to save");
					//TempText.LoadString(IDS_FileNameToSaveImageAs);
					IAlphaEntryDialog.m_DialogTitleStaticText = TempText;
					IAlphaEntryDialog.vAllowPeriod = true;

					//TempText.LoadString(IDS_OriginalValue);
					IAlphaEntryDialog.m_DialogTitleStaticText2 = "series of images to";
					//Pass control to dialog box and display
					int nResponse = IAlphaEntryDialog.DoModal();
					//dialog box is now closed, if user pressed select do this
					//if user pressed cancel, do nothing
					if (nResponse == IDOK)
					{
						bool TempOKToWriteFile = true;
						vSaveImagesToFilesName = IAlphaEntryDialog.vEditString;

						BYTE TempPosition = vSaveImagesToFilesName.Find(_T("."));
						if (TempPosition != 255)
						{
							BYTE TempLength = vSaveImagesToFilesName.GetLength();	
							vSaveImagesToFilesName.Delete(TempPosition,TempLength - TempPosition);
						}
						
						CString TempString = vSaveImagesToFilesName + "1.BMP";
						TempString = vLocalConfigurationData->vScanTracImagesDirectory + TempString;

						CFileStatus TempFileStatus;
						if (CFile::GetStatus(TempString, TempFileStatus))
						{
							CYesNoDialog TempYesNoDialog;
							CString TempDialogString("\n\nFile: ");
							TempDialogString = "\n\n" + vSaveImagesToFilesName + 
								" Image Series already exists.\nOverwrite files as needed?";
							TempYesNoDialog.vNoticeText = TempDialogString;
							TempYesNoDialog.vQuestionType = cConfirmQuestion;
							TempYesNoDialog.vYesButtonText = "Overwrite Files";
							int TempResult = TempYesNoDialog.DoModal();
							if (TempResult != IDOK)
								TempOKToWriteFile = false;
						}
						if (TempOKToWriteFile)
						{
							if (!vShowingLiveImages)
								StartShowingLiveImages();
							vSaveImagesToFiles = TempSaveImagesToFiles;
						}
					}
					else 
					if (nResponse == 10)
					{
						//Main Menu button pressed
						Sleep(1); //is the equivelent of a yeild statement;
						PrepareAndExit(10);
					}
				}
			}
			else 
			if (nResponse == 10)
			{
				//Main Menu button pressed
				Sleep(1); //is the equivelent of a yeild statement;
				PrepareAndExit(10);
			}
		}
	}
	else 
	{
		if (vSubFunction1ButtonEnable == true)
			OperatorSelectCurrentInspection();
		else
			ShowNoMouseClickDialogMessage();
	}
}

void CNewImageDisplayDialog::OnSubFunction2Button() 
{
	//vGlobalMouseMoveCounter = 0;
	if (vEditROIBoundaries)
	{
		if (!vRecievedButtonPress)
		{
			if ((vMouseState == 0) || ((vMouseState > 8) && (vMouseState < 17)))	//if mouse is idle or in double click toggle on mode
			{
				vMouseState = 2;	//set mouse to on while pressed down
				int TimerResult = SetTimer(vMouseDownRepeatTimerHandle,200,NULL);
				if (!TimerResult)
					ReportErrorMessage("Error-Second Timer Failed",cEMailInspx,32000);
			}
		}
		
		if (vInspectionShowing)
		if (vInspectionShowing->vROIPixelTop > vInspectionShowing->vROIPixelBottom + 1)
		{
			vInspectionShowing->vROIPixelTop--; 
			SetROILines();
		}
	}
	else
	if ((vShowThresholdMenu) && (vTweakingThreshold))
	{	//Add 1 to Threshold
		if (!vRecievedButtonPress)
		{
			if ((vMouseState == 0) || ((vMouseState > 8) && (vMouseState < 17)))	//if mouse is idle or in double click toggle on mode
			{
				vMouseState = 2;	//set mouse to on while pressed down
				int TimerResult = SetTimer(vMouseDownRepeatTimerHandle,200,NULL);
				if (!TimerResult)
					ReportErrorMessage("Error-Second Timer Failed",cEMailInspx,32000);
			}
		}
		
		vThreshold = vInspectionShowing->ConvertToWeight(vThreshold);

		if (vThreshold < vMaximumThreshold - 1)
			vThreshold = vThreshold + 1;
		else
			vThreshold = vMaximumThreshold;

		vThreshold = vInspectionShowing->ConvertWeightToBrightness(vThreshold);
		//vThreshold = ((vThreshold - vInspectionShowing->vCheckWeighBFactor) / vInspectionShowing->vCheckWeighMFactor);

		vChangeMade = true;
		DisplayThreshold();
	}
	else
	if (vRejectsImageDisplay)
	{  //Filter 2 Button was pressed
		ToggleFilter(2 + vFirstInspectionToView);
	}
	else
	if (vImageDisplayOptions)
	{
		SaveImageToFile(false, true);
	}
	else
	if (vImproveLearnMenu)
	{  
		//revert to original Learn Permanently
		if ((vLocalSystemData->vITIPCDig->HaveLearnImageFile("=Process 2o")) &&
			(vGlobalCurrentProduct) && (vGlobalCurrentProduct->vAddToLearnCount))
		{
			vLocalSystemData->vITIPCDig->RestoreOriginalStructurePermanently();
			if (vLocalConfigurationData->vAutoImproveEnabled)
			{
				vLocalSystemData->vITIPCDig->ClearImproveLearn(false);//clear AutoImprove Learn so does not over write
			}

			vLocalSystemData->vITIPCDig->vUsingOriginalLearnTemporarily = false;
			UpdateButtons();

			CNoticeDialog TempNoticeDialog;
			CString TempString = "\n\n\nReverteded Original Learn Permanently.";
			//TempString.LoadString(IDS_RejectAddedToLearn);//"\n\n\nReject Added To Learn."
			TempNoticeDialog.vNoticeText = TempString;
			TempNoticeDialog.vType = cInformationMessage;
			TempNoticeDialog.vAutoClose = 3000;
			TempNoticeDialog.DoModal();
			WaitThenDrawImageOnScreen();
			UpdateThresholdDisplay();
		}
	}
	else
	if (vEditWhileRunning)// || ((vImproveLearnMenu) && (PasswordOK(cTemporaryInspxPassword,false))))
	{//edit ROI Lines
		if (vInspectionShowing)
		if (PasswordOK(cSetupPassword,true))
		{
			if (vInspectionShowing->vInspectionNameForWeightTrendingPointer)
			{
				CNoticeDialog TempNoticeDialog;
				CString TempString = "\n\n\nThis inspection uses inspection:\n" + vInspectionShowing->vInspectionNameForWeightTrendingPointer->vName + "\nfor its ROI";
				TempNoticeDialog.vNoticeText = TempString;
				TempNoticeDialog.vType = cInformationMessage;
				TempNoticeDialog.DoModal();
			}
			else
			if (vHaveAnImageToShow)
			{
				if (vShowingLiveImages)
					StopShowingLiveImages();
				SaveOriginalROIValues();
				vEditROIBoundaries = true;
				this->SetWindowText(_T("Rejects ROI"));
				StartSensitivity();

				vShowThreshold = false;
				vEditWhileRunning = false;
				m_SubFunction1Button.ShowNavigationArrow(cUpArrowSmall);
				m_SubFunction2Button.ShowNavigationArrow(cDownArrowSmall);
				m_SubFunction3Button.ShowNavigationArrow(cUpArrowSmall);
				m_SubFunction4Button.ShowNavigationArrow(cDownArrowSmall);
				m_SubFunction5Button.ShowNavigationArrow(cLeftArrowSmall);
				m_SubFunction6Button.ShowNavigationArrow(cRightArrowSmall);
				m_SubFunction7Button.ShowNavigationArrow(cLeftArrowSmall);
				m_SubFunction8Button.ShowNavigationArrow(cRightArrowSmall);

				m_SubFunction1Button.Invalidate();
				m_SubFunction2Button.Invalidate();
				m_SubFunction3Button.Invalidate();
				m_SubFunction4Button.Invalidate();
				m_SubFunction5Button.Invalidate();
				m_SubFunction6Button.Invalidate();
				m_SubFunction7Button.Invalidate();
				m_SubFunction8Button.Invalidate();
			
				m_SubFunction1Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
				m_SubFunction2Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
				m_SubFunction3Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
				m_SubFunction4Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
				m_SubFunction5Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
				m_SubFunction6Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
				m_SubFunction7Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
				m_SubFunction8Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
				vDialogTitleString.LoadString(IDS_EditROI);
				SetDlgItemText(IDC_DialogTitleStaticText1,vDialogTitleString);
				UpdateButtons();
				WaitThenDrawImageOnScreen();
			}
			else
			{
				CWnd *TempWindow = NULL;
				TempWindow = FindWindow(NULL, _T("AutoSetup"));
				if (TempWindow) 
					OnFunction1Button();
				else
				{
					//edit ROI Lines button was pressed
					CAutoSetupImageDisplayDialog IAutoSetupImageDisplayDialog;
					IAutoSetupImageDisplayDialog.vLocalConfigurationData = vLocalConfigurationData;
					IAutoSetupImageDisplayDialog.vLocalSystemData = vLocalSystemData;
					IAutoSetupImageDisplayDialog.vMainWindowPointer = vMainWindowPointer;
					IAutoSetupImageDisplayDialog.vShowReturnToEditMenu = true;
					IAutoSetupImageDisplayDialog.vAutoSetupType = cEditInspectionAutoSetup;
					IAutoSetupImageDisplayDialog.vMode = cSetROI;
					IAutoSetupImageDisplayDialog.vInspectionEditing = vInspectionShowing;
					IAutoSetupImageDisplayDialog.vLocalProductCollection = vLocalProductCollection;
					//when go into auto setup, it will go into inspection setup
					//6/9/04
					CString TempInspectionName("");
					if (vInspectionShowing)
						TempInspectionName = vInspectionShowing->vName;

					BYTE TempEnableEjectors = vLocalConfigurationData->vEnableEjectors;
					vInspectionShowing = NULL;
					int nResponse = IAutoSetupImageDisplayDialog.DoModal();

					//if ((nResponse == IDOK) || (nResponse == 20))
					{
						// TODO: Place code here to handle when the dialog is accepted
						if (vLocalSystemData->vCurrentProductChanged)
						{
							/*
							if (vLocalSystemData->vSystemRunMode == cStoppedSystemMode)
							{
								vMainWindowPointer->SetupProduct(vGlobalCurrentProduct, false);
								vMainWindowPointer->ResetCounters();
								vLocalSystemData->vInspectionNameChanged = false;
								vLocalSystemData->vInspectionNameMoved = false;
							}
							else
							*/
							{
								if (vLocalSystemData->vInspectionNameChanged)
								{
									vInspectionShowing = NULL;
									vInspectionNumberShowing = 0;
									if (vGlobalCurrentProduct->vNumberOfInspections)
										vInspectionShowing = vGlobalCurrentProduct->vInspection[0];
									vLocalSystemData->vInspectionNameChanged = false;
									vMainWindowPointer->SetupRejectReasonNames();
									if (vLocalSystemData->vInspectionNameMoved)
									{
										vLocalSystemData->vInspectionNameMoved = false;
										vMainWindowPointer->MakeProductionReportIfNeeded();
										vMainWindowPointer->ResetCounters();
									}
								}
							}
						}
						if (TempEnableEjectors != vLocalConfigurationData->vEnableEjectors)
						{
							CString TempProductName = *vGlobalCurrentProduct->GetProductName();
							CYesNoDialog TempYesNoDialog;
							CString TempText("");//("\n\nEjectors were disabled when entered Edit While Running\nRe-enable Ejectors Now?");
							TempText.LoadString(IDS_EjectorsweredisabledwhenenteredEditWhileRunning);
							TempYesNoDialog.vNoticeText = TempText;
							CString TempText1("");
							TempText1.LoadString(IDS_EnableEjectors);
							TempYesNoDialog.vYesButtonText = TempText1;
							TempText1.LoadString(IDS_KeepDisabled);
							TempYesNoDialog.vNoButtonText = TempText1;
							TempYesNoDialog.vQuestionType = cConfirmQuestion;
							int TempResult = TempYesNoDialog.DoModal();
							if (TempResult == IDOK)
							{
								//vLocalConfigurationData->vEnableEjectors = true;
								//vGlobaluCSerialPort->EnableDisableEjectors(vLocalConfigurationData->vEnableEjectors);
								//vMainWindowPointer->UpdateDisableEjectorsButton();
								vMainWindowPointer->EnableDisableEjectors(true);
								ReportErrorMessage("Ejectors enabled by operator leaving Setup", cUserChanges,0);
							}
						}
					}
					//6/9/04
					vInspectionShowing = vGlobalCurrentProduct->vInspection[0];
					vInspectionNumberShowing = 0;
					if (TempInspectionName != "")
					{
						for (BYTE TempLoop = 1; TempLoop < vGlobalCurrentProduct->vNumberOfInspections; TempLoop++)
						if (vGlobalCurrentProduct->vInspection[TempLoop])
						if (vGlobalCurrentProduct->vInspection[TempLoop]->vName == TempInspectionName)
						{
							vInspectionShowing = vGlobalCurrentProduct->vInspection[TempLoop];
							vInspectionNumberShowing = TempLoop;
						}
					}
					SetInspectionShowing(vInspectionShowing);
					WaitThenDrawImageOnScreen();
				}
			}
		}
		else
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\n\nNo current inspection to adjust ROI";
			TempNoticeDialog.vType = cErrorMessage;
			TempNoticeDialog.DoModal();
		}
		m_InspectionThreshold.ShowWindow(SW_SHOW);
		m_SubFunction3Display.ShowWindow(SW_HIDE);
		m_SubFunction4Display.ShowWindow(SW_HIDE);
		UpdateThresholdDisplay();
	}
	else
	if (PasswordOK(cSuperInspxPassword,false))
	{
		if ((vHaveAnImageToShow) && (!vShowAlternateImage))
			ShowNextProcess(false);
		else
			ShowNeedImageDialogMessage();
	}
	else if (vSubFunction2ButtonEnable == false)
	{
		ShowNoMouseClickDialogMessage();
	}
}

void CNewImageDisplayDialog::OnSubFunction3Button() 
{
	//vGlobalMouseMoveCounter = 0;
	if ((vEditROIBoundaries) && (vSubFunction3ButtonEnable == true))
	{
		if (!vRecievedButtonPress)
		{
			if ((vMouseState == 0) || ((vMouseState > 8) && (vMouseState < 17)))	//if mouse is idle or in double click toggle on mode
			{
				vMouseState = 3;	//set mouse to on while pressed down
				int TimerResult = SetTimer(vMouseDownRepeatTimerHandle,200,NULL);
				if (!TimerResult)
					ReportErrorMessage("Error-Second Timer Failed",cEMailInspx,32000);
			}
		}
		
		if (vInspectionShowing)
		if ((vInspectionShowing->vInspectionType == cGlass13mmContaminantInspection) && (vInspectionShowing->vPerformGlassAlgorithm == 1) &&
			(vInspectionShowing->vROIPixelBottom < 15))
		{ //glass in body inspection, must go to pixel 1 if below pixel 20
			vInspectionShowing->vROIPixelBottom = 15; 
			vInspectionShowing->vRemoveJarPunt = false;
			SetROILines();

			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\n\nMoved Bottom ROI above Push Up.";
			TempNoticeDialog.vType = cInformationMessage;
			TempNoticeDialog.vAutoClose = 2000;
			TempNoticeDialog.DoModal();
		}
		else
		if (vInspectionShowing->vROIPixelBottom < vInspectionShowing->vROIPixelTop - 1)
		{
			vInspectionShowing->vROIPixelBottom++; 
			SetROILines();
		}
	}
	else
	if ((vShowThresholdMenu) && (vTweakingThreshold))
	{	//Subtract 1 from Threshold
		if (!vRecievedButtonPress)
		{
			if ((vMouseState == 0) || ((vMouseState > 8) && (vMouseState < 17)))	//if mouse is idle or in double click toggle on mode
			{
				vMouseState = 3;	//set mouse to on while pressed down
				int TimerResult = SetTimer(vMouseDownRepeatTimerHandle,200,NULL);
				if (!TimerResult)
					ReportErrorMessage("Error-Second Timer Failed",cEMailInspx,32000);
			}
		}
		vThreshold = vInspectionShowing->ConvertToWeight(vThreshold);

		if (vThreshold > 1)
			vThreshold = vThreshold - 1;
		else
			vThreshold = 0;

		vThreshold = vInspectionShowing->ConvertWeightToBrightness(vThreshold);
		//vThreshold = ((vThreshold - vInspectionShowing->vCheckWeighBFactor) / vInspectionShowing->vCheckWeighMFactor);

		vChangeMade = true;
		DisplayThreshold();
	}
	else
	if (vRejectsImageDisplay)
	{  //Filter 3 Button was pressed
		ToggleFilter(3 + vFirstInspectionToView);
	}
	else
	if (vImproveLearnMenu)
	{  
	}
	else
	if ((vEditWhileRunning) && (vSubFunction3ButtonEnable == true))// || ((vImproveLearnMenu) && (PasswordOK(cTemporaryInspxPassword,false))))
	{ //minimum defect size
		if (((PasswordOK(cTemporaryInspxPassword,false)) && (vInspectionShowing->vAllowChangingAdvancedOptions)) || (PasswordOK(cSuperInspxPassword,false)) || (vInspectionShowing->vInspectionType == cItemCountInspection))
			EditInspectionSettings();
		else
		if ((vInspectionShowing) && ((vInspectionShowing->vInspectionType == cItemCountInspection) || (vInspectionShowing->vInspectionType == cSizeInspection) || (vInspectionShowing->vInspectionType == cMissingLidInspectionOnSize) || 
			(vInspectionShowing->vInspectionType == cFloodedInspection)))
			OperatorSetInspectionSize();
		else
		if ((PasswordOK(cAdvancedSetupPassword,false)) && (vInspectionShowing->vHasAMinimumSize))
			OperatorSetInspectionSize();
	}
	else
	if (vImageDisplayOptions)
	{
		LoadImageFromFile();
	}
	else
	if (PasswordOK(cSuperInspxPassword,false))
	{
		if ((vHaveAnImageToShow) && (!vShowAlternateImage))
			ShowNextProcess(true);
		else
			ShowNeedImageDialogMessage();
	}	
	else if (vSubFunction3ButtonEnable == false)
	{
		ShowNoMouseClickDialogMessage();
	}
}

void CNewImageDisplayDialog::OnSubFunction4Button() 
{
	//vGlobalMouseMoveCounter = 0;
	if (vSubFunction4ButtonEnable == true)
	{
		if (vEditROIBoundaries)
		{
			if (!vRecievedButtonPress)
			{
				if ((vMouseState == 0) || ((vMouseState > 8) && (vMouseState < 17)))	//if mouse is idle or in double click toggle on mode
				{
					vMouseState = 4;
					int TimerResult = SetTimer(vMouseDownRepeatTimerHandle,200,NULL);
					if (!TimerResult)
						ReportErrorMessage("Error-Second Timer Failed",cEMailInspx,32000);

				}
			}
			if (vInspectionShowing)
			if (vInspectionShowing->vROIPixelBottom)
			if ((vInspectionShowing->vInspectionType == cGlass13mmContaminantInspection) && (vInspectionShowing->vPerformGlassAlgorithm == 1) &&
				(vInspectionShowing->vROIPixelBottom <= 15))
			{ //glass in body inspection, must go to pixel 1 if below pixel 20
				vInspectionShowing->vROIPixelBottom = 1; 
				vInspectionShowing->vRemoveJarPunt = true;
				SetROILines();

				CNoticeDialog TempNoticeDialog;
				TempNoticeDialog.vNoticeText = "\n\n\nMoved Bottom ROI to bottom below Push Up.";
				TempNoticeDialog.vType = cInformationMessage;
				TempNoticeDialog.vAutoClose = 2000;
				TempNoticeDialog.DoModal();
			}
			else
			{
				vInspectionShowing->vROIPixelBottom--; 
				SetROILines();
			}
		}
		else
		if (vShowThresholdMenu)
		{	//Subtract 10 from Threshold
			//if (!vRecievedButtonPress)
			//{
			//	if ((vMouseState == 0) || ((vMouseState > 8) && (vMouseState < 17)))	//if mouse is idle or in double click toggle on mode
			//	{
			//		vMouseState = 4;	//set mouse to on while pressed down
			//		int TimerResult = SetTimer(vMouseDownRepeatTimerHandle,200,NULL);
			//		if (!TimerResult)
			//			ReportErrorMessage("Error-Second Timer Failed",cEMailInspx,32000);
			//	}
			//}
			vTweakingThreshold = true;
			vThreshold = vInspectionShowing->ConvertToWeight(vThreshold);

			if (vThreshold > 10)
				vThreshold = vThreshold - 10;
			else
				vThreshold = 0;

			vThreshold = vInspectionShowing->ConvertWeightToBrightness(vThreshold);
			//vThreshold = (vThreshold - vInspectionShowing->vCheckWeighBFactor) / vInspectionShowing->vCheckWeighMFactor;
			vChangeMade = true;
			DisplayThreshold();
		}
		else
		if (vRejectsImageDisplay)
		{  //Filter 4 Button was pressed
			ToggleFilter(4 + vFirstInspectionToView);
		}
		else
		if (vImproveLearnMenu)
		{  
			if (vShowType == cShowTypeContaminant2NoDilation)
				vShowType = cShowTypeContaminant;
			else
			if ((vShowType == cShowTypeContaminant2) && (vLocalSystemData->vITIPCDig->vProductContaminant2StructureImageWithNoDilation))
				vShowType = cShowTypeContaminant2NoDilation;
			else
			if (vShowType == cShowTypeContaminant2)
				vShowType = cShowTypeContaminant;
			else
			{
				if ((vLocalSystemData->vITIPCDig->vProductContaminant2StructureImage) && (vLocalSystemData->vITIPCDig->vProductContaminant2StructureImage->vGreyImage))
					vShowType = cShowTypeContaminant2;
				else
				{
					CNoticeDialog TempNoticeDialog;
					TempNoticeDialog.vNoticeText = "\n\n\nNo Contaminant 2 Image available.";
					TempNoticeDialog.vType = cInformationMessage;
					TempNoticeDialog.DoModal();
				}
			}
			WaitThenDrawImageOnScreen();
			UpdateButtons();
		}
		else
		if (vEditWhileRunning)// || (() && (PasswordOK(cTemporaryInspxPassword,false))))
		{ //show reject threshold
			m_InspectionThreshold.ShowWindow(SW_SHOW);
			m_SubFunction3Display.ShowWindow(SW_HIDE);
			m_SubFunction4Display.ShowWindow(SW_HIDE);
			if (vInspectionShowing)
			{  //show product defect threshold
				if (PasswordOK(cSetupPassword,true))
				{	//Show Contaminant Threshold / show no threshold
					if (vShowingLiveImages)
						StopShowingLiveImages();
					vChangeMade = false;
					vEditWhileRunning = false;
					//if (vImproveLearnMenu)
					//{
						//vGlobalCurrentProduct->vTypesOfRejectsToView = vSaveTypesOfRejectsToView;
						//vImproveLearnMenu = false;
						//vReturnToImproveLearnMenu = true;

						//SetInspectionShowing(vGlobalCurrentProduct->vInspection[0]);
						//should it set current inspection to reject reason for current image?
						//vLocalSystemData->vITIPCDig->vRejectBufferShowing = -1;
						//ShowNextReject(true, false);
					//}
					if (vInspectionShowing)
					if ((vInspectionShowing->vInspectionType == cUnderfillByWeightInspection) ||
						(vInspectionShowing->vInspectionType == cCheckWeighInspection))
					{
						vThreshold = vInspectionShowing->vDensityThresholdUpper;
						vMaximumThreshold = vMaximumUnderfillByWeightThreshold;
						vShowingUpperThreshold = true;
					}
					else
					if (vInspectionShowing->vInspectionType == cOverfillByWeightInspection)
					{
						vThreshold = vInspectionShowing->vDensityThresholdLower;
						vMaximumThreshold = vMaximumUnderfillByWeightThreshold;
						vShowingUpperThreshold = false;
					}
					else
					if ((vInspectionShowing->vInspectionType == cDensityInspection) ||
						//(vInspectionShowing->vInspectionType == cCheckWeighInspection) ||
						(vInspectionShowing->vInspectionType == cMissingLidInspection) ||
						(vInspectionShowing->vInspectionType == cUnderfillInspection) ||
						(vInspectionShowing->vInspectionType == cOverfillInspection) ||
						(vInspectionShowing->vInspectionType == cStandardDeviationInspection))
					{
						vMaximumThreshold = 255;
						if (vInspectionShowing->vHasDensityUpperThreshold)
						{
							vThreshold = vInspectionShowing->vDensityThresholdUpper;
							vShowingUpperThreshold = true;
						}
						else
						{
							vThreshold = vInspectionShowing->vDensityThresholdLower;
							vShowingUpperThreshold = false;
						}
					}
					else
					{
						vThreshold = vInspectionShowing->vThreshold;
						vMaximumThreshold = 255;
					}
					vShowThresholdMenu = true;
					this->SetWindowText(_T("Rejects Threshold"));
					StartSensitivity();

					if (!PasswordOK(cSuperInspxPassword,false))
					{
						vShowingProcess = 0;
						ShowNextProcess(false);  //show the last step of image processing which shows the defects
					}
					else
						vShowThreshold = true; //just do a straight Threshold on the image, don't do the inspection image processing

					vTweakingThreshold = true;
					vDialogTitleString.LoadString(IDS_EditThreshold);
					SetDlgItemText(IDC_DialogTitleStaticText1,vDialogTitleString);
					UpdateButtons();
					DisplayThreshold();
					UpdateDisplayIndicators();
				}
			}
			else
			{
				CNoticeDialog TempNoticeDialog;
				TempNoticeDialog.vNoticeText = "\n\n\nNo Inspection with a set threshold selected";
				TempNoticeDialog.vType = cErrorMessage;
				TempNoticeDialog.DoModal();
			}
			//m_InspectionThreshold.ShowWindow(SW_SHOW);
			//m_SubFunction3Display.ShowWindow(SW_HIDE);
			//m_SubFunction4Display.ShowWindow(SW_HIDE);
			UpdateThresholdDisplay();
		}
		else
		if (vImageDisplayOptions)
		{
			if (PasswordOK(cTemporaryInspxPassword,false))
			{
				vLocalSystemData->vITIPCDig->vImageShowCrossHair++;
				//if (vLocalSystemData->vITIPCDig->vImageShowCrossHair > 4)
				if (vLocalSystemData->vITIPCDig->vImageShowCrossHair > 3) //does not show cross hair
					vLocalSystemData->vITIPCDig->vImageShowCrossHair = 0;
				WaitThenDrawImageOnScreen();
				UpdateDisplayIndicators();
				UpdateButtons();
			}
		}
		else
				//correct pixels that are out of whack on a pipeline
		if ((vAnalysisImageDisplay) && (!PasswordOK(cSuperInspxPassword,false)) && 
			((vLocalConfigurationData->vScanTracType == cLegatoScanTrac) || (vLocalConfigurationData->vScanTracType == cFermataScanTrac)))//pipeline ScanTrac
		{
			if (vHaveAnImageToShow)
			{
				CorrectPipeEdgePixels();
			}
			else
			{
				CNoticeDialog TempNoticeDialog;
				TempNoticeDialog.vNoticeText = "\n\n\nNeed a good image to correct pixels";
				TempNoticeDialog.vType = cErrorMessage;
				TempNoticeDialog.DoModal();
			}
		}
		else
		{  //
			if (PasswordOK(cSuperInspxPassword,false))
			{
				//bool TempHaveBackground = 
				//			vGlobalCurrentProduct->ProductHasBackgroundFile(vGlobalCurrentDirectory);
				if ((vLocalSystemData->vITIPCDig->vProductAverageImage) || 
					((vLocalSystemData->vITIPCDig->vProductBackgroundImage)) ||
					(vLocalSystemData->vITIPCDig->vAverageImage))
				{
					//ClearAllDisplayOptions(true);
					vTweakingThreshold = false;
					vDisplayEnhancedImage = false;
					vPreSmoothImage = 0;
					vRemoveJarPunt = 0;
					vFillInLightSpots = 0;
					vShowSubtractAverage = false;
					vShowAddStructure = false;
					vShowThreshold = false;
					vShowAddHistogram = false;
					//vShowImageFactors = false;
					vShowReferenceAdjustedImage = 0;
					vShowLineDerivative = 0;
					vLookForDarkSpots = 0;
					vTakeDerivative = 0;
					vFindSpotInSubROI = 0;
					vDoContaminant22Algorithm = 0;
					vTakeDifferenceFromMedian = 0;
					vTakeDifferenceFromMedianFilter = 0;
					vBonesInspection = 0;
					vFillBackground = 0;
					vUseVariabiltyImage = 0;
					vFindDarkSpotOnLine = 0;
					vTrimEdgesOfJarAtROI = 0;
					vShowThreshold = false;
					vRequire2InARow = 0;
					vShowDilationCount = 0;
					vShowErodeCount = 0;
					vShowErodeCountAtStart = 0;
					vShowDilationCountAtEnd = 0;
					vShowErodeCountAtEnd = 0;
					vShowBluredImage = 0;
					vIPContaminant = 0;
					vShowTopHatImage = 0;
					vIntensityAdjust = 0;
					vShowClipboard = false;
					//vPreSelectDarkSpotsInImage = 0;

					vShowOriginalImageFactor = 0;
					vShowEnhancedImageFactor = 0;

					vShowImproveLearnPreview = 0;
					vShowOriginalStructure = false;
					vShowDeltaOriginalToCurrentStructure = 0;
					vShowDeltaCurrentPreviewStructure = 0;

					ShowNextProcessData();
					UpdateButtons();
				}
			}
		}
	}	
	else //if (vSubFunction4ButtonEnable == false)
	{
		ShowNoMouseClickDialogMessage();
	}
}

void CNewImageDisplayDialog::OnSubFunction5Button() 
{
	//vGlobalMouseMoveCounter = 0;
	if (vImproveLearnMenu)
	{
		if (!vLocalSystemData->vITIPCDig->vImproveLearnCount)
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\n\nNo Improve Learn To Add";
			TempNoticeDialog.vType = cInformationMessage;
			TempNoticeDialog.DoModal();
		}
		else
		{
			//Add Improve To Learn
			CYesNoDialog TempYesNoDialog;
			TempYesNoDialog.vNoticeText = _T("\n\nAdd Improve Learn to Current Learn?");
			TempYesNoDialog.vQuestionType = cConfirmAbandon;
			TempYesNoDialog.vYesButtonText = "Add Improve Learn";
			int TempResult = TempYesNoDialog.DoModal();
			if (TempResult == IDOK)
			{
				vLocalSystemData->vITIPCDig->MakeUndoBackupOfLearnStructureFiles(vGlobalCurrentProduct);

				if ((vLocalSystemData->vITIPCDig->vImproveLearnStructureImage) && (vLocalSystemData->vITIPCDig->vImproveLearnStructureImage->vGreyImage))
				if ((vLocalSystemData->vITIPCDig->vProductContaminant2StructureImage) && (vLocalSystemData->vITIPCDig->vProductContaminant2StructureImage->vGreyImage))
				{
					CopyMemory(vLocalSystemData->vITIPCDig->vProductStructureImage->vGreyImage, vLocalSystemData->vITIPCDig->vImproveLearnStructureImage->vGreyImage, vLocalSystemData->vITIPCDig->vOriginalBufferSize);

					vLocalSystemData->vITIPCDig->SaveStructureImageToFile(vGlobalCurrentProduct, false);

					CopyMemory(vLocalSystemData->vITIPCDig->vProductContaminant2StructureImage->vGreyImage, vLocalSystemData->vITIPCDig->vImproveLearnContaminant2StructureImage->vGreyImage,
						vLocalSystemData->vITIPCDig->vOriginalBufferSize);

					if ((vLocalSystemData->vITIPCDig->vProductContaminant2StructureImageWithNoDilation) && (vLocalSystemData->vITIPCDig->vProductContaminant2StructureImageWithNoDilation->vGreyImage))
					{ //current one has dilations in it, so must add to undilated one, then write to file as we only dilate when read from file which is undilated
						CopyMemory(vLocalSystemData->vITIPCDig->vProductContaminant2StructureImageWithNoDilation->vGreyImage, vLocalSystemData->vITIPCDig->vImproveLearnContaminant2StructureImage->vGreyImage,
							vLocalSystemData->vITIPCDig->vOriginalBufferSize);
					}

					vLocalSystemData->vITIPCDig->SaveContaminant2StructureImageToFile(vGlobalCurrentProduct, vLocalSystemData->vITIPCDig->vImproveLearnContaminant2StructureImage, false);

					if ((vLocalSystemData->vITIPCDig->vProductContaminant2StructureImageWithNoDilation) && (vLocalSystemData->vITIPCDig->vProductContaminant2StructureImageWithNoDilation->vGreyImage)) //must read back in to get dilated version
					if ((vGlobalLearnState) && (((vGlobalNormalLearnDilateTimes) && (vGlobalCurrentProduct->vSetupHistoryStructureCount > 19) &&
						(vGlobalCurrentProduct->vSetupHistoryStructureCount != vGlobalCurrentProduct->vSetupHistoryAverageCount)) ||
						((vGlobalQuickLearnDilateTimes) && (vGlobalCurrentProduct->vSetupHistoryStructureCount > 1) &&
						(vGlobalCurrentProduct->vSetupHistoryStructureCount == vGlobalCurrentProduct->vSetupHistoryAverageCount)))) //if the last learn was a normal learn, and not a quick learn
					if (vLocalSystemData->vITIPCDig->vContainerCollectionLock.Lock())
					{  //end of collecting structure, Save structure image
						vLocalSystemData->vITIPCDig->LoadStructureImagesFromFiles(true, false);
						vLocalSystemData->vITIPCDig->vContainerCollectionLock.Unlock();	
					}

					ReportErrorMessage("User did Manual Improve Learn For Product: " + vLocalSystemData->vCurrentProductName, cUserChanges,0);

					DWORD TempWord = vLocalSystemData->vITIPCDig->vImproveLearnCount + vGlobalCurrentProduct->vAddToLearnCount;

					vLocalSystemData->vITIPCDig->vNumberOfImagesInLastAddToLearn = vLocalSystemData->vITIPCDig->vImproveLearnCount;
					if (TempWord > 65535)
						TempWord = 65535;
					vGlobalCurrentProduct->vAddToLearnCount = (WORD)TempWord;

					ReportErrorMessage("Improve Learn By: " + dtoa(vLocalSystemData->vITIPCDig->vNumberOfImagesInLastAddToLearn,0), cUserChanges,0);

					vMainWindowPointer->SetLearnState();
					//don't change as it is the current learn now
					//vLocalSystemData->vITIPCDig->ClearImproveLearn(false);
					vLocalSystemData->vITIPCDig->vImproveLearnCount = 0;
					vLocalSystemData->vImproveLearnTotalContainers = 0;
				}

				//return to Rejects Menu
				ClearAllDisplayOptions(true);
				vImproveLearnMenu = false;
				vRejectsImageDisplay = true;
				this->SetWindowText(_T("Rejects"));
				vDialogTitleString.LoadString(IDS_RejectDisplay);
				SetDlgItemText(IDC_DialogTitleStaticText1,vDialogTitleString);
				UpdateButtons();
				UpdateDisplayIndicators();
				//causes crash UpdateDisplay(vDisplayingImageData);
				UpdateRejectCounts();
				ShowOrHideDisplays();
				m_InspectionThreshold.ShowWindow(SW_HIDE);

				SetInspectionShowing(vGlobalCurrentProduct->vInspection[0]);
				//should it set current inspection to reject reason for current image?
				vLocalSystemData->vITIPCDig->vRejectBufferShowing = -1;
				DisplayLocationEdge();
				ShowNextReject(true, true);
				if ((!vHaveAnImageToShow) || ((vNumberOfRejectsInLocalBuffer == 0) && (vAllRejectsHidden)))
					ShowNeedImageDialogMessage();
			}
		}
	}
	else
	if (vEditROIBoundaries)
	{
		if (!vRecievedButtonPress)
		{
			if ((vMouseState == 0) || ((vMouseState > 8) && (vMouseState < 17)))	//if mouse is idle or in double click toggle on mode
			{
				vMouseState = 5;	//set mouse to on while pressed down
				int TimerResult = SetTimer(vMouseDownRepeatTimerHandle,200,NULL);
				if (!TimerResult)
					ReportErrorMessage("Error-Second Timer Failed",cEMailInspx,32000);
			}
		}
		
		if (vInspectionShowing)
		if (vInspectionShowing->vROIPixelLeft)
		{
			vInspectionShowing->vROIPixelLeft--; 
			SetROILines();
		}
	}
	else
	if (vShowThresholdMenu)
	{
		
		if ((vInspectionShowing) && 
			((vInspectionShowing->vInspectionType == cCheckWeighInspection) ||
			(vInspectionShowing->vInspectionType == cUnderfillByWeightInspection) ||
			(vInspectionShowing->vInspectionType == cOverfillByWeightInspection) ||
			(vInspectionShowing->vInspectionType == cDensityInspection) ||
			(vInspectionShowing->vInspectionType == cStandardDeviationInspection)))
		{
			bool TempOKToChangeModes = true;
			if (vChangeMade)
			{
				CYesNoDialog TempYesNoDialog;
				CString TempString("");
				TempString = "";
				TempString.LoadString(IDS_TheThresholdhasbeenchangedAbandonthresholdchangeswithoutsaving);
				TempYesNoDialog.vNoticeText = TempString;
				TempYesNoDialog.vQuestionType = cConfirmAbandon;
				int TempResult = TempYesNoDialog.DoModal();
				if (TempResult != IDOK)
					TempOKToChangeModes = false;
			}
			if (TempOKToChangeModes)
			{
				vChangeMade = false;
				vShowingUpperThreshold = !vShowingUpperThreshold;
				if (vShowingUpperThreshold)
					vThreshold = vInspectionShowing->vDensityThresholdUpper;
				else
					vThreshold = vInspectionShowing->vDensityThresholdLower;
				UpdateButtons();
				UpdateDisplay(vDisplayingImageData);
				DrawImageOnScreen();
			}
		}
	}
	else
	if (vRejectsImageDisplay)
	{  //Filter 5 Button was pressed
		ToggleFilter(5 + vFirstInspectionToView);
	}
	else
	if (vImageDisplayOptions)
	{
		vLocalConfigurationData->vImageShowFixedROILines = 
			!vLocalConfigurationData->vImageShowFixedROILines;
		WaitThenDrawImageOnScreen();
		UpdateButtons();
		UpdateDisplayIndicators();
	}
	else
	if (vEditWhileRunning)
	{	//relearn
		CWnd *TempWindow = NULL;
		TempWindow = FindWindow(NULL, _T("AutoSetup"));
		if (TempWindow) 
			OnFunction1Button();
		else
		{
			//relearn menu button was pressed
			CAutoSetupImageDisplayDialog IAutoSetupImageDisplayDialog;
			IAutoSetupImageDisplayDialog.vLocalConfigurationData = vLocalConfigurationData;
			IAutoSetupImageDisplayDialog.vLocalSystemData = vLocalSystemData;
			IAutoSetupImageDisplayDialog.vMainWindowPointer = vMainWindowPointer;
			IAutoSetupImageDisplayDialog.vShowReturnToEditMenu = true;
			IAutoSetupImageDisplayDialog.vAutoSetupType = cEvaluateAutoSetup;
			//when go into auto setup, it will go into evaluate inspection
			IAutoSetupImageDisplayDialog.vMode = cLearnMenu;

			//6/9/04
			CString TempInspectionName("");
			if (vInspectionShowing)
				TempInspectionName = vInspectionShowing->vName;

			IAutoSetupImageDisplayDialog.vInspectionEditing = vInspectionShowing;
			IAutoSetupImageDisplayDialog.vLocalProductCollection = vLocalProductCollection;
			BYTE TempEnableEjectors = vLocalConfigurationData->vEnableEjectors;
			vInspectionShowing = NULL;
			int nResponse = IAutoSetupImageDisplayDialog.DoModal();

			//if ((nResponse == IDOK) || (nResponse == 20))
			{
				// TODO: Place code here to handle when the dialog is accepted
				if (vLocalSystemData->vCurrentProductChanged)
				{
					{
						if (vLocalSystemData->vInspectionNameChanged)
						{
							vInspectionShowing = NULL;
							vInspectionNumberShowing = 0;
							if (vGlobalCurrentProduct->vNumberOfInspections)
								vInspectionShowing = vGlobalCurrentProduct->vInspection[0];
							vLocalSystemData->vInspectionNameChanged = false;
							vMainWindowPointer->SetupRejectReasonNames();
							if (vLocalSystemData->vInspectionNameMoved)
							{
								vLocalSystemData->vInspectionNameMoved = false;
								vMainWindowPointer->MakeProductionReportIfNeeded();
								vMainWindowPointer->ResetCounters();
							}
						}
					}
				}
				if (TempEnableEjectors != vLocalConfigurationData->vEnableEjectors)
				{
					CString TempProductName = *vGlobalCurrentProduct->GetProductName();
					CYesNoDialog TempYesNoDialog;
					CString TempText("");//("\n\nEjectors were disabled when entered Edit While Running\nRe-enable Ejectors Now?");
					TempText.LoadString(IDS_EjectorsweredisabledwhenenteredEditWhileRunning);
					TempYesNoDialog.vNoticeText = TempText;
					CString TempText1("");
					TempText1.LoadString(IDS_EnableEjectors);
					TempYesNoDialog.vYesButtonText = TempText1;
					TempText1.LoadString(IDS_KeepDisabled);
					TempYesNoDialog.vNoButtonText = TempText1;
					TempYesNoDialog.vQuestionType = cConfirmQuestion;
					int TempResult = TempYesNoDialog.DoModal();
					if (TempResult == IDOK)
					{
						//vLocalConfigurationData->vEnableEjectors = true;
						//vGlobaluCSerialPort->EnableDisableEjectors(vLocalConfigurationData->vEnableEjectors);
						//vMainWindowPointer->UpdateDisableEjectorsButton();
						vMainWindowPointer->EnableDisableEjectors(true);
						ReportErrorMessage("Ejectors enabled by operator leaving Setup", cUserChanges,0);
					}
				}
			}
			//6/9/04
			vInspectionShowing = vGlobalCurrentProduct->vInspection[0];
			vInspectionNumberShowing = 0;
			if (TempInspectionName != "")
			{
				for (BYTE TempLoop = 1; TempLoop < vGlobalCurrentProduct->vNumberOfInspections; TempLoop++)
				if (vGlobalCurrentProduct->vInspection[TempLoop])
				if (vGlobalCurrentProduct->vInspection[TempLoop]->vName == TempInspectionName)
				{
					vInspectionShowing = vGlobalCurrentProduct->vInspection[TempLoop];
					vInspectionNumberShowing = TempLoop;
				}
			}
			SetInspectionShowing(vInspectionShowing);
			WaitThenDrawImageOnScreen();
		}
	}
	else
	{ 
		if (vSubFunction5ButtonEnable == false)
		{
			ShowNoMouseClickDialogMessage();
		}
		else
		{
			//vAnalysisImageDisplay
			//in image analysis display, so go to Display Image Options Menu
			vAnalysisImageDisplay = false;
			vImageDisplayOptions = true;
			this->SetWindowText(_T("Rejects Options"));
			vDialogTitleString.LoadString(IDS_DisplayOptions);
			SetDlgItemText(IDC_DialogTitleStaticText1,vDialogTitleString);
			UpdateButtons();
		}
	}
}

void CNewImageDisplayDialog::OnSubFunction6Button() 
{
	//vGlobalMouseMoveCounter = 0;
	if (vImproveLearnMenu)
	{ 
		//show delta original to current
		if (!vLocalSystemData->vITIPCDig->HaveLearnImageFile("=Process 2o"))
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\n\nNo Original Learn To Show";
			TempNoticeDialog.vType = cInformationMessage;
			TempNoticeDialog.DoModal();
		}
		else
		if (!vLocalSystemData->vITIPCDig->vProductStructureImage)
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\n\nNo Current Learn To Show";
			TempNoticeDialog.vType = cInformationMessage;
			TempNoticeDialog.DoModal();
		}
		else
		if (vShowDeltaOriginalToCurrentStructure)
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\n\nAlready showing this image";
			TempNoticeDialog.vType = cInformationMessage;
			TempNoticeDialog.DoModal();
		}
		else
		{
			BYTE TempShowType = vShowType;
			ClearAllDisplayOptions(true);
			vShowType = TempShowType;

			vShowDeltaOriginalToCurrentStructure = true;
			UpdateButtons();
			DrawImageOnScreen();
		}
	}
	else
	if (vEditROIBoundaries)
	{
		if (!vRecievedButtonPress)
		{
			if ((vMouseState == 0) || ((vMouseState > 8) && (vMouseState < 17)))	//if mouse is idle or in double click toggle on mode
			{
				vMouseState = 6;	//set mouse to on while pressed down
				int TimerResult = SetTimer(vMouseDownRepeatTimerHandle,200,NULL);
				if (!TimerResult)
					ReportErrorMessage("Error-Second Timer Failed",cEMailInspx,32000);
			}
		}
		
		if (vInspectionShowing)
		if ((vInspectionShowing->vROIPixelLeft < vInspectionShowing->vROIPixelRight - 2) && 
			(vInspectionShowing->vROIPixelRight > 2))
		{
			vInspectionShowing->vROIPixelLeft++; 
			SetROILines();
		}
	}
	else
	if (vShowThresholdMenu)
	{

		if (vInspectionShowing)
		{
			CNumericEntryDialog INumericEntryDialog;  
			//Set dialog box data titles and number value
			INumericEntryDialog.vIntegerOnly = false;
			INumericEntryDialog.vMaxValue = vMaximumThreshold;
			INumericEntryDialog.m_UnitsString = "Range 0 to " + dtoaWithCommas(vMaximumThreshold,0) + ", 0 = Disabled";
			INumericEntryDialog.vEditString = dtoaWithCommas(vInspectionShowing->ConvertToWeight(vThreshold),2);
			CString TempText("");
			TempText.LoadString(IDS_Inspection);
			INumericEntryDialog.m_DialogTitleStaticText1 = "Enter Threshold Value to Display for";
			INumericEntryDialog.m_DialogTitleStaticText2 = TempText + ": " + vInspectionShowing->vName;
			INumericEntryDialog.m_DialogTitleStaticText3 = "";
			TempText.LoadString(IDS_OriginalValue);
			INumericEntryDialog.m_DialogTitleStaticText4 = TempText + ": " + INumericEntryDialog.vEditString;
			//Pass control to dialog box and display
			int nResponse = INumericEntryDialog.DoModal();
			//dialog box is now closed, if user pressed select do this
			//if user pressed cancel, do nothing
			if (nResponse == IDOK)
			{
				if (vGlobalUseCommaForDecimalPoint)
					INumericEntryDialog.vEditString.Remove('.');
				else
					INumericEntryDialog.vEditString.Remove(',');
				INumericEntryDialog.vEditString.Remove('-');
				vThreshold = vInspectionShowing->ConvertWeightToBrightness(ATOF(INumericEntryDialog.vEditString));
				//if ((vInspectionShowing->vCheckWeighBFactor) && (vInspectionShowing->vCheckWeighMFactor) && (vInspectionShowing->vCheckWeighMFactor != 1))
				//	vThreshold = ((ATOF(INumericEntryDialog.vEditString) - vInspectionShowing->vCheckWeighBFactor) / vInspectionShowing->vCheckWeighMFactor);

				vChangeMade = true;
				DisplayThreshold();
				UpdateButtons();
			}
			else 
			if (nResponse == 10)
			{	//Main Menu button pressed
				Sleep(1); //is the equivelent of a yeild statement;
				PrepareAndExit(10);
			}
		}
	}
	else
	if (vRejectsImageDisplay)
	{  //Filter 6 Button was pressed
		ToggleFilter(6 + vFirstInspectionToView);
	}
	else
	if ((vEditWhileRunning) && (vSubFunction6ButtonEnable == true))
	{//edit Inspections
		CWnd *TempWindow = NULL;
		TempWindow = FindWindow(NULL, _T("AutoSetup"));
		if (TempWindow) 
			OnFunction1Button();
		else
		{
			//edit inspections button was pressed
			CAutoSetupImageDisplayDialog IAutoSetupImageDisplayDialog;
			IAutoSetupImageDisplayDialog.vLocalConfigurationData = vLocalConfigurationData;
			IAutoSetupImageDisplayDialog.vLocalSystemData = vLocalSystemData;
			IAutoSetupImageDisplayDialog.vMainWindowPointer = vMainWindowPointer;
			IAutoSetupImageDisplayDialog.vShowReturnToEditMenu = true;
			IAutoSetupImageDisplayDialog.vAutoSetupType = cEditInspectionAutoSetup;
			IAutoSetupImageDisplayDialog.vMode = cSetInspections;
			IAutoSetupImageDisplayDialog.vInspectionEditing = NULL; //vInspectionShowing;
			IAutoSetupImageDisplayDialog.vLocalProductCollection = vLocalProductCollection;
			//when go into auto setup, it will go into inspection setup
			//6/9/04
			CString TempInspectionName("");
			if (vInspectionShowing)
				TempInspectionName = vInspectionShowing->vName;

			BYTE TempEnableEjectors = vLocalConfigurationData->vEnableEjectors;
			vInspectionShowing = NULL;
			int nResponse = IAutoSetupImageDisplayDialog.DoModal();

			//if ((nResponse == IDOK) || (nResponse == 20))
			{
				// TODO: Place code here to handle when the dialog is accepted
				if (vLocalSystemData->vCurrentProductChanged)
				{
					//if (vLocalSystemData->vSystemRunMode == cStoppedSystemMode)
					//{
						//vMainWindowPointer->SetupProduct(vGlobalCurrentProduct, false);
						//vMainWindowPointer->ResetCounters();
					//	vLocalSystemData->vInspectionNameChanged = false;
					//	vLocalSystemData->vInspectionNameMoved = false;
					//}
					//else
					{
						if (vLocalSystemData->vInspectionNameChanged)
						{
							vInspectionShowing = NULL;
							vInspectionNumberShowing = 0;
							if (vGlobalCurrentProduct->vNumberOfInspections)
								vInspectionShowing = vGlobalCurrentProduct->vInspection[0];
							vLocalSystemData->vInspectionNameChanged = false;
							vMainWindowPointer->SetupRejectReasonNames();
							if (vLocalSystemData->vInspectionNameMoved)
							{
								vLocalSystemData->vInspectionNameMoved = false;
								vMainWindowPointer->MakeProductionReportIfNeeded();
								vMainWindowPointer->ResetCounters();
							}
						}
					}
				}
				if (TempEnableEjectors != vLocalConfigurationData->vEnableEjectors)
				{
					CString TempProductName = *vGlobalCurrentProduct->GetProductName();
					CYesNoDialog TempYesNoDialog;
					CString TempText("");//("\n\nEjectors were disabled when entered Edit While Running\nRe-enable Ejectors Now?");
					TempText.LoadString(IDS_EjectorsweredisabledwhenenteredEditWhileRunning);
					TempYesNoDialog.vNoticeText = TempText;
					CString TempText1("");
					TempText1.LoadString(IDS_EnableEjectors);
					TempYesNoDialog.vYesButtonText = TempText1;
					TempText1.LoadString(IDS_KeepDisabled);
					TempYesNoDialog.vNoButtonText = TempText1;
					TempYesNoDialog.vQuestionType = cConfirmQuestion;
					int TempResult = TempYesNoDialog.DoModal();
					if (TempResult == IDOK)
					{
						//vLocalConfigurationData->vEnableEjectors = true;
						//vGlobaluCSerialPort->EnableDisableEjectors(vLocalConfigurationData->vEnableEjectors);
						//vMainWindowPointer->UpdateDisableEjectorsButton();
						vMainWindowPointer->EnableDisableEjectors(true);
						ReportErrorMessage("Ejectors enabled by operator leaving Setup", cUserChanges,0);
					}
				}
			}
			//6/9/04
			vInspectionShowing = vGlobalCurrentProduct->vInspection[0];
			vInspectionNumberShowing = 0;
			if (TempInspectionName != "")
			{
				for (BYTE TempLoop = 1; TempLoop < vGlobalCurrentProduct->vNumberOfInspections; TempLoop++)
				if (vGlobalCurrentProduct->vInspection[TempLoop])
				if (vGlobalCurrentProduct->vInspection[TempLoop]->vName == TempInspectionName)
				{
					vInspectionShowing = vGlobalCurrentProduct->vInspection[TempLoop];
					vInspectionNumberShowing = TempLoop;
				}
			}
			if (nResponse == 10)
			{
				//Main Menu button pressed
				Sleep(1); //is the equivelent of a yeild statement;
				PrepareAndExit(10);
			}
			SetInspectionShowing(vInspectionShowing);  
			WaitThenDrawImageOnScreen();
		}
	}
	else
	if (vImageDisplayOptions)
	{ //show or hide scale lines
		vLocalConfigurationData->vImageShowScale = !vLocalConfigurationData->vImageShowScale;
		WaitThenDrawImageOnScreen();
		UpdateButtons();
	}
	else
	{ //goto Edit while running
		if (vSubFunction6ButtonEnable == false)
		{
			ShowNoMouseClickDialogMessage();
		}
		else
		if (PasswordOK(cSetupPassword,true))
		{
			vAnalysisImageDisplay = false;
			vEditWhileRunning = true;
			this->SetWindowText(_T("Rejects Edit"));
			vDialogTitleString.LoadString(IDS_EditInspection);
			SetDlgItemText(IDC_DialogTitleStaticText1,vDialogTitleString);
			UpdateButtons();
			m_InspectionThreshold.ShowWindow(SW_HIDE);
			m_SubFunction3Display.ShowWindow(SW_SHOW);
			m_SubFunction4Display.ShowWindow(SW_SHOW);
			UpdateThresholdDisplay();
		}
	}
}
/*
void CNewImageDisplayDialog::OnSubFunction6Display1() 
{
	//vGlobalMouseMoveCounter = 0;
	OnSubFunction6Button();
}

void CNewImageDisplayDialog::OnSubFunction6Display2() 
{
	OnSubFunction6Button();
}

void CNewImageDisplayDialog::OnSubFunction6Display3() 
{
	OnSubFunction6Button();
}
*/
void CNewImageDisplayDialog::OnSubFunction7Button() 
{
	//vGlobalMouseMoveCounter = 0;
	if (vImproveLearnMenu)
	{
		//show delta current to Improve
		if (!vLocalSystemData->vITIPCDig->vImproveLearnCount)
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\n\nNo Improve Learn To Show";
			TempNoticeDialog.vType = cInformationMessage;
			TempNoticeDialog.DoModal();
		}
		else
		if (!vLocalSystemData->vITIPCDig->vProductStructureImage)
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\n\nNo Current Learn To Show";
			TempNoticeDialog.vType = cInformationMessage;
			TempNoticeDialog.DoModal();
		}
		else
		if (vShowDeltaCurrentPreviewStructure)
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\n\nAlready showing this image";
			TempNoticeDialog.vType = cInformationMessage;
			TempNoticeDialog.DoModal();
		}
		else
		{
			BYTE TempShowType = vShowType;
			ClearAllDisplayOptions(true);
			vShowType = TempShowType;

			vShowDeltaCurrentPreviewStructure = true;
			UpdateButtons();
			DrawImageOnScreen();
		}
	}
	else
	if (vShowThresholdMenu)
	{
		if (vInspectionShowing)
		{
			CYesNoDialog TempYesNoDialog;
			CString TempString("");
			TempString = "";
			TempString.LoadString(IDS_SavecurrentThresholdforproduct);

			if ((vInspectionShowing->vInspectionType == cUnderfillByWeightInspection) || (vInspectionShowing->vInspectionType == cUnderfillByWeightInspection))
			{
				TempString.LoadString(IDS_SavecurrentThresholdforproduct);
			}
			else
			if ((vInspectionShowing->vInspectionType == cDensityInspection) ||
				(vInspectionShowing->vInspectionType == cCheckWeighInspection) ||
				(vInspectionShowing->vInspectionType == cStandardDeviationInspection))
			{
				if (vShowingUpperThreshold)
					TempString.LoadString(IDS_SavecurrentUpperThresholdforproduct);
				else
					TempString.LoadString(IDS_SavecurrentLowerThresholdforproduct);
			}

			TempString = "\n\n" + TempString + ":\n";
			CString TempString1("");
			TempString1.LoadString(IDS_Inspection);
			TempString = TempString +	vLocalSystemData->vCurrentProductName + "\n" + TempString1 + ": " + vInspectionShowing->vName + "?";
			TempYesNoDialog.vNoticeText = TempString;
			TempYesNoDialog.vQuestionType = cConfirmSave;
			int TempResult = TempYesNoDialog.DoModal();
			if (TempResult == IDOK)
			if (vInspectionShowing)
			{
				vChangeMade = false;
				vInspectionShowing->vThreshold = (BYTE)vThreshold;
				ReportErrorMessage("Changed " + vInspectionShowing->vName + " Threshold: " + dtoa(vInspectionShowing->ConvertToWeight(vInspectionShowing->vThreshold),2) + " by operator", cUserChanges,0);

				if ((vInspectionShowing->vInspectionType == cDensityInspection) ||
					(vInspectionShowing->vInspectionType == cCheckWeighInspection) ||
					(vInspectionShowing->vInspectionType == cMissingLidInspection) ||
					(vInspectionShowing->vInspectionType == cUnderfillInspection) ||
					(vInspectionShowing->vInspectionType == cOverfillInspection) ||
					(vInspectionShowing->vInspectionType == cUnderfillByWeightInspection) ||
					(vInspectionShowing->vInspectionType == cOverfillByWeightInspection) ||
					(vInspectionShowing->vInspectionType == cStandardDeviationInspection))
				{
					if ((vShowingUpperThreshold) && (vInspectionShowing->vHasDensityUpperThreshold))
					{
						if ((vThreshold > vInspectionShowing->vDensityThresholdLower) || (vThreshold == 0) || (!vInspectionShowing->vHasDensityLowerThreshold))
							vInspectionShowing->vDensityThresholdUpper = (float)vInspectionShowing->ConvertToWeight(vThreshold);
						else
						{
							CNoticeDialog TempNoticeDialog;
							CString TempString("");
							TempString = "";
							TempString.LoadString(IDS_ValuenowaboveLowerThresholdValuenotsaved);
							TempNoticeDialog.vNoticeText = TempString; //"\n\n\nValue not above Lower Threshold.\nValue not saved.";
							TempNoticeDialog.vType = cErrorMessage;
							TempNoticeDialog.DoModal();
						}
					}
					else
					{
						if ((vThreshold < vInspectionShowing->vDensityThresholdUpper) || (!vInspectionShowing->vHasDensityUpperThreshold))
							vInspectionShowing->SetDensityLowerThreshold(vThreshold);
						else
						{
							CNoticeDialog TempNoticeDialog;
							CString TempString("");
							TempString = "";
							TempString.LoadString(IDS_ValuenowbelowUpperThresholdValuenotsaved);
							TempNoticeDialog.vNoticeText = TempString; //\n\n\nValue now below Upper Threshold.\nValue not saved.";
							TempNoticeDialog.vType = cErrorMessage;
							TempNoticeDialog.DoModal();
						}
					}
				}
				//don't change menus when done if super password
				if (!PasswordOK(cSuperInspxPassword,false))
				{
					vShowThresholdMenu = false;
					ClearSensitivity();
					vEditWhileRunning = true;
					this->SetWindowText(_T("Rejects Edit"));
					vDialogTitleString.LoadString(IDS_EditInspection);
					SetDlgItemText(IDC_DialogTitleStaticText1,vDialogTitleString);
				}
				SetInspectionShowing(vInspectionShowing);  //update settings on screen display
				UpdateButtons();
				m_InspectionThreshold.ShowWindow(SW_HIDE);
				m_SubFunction3Display.ShowWindow(SW_SHOW);
				m_SubFunction4Display.ShowWindow(SW_SHOW);
				UpdateThresholdDisplay();
				DrawImageOnScreen();
				if (PasswordOK(cSuperInspxPassword,false))
					ShowSensitivity();
			}
		}
	}
	else
	if (vEditROIBoundaries)
	{
		if (!vRecievedButtonPress)
		{
			if ((vMouseState == 0) || ((vMouseState > 8) && (vMouseState < 17)))	//if mouse is idle or in double click toggle on mode
			{
				vMouseState = 7;	//set mouse to on while pressed down
				int TimerResult = SetTimer(vMouseDownRepeatTimerHandle,200,NULL);
				if (!TimerResult)
					ReportErrorMessage("Error-Second Timer Failed",cEMailInspx,32000);
			}
		}
		
		if (vInspectionShowing)
		if (vInspectionShowing->vROIPixelRight > vInspectionShowing->vROIPixelLeft + 2)
		{
			vInspectionShowing->vROIPixelRight--; 
			SetROILines();
		}
	}
	else
	if (vRejectsImageDisplay)
	{  //Filter 7 Button was pressed
		//Next Set of Filters Button was pressed
		BYTE TempNumberOfInspectionsToView = vTotalNumberOfInspectionsToDisplay;
		if (PasswordOK(cTemporaryInspxPassword,false))
			TempNumberOfInspectionsToView++;  //include good containers

		if (TempNumberOfInspectionsToView > 7)
		{
			if ((vFirstInspectionToView < 12) && (vFirstInspectionToView + 6 < TempNumberOfInspectionsToView))
			{
				vFirstInspectionToView = vFirstInspectionToView + 6;
			}
			else
			{
				vFirstInspectionToView = 0;
			}
			ShowOrHideDisplays();
			for (BYTE TempLoop = 0; TempLoop < 7; TempLoop++)
				vOldCounter[TempLoop] = 0xFFFFFFFF;
			vOldGoodCountTotal = 0xFFFFFFFF;
			UpdateButtons();
			OneSecondUpdate();
		}
		else
			ToggleFilter(7 + vFirstInspectionToView);
	}
	else
	if (vEditWhileRunning)
	{ 
		if (PasswordOK(cTemporaryInspxPassword,false))
//			(!((vGlobalShiftKeyDown) && (vMainWindowPointer->GetImproveLearnInspectionNumber()))))
		{
			CUndoAddToLearnDialog TempUndoAddToLearnDialog;
			TempUndoAddToLearnDialog.vLocalSystemData = vLocalSystemData;
			TempUndoAddToLearnDialog.vLocalConfigurationData = vLocalConfigurationData;
			int nResponse = TempUndoAddToLearnDialog.DoModal();
			vMainWindowPointer->SetLearnState();
			if (nResponse == 10)
			{
				//Main Menu button pressed
				Sleep(1); //is the equivelent of a yeild statement;
				PrepareAndExit(10);
			}
		}
		else
		if (vLocalSystemData->vITIPCDig->HaveUndoAddToLearnImage()) //&& 
		//	(!((vGlobalShiftKeyDown) && (vMainWindowPointer->GetImproveLearnInspectionNumber()))))
		{//undo last add to learn
			if (vLocalSystemData->vITIPCDig->HaveUndoAddToLearnImage())
			{
				vLocalSystemData->vITIPCDig->UndoLastAddToLearn();
				UpdateButtons();
				vMainWindowPointer->SetLearnState();

				CNoticeDialog TempNoticeDialog;
				CString TempString = "\n\n\nLast Add To Learn Removed.";
				//TempString.LoadString(IDS_RejectAddedToLearn);//"\n\n\nReject Added To Learn."
				TempNoticeDialog.vNoticeText = TempString;
				TempNoticeDialog.vType = cInformationMessage;
				TempNoticeDialog.vAutoClose = 3000;
				TempNoticeDialog.DoModal();
			}
		}
	}
	else
	if (vImageDisplayOptions)
	{//toggle show found boudaries lines, don't show lines  S/W ROI
		vLocalConfigurationData->vImageShowLines = !vLocalConfigurationData->vImageShowLines;
		UpdateButtons();
		WaitThenDrawImageOnScreen();
		DisplayLocationEdge();
	}
	else 
	{
		//vAnalysisImageDisplay  image analysis menu, add to learn
		if (vDisplayingImageData)
		if (vDisplayingImageData->vFinalImageIntensity == 0)
		if (vGlobalCurrentProduct->vDriftDensityInspection)
		{
			vDisplayingImageData->vFinalImageIntensity = vLocalSystemData->vITIPCDig->CalculateInspectionDensityAfterReference(vDisplayingImageData->vGreyImage, vGlobalCurrentProduct->vDriftDensityInspection,
				vDisplayingImageData->vLeftReferenceAdjust, vDisplayingImageData->vTopReferenceAdjust, vLocalConfigurationData->vReferenceROIsToEdges, vDisplayingImageData->vLeft, vDisplayingImageData->vRight); 
		}

		bool TempOKToAdd = true;
		if ((vGlobalShiftKeyDown) && (PasswordOK(cTemporaryInspxPassword,false)) && (vLocalSystemData->vITIPCDig->HaveUndoAddToLearnImage()))  //undo last add to learn if shift key down
		{
			vLocalSystemData->vITIPCDig->UndoLastAddToLearn();
			vMainWindowPointer->SetLearnState();

			CNoticeDialog TempNoticeDialog;
			CString TempString = "\n\n\nLast Add To Learn Removed.";
			TempNoticeDialog.vNoticeText = TempString;
			TempNoticeDialog.vType = cInformationMessage;
			TempNoticeDialog.vAutoClose = 3000;
			TempNoticeDialog.DoModal();

			CString TempText("");
			TempText.LoadString(IDS_AddRejecttoLearn);
			SetDlgItemText(IDC_SubFunction7Button,TempText);
			vGlobalShiftKeyDown = false;
			PostThreadMessage(vGlobalCallingThreadID,cUpdateTitleBarMessage, 0, 0);
			UpdateButtons();
			TempOKToAdd = false;
		}
		if (TempOKToAdd)
		if ((!vHaveAnImageToShow) || (vShowAlternateImage) || (!vDisplayingImageData))
		{
			ShowNeedImageDialogMessage();
			TempOKToAdd = false;
		}
		if (TempOKToAdd)
		if (vShowingLiveImages)
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\n\nShowing Images, You Cannot Add To Learn";
			TempNoticeDialog.vType = cInformationMessage;
			TempNoticeDialog.DoModal();
			TempOKToAdd = false;
		}
		if (TempOKToAdd)
		if (!vGlobalCurrentProduct->ProductHasImageFiles(vGlobalCurrentDirectory))
		{
			CNoticeDialog TempNoticeDialog;
			CString TempText("");
			TempText.LoadString(IDS_ProducthasnotbeenlearnedyoucannotAddToLearn);
			TempText = "\n\n" + TempText;
			TempNoticeDialog.vNoticeText = TempText;
			TempNoticeDialog.vType = cInformationMessage;
			TempNoticeDialog.DoModal();
			TempOKToAdd = false;
		}
		if (TempOKToAdd)
		if ((vLocalSystemData->vITIPCDig->vRejectBufferShowing == -1) && (vDisplayingImageData) && (vDisplayingImageData->vContainerEjectReason > cMaximumNumberOfCountsDisplayed))
		{
			if (PasswordOK(cSuperInspxPassword,false))
			{
				CYesNoDialog TempYesNoDialog;
				CString TempString("\n\nThis " + vLocalSystemData->vContainerString + " not rejected.\nAre you sure you want to Add it To Learn?");
				TempYesNoDialog.vNoticeText = TempString;
				TempYesNoDialog.vYesButtonText = "Add To Learn";
				TempYesNoDialog.vNoButtonText = "Cancel Add To Learn";
				TempYesNoDialog.vQuestionType = cConfirmSave;
				int TempResult = TempYesNoDialog.DoModal();
				if (TempResult != IDOK)
					TempOKToAdd = false;
			}
			else
			{
				CNoticeDialog TempNoticeDialog;
				CString TempText("");
				TempText.LoadString(IDS_ContainerwasnotrejectedyoucannotAddToLearn);
				TempText = "\n\n" + TempText;
				TempNoticeDialog.vNoticeText = TempText;
				TempNoticeDialog.vType = cInformationMessage;
				TempNoticeDialog.DoModal();
				TempOKToAdd = false;
			}
		}
		if (TempOKToAdd)
		if (vLocalSystemData->vITIPCDig->vUsingOriginalLearnTemporarily)
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\nYou cannot add to learn because\nyou are Temporarily Using the Original Learn";
			TempNoticeDialog.vType = cInformationMessage;
			TempNoticeDialog.DoModal();
			TempOKToAdd = false;
		}
		if (TempOKToAdd)
		if (((vDisplayingImageData) && 
			((vDisplayingImageData->vContainerEjectInspection == 255) ||
			((vDisplayingImageData->vContainerEjectInspection < vGlobalCurrentProduct->vNumberOfInspections) &&
			(!vGlobalCurrentProduct->vInspection[vDisplayingImageData->vContainerEjectInspection]->vAddStructure)))))
		{
			if (PasswordOK(cSuperInspxPassword,false))
			{
				CYesNoDialog TempYesNoDialog;
				CString TempString("\n\nThis " + vLocalSystemData->vContainerString + " not rejected by a Learned Inspection.\nAre you sure you want to Add it To Learn?");
				TempYesNoDialog.vNoticeText = TempString;
				TempYesNoDialog.vYesButtonText = "Add To Learn";
				TempYesNoDialog.vNoButtonText = "Cancel Add To Learn";
				TempYesNoDialog.vQuestionType = cConfirmSave;
				int TempResult = TempYesNoDialog.DoModal();
				if (TempResult != IDOK)
					TempOKToAdd = false;
			}
			else
			{
				CNoticeDialog TempNoticeDialog;
				CString TempText("");
				TempText.LoadString(IDS_ContainerwasnotrejectedbyaLearnedInspectionyoucannotAddToLearn);
				TempText = "\n\n" + TempText;
				TempNoticeDialog.vNoticeText = TempText;
				TempNoticeDialog.vType = cInformationMessage;
				TempNoticeDialog.DoModal();
				TempOKToAdd = false;
			}
		}
		if (TempOKToAdd)
		if (vDisplayingImageData->vWidth > 
			vGlobalCurrentProduct->vReferenceWidth + vLocalConfigurationData->vShapeHandlingWidthTolerancePixels) //don't do if too wide
		{
			if (PasswordOK(cSuperInspxPassword,false))
			{
				CYesNoDialog TempYesNoDialog;
				CString TempString("\n\nThis image is wider than Learned.\nAre you sure you want to Add it To Learn?");
				TempYesNoDialog.vNoticeText = TempString;
				TempYesNoDialog.vYesButtonText = "Add To Learn";
				TempYesNoDialog.vNoButtonText = "Cancel Add To Learn";
				TempYesNoDialog.vQuestionType = cConfirmSave;
				int TempResult = TempYesNoDialog.DoModal();
				if (TempResult != IDOK)
					TempOKToAdd = false;
			}
			else
			{
				CNoticeDialog TempNoticeDialog;
				TempNoticeDialog.vNoticeText = "\n\nContainer much wider than learned,\n can not Add To Learn";
				TempNoticeDialog.vType = cInformationMessage;
				TempNoticeDialog.DoModal();
				TempOKToAdd = false;
			}
		}
		if (TempOKToAdd)
		if (vDisplayingImageData->vWidth < 
			vGlobalCurrentProduct->vReferenceWidth - vLocalConfigurationData->vShapeHandlingWidthTolerancePixels) //don't do if too narrow
		{
			if (PasswordOK(cSuperInspxPassword,false))
			{
				CYesNoDialog TempYesNoDialog;
				CString TempString("\n\nThis image is narrower than Learned.\nAre you sure you want to Add it To Learn?");
				TempYesNoDialog.vNoticeText = TempString;
				TempYesNoDialog.vYesButtonText = "Add To Learn";
				TempYesNoDialog.vNoButtonText = "Cancel Add To Learn";
				TempYesNoDialog.vQuestionType = cConfirmSave;
				int TempResult = TempYesNoDialog.DoModal();
				if (TempResult != IDOK)
					TempOKToAdd = false;
			}
			else
			{
				CNoticeDialog TempNoticeDialog;
				TempNoticeDialog.vNoticeText = "\n\nContainer much narrower than learned,\n can not Add To Learn";
				TempNoticeDialog.vType = cInformationMessage;
				TempNoticeDialog.DoModal();
				TempOKToAdd = false;
			}
		}
		if (TempOKToAdd)
		if ((vLocalConfigurationData->vMaxImageIntensityAdjust) && (vDisplayingImageData->vFinalImageIntensity < vGlobalCurrentProduct->vSetupHistoryAverageDensity - vLocalConfigurationData->vMaxImageIntensityAdjust)) 
		{//don't do if too dark
			if (PasswordOK(cSuperInspxPassword,false))
			{
				CYesNoDialog TempYesNoDialog;
				CString TempString("\n\nThis image is darker than Learned.\nAre you sure you want to Add it To Learn?");
				TempYesNoDialog.vNoticeText = TempString;
				TempYesNoDialog.vYesButtonText = "Add To Learn";
				TempYesNoDialog.vNoButtonText = "Cancel Add To Learn";
				TempYesNoDialog.vQuestionType = cConfirmSave;
				int TempResult = TempYesNoDialog.DoModal();
				if (TempResult != IDOK)
					TempOKToAdd = false;
			}
			else
			{
				CNoticeDialog TempNoticeDialog;
				TempNoticeDialog.vNoticeText = "\n\nContainer much darker than learned,\n can not Add To Learn";
				TempNoticeDialog.vType = cInformationMessage;
				TempNoticeDialog.DoModal();
				TempOKToAdd = false;
			}
		}
		if (TempOKToAdd)
		if ((vLocalConfigurationData->vMaxImageIntensityAdjust) && (vDisplayingImageData->vFinalImageIntensity > vGlobalCurrentProduct->vSetupHistoryAverageDensity + vLocalConfigurationData->vMaxImageIntensityAdjust)) 
		{//don't do if too bright
			if (PasswordOK(cSuperInspxPassword,false))
			{
				CYesNoDialog TempYesNoDialog;
				CString TempString("\n\nThis image is brighter than Learned.\nAre you sure you want to Add it To Learn?");
				TempYesNoDialog.vNoticeText = TempString;
				TempYesNoDialog.vYesButtonText = "Add To Learn";
				TempYesNoDialog.vNoButtonText = "Cancel Add To Learn";
				TempYesNoDialog.vQuestionType = cConfirmSave;
				int TempResult = TempYesNoDialog.DoModal();
				if (TempResult != IDOK)
					TempOKToAdd = false;
			}
			else
			{
				CNoticeDialog TempNoticeDialog;
				TempNoticeDialog.vNoticeText = "\n\nContainer much brighter than learned,\n can not Add To Learn";
				TempNoticeDialog.vType = cInformationMessage;
				TempNoticeDialog.DoModal();
				TempOKToAdd = false;
			}
		}
		if (TempOKToAdd)
		{
			/*
			if (vLocalSystemData->vITIPCDig->vRejectBufferShowing >= 0)
			{
				vLocalSystemData->vITIPCDig->MakeUndoBackupOfLearnStructureFiles(vGlobalCurrentProduct);

				if (vLocalSystemData->vITIPCDig->AddImageToStructure(vLocalLastNRejectImages[
					vLocalSystemData->vITIPCDig->vRejectBufferShowing],vInspectionShowing))
				{
					//vLocalSystemData->vITIPCDig->AddImageToGlassStructure(
					//	vLocalLastNRejectImages[vLocalSystemData->vITIPCDig->vRejectBufferShowing]->vGreyImage,
					//	vLocalLastNRejectImages[vLocalSystemData->vITIPCDig->vRejectBufferShowing]->vLeftReferenceAdjust,
					//	true);
					
					vLocalSystemData->vITIPCDig->AddImageToVoidStructure(
						vLocalLastNRejectImages[vLocalSystemData->vITIPCDig->vRejectBufferShowing],
						vInspectionShowing);
					vMainWindowPointer->SetLearnState();

					ReportErrorMessage("Added Reject to Learn", cUserChanges,0);

					CNoticeDialog TempNoticeDialog;
					CString TempString("");
					TempString = "";
					TempString.LoadString(IDS_RejectAddedToLearn);//"\n\n\nReject Added To Learn."
					TempNoticeDialog.vNoticeText = TempString;
					TempNoticeDialog.vType = cInformationMessage;
					TempNoticeDialog.vAutoClose = 750;
					TempNoticeDialog.DoModal();
				}
				else
					vLocalSystemData->vITIPCDig->DeleteUndoStructureLearnFiles(vGlobalCurrentProduct);
			}
			else
			*/
			if (vDisplayingImageData)
			{
				//record the Learn Quality before Add To Learn
				bool TempLearnQualityIncreaseTooMuch = false;

				double TempLearnQualityBeforeAddToLearn = vLocalSystemData->vITIPCDig->CalculateStructureDensityInDriftDensityROI(vGlobalCurrentProduct);

				double TempWholeLearnQualityBeforeAddToLearn = vLocalSystemData->vITIPCDig->CalculateStructureDensityInProduct(vGlobalCurrentProduct);

				//do the Add To Learn Now
				vLocalSystemData->vITIPCDig->MakeUndoBackupOfLearnStructureFiles(vGlobalCurrentProduct);

				if (vLocalSystemData->vITIPCDig->AddImageToStructure(vDisplayingImageData, vInspectionShowing))
				{
					//vLocalSystemData->vITIPCDig->AddImageToGlassStructure(vDisplayingImageData->vGreyImage,
					//	vDisplayingImageData->vLeftReferenceAdjust, true);						

					vLocalSystemData->vITIPCDig->AddImageToVoidStructure(vDisplayingImageData, vInspectionShowing);

				}
				else
					vLocalSystemData->vITIPCDig->DeleteUndoStructureLearnFiles(vGlobalCurrentProduct);

				//Record the Learn Quality After the Add To Learn
				double TempLearnQualityAfterAddToLearn = vLocalSystemData->vITIPCDig->CalculateStructureDensityInDriftDensityROI(vGlobalCurrentProduct);

				double TempWholeLearnQualityAfterAddToLearn = vLocalSystemData->vITIPCDig->CalculateStructureDensityInProduct(vGlobalCurrentProduct);

				double TempIncreaseInLearnQuality = TempLearnQualityAfterAddToLearn - TempLearnQualityBeforeAddToLearn;
				double TempIncreaseInWholeLearnQuality = TempWholeLearnQualityAfterAddToLearn - TempWholeLearnQualityBeforeAddToLearn;

				//if Learn Quality went up much, as operator if they are sure they want to Add To Learn
				if ((TempIncreaseInLearnQuality > 1) || (TempIncreaseInWholeLearnQuality > 1))
				{
					CYesNoDialog TempYesNoDialog;
					CString TempString("\nLearn Quality will increase by: ");

					TempString = TempString + dtoa(TempIncreaseInLearnQuality,2) + ", " +
						dtoa(TempIncreaseInWholeLearnQuality,2) + 
						" (Center, Whole)\nOriginal Learn Quality: "  +
						dtoa(TempLearnQualityBeforeAddToLearn,2) + ", " + dtoa(TempWholeLearnQualityBeforeAddToLearn,2) + 
						" (Center, Whole)\nKeep Add To Learn?";

					TempYesNoDialog.vNoticeText = TempString;
					TempYesNoDialog.vYesButtonText = "Keep Add To Learn";
					TempYesNoDialog.vNoButtonText = "Cancel Add To Learn";
					TempYesNoDialog.vQuestionType = cConfirmSave;
					int TempResult = TempYesNoDialog.DoModal();
					if (TempResult != IDOK)
						TempLearnQualityIncreaseTooMuch = true;
					else
					{
						ReportErrorMessage("Add To Learn Quality Increase: " + dtoa(TempIncreaseInLearnQuality,2) + ", " +
							dtoa(TempIncreaseInWholeLearnQuality,2) + " (Center, Whole)", cUserChanges,32000);
					}
				}

				if (TempLearnQualityIncreaseTooMuch)
				{  //un do add to learn as operator decided learn quality went up too much
					vLocalSystemData->vITIPCDig->UndoLastAddToLearn();
					vMainWindowPointer->SetLearnState();

					CNoticeDialog TempNoticeDialog;
					CString TempString = "\n\n\nAdd To Learn Aborted.";
					//TempString.LoadString(IDS_RejectAddedToLearn);//"\n\n\nReject Added To Learn."
					TempNoticeDialog.vNoticeText = TempString;
					TempNoticeDialog.vType = cInformationMessage;
					TempNoticeDialog.vAutoClose = 3000;
					TempNoticeDialog.DoModal();

					CString TempText("");
					TempText.LoadString(IDS_AddRejecttoLearn);
					SetDlgItemText(IDC_SubFunction7Button,TempText);
				}
				else
				{  //keep the add to learn, so notify operator add to learn succeeded
					CNoticeDialog TempNoticeDialog;
					CString TempString("");
					TempString = "";
					TempString.LoadString(IDS_RejectAddedToLearn);//"\n\n\nReject Added To Learn."
					TempNoticeDialog.vNoticeText = TempString;
					TempNoticeDialog.vType = cInformationMessage;
					TempNoticeDialog.vAutoClose = 750;
					TempNoticeDialog.DoModal();
				}
			}
		}
	}
}

void CNewImageDisplayDialog::OnSubFunction8Button() 
{
	//vGlobalMouseMoveCounter = 0;
	if (vEditROIBoundaries)
	{
		if (!vRecievedButtonPress)
		{
			if ((vMouseState == 0) || ((vMouseState > 8) && (vMouseState < 17)))	//if mouse is idle or in double click toggle on mode
			{
				vMouseState = 8;	//set mouse to on while pressed down
				int TimerResult = SetTimer(vMouseDownRepeatTimerHandle,200,NULL);
				if (!TimerResult)
					ReportErrorMessage("Error-Second Timer Failed",cEMailInspx,32000);
			}
		}
		
		if (vInspectionShowing)
		if (vInspectionShowing->vROIPixelRight < vLocalSystemData->vITIPCDig->vOriginalBufferSizeY)
		{
			vInspectionShowing->vROIPixelRight++; 
			SetROILines();
		}
	}
	else
	{  //toggle show defects in red
		//if ((vGlobalShiftKeyDown) && (PasswordOK(cTemporaryInspxPassword,false)))
		//{
		//	//enum {cDefectsInNoColor, cDefectsInPseudoColor, cDefectsInRed, cDefectInRedBox, cDefectsInRedBoxes};
		//	if (vShowThreshold)  //editing the threshold  can see red pixels or pseudo color
		//	{
		//		if (vLocalConfigurationData->vDefectDisplayMode < cDefectsInRed)
		//			vLocalConfigurationData->vDefectDisplayMode++;
		//		else
		//			vLocalConfigurationData->vDefectDisplayMode = cDefectsInPseudoColor;
		//	}
		//	else
		//	if (!vShowingProcess)
		//	{  //showing original image can see all 4 display modes
		//		if (vLocalConfigurationData->vDefectDisplayMode < cDefectsInRedBoxes)
		//			vLocalConfigurationData->vDefectDisplayMode++;
		//		else
		//			vLocalConfigurationData->vDefectDisplayMode = cDefectsInPseudoColor;
		//	}
		//	vHideRedIndicator = false;
		//	vGlobalShiftKeyDown = 4;
		//	WaitThenDrawImageOnScreen();
		//	UpdateButtons();
		//	UpdateDisplayIndicators();
		//}
		//else
		if (vSubFunction8ButtonEnable == false)	// not needed
		{
			ShowNoMouseClickDialogMessage();
		}
		else
		{
			vHideRedIndicator = !vHideRedIndicator;
			WaitThenDrawImageOnScreen();
			UpdateButtons();
			UpdateDisplayIndicators();
		}
	}
}

void CNewImageDisplayDialog::CalculateMinimumAndMaximumIntensity()
{
	if (vInspectionShowing)
	if (ThereIsEnoughMemory((vLocalSystemData->vITIPCDig->vOriginalBufferSize + sizeof(Image)), "Temp Image"))
	{
		Image *TempImage = NULL;
		TempImage = im_create(IM_BYTE, vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, 
			vLocalSystemData->vITIPCDig->vOriginalBufferSizeY);

		if ((TempImage) && (TempImage->pix))
		{
			if ((vDisplayingImageData) && (vDisplayingImageData->vGreyImage))
			{
				CopyMemory(TempImage->pix,
					vDisplayingImageData->vGreyImage,
					vLocalSystemData->vITIPCDig->vOriginalBufferSize);
				int TempX = TempImage->x;
				int TempY = TempImage->y;
	#ifdef UseIFCDLLs
				MinimumROIIntensity = MinimumValueInImage(TempImage);
				MaximumROIIntensity = MaximumValueInImage(TempImage);
	#endif
				if (vInspectionShowing->vEnhanceMethod)
				{
					vLocalSystemData->vITIPCDig->PerformEnhanceOnImage(TempImage, vLocalSystemData->vITIPCDig->vNIDDScratchImage, vInspectionShowing->vEnhanceMethod);
	#ifdef UseIFCDLLs
					MinimumEnhanceROIIntensity = MinimumValueInImage(TempImage);
					MaximumEnhanceROIIntensity = MaximumValueInImage(TempImage);
	#endif
				}
				else
				{
					MinimumEnhanceROIIntensity = 0;
					MaximumEnhanceROIIntensity = 0;
				}
			}
			else
				ReportErrorMessage("Error, MVT Could not create Image for Image Enhancement", 
					cEMailInspx,32000);
		}
		if (TempImage)
			im_delete(TempImage);
	}
}

BOOL CNewImageDisplayDialog::PreTranslateMessage(MSG* pMsg) 
{
	CWnd * TempWindow = NULL;
	
	if (pMsg->message == cUpdateTitleBarMessage)  //the hidden key was pressed or cleared so update buttons and title bar
	{
		CWnd *TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText1);
		if (TempWindow)
			TempWindow->Invalidate(false);
		if (!vEditROIBoundaries)
		{//update color no color button 
			UpdateButtons();
		}
		return true;  //true indicates it has been handled, so do not process
	}
	else
	if (pMsg->message == cNewImageReadyToDisplay)
	{
		//newimage newframe
		if (!vHaveAnImageToShow)
		{  //have no im
			vHaveAnImageToShow = true;
			m_CurrentInspection.ShowWindow(SW_SHOW);
			m_Background.ShowWindow(SW_HIDE);
			m_ErrorMessage.ShowWindow(SW_HIDE);
			if (vRejectsImageDisplay)
			{
				ShowNextReject(true, true);
				if ((!vHaveAnImageToShow) || ((vNumberOfRejectsInLocalBuffer == 0) && (vAllRejectsHidden)))
					ShowNeedImageDialogMessage();
			}
			if (vLocalSystemData->vITIPCDig->vRejectBufferShowing == -1)
				vShowNextFrame = true;
		}
		//else
		//if (vAllRejectsHidden)
		//	if (vRejectsImageDisplay)
		//		ShowNextReject(false,false);

		if (vLocalSystemData->vLogFile.vLogSerialData)
			vLocalSystemData->vLogFile.WriteToLogFile("NIDD-NF-Start",cDebugMessage);
		if (((vGlobalImageAquisitionThread) && (vShowingLiveImages)) || 
			(vShowNextFrame))
		if ((!vLocalSystemData->vITIPCDig->vShowOnlyRealImages) || 
			(vLocalSystemData->vITIPCDig->vNextImageToDisplay->vHaveRealImage) || 
			(vShowNextFrame))
		{
			if (vLocalSystemData->vITIPCDig->vOriginalImage)
			{
				vProcessNextImage = false;
				if (vLocalSystemData->vLogFile.vLogSerialData)
					vLocalSystemData->vLogFile.WriteToLogFile("30NIDD-vNextImageToDisplayLock.Lock",cDebugMessage);
				if (vLocalSystemData->vITIPCDig->vNextImageToDisplayLock.Lock())
				{
					vLocalSystemData->vITIPCDig->CopyImageData(
						vLocalSystemData->vITIPCDig->vNextImageToDisplay,
						vLocalSystemData->vITIPCDig->vOriginalImage,
						vLocalSystemData->vITIPCDig->vOriginalBufferSize);
					vLocalSystemData->vITIPCDig->vNextImageToDisplayLock.Unlock();
				if (vLocalSystemData->vLogFile.vLogSerialData)
					vLocalSystemData->vLogFile.WriteToLogFile("31NIDD-vNextImageToDisplayLock.Unlock",cDebugMessage);
				}

				vLocalSystemData->vTweakReferenceAdjust =
					vLocalSystemData->vITIPCDig->vOriginalImage->vFineReferenceAdjust;
				if (vInspectionShowing)
				if (vInspectionShowing->vInspectionType == cKernelInspection)
					vLocalSystemData->vTweakReferenceAdjust = 0;

				ShowNewImageOnDisplay();
				if (vSaveImagesToFiles)
				if (vSaveImagesToFilesName != "")
				{
					CString TempString(vSaveImagesToFilesName);
					if (_wtoi(dtoa(vSaveImagesToFiles, 0)) == 1)
					{
						vSaveImagesToFilesName = ""; //delete name after writing last file
					}
					
					TempString = TempString + dtoa(vSaveImagesToFiles, 0) + ".BMP";
					TempString = vLocalConfigurationData->vScanTracImagesDirectory + TempString;
					int TempLength = TempString.GetLength();
					if (TempLength > MAX_PATH)
					{
						CNoticeDialog TempNoticeDialog;
						CString TempText("");
						TempText.LoadString(IDS_FileNamemorethe83charactersfilesaveaborted);//"\n\n\nFile Name more the MAX_PATH characters, file save aborted"
						CString TempString = TempText;
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
						if ((vDisplayingImageData) && (vDisplayingImageData->vGreyImage))
						{
							if (!TempImageFile->WriteFile(vDisplayingImageData->vGreyImage, vLocalSystemData->vITIPCDig->vOriginalBufferSizeX,
								vLocalSystemData->vITIPCDig->vOriginalBufferSizeY, (WORD)vLocalSystemData->vITIPCDig->vBitsPerPixel))
							{
								ReportErrorMessage("Failed to Write Image Series File", cEMailInspx, 32000);
							}
							vSaveImagesToFiles--;
						}
						if (TempImageFile) 
							delete TempImageFile;
					}
				}
			}
			if (vLocalSystemData->vLogFile.vLogSerialData)
				vLocalSystemData->vLogFile.WriteToLogFile("NIDD-NF-End",cDebugMessage);
		}
		return true;  //true indicates it has been handled, so do not process
	}
	//else
	//if (pMsg->message == WM_LBUTTONDBLCLK)
	//{
	//	ReportErrorMessage("Left Button Double Click",cEMailInspx, 32000);
	//	
	//	if ((vMouseState > 0) && (vMouseState <= 8))
	//	{
	//		vMouseState = vMouseState + 8;	//if registered single click on button, and got to this double click method, use button # from single click
	//		//begin scrolling
	//	}	
	//	else
	//	if (vMouseState > 8)	//if ((vMouseState >= 9) && (vMouseState <= 16))
	//	{
	//		//stop scrolling
	//		vMouseState = 0;
	//	}
	//	else	//if (vMouseState == 0)
	//	{
	//		ReportErrorMessage("Left Button Double Click, mouse = 0",cEMailInspx, 32000);
	//	}
	//}
	else
	if (pMsg->message == WM_LBUTTONDOWN)
	{
		if (vMouseState > 0)
		{
			//ReportErrorMessage("Screen Clicked, repeat ended",cWriteToLog, 0);
			KillTimer(vMouseDownRepeatTimerHandle);
			vMouseState = 0;
		}
	}
	else
	if (pMsg->message == WM_LBUTTONUP)
	{
		//ReportErrorMessage("Left Button Up",cAction, 32000);
		if (vMouseState < 9)
		{
			if (vMouseState > 0)
			{
				KillTimer(vMouseDownRepeatTimerHandle);
				vMouseState = 0;
			}
		}
	}
	else
	if (pMsg->message == WM_KEYDOWN)
	{
		vRecievedButtonPress = true;
		if (vGlobalPasswordTimeOut)
			vGlobalPasswordTimeOut = 600;
		//CWnd *TempActiveWindow = this->GetForegroundWindow();
		int TempValue = pMsg->wParam;
		if (vGlobalShiftKeyDown)
		{
			if (pMsg->wParam == 114)
				OnSubFunction7Button();
			else
			if (pMsg->wParam == 119)
				OnSubFunction8Button();
			else
				PostThreadMessage(vGlobalCallingThreadID,cHiddenKeyPressed, 0, pMsg->wParam);
		}
		else
		switch (pMsg->wParam)
		{
			//case 37: case 39: //arrow Left and Right
			//	vPlotLine = 0;
			//break;
			case 38: //arrow up
			case 40: //arrow down
			case c2: //use the 2 key as an up arrow
			case c8: //use the 8 key as a down arrow
				if (PasswordOK(cSuperInspxPassword,false))
					PlotLine(pMsg->wParam);
			break;
			case 113: // F2:
				OnSubFunction5Button();
			break;
			case 114: // F3:
				OnSubFunction7Button();
			break;
			case 115: // F4:
				OnSubFunction6Button();
			break;
			case 116: // F1:
				OnSubFunction1Button();
			break;
			case 117: // F6 Key - Function 5:
				OnSubFunction4Button();
			break;
			case 118: // F7 Key - Sub Function 1:
				OnFunction1Button();
			break;
			case 119: // F8 Key - Sub Function 2:
				OnSubFunction8Button();
			break;
			case 120: // F9 Key - Sub Function 3:
				OnSubFunction2Button();
			break;
			case 122: // F11 Key - Sub Function 4:
				OnSubFunction3Button();
			break;
			case 111: // / Key - Sub Function 5:
				OnFunction2Button();
			break;
			case 106: // * Key - Sub Function 6:
				OnFunction4Button();
			break;
			case 109: // - Key - Sub Function 7  Decimal Point:
				OnFunction3Button();
			break;
			case 107: // + Key -  Function 5:
				OnFunction5Button();
			break;
			case 16: // Shift Key
			case 0x60:  //0 key on KeyPad
				PostThreadMessage(vGlobalCallingThreadID,cHiddenKeyPressed, 1, 0);
			break;
			case 36:  //Home key, Screen Capture
				vGlobalShiftKeyDown = true;
				PostThreadMessage(vGlobalCallingThreadID,cHiddenKeyPressed, 0, 0x61);
			break;
			case cPageDown: // Help
				//vGlobalMouseMoveCounter = 0;
				PostThreadMessage(vGlobalCallingThreadID,cShowHelp, 16, 0);
				{
					TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText1);
					if (TempWindow)
						TempWindow->Invalidate(false);
				}
			break;
			case 83: // key s
			//case 115:
			if (PasswordOK(cTemporaryInspxPassword,false))
			if (vAnalysisImageDisplay)
			{
				SaveImageToFile(false, false);
			}
			break;

			/*
			case 78: //n key 
				if (PasswordOK(cTemporaryInspxPassword,false))
				if (vShowTopHatImage)
					vShowTopHatImage--;
				WaitThenDrawImageOnScreen();
			break;
			case 85: //u key
				if (PasswordOK(cTemporaryInspxPassword,false))
					vShowTopHatImage++;
				WaitThenDrawImageOnScreen();
			break;
			case 72: //h key
				if (PasswordOK(cTemporaryInspxPassword,false))
					vShowTopHatImage = 0;
				WaitThenDrawImageOnScreen();
			break;

			case 77: //m key
				if (PasswordOK(cTemporaryInspxPassword,false))
				if (vShowBluredImage)
					vShowBluredImage--;
				WaitThenDrawImageOnScreen();
			break;
			case 73://i key
				if (PasswordOK(cTemporaryInspxPassword,false))
					vShowBluredImage++;
				WaitThenDrawImageOnScreen();
			break;
			case 74: //j key
				if (PasswordOK(cTemporaryInspxPassword,false))
					vShowBluredImage = 0;
				WaitThenDrawImageOnScreen();
			break;
			case 82: // key r
				if (PasswordOK(cTemporaryInspxPassword,false))
				{
					vLocalSystemData->vTweakReferenceAdjust = 0;
					vLocalSystemData->vTweakTopReferenceAdjust = 0;
					WaitThenDrawImageOnScreen();
				}
			break;
			case 84: // key t
				if (PasswordOK(cTemporaryInspxPassword,false))
				{
					vLocalSystemData->vTweakReferenceAdjust = -1;
					WaitThenDrawImageOnScreen();
				}
			break;
			case 69: // key e
				if (PasswordOK(cTemporaryInspxPassword,false))
				{
					vLocalSystemData->vTweakReferenceAdjust = 1;
					WaitThenDrawImageOnScreen();
				}
			break;
			case 89: // key y
				if (PasswordOK(cTemporaryInspxPassword,false))
				{
					vLocalSystemData->vTweakReferenceAdjust = vLocalSystemData->vTweakReferenceAdjust - 1;
					WaitThenDrawImageOnScreen();
				}
			break;
			case 87: // key w
				if (PasswordOK(cTemporaryInspxPassword,false))
				{
					vLocalSystemData->vTweakReferenceAdjust = vLocalSystemData->vTweakReferenceAdjust + 1;
					WaitThenDrawImageOnScreen();
				}
			break;
			case 68: // key d
				if (PasswordOK(cTemporaryInspxPassword,false))
				{
					vLocalSystemData->vTweakTopReferenceAdjust = -1;
					WaitThenDrawImageOnScreen();
				}
			break;
			case 83: // key s
				if (PasswordOK(cTemporaryInspxPassword,false))
				{
					vLocalSystemData->vTweakTopReferenceAdjust = vLocalSystemData->vTweakTopReferenceAdjust - 1;
					WaitThenDrawImageOnScreen();
				}
			break;
			case 70: // key f
				if (PasswordOK(cTemporaryInspxPassword,false))
				{
					vLocalSystemData->vTweakTopReferenceAdjust = + 1;
					WaitThenDrawImageOnScreen();
				}
			break;
			case 71: // key g
				if (PasswordOK(cTemporaryInspxPassword,false))
				{
					vLocalSystemData->vTweakTopReferenceAdjust = 
						vLocalSystemData->vTweakTopReferenceAdjust + 1;
					WaitThenDrawImageOnScreen();
				}
			break;
				*/
			case cPageUp: // Clear, but do inspx hidden dialog
			{
				if (vGlobalPasswordLevel)
				{
					if (vShowingProcess)
						OnSubFunction2Button();

					bool TempNeedToDrawImage = false;
					if ((vShowingProcess) || (vShowAverage) || (vShowStructure) || 
						(vShowHistogram))// || (vShowGlassStructure) || (vShowGlassHistogram))
						TempNeedToDrawImage = true;
					PasswordOK(cNoPassword,false);

					vShowAverage = false;
					vShowStructure = false;
					vShowHistogram = false;

					//vShowGlassStructure = false;
					//vShowGlassHistogram = false;

					ClearAllDisplayOptions(true);

					//redisplay reject group if showing goods and changed inspx password state
					BYTE TempGoodIndex = vTotalNumberOfInspectionsToDisplay + 1;
					if (vGlobalCurrentProduct->vTypesOfRejectsToView & (0x0001 << (TempGoodIndex - 1)))
					{
						vLocalSystemData->vITIPCDig->vRejectBufferShowing = -1;
						ShowNextReject(true, true);
						if ((!vHaveAnImageToShow) || ((vNumberOfRejectsInLocalBuffer == 0) && (vAllRejectsHidden)))
							ShowNeedImageDialogMessage();
					}
					DrawImageOnScreen();

					UpdateDisplayIndicators();
				}
				else
				{
					PasswordOK(cAnyPasswordToEnter, true);

					if (PasswordOK(cTemporaryInspxPassword, false))
					{
						//redisplay reject group if showing goods and changed inspx password state
						BYTE TempGoodIndex = vTotalNumberOfInspectionsToDisplay + 1;
						if (vGlobalCurrentProduct->vTypesOfRejectsToView & (0x0001 << (TempGoodIndex - 1)))
						{
							vLocalSystemData->vITIPCDig->vRejectBufferShowing = -1;
							ShowNextReject(true, true);
						}

						DrawImageOnScreen();
					//if (vEditROIBoundaries)
					//{
					//	m_SubFunction1Arrow.Invalidate(false);
					//	m_SubFunction2Arrow.Invalidate(false);
					//	m_SubFunction3Arrow.Invalidate(false);
					//	m_SubFunction4Arrow.Invalidate(false);
					//	m_SubFunction5Arrow.Invalidate(false);
					//	m_SubFunction6Arrow.Invalidate(false);
					//	m_SubFunction7Arrow.Invalidate(false);
					//	m_SubFunction8Arrow.Invalidate(false);
					//}
					}
				}
				UpdateButtons();
				ShowOrHideDisplays();
				UpdateThresholdDisplay();
				CWnd *TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText1);
				if (TempWindow)
					TempWindow->Invalidate(false);
			}
			break;	
			case 27: // Escape
				::PostThreadMessage(vGlobalCallingThreadID,	cStartChatWindowMessage, 0,0);
			break;	
			default:
				ReportErrorMessage("Key Not Processed: " + dtoa(pMsg->wParam,0), cAction, 0);
			break;
		}
		vRecievedButtonPress = false;
		return true;  //true indicates it has been handled, so do not process
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CNewImageDisplayDialog::ShowNewImageOnDisplay()
{
	if (vLocalSystemData->vITIPCDig->vOriginalImage)
	{
		if (vLocalSystemData->vITIPCDig->vOriginalImage->vContainerEjectInspection < vGlobalCurrentProduct->vNumberOfInspections)
		if ((vLocalSystemData->vITIPCDig->vOriginalImage->vContainerEjectReason < 255) && (vGlobalCurrentProduct->vInspection[vLocalSystemData->vITIPCDig->vOriginalImage->vContainerEjectInspection]))
		if (!vShowThresholdMenu) //don't change inspection showing if evaluating a threshold
			SetInspectionShowing(vGlobalCurrentProduct->vInspection[vLocalSystemData->vITIPCDig->vOriginalImage->vContainerEjectInspection]);

		vLocalSystemData->vTweakReferenceAdjust = vLocalSystemData->vITIPCDig->vOriginalImage->vFineReferenceAdjust;
		if (!PasswordOK(cTemporaryInspxPassword,false))
			vLocalSystemData->vTweakTopReferenceAdjust = 0;
		if (vInspectionShowing)
		if (vInspectionShowing->vInspectionType == cKernelInspection)
			vLocalSystemData->vTweakReferenceAdjust = 0;

		if (vShowNextFrame)
		{
			vShowNextFrame = false;
			UpdateButtons();
			KillTimer(vCursorUpdateTimerHandle);
			::SetCursor(vGlobalCursorSave);
			//SetCursorPos(320,240);
		}
		if (vLocalSystemData->vLogFile.vLogSerialData)
			vLocalSystemData->vLogFile.WriteToLogFile("NIDD-PTM-Displayed New Image", cDebugMessage);
		DrawImageOnScreen();
		if (vDisplayChanges)
		{
			UpdateButtons();
			UpdateDisplayIndicators();
			vDisplayChanges = false;
		}
	}
}

HBRUSH CNewImageDisplayDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	HWND TempDialogHwnd = this->m_hWnd;
	int TextSize;
	pDC->SetAttribDC(pDC->m_hDC);
	
	int TempControlID = pWnd->GetDlgCtrlID();

	if (TempControlID == IDC_Background)
	{
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (TempControlID == IDC_ErrorMessage)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_ErrorMessage, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}

	if (TempControlID == IDC_DialogTitleStaticText1)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DialogTitleStaticText, 8);  //5 is medium large
		if (vGlobalShiftKeyDown)
			pDC->SetTextColor(cBlack);
		else
			pDC->SetTextColor(vGlobalDialogTitleColor);
	}

	if (TempControlID == IDC_Function1Button)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function1Button, 5);  //5 is medium large
		if (vMakeExitButtonGreen)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cButtonTextColor);
			return vLocalSystemData->vGreenBrush;
	  }

		//make return to Image Display green
	}
	if (TempControlID == IDC_Function2Button)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function2Button, 5);  //5 is medium large
		if (vRejectsImageDisplay)
		{
			if (vFunction2ButtonEnable == true)
				pDC->SetTextColor(cSupervisorTextColor);
			else
				pDC->SetTextColor(cGray);
			pDC->SetBkMode(TRANSPARENT);
			return vGlobalButtonColorBrush;
	  }
	}

	//make Freeze last image red
	if (TempControlID == IDC_Function2Button)
	if (vShowingLiveImages)
	if (!PasswordOK(cTemporaryInspxPassword,false))
	if (vAnalysisImageDisplay)
	{
		pDC->SetBkMode(TRANSPARENT);
		if (vFunction2ButtonEnable == false)
			pDC->SetTextColor(cGray);
		return vLocalSystemData->vRedBrush;
	}

	//make return to Image Display green
	if (TempControlID == IDC_Function1Button)
	if ((vLocalSystemData->vInAutoSetup) || (vAnalysisImageDisplay) || (vFromSetupWindow))
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vLocalSystemData->vGreenBrush;
	}

	//make return to Image Display green
	if (TempControlID == IDC_Function2Button)
	if ((vImageDisplayOptions) || (vEditWhileRunning) || 
		(vShowThresholdMenu) || (vEditROIBoundaries))
	if (!vChangeMade)
	{
		pDC->SetBkMode(TRANSPARENT);
		if (vFunction2ButtonEnable == true)
			pDC->SetTextColor(cButtonTextColor);
		else
			pDC->SetTextColor(cGray);
		return vLocalSystemData->vGreenBrush;
	}

	//make Wait For Next Image yellow
	if (TempControlID == IDC_Function2Button)
	if (vShowNextFrame)
	if (vAnalysisImageDisplay)
	{
		pDC->SetBkMode(TRANSPARENT);
		if (vFunction2ButtonEnable == false)
			pDC->SetTextColor(cGray);
		return vLocalSystemData->vYellowBrush;
	}

	//make save threshold green if change made
	if (TempControlID == IDC_SubFunction7Button)
	if (vShowThresholdMenu)
	if (vChangeMade)
	{
		BYTE TempArrowsAreSet = m_SubFunction1Button.AreNavigationArrowsSet();
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction7Button, 5, TempArrowsAreSet);  
		pDC->SetBkMode(TRANSPARENT);
		if (vSubFunction7ButtonEnable == true)
			pDC->SetTextColor(cButtonTextColor);
		else
			pDC->SetTextColor(cGray);
		return vLocalSystemData->vGreenBrush;
	}

	if (TempControlID == IDC_Function4Button)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function4Button, 5);  //5 is medium large
		if (vEditROIBoundaries)
		if (vChangeMade)
		{
			pDC->SetBkMode(TRANSPARENT);
			if (vFunction4ButtonEnable == true)
				pDC->SetTextColor(cButtonTextColor);
			else
				pDC->SetTextColor(cGray);
			return vLocalSystemData->vGreenBrush;
		}
		if ((!vEditWhileRunning) && (!vImageDisplayOptions) && 
			(!vShowThresholdMenu) && (!vEditROIBoundaries) && (!vImproveLearnMenu))
		if (vHaveAnImageToShow)
		if (vLocalSystemData->vITIPCDig->vRejectBufferShowing >= 0)
		if (vHaveRejects)
		if (!vShowAlternateImage)
		{
			pDC->SetBkMode(TRANSPARENT);
			if (vFunction4ButtonEnable == true)
				pDC->SetTextColor(cButtonTextColor);
			else
				pDC->SetTextColor(cGray);
			return vLocalSystemData->vGreenBrush;
		}
				pDC->SetBkMode(TRANSPARENT);
		if (vFunction4ButtonEnable == true)
			pDC->SetTextColor(cButtonTextColor);
		else
			pDC->SetTextColor(cGray);
		return vGlobalButtonColorBrush;

	}

	//make show next reject green
	if (TempControlID == IDC_Function5Button)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function5Button, 5);  //5 is medium large
		if ((!vEditWhileRunning) && (!vImageDisplayOptions) && 
			(!vShowThresholdMenu) && (!vEditROIBoundaries) && (!vImproveLearnMenu))
		if (vHaveAnImageToShow)
		if (vLocalSystemData->vITIPCDig->vRejectBufferShowing >= 0)
		if (vHaveRejects)
		if (!vShowAlternateImage)
		{
			pDC->SetBkMode(TRANSPARENT);
			if (vFunction5ButtonEnable == true)
				pDC->SetTextColor(cButtonTextColor);
			else
				pDC->SetTextColor(cGray);
			return vLocalSystemData->vGreenBrush;
		}
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction1Button)
	{
		BYTE TempArrowsAreSet = m_SubFunction1Button.AreNavigationArrowsSet();
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction1Button, 5, TempArrowsAreSet);  //5 is medium large
		if (vRejectsImageDisplay)
		{
			if ((vGlobalCurrentProduct->vInspection[0 + vFirstInspectionToView]) &&
				(!vGlobalCurrentProduct->vInspection[0 + vFirstInspectionToView]->vEnable))
			{
				pDC->SetBkMode(TRANSPARENT);
				if (vSubFunction1ButtonEnable == true)
					pDC->SetTextColor(cButtonTextColor);
				else
					pDC->SetTextColor(cGray);
				return vLocalSystemData->vGrayBrush;
			}
			if (!(vGlobalCurrentProduct->vTypesOfRejectsToView & (0x0001 << vFirstInspectionToView)))
			{
				pDC->SetBkMode(TRANSPARENT);
				if (vSubFunction1ButtonEnable == true)
					pDC->SetTextColor(cButtonTextColor);
				else
					pDC->SetTextColor(cGray);
				return vLocalSystemData->vGreenBrush;
			}
		}
		pDC->SetBkMode(TRANSPARENT);
		if (vSubFunction1ButtonEnable == true)
			pDC->SetTextColor(cButtonTextColor);
		else
			pDC->SetTextColor(cGray);
		return vGlobalButtonColorBrush;


	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction2Button)
	{
		BYTE TempArrowsAreSet = m_SubFunction1Button.AreNavigationArrowsSet();
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction2Button, 5, TempArrowsAreSet);  //5 is medium large
		if (vRejectsImageDisplay)
		{
			if (1 + vFirstInspectionToView < vGlobalCurrentProduct->vNumberOfInspections)
			if ((vGlobalCurrentProduct->vInspection[1 + vFirstInspectionToView]) &&
				(!vGlobalCurrentProduct->vInspection[1 + vFirstInspectionToView]->vEnable))
			{
				pDC->SetBkMode(TRANSPARENT);
				if (vSubFunction2ButtonEnable == true)
					pDC->SetTextColor(cButtonTextColor);
				else
					pDC->SetTextColor(cGray);
				return vLocalSystemData->vGrayBrush;
			}
			if (!(vGlobalCurrentProduct->vTypesOfRejectsToView & (0x0002 << vFirstInspectionToView)))
			{
				pDC->SetBkMode(TRANSPARENT);
				if (vSubFunction2ButtonEnable == true)
					pDC->SetTextColor(cButtonTextColor);
				else
					pDC->SetTextColor(cGray);
				return vLocalSystemData->vGreenBrush;
			}
		}
		else
		if (vImproveLearnMenu)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cButtonTextColor);
			return vLocalSystemData->vYellowBrush;
		}
		if (vShowingProcess)
		if (PasswordOK(cTemporaryInspxPassword,false))
		if (vHaveAnImageToShow)
		if (vAnalysisImageDisplay)
		if (!vShowAlternateImage)
		if (!vTweakingThreshold)		//show next and previous step in green if not normal view
		{
			pDC->SetBkMode(TRANSPARENT);
			if (vSubFunction2ButtonEnable == true)
				pDC->SetTextColor(cButtonTextColor);
			else
				pDC->SetTextColor(cGray);
			return vLocalSystemData->vGreenBrush;
		}
		pDC->SetBkMode(TRANSPARENT);
		if (vSubFunction2ButtonEnable == true)
			pDC->SetTextColor(cButtonTextColor);
		else
			pDC->SetTextColor(cGray);
		return vGlobalButtonColorBrush;
	}

	if (pWnd->GetDlgCtrlID() == IDC_SubFunction3Button)
	{
		BYTE TempArrowsAreSet = m_SubFunction1Button.AreNavigationArrowsSet();
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction3Button, 5, TempArrowsAreSet);  
		if (vRejectsImageDisplay)
		{
			if (2 + vFirstInspectionToView < vGlobalCurrentProduct->vNumberOfInspections)
			if ((vGlobalCurrentProduct->vInspection[2 + vFirstInspectionToView]) &&
				(!vGlobalCurrentProduct->vInspection[2 + vFirstInspectionToView]->vEnable))
			{
				pDC->SetBkMode(TRANSPARENT);
				if (vSubFunction3ButtonEnable == true)
					pDC->SetTextColor(cButtonTextColor);
				else
					pDC->SetTextColor(cGray);
				return vLocalSystemData->vGrayBrush;
			}
			if (!(vGlobalCurrentProduct->vTypesOfRejectsToView & (0x0004 << vFirstInspectionToView)))
			{
				pDC->SetBkMode(TRANSPARENT);
				if (vSubFunction3ButtonEnable == true)
					pDC->SetTextColor(cButtonTextColor);
				else
					pDC->SetTextColor(cGray);
				return vLocalSystemData->vGreenBrush;
			}
		}
		if (vShowingProcess)
		if (PasswordOK(cTemporaryInspxPassword,false))
		if (vHaveAnImageToShow)
		if (vAnalysisImageDisplay)
		if (!vShowAlternateImage)
		if (!vTweakingThreshold)	//show next and previous step in green if not normal view
		{
			pDC->SetBkMode(TRANSPARENT);
			if (vSubFunction3ButtonEnable == true)
				pDC->SetTextColor(cButtonTextColor);
			else
				pDC->SetTextColor(cGray);
			return vLocalSystemData->vGreenBrush;
		}
		pDC->SetBkMode(TRANSPARENT);
		if (vSubFunction3ButtonEnable == true)
			pDC->SetTextColor(cButtonTextColor);
		else
			pDC->SetTextColor(cGray);
		return vGlobalButtonColorBrush;
	}

	if (pWnd->GetDlgCtrlID() == IDC_SubFunction4Button)
	{
		BYTE TempArrowsAreSet = m_SubFunction1Button.AreNavigationArrowsSet();
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction4Button, 5, TempArrowsAreSet);  
		if (vRejectsImageDisplay)
		{
			if (3 + vFirstInspectionToView < vGlobalCurrentProduct->vNumberOfInspections)
			if ((vGlobalCurrentProduct->vInspection[3 + vFirstInspectionToView]) &&
				(!vGlobalCurrentProduct->vInspection[3 + vFirstInspectionToView]->vEnable))
			{
				pDC->SetBkMode(TRANSPARENT);
				if (vSubFunction4ButtonEnable == true)
					pDC->SetTextColor(cButtonTextColor);
				else
					pDC->SetTextColor(cGray);
				return vLocalSystemData->vGrayBrush;
			}
			if (!(vGlobalCurrentProduct->vTypesOfRejectsToView & (0x0008 << vFirstInspectionToView)))
			{
				pDC->SetBkMode(TRANSPARENT);
				if (vSubFunction4ButtonEnable == true)
					pDC->SetTextColor(cButtonTextColor);
				else
					pDC->SetTextColor(cGray);
				return vLocalSystemData->vGreenBrush;
			}
		}
		if ((vAnalysisImageDisplay) && (!PasswordOK(cSuperInspxPassword,false)) && 
			((vLocalConfigurationData->vScanTracType == cLegatoScanTrac) || (vLocalConfigurationData->vScanTracType == cFermataScanTrac)))//pipeline ScanTrac
		{
			pDC->SetBkMode(TRANSPARENT);
			if (vSubFunction4ButtonEnable == true)
				pDC->SetTextColor(cBrown);
			else
				pDC->SetTextColor(cGray);
			return vGlobalButtonColorBrush;
		}
		if ((vShowAverage) || (vShowStructure) || (vShowHistogram) || 
			//(vShowGlassStructure) || (vShowGlassHistogram) || 
			(vShowBackground))
		if (vAnalysisImageDisplay)
		if (vHaveAnImageToShow)			//show next and previous step in green if not normal view
		{
			pDC->SetBkMode(TRANSPARENT);
			if (vSubFunction4ButtonEnable == true)
				pDC->SetTextColor(cButtonTextColor);
			else
				pDC->SetTextColor(cGray);
			return vLocalSystemData->vGreenBrush;
		}
		pDC->SetBkMode(TRANSPARENT);
		if (vSubFunction4ButtonEnable == true)
			pDC->SetTextColor(cButtonTextColor);
		else
			pDC->SetTextColor(cGray);
		return vGlobalButtonColorBrush;
	}

	if (pWnd->GetDlgCtrlID() == IDC_SubFunction5Button)
	{
		BYTE TempArrowsAreSet = m_SubFunction1Button.AreNavigationArrowsSet();
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction5Button, 5, TempArrowsAreSet); 
		if (vRejectsImageDisplay)
		{
			if (4 + vFirstInspectionToView < vGlobalCurrentProduct->vNumberOfInspections)
			if ((vGlobalCurrentProduct->vInspection[4 + vFirstInspectionToView]) &&
				(!vGlobalCurrentProduct->vInspection[4 + vFirstInspectionToView]->vEnable))
			{
				pDC->SetBkMode(TRANSPARENT);
				if (vSubFunction5ButtonEnable == true)
					pDC->SetTextColor(cButtonTextColor);
				else
					pDC->SetTextColor(cGray);
				return vLocalSystemData->vGrayBrush;
			}
			if (!(vGlobalCurrentProduct->vTypesOfRejectsToView & (0x0010 << vFirstInspectionToView)))
			{
				pDC->SetBkMode(TRANSPARENT);
				if (vSubFunction5ButtonEnable == true)
					pDC->SetTextColor(cButtonTextColor);
				else
					pDC->SetTextColor(cGray);
				return vLocalSystemData->vGreenBrush;
			}
		}
		pDC->SetBkMode(TRANSPARENT);
		if (vSubFunction5ButtonEnable == true)
			pDC->SetTextColor(cButtonTextColor);
		else
			pDC->SetTextColor(cGray);
		return vGlobalButtonColorBrush;
	}

	if (pWnd->GetDlgCtrlID() == IDC_SubFunction6Button)
	{
		BYTE TempArrowsAreSet = m_SubFunction1Button.AreNavigationArrowsSet();
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction6Button, 5, TempArrowsAreSet);  
		if (vRejectsImageDisplay)
		{
			if (5 + vFirstInspectionToView < vGlobalCurrentProduct->vNumberOfInspections)
			if ((vGlobalCurrentProduct->vInspection[5 + vFirstInspectionToView]) &&
				(!vGlobalCurrentProduct->vInspection[5 + vFirstInspectionToView]->vEnable))
			{
				pDC->SetBkMode(TRANSPARENT);
				if (vSubFunction6ButtonEnable == true)
					pDC->SetTextColor(cButtonTextColor);
				else
					pDC->SetTextColor(cGray);
				return vLocalSystemData->vGrayBrush;
			}
			if (!(vGlobalCurrentProduct->vTypesOfRejectsToView & (0x0020 << vFirstInspectionToView)))
			{
				pDC->SetBkMode(TRANSPARENT);
				if (vSubFunction6ButtonEnable == true)
					pDC->SetTextColor(cButtonTextColor);
				else
					pDC->SetTextColor(cGray);
				return vLocalSystemData->vGreenBrush;
			}
		}
		pDC->SetBkMode(TRANSPARENT);
		if (vSubFunction6ButtonEnable == true)
			pDC->SetTextColor(cButtonTextColor);
		else
			pDC->SetTextColor(cGray);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction7Button)
	{
		BYTE TempArrowsAreSet = m_SubFunction1Button.AreNavigationArrowsSet();
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction7Button, 5, TempArrowsAreSet);  
		if (vRejectsImageDisplay)
		if (HaveAnInspection(8))  
		{
			if (vFirstInspectionToView)
			{
				pDC->SetBkMode(TRANSPARENT);
				if (vSubFunction7ButtonEnable == true)
					pDC->SetTextColor(cButtonTextColor);
				else
					pDC->SetTextColor(cGray);
				return vLocalSystemData->vGreenBrush;
			}
		}
		else
		{//if have 7 inspections, but not 8, then do color for 7th inspection
			if (6 + vFirstInspectionToView < vGlobalCurrentProduct->vNumberOfInspections)
			if ((vGlobalCurrentProduct->vInspection[6 + vFirstInspectionToView]) &&
				(!vGlobalCurrentProduct->vInspection[6 + vFirstInspectionToView]->vEnable))
			{
				pDC->SetBkMode(TRANSPARENT);
				if (vSubFunction7ButtonEnable == true)
					pDC->SetTextColor(cButtonTextColor);
				else
					pDC->SetTextColor(cGray);
				return vLocalSystemData->vGrayBrush;
			}
			if (!(vGlobalCurrentProduct->vTypesOfRejectsToView & (0x0040 << vFirstInspectionToView)))
			{
				pDC->SetBkMode(TRANSPARENT);
				if (vSubFunction7ButtonEnable == true)
					pDC->SetTextColor(cButtonTextColor);
				else
					pDC->SetTextColor(cGray);
				return vLocalSystemData->vGreenBrush;
			}
		}
		pDC->SetBkMode(TRANSPARENT);
		if (vSubFunction7ButtonEnable == true)
			pDC->SetTextColor(cButtonTextColor);
		else
			pDC->SetTextColor(cGray);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction8Button)
	{
		BYTE TempArrowsAreSet = m_SubFunction1Button.AreNavigationArrowsSet();
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction8Button, 5, TempArrowsAreSet); 
		if (!vEditROIBoundaries)
		if (!((vGlobalShiftKeyDown) && (PasswordOK(cTemporaryInspxPassword,false))))
		if (!vShowingProcess)
		if (vHideRedIndicator)
		{
			pDC->SetBkMode(TRANSPARENT);
			if (vSubFunction8ButtonEnable == true)
				pDC->SetTextColor(cButtonTextColor);
			else
				pDC->SetTextColor(cGray);
			return vLocalSystemData->vGreenBrush;
		}
		pDC->SetBkMode(TRANSPARENT);
		if (vSubFunction8ButtonEnable == true)
			pDC->SetTextColor(cButtonTextColor);
		else
			pDC->SetTextColor(cGray);
		return vGlobalButtonColorBrush;
	}
	if (TempControlID == IDC_RejectReason)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_RejectReason, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vYellowBrush;
	}

	if (TempControlID == IDC_XRaysOn)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_XRaysOn, 7);
		pDC->SetAttribDC(pDC->m_hDC);
		pDC->SetTextColor(cYellow);
		pDC->SetBkColor(cRed);
		return vLocalSystemData->vRedBrush;
	}

	if (TempControlID == IDC_Function1Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}

  if (TempControlID == IDC_Function2Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		if (vFunction2ButtonEnable == true)
			pDC->SetTextColor(cButtonTextColor);
		else
			pDC->SetTextColor(cGray);
		return vGlobalButtonColorBrush;
	}

  if (TempControlID == IDC_Function3Button)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function3Button, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		if (vFunction3ButtonEnable == true)
			pDC->SetTextColor(cButtonTextColor);
		else
			pDC->SetTextColor(cGray);
		return vGlobalButtonColorBrush;
	}

	if (TempControlID == IDC_Function5Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		if (vFunction5ButtonEnable == true)
			pDC->SetTextColor(cButtonTextColor);
		else
			pDC->SetTextColor(cGray);
		return vGlobalButtonColorBrush;
	}

	if (pWnd->GetDlgCtrlID() == IDC_SubFunction1Display)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction1Display, 5);
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction2Display)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction2Display, 5);
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction3Display)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction3Display, 5);
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction4Display)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction4Display, 5);
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction5Display)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction5Display, 5);
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction6Display)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction6Display, 5);
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction7Display)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction7Display, 5);
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction8Display)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction8Display, 5);

	//if (pWnd->GetDlgCtrlID() == IDC_Sensitivity)
	//	TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Sensitivity, 5);
	if (pWnd->GetDlgCtrlID() == IDC_MainStatusLine)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_MainStatusLine, 5);
	if (pWnd->GetDlgCtrlID() == IDC_ImageStatus)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_ImageStatus, 5);
	if (pWnd->GetDlgCtrlID() == IDC_InspectionThreshold)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_InspectionThreshold, 5);
	if (pWnd->GetDlgCtrlID() == IDC_CurrentInspection)
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_CurrentInspection, 5);	//not button, but has two lines
	return hbr;
}

void CNewImageDisplayDialog::OnTimer(UINT nIDEvent) 
{

	if (nIDEvent == vCursorUpdateTimerHandle)
	{
		::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_WAIT)));
		KillTimer(vCursorUpdateTimerHandle);
	}

	if (nIDEvent == vUpdateDisplayTimerHandle)
	{
		WaitThenDrawImageOnScreen();
		KillTimer(vUpdateDisplayTimerHandle);
	}
	else
	if (nIDEvent == vFinishLoadingFileTimerHandle)
	{
		KillTimer(vFinishLoadingFileTimerHandle);
		FinishLoadingFile();
	}
	else
	if (nIDEvent == vWaitForScreenUpdateTimerHandle)
	{
		KillTimer(vWaitForScreenUpdateTimerHandle);
		vWaitForScreenUpdate = false;
		DrawImageOnScreen();
	}
	else
	if (nIDEvent == vOneSecondTimerHandle)
	{
		OneSecondUpdate();
	}
	else
	if (nIDEvent == vMouseDownRepeatTimerHandle)
	{
		switch (vMouseState)
		{
			case 1:
			case 9:
				OnSubFunction1Button();
				break;
			case 2:
			case 10:
				OnSubFunction2Button();
				break;
			case 3:
			case 11:
				OnSubFunction3Button();
				break;
			case 4:
			case 12:
				OnSubFunction4Button();
				break;
			case 5:
			case 13:
				OnSubFunction5Button();
				break;
			case 6:
			case 14:
				OnSubFunction6Button();
				break;
			case 7:
			case 15:
				OnSubFunction7Button();
				break;
			case 8:
			case 16:
				OnSubFunction8Button();
				break;
			default:
				break;
		}
	}

	CDialog::OnTimer(nIDEvent);
}

void CNewImageDisplayDialog::UpdateRejectCounts()
{
	SetDlgItemText(IDC_SubFunction1Display,dtoaWithCommas(vOldCounter[0],0));
	SetDlgItemText(IDC_SubFunction2Display,dtoaWithCommas(vOldCounter[1],0));
	SetDlgItemText(IDC_SubFunction3Display,dtoaWithCommas(vOldCounter[2],0));
	SetDlgItemText(IDC_SubFunction4Display,dtoaWithCommas(vOldCounter[3],0));
	SetDlgItemText(IDC_SubFunction5Display,dtoaWithCommas(vOldCounter[4],0));
	SetDlgItemText(IDC_SubFunction6Display,dtoaWithCommas(vOldCounter[5],0));
	SetDlgItemText(IDC_SubFunction7Display,dtoaWithCommas(vOldCounter[6],0));
	//SetDlgItemText(IDC_SubFunction8Display,dtoaWithCommas(vOldCounter[7],0));
	if (PasswordOK(cTemporaryInspxPassword,false))
	{
		if (vLocalSystemData->vGoodImagesIndex)
		if (vLocalSystemData->vGoodImagesIndex - vFirstInspectionToView < 9)
		{
			vOldGoodCountTotal = vLocalSystemData->vGoodCountTotal;
			CString TempString = dtoa(vOldGoodCountTotal,0) + ", " +
				dtoa(vLocalSystemData->vGoodCountInBufferTotal,0);
			switch (vLocalSystemData->vGoodImagesIndex - vFirstInspectionToView)
			{
				case 1:
					SetDlgItemText(IDC_SubFunction1Display,TempString);
				break;
				case 2:
					SetDlgItemText(IDC_SubFunction2Display,TempString);
				break;
				case 3:
					SetDlgItemText(IDC_SubFunction3Display,TempString);
				break;
				case 4:
					SetDlgItemText(IDC_SubFunction4Display,TempString);
				break;
				case 5:
					SetDlgItemText(IDC_SubFunction5Display,TempString);
				break;
				case 6:
					SetDlgItemText(IDC_SubFunction6Display,TempString);
				break;
				case 7:
					if (!HaveAnInspection(8))
						SetDlgItemText(IDC_SubFunction7Display,TempString);
				break;
				//case 8:
				//	if (!HaveAnInspection(9))
				//		SetDlgItemText(IDC_SubFunction8Display,TempString);
				//break;
			}
		}
	}
}

void CNewImageDisplayDialog::OneSecondUpdate()
{
	if (vLocalSystemData->vITIPCDig->vALearnedImageHasChanged)
	{
		vLocalSystemData->vITIPCDig->vALearnedImageHasChanged = false;
		if (vImproveLearnMenu)
			DrawImageOnScreen();
	}

	if (vImproveLearnMenu)
	{
		if (vOldAddToLearnCount != vGlobalCurrentProduct->vAddToLearnCount)
		{
			vOldAddToLearnCount = vGlobalCurrentProduct->vAddToLearnCount;
			UpdateThresholdDisplay();
		}

		if (vOldImproveLearnTotalContainers != vLocalSystemData->vImproveLearnTotalContainers)
		{
			//BYTE TempILIN = vMainWindowPointer->GetImproveLearnInspectionNumber() - 1;
			//vOldCounter[cImproveLearnIndex] = 
			//	vLocalSystemData->vXRayCriteriaCountButDisabled[cImproveLearnIndex];
			vOldImproveLearnTotalContainers = vLocalSystemData->vImproveLearnTotalContainers;

			CString TempString("");
			TempString = "";
			if (vOldImproveLearnTotalContainers)
				TempString = dtoa(vLocalSystemData->vITIPCDig->vImproveLearnCount,0) + " of " + dtoa(vOldImproveLearnTotalContainers,0) + "\n" +
					dtoa((double)vLocalSystemData->vITIPCDig->vImproveLearnCount / (double)vOldImproveLearnTotalContainers * 100.0,3) + "%";
			SetDlgItemText(IDC_CurrentInspection,TempString);
		}
		if (vOldImproveLearnCount != vLocalSystemData->vITIPCDig->vImproveLearnCount)
		{
			//if (!vOldImproveLearnCount)
			//{
			//	UpdateButtons();
			//	vShowDeltaCurrentPreviewStructure = true;
			//}
			vOldImproveLearnCount = vLocalSystemData->vITIPCDig->vImproveLearnCount;
			if ((vShowImproveLearnPreview) || (vShowDeltaCurrentPreviewStructure))
				DrawImageOnScreen();
		}
	}

	//if ((vOldUnderRemoteControl) && (!vGlobalUnderRemoteControl))
	//{
	//	SetDlgItemText(IDC_DialogTitleStaticText1,vDialogTitleString);
	//}
	//else
	//if (vGlobalUnderRemoteControl)
	//{
	//	if (vGlobalEvenSecond)
	//	{
	//		CString TempText;
	//		TempText.LoadString(IDS_UnderRemoteControl);
	//		SetDlgItemText(IDC_DialogTitleStaticText1,TempText);
	//	}
	//	else
	//		SetDlgItemText(IDC_DialogTitleStaticText1,vDialogTitleString);
	//}
	//vOldUnderRemoteControl = vGlobalUnderRemoteControl;

	if (vRejectsImageDisplay)
	{
		if (PasswordOK(cTemporaryInspxPassword,false))
		{
			if (vOldGoodCountTotal != vLocalSystemData->vGoodCountTotal)
			if (vRejectsImageDisplay)
			if (vLocalSystemData->vGoodImagesIndex - vFirstInspectionToView < 9)
			{
				vOldGoodCountTotal = vLocalSystemData->vGoodCountTotal;
				CString TempString = dtoa(vOldGoodCountTotal,0) + ", " +
					dtoa(vLocalSystemData->vGoodCountInBufferTotal,0);
				if (vLocalSystemData->vGoodImagesIndex)
				if (vLocalSystemData->vGoodImagesIndex - vFirstInspectionToView < 9)
				switch (vLocalSystemData->vGoodImagesIndex - vFirstInspectionToView)
				{
					case 1:
						SetDlgItemText(IDC_SubFunction1Display,TempString);
					break;
					case 2:
						SetDlgItemText(IDC_SubFunction2Display,TempString);
					break;
					case 3:
						SetDlgItemText(IDC_SubFunction3Display,TempString);
					break;
					case 4:
						SetDlgItemText(IDC_SubFunction4Display,TempString);
					break;
					case 5:
						SetDlgItemText(IDC_SubFunction5Display,TempString);
					break;
					case 6:
						SetDlgItemText(IDC_SubFunction6Display,TempString);
					break;
					case 7:
						if (!HaveAnInspection(8))
							SetDlgItemText(IDC_SubFunction7Display,TempString);
					break;
					//case 8:
					//	if (!HaveAnInspection(9))
					//		SetDlgItemText(IDC_SubFunction8Display,TempString);
					//break;
				}
			}
		}

		if (vOldCounter[0] != vLocalSystemData->vXRayCriteriaCountTotal[0 + vFirstInspectionToView])
		if (vTotalNumberOfInspectionsToDisplay > (0 + vFirstInspectionToView))
		{
			vOldCounter[0] = vLocalSystemData->vXRayCriteriaCountTotal[0 + vFirstInspectionToView];
			SetDlgItemText(IDC_SubFunction1Display,dtoaWithCommas(vOldCounter[0],0));
		}

		if (vOldCounter[1] != vLocalSystemData->vXRayCriteriaCountTotal[1 + vFirstInspectionToView])
		if (vTotalNumberOfInspectionsToDisplay > (1 + vFirstInspectionToView))
		{
			vOldCounter[1] = vLocalSystemData->vXRayCriteriaCountTotal[1 + vFirstInspectionToView];
			SetDlgItemText(IDC_SubFunction2Display,dtoaWithCommas(vOldCounter[1],0));
		}

		if (vOldCounter[2] != vLocalSystemData->vXRayCriteriaCountTotal[2 + vFirstInspectionToView])
		if (vTotalNumberOfInspectionsToDisplay > (2 + vFirstInspectionToView))
		{
			vOldCounter[2] = vLocalSystemData->vXRayCriteriaCountTotal[2 + vFirstInspectionToView];
			SetDlgItemText(IDC_SubFunction3Display,dtoaWithCommas(vOldCounter[2],0));
		}

		if (vOldCounter[3] != vLocalSystemData->vXRayCriteriaCountTotal[3 + vFirstInspectionToView])
		if (vTotalNumberOfInspectionsToDisplay > (3 + vFirstInspectionToView))
		{
			vOldCounter[3] = vLocalSystemData->vXRayCriteriaCountTotal[3 + vFirstInspectionToView];
			SetDlgItemText(IDC_SubFunction4Display,dtoaWithCommas(vOldCounter[3],0));
		}

		if (vOldCounter[4] != vLocalSystemData->vXRayCriteriaCountTotal[4 + vFirstInspectionToView])
		if (vTotalNumberOfInspectionsToDisplay > (4 + vFirstInspectionToView))
		{
			vOldCounter[4] = vLocalSystemData->vXRayCriteriaCountTotal[4 + vFirstInspectionToView];
			SetDlgItemText(IDC_SubFunction5Display,dtoaWithCommas(vOldCounter[4],0));
		}

		if (vOldCounter[5] != vLocalSystemData->vXRayCriteriaCountTotal[5 + vFirstInspectionToView])
		if (vTotalNumberOfInspectionsToDisplay > (5 + vFirstInspectionToView))
		{
			vOldCounter[5] = vLocalSystemData->vXRayCriteriaCountTotal[5 + vFirstInspectionToView];
			SetDlgItemText(IDC_SubFunction6Display,dtoaWithCommas(vOldCounter[5],0));
		}

		if (vOldCounter[6] != vLocalSystemData->vXRayCriteriaCountTotal[6 + vFirstInspectionToView])
		if (vTotalNumberOfInspectionsToDisplay > (6 + vFirstInspectionToView))
		{
			vOldCounter[6] = vLocalSystemData->vXRayCriteriaCountTotal[6 + vFirstInspectionToView];
			SetDlgItemText(IDC_SubFunction7Display,dtoaWithCommas(vOldCounter[6],0));
		}

		//if (vOldCounter[7] != vLocalSystemData->vXRayCriteriaCountTotal[7 + vFirstInspectionToView])
		//if (vTotalNumberOfInspectionsToDisplay > (7 + vFirstInspectionToView))
		//{
		//	vOldCounter[7] = vLocalSystemData->vXRayCriteriaCountTotal[7 + vFirstInspectionToView];
		//	SetDlgItemText(IDC_SubFunction8Display,dtoaWithCommas(vOldCounter[7],0));
		//}
	}

	if (vOldGlobalPasswordOK != PasswordOK(cTemporaryInspxPassword,false))
	{
		vOldGlobalPasswordOK = PasswordOK(cTemporaryInspxPassword,false);
		UpdateButtons();
		CWnd *TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText1);
		if (TempWindow)
			TempWindow->Invalidate(false);
	}

	if (vOldCalibrateBodyTriggerReferenceEnabled != 
		vLocalSystemData->vITIPCDig->vCalibrateBodyTriggerReferenceEnabled)
	{
		vOldCalibrateBodyTriggerReferenceEnabled = 
			vLocalSystemData->vITIPCDig->vCalibrateBodyTriggerReferenceEnabled;
		UpdateButtons();
	}

	if (vLocalSystemData->vITIPCDig->vAverageEnabled != vOldAverageEnabled)
	{
		vOldAverageEnabled = vLocalSystemData->vITIPCDig->vAverageEnabled;
		UpdateButtons();
	}
	if ((vLocalSystemData->vITIPCDig->vStructureEnabled != vOldStructureEnabled) &&
		(vOldStructureEnabled) &&
		(vGlobalCurrentProduct->vSetupHistoryAverageDensity))
	{
		vOldStructureEnabled = vLocalSystemData->vITIPCDig->vStructureEnabled;
		UpdateButtons();
		vAverageDensity = vGlobalCurrentProduct->vSetupHistoryAverageDensity;
	}
	if ((vLocalSystemData->vITIPCDig->vStructureEnabled != vOldStructureEnabled) &&
		(!vOldStructureEnabled))
	{
		vOldStructureEnabled = vLocalSystemData->vITIPCDig->vStructureEnabled;
		UpdateButtons();
	}
	if (vLocalSystemData->vTakingQuickLearnImages != vOldTakingQuickLearnImages)
	{
		if (vOldTakingQuickLearnImages)
		{
			vAverageDensity = vGlobalCurrentProduct->vSetupHistoryAverageDensity;
		}
		vOldTakingQuickLearnImages = vLocalSystemData->vTakingQuickLearnImages;
		UpdateButtons();
	}

	if (vDisplayingImageData)
	{
		if (vOldTakingQuickLearnImages)
		{
			if (vOldLearnCount != vLocalSystemData->vQuickLearnImageIndex)
			{
				vOldLearnCount = vLocalSystemData->vQuickLearnImageIndex;
				UpdateDisplay(vDisplayingImageData);
			}
		}
		if (vOldCalibrateBodyTriggerReferenceEnabled)
		{
			if (vOldLearnCount != vLocalSystemData->vITIPCDig->vCalibrateBodyTriggerReferenceCount)
			{
				vOldLearnCount = vLocalSystemData->vITIPCDig->vCalibrateBodyTriggerReferenceCount;
				UpdateDisplay(vDisplayingImageData);
			}
		}
		if (vOldAverageEnabled)
		{
			if (vOldLearnCount != vLocalSystemData->vITIPCDig->vAverageCount)
			{
				vOldLearnCount = vLocalSystemData->vITIPCDig->vAverageCount;
				UpdateDisplay(vDisplayingImageData);
			}
		}
		if (vOldStructureEnabled)
		{
			if (vOldLearnCount != vLocalSystemData->vITIPCDig->vStructureCount)
			{
				vOldLearnCount = vLocalSystemData->vITIPCDig->vStructureCount;
				UpdateDisplay(vDisplayingImageData);
			}
		}
	}

	if (vOldXRayOnNotice != vLocalSystemData->vXRayOnNotice)
	{
		vOldXRayOnNotice = vLocalSystemData->vXRayOnNotice;
		if (vOldXRayOnNotice)
			m_XRaysOn.ShowWindow(SW_SHOW);
		else
		{
			m_XRaysOn.ShowWindow(SW_HIDE);
		}
	}

	if (vOldSystemRunMode != vLocalSystemData->vSystemRunMode)
	{
		if ((vOldSystemRunMode == cRunningSystemMode) && (vLocalSystemData->vSystemRunMode == cStoppedSystemMode))
		{
			vOldSystemRunMode = vLocalSystemData->vSystemRunMode;
			CNoticeDialog TempNoticeDialog;
			CString TempString("");
			TempString = "";
			TempString.LoadString(IDS_SystemStoppedInspecting);//"\n\n\nSystem Stopped Inspecting"
			TempNoticeDialog.vNoticeText = TempString;
			TempNoticeDialog.vType = cNoticeMessage;
			TempNoticeDialog.DoModal();
		}
		else
			vOldSystemRunMode = vLocalSystemData->vSystemRunMode;
	}

	if (!vHaveRejects)
	{
		if (vMainWindowPointer->HaveRejects(PasswordOK(cTemporaryInspxPassword,false)))//false))
		{
			vHaveRejects = true;
			UpdateButtons();
			UpdateDisplayIndicators();
		}
	}
}


void CNewImageDisplayDialog::UpdateButtons()
{
	//function1Button
	CString TempText("");
	if ((vAnalysisImageDisplay) || (vShowThresholdMenu))
	{
		m_Function1Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		TempText.LoadString(IDS_BacktoRejects);
	  SetDlgItemText(IDC_Function1Button,TempText);
	}
	else
	if ((vLocalSystemData->vInAutoSetup) || (vFromSetupWindow))
	{
		m_Function1Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		TempText.LoadString(IDS_ReturntoAutoSetup);
	  SetDlgItemText(IDC_Function1Button,TempText);
	}
	else 
	{
		TempText.LoadString(IDS_MainMenu);
		m_Function1Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
	  SetDlgItemText(IDC_Function1Button,TempText);
	}

	//function2Button
	if (vEditROIBoundaries)
	{
		m_Function2Button.ShowWindow(SW_SHOW);
		vFunction2ButtonEnable = true;	//m_Function2Button.EnableWindow(true);		
		TempText.LoadString(IDS_ImageAnalysis);
	  SetDlgItemText(IDC_Function2Button,TempText);
	}
	else
	if (vRejectsImageDisplay)
	{
		m_Function2Button.ShowWindow(SW_SHOW);
		vFunction2ButtonEnable = true;	//m_Function2Button.EnableWindow(true);		
		TempText.LoadString(IDS_ImageAnalysis);
	  SetDlgItemText(IDC_Function2Button,TempText);
	}
	else
	if ((vEditWhileRunning) || (vImageDisplayOptions) || (vShowThresholdMenu) ||
		(vImproveLearnMenu))
	{
		m_Function2Button.ShowWindow(SW_SHOW);
		vFunction2ButtonEnable = true;	//m_Function2Button.EnableWindow(true);
		TempText.LoadString(IDS_ReturnToImageMenu);
		SetDlgItemText(IDC_Function2Button,TempText);
	}
	else
	if (vAnalysisImageDisplay)
	{
		if (PasswordOK(cAllansPassword,false))
		{			//if (vMainWindowPointer->GetImproveLearnInspectionNumber())
			//{
				m_Function2Button.ShowWindow(SW_SHOW);
				//if (vLocalSystemData->vXRayCriteriaCountButDisabled[cImproveLearnIndex])
				vFunction2ButtonEnable = true;	//m_Function2Button.EnableWindow(true);
				TempText.LoadString(IDS_ImproveLearnMenu);
				SetDlgItemText(IDC_Function2Button,TempText);
		}
		else
		{
			if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)
				vFunction2ButtonEnable = true;	//m_Function2Button.EnableWindow(true);
			else
				vFunction2ButtonEnable = false;	//m_Function2Button.EnableWindow(false);

			if (vShowingLiveImages)
				TempText.LoadString(IDS_FreezeLastImage);
			else
				TempText.LoadString(IDS_ShowImages);
			m_Function2Button.ShowWindow(SW_SHOW);
			SetDlgItemText(IDC_Function2Button,TempText);
		}
	}

	//function3Button
	if (vImproveLearnMenu)
	{
		m_Function3Button.ShowWindow(SW_SHOW);
		if (vLocalSystemData->vITIPCDig->HaveLearnImageFile("=Process 2o"))
		{
			if (vShowOriginalStructure)
				vFunction3ButtonEnable = false;		//m_Function3Button.EnableWindow(false);				
			else
				vFunction3ButtonEnable = true;		//m_Function3Button.EnableWindow(true);				
		}
		else
			vFunction3ButtonEnable = false;			//m_Function3Button.EnableWindow(false);
		SetDlgItemText(IDC_Function3Button, _T("Show\nOriginal"));
	}
	else
	{
		m_Function3Button.ShowWindow(SW_SHOW);
		if (vHaveAnImageToShow)
			vFunction3ButtonEnable = true;		//m_Function3Button.EnableWindow(true);
		else
			vFunction3ButtonEnable = false;		//m_Function3Button.EnableWindow(false);
		TempText.LoadString(IDS_RejectInformation);
		SetDlgItemText(IDC_Function3Button,TempText);
	}

	//function4Button
	if (vImproveLearnMenu)
	{
		m_Function4Button.ShowWindow(SW_SHOW);
		if (vLocalSystemData->vITIPCDig->vProductStructureImage)
		{
			if (vShowStructure)
				vFunction4ButtonEnable = false;		//m_Function4Button.EnableWindow(false);
			else
				vFunction4ButtonEnable = true;		//m_Function4Button.EnableWindow(true);
		}
		else
			vFunction4ButtonEnable = false;			//m_Function4Button.EnableWindow(false);
		SetDlgItemText(IDC_Function4Button, _T("Show\nCurrent"));
	}
	else
	if (vEditROIBoundaries)
	{
		vFunction4ButtonEnable = true;		//m_Function4Button.EnableWindow(true);
		SetDlgItemText(IDC_Function4Button, _T("Save New ROI"));
		m_Function4Button.ShowWindow(SW_SHOW);
	}
	else
	if ((vEditWhileRunning) || (vImageDisplayOptions))
	{
		/*
		if (vHaveAnImageToShow)
		{
			//m_Function4Button.EnableWindow(true);
			vFunction4ButtonEnable = true;
			m_Function4Button.ShowWindow(SW_SHOW);
			CString TempString;
			TempString.LoadString(IDS_RejectInformation);
			SetDlgItemText(IDC_Function4Button,TempString);
		}
		else
		*/
		{
			m_Function4Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_Function4Button.GetRectToInvalidateArrow());
		}
	}
	else
	{
		m_Function4Button.ShowWindow(SW_SHOW);

		if ((!(vMainWindowPointer->HaveRejects(PasswordOK(cTemporaryInspxPassword,false)))) && (!((PasswordOK(cTemporaryInspxPassword,false)) && (vHaveAnImageToShow))))
		{
			vFunction4ButtonEnable = false;		//m_Function4Button.EnableWindow(false);
			TempText.LoadString(IDS_NoRejectsWithCarriageReturn);
			SetDlgItemText(IDC_Function4Button,TempText);
		}
		else
		if (vAllRejectsHidden)
		{
			vFunction4ButtonEnable = false;		//m_Function4Button.EnableWindow(false);
			TempText.LoadString(IDS_RejectsHidden);
			SetDlgItemText(IDC_Function4Button,TempText);
		}
		else
		if (vLocalSystemData->vITIPCDig->vRejectBufferShowing == 0)
		{
			TempText.LoadString(IDS_ShowNormal);
			vFunction4ButtonEnable = true;		//m_Function4Button.EnableWindow(true);
			SetDlgItemText(IDC_Function4Button,TempText);
		}
		else
		if (vLocalSystemData->vITIPCDig->vRejectBufferShowing == -1)
		{
			vFunction4ButtonEnable = true;		//m_Function4Button.EnableWindow(true);
			TempText.LoadString(IDS_ShowLastReject);
			SetDlgItemText(IDC_Function4Button,TempText);
		}
		else
		{
			vFunction4ButtonEnable = true;		//m_Function4Button.EnableWindow(true);
			TempText.LoadString(IDS_ShowReject);
			SetDlgItemText(IDC_Function4Button,TempText + " " + dtoa(vLocalSystemData->vITIPCDig->vRejectBufferShowing, 0));
		}
	}

	//function5Button
	if (vImproveLearnMenu)
	{
		m_Function5Button.ShowWindow(SW_SHOW);
		if (vLocalSystemData->vITIPCDig->vImproveLearnCount)
		{
			if (vShowImproveLearnPreview)
				vFunction5ButtonEnable = false;		//m_Function5Button.EnableWindow(false);
			else				
				vFunction5ButtonEnable = true;		//m_Function5Button.EnableWindow(true);
		}
		else
			vFunction5ButtonEnable = false;			//m_Function5Button.EnableWindow(false);
		SetDlgItemText(IDC_Function5Button, _T("Preview Improve"));
	}
	else
	if (vEditROIBoundaries)
	{
		m_Function5Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_Function5Button.GetRectToInvalidateArrow());
	}
	else
	if (vEditWhileRunning)
	{
		{
			if ((vInspectionShowing) && 
				((vLocalSystemData->vSystemRunMode == cRunningSystemMode) || 
				(vLocalSystemData->vSystemRunMode == cStoppedSystemMode)) &&
				(!vLocalSystemData->vITIPCDig->vAverageEnabled) &&
				(!vLocalSystemData->vITIPCDig->vStructureEnabled) &&
				(!vLocalSystemData->vITIPCDig->vCalibrateBodyTriggerReferenceEnabled) &&
				(!vLocalSystemData->vTakingQuickLearnImages))
				vFunction5ButtonEnable = true;		//m_Function5Button.EnableWindow(true);
			else
				vFunction5ButtonEnable = false;		//m_Function5Button.EnableWindow(false);
			m_Function5Button.ShowWindow(SW_SHOW);
			TempText.LoadString(IDS_EvaluateInspection);
			SetDlgItemText(IDC_Function5Button,TempText);
		}
	}
	else
	if (vImageDisplayOptions)
	{
		if (PasswordOK(cTemporaryInspxPassword,false))
		{
			if ((vDisplayingImageData) && (vHaveAnImageToShow))		
				vSubFunction2ButtonEnable = true;		//m_SubFunction2Button.EnableWindow(true);
			else
				vSubFunction2ButtonEnable = false;		//m_SubFunction2Button.EnableWindow(false);
			m_Function5Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
			TempText = "Save Screen to File";
			m_Function5Button.ShowWindow(SW_SHOW);
			vFunction5ButtonEnable = true;		//m_Function5Button.EnableWindow(true);
			SetDlgItemText(IDC_Function5Button,TempText);
		}
		else
		{
			m_Function5Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_Function5Button.GetRectToInvalidateArrow());
		}
	}
	else
	{
		m_Function5Button.ShowWindow(SW_SHOW);

		if ((!(vMainWindowPointer->HaveRejects(PasswordOK(cTemporaryInspxPassword,false)))) && (!((PasswordOK(cTemporaryInspxPassword,false)) && (vHaveAnImageToShow))))
		{
			vFunction5ButtonEnable = false;		//m_Function5Button.EnableWindow(false);
			TempText.LoadString(IDS_NoRejectsWithCarriageReturn);
			SetDlgItemText(IDC_Function5Button,TempText);
		}
		else
		if (vAllRejectsHidden)
		{
			vFunction5ButtonEnable = false;		//m_Function5Button.EnableWindow(false);
			TempText.LoadString(IDS_RejectsHidden);
			SetDlgItemText(IDC_Function5Button,TempText);
		}
		else
		if (((vLocalSystemData->vITIPCDig->vRejectBufferShowing + 1) >= vNumberOfRejectsInLocalBuffer) || (vLocalLastNRejectImages[vLocalSystemData->vITIPCDig->vRejectBufferShowing + 1] == NULL))
		{
			if (vLocalSystemData->vITIPCDig->vRejectBufferShowing == -1)
			{
				vFunction5ButtonEnable = true;		//m_Function5Button.EnableWindow(true);
				TempText.LoadString(IDS_ShowReject);
				SetDlgItemText(IDC_Function5Button,TempText + " 1");
			}
			else
			{
				vFunction5ButtonEnable = true;		//m_Function5Button.EnableWindow(true);
				TempText.LoadString(IDS_ShowNormal);
				SetDlgItemText(IDC_Function5Button,TempText);
			}
		}
		else
		{
			vFunction5ButtonEnable = true;			//m_Function5Button.EnableWindow(true);
			TempText.LoadString(IDS_ShowReject);
			SetDlgItemText(IDC_Function5Button,TempText + " " + dtoa(vLocalSystemData->vITIPCDig->vRejectBufferShowing + 2, 0));
		}
	}

	//Subfunction1Button
	if (vImproveLearnMenu)
	{
		if (vLocalSystemData->vITIPCDig->vImproveLearnCount)
			vSubFunction1ButtonEnable = true;		//m_SubFunction1Button.EnableWindow(true);
		else			
			vSubFunction1ButtonEnable = false;		//m_SubFunction1Button.EnableWindow(false);

		m_SubFunction1Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		SetDlgItemText(IDC_SubFunction1Button, _T("Clear\nImprove\nLearn"));
		m_SubFunction1Button.ShowWindow(SW_SHOW);
	}
	else
	if (vEditROIBoundaries)
	{		
		vSubFunction1ButtonEnable = true;		//m_SubFunction1Button.EnableWindow(true);
		TempText = vGlobalTopString;
		SetDlgItemText(IDC_SubFunction1Button,TempText + "\n\n");
		SetDlgItemText(IDC_SubFunction1Button,TempText + "\n\n");
		m_SubFunction1Button.ShowWindow(SW_SHOW);
	}
	else
	if (vShowThresholdMenu)
	{
		vSubFunction1ButtonEnable = true;		//m_SubFunction1Button.EnableWindow(true);
		m_SubFunction1Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		m_SubFunction1Button.ShowWindow(SW_SHOW);
		TempText.LoadString(IDS_ShowThreshold);
		SetDlgItemText(IDC_SubFunction1Button,TempText + " +10");
	}
	else
	if (vRejectsImageDisplay)
	{
		if (HaveAnInspection(1 + vFirstInspectionToView))
		{			
			vSubFunction1ButtonEnable = true;		//m_SubFunction1Button.EnableWindow(true);
			m_SubFunction1Button.ShowWindow(SW_SHOW);
			CString TempName = GetInspectionName(1 + vFirstInspectionToView);
			if (TempName.GetLength() < 10)
				m_SubFunction1Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
			else
				m_SubFunction1Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
			SetDlgItemText(IDC_SubFunction1Button,TempName);
		}
		else
		if ((PasswordOK(cTemporaryInspxPassword,false)) && (vLocalSystemData->vGoodImagesIndex == 1))
		{
			m_SubFunction1Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_SubFunction1Button.GetRectToInvalidateArrow());
		}
	}
	else
	if (vImageDisplayOptions)
	{
		if ((vLocalSystemData->vSystemRunMode != cStoppedSystemMode) &&  (PasswordOK(cTemporaryInspxPassword,false)))
		{
			m_SubFunction1Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
			TempText = "Save\nImage\nSeries";
			m_SubFunction1Button.ShowWindow(SW_SHOW);
			vSubFunction1ButtonEnable = true;		//m_SubFunction1Button.EnableWindow(true);
			SetDlgItemText(IDC_SubFunction1Button,TempText);
		}
		else
		{
			m_SubFunction1Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_SubFunction1Button.GetRectToInvalidateArrow());
		}
	}
	else
	{
		m_SubFunction1Button.ShowNavigationArrow(NULL);
		if (vGlobalCurrentProduct->vNumberOfInspections > 1)
		{
			vSubFunction1ButtonEnable = true;		//m_SubFunction1Button.EnableWindow(true);
			m_SubFunction1Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
			m_SubFunction1Button.ShowWindow(SW_SHOW);
			TempText.LoadString(IDS_SelectInspectionToView);
			SetDlgItemText(IDC_SubFunction1Button,TempText);
		}
		else
		{
			m_SubFunction1Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_SubFunction1Button.GetRectToInvalidateArrow());
		}
	}

	//Subfunction2Button
	if (vEditROIBoundaries)
	{
		vSubFunction2ButtonEnable = true;	//m_SubFunction2Button.EnableWindow(true);
		TempText = vGlobalTopString;
		SetDlgItemText(IDC_SubFunction1Button,TempText);
		SetDlgItemText(IDC_SubFunction2Button,TempText);
		m_SubFunction2Button.ShowWindow(SW_SHOW);
	}
	else
	if ((vShowThresholdMenu) && (vTweakingThreshold))
	{
		vSubFunction2ButtonEnable = true;	//m_SubFunction2Button.EnableWindow(true);
		TempText.LoadString(IDS_ShowThreshold);
		SetDlgItemText(IDC_SubFunction2Button,TempText + " +1");
		m_SubFunction2Button.ShowWindow(SW_SHOW);
	}
	else
	if (vRejectsImageDisplay)
	{
		if (HaveAnInspection(2 + vFirstInspectionToView))
		{
			vSubFunction2ButtonEnable = true;		//m_SubFunction2Button.EnableWindow(true);
			m_SubFunction2Button.ShowWindow(SW_SHOW);
			CString TempName = GetInspectionName(2 + vFirstInspectionToView);
			if (TempName.GetLength() < 10)
				m_SubFunction2Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
			else
				m_SubFunction2Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
			SetDlgItemText(IDC_SubFunction2Button,TempName);
		}
		else
		{
			m_SubFunction2Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_SubFunction2Button.GetRectToInvalidateArrow());
		}
	}
	else
	{
		m_SubFunction2Button.ShowNavigationArrow(NULL);
		if (vImproveLearnMenu)
		{
			if ((vLocalSystemData->vITIPCDig->HaveLearnImageFile("=Process 2o")) &&
				(vGlobalCurrentProduct) && (vGlobalCurrentProduct->vAddToLearnCount))
			{
				vSubFunction2ButtonEnable = true;		//m_SubFunction2Button.EnableWindow(true);
				m_SubFunction2Button.ShowWindow(SW_SHOW);
				SetDlgItemText(IDC_SubFunction2Button, _T("Revert to Original Learn!"));
			}
			else
			{
				m_SubFunction2Button.ShowWindow(SW_HIDE);
				this->InvalidateRect(&m_SubFunction2Button.GetRectToInvalidateArrow());
			}
		}
		else
		if (vEditWhileRunning)// || ((vImproveLearnMenu) && (PasswordOK(cTemporaryInspxPassword,false))))
		{
			m_SubFunction2Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
			if (vInspectionShowing)
				vSubFunction2ButtonEnable = true;		//m_SubFunction2Button.EnableWindow(true);
			else
				vSubFunction2ButtonEnable = false;		//m_SubFunction2Button.EnableWindow(false);
			m_SubFunction2Button.ShowWindow(SW_SHOW);
			TempText.LoadString(IDS_ChangeROIBounds);
			SetDlgItemText(IDC_SubFunction2Button,TempText);
		}
		else
		if (vImageDisplayOptions)
		{
			m_SubFunction2Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
			vSubFunction2ButtonEnable = true;	//m_SubFunction2Button.EnableWindow(true);
			if ((vDisplayingImageData) && (vHaveAnImageToShow))
				vSubFunction2ButtonEnable = true;	//m_SubFunction2Button.EnableWindow(true);
			else
				vSubFunction2ButtonEnable = false;	//m_SubFunction2Button.EnableWindow(false);
			TempText.LoadString(IDS_SaveImageToFile);
			m_SubFunction2Button.ShowWindow(SW_SHOW);
			SetDlgItemText(IDC_SubFunction2Button,TempText);
		}
		else
		if (PasswordOK(cSuperInspxPassword,false))
		{
			m_SubFunction2Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
			if ((vHaveAnImageToShow) && (!vShowAlternateImage))				
				vSubFunction2ButtonEnable = true;		//m_SubFunction2Button.EnableWindow(true);
			else				
				vSubFunction2ButtonEnable = false;		//m_SubFunction2Button.EnableWindow(false);
			m_SubFunction2Button.ShowWindow(SW_SHOW);
			SetDlgItemText(IDC_SubFunction2Button, _T("Show\nPrevious\nStep"));
		}
		else
		{
			m_SubFunction2Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_SubFunction2Button.GetRectToInvalidateArrow());
		}
	}

	//Subfunction3Button
	if (vEditROIBoundaries)
	{		
		vSubFunction3ButtonEnable = true;	//m_SubFunction3Button.EnableWindow(true);
		TempText = vGlobalBottomString;
		SetDlgItemText(IDC_SubFunction3Button,TempText);
		m_SubFunction3Button.ShowWindow(SW_SHOW);
	}
	else
	if ((vShowThresholdMenu) && (vTweakingThreshold))
	{		
		vSubFunction3ButtonEnable = true;	//m_SubFunction3Button.EnableWindow(true);
		TempText.LoadString(IDS_ShowThreshold);
		m_SubFunction3Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		SetDlgItemText(IDC_SubFunction3Button,TempText + " -1");
		m_SubFunction3Button.ShowWindow(SW_SHOW);
		m_SubFunction3Button.ShowNavigationArrow(NULL);
	}
	else
	if (vRejectsImageDisplay)
	{
		if (HaveAnInspection(3 + vFirstInspectionToView))
		{			
			vSubFunction3ButtonEnable = true;	//m_SubFunction3Button.EnableWindow(true);
			m_SubFunction3Button.ShowWindow(SW_SHOW);
			CString TempName = GetInspectionName(3 + vFirstInspectionToView);
			if (TempName.GetLength() < 10)
				m_SubFunction3Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
			else
				m_SubFunction3Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
			SetDlgItemText(IDC_SubFunction3Button,TempName);
		}
		else
		{
			m_SubFunction3Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_SubFunction3Button.GetRectToInvalidateArrow());
		}
	}
	else
	{
		if (vImproveLearnMenu)
		{
			m_SubFunction3Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_SubFunction3Button.GetRectToInvalidateArrow());
		}
		else
		if (vEditWhileRunning)// || ((vImproveLearnMenu) && (PasswordOK(cTemporaryInspxPassword,false))))
		{
			if ((PasswordOK(cSuperInspxPassword,false)) && (vInspectionShowing) && (vInspectionShowing->vHasAMinimumSize))
			{
				m_SubFunction3Button.ShowWindow(SW_SHOW);
				vSubFunction3ButtonEnable = true;	//m_SubFunction3Button.EnableWindow(true);
				m_SubFunction3Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
				SetDlgItemText(IDC_SubFunction3Button, _T("Edit Inspection Settings"));
			}
			else
			if ((vInspectionShowing) && ((vInspectionShowing->vInspectionType == cItemCountInspection) || (vInspectionShowing->vInspectionType == cSizeInspection) || (vInspectionShowing->vInspectionType == cMissingLidInspectionOnSize)))
			{
				m_SubFunction3Button.ShowWindow(SW_SHOW);
				vSubFunction3ButtonEnable = true;	//m_SubFunction3Button.EnableWindow(true);
				m_SubFunction3Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
				SetDlgItemText(IDC_SubFunction3Button, _T("Size"));
			}
			else
			if ((vInspectionShowing) && (vInspectionShowing->vInspectionType == cFloodedInspection))
			{
				m_SubFunction3Display.ShowWindow(SW_SHOW);
				m_SubFunction3Button.ShowWindow(SW_SHOW);
				vSubFunction3ButtonEnable = true;	//m_SubFunction3Button.EnableWindow(true);
				m_SubFunction3Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
				SetDlgItemText(IDC_SubFunction3Button, _T("Minimum Spout Length"));
			}
			else
			if ((vInspectionShowing) && (vInspectionShowing->vHasAMinimumSize) && (PasswordOK(cAdvancedSetupPassword,false)))
			{
				m_SubFunction3Display.ShowWindow(SW_SHOW);
				m_SubFunction3Button.ShowWindow(SW_SHOW);
				vSubFunction3ButtonEnable = true;	//m_SubFunction3Button.EnableWindow(true);
				m_SubFunction3Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
				TempText.LoadString(IDS_MinimumDefectSize);
				SetDlgItemText(IDC_SubFunction3Button,TempText);
			}
			else
			{
				m_SubFunction3Display.ShowWindow(SW_HIDE);
				m_SubFunction3Button.ShowWindow(SW_HIDE);
				this->InvalidateRect(&m_SubFunction3Button.GetRectToInvalidateArrow());
			}
		}
		else
		if (vImageDisplayOptions)
		{
			m_SubFunction3Button.ShowWindow(SW_SHOW);
			m_SubFunction3Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
			if ((!vLocalSystemData->vLoadedFileAvailable) && (!vShowNextFrame))				
				vSubFunction3ButtonEnable = true;	//m_SubFunction3Button.EnableWindow(true);
			else				
				vSubFunction3ButtonEnable = false;	//m_SubFunction3Button.EnableWindow(false);
			TempText.LoadString(IDS_LoadImageFromFile);
			SetDlgItemText(IDC_SubFunction3Button,TempText);
		}
		else
		if (PasswordOK(cSuperInspxPassword,false))
		{
			m_SubFunction3Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
			if ((vHaveAnImageToShow) && (!vShowAlternateImage))				
				vSubFunction3ButtonEnable = true;	//m_SubFunction3Button.EnableWindow(true);
			else				
				vSubFunction3ButtonEnable = false;	//m_SubFunction3Button.EnableWindow(false);
			m_SubFunction3Button.ShowWindow(SW_SHOW);
			SetDlgItemText(IDC_SubFunction3Button, _T("Show\nNext\nStep"));
		}
		else
		{
			m_SubFunction3Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_SubFunction3Button.GetRectToInvalidateArrow());
		}
	}

	//Subfunction4Button
	if (vEditROIBoundaries)
	{		
		vSubFunction4ButtonEnable = true;	//m_SubFunction4Button.EnableWindow(true);
		TempText = vGlobalBottomString;
		SetDlgItemText(IDC_SubFunction4Button,TempText);
		m_SubFunction4Button.ShowWindow(SW_SHOW);
	}
	else
	if (vShowThresholdMenu)
	{		
		vSubFunction4ButtonEnable = true;	//m_SubFunction4Button.EnableWindow(true);
		TempText.LoadString(IDS_ShowThreshold);
		SetDlgItemText(IDC_SubFunction4Button,TempText + " -10");
		m_SubFunction4Button.ShowWindow(SW_SHOW);
		m_SubFunction4Button.ShowNavigationArrow(NULL);
	}
	else
	if (vRejectsImageDisplay)
	{
		if (HaveAnInspection(4 + vFirstInspectionToView))
		{			
			vSubFunction4ButtonEnable = true;	//m_SubFunction4Button.EnableWindow(true);
			m_SubFunction4Button.ShowWindow(SW_SHOW);
			CString TempName = GetInspectionName(4 + vFirstInspectionToView);
			if (TempName.GetLength() < 10)
				m_SubFunction4Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
			else
				m_SubFunction4Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
			SetDlgItemText(IDC_SubFunction4Button,TempName);
		}
		else
		{
			m_SubFunction4Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_SubFunction4Button.GetRectToInvalidateArrow());
		}
	}
	else
	if (vImageDisplayOptions)
	{
		m_SubFunction4Button.ShowNavigationArrow(NULL);
		if (PasswordOK(cTemporaryInspxPassword,false))
		{			
			vSubFunction4ButtonEnable = true;	//m_SubFunction4Button.EnableWindow(true);
			m_SubFunction4Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
			m_SubFunction4Button.ShowWindow(SW_SHOW);
			if (vLocalSystemData->vITIPCDig->vImageShowCrossHair == 1)
				SetDlgItemText(IDC_SubFunction4Button, _T("Show Top Detector Edges"));
			else
			if (vLocalSystemData->vITIPCDig->vImageShowCrossHair == 2)
				SetDlgItemText(IDC_SubFunction4Button, _T("Show Both Detector Edges"));
			else
			if (vLocalSystemData->vITIPCDig->vImageShowCrossHair == 3)
				SetDlgItemText(IDC_SubFunction4Button, _T("Clear Detector Edges"));
				//SetDlgItemText(IDC_SubFunction4Button,"Show Center Crosshair");
			else
			if (vLocalSystemData->vITIPCDig->vImageShowCrossHair == 4)
				SetDlgItemText(IDC_SubFunction4Button, _T("Hide Center Crosshair"));
			else
				SetDlgItemText(IDC_SubFunction4Button, _T("Show Bottom Det. Edges"));
		}
		else
		{
			m_SubFunction4Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_SubFunction4Button.GetRectToInvalidateArrow());
		}
	}
	else
	if ((vAnalysisImageDisplay) && (!PasswordOK(cSuperInspxPassword,false)) && 
		((vLocalConfigurationData->vScanTracType == cLegatoScanTrac) || (vLocalConfigurationData->vScanTracType == cFermataScanTrac)))//pipeline ScanTrac
	{		
		vSubFunction4ButtonEnable = true;	//m_SubFunction4Button.EnableWindow(true);
		m_SubFunction4Button.ShowWindow(SW_SHOW);
		m_SubFunction4Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		m_SubFunction4Display.ShowWindow(SW_HIDE);
		SetDlgItemText(IDC_SubFunction4Button, _T("Correct Light/Dark Pixels"));
	}
	else
	{
		m_SubFunction4Button.ShowNavigationArrow(NULL);
		if (vImproveLearnMenu)
		{			
			vSubFunction4ButtonEnable = true;	//m_SubFunction4Button.EnableWindow(true);
			m_SubFunction4Button.ShowWindow(SW_SHOW);
			m_SubFunction4Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
			m_SubFunction4Display.ShowWindow(SW_SHOW);
			if (vShowType == cShowTypeContaminant2NoDilation)
			{
				SetDlgItemText(IDC_SubFunction4Button, _T("Show Contam Image"));
				SetDlgItemText(IDC_SubFunction4Display, _T("Contam2NoDilate"));
			}
			else
			if ((vShowType == cShowTypeContaminant2) && (vLocalSystemData->vITIPCDig->vProductContaminant2StructureImageWithNoDilation))
			{
				SetDlgItemText(IDC_SubFunction4Button, _T("Show Contam2 No Dilate"));
				SetDlgItemText(IDC_SubFunction4Display, _T("Contaminant2"));
			}
			else
			if (vShowType == cShowTypeContaminant2)
			{
				SetDlgItemText(IDC_SubFunction4Button, _T("Show Contam Image"));
				SetDlgItemText(IDC_SubFunction4Display, _T("Contaminant2"));
			}
			else
			{
				SetDlgItemText(IDC_SubFunction4Button, _T("Show Contam2 Image"));
				SetDlgItemText(IDC_SubFunction4Display, _T("Contaminant"));
			}
		}
		else
		if (vEditWhileRunning)// || ((vImproveLearnMenu) && (PasswordOK(cTemporaryInspxPassword,false))))
		{			
			vSubFunction4ButtonEnable = true;	//m_SubFunction4Button.EnableWindow(true);
			m_SubFunction4Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
			if (vInspectionShowing)				
				vSubFunction4ButtonEnable = true;	//m_SubFunction4Button.EnableWindow(true);
			else				
				vSubFunction4ButtonEnable = false;	//m_SubFunction4Button.EnableWindow(false);
			m_SubFunction4Button.ShowWindow(SW_SHOW);
			TempText.LoadString(IDS_ChangeRejectThreshold);
			SetDlgItemText(IDC_SubFunction4Button,TempText);
		}
		else
		{
			m_SubFunction4Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
			ShowNextDataButton(IDC_SubFunction4Button, &m_SubFunction4Button);
		}
	}

	//Subfunction5Button
	if (vImproveLearnMenu)
	{
		
		vSubFunction5ButtonEnable = true;	//m_SubFunction5Button.EnableWindow(true);
		m_SubFunction5Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		if (vLocalSystemData->vITIPCDig->vImproveLearnCount)			
			vSubFunction5ButtonEnable = true; //m_SubFunction5Button.EnableWindow(true);
		else			
			vSubFunction5ButtonEnable = false;	//m_SubFunction5Button.EnableWindow(false);
		SetDlgItemText(IDC_SubFunction5Button, _T("Add Improve Learn!"));
		m_SubFunction5Button.ShowWindow(SW_SHOW);
	}
	else
	if (vEditROIBoundaries)
	{		
		vSubFunction5ButtonEnable = true;	//m_SubFunction5Button.EnableWindow(true);
		TempText.LoadString(IDS_Left);
		SetDlgItemText(IDC_SubFunction5Button,TempText);
		m_SubFunction5Button.ShowWindow(SW_SHOW);
	}
	else
	if (vShowThresholdMenu)
	{
		if ((vInspectionShowing) && 
			((vInspectionShowing->vInspectionType == cCheckWeighInspection) ||
			(vInspectionShowing->vInspectionType == cDensityInspection) ||
			(vInspectionShowing->vInspectionType == cStandardDeviationInspection)))
		{			
			vSubFunction5ButtonEnable = true;	//m_SubFunction5Button.EnableWindow(true);
			m_SubFunction5Button.ShowWindow(SW_SHOW);
			m_SubFunction5Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
			if (vShowingUpperThreshold)
				TempText.LoadString(IDS_SelectLowerThreshold);
			else
				TempText.LoadString(IDS_SelectUpperThreshold);
			SetDlgItemText(IDC_SubFunction5Button,TempText);
		}
		else
		{
			m_SubFunction5Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_SubFunction5Button.GetRectToInvalidateArrow());
		}
	}
	else
	if (vRejectsImageDisplay)
	{
		if (HaveAnInspection(5 + vFirstInspectionToView))
		{			
			vSubFunction5ButtonEnable = true;	//m_SubFunction5Button.EnableWindow(true);
			m_SubFunction5Button.ShowWindow(SW_SHOW);
			CString TempName = GetInspectionName(5 + vFirstInspectionToView);
			if (TempName.GetLength() < 10)
				m_SubFunction5Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
			else
				m_SubFunction5Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
			SetDlgItemText(IDC_SubFunction5Button,TempName);
		}
		else
		{
			m_SubFunction5Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_SubFunction5Button.GetRectToInvalidateArrow());
		}
	}
	else
	if (vEditWhileRunning)
	{
		m_SubFunction5Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		vSubFunction5ButtonEnable = true;	//m_SubFunction5Button.EnableWindow(true);
		m_SubFunction5Button.ShowWindow(SW_SHOW);
		SetDlgItemText(IDC_SubFunction5Button,"Re-Learn " + vLocalSystemData->vContainerString + " Menu");
	}
	else
	if (vImageDisplayOptions) 
	{
		m_SubFunction5Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		vSubFunction5ButtonEnable = true;	//m_SubFunction5Button.EnableWindow(true);
		m_SubFunction5Button.ShowWindow(SW_SHOW);
		if (vLocalConfigurationData->vImageShowFixedROILines)
			TempText.LoadString(IDS_ClearFixedROILines);
		else
			TempText.LoadString(IDS_ShowFixedROILines);
		SetDlgItemText(IDC_SubFunction5Button,TempText);
	}
	else
	{
		m_SubFunction5Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		vSubFunction5ButtonEnable = true;	//m_SubFunction5Button.EnableWindow(true);
		m_SubFunction5Button.ShowWindow(SW_SHOW);
		TempText.LoadString(IDS_ImageDisplayOptions);
		SetDlgItemText(IDC_SubFunction5Button,TempText);
	}

	//Subfunction6Button
	if (vImproveLearnMenu)
	{
		m_SubFunction6Button.ShowWindow(SW_SHOW);
		if ((vLocalSystemData->vITIPCDig->HaveLearnImageFile("=Process 2o")) && (vLocalSystemData->vITIPCDig->vProductStructureImage))
		{
			if (vShowDeltaOriginalToCurrentStructure)
				vSubFunction6ButtonEnable = false; //m_SubFunction6Button.EnableWindow(false);
			else				
				vSubFunction6ButtonEnable = true;	//m_SubFunction6Button.EnableWindow(true);
		}
		else
			vSubFunction6ButtonEnable = false;	//m_SubFunction6Button.EnableWindow(false);
		SetDlgItemText(IDC_SubFunction6Button, _T("Delta Original to Current"));
	}
	else
	if (vEditROIBoundaries)
	{
		vSubFunction6ButtonEnable = true;	//m_SubFunction6Button.EnableWindow(true);
		TempText.LoadString(IDS_Left);
		SetDlgItemText(IDC_SubFunction6Button,TempText);
		m_SubFunction6Button.ShowWindow(SW_SHOW);
	}
	else
	if (vShowThresholdMenu)
	{
		vSubFunction6ButtonEnable = true;	//m_SubFunction6Button.EnableWindow(true);
		CString TempText;
		TempText.LoadString(IDS_EnterThresholdValue);
		SetDlgItemText(IDC_SubFunction6Button,TempText);
		m_SubFunction6Button.ShowWindow(SW_SHOW);
		//m_SubFunction6Button.ShowWindow(SW_HIDE);
		//m_ArrowRight2.ShowWindow(SW_HIDE);
	}
	else
	if (vRejectsImageDisplay)
	{
		if (HaveAnInspection(6 + vFirstInspectionToView))
		{
			vSubFunction6ButtonEnable = true;		//m_SubFunction6Button.EnableWindow(true);
			m_SubFunction6Button.ShowWindow(SW_SHOW);
			CString TempName = GetInspectionName(6 + vFirstInspectionToView);
			if (TempName.GetLength() < 10)
				m_SubFunction6Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
			else
				m_SubFunction6Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
			SetDlgItemText(IDC_SubFunction6Button,TempName);
		}
		else
		{
			m_SubFunction6Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_SubFunction6Button.GetRectToInvalidateArrow());
		}
	}
	else
	if (vEditWhileRunning)
	{
		m_SubFunction6Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		m_SubFunction6Button.ShowWindow(SW_SHOW);
		TempText.LoadString(IDS_InspectionEditWhileRunning);
		SetDlgItemText(IDC_SubFunction6Button,TempText);
	}
	else
	if (vImageDisplayOptions)
	{
		m_SubFunction6Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		vSubFunction6ButtonEnable = true;	//m_SubFunction6Button.EnableWindow(true);
		if (vLocalConfigurationData->vImageShowScale)
			TempText.LoadString(IDS_ClearScaleLines);
		else
			TempText.LoadString(IDS_ShowScaleLines);
		m_SubFunction6Button.ShowWindow(SW_SHOW);
		SetDlgItemText(IDC_SubFunction6Button,TempText);
	}
	else
	{
		m_SubFunction6Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		m_SubFunction6Button.ShowWindow(SW_SHOW);
		TempText.LoadString(IDS_EditWhileRunning);
		SetDlgItemText(IDC_SubFunction6Button,TempText);
	}

	//Subfunction7Button
	if (vImproveLearnMenu)
	{
		if ((vLocalSystemData->vITIPCDig->vProductStructureImage) && (vLocalSystemData->vITIPCDig->vImproveLearnCount))
		{
			if (vShowDeltaCurrentPreviewStructure)				
				vSubFunction7ButtonEnable = false;	//m_SubFunction7Button.EnableWindow(false);
			else				
				vSubFunction7ButtonEnable = true;	//m_SubFunction7Button.EnableWindow(true);
		}
		else			
			vSubFunction7ButtonEnable = false;	//m_SubFunction7Button.EnableWindow(false);
		SetDlgItemText(IDC_SubFunction7Button, _T("Delta Current to Improve"));
		m_SubFunction7Button.ShowWindow(SW_SHOW);
	}
	else
	if (vShowThresholdMenu)
	{
		vSubFunction7ButtonEnable = true;	//m_SubFunction7Button.EnableWindow(true);
		TempText.LoadString(IDS_SaveThreshold);
		SetDlgItemText(IDC_SubFunction7Button,TempText);
		m_SubFunction7Button.ShowWindow(SW_SHOW);
	}
	else
	if (vEditROIBoundaries)
	{
		vSubFunction7ButtonEnable = true;	//m_SubFunction7Button.EnableWindow(true);
		SetDlgItemText(IDC_SubFunction7Button, _T("Right\n\n"));
		TempText.LoadString(IDS_Right);
		SetDlgItemText(IDC_SubFunction7Button,TempText);
		m_SubFunction7Button.ShowWindow(SW_SHOW);
	}
	else
	if (vShowThresholdMenu)
	{
		m_SubFunction7Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction7Button.GetRectToInvalidateArrow());
	}
	else
	if (vRejectsImageDisplay)
	{
		BYTE TempNumberOfInspectionsToView = vTotalNumberOfInspectionsToDisplay;
		if (PasswordOK(cTemporaryInspxPassword,false))
			TempNumberOfInspectionsToView++;  //include good containers
		if (TempNumberOfInspectionsToView > 7)
		{  //More Filters Button was pressed
			vSubFunction7ButtonEnable = true;	//m_SubFunction7Button.EnableWindow(true);
			m_SubFunction7Button.ShowWindow(SW_SHOW);
			m_SubFunction7Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
			if ((vFirstInspectionToView < 12) && (vFirstInspectionToView + 6 < TempNumberOfInspectionsToView))
			{
				TempText.LoadString(IDS_ShowNextSetofFilters);
				m_SubFunction7Button.ShowWindow(SW_SHOW);
				SetDlgItemText(IDC_SubFunction7Button,TempText);
			}
			else
			{
				TempText.LoadString(IDS_ShowFirstSetofFilters);
				SetDlgItemText(IDC_SubFunction7Button,TempText);
			}
		}
		else
		if (HaveAnInspection(7 + vFirstInspectionToView))
		{			
			vSubFunction7ButtonEnable = true;	//m_SubFunction7Button.EnableWindow(true);
			m_SubFunction7Button.ShowWindow(SW_SHOW);
			CString TempName = GetInspectionName(7 + vFirstInspectionToView);
			if (TempName.GetLength() < 10)
				m_SubFunction7Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
			else
				m_SubFunction7Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
			SetDlgItemText(IDC_SubFunction7Button,TempName);
		}
		else
		{
			m_SubFunction7Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_SubFunction7Button.GetRectToInvalidateArrow());
		}
	}
	else
	if (vEditWhileRunning)
	{
		if (PasswordOK(cTemporaryInspxPassword,false))
		{
			m_SubFunction7Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
			vSubFunction7ButtonEnable = true;	//m_SubFunction7Button.EnableWindow(true);
			m_SubFunction7Button.ShowWindow(SW_SHOW);
			TempText.LoadString(IDS_UndoLearnMenu);
			SetDlgItemText(IDC_SubFunction7Button,TempText);
		}
		else
		if (vLocalSystemData->vITIPCDig->HaveUndoAddToLearnImage())
		{
			m_SubFunction7Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
			vSubFunction7ButtonEnable = true;	//m_SubFunction7Button.EnableWindow(true);
			m_SubFunction7Button.ShowWindow(SW_SHOW);
			TempText.LoadString(IDS_UndoLastAddToLearn);
			SetDlgItemText(IDC_SubFunction7Button,TempText + "!");
		}
		else
		{
			m_SubFunction7Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_SubFunction7Button.GetRectToInvalidateArrow());
		}
	}
	else
	if (vImageDisplayOptions)
	{
		m_SubFunction7Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);		
		vSubFunction7ButtonEnable = true;	//m_SubFunction7Button.EnableWindow(true);
		m_SubFunction7Button.ShowWindow(SW_SHOW);
		if (vLocalConfigurationData->vImageShowLines)
			TempText.LoadString(IDS_ClearFoundBoundary);
		else
			TempText.LoadString(IDS_ShowFoundBoundary);
		SetDlgItemText(IDC_SubFunction7Button,TempText);
	}
	else
	{
		//if (((vLocalSystemData->vITIPCDig->vRejectBufferShowing >= 0) || 
		if ((vDisplayingImageData) && (vDisplayingImageData->vContainerEjectReason < cMaximumRejectReason) &&
			(!vShowingLiveImages))
		{
			if ((vDisplayingImageData->vContainerEjectInspection == 255) ||
				((vDisplayingImageData->vContainerEjectInspection < vGlobalCurrentProduct->vNumberOfInspections) &&
				(!vGlobalCurrentProduct->vInspection[vDisplayingImageData->vContainerEjectInspection]->vAddStructure)))
				vSubFunction7ButtonEnable = false;	//m_SubFunction7Button.EnableWindow(false);
			else
				vSubFunction7ButtonEnable = true;	//m_SubFunction7Button.EnableWindow(true);

			m_SubFunction7Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
			m_SubFunction7Button.ShowWindow(SW_SHOW);
			if ((vGlobalShiftKeyDown) && (PasswordOK(cTemporaryInspxPassword,false)) && (vLocalSystemData->vITIPCDig->HaveUndoAddToLearnImage())) //undo last add to learn if shift key down
				TempText.LoadString(IDS_UndoLastAddToLearn);
			else
				TempText.LoadString(IDS_AddRejecttoLearn);
			SetDlgItemText(IDC_SubFunction7Button,TempText);
		}
		else
		{
			//m_SubFunction7Button.ShowWindow(SW_HIDE);
			//m_ArrowRight3.ShowWindow(SW_HIDE);			
			vSubFunction7ButtonEnable = false;	//m_SubFunction7Button.EnableWindow(false);
			m_SubFunction7Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
			m_SubFunction7Button.ShowWindow(SW_SHOW);
			TempText.LoadString(IDS_AddRejecttoLearn);
			SetDlgItemText(IDC_SubFunction7Button,TempText);
		}
	}

	if (vEditROIBoundaries)
	{
		vSubFunction8ButtonEnable = true;	//m_SubFunction8Button.EnableWindow(true);
		TempText.LoadString(IDS_Right);
		SetDlgItemText(IDC_SubFunction8Button,TempText);
		m_SubFunction8Button.ShowWindow(SW_SHOW);
	}
	else
	{
		if ((vShowingProcess) && (!vShowThreshold))
		{
			m_SubFunction8Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_SubFunction8Button.GetRectToInvalidateArrow());
		}
		else
		{
			m_SubFunction8Button.ShowWindow(SW_SHOW);
			vSubFunction8ButtonEnable = true;	//m_SubFunction8Button.EnableWindow(true);
			//enum {cDefectsInNoColor, cDefectsInPseudoColor, cDefectsInRed, cDefectInRedBox, cDefectsInRedBoxes};

			m_SubFunction8Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
			//if ((vGlobalShiftKeyDown) && (PasswordOK(cTemporaryInspxPassword,false)))
			//{
			//	if (vLocalConfigurationData->vDefectDisplayMode == cDefectsInPseudoColor)
			//		TempText.LoadString(IDS_DefectsInRedWithCarriageReturns);
			//	else
			//	if ((vLocalConfigurationData->vDefectDisplayMode >= cDefectsInRed) && (vShowThreshold))
			//		TempText.LoadString(IDS_DefectsInColorWithCarriageReturns);
			//	else
			//	if (vLocalConfigurationData->vDefectDisplayMode == cDefectsInRed)
			//		TempText.LoadString(IDS_DefectsInRedBoxWithCarriageReturns);
			//	else
			//	if (vLocalConfigurationData->vDefectDisplayMode == cDefectInRedBox)
			//		TempText.LoadString(IDS_DefectsInRedBoxesWithCarriageReturns);
			//	else
			//		TempText.LoadString(IDS_DefectsInColorWithCarriageReturns);
			//}
			//else
			{
				if (vHideRedIndicator)
					TempText.LoadString(IDS_ShowColor);
				else
				{
					TempText.LoadString(IDS_HideColor);
				}
			}
			SetDlgItemText(IDC_SubFunction8Button,TempText);
		}
	}
}

void CNewImageDisplayDialog::UpdateDisplay(CImageData *TempDisplayImage)
{
	// Update ImageStatus Display
	CString TempText("");
	if ((vShowThresholdMenu) && (vInspectionShowing))
	{
		CString TempString1("");
		//if (vDisplayingImageData)
			//if (vDisplayingImageData->vNegitiveThresholdCorrection)
				//TempString1 = "*";
		TempText.LoadString(IDS_DisplayedThreshold);
		if ((vInspectionShowing) && (vInspectionShowing->vInspectionType == cUnderfillByWeightInspection))
		{
			TempText = "Min. Acceptable Weight";
			SetDlgItemText(IDC_ImageStatus,TempText + ":" + dtoa(vInspectionShowing->ConvertToWeight(vThreshold),2) + TempString1);
		}
		else
		if ((vInspectionShowing) && (vInspectionShowing->vInspectionType == cOverfillByWeightInspection))
		{
			TempText = "Max. Acceptable Weight";
			SetDlgItemText(IDC_ImageStatus,TempText + ":" + dtoa(vInspectionShowing->ConvertToWeight(vThreshold),2) + TempString1);
		}
		else
		if ((vInspectionShowing->vInspectionType == cCheckWeighInspection) ||
			(vInspectionShowing->vInspectionType == cDensityInspection) ||
			(vInspectionShowing->vInspectionType == cStandardDeviationInspection))
		{
			if (vShowingUpperThreshold)
				TempText.LoadString(IDS_DisplayedUThreshold);
			else
				TempText.LoadString(IDS_DisplayedLThreshold);
			SetDlgItemText(IDC_ImageStatus,TempText + ":" + dtoa(vInspectionShowing->ConvertToWeight(vThreshold),2) + TempString1);
		}
		else
			SetDlgItemText(IDC_ImageStatus,TempText + ":" + dtoa(vThreshold,2) + TempString1);
	}
	else
	{
		CString TempString("");
		TempString = "";
		if (vSaveImagesToFiles)
		if (vSaveImagesToFilesName != "")
		{
			BYTE TempLength = TempString.GetLength();
			if (TempLength)
				TempString = TempString + " ";
			TempString = TempString + "Save Image: " + dtoa(vSaveImagesToFiles - 1,0);
		}
		if ((vHaveAnImageToShow) || (vShowAlternateImage))
		{
			if (vOldTakingQuickLearnImages)
			{
				TempText.LoadString(IDS_QLearn);
				TempString = TempText + ":" + 
					dtoa(vLocalSystemData->vQuickLearnImageIndex,0) +
					"/" + dtoa(vLocalConfigurationData->vNumberOfQuickLearnImagesToTake,0);
			}
			else
			if (vOldCalibrateBodyTriggerReferenceEnabled)
			{
				TempText.LoadString(IDS_Learn);
				TempString = TempText + ":" + 
					dtoa(vLocalSystemData->vITIPCDig->vCalibrateBodyTriggerReferenceCount,0) + "/" + dtoa(vLocalConfigurationData->vCalibrateBodyTriggerReferenceDisableCount +
					vLocalConfigurationData->vAverageDisableCount + vLocalConfigurationData->vStructureDisableCount,0);
			}
			else
			if (vOldAverageEnabled)
			{
				TempText.LoadString(IDS_Learn);
				TempString = TempText + ":" + dtoa(vLocalConfigurationData->vCalibrateBodyTriggerReferenceDisableCount + vLocalSystemData->vITIPCDig->vAverageCount,0) +
					"/" + dtoa(vLocalConfigurationData->vCalibrateBodyTriggerReferenceDisableCount + vLocalConfigurationData->vAverageDisableCount + vLocalConfigurationData->vStructureDisableCount,0);
			}
			else
			if (vOldStructureEnabled)
			{
				TempText.LoadString(IDS_Learn);
				TempString = TempText + ":" + dtoa(vLocalConfigurationData->vCalibrateBodyTriggerReferenceDisableCount + vLocalSystemData->vITIPCDig->vStructureCount + vLocalConfigurationData->vAverageDisableCount,0) +
					"/" + dtoa(vLocalConfigurationData->vCalibrateBodyTriggerReferenceDisableCount + vLocalConfigurationData->vAverageDisableCount + vLocalConfigurationData->vStructureDisableCount,0);
			}
			if (!vShowAlternateImage)
			if (TempDisplayImage)
			if ((TempDisplayImage->vContainerEjected) || (TempDisplayImage->vContainerEjectReason < cMaximumRejectReason))
			{
				BYTE TempLength = TempString.GetLength();
				if (TempLength)
					TempString = TempString + " ";
				if (vLocalSystemData->vITIPCDig->vRejectBufferShowing != -1)
				{
					TempText.LoadString(IDS_Rej);
					TempString = TempString + TempText + ": " + dtoaWithCommas(vLocalSystemData->vITIPCDig->vRejectBufferShowing + 1,0);

				}
				else
				if (TempDisplayImage->vContainerEjected)
				{
					TempText.LoadString(IDS_Ejected);
					TempString = TempString + " " + TempText;
				}
				else
				{
					if ((TempDisplayImage->vContainerEjectReason == cPassSlippedContainer) ||
						(TempDisplayImage->vContainerEjectReason == cPassContainerWrongWidth) ||
						(TempDisplayImage->vContainerEjectReason == cPassContainerOnTop) ||
						(TempDisplayImage->vContainerEjectReason == cPassContainerWrongHeight) ||
						(TempDisplayImage->vContainerEjectReason == cPassContainerOnConveyorStop))
					{
						TempText.LoadString(IDS_Passed);
						TempString = TempString + " " + TempText;
					}
					else
					if (vLocalSystemData->vInAutoSetup)
					{
						TempText.LoadString(IDS_InAutoSetup);
						TempString = TempString + " " + TempText;
					}
					else
					if (TempDisplayImage->vRejectDisabled)
					{
						TempText.LoadString(IDS_Disabled);
						TempString = TempString + " " + TempText;
					}
					else
					if ((TempDisplayImage->vLeft <= 1) || (TempDisplayImage->vLeft >= vLocalSystemData->vITIPCDig->vOriginalBufferSizeY - 1))
					{
						TempText.LoadString(IDS_Passed);
						TempString = TempString + "  Slipped";
					}
					else
					{
						TempText.LoadString(IDS_InAutoSetup);
						TempString = TempString + " " + TempText;
					}
				}
				//1/10/2006
				if (vNumberOfRejectsInLocalBuffer)
					TempString = TempString + " of " + dtoaWithCommas(vNumberOfRejectsInLocalBuffer,0);
			}
			else
			{
				BYTE TempLength = TempString.GetLength();
				if (TempLength)
					TempString = TempString + " ";
				TempText.LoadString(IDS_ContainerOK);
				TempString = TempString + TempText;
			}
			if (TempDisplayImage)
			{
				//5/7/2011
				if (TempDisplayImage->vImageTypeString.GetLength() > 3)
				if (TempString.GetLength() + TempDisplayImage->vImageTypeString.GetLength() < 32)
					TempString = TempString + " " + TempDisplayImage->vImageTypeString;

				if (!vShowAlternateImage)				
				//if (TempString.GetLength() + TempDisplayImage->vTime.GetLength() < 32)
					TempString = TempString + " " + TempDisplayImage->vTime;
			}
		}

		SetDlgItemText(IDC_ImageStatus,TempString);
	}

	//Update CurrentInspection display
	if (TempDisplayImage)
	{
		CString TempString("");
		TempString = "";
		//if rejected image and in rejects display, or in other display and 
		//inspection showing is NULL, or showing a different inspection, then
		//show the reason this container was ejected
		if (!vImproveLearnMenu)
		{
			/*
		if ((vInspectionShowing) && (!vRejectsImageDisplay) && (vInspectionNumberShowing != 
			TempDisplayImage->vContainerEjectReason - cEjectInspectionOffset))
		{
			CString TempName = vInspectionShowing->vName;
			if (TempName.GetLength() > 25)
				SetDlgItemText(IDC_CurrentInspection,TempName);
			else
			{
				TempText.LoadString(IDS_Viewing);
				SetDlgItemText(IDC_CurrentInspection,TempText + ": " + TempName);
			}
		}
		else
		if (TempDisplayImage->vContainerEjectReason < cMaximumRejectReason)// && 
			//(vRejectsImageDisplay)) || 
			//((!vRejectsImageDisplay) && 
			//(TempDisplayImage->vContainerEjectReason < cMaximumRejectReason) &&
			//((vInspectionShowing) &&
			//(TempDisplayImage->vContainerEjectInspection <
			//	vGlobalCurrentProduct->vNumberOfInspections) &&
			//(vInspectionShowing == vGlobalCurrentProduct->vInspection[
			//TempDisplayImage->vContainerEjectInspection]))))
		{
			CString TempName = 
				vLocalSystemData->vCriteriaString[vLocalSystemData->vCountToUseForEjectReason[
				TempDisplayImage->vContainerEjectReason]];
			if (TempName.GetLength() > 25)
				SetDlgItemText(IDC_CurrentInspection,TempName);
			else
			if (TempDisplayImage->vContainerEjected)
			{
				TempText.LoadString(IDS_Reject);
				SetDlgItemText(IDC_CurrentInspection,TempText + ": " + TempName);
			}
			else
			if ((TempDisplayImage->vContainerEjectReason == cPassSlippedContainer) ||
				(TempDisplayImage->vContainerEjectReason == cPassContainerWrongWidth) ||
				(TempDisplayImage->vContainerEjectReason == cPassContainerOnTop) ||
				(TempDisplayImage->vContainerEjectReason == cPassContainerWrongHeight) ||
						(TempDisplayImage->vContainerEjectReason == cPassContainerOnConveyorStop))
			{
				TempText.LoadString(IDS_Passed);
				SetDlgItemText(IDC_CurrentInspection,TempText + ": " + TempName);
			}
			else
			if (vLocalSystemData->vInAutoSetup)
			{
				TempText.LoadString(IDS_AutoSetup);
				SetDlgItemText(IDC_CurrentInspection,TempText + ": " + TempName);
			}
			else
			{
				TempText.LoadString(IDS_Disabled);
				SetDlgItemText(IDC_CurrentInspection,TempText + ": " + TempName);
			}
		}
		else
			SetDlgItemText(IDC_CurrentInspection,"");
			*/
//const BYTE cEjectSlippedContainer = 16;
//const BYTE cPassSlippedContainer = 17;
//const BYTE cEjectContainerWrongWidth = 18;
//const BYTE cPassContainerWrongWidth = 19;
//const BYTE cEjectContainerOnTop = 20;
//const BYTE cPassContainerOnTop = 21;
//const BYTE cEjectContainerWrongHeight = 22;
//const BYTE cPassContainerWrongHeight = 23;
//const BYTE cEjectContainerOnConveyorStop = 24;
//const BYTE cPassContainerOnConveyorStop = 25;
//const BYTE cEjectEmptySlippedContainer = 26;
//const BYTE cEjectAuxilaryDetectorOffset = 32;
//const BYTE cTestEject = 35;
//const BYTE cEjectInspectionOffset = 1;
//const BYTE cMaximumRejectReason = 36;

			if (TempDisplayImage->vContainerEjectReason < cMaximumRejectReason)
			{
				//if container has an eject reason, list it
				CString TempName = vLocalSystemData->vCriteriaString[vLocalSystemData->vCountToUseForEjectReason[TempDisplayImage->vContainerEjectReason]];

				if ((TempDisplayImage->vContainerEjectReason <= vGlobalCurrentProduct->vNumberOfInspections) &&
					(vGlobalCurrentProduct->vInspection[TempDisplayImage->vContainerEjectReason - 1]->vInspectionType == cUnderfillByWeightInspection))
					TempName = TempName + " " + dtoa(TempDisplayImage->vFillLevel, 1) + " " + vGlobalCurrentProduct->vInspection[TempDisplayImage->vContainerEjectReason - 1]->vCheckWeighUnits;

				if (TempName.GetLength() == 0)
					TempName = "ER: " + dtoa(TempDisplayImage->vContainerEjectReason, 0);

				if ((TempDisplayImage->vContainerEjectReason == cPassSlippedContainer) ||
					(TempDisplayImage->vContainerEjectReason == cPassContainerWrongWidth) ||
					(TempDisplayImage->vContainerEjectReason == cPassContainerOnTop) ||
					(TempDisplayImage->vContainerEjectReason == cPassContainerWrongHeight) ||
					(TempDisplayImage->vContainerEjectReason == cPassContainerOnConveyorStop))
				{
					TempString.LoadString(IDS_Passed);

					if (TempDisplayImage->vContainerEjectReason == cPassContainerWrongWidth)
						TempName = "Wrong Width";
					if (TempDisplayImage->vContainerEjectReason == cPassSlippedContainer)
						TempName = "Slipped";
					if (TempDisplayImage->vContainerEjectReason == cPassContainerOnTop)
						TempName = "Touching Top";
					if (TempDisplayImage->vContainerEjectReason == cPassContainerWrongHeight)
						TempName = "Wrong Height";
					if (TempDisplayImage->vContainerEjectReason == cPassContainerOnConveyorStop)
						TempName = "Conveyor Stop";
				}
				else
				if (TempDisplayImage->vContainerEjected)
				{
					TempString.LoadString(IDS_Ejected);

					if (TempDisplayImage->vContainerEjectReason == cEjectContainerWrongWidth)
						TempName = "Wrong Width";
					if (TempDisplayImage->vContainerEjectReason == cEjectSlippedContainer)
						TempName = "Slipped";
					if (TempDisplayImage->vContainerEjectReason == cEjectContainerOnTop)
						TempName = "Touching Top";
					if (TempDisplayImage->vContainerEjectReason == cEjectContainerWrongHeight)
						TempName = "Wrong Height";
					if (TempDisplayImage->vContainerEjectReason == cEjectContainerOnConveyorStop)
						TempName = "Conveyor Stop";
					if (TempDisplayImage->vContainerEjectReason == cEjectEmptySlippedContainer)
						TempName = "Empty";
					if (TempDisplayImage->vContainerEjectReason == cTestEject)
						TempName = "Test Eject";
				}
				else
				if (TempDisplayImage->vContainerEjectInspection < cMaximumRejectReason)
				if (vLocalSystemData->vInAutoSetup)
					TempString.LoadString(IDS_AutoSetup);
				else
					TempString.LoadString(IDS_Disabled);

				if (TempString.GetLength())
					TempString = TempString + ": " + TempName;
				else
					TempString = TempName;
			}
			//17 characters maximum per line, 2 lines
			//if the name is short enough, or have no name, and editing a different inpection, then show viewing that inspection
			if ((TempString.GetLength() < 18) || (TempDisplayImage->vContainerEjectReason > cMaximumRejectReason))
			if ((vInspectionShowing) && (!vRejectsImageDisplay) && 
				(vInspectionNumberShowing != TempDisplayImage->vContainerEjectReason - cEjectInspectionOffset))
			{
				if (TempString.GetLength())
					TempString = TempString + " ";

				CString TempName = vInspectionShowing->vName;
				//if have room put 'viewing' in
				if ((TempName.GetLength() < 26) && ((TempDisplayImage->vContainerEjectReason > cMaximumRejectReason) || (TempName.GetLength() < 9)))
					TempString = TempString + "Viewing: ";

				TempString = TempString + TempName;
			}
			SetDlgItemText(IDC_CurrentInspection,TempString);
		}
	}
}

void CNewImageDisplayDialog::WaitThenDrawImageOnScreen() 
{
	//if (vHaveAnImageToShow)
	if (!vWaitForScreenUpdate)
	{
		vWaitForScreenUpdate = true;
		int TimerResult = SetTimer(vWaitForScreenUpdateTimerHandle,50,NULL);
		if (!TimerResult)
			ReportErrorMessage("Error-Second Timer Failed",cEMailInspx,32000);
	}
}
/*
void CNewImageDisplayDialog::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your message handler code here and/or call default
	CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
}
*/
void CNewImageDisplayDialog::DrawImageOnScreen()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	vShowingInspectionNumberOfRedPixels = 0;
	vSizeOfAllContaminants = 0;
	vDarkestValueInROI = 255;

	WORD TempOffsetX = (108 * vLocalSystemData->vGlobalMonitorSizeX + cOldResolutionX/2) / cOldResolutionX;
	WORD TempOffsetY = (43 * vLocalSystemData->vGlobalMonitorSizeY + cOldResolutionY/2) / cOldResolutionY;

	if (vNeedToDeleteDisplayingImageData)
	{
		vNeedToDeleteDisplayingImageData = false;
		if (vDisplayingImageData)
			delete vDisplayingImageData;
	}

	vDisplayingImageData = NULL;
	double TempStartTime = CIFCOS::GetSystimeMicrosecs();
	vShowAlternateImage = (((vShowAverage) && (vLocalSystemData->vITIPCDig->vProductAverageImage)) ||
		((vShowImproveLearnPreview) && (vLocalSystemData->vITIPCDig->vImproveLearnStructureImage)) ||
		((vShowOriginalStructure) && (vLocalSystemData->vITIPCDig->HaveLearnImageFile("=Process 2o"))) ||
		((vShowDeltaOriginalToCurrentStructure) && (vLocalSystemData->vITIPCDig->vProductStructureImage) && (vLocalSystemData->vITIPCDig->HaveLearnImageFile("=Process 2o"))) ||
		((vShowDeltaCurrentPreviewStructure) && (vLocalSystemData->vITIPCDig->vImproveLearnStructureImage) && (vLocalSystemData->vITIPCDig->vProductStructureImage)) ||

		((vShowHistogram) && (vLocalSystemData->vITIPCDig->vProductHistogramImage)) || ((vShowStructure) && (vLocalSystemData->vITIPCDig->vProductStructureImage)) ||
		((vShowStructure) && (vLocalConfigurationData->vAllowVoidInspection) && (vLocalSystemData->vITIPCDig->vStructureCount) && ((vLocalConfigurationData->vAllowVoidInspection) && 
		(vLocalSystemData->vITIPCDig->vVoidStructureImage))) ||
		((vShowStructure) && (vLocalConfigurationData->vAllowVoidInspection) && (vLocalConfigurationData->vAllowVoidInspection) && (vLocalSystemData->vITIPCDig->vVoidProductStructureImage)) ||
		(vShowBackground));
	if (vLocalSystemData->vITIPCDig->vOriginalImage)
	if ((vHaveAnImageToShow) || (vShowAlternateImage) || (vShowClipboard))
	{
		if (!vHaveAnImageToShow)
		{
			vDisplayChanges = true;
			if (vShowAlternateImage)
			{
				m_Background.ShowWindow(SW_HIDE);
				m_ErrorMessage.ShowWindow(SW_HIDE);
			}
		}
		if (ThereIsEnoughMemory((vLocalSystemData->vITIPCDig->vOriginalBufferSize + sizeof(Image)) * 2 +	sizeof(CImageData), "Temp Image In and Out"))
		{
			CImageData *TempDisplayImageData = NULL;
			Image *TempImageIn = NULL;
			TempImageIn = im_create(IM_BYTE, vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY);
			Image *TempImageOut = NULL;
			TempImageOut = im_create(IM_BYTE, vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY);

			if ((TempImageIn) && (TempImageOut) && (TempImageIn->pix) && (TempImageOut->pix))
			{
				if ((vShowClipboard) && (vLocalSystemData->vITIPCDig->vClipboardImage))
					vDisplayingImageData = vLocalSystemData->vITIPCDig->vClipboardImage;
				else
				if ((vShowAverage) && (vLocalSystemData->vITIPCDig->vAverageCount) && (vLocalSystemData->vITIPCDig->vAverageImage))
				{
					if (ThereIsEnoughMemory(sizeof(CImageData), "TempDisplayImageData"))
					{
						TempDisplayImageData = new CImageData;
						if (TempDisplayImageData)
						{
							vNeedToDeleteDisplayingImageData = true;
							TempDisplayImageData->MakeGreyImage(vLocalSystemData->vITIPCDig->vOriginalBufferSize);
							TempDisplayImageData->vImageTypeString = "Product Process 1 Acc";

							if (TempDisplayImageData->vGreyImage)
								vLocalSystemData->vITIPCDig->CopyAverageBuffer(vLocalSystemData->vITIPCDig->vAverageImage, TempDisplayImageData->vGreyImage, vLocalSystemData->vITIPCDig->vOriginalBufferSize);

							vDisplayingImageData = TempDisplayImageData;
						}
					}
				}
				else
				if (vShowBackground)
				{
					if ((vLocalSystemData->vITIPCDig->vProductBackgroundImage) && (vLocalSystemData->vITIPCDig->vProductBackgroundImage->pix))
					{
						if (ThereIsEnoughMemory(sizeof(CImageData), "TempDisplayImageData"))
						{
							TempDisplayImageData = new CImageData;
							if (TempDisplayImageData)
							{
								vNeedToDeleteDisplayingImageData = true;
								TempDisplayImageData->MakeGreyImage(vLocalSystemData->vITIPCDig->vOriginalBufferSize);

								if (TempDisplayImageData->vGreyImage)
									CopyMemory(
										TempDisplayImageData->vGreyImage,
										vLocalSystemData->vITIPCDig->vProductBackgroundImage->pix,
										vLocalSystemData->vITIPCDig->vOriginalBufferSize);

								TempDisplayImageData->vImageTypeString = "Product Process 0";
								vDisplayingImageData = TempDisplayImageData;
							}
						}
					}
					else
					if (ThereIsEnoughMemory(sizeof(CImageData), "TempDisplayImageData"))
					{
						if ((TempDisplayImageData) && (TempDisplayImageData->vGreyImage))
						{
							TempDisplayImageData = new CImageData;
							if (TempDisplayImageData)
							{
								vNeedToDeleteDisplayingImageData = true;
								TempDisplayImageData->MakeGreyImage(vLocalSystemData->vITIPCDig->vOriginalBufferSize);

								if (TempDisplayImageData->vGreyImage)
									GetBackgroundImage(TempDisplayImageData->vGreyImage);

								TempDisplayImageData->vImageTypeString = "Product Process 0";
								vDisplayingImageData = TempDisplayImageData;
							}
						}
					}
				}
				else
				if ((vShowAverage) && 
					(vLocalSystemData->vITIPCDig->vProductAverageImage))
					vDisplayingImageData = vLocalSystemData->vITIPCDig->vProductAverageImage;
				else
				if ((vShowHistogram) && (vLocalSystemData->vITIPCDig->vProductHistogramImage))
					vDisplayingImageData = vLocalSystemData->vITIPCDig->vProductHistogramImage;
				else
				if ((vShowStructure) && (vLocalSystemData->vITIPCDig->vProductStructureImage) && (vShowType == cShowTypeContaminant))
					vDisplayingImageData = vLocalSystemData->vITIPCDig->vProductStructureImage;
				else
				if ((vShowImproveLearnPreview) && (vLocalSystemData->vITIPCDig->vImproveLearnStructureImage))
				{
					if (vShowType == cShowTypeContaminant2)
						vDisplayingImageData = vLocalSystemData->vITIPCDig->vImproveLearnContaminant2StructureImage;
					else
						vDisplayingImageData = vLocalSystemData->vITIPCDig->vImproveLearnStructureImage;
				}
				else
				if ((vShowOriginalStructure) && (vLocalSystemData->vITIPCDig->HaveLearnImageFile("=Process 2o")))
				{
					ReadInOriginalImageFile();
					if ((vImageBufferForDisplay) && (vImageBufferForDisplay->vGreyImage))
						vDisplayingImageData = vImageBufferForDisplay;
				}
				else
				if (vShowDeltaOriginalToCurrentStructure)
				{
					ReadInOriginalImageFile();
					if ((vImageBufferForDisplay) && (vImageBufferForDisplay->vGreyImage))
					{
						if (vShowType == cShowTypeContaminant2)
						{
							if ((vLocalSystemData->vITIPCDig->vProductContaminant2StructureImage) && (vLocalSystemData->vITIPCDig->vProductContaminant2StructureImage->vGreyImage))
								CopyMemory(TempImageOut->pix, vLocalSystemData->vITIPCDig->vProductContaminant2StructureImage->vGreyImage, vLocalSystemData->vITIPCDig->vOriginalBufferSize);

								CopyMemory(TempImageIn->pix, vImageBufferForDisplay->vGreyImage, vLocalSystemData->vITIPCDig->vOriginalBufferSize);
						}
						else
						{
							if ((vLocalSystemData->vITIPCDig->vProductStructureImage) && (vLocalSystemData->vITIPCDig->vProductStructureImage->vGreyImage))
								CopyMemory(TempImageIn->pix, vLocalSystemData->vITIPCDig->vProductStructureImage->vGreyImage, vLocalSystemData->vITIPCDig->vOriginalBufferSize);

								CopyMemory(TempImageOut->pix,vImageBufferForDisplay->vGreyImage, vLocalSystemData->vITIPCDig->vOriginalBufferSize);
						}

						SubtractImage(TempImageIn, TempImageOut);

						CopyMemory(vImageBufferForDisplay->vGreyImage, TempImageOut->pix, vLocalSystemData->vITIPCDig->vOriginalBufferSize);

						if (vShowType == cShowTypeContaminant2)
							vImageBufferForDisplay->vImageTypeString = "Delta Original-Current2";
						else
							vImageBufferForDisplay->vImageTypeString = "Delta Original-Current";
						vDisplayingImageData = vImageBufferForDisplay;
					}
				}
				else
				if (vShowDeltaCurrentPreviewStructure)
				{
					if (!vImageBufferForDisplay)
					{
						if (ThereIsEnoughMemory(sizeof(CImageData), "vImageBufferForDisplay"))
							vImageBufferForDisplay = new CImageData;
						if (vImageBufferForDisplay)
						{
							//fill buffer with 128 because are finding smallest value below 128
							if (vShowType == cShowTypeContaminant2)
								vImageBufferForDisplay->vImageTypeString = "Delta Current-Improve2";
							else
								vImageBufferForDisplay->vImageTypeString = "Delta Current-Improve";
							vImageBufferForDisplay->vImageType = cDataImageType;
							vImageBufferForDisplay->MakeGreyImage(vLocalSystemData->vITIPCDig->vOriginalBufferSize);

							if (vImageBufferForDisplay->vGreyImage)
								FillMemory(vImageBufferForDisplay->vGreyImage, vLocalSystemData->vITIPCDig->vOriginalBufferSize,0x80);
						}
					}
					if ((vImageBufferForDisplay) && (vImageBufferForDisplay->vGreyImage))
					{
						if (vShowType == cShowTypeContaminant2)
						{
							if (vLocalSystemData->vITIPCDig->vImproveLearnContaminant2StructureImage)
							if (vLocalSystemData->vITIPCDig->vImproveLearnContaminant2StructureImage->vGreyImage)
								CopyMemory(TempImageOut->pix, vLocalSystemData->vITIPCDig->vImproveLearnContaminant2StructureImage->vGreyImage, vLocalSystemData->vITIPCDig->vOriginalBufferSize);

							if ((vLocalSystemData->vITIPCDig->vProductContaminant2StructureImage) && (vLocalSystemData->vITIPCDig->vProductContaminant2StructureImage->vGreyImage))
								CopyMemory(TempImageIn->pix, vLocalSystemData->vITIPCDig->vProductContaminant2StructureImage->vGreyImage, vLocalSystemData->vITIPCDig->vOriginalBufferSize);
						}
						else
						{
							if (vLocalSystemData->vITIPCDig->vImproveLearnStructureImage)
							if (vLocalSystemData->vITIPCDig->vImproveLearnStructureImage->vGreyImage)
								CopyMemory(TempImageIn->pix, vLocalSystemData->vITIPCDig->vImproveLearnStructureImage->vGreyImage, vLocalSystemData->vITIPCDig->vOriginalBufferSize);

							if (vLocalSystemData->vITIPCDig->vProductStructureImage)
							if (vLocalSystemData->vITIPCDig->vProductStructureImage->vGreyImage)
								CopyMemory(TempImageOut->pix, vLocalSystemData->vITIPCDig->vProductStructureImage->vGreyImage, vLocalSystemData->vITIPCDig->vOriginalBufferSize);
						}

						SubtractImage(TempImageIn, TempImageOut);

						CopyMemory(vImageBufferForDisplay->vGreyImage, TempImageOut->pix, vLocalSystemData->vITIPCDig->vOriginalBufferSize);

						vDisplayingImageData = vImageBufferForDisplay;
					}
				}
				else
				if ((vShowStructure) && (vLocalSystemData->vITIPCDig->vProductContaminant2StructureImageWithNoDilation) && (vLocalSystemData->vITIPCDig->vProductContaminant2StructureImageWithNoDilation->vGreyImage) && 
					(vShowType == cShowTypeContaminant2NoDilation))
				{
					vDisplayingImageData = vLocalSystemData->vITIPCDig->vProductContaminant2StructureImageWithNoDilation;
				}
				else
				if ((vShowStructure) && (vLocalSystemData->vITIPCDig->vProductContaminant2StructureImage) && (vLocalSystemData->vITIPCDig->vProductContaminant2StructureImage->vGreyImage) && 
					(vShowType == cShowTypeContaminant2))
				{
					vDisplayingImageData = vLocalSystemData->vITIPCDig->vProductContaminant2StructureImage;
				}
				else
				if ((vShowStructure) && (vLocalSystemData->vITIPCDig->vStructureCount) &&
					(vLocalSystemData->vITIPCDig->vVoidStructureImage) &&
					(vShowType == cShowTypeVoid))
				{
					vDisplayingImageData = vLocalSystemData->vITIPCDig->vVoidStructureImage;
				}
				else
				//if ((vShowStructure) &&
				//	(vLocalSystemData->vITIPCDig->vVoidProductFactoredStructureImage) &&
				//	(vShowType == cShowTypeVoid))
				//	vDisplayingImageData = vLocalSystemData->vITIPCDig->vVoidProductFactoredStructureImage;
				//else
				if ((vShowStructure) && (vLocalConfigurationData->vAllowVoidInspection) && (vLocalSystemData->vITIPCDig->vVoidProductStructureImage) && (vShowType == cShowTypeVoid))
				{
					vDisplayingImageData = vLocalSystemData->vITIPCDig->vVoidProductStructureImage;
				}
				else
				if ((vLocalSystemData->vITIPCDig->vRejectBufferShowing >= 0) &&
					(vLocalLastNRejectImages[vLocalSystemData->vITIPCDig->vRejectBufferShowing]) &&
					(vLocalLastNRejectImages[vLocalSystemData->vITIPCDig->vRejectBufferShowing]->vTop) &&
					(vLocalLastNRejectImages[vLocalSystemData->vITIPCDig->vRejectBufferShowing]->vTop <= 
					vLocalSystemData->vITIPCDig->vOriginalBufferSizeX))
				{
					vDisplayingImageData = vLocalLastNRejectImages[vLocalSystemData->vITIPCDig->vRejectBufferShowing];
				}
				else
				if ((vLocalSystemData->vITIPCDig->vOriginalImage->vTop) &&
					(vLocalSystemData->vITIPCDig->vOriginalImage->vTop <= vLocalSystemData->vITIPCDig->vOriginalBufferSizeX))
					vDisplayingImageData = vLocalSystemData->vITIPCDig->vOriginalImage;

				//if (!vShowAlternateImage)
				if ((vDisplayingImageData) && (vDisplayingImageData->vGreyImage))
				{
					//clear any 0 value pixels in original image so not shown as red
					BYTE *TempPointer = vDisplayingImageData->vGreyImage;
					//8/27/2009
					if (!vShowAlternateImage)
					for (DWORD TempLoop = 0;TempLoop < vLocalSystemData->vITIPCDig->vOriginalBufferSize; TempLoop++)
					{
						if (*TempPointer < 2)
							*TempPointer = 2;
						*TempPointer++;
					}

					if ((vDisplayingImageData) && (vDisplayingImageData->vGreyImage))
						CopyMemory(TempImageIn->pix, vDisplayingImageData->vGreyImage, vLocalSystemData->vITIPCDig->vOriginalBufferSize);

					if ((vIntensityAdjust > 1) || (vIntensityAdjust < -1))
					{
						int TempResult = 0;
						BYTE *TempBuffer = TempImageIn->pix; 
						for (DWORD TempLoop = 0; TempLoop < vLocalSystemData->vITIPCDig->vOriginalBufferSize; TempLoop++)
						{
							if ((*TempBuffer != 255) && (*TempBuffer != 0))
							{
								TempResult = *TempBuffer + vIntensityAdjust;
								if (TempResult > 254)
									*TempBuffer = 255;
								else
								if (TempResult < 1) 
									*TempBuffer = 0;
								else
									*TempBuffer = TempResult;
							}
							TempBuffer++;
						}
					}

					int TempTopReferenceAdjust = 0;
					int TempLeftReferenceAdjust = 0;
					if (vDisplayingImageData)
					if (vInspectionShowing)
					{
						if (vInspectionShowing->vReferenceToTop)
							TempTopReferenceAdjust = vDisplayingImageData->vTopReferenceAdjust + vLocalSystemData->vTweakTopReferenceAdjust;
						else
						if (vInspectionShowing->vReferenceToBottom)
							TempTopReferenceAdjust = vDisplayingImageData->vTopReferenceAdjust + vLocalSystemData->vTweakTopReferenceAdjust;
						else
							TempTopReferenceAdjust = 0;

						TempLeftReferenceAdjust = vDisplayingImageData->vLeftReferenceAdjust + vLocalSystemData->vTweakReferenceAdjust;
					}

					if (vInspectionShowing)
					if ((vInspectionShowing->vROIPixelLeft == 0) && (vInspectionShowing->vROIPixelRight == vLocalSystemData->vITIPCDig->vOriginalBufferSizeY))
					{
						TempLeftReferenceAdjust = 0;
						TempTopReferenceAdjust = 0;
					}

					if (vShowReferenceAdjustedImage)
					if ((TempTopReferenceAdjust != 0) ||
						(TempLeftReferenceAdjust != 0))
					if (!vShowAlternateImage)
					if ((vLocalSystemData->vITIPCDig->vNIDDScratchImage) && (vLocalSystemData->vITIPCDig->vNIDDScratchImage->pix))
							vLocalSystemData->vITIPCDig->MakeReferenceImageAdjustment(TempImageIn->pix, vLocalSystemData->vITIPCDig->vNIDDScratchImage->pix, TempTopReferenceAdjust, TempLeftReferenceAdjust);

					//12/1/2005
					CopyMemory(
						TempImageOut->pix,
						TempImageIn->pix,
						vLocalSystemData->vITIPCDig->vOriginalBufferSize);

					if (!vShowAlternateImage)
					if (vInspectionShowing)
					{
						if (vWhiteOutAnythingTouchingTheTop)
							vInspectionShowing->WhiteOutAnythingTouchingTheTop(TempImageIn->pix, vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY);

						if (vFillBackground)
							vInspectionShowing->FillBackground(TempImageIn->pix, vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY);

						if (vFillInLightSpots)
						if (vInspectionShowing)
							vInspectionShowing->FillInLightSpots(TempImageIn->pix, vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY, vFillInLightSpots);

						//if (vFillInLightSpots)
						//if (vInspectionShowing)
						//	vInspectionShowing->CurveFitFillInLightSpots(TempImageIn->pix,
						//	vLocalSystemData->vITIPCDig->vDisplayBufferHeightInPixels, vLocalSystemData->vITIPCDig->vDisplayBufferWidthInPixels);

						if (vPreSmoothImage)
						{
							WORD TempBottom = 0;
							WORD TempLeft = 0;
							WORD TempHeight = 0;
							WORD TempWidth = 0;

							BYTE TempAutoImproveState = vDisplayingImageData->vContainerLearnState;
							if (vInspectionShowing)
							if (!vInspectionShowing->vAddStructure)
								TempAutoImproveState = 0;
							else
							if ((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode) ||
								(vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vLocalConfigurationData->vScanTracType == cAllegroScanTrac) ||
								(vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac)) //Continuous Feed, or a Pipeline or TD
								TempAutoImproveState = 0;
							else
							if (!vLocalConfigurationData->vAutoImproveEnabled)
								TempAutoImproveState = 0;

							vLocalSystemData->vITIPCDig->GetInspectionROIBounds(vInspectionShowing, 
								&TempBottom, &TempLeft, &TempHeight, &TempWidth, vDisplayingImageData->vLeftReferenceAdjust, vLocalConfigurationData->vReferenceROIsToEdges, TempAutoImproveState);

							vLocalSystemData->vITIPCDig->MedianSmoothImage(TempImageIn, vPreSmoothImage, TempLeft, TempWidth, TempBottom, TempHeight);
						}

						//Erode/dilate image AT BEGINNING if should to join sections of bones or contaminants
						//erode image if should  make dark areas bigger
						if (vShowErodeCount)
						for (BYTE TempLoop = 0; TempLoop < vShowErodeCount; TempLoop++)
							vInspectionShowing->ErodeImage(TempImageIn, vLocalSystemData->vITIPCDig->vNIDDScratchImage);

						//dilate image if should make dark areas smaller
						if (vShowDilationCount)
						{
							for (BYTE TempLoop = 0; TempLoop < vShowDilationCount; TempLoop++)
								vInspectionShowing->DilateImage(TempImageIn, vLocalSystemData->vITIPCDig->vNIDDScratchImage);

							if (vShowErodeCountAtStart)
							for (BYTE TempLoop = 0; TempLoop < vShowErodeCountAtStart; TempLoop++)
								vInspectionShowing->ErodeImage(TempImageIn, vLocalSystemData->vITIPCDig->vNIDDScratchImage);
						}

						//	//for testing
						//	if (!vDisplayEnhancedImage)
						//		CopyMemory(TempImageIn->pix, vLocalSystemData->vITIPCDig->vNIDDScratchImage1->pix, vLocalSystemData->vITIPCDig->vOriginalBufferSize);
						//}

						if (vUseVariabiltyImage)
						{
							if (vUseVariabiltyImage > 3) //4 or higher apply the desensitizing to the processing
							{
								if (vInspectionShowing->vDesensitizeIrregularArea)
								{
									CopyMemory(vLocalSystemData->vITIPCDig->vNIDDScratchImage->pix, TempImageIn->pix, vLocalSystemData->vITIPCDig->vOriginalBufferSize);

									//just show first steps of processing in the current image
									vInspectionShowing->MakeVariablityImage(vLocalSystemData->vITIPCDig->vNIDDScratchImage, vLocalSystemData->vITIPCDig->vNIDDScratchImage,
										vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY, vUseVariabiltyImage);
								}
							}
							else
								vInspectionShowing->MakeVariablityImage(TempImageIn, vLocalSystemData->vITIPCDig->vNIDDScratchImage,
									vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY, vUseVariabiltyImage);
						}
						if (vRemoveJarPunt >= 2)
						{
							vInspectionShowing->RemoveJarPunt(TempImageIn->pix, vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY, 
								vDisplayingImageData->vLeft - vDisplayingImageData->vLeftReferenceAdjust, vDisplayingImageData->vRight - vDisplayingImageData->vLeftReferenceAdjust, 
								vGlobalCurrentProduct->vSetupHistoryAverageDensity, vRemoveJarPunt - 1);
						}
						else
						if (vRemoveJarPunt)
						{
							vInspectionShowing->RemoveJarPunt(TempImageIn->pix, vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY, 
								vDisplayingImageData->vLeft - vDisplayingImageData->vLeftReferenceAdjust, vDisplayingImageData->vRight - vDisplayingImageData->vLeftReferenceAdjust, 
								vGlobalCurrentProduct->vSetupHistoryAverageDensity, 0);
						}

						if (vTrimEdgesOfJarAtROI)
						{
							vInspectionShowing->TrimEdgesOfJarAtROI(TempImageIn->pix, vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY, true,
								vDisplayingImageData->vLeft - vDisplayingImageData->vLeftReferenceAdjust, vDisplayingImageData->vRight - vDisplayingImageData->vLeftReferenceAdjust);
						}


						/*
						if (vPreSelectDarkSpotsInImage)
						{
							int TempKernel[9];
							for (BYTE TempLoop = 0; TempLoop < 9; TempLoop++)
								TempKernel[TempLoop] = 1;

							Erflag TempError = mvt_bottom_hat(TempImageIn,vPreSelectDarkSpotsInImage,
								vPreSelectDarkSpotsInImage,
								&TempKernel[0], EDGE_IGNORE);
						}

						if (vShowLineDerivative)
						if ((vLocalSystemData->vITIPCDig->vNIDDScratchImage) && (vLocalSystemData->vITIPCDig->vNIDDScratchImage->pix))
						{
							vInspectionShowing->CalculateLineDerivative(TempImageIn->pix, vLocalSystemData->vITIPCDig->vNIDDScratchImage->pix,
								vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY, 
								vRequire2InARow, 0); //vInspectionShowing->vOverlap);
						}
						*/

						if (vTakeDerivative)
						if ((vLocalSystemData->vITIPCDig->vNIDDScratchImage) && (vLocalSystemData->vITIPCDig->vNIDDScratchImage->pix))
							vInspectionShowing->TakeLineDerivative(TempImageIn->pix, vLocalSystemData->vITIPCDig->vNIDDScratchImage->pix,
								vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY, vGlobalCurrentProduct->vSetupHistoryAverageDensity, (BYTE)vThreshold);

						if (vTakeDifferenceFromMedianFilter)
						{
							CopyMemory(vLocalSystemData->vITIPCDig->vNIDDScratchImage->pix, TempImageIn->pix, vLocalSystemData->vITIPCDig->vOriginalBufferSize);

							BYTE TempMedian = vTakeDifferenceFromMedianFilter & 0x3F;
							DoMedianSmooth(vLocalSystemData->vITIPCDig->vNIDDScratchImage, TempMedian, TempMedian);

							if (vTakeDifferenceFromMedianFilter < 128)
							{
#ifdef UseIFCDLLs
								mvt_subtract(vLocalSystemData->vITIPCDig->vNIDDScratchImage, TempImageIn);
								mvt_invert(TempImageIn);
#endif
							}
							else
								CopyMemory(TempImageIn->pix, vLocalSystemData->vITIPCDig->vNIDDScratchImage->pix, vLocalSystemData->vITIPCDig->vOriginalBufferSize);
						}

						if (vBonesInspection)
						{
							WORD TempBottom = 0;
							WORD TempLeft = 0;
							WORD TempHeight = 0;
							WORD TempWidth = 0;
							BYTE TempAutoImproveState = 0;

							vLocalSystemData->vITIPCDig->GetInspectionROIBounds(vInspectionShowing, &TempBottom, &TempLeft, &TempHeight, &TempWidth, 
								vDisplayingImageData->vLeftReferenceAdjust, vLocalConfigurationData->vReferenceROIsToEdges, TempAutoImproveState);

							vInspectionShowing->FindBonesInspection(TempImageIn->pix, vLocalSystemData->vITIPCDig->vNIDDScratchImage,
								vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY, 
								TempLeft, TempLeft + TempWidth,  TempBottom, TempBottom + TempHeight);
						}

						if (vTakeDifferenceFromMedian) //bones algorithms
							vInspectionShowing->TakeDifferenceFromMedian(TempImageIn->pix, vLocalSystemData->vITIPCDig->vNIDDScratchImage,
							vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY, vDisplayingImageData->vGreyImage, (BYTE)vThreshold);

						if (vFindSpotInSubROI)
						{
							WORD TempBottom = 0;
							WORD TempLeft = 0;
							WORD TempHeight = 0;
							WORD TempWidth = 0;

							BYTE TempAutoImproveState = vDisplayingImageData->vContainerLearnState;
							if (vInspectionShowing)
							if (!vInspectionShowing->vAddStructure)
								TempAutoImproveState = 0;
							else
							if ((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode) ||
								(vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vLocalConfigurationData->vScanTracType == cAllegroScanTrac) ||
								(vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac)) //Continuous Feed, or a Pipeline or TD
								TempAutoImproveState = 0;
							else
							if (!vLocalConfigurationData->vAutoImproveEnabled)
								TempAutoImproveState = 0;

							vLocalSystemData->vITIPCDig->GetInspectionROIBounds(vInspectionShowing, &TempBottom, &TempLeft, &TempHeight, &TempWidth, 
								vDisplayingImageData->vLeftReferenceAdjust, vLocalConfigurationData->vReferenceROIsToEdges, TempAutoImproveState);

							vInspectionShowing->FindSpotInSubROI(TempImageIn->pix, vLocalSystemData->vITIPCDig->vNIDDScratchImage, vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, 
								vLocalSystemData->vITIPCDig->vOriginalBufferSizeY, (BYTE)vThreshold, vLocalSystemData->vITIPCDig->vDesensitizeImage, true, TempBottom, TempBottom + TempHeight, TempLeft, TempLeft + TempWidth);
						}

						if (vFindDarkSpotOnLine)
						if ((vLocalSystemData->vITIPCDig->vNIDDScratchImage) && (vLocalSystemData->vITIPCDig->vNIDDScratchImage->pix))
							vInspectionShowing->FindDarkSpotOnLine(TempImageIn->pix, vLocalSystemData->vITIPCDig->vNIDDScratchImage->pix,
							vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY, 
							(BYTE)vThreshold);

						if (vShowLineDerivative == 1)
						{
							vInspectionShowing->ClipToCurveFit(TempImageIn->pix,
								vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY);
						}

						if (vShowLineDerivative == 2)
						if ((vLocalSystemData->vITIPCDig->vNIDDScratchImage) && (vLocalSystemData->vITIPCDig->vNIDDScratchImage->pix))
						{
							vInspectionShowing->CalculateLineDerivative(TempImageIn->pix, vLocalSystemData->vITIPCDig->vNIDDScratchImage->pix,
								vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY,
								vGlobalCurrentProduct->vSetupHistoryAverageDensity);
						}

 						if (vLookForDarkSpots)
						{
							WORD TempBottom = 0;
							WORD TempLeft = 0;
							WORD TempHeight = 0;
							WORD TempWidth = 0;
							BYTE TempAutoImproveState = 0;

							vLocalSystemData->vITIPCDig->GetInspectionROIBounds(vInspectionShowing, &TempBottom, &TempLeft, &TempHeight, &TempWidth, 
								vDisplayingImageData->vLeftReferenceAdjust, vLocalConfigurationData->vReferenceROIsToEdges, TempAutoImproveState);

							WORD TempTop = TempBottom + TempHeight;
							WORD TempRight = TempLeft + TempWidth;

							vInspectionShowing->LookForDarkSpotsOnLine(TempImageIn->pix, vLocalSystemData->vITIPCDig->vNIDDScratchImage,
								vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY, TempLeft, TempRight, TempBottom, TempTop, (BYTE)vThreshold);
						}

						if (vDoContaminant22Algorithm)
						{
							BYTE TempThreshold = (BYTE)vThreshold;	//ams 0407 TH scaling begins
							int TempResult22 = vInspectionShowing->InspxContaminant22InspectionB(TempImageIn->pix,
								vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, 
								vLocalSystemData->vITIPCDig->vOriginalBufferSizeY, TempThreshold, 1);
						}

						if (vShowSubtractAverage)
						if (vLocalSystemData->vITIPCDig->vProductAverageImage)
						//if (vInspectionShowing->vSubtractAverage)
						if (vInspectionShowing->vInspectionType != cVoidInspection)
						{
							vLocalSystemData->vITIPCDig->SubtractProductAverageImage(TempImageIn->pix, vLocalSystemData->vITIPCDig->vProductAverageImage);
						}

						if (vShowSubtractAverage)
						if (vInspectionShowing->vInspectionType == cVoidInspection)
						if (vLocalSystemData->vITIPCDig->vProductAverageImage)
							vLocalSystemData->vITIPCDig->SubtractVoidProductAverageImage(TempImageIn->pix);

						if (vShowAddStructure)
						{
							if (vInspectionShowing->vInspectionType == cVoidInspection)
							{
								if (vLocalSystemData->vITIPCDig->vVoidProductStructureImage)
									vLocalSystemData->vITIPCDig->AddVoidProductStructureImage(TempImageIn->pix);
							}
							else
							if (vShowSubtractAverage)  //standard contaminant structure
							{
								if (vLocalSystemData->vITIPCDig->vProductStructureImage)
									vLocalSystemData->vITIPCDig->AddProductStructureImage(TempImageIn->pix);
							}
							else //Contaminant2 structure
							if ((vLocalSystemData->vITIPCDig->vProductContaminant2StructureImage) && (vLocalSystemData->vITIPCDig->vProductContaminant2StructureImage->vGreyImage) &&
								(vInspectionShowing->vAddStructure) && (!vInspectionShowing->vSubtractAverage) && (vInspectionShowing->vInspectionType != cVoidInspection))
							{
								vLocalSystemData->vITIPCDig->AddProductContaminant2StructureImage(TempImageIn->pix);
							}
						}
						if (vShowTopHatImage)
						{
							int TempKernel[100];
							for (BYTE TempLoop = 0; TempLoop < 100; TempLoop = TempLoop + 2)
							{
								TempKernel[TempLoop] = 0;
								TempKernel[TempLoop + 1] = 1;
							}
							TempKernel[0] = 1;TempKernel[1] = 1;TempKernel[2] = 1;
							TempKernel[3] = 1;TempKernel[4] = 1;TempKernel[5] = 1;
							TempKernel[6] = 1;TempKernel[7] = 1;TempKernel[8] = 1;

							Erflag TempError = mvt_convolve_byte(TempImageIn, vShowTopHatImage, vShowTopHatImage, &TempKernel[0], 0, 0, MVT_NOCHANGE);
						}

						//blur image if should
						if (vShowBluredImage)
						{
							WORD TempKernelSize = 3;
							double TempScale = 1.0 / (TempKernelSize * TempKernelSize);
							for (DWORD TempLoop = 0; TempLoop < vShowBluredImage; TempLoop++)
								Erflag TempError = mvt_blur(TempImageIn,
									TempKernelSize,TempKernelSize,(float)TempScale,0);
						}

						if (vIPContaminant)
						if (vLocalSystemData->vITIPCDig->vNIDDScratchImage2)
						if (vLocalSystemData->vITIPCDig->vNIDDScratchImage2->pix)
						{
							CopyMemory(vLocalSystemData->vITIPCDig->vNIDDScratchImage2->pix, TempImageIn->pix, vLocalSystemData->vITIPCDig->vOriginalBufferSize);
							ThresholdImage(vLocalSystemData->vITIPCDig->vNIDDScratchImage2, vIPContaminant); //create a mask of what is below the threshold

							if (!vDisplayEnhancedImage) //if not showing more steps, copy mask to output image
								CopyMemory(TempImageIn->pix, vLocalSystemData->vITIPCDig->vNIDDScratchImage2->pix, vLocalSystemData->vITIPCDig->vOriginalBufferSize);
						}

						//enhance image if should
						if ((vDisplayEnhancedImage) && (vInspectionShowing->vEnhanceMethod))
						{
							vLocalSystemData->vITIPCDig->PerformEnhanceOnImage(TempImageIn, vLocalSystemData->vITIPCDig->vNIDDScratchImage, vInspectionShowing->vEnhanceMethod);
						}

						//Erode/dilate image AT END if should to join sections of bones
						//erode image if should  make dark areas bigger
						if (vShowErodeCountAtEnd)
						for (BYTE TempLoop = 0; TempLoop < vShowErodeCountAtEnd; TempLoop++)
							vInspectionShowing->ErodeImage(TempImageIn, vLocalSystemData->vITIPCDig->vNIDDScratchImage);

						//dilate image if should make dark areas smaller
						if (vShowDilationCountAtEnd)
						for (BYTE TempLoop = 0; TempLoop < vShowDilationCountAtEnd; TempLoop++)
							vInspectionShowing->DilateImage(TempImageIn, vLocalSystemData->vITIPCDig->vNIDDScratchImage);

						//if ((vShowImageFactors) && (vShowOriginalImageFactor > 0) && (vShowEnhancedImageFactor > 0))
						//{
						//	vLocalSystemData->vITIPCDig->CombineImages(TempImageIn->pix, vShowEnhancedImageFactor, vDisplayingImageData->vGreyImage, vShowOriginalImageFactor);
						//}

						//add histogram if should
						if (vShowAddHistogram)
						if (vLocalSystemData->vITIPCDig->vProductHistogramImage)
							vLocalSystemData->vITIPCDig->AddProductHistogramImage(TempImageIn->pix);
					}

					WORD TempBottom = 0;
					WORD TempLeft = 0;
					WORD TempHeight = 0;
					WORD TempWidth = 0;

					BYTE TempAutoImproveState = vDisplayingImageData->vContainerLearnState;
					if (vInspectionShowing)
					if (!vInspectionShowing->vAddStructure)
						TempAutoImproveState = 0;
					else
					if ((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode) ||
						(vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vLocalConfigurationData->vScanTracType == cAllegroScanTrac) ||
						(vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac)) //Continuous Feed, or a Pipeline or TD
						TempAutoImproveState = 0;
					else
					if (!vLocalConfigurationData->vAutoImproveEnabled)
						TempAutoImproveState = 0;

					vLocalSystemData->vITIPCDig->GetInspectionROIBounds(vInspectionShowing, 
						&TempBottom, &TempLeft, &TempHeight, &TempWidth, vDisplayingImageData->vLeftReferenceAdjust, 
						vLocalConfigurationData->vReferenceROIsToEdges, TempAutoImproveState);

					//calculate number of pixels in ROI that are red, and value of darkest and lightest
					if (vDisplayingImageData)
					if (vInspectionShowing)
					{ //calculate the number of red pixels for this inspection
						vShowingInspectionNumberOfRedPixels = 0;
						vDarkestValueInROI = 255;
						vLightestValueInROI = 0;
						vNumberOfContaminants = 0;
						vSizeOfLargestContaminant = 0;
						vSizeOfAllContaminants = 0;
					}

					if (!vShowReferenceAdjustedImage)
					{
						TempLeft = TempLeft + vDisplayingImageData->vLeftReferenceAdjust;
						if (TempLeft + TempWidth >= vLocalSystemData->vITIPCDig->vAquisitionSizeY)
							TempLeft = vLocalSystemData->vITIPCDig->vAquisitionSizeY - TempWidth;
						if (TempLeft + TempWidth >= vLocalSystemData->vITIPCDig->vAquisitionSizeY)
							TempLeft = 0;
					}

					vLastCalculateThreshold = (int)vThreshold;
					if (vShowThreshold)
					for (WORD TempXLoop = 0; TempXLoop < TempWidth; TempXLoop++)
					{
						TempPointer = TempImageIn->pix + TempBottom + ((TempLeft +
							TempXLoop) * vLocalSystemData->vITIPCDig->vOriginalBufferSizeX);

						for (WORD TempYLoop = 0; TempYLoop < TempHeight; TempYLoop++)
						{
							if (*TempPointer < vDarkestValueInROI)
								vDarkestValueInROI = *TempPointer;
							if (*TempPointer > vLightestValueInROI)
								vLightestValueInROI = *TempPointer;
							if (*TempPointer < vLastCalculateThreshold)  
							{//if pixel is  darker than threshold
								vShowingInspectionNumberOfRedPixels++;
							}
							TempPointer++;
						}
					}
							/*
							//set areas outside ROI to white
							if (TempBottom)
							{
								BYTE *TempPointer = NULL;
								for (WORD TempXLoop = 0; TempXLoop < 
									vLocalSystemData->vITIPCDig->vOriginalBufferSizeY; TempXLoop++)
								{
									TempPointer = TempImageIn->pix + 
										(TempXLoop * vLocalSystemData->vITIPCDig->vOriginalBufferSizeX);

									for (WORD TempYLoop = 0; TempYLoop < TempBottom; TempYLoop++)
									{//set pixel to white since not in ROI
										*TempPointer++ = 255;
									}
								}
							}
							if (TempTop < vLocalSystemData->vITIPCDig->vOriginalBufferSizeX - 1)
							{
								BYTE *TempPointer = NULL;
								for (WORD TempXLoop = 0; TempXLoop < 
									vLocalSystemData->vITIPCDig->vOriginalBufferSizeY; TempXLoop++)
								{
									TempPointer = TempImageIn->pix + TempTop +
										(TempXLoop * vLocalSystemData->vITIPCDig->vOriginalBufferSizeX);

									for (WORD TempYLoop = 0; TempYLoop < 
										vLocalSystemData->vITIPCDig->vOriginalBufferSizeX - TempTop; TempYLoop++)
									{//set pixel to white since not in ROI
										*TempPointer++ = 255;
									}
								}
							}
							if (TempLeft)
							{
								BYTE *TempPointer = NULL;
								for (WORD TempXLoop = 0; TempXLoop < TempLeft; TempXLoop++)
								{
									TempPointer = TempImageIn->pix + 
										(TempXLoop * vLocalSystemData->vITIPCDig->vOriginalBufferSizeX);

									for (WORD TempYLoop = 0; TempYLoop < 
										vLocalSystemData->vITIPCDig->vOriginalBufferSizeX; TempYLoop++)
									{//set pixel to white since not in ROI
										*TempPointer++ = 255;
									}
								}
							}
							if (TempRight < vLocalSystemData->vITIPCDig->vOriginalBufferSizeY - 1)
							{
								BYTE *TempPointer = NULL;
								for (WORD TempXLoop = 0; TempXLoop < 
									vLocalSystemData->vITIPCDig->vOriginalBufferSizeY - TempRight; TempXLoop++)
								{
									TempPointer = TempImageIn->pix + 
										((TempXLoop + TempRight) * vLocalSystemData->vITIPCDig->vOriginalBufferSizeX);

									for (WORD TempYLoop = 0; TempYLoop < 
										vLocalSystemData->vITIPCDig->vOriginalBufferSizeX; TempYLoop++)
									{//set pixel to white since not in ROI
										*TempPointer++ = 255;
									}
								}
							}
					*/

					//threshold the image if should
					if (vShowThreshold)
					if (vInspectionShowing)
					{
						Erflag TempError = 0;
						if (vInspectionShowing->vInspectionType != cUnderfillByWeightInspection)
						if (vInspectionShowing->vInspectionType != cOverfillByWeightInspection)
						if ((vLocalConfigurationData->vDemoMode) && (vInspectionShowing->vInspectionType != cKernelInspection))
						{
							if ((vLocalConfigurationData->vRedIndicatorToShowOnMainMenu == cDefectsInRed) || ((vShowThreshold) && (vLocalConfigurationData->vRedIndicatorToShowOnMainMenu != cDefectsInPseudoColor)))
								vLocalSystemData->vITIPCDig->ThresholdBufferDown(TempImageIn->pix, vLocalSystemData->vITIPCDig->vOriginalBufferSize, (BYTE)vThreshold);
						}
						else
						{
							if ((vInspectionShowing->vInspectionType == cVoidInspection) &&
								(!vInspectionShowing->vEnhanceMethod) &&
								(!vShowAlternateImage) &&
								(vShowThreshold))
							{  //if void, and not enhanced, then must invert image
								BYTE TempThreshold = (BYTE)vThreshold;
								if (((!vShowThreshold) || (vLocalConfigurationData->vRedIndicatorToShowOnMainMenu == cDefectsInRed)) ||
									((vShowThreshold) && (vLocalConfigurationData->vRedIndicatorToShowOnMainMenu != cDefectsInPseudoColor)) ||
									((vShowThreshold) && (vLocalConfigurationData->vRedIndicatorToShowOnMainMenu == cDefectsInPseudoColor) && (vHideRedIndicator)))
								if (!vHideRedIndicator)
									ThresholdImageDown(TempImageIn,TempThreshold);
								else
									ThresholdImage(TempImageIn,TempThreshold);
							}
							else
							if ((vInspectionShowing->vInspectionType == cVoidInspection) ||
								(vInspectionShowing->vInspectionType == cGlass13mmContaminantInspection) ||
								(vInspectionShowing->vInspectionType == cJarBottomContaminantInspection) ||
								(vInspectionShowing->vInspectionType == cIPContaminantInspection) ||
								(vInspectionShowing->vInspectionType == cSpotInROIContaminantInspection) ||
								(vInspectionShowing->vInspectionType == cContaminantInspection) ||
								(vInspectionShowing->vInspectionType == cWireContaminantInspection) ||
								(vInspectionShowing->vInspectionType == cThresholdContaminantInspection) ||
								(vInspectionShowing->vInspectionType == cBadHeartInspection) ||
								(vInspectionShowing->vInspectionType == cDripBySpoutInspection) ||
								(vInspectionShowing->vInspectionType == cDripInspection) ||
								(vInspectionShowing->vInspectionType == cFloodedInspection) ||
								(vInspectionShowing->vInspectionType == cContaminantBonesInspection) ||
								//(vInspectionShowing->vInspectionType == cContaminant22Inspection) ||
								(vInspectionShowing->vInspectionType == cTopContaminantInspection) ||
								(vInspectionShowing->vInspectionType == cBottomContaminantInspection) ||

								(vInspectionShowing->vInspectionType == cItemCountInspection) || (vInspectionShowing->vInspectionType == cSizeInspection) ||
								(vInspectionShowing->vInspectionType == cMissingLidInspectionOnSize) ||
								//(vInspectionShowing->vInspectionType == cCheckWeighInspection) ||
								(vInspectionShowing->vInspectionType == cDensityInspection) ||
								(vInspectionShowing->vInspectionType == cMissingLidInspection) ||
								(vInspectionShowing->vInspectionType == cUnderfillInspection) ||
								(vInspectionShowing->vInspectionType == cUnderfillByWeightInspection) ||
								(vInspectionShowing->vInspectionType == cOverfillByWeightInspection) ||
								(vInspectionShowing->vInspectionType == cOverfillInspection) ||
								(vInspectionShowing->vInspectionType == cStandardDeviationInspection) ||
								(vInspectionShowing->vInspectionType == cContaminant2Inspection) ||
								(vInspectionShowing->vInspectionType == cAdvancedContaminant2Inspection))
							{
								if ((vInspectionShowing->vInspectionType == cItemCountInspection) && (vInspectionShowing->vMinimumBrightnessAceptable > 0) && (vInspectionShowing->vMinimumBrightnessAceptable < 255))
								{
									BYTE *TempPointer = TempImageIn->pix;
									for (DWORD TempLoop = 0; TempLoop < vLocalSystemData->vITIPCDig->vOriginalBufferSize; TempLoop++)
									{
										if (*TempPointer < vInspectionShowing->vMinimumBrightnessAceptable)
											*TempPointer = 255;
										TempPointer++;
									}
								}

								BYTE TempThreshold = (BYTE)vThreshold;
								if (((!vShowThreshold) || (vLocalConfigurationData->vRedIndicatorToShowOnMainMenu == cDefectsInRed)) ||
									((vShowThreshold) && (vLocalConfigurationData->vRedIndicatorToShowOnMainMenu != cDefectsInPseudoColor)) ||
									((vShowThreshold) && (vLocalConfigurationData->vRedIndicatorToShowOnMainMenu == cDefectsInPseudoColor) && (vHideRedIndicator)))
								if ((!vHideRedIndicator) && (!vIPContaminant))
									ThresholdImageDown(TempImageIn,TempThreshold);
								else
									ThresholdImage(TempImageIn,TempThreshold);
							}
							else
							if (vInspectionShowing->vInspectionType == cKernelInspection)
							{
								Image *TempKernelImage;
								//create an image to store the amount each pixel is different from the baseline
								TempKernelImage = im_create(IM_BYTE, 
									vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, 
									vLocalSystemData->vITIPCDig->vAquisitionSizeY);
								if ((TempKernelImage) && (TempKernelImage->pix))
								{
									
									BYTE *TempDesitinationPointer = TempKernelImage->pix;
									BYTE *TempSourcePointer = TempImageIn->pix;
									for (DWORD TempLoop = 0;TempLoop < vLocalSystemData->vITIPCDig->vOriginalBufferSize; 
										TempLoop++)
									{
										if (*TempSourcePointer < 2)
											*TempSourcePointer = 2;
										*TempDesitinationPointer++ = *TempSourcePointer++;// + TempMax;
									}

									//find out how many kernels high and wide will have to analyze
									WORD TempNumberOfKernelsHigh = (WORD)((((double)vInspectionShowing->vROIPixelTop - 
										(double)vInspectionShowing->vROIPixelBottom) / 
										(double)vInspectionShowing->vSize * 2.0) + .99);
									WORD TempNumberOfKernelsWide = (WORD)((((double)vInspectionShowing->vROIPixelRight - 
										(double)vInspectionShowing->vROIPixelLeft) / 
										(double)vInspectionShowing->vSize * 2.0) + .99);

									//analyze each kernel high and wide
									for (WORD TempKernelHighLoop = 0; TempKernelHighLoop < TempNumberOfKernelsHigh;
										TempKernelHighLoop++)
									for (WORD TempKernelWideLoop = 0; TempKernelWideLoop < TempNumberOfKernelsWide;
										TempKernelWideLoop++)
									{
										DWORD TempKernelSum = 0;
										//find the starting place for this kernel analysis
										WORD TempKernelWideStart = (WORD)(vInspectionShowing->vROIPixelLeft + (TempKernelWideLoop * vInspectionShowing->vSize / 2));
										if (TempKernelWideStart + vInspectionShowing->vSize > vInspectionShowing->vROIPixelRight)
											TempKernelWideStart = (WORD)(vInspectionShowing->vROIPixelRight - vInspectionShowing->vSize);

										WORD TempKernelHighStart = (WORD)(vInspectionShowing->vROIPixelBottom + (TempKernelHighLoop * vInspectionShowing->vSize / 2));
										if (TempKernelHighStart + vInspectionShowing->vSize > vInspectionShowing->vROIPixelTop)
											TempKernelHighStart = (WORD)(vInspectionShowing->vROIPixelTop - vInspectionShowing->vSize);
										
										//sum up all the pixels in this kernel
										WORD TempSumCount = 0;
										for (WORD TempSumWideLoop = 0; TempSumWideLoop < vInspectionShowing->vSize;
											TempSumWideLoop++)
										{
											BYTE *TempDataPointer = TempKernelImage->pix + TempKernelHighStart + ((TempKernelWideStart + TempSumWideLoop) * vLocalSystemData->vITIPCDig->vOriginalBufferSizeX);
											for (WORD TempSumHighLoop = 0; TempSumHighLoop < vInspectionShowing->vSize; TempSumHighLoop++)
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

										//if this kernel is over the average, then it is a reject
										if (TempKernelAverage < vThreshold)// + TempMax)
										{
											//put any pixels not in the baseline in the red image buffer since going to reject
											{
												BYTE *TempDestinationPointer;
												//look at all the pixels in this kernel, any that are not at baseline set red in red image
												for (WORD TempSumWideLoop = 0; TempSumWideLoop < vInspectionShowing->vSize;
													TempSumWideLoop++)
												{
													BYTE *TempDataPointer = TempKernelImage->pix + 
														TempKernelHighStart + 
														((TempKernelWideStart +	TempSumWideLoop) * 
														vLocalSystemData->vITIPCDig->vOriginalBufferSizeX);
													TempDestinationPointer = 
														TempImageIn->pix +
														TempKernelHighStart + 
														((TempKernelWideStart + TempSumWideLoop) * 
														vLocalSystemData->vITIPCDig->vOriginalBufferSizeX);
													for (WORD TempSumHighLoop = 0; TempSumHighLoop < vInspectionShowing->vSize;
														TempSumHighLoop++)
													{
														if ((*TempDataPointer < vThreshold) || (vLocalConfigurationData->vRedIndicatorToShowOnMainMenu == cDefectsInRed) ||
															((vLocalConfigurationData->vRedIndicatorToShowOnMainMenu == cDefectInRedBox) && (vShowThreshold)))
															*TempDestinationPointer = 0; //red spot
														*TempDataPointer++;
														*TempDestinationPointer++;
													}
												}
											}
										}
									}
										/*
									if (TempMax == 127)
									{
										BYTE *TempSourcePointer = TempImageIn->pix;
										for (DWORD TempLoop = 0;TempLoop < vLocalSystemData->vITIPCDig->vOriginalBufferSize; 
											TempLoop++)
										{
											if (*TempSourcePointer)
												*TempSourcePointer = *TempSourcePointer + TempMax;
											TempSourcePointer++;
										}
									}
									*/
								}
								if (TempKernelImage)
									im_delete(TempKernelImage);
							}//end of thresholding for kernel
						}
						if (TempError == 105) 
						{
							ReportErrorMessage("Error-MVT NO Lic.", cEMailInspx,32000);
						}
						else
						if (TempError != IM_OK)
						{
							ReportErrorMessage("Error-MVT Byte Only", cEMailInspx,32000);
						}

						if (vIPContaminant)
						if (vLocalSystemData->vITIPCDig->vNIDDScratchImage2)
						if (vLocalSystemData->vITIPCDig->vNIDDScratchImage2->pix)
						{
							ThresholdImage(TempImageIn, (BYTE)vThreshold); //binarize the processed image at the Threshold

							vLocalSystemData->vITIPCDig->AndImages(vLocalSystemData->vITIPCDig->vNIDDScratchImage2->pix, TempImageIn->pix, vLocalSystemData->vITIPCDig->vOriginalBufferSize);
						}

						//find blob statistics
						vSizeOfAllContaminants = 0;
						vSizeOfLargestContaminant = 0;
						vNumberOfContaminants = 0;

						if (vShowingProcess)
							CopyMemory(vProcessedRedImage, TempImageIn->pix, vLocalSystemData->vITIPCDig->vOriginalBufferSize);

						Image *TempSubImage = im_rroi(TempImageIn, TempBottom, TempLeft, TempHeight, TempWidth);

						if (vInspectionShowing->vInspectionType != cUnderfillByWeightInspection)
						if (vInspectionShowing->vInspectionType != cOverfillByWeightInspection)
						if ((TempSubImage) && (TempSubImage->pix))
						if (vBlobParameters)
						{
							if (vInspectionShowing->vSize > 1)
							{
								vBlobParameters->min_area = 2;//show all pixels vInspectionShowing->vSize;
								//vBlobParameters->min_area = vInspectionShowing->vSize;
								//if (vInspectionShowing->vSize > 3)
								//	vBlobParameters->min_area = vInspectionShowing->vSize / 2;
							}
							else
								vBlobParameters->min_area = -1;
							//TempBlobParameters->calc_results = AREA_BBOX;
							//TempBlobParameters->calc_results = 0;
							vBlobParameters->blob_color = 0x00;  //fewer xrays
							vBlobParameters->min_height = -1;
							vBlobParameters->max_area = -1;
							vBlobParameters->connected = CONNECT8;
							if (vInspectionShowing->vInspectionType == cDripInspection)
							{
								if (vInspectionShowing->vMaximumSize) //for Drip Inspection
									vBlobParameters->max_area = vInspectionShowing->vMaximumSize;
								if (vInspectionShowing->vMinimumWidth) //for Drip Inspection
									vBlobParameters->min_height = vInspectionShowing->vMinimumWidth;
							}
							if (vInspectionShowing->vInspectionType == cBadHeartInspection)
								vBlobParameters->calc_results = AREA_BBOX + AREA_BBOX_CENT_PERIM;
							else
								vBlobParameters->calc_results = AREA_BBOX;

							if (vInspectionShowing->vInspectionType != cContaminant22Inspection)
								ThresholdImage(TempSubImage,(BYTE)vThreshold);

							Erflag TempResult = mvt_blob_find(TempSubImage, vBlobParameters, vBlobResults, MVT_CF_DEFAULT);
							// area of blob calculation

							/*
							//bug fix for mvtools bug
							BYTE *TempCounterPointer = NULL;
							DWORD TempMainImageRedPixels = 0;
							for (WORD TempXLoop = 0; TempXLoop < TempWidth; TempXLoop++)
							{
								if ((DWORD)(TempBottom + (TempLeft + TempXLoop + 1) * vLocalSystemData->vITIPCDig->vOriginalBufferSizeX) < 
									vLocalSystemData->vITIPCDig->vOriginalBufferSize)
								{
									TempCounterPointer = TempSubImage->pix + 
										//TempBottom + (TempROILeft + //just looking at sub roi so do not need
										TempXLoop * vLocalSystemData->vITIPCDig->vOriginalBufferSizeX;

									for (WORD TempYLoop = 0; TempYLoop < TempHeight; TempYLoop++)
									{
										if (*TempCounterPointer == 0)  
										{//if pixel is  darker than threshold
											TempMainImageRedPixels++;
										}
										TempCounterPointer++;
									}
								}
								//else
								//	ReportErrorMessage("Error, Exceed ROI space", cEMailInspx,32000);
							}
							*/
							//show the red from doing image processing right now
							BYTE *TempPointer = TempImageIn->pix;
							for (DWORD TempLoop = 0; TempLoop < vLocalSystemData->vITIPCDig->vOriginalBufferSize; TempLoop++)
							{
								if (*TempPointer == 0)
									*TempPointer = 1;
								TempPointer++;
							}
							//now find the blobs that are bigger than minimum size and make them red
							//showing the AS Displayed Image processing on the screen if changed any settings.
							BYTE *TempMaximumPointer = TempImageIn->pix + vLocalSystemData->vITIPCDig->vOriginalBufferSize;
							if ((WORD)vBlobResults->num_blobs_found)
							{
								for (WORD TempLoop = 0; TempLoop < (WORD)vBlobResults->num_blobs_found; TempLoop++)
								{
									WORD TempBlobArea = (WORD)mvt_blob_get_area(vBlobResults, TempLoop);
									if (TempBlobArea >= vInspectionShowing->vSize)
									{
#ifdef UseIFCDLLs
										MVT_BLOB *TempBlob = vBlobResults->blobs[TempLoop];

										int TempOffset = vDisplayingImageData->vLeftReferenceAdjust;
										if (vShowReferenceAdjustedImage)
											TempOffset = 0;

										int TempStart = TempBlob->ymin + TempOffset;
										if (TempStart < 0)
											TempStart = 0;
										if (TempStart >= vLocalSystemData->vITIPCDig->vOriginalBufferSizeY)
											TempStart = vLocalSystemData->vITIPCDig->vOriginalBufferSizeY - 1;

										int TempStop = TempBlob->ymax + TempOffset;
										if (TempStop < 0)
											TempStop = 0;
										if (TempStop >= vLocalSystemData->vITIPCDig->vOriginalBufferSizeY)
											TempStop = vLocalSystemData->vITIPCDig->vOriginalBufferSizeY - 1;

										for (int TempLoopY = TempStart; TempLoopY <= TempStop; TempLoopY++)
										{
											BYTE *TempPointer = TempImageIn->pix + (TempLoopY * vLocalSystemData->vITIPCDig->vOriginalBufferSizeX) + 
												TempBlob->xmin;

											for (DWORD TempLoopX = TempBlob->xmin; TempLoopX <= TempBlob->xmax; TempLoopX++)
											if (TempPointer < TempMaximumPointer)
											{
												if (*TempPointer == 1)
													*TempPointer = 0;  //make blue pixel red as in a blob area big enough to meet minimum size
												TempPointer++;
											}
										}
#endif
									}
								}
							}
							vNumberOfContaminants = (DWORD)vBlobResults->num_blobs_found;
							vSizeOfLargestPerimeter = 0;
							DWORD TempArea = 0;
// draw image on screen
							if ((WORD)vBlobResults->num_blobs_found)
							{
								for (WORD TempLoop = 0; TempLoop < (WORD)vBlobResults->num_blobs_found; TempLoop++)
								{
									WORD TempBlobArea = (WORD)mvt_blob_get_area(vBlobResults, TempLoop);
									if (TempBlobArea > vSizeOfLargestContaminant)
										vSizeOfLargestContaminant = TempBlobArea;

									WORD TempBlobPerimeter = (WORD)mvt_blob_get_perim(vBlobResults, TempLoop);
									if (vSizeOfLargestPerimeter < TempBlobPerimeter)
										vSizeOfLargestPerimeter = TempBlobPerimeter;

									if ((TempBlobArea >= vInspectionShowing->vSize) || (vInspectionShowing->vInspectionType == cItemCountInspection) || (vInspectionShowing->vInspectionType == cFloodedInspection) || 
										(vInspectionShowing->vInspectionType == cMissingLidInspectionOnSize) || (vInspectionShowing->vInspectionType == cSizeInspection))
										TempArea = TempArea + TempBlobArea;
									else
										vNumberOfContaminants--;
								}
							}
							//if (vInspectionShowing->vSize == 1)
							//if (TempArea < TempMainImageRedPixels)
							//	TempArea = TempMainImageRedPixels;

							vSizeOfAllContaminants = TempArea;

							im_delete(TempSubImage);
							TempSubImage = NULL;
						}
						else
							if (vLocalSystemData->vLogFile.vLogSerialData)
								vLocalSystemData->vLogFile.WriteToLogFile(vLocalSystemData->vFrameCountString +
										" Error, MVT Could not create Image for Blobs",cDebugMessage);
		//end of find blob statistics

					}  //end of thresholding the image if should
						//right line red spots from buffer
					if (vLocalConfigurationData->vRedIndicatorToShowOnMainMenu == cDefectsInRed)
					if (!vShowThreshold)  //don't add image processings red view if showing processing steps
					//if (!vShowSubtractAverage)  //don't add image processings red view if showing processing steps
					if (!((vShowSubtractAverage) || (vFillInLightSpots) || (vDisplayEnhancedImage)))  
					if (vDisplayingImageData)
					if (!vShowAlternateImage)
					if (vShowingProcess == 0)
					if (vDisplayingImageData->vContainerEjectInspection < vGlobalCurrentProduct->vNumberOfInspections)
					if (!vHideRedIndicator)
					{
						if (vInspectionNumberShowing == vDisplayingImageData->vContainerEjectInspection)
						if ((!vInspectionShowing) || 
							((vInspectionShowing->vInspectionType != cDensityInspection) && 
							(vInspectionShowing->vInspectionType != cCheckWeighInspection) &&
							(vInspectionShowing->vInspectionType != cMissingLidInspection) &&
							(vInspectionShowing->vInspectionType != cUnderfillByWeightInspection) &&
							(vInspectionShowing->vInspectionType != cOverfillByWeightInspection) &&
							(vInspectionShowing->vInspectionType != cUnderfillInspection) &&
							(vInspectionShowing->vInspectionType != cOverfillInspection) &&
							(vInspectionShowing->vInspectionType != cStandardDeviationInspection)))
							AddRedSpotsToBuffer(TempImageIn->pix, vDisplayingImageData, vShowReferenceAdjustedImage, vInspectionShowing);
							//this set all blobs to 1 (blue in color) in the TempImageIn->pix image

						//now find the blobs that are bigger than minimum size and make them red
						//showing red from original image processing when container went through the tunnel
						DWORD TempArea = 0;
						BYTE TempInspectionNumber = 0;
						for (BYTE TempLoopI = 0; TempLoopI < vGlobalCurrentProduct->vNumberOfInspections; TempLoopI++)
						if (vGlobalCurrentProduct->vInspection[TempLoopI] == vInspectionShowing)
						{
							TempInspectionNumber = TempLoopI;
							TempLoopI = vGlobalCurrentProduct->vNumberOfInspections;
						}

						BYTE *TempMaximumPointer = TempImageIn->pix + vLocalSystemData->vITIPCDig->vOriginalBufferSize;
						for (WORD TempLoop = 0; TempLoop < 10; TempLoop++)
						if ((vRejectsImageDisplay) || (!vInspectionShowing) || ((vDisplayingImageData->vRedBoxInspection[TempLoop] >> TempInspectionNumber) & 1))
						{
							if ((vDisplayingImageData->vRedBoxWidth[TempLoop] > 12) && (vDisplayingImageData->vRedBoxHeight[TempLoop] > 12))
							{
								WORD TempXCorner = vDisplayingImageData->vRedBoxCornerX[TempLoop] + 6;
								WORD TempYCorner = vDisplayingImageData->vRedBoxCornerY[TempLoop] + 6;
								WORD TempHeight = vDisplayingImageData->vRedBoxHeight[TempLoop] - 12;
								WORD TempWidth = vDisplayingImageData->vRedBoxWidth[TempLoop] - 12;

								//reference red/blue

								TempYCorner = TempYCorner + vDisplayingImageData->vLeftReferenceAdjust;

								if (TempYCorner < 0)
									TempYCorner = 0;
								if (TempYCorner >= vLocalSystemData->vITIPCDig->vOriginalBufferSizeY)
									TempYCorner = vLocalSystemData->vITIPCDig->vOriginalBufferSizeY - 1;

								for (DWORD TempLoopY = 0; TempLoopY <= TempWidth; TempLoopY++)
								{
									BYTE *TempPointer = TempImageIn->pix + TempXCorner + 
										((TempLoopY + TempYCorner) * vLocalSystemData->vITIPCDig->vOriginalBufferSizeX);

									for (DWORD TempLoopX = 0; TempLoopX <= TempHeight; TempLoopX++)
									if (TempPointer < TempMaximumPointer)
									{
										if (*TempPointer == 1)
											*TempPointer = 0;  //make blue pixel red as in a blob area big enough to meet minimum size
										TempPointer++;
									}
								}
							}
						}
					}

					if (vLocalConfigurationData->vRedIndicatorToShowOnMainMenu == cDefectInRedBox)
					if (!vShowThreshold)  //don't add image processings red view if showing processing steps
					if (!((vShowSubtractAverage) || (vFillInLightSpots) || (vDisplayEnhancedImage)))  
					if (vDisplayingImageData)
					if (!vShowAlternateImage)
					if (vShowingProcess == 0)
					if (vDisplayingImageData->vContainerEjectInspection < vGlobalCurrentProduct->vNumberOfInspections)
					if (!vHideRedIndicator)
					{
						if (vInspectionNumberShowing == vDisplayingImageData->vContainerEjectInspection)
						if ((!vInspectionShowing) || (vRejectsImageDisplay) ||
							((vInspectionShowing->vInspectionType != cDensityInspection) && 
							(vInspectionShowing->vInspectionType != cCheckWeighInspection) &&
							(vInspectionShowing->vInspectionType != cMissingLidInspection) &&
							(vInspectionShowing->vInspectionType != cUnderfillByWeightInspection) &&
							(vInspectionShowing->vInspectionType != cOverfillByWeightInspection) &&
							(vInspectionShowing->vInspectionType != cUnderfillInspection) &&
							(vInspectionShowing->vInspectionType != cOverfillInspection) &&
							(vInspectionShowing->vInspectionType != cStandardDeviationInspection)))
							AddRedBoxToBuffer(TempImageIn->pix, vDisplayingImageData, vShowReferenceAdjustedImage, vInspectionShowing);
					}

					if (vLocalConfigurationData->vRedIndicatorToShowOnMainMenu == cDefectsInRedBoxes)
					//if ((PasswordOK(cTemporaryInspxPassword,false)) || (vLocalConfigurationData->vRedMarkUserAllowedToSee != cNoRedBox))
					if (!vShowThreshold)  //don't add image processings red view if showing processing steps
					//if (!vShowSubtractAverage)  //don't add image processings red view if showing processing steps
					if (!((vShowSubtractAverage) || (vFillInLightSpots) || (vDisplayEnhancedImage)))  
					if (vDisplayingImageData)
					if (!vShowAlternateImage)
					if (!vHideRedIndicator)
					if (vShowingProcess == 0)
					if (vDisplayingImageData->vContainerEjectInspection < vGlobalCurrentProduct->vNumberOfInspections)
					if (vGlobalCurrentProduct->vInspection[vDisplayingImageData->vContainerEjectInspection])
					if (vDisplayingImageData->vRejectedForThisInspection[vInspectionNumberShowing])  //if this image was rejected for the inspection that is showing
					{
						//if (vInspectionNumberShowing == vDisplayingImageData->vContainerEjectInspection)
						if ((!vInspectionShowing) ||  (vRejectsImageDisplay) ||
							((vInspectionShowing->vInspectionType != cDensityInspection) && 
							(vInspectionShowing->vInspectionType != cCheckWeighInspection) &&
							(vInspectionShowing->vInspectionType != cMissingLidInspection) &&
							(vInspectionShowing->vInspectionType != cUnderfillByWeightInspection) &&
							(vInspectionShowing->vInspectionType != cOverfillByWeightInspection) &&
							(vInspectionShowing->vInspectionType != cUnderfillInspection) &&
							(vInspectionShowing->vInspectionType != cOverfillInspection) &&
							(vInspectionShowing->vInspectionType != cStandardDeviationInspection)))
							AddRedBoxesToBuffer(TempImageIn->pix, vDisplayingImageData, vShowReferenceAdjustedImage, vInspectionShowing);
					}
	/*
					if (!(TempShowReferenceAdjustedImage & 1))
					if (!vLocalConfigurationData->vReferenceROIsToEdges)
					if (TempImage)
					{
						TempROILeft = TempROILeft + 
							TempImage->vLeftReferenceAdjust;

						TempROIRight = TempROIRight + 
							TempImage->vLeftReferenceAdjust;
					}
	*/
					if (PasswordOK(cTemporaryInspxPassword,false))
					{
						if (!vOnScreenImageData)
						{
							vOnScreenImageData = new CImageData;
							if (vOnScreenImageData)
								vOnScreenImageData->MakeGreyImage(vLocalSystemData->vITIPCDig->vOriginalBufferSize);
						}
						if ((vOnScreenImageData) && (vOnScreenImageData->vGreyImage))
							CopyMemory(
								vOnScreenImageData->vGreyImage,
								TempImageIn->pix,
								vLocalSystemData->vITIPCDig->vOriginalBufferSize);
					}

					//if (!vGlobalUseHalconLibrary)
					{
						//Save Histogram data for final image displayed on the screen
						Image *TempSubImage = im_rroi(TempImageIn, TempBottom, TempLeft, TempHeight, TempWidth);

						if ((TempSubImage) && (TempSubImage->pix))
						{
#ifdef UseIFCDLLs
							Erflag TempError = mvt_hist(TempSubImage, vHistogramData);

							TempError = im_delete(TempSubImage);
							if (TempError == IM_OK)
								TempSubImage = NULL;
#endif
						}
					}
					//10/5/2009
					vLocalSystemData->vITIPCDig->ScaleImageSideways(
						vLocalSystemData->vITIPCDig->vDisplayImageSizeX,
						vLocalSystemData->vITIPCDig->vDisplayImageSizeY, 
						1,1,0,0,TempImageIn->pix,vLocalConfigurationData->vPixelsPerDetector, 
						(vLocalConfigurationData->vImageShowScale && (!vRejectsImageDisplay)));

					if (vLocalSystemData->vITIPCDig->vImageBuffer)
					if (vLocalSystemData->vITIPCDig->vImageBufferWithNoLines)
						CopyMemory(
							vLocalSystemData->vITIPCDig->vImageBufferWithNoLines,
							vLocalSystemData->vITIPCDig->vImageBuffer,
							vLocalSystemData->vITIPCDig->vDisplayImageSizeX *
							vLocalSystemData->vITIPCDig->vDisplayImageSizeY);

					//Add Lines if needed;
					if ((!vRejectsImageDisplay) && (!vImproveLearnMenu))
					if (vLocalConfigurationData->vImageShowLines)
					if ((!vShowAlternateImage) || (vShowAverage) || (vShowHistogram))
						vLocalSystemData->vITIPCDig->AddLinesToImageSideways(
							vLocalSystemData->vITIPCDig->vDisplayImageSizeX,
							vLocalSystemData->vITIPCDig->vDisplayImageSizeY, 
							0, 0, 0, vShowReferenceAdjustedImage, vDisplayingImageData, vInspectionShowing, vLocalConfigurationData->vScanTracType);
					/*
					if ((!vRejectsImageDisplay) && (!vImproveLearnMenu))
					if (vLocalConfigurationData->vImageShowScale)
						vLocalSystemData->vITIPCDig->AddScaleToImageSideways(
							vLocalSystemData->vITIPCDig->vDisplayImageSizeX,
							vLocalSystemData->vITIPCDig->vDisplayImageSizeY, 
							1, 0, 0);
							*/
	
					if (vDisplayingImageData)
					if (!vImproveLearnMenu)
					if ((vLocalConfigurationData->vImageShowFixedROILines) || (vEditROIBoundaries))
						//5/16/2007
					if ((!vRejectsImageDisplay) || 
						(vDisplayingImageData->vContainerEjectReason == vInspectionNumberShowing + cEjectInspectionOffset))
					{
						if (vInspectionShowing)
						{
							if (vEditROIBoundaries)
							{
								vLocalSystemData->vITIPCDig->AddFixedROILinesToImageSideways(
									vLocalSystemData->vITIPCDig->vDisplayImageSizeX,
									vLocalSystemData->vITIPCDig->vDisplayImageSizeY, 
									0, 0, 0,vShowReferenceAdjustedImage,vDisplayingImageData,
									vInspectionShowing,vGlobalCurrentProduct->vOverScanMultiplier,
									vLocalConfigurationData->vReferenceROIsToEdges, vPlotLine);
							}
							else
								vLocalSystemData->vITIPCDig->AddFixedROILinesToImageSideways(
									vLocalSystemData->vITIPCDig->vDisplayImageSizeX,
									vLocalSystemData->vITIPCDig->vDisplayImageSizeY, 
									0, 0, 0,
									//vShowReferenceAdjustedImage | (vInspectionShowing->vReferenceToTopLeft << 1),
									vShowReferenceAdjustedImage,
									vDisplayingImageData,
									vInspectionShowing,vGlobalCurrentProduct->vOverScanMultiplier,
									vLocalConfigurationData->vReferenceROIsToEdges, vPlotLine);
						}
						/* 5/16/2007
						else
						{
							vLocalSystemData->vITIPCDig->AddFixedROILinesToImageSideways(
								vLocalSystemData->vITIPCDig->vDisplayImageSizeX,
								vLocalSystemData->vITIPCDig->vDisplayImageSizeY, 
								1, 0, 0,
								vShowReferenceAdjustedImage,
								vDisplayingImageData,
								vInspectionShowing,vGlobalCurrentProduct->vOverScanMultiplier,
								vLocalConfigurationData->vReferenceROIsToEdges, vPlotLine);
						}
						*/
					}

					UpdateDisplay(vDisplayingImageData);
					UpdateDisplayIndicators();
					//if (!vShowThreshold)  //if your not showing the threshold step, calculate image data for reject information
					//	CalculateImageDataInformation();

					RECT TempRectangle;
					TempRectangle.left = TempOffsetX;
					TempRectangle.top = TempOffsetY;
					TempRectangle.right = TempOffsetX + vLocalSystemData->vITIPCDig->vDisplayImageSizeX;
					TempRectangle.bottom = TempOffsetY + vLocalSystemData->vITIPCDig->vDisplayImageSizeY;
					if (!vWaitForScreenUpdate)
						this->InvalidateRect(&TempRectangle,false);
				}
			}
			else
				ReportErrorMessage("Error, MVT Could not create Image for Image Enhancement", cEMailInspx,32000);
			if (TempImageOut)
			{
				im_delete(TempImageOut); 
				TempImageOut = NULL;
			}
			if (TempImageIn)
			{
				im_delete(TempImageIn); 
				TempImageIn = NULL;
			}
			if (vInspectionShowing)
			if ((vDisplayingImageData) && (vDisplayingImageData->vGreyImage))
			if ((vInspectionShowing->vInspectionType == cItemCountInspection) || (vInspectionShowing->vInspectionType == cSizeInspection) || (vInspectionShowing->vInspectionType == cMissingLidInspectionOnSize))
			{
				DWORD TempRedPixels = 0;
				WORD TempBottom = 0;
				WORD TempLeft = 0;
				WORD TempHeight = 0;
				WORD TempWidth = 0;
				vLocalSystemData->vITIPCDig->GetInspectionROIBounds(vInspectionShowing, 
					&TempBottom, &TempLeft, &TempHeight, &TempWidth, vDisplayingImageData->vLeftReferenceAdjust, 
					vLocalConfigurationData->vReferenceROIsToEdges, 
					((vDisplayingImageData->vContainerLearnState) && (vLocalConfigurationData->vAutoImproveEnabled) && 
					(vInspectionShowing->vAddStructure) &&
					((vLocalConfigurationData->vScanTracType == cSoloScanTrac) || (vLocalConfigurationData->vScanTracType == cSoloPlusScanTrac) || (vLocalConfigurationData->vScanTracType == cPiccoloScanTrac))));

				BYTE *TempPointer = NULL;
				for (WORD TempXLoop = 0; TempXLoop < TempWidth; TempXLoop++)
				{
					TempPointer = vDisplayingImageData->vGreyImage + 
						TempBottom + (TempLeft + TempXLoop) * vLocalSystemData->vITIPCDig->vOriginalBufferSizeX;

					if ((vShowAverage) && (vInspectionShowing) && (vInspectionShowing->vSubtractAverage) &&
						(vLocalSystemData->vITIPCDig->vProductAverageImage) && (vLocalSystemData->vITIPCDig->vProductAverageImage->vGreyImage))
					{
						TempPointer = vLocalSystemData->vITIPCDig->vProductAverageImage->vGreyImage + 
							TempBottom + (TempLeft + TempXLoop) * vLocalSystemData->vITIPCDig->vOriginalBufferSizeX;
					}

					for (WORD TempYLoop = 0; TempYLoop < TempHeight; TempYLoop++)
					{
						if (*TempPointer <= vThreshold)  
						{//if pixel is  darker than threshold
							TempRedPixels++;
						}
						TempPointer++;
					}
				}
			}
			// 8/14/2006
			/*
			if (TempDisplayImageData)
			{
				if (TempDisplayImageData == vDisplayingImageData)
					vDisplayingImageData = NULL;

				delete TempDisplayImageData;
				TempDisplayImageData = NULL;
			}
			*/
		}
		if ((vGlobalCurrentProduct) && ((vLocalConfigurationData->vScanTracType == cSoloScanTrac) || (vLocalConfigurationData->vScanTracType == cSoloPlusScanTrac) || (vLocalConfigurationData->vScanTracType == cPiccoloScanTrac)) &&
			((!vLocalConfigurationData->vContinuousFeedContainerRate) && (!vLocalConfigurationData->vBulkProductMode) && (!vGlobalCurrentProduct->vBulkProductMode)) && //Not Continuous Feed, and Not a Pipeline
			(vLocalConfigurationData->vShapeHandlingWidthTolerancePixels) && (vGlobalCurrentProduct->vReferenceWidth) && (!vShowAlternateImage) && (vDisplayingImageData))
		{
			int TempWidthDelta = vDisplayingImageData->vWidth - vGlobalCurrentProduct->vReferenceWidth;
			if (TempWidthDelta < 0)
				TempWidthDelta = -TempWidthDelta;
			if (TempWidthDelta > vLocalConfigurationData->vShapeHandlingWidthTolerancePixels)
			{
				CString TempText = "Wrong Width";
				BYTE TempLength = TempText.GetLength();
				m_RejectReason.SetWindowPos(NULL,ResizeXCoor(cMainBackgroundLeft + 2 + 1),	ResizeYCoor(cMainBackgroundTop + 2 - 1), ResizeXCoor(TempLength * 10),ResizeYCoor(19),SWP_NOZORDER);
		//		::SetupItem(&m_RejectReason);
				SetDlgItemText(IDC_RejectReason,TempText);
				m_RejectReason.ShowWindow(SW_SHOW);
			}
			else
				m_RejectReason.ShowWindow(SW_HIDE);
		}
		else
			m_RejectReason.ShowWindow(SW_HIDE);
	}
	else
	{
		m_Background.ShowWindow(SW_SHOW);
		m_ErrorMessage.ShowWindow(SW_SHOW);
		UpdateDisplay(NULL);
	}
	double TempEndTime = CIFCOS::GetSystimeMicrosecs();
	vLocalSystemData->vITIPCDig->vLastDisplayTime = TempEndTime - TempStartTime;
	if (vAnalysisImageDisplay)  //may need to update the Add Last Reject to learn button if show a new image
		UpdateButtons();
}

void CNewImageDisplayDialog::SaveImageToFile(bool TempSaveImageOnScreen, bool TempShowNameDialog)
{
	// Save Image button pressed
	if ((!vLocalConfigurationData->vDemoMode) || (PasswordOK(cTemporaryInspxPassword,false)))
	if ((vDisplayingImageData) && (vDisplayingImageData->vGreyImage) && (vHaveAnImageToShow))
	{
		if (vShowingLiveImages)
			StopShowingLiveImages();

		CString TempStringSSD = vLocalConfigurationData->vScanTracImagesDirectory;
		TempStringSSD.Delete(TempStringSSD.GetLength() -1, 1);
		LPTSTR TempStringSSDP = TempStringSSD.GetBuffer(TempStringSSD.GetLength());
		CFileStatus TempFileStatus;
		if (CFile::GetStatus(TempStringSSDP, TempFileStatus))
		{
			CAlphaEntryDialog IAlphaEntryDialog;  
			IAlphaEntryDialog.vLocalSystemData = vLocalSystemData;
			//Set dialog box data titles and number value
			IAlphaEntryDialog.vMainWindowPointer = vMainWindowPointer;
			CTime TempTime = CTime::GetCurrentTime();
			CString TempTimeString = TempTime.Format("%Y-%m-%d ");
			TempTimeString = TempTimeString + vDisplayingImageData->vTime;
			TempTimeString.Replace(':', '-');
			CString TempString = vLocalSystemData->vCurrentProductName + " " + TempTimeString + ".BMP";
			IAlphaEntryDialog.vEditString = TempString;
			CString TempText("");
			TempText.LoadString(IDS_FileNameToSaveImageAs);
			IAlphaEntryDialog.m_DialogTitleStaticText = TempText;
			IAlphaEntryDialog.vAllowPeriod = true;
			IAlphaEntryDialog.vAllowMoreThan32Characters = true;

			TempText.LoadString(IDS_OriginalValue);
			IAlphaEntryDialog.m_DialogTitleStaticText2 = TempText + ": " + IAlphaEntryDialog.vEditString;
			//Pass control to dialog box and display
			int nResponse = IDOK;
			if (TempShowNameDialog)
				nResponse = IAlphaEntryDialog.DoModal();
			//dialog box is now closed, if user pressed select do this
			//if user pressed cancel, do nothing
			if (nResponse == IDOK)
			{
				CString TempString(IAlphaEntryDialog.vEditString);
				int TempLength = TempString.GetLength();	
				BYTE TempPosition = TempString.Find(_T(".BMP"));
				if ((TempPosition == 255) || (TempPosition != (TempLength - 4)))
				{
					/*
					BYTE TempDotPosition = TempString.Find(".");
					if (TempDotPosition < 255)
					{
						TempString.Delete(TempDotPosition,TempLength - TempDotPosition);
					}
					BYTE TempLength1 = TempString.GetLength();
					*/
					if (!TempLength)
						TempString = "Inspx Image.BMP";
					else
						TempString = TempString + ".BMP";
				}
				TempString = vLocalConfigurationData->vScanTracImagesDirectory + TempString;
				TempLength = TempString.GetLength();
				LPTSTR TempString1 = _T("Test");
				bool TempOKToWriteFile = false;
				if (TempLength > MAX_PATH)
				{
					CNoticeDialog TempNoticeDialog;
					TempText.LoadString(IDS_FileNamemorethe83charactersfilesaveaborted);//"\n\n\nFile Name more the MAX_PATH characters, file save aborted"
					CString TempString = TempText;
					TempNoticeDialog.vNoticeText = TempString;
					TempNoticeDialog.vType = cErrorMessage;
					TempNoticeDialog.DoModal();
					TempLength = MAX_PATH;
				}
				else
				{
					TempString1 = TempString.GetBuffer(MAX_PATH); //TempString.GetLength());
					TempOKToWriteFile = true;

					CFileStatus TempFileStatus;
					if (CFile::GetStatus(TempString1, TempFileStatus))
					{
						CYesNoDialog TempYesNoDialog;
						CString TempString("\n\nFile: ");
						TempString = TempString + TempString1 + " already exists.\nDelete and save new image to file?";
						TempYesNoDialog.vNoticeText = TempString;
						TempYesNoDialog.vYesButtonText = "Overwrite File";
						TempYesNoDialog.vNoButtonText = "Cancel";
						TempYesNoDialog.vQuestionType = cConfirmSave;
						int TempResult = TempYesNoDialog.DoModal();
						if (TempResult != IDOK)
							TempOKToWriteFile = false;
					}
				}

				if (TempOKToWriteFile)
				{
					//OutputImageROIToCSVFile(TempString1);
					CW2A TempFileNameString(TempString1);

					CImgFile *TempImageFile=IfxCreateImgFile(TempFileNameString);
					if (TempImageFile) 
					{
						if ((TempSaveImageOnScreen) && (vOnScreenImageData) && (vOnScreenImageData->vGreyImage))
						{
							if (!TempImageFile->WriteFile(vOnScreenImageData->vGreyImage,
								vLocalSystemData->vITIPCDig->vOriginalBufferSizeX,
								vLocalSystemData->vITIPCDig->vOriginalBufferSizeY,
								(WORD)vLocalSystemData->vITIPCDig->vBitsPerPixel))
							{
								ReportErrorMessage("Failed to Write Image to File", cEMailMaintenance,0);
							}
						}
						else
						{
							if (!TempImageFile->WriteFile(vDisplayingImageData->vGreyImage,
								vLocalSystemData->vITIPCDig->vOriginalBufferSizeX,
								vLocalSystemData->vITIPCDig->vOriginalBufferSizeY,
								(WORD)vLocalSystemData->vITIPCDig->vBitsPerPixel))
							{
								ReportErrorMessage("Failed to Write Image to File", cEMailMaintenance,0);
							}
							else
							{
								CNoticeDialog TempNoticeDialog;
								TempText = "\n\nWrote Image to file: \n";
								TempText = TempText + TempString1;
								CString TempString = TempText;
								TempNoticeDialog.vNoticeText = TempString;
								TempNoticeDialog.vType = cNoticeMessage;
								TempNoticeDialog.vAutoClose = 1000;
								TempNoticeDialog.DoModal();
							}
						}
						delete TempImageFile;
					}
				}
			}
		}
		else
		{
			CNoticeDialog TempNoticeDialog1;
			CString TempText = "\n\nCannot write image to file\nCannot access folder on Server";
			TempNoticeDialog1.vNoticeText = TempText;
			TempNoticeDialog1.vType = cErrorMessage;
			TempNoticeDialog1.DoModal();
		}
	}
	else
		ShowNeedImageDialogMessage();
}

void CNewImageDisplayDialog::LoadImageFromFile()
{
	// Load Image button pressed
	if ((vLocalSystemData->vLoadedFileAvailable) || (vShowNextFrame))
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\n\nLoading previous file, please wait.";
		TempNoticeDialog.vType = cErrorMessage;
		TempNoticeDialog.DoModal();
	}
	else
	if ((!vLocalConfigurationData->vDemoMode) || (PasswordOK(cTemporaryInspxPassword,false)))
	{
		if (vLocalSystemData->vLoadedFileImage)
		{
			im_delete(vLocalSystemData->vLoadedFileImage);
			vLocalSystemData->vLoadedFileImage = NULL;
		}
	/*
typedef struct _WIN32_FIND_DATA {
  DWORD    dwFileAttributes; 
  FILETIME ftCreationTime; 
  FILETIME ftLastAccessTime; 
  FILETIME ftLastWriteTime; 
  DWORD    nFileSizeHigh; 
  DWORD    nFileSizeLow; 
  DWORD    dwReserved0; 
  DWORD    dwReserved1; 
  TCHAR    cFileName[ MAX_PATH ]; 
  TCHAR    cAlternateFileName[ 14 ]; 
} WIN32_FIND_DATA, *PWIN32_FIND_DATA; 
Members
	*/
		WIN32_FIND_DATA FindFileData;
		HANDLE hFind;

		CString TempString(vLocalConfigurationData->vScanTracImagesDirectory);
		TempString = TempString + "*.BMP";
		LPTSTR TempDirectoryName = TempString.GetBuffer(TempString.GetLength());

		hFind = FindFirstFile(TempDirectoryName, &FindFileData);

		if (hFind == INVALID_HANDLE_VALUE) 
		{
			//int TimerResult = vMainWindowPointer->SetTimer(vMainWindowPointer->vFlashYellowLightTimerHandle,500,NULL);
			//vMainWindowPointer->vCantInspectError = true; //flash red + yellow

			CNoticeDialog TempNoticeDialog;
			CString TempText("");
			TempText.LoadString(IDS_NoImageFilesAvailableToLoad);
			TempNoticeDialog.vNoticeText = TempText;//"Invalid Handle";
			TempNoticeDialog.vType = cErrorMessage;
			TempNoticeDialog.DoModal();
		} 
		else 
		{
			CSelectItemDialog ISelectItemDialog;  
			ISelectItemDialog.vNumberOfItems = 1;
			ISelectItemDialog.vItems[0] = FindFileData.cFileName;

			while (FindNextFile(hFind,&FindFileData))
			{
				if (ISelectItemDialog.vNumberOfItems < cMaxSelectDialogItems)
				{
					ISelectItemDialog.vItems[ISelectItemDialog.vNumberOfItems] = FindFileData.cFileName;
					ISelectItemDialog.vNumberOfItems++;
				}
			}

			FindClose(hFind);

			//Set dialog box data titles and number value
			CString TempText("");
			TempText.LoadString(IDS_ChooseImageFileToLoad);
			ISelectItemDialog.vTitleString = TempText;
			ISelectItemDialog.vBackgroundColor = cYellow;

			ISelectItemDialog.m_DialogTitleStaticText2 = "";
			ISelectItemDialog.m_DialogTitleStaticText3 = "";
			//Pass control to dialog box and display
			int nResponse = ISelectItemDialog.DoModal();
			//dialog box is now closed, if user pressed select do this
			//if user pressed cancel, do nothing
			if (nResponse == IDOK)
			{
				CString TempResultString = ISelectItemDialog.vItems[ISelectItemDialog.vItemNumberSelected - 1];
				TempResultString = vLocalConfigurationData->vScanTracImagesDirectory + TempResultString;
				int TempHeight = 0;
				int TempWidth = 0;
				int TempBytesPerPixel = 0;
				int TempType = 0;

				CW2A TempFileNameString(TempResultString);
				Erflag TempError = im_file_get_size(TempFileNameString, IM_BMP_FILE, &TempWidth, &TempHeight, &TempBytesPerPixel, &TempType);
				if (TempError == IM_OK)
				if (ThereIsEnoughMemory((vLocalSystemData->vITIPCDig->vOriginalBufferSize + sizeof(Image)), "Load File Buffer"))
				{
					vLocalSystemData->vLoadedFileImage = im_create(IM_BYTE, 
						vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, 
						vLocalSystemData->vITIPCDig->vAquisitionSizeY);

					if ((vLocalSystemData->vLoadedFileImage) && (vLocalSystemData->vLoadedFileImage->pix))
					{
						BYTE *TempPointer = vLocalSystemData->vLoadedFileImage->pix;

						for (DWORD TempLoopX = 0; TempLoopX < vLocalSystemData->vITIPCDig->vOriginalBufferSize; TempLoopX++)
							*TempPointer++ = 255;

						TempError = im_file_read(vLocalSystemData->vLoadedFileImage, TempFileNameString, IM_AUTO_FILE);
						if (TempError == IM_OK)
						{
							mvt_mirror_y(vLocalSystemData->vLoadedFileImage);

							vGlobalCursorSave = ::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_WAIT)));
							vLocalSystemData->vShowImages = false;
							vShowNextFrame = true;
							//don't show images for a second so screen ready to draw next image
							int TimerResult = SetTimer(vFinishLoadingFileTimerHandle,1200,NULL);
							if (!TimerResult)
								ReportErrorMessage("Error-FinishLoadingFile Timer Failed",cEMailInspx,32000);

							ReportErrorMessage("Loaded Image From File by operator: " + TempResultString, cUserAction,0);

							//SetCursorPos(320,230);
							//TimerResult = SetTimer(vCursorUpdateTimerHandle,10,NULL);
							//if (!TimerResult)
							//	ReportErrorMessage("Error-Cursor Wait Timer Failed",cEMailInspx,32000);
						}
						else
						{
							CString TempText("");
							CString TempText1("");
							CString TempText2("");
							TempText.LoadString(IDS_ImageFile);
							TempText1.LoadString(IDS_CouldNotBeRead);
							TempText2.LoadString(IDS_Error);
							CString TempString1 = "\n" + TempText + ": " + TempResultString + "\n" + TempText1 + "\n" + TempText2 + ": " + dtoa(TempError, 0);
							CNoticeDialog TempNoticeDialog;
							TempNoticeDialog.vNoticeText = TempString1;
							TempNoticeDialog.vType = cErrorMessage;
							TempNoticeDialog.DoModal();
						}
					}
				}
				else
				{
					CString TempText("");
					CString TempText1("");
					CString TempText2("");
					TempText.LoadString(IDS_ImageFile);
					TempText1.LoadString(IDS_CouldNotBeFound);
					TempText2.LoadString(IDS_Error);
					CString TempString1 = "\n" + TempText + ": " + TempResultString + "\n" + TempText1 + "\n" + TempText2 + ": " + dtoa(TempError, 0);
					CNoticeDialog TempNoticeDialog;
					TempNoticeDialog.vNoticeText = TempString1;
					TempNoticeDialog.vType = cErrorMessage;
					TempNoticeDialog.DoModal();
				}
			}
			else 
			if (nResponse == 10)
			{
				//Main Menu button pressed
				Sleep(1); //is the equivelent of a yeild statement;
				PrepareAndExit(10);
			}
		}
	}
	/*
	else
	{
		CNoticeDialog TempNoticeDialog;
		TempText.LoadString(IDS_SystemMustBeRunningToShowImages);
		TempNoticeDialog.vNoticeText = TempText;
		TempNoticeDialog.vType = cErrorMessage;
		TempNoticeDialog.DoModal();
	}
	*/
}

void CNewImageDisplayDialog::FinishLoadingFile()
{
	vShowingLiveImages = false;
	//vLocalSystemData->vITIPCDig->vCameraLive = false;
	vLocalSystemData->vITIPCDig->vProcessNextImage = true;
	vProcessNextImage = true;
	
	vLocalSystemData->vITIPCDig->vRejectBufferShowing = -1;
	vShowClipboard = false;
	vShowAverage = false;
	vShowBackground = false;
	vShowStructure = false;
	vShowHistogram = false;
	//vShowGlassStructure = false;
	//vShowGlassHistogram = false;
	vShowNextFrame = true;
	vLocalSystemData->vLoadedFileAvailable = true;
	//allow screen to draw this image
	vLocalSystemData->vShowImages = true;
}

void CNewImageDisplayDialog::UpdateDisplayIndicators() 
{
	CString TempText("");
	CString TempText1("");

	if (vDisplayingImageData)
	if ((vDisplayingImageData->vContainerEjectReason < cMaximumRejectReason) && 
		((vInspectionShowing) && (vDisplayingImageData->vContainerEjectInspection < vGlobalCurrentProduct->vNumberOfInspections) &&
		(vInspectionShowing != vGlobalCurrentProduct->vInspection[vDisplayingImageData->vContainerEjectInspection])))
		TempText = TempText + vInspectionShowing->vName + ": ";

	if ((vShowClipboard) && (PasswordOK(cTemporaryInspxPassword,false)))
		TempText = TempText + "Clipboard ";

	if ((vShowAverage) && (PasswordOK(cTemporaryInspxPassword,false)))
		TempText = TempText + "Average ";

	if ((vShowBackground) && (PasswordOK(cTemporaryInspxPassword,false)))
		TempText = TempText + "Background ";

	if (//(vShowGlassStructure) || 
			(vShowStructure) || ((vUseVariabiltyImage) && (vUseVariabiltyImage < 4)) ||
			(vShowOriginalStructure) || (vShowImproveLearnPreview) ||
			(vShowDeltaOriginalToCurrentStructure) || (vShowDeltaCurrentPreviewStructure))
	{
		if (vShowOriginalStructure)	
		{
			TempText = TempText + "Original Structure";
			if (vShowType == cShowTypeContaminant2)
				TempText = TempText + "2";
		}
		else
		if (vShowImproveLearnPreview)	
		{
			TempText = TempText + "Improve Preview";
			if (vShowType == cShowTypeContaminant2)
				TempText = TempText + "2";
		}
		else
		if (vShowDeltaOriginalToCurrentStructure)	
		{
			TempText = TempText + "Delta Original-Current";
			if (vShowType == cShowTypeContaminant2)
			{
				TempText.Delete(TempText.GetLength() - 3, 3);
				TempText = TempText + "2";
			}
		}
		else
		if (vShowDeltaCurrentPreviewStructure)	
		{
			TempText = TempText + "Delta Curr-Improve";
			if (vShowType == cShowTypeContaminant2)
				TempText = TempText + "2";
		}
		else
		if (vShowType == cShowTypeVoid)	
			TempText = TempText + "Void Structure";
		else
		if (vShowType == cShowTypeContaminant2NoDilation)	
			TempText = TempText + "Contam2 NoDilate";
		else
		if (vShowType == cShowTypeContaminant2)	
			TempText = TempText + "Contam2 Structure";
		else
			TempText = TempText + "Current Structure";

		if (!vHideRedIndicator)
		{
			if ((vShowType == cShowTypeContaminant2) || (vShowType == cShowTypeContaminant2NoDilation))
				TempText = TempText + " 0=bla,1-45=blu,46-90=g,91-135=y,136-254=o,255=w,larger=brighter";
			else
			{
				if ((vShowDeltaCurrentPreviewStructure) || ((vUseVariabiltyImage) && (vUseVariabiltyImage < 4)))
					TempText = TempText + " 0=bla,1-5=blu,6-10=g,11-20=y,21-40=o,41-255=w,larger=brighter";
				else
					TempText = TempText + " 0=bla,1-20=blu,21-30=g,31-50=y,51-126=o,127-255=w,larger=brighter";
			}
		}
	}

	if ((vShowHistogram) && (PasswordOK(cTemporaryInspxPassword,false)))
		TempText = TempText + "Histogram ";

	//if ((vShowGlassHistogram) && (PasswordOK(cTemporaryInspxPassword,false)))
	//	TempText = TempText + "Glass Histogram ";

	//if ((vShowGlassStructure) && (PasswordOK(cTemporaryInspxPassword,false)))
	//	TempText = TempText + "Glass Structure 0=bla,1-20=blu,21-30=g,31-50=y,51-126=o,127=w,larger=brighter";

	if ((vShowReferenceAdjustedImage) && (!vShowAlternateImage) && (PasswordOK(cSuperInspxPassword,false)))
		TempText = TempText + "Reference ";

	if ((vWhiteOutAnythingTouchingTheTop) && (!vShowAlternateImage) && (PasswordOK(cSuperInspxPassword,false)))
		TempText = TempText + "Remove Top Item ";
	
	if ((vFillBackground) && (!vShowAlternateImage) && (PasswordOK(cSuperInspxPassword,false)))
		TempText = TempText + "Background ";
	
	if ((vShowErodeCount) && (!vShowAlternateImage) && (PasswordOK(cTemporaryInspxPassword,false)))
			TempText = TempText + "Erode ";

	if ((vShowDilationCount) && (!vShowAlternateImage) && (PasswordOK(cTemporaryInspxPassword,false)))
			TempText = TempText + "Dilate ";

	if ((vShowErodeCountAtStart) && (!vShowAlternateImage) && (PasswordOK(cTemporaryInspxPassword,false)))
			TempText = TempText + "ErodeS ";

	if ((vPreSmoothImage) && (!vShowAlternateImage) && (PasswordOK(cSuperInspxPassword,false)))
		TempText = TempText + "Smooth ";

	if ((vUseVariabiltyImage == 1) && (!vShowAlternateImage) && (PasswordOK(cSuperInspxPassword,false)))
		TempText = TempText + "Var1 ";
	else
	if ((vUseVariabiltyImage == 2) && (!vShowAlternateImage) && (PasswordOK(cSuperInspxPassword,false)))
		TempText = TempText + "Var2 ";
	else
	if ((vUseVariabiltyImage == 3) && (!vShowAlternateImage) && (PasswordOK(cSuperInspxPassword,false)))
		TempText = TempText + "Var3 ";
	else
	if ((vUseVariabiltyImage) && (!vShowAlternateImage) && (PasswordOK(cSuperInspxPassword,false)))
		TempText = TempText + "Var ";
	if ((vRemoveJarPunt >= 2) && (!vShowAlternateImage) && (PasswordOK(cSuperInspxPassword,false)))
		TempText = TempText + "PuntM ";
	else
	if ((vRemoveJarPunt) && (!vShowAlternateImage) && (PasswordOK(cSuperInspxPassword,false)))
		TempText = TempText + "Punt ";

	if ((vTrimEdgesOfJarAtROI) && (!vShowAlternateImage) && (PasswordOK(cSuperInspxPassword,false)))
		TempText = TempText + "Trim ";

	//if ((vFillInLightSpots == 0xAA) && (!vShowAlternateImage) && (PasswordOK(cSuperInspxPassword,false)))
	//	TempText = TempText + "Fill Light1 ";
	//else
	if ((vFillInLightSpots) && (!vShowAlternateImage) && (PasswordOK(cSuperInspxPassword,false)))
		TempText = TempText + "Fill Light ";

	//if ((vPreSelectDarkSpotsInImage) && (!vShowAlternateImage) && (PasswordOK(cSuperInspxPassword,false)))
	//	TempText = TempText + "Select Dark ";

	if ((vTakeDerivative) && (!vShowAlternateImage) && (PasswordOK(cSuperInspxPassword,false)))
		TempText = TempText + "Derivative ";
	
	if ((vTakeDifferenceFromMedianFilter) && (!vShowAlternateImage) && (PasswordOK(cSuperInspxPassword,false)))
	{
		if (vTakeDifferenceFromMedianFilter < 128)
			TempText = TempText + "DifMedian" + dtoa(vTakeDifferenceFromMedianFilter, 0) + " ";
		else
			TempText = TempText + "Median " + dtoa(vTakeDifferenceFromMedianFilter & 0x3F, 0) + " ";
	}
	
	if ((vBonesInspection) && (!vShowAlternateImage) && (PasswordOK(cSuperInspxPassword,false)))
		TempText = TempText + "LBones ";
	
	if ((vTakeDifferenceFromMedian) && (!vShowAlternateImage) && (PasswordOK(cSuperInspxPassword,false)))
		TempText = TempText + "Bones ";
	
	if ((vFindSpotInSubROI) && (!vShowAlternateImage) && (PasswordOK(cSuperInspxPassword,false)))
		TempText = TempText + "Spot in ROI ";
	
	if ((vFindDarkSpotOnLine) && (!vShowAlternateImage) && (PasswordOK(cSuperInspxPassword,false)))
		TempText = TempText + "Spot on Line ";
	
	if ((vRequire2InARow) && (!vShowAlternateImage) && (PasswordOK(cSuperInspxPassword,false)))
		TempText = TempText + "2InRow ";

	if ((vShowLineDerivative) && (!vShowAlternateImage) && (PasswordOK(cSuperInspxPassword,false)))
		TempText = TempText + "Glass ";

	if (vLookForDarkSpots)
		TempText = TempText + "Wire ";// + dtoa(vInspectionShowing->vPixelLookAhead) + ", " + dtoa(vInspectionShowing->vOverlap, 0) + " ";

	if ((vDoContaminant22Algorithm) && (!vShowAlternateImage) && (PasswordOK(cSuperInspxPassword,false)))
		TempText = TempText + "Contaminant22 ";
	
	if ((vShowSubtractAverage) && (!vShowAlternateImage) && (PasswordOK(cSuperInspxPassword,false)))
	if ((vInspectionShowing) && (vInspectionShowing->vSubtractAverage))
		TempText = TempText + "-Average ";
	else
		TempText = TempText + "-Average (Display Only) ";

	if ((vShowAddStructure) && (!vShowAlternateImage) && (PasswordOK(cSuperInspxPassword,false)))
		TempText = TempText + "+Structure ";

	//if ((vShowImageFactors) && (vShowOriginalImageFactor) && (!vShowAlternateImage) &&
	//	(vShowEnhancedImageFactor)  && (PasswordOK(cSuperInspxPassword,false)))
	//	TempText = TempText + "Image Factors ";

	if ((vIPContaminant) && (!vDisplayEnhancedImage) && (!vShowAlternateImage) && (PasswordOK(cTemporaryInspxPassword,false)))
		TempText = TempText + "IP Mask ";
		
	if ((vDisplayEnhancedImage) && (!vShowAlternateImage) && (PasswordOK(cSuperInspxPassword,false)))
		TempText = TempText + "Enhanced ";

	if ((vShowErodeCountAtEnd) && (!vShowAlternateImage) && (PasswordOK(cTemporaryInspxPassword,false)))
		TempText = TempText + "Erode ";

	if ((vShowDilationCountAtEnd) && (!vShowAlternateImage) && (PasswordOK(cTemporaryInspxPassword,false)))
		TempText = TempText + "Dilate ";

	if ((vShowAddHistogram) && (!vShowAlternateImage) && (PasswordOK(cSuperInspxPassword,false)))
		TempText = TempText + "+Histogram ";

	if ((vShowThreshold) && (PasswordOK(cSuperInspxPassword,false)))
	{
		TempText = TempText + "Threshold ";

		if ((vIPContaminant) && (vDisplayEnhancedImage) && (!vShowAlternateImage) && (PasswordOK(cTemporaryInspxPassword,false)))
			TempText = TempText + "IP Cont ";
	}

	if ((vShowThreshold) && (!PasswordOK(cSuperInspxPassword,false)))
	{
		TempText1.LoadString(IDS_ProcessedImage);
		TempText = TempText + TempText1 + " ";
	}

	if (!vShowAlternateImage)
	if ((vShowingProcess == 0) || (vShowThreshold))
	if (!vHideRedIndicator)
	if ((vLocalConfigurationData->vRedIndicatorToShowOnMainMenu == cDefectsInRed) || //&& 
		((vShowThreshold) && (vLocalConfigurationData->vRedIndicatorToShowOnMainMenu != cDefectsInPseudoColor) &&
		(vLocalConfigurationData->vRedIndicatorToShowOnMainMenu != cDefectsInNoColor)))
	{

		TempText1.LoadString(IDS_DefectsInRed);
		TempText = TempText + TempText1 + " ";
	}
	else
	if (vLocalConfigurationData->vRedIndicatorToShowOnMainMenu == cDefectsInPseudoColor)//)
	{
		TempText1.LoadString(IDS_DefectsInColor);
		if (TempText.GetLength() < 30)
		{
			if (PasswordOK(cTemporaryInspxPassword,false))
				TempText = TempText + TempText1 + ": Blue=1-15, Green=16-35, Orange=36-65, Red=66+ ";
			else
				TempText = TempText + TempText1 + ": Blue=Faint, Green=Light, Orange=Moderate, Red=Strong ";
		}
		else
		{
			if (PasswordOK(cTemporaryInspxPassword,false))
				TempText = TempText + TempText1 + ":B=1-15,G=16-35,O=36-65,R=66+ ";
			else
				TempText = TempText + TempText1 + ":B=Faint,G=Light O=Moderate R=Strong ";
		}
	}
	else
	if ((vLocalConfigurationData->vRedIndicatorToShowOnMainMenu == cDefectInRedBox) && (vShowingProcess == 0))
	//if ((PasswordOK(cTemporaryInspxPassword,false)) || (vLocalConfigurationData->vRedMarkUserAllowedToSee != cNoRedBox))
	{
		TempText1.LoadString(IDS_DefectInRedBox);
		TempText = TempText + TempText1 + " ";
	}
	else
	if ((vLocalConfigurationData->vRedIndicatorToShowOnMainMenu == cDefectsInRedBoxes) && (vShowingProcess == 0))
	//if ((PasswordOK(cTemporaryInspxPassword,false)) || (vLocalConfigurationData->vRedMarkUserAllowedToSee != cNoRedBox))
	{
		TempText1.LoadString(IDS_DefectsInRedBoxes);
		TempText = TempText + TempText1 + " ";
	}

	if ((vLocalSystemData->vITIPCDig->vImageDisplayOnlyRejects)	&& (!vRejectsImageDisplay) &&
		(vShowingLiveImages))
	{
		TempText1.LoadString(IDS_RejectsOnly);
		TempText = TempText + TempText1;
	}
	SetDlgItemText(IDC_MainStatusLine,TempText);
}

void CNewImageDisplayDialog::UpdateThresholdDisplay()
{
	if (vInspectionShowing)
	{
		CString TempText("");
		CString TempText1("");
		if (vImproveLearnMenu)
		{
			m_InspectionThreshold.ShowWindow(SW_SHOW);
			m_SubFunction3Display.ShowWindow(SW_HIDE);
			m_SubFunction4Display.ShowWindow(SW_HIDE);
			SetDlgItemText(IDC_InspectionThreshold, dtoa(vGlobalCurrentProduct->vAddToLearnCount) + " Added To Learn");
		}
		else
		if (vEditWhileRunning)
		{
			m_InspectionThreshold.ShowWindow(SW_HIDE);

			if ((PasswordOK(cTemporaryInspxPassword,false)) || (vInspectionShowing->vInspectionType == cFloodedInspection))
				m_SubFunction3Display.ShowWindow(SW_SHOW);
			else
				m_SubFunction3Display.ShowWindow(SW_HIDE);

			m_SubFunction4Display.ShowWindow(SW_SHOW);
			if ((vInspectionShowing->vInspectionType == cItemCountInspection) || (vInspectionShowing->vInspectionType == cSizeInspection) || (vInspectionShowing->vInspectionType == cMissingLidInspectionOnSize) || 
				(vInspectionShowing->vInspectionType == cFloodedInspection))
			{
				CString TempText2("");
				TempText1.LoadString(IDS_Threshold);
				TempText2.LoadString(IDS_Size);
				SetDlgItemText(IDC_SubFunction3Display,dtoa(vInspectionShowing->vSize,0));
				SetDlgItemText(IDC_SubFunction4Display,dtoa(vInspectionShowing->vThreshold,0));
			}
			else
			{
				TempText.LoadString(IDS_Threshold);
				if ((vInspectionShowing->vInspectionType == cDensityInspection) || (vInspectionShowing->vInspectionType == cCheckWeighInspection))
				{
					SetDlgItemText(IDC_SubFunction3Display, _T(""));
					SetDlgItemText(IDC_SubFunction4Display, dtoa(vInspectionShowing->ConvertToWeight(vInspectionShowing->vDensityThresholdLower),2) + "-" + 
						dtoa(vInspectionShowing->ConvertToWeight(vInspectionShowing->vDensityThresholdUpper),2));
				}
				else
				if ((vInspectionShowing->vInspectionType == cMissingLidInspection) || (vInspectionShowing->vInspectionType == cUnderfillInspection))
				{
					SetDlgItemText(IDC_SubFunction3Display, _T(""));
					SetDlgItemText(IDC_SubFunction4Display,dtoa(vInspectionShowing->vDensityThresholdUpper,2));
				}
				else
				if (vInspectionShowing->vInspectionType == cUnderfillByWeightInspection)
				{
					SetDlgItemText(IDC_SubFunction3Display, _T(""));
					SetDlgItemText(IDC_SubFunction4Display, dtoa(vInspectionShowing->ConvertToWeight(vInspectionShowing->vDensityThresholdUpper),2));
				}
				else
				if (vInspectionShowing->vInspectionType == cOverfillByWeightInspection)
				{
					SetDlgItemText(IDC_SubFunction3Display, _T(""));
					SetDlgItemText(IDC_SubFunction4Display, dtoa(vInspectionShowing->ConvertToWeight(vInspectionShowing->vDensityThresholdLower),2));
				}
				else
				if (vInspectionShowing->vInspectionType == cOverfillInspection)
				{
					SetDlgItemText(IDC_SubFunction3Display, _T(""));
					SetDlgItemText(IDC_SubFunction4Display,dtoa(vInspectionShowing->vDensityThresholdLower,2));
				}
				else
				if (vInspectionShowing->vInspectionType == cStandardDeviationInspection)
				{
					SetDlgItemText(IDC_SubFunction4Display, dtoa(vInspectionShowing->vDensityThresholdLower,2) + "-" + dtoa(vInspectionShowing->vDensityThresholdUpper,2));
				}
				else
				{
					SetDlgItemText(IDC_SubFunction3Display,dtoa(vInspectionShowing->vSize,0));
					SetDlgItemText(IDC_SubFunction4Display,dtoa(vInspectionShowing->vThreshold,0));
				}
			}
		}
		else
		{
			if (!vRejectsImageDisplay)
			{
				m_InspectionThreshold.ShowWindow(SW_SHOW);
				m_SubFunction3Display.ShowWindow(SW_HIDE);
				m_SubFunction4Display.ShowWindow(SW_HIDE);
			}
			if ((vInspectionShowing->vInspectionType == cItemCountInspection) || (vInspectionShowing->vInspectionType == cSizeInspection) || (vInspectionShowing->vInspectionType == cMissingLidInspectionOnSize))
			{
				CString TempText2("");
				TempText1.LoadString(IDS_Threshold);
				TempText2.LoadString(IDS_Size);
				SetDlgItemText(IDC_InspectionThreshold,TempText1 + ":" + dtoa(vInspectionShowing->vThreshold,0) + ", " + TempText2 + ":" + dtoa(vInspectionShowing->vSize,0));
			}
			else
			{
				TempText.LoadString(IDS_Threshold);
				if ((vInspectionShowing->vInspectionType == cDensityInspection) || (vInspectionShowing->vInspectionType == cCheckWeighInspection))
				{
					if (((vInspectionShowing->vCheckWeighBFactor != 0) || (vInspectionShowing->vCheckWeighMFactor != 1)) && (vInspectionShowing->vCheckWeighMFactor))
						TempText.LoadString(IDS_AcceptableWeight);
					else
						TempText.LoadString(IDS_AcceptableDensity);

					if (vInspectionShowing->vHasDensityLowerThreshold)
						SetDlgItemText(IDC_InspectionThreshold,TempText + ":" + dtoa(vInspectionShowing->ConvertToWeight(vInspectionShowing->vDensityThresholdUpper),2) + "-" +
							dtoa(vInspectionShowing->ConvertToWeight(vInspectionShowing->vDensityThresholdLower),2));
					else
						SetDlgItemText(IDC_InspectionThreshold,TempText + ":0-" + dtoa(vInspectionShowing->ConvertToWeight(vInspectionShowing->vDensityThresholdUpper),2));
				}
				else
				if ((vInspectionShowing->vInspectionType == cMissingLidInspection) || (vInspectionShowing->vInspectionType == cUnderfillInspection))
				{
					TempText.LoadString(IDS_Threshold);
					SetDlgItemText(IDC_InspectionThreshold, TempText + ":" + dtoa(vInspectionShowing->vDensityThresholdUpper,2));
				}
				else
				if (vInspectionShowing->vInspectionType == cUnderfillByWeightInspection)
				{
					TempText.LoadString(IDS_Threshold);
					SetDlgItemText(IDC_InspectionThreshold, TempText + ":" + dtoa(vInspectionShowing->ConvertToWeight(vInspectionShowing->vDensityThresholdUpper),2));
				}
				else
				if (vInspectionShowing->vInspectionType == cOverfillByWeightInspection)
				{
					TempText.LoadString(IDS_Threshold);
					SetDlgItemText(IDC_InspectionThreshold, TempText + ":" + dtoa(vInspectionShowing->ConvertToWeight(vInspectionShowing->vDensityThresholdLower),2));
				}
				else
				if (vInspectionShowing->vInspectionType == cOverfillInspection)
				{
					TempText.LoadString(IDS_Threshold);
					SetDlgItemText(IDC_InspectionThreshold,TempText + ":" + dtoa(vInspectionShowing->vDensityThresholdLower,2));
				}
				else
				if (vInspectionShowing->vInspectionType == cStandardDeviationInspection)
				{
					TempText.LoadString(IDS_AcceptableTexture);
					SetDlgItemText(IDC_InspectionThreshold,TempText + ":" + dtoa(vInspectionShowing->vDensityThresholdLower,2) + "-" + dtoa(vInspectionShowing->vDensityThresholdUpper,2));
				}
				else
				{
					TempText1.LoadString(IDS_MinSize);
					SetDlgItemText(IDC_InspectionThreshold,TempText + ": " + dtoa(vInspectionShowing->vThreshold,0) + ", " + TempText1 + ": " + dtoa(vInspectionShowing->vSize,0));
				}
			}
		}
	}
}

void CNewImageDisplayDialog::SetInspectionShowing(CInspection *TempInspection)
{
	if (TempInspection)
	{
		CInspection *TempOriginalInspection = vInspectionShowing;
		//not super user always do, if super user and different inspection, then do
		//if ((TempOriginalInspection != TempInspection) || (!PasswordOK(cSuperInspxPassword,false)))vShowingProcess
		if ((!vShowingProcess) || (!PasswordOK(cSuperInspxPassword,false)))  //don't change inspections if super password and showing a step
		if (!vShowThresholdMenu)  //don't change inspections if editing the threshold
		{
			vSensitivity = 0xFF;
			vInspectionShowing = TempInspection;

			vInspectionNumberShowing = 0;
			for (BYTE TempLoop = 0; TempLoop < cMaximumNumberOfInspections; TempLoop++)
			if (vGlobalCurrentProduct->vInspection[TempLoop] == TempInspection)
			{
				vInspectionNumberShowing = TempLoop;
				TempLoop = cMaximumNumberOfInspections;
			}

			//vShowOriginalImageFactor = vInspectionShowing->vOriginalImageFactor;
			//vShowEnhancedImageFactor = vInspectionShowing->vEnhancedImageFactor;

			UpdateThresholdDisplay();

			vShowingInspectionNumberOfRedPixels = 0;
			vDarkestValueInROI = 255;
			vLightestValueInROI = 0;
			vSizeOfAllContaminants = 0;
			vSizeOfLargestContaminant = 0;
			vNumberOfContaminants = 0;

			vThreshold = vInspectionShowing->vThreshold;
			vMaximumThreshold = 255;
			if (vInspectionShowing->vInspectionType == cUnderfillByWeightInspection)
			{
				vThreshold = vInspectionShowing->vDensityThresholdUpper;
				vMaximumThreshold = vMaximumUnderfillByWeightThreshold;
			}
			else
			if (vInspectionShowing->vInspectionType == cOverfillByWeightInspection)
			{
				vThreshold = vInspectionShowing->vDensityThresholdLower;
				vMaximumThreshold = vMaximumUnderfillByWeightThreshold;
			}
			else
			if ((vInspectionShowing->vInspectionType == cDensityInspection) ||
				(vInspectionShowing->vInspectionType == cCheckWeighInspection) ||
				(vInspectionShowing->vInspectionType == cMissingLidInspection) ||
				(vInspectionShowing->vInspectionType == cUnderfillInspection) ||
				(vInspectionShowing->vInspectionType == cStandardDeviationInspection))
				vThreshold = vInspectionShowing->vDensityThresholdUpper;
			else
			if (vInspectionShowing->vInspectionType == cOverfillInspection)
				vThreshold = vInspectionShowing->vDensityThresholdLower;

			//5/16/2007
			/*
			if (vInspectionShowing->vName.GetLength() > 15)
				SetDlgItemText(IDC_CurrentInspection,vInspectionShowing->vName);
			else
			if (!vImproveLearnMenu)
			{
				CString TempText1;
				TempText1.LoadString(IDS_Viewing);
				SetDlgItemText(IDC_CurrentInspection,TempText1 + ": " + vInspectionShowing->vName);
			}
			*/
			ClearAllDisplayOptions(true);
			UpdateButtons();
		}
	}
}

void CNewImageDisplayDialog::OperatorSetInspectionSize()
{
	if (vInspectionShowing)
	{
		CString TempText("");
		if (vInspectionShowing->vHasAMinimumSize)
		{
			CInspection *TempInspection = vInspectionShowing;
			//edit size of defect parameter
			CNumericEntryDialog INumericEntryDialog;  
			//Set dialog box data titles and number value
			INumericEntryDialog.vEditString = dtoa(TempInspection->vSize,2);
			TempText.LoadString(IDS_Inspection);//"\n\n\nPassword not correct"
			INumericEntryDialog.m_DialogTitleStaticText1 = TempText + ": " + TempInspection->vName;
							
			if (TempInspection->vInspectionType == cFloodedInspection)
				INumericEntryDialog.m_DialogTitleStaticText2 = "Enter the Minimum Spout Length";
			else
			if ((vInspectionShowing->vInspectionType == cItemCountInspection) || (TempInspection->vInspectionType == cSizeInspection) || (TempInspection->vInspectionType == cMissingLidInspectionOnSize))
			{
				TempText.LoadString(IDS_MinimumSizeofDarkAreaTheObjectSize);//"Minimum Size of Dark Area (The Object Size)"
				INumericEntryDialog.m_DialogTitleStaticText2 = TempText;
			}
			else
			{
				TempText.LoadString(IDS_MinimumDefectSize);//"Minimum Defect Size"
				INumericEntryDialog.m_DialogTitleStaticText2 = TempText;
			}
			TempText.LoadString(IDS_PermanentChangeinProduct);
			INumericEntryDialog.m_DialogTitleStaticText3 = TempText;
			TempText.LoadString(IDS_OriginalValue);
			INumericEntryDialog.m_DialogTitleStaticText4 = TempText + ": " + INumericEntryDialog.vEditString;
			TempText.LoadString(IDS_Default);
			INumericEntryDialog.m_UnitsString = TempText + ": 10";
			INumericEntryDialog.vMaxValue = 9000000;
			INumericEntryDialog.vIntegerOnly = true;
			//Pass control to dialog box and display
			int nResponse = INumericEntryDialog.DoModal();
			//dialog box is now closed, if user pressed select do this
			//if user pressed cancel, do nothing
			if (nResponse == IDOK)
			{
				ReportErrorMessage("Changed " + TempInspection->vName + " Minimum Size from: " + dtoa(TempInspection->vSize, 0) + " to " + dtoa(_wtoi(INumericEntryDialog.vEditString),0) + " by operator", cUserChanges,0);
				TempInspection->vSize = _wtoi(INumericEntryDialog.vEditString);
				//WaitThenDrawImageOnScreen();
				//SetInspectionShowing(TempInspection);
				SetDlgItemText(IDC_SubFunction3Display,dtoa(vInspectionShowing->vSize,0));

				if ((TempInspection->vInspectionType == cDripInspection) || (TempInspection->vInspectionType == cDripBySpoutInspection))
				{
					//Set dialog box data titles and number value For MAXIMUM size
					INumericEntryDialog.vEditString = dtoa(TempInspection->vMaximumSize, 0);

					INumericEntryDialog.m_DialogTitleStaticText1 = "Maximum size of Contaminant or foreign matter";
					INumericEntryDialog.m_DialogTitleStaticText2 = "Above this size is considered normal product";
					INumericEntryDialog.vMaxValue = 65536;
					INumericEntryDialog.vMinValue = TempInspection->vSize + 2;
					INumericEntryDialog.vAllowZero = true;
					INumericEntryDialog.vIntegerOnly = true;

					//Pass control to dialog box and display
					int nResponse = INumericEntryDialog.DoModal();
					//dialog box is now closed, if user pressed select do this
					//if user pressed cancel, do nothing
					if (nResponse == IDOK)
					{
						TempInspection->vMaximumSize = _wtoi(INumericEntryDialog.vEditString);
						if (TempInspection->vMaximumSize)
							SetDlgItemText(IDC_SubFunction3Display,dtoa(vInspectionShowing->vSize,0) + " - " + dtoa(TempInspection->vMaximumSize, 0));
						else
							SetDlgItemText(IDC_SubFunction3Display,dtoa(vInspectionShowing->vSize,0));

						//Set dialog box data titles and number value For MinimumWidth
						INumericEntryDialog.vEditString = dtoa(TempInspection->vMinimumWidth, 0);

						INumericEntryDialog.m_DialogTitleStaticText1 = "Minimum Width of Contaminant or foreign matter";
						INumericEntryDialog.m_DialogTitleStaticText2 = "Above this width is considered defective product";
						INumericEntryDialog.vMaxValue = 65536;
						INumericEntryDialog.vMinValue = 0;
						INumericEntryDialog.vAllowZero = true;
						INumericEntryDialog.vIntegerOnly = true;

						//Pass control to dialog box and display
						int nResponse = INumericEntryDialog.DoModal();
						//dialog box is now closed, if user pressed select do this
						//if user pressed cancel, do nothing
						if (nResponse == IDOK)
						{
							TempInspection->vMinimumWidth = _wtoi(INumericEntryDialog.vEditString);

							CString TempString = "";
							if (TempInspection->vMaximumSize)
								TempString = TempString + "-" + dtoa(TempInspection->vMaximumSize, 0);
							if (TempInspection->vMinimumWidth)
								TempString = TempString + ", " + dtoa(TempInspection->vMinimumWidth, 0);

							SetDlgItemText(IDC_SubFunction3Display, dtoa(vInspectionShowing->vSize,0) + TempString);

							INumericEntryDialog.vEditString = dtoa(TempInspection->vMaximumHeightAboveBottom, 0);
							INumericEntryDialog.m_DialogTitleStaticText1 = "Maximum Contaminant Centroid Height Above Bottom of container";
							INumericEntryDialog.m_DialogTitleStaticText2 = "Above this height is considered OK";
							INumericEntryDialog.vMaxValue = 384;
							INumericEntryDialog.vMinValue = 0;
							INumericEntryDialog.vAllowZero = true;
							INumericEntryDialog.vIntegerOnly = true;

							//Pass control to dialog box and display
							int nResponse = INumericEntryDialog.DoModal();
							//dialog box is now closed, if user pressed select do this
							//if user pressed cancel, do nothing
							if (nResponse == IDOK)
							{
								TempInspection->vMaximumHeightAboveBottom = _wtoi(INumericEntryDialog.vEditString);

								CString TempString = "";
								if (TempInspection->vMaximumSize)
									TempString = TempString + "-" + dtoa(TempInspection->vMaximumSize, 0);
								if (TempInspection->vMinimumWidth)
									TempString = TempString + ", " + dtoa(TempInspection->vMinimumWidth, 0);
								if (TempInspection->vMaximumHeightAboveBottom)
									TempString = TempString + ", " + dtoa(TempInspection->vMaximumHeightAboveBottom, 0);

								SetDlgItemText(IDC_SubFunction3Display, dtoa(vInspectionShowing->vSize,0) + TempString);
							}
							else 
							if (nResponse == 10)
							{
								//Main Menu button pressed
								CDialog::EndDialog(10);
							}
						}
						else 
						if (nResponse == 10)
						{
							//Main Menu button pressed
							CDialog::EndDialog(10);
						}
					}
					else 
					if (nResponse == 10)
					{
						//Main Menu button pressed
						CDialog::EndDialog(10);
					}
				}
			}
			else 
			if (nResponse == 10)
			{	//Main Menu button pressed
				Sleep(1); //is the equivelent of a yeild statement;
				PrepareAndExit(10);
			}
		}
	}
}

void CNewImageDisplayDialog::StopShowingLiveImages()
{
	vShowingLiveImages = false;
	UpdateButtons();
}

void CNewImageDisplayDialog::StartShowingLiveImages()
{
	vLocalSystemData->vITIPCDig->vRejectBufferShowing = -1;
	vShowClipboard = false;
	vShowAverage = false;
	vShowStructure = false;
	//vShowGlassStructure = false;
	//vShowGlassHistogram = false;
	vShowBackground = false;
	vShowHistogram = false;
	vShowingLiveImages = true;
	//CString TempText("");
	//TempText.LoadString(IDS_FreezeLastImage);
	//SetDlgItemText(IDC_Function2Button,TempText);
	vDisplayChanges = true;
}

void CNewImageDisplayDialog::ShowNextReject(bool TempGiveNoticeIfNone, bool TempShowPrevious)
{//show reject n
	vAllRejectsHidden = false;
	vPlotLine = 0;

	if ((!(vMainWindowPointer->HaveRejects(PasswordOK(cTemporaryInspxPassword,false)))) &&//vImproveLearnMenu))) &&
		(!((PasswordOK(cTemporaryInspxPassword,false)) && (vHaveAnImageToShow))))
	{
		vLocalSystemData->vITIPCDig->vRejectBufferShowing = -1;
		vNumberOfRejectsInLocalBuffer = 0;
		if (vHaveAnImageToShow)
		{
			if (vLocalSystemData->vITIPCDig->vOriginalImage)
			if (vLocalSystemData->vITIPCDig->vOriginalImage->vContainerEjectInspection < 255)
				SetInspectionShowing(vGlobalCurrentProduct->vInspection[vLocalSystemData->vITIPCDig->vOriginalImage->vContainerEjectInspection]);
			WaitThenDrawImageOnScreen();
			UpdateButtons();
			UpdateDisplayIndicators();
		}
		/*
		if (TempGiveNoticeIfNone)
		if (!vGaveNoRejectsToShowMessage)
		{
			CNoticeDialog TempNoticeDialog;
			CString TempText("");
			TempText.LoadString(IDS_NoRejectsAvailableToShow);
			TempNoticeDialog.vNoticeText = TempText;
			TempNoticeDialog.vType = cInformationMessage;
			TempNoticeDialog.DoModal();
			vGaveNoRejectsToShowMessage = true;
		}
		*/
	}
	else
	if (vWindowInitialized)
	if (vHaveAnImageToShow)
	{	//show next reject button
		//need to show first reject, so copy all the data
		//vHaveAnImageToShow = true;
		if (vLocalSystemData->vITIPCDig->vRejectBufferShowing == -1)
		{
			if (vLocalSystemData->vITIPCDig->vLastNRejectImagesLock.Lock())
			{
				WORD TempNumberOfRejectsToShow = 0;
				InitializeLocalLastRejectsBuffer(false);

				int TempRejectTypeIndex[cNumberOfExternalDetectors + cMaximumNumberOfInspections + 4];
				for (WORD TempOuterLoop = 0; TempOuterLoop < (cNumberOfExternalDetectors + cMaximumNumberOfInspections + 4); TempOuterLoop++)
					TempRejectTypeIndex[TempOuterLoop] = 0;

				WORD TempOuterLoopMax = vTotalNumberOfInspectionsToDisplay; 
				if (PasswordOK(cTemporaryInspxPassword,false))
					TempOuterLoopMax++;  //add one for good images if Inspx password mode

				if (TempOuterLoopMax > cMaximumNumberOfCountsDisplayed)
					TempOuterLoopMax = cMaximumNumberOfCountsDisplayed;

				bool TempDone = false;
				DWORD TempThisImageNumber = 0;
				DWORD TempLargestImageNumber = 0;
				DWORD TempTypeOfLargestImage = 0;
				while (!TempDone)
				{
					//find the most recent image
					TempLargestImageNumber = 0;
					int TempTypeWithMostRecentImage = -1;
					for (WORD TempOuterLoop = 0; TempOuterLoop < TempOuterLoopMax; TempOuterLoop++)
					{ //TempOuterLoop is the RejectReason for this type
						if (vGlobalCurrentProduct->vTypesOfRejectsToView & (0x0001 << TempOuterLoop))
						if (vLocalSystemData->vITIPCDig->vLastNRejectImages[TempRejectTypeIndex[TempOuterLoop]][TempOuterLoop])
						{
							TempThisImageNumber = vLocalSystemData->vITIPCDig->vLastNRejectImages[
								TempRejectTypeIndex[TempOuterLoop]][TempOuterLoop]->vContainerNumber;
							if (TempThisImageNumber >= TempLargestImageNumber)
							{
								TempLargestImageNumber = TempThisImageNumber;
								TempTypeWithMostRecentImage = TempOuterLoop;
							}
						}
					}
					if (TempTypeWithMostRecentImage >= 0) //then found the next reject to include
					{
						if (ThereIsEnoughMemory(sizeof(CImageData) + 2000000 + (2 * vLocalSystemData->vITIPCDig->vOriginalBufferSize), "Local Reject Image Data Object"))
						{
							vLocalLastNRejectImages[TempNumberOfRejectsToShow] = new CImageData;
							if (vLocalLastNRejectImages[TempNumberOfRejectsToShow])
							{
								vLocalSystemData->vITIPCDig->CopyImageData(vLocalSystemData->vITIPCDig->vLastNRejectImages[TempRejectTypeIndex[TempTypeWithMostRecentImage]][TempTypeWithMostRecentImage],
									vLocalLastNRejectImages[TempNumberOfRejectsToShow], vLocalSystemData->vITIPCDig->vOriginalBufferSize);
								if (TempRejectTypeIndex[TempTypeWithMostRecentImage] < cMaxNumberOfLastRejectImages)
								{
									TempRejectTypeIndex[TempTypeWithMostRecentImage]++;
									TempNumberOfRejectsToShow++;
								}
							}
						}
						else
							TempDone = true;
					}
					else
						TempDone = true;
				}
				vNumberOfRejectsInLocalBuffer = TempNumberOfRejectsToShow;

				vLocalSystemData->vITIPCDig->vLastNRejectImagesLock.Unlock();
				if (!vNumberOfRejectsInLocalBuffer)
				{
					vAllRejectsHidden = true;
				}
			}
			vShowAlternateImage = false;
			UpdateButtons();
		}
		vShowClipboard = false;
		vShowAverage = false;
		vShowBackground = false;
		vShowStructure = false;
		//vShowGlassStructure = false;
		//vShowGlassHistogram = false;
		vShowHistogram = false;
		if (vShowingLiveImages)
			StopShowingLiveImages();
		vShowNextFrame = false;

		if (TempShowPrevious)
		{
			if (vLocalSystemData->vITIPCDig->vRejectBufferShowing < 0)
				vLocalSystemData->vITIPCDig->vRejectBufferShowing = vNumberOfRejectsInLocalBuffer - 1;
			else
				vLocalSystemData->vITIPCDig->vRejectBufferShowing--;
		}
		else
			vLocalSystemData->vITIPCDig->vRejectBufferShowing++;
		if (vLocalSystemData->vITIPCDig->vRejectBufferShowing >= 
			vNumberOfRejectsInLocalBuffer)
			vLocalSystemData->vITIPCDig->vRejectBufferShowing = -1;
		if ((vLocalSystemData->vITIPCDig->vRejectBufferShowing >= 0) &&
			(vLocalLastNRejectImages[
			vLocalSystemData->vITIPCDig->vRejectBufferShowing] == NULL))
				vLocalSystemData->vITIPCDig->vRejectBufferShowing = -1;

		/*
		if (vLocalSystemData->vITIPCDig->vRejectBufferShowing == -1)
		if (vImproveLearnMenu)
		if (vNumberOfRejectsInLocalBuffer)
		{
			if (TempShowPrevious)
				vLocalSystemData->vITIPCDig->vRejectBufferShowing = vNumberOfRejectsInLocalBuffer - 1;
			else
				vLocalSystemData->vITIPCDig->vRejectBufferShowing = 0;
		}
		*/

		if (vLocalSystemData->vITIPCDig->vRejectBufferShowing < 0)
		{
			vNumberOfRejectsInLocalBuffer = 0;

			//free the memory as not showing rejects anymore
			if (vWindowInitialized)
				InitializeLocalLastRejectsBuffer(false);

			//show image on main menu if have one
			if (vHaveAnImageToShow)
			{
				if (vLocalSystemData->vITIPCDig->vOriginalImage)
				if (vLocalSystemData->vITIPCDig->vOriginalImage->vContainerEjectInspection < 255)
					SetInspectionShowing(vGlobalCurrentProduct->vInspection[vLocalSystemData->vITIPCDig->vOriginalImage->vContainerEjectInspection]);
			}
		}
		else  //showing a reject
		if (vLocalLastNRejectImages[vLocalSystemData->vITIPCDig->vRejectBufferShowing])
		{
			vGaveNoRejectsToShowMessage = false;
			if ((vGlobalCurrentProduct->vNumberOfInspections > vLocalLastNRejectImages[vLocalSystemData->vITIPCDig->vRejectBufferShowing]->vContainerEjectInspection) &&
				(vGlobalCurrentProduct->vInspection[vLocalLastNRejectImages[vLocalSystemData->vITIPCDig->vRejectBufferShowing]->vContainerEjectInspection]))
			{
				SetInspectionShowing(vGlobalCurrentProduct->vInspection[vLocalLastNRejectImages[vLocalSystemData->vITIPCDig->vRejectBufferShowing]->vContainerEjectInspection]);
			}

			vLocalSystemData->vTweakReferenceAdjust = vLocalLastNRejectImages[vLocalSystemData->vITIPCDig->vRejectBufferShowing]->vFineReferenceAdjust;
			if (vInspectionShowing)
			if (vInspectionShowing->vInspectionType == cKernelInspection)
				vLocalSystemData->vTweakReferenceAdjust = 0;
		}
		WaitThenDrawImageOnScreen();
		UpdateButtons();
		UpdateDisplayIndicators();
	}
}

void CNewImageDisplayDialog::OperatorSelectCurrentInspection()
{
	//change current inspection
	CString TempText("");
	if (vGlobalCurrentProduct->vNumberOfInspections)
	{
		//Create dialog box
		int nResponse = IDOK;
		int TempInt2 = 1;
		//BYTE TempNumberOfInspections = vGlobalCurrentProduct->vNumberOfInspections;
		//if ((PasswordOK(cTemporaryInspxPassword,false)) && (vGlobalCurrentProduct->vDriftDensityInspection))
		//	TempNumberOfInspections++;
		//if (vMainWindowPointer->GetImproveLearnInspectionNumber()) 
		//	TempNumberOfInspections--;

		if (vGlobalCurrentProduct->vNumberOfInspections > 1)
		{
			CSelectInspectionDialog ISelectInspectionDialog;  
			//Set dialog box data like which square to start on
			ISelectInspectionDialog.vLocalSystemData = vLocalSystemData;
			ISelectInspectionDialog.vProductEditing = vGlobalCurrentProduct;
			ISelectInspectionDialog.SelectWindowForStyle = cSelectInspectionForDisplay;
			//Pass control to dialog box and display
			nResponse = ISelectInspectionDialog.DoModal();
			//dialog box is now closed, if user pressed select do this
			//if user pressed cancel, do nothing
			if (nResponse == IDOK) //  closed with OK
				TempInt2 = ISelectInspectionDialog.vInspectionNumberSelected;
		}
		else
		{
			CNoticeDialog TempNoticeDialog;
			TempText.LoadString(IDS_Only1Inspectionforcurrentproductsodonotneedtoselect);//"\n\nOnly 1 Inspection for current product,\nso do not need to select."
			TempNoticeDialog.vNoticeText = TempText;
			TempNoticeDialog.vType = cInformationMessage;
			//TempNoticeDialog.vAutoClose = 15000;
			TempNoticeDialog.DoModal();
		}
		if (nResponse == IDOK)
		{ //  closed with OK
			if(TempInt2 > 0)
			{
				TempInt2--;
				if (TempInt2 >= vGlobalCurrentProduct->vNumberOfInspections)
				{
					SetInspectionShowing(vGlobalCurrentProduct->vDriftDensityInspection);
				}
				else
				if(vGlobalCurrentProduct->vInspection[TempInt2])
				{
					SetInspectionShowing(vGlobalCurrentProduct->vInspection[TempInt2]);
				}
			}
		}
		else 
		if (nResponse == 10)
		{
			//Main Menu Button Pressed
			Sleep(1); //is the equivelent of a yeild statement;
			PrepareAndExit(10);
		}
	}
	else
	{
		CNoticeDialog TempNoticeDialog;
		TempText.LoadString(IDS_NoInspectionsforcurrentproducttoselect);//"\n\n\nNo Inspections for current product to select."
		TempNoticeDialog.vNoticeText = TempText;
		//TempNoticeDialog.vAutoClose = 15000;
		TempNoticeDialog.vType = cInformationMessage;
		TempNoticeDialog.DoModal();
	}

	WaitThenDrawImageOnScreen();
	UpdateButtons();
	UpdateDisplayIndicators();
}

void CNewImageDisplayDialog::ShowNextProcessData() 
{
	vTweakingThreshold = false;
	bool TempUpdateDisplay = true;
	//bool TempHaveBackground = 
	//	vGlobalCurrentProduct->ProductHasBackgroundFile(vGlobalCurrentDirectory);
		//(((vLocalConfigurationData->vRemoveBackgroundFromImage) || 
		//(vLocalSystemData->vRemoveBackgroundInAutoSize)) &&
		//(vGlobalCurrentProduct->ProductHasBackgroundFile(vGlobalCurrentDirectory)));
	if ((vLocalSystemData->vITIPCDig->vProductAverageImage) ||
		(vLocalSystemData->vITIPCDig->vProductBackgroundImage) ||
		(vLocalSystemData->vITIPCDig->vAverageImage))
	{
		if (vShowAverage)
		{
			vShowAverage = false;
			if ((vLocalSystemData->vITIPCDig->vProductStructureImage) || 
				((vLocalSystemData->vITIPCDig->vProductContaminant2StructureImage) && (vLocalSystemData->vITIPCDig->vProductContaminant2StructureImage->vGreyImage)) ||
				((vLocalConfigurationData->vAllowVoidInspection) && ((vLocalSystemData->vITIPCDig->vVoidProductStructureImage) || (vLocalSystemData->vITIPCDig->vVoidStructureImage))))
			{
				vShowStructure = true;
				if (vLocalSystemData->vITIPCDig->vProductStructureImage)
					vShowType = cShowTypeContaminant;
				else
				if (vLocalSystemData->vITIPCDig->vProductContaminant2StructureImage)
					vShowType = cShowTypeContaminant2;
				else
					vShowType = cShowTypeVoid;
			}
			else
			if (vLocalSystemData->vITIPCDig->vProductHistogramImage)
				vShowHistogram = true;
		}
		else
		if (vShowStructure)
		{
			if ((vShowType == cShowTypeContaminant) && (vLocalSystemData->vITIPCDig->vProductContaminant2StructureImage))
			  vShowType = cShowTypeContaminant2;
			else
			if (((vShowType == cShowTypeContaminant2) || (vShowType == cShowTypeContaminant)) && 
				((vLocalConfigurationData->vAllowVoidInspection) && ((vLocalSystemData->vITIPCDig->vVoidProductStructureImage) || (vLocalSystemData->vITIPCDig->vVoidStructureImage))))
				vShowType = cShowTypeVoid;
			else
			{
				vShowStructure = false;
				//if (vLocalSystemData->vITIPCDig->vProductGlassStructureImage)
				//		vShowGlassStructure = true;
				//else
				if (vLocalSystemData->vITIPCDig->vProductHistogramImage)
					vShowHistogram = true;
			}
		}
		else
			/*
		if (vShowGlassStructure)
		{
			vShowGlassStructure = false;
			if (vLocalSystemData->vITIPCDig->vProductHistogramImage)
				vShowHistogram = true;
		}
		else
		*/
		if (vShowHistogram)
		{
			vShowHistogram = false;
			//if (vLocalSystemData->vITIPCDig->vProductGlassHistogramImage)
			//	vShowGlassHistogram = true;
			//else
			//{ //if did not have an image, then blank 
				vShowAlternateImage = false;
				if (!vHaveAnImageToShow)
				{
					m_Background.ShowWindow(SW_SHOW);
					m_ErrorMessage.ShowWindow(SW_SHOW);
					TempUpdateDisplay = false;
				}
			//}
		}
		else
			/*
		if (vShowGlassHistogram)
		{
			vShowGlassHistogram = false;
			{ //if did not have an image, then blank 
				vShowAlternateImage = false;
				if (!vHaveAnImageToShow)
				{
					m_Background.ShowWindow(SW_SHOW);
					m_ErrorMessage.ShowWindow(SW_SHOW);
					TempUpdateDisplay = false;
				}
			}
		}
		else
		*/
		if (vShowBackground)
		{
			vShowAverage = true;
			vShowBackground = false;
			vShowAlternateImage = true;
		}
		else
		if (vLocalSystemData->vITIPCDig->vProductBackgroundImage)
		{
			vShowBackground = true;
			vShowAlternateImage = true;
		}
		else
		{
			vShowAverage = true;
			vShowAlternateImage = true;
		}
		WaitThenDrawImageOnScreen();
		UpdateDisplayIndicators();
		UpdateButtons();
	}
}

void CNewImageDisplayDialog::ShowNextProcess(bool TempForward) 
{
	BYTE TempNumberOfSteps = 34;
	bool TempChanged = false;
	BYTE TempLoop = 0;
	if (vInspectionShowing)
	{  //not glass algorithm
		BYTE TempProductReference = 0;
		if (vGlobalCurrentProduct->vReferenceRight)
			TempProductReference = 1;

		if ((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode) ||
			(vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vLocalConfigurationData->vScanTracType == cAllegroScanTrac) ||
			(vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac) || (vLocalConfigurationData->vScanTracType == cBriosoScanTrac)) //Continuous Feed, or a Pipeline or TD
			TempProductReference = 0;

		if (vInspectionShowing)
		if (vInspectionShowing->vInspectionType == cJarBottomContaminantInspection)
			TempProductReference = 0;

		//if (vInspectionShowing)
		//if (!vInspectionShowing->vSubtractAverage)
		//	TempProductReference = 0;

		//BYTE TempOriginalShowReferenceAdjustedImage = vShowReferenceAdjustedImage;
		ClearAllDisplayOptions(false);

		if (vInspectionShowing)
		{
			if (vGlobalCurrentProduct->vReferenceTop)
			if (vInspectionShowing->vReferenceToTop)
				TempProductReference = TempProductReference | 2;

			if (vInspectionShowing->vReferenceToBottom)
				TempProductReference = TempProductReference | 2;
		}

		if ((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode))
			TempProductReference = 0;
		while ((!TempChanged) && (TempLoop < TempNumberOfSteps))
		{
			TempLoop++;
			if (TempForward)
			{
				if (vShowingProcess == TempNumberOfSteps)
				{
					vShowingProcess = 0;
					TempChanged = true;
					vAnalysisImageDisplay = true;
					this->SetWindowText(_T("Rejects Analysis"));
					vShowThreshold = false;
					vShowThresholdMenu = false; 
					ClearSensitivity();
					vDialogTitleString.LoadString(IDS_ImageAnalysis);
					SetDlgItemText(IDC_DialogTitleStaticText1,vDialogTitleString);
				}
				else
					vShowingProcess++;
			}
			else
			{
				if (vShowingProcess == TempNumberOfSteps)
				{
					vAnalysisImageDisplay = true;
					this->SetWindowText(_T("Rejects Analysis"));
					vShowThreshold = false;
					vShowThresholdMenu = false; 
					ClearSensitivity();
					vDialogTitleString.LoadString(IDS_ImageAnalysis);
					SetDlgItemText(IDC_DialogTitleStaticText1,vDialogTitleString);
				}
				if (vShowingProcess)
				{
					vShowingProcess--;
					if (!vShowingProcess)
						TempChanged = true;
				}
				else
					vShowingProcess = TempNumberOfSteps;
			}
			if (vShowingProcess)
			switch (vShowingProcess)
			{
				case 1:
					if (TempProductReference & 1)
					{
						vShowReferenceAdjustedImage = TempProductReference & 1;

						TempChanged = true;
					}
				break;
				case 2:
					if (vInspectionShowing->vWhiteOutAnythingTouchingTheTop)
					{
						vShowReferenceAdjustedImage = TempProductReference & 1;
						vWhiteOutAnythingTouchingTheTop = vInspectionShowing->vWhiteOutAnythingTouchingTheTop;
						TempChanged = true;
					}
				break;
				case 3:
					if (vInspectionShowing->vFillBackground)
					{
						vShowReferenceAdjustedImage = TempProductReference & 1;
						vWhiteOutAnythingTouchingTheTop = vInspectionShowing->vWhiteOutAnythingTouchingTheTop;
						vFillBackground = vInspectionShowing->vFillBackground;
						TempChanged = true;
					}
				break;
				case 4:
					if (vInspectionShowing->vErodeCount)
					{
						vShowReferenceAdjustedImage = TempProductReference & 1;
						vWhiteOutAnythingTouchingTheTop = vInspectionShowing->vWhiteOutAnythingTouchingTheTop;
						vFillBackground = vInspectionShowing->vFillBackground;
						vShowErodeCount = vInspectionShowing->vErodeCount;
						TempChanged = true;
					}
				break;
				case 5:
					if (vInspectionShowing->vDilationCount)
					{
						vShowReferenceAdjustedImage = TempProductReference & 1;
						vWhiteOutAnythingTouchingTheTop = vInspectionShowing->vWhiteOutAnythingTouchingTheTop;
						vFillBackground = vInspectionShowing->vFillBackground;
						vShowErodeCount = vInspectionShowing->vErodeCount;
						vShowDilationCount = vInspectionShowing->vDilationCount;
						TempChanged = true;
					}
				break;
				case 6:
					if (vInspectionShowing->vErodeCountAtStart)
					{
						vShowReferenceAdjustedImage = TempProductReference & 1;
						vWhiteOutAnythingTouchingTheTop = vInspectionShowing->vWhiteOutAnythingTouchingTheTop;
						vFillBackground = vInspectionShowing->vFillBackground;
						vShowErodeCount = vInspectionShowing->vErodeCount;
						vShowDilationCount = vInspectionShowing->vDilationCount;
						vShowErodeCountAtStart = vInspectionShowing->vErodeCountAtStart;
						TempChanged = true;
					}
				break;
				case 7:
					if (vInspectionShowing->vPreSmoothImage)
					{
						vShowReferenceAdjustedImage = TempProductReference & 1;
						vWhiteOutAnythingTouchingTheTop = vInspectionShowing->vWhiteOutAnythingTouchingTheTop;
						vFillBackground = vInspectionShowing->vFillBackground;
						vShowErodeCount = vInspectionShowing->vErodeCount;
						vShowDilationCount = vInspectionShowing->vDilationCount;
						vShowErodeCountAtStart = vInspectionShowing->vErodeCountAtStart;
						vPreSmoothImage = vInspectionShowing->vPreSmoothImage;
						TempChanged = true;
					}
				break;
				case 8:
					if (vInspectionShowing->vRemoveJarPunt)
					{
						vShowReferenceAdjustedImage = TempProductReference & 1;
						vWhiteOutAnythingTouchingTheTop = vInspectionShowing->vWhiteOutAnythingTouchingTheTop;
						vFillBackground = vInspectionShowing->vFillBackground;
						vShowDilationCount = vInspectionShowing->vDilationCount;
						vShowErodeCount = vInspectionShowing->vErodeCount;
						vShowErodeCountAtStart = vInspectionShowing->vErodeCountAtStart;
						vPreSmoothImage = vInspectionShowing->vPreSmoothImage;
						vRemoveJarPunt = 2;
						TempChanged = true;
					}
				break;
				// test
				case 9:
					if (vInspectionShowing->vRemoveJarPunt)
					{
						vShowReferenceAdjustedImage = TempProductReference & 1;
						vWhiteOutAnythingTouchingTheTop = vInspectionShowing->vWhiteOutAnythingTouchingTheTop;
						vFillBackground = vInspectionShowing->vFillBackground;
						vShowDilationCount = vInspectionShowing->vDilationCount;
						vShowErodeCount = vInspectionShowing->vErodeCount;
						vShowErodeCountAtStart = vInspectionShowing->vErodeCountAtStart;
						vPreSmoothImage = vInspectionShowing->vPreSmoothImage;
						vRemoveJarPunt = 3;
						TempChanged = true;
					}
				break;
				case 10:
					if (vInspectionShowing->vRemoveJarPunt)
					{
						vShowReferenceAdjustedImage = TempProductReference & 1;
						vWhiteOutAnythingTouchingTheTop = vInspectionShowing->vWhiteOutAnythingTouchingTheTop;
						vFillBackground = vInspectionShowing->vFillBackground;
						vShowDilationCount = vInspectionShowing->vDilationCount;
						vShowErodeCount = vInspectionShowing->vErodeCount;
						vShowErodeCountAtStart = vInspectionShowing->vErodeCountAtStart;
						vPreSmoothImage = vInspectionShowing->vPreSmoothImage;
						vRemoveJarPunt = 4;
						TempChanged = true;
					}
				break;
				case 11:
					if (vInspectionShowing->vRemoveJarPunt)
					{
						vShowReferenceAdjustedImage = TempProductReference & 1;
						vWhiteOutAnythingTouchingTheTop = vInspectionShowing->vWhiteOutAnythingTouchingTheTop;
						vFillBackground = vInspectionShowing->vFillBackground;
						vShowDilationCount = vInspectionShowing->vDilationCount;
						vShowErodeCount = vInspectionShowing->vErodeCount;
						vShowErodeCountAtStart = vInspectionShowing->vErodeCountAtStart;
						vPreSmoothImage = vInspectionShowing->vPreSmoothImage;
						vRemoveJarPunt = vInspectionShowing->vRemoveJarPunt;
						TempChanged = true;
					}
				break;
				case 12:
					if (vInspectionShowing->vTrimEdgesOfJar)
					{
						vShowReferenceAdjustedImage = TempProductReference & 1;
						vWhiteOutAnythingTouchingTheTop = vInspectionShowing->vWhiteOutAnythingTouchingTheTop;
						vFillBackground = vInspectionShowing->vFillBackground;
						vShowDilationCount = vInspectionShowing->vDilationCount;
						vShowErodeCount = vInspectionShowing->vErodeCount;
						vShowErodeCountAtStart = vInspectionShowing->vErodeCountAtStart;
						vPreSmoothImage = vInspectionShowing->vPreSmoothImage;
						vRemoveJarPunt = vInspectionShowing->vRemoveJarPunt;
						vTrimEdgesOfJarAtROI = vInspectionShowing->vTrimEdgesOfJar;
						TempChanged = true;
					}
				break;
				//case 13:
				//	if (vInspectionShowing->vFillInLightSpots)
				//	{
				//		vShowReferenceAdjustedImage = TempProductReference & 1;
				//		vWhiteOutAnythingTouchingTheTop = vInspectionShowing->vWhiteOutAnythingTouchingTheTop;
				//		vFillBackground = vInspectionShowing->vFillBackground;
				//		vShowDilationCount = vInspectionShowing->vDilationCount;
				//		vShowErodeCount = vInspectionShowing->vErodeCount;
				//		vShowErodeCountAtStart = vInspectionShowing->vErodeCountAtStart;
				//		vPreSmoothImage = vInspectionShowing->vPreSmoothImage;
				//		vRemoveJarPunt = vInspectionShowing->vRemoveJarPunt;
				//		vTrimEdgesOfJarAtROI = vInspectionShowing->vTrimEdgesOfJar;
				//		vFillInLightSpots = 0xAA;
				//		TempChanged = true;
				//	}
				//break;
				case 14:
					if (vInspectionShowing->vFillInLightSpots)
					{
						vShowReferenceAdjustedImage = TempProductReference & 1;
						vWhiteOutAnythingTouchingTheTop = vInspectionShowing->vWhiteOutAnythingTouchingTheTop;
						vFillBackground = vInspectionShowing->vFillBackground;
						vShowDilationCount = vInspectionShowing->vDilationCount;
						vShowErodeCount = vInspectionShowing->vErodeCount;
						vShowErodeCountAtStart = vInspectionShowing->vErodeCountAtStart;
						vPreSmoothImage = vInspectionShowing->vPreSmoothImage;
						vRemoveJarPunt = vInspectionShowing->vRemoveJarPunt;
						vTrimEdgesOfJarAtROI = vInspectionShowing->vTrimEdgesOfJar;
						vFillInLightSpots = vInspectionShowing->vFillInLightSpots;
						TempChanged = true;
					}
				break;
				case 15:
					if (vInspectionShowing->vTakeDerivative)
					{
						vShowReferenceAdjustedImage = TempProductReference & 1;
						vWhiteOutAnythingTouchingTheTop = vInspectionShowing->vWhiteOutAnythingTouchingTheTop;
						vFillBackground = vInspectionShowing->vFillBackground;
						vShowDilationCount = vInspectionShowing->vDilationCount;
						vShowErodeCount = vInspectionShowing->vErodeCount;
						vShowErodeCountAtStart = vInspectionShowing->vErodeCountAtStart;
						vPreSmoothImage = vInspectionShowing->vPreSmoothImage;
						vRemoveJarPunt = vInspectionShowing->vRemoveJarPunt;
						vTrimEdgesOfJarAtROI = vInspectionShowing->vTrimEdgesOfJar;
						vFillInLightSpots = vInspectionShowing->vFillInLightSpots;
						vTakeDerivative = vInspectionShowing->vTakeDerivative;
						TempChanged = true;
					}
				break;
				case 16:
					if (vInspectionShowing->vBonesInspection)
					{
						vShowReferenceAdjustedImage = TempProductReference & 1;
						vWhiteOutAnythingTouchingTheTop = vInspectionShowing->vWhiteOutAnythingTouchingTheTop;
						vFillBackground = vInspectionShowing->vFillBackground;
						vShowDilationCount = vInspectionShowing->vDilationCount;
						vShowErodeCount = vInspectionShowing->vErodeCount;
						vShowErodeCountAtStart = vInspectionShowing->vErodeCountAtStart;
						vPreSmoothImage = vInspectionShowing->vPreSmoothImage;
						vRemoveJarPunt = vInspectionShowing->vRemoveJarPunt;
						vTrimEdgesOfJarAtROI = vInspectionShowing->vTrimEdgesOfJar;
						vFillInLightSpots = vInspectionShowing->vFillInLightSpots;
						vTakeDerivative = vInspectionShowing->vTakeDerivative;
						vBonesInspection =vInspectionShowing->vBonesInspection;
						TempChanged = true;
					}
				break;
				case 17:
					if (vInspectionShowing->vTakeDifferenceFromMedianFilter)
					{//show median of matrix size going to take difference of
						vShowReferenceAdjustedImage = TempProductReference & 1;
						vWhiteOutAnythingTouchingTheTop = vInspectionShowing->vWhiteOutAnythingTouchingTheTop;
						vFillBackground = vInspectionShowing->vFillBackground;
						vShowDilationCount = vInspectionShowing->vDilationCount;
						vShowErodeCount = vInspectionShowing->vErodeCount;
						vShowErodeCountAtStart = vInspectionShowing->vErodeCountAtStart;
						vPreSmoothImage = vInspectionShowing->vPreSmoothImage;
						vRemoveJarPunt = vInspectionShowing->vRemoveJarPunt;
						vTrimEdgesOfJarAtROI = vInspectionShowing->vTrimEdgesOfJar;
						vFillInLightSpots = vInspectionShowing->vFillInLightSpots;
						vTakeDerivative = vInspectionShowing->vTakeDerivative;
						vBonesInspection =vInspectionShowing->vBonesInspection;
						vTakeDifferenceFromMedianFilter = vInspectionShowing->vTakeDifferenceFromMedianFilter | 0x80;
						TempChanged = true;
					}
				break;
				case 18:
					if (vInspectionShowing->vTakeDifferenceFromMedianFilter)
					{//show median of matrix size going to take difference of
						vShowReferenceAdjustedImage = TempProductReference & 1;
						vWhiteOutAnythingTouchingTheTop = vInspectionShowing->vWhiteOutAnythingTouchingTheTop;
						vFillBackground = vInspectionShowing->vFillBackground;
						vShowDilationCount = vInspectionShowing->vDilationCount;
						vShowErodeCount = vInspectionShowing->vErodeCount;
						vShowErodeCountAtStart = vInspectionShowing->vErodeCountAtStart;
						vPreSmoothImage = vInspectionShowing->vPreSmoothImage;
						vRemoveJarPunt = vInspectionShowing->vRemoveJarPunt;
						vTrimEdgesOfJarAtROI = vInspectionShowing->vTrimEdgesOfJar;
						vFillInLightSpots = vInspectionShowing->vFillInLightSpots;
						vTakeDerivative = vInspectionShowing->vTakeDerivative;
						vBonesInspection =vInspectionShowing->vBonesInspection;
						vTakeDifferenceFromMedianFilter = vInspectionShowing->vTakeDifferenceFromMedianFilter;
						TempChanged = true;
					}
				break;
				case 19:
					if ((vInspectionShowing->vTakeDifferenceFromMedian) && (!(vInspectionShowing->vTakeDifferenceFromMedian & 0x80)))
					{//show median of matrix size going to take difference of
						vShowReferenceAdjustedImage = TempProductReference & 1;
						vWhiteOutAnythingTouchingTheTop = vInspectionShowing->vWhiteOutAnythingTouchingTheTop;
						vFillBackground = vInspectionShowing->vFillBackground;
						vShowDilationCount = vInspectionShowing->vDilationCount;
						vShowErodeCount = vInspectionShowing->vErodeCount;
						vShowErodeCountAtStart = vInspectionShowing->vErodeCountAtStart;
						vPreSmoothImage = vInspectionShowing->vPreSmoothImage;
						vRemoveJarPunt = vInspectionShowing->vRemoveJarPunt;
						vTrimEdgesOfJarAtROI = vInspectionShowing->vTrimEdgesOfJar;
						vFillInLightSpots = vInspectionShowing->vFillInLightSpots;
						vTakeDerivative = vInspectionShowing->vTakeDerivative;
						vBonesInspection =vInspectionShowing->vBonesInspection;
						vTakeDifferenceFromMedianFilter = vInspectionShowing->vTakeDifferenceFromMedianFilter;
						TempChanged = true;
					}
				break;
				case 20:
					if (vInspectionShowing->vTakeDifferenceFromMedian)
					{
						vShowReferenceAdjustedImage = TempProductReference & 1;
						vWhiteOutAnythingTouchingTheTop = vInspectionShowing->vWhiteOutAnythingTouchingTheTop;
						vFillBackground = vInspectionShowing->vFillBackground;
						vShowDilationCount = vInspectionShowing->vDilationCount;
						vShowErodeCount = vInspectionShowing->vErodeCount;
						vShowErodeCountAtStart = vInspectionShowing->vErodeCountAtStart;
						vPreSmoothImage = vInspectionShowing->vPreSmoothImage;
						vRemoveJarPunt = vInspectionShowing->vRemoveJarPunt;
						vTrimEdgesOfJarAtROI = vInspectionShowing->vTrimEdgesOfJar;
						vFillInLightSpots = vInspectionShowing->vFillInLightSpots;
						vTakeDifferenceFromMedianFilter = vInspectionShowing->vTakeDifferenceFromMedianFilter;
						vTakeDerivative = vInspectionShowing->vTakeDerivative;
						vBonesInspection =vInspectionShowing->vBonesInspection;
						vTakeDifferenceFromMedian = vInspectionShowing->vTakeDifferenceFromMedian;
						TempChanged = true;
					}
				break;
				case 21:
					if (vInspectionShowing->vFindSpotInSubROI)
					{
						vShowReferenceAdjustedImage = TempProductReference & 1;
						vWhiteOutAnythingTouchingTheTop = vInspectionShowing->vWhiteOutAnythingTouchingTheTop;
						vFillBackground = vInspectionShowing->vFillBackground;
						vShowDilationCount = vInspectionShowing->vDilationCount;
						vShowErodeCount = vInspectionShowing->vErodeCount;
						vShowErodeCountAtStart = vInspectionShowing->vErodeCountAtStart;
						vPreSmoothImage = vInspectionShowing->vPreSmoothImage;
						vRemoveJarPunt = vInspectionShowing->vRemoveJarPunt;
						vTrimEdgesOfJarAtROI = vInspectionShowing->vTrimEdgesOfJar;
						vFillInLightSpots = vInspectionShowing->vFillInLightSpots;
						vTakeDifferenceFromMedianFilter = vInspectionShowing->vTakeDifferenceFromMedianFilter;
						vTakeDerivative = vInspectionShowing->vTakeDerivative;
						vBonesInspection =vInspectionShowing->vBonesInspection;
						vTakeDifferenceFromMedian = vInspectionShowing->vTakeDifferenceFromMedian;
						vFindSpotInSubROI = vInspectionShowing->vFindSpotInSubROI;
						TempChanged = true;
					}
				break;
				case 22:
					if (vInspectionShowing->vFindDarkSpotOnLine)
					{
						vShowReferenceAdjustedImage = TempProductReference & 1;
						vWhiteOutAnythingTouchingTheTop = vInspectionShowing->vWhiteOutAnythingTouchingTheTop;
						vFillBackground = vInspectionShowing->vFillBackground;
						vShowDilationCount = vInspectionShowing->vDilationCount;
						vShowErodeCount = vInspectionShowing->vErodeCount;
						vShowErodeCountAtStart = vInspectionShowing->vErodeCountAtStart;
						vPreSmoothImage = vInspectionShowing->vPreSmoothImage;
						vRemoveJarPunt = vInspectionShowing->vRemoveJarPunt;
						vTrimEdgesOfJarAtROI = vInspectionShowing->vTrimEdgesOfJar;
						vFillInLightSpots = vInspectionShowing->vFillInLightSpots;
						vTakeDifferenceFromMedianFilter = vInspectionShowing->vTakeDifferenceFromMedianFilter;
						vTakeDerivative = vInspectionShowing->vTakeDerivative;
						vBonesInspection =vInspectionShowing->vBonesInspection;
						vTakeDifferenceFromMedian = vInspectionShowing->vTakeDifferenceFromMedian;
						vFindSpotInSubROI = vInspectionShowing->vFindSpotInSubROI;
						vFindDarkSpotOnLine = vInspectionShowing->vFindDarkSpotOnLine;
						TempChanged = true;
					}
				break;
				case 23:
					if (vInspectionShowing->vPerformGlassAlgorithm)
					{
						vShowReferenceAdjustedImage = TempProductReference & 1;
						vWhiteOutAnythingTouchingTheTop = vInspectionShowing->vWhiteOutAnythingTouchingTheTop;
						vFillBackground = vInspectionShowing->vFillBackground;
						vShowDilationCount = vInspectionShowing->vDilationCount;
						vShowErodeCount = vInspectionShowing->vErodeCount;
						vShowErodeCountAtStart = vInspectionShowing->vErodeCountAtStart;
						vPreSmoothImage = vInspectionShowing->vPreSmoothImage;
						vRemoveJarPunt = vInspectionShowing->vRemoveJarPunt;
						vTrimEdgesOfJarAtROI = vInspectionShowing->vTrimEdgesOfJar;
						vFillInLightSpots = vInspectionShowing->vFillInLightSpots;
						vTakeDifferenceFromMedianFilter = vInspectionShowing->vTakeDifferenceFromMedianFilter;
						vTakeDerivative = vInspectionShowing->vTakeDerivative;
						vBonesInspection =vInspectionShowing->vBonesInspection;
						vTakeDifferenceFromMedian = vInspectionShowing->vTakeDifferenceFromMedian;
						vFindSpotInSubROI = vInspectionShowing->vFindSpotInSubROI;
						vFindDarkSpotOnLine = vInspectionShowing->vFindDarkSpotOnLine;
						vShowLineDerivative = vInspectionShowing->vPerformGlassAlgorithm;
						TempChanged = true;
					}
				break;
				case 24:
					if (vInspectionShowing->vPerformGlassAlgorithm == 2)
					{
						vShowReferenceAdjustedImage = TempProductReference & 1;
						vWhiteOutAnythingTouchingTheTop = vInspectionShowing->vWhiteOutAnythingTouchingTheTop;
						vFillBackground = vInspectionShowing->vFillBackground;
						vShowDilationCount = vInspectionShowing->vDilationCount;
						vShowErodeCount = vInspectionShowing->vErodeCount;
						vShowErodeCountAtStart = vInspectionShowing->vErodeCountAtStart;
						vPreSmoothImage = vInspectionShowing->vPreSmoothImage;
						vRemoveJarPunt = vInspectionShowing->vRemoveJarPunt;
						vTrimEdgesOfJarAtROI = vInspectionShowing->vTrimEdgesOfJar;
						vFillInLightSpots = vInspectionShowing->vFillInLightSpots;
						vTakeDifferenceFromMedianFilter = vInspectionShowing->vTakeDifferenceFromMedianFilter;
						vTakeDerivative = vInspectionShowing->vTakeDerivative;
						vBonesInspection =vInspectionShowing->vBonesInspection;
						vTakeDifferenceFromMedian = vInspectionShowing->vTakeDifferenceFromMedian;
						vFindSpotInSubROI = vInspectionShowing->vFindSpotInSubROI;
						vFindDarkSpotOnLine = vInspectionShowing->vFindDarkSpotOnLine;
						vShowLineDerivative = vInspectionShowing->vPerformGlassAlgorithm;
						vRequire2InARow = 1;
						TempChanged = true;
					}
				break;
				case 25:
					if (vInspectionShowing->vLookForDarkSpots)
					{
						vShowReferenceAdjustedImage = TempProductReference & 1;
						vWhiteOutAnythingTouchingTheTop = vInspectionShowing->vWhiteOutAnythingTouchingTheTop;
						vFillBackground = vInspectionShowing->vFillBackground;
						vShowDilationCount = vInspectionShowing->vDilationCount;
						vShowErodeCount = vInspectionShowing->vErodeCount;
						vShowErodeCountAtStart = vInspectionShowing->vErodeCountAtStart;
						vPreSmoothImage = vInspectionShowing->vPreSmoothImage;
						vTrimEdgesOfJarAtROI = (vInspectionShowing->vTrimEdgesOfJar); //&& (!vInspectionShowing->vDoContaminant22Algorithm));
						if (vInspectionShowing->vFindSpotInSubROI)
						if (vInspectionShowing->vDesensitizeIrregularArea)
							vUseVariabiltyImage = 4;
						vRemoveJarPunt = vInspectionShowing->vRemoveJarPunt & 0x7F;
						vFillInLightSpots = vInspectionShowing->vFillInLightSpots;
						//vPreSelectDarkSpotsInImage =vInspectionShowing->vPreSelectDarkSpotsInImage;
						vTakeDifferenceFromMedian = vInspectionShowing->vTakeDifferenceFromMedian;
						vFindSpotInSubROI = vInspectionShowing->vFindSpotInSubROI;
						vLookForDarkSpots = vInspectionShowing->vLookForDarkSpots;
						TempChanged = true;
					}
				break;
				case 26:
					if (vInspectionShowing->vDoContaminant22Algorithm)
					{
						vShowReferenceAdjustedImage = TempProductReference & 1;
						vWhiteOutAnythingTouchingTheTop = vInspectionShowing->vWhiteOutAnythingTouchingTheTop;
						vFillBackground = vInspectionShowing->vFillBackground;
						vShowDilationCount = vInspectionShowing->vDilationCount;
						vShowErodeCount = vInspectionShowing->vErodeCount;
						vShowErodeCountAtStart = vInspectionShowing->vErodeCountAtStart;
						vPreSmoothImage = vInspectionShowing->vPreSmoothImage;
						vRemoveJarPunt = vInspectionShowing->vRemoveJarPunt;
						vTrimEdgesOfJarAtROI = vInspectionShowing->vTrimEdgesOfJar;
						vFillInLightSpots = vInspectionShowing->vFillInLightSpots;
						vTakeDifferenceFromMedianFilter = vInspectionShowing->vTakeDifferenceFromMedianFilter;
						vTakeDerivative = vInspectionShowing->vTakeDerivative;
						vBonesInspection =vInspectionShowing->vBonesInspection;
						vTakeDifferenceFromMedian = vInspectionShowing->vTakeDifferenceFromMedian;
						vFindSpotInSubROI = vInspectionShowing->vFindSpotInSubROI;
						vLookForDarkSpots = vInspectionShowing->vLookForDarkSpots;
						vFindDarkSpotOnLine = vInspectionShowing->vFindDarkSpotOnLine;
						vShowLineDerivative = vInspectionShowing->vPerformGlassAlgorithm;
						vRequire2InARow = (vInspectionShowing->vPerformGlassAlgorithm == 2);
						vDoContaminant22Algorithm = vInspectionShowing->vDoContaminant22Algorithm;
						TempChanged = true;
					}
				break;
				case 27:
					if ((vLocalSystemData->vITIPCDig->vProductAverageImage) &&
						((vInspectionShowing->vSubtractAverage) || (vInspectionShowing->vAddStructure)))
					{
						vShowReferenceAdjustedImage = TempProductReference & 1;
						vWhiteOutAnythingTouchingTheTop = vInspectionShowing->vWhiteOutAnythingTouchingTheTop;
						vFillBackground = vInspectionShowing->vFillBackground;
						vShowDilationCount = vInspectionShowing->vDilationCount;
						vShowErodeCount = vInspectionShowing->vErodeCount;
						vShowErodeCountAtStart = vInspectionShowing->vErodeCountAtStart;
						vPreSmoothImage = vInspectionShowing->vPreSmoothImage;
						vRemoveJarPunt = vInspectionShowing->vRemoveJarPunt;
						vTrimEdgesOfJarAtROI = vInspectionShowing->vTrimEdgesOfJar;
						vFillInLightSpots = vInspectionShowing->vFillInLightSpots;
						vTakeDifferenceFromMedianFilter = vInspectionShowing->vTakeDifferenceFromMedianFilter;
						vTakeDerivative = vInspectionShowing->vTakeDerivative;
						vBonesInspection =vInspectionShowing->vBonesInspection;
						vTakeDifferenceFromMedian = vInspectionShowing->vTakeDifferenceFromMedian;
						vFindSpotInSubROI = vInspectionShowing->vFindSpotInSubROI;
						vLookForDarkSpots = vInspectionShowing->vLookForDarkSpots;
						vFindDarkSpotOnLine = vInspectionShowing->vFindDarkSpotOnLine;
						vShowLineDerivative = vInspectionShowing->vPerformGlassAlgorithm;
						vRequire2InARow = (vInspectionShowing->vPerformGlassAlgorithm == 2);
						vDoContaminant22Algorithm = vInspectionShowing->vDoContaminant22Algorithm;
						vShowSubtractAverage = true; //go through average even if only doing structure
						TempChanged = true;
					}
				break;
				case 28:
					if ((vLocalSystemData->vITIPCDig->vProductContaminant2StructureImage) && (vInspectionShowing->vAddStructure))
					{
						vShowReferenceAdjustedImage = TempProductReference & 1;
						vWhiteOutAnythingTouchingTheTop = vInspectionShowing->vWhiteOutAnythingTouchingTheTop;
						vFillBackground = vInspectionShowing->vFillBackground;
						vShowDilationCount = vInspectionShowing->vDilationCount;
						vShowErodeCount = vInspectionShowing->vErodeCount;
						vShowErodeCountAtStart = vInspectionShowing->vErodeCountAtStart;
						vPreSmoothImage = vInspectionShowing->vPreSmoothImage;
						vRemoveJarPunt = vInspectionShowing->vRemoveJarPunt;
						vTrimEdgesOfJarAtROI = vInspectionShowing->vTrimEdgesOfJar;
						vFillInLightSpots = vInspectionShowing->vFillInLightSpots;
						vTakeDifferenceFromMedianFilter = vInspectionShowing->vTakeDifferenceFromMedianFilter;
						vTakeDerivative = vInspectionShowing->vTakeDerivative;
						vBonesInspection =vInspectionShowing->vBonesInspection;
						vTakeDifferenceFromMedian = vInspectionShowing->vTakeDifferenceFromMedian;
						vFindSpotInSubROI = vInspectionShowing->vFindSpotInSubROI;
						vLookForDarkSpots = vInspectionShowing->vLookForDarkSpots;
						vFindDarkSpotOnLine = vInspectionShowing->vFindDarkSpotOnLine;
						vShowLineDerivative = vInspectionShowing->vPerformGlassAlgorithm;
						vRequire2InARow = (vInspectionShowing->vPerformGlassAlgorithm == 2);
						vDoContaminant22Algorithm = vInspectionShowing->vDoContaminant22Algorithm;
						vShowSubtractAverage = ((vInspectionShowing->vSubtractAverage) && 
							(vLocalSystemData->vITIPCDig->vProductAverageImage));

						vShowAddStructure = ((vLocalSystemData->vITIPCDig->vProductContaminant2StructureImage) && (vInspectionShowing->vAddStructure));
						TempChanged = true;
					}
				break;
				case 29:
					if (vInspectionShowing->vIPContaminant)
					{
						vShowReferenceAdjustedImage = TempProductReference & 1;
						vWhiteOutAnythingTouchingTheTop = vInspectionShowing->vWhiteOutAnythingTouchingTheTop;
						vFillBackground = vInspectionShowing->vFillBackground;
						vShowDilationCount = vInspectionShowing->vDilationCount;
						vShowErodeCount = vInspectionShowing->vErodeCount;
						vShowErodeCountAtStart = vInspectionShowing->vErodeCountAtStart;
						vPreSmoothImage = vInspectionShowing->vPreSmoothImage;
						vRemoveJarPunt = vInspectionShowing->vRemoveJarPunt;
						vTrimEdgesOfJarAtROI = vInspectionShowing->vTrimEdgesOfJar;
						vFillInLightSpots = vInspectionShowing->vFillInLightSpots;
						vTakeDifferenceFromMedianFilter = vInspectionShowing->vTakeDifferenceFromMedianFilter;
						vTakeDerivative = vInspectionShowing->vTakeDerivative;
						vBonesInspection =vInspectionShowing->vBonesInspection;
						vTakeDifferenceFromMedian = vInspectionShowing->vTakeDifferenceFromMedian;
						vFindSpotInSubROI = vInspectionShowing->vFindSpotInSubROI;
						vLookForDarkSpots = vInspectionShowing->vLookForDarkSpots;
						vFindDarkSpotOnLine = vInspectionShowing->vFindDarkSpotOnLine;
						vShowLineDerivative = vInspectionShowing->vPerformGlassAlgorithm;
						vRequire2InARow = (vInspectionShowing->vPerformGlassAlgorithm == 2);
						vDoContaminant22Algorithm = vInspectionShowing->vDoContaminant22Algorithm;
						vShowSubtractAverage = ((vInspectionShowing->vSubtractAverage) && (vLocalSystemData->vITIPCDig->vProductAverageImage));
						vShowAddStructure = ((vLocalSystemData->vITIPCDig->vProductContaminant2StructureImage) && (vInspectionShowing->vAddStructure));
						vIPContaminant = vInspectionShowing->vIPContaminant;
						TempChanged = true;
					}
				break;
				case 30:  //enhance method
					if (vInspectionShowing->vEnhanceMethod)
					{
						vShowReferenceAdjustedImage = TempProductReference & 1;
						vWhiteOutAnythingTouchingTheTop = vInspectionShowing->vWhiteOutAnythingTouchingTheTop;
						vFillBackground = vInspectionShowing->vFillBackground;
						vShowDilationCount = vInspectionShowing->vDilationCount;
						vShowErodeCount = vInspectionShowing->vErodeCount;
						vShowErodeCountAtStart = vInspectionShowing->vErodeCountAtStart;
						vPreSmoothImage = vInspectionShowing->vPreSmoothImage;
						vRemoveJarPunt = vInspectionShowing->vRemoveJarPunt;
						vTrimEdgesOfJarAtROI = vInspectionShowing->vTrimEdgesOfJar;
						vFillInLightSpots = vInspectionShowing->vFillInLightSpots;
						vTakeDifferenceFromMedianFilter = vInspectionShowing->vTakeDifferenceFromMedianFilter;
						vTakeDerivative = vInspectionShowing->vTakeDerivative;
						vBonesInspection =vInspectionShowing->vBonesInspection;
						vTakeDifferenceFromMedian = vInspectionShowing->vTakeDifferenceFromMedian;
						vFindSpotInSubROI = vInspectionShowing->vFindSpotInSubROI;
						vLookForDarkSpots = vInspectionShowing->vLookForDarkSpots;
						vFindDarkSpotOnLine = vInspectionShowing->vFindDarkSpotOnLine;
						vShowLineDerivative = vInspectionShowing->vPerformGlassAlgorithm;
						vRequire2InARow = (vInspectionShowing->vPerformGlassAlgorithm == 2);
						vDoContaminant22Algorithm = vInspectionShowing->vDoContaminant22Algorithm;
						vShowSubtractAverage = ((vInspectionShowing->vSubtractAverage) && (vLocalSystemData->vITIPCDig->vProductAverageImage));
						vShowAddStructure = ((vLocalSystemData->vITIPCDig->vProductContaminant2StructureImage) && (vInspectionShowing->vAddStructure));
						vIPContaminant = vInspectionShowing->vIPContaminant;
						vDisplayEnhancedImage = vInspectionShowing->vEnhanceMethod;
						TempChanged = true;
					}
				break;
				//case 26:
				//	if ((vInspectionShowing->vDoImageFactors) &&
				//		(vInspectionShowing->vOriginalImageFactor) && 
				//		(vInspectionShowing->vEnhancedImageFactor))
				//	{
				//		vShowReferenceAdjustedImage = TempProductReference & 1;
				//		vFillBackground = vInspectionShowing->vFillBackground;
				//		vPreSmoothImage = vInspectionShowing->vPreSmoothImage;
				//		vShowDilation = vInspectionShowing->vDoDilation;
				//		vShowErodeTimes = vInspectionShowing->vErodeCount;
				//		vShowDilationCount = vInspectionShowing->vDilationCount;
				//		vRemoveJarPunt = vInspectionShowing->vRemoveJarPunt;
				//		vTrimEdgesOfJarAtROI = vInspectionShowing->vTrimEdgesOfJar;
				//		vFillInLightSpots = vInspectionShowing->vFillInLightSpots;
				//		vTakeDifferenceFromMedianFilter = vInspectionShowing->vTakeDifferenceFromMedianFilter;
				//		vTakeDerivative = vInspectionShowing->vTakeDerivative;
				//		vTakeDifferenceFromMedian = vInspectionShowing->vTakeDifferenceFromMedian;
				//		vFindSpotInSubROI = vInspectionShowing->vFindSpotInSubROI;
				//		vFindDarkSpotOnLine = vInspectionShowing->vFindDarkSpotOnLine;
				//		vShowLineDerivative = vInspectionShowing->vPerformGlassAlgorithm;
				//		vRequire2InARow = (vInspectionShowing->vPerformGlassAlgorithm == 2);
				//		vDoContaminant22Algorithm = vInspectionShowing->vDoContaminant22Algorithm;
				//		vShowSubtractAverage = ((vInspectionShowing->vSubtractAverage) && 
				//			(vLocalSystemData->vITIPCDig->vProductAverageImage));

				//		vShowAddStructure = ((vLocalSystemData->vITIPCDig->vProductContaminant2StructureImage) && (vInspectionShowing->vAddStructure));
				//		vDisplayEnhancedImage = vInspectionShowing->vEnhanceMethod;
				//		if ((vInspectionShowing->vDoImageFactors) &&
				//			(vInspectionShowing->vOriginalImageFactor) && 
				//			(vInspectionShowing->vEnhancedImageFactor))
				//		{
				//			vShowImageFactors = true;
				//			vShowOriginalImageFactor = vInspectionShowing->vOriginalImageFactor;
				//			vShowEnhancedImageFactor = vInspectionShowing->vEnhancedImageFactor;
				//		}
				//		TempChanged = true;
				//	}
				//break;
				case 31:
					if (vInspectionShowing->vErodeCountAtEnd)
					{
						vShowReferenceAdjustedImage = TempProductReference & 1;
						vWhiteOutAnythingTouchingTheTop = vInspectionShowing->vWhiteOutAnythingTouchingTheTop;
						vFillBackground = vInspectionShowing->vFillBackground;
						vShowDilationCount = vInspectionShowing->vDilationCount;
						vShowErodeCount = vInspectionShowing->vErodeCount;
						vShowErodeCountAtStart = vInspectionShowing->vErodeCountAtStart;
						vPreSmoothImage = vInspectionShowing->vPreSmoothImage;
						vRemoveJarPunt = vInspectionShowing->vRemoveJarPunt;
						vTrimEdgesOfJarAtROI = vInspectionShowing->vTrimEdgesOfJar;
						vFillInLightSpots = vInspectionShowing->vFillInLightSpots;
						vTakeDifferenceFromMedianFilter = vInspectionShowing->vTakeDifferenceFromMedianFilter;
						vTakeDerivative = vInspectionShowing->vTakeDerivative;
						vBonesInspection =vInspectionShowing->vBonesInspection;
						vTakeDifferenceFromMedian = vInspectionShowing->vTakeDifferenceFromMedian;
						vFindSpotInSubROI = vInspectionShowing->vFindSpotInSubROI;
						vLookForDarkSpots = vInspectionShowing->vLookForDarkSpots;
						vFindDarkSpotOnLine = vInspectionShowing->vFindDarkSpotOnLine;
						vShowLineDerivative = vInspectionShowing->vPerformGlassAlgorithm;
						vRequire2InARow = (vInspectionShowing->vPerformGlassAlgorithm == 2);
						vDoContaminant22Algorithm = vInspectionShowing->vDoContaminant22Algorithm;
						vShowSubtractAverage = ((vInspectionShowing->vSubtractAverage) && (vLocalSystemData->vITIPCDig->vProductAverageImage));
						vShowAddStructure = ((vLocalSystemData->vITIPCDig->vProductContaminant2StructureImage) && (vInspectionShowing->vAddStructure));
						vIPContaminant = vInspectionShowing->vIPContaminant;
						vDisplayEnhancedImage = vInspectionShowing->vEnhanceMethod;
						vShowErodeCountAtEnd = vInspectionShowing->vErodeCountAtEnd;
						TempChanged = true;
					}
				break;
				case 32:
					if (vInspectionShowing->vDilationCountAtEnd)
					{
						vShowReferenceAdjustedImage = TempProductReference & 1;
						vWhiteOutAnythingTouchingTheTop = vInspectionShowing->vWhiteOutAnythingTouchingTheTop;
						vFillBackground = vInspectionShowing->vFillBackground;
						vShowDilationCount = vInspectionShowing->vDilationCount;
						vShowErodeCount = vInspectionShowing->vErodeCount;
						vShowErodeCountAtStart = vInspectionShowing->vErodeCountAtStart;
						vPreSmoothImage = vInspectionShowing->vPreSmoothImage;
						vRemoveJarPunt = vInspectionShowing->vRemoveJarPunt;
						vTrimEdgesOfJarAtROI = vInspectionShowing->vTrimEdgesOfJar;
						vFillInLightSpots = vInspectionShowing->vFillInLightSpots;
						vTakeDifferenceFromMedianFilter = vInspectionShowing->vTakeDifferenceFromMedianFilter;
						vTakeDerivative = vInspectionShowing->vTakeDerivative;
						vBonesInspection =vInspectionShowing->vBonesInspection;
						vTakeDifferenceFromMedian = vInspectionShowing->vTakeDifferenceFromMedian;
						vFindSpotInSubROI = vInspectionShowing->vFindSpotInSubROI;
						vLookForDarkSpots = vInspectionShowing->vLookForDarkSpots;
						vFindDarkSpotOnLine = vInspectionShowing->vFindDarkSpotOnLine;
						vShowLineDerivative = vInspectionShowing->vPerformGlassAlgorithm;
						vRequire2InARow = (vInspectionShowing->vPerformGlassAlgorithm == 2);
						vDoContaminant22Algorithm = vInspectionShowing->vDoContaminant22Algorithm;
						vShowSubtractAverage = ((vInspectionShowing->vSubtractAverage) && (vLocalSystemData->vITIPCDig->vProductAverageImage));
						vShowAddStructure = ((vLocalSystemData->vITIPCDig->vProductContaminant2StructureImage) && (vInspectionShowing->vAddStructure));
						vIPContaminant = vInspectionShowing->vIPContaminant;
						vDisplayEnhancedImage = vInspectionShowing->vEnhanceMethod;
						vShowErodeCountAtEnd = vInspectionShowing->vErodeCountAtEnd;
						vShowDilationCountAtEnd = vInspectionShowing->vDilationCountAtEnd;
						TempChanged = true;
					}
				break;
				case 33:
					if ((vLocalSystemData->vITIPCDig->vProductHistogramImage) && (vInspectionShowing->vAddHistogram)) //|| 
						//((vDisplayingImageData) && (vDisplayingImageData->vContainerLearnState) && (vInspectionShowing->vAddStructure))))
					{
						vShowReferenceAdjustedImage = TempProductReference & 1;
						vWhiteOutAnythingTouchingTheTop = vInspectionShowing->vWhiteOutAnythingTouchingTheTop;
						vFillBackground = vInspectionShowing->vFillBackground;
						vShowDilationCount = vInspectionShowing->vDilationCount;
						vShowErodeCount = vInspectionShowing->vErodeCount;
						vShowErodeCountAtStart = vInspectionShowing->vErodeCountAtStart;
						vPreSmoothImage = vInspectionShowing->vPreSmoothImage;
						vRemoveJarPunt = vInspectionShowing->vRemoveJarPunt;
						vTrimEdgesOfJarAtROI = vInspectionShowing->vTrimEdgesOfJar;
						vFillInLightSpots = vInspectionShowing->vFillInLightSpots;
						vTakeDifferenceFromMedianFilter = vInspectionShowing->vTakeDifferenceFromMedianFilter;
						vTakeDerivative = vInspectionShowing->vTakeDerivative;
						vBonesInspection =vInspectionShowing->vBonesInspection;
						vTakeDifferenceFromMedian = vInspectionShowing->vTakeDifferenceFromMedian;
						vFindSpotInSubROI = vInspectionShowing->vFindSpotInSubROI;
						vLookForDarkSpots = vInspectionShowing->vLookForDarkSpots;
						vFindDarkSpotOnLine = vInspectionShowing->vFindDarkSpotOnLine;
						vShowLineDerivative = vInspectionShowing->vPerformGlassAlgorithm;
						vRequire2InARow = (vInspectionShowing->vPerformGlassAlgorithm == 2);
						vDoContaminant22Algorithm = vInspectionShowing->vDoContaminant22Algorithm;
						vShowAddStructure = ((vLocalSystemData->vITIPCDig->vProductContaminant2StructureImage) && (vInspectionShowing->vAddStructure));
						vIPContaminant = vInspectionShowing->vIPContaminant;
						vDisplayEnhancedImage = vInspectionShowing->vEnhanceMethod;
						vShowErodeCountAtEnd = vInspectionShowing->vErodeCountAtEnd;
						vShowDilationCountAtEnd = vInspectionShowing->vDilationCountAtEnd;
						vShowAddHistogram = true;
						TempChanged = true;
					}
				break;
				case 34:  //threshold
					//if (vInspectionShowing->vThreshold)
					{
						vShowReferenceAdjustedImage = TempProductReference & 1;
						vWhiteOutAnythingTouchingTheTop = vInspectionShowing->vWhiteOutAnythingTouchingTheTop;
						vFillBackground = vInspectionShowing->vFillBackground;
						vShowDilationCount = vInspectionShowing->vDilationCount;
						vShowErodeCount = vInspectionShowing->vErodeCount;
						vShowErodeCountAtStart = vInspectionShowing->vErodeCountAtStart;
						vPreSmoothImage = vInspectionShowing->vPreSmoothImage;
						vRemoveJarPunt = vInspectionShowing->vRemoveJarPunt;
						vTrimEdgesOfJarAtROI = vInspectionShowing->vTrimEdgesOfJar;
						vFillInLightSpots = vInspectionShowing->vFillInLightSpots;
						vTakeDifferenceFromMedianFilter = vInspectionShowing->vTakeDifferenceFromMedianFilter;
						vTakeDerivative = vInspectionShowing->vTakeDerivative;
						vBonesInspection =vInspectionShowing->vBonesInspection;
						vTakeDifferenceFromMedian = vInspectionShowing->vTakeDifferenceFromMedian;
						vFindSpotInSubROI = vInspectionShowing->vFindSpotInSubROI;
						vLookForDarkSpots = vInspectionShowing->vLookForDarkSpots;
						vFindDarkSpotOnLine = vInspectionShowing->vFindDarkSpotOnLine;
						vShowLineDerivative = vInspectionShowing->vPerformGlassAlgorithm;
						vRequire2InARow = (vInspectionShowing->vPerformGlassAlgorithm == 2);
						vDoContaminant22Algorithm = vInspectionShowing->vDoContaminant22Algorithm;
						vShowAddStructure = ((vLocalSystemData->vITIPCDig->vProductContaminant2StructureImage) && (vInspectionShowing->vAddStructure));
						vIPContaminant = vInspectionShowing->vIPContaminant;
						vDisplayEnhancedImage = vInspectionShowing->vEnhanceMethod;
						vShowErodeCountAtEnd = vInspectionShowing->vErodeCountAtEnd;
						vShowDilationCountAtEnd = vInspectionShowing->vDilationCountAtEnd;
						vShowAddHistogram = ((vLocalSystemData->vITIPCDig->vProductHistogramImage) && (vInspectionShowing->vAddHistogram));
						vMaximumThreshold = 255;
						if (vInspectionShowing->vInspectionType == cUnderfillByWeightInspection)
						{
							vThreshold = vInspectionShowing->vDensityThresholdUpper;
							vMaximumThreshold = vMaximumUnderfillByWeightThreshold;
						}
						else
						if (vInspectionShowing->vInspectionType == cOverfillByWeightInspection)
						{
							vThreshold = vInspectionShowing->vDensityThresholdLower;
							vMaximumThreshold = vMaximumUnderfillByWeightThreshold;
						}
						else
						if (vInspectionShowing->vHasDensityUpperThreshold)
							vThreshold = vInspectionShowing->vDensityThresholdUpper;
						else
						if (vInspectionShowing->vHasDensityLowerThreshold)
							vThreshold = vInspectionShowing->vDensityThresholdLower;
						else
							vThreshold = vInspectionShowing->vThreshold;
						vShowThreshold = true;
						vEditROIBoundaries = false;
						vAnalysisImageDisplay = false;
						vShowingUpperThreshold = true;
						vShowThresholdMenu = true; 
						this->SetWindowText(_T("Rejects Threshold"));
						StartSensitivity();
						vDialogTitleString.LoadString(IDS_EditThreshold);
						SetDlgItemText(IDC_DialogTitleStaticText1,vDialogTitleString);
						TempChanged = true;
					}
				break;
			}
		}
	}
	WaitThenDrawImageOnScreen();
	UpdateButtons();
	UpdateDisplayIndicators();
}

void CNewImageDisplayDialog::DisplayLocationEdge()
{
	//vDrawPurpleLine = true;
	WORD TempOffsetX = (108 * vLocalSystemData->vGlobalMonitorSizeX) / cOldResolutionX;
	WORD TempOffsetY = (43 * vLocalSystemData->vGlobalMonitorSizeY) / cOldResolutionY;
	RECT TempRectangle;
	TempRectangle.left = TempOffsetX - 8;
	TempRectangle.top = TempOffsetY;
	TempRectangle.right = TempOffsetX + vLocalSystemData->vITIPCDig->vDisplayImageSizeX + 8;
	TempRectangle.bottom = TempOffsetY + vLocalSystemData->vITIPCDig->vDisplayImageSizeY;
	// could set a timer to invalidate in 200 milliseconds
	this->InvalidateRect(&TempRectangle, false);
}

void CNewImageDisplayDialog::EditInspectionSettings()
{
	if (vInspectionShowing)
	{
		CConfigureProductContaminantProcessesDialog IConfigureProductContaminantProcessesDialog;  
		
		//Set dialog box data like how many products there are
		IConfigureProductContaminantProcessesDialog.vPointerToProductEditing = 
			vGlobalCurrentProduct;
		IConfigureProductContaminantProcessesDialog.vInspectionEditing = vInspectionShowing;

		IConfigureProductContaminantProcessesDialog.vLocalSystemData = vLocalSystemData;

		IConfigureProductContaminantProcessesDialog.vLocalConfigurationData = 
			vLocalConfigurationData;
		IConfigureProductContaminantProcessesDialog.vLocalSystemData = vLocalSystemData;
		//Pass control to dialog box and display
		//IEditProductDialog.vEPLocalNumberOfProducts = vCPLocalNumberOfProducts;
		int nResponse = IConfigureProductContaminantProcessesDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			// TODO: Place code here to handle when the dialog is accepted
			if (IConfigureProductContaminantProcessesDialog.vChangeMade)
			{
				SetInspectionShowing(vInspectionShowing);
				if (IConfigureProductContaminantProcessesDialog.vChangeMade)
					ReportErrorMessage("Changed Settings for Product: " + 
						vLocalSystemData->vCurrentProductName, cUserChanges,0);
				DrawImageOnScreen();
			}
		}
		else 
		if (nResponse == 10)
		{
			//  Main Menu button was pressed
			Sleep(1); //is the equivelent of a yeild statement;
			PrepareAndExit(10);
		}
	}
}

void CNewImageDisplayDialog::ShowNextDataButton(int TempControlID, CFunctionButton *TempButton)
{
	//bool TempHaveBackground = 
	//			vGlobalCurrentProduct->ProductHasBackgroundFile(vGlobalCurrentDirectory);
			//(((vLocalConfigurationData->vRemoveBackgroundFromImage) || 
			//(vLocalSystemData->vRemoveBackgroundInAutoSize)) &&
			//(vGlobalCurrentProduct->ProductHasBackgroundFile(vGlobalCurrentDirectory)));
	if ((PasswordOK(cSuperInspxPassword,false)) && ((vLocalSystemData->vITIPCDig->vProductAverageImage) ||
		(vLocalSystemData->vITIPCDig->vAverageImage) || 
		(vLocalSystemData->vITIPCDig->vProductBackgroundImage)))
	{
		TempButton->EnableWindow(true);

		if ((vShowAverage) && (vLocalSystemData->vITIPCDig->vProductStructureImage))
			SetDlgItemText(TempControlID, _T("Show\nContam. Structure"));
		else
		if ((vShowAverage) && (vLocalSystemData->vITIPCDig->vProductHistogramImage))
			SetDlgItemText(TempControlID, _T("Show\nHistogram\nData"));
		else
		if (vShowAverage)
			SetDlgItemText(TempControlID, _T("Show Normal Image"));
		else
		if (((vShowAverage) || ((vShowStructure) && (vShowType == cShowTypeContaminant))) &&
			(vLocalSystemData->vITIPCDig->vProductContaminant2StructureImage))
			SetDlgItemText(TempControlID, _T("Show\nContam2\nStructure"));
		else
		if (((vShowAverage) || ((vShowStructure) && 
			((vShowType == cShowTypeContaminant2) || (vShowType == cShowTypeContaminant)))) && 
			((vLocalConfigurationData->vAllowVoidInspection) && ((vLocalSystemData->vITIPCDig->vVoidProductStructureImage) || (vLocalSystemData->vITIPCDig->vVoidStructureImage))))
			SetDlgItemText(TempControlID, _T("Show\nVoid Structure"));
		else
		//if ((vShowStructure) && (vLocalSystemData->vITIPCDig->vProductGlassStructureImage))
		//	SetDlgItemText(TempControlID,"Show\nGlass\nStructure");
		//else
		if ((vShowStructure) && (vLocalSystemData->vITIPCDig->vProductHistogramImage))
			SetDlgItemText(TempControlID, _T("Show\nHistogram\nData"));
		else
		//if ((vShowGlassStructure) && (vLocalSystemData->vITIPCDig->vProductHistogramImage))
		//	SetDlgItemText(TempControlID,"Show\nHistogram\nData");
		//else
		//if ((vShowHistogram) && (vLocalSystemData->vITIPCDig->vProductGlassHistogramImage))
		//	SetDlgItemText(TempControlID,"Show\nGlass\nHistogram");
		//else
		//if (vShowGlassHistogram)
		//	SetDlgItemText(TempControlID,"Show Normal Image");
		//else
		if (vShowHistogram)
			SetDlgItemText(TempControlID, _T("Show Normal Image"));
		else
		if (vShowBackground)
			SetDlgItemText(TempControlID, _T("Show\nAverage\nData"));
		else
		if (vLocalSystemData->vITIPCDig->vProductBackgroundImage)
			SetDlgItemText(TempControlID, _T("Show\nBack- ground"));
		else
			SetDlgItemText(TempControlID, _T("Show\nAverage\nData"));

		TempButton->ShowWindow(SW_SHOW);
	}
	else
	if (PasswordOK(cSuperInspxPassword,false))
	{
		//TempButton->EnableWindow(false);
		TempButton->ShowWindow(SW_SHOW);
		SetDlgItemText(TempControlID, _T("No Learn Data To Show"));
	}
	else
	{
		TempButton->ShowWindow(SW_HIDE);
		this->InvalidateRect(&TempButton->GetRectToInvalidateArrow());
	}
}

void CNewImageDisplayDialog::ToggleFilter(BYTE TempWhichFilter)
{
	if (HaveAnInspection(TempWhichFilter))
	{
		vGlobalCurrentProduct->vTypesOfRejectsToView = vGlobalCurrentProduct->vTypesOfRejectsToView ^ (1 << (TempWhichFilter - 1));
		if ((vLocalSystemData->vITIPCDig->vRejectBufferShowing != -1) || (vRejectsImageDisplay) || (vLocalSystemData->vITIPCDig->vImageDisplayOnlyRejects))
		{
			vLocalSystemData->vITIPCDig->vRejectBufferShowing = -1;
			ShowNextReject(true, false);
		}
		UpdateButtons();
	}
}

bool CNewImageDisplayDialog::HaveAnInspection(BYTE TempWhichInspection)
{
	if ((TempWhichInspection) && 
		(TempWhichInspection <= vTotalNumberOfInspectionsToDisplay))
		return true;
	else
	if ((PasswordOK(cTemporaryInspxPassword,false)) && (vLocalSystemData->vGoodImagesIndex == TempWhichInspection))
		return true;
	else
		return false;
}

CString CNewImageDisplayDialog::GetInspectionName(BYTE TempWhichInspection)
{
	CString TempString("");
	TempString = "";
	CString TempText("");
	TempText = "";
	if (HaveAnInspection(TempWhichInspection))
	{
		if ((PasswordOK(cTemporaryInspxPassword,false)) && (vLocalSystemData->vGoodImagesIndex == TempWhichInspection))
		{
			if (vGlobalCurrentProduct->vTypesOfRejectsToView & (0x0001 << (TempWhichInspection - 1)))
				TempString = "Hide\nGood Containers";
			else
				TempString = "Show\nGood Containers";
		}
		else
		if (vTotalNumberOfInspectionsToDisplay)
		{
			if (vGlobalCurrentProduct->vTypesOfRejectsToView & (0x0001 << (TempWhichInspection - 1)))
				TempText.LoadString(IDS_Hide);
			else
				TempText.LoadString(IDS_Show);

			//if (vLocalSystemData->vCriteriaString[TempWhichInspection - 1].GetLength() < 6)
			//	TempString = TempText + " \n" + vLocalSystemData->vCriteriaString[TempWhichInspection - 1];
			//else
				TempString = TempText + " " + vLocalSystemData->vCriteriaString[TempWhichInspection - 1];

		}
	}
	return TempString;
}

void CNewImageDisplayDialog::ShowImageDisplayInformation(CImageData *TempImage)
{
	//if (!vShowThreshold)  //if your not showing the threshold step, calculate image data for reject information
		CalculateImageDataInformation();
	// Show Image Display Information
	if ((TempImage) && (TempImage->vGreyImage) && (vHaveAnImageToShow) && (vDisplayingImageData))
	{
		CString TempAbreviation = "";
		CString TempString("");
		TempString = "";
		CString TempText("");
		CString TempText1("");
		CString TempText2("");
		TempText.LoadString(IDS_ImageTime);
		TempText1.LoadString(IDS_ContainerNumber);
		TempText2.LoadString(IDS_ImageType);
		//5/7/2011
		if (TempImage->vImageTypeString.GetLength() > 3)
			TempString = TempString + TempText + ": " + TempImage->vTime + ", " + TempText1 + ": " + dtoa(TempImage->vContainerNumber, 0) + ", " + TempText2 + ": " + TempImage->vImageTypeString;
		else
			TempString = TempString + TempText + ": " + TempImage->vTime + ", " + TempText1 + ": " + dtoa(TempImage->vContainerNumber, 0);

		if (vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType])
		{
			TempString = TempString + "\nFiller valve # " + dtoa(vMainWindowPointer->CalculateHeadNumber(((int)TempImage->vHeadNumber[cFillerSamplingType]), cFillerSamplingType), 0);
			if ((vLocalSystemData->vValveMonitorInspection < vGlobalCurrentProduct->vNumberOfInspections) && (vGlobalCurrentProduct->vInspection[vLocalSystemData->vValveMonitorInspection]))
				//TempString = TempString + "\nFill Level: " + dtoa(vGlobalCurrentProduct->vInspection[vLocalSystemData->vValveMonitorInspection]->ConvertToWeight(TempImage->vFillLevel), 2);  
				TempString = TempString + "\nFill Level: " + dtoa(TempImage->vFillLevel, 2);  //was saved as a weight, so don't convert to weight again
			else
			{
				if ((TempImage->vFillLevel < 0) || (TempImage->vFillLevel > 32000))
					TempImage->vFillLevel = 0;
				TempString = TempString + "\nFill Level: " + dtoa(TempImage->vFillLevel, 2);
			}
		}

		if (vLocalConfigurationData->vNumberOfHeadsToMonitor[cSeamerSamplingType])
			TempString = TempString + "\nSeamer Head # " + dtoa(vMainWindowPointer->CalculateHeadNumber(((int)TempImage->vHeadNumber[cSeamerSamplingType]), cSeamerSamplingType),0);

		BYTE TempEjectInspection = TempImage->vContainerEjectReason - cEjectInspectionOffset;
		BYTE TempNumberOfInspectionsThatFoundAContaminant = 0;

		if (!((TempEjectInspection < vGlobalCurrentProduct->vNumberOfInspections) && (vGlobalCurrentProduct->vInspection[TempEjectInspection])))
		{
			for (BYTE TempLoop = 0; TempLoop < vGlobalCurrentProduct->vNumberOfInspections; TempLoop++)
			if (vGlobalCurrentProduct->vInspection[TempLoop])
			if (TempImage->vRejectedForThisInspection[TempLoop])
			{
				TempNumberOfInspectionsThatFoundAContaminant++;
				if (TempEjectInspection >= vGlobalCurrentProduct->vNumberOfInspections)
					TempEjectInspection = TempLoop;
			}
		}
		if (vGlobalCurrentProduct->vMultiLaneNumberOfLanes)
		{
			TempString = TempString + "\nLane: " + dtoa(TempImage->vMultiLaneLaneNumber, 0);
			if (PasswordOK(cTemporaryInspxPassword, false))
			TempString = TempString + ",  Ejector Offset in Encoder Pulses: " + dtoa(TempImage->vMultiLaneEjectorOffset, 0);
		}

		if (TempImage->vContainerEjectReason >= cMaximumRejectReason)
			TempString = TempString + "\n\nGood Container";

		if ((TempImage->vContainerEjected) || (TempImage->vContainerEjectReason < cMaximumRejectReason))
		{
			TempString = TempString + "\n\n";
			TempString = TempString + vLocalSystemData->vCriteriaString[vLocalSystemData->vCountToUseForEjectReason[TempImage->vContainerEjectReason]];

			TempString = TempString + " ";


			//if (TempImage->vImproveLearn)
			//{
			//	TempText.LoadString(IDS_ImproveLearn);
			//	TempString = TempString + "+" + TempText;
			//}
			//else
			if (TempImage->vContainerEjected)
			{
				TempText.LoadString(IDS_ContainerEjected);
				TempString = TempString + TempText;
			}
			else
			{
				if (TempImage->vContainerEjectReason >= cMaximumRejectReason)
					TempString = TempString + "Good Container";
				else
				if ((TempImage->vContainerEjectReason == cPassSlippedContainer) ||
					(TempImage->vContainerEjectReason == cPassContainerWrongWidth) ||
					(TempImage->vContainerEjectReason == cPassContainerOnTop) ||
					(TempImage->vContainerEjectReason == cPassContainerWrongHeight) ||
					(TempImage->vContainerEjectReason == cPassContainerOnConveyorStop))
				{
					TempText.LoadString(IDS_Containerpassedmayhaveslippedonconveyor);//" Container passed without inspection (may have slipped on conveyor)"
					TempString = TempString + "*" + TempText;
				}
				else
				if (TempImage->vPassContainer)
					TempString = TempString + "*Container Not Inspected.  Passed";
				else
				//if (TempImage->vImproveLearn)
				//{
					//TempText.LoadString(IDS_CheckTo);
					//TempString = TempString + TempText;
				//}
				//else
				if (TempImage->vContainerEjectInspection < cMaximumRejectReason)
				if (vLocalSystemData->vInAutoSetup)
				{
					TempText.LoadString(IDS_InAutoSetupallcontainerspass);//" In AutoSetup, all containers pass"
					TempString = TempString + "*" + TempText;
				}
				else
				if (TempImage->vRejectDisabled)
				{
					TempText.LoadString(IDS_EjectionsDISABLEDcontainerpassed);//" Ejections DISABLED, container passed"
					TempString = TempString + "*" + TempText;
				}
				else
				//	TempString = TempString + " ???????";
				{
					TempText.LoadString(IDS_InAutoSetup);
					TempString = TempString + "*" + " " + TempText;
				}
			}
			
			if (TempImage->vContainerEjectInspection < cMaximumRejectReason)
			if (vGlobalCurrentProduct->vInspection[TempImage->vContainerEjectInspection])
			{	
				if ((vGlobalCurrentProduct->vInspection[TempImage->vContainerEjectInspection]->vInspectionType == cUnderfillByWeightInspection) || 
					(vGlobalCurrentProduct->vInspection[TempImage->vContainerEjectInspection]->vInspectionType == cOverfillByWeightInspection))
				if (vGlobalCurrentProduct->vInspection[TempImage->vContainerEjectInspection]->vCheckWeighUnits == "Ounces")
					TempAbreviation = "oz";
				else
					 TempAbreviation = "g";

				if (PasswordOK(cTemporaryInspxPassword, false))
				if (PasswordOK(cTemporaryInspxPassword, false))
				{
					int TempPosition = vGlobalCurrentProduct->vInspection[TempImage->vContainerEjectInspection]->vName.Find(vGlobalCurrentProduct->vInspection[TempImage->vContainerEjectInspection]->GetInspectionTypeString(), 0);
					if (TempPosition < 0)
						TempString = TempString + "\n    Inspection Type: " + vGlobalCurrentProduct->vInspection[TempImage->vContainerEjectInspection]->GetInspectionTypeString();
				}
			}

			if (TempImage->vContainerEjectInspection < 255)
			if ((TempImage->vContainerEjectReason != cPassSlippedContainer) &&
				(TempImage->vContainerEjectReason != cPassContainerWrongWidth) &&
				(TempImage->vContainerEjectReason != cPassContainerOnTop) &&
				(TempImage->vContainerEjectReason != cPassContainerWrongHeight) &&
				(TempImage->vContainerEjectReason != cPassContainerOnConveyorStop))
			{
				BYTE TempEjectInspection = TempImage->vContainerEjectReason - cEjectInspectionOffset;


				if ((TempEjectInspection < vGlobalCurrentProduct->vNumberOfInspections) && (vGlobalCurrentProduct->vInspection[TempEjectInspection]))
					TempString = TempString + "\n    Reject Threshold = " + dtoa(vGlobalCurrentProduct->vInspection[TempEjectInspection]->ConvertToWeight(TempImage->vContainerEjectThreshold), 2) + TempAbreviation;
				else
					TempString = TempString + "\n    Reject Threshold = " + dtoaWithCommas(TempImage->vContainerEjectThreshold,0);


				if ((TempImage->vContainerLearnState) && (TempEjectInspection < vGlobalCurrentProduct->vNumberOfInspections) &&
					(vGlobalCurrentProduct->vInspection[TempEjectInspection]) && (vGlobalCurrentProduct->vInspection[TempEjectInspection]->vAddStructure))
				{
					TempString = TempString + "\n+    Improve State = " + dtoa(TempImage->vContainerLearnState,0);
				}
			}
		}
		if ((TempImage->vContainerEjected) || (TempEjectInspection < cMaximumRejectReason))
		if (!vLocalConfigurationData->vStopAfterAnInspectionRejects)
		{
			TempString = TempString + "\n\nReject Inspections:";
			if (PasswordOK(cTemporaryInspxPassword,false))
				TempString = TempString + " (defective pixel count)";

			bool TempImageRejected = false;

			for (BYTE TempLoop = 0; TempLoop < vGlobalCurrentProduct->vNumberOfInspections; TempLoop++)
			if (TempImage->vRejectedForThisInspection[TempLoop])
			{
				if (vGlobalCurrentProduct->vInspection[TempLoop])
				{
					TempImageRejected = true;
					TempString = TempString + "\n    " + vGlobalCurrentProduct->vInspection[TempLoop]->vName;
				}
				else
					TempString = TempString + "\n    Inspection " + dtoa(TempLoop + 1, 0);

				if (PasswordOK(cTemporaryInspxPassword,false))
					if (TempImage->vNumberOfBlobs[TempLoop])
					TempString = TempString + " (" + dtoa(TempImage->vNumberOfBlobs[TempLoop], 0) + ")";
			}
		}
		//else
		//{
		//	TempText.LoadString(IDS_ContainerOK);//"\n\nContainer OK"
		//	TempString = TempString + "\n\n" + TempText;
		//}

		if (vGlobalCurrentProduct->vNumberOfInspectionsRequiredToEject > 1)
			TempString = TempString + "\n+    " + dtoa(vGlobalCurrentProduct->vNumberOfInspectionsRequiredToEject, 0) + " Inspections must find a contaminant to cause eject.";

		if (!TempImage->vDriftCompensationLocked)
			TempString = TempString + "\n*Adjusting Brightness";



		CString TempLineText = "";

		const int cTab0 = 5;
		const int cTab1 = 10;
		const int cTab2 = 30;
		const int cTab3 = 45;
		const int cTab4 = 55;
		const int cTab5 = 70;

		if ((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode)) //Continuous Feed, or a Pipeline
			TempLineText = "Image Information";
		else
		{
//Title Line
			//TempText.LoadString(IDS_ContainerDimensionsThisImageReferenceImage);//"\n\nContainer Dimensions   This Image\t Reference Image"
			TempLineText = "";

			FillToTab(&TempLineText, cTab1);
			TempLineText = TempLineText + "Container Dimensions in Inches (Pixels)";
			TempString = TempString + "\n\n" + TempLineText;

			TempLineText = "";
			FillToTab(&TempLineText, cTab2);
			TempLineText = TempLineText + "This Image";

			FillToTab(&TempLineText, cTab3);
			TempLineText = TempLineText + "Reference Image";
		}

		TempString = TempString + "\n" + TempLineText;

		if (!((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode))) //Not Continuous Feed, or a Pipeline
		{
//width
			TempLineText = "";
			if ((TempImage->vWidth > vGlobalCurrentProduct->vReferenceWidth + vLocalConfigurationData->vShapeHandlingWidthTolerancePixels) || 
				(TempImage->vWidth + vLocalConfigurationData->vShapeHandlingWidthTolerancePixels < vGlobalCurrentProduct->vReferenceWidth))
				TempLineText = "*";

			FillToTab(&TempLineText, cTab1);
			TempText.LoadString(IDS_Width);
			TempLineText = TempLineText + TempText;

			FillToTab(&TempLineText, cTab2);
			TempLineText = TempLineText + Pad(dtoa((double)TempImage->vWidth / (double)(vGlobalPixelsPerUnit * vGlobalCurrentProduct->vOverScanMultiplier),2), 2, 2) +
				" (" + Pad(dtoa(TempImage->vWidth ,0), 3, 0)  + ")";

			FillToTab(&TempLineText, cTab3);
			TempLineText = TempLineText + Pad(dtoa((double)vGlobalCurrentProduct->vReferenceWidth / (double)(vGlobalPixelsPerUnit * vGlobalCurrentProduct->vOverScanMultiplier),2), 2, 2) +
				" (" + Pad(dtoa(vGlobalCurrentProduct->vReferenceWidth,0), 3, 0) + ")";

			TempString = TempString + "\n" + TempLineText;

			//TempString = TempString + "\t" + TempText + "\t" + 
			//	Pad(dtoa((double)TempImage->vWidth / (double)(vGlobalPixelsPerUnit * vGlobalCurrentProduct->vOverScanMultiplier * TempScaleFactor),2) +
			//	" (" + dtoa(TempImage->vWidth,0) + ")",6,0) + "\t" +
			//	Pad(dtoa((double)vGlobalCurrentProduct->vReferenceWidth / (double)(vGlobalPixelsPerUnit * vGlobalCurrentProduct->vOverScanMultiplier * TempScaleFactor),2) +
			//	" (" + dtoa(vGlobalCurrentProduct->vReferenceWidth,0) + ")",4,0);

//top
			TempLineText = "";

			FillToTab(&TempLineText, cTab1);
			TempText = vGlobalTopString;
			TempLineText = TempLineText + TempText;

			FillToTab(&TempLineText, cTab2);
			TempLineText = TempLineText + Pad(dtoa((double)TempImage->vTop / (double)vGlobalPixelsPerUnitInHeight,2), 2, 2) +
				" (" + Pad(dtoa(TempImage->vTop,0),3, 0) + ")";

			FillToTab(&TempLineText, cTab3);
			TempLineText = TempLineText + Pad(dtoa((double)vGlobalCurrentProduct->vReferenceTop / (double)vGlobalPixelsPerUnitInHeight,2), 2, 2) +
				" (" + Pad(dtoa(vGlobalCurrentProduct->vReferenceTop,0),3, 0) + ")";

			TempString = TempString + "\n" + TempLineText;

			//TempText = vGlobalTopString;
			//TempString = TempString + "\n" + "\t" + TempText + " " + "\t" + Pad(dtoa((double)TempImage->vTop / (double)vGlobalPixelsPerUnitInHeight,2) +
			//	" (" + dtoa(TempImage->vTop,0) + ")",6,0) + "\t" + Pad(dtoa((double)vGlobalCurrentProduct->vReferenceTop / (double)vGlobalPixelsPerUnitInHeight,2) +
			//	" (" + dtoa(vGlobalCurrentProduct->vReferenceTop,0) + ")",4,0);

//bottom
			TempLineText = "";

			FillToTab(&TempLineText, cTab1);
			TempText = vGlobalBottomString;
			TempLineText = TempLineText + TempText;

			FillToTab(&TempLineText, cTab2);
				TempLineText = TempLineText + Pad(dtoa((double)TempImage->vBottom / (double)vGlobalPixelsPerUnitInHeight,2), 2, 2) +
					" (" + Pad(dtoa(TempImage->vBottom,0),3, 0) + ")";

			FillToTab(&TempLineText, cTab3);
				TempLineText = TempLineText + Pad(dtoa((double)vGlobalCurrentProduct->vReferenceBottom / (double)vGlobalPixelsPerUnitInHeight,2), 2, 2) +
					" (" + Pad(dtoa(vGlobalCurrentProduct->vReferenceBottom,0),3, 0) + ")";

			TempString = TempString + "\n" + TempLineText;

			//TempText = vGlobalBottomString;
			//if ((vLocalConfigurationData->vScanTracType == cGlissandoScanTrac) || (vGlobalScanTracType == cGlissandoPlusScanTrac))
			//	TempString = TempString + "\n" + "\t" + TempText + " " + "\t" + Pad(dtoa((double)TempImage->vBottom / (double)vGlobalPixelsPerUnitInHeight,2) +
			//		" (" + dtoa(TempImage->vBottom,0) + ")",6,0) + "\t" + Pad(dtoa((double)vGlobalCurrentProduct->vReferenceBottom / (double)vGlobalPixelsPerUnitInHeight,2) +
			//		" (" + dtoa(vGlobalCurrentProduct->vReferenceBottom,0) + ")",4,0);

//left
			TempLineText = "";
			if (TempImage->vLeft == 0)
				TempLineText = "*";

			FillToTab(&TempLineText, cTab1);
			TempText.LoadString(IDS_Left);
			TempLineText = TempLineText + TempText;

			FillToTab(&TempLineText, cTab2);
			TempLineText = TempLineText + Pad(dtoa((double)TempImage->vLeft / (double)(vGlobalPixelsPerUnit * vGlobalCurrentProduct->vOverScanMultiplier),2), 2, 2) +
				" (" + Pad(dtoa(TempImage->vLeft,0),3, 0) + ")";

			FillToTab(&TempLineText, cTab3);
			TempLineText = TempLineText + Pad(dtoa((double)vGlobalCurrentProduct->vReferenceLeft / (double)(vGlobalPixelsPerUnit * vGlobalCurrentProduct->vOverScanMultiplier),2), 2, 2) +
				" (" + Pad(dtoa(vGlobalCurrentProduct->vReferenceLeft,0),3, 0) + ")";

			TempString = TempString + "\n" + TempLineText;

			//TempText.LoadString(IDS_Left);
			//if (TempImage->vLeft == 0)
			//	TempString = TempString + "\n*";
			//else
			//	TempString = TempString + "\n";

			//TempString = TempString + "\t" + TempText + "\t" + 
			//	Pad(dtoa((double)TempImage->vLeft / 
			//	(double)(vGlobalPixelsPerUnit * vGlobalCurrentProduct->vOverScanMultiplier),2) +
			//	" (" + dtoa(TempImage->vLeft,0) + ")",6,0) + "\t" +
			//	Pad(dtoa((double)vGlobalCurrentProduct->vReferenceLeft / 
			//	(double)(vGlobalPixelsPerUnit * vGlobalCurrentProduct->vOverScanMultiplier),2) +
			//	" (" + dtoa(vGlobalCurrentProduct->vReferenceLeft,0) + ")",4,0);


//Right
			TempLineText = "";
			if (TempImage->vRight == vLocalSystemData->vITIPCDig->vOriginalBufferSizeY)
				TempLineText = "*";

			FillToTab(&TempLineText, cTab1);
			TempText.LoadString(IDS_Right);
			TempLineText = TempLineText + TempText;

			FillToTab(&TempLineText, cTab2);
			TempLineText = TempLineText + Pad(dtoa((double)TempImage->vRight / (double)(vGlobalPixelsPerUnit * vGlobalCurrentProduct->vOverScanMultiplier), 2), 2, 2) +
				" (" + Pad(dtoa(TempImage->vRight, 0),3, 0) + ")";

			FillToTab(&TempLineText, cTab3);
			TempLineText = TempLineText + Pad(dtoa((double)vGlobalCurrentProduct->vReferenceRight / (double)(vGlobalPixelsPerUnit * vGlobalCurrentProduct->vOverScanMultiplier), 2), 2, 2) +
				" (" + Pad(dtoa(vGlobalCurrentProduct->vReferenceRight, 0),3, 0) + ")";

			TempString = TempString + "\n" + TempLineText;

			//TempString = TempString + "\t" + TempText + "\t" + 
			//	Pad(dtoa((double)TempImage->vRight / 
			//	(double)(vGlobalPixelsPerUnit * vGlobalCurrentProduct->vOverScanMultiplier),2) +
			//	" (" + dtoa(TempImage->vRight,0) + ")",6,0) + "\t" +
			//	Pad(dtoa((double)vGlobalCurrentProduct->vReferenceRight / 
			//	(double)(vGlobalPixelsPerUnit * vGlobalCurrentProduct->vOverScanMultiplier),2) +
			//	" (" + dtoa(vGlobalCurrentProduct->vReferenceRight,0) + ")",4,0);
		}

		if ((vLocalConfigurationData->vScanTracType == cFermataScanTrac) || (vLocalConfigurationData->vScanTracType == cLegatoScanTrac)) 
		{//use algorithm to find brightness that can ignore bubbles by finding darkest value of each pixel
			if (TempImage->vFinalImageIntensity == 0)
			{
				TempImage->vFinalImageIntensity = vLocalSystemData->vITIPCDig->CalculatePipelineProductDensity(TempImage->vGreyImage, vGlobalCurrentProduct->vDriftDensityInspection);
				if ((TempImage->vFinalImageIntensity > 25) && (TempImage->vFinalImageIntensity < 230) && (vLocalSystemData->vSmallestDifferenceBetweenAverageAndMinimum < 40))
					TempImage->vFinalImageIntensity = TempImage->vFinalImageIntensity + vLocalSystemData->vSmallestDifferenceBetweenAverageAndMinimum;
			}
		}
		else
		if (TempImage->vFinalImageIntensity == 0)
		if (vGlobalCurrentProduct->vDriftDensityInspection)
		{
			TempImage->vFinalImageIntensity = vLocalSystemData->vITIPCDig->CalculateInspectionDensityAfterReference(TempImage->vGreyImage, vGlobalCurrentProduct->vDriftDensityInspection,
				TempImage->vLeftReferenceAdjust, TempImage->vTopReferenceAdjust, vLocalConfigurationData->vReferenceROIsToEdges, TempImage->vLeft, TempImage->vRight); 
		}
		else
		{
			TempImage->vFinalImageIntensity = vLocalSystemData->vITIPCDig->CalculateFullDensity(TempImage->vGreyImage, vLocalSystemData->vITIPCDig->vOriginalBufferSize); 
		}
	//Exposure
		TempLineText = "";

		FillToTab(&TempLineText, cTab1);
		TempText.LoadString(IDS_Exposure);
		TempLineText = TempLineText + TempText;

		FillToTab(&TempLineText, cTab2 - 1);
		TempLineText = TempLineText + Pad(dtoa(TempImage->vFinalImageIntensity, 2), 3, 0);

		FillToTab(&TempLineText, cTab3 - 1);
			TempLineText = TempLineText + Pad(dtoa(vGlobalCurrentProduct->vSetupHistoryAverageDensity, 2), 3, 0);

		TempString = TempString + "\n" + TempLineText;

		//TempText.LoadString(IDS_Exposure);
		//TempString = TempString + "\n" + "\t" + TempText + "\t" + Pad(dtoa(TempImage->vFinalImageBrightness,2),6,0) + "  \t" +
		//	Pad(dtoa(vGlobalCurrentProduct->vSetupHistoryAverageDensity,2),4,0);

		if (PasswordOK(cTemporaryInspxPassword,false))
		{
			if (TempImage->vOriginalImageIntensity)
			if (TempImage->vOriginalImageIntensity != TempImage->vFinalImageIntensity)
			{
//Original Density
				TempLineText = " ";

				FillToTab(&TempLineText, cTab1);
				TempText.LoadString(IDS_OrigDensity);
				TempLineText = TempLineText + TempText;

				FillToTab(&TempLineText, cTab2 - 1);
				TempLineText = TempLineText + Pad(dtoa(TempImage->vOriginalImageIntensity, 2), 3, 0);

				TempString = TempString + "\n" + TempLineText;

				//TempText.LoadString(IDS_OrigDensity);
				//TempString = TempString + "\n" + "\t" + TempText + Pad(dtoa(TempImage->vOriginalImageIntensity,2),4,0);
			}

			if ((vLocalConfigurationData->vShapeHandlingWidthTolerancePixels) && (vLocalConfigurationData->vShapeHandlingProcessIfWrongWidth))
			{
				TempText.LoadString(IDS_AcceptableWidthRange);
				TempString = TempString + "\n\n" + TempText + ": " + 
					dtoa(((double)vGlobalCurrentProduct->vReferenceWidth - (double)vLocalConfigurationData->vShapeHandlingWidthTolerancePixels) / 
					(double)(vGlobalPixelsPerUnit * vGlobalCurrentProduct->vOverScanMultiplier),2) + " (" +
					dtoa(((double)vGlobalCurrentProduct->vReferenceWidth - (double)vLocalConfigurationData->vShapeHandlingWidthTolerancePixels),0) + ") - " +
					dtoa(((double)vGlobalCurrentProduct->vReferenceWidth + (double)vLocalConfigurationData->vShapeHandlingWidthTolerancePixels) /
					(double)(vGlobalPixelsPerUnit * vGlobalCurrentProduct->vOverScanMultiplier),2) + " (" +
					dtoa(((double)vGlobalCurrentProduct->vReferenceWidth + (double)vLocalConfigurationData->vShapeHandlingWidthTolerancePixels),0) + ")";
			}
		}

		if (vLocalConfigurationData->vMaxImageIntensityAdjust)
		if (TempImage->vFinalImageIntensity > vGlobalCurrentProduct->vSetupHistoryAverageDensity + vLocalConfigurationData->vMaxImageIntensityAdjust)
		{
			TempString = TempString + "\n\n*Notice-This " + vLocalSystemData->vContainerString + " is Much Brighter than Learned\n";
		}
		else
		if (TempImage->vFinalImageIntensity + vLocalConfigurationData->vMaxImageIntensityAdjust < vGlobalCurrentProduct->vSetupHistoryAverageDensity)
		{
			TempString = TempString + "\n\n*Notice-This " + vLocalSystemData->vContainerString + " is Much Darker than Learned\n";
		}
		else
		if (TempImage->vImageIntensityTooFarOff)
			TempString = TempString + "\n\n*Notice-This " + vLocalSystemData->vContainerString + " is a bit Brighter or Darker than Learned\n";

		TempText1.LoadString(IDS_AcceptableWidthRange);
		if (vLocalConfigurationData->vShapeHandlingWidthTolerancePixels)
			TempString = TempString + "\n\n" + TempText1 + ": " + dtoa(((double)vGlobalCurrentProduct->vReferenceWidth - 
				(double)vLocalConfigurationData->vShapeHandlingWidthTolerancePixels) / (double)(vGlobalPixelsPerUnit * vGlobalCurrentProduct->vOverScanMultiplier),2) + " (" +
				dtoa(((double)vGlobalCurrentProduct->vReferenceWidth - (double)vLocalConfigurationData->vShapeHandlingWidthTolerancePixels),0) + ") - " +
				dtoa(((double)vGlobalCurrentProduct->vReferenceWidth + (double)vLocalConfigurationData->vShapeHandlingWidthTolerancePixels) / (double)(vGlobalPixelsPerUnit * vGlobalCurrentProduct->vOverScanMultiplier),2) + 
				" (" + dtoa(((double)vGlobalCurrentProduct->vReferenceWidth + (double)vLocalConfigurationData->vShapeHandlingWidthTolerancePixels),0) + ")";

		//TempString = TempString + "\n" + "\t" + "Bottom" + "\t" + 
		//	Pad(dtoa((double)TempImage->vBottom / (double)vGlobalPixelsPerUnitInHeight,2) +
		//	" (" + dtoa(TempImage->vBottom,0) + ")",4,0) + "\t" +
		//	Pad(dtoa((double)vGlobalCurrentProduct->vReferenceBottom / 
		//	(double)vGlobalPixelsPerUnitInHeight,2) +
		//	" (" + dtoa(vGlobalCurrentProduct->vReferenceBottom,0) + ")",2,0);

		CString TempDividerString("\n\n===========================================================");
		TempString = TempString + TempDividerString;
		if (TempImage->vContainerEjectInspection < cMaximumRejectReason)
		{
			TempString = TempString + "\n\t\t\tAS EJECTED";
		}

		//information on current inspection for this image
		if ((TempEjectInspection < vGlobalCurrentProduct->vNumberOfInspections) && (vGlobalCurrentProduct->vInspection[TempEjectInspection]) && (TempImage))
		{
			TempText.LoadString(IDS_RejectInspection);
			TempString = TempString + "\n\n" + TempText + ": " + vGlobalCurrentProduct->vInspection[TempEjectInspection]->vName;

			if (PasswordOK(cTemporaryInspxPassword, false))
			if (PasswordOK(cTemporaryInspxPassword, false))
			{
				int TempPosition = vGlobalCurrentProduct->vInspection[TempEjectInspection]->vName.Find(vGlobalCurrentProduct->vInspection[TempEjectInspection]->GetInspectionTypeString(), 0);
				if (TempPosition < 0)
					TempString = TempString + "\n    Inspection Type: " + vGlobalCurrentProduct->vInspection[TempEjectInspection]->GetInspectionTypeString();
			}

			if (vGlobalCurrentProduct->vInspection[TempEjectInspection]->vInspectionType == cFloodedInspection)
			{
					TempString = TempString + "\nSpout Length when inspected: " + dtoa(TempImage->vNumberOfBlobs[TempEjectInspection], 0);
					TempString = TempString + "\nSpout Length required when Ejected: " + dtoa(TempImage->vContainerEjectThreshold, 0);
					TempString = TempString + "\nThreshold to find Spout Length when Ejected: " + dtoa(TempImage->vContainerLearnState, 0);
			}
			else
			if (vGlobalCurrentProduct->vInspection[TempEjectInspection]->vInspectionType == cBadHeartInspection)
			{
					TempString = TempString + "\nPerimeter when inspected: " + dtoa(TempImage->vLargestBlobPerimeter[TempEjectInspection], 0);
					TempString = TempString + "\nArea when inspected: " + dtoa(TempImage->vContainerLargestContaminant, 0);
					TempString = TempString + "\nRatio when Ejected: " + dtoa((double)TempImage->vContainerLargestContaminant / (double)TempImage->vLargestBlobPerimeter[TempEjectInspection], 2);
			}
			else
			if (vGlobalCurrentProduct->vInspection[TempEjectInspection]->vInspectionType == cStandardDeviationInspection)
			{ //StandardDeviation inspection
				double TempStandardDeviation = 0;

				if (vGlobalCurrentProduct->vInspection[TempEjectInspection]->vPreSmoothImage)
				{
					CopyMemory(vLocalSystemData->vITIPCDig->vNIDDScratchImage->pix, TempImage->vGreyImage, vLocalSystemData->vITIPCDig->vOriginalBufferSize);
					DoMedianSmooth(vLocalSystemData->vITIPCDig->vNIDDScratchImage, vGlobalCurrentProduct->vInspection[TempEjectInspection]->vPreSmoothImage, vGlobalCurrentProduct->vInspection[TempEjectInspection]->vPreSmoothImage);
					TempStandardDeviation = vLocalSystemData->vITIPCDig-> CalculateInspectionStandardDeviationAfterReference(vLocalSystemData->vITIPCDig->vNIDDScratchImage->pix, 
						vGlobalCurrentProduct->vInspection[TempEjectInspection], TempImage->vLeftReferenceAdjust);
				}
				else
					TempStandardDeviation = vLocalSystemData->vITIPCDig->CalculateInspectionStandardDeviationAfterReference(TempImage->vGreyImage, 
						vGlobalCurrentProduct->vInspection[TempEjectInspection], TempImage->vLeftReferenceAdjust);

				TempText.LoadString(IDS_ThiscontainersTextureValue);
				TempString = TempString + "\n    " + TempText + ": " + dtoa(TempStandardDeviation,2);

				TempText.LoadString(IDS_AcceptableTextureValue);
				TempString = TempString + "\n    " + TempText + ": " + dtoa(vGlobalCurrentProduct->vInspection[TempEjectInspection]->vDensityThresholdLower,2) + "-" +
					dtoa(vGlobalCurrentProduct->vInspection[TempEjectInspection]->vDensityThresholdUpper,2);
			}
			else
			if ((vGlobalCurrentProduct->vInspection[TempEjectInspection]->vInspectionType == cUnderfillByWeightInspection) || (vGlobalCurrentProduct->vInspection[TempEjectInspection]->vInspectionType == cOverfillByWeightInspection))
			{
				TempString = TempString + "\n    Weight when Inspected: " + dtoaWithCommas(vGlobalCurrentProduct->vInspection[TempEjectInspection]->ConvertToWeight(TempImage->vDensityValue[TempEjectInspection]), 2);

				if (TempImage->vContainerWeightCorrection[TempEjectInspection])
					TempString = TempString + "\n    Weight Offset when Inspected: " + dtoa(TempImage->vContainerWeightCorrection[TempEjectInspection], 2) + "\n";

				if (vInspectionNumberShowing == TempEjectInspection)
				{
					TempString = TempString + "\n    Threshold when Inspected: " + dtoaWithCommas(vGlobalCurrentProduct->vInspection[TempEjectInspection]->ConvertToWeight(TempImage->vContainerEjectThreshold), 2);
				}
			}
			else
			if ((vGlobalCurrentProduct->vInspection[TempEjectInspection]->vInspectionType == cDensityInspection) ||
				(vGlobalCurrentProduct->vInspection[TempEjectInspection]->vInspectionType == cCheckWeighInspection) ||
				(vGlobalCurrentProduct->vInspection[TempEjectInspection]->vInspectionType == cStandardDeviationInspection) ||
				(vGlobalCurrentProduct->vInspection[TempEjectInspection]->vInspectionType == cMissingLidInspection) ||
				(vGlobalCurrentProduct->vInspection[TempEjectInspection]->vInspectionType == cUnderfillInspection) ||
				(vGlobalCurrentProduct->vInspection[TempEjectInspection]->vInspectionType == cOverfillInspection))
			{ //density inspection
				double TempDensity = TempImage->vDensityValue[TempEjectInspection];

				if (TempDensity == 0)
				if (vGlobalCurrentProduct->vInspection[TempEjectInspection]->vInspectionType != cStandardDeviationInspection)
					TempDensity = vLocalSystemData->vITIPCDig->CalculateInspectionDensityAfterReference(
						TempImage->vGreyImage,
						vGlobalCurrentProduct->vInspection[TempEjectInspection],
						TempImage->vLeftReferenceAdjust,
						TempImage->vTopReferenceAdjust,
						vLocalConfigurationData->vReferenceROIsToEdges,
						TempImage->vLeft,
						TempImage->vRight); 
				else
				{
					if (vGlobalCurrentProduct->vInspection[TempEjectInspection]->vPreSmoothImage)
					{
						CopyMemory(vLocalSystemData->vITIPCDig->vNIDDScratchImage->pix, TempImage->vGreyImage, vLocalSystemData->vITIPCDig->vOriginalBufferSize);
						DoMedianSmooth(vLocalSystemData->vITIPCDig->vNIDDScratchImage, vGlobalCurrentProduct->vInspection[TempEjectInspection]->vPreSmoothImage, vGlobalCurrentProduct->vInspection[TempEjectInspection]->vPreSmoothImage);
						TempDensity = vLocalSystemData->vITIPCDig->CalculateInspectionStandardDeviationAfterReference(vLocalSystemData->vITIPCDig->vNIDDScratchImage->pix,
							vGlobalCurrentProduct->vInspection[TempEjectInspection], TempImage->vLeftReferenceAdjust); 
					}
					else
						TempDensity = vLocalSystemData->vITIPCDig->CalculateInspectionStandardDeviationAfterReference(TempImage->vGreyImage,
							vGlobalCurrentProduct->vInspection[TempEjectInspection], TempImage->vLeftReferenceAdjust); 
				}

				TempDensity = vGlobalCurrentProduct->vInspection[TempEjectInspection]->ConvertToWeight(TempDensity);

				if (((vGlobalCurrentProduct->vInspection[TempEjectInspection]->vCheckWeighBFactor != 0) || (vGlobalCurrentProduct->vInspection[TempEjectInspection]->vCheckWeighMFactor != 1)) && 
					(vGlobalCurrentProduct->vInspection[TempEjectInspection]->vCheckWeighMFactor))
					TempText.LoadString(IDS_ThisContainersWeight);
				else
					TempText.LoadString(IDS_ThiscontainersDensity);

				TempString = TempString + "\n    " + TempText + ": " + dtoa(TempDensity,2) + TempAbreviation;

				if (vGlobalCurrentProduct->vInspection[TempEjectInspection]->vInspectionType == cDensityInspection)
				{
					if (((vGlobalCurrentProduct->vInspection[TempEjectInspection]->vCheckWeighBFactor != 0) || (vGlobalCurrentProduct->vInspection[TempEjectInspection]->vCheckWeighMFactor != 1)) && 
						(vGlobalCurrentProduct->vInspection[TempEjectInspection]->vCheckWeighMFactor))
						TempText.LoadString(IDS_AcceptableWeight);
					else
						TempText.LoadString(IDS_AcceptableDensity);

					if (vGlobalCurrentProduct->vInspection[TempEjectInspection]->vHasDensityLowerThreshold)
						TempString = TempString + "\n    " + TempText + ": " + 
							dtoa(vGlobalCurrentProduct->vInspection[TempEjectInspection]->ConvertToWeight(vGlobalCurrentProduct->vInspection[TempEjectInspection]->vDensityThresholdUpper),2) + TempAbreviation + "-" +
							dtoa(vGlobalCurrentProduct->vInspection[TempEjectInspection]->ConvertToWeight(vGlobalCurrentProduct->vInspection[TempEjectInspection]->vDensityThresholdLower),2) + TempAbreviation;
					else
						TempString = TempString + "\n    " + TempText + ": 0-" +
							dtoa(vGlobalCurrentProduct->vInspection[TempEjectInspection]->ConvertToWeight(vGlobalCurrentProduct->vInspection[TempEjectInspection]->vDensityThresholdUpper),2) + TempAbreviation;
				}
				else
				if ((vGlobalCurrentProduct->vInspection[TempEjectInspection]->vInspectionType == cMissingLidInspection) ||
					(vGlobalCurrentProduct->vInspection[TempEjectInspection]->vInspectionType == cUnderfillInspection))
				{
					TempString = TempString + "\n    Found at Threshold: " + dtoa(TempImage->vContainerEjectThreshold, 2);

					TempText.LoadString(IDS_AcceptableDensity);
					TempString = TempString + "\n    " + TempText + ": " + 
						dtoa(vGlobalCurrentProduct->vInspection[TempEjectInspection]->vDensityThresholdLower,2) + "-" + dtoa(TempImage->vContainerEjectThreshold,2);
				}
				else
				if (vGlobalCurrentProduct->vInspection[TempEjectInspection]->vInspectionType == cOverfillInspection)
				{
					TempString = TempString + "\n    Found at Threshold: " + dtoa(TempImage->vContainerEjectThreshold, 2);

					TempString = TempString + "\n    Acceptable Density: " + 
						dtoa(vInspectionShowing->ConvertToWeight(TempImage->vContainerEjectThreshold),2) + " - " + 
						dtoa(vInspectionShowing->ConvertToWeight(vInspectionShowing->vDensityThresholdUpper),2);
				}
			}
			else
			if ((vGlobalCurrentProduct->vInspection[TempEjectInspection]->vInspectionType == cItemCountInspection) || 
				(vGlobalCurrentProduct->vInspection[TempEjectInspection]->vInspectionType == cFloodedInspection) || 
				(vGlobalCurrentProduct->vInspection[TempEjectInspection]->vInspectionType == cSizeInspection) || 
				(vGlobalCurrentProduct->vInspection[TempEjectInspection]->vInspectionType == cMissingLidInspectionOnSize)) //as ejected
			{ //size inspection when rejected
				if (vGlobalCurrentProduct->vInspection[TempEjectInspection]->vInspectionType == cFloodedInspection)
					TempText = "Spout Length when inspected";
				else
				if (vGlobalCurrentProduct->vInspection[TempEjectInspection]->vInspectionType == cMissingLidInspectionOnSize)
					TempText = "Size of Lid when inspected";
				else
				if (vGlobalCurrentProduct->vInspection[TempEjectInspection]->vInspectionType == cItemCountInspection)
					TempText = "Number of Items found meeting requirements when inspected";
				else
					TempText = "Size of Item when inspected";

				TempString = TempString + "\n    " + TempText + ": " + dtoa(TempImage->vNumberOfBlobs[TempEjectInspection],0);

				if (vGlobalCurrentProduct->vInspection[TempEjectInspection]->vInspectionType == cFloodedInspection)
					TempText = "Flooded IV Bag Inspection Settings";
				else
				if (vGlobalCurrentProduct->vInspection[TempEjectInspection]->vInspectionType == cMissingLidInspectionOnSize)
					TempText = "Missing Lid Inspection Settings";
				else
				if (vGlobalCurrentProduct->vInspection[TempEjectInspection]->vInspectionType == cItemCountInspection)
					TempText = "Item Count Inspection Settings";
				else
					TempText = "Size of Item Inspection Settings";

				if (vGlobalCurrentProduct->vInspection[TempEjectInspection]->vInspectionType == cMissingLidInspectionOnSize)
					TempString = TempString + "\n    Pixels must be darker than " + dtoa(TempImage->vContainerEjectThreshold,0) + " to be part of Lid";
				else
				if (vGlobalCurrentProduct->vInspection[TempEjectInspection]->vInspectionType == cItemCountInspection)
					TempString = TempString + "\n    Pixels must be darker than " + dtoa(TempImage->vContainerEjectThreshold,0) + " to be part of an item";
				else
					TempString = TempString + "\n    Pixels must be darker than " + dtoa(TempImage->vContainerEjectThreshold,0) + " to be part of item";

				//if (vGlobalCurrentProduct->vInspection[TempEjectInspection]->vInspectionType == cItemCountInspection)
				//{
				//	if ((vGlobalCurrentProduct->vInspection[TempEjectInspection]->vMinimumBrightnessAceptable) && (vGlobalCurrentProduct->vInspection[TempEjectInspection]->vMinimumBrightnessAceptable < 255))
				//		TempString = TempString + "\n        Pixels must be brighter than Threshold to be part of an item: " + dtoa(vGlobalCurrentProduct->vInspection[TempEjectInspection]->vMinimumBrightnessAceptable,0);
				//	else
				//		TempString = TempString + "\n        Pixels must be brighter than Threshold to be part of an item disabled";
				//}

				//if (vGlobalCurrentProduct->vInspection[TempEjectInspection]->vInspectionType == cMissingLidInspectionOnSize)
				//	TempString = TempString + "\n        " + "Minimum number of Pixels in Lid to be complete: " + dtoa(vGlobalCurrentProduct->vInspection[TempEjectInspection]->vSize,0);
				//else
				//	TempString = TempString + "\n        " + "Minimum number of Pixels in each item to be complete: " + dtoa(vGlobalCurrentProduct->vInspection[TempEjectInspection]->vSize,0);

				//if (vGlobalCurrentProduct->vInspection[TempEjectInspection]->vInspectionType == cItemCountInspection)
				//{
				//	if (vGlobalCurrentProduct->vInspection[TempEjectInspection]->vMaximumSize)
				//		TempString = TempString + "\n        " + "Good items have " + dtoa(vGlobalCurrentProduct->vInspection[TempEjectInspection]->vMaximumSize,0) + " or fewer pixels";
				//	else
				//		TempString = TempString + "\n        " + "Maximum number of Pixels allowed in each item is disabled";

				//	if (vGlobalCurrentProduct->vInspection[TempEjectInspection]->vMinimumNumberOfItemsRequired)
				//		TempString = TempString + "\n        " + "Must have " + dtoa(vGlobalCurrentProduct->vInspection[TempEjectInspection]->vMinimumNumberOfItemsRequired,0) + " or more items for package to be good";
				//	else
				//		TempString = TempString + "\n        " + "Minimum number of items required in good package disabled";

				//	if (vGlobalCurrentProduct->vInspection[TempEjectInspection]->vMaximumNumberOfItemsRequired)
				//		TempString = TempString + "\n        " + "Must have " + dtoa(vGlobalCurrentProduct->vInspection[TempEjectInspection]->vMaximumNumberOfItemsRequired,0) + " or fewer items for package to be good";
				//	else
				//		TempString = TempString + "\n        " + "Maximum number of items permitted in good package disabled";
				//}
			}
			else
			if (vGlobalCurrentProduct->vInspection[TempEjectInspection]->vInspectionType == cOverfillByWeightInspection)
			{ 
				double TempCalculatedUnderfillByWeight = 0;
				if ((vInspectionShowing->vWhiteOutAnythingTouchingTheTop) && //remove the IV Bag if need to
					(vLocalSystemData->vITIPCDig->vNIDDScratchImage) && (vLocalSystemData->vITIPCDig->vNIDDScratchImage->pix))
				{
					CopyMemory(vLocalSystemData->vITIPCDig->vNIDDScratchImage->pix, TempImage->vGreyImage, vLocalSystemData->vITIPCDig->vOriginalBufferSize);
					vInspectionShowing->WhiteOutAnythingTouchingTheTop(vLocalSystemData->vITIPCDig->vNIDDScratchImage->pix, vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY);

					TempCalculatedUnderfillByWeight = vLocalSystemData->vITIPCDig->CalculateUnderfillByWeight(vLocalSystemData->vITIPCDig->vNIDDScratchImage->pix, vInspectionShowing, TempImage->vLeftReferenceAdjust); 
				}
				else
					TempCalculatedUnderfillByWeight = vLocalSystemData->vITIPCDig->CalculateUnderfillByWeight(TempImage->vGreyImage, vInspectionShowing, TempImage->vLeftReferenceAdjust); 

				if (vGlobalCurrentProduct->vInspection[TempEjectInspection]->vCheckWeighOffset)
				{
					double TempWeight = vGlobalCurrentProduct->vInspection[TempEjectInspection]->ConvertToWeight(TempCalculatedUnderfillByWeight) + vGlobalCurrentProduct->vInspection[TempEjectInspection]->vCheckWeighOffset;
					TempCalculatedUnderfillByWeight = 	vGlobalCurrentProduct->vInspection[TempEjectInspection]->ConvertWeightToBrightness(TempWeight);
				}

				if (TempCalculatedUnderfillByWeight > vThreshold)
					TempString = TempString + "\nWith current settings this container would be REJECTED";
				else
					TempString = TempString + "\nWith current settings this container would be ACCEPTED";

				TempString = TempString + "\n    Weight when Inspected: " + dtoaWithCommas(vGlobalCurrentProduct->vInspection[TempEjectInspection]->ConvertToWeight(TempImage->vDensityValue[TempEjectInspection]), 2);

				if (TempImage->vContainerWeightCorrection[TempEjectInspection])
					TempString = TempString + "\n    Weight Offset when Inspected: " + dtoa(TempImage->vContainerWeightCorrection[TempEjectInspection], 2) + "\n";

				if (vInspectionNumberShowing == TempEjectInspection)
					TempString = TempString + "\n    Threshold when Inspected: " + dtoaWithCommas(vGlobalCurrentProduct->vInspection[TempEjectInspection]->ConvertToWeight(TempImage->vContainerEjectThreshold), 2);

				TempString = TempString + "\n    Image Weight Now: " + dtoaWithCommas(vGlobalCurrentProduct->vInspection[TempEjectInspection]->ConvertToWeight(TempCalculatedUnderfillByWeight), 2);
				if (vGlobalCurrentProduct->vInspection[TempEjectInspection]->vCheckWeighOffset)
					TempString = TempString + "\n    Current Weight Offset: " + dtoa(vGlobalCurrentProduct->vInspection[TempEjectInspection]->vCheckWeighOffset, 2) + "\n";

				TempString = TempString + "\n    Maximum Acceptable Weight (Threshold): " + dtoaWithCommas(vGlobalCurrentProduct->vInspection[TempEjectInspection]->ConvertToWeight(vThreshold),2);
			}
			else
			if (vGlobalCurrentProduct->vInspection[TempEjectInspection]->vInspectionType == cUnderfillByWeightInspection)
			{ 
				double TempCalculatedUnderfillByWeight = 0;
				if ((vInspectionShowing->vWhiteOutAnythingTouchingTheTop) && //remove the IV Bag if need to
					(vLocalSystemData->vITIPCDig->vNIDDScratchImage) && (vLocalSystemData->vITIPCDig->vNIDDScratchImage->pix))
				{
					CopyMemory(vLocalSystemData->vITIPCDig->vNIDDScratchImage->pix, TempImage->vGreyImage, vLocalSystemData->vITIPCDig->vOriginalBufferSize);
					vInspectionShowing->WhiteOutAnythingTouchingTheTop(vLocalSystemData->vITIPCDig->vNIDDScratchImage->pix, vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY);

					TempCalculatedUnderfillByWeight = vLocalSystemData->vITIPCDig->CalculateUnderfillByWeight(vLocalSystemData->vITIPCDig->vNIDDScratchImage->pix, vInspectionShowing, TempImage->vLeftReferenceAdjust); 
				}
				else
					TempCalculatedUnderfillByWeight = vLocalSystemData->vITIPCDig->CalculateUnderfillByWeight(TempImage->vGreyImage, vInspectionShowing, TempImage->vLeftReferenceAdjust); 

				if (vGlobalCurrentProduct->vInspection[TempEjectInspection]->vCheckWeighOffset)
				{
					double TempWeight = vGlobalCurrentProduct->vInspection[TempEjectInspection]->ConvertToWeight(TempCalculatedUnderfillByWeight) + vGlobalCurrentProduct->vInspection[TempEjectInspection]->vCheckWeighOffset;
					TempCalculatedUnderfillByWeight = 	vGlobalCurrentProduct->vInspection[TempEjectInspection]->ConvertWeightToBrightness(TempWeight);
				}

				if (TempCalculatedUnderfillByWeight < vThreshold)
					TempString = TempString + "\nWith current settings this container would be REJECTED";
				else
					TempString = TempString + "\nWith current settings this container would be ACCEPTED";

				TempString = TempString + "\n    Weight when Inspected: " + dtoaWithCommas(vGlobalCurrentProduct->vInspection[TempEjectInspection]->ConvertToWeight(TempImage->vDensityValue[TempEjectInspection]), 2);

				if (TempImage->vContainerWeightCorrection[TempEjectInspection])
					TempString = TempString + "\n    Weight Offset when Inspected: " + dtoa(TempImage->vContainerWeightCorrection[TempEjectInspection], 2) + "\n";

				if (vInspectionNumberShowing == TempEjectInspection)
					TempString = TempString + "\n    Threshold when Inspected: " + dtoaWithCommas(vGlobalCurrentProduct->vInspection[TempEjectInspection]->ConvertToWeight(TempImage->vContainerEjectThreshold), 2);

				TempString = TempString + "\n    Image Weight Now: " + dtoaWithCommas(vGlobalCurrentProduct->vInspection[TempEjectInspection]->ConvertToWeight(TempCalculatedUnderfillByWeight), 2);
				if (vGlobalCurrentProduct->vInspection[TempEjectInspection]->vCheckWeighOffset)
					TempString = TempString + "\n    Current Weight Offset: " + dtoa(vGlobalCurrentProduct->vInspection[TempEjectInspection]->vCheckWeighOffset, 2) + "\n";

				TempString = TempString + "\n    Minimum Acceptable Weight (Threshold): " + dtoaWithCommas(vGlobalCurrentProduct->vInspection[TempEjectInspection]->ConvertToWeight(vThreshold),2);
			}
			else
			if (vGlobalCurrentProduct->vInspection[TempEjectInspection]->vInspectionType == cCheckWeighInspection)
			{ 
				double TempCalculatedUnderfillByWeight = 0;
				if ((vInspectionShowing->vWhiteOutAnythingTouchingTheTop) && //remove the IV Bag if need to
					(vLocalSystemData->vITIPCDig->vNIDDScratchImage) && (vLocalSystemData->vITIPCDig->vNIDDScratchImage->pix))
				{
					CopyMemory(vLocalSystemData->vITIPCDig->vNIDDScratchImage->pix, TempImage->vGreyImage, vLocalSystemData->vITIPCDig->vOriginalBufferSize);
					vInspectionShowing->WhiteOutAnythingTouchingTheTop(vLocalSystemData->vITIPCDig->vNIDDScratchImage->pix, vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY);

					TempCalculatedUnderfillByWeight = vLocalSystemData->vITIPCDig->CalculateUnderfillByWeight(vLocalSystemData->vITIPCDig->vNIDDScratchImage->pix, vInspectionShowing, TempImage->vLeftReferenceAdjust); 
				}
				else
					TempCalculatedUnderfillByWeight = vLocalSystemData->vITIPCDig->CalculateUnderfillByWeight(TempImage->vGreyImage, vInspectionShowing, TempImage->vLeftReferenceAdjust); 

				TempString = TempString + "\n    Weight when Inspected: " + dtoaWithCommas(vGlobalCurrentProduct->vInspection[TempEjectInspection]->ConvertToWeight(TempImage->vDensityValue[TempEjectInspection]), 2);

				if (TempImage->vContainerWeightCorrection[TempEjectInspection])
					TempString = TempString + "\n    Weight Offset when Inspected: " + dtoa(TempImage->vContainerWeightCorrection[TempEjectInspection], 2) + "\n";

				if (vGlobalCurrentProduct->vInspection[TempEjectInspection]->vCheckWeighOffset)
				{
					double TempWeight = vGlobalCurrentProduct->vInspection[TempEjectInspection]->ConvertToWeight(TempCalculatedUnderfillByWeight) + vGlobalCurrentProduct->vInspection[TempEjectInspection]->vCheckWeighOffset;
					TempCalculatedUnderfillByWeight = vGlobalCurrentProduct->vInspection[TempEjectInspection]->ConvertWeightToBrightness(TempWeight);
				}
				TempString = TempString + "\n    Image Weight Now: " + dtoaWithCommas(vGlobalCurrentProduct->vInspection[TempEjectInspection]->ConvertToWeight(TempCalculatedUnderfillByWeight), 2);
				if (vGlobalCurrentProduct->vInspection[TempEjectInspection]->vCheckWeighOffset)
					TempString = TempString + "\n    Current Weight Offset: " + dtoa(vGlobalCurrentProduct->vInspection[TempEjectInspection]->vCheckWeighOffset, 2) + "\n";

				if (((vGlobalCurrentProduct->vInspection[TempEjectInspection]->vCheckWeighBFactor != 0) || (vGlobalCurrentProduct->vInspection[TempEjectInspection]->vCheckWeighMFactor != 1)) && 
					(vGlobalCurrentProduct->vInspection[TempEjectInspection]->vCheckWeighMFactor))
					TempText.LoadString(IDS_AcceptableWeight);
				else
					TempText.LoadString(IDS_AcceptableDensity);

				if (vGlobalCurrentProduct->vInspection[TempEjectInspection]->vHasDensityLowerThreshold)
					TempString = TempString + "\n    " + TempText + ": " + 
						dtoa(vGlobalCurrentProduct->vInspection[TempEjectInspection]->ConvertToWeight(vGlobalCurrentProduct->vInspection[TempEjectInspection]->vDensityThresholdUpper),2) + TempAbreviation + "-" +
						dtoa(vGlobalCurrentProduct->vInspection[TempEjectInspection]->ConvertToWeight(vGlobalCurrentProduct->vInspection[TempEjectInspection]->vDensityThresholdLower),2) + TempAbreviation;
				else
					TempString = TempString + "\n    " + TempText + ": 0-" +
						dtoa(vGlobalCurrentProduct->vInspection[TempEjectInspection]->ConvertToWeight(vGlobalCurrentProduct->vInspection[TempEjectInspection]->vDensityThresholdUpper),2) + TempAbreviation;
			}
			else
			{ //contaminant or void inspection
				TempString = TempString + "\n    " + "Largest Blob Size" + ": " + dtoa(TempImage->vContainerLargestContaminant,0);

				if (vInspectionNumberShowing == TempEjectInspection)
				if (TempImage->vLargestBlobPerimeter[TempEjectInspection])
				if (vGlobalCurrentProduct->vInspection[TempEjectInspection]->vInspectionType == cBadHeartInspection)
					TempString = TempString + "\n    Largest Perimeter when Inspected: " + dtoa(TempImage->vLargestBlobPerimeter[TempEjectInspection], 0);

				TempText.LoadString(IDS_Thiscontainersnumberofdefects);
				TempString = TempString + "\n    " + TempText + ": " + dtoa(TempImage->vNumberOfBlobs[TempEjectInspection],0);
				
				TempString = TempString + "\n    Found at Threshold: " + dtoa(TempImage->vContainerEjectThreshold,0);

				if ((TempImage->vContainerLearnState) && (TempEjectInspection < vGlobalCurrentProduct->vNumberOfInspections) &&
					(vGlobalCurrentProduct->vInspection[TempEjectInspection]) && (vGlobalCurrentProduct->vInspection[TempEjectInspection]->vAddStructure))
				{
					TempString = TempString + "\n+    Improve State = " + dtoa(TempImage->vContainerLearnState,0);
				}

				TempText.LoadString(IDS_InspectionThreshold);
				TempText1.LoadString(IDS_MinimumSize);
				
				if (vGlobalCurrentProduct->vInspection[TempEjectInspection]->vInspectionType == cFloodedInspection)
					TempText1 = "Minimum Spout Length";

				TempString = TempString + "\n    " + TempText + ": " +dtoa(vGlobalCurrentProduct->vInspection[TempEjectInspection]->vThreshold,0) + 
					", " + TempText1 + ": " + dtoa(vGlobalCurrentProduct->vInspection[TempEjectInspection]->vSize,0);

				if (vGlobalCurrentProduct->vInspection[TempEjectInspection]->vInspectionType == cDripInspection)
				{
					if (vGlobalCurrentProduct->vInspection[TempEjectInspection]->vMaximumSize)
						TempString = TempString + ", Maximum Size: " + dtoa(vGlobalCurrentProduct->vInspection[TempEjectInspection]->vMaximumSize,0);
					if (vGlobalCurrentProduct->vInspection[TempEjectInspection]->vMinimumWidth)
						TempString = TempString + ", Minimum Width: " + dtoa(vGlobalCurrentProduct->vInspection[TempEjectInspection]->vMinimumWidth,0);
					if (vGlobalCurrentProduct->vInspection[TempEjectInspection]->vMaximumHeightAboveBottom)
						TempString = TempString + ", Maximum Height Above Bottom: " + dtoa(vGlobalCurrentProduct->vInspection[TempEjectInspection]->vMaximumHeightAboveBottom,0);
				}
			}
			if (vGlobalCurrentProduct->vInspection[TempEjectInspection]->vReferenceToTop)
				TempString = TempString + "\n++  Reference To Top"; 

			if (vGlobalCurrentProduct->vInspection[TempEjectInspection]->vReferenceToBottom)
				TempString = TempString + "\n++  Reference To Bottom"; 
		}
		else //wrong width or height
		if ((TempImage->vContainerEjectReason == cEjectContainerWrongWidth) || (TempImage->vContainerEjectReason == cPassContainerWrongWidth))
		{
			TempText.LoadString(IDS_WrongWidth);
			TempText1.LoadString(IDS_AcceptableWidthRange);
			TempString = TempString + "\n\n" + TempText;
			if (vLocalConfigurationData->vShapeHandlingWidthTolerancePixels)
				TempString = TempString + "\n\n" + TempText1 + ": " + dtoa(((double)vGlobalCurrentProduct->vReferenceWidth - 
					(double)vLocalConfigurationData->vShapeHandlingWidthTolerancePixels) / 
					(double)(vGlobalPixelsPerUnit * vGlobalCurrentProduct->vOverScanMultiplier),2) + " (" +
					dtoa(((double)vGlobalCurrentProduct->vReferenceWidth - 
					(double)vLocalConfigurationData->vShapeHandlingWidthTolerancePixels),0) + ") - " +
					dtoa(((double)vGlobalCurrentProduct->vReferenceWidth + 
					(double)vLocalConfigurationData->vShapeHandlingWidthTolerancePixels) /
					(double)(vGlobalPixelsPerUnit * vGlobalCurrentProduct->vOverScanMultiplier),2) + " (" +
					dtoa(((double)vGlobalCurrentProduct->vReferenceWidth + 
					(double)vLocalConfigurationData->vShapeHandlingWidthTolerancePixels),0) + ")";
		}
		else //wrong width or height
		if (TempImage->vContainerEjectReason == cTestEject)
		{
			TempText.LoadString(IDS_TestEjectedContainer);
			TempString = TempString + "\n\n" + TempText;
		}
		if ((TempImage->vContainerEjectReason == cEjectContainerWrongHeight) ||
			(TempImage->vContainerEjectReason == cPassContainerWrongHeight))
		{
			TempText1.LoadString(IDS_AcceptableHeightRange);
			if (vLocalConfigurationData->vShapeHandlingHeightTolerancePixels)
				TempString = TempString + "\n\n" + TempText1 + ": " + dtoa(((double)(vGlobalCurrentProduct->vReferenceTop - vGlobalCurrentProduct->vReferenceBottom) - (double)vLocalConfigurationData->vShapeHandlingHeightTolerancePixels) /
					(double)vGlobalPixelsPerUnitInHeight,2) + " (" + dtoa(((double)(vGlobalCurrentProduct->vReferenceTop - vGlobalCurrentProduct->vReferenceBottom) - (double)vLocalConfigurationData->vShapeHandlingHeightTolerancePixels),0) + 
					") - " + dtoa(((double)(vGlobalCurrentProduct->vReferenceTop - vGlobalCurrentProduct->vReferenceBottom) + (double)vLocalConfigurationData->vShapeHandlingHeightTolerancePixels) / (double)vGlobalPixelsPerUnitInHeight,2) + 
					" (" + dtoa(((double)(vGlobalCurrentProduct->vReferenceTop - vGlobalCurrentProduct->vReferenceBottom) + (double)vLocalConfigurationData->vShapeHandlingHeightTolerancePixels),0) + ")";
		}
						
		if ((TempImage->vContainerEjectReason == cEjectSlippedContainer) || (TempImage->vContainerEjectReason == cPassSlippedContainer))
		{
			TempText.LoadString(IDS_Containerisatsideofimage);
			TempString = TempString + "\n\n" + TempText;
		}

		if (TempImage->vContainerEjectReason == cEjectEmptySlippedContainer)
		{
			TempText.LoadString(IDS_EmptyContainer);
			TempString = TempString + "\n\n" + TempText;
		}

		if ((TempImage->vContainerEjectReason == cEjectContainerOnTop) || (TempImage->vContainerEjectReason == cPassContainerOnTop))
		{
			TempText.LoadString(IDS_Containerisattopofimage);
			TempString = TempString + "\n\n" + TempText;
		}
		if ((TempImage->vContainerEjectReason == cEjectContainerOnConveyorStop) || (TempImage->vContainerEjectReason == cPassContainerOnConveyorStop))
		{
			TempText1.LoadString(IDS_ConveyorStop);
				TempString = TempString + "\n\n" + TempText1; 
		}
		if (TempImage->vContainerEjectReason == cTestEject)
		{
			TempText.LoadString(IDS_TestEjectedContainer);
			TempString = TempString + "\n\n" + TempText;
		}

		//if (!vRejectsImageDisplay)
		if ((vInspectionNumberShowing < vGlobalCurrentProduct->vNumberOfInspections) && (vInspectionShowing) && (TempImage))
		{
			//information on current inspection for this image
			if (vInspectionShowing)
			if (vInspectionNumberShowing != TempEjectInspection)
			{
				TempString = TempString + TempDividerString;

				TempString = TempString + "\n\t\tAS DISPLAYED VIEWING INSPECTION";

				TempText.LoadString(IDS_ViewingInspection);
				TempString = TempString + "\n\n" + TempText + ": " + vInspectionShowing->vName;

				if (vInspectionShowing)
				if (PasswordOK(cTemporaryInspxPassword, false))
				{
					int TempPosition = vInspectionShowing->vName.Find(vInspectionShowing->GetInspectionTypeString(), 0);
					if (TempPosition < 0)
						TempString = TempString + "\n    Inspection Type: " + vInspectionShowing->GetInspectionTypeString();
				}

				if (vInspectionShowing->vInspectionType == cFloodedInspection)
				{
					TempString = TempString + "\nSpout Length Required to be greater or equal to: " + dtoa(vInspectionShowing->vSize, 0);
					TempString = TempString + "\nThreshold used to detect Spout Length: " + dtoa(vThreshold, 0);
				}
				else
				if (vInspectionShowing->vInspectionType == cStandardDeviationInspection)
				{ //density inspection
					double TempStandardDeviation = 0;
					if (vInspectionShowing->vPreSmoothImage)
					{
						CopyMemory(vLocalSystemData->vITIPCDig->vNIDDScratchImage->pix, TempImage->vGreyImage, vLocalSystemData->vITIPCDig->vOriginalBufferSize);
						DoMedianSmooth(vLocalSystemData->vITIPCDig->vNIDDScratchImage, vInspectionShowing->vPreSmoothImage, vInspectionShowing->vPreSmoothImage);
						TempStandardDeviation = vLocalSystemData->vITIPCDig->CalculateInspectionStandardDeviationAfterReference(vLocalSystemData->vITIPCDig->vNIDDScratchImage->pix, vInspectionShowing,TempImage->vLeftReferenceAdjust);
					}
					else
						TempStandardDeviation = vLocalSystemData->vITIPCDig->CalculateInspectionStandardDeviationAfterReference(TempImage->vGreyImage, vInspectionShowing,TempImage->vLeftReferenceAdjust);

					TempText.LoadString(IDS_ThiscontainersTextureValue);
					TempString = TempString + "\n    " + TempText + ": " + dtoa(TempStandardDeviation,2);
					TempText.LoadString(IDS_AcceptableTextureValue);

					TempString = TempString + "\n    " + TempText + ": " + dtoa(vInspectionShowing->vDensityThresholdLower,2) + "-" + dtoa(vInspectionShowing->vDensityThresholdUpper,2);
				}
				else
				if ((vInspectionShowing->vInspectionType == cUnderfillByWeightInspection) || (vInspectionShowing->vInspectionType == cCheckWeighInspection) || (vInspectionShowing->vInspectionType == cOverfillByWeightInspection))
				{ //weight inspection
					if (TempImage->vDensityValue[vInspectionNumberShowing]) //if calculated this weight when inspected
						TempString = TempString + "\n    Weight when Inspected: " + dtoaWithCommas(vInspectionShowing->ConvertToWeight(TempImage->vDensityValue[vInspectionNumberShowing]), 2);

					if (TempImage->vContainerWeightCorrection[vInspectionNumberShowing])
						TempString = TempString + "\n    Weight Offset when Inspected: " + dtoa(TempImage->vContainerWeightCorrection[vInspectionNumberShowing], 2) + "\n";

					//if (vInspectionNumberShowing == TempEjectInspection)
					//	TempString = TempString + "\n    Threshold when Inspected: " + 
					//	dtoaWithCommas(vGlobalCurrentProduct->vInspection[TempEjectInspection]->ConvertToWeight(TempImage->vContainerEjectThreshold), 2);
				}
				else
				if ((vInspectionShowing->vInspectionType == cDensityInspection) || (vInspectionShowing->vInspectionType == cMissingLidInspection) ||
					(vInspectionShowing->vInspectionType == cUnderfillInspection) || (vInspectionShowing->vInspectionType == cOverfillInspection))
				{ //density inspection
					double TempDensity = TempImage->vDensityValue[vInspectionNumberShowing];
					if (!TempDensity)
						TempDensity = vLocalSystemData->vITIPCDig->CalculateInspectionDensityAfterReference(TempImage->vGreyImage, vInspectionShowing,TempImage->vLeftReferenceAdjust, TempImage->vTopReferenceAdjust,
							vLocalConfigurationData->vReferenceROIsToEdges, TempImage->vLeft, TempImage->vRight); 

					TempDensity = vInspectionShowing->ConvertToWeight(TempDensity);

					if (((vInspectionShowing->vCheckWeighBFactor != 0) || (vInspectionShowing->vCheckWeighMFactor != 1)) && (vInspectionShowing->vCheckWeighMFactor))
						TempText.LoadString(IDS_ThisContainersWeight);
					else
						TempText.LoadString(IDS_ThiscontainersDensity);

					TempString = TempString + "\n    " + TempText + ": " + dtoa(TempDensity,2);

					if ((vInspectionShowing->vInspectionType == cDensityInspection) || (vInspectionShowing->vInspectionType == cCheckWeighInspection))
					{
						if (((vInspectionShowing->vCheckWeighBFactor != 0) || (vInspectionShowing->vCheckWeighMFactor != 1)) && (vInspectionShowing->vCheckWeighMFactor))
							TempText.LoadString(IDS_AcceptableWeight);
						else
							TempText.LoadString(IDS_AcceptableDensity);

						if (vInspectionShowing->vHasDensityLowerThreshold)
							TempString = TempString + "\n    " + TempText + ": " + dtoa(vInspectionShowing->ConvertToWeight(vInspectionShowing->vDensityThresholdUpper),2) + "-" + 
								dtoa(vInspectionShowing->ConvertToWeight(vInspectionShowing->vDensityThresholdLower),2);
						else
							TempString = TempString + "\n    " + TempText + ": 0-" + dtoa(vInspectionShowing->ConvertToWeight(vInspectionShowing->vDensityThresholdUpper),2);
					}
					else
					if ((vInspectionShowing->vInspectionType == cMissingLidInspection) || (vInspectionShowing->vInspectionType == cUnderfillInspection))
					{
						if (TempDensity >= vLastCalculateThreshold)
							TempString = TempString + "\nThis " + vLocalSystemData->vContainerString + " would be REJECTED";
						else
							TempString = TempString + "\nThis " + vLocalSystemData->vContainerString + " would be ACCEPTED";

						TempText.LoadString(IDS_Threshold);
						TempString = TempString + "\n    " + TempText + ": " + dtoa(vLastCalculateThreshold,2);
					}
					else
					if (vInspectionShowing->vInspectionType == cOverfillInspection)
					{
						TempText.LoadString(IDS_Threshold);
						TempString = TempString + "\n    " + TempText + ": " + dtoa(vInspectionShowing->vDensityThresholdLower,2);

						TempString = TempString + "\n    Acceptable Density: " + dtoa(vInspectionShowing->vDensityThresholdLower,2) + " - " + dtoa(vInspectionShowing->vDensityThresholdUpper,2);
					}
				}
				else
				if ((vInspectionShowing->vInspectionType == cItemCountInspection) || (vInspectionShowing->vInspectionType == cSizeInspection) || 
					(vInspectionShowing->vInspectionType == cMissingLidInspectionOnSize) || (vInspectionShowing->vInspectionType == cFloodedInspection)) //as displayed
				{ //size inspection
					if (vInspectionShowing->vInspectionType == cFloodedInspection)
						TempString = TempString + "\n    Size of spout: " + dtoa(vSizeOfAllContaminants,0);
					else
					if (vInspectionShowing->vInspectionType == cMissingLidInspectionOnSize)
						TempString = TempString + "\n    Size of Lid: " + dtoa(vSizeOfAllContaminants,0);
					else
					if (vInspectionShowing->vInspectionType == cItemCountInspection)
						TempString = TempString + "\n    Number of Items found meeting requirements: " + dtoa(vNumberOfContaminants,0);
					else
						TempString = TempString + "\n    Size of Item: " + dtoa(vSizeOfAllContaminants,0);

					if (vInspectionShowing->vInspectionType == cFloodedInspection)
						TempText = "Flooded IV Bag Inspection Settings";
					else
					if (vInspectionShowing->vInspectionType == cMissingLidInspectionOnSize)
						TempText = "Missing Lid Inspection Settings";
					else
					if (vInspectionShowing->vInspectionType == cItemCountInspection)
						TempText = "Item Count Inspection Settings";
					else
					if (vInspectionShowing->vInspectionType == cFloodedInspection)
						TempText = "Size of spout to detect flooded bag";
					else
						TempText = "Size of Item Inspection Settings";

					if (vInspectionShowing->vInspectionType == cMissingLidInspectionOnSize)
						TempString = TempString + "\n        Pixels must be darker than " + dtoa(vInspectionShowing->vThreshold,0) + " to be part of Lid";
					else
					if (vInspectionShowing->vInspectionType == cItemCountInspection)
						TempString = TempString + "\n        Pixels must be darker than " + dtoa(vInspectionShowing->vThreshold,0) + " to be part of an item";
					else
						TempString = TempString + "\n        Pixels must be darker than " + dtoa(vInspectionShowing->vThreshold,0) + " to be part of the item";

					if (vInspectionShowing->vInspectionType == cItemCountInspection)
					{
						if ((vInspectionShowing->vMinimumBrightnessAceptable) && (vInspectionShowing->vMinimumBrightnessAceptable < 255))
							TempString = TempString + "\n        Pixels must be brighter than " + dtoa(vInspectionShowing->vMinimumBrightnessAceptable,0) + " to be part of an item";
						else
							TempString = TempString + "\n        Pixels must be brighter than Threshold to be part of an item disabled";
					}

					if (vInspectionShowing->vInspectionType == cMissingLidInspectionOnSize)
						TempString = TempString + "\n        " + "Good Lids must have " + dtoa(vInspectionShowing->vSize,0) + " or more pixels";
					else
						TempString = TempString + "\n        " + "Good items must have " + dtoa(vInspectionShowing->vSize,0) + " or more pixels";

					if (vInspectionShowing->vInspectionType == cItemCountInspection)
					{
						if (vInspectionShowing->vMaximumSize)
							TempString = TempString + "\n        " + "Good items must have " + dtoa(vInspectionShowing->vMaximumSize,0) + " or fewer pixels";
						else
							TempString = TempString + "\n        " + "Maximum number of Pixels allowed in each item is disabled";

						if (vInspectionShowing->vMinimumNumberOfItemsRequired)
							TempString = TempString + "\n        " + "Good packages must have " + dtoa(vInspectionShowing->vMinimumNumberOfItemsRequired,0) + " or more items";
						else
							TempString = TempString + "\n        " + "Minimum number of items required in good package disabled";

						if (vInspectionShowing->vMaximumNumberOfItemsRequired)
							TempString = TempString + "\n        " + "Good packages must have " + dtoa(vInspectionShowing->vMaximumNumberOfItemsRequired,0) + " or fewer items";
						else
							TempString = TempString + "\n        " + "Maximum number of items permitted in good package disabled";
					}
				}
				else
				if (vInspectionShowing->vInspectionType == cUnderfillByWeightInspection)
				{ //image not rejected for this inspection.  Stats on Viewing inspection
					double TempCalculatedUnderfillByWeight = 0;
					if ((vInspectionShowing->vWhiteOutAnythingTouchingTheTop) && //remove the IV Bag if need to
						(vLocalSystemData->vITIPCDig->vNIDDScratchImage) && (vLocalSystemData->vITIPCDig->vNIDDScratchImage->pix))
					{
						CopyMemory(vLocalSystemData->vITIPCDig->vNIDDScratchImage->pix, TempImage->vGreyImage, vLocalSystemData->vITIPCDig->vOriginalBufferSize);
						vInspectionShowing->WhiteOutAnythingTouchingTheTop(vLocalSystemData->vITIPCDig->vNIDDScratchImage->pix, vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY);

						TempCalculatedUnderfillByWeight = vLocalSystemData->vITIPCDig->CalculateUnderfillByWeight(vLocalSystemData->vITIPCDig->vNIDDScratchImage->pix, vInspectionShowing, TempImage->vLeftReferenceAdjust); 
					}
					else
						TempCalculatedUnderfillByWeight = vLocalSystemData->vITIPCDig->CalculateUnderfillByWeight(TempImage->vGreyImage, vInspectionShowing, TempImage->vLeftReferenceAdjust); 

					if (vInspectionShowing->vCheckWeighOffset)
					{
						double TempWeight = vInspectionShowing->ConvertToWeight(TempCalculatedUnderfillByWeight) + vInspectionShowing->vCheckWeighOffset;
						TempCalculatedUnderfillByWeight = vInspectionShowing->ConvertWeightToBrightness(TempWeight);
					}

					if (TempCalculatedUnderfillByWeight < vThreshold)
						TempString = TempString + "\nWith current settings this container would be REJECTED";
					else
						TempString = TempString + "\nWith current settings this container would be ACCEPTED";

					//TempString = TempString + "\n    Weight when Inspected: " + dtoaWithCommas(vInspectionShowing->ConvertToWeight(TempImage->vDensityValue[vInspectionNumberShowing]), 2);

					//if (TempImage->vContainerWeightCorrection[TempEjectInspection])
					//	TempString = TempString + "\n    Weight Offset when Inspected: " + dtoa(TempImage->vContainerWeightCorrection[TempEjectInspection], 2) + "\n";

					//if (vInspectionNumberShowing == TempEjectInspection)
					//	TempString = TempString + "\n    Threshold when Inspected: " + dtoaWithCommas(vInspectionShowing->ConvertToWeight(TempImage->vContainerEjectThreshold), 2);

					TempString = TempString + "\n    Image Weight Now: " + dtoaWithCommas(vInspectionShowing->ConvertToWeight(TempCalculatedUnderfillByWeight), 2);
					if (vInspectionShowing->vCheckWeighOffset)
						TempString = TempString + "\n    Current Weight Offset: " + dtoa(vInspectionShowing->vCheckWeighOffset, 2) + "\n";

					TempString = TempString + "\n    Minimum Acceptable Weight (Threshold): " + dtoaWithCommas(vInspectionShowing->ConvertToWeight(vThreshold),2);
				}
				else
				if (vInspectionShowing->vInspectionType == cOverfillByWeightInspection)
				{ //image not rejected for this inspection.  Stats on Viewing inspection
					double TempCalculatedUnderfillByWeight = 0;
					if ((vInspectionShowing->vWhiteOutAnythingTouchingTheTop) && //remove the IV Bag if need to
						(vLocalSystemData->vITIPCDig->vNIDDScratchImage) && (vLocalSystemData->vITIPCDig->vNIDDScratchImage->pix))
					{
						CopyMemory(vLocalSystemData->vITIPCDig->vNIDDScratchImage->pix, TempImage->vGreyImage, vLocalSystemData->vITIPCDig->vOriginalBufferSize);
						vInspectionShowing->WhiteOutAnythingTouchingTheTop(vLocalSystemData->vITIPCDig->vNIDDScratchImage->pix, vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY);

						TempCalculatedUnderfillByWeight = vLocalSystemData->vITIPCDig->CalculateUnderfillByWeight(vLocalSystemData->vITIPCDig->vNIDDScratchImage->pix, vInspectionShowing, TempImage->vLeftReferenceAdjust); 
					}
					else
						TempCalculatedUnderfillByWeight = vLocalSystemData->vITIPCDig->CalculateUnderfillByWeight(TempImage->vGreyImage, vInspectionShowing, TempImage->vLeftReferenceAdjust); 

					if (vInspectionShowing->vCheckWeighOffset)
					{
						double TempWeight = vInspectionShowing->ConvertToWeight(TempCalculatedUnderfillByWeight) + vInspectionShowing->vCheckWeighOffset;
						TempCalculatedUnderfillByWeight = vInspectionShowing->ConvertWeightToBrightness(TempWeight);
						TempString = TempString + "\n    Current Weight Offset: " + dtoa(vInspectionShowing->vCheckWeighOffset, 2) + "\n";
					}

					if (TempCalculatedUnderfillByWeight > vThreshold)
						TempString = TempString + "\nWith current settings this container would be REJECTED";
					else
						TempString = TempString + "\nWith current settings this container would be ACCEPTED";

					//TempString = TempString + "\n    Weight when Inspected: " + dtoaWithCommas(vInspectionShowing->ConvertToWeight(TempImage->vDensityValue[vInspectionNumberShowing]), 2);

					//if (TempImage->vContainerWeightCorrection[TempEjectInspection])
					//	TempString = TempString + "\n    Weight Offset when Inspected: " + dtoa(TempImage->vContainerWeightCorrection[TempEjectInspection], 2) + "\n";

					//if (vInspectionNumberShowing == TempEjectInspection)
					//	TempString = TempString + "\n    Threshold when Inspected: " + dtoaWithCommas(vInspectionShowing->ConvertToWeight(TempImage->vContainerEjectThreshold), 2);

					TempString = TempString + "\n    Image Weight Now: " + dtoaWithCommas(vInspectionShowing->ConvertToWeight(TempCalculatedUnderfillByWeight), 2);
					if (vInspectionShowing->vCheckWeighOffset)
						TempString = TempString + "\n    Current Weight Offset: " + dtoa(vInspectionShowing->vCheckWeighOffset, 2) + "\n";

					TempString = TempString + "\n    Maximum Acceptable Weight (Threshold): " + dtoaWithCommas(vInspectionShowing->ConvertToWeight(vThreshold),2);
				}
				else
				{ //contaminant or void inspection
					if (vSizeOfLargestContaminant)
						TempString = TempString + "\nLargest Blob Size: " + dtoa(vSizeOfLargestContaminant,0);

					TempText.LoadString(IDS_InspectionThreshold);
					TempText1.LoadString(IDS_MinimumSize);

					if (vInspectionShowing->vInspectionType == cFloodedInspection)
						TempText1 = "Minimum Spout Length";

					TempString = TempString + "\n    " + TempText + ": " + dtoa(vThreshold,0) + ", " + TempText1 + ": " + dtoa(vInspectionShowing->vSize,0);

					if (vInspectionShowing->vInspectionType == cDripInspection)
					{
						if (vInspectionShowing->vMaximumSize)
							TempString = TempString + ", Maximum Size: " + dtoa(vInspectionShowing->vMaximumSize,0);
						if (vInspectionShowing->vMinimumWidth)
							TempString = TempString + ", Minimum Width: " + dtoa(vInspectionShowing->vMinimumWidth,0);
						if (vInspectionShowing->vMaximumHeightAboveBottom)
							TempString = TempString + ", Maximum Height Above Bottom: " + dtoa(vInspectionShowing->vMaximumHeightAboveBottom,0);
						
					}
				}
			}
		}

		if (vInspectionShowing)
		//if (vDarkestValueInROI < 255)
		{
			TempString = TempString + TempDividerString;
			TempString = TempString + "\n\t\t\tAS DISPLAYED";
			if (vInspectionShowing->vInspectionType != cUnderfillByWeightInspection)
			if (vInspectionShowing->vInspectionType != cOverfillByWeightInspection)
			{
				//if ((vInspectionNumberShowing != TempEjectInspection) || (TempImage->vContainerEjectThreshold != vLastCalculateThreshold))
				//else
				//	TempString = TempString + "\n\t\t\tAS EJECTED";

				TempString = TempString + "\n\nAnalysis of displayed image processed with displayed settings:";

				if (vInspectionShowing->vHasAMinimumSize)
				{
					if (vInspectionShowing->vInspectionType == cItemCountInspection)
					{
						if (((vInspectionShowing->vMinimumNumberOfItemsRequired) && (vNumberOfContaminants < vInspectionShowing->vMinimumNumberOfItemsRequired)) || 
							((vInspectionShowing->vMaximumNumberOfItemsRequired) && (vNumberOfContaminants > vInspectionShowing->vMaximumNumberOfItemsRequired)))
							TempString = TempString + "\nThis " + vLocalSystemData->vContainerString + " would be REJECTED";
						else
							TempString = TempString + "\nThis " + vLocalSystemData->vContainerString + " would be ACCEPTED";
					}
					else
					if ((vInspectionShowing->vInspectionType == cSizeInspection) || (vInspectionShowing->vInspectionType == cMissingLidInspectionOnSize))
					{
						if (vSizeOfLargestContaminant < vInspectionShowing->vSize)
							TempString = TempString + "\nThis " + vLocalSystemData->vContainerString + " would be REJECTED";
						else
							TempString = TempString + "\nThis " + vLocalSystemData->vContainerString + " would be ACCEPTED";
					}
					else
					{
						if (vSizeOfLargestContaminant >= vInspectionShowing->vSize)
							TempString = TempString + "\nThis " + vLocalSystemData->vContainerString + " would be REJECTED";
						else
							TempString = TempString + "\nThis " + vLocalSystemData->vContainerString + " would be ACCEPTED";
					}

					if (vSizeOfLargestContaminant)
					{
						if ((TempEjectInspection < vGlobalCurrentProduct->vNumberOfInspections) && (vInspectionShowing) && (vInspectionShowing->vInspectionType == cItemCountInspection))
							TempString = TempString + "\nLargest item: " + dtoa(vSizeOfLargestContaminant,0);
						else
						if ((TempEjectInspection < vGlobalCurrentProduct->vNumberOfInspections) && (vInspectionShowing) && (vInspectionShowing->vInspectionType == cSizeInspection))
							TempString = TempString + "\n Size of largest piece of item: " + dtoa(vSizeOfLargestContaminant,0);
						else
						if ((TempEjectInspection < vGlobalCurrentProduct->vNumberOfInspections) && (vInspectionShowing) && (vInspectionShowing->vInspectionType == cMissingLidInspectionOnSize))
							TempString = TempString + "\nSize of largest piece of Lid: " + dtoa(vSizeOfLargestContaminant,0);
						else
							TempString = TempString + "\nLargest Blob Size: " + dtoa(vSizeOfLargestContaminant,0);
					}
					if ((vInspectionShowing->vInspectionType != cSizeInspection) &&(vInspectionShowing->vInspectionType != cMissingLidInspectionOnSize) &&(vInspectionShowing->vInspectionType != cFloodedInspection))
					if (vNumberOfContaminants)
					{
						CString TempContaminantString = "Contaminants";
						if ((vInspectionShowing->vInspectionType == cItemCountInspection) || (vInspectionShowing->vInspectionType == cSizeInspection) || (vInspectionShowing->vInspectionType == cMissingLidInspectionOnSize))
							TempContaminantString = "Items";
						TempString = TempString + "\nNumber Of " + TempContaminantString + " (that meet Minimum Size): " + dtoa(vNumberOfContaminants,0);
					}
					if (vSizeOfAllContaminants)
					{
						TempText.LoadString(IDS_Thiscontainersnumberofdefects);
						TempString = TempString + "\n" + TempText + " (that meet Minimum Size): " + dtoa(vSizeOfAllContaminants,0);
					}
				}
			}

			if ((vInspectionShowing->vInspectionType == cItemCountInspection) || (vInspectionShowing->vInspectionType == cSizeInspection) || 
				(vInspectionShowing->vInspectionType == cMissingLidInspectionOnSize) || (vInspectionShowing->vInspectionType == cFloodedInspection)) //as displayed
			if (vInspectionShowing == vGlobalCurrentProduct->vInspection[TempEjectInspection]) // if showing same inspecting that ejected, show its settings
			{ //size inspection
				if (vInspectionShowing->vInspectionType == cFloodedInspection)
					TempString = TempString + "\n    Spout Size: " + dtoa(vSizeOfAllContaminants, 0);
				else
				if (vInspectionShowing->vInspectionType == cMissingLidInspectionOnSize)
					TempString = TempString + "\n    Size of Lid: " + dtoa(vSizeOfAllContaminants, 0);
				else
				if (vInspectionShowing->vInspectionType == cItemCountInspection)
					TempString = TempString + "\n    Number of Items found meeting requirements: " + dtoa(vNumberOfContaminants, 0);
				else
					TempString = TempString + "\n    Size of Item: " + dtoa(vSizeOfAllContaminants, 0);

				if (vInspectionShowing->vInspectionType == cFloodedInspection)
					TempText = "Flooded IV Bag Inspection Settings";
				else
				if (vInspectionShowing->vInspectionType == cMissingLidInspectionOnSize)
					TempText = "Missing Lid Inspection Settings";
				else
				if (vInspectionShowing->vInspectionType == cItemCountInspection)
					TempText = "Item Count Inspection Settings";
				else
				if (vInspectionShowing->vInspectionType == cFloodedInspection)
					TempText = "Size of spout to detect flooded bag";
				else
					TempText = "Size of Item Inspection Settings";

				if (vInspectionShowing->vInspectionType == cFloodedInspection)
					TempString = TempString + "\n        Pixels must be darker than " + dtoa(vInspectionShowing->vThreshold,0) + " to be part of spout";
				else
				if (vInspectionShowing->vInspectionType == cMissingLidInspectionOnSize)
					TempString = TempString + "\n        Pixels must be darker than " + dtoa(vInspectionShowing->vThreshold,0) + " to be part of Lid";
				else
				if (vInspectionShowing->vInspectionType == cItemCountInspection)
					TempString = TempString + "\n        Pixels must be darker than " + dtoa(vInspectionShowing->vThreshold,0) + " to be part of an item";
				else
					TempString = TempString + "\n        Pixels must be darker than " + dtoa(vInspectionShowing->vThreshold,0) + " to be part of item";

				if (vInspectionShowing->vInspectionType == cItemCountInspection)
				{
					if ((vInspectionShowing->vMinimumBrightnessAceptable) && (vInspectionShowing->vMinimumBrightnessAceptable < 255))
						TempString = TempString + "\n        Pixels must be brighter than " + dtoa(vInspectionShowing->vMinimumBrightnessAceptable,0) + " to be part of an item";
					else
						TempString = TempString + "\n        Pixels must be brighter than Threshold to be part of an item disabled";
				}

				if (vInspectionShowing->vInspectionType == cMissingLidInspectionOnSize)
					TempString = TempString + "\n        " + "Minimum number of Pixels in Lid to be complete: " + dtoa(vInspectionShowing->vSize,0);
				else
				if (vInspectionShowing->vInspectionType == cItemCountInspection)
					TempString = TempString + "\n        " + "Good items have " + dtoa(vInspectionShowing->vSize,0) + " or more pixels";
				else
					TempString = TempString + "\n        " + "Item must have " + dtoa(vInspectionShowing->vSize,0) + " or more pixels";

				if (vInspectionShowing->vInspectionType == cItemCountInspection)
				{
					if (vInspectionShowing->vMaximumSize)
						TempString = TempString + "\n        " + "Good items have " + dtoa(vInspectionShowing->vMaximumSize,0) + " or fewer pixels";
					else
						TempString = TempString + "\n        " + "Maximum number of Pixels allowed in each item is disabled";

					if (vInspectionShowing->vMinimumNumberOfItemsRequired)
						TempString = TempString + "\n        " + "Must have " + dtoa(vInspectionShowing->vMinimumNumberOfItemsRequired,0) + " or more items for package to be good";
					else
						TempString = TempString + "\n        " + "Minimum number of items required in good package disabled";

					if (vInspectionShowing->vMaximumNumberOfItemsRequired)
						TempString = TempString + "\n        " + "Must have " + dtoa(vInspectionShowing->vMaximumNumberOfItemsRequired,0) + " or fewer items for package to be good";
					else
						TempString = TempString + "\n        " + "Maximum number of items permitted in good package disabled";
				}
			}

			TempText.LoadString(IDS_ValueofdarkestPixelinROI);
			TempString = TempString + "\n    " + TempText + ": " + dtoa(vDarkestValueInROI,0);
			TempText.LoadString(IDS_ValueoflightestPixelinROI);
			TempString = TempString + "\n    " + TempText + ": " + dtoa(vLightestValueInROI,0);
			TempString = TempString + "\n" + dtoa(vShowingInspectionNumberOfRedPixels,0) + " pixels below Threshold: " + dtoa(vLastCalculateThreshold,0);
			//TempString = TempString + "\nCurrent Minimum Size: " + dtoa(vInspectionShowing->vSize,0);

			if ((vInspectionShowing->vInspectionType == cDripInspection) && (vInspectionShowing->vMaximumSize))
				TempString = TempString + "-" + dtoa(vInspectionShowing->vMaximumSize,0);
			if ((vInspectionShowing->vInspectionType == cDripInspection) && (vInspectionShowing->vMinimumWidth))
				TempString = TempString + ", Minimum Width: " + dtoa(vInspectionShowing->vMinimumWidth,0);
			if ((vInspectionShowing->vInspectionType == cDripInspection) && (vInspectionShowing->vMaximumHeightAboveBottom))
				TempString = TempString + ", Maximum Height Above Bottom: " + dtoa(vInspectionShowing->vMaximumHeightAboveBottom,0);

			if (vInspectionShowing->vHasAMinimumSize)  //show all blobs bigger than 1
			if (vBlobResults->num_blobs_found)
			if (vInspectionShowing)
			{
				const int cTab1 = 3;
				const int cTab2 = 10;
				const int cTab2n = 12;
				const int cTab3 = 25;
				const int cTab3n = 24;
				const int cTab4 = 40;
				const int cTab4n = 42;
				const int cTab5 = 55;

				CString TempContaminantString = "Contaminants (Red * = Meets Minimum Size Eject Criteria)";
				if ((vInspectionShowing->vInspectionType == cItemCountInspection) || (vInspectionShowing->vInspectionType == cSizeInspection) || (vInspectionShowing->vInspectionType == cMissingLidInspectionOnSize))
					TempContaminantString = "Items";
				TempString = TempString + "\n\n" + TempContaminantString;

				//TempString = TempString + "\n" + TempContaminantString + ":Size or Area (Width,Height), Position (X,Y) Maximum is (" + 
				//	dtoa(vLocalSystemData->vITIPCDig->vDisplayBufferWidthInPixels,0) + ", " + dtoa(vLocalSystemData->vITIPCDig->vDisplayBufferHeightInPixels,0) + ")";

				CString TempLineText = "";

				FillToTab(&TempLineText, cTab2);
				TempLineText = TempLineText + "Size or Area";

				FillToTab(&TempLineText, cTab3);
				TempLineText = TempLineText + "Width, Height";

				FillToTab(&TempLineText, cTab4);
				TempLineText = TempLineText + "Position X, Y (Maximum is: " + 
					dtoa(vLocalSystemData->vITIPCDig->vOriginalBufferSizeY,0) + ", " + dtoa(vLocalSystemData->vITIPCDig->vOriginalBufferSizeX,0) + ")";
				
				TempString = TempString + "\n" + TempLineText;

				if (vInspectionShowing->vInspectionType == cBadHeartInspection)
					TempString = TempString + ", Perimeter, Ratio";

				for (WORD TempLoop = 0; TempLoop < vBlobResults->num_blobs_found; TempLoop++)
				{
#ifdef UseIFCDLLs
					MVT_BLOB *TempBlob = vBlobResults->blobs[TempLoop];

					CString TempSmaller("");
					if (vInspectionShowing->vSize <= TempBlob->area)
						TempSmaller = "*";
					TempLineText = TempSmaller;

					//TempString = TempString + "\n        " + TempSmaller + dtoa(TempLoop + 1,0) + ": " + dtoa(TempBlob->area,0) + "   (" + dtoa(TempBlob->height,0) + ", " +
					//	dtoa(TempBlob->width,0) + ")   (" + dtoa(TempBlob->ycent,0) + ", " + dtoa(TempBlob->xcent,0) + ")" + TempEndText;

					FillToTab(&TempLineText, cTab1);
					TempLineText = TempLineText + Pad(dtoa(TempLoop + 1, 0), 3, 0);

					FillToTab(&TempLineText, cTab2n);
					TempLineText = TempLineText + Pad(dtoa(TempBlob->area, 0), 5, 0);

					FillToTab(&TempLineText, cTab3n);
					TempLineText = TempLineText + Pad(dtoa(TempBlob->height, 0), 4, 0) + ", " + Pad(dtoa(TempBlob->width,0), 4, 0);

					FillToTab(&TempLineText, cTab4n);
					TempLineText = TempLineText + Pad(dtoa(TempBlob->ycent, 0), 4, 0) + ", " + Pad(dtoa(TempBlob->xcent, 0), 4, 0);

					CString TempEndText = "";
					if (vInspectionShowing->vInspectionType == cBadHeartInspection)
						TempEndText = ", " + dtoa(TempBlob->perim, 0) + ", " + dtoa((double)TempBlob->area / (double)TempBlob->perim,2);

					if ((vInspectionShowing->vInspectionType == cDripInspection) && (vInspectionShowing->vMaximumHeightAboveBottom))
					{
						if (TempBlob->xcent + vLocalSystemData->vITIPCDig->vLastBottomReferenceAdjust > TempImage->vBottom + vInspectionShowing->vMaximumHeightAboveBottom) //zero out this blobs area so it does not count if it is above the limit
						{
							TempEndText = ", blob above bottom cutof line, excluded";
							TempSmaller = "";
						}
					}

					TempString = TempString + "\n" + TempLineText + TempEndText;

#endif
				}
			}

			if (vInspectionShowing)
			if (vInspectionShowing->vInspectionType == cFloodedInspection)
			{
					if (vNumberOfContaminants < vInspectionShowing->vSize) //spout length shorter than required length
						TempString = TempString + "\n\nWith current settings this container would be REJECTED";
					else
						TempString = TempString + "\n\nWith current settings this container would be ACCEPTED";

					TempString = TempString + "\nSpout Length with current settings: " + dtoa(vNumberOfContaminants, 0);
					TempString = TempString + "\nSpout Length Required to be greater or equal to: " + dtoa(vInspectionShowing->vSize, 0);
					TempString = TempString + "\nThreshold used to detect Spout Length: " + dtoa(vThreshold, 0);
			}
			else
			if (vInspectionShowing->vInspectionType == cUnderfillByWeightInspection)
			{ //image not rejected for this inspection.  Stats on Viewing inspection
				double TempCalculatedUnderfillByWeight = 0;
				if ((vInspectionShowing->vWhiteOutAnythingTouchingTheTop) && //remove the IV Bag if need to
					(vLocalSystemData->vITIPCDig->vNIDDScratchImage) && (vLocalSystemData->vITIPCDig->vNIDDScratchImage->pix))
				{
					CopyMemory(vLocalSystemData->vITIPCDig->vNIDDScratchImage->pix, TempImage->vGreyImage, vLocalSystemData->vITIPCDig->vOriginalBufferSize);
					vInspectionShowing->WhiteOutAnythingTouchingTheTop(vLocalSystemData->vITIPCDig->vNIDDScratchImage->pix, vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY);

					TempCalculatedUnderfillByWeight = vLocalSystemData->vITIPCDig->CalculateUnderfillByWeight(vLocalSystemData->vITIPCDig->vNIDDScratchImage->pix, vInspectionShowing, TempImage->vLeftReferenceAdjust); 
				}
				else
					TempCalculatedUnderfillByWeight = vLocalSystemData->vITIPCDig->CalculateUnderfillByWeight(TempImage->vGreyImage, vInspectionShowing, TempImage->vLeftReferenceAdjust); 

				if (vInspectionShowing->vCheckWeighOffset)
				{
					double TempWeight = vInspectionShowing->ConvertToWeight(TempCalculatedUnderfillByWeight) + vInspectionShowing->vCheckWeighOffset;
					TempCalculatedUnderfillByWeight = vInspectionShowing->ConvertWeightToBrightness(TempWeight);
				}

				if (TempCalculatedUnderfillByWeight < vThreshold)
					TempString = TempString + "\nWith current settings this container would be REJECTED";
				else
					TempString = TempString + "\nWith current settings this container would be ACCEPTED";

				TempString = TempString + "\n    Image Weight Now: " + dtoaWithCommas(vInspectionShowing->ConvertToWeight(TempCalculatedUnderfillByWeight), 2);
				if (vInspectionShowing->vCheckWeighOffset)
					TempString = TempString + "\n    Current Weight Offset: " + dtoa(vInspectionShowing->vCheckWeighOffset, 2) + "\n";

				TempString = TempString + "\n    Minimum Acceptable Weight (Threshold): " + dtoaWithCommas(vInspectionShowing->ConvertToWeight(vThreshold),2);
				//TempString = TempString + TempDividerString;
			}
			else
			if (vInspectionShowing->vInspectionType == cOverfillByWeightInspection)
			{ //image not rejected for this inspection.  Stats on Viewing inspection
				double TempCalculatedUnderfillByWeight = 0;
				if ((vInspectionShowing->vWhiteOutAnythingTouchingTheTop) && //remove the IV Bag if need to
					(vLocalSystemData->vITIPCDig->vNIDDScratchImage) && (vLocalSystemData->vITIPCDig->vNIDDScratchImage->pix))
				{
					CopyMemory(vLocalSystemData->vITIPCDig->vNIDDScratchImage->pix, TempImage->vGreyImage, vLocalSystemData->vITIPCDig->vOriginalBufferSize);
					vInspectionShowing->WhiteOutAnythingTouchingTheTop(vLocalSystemData->vITIPCDig->vNIDDScratchImage->pix, vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY);

					TempCalculatedUnderfillByWeight = vLocalSystemData->vITIPCDig->CalculateUnderfillByWeight(vLocalSystemData->vITIPCDig->vNIDDScratchImage->pix, vInspectionShowing, TempImage->vLeftReferenceAdjust); 
				}
				else
					TempCalculatedUnderfillByWeight = vLocalSystemData->vITIPCDig->CalculateUnderfillByWeight(TempImage->vGreyImage, vInspectionShowing, TempImage->vLeftReferenceAdjust); 

				if (vInspectionShowing->vCheckWeighOffset)
				{
					double TempWeight = vInspectionShowing->ConvertToWeight(TempCalculatedUnderfillByWeight) + vInspectionShowing->vCheckWeighOffset;
					TempCalculatedUnderfillByWeight = vInspectionShowing->ConvertWeightToBrightness(TempWeight);
				}

				if (TempCalculatedUnderfillByWeight > vThreshold)
					TempString = TempString + "\nWith current settings this container would be REJECTED";
				else
					TempString = TempString + "\nWith current settings this container would be ACCEPTED";

				TempString = TempString + "\n    Image Weight Now: " + dtoaWithCommas(vInspectionShowing->ConvertToWeight(TempCalculatedUnderfillByWeight), 2);
				if (vInspectionShowing->vCheckWeighOffset)
					TempString = TempString + "\n    Current Weight Offset: " + dtoa(vInspectionShowing->vCheckWeighOffset, 2) + "\n";

				TempString = TempString + "\n    Maximum Acceptable Weight (Threshold): " + dtoaWithCommas(vInspectionShowing->ConvertToWeight(vThreshold),2);
			}
			else
			if ((vInspectionShowing->vHasAMinimumSize) && (!vNumberOfContaminants))
			{
				//TempString = TempString + "\n\t\t\tAS DISPLAYED";
				TempText.LoadString(IDS_ViewingInspection);
				TempString = TempString + "\n\n" + TempText + ": " + vInspectionShowing->vName;

				if (vInspectionShowing)
				if (PasswordOK(cTemporaryInspxPassword, false))
				{
					int TempPosition = vInspectionShowing->vName.Find(vInspectionShowing->GetInspectionTypeString(), 0);
					if (TempPosition < 0)
						TempString = TempString + "\n    Inspection Type: " + vInspectionShowing->GetInspectionTypeString();
				}

				if ((vInspectionShowing->vInspectionType == cItemCountInspection) || (vInspectionShowing->vInspectionType == cSizeInspection) || (vInspectionShowing->vInspectionType == cMissingLidInspectionOnSize))
					TempString = TempString + "\nThis container would be REJECTED with the current inspection settings";
				else
					TempString = TempString + "\nThis container would be ACCEPTED with the current inspection settings";
				//TempString = TempString + TempDividerString;
			}
			else
			if ((vInspectionShowing->vInspectionType == cDensityInspection) ||
				(vInspectionShowing->vInspectionType == cCheckWeighInspection) ||
				(vInspectionShowing->vInspectionType == cMissingLidInspection) ||
				(vInspectionShowing->vInspectionType == cUnderfillInspection) ||
				(vInspectionShowing->vInspectionType == cOverfillInspection))
			{ //density inspection
				//TempString = TempString + "\n\t\t\tAS DISPLAYED";
				TempText.LoadString(IDS_ViewingInspection);
				TempString = TempString + "\n\n" + TempText + ": " + vInspectionShowing->vName;

				if (vInspectionShowing)
				if (PasswordOK(cTemporaryInspxPassword, false))
				{
					int TempPosition = vInspectionShowing->vName.Find(vInspectionShowing->GetInspectionTypeString(), 0);
					if (TempPosition < 0)
						TempString = TempString + "\n    Inspection Type: " + vInspectionShowing->GetInspectionTypeString();
				}

				double TempDensity = 0;

				TempDensity = vLocalSystemData->vITIPCDig->CalculateInspectionDensityAfterReference(TempImage->vGreyImage, vInspectionShowing,TempImage->vLeftReferenceAdjust,
					TempImage->vTopReferenceAdjust, vLocalConfigurationData->vReferenceROIsToEdges, TempImage->vLeft, TempImage->vRight); 

				if (((vInspectionShowing->vCheckWeighBFactor != 0) || (vInspectionShowing->vCheckWeighMFactor != 1)) && (vInspectionShowing->vCheckWeighMFactor))
				{
					double TempCalculatedUnderfillByWeight = 0;
					if ((vInspectionShowing->vWhiteOutAnythingTouchingTheTop) && //remove the IV Bag if need to
						(vLocalSystemData->vITIPCDig->vNIDDScratchImage) && (vLocalSystemData->vITIPCDig->vNIDDScratchImage->pix))
					{
						CopyMemory(vLocalSystemData->vITIPCDig->vNIDDScratchImage->pix, TempImage->vGreyImage, vLocalSystemData->vITIPCDig->vOriginalBufferSize);
						vInspectionShowing->WhiteOutAnythingTouchingTheTop(vLocalSystemData->vITIPCDig->vNIDDScratchImage->pix, vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY);

						TempCalculatedUnderfillByWeight = vLocalSystemData->vITIPCDig->CalculateUnderfillByWeight(vLocalSystemData->vITIPCDig->vNIDDScratchImage->pix, vInspectionShowing, TempImage->vLeftReferenceAdjust); 
					}
					else
						TempCalculatedUnderfillByWeight = vLocalSystemData->vITIPCDig->CalculateUnderfillByWeight(TempImage->vGreyImage, vInspectionShowing, TempImage->vLeftReferenceAdjust); 

					TempText.LoadString(IDS_ThisContainersWeight);
					if (vInspectionShowing->vCheckWeighOffset)
					{
						double TempWeight = vInspectionShowing->ConvertToWeight(TempCalculatedUnderfillByWeight) + vInspectionShowing->vCheckWeighOffset;
						TempCalculatedUnderfillByWeight = vInspectionShowing->ConvertWeightToBrightness(TempWeight);
					}

					TempString = TempString + "\n    " + TempText + ": " + dtoa(vInspectionShowing->ConvertToWeight(TempCalculatedUnderfillByWeight),2);
				}
				else
				{
					TempText.LoadString(IDS_ThiscontainersDensity);
					TempString = TempString + "\n    " + TempText + ": " + dtoa(vInspectionShowing->ConvertToWeight(TempDensity),2);
				}

				if ((vInspectionShowing->vInspectionType == cDensityInspection) || (vInspectionShowing->vInspectionType == cCheckWeighInspection))
				{
					if (((vInspectionShowing->vCheckWeighBFactor != 0) || (vInspectionShowing->vCheckWeighMFactor != 1)) && (vInspectionShowing->vCheckWeighMFactor))
						TempText.LoadString(IDS_AcceptableWeight);
					else
						TempText.LoadString(IDS_AcceptableDensity);

					if (vInspectionShowing->vHasDensityLowerThreshold)
						TempString = TempString + "\n    " + TempText + ": " + dtoa(vInspectionShowing->ConvertToWeight(vInspectionShowing->vDensityThresholdUpper),2) + "-" + 
							dtoa(vInspectionShowing->ConvertToWeight(vInspectionShowing->vDensityThresholdLower),2);
					else
						TempString = TempString + "\n    " + TempText + ": 0-" + dtoa(vInspectionShowing->ConvertToWeight(vInspectionShowing->vDensityThresholdUpper),2);
				}
				else
				if ((vInspectionShowing->vInspectionType == cMissingLidInspection) ||					
					(vInspectionShowing->vInspectionType == cUnderfillInspection))
				{
					if (TempDensity >= vThreshold)
						TempString = TempString + "\nThis container would be REJECTED";
					else
						TempString = TempString + "\nThis container would be ACCEPTED";

					TempText.LoadString(IDS_Threshold);
					TempString = TempString + "\n    " + TempText + ": " + dtoa(vThreshold,2);
					TempString = TempString + "\n    Acceptable Density: " + dtoa(vInspectionShowing->vDensityThresholdLower,2) + " - " + dtoa(vThreshold,2);
				}
				else
				if (vInspectionShowing->vInspectionType == cOverfillInspection)
				{
					if (TempDensity < vThreshold)
						TempString = TempString + "\nThis container would be REJECTED";
					else
						TempString = TempString + "\nThis container would be ACCEPTED";

					TempText.LoadString(IDS_Threshold);
					TempString = TempString + "\n    " + TempText + ": " + dtoa(vThreshold,2);
					TempString = TempString + "\n    Acceptable Density: " + dtoa(vThreshold,2) + " - " + dtoa(vInspectionShowing->vDensityThresholdUpper,2);
				}
				//TempString = TempString + TempDividerString;
			}
		}

		TempString = TempString + TempDividerString;

		if (vLocalConfigurationData->vShapeHandlingWidthTolerancePixels)
		{
			TempText.LoadString(IDS_AcceptableWidthRange);
			TempString = TempString + "\n\n" + TempText + ": " + 
				dtoa(((double)vGlobalCurrentProduct->vReferenceWidth - 
				(double)vLocalConfigurationData->vShapeHandlingWidthTolerancePixels) /
				(double)(vGlobalPixelsPerUnit * vGlobalCurrentProduct->vOverScanMultiplier),2) + 
				" (" +
				dtoa(((double)vGlobalCurrentProduct->vReferenceWidth - 
				(double)vLocalConfigurationData->vShapeHandlingWidthTolerancePixels),0) + 
				") - " +
				dtoa(((double)vGlobalCurrentProduct->vReferenceWidth + 
				(double)vLocalConfigurationData->vShapeHandlingWidthTolerancePixels) /
				(double)(vGlobalPixelsPerUnit * vGlobalCurrentProduct->vOverScanMultiplier),2) + 
				" (" +
				dtoa(((double)vGlobalCurrentProduct->vReferenceWidth + 
				(double)vLocalConfigurationData->vShapeHandlingWidthTolerancePixels),0) + ")";
		}

		if (vLocalConfigurationData->vShapeHandlingHeightTolerancePixels)
		{
			TempText.LoadString(IDS_AcceptableHeightRange);
			TempString = TempString + "\n" + TempText + ": " + 
				dtoa(((double)(vGlobalCurrentProduct->vReferenceTop - vGlobalCurrentProduct->vReferenceBottom) - (double)vLocalConfigurationData->vShapeHandlingHeightTolerancePixels) / (double)vGlobalPixelsPerUnitInHeight,2) + 
				" (" + dtoa(((double)(vGlobalCurrentProduct->vReferenceTop - vGlobalCurrentProduct->vReferenceBottom) - (double)vLocalConfigurationData->vShapeHandlingHeightTolerancePixels),0) + 
				") - " + dtoa(((double)(vGlobalCurrentProduct->vReferenceTop - vGlobalCurrentProduct->vReferenceBottom) + (double)vLocalConfigurationData->vShapeHandlingHeightTolerancePixels) / (double)vGlobalPixelsPerUnitInHeight,2) + 
				" (" + dtoa(((double)(vGlobalCurrentProduct->vReferenceTop - vGlobalCurrentProduct->vReferenceBottom) + (double)vLocalConfigurationData->vShapeHandlingHeightTolerancePixels),0) + ")";
		}

		//show image size
		TempText.LoadString(IDS_SizeofDisplay);
		TempString = TempString + "\n\n" + TempText;
		TempString = TempString + "\n    Display Height: " +  dtoa(vLocalSystemData->vITIPCDig->vDisplayImageSizeY,0);

		TempString = TempString + "\n    Display Width: " + dtoa(vLocalSystemData->vITIPCDig->vDisplayImageSizeX,0);

		TempString = TempString + "\n\nSize of Image";
		TempText.LoadString(IDS_ImageHeight);
		TempString = TempString + "\n    " + TempText + ": " + 
			dtoa((double)vLocalSystemData->vITIPCDig->vOriginalBufferSizeX / 
			(double)vGlobalPixelsPerUnitInHeight,2) +
			" (" + dtoa(vLocalSystemData->vITIPCDig->vOriginalBufferSizeX,0) + ")";

		if (vGlobalCurrentProduct->vMultiLaneNumberOfLanes)
		TempString = TempString + "  (Lane Width)";

		TempText.LoadString(IDS_ImageWidth);
		TempString = TempString + "\n    " + TempText + ": " + 
			dtoa(((double)vLocalSystemData->vITIPCDig->vAquisitionSizeY / 
			(double)vGlobalPixelsPerUnit) / (double)vGlobalCurrentProduct->vOverScanMultiplier,2) +
			" (" + dtoa(vLocalSystemData->vITIPCDig->vAquisitionSizeY,0) + ")";

		//show scale
		/*
		if ((!vRejectsImageDisplay) && (vLocalConfigurationData->vImageShowScale))
		{
			TempText.LoadString(IDS_ScalelinespacinginHeight);
			TempString = TempString + "\n    " + TempText + ": " + 
				dtoa(((double)vLocalSystemData->vITIPCDig->vOriginalBufferSizeX / 
				(double)vGlobalPixelsPerUnitInHeight) / 10.0,2) +
				" (" + dtoa((double)vLocalSystemData->vITIPCDig->vOriginalBufferSizeX / 10.0,1) + ")";

			TempText.LoadString(IDS_ScalelinespacinginWidth);
			TempString = TempString + "\n    " + TempText + ": " + 
				dtoa((((double)vLocalSystemData->vITIPCDig->vAquisitionSizeY / 
				(double)vGlobalPixelsPerUnit) / (double)vGlobalCurrentProduct->vOverScanMultiplier) /
				10.0,2) +
				" (" + dtoa((double)vLocalSystemData->vITIPCDig->vAquisitionSizeY / 10.0,1) + ")";
		}
		*/

		//Show Units
		TempText.LoadString(IDS_Valuesarein);
		TempText1.LoadString(IDS_Pixels);
		TempString = TempString + "\n\n" + TempText + ": " + vLocalConfigurationData->vUnitsString +
			" (" + TempText1 + ")";

		//show inspx only items next
		if (TempImage)
		if (PasswordOK(cTemporaryInspxPassword,false))  
		{
			if (TempImage->vRedBoxWidth[0])
			{
				TempString = TempString + TempDividerString;
				//TempString = TempString + "Red Box Lower Left Corner: (" + dtoa(TempImage->vRedBoxCornerY[0],0) + ", " + 
				//	dtoa(TempImage->vRedBoxCornerX[0],0) + ")";
				//TempString = TempString + ", Width: " + dtoa(TempImage->vRedBoxWidth[0],0) + ", Height: " + dtoa(TempImage->vRedBoxHeight[0],0);

				//CDC* TempDeviceContext = new CDC;
				//TempDeviceContext->CreateCompatibleDC(NULL);
				//TempDeviceContext->SetAttribDC(TempDeviceContext->m_hDC);

				const int cTab1 = 5;
				const int cTab2 = 20;
				const int cTab3 = 35;
				const int cTab4 = 50;
				const int cTab5 = 65;


				CString TempLineText = "";
				//TempLineText.LoadString(IDS_RedBoxLowerLeftCorner);
				TempString = TempString + "\n\nRed Boxes:";

				TempLineText = "Lower Left Corner";

				FillToTab(&TempLineText, cTab2);
				TempLineText = TempLineText + "Width";

				FillToTab(&TempLineText, cTab3);
				TempLineText = TempLineText + "Height";

				FillToTab(&TempLineText, cTab4);
				TempLineText = TempLineText + "Inspection Bit Map";

				TempString = TempString + "\n" + TempLineText;

				for (BYTE TempLoop = 0; TempLoop < cMaximumRedBoxes; TempLoop++)
				if (TempImage->vRedBoxWidth[TempLoop])
				{
					//TempText.LoadString(IDS_RedBoxLowerLeftCorner);
					//TempString = TempString + "\n" + TempText + ": (" + dtoa(TempImage->vRedBoxCornerY[TempLoop],0) + ", " + dtoa(TempImage->vRedBoxCornerX[TempLoop],0) + ")";
					//TempString = TempString + ", Width: " + dtoa(TempImage->vRedBoxWidth[TempLoop],0) + ", Height: " + dtoa(TempImage->vRedBoxHeight[TempLoop],0) + ", Image: " + vGlobalImageName[TempImage->vRedBoxImage[TempLoop]] + 
					//	", Inspection Bit map: " + WordToHex(TempImage->vRedBoxInspection[TempLoop] >> 16) + " " + WordToHex((WORD)TempImage->vRedBoxInspection[TempLoop]);
					CString TempLineText = "";
					FillToTab(&TempLineText, cTab1);
					TempLineText = Pad(dtoa(TempImage->vRedBoxCornerY[TempLoop], 0), 5, 0) + ", " + Pad(dtoa(TempImage->vRedBoxCornerX[TempLoop], 0), 5, 0);

					FillToTab(&TempLineText, cTab2);
					TempLineText = TempLineText + Pad(dtoa(TempImage->vRedBoxWidth[TempLoop], 0), 4, 0);

					FillToTab(&TempLineText, cTab3);
					TempLineText = TempLineText +Pad( dtoa(TempImage->vRedBoxHeight[TempLoop], 0), 4, 0);

					FillToTab(&TempLineText, cTab4);
					TempLineText = TempLineText + WordToHex(TempImage->vRedBoxInspection[TempLoop]);

					TempString = TempString + "\n" + TempLineText;
				}
			}

			TempString = TempString + TempDividerString;
			TempString = TempString + "\n\n";

			TempString = TempString + "Processing This Image Time: " + dtoaWithCommas(TempImage->vProcessTime,1) + " microseconds\n";

			TempString = TempString + "Minimum Processing Image Time: " + dtoaWithCommas(vLocalSystemData->vImageProcessingTimeMinimum,1) + "\n";

			TempString = TempString + "Maximum Processing Image Time: " + dtoaWithCommas(vLocalSystemData->vImageProcessingTimeMaximum,1) + "\n";

			vLocalSystemData->vImageProcessingTimeStandardDeviation = CalculateStandardDeviation(&vLocalSystemData->vImageProcessingTimeData[0], 
				vLocalSystemData->vImageProcessingTimeSamples, &vLocalSystemData->vImageProcessingTimeAverage);

			TempString = TempString + "Average Processing Image Time: " + dtoaWithCommas(vLocalSystemData->vImageProcessingTimeAverage,1) + "\n";

			TempString = TempString + "Standard Deviations Processing Image Time: " + dtoaWithCommas(vLocalSystemData->vImageProcessingTimeStandardDeviation,1) + "\n";

			TempString = TempString + "\nMajor Inspections Time: " + dtoaWithCommas(TempImage->vFindTime,1) + " micro-seconds\n";

			for (WORD TempLoop = 0; TempLoop < vGlobalCurrentProduct->vNumberOfInspections; TempLoop++)
			if (vGlobalCurrentProduct->vInspection[TempLoop])
				TempString = TempString + "\tInspection: " + dtoa(TempLoop + 1,0) + "-" + vGlobalCurrentProduct->vInspection[TempLoop]->vName +
				"-Time: " + dtoaWithCommas(TempImage->vInspectonProcessingTime[TempLoop],1) + "\n";

			CString TempString1 = " ";
			TempString1 = dtoaWithCommas(vLocalSystemData->vITIPCDig->vLastDisplayTime,1);
			TempString = TempString + "Display Time: " + TempString1 + "\n";

			TempString1 = dtoaWithCommas(TempImage->vFindBlobsTime,1);
			TempString = TempString + "Blob Time: " + TempString1 + "\n";

			TempString1 = dtoaWithCommas(TempImage->vEdgeAdjustTime,1);
			TempString = TempString + "Edge Adjust Time: " + TempString1 + "\n";

			TempString1 = dtoaWithCommas(TempImage->vFindEdgesTime,1);
			TempString = TempString + "Find Edges Time: " + TempString1 + "\n";

			TempString1 = dtoaWithCommas(TempImage->vEnhanceTime,1);
			TempString = TempString + "Enhance Time: " + TempString1 + "\n";

			TempString1 = dtoaWithCommas(TempImage->vFindContaminantsTime,1);
			TempString = TempString + "Find Contaminant Time: " + TempString1 + "\n";

			TempString1 = dtoaWithCommas(TempImage->vGlassInspectionTime,1);
			TempString = TempString + "Glass Inspection Time: " + TempString1 + "\n";

			TempString1 = dtoaWithCommas(TempImage->vCurveFitTime,1);
			TempString = TempString + "Glass2C Time: " + TempString1 + "\n";

			TempString1 = dtoaWithCommas(TempImage->vSmoothTime,1);
			TempString = TempString + "Smooth Time: " + TempString1 + "\n";

			//TempString1 = dtoaWithCommas(TempImage->vFindGlassContaminantsTime,1);
			//TempString = TempString + "Find Glass Contaminant Time: " + TempString1 + "\n";

			TempString1 = dtoaWithCommas(TempImage->vFindMedianTime,1);
			TempString = TempString + "Find Smooth Time: " + TempString1 + "\n";

			TempString1 = dtoaWithCommas(TempImage->vFindVoidsTime,1);
			TempString = TempString + "Find Void Time: " + TempString1 + "\n";

			TempString1 = dtoaWithCommas(TempImage->vFindDensityTime,1);
			TempString = TempString + "Find Density Time: " + TempString1;

			TempString1 = dtoaWithCommas(TempImage->vCopyBufferTime,1);
			TempString = TempString + "\nCopy to Buffer Time: " + TempString1;

			TempString1 = dtoaWithCommas(TempImage->vCopyToMainTime,1);
			TempString = TempString + "\nCopy To Main Buffer Time: " + TempString1;

			TempString1 = dtoaWithCommas(vLocalSystemData->vITIPCDig->vLastScreenPaintTime,1);
			TempString = TempString + "\nScreen Paint Time: " + TempString1;

			TempString1 = dtoaWithCommas(TempImage->vReferencePositionTime,1);
			TempString = TempString + "\nReference Position Time: " + TempString1;

			TempString1 = dtoaWithCommas(vLocalSystemData->vFreeTimeMinimum,1);
			TempString = TempString + "\nMinimum Free Time: " + TempString1 + " milliseconds\n";

			TempString1 = dtoaWithCommas(vLocalSystemData->vFreeTimeMaximum,1);
			TempString = TempString + "Maximum Free Time: " + TempString1 + "\n";

			TempString1 = dtoaWithCommas(vLocalSystemData->vFreeTimeAverage,1);
			TempString = TempString + "Average Free Time: " + TempString1 + "\n";

			TempString1 = dtoaWithCommas(vLocalSystemData->vFreeTimeStandardDeviation,1);
			TempString = TempString + "Standard Deviation Free Image Time: " + TempString1;

			TempString = TempString + TempDividerString;

			if (!((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode)))
			{
			TempString = TempString + "\n\nTop Reference Adjust: " + dtoa(vGlobalCurrentProduct->vReferenceTop - TempImage->vTop, 0) + " pixels\n";
			TempString = TempString + "Top of Container: " + dtoa(TempImage->vTop, 0) + "\n";
			TempString = TempString + "Top Reference Line: " + dtoa(vGlobalCurrentProduct->vReferenceTop, 0) + "\n";

			TempString = TempString + "\nBottom Reference Adjust: " + dtoa(vGlobalCurrentProduct->vReferenceBottom - TempImage->vBottom, 0) + " pixels\n";
			TempString = TempString + "Bottom of " + vLocalSystemData->vContainerString + ": " + dtoa(TempImage->vBottom, 0) + "\n";
			TempString = TempString + "Bottom Reference Line: " + dtoa(vGlobalCurrentProduct->vReferenceBottom, 0) + "\n";

			TempString = TempString + "\nSide to Side Reference Adjust: " + dtoa(-TempImage->vLeftReferenceAdjust, 0);
			TempString = TempString + "\nLeft Reference Adjust: " + dtoa(vGlobalCurrentProduct->vReferenceLeft - TempImage->vLeft, 0);
			TempString = TempString + "\nLeft Side of Container: " + dtoa(TempImage->vLeft, 0);
			TempString = TempString + "\nLeft Reference Line: " + dtoa(vGlobalCurrentProduct->vReferenceLeft, 0);
			TempString = TempString + "\nRight Reference Adjust: " + dtoa(-TempImage->vRightReferenceAdjust, 0);
			TempString = TempString + "\nRight Side of Container: " + dtoa(TempImage->vRight, 0);
			TempString = TempString + "\nRight Reference Line:" + dtoa(vGlobalCurrentProduct->vReferenceRight);
			TempString = TempString + "\n\nReference Width: " + dtoa(vGlobalCurrentProduct->vReferenceWidth, 0);
			TempString = TempString + "\nImage Width:" + dtoa(TempImage->vRight - TempImage->vLeft, 0);
			TempString = TempString + "\nMinimum Container Width: " + dtoaWithCommas(vLocalSystemData->vITIPCDig->vContainerWidthMinimum,1);
			TempString = TempString + "\nMaximum Container Width: " + dtoaWithCommas(vLocalSystemData->vITIPCDig->vContainerWidthMaximum,1);
			TempString = TempString + "\nAverage Container Width: " + dtoaWithCommas(vLocalSystemData->vITIPCDig->vContainerWidthAverage,1);
			TempString = TempString + "\nStandard Deviations of Container Width: " + dtoaWithCommas(vLocalSystemData->vITIPCDig->vContainerWidthStandardDeviation,1);
			}
			TempString = TempString + "\n\nLast Trigger Width: " + dtoaWithCommas(vLocalSystemData->vLastBodyTriggerLength,1) + " pixels";
			TempString = TempString + "\nLast Trigger Gap Width: " + dtoaWithCommas(TempImage->vBodyTriggerGapLength,1) + " pixels";
			TempString = TempString + "Average Trigger Width: " + dtoaWithCommas(vLocalSystemData->vBodyTriggerWidthAverage,1);
			TempString = TempString + "\nStandard Deviations Trigger Width: " + dtoaWithCommas(vLocalSystemData->vBodyTriggerWidthStandardDeviation,1);
			TempString = TempString + TempDividerString;

			CalculateMinimumAndMaximumIntensity();

			TempString = TempString + "\n\nOriginal " + vLocalSystemData->vContainerString + " Intensity:" + dtoa(TempImage->vOriginalImageIntensity,2);
			TempString = TempString + "\nContainer Intensity Adjust:" + dtoa(TempImage->vImageIntensityAdjust,0);
			TempString = TempString + "\nAverage " + vLocalSystemData->vContainerString + " Intensity:" + dtoa(vGlobalCurrentProduct->vSetupHistoryAverageDensity,2);
			if (TempImage->vFinalImageIntensity == 0)
			if (vGlobalCurrentProduct->vDriftDensityInspection)
			{
				if ((vLocalConfigurationData->vScanTracType == cFermataScanTrac) || (vLocalConfigurationData->vScanTracType == cLegatoScanTrac)) 
				{//use algorithm to find brightness that can ignore bubbles by finding darkest value of each pixel
					TempImage->vFinalImageIntensity = vLocalSystemData->vITIPCDig->CalculatePipelineProductDensity(TempImage->vGreyImage, vGlobalCurrentProduct->vDriftDensityInspection);
					if ((TempImage->vFinalImageIntensity > 25) && (TempImage->vFinalImageIntensity < 230) && (vLocalSystemData->vSmallestDifferenceBetweenAverageAndMinimum < 40))
						TempImage->vFinalImageIntensity = TempImage->vFinalImageIntensity + vLocalSystemData->vSmallestDifferenceBetweenAverageAndMinimum;
				}
				else
				TempImage->vFinalImageIntensity = vLocalSystemData->vITIPCDig->CalculateInspectionDensityAfterReference(TempImage->vGreyImage, vGlobalCurrentProduct->vDriftDensityInspection,
					TempImage->vLeftReferenceAdjust, TempImage->vTopReferenceAdjust, vLocalConfigurationData->vReferenceROIsToEdges, TempImage->vLeft, TempImage->vRight); 
			}
			else
			{
				TempImage->vFinalImageIntensity = vLocalSystemData->vITIPCDig->CalculateFullDensity(TempImage->vGreyImage, vLocalSystemData->vITIPCDig->vOriginalBufferSize); 
			}
			TempString = TempString + "\nFinal " + vLocalSystemData->vContainerString + " Intensity:" + dtoa(TempImage->vFinalImageIntensity,2) + "\n";

				//TempString = TempString + "Threshold Compensation: " + 
				//	dtoa(TempImage->vThresholdCorrection,2) + "\n";
			//if (TempImage->vNegitiveThresholdCorrection)
			//	TempString = TempString + "Threshold Compensation: -" + 
			//		dtoa(TempImage->vNegitiveThresholdCorrection,2) + "\n";

			if (vInspectionShowing)
			if ((vInspectionShowing->vROIPixelTop) && 
				(vInspectionShowing->vROIPixelRight))
			{
				TempString = TempString + "\nCurrent Inspection: " + vInspectionShowing->vName + "\n";
				TempString = TempString + "ROI Bottom: " + 
					dtoa(vInspectionShowing->vROIPixelBottom,0) + "\n";
				TempString = TempString + "ROI Top: " + 
					dtoa(vInspectionShowing->vROIPixelTop,0) + "\n";
				TempString = TempString + "ROI Height: " + 
					dtoa(vInspectionShowing->vROIPixelTop - vInspectionShowing->vROIPixelBottom,0) + "\n";
				TempString = TempString + "ROI Left: " + 
					dtoa(vInspectionShowing->vROIPixelLeft,0) + "\n";
				TempString = TempString + "ROI Right: " + 
					dtoa(vInspectionShowing->vROIPixelRight,0) + "\n";
				TempString = TempString + "ROI Width: " + 
					dtoa(vInspectionShowing->vROIPixelRight - vInspectionShowing->vROIPixelLeft,0) + "\n";
			}

			TempString = TempString + TempDividerString;
			TempString = TempString + "\n\n\n\nMinimum Enhanced Intensity:" + dtoa(MinimumEnhanceROIIntensity, 0) + "\n";
			TempString = TempString + "Maximum Enhanced Intensity:" + dtoa(MaximumEnhanceROIIntensity, 0) + "\n";
			TempString = TempString + "Minimum Intensity:" + dtoa(MinimumROIIntensity, 0) + "\n";
			TempString = TempString + "Maximum Intensity:" + dtoa(MaximumROIIntensity, 0) + "\n";

			TempString = TempString + "\nGap before this " + vLocalSystemData->vContainerString + ": " + dtoa(TempImage->vBodyTriggerGapLength,0) + " pulses";
			TempString = TempString + "\n\n" + vGlobalTopString + "\n";

			TempString1 = "";
			if (vGlobalImageAquisitionThread)
			if (vHaveAnImageToShow)
			for (int TempLoop = vLocalConfigurationData->vNumberOfXRayDetectors; TempLoop > 0; TempLoop--)
			{
				TempString1 = TempString1 + "Detector: " + dtoa(TempLoop,0) + " = " + 
					dtoa(vGlobalImageAquisitionThread->CalculateDetectorDensity(TempImage->vGreyImage,(BYTE)TempLoop - 1, vLocalSystemData->vITIPCDig->vOriginalBufferSizeX),2) + "\n";
			}
			TempString = TempString + TempString1;
			TempString = TempString + vGlobalBottomString;

			//TempString = TempString + "Intensity Adjust:" + dtoa(vIntensityAdjust,0) + "\n";
	/*
			TempString = TempString + "Blur Times:" + dtoa(vShowBluredImage,0) + "\n";
			TempString = TempString + "Hat Size:" + dtoa(vShowTopHatImage,0);
	*/
		}
		CHelpDialog * IHelpDialog;
		IHelpDialog = new(CHelpDialog);
		IHelpDialog->vHelpContext = 16;
		IHelpDialog->vShowTable = true;
		IHelpDialog->vHelpType = 0;
		IHelpDialog->vWindowType = cTextWindow;
		//IHelpDialog->vLocalProductCollection = vLocalProductCollection;
		IHelpDialog->vLocalSystemData = vLocalSystemData;
		IHelpDialog->vMainWindowPointer = vMainWindowPointer;
		IHelpDialog->vLocalConfigurationData = vLocalConfigurationData;
		IHelpDialog->vProductPointer = vGlobalCurrentProduct;
		IHelpDialog->vTextString = TempString;
		TempText.LoadString(IDS_ImageInformation);
		IHelpDialog->vTitleString = TempText;
		IHelpDialog->vHistogramData = &vHistogramData[0];
		int TempResult = IHelpDialog->DoModal();
		delete IHelpDialog;
		if (TempResult == 10)
		{
			//Main Menu button pressed
			Sleep(1); //is the equivelent of a yeild statement;
			PrepareAndExit(10);
		}
	}
	else
		ShowNeedImageDialogMessage();
}

void CNewImageDisplayDialog::InitializeLocalLastRejectsBuffer(bool TempJustCreatedVariables)
{
	for (DWORD TempLoop = 0; TempLoop < (cMaxNumberOfLastRejectImages + 1) * cMaximumNumberOfCountsDisplayed; TempLoop++)
			// 8/14/2006
	if (vLocalLastNRejectImages[TempLoop])
	{
		if (vDisplayingImageData == vLocalLastNRejectImages[TempLoop])
			vDisplayingImageData = NULL;

		if (!TempJustCreatedVariables)
		{
			if (vLocalLastNRejectImages[TempLoop])
				delete vLocalLastNRejectImages[TempLoop];
		}
		vLocalLastNRejectImages[TempLoop] = NULL;
	}
}

void CNewImageDisplayDialog::DisplayThreshold()
{
	UpdateButtons();
	UpdateDisplay(vDisplayingImageData);
	WaitThenDrawImageOnScreen();
	ShowSensitivity();
}

void CNewImageDisplayDialog::PrepareAndExit(BYTE TempExitCode)
{
	if (vWindowInitialized)
	{
		if (vProcessedRedImage)
			free(vProcessedRedImage);
		vProcessedRedImage = NULL;

		if (vMouseState > 0)
		{
			ReportErrorMessage("Screen exited, repeat ended",cAction, 32000);
			KillTimer(vMouseDownRepeatTimerHandle);
			vMouseState = 0;
		}

		if (vBlobResults)
		{
			mvt_blob_delete_results(vBlobResults);
			vBlobResults = NULL;
		}

		if (vBlobParameters)
		{
			mvt_blob_delete_params(vBlobParameters);
			vBlobParameters = NULL;
		}

		//free the memory as not showing rejects anymore
		InitializeLocalLastRejectsBuffer(false);

		vLocalSystemData->vShowImages = true;
		if (vOnScreenImageData)
			delete vOnScreenImageData;
		vOnScreenImageData = NULL;

		if (vLocalSystemData->vITIPCDig->vImageBuffer)
			FillMemory(vLocalSystemData->vITIPCDig->vImageBuffer,
				vLocalSystemData->vITIPCDig->vDisplayImageSizeX * 
				vLocalSystemData->vITIPCDig->vDisplayImageSizeY, 255);

		if ((vLocalSystemData->vInAutoSetup) || (vFromSetupWindow))
		{
			//vLocalSystemData->vITIPCDig->vDisplayImageSizeX = 416;
			vLocalSystemData->vITIPCDig->vDisplayImageSizeY = (WORD) ResizeYCoor(338); //228 is autosetup window size
			//360 is regular image display;

			if (vLocalSystemData->vITIPCDig->vImage)
				delete vLocalSystemData->vITIPCDig->vImage;
			vLocalSystemData->vITIPCDig->vImage = NULL;

			vLocalSystemData->vITIPCDig->SetupImageBuffer(
				vLocalSystemData->vITIPCDig->vDisplayImageSizeX, 
				vLocalSystemData->vITIPCDig->vDisplayImageSizeY);
		}

		if (vImageBufferForDisplay)
		{
			delete vImageBufferForDisplay;
			vImageBufferForDisplay = NULL;
		}
		CDialog::EndDialog(true);
	}
}

void CNewImageDisplayDialog::ShowOrHideDisplays()
{
	if (vRejectsImageDisplay)
	{
		BYTE TempGoodIndex = 0;
		if (PasswordOK(cTemporaryInspxPassword,false))
		if (vLocalSystemData->vGoodImagesIndex)
		if (vLocalSystemData->vGoodImagesIndex - vFirstInspectionToView < 9)
			TempGoodIndex = vLocalSystemData->vGoodImagesIndex - vFirstInspectionToView;

		if (((vTotalNumberOfInspectionsToDisplay - vFirstInspectionToView) > 0) || (TempGoodIndex == 1))
			m_SubFunction1Display.ShowWindow(SW_SHOW);
		else
			m_SubFunction1Display.ShowWindow(SW_HIDE);
		if (((vTotalNumberOfInspectionsToDisplay - vFirstInspectionToView) > 1) || (TempGoodIndex == 2))
			m_SubFunction2Display.ShowWindow(SW_SHOW);
		else
			m_SubFunction2Display.ShowWindow(SW_HIDE);
		if (((vTotalNumberOfInspectionsToDisplay - vFirstInspectionToView) > 2) || (TempGoodIndex == 3))
			m_SubFunction3Display.ShowWindow(SW_SHOW);
		else
			m_SubFunction3Display.ShowWindow(SW_HIDE);
		if (((vTotalNumberOfInspectionsToDisplay - vFirstInspectionToView) > 3) || (TempGoodIndex == 4))
			m_SubFunction4Display.ShowWindow(SW_SHOW);
		else
			m_SubFunction4Display.ShowWindow(SW_HIDE);
		if (((vTotalNumberOfInspectionsToDisplay - vFirstInspectionToView) > 4) || (TempGoodIndex == 5))
			m_SubFunction5Display.ShowWindow(SW_SHOW);
		else
			m_SubFunction5Display.ShowWindow(SW_HIDE);
		if (((vTotalNumberOfInspectionsToDisplay - vFirstInspectionToView) > 5) || (TempGoodIndex == 6))
			m_SubFunction6Display.ShowWindow(SW_SHOW);
		else
			m_SubFunction6Display.ShowWindow(SW_HIDE);
		if ((((vTotalNumberOfInspectionsToDisplay - vFirstInspectionToView) > 6) || 
			(TempGoodIndex == 7)) &&
			(!HaveAnInspection(8)))
			m_SubFunction7Display.ShowWindow(SW_SHOW);
		else
			m_SubFunction7Display.ShowWindow(SW_HIDE);
		//if ((((vTotalNumberOfInspectionsToDisplay - vFirstInspectionToView) > 7) || 
		//	(TempGoodIndex == 8)) &&
		//	(!HaveAnInspection(9)))
	//	m_SubFunction8Display.ShowWindow(SW_SHOW);
		//else
		//	m_SubFunction8Display.ShowWindow(SW_HIDE);
		OneSecondUpdate();
	}
}

void CNewImageDisplayDialog::SetROILines()
{
	if (vInspectionShowing)
	{
		vChangeMade = true;
		vInspectionShowing->SetROITop(vInspectionShowing->vROIPixelTop / vGlobalPixelsPerUnitInHeight);
		vInspectionShowing->SetROIBottom(vInspectionShowing->vROIPixelBottom / vGlobalPixelsPerUnitInHeight);
		vInspectionShowing->SetROILeft(vInspectionShowing->vROIPixelLeft / (vGlobalPixelsPerUnit * vGlobalCurrentProduct->vOverScanMultiplier), vGlobalCurrentProduct->vOverScanMultiplier);
		vInspectionShowing->SetROIRight(vInspectionShowing->vROIPixelRight / (vGlobalPixelsPerUnit * vGlobalCurrentProduct->vOverScanMultiplier), vGlobalCurrentProduct->vOverScanMultiplier);

		CWnd *TempWindow = CWnd::GetDlgItem(IDC_Function2Button);
		if (TempWindow)
			TempWindow->Invalidate(false);
		TempWindow = CWnd::GetDlgItem(IDC_Function4Button);
		if (TempWindow)
			TempWindow->Invalidate(false);
		WaitThenDrawImageOnScreen();
		ShowLearnSensitivity();
	}
}

void CNewImageDisplayDialog::RestoreOriginalROIValues()
{
	if (vInspectionShowing)
	{
		vChangeMade = false;
		vInspectionShowing->SetROITop(vOriginalROITop);
		vInspectionShowing->SetROIBottom(vOriginalROIBottom);
		vInspectionShowing->SetROILeft(vOriginalROILeft, vGlobalCurrentProduct->vOverScanMultiplier);
		vInspectionShowing->SetROIRight(vOriginalROIRight, vGlobalCurrentProduct->vOverScanMultiplier);
		vInspectionShowing->vRemoveJarPunt = vOriginalRemoveJarPunt;
	}
}

void CNewImageDisplayDialog::SaveOriginalROIValues()
{
	if (vInspectionShowing)
	{
		vChangeMade = false;
		//don't have to save, as changes were made to original, just remove changed indicator
		vOriginalROIRight = vInspectionShowing->vROIRight;
		vOriginalROILeft = vInspectionShowing->vROILeft;
		vOriginalROITop = vInspectionShowing->vROITop;
		vOriginalROIBottom = vInspectionShowing->vROIBottom;
		vOriginalRemoveJarPunt = vInspectionShowing->vRemoveJarPunt;
	}
}
void CNewImageDisplayDialog::GetBackgroundImage(BYTE *TempBufferPointer)
{
	CString TempString1 = "=Process 0";
	TempString1 =  *vGlobalCurrentProduct->GetProductName() + TempString1;
	if (!vLocalSystemData->vITIPCDig->vUsingLocalSetupDataFile)
		TempString1 = cSetupDataDirectory + TempString1;
	else
		TempString1 = vGlobalCurrentDirectory + TempString1;
	TempString1 = TempString1 + ".BMP";
	LPTSTR TempString = TempString1.GetBuffer(TempString1.GetLength());
	int TempHeight = 0;
	int TempWidth = 0;
	int TempBytesPerPixel = 0;
	int TempType = 0;

	CFileStatus TempFileStatus;

	if (TempBufferPointer)
	if (CFile::GetStatus(TempString, TempFileStatus))
	{
		CW2A TempFileNameString(TempString);
		Erflag TempError = im_file_get_size(TempFileNameString, 
				IM_BMP_FILE, &TempHeight, &TempWidth, 
				&TempBytesPerPixel, &TempType);
		if (TempError == IM_OK)
		if (((TempWidth != vLocalSystemData->vITIPCDig->vOriginalBufferSizeY) ||
			(TempHeight != vLocalSystemData->vITIPCDig->vOriginalBufferSizeX)) &&
			(!vLocalConfigurationData->vDemoMode))
		{
			ReportErrorMessage("Error-Process 0 Learned Image wrong size, ignored", cWriteToLog, 0);
		}
		else
		if (ThereIsEnoughMemory(vLocalSystemData->vITIPCDig->vOriginalBufferSize + sizeof(Image), "Temp Image In"))
		{
			Image *TempImageIn = NULL;
			TempImageIn = im_create(IM_BYTE, vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY);
			if ((TempImageIn) && (TempImageIn->pix))
			{
				TempError = im_file_read(TempImageIn, TempFileNameString, IM_AUTO_FILE);
				if (TempError == IM_OK)
				{
					CopyMemory(TempBufferPointer,TempImageIn->pix, vLocalSystemData->vITIPCDig->vOriginalBufferSize);
				}
			}
			if (TempImageIn)
				im_delete(TempImageIn);
		}
	}
}


void CNewImageDisplayDialog::OnDialogTitleStaticText() 
{
	//if (vGlobalUnderRemoteControl)
	//	vGlobalUnderRemoteControl = 0;
	//else
	//	vGlobalUnderRemoteControl = 600;
}

void CNewImageDisplayDialog::ShowNeedImageDialogMessage() 
{
	CNoticeDialog TempNoticeDialog;
	CString TempText("");
	TempText.LoadString(IDS_ThisFunctionrequiresanimageonthescreen);
	TempNoticeDialog.vNoticeText = "\n\n\nThis Function requires an image on the screen";
	if ((vHaveAnImageToShow) && (vAllRejectsHidden))
		TempNoticeDialog.vNoticeText = "\n\n\nAll Images now hidden";
	TempNoticeDialog.vType = cInformationMessage;
	TempNoticeDialog.DoModal();
}

void CNewImageDisplayDialog::ShowNoMouseClickDialogMessage() 
{
	CNoticeDialog TempNoticeDialog;
	CString TempText("");
	TempText = ("Mouse Click Not Available");
	TempNoticeDialog.vNoticeText = "\n\n\nMouse Click Not Available";
	TempNoticeDialog.vType = cInformationMessage;
	TempNoticeDialog.DoModal();
}

//void CNewImageDisplayDialog::OnMouseMove(UINT nFlags, CPoint point) 
//{
//	// TODO: Add your message handler code here and/or call default
//	vGlobalMouseMoveCounter++;
//	if (vGlobalMouseMoveCounter > 5)
//	{
//		vGlobalMouseMoveCounter = 0;
//		if (vGlobalPasswordTimeOut)
//			vGlobalPasswordTimeOut = 600;
//		if (vGlobalUnderRemoteControl)
//			vGlobalUnderRemoteControl = 600;
//	}
//	CDialog::OnMouseMove(nFlags, point);
//}

void CNewImageDisplayDialog::ClearAllDisplayOptions(bool TempClearSteps)
{
	if (TempClearSteps)
	{
		vShowingProcess = 0;
	}
	vTweakingThreshold = false;
	vDisplayEnhancedImage = false;
	vPreSmoothImage = 0;
	vRemoveJarPunt = 0;
	vTrimEdgesOfJarAtROI = 0;
	vFillInLightSpots = false;
	vShowSubtractAverage = false;
	vShowAddStructure = false;
	vShowThreshold = false;
	vShowAddHistogram = false;
	//vShowImageFactors = false;
	vShowReferenceAdjustedImage = 0;
	vShowLineDerivative = 0;
	vLookForDarkSpots = 0;
	vTakeDerivative = 0;
	vFindSpotInSubROI = 0;
	vDoContaminant22Algorithm = 0;
	vTakeDifferenceFromMedian = 0;
	vTakeDifferenceFromMedianFilter = 0;
	vBonesInspection = 0;
	vWhiteOutAnythingTouchingTheTop = 0;
	vFillBackground = 0;
	vUseVariabiltyImage = 0;
	vFindDarkSpotOnLine = 0;
	vShowThreshold = false;
	vRequire2InARow = 0;
	vShowDilationCount = 0;
	vShowErodeCount = 0;
	vShowErodeCountAtStart = 0;
	vShowDilationCountAtEnd = 0;
	vShowErodeCountAtEnd = 0;
	vShowBluredImage = 0;
	vIPContaminant = 0;
	vShowTopHatImage = 0;
	vIntensityAdjust = 0;
	vShowClipboard = false;
	//vPreSelectDarkSpotsInImage = 0;

	vShowOriginalImageFactor = 0;
	vShowEnhancedImageFactor = 0;
	vShowAverage = false;
	vShowHistogram = false;
	vShowStructure = false;
	//vShowGlassStructure = false;
	//vShowGlassHistogram = false;
	vShowType = cShowTypeContaminant;

	vShowImproveLearnPreview = 0;
	vShowOriginalStructure = false;
	vShowDeltaOriginalToCurrentStructure = 0;
	vShowDeltaCurrentPreviewStructure = 0;
}

void CNewImageDisplayDialog::PlotLine(WORD TempCode)
{
	int TempSideReferenceAmount = 0;

	CImageData *TempImage = vDisplayingImageData;
	if ((PasswordOK(cTemporaryInspxPassword,false)) && (vOnScreenImageData))
	{
		TempImage = vOnScreenImageData;
	}
	if (!vShowReferenceAdjustedImage)
		TempSideReferenceAmount = vDisplayingImageData->vLeftReferenceAdjust;

	if ((TempImage) && (TempImage->vGreyImage))
	{
		int nResponse = TempCode;
		while (nResponse)
		{
			switch (nResponse)
			{
				case 38: //arrow up
				case c2:
					if (vInspectionShowing)
					{
						if (vPlotLine < vInspectionShowing->vROIPixelTop - 1)
							vPlotLine++;
						else
							vPlotLine = vInspectionShowing->vROIPixelBottom;
					}
					else
					if (vPlotLine < vLocalSystemData->vITIPCDig->vOriginalBufferSizeX - 2)
						vPlotLine++;
					else
						vPlotLine = 0;
				break;
				case 40: //arrow down
				case c8:
					if (vInspectionShowing)
					{
						if (vPlotLine > vInspectionShowing->vROIPixelBottom)
							vPlotLine--;
						else
							vPlotLine = vInspectionShowing->vROIPixelTop;
					}
					else
					if (vPlotLine)
						vPlotLine--;
					else
						vPlotLine = vLocalSystemData->vITIPCDig->vOriginalBufferSizeX - 10;
				break;
			}

			//if (vPlotLine)
			{
				int TempNumberOfPixels = vLocalSystemData->vITIPCDig->vOriginalBufferSizeX - 4;
				if (vInspectionShowing)
					TempNumberOfPixels = vInspectionShowing->vROIPixelRight - vInspectionShowing->vROIPixelLeft;
				if (TempNumberOfPixels > 1024)
					TempNumberOfPixels = 1024;
				double TempData[1024];
				BYTE *TempImageMaximum = TempImage->vGreyImage + vLocalSystemData->vITIPCDig->vOriginalBufferSize;
				BYTE *TempSourcePointer = TempImage->vGreyImage + vPlotLine + ((2) * vLocalSystemData->vITIPCDig->vOriginalBufferSizeX);
				if (vInspectionShowing)
					TempSourcePointer = TempImage->vGreyImage + vPlotLine + ((vInspectionShowing->vROIPixelLeft + TempSideReferenceAmount) * vLocalSystemData->vITIPCDig->vOriginalBufferSizeX);
				for (WORD TempLoop = 0; TempLoop < TempNumberOfPixels; TempLoop++)
				if ((TempSourcePointer >= TempImage->vGreyImage) && (TempSourcePointer < TempImageMaximum))
				{
					TempData[TempLoop] = *TempSourcePointer;
					TempSourcePointer = TempSourcePointer + vLocalSystemData->vITIPCDig->vOriginalBufferSizeX;
				}
				else
					TempData[TempLoop] = 0;

				CHistogramDisplayDialog IHistogramDisplayDialog;
				IHistogramDisplayDialog.vTopDataLine = "Image Pixel Plot";
				IHistogramDisplayDialog.vNumberOfNumbers = TempNumberOfPixels;
				IHistogramDisplayDialog.vDataTitle = "Data Values in ROI, Line: " + dtoa(vPlotLine,0);
				IHistogramDisplayDialog.vValuesAreFloats = true;
				IHistogramDisplayDialog.vPointerToFloats = &TempData[0];
				IHistogramDisplayDialog.vLocalSystemData = vLocalSystemData;
				IHistogramDisplayDialog.vLocalConfigurationData = vLocalConfigurationData;
				IHistogramDisplayDialog.vMainWindowPointer = vMainWindowPointer;
				IHistogramDisplayDialog.vPlotRawData = true;
				nResponse = IHistogramDisplayDialog.DoModal();
				//if up or down arrow pressed then leave response alone so goes back into plot the next line
				if ((nResponse != 38) && (nResponse != 40) && (nResponse != c2) && (nResponse != c8))
				{
					//if pressed right or left arrow to exit, leave the line number last ploted to enter there next time
					if ((nResponse != 37) && (nResponse != 39) && (nResponse != c4) && (nResponse != c6))
						vPlotLine = 0;  //clear line number were at so start at top or bottom next time
					nResponse = 0;
				}
				//else 
				//if (nResponse == 10)
				//{
					//Main Menu button pressed
				//	Sleep(1); //is the equivelent of a yeild statement;
				//	PrepareAndExit(10);
				//}
			}
		}
		//if (vPlotLine)
			WaitThenDrawImageOnScreen();
	}
}
/*
void CNewImageDisplayDialog::OutputImageROIToCSVFile(CString TempImageFileName)
{
	if ((vDisplayingImageData) && (vDisplayingImageData->vGreyImage))
	if (vInspectionShowing)
	{
		TempImageFileName.Delete(TempImageFileName.GetLength() - 4, 4);
		TempImageFileName = TempImageFileName + ".csv";
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
					{
						CString TempString("");
						TempString = "";
						BYTE *TempImageBuffer;

						for (WORD TempLineLoop = vInspectionShowing->vROIPixelTop; 
							TempLineLoop >= vInspectionShowing->vROIPixelBottom; TempLineLoop--) 
						{
							TempString = "";
							TempImageBuffer = vDisplayingImageData->vGreyImage + TempLineLoop +
								((vInspectionShowing->vROIPixelLeft + 
								vDisplayingImageData->vLeftReferenceAdjust) * 
								vLocalSystemData->vITIPCDig->vOriginalBufferSizeX);

							for (WORD TempPixelLoop = vInspectionShowing->vROIPixelLeft; 
								TempPixelLoop < vInspectionShowing->vROIPixelRight; TempPixelLoop++) 
							{
								TempString = TempString + dtoa(*TempImageBuffer,0) + ',';
								TempImageBuffer = TempImageBuffer + 
									vLocalSystemData->vITIPCDig->vOriginalBufferSizeX;
							}
							TempString = TempString + "\n ";
							vDataFile->Write( 
								TempString.GetBuffer(TempString.GetLength()), TempString.GetLength() );
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
*/
void CNewImageDisplayDialog::ReadInOriginalImageFile()
{
	CString TempImageNameString = "=Process 2o";
	if (vShowType == cShowTypeContaminant2)
		TempImageNameString = "=Process 2To";

	if (vGlobalCurrentProduct)
	if (vLocalSystemData->vITIPCDig->HaveLearnImageFile(TempImageNameString))
	{
		if (!vImageBufferForDisplay)
		{
			if (ThereIsEnoughMemory(sizeof(CImageData), "vImageBufferForDisplay"))
				vImageBufferForDisplay = new CImageData;
			if (vImageBufferForDisplay)
			{
				//fill buffer with 128 because are finding smallest value below 128
				vImageBufferForDisplay->vImageType = cDataImageType;
				vImageBufferForDisplay->MakeGreyImage(vLocalSystemData->vITIPCDig->vOriginalBufferSize);

				if (vImageBufferForDisplay->vGreyImage)
					FillMemory(vImageBufferForDisplay->vGreyImage, vLocalSystemData->vITIPCDig->vOriginalBufferSize,0x80);
			}
		}
		//read original structure in and put in buffer for display
		CString TempString1 =  *vGlobalCurrentProduct->GetProductName() + TempImageNameString;
		if (!vLocalSystemData->vITIPCDig->vUsingLocalSetupDataFile)
			TempString1 = cSetupDataDirectory + TempString1;
		else
			TempString1 = vGlobalCurrentDirectory + TempString1;
		TempString1 = TempString1 + ".BMP";
		LPTSTR TempString = TempString1.GetBuffer(TempString1.GetLength());
		int TempHeight = 0;
		int TempWidth = 0;
		int TempBytesPerPixel = 0;
		int TempType = 0;

		CFileStatus TempFileStatus;
		if ((vImageBufferForDisplay) && (vImageBufferForDisplay->vGreyImage))
		if (CFile::GetStatus(TempString, TempFileStatus))
		{
			CW2A TempFileNameString(TempString);
			Erflag TempError = im_file_get_size(TempFileNameString, 
					IM_BMP_FILE, &TempHeight, &TempWidth, 
					&TempBytesPerPixel, &TempType);
			if (TempError == IM_OK)
			if ((TempWidth != vLocalSystemData->vITIPCDig->vOriginalBufferSizeY) ||
				(TempHeight != vLocalSystemData->vITIPCDig->vOriginalBufferSizeX))
			{
				ReportErrorMessage("Error-Process 2 Learned Image wrong size, ignored", cWriteToLog,0);
			}
			else
			if (ThereIsEnoughMemory(vLocalSystemData->vITIPCDig->vOriginalBufferSize + 
				sizeof(Image), "Temp Image"))
			{
				Image *TempImageIn = NULL;
				TempImageIn = im_create(IM_BYTE, vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, 
					vLocalSystemData->vITIPCDig->vOriginalBufferSizeY);
				if ((TempImageIn) && (TempImageIn->pix))
				{
					CW2A TempFileNameString(TempString);
					TempError = im_file_read(TempImageIn, TempFileNameString, IM_AUTO_FILE);
					if (TempError == IM_OK)
					{
						vImageBufferForDisplay->vImageTypeString = "Original Structure";
						if (vShowType == cShowTypeContaminant2)
							vImageBufferForDisplay->vImageTypeString = "Original Structure2";

						CopyMemory(vImageBufferForDisplay->vGreyImage, TempImageIn->pix, vLocalSystemData->vITIPCDig->vOriginalBufferSize);
					}
				}
				if (TempImageIn)
					im_delete(TempImageIn);
				TempImageIn = NULL;
			}
		}
	}
}


void CNewImageDisplayDialog::CalculateImageDataInformation()
{
	if (!vShowAlternateImage)
	if ((vDisplayingImageData) && (vDisplayingImageData->vGreyImage))
	if ((vLocalSystemData->vITIPCDig->vNIDDScratchImage) && (vLocalSystemData->vITIPCDig->vNIDDScratchImage->pix))
	if ((vInspectionShowing) && (vInspectionShowing->vInspectionType == cFloodedInspection))
	{
		WORD TempDelta = 0;
		WORD TempBottom = 0;
		CopyMemory(vLocalSystemData->vITIPCDig->vNIDDScratchImage->pix, vDisplayingImageData->vGreyImage, vLocalSystemData->vITIPCDig->vOriginalBufferSize);

		//reference IV Bag
		if (vDisplayingImageData->vLeftReferenceAdjust != 0)
				vLocalSystemData->vITIPCDig->MakeReferenceImageAdjustment(vLocalSystemData->vITIPCDig->vNIDDScratchImage->pix, vLocalSystemData->vITIPCDig->vNIDDScratchImage2->pix, 0, //TempTopReferenceAdjust,
					vDisplayingImageData->vLeftReferenceAdjust);

		vInspectionShowing->IsIVBagFlooded(vLocalSystemData->vITIPCDig->vNIDDScratchImage, &TempDelta, &TempBottom,
			vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY);

		vNumberOfContaminants = TempDelta; //save to show spout lenght in Rejects Display
	}
	else
	if ((vInspectionShowing) && ((vInspectionShowing->vInspectionType == cContaminantInspection) ||
		(vInspectionShowing->vInspectionType == cIPContaminantInspection) ||
		(vInspectionShowing->vInspectionType == cWireContaminantInspection) ||
		(vInspectionShowing->vInspectionType == cSpotInROIContaminantInspection) ||
		(vInspectionShowing->vInspectionType == cVoidInspection) ||
		(vInspectionShowing->vInspectionType == cItemCountInspection) || (vInspectionShowing->vInspectionType == cSizeInspection) ||  //treat size inspections like contaminant for finding size of items like blobs
		(vInspectionShowing->vInspectionType == cMissingLidInspectionOnSize) ||//treat size inspections like contaminant for finding size of items like blobs
		(vInspectionShowing->vInspectionType == cKernelInspection) ||
		(vInspectionShowing->vInspectionType == cThresholdContaminantInspection) ||
		(vInspectionShowing->vInspectionType == cBadHeartInspection) ||
		(vInspectionShowing->vInspectionType == cDripBySpoutInspection) ||
		(vInspectionShowing->vInspectionType == cDripInspection) ||
		(vInspectionShowing->vInspectionType == cFloodedInspection) ||
		(vInspectionShowing->vInspectionType == cContaminant2Inspection) ||
		(vInspectionShowing->vInspectionType == cTopContaminantInspection) ||
		(vInspectionShowing->vInspectionType == cAdvancedContaminant2Inspection) ||
		(vInspectionShowing->vInspectionType == cGlass13mmContaminantInspection) ||
		(vInspectionShowing->vInspectionType == cContaminantBonesInspection) ||
		(vInspectionShowing->vInspectionType == cContaminant22Inspection) ||
		(vInspectionShowing->vInspectionType == cBottomContaminantInspection)))
	{
		if (ThereIsEnoughMemory((vLocalSystemData->vITIPCDig->vOriginalBufferSize + sizeof(Image)) * 2 +	sizeof(CImageData), "Temp Image In and Out"))
		{
			CImageData *TempDisplayImageData = NULL;
			Image *TempImageIn = NULL;
			TempImageIn = im_create(IM_BYTE, vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY);
			Image *TempImageOut = NULL;
			TempImageOut = im_create(IM_BYTE, vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY);

			if ((TempImageIn) && (TempImageOut) && (TempImageIn->pix) && (TempImageOut->pix))
			{
				CopyMemory(TempImageIn->pix, vDisplayingImageData->vGreyImage, vLocalSystemData->vITIPCDig->vOriginalBufferSize);

				if ((vIntensityAdjust > 1) || (vIntensityAdjust < -1))
				{
					int TempResult = 0;
					BYTE *TempBuffer = TempImageIn->pix; 
					for (DWORD TempLoop = 0; TempLoop < vLocalSystemData->vITIPCDig->vOriginalBufferSize; TempLoop++)
					{
						if ((*TempBuffer != 255) && (*TempBuffer != 0))
						{
							TempResult = *TempBuffer + vIntensityAdjust;
							if (TempResult > 254)
								*TempBuffer = 255;
							else
							if (TempResult < 1) 
								*TempBuffer = 0;
							else
								*TempBuffer = TempResult;
						}
						TempBuffer++;
					}
				}

				int TempTopReferenceAdjust = 0;
				int TempLeftReferenceAdjust = 0;
				if (vInspectionShowing)
				{
					if (vInspectionShowing->vReferenceToTop)
						TempTopReferenceAdjust = vDisplayingImageData->vTopReferenceAdjust + vLocalSystemData->vTweakTopReferenceAdjust;
					else
					if (vInspectionShowing->vReferenceToBottom)
						TempTopReferenceAdjust = vDisplayingImageData->vTopReferenceAdjust + vLocalSystemData->vTweakTopReferenceAdjust;
					else
						TempTopReferenceAdjust = 0;
					TempLeftReferenceAdjust = vDisplayingImageData->vLeftReferenceAdjust + vLocalSystemData->vTweakReferenceAdjust;
				}

				if ((vInspectionShowing->vROIPixelLeft == 0) && (vInspectionShowing->vROIPixelRight == vLocalSystemData->vITIPCDig->vOriginalBufferSizeY))
				{
					TempLeftReferenceAdjust = 0;
					TempTopReferenceAdjust = 0;
				}

				//if (vShowReferenceAdjustedImage)
				if ((TempTopReferenceAdjust != 0) || (TempLeftReferenceAdjust != 0))
				//if (!vShowAlternateImage)
				if ((vLocalSystemData->vITIPCDig->vNIDDScratchImage) && (vLocalSystemData->vITIPCDig->vNIDDScratchImage->pix))
						vLocalSystemData->vITIPCDig->MakeReferenceImageAdjustment(
							TempImageIn->pix, vLocalSystemData->vITIPCDig->vNIDDScratchImage->pix,
							TempTopReferenceAdjust,
							TempLeftReferenceAdjust);

				//12/1/2005
				CopyMemory(
					TempImageOut->pix,
					TempImageIn->pix,
					vLocalSystemData->vITIPCDig->vOriginalBufferSize);

				if (vInspectionShowing)
				{
					if (vWhiteOutAnythingTouchingTheTop)
						vInspectionShowing->WhiteOutAnythingTouchingTheTop(TempImageIn->pix, vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY);

					if (vInspectionShowing->vFillBackground)
						vInspectionShowing->FillBackground(TempImageIn->pix, vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY);

					if (vInspectionShowing->vFillInLightSpots)
					{
						vInspectionShowing->FillInLightSpots(TempImageIn->pix, vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY, vInspectionShowing->vFillInLightSpots);
					}

					//if (vInspectionShowing->vFillInLightSpots)
					//	vInspectionShowing->CurveFitFillInLightSpots(TempImageIn->pix, vLocalSystemData->vITIPCDig->vDisplayBufferHeightInPixels, vLocalSystemData->vITIPCDig->vDisplayBufferWidthInPixels);

					if (vInspectionShowing->vPreSmoothImage)
					{
						WORD TempBottom = 0;
						WORD TempLeft = 0;
						WORD TempHeight = 0;
						WORD TempWidth = 0;

						BYTE TempAutoImproveState = vDisplayingImageData->vContainerLearnState;
						if (vInspectionShowing)
						if (!vInspectionShowing->vAddStructure)
							TempAutoImproveState = 0;
						else
						if ((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode) ||
							(vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vLocalConfigurationData->vScanTracType == cAllegroScanTrac) ||
							(vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac)) //Continuous Feed, or a Pipeline or TD
							TempAutoImproveState = 0;
						else
						if (!vLocalConfigurationData->vAutoImproveEnabled)
							TempAutoImproveState = 0;

						vLocalSystemData->vITIPCDig->GetInspectionROIBounds(vInspectionShowing, 
							&TempBottom, &TempLeft, &TempHeight, &TempWidth, vDisplayingImageData->vLeftReferenceAdjust, 
							vLocalConfigurationData->vReferenceROIsToEdges, TempAutoImproveState);

						vLocalSystemData->vITIPCDig->MedianSmoothImage(TempImageIn, vInspectionShowing->vPreSmoothImage, TempLeft, TempWidth, TempBottom, TempHeight);
					}

					//erode image if should
					if (vInspectionShowing->vErodeCount)
					for (BYTE TempLoop = 0; TempLoop < vInspectionShowing->vErodeCount; TempLoop++)
						vInspectionShowing->ErodeImage(TempImageIn, vLocalSystemData->vITIPCDig->vNIDDScratchImage);

					//dilate image if should
					if (vInspectionShowing->vDilationCount)
					{
						for (BYTE TempLoop = 0; TempLoop < vInspectionShowing->vDilationCount; TempLoop++)
							vInspectionShowing->DilateImage(TempImageIn, vLocalSystemData->vITIPCDig->vNIDDScratchImage);

						if (vInspectionShowing->vErodeCountAtStart)
						for (BYTE TempLoop = 0; TempLoop < vInspectionShowing->vErodeCountAtStart; TempLoop++)
							vInspectionShowing->ErodeImage(TempImageIn, vLocalSystemData->vITIPCDig->vNIDDScratchImage);
					}

					if (vInspectionShowing->vRemoveJarPunt)
					{
						vInspectionShowing->RemoveJarPunt(TempImageIn->pix,
							vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY,
							vDisplayingImageData->vLeft - vDisplayingImageData->vLeftReferenceAdjust,
							vDisplayingImageData->vRight - vDisplayingImageData->vLeftReferenceAdjust, 
							vGlobalCurrentProduct->vSetupHistoryAverageDensity, 0);
					}

					if (vInspectionShowing->vTrimEdgesOfJar)
					{
						vInspectionShowing->TrimEdgesOfJarAtROI(TempImageIn->pix,
							vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY, true,
							vDisplayingImageData->vLeft - vDisplayingImageData->vLeftReferenceAdjust,
							vDisplayingImageData->vRight - vDisplayingImageData->vLeftReferenceAdjust);
					}

/*
					if (vInspectionShowing->vPreSelectDarkSpotsInImage)
					{
						int TempKernel[9];
						for (BYTE TempLoop = 0; TempLoop < 9; TempLoop++)
							TempKernel[TempLoop] = 1;

						Erflag TempError = mvt_bottom_hat(TempImageIn,vInspectionShowing->vPreSelectDarkSpotsInImage,
							vInspectionShowing->vPreSelectDarkSpotsInImage,
							&TempKernel[0], EDGE_IGNORE);
					}
					*/

					if (vInspectionShowing->vPerformGlassAlgorithm == 1)
					{
						vInspectionShowing->ClipToCurveFit(TempImageIn->pix,
							vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY);
					}

					if (vInspectionShowing->vTakeDerivative)
					if ((vLocalSystemData->vITIPCDig->vNIDDScratchImage) && (vLocalSystemData->vITIPCDig->vNIDDScratchImage->pix))
						vInspectionShowing->TakeLineDerivative(TempImageIn->pix, vLocalSystemData->vITIPCDig->vNIDDScratchImage->pix,
							vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY, vGlobalCurrentProduct->vSetupHistoryAverageDensity, (BYTE)vThreshold);

					if (vInspectionShowing->vBonesInspection)
					{
						WORD TempBottom = 0;
						WORD TempLeft = 0;
						WORD TempHeight = 0;
						WORD TempWidth = 0;
						BYTE TempAutoImproveState = 0;

						vLocalSystemData->vITIPCDig->GetInspectionROIBounds(vInspectionShowing, &TempBottom, &TempLeft, &TempHeight, &TempWidth, 
							vDisplayingImageData->vLeftReferenceAdjust, vLocalConfigurationData->vReferenceROIsToEdges, TempAutoImproveState);

						vInspectionShowing->FindBonesInspection(TempImageIn->pix, vLocalSystemData->vITIPCDig->vNIDDScratchImage,
							vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY, 
							TempLeft, TempLeft + TempWidth,  TempBottom, TempBottom + TempHeight);
					}

					if (vInspectionShowing->vTakeDifferenceFromMedianFilter)
					{
						CopyMemory(vLocalSystemData->vITIPCDig->vNIDDScratchImage->pix, TempImageIn->pix, vLocalSystemData->vITIPCDig->vOriginalBufferSize);

						BYTE TempMedian = vInspectionShowing->vTakeDifferenceFromMedianFilter & 0x3F;
						DoMedianSmooth(vLocalSystemData->vITIPCDig->vNIDDScratchImage, TempMedian, TempMedian);

						if (vTakeDifferenceFromMedianFilter < 128)
						{
#ifdef UseIFCDLLs
							mvt_subtract(vLocalSystemData->vITIPCDig->vNIDDScratchImage, TempImageIn);
							mvt_invert(TempImageIn);
#endif
						}
						else
							CopyMemory(TempImageIn->pix, vLocalSystemData->vITIPCDig->vNIDDScratchImage->pix, vLocalSystemData->vITIPCDig->vOriginalBufferSize);
					}

					if (vInspectionShowing->vTakeDifferenceFromMedian)
					{
						vInspectionShowing->TakeDifferenceFromMedian(TempImageIn->pix, vLocalSystemData->vITIPCDig->vNIDDScratchImage,
							vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY, vDisplayingImageData->vGreyImage, (BYTE)vThreshold);
					}

					if (vInspectionShowing->vFindDarkSpotOnLine)
					if ((vLocalSystemData->vITIPCDig->vNIDDScratchImage) && (vLocalSystemData->vITIPCDig->vNIDDScratchImage->pix))
						vInspectionShowing->FindDarkSpotOnLine(TempImageIn->pix, vLocalSystemData->vITIPCDig->vNIDDScratchImage->pix,
						vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY, 
						(BYTE)vThreshold);

					if (vInspectionShowing->vFindSpotInSubROI)
					{
						WORD TempBottom = 0;
						WORD TempLeft = 0;
						WORD TempHeight = 0;
						WORD TempWidth = 0;
						BYTE TempAutoImproveState = 0;

						vLocalSystemData->vITIPCDig->GetInspectionROIBounds(vInspectionShowing, &TempBottom, &TempLeft, &TempHeight, &TempWidth, 
							vDisplayingImageData->vLeftReferenceAdjust, vLocalConfigurationData->vReferenceROIsToEdges, TempAutoImproveState);

						vInspectionShowing->FindSpotInSubROI(TempImageIn->pix, vLocalSystemData->vITIPCDig->vNIDDScratchImage, vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, 
							vLocalSystemData->vITIPCDig->vOriginalBufferSizeY, (BYTE)vThreshold, vLocalSystemData->vITIPCDig->vDesensitizeImage, false, TempBottom, TempBottom + TempHeight, TempLeft, TempLeft + TempWidth);
					}

					if (vInspectionShowing->vPerformGlassAlgorithm == 2)
					if ((vLocalSystemData->vITIPCDig->vNIDDScratchImage) && (vLocalSystemData->vITIPCDig->vNIDDScratchImage->pix))
					{
						vInspectionShowing->CalculateLineDerivative(TempImageIn->pix, vLocalSystemData->vITIPCDig->vNIDDScratchImage->pix,
							vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY,
							vGlobalCurrentProduct->vSetupHistoryAverageDensity);
					}

					if (vInspectionShowing->vLookForDarkSpots)
					{
						WORD TempBottom = 0;
						WORD TempLeft = 0;
						WORD TempHeight = 0;
						WORD TempWidth = 0;
						BYTE TempAutoImproveState = 0;

						vLocalSystemData->vITIPCDig->GetInspectionROIBounds(vInspectionShowing, &TempBottom, &TempLeft, &TempHeight, &TempWidth, 
							vDisplayingImageData->vLeftReferenceAdjust, vLocalConfigurationData->vReferenceROIsToEdges, TempAutoImproveState);

						WORD TempTop = TempBottom + TempHeight;
						WORD TempRight = TempLeft + TempWidth;

						vInspectionShowing->LookForDarkSpotsOnLine(TempImageIn->pix, vLocalSystemData->vITIPCDig->vNIDDScratchImage,
							vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY, TempLeft, TempRight, TempBottom, TempTop, (BYTE)vThreshold);
					}

					if (vInspectionShowing->vDoContaminant22Algorithm)
					{
						BYTE TempThreshold = (BYTE)vThreshold;	//ams 0407 TH scaling begins
															// not from vInspectionShowing
						int TempResult22 = vInspectionShowing->InspxContaminant22InspectionB(TempImageIn->pix,
							vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, 
							vLocalSystemData->vITIPCDig->vOriginalBufferSizeY, TempThreshold,1);
					}

					if (vInspectionShowing->vSubtractAverage)
					if (vLocalSystemData->vITIPCDig->vProductAverageImage)
					//if (vInspectionShowing->vSubtractAverage)
					if (vInspectionShowing->vInspectionType != cVoidInspection)
					{
						vLocalSystemData->vITIPCDig->SubtractProductAverageImage(TempImageIn->pix,
							vLocalSystemData->vITIPCDig->vProductAverageImage);
					}

					if (vInspectionShowing->vSubtractAverage)
					if (vInspectionShowing->vInspectionType == cVoidInspection)
					if (vLocalSystemData->vITIPCDig->vProductAverageImage)
						vLocalSystemData->vITIPCDig->SubtractVoidProductAverageImage(TempImageIn->pix);

					if (vInspectionShowing->vAddStructure)
					{
						if (vInspectionShowing->vInspectionType == cVoidInspection)
						{
							if (vLocalSystemData->vITIPCDig->vVoidProductStructureImage)
								vLocalSystemData->vITIPCDig->AddVoidProductStructureImage(TempImageIn->pix);
						}
						else
						if (vInspectionShowing->vSubtractAverage)  //standard contaminant structure
						{
							if (vLocalSystemData->vITIPCDig->vProductStructureImage)
								vLocalSystemData->vITIPCDig->AddProductStructureImage(TempImageIn->pix);
						}
						else //Contaminant2 structure
						if ((vLocalSystemData->vITIPCDig->vProductContaminant2StructureImage) && (vLocalSystemData->vITIPCDig->vProductContaminant2StructureImage->vGreyImage) &&
							(vInspectionShowing->vAddStructure) && (!vInspectionShowing->vSubtractAverage) &&
							(vInspectionShowing->vInspectionType != cVoidInspection))
						{
							vLocalSystemData->vITIPCDig->AddProductContaminant2StructureImage(TempImageIn->pix);
						}
					}
					if (vShowTopHatImage)
					{
						int TempKernel[100];
						for (BYTE TempLoop = 0; TempLoop < 100; TempLoop = TempLoop + 2)
						{
							TempKernel[TempLoop] = 0;
							TempKernel[TempLoop + 1] = 1;
						}
						TempKernel[0] = 1;TempKernel[1] = 1;TempKernel[2] = 1;
						TempKernel[3] = 1;TempKernel[4] = 1;TempKernel[5] = 1;
						TempKernel[6] = 1;TempKernel[7] = 1;TempKernel[8] = 1;

						Erflag TempError = mvt_convolve_byte(TempImageIn, vShowTopHatImage,vShowTopHatImage,&TempKernel[0],0,0,MVT_NOCHANGE);
					}

					//blur image if should
					if (vShowBluredImage)
					{
						WORD TempKernelSize = 3;
						double TempScale = 1.0 / (TempKernelSize * TempKernelSize);
						for (DWORD TempLoop = 0; TempLoop < vShowBluredImage; TempLoop++)
							Erflag TempError = mvt_blur(TempImageIn,
								TempKernelSize,TempKernelSize,(float)TempScale,0);
					}

					////dilate image if should
					////if (vInspectionShowing->vDilation)
					//if (vInspectionShowing->vDilationCount)
					//for (BYTE TempLoop = 0; TempLoop < vInspectionShowing->vDilationCount; TempLoop++)
					//{
					//	DilateImage(TempImageIn, vLocalSystemData->vITIPCDig->vNIDDScratchImage);
					//}

					////erode image if should
					//if (vInspectionShowing->vDilationCount)
					//if (vInspectionShowing->vErodeCount)
					//for (BYTE TempLoop = 0; TempLoop < vInspectionShowing->vErodeCount; TempLoop++)
					//{
					//	Erflag TempResultError = mvt_erode(TempImageIn,TempImageOut);
					//	CopyMemory(TempImageIn->pix, TempImageOut->pix, vLocalSystemData->vITIPCDig->vOriginalBufferSize);
					//}

					if (vInspectionShowing->vIPContaminant)
					if (vLocalSystemData->vITIPCDig->vNIDDScratchImage2)
					if (vLocalSystemData->vITIPCDig->vNIDDScratchImage2->pix)
					{
						CopyMemory(vLocalSystemData->vITIPCDig->vNIDDScratchImage2->pix, TempImageIn->pix, vLocalSystemData->vITIPCDig->vOriginalBufferSize);
						ThresholdImage(vLocalSystemData->vITIPCDig->vNIDDScratchImage2, vInspectionShowing->vIPContaminant); //create a mask of what is below the Contrast threshold
					}

					//enhance image if should
					if (vInspectionShowing->vEnhanceMethod)
					{
						vLocalSystemData->vITIPCDig->PerformEnhanceOnImage(TempImageIn, vLocalSystemData->vITIPCDig->vNIDDScratchImage, vInspectionShowing->vEnhanceMethod);
					}

					//Erode/dilate image AT END if should to join sections of bones
					//erode image if should  make dark areas bigger
					if (vInspectionShowing->vErodeCountAtEnd)
					for (BYTE TempLoop = 0; TempLoop < vInspectionShowing->vErodeCountAtEnd; TempLoop++)
						vInspectionShowing->ErodeImage(TempImageIn, vLocalSystemData->vITIPCDig->vNIDDScratchImage);

					//dilate image if should make dark areas smaller
					if (vInspectionShowing->vDilationCountAtEnd)
					for (BYTE TempLoop = 0; TempLoop < vInspectionShowing->vDilationCountAtEnd; TempLoop++)
						vInspectionShowing->DilateImage(TempImageIn, vLocalSystemData->vITIPCDig->vNIDDScratchImage);

					//if ((vInspectionShowing->vOriginalImageFactor > 0) && (vInspectionShowing->vEnhancedImageFactor > 0))
					//{
					//	vLocalSystemData->vITIPCDig->CombineImages(TempImageIn->pix, vInspectionShowing->vEnhancedImageFactor,
					//		vDisplayingImageData->vGreyImage,vInspectionShowing->vOriginalImageFactor);
					//}

					//add histogram if should
					if (vInspectionShowing->vAddHistogram)
					if (vLocalSystemData->vITIPCDig->vProductHistogramImage)
						vLocalSystemData->vITIPCDig->AddProductHistogramImage(TempImageIn->pix);
				}

				WORD TempBottom = 0;
				WORD TempLeft = 0;
				WORD TempHeight = 0;
				WORD TempWidth = 0;

				BYTE TempAutoImproveState = vDisplayingImageData->vContainerLearnState;
				if (vInspectionShowing)
				if (!vInspectionShowing->vAddStructure)
					TempAutoImproveState = 0;
				else
				if ((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode) ||
					(vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vLocalConfigurationData->vScanTracType == cAllegroScanTrac) ||
					(vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac)) //Continuous Feed, or a Pipeline or TD
					TempAutoImproveState = 0;
				else
				if (!vLocalConfigurationData->vAutoImproveEnabled)
					TempAutoImproveState = 0;

				vLocalSystemData->vITIPCDig->GetInspectionROIBounds(vInspectionShowing, 
					&TempBottom, &TempLeft, &TempHeight, &TempWidth, vDisplayingImageData->vLeftReferenceAdjust, 
					vLocalConfigurationData->vReferenceROIsToEdges, TempAutoImproveState);

				//calculate number of pixels in ROI that are red, and value of darkest and lightest
				if (vDisplayingImageData)
				if (vInspectionShowing)
				{ //calculate the number of red pixels for this inspection
					vShowingInspectionNumberOfRedPixels = 0;
					vDarkestValueInROI = 255;
					vLightestValueInROI = 0;
					vNumberOfContaminants = 0;
					vSizeOfLargestContaminant = 0;
					vSizeOfAllContaminants = 0;
				}
/*
				if (!vInspectionShowing->vReferenceAdjustedImage)
				{
					TempLeft = TempLeft + vDisplayingImageData->vLeftReferenceAdjust;
					if (TempLeft + TempWidth >= vLocalSystemData->vITIPCDig->vAquisitionSizeY)
						TempLeft = vLocalSystemData->vITIPCDig->vAquisitionSizeY - TempWidth;
					if (TempLeft + TempWidth >= vLocalSystemData->vITIPCDig->vAquisitionSizeY)
						TempLeft = 0;
				}
*/
				vLastCalculateThreshold = (int)vThreshold;
				//if (vInspectionShowing->vThreshold)
				for (WORD TempXLoop = 0; TempXLoop < TempWidth; TempXLoop++)
				{
					BYTE *TempPointer = TempImageIn->pix + TempBottom + ((TempLeft +
						TempXLoop) * vLocalSystemData->vITIPCDig->vOriginalBufferSizeX);

					for (WORD TempYLoop = 0; TempYLoop < TempHeight; TempYLoop++)
					{
						if (*TempPointer < vDarkestValueInROI)
							vDarkestValueInROI = *TempPointer;
						if (*TempPointer > vLightestValueInROI)
							vLightestValueInROI = *TempPointer;
						if (*TempPointer < vLastCalculateThreshold)  
						{//if pixel is  darker than threshold
							vShowingInspectionNumberOfRedPixels++;
						}
						TempPointer++;
					}
				}
						/*
						//set areas outside ROI to white
						if (TempBottom)
						{
							BYTE *TempPointer = NULL;
							for (WORD TempXLoop = 0; TempXLoop < 
								vLocalSystemData->vITIPCDig->vOriginalBufferSizeY; TempXLoop++)
							{
								TempPointer = TempImageIn->pix + 
									(TempXLoop * vLocalSystemData->vITIPCDig->vOriginalBufferSizeX);

								for (WORD TempYLoop = 0; TempYLoop < TempBottom; TempYLoop++)
								{//set pixel to white since not in ROI
									*TempPointer++ = 255;
								}
							}
						}
						if (TempTop < vLocalSystemData->vITIPCDig->vOriginalBufferSizeX - 1)
						{
							BYTE *TempPointer = NULL;
							for (WORD TempXLoop = 0; TempXLoop < 
								vLocalSystemData->vITIPCDig->vOriginalBufferSizeY; TempXLoop++)
							{
								TempPointer = TempImageIn->pix + TempTop +
									(TempXLoop * vLocalSystemData->vITIPCDig->vOriginalBufferSizeX);

								for (WORD TempYLoop = 0; TempYLoop < 
									vLocalSystemData->vITIPCDig->vOriginalBufferSizeX - TempTop; TempYLoop++)
								{//set pixel to white since not in ROI
									*TempPointer++ = 255;
								}
							}
						}
						if (TempLeft)
						{
							BYTE *TempPointer = NULL;
							for (WORD TempXLoop = 0; TempXLoop < TempLeft; TempXLoop++)
							{
								TempPointer = TempImageIn->pix + 
									(TempXLoop * vLocalSystemData->vITIPCDig->vOriginalBufferSizeX);

								for (WORD TempYLoop = 0; TempYLoop < 
									vLocalSystemData->vITIPCDig->vOriginalBufferSizeX; TempYLoop++)
								{//set pixel to white since not in ROI
									*TempPointer++ = 255;
								}
							}
						}
						if (TempRight < vLocalSystemData->vITIPCDig->vOriginalBufferSizeY - 1)
						{
							BYTE *TempPointer = NULL;
							for (WORD TempXLoop = 0; TempXLoop < 
								vLocalSystemData->vITIPCDig->vOriginalBufferSizeY - TempRight; TempXLoop++)
							{
								TempPointer = TempImageIn->pix + 
									((TempXLoop + TempRight) * vLocalSystemData->vITIPCDig->vOriginalBufferSizeX);

								for (WORD TempYLoop = 0; TempYLoop < 
									vLocalSystemData->vITIPCDig->vOriginalBufferSizeX; TempYLoop++)
								{//set pixel to white since not in ROI
									*TempPointer++ = 255;
								}
							}
						}
				*/

				//threshold the image if should
				//if (vInspectionShowing->vThreshold)
				if (vInspectionShowing)
				{
					Erflag TempError = 0;
					if (vInspectionShowing->vInspectionType != cUnderfillByWeightInspection)
					if (vInspectionShowing->vInspectionType != cOverfillByWeightInspection)
					if ((vLocalConfigurationData->vDemoMode) && (vInspectionShowing->vInspectionType != cKernelInspection))
					{
						if (vLocalConfigurationData->vRedIndicatorToShowOnMainMenu == cDefectsInRed)
							vLocalSystemData->vITIPCDig->ThresholdBufferDown(
							TempImageIn->pix, vLocalSystemData->vITIPCDig->vOriginalBufferSize,
							(BYTE)vThreshold);
						else
							vLocalSystemData->vITIPCDig->ThresholdBuffer(
							TempImageIn->pix, vLocalSystemData->vITIPCDig->vOriginalBufferSize,
							(BYTE)vThreshold);
					}
					else
					{
						if ((vInspectionShowing->vInspectionType == cVoidInspection) && (!vInspectionShowing->vEnhanceMethod))
						{  //if void, and not enhanced, then must invert image
							BYTE TempThreshold = (BYTE)vThreshold;

							if (((!vShowThreshold) || (vLocalConfigurationData->vRedIndicatorToShowOnMainMenu == cDefectsInRed)) ||
								((vShowThreshold) && (vLocalConfigurationData->vRedIndicatorToShowOnMainMenu != cDefectsInPseudoColor)) ||
								((vShowThreshold) && (vLocalConfigurationData->vRedIndicatorToShowOnMainMenu == cDefectsInPseudoColor) && (vHideRedIndicator)))
							if (!vHideRedIndicator)
								ThresholdImageDown(TempImageIn,TempThreshold);
							else
								ThresholdImage(TempImageIn,TempThreshold);
						}
						else
						if ((vInspectionShowing->vInspectionType == cVoidInspection) ||
							(vInspectionShowing->vInspectionType == cGlass13mmContaminantInspection) ||
							(vInspectionShowing->vInspectionType == cJarBottomContaminantInspection) ||
							(vInspectionShowing->vInspectionType == cIPContaminantInspection) ||
							(vInspectionShowing->vInspectionType == cSpotInROIContaminantInspection) ||
							(vInspectionShowing->vInspectionType == cContaminantInspection) ||
							(vInspectionShowing->vInspectionType == cWireContaminantInspection) ||
							(vInspectionShowing->vInspectionType == cContaminantBonesInspection) ||
							//(vInspectionShowing->vInspectionType == cContaminant22Inspection) ||
							(vInspectionShowing->vInspectionType == cThresholdContaminantInspection) ||
							(vInspectionShowing->vInspectionType == cBadHeartInspection) ||
							(vInspectionShowing->vInspectionType == cDripBySpoutInspection) ||
							(vInspectionShowing->vInspectionType == cDripInspection) ||
							(vInspectionShowing->vInspectionType == cFloodedInspection) ||
							(vInspectionShowing->vInspectionType == cTopContaminantInspection) ||
							(vInspectionShowing->vInspectionType == cBottomContaminantInspection) ||

							(vInspectionShowing->vInspectionType == cItemCountInspection) || (vInspectionShowing->vInspectionType == cSizeInspection) || 
							(vInspectionShowing->vInspectionType == cMissingLidInspectionOnSize) ||
							//(vInspectionShowing->vInspectionType == cCheckWeighInspection) ||
							(vInspectionShowing->vInspectionType == cDensityInspection) ||
							(vInspectionShowing->vInspectionType == cMissingLidInspection) ||
							(vInspectionShowing->vInspectionType == cUnderfillByWeightInspection) ||
							(vInspectionShowing->vInspectionType == cOverfillByWeightInspection) ||
							(vInspectionShowing->vInspectionType == cUnderfillInspection) ||
							(vInspectionShowing->vInspectionType == cOverfillInspection) ||
							(vInspectionShowing->vInspectionType == cStandardDeviationInspection) ||
							(vInspectionShowing->vInspectionType == cContaminant2Inspection) ||
							(vInspectionShowing->vInspectionType == cAdvancedContaminant2Inspection))
						{
							if ((vInspectionShowing->vInspectionType == cItemCountInspection) && (vInspectionShowing->vMinimumBrightnessAceptable > 0) && (vInspectionShowing->vMinimumBrightnessAceptable < 255))
							{
								BYTE *TempPointer = TempImageIn->pix;
								for (DWORD TempLoop = 0; TempLoop < vLocalSystemData->vITIPCDig->vOriginalBufferSize; TempLoop++)
								{
									if (*TempPointer < vInspectionShowing->vMinimumBrightnessAceptable)
										*TempPointer = 255;
									TempPointer++;
								}
							}

							BYTE TempThreshold = (BYTE)vThreshold;

							//if (((!vShowThreshold) || (vLocalConfigurationData->vRedIndicatorToShowOnMainMenu == cDefectsInRed)) ||
							//	((vShowThreshold) && (vLocalConfigurationData->vRedIndicatorToShowOnMainMenu != cDefectsInPseudoColor)) ||
							//	((vShowThreshold) && (vLocalConfigurationData->vRedIndicatorToShowOnMainMenu == cDefectsInPseudoColor) && (vHideRedIndicator)))
							//if (!vHideRedIndicator)
							//	ThresholdImageDown(TempImageIn,TempThreshold);
							//else
								ThresholdImage(TempImageIn,TempThreshold);
						}
						else
						if (vInspectionShowing->vInspectionType == cKernelInspection)
						{
							Image *TempKernelImage;
							//create an image to store the amount each pixel is different from the baseline
							TempKernelImage = im_create(IM_BYTE, 
								vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, 
								vLocalSystemData->vITIPCDig->vAquisitionSizeY);
							if ((TempKernelImage) && (TempKernelImage->pix))
							{
								
								//store the amount each pixel is different from the baseline in the pixel in the new image
								//BYTE TempMax = 0;
								//if (!vInspectionShowing->vEnhanceMethod)
								//if (vInspectionShowing->vSubtractAverage)
								//	TempMax = 127;
								BYTE *TempDesitinationPointer = TempKernelImage->pix;
								BYTE *TempSourcePointer = TempImageIn->pix;
								for (DWORD TempLoop = 0;TempLoop < vLocalSystemData->vITIPCDig->vOriginalBufferSize; 
									TempLoop++)
								{
									if (*TempSourcePointer < 2)
										*TempSourcePointer = 2;
									*TempDesitinationPointer++ = *TempSourcePointer++;// + TempMax;
								}

								//find out how many kernels high and wide will have to analyze
								WORD TempNumberOfKernelsHigh = (WORD)((((double)vInspectionShowing->vROIPixelTop - 
									(double)vInspectionShowing->vROIPixelBottom) / 
									(double)vInspectionShowing->vSize * 2.0) + .99);
								WORD TempNumberOfKernelsWide = (WORD)((((double)vInspectionShowing->vROIPixelRight - 
									(double)vInspectionShowing->vROIPixelLeft) / 
									(double)vInspectionShowing->vSize * 2.0) + .99);

								//analyze each kernel high and wide
								for (WORD TempKernelHighLoop = 0; TempKernelHighLoop < TempNumberOfKernelsHigh;
									TempKernelHighLoop++)
								for (WORD TempKernelWideLoop = 0; TempKernelWideLoop < TempNumberOfKernelsWide;
									TempKernelWideLoop++)
								{
									DWORD TempKernelSum = 0;
									//find the starting place for this kernel analysis
									WORD TempKernelWideStart = (WORD)(vInspectionShowing->vROIPixelLeft + (TempKernelWideLoop * vInspectionShowing->vSize / 2));
									if (TempKernelWideStart + vInspectionShowing->vSize > vInspectionShowing->vROIPixelRight)
										TempKernelWideStart = (WORD)(vInspectionShowing->vROIPixelRight - vInspectionShowing->vSize);

									WORD TempKernelHighStart = (WORD)(vInspectionShowing->vROIPixelBottom + (TempKernelHighLoop * vInspectionShowing->vSize / 2));
									if (TempKernelHighStart + vInspectionShowing->vSize > vInspectionShowing->vROIPixelTop)
										TempKernelHighStart = (WORD)(vInspectionShowing->vROIPixelTop - vInspectionShowing->vSize);
									
									//sum up all the pixels in this kernel
									WORD TempSumCount = 0;
									for (WORD TempSumWideLoop = 0; TempSumWideLoop < vInspectionShowing->vSize;
										TempSumWideLoop++)
									{
										BYTE *TempDataPointer = TempKernelImage->pix + 
											TempKernelHighStart + 
											((TempKernelWideStart + TempSumWideLoop) * 
											vLocalSystemData->vITIPCDig->vOriginalBufferSizeX);
										for (WORD TempSumHighLoop = 0; TempSumHighLoop < vInspectionShowing->vSize;
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

									//if this kernel is over the average, then it is a reject
									if (TempKernelAverage < vThreshold)// + TempMax)
									{
										//put any pixels not in the baseline in the red image buffer since going to reject
										{
											BYTE *TempDestinationPointer;
											//look at all the pixels in this kernel, any that are not at baseline set red in red image
											for (WORD TempSumWideLoop = 0; TempSumWideLoop < vInspectionShowing->vSize;
												TempSumWideLoop++)
											{
												BYTE *TempDataPointer = TempKernelImage->pix + 
													TempKernelHighStart + 
													((TempKernelWideStart +	TempSumWideLoop) * 
													vLocalSystemData->vITIPCDig->vOriginalBufferSizeX);
												TempDestinationPointer = 
													TempImageIn->pix +
													TempKernelHighStart + 
													((TempKernelWideStart + TempSumWideLoop) * 
													vLocalSystemData->vITIPCDig->vOriginalBufferSizeX);
												for (WORD TempSumHighLoop = 0; TempSumHighLoop < vInspectionShowing->vSize;
													TempSumHighLoop++)
												{
													if ((*TempDataPointer < vThreshold) || (vLocalConfigurationData->vRedIndicatorToShowOnMainMenu == cDefectsInRed))
														*TempDestinationPointer = 0; //red spot
													*TempDataPointer++;
													*TempDestinationPointer++;
												}
											}
										}
									}
								}
									/*
								if (TempMax == 127)
								{
									BYTE *TempSourcePointer = TempImageIn->pix;
									for (DWORD TempLoop = 0;TempLoop < vLocalSystemData->vITIPCDig->vOriginalBufferSize; 
										TempLoop++)
									{
										if (*TempSourcePointer)
											*TempSourcePointer = *TempSourcePointer + TempMax;
										TempSourcePointer++;
									}
								}
								*/
							}
							if (TempKernelImage)
								im_delete(TempKernelImage);
						}//end of thresholding for kernel
					}
					if (TempError == 105) 
					{
						ReportErrorMessage("Error-MVT NO Lic.", cEMailInspx,32000);
					}
					else
					if (TempError != IM_OK)
					{
						ReportErrorMessage("Error-MVT Byte Only", cEMailInspx,32000);
					}

					if (vInspectionShowing->vIPContaminant)
					if (vLocalSystemData->vITIPCDig->vNIDDScratchImage2)
					if (vLocalSystemData->vITIPCDig->vNIDDScratchImage2->pix)
					{
						ThresholdImage(TempImageIn, (BYTE)vThreshold); //binarize the processed image at the Threshold

						vLocalSystemData->vITIPCDig->AndImages(vLocalSystemData->vITIPCDig->vNIDDScratchImage2->pix, TempImageIn->pix, vLocalSystemData->vITIPCDig->vOriginalBufferSize);
					}

					//find blob statistics
					vSizeOfAllContaminants = 0;
					vSizeOfLargestContaminant = 0;
					vNumberOfContaminants = 0;
					Image *TempSubImage = im_rroi(TempImageIn, TempBottom, TempLeft, TempHeight, TempWidth);

					if ((TempSubImage) && (TempSubImage->pix))
					{
						//8/27/2009
						//if (!vHideRedIndicator)
						//	TempError = vLocalSystemData->vITIPCDig->DoThresholdDwn(TempSubImage,(BYTE)vThreshold);
						//else
						//	ThresholdImage(TempSubImage,(BYTE)vThreshold);

						if (vInspectionShowing->vSize > 1)
						{
							vBlobParameters->min_area = 2;
							//vBlobParameters->min_area = vInspectionShowing->vSize;
							//if (vInspectionShowing->vSize > 3)
							//	vBlobParameters->min_area = vInspectionShowing->vSize / 2;
						}
						else
							vBlobParameters->min_area = -1;
						//TempBlobParameters->calc_results = AREA_BBOX;
						//TempBlobParameters->calc_results = 0;
						vBlobParameters->blob_color = 0x00;  //fewer xrays
						vBlobParameters->min_height = -1;
						vBlobParameters->max_area = -1;

						if ((vInspectionShowing->vInspectionType == cDripInspection) || (vInspectionShowing->vInspectionType == cDripBySpoutInspection))
						{
							if (vInspectionShowing->vMaximumSize) //for Drip Inspection
								vBlobParameters->max_area = vInspectionShowing->vMaximumSize;
							if (vInspectionShowing->vMinimumWidth) //for Drip Inspection
								vBlobParameters->min_height = vInspectionShowing->vMinimumWidth;
						}

						if (vInspectionShowing->vInspectionType == cBadHeartInspection)
							vBlobParameters->calc_results = AREA_BBOX + AREA_BBOX_CENT_PERIM;
						else
							vBlobParameters->calc_results = AREA_BBOX;


						//ThresholdImage(TempSubImage,(BYTE)vThreshold);

						if (vInspectionShowing->vInspectionType == cVoidInspection)
							ThresholdImage(TempSubImage,(BYTE)vThreshold);

						Erflag TempResult = mvt_blob_find(TempSubImage, vBlobParameters, vBlobResults, MVT_CF_DEFAULT);
						// area of blob calculation

						//bug fix for mvtools bug
						/*
						BYTE *TempCounterPointer = NULL;
						DWORD TempMainImageRedPixels = 0;
						for (WORD TempXLoop = 0; TempXLoop < TempWidth; TempXLoop++)
						{
							if ((DWORD)(TempBottom + (TempLeft + TempXLoop + 1) * vLocalSystemData->vITIPCDig->vOriginalBufferSizeX) < 
								vLocalSystemData->vITIPCDig->vOriginalBufferSize)
							{
								TempCounterPointer = TempSubImage->pix + 
									//TempBottom + (TempROILeft + //just looking at sub roi so do not need
									TempXLoop * vLocalSystemData->vITIPCDig->vOriginalBufferSizeX;

								for (WORD TempYLoop = 0; TempYLoop < TempHeight; TempYLoop++)
								{
									if (*TempCounterPointer == 0)  
									{//if pixel is  darker than threshold
										TempMainImageRedPixels++;
									}
									TempCounterPointer++;
								}
							}
							//else
							//	ReportErrorMessage("Error, Exceed ROI space", cEMailInspx,32000);
						}
						*/
//calculate data information
						vNumberOfContaminants = (DWORD)vBlobResults->num_blobs_found;
						DWORD TempArea = 0;
						vSizeOfLargestPerimeter = 0;
						if ((WORD)vBlobResults->num_blobs_found)
						{
#ifdef UseIFCDLLs
							for (WORD TempLoop = 0; TempLoop < (WORD)vBlobResults->num_blobs_found; TempLoop++)
							if (vBlobResults->blobs[TempLoop])
							{
								WORD TempBlobArea = (WORD)vBlobResults->blobs[TempLoop]->area;

								WORD TempBlobPerimeter = (WORD)mvt_blob_get_perim(vBlobResults, TempLoop);
								if (vSizeOfLargestPerimeter < TempBlobPerimeter)
									vSizeOfLargestPerimeter = TempBlobPerimeter;

								if (TempBlobArea > vSizeOfLargestContaminant)
									vSizeOfLargestContaminant = TempBlobArea;
								//if ((vInspectionShowing) && (vInspectionShowing->vInspectionType == cItemCountInspection))
								//	vSizeOfLargestContaminant = (WORD)vBlobResults->num_blobs_found;

								if ((TempBlobArea >= vInspectionShowing->vSize) || (vInspectionShowing->vInspectionType == cItemCountInspection) || (vInspectionShowing->vInspectionType == cFloodedInspection) || 
									(vInspectionShowing->vInspectionType == cMissingLidInspectionOnSize) || (vInspectionShowing->vInspectionType == cSizeInspection))
									TempArea = TempArea + TempBlobArea;
								else
									vNumberOfContaminants--;
							}
#endif					
						}
						//if (vInspectionShowing->vSize == 1)
						//if (TempArea < TempMainImageRedPixels)
						//	TempArea = TempMainImageRedPixels;

						vSizeOfAllContaminants = TempArea;

						im_delete(TempSubImage);
						TempSubImage = NULL;
					}
					else
						if (vLocalSystemData->vLogFile.vLogSerialData)
							vLocalSystemData->vLogFile.WriteToLogFile(vLocalSystemData->vFrameCountString + " Error, MVT Could not create Image for Blobs",cDebugMessage);

	//end of find blob statistics

				}  //end of thresholding the image if should
					//right line red spots from buffer
			}
			if (TempImageOut)
			{
				im_delete(TempImageOut); 
				TempImageOut = NULL;
			}
			if (TempImageIn)
			{
				im_delete(TempImageIn); 
				TempImageIn = NULL;
			}
		}
	}
}

void CNewImageDisplayDialog::AddRedSpotsToBuffer(BYTE *TempWriteBuffer,
	CImageData *TempFromImage, BYTE TempShowReferenceAdjust, CInspection *TempInspection)
{
	if (vLocalSystemData->vITIPCDig->vHaveImage)
	if ((TempFromImage) && (TempFromImage->vRedImage))
	if (TempWriteBuffer)
	{
		BYTE *TempDestinationBuffer = TempWriteBuffer;
		BYTE *TempRedSpotsBuffer = TempFromImage->vRedImage;

		double TempReferenceDelta = 0;
		int TempTopReference = 0;
		if (TempInspection)
		{
			if (TempInspection->vReferenceToTop)
				TempTopReference = TempFromImage->vTopReferenceAdjust;

			if (TempInspection->vReferenceToBottom)
				TempTopReference = TempFromImage->vTopReferenceAdjust;
		}

		if (!TempShowReferenceAdjust)
			TempReferenceDelta = TempTopReference + (TempFromImage->vLeftReferenceAdjust * vLocalSystemData->vITIPCDig->vOriginalBufferSizeX);
			
		double TempDelta = 0;

		for (DWORD TempLoop = 0; TempLoop < vLocalSystemData->vITIPCDig->vOriginalBufferSize; TempLoop++)
		{
			TempDelta = TempLoop + TempReferenceDelta;
			if (TempDelta < 0)
				TempDelta = 0;
			if (TempDelta >= vLocalSystemData->vITIPCDig->vOriginalBufferSize)
			{
				TempDelta = vLocalSystemData->vITIPCDig->vOriginalBufferSize - 1;
				//ReportErrorMessage("Error-Scaling Problem 16", cEMailInspx,32000);
			}
			TempDestinationBuffer = TempWriteBuffer + (DWORD)TempDelta;
			if (*TempDestinationBuffer < 2)  //if data is zero, make 1 so will not be red on display
				*TempDestinationBuffer = 2;
			if (*TempRedSpotsBuffer++ < TempFromImage->vContainerEjectThreshold)  //if should be red
				*TempDestinationBuffer = 1;		//set mask so will show red
		}
	}
}

void CNewImageDisplayDialog::AddRedBoxToBuffer(BYTE *TempWriteBuffer,
	CImageData *TempFromImage, BYTE TempShowReferenceAdjust, CInspection *TempInspection)
{
	if (vLocalSystemData->vITIPCDig->vHaveImage)
	if (TempFromImage)
	if (TempWriteBuffer)
	{
		double TempScaleX = 1;
		double TempScaleY = 1;

		WORD TempCornerX = TempFromImage->vRedBoxCornerY[0];
		WORD TempCornerY = TempFromImage->vRedBoxCornerX[0];
		WORD TempHeight = TempFromImage->vRedBoxHeight[0];
		WORD TempWidth = TempFromImage->vRedBoxWidth[0];

		if (!TempShowReferenceAdjust)
		{
			TempCornerX = TempCornerX + TempFromImage->vLeftReferenceAdjust;
			if (TempInspection)
			{
				if (TempInspection->vReferenceToTop)
					TempCornerY = TempCornerY + TempFromImage->vTopReferenceAdjust;

				if (TempInspection->vReferenceToBottom)
					TempCornerY = TempCornerY + TempFromImage->vTopReferenceAdjust;
			}
		}

		if (TempCornerX >= vLocalSystemData->vITIPCDig->vOriginalBufferSizeY)
			TempCornerX = 0;
		if (TempCornerY >= vLocalSystemData->vITIPCDig->vOriginalBufferSizeX)
			TempCornerY = 0;

		if (TempCornerX + TempWidth >= vLocalSystemData->vITIPCDig->vOriginalBufferSizeY)
			TempWidth = vLocalSystemData->vITIPCDig->vOriginalBufferSizeY - TempCornerX - 1;

		if (TempCornerY + TempHeight >= vLocalSystemData->vITIPCDig->vOriginalBufferSizeX)
			TempHeight = vLocalSystemData->vITIPCDig->vOriginalBufferSizeX - TempCornerY - 1;

		BYTE *TempDestinationPointer = TempWriteBuffer + TempCornerY + (TempCornerX * vLocalSystemData->vITIPCDig->vOriginalBufferSizeX);

		//draw bottom horizontal line
		BYTE *TempPointer = TempDestinationPointer;
		for (WORD TempLoop = 0; TempLoop < TempWidth; TempLoop++)
		{
			*TempPointer = 0; //set left pixel red
			TempPointer = TempPointer + vLocalSystemData->vITIPCDig->vOriginalBufferSizeX;
		}

		TempPointer = TempDestinationPointer + 1;
		for (WORD TempLoop = 0; TempLoop < TempWidth; TempLoop++)
		{
			*TempPointer = 0; //set left pixel red
			TempPointer = TempPointer + vLocalSystemData->vITIPCDig->vOriginalBufferSizeX;
		}

		//draw Top horizontal line
		TempPointer = TempDestinationPointer + TempHeight;
		for (WORD TempLoop = 0; TempLoop < TempWidth; TempLoop++)
		{
			*TempPointer = 0; //set left pixel red
			TempPointer = TempPointer + vLocalSystemData->vITIPCDig->vOriginalBufferSizeX;
		}

		TempPointer = TempDestinationPointer + TempHeight - 1;
		for (WORD TempLoop = 0; TempLoop < TempWidth; TempLoop++)
		{
			*TempPointer = 0; //set left pixel red
			TempPointer = TempPointer + vLocalSystemData->vITIPCDig->vOriginalBufferSizeX;
		}

		//draw left vertical line
		TempPointer = TempDestinationPointer;
		for (WORD TempLoop = 0; TempLoop < TempHeight; TempLoop++)
			*TempPointer++ = 0; //set left pixel red

		TempPointer = TempDestinationPointer + vLocalSystemData->vITIPCDig->vOriginalBufferSizeX;
		for (WORD TempLoop = 0; TempLoop < TempHeight; TempLoop++)
			*TempPointer++ = 0; //set left pixel red

		//draw Right vertical line
		TempPointer = TempDestinationPointer + ((TempWidth - 1) * vLocalSystemData->vITIPCDig->vOriginalBufferSizeX);
		for (WORD TempLoop = 0; TempLoop < TempHeight; TempLoop++)
			*TempPointer++ = 0; //set left pixel red

		TempPointer = TempDestinationPointer + ((TempWidth - 2) * vLocalSystemData->vITIPCDig->vOriginalBufferSizeX);
		for (WORD TempLoop = 0; TempLoop < TempHeight; TempLoop++)
			*TempPointer++ = 0; //set left pixel red
	}
}

void CNewImageDisplayDialog::AddRedBoxesToBuffer(BYTE *TempWriteBuffer, CImageData *TempFromImage, BYTE TempShowReferenceAdjust, CInspection *TempInspection)
{
	BYTE TempInspectionNumber = 0;
	for (BYTE TempLoopI = 0; TempLoopI < vGlobalCurrentProduct->vNumberOfInspections; TempLoopI++)
	if (vGlobalCurrentProduct->vInspection[TempLoopI] == TempInspection)
	{
		TempInspectionNumber = TempLoopI;
		TempLoopI = vGlobalCurrentProduct->vNumberOfInspections;
	}

	if (vLocalSystemData->vITIPCDig->vHaveImage)
	if (TempFromImage)
	if (TempWriteBuffer)
	for (BYTE TempLoop = 0; TempLoop < cMaximumRedBoxes; TempLoop++)
	if ((TempFromImage->vRedBoxWidth[TempLoop]) && (TempFromImage->vRedBoxHeight[TempLoop]))
	if ((vRejectsImageDisplay) || (!TempInspection) || ((TempFromImage->vRedBoxInspection[TempLoop] >> TempInspectionNumber) & 1))
	{
		double TempScaleX = 1;
		double TempScaleY = 1;

		WORD TempCornerX = TempFromImage->vRedBoxCornerY[TempLoop];
		WORD TempCornerY = TempFromImage->vRedBoxCornerX[TempLoop];
		WORD TempHeight = TempFromImage->vRedBoxHeight[TempLoop];
		WORD TempWidth = TempFromImage->vRedBoxWidth[TempLoop];

		if (!TempShowReferenceAdjust)
		{
			TempCornerX = TempCornerX + TempFromImage->vLeftReferenceAdjust;
			if (TempInspection)
			{
				if (TempInspection->vReferenceToTop)
					TempCornerY = TempCornerY + TempFromImage->vTopReferenceAdjust;

				if (TempInspection->vReferenceToBottom)
					TempCornerY = TempCornerY + TempFromImage->vTopReferenceAdjust;
			}
		}

		if (TempCornerX >= vLocalSystemData->vITIPCDig->vOriginalBufferSizeY)
			TempCornerX = 0;
		if (TempCornerY >= vLocalSystemData->vITIPCDig->vOriginalBufferSizeX)
			TempCornerY = 0;

		if (TempCornerX + TempWidth >= vLocalSystemData->vITIPCDig->vOriginalBufferSizeY)
			TempWidth = vLocalSystemData->vITIPCDig->vOriginalBufferSizeY - TempCornerX - 1;

		if (TempCornerY + TempHeight >= vLocalSystemData->vITIPCDig->vOriginalBufferSizeX)
			TempHeight = vLocalSystemData->vITIPCDig->vOriginalBufferSizeX - TempCornerY - 1;

		BYTE *TempDestinationPointer = TempWriteBuffer + TempCornerY + (TempCornerX * vLocalSystemData->vITIPCDig->vOriginalBufferSizeX);

		//draw bottom horizontal line
		BYTE *TempPointer = TempDestinationPointer;
		for (WORD TempPixelLoop = 0; TempPixelLoop < TempWidth; TempPixelLoop++)
		{
			*TempPointer = 0; //set left pixel red
			TempPointer = TempPointer + vLocalSystemData->vITIPCDig->vOriginalBufferSizeX;
		}

		TempPointer = TempDestinationPointer + 1;
		for (WORD TempPixelLoop = 0; TempPixelLoop < TempWidth; TempPixelLoop++)
		{
			*TempPointer = 0; //set left pixel red
			TempPointer = TempPointer + vLocalSystemData->vITIPCDig->vOriginalBufferSizeX;
		}

		//draw Top horizontal line
		TempPointer = TempDestinationPointer + TempHeight;
		for (WORD TempPixelLoop = 0; TempPixelLoop < TempWidth; TempPixelLoop++)
		{
			*TempPointer = 0; //set left pixel red
			TempPointer = TempPointer + vLocalSystemData->vITIPCDig->vOriginalBufferSizeX;
		}

		TempPointer = TempDestinationPointer + TempHeight - 1;
		for (WORD TempPixelLoop = 0; TempPixelLoop < TempWidth; TempPixelLoop++)
		{
			*TempPointer = 0; //set left pixel red
			TempPointer = TempPointer + vLocalSystemData->vITIPCDig->vOriginalBufferSizeX;
		}

		//draw left vertical line
		TempPointer = TempDestinationPointer;
		for (WORD TempPixelLoop = 0; TempPixelLoop < TempHeight; TempPixelLoop++)
			*TempPointer++ = 0; //set left pixel red

		TempPointer = TempDestinationPointer + vLocalSystemData->vITIPCDig->vOriginalBufferSizeX;
		for (WORD TempPixelLoop = 0; TempPixelLoop < TempHeight; TempPixelLoop++)
			*TempPointer++ = 0; //set left pixel red

		//draw Right vertical line
		TempPointer = TempDestinationPointer + ((TempWidth - 1) * vLocalSystemData->vITIPCDig->vOriginalBufferSizeX);
		for (WORD TempPixelLoop = 0; TempPixelLoop < TempHeight; TempPixelLoop++)
			*TempPointer++ = 0; //set left pixel red

		TempPointer = TempDestinationPointer + ((TempWidth - 2) * vLocalSystemData->vITIPCDig->vOriginalBufferSizeX);
		for (WORD TempPixelLoop = 0; TempPixelLoop < TempHeight; TempPixelLoop++)
			*TempPointer++ = 0; //set left pixel red
	}
}


void CNewImageDisplayDialog::CorrectPipeEdgePixels()
{
	if ((vLocalSystemData->vITIPCDig->vOriginalImage) && (vLocalSystemData->vITIPCDig->vOriginalImage->vGreyImage))
	if (PasswordOK(cCertifiedPassword, true))
	{
		CYesNoDialog TempYesNoDialog;
		CString TempString("");
		TempYesNoDialog.vNoticeText = _T("You should only Correct Light/Dark Pixels\nwhen you have a full pipe\nand a good image without white spots in the image.\nAre you sure you want to Correct Light/Dark Pixels?");
		TempYesNoDialog.vQuestionType = cConfirmAbandon;
		TempYesNoDialog.vYesButtonText = "Correct Pixels";
		TempYesNoDialog.vNoButtonText = "Abort";
		int TempResult = TempYesNoDialog.DoModal();
		if (TempResult == IDOK)
		{
			ReportErrorMessage("Pipeline correct edge pixels", cWriteToLog,0);
			double TempPixelAverages[cNumberOfPixelsToAdjustAllPixels1152];
			for (WORD TempLoop = 0; TempLoop < cNumberOfPixelsToAdjustAllPixels1152; TempLoop++)
				TempPixelAverages[TempLoop] = 0;
			BYTE *TempPointer = vLocalSystemData->vITIPCDig->vOriginalImage->vGreyImage;

			//get average of each pixel
			for (WORD TempLoopY = 0; TempLoopY < vLocalSystemData->vITIPCDig->vOriginalBufferSizeY; TempLoopY++)
			{
				for (WORD TempLoopX = 0; TempLoopX < vLocalSystemData->vITIPCDig->vOriginalBufferSizeX; TempLoopX++)
				{
					TempPixelAverages[TempLoopX] = TempPixelAverages[TempLoopX] + *TempPointer++;
				}
			}
			WORD TempStartCurve = 0;
			WORD TempEndCurve = 0;
			for (WORD TempLoopX = 0; TempLoopX < vLocalSystemData->vITIPCDig->vOriginalBufferSizeX; TempLoopX++)
			{
				TempPixelAverages[TempLoopX] = TempPixelAverages[TempLoopX] / vLocalSystemData->vITIPCDig->vOriginalBufferSizeY;
				if (TempPixelAverages[TempLoopX] > 255)
					TempPixelAverages[TempLoopX] = 255;
				if (TempPixelAverages[TempLoopX] < 0)
					TempPixelAverages[TempLoopX] = 0;
				if (!TempStartCurve)
				{
					if (TempPixelAverages[TempLoopX] < 250)
						TempStartCurve = TempLoopX;
				}
				else
				{
					if (TempPixelAverages[TempLoopX] < 250)
						TempEndCurve = TempLoopX;
				}
			}
			TempPixelAverages[0] = 255;
			//have average of each pixel

			//check for dead pixels
			for (WORD TempLoopX = 2; TempLoopX < vLocalSystemData->vITIPCDig->vOriginalBufferSizeX - 2; TempLoopX++)
			if (TempPixelAverages[TempLoopX] < 5)
			{
				if ((TempPixelAverages[TempLoopX - 1] > 15) && (TempPixelAverages[TempLoopX + 1] > 15))
				{
					double TempDesiredValue = (TempPixelAverages[TempLoopX - 1] + TempPixelAverages[TempLoopX + 1]) / 2.0;
					if (TempPixelAverages[TempLoopX] < TempDesiredValue)
					{
						double TempNeededCorrection = TempDesiredValue - TempPixelAverages[TempLoopX];
						vLocalConfigurationData->vDetectorEdgeAdjustOffset[TempLoopX][vGlobalCurrentProduct->vUsingXScanPoint4mmDetectors] = 
							vLocalConfigurationData->vDetectorEdgeAdjustOffset[TempLoopX][vGlobalCurrentProduct->vUsingXScanPoint4mmDetectors] + TempNeededCorrection;
						ReportErrorMessage("Corrected dead pixel1: " + dtoa(TempLoopX, 0), cEMailInspx,32000);
					}
				}
				else
				if ((TempPixelAverages[TempLoopX - 2] > 15) && (TempPixelAverages[TempLoopX + 2] > 15))
				{
					double TempDesiredValue = (TempPixelAverages[TempLoopX - 2] + TempPixelAverages[TempLoopX + 2]) / 2.0;
					if (TempPixelAverages[TempLoopX] < TempDesiredValue)
					{
						double TempNeededCorrection = TempDesiredValue - TempPixelAverages[TempLoopX];
						vLocalConfigurationData->vDetectorEdgeAdjustOffset[TempLoopX][vGlobalCurrentProduct->vUsingXScanPoint4mmDetectors] = 
							vLocalConfigurationData->vDetectorEdgeAdjustOffset[TempLoopX][vGlobalCurrentProduct->vUsingXScanPoint4mmDetectors] + TempNeededCorrection;
						ReportErrorMessage("Corrected dead pixel2: " + dtoa(TempLoopX, 0), cEMailInspx,32000);
					}
				}
				else
					ReportErrorMessage("Can't correct dead pixel: " + dtoa(TempLoopX, 0), cEMailInspx,32000);
			}
	/*
			//curve fitting here to fit curve to pipe profile
			double TempCurveFitData[cNumberOfPixelsToAdjustAllPixels1152];
			WORD TempCounter = 0;
			for (WORD TempCurveDataLoop = TempStartCurve; TempCurveDataLoop <= TempEndCurve; TempCurveDataLoop++)
			{
				TempCurveFitData[TempCounter] = TempPixelAverages[TempCurveDataLoop];
				TempCounter++;
			}
	*/
			CInspection *TempCurveFitInspection;
			TempCurveFitInspection = new CInspection;
			WORD TempNumberOfCurveFitPoints = TempEndCurve - TempStartCurve + 1;
			BYTE *TempCurveFitData = NULL;
			if ((TempNumberOfCurveFitPoints > 15) && (TempNumberOfCurveFitPoints < cNumberOfPixelsToAdjustAllPixels1152))
				TempCurveFitData = (BYTE *)malloc(TempNumberOfCurveFitPoints + 10);

			if ((TempCurveFitData) && (TempNumberOfCurveFitPoints > 15) && (TempNumberOfCurveFitPoints <= cNumberOfPixelsToAdjustAllPixels1152) && (TempEndCurve) && (TempStartCurve) && 
				(TempEndCurve < vLocalSystemData->vITIPCDig->vOriginalBufferSizeX))
			{
				BYTE *TempCurveFitDataPointer = TempCurveFitData;
		 		BYTE TempTop = (BYTE)(TempEndCurve);
				BYTE TempBottom = (BYTE)(TempStartCurve);
				//double *TempAverageDataPointer = &TempPixelAverages[0];
				for (WORD TempCurveDataLoop = TempStartCurve; TempCurveDataLoop <= TempEndCurve; TempCurveDataLoop++)
				{
					*TempCurveFitDataPointer = (BYTE)(TempPixelAverages[TempCurveDataLoop]);
					TempCurveFitDataPointer++;
				}

				tCurveFitAnalysisData vCurveFitAnalysisData;
				double vStartTime = CIFCOS::GetSystimeMicrosecs();
				vCurveFitAnalysisData.vTime = 0;
				vCurveFitAnalysisData.vCallsToSquare = 0;
				vCurveFitAnalysisData.vMainCalls = 0;
				vCurveFitAnalysisData.vNumberOfPointsToPlot = 0;
				for (BYTE TempLoopI = 0; TempLoopI < 10; TempLoopI++)
					vCurveFitAnalysisData.vTerms[TempLoopI] = 1;
				vCurveFitAnalysisData.vTerms[1] = 10;
				vCurveFitAnalysisData.vTerminationReason = -1;
				vCurveFitAnalysisData.vFNorm = 0;

				TempCurveFitInspection->ReturnCurveFitLine(TempCurveFitData, TempNumberOfCurveFitPoints, &vCurveFitAnalysisData, 3);  // use 5 terms to fit curve

				TempCurveFitDataPointer = TempCurveFitData;
				for (WORD TempCurveDataLoop = TempStartCurve; TempCurveDataLoop <= TempEndCurve; TempCurveDataLoop++)
				{
					if ((TempPixelAverages[TempCurveDataLoop] > *TempCurveFitDataPointer + 1) || (TempPixelAverages[TempCurveDataLoop] + 1 < *TempCurveFitDataPointer)) //was +-3
					{
						double TempDesiredValue = *TempCurveFitDataPointer;
						double TempNeededCorrection = TempDesiredValue - TempPixelAverages[TempCurveDataLoop];
						double TempNewOffsetValue = vLocalConfigurationData->vDetectorEdgeAdjustOffset[TempCurveDataLoop][vGlobalCurrentProduct->vUsingXScanPoint4mmDetectors] + TempNeededCorrection;
						if (TempNewOffsetValue > 128)
							TempNewOffsetValue = 128;
						if (TempNewOffsetValue < -128)
							TempNewOffsetValue = -128;
						
						ReportErrorMessage("Corrected offset pixel: " + dtoa(TempCurveDataLoop, 0, 4) +  " by: " + dtoa(TempNeededCorrection, 0, 4) + " was: " + 
							dtoa(vLocalConfigurationData->vDetectorEdgeAdjustOffset[TempCurveDataLoop][vGlobalCurrentProduct->vUsingXScanPoint4mmDetectors],0, 4) + " is: " + dtoa(TempNewOffsetValue,0, 4), cWriteToLog,0);

						vLocalConfigurationData->vDetectorEdgeAdjustOffset[TempCurveDataLoop][vGlobalCurrentProduct->vUsingXScanPoint4mmDetectors] = TempNewOffsetValue;

						if ((!vLocalConfigurationData->vDetectorEdgeAdjustGain[TempCurveDataLoop][vGlobalCurrentProduct->vUsingXScanPoint4mmDetectors]) && 
							(vLocalConfigurationData->vDetectorEdgeAdjustOffset[TempCurveDataLoop][vGlobalCurrentProduct->vUsingXScanPoint4mmDetectors]))
							vLocalConfigurationData->vDetectorEdgeAdjustGain[TempCurveDataLoop][vGlobalCurrentProduct->vUsingXScanPoint4mmDetectors] = 1;
					}
					TempCurveFitDataPointer++;
				}
			}
			if (TempCurveFitData)
				free(TempCurveFitData);
			if (TempCurveFitInspection)
				delete TempCurveFitInspection;
			vHaveAnImageToShow = false;  //don't allow to do 2 times on same image
		}
	}
}

void CNewImageDisplayDialog::ShowSensitivity()
{
	if (vInspectionShowing)
	if (vShowThresholdMenu)
	if (vLocalConfigurationData->vShowSensitivityDisplays == cShowSensitivityDisplays)
	{
		//BYTE vOldSensitivity = vSensitivity;
		vSensitivity = vInspectionShowing->GetSensitivity(true, vThreshold);
		if (vSensitivity < 255)
//		if ((vOldSensitivity != vSensitivity) || (!vOldSensitivity) || (!vSensitivity))
		{
			SetDlgItemText(IDC_SubFunction1Display, "Sen: " + dtoa((BYTE)vSensitivity,0) + "%");
			//m_SubFunction1Display.SetWindowPos(NULL,5,cSensitivityBottom,100,15,SWP_NOZORDER);
			//this->Invalidate();

			RECT TempRectangle;
			TempRectangle.left = vSensitivityLeft;
			TempRectangle.top = vSensitivityTop;
			TempRectangle.right = vSensitivityRight;
			TempRectangle.bottom = vSensitivityBottom + 1;
			this->InvalidateRect(&TempRectangle,false);

			TempRectangle.bottom = vLearnSensitivityBottom + 1;
			TempRectangle.top = TempRectangle.bottom - vSensitivityHeight - 1;
			this->InvalidateRect(&TempRectangle,false);
		}
		else
			SetDlgItemText(IDC_SubFunction1Display, _T(""));
	}
}

void CNewImageDisplayDialog::StartSensitivity()
{
	vSensitivity = 0xFF;
	if (vInspectionShowing)
	{
		vMainWindowPointer->CalculateSensitivity();
		if (vShowThresholdMenu)
		if (vLocalSystemData->vSensitivities[vInspectionNumberShowing] <= 100)
		{
			m_SubFunction1Display.ShowWindow(SW_SHOW);
			ShowSensitivity();
		}
		if (vLocalSystemData->vLearnSensitivities[vInspectionNumberShowing] <= 100)
		{
			m_SubFunction2Display.ShowWindow(SW_SHOW);
			ShowLearnSensitivity();
		}
	}
}

void CNewImageDisplayDialog::ShowLearnSensitivity()
{
	if (vInspectionShowing)
	{
		vMainWindowPointer->CalculateSensitivity();
		if (vLocalSystemData->vLearnSensitivities[vInspectionNumberShowing] <= 100)
		{
			if (vLocalSystemData->vLearnSensitivities[vInspectionNumberShowing] == 100)
				SetDlgItemText(IDC_SubFunction2Display, _T("Lrn Sen:100%"));
			else
				SetDlgItemText(IDC_SubFunction2Display, "Lrn Sen: " + dtoa((BYTE)vLocalSystemData->vLearnSensitivities[vInspectionNumberShowing], 0) + "%");

			RECT TempRectangle;
			TempRectangle.left = vSensitivityLeft;
			TempRectangle.right = vSensitivityRight;

			TempRectangle.bottom = vLearnSensitivityBottom + 1;
			TempRectangle.top = TempRectangle.bottom - vSensitivityHeight - 1;
			this->InvalidateRect(&TempRectangle,false);
		}
	}
}

void CNewImageDisplayDialog::ClearSensitivity()
{
	m_SubFunction1Display.ShowWindow(SW_HIDE);
//	m_SubFunction1Display.SetWindowPos(NULL,5,cSensitivityBottom - 5,100,20,SWP_NOZORDER);
//	::SetupItem(&m_SubFunction1Display);
	m_SubFunction2Display.ShowWindow(SW_HIDE);
//	m_SubFunction2Display.SetWindowPos(NULL,5,cLearnSensitivityBottom - 5,100,20,SWP_NOZORDER);
//	::SetupItem(&m_SubFunction2Display);
	RECT TempRectangle;
	TempRectangle.left = vSensitivityLeft;
	TempRectangle.top = vSensitivityTop;
	TempRectangle.right = vSensitivityRight;
	TempRectangle.bottom = vSensitivityBottom + 1;
	this->InvalidateRect(&TempRectangle,false);

	TempRectangle.bottom = vLearnSensitivityBottom + 1;
	TempRectangle.top = TempRectangle.bottom - vSensitivityHeight - 1;
	this->InvalidateRect(&TempRectangle,false);
}

void CNewImageDisplayDialog::OnDialogtitlestatictext1()
{
	BYTE TempHiddenKeys = 8;
	if (!PasswordOK(cTemporaryInspxPassword,false))
		TempHiddenKeys = 10;
	PostThreadMessage(vGlobalCallingThreadID, cDisplaySoftKeyMenu, TempHiddenKeys, TempHiddenKeys);
}


void CNewImageDisplayDialog::OnSubFunction1ButtonDoubleClicked()
{
	//ReportErrorMessage("Left Button Double Click on SubFunction 1",cAction, 32000);
		
	if (vMouseState > 8)	//if ((vMouseState >= 9) && (vMouseState <= 16))
	{
		KillTimer(vMouseDownRepeatTimerHandle);
		//stop scrolling
		vMouseState = 0;
	}
	else	//if (vMouseState == 0)
	if (vEditROIBoundaries)
	{
		vMouseState = 9;	//toggle move ROI lines
		int TimerResult = SetTimer(vMouseDownRepeatTimerHandle,200,NULL);
		if (!TimerResult)
			ReportErrorMessage("Error-Second Timer Failed",cEMailInspx,32000);
	}
}


void CNewImageDisplayDialog::OnSubFunction2ButtonDoubleClicked()
{
	//ReportErrorMessage("Left Button Double Click on SubFunction 2",cAction, 32000);
		
	if (vMouseState > 8)	//if ((vMouseState >= 9) && (vMouseState <= 16))
	{
		KillTimer(vMouseDownRepeatTimerHandle);
		//stop scrolling
		vMouseState = 0;
	}
	else	//if (vMouseState == 0)
	if (vEditROIBoundaries)
	{
		vMouseState = 10;	//toggle move ROI lines
		int TimerResult = SetTimer(vMouseDownRepeatTimerHandle,200,NULL);
		if (!TimerResult)
			ReportErrorMessage("Error-Second Timer Failed",cEMailInspx,32000);
	}
}


void CNewImageDisplayDialog::OnSubFunction3ButtonDoubleClicked()
{
	//ReportErrorMessage("Left Button Double Click on SubFunction 3",cAction, 32000);
		
	if (vMouseState > 8)	//if ((vMouseState >= 9) && (vMouseState <= 16))
	{
		KillTimer(vMouseDownRepeatTimerHandle);
		//stop scrolling
		vMouseState = 0;
	}
	else	//if (vMouseState == 0)
	if (vEditROIBoundaries)
	{
		vMouseState = 11;	//toggle move ROI lines
		int TimerResult = SetTimer(vMouseDownRepeatTimerHandle,200,NULL);
		if (!TimerResult)
			ReportErrorMessage("Error-Second Timer Failed",cEMailInspx,32000);
	}
}


void CNewImageDisplayDialog::OnSubFunction4ButtonDoubleClicked()
{
	//ReportErrorMessage("Left Button Double Click on SubFunction 4",cAction, 32000);
		
	if (vMouseState > 8)	//if ((vMouseState >= 9) && (vMouseState <= 16))
	{
		KillTimer(vMouseDownRepeatTimerHandle);
		//stop scrolling
		vMouseState = 0;
	}
	else	//if (vMouseState == 0)
	if (vEditROIBoundaries)
	{
		vMouseState = 12;	//toggle move ROI lines
		int TimerResult = SetTimer(vMouseDownRepeatTimerHandle,200,NULL);
		if (!TimerResult)
			ReportErrorMessage("Error-Second Timer Failed",cEMailInspx,32000);
	}
}


void CNewImageDisplayDialog::OnSubFunction5ButtonDoubleClicked()
{
	//ReportErrorMessage("Left Button Double Click on SubFunction 5",cAction, 32000);
		
	if (vMouseState > 8)	//if ((vMouseState >= 9) && (vMouseState <= 16))
	{
		KillTimer(vMouseDownRepeatTimerHandle);
		//stop scrolling
		vMouseState = 0;
	}
	else	//if (vMouseState == 0)
	if (vEditROIBoundaries)
	{
		vMouseState = 13;	//toggle move ROI lines
		int TimerResult = SetTimer(vMouseDownRepeatTimerHandle,200,NULL);
		if (!TimerResult)
			ReportErrorMessage("Error-Second Timer Failed",cEMailInspx,32000);
	}
}


void CNewImageDisplayDialog::OnSubFunction6ButtonDoubleClicked()
{
	//ReportErrorMessage("Left Button Double Click on SubFunction 6",cAction, 32000);
		
	if (vMouseState > 8)	//if ((vMouseState >= 9) && (vMouseState <= 16))
	{
		KillTimer(vMouseDownRepeatTimerHandle);
		//stop scrolling
		vMouseState = 0;
	}
	else	//if (vMouseState == 0)
	if (vEditROIBoundaries)
	{
		vMouseState = 14;	//toggle move ROI lines
		int TimerResult = SetTimer(vMouseDownRepeatTimerHandle,200,NULL);
		if (!TimerResult)
			ReportErrorMessage("Error-Second Timer Failed",cEMailInspx,32000);
	}
}


void CNewImageDisplayDialog::OnSubFunction7ButtonDoubleClicked()
{
	//ReportErrorMessage("Left Button Double Click on SubFunction 7",cAction, 32000);
		
	if (vMouseState > 8)	//if ((vMouseState >= 9) && (vMouseState <= 16))
	{
		KillTimer(vMouseDownRepeatTimerHandle);
		//stop scrolling
		vMouseState = 0;
	}
	else	//if (vMouseState == 0)
	if (vEditROIBoundaries)
	{
		vMouseState = 15;	//toggle move ROI lines
		int TimerResult = SetTimer(vMouseDownRepeatTimerHandle,200,NULL);
		if (!TimerResult)
			ReportErrorMessage("Error-Second Timer Failed",cEMailInspx,32000);
	}
}


void CNewImageDisplayDialog::OnSubFunction8ButtonDoubleClicked()
{
	//ReportErrorMessage("Left Button Double Click on SubFunction 8",cAction, 32000);
		
	if (vMouseState > 8)	//if ((vMouseState >= 9) && (vMouseState <= 16))
	{
		KillTimer(vMouseDownRepeatTimerHandle);
		//stop scrolling
		vMouseState = 0;
	}
	else	//if (vMouseState == 0)
	if (vEditROIBoundaries)
	{
		vMouseState = 16;	//toggle move ROI lines
		int TimerResult = SetTimer(vMouseDownRepeatTimerHandle,200,NULL);
		if (!TimerResult)
			ReportErrorMessage("Error-Second Timer Failed",cEMailInspx,32000);
	}
}
