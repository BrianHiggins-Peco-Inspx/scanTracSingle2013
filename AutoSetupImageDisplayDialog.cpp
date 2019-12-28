//ScanTrac Side View Source File
// AutoSetupImageDisplayDialog.cpp : implementation file
//

#include "stdafx.h"
#include "ScanTrac.h"
#include "SystemConfigurationData.h"
#include "AutoSetupImageDisplayDialog.h"
#include "NoticeDialog.h"
#include "NumericEntryDialog.h"
#include "PasswordEntryDialog.h"
#include "YesNoDialog.h"
#include "SerialPort.h"
#include "SelectMultipleItemDialog.h"
#include "AlphaEntryDialog.h"
#include "ConfigureProductContaminantProcessesDialog.h"
#include "HelpDialog.h"
#include "ConfigureInspectionsDialog.h"
#include "HistogramDisplayDialog.h"
#include "NewDiagnosticDialog.h"
#include "ImageAquisitionThread.h"
#include "SelectItemDialog.h"
#include "NewConfigProductEjectorDialog.h"
#include "ProcessQuickLearnImagesThread.h"
#include "ScanTracDlg.h"
#include "SpellmanDXMXRaySourcePort.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAutoSetupImageDisplayDialog dialog
const double cMissingLidSizeThresholdRatio = .45;

extern CSpellmanDXMXRaySourcePort *vGlobalDXMXRayPort;
extern BYTE vGlobalScanTracType;
extern bool vGlobalFPGAVersion11Point0OrHigher;
extern CString vGlobalTopString;
extern CString vGlobalBottomString;
extern WORD vGlobalPasswordTimeOut;
extern bool vGlobalFPGAVersion10Point0OrHigher;
extern BYTE vGlobalLearnState;
extern bool vGlobalEvenSecond;
//extern BYTE vGlobalMouseMoveCounter;
//extern WORD vGlobalUnderRemoteControl;
extern CProcessQuickLearnImagesThread *vGlobalProcessQuickLearnImagesThread;
extern BYTE vGlobalShiftKeyDown;
extern CSerialPort *vGlobaluCSerialPort;
extern CProduct *vGlobalCurrentProduct;
extern CString vGlobalCurrentDirectory;
extern CImageAquisitionThread *vGlobalImageAquisitionThread;
extern BYTE vGlobalInAutoSize;
extern bool vGlobalInAutoExposure;
extern int vGlobalPasswordLevel;
extern unsigned long vGlobalDialogTitleColor;

CAutoSetupImageDisplayDialog::CAutoSetupImageDisplayDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CAutoSetupImageDisplayDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAutoSetupImageDisplayDialog)
	//}}AFX_DATA_INIT
	//vAskedIfShouldRemoveBackground = false;
	vMultiImageLaneEditing = 0;
	vMaximumLaneWidth = 0;
	vGaveSendAContainerNotice = false;
	vMouseClickCount = 0;
	vEditOnlyThisInspection = false;
	vOldActualEncoderRate = 0;
	vOldPipeIsEmpty = 0;
	vOldAirBubblesInPipe = 0;
	vOldImageNumberToDisplay = 0xFFFF;
	vOldCalibrationImageNumber = 0;
	vOldXRayState = 0;
	vAdjustingBottomEdgeFinding = false;
	vBlink = 0;
	vSensitivity = 255;
	vAutosizedContainer = false;
	vChangedEdgeFindingMode = false;
	vChangedPurpleLine = false;
	vToggleSign = false;
	vOldAdjustingBrightnessBeforeLearning = false;
	vInitialized = false;
	vOldGlobalPasswordOK = false;
	vMakeStatusLineYellowRed = false;
	vNeedToAutoSizeNextContainer = false;
	//vOldUnderRemoteControl = vGlobalUnderRemoteControl;
	vJustFinishedProcessingQuickLearn = false;
	vNeedToSetContainerThresholdBounds = 0;
	vHaveAnAutoThresholdValue = false;
	vWaitingForBackgroundImage = false;
	vShowingBackground = false;
	vPickOneToEvaluate = false;
	vInspectionNumberEditing = 0;
	//vUseLowExposureModeInAutoSize = true;
	vHaveScaledImageToPaint = false;
	vNeedToCalibrateDetectors = false;
	vFreezeImageOnScreen = false;
	vOldDriftCompensationLocked = false;
	vShowReturnToEditMenu = false;
	vNeedToConfigureInspections = false;
	vCurrentDensity = 0;
	vAverageDensity = 0;
	vReturnToSetSource = false;
	vOldDriftCompensationADCOffset = 999999;
	vOldDriftCompensationDACOffset = 999999;
	vOldTakingQuickLearnImages = false;
	vTotalToLearn = 0;
	vLearningReference = false;
	vDrawPurpleLine = false;
	vCollectingBadDensityAverage = false;
	vTookNewDensityData = false;
	vInspectionEditing = NULL;
	vOldFindGoodIntensity = false;
	vOldVoidFindGoodIntensity = false;
	vOldFindBadIntensity = false;
	vOldVoidFindBadIntensity = false;
	vExitCode = 0;
	vOldErrorMessage = "None";
	vCurrentStatusLineText = "";
	vOldButton2Yellow = false;
	vOldCollectingDensityAverage = false;
	vOldDensityAverageValue = 0;
	vAutoSetupType = cNewAutoSetup;
	vShowAlternateImage = false;
	vWaitingToUpdateScreen = false;
	vStartedSource = false;
	vStartingUpSource = false;
	vRestartingXRaysWithNewSettings = false;
	vSavedImageProcessingData = false;

	vShowImageInColor = true;
	//vNeedToSetSize = false;
	vHaveNotSetSize = false;
	vHaveAnImage = false;
	vXScale = 1;
	vYScale = 1;
	vXOffset = 0;
	vYOffset = 0;
	vMode = cInitializing;
	vOldSystemRunMode = 0;

	vShowAverage = false;
	vShowStructure = false;
	vShowVoidStructure = false;
	//vShowGlassStructure = false;
	vShowContaminant2Structure = false;
	vShowHistogram = false;
	//vShowGlassHistogram = false;

	vOldXRayOnNotice = false;
	vChangeMade = false;
	vAnyChangeMade = true;
	vLocalSystemData = NULL;
	vClearStartingIndicatorIfXRaysOnTimerHandle = 52876;
	vUpdateDisplayTimerHandle = 52877;
	vOneSecondTimerHandle = 52878;
	vStartTimerHandle = 52879;
	vSetupTimerHandle = 52880;
	vGrabFocusTimerHandle = 52881;
	vGetBackgroundImageTimerHandle = 52882;
	vStartSetupTimerHandle = 52883;
	vCheckKeySwitchTimerHandle = 52884;
	vWaitToUpdateScreenTimerHandle = 52885;
	vWaitToUpdateScreenLocationEdgeTimerHandle = 52886;
	vWaitUntilDoneRunningForExitTimer = 52897;
	vWaitUntilDoneRunningForReloadTimer = 52888;
	vRestoreStatusLineTimerHandle = 52889;
	vMouseDownRepeatTimerHandle = 52890;
	vProductEditingName = "No Name";
	vHoldCurrentProduct = NULL;
	vOldAverageEnabled = false;
	vOldCalibrateBodyTriggerReferenceEnabled = false;
	vOldStructureEnabled = false;
	vOldMaximumBadAverage = 0;
	vOldMaximumBadStandardDeviation = 0;
	vOldMinimumGoodStandardDeviation = 0;
	vOldMinimumGoodAverage = 0;
	vOldQualityFactor = 0;
	vPercentBeltSpeed = 0;
	vOldMaximumBadIntensity = 0;
	vOldMinimumGoodIntensity = 256;
	vOldMaximumBadCount = 0;
	vOldMinimumGoodCount = 0;
	vImageMargin = 0;
	vMaxDetectorHeight = 0;
	vAutomaticDensityThresholdUpper = 0;
	vAutomaticDensityThresholdLower = 0;
	vHoldImageDisplayOnlyRejects = 0;
	vMainWindowPointer = NULL;
	vLocalConfigurationData = NULL;
	vLocalProductCollection = NULL;
	vMaximumThreshold = 255;
	vMouseState = 0;		//1 is left mouse button clicked down, 2 is left mouse button transistioned up, 3 is double clicked left mouse button
	vRecievedButtonPress = false;

	vLightGrayBrush = ::CreateSolidBrush(cLightGray);

	vLocalCWndCollection.Add(this);
	vLocalCWndCollection.Add(&m_DialogTitleStaticText		);
	vLocalCWndCollection.Add(&m_RejectReason				);
	vLocalCWndCollection.Add(&m_DensityNotLockedIndicator	);
	vLocalCWndCollection.Add(&m_Function1Button				);
	vLocalCWndCollection.Add(&m_SubFunction4Display			);
	vLocalCWndCollection.Add(&m_CurrentDensity				);
	vLocalCWndCollection.Add(&m_AverageDensity				);
	vLocalCWndCollection.Add(&m_SizeY						);
	vLocalCWndCollection.Add(&m_SizeX						);
	vLocalCWndCollection.Add(&m_SizeLabelY					);
	vLocalCWndCollection.Add(&m_SizeLabelX					);
	vLocalCWndCollection.Add(&m_XScale2						);
	vLocalCWndCollection.Add(&m_XScaleLabel					);
	vLocalCWndCollection.Add(&m_YScale2						);
	vLocalCWndCollection.Add(&m_YScaleLabel					);
	vLocalCWndCollection.Add(&m_Function2Button				);
	vLocalCWndCollection.Add(&m_Function3Button				);
	vLocalCWndCollection.Add(&m_SubFunction8Button3			);
	vLocalCWndCollection.Add(&m_SubFunction8Button2			);
	vLocalCWndCollection.Add(&m_SubFunction8Button1			);
	vLocalCWndCollection.Add(&m_SubFunction6Button3			);
	vLocalCWndCollection.Add(&m_SubFunction6Button2			);
	vLocalCWndCollection.Add(&m_SubFunction6Button1			);
	vLocalCWndCollection.Add(&m_Calibrating					);
	vLocalCWndCollection.Add(&m_SubFunction2Display			);
	vLocalCWndCollection.Add(&m_Reference					);
	vLocalCWndCollection.Add(&m_SubFunction3Display			);
	vLocalCWndCollection.Add(&m_SubFunction1Display			);
	vLocalCWndCollection.Add(&m_ThresholdForContainerBounds2);
	vLocalCWndCollection.Add(&m_Width2						);
	vLocalCWndCollection.Add(&m_CurrentLabel				);
	vLocalCWndCollection.Add(&m_Background					);
	vLocalCWndCollection.Add(&m_Function5Button				);
	vLocalCWndCollection.Add(&m_SubFunction8Button			);
	vLocalCWndCollection.Add(&m_SubFunction7Button			);
	vLocalCWndCollection.Add(&m_SubFunction6Button			);
	vLocalCWndCollection.Add(&m_SubFunction5Button			);
	vLocalCWndCollection.Add(&m_SubFunction8Display			);
	vLocalCWndCollection.Add(&m_SubFunction4Button			);
	vLocalCWndCollection.Add(&m_SubFunction3Button			);
	vLocalCWndCollection.Add(&m_SubFunction2Button			);
	vLocalCWndCollection.Add(&m_SubFunction1Button			);
	vLocalCWndCollection.Add(&m_SubFunction4DisplayA		);
	vLocalCWndCollection.Add(&m_Function4Button				);
	vLocalCWndCollection.Add(&m_SubFunction5Display			);
	vLocalCWndCollection.Add(&m_SubFunction7Display			);
	vLocalCWndCollection.Add(&m_SubFunction6Display			);
	vLocalCWndCollection.Add(&m_XRaysOn						);
	vLocalCWndCollection.Add(&m_PleaseWait					);
	vLocalCWndCollection.Add(&m_Simulating					);
	vLocalCWndCollection.Add(&m_StatusLine					);
	vLocalCWndCollection.Add(&m_StatusLine2					);
}

void CAutoSetupImageDisplayDialog::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	// TODO: Add your message handler code here

	vAutoSetupSensitivityLeft = (WORD)ResizeXCoor(3);
	vAutoSetupSensitivityBottom = (WORD)ResizeYCoor(125);
	vAutoSetupSensitivityHeight = (WORD)ResizeYCoor(10);
	vAutoSetupTop = vAutoSetupSensitivityBottom - vAutoSetupSensitivityHeight;
	vAutoSetupSensitivityWidth = (WORD)ResizeXCoor(100);
	vAutoSetupRight = vAutoSetupSensitivityLeft + vAutoSetupSensitivityWidth;
	vAutoSetupSensitivityRight = vAutoSetupSensitivityLeft + vAutoSetupSensitivityWidth;
	vAutoSetupLearnSensitivityBottom = (WORD)ResizeYCoor(215);
	vAutoSetupSensitivityTop = vAutoSetupSensitivityBottom - vAutoSetupSensitivityHeight;

	vFunction1ButtonEnable = true;
	vFunction2ButtonEnable = true;
	vFunction3ButtonEnable = true;
	vFunction4ButtonEnable = true;
	vFunction5ButtonEnable = true;

	vSubFunction1ButtonEnable = true;
	vSubFunction2ButtonEnable = true;
	vSubFunction3ButtonEnable = true;
	vSubFunction4ButtonEnable = true;
	vSubFunction5ButtonEnable = true;
	vSubFunction6ButtonEnable = true;
	vSubFunction7ButtonEnable = true;
	vSubFunction8ButtonEnable = true;


	if (vLocalSystemData->vXRayOnNotice)
	{
		CString TempText("");
		TempText.LoadString(IDS_XRAYON);
		SetDlgItemText(IDC_Calibrating,TempText);
		m_Calibrating.ShowWindow(SW_SHOW);
		m_PleaseWait.ShowWindow(SW_HIDE);
		vBlink = 255;
		//SetDlgItemText(IDC_Background, " ");
	}
	vLocalSystemData->vChangedSizeOfProduct = false;
	vLocalSystemData->vChangedSizeOfProductCheckDensityROI = 0;
	vMaximumUnderfillByWeightThreshold = vLocalSystemData->vITIPCDig->vOriginalBufferSizeX * vLocalSystemData->vITIPCDig->vOriginalBufferSizeY * 255;
	vMaximumThreshold = 255;
	if ((vInspectionEditing) && ((vInspectionEditing->vInspectionType == cUnderfillByWeightInspection) || 
		(vInspectionEditing->vInspectionType == cOverfillByWeightInspection) || (vInspectionEditing->vInspectionType == cCheckWeighInspection)))
		vMaximumThreshold = vMaximumUnderfillByWeightThreshold;

	//9/10/2009
	if (!PasswordOK(cAdvancedSetupPassword,false))  //if password below setup advanced
	{
		BYTE TempLength = vLocalConfigurationData->vProductSetupPassword.GetLength();
		BYTE TempLengthA = vLocalConfigurationData->vAdvancedSetupPassword.GetLength();

		if ((TempLength == 0) && (TempLengthA == 0))
			vGlobalPasswordLevel = cAdvancedSetupPassword; //if both passwords blank, enable advanced mode
		else
		if (TempLength)
		if (TempLengthA >= TempLength)
		{
			CString TempVPassword = vLocalConfigurationData->vAdvancedSetupPassword.Left(TempLength);
			if (TempVPassword == vLocalConfigurationData->vProductSetupPassword)
				vGlobalPasswordLevel = cAdvancedSetupPassword; //if start of advanced password is same as product password, enable advanced mode
		}
	}

	CString TempText;
	TempText.LoadString(IDS_Wait);
	SetDlgItemText(IDC_Function3Button,TempText);

	TempText.LoadString(IDS_Cur);
	TempText = TempText + ": 0";
	SetDlgItemText(IDC_CurrentDensity, TempText);

	m_SubFunction5Button.ShowNavigationArrow(NULL);
	m_SubFunction6Button.ShowNavigationArrow(NULL);
	m_SubFunction7Button.ShowNavigationArrow(NULL);
	m_SubFunction8Button.ShowNavigationArrow(NULL);

	if ((vAutoSetupType == cNewAutoSetup) || (vAutoSetupType == cCopyAutoSetup))
		vGlobalCurrentProduct->ChangeInspectionNames();

	if (vAutoSetupType == cNewAutoSetup)
	{
		vLocalSystemData->vSetupANewProduct = true;
	}
	else
		vLocalSystemData->vSetupANewProduct = false;

	//set standard integration time and voltage if copied product
	if ((!vLocalConfigurationData->vContinuousFeedContainerRate) && (!vLocalConfigurationData->vBulkProductMode) && (!vGlobalCurrentProduct->vBulkProductMode)) //Not Continuous Feed, Not a Pipeline
	if (vAutoSetupType == cCopyAutoSetup)
	{
		//9/11/2009
		vGlobalCurrentProduct->vTypesOfRejectsToView = 0xFFFF; //view all reject types

		CYesNoDialog TempYesNoDialog;
		TempYesNoDialog.vNoticeText = _T("\nDoes this new product have the\nexact same size container as the product copied?\n(If not, settings will be changed for easier auto sizing)");
		TempYesNoDialog.vQuestionType = cConfirmQuestion;
		TempYesNoDialog.vYesButtonText = "Different\nSize";
		TempYesNoDialog.vNoButtonText = "Same Exact Size";
		int TempResult = TempYesNoDialog.DoModal();
		if (TempResult == IDOK)
		{
			vAutoSetupType = cNewAutoSetup;
			ReportErrorMessage("Start Auto Setup Copy Product but different size: " + *vGlobalCurrentProduct->GetProductName(), cUserAction,0);
		}
		else
		{
			ReportErrorMessage("Start Auto Setup Copy Product same size: " + *vGlobalCurrentProduct->GetProductName(), cUserAction,0);
/*
			CProduct *TempCalibrateProduct = vMainWindowPointer->GetProductByName("~Calibrate Gain");
			if ((TempCalibrateProduct) && (vGlobalCurrentProduct))
			{
				float TempVoltage = (float)(38.0 / 6); //600 wat
				if (vLocalConfigurationData->vHVPSType == cMNXHVPS)  //50 watt tube
					TempVoltage = (float)(25.0 / 6); //50 watt

				double TempMaximumCurrent = GetMaximumCurrentFromVoltage(vGlobalCurrentProduct->vXRaySourceVoltage * 6);
				if (TempMaximumCurrent > vLocalConfigurationData->vMaximumCurrent)
					TempMaximumCurrent = vLocalConfigurationData->vMaximumCurrent;

				double TempMaximumCurrentP = GetMaximumCurrentFromPower(vGlobalCurrentProduct->vXRaySourceVoltage * 6, 
					vLocalConfigurationData->vMaximumPower);

				if (TempMaximumCurrent > TempMaximumCurrentP)
					TempMaximumCurrent = TempMaximumCurrentP;

				if (TempMaximumCurrent > 5)
					TempMaximumCurrent = 5;

				if ((vGlobalCurrentProduct->vXRayIntegrationTime != TempCalibrateProduct->vXRayIntegrationTime) ||
					(vGlobalCurrentProduct->vXRaySourceCurrent != TempMaximumCurrent))
				{
					CYesNoDialog TempYesNoDialog;
					TempYesNoDialog.vNoticeText = "\nThis Product's Exposure settings are Non-Standard.\nIt is recommended to use the standard settings.\nUse standard Integration Time and Current?";
					TempYesNoDialog.vQuestionType = cConfirmQuestion;
					TempYesNoDialog.vYesButtonText = "Use Standard";
					TempYesNoDialog.vNoButtonText = "Keep Existing";
					int TempResult = TempYesNoDialog.DoModal();
					if (TempResult == IDOK)
					{
						vGlobalCurrentProduct->vXRayIntegrationTime = TempCalibrateProduct->vXRayIntegrationTime;
						vGlobalCurrentProduct->vXRaySourceCurrent = (float)TempMaximumCurrent;
						vGlobalCurrentProduct->vXRaySourceVoltage = TempVoltage;
					}
				}
			}
			*/
		}
	}
	//if a new product
	if (vAutoSetupType == cNewAutoSetup)
	{
		ReportErrorMessage("Start Auto Setup New Product", cUserChanges,0);
		vLocalSystemData->vShiftProductCode = " ";

		vHaveNotSetSize = true;
		//CProduct *TempCalibrateProduct = vMainWindowPointer->GetProductByName("~Calibrate Gain");
		//if ((TempCalibrateProduct) && (vGlobalCurrentProduct))
		//{

		if (vLocalConfigurationData->vSystemBodyTriggerToXRayBeam)
			vGlobalCurrentProduct->SetProductBodyTriggerToImageBeltPositionOffset((float)(vLocalConfigurationData->vSystemBodyTriggerToXRayBeam - 2));
		else
		if (vLocalConfigurationData->vScanTracType == cForteScanTrac)
			vGlobalCurrentProduct->SetProductBodyTriggerToImageBeltPositionOffset((float)(cForteTriggerToBeamOffset));
		else
		if ((vLocalConfigurationData->vScanTracType == cSoloScanTrac) || (vLocalConfigurationData->vScanTracType == cSoloPlusScanTrac))
			vGlobalCurrentProduct->SetProductBodyTriggerToImageBeltPositionOffset((float)(cSoloRightToLeftDistance));
		else
		if ((vLocalConfigurationData->vScanTracType == cSoloScanTrac) || (vLocalConfigurationData->vScanTracType == cSoloPlusScanTrac))
			vGlobalCurrentProduct->SetProductBodyTriggerToImageBeltPositionOffset((float)(cSoloLeftToRightDistance));
		else
		if (vLocalConfigurationData->vScanTracType == cFermataScanTrac)
			vGlobalCurrentProduct->SetProductBodyTriggerToImageBeltPositionOffset((float)(4));
		else
		if (vLocalConfigurationData->vScanTracType == cPiccoloScanTrac)
			vGlobalCurrentProduct->SetProductBodyTriggerToImageBeltPositionOffset((float)(cPiccoloDistance));
		else
		if (vLocalConfigurationData->vScanTracType == cAllegroScanTrac)
			vGlobalCurrentProduct->SetProductBodyTriggerToImageBeltPositionOffset((float)(cAllegroTriggerToBeamOffset));
		else
			vGlobalCurrentProduct->SetProductBodyTriggerToImageBeltPositionOffset((float)(cSoloRightToLeftDistance));

		//	vGlobalCurrentProduct->vXRayIntegrationTime = TempCalibrateProduct->vXRayIntegrationTime;
		//}
		//if (vLocalConfigurationData->vHVPSType == cMNXHVPS)  //50 watt tube
		//	vGlobalCurrentProduct->vXRaySourceVoltage = (float)(25.0 / 6); //50 watt
		//else
		//if (vLocalConfigurationData->vHVPSType == cDXMHVPS)  //100 watt monoblock
		//	vGlobalCurrentProduct->vXRaySourceVoltage = (float)(38.0 / 6); //600 watt
		//else
		//	vGlobalCurrentProduct->vXRaySourceVoltage = (float)(60.0 / 6); //100 watt

		double TempMaximumCurrent = GetMaximumCurrentFromVoltage(vGlobalCurrentProduct->vXRaySourceVoltage * 6);
		if (TempMaximumCurrent > vLocalConfigurationData->vMaximumCurrent)
			TempMaximumCurrent = vLocalConfigurationData->vMaximumCurrent;

		double TempMaximumCurrentP = GetMaximumCurrentFromPower(vGlobalCurrentProduct->vXRaySourceVoltage,
			vLocalConfigurationData->vMaximumPower);

		if (TempMaximumCurrent > TempMaximumCurrentP)
			TempMaximumCurrent = TempMaximumCurrentP;

		if (TempMaximumCurrent > 5)
			TempMaximumCurrent = 5;

		vGlobalCurrentProduct->vXRaySourceCurrent = (float)TempMaximumCurrent;

		//set ejector position so be sure to get image and process it before passes ejector
		for (BYTE TempLoop = 0; TempLoop < cNumberOfEjectors; TempLoop++)
		if (vGlobalCurrentProduct->vEjectorDistanceFromTriggerInInches[TempLoop] < 54)
			vGlobalCurrentProduct->SetEjectorBeltPositionOffset(TempLoop, (float)(64 + (TempLoop * 10)));

		vGlobalCurrentProduct->vTypesOfRejectsToView = 0xFFFF; //view all reject types

		vLocalSystemData->vITIPCDig->DeleteLearnImages();
		vLocalSystemData->vITIPCDig->DeleteLearnFiles(vGlobalCurrentProduct);
		ClearDensityFeedBackLoop();

		if ((!vLocalConfigurationData->vContinuousFeedContainerRate) && (!vLocalConfigurationData->vBulkProductMode) && (!vGlobalCurrentProduct->vBulkProductMode) && (vGlobalScanTracType != cBriosoScanTrac)) //Not Continuous Feed, or a Pipeline or TD
		{
			float TempNewWidth = 24;
			if (vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac)  //on Trio, can only have 672 lines in the image
			{
				TempNewWidth = 48; //merge 12/6/2012
				WORD TempNumberOfLines = (WORD)(TempNewWidth * vGlobalPixelsPerUnit * vGlobalCurrentProduct->vOverScanMultiplier);
				if (TempNumberOfLines > 672)
					TempNewWidth =  (float)((672 / vGlobalPixelsPerUnit) / vGlobalCurrentProduct->vOverScanMultiplier);
			}
			if ((vLocalConfigurationData->vScanTracType == cFermataScanTrac) || (vLocalConfigurationData->vScanTracType == cLegatoScanTrac) ||
				(vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode))
				TempNewWidth = 1;
			if (vGlobalScanTracType == cBriosoScanTrac)//on Brioso, want to start with 12" square image
				//TempNewWidth = 12; //set width to same number of pixels (converted to inches) as the height
				TempNewWidth = (float)(vLocalConfigurationData->vDetectorLength * vLocalConfigurationData->vNumberOfXRayDetectors * vGlobalPixelsPerUnitInHeight / vGlobalPixelsPerUnit);

			vGlobalCurrentProduct->SetProductImageWidth(TempNewWidth);
			vLocalSystemData->vChangedSizeOfProduct = true;
			vGlobalCurrentProduct->vProductImageHeightBottom = 0;
			vGlobalCurrentProduct->vProductImageHeightTop = (float)(vLocalConfigurationData->vDetectorLength * vLocalConfigurationData->vNumberOfXRayDetectors);
			//if (vLocalConfigurationData->vScanTracType == cAllegroScanTrac)
			//	vGlobalCurrentProduct->vProductImageHeightTop = (float)(vLocalConfigurationData->vDetectorLength * 9);


			if ((vLocalConfigurationData->vScanTracType == cPiccoloScanTrac) || (vLocalConfigurationData->vScanTracType == cLegatoScanTrac))//50 watt system with 8 inch image max
			if (vGlobalCurrentProduct->vProductImageHeightTop > 8)
			 vGlobalCurrentProduct->vProductImageHeightTop = 8;

			//vGlobalCurrentProduct->vProductImageWidth

			//vGlobalCurrentProduct->vImageWidthPosition

			//if (vLocalSystemData->vITIPCDig->vOriginalBufferSizeY == vLocalSystemData->vITIPCDig->vOriginalBufferSizeX)
			//if (vLocalConfigurationData->vUseMetric)
			//{
			//	vGlobalCurrentProduct->vProductImageHeightTop = (WORD)(vGlobalCurrentProduct->vProductImageHeightTop * 25.4);
			//	vGlobalCurrentProduct->SetProductImageWidth((float)(vGlobalCurrentProduct->vProductImageWidth * 25.4));
			//}

			vGlobalCurrentProduct->vProductBodyTriggerToImageDistanceInInches = 8;

			if (vLocalConfigurationData->vSystemBodyTriggerToXRayBeam > 1)
			{
				double TempWhiteSpaceOnOneSideOfImage = (vGlobalCurrentProduct->vProductImageWidth / 1.6) * 0.3;
				if (TempWhiteSpaceOnOneSideOfImage > 4)
					TempWhiteSpaceOnOneSideOfImage = 4;

				vGlobalCurrentProduct->vProductBodyTriggerToImageDistanceInInches = (float)(vLocalConfigurationData->vSystemBodyTriggerToXRayBeam - TempWhiteSpaceOnOneSideOfImage);
			}
			if (vGlobalCurrentProduct->vProductBodyTriggerToImageDistanceInInches < 2)
				vGlobalCurrentProduct->vProductBodyTriggerToImageDistanceInInches = 2;

			//if (vLocalConfigurationData->vUseMetric)
			//	vGlobalCurrentProduct->vProductBodyTriggerToImageDistanceInInches = (float)(vGlobalCurrentProduct->vProductBodyTriggerToImageDistanceInInches * 25.4);

			vGlobalCurrentProduct->SetProductBodyTriggerToImageBeltPositionOffset(vGlobalCurrentProduct->vProductBodyTriggerToImageDistanceInInches);

			vGlobalCurrentProduct->SetProductLockOutWidth(TempNewWidth); //merge 12/6/2012
			vGlobalCurrentProduct->vHaveManuallySetRetriggerLockout = false;

			if (((vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vLocalConfigurationData->vScanTracType == cAllegroScanTrac) ||
				(vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac) || (vLocalConfigurationData->vScanTracType == cBriosoScanTrac) || (vGlobalScanTracType == cBriosoScanTrac))) //Not Continuous Feed, or a Pipeline or TD
			{
				vGlobalCurrentProduct->SetEdgeLocationBottom((float)(2));
				vGlobalCurrentProduct->SetEdgeLocationHeight((float)(10));
			}
			else
			{
				vGlobalCurrentProduct->SetEdgeLocationBottom((float)(.75));
				vGlobalCurrentProduct->SetEdgeLocationHeight((float)(1));
			}
		}
	}
			
	if ((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode)) // Continuous Feed, or a Pipeline or TD
	if (vAutoSetupType == cNewAutoSetup)
	{
		vGlobalCurrentProduct->SetProductLockOutWidth(.5); //was 2
		vGlobalCurrentProduct->SetProductBodyTriggerToImageBeltPositionOffset(1);  //was 4
		float TempNewWidth = 1; //was 4
		vGlobalCurrentProduct->SetProductImageWidth(TempNewWidth);
		vGlobalCurrentProduct->SetEdgeLocationBottom((float)(2));
		vGlobalCurrentProduct->SetEdgeLocationHeight((float)(10));
		vGlobalCurrentProduct->vHaveManuallySetRetriggerLockout = false;
	}

	if (vAutoSetupType == cNewAutoSetup)
		vNeedToSetContainerThresholdBounds = 2;

	//vLocalSystemData->vInLowExposureMode = false;
	if (vLocalSystemData->vSystemRunMode == cStoppedSystemMode)
		vLocalSystemData->vInAutoSetup = true;

	//vLocalSystemData->vITIPCDig->vDisplayImageSizeX = 416;
	vLocalSystemData->vITIPCDig->vDisplayImageSizeY = (WORD) ResizeYCoor(338); //360 is regular image display;

	if (vLocalSystemData->vITIPCDig->vImage)
		delete vLocalSystemData->vITIPCDig->vImage;
	vLocalSystemData->vITIPCDig->vImage = NULL;

	vLocalSystemData->vITIPCDig->SetupImageBuffer(
		vLocalSystemData->vITIPCDig->vDisplayImageSizeX, 
		vLocalSystemData->vITIPCDig->vDisplayImageSizeY);

	if (!vLocalSystemData->vITIPCDig->vImage)
	if (vLocalSystemData->vITIPCDig->vImageBuffer)
	if (ThereIsEnoughMemory(sizeof(CImgConn), "Frame Grabber Interface3"))
		vLocalSystemData->vITIPCDig->vImage = new CImgConn(
			vLocalSystemData->vITIPCDig->vImageBuffer,
			(WORD)vLocalSystemData->vITIPCDig->vDisplayImageSizeX,
			(WORD)vLocalSystemData->vITIPCDig->vDisplayImageSizeY,
			(WORD)vLocalSystemData->vITIPCDig->vBitsPerPixel, 
			vLocalSystemData->vITIPCDig->vWindowHandle);

	if (vLocalSystemData->vInAutoSetup)
	{
		if (vMainWindowPointer->InterlocksOK())
			vStartingUpSource = true;

		if (!vShowReturnToEditMenu)
		{
			vMainWindowPointer->MakeProductionReportIfNeeded();
			vMainWindowPointer->ResetCounters();
			vMainWindowPointer->SetupProduct(vGlobalCurrentProduct, vStartedSource);
			//LoadBackGroundIfNeeded();
		}

		if (!vGlobalCurrentProduct->ProductHasImageFiles(vGlobalCurrentDirectory))
			vGlobalCurrentProduct->vSetupHistoryAverageDensity = 0;

		//if (vMainWindowPointer->InterlocksOK())
		{
			int TimerResult = SetTimer(vStartSetupTimerHandle,20,NULL);
			if (!TimerResult)
				ReportErrorMessage("Error-Start Timer Failed",cEMailInspx,32000);
		}
	}

	//edit while running or autosetup
	//copy product to a temporary so can edit current one and abort if needed
	//and can copy settings from temporary one back
	if (ThereIsEnoughMemory(sizeof(CProduct), "Temp Edit Product"))
		vHoldCurrentProduct = new CProduct;
	if (vHoldCurrentProduct != NULL)
	{
		vHoldCurrentProduct->CopyProduct(vGlobalCurrentProduct);
	}

	vNumberEjectorsConfigured = 0;
	vOnlyEjectorNumberConfigured = 0;
	for (BYTE TempLoop = 0; TempLoop < cNumberOfEjectors; TempLoop++)
	{
		if ((vLocalConfigurationData->vEjector[TempLoop].vEnabled) && (vLocalConfigurationData->vEjector[TempLoop].vLineBitMask))
		{
			vNumberEjectorsConfigured++;
			vOnlyEjectorNumberConfigured = TempLoop;
		}
	}

	vImageMargin = (float)0.40;

	vMaxDetectorHeight = (float)(vLocalConfigurationData->vNumberOfXRayDetectors * vLocalConfigurationData->vDetectorLength);
	//if (vLocalConfigurationData->vScanTracType == cAllegroScanTrac)
	//	vMaxDetectorHeight = (float)(9 * vLocalConfigurationData->vDetectorLength);

	UpdateAverageDensityDisplay();

	vHoldImageDisplayOnlyRejects = vLocalSystemData->vITIPCDig->vImageDisplayOnlyRejects;
	vLocalSystemData->vITIPCDig->vImageDisplayOnlyRejects = false;
	
	if (vLocalSystemData->vInAutoSetup)
	{
		vLocalSystemData->vITIPCDig->vCalibrateBodyTriggerReferenceEnabled = false;
		vLocalSystemData->vITIPCDig->vAverageEnabled = false;
		vLocalSystemData->vITIPCDig->vStructureEnabled = false;
	}
	
	vProductEditingName = *vGlobalCurrentProduct->GetProductName();

	if (vLocalSystemData->vXRayOnNotice)
		TempText.LoadString(IDS_WaitingForContainer);
	else
		TempText = " Starting X-Rays, Please Wait..";

	SetStatusLine(TempText + vProductEditingName);
	CWnd *TempWindow = CWnd::GetDlgItem(IDC_Background);
	if (TempWindow)
		vLocalSystemData->vITIPCDig->vWindowHandle = TempWindow->m_hWnd;

	TempText.LoadString(IDS_XRAYON);
	SetDlgItemText(IDC_XRaysOn,TempText);

	if (!vGlobalCurrentProduct->vSetupHistoryAverageDensity)
		vLocalSystemData->vITIPCDig->vDriftCompensationLocked = true;
	vOldDriftCompensationLocked = vLocalSystemData->vITIPCDig->vDriftCompensationLocked;
	if (vOldDriftCompensationLocked)
	{
		m_DensityNotLockedIndicator.ShowWindow(SW_HIDE);
	}
	else
	{
		m_DensityNotLockedIndicator.ShowWindow(SW_SHOW);
		if (ResizeXCoor(1) >= 2)
		{
			vGraphicBitmap.DeleteObject();
			vGraphicBitmap.LoadBitmap(IDB_InspxGraphicYellow4xBigger);
			m_DensityNotLockedIndicator.SetBitmap(vGraphicBitmap);

			RECT TempRect;
			m_DensityNotLockedIndicator.GetWindowRect(&TempRect);
			TempRect.top -= 26;
			TempRect.bottom -= 23;
			m_DensityNotLockedIndicator.SetWindowPos(NULL, TempRect.left, TempRect.top, TempRect.right - TempRect.left, TempRect.bottom - TempRect.top, NULL);
		}
		else if (ResizeXCoor(10) >= 15)
		{
			vGraphicBitmap.DeleteObject();
			vGraphicBitmap.LoadBitmap(IDB_InspxGraphicYellow2xBigger);
			m_DensityNotLockedIndicator.SetBitmap(vGraphicBitmap);
		}
	}

	m_Function1Button.ShowWindow(SW_SHOW);
	m_Function1Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
	TempText.LoadString(IDS_Abort);
	SetDlgItemText(IDC_Function1Button,TempText);

	m_Function4Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
	m_Function4Button.ShowWindow(SW_SHOW);
	if (vLocalSystemData->vInAutoSetup)
		TempText.LoadString(IDS_PreviousAutoSetup);
	else
		TempText.LoadString(IDS_PreviousSetupStep);
	SetDlgItemText(IDC_Function4Button,TempText);

	m_Function5Button.ShowWindow(SW_SHOW);
	m_Function5Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
	if (vLocalSystemData->vInAutoSetup)
		TempText.LoadString(IDS_NextAutoSetupStep);
	else
		TempText.LoadString(IDS_NextSetupStep);
	SetDlgItemText(IDC_Function5Button,TempText);

	//m_CurrentDensity.ShowWindow(SW_SHOW);
	UpdateButtonText();

	if (!vLocalSystemData->vInAutoSetup)
	{	//started later when in autosetup	
		int TimerResult = SetTimer(vOneSecondTimerHandle,1000,NULL);
		if (!TimerResult)
			ReportErrorMessage("Error-One Second Timer Failed",cEMailInspx,32000);
	}

	if (vLocalSystemData->vInAutoSetup)
		this->SetWindowText(_T("AutoSetup"));
	else
		this->SetWindowText(_T("Setup"));

	if (vLocalSystemData->vInAutoSetup)
		ReportErrorMessage("Start Auto Setup Product :" + *vGlobalCurrentProduct->GetProductName(), cUserAction,0);
	else
		ReportErrorMessage("Start Setup Product :" + *vGlobalCurrentProduct->GetProductName(), cUserAction,0);

	CheckIfShouldLockDensityFeedbackLoop();

	if (vMode == cSetROI)
	{
		//if ((!vLocalSystemData->vInAutoSetup) && (vInspectionEditing))
		if (vInspectionEditing)
		{
			StartROI();
		}
		else
			vNeedToConfigureInspections = true;
	}
	else
	if (vAutoSetupType == cEditInspectionAutoSetup)
		vNeedToConfigureInspections = true;
	else
	if (vMode == cSetInspections)
		vNeedToConfigureInspections = true;
	else
	if (vMode == cSetVoidThreshold)
	{
		if ((!vLocalSystemData->vInAutoSetup) && (vInspectionEditing))
		{
			StartVoidThreshold();
		}
		else
			vNeedToConfigureInspections = true;
	}
	else
	if (vMode == cSetContaminantThreshold)
	{
		if ((!vLocalSystemData->vInAutoSetup) && (vInspectionEditing))
		{
			StartContaminantThreshold();
		}
		else
			vNeedToConfigureInspections = true;
	}
	else
	if (vMode == cSetDensityThreshold)
	{
		if ((!vLocalSystemData->vInAutoSetup) && (vInspectionEditing))
		{
			StartDensityThreshold();
		}
		else
			vNeedToConfigureInspections = true;
	}
	else
	if (vMode == cSetImageSize)
		StartSizeMenu();
	else
	if (vMode == cLearnMenu)
		StartLearnMenu();
	else
	if (vMode == cCenterExposureMenu)
		StartCenterExposureMenu();
	UpdateFunction3Button();

	//vGlobalMouseMoveCounter = 0;
	int TimerResult = SetTimer(vGrabFocusTimerHandle,50,NULL);  //WAS 100
	if (!TimerResult)
		ReportErrorMessage("Error-GrabFocus Timer Failed",cEMailInspx, 32000);

	SetupMenu(vLocalCWndCollection);

	TimerResult = SetTimer(vClearStartingIndicatorIfXRaysOnTimerHandle,60000,NULL);
	if (!TimerResult)
		ReportErrorMessage("Error-vClearStartingIndicatorIfXRaysOnTimerHandle Timer Failed",cEMailInspx, 32000);

	if (vEditOnlyThisInspection)
		vAnyChangeMade = false;
}

BOOL CAutoSetupImageDisplayDialog::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
DeleteObject(vLightGrayBrush);
return CDialog::DestroyWindow();
}

void CAutoSetupImageDisplayDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAutoSetupImageDisplayDialog)
	DDX_Control(pDX, IDC_DialogTitleStaticText1, m_DialogTitleStaticText);
	DDX_Control(pDX, IDC_RejectReason, m_RejectReason);
	DDX_Control(pDX, IDC_DensityNotLockedIndicator, m_DensityNotLockedIndicator);
	DDX_Control(pDX, IDC_Function1Button, m_Function1Button);
	DDX_Control(pDX, IDC_SubFunction4Display, m_SubFunction4Display);
	DDX_Control(pDX, IDC_CurrentDensity, m_CurrentDensity);
	DDX_Control(pDX, IDC_AverageDensity, m_AverageDensity);
	DDX_Control(pDX, IDC_SizeY, m_SizeY);
	DDX_Control(pDX, IDC_SizeX, m_SizeX);
	DDX_Control(pDX, IDC_SizeLabelY, m_SizeLabelY);
	DDX_Control(pDX, IDC_SizeLabelX, m_SizeLabelX);
	DDX_Control(pDX, IDC_XScale2, m_XScale2);
	DDX_Control(pDX, IDC_XScaleLabel, m_XScaleLabel);
	DDX_Control(pDX, IDC_YScale2, m_YScale2);
	DDX_Control(pDX, IDC_YScaleLabel, m_YScaleLabel);
	DDX_Control(pDX, IDC_Function2Button, m_Function2Button);
	DDX_Control(pDX, IDC_Function3Button, m_Function3Button);
	DDX_Control(pDX, IDC_SubFunction8Button3, m_SubFunction8Button3);
	DDX_Control(pDX, IDC_SubFunction8Button2, m_SubFunction8Button2);
	DDX_Control(pDX, IDC_SubFunction8Button1, m_SubFunction8Button1);
	DDX_Control(pDX, IDC_SubFunction6Button3, m_SubFunction6Button3);
	DDX_Control(pDX, IDC_SubFunction6Button2, m_SubFunction6Button2);
	DDX_Control(pDX, IDC_SubFunction6Button1, m_SubFunction6Button1);
	DDX_Control(pDX, IDC_Calibrating, m_Calibrating);
	DDX_Control(pDX, IDC_SubFunction2Display, m_SubFunction2Display);
	DDX_Control(pDX, IDC_Reference, m_Reference);
	DDX_Control(pDX, IDC_SubFunction3Display, m_SubFunction3Display);
	DDX_Control(pDX, IDC_SubFunction1Display, m_SubFunction1Display);
	DDX_Control(pDX, IDC_ThresholdForContainerBounds2, m_ThresholdForContainerBounds2);
	DDX_Control(pDX, IDC_Width2, m_Width2);
	DDX_Control(pDX, IDC_CurrentLabel, m_CurrentLabel);
	DDX_Control(pDX, IDC_Background, m_Background);
	DDX_Control(pDX, IDC_Function5Button, m_Function5Button);
	DDX_Control(pDX, IDC_SubFunction8Button, m_SubFunction8Button);
	DDX_Control(pDX, IDC_SubFunction7Button, m_SubFunction7Button);
	DDX_Control(pDX, IDC_SubFunction6Button, m_SubFunction6Button);
	DDX_Control(pDX, IDC_SubFunction5Button, m_SubFunction5Button);
	DDX_Control(pDX, IDC_SubFunction8Display, m_SubFunction8Display);
	DDX_Control(pDX, IDC_SubFunction4Button, m_SubFunction4Button);
	DDX_Control(pDX, IDC_SubFunction3Button, m_SubFunction3Button);
	DDX_Control(pDX, IDC_SubFunction2Button, m_SubFunction2Button);
	DDX_Control(pDX, IDC_SubFunction1Button, m_SubFunction1Button);
	DDX_Control(pDX, IDC_SubFunction4DisplayA, m_SubFunction4DisplayA);
	DDX_Control(pDX, IDC_Function4Button, m_Function4Button);
	DDX_Control(pDX, IDC_SubFunction5Display, m_SubFunction5Display);
	DDX_Control(pDX, IDC_SubFunction7Display, m_SubFunction7Display);
	DDX_Control(pDX, IDC_SubFunction6Display, m_SubFunction6Display);
	DDX_Control(pDX, IDC_XRaysOn, m_XRaysOn);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_PleaseWait, m_PleaseWait);
	DDX_Control(pDX, IDC_Simulating, m_Simulating);
	DDX_Control(pDX, IDC_StatusLine, m_StatusLine);
	DDX_Control(pDX, IDC_StatusLine2, m_StatusLine2);
}

BEGIN_MESSAGE_MAP(CAutoSetupImageDisplayDialog, CDialog)
	//{{AFX_MSG_MAP(CAutoSetupImageDisplayDialog)
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
	ON_WM_CTLCOLOR()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_SubFunction8Button3, OnSubFunction8Button3)
	ON_BN_CLICKED(IDC_SubFunction8Button2, OnSubFunction8Button2)
	ON_BN_CLICKED(IDC_SubFunction8Button1, OnSubFunction8Button1)
	ON_BN_CLICKED(IDC_SubFunction6Button3, OnSubFunction6Button3)
	ON_BN_CLICKED(IDC_SubFunction6Button2, OnSubFunction6Button2)
	ON_BN_CLICKED(IDC_SubFunction6Button1, OnSubFunction6Button1)
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_DialogTitleStaticText, OnDialogTitleStaticText)
	//}}AFX_MSG_MAP
	ON_STN_CLICKED(IDC_DialogTitleStaticText1, &CAutoSetupImageDisplayDialog::OnDialogtitlestatictext1)
	ON_STN_DBLCLK(IDC_SubFunction2Button, &CAutoSetupImageDisplayDialog::OnSubFunction2ButtonDoubleClicked)
	ON_STN_DBLCLK(IDC_SubFunction1Button, &CAutoSetupImageDisplayDialog::OnSubFunction1ButtonDoubleClicked)
	ON_STN_DBLCLK(IDC_SubFunction3Button, &CAutoSetupImageDisplayDialog::OnSubFunction3ButtonDoubleClicked)
	ON_STN_DBLCLK(IDC_SubFunction4Button, &CAutoSetupImageDisplayDialog::OnSubFunction4ButtonDoubleClicked)
	ON_STN_DBLCLK(IDC_SubFunction5Button, &CAutoSetupImageDisplayDialog::OnSubFunction5ButtonDoubleClicked)
	ON_STN_DBLCLK(IDC_SubFunction6Button, &CAutoSetupImageDisplayDialog::OnSubFunction6ButtonDoubleClicked)
	ON_STN_DBLCLK(IDC_SubFunction7Button, &CAutoSetupImageDisplayDialog::OnSubFunction7ButtonDoubleClicked)
	ON_STN_DBLCLK(IDC_SubFunction8Button, &CAutoSetupImageDisplayDialog::OnSubFunction8ButtonDoubleClicked)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAutoSetupImageDisplayDialog message handlers

void CAutoSetupImageDisplayDialog::OnFunction1Button() 
{
	if (vInitialized)
	if (vMode == CMultiLanePositionAdjustment)
	{
		FinishMultiLanePositionMenu();
		StartSizeMenu();
	}
	else
	if ((vMode == cSetROI) && (vEditOnlyThisInspection))
	{
		ReportErrorMessage(" Operator Aborted Setup",cUserAction,0);
		
		if (MyAbortSetupChanges())
			ExitAutoSetupDialog(10);
	}
	else
	if ((vMode == cSetROI) && (vReturnToSetSource))
	{
		//if operator edited Product Density ROI do not let software reset it automatically later after learn
		vLocalSystemData->vChangedSizeOfProductCheckDensityROI = 0;
		FinishROI(false);
		StartCenterExposureMenu();
	}
	else
	if (vMode == cSetSizeSettings)
	{
		FinishSetSizeSettings();
		StartSizeMenu();
	}
	else
	if ((vMode == cSetContaminantThreshold) || (vMode == cSetVoidThreshold))
	{
		if ((vInspectionEditing) && (vInspectionEditing->vInspectionType == cItemCountInspection))  //count items inspection settings
		{
			//Create dialog box
			CConfigureProductContaminantProcessesDialog IConfigureProductContaminantProcessesDialog;  
			//Set dialog box data like how many products there are
			IConfigureProductContaminantProcessesDialog.vPointerToProductEditing = vGlobalCurrentProduct;
			IConfigureProductContaminantProcessesDialog.vInspectionEditing = vInspectionEditing;
			IConfigureProductContaminantProcessesDialog.vLocalSystemData = vLocalSystemData;
			IConfigureProductContaminantProcessesDialog.vLocalConfigurationData = vLocalConfigurationData;
			IConfigureProductContaminantProcessesDialog.vLocalSystemData = vLocalSystemData;
			//Pass control to dialog box and display
			//IEditProductDialog.vEPLocalNumberOfProducts = vCPLocalNumberOfProducts;
			int nResponse = IConfigureProductContaminantProcessesDialog.DoModal();
			//dialog box is now closed, if user pressed select do this
			//if user pressed cancel, do nothing
			if (nResponse == IDOK)
			{
				if (IConfigureProductContaminantProcessesDialog.vChangeMade)
					vAnyChangeMade = true;
			}
			else 
			if (nResponse == 10)
			if (MyAbortSetupChanges())
				//  Main Menu button was pressed
				ExitAutoSetupDialog(10);
		}
		else
		if ((vInspectionEditing) && ((vInspectionEditing->vInspectionType == cSizeInspection) || (vInspectionEditing->vInspectionType == cMissingLidInspectionOnSize) || 
			(vInspectionEditing->vInspectionType == cFloodedInspection)))
		{
			//edit inspection size
			CNumericEntryDialog INumericEntryDialog;  
				
			//Set dialog box data titles and number value
			INumericEntryDialog.vEditString = dtoa(vInspectionEditing->vSize,0);

			INumericEntryDialog.m_DialogTitleStaticText1 = "Enter Minimum Acceptable";
			CString TempText("");

			if (vInspectionEditing->vInspectionType == cFloodedInspection)
				INumericEntryDialog.m_DialogTitleStaticText2 = "Spout Length for Inspection: " + vInspectionEditing->vName;
			else
				INumericEntryDialog.m_DialogTitleStaticText2 = "Size Required for Inspection: " + vInspectionEditing->vName;
			TempText.LoadString(IDS_Product);
			INumericEntryDialog.m_DialogTitleStaticText3 = TempText + ": " + vProductEditingName;
			TempText.LoadString(IDS_OriginalValue);
			INumericEntryDialog.m_DialogTitleStaticText4 = TempText + ": " + INumericEntryDialog.vEditString;

			INumericEntryDialog.m_UnitsString = "Measured in Pixels";
			INumericEntryDialog.vIntegerOnly = true;
			INumericEntryDialog.vMaxValue = 9000000;
			INumericEntryDialog.vMinValue = 0;
			//Pass control to dialog box and display
			int nResponse = INumericEntryDialog.DoModal();
			//dialog box is now closed, if user pressed select do this
			//if user pressed cancel, do nothing
			if (nResponse == IDOK)
			{
				vLocalSystemData->vManuallyEditedDuringEvaluate = true;
				vInspectionEditing->vSize = _wtoi(INumericEntryDialog.vEditString);
				SetDlgItemText(IDC_Function1Button, "Size\n" + dtoa(vInspectionEditing->vSize, 0));
				SetChangeMade();
			}
			else 
			if (nResponse == 10)
			{
				//Main Menu button pressed
				if (MyAbortSetupChanges())
					ExitAutoSetupDialog(10);
			}
		}
		else
		if (AskIfAbortAutoSetupDialog())
		//if ((!vChangeMade) || (AbortSetupChanges()))
		if (MyAbortSetupChanges())
			ExitAutoSetupDialog(10);
	}
	else
	if (vMode == cSetDensityThreshold)
	{
		if ((vInspectionEditing->vInspectionType == cUnderfillByWeightInspection) || (vInspectionEditing->vInspectionType == cOverfillByWeightInspection))
		{
			AskForWeightTrendingInspectionToLinkThisOneTo(vInspectionNumberEditing);
			vGlobalCurrentProduct->MatchInspectionsForWeightTrending();
			FinishDensityThreshold(false);
			StartDensityThreshold();
		}
		else
		if (vEditOnlyThisInspection)
		{
			if (MyAbortSetupChanges())
				ExitAutoSetupDialog(10);
		}
		else
		if (AskIfAbortAutoSetupDialog())
		//if ((!vChangeMade) || (AbortSetupChanges()))
		if (MyAbortSetupChanges())
			ExitAutoSetupDialog(10);
	}
	else
	if ((!((vOldAverageEnabled) || (vOldStructureEnabled) || (vOldTakingQuickLearnImages) ||
		(vOldCalibrateBodyTriggerReferenceEnabled) || (vOldAdjustingBrightnessBeforeLearning))) ||
		(!vLocalSystemData->vInAutoSetup))
	{
		if (((!vLocalSystemData->vXRayOnNotice) || (vStartingUpSource) || (vRestartingXRaysWithNewSettings) ||
			((vLocalSystemData->vSystemRunMode != cRunningSystemMode) && (vLocalSystemData->vSystemRunMode != cAutoSetupRunningSystem))) &&
			(vMainWindowPointer->InterlocksOK()) && (vStartedSource))
			StartingSystemPleaseTryAgain();
		else
		if (!((vMode == cSetROI) && 
			(vInspectionEditing == vGlobalCurrentProduct->vDriftDensityInspection)))
		if (AskIfAbortAutoSetupDialog())
		//if ((!vChangeMade) || (AbortSetupChanges()))
		if (MyAbortSetupChanges())
			ExitAutoSetupDialog(10);
	}
	else
	if (vMode != cInitializing)
	{
		CNoticeDialog TempNoticeDialog;
		CString TempText;
		TempText.LoadString(IDS_YoucannotexitwhileLearning);//"\n\n\nYou cannot exit while Learning"
		TempNoticeDialog.vNoticeText = TempText;
		TempNoticeDialog.vType = cErrorMessage;
		TempNoticeDialog.DoModal();
	}
}

bool CAutoSetupImageDisplayDialog::MyAbortSetupChanges() 
{
	if (!vAnyChangeMade)
		return true;

	CYesNoDialog TempYesNoDialog;
	CString TempText;
	if ((vAnyChangeMade) || (vAutoSetupType == cNewAutoSetup) || 
		(vAutoSetupType ==cCopyAutoSetup))
	{
		TempText.LoadString(IDS_AbandonSetupChangesandExit);//"\n\nAbandon Setup Changes and Exit?"
		TempYesNoDialog.vNoticeText = TempText;
		TempYesNoDialog.vQuestionType = cConfirmAbandon;
	}
	else
	if ((vOldAverageEnabled) || (vOldStructureEnabled) || (vOldTakingQuickLearnImages) ||
		(vOldCalibrateBodyTriggerReferenceEnabled) || (vOldAdjustingBrightnessBeforeLearning))
	{
		//TempText.LoadString(IDS_AbandonSetupChangesandExit);//"\n\nAbandon Setup Changes and Exit?"
		TempYesNoDialog.vNoticeText = _T("\n\n\nAbort Learn and Exit?");
		TempYesNoDialog.vYesButtonText = "Abort Learn";
		TempYesNoDialog.vQuestionType = cConfirmAbandon;
	}
	else
	if (vEditOnlyThisInspection)
	{
		TempYesNoDialog.vNoticeText = _T("\n\n\nAbandon Changes and Exit?");
		TempYesNoDialog.vQuestionType = cConfirmAbandon;
	}
	else
	{
		TempText.LoadString(IDS_ExitAutoSetup);//"\n\nExit Auto Setup?"
		TempYesNoDialog.vNoticeText = TempText;
		TempYesNoDialog.vQuestionType = cConfirmQuestion;
	}
	int TempResult = TempYesNoDialog.DoModal();
	if (TempResult == IDOK)
		return true;
	else
		return false;
}

void CAutoSetupImageDisplayDialog::OnFunction2Button() 
{
	//vGlobalMouseMoveCounter = 0;
//	if (vFunction2ButtonEnable == true)
//	{
	if (vInitialized)
	if (vMode == cSetSizeSettings)
	{
		vGlobalCurrentProduct->vBulkProductMode = !vGlobalCurrentProduct->vBulkProductMode;
		SetChangeMade();

		CNoticeDialog TempNoticeDialog;
		if (vGlobalCurrentProduct->vBulkProductMode)
		{
			TempNoticeDialog.vNoticeText = "\nYou have just put this product into Bulk Mode.\nIt does not use the Container Trigger.\nIt triggers automatically when the encoder is running to take images continuously.";
			vGlobalCurrentProduct->SetProductImageWidth(1);
		}
		else
			TempNoticeDialog.vNoticeText = "\nYou have just turned off Bulk Mode for this product.\nIt will use the Container Trigger.\nThere must be space between the containers for the trigger to detect each container.";
		TempNoticeDialog.vType = cInformationMessage;
		TempNoticeDialog.DoModal();

		vLocalSystemData->vITIPCDig->RenameLearnFilesToBackup(vGlobalCurrentProduct);
		vLocalSystemData->vITIPCDig->DeleteLearnImages();
		vLocalSystemData->vITIPCDig->DeleteLearnFiles(vGlobalCurrentProduct);
		ClearDensityFeedBackLoop();
		ChangedProductSettingsSoReload();
		UpdateButtonText();

		//if (PasswordOK(cTemporaryInspxPassword,false))
		//{
		//	//Maximum Image Overlap Button Was Pressed
		//	CNumericEntryDialog INumericEntryDialog;  
		//	
		//	//Set dialog box data titles and number value
		//	INumericEntryDialog.vEditString = dtoa(vGlobalCurrentProduct->vMaximumImageOverlap,2);

		//	INumericEntryDialog.m_DialogTitleStaticText1 = "Maximum X-Ray Image Overlap";
		//	INumericEntryDialog.m_DialogTitleStaticText2 = "Product: " + *vGlobalCurrentProduct->GetProductName();
		//	INumericEntryDialog.m_DialogTitleStaticText3 = "Original Value: " + INumericEntryDialog.vEditString;
		//	INumericEntryDialog.m_DialogTitleStaticText4 = "Default Value: half of image width";

		//	INumericEntryDialog.m_UnitsString = "Measured in " + vLocalConfigurationData->vUnitsString;
		//	double TempMaximum = vGlobalCurrentProduct->vProductImageWidth;
		//	//if (vLocalConfigurationData->vUseMetric)
		//	//	INumericEntryDialog.vMaxValue = TempMaximum * (float)25.4;
		//	//else
		//		INumericEntryDialog.vMaxValue = TempMaximum;
		//	INumericEntryDialog.vIntegerOnly = false;
		//	//Pass control to dialog box and display
		//	int nResponse = INumericEntryDialog.DoModal();
		//	//dialog box is now closed, if user pressed select do this
		//	//if user pressed cancel, do nothing
		//	if (nResponse == IDOK)
		//	{
		//		vGlobalCurrentProduct->vMaximumImageOverlap = ATOF(INumericEntryDialog.vEditString);

		//		SetChangeMade();
		//		SetDlgItemText(IDC_SubFunction7Display,dtoa(vGlobalCurrentProduct->vMaximumImageOverlap,2));

		//		vLocalSystemData->vITIPCDig->vMaximumImageOverlap = (WORD)(vGlobalCurrentProduct->vMaximumImageOverlap * vGlobalPixelsPerUnit * vGlobalCurrentProduct->vOverScanMultiplier);
		//	}
		//	else 
		//	if (nResponse == 10)
		//	{
		//		//Main Menu button pressed
		//		ExitAutoSetupDialog(10);
		//	}
		//}
	}
	else
	if ((vMode == cLearnMenu) || (vMode == cSetImageSize) || (vMode == cCenterExposureMenu) || //(vMode == cEdgeExposureMenu) ||
		(vMode == cSetBackground))
	{
		if ((!((vOldAverageEnabled) || (vOldStructureEnabled) || (vOldTakingQuickLearnImages) ||
			(vOldCalibrateBodyTriggerReferenceEnabled) || (vOldAdjustingBrightnessBeforeLearning))) ||
			(!vLocalSystemData->vInAutoSetup))
		{
			if (((!vLocalSystemData->vXRayOnNotice) || (vStartingUpSource) || (vRestartingXRaysWithNewSettings) ||
				((vLocalSystemData->vSystemRunMode != cRunningSystemMode) && (vLocalSystemData->vSystemRunMode != cAutoSetupRunningSystem))) &&
				(vMainWindowPointer->InterlocksOK()) && (vStartedSource))
				StartingSystemPleaseTryAgain();
			else
			if ((!vAnyChangeMade) || (KeepSetupChanges()))
			{
				ExitAutoSetupDialog(20);
			}
		}
		else
		{
			CNoticeDialog TempNoticeDialog;
			CString TempText("");
			TempText.LoadString(IDS_YoucannotexitwhileLearning);
			TempNoticeDialog.vNoticeText = TempText;
			TempNoticeDialog.vType = cErrorMessage;
			TempNoticeDialog.DoModal();
		}
	}
	else
	if (vMode == cSetROI)
	{
		if (!vReturnToSetSource)
		if (vInspectionEditing)
		{
			//Inspection name
			CAlphaEntryDialog IAlphaEntryDialog;  
			IAlphaEntryDialog.vLocalSystemData = vLocalSystemData;
			//Set dialog box data titles and number value
			IAlphaEntryDialog.vEditString = vInspectionEditing->vName;				
			bool TempNameADuplicate = true;

			while (TempNameADuplicate)
			{
				IAlphaEntryDialog.m_DialogTitleStaticText = "Inspection Name for Type: " + vInspectionEditing->GetInspectionTypeString();
				IAlphaEntryDialog.m_DialogTitleStaticText2 = "Original Value: " + IAlphaEntryDialog.vEditString;
				IAlphaEntryDialog.vAllowDashes = true;
				IAlphaEntryDialog.vMaximumNumberOfCharacters = 20;
				IAlphaEntryDialog.vMainWindowPointer = vMainWindowPointer;
				//Pass control to dialog box and display
				int nResponse = IAlphaEntryDialog.DoModal();
				//dialog box is now closed, if user pressed select do this
				//if user pressed cancel, do nothing
				if (nResponse == IDOK)
				{
					CString TempNewNameInUpperCase = IAlphaEntryDialog.vEditString;
					TempNewNameInUpperCase.MakeUpper();

					TempNameADuplicate = false;
					for (BYTE TempLoop = 0; TempLoop < vGlobalCurrentProduct->vNumberOfInspections; TempLoop++)
					if (TempLoop != vInspectionNumberEditing)
					if (vGlobalCurrentProduct->vInspection[TempLoop])
					{
						CString TempInspectionNameInUpperCase = vGlobalCurrentProduct->vInspection[TempLoop]->vName;
						TempInspectionNameInUpperCase.MakeUpper();
						if (TempInspectionNameInUpperCase == TempNewNameInUpperCase)
							TempNameADuplicate = true;
					}

					if (!TempNameADuplicate)
					{
						CString TempOriginalName = vInspectionEditing->vName;

						vInspectionEditing->vName = IAlphaEntryDialog.vEditString;
						vAnyChangeMade = true;
						vMainWindowPointer->SetupRejectReasonNames();

						//if any linked weight trending inspections to this one, then change the name of the link
						if ((vInspectionEditing->vInspectionType == cUnderfillByWeightInspection) || (vInspectionEditing->vInspectionType == cOverfillByWeightInspection))
						if (vGlobalCurrentProduct->AreThereAnyOtherWeightTrendingInspections(vInspectionNumberEditing))
						{
							for (BYTE TempLoop = 0; TempLoop < vGlobalCurrentProduct->vNumberOfInspections; TempLoop++)
							if (TempLoop != vInspectionNumberEditing)
							if (vGlobalCurrentProduct->vInspection[TempLoop])
							if ((vGlobalCurrentProduct->vInspection[TempLoop]->vInspectionType == cUnderfillByWeightInspection) || (vGlobalCurrentProduct->vInspection[TempLoop]->vInspectionType == cOverfillByWeightInspection))
							{
								if (vGlobalCurrentProduct->vInspection[TempLoop]->vInspectionNameForWeightTrendingName == TempOriginalName) //if pointed to this inspections old name
									vGlobalCurrentProduct->vInspection[TempLoop]->vInspectionNameForWeightTrendingName = vInspectionEditing->vName; //set to point to this inspections new name
							}
						}
					}
					else
					{
						CNoticeDialog TempNoticeDialog;
						TempNoticeDialog.vNoticeText =  "\n\nThis product already has an inspection with that name.\nPlease enter a new inspection name.";
						TempNoticeDialog.vType = cErrorMessage;
						TempNoticeDialog.DoModal();
					}
				}
				else 
				if (nResponse == 10)
				{
					TempNameADuplicate = false;
					ExitAutoSetupDialog(10);
				}
				else
				if (nResponse == IDCANCEL)
				{
					//Cancel button pressed
					TempNameADuplicate = false;
				}
			}
		}
	}
	else //OnFunction2Button
	if ((vMode == cSetContaminantThreshold) || (vMode == cSetVoidThreshold) || (vMode == cSetDensityThreshold))
	{
		if (vInspectionEditing->vInspectionType != cItemCountInspection)
		if (!vEditOnlyThisInspection)
		if (vInspectionEditing)
		{
			if ((vInspectionEditing->vInspectionType == cCheckWeighInspection) || (vInspectionEditing->vInspectionType == cUnderfillByWeightInspection) || (vInspectionEditing->vInspectionType == cOverfillByWeightInspection))
			{
				if (vInspectionEditing->vInspectionNameForWeightTrendingPointer) //if wieght trending inspection, use other inspections settings
					GiveLinkedInspectionNotice();
				else
				//if (DeleteCheckWeighCalibrations(false))
				{
					if (vInspectionEditing->vCheckWeighUnits == "Ounces")
						vInspectionEditing->vCheckWeighUnits = "Grams";
					else
						vInspectionEditing->vCheckWeighUnits = "Ounces";
					UpdateButtonText();
					SetChangeMade();
					DisplayDensityThresholdUpper();
					DisplayDensityThresholdLower();
				}
			}
			else
			if ((vInspectionEditing->vAllowChangingAdvancedOptions) || (PasswordOK(cSuperInspxPassword,false)))
			{
				//setup X-Ray Detectors Contaminant or void Processing or imaging
				//Create dialog box

				CConfigureProductContaminantProcessesDialog IConfigureProductContaminantProcessesDialog;  
				
				//Set dialog box data like how many products there are
				IConfigureProductContaminantProcessesDialog.vPointerToProductEditing = vGlobalCurrentProduct;
				IConfigureProductContaminantProcessesDialog.vInspectionEditing = vInspectionEditing;
				IConfigureProductContaminantProcessesDialog.vLocalSystemData = vLocalSystemData;
				IConfigureProductContaminantProcessesDialog.vLocalConfigurationData = vLocalConfigurationData;
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
						vAnyChangeMade = true;
						vMainWindowPointer->SetupRejectReasonNames();

						if ((vInspectionEditing) && ((vInspectionEditing->vInspectionType == cSizeInspection) || (vInspectionEditing->vInspectionType == cMissingLidInspectionOnSize)))
							SetDlgItemText(IDC_Function1Button, "Size\n" + dtoa(vInspectionEditing->vSize, 0));
					}
				}
				else 
				if (nResponse == 10)
				if (MyAbortSetupChanges())
					//  Main Menu button was pressed
					ExitAutoSetupDialog(10);
			}
			else
			if (((vInspectionEditing->vHasAMinimumSize) && (PasswordOK(cTemporaryInspxPassword,false)) && 
				(vInspectionEditing->vInspectionType != cItemCountInspection) && (vInspectionEditing->vInspectionType != cSizeInspection) && (vInspectionEditing->vInspectionType != cMissingLidInspectionOnSize)) || (vInspectionEditing->vInspectionType == cFloodedInspection))
			{
				//edit minimum size
				CNumericEntryDialog INumericEntryDialog;  
				
				//Set dialog box data titles and number value
				INumericEntryDialog.vEditString = dtoa(vInspectionEditing->vSize,0);

				INumericEntryDialog.m_DialogTitleStaticText1 = "";
				CString TempText("");
				if (vInspectionEditing->vInspectionType == cFloodedInspection)
					INumericEntryDialog.m_DialogTitleStaticText2 = "Enter Minimum Spout Length Inspection: " + vInspectionEditing->vName;
				else
					INumericEntryDialog.m_DialogTitleStaticText2 = "Enter Minimum Size for Inspection: " + vInspectionEditing->vName;
				TempText.LoadString(IDS_Product);
				INumericEntryDialog.m_DialogTitleStaticText3 = TempText + ": " + vProductEditingName;
				TempText.LoadString(IDS_OriginalValue);
				INumericEntryDialog.m_DialogTitleStaticText4 = TempText + ": " + INumericEntryDialog.vEditString;

				INumericEntryDialog.m_UnitsString = "Measured in Pixels";
				INumericEntryDialog.vIntegerOnly = true;
				INumericEntryDialog.vMaxValue = 9000000;
				INumericEntryDialog.vMinValue = 1;
				//Pass control to dialog box and display
				int nResponse = INumericEntryDialog.DoModal();
				//dialog box is now closed, if user pressed select do this
				//if user pressed cancel, do nothing
				if (nResponse == IDOK)
				{
					vInspectionEditing->vSize = _wtoi(INumericEntryDialog.vEditString);

					if ((vInspectionEditing) && ((vInspectionEditing->vInspectionType == cSizeInspection) || (vInspectionEditing->vInspectionType == cMissingLidInspectionOnSize)))
						SetDlgItemText(IDC_Function1Button, "Size\n" + dtoa(vInspectionEditing->vSize, 0));

					if ((vInspectionEditing->vInspectionType == cDripInspection) || (vInspectionEditing->vInspectionType == cDripInspection))
					{
						//Set dialog box data titles and number value For MAXIMUM size
						INumericEntryDialog.vEditString = dtoa(vInspectionEditing->vMaximumSize, 0);
						INumericEntryDialog.m_DialogTitleStaticText1 = "Maximum size of Contaminant or foreign matter";
						INumericEntryDialog.m_DialogTitleStaticText2 = "Above this size is considered normal product";
						INumericEntryDialog.vMaxValue = 9000000;
						INumericEntryDialog.vMinValue = vInspectionEditing->vSize + 2;
						INumericEntryDialog.vAllowZero = true;
						INumericEntryDialog.vIntegerOnly = true;

						//Pass control to dialog box and display
						int nResponse = INumericEntryDialog.DoModal();
						//dialog box is now closed, if user pressed select do this
						//if user pressed cancel, do nothing
						if (nResponse == IDOK)
						{
							vInspectionEditing->vMaximumSize = _wtoi(INumericEntryDialog.vEditString);

							INumericEntryDialog.vEditString = dtoa(vInspectionEditing->vMinimumWidth, 0);
							INumericEntryDialog.m_DialogTitleStaticText1 = "Minimum Width of Contaminant or foreign matter";
							INumericEntryDialog.m_DialogTitleStaticText2 = "Above this width is considered defective";
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
								vInspectionEditing->vMinimumWidth = _wtoi(INumericEntryDialog.vEditString);
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
				{
					//Main Menu button pressed
					ExitAutoSetupDialog(10);
				}
			}
		}
	}
	/*
	else
	if (vMode == cSetROI)
	{  
		vLocalConfigurationData->vReferenceROIsToEdges = 
			!vLocalConfigurationData->vReferenceROIsToEdges;
		UpdateAverageDensityDisplay();
		UpdateButtonText();
		DrawImageOnScreen();
	}
	*/
//	}
//	else
//	{
//		ShowNoMouseClickDialogMessage();
//	}
}

void CAutoSetupImageDisplayDialog::OnFunction3Button() 
{
	//vGlobalMouseMoveCounter = 0;
//	if (vFunction3ButtonEnable == true)
//	{
	if (vInitialized)
	if (vLocalSystemData->vYellowMessageButtonYellow)
	{  //clear yellow error message
		ClearYellowWarningButton();
		for (BYTE TempLoop = 0; TempLoop < 2; TempLoop++)
		if (vLocalSystemData->vGaveTooManyRejectsAlarm[TempLoop])
		{//turn alarm output bit off
			vLocalSystemData->vGaveTooManyRejectsAlarm[TempLoop] = false;
		}
		if (vGlobaluCSerialPort)
		if (vLocalSystemData->vCurrentAlarmStatus)
			vGlobaluCSerialPort->TurnAlarmOnOff(1,false);  //turns alarms 1,2,3 off
	}
	else
	if ((vMode != cInitializing) && ((!vStartingUpSource) && (!vRestartingXRaysWithNewSettings) &&
		((!vLocalSystemData->vXRayOnNotice) || (vLocalSystemData->vSystemRunMode == cStoppedSystemMode))))
	{  //run system if not running
		if (vMainWindowPointer->InterlocksOK())
		{
			int TimerResult = SetTimer(vStartTimerHandle,20,NULL);
			if (!TimerResult)
				ReportErrorMessage("Error-Start Timer Failed",cEMailInspx,32000);
			vStartingUpSource = true;
			UpdateButtonText();
			m_Calibrating.ShowWindow(SW_SHOW);

			m_PleaseWait.ShowWindow(SW_SHOW);
			KillTimer(vClearStartingIndicatorIfXRaysOnTimerHandle);
			TimerResult = SetTimer(vClearStartingIndicatorIfXRaysOnTimerHandle,60000,NULL);
			if (!TimerResult)
				ReportErrorMessage("Error-vClearStartingIndicatorIfXRaysOnTimerHandle Timer Failed",cEMailInspx, 32000);
		}
	}
	else //OnFunction3Button
	if (((vMode == cSetContaminantThreshold) || (vMode == cSetVoidThreshold) || (vMode == cSetDensityThreshold)) && (vNumberEjectorsConfigured > 1) && (!vReturnToSetSource))
	{
		if (!vEditOnlyThisInspection)
			SelectInspectionEjectors(true);
	}
	else
	if (vHaveAnAutoThresholdValue)
	{
		//User pressed Auto Threshold Button
		if (vMode == cSetDensityThreshold)
		{
			if ((vInspectionEditing->vInspectionType != cUnderfillByWeightInspection) && (vInspectionEditing->vInspectionType != cOverfillByWeightInspection) && (vInspectionEditing->vInspectionType != cCheckWeighInspection))
			{
				vInspectionEditing->vDensityThresholdUpper = vInspectionEditing->vSetupHistoryDensityAutoThresholdUpper; //don't set threshold on check weighing
				DisplayDensityThresholdUpper();
				vInspectionEditing->SetDensityLowerThreshold(vInspectionEditing->vSetupHistoryDensityAutoThresholdLower);
				DisplayDensityThresholdLower();
			}
		}
		else
		{
			vInspectionEditing->vThreshold = vInspectionEditing->vSetupHistoryAutoThreshold;
			SetDlgItemText(IDC_ThresholdForContainerBounds2, dtoa(vInspectionEditing->vThreshold,0));
			ShowSensitivity();
		}
		vHaveAnAutoThresholdValue = false;
	}
	UpdateFunction3Button();
//	}
//	else
//	{
//		ShowNoMouseClickDialogMessage();
//	}
}

void CAutoSetupImageDisplayDialog::OnFunction4Button() 
{
	//vGlobalMouseMoveCounter = 0;
//	if (vFunction4ButtonEnable == true)
//	{
	if (vInitialized)
	if (vMode == cSetSizeSettings)
	{
		if (PasswordOK(cTemporaryInspxPassword,false))
		{
			if (!vLocalSystemData->vInAutoSetup)
			{
				CNoticeDialog TempNoticeDialog;
				TempNoticeDialog.vNoticeText = "\nYou cannot change this setting because\nScanTrac is inspecting containers.\nYou must exit setup, stop running,\nthen enter Setup Product to change this setting.";
				TempNoticeDialog.vType = cErrorMessage;
				TempNoticeDialog.DoModal();
			}
			else
			if (vLocalConfigurationData->vEncoderDivider % 2 != 0)// % is mod function  //if not divisible by 2, then can't do
			{
				CNoticeDialog TempNoticeDialog;
				TempNoticeDialog.vNoticeText = "\nYou cannot use Overscan because the\nEncoder Divider factor is not divisible by two.\nYou must re-calibrate the encoder with the divider divisible by 2.";
				TempNoticeDialog.vType = cErrorMessage;
				TempNoticeDialog.DoModal();
				vGlobalCurrentProduct->vOverScanMultiplier = 1;
				vGlobalCurrentProduct->vOverScanFactor = 0;
			}
			else
			{
				// Overscan button pressed
				bool TempOKToChange = true;

				if (vLocalConfigurationData->vEncoderDivider == 1)
				{
					CNoticeDialog TempNoticeDialog;
					TempNoticeDialog.vNoticeText = "\n\nOverScan is not available.\nEncoder Divider set to 1.\nOverScan works by reducing Encoder Divider."; 
					TempNoticeDialog.vType = cNoticeMessage;
					TempNoticeDialog.DoModal();

					TempOKToChange = false;
				}

				if (TempOKToChange)
				if (vGlobalCurrentProduct->ProductHasImageFiles(vGlobalCurrentDirectory))
				{
					CYesNoDialog TempYesNoDialog;
					TempYesNoDialog.vNoticeText = _T("This product has been Learned.\nIf you change the Overscan Factor,\nthe Learned Data will be deleted,\nand you will have to Learn again.\nDo you want to continue?");
					TempYesNoDialog.vQuestionType = cConfirmQuestion;
					TempYesNoDialog.vYesButtonText = "Delete Learn";
					TempYesNoDialog.vNoButtonText = "";
					int TempResult = TempYesNoDialog.DoModal();
					if (TempResult != IDOK)
						TempOKToChange = false;
				}

				if (TempOKToChange)
				{
					CString TempString;
					CSelectItemDialog ISelectItemDialog;  
					//Set dialog box data titles and number value
					ISelectItemDialog.vTitleString = "Choose Encoder Overscan Factor";
					ISelectItemDialog.m_DialogTitleStaticText2 = "";
					ISelectItemDialog.m_DialogTitleStaticText3 = "";
					ISelectItemDialog.vBackgroundColor = cGreen;
					ISelectItemDialog.vGreenItem = 1;
					ISelectItemDialog.vShowExit = 0;
					ISelectItemDialog.vInAutoSetup = vLocalSystemData->vInAutoSetup;

					ISelectItemDialog.vNumberOfItems = 3;
					ISelectItemDialog.vItems[0] = "1 X (Disabled)";
					ISelectItemDialog.vItems[1] = "2 X";
					ISelectItemDialog.vItems[2] = "4 X";
					if (vLocalConfigurationData->vEncoderDivider < 4)
					{
						ISelectItemDialog.vItems[2] = "4 X - Disabled - Encoder Divider < 4";
						ISelectItemDialog.vItemEnabled[2] = false;
					}

					ISelectItemDialog.vMakeTitle3Big = true;
					ISelectItemDialog.m_DialogTitleStaticText3 = "Original Value: " + ISelectItemDialog.vItems[vGlobalCurrentProduct->vOverScanFactor];

					//Pass control to dialog box and display
					int nResponse = ISelectItemDialog.DoModal();
					//dialog box is now closed, if user pressed select do this
					//if user pressed cancel, do nothing
					if (nResponse == IDOK)
					{
						BYTE TempOriginalOverScanMultiplier = vGlobalCurrentProduct->vOverScanMultiplier;

						BYTE TempOverScanFactor = ISelectItemDialog.vItemNumberSelected - 1;
						if (TempOverScanFactor != vGlobalCurrentProduct->vOverScanFactor)
						{
							//cannot change overscan factor if in edit while running mode
							//over scan multiplier set in dialog box
							vMainWindowPointer->StopImageAcquisition();
							vMainWindowPointer->StopRunning(false);
							vGlobalCurrentProduct->SetOverScanFactor(TempOverScanFactor);

							double TempMaximumWidth = cMaximumImageWidth;
							double TempMinimumWidth = 1;
							//if (vLocalConfigurationData->vUseMetric)
							//{
							//	TempMaximumWidth = cMaximumImageWidth * (float)25.4;
							//	TempMinimumWidth = 25.4;
							//}
							if (vGlobalCurrentProduct->vOverScanMultiplier > 1)
							{
								TempMaximumWidth = TempMaximumWidth / vGlobalCurrentProduct->vOverScanMultiplier;
								TempMinimumWidth = TempMinimumWidth / vGlobalCurrentProduct->vOverScanMultiplier;
							}
							if (vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac)  //on Trio, can only have 672 lines in the image
							{
								WORD TempNumberOfLines = (WORD)(vGlobalCurrentProduct->vProductImageWidth * vGlobalPixelsPerUnit * vGlobalCurrentProduct->vOverScanMultiplier);
								if (TempNumberOfLines > 672)
								{
									vGlobalCurrentProduct->vProductImageWidth =  (float)((672 / vGlobalPixelsPerUnit) / vGlobalCurrentProduct->vOverScanMultiplier);
									CNoticeDialog TempNoticeDialog;
									TempNoticeDialog.vNoticeText = "\n\n\nProduct Width too many lines.\nSet to: " + dtoa(vGlobalCurrentProduct->vProductImageWidth,2);
									TempNoticeDialog.vType = cNoticeMessage;
									TempNoticeDialog.DoModal();
								}
							}

							if (vGlobalCurrentProduct->vProductImageWidth > TempMaximumWidth)
							{
								vGlobalCurrentProduct->SetProductImageWidth((float)TempMaximumWidth);
								CNoticeDialog TempNoticeDialog;
								TempNoticeDialog.vNoticeText = "\n\n\nProduct Width set to: " + dtoa(TempMaximumWidth,2) + "\nMaximum allowed for this Overscan";
								TempNoticeDialog.vType = cNoticeMessage;
								TempNoticeDialog.DoModal();
							}
							if (vGlobalCurrentProduct->vProductImageWidth < TempMinimumWidth)
							{
								vGlobalCurrentProduct->SetProductImageWidth((float)TempMinimumWidth);
								CNoticeDialog TempNoticeDialog;
								TempNoticeDialog.vNoticeText = "\n\n\nProduct Width set to: " + dtoa(TempMinimumWidth,2) + "\nMinimum allowed for this Overscan";
								TempNoticeDialog.vType = cNoticeMessage;
								TempNoticeDialog.DoModal();
							}
							vGlobalCurrentProduct->SetProductImageWidth(vGlobalCurrentProduct->vProductImageWidth);

							//update distances that depend on Overscan Factor
							vGlobalCurrentProduct->SetEndOfLineTimeOut(vGlobalCurrentProduct->vEndOfLineTimeOut);
							vGlobalCurrentProduct->SetProductLockOutWidth(vGlobalCurrentProduct->vProductLockOutWidth);
							vGlobalCurrentProduct->SetProductBodyTriggerToImageBeltPositionOffset(vGlobalCurrentProduct->vProductBodyTriggerToImageDistanceInInches);
							for (BYTE TempLoop = 0; TempLoop < cNumberOfEjectors; TempLoop++)
								vGlobalCurrentProduct->SetEjectorBeltPositionOffset(TempLoop, vGlobalCurrentProduct->vEjectorDistanceFromTriggerInInches[TempLoop]);
							for (BYTE TempLoop = 0; TempLoop < cNumberOfExternalDetectors; TempLoop++)
							{
								vGlobalCurrentProduct->SetExternalDetectorWindowStart(TempLoop, vGlobalCurrentProduct->vExternalDetectorWindowStart[TempLoop]);
								vGlobalCurrentProduct->SetExternalDetectorWindowEnd(TempLoop, vGlobalCurrentProduct->vExternalDetectorWindowEnd[TempLoop]);
							}
							vGlobalCurrentProduct->CalculateEndOfLineTimeOut();

							if (TempOriginalOverScanMultiplier)
								vLocalSystemData->vITIPCDig->vMaximumImageOverlap = (WORD)(vGlobalCurrentProduct->vMaximumImageOverlap / TempOriginalOverScanMultiplier * vGlobalCurrentProduct->vOverScanMultiplier);

							if (vGlobaluCSerialPort)
								vGlobaluCSerialPort->SendEncoderDividerTouC();
							vLocalSystemData->vITIPCDig->RenameLearnFilesToBackup(vGlobalCurrentProduct);
							ClearReferenceLocationData();

							if (vLocalSystemData->vInAutoSetup)
							{
								vLocalSystemData->vChangedSizeOfProduct = true;
								vLocalSystemData->vChangedSizeOfProductCheckDensityROI = 1;
							}
							vGlobalCurrentProduct->CheckAllROIs(vLocalSystemData->vITIPCDig->vProductAverageImage);

							vLocalSystemData->vITIPCDig->DeleteLearnImages();
							vLocalSystemData->vITIPCDig->DeleteLearnFiles(vGlobalCurrentProduct);
							ClearDensityFeedBackLoop();

							if (vLocalSystemData->vLogFile.vLogSerialData)
								vLocalSystemData->vLogFile.WriteToLogFile("Changed Overscan, Reload Product",cDebugMessage);

							ChangedProductSettingsSoReload();  //image size changed so must reload
							vHaveAnImage = false;

							SetDlgItemText(IDC_SubFunction8Display,dtoa(vGlobalCurrentProduct->vOverScanMultiplier,0));
						}
						SetChangeMade();
					}
					else
					if (nResponse == 10)
					{
						//Main Menu button pressed
						ExitAutoSetupDialog(10);
					}
				}
			}
		}
	}
	else
	if (vMode == cSetImageSize)
	{
		if (vLocalSystemData->vInAutoSetup)
		if (PasswordOK(cTemporaryInspxPassword,false))
		{
			vGlobalInAutoSize = !vGlobalInAutoSize;
			vChangedEdgeFindingMode = true;

			UpdateButtonText();
			if (vGlobalInAutoSize)
				SetStatusLine(" Finding edges from right only");
			else
				SetStatusLine(" Finding edges from left and right");
		}
	}
	else
		/*
	if (vMode == cSetImageSize)
	{
		if (PasswordOK(cTemporaryInspxPassword,false))
		if (vLocalSystemData->vInLowExposureMode)
		if (vLocalSystemData->vRemoveBackgroundInAutoSize)
		if (vLocalSystemData->vITIPCDig->vProductBackgroundImage)
		{
			CYesNoDialog TempYesNoDialog;
			TempYesNoDialog.vNoticeText = "\n\nAre you sure you want to remove the\nLow Exposure Mode Background subtraction?";
			TempYesNoDialog.vYesButtonText = "Remove Background";
			TempYesNoDialog.vNoButtonText = "Leave Background";
			TempYesNoDialog.vQuestionType = cConfirmQuestion;
			int TempResult = TempYesNoDialog.DoModal();
			if (TempResult == IDOK)
			{
				vAskedIfShouldRemoveBackground = false;
				vLocalSystemData->vRemoveBackgroundInAutoSize = false;
				delete vLocalSystemData->vITIPCDig->vProductBackgroundImage;

				CProduct *TempProduct = vMainWindowPointer->GetProductByName("~SizeExposure");
				if (TempProduct)
				{
					CString *TempProductName = TempProduct->GetProductName();
					CString TempString1 = "=Process 0";
					TempString1 =  *TempProductName + TempString1;
					if (!vLocalSystemData->vITIPCDig->vUsingLocalSetupDataFile)
						TempString1 = cSetupDataDirectory + TempString1;
					else
						TempString1 = vGlobalCurrentDirectory + TempString1;
					TempString1 = TempString1 + ".BMP";
					LPTSTR TempString = TempString1.GetBuffer(TempString1.GetLength());
					CFileStatus TempFileStatus;
					if (CFile::GetStatus(TempString, TempFileStatus))
						DeleteFile(TempString);
				}
			}
		}
	}
	else
	*/
	if (vMode == cCenterExposureMenu)
	{
		FinishCenterExposureMenu(false);
		StartSizeMenu();
	}
	else
		/*
	if (vMode == cEdgeExposureMenu)
	{
		FinishEdgeExposureMenu(false);
		StartCenterExposureMenu();
	}
	else
	*/
	if (vMode == cSetBackground)
	{
		FinishBackground();
		//hide variable integration time
		//if (vLocalSystemData->vFPGAVersion10Point9OrHigher)
		//	StartEdgeExposureMenu();
		//else
			StartCenterExposureMenu();
	}
	else
	if (vMode == cLearnMenu)
	{
		if ((!((vOldAverageEnabled) || (vOldStructureEnabled) || (vOldTakingQuickLearnImages) ||
			(vOldCalibrateBodyTriggerReferenceEnabled) || (vOldAdjustingBrightnessBeforeLearning))) ||
			(!vLocalSystemData->vInAutoSetup))
		{
			FinishLearnMenu(false);  //don't want to ask if going back
			if (vLocalConfigurationData->vRemoveBackgroundFromImage)
				StartBackground();
			//hide variable integration time
			//else
			//if (vLocalSystemData->vFPGAVersion10Point9OrHigher)
			//	StartEdgeExposureMenu();
			else
				StartCenterExposureMenu();
		}
		else
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\n\nYou cannot exit while learning";
			TempNoticeDialog.vType = cErrorMessage;
			TempNoticeDialog.DoModal();
		}
	}
	else //function4button
	if (vMode == cSetROI)
	{  
		if (vEditOnlyThisInspection)
		{
			if ((!vAnyChangeMade) || (KeepSetupChanges()))
			{
				ExitAutoSetupDialog(20);
			}
		}
		else
		if (!vReturnToSetSource)
		{
			FinishROI(false);
			ConfigureInspections(false, false);
		}
	}
	else
	if (vMode == cSetContaminantThreshold)
	{
		FinishContaminantThreshold(false);

		StartROI();
	}
	else
	if (vMode == cSetVoidThreshold)
	{
		FinishVoidThreshold(false);

		StartROI();
	}
	else
	if (vMode == cSetDensityThreshold)
	{
		if (FinishDensityThreshold(true))
			StartROI();
	}
//	}
//	else
//	{
//		ShowNoMouseClickDialogMessage();
//	}
}

void CAutoSetupImageDisplayDialog::SelectInspectionEjectors(bool TempGoingForward)
{
	if (vNumberEjectorsConfigured == 0)
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\nNo Ejectors Configured.\nYou Must configure an ejector in the System Setup";
		TempNoticeDialog.vType = cErrorMessage;
		TempNoticeDialog.DoModal();
	}
	else
	{
		if (vInspectionEditing)
		{ //only configure ejectors if inspection can show rejects
			//CString TempText;
			//TempText.LoadString(IDS_ImproveLearn);
			//if (vInspectionEditing->vName != TempText)
			if (vInspectionEditing->vShowRejects)
			if (vNumberEjectorsConfigured == 1)
			{
				//select the only ejector in the system automatically
				vInspectionEditing->vEjector = 1 << vOnlyEjectorNumberConfigured;

				CNoticeDialog TempNoticeDialog;
				CString TempText;
				TempText.LoadString(IDS_Onlyoneejectorconfiguredinthesystem);//"\n\nOnly one ejector configured in the system.\nIt has been automatically selected."
				TempNoticeDialog.vNoticeText = TempText;
				TempNoticeDialog.vType = cNoticeMessage;
				TempNoticeDialog.DoModal();
				//UpdateDisplay();
				SetChangeMade();
			}
			else
			{
				int TempResult = vLocalConfigurationData->SelectEjectors(vInspectionEditing->vName, 
					&vInspectionEditing->vEjector, vLocalConfigurationData, false);

				if (TempResult == IDOK)
				{ 
					vMainWindowPointer->CheckEjectorsConfigured(vGlobalCurrentProduct, vInspectionEditing->vEjector);
					SetChangeMade(); 
				}
				else 
				if (TempResult == 10)
				{
					//Main Menu button pressed
					ExitAutoSetupDialog(10);
				}
			}
			vGlobalCurrentProduct->CalculateEndOfLineTimeOut();
		}
	}
}

void CAutoSetupImageDisplayDialog::StartSetSizeSettings()
{
	vGaveSendAContainerNotice = false;
	vChangeMade = false;
	vMode = cSetSizeSettings;

	if (vLocalSystemData->vInAutoSetup)
		vGlobalInAutoSize = true; 

	vChangedPurpleLine = false;
	vDialogTitleString = "Container Edge Finding Settings";
	SetDlgItemText(IDC_DialogTitleStaticText1,vDialogTitleString);

	DisplayWithPassword();

	m_Function2Button.ShowWindow(SW_SHOW);
	{
		m_SubFunction5Button.ShowWindow(SW_SHOW);
		m_SubFunction6Button.ShowWindow(SW_SHOW);
		m_SubFunction7Button.ShowWindow(SW_SHOW);
		m_SubFunction8Button.ShowWindow(SW_SHOW);
		m_SubFunction7Display.ShowWindow(SW_SHOW);
		if (PasswordOK(cTemporaryInspxPassword,false))
			SetDlgItemText(IDC_SubFunction5Display,dtoa(vGlobalCurrentProduct->vEdgeLocationHeight,2) + "(" + dtoa(vGlobalCurrentProduct->vEdgeLocationHeightPixel,0) + ")");
		else
			SetDlgItemText(IDC_SubFunction5Display,dtoa(vGlobalCurrentProduct->vEdgeLocationHeight,2));

		m_SubFunction5Display.ShowWindow(SW_SHOW);
	}

	m_SubFunction7Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
	CString TempText;

	if (PasswordOK(cTemporaryInspxPassword,false))
		SetDlgItemText(IDC_SubFunction7Display,dtoa(vGlobalCurrentProduct->vEdgeLocationBottom,2) + "(" + dtoa(vGlobalCurrentProduct->vEdgeLocationBottomPixel,0) + ")");
	else
		SetDlgItemText(IDC_SubFunction7Display,dtoa(vGlobalCurrentProduct->vEdgeLocationBottom,2));

	//m_SubFunction8Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);

	//m_SubFunction5Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);

	//m_SubFunction6Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);

	m_Function1Button.ShowWindow(SW_SHOW);
	m_Function1Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
	TempText.LoadString(IDS_ReturnToSize);
	SetDlgItemText(IDC_Function1Button,TempText);

	vShowImageInColor = true;
	m_Function5Button.ShowWindow(SW_SHOW);

	vAdjustingBottomEdgeFinding = false;

	WaitThenDrawImageOnScreen();
	UpdateButtonText();
}

void CAutoSetupImageDisplayDialog::FinishSetSizeSettings()
{
	if (vChangedPurpleLine)
	if (vLocalSystemData->vITIPCDig->vProductAverageImage)
	if (vGlobalCurrentProduct->vReferenceRight) //if have a reference already
	{ //find new reference values
		WORD TempTop = 0;
		WORD TempBottom = 0;
		WORD TempHeight = 0;
		WORD TempWidth = 0;
		if ((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode)) //Continuous Feed, or a Pipeline
		{
			TempTop = vLocalSystemData->vITIPCDig->vOriginalBufferSizeX - 1;
			vGlobalCurrentProduct->vReferenceBottom = 0;
			vGlobalCurrentProduct->vReferenceLeft = 0;
			vGlobalCurrentProduct->vReferenceRight = vLocalSystemData->vITIPCDig->vOriginalBufferSizeY - 1;
			TempBottom = 0;
			TempHeight = vLocalSystemData->vITIPCDig->vOriginalBufferSizeX - 1;
			TempWidth = vLocalSystemData->vITIPCDig->vOriginalBufferSizeY - 1;
			if (vLocalSystemData->vLogFile.vLogSerialData)
				vLocalSystemData->vLogFile.WriteToLogFile("Set Reference A Right: " + dtoa(vGlobalCurrentProduct->vReferenceRight, 2) + ", Left: " + dtoa(vGlobalCurrentProduct->vReferenceLeft, 2) +
						", Width: " + dtoa(vGlobalCurrentProduct->vReferenceWidth, 2), cDebugMessage);
		}
		else
		{
			CalculateLearnedReferenceValues();
			ReportErrorMessage("Save Reference Data from Learned Image", cAction, 0);
		}
	}
	vChangedPurpleLine = false;

	m_SubFunction1Button.ShowWindow(SW_HIDE);
	this->InvalidateRect(&m_SubFunction1Button.GetRectToInvalidateArrow());
	m_SubFunction4Button.ShowWindow(SW_HIDE);
	this->InvalidateRect(&m_SubFunction4Button.GetRectToInvalidateArrow());
	m_SubFunction4DisplayA.ShowWindow(SW_HIDE);
	vGlobalInAutoSize = false; 
}

void CAutoSetupImageDisplayDialog::StartSizeMenu()
{
	vMode = cSetImageSize;
	CString TempText("");
	//if in autosetup and not a pipeline, find edges from right side only
	if (vLocalSystemData->vInAutoSetup)
		vGlobalInAutoSize = true; 

	vFunction4ButtonEnable = true;		//m_Function4Button.EnableWindow(true);
	if ((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode) || (vLocalConfigurationData->vScanTracType == cBriosoScanTrac)) //Continuous Feed, or a Pipeline
	{
		m_SubFunction2Button.ShowWindow(SW_HIDE); //hide Auto Size Button
		this->InvalidateRect(&m_SubFunction2Button.GetRectToInvalidateArrow());
	}
	else
	{
		if ((vHaveAnImage) && (vLocalSystemData->vInAutoSetup) && (!vNeedToAutoSizeNextContainer))
		{
			vSubFunction2ButtonEnable = true;		//m_SubFunction2Button.EnableWindow(true);
			m_SubFunction2Button.Invalidate(false);
		}
		else
		{
			vSubFunction2ButtonEnable = false;		//m_SubFunction2Button.EnableWindow(false);
			m_SubFunction2Button.Invalidate(false);
		}
		m_SubFunction2Button.ShowWindow(SW_SHOW);
		m_SubFunction2Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		TempText.LoadString(IDS_AutoSizeThisContainer);
		SetDlgItemText(IDC_SubFunction2Button,TempText);
	}
	m_Function4Button.ShowWindow(SW_HIDE);
	this->InvalidateRect(&m_Function4Button.GetRectToInvalidateArrow());

	m_Width2.ShowWindow(SW_SHOW);
	m_Function2Button.ShowWindow(SW_SHOW);
	m_Function2Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
	if (vLocalSystemData->vInAutoSetup)
		TempText.LoadString(IDS_FinishAutoSetup);
	else
		TempText.LoadString(IDS_FinishSetup);
	SetDlgItemText(IDC_Function2Button,TempText);
	
	m_Function5Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
	TempText.LoadString(IDS_NextToExposure);
	SetDlgItemText(IDC_Function5Button,TempText);

	if (vLocalSystemData->vInAutoSetup)
		vDialogTitleString.LoadString(IDS_AutoSetupImageSize);
	else
		vDialogTitleString.LoadString(IDS_SetupImageSize);
	SetDlgItemText(IDC_DialogTitleStaticText1,vDialogTitleString);
	vShowImageInColor = true;

	if (vLocalSystemData->vXRayOnNotice)
		TempText.LoadString(IDS_WaitingForContainer);
	else
		TempText = " Starting X-Rays, Please Wait..";

	if (!vChangedEdgeFindingMode)
		SetStatusLine(TempText + vProductEditingName);
	vChangeMade = false;
	UpdateButtonText();
	DrawImageOnScreen();

	m_SubFunction5Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
	if (!vLocalSystemData->vInAutoSetup)
	{
		vSubFunction2ButtonEnable = false;		//m_SubFunction2Button.EnableWindow(false);
		m_SubFunction2Button.Invalidate(false);
		vSubFunction4ButtonEnable = false;		//m_SubFunction5Button.EnableWindow(false);
		vSubFunction5ButtonEnable = false;		//m_SubFunction5Button.EnableWindow(false);
		vSubFunction6ButtonEnable = false;		//m_SubFunction6Button.EnableWindow(false);
		vSubFunction8ButtonEnable = false;		//m_SubFunction8Button.EnableWindow(false);
	}
	if (PasswordOK(cTemporaryInspxPassword,false))
		SetDlgItemText(IDC_SubFunction5Display, dtoa(vGlobalCurrentProduct->vProductImageHeightTop,2) + 
		" (" + dtoa(vGlobalCurrentProduct->vProductImageHeightTop * vGlobalPixelsPerUnitInHeight,0) + ")");
	else
		SetDlgItemText(IDC_SubFunction5Display, dtoa(vGlobalCurrentProduct->vProductImageHeightTop,2));

	m_SubFunction6Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
	m_SubFunction8Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);

	/*
	TempText.LoadString(IDS_XScale);
	SetDlgItemText(IDC_XScaleLabel,TempText + ":");
	TempText.LoadString(IDS_YScale);
	SetDlgItemText(IDC_YScaleLabel,TempText + ":");
	TempText.LoadString(IDS_XSize);
	SetDlgItemText(IDC_SizeLabelX,TempText + ":");
	TempText.LoadString(IDS_YSize);
	SetDlgItemText(IDC_SizeLabelY,TempText + ":");
	DisplayScale();
*/

	DisplayWithPassword();
	WaitDisplayLocationEdge();
	if (vGlobalCurrentProduct->vMultiLaneNumberOfLanes)
	{
		TempText.LoadString(IDS_AdjustLanePositions);
		SetDlgItemText(IDC_SubFunction5Button, TempText);
		m_SubFunction6Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction6Button.GetRectToInvalidateArrow());
		m_SubFunction5Display.ShowWindow(SW_HIDE);
	}
}

void CAutoSetupImageDisplayDialog::WaitDisplayLocationEdge()
{
	// could set a timer to invalidate in 200 milliseconds
	int TimerResult = SetTimer(vWaitToUpdateScreenLocationEdgeTimerHandle,100,NULL);
	if (!TimerResult)
		ReportErrorMessage("Error-Wait to update screen Timer Failed",cEMailInspx,32000);
}

void CAutoSetupImageDisplayDialog::DisplayLocationEdge()
{
	if ((PasswordOK(cTemporaryInspxPassword,false)) || ((!vLocalConfigurationData->vContinuousFeedContainerRate) && (!vLocalConfigurationData->vBulkProductMode) && (!vGlobalCurrentProduct->vBulkProductMode))) //Not Continuous Feed, Not a Pipeline or password in
	if (vHaveAnImage)
	if ((vMode == cSetImageSize) || (vMode == cSetSizeSettings))
	{
			vDrawPurpleLine = true;
		BYTE TempOffsetX = (BYTE) ResizeXCoor(108);
		BYTE TempOffsetY = (BYTE) ResizeYCoor(43);
		RECT TempRectangle;

		WORD TempBottomPurpleLine = 0;
		if (vLocalSystemData->vITIPCDig->vDoBottomReferencing)
		if (vGlobalCurrentProduct->vBottomLocationLengthPixel)
			TempBottomPurpleLine = 8;

		TempRectangle.left = TempOffsetX - 8;
		TempRectangle.top = TempOffsetY;
		TempRectangle.right = TempOffsetX + vLocalSystemData->vITIPCDig->vDisplayImageSizeX + 8;
		TempRectangle.bottom = TempOffsetY + vLocalSystemData->vITIPCDig->vDisplayImageSizeY + TempBottomPurpleLine;
		// could set a timer to invalidate in 200 milliseconds
		this->InvalidateRect(&TempRectangle, false);
	}
}

/*
void CAutoSetupImageDisplayDialog::DisplayScale()
{
	CString TempText("");

	SetDlgItemText(IDC_SizeX,
		dtoa(vGlobalCurrentProduct->vProductImageWidth,2));

	SetDlgItemText(IDC_SizeY,
		dtoa(vGlobalCurrentProduct->vProductImageHeightTop - 
		vGlobalCurrentProduct->vProductImageHeightBottom,2));

	TempText = dtoa(vGlobalCurrentProduct->vProductImageWidth / 10.0,2);
	SetDlgItemText(IDC_XScale2,TempText);

	TempText = dtoa((vGlobalCurrentProduct->vProductImageHeightTop - 
		vGlobalCurrentProduct->vProductImageHeightBottom) / 10.0,2);
	SetDlgItemText(IDC_YScale2,TempText);
}
*/

bool CAutoSetupImageDisplayDialog::FinishSizeMenu(bool TempWarn, bool TempExiting)
{
	bool TempFinishedImageSize = true;

	if (TempExiting)
		vChangedEdgeFindingMode = false;
	float TempContainerWidth = 0;
	if (vLocalSystemData->vITIPCDig->vOriginalImage)
		TempContainerWidth = (float)((
			(vLocalSystemData->vITIPCDig->vOriginalImage->vRight - 
			vLocalSystemData->vITIPCDig->vOriginalImage->vLeft) 
			/ (vGlobalPixelsPerUnit * vGlobalCurrentProduct->vOverScanMultiplier)));

	if (!((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode) ||
		(vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vLocalConfigurationData->vScanTracType == cAllegroScanTrac) || 
		(vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac) || (vGlobalScanTracType == cBriosoScanTrac))) //not Continuous Feed, or a Pipeline or TD
	if (TempWarn)
	if (vHaveAnImage)
	if (vNeedToAutoSizeNextContainer)
	{
		CYesNoDialog TempYesNoDialog;
		CString TempText;
		TempText.LoadString(IDS_WaitForA2OrMoreFootGapInProductionToAutosizeDoYouWantToAbort);
		TempYesNoDialog.vNoticeText = TempText;
		CString TempText1;
		TempText1.LoadString(IDS_Abort);
		TempYesNoDialog.vYesButtonText = TempText1;
		CString TempText2;
		TempText2.LoadString(IDS_Wait);
		TempYesNoDialog.vNoButtonText = TempText2;
		TempYesNoDialog.vQuestionType = cConfirmQuestion;
		int TempResult = TempYesNoDialog.DoModal();
		if (TempResult != IDOK)
			TempFinishedImageSize = false;
	}
	else
	//if ((PasswordOK(cTemporaryInspxPassword,false)) || PasswordOK(cAdvancedSetupPassword,false)) //only give warnings to advanced users
	if ((TempWarn) && (TempContainerWidth) && (vLocalSystemData->vITIPCDig->vOriginalImage) && (!vLocalConfigurationData->vContinuousFeedContainerRate) &&
		(!vLocalConfigurationData->vBulkProductMode) && (!vGlobalCurrentProduct->vBulkProductMode) && (vLocalConfigurationData->vScanTracType != cBriosoScanTrac) && 
		(vLocalConfigurationData->vScanTracType != cAtempoScanTrac) && (vLocalConfigurationData->vScanTracType != cForteScanTrac) && (vLocalConfigurationData->vScanTracType != cAllegroScanTrac) && 
		(vLocalConfigurationData->vScanTracType != cCaseInspectorScanTrac) &&
		((vLocalSystemData->vITIPCDig->vOriginalImage->vLeft < 3) || (vLocalSystemData->vITIPCDig->vOriginalImage->vRight > vLocalSystemData->vITIPCDig->vAquisitionSizeY - 4)))
	{
		CYesNoDialog TempYesNoDialog;
		CString TempText("");
		TempText.LoadString(IDS_ScanTracdidnotfindtheedgesofthecontainercorrectly);
		TempYesNoDialog.vNoticeText = TempText;
		CString TempText1("");
		TempText1.LoadString(IDS_GoBack);
		TempYesNoDialog.vYesButtonText = TempText1;
		CString TempText2("");
		TempText2.LoadString(IDS_ContinueAsSet);
		TempYesNoDialog.vNoButtonText = TempText2;
		TempYesNoDialog.vQuestionType = cConfirmQuestion;
		int TempResult = TempYesNoDialog.DoModal();
		if (TempResult == IDOK)
			TempFinishedImageSize = false;
	}
	else
	if (((TempWarn) && (TempContainerWidth > 0.5) && (vGlobalCurrentProduct->vProductLockOutWidth > TempContainerWidth + 0.5)) && 
		(!((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode))))
	{
		CYesNoDialog TempYesNoDialog;
		CString TempText("");
		TempText.LoadString(IDS_ContainerReTriggerLockoutlargerthancontainerwidthYoushouldgobackandAutosize);
		TempText = TempText;
		TempYesNoDialog.vNoticeText = TempText;
		CString TempText1("");
		TempText1.LoadString(IDS_GoBackToAutosize);
		TempYesNoDialog.vYesButtonText = TempText1;
		CString TempText2("");
		TempText2.LoadString(IDS_ContinueAsSet);
		TempYesNoDialog.vNoButtonText = TempText2;
		TempYesNoDialog.vQuestionType = cConfirmQuestion;
		int TempResult = TempYesNoDialog.DoModal();
		if (TempResult == IDOK)
			TempFinishedImageSize = false;
	}
	else
	if ((TempWarn) && (vGlobalCurrentProduct->vProductLockOutWidth + 0.5 < TempContainerWidth) && 
		(!((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode))))
	{
		double TempWidth = vGlobalCurrentProduct->vProductLockOutWidth;
		CYesNoDialog TempYesNoDialog;
		CString TempText;
		TempText.LoadString(IDS_ContainerReTriggerLockoutsmallthancontainerwidthYoushouldgobackandAutosize);
		TempText = TempText;
		TempYesNoDialog.vNoticeText = TempText;
		CString TempText1;
		TempText1.LoadString(IDS_GoBackToAutosize);
		TempYesNoDialog.vYesButtonText = TempText1;
		CString TempText2;
		TempText2.LoadString(IDS_ContinueAsSet);
		TempYesNoDialog.vNoButtonText = TempText2;
		TempYesNoDialog.vQuestionType = cConfirmQuestion;
		int TempResult = TempYesNoDialog.DoModal();
		if (TempResult == IDOK)
			TempFinishedImageSize = false;
	}
	else
		/*
	if (vLocalSystemData->vBodyTriggerWidthAverage > 16)
	{
		if (vGlobalCurrentProduct->vLockoutPosition > vLocalSystemData->vBodyTriggerWidthAverage + 16)
		{
			CYesNoDialog TempYesNoDialog;
			CString TempText("");
			TempText.LoadString(IDS_ContainerReTriggerLockoutlargerthancontainerwidthYoushouldgobackandAutosize);
			TempText = TempText;
			TempYesNoDialog.vNoticeText = TempText;
			CString TempText1("");
			TempText1.LoadString(IDS_GoBackToAutosize);
			TempYesNoDialog.vYesButtonText = TempText1;
			CString TempText2("");
			TempText2.LoadString(IDS_ContinueAsSet);
			TempYesNoDialog.vNoButtonText = TempText2;
			TempYesNoDialog.vQuestionType = cConfirmQuestion;
			int TempResult = TempYesNoDialog.DoModal();
			if (TempResult == IDOK)
				TempFinishedImageSize = false;
		}
	}
	else
	if (vLocalSystemData->vLastBodyTriggerLength > 14)
	{
		if (vGlobalCurrentProduct->vLockoutPosition > vLocalSystemData->vLastBodyTriggerLength + 16)
		{
			CYesNoDialog TempYesNoDialog;
			CString TempText("");
			TempText.LoadString(IDS_ContainerReTriggerLockoutlargerthancontainerwidthYoushouldgobackandAutosize);
			TempText = TempText;
			TempYesNoDialog.vNoticeText = TempText;
			CString TempText1("");
			TempText1.LoadString(IDS_GoBackToAutosize);
			TempYesNoDialog.vYesButtonText = TempText1;
			CString TempText2("");
			TempText2.LoadString(IDS_ContinueAsSet);
			TempYesNoDialog.vNoButtonText = TempText2;
			TempYesNoDialog.vQuestionType = cConfirmQuestion;
			int TempResult = TempYesNoDialog.DoModal();
			if (TempResult == IDOK)
				TempFinishedImageSize = false;
		}
	}
	else
	*/
	if (vGlobalCurrentProduct->vProductImageWidth > TempContainerWidth * 1.95)
	if ((TempWarn) && (!vLocalConfigurationData->vContinuousFeedContainerRate) && (!vLocalConfigurationData->vBulkProductMode) && (!vGlobalCurrentProduct->vBulkProductMode) && (vLocalConfigurationData->vScanTracType != cBriosoScanTrac) && 
			(vLocalConfigurationData->vScanTracType != cAtempoScanTrac) && (vLocalConfigurationData->vScanTracType != cForteScanTrac) && (vLocalConfigurationData->vScanTracType != cAllegroScanTrac) && (vLocalConfigurationData->vScanTracType != cCaseInspectorScanTrac)) //Continuous Feed, or a Pipeline or TD
	{
		CYesNoDialog TempYesNoDialog;
		CString TempText("");
		TempText.LoadString(IDS_ImageIsTooWideYoushouldgobackandAutosize);
		TempText = TempText;
		TempYesNoDialog.vNoticeText = TempText;
		CString TempText1("");
		TempText1.LoadString(IDS_GoBackToAutosize);
		TempYesNoDialog.vYesButtonText = TempText1;
		CString TempText2("");
		TempText2.LoadString(IDS_ContinueAsSet);
		TempYesNoDialog.vNoButtonText = TempText2;
		TempYesNoDialog.vQuestionType = cConfirmQuestion;
		int TempResult = TempYesNoDialog.DoModal();
		if (TempResult == IDOK)
			TempFinishedImageSize = false;
	}
	if (TempFinishedImageSize)
	if ((vLocalSystemData->vChangedSizeOfProduct) && (vAutoSetupType == cNewAutoSetup))
	{
		if (vHaveAnImage)
		{
			bool TempMakeInspectionsFullImage = false;
			if ((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode) ||
				(vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vLocalConfigurationData->vScanTracType == cAllegroScanTrac) ||
				(vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac) || (vGlobalScanTracType == cBriosoScanTrac)) //Continuous Feed, or a Pipeline or TD
				TempMakeInspectionsFullImage = true;
			vGlobalCurrentProduct->SetupAllInspectionROIsFromReference(vLocalSystemData->vITIPCDig->vOriginalImage, true, TempMakeInspectionsFullImage, vLocalSystemData->vITIPCDig->vProductAverageImage);
		}
		else
		if (TempWarn)
		{
			CYesNoDialog TempYesNoDialog;

			if ((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode)) //Continuous Feed, or a Pipeline
				TempYesNoDialog.vNoticeText = _T("You changed the size of the product,\nbut dont have a new product image.\nThe system needs to verify the image\nbefore leaving Size Menu");
			else
				TempYesNoDialog.vNoticeText = _T("You changed the size of the product,\nbut have not sent a new container though.\nThe system needs to verify container size\nbefore leaving Size Menu");

			CString TempText1;
			TempText1.LoadString(IDS_GoBackToAutosize);
			TempYesNoDialog.vYesButtonText = TempText1;
			CString TempText2;
			TempText2.LoadString(IDS_ContinueAsSet);
			TempYesNoDialog.vNoButtonText = TempText2;
			TempYesNoDialog.vQuestionType = cConfirmQuestion;
			int TempResult = TempYesNoDialog.DoModal();
			if (TempResult == IDOK)
				TempFinishedImageSize = false;
		}
	}
	else
		vGlobalCurrentProduct->CheckAllROIs(vLocalSystemData->vITIPCDig->vProductAverageImage);


	if (TempFinishedImageSize)
	{
		vNeedToAutoSizeNextContainer = false;
		vWaitingForBackgroundImage = false;
		vLocalSystemData->vRemoveBackgroundInAutoSize = false;
		//delete background image if only for autosize
		if ((!vLocalConfigurationData->vRemoveBackgroundFromImage) &&
			(vLocalSystemData->vRemoveBackgroundInAutoSize))
		if (vLocalSystemData->vITIPCDig->vProductBackgroundImage)
		{
			im_delete(vLocalSystemData->vITIPCDig->vProductBackgroundImage);
			vLocalSystemData->vITIPCDig->vProductBackgroundImage = NULL;
		}

		vHaveScaledImageToPaint = false;
		vGlobalInAutoSize = false;
		vSubFunction2ButtonEnable = true;		//m_SubFunction2Button.EnableWindow(true);
		m_SubFunction2Button.Invalidate(false);
		m_Width2.ShowWindow(SW_HIDE);
		vSubFunction4ButtonEnable = true;		//m_SubFunction5Button.EnableWindow(true);
		vSubFunction5ButtonEnable = true;		//m_SubFunction5Button.EnableWindow(true);
		vSubFunction6ButtonEnable = true;		//m_SubFunction6Button.EnableWindow(true);
		vSubFunction8ButtonEnable = true;		//m_SubFunction8Button.EnableWindow(true);
		m_Function2Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_Function2Button.GetRectToInvalidateArrow());
		m_SubFunction2Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction2Button.GetRectToInvalidateArrow());

		m_SubFunction1Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction1Button.GetRectToInvalidateArrow());
		m_SubFunction4Display.ShowWindow(SW_HIDE);
		m_SubFunction4Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction4Button.GetRectToInvalidateArrow());
		m_SubFunction5Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction5Button.GetRectToInvalidateArrow());
		m_SubFunction6Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction6Button.GetRectToInvalidateArrow());
		m_SubFunction7Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction7Button.GetRectToInvalidateArrow());
		m_SubFunction8Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction8Button.GetRectToInvalidateArrow());
		vSubFunction8ButtonEnable = true;	//m_SubFunction8Button.EnableWindow(true);

		m_SubFunction3Display.ShowWindow(SW_HIDE);
		m_SubFunction7Display.ShowWindow(SW_HIDE);
		m_SubFunction5Display.ShowWindow(SW_HIDE);
		m_SubFunction6Display.ShowWindow(SW_HIDE);
		m_SubFunction8Display.ShowWindow(SW_HIDE);
		m_XScaleLabel.ShowWindow(SW_HIDE);
		m_XScale2.ShowWindow(SW_HIDE);
		m_YScaleLabel.ShowWindow(SW_HIDE);
		m_YScale2.ShowWindow(SW_HIDE);
		m_SizeX.ShowWindow(SW_HIDE);
		m_SizeLabelX.ShowWindow(SW_HIDE);
		m_SizeY.ShowWindow(SW_HIDE);
		m_SizeLabelY.ShowWindow(SW_HIDE);

		vMakeStatusLineYellowRed = false;

		if (m_PleaseWait.IsWindowVisible())  //if have already cleared the please wait message, then kill timer and do nothing
		if ((vLocalSystemData->vSystemRunMode == cAutoSetupRunningSystem) && (vLocalSystemData->vXRayOnNotice))  //if X-Rays on now, then clear please wait message
		{
			KillTimer(vClearStartingIndicatorIfXRaysOnTimerHandle);
			CString TempText("");
			TempText.LoadString(IDS_XRAYON);
			SetDlgItemText(IDC_Calibrating, TempText);
			m_Calibrating.ShowWindow(SW_SHOW);
		
			if ((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode)) //Continuous Feed, or a Pipeline
				SetStatusLine(" Waiting for Image");
			else
				SetStatusLine(" Send Container Through ScanTrac");
			m_PleaseWait.ShowWindow(SW_HIDE);
		}

	}
	return TempFinishedImageSize;
}

void CAutoSetupImageDisplayDialog::FinishROI(bool TempExitingSetup) 
{
	vShowAverage = false;
	ClearLearnSensitivity();
	m_SubFunction1Display.ShowWindow(SW_HIDE);
	m_SubFunction3Display.ShowWindow(SW_HIDE);
	m_SubFunction5Display.ShowWindow(SW_HIDE);
	m_SubFunction7Display.ShowWindow(SW_HIDE);
	m_SubFunction8Display.ShowWindow(SW_HIDE);
	m_Width2.ShowWindow(SW_HIDE);
	m_Reference.ShowWindow(SW_HIDE);
	m_Function1Button.ShowWindow(SW_SHOW);

	vFunction2ButtonEnable = true;		//m_Function2Button.EnableWindow(true);
	m_Function2Button.ShowWindow(SW_HIDE);
	this->InvalidateRect(&m_Function2Button.GetRectToInvalidateArrow());
	vFunction3ButtonEnable = true;		//m_Function3Button.EnableWindow(true);
	m_Function3Button.ShowWindow(SW_HIDE);
	this->InvalidateRect(&m_Function3Button.GetRectToInvalidateArrow());
	m_SubFunction1Button.ShowWindow(SW_HIDE);
	this->InvalidateRect(&m_SubFunction1Button.GetRectToInvalidateArrow());
	m_SubFunction2Button.ShowWindow(SW_HIDE);
	this->InvalidateRect(&m_SubFunction2Button.GetRectToInvalidateArrow());
	m_SubFunction3Button.ShowWindow(SW_HIDE);
	this->InvalidateRect(&m_SubFunction3Button.GetRectToInvalidateArrow());
	m_SubFunction4Button.ShowWindow(SW_HIDE);
	this->InvalidateRect(&m_SubFunction4Button.GetRectToInvalidateArrow());
	m_SubFunction5Button.ShowWindow(SW_HIDE);
	this->InvalidateRect(&m_SubFunction5Button.GetRectToInvalidateArrow());
	m_SubFunction6Button.ShowWindow(SW_HIDE);
	this->InvalidateRect(&m_SubFunction6Button.GetRectToInvalidateArrow());
	m_SubFunction7Button.ShowWindow(SW_HIDE);
	this->InvalidateRect(&m_SubFunction7Button.GetRectToInvalidateArrow());
	m_SubFunction8Button.ShowWindow(SW_HIDE);
	this->InvalidateRect(&m_SubFunction8Button.GetRectToInvalidateArrow());

	m_SubFunction1Button.ShowNavigationArrow(NULL);
	m_SubFunction2Button.ShowNavigationArrow(NULL);
	m_SubFunction3Button.ShowNavigationArrow(NULL);
	m_SubFunction4Button.ShowNavigationArrow(NULL);
	m_SubFunction5Button.ShowNavigationArrow(NULL);
	m_SubFunction6Button.ShowNavigationArrow(NULL);
	m_SubFunction7Button.ShowNavigationArrow(NULL);
	m_SubFunction8Button.ShowNavigationArrow(NULL);
	if (vInspectionEditing)
	if (vInspectionEditing == vGlobalCurrentProduct->vDriftDensityInspection)
	{
		vLocalSystemData->vITIPCDig->CalculateAverageDriftDensity(vGlobalCurrentProduct);
		vLocalSystemData->vIndexForDriftDensityOfLastNImages = 0;
	}

	if (vInspectionEditing)
	if ((vInspectionEditing->vInspectionType == cUnderfillByWeightInspection) || (vInspectionEditing->vInspectionType == cOverfillByWeightInspection) || (vInspectionEditing->vInspectionType == cCheckWeighInspection))
		vGlobalCurrentProduct->MatchInspectionsForWeightTrending();  //must update in case calibrated weights, or changed linking
}

void CAutoSetupImageDisplayDialog::StartROI() 
{
	if ((vInspectionEditing) && ((vInspectionEditing->vInspectionType == cUnderfillByWeightInspection) || (vInspectionEditing->vInspectionType == cOverfillByWeightInspection)) && 
		(vInspectionEditing->vInspectionNameForWeightTrendingPointer)) //if wieght trending inspection, use other inspections settings
	{
		GiveLinkedInspectionNotice();
		StartDensityThreshold();
	}
	else
	{
		vMode = cSetROI;

		m_SubFunction1Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		m_SubFunction2Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		m_SubFunction3Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		m_SubFunction4Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);

		CString TempText;
		m_Function1Button.ShowWindow(SW_SHOW);
		m_Function1Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
		TempText.LoadString(IDS_Abort);
		SetDlgItemText(IDC_Function1Button,TempText);

		m_Function2Button.ShowWindow(SW_SHOW);
		m_SubFunction8Display.ShowWindow(SW_HIDE);
		m_Width2.ShowWindow(SW_HIDE);
		m_Reference.ShowWindow(SW_HIDE);

		TempText.LoadString(IDS_InspectionName);
		m_Function2Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		SetDlgItemText(IDC_Function2Button,TempText);

		vSubFunction1ButtonEnable = true;		//m_SubFunction1Button.EnableWindow(true);
		vSubFunction2ButtonEnable = true;		//m_SubFunction2Button.EnableWindow(true);
		vSubFunction3ButtonEnable = true;		//m_SubFunction3Button.EnableWindow(true);
		vSubFunction4ButtonEnable = true;		//m_SubFunction4Button.EnableWindow(true);
		vSubFunction5ButtonEnable = true;		//m_SubFunction5Button.EnableWindow(true);
		vSubFunction6ButtonEnable = true;		//m_SubFunction6Button.EnableWindow(true);
		vSubFunction7ButtonEnable = true;		//m_SubFunction7Button.EnableWindow(true);
		vSubFunction8ButtonEnable = true;		//m_SubFunction8Button.EnableWindow(true);

		if (vReturnToSetSource)
		{
			vDialogTitleString.LoadString(IDS_SetupProductDensityROI);
			SetDlgItemText(IDC_DialogTitleStaticText1,vDialogTitleString);

			m_Function2Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_Function2Button.GetRectToInvalidateArrow());

			m_Function4Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_Function4Button.GetRectToInvalidateArrow());

			m_Function5Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_Function5Button.GetRectToInvalidateArrow());

			m_Function1Button.ShowWindow(SW_SHOW);
			m_Function1Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
			TempText.LoadString(IDS_ReturntoExposure);
			SetDlgItemText(IDC_Function1Button,TempText);
		}
		else
		{
			m_Function5Button.ShowWindow(SW_SHOW);
			if (vLocalSystemData->vInAutoSetup)
				TempText.LoadString(IDS_NextToEvaluate);
			else
				TempText.LoadString(IDS_EvaluateThreshold);
			m_Function5Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
			SetDlgItemText(IDC_Function5Button,TempText);

			if (vInspectionEditing)
			{	
				vDialogTitleString = vInspectionEditing->vName + " ROI";
				if (vLocalSystemData->vInAutoSetup)
					vDialogTitleString = "Auto " + vDialogTitleString;
			}
			else
			if (vLocalSystemData->vInAutoSetup)
				vDialogTitleString.LoadString(IDS_AutoSetupInspectionROI);
			else
				vDialogTitleString.LoadString(IDS_SetupInspectionROI);
			SetDlgItemText(IDC_DialogTitleStaticText1,vDialogTitleString);

			m_Function4Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
			m_Function4Button.ShowWindow(SW_SHOW);
			//if (vLocalSystemData->vInAutoSetup)
				TempText.LoadString(IDS_BackToInspections);
			//else
			//	TempText.LoadString(IDS_ChangeInspections);
			SetDlgItemText(IDC_Function4Button,TempText);
		}

		m_SubFunction1Button.ShowWindow(SW_SHOW);
		m_SubFunction2Button.ShowWindow(SW_SHOW);
		m_SubFunction3Button.ShowWindow(SW_SHOW);
		m_SubFunction4Button.ShowWindow(SW_SHOW);
		m_SubFunction5Button.ShowWindow(SW_SHOW);
		m_SubFunction6Button.ShowWindow(SW_SHOW);
		m_SubFunction7Button.ShowWindow(SW_SHOW);
		m_SubFunction8Button.ShowWindow(SW_SHOW);
		m_SubFunction5Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		m_SubFunction6Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		m_SubFunction7Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		m_SubFunction8Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);

		vSubFunction5ButtonEnable = true;		//m_SubFunction5Button.EnableWindow(true);
		vSubFunction6ButtonEnable = true;		//m_SubFunction6Button.EnableWindow(true);
		vSubFunction7ButtonEnable = true;		//m_SubFunction7Button.EnableWindow(true);

		vChangeMade = false;
		//vLocalSystemData->vFrameCount = 0;
		TempText.LoadString(IDS_AdjustRegionOfInterestLines);
		if (vInspectionEditing)
			TempText = TempText + ": " + vInspectionEditing->vName;
		SetStatusLine(TempText);

		SetDlgItemText(IDC_SubFunction6Button1, _T(""));
		SetDlgItemText(IDC_SubFunction6Button2, _T(""));
		SetDlgItemText(IDC_SubFunction6Button3, _T(""));
		SetDlgItemText(IDC_SubFunction8Button1, _T(""));
		SetDlgItemText(IDC_SubFunction8Button2, _T(""));
		SetDlgItemText(IDC_SubFunction8Button3, _T(""));

		TempText = vGlobalTopString;
		SetDlgItemText(IDC_SubFunction1Button,TempText);
		SetDlgItemText(IDC_SubFunction2Button,TempText);
		TempText = vGlobalBottomString;
		SetDlgItemText(IDC_SubFunction3Button,TempText);
		SetDlgItemText(IDC_SubFunction4Button,TempText);
		TempText.LoadString(IDS_Left);
		SetDlgItemText(IDC_SubFunction5Button,TempText);
		SetDlgItemText(IDC_SubFunction6Button,TempText);
		TempText.LoadString(IDS_Right);
		SetDlgItemText(IDC_SubFunction7Button,TempText);
		SetDlgItemText(IDC_SubFunction8Button,TempText);

		m_SubFunction1Button.ShowNavigationArrow(cUpArrowSmall);
		m_SubFunction2Button.ShowNavigationArrow(cDownArrowSmall);
		m_SubFunction3Button.ShowNavigationArrow(cUpArrowSmall);
		m_SubFunction4Button.ShowNavigationArrow(cDownArrowSmall);
		m_SubFunction5Button.ShowNavigationArrow(cLeftArrowSmall);
		m_SubFunction6Button.ShowNavigationArrow(cRightArrowSmall);
		m_SubFunction7Button.ShowNavigationArrow(cLeftArrowSmall);
		m_SubFunction8Button.ShowNavigationArrow(cRightArrowSmall);

		if (vLocalSystemData->vITIPCDig->vProductAverageImage)
			vShowAverage = true;

		m_SubFunction1Display.ShowWindow(SW_SHOW);
		m_SubFunction3Display.ShowWindow(SW_SHOW);
		m_SubFunction5Display.ShowWindow(SW_SHOW);
		m_SubFunction7Display.ShowWindow(SW_SHOW);

		UpdateButtonText();
		UpdateFunction3Button();
		DrawImageOnScreen();

		ShowLearnSensitivity();
		if (vLocalSystemData->vLearnSensitivities[vInspectionNumberEditing] < 255)
		{
			m_SubFunction2Display.ShowWindow(SW_SHOW);
	//		m_SubFunction2Display.SetWindowPos(NULL,5,cLearnSensitivityBottom + 1,100,15,SWP_NOZORDER);
//			::SetupItem(&m_SubFunction2Display);
		}
	}
}

void CAutoSetupImageDisplayDialog::InitializeThresholdValues() 
{
	vShowAverage = false;
	vShowStructure = false;
	vShowVoidStructure = false;
	//vShowGlassStructure = false;
	vShowContaminant2Structure = false;
	vShowHistogram = false;
	//vShowGlassHistogram = false;

	vLocalSystemData->vMinimumGoodIndex = 0;
	//vLocalSystemData->vFrameCount = 0;
	WriteCSVFiles();

	vOldMinimumGoodIntensity = 256;
	vOldMinimumGoodAverage = 0;  
	vOldMinimumGoodStandardDeviation = 0;
	vOldMaximumBadIntensity = 0;
	vOldMaximumBadAverage = 0;
	vOldMaximumBadStandardDeviation = 0;
	vOldMaximumBadCount = 0;
	vOldMinimumGoodCount = 0;
	if (vInspectionEditing)
	{
		vLocalSystemData->vMinimumGoodIntensity = vInspectionEditing->vSetupHistoryMinimumGood;
		vLocalSystemData->vMinimumGoodAverage = vInspectionEditing->vSetupHistoryMinimumGoodAverage;
		vLocalSystemData->vMinimumGoodStandardDeviation = vInspectionEditing->vSetupHistoryMinimumGoodStandardDeviation;
		vLocalSystemData->vMinimumGoodIndex = (WORD)vInspectionEditing->vSetupHistoryMinimumGoodCount;

		vLocalSystemData->vMaximumBadIntensity = vInspectionEditing->vSetupHistoryMaximumBad;
		vLocalSystemData->vMaximumBadAverage = vInspectionEditing->vSetupHistoryMaximumBadAverage;
		vLocalSystemData->vMaximumBadStandardDeviation = vInspectionEditing->vSetupHistoryMaximumBadStandardDeviation;
		vLocalSystemData->vMaximumBadCount = vInspectionEditing->vSetupHistoryMaximumBadCount;
		vOldMaximumBadCount = vInspectionEditing->vSetupHistoryMaximumBadCount;
	}
	vLocalSystemData->vMaximumBadIndex = 0;
	vOldQualityFactor = 0;
	//vOldMinimumGoodIntensity = vInspectionEditing->vSetupHistoryMinimumGood;
	//vOldMinimumGoodAverage = vInspectionEditing->vSetupHistoryMinimumGoodAverage;  
	//vOldMinimumGoodStandardDeviation = vInspectionEditing->vSetupHistoryMinimumGoodStandardDeviation;
	//vOldMaximumBadIntensity = vInspectionEditing->vSetupHistoryMaximumBad;
	//vOldMaximumBadAverage = vInspectionEditing->vSetupHistoryMaximumBadAverage;
	//vOldMaximumBadStandardDeviation = vInspectionEditing->vSetupHistoryMaximumBadStandardDeviation;
}

void CAutoSetupImageDisplayDialog::FinishThreshold()
{
	ClearLearnSensitivity();
	ClearSensitivity();
	m_SubFunction1Display.ShowWindow(SW_HIDE);
	m_SubFunction1Display.SetWindowPos(NULL,5,ResizeYCoor(120),ResizeXCoor(100),ResizeYCoor(20),SWP_NOZORDER);
//	::SetupItem(&m_SubFunction1Display);
	this->Invalidate();
	vHaveAnAutoThresholdValue = false;
	vPickOneToEvaluate = false;
	m_SubFunction5Display.ShowWindow(SW_HIDE);
	m_SubFunction7Display.ShowWindow(SW_HIDE);
	m_SubFunction8Display.ShowWindow(SW_HIDE);
	m_SubFunction6Button1.ShowWindow(SW_HIDE);
	m_SubFunction6Button2.ShowWindow(SW_HIDE);
	m_SubFunction6Button3.ShowWindow(SW_HIDE);
	m_SubFunction8Button1.ShowWindow(SW_HIDE);
	m_SubFunction8Button2.ShowWindow(SW_HIDE);
	m_SubFunction8Button3.ShowWindow(SW_HIDE);
	m_SubFunction8Button.ShowWindow(SW_HIDE);
	this->InvalidateRect(&m_SubFunction8Button.GetRectToInvalidateArrow());
	m_Reference.ShowWindow(SW_HIDE);
	m_SubFunction4DisplayA.ShowWindow(SW_HIDE);
	m_Width2.ShowWindow(SW_HIDE);
	m_ThresholdForContainerBounds2.ShowWindow(SW_HIDE);
	m_Function2Button.ShowWindow(SW_HIDE);
	this->InvalidateRect(&m_Function2Button.GetRectToInvalidateArrow());
	m_Function3Button.ShowWindow(SW_HIDE);
	this->InvalidateRect(&m_Function3Button.GetRectToInvalidateArrow());
	m_SubFunction1Button.ShowWindow(SW_HIDE);
	this->InvalidateRect(&m_SubFunction1Button.GetRectToInvalidateArrow());
	m_SubFunction2Button.ShowWindow(SW_HIDE);
	this->InvalidateRect(&m_SubFunction2Button.GetRectToInvalidateArrow());
	m_SubFunction3Button.ShowWindow(SW_HIDE);
	this->InvalidateRect(&m_SubFunction3Button.GetRectToInvalidateArrow());
	m_SubFunction4Button.ShowWindow(SW_HIDE);
	this->InvalidateRect(&m_SubFunction4Button.GetRectToInvalidateArrow());
	m_SubFunction5Button.ShowWindow(SW_HIDE);
	this->InvalidateRect(&m_SubFunction5Button.GetRectToInvalidateArrow());
	m_SubFunction7Button.ShowWindow(SW_HIDE);
	this->InvalidateRect(&m_SubFunction7Button.GetRectToInvalidateArrow());

	m_SubFunction1Display.ShowWindow(SW_HIDE);
	m_SubFunction2Display.ShowWindow(SW_HIDE);
	m_SubFunction3Display.ShowWindow(SW_HIDE);
	m_Reference.ShowWindow(SW_HIDE);
	m_SubFunction6Display.ShowWindow(SW_HIDE);
	m_Function2Button.ShowWindow(SW_HIDE);
	this->InvalidateRect(&m_Function2Button.GetRectToInvalidateArrow());
	vOldQualityFactor = 0;
	UpdateButtonText();
}

void CAutoSetupImageDisplayDialog::StartThreshold() 
{
	vChangeMade = false;
	vLocalSystemData->vManuallyEditedDuringEvaluate = false;
	vSensitivity = 0xFF; //cause the sensitivity display to be updated
	double TempSensitivity = 0xFF;
	if (vInspectionEditing)
	if (vLocalConfigurationData->vShowSensitivityDisplays == cShowSensitivityDisplays)
		TempSensitivity = vInspectionEditing->GetSensitivity(false, 0);
	if (TempSensitivity < 255)
	{
		m_SubFunction1Display.ShowWindow(SW_SHOW);
		m_SubFunction1Display.SetWindowPos(NULL,0,ResizeYCoor(123),ResizeXCoor(106),ResizeYCoor(18),SWP_NOZORDER);
//		::SetupItem(&m_SubFunction1Display);
	}
	else
		m_SubFunction1Display.ShowWindow(SW_HIDE);
	InitializeThresholdValues();
	vPickOneToEvaluate = false; //since going into threshold, don't do later
	vHaveAnAutoThresholdValue = false;

	CString TempText("");
	TempText.LoadString(IDS_PressSendGoodButtonWhenImagesLookGood);
	//if (vInspectionEditing)
	//	TempText = TempText + ": " + vInspectionEditing->vName;
	SetStatusLine(TempText);
/*
	if (vLocalSystemData->vITIPCDig->vProductFactoredStructureImage != NULL)
	{
		delete vLocalSystemData->vITIPCDig->vProductFactoredStructureImage;
		vLocalSystemData->vITIPCDig->vProductFactoredStructureImage = NULL;
	}
	*/
	TempText.LoadString(IDS_None);
	SetDlgItemText(IDC_Width2,TempText);
	vFunction2ButtonEnable = true;		//m_Function2Button.EnableWindow(true);
	SetDlgItemText(IDC_SubFunction8Button1, _T(""));
	SetDlgItemText(IDC_SubFunction8Button2, _T(""));
	SetDlgItemText(IDC_SubFunction8Button3, _T(""));

	m_Function1Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
	TempText.LoadString(IDS_Abort);
	SetDlgItemText(IDC_Function1Button,TempText);

	m_Function4Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
	m_Function4Button.ShowWindow(SW_SHOW);
	//if (vLocalSystemData->vInAutoSetup)
		TempText.LoadString(IDS_BackToROI);
	//else
	//	TempText.LoadString(IDS_ChangeROI);
	SetDlgItemText(IDC_Function4Button,TempText);

	//if (vLocalSystemData->vInAutoSetup)
		TempText.LoadString(IDS_NextToInspections);
	//else
	//	TempText.LoadString(IDS_ChangeInspections);
	SetDlgItemText(IDC_Function5Button,TempText);

	m_SubFunction5Display.ShowWindow(SW_SHOW);
	TempText.LoadString(IDS_MinGood);
	SetDlgItemText(IDC_SubFunction5Display,TempText);

	TempText.LoadString(IDS_MaxBad);
	SetDlgItemText(IDC_SubFunction7Display,TempText);
	m_SubFunction8Display.ShowWindow(SW_SHOW);
	TempText.LoadString(IDS_Automatic);
	SetDlgItemText(IDC_SubFunction8Display,TempText);

	m_SubFunction6Button.ShowWindow(SW_HIDE);
	this->InvalidateRect(&m_SubFunction6Button.GetRectToInvalidateArrow());
	m_SubFunction6Button1.ShowWindow(SW_SHOW);
	m_SubFunction6Button2.ShowWindow(SW_SHOW);
	m_SubFunction6Button3.ShowWindow(SW_SHOW);

	SetDlgItemText(IDC_Reference, _T(""));
	m_Reference.ShowWindow(SW_SHOW);
	SetDlgItemText(IDC_Simulating, _T(""));

	m_SubFunction4DisplayA.ShowWindow(SW_SHOW);
	TempText.LoadString(IDS_Threshold);
	SetDlgItemText(IDC_SubFunction4DisplayA,TempText);

	m_Width2.ShowWindow(SW_SHOW);
	m_ThresholdForContainerBounds2.ShowWindow(SW_SHOW);
	m_SubFunction1Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
	m_SubFunction2Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
	m_SubFunction3Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
	m_SubFunction4Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
	m_SubFunction1Button.ShowWindow(SW_SHOW);
	if (vInspectionEditing)
	{
		if ((PasswordOK(cTemporaryInspxPassword,false)) && (vInspectionEditing) && (vInspectionEditing->vHasAMinimumSize) && (vInspectionEditing->vInspectionType != cItemCountInspection) &&
			((((vInspectionEditing->vInspectionType != cItemCountInspection) && (vInspectionEditing->vInspectionType != cSizeInspection) && (vInspectionEditing->vInspectionType != cMissingLidInspectionOnSize)) && 
			(vInspectionEditing->vInspectionType != cMissingLidInspectionOnSize)) || ((PasswordOK(cSuperInspxPassword,false) && (vInspectionEditing->vInspectionType != cItemCountInspection)))))
		{
			CString TempText;
			if ((PasswordOK(cSuperInspxPassword,false)) || (vInspectionEditing->vAllowChangingAdvancedOptions))
				TempText.LoadString(IDS_InspectionSettings);
			else
			if (vInspectionEditing->vInspectionType == cFloodedInspection)
				TempText = "Min. Spout Length";
			else
				TempText.LoadString(IDS_MinimumSize);
			SetDlgItemText(IDC_Function2Button,TempText);
			m_Function2Button.ShowWindow(SW_SHOW);
		}
		else
		if (vInspectionEditing->vInspectionType == cFloodedInspection)
		{
			SetDlgItemText(IDC_Function2Button, _T("Min. Spout Length"));
			m_Function2Button.ShowWindow(SW_SHOW);
		}
		else
		{
			m_Function2Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_Function2Button.GetRectToInvalidateArrow());
		}
	}
	m_SubFunction2Button.ShowWindow(SW_SHOW);
	m_SubFunction3Button.ShowWindow(SW_SHOW);
	m_SubFunction4Button.ShowWindow(SW_SHOW);

	m_SubFunction5Button.ShowWindow(SW_SHOW);
	this->InvalidateRect(&m_Function2Button.GetRectToInvalidateArrow());

	SetDlgItemText(IDC_SubFunction1Button, _T("+10"));
	SetDlgItemText(IDC_SubFunction2Button, _T("+1"));
	SetDlgItemText(IDC_SubFunction3Button, _T("-1"));
	SetDlgItemText(IDC_SubFunction4Button, _T("-10"));

	m_SubFunction7Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
	m_SubFunction7Button.ShowWindow(SW_SHOW);

	if ((vInspectionEditing) && (vInspectionEditing->vFindBadIntensity))
	{
		TempText.LoadString(IDS_StopProcessingBad);
		m_SubFunction7Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL); //3 lines of text
		m_SubFunction7Button.ShowWindow(SW_SHOW);
	}
	else
	if (PasswordOK(cAdvancedSetupPassword,false))
	{
		if ((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode))
			TempText = "Send Bad Product";
		else
			TempText.LoadString(IDS_SendBadContainersButton);
		m_SubFunction7Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		m_SubFunction7Button.ShowWindow(SW_SHOW);
	}
	else
	{
		m_SubFunction7Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction7Button.GetRectToInvalidateArrow());
	}

	SetDlgItemText(IDC_SubFunction7Button,TempText);

	UpdateButtonText();
	UpdateFunction3Button();
	DisplayWithPassword();
	UpdateAverageDensityDisplay();

	if (!vLocalSystemData->vITIPCDig->vDriftCompensationLocked)
		GiveSendGoodContainersNotice();
}

void CAutoSetupImageDisplayDialog::StartDensityThreshold() 
{
	if (vInspectionEditing)
	{
		CString TempText("");
		vMode = cSetDensityThreshold;
		vTookNewDensityData = false;
		SetDlgItemText(IDC_SubFunction7Display, _T(" "));

		vAutomaticDensityThresholdUpper = vInspectionEditing->vDensityThresholdUpper;
		vAutomaticDensityThresholdLower = vInspectionEditing->vDensityThresholdLower;

		if ((vInspectionEditing->vSetupHistoryQualityFactor < 0) || (vInspectionEditing->vSetupHistoryQualityFactor > 99999))
			vInspectionEditing->vSetupHistoryQualityFactor = 0;

		vLocalSystemData->vQualityFactor = vInspectionEditing->vSetupHistoryQualityFactor;
		vOldQualityFactor = vLocalSystemData->vQualityFactor;
		if (vLocalSystemData->vQualityFactor == 0)
			SetDlgItemText(IDC_Simulating, _T(""));
		else
		if (vInspectionEditing->vInspectionType != cUnderfillByWeightInspection)
		if (vInspectionEditing->vInspectionType != cOverfillByWeightInspection)
		if (vInspectionEditing->vInspectionType != cCheckWeighInspection)
		{
			CString TempString = dtoa(vLocalSystemData->vQualityFactor,2);
			TempText.LoadString(IDS_QF);
			SetDlgItemText(IDC_Simulating,TempText + ": " + TempString);
		}

		if (vLocalSystemData->vInAutoSetup)
		{
			if (vInspectionEditing)
			{	
				vDialogTitleString.LoadString(IDS_Evaluate);
				vDialogTitleString = "Auto " + vDialogTitleString + " " + vInspectionEditing->vName;
			}
			else
			if (vInspectionEditing->vInspectionType == cStandardDeviationInspection)
				vDialogTitleString.LoadString(IDS_AutoSetupTextureThreshold);
			else
			if (vInspectionEditing->vInspectionType == cUnderfillByWeightInspection)
				vDialogTitleString.LoadString(IDS_AutoSetupLowWeightThreshold);
			else
			if (vInspectionEditing->vInspectionType == cOverfillByWeightInspection)
				vDialogTitleString.LoadString(IDS_AutoSetupOverWeightThreshold);
			else
			if (vInspectionEditing->vInspectionType == cCheckWeighInspection)
				vDialogTitleString.LoadString(IDS_AutoSetupWeightTrending);
			else
				vDialogTitleString.LoadString(IDS_AutoSetupDensityThreshold);
		}
		else
		{
			if (vInspectionEditing)
			{	
				vDialogTitleString.LoadString(IDS_Evaluate);
				vDialogTitleString = vDialogTitleString + " " + vInspectionEditing->vName;
			}
			else
			if (vInspectionEditing->vInspectionType == cStandardDeviationInspection)
				vDialogTitleString.LoadString(IDS_SetupTextureThreshold);
			else
			if (vInspectionEditing->vInspectionType == cUnderfillByWeightInspection)
				vDialogTitleString.LoadString(IDS_UnderfillByWeightThreshold);
			else
			if (vInspectionEditing->vInspectionType == cOverfillByWeightInspection)
				vDialogTitleString.LoadString(IDS_SetupOverWeightThreshold);
			else
			if (vInspectionEditing->vInspectionType == cCheckWeighInspection)
				vDialogTitleString.LoadString(IDS_SetupWeightTrending);
			else
				vDialogTitleString.LoadString(IDS_SetupDensityThreshold);
		}

		SetDlgItemText(IDC_DialogTitleStaticText1,vDialogTitleString);

		DisplayWithPassword();
		TempText.LoadString(IDS_PressStartButtonWhenImagesLookGood);
		//if (vInspectionEditing)
		//	TempText = TempText + ": " + vInspectionEditing->vName;
		SetStatusLine(TempText);

		TempText.LoadString(IDS_Abort);
		SetDlgItemText(IDC_Function1Button, TempText);

		m_SubFunction4Display.ShowWindow(SW_SHOW);

		TempText.LoadString(IDS_MinGood);
		SetDlgItemText(IDC_SubFunction5Display,TempText);

		m_Function4Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		m_Function4Button.ShowWindow(SW_SHOW);
		//if (vLocalSystemData->vInAutoSetup)
			TempText.LoadString(IDS_BackToROI);
		//else
		//	TempText.LoadString(IDS_ChangeROI);

		SetDlgItemText(IDC_Function4Button,TempText);

		TempText.LoadString(IDS_StartAveragingDensity);
		SetDlgItemText(IDC_SubFunction5Button,TempText);

		CString TempString("");
		TempString = "";
		m_SubFunction2Display.ShowWindow(SW_SHOW);
		//TempText.LoadString(IDS_Manual);
		//SetDlgItemText(IDC_SubFunction1Display,TempText);

		if (vInspectionEditing->vHasDensityUpperThreshold)
		{
			m_SubFunction2Display.ShowWindow(SW_SHOW);
			m_SubFunction2Button.ShowWindow(SW_SHOW);

			DisplayDensityThresholdUpper();

			m_SubFunction2Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);

			m_SubFunction4Display.ShowWindow(SW_SHOW);
			DisplayAutoDensityThresholdUpper();
		}
		else
		{
			m_SubFunction2Display.ShowWindow(SW_HIDE);
			m_SubFunction2Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_SubFunction2Button.GetRectToInvalidateArrow());
			if (!((vInspectionEditing->vInspectionType == cCheckWeighInspection) || (vInspectionEditing->vInspectionType == cUnderfillByWeightInspection) || (vInspectionEditing->vInspectionType == cOverfillByWeightInspection)))
				m_SubFunction4Display.ShowWindow(SW_HIDE);
		}

		if (vInspectionEditing->vHasDensityLowerThreshold)
		{
			m_SubFunction3Display.ShowWindow(SW_SHOW);
			DisplayDensityThresholdLower();
			m_SubFunction3Button.ShowWindow(SW_SHOW);
			m_SubFunction3Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);

			m_SubFunction4DisplayA.ShowWindow(SW_SHOW);
			DisplayAutoDensityThresholdLower();
		}
		else
		{
			m_SubFunction4DisplayA.ShowWindow(SW_HIDE);
			m_SubFunction3Display.ShowWindow(SW_HIDE);
			m_SubFunction3Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_SubFunction3Button.GetRectToInvalidateArrow());
		}
		if ((vInspectionEditing->vInspectionType == cCheckWeighInspection) || (vInspectionEditing->vInspectionType == cUnderfillByWeightInspection) || (vInspectionEditing->vInspectionType == cOverfillByWeightInspection))
			m_SubFunction4DisplayA.ShowWindow(SW_HIDE);

		vChangeMade = false;

		if (vInspectionEditing->vInspectionType == cStandardDeviationInspection)
			TempText.LoadString(IDS_Texture);
		else
		if ((vInspectionEditing->vInspectionType == cUnderfillByWeightInspection) || (vInspectionEditing->vInspectionType == cOverfillByWeightInspection) || (vInspectionEditing->vInspectionType == cCheckWeighInspection))
			TempText.LoadString(IDS_Weight);
		else
			TempText.LoadString(IDS_Density);
		SetDlgItemText(IDC_Width2,TempText + ":");

		m_Width2.ShowWindow(SW_SHOW);
		SetDlgItemText(IDC_Reference, _T("_"));
		m_Reference.ShowWindow(SW_SHOW);

		m_SubFunction5Display.ShowWindow(SW_SHOW);
		SetDlgItemText(IDC_SubFunction6Button1, _T(""));
		SetDlgItemText(IDC_SubFunction6Button2, _T(""));
		SetDlgItemText(IDC_SubFunction6Button3, _T(""));
		
		vLocalSystemData->vMaximumBadIntensity = vInspectionEditing->vSetupHistoryMaximumBad;
		vLocalSystemData->vMaximumBadCount = vInspectionEditing->vSetupHistoryMaximumBadCount;
		vLocalSystemData->vMaximumBadAverage = vInspectionEditing->vSetupHistoryMaximumBadAverage;
		vLocalSystemData->vMaximumBadStandardDeviation = vInspectionEditing->vSetupHistoryMaximumBadStandardDeviation;
		if (vInspectionEditing->vSetupHistoryMaximumBadCount)
		{
			m_SubFunction8Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_SubFunction8Button.GetRectToInvalidateArrow());
			m_SubFunction8Button1.ShowWindow(SW_SHOW);
			m_SubFunction8Button2.ShowWindow(SW_SHOW);
			m_SubFunction8Button3.ShowWindow(SW_SHOW);
			m_SubFunction7Display.ShowWindow(SW_SHOW);
		}
		else
		{
			m_SubFunction8Button1.ShowWindow(SW_HIDE);
			m_SubFunction8Button2.ShowWindow(SW_HIDE);
			m_SubFunction8Button3.ShowWindow(SW_HIDE);
			m_SubFunction7Display.ShowWindow(SW_HIDE);
			m_SubFunction8Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_SubFunction8Button.GetRectToInvalidateArrow());
		}

		DisplayMinimumGoodMaximumBadDataValues(false);

		m_SubFunction6Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction6Button.GetRectToInvalidateArrow());
		m_SubFunction7Button.ShowWindow(SW_SHOW);
		m_SubFunction6Button1.ShowWindow(SW_SHOW);
		m_SubFunction6Button2.ShowWindow(SW_SHOW);
		m_SubFunction6Button3.ShowWindow(SW_SHOW);

		m_ThresholdForContainerBounds2.ShowWindow(SW_HIDE);

		//m_SubFunction1Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
		m_Function2Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_Function2Button.GetRectToInvalidateArrow());
		this->InvalidateRect(&m_SubFunction4Button.GetRectToInvalidateArrow());

		m_SubFunction5Button.ShowWindow(SW_SHOW);
		m_Function5Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		if ((vAutoSetupType == cEvaluateAutoSetup) && (vGlobalCurrentProduct->vNumberOfInspections == 1))
		{
			m_Function2Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
			if (vLocalSystemData->vInAutoSetup)
				TempText.LoadString(IDS_FinishAutoSetup);
			else
				TempText.LoadString(IDS_FinishSetup);
		}
		else
		{
			//if (vLocalSystemData->vInAutoSetup)
				TempText.LoadString(IDS_NextToInspections);
			//else
			//	TempText.LoadString(IDS_ChangeInspections);
		}
		SetDlgItemText(IDC_Function5Button,TempText);

		if (vInspectionEditing->vInspectionType != cDensityInspection)
		if (vInspectionEditing->vInspectionType != cUnderfillByWeightInspection)
		if (vInspectionEditing->vInspectionType != cOverfillByWeightInspection)
		if (vInspectionEditing->vInspectionType != cCheckWeighInspection)
		{
			TempString = dtoa(vInspectionEditing->vSetupHistoryQualityFactor,2);
			TempText.LoadString(IDS_QF);
			SetDlgItemText(IDC_Simulating,TempText + ": " + TempString);
		}	

		if ((vInspectionEditing->vInspectionType == cCheckWeighInspection) || (vInspectionEditing->vInspectionType == cUnderfillByWeightInspection) || (vInspectionEditing->vInspectionType == cOverfillByWeightInspection))
		{
			TempText.LoadString(IDS_MinimumGoodWeight);
			SetDlgItemText(IDC_SubFunction1Button,TempText);
			TempText.LoadString(IDS_MaximumGoodWeight);
			SetDlgItemText(IDC_SubFunction4Button,TempText);

			m_SubFunction1Display.ShowWindow(SW_SHOW);
			m_SubFunction1Button.ShowWindow(SW_SHOW);
			m_SubFunction1Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);

			m_SubFunction2Display.ShowWindow(SW_SHOW);
			m_SubFunction2Button.ShowWindow(SW_SHOW);

			m_SubFunction3Display.ShowWindow(SW_SHOW);
			m_SubFunction3Button.ShowWindow(SW_SHOW);

			m_SubFunction4Button.ShowWindow(SW_SHOW);
			m_SubFunction4Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
			m_Function2Button.ShowWindow(SW_SHOW);

			if (vInspectionEditing->vInspectionNameForWeightTrendingPointer) //if wieght trending inspection, use other inspections settings
			{
				vFunction2ButtonEnable = false;		//m_Function2Button.EnableWindow(false);
				vSubFunction1ButtonEnable = false;	//m_SubFunction1Button.EnableWindow(false);
				m_SubFunction1Display.ShowWindow(SW_HIDE);
				if ((vInspectionEditing) && (vInspectionEditing->vInspectionType == cUnderfillByWeightInspection))
				{
					vSubFunction3ButtonEnable = false;		//m_SubFunction3Button.EnableWindow(false);
					m_SubFunction3Display.ShowWindow(SW_HIDE);
				}
				if ((vInspectionEditing) && (vInspectionEditing->vInspectionType == cOverfillByWeightInspection))
				{
					vSubFunction2ButtonEnable = false;		//m_SubFunction2Button.EnableWindow(false);
					m_SubFunction2Display.ShowWindow(SW_HIDE);
					m_SubFunction2Button.Invalidate(false);
				}
				vSubFunction4ButtonEnable = false;		//m_SubFunction4Button.EnableWindow(false);
				m_SubFunction4Display.ShowWindow(SW_HIDE);

				vSubFunction5ButtonEnable = false;		//m_SubFunction5Button.EnableWindow(false);
				vSubFunction7ButtonEnable = false;		//m_SubFunction7Button.EnableWindow(false);

			}
	//		if (vGlobalCurrentProduct->AreThereAnyOtherWeightTrendingInspections(vInspectionNumberEditing))
			{
				m_Function1Button.ShowWindow(SW_SHOW);
				m_Function1Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
				SetDlgItemText(IDC_Function1Button, _T("Trending Inspection"));
			}
		//	else
		//	{;
		//		m_Function1Button.ShowWindow(SW_HIDE);
			//	this->InvalidateRect(&m_Function1Button.GetRectToInvalidateArrow());
		//	}
		}
		UpdateButtonText();
		UpdateFunction3Button();
		DrawImageOnScreen();
	}

} //end of StartDensityThreshold

void CAutoSetupImageDisplayDialog::StartContaminantThreshold() 
{
	vMode = cSetContaminantThreshold;
	StartThreshold();

	if (vInspectionEditing)
	{
		SetDlgItemText(IDC_ThresholdForContainerBounds2,dtoa(vInspectionEditing->vThreshold,0));

		if (vInspectionEditing->vInspectionType == cKernelInspection)
		{
			if (vInspectionEditing)
			{	
				vDialogTitleString.LoadString(IDS_Evaluate);
				vDialogTitleString = vDialogTitleString + " " + vInspectionEditing->vName;
				if (vLocalSystemData->vInAutoSetup)
					vDialogTitleString = "Auto " + vDialogTitleString;
			}
			else
			if (vLocalSystemData->vInAutoSetup)
				vDialogTitleString.LoadString(IDS_AutoSetupKernelThreshold);
			else
				vDialogTitleString.LoadString(IDS_SetupKernelThreshold);
		}
		else
		{
			if (vInspectionEditing)
			{	
				vDialogTitleString.LoadString(IDS_Evaluate);
				vDialogTitleString = vDialogTitleString + " " + vInspectionEditing->vName;
				if (vLocalSystemData->vInAutoSetup)
					vDialogTitleString = "Auto " + vDialogTitleString;
			}
			else
			if (vLocalSystemData->vInAutoSetup)
				vDialogTitleString.LoadString(IDS_AutoSetupContaminantThreshold);
			else
				vDialogTitleString.LoadString(IDS_SetupContaminantThreshold);
		}
		SetDlgItemText(IDC_DialogTitleStaticText1,vDialogTitleString);

		vOldMinimumGoodCount = vInspectionEditing->vSetupHistoryMinimumGoodCount;
		vOldMaximumBadCount = vInspectionEditing->vSetupHistoryMaximumBadCount;

		CString TempString;
		CString TempText;
		if (vInspectionEditing->vSetupHistoryMinimumGoodAverage)
		{
			TempText.LoadString(IDS_Min);
			SetDlgItemText(IDC_SubFunction6Button3,TempText + ": " + 
				dtoa(vInspectionEditing->vSetupHistoryMinimumGood,0));

			if (vInspectionEditing->vInspectionType != cJarBottomContaminantInspection)
			{
				TempString = dtoa(vInspectionEditing->vSetupHistoryMinimumGoodAverage,2);
				TempText.LoadString(IDS_Avg);
				SetDlgItemText(IDC_SubFunction6Button1,TempText + ": " + TempString);

				TempString = dtoa(vInspectionEditing->vSetupHistoryMinimumGoodStandardDeviation,2);
				TempText.LoadString(IDS_SD);
				SetDlgItemText(IDC_SubFunction6Button2,TempText + ": " + TempString);
				m_SubFunction6Button.ShowWindow(SW_HIDE);
				this->InvalidateRect(&m_SubFunction6Button.GetRectToInvalidateArrow());
			}
		}
		else
		{
			SetDlgItemText(IDC_SubFunction6Button1, _T(""));
			SetDlgItemText(IDC_SubFunction6Button2, _T(""));
			SetDlgItemText(IDC_SubFunction6Button3, _T(""));
		}

		vLocalSystemData->vMaximumBadIntensity = vInspectionEditing->vSetupHistoryMaximumBad;
		vLocalSystemData->vMaximumBadCount = vInspectionEditing->vSetupHistoryMaximumBadCount;
		vLocalSystemData->vMaximumBadAverage = vInspectionEditing->vSetupHistoryMaximumBadAverage;
		vLocalSystemData->vMaximumBadStandardDeviation = vInspectionEditing->vSetupHistoryMaximumBadStandardDeviation;

		if ((vInspectionEditing->vSetupHistoryQualityFactor < 0) || (vInspectionEditing->vSetupHistoryQualityFactor > 99999))
			vInspectionEditing->vSetupHistoryQualityFactor = 0;

		vLocalSystemData->vQualityFactor = vInspectionEditing->vSetupHistoryQualityFactor;

		vOldQualityFactor = vLocalSystemData->vQualityFactor;
		if (vLocalSystemData->vQualityFactor == 0)
			SetDlgItemText(IDC_Simulating, _T(""));
		else
		{
			CString TempString = dtoa(vLocalSystemData->vQualityFactor,2);
			TempText.LoadString(IDS_QF);
			SetDlgItemText(IDC_Simulating,TempText + ": " + TempString);
		}
		
		//if (vInspectionEditing->vSetupHistoryMaximumBadAverage)
		if ((vInspectionEditing->vSetupHistoryMaximumBadCount > 4) && (vInspectionEditing->vSetupHistoryMinimumGoodCount > 4))
		{
			CString TempText("");
			if ((vInspectionEditing->vCheckWeighMFactor != 1) || (vInspectionEditing->vCheckWeighBFactor))
				SetDlgItemText(IDC_SubFunction8Button3, "Min: " + dtoa(vInspectionEditing->vSetupHistoryMaximumBad,0));
			else
				SetDlgItemText(IDC_SubFunction8Button3, "Max: " + dtoa(vInspectionEditing->vSetupHistoryMaximumBad,0));

			TempString = dtoa(vInspectionEditing->vSetupHistoryMaximumBadAverage,2);
			TempText.LoadString(IDS_Avg);
			SetDlgItemText(IDC_SubFunction8Button1,TempText + ": " + TempString);

			TempString = dtoa(vInspectionEditing->vSetupHistoryMaximumBadStandardDeviation,2);
			TempText.LoadString(IDS_SD);
			SetDlgItemText(IDC_SubFunction8Button2,TempText + ": " + TempString);

			//if ((vInspectionEditing->vInspectionType != cSizeInspection) && (vInspectionEditing->vInspectionType != cMissingLidInspectionOnSize))
			{
				vLocalSystemData->CalculateQualityFactor();
				TempString = dtoa(vInspectionEditing->vSetupHistoryQualityFactor,2);
				TempText.LoadString(IDS_QF);
				SetDlgItemText(IDC_Simulating,TempText + ": " + TempString);
			}	
		}

		UpdateButtonText();
		DrawImageOnScreen();
		if ((vInspectionEditing) && ((vInspectionEditing->vInspectionType == cItemCountInspection) || (vInspectionEditing->vInspectionType == cSizeInspection) || (vInspectionEditing->vInspectionType == cMissingLidInspectionOnSize)))
		if (vLocalSystemData->vITIPCDig->vOriginalImage)
		{
			if (vInspectionEditing)
			{	
				vDialogTitleString.LoadString(IDS_Evaluate);
				vDialogTitleString = vDialogTitleString + " " + vInspectionEditing->vName;
				if (vLocalSystemData->vInAutoSetup)
					vDialogTitleString = "Auto " + vDialogTitleString;
			}
			else
			if (vLocalSystemData->vInAutoSetup)
				vDialogTitleString.LoadString(IDS_AutoSetupSizeThreshold);
			else
				vDialogTitleString.LoadString(IDS_SetupSizeThreshold);
			SetDlgItemText(IDC_DialogTitleStaticText1,vDialogTitleString);
			m_Reference.ShowWindow(SW_SHOW);
			SetDlgItemText(IDC_Reference, dtoa(vLocalSystemData->vITIPCDig->vOriginalImage->vNumberOfBlobs[vInspectionNumberEditing],0));
			m_SubFunction8Display.ShowWindow(SW_HIDE);
			m_Width2.ShowWindow(SW_HIDE);
		}
		if ((vInspectionEditing) && (vInspectionEditing->vInspectionType == cFloodedInspection))
		if (vLocalSystemData->vITIPCDig->vOriginalImage)
		{
			if (vLocalSystemData->vInAutoSetup)
				vDialogTitleString =  "Auto Setup Minimum Spout Length";
			else
				vDialogTitleString =  "Auto Setup Minimum Spout Length";
			SetDlgItemText(IDC_DialogTitleStaticText1,vDialogTitleString);
			//m_Reference.ShowWindow(SW_SHOW);
			//SetDlgItemText(IDC_Reference, dtoa(vLocalSystemData->vITIPCDig->vOriginalImage->vNumberOfBlobs[vInspectionNumberEditing],0));
			//m_SubFunction8Display.ShowWindow(SW_HIDE);
			//m_Width2.ShowWindow(SW_HIDE);
		}
	}
	ShowSensitivity();
	ShowLearnSensitivity();
	if (vLocalSystemData->vLearnSensitivities[vInspectionNumberEditing] < 255)
	{
		m_SubFunction2Display.ShowWindow(SW_SHOW);

	}

	if ((vInspectionEditing) && ((vInspectionEditing->vInspectionType == cItemCountInspection) || (vInspectionEditing->vInspectionType == cSizeInspection) || (vInspectionEditing->vInspectionType == cMissingLidInspectionOnSize)))
	{
		m_Function1Button.ShowWindow(SW_SHOW);
		m_Function1Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		if (vInspectionEditing->vInspectionType == cItemCountInspection)
			SetDlgItemText(IDC_Function1Button, _T("Size\nSettings"));
		else
			SetDlgItemText(IDC_Function1Button, "Size\n" + dtoa(vInspectionEditing->vSize, 0));
	}
}

void CAutoSetupImageDisplayDialog::StartVoidThreshold() 
{
	vMode = cSetVoidThreshold;
	StartThreshold();

	if (vInspectionEditing)
	{
		SetDlgItemText(IDC_ThresholdForContainerBounds2,dtoa(vInspectionEditing->vThreshold,0));

		CString TempString("");
		TempString = "";
		if (vInspectionEditing)
		{	
			vDialogTitleString.LoadString(IDS_Evaluate);
			vDialogTitleString = vDialogTitleString + " " + vInspectionEditing->vName;
			if (vLocalSystemData->vInAutoSetup)
				vDialogTitleString = "Auto " + vDialogTitleString;
		}
		else
		if (vLocalSystemData->vInAutoSetup)
			vDialogTitleString.LoadString(IDS_AutoSetupVoidThreshold);
		else
			vDialogTitleString.LoadString(IDS_SetupVoidThreshold);
		SetDlgItemText(IDC_DialogTitleStaticText1,vDialogTitleString);
			
		vOldMinimumGoodCount = vInspectionEditing->vSetupHistoryMinimumGoodCount;
		vOldMaximumBadCount = vInspectionEditing->vSetupHistoryMaximumBadCount;

		if (vInspectionEditing->vSetupHistoryMinimumGoodAverage)
		{
			CString TempText("");
			TempText.LoadString(IDS_Min);
			SetDlgItemText(IDC_SubFunction6Button3,TempText + ": " + 
				dtoa(vInspectionEditing->vSetupHistoryMinimumGood,0));

			TempString = dtoa(vInspectionEditing->vSetupHistoryMinimumGoodAverage,2);
			TempText.LoadString(IDS_Avg);
			SetDlgItemText(IDC_SubFunction6Button1,TempText + ": " + TempString);

			TempString = dtoa(vInspectionEditing->vSetupHistoryMinimumGoodStandardDeviation,2);
			TempText.LoadString(IDS_SD);
			SetDlgItemText(IDC_SubFunction6Button2,TempText + ": " + TempString);
			m_SubFunction6Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_SubFunction6Button.GetRectToInvalidateArrow());
		}
		 else
		 {
	 		SetDlgItemText(IDC_SubFunction6Button1, _T(""));
	 		SetDlgItemText(IDC_SubFunction6Button2, _T(""));
	 		SetDlgItemText(IDC_SubFunction6Button3, _T(""));
		 }

		 vLocalSystemData->vMinimumGoodIntensity = vInspectionEditing->vSetupHistoryMinimumGood;
		 vLocalSystemData->vMinimumGoodAverage = vInspectionEditing->vSetupHistoryMinimumGoodAverage;
		 vLocalSystemData->vMinimumGoodStandardDeviation = vInspectionEditing->vSetupHistoryMinimumGoodStandardDeviation;


		 vLocalSystemData->vMaximumBadIntensity = vInspectionEditing->vSetupHistoryMaximumBad;
		 vLocalSystemData->vMaximumBadAverage = vInspectionEditing->vSetupHistoryMaximumBadAverage;
		 vLocalSystemData->vMaximumBadStandardDeviation = vInspectionEditing->vSetupHistoryMaximumBadStandardDeviation;

			if ((vInspectionEditing->vSetupHistoryQualityFactor < 0) || (vInspectionEditing->vSetupHistoryQualityFactor > 99999))
				vInspectionEditing->vSetupHistoryQualityFactor = 0;

		 vLocalSystemData->vQualityFactor = vInspectionEditing->vSetupHistoryQualityFactor;
		 
		 if (vInspectionEditing->vSetupHistoryMaximumBadAverage)
		{
			CString TempText("");
			if ((vInspectionEditing->vCheckWeighMFactor != 1) || (vInspectionEditing->vCheckWeighBFactor))
				SetDlgItemText(IDC_SubFunction8Button3, "Min: " + dtoa(vInspectionEditing->vSetupHistoryMaximumBad,0));
			else
				SetDlgItemText(IDC_SubFunction8Button3, "Max: " + dtoa(vInspectionEditing->vSetupHistoryMaximumBad,0));

			TempString = dtoa(vInspectionEditing->vSetupHistoryMaximumBadAverage,2);
			TempText.LoadString(IDS_Avg);
			SetDlgItemText(IDC_SubFunction8Button1,TempText +": " + TempString);

			TempString = dtoa(vInspectionEditing->vSetupHistoryMaximumBadStandardDeviation,2);
			TempText.LoadString(IDS_SD);
			SetDlgItemText(IDC_SubFunction8Button2,TempText +": " + TempString);
		}
	}
	vLocalSystemData->CalculateQualityFactor();

	UpdateButtonText();
	DrawImageOnScreen();
}

bool CAutoSetupImageDisplayDialog::FinishVoidThreshold(bool TempWarn)
{
	/*
	bool TempFinishVoidThreshold = true;

	if (vInspectionEditing)
	{
		TempFinishVoidThreshold = false;

		if (vOldMaximumBadIntensity != 0)
		{
			 vInspectionEditing->vSetupHistoryMaximumBad = vOldMaximumBadIntensity;
			 vInspectionEditing->vSetupHistoryMaximumBadCount = vOldMaximumBadCount;
		}
		if (vOldMaximumBadAverage != 0)
		{
			 vInspectionEditing->vSetupHistoryMaximumBadCount = vOldMaximumBadCount;

			 vInspectionEditing->vSetupHistoryMaximumBadAverage = vOldMaximumBadAverage;
			 vInspectionEditing->vSetupHistoryMaximumBadStandardDeviation = 
		 		vOldMaximumBadStandardDeviation;
		}
		if (vOldMinimumGoodIntensity != 255)
		{
			vInspectionEditing->vSetupHistoryMinimumGood = vOldMinimumGoodIntensity;
			vInspectionEditing->vSetupHistoryMinimumGoodCount = vOldMinimumGoodCount;
		}
		if (vOldMinimumGoodAverage != 0)
		{
			 vInspectionEditing->vSetupHistoryMinimumGoodAverage = vOldMinimumGoodAverage;
			 vInspectionEditing->vSetupHistoryMinimumGoodStandardDeviation =vOldMinimumGoodStandardDeviation;
		}

		//if automatic algorithm to find threshold completed and got a good value
		if (((vOldMaximumBadIntensity != 0) ||
			(vOldMinimumGoodIntensity != 255)) &&
			(vOldMinimumGoodIntensity > vOldMaximumBadIntensity) &&
			(vOldMaximumBadAverage) && (vOldMinimumGoodAverage))
		{
			TempFinishVoidThreshold = true;
			bool TempUseAutoThreshold = true;
			if (vChangeMade)
			{
				TempUseAutoThreshold = false;
			}
			if (TempUseAutoThreshold)  //user wants to use automatic threshold
			{
				if (vLocalSystemData->vMinimumGoodIntensity > 
					vLocalSystemData->vMaximumBadIntensity)
				{
					BYTE TempDifference = (BYTE)(vLocalSystemData->vMinimumGoodIntensity - 
						vLocalSystemData->vMaximumBadIntensity);
					 vInspectionEditing->vThreshold = 
				 		(BYTE)vLocalSystemData->vMaximumBadIntensity + (TempDifference / 2);
					vInspectionEditing->vSetupHistoryAutoThreshold = vInspectionEditing->vThreshold;
				}
				else
					TempFinishVoidThreshold = false;
			}
		}
		else
		{
			BYTE TempThreshold = cDefaultThreshold;
			 if (!vInspectionEditing->vEnhanceMethod)
		 		TempThreshold = TempThreshold / 2;
			if (vChangeMade)  //if did not edit manually
				TempFinishVoidThreshold = true;
			else
			if (vInspectionEditing->vThreshold == TempThreshold)
			{
				if ((vOldMinimumGoodIntensity != 255) &&
					(vLocalSystemData->vMinimumGoodStandardDeviation) &&
					(vLocalSystemData->vMinimumGoodIntensity > 
					(3 * vLocalSystemData->vMinimumGoodStandardDeviation)))
				{
					TempFinishVoidThreshold = true;
					 vInspectionEditing->vThreshold = 
				 		(BYTE)(vLocalSystemData->vMinimumGoodIntensity - 
				 		(3 * vLocalSystemData->vMinimumGoodStandardDeviation));
					vInspectionEditing->vSetupHistoryAutoThreshold = vInspectionEditing->vThreshold;
					//CNoticeDialog TempNoticeDialog;
					//CString TempText("");
					//TempText.LoadString(IDS_SettingVoidThresholdToMinimumGood);
					//TempNoticeDialog.vNoticeText = TempText;
					//TempNoticeDialog.vType = cInstructionsMessage;
					//TempNoticeDialog.DoModal();
				}
				else
				if (TempWarn) //if going back, don't warn user
				{
					TempFinishVoidThreshold = false;
					CYesNoDialog TempYesNoDialog;
					CString TempText;
					TempText.LoadString(IDS_VoidThresholdNotSetContinueAnyway);
					TempYesNoDialog.vNoticeText = TempText;
					CString TempText1;
					TempText1.LoadString(IDS_ThresholdLater);
					TempYesNoDialog.vYesButtonText = TempText1;
					TempYesNoDialog.vQuestionType = cConfirmQuestion;
					int TempResult = TempYesNoDialog.DoModal();
					if (TempResult == IDOK)
						TempFinishVoidThreshold = true;
				}
			}
			// else
		 	//	TempFinishVoidThreshold = true;

		}
*/		//if ((!TempWarn) || (TempFinishVoidThreshold))
		//{
			WriteCSVFiles();
		//}
		//if (TempFinishVoidThreshold)
			FinishThreshold();
	//}
	vChangeMade = false;
	return true; //TempFinishVoidThreshold;
}

bool CAutoSetupImageDisplayDialog::FinishDensityThreshold(bool TempWarn)
{
	bool TempFinishDensityThreshold = true;
	if (vInspectionEditing)
	{
		if (TempWarn)
		if (!PasswordOK(cSuperInspxPassword,false))
		if (!vInspectionEditing->vInspectionNameForWeightTrendingPointer)
		if (((vInspectionEditing->vInspectionType == cUnderfillByWeightInspection) || (vInspectionEditing->vInspectionType == cOverfillByWeightInspection) || 
			(vInspectionEditing->vInspectionType == cCheckWeighInspection)) &&
			(((vInspectionEditing->vSetupHistoryMaximumBadCount) && (vInspectionEditing->vSetupHistoryMinimumGoodCount < 4)) || //if you have evaluated one sample, but not the other, clear both
			((vInspectionEditing->vSetupHistoryMaximumBadCount < 4) && (vInspectionEditing->vSetupHistoryMinimumGoodCount))))
		{
			TempFinishDensityThreshold = false;
			CYesNoDialog TempYesNoDialog;
			CString TempText = "You have not finished calibrating the weight\nwith both the light and the heavy samples.\nIf you exit, samples done will be deleted.\n" + 
				dtoa(vInspectionEditing->vSetupHistoryMinimumGoodCount, 0) + " Light and " + dtoa(vInspectionEditing->vSetupHistoryMaximumBadCount, 0) + " Heavy have been sent.\nNeed 4 or more each.";
			TempYesNoDialog.vNoticeText = TempText;
			TempYesNoDialog.vYesButtonText = "Return To Calibration";
			TempYesNoDialog.vNoButtonText = "Proceed To Abandon";
			TempYesNoDialog.vQuestionType = cConfirmQuestion;
			int TempResult = TempYesNoDialog.DoModal();
			if (TempResult != IDOK)
			{
				TempFinishDensityThreshold = true;
				vInspectionEditing->vSetupHistoryMaximumBadCount = 0;
				vInspectionEditing->vSetupHistoryMinimumGoodCount = 0;
			}
		}
		else
		if (((vInspectionEditing->vInspectionType == cUnderfillByWeightInspection) || (vInspectionEditing->vInspectionType == cOverfillByWeightInspection) || 
			(vInspectionEditing->vInspectionType == cCheckWeighInspection)) && ((vInspectionEditing->vSetupHistoryMinimumGoodCount < 4) || (vInspectionEditing->vSetupHistoryMaximumBadCount < 4)))
		{
			TempFinishDensityThreshold = false;
			CYesNoDialog TempYesNoDialog;
			CString TempText = "You have not finished calibrating the weight.\nThis inspection will not inspect properly until calibration is completed.\n" + 
				dtoa(vInspectionEditing->vSetupHistoryMinimumGoodCount, 0) + " Light and " + dtoa(vInspectionEditing->vSetupHistoryMaximumBadCount, 0) + " Heavy have been sent.\nNeed 4 or more each.";
			TempYesNoDialog.vNoticeText = TempText;
			TempYesNoDialog.vYesButtonText = "Return To Calibrate";
			TempYesNoDialog.vNoButtonText = "Proceed";
			TempYesNoDialog.vQuestionType = cConfirmQuestion;
			int TempResult = TempYesNoDialog.DoModal();
			if (TempResult != IDOK)
			{
				TempFinishDensityThreshold = true;
				vInspectionEditing->vSetupHistoryMaximumBadCount = 0;
				vInspectionEditing->vSetupHistoryMinimumGoodCount = 0;
			}
		}
		if (TempFinishDensityThreshold)
		{
			vInspectionEditing->vCollectingDensityAverage = false;
			vInspectionEditing->vSetupHistoryQualityFactor = vLocalSystemData->vQualityFactor;

			m_Width2.ShowWindow(SW_HIDE);
			m_Reference.ShowWindow(SW_HIDE);
			m_SubFunction7Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_SubFunction7Button.GetRectToInvalidateArrow());
			m_SubFunction8Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_SubFunction8Button.GetRectToInvalidateArrow());
			vSubFunction2ButtonEnable = true;		//m_SubFunction2Button.EnableWindow(true);
			m_SubFunction2Button.Invalidate(false);

			m_SubFunction4DisplayA.ShowWindow(SW_HIDE);
			m_SubFunction4Display.ShowWindow(SW_HIDE);
			m_SubFunction8Button1.ShowWindow(SW_HIDE);
			m_SubFunction8Button2.ShowWindow(SW_HIDE);
			m_SubFunction8Button3.ShowWindow(SW_HIDE);
			m_SubFunction7Display.ShowWindow(SW_HIDE);
			FinishThreshold();
			vChangeMade = false;
			vTookNewDensityData = false;

			vFunction2ButtonEnable = true;		//m_Function2Button.EnableWindow(true);
			vSubFunction1ButtonEnable = true;		//m_SubFunction1Button.EnableWindow(true);
			vSubFunction2ButtonEnable = true;		//m_SubFunction2Button.EnableWindow(true);
			m_SubFunction2Button.Invalidate(false);
			vSubFunction3ButtonEnable = true;		//m_SubFunction3Button.EnableWindow(true);
			vSubFunction4ButtonEnable = true;		//m_SubFunction4Button.EnableWindow(true);
			vSubFunction5ButtonEnable = true;		//m_SubFunction5Button.EnableWindow(true);
			vSubFunction7ButtonEnable = true;		//m_SubFunction7Button.EnableWindow(true);

			if ((vInspectionEditing->vInspectionType == cUnderfillByWeightInspection) || (vInspectionEditing->vInspectionType == cOverfillByWeightInspection) || (vInspectionEditing->vInspectionType == cCheckWeighInspection))
				vGlobalCurrentProduct->MatchInspectionsForWeightTrending();  //must update in case calibrated weights, or changed linking
		}
	}

	return TempFinishDensityThreshold;
}

void CAutoSetupImageDisplayDialog::StartLearnMenu()
{
	vMode = cLearnMenu;
	CheckIfShouldLockDensityFeedbackLoop();

	if ((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode))
		vLocalConfigurationData->vNumberOfQuickLearnImagesToTake = 1;

	m_SubFunction1Display.ShowWindow(SW_HIDE);
	m_SubFunction3Display.ShowWindow(SW_HIDE);
	CString TempText("");
	m_Function4Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
	vSubFunction6ButtonEnable = true;		//m_SubFunction6Button.EnableWindow(true);
	m_Function4Button.ShowWindow(SW_SHOW);
	m_SubFunction7Button.ShowWindow(SW_SHOW);

	if (vLocalConfigurationData->vRemoveBackgroundFromImage)
		TempText.LoadString(IDS_BackToBackground);
	else
		TempText.LoadString(IDS_BackToExposure);

	SetDlgItemText(IDC_Function4Button,TempText);

	//if (vLocalSystemData->vInAutoSetup)
		TempText.LoadString(IDS_NextToInspections);
	//else
	//	TempText.LoadString(IDS_ChangeInspections);
	SetDlgItemText(IDC_Function5Button,TempText);

	if (vLocalSystemData->vInAutoSetup)
		vDialogTitleString.LoadString(IDS_AutoSetupImageLearn);
	else
		vDialogTitleString.LoadString(IDS_SetupImageLearn);
	SetDlgItemText(IDC_DialogTitleStaticText1,vDialogTitleString);

	vSubFunction5ButtonEnable = true;		//m_SubFunction5Button.EnableWindow(true);
	vSubFunction7ButtonEnable = true;		//m_SubFunction7Button.EnableWindow(true);
	m_SubFunction7Display.ShowWindow(SW_SHOW);
	m_Function2Button.ShowWindow(SW_SHOW);
	m_Function2Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
	if (vLocalSystemData->vInAutoSetup)
		TempText.LoadString(IDS_FinishAutoSetup);
	else
		TempText.LoadString(IDS_FinishSetup);
	SetDlgItemText(IDC_Function2Button,TempText);

	vSubFunction1ButtonEnable = true;		//m_SubFunction1Button.EnableWindow(true);
	m_SubFunction5Button.ShowWindow(SW_SHOW);
	//m_Function5Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
	//vLocalSystemData->vFrameCount = 0;

	if (vLocalSystemData->vITIPCDig->HaveLearnImageFile("=Process 2"))
		TempText = " Product Learned: " + vGlobalCurrentProduct->vHistoryTookAverageDate;
	else
		TempText.LoadString(IDS_PressStartAutoLearnButtonWhenImagesLookGood);
	SetStatusLine(TempText);

	vChangeMade = false;
	/*//hide variable integration time
	if (vLocalSystemData->vFPGAVersion10Point9OrHigher)
	{
		if (vLocalConfigurationData->vRemoveBackgroundFromImage)
			TempText.LoadString(IDS_BackToBackground);
		else
			TempText.LoadString(IDS_BackToEExposure);
	}
	else
	*/
	{
		if (vLocalConfigurationData->vRemoveBackgroundFromImage)
			TempText.LoadString(IDS_BackToBackground);
		else
			TempText.LoadString(IDS_BackToExposure);
	}
	DisplayWithPassword();
	UpdateAverageDensityDisplay();
	UpdateButtonText();
	DrawImageOnScreen();
}

bool CAutoSetupImageDisplayDialog::FinishLearnMenu(bool TempWarn) 
{
	//m_SubFunction4Display.ShowWindow(SW_HIDE);
	m_SubFunction8Display.ShowWindow(SW_HIDE);
	bool TempFinishImageProcessing = false;
	if ((!((vOldAverageEnabled) || (vOldStructureEnabled) || (vOldTakingQuickLearnImages) || (vOldCalibrateBodyTriggerReferenceEnabled) || (vOldAdjustingBrightnessBeforeLearning))) ||
		(!vLocalSystemData->vInAutoSetup))
	{
		TempFinishImageProcessing = true;

		if (TempWarn)
		if ((!((vLocalSystemData->vITIPCDig->vAverageImage) || (vLocalSystemData->vITIPCDig->vProductAverageImage))) && (vLocalSystemData->vInAutoSetup))
		{
			CYesNoDialog TempYesNoDialog;
			CString TempText;
			CString TempText1;
			TempText.LoadString(IDS_ImageProcessingNotSetupYet);
			//TempText1.LoadString(IDS_ThisisOKifyouareonlydoingDensityandTextureInspections);//"\nThis is OK if you are only doing Density and Texture Inspections."
			//TempText = TempText + TempText1;
			TempYesNoDialog.vNoticeText = TempText;
			TempText1.LoadString(IDS_SkipLearn);
			TempYesNoDialog.vYesButtonText = TempText1;
			TempYesNoDialog.vQuestionType = cConfirmQuestion;
			int TempResult = TempYesNoDialog.DoModal();
			if (TempResult != IDOK)
				TempFinishImageProcessing = false;
		}
		else
			TempFinishImageProcessing = true;
		if (TempFinishImageProcessing)
		{
			m_Width2.ShowWindow(SW_HIDE);
			m_Reference.ShowWindow(SW_HIDE);
			m_SubFunction8Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_SubFunction8Button.GetRectToInvalidateArrow());
			m_SubFunction5Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_SubFunction5Button.GetRectToInvalidateArrow());
			m_Function2Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_Function2Button.GetRectToInvalidateArrow());
			m_SubFunction7Display.ShowWindow(SW_HIDE);
			m_SubFunction7Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_SubFunction7Button.GetRectToInvalidateArrow());
			m_SubFunction4Display.ShowWindow(SW_HIDE);
			m_SubFunction1Display.ShowWindow(SW_HIDE);
			m_SubFunction2Display.ShowWindow(SW_HIDE);
			m_SubFunction3Display.ShowWindow(SW_HIDE);
			m_SubFunction1Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_SubFunction1Button.GetRectToInvalidateArrow());
			m_SubFunction2Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_SubFunction2Button.GetRectToInvalidateArrow());
			m_SubFunction3Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_SubFunction3Button.GetRectToInvalidateArrow());
			m_SubFunction4Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_SubFunction4Button.GetRectToInvalidateArrow());
			m_SubFunction6Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_SubFunction6Button.GetRectToInvalidateArrow());
			/*
			if (vLocalSystemData->vITIPCDig->vProductFactoredStructureImage != NULL)
			{
				delete vLocalSystemData->vITIPCDig->vProductFactoredStructureImage;
				vLocalSystemData->vITIPCDig->vProductFactoredStructureImage = NULL;
			}
			*/
			vShowAverage = false;
			vShowStructure = false;
			vShowVoidStructure = false;
			//vShowGlassStructure = false;
			vShowContaminant2Structure = false;
			vShowHistogram = false;
			//vShowGlassHistogram = false;


			if (vChangeMade)
			{
				//ChangedProductSettingsSoReload();
				vChangeMade = false;
			}
		}
	}
	else
	{
		CNoticeDialog TempNoticeDialog;
		CString TempText;
		TempText.LoadString(IDS_YoucannotexitwhileLearning);//"\n\n\nYou cannot exit while Learning"
		TempNoticeDialog.vNoticeText = TempText;
		TempNoticeDialog.vType = cErrorMessage;
		TempNoticeDialog.DoModal();
	}
	if ((vAutoSetupType == cNewAutoSetup) && (vLocalSystemData->vChangedSizeOfProduct))
	{
		bool TempMakeInspectionsFullImage = false;
		if ((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode) ||
			(vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vLocalConfigurationData->vScanTracType == cAllegroScanTrac) || 
			(vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac) || (vGlobalScanTracType == cBriosoScanTrac)) //Continuous Feed, or a Pipeline or TD
			TempMakeInspectionsFullImage = true;

		vGlobalCurrentProduct->SetupAllInspectionROIsFromReference(vLocalSystemData->vITIPCDig->vOriginalImage, vLocalSystemData->vChangedSizeOfProductCheckDensityROI, TempMakeInspectionsFullImage, vLocalSystemData->vITIPCDig->vProductAverageImage);
	}
	else
		vGlobalCurrentProduct->CheckAllROIs(vLocalSystemData->vITIPCDig->vProductAverageImage);

	return TempFinishImageProcessing;
}

void CAutoSetupImageDisplayDialog::StartBackground()
{
	CString TempText;
	vMode = cSetBackground;
	vWaitingForBackgroundImage = false;
	vShowingBackground = false;
	m_Function4Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
	m_Function4Button.ShowWindow(SW_SHOW);
	//if (vLocalSystemData->vInAutoSetup)
		TempText.LoadString(IDS_BackToExposure);
	//else
	//	TempText.LoadString(IDS_ChangeExposure);
	SetDlgItemText(IDC_Function4Button,TempText);

	if (vLocalSystemData->vInAutoSetup)
		vDialogTitleString = "Auto Setup Image Background";
	else
		vDialogTitleString = "Setup Image Background";
	SetDlgItemText(IDC_DialogTitleStaticText1,vDialogTitleString);

	m_Function5Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);

	m_SubFunction5Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
	vSubFunction5ButtonEnable = true;		//m_SubFunction5Button.EnableWindow(true);
	m_SubFunction5Button.ShowWindow(SW_SHOW);
	SetDlgItemText(IDC_SubFunction5Button, _T("Sample Back- ground"));
	m_Function2Button.ShowWindow(SW_SHOW);

	SetDlgItemText(IDC_SubFunction1Button, _T("No\nBack- ground"));

	UpdateButtonText();
	DrawImageOnScreen();
}

void CAutoSetupImageDisplayDialog::StartCenterExposureMenu()
{
	if (vLocalSystemData->vInAutoSetup)
	if ((!vLocalConfigurationData->vContinuousFeedContainerRate) && (!vLocalConfigurationData->vBulkProductMode) && (!vGlobalCurrentProduct->vBulkProductMode)) //Not Continuous Feed, Not a Pipeline
		vGlobalInAutoExposure = true; 

	vReturnToSetSource = false;
	vLocalSystemData->vAdjustingSourceSettings = true;
	CString TempText("");
	vMode = cCenterExposureMenu;
	m_Function4Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
	m_Function4Button.ShowWindow(SW_SHOW);
	//if (vLocalSystemData->vInAutoSetup)
		TempText.LoadString(IDS_BackToSize);
	//else
	//	TempText.LoadString(IDS_ChangeSize);
	SetDlgItemText(IDC_Function4Button,TempText);

	if (vLocalSystemData->vInAutoSetup)
		vDialogTitleString.LoadString(IDS_AutoSetupImageIntensity);
	else
		vDialogTitleString.LoadString(IDS_SetupImageExposure);
	SetDlgItemText(IDC_DialogTitleStaticText1,vDialogTitleString);
	m_Function5Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);

	if (vLocalConfigurationData->vRemoveBackgroundFromImage)
		TempText.LoadString(IDS_NextToBackground);
	else
		TempText.LoadString(IDS_NextToLearn);

	SetDlgItemText(IDC_Function5Button,TempText);

	m_Function5Button.ShowWindow(SW_SHOW);

	m_SubFunction1Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
	m_SubFunction1Button.ShowWindow(SW_SHOW);
	vSubFunction1ButtonEnable = true;		//m_SubFunction1Button.EnableWindow(true);
	TempText.LoadString(IDS_ProductDensityROI);
	SetDlgItemText(IDC_SubFunction1Button,TempText);

	m_SubFunction3Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
	m_SubFunction3Button.ShowWindow(SW_SHOW);
	m_SubFunction3Display.ShowWindow(SW_SHOW);
	m_SizeX.ShowWindow(SW_SHOW);
	SetDlgItemText(IDC_SubFunction3Display, _T("Darkest Pixel"));
	SetDlgItemText(IDC_SizeX, _T("--"));

	m_SubFunction5Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
	m_SubFunction5Button.ShowWindow(SW_SHOW);
	m_SubFunction5Display.ShowWindow(SW_SHOW);
	TempText.LoadString(IDS_XRaySourceVoltage);
	SetDlgItemText(IDC_SubFunction5Button,TempText);

	m_SubFunction6Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
	m_SubFunction6Display.ShowWindow(SW_SHOW);
	m_SubFunction6Button.ShowWindow(SW_SHOW);
	TempText.LoadString(IDS_XRaySourceCurrent);
	SetDlgItemText(IDC_SubFunction6Button,TempText);

	m_SubFunction7Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
	m_SubFunction7Button.ShowWindow(SW_SHOW);
	m_SubFunction7Display.ShowWindow(SW_SHOW);
	TempText.LoadString(IDS_ChangeExposureTime);
	SetDlgItemText(IDC_SubFunction7Button,TempText);

	m_SubFunction8Button1.ShowWindow(SW_SHOW);
	m_SubFunction8Button2.ShowWindow(SW_SHOW);
	m_SubFunction8Button3.ShowWindow(SW_SHOW);

	if (vLocalSystemData->vInAutoSetup)
		vSubFunction7ButtonEnable = true;		//m_SubFunction7Button.EnableWindow(true);
	else
		vSubFunction7ButtonEnable = false;		//m_SubFunction7Button.EnableWindow(false);

	CString TempString = dtoa((vGlobalCurrentProduct->vXRayIntegrationTime / 2500.0),3);
	TempText.LoadString(IDS_mSec);
	SetDlgItemText(IDC_SubFunction7Display,TempString + " " +TempText);

	TempString = dtoa(vGlobalCurrentProduct->vXRaySourceVoltage * 6, 1);
	SetDlgItemText(IDC_SubFunction5Display,TempString);
	TempString = dtoa(vGlobalCurrentProduct->vXRaySourceCurrent, 2);
	SetDlgItemText(IDC_SubFunction6Display,TempString);

	TempText.LoadString(IDS_BeltSpeed);
	SetDlgItemText(IDC_SubFunction8Button2,TempText);
	SetDlgItemText(IDC_SubFunction8Button3, _T("0% of Max."));

	m_Function2Button.ShowWindow(SW_SHOW);
	m_Function2Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
	if (vLocalSystemData->vInAutoSetup)
		TempText.LoadString(IDS_FinishAutoSetup);
	else
		TempText.LoadString(IDS_FinishSetup);
	SetDlgItemText(IDC_Function2Button,TempText);

	TempText.LoadString(IDS_AdjustExposureAsNeeded);

	if ((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode) || (vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || 
		(vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vLocalConfigurationData->vScanTracType == cAllegroScanTrac) ||
		(vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac) || (vGlobalScanTracType == cBriosoScanTrac)) //Not Continuous Feed, or a Pipeline or TD
			TempText = "Adjust Voltage to get Darkest Pixel between 50 and 70";
	
	SetStatusLine(TempText);

	m_SubFunction8Display.ShowWindow(SW_HIDE);
	m_Width2.ShowWindow(SW_HIDE);
	m_Reference.ShowWindow(SW_HIDE);
	ShowMaximumConveyorSpeed();
	
	vShowImageInColor = true;
	vChangeMade = false;
	//5/25/04
	vInspectionEditing = vGlobalCurrentProduct->vDriftDensityInspection;
		
	vMaximumThreshold = 255;
	if ((vInspectionEditing) && ((vInspectionEditing->vInspectionType == cUnderfillByWeightInspection) || (vInspectionEditing->vInspectionType == cOverfillByWeightInspection) || (vInspectionEditing->vInspectionType == cCheckWeighInspection)))
		vMaximumThreshold = vMaximumUnderfillByWeightThreshold;

	vInspectionNumberEditing = 0;

		//9/9/2009
	if (!vGlobalCurrentProduct->vDriftDensityInspection)
		CheckDensityCompensationInspection();

	vGlobalCurrentProduct->CheckAllROIs(vLocalSystemData->vITIPCDig->vProductAverageImage);

	/*
	//hide variable integration time
	if (vLocalSystemData->vFPGAVersion10Point9OrHigher)
	{
		if (vGlobalCurrentProduct->vXRayIntegrationTimeAtEdge < 205)
			vGlobalCurrentProduct->vXRayIntegrationTimeAtEdge = vGlobalCurrentProduct->vXRayIntegrationTime;

		if (vLocalSystemData->vInAutoSetup)
			vDialogTitleString.LoadString(IDS_AutoSetupCenterExposure);
		else
			vDialogTitleString.LoadString(IDS_SetupCenterExposure);
		SetDlgItemText(IDC_DialogTitleStaticText1,vDialogTitleString);

		//TempText.LoadString(IDS_NextToEExposure);
		TempText.LoadString(IDS_NextToLearn);

		SetDlgItemText(IDC_Function5Button,TempText);
	}
	else
	*/
	{
		if (vLocalSystemData->vInAutoSetup)
			vDialogTitleString.LoadString(IDS_AutoSetupExposure);
		else
			vDialogTitleString.LoadString(IDS_SetupExposure);
		SetDlgItemText(IDC_DialogTitleStaticText1,vDialogTitleString);

		TempText.LoadString(IDS_NextToLearn);
		SetDlgItemText(IDC_Function5Button,TempText);
	}

	DisplayWithPassword();
	UpdateButtonText();
	DrawImageOnScreen();
}
/*
void CAutoSetupImageDisplayDialog::StartEdgeExposureMenu()
{
	vReturnToSetSource = false;
	vLocalSystemData->vAdjustingSourceSettings = true;
	CString TempText("");
	vMode = cEdgeExposureMenu;
	m_Function4Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
	m_Function4Button.ShowWindow(SW_SHOW);
	m_ArrowDown4.ShowWindow(SW_SHOW);
	TempText.LoadString(IDS_BackCExposure);
	SetDlgItemText(IDC_Function4Button,TempText);

	if (vLocalSystemData->vInAutoSetup)
		vDialogTitleString.LoadString(IDS_AutoSetupEdgeExposure);
	else
		vDialogTitleString.LoadString(IDS_SetupEdgeExposure);

	SetDlgItemText(IDC_DialogTitleStaticText1,vDialogTitleString);
	m_Function5Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);

	if (vLocalConfigurationData->vRemoveBackgroundFromImage)
		TempText.LoadString(IDS_NextToBackground);
	else
		TempText.LoadString(IDS_NextToLearn);

	SetDlgItemText(IDC_Function5Button,TempText);
	CWnd *TempWindow = CWnd::GetDlgItem(IDC_ArrowDown5);
	TempWindow->Invalidate(false);

	m_SubFunction3Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
	m_SubFunction3Button.ShowWindow(SW_SHOW);
	m_ArrowLeft3.ShowWindow(SW_SHOW);

	m_Function5Button.ShowWindow(SW_SHOW);
	m_ArrowDown5.ShowWindow(SW_SHOW);

	m_SubFunction5Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
	m_SubFunction6Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
	m_SubFunction7Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
	m_SubFunction8Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);

	m_SubFunction5Button.ShowWindow(SW_SHOW);
	m_ArrowRight1.ShowWindow(SW_SHOW);
	m_SubFunction6Button.ShowWindow(SW_SHOW);
	m_ArrowRight2.ShowWindow(SW_SHOW);
	m_ArrowRight3.ShowWindow(SW_SHOW);
	m_SubFunction7Button.ShowWindow(SW_SHOW);
	m_ArrowRight4.ShowWindow(SW_SHOW);
	m_SubFunction8Button.ShowWindow(SW_SHOW);

	m_ArrowLeft1.ShowWindow(SW_SHOW);
	m_SubFunction1Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
	m_SubFunction1Button.ShowWindow(SW_SHOW);
	TempText.LoadString(IDS_UseOneIntegrationTime);
	SetDlgItemText(IDC_SubFunction1Button,TempText);

	if (vLocalSystemData->vInAutoSetup)
	{
		vSubFunction1ButtonEnable = true;		//m_SubFunction1Button.EnableWindow(true);
		vSubFunction5ButtonEnable = true;		//m_SubFunction5Button.EnableWindow(true);
		vSubFunction6ButtonEnable = true;		//m_SubFunction6Button.EnableWindow(true);
		vSubFunction7ButtonEnable = true;		//m_SubFunction7Button.EnableWindow(true);
		vSubFunction8ButtonEnable = true;		//m_SubFunction8Button.EnableWindow(true);
	}
	else
	{
		vSubFunction1ButtonEnable = false;		//m_SubFunction1Button.EnableWindow(false);
		vSubFunction5ButtonEnable = false;		//m_SubFunction5Button.EnableWindow(false);
		vSubFunction6ButtonEnable = false;		//m_SubFunction6Button.EnableWindow(false);
		vSubFunction7ButtonEnable = false;		//m_SubFunction7Button.EnableWindow(false);
		vSubFunction8ButtonEnable = false;		//m_SubFunction8Button.EnableWindow(false);
	}

	m_SubFunction5Display.ShowWindow(SW_HIDE);
	m_SubFunction6Display.ShowWindow(SW_SHOW);
	m_SubFunction7Display.ShowWindow(SW_HIDE);
	//TempText.LoadString(IDS_MakeEdgesBrighter);
	TempText = "";
	SetDlgItemText(IDC_SubFunction5Button,TempText);
	//TempText.LoadString(IDS_MakeALittleBrighter);
	SetDlgItemText(IDC_SubFunction6Button,TempText);
	//TempText.LoadString(IDS_MakeALittleDarker);
	SetDlgItemText(IDC_SubFunction7Button,TempText);
	//TempText.LoadString(IDS_MakeEdgesDarker);
	SetDlgItemText(IDC_SubFunction8Button,TempText);

	DisplayEdgeExposureTimePercent();

	m_Function2Button.ShowWindow(SW_SHOW);
	m_ArrowDown2.ShowWindow(SW_SHOW);
	m_Function2Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
	if (vLocalSystemData->vInAutoSetup)
		TempText.LoadString(IDS_FinishAutoSetup);
	else
		TempText.LoadString(IDS_FinishSetup);
	SetDlgItemText(IDC_Function2Button,TempText);

	TempText.LoadString(IDS_AdjustEdgeExposureToMatchCenter);
	SetStatusLine(TempText);

	m_SubFunction8Display.ShowWindow(SW_HIDE);
	m_Width2.ShowWindow(SW_HIDE);
	m_Reference.ShowWindow(SW_HIDE);
	
	vShowImageInColor = true;
	vChangeMade = false;
	vInspectionEditing = vGlobalCurrentProduct->vDriftDensityInspection;
	vInspectionNumberEditing = 0;
	DisplayWithPassword();
	UpdateButtonText();
	DrawImageOnScreen();
}
*/
void CAutoSetupImageDisplayDialog::FinishBackground()
{
	vWaitingForBackgroundImage = false;
	vShowingBackground = false;
	m_SubFunction5Button.ShowWindow(SW_HIDE);
	this->InvalidateRect(&m_SubFunction5Button.GetRectToInvalidateArrow());
	m_SubFunction1Button.ShowWindow(SW_HIDE);
	this->InvalidateRect(&m_SubFunction1Button.GetRectToInvalidateArrow());
	m_SubFunction8Button.ShowWindow(SW_HIDE);
	this->InvalidateRect(&m_SubFunction8Button.GetRectToInvalidateArrow());
}

bool CAutoSetupImageDisplayDialog::FinishCenterExposureMenu(bool TempWarn)
{
	bool TempOKToExit = true;
	float TempContainerWidth = 0;
	if (vLocalSystemData->vITIPCDig->vOriginalImage)
		TempContainerWidth = (float)((
			(vLocalSystemData->vITIPCDig->vOriginalImage->vRight - 
			vLocalSystemData->vITIPCDig->vOriginalImage->vLeft) 
			/ (vGlobalPixelsPerUnit * vGlobalCurrentProduct->vOverScanMultiplier)));

	if (TempWarn)
	if (vHaveAnImage)
	if (TempContainerWidth)
	if (!((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode) || (vLocalConfigurationData->vScanTracType == cBriosoScanTrac) || 
		(vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vLocalConfigurationData->vScanTracType == cAllegroScanTrac) ||
		(vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac) || (vGlobalScanTracType == cBriosoScanTrac))) //Not Continuous Feed, or a Pipeline or TD
	if ((vLocalSystemData->vITIPCDig->vOriginalImage->vLeft < 3) ||
		(vLocalSystemData->vITIPCDig->vOriginalImage->vRight > vLocalSystemData->vITIPCDig->vAquisitionSizeY - 4))
	{
		vHaveNotSetSize = true;
		CYesNoDialog TempYesNoDialog;
		CString TempText;
		TempText.LoadString(IDS_ScanTracdidnotfindtheedgesofthecontainercorrectly2);
		TempText = TempText;
		TempYesNoDialog.vNoticeText = TempText;
		CString TempText1;
		TempText1.LoadString(IDS_GoBackToAutosize);
		TempYesNoDialog.vYesButtonText = TempText1;
		CString TempText2;
		TempText2.LoadString(IDS_ContinueAsSet);
		TempYesNoDialog.vNoButtonText = TempText2;
		TempYesNoDialog.vQuestionType = cConfirmQuestion;
		int TempResult = TempYesNoDialog.DoModal();
		if (TempResult == IDOK)
			TempOKToExit = false;
	}

	m_SubFunction1Button.ShowWindow(SW_HIDE);
	this->InvalidateRect(&m_SubFunction1Button.GetRectToInvalidateArrow());
	m_SubFunction2Button.ShowWindow(SW_HIDE);
	this->InvalidateRect(&m_SubFunction2Button.GetRectToInvalidateArrow());

	m_SubFunction3Display.ShowWindow(SW_HIDE);
	m_SizeX.ShowWindow(SW_HIDE);

	m_SubFunction4Button.ShowWindow(SW_HIDE);
	this->InvalidateRect(&m_SubFunction4Button.GetRectToInvalidateArrow());
	m_SubFunction4DisplayA.ShowWindow(SW_HIDE);

	m_SubFunction5Button.ShowWindow(SW_HIDE);
	this->InvalidateRect(&m_SubFunction5Button.GetRectToInvalidateArrow());
	m_SubFunction6Button.ShowWindow(SW_HIDE);
	this->InvalidateRect(&m_SubFunction6Button.GetRectToInvalidateArrow());

	m_SubFunction7Button.ShowWindow(SW_HIDE);
	this->InvalidateRect(&m_SubFunction7Button.GetRectToInvalidateArrow());

	m_SubFunction3Button.ShowWindow(SW_HIDE);
	this->InvalidateRect(&m_SubFunction3Button.GetRectToInvalidateArrow());

	m_SubFunction7Display.ShowWindow(SW_HIDE);
	m_SubFunction5Display.ShowWindow(SW_HIDE);
	m_SubFunction6Display.ShowWindow(SW_HIDE);

	m_SubFunction8Button1.ShowWindow(SW_HIDE);
	m_SubFunction8Button2.ShowWindow(SW_HIDE);
	m_SubFunction8Button3.ShowWindow(SW_HIDE);

	m_SubFunction8Display.ShowWindow(SW_HIDE);
	m_Width2.ShowWindow(SW_HIDE);
	m_Reference.ShowWindow(SW_HIDE);
	vLocalSystemData->vAdjustingSourceSettings = false;
	vGlobalInAutoExposure = false; 
	return TempOKToExit;
}

bool CAutoSetupImageDisplayDialog::FinishContaminantThreshold(bool TempWarn) 
{
	WriteCSVFiles();
	m_Reference.ShowWindow(SW_HIDE);
	vChangeMade = false;
	FinishThreshold();
	return true;
}


void CAutoSetupImageDisplayDialog::OnFunction5Button() 
{
	if (vInitialized)
	if (vMode == CMultiLanePositionAdjustment)
	{
		//set default lane spacing
		if (vGlobalCurrentProduct->vMultiLaneNumberOfLanes)
		{
			double TempImageHeight = vLocalConfigurationData->vNumberOfXRayDetectors * vLocalConfigurationData->vDetectorLength;
			double TempLaneWidthSuggestion = TempImageHeight / vGlobalCurrentProduct->vMultiLaneNumberOfLanes;
			WORD TempLaneWidth = (WORD)(TempLaneWidthSuggestion * vGlobalPixelsPerUnitInHeight);
			if (TempLaneWidth > vLocalSystemData->vITIPCDig->vOriginalBufferSizeX)
				TempLaneWidth = vLocalSystemData->vITIPCDig->vOriginalBufferSizeX;

			for (BYTE TempLaneLoop = 0; TempLaneLoop < vGlobalCurrentProduct->vMultiLaneNumberOfLanes; TempLaneLoop++)
			{
				vGlobalCurrentProduct->vMultiLaneWidthInPixels[TempLaneLoop] = TempLaneWidth;
				vGlobalCurrentProduct->vMultiLaneStartPixel[TempLaneLoop] = TempLaneWidth * TempLaneLoop;
				//if lane extends beyond actual detectors, correct positions so they are valid
				if (vGlobalCurrentProduct->vMultiLaneWidthInPixels[TempLaneLoop] + vGlobalCurrentProduct->vMultiLaneStartPixel[TempLaneLoop] > vLocalSystemData->vITIPCDig->vAquisitionSizeXUpTo12Detectors)
				{
					if ((TempLaneLoop < vGlobalCurrentProduct->vMultiLaneNumberOfLanes - 1) || 
						(vGlobalCurrentProduct->vMultiLaneWidthInPixels[TempLaneLoop] + vGlobalCurrentProduct->vMultiLaneStartPixel[TempLaneLoop] > vLocalSystemData->vITIPCDig->vAquisitionSizeXUpTo12Detectors + 15))
					{ //give warning unless last lane and less than 15 pixels past image width
							ReportErrorMessage("\nMulti Lane Setup Error.\n\nLanes are wider than detectors.", cError, 0);
					}
					if (vGlobalCurrentProduct->vMultiLaneWidthInPixels[TempLaneLoop] > vLocalSystemData->vITIPCDig->vAquisitionSizeX)
						vGlobalCurrentProduct->vMultiLaneStartPixel[TempLaneLoop] = vLocalSystemData->vITIPCDig->vAquisitionSizeX - vGlobalCurrentProduct->vMultiLaneWidthInPixels[TempLaneLoop];
				}
			}
		}
		UpdateButtonText();
		WaitThenDrawImageOnScreen();
	}
	else
	if (vMode == cSetSizeSettings)
	{
		if ((vLocalSystemData->vITIPCDig->vDoBottomReferencing) && (!vGlobalCurrentProduct->vMultiLaneNumberOfLanes))
		{
			vAdjustingBottomEdgeFinding = !vAdjustingBottomEdgeFinding;

			m_SubFunction6Display.ShowWindow(SW_SHOW);
			if (vAdjustingBottomEdgeFinding)
				SetDlgItemText(IDC_SubFunction6Display, _T("Adjust Bottom"));
			else
				SetDlgItemText(IDC_SubFunction6Display, _T("Adjust Side"));
		}
		else
		{
			vShowImageInColor = !vShowImageInColor;
			DrawImageOnScreen();
		}
		UpdateButtonText();
	}
	else
	if (vMode == cSetImageSize)
	{
		if (FinishSizeMenu(true, false))
			StartCenterExposureMenu();
	}
	else
	if (vMode == cCenterExposureMenu)
	{
		if (FinishCenterExposureMenu(false))  //was true for go back if not finding edges
		{
			//hide variable integration time
			//if (vLocalSystemData->vFPGAVersion10Point9OrHigher)
			//	StartEdgeExposureMenu();
			//else
			if (vLocalConfigurationData->vRemoveBackgroundFromImage)
				StartBackground();
			else
				StartLearnMenu();
		}
		else
			StartSizeMenu();
	}
	else
		/*
	if (vMode == cEdgeExposureMenu)
	{
		if (FinishEdgeExposureMenu(false))  //was true for go back if not finding edges
		{
			if (vLocalConfigurationData->vRemoveBackgroundFromImage)
				StartBackground();
			else
				StartLearnMenu();
		}
		else
			StartSizeMenu();
	}
	else
	*/
	if (vMode == cSetBackground)
	{
		FinishBackground();
		StartLearnMenu();
	}
	else
	if (vMode == cLearnMenu)
	{
		if ((!((vOldAverageEnabled) || (vOldStructureEnabled) || (vOldTakingQuickLearnImages) ||
			(vOldCalibrateBodyTriggerReferenceEnabled) || (vOldAdjustingBrightnessBeforeLearning))) ||
			(!vLocalSystemData->vInAutoSetup))
		{
			if (FinishLearnMenu(true))
			{
				if (vLocalConfigurationData->vDriftCompensationEnabled)
					vLocalSystemData->vKeepDensityFeedbackLoopLocked = true;
				ConfigureInspections(false, false);
			}
		}
		else
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\n\nYou cannot exit while learning";
			TempNoticeDialog.vType = cErrorMessage;
			TempNoticeDialog.DoModal();
		}
	}
	else
	if (vMode == cSetROI)
	{  //if void enabled go to void ROI, else to Contaminant Threshold
		if (!vReturnToSetSource)
		if (vInspectionEditing)
		{
			FinishROI(false);

			if (!vReturnToSetSource)
			if (vInspectionEditing->vInspectionType == cKernelInspection)
				StartContaminantThreshold();
			else
			if ((vInspectionEditing->vInspectionType == cContaminantInspection) || 
				(vInspectionEditing->vInspectionType == cWireContaminantInspection) ||
				(vInspectionEditing->vInspectionType == cSpotInROIContaminantInspection) ||
				(vInspectionEditing->vInspectionType == cGlass13mmContaminantInspection) ||
				(vInspectionEditing->vInspectionType == cThresholdContaminantInspection) ||
				(vInspectionEditing->vInspectionType == cBadHeartInspection) ||
				(vInspectionEditing->vInspectionType == cDripBySpoutInspection) ||
				(vInspectionEditing->vInspectionType == cDripInspection) ||
				(vInspectionEditing->vInspectionType == cFloodedInspection) ||
				(vInspectionEditing->vInspectionType == cTopContaminantInspection) ||
				(vInspectionEditing->vInspectionType == cBottomContaminantInspection) ||
				(vInspectionEditing->vInspectionType == cIPContaminantInspection) ||
				(vInspectionEditing->vInspectionType == cJarBottomContaminantInspection) ||
				(vInspectionEditing->vInspectionType == cContaminant2Inspection) ||
				(vInspectionEditing->vInspectionType == cContaminantBonesInspection) ||
				(vInspectionEditing->vInspectionType == cContaminant22Inspection) ||
				(vInspectionEditing->vInspectionType == cAdvancedContaminant2Inspection))
				StartContaminantThreshold();
			else
			if (vInspectionEditing->vInspectionType == cVoidInspection)
				StartVoidThreshold();
			else
			if ((vInspectionEditing->vInspectionType == cDensityInspection) ||
				(vInspectionEditing->vInspectionType == cCheckWeighInspection) ||
				(vInspectionEditing->vInspectionType == cMissingLidInspection) ||
				(vInspectionEditing->vInspectionType == cUnderfillInspection) ||
				(vInspectionEditing->vInspectionType == cUnderfillByWeightInspection) || 
				(vInspectionEditing->vInspectionType == cOverfillByWeightInspection) ||
				(vInspectionEditing->vInspectionType == cOverfillInspection))
				StartDensityThreshold();
			else
			if (vInspectionEditing->vInspectionType == cStandardDeviationInspection)
				StartDensityThreshold();
			else
			if ((vInspectionEditing) && ((vInspectionEditing->vInspectionType == cItemCountInspection) || (vInspectionEditing->vInspectionType == cSizeInspection) || (vInspectionEditing->vInspectionType == cMissingLidInspectionOnSize)))
				StartContaminantThreshold();
		}
	}
	else
	if (vMode == cSetContaminantThreshold)
	{
		if (FinishContaminantThreshold(true))
		{
			if ((vAutoSetupType == cEvaluateAutoSetup) && 
				(vGlobalCurrentProduct->vNumberOfInspections == 1))
			{
				if (KeepSetupChanges())
					ExitAutoSetupDialog(IDOK);
				else
					StartContaminantThreshold();
			}
			else
				ConfigureInspections(false, false);
		}
	}
	else
	if (vMode == cSetVoidThreshold)
	{
		if (FinishVoidThreshold(true))
		{
			if ((vAutoSetupType == cEvaluateAutoSetup) && 
				(vGlobalCurrentProduct->vNumberOfInspections == 1))
			{
				if (KeepSetupChanges())
					ExitAutoSetupDialog(IDOK);
				else
					StartVoidThreshold();
			}
			else
				ConfigureInspections(false, false);
		}
	}
	else  //OnFunction5Button
	if (vMode == cSetDensityThreshold)
	{
		if (vEditOnlyThisInspection)
		{
			if ((!vAnyChangeMade) || (KeepSetupChanges()))
			{
				ExitAutoSetupDialog(20);
			}
		}
		else
		if (FinishDensityThreshold(true))
		{
			if ((vAutoSetupType == cEvaluateAutoSetup) && 
				(vGlobalCurrentProduct->vNumberOfInspections == 1))
			{
				if (KeepSetupChanges())
					ExitAutoSetupDialog(IDOK);
				else
					StartDensityThreshold();
			}
			else
				ConfigureInspections(false, false);
		}
	}
}

void CAutoSetupImageDisplayDialog::ConfigureInspections(bool TempSelectForEvaluate, bool TempEditROI)
{
	bool TempDone = false;
	CConfigureInspectionsDialog TempConfigureInspectionsDialog;
	TempConfigureInspectionsDialog.vProductEditing = vGlobalCurrentProduct;
	TempConfigureInspectionsDialog.vLocalProductCollection = vLocalProductCollection;
	TempConfigureInspectionsDialog.vMainWindowPointer = vMainWindowPointer;
	TempConfigureInspectionsDialog.vLocalConfigurationData = vLocalConfigurationData;
	TempConfigureInspectionsDialog.vLocalSystemData = vLocalSystemData;
	TempConfigureInspectionsDialog.vShowReturnToEditMenu = vShowReturnToEditMenu;

	if ((vAutoSetupType == cEvaluateAutoSetup) || (vAutoSetupType == cEditIPAutoSetup))
		TempConfigureInspectionsDialog.ShowFinishAutoSetupButton = true;

	TempConfigureInspectionsDialog.vPickOneToEvaluate = vPickOneToEvaluate;
	vPickOneToEvaluate = false; //only do this once.

	while (!TempDone)
	{
		BYTE TempNumberOfInspections = vGlobalCurrentProduct->vNumberOfInspections;
		int TempResult = TempConfigureInspectionsDialog.DoModal();
		vInspectionEditing = NULL;
		vInspectionNumberEditing = 0;
		if (TempResult == IDOK)  //indicates selected an inspection to edit or evaluate
		{
			if (TempNumberOfInspections != vGlobalCurrentProduct->vNumberOfInspections)
				SetChangeMade();
			if (TempConfigureInspectionsDialog.vInspectionToEdit)
			{
				WriteCSVFiles();
				vInspectionEditing = TempConfigureInspectionsDialog.vInspectionToEdit;
				vMaximumThreshold = 255;
				if ((vInspectionEditing) && ((vInspectionEditing->vInspectionType == cUnderfillByWeightInspection) || (vInspectionEditing->vInspectionType == cOverfillByWeightInspection) || (vInspectionEditing->vInspectionType == cCheckWeighInspection)))
					vMaximumThreshold = vMaximumUnderfillByWeightThreshold;

				for (BYTE TempLoop = 0; TempLoop < cMaximumNumberOfInspections; TempLoop++)
				if (vGlobalCurrentProduct->vInspection[TempLoop] == vInspectionEditing)
				{
					vInspectionNumberEditing = TempLoop;
					TempLoop = cMaximumNumberOfInspections;
				}
				if (TempConfigureInspectionsDialog.vSelectedEvaluate)
				{
					if (vInspectionEditing)
					{
						if (TempEditROI)
							StartROI();
						else
						if (vInspectionEditing->vInspectionType == cKernelInspection)
							StartContaminantThreshold();
						else
						if ((vInspectionEditing->vInspectionType == cContaminantInspection) ||
							(vInspectionEditing->vInspectionType == cWireContaminantInspection) ||
							(vInspectionEditing->vInspectionType == cSpotInROIContaminantInspection) ||
							(vInspectionEditing->vInspectionType == cGlass13mmContaminantInspection) ||
							(vInspectionEditing->vInspectionType == cThresholdContaminantInspection) ||
							(vInspectionEditing->vInspectionType == cBadHeartInspection) ||
							(vInspectionEditing->vInspectionType == cDripBySpoutInspection) ||
							(vInspectionEditing->vInspectionType == cDripInspection) ||
							(vInspectionEditing->vInspectionType == cFloodedInspection) ||
							(vInspectionEditing->vInspectionType == cTopContaminantInspection) ||
							(vInspectionEditing->vInspectionType == cBottomContaminantInspection) ||
							(vInspectionEditing->vInspectionType == cIPContaminantInspection) ||
							(vInspectionEditing->vInspectionType == cJarBottomContaminantInspection) ||
							(vInspectionEditing->vInspectionType == cContaminant2Inspection) ||
							(vInspectionEditing->vInspectionType == cContaminantBonesInspection) ||
							(vInspectionEditing->vInspectionType == cContaminant22Inspection) ||
							(vInspectionEditing->vInspectionType == cAdvancedContaminant2Inspection))
							StartContaminantThreshold();
						else
						if (vInspectionEditing->vInspectionType == cVoidInspection)
							StartVoidThreshold();
						else
						if ((vInspectionEditing->vInspectionType == cDensityInspection) ||
							(vInspectionEditing->vInspectionType == cCheckWeighInspection) ||
							(vInspectionEditing->vInspectionType == cMissingLidInspection) ||
							(vInspectionEditing->vInspectionType == cUnderfillByWeightInspection) || 
							(vInspectionEditing->vInspectionType == cOverfillByWeightInspection) ||
							(vInspectionEditing->vInspectionType == cUnderfillInspection) ||
							(vInspectionEditing->vInspectionType == cOverfillInspection))
							StartDensityThreshold();
						else
						if (vInspectionEditing->vInspectionType == cStandardDeviationInspection)
							StartDensityThreshold();
						else
						if ((vInspectionEditing) && ((vInspectionEditing->vInspectionType == cItemCountInspection) || (vInspectionEditing->vInspectionType == cSizeInspection) || (vInspectionEditing->vInspectionType == cMissingLidInspectionOnSize)))
							StartContaminantThreshold();
					}
				}
				else
					StartROI();
				TempDone = true;
			}
			else
			{
				TempDone = true;
				ExitAutoSetupDialog(true);
			}
		}
		else
		if (TempResult == 5)
		{
			StartSizeMenu();
			TempDone = true;
		}
		else
		if (TempResult == 20)
		{
			TempDone = true;
			ExitAutoSetupDialog(20);
		}
		else
		if (TempResult == IDCANCEL)
		{
			StartLearnMenu();
			TempDone = true;
		}
		else 
		if (TempResult == 10)
		{
			//Main Menu button pressed
			TempDone = true;
			ExitAutoSetupDialog(10);
			CDialog::EndDialog(10);
		}
	}
	if (vLocalSystemData->vXRayOnNotice)
	{
		CString TempText("");
		TempText.LoadString(IDS_XRAYON);
		SetDlgItemText(IDC_Calibrating,TempText);
		m_Calibrating.ShowWindow(SW_SHOW);
		m_PleaseWait.ShowWindow(SW_HIDE);
		KillTimer(vClearStartingIndicatorIfXRaysOnTimerHandle);
		vBlink = 255;
	}
	else
		m_PleaseWait.ShowWindow(SW_SHOW);
}

void CAutoSetupImageDisplayDialog::WaitThenDrawImageOnScreen() 
{
	if (vWaitingToUpdateScreen)
		KillTimer(vWaitToUpdateScreenTimerHandle);

	vWaitingToUpdateScreen = true;
	int TimerResult = SetTimer(vWaitToUpdateScreenTimerHandle,300,NULL);
	if (!TimerResult)
		ReportErrorMessage("Error-Wait to update screen Timer Failed",cEMailInspx,32000);
}

void CAutoSetupImageDisplayDialog::OnSubFunction1Button()
{
	if (vInitialized)
	if (vMode == CMultiLanePositionAdjustment)
	{
		if (vGlobalCurrentProduct->vMultiLaneNumberOfLanes > vMultiImageLaneEditing + 1)
		{
			vMultiImageLaneEditing++;
			UpdateButtonText();
			WaitThenDrawImageOnScreen();
		}
		else
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\n\nEditing last lane positions";
			TempNoticeDialog.vType = cErrorMessage;
			TempNoticeDialog.DoModal();
		}
	}
	else
	if (vMode == cSetSizeSettings)
	{
		if (!vRecievedButtonPress)	//do not start repeat timer if function called from keyboard press not mouse press
		{
			//ReportErrorMessage("Left Mouse Button Down IN SUBFUNCTION 1",cAction, 32000);
			if ((vMouseState == 0) || ((vMouseState > 8) && (vMouseState < 17)))	//if mouse is idle or in double click toggle on mode
			{
				vMouseState = 1;	//set mouse to on while pressed down
				KillTimer(vMouseDownRepeatTimerHandle);
				int TimerResult = SetTimer(vMouseDownRepeatTimerHandle,200,NULL);
				if (!TimerResult)
					ReportErrorMessage("Error-Second Timer Failed",cEMailInspx,32000);
			}
		}
		
		if (PasswordOK(cTemporaryInspxPassword,false))
		{
			if (vGlobalCurrentProduct->vThresholdForContainerBounds < 245)
				vGlobalCurrentProduct->vThresholdForContainerBounds = 
					vGlobalCurrentProduct->vThresholdForContainerBounds + 10;
			else
				vGlobalCurrentProduct->vThresholdForContainerBounds = 255;
			SetChangeMade();
			SetDlgItemText(IDC_SubFunction4DisplayA, dtoa(vGlobalCurrentProduct->vThresholdForContainerBounds,0));
			WaitThenDrawImageOnScreen();
		}
	}
	else
	if (vMode == cSetBackground)
	{
		if (vLocalSystemData->vITIPCDig->vProductBackgroundImage)
		{
			CYesNoDialog TempYesNoDialog;
			TempYesNoDialog.vNoticeText =  _T("\n\nAre you sure you want to\nremove the Sampled Background?");
			TempYesNoDialog.vYesButtonText = "Remove Background";
			TempYesNoDialog.vNoButtonText = "";
			TempYesNoDialog.vQuestionType = cConfirmQuestion;
			int TempResult = TempYesNoDialog.DoModal();
			if (TempResult == IDOK)
			{
				vLocalSystemData->vITIPCDig->RenameBackgroundFileToBackup(vGlobalCurrentProduct);
				if (vLocalSystemData->vITIPCDig->vProductBackgroundImage != NULL)
				{
					im_delete(vLocalSystemData->vITIPCDig->vProductBackgroundImage);
					vLocalSystemData->vITIPCDig->vProductBackgroundImage = NULL;
				}
				vShowingBackground = false;
				m_SubFunction5Button.Invalidate(false);
				UpdateButtonText();
				WaitThenDrawImageOnScreen();
			}
		}
	}
	else
		/*
	if (vMode == cEdgeExposureMenu)
	{		//Make Edges as bright as center
		if (vLocalSystemData->vInAutoSetup)
		{
			if (vGlobalCurrentProduct->vXRayIntegrationTimeAtEdge != vGlobalCurrentProduct->vXRayIntegrationTime)
			{
				vGlobalCurrentProduct->vXRayIntegrationTimeAtEdge = vGlobalCurrentProduct->vXRayIntegrationTime;

				DisplayEdgeExposureTimePercent();

				//variable Integration Time temp can't do as will mess something up in the FPGA or uC, so reload all settings
				vMainWindowPointer->SendProductIntegrationTime(vGlobalCurrentProduct->vXRayIntegrationTime, 
					vGlobalCurrentProduct->vXRayIntegrationTimeAtEdge);

				//vMainWindowPointer->StopRunning(true);

				//int TimerResult = SetTimer(vStartSetupTimerHandle,1000,NULL);
				//if (!TimerResult)
				//	ReportErrorMessage("Error-Start Timer Failed",cEMailInspx,32000);
			}
			else
				GiveMessageAlreadyAtThisValue(2); //2 = maximum value
		}
		else
			YouCantChangeSettingBecauseInspecting();
	}
	else
	*/
	if (vMode == cCenterExposureMenu)
	{		//Edit drift density ROI
		if ((PasswordOK(cAdvancedSetupPassword,false)) || ((!vLocalConfigurationData->vContinuousFeedContainerRate) && (!vLocalConfigurationData->vBulkProductMode) && (!vGlobalCurrentProduct->vBulkProductMode))) //Not Continuous Feed, Not a Pipeline or password in
		{
			FinishCenterExposureMenu(false);
			CheckDensityCompensationInspection();
			vInspectionEditing = vGlobalCurrentProduct->vDriftDensityInspection;
			vMaximumThreshold = 255;
			if ((vInspectionEditing) && ((vInspectionEditing->vInspectionType == cUnderfillByWeightInspection) || (vInspectionEditing->vInspectionType == cOverfillByWeightInspection) || (vInspectionEditing->vInspectionType == cCheckWeighInspection)))
				vMaximumThreshold = vMaximumUnderfillByWeightThreshold;

			vInspectionNumberEditing = 0;

			vGlobalCurrentProduct->CheckAllROIs(vLocalSystemData->vITIPCDig->vProductAverageImage);

			vReturnToSetSource = true;
			//edit product density ROI
			StartROI();
		}
	}
	else
	if (vMode == cSetImageSize) //sF1
	{
		//ReportErrorMessage("Left Button Down IN SUBFUNCTION 1",cEMailInspx, 32000);
		//if ((vMouseState == 0) || ((vMouseState > 8) && (vMouseState < 17)))	//if mouse is idle or in double click toggle on mode
		//{
		//	vMouseState = 1;	//set mouse to on while pressed down
		//	KillTimer(vMouseDownRepeatTimerHandle);
		//	int TimerResult = SetTimer(vMouseDownRepeatTimerHandle,200,NULL);
		//	if (!TimerResult)
		//		ReportErrorMessage("Error-Second Timer Failed",cEMailInspx,32000);
		//}
		
		//if (!((PasswordOK(cAdvancedSetupPassword,false)) || ((!vLocalConfigurationData->vContinuousFeedContainerRate) && (!vLocalConfigurationData->vBulkProductMode) && (!vGlobalCurrentProduct->vBulkProductMode)))) //Not Continuous Feed, Not a Pipeline or password in
		//{
		//	if (PasswordOK(cAdvancedSetupPassword, true))
		//		DisplayWithPassword();
		//}
		//else
		if ((PasswordOK(cTemporaryInspxPassword,false)) || ((!vLocalConfigurationData->vContinuousFeedContainerRate) && (!vLocalConfigurationData->vBulkProductMode))) //Not Continuous Feed, Not a Pipeline or password in
		if (((!vLocalSystemData->vXRayOnNotice) || (vStartingUpSource) || (vRestartingXRaysWithNewSettings) ||
			((vLocalSystemData->vSystemRunMode != cRunningSystemMode) && (vLocalSystemData->vSystemRunMode != cAutoSetupRunningSystem))) &&
			(vMainWindowPointer->InterlocksOK()) && (vStartedSource))
			StartingSystemPleaseTryAgain();
		else
		//if (NoSetupOrOKToMinorChange())
		{
			FinishSizeMenu(false, false);
			StartSetSizeSettings();
		}
	}
	else
	if (vMode == cSetContaminantThreshold)
	{		//+10 to Contaminant Threshold 
		if (vInspectionEditing)
		{
			if (vInspectionEditing->vThreshold < 245)
			{
				vInspectionEditing->vThreshold = vInspectionEditing->vThreshold + 10;
				//ReportErrorMessage("Left Mouse Button Down IN SUBFUNCTION 1",cAction, 32000);
				if (!vRecievedButtonPress)
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
			}
			else
			{
				vInspectionEditing->vThreshold = 255;
				if (vMouseState > 0)
				{
					KillTimer(vMouseDownRepeatTimerHandle);
					vMouseState = 0;
				}
			}

			if ((vInspectionEditing) && ((vInspectionEditing->vInspectionType == cItemCountInspection) || (vInspectionEditing->vInspectionType == cSizeInspection) || (vInspectionEditing->vInspectionType == cMissingLidInspectionOnSize)))
				ClearEvaluate();
		
			vLocalSystemData->vManuallyEditedDuringEvaluate = true;
			if ((vInspectionEditing->vInspectionType != cItemCountInspection) && (vInspectionEditing->vInspectionType != cSizeInspection) && (vInspectionEditing->vInspectionType != cMissingLidInspectionOnSize))
				SetChangeMade();
			SetDlgItemText(IDC_ThresholdForContainerBounds2,dtoa(vInspectionEditing->vThreshold,0));
			WaitThenDrawImageOnScreen();
			ShowSensitivity();
		}
	}
	else
	if (vMode == cSetVoidThreshold)
	{		//+10 to Void Threshold 
		if (!vRecievedButtonPress)
		{
			//ReportErrorMessage("Left Mouse Button Down IN SUBFUNCTION 1",cAction, 32000);
			if ((vMouseState == 0) || ((vMouseState > 8) && (vMouseState < 17)))	//if mouse is idle or in double click toggle on mode
			{
				vMouseState = 1;	//set mouse to on while pressed down
				KillTimer(vMouseDownRepeatTimerHandle);
				int TimerResult = SetTimer(vMouseDownRepeatTimerHandle,200,NULL);
				if (!TimerResult)
					ReportErrorMessage("Error-Second Timer Failed",cEMailInspx,32000);
			}
		}
		
		if (vInspectionEditing)
		{
			if (vInspectionEditing->vThreshold < 245)
				vInspectionEditing->vThreshold = vInspectionEditing->vThreshold + 10;
			else
				vInspectionEditing->vThreshold = 255;
		
			if ((vInspectionEditing) && ((vInspectionEditing->vInspectionType == cItemCountInspection) || (vInspectionEditing->vInspectionType == cSizeInspection) || (vInspectionEditing->vInspectionType == cMissingLidInspectionOnSize)))
				ClearEvaluate();
		
			vLocalSystemData->vManuallyEditedDuringEvaluate = true;
			SetChangeMade();
			SetDlgItemText(IDC_ThresholdForContainerBounds2,dtoa(vInspectionEditing->vThreshold, 0));
			WaitThenDrawImageOnScreen();
		}
	}
	else
	//Average disable count button pressed
	if (vMode == cLearnMenu)
	{
		if (!((PasswordOK(cTemporaryInspxPassword,false)) || (PasswordOK(cAdvancedSetupPassword,false)) || (vLocalConfigurationData->vAutoImproveEnabled)))
		{
			if (PasswordOK(cAdvancedSetupPassword, true))
				DisplayWithPassword();
		}
		else
		if (PasswordOK(cTemporaryInspxPassword,false))
		{
			CNumericEntryDialog INumericEntryDialog;  
			
			//Set dialog box data titles and number value
			INumericEntryDialog.vEditString = dtoa(vLocalConfigurationData->vAverageDisableCount, 0);

			INumericEntryDialog.m_DialogTitleStaticText1 = "";
			INumericEntryDialog.m_DialogTitleStaticText2 = "Number of Images for Average";
			INumericEntryDialog.m_DialogTitleStaticText3 = "Default Value: 200";
			INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + INumericEntryDialog.vEditString;

			INumericEntryDialog.m_UnitsString = "";
				INumericEntryDialog.vMaxValue = 20000;
			if (vLocalSystemData->vITIPCDig->vAverageEnabled)
				INumericEntryDialog.vMaxValue = vLocalConfigurationData->vAverageDisableCount;
			INumericEntryDialog.vIntegerOnly = true;
			//Pass control to dialog box and display
			int nResponse = INumericEntryDialog.DoModal();
			//dialog box is now closed, if user pressed select do this
			//if user pressed cancel, do nothing
			if (nResponse == IDOK)
			{
				vLocalConfigurationData->vAverageDisableCount = _wtoi(INumericEntryDialog.vEditString);
				SetDlgItemText(IDC_SubFunction1Display,INumericEntryDialog.vEditString);
			}
			else 
			if (nResponse == 10)
			{
				//Main Menu button pressed
				ExitAutoSetupDialog(10);
			}
		}
	}
	else
	if (vMode == cSetROI)
	{  //move top up one pixel
		if (!vRecievedButtonPress)
		{
			//ReportErrorMessage("Left Mouse Button Down IN SUBFUNCTION 1",cAction, 32000);
			if ((vMouseState == 0) || ((vMouseState > 8) && (vMouseState < 17)))	//if mouse is idle or in double click toggle on mode
			{
				vMouseState = 1;	//set mouse to on while pressed down
				KillTimer(vMouseDownRepeatTimerHandle);
				int TimerResult = SetTimer(vMouseDownRepeatTimerHandle,200,NULL);
				if (!TimerResult)
					ReportErrorMessage("Error-Second Timer Failed",cEMailInspx,32000);
			}
		}

		if (vInspectionEditing)
		if (DeleteCheckWeighCalibrations(true))
		{
			bool TempOKToMoveROI = true;
			if (vInspectionEditing->vInspectionType == cJarBottomContaminantInspection)
			if (vInspectionEditing->vROIPixelTop > 55)
			{
				CYesNoDialog TempYesNoDialog;
				TempYesNoDialog.vNoticeText =  _T("\nJar Bottom ROI should contain the punt (pushup, or baffle) of the jar,\nand go about an inch above the punt.\nAre you sure you want to move it?");
				TempYesNoDialog.vYesButtonText = "Leave As Is";
				TempYesNoDialog.vNoButtonText = "Move ROI";
				TempYesNoDialog.vQuestionType = cConfirmQuestion;
				int TempResult = TempYesNoDialog.DoModal();
				if (TempResult == IDOK)
					TempOKToMoveROI = false;
			}

			if (TempOKToMoveROI)
			if (vInspectionEditing->vROIPixelTop < vLocalSystemData->vITIPCDig->vOriginalBufferSizeX)
			{
				vInspectionEditing->vROIPixelTop++; 
				SetROILines();
			}
		}
	}
	else
	if (vMode == cSetDensityThreshold) //onSubFunction1Button
	{ 
		if (vInspectionEditing)
		if (vInspectionEditing->vInspectionNameForWeightTrendingPointer) //if wieght trending inspection, use other inspections settings
			GiveLinkedInspectionNotice();
		else
		if ((vInspectionEditing->vInspectionType == cCheckWeighInspection) || (vInspectionEditing->vInspectionType == cUnderfillByWeightInspection) || (vInspectionEditing->vInspectionType == cOverfillByWeightInspection))
		if (DeleteCheckWeighCalibrations(false))
		{
			CNumericEntryDialog INumericEntryDialog;  
			
			//Set dialog box data titles and number value
			INumericEntryDialog.vEditString = dtoa(vInspectionEditing->vCheckWeighMinimumGoodWeight,2);

			INumericEntryDialog.m_DialogTitleStaticText1 = "Enter the weight of the light sample sending";
			INumericEntryDialog.m_DialogTitleStaticText2 = "Used to calibrate density to " + vInspectionEditing->vCheckWeighUnits;
			INumericEntryDialog.m_DialogTitleStaticText3 = "Send same sample at least 10 times";
			INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + INumericEntryDialog.vEditString;

			INumericEntryDialog.m_UnitsString = "Units in " + vInspectionEditing->vCheckWeighUnits;
			if ((vInspectionEditing->vCheckWeighMaximumGoodWeight) && (vInspectionEditing->vCheckWeighMaximumGoodWeight > 1))
				INumericEntryDialog.vMaxValue = vInspectionEditing->vCheckWeighMaximumGoodWeight;
			else
				INumericEntryDialog.vMaxValue = 30000;

			INumericEntryDialog.vIntegerOnly = false;
			//Pass control to dialog box and display
			int nResponse = INumericEntryDialog.DoModal();
			//dialog box is now closed, if user pressed select do this
			//if user pressed cancel, do nothing
			if (nResponse == IDOK)
			{
				if (vInspectionEditing->vDensityThresholdUpper > 5000)
					vInspectionEditing->vDensityThresholdUpper = (float)vInspectionEditing->ConvertToWeight(vInspectionEditing->vDensityThresholdUpper);

				if (vInspectionEditing->vDensityThresholdLower > 5000)
					vInspectionEditing->SetDensityLowerThreshold((float)vInspectionEditing->ConvertToWeight(vInspectionEditing->vDensityThresholdLower));


				if ((vInspectionEditing->vInspectionType == cUnderfillByWeightInspection) || (vInspectionEditing->vInspectionType == cCheckWeighInspection))
				if (!vInspectionEditing->vCheckWeighMinimumGoodWeight) //if have not entered the weight before, set this to the underfill value
				{
					 vInspectionEditing->vDensityThresholdUpper = (float)vInspectionEditing->ConvertWeightToBrightness(ATOF(INumericEntryDialog.vEditString));
					 DisplayDensityThresholdUpper();
				}

				vInspectionEditing->vCheckWeighMinimumGoodWeight = ATOF(INumericEntryDialog.vEditString);
				if ((vInspectionEditing->vCheckWeighMinimumGoodWeight) && (vInspectionEditing->vCheckWeighMaximumGoodWeight) && 
					(vInspectionEditing->vSetupHistoryMinimumGoodAverage) && (vInspectionEditing->vSetupHistoryMaximumBadAverage) && ((vInspectionEditing->vCheckWeighMFactor != 1) || (vInspectionEditing->vCheckWeighBFactor)))
				{
						vInspectionEditing->vCheckWeighMFactor = (vInspectionEditing->vCheckWeighMinimumGoodWeight - vInspectionEditing->vCheckWeighMaximumGoodWeight) /
							(vInspectionEditing->vSetupHistoryMinimumGoodAverage - vInspectionEditing->vSetupHistoryMaximumBadAverage);
						vInspectionEditing->vCheckWeighOffset = 0;
												
						if ((vInspectionEditing->vSetupHistoryMaximumBadAverage) && (vInspectionEditing->vCheckWeighMFactor))
							vInspectionEditing->vCheckWeighBFactor = vInspectionEditing->vCheckWeighMinimumGoodWeight - (vInspectionEditing->vSetupHistoryMinimumGoodAverage * vInspectionEditing->vCheckWeighMFactor);

						if (PasswordOK(cTemporaryInspxPassword,false))
							SetStatusLine(vInspectionEditing->vName + " " + dtoa(vLocalSystemData->vDensityIndex,0) + ": " + vInspectionEditing->vCheckWeighUnits + "(new)=Density*" +
								dtoa(vInspectionEditing->vCheckWeighMFactor,6) + "+" + dtoa(vInspectionEditing->vCheckWeighBFactor,2));
						else
							SetStatusLine(vInspectionEditing->vName + " Weight calibration adjustment complete");
				}
				else
				{
					vInspectionEditing->vSetupHistoryMaximumBadCount = 0;
					vInspectionEditing->vSetupHistoryMinimumGoodCount = 0;
					vInspectionEditing->vCheckWeighBFactor = 0;
					vInspectionEditing->vCheckWeighMFactor = 1;
					vInspectionEditing->vCheckWeighOffset = 0;
				}

				if (!vInspectionEditing->vCheckWeighMinimumGoodWeight)
					vSubFunction5ButtonEnable = false;		//m_SubFunction5Button.EnableWindow(false);
				else
					vSubFunction5ButtonEnable = true;		//m_SubFunction5Button.EnableWindow(true);

				CWnd *TempWindow = CWnd::GetDlgItem(IDC_SubFunction1Button);
				if (TempWindow)
					TempWindow->Invalidate(false);

				DisplayMinimumGoodMaximumBadDataValues(false);
				DisplayAutoDensityThresholdLower();
				DisplayAutoDensityThresholdUpper();
				DisplayDensityThresholdLower();
				DisplayDensityThresholdUpper();
			}
			else 
			if (nResponse == 10)
			{
				//Main Menu button pressed
				ExitAutoSetupDialog(10);
			}
		}
		UpdateButtonText();
	}
	CWnd *TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText1);
	if (TempWindow)
		TempWindow->Invalidate(false);
}

void CAutoSetupImageDisplayDialog::SetROILines()
{
	if (vInspectionEditing)
	{
		if (vInspectionEditing->vROIPixelTop > vLocalSystemData->vITIPCDig->vOriginalBufferSizeX)
			vInspectionEditing->vROIPixelTop = vLocalSystemData->vITIPCDig->vOriginalBufferSizeX - 1;
		if (vInspectionEditing->vROIPixelBottom >= vInspectionEditing->vROIPixelTop)
			vInspectionEditing->vROIPixelBottom = 1;
		if (vInspectionEditing->vROIPixelRight > vLocalSystemData->vITIPCDig->vOriginalBufferSizeY)
			vInspectionEditing->vROIPixelRight = vLocalSystemData->vITIPCDig->vOriginalBufferSizeY - 1;
		if (vInspectionEditing->vROIPixelLeft >= vInspectionEditing->vROIPixelRight)
			vInspectionEditing->vROIPixelLeft = 1;

		vInspectionEditing->SetROITop(vInspectionEditing->vROIPixelTop / vGlobalPixelsPerUnitInHeight);
		vInspectionEditing->SetROIBottom(vInspectionEditing->vROIPixelBottom / vGlobalPixelsPerUnitInHeight);
		if (vGlobalPixelsPerUnit)
		if (vGlobalCurrentProduct->vOverScanMultiplier)
		{
			vInspectionEditing->SetROILeft(vInspectionEditing->vROIPixelLeft / (vGlobalPixelsPerUnit * vGlobalCurrentProduct->vOverScanMultiplier), vGlobalCurrentProduct->vOverScanMultiplier);
			vInspectionEditing->SetROIRight(vInspectionEditing->vROIPixelRight / (vGlobalPixelsPerUnit * vGlobalCurrentProduct->vOverScanMultiplier),vGlobalCurrentProduct->vOverScanMultiplier);
		}
		if (vInspectionEditing->vROILeft >= vInspectionEditing->vROIRight)
			vInspectionEditing->SetROILeft(vImageMargin, vGlobalCurrentProduct->vOverScanMultiplier);
		if (vInspectionEditing->vROILeft >= vInspectionEditing->vROIRight)
			vInspectionEditing->SetROILeft(0, vGlobalCurrentProduct->vOverScanMultiplier);

		SetChangeMade();
		vMouseClickCount++;
		if (vMouseClickCount % 5 == 0)
			DrawImageOnScreen();
		else
			WaitThenDrawImageOnScreen();
		if (vInspectionEditing == vGlobalCurrentProduct->vDriftDensityInspection)
		{
			UpdateAverageDensityDisplay();
			DisplayCurrentDensityValue();
		}
		ShowLearnSensitivity();
		UpdateButtonText();
	}
}

void CAutoSetupImageDisplayDialog::ClearYellowWarningButton() 
{
	vMainWindowPointer->ClearYellowWarningButton();
	UpdateFunction3Button();
	SetDlgItemText(IDC_StatusLine,vCurrentStatusLineText);
}

void CAutoSetupImageDisplayDialog::SetStatusLine(CString TempStatusLineText) 
{
	if (!vLocalSystemData->vYellowMessageButtonYellow)
		SetDlgItemText(IDC_StatusLine,TempStatusLineText);
	vCurrentStatusLineText = TempStatusLineText;
}

void CAutoSetupImageDisplayDialog::UpdateFunction3Button() 
{
	CString TempText("");
	if (vLocalSystemData->vYellowMessageButtonYellow)
	{
		m_Function3Button.ShowWindow(SW_SHOW);
		TempText.LoadString(IDS_ClearYellowMessage);
		m_Function3Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		SetDlgItemText(IDC_Function3Button,TempText);
	}
	else
	{
		if ((!vStartingUpSource) && (!vRestartingXRaysWithNewSettings) &&
			((!vLocalSystemData->vXRayOnNotice) || (vLocalSystemData->vSystemRunMode == cStoppedSystemMode)))
		{
			m_Function3Button.ShowWindow(SW_SHOW);
			TempText.LoadString(IDS_Run);
			SetDlgItemText(IDC_Function3Button,TempText);
			m_Function3Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
		}
		else
		if (((vMode == cSetContaminantThreshold) || (vMode == cSetVoidThreshold) || (vMode == cSetDensityThreshold)) && (vNumberEjectorsConfigured > 1))
		{
			if (!vEditOnlyThisInspection)
			{
				m_Function3Button.ShowWindow(SW_SHOW);
				TempText.LoadString(IDS_SelectEjectors);
				m_Function3Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
				SetDlgItemText(IDC_Function3Button,TempText);
			}
			else
			{
				m_Function3Button.ShowWindow(SW_HIDE);
			}
		}
		else
		if ((vHaveAnAutoThresholdValue) && ((vInspectionEditing) && (vInspectionEditing->vInspectionType != cUnderfillByWeightInspection) && (vInspectionEditing->vInspectionType != cOverfillByWeightInspection)))
		{
			m_Function3Button.ShowWindow(SW_SHOW);
			TempText.LoadString(IDS_UseAutoThreshold);
			m_Function3Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
			SetDlgItemText(IDC_Function3Button,TempText);
		}
		else
		{
			m_Function3Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_Function3Button.GetRectToInvalidateArrow());
		}
	}
}

void CAutoSetupImageDisplayDialog::OnSubFunction2Button() 
{
	if (vInitialized)
	if (vMode == CMultiLanePositionAdjustment)
	{
		if (vMultiImageLaneEditing)
		{
			vMultiImageLaneEditing--;
			UpdateButtonText();
			WaitThenDrawImageOnScreen();
		}
		else
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\n\nEditing First lane position";
			TempNoticeDialog.vType = cErrorMessage;
			TempNoticeDialog.DoModal();
		}
	}
	else
	if (vMode == cSetSizeSettings)
	{
		if (PasswordOK(cTemporaryInspxPassword,false))
		{
			if (vGlobalCurrentProduct->vThresholdForContainerBounds < 255)
			{
				vGlobalCurrentProduct->vThresholdForContainerBounds++;
				if (!vRecievedButtonPress)
				{
					//ReportErrorMessage("Left Mouse Button Down IN SUBFUNCTION 2",cAction, 32000);
					if ((vMouseState == 0) || ((vMouseState > 8) && (vMouseState < 17)))	//if mouse is idle or in double click toggle on mode
					{
						vMouseState = 2;	//set mouse to on while pressed down
						int TimerResult = SetTimer(vMouseDownRepeatTimerHandle,200,NULL);
						if (!TimerResult)
							ReportErrorMessage("Error-Second Timer Failed",cEMailInspx,32000);
					}
				}
			}
			else
			if (vMouseState > 0)
			{
				KillTimer(vMouseDownRepeatTimerHandle);
				vMouseState = 0;
			}
			SetChangeMade();
			SetDlgItemText(IDC_SubFunction4DisplayA,
				dtoa(vGlobalCurrentProduct->vThresholdForContainerBounds,0));
			WaitThenDrawImageOnScreen();
		}
	}
	else
	if (vMode == cSetContaminantThreshold)
	{		//+1 to Threshold Bounds
		if (vInspectionEditing)
		{
			if (vInspectionEditing->vThreshold < 254)
			{
				if (!vRecievedButtonPress)
				{
					//ReportErrorMessage("Left Mouse Button Down IN SUBFUNCTION 2",cAction, 32000);
					if ((vMouseState == 0) || ((vMouseState > 8) && (vMouseState < 17)))	//if mouse is idle or in double click toggle on mode
					{
						vMouseState = 2;	//set mouse to on while pressed down
						int TimerResult = SetTimer(vMouseDownRepeatTimerHandle,200,NULL);
						if (!TimerResult)
							ReportErrorMessage("Error-Second Timer Failed",cEMailInspx,32000);
					}
				}

				vInspectionEditing->vThreshold++;
			}
			else
			{
				if (vMouseState > 0)
				{
					KillTimer(vMouseDownRepeatTimerHandle);
					vMouseState = 0;
				}
				vInspectionEditing->vThreshold = 255;
			}
		
			if ((vInspectionEditing) && ((vInspectionEditing->vInspectionType == cItemCountInspection) || (vInspectionEditing->vInspectionType == cSizeInspection) || (vInspectionEditing->vInspectionType == cMissingLidInspectionOnSize)))
				ClearEvaluate();
		
			vLocalSystemData->vManuallyEditedDuringEvaluate = true;
			if ((vInspectionEditing->vInspectionType != cItemCountInspection) && (vInspectionEditing->vInspectionType != cSizeInspection) && (vInspectionEditing->vInspectionType != cMissingLidInspectionOnSize))
				SetChangeMade();
			SetDlgItemText(IDC_ThresholdForContainerBounds2,dtoa(vInspectionEditing->vThreshold,0));
			WaitThenDrawImageOnScreen();
			ShowSensitivity();
		}
	}
	else
	if (vMode == cSetVoidThreshold)
	{		//+1 to Void Threshold
		if (vInspectionEditing)
		{
			if (vInspectionEditing->vThreshold < 254)
			{
				vInspectionEditing->vThreshold++;

				if (!vRecievedButtonPress)
				{
					//ReportErrorMessage("Left Mouse Button Down IN SUBFUNCTION 2",cAction, 32000);
					if ((vMouseState == 0) || ((vMouseState > 8) && (vMouseState < 17)))	//if mouse is idle or in double click toggle on mode
					{
						vMouseState = 2;	//set mouse to on while pressed down
						int TimerResult = SetTimer(vMouseDownRepeatTimerHandle,200,NULL);
						if (!TimerResult)
							ReportErrorMessage("Error-Second Timer Failed",cEMailInspx,32000);
					}
				}
			}
			else
			{
				vInspectionEditing->vThreshold = 255;
				if (vMouseState > 0)
				{
					KillTimer(vMouseDownRepeatTimerHandle);
					vMouseState = 0;
				}
			}
		
			if ((vInspectionEditing) && ((vInspectionEditing->vInspectionType == cItemCountInspection) || (vInspectionEditing->vInspectionType == cSizeInspection) || (vInspectionEditing->vInspectionType == cMissingLidInspectionOnSize)))
				ClearEvaluate();
		
			vLocalSystemData->vManuallyEditedDuringEvaluate = true;
			if ((vInspectionEditing->vInspectionType != cItemCountInspection) && (vInspectionEditing->vInspectionType != cSizeInspection) && (vInspectionEditing->vInspectionType != cMissingLidInspectionOnSize))
				SetChangeMade();
			SetDlgItemText(IDC_ThresholdForContainerBounds2,dtoa(vInspectionEditing->vThreshold,0));
			WaitThenDrawImageOnScreen();
		}
	}
	else
	if (vMode == cLearnMenu)
	{
		if (PasswordOK(cTemporaryInspxPassword,false))
		{	//Structure disable count button pressed
			CNumericEntryDialog INumericEntryDialog;  
			
			//Set dialog box data titles and number value
			INumericEntryDialog.vEditString = dtoa(vLocalConfigurationData->vStructureDisableCount, 0);

			INumericEntryDialog.m_DialogTitleStaticText1 = "";
			INumericEntryDialog.m_DialogTitleStaticText2 = "Number of Images for Structure";
			INumericEntryDialog.m_DialogTitleStaticText3 = "Default Value: 2000";
			INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + INumericEntryDialog.vEditString;

			INumericEntryDialog.m_UnitsString = "";
			INumericEntryDialog.vMaxValue = 20000;
			if (vLocalSystemData->vITIPCDig->vStructureEnabled)
				INumericEntryDialog.vMaxValue = vLocalConfigurationData->vStructureDisableCount;
			INumericEntryDialog.vIntegerOnly = true;
			//Pass control to dialog box and display
			int nResponse = INumericEntryDialog.DoModal();
			//dialog box is now closed, if user pressed select do this
			//if user pressed cancel, do nothing
			if (nResponse == IDOK)
			{
				vLocalConfigurationData->vStructureDisableCount = _wtoi(INumericEntryDialog.vEditString);
				SetDlgItemText(IDC_SubFunction2Display,INumericEntryDialog.vEditString);
			}
			else 
			if (nResponse == 10)
			{
				//Main Menu button pressed
				ExitAutoSetupDialog(10);
			}
		}
	}
	else
	if (vMode == cSetROI)
	{  //move top down one pixel
		
		if (vInspectionEditing)
		if (DeleteCheckWeighCalibrations(true))
		{
			bool TempOKToMoveROI = true;
			if (vInspectionEditing->vInspectionType == cJarBottomContaminantInspection)
			if (vInspectionEditing->vROIPixelTop < 45)
			{
				CYesNoDialog TempYesNoDialog;
				TempYesNoDialog.vNoticeText =  _T("\nJar Bottom ROI should contain the punt (pushup, or baffle) of the jar,\nand go about an inch above the punt.\nAre you sure you want to move it?");
				TempYesNoDialog.vYesButtonText = "Leave As Is";
				TempYesNoDialog.vNoButtonText = "Move ROI";
				TempYesNoDialog.vQuestionType = cConfirmQuestion;
				int TempResult = TempYesNoDialog.DoModal();
				if (TempResult == IDOK)
					TempOKToMoveROI = false;
			}

			if (TempOKToMoveROI)
			if (vInspectionEditing->vROIPixelTop > vInspectionEditing->vROIPixelBottom + 1)
			{
				if (!vRecievedButtonPress)
				{
					//ReportErrorMessage("Left Mouse Button Down IN SUBFUNCTION 2",cAction, 32000);
					if ((vMouseState == 0) || ((vMouseState > 8) && (vMouseState < 17)))	//if mouse is idle or in double click toggle on mode
					{
						vMouseState = 2;	//set mouse to on while pressed down
						int TimerResult = SetTimer(vMouseDownRepeatTimerHandle,200,NULL);
						if (!TimerResult)
							ReportErrorMessage("Error-Second Timer Failed",cEMailInspx,32000);
					}
				}
				vInspectionEditing->vROIPixelTop--; 
				SetROILines();
			}
			else
			if (vMouseState > 0)
			{
				KillTimer(vMouseDownRepeatTimerHandle);
				vMouseState = 0;
			}
		}
	}
	else
	if (vMode == cSetDensityThreshold)
	{ 
		if (vInspectionEditing)
		if (vInspectionEditing->vInspectionType == cOverfillByWeightInspection)
		{
			if (vInspectionEditing->vInspectionNameForWeightTrendingPointer) //if wieght trending inspection, use other inspections settings
				GiveLinkedInspectionNotice();
			else
				EnterNominalWeight();
		}
		else
		if (((vInspectionEditing->vInspectionType == cUnderfillByWeightInspection) || (vInspectionEditing->vInspectionType == cCheckWeighInspection))	&& (!vInspectionEditing->vInspectionNameForWeightTrendingPointer) &&  //if using your own ROI and weight calibrations
			((!vInspectionEditing->vCheckWeighMinimumGoodWeight) || (!vInspectionEditing->vCheckWeighMaximumGoodWeight)))
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\nYou must enter the weight of the\nlight and heavy samples first";
			TempNoticeDialog.vType = cErrorMessage;
			TempNoticeDialog.DoModal();
		}
		else
		{
			CNumericEntryDialog INumericEntryDialog;  
			
			//Set dialog box data titles and number value
			INumericEntryDialog.vEditString = dtoa(vInspectionEditing->vDensityThresholdUpper,2);

			if ((vInspectionEditing) &&
				(vInspectionEditing->vInspectionType == cStandardDeviationInspection))
				INumericEntryDialog.m_DialogTitleStaticText1 = "Upper Threshold of Texture";
			else
				INumericEntryDialog.m_DialogTitleStaticText1 = "Upper Threshold of Density";
			INumericEntryDialog.m_DialogTitleStaticText2 = "of pixel values in ROI";
			INumericEntryDialog.m_DialogTitleStaticText3 = "Values above this threshold is rejected, 0 = disabled";
			INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + INumericEntryDialog.vEditString;

			INumericEntryDialog.m_UnitsString = "In Pixel Intensity";
			if ((vInspectionEditing->vCheckWeighMFactor != 1) || (vInspectionEditing->vCheckWeighBFactor) || 
				(vInspectionEditing->vInspectionType == cUnderfillByWeightInspection) || (vInspectionEditing->vInspectionType == cOverfillByWeightInspection))
			{
				INumericEntryDialog.vMinValue = 0;
				if ((vInspectionEditing->vHasDensityLowerThreshold) && (vInspectionEditing->vInspectionType != cUnderfillByWeightInspection))
					INumericEntryDialog.vMaxValue = vInspectionEditing->ConvertToWeight(vInspectionEditing->vDensityThresholdLower);
				else
					INumericEntryDialog.vMaxValue = 30000;
			}
			else
			{
				INumericEntryDialog.vMaxValue = 255;
				if (vInspectionEditing->vHasDensityLowerThreshold)
					INumericEntryDialog.vMinValue = vInspectionEditing->ConvertToWeight(vInspectionEditing->vDensityThresholdLower);
				else
					INumericEntryDialog.vMinValue = 0;
			}
			INumericEntryDialog.vIntegerOnly = false;

			if ((vInspectionEditing->vInspectionType == cCheckWeighInspection) || (vInspectionEditing->vInspectionType == cUnderfillByWeightInspection) || (vInspectionEditing->vInspectionType == cOverfillByWeightInspection))
			{
				if ((vInspectionEditing->vCheckWeighMFactor != 1) || (vInspectionEditing->vCheckWeighBFactor))
				{
					INumericEntryDialog.m_DialogTitleStaticText2 = "Weight of container";
					INumericEntryDialog.m_DialogTitleStaticText3 = "Values below this threshold cause an Alarm, 0 = disabled";
				}
				else
				{
					INumericEntryDialog.m_DialogTitleStaticText2 = "Average of pixels in ROI";
					INumericEntryDialog.m_DialogTitleStaticText3 = "Values below this threshold cause an Alarm, 0 = disabled";
				}

				INumericEntryDialog.m_UnitsString = "Units In " + vInspectionEditing->vCheckWeighUnits;
				INumericEntryDialog.m_DialogTitleStaticText1 = "Weight Trending Threshold for Low Fill";
				INumericEntryDialog.vEditString = dtoa(vInspectionEditing->ConvertToWeight(vInspectionEditing->vDensityThresholdUpper),2);

				if ((vInspectionEditing->vHasDensityLowerThreshold) && (vInspectionEditing->vInspectionType != cUnderfillByWeightInspection))
					INumericEntryDialog.vMaxValue = vInspectionEditing->ConvertToWeight(vInspectionEditing->vDensityThresholdLower);
				else
					INumericEntryDialog.vMaxValue = 30000;

				INumericEntryDialog.vMinValue = 0;

				INumericEntryDialog.m_DialogTitleStaticText1 = "Under Weight Threshold";
				INumericEntryDialog.m_DialogTitleStaticText2 = "Weight of Product in image";
				INumericEntryDialog.vEditString = dtoa(vInspectionEditing->ConvertToWeight(vInspectionEditing->vDensityThresholdUpper),2);
				if (((vInspectionEditing->vCheckWeighMFactor != 1) || (vInspectionEditing->vCheckWeighBFactor)) && (vInspectionEditing->vCheckWeighMFactor != 0))
				{
					INumericEntryDialog.m_UnitsString = "Value is Container Weight in " + vInspectionEditing->vCheckWeighUnits;
					INumericEntryDialog.m_DialogTitleStaticText3 = "Values above this threshold are ejected, 0 = disabled";
				}
				else
				{
					INumericEntryDialog.m_UnitsString = "Value is Total Pixel Weight";
					INumericEntryDialog.m_DialogTitleStaticText3 = "Values below this threshold are ejected, 0 = disabled";
				}
			}

			INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + INumericEntryDialog.vEditString;

			//Pass control to dialog box and display
			int nResponse = INumericEntryDialog.DoModal();
			//dialog box is now closed, if user pressed select do this
			//if user pressed cancel, do nothing
			if (nResponse == IDOK)
			{
				SetChangeMade();
				vInspectionEditing->vDensityThresholdUpper = (float)ATOF(INumericEntryDialog.vEditString);
				if ((vInspectionEditing->vCheckWeighMFactor != 1) || (vInspectionEditing->vCheckWeighMFactor))
				{
					vInspectionEditing->vDensityThresholdUpper = (float)vInspectionEditing->ConvertWeightToBrightness(ATOF(INumericEntryDialog.vEditString));

					if (vInspectionEditing->vDensityThresholdUpper < 0)
						vInspectionEditing->vDensityThresholdUpper = 0;
				}
				DisplayDensityThresholdUpper();
			}
			else 
			if (nResponse == 10)
			{
				//Main Menu button pressed
				ExitAutoSetupDialog(10);
			}
			SetChangeMade();
		}
	}
	else
	if (vMode == cSetImageSize) //sF2
	{
		if (!vGlobalCurrentProduct->vMultiLaneNumberOfLanes) //don't do if in Multi Lane
		if ((PasswordOK(cAdvancedSetupPassword,false)) || ((!vLocalConfigurationData->vContinuousFeedContainerRate) && (!vLocalConfigurationData->vBulkProductMode) && (!vGlobalCurrentProduct->vBulkProductMode) && 
			(vLocalConfigurationData->vScanTracType != cBriosoScanTrac))) //Not Continuous Feed, Not a Pipeline or password in
		if (!vLocalSystemData->vInAutoSetup)
		{
			YouCantChangeSettingBecauseInspecting();
		}
		else
		if (!vHaveAnImage)
		{
			CNoticeDialog TempNoticeDialog;
			CString TempText;
			TempText.LoadString(IDS_Needanewimagetoautosize);//"\n\n\nNeed a new image to auto size";
			TempNoticeDialog.vNoticeText = TempText;
			TempNoticeDialog.vType = cErrorMessage;
			TempNoticeDialog.vAutoClose = 1000;
			TempNoticeDialog.DoModal();
		}
		else
		if (vNeedToAutoSizeNextContainer)
		{
			CNoticeDialog TempNoticeDialog;
			CString TempText;
			TempText.LoadString(IDS_AlreadyWaitingFor2FootGapInContainers);
			TempNoticeDialog.vNoticeText = TempText;
			TempNoticeDialog.vType = cErrorMessage;
			TempNoticeDialog.vAutoClose = 1000;
			TempNoticeDialog.DoModal();
		}
		else
		{
			if (NoSetupOrOKToChange())
			{
				bool TempGapLengthOK = !vLocalConfigurationData->vShowOnlyRealImages;  //if showing simulated images, gap length is OK, else not

				if (vLocalSystemData->vITIPCDig->vOriginalImage)
				if (vLocalSystemData->vITIPCDig->vOriginalImage->vBodyTriggerGapLength > 
					vGlobalPixelsPerUnit * (24 + vGlobalCurrentProduct->vProductLockOutWidth)) //24 inches plus image width
					TempGapLengthOK = true;

				if ((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode)) //Continuous Feed, or a Pipeline
					TempGapLengthOK = true;

				if (!TempGapLengthOK)
				if (PasswordOK(cSuperInspxPassword,false))
				if (vLocalSystemData->vITIPCDig->vOriginalImage)
				{
					CYesNoDialog TempYesNoDialog;
					CString TempText = dtoa(vLocalSystemData->vITIPCDig->vOriginalImage->vBodyTriggerGapLength / vGlobalPixelsPerUnit,0);
					TempText = "\nGap between containers only: " + TempText +
						" inches\n" + dtoa(24 + vGlobalCurrentProduct->vProductLockOutWidth, 0) + " inches reccommended.\nDo you want to AutoSize anyway?";
					TempYesNoDialog.vNoticeText = TempText;
					TempYesNoDialog.vQuestionType = cConfirmQuestion;
					TempYesNoDialog.vYesButtonText = "AutoSize Anyway";
					TempYesNoDialog.vNoButtonText = "Wait 2 Feet";
					int TempResult = TempYesNoDialog.DoModal();
					if (TempResult == IDOK)
					TempGapLengthOK = true;
				}

				if (!TempGapLengthOK)
				{
					vNeedToAutoSizeNextContainer = true;
					CString TempText;
					TempText.LoadString(IDS_WaitingForAGapOfAtleast2FeetToAutoSize);

					vMakeStatusLineYellowRed = true;
					SetStatusLine(TempText);
					UpdateButtonText();

					CNoticeDialog TempNoticeDialog;
					TempText.LoadString(IDS_3Lines_WaitingForAGapOfAtleast2FeetToAutoSize);
					TempNoticeDialog.vNoticeText = TempText;
					TempNoticeDialog.vType = cErrorMessage;
					TempNoticeDialog.DoModal();
				}
				if (TempGapLengthOK)
					AutoSizeImage();
			}
		}
		UpdateButtonText();
	}
}

void CAutoSetupImageDisplayDialog::OnSubFunction3Button() 
{
	if (vInitialized)
	if (vMode == cSetSizeSettings)
	{
		if (PasswordOK(cTemporaryInspxPassword,false))
		{
			if (vGlobalCurrentProduct->vThresholdForContainerBounds)
			{
				if (!vRecievedButtonPress)
				{
					//ReportErrorMessage("Left Mouse Button Down IN SUBFUNCTION 2",cAction, 32000);
					if ((vMouseState == 0) || ((vMouseState > 8) && (vMouseState < 17)))	//if mouse is idle or in double click toggle on mode
					{
						vMouseState = 3;	//set mouse to on while pressed down
						int TimerResult = SetTimer(vMouseDownRepeatTimerHandle,200,NULL);
						if (!TimerResult)
							ReportErrorMessage("Error-Second Timer Failed",cEMailInspx,32000);
					}
				}
		
				vGlobalCurrentProduct->vThresholdForContainerBounds--;
			}
			else
			if (vMouseState > 0)
			{
				KillTimer(vMouseDownRepeatTimerHandle);
				vMouseState = 0;
			}
			SetChangeMade();
			SetDlgItemText(IDC_SubFunction4DisplayA, dtoa(vGlobalCurrentProduct->vThresholdForContainerBounds,0));
			WaitThenDrawImageOnScreen();
		}
	}
	else
	if (vMode == cSetImageSize) //sF3
	{
		//RetriggerLockout Button Was Pressed
		if (!vGlobalCurrentProduct->vMultiLaneNumberOfLanes) //don't do if in Multi Lane
		if ((PasswordOK(cCertifiedPassword,false)) || ((!vLocalConfigurationData->vContinuousFeedContainerRate) && (!vLocalConfigurationData->vBulkProductMode) && (!vGlobalCurrentProduct->vBulkProductMode))) //Not Continuous Feed, Not a Pipeline or password in
		{
			float TempRetriggerLockout = vGlobalCurrentProduct->vProductLockOutWidth;
			CNumericEntryDialog INumericEntryDialog;  
			
			//Set dialog box data titles and number value
			INumericEntryDialog.vEditString = dtoa(vGlobalCurrentProduct->vProductLockOutWidth,2);

			INumericEntryDialog.m_DialogTitleStaticText1 = "Retrigger Lockout to prevent double triggering.";
			INumericEntryDialog.m_DialogTitleStaticText2 = "Product: " + *vGlobalCurrentProduct->GetProductName();
			if ((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode)) //Continuous Feed, or a Pipeline
				INumericEntryDialog.m_DialogTitleStaticText3 = "Default Value: 4 inches";
			else
				INumericEntryDialog.m_DialogTitleStaticText3 = "Default Value: width of container";
			INumericEntryDialog.m_DialogTitleStaticText4 = "";
			if (vLocalSystemData->vBodyTriggerWidthSamples)
			{
				double *TempSource = &vLocalSystemData->vBodyTriggerWidthData[0];
				double TempMaximum = (WORD)(vLocalSystemData->vBodyTriggerWidthAverage * 1.6);
				double TempMaximumValue = 0;
				for (WORD TempLoop = 0; TempLoop < vLocalSystemData->vBodyTriggerWidthSamples; TempLoop++)
				{
					if ((!TempMaximum) || (*TempSource < TempMaximum))
					//if (*TempSource > 14)
					if (TempMaximumValue < *TempSource)
						TempMaximumValue = *TempSource;

					TempSource++;
				}
				double TempSuggestedBodyTriggerLockout = 
					((TempMaximumValue + 1) / (vGlobalPixelsPerUnit * vGlobalCurrentProduct->vOverScanMultiplier));

				if (TempSuggestedBodyTriggerLockout < .5)
					TempSuggestedBodyTriggerLockout = .5;

				INumericEntryDialog.m_DialogTitleStaticText4 = "Suggested Value from Optical Sensor: " + dtoa(TempSuggestedBodyTriggerLockout,2);
			}
			if ((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode)) //Continuous Feed, or a Pipeline
				INumericEntryDialog.m_DialogTitleStaticText4 = "Default Value: 1 inch";
			else
			if (vGlobalCurrentProduct->vReferenceWidth)
			{
				INumericEntryDialog.m_DialogTitleStaticText4 = "Container Width (suggested maximum): " +
					dtoa(vGlobalCurrentProduct->vReferenceWidth / vGlobalPixelsPerUnit / vGlobalCurrentProduct->vOverScanMultiplier,2);
			}

			INumericEntryDialog.m_UnitsString = "Measured in " + vLocalConfigurationData->vUnitsString;
			INumericEntryDialog.vMaxValue = vGlobalCurrentProduct->vProductImageWidth * 2.0; 
			if (INumericEntryDialog.vMaxValue < 35)
				INumericEntryDialog.vMaxValue = 35;
			//if (vLocalConfigurationData->vUseMetric)
			//if (INumericEntryDialog.vMaxValue < 18 * (float)25.4)
			//	INumericEntryDialog.vMaxValue = 18 * (float)25.4;

			INumericEntryDialog.vMinValue = 0;
			INumericEntryDialog.vIntegerOnly = false;
			//Pass control to dialog box and display
			int nResponse = INumericEntryDialog.DoModal();
			//dialog box is now closed, if user pressed select do this
			//if user pressed cancel, do nothing
			if (nResponse == IDOK)
			{
				double TempLockOutWidth = ATOF(INumericEntryDialog.vEditString);

				bool TempOKToSet = true;

				if (TempLockOutWidth * 1.93 <= vGlobalCurrentProduct->vProductImageWidth)
				{
					CYesNoDialog TempYesNoDialog;
					TempYesNoDialog.vNoticeText =  _T("\nWarning\nRe-Trigger Lockout increased to half image width\nScanTrac will skip containers (trigger will be locked out)\n if containers are too close together.\nUse Autosize on new image to correct!");
					TempYesNoDialog.vQuestionType = cQuestion;
					TempYesNoDialog.vYesButtonText = "Proceed";
					TempYesNoDialog.vNoButtonText = "Cancel";
					int TempResult = TempYesNoDialog.DoModal();
					if (TempResult == IDOK)
						TempLockOutWidth = (vGlobalCurrentProduct->vProductImageWidth / 1.93) + (1.0 / (vGlobalPixelsPerUnit * vGlobalCurrentProduct->vOverScanMultiplier));
					else
						TempOKToSet = false;
				}

				if (TempOKToSet)
				{
					vGlobalCurrentProduct->vProductLockOutWidth = (float)TempLockOutWidth;
					vGlobalCurrentProduct->SetProductLockOutWidth(vGlobalCurrentProduct->vProductLockOutWidth);

					vGlobalCurrentProduct->vHaveManuallySetRetriggerLockout = true;
					SetChangeMade();
					DisplayWithPassword();

					if (TempRetriggerLockout != vGlobalCurrentProduct->vProductLockOutWidth)
					if (vMainWindowPointer)
						vMainWindowPointer->SendRetriggerLockout(vGlobalCurrentProduct->vLockoutPosition);
						if (vLocalConfigurationData->vResynchronizeEjectors)
						{
							BYTE TempEjectorsEnabled = false;
							if (vLocalConfigurationData->vEnableEjectors)
							{
								TempEjectorsEnabled = true;
								vLocalConfigurationData->vEnableEjectors = false;
								if (vGlobaluCSerialPort)
									vGlobaluCSerialPort->EnableDisableEjectors(vLocalConfigurationData->vEnableEjectors);
								Sleep(cSendCommandSleepTimeSetup);
							}

							//send re-trigger lockout to sensor 1
							tSerialCommand TempCommand;
							TempCommand[0] = 0x3C;
							if (vLocalConfigurationData->vApplyRetriggerLockoutToResynchronizingSensors)
							{
								TempCommand[1] = (BYTE)(vGlobalCurrentProduct->vLockoutPosition >> 8);
								TempCommand[2] = (BYTE)vGlobalCurrentProduct->vLockoutPosition;
							}
							else
							{
								TempCommand[1] = 0;
								TempCommand[2] = 0;
							}
							TempCommand[3] = 1;
							if (vGlobaluCSerialPort)
								vGlobaluCSerialPort->SendSerialCommand(TempCommand);
							Sleep(cSendCommandSleepTimeSetup);

							//send re-trigger lockout to sensor 2
							TempCommand[3] = 2;
							if (vGlobaluCSerialPort)
								vGlobaluCSerialPort->SendSerialCommand(TempCommand);
							Sleep(cSendCommandSleepTimeSetup);

							if (TempEjectorsEnabled) //re-enable ejectors if had to disable them to send the dwell times
							{
								vLocalConfigurationData->vEnableEjectors = true;
								if (vGlobaluCSerialPort)
									vGlobaluCSerialPort->EnableDisableEjectors(vLocalConfigurationData->vEnableEjectors);
								Sleep(cSendCommandSleepTimeSetup);
							}
						}
				}
			}
			else 
			if (nResponse == 10)
			{
				//Main Menu button pressed
				ExitAutoSetupDialog(10);
			}
		}
	}
	else
	if (vMode == cSetContaminantThreshold)
	{		//-1 to Threshold Bounds
		if (vInspectionEditing)
		{
			if (vInspectionEditing->vThreshold)
			{
				vInspectionEditing->vThreshold--;

				if (!vRecievedButtonPress)
				{
					//ReportErrorMessage("Left Mouse Button Down IN SUBFUNCTION 3",cAction, 32000);
					if ((vMouseState == 0) || ((vMouseState > 8) && (vMouseState < 17)))	//if mouse is idle or in double click toggle on mode
					{
						vMouseState = 3;	//set mouse to on while pressed down
						int TimerResult = SetTimer(vMouseDownRepeatTimerHandle,200,NULL);
						if (!TimerResult)
							ReportErrorMessage("Error-Second Timer Failed",cEMailInspx,32000);
					}
				}
			}
			else
			{
				vInspectionEditing->vThreshold = 0;
				if (vMouseState > 0)
				{
					KillTimer(vMouseDownRepeatTimerHandle);
					vMouseState = 0;
				}
			}
		
			if ((vInspectionEditing) && ((vInspectionEditing->vInspectionType == cItemCountInspection) || (vInspectionEditing->vInspectionType == cSizeInspection) || (vInspectionEditing->vInspectionType == cMissingLidInspectionOnSize)))
				ClearEvaluate();
		
			vLocalSystemData->vManuallyEditedDuringEvaluate = true;
			if ((vInspectionEditing->vInspectionType != cItemCountInspection) && (vInspectionEditing->vInspectionType != cSizeInspection) && (vInspectionEditing->vInspectionType != cMissingLidInspectionOnSize))
				SetChangeMade();
			SetDlgItemText(IDC_ThresholdForContainerBounds2,dtoa(vInspectionEditing->vThreshold,0));
			WaitThenDrawImageOnScreen();
			ShowSensitivity();
		}
	}
	else
	if (vMode == cSetVoidThreshold)
	{		//-1 to Void Threshold

		if (vInspectionEditing)
		{
			if (vInspectionEditing->vThreshold)
			{
				vInspectionEditing->vThreshold--;
				if (!vRecievedButtonPress)
				{
					//ReportErrorMessage("Left Mouse Button Down IN SUBFUNCTION 3",cAction, 32000);
					if ((vMouseState == 0) || ((vMouseState > 8) && (vMouseState < 17)))	//if mouse is idle or in double click toggle on mode
					{
						vMouseState = 3;	//set mouse to on while pressed down
						int TimerResult = SetTimer(vMouseDownRepeatTimerHandle,200,NULL);
						if (!TimerResult)
							ReportErrorMessage("Error-Second Timer Failed",cEMailInspx,32000);
					}
				}
			}
			else
			{
				vInspectionEditing->vThreshold = 0;
				if (vMouseState > 0)
				{
					KillTimer(vMouseDownRepeatTimerHandle);
					vMouseState = 0;
				}
			}
		
			if ((vInspectionEditing) && ((vInspectionEditing->vInspectionType == cItemCountInspection) || (vInspectionEditing->vInspectionType == cSizeInspection) || (vInspectionEditing->vInspectionType == cMissingLidInspectionOnSize)))
				ClearEvaluate();
		
			vLocalSystemData->vManuallyEditedDuringEvaluate = true;
			SetChangeMade();
			SetDlgItemText(IDC_ThresholdForContainerBounds2,dtoa(vInspectionEditing->vThreshold,0));
			WaitThenDrawImageOnScreen();
		}
	}
	else
	//Histogram disable count button pressed
	if (vMode == cLearnMenu)
	{
		if (PasswordOK(cTemporaryInspxPassword,false))
		{
			CNumericEntryDialog INumericEntryDialog;  
			
			//Set dialog box data titles and number value
			INumericEntryDialog.vEditString = dtoa(vGlobalCurrentProduct->vHistogramThreshold, 0);

			INumericEntryDialog.m_DialogTitleStaticText1 = "Threshold for Histogram";
			INumericEntryDialog.m_DialogTitleStaticText2 = "Setting for product: " + vProductEditingName;
			INumericEntryDialog.m_DialogTitleStaticText3 = "Default Value: 240";
			INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + INumericEntryDialog.vEditString;

			INumericEntryDialog.m_UnitsString = "";
				INumericEntryDialog.vMaxValue = 255;
			INumericEntryDialog.vIntegerOnly = true;
			//Pass control to dialog box and display
			int nResponse = INumericEntryDialog.DoModal();
			//dialog box is now closed, if user pressed select do this
			//if user pressed cancel, do nothing
			if (nResponse == IDOK)
			{
				vGlobalCurrentProduct->vHistogramThreshold = _wtoi(INumericEntryDialog.vEditString);
				SetDlgItemText(IDC_SubFunction3Display,INumericEntryDialog.vEditString);

				//Set dialog box data titles and number value
				INumericEntryDialog.vEditString = dtoa(vGlobalCurrentProduct->vHistogramLowerThreshold);

				INumericEntryDialog.m_DialogTitleStaticText1 = "Lower Threshold of Histogram";
				INumericEntryDialog.m_DialogTitleStaticText2 = "Default Value: 0";
				INumericEntryDialog.m_DialogTitleStaticText3 = "Setting for product: " + vProductEditingName;
				INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + INumericEntryDialog.vEditString;

				INumericEntryDialog.vMaxValue = 250;
				//Pass control to dialog box and display
				int nResponse = INumericEntryDialog.DoModal();
				if (nResponse == IDOK)
				{
					vGlobalCurrentProduct->vHistogramLowerThreshold = _wtoi(INumericEntryDialog.vEditString);
					//SetDlgItemText(IDC_SubFunction3Display,INumericEntryDialog.vEditString);

					//Set dialog box data titles and number value
					INumericEntryDialog.vEditString = dtoa(vGlobalCurrentProduct->vHistogramDilateTimes);

					INumericEntryDialog.m_DialogTitleStaticText1 = "Number of times to Dilate Histogram";
					INumericEntryDialog.m_DialogTitleStaticText2 = "Setting for product: " + vProductEditingName;
					INumericEntryDialog.m_DialogTitleStaticText3 = "Default Value: 0";
					INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + INumericEntryDialog.vEditString;

					INumericEntryDialog.vMaxValue = 25;
					//Pass control to dialog box and display
					int nResponse = INumericEntryDialog.DoModal();
					if (nResponse == IDOK)
					{
						vGlobalCurrentProduct->vHistogramDilateTimes = _wtoi(INumericEntryDialog.vEditString);

						if ((vLocalSystemData->vITIPCDig->vProductAverageImage) && (vLocalSystemData->vITIPCDig->vProductAverageImage->vGreyImage))
						{
							vLocalSystemData->vITIPCDig->SaveThresholdImageInHistogram(vGlobalCurrentProduct, vGlobalCurrentProduct->vHistogramThreshold,
								vLocalSystemData->vITIPCDig->vProductAverageImage->vGreyImage, vGlobalCurrentProduct->vHistogramDilateTimes, vGlobalCurrentProduct->vHistogramLowerThreshold);

							vGlobalCurrentProduct->vSetupHistoryHistogramThreshold = vGlobalCurrentProduct->vHistogramThreshold;

							vShowHistogram = true;
							vShowStructure = false;
							vShowVoidStructure = false;
							//vShowGlassStructure = false;
							vShowContaminant2Structure = false;
							vShowAverage = false;
							DrawImageOnScreen();
							UpdateButtonText();
							//SaveHistogramDataToFile();
						}
					}
				}
				else 
				if (nResponse == 10)
				{
					//Main Menu button pressed
					ExitAutoSetupDialog(10);
				}
			}
			else 
			if (nResponse == 10)
			{
				//Main Menu button pressed
				ExitAutoSetupDialog(10);
			}
		}
	}
	else
	if (vMode == cSetROI)
	{  //move bottom up one pixel
		if (!vRecievedButtonPress)
		{
			//ReportErrorMessage("Left Mouse Button Down IN SUBFUNCTION 3",cAction, 32000);
			if ((vMouseState == 0) || ((vMouseState > 8) && (vMouseState < 17)))	//if mouse is idle or in double click toggle on mode
			{
				vMouseState = 3;	//set mouse to on while pressed down
				int TimerResult = SetTimer(vMouseDownRepeatTimerHandle,200,NULL);
				if (!TimerResult)
					ReportErrorMessage("Error-Second Timer Failed",cEMailInspx,32000);
			}
		}
		
		if (vInspectionEditing)
		if (DeleteCheckWeighCalibrations(true))
		{
			if ((vInspectionEditing->vInspectionType == cGlass13mmContaminantInspection) && (vInspectionEditing->vPerformGlassAlgorithm == 1) &&
				(vInspectionEditing->vROIPixelBottom < 15))
			{ //glass in body inspection, must go to pixel 1 if below pixel 20
				vInspectionEditing->vROIPixelBottom = 15; 
				vInspectionEditing->vRemoveJarPunt = false;
				SetROILines();

				CNoticeDialog TempNoticeDialog;
				TempNoticeDialog.vNoticeText = "\n\n\nMoved Bottom ROI above Push Up.";
				TempNoticeDialog.vType = cInformationMessage;
				TempNoticeDialog.vAutoClose = 2000;
				TempNoticeDialog.DoModal();
			}
			else
			{
				bool TempOKToMoveROI = true;
				if (vInspectionEditing->vInspectionType == cJarBottomContaminantInspection)
				if (vInspectionEditing->vROIPixelBottom > 15)
				{
					CYesNoDialog TempYesNoDialog;
					TempYesNoDialog.vNoticeText =  _T("\nJar Bottom ROI should contain the punt (pushup, or Baffle) of the jar.\nAre you sure you want to move it?");
					TempYesNoDialog.vYesButtonText = "Leave As Is";
					TempYesNoDialog.vNoButtonText = "Move ROI";
					TempYesNoDialog.vQuestionType = cConfirmQuestion;
					int TempResult = TempYesNoDialog.DoModal();
					if (TempResult == IDOK)
						TempOKToMoveROI = false;
				}

				if (TempOKToMoveROI)
				if (vInspectionEditing->vROIPixelBottom < vInspectionEditing->vROIPixelTop - 1)
				{
					vInspectionEditing->vROIPixelBottom++; 
					SetROILines();
				}
			}
		}
	}
	else
	if (vMode == cSetDensityThreshold)
	{  
		if (vInspectionEditing)
		if (vInspectionEditing->vInspectionType == cUnderfillByWeightInspection)
		{
			if (vInspectionEditing->vInspectionNameForWeightTrendingPointer) //if wieght trending inspection, use other inspections settings
				GiveLinkedInspectionNotice();
			else
				EnterNominalWeight();
		}
		else
		if (((vInspectionEditing->vInspectionType == cOverfillByWeightInspection) || (vInspectionEditing->vInspectionType == cCheckWeighInspection)) && (!vInspectionEditing->vInspectionNameForWeightTrendingPointer) &&  //if using your own ROI and weight calibrations
			((!vInspectionEditing->vCheckWeighMinimumGoodWeight) || (!vInspectionEditing->vCheckWeighMaximumGoodWeight)))
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\nYou must enter the weight of the\nlight and heavy samples first";
			TempNoticeDialog.vType = cErrorMessage;
			TempNoticeDialog.DoModal();
		}
		else
		{
			CNumericEntryDialog INumericEntryDialog;  
			
			//Set dialog box data titles and number value
			INumericEntryDialog.vEditString = dtoa(vInspectionEditing->ConvertToWeight(vInspectionEditing->vDensityThresholdLower),2);

			INumericEntryDialog.vMaxValue = vMaximumThreshold;
			if (vInspectionEditing->vHasDensityUpperThreshold)
				INumericEntryDialog.vMaxValue = vInspectionEditing->vDensityThresholdUpper;

			if (vInspectionEditing->vInspectionType == cOverfillByWeightInspection)
			{
				INumericEntryDialog.vMaxValue = 30000;
				INumericEntryDialog.m_DialogTitleStaticText2 = "";
				INumericEntryDialog.m_DialogTitleStaticText1 = "Over Weight Threshold";
				INumericEntryDialog.vEditString = dtoa(vInspectionEditing->ConvertToWeight(vInspectionEditing->vDensityThresholdLower),0);
			}
			else
			if ((vInspectionEditing) &&
				(vInspectionEditing->vInspectionType == cStandardDeviationInspection))
				INumericEntryDialog.m_DialogTitleStaticText1 = "Lower Threshold of Texture";
			else
				INumericEntryDialog.m_DialogTitleStaticText1 = "Lower Threshold of Density";
			INumericEntryDialog.m_DialogTitleStaticText2 = "of pixel values in ROI";
			INumericEntryDialog.m_DialogTitleStaticText3 = "Values below this threshold are rejected, 0 = disabled";
			INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + INumericEntryDialog.vEditString;

			if ((vInspectionEditing->vInspectionType == cUnderfillByWeightInspection) || (vInspectionEditing->vInspectionType == cOverfillByWeightInspection) || (vInspectionEditing->vInspectionType == cCheckWeighInspection))
			{
				if (((vInspectionEditing->vCheckWeighMFactor != 1) || (vInspectionEditing->vCheckWeighBFactor)) && (vInspectionEditing->vCheckWeighMFactor != 0))
					INumericEntryDialog.m_UnitsString = "Value is Container Weight in " + vInspectionEditing->vCheckWeighUnits;
				else
					INumericEntryDialog.m_UnitsString = "Value is Total Pixel Weight";

				if ((vInspectionEditing->vCheckWeighMFactor != 1) || (vInspectionEditing->vCheckWeighBFactor))
				{
					INumericEntryDialog.vMaxValue = 30000;
					if (vInspectionEditing->vHasDensityUpperThreshold)
						INumericEntryDialog.vMinValue = (vInspectionEditing->ConvertToWeight(vInspectionEditing->vDensityThresholdUpper));

					INumericEntryDialog.vAllowZero = true;
					INumericEntryDialog.m_UnitsString = "Units In " + vInspectionEditing->vCheckWeighUnits;
					INumericEntryDialog.m_DialogTitleStaticText1 = "Weight Trending Threshold for Over Fill";
					INumericEntryDialog.m_DialogTitleStaticText2 = "Average of pixels in ROI";
					INumericEntryDialog.m_DialogTitleStaticText3 = "Values above this threshold cause an Alarm, 0 = disabled";
					INumericEntryDialog.vEditString = dtoa(((vInspectionEditing->vDensityThresholdLower * vInspectionEditing->vCheckWeighMFactor) + vInspectionEditing->vCheckWeighBFactor),2);
				}
			}
			else
				INumericEntryDialog.m_UnitsString = "In Pixel Intensity";

			INumericEntryDialog.vIntegerOnly = false;
			//Pass control to dialog box and display
			INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + INumericEntryDialog.vEditString;
			int nResponse = INumericEntryDialog.DoModal();
			//dialog box is now closed, if user pressed select do this
			//if user pressed cancel, do nothing
			if (nResponse == IDOK)
			{
				SetChangeMade();
				
				vInspectionEditing->SetDensityLowerThreshold((float)vInspectionEditing->ConvertWeightToBrightness(ATOF(INumericEntryDialog.vEditString)));

				vInspectionEditing->SetDensityLowerThreshold(vInspectionEditing->vDensityThresholdLower);
				DisplayDensityThresholdLower();
			}
			else 
			if (nResponse == 10)
			{
				//Main Menu button pressed
				ExitAutoSetupDialog(10);
			}
			SetChangeMade();
		}
		UpdateButtonText();
	}
	else
	if ((vMode == cCenterExposureMenu))// || (vMode == cEdgeExposureMenu))
	{
		vShowImageInColor = !vShowImageInColor;
		UpdateButtonText();
		DrawImageOnScreen();
	}
}

void CAutoSetupImageDisplayDialog::OnSubFunction4Button() 
{
	if (vInitialized)
	if (vMode == cSetSizeSettings)
	{
		if (!vRecievedButtonPress)
		{
			//ReportErrorMessage("Left Mouse Button Down IN SUBFUNCTION 4",cAction, 32000);
			if ((vMouseState == 0) || ((vMouseState > 8) && (vMouseState < 17)))	//if mouse is idle or in double click toggle on mode
			{
				vMouseState = 4;	//set mouse to on while pressed down
				int TimerResult = SetTimer(vMouseDownRepeatTimerHandle,200,NULL);
				if (!TimerResult)
					ReportErrorMessage("Error-Second Timer Failed",cEMailInspx,32000);
			}
		}
		
		if (PasswordOK(cTemporaryInspxPassword,false))
		{
			if (vGlobalCurrentProduct->vThresholdForContainerBounds > 10)
				vGlobalCurrentProduct->vThresholdForContainerBounds =
					vGlobalCurrentProduct->vThresholdForContainerBounds - 10;
			else
				vGlobalCurrentProduct->vThresholdForContainerBounds = 0;

			SetChangeMade();
			SetDlgItemText(IDC_SubFunction4DisplayA, dtoa(vGlobalCurrentProduct->vThresholdForContainerBounds,0));
			WaitThenDrawImageOnScreen();
		}
	}
	else
	if (vMode == cSetContaminantThreshold)
	{		//-10 to Threshold Bounds
		if (!vRecievedButtonPress)
		{
			//ReportErrorMessage("Left Mouse Button Down IN SUBFUNCTION 4",cAction, 32000);
			if ((vMouseState == 0) || ((vMouseState > 8) && (vMouseState < 17)))	//if mouse is idle or in double click toggle on mode
			{
				vMouseState = 4;	//set mouse to on while pressed down
				int TimerResult = SetTimer(vMouseDownRepeatTimerHandle,200,NULL);
				if (!TimerResult)
					ReportErrorMessage("Error-Second Timer Failed",cEMailInspx,32000);
			}
		}
		if (vInspectionEditing)
		{
			if (vInspectionEditing->vThreshold > 10)
				vInspectionEditing->vThreshold = vInspectionEditing->vThreshold - 10;
			else
				vInspectionEditing->vThreshold = 0;
		
			if ((vInspectionEditing) && ((vInspectionEditing->vInspectionType == cItemCountInspection) || (vInspectionEditing->vInspectionType == cSizeInspection) || (vInspectionEditing->vInspectionType == cMissingLidInspectionOnSize)))
				ClearEvaluate();
		
			vLocalSystemData->vManuallyEditedDuringEvaluate = true;
			if ((vInspectionEditing->vInspectionType != cItemCountInspection) && (vInspectionEditing->vInspectionType != cSizeInspection) && (vInspectionEditing->vInspectionType != cMissingLidInspectionOnSize))
				SetChangeMade();
			SetDlgItemText(IDC_ThresholdForContainerBounds2,dtoa(vInspectionEditing->vThreshold,0));
			WaitThenDrawImageOnScreen();
			ShowSensitivity();
		}
	}
	else
	if (vMode == cSetVoidThreshold)
	{		//-10 to Void Threshold
		if (!vRecievedButtonPress)
		{
			//ReportErrorMessage("Left Mouse Button Down IN SUBFUNCTION 4",cAction, 32000);
			if ((vMouseState == 0) || ((vMouseState > 8) && (vMouseState < 17)))	//if mouse is idle or in double click toggle on mode
			{
				vMouseState = 4;	//set mouse to on while pressed down
				int TimerResult = SetTimer(vMouseDownRepeatTimerHandle,200,NULL);
				if (!TimerResult)
					ReportErrorMessage("Error-Second Timer Failed",cEMailInspx,32000);
			}
		}
		if (vInspectionEditing)
		{
			if (vInspectionEditing->vThreshold > 10)
				vInspectionEditing->vThreshold = vInspectionEditing->vThreshold - 10;
			else
				vInspectionEditing->vThreshold = 0;
		
			if ((vInspectionEditing) && ((vInspectionEditing->vInspectionType == cItemCountInspection) || (vInspectionEditing->vInspectionType == cSizeInspection) || (vInspectionEditing->vInspectionType == cMissingLidInspectionOnSize)))
				ClearEvaluate();
		
			vLocalSystemData->vManuallyEditedDuringEvaluate = true;
			SetChangeMade();
			SetDlgItemText(IDC_ThresholdForContainerBounds2,dtoa(vInspectionEditing->vThreshold,0));
			WaitThenDrawImageOnScreen();
		}
	}
	else
	//Reference disable count button pressed
	if (vMode == cLearnMenu)
	{
		if (PasswordOK(cTemporaryInspxPassword,false))
		{
			CNumericEntryDialog INumericEntryDialog;  
			
			//Set dialog box data titles and number value
			INumericEntryDialog.vEditString = dtoa(vLocalConfigurationData->vCalibrateBodyTriggerReferenceDisableCount, 0);

			INumericEntryDialog.m_DialogTitleStaticText1 = "";
			INumericEntryDialog.m_DialogTitleStaticText2 = "Number of Images to find Average Reference position";
			INumericEntryDialog.m_DialogTitleStaticText3 = "Default Value: 20";
			INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + INumericEntryDialog.vEditString;

			INumericEntryDialog.m_UnitsString = "";
			INumericEntryDialog.vMaxValue = 20000;
			if (vLocalSystemData->vITIPCDig->vCalibrateBodyTriggerReferenceEnabled)
				INumericEntryDialog.vMaxValue = vLocalConfigurationData->vCalibrateBodyTriggerReferenceDisableCount;
			INumericEntryDialog.vIntegerOnly = true;
			//Pass control to dialog box and display
			int nResponse = INumericEntryDialog.DoModal();
			//dialog box is now closed, if user pressed select do this
			//if user pressed cancel, do nothing
			if (nResponse == IDOK)
			{
				vLocalConfigurationData->vCalibrateBodyTriggerReferenceDisableCount = _wtoi(INumericEntryDialog.vEditString);
				SetDlgItemText(IDC_SubFunction4Display,INumericEntryDialog.vEditString);
			}
			else 
			if (nResponse == 10)
			{
				//Main Menu button pressed
				ExitAutoSetupDialog(10);
			}
		}
	}
	else
	if (vMode == cSetROI)
	{  //move bottom down one pixel
		if (!vRecievedButtonPress)
		{
			//ReportErrorMessage("Left Mouse Button Down IN SUBFUNCTION 4",cAction, 32000);
			if ((vMouseState == 0) || ((vMouseState > 8) && (vMouseState < 17)))	//if mouse is idle or in double click toggle on mode
			{
				vMouseState = 4;	//set mouse to on while pressed down
				int TimerResult = SetTimer(vMouseDownRepeatTimerHandle,200,NULL);
				if (!TimerResult)
					ReportErrorMessage("Error-Second Timer Failed",cEMailInspx,32000);
			}
		}
		
		if (vInspectionEditing)
		if (DeleteCheckWeighCalibrations(true))
		{
			if ((vInspectionEditing->vInspectionType == cGlass13mmContaminantInspection) && (vInspectionEditing->vPerformGlassAlgorithm == 1) &&
				(vInspectionEditing->vROIPixelBottom <= 15))
			{ //glass in body inspection, must go to pixel 1 if below pixel 20
				vInspectionEditing->vROIPixelBottom = 1; 
				vInspectionEditing->vRemoveJarPunt = true;
				SetROILines();

				CNoticeDialog TempNoticeDialog;
				TempNoticeDialog.vNoticeText = "\n\n\nMoved Bottom ROI to bottom below Push Up.";
				TempNoticeDialog.vType = cInformationMessage;
				TempNoticeDialog.vAutoClose = 2000;
				TempNoticeDialog.DoModal();
			}
			else
			{
				if (vInspectionEditing->vROIPixelBottom)
					vInspectionEditing->vROIPixelBottom--; 
				SetROILines();
			}
		}
	}
	else
	if ((vMode == cCenterExposureMenu) || (vMode == cCenterExposureMenu))
	{
	}
	else
	if (vMode == cSetDensityThreshold)
	{
		if (vInspectionEditing)
		if (vInspectionEditing->vInspectionNameForWeightTrendingPointer) //if wieght trending inspection, use other inspections settings
			GiveLinkedInspectionNotice();
		else
		if ((vInspectionEditing->vInspectionType == cCheckWeighInspection) || (vInspectionEditing->vInspectionType == cUnderfillByWeightInspection) || (vInspectionEditing->vInspectionType == cOverfillByWeightInspection))
		if (DeleteCheckWeighCalibrations(false))
		{
			CNumericEntryDialog INumericEntryDialog;  
			
			//Set dialog box data titles and number value
			INumericEntryDialog.vEditString = dtoa(vInspectionEditing->vCheckWeighMaximumGoodWeight,2);

			INumericEntryDialog.m_DialogTitleStaticText1 = "Enter weight of the heavy sample you will send";
			INumericEntryDialog.m_DialogTitleStaticText2 = "Used to calibrate density to " + vInspectionEditing->vCheckWeighUnits;
			INumericEntryDialog.m_DialogTitleStaticText3 = "Send same sample at least 10 times";
			INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + INumericEntryDialog.vEditString;

			INumericEntryDialog.m_UnitsString = "Units in " + vInspectionEditing->vCheckWeighUnits;
			INumericEntryDialog.vAllowZero = true;
			INumericEntryDialog.vMinValue = vInspectionEditing->vCheckWeighMinimumGoodWeight;
			INumericEntryDialog.vMaxValue = 100000;
			INumericEntryDialog.vIntegerOnly = false;
			//Pass control to dialog box and display
			int nResponse = INumericEntryDialog.DoModal();
			//dialog box is now closed, if user pressed select do this
			//if user pressed cancel, do nothing
			if (nResponse == IDOK)
			{
				if ((vInspectionEditing->vInspectionType == cOverfillByWeightInspection) || (vInspectionEditing->vInspectionType == cCheckWeighInspection))
				if (vInspectionEditing->vDensityThresholdLower > 5000)
					vInspectionEditing->SetDensityLowerThreshold((float)vInspectionEditing->ConvertToWeight(vInspectionEditing->vDensityThresholdLower));

				if ((vInspectionEditing->vInspectionType == cOverfillByWeightInspection) || (vInspectionEditing->vInspectionType == cCheckWeighInspection))
				if (!vInspectionEditing->vCheckWeighMaximumGoodWeight) //if have not entered the weight before, set this to the overfill value
				{
					vInspectionEditing->SetDensityLowerThreshold((float)vInspectionEditing->ConvertWeightToBrightness(ATOF(INumericEntryDialog.vEditString)));
					DisplayDensityThresholdLower();
				}

				vInspectionEditing->vCheckWeighMaximumGoodWeight = ATOF(INumericEntryDialog.vEditString);
				if ((vInspectionEditing->vCheckWeighMaximumGoodWeight) && (vInspectionEditing->vCheckWeighMinimumGoodWeight) && 
					(vInspectionEditing->vSetupHistoryMinimumGoodAverage) && (vInspectionEditing->vSetupHistoryMaximumBadAverage) && ((vInspectionEditing->vCheckWeighMFactor != 1) || (vInspectionEditing->vCheckWeighBFactor)))
				{
						vInspectionEditing->vCheckWeighMFactor = (vInspectionEditing->vCheckWeighMinimumGoodWeight - vInspectionEditing->vCheckWeighMaximumGoodWeight) /
							(vInspectionEditing->vSetupHistoryMinimumGoodAverage - vInspectionEditing->vSetupHistoryMaximumBadAverage);
						vInspectionEditing->vCheckWeighOffset = 0;
												
						if ((vInspectionEditing->vSetupHistoryMaximumBadAverage) && (vInspectionEditing->vCheckWeighMFactor))
							vInspectionEditing->vCheckWeighBFactor = vInspectionEditing->vCheckWeighMinimumGoodWeight - (vInspectionEditing->vSetupHistoryMinimumGoodAverage * vInspectionEditing->vCheckWeighMFactor);

						if (PasswordOK(cTemporaryInspxPassword,false))
							SetStatusLine(vInspectionEditing->vName + " " + dtoa(vLocalSystemData->vDensityIndex,0) + ": " + vInspectionEditing->vCheckWeighUnits + "(new)=Density*" +
								dtoa(vInspectionEditing->vCheckWeighMFactor,6) + "+" + dtoa(vInspectionEditing->vCheckWeighBFactor,2));
						else
							SetStatusLine(vInspectionEditing->vName + " Weight calibration adjustment complete");
				}
				else
				{
					vInspectionEditing->vSetupHistoryMaximumBadCount = 0;
					vInspectionEditing->vSetupHistoryMinimumGoodCount = 0;
					vInspectionEditing->vCheckWeighBFactor = 0;
					vInspectionEditing->vCheckWeighMFactor = 1;
					vInspectionEditing->vCheckWeighOffset = 0;
				}

				if (!vInspectionEditing->vCheckWeighMaximumGoodWeight)
					vSubFunction7ButtonEnable = false;			//m_SubFunction7Button.EnableWindow(false);
				else
					vSubFunction7ButtonEnable = true;			//m_SubFunction7Button.EnableWindow(true);

				CWnd *TempWindow = CWnd::GetDlgItem(IDC_SubFunction4Button);
				if (TempWindow)
					TempWindow->Invalidate(false);
				UpdateButtonText();
				DisplayMinimumGoodMaximumBadDataValues(false);
				DisplayAutoDensityThresholdLower();
				DisplayAutoDensityThresholdUpper();
				DisplayDensityThresholdLower();
				DisplayDensityThresholdUpper();
			}
			else 
			if (nResponse == 10)
			{
				//Main Menu button pressed
				ExitAutoSetupDialog(10);
			}
		}
	}
	else
	if (vMode == cSetImageSize) //sF4
	{
		if (vInitialized)
		if (vAutosizedContainer)
		{
			if ((PasswordOK(cAdvancedSetupPassword,false)) || ((!vLocalConfigurationData->vContinuousFeedContainerRate) && (!vLocalConfigurationData->vBulkProductMode) && (!vGlobalCurrentProduct->vBulkProductMode))) //Not Continuous Feed, Not a Pipeline or password in
			{
				vGlobalCurrentProduct->vProductImageHeightTop = (float)vLocalConfigurationData->vNumberOfXRayDetectors * vLocalConfigurationData->vDetectorLength;
				vGlobalCurrentProduct->vProductImageHeightBottom = 0;

				if (PasswordOK(cTemporaryInspxPassword,false))
					SetDlgItemText(IDC_SubFunction5Display, dtoa(vGlobalCurrentProduct->vProductImageHeightTop,2) + 
					" (" + dtoa(vGlobalCurrentProduct->vProductImageHeightTop * vGlobalPixelsPerUnitInHeight,0) + ")");
				else
					SetDlgItemText(IDC_SubFunction5Display, dtoa(vGlobalCurrentProduct->vProductImageHeightTop,2));

				if ((PasswordOK(cTemporaryInspxPassword,false)) && (vGlobalCurrentProduct->vProductImageHeightBottom))
					SetDlgItemText(IDC_SubFunction6Display, dtoa(vGlobalCurrentProduct->vProductImageHeightBottom,2) + 
					" (" + dtoa(vGlobalCurrentProduct->vProductImageHeightBottom * vGlobalPixelsPerUnitInHeight,0) + ")");
				else
					SetDlgItemText(IDC_SubFunction6Display, dtoa(vGlobalCurrentProduct->vProductImageHeightBottom,2));

				if (vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac)
					ManuallySetWidth(48);
				else
				if (vLocalConfigurationData->vScanTracType == cForteScanTrac)
					ManuallySetWidth(36);
				else
					ManuallySetWidth(24);

				//set offset to zero if using offset
				if (vLocalConfigurationData->vSystemBodyTriggerToXRayBeam > 1)
				{
					double TempWhiteSpaceOnOneSideOfImage = (vGlobalCurrentProduct->vProductImageWidth / 1.6) * 0.3;
					if (TempWhiteSpaceOnOneSideOfImage > 4)
						TempWhiteSpaceOnOneSideOfImage = 4;
			
					double TempDistance = vLocalConfigurationData->vSystemBodyTriggerToXRayBeam - TempWhiteSpaceOnOneSideOfImage;  //set offset to zero

					vGlobalCurrentProduct->SetProductBodyTriggerToImageBeltPositionOffset((float)TempDistance);

					double TempTriggerToBeamOffset = -(vLocalConfigurationData->vSystemBodyTriggerToXRayBeam - vGlobalCurrentProduct->vProductBodyTriggerToImageDistanceInInches - TempWhiteSpaceOnOneSideOfImage);
					SetDlgItemText(IDC_SubFunction7Display, _T("0"));
				}
				vAutosizedContainer = false;
				UpdateButtonText();

				CNoticeDialog TempNoticeDialog;
				TempNoticeDialog.vNoticeText = "\n\n\nSend a container through to display new image";
				TempNoticeDialog.vType = cInformationMessage;
				TempNoticeDialog.DoModal();
			}
		}
		else
		if ((vLocalConfigurationData->vUseXScanDetectors) && (vLocalConfigurationData->vScanTracType != cForteScanTrac) && (vLocalConfigurationData->vScanTracType != cCaseInspectorScanTrac))
		if ((PasswordOK(cAdvancedSetupPassword,false)) || ((!vLocalConfigurationData->vContinuousFeedContainerRate) && (!vLocalConfigurationData->vBulkProductMode) && (!vGlobalCurrentProduct->vBulkProductMode))) //Not Continuous Feed, Not a Pipeline or password in
		if (!vLocalSystemData->vInAutoSetup)
		{
			YouCantChangeSettingBecauseInspecting();
		}
		else
		if (((!vLocalSystemData->vXRayOnNotice) || (vStartingUpSource) || (vRestartingXRaysWithNewSettings) ||
			((vLocalSystemData->vSystemRunMode != cRunningSystemMode) && (vLocalSystemData->vSystemRunMode != cAutoSetupRunningSystem))) &&
			(vMainWindowPointer->InterlocksOK()) && (vStartedSource))
			StartingSystemPleaseTryAgain();
		else
		{
			if (vGlobalCurrentProduct->vUsingXScanPoint4mmDetectors == 0)
			{
				vGlobalCurrentProduct->vUsingXScanPoint4mmDetectors = 1; //1 = use .4mm calibrations for pixels
				vLocalSystemData->vCalibratingXScanPoint4mmDetectors = 1;
				vLocalConfigurationData->SetPixelsPerDetector(128);

				if (vLocalConfigurationData->vEncoderDividerForPoint4mmMode == 0)
				{
					WORD TempEncoderDivider = (BYTE)vLocalConfigurationData->vEncoderDivider;
					if (vGlobalCurrentProduct->vOverScanMultiplier > 1)
					{
						CNoticeDialog TempNoticeDialog;
						TempNoticeDialog.vNoticeText = "\nWarning - Overscan is active\nThere may be a conflict with X-Scan .4 mm Mode\nThe both divide the Encoder Divider by two.";
						TempNoticeDialog.vType = cWarningMessage;
						TempNoticeDialog.DoModal();
					}
					if (TempEncoderDivider >= vGlobalCurrentProduct->vOverScanMultiplier)
						TempEncoderDivider = (BYTE)(((double)TempEncoderDivider / vGlobalCurrentProduct->vOverScanMultiplier) + .5);
					if (TempEncoderDivider % 2 != 0)// % is mod function  //if not divisible by 2, then can't do
					{
						CNoticeDialog TempNoticeDialog;
						TempNoticeDialog.vNoticeText = "\nX-Scan .4 mm Mode Detectors can not scan\nat fast rate because the\nEncoder Divider factor is not divisible by two.\nYou must re-calibrate the encoder with the divider divisible by 2.";
						TempNoticeDialog.vType = cErrorMessage;
						TempNoticeDialog.DoModal();
					}
				}
			}
			else
			{
				vGlobalCurrentProduct->vUsingXScanPoint4mmDetectors = 0; //0 = use .8mm calibrations for pixels
				vLocalSystemData->vCalibratingXScanPoint4mmDetectors = 0;
				vLocalConfigurationData->SetPixelsPerDetector(64);
			}
			vLocalConfigurationData->SetDetectorLength(vLocalConfigurationData->vDetectorLength);

			vLocalSystemData->vChangedSizeOfProduct = true;
			vLocalSystemData->vChangedSizeOfProductCheckDensityROI = 1;
			CheckDensityCompensationInspection();
			vGlobalCurrentProduct->CheckAllROIs(vLocalSystemData->vITIPCDig->vProductAverageImage);
			vLocalSystemData->vITIPCDig->RenameLearnFilesToBackup(vGlobalCurrentProduct);
			vLocalSystemData->vITIPCDig->DeleteLearnImages();
			vLocalSystemData->vITIPCDig->DeleteLearnFiles(vGlobalCurrentProduct);
			ClearDensityFeedBackLoop();
			ChangedProductSettingsSoReload();
			WaitDisplayLocationEdge();
			CheckPurpleLineHeight();
			UpdateButtonText();

			if (PasswordOK(cTemporaryInspxPassword,false))
				SetDlgItemText(IDC_SubFunction5Display, dtoa(vGlobalCurrentProduct->vProductImageHeightTop,2) + 
				" (" + dtoa(vGlobalCurrentProduct->vProductImageHeightTop * vGlobalPixelsPerUnitInHeight,0) + ")");
			else
				SetDlgItemText(IDC_SubFunction5Display, dtoa(vGlobalCurrentProduct->vProductImageHeightTop,2));

			if ((PasswordOK(cTemporaryInspxPassword,false)) && (vGlobalCurrentProduct->vProductImageHeightBottom))
				SetDlgItemText(IDC_SubFunction6Display, dtoa(vGlobalCurrentProduct->vProductImageHeightBottom,2) + " (" + dtoa(vGlobalCurrentProduct->vProductImageHeightBottom * vGlobalPixelsPerUnitInHeight,0) + ")");
			else
				SetDlgItemText(IDC_SubFunction6Display, dtoa(vGlobalCurrentProduct->vProductImageHeightBottom,2));
						
			if (PasswordOK(cTemporaryInspxPassword,false))
				SetDlgItemText(IDC_SubFunction3Display, dtoa(vGlobalCurrentProduct->vProductLockOutWidth,2) + " (" + dtoa(vGlobalCurrentProduct->vLockoutPosition, 0) + ")");
			else
				SetDlgItemText(IDC_SubFunction3Display, dtoa(vGlobalCurrentProduct->vProductLockOutWidth,2));
		}
	}
}

void CAutoSetupImageDisplayDialog::OnSubFunction5Button() 
{
	if (vInitialized)
	if (vMode == CMultiLanePositionAdjustment)
	{
		if (vGlobalCurrentProduct->vMultiLaneStartPixel[vMultiImageLaneEditing] + vGlobalCurrentProduct->vMultiLaneWidthInPixels[vMultiImageLaneEditing] < vMaximumLaneWidth)
		{
			vGlobalCurrentProduct->vMultiLaneStartPixel[vMultiImageLaneEditing]++;
			UpdateButtonText();
			WaitThenDrawImageOnScreen();
		}
		else
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\n\nLane at edge of image";
			TempNoticeDialog.vType = cErrorMessage;
			TempNoticeDialog.DoModal();
		}
	}
	else
	if (vMode == cSetSizeSettings)
	{
		if (!vRecievedButtonPress)
		{
			//ReportErrorMessage("Left Mouse Button Down IN SUBFUNCTION 5",cAction, 32000);
			if ((vMouseState == 0) || ((vMouseState > 8) && (vMouseState < 17)))	//if mouse is idle or in double click toggle on mode
			{
				vMouseState = 5;	//set mouse to on while pressed down
				int TimerResult = SetTimer(vMouseDownRepeatTimerHandle,200,NULL);
				if (!TimerResult)
					ReportErrorMessage("Error-Second Timer Failed",cEMailInspx,32000);
			}
		}
		
		if (vAdjustingBottomEdgeFinding)
		{
			if ((vGlobalCurrentProduct->vBottomLocationLengthPixel + vGlobalCurrentProduct->vBottomLocationLeftPixel) < vLocalSystemData->vITIPCDig->vOriginalBufferSizeY - 1)
			{
				vChangedPurpleLine = true;
				vGlobalCurrentProduct->vBottomLocationLengthPixel++;
				vGlobalCurrentProduct->SetBottomLocationLength((float)(vGlobalCurrentProduct->vBottomLocationLengthPixel / vGlobalPixelsPerUnit));

				SetChangeMade();
				UpdateButtonText();

				vNeedToSetContainerThresholdBounds = 1;

				//9/10/2009
				if (!vHaveAnImage)
				if (!vGaveSendAContainerNotice)
				{
					vMouseState = 0;
					vGaveSendAContainerNotice = true;
					CNoticeDialog TempNoticeDialog;
					TempNoticeDialog.vNoticeText = "\n\n\nSend another container through\nto update the screen";
					TempNoticeDialog.vType = cInformationMessage;
					TempNoticeDialog.DoModal();
				}
				CalculateLearnedReferenceValues();

				WaitThenDrawImageOnScreen();
				WaitDisplayLocationEdge();
			}
		}
		else
		{
			if ((vGlobalCurrentProduct->vEdgeLocationHeightPixel + vGlobalCurrentProduct->vEdgeLocationBottomPixel) < vLocalSystemData->vITIPCDig->vOriginalBufferSizeX - 1)
			{
				vChangedPurpleLine = true;
				vGlobalCurrentProduct->vEdgeLocationHeightPixel++;
				vGlobalCurrentProduct->SetEdgeLocationHeight((float)(vGlobalCurrentProduct->vEdgeLocationHeightPixel / vGlobalPixelsPerUnitInHeight));

				SetChangeMade();
				UpdateButtonText();

				vNeedToSetContainerThresholdBounds = 1;

				//9/10/2009
				if (!vHaveAnImage)
				if (!vGaveSendAContainerNotice)
				{
					vMouseState = 0;
					vGaveSendAContainerNotice = true;
					CNoticeDialog TempNoticeDialog;
					TempNoticeDialog.vNoticeText = "\n\n\nSend another container through\nto update the screen";
					TempNoticeDialog.vType = cInformationMessage;
					TempNoticeDialog.DoModal();
				}
				CalculateLearnedReferenceValues();

				WaitThenDrawImageOnScreen();
				WaitDisplayLocationEdge();
			}
		}
	}
	else
	if (vMode == cSetBackground)
	{
		CYesNoDialog TempYesNoDialog;
		TempYesNoDialog.vNoticeText =  _T("\n\nNo containers may go through ScanTrac\nwhile sampling background.");
		TempYesNoDialog.vYesButtonText = "Sample Background";
		TempYesNoDialog.vNoButtonText = "Cancel";
		TempYesNoDialog.vQuestionType = cConfirmQuestion;
		int TempResult = TempYesNoDialog.DoModal();
		if (TempResult == IDOK)
		{
			TakeBackgroundImageSample();
		}
	}
	else
		/*
	if (vMode == cEdgeExposureMenu)
	{		//Make Edges A Lot Brighter
		if (vLocalSystemData->vInAutoSetup)
		{
			if (vGlobalCurrentProduct->vXRayIntegrationTimeAtEdge < vGlobalCurrentProduct->vXRayIntegrationTime)
			{
				WORD TempTenPercentStep = (vGlobalCurrentProduct->vXRayIntegrationTime - 205) / 10;
				if (TempTenPercentStep < 1)
					TempTenPercentStep = 1;
				if (vGlobalCurrentProduct->vXRayIntegrationTimeAtEdge + TempTenPercentStep < vGlobalCurrentProduct->vXRayIntegrationTime)
					vGlobalCurrentProduct->vXRayIntegrationTimeAtEdge = vGlobalCurrentProduct->vXRayIntegrationTimeAtEdge + TempTenPercentStep;
				else
					vGlobalCurrentProduct->vXRayIntegrationTimeAtEdge = vGlobalCurrentProduct->vXRayIntegrationTime;

				DisplayEdgeExposureTimePercent();

				//variable Integration Time temp can't do as will mess something up in the FPGA or uC, so reload all settings
				vMainWindowPointer->SendProductIntegrationTime(vGlobalCurrentProduct->vXRayIntegrationTime, 
					vGlobalCurrentProduct->vXRayIntegrationTimeAtEdge);

				//vMainWindowPointer->StopRunning(true);

				//int TimerResult = SetTimer(vStartSetupTimerHandle,1000,NULL);
				//if (!TimerResult)
				//	ReportErrorMessage("Error-Start Timer Failed",cEMailInspx,32000);
			}
			else
				GiveMessageAlreadyAtThisValue(2); //2 = maximum value
		}
		else
			YouCantChangeSettingBecauseInspecting();
	}
	else
	*/
	if (vMode == cCenterExposureMenu)
	{		//Edit X-Ray Voltage
		bool TempByPassDeletingLearn = false;
		if ((vLocalSystemData->vInAutoSetup) || (PasswordOK(cSuperInspxPassword,false)))
		{
			if (PasswordOK(cSuperInspxPassword,false))
			{
				if (vGlobalCurrentProduct->ProductHasImageFiles(vGlobalCurrentDirectory))
				{
					CYesNoDialog TempYesNoDialog;
					TempYesNoDialog.vNoticeText =  _T("\nThis product has been Learned.\nIf you change this setting it may effect the Learn.\nDo you want to keep it regardless?");
					TempYesNoDialog.vYesButtonText = "Keep Learn";
					TempYesNoDialog.vQuestionType = cConfirmQuestion;
					int TempResult = TempYesNoDialog.DoModal();
					if (TempResult == IDOK)
						TempByPassDeletingLearn = true;
				}
			}
			float TempOriginalVoltage = vGlobalCurrentProduct->vXRaySourceVoltage;
			CString TempVoltageString = dtoa(vGlobalCurrentProduct->vXRaySourceVoltage * 6,1); 
			//convert from 0 to 10 volt to 0 to 60 KV
			CNumericEntryDialog INumericEntryDialog;  
				
			//Set dialog box data titles and number value
			INumericEntryDialog.vEditString = TempVoltageString;

			CString TempText("");
			TempText.LoadString(IDS_XRaySourceVoltage);

			TempText.LoadString(IDS_Product);
			INumericEntryDialog.m_DialogTitleStaticText1 = "X-Ray Source Voltage for " + TempText + ": " + vProductEditingName;
			TempText.LoadString(IDS_OriginalValue);
			INumericEntryDialog.m_DialogTitleStaticText4 = TempText + ": " + TempVoltageString;

			TempText.LoadString(IDS_MeasuredInVolts);
			INumericEntryDialog.m_UnitsString = TempText;
			INumericEntryDialog.vMinValue = cMinimumXRayVoltage;
			INumericEntryDialog.vAllowZero = true;
			INumericEntryDialog.vMaxValue = vLocalConfigurationData->vMaximumVoltage;
			INumericEntryDialog.m_DialogTitleStaticText2 = "Maximum Voltage: " + dtoa(vLocalConfigurationData->vMaximumVoltage,2);
			INumericEntryDialog.m_DialogTitleStaticText3 = "";

			INumericEntryDialog.vIntegerOnly = false;
			//Pass control to dialog box and display
			int nResponse = INumericEntryDialog.DoModal();
			//dialog box is now closed, if user pressed select do this
			//if user pressed cancel, do nothing
			if (nResponse == IDOK)
			{
				TempVoltageString = INumericEntryDialog.vEditString;
				vNeedToSetContainerThresholdBounds = 2;

				SetChangeMade();

				if ((!TempByPassDeletingLearn) &&	(vLocalSystemData->vInAutoSetup))
				{
					vLocalSystemData->vITIPCDig->RenameLearnFilesToBackup(vGlobalCurrentProduct);
					vLocalSystemData->vITIPCDig->DeleteLearnImages();
					vLocalSystemData->vITIPCDig->DeleteLearnFiles(vGlobalCurrentProduct);
					ClearDensityFeedBackLoop();
					//LoadBackGroundIfNeeded();
				}

				vGlobalCurrentProduct->vXRaySourceVoltage = (float)ATOF(TempVoltageString);
				SetDlgItemText(IDC_SubFunction5Display,dtoa(vGlobalCurrentProduct->vXRaySourceVoltage,2));
				double TempMaximumCurrentV = GetMaximumCurrentFromVoltage(vGlobalCurrentProduct->vXRaySourceVoltage);

				double TempMaximumCurrentP = GetMaximumCurrentFromPower(vGlobalCurrentProduct->vXRaySourceVoltage, 
					vLocalConfigurationData->vMaximumPower);

				double TempMaximumCurrent = TempMaximumCurrentV;
				if (TempMaximumCurrentP < TempMaximumCurrentV)
					TempMaximumCurrent = TempMaximumCurrentP;

				if (TempMaximumCurrent > vLocalConfigurationData->vMaximumCurrent)
					TempMaximumCurrent = vLocalConfigurationData->vMaximumCurrent;

				if (vGlobalCurrentProduct->vXRaySourceVoltage == 0)
				if (vGlobalCurrentProduct->vXRaySourceCurrent)
				{ // lower current to zero since voltage is zero
					vGlobalCurrentProduct->vXRaySourceCurrent = 0;
					vLocalSystemData->vRampXRayCurrentAmount = vGlobalCurrentProduct->vXRaySourceCurrent;
					vMainWindowPointer->SendSourceCurrentToUController(vGlobalCurrentProduct->vXRaySourceCurrent);
					SetDlgItemText(IDC_SubFunction6Display,dtoa(vGlobalCurrentProduct->vXRaySourceCurrent, 2));
					CNoticeDialog TempNoticeDialog;
					TempNoticeDialog.vNoticeText = "\n\n\nReduced current to 0 since Voltage is 0";
					TempNoticeDialog.vType = cInformationMessage;
					TempNoticeDialog.DoModal();
					Sleep(100);
				}
				if (vGlobalCurrentProduct->vXRaySourceCurrent > TempMaximumCurrent)
				{ // lower current first with old voltage
					CNoticeDialog TempNoticeDialog;
					TempNoticeDialog.vNoticeText = "\n\n\nReduced current to maximum of: " + dtoa(TempMaximumCurrent, 2) + "\ndue to X-Ray Power Limit at new Voltage";
					vGlobalCurrentProduct->vXRaySourceCurrent = (float)TempMaximumCurrent;
					vLocalSystemData->vRampXRayCurrentAmount = vGlobalCurrentProduct->vXRaySourceCurrent;
					vMainWindowPointer->SendSourceCurrentToUController(TempMaximumCurrent);
					SetDlgItemText(IDC_SubFunction6Display,dtoa(vGlobalCurrentProduct->vXRaySourceCurrent, 2));
					TempNoticeDialog.vType = cInformationMessage;
					TempNoticeDialog.DoModal();
					Sleep(100);
				}
				vGlobalCurrentProduct->vXRaySourceVoltage = vGlobalCurrentProduct->vXRaySourceVoltage / 6; 
				//on a 50 watt system, set current to maximum when voltage is changed
				if (vLocalConfigurationData->vHVPSType == cMNXHVPS)  //50 watt tube
				{
					double TempMaximumCurrent = GetMaximumCurrentFromVoltage(vGlobalCurrentProduct->vXRaySourceVoltage * 6);
					if (TempMaximumCurrent > vLocalConfigurationData->vMaximumCurrent)
						TempMaximumCurrent = vLocalConfigurationData->vMaximumCurrent;

					double TempMaximumCurrentP = GetMaximumCurrentFromPower(vGlobalCurrentProduct->vXRaySourceVoltage, vLocalConfigurationData->vMaximumPower);

					if (TempMaximumCurrent > TempMaximumCurrentP)
						TempMaximumCurrent = TempMaximumCurrentP;

					vGlobalCurrentProduct->vXRaySourceCurrent = (float)TempMaximumCurrent;
				}
					
				//store in 0 to 10 volt format from 0 to 60 KV
				vLocalSystemData->vRampXRayCurrentAmount = vGlobalCurrentProduct->vXRaySourceCurrent;
				vLocalSystemData->vRampXRayVoltageAmount = vGlobalCurrentProduct->vXRaySourceVoltage;
				vMainWindowPointer->SendSourceSettingsToUController(vLocalSystemData->vRampXRayVoltageAmount, vLocalSystemData->vRampXRayCurrentAmount);
			}
			else 
			if (nResponse == 10)
			{
				//Main Menu button pressed
				ExitAutoSetupDialog(10);
			}
		}
		else
		{
			CNoticeDialog TempNoticeDialog;
			CString TempText;
			TempText.LoadString(IDS_Youcannotchangeimageexposurewhileinspecting);
			//TempNoticeDialog.vNoticeText = "\n\nYou can not change image exposure while inspecting.\nYou must stop running on the Main Menu,\nthen Edit the Product Exposure";
			TempNoticeDialog.vNoticeText = TempText;
			TempNoticeDialog.vType = cErrorMessage;
			TempNoticeDialog.DoModal();
		}
	}
	else
	// Start or stop auto setup aquistion
	if (vMode == cLearnMenu)
	{
		vLocalSystemData->vImprovingLearn = false;
		CString TempText;
		//doing quick learn, so abort
		if (vLocalSystemData->vTakingQuickLearnImages)
		{
			vOldTakingQuickLearnImages = false;
			vLocalSystemData->vTakingQuickLearnImages = false;
			//vOldTakingQuickLearnImages = false;
			vLocalSystemData->FreeQuickLearnImages();
			TempText.LoadString(IDS_PressStartAutoLearnButtonWhenImagesLookGood);
			SetStatusLine(TempText);
			ReportErrorMessage("Abort Quick Learn", cUserChanges,0);
		}
		else
		if (vOldAdjustingBrightnessBeforeLearning)
		{  //abort adjust brightness before relearn
			vLocalSystemData->vAdjustingBrightnessBeforeLearning = false;
			vOldAdjustingBrightnessBeforeLearning = false;  //abort to set false so will not start learning
			vMainWindowPointer->vOldAdjustingBrightnessBeforeLearning = false;//abort to set false so will not start learning

			TempText.LoadString(IDS_PressStartAutoLearnButtonWhenImagesLookGood);
			SetStatusLine(TempText);
			UpdateButtonText();
		}
		else
		if ((vOldAverageEnabled) || (vOldStructureEnabled) || 
			(vOldCalibrateBodyTriggerReferenceEnabled))
		//running auto setup, so abort
		{
			CString TempText1;
			bool TempMustReloadImages = false;
			bool TempAbandonLearn = true;
			if (PasswordOK(cTemporaryInspxPassword,false))
			{
				CYesNoDialog TempYesNoDialog;
				TempYesNoDialog.vNoticeText =  _T("\n\nThrow Away or Keep Data Leaned so far?");
				TempYesNoDialog.vQuestionType = cConfirmQuestion;
				TempYesNoDialog.vNoButtonText = "Keep Data";
				TempYesNoDialog.vYesButtonText = "Throw Away";
				int TempResult = TempYesNoDialog.DoModal();
				if (TempResult != IDOK)
					TempAbandonLearn = false;
			}
			if (PasswordOK(cTemporaryInspxPassword,false))
			{
				m_Width2.ShowWindow(SW_HIDE);
				m_Reference.ShowWindow(SW_HIDE);
			}
			if (TempAbandonLearn)
			{
				vLocalSystemData->vITIPCDig->RestoreLearnFilesFromBackups(vGlobalCurrentProduct);
				vOldCalibrateBodyTriggerReferenceEnabled = false;
				vLocalSystemData->vITIPCDig->vCalibrateBodyTriggerReferenceEnabled = false;
				vMainWindowPointer->vOldAverageEnabled = false;
				vOldAverageEnabled = false;
				vLocalSystemData->vITIPCDig->vAverageEnabled = false;
				if (vLocalSystemData->vITIPCDig->vStructureEnabled)
					TempMustReloadImages = true;
				vOldStructureEnabled = false;
				vLocalSystemData->vITIPCDig->vStructureEnabled = false;
				vOldCollectingDensityAverage = false;
				vCollectingBadDensityAverage = false;
				TempText.LoadString(IDS_PressStartAutoLearnButtonWhenImagesLookGood);
				SetStatusLine(TempText);

				if (vLocalSystemData->vITIPCDig->vAverageImage)
				{
					free(vLocalSystemData->vITIPCDig->vAverageImage);
					vLocalSystemData->vITIPCDig->vAverageImage = NULL;
				}
				if (vLocalSystemData->vITIPCDig->vStructureImage)
				{
					delete vLocalSystemData->vITIPCDig->vStructureImage;
					vLocalSystemData->vITIPCDig->vStructureImage = NULL;
				}

				if (PasswordOK(cTemporaryInspxPassword,false))
				{
					vShowHistogram = false;
					vShowStructure = false;
					vShowVoidStructure = false;
					//vShowGlassStructure = false;
					vShowContaminant2Structure = false;
					vShowAverage = false;
				}
				UpdateButtonText();
				if (TempMustReloadImages)
					ChangedProductSettingsSoReload();
				ReportErrorMessage("Abort Learn", cUserChanges,0);
			}
			else
			{
				if (vLocalSystemData->vITIPCDig->vAverageEnabled)
				{
					vLocalSystemData->vITIPCDig->vAverageEnabled = false;
					if (vLocalSystemData->vITIPCDig->vContainerCollectionLock.Lock())
					{  //end of collecting structure, Save structure image
						if (vLocalSystemData->vLogFile.vLogSerialData)
						{
							vLocalSystemData->vLogFile.WriteToLogFile("Make Product Average5, Average Count: " + dtoa(vLocalSystemData->vITIPCDig->vAverageCount,0),cDebugMessage);
						}
					
						if (vLocalSystemData->vITIPCDig->MakeProductAverageImage())
						{
							vLocalSystemData->vITIPCDig->SaveAverageImageToFile(vGlobalCurrentProduct);
							vGlobalCurrentProduct->vSetupHistoryAverageDensity = 0;

							if (vGlobalCurrentProduct->vHistogramThreshold > 0)
								vLocalSystemData->vITIPCDig->SaveProcess4ImageData(vGlobalCurrentProduct);

							vGlobalCurrentProduct->vSetupHistoryAverageCount = vLocalConfigurationData->vAverageDisableCount;
							vLocalSystemData->vITIPCDig->CalculateAverageDriftDensity(vGlobalCurrentProduct);
							vLocalSystemData->vIndexForDriftDensityOfLastNImages = 0;
							if (vGlobalCurrentProduct->vSetupHistoryAverageDensity > 255)
								vGlobalCurrentProduct->vSetupHistoryAverageDensity = 0;
							vGlobalCurrentProduct->vSetupHistoryContainerTriggerStandardDeviation = vLocalSystemData->vBodyTriggerWidthStandardDeviation;
						}
						vLocalSystemData->vITIPCDig->vContainerCollectionLock.Unlock();		
					}
				}
				if (vLocalSystemData->vITIPCDig->vStructureEnabled)
				{
					//first save the average, then structure
					if (!vLocalSystemData->vInAutoSetup)
					if (vLocalSystemData->vITIPCDig->vContainerCollectionLock.Lock())
					{  //end of collecting structure, Save structure image
						if (vLocalSystemData->vITIPCDig->MakeProductAverageImage())
						{
							vLocalSystemData->vITIPCDig->SaveAverageImageToFile(vGlobalCurrentProduct);
							vGlobalCurrentProduct->vSetupHistoryAverageDensity = 0;

							if (vGlobalCurrentProduct->vHistogramThreshold > 0)
								vLocalSystemData->vITIPCDig->SaveProcess4ImageData(vGlobalCurrentProduct);

							vGlobalCurrentProduct->vSetupHistoryAverageCount = vLocalConfigurationData->vAverageDisableCount;
							vLocalSystemData->vITIPCDig->CalculateAverageDriftDensity(vGlobalCurrentProduct);
							vLocalSystemData->vIndexForDriftDensityOfLastNImages = 0;
							if (vGlobalCurrentProduct->vSetupHistoryAverageDensity > 255)
								vGlobalCurrentProduct->vSetupHistoryAverageDensity = 0;
							vGlobalCurrentProduct->vSetupHistoryContainerTriggerStandardDeviation = vLocalSystemData->vBodyTriggerWidthStandardDeviation;

							vLocalSystemData->vITIPCDig->vLearnedAverageDensity = vGlobalCurrentProduct->vSetupHistoryAverageDensity;
						}
						vLocalSystemData->vITIPCDig->vContainerCollectionLock.Unlock();		
					}
					vLocalSystemData->vITIPCDig->vStructureEnabled = false;
					if (vLocalSystemData->vITIPCDig->vContainerCollectionLock.Lock())
					{  //end of collecting structure, Save structure image
						//remove normal learn dilate from here as put into reading of learn
						//if ((vLocalConfigurationData->vNormalLearnDilateTimes) && (vLocalConfigurationData->vNormalLearnDilateTimes < 20))
						//{
						//	Image *TempScratchImage = NULL;
						//	if (ThereIsEnoughMemory(vLocalSystemData->vITIPCDig->vOriginalBufferSize + sizeof(Image), "TempScratchImage"))
						//		TempScratchImage = im_create(IM_BYTE, vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vAquisitionSizeY);
						//	if ((TempScratchImage) && (TempScratchImage->pix))
						//	{
						//		CopyMemory(TempScratchImage->pix, vLocalSystemData->vITIPCDig->vContaminant2StructureImage->vGreyImage, vLocalSystemData->vITIPCDig->vOriginalBufferSize);

						//		//mvt_erode(TempScratchImage, TempScratchImage2);
						//		mvt_erode_rect(TempScratchImage, vLocalConfigurationData->vNormalLearnDilateTimes, vLocalConfigurationData->vNormalLearnDilateTimes, EDGE_IGNORE);

						//		CopyMemory(vLocalSystemData->vITIPCDig->vContaminant2StructureImage->vGreyImage, TempScratchImage->pix, vLocalSystemData->vITIPCDig->vOriginalBufferSize);

						//		CopyMemory(TempScratchImage->pix, vLocalSystemData->vITIPCDig->vStructureImage->vGreyImage, vLocalSystemData->vITIPCDig->vOriginalBufferSize);

						//		//mvt_erode(TempScratchImage, TempScratchImage2);
						//		mvt_erode_rect(TempScratchImage, vLocalConfigurationData->vNormalLearnDilateTimes, vLocalConfigurationData->vNormalLearnDilateTimes, EDGE_IGNORE);

						//		CopyMemory(vLocalSystemData->vITIPCDig->vStructureImage->vGreyImage, TempScratchImage->pix, vLocalSystemData->vITIPCDig->vOriginalBufferSize);
						//		im_delete(TempScratchImage);
						//	}
						//}

						vLocalSystemData->vITIPCDig->SaveContaminant2StructureImageToFile(vGlobalCurrentProduct,vLocalSystemData->vITIPCDig->vContaminant2StructureImage, true);
						ReportErrorMessage("Save Standard Learn in Setup For Product: " + vLocalSystemData->vCurrentProductName, cUserChanges,0);

						DWORD TempStructureCount = vLocalSystemData->vITIPCDig->vStructureCount;
						if (vLocalSystemData->vITIPCDig->MakeProductStructureImage())
						{
							if (vLocalConfigurationData->vDriftCompensationEnabled)
								vLocalSystemData->vKeepDensityFeedbackLoopLocked = true;
							vGlobalCurrentProduct->vSetupHistoryStructureCount = TempStructureCount;
							vGlobalCurrentProduct->vNeedToRelearn = 0;
							vLocalSystemData->vITIPCDig->SaveStructureImageToFile(vGlobalCurrentProduct,true);
						}

						if (vLocalSystemData->vITIPCDig->MakeVoidProductStructureImage())
						{
							vLocalSystemData->vITIPCDig->SaveVoidStructureImageToFile(vGlobalCurrentProduct,true);
						}
						vLocalSystemData->vITIPCDig->LoadStructureImagesFromFiles(false, false);
						vLocalSystemData->vITIPCDig->vContainerCollectionLock.Unlock();
						vLocalSystemData->vITIPCDig->ClearImproveLearn(false);
					}
				}
				vOldCalibrateBodyTriggerReferenceEnabled = false;
				vLocalSystemData->vITIPCDig->vCalibrateBodyTriggerReferenceEnabled = false;
				vMainWindowPointer->vOldAverageEnabled = false;
				vOldAverageEnabled = false;
				vLocalSystemData->vITIPCDig->vAverageEnabled = false;
				vOldStructureEnabled = false;
				vLocalSystemData->vITIPCDig->vStructureEnabled = false;
				vOldCollectingDensityAverage = false;
				vCollectingBadDensityAverage = false;
				TempText.LoadString(IDS_PressStartAutoLearnButtonWhenImagesLookGood);
				SetStatusLine(TempText);
				if (vLocalSystemData->vITIPCDig->vAverageImage)
				{
					free(vLocalSystemData->vITIPCDig->vAverageImage);
					vLocalSystemData->vITIPCDig->vAverageImage = NULL;
				}
				if (vLocalSystemData->vITIPCDig->vStructureImage)
				{
					delete vLocalSystemData->vITIPCDig->vStructureImage;
					vLocalSystemData->vITIPCDig->vStructureImage = NULL;
				}
				if (PasswordOK(cTemporaryInspxPassword,false))
				{
					vShowHistogram = false;
					vShowStructure = false;
					vShowVoidStructure = false;
					//vShowGlassStructure = false;
					vShowContaminant2Structure = false;
					vShowAverage = false;
				}
				UpdateButtonText();
			}
			vLocalSystemData->vITIPCDig->CalculateAverageDriftDensity(vGlobalCurrentProduct);
		}
		else
		{
			CString TempText1;
			vLocalSystemData->vIncludeRejectsInAverageAndStructure = true;
			//if (!vLocalSystemData->vInAutoSetup)
			//{
			//	CYesNoDialog TempYesNoDialog;
			//	TempText.LoadString(IDS_IncludeRejectsinLearnedImage);//"\n\n\nInclude Rejects in Learned Image?"
			//	TempYesNoDialog.vNoticeText = TempText;
			//	TempYesNoDialog.vQuestionType = cConfirmQuestion;
			//	TempText1.LoadString(IDS_IncludeRejects);
			//	TempYesNoDialog.vYesButtonText = TempText1;
			//	TempText1.LoadString(IDS_ExcludeRejects);
			//	TempYesNoDialog.vNoButtonText = TempText1;
			//	int TempResult = TempYesNoDialog.DoModal();
			//	if (TempResult != IDOK)
			//		vLocalSystemData->vIncludeRejectsInAverageAndStructure = false;
			//}

			bool TempOKToRelearn = true;
			if (!vLocalSystemData->vInAutoSetup)
			if ((vLocalSystemData->vCurrentBrightnessAdjustment > 15) || (vLocalSystemData->vCurrentBrightnessAdjustment < -15)) //8/23/2010 was 20
			//if (vLocalSystemData->vKeepDensityFeedbackLoopLocked)
			if (vLocalConfigurationData->vDriftCompensationEnabled)
			{
				if (vLocalSystemData->vInAutoSetup)
					ReportErrorMessage("start Learn in Auto Setup",cUserChanges,0);
				else
				if (vLocalSystemData->vIncludeRejectsInAverageAndStructure)
					ReportErrorMessage("Setup start Learn while running Including Rejects",cUserChanges,0);
				else
					ReportErrorMessage("Setup start Learn while running Excluding Rejects",cUserChanges,0);

				vGlobalCurrentProduct->vXRaySourceCurrent = (float)(vLocalSystemData->vRampXRayCurrentAmount);
				vGlobalCurrentProduct->vXRaySourceVoltage = (float)(vLocalSystemData->vRampXRayVoltageAmount);

				vLocalSystemData->vAdjustingBrightnessBeforeLearning = true;
				vOldAdjustingBrightnessBeforeLearning = true;
				TempOKToRelearn = false;
				if (vLocalSystemData->vITIPCDig->vDriftCompensationLocked)
				{
					ReportErrorMessage(" Adjusting for Relearning, Send Containers",cUserAction,0);
					SetDlgItemText(IDC_StatusLine, _T(" Adjusting for Relearning, Send Containers"));
				}
				else
					SetStatusLine(" Waiting for lock, Send Good Containers...");
			}

			if (!vLocalSystemData->vInAutoSetup)
				vLocalSystemData->vManualReLearnWhileInspecting = true;
			else
				vLocalSystemData->vManualReLearnWhileInspecting = false;

			if (TempOKToRelearn)
				StartLearning();
		}
		UpdateButtonText();
	}
	else
	if (vMode == cSetImageSize)//subfunction5button
	{
		bool TempValueChanged = false;
		float TempTop = 0;
		if ((PasswordOK(cAdvancedSetupPassword,false)) || ((!vLocalConfigurationData->vContinuousFeedContainerRate) && (!vLocalConfigurationData->vBulkProductMode) && (!vGlobalCurrentProduct->vBulkProductMode))) //Not Continuous Feed, Not a Pipeline or password in
		if (!vLocalSystemData->vInAutoSetup)
		{
			YouCantChangeSettingBecauseInspecting();
		}
		else
		if (((!vLocalSystemData->vXRayOnNotice) || (vStartingUpSource) || (vRestartingXRaysWithNewSettings) ||
			((vLocalSystemData->vSystemRunMode != cRunningSystemMode) && (vLocalSystemData->vSystemRunMode != cAutoSetupRunningSystem))) &&
			(vMainWindowPointer->InterlocksOK()) && (vStartedSource))
			StartingSystemPleaseTryAgain();
		else
		{
			if (vGlobalCurrentProduct->vMultiLaneNumberOfLanes)
			{
				FinishSizeMenu(false, false);
				StartMultiLanePositionMenu();
			}
			else
			{
				//Top, or Height Button Was Pressed
				CNumericEntryDialog INumericEntryDialog;  
			
				//Set dialog box data titles and number value
				INumericEntryDialog.vEditString = dtoa(vGlobalCurrentProduct->vProductImageHeightTop);

				INumericEntryDialog.m_DialogTitleStaticText1 = "";
				CString TempText;
				if (vGlobalScanTracType == cBriosoScanTrac)
					TempText = "Right of X-Ray Image";
				else
					TempText = vGlobalTopString + " of X-Ray Image";

				INumericEntryDialog.m_DialogTitleStaticText2 = TempText;
				TempText.LoadString(IDS_Product);
				INumericEntryDialog.m_DialogTitleStaticText3 = TempText + ": " + vProductEditingName;
				TempText.LoadString(IDS_OriginalValue);
				INumericEntryDialog.m_DialogTitleStaticText4 = TempText + ": " + INumericEntryDialog.vEditString;

				TempText.LoadString(IDS_MeasuredIn);
				INumericEntryDialog.m_UnitsString = TempText + " " + vLocalConfigurationData->vUnitsString;

				double TempImageHeight = vLocalConfigurationData->vNumberOfXRayDetectors * vLocalConfigurationData->vDetectorLength;

				double TempImageMargin = 1;

				if (vGlobalCurrentProduct->vMultiLaneNumberOfLanes)
				{
					double TempLaneWidthSuggestion = TempImageHeight / vGlobalCurrentProduct->vMultiLaneNumberOfLanes;
					INumericEntryDialog.m_DialogTitleStaticText1 = "Multi Lane (" + dtoa(vGlobalCurrentProduct->vMultiLaneNumberOfLanes, 0) + ") Suggest: " + dtoa(TempLaneWidthSuggestion, 2);
				}

				INumericEntryDialog.vMaxValue = TempImageHeight;
				INumericEntryDialog.vMinValue = vGlobalCurrentProduct->vProductImageHeightBottom + TempImageMargin;
				INumericEntryDialog.vAllowZero = false;
				INumericEntryDialog.vIntegerOnly = false;
				//Pass control to dialog box and display
				int nResponse = INumericEntryDialog.DoModal();
				//dialog box is now closed, if user pressed select do this
				//if user pressed cancel, do nothing
				if (nResponse == IDOK)
				{
					TempTop = (float)ATOF(INumericEntryDialog.vEditString);
					TempValueChanged = true;
				}
				else 
				if (nResponse == 10)
				{
					//Main Menu button pressed
					ExitAutoSetupDialog(10);
				}
			}
			if (TempValueChanged)
			{
				double TempDelta = .5;

				//if new top is higher than half inch from bottom
				if (TempTop > vGlobalCurrentProduct->vProductImageHeightBottom + TempDelta)
				{
					float TempOldTop = vGlobalCurrentProduct->vProductImageHeightTop;
					vGlobalCurrentProduct->vProductImageHeightTop = TempTop;
					//check edge location not too tall
					CheckPurpleLineHeight();

					if ((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode)) //Continuous Feed, or a Pipeline
						ManuallySetWidth((vGlobalCurrentProduct->vProductImageHeightTop - vGlobalCurrentProduct->vProductImageHeightBottom) * 1.5);

					SetChangeMade();
					if (PasswordOK(cTemporaryInspxPassword,false))
						SetDlgItemText(IDC_SubFunction5Display, dtoa(vGlobalCurrentProduct->vProductImageHeightTop,2) + 
						" (" + dtoa(vGlobalCurrentProduct->vProductImageHeightTop * vGlobalPixelsPerUnitInHeight,0) + ")");
					else
						SetDlgItemText(IDC_SubFunction5Display, dtoa(vGlobalCurrentProduct->vProductImageHeightTop,2));

					if (vLocalSystemData->vInAutoSetup)
					{
						vLocalSystemData->vChangedSizeOfProduct = true;
						vLocalSystemData->vChangedSizeOfProductCheckDensityROI = 1;
					}
					if (vAutoSetupType == cNewAutoSetup)
					{
						if (!((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode) ||
							(vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vLocalConfigurationData->vScanTracType == cAllegroScanTrac) ||
							(vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac) || (vLocalConfigurationData->vScanTracType == cBriosoScanTrac))) // Not Continuous Feed, or a Pipeline or TD
						{
							CheckDensityCompensationInspection();
							vGlobalCurrentProduct->CheckAllROIs(vLocalSystemData->vITIPCDig->vProductAverageImage);
						}
						else
						{
							bool TempMakeInspectionsFullImage = false;
							if ((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode) ||
								(vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vLocalConfigurationData->vScanTracType == cAllegroScanTrac) ||
								(vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac) || (vLocalConfigurationData->vScanTracType == cBriosoScanTrac)) //Continuous Feed, or a Pipeline or TD
								TempMakeInspectionsFullImage = true;

							vGlobalCurrentProduct->SetupAllInspectionROIsFromReference(vLocalSystemData->vITIPCDig->vOriginalImage, true, TempMakeInspectionsFullImage, vLocalSystemData->vITIPCDig->vProductAverageImage); 
						}
					}
					else
						vGlobalCurrentProduct->CheckAllROIs(vLocalSystemData->vITIPCDig->vProductAverageImage);

					vLocalSystemData->vITIPCDig->RenameLearnFilesToBackup(vGlobalCurrentProduct);
					ChangedProductSettingsSoReload();
					vLocalSystemData->vITIPCDig->DeleteLearnImages();
					vLocalSystemData->vITIPCDig->DeleteLearnFiles(vGlobalCurrentProduct);
					ClearDensityFeedBackLoop();
					WaitDisplayLocationEdge();
				}
				else
				{
					CNoticeDialog TempNoticeDialog;
					CString TempText1;
					TempText1.LoadString(IDS_Productmustbetallerthanthebottomplus);//"\n\nProduct must be taller than\nthe bottom plus
					CString TempText(TempText1 + ": " + dtoa(TempDelta) + " " + vLocalConfigurationData->vUnitsString);
					TempNoticeDialog.vNoticeText = TempText;
					TempNoticeDialog.vType = cWarningMessage;
					TempNoticeDialog.DoModal();
				}
			}
		}
	}
	else
	if ((vMode == cSetContaminantThreshold) || (vMode == cSetVoidThreshold))
	{
		//ReportErrorMessage("Left Button Down IN SUBFUNCTION 5",cEMailInspx, 32000);
		//if ((vMouseState == 0) || ((vMouseState > 8) && (vMouseState < 17)))	//if mouse is idle or in double click toggle on mode
		//{
		//	vMouseState = 5;	//set mouse to on while pressed down
		//	int TimerResult = SetTimer(vMouseDownRepeatTimerHandle,200,NULL);
		//	if (!TimerResult)
		//		ReportErrorMessage("Error-Second Timer Failed",cEMailInspx,32000);

		//}
		
		if (vInspectionEditing)
		//if (vInspectionEditing->vInspectionType != cJarBottomContaminantInspection)
		{
			if (vInspectionEditing->vFindGoodIntensity)
			{ //finish send good
				WriteCSVFiles();
				UpdateButtonText();
				CString TempText = "Finished Evaluate Good";
				if (vLocalSystemData->vMinimumGoodIndex < 5)
					TempText = "Abort Evaluate Good";
				if (vInspectionEditing)
					TempText = TempText + ": " + vInspectionEditing->vName;

				//if ((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode))
				//	TempText = "Send Good Product";
				//else
				//	TempText.LoadString(IDS_SendGoodContainers);
				CString TempText1("");
				TempText1.LoadString(IDS_Product);
				SetStatusLine(" " + TempText + ": " + dtoa(vLocalSystemData->vMinimumGoodIndex,0) + ", " + TempText1 + ": " + vProductEditingName);
				if (!vInspectionEditing->vSetupHistoryMinimumGoodCount)
					SetDlgItemText(IDC_SubFunction6Button3, _T(" "));
			}
			else
			if ((vOldAverageEnabled) || (vOldTakingQuickLearnImages) || (vGlobalProcessQuickLearnImagesThread) ||
				(vOldStructureEnabled) || (vOldCalibrateBodyTriggerReferenceEnabled))
			{
				CNoticeDialog TempNoticeDialog;
				TempNoticeDialog.vNoticeText = "\n\nScanTrac is Learning.\nScanTrac can not evaluate while learning.";
				TempNoticeDialog.vType = cErrorMessage;
				TempNoticeDialog.DoModal();
			}
			else
			if ((vInspectionEditing->vAddStructure) && (vGlobalLearnState >= 3) &&
				(vInspectionEditing->vROIPixelBottom >= vGlobalCurrentProduct->vReferenceTop / 2))
			{
				CNoticeDialog TempNoticeDialog;
				TempNoticeDialog.vNoticeText = "\n\nCan Not Evaluate Top FM.\nAuto Improve is still in early stages.\nEvaluate Top FM when Auto Improve is done.";
				TempNoticeDialog.vType = cNoticeMessage;
				TempNoticeDialog.DoModal();
			}
			else
			{ //start sending good containers
				if (vLocalSystemData->vInAutoSetup)
				if (!vShowReturnToEditMenu)
					vMainWindowPointer->ResetCounters();
				vChangeMade = false; 
				vAnyChangeMade = true;
				vLocalSystemData->vQualityFactor = 0;
				CString TempText("");
				WriteCSVFiles();
				InitializeThresholdValues();
				vLocalSystemData->vMinimumGoodIndex = 0;
				vLocalSystemData->vMinimumGoodIntensity = 256;
				if (vInspectionEditing)
				if ((vInspectionEditing->vInspectionType == cItemCountInspection) || (vInspectionEditing->vInspectionType == cSizeInspection))
					vLocalSystemData->vMinimumGoodIntensity = 999999999;

				vLocalSystemData->vMinimumGoodAverage = 0;
				vLocalSystemData->vMinimumGoodStandardDeviation = 0;
				vOldMinimumGoodIntensity = 256;
				vOldMinimumGoodAverage = 0;
				vOldMinimumGoodStandardDeviation = 0;
				vHaveAnAutoThresholdValue = false;
				if (vLocalSystemData->vMaximumBadCount < 5)
				{
					vLocalSystemData->vMaximumBadCount = 0;
					vLocalSystemData->vMaximumBadIntensity = 0;
					vLocalSystemData->vMaximumBadIndex = 0;
					vLocalSystemData->vQualityFactor = 0;
					vLocalSystemData->vMaximumBadAverage = 0;
					vLocalSystemData->vMaximumBadStandardDeviation = 0;
				}

				if (vLocalSystemData->vInAutoSetup)
				if (vInspectionEditing->vDoContaminant22Algorithm)
				for (BYTE TempLoop = 0; TempLoop < 7; TempLoop++) 
				{
					vInspectionEditing->_ThresholdsH[TempLoop] = 0;
					vInspectionEditing->_ThresholdsV[TempLoop] = 0;
					vInspectionEditing->_MinValuesGoodH[TempLoop] = 0;
					vInspectionEditing->_MinValuesGoodV[TempLoop] = 0;
					vInspectionEditing->_varH[TempLoop] = 0;
					vInspectionEditing->_varV[TempLoop] = 0;
				}

				if ((vLocalSystemData->vInAutoSetup) || ((vInspectionEditing) && ((vInspectionEditing->vInspectionType == cItemCountInspection) || (vInspectionEditing->vInspectionType == cSizeInspection) || (vInspectionEditing->vInspectionType == cMissingLidInspectionOnSize))))
				{
					if ((vInspectionEditing) && ((vInspectionEditing->vInspectionType == cSizeInspection) || (vInspectionEditing->vInspectionType == cMissingLidInspectionOnSize)))
					if (vInspectionEditing->vSize < 20)// || (vInspectionEditing->vSize > 100))
					{
						vInspectionEditing->vSize = 30;
						SetDlgItemText(IDC_Function1Button, "Size\n" + dtoa(vInspectionEditing->vSize, 0));
					}

					//if (!vChangeMade) // OnSubFunction5Button
					if ((vInspectionEditing) && (vInspectionEditing->vInspectionType == cMissingLidInspectionOnSize))
					if ((vInspectionEditing->vThreshold < 50) || (vInspectionEditing->vThreshold > 100) || (vLocalSystemData->vInAutoSetup))
					{
						//BYTE TempAutoThreshold = vInspectionEditing->vThreshold;
						//if ((vGlobalCurrentProduct->vSetupHistoryAverageDensity > 25) && (vGlobalCurrentProduct->vSetupHistoryAverageDensity < 200))
						//	TempAutoThreshold = (BYTE)(vGlobalCurrentProduct->vSetupHistoryAverageDensity * cMissingLidSizeThresholdRatio);
						//else
						//if ((vLocalSystemData->vITIPCDig->vOriginalImage) && (vLocalSystemData->vITIPCDig->vOriginalImage->vGreyImage) &&
						//	((vLocalSystemData->vITIPCDig->vOriginalImage->vHaveRealImage) || (vLocalConfigurationData->vSimulateLightControlBoard)))
						//{
						//	double TempDensity = vLocalSystemData->vITIPCDig->vOriginalImage->vOriginalImageIntensity;//must use temporary variable to be sure value does not change after range check
						//	if ((TempDensity > 25) && (TempDensity < 200))
						//		TempAutoThreshold = (BYTE)(TempDensity * cMissingLidSizeThresholdRatio);
						//}
						//else
						//	TempAutoThreshold = 70;

						//if ((((vInspectionEditing->vThreshold - 7 > TempAutoThreshold) || (vInspectionEditing->vThreshold + 7 < TempAutoThreshold)) && (vLocalSystemData->vFrameCount < 10)) ||
						//	((vInspectionEditing->vThreshold - 20 > TempAutoThreshold) || (vInspectionEditing->vThreshold + 20 < TempAutoThreshold)))
						//{
						//	vChangeMade = false;
						//	vInspectionEditing->vThreshold = TempAutoThreshold;
						//	ClearEvaluate();
						//}

						if (!vChangeMade)
						if (vInspectionEditing->vThreshold != vInspectionEditing->vSetupHistoryAutoThreshold)
						if (vInspectionEditing->vSetupHistoryAutoThreshold)
						{
							ReportErrorMessage("Evaluate change Size Inspection Threshold from: " + dtoa(vInspectionEditing->vThreshold, 0) + " to: " + dtoa(vInspectionEditing->vSetupHistoryAutoThreshold, 0), cAction, 0);
							//vInspectionEditing->vThreshold = vInspectionEditing->vSetupHistoryAutoThreshold;

							if (vGlobalCurrentProduct->vSetupHistoryAverageDensity)
								vInspectionEditing->vSetupHistoryAutoThreshold = (BYTE)(vGlobalCurrentProduct->vSetupHistoryAverageDensity * 0.71);

							ClearEvaluate();
							SetDlgItemText(IDC_ThresholdForContainerBounds2,dtoa(vInspectionEditing->ConvertToWeight(vInspectionEditing->vThreshold), 0));
						}
					}
				} //end of size inspection

				vInspectionEditing->vFindGoodIntensity = true;
				m_Reference.ShowWindow(SW_SHOW);
				SetDlgItemText(IDC_Reference, _T("0"));

				if ((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode))
					TempText = "Send Good Product";
				else
					TempText.LoadString(IDS_SendGoodContainers);
				CString TempText1("");
				TempText1.LoadString(IDS_Product);
				SetStatusLine(" " + TempText + ", " + TempText1 + ": " + vProductEditingName);

				TempText.LoadString(IDS_None);
				SetDlgItemText(IDC_Width2,TempText);
				TempText.LoadString(IDS_Min);
				SetDlgItemText(IDC_SubFunction6Button3,TempText + ": _");

				if (vInspectionEditing->vInspectionType != cJarBottomContaminantInspection)
				{
					TempText.LoadString(IDS_Avg);
					SetDlgItemText(IDC_SubFunction6Button1,TempText + ": _");
					TempText.LoadString(IDS_SD);
					SetDlgItemText(IDC_SubFunction6Button2,TempText + ": _");
				}
				UpdateButtonText();
				if (!vLocalSystemData->vITIPCDig->vDriftCompensationLocked)
				{
					if ((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode))
						SetStatusLine(" Waiting for lock, Send Good Product...");
					else
						SetStatusLine(" Waiting for lock, Send Good Containers...");
					SetDlgItemText(IDC_SubFunction6Button3, _T("Evaluating"));

					GiveSendGoodContainersNotice();
				}
			}
			UpdateFunction3Button();
		}
	}
	else
	if (vMode == cSetROI)
	{  //move left, left one pixel
		if (!vRecievedButtonPress)
		{
			//ReportErrorMessage("Left Mouse Button Down IN SUBFUNCTION 5",cAction, 32000);
			if ((vMouseState == 0) || ((vMouseState > 8) && (vMouseState < 17)))	//if mouse is idle or in double click toggle on mode
			{
				vMouseState = 5;	//set mouse to on while pressed down
				int TimerResult = SetTimer(vMouseDownRepeatTimerHandle,200,NULL);
				if (!TimerResult)
					ReportErrorMessage("Error-Second Timer Failed",cEMailInspx,32000);
			}
		}
		
		if (vInspectionEditing)
		if (DeleteCheckWeighCalibrations(true))
		{
			/*
			bool TempOKToMoveROI = true;
			if ((vInspectionEditing->vInspectionType == cGlass13mmContaminantInspection) && (vInspectionEditing->vTrimEdgesOfJarAtROI))
			{
				CYesNoDialog TempYesNoDialog;
				TempYesNoDialog.vNoticeText = "\nGlass ROI should be at the edge of the container.\nAre you sure you want to move it?";
				TempYesNoDialog.vYesButtonText = "Leave As Is";
				TempYesNoDialog.vNoButtonText = "Move ROI";
				TempYesNoDialog.vQuestionType = cConfirmQuestion;
				int TempResult = TempYesNoDialog.DoModal();
				if (TempResult == IDOK)
					TempOKToMoveROI = false;
			}

			if (TempOKToMoveROI)
			*/
			if (vInspectionEditing->vROIPixelLeft)
			{
				vInspectionEditing->vROIPixelLeft--; 
				SetROILines();
			}
		}
	}
	else
	if (vMode == cSetDensityThreshold)  //SubFunction5button
	{  //stop or start averaging density data
		
		if (vInspectionEditing)
		if (vInspectionEditing->vInspectionNameForWeightTrendingPointer) //if wieght trending inspection, use other inspections settings
			GiveLinkedInspectionNotice();
		else
		{
			WriteCSVFiles();
			//if stop collecting good, and not starting to collect bad
			if ((vInspectionEditing->vCollectingDensityAverage) && (!vCollectingBadDensityAverage))
			{
				vInspectionEditing->vCollectingDensityAverage = false;
				vCollectingBadDensityAverage = false;
				CString TempText("");
				//if (!vInspectionEditing->vSetupHistoryMinimumGoodAverage)
				{
					TempText.LoadString(IDS_PressStartButtonWhenImagesLookGood);
					CString TempText = "Finished Evaluate Good";
					if (vLocalSystemData->vDensityIndex < 5)
						TempText = "Abort Evaluate Good";
					if (vInspectionEditing)
						TempText = TempText + ": " + vInspectionEditing->vName;
					SetStatusLine(" " + TempText);
				}
				//else
				//	SetStatusLine(" " + vProductEditingName);
				if (!vInspectionEditing->vSetupHistoryMinimumGoodCount)
					SetDlgItemText(IDC_SubFunction6Button3, _T(" "));
			}
			else  //start collecting good values
			if ((vOldAverageEnabled) || (vOldTakingQuickLearnImages) || (vGlobalProcessQuickLearnImagesThread) ||
				(vOldStructureEnabled) || (vOldCalibrateBodyTriggerReferenceEnabled))
			{
				CNoticeDialog TempNoticeDialog;
				TempNoticeDialog.vNoticeText = "\n\nScanTrac is Learning.\nScanTrac can not evaluate while learning.";
				TempNoticeDialog.vType = cErrorMessage;
				TempNoticeDialog.DoModal();
			}
			else
			{
				bool TempOKToStart = true;
				if ((vInspectionEditing->vInspectionType == cCheckWeighInspection) || (vInspectionEditing->vInspectionType == cUnderfillByWeightInspection) || (vInspectionEditing->vInspectionType == cOverfillByWeightInspection))
				{
					if (!vInspectionEditing->vCheckWeighMinimumGoodWeight)
					{
						TempOKToStart = false;
						CNoticeDialog TempNoticeDialog;
						TempNoticeDialog.vNoticeText = "\n\n\nYou must enter Weight of Light Sample first";
						TempNoticeDialog.vType = cErrorMessage;
						TempNoticeDialog.DoModal();
					}
				}
				if (TempOKToStart)
				{
					if ((vInspectionEditing->vInspectionType == cCheckWeighInspection) || (vInspectionEditing->vInspectionType == cUnderfillByWeightInspection) || (vInspectionEditing->vInspectionType == cOverfillByWeightInspection))
					{
						CNoticeDialog TempNoticeDialog;
						TempNoticeDialog.vNoticeText = "\n\nSend your Light Weight Sample Container\nat least 5 times.\nWhen done, press Stop Sending Light";
						TempNoticeDialog.vType = cInstructionsMessage;
						TempNoticeDialog.DoModal();
					}
					vAnyChangeMade = true;
					vLocalSystemData->vDensityIndex = 0;
					vLocalSystemData->vFrameCount = 0;
					vLocalSystemData->vITIPCDig->vDensityMinimumValue = 255;
					if ((vInspectionEditing->vInspectionType == cUnderfillByWeightInspection) || (vInspectionEditing->vInspectionType == cOverfillByWeightInspection) || (vInspectionEditing->vInspectionType == cCheckWeighInspection))
						vLocalSystemData->vITIPCDig->vDensityMinimumValue = (float)0xFFFFFFFFFFFFFFFF;
					vLocalSystemData->vITIPCDig->vDensityMaximumValue = 0;
					vLocalSystemData->vITIPCDig->vDensityAverage = 0;
					vLocalSystemData->vITIPCDig->vDensityStandardDeviation = 0;
					vLocalSystemData->vQualityFactor = 0;
					vOldQualityFactor = 0;
					SetDlgItemText(IDC_Simulating, _T(""));
					vHaveAnAutoThresholdValue = false;

					vInspectionEditing->vSetupHistoryMinimumGoodAverage = 0;
					vInspectionEditing->vSetupHistoryMinimumGoodStandardDeviation = 0;
					vInspectionEditing->vSetupHistoryMinimumGoodCount = 0;
					vInspectionEditing->vSetupHistoryMinimumGood = 0;
					if ((vInspectionEditing->vInspectionType == cCheckWeighInspection) || (vInspectionEditing->vInspectionType == cUnderfillByWeightInspection) || (vInspectionEditing->vInspectionType == cOverfillByWeightInspection))
					{
						if (vInspectionEditing->vDensityThresholdLower > 5000)
							vInspectionEditing->SetDensityLowerThreshold((float)vInspectionEditing->ConvertToWeight(vInspectionEditing->vDensityThresholdLower));
						if (vInspectionEditing->vDensityThresholdUpper > 5000)
							vInspectionEditing->vDensityThresholdUpper = (float)vInspectionEditing->ConvertToWeight(vInspectionEditing->vDensityThresholdUpper);

						vInspectionEditing->vCheckWeighMFactor = 1;
						vInspectionEditing->vCheckWeighBFactor = 0;
						vInspectionEditing->vCheckWeighOffset = 0;
					}

					vInspectionEditing->vCollectingDensityAverage = true;
					vCollectingBadDensityAverage = false;
					CString TempText("");
					if ((vInspectionEditing) &&
						(vInspectionEditing->vInspectionType == cStandardDeviationInspection))
						TempText.LoadString(IDS_CollectingTextureSamples);
					else
						TempText.LoadString(IDS_CollectingDensitySamples);
					SetStatusLine(" " + TempText + ": " + vProductEditingName);

					TempText.LoadString(IDS_Min);
					SetDlgItemText(IDC_SubFunction5Display,TempText + ": _");
					//TempText.LoadString(IDS_Max);
					TempText.LoadString(IDS_Evaluating);
					SetDlgItemText(IDC_SubFunction6Button3,TempText);

					TempText.LoadString(IDS_Avg);
					SetDlgItemText(IDC_SubFunction6Button1,TempText + ": _");
					TempText.LoadString(IDS_SD);
					SetDlgItemText(IDC_SubFunction6Button2,TempText + ": _");
					if (!vLocalSystemData->vITIPCDig->vDriftCompensationLocked)
					{
						if ((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode))
							SetStatusLine(" Waiting for lock, Send Good Product...");
						else
							SetStatusLine(" Waiting for lock, Send Good Containers...");
						SetDlgItemText(IDC_SubFunction6Button3, _T("Evaluating"));

						if (!vLocalSystemData->vITIPCDig->vDriftCompensationLocked)
							GiveSendGoodContainersNotice();
					}
				}
			}
			UpdateButtonText();
			UpdateFunction3Button();
		}
	}
}

void CAutoSetupImageDisplayDialog::OnSubFunction6Button() 
{
	if (vInitialized)
	if (vMode == CMultiLanePositionAdjustment)
	{
		if (vGlobalCurrentProduct->vMultiLaneStartPixel[vMultiImageLaneEditing])
		{
			vGlobalCurrentProduct->vMultiLaneStartPixel[vMultiImageLaneEditing]--;
			UpdateButtonText();
			WaitThenDrawImageOnScreen();
		}
		else
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\n\nLane at edge of image";
			TempNoticeDialog.vType = cErrorMessage;
			TempNoticeDialog.DoModal();
		}
	}
	else
	if (vMode == cSetSizeSettings)
	{  //move purple line height down one pixel
		if (!vRecievedButtonPress)
		{
			//ReportErrorMessage("Left Mouse Button Down IN SUBFUNCTION 6",cAction, 32000);
			if ((vMouseState == 0) || ((vMouseState > 8) && (vMouseState < 17)))	//if mouse is idle or in double click toggle on mode
			{
				vMouseState = 6;	//set mouse to on while pressed down
				int TimerResult = SetTimer(vMouseDownRepeatTimerHandle,200,NULL);
				if (!TimerResult)
					ReportErrorMessage("Error-Second Timer Failed",cEMailInspx,32000);
			}
		}
		if (vAdjustingBottomEdgeFinding)
		{
			if (vGlobalCurrentProduct->vBottomLocationLengthPixel > 3)
			{
				vChangedPurpleLine = true;
				vGlobalCurrentProduct->vBottomLocationLengthPixel--;
				vGlobalCurrentProduct->SetBottomLocationLength((float)(vGlobalCurrentProduct->vBottomLocationLengthPixel / vGlobalPixelsPerUnit));

				SetChangeMade();
				UpdateButtonText();

				vNeedToSetContainerThresholdBounds = 1;

				//9/10/2009
				if (!vHaveAnImage)
				if (!vGaveSendAContainerNotice)
				{
					vMouseState = 0;
					vGaveSendAContainerNotice = true;
					CNoticeDialog TempNoticeDialog;
					TempNoticeDialog.vNoticeText = "\n\n\nSend another container through\nto update the screen";
					TempNoticeDialog.vType = cInformationMessage;
					TempNoticeDialog.DoModal();
				}
				CalculateLearnedReferenceValues();

				WaitThenDrawImageOnScreen();
				WaitDisplayLocationEdge();
			}
		}
		else
		{  //move purple line height down one pixel
			if (vGlobalCurrentProduct->vEdgeLocationHeightPixel > 3)
			{
				vChangedPurpleLine = true;
				vGlobalCurrentProduct->vEdgeLocationHeightPixel--;
				vGlobalCurrentProduct->SetEdgeLocationHeight((float)(vGlobalCurrentProduct->vEdgeLocationHeightPixel / vGlobalPixelsPerUnitInHeight));

				SetChangeMade();
				UpdateButtonText();

				vNeedToSetContainerThresholdBounds = 1;

				//9/10/2009
				if (!vHaveAnImage)
				if (!vGaveSendAContainerNotice)
				{
					vMouseState = 0;
					vGaveSendAContainerNotice = true;
					CNoticeDialog TempNoticeDialog;
					TempNoticeDialog.vNoticeText = "\n\n\nSend another container through\nto update the screen";
					TempNoticeDialog.vType = cInformationMessage;
					TempNoticeDialog.DoModal();
				}
				CalculateLearnedReferenceValues();

				WaitThenDrawImageOnScreen();
				WaitDisplayLocationEdge();
			}
		}
	}
	else
		/*
	if (vMode == cEdgeExposureMenu)
	{		//Make Edges A Little Brighter
		if (vLocalSystemData->vInAutoSetup)
		{
			if (vGlobalCurrentProduct->vXRayIntegrationTimeAtEdge < vGlobalCurrentProduct->vXRayIntegrationTime)
			{
				WORD TempOnePercentStep = (vGlobalCurrentProduct->vXRayIntegrationTime - 205) / 100;
				if (TempOnePercentStep < 1)
					TempOnePercentStep = 1;
				if (vGlobalCurrentProduct->vXRayIntegrationTimeAtEdge + TempOnePercentStep < vGlobalCurrentProduct->vXRayIntegrationTime)
					vGlobalCurrentProduct->vXRayIntegrationTimeAtEdge = vGlobalCurrentProduct->vXRayIntegrationTimeAtEdge + TempOnePercentStep;
				else
					vGlobalCurrentProduct->vXRayIntegrationTimeAtEdge = vGlobalCurrentProduct->vXRayIntegrationTime;

				DisplayEdgeExposureTimePercent();

				//variable Integration Time temp can't do as will mess something up in the FPGA or uC, so reload all settings
				vMainWindowPointer->SendProductIntegrationTime(vGlobalCurrentProduct->vXRayIntegrationTime, 
					vGlobalCurrentProduct->vXRayIntegrationTimeAtEdge);

				//vMainWindowPointer->StopRunning(true);

				//int TimerResult = SetTimer(vStartSetupTimerHandle,1000,NULL);
				//if (!TimerResult)
				//	ReportErrorMessage("Error-Start Timer Failed",cEMailInspx,32000);

			}
			else
				GiveMessageAlreadyAtThisValue(2); //2 = maximum value
		}
		else
			YouCantChangeSettingBecauseInspecting();
	}
	else
	*/
	if (vMode == cCenterExposureMenu)
	{		//Edit X-Ray current
		bool TempByPassDeletingLearn = false;
		if ((vLocalSystemData->vInAutoSetup) || (PasswordOK(cSuperInspxPassword,false)))
		{
			if (PasswordOK(cSuperInspxPassword,false))
			{
				if (vGlobalCurrentProduct->ProductHasImageFiles(vGlobalCurrentDirectory))
				{
					CYesNoDialog TempYesNoDialog;
					TempYesNoDialog.vNoticeText =  _T("\nThis product has been Learned.\nIf you change this setting it may effect the Learn.\nDo you want to keep it regardless?");
					TempYesNoDialog.vYesButtonText = "Keep Learn";
					TempYesNoDialog.vQuestionType = cConfirmQuestion;
					int TempResult = TempYesNoDialog.DoModal();
					if (TempResult == IDOK)
						TempByPassDeletingLearn = true;
				}
			}
			CString TempCurrentString = dtoa(vGlobalCurrentProduct->vXRaySourceCurrent, 2);
			CNumericEntryDialog INumericEntryDialog;  
					
			//Set dialog box data titles and number value
			INumericEntryDialog.vEditString = TempCurrentString;

			CString TempText;
			CString TempText1;
			TempText.LoadString(IDS_Product);
			INumericEntryDialog.m_DialogTitleStaticText1 = "X-Ray Source Current for " + TempText + ": " + vProductEditingName;
			TempText.LoadString(IDS_OriginalValue);
			INumericEntryDialog.m_DialogTitleStaticText4 = TempText + ": " + INumericEntryDialog.vEditString;

			TempText.LoadString(IDS_MeasuredIn);
			TempText1.LoadString(IDS_MilliAmps);
			INumericEntryDialog.m_UnitsString = TempText + " " + TempText1;
			INumericEntryDialog.m_DialogTitleStaticText2 = "";

			if (vLocalConfigurationData->vHVPSType == c100WMonoBlockHVPS)
			{
				if (vGlobalDXMXRayPort)
					INumericEntryDialog.m_DialogTitleStaticText2 = "Minimum Current for 100 Watt Mono Block is: " + dtoa(vGlobalDXMXRayPort->vMinimumCurrent, 2) + " mA";
				INumericEntryDialog.vMinValue = vGlobalDXMXRayPort->vMinimumCurrent;
			}
			else
				INumericEntryDialog.m_DialogTitleStaticText2 = "";

			INumericEntryDialog.vMaxValue = vLocalConfigurationData->vMaximumCurrent;
			INumericEntryDialog.m_DialogTitleStaticText3 = "Maximum Current: " + dtoa(INumericEntryDialog.vMaxValue,2);

			if (vGlobalCurrentProduct->vXRaySourceVoltage == 0)
			{
				INumericEntryDialog.vMaxValue = 0;
				INumericEntryDialog.m_DialogTitleStaticText3 = "Maximum Current: 0 mA at 0 kV";
			}

			double TempMaximumCurrent = GetMaximumCurrentFromVoltage(vLocalSystemData->vRampXRayVoltageAmount * 6);
			if (INumericEntryDialog.vMaxValue > TempMaximumCurrent)
			{
				INumericEntryDialog.vMaxValue = TempMaximumCurrent;
				INumericEntryDialog.m_DialogTitleStaticText3 = "Maximum Current: " + dtoa(TempMaximumCurrent,2) + " from Voltage Limit";
			}

			TempMaximumCurrent = GetMaximumCurrentFromPower(vLocalSystemData->vRampXRayVoltageAmount *  6,vLocalConfigurationData->vMaximumPower);
			if (INumericEntryDialog.vMaxValue > TempMaximumCurrent)
			{
				INumericEntryDialog.vMaxValue = TempMaximumCurrent;
				INumericEntryDialog.m_DialogTitleStaticText3 = "Maximum Current: " + dtoa(TempMaximumCurrent,2) + " from Power Limit";
			}

			INumericEntryDialog.vIntegerOnly = false;
			//Pass control to dialog box and display
			int nResponse = INumericEntryDialog.DoModal();
			//dialog box is now closed, if user pressed select do this
			//if user pressed cancel, do nothing
			if (nResponse == IDOK)
			{
				TempCurrentString = INumericEntryDialog.vEditString;
				vNeedToSetContainerThresholdBounds = 2;

				SetChangeMade();
				if ((!TempByPassDeletingLearn) &&	(vLocalSystemData->vInAutoSetup))
				{
					vLocalSystemData->vITIPCDig->RenameLearnFilesToBackup(vGlobalCurrentProduct);
					vLocalSystemData->vITIPCDig->DeleteLearnImages();
					vLocalSystemData->vITIPCDig->DeleteLearnFiles(vGlobalCurrentProduct);
					ClearDensityFeedBackLoop();
				}
				SetDlgItemText(IDC_SubFunction6Display,TempCurrentString);
				vGlobalCurrentProduct->vXRaySourceCurrent = (float)ATOF(TempCurrentString);
				vLocalSystemData->vRampXRayCurrentAmount = vGlobalCurrentProduct->vXRaySourceCurrent;
				vLocalSystemData->vRampXRayVoltageAmount = vGlobalCurrentProduct->vXRaySourceVoltage;
				vMainWindowPointer->SendSourceSettingsToUController(vLocalSystemData->vRampXRayVoltageAmount,vLocalSystemData->vRampXRayCurrentAmount);
			}
			else 
			if (nResponse == 10)
			{
				//Main Menu button pressed
				ExitAutoSetupDialog(10);
			}
		}
		else
		{
			CNoticeDialog TempNoticeDialog;
			CString TempText;
			TempText.LoadString(IDS_Youcannotchangeimageexposurewhileinspecting);
			//TempNoticeDialog.vNoticeText = "\n\nYou can not change image exposure while inspecting.\nYou must stop running on the Main Menu,\nthen Edit the Product Exposure";
			TempNoticeDialog.vNoticeText = TempText;
			TempNoticeDialog.vType = cErrorMessage;
			TempNoticeDialog.DoModal();
		}
	}
	else
	if (vMode == cLearnMenu)
	{
		if (PasswordOK(cTemporaryInspxPassword,false))
		{	
			bool TempDone = false;
			while (!TempDone)
			{
				if ((vShowAverage) && (HaveAContaminantStructure()))
				{
					vShowAverage = false;
					vShowStructure = true;
					vShowVoidStructure = false;
					//vShowGlassStructure = false;
					vShowContaminant2Structure = false;
					TempDone = true;
				}
				else
				if (((vShowAverage) || (vShowStructure)) && (vLocalSystemData->vITIPCDig->vProductContaminant2StructureImage))
				{
					vShowAverage = false;
					vShowStructure = false;
					vShowContaminant2Structure = true;
					TempDone = true;
				}
				else
				if ((//(vShowGlassStructure) || 
					(vShowAverage) || (vShowStructure) || (vShowContaminant2Structure)) && (HaveAVoidStructure()))
				{
					vShowAverage = false;
					vShowStructure = false;
					vShowContaminant2Structure = false;
					//vShowGlassStructure = false;
					vShowVoidStructure = true;
					TempDone = true;
				}
				else
				if (((vShowContaminant2Structure) || //(vShowGlassStructure) || 
					(vShowAverage) || (vShowVoidStructure) || (vShowStructure)) && 
					(vLocalSystemData->vITIPCDig->vProductHistogramImage))
				{
					vShowAverage = false;
					vShowStructure = false;
					vShowVoidStructure = false;
					//vShowGlassStructure = false;
					vShowContaminant2Structure = false;
					vShowHistogram = true;
					TempDone = true;
				}
				else
				if ((vShowContaminant2Structure) || //(vShowGlassStructure) || 
					(vShowAverage) || (vShowVoidStructure) || (vShowStructure) || (vShowHistogram))//( || (vShowGlassHistogram))
				{
					vShowAverage = false;
					vShowStructure = false;
					vShowVoidStructure = false;
					//vShowGlassStructure = false;
					vShowContaminant2Structure = false;
					vShowHistogram = false;
					TempDone = true;
				}
				else
				{
					vShowAverage = true;
					TempDone = true;
				}
			}
			DrawImageOnScreen();
			UpdateButtonText();
			DisplayWithPassword();
		}
	}
	else
	if (vMode == cSetImageSize) //sF6
	{
		if (!vGlobalCurrentProduct->vMultiLaneNumberOfLanes) //don't do if in Multi Lane
		if ((PasswordOK(cAdvancedSetupPassword,false)) || ((!vLocalConfigurationData->vContinuousFeedContainerRate) && (!vLocalConfigurationData->vBulkProductMode) && (!vGlobalCurrentProduct->vBulkProductMode))) //Not Continuous Feed, Not a Pipeline or password in
		if (!vLocalSystemData->vInAutoSetup)
		{
			YouCantChangeSettingBecauseInspecting();
		}
		else
		if (((!vLocalSystemData->vXRayOnNotice) || (vStartingUpSource) || (vRestartingXRaysWithNewSettings) ||
			((vLocalSystemData->vSystemRunMode != cRunningSystemMode) && (vLocalSystemData->vSystemRunMode != cAutoSetupRunningSystem))) &&
			(vMainWindowPointer->InterlocksOK()) && (vStartedSource))
			StartingSystemPleaseTryAgain();
		else
		if (NoSetupOrOKToChange())
		{
			//Bottom Button Was Pressed
			CNumericEntryDialog INumericEntryDialog;  
			
			//Set dialog box data titles and number value
			INumericEntryDialog.vEditString = dtoa(vGlobalCurrentProduct->vProductImageHeightBottom);

			CString TempText("");
			if (vGlobalScanTracType == cBriosoScanTrac)
				TempText = "Left of X-Ray Image";

			TempText = vGlobalBottomString + " of X-Ray Image";
			INumericEntryDialog.m_DialogTitleStaticText1 = TempText;

			if ((vGlobalScanTracType != cBriosoScanTrac) && (vLocalConfigurationData->vScanTracType != cAtempoScanTrac) && (vLocalConfigurationData->vScanTracType != cForteScanTrac) && 
				(vLocalConfigurationData->vScanTracType != cAllegroScanTrac) && (vLocalConfigurationData->vScanTracType != cCaseInspectorScanTrac)) //Not TD
				INumericEntryDialog.m_DialogTitleStaticText2 = "Should ALLWAYS be zero";

			TempText.LoadString(IDS_Product);
			INumericEntryDialog.m_DialogTitleStaticText3 = TempText + ": " + vProductEditingName;
			TempText.LoadString(IDS_OriginalValue);
			INumericEntryDialog.m_DialogTitleStaticText4 = TempText + ": " + INumericEntryDialog.vEditString;

			TempText.LoadString(IDS_MeasuredIn);
			INumericEntryDialog.m_UnitsString = TempText + " " + vLocalConfigurationData->vUnitsString;

			double TempImageHeight = 0.5;
			//if (vLocalConfigurationData->vUseMetric)
			//	TempImageHeight = TempImageHeight * 25.4;

			INumericEntryDialog.vMaxValue = vGlobalCurrentProduct->vProductImageHeightTop - TempImageHeight;
			INumericEntryDialog.vIntegerOnly = false;
			//Pass control to dialog box and display
			int nResponse = INumericEntryDialog.DoModal();
			//dialog box is now closed, if user pressed select do this
			//if user pressed cancel, do nothing
			if (nResponse == IDOK)
			{
				float TempBottom = (float)ATOF(INumericEntryDialog.vEditString);
				double TempDelta = 0.5;
				//if (vLocalConfigurationData->vUseMetric)
				//	TempDelta = 13;
				if (TempBottom + TempDelta <= vGlobalCurrentProduct->vProductImageHeightTop)
				{
					vGlobalCurrentProduct->vProductImageHeightBottom = TempBottom;
					SetChangeMade();

					if ((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode)) //Continuous Feed, or a Pipeline
						ManuallySetWidth((vGlobalCurrentProduct->vProductImageHeightTop - vGlobalCurrentProduct->vProductImageHeightBottom) * 1.5);

					if (vGlobalScanTracType == cBriosoScanTrac)//on Brioso, keep X and Y size the same.
					{
						ManuallySetWidth((vGlobalCurrentProduct->vProductImageHeightTop - vGlobalCurrentProduct->vProductImageHeightBottom));

						if (vGlobalCurrentProduct->vProductImageWidth != (vGlobalCurrentProduct->vProductImageHeightTop - vGlobalCurrentProduct->vProductImageHeightBottom))
							//vGlobalCurrentProduct->vProductImageHeightBottom = vGlobalCurrentProduct->vProductImageHeightTop - vGlobalCurrentProduct->vProductImageWidth;
							vGlobalCurrentProduct->vProductImageHeightBottom = (float)(vGlobalCurrentProduct->vProductImageHeightTop - 
							(vGlobalCurrentProduct->vImageWidthPosition / (vGlobalPixelsPerUnit * vGlobalCurrentProduct->vOverScanMultiplier)));
					}

					if ((PasswordOK(cTemporaryInspxPassword,false)) && (vGlobalCurrentProduct->vProductImageHeightBottom))
						SetDlgItemText(IDC_SubFunction6Display, dtoa(vGlobalCurrentProduct->vProductImageHeightBottom,2) + 
						" (" + dtoa(vGlobalCurrentProduct->vProductImageHeightBottom * vGlobalPixelsPerUnitInHeight,0) + ")");
					else
						SetDlgItemText(IDC_SubFunction6Display, dtoa(vGlobalCurrentProduct->vProductImageHeightBottom,2));

					if (vGlobalCurrentProduct->vDriftDensityInspection)
					if (!vGlobalCurrentProduct->vMultiLaneNumberOfLanes)
					if ((!vGlobalCurrentProduct->vDriftDensityInspection->vROIPixelBottom) ||
						(vGlobalCurrentProduct->vDriftDensityInspection->vROIBottom < vGlobalCurrentProduct->vProductImageHeightBottom))
						vGlobalCurrentProduct->vDriftDensityInspection->SetROIBottom(vGlobalCurrentProduct->vProductImageHeightBottom + vImageMargin);

					if (vGlobalCurrentProduct->vNumberOfInspections)
					for (BYTE TempLoop = 0; TempLoop < vGlobalCurrentProduct->vNumberOfInspections; TempLoop++)
					{
						if (vGlobalCurrentProduct->vInspection[TempLoop]->vROIBottom < vGlobalCurrentProduct->vProductImageHeightBottom)
						vGlobalCurrentProduct->vInspection[TempLoop]->SetROIBottom(vGlobalCurrentProduct->vProductImageHeightBottom);
					}

					//check edge location not too tall
					CheckPurpleLineHeight();

					vLocalSystemData->vITIPCDig->RenameLearnFilesToBackup(vGlobalCurrentProduct);

					if (vLocalSystemData->vLogFile.vLogSerialData)
						vLocalSystemData->vLogFile.WriteToLogFile("Manually Set Bottom, Reload Product",cDebugMessage);

					ChangedProductSettingsSoReload();
					vLocalSystemData->vITIPCDig->DeleteLearnImages();
					vLocalSystemData->vITIPCDig->DeleteLearnFiles(vGlobalCurrentProduct);
					ClearDensityFeedBackLoop();
				}
				else
				{
					CNoticeDialog TempNoticeDialog;
					TempText.LoadString(IDS_Productbottommustbelowerthantopminus);//"\n\nProduct bottom must be lower than\ntop minus
					TempText = TempText + ": " + dtoa(TempDelta) + " " + vLocalConfigurationData->vUnitsString;
					TempNoticeDialog.vNoticeText = TempText;
					TempNoticeDialog.vType = cWarningMessage;
					TempNoticeDialog.DoModal();
				}
			}
			else 
			if (nResponse == 10)
			{
				//Main Menu button pressed
				ExitAutoSetupDialog(10);
			}
		}
	}
	else
	if (vMode == cSetROI)
	{  //move left, right one pixel
		if (!vRecievedButtonPress)
		{
			//ReportErrorMessage("Left Mouse Button Down IN SUBFUNCTION 6",cAction, 32000);
			if ((vMouseState == 0) || ((vMouseState > 8) && (vMouseState < 17)))	//if mouse is idle or in double click toggle on mode
			{
				vMouseState = 6;	//set mouse to on while pressed down
				int TimerResult = SetTimer(vMouseDownRepeatTimerHandle,200,NULL);
				if (!TimerResult)
					ReportErrorMessage("Error-Second Timer Failed",cEMailInspx,32000);
			}
		}
		
		if (vInspectionEditing)
		if (DeleteCheckWeighCalibrations(true))
		{
			/*
			bool TempOKToMoveROI = true;
			if ((vInspectionEditing->vInspectionType == cGlass13mmContaminantInspection) && (vInspectionEditing->vTrimEdgesOfJarAtROI))
			{
				CYesNoDialog TempYesNoDialog;
				TempYesNoDialog.vNoticeText = "\nGlass ROI should be at the edge of the container.\nAre you sure you want to move it?";
				TempYesNoDialog.vYesButtonText = "Leave As Is";
				TempYesNoDialog.vNoButtonText = "Move ROI";
				TempYesNoDialog.vQuestionType = cConfirmQuestion;
				int TempResult = TempYesNoDialog.DoModal();
				if (TempResult == IDOK)
					TempOKToMoveROI = false;
			}

			if (TempOKToMoveROI)
			*/
			if ((vInspectionEditing->vROIPixelLeft < vInspectionEditing->vROIPixelRight - 2) && (vInspectionEditing->vROIPixelRight > 2))
			{
				vInspectionEditing->vROIPixelLeft++; 
				SetROILines();
			}
		}
	}
	else
	if (vMode == cSetDensityThreshold)
	{  //show histogram of density data
		
		if (vInspectionEditing)
		if (vInspectionEditing->vInspectionNameForWeightTrendingPointer) //if wieght trending inspection, use other inspections settings
			GiveLinkedInspectionNotice();
		else
		if ((vLocalSystemData->vDensityDataD) && (vLocalSystemData->vDensityIndex))
		{
			double *TempData = NULL;
			if ((vInspectionEditing->vCheckWeighMFactor != 1) || (vInspectionEditing->vCheckWeighBFactor != 0))
			{
				if (ThereIsEnoughMemory(vLocalSystemData->vDensityIndex * 8, "Temp Plot buffer"))
				{
					TempData = (double *)malloc(vLocalSystemData->vDensityIndex * 8);
					if (TempData)
					{
						double *TempSourcePointer = vLocalSystemData->vDensityDataD;
						double *TempDestinationPointer = TempData;
						for (DWORD TempLoop = 0; TempLoop < vLocalSystemData->vDensityIndex; TempLoop++)
						{
							*TempDestinationPointer++ = vInspectionEditing->ConvertToWeight(*TempSourcePointer++);
						}
					}
				}
			}
			CHistogramDisplayDialog IHistogramDisplayDialog;
			IHistogramDisplayDialog.vNumberOfNumbers = vLocalSystemData->vDensityIndex;
			IHistogramDisplayDialog.vValuesAreFloats = true;

			if ((vInspectionEditing) && (vInspectionEditing->vInspectionType == cStandardDeviationInspection))
				IHistogramDisplayDialog.vDataTitle = "Texture in ROI";
			else
			if (TempData)
				IHistogramDisplayDialog.vDataTitle = "Weight in ROI";
			else
				IHistogramDisplayDialog.vDataTitle = "Density in ROI";

			IHistogramDisplayDialog.vPointerToFloats = vLocalSystemData->vDensityDataD;
			if (TempData)
				IHistogramDisplayDialog.vPointerToFloats = TempData;

			IHistogramDisplayDialog.vLocalSystemData = vLocalSystemData;
			IHistogramDisplayDialog.vLocalConfigurationData = vLocalConfigurationData;
			IHistogramDisplayDialog.vMainWindowPointer = vMainWindowPointer;
			int nResponse = IHistogramDisplayDialog.DoModal();

			if (TempData)
				free(TempData);
			if (nResponse == 10)
			{
				//Main Menu button pressed
				Sleep(1); //is the equivelent of a yeild statement;
				ExitAutoSetupDialog(10);
			}
		}
		/*
		else
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\n\nNo Minimum Good Data to Show";
			TempNoticeDialog.vType = cErrorMessage;
			TempNoticeDialog.DoModal();
		}
		*/
	}
	else
	if ((vMode == cSetContaminantThreshold) || (vMode == cSetVoidThreshold))
	{  //show histogram of Minimum Good data
		//ReportErrorMessage("Left Button Down IN SUBFUNCTION 6",cEMailInspx, 32000);
		//if ((vMouseState == 0) || ((vMouseState > 8) && (vMouseState < 17)))	//if mouse is idle or in double click toggle on mode
		//{
		//	vMouseState = 6;	//set mouse to on while pressed down
		//	int TimerResult = SetTimer(vMouseDownRepeatTimerHandle,200,NULL);
		//	if (!TimerResult)
		//		ReportErrorMessage("Error-Second Timer Failed",cEMailInspx,32000);

		//}
		
		if (vLocalSystemData->vMinimumGoodData)
		{
			CHistogramDisplayDialog IHistogramDisplayDialog;
			IHistogramDisplayDialog.vNumberOfNumbers = vLocalSystemData->vMinimumGoodIndex;
			IHistogramDisplayDialog.vDataTitle = "Minimum Good in ROI";
			IHistogramDisplayDialog.vValuesAreBytes = true;
			IHistogramDisplayDialog.vPointerToBytes = vLocalSystemData->vMinimumGoodData;
			IHistogramDisplayDialog.vLocalSystemData = vLocalSystemData;
			IHistogramDisplayDialog.vLocalConfigurationData = vLocalConfigurationData;
			IHistogramDisplayDialog.vMainWindowPointer = vMainWindowPointer;
			int nResponse = IHistogramDisplayDialog.DoModal();
			if (nResponse == 10)
			{
				//Main Menu button pressed
				Sleep(1); //is the equivelent of a yeild statement;
				ExitAutoSetupDialog(10);
			}
		}
		else
		if (vLocalSystemData->vMinimumGoodDataFloat)
		{
			CHistogramDisplayDialog IHistogramDisplayDialog;
			IHistogramDisplayDialog.vNumberOfNumbers = vLocalSystemData->vMinimumGoodIndex;
			IHistogramDisplayDialog.vDataTitle = "Minimum Good in ROI";
			IHistogramDisplayDialog.vValuesAreFloats = true;
			IHistogramDisplayDialog.vPointerToFloats = vLocalSystemData->vMinimumGoodDataFloat;
			IHistogramDisplayDialog.vLocalSystemData = vLocalSystemData;
			IHistogramDisplayDialog.vLocalConfigurationData = vLocalConfigurationData;
			IHistogramDisplayDialog.vMainWindowPointer = vMainWindowPointer;
			int nResponse = IHistogramDisplayDialog.DoModal();
			if (nResponse == 10)
			{
				//Main Menu button pressed
				Sleep(1); //is the equivelent of a yeild statement;
				ExitAutoSetupDialog(10);
			}
		}
		/*
		else
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\n\nNo Minimum Good Data to Show";
			TempNoticeDialog.vType = cErrorMessage;
			TempNoticeDialog.DoModal();
		}
		*/
	}
	CWnd *TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText1);
	if (TempWindow)
		TempWindow->Invalidate(false);
}

void CAutoSetupImageDisplayDialog::OnSubFunction7Button() 
{
	if (vInitialized)
	if (vMode == CMultiLanePositionAdjustment)
	{
		if (vGlobalCurrentProduct->vMultiLaneStartPixel[vMultiImageLaneEditing] + vGlobalCurrentProduct->vMultiLaneWidthInPixels[vMultiImageLaneEditing] < vMaximumLaneWidth)
		{
			vGlobalCurrentProduct->vMultiLaneWidthInPixels[vMultiImageLaneEditing]++;
			UpdateButtonText();
			WaitThenDrawImageOnScreen();
		}
		else
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\n\nLane at edge of image";
			TempNoticeDialog.vType = cErrorMessage;
			TempNoticeDialog.DoModal();
		}
	}
	else
	if (vMode == cSetSizeSettings)
	{
		if (!vRecievedButtonPress)
		{
			//ReportErrorMessage("Left Mouse Button Down IN SUBFUNCTION 7",cAction, 32000);
			if ((vMouseState == 0) || ((vMouseState > 8) && (vMouseState < 17)))	//if mouse is idle or in double click toggle on mode
			{
				vMouseState = 7;	//set mouse to on while pressed down
				int TimerResult = SetTimer(vMouseDownRepeatTimerHandle,200,NULL);
				if (!TimerResult)
					ReportErrorMessage("Error-Second Timer Failed",cEMailInspx,32000);
			}
		}
		
		if (vAdjustingBottomEdgeFinding)
		{
			if ((vGlobalCurrentProduct->vBottomLocationLengthPixel + vGlobalCurrentProduct->vBottomLocationLeftPixel) < vLocalSystemData->vITIPCDig->vOriginalBufferSizeY - 2)
			{
				vChangedPurpleLine = true;
				vGlobalCurrentProduct->vBottomLocationLeftPixel++;
				vGlobalCurrentProduct->SetBottomLocationLeft((float)(vGlobalCurrentProduct->vBottomLocationLeftPixel / vGlobalPixelsPerUnit));

				SetChangeMade();
				UpdateButtonText();

				vNeedToSetContainerThresholdBounds = 1;

				//9/10/2009
				if (!vHaveAnImage)
				if (!vGaveSendAContainerNotice)
				{
					vMouseState = 0;
					vGaveSendAContainerNotice = true;
					CNoticeDialog TempNoticeDialog;
					TempNoticeDialog.vNoticeText = "\n\n\nSend another container through\nto update the screen";
					TempNoticeDialog.vType = cInformationMessage;
					TempNoticeDialog.DoModal();
				}
				CalculateLearnedReferenceValues();

				WaitThenDrawImageOnScreen();
				WaitDisplayLocationEdge();
			}
		}
		else
		{
			if ((vGlobalCurrentProduct->vEdgeLocationHeightPixel + vGlobalCurrentProduct->vEdgeLocationBottomPixel) < vLocalSystemData->vITIPCDig->vOriginalBufferSizeX - 2)
			{
				vChangedPurpleLine = true;
				vGlobalCurrentProduct->vEdgeLocationBottomPixel++;
				vGlobalCurrentProduct->SetEdgeLocationBottom((float)(vGlobalCurrentProduct->vEdgeLocationBottomPixel / vGlobalPixelsPerUnitInHeight));

				SetChangeMade();
				UpdateButtonText();

				vNeedToSetContainerThresholdBounds = 1;

				//9/10/2009
				if (!vHaveAnImage)
				if (!vGaveSendAContainerNotice)
				{
					vMouseState = 0;
					vGaveSendAContainerNotice = true;
					CNoticeDialog TempNoticeDialog;
					TempNoticeDialog.vNoticeText = "\n\n\nSend another container through\nto update the screen";
					TempNoticeDialog.vType = cInformationMessage;
					TempNoticeDialog.DoModal();
				}
				CalculateLearnedReferenceValues();

				WaitThenDrawImageOnScreen();
				WaitDisplayLocationEdge();
			}
		}
	}
	else
	if (vMode == cLearnMenu)
	{
		if ((PasswordOK(cTemporaryInspxPassword,false)) || (PasswordOK(cAdvancedSetupPassword,false)) || (vLocalConfigurationData->vAutoImproveEnabled))
		if ((vLocalSystemData->vTakingQuickLearnImages) ||
			(vOldAverageEnabled) || (vOldStructureEnabled) || 
			(vOldCalibrateBodyTriggerReferenceEnabled))
		{
			CNoticeDialog TempNoticeDialog;
			CString TempText("");
			TempText.LoadString(IDS_Youcannotchangethiswhilelearning);
			//TempNoticeDialog.vNoticeText = "\n\n\nYou can not change this while learning.";
			TempNoticeDialog.vNoticeText = TempText;
			TempNoticeDialog.vType = cErrorMessage;
			TempNoticeDialog.DoModal();
		}
		else
		{

			if ((vLocalConfigurationData->vAutoImproveEnabled) || (PasswordOK(cAdvancedSetupPassword, true)))
			{
				if (vLocalConfigurationData->vNumberOfQuickLearnImagesToTake)
				{
					CYesNoDialog TempYesNoDialog;
					TempYesNoDialog.vNoticeText =  _T("\n\n\nEnable Normal Learn?");
					TempYesNoDialog.vYesButtonText = "Enable Normal";
					TempYesNoDialog.vNoButtonText = "Keep Quick Learn";
					TempYesNoDialog.vQuestionType = cConfirmQuestion;
					int TempResult = TempYesNoDialog.DoModal();
					if (TempResult == IDOK)
					{
						vLocalConfigurationData->vNumberOfQuickLearnImagesToTake = 0;
						UpdateButtonText();
						DisplayWithPassword();
					}
				}
				else
				{
					CYesNoDialog TempYesNoDialog;
					TempYesNoDialog.vNoticeText =  _T("\n\nEnable Quick Learn?\nNormal Learn is more sensitive");
					TempYesNoDialog.vYesButtonText = "Keep Normal";
					TempYesNoDialog.vNoButtonText = "Enable Quick Learn";
					TempYesNoDialog.vQuestionType = cConfirmQuestion;
					int TempResult = TempYesNoDialog.DoModal();
					if (TempResult != IDOK)
						if (!vLocalConfigurationData->vNumberOfQuickLearnImagesToTake)
							vLocalConfigurationData->vNumberOfQuickLearnImagesToTake = 100;
				}
				if (vLocalConfigurationData->vNumberOfQuickLearnImagesToTake)
				{
					//Quick learn parameters
					CNumericEntryDialog INumericEntryDialog;  
					
					//Set dialog box data titles and number value
					INumericEntryDialog.vEditString = dtoa(vLocalConfigurationData->vNumberOfQuickLearnImagesToTake,0);

					INumericEntryDialog.m_DialogTitleStaticText1 = "Number of Images for Quick Learn";
					INumericEntryDialog.m_DialogTitleStaticText2 = "Default Value: 10 or 100";
					INumericEntryDialog.m_DialogTitleStaticText3 = "0 = Disable Quick Learn, Use Normal Learn";
					INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + INumericEntryDialog.vEditString;

					INumericEntryDialog.m_UnitsString = "";
						INumericEntryDialog.vMaxValue = cMaximumQuickLearnImages;
					if (vLocalSystemData->vTakingQuickLearnImages) //cannot add more images while learning 
						INumericEntryDialog.vMaxValue = 
						vLocalConfigurationData->vNumberOfQuickLearnImagesToTake;
					INumericEntryDialog.vIntegerOnly = true;
					//Pass control to dialog box and display
					int nResponse = INumericEntryDialog.DoModal();
					//dialog box is now closed, if user pressed select do this
					//if user pressed cancel, do nothing
					if (nResponse == IDOK)
					{
						vLocalConfigurationData->vNumberOfQuickLearnImagesToTake = _wtoi(INumericEntryDialog.vEditString);
						if (PasswordOK(cTemporaryInspxPassword,false))
						{
							SetDlgItemText(IDC_SubFunction7Display, dtoa(vLocalConfigurationData->vNumberOfQuickLearnImagesToTake,0) + ", " + dtoa(vLocalConfigurationData->vQuickLearnNumberOfStructureDeltas,2));
							if (vLocalConfigurationData->vNumberOfQuickLearnImagesToTake)
							{
								INumericEntryDialog.vEditString = dtoa(vLocalConfigurationData->vQuickLearnNumberOfStructureDeltas,2);
								if (vLocalSystemData->vTakingQuickLearnImages) //Update number needed if learning 
									vTotalToLearn = vLocalConfigurationData->vNumberOfQuickLearnImagesToTake;

								INumericEntryDialog.m_DialogTitleStaticText1 = "";
								INumericEntryDialog.m_DialogTitleStaticText2 = "Number of Standard Deviations for Structure Depth";
								INumericEntryDialog.m_DialogTitleStaticText3 = "Default Value: 3.0";
								INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + INumericEntryDialog.vEditString;

								INumericEntryDialog.m_UnitsString = "";
								INumericEntryDialog.vMaxValue = 100;
								INumericEntryDialog.vIntegerOnly = false;
								//Pass control to dialog box and display
								int nResponse = INumericEntryDialog.DoModal();
								//dialog box is now closed, if user pressed select do this
								//if user pressed cancel, do nothing
								if (nResponse == IDOK)
								{
									vLocalConfigurationData->vQuickLearnNumberOfStructureDeltas = ATOF(INumericEntryDialog.vEditString);
									SetDlgItemText(IDC_SubFunction7Display, dtoa(vLocalConfigurationData->vNumberOfQuickLearnImagesToTake,0) + ", " + dtoa(vLocalConfigurationData->vQuickLearnNumberOfStructureDeltas,2));
								}
								else 
								if (nResponse == 10)
								{
									//Main Menu button pressed
									ExitAutoSetupDialog(10);
								}
							}
						}
						else
							SetDlgItemText(IDC_SubFunction7Display, dtoa(vLocalConfigurationData->vNumberOfQuickLearnImagesToTake,0));

						UpdateButtonText();
						DisplayWithPassword();
					}
					else 
					if (nResponse == 10)
					{
						//Main Menu button pressed
						ExitAutoSetupDialog(10);
					}
				}
			}
		}
	}
	else
		/*
	if (vMode == cEdgeExposureMenu)
	{		//Make Edges A Little Darker
		if (vLocalSystemData->vInAutoSetup)
		{
			if (vGlobalCurrentProduct->vXRayIntegrationTimeAtEdge > 205)  //shortest integration time possible
			{
				WORD TempOnePercentStep = (vGlobalCurrentProduct->vXRayIntegrationTime - 205) / 100;
				if (TempOnePercentStep < 1)
					TempOnePercentStep = 1;
				if (vGlobalCurrentProduct->vXRayIntegrationTimeAtEdge > 205 + TempOnePercentStep)
					vGlobalCurrentProduct->vXRayIntegrationTimeAtEdge = vGlobalCurrentProduct->vXRayIntegrationTimeAtEdge - TempOnePercentStep;
				else
					vGlobalCurrentProduct->vXRayIntegrationTimeAtEdge = 205;

				DisplayEdgeExposureTimePercent();

				//variable Integration Time temp can't do as will mess something up in the FPGA or uC, so reload all settings
				vMainWindowPointer->SendProductIntegrationTime(vGlobalCurrentProduct->vXRayIntegrationTime, 
					vGlobalCurrentProduct->vXRayIntegrationTimeAtEdge);

				//vMainWindowPointer->StopRunning(true);

				//int TimerResult = SetTimer(vStartSetupTimerHandle,1000,NULL);
				//if (!TimerResult)
				//	ReportErrorMessage("Error-Start Timer Failed",cEMailInspx,32000);
			}
			else
				GiveMessageAlreadyAtThisValue(1); //1 = minimum value
		}
		else
			YouCantChangeSettingBecauseInspecting();
	}
	else
	*/
	if (vMode == cCenterExposureMenu)
	{		//show Conveyor speed/exposure chart
		//if ((PasswordOK(cTemporaryInspxPassword,false)) || (PasswordOK(cAdvancedSetupPassword,false)))
		if (((vLocalConfigurationData->vScanTracType != cFermataScanTrac) && (vLocalConfigurationData->vScanTracType != cLegatoScanTrac)) ||//Not  a Pipeline
			(PasswordOK(cSuperInspxPassword,false)))
		if (((!vLocalSystemData->vXRayOnNotice) || (vStartingUpSource) || (vRestartingXRaysWithNewSettings) ||
			((vLocalSystemData->vSystemRunMode != cRunningSystemMode) && (vLocalSystemData->vSystemRunMode != cAutoSetupRunningSystem))) &&
			(vMainWindowPointer->InterlocksOK()) && (vStartedSource))
			StartingSystemPleaseTryAgain();
		else
		{
			bool TempByPassDeletingLearn = false;
			if (vLocalSystemData->vInAutoSetup)
			{
				if (NoSetupOrOKToChange())
				{
					/*
					if (PasswordOK(cTemporaryInspxPassword,false))
					{
						if (vGlobalCurrentProduct->ProductHasImageFiles(vGlobalCurrentDirectory))
						{
							CYesNoDialog TempYesNoDialog;
							TempYesNoDialog.vNoticeText = "This product has been Learned.\nIf you change this setting it may effect the Learn.\nDo you want to keep it regardless?";
							TempYesNoDialog.vYesButtonText = "Keep Learn";
							TempYesNoDialog.vQuestionType = cConfirmQuestion;
							int TempResult = TempYesNoDialog.DoModal();
							if (TempResult == IDOK)
								TempByPassDeletingLearn = true;
						}
					}
					*/

					int nResponse = 0;
					CString TempText1 = "";
					CString TempText = "";
					//double TempOriginalEdgeExposurePercentage = ((vGlobalCurrentProduct->vXRayIntegrationTimeAtEdge - 205.0) / 
					//	(vGlobalCurrentProduct->vXRayIntegrationTime - 205.0));
					if (PasswordOK(cTemporaryInspxPassword,false))
					{
						CNumericEntryDialog INumericEntryDialog;  
						
						//Set dialog box data titles and number value
						double TempUserIntegrationTime =((double)vGlobalCurrentProduct->vXRayIntegrationTime / 2500.0);
						INumericEntryDialog.vEditString = dtoa(TempUserIntegrationTime,2);

						INumericEntryDialog.m_DialogTitleStaticText1 = "Enter Product Integration Time";
						INumericEntryDialog.m_DialogTitleStaticText2 = "For best results set so get 80-92% of ";
						INumericEntryDialog.m_DialogTitleStaticText3 = "Maximum Conveyor Speed, but not over 2 milli-Seconds";
						INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + INumericEntryDialog.vEditString;

						if (vGlobalFPGAVersion10Point0OrHigher)
						{
						INumericEntryDialog.m_UnitsString = "Default: 0.3, Minimum: 0.082 Milli-Seconds";
						INumericEntryDialog.vMinValue = 0.082;
						}
						else
						{
							INumericEntryDialog.m_UnitsString = "Default: .3, Minimum: .3 Milli-Seconds";
							INumericEntryDialog.vMinValue = .3;
						}
						INumericEntryDialog.vMaxValue = 10;
						INumericEntryDialog.vIntegerOnly = false;
						//Pass control to dialog box and display
						nResponse = INumericEntryDialog.DoModal();
						//dialog box is now closed, if user pressed select do this
						//if user pressed cancel, do nothing
						if (nResponse == IDOK)
						{
							TempUserIntegrationTime = ATOF(INumericEntryDialog.vEditString);
							vGlobalCurrentProduct->vXRayIntegrationTime = (WORD)(TempUserIntegrationTime * 2500);
							vMainWindowPointer->CalculatePipelineEncoderAndTrigger();
						}
					}
					else
					{
						CSelectItemDialog ISelectItemDialog;  
						ISelectItemDialog.vNumberOfItems = 56;
						ISelectItemDialog.vTitleString = "Select Exposure Time";
						ISelectItemDialog.vBackgroundColor = cGreen;

						//if (vLocalConfigurationData->vUseMetric)
						//	TempText1 = "M/Min";
						//else
						//	TempText1 = "Ft/Min";

						for (BYTE TempLoop = 0; TempLoop < ISelectItemDialog.vNumberOfItems; TempLoop++)
						{
							double TempIntegrationTime = (TempLoop * 0.05) + 0.3;

							ISelectItemDialog.vItems[TempLoop] = Pad(dtoa(TempIntegrationTime,2),2,1) + " (mSec)";
						}
						ISelectItemDialog.m_DialogTitleStaticText2 = "";
						ISelectItemDialog.m_DialogTitleStaticText3 = "";
						ISelectItemDialog.vInAutoSetup = vLocalSystemData->vInAutoSetup;
						//Pass control to dialog box and display
						nResponse = ISelectItemDialog.DoModal();
						if (nResponse == IDOK)
						{
							//convert dialog answer to new style integration time code
							vGlobalCurrentProduct->vXRayIntegrationTime = ((ISelectItemDialog.vItemNumberSelected - 1) + 6) * 125;
							vMainWindowPointer->CalculatePipelineEncoderAndTrigger();
						}
					}
					//dialog box is now closed, if user pressed select do this
					//if user pressed cancel, do nothing
					if (nResponse == IDOK)
					{
						double TempIntegrationTime = vGlobalCurrentProduct->vXRayIntegrationTime * .0004;

						TempText.LoadString(IDS_mSec);
						SetDlgItemText(IDC_SubFunction7Display,dtoa(TempIntegrationTime,2) + " " + TempText);
						SetChangeMade();

						if ((!TempByPassDeletingLearn) &&	(vLocalSystemData->vInAutoSetup))
						{
							vLocalSystemData->vITIPCDig->RenameLearnFilesToBackup(vGlobalCurrentProduct);
							vLocalSystemData->vITIPCDig->DeleteLearnImages();
							vLocalSystemData->vITIPCDig->DeleteLearnFiles(vGlobalCurrentProduct);
							ClearDensityFeedBackLoop();
						}
						vNeedToSetContainerThresholdBounds = 2;

						//Send Integration Time
						vMainWindowPointer->SendProductIntegrationTime(vGlobalCurrentProduct->vXRayIntegrationTime);// must do before updating maximum conveyor speed as it calculates maximum conveyor rate

						vGlobalCurrentProduct->vMaxConveyorSpeedAsEntered = ConvertIntegrationTimeToMaxConveyorSpeed(vGlobalCurrentProduct->vXRayIntegrationTime);
						ShowMaximumConveyorSpeed();

						if (vGlobalFPGAVersion11Point0OrHigher)
						{ //don't need to re-calibrate so just re-load settings
							//ChangedProductSettingsSoReload();
							if ((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode)) //Continuous Feed, or a Pipeline
								SetStatusLine(" Waiting for Image.");
							else
								SetStatusLine(" Send Container Through ScanTrac.");
						}
						else
						{ //old system need to re-calibrate the detectors
							SetStatusLine(" Please wait, calibrating");
							vStartingUpSource = true;
							vRestartingXRaysWithNewSettings = true;
							vMainWindowPointer->StopRunning(true);

							int TimerResult = SetTimer(vStartSetupTimerHandle,1000,NULL);
							if (!TimerResult)
								ReportErrorMessage("Error-Start Timer Failed",cEMailInspx,32000);
						}
					}
				}
			}
			else
			{
				CNoticeDialog TempNoticeDialog;
				CString TempText;
				TempText.LoadString(IDS_Youcannotchangeimageexposurewhileinspecting);
				//TempNoticeDialog.vNoticeText = "\n\nYou can not change image exposure while inspecting.\nYou must stop running on the Main Menu,\nthen Edit the Product Exposure";
				TempNoticeDialog.vNoticeText = TempText;
				TempNoticeDialog.vType = cErrorMessage;
				TempNoticeDialog.DoModal();
			}
		}
	}
	else
	if (vMode == cSetImageSize) //sF7
	{
		//setup Container trigger to detector distance button pressed
		if (!vGlobalCurrentProduct->vMultiLaneNumberOfLanes) //don't do if in Multi Lane
		if ((PasswordOK(cCertifiedPassword,false)) || ((!vLocalConfigurationData->vContinuousFeedContainerRate) && (!vLocalConfigurationData->vBulkProductMode) && (!vGlobalCurrentProduct->vBulkProductMode))) //Not Continuous Feed, Not a Pipeline or password in
		if (NoSetupOrOKToMinorChange())
		{
			CNumericEntryDialog INumericEntryDialog;  

			double TempWhiteSpaceOnOneSideOfImage = (vGlobalCurrentProduct->vProductImageWidth / 1.6) * 0.3;

			if (TempWhiteSpaceOnOneSideOfImage > 4)
				TempWhiteSpaceOnOneSideOfImage = 4;
			
			double TempTriggerToBeamOffset = vGlobalCurrentProduct->vProductBodyTriggerToImageDistanceInInches;

			if (vLocalConfigurationData->vSystemBodyTriggerToXRayBeam > 1)
				TempTriggerToBeamOffset = -(vLocalConfigurationData->vSystemBodyTriggerToXRayBeam - vGlobalCurrentProduct->vProductBodyTriggerToImageDistanceInInches - TempWhiteSpaceOnOneSideOfImage);

			//Set dialog box data titles and number value
			INumericEntryDialog.vEditString = dtoa(TempTriggerToBeamOffset,2);

			CString TempText("");
			if (vGlobalScanTracType == cBriosoScanTrac)
				INumericEntryDialog.m_DialogTitleStaticText4 = "Larger numbers move container up";
			else
				INumericEntryDialog.m_DialogTitleStaticText4 = "Larger numbers move container to the right";

			if (vLocalConfigurationData->vSystemBodyTriggerToXRayBeam > 1)
				INumericEntryDialog.m_DialogTitleStaticText1 = "Trigger to X-Ray Beam OFFSET (normally zero)";
			else 
			{
				TempText.LoadString(IDS_ContainerTriggerToXRayDetectorDistance);
				INumericEntryDialog.m_DialogTitleStaticText1 = TempText;
			}
			
			TempText.LoadString(IDS_Product);
			INumericEntryDialog.m_DialogTitleStaticText2 = TempText + ": " + vProductEditingName;
			TempText.LoadString(IDS_OriginalValue);
			INumericEntryDialog.m_DialogTitleStaticText3 = TempText + ": " + INumericEntryDialog.vEditString;
			if (vLocalConfigurationData->vSystemBodyTriggerToXRayBeam > 1)
				INumericEntryDialog.m_DialogTitleStaticText3 = "Default Value: near zero on calibrated systems";

			TempText.LoadString(IDS_MeasuredIn);
			INumericEntryDialog.m_UnitsString = TempText + " " + vLocalConfigurationData->vUnitsString;
			//INumericEntryDialog.vMaxValue = cMaxBeltPositionOffset / (vGlobalPixelsPerUnit *
			//	vGlobalCurrentProduct->vOverScanMultiplier);
			INumericEntryDialog.vMaxValue = 100;
			INumericEntryDialog.vMinValue = 0;
			if (vLocalConfigurationData->vSystemBodyTriggerToXRayBeam > 1)
			{
				INumericEntryDialog.vMinValue = -30;
				INumericEntryDialog.vAllowNegative = true;
					INumericEntryDialog.vWarnIfLargerThan4 = true;
			}
			else
			{
				INumericEntryDialog.vMinValue = 0;
				INumericEntryDialog.vAllowNegative = false;
			}

			//if (vLocalConfigurationData->vUseMetric)
			//{
			//	INumericEntryDialog.vMinValue = 2 * (float)25.4;
			//	INumericEntryDialog.vMaxValue = 30 * (float)25.4;
			//}
			INumericEntryDialog.vIntegerOnly = false;
			//Pass control to dialog box and display
			int nResponse = INumericEntryDialog.DoModal();
			//dialog box is now closed, if user pressed select do this
			//if user pressed cancel, do nothing
			if (nResponse == IDOK)
			{
				//if (PasswordOK(cSuperInspxPassword,false)) 
				//{
				//	vGlobalCurrentProduct->SetProductBodyTriggerToImageBeltPositionOffset((float)ATOF(INumericEntryDialog.vEditString));
				//	SetDlgItemText(IDC_SubFunction7Display, dtoa(vGlobalCurrentProduct->vProductBodyTriggerToImageDistanceInInches,2));
				//}
				//else
				{
					double TempOffset = ATOF(INumericEntryDialog.vEditString);

					double TempDistance = TempOffset;

					if (vLocalConfigurationData->vSystemBodyTriggerToXRayBeam > 1)
						TempDistance = vLocalConfigurationData->vSystemBodyTriggerToXRayBeam - TempWhiteSpaceOnOneSideOfImage + TempOffset;

					if (TempDistance < 4)
					{
						CNoticeDialog TempNoticeDialog;
						TempNoticeDialog.vNoticeText = "\n\n\nEntry below minimum distance.\nSetting to Minimum Distance.";
						TempNoticeDialog.vType = cErrorMessage;
						TempNoticeDialog.DoModal();
					}

					vGlobalCurrentProduct->SetProductBodyTriggerToImageBeltPositionOffset((float)TempDistance);
					DisplayWithPassword();
				}
				SetChangeMade();

				vGlobaluCSerialPort->SendContainerTriggerToImageDelay(vGlobalCurrentProduct);
				//vNeedToSetSize = false;
				vMainWindowPointer->CheckEjectorsConfigured(vGlobalCurrentProduct, 7);

				if (vLocalSystemData->vInAutoSetup)
				{
					vLocalSystemData->vChangedSizeOfProduct = true;
					vLocalSystemData->vChangedSizeOfProductCheckDensityROI = 1;
				}
				vGlobalCurrentProduct->CheckAllROIs(vLocalSystemData->vITIPCDig->vProductAverageImage);
			}
			else 
			if (nResponse == 10)
			{
				//Main Menu button pressed
				ExitAutoSetupDialog(10);
			}
		}
	}
	else
	if ((vMode == cSetContaminantThreshold) || (vMode == cSetVoidThreshold))
	{
		//ReportErrorMessage("Left Button Down IN SUBFUNCTION 7",cEMailInspx, 32000);
		//if ((vMouseState == 0) || ((vMouseState > 8) && (vMouseState < 17)))	//if mouse is idle or in double click toggle on mode
		//{
		//	vMouseState = 7;	//set mouse to on while pressed down
		//	int TimerResult = SetTimer(vMouseDownRepeatTimerHandle,200,NULL);
		//	if (!TimerResult)
		//		ReportErrorMessage("Error-Second Timer Failed",cEMailInspx,32000);

		//}
		
		if (vInspectionEditing)
		if (vInspectionEditing->vInspectionType != cJarBottomContaminantInspection)
		if ((vInspectionEditing->vFindBadIntensity) || (PasswordOK(cAdvancedSetupPassword,false)))
		{
			if (vInspectionEditing->vFindBadIntensity)
			{  //finding bad so abort
				WriteCSVFiles();
				CString TempText = "Finished Evaluate Bad";
				if (vLocalSystemData->vMaximumBadIndex < 5)
					TempText = "Abort Evaluate Bad";
				if (vInspectionEditing)
					TempText = TempText + ": " + vInspectionEditing->vName;

				SetStatusLine(" " + TempText);
			}
			else
			if ((vOldAverageEnabled) || (vOldTakingQuickLearnImages) || (vGlobalProcessQuickLearnImagesThread) ||
				(vOldStructureEnabled) || (vOldCalibrateBodyTriggerReferenceEnabled))
			{
				CNoticeDialog TempNoticeDialog;
				TempNoticeDialog.vNoticeText = "\n\nScanTrac is Learning.\nScanTrac can not evaluate while learning.";
				TempNoticeDialog.vType = cErrorMessage;
				TempNoticeDialog.DoModal();
			}
			else
			{
				vAnyChangeMade = true;
				WriteCSVFiles();
				vLocalSystemData->vMaximumBadIndex = 0;

				vLocalSystemData->vSendingGoodWithBad = false;
				vInspectionEditing->vFindBadIntensity = true;

				m_Reference.ShowWindow(SW_SHOW);
				SetDlgItemText(IDC_Reference, _T("0"));

				if (vLocalSystemData->vInAutoSetup)
				if (!vShowReturnToEditMenu)
					vMainWindowPointer->ResetCounters();
				vOldMaximumBadIntensity = 0;
				vOldMaximumBadCount = 0;
				vOldMaximumBadStandardDeviation = 0;
				vLocalSystemData->vMaximumBadIntensity = 0;
				vLocalSystemData->vMaximumBadStandardDeviation = 0;
				vLocalSystemData->vMaximumBadCount = 0;
				vLocalSystemData->vMaximumBadIndex = 0;
				vLocalSystemData->vQualityFactor = 0;
				vLocalSystemData->vMaximumBadAverage = 0;
				vHaveAnAutoThresholdValue = false;

				CString TempText("");
				CString TempText1("");
				TempText1.LoadString(IDS_Product);
				TempText.LoadString(IDS_Max);
				SetDlgItemText(IDC_SubFunction8Button3,TempText + ": _");
				TempText.LoadString(IDS_Avg);
				SetDlgItemText(IDC_SubFunction8Button1,TempText + ": _");
				TempText.LoadString(IDS_SD);
				SetDlgItemText(IDC_SubFunction8Button2,TempText + ": _");

				if (!vLocalSystemData->vITIPCDig->vDriftCompensationLocked)
				{
					if ((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode))
						SetStatusLine(" Waiting for lock, Send Good Product...");
					else
					SetStatusLine(" Waiting for lock, Send good containers...");
					SetDlgItemText(IDC_SubFunction8Button3, _T("Evaluating"));

					GiveSendGoodContainersNotice();
				}
				else
					SetStatusLine(" Evaluating, Send Bad Containers... ");

				vLocalSystemData->vFrameCount = 0;
				TempText.LoadString(IDS_None);
				SetDlgItemText(IDC_Width2,TempText);
				m_SubFunction8Button.ShowWindow(SW_HIDE);
					this->InvalidateRect(&m_SubFunction8Button.GetRectToInvalidateArrow());
				m_SubFunction8Button1.ShowWindow(SW_SHOW);
				m_SubFunction8Button2.ShowWindow(SW_SHOW);
				m_SubFunction8Button3.ShowWindow(SW_SHOW);
				m_SubFunction7Display.ShowWindow(SW_SHOW);
			}
		}
		else
		{
			if (PasswordOK(cAdvancedSetupPassword, true))
				DisplayWithPassword();
		}
		UpdateButtonText();
		UpdateFunction3Button();
	}
	else
	if (vMode == cSetROI)
	{  //move right, left one pixel
		if (!vRecievedButtonPress)
		{
			//ReportErrorMessage("Left Mouse Button Down IN SUBFUNCTION 7",cAction, 32000);
			if ((vMouseState == 0) || ((vMouseState > 8) && (vMouseState < 17)))	//if mouse is idle or in double click toggle on mode
			{
				vMouseState = 7;	//set mouse to on while pressed down
				int TimerResult = SetTimer(vMouseDownRepeatTimerHandle,200,NULL);
				if (!TimerResult)
					ReportErrorMessage("Error-Second Timer Failed",cEMailInspx,32000);
			}
		}
		
		if (vInspectionEditing)
		if (DeleteCheckWeighCalibrations(true))
		{
			/*
			bool TempOKToMoveROI = true;
			if ((vInspectionEditing->vInspectionType == cGlass13mmContaminantInspection) && (vInspectionEditing->vTrimEdgesOfJarAtROI))
			{
				CYesNoDialog TempYesNoDialog;
				TempYesNoDialog.vNoticeText = "\nGlass ROI should be at the edge of the container.\nAre you sure you want to move it?";
				TempYesNoDialog.vYesButtonText = "Leave As Is";
				TempYesNoDialog.vNoButtonText = "Move ROI";
				TempYesNoDialog.vQuestionType = cConfirmQuestion;
				int TempResult = TempYesNoDialog.DoModal();
				if (TempResult == IDOK)
					TempOKToMoveROI = false;
			}

			if (TempOKToMoveROI)
			*/
			if (vInspectionEditing->vROIPixelRight > vInspectionEditing->vROIPixelLeft + 2)
			{
				vInspectionEditing->vROIPixelRight--; 
				SetROILines();
			}
			else
			if (vInspectionEditing->vROIPixelRight == 0)
			{
				vInspectionEditing->SetROIRight(vGlobalCurrentProduct->vProductImageWidth, vGlobalCurrentProduct->vOverScanMultiplier);
				SetROILines();
			}
		}
	}
	else
	if (vMode == cSetDensityThreshold)
	{  //stop or start averaging density data
		
		if (vInspectionEditing)
		if (vInspectionEditing->vInspectionNameForWeightTrendingPointer) //if wieght trending inspection, use other inspections settings
			GiveLinkedInspectionNotice();
		else
		{
			WriteCSVFiles();
			if ((vInspectionEditing->vCollectingDensityAverage) && (vCollectingBadDensityAverage))
			{ //stop collecting bad density
				vInspectionEditing->vCollectingDensityAverage = false;
				vCollectingBadDensityAverage = false;
				CString TempText("");
				//TempText.LoadString(IDS_PressStartButtonWhenImagesLookGood);
				TempText = "Finished Evaluate Bad";
				if (vLocalSystemData->vDensityIndex < 5)
					TempText = "Abort Evaluate Bad";
				if (vInspectionEditing)
					TempText = TempText + ": " + vInspectionEditing->vName;
				SetStatusLine(" " + TempText);

				if (!vInspectionEditing->vSetupHistoryMaximumBadAverage)
				{
					m_SubFunction8Button1.ShowWindow(SW_HIDE);
					m_SubFunction8Button2.ShowWindow(SW_HIDE);
					m_SubFunction8Button3.ShowWindow(SW_HIDE);
					m_SubFunction7Display.ShowWindow(SW_HIDE);
					vInspectionEditing->vSetupHistoryBadMaximumValue = 0;
					vInspectionEditing->vSetupHistoryBadMinimumValue = 0;
					vLocalSystemData->vITIPCDig->vDensityMinimumValue = 255;
					if ((vInspectionEditing->vInspectionType == cUnderfillByWeightInspection) || (vInspectionEditing->vInspectionType == cOverfillByWeightInspection) || (vInspectionEditing->vInspectionType == cCheckWeighInspection))
						vLocalSystemData->vITIPCDig->vDensityMinimumValue = (float)0xFFFFFFFFFFFFFFFF;
					vLocalSystemData->vITIPCDig->vDensityMaximumValue = 0;
					TempText.LoadString(IDS_Min);
					CString TempString = dtoa(vInspectionEditing->vSetupHistoryBadMinimumValue,2);
					SetDlgItemText(IDC_SubFunction7Display,TempText + ": " + TempString);
		
					if ((vInspectionEditing->vCheckWeighMFactor != 1) || (vInspectionEditing->vCheckWeighBFactor))
						SetDlgItemText(IDC_SubFunction8Button3, "Min: " + dtoa(vInspectionEditing->vSetupHistoryBadMaximumValue,2));
					else
						SetDlgItemText(IDC_SubFunction8Button3, "Max: " + dtoa(vInspectionEditing->vSetupHistoryBadMaximumValue,2));
				}
				//else
				//	SetStatusLine(" " + vProductEditingName);
			}
			else
			if ((vOldAverageEnabled) || (vOldTakingQuickLearnImages) || (vGlobalProcessQuickLearnImagesThread) ||
				(vOldStructureEnabled) || (vOldCalibrateBodyTriggerReferenceEnabled))
			{
				CNoticeDialog TempNoticeDialog;
				TempNoticeDialog.vNoticeText = "\n\nScanTrac is Learning.\nScanTrac can not evaluate while learning.";
				TempNoticeDialog.vType = cErrorMessage;
				TempNoticeDialog.DoModal();
			}
			else
			{ //start collecting bad density
				bool TempOKToStart = true;
				if ((vInspectionEditing->vInspectionType == cCheckWeighInspection) || (vInspectionEditing->vInspectionType == cUnderfillByWeightInspection) || (vInspectionEditing->vInspectionType == cOverfillByWeightInspection))
				{
					if (!vInspectionEditing->vCheckWeighMaximumGoodWeight)
					{
						TempOKToStart = false;
						CNoticeDialog TempNoticeDialog;
						TempNoticeDialog.vNoticeText = "\n\n\nYou must enter Weight of Heavy Sample first";
						TempNoticeDialog.vType = cErrorMessage;
						TempNoticeDialog.DoModal();
					}
				}
				if (TempOKToStart)
				{
					if ((vInspectionEditing->vInspectionType == cCheckWeighInspection) || (vInspectionEditing->vInspectionType == cUnderfillByWeightInspection) || (vInspectionEditing->vInspectionType == cOverfillByWeightInspection))
					{
						CNoticeDialog TempNoticeDialog;
						TempNoticeDialog.vNoticeText = "\n\nSend your Heavy Weight Sample Container\nat least 5 times.\nWhen done, press Stop Sending Heavy";
						TempNoticeDialog.vType = cInstructionsMessage;
						TempNoticeDialog.DoModal();
					}
					vAnyChangeMade = true;
					vLocalSystemData->vDensityIndex = 0;
					vLocalSystemData->vFrameCount = 0;
					vLocalSystemData->vITIPCDig->vDensityMinimumValue = 255;
					if ((vInspectionEditing->vInspectionType == cUnderfillByWeightInspection) || (vInspectionEditing->vInspectionType == cOverfillByWeightInspection) || (vInspectionEditing->vInspectionType == cCheckWeighInspection))
						vLocalSystemData->vITIPCDig->vDensityMinimumValue = (float)0xFFFFFFFFFFFFFFFF;
					vLocalSystemData->vITIPCDig->vDensityMaximumValue = 0;
					vLocalSystemData->vITIPCDig->vDensityAverage = 0;
					vLocalSystemData->vITIPCDig->vDensityStandardDeviation = 0;
					vLocalSystemData->vQualityFactor = 0;
					vOldQualityFactor = 0;
					SetDlgItemText(IDC_Simulating, _T(""));

					CString TempText("");
					CString TempText1;
					TempText.LoadString(IDS_Min);
					SetDlgItemText(IDC_SubFunction7Display,TempText + ": _");
					TempText.LoadString(IDS_Max);
					SetDlgItemText(IDC_SubFunction8Button3,TempText + ": _");
					TempText.LoadString(IDS_Avg);
					SetDlgItemText(IDC_SubFunction8Button1,TempText + ": _");
					TempText.LoadString(IDS_SD);
					SetDlgItemText(IDC_SubFunction8Button2,TempText + ": _");
					if (!vLocalSystemData->vITIPCDig->vDriftCompensationLocked)
					{
						if ((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode))
							SetStatusLine(" Waiting for lock, Send Good Product...");
						else
							SetStatusLine(" Waiting for lock, Send Good Containers...");
						SetDlgItemText(IDC_SubFunction8Button3, _T("Evaluating"));

						GiveSendGoodContainersNotice();
					}
					else
					{
						if ((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode))
							TempText = "Send Good Product";
						else
							TempText.LoadString(IDS_SendGoodContainers);
						SetStatusLine(" " + TempText + ": " + vProductEditingName);
					}
					m_SubFunction8Button.ShowWindow(SW_HIDE);
					this->InvalidateRect(&m_SubFunction8Button.GetRectToInvalidateArrow());
					m_SubFunction8Button1.ShowWindow(SW_SHOW);
					m_SubFunction8Button2.ShowWindow(SW_SHOW);
					m_SubFunction8Button3.ShowWindow(SW_SHOW);
					m_SubFunction7Display.ShowWindow(SW_SHOW);

					vInspectionEditing->vSetupHistoryMaximumBadAverage = 0;
					vInspectionEditing->vSetupHistoryMaximumBadStandardDeviation = 0;
					vInspectionEditing->vSetupHistoryMaximumBadCount = 0;
					vInspectionEditing->vSetupHistoryMaximumBad = 0;
					vHaveAnAutoThresholdValue = false;
	
					if ((vInspectionEditing->vInspectionType == cCheckWeighInspection) || (vInspectionEditing->vInspectionType == cUnderfillByWeightInspection) || (vInspectionEditing->vInspectionType == cOverfillByWeightInspection))
					{
						if (vInspectionEditing->vDensityThresholdLower > 5000)
							vInspectionEditing->SetDensityLowerThreshold((float)vInspectionEditing->ConvertToWeight(vInspectionEditing->vDensityThresholdLower));
						if (vInspectionEditing->vDensityThresholdUpper > 5000)
							vInspectionEditing->vDensityThresholdUpper = (float)vInspectionEditing->ConvertToWeight(vInspectionEditing->vDensityThresholdUpper);

						vInspectionEditing->vCheckWeighMFactor = 1;
						vInspectionEditing->vCheckWeighBFactor = 0;
						vInspectionEditing->vCheckWeighOffset = 0;
					}

					vCollectingBadDensityAverage = true;
					vInspectionEditing->vCollectingDensityAverage = true;
					TempText.LoadString(IDS_CollectingDensitySamples);
					if (!vLocalSystemData->vITIPCDig->vDriftCompensationLocked)
					{
						SetStatusLine(" Waiting for lock, Send Good Containers...");
						SetDlgItemText(IDC_SubFunction8Button3, _T("Evaluating"));
					}
					else
					 SetStatusLine(" " + TempText + ": " + vProductEditingName);
				}
			}
			UpdateButtonText();
			DisplayMinimumGoodMaximumBadDataValues(false);
			DisplayAutoDensityThresholdLower();
			DisplayAutoDensityThresholdUpper();
			DisplayDensityThresholdLower();
			DisplayDensityThresholdUpper();
			UpdateFunction3Button();
		}
	}
	CWnd *TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText1);
	if (TempWindow)
		TempWindow->Invalidate(false);
}

void CAutoSetupImageDisplayDialog::OnSubFunction8Button() 
{
	if (vInitialized)
	if (vMode == CMultiLanePositionAdjustment)
	{
		if (vGlobalCurrentProduct->vMultiLaneWidthInPixels[vMultiImageLaneEditing] > 4)
		{
			vGlobalCurrentProduct->vMultiLaneWidthInPixels[vMultiImageLaneEditing]--;
			UpdateButtonText();
			WaitThenDrawImageOnScreen();
		}
		else
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\n\nLane at minimum width";
			TempNoticeDialog.vType = cErrorMessage;
			TempNoticeDialog.DoModal();
		}
	}
	else
	if (vMode == cSetSizeSettings)
	{
		if (!vRecievedButtonPress)
		{
			//ReportErrorMessage("Left Mouse Button Down IN SUBFUNCTION 8",cAction, 32000);
			if ((vMouseState == 0) || ((vMouseState > 8) && (vMouseState < 17)))	//if mouse is idle or in double click toggle on mode
			{
				vMouseState = 8;	//set mouse to on while pressed down
				int TimerResult = SetTimer(vMouseDownRepeatTimerHandle,200,NULL);
				if (!TimerResult)
					ReportErrorMessage("Error-Second Timer Failed",cEMailInspx,32000);
			}
		}
		
		if (vAdjustingBottomEdgeFinding)
		{
			if (vGlobalCurrentProduct->vBottomLocationLeftPixel > 0)
			{
				vChangedPurpleLine = true;
				vGlobalCurrentProduct->vBottomLocationLeftPixel--;
				vGlobalCurrentProduct->SetBottomLocationLeft((float)(vGlobalCurrentProduct->vBottomLocationLeftPixel / vGlobalPixelsPerUnit));

				SetChangeMade();
				UpdateButtonText();

				vNeedToSetContainerThresholdBounds = 1;

				//9/10/2009
				if (!vHaveAnImage)
				if (!vGaveSendAContainerNotice)
				{
					vMouseState = 0;
					vGaveSendAContainerNotice = true;
					CNoticeDialog TempNoticeDialog;
					TempNoticeDialog.vNoticeText = "\n\n\nSend another container through\nto update the screen";
					TempNoticeDialog.vType = cInformationMessage;
					TempNoticeDialog.DoModal();
				}
				CalculateLearnedReferenceValues();

				WaitThenDrawImageOnScreen();
				WaitDisplayLocationEdge();
			}
		}
		else
		{
			if (vGlobalCurrentProduct->vEdgeLocationBottomPixel > 0)
			{
				vChangedPurpleLine = true;
				vGlobalCurrentProduct->vEdgeLocationBottomPixel--;
				vGlobalCurrentProduct->SetEdgeLocationBottom((float)(vGlobalCurrentProduct->vEdgeLocationBottomPixel / vGlobalPixelsPerUnitInHeight));

				SetChangeMade();
				UpdateButtonText();

				vNeedToSetContainerThresholdBounds = 1;

				//9/10/2009
				if (!vHaveAnImage)
				if (!vGaveSendAContainerNotice)
				{
					vMouseState = 0;
					vGaveSendAContainerNotice = true;
					CNoticeDialog TempNoticeDialog;
					TempNoticeDialog.vNoticeText = "\n\n\nSend another container through\nto update the screen";
					TempNoticeDialog.vType = cInformationMessage;
					TempNoticeDialog.DoModal();
				}
				CalculateLearnedReferenceValues();

				WaitThenDrawImageOnScreen();
				WaitDisplayLocationEdge();
			}
		}
	}
	else
	if (vMode == cSetBackground)
	{
		if (vLocalSystemData->vITIPCDig->vProductBackgroundImage)
		{
			if (vShowingBackground)
				vShowingBackground = false;
			else
				vShowingBackground = true;
			UpdateButtonText();
			WaitThenDrawImageOnScreen();
		}
	}
	else
			/*
	if (vMode == cEdgeExposureMenu)
	{		//Make Edges A Lot Darker
		if (vLocalSystemData->vInAutoSetup)
		{
			if (vGlobalCurrentProduct->vXRayIntegrationTimeAtEdge > 205)  //shortest integration time possible
			{
				WORD TempTenPercentStep = (vGlobalCurrentProduct->vXRayIntegrationTime - 205) / 10;
				if (TempTenPercentStep < 1)
					TempTenPercentStep = 1;
				if (vGlobalCurrentProduct->vXRayIntegrationTimeAtEdge > 205 + TempTenPercentStep)
					vGlobalCurrentProduct->vXRayIntegrationTimeAtEdge = vGlobalCurrentProduct->vXRayIntegrationTimeAtEdge - TempTenPercentStep;
				else
					vGlobalCurrentProduct->vXRayIntegrationTimeAtEdge = 205;

				DisplayEdgeExposureTimePercent();
				
				//variable Integration Time temp can't do as will mess something up in the FPGA or uC, so reload all settings
				vMainWindowPointer->SendProductIntegrationTime(vGlobalCurrentProduct->vXRayIntegrationTime);//, 
					//vGlobalCurrentProduct->vXRayIntegrationTimeAtEdge);

				//vMainWindowPointer->StopRunning(true);

				//int TimerResult = SetTimer(vStartSetupTimerHandle,1000,NULL);
				//if (!TimerResult)
				//	ReportErrorMessage("Error-Start Timer Failed",cEMailInspx,32000);
			}
			else
				GiveMessageAlreadyAtThisValue(1); //1 = minimum value
		}
		else
			YouCantChangeSettingBecauseInspecting();
	}
	else
	*/
	if (vMode == cCenterExposureMenu)
	{		
	}
	else
	if (vMode == cSetDensityThreshold)
	{  //show histogram of density data
		
		if (vInspectionEditing)
		if (vInspectionEditing->vInspectionNameForWeightTrendingPointer) //if wieght trending inspection, use other inspections settings
			GiveLinkedInspectionNotice();
		else
		if ((vLocalSystemData->vDensityDataD) && (vLocalSystemData->vDensityIndex))
		{
			double *TempData = NULL;
			if ((vInspectionEditing->vCheckWeighMFactor != 1) || (vInspectionEditing->vCheckWeighBFactor != 0))
			{
				if (ThereIsEnoughMemory(vLocalSystemData->vDensityIndex * 8, "Temp Plot buffer"))
				{
					TempData = (double *)malloc(vLocalSystemData->vDensityIndex * 8);
					if (TempData)
					{
						double *TempSourcePointer = vLocalSystemData->vDensityDataD;
						double *TempDestinationPointer = TempData;
						for (DWORD TempLoop = 0; TempLoop < vLocalSystemData->vDensityIndex; TempLoop++)
						{
							*TempDestinationPointer++ = vInspectionEditing->ConvertToWeight(*TempSourcePointer++);
						}
					}
				}
			}
			CHistogramDisplayDialog IHistogramDisplayDialog;
			IHistogramDisplayDialog.vNumberOfNumbers = vLocalSystemData->vDensityIndex;
			IHistogramDisplayDialog.vValuesAreFloats = true;

			if ((vInspectionEditing) && (vInspectionEditing->vInspectionType == cStandardDeviationInspection))
				IHistogramDisplayDialog.vDataTitle = "Texture in ROI";
			else
			if (TempData)
				IHistogramDisplayDialog.vDataTitle = "Weight in ROI";
			else
				IHistogramDisplayDialog.vDataTitle = "Density in ROI";

			IHistogramDisplayDialog.vPointerToFloats = vLocalSystemData->vDensityDataD;
			if (TempData)
				IHistogramDisplayDialog.vPointerToFloats = TempData;

			IHistogramDisplayDialog.vLocalSystemData = vLocalSystemData;
			IHistogramDisplayDialog.vLocalConfigurationData = vLocalConfigurationData;
			IHistogramDisplayDialog.vMainWindowPointer = vMainWindowPointer;
			int nResponse = IHistogramDisplayDialog.DoModal();

			if (TempData)
				free(TempData);

			if (nResponse == 10)
			{
				//Main Menu button pressed
				Sleep(1); //is the equivelent of a yeild statement;
				ExitAutoSetupDialog(10);
			}
		}
	}
	else
	if (vMode == cSetImageSize) //sF8
	{
		//ReportErrorMessage("Left Button Down IN SUBFUNCTION 8",cEMailInspx, 32000);
		//if ((vMouseState == 0) || ((vMouseState > 8) && (vMouseState < 17)))	//if mouse is idle or in double click toggle on mode
		//{
		//	vMouseState = 8;	//set mouse to on while pressed down
		//	int TimerResult = SetTimer(vMouseDownRepeatTimerHandle,200,NULL);
		//	if (!TimerResult)
		//		ReportErrorMessage("Error-Second Timer Failed",cEMailInspx,32000);
		//}
		
		if ((PasswordOK(cAdvancedSetupPassword,false)) || ((!vLocalConfigurationData->vContinuousFeedContainerRate) && (!vLocalConfigurationData->vBulkProductMode) && (!vGlobalCurrentProduct->vBulkProductMode))) //Not Continuous Feed, Not a Pipeline or password in
		if (!vLocalSystemData->vInAutoSetup)
		{
			YouCantChangeSettingBecauseInspecting();
		}
		else
		if (((!vLocalSystemData->vXRayOnNotice) || (vStartingUpSource) || (vRestartingXRaysWithNewSettings) ||
			((vLocalSystemData->vSystemRunMode != cRunningSystemMode) && (vLocalSystemData->vSystemRunMode != cAutoSetupRunningSystem))) &&
			(vMainWindowPointer->InterlocksOK()) && (vStartedSource))
			StartingSystemPleaseTryAgain();
		else
		if (NoSetupOrOKToChange())
		{
			//if ((vGlobalScanTracType != cBriosoScanTrac) || //on Brioso, keep X and Y size the same.
			//	(PasswordOK(cSuperInspxPassword,false)))
			if (((vLocalConfigurationData->vScanTracType == cFermataScanTrac) || (vLocalConfigurationData->vScanTracType == cLegatoScanTrac) ||
				(vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode)) && (!PasswordOK(cSuperInspxPassword,false)))
			{
				if ((vGlobalCurrentProduct->vProductImageWidth < 3) || (vGlobalCurrentProduct->vProductImageWidth > 5))
				{
					CYesNoDialog TempYesNoDialog;
					TempYesNoDialog.vNoticeText =  _T("Image width best set to 4 inches in this mode");
					TempYesNoDialog.vQuestionType = cConfirmAbandon;
					TempYesNoDialog.vYesButtonText = "Set Width To 4 Inches";
					TempYesNoDialog.vNoButtonText = "Leave As Is";
					int TempResult = TempYesNoDialog.DoModal();
					if (TempResult == IDOK)
					{
						ManuallySetWidth(4);
					}
				}
			}
			else
			{
				//Width Button Was Pressed
				CNumericEntryDialog INumericEntryDialog;  
			
				//Set dialog box data titles and number value
				INumericEntryDialog.vEditString = dtoa(vGlobalCurrentProduct->vProductImageWidth,2);

				CString TempText("");

				if (vGlobalScanTracType == cBriosoScanTrac)
					TempText = "Height of X-Ray Image";
				else
					TempText = "Width of X-Ray Image";

				INumericEntryDialog.m_DialogTitleStaticText1 = TempText;
				TempText.LoadString(IDS_Product);
				INumericEntryDialog.m_DialogTitleStaticText2 = TempText + ": " + vProductEditingName;
				TempText.LoadString(IDS_OriginalValue);
				INumericEntryDialog.m_DialogTitleStaticText4 = TempText + ": " + INumericEntryDialog.vEditString;

				if ((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode)) //Continuous Feed, or a Pipeline
					INumericEntryDialog.m_DialogTitleStaticText3 = "Default Value: 4 inches";
				else
					INumericEntryDialog.m_DialogTitleStaticText3 = "Default Value: 1.6 times the container width";

				TempText.LoadString(IDS_MeasuredIn);
				INumericEntryDialog.m_UnitsString = TempText + " " + vLocalConfigurationData->vUnitsString;
				//if (vLocalConfigurationData->vUseMetric)
				//{
				//	INumericEntryDialog.vMaxValue = cMaximumImageWidth * (float)25.4;
				//	INumericEntryDialog.vMinValue = 25.4;
				//}
				//else
				{
					INumericEntryDialog.vMaxValue = cMaximumImageWidth;
					INumericEntryDialog.vMinValue = 1;
				}
				if (vGlobalCurrentProduct->vOverScanMultiplier > 1)
				{
					INumericEntryDialog.vMaxValue = INumericEntryDialog.vMaxValue / vGlobalCurrentProduct->vOverScanMultiplier;
					INumericEntryDialog.vMinValue = INumericEntryDialog.vMinValue / vGlobalCurrentProduct->vOverScanMultiplier;
				}
				INumericEntryDialog.vIntegerOnly = false;
				//Pass control to dialog box and display
				int nResponse = INumericEntryDialog.DoModal();
				//dialog box is now closed, if user pressed select do this
				//if user pressed cancel, do nothing
				if (nResponse == IDOK)
				{
					ManuallySetWidth((float)ATOF(INumericEntryDialog.vEditString));

					//set offset to zero if using offset
					if (vLocalConfigurationData->vSystemBodyTriggerToXRayBeam > 1)
					{
						double TempWhiteSpaceOnOneSideOfImage = (vGlobalCurrentProduct->vProductImageWidth / 1.6) * 0.3;
						if (TempWhiteSpaceOnOneSideOfImage > 4)
							TempWhiteSpaceOnOneSideOfImage = 4;
			
						double TempDistance = vLocalConfigurationData->vSystemBodyTriggerToXRayBeam - TempWhiteSpaceOnOneSideOfImage;  //set offset to zero

						vGlobalCurrentProduct->SetProductBodyTriggerToImageBeltPositionOffset((float)TempDistance);

						double TempTriggerToBeamOffset = -(vLocalConfigurationData->vSystemBodyTriggerToXRayBeam - vGlobalCurrentProduct->vProductBodyTriggerToImageDistanceInInches - TempWhiteSpaceOnOneSideOfImage);
						SetDlgItemText(IDC_SubFunction7Display, _T("0"));
					}
				}
				else 
				if (nResponse == 10)
				{
					//Main Menu button pressed
					ExitAutoSetupDialog(10);
				}
			}
			//else
			//{
			//	CNoticeDialog TempNoticeDialog;
			//	if (vLocalSystemData->vITIPCDig->vOriginalBufferSizeY == vLocalSystemData->vITIPCDig->vOriginalBufferSizeX)
			//		TempNoticeDialog.vNoticeText = "\n\n\nOn Brioso ScanTrac Height is set by setting\nLeft or Right";
			//	else
			//		TempNoticeDialog.vNoticeText = "\n\n\nOn Brioso ScanTrac Width is set by setting\nTop or Bottom";

			//	TempNoticeDialog.vType = cInformationMessage;
			//	TempNoticeDialog.DoModal();
			//}
		}
	}
	else
	if (vMode == cSetROI)
	{  //move right, right one pixel
		if (!vRecievedButtonPress)
		{
			//ReportErrorMessage("Left Mouse Button Down IN SUBFUNCTION 8",cAction, 32000);
			if ((vMouseState == 0) || ((vMouseState > 8) && (vMouseState < 17)))	//if mouse is idle or in double click toggle on mode
			{
				vMouseState = 8;	//set mouse to on while pressed down
				int TimerResult = SetTimer(vMouseDownRepeatTimerHandle,200,NULL);
				if (!TimerResult)
					ReportErrorMessage("Error-Second Timer Failed",cEMailInspx,32000);
			}
		}
		
		if (vInspectionEditing)
		if (DeleteCheckWeighCalibrations(true))
		{
			/*
			bool TempOKToMoveROI = true;
			if ((vInspectionEditing->vInspectionType == cGlass13mmContaminantInspection) && (vInspectionEditing->vTrimEdgesOfJarAtROI))
			{
				CYesNoDialog TempYesNoDialog;
				TempYesNoDialog.vNoticeText = "\nGlass ROI should be at the edge of the container.\nAre you sure you want to move it?";
				TempYesNoDialog.vYesButtonText = "Leave As Is";
				TempYesNoDialog.vNoButtonText = "Move ROI";
				TempYesNoDialog.vQuestionType = cConfirmQuestion;
				int TempResult = TempYesNoDialog.DoModal();
				if (TempResult == IDOK)
					TempOKToMoveROI = false;
			}

			if (TempOKToMoveROI)
			*/
			if (vInspectionEditing->vROIPixelRight < vLocalSystemData->vITIPCDig->vOriginalBufferSizeY)
			{
				vInspectionEditing->vROIPixelRight++; 
				SetROILines();
			}
		}
	}
	else
	if ((vMode == cSetContaminantThreshold) || (vMode == cSetVoidThreshold))
	{  //show histogram of Minimum Good data
		//ReportErrorMessage("Left Button Down IN SUBFUNCTION 8",cEMailInspx, 32000);
		//if ((vMouseState == 0) || ((vMouseState > 8) && (vMouseState < 17)))	//if mouse is idle or in double click toggle on mode
		//{
		//	vMouseState = 8;	//set mouse to on while pressed down
		//	int TimerResult = SetTimer(vMouseDownRepeatTimerHandle,200,NULL);
		//	if (!TimerResult)
		//		ReportErrorMessage("Error-Second Timer Failed",cEMailInspx,32000);
		//}
		
		if (vLocalSystemData->vMaximumBadData)// && (vInspectionEditing->vFindBadIntensity))
		{
			CHistogramDisplayDialog IHistogramDisplayDialog;
			IHistogramDisplayDialog.vNumberOfNumbers = vLocalSystemData->vMaximumBadIndex;
			IHistogramDisplayDialog.vDataTitle = "Maximum Bad in ROI";
			IHistogramDisplayDialog.vValuesAreBytes = true;
			IHistogramDisplayDialog.vPointerToBytes = vLocalSystemData->vMaximumBadData;
			IHistogramDisplayDialog.vLocalSystemData = vLocalSystemData;
			IHistogramDisplayDialog.vLocalConfigurationData = vLocalConfigurationData;
			IHistogramDisplayDialog.vMainWindowPointer = vMainWindowPointer;
			int nResponse = IHistogramDisplayDialog.DoModal();
			if (nResponse == 10)
			{
				//Main Menu button pressed
				Sleep(1); //is the equivelent of a yeild statement;
				ExitAutoSetupDialog(10);
			}
		}
		else
		if (vLocalSystemData->vMaximumBadDataFloat)// && (vInspectionEditing->vFindBadIntensity))
		{
			CHistogramDisplayDialog IHistogramDisplayDialog;
			IHistogramDisplayDialog.vNumberOfNumbers = vLocalSystemData->vMaximumBadIndex;
			IHistogramDisplayDialog.vDataTitle = "Maximum Bad in ROI";
			IHistogramDisplayDialog.vValuesAreFloats = true;
			IHistogramDisplayDialog.vPointerToFloats = vLocalSystemData->vMaximumBadDataFloat;
			IHistogramDisplayDialog.vLocalSystemData = vLocalSystemData;
			IHistogramDisplayDialog.vLocalConfigurationData = vLocalConfigurationData;
			IHistogramDisplayDialog.vMainWindowPointer = vMainWindowPointer;
			int nResponse = IHistogramDisplayDialog.DoModal();
			if (nResponse == 10)
			{
				//Main Menu button pressed
				Sleep(1); //is the equivelent of a yeild statement;
				ExitAutoSetupDialog(10);
			}
		}
	}
}

void CAutoSetupImageDisplayDialog::WriteCSVFiles() 
{
	if (vInspectionEditing)
	{
		if (vInspectionEditing->vFindBadIntensity)
		{  //finding bad or so abort
			vInspectionEditing->vSetupHistoryMaximumBad = (float)vLocalSystemData->vMaximumBadIntensity;
			vInspectionEditing->vSetupHistoryMaximumBadAverage = (float)vLocalSystemData->vMaximumBadAverage;
			vInspectionEditing->vSetupHistoryMaximumBadStandardDeviation = (float)vLocalSystemData->vMaximumBadStandardDeviation;
			vInspectionEditing->vSetupHistoryMaximumBadCount = vLocalSystemData->vMaximumBadCount;
			vInspectionEditing->vSetupHistoryQualityFactor = vLocalSystemData->vQualityFactor;

			vInspectionEditing->vFindBadIntensity = false;
			m_Reference.ShowWindow(SW_HIDE);
			if (!vInspectionEditing->vSetupHistoryMaximumBadAverage)
			{
				m_SubFunction8Button.ShowWindow(SW_SHOW);
				m_SubFunction8Button1.ShowWindow(SW_HIDE);
				m_SubFunction8Button2.ShowWindow(SW_HIDE);
				m_SubFunction8Button3.ShowWindow(SW_HIDE);
				m_SubFunction7Display.ShowWindow(SW_HIDE);
			}
		}
		if (vInspectionEditing->vFindGoodIntensity)
		{
			vInspectionEditing->vSetupHistoryMinimumGood = (float)vLocalSystemData->vMinimumGoodIntensity;
			vInspectionEditing->vSetupHistoryMinimumGoodAverage = (float)vLocalSystemData->vMinimumGoodAverage;
			vInspectionEditing->vSetupHistoryMinimumGoodStandardDeviation = (float)vLocalSystemData->vMinimumGoodStandardDeviation;
			vInspectionEditing->vSetupHistoryMinimumGoodCount = vOldMinimumGoodCount;
			vInspectionEditing->vSetupHistoryQualityFactor = vLocalSystemData->vQualityFactor;

			vInspectionEditing->vFindGoodIntensity = false;
			m_Reference.ShowWindow(SW_HIDE);
		}
	}
	vOldFindGoodIntensity = false;
	vOldFindBadIntensity = false;
	vOldVoidFindGoodIntensity = false;
	vOldVoidFindBadIntensity = false;
}

void CAutoSetupImageDisplayDialog::ShowMaximumConveyorSpeed() 
{
	double TempMaximumConveyorRate = (vLocalSystemData->vMaximumEncoderRate / vGlobalPixelsPerUnit) * 60;  //get units per minute

	//if (vLocalConfigurationData->vUseMetric)
	//	TempMaximumConveyorRate = TempMaximumConveyorRate / 1000;  //convert mm to meters
	//else
		TempMaximumConveyorRate = TempMaximumConveyorRate / 12;  //convert inches to feet

	if (vGlobalCurrentProduct->vOverScanMultiplier > 1)
		TempMaximumConveyorRate = TempMaximumConveyorRate / vGlobalCurrentProduct->vOverScanMultiplier;

	//if (vGlobalCurrentProduct->vMaxConveyorSpeedAsEntered == 0)
		vGlobalCurrentProduct->vMaxConveyorSpeedAsEntered = TempMaximumConveyorRate;

	CString TempText = "Ft/Min Max";
	//if (vLocalConfigurationData->vUseMetric)
	//	TempText = " M/Min Max";

	//if (dtoa(vGlobalCurrentProduct->vMaxConveyorSpeedAsEntered,0) == dtoa(TempMaximumConveyorRate,0))
		SetDlgItemText(IDC_SubFunction8Button1,dtoa(TempMaximumConveyorRate,0) + TempText);
	//else
	//	SetDlgItemText(IDC_SubFunction7Display, 
	//		dtoa(vGlobalCurrentProduct->vMaxConveyorSpeedAsEntered,0) + " - " + dtoa(TempMaximumConveyorRate,0));

	if (vLocalSystemData->vMaximumEncoderRate)
	if ((!vLocalSystemData->vFPGASimulateEncoderRate) && (!vLocalConfigurationData->vContinuousFeedEncoderRate))
		vPercentBeltSpeed = vLocalSystemData->vActualEncoderRate / vLocalSystemData->vMaximumEncoderRate * cMaximumConveyorSpeedDerating;
	else
		vPercentBeltSpeed = vLocalSystemData->vActualEncoderRate / vLocalSystemData->vMaximumEncoderRate * cMaximumConveyorSpeedDeratingWithSimulatedEncoder;
}

bool CAutoSetupImageDisplayDialog::HaveAVoidStructure() 
{
	bool TempResult = false;
	if (((vLocalSystemData->vITIPCDig->vVoidStructureImage) &&
		(vLocalSystemData->vITIPCDig->vStructureCount)) ||
		(vLocalSystemData->vITIPCDig->vVoidProductStructureImage))
		TempResult = true;
	return TempResult;
}

bool CAutoSetupImageDisplayDialog::HaveAContaminantStructure() 
{
	bool TempResult = false;
	if (((vLocalSystemData->vITIPCDig->vStructureImage) && (vLocalSystemData->vITIPCDig->vStructureCount)) || (vLocalSystemData->vITIPCDig->vProductContaminant2StructureImage))
		TempResult = true;
	return TempResult;
}

void CAutoSetupImageDisplayDialog::EditNotes()
{
	// Product Notes Button pressed
	//edit product comment
	CAlphaEntryDialog IAlphaEntryDialog;  
	//Set dialog box data titles and number value
	IAlphaEntryDialog.vLocalSystemData = vLocalSystemData;
	IAlphaEntryDialog.vEditString = vGlobalCurrentProduct->vNotes;
	IAlphaEntryDialog.vMainWindowPointer = vMainWindowPointer;
	IAlphaEntryDialog.m_DialogTitleStaticText = "Product Notes";

	IAlphaEntryDialog.m_DialogTitleStaticText2 = "Product: " + vProductEditingName;
	IAlphaEntryDialog.vAllowDashes = true;
	IAlphaEntryDialog.vAllowPeriod = true;
	IAlphaEntryDialog.vBlankNameOK = true;
	IAlphaEntryDialog.vNoMainMenu = true;
	//Pass control to dialog box and display
	int nResponse = IAlphaEntryDialog.DoModal();
	//dialog box is now closed, if user pressed select do this
	//if user pressed cancel, do nothing
	if (nResponse == IDOK)
	{
		vGlobalCurrentProduct->vNotes = IAlphaEntryDialog.vEditString;
		SetChangeMade();
	}
}

BOOL CAutoSetupImageDisplayDialog::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message != 275)
	if (pMsg->message == cOffsetCalibrationNextStep)
	{
		CString TempText;
		TempText.LoadString(IDS_Wait);
		m_Calibrating.ShowWindow(SW_SHOW);
		if (vLocalSystemData->vCalibrateUsingDAC)
		{
			if (vLocalSystemData->vCalibrationImageNumber == 0)
				SetDlgItemText(IDC_Calibrating,TempText);
			else
				SetDlgItemText(IDC_Calibrating,TempText + " " + dtoa(vLocalSystemData->vCalibrationImageNumber, 0));
		}
		else
			SetDlgItemText(IDC_Calibrating,TempText + " " + dtoa(vLocalSystemData->vCalibrationImageNumber + 12 ,0));

		//if (vLocalSystemData->vCalibrationImageNumber % 2)
		//	SetDlgItemText(IDC_PleaseWait, "Starting X-Rays.... Please Wait.....");
		//else
		//	SetDlgItemText(IDC_PleaseWait, "Starting X-Rays.... Please Wait     ");

		return true;
	}
	else
	if (pMsg->message == cUpdateTitleBarMessage)
	{
		CWnd *TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText1);
		if (TempWindow)
			TempWindow->Invalidate(false);
		return true;  //true indicates it has been handled, so do not process
	}
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
		CWnd *TempWindow;

		if (vGlobalShiftKeyDown)
			PostThreadMessage(vGlobalCallingThreadID,cHiddenKeyPressed, 0, pMsg->wParam);
		else
		switch (pMsg->wParam)
		{
			case 113: // F2:
				OnSubFunction5Button();
			break;
			case 114: // F3:
				OnSubFunction7Button();
			break;
			case 115: // F4:
				OnSubFunction6Button();
			break;
			case 116: // F5:
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
			case 107: // + Key - Sub Function 8:
				OnFunction5Button();
			break;
			case 16: // Shift Key
			case 0x60:  //0 key on KeyPad
				PostThreadMessage(vGlobalCallingThreadID,cHiddenKeyPressed, 1, 0);
			break;
			case 35: // End Key, Left Right
			break;
			case 36:  //Home key, Screen Capture
				vGlobalShiftKeyDown = true;
				PostThreadMessage(vGlobalCallingThreadID,cHiddenKeyPressed, 0, 0x61);
			break;
			case cPageDown: // Help
				//vGlobalMouseMoveCounter = 0;
				if (vMode == cSetImageSize)
					PostThreadMessage(vGlobalCallingThreadID, cShowHelp, 33, 0);
				else
				if (vMode == cSetInspections)
					PostThreadMessage(vGlobalCallingThreadID, cShowHelp, 9, 0);
				else
				if (vMode == cCenterExposureMenu)
					PostThreadMessage(vGlobalCallingThreadID, cShowHelp, 34, 0);
				else
				if (vMode == cLearnMenu)
					PostThreadMessage(vGlobalCallingThreadID, cShowHelp, 35, 0);
				else
				if (vMode == cSetContaminantThreshold)
					PostThreadMessage(vGlobalCallingThreadID, cShowHelp, 36, 0);
				else
				if (vMode == cSetDensityThreshold)
					PostThreadMessage(vGlobalCallingThreadID, cShowHelp, 56, 0);
				else
				if (vMode == cSetVoidThreshold)
					PostThreadMessage(vGlobalCallingThreadID, cShowHelp, 47, 0);
				else
				if (vMode == cSetROI)
					PostThreadMessage(vGlobalCallingThreadID, cShowHelp, 30, 0);
				else
				if (vMode == cSetSizeSettings)
					PostThreadMessage(vGlobalCallingThreadID, cShowHelp, 54, 0);
				else
					PostThreadMessage(vGlobalCallingThreadID, cShowHelp, 0, 0);
				{
					TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText1);
					if (TempWindow)
						TempWindow->Invalidate(false);
				}
			break;
			case cPageUp: // Clear, but enter System setup password
				if (vGlobalPasswordLevel)
				{
					PasswordOK(cNoPassword,false);
				}
				else
				{
					PasswordOK(cAnyPasswordToEnter, true);
				}
				DisplayWithPassword();
				UpdateButtonText();
				TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText1);
				if (TempWindow)
					TempWindow->Invalidate(false);
			break;
			//case c1:
			//		vMainWindowPointer->Send0x9ACommand();

			//break;
			//case c2:
			//		vLocalSystemData->vCurrentConveyorOutput = (vLocalSystemData->vCurrentConveyorOutput & 0xFFF8) | 0x08;  //pulse off, conveyor off, disable encoder
			//		vMainWindowPointer->Send0x9ACommand();

			//break;
			//case c3:
			//		//vLocalSystemData->vCurrentConveyorOutput = (vLocalSystemData->vCurrentConveyorOutput & 0xFFF8) | 0x08;  //pulse off, conveyor off, disable encoder
			//		vLocalSystemData->vCurrentConveyorOutput = (vLocalSystemData->vCurrentConveyorOutput & 0xFFF7) | 0x07;  //on, enable encoder, start pulse
			//		vMainWindowPointer->Send0x9ACommand();

			//break;

			//br ht
			/*
			case c1:
				if ((vLocalSystemData->vLastXRayCommandOn) || 
					(vLocalSystemData->vSystemRunMode != cStoppedSystemMode))
				{
					::PostThreadMessage(vGlobalCallingThreadID,65100,0,0);
					//EmergencyShutXRaysDown();//"Key Switch Turned Off - Inspection Stopped"
					//TempString.LoadString(IDS_KeySwitchTurnedOff);
					ReportErrorMessage("simulate key off - Inspection Stopped", cError,15);
				}
			break;
			*/
			/*
			case c1:
				vLocalSystemData->vModeToRunAfterCalibrateOffset = cAutoSetupRunningSystem;
				vLocalSystemData->vCalibrateErrorCount = 0;
				vMainWindowPointer->CalibrateADCDetectorOffset(false, true);
			break;
			*/
			case 27: // Escape
				::PostThreadMessage(vGlobalCallingThreadID,	cStartChatWindowMessage, 0,0);
			break;	
			case 0x0d: // F2:
				OnFunction2Button();
			break;
		}
		/*
		if (vLocalSystemData->vXRayOnNotice)
		{
			CString TempText("");
			TempText.LoadString(IDS_XRAYON);
			SetDlgItemText(IDC_Calibrating,TempText);
			m_Calibrating.ShowWindow(SW_SHOW);
		}
		*/
		vRecievedButtonPress = false;
		return true;  //true indicates it has been handled, so do not process
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CAutoSetupImageDisplayDialog::ChangedProductSettingsSoReload()
{
	//Stop running, setup new product, start running
	if (vLocalSystemData->vInAutoSetup)
	{
		if (vLocalSystemData->vITIPCDig->vOriginalImage)
			vLocalSystemData->vITIPCDig->vOriginalImage->vTop;
		vHaveAnImage = false;

		KillTimer(vCheckKeySwitchTimerHandle);
		CString TempText;
		TempText.LoadString(IDS_PleaseWait);
		TempText = " " + TempText;
		if (!vChangedEdgeFindingMode)
			SetStatusLine(TempText);
		vStartingUpSource = true;
		vRestartingXRaysWithNewSettings = true;
		vMainWindowPointer->StopImageAcquisition();
		vMainWindowPointer->StopRunning(false);
		int TimerResult = SetTimer(vWaitUntilDoneRunningForReloadTimer,500,NULL);
		if (!TimerResult)
			ReportErrorMessage("Error-WaitForReload Timer Failed",cEMailInspx,32000);
	}
}

void CAutoSetupImageDisplayDialog::DoneRunningSoReload()
{
	KillTimer(vCheckKeySwitchTimerHandle);
	WORD TempTimeToWait = 500;
	//if ((vLocalConfigurationData->vContinuousFeedEncoderRate) || (vLocalConfigurationData->vContinuousFeedContainerRate))
	//	TempTimeToWait = 2000;

	int TimerResult = SetTimer(vSetupTimerHandle,TempTimeToWait,NULL);
	if (!TimerResult)
		ReportErrorMessage("Error-Setup Timer Failed",cEMailInspx,32000);
}

HBRUSH CAutoSetupImageDisplayDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	int TempControlID = pWnd->GetDlgCtrlID();
	HWND TempDialogHwnd = this->m_hWnd;
	pDC->SetAttribDC(pDC->m_hDC);
	int TextSize;

	if (TempControlID == IDC_PleaseWait)
	{
		TextSize = SetTextSizeMultilineDisplay(TempDialogHwnd, pWnd, pDC, &m_PleaseWait, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
  }
	if (TempControlID == IDC_Background)
	{
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	//if (TempControlID == IDC_ErrorMessage)
	//{
	//	TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_ErrorMessage, 8);  //5 is medium large
	//	pDC->SetBkMode(TRANSPARENT);
	//	return vLocalSystemData->vLightBrownBrush;
	//}

	if (TempControlID == IDC_DialogTitleStaticText1)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DialogTitleStaticText, 8);  //5 is medium large
		if (vGlobalShiftKeyDown)
			pDC->SetTextColor(cBlack);
		else
			pDC->SetTextColor(vGlobalDialogTitleColor);
	}


  if (TempControlID == IDC_CurrentDensity)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_CurrentDensity, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBlack);
		if (vCurrentDensity < 2)
			return vLocalSystemData->vRedBrush;
		else
		if (vCurrentDensity > 253)
			return vLocalSystemData->vWhiteBrush;
		else
		if (vCurrentDensity < 100)
			return vLocalSystemData->vLightBlueBrush;
		else
		if (vCurrentDensity > 160)
			return vLocalSystemData->vYellowBrush;
		else
			return vLocalSystemData->vGreenBrush;
	}

	if (TempControlID == IDC_AverageDensity)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_AverageDensity, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBlack);
		if (vAverageDensity)
		if (vAverageDensity < 2)
			return vLocalSystemData->vRedBrush;
		else
		if (vAverageDensity > 253)
			return vLocalSystemData->vWhiteBrush;
		else
		if (vAverageDensity < 100)
			return vLocalSystemData->vLightBlueBrush;
		else
		if (vAverageDensity > 160)
			return vLocalSystemData->vYellowBrush;
		else
			return vLocalSystemData->vGreenBrush;
	}
	
	if (TempControlID == IDC_RejectReason)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_RejectReason, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vYellowBrush;
	}

  if (TempControlID == IDC_StatusLine)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_StatusLine, 5);  //5 is medium large
		if ((vLocalSystemData->vYellowMessageButtonYellow) || (vMakeStatusLineYellowRed))
		{
			pDC->SetTextColor(cDarkBlue);
			pDC->SetBkMode(TRANSPARENT);
			if (vMakeStatusLineYellowRed)
				pDC->SetTextColor(cRed);

			return vLocalSystemData->vYellowBrush;
		}
		else
		if (!vLocalSystemData->vITIPCDig->vDriftCompensationLocked)
		if (vInspectionEditing)
		if ((vInspectionEditing->vFindGoodIntensity) || (vInspectionEditing->vFindBadIntensity) ||
			(vInspectionEditing->vCollectingDensityAverage))
		{
			pDC->SetTextColor(cDarkBlue);
			pDC->SetBkMode(TRANSPARENT);
			return vLocalSystemData->vRedBrush;
		}

		if (vInspectionEditing)
		if ((vInspectionEditing->vCollectingDensityAverage) || 
			(vOldAverageEnabled) || (vOldStructureEnabled) || 
			(vOldCalibrateBodyTriggerReferenceEnabled) || (vOldTakingQuickLearnImages) || (vOldAdjustingBrightnessBeforeLearning))
		{
			pDC->SetBkMode(TRANSPARENT);
			//pDC->SetTextColor(cButtonTextColor);
			return vLocalSystemData->vGreenBrush;
		}
	}

	if (TempControlID == IDC_SubFunction6Button1)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction6Button1, 5);  //5 is medium large
		
	if (TempControlID == IDC_SubFunction6Button2)  //make SD yellow if > 16
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction6Button2, 5);  //5 is medium large
		if ((vMode == cSetContaminantThreshold) || (vMode == cSetVoidThreshold))
		if ((vInspectionEditing) && ((vInspectionEditing->vInspectionType != cItemCountInspection) && (vInspectionEditing->vInspectionType != cSizeInspection) && (vInspectionEditing->vInspectionType != cMissingLidInspectionOnSize)))
		if (vLocalSystemData->vMinimumGoodStandardDeviation > 16)
		{
			pDC->SetBkMode(TRANSPARENT);
			return vLocalSystemData->vYellowBrush;
		}
	}

	if (TempControlID == IDC_Simulating)  //make QF yellow if out of range data
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Simulating, 5);
		if ((vMode == cSetContaminantThreshold) || (vMode == cSetVoidThreshold) || (vMode == cSetDensityThreshold))
		if ((vInspectionEditing) && ((vInspectionEditing->vInspectionType != cItemCountInspection) && (vInspectionEditing->vInspectionType != cSizeInspection) && (vInspectionEditing->vInspectionType != cMissingLidInspectionOnSize)))
		if (vInspectionEditing->vInspectionType != cUnderfillByWeightInspection)
		if (vInspectionEditing->vInspectionType != cOverfillByWeightInspection)
		if (vInspectionEditing->vInspectionType != cCheckWeighInspection)
		if (vOldQualityFactor)
		{
			pDC->SetBkMode(TRANSPARENT);
			if (vOldQualityFactor >= 3.7)
				return vLocalSystemData->vGreenBrush;
			else
			if (vOldQualityFactor < 1)
				return vLocalSystemData->vRedBrush;
			else
				return vLocalSystemData->vYellowBrush;
		}
	}

	if (TempControlID == IDC_Function1Button)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function1Button, 5); 
		pDC->SetBkMode(TRANSPARENT);

	  if (vFunction1ButtonEnable == true)
			pDC->SetTextColor(cButtonTextColor);
		else
		{
			pDC->SetTextColor(cGray);
			return vLightGrayBrush;	  
		}

		if ((vMode == cSetSizeSettings) || (vReturnToSetSource))
			return vLocalSystemData->vGreenBrush;

		return vGlobalButtonColorBrush;	  
	}
 
	if (TempControlID == IDC_Function2Button)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function2Button, 5);  //5 is medium large
		pDC->SetAttribDC(pDC->m_hDC);
		pDC->SetBkMode(TRANSPARENT);

		if (vFunction2ButtonEnable)
			pDC->SetTextColor(cButtonTextColor);
		else
		{	
			pDC->SetTextColor(cGray);
			return vLightGrayBrush;
		}
		if (vMode == cSetDensityThreshold)
		if ((vInspectionEditing) && ((vInspectionEditing->vInspectionType == cCheckWeighInspection) || (vInspectionEditing->vInspectionType == cUnderfillByWeightInspection) || 
			(vInspectionEditing->vInspectionType == cOverfillByWeightInspection)))
		{
			if (vInspectionEditing->vInspectionNameForWeightTrendingPointer) //if wieght trending inspection, use other inspections settings
				return vGlobalButtonColorBrush;
		}
		return vGlobalButtonColorBrush;	  
	}

	if (TempControlID == IDC_Function3Button)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function3Button, 5);  //5 is medium large
		pDC->SetAttribDC(pDC->m_hDC);
		pDC->SetBkMode(TRANSPARENT);

		if (vFunction3ButtonEnable)
			pDC->SetTextColor(cButtonTextColor);
		else
		{	
			pDC->SetTextColor(cGray);
			return vLightGrayBrush;
		}

		if (vLocalSystemData->vYellowMessageButtonYellow)
			return vLocalSystemData->vYellowBrush;

		if ((!vStartingUpSource) && (!vRestartingXRaysWithNewSettings) && ((!vLocalSystemData->vXRayOnNotice) || (vLocalSystemData->vSystemRunMode == cStoppedSystemMode)))
			return vLocalSystemData->vGreenBrush;

		if (vHaveAnAutoThresholdValue)
			return vLocalSystemData->vGreenBrush;

		return vGlobalButtonColorBrush;	  
	}

	if (TempControlID == IDC_Function4Button) 
	{   
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function4Button, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);

		if (vFunction4ButtonEnable == true)
		{
			pDC->SetTextColor(cButtonTextColor);
			return vGlobalButtonColorBrush;	  
		}
		else
		{
			pDC->SetTextColor(cGray);
			return vLightGrayBrush;	  
		}
	}

	if (TempControlID == IDC_Function5Button)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function5Button, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);

		if (vFunction5ButtonEnable == true)
			pDC->SetTextColor(cButtonTextColor);
		else
		{
			pDC->SetTextColor(cGray);
			return vLightGrayBrush;
		}

		if (vMode != cCenterExposureMenu)
		if (vMode != cSetSizeSettings)
		if ((vMode != cLearnMenu) || ((((vLocalSystemData->vITIPCDig->vAverageImage) || (vLocalSystemData->vITIPCDig->vProductAverageImage))) &&
				(!vOldAverageEnabled) && (!vOldStructureEnabled) && (!vOldCalibrateBodyTriggerReferenceEnabled) && (!vOldTakingQuickLearnImages)))
		if ((vMode != cSetContaminantThreshold) || (!vInspectionEditing) || ((vLocalSystemData->vMinimumGoodAverage != 0) && 
			((vInspectionEditing) && (!vInspectionEditing->vFindGoodIntensity))) || (((vInspectionEditing) && (vInspectionEditing->vFindGoodIntensity)) && (vLocalSystemData->vFrameCount > 100)))
		if ((vMode != cSetVoidThreshold) || (!vInspectionEditing) || ((vLocalSystemData->vMinimumGoodAverage != 0) && 
			((vInspectionEditing) && (!vInspectionEditing->vFindGoodIntensity))) || (((vInspectionEditing) && (vInspectionEditing->vFindGoodIntensity)) && (vLocalSystemData->vFrameCount > 100)))
		if ((vMode != cSetBackground) || (vLocalSystemData->vITIPCDig->vProductBackgroundImage))
		if ((vMode != cSetImageSize) || (!vHaveNotSetSize))
		if ((vMode != cSetDensityThreshold) || (vChangeMade))
			return vLocalSystemData->vGreenBrush;

		if (vMode == cSetROI)
			return vLocalSystemData->vGreenBrush;

		if (vMode == cSetDensityThreshold)
		{
			if (vInspectionEditing)
			if ((vInspectionEditing->vInspectionType == cUnderfillByWeightInspection) || (vInspectionEditing->vInspectionType == cOverfillByWeightInspection) || (vInspectionEditing->vInspectionType == cCheckWeighInspection))
			{
				if ((vInspectionEditing->vCheckWeighMFactor != 1) || (vInspectionEditing->vCheckWeighBFactor))
					return vLocalSystemData->vGreenBrush;
			}
			else
			{
				if (vInspectionEditing->vSetupHistoryMinimumGoodCount > 5) 
					return vLocalSystemData->vGreenBrush;
			}
		}

		if (vMode == cCenterExposureMenu)
		if ((vCurrentDensity >= 80) && (vCurrentDensity <= 180))
			return vLocalSystemData->vGreenBrush;

		if (vMode == cSetDensityThreshold)
		if (vInspectionEditing)
		if ((vInspectionEditing->vInspectionType == cUnderfillByWeightInspection) || (vInspectionEditing->vInspectionType == cCheckWeighInspection))
		if (!vInspectionEditing->vInspectionNameForWeightTrendingPointer)  //if using your own ROI and weight calibrations
		{
			if ((!vInspectionEditing->vCheckWeighMinimumGoodWeight) || (!vInspectionEditing->vCheckWeighMaximumGoodWeight))
				return vGlobalButtonColorBrush;
		}
		return vGlobalButtonColorBrush;
	}

	if (TempControlID == IDC_SubFunction1Button)
	{
		BYTE TempArrowsAreSet = m_SubFunction1Button.AreNavigationArrowsSet();
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction1Button, 5, TempArrowsAreSet);  //5 is medium large
		pDC->SetAttribDC(pDC->m_hDC);
		pDC->SetBkMode(TRANSPARENT);

		if (vSubFunction1ButtonEnable)
			pDC->SetTextColor(cButtonTextColor);
		else
		{
			pDC->SetTextColor(cGray);
			return vLightGrayBrush;
		}

		if (vMode == cSetDensityThreshold)
		if ((vInspectionEditing) && ((vInspectionEditing->vInspectionType == cCheckWeighInspection) || (vInspectionEditing->vInspectionType == cUnderfillByWeightInspection) || 
			(vInspectionEditing->vInspectionType == cOverfillByWeightInspection)))
		{
			if (vInspectionEditing->vInspectionNameForWeightTrendingPointer) //if wieght trending inspection, use other inspections settings
				return vGlobalButtonColorBrush;
			else
			if (!vInspectionEditing->vCheckWeighMinimumGoodWeight)
				return vLocalSystemData->vGreenBrush;
		}
		return vGlobalButtonColorBrush;	  
	}

	if (TempControlID == IDC_SubFunction2Button)
	{
		BYTE TempArrowsAreSet = m_SubFunction2Button.AreNavigationArrowsSet();
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction2Button, 5, TempArrowsAreSet);
		pDC->SetAttribDC(pDC->m_hDC);
		pDC->SetBkMode(TRANSPARENT);

		if (vSubFunction2ButtonEnable)
			pDC->SetTextColor(cButtonTextColor);
		else
		{
			pDC->SetTextColor(cGray);
			return vLightGrayBrush;
		}

		if (vMode == cSetDensityThreshold)
		{
			if ((vInspectionEditing) && ((vInspectionEditing->vInspectionType == cOverfillByWeightInspection) || (vInspectionEditing->vInspectionType == cUnderfillByWeightInspection)))
			{
				if (!vInspectionEditing->vInspectionNameForWeightTrendingPointer) //if wieght trending inspection, use other inspections settings
				if ((vInspectionEditing->vCheckWeighBFactor) || (vInspectionEditing->vCheckWeighMFactor != 1))
				if (!vInspectionEditing->vCheckWeighNominalWeight)
					return vLocalSystemData->vGreenBrush;
			}
		}

		if (vMode == cSetImageSize)
		if (vLocalSystemData->vInAutoSetup)
		{
			if (vHaveNotSetSize)
				return vLocalSystemData->vGreenBrush;
		}

		return vGlobalButtonColorBrush;	  
	}

	if (TempControlID == IDC_SubFunction3Button)
	{
		BYTE TempArrowsAreSet = m_SubFunction3Button.AreNavigationArrowsSet();
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction3Button, 5, TempArrowsAreSet);
		pDC->SetAttribDC(pDC->m_hDC);
		pDC->SetBkMode(TRANSPARENT);

		if (vSubFunction3ButtonEnable)
			pDC->SetTextColor(cButtonTextColor);
		else
		{
			pDC->SetTextColor(cGray);
			return vLightGrayBrush;
		}

		if (vMode == cSetDensityThreshold)
		{
			if ((vInspectionEditing) && (vInspectionEditing->vInspectionType == cUnderfillByWeightInspection))
			{
				if (!vInspectionEditing->vInspectionNameForWeightTrendingPointer) //if wieght trending inspection, use other inspections settings
				if ((vInspectionEditing->vCheckWeighBFactor) || (vInspectionEditing->vCheckWeighMFactor != 1))
				if (!vInspectionEditing->vCheckWeighNominalWeight)
					return vLocalSystemData->vGreenBrush;
			}
			else
			if ((vInspectionEditing) && (vInspectionEditing->vInspectionType == cOverfillByWeightInspection))
			if ((vInspectionEditing->vCheckWeighBFactor) || (vInspectionEditing->vCheckWeighMFactor != 1)) //if have weight calibrations, make threshold button green
				return vLocalSystemData->vGreenBrush;
		}
		if (vMode == cSetDensityThreshold)
		if (vInspectionEditing)
		if ((vInspectionEditing->vInspectionType == cOverfillByWeightInspection) || (vInspectionEditing->vInspectionType == cCheckWeighInspection))
		if (!vInspectionEditing->vInspectionNameForWeightTrendingPointer)  //if using your own ROI and weight calibrations
		if ((!vInspectionEditing->vCheckWeighMinimumGoodWeight) || (!vInspectionEditing->vCheckWeighMaximumGoodWeight))
			return vGlobalButtonColorBrush;

		return vGlobalButtonColorBrush;	  
	}

	if (TempControlID == IDC_SubFunction4Button)
	{
		BYTE TempArrowsAreSet = m_SubFunction4Button.AreNavigationArrowsSet();
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction4Button, 5, TempArrowsAreSet);

		pDC->SetAttribDC(pDC->m_hDC);
		pDC->SetBkMode(TRANSPARENT);

		if (vSubFunction4ButtonEnable)
			pDC->SetTextColor(cButtonTextColor);
		else
		{	
			pDC->SetTextColor(cGray);
			return vLightGrayBrush;
		}

		if (vMode == cSetDensityThreshold)
		if ((vInspectionEditing) && ((vInspectionEditing->vInspectionType == cCheckWeighInspection) || (vInspectionEditing->vInspectionType == cUnderfillByWeightInspection) || 
			(vInspectionEditing->vInspectionType == cOverfillByWeightInspection)))
		{
			if (vInspectionEditing->vInspectionNameForWeightTrendingPointer) //if wieght trending inspection, use other inspections settings
				return vGlobalButtonColorBrush;
			else
			if (!vInspectionEditing->vCheckWeighMaximumGoodWeight)
				return vLocalSystemData->vGreenBrush;
		}

		return vGlobalButtonColorBrush;	  
	}

	if (TempControlID == IDC_SubFunction5Button)
	{
		pDC->SetAttribDC(pDC->m_hDC);
		pDC->SetBkMode(TRANSPARENT);
		BYTE TempArrowsAreSet = m_SubFunction5Button.AreNavigationArrowsSet();
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction5Button, 5, TempArrowsAreSet);  //5 is medium large

		if (vSubFunction5ButtonEnable)
			pDC->SetTextColor(cButtonTextColor);
		else
		{
			pDC->SetTextColor(cGray);
			return vLightGrayBrush;
		}

		if (vMode == cLearnMenu)
		if ((vOldAverageEnabled) || (vOldStructureEnabled) || (vOldCalibrateBodyTriggerReferenceEnabled) || (vLocalSystemData->vTakingQuickLearnImages) || (vOldAdjustingBrightnessBeforeLearning))
			return vLocalSystemData->vRedBrush;
	
		if (vMode == cLearnMenu)
		if (!vGlobalCurrentProduct->vSetupHistoryAverageCount) //if have not learned yet
			return vLocalSystemData->vGreenBrush;
	
		if (vMode == cSetBackground)
		if (!vLocalSystemData->vITIPCDig->vProductBackgroundImage)
			return vLocalSystemData->vGreenBrush;

		if (vMode == cSetDensityThreshold)
		{
			if ((vInspectionEditing) && (vInspectionEditing->vCollectingDensityAverage))
			if (!vCollectingBadDensityAverage)
				return vLocalSystemData->vRedBrush;

			if (vInspectionEditing)
			if ((vInspectionEditing->vInspectionType == cUnderfillByWeightInspection) || (vInspectionEditing->vInspectionType == cOverfillByWeightInspection) || (vInspectionEditing->vInspectionType == cCheckWeighInspection))
			if (!vInspectionEditing->vSetupHistoryMinimumGoodCount)
				return vLocalSystemData->vGreenBrush;

			if ((((vMode == cSetContaminantThreshold) || (vMode == cSetVoidThreshold)) && (((vInspectionEditing) && (!vInspectionEditing->vFindGoodIntensity)) && 
				(vLocalSystemData->vMinimumGoodStandardDeviation == 0))) || ((vMode == cLearnMenu) && (!((vLocalSystemData->vITIPCDig->vAverageImage) || (vLocalSystemData->vITIPCDig->vProductAverageImage)))))
			if ((vLocalSystemData->vSystemRunMode != cStoppedSystemMode) || (vStartingUpSource) || (vRestartingXRaysWithNewSettings))
				return vLocalSystemData->vGreenBrush;

			if (((vMode == cSetDensityThreshold) && ((vInspectionEditing) && (!vInspectionEditing->vCollectingDensityAverage))) && (!vInspectionEditing->vSetupHistoryMinimumGoodAverage))
			{
				pDC->SetBkMode(TRANSPARENT);
			   if (vSubFunction5ButtonEnable == true)
					pDC->SetTextColor(cButtonTextColor);
				else
					pDC->SetTextColor(cGray);
				return vLocalSystemData->vGreenBrush;
			}
		}
		if ((vMode == cCenterExposureMenu) && (vLocalSystemData->vInAutoSetup))
		if ((vCurrentDensity < 80) || (vCurrentDensity > 180))
			return vLocalSystemData->vGreenBrush;

		if (vInspectionEditing)
		if (vInspectionEditing->vFindGoodIntensity)
			return vLocalSystemData->vRedBrush;

		return vGlobalButtonColorBrush;
	}

	if (TempControlID == IDC_SubFunction6Button)
	{
		BYTE TempArrowsAreSet = m_SubFunction6Button.AreNavigationArrowsSet();
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction6Button, 5, TempArrowsAreSet);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);

		if (vSubFunction6ButtonEnable)
			pDC->SetTextColor(cButtonTextColor);
		else
		{
			pDC->SetTextColor(cGray);
			return vLightGrayBrush;	  
		}  

		if (((vShowAverage) || (vShowStructure) || (vShowHistogram) || (vShowVoidStructure) || (vShowContaminant2Structure)) && (vMode == cLearnMenu))
			return vLocalSystemData->vGreenBrush;

		return vGlobalButtonColorBrush;	  
	}
	
	if (TempControlID == IDC_SubFunction6Button3)  //make minimum good yellow if too low
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction6Button3, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		if ((vMode ==  cSetContaminantThreshold) || (vMode == cSetVoidThreshold))
		if ((vInspectionEditing) && ((vInspectionEditing->vInspectionType != cItemCountInspection) && (vInspectionEditing->vInspectionType != cItemCountInspection) && (vInspectionEditing->vInspectionType != cSizeInspection) && (vInspectionEditing->vInspectionType != cMissingLidInspectionOnSize)))
		{
			if ((vLocalSystemData->vMinimumGoodIntensity < 40) ||
				((vLocalSystemData->vMinimumGoodIntensity < vLocalSystemData->vMinimumGoodAverage - (5 * vLocalSystemData->vMinimumGoodStandardDeviation)) && (vLocalSystemData->vMinimumGoodStandardDeviation)))
			if (vLocalSystemData->vMinimumGoodAverage)
				return vLocalSystemData->vYellowBrush;
		}
		else
		if ((vInspectionEditing) && ((vInspectionEditing->vInspectionType == cSizeInspection) || (vInspectionEditing->vInspectionType == cMissingLidInspectionOnSize)))
		if ((vLocalSystemData->vMaximumBadIntensity > vLocalSystemData->vMinimumGoodIntensity) && (vLocalSystemData->vMaximumBadCount > 4))
			return vLocalSystemData->vYellowBrush;
	}

	if (TempControlID == IDC_SubFunction7Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		BYTE TempArrowsAreSet = m_SubFunction7Button.AreNavigationArrowsSet();
		pDC->SetAttribDC(pDC->m_hDC);
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction7Button, 5, TempArrowsAreSet);  //5 is medium large
		
		if (vSubFunction7ButtonEnable)
			pDC->SetTextColor(cButtonTextColor);
		else
		{
			pDC->SetTextColor(cGray);
			return vLightGrayBrush;	  
		}

		if (vMode == cSetDensityThreshold)
		if ((vInspectionEditing) && ((vInspectionEditing->vInspectionType == cCheckWeighInspection) || (vInspectionEditing->vInspectionType == cUnderfillByWeightInspection) || 
			(vInspectionEditing->vInspectionType == cOverfillByWeightInspection)))
		if (vInspectionEditing->vInspectionNameForWeightTrendingPointer) //if wieght trending inspection, use other inspections settings
			return vGlobalButtonColorBrush;

		if (vMode == cLearnMenu)
		{
			if ((vLocalConfigurationData->vScanTracType == cSoloScanTrac) || (vLocalConfigurationData->vScanTracType == cSoloPlusScanTrac) || (vLocalConfigurationData->vScanTracType == cFermataScanTrac))
			if (vLocalConfigurationData->vNumberOfQuickLearnImagesToTake)
				return vLocalSystemData->vGreenBrush;
			else
				return vLocalSystemData->vYellowBrush;
		}

		if (vMode == cSetDensityThreshold)
		{
			if ((vInspectionEditing) && (vInspectionEditing->vCollectingDensityAverage))
			if (vCollectingBadDensityAverage)
				return vLocalSystemData->vRedBrush;

			if (vInspectionEditing)
			if (!vInspectionEditing->vSetupHistoryMaximumBadCount)
				return vLocalSystemData->vGreenBrush;
		}

		if (vInspectionEditing)
		if (vInspectionEditing->vFindBadIntensity)
			return vLocalSystemData->vRedBrush;
	
		return vGlobalButtonColorBrush;	  
	}
	
	if (TempControlID == IDC_SubFunction8Button) 
	{   
		BYTE TempArrowsAreSet = m_SubFunction4Button.AreNavigationArrowsSet();
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction8Button, 5, TempArrowsAreSet);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		if (vSubFunction8ButtonEnable == true)
		{
			pDC->SetTextColor(cButtonTextColor);
			return vGlobalButtonColorBrush;
		}
		else
		{
			pDC->SetTextColor(cGray);
			return vLightGrayBrush;
		}
	}

  if (TempControlID == IDC_SubFunction8Button3) //make max bad yellow if too big
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction8Button3, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);

		if (vMode == cCenterExposureMenu)
		if (((vPercentBeltSpeed > 99) && (!vLocalConfigurationData->vContinuousFeedEncoderRate) && (!vLocalSystemData->vFPGASimulateEncoderRate)) || (vPercentBeltSpeed > 105)) //if over 105% and pipeline
			return vLocalSystemData->vRedBrush;

		if ((vMode ==  cSetContaminantThreshold) || (vMode == cSetVoidThreshold))
		{
			if ((vInspectionEditing) && ((vInspectionEditing->vInspectionType != cItemCountInspection) && (vInspectionEditing->vInspectionType != cSizeInspection) && (vInspectionEditing->vInspectionType != cMissingLidInspectionOnSize)))
			{
				if ((vLocalSystemData->vMaximumBadIntensity > 240) ||
					((vLocalSystemData->vMaximumBadIntensity > vLocalSystemData->vMaximumBadAverage + (3.0 * vLocalSystemData->vMaximumBadStandardDeviation)) && (vLocalSystemData->vMaximumBadStandardDeviation)))
					return vLocalSystemData->vYellowBrush;
			}
			else
			if ((vInspectionEditing) && ((vInspectionEditing->vInspectionType == cItemCountInspection) || (vInspectionEditing->vInspectionType == cSizeInspection) || (vInspectionEditing->vInspectionType == cMissingLidInspectionOnSize)))
			if ((vLocalSystemData->vMaximumBadIntensity > vLocalSystemData->vMinimumGoodIntensity) && (vLocalSystemData->vMaximumBadCount > 4))
				return vLocalSystemData->vYellowBrush;
		}
	}

	if (TempControlID == IDC_XRaysOn)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_XRaysOn, 7);
		pDC->SetTextColor(cYellow);
		pDC->SetBkColor(cRed);
		return vLocalSystemData->vRedBrush;
	}

	if (TempControlID == IDC_Calibrating)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Calibrating, 7);
		pDC->SetTextColor(cYellow);
		pDC->SetBkColor(cRed);
		return vLocalSystemData->vRedBrush;
	}

	if (TempControlID == IDC_DensityNotLockedIndicator)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DensityNotLockedIndicator, 5);  //5 is medium large
	}

	if (TempControlID == IDC_SubFunction1Display)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction1Display, 5);  //5 is medium large
	if (TempControlID == IDC_SubFunction2Display)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction2Display, 5);  //5 is medium large
	if (TempControlID == IDC_SubFunction3Display)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction3Display, 5);  //5 is medium large
	if (TempControlID == IDC_SubFunction4Display)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction4Display, 5);  //5 is medium large
	if (TempControlID == IDC_SubFunction5Display)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction5Display, 5);  //5 is medium large
	if (TempControlID == IDC_SubFunction6Display)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction6Display, 5);  //5 is medium large
	if (TempControlID == IDC_SubFunction7Display)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction7Display, 5);  //5 is medium large
	if (TempControlID == IDC_SizeY)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SizeY, 5);  //5 is medium large
	if (TempControlID == IDC_SizeX)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SizeX, 5);  //5 is medium large
	if (TempControlID == IDC_SizeLabelY)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SizeLabelY, 5);  //5 is medium large
	if (TempControlID == IDC_SizeLabelX)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SizeLabelX, 5);  //5 is medium large
	if (TempControlID == IDC_XScale2)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_XScaleLabel, 5);  //5 is medium large
	if (TempControlID == IDC_XScaleLabel)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_XScaleLabel, 5);  //5 is medium large
	if (TempControlID == IDC_YScale2)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_YScale2, 5);  //5 is medium large
	if (TempControlID == IDC_YScaleLabel)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_YScaleLabel, 5);  //5 is medium large
	if (TempControlID == IDC_ThresholdForContainerBounds2)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_ThresholdForContainerBounds2, 5);  //5 is medium large
	if (TempControlID == IDC_Reference)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Reference, 5);  //5 is medium large
	if (TempControlID == IDC_Width2)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Width2, 5);  //5 is medium large
	if (TempControlID == IDC_CurrentLabel)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_CurrentLabel, 5);  //5 is medium large
	if (TempControlID == IDC_SubFunction8Display)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction8Display, 5);  //5 is medium large
	if (TempControlID == IDC_SubFunction8Button1)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction8Button1, 5);  //5 is medium large
	if (TempControlID == IDC_SubFunction8Button2)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction8Button2, 5);  //5 is medium large
	if (TempControlID == IDC_SubFunction4DisplayA)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction4DisplayA, 5);  //5 is medium large
	if (TempControlID == IDC_Simulating)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Simulating, 5);  //5 is medium large
	
	return hbr;
}


void CAutoSetupImageDisplayDialog::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
  if (nIDEvent == vClearStartingIndicatorIfXRaysOnTimerHandle)
  {
		if (!m_PleaseWait.IsWindowVisible())  //if have already cleared the please wait message, then kill timer and do nothing
			KillTimer(vClearStartingIndicatorIfXRaysOnTimerHandle); 
		else
		if ((vLocalSystemData->vSystemRunMode == cAutoSetupRunningSystem) && (vLocalSystemData->vXRayOnNotice))  //if X-Rays on now, then clear please wait message
		{
			KillTimer(vClearStartingIndicatorIfXRaysOnTimerHandle);
			CString TempText("");
			TempText.LoadString(IDS_XRAYON);
			SetDlgItemText(IDC_Calibrating, TempText);
			m_Calibrating.ShowWindow(SW_SHOW);
		
			if (vGlobalCurrentProduct->vMultiLaneNumberOfLanes)
			{
				vLocalSystemData->vCurrentConveyorOutput = (vLocalSystemData->vCurrentConveyorOutput & 0xFFF8) | 0x08;  //pulse off, conveyor off, disable encoder
				vMainWindowPointer->Send0x9ACommand();
				Sleep(100);

				vLocalSystemData->vCurrentConveyorOutput = (vLocalSystemData->vCurrentConveyorOutput & 0xFFF7) | 0x07;  //on, enable encoder, start pulse
				vMainWindowPointer->Send0x9ACommand();

				SetStatusLine(" Send container in any lane....");
			}
			else
			if ((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode)) //Continuous Feed, or a Pipeline
			{
				SetStatusLine(" Waiting for Image..");
			}
			else
				SetStatusLine(" Send Container Through ScanTrac...");
			m_PleaseWait.ShowWindow(SW_HIDE);
		}
	}
	else	
  if (nIDEvent == vOneSecondTimerHandle)
  {
		OneSecondUpdate();
	}
	else	
  if (nIDEvent == vWaitUntilDoneRunningForReloadTimer)
  {
		CString TempText = vCurrentStatusLineText + ".";
		if (!vChangedEdgeFindingMode)
			SetStatusLine(TempText);
		if (((vLocalSystemData->vContainerCollection->GetCount() == 0) ||
			(vLocalSystemData->vActualEncoderRate == 0)) ||
			(vLocalSystemData->vSystemRunMode == cStoppedSystemMode))
		{
			if (vLocalSystemData->vContainerCollection->GetCount())
			vMainWindowPointer->RemoveAllContainers();
			if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)
				vMainWindowPointer->StopRunning(false);

			//turns off conveyor when autosize if do this
			//vMainWindowPointer->SendConveyorOnOffToUController(0);
			//vMainWindowPointer->TurnSimulatedEncoderOff();
			vMainWindowPointer->TurnSimulatedContainersOff();

			KillTimer(vWaitUntilDoneRunningForReloadTimer);
			DoneRunningSoReload();
		}
	}
	else
  if (nIDEvent == vWaitUntilDoneRunningForExitTimer)
  {
		CString TempText = vCurrentStatusLineText + ".";
		if (!vChangedEdgeFindingMode)
			SetStatusLine(TempText);
		if (((vLocalSystemData->vContainerCollection->GetCount() == 0) ||
			(vLocalSystemData->vActualEncoderRate == 0)) ||
			(vLocalSystemData->vSystemRunMode == cStoppedSystemMode))
		{
			KillTimer(vWaitUntilDoneRunningForExitTimer);
			DoneRunningSoExit();
		}
	}
	else
  if (nIDEvent == vSetupTimerHandle) // called to re-setup the product
  {
		//if done processing quick learn images
		if (!vGlobalProcessQuickLearnImagesThread)
		if (vLocalSystemData->vSystemRunMode == cStoppedSystemMode)
		{
			CString TempText;
			TempText.LoadString(IDS_StartingSystem);
			TempText = " " + TempText;
			if (!vChangedEdgeFindingMode)
				SetStatusLine(TempText);

			KillTimer(vSetupTimerHandle);
			KillTimer(vCheckKeySwitchTimerHandle);
			DrawImageOnScreen();
			OnPaint();
			//save the image
			DWORD TempBufferSize = vLocalSystemData->vITIPCDig->vOriginalBufferSize;
			WORD TempBufferSizeX = vLocalSystemData->vITIPCDig->vOriginalBufferSizeX;
			WORD TempBufferSizeY = vLocalSystemData->vITIPCDig->vOriginalBufferSizeY;
			//BYTE *TempBuffer = NULL;
			/*
			if (ThereIsEnoughMemory(TempBufferSize, "Temp Image Buffer"))
			{
				TempBuffer = (BYTE *)malloc(TempBufferSize);
				if (TempBuffer)
				{
					if (vHaveAnImage)
					{
						if (vLocalSystemData->vITIPCDig->vOriginalImage)
							CopyMemory(TempBuffer,
								vLocalSystemData->vITIPCDig->vOriginalImage->vGreyImage,TempBufferSize);
					}
					else
						FillMemory(TempBuffer,TempBufferSize,255);
				}
			}
			*/

			if (vStartedSource)
			{
				if (vLocalSystemData->vLogFile.vLogSerialData)
					vLocalSystemData->vLogFile.WriteToLogFile("Setup Product Source running",cDebugMessage);
				CString TempText("");
				TempText.LoadString(IDS_XRAYON);
				SetDlgItemText(IDC_Calibrating,TempText);
				m_Calibrating.ShowWindow(SW_SHOW);
				m_PleaseWait.ShowWindow(SW_HIDE);
				KillTimer(vClearStartingIndicatorIfXRaysOnTimerHandle);
				vBlink = 255;
				//SetDlgItemText(IDC_Background, " ");
				vStartingUpSource = false;
			}
			else
				if (vLocalSystemData->vLogFile.vLogSerialData)
					vLocalSystemData->vLogFile.WriteToLogFile("Setup Product Source not running",cDebugMessage);
			vMainWindowPointer->SetupProduct(vGlobalCurrentProduct, vStartedSource);//leave source on
			//LoadBackGroundIfNeeded();

			if ((vLocalSystemData->vITIPCDig->vOriginalImage) && (vLocalSystemData->vITIPCDig->vOriginalImage->vGreyImage))
			{
				FillMemory(vLocalSystemData->vITIPCDig->vOriginalImage->vGreyImage,
					vLocalSystemData->vITIPCDig->vOriginalBufferSize,255);
/*
			//copy saved image back into dislpay 
				if (TempBuffer)
				{
					//if the image size did not change, copy the old image to the new buffer
					if ((TempBufferSizeX == vLocalSystemData->vITIPCDig->vOriginalBufferSizeX) &&
						(TempBufferSizeY == vLocalSystemData->vITIPCDig->vOriginalBufferSizeY))
						CopyMemory(vLocalSystemData->vITIPCDig->vOriginalImage->vGreyImage,
							TempBuffer,TempBufferSize);
					else
					{ //if the image size changed, copy the smaller dimensions of the image data
						//to the new buffer to display as good as can.
						WORD TempOriginalSizeX = TempBufferSizeX;
						if (TempBufferSizeX > vLocalSystemData->vITIPCDig->vOriginalBufferSizeX)
							TempBufferSizeX = vLocalSystemData->vITIPCDig->vOriginalBufferSizeX;
						if (TempBufferSizeY > vLocalSystemData->vITIPCDig->vOriginalBufferSizeY)
							TempBufferSizeY = vLocalSystemData->vITIPCDig->vOriginalBufferSizeY;

						for (WORD TempLoopY = 0; TempLoopY < TempBufferSizeY; TempLoopY++)
						{
							BYTE *TempOldImageDataPointer = TempBuffer + 
								(TempLoopY * TempOriginalSizeX);
							BYTE *TempNewImageBufferPointer = 
							vLocalSystemData->vITIPCDig->vOriginalImage->vGreyImage +
								(TempLoopY * vLocalSystemData->vITIPCDig->vOriginalBufferSizeX);
							for (WORD TempLoopX = 0; TempLoopX < TempBufferSizeX; TempLoopX++)
							{
								*TempNewImageBufferPointer++ = *TempOldImageDataPointer++;
							}
						}
					}
					free(TempBuffer);
				}
				*/
			}

			if (vMainWindowPointer->InterlocksOK())
			{
				int TimerResult = SetTimer(vStartTimerHandle,100,NULL);
				if (!TimerResult)
					ReportErrorMessage("Error-Start Timer Failed",cEMailInspx,32000);
			}
			UpdateButtonText();
			if (vMode == cSetImageSize)
			{
				//DisplayScale();
				DisplayLocationEdge();
			}
			WaitThenDrawImageOnScreen();
		}
	}
	else
  if (nIDEvent == vGrabFocusTimerHandle)  
  {
		KillTimer(vGrabFocusTimerHandle);
		SetForegroundWindow();
		vInitialized = true;
	}
	else
  if (nIDEvent == vGetBackgroundImageTimerHandle)  
  {
		KillTimer(vGetBackgroundImageTimerHandle);
		vWaitingForBackgroundImage = true;
	}
	else
  if (nIDEvent == vStartTimerHandle)  //called to run system, after setup, to allow uC time
  {
		KillTimer(vStartTimerHandle);
		if (vGlobalImageAquisitionThread)
		if (vMainWindowPointer->PrepareToRun())
		{
			if (vStartedSource)
			{
				vMainWindowPointer->StartRunning(false, false);//don't have to turn source on
				if (vLocalSystemData->vLogFile.vLogSerialData)
					vLocalSystemData->vLogFile.WriteToLogFile("Start running source already running",cDebugMessage);
				if (vLocalSystemData->vInAutoSetup)
					vLocalSystemData->vSystemRunMode = cAutoSetupRunningSystem;

				vRestartingXRaysWithNewSettings = false;
			}
			else
			{
				if (vLocalSystemData->vLogFile.vLogSerialData)
					vLocalSystemData->vLogFile.WriteToLogFile("Start running source not already running",cDebugMessage);
				vStartedSource = true;
				if (vLocalConfigurationData->vDemoMode)
				{
					vMainWindowPointer->StartRunning(true, false);//must turn on source
					if (vLocalSystemData->vInAutoSetup)
						vLocalSystemData->vSystemRunMode = cAutoSetupRunningSystem;
				}
				else
				{
					CString TempText("");
					TempText.LoadString(IDS_Starting);
					SetDlgItemText(IDC_Calibrating,TempText);
					m_Calibrating.ShowWindow(SW_SHOW);
					if (vLocalSystemData->vInAutoSetup)
						vLocalSystemData->vModeToRunAfterCalibrateOffset = cAutoSetupRunningSystem;
					else
						vLocalSystemData->vModeToRunAfterCalibrateOffset = cRunningSystemMode;

					vLocalSystemData->vCalibrateErrorCount = 0;
					//vMainWindowPointer->CalibrateADCDetectorOffset(true, false);
					vLocalSystemData->vCalibrateADCAfterDAC = true;
					vLocalSystemData->vAutoCalibrate = true;
					vLocalSystemData->vAutoCalibrateWhileRunning = false;
					vLocalSystemData->vFullDetectorCalibration = false;
					if (vLocalConfigurationData->vUseDACAndADCinDensityCompensation)
						vMainWindowPointer->CalibrateDACDetectorOffset();
					else
						vMainWindowPointer->CalibrateADCDetectorOffset(vLocalSystemData->vAutoCalibrate, false);
				}
			}
			int TimerResult = SetTimer(vCheckKeySwitchTimerHandle,20000,NULL);
			if (!TimerResult)
				ReportErrorMessage("Error-Check Key Switch Timer Failed",cEMailInspx,32000);
			//CString TempText;
			//TempText.LoadString(IDS_SystemRunning);
			//CString TempStringName(cSpace);
			//TempText = TempStringName + TempText;
			//CString TempText1;
			//TempText1.LoadString(IDS_Product);
			//TempStringName = TempStringName + vProductEditingName;
			//TempText = TempText + " " + TempText1 + ":" + TempStringName;  // System Running Product:
			//if (!vChangedEdgeFindingMode)
			//	SetStatusLine(TempText);
		}
		UpdateButtonText();
		m_Function1Button.ShowWindow(SW_SHOW);
		m_Function5Button.ShowWindow(SW_SHOW);
		UpdateFunction3Button();
	}

	if (nIDEvent == vWaitToUpdateScreenLocationEdgeTimerHandle)
	{
		KillTimer(vWaitToUpdateScreenLocationEdgeTimerHandle);

		DisplayLocationEdge();
	}
	if (nIDEvent == vWaitToUpdateScreenTimerHandle)
	{
		KillTimer(vWaitToUpdateScreenTimerHandle);

		vWaitingToUpdateScreen = false;
		DrawImageOnScreen();
	}

	if (nIDEvent == vCheckKeySwitchTimerHandle)
	{
		if (vLocalSystemData->vSystemRunMode != cCalibratingDetectorOffsetMode)
		{
			KillTimer(vCheckKeySwitchTimerHandle);
			vStartingUpSource = false;
			if (vLocalSystemData->vSystemRunMode == cStoppedSystemMode)
			{
				if (vLocalSystemData->vLogFile.vLogSerialData)
					vLocalSystemData->vLogFile.WriteToLogFile("Timer to check key switch, not running",cDebugMessage);
				vStartedSource = false;
				m_Calibrating.ShowWindow(SW_HIDE);
			}
			else
			{
				if (vLocalSystemData->vLogFile.vLogSerialData)
					vLocalSystemData->vLogFile.WriteToLogFile("Timer to check key switch, running",cDebugMessage);
				if (vLocalSystemData->vXRayOnNotice)
				{
					//CString TempText("");
					//TempText.LoadString(IDS_XRAYON);
					//SetDlgItemText(IDC_Calibrating,TempText);
					//m_Calibrating.ShowWindow(SW_SHOW);
					vStartedSource = true;
				}
			}

			UpdateButtonText();
			if (vNeedToCalibrateDetectors)
			{  //if image got taller, need to calibrate upper detectors
				vNeedToCalibrateDetectors = false;

				if (vLocalSystemData->vInAutoSetup)
					vLocalSystemData->vModeToRunAfterCalibrateOffset = cAutoSetupRunningSystem;
				else
					vLocalSystemData->vModeToRunAfterCalibrateOffset = cRunningSystemMode;

				vLocalSystemData->vCalibrateErrorCount = 0;
				//vMainWindowPointer->CalibrateADCDetectorOffset(false, true);
				vLocalSystemData->vCalibrateADCAfterDAC = true;
				vLocalSystemData->vAutoCalibrate = true;
				vLocalSystemData->vAutoCalibrateWhileRunning = false;
				vLocalSystemData->vFullDetectorCalibration = false;
				if (vLocalConfigurationData->vUseDACAndADCinDensityCompensation)
					vMainWindowPointer->CalibrateDACDetectorOffset();
				else
					vMainWindowPointer->CalibrateADCDetectorOffset(vLocalSystemData->vAutoCalibrate, false);
			}
		}
	}

	if (nIDEvent == vRestoreStatusLineTimerHandle)  
	{
		if (!vLocalSystemData->vYellowMessageButtonYellow)
		{
			//if ((!vLocalSystemData->vPipeIsEmpty) && (!vLocalSystemData->vAirBubblesInPipe)) //if cleared by image thread
			//{
			//	vShowingAirInPipe = false;
			//	vShowingPipeEmpty = false;
				SetDlgItemText(IDC_StatusLine,vCurrentStatusLineText);
				KillTimer(vRestoreStatusLineTimerHandle);
			//}
			//if ((vShowingPipeEmpty) && (!vLocalSystemData->vPipeIsEmpty) && (vLocalSystemData->vAirBubblesInPipe))
			//	vShowingPipeEmpty = false;
		}
	}

	if (nIDEvent == vStartSetupTimerHandle)  //done only when enter the window
	{
		WaitDisplayLocationEdge();
		bool TempOKToStart = true;
		KillTimer(vStartSetupTimerHandle);
		if (TempOKToStart)
		{
			if (vLocalSystemData->vLogFile.vLogSerialData)
				vLocalSystemData->vLogFile.WriteToLogFile("Setup product, Entered Auto Setup",cDebugMessage);
			int TimerResult = SetTimer(vOneSecondTimerHandle,1000,NULL);
			if (!TimerResult)
				ReportErrorMessage("Error-One Second Timer Failed",cEMailInspx,32000);
			//if have an average image, show it until aquire a new image
			if (vMainWindowPointer->InterlocksOK())
			{
				//value worked well trying shorter value 7/21/2004
				//int TimerResult = SetTimer(vStartTimerHandle,2200,NULL);
				int TimerResult = SetTimer(vStartTimerHandle,200,NULL);
				if (!TimerResult)
					ReportErrorMessage("Error-Start Timer Failed",cEMailInspx,32000);
			}
			if (vLocalSystemData->vITIPCDig->vProductAverageImage)
			{
				m_Background.ShowWindow(SW_HIDE);
				if (vMode == cSetImageSize)
					WaitDisplayLocationEdge();
			}
			DrawImageOnScreen();
			UpdateButtonText();
			//if (vMode == cSetImageSize)
			//	DisplayScale();
		}
	}
	else
	if (nIDEvent == vMouseDownRepeatTimerHandle)
	{
		KillTimer(vMouseDownRepeatTimerHandle);
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
		int TimerResult = SetTimer(vMouseDownRepeatTimerHandle, 100, NULL);
		if (!TimerResult)
			ReportErrorMessage("Error-Mouse Timer Failed",cEMailInspx,32000);
	}
	CDialog::OnTimer(nIDEvent);
}

void CAutoSetupImageDisplayDialog::OneSecondUpdate()
{
	if (vOldImageNumberToDisplay != vLocalSystemData->vImageNumberToDisplay)
	{
		vOldImageNumberToDisplay = vLocalSystemData->vImageNumberToDisplay;
		if ((vLocalSystemData->vSystemRunMode == cAutoSetupRunningSystem) || (vLocalSystemData->vSystemRunMode == cRunningSystemMode))
			DisplayANewImage();
	}

	if ((vLocalConfigurationData->vScanTracType == cFermataScanTrac) || (vLocalConfigurationData->vScanTracType == cLegatoScanTrac))
	if (vOldActualEncoderRate != vLocalSystemData->vActualEncoderRate)
	{
		if ((vOldActualEncoderRate > 5) && (vLocalSystemData->vActualEncoderRate < 5))
		{
			CString TempText = "Pump Stopped";
			BYTE TempLength = TempText.GetLength();
			m_RejectReason.SetWindowPos(NULL,ResizeXCoor(cMainBackgroundLeft + 2 + 6),	ResizeYCoor(cMainBackgroundTop + 2), ResizeXCoor(TempLength * 10),ResizeYCoor(19),SWP_NOZORDER);
			SetDlgItemText(IDC_RejectReason,TempText);
			m_RejectReason.ShowWindow(SW_SHOW);
		}
		vOldActualEncoderRate = vLocalSystemData->vActualEncoderRate;
	}

	//if (vOldCalibrationImageNumber != vLocalSystemData->vCalibrationImageNumber)
	//{
	//	vOldCalibrationImageNumber = vLocalSystemData->vCalibrationImageNumber;
	//	CString TempText;
	//	TempText.LoadString(IDS_Wait);
	//	m_Calibrating.ShowWindow(SW_SHOW);
	//	if (vLocalSystemData->vCalibrateUsingDAC)
	//		SetDlgItemText(IDC_Calibrating,TempText + " " + dtoa(vOldCalibrationImageNumber, 0));
	//	else
	//		SetDlgItemText(IDC_Calibrating,TempText + " " + dtoa(vOldCalibrationImageNumber + 12, 0));
	//}

	if (vOldXRayState != vLocalSystemData->vXRayState)
	{
		vOldXRayState = vLocalSystemData->vXRayState;
		CString TempText("");

		if (vLocalSystemData->vLogFile.vLogSerialData)
			vLocalSystemData->vLogFile.WriteToLogFile("Autosetup X-Ray Step: " + dtoa(vOldXRayState, 0),cDebugMessage);

		switch (vOldXRayState)
		{
			case 0:
			{
				m_Calibrating.ShowWindow(SW_HIDE);
			}
			break;
			case 24:
			{
				CString TempText("");
				if (vLocalSystemData->vXRayOnNotice)
				{
					TempText.LoadString(IDS_Wait);
				}
				else
				{
					TempText.LoadString(IDS_XRAY);
					TempText = TempText + " **";
				}
				SetDlgItemText(IDC_Calibrating,TempText);
				m_Calibrating.ShowWindow(SW_SHOW);
			}
			break;
			case 25:
				if (vLocalSystemData->vXRayOnNotice)
				{
					TempText.LoadString(IDS_XRAY);
					TempText = TempText + " Wait";
				}
				else
				{
					TempText.LoadString(IDS_XRAY);
					TempText = TempText + " **";
				}
				SetDlgItemText(IDC_Calibrating,TempText);
				m_Calibrating.ShowWindow(SW_SHOW);
			break;
			case 26:
			{
				if (vLocalSystemData->vXRayOnNotice)
				{
					KillTimer(vClearStartingIndicatorIfXRaysOnTimerHandle);
					TempText.LoadString(IDS_XRAYON);
					SetDlgItemText(IDC_Calibrating, TempText);
					m_Calibrating.ShowWindow(SW_SHOW);
		
					if (vGlobalCurrentProduct->vMultiLaneNumberOfLanes)
					{
						vLocalSystemData->vCurrentConveyorOutput = (vLocalSystemData->vCurrentConveyorOutput & 0xFFF8) | 0x08;  //pulse off, conveyor off, disable encoder
						vMainWindowPointer->Send0x9ACommand();
						Sleep(100);

						vLocalSystemData->vCurrentConveyorOutput = (vLocalSystemData->vCurrentConveyorOutput & 0xFFF7) | 0x07;  //on, enable encoder, start pulse
						vMainWindowPointer->Send0x9ACommand();

						SetStatusLine(" Send container in any lane...");
					}
					else
					if ((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode)) //Continuous Feed, or a Pipeline
					{
						SetStatusLine(" Waiting for Image...");
					}
					else
						SetStatusLine(" Send Container Through ScanTrac....");
					m_PleaseWait.ShowWindow(SW_HIDE);

					if (vMode == cSetImageSize)//subfunction5button
					{
						if (PasswordOK(cTemporaryInspxPassword,false))
							SetDlgItemText(IDC_SubFunction5Display, dtoa(vGlobalCurrentProduct->vProductImageHeightTop,2) + 
							" (" + dtoa(vGlobalCurrentProduct->vProductImageHeightTop * vGlobalPixelsPerUnitInHeight,0) + ")");
						else
							SetDlgItemText(IDC_SubFunction5Display, dtoa(vGlobalCurrentProduct->vProductImageHeightTop,2));

						if ((PasswordOK(cTemporaryInspxPassword,false)) && (vGlobalCurrentProduct->vProductImageHeightBottom))
							SetDlgItemText(IDC_SubFunction6Display, dtoa(vGlobalCurrentProduct->vProductImageHeightBottom,2) + " (" + dtoa(vGlobalCurrentProduct->vProductImageHeightBottom * vGlobalPixelsPerUnitInHeight,0) + ")");
						else
							SetDlgItemText(IDC_SubFunction6Display, dtoa(vGlobalCurrentProduct->vProductImageHeightBottom,2));
						
						if (PasswordOK(cTemporaryInspxPassword,false))
							SetDlgItemText(IDC_SubFunction3Display, dtoa(vGlobalCurrentProduct->vProductLockOutWidth,2) + " (" + dtoa(vGlobalCurrentProduct->vLockoutPosition, 0) + ")");
						else
							SetDlgItemText(IDC_SubFunction3Display, dtoa(vGlobalCurrentProduct->vProductLockOutWidth,2));

						if (PasswordOK(cTemporaryInspxPassword,false))
							SetDlgItemText(IDC_SubFunction8Display, dtoa(vGlobalCurrentProduct->vProductImageWidth,2) + " (" + dtoa(vGlobalCurrentProduct->vImageWidthPosition,0) + ")");
						else
							SetDlgItemText(IDC_SubFunction8Display, dtoa(vGlobalCurrentProduct->vProductImageWidth,2));
					}
				}
			}
			break;
			default:
				CString TempXRayString = " ";
				GetDlgItemText(IDC_Calibrating, TempXRayString);
				if (TempXRayString != "X-RAY ON")
				{
					CString TempText("");
					TempText.LoadString(IDS_XRAY);
					SetDlgItemText(IDC_Calibrating,TempText + " " + dtoa(vOldXRayState + 13, 0));
					m_Calibrating.ShowWindow(SW_SHOW);
				}
			break;
		}
	}

	if (vBlink < 128)
	{
		if (vBlink)
			vBlink = 0;
		else
			vBlink = 1;
		if (vBlink)
			SetDlgItemText(IDC_PleaseWait,  _T("Starting X-Rays.... Please Wait....."));
		else
			SetDlgItemText(IDC_PleaseWait,  _T("Starting X-Rays.... Please Wait     "));
	}

	if (vOldGlobalPasswordOK != PasswordOK(cTemporaryInspxPassword,false))
	{
		vOldGlobalPasswordOK = PasswordOK(cTemporaryInspxPassword,false);
		if ((vShowAverage) || (vShowStructure) || (vShowVoidStructure) || (vShowContaminant2Structure) || (vShowHistogram))
		{
			vShowAverage = false;
			vShowStructure = false;
			vShowVoidStructure = false;
			//vShowGlassStructure = false;
			vShowContaminant2Structure = false;
			vShowHistogram = false;
			DrawImageOnScreen();
		}
		UpdateButtonText();
		DisplayWithPassword();

		CWnd *TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText1);
		if (TempWindow)
			TempWindow->Invalidate(false);
	}

	//if (vLocalSystemData->vPipeIsEmpty > 3)
	//if (!vShowingPipeEmpty)
	//	ShowPipeEmptyIndicator();


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

	if (vNeedToConfigureInspections)
	{
		vNeedToConfigureInspections = false;
		if (vMode == cSetROI)
			ConfigureInspections(true, true);
		else
		if (vMode == cSetInspections)
			ConfigureInspections(false, false);//evaluate, Edit ROI
		else
		if (vAutoSetupType == cEditInspectionAutoSetup)
			ConfigureInspections(true, false);
		else
		if (vMode == cSetVoidThreshold)
			ConfigureInspections(true, false);
		else
		if (vMode == cSetContaminantThreshold)
			ConfigureInspections(true, false);
		else
		if (vMode == cSetDensityThreshold)
			ConfigureInspections(true, false);
	}

	if ((PasswordOK(cTemporaryInspxPassword,false)) && (vLocalConfigurationData->vDriftCompensationEnabled) &&
		(vMode == cLearnMenu))
	{
		if ((vOldDriftCompensationADCOffset != vLocalSystemData->vDriftCompensationADCOffset) ||
			(vOldDriftCompensationDACOffset != vLocalSystemData->vDriftCompensationDACOffset))
		{
			vOldDriftCompensationADCOffset = vLocalSystemData->vDriftCompensationADCOffset;
			vOldDriftCompensationDACOffset = vLocalSystemData->vDriftCompensationDACOffset;
			CString TempSign = "";
			if (vLocalSystemData->vCurrentBrightnessAdjustment > 0)
				TempSign = "+";
			SetDlgItemText(IDC_SubFunction8Display,"Corr: " + TempSign + 
				dtoa(vLocalSystemData->vCurrentBrightnessAdjustment,0));
		}
	}

	if (vOldDriftCompensationLocked != vLocalSystemData->vITIPCDig->vDriftCompensationLocked)
	{
		vOldDriftCompensationLocked = vLocalSystemData->vITIPCDig->vDriftCompensationLocked;
		if (vOldDriftCompensationLocked)
			m_DensityNotLockedIndicator.ShowWindow(SW_HIDE);
		else
		{
			m_DensityNotLockedIndicator.ShowWindow(SW_SHOW);
			if (ResizeXCoor(1) >= 2)
			{
				vGraphicBitmap.DeleteObject();
				vGraphicBitmap.LoadBitmap(IDB_InspxGraphicYellow4xBigger);
				m_DensityNotLockedIndicator.SetBitmap(vGraphicBitmap);
				RECT TempRect;
				m_DensityNotLockedIndicator.GetWindowRect(&TempRect);
				TempRect.top -= 26;
				TempRect.bottom -= 23;
				m_DensityNotLockedIndicator.SetWindowPos(NULL, TempRect.left, TempRect.top, TempRect.right - TempRect.left, TempRect.bottom - TempRect.top, NULL);
			}
			else if (ResizeXCoor(10) >= 15)
			{
				vGraphicBitmap.DeleteObject();
				vGraphicBitmap.LoadBitmap(IDB_InspxGraphicYellow2xBigger);
				m_DensityNotLockedIndicator.SetBitmap(vGraphicBitmap);
			}
		}
	}

	if (vLocalSystemData->vYellowMessageButtonYellow != vOldButton2Yellow)
	{  //if just get an error message, display it and update buttons
		if (vLocalSystemData->vYellowMessageButtonYellow)
		{
			SetDlgItemText(IDC_StatusLine,vLocalSystemData->vLastErrorMessage);
			vOldErrorMessage = vLocalSystemData->vLastErrorMessage;
		}
		UpdateFunction3Button();
		vOldButton2Yellow = vLocalSystemData->vYellowMessageButtonYellow;
	}
	else
	if (vOldButton2Yellow)
	{ //if there is a new error message, then display it
		if (vOldErrorMessage != vLocalSystemData->vLastErrorMessage)
		{
			SetDlgItemText(IDC_StatusLine,vLocalSystemData->vLastErrorMessage);
			vOldErrorMessage = vLocalSystemData->vLastErrorMessage;
		}
	}

	if (vMode == cCenterExposureMenu)
	{
		//get integration time in mSec
		if (vLocalSystemData->vMaximumEncoderRate)
		if ((!vLocalSystemData->vFPGASimulateEncoderRate) && (!vLocalConfigurationData->vContinuousFeedEncoderRate))
			vPercentBeltSpeed = vLocalSystemData->vActualEncoderRate / vLocalSystemData->vMaximumEncoderRate * cMaximumConveyorSpeedDerating;
		else
			vPercentBeltSpeed = vLocalSystemData->vActualEncoderRate / vLocalSystemData->vMaximumEncoderRate * cMaximumConveyorSpeedDeratingWithSimulatedEncoder;

		CString TempString = dtoa(vPercentBeltSpeed,1) + "% of Max.";
		SetDlgItemText(IDC_SubFunction8Button3,TempString);
	}

	if (vOldXRayOnNotice != vLocalSystemData->vXRayOnNotice)
	{
		vOldXRayOnNotice = vLocalSystemData->vXRayOnNotice;
		if (vOldXRayOnNotice)
		{
			m_XRaysOn.ShowWindow(SW_SHOW);
			vRestartingXRaysWithNewSettings = false;
		}
		else
		{
			vStartingUpSource = false;
			m_XRaysOn.ShowWindow(SW_HIDE);
			if (vLocalSystemData->vSystemRunMode != cCalibratingDetectorOffsetMode)
			{
				vStartedSource = false;
				vLocalSystemData->vSystemRunMode = cStoppedSystemMode; //sense failure to start up and put in stopped mode
				m_Calibrating.ShowWindow(SW_HIDE);
			}
		}
		//source went on or off so update button 3 to run
		UpdateButtonText();
		UpdateFunction3Button();
	}

	if ((vLocalSystemData->vITIPCDig->vCalibrateBodyTriggerReferenceEnabled != vOldCalibrateBodyTriggerReferenceEnabled) &&
		(vOldCalibrateBodyTriggerReferenceEnabled))
	{  //end of collecting reference, Save data, collect average or stucture if needed
		vOldCalibrateBodyTriggerReferenceEnabled = false;
		SetChangeMade();
		//start next step
		if (vLocalConfigurationData->vAverageDisableCount)
		if (vLocalSystemData->vInAutoSetup)
		{
			StartAccumulatingAverage();
		}
		else
		if (vLocalConfigurationData->vStructureDisableCount)
		if (vLocalSystemData->vInAutoSetup)
		{
			StartAccumulatingStructure();
		}
		else
		{
			if (PasswordOK(cTemporaryInspxPassword,false))
			{
				m_Width2.ShowWindow(SW_HIDE);
				m_Reference.ShowWindow(SW_HIDE);
			}
		}
		UpdateButtonText();
	}
	if (vGlobalProcessQuickLearnImagesThread)
	{
		if (vProcessCount < 3)
			vProcessCount++;
		else
			vProcessCount = 0;
		CString TempTimeString("");
		switch (vProcessCount)
		{
			case 0: TempTimeString = "-"; break;
			case 1: TempTimeString = "\\"; break;
			case 2: TempTimeString = "I"; break;
			case 3: TempTimeString = "/"; break;
		}
		SetStatusLine(" Processing Quick Learn Images, Please Wait " + TempTimeString);
		vJustFinishedProcessingQuickLearn = true;
	}
	else
	if (vJustFinishedProcessingQuickLearn)
	{
		vJustFinishedProcessingQuickLearn = false;
		SetStatusLine(" Learn Complete");
		//if (vMode == cLearnMenu)
			UpdateAverageDensityDisplay();
	}

	if (vLocalSystemData->vTakingQuickLearnImages != vOldTakingQuickLearnImages)
	{
		if (vOldTakingQuickLearnImages)
		{
			if (!vGlobalProcessQuickLearnImagesThread)
			{
				vOldTakingQuickLearnImages = vLocalSystemData->vTakingQuickLearnImages;
				//if did not shut down x-rays because of key switch or interlock
				if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)
				{
					//just load the images from the new learn so you don't loose the last image you took // ChangedProductSettingsSoReload();
					vLocalSystemData->vITIPCDig->LoadProductDataImages(vGlobalCurrentProduct);
					UpdateAverageDensityDisplay();
					if (vLocalConfigurationData->vDriftCompensationEnabled)
						vLocalSystemData->vITIPCDig->vDriftCompensationLocked = false;
				}
				if (vLocalSystemData->vITIPCDig->vProductAverageImage)
				{
					ReportErrorMessage("Quick Learn Complete, Show Average", cUserAction,0);
					vShowAverage = true;
					vHaveAnImage = false;
					DrawImageOnScreen();
					UpdateButtonText();
					DisplayWithPassword();
					WaitThenDrawImageOnScreen();
				}

				if (vLocalSystemData->vLogFile.vLogSerialData)
					vLocalSystemData->vLogFile.WriteToLogFile("Process Quick Learn Done, Show learn",cDebugMessage);
			}
		}
		else
			vOldTakingQuickLearnImages = vLocalSystemData->vTakingQuickLearnImages;

	}

	if ((vLocalSystemData->vITIPCDig->vAverageEnabled != vOldAverageEnabled) &&
		(vOldAverageEnabled)) 
	//  wait for average density to be calculated if in autosetup
	if ((vGlobalCurrentProduct->vSetupHistoryAverageDensity) ||
		(!vLocalSystemData->vInAutoSetup))
	{  //end of collecting average, Save average image, collect stucture if needed
		vOldAverageEnabled = false;
		if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)
		{
			vSavedImageProcessingData = true;
			if (PasswordOK(cTemporaryInspxPassword,false))
			if (vLocalSystemData->vInAutoSetup)
			if (!vShowReturnToEditMenu)
				vMainWindowPointer->ResetCounters();

			UpdateAverageDensityDisplay();
			if (vGlobalCurrentProduct->vHistogramThreshold > 0)
			{
				if (vShowHistogram)
				{
					DrawImageOnScreen();
					if (vLocalConfigurationData->vStructureDisableCount)
						vOldStructureEnabled = true;
					UpdateButtonText();
				}
			}

			SetChangeMade();

			if (vLocalSystemData->vInAutoSetup)
			if (vLocalConfigurationData->vStructureDisableCount)
			{
				StartAccumulatingStructure();
				vOldStructureEnabled = vLocalSystemData->vITIPCDig->vStructureEnabled;
			}
			else
			{
				if (PasswordOK(cTemporaryInspxPassword,false))
				{
					vShowHistogram = false;
					//vShowGlassHistogram = false;
					vShowAverage = false;
					vShowStructure = false;
					vShowVoidStructure = false;
					//vShowGlassStructure = false;
					vShowContaminant2Structure = false;
					m_Width2.ShowWindow(SW_HIDE);
					m_Reference.ShowWindow(SW_HIDE);
					//vLocalSystemData->vFrameCount = 0;
				}
				//just load the images from the new learn so you don't loose the last image you took // ChangedProductSettingsSoReload();
				vLocalSystemData->vITIPCDig->LoadProductDataImages(vGlobalCurrentProduct);
			}
			if (vLocalConfigurationData->vDriftCompensationEnabled)
				vLocalSystemData->vKeepDensityFeedbackLoopLocked = true;
			//br htest  //did not lock after standard relearn while running
			//if (vLocalConfigurationData->vDriftCompensationEnabled)
			//	vLocalSystemData->vITIPCDig->vDriftCompensationLocked = false;
		}
		UpdateButtonText();
	}
	if ((vLocalSystemData->vITIPCDig->vAverageEnabled != vOldAverageEnabled) &&
		(!vOldAverageEnabled))
	{
		vOldAverageEnabled = vLocalSystemData->vITIPCDig->vAverageEnabled;
		UpdateButtonText();
	}

	if (vLocalSystemData->vITIPCDig->vStructureEnabled != vOldStructureEnabled)
	if (!vOldStructureEnabled)
	{
		vOldStructureEnabled = vLocalSystemData->vITIPCDig->vStructureEnabled;
		UpdateButtonText();
	}

	if (vLocalSystemData->vITIPCDig->vStructureEnabled != vOldStructureEnabled)
	if (vGlobalCurrentProduct->vSetupHistoryAverageDensity)
	if (vOldStructureEnabled)
	{
		//end of collecting Structure, Save Structure image collected
		if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)
		{
			if (PasswordOK(cTemporaryInspxPassword,false))
			{
				vShowHistogram = false;
				//vShowGlassHistogram = false;
				vShowAverage = false;
				vShowStructure = false;
				vShowVoidStructure = false;
				//vShowGlassStructure = false;
				vShowContaminant2Structure = false;
				m_Width2.ShowWindow(SW_HIDE);
				m_Reference.ShowWindow(SW_HIDE);
				//vLocalSystemData->vFrameCount = 0;
				if (vLocalSystemData->vInAutoSetup)
				if (!vShowReturnToEditMenu)
					vMainWindowPointer->ResetCounters();
			}

			//just load the images from the new learn so you don't loose the last image you took // ChangedProductSettingsSoReload();
			vLocalSystemData->vITIPCDig->LoadProductDataImages(vGlobalCurrentProduct);

			SetChangeMade();
		}
		vOldStructureEnabled = vLocalSystemData->vITIPCDig->vStructureEnabled;
		if (vLocalSystemData->vITIPCDig->vProductAverageImage)
		{
			ReportErrorMessage("Normal Learn Complete, Show Average", cUserAction,0);
			vShowAverage = true;
			vHaveAnImage = false;
			DrawImageOnScreen();
			UpdateButtonText();
			DisplayWithPassword();
			WaitThenDrawImageOnScreen();
		}
	}
	if ((vLocalSystemData->vMinimumGoodIndex >= cStandardDeviationSampleSize - 1) ||
		(vLocalSystemData->vMaximumBadIndex >= cStandardDeviationSampleSize - 1))
	if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)
	{
		if (vInspectionEditing)
		if ((vInspectionEditing->vFindGoodIntensity) ||
			(vInspectionEditing->vFindBadIntensity))
		{
			WriteCSVFiles();
			UpdateButtonText();
			vLocalSystemData->vMinimumGoodIndex = 0;
			vLocalSystemData->vMaximumBadIndex = 0;
		}
	}
}
/*
double CAutoSetupImageDisplayDialog::ConvertMaxConveyorSpeedToIntegrationTime
	(double TempMaxConveyorSpeed)
{
	double TempIntegrationTime = 0;
	double TempMaximumEncoderRate = ((TempMaxConveyorSpeed / 60) *
		vGlobalPixelsPerUnit);

	if (vLocalConfigurationData->vUseMetric)
		TempMaximumEncoderRate = TempMaximumEncoderRate * 1000;  //convert meters to mm
	else
		TempMaximumEncoderRate = TempMaximumEncoderRate * 12;  //convert feet to inches

	TempIntegrationTime = 1000 / TempMaximumEncoderRate;//convert seconds to Milliseconds and invert
	TempIntegrationTime = ATOF(dtoa(TempIntegrationTime - .149,1));
	if (TempIntegrationTime > 10)
		TempIntegrationTime = 10;

	return TempIntegrationTime;
}
*/

double CAutoSetupImageDisplayDialog::ConvertIntegrationTimeToMaxConveyorSpeed(WORD TempIntegrationCode)
{
	double TempMaxConveyorSpeed = 0;
	double TempMaximumEncoderRate = 0;

	TempMaximumEncoderRate =  vMainWindowPointer->CalculateMaximumEncoderRate(TempIntegrationCode);

	double TempMaximumFeetPerMinute = (TempMaximumEncoderRate / vGlobalPixelsPerUnit) * 60;

	//if (vLocalConfigurationData->vUseMetric)
	//	TempMaximumFeetPerMinute = TempMaximumFeetPerMinute / 1000;  //convert mm to meters
	//else
		TempMaximumFeetPerMinute = TempMaximumFeetPerMinute / 12;  //convert inches to feet

	if (vGlobalCurrentProduct->vOverScanMultiplier > 1)
		TempMaximumFeetPerMinute = TempMaximumFeetPerMinute / vGlobalCurrentProduct->vOverScanMultiplier;

	return ATOF(dtoa(TempMaximumFeetPerMinute,1));
}

void CAutoSetupImageDisplayDialog::DrawImageOnScreen()
{
	if ((vLocalSystemData->vITIPCDig->vProcessManipulationImage) && (vLocalSystemData->vITIPCDig->vProcessManipulationImage->vGreyImage))
	if ((vLocalSystemData->vITIPCDig->vOriginalImage) && (vLocalSystemData->vITIPCDig->vOriginalImage->vGreyImage))
	{
		vShowAlternateImage = false;
		if (vShowingBackground)
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
			int TempBitsPerPixel = 0;
			int TempType = 0;

			CFileStatus TempFileStatus;

			if (CFile::GetStatus(TempString, TempFileStatus))
			{
				CW2A TempFileNameString(TempString);
				Erflag TempError = im_file_get_size(TempFileNameString, IM_BMP_FILE, &TempHeight, &TempWidth, &TempBitsPerPixel, &TempType);
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
					TempError = im_file_read(vLocalSystemData->vITIPCDig->vASIDDScratchImage, TempFileNameString, IM_AUTO_FILE);
					if (TempError == IM_OK)
					{
						vLocalSystemData->vITIPCDig->ScaleImageSideways( vLocalSystemData->vITIPCDig->vDisplayImageSizeX, vLocalSystemData->vITIPCDig->vDisplayImageSizeY, 
							1, 1, 0, 0, vLocalSystemData->vITIPCDig->vASIDDScratchImage->pix, vLocalConfigurationData->vPixelsPerDetector, false);

						vShowAlternateImage = true;
					}
				}
			}
		}
		else
		if ((vShowAverage) && (vLocalSystemData->vITIPCDig->vAverageCount) &&
			(vLocalSystemData->vITIPCDig->vAverageImage))
		{
			if (ThereIsEnoughMemory(vLocalSystemData->vITIPCDig->vOriginalBufferSize, "Show Average Buffer"))
			{
				BYTE *TempBuffer;
				TempBuffer = (BYTE *)malloc(vLocalSystemData->vITIPCDig->vOriginalBufferSize);

				if (TempBuffer)
				{
					SetStatusLine("Learning Image: " + *vGlobalCurrentProduct->GetProductName());

					vLocalSystemData->vITIPCDig->CopyAverageBuffer(
						vLocalSystemData->vITIPCDig->vAverageImage,
						TempBuffer,
						vLocalSystemData->vITIPCDig->vOriginalBufferSize);

					vLocalSystemData->vITIPCDig->ScaleImageSideways(
						vLocalSystemData->vITIPCDig->vDisplayImageSizeX, vLocalSystemData->vITIPCDig->vDisplayImageSizeY, 
						1, 1, 0, 0, TempBuffer,vLocalConfigurationData->vPixelsPerDetector, false);

					free(TempBuffer);
					vShowAlternateImage = true;
				}
			}
		}
		else
		if ((vShowHistogram) && 
			(vLocalSystemData->vITIPCDig->vProductHistogramImage) && (vLocalSystemData->vITIPCDig->vProductHistogramImage->vGreyImage))
		{
			vLocalSystemData->vITIPCDig->ScaleImageSideways(
				vLocalSystemData->vITIPCDig->vDisplayImageSizeX, vLocalSystemData->vITIPCDig->vDisplayImageSizeY, 
				1, 1, 0, 0, vLocalSystemData->vITIPCDig->vProductHistogramImage->vGreyImage, vLocalConfigurationData->vPixelsPerDetector, false);

			vShowAlternateImage = true;
		}
		else
			/*
		if ((vShowGlassHistogram) && 
			(vLocalSystemData->vITIPCDig->vProductGlassHistogramImage))
		{
			vLocalSystemData->vITIPCDig->ScaleImageSideways(
				vLocalSystemData->vITIPCDig->vDisplayImageSizeX,
				vLocalSystemData->vITIPCDig->vDisplayImageSizeY, 
				1, 1, 0, 0, vLocalSystemData->vITIPCDig->vProductGlassHistogramImage->vGreyImage,
				vLocalConfigurationData->vPixelsPerDetector);
			vShowAlternateImage = true;
		}
		else
		*/
		if ((vShowStructure) && (vLocalSystemData->vITIPCDig->vStructureCount) && (vLocalSystemData->vITIPCDig->vStructureImage) && (vLocalSystemData->vITIPCDig->vStructureImage->vGreyImage))
		{
			vLocalSystemData->vITIPCDig->ScaleImageSideways(vLocalSystemData->vITIPCDig->vDisplayImageSizeX, vLocalSystemData->vITIPCDig->vDisplayImageSizeY, 
				1, 1, 0, 0, vLocalSystemData->vITIPCDig->vStructureImage->vGreyImage, vLocalConfigurationData->vPixelsPerDetector, false);

			vShowAlternateImage = true;
		}
		else
		if ((vShowStructure) && (vLocalSystemData->vITIPCDig->vProductStructureImage) && (vLocalSystemData->vITIPCDig->vProductStructureImage->vGreyImage))
		{
			vLocalSystemData->vITIPCDig->ScaleImageSideways(vLocalSystemData->vITIPCDig->vDisplayImageSizeX, vLocalSystemData->vITIPCDig->vDisplayImageSizeY, 1, 1, 0, 0, 
				vLocalSystemData->vITIPCDig->vProductStructureImage->vGreyImage, vLocalConfigurationData->vPixelsPerDetector, false);

			vShowAlternateImage = true;
		}
		else
		if ((vShowContaminant2Structure) && (vLocalSystemData->vITIPCDig->vProductContaminant2StructureImage) && (vLocalSystemData->vITIPCDig->vProductContaminant2StructureImage->vGreyImage))
		{
			vLocalSystemData->vITIPCDig->ScaleImageSideways(vLocalSystemData->vITIPCDig->vDisplayImageSizeX, vLocalSystemData->vITIPCDig->vDisplayImageSizeY, 
				1, 1, 0, 0, vLocalSystemData->vITIPCDig->vProductContaminant2StructureImage->vGreyImage, vLocalConfigurationData->vPixelsPerDetector, false);

			vShowAlternateImage = true;
		}
		else
		if ((vShowVoidStructure) && (vLocalSystemData->vITIPCDig->vStructureCount) && (vLocalSystemData->vITIPCDig->vVoidStructureImage) && (vLocalSystemData->vITIPCDig->vVoidStructureImage->vGreyImage))
		{
			vLocalSystemData->vITIPCDig->ScaleImageSideways(vLocalSystemData->vITIPCDig->vDisplayImageSizeX, vLocalSystemData->vITIPCDig->vDisplayImageSizeY, 
				1, 1, 0, 0, vLocalSystemData->vITIPCDig->vVoidStructureImage->vGreyImage, vLocalConfigurationData->vPixelsPerDetector, false);

			vShowAlternateImage = true;
		}
		else
		if ((vShowVoidStructure) && 
			(vLocalSystemData->vITIPCDig->vVoidProductStructureImage) && (vLocalSystemData->vITIPCDig->vVoidProductStructureImage->vGreyImage))
		{
			vLocalSystemData->vITIPCDig->ScaleImageSideways(vLocalSystemData->vITIPCDig->vDisplayImageSizeX, 
				vLocalSystemData->vITIPCDig->vDisplayImageSizeY, 1, 1, 0, 0, vLocalSystemData->vITIPCDig->vVoidProductStructureImage->vGreyImage, 
				vLocalConfigurationData->vPixelsPerDetector, false);

			vShowAlternateImage = true;
		}
		else
		if (((vShowAverage) || (!vHaveAnImage)) &&
			(vLocalSystemData->vITIPCDig->vProductAverageImage) && (vLocalSystemData->vITIPCDig->vProductAverageImage->vGreyImage))
		{
			SetStatusLine("Learned Image: " + *vGlobalCurrentProduct->GetProductName());

			vLocalSystemData->vITIPCDig->ScaleImageSideways(
				vLocalSystemData->vITIPCDig->vDisplayImageSizeX, vLocalSystemData->vITIPCDig->vDisplayImageSizeY, 
				1, 1, 0, 0, vLocalSystemData->vITIPCDig->vProductAverageImage->vGreyImage, vLocalConfigurationData->vPixelsPerDetector, false);

			vShowAlternateImage = true;
		}
		else
		if (vHaveAnImage)
		{  //show acquired image

/*
			if (vMode == cSetImageSize)
			if (!vAskedIfShouldRemoveBackground)
			if (vLocalSystemData->vInAutoSetup)
			{
				if ((vAutoSetupType == cNewAutoSetup) || (vAutoSetupType ==cCopyAutoSetup))
				{
					vLocalSystemData->vITIPCDig->DeleteLearnImages();
					//im_delete(vLocalSystemData->vITIPCDig->vProductBackgroundImage);
					//vLocalSystemData->vITIPCDig->vProductBackgroundImage = NULL;
				}
				if (!vLocalSystemData->vITIPCDig->vProductBackgroundImage)
				{
					//calculate if background is saturated, 
					//if not ask operator if want to remove background
					BYTE *TempPointer = vLocalSystemData->vITIPCDig->vOriginalImage->vGreyImage;
					DWORD TempSaturatedCount = 0;
					for (DWORD TempLoop = 0; TempLoop < vLocalSystemData->vITIPCDig->vOriginalBufferSize;
						TempLoop++)
					{
						if (*TempPointer++ == 255)
							TempSaturatedCount++;
					}
					if (TempSaturatedCount < vLocalSystemData->vITIPCDig->vOriginalBufferSize * .2)
					{
						CYesNoDialog TempYesNoDialog;
						//CString TempText("");
						//TempText.LoadString(IDS_AutoSetupInProcess);
						TempYesNoDialog.vNoticeText = "\nThe Background is not white,\nBackground Removal for Autosize is recommended.\nRemove Background for AutoSize?";
						TempYesNoDialog.vQuestionType = cConfirmAbandon;
						TempYesNoDialog.vYesButtonText = "Remove Background";
						TempYesNoDialog.vNoButtonText = "Proceed As Is";
						int TempResult = TempYesNoDialog.DoModal();
						if (TempResult == IDOK)
						{
							vLocalSystemData->vRemoveBackgroundInAutoSize = true;
							TempYesNoDialog.vNoticeText = "\n\nAssure no containers are going through ScanTrac\nso background can be measured";
							TempYesNoDialog.vQuestionType = cConfirmAbandon;
							TempYesNoDialog.vYesButtonText = "Ready To Measure";
							TempYesNoDialog.vNoButtonText = "Cancel";
							TempResult = TempYesNoDialog.DoModal();
							if (TempResult == IDOK)
							{
								TakeBackgroundImageSample();

								vAskedIfShouldRemoveBackground = true;
							}
						}
						else
							vAskedIfShouldRemoveBackground = true;
					}
				}
			}
			*/
			if ((vMode == cSetContaminantThreshold) || (vMode == cSetVoidThreshold) ||
				((vMode == cSetSizeSettings) && (vShowImageInColor)))
			{
				//make new image, copy data from original image, process, scale, display
				if ((vLocalSystemData->vITIPCDig->vProcessManipulationImage) && 
					(vLocalSystemData->vITIPCDig->vProcessManipulationImage->vGreyImage))
				{
					ProcessTheImage();

					//only for testing CopyMemory(vLocalSystemData->vITIPCDig->vProcessManipulationImage->vGreyImage, vLocalSystemData->vITIPCDig->vOriginalImage->vGreyImage, vLocalSystemData->vITIPCDig->vOriginalBufferSize);

					vLocalSystemData->vITIPCDig->ScaleImageSideways(vLocalSystemData->vITIPCDig->vDisplayImageSizeX, vLocalSystemData->vITIPCDig->vDisplayImageSizeY, 1, 1, 0, 0, 
						vLocalSystemData->vITIPCDig->vProcessManipulationImage->vGreyImage, vLocalConfigurationData->vPixelsPerDetector, false);
				}
			}
			else
			{
				vLocalSystemData->vITIPCDig->ScaleImageSideways(vLocalSystemData->vITIPCDig->vDisplayImageSizeX,
					vLocalSystemData->vITIPCDig->vDisplayImageSizeY, vYScale, vXScale, vXOffset, vYOffset, 
					vLocalSystemData->vITIPCDig->vOriginalImage->vGreyImage, vLocalConfigurationData->vPixelsPerDetector, ((vMode == cSetImageSize) && (vGlobalScanTracType != cBriosoScanTrac)));
			}
		}

		if ((vShowAlternateImage) || (vHaveAnImage))
		{
			if (vShowAlternateImage) //hide background if may be out.
			if (!vHaveAnImage)
			{
				m_Background.ShowWindow(SW_HIDE);
				if (vMode == cSetImageSize)
					WaitDisplayLocationEdge();
			}

			if ((vMode == cSetImageSize) || (vMode == cSetSizeSettings))
			if (vXScale == 1)
			if (vYScale == 1)
			//if ((PasswordOK(cTemporaryInspxPassword,false)) || ((!vLocalConfigurationData->vContinuousFeedContainerRate) && (!vLocalConfigurationData->vBulkProductMode) && (!vGlobalCurrentProduct->vBulkProductMode) && 
			//	(vLocalConfigurationData->vScanTracType != cBriosoScanTrac) && (vLocalConfigurationData->vScanTracType != cAtempoScanTrac) && (vLocalConfigurationData->vScanTracType != cForteScanTrac) &&
			//	(vLocalConfigurationData->vScanTracType != cCaseInspectorScanTrac))) //Not Continuous Feed, Not a Pipeline or password in
			{
				if ((PasswordOK(cTemporaryInspxPassword,false)) || ((!vLocalConfigurationData->vContinuousFeedContainerRate) && (!vLocalConfigurationData->vBulkProductMode) && (!vGlobalCurrentProduct->vBulkProductMode))) //Not Continuous Feed, Not a Pipeline or password in
				if (vHaveAnImage)
				if ((vMode == cSetImageSize) || (vMode == cSetSizeSettings))
				if (!vShowAlternateImage)
				{  //show found lines if setting size display
					BYTE TempShowReferenceAdjustedImage = false;
					//if (vMode == cSetSizeSettings)
					//	TempShowReferenceAdjustedImage = true;
					vLocalSystemData->vITIPCDig->AddLinesToImageSideways(  //draws found edges of container lines
						vLocalSystemData->vITIPCDig->vDisplayImageSizeX,
						vLocalSystemData->vITIPCDig->vDisplayImageSizeY, 
						1, 0, 0,TempShowReferenceAdjustedImage,
						vLocalSystemData->vITIPCDig->vOriginalImage, vInspectionEditing, vLocalConfigurationData->vScanTracType);
				}
				//draw scale lines
				//if (vMode == cSetImageSize)
				/*
					vLocalSystemData->vITIPCDig->AddScaleToImageSideways(
						vLocalSystemData->vITIPCDig->vDisplayImageSizeX,
						vLocalSystemData->vITIPCDig->vDisplayImageSizeY, 
						1, 0, 0);
						*/
			}
			if ((vMode == cSetROI) || (vMode == cSetContaminantThreshold) || 
				(vMode == cSetVoidThreshold) || (vMode == cSetDensityThreshold) || 
				(vMode == cCenterExposureMenu))
			//if ((PasswordOK(cTemporaryInspxPassword,false)) || ((!vLocalConfigurationData->vContinuousFeedContainerRate) && (!vLocalConfigurationData->vBulkProductMode) && (!vGlobalCurrentProduct->vBulkProductMode) && 
			//	(vLocalConfigurationData->vScanTracType != cBriosoScanTrac) && (vLocalConfigurationData->vScanTracType != cAtempoScanTrac) && (vLocalConfigurationData->vScanTracType != cForteScanTrac) &&
			//	(vLocalConfigurationData->vScanTracType != cCaseInspectorScanTrac))) //Not Continuous Feed, Not a Pipeline or password in
			{
				int TempShowReferenceAdjustedImage = false;

				if (vShowAlternateImage)
					vLocalSystemData->vITIPCDig->AddFixedROILinesToImageSideways(
						vLocalSystemData->vITIPCDig->vDisplayImageSizeX,
						vLocalSystemData->vITIPCDig->vDisplayImageSizeY, 
						1, 0, 0, 0, //TempShowReferenceAdjustedImage,
						NULL,//don't want to reference ROI lines, so send null image data
						vInspectionEditing, vGlobalCurrentProduct->vOverScanMultiplier,
						0,0);//vLocalConfigurationData->vReferenceROIsToEdges,0);  
				else
				if (vInspectionEditing)
					vLocalSystemData->vITIPCDig->AddFixedROILinesToImageSideways(
						vLocalSystemData->vITIPCDig->vDisplayImageSizeX,
						vLocalSystemData->vITIPCDig->vDisplayImageSizeY, 
						1, 0, 0,
						TempShowReferenceAdjustedImage | (vInspectionEditing->vReferenceToTop << 1) | (vInspectionEditing->vReferenceToBottom << 1),
						vLocalSystemData->vITIPCDig->vOriginalImage,//want to reference ROI lines, so send image data
						vInspectionEditing, vGlobalCurrentProduct->vOverScanMultiplier,
						vLocalConfigurationData->vReferenceROIsToEdges,0);
				else
					vLocalSystemData->vITIPCDig->AddFixedROILinesToImageSideways(
						vLocalSystemData->vITIPCDig->vDisplayImageSizeX,
						vLocalSystemData->vITIPCDig->vDisplayImageSizeY, 
						1, 0, 0,
						TempShowReferenceAdjustedImage,
						vLocalSystemData->vITIPCDig->vOriginalImage,//want to reference ROI lines, so send image data
						vInspectionEditing, vGlobalCurrentProduct->vOverScanMultiplier,
						vLocalConfigurationData->vReferenceROIsToEdges,0);
			}
			if (vMode == cSetContaminantThreshold)
			if (vInspectionEditing)
			if ((vInspectionEditing) && ((vInspectionEditing->vInspectionType == cItemCountInspection) || (vInspectionEditing->vInspectionType == cSizeInspection) || (vInspectionEditing->vInspectionType == cMissingLidInspectionOnSize)))
			{
				DWORD TempRedPixels = 0;

				WORD TempBottom = 0;
				WORD TempLeft = 0;
				WORD TempHeight = 0;
				WORD TempWidth = 0;
				BYTE TempAutoImproveState = 0; //not a learned inspection, so don't bring in ROI edges

				vLocalSystemData->vITIPCDig->GetInspectionROIBounds(vInspectionEditing, &TempBottom, &TempLeft, &TempHeight, &TempWidth, 
					vLocalSystemData->vITIPCDig->vOriginalImage->vLeftReferenceAdjust, vLocalConfigurationData->vReferenceROIsToEdges, TempAutoImproveState);

				//take threshold of image so just get pixels that are dark enough for size measurement
				//BYTE *TempPointer = NULL;
				//for (WORD TempXLoop = 0; TempXLoop < TempWidth; TempXLoop++)
				//{
				//	TempPointer = vLocalSystemData->vITIPCDig->vOriginalImage->vGreyImage + 
				//		TempBottom + (TempLeft + TempXLoop) * vLocalSystemData->vITIPCDig->vOriginalBufferSizeX;

				//	if (((vShowAverage) || (!vHaveAnImage)) && (vLocalSystemData->vITIPCDig->vProductAverageImage) && (vLocalSystemData->vITIPCDig->vProductAverageImage->vGreyImage))
				//	{
				//		TempPointer = vLocalSystemData->vITIPCDig->vProductAverageImage->vGreyImage + 
				//			TempBottom + (TempLeft + TempXLoop) * vLocalSystemData->vITIPCDig->vOriginalBufferSizeX;
				//	}

				//	for (WORD TempYLoop = 0; TempYLoop < TempHeight; TempYLoop++)
				//	{
				//		if (*TempPointer <= vInspectionEditing->vThreshold)  
				//		if (vInspectionEditing)
				//		{//if pixel is  darker than threshold
				//			TempRedPixels++;
				//		}
				//		TempPointer++;
				//	}
				//}
				//vLocalSystemData->vITIPCDig->vOriginalImage->vNumberOfBlobs[vInspectionNumberEditing] = TempRedPixels;
				SetDlgItemText(IDC_Reference,dtoa(vLocalSystemData->vITIPCDig->vOriginalImage->vNumberOfBlobs[vInspectionNumberEditing],0));
			}

			WORD TempOffsetX = (108 * vLocalSystemData->vGlobalMonitorSizeX) / cOldResolutionX;
			WORD TempOffsetY = (43 * vLocalSystemData->vGlobalMonitorSizeY) / cOldResolutionY;
			RECT TempRectangle;
			TempRectangle.left = TempOffsetX;
			TempRectangle.top = TempOffsetY;
			TempRectangle.right = TempOffsetX + vLocalSystemData->vITIPCDig->vDisplayImageSizeX + 10;
			TempRectangle.bottom = TempOffsetY + vLocalSystemData->vITIPCDig->vDisplayImageSizeY;
			// could set a timer to invalidate in 200 milliseconds
			this->InvalidateRect(&TempRectangle,false);
		}
	}
	vXScale = 1;
	vYScale = 1;
	vXOffset = 0;
	vYOffset = 0;
			
	if (vLocalSystemData->vITIPCDig->vDoBottomReferencing)
		DisplayLocationEdge();
}

bool CAutoSetupImageDisplayDialog::AskIfAbortAutoSetupDialog() 
{
	int TempResult = 0;
	bool TempExit = true;
	if ((vOldAverageEnabled) || (vOldStructureEnabled) || 
		(vOldCalibrateBodyTriggerReferenceEnabled) || (vOldTakingQuickLearnImages) || (vOldAdjustingBrightnessBeforeLearning))
	if (vLocalSystemData->vInAutoSetup)
	{
		TempExit = false;
		CYesNoDialog TempYesNoDialog;
		CString TempText("");
		TempText.LoadString(IDS_AutoSetupInProcess);
		TempYesNoDialog.vNoticeText = TempText;
		TempYesNoDialog.vQuestionType = cConfirmAbandon;
		TempResult = TempYesNoDialog.DoModal();
		if (TempResult == IDOK)
		{
			TempExit = true;
			vChangeMade = false;
		}
	}
	return TempExit;
}

void CAutoSetupImageDisplayDialog::ExitAutoSetupDialog(int TempExitCode) 
{
	vLocalSystemData->vMultiLaneInSetupLanePositionsMenu = false;

	//KillTimer(vSetupTimerHandle);
	KillTimer(vStartTimerHandle);
	KillTimer(vWaitUntilDoneRunningForReloadTimer);
	KillTimer(vCheckKeySwitchTimerHandle);
	KillTimer(vRestoreStatusLineTimerHandle);
	if (vMouseState > 0)
	{
		ReportErrorMessage("Screen exited, repeat ended",cAction, 32000);
		KillTimer(vMouseDownRepeatTimerHandle);
		vMouseState = 0;
	}

	bool TempOKToExit = true;
	bool TempWarn = true;
	if (TempExitCode == 10)
		TempWarn = false;

	if (vMode == cSetBackground)
		FinishBackground();
	else
	if (vMode == cSetImageSize)
		TempOKToExit = FinishSizeMenu(TempWarn, true);
	else
	if (vMode == cCenterExposureMenu)
		TempOKToExit = FinishCenterExposureMenu(false); //was TempWarn for go back if not finding edges
	else
		/*
	if (vMode == cEdgeExposureMenu)
		TempOKToExit = FinishEdgeExposureMenu(false); //was TempWarn for go back if not finding edges
	else
	*/
	if (vMode == cLearnMenu)
		TempOKToExit = FinishLearnMenu(TempWarn);
	else
	if (vMode == cSetROI)
		FinishROI(true);
	else
	if (vMode == cSetContaminantThreshold)
		FinishContaminantThreshold(TempWarn);
	else
	if (vMode == cSetVoidThreshold)
		FinishVoidThreshold(TempWarn);
	else
	if (vMode == cSetDensityThreshold)
		TempOKToExit = FinishDensityThreshold(TempWarn);

	if (TempOKToExit)
	{
		//set display size back to image display size
		vLocalSystemData->vITIPCDig->vDisplayImageSizeX = (WORD) ResizeXCoor(416);
		vLocalSystemData->vITIPCDig->vDisplayImageSizeY = (WORD) ResizeYCoor(360);

		if (vLocalSystemData->vITIPCDig->vImage)
			delete vLocalSystemData->vITIPCDig->vImage;
		vLocalSystemData->vITIPCDig->vImage = NULL;

		vLocalSystemData->vITIPCDig->SetupImageBuffer(
			vLocalSystemData->vITIPCDig->vDisplayImageSizeX, 
			vLocalSystemData->vITIPCDig->vDisplayImageSizeY);

		if (!vLocalSystemData->vITIPCDig->vImage)
		if (vLocalSystemData->vITIPCDig->vImageBuffer)
		if (ThereIsEnoughMemory(sizeof(CImgConn), "Frame Grabber Interface3"))
			vLocalSystemData->vITIPCDig->vImage = new CImgConn(
				vLocalSystemData->vITIPCDig->vImageBuffer,
				(WORD)vLocalSystemData->vITIPCDig->vDisplayImageSizeX,
				(WORD)vLocalSystemData->vITIPCDig->vDisplayImageSizeY,
				(WORD)vLocalSystemData->vITIPCDig->vBitsPerPixel, 
				vLocalSystemData->vITIPCDig->vWindowHandle);

		if (vLocalConfigurationData->vDriftCompensationEnabled)
			vLocalSystemData->vKeepDensityFeedbackLoopLocked = true;

//		m_Function1Button.ShowWindow(SW_HIDE);
//		this->InvalidateRect(&m_Function1Button.GetRectToInvalidateArrow());
		m_Function3Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_Function3Button.GetRectToInvalidateArrow());
		m_Function4Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_Function4Button.GetRectToInvalidateArrow());
		m_Function5Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_Function5Button.GetRectToInvalidateArrow());

		vLocalSystemData->vAdjustingSourceSettings = false;
		CString TempText;
		TempText.LoadString(IDS_PleaseWait);
		TempText = " " + TempText;
		SetStatusLine(TempText);

		vGlobalInAutoSize = false;
		vLocalSystemData->vITIPCDig->vImageDisplayOnlyRejects = vHoldImageDisplayOnlyRejects;
		if (vLocalSystemData->vMaximumBadData)
		{
			free(vLocalSystemData->vMaximumBadData);
			vLocalSystemData->vMaximumBadData = NULL;
		}
		if (vLocalSystemData->vMinimumGoodData)
		{
			free(vLocalSystemData->vMinimumGoodData);
			vLocalSystemData->vMinimumGoodData = NULL;
		}
		if (vLocalSystemData->vDensityDataD)
		{
			free(vLocalSystemData->vDensityDataD);
			vLocalSystemData->vDensityDataD = NULL;
		}
		vExitCode = TempExitCode;

		if (vGlobalCurrentProduct)
		if ((TempExitCode == 10) || (TempExitCode == 0))
		{
			if (vLocalSystemData->vInAutoSetup) 
				ReportErrorMessage("Abort Auto Setup Product: " + *vGlobalCurrentProduct->GetProductName(), cUserAction,0);
			else
				ReportErrorMessage("Abort Setup Product: " + *vGlobalCurrentProduct->GetProductName(), cUserAction,0);
					
			vLocalSystemData->vSetupANewProduct = false;
		}
		else
		if ((TempExitCode == 1) || (TempExitCode == 20))
		{
			//if (vLocalSystemData->vInAutoSetup) 
			//	ReportErrorMessage("Save and Exit Auto Setup Product: " + *vGlobalCurrentProduct->GetProductName(), cUserAction,0);
			//else
			//	ReportErrorMessage("Save and Exit Setup Product: " + *vGlobalCurrentProduct->GetProductName(), cUserAction,0);
			if (vAnyChangeMade)
				ReportErrorMessage("Save and Exit Setup: " + *vGlobalCurrentProduct->GetProductName() + " by operator", cUserChanges,0);
			else
			if (vLocalConfigurationData->vResynchronizeEjectors)
			if (vLocalSystemData->vInAutoSetup)
				SetChangeMade();  //indicate a change was made as may have updated end of line timeout
			if (vGlobalCurrentProduct)
			if (vAnyChangeMade)
				ReportErrorMessage("Save Setup: " + *vGlobalCurrentProduct->GetProductName() + " by operator", cUserChanges,0);

			if (vHoldCurrentProduct)
			{
				if (vHoldCurrentProduct->vBTToImagePosition != vGlobalCurrentProduct->vBTToImagePosition)
					ReportErrorMessage("Changed Trigger to Image Distance From: " + dtoa(vHoldCurrentProduct->vProductBodyTriggerToImageDistanceInInches,2) + " to " +
						dtoa(vGlobalCurrentProduct->vProductBodyTriggerToImageDistanceInInches,2), cUserChanges,0);

				if (vHoldCurrentProduct->vDriftDensityInspection)
				if (vGlobalCurrentProduct->vDriftDensityInspection)
				if ((vHoldCurrentProduct->vDriftDensityInspection->vROIPixelTop != vGlobalCurrentProduct->vDriftDensityInspection->vROIPixelTop) ||
					(vHoldCurrentProduct->vDriftDensityInspection->vROIPixelLeft != vGlobalCurrentProduct->vDriftDensityInspection->vROIPixelLeft) ||
					(vHoldCurrentProduct->vDriftDensityInspection->vROIPixelRight != vGlobalCurrentProduct->vDriftDensityInspection->vROIPixelRight) ||
					(vHoldCurrentProduct->vDriftDensityInspection->vROIPixelBottom != vGlobalCurrentProduct->vDriftDensityInspection->vROIPixelBottom))
					ReportErrorMessage("Changed Product Density ROI", cUserChanges,0);

				if (vHoldCurrentProduct->vEdgeLocationBottomPixel != vGlobalCurrentProduct->vEdgeLocationBottomPixel)
					ReportErrorMessage("Changed Edge Finding Bottom From: " + dtoa(vHoldCurrentProduct->vEdgeLocationBottom,2) + " to " +
						dtoa(vGlobalCurrentProduct->vEdgeLocationBottom, 2), cUserChanges, 0);

				if (vHoldCurrentProduct->vEdgeLocationHeightPixel != vGlobalCurrentProduct->vEdgeLocationHeightPixel)
					ReportErrorMessage("Changed Edge Finding Height From: " + dtoa(vHoldCurrentProduct->vEdgeLocationHeight,2) + " to " +
						dtoa(vGlobalCurrentProduct->vEdgeLocationHeight, 2), cUserChanges, 0);

				if (vHoldCurrentProduct->vBottomLocationLeftPixel != vGlobalCurrentProduct->vBottomLocationLeftPixel)
					ReportErrorMessage("Changed Edge Finding Bottom From: " + dtoa(vHoldCurrentProduct->vBottomLocationLeftPixel,2) + " to " +
						dtoa(vGlobalCurrentProduct->vBottomLocationLeftPixel, 2), cUserChanges, 0);

				if (vHoldCurrentProduct->vBottomLocationLengthPixel != vGlobalCurrentProduct->vBottomLocationLengthPixel)
					ReportErrorMessage("Changed Edge Finding Height From: " + dtoa(vHoldCurrentProduct->vBottomLocationLengthPixel,2) + " to " +
						dtoa(vGlobalCurrentProduct->vBottomLocationLengthPixel, 2), cUserChanges, 0);

				if (vHoldCurrentProduct->vNumberOfInspections != vGlobalCurrentProduct->vNumberOfInspections)
					ReportErrorMessage("Changed Number of Inspections From: " + dtoa(vHoldCurrentProduct->vNumberOfInspections,0) + " to " +
						dtoa(vGlobalCurrentProduct->vNumberOfInspections, 0), cUserChanges, 0);

				if (vHoldCurrentProduct->vXRayIntegrationTime != vGlobalCurrentProduct->vXRayIntegrationTime)
					ReportErrorMessage("Changed X-Ray Integration Time From: " + dtoa((vHoldCurrentProduct->vXRayIntegrationTime * .0004),2) + " to " +
						dtoa((vGlobalCurrentProduct->vXRayIntegrationTime * .0004), 2), cUserChanges,0);

				if (vHoldCurrentProduct->vXRaySourceCurrent != vGlobalCurrentProduct->vXRaySourceCurrent)
					ReportErrorMessage("Changed X-Ray Current From: " + dtoa(vHoldCurrentProduct->vXRaySourceCurrent, 2) + " to " +
						dtoa(vGlobalCurrentProduct->vXRaySourceCurrent, 2), cUserChanges,0);

				if (vHoldCurrentProduct->vXRaySourceVoltage != vGlobalCurrentProduct->vXRaySourceVoltage)
					ReportErrorMessage("Changed X-Ray Voltage From: " + dtoa(vHoldCurrentProduct->vXRaySourceVoltage * 6,2) + " to " +
						dtoa(vGlobalCurrentProduct->vXRaySourceVoltage * 6, 2), cUserChanges,0);

				if (vHoldCurrentProduct->vThresholdForContainerBounds != vGlobalCurrentProduct->vThresholdForContainerBounds)
					ReportErrorMessage("Changed Container Bounds Threshold From: " + dtoa(vHoldCurrentProduct->vThresholdForContainerBounds,0) + 
					" to " + dtoa(vGlobalCurrentProduct->vThresholdForContainerBounds, 0), cUserChanges,0);

				if (vHoldCurrentProduct->vProductLockOutWidth != vGlobalCurrentProduct->vProductLockOutWidth)
					ReportErrorMessage("Changed Re-trigger Lockout From: " + dtoa(vHoldCurrentProduct->vProductLockOutWidth,2) + 
					" to " + dtoa(vGlobalCurrentProduct->vProductLockOutWidth,2), cUserChanges,0);

				if (vHoldCurrentProduct->vProductImageHeightTop != vGlobalCurrentProduct->vProductImageHeightTop)
					ReportErrorMessage("Changed Height From: " + dtoa(vHoldCurrentProduct->vProductImageHeightTop,2) + 
					" to " + dtoa(vGlobalCurrentProduct->vProductImageHeightTop,2), cUserChanges,0);

				if (vHoldCurrentProduct->vProductImageHeightBottom != vGlobalCurrentProduct->vProductImageHeightBottom)
					ReportErrorMessage("Changed Bottom From: " + dtoa(vHoldCurrentProduct->vProductImageHeightBottom,2) + 
					" to " + dtoa(vGlobalCurrentProduct->vProductImageHeightBottom,2), cUserChanges,0);

				if (vHoldCurrentProduct->vProductImageWidth != vGlobalCurrentProduct->vProductImageWidth)
					ReportErrorMessage("Changed Width From: " + dtoa(vHoldCurrentProduct->vProductImageWidth,2) + 
					" to " + dtoa(vGlobalCurrentProduct->vProductImageWidth,2), cUserChanges,0);

				if (vHoldCurrentProduct->vNumberOfAuxiliaryDetectors != vGlobalCurrentProduct->vNumberOfAuxiliaryDetectors)
					ReportErrorMessage("Changed Number of Auxilary Detectors From: " + dtoa(vHoldCurrentProduct->vNumberOfAuxiliaryDetectors,0) + 
					" to " + dtoa(vGlobalCurrentProduct->vNumberOfAuxiliaryDetectors,0), cUserChanges,0);

				if (vHoldCurrentProduct->vDriftDensityInspection)
				if (vGlobalCurrentProduct->vDriftDensityInspection)
				{
					if (vHoldCurrentProduct->vDriftDensityInspection->vROIPixelLeft != vGlobalCurrentProduct->vDriftDensityInspection->vROIPixelLeft)
							ReportErrorMessage("Changed Product Density ROI Left from: " +
								dtoa(vHoldCurrentProduct->vDriftDensityInspection->vROIPixelLeft,0) + " to " + dtoa(vGlobalCurrentProduct->vDriftDensityInspection->vROIPixelLeft,0), cUserChanges,0);

					if (vHoldCurrentProduct->vDriftDensityInspection->vROIPixelRight != vGlobalCurrentProduct->vDriftDensityInspection->vROIPixelRight)
							ReportErrorMessage("Changed Product Density ROI Right from: " +
								dtoa(vHoldCurrentProduct->vDriftDensityInspection->vROIPixelRight,0) + " to " + dtoa(vGlobalCurrentProduct->vDriftDensityInspection->vROIPixelRight,0), cUserChanges,0);

					if (vHoldCurrentProduct->vDriftDensityInspection->vROIPixelTop != vGlobalCurrentProduct->vDriftDensityInspection->vROIPixelTop)
							ReportErrorMessage("Changed Product Density ROI Top from: " +
								dtoa(vHoldCurrentProduct->vDriftDensityInspection->vROIPixelTop,0) + " to " + dtoa(vGlobalCurrentProduct->vDriftDensityInspection->vROIPixelTop,0), cUserChanges,0);

					if (vHoldCurrentProduct->vDriftDensityInspection->vROIPixelBottom != vGlobalCurrentProduct->vDriftDensityInspection->vROIPixelBottom)
							ReportErrorMessage("Changed Product Density ROI Bottom from: " +
								dtoa(vHoldCurrentProduct->vDriftDensityInspection->vROIPixelBottom,0) + " to " + dtoa(vGlobalCurrentProduct->vDriftDensityInspection->vROIPixelBottom,0), cUserChanges,0);
				}

				for (BYTE TempLoop = 0; TempLoop < 2; TempLoop++)
				{
					if (vHoldCurrentProduct->vTooManyRejectsAlarmX[TempLoop] != vGlobalCurrentProduct->vTooManyRejectsAlarmX[TempLoop])
						ReportErrorMessage("Changed X of Y Alarm " + dtoa(TempLoop + 1,0) + " X From: " + dtoa(vHoldCurrentProduct->vTooManyRejectsAlarmX[TempLoop],0) + 
						" to " + dtoa(vGlobalCurrentProduct->vTooManyRejectsAlarmX[TempLoop],0), cUserChanges,0);

					if (vHoldCurrentProduct->vTooManyRejectsAlarmY[TempLoop] != vGlobalCurrentProduct->vTooManyRejectsAlarmY[TempLoop])
						ReportErrorMessage("Changed X of Y Alarm " + dtoa(TempLoop + 1, 0) + " Y From: " + dtoa(vHoldCurrentProduct->vTooManyRejectsAlarmY[TempLoop],0) + 
						" to " + dtoa(vGlobalCurrentProduct->vTooManyRejectsAlarmY[TempLoop],0), cUserChanges,0);

					if (vGlobalCurrentProduct->vTooManyRejectsAlarmX[TempLoop]) //if the alarm is configured 
					if (vHoldCurrentProduct->vTooManyRejectsAlarmDisableEjectors[TempLoop] != vGlobalCurrentProduct->vTooManyRejectsAlarmDisableEjectors[TempLoop])
						ReportErrorMessage("Changed X of Y Alarm " + dtoa(TempLoop + 1, 0) + " Disable From: " + dtoa(vHoldCurrentProduct->vTooManyRejectsAlarmDisableEjectors[TempLoop], 0) + 
							" to " + dtoa(vGlobalCurrentProduct->vTooManyRejectsAlarmDisableEjectors[TempLoop], 0), cUserChanges,0);

					if (vGlobalCurrentProduct->vTooManyRejectsAlarmX[TempLoop]) //if the alarm is configured 
					if (vHoldCurrentProduct->vInspectionAlarmSettings[TempLoop] != vGlobalCurrentProduct->vInspectionAlarmSettings[TempLoop])
						ReportErrorMessage("Changed X of Y Alarm " + dtoa(TempLoop + 1, 0) + " Selected Inspections From: " + 
							WordToHex((WORD)vHoldCurrentProduct->vInspectionAlarmSettings[TempLoop]) + " to " + WordToHex((WORD)vGlobalCurrentProduct->vInspectionAlarmSettings[TempLoop]), cUserChanges,0);
				}
				for (BYTE TempLoop = 0; TempLoop < cMaximumNumberOfInspections; TempLoop++)
				{
					if ((vHoldCurrentProduct->vInspection[TempLoop]) && (vGlobalCurrentProduct->vInspection[TempLoop]))
					{
						if (vHoldCurrentProduct->vInspection[TempLoop]->vName != vGlobalCurrentProduct->vInspection[TempLoop]->vName)
							ReportErrorMessage("Changed Inspection: " + dtoa(TempLoop + 1, 0) + ". " + vHoldCurrentProduct->vInspection[TempLoop]->vName + " to " + vGlobalCurrentProduct->vInspection[TempLoop]->vName, cUserChanges,0);
						else //don't compare settings if moved inspections around
						{
							if (vHoldCurrentProduct->vInspection[TempLoop]->vThreshold != vGlobalCurrentProduct->vInspection[TempLoop]->vThreshold)
							{
								if (vHoldCurrentProduct->vInspection[TempLoop]->vInspectionType == cOverfillByWeightInspection)
									ReportErrorMessage("Changed Inspection: " + dtoa(TempLoop + 1, 0) + " - " + vHoldCurrentProduct->vInspection[TempLoop]->vName + " Threshold From: " + 
									dtoa(vHoldCurrentProduct->vInspection[TempLoop]->ConvertToWeight(vHoldCurrentProduct->vInspection[TempLoop]->vDensityThresholdLower),0) + " to " + 
									dtoa(vHoldCurrentProduct->vInspection[TempLoop]->ConvertToWeight(vHoldCurrentProduct->vInspection[TempLoop]->vDensityThresholdLower),0), cUserChanges,0);
								else
								if (vHoldCurrentProduct->vInspection[TempLoop]->vInspectionType == cUnderfillByWeightInspection)
									ReportErrorMessage("Changed Inspection: " + dtoa(TempLoop + 1, 0) + " - " + vHoldCurrentProduct->vInspection[TempLoop]->vName + " Threshold From: " + 
									dtoa(vHoldCurrentProduct->vInspection[TempLoop]->ConvertToWeight(vHoldCurrentProduct->vInspection[TempLoop]->vDensityThresholdUpper),0) + " to " + 
									dtoa(vHoldCurrentProduct->vInspection[TempLoop]->ConvertToWeight(vHoldCurrentProduct->vInspection[TempLoop]->vDensityThresholdUpper),0), cUserChanges,0);
								else
								if (vHoldCurrentProduct->vInspection[TempLoop]->vInspectionType == cOverfillInspection)
									ReportErrorMessage("Changed Inspection: " + dtoa(TempLoop + 1, 0) + " - " + vHoldCurrentProduct->vInspection[TempLoop]->vName + " Threshold From: " + 
									dtoa(vHoldCurrentProduct->vInspection[TempLoop]->vDensityThresholdLower,0) + " to " + 
									dtoa(vHoldCurrentProduct->vInspection[TempLoop]->vDensityThresholdLower,0), cUserChanges,0);
								else
								if (vHoldCurrentProduct->vInspection[TempLoop]->vInspectionType == cUnderfillInspection)
									ReportErrorMessage("Changed Inspection: " + dtoa(TempLoop + 1, 0) + " - " + vHoldCurrentProduct->vInspection[TempLoop]->vName + " Threshold From: " + 
									dtoa(vHoldCurrentProduct->vInspection[TempLoop]->vDensityThresholdUpper,0) + " to " + 
									dtoa(vHoldCurrentProduct->vInspection[TempLoop]->vDensityThresholdUpper,0), cUserChanges,0);
								else
									ReportErrorMessage("Changed Inspection: " + dtoa(TempLoop + 1, 0) + " - " + vHoldCurrentProduct->vInspection[TempLoop]->vName + " Threshold From: " + 
										dtoa(vHoldCurrentProduct->vInspection[TempLoop]->vThreshold,0) + " to " + dtoa(vGlobalCurrentProduct->vInspection[TempLoop]->vThreshold,0), cUserChanges,0);
							}

							if (vHoldCurrentProduct->vInspection[TempLoop]->vSize != vGlobalCurrentProduct->vInspection[TempLoop]->vSize)
								ReportErrorMessage("Changed " + vHoldCurrentProduct->vInspection[TempLoop]->vName + " Minimum Size from: " + dtoa(vHoldCurrentProduct->vInspection[TempLoop]->vSize, 0) + " to " + dtoa(vGlobalCurrentProduct->vInspection[TempLoop]->vSize,0) + " by operator", cUserChanges,0);

							if (vHoldCurrentProduct->vInspection[TempLoop]->vROIPixelLeft != vGlobalCurrentProduct->vInspection[TempLoop]->vROIPixelLeft)
									ReportErrorMessage("Changed ROI Left for Inspection: " + dtoa(TempLoop + 1, 0) + " - " + vHoldCurrentProduct->vInspection[TempLoop]->vName + " From: " +
										dtoa(vHoldCurrentProduct->vInspection[TempLoop]->vROIPixelLeft,0) + " to " + dtoa(vGlobalCurrentProduct->vInspection[TempLoop]->vROIPixelLeft,0), cUserChanges,0);

							if (vHoldCurrentProduct->vInspection[TempLoop]->vROIPixelRight != vGlobalCurrentProduct->vInspection[TempLoop]->vROIPixelRight)
									ReportErrorMessage("Changed ROI Right for Inspection: " + dtoa(TempLoop + 1, 0) + " - " + vHoldCurrentProduct->vInspection[TempLoop]->vName + " From: " +
										dtoa(vHoldCurrentProduct->vInspection[TempLoop]->vROIPixelRight,0) + " to " + dtoa(vGlobalCurrentProduct->vInspection[TempLoop]->vROIPixelRight,0), cUserChanges,0);

							if (vHoldCurrentProduct->vInspection[TempLoop]->vROIPixelTop != vGlobalCurrentProduct->vInspection[TempLoop]->vROIPixelTop)
									ReportErrorMessage("Changed ROI Top for Inspection: " + dtoa(TempLoop + 1, 0) + " - " + vHoldCurrentProduct->vInspection[TempLoop]->vName + " From: " +
										dtoa(vHoldCurrentProduct->vInspection[TempLoop]->vROIPixelTop,0) + " to " + dtoa(vGlobalCurrentProduct->vInspection[TempLoop]->vROIPixelTop,0), cUserChanges,0);

							if (vHoldCurrentProduct->vInspection[TempLoop]->vROIPixelBottom != vGlobalCurrentProduct->vInspection[TempLoop]->vROIPixelBottom)
									ReportErrorMessage("Changed ROI Bottom for Inspection: " + dtoa(TempLoop + 1, 0) + " - " + vHoldCurrentProduct->vInspection[TempLoop]->vName + " From: " +
										dtoa(vHoldCurrentProduct->vInspection[TempLoop]->vROIPixelBottom,0) + " to " + dtoa(vGlobalCurrentProduct->vInspection[TempLoop]->vROIPixelBottom,0), cUserChanges,0);

							if (vHoldCurrentProduct->vInspection[TempLoop]->vSize != vGlobalCurrentProduct->vInspection[TempLoop]->vSize)
								ReportErrorMessage("Changed Inspection: " + dtoa(TempLoop + 1, 0) + " - " + vHoldCurrentProduct->vInspection[TempLoop]->vName + " Minimum Size From: " + 
									dtoa(vHoldCurrentProduct->vInspection[TempLoop]->vSize,0) + " to " + dtoa(vGlobalCurrentProduct->vInspection[TempLoop]->vSize,0), cUserChanges,0);
						}
					}
					else
					if (vHoldCurrentProduct->vInspection[TempLoop])
					{
						ReportErrorMessage("Deleted Inspection: " + dtoa(TempLoop + 1, 0) + " Name: " + vHoldCurrentProduct->vInspection[TempLoop]->vName, cUserChanges,0);
					}
					else
					if (vGlobalCurrentProduct->vInspection[TempLoop])
					{
						ReportErrorMessage("Added Inspection: " + dtoa(TempLoop + 1, 0) + " Name: " + 
							vGlobalCurrentProduct->vInspection[TempLoop]->vName, cUserChanges,0);
					}
				}

				for (BYTE TempLoop = 0; TempLoop < cNumberOfEjectors; TempLoop++)
				{
					if (vHoldCurrentProduct->vEjectorDistanceFromTriggerInInches[TempLoop] != vGlobalCurrentProduct->vEjectorDistanceFromTriggerInInches[TempLoop])
						ReportErrorMessage("Changed Ejector: " + dtoa(TempLoop + 1, 0) + " Position From: " + 
						dtoa(vHoldCurrentProduct->vEjectorDistanceFromTriggerInInches[TempLoop],2) + " to " +
							dtoa(vGlobalCurrentProduct->vEjectorDistanceFromTriggerInInches[TempLoop],2), cUserChanges,0);

					if (vHoldCurrentProduct->vEjectorDwellTime[TempLoop] != vGlobalCurrentProduct->vEjectorDwellTime[TempLoop])
						ReportErrorMessage("Changed Ejector: " + dtoa(TempLoop + 1, 0) + " Dwell Time From: " + 
							dtoa(vHoldCurrentProduct->vEjectorDwellTime[TempLoop],2) + " to " +
							dtoa(vGlobalCurrentProduct->vEjectorDwellTime[TempLoop],2), cUserChanges,0);
				}
				vMainWindowPointer->SelectValveMonitoringInspection();
			}
		}
		if (vLocalSystemData->vInAutoSetup) 
		{
			vMainWindowPointer->StopRunning(true);

			if (vLocalSystemData->vTotalContainers > 0)
			{
				//vMainWindowPointer->GenerateProductionReport(cAutoSetupProductionReport);
				vMainWindowPointer->ResetCounters();
				//vMainWindowPointer->SendProductionReport(cAutoSetupProductionReport);
			}

			int TimerResult = SetTimer(vWaitUntilDoneRunningForExitTimer,100,NULL);
			if (!TimerResult)
				ReportErrorMessage("Error-Done Running Timer Failed",cEMailInspx,32000);
		}
		else
		{  //edit while running

			if (vExitCode == 10) //hit abort, so restore data files if deleted
			{
				if (vMode == cLearnMenu)
				if ((vOldAverageEnabled) || (vOldStructureEnabled) || (vOldTakingQuickLearnImages) ||
					(vOldCalibrateBodyTriggerReferenceEnabled) || (vOldAdjustingBrightnessBeforeLearning))
				{
					OnSubFunction5Button();
				}

				vLocalSystemData->vAdjustingBrightnessBeforeLearning = false;
				vMainWindowPointer->vOldAdjustingBrightnessBeforeLearning = false;//abort to set false so will not start learning

				//in edit while running, could not be copy or new product, so just restore from edit
				RestoreOriginalProductSettings();
			}
			if (vHoldCurrentProduct)
			if (vHoldCurrentProduct != vGlobalCurrentProduct)
			{
				delete vHoldCurrentProduct;
				vHoldCurrentProduct = NULL;
			}

			vLocalSystemData->vITIPCDig->DeleteBackupLearnFiles();
			vLocalSystemData->vInAutoSetup = false;
			CDialog::EndDialog(vExitCode);
		}
	}
	vLocalSystemData->vChangedSizeOfProduct = false;
}

void CAutoSetupImageDisplayDialog::DoneRunningSoExit()
{  //not done if edit while running, only done in auto setup
	vStartingUpSource = true;

	if ((vExitCode != 1) && (vExitCode != 20))//1 = save and exit 
	{  //hit abort, so set no current product
		//vMainWindowPointer->SendSourceSettingsToUController(0,0);		
		if ((vAutoSetupType == cNewAutoSetup) || (vAutoSetupType ==cCopyAutoSetup)) 
		{//was a new product so delete data files and set no current product
			if (vLocalSystemData->vITIPCDig->vContainerCollectionLock.Lock())
			{
				vLocalSystemData->vITIPCDig->DeleteLearnFiles(vGlobalCurrentProduct);
				vLocalSystemData->vITIPCDig->DeleteBackupLearnFiles();
				delete vGlobalCurrentProduct;
				vGlobalCurrentProduct = NULL;
				vLocalSystemData->vITIPCDig->vContainerCollectionLock.Unlock();
			}
			CString TempText("");
			TempText.LoadString(IDS_NoCurrentProduct);
			vLocalSystemData->vCurrentProductName = TempText;
		}
		else
		{//aborted edit product, so restore original data files and get pointer to original product
			//if aborted, copy all product settings back to original product
			//no vGlobalCurrentProduct here if exit quickly

			//if have a checkweigh inspection, save its data, then fill data in copied inspection
			if (vLocalSystemData->vITIPCDig->vContainerCollectionLock.Lock())
			{
				CInspectionCheckWeigh *TempInspection[cMaximumNumberOfInspections];
				for (BYTE TempLoop = 0; TempLoop < cMaximumNumberOfInspections; TempLoop++)
					TempInspection[TempLoop] = NULL;

				for (BYTE TempLoop = 0; TempLoop < vHoldCurrentProduct->vNumberOfInspections; TempLoop++)
				if (vHoldCurrentProduct->vInspection[TempLoop])
				if ((vHoldCurrentProduct->vInspection[TempLoop]->vInspectionType == cCheckWeighInspection) || (vHoldCurrentProduct->vInspection[TempLoop]->vInspectionType == cOverfillByWeightInspection) ||
					(vHoldCurrentProduct->vInspection[TempLoop]->vInspectionType == cUnderfillByWeightInspection))
				{
					if (ThereIsEnoughMemory(sizeof(CInspectionCheckWeigh), "New Inspection"))
						TempInspection[TempLoop] = new CInspectionCheckWeigh;

					CInspectionCheckWeigh *TempInspectionToCopyPointer = (CInspectionCheckWeigh *)vHoldCurrentProduct->vInspection[TempLoop];
					TempInspection[TempLoop]->CopyInspection(TempInspectionToCopyPointer, vGlobalCurrentProduct->vOverScanMultiplier);
				}

				vGlobalCurrentProduct->CopyProduct(vHoldCurrentProduct);

				//restore any checkweigh data into its inspection
				for (WORD TempLoop = 0; TempLoop < vHoldCurrentProduct->vNumberOfInspections; TempLoop++)
				if (TempInspection[TempLoop])
				{
					vGlobalCurrentProduct->vInspection[TempLoop]->CopyInspection(TempInspection[TempLoop], vGlobalCurrentProduct->vOverScanMultiplier);
				}
				vLocalSystemData->vITIPCDig->vContainerCollectionLock.Unlock();
			}
			vLocalSystemData->vITIPCDig->RestoreLearnFilesFromBackups(vGlobalCurrentProduct);
			vLocalSystemData->vCurrentProductChanged = true;//force system to reload product settings
		}
		if (vLocalSystemData->vITIPCDig->vProductAverageImage != NULL)
		{
			delete vLocalSystemData->vITIPCDig->vProductAverageImage;
			vLocalSystemData->vITIPCDig->vProductAverageImage = NULL;
		}
	}
	else
	{
		WriteCSVFiles();
		vLocalSystemData->vCurrentProductChanged = true;
		vLocalSystemData->vEjectTooLateMessageGivenThisRun = false;
	}
	vLocalSystemData->vITIPCDig->vMainDisplayIsDefaultImage = false;
	if (vLocalSystemData->vInAutoSetup)
	if (!vShowReturnToEditMenu)
		vMainWindowPointer->ResetCounters();

	if (vHoldCurrentProduct)
	if (vHoldCurrentProduct != vGlobalCurrentProduct)
	{
		delete vHoldCurrentProduct;
		vHoldCurrentProduct = NULL;
	}

	vLocalSystemData->vInAutoSetup = false;
	CDialog::EndDialog(vExitCode);
}

void CAutoSetupImageDisplayDialog::SetChangeMade() 
{
	if (!vChangeMade)
	{
		vChangeMade = true;
		vAnyChangeMade = true;
	}
}

void CAutoSetupImageDisplayDialog::UpdateButtonText()
{
	CString TempText("");
	if ((vOldQualityFactor == 0)	||
		(!((vMode == cSetContaminantThreshold) || (vMode == cSetVoidThreshold) || 
		(vMode == cSetDensityThreshold))))
	//remove for mike and his demo for the manual
	if ((vLocalSystemData->vITIPCDig->vShowOnlyRealImages) && 
		(!vLocalConfigurationData->vDemoMode))
		SetDlgItemText(IDC_Simulating, _T(""));
	//remove for mike and his demo for the manual
	else
		SetDlgItemText(IDC_Simulating, _T("Sim"));

	if (vMode == CMultiLanePositionAdjustment)
	{
		SetDlgItemText(IDC_SubFunction1Display, dtoa(vMultiImageLaneEditing + 1, 0));
		SetDlgItemText(IDC_SubFunction5Display, dtoa(vGlobalCurrentProduct->vMultiLaneStartPixel[vMultiImageLaneEditing], 0));
		SetDlgItemText(IDC_SubFunction7Display, dtoa(vGlobalCurrentProduct->vMultiLaneWidthInPixels[vMultiImageLaneEditing], 0));
	}
	else
	if (vMode == cSetSizeSettings)
	{
		if (vAdjustingBottomEdgeFinding)
		{
			TempText.LoadString(IDS_Longer);
			SetDlgItemText(IDC_SubFunction5Button,TempText);
			TempText.LoadString(IDS_Shorter);
			SetDlgItemText(IDC_SubFunction6Button,TempText);
			TempText.LoadString(IDS_Right);
			SetDlgItemText(IDC_SubFunction7Button,TempText);
			TempText.LoadString(IDS_Left);
			SetDlgItemText(IDC_SubFunction8Button,TempText);

			if (PasswordOK(cTemporaryInspxPassword,false))
				SetDlgItemText(IDC_SubFunction5Display,dtoa(vGlobalCurrentProduct->vBottomLocationLength,2) + "(" + dtoa(vGlobalCurrentProduct->vBottomLocationLengthPixel,0) + ")");
			else
				SetDlgItemText(IDC_SubFunction5Display,dtoa(vGlobalCurrentProduct->vBottomLocationLength,2));

			if (PasswordOK(cTemporaryInspxPassword,false))
				SetDlgItemText(IDC_SubFunction5Display,dtoa(vGlobalCurrentProduct->vBottomLocationLength,2) + "(" + dtoa(vGlobalCurrentProduct->vBottomLocationLengthPixel,0) + ")");
			else
				SetDlgItemText(IDC_SubFunction5Display,dtoa(vGlobalCurrentProduct->vBottomLocationLength,2));

			if (PasswordOK(cTemporaryInspxPassword,false))
				SetDlgItemText(IDC_SubFunction7Display,dtoa(vGlobalCurrentProduct->vBottomLocationLeft,2) + "(" + dtoa(vGlobalCurrentProduct->vBottomLocationLeftPixel,0) + ")");
			else
				SetDlgItemText(IDC_SubFunction7Display,dtoa(vGlobalCurrentProduct->vBottomLocationLeft,2));

			if (PasswordOK(cTemporaryInspxPassword,false))
				SetDlgItemText(IDC_SubFunction7Display,dtoa(vGlobalCurrentProduct->vBottomLocationLeft,2) + "(" + dtoa(vGlobalCurrentProduct->vBottomLocationLeftPixel,0) + ")");
			else
				SetDlgItemText(IDC_SubFunction7Display,dtoa(vGlobalCurrentProduct->vBottomLocationLeft,2));
		}
		else
		{
			TempText.LoadString(IDS_Longer);
			SetDlgItemText(IDC_SubFunction5Button,TempText);
			TempText.LoadString(IDS_Shorter);
			SetDlgItemText(IDC_SubFunction6Button,TempText);
			TempText.LoadString(IDS_Up);
			SetDlgItemText(IDC_SubFunction7Button,TempText);
			TempText.LoadString(IDS_Down);
			SetDlgItemText(IDC_SubFunction8Button,TempText);

			if (PasswordOK(cTemporaryInspxPassword,false))
				SetDlgItemText(IDC_SubFunction5Display,dtoa(vGlobalCurrentProduct->vEdgeLocationHeight,2) + "(" + dtoa(vGlobalCurrentProduct->vEdgeLocationHeightPixel,0) + ")");
			else
				SetDlgItemText(IDC_SubFunction5Display,dtoa(vGlobalCurrentProduct->vEdgeLocationHeight,2));

			if (PasswordOK(cTemporaryInspxPassword,false))
				SetDlgItemText(IDC_SubFunction5Display,dtoa(vGlobalCurrentProduct->vEdgeLocationHeight,2) + "(" + dtoa(vGlobalCurrentProduct->vEdgeLocationHeightPixel,0) + ")");
			else
				SetDlgItemText(IDC_SubFunction5Display,dtoa(vGlobalCurrentProduct->vEdgeLocationHeight,2));

			if (PasswordOK(cTemporaryInspxPassword,false))
				SetDlgItemText(IDC_SubFunction7Display,dtoa(vGlobalCurrentProduct->vEdgeLocationBottom,2) + "(" + dtoa(vGlobalCurrentProduct->vEdgeLocationBottomPixel,0) + ")");
			else
				SetDlgItemText(IDC_SubFunction7Display,dtoa(vGlobalCurrentProduct->vEdgeLocationBottom,2));

			if (PasswordOK(cTemporaryInspxPassword,false))
				SetDlgItemText(IDC_SubFunction7Display,dtoa(vGlobalCurrentProduct->vEdgeLocationBottom,2) + "(" + dtoa(vGlobalCurrentProduct->vEdgeLocationBottomPixel,0) + ")");
			else
				SetDlgItemText(IDC_SubFunction7Display,dtoa(vGlobalCurrentProduct->vEdgeLocationBottom,2));
		}

		if (vGlobalCurrentProduct->vBulkProductMode)
			SetDlgItemText(IDC_Function2Button, _T("Use\nTrigger"));
		else
			SetDlgItemText(IDC_Function2Button, _T("Use Bulk Mode"));

		if ((vLocalSystemData->vITIPCDig->vDoBottomReferencing) && (!vGlobalCurrentProduct->vMultiLaneNumberOfLanes))
		{
			if (vAdjustingBottomEdgeFinding)
				SetDlgItemText(IDC_Function5Button, _T("Adjust Side Finder"));
			else
				SetDlgItemText(IDC_Function5Button, _T("Bottom Finder"));
		}
		else
		{
			if (vShowImageInColor)
				TempText.LoadString(IDS_DisplayGreyImage);
			else
				TempText.LoadString(IDS_DisplayColorImage);
			SetDlgItemText(IDC_Function5Button,TempText);
		}
	}
	else
	if (vMode == cSetImageSize)
	{
		if (vGlobalScanTracType == cBriosoScanTrac)//on Brioso, uController board is mounted on its side, so must swap X and Y from side view ScanTracs.
		{
			if (vLocalSystemData->vITIPCDig->vOriginalBufferSizeY == vLocalSystemData->vITIPCDig->vOriginalBufferSizeX)
			{
				SetDlgItemText(IDC_SubFunction5Button, _T("Right"));
				SetDlgItemText(IDC_SubFunction6Button, _T("Left"));
				SetDlgItemText(IDC_SubFunction8Button, _T("Height"));

				if (PasswordOK(cSuperInspxPassword,false))
					vSubFunction8ButtonEnable = true;		//m_SubFunction8Button.EnableWindow(true);
				else
					vSubFunction8ButtonEnable = false;		//m_SubFunction8Button.EnableWindow(false);
			}
			else
			{
				SetDlgItemText(IDC_SubFunction5Button, _T("Top"));
				SetDlgItemText(IDC_SubFunction6Button, _T("Bottom"));
				SetDlgItemText(IDC_SubFunction8Button, _T("Width"));
			}
		}
		if (vGlobalCurrentProduct->vMultiLaneNumberOfLanes)
		{
			TempText.LoadString(IDS_AdjustLanePositions);
			SetDlgItemText(IDC_SubFunction5Button, TempText);
			m_SubFunction2Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_SubFunction2Button.GetRectToInvalidateArrow());
			m_SubFunction3Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_SubFunction3Button.GetRectToInvalidateArrow());
			m_SubFunction3Display.ShowWindow(SW_HIDE);
			m_SubFunction6Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_SubFunction6Button.GetRectToInvalidateArrow());
			m_SubFunction6Display.ShowWindow(SW_HIDE);
			m_SubFunction7Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_SubFunction7Button.GetRectToInvalidateArrow());
			m_SubFunction7Display.ShowWindow(SW_HIDE);
		}

		if ((vHaveAnImage) && (vLocalSystemData->vInAutoSetup))
		{
			vSubFunction2ButtonEnable = true;		//m_SubFunction2Button.EnableWindow(true);
			m_SubFunction2Button.Invalidate(false);
		}
		else
		{
			vSubFunction2ButtonEnable = false;		//m_SubFunction2Button.EnableWindow(false);
			m_SubFunction2Button.Invalidate(false);
		}
		if ((PasswordOK(cTemporaryInspxPassword,false)) && (vLocalSystemData->vInAutoSetup))
		{
			m_Function4Button.ShowWindow(SW_SHOW);
			m_Function4Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
			if (vGlobalInAutoSize)
				SetDlgItemText(IDC_Function4Button, _T("Edges Left\nand Right"));
			else
				SetDlgItemText(IDC_Function4Button, _T("Find Edges\nRight Only"));
		}
		else
		{
			m_Function4Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_Function4Button.GetRectToInvalidateArrow());
		}
		if (vAutosizedContainer)
		{
			m_SubFunction4Button.ShowWindow(SW_SHOW);
			m_SubFunction4Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
			CString TempText = "-";
			TempText.LoadString(IDS_StartAutoSizeOver);
			SetDlgItemText(IDC_SubFunction4Button, TempText);
			m_SubFunction4Display.ShowWindow(SW_HIDE);
		}
		else
		if ((vLocalConfigurationData->vUseXScanDetectors) && (vLocalConfigurationData->vScanTracType != cForteScanTrac) && (vLocalConfigurationData->vScanTracType != cCaseInspectorScanTrac) && 
			((PasswordOK(cAdvancedSetupPassword,false)) || ((!vLocalConfigurationData->vContinuousFeedContainerRate) && (!vLocalConfigurationData->vBulkProductMode) && (!vGlobalCurrentProduct->vBulkProductMode)))) //Not Continuous Feed, Not a Pipeline or password in
		{
			m_SubFunction4Button.ShowWindow(SW_SHOW);
			m_SubFunction4Display.ShowWindow(SW_SHOW);
			m_SubFunction4Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
			SetDlgItemText(IDC_SubFunction4Button, _T("Resolution"));
			m_SubFunction4Display.ShowWindow(SW_SHOW);

			if (vGlobalCurrentProduct->vUsingXScanPoint4mmDetectors == 1)
			{
				//SetDlgItemText(IDC_SubFunction4Button, "Use .8mm Detector Resolution");
				SetDlgItemText(IDC_SubFunction4Display, _T(".4mm"));
			}
			else
			{
				//SetDlgItemText(IDC_SubFunction4Button, "Use .4mm Detector Resolution");
				SetDlgItemText(IDC_SubFunction4Display, _T(".8mm"));
			}
		}
		else
		{
			m_SubFunction4Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_SubFunction4Button.GetRectToInvalidateArrow());
		}
	}
	else
	if (vMode == cSetBackground)
	{
		m_SubFunction5Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);

		if (vLocalSystemData->vITIPCDig->vProductBackgroundImage)
		{
			//if (vLocalSystemData->vInAutoSetup)
				TempText.LoadString(IDS_NextToLearn);
			//else
			//	TempText.LoadString(IDS_ChangeLearn);
			m_SubFunction1Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
			m_SubFunction1Button.ShowWindow(SW_SHOW);

			if (vShowingBackground != 100)
			{
				m_SubFunction8Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
				m_SubFunction8Button.ShowWindow(SW_SHOW);
			}
			else
			{
				m_SubFunction8Button.ShowWindow(SW_HIDE);
				this->InvalidateRect(&m_SubFunction8Button.GetRectToInvalidateArrow());
			}
			if (vShowingBackground)
			{
				if ((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode)) //Continuous Feed, or a Pipeline
					SetDlgItemText(IDC_SubFunction8Button, _T("Show Product Image"));
				else
					SetDlgItemText(IDC_SubFunction8Button, _T("Show Container Image"));
			}
			else
				SetDlgItemText(IDC_SubFunction8Button, _T("Show Back- ground"));

			if (vShowingBackground != 100)
			if (vShowingBackground)
				SetStatusLine(" Showing Background");
			else
			if ((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode)) //Continuous Feed, or a Pipeline
				SetDlgItemText(IDC_SubFunction8Button, _T("Show Product Image"));
			else
				SetStatusLine(" Showing Container Image");
		}
		else
		{
			SetStatusLine(" Ready to sample Background");
			TempText = "Skip Background";
			m_SubFunction1Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_SubFunction1Button.GetRectToInvalidateArrow());
			m_SubFunction8Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_SubFunction8Button.GetRectToInvalidateArrow());
		}
		SetDlgItemText(IDC_Function5Button,TempText);
	}
	else
	if (vMode == cCenterExposureMenu)// || (vMode == cEdgeExposureMenu))
	{
		if (vShowImageInColor)
			TempText.LoadString(IDS_DisplayGreyImage);
		else
			TempText.LoadString(IDS_DisplayColorImage);

		SetDlgItemText(IDC_SubFunction3Button,TempText);

		m_SubFunction5Button.ShowNavigationArrow(NULL);
		m_SubFunction6Button.ShowNavigationArrow(NULL);
		m_SubFunction7Button.ShowNavigationArrow(NULL);
		m_SubFunction8Button.ShowNavigationArrow(NULL);
		m_SubFunction5Button.ShowWindow(SW_SHOW);
		this->InvalidateRect(&m_SubFunction5Button.GetRectToInvalidateArrow());
		m_SubFunction8Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction8Button.GetRectToInvalidateArrow());

		vSubFunction1ButtonEnable = true;		//m_SubFunction1Button.EnableWindow(true);

		if ((vLocalSystemData->vInAutoSetup) || (PasswordOK(cSuperInspxPassword,false)))
		{
			vSubFunction5ButtonEnable = true;		//m_SubFunction5Button.EnableWindow(true);
			vSubFunction6ButtonEnable = true;		//m_SubFunction6Button.EnableWindow(true);
		}
		else
		{
			vSubFunction5ButtonEnable = false;		//m_SubFunction5Button.EnableWindow(false);
			vSubFunction6ButtonEnable = false;		//m_SubFunction6Button.EnableWindow(false);
		}
	}
	else
	if (vMode == cLearnMenu)
	{
		if ((vOldAverageEnabled) || (vOldTakingQuickLearnImages) ||
			(vOldStructureEnabled) || (vOldCalibrateBodyTriggerReferenceEnabled) || (vOldAdjustingBrightnessBeforeLearning))
			TempText.LoadString(IDS_AbortIPAutoSetup);
		else
			TempText.LoadString(IDS_StartIPAutoSetup);
		SetDlgItemText(IDC_SubFunction5Button,TempText);

		if (((vOldAverageEnabled) || (vOldStructureEnabled) || (vOldTakingQuickLearnImages) ||
			(vOldCalibrateBodyTriggerReferenceEnabled) || (vOldAdjustingBrightnessBeforeLearning)) && (vLocalSystemData->vInAutoSetup))
		{
			vFunction1ButtonEnable = false;		//m_Function1Button.EnableWindow(false);
			vFunction2ButtonEnable = false;		//m_Function2Button.EnableWindow(false);
			vFunction4ButtonEnable = false;		//m_Function4Button.EnableWindow(false);
			vFunction5ButtonEnable = false;		//m_Function5Button.EnableWindow(false);
		}
		else
		{
			vFunction1ButtonEnable = true;		//m_Function1Button.EnableWindow(true);
			vFunction2ButtonEnable = true;		//m_Function2Button.EnableWindow(true);
			vFunction4ButtonEnable = true;		//m_Function4Button.EnableWindow(true);
			vFunction5ButtonEnable = true;		//m_Function5Button.EnableWindow(true);
		}
		m_Function1Button.Invalidate(false);
		m_Function2Button.Invalidate(false);
		m_Function4Button.Invalidate(false);
		m_Function5Button.Invalidate(false);
		if (vLocalSystemData->vITIPCDig->vProductAverageImage)
		{
				TempText.LoadString(IDS_NextToInspections);
		}
		else
		{
			if (vLocalSystemData->vInAutoSetup)
				TempText.LoadString(IDS_SkipAutoSetupStep);
			else
				TempText.LoadString(IDS_SkipSetupStep);
		}
		SetDlgItemText(IDC_Function5Button,TempText);
		if (PasswordOK(cTemporaryInspxPassword,false))
		{
			m_SubFunction6Button.ShowWindow(SW_SHOW);
		/*
			if (vLocalSystemData->vITIPCDig->vStructureEnabled)
			{  //if accumulating structure, show order backwards

				if (((!vShowHistogram) && (!vShowStructure) && (!vShowAverage)) &&
					(vLocalSystemData->vITIPCDig->vProductHistogramImage)))
					SetDlgItemText(IDC_SubFunction6Button,"Show Histogram Data");
				else
				if ((vShowHistogram) && (HaveAVoidStructure()))
					SetDlgItemText(IDC_SubFunction6Button,"Show\nVoid Structure");
				else
				if (((vShowHistogram) || (vShowVoidStructure)) && (HaveAContaminantStructure()))
					SetDlgItemText(IDC_SubFunction6Button,"Contam- inant Structure");
				else
				if ((!vShowAverage) && 
						((vLocalSystemData->vITIPCDig->vAverageImage) ||
						(vLocalSystemData->vITIPCDig->vProductAverageImage)))
					SetDlgItemText(IDC_SubFunction6Button,"Show Average Data");
				else
				if (vShowAverage)
					SetDlgItemText(IDC_SubFunction6Button,"Show Current Image");
				else
				{
					m_SubFunction6Button.ShowWindow(SW_HIDE);
					m_ArrowRight2.ShowWindow(SW_HIDE);
				}
			}
			else
			*/
			{
				if ((vShowAverage) && (HaveAContaminantStructure()))
				{
					SetDlgItemText(IDC_SubFunction6Button, _T("Contam- inant Structure"));
				}
				else
				if (((vShowAverage) || (vShowStructure)) && (vLocalSystemData->vITIPCDig->vProductContaminant2StructureImage))
				{
					SetDlgItemText(IDC_SubFunction6Button, _T("Contam- inant2 Structure"));
				}
				else
				if (((vShowAverage) || (vShowStructure) || (vShowContaminant2Structure)) && (HaveAVoidStructure()))
				{
					SetDlgItemText(IDC_SubFunction6Button, _T("Show\nVoid Structure"));
				}
				else
				if (((vShowContaminant2Structure) || (vShowAverage) || (vShowVoidStructure) || (vShowStructure)) && (vLocalSystemData->vITIPCDig->vProductHistogramImage))
				{
					SetDlgItemText(IDC_SubFunction6Button, _T("Show Histogram Data"));
				}
				else
				if ((vShowContaminant2Structure) || (vShowAverage) || (vShowVoidStructure) || (vShowStructure) || (vShowHistogram))//( || (vShowGlassHistogram))
				{
					SetDlgItemText(IDC_SubFunction6Button, _T("Show\nCurrent\nImage"));
				}
				else
				{
					SetDlgItemText(IDC_SubFunction6Button, _T("Show\nAverage\nData"));
				}
			}

			CString TempStringName(cSpace);
			TempStringName = TempStringName + vProductEditingName;
			if (PasswordOK(cTemporaryInspxPassword,false))
			{
				if (vShowStructure)
					SetStatusLine(" Contaminant Structure: " + TempStringName);
				else
				if (vShowVoidStructure)
					SetStatusLine(" Void Structure: " + TempStringName);
				else
				if (vShowContaminant2Structure)
					SetStatusLine(" Contaminant2 Structure: " + TempStringName);
				else
				if (vShowAverage)
					SetStatusLine(" Average: " + TempStringName);
				else
				if (vShowHistogram)
					SetStatusLine(" Histogram: " + TempStringName);
				else
					SetStatusLine(TempStringName);
			}
			else
			{
				if (vShowStructure)
					SetStatusLine(" Step 3a: " + TempStringName);
				else
				if (vShowVoidStructure)
					SetStatusLine(" Step 3v: " + TempStringName);
				else
				if (vShowContaminant2Structure)
					SetStatusLine(" Step 3t: " + TempStringName);
				else
				if (vShowAverage)
					SetStatusLine(" Step 2: " + TempStringName);
				else
				if (vShowHistogram)
					SetStatusLine(" Step4: " + TempStringName);
				else
					SetStatusLine(TempStringName);
			}
		}
		if ((vLocalSystemData->vTakingQuickLearnImages) ||
			(vOldAverageEnabled) || (vOldStructureEnabled) || 
			(vOldCalibrateBodyTriggerReferenceEnabled))
			vSubFunction7ButtonEnable = false;		//m_SubFunction7Button.EnableWindow(false);
		else
			vSubFunction7ButtonEnable = true;		//m_SubFunction7Button.EnableWindow(true);

		m_SubFunction7Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL); //3 lines of text
		CString TempString;
		if (vLocalConfigurationData->vNumberOfQuickLearnImagesToTake)
		{
			TempString.LoadString(IDS_EnableNormalLearn);
			SetDlgItemText(IDC_SubFunction7Button,TempString);
		}
		else
		{
			TempString.LoadString(IDS_EnableQuickLearn);
			SetDlgItemText(IDC_SubFunction7Button,TempString);
		}
	}
	else
	if (vMode == cSetROI)
	{
		if (vInspectionEditing)
		{
			if ((PasswordOK(cTemporaryInspxPassword,false)) && (vGlobalCurrentProduct->vReferenceTop))
			{  //show reference location from learned image
				SetDlgItemText(IDC_SubFunction1Display,dtoa(vInspectionEditing->vROITop,2) + "(" + dtoa(vInspectionEditing->vROIPixelTop,0) + ":" + dtoa(vGlobalCurrentProduct->vReferenceTop,0) + ")");

				SetDlgItemText(IDC_SubFunction3Display,dtoa(vInspectionEditing->vROIBottom,2) + "(" + dtoa(vInspectionEditing->vROIPixelBottom,0) + ":" + dtoa(vGlobalCurrentProduct->vReferenceBottom,0) + ")");

				SetDlgItemText(IDC_SubFunction5Display,dtoa(vInspectionEditing->vROILeft,2) + "(" + dtoa(vInspectionEditing->vROIPixelLeft,0) + ":" + dtoa(vGlobalCurrentProduct->vReferenceLeft,0) + ")");

				SetDlgItemText(IDC_SubFunction7Display,dtoa(vInspectionEditing->vROIRight,2) + "(" + dtoa(vInspectionEditing->vROIPixelRight,0) + ":" + dtoa(vGlobalCurrentProduct->vReferenceRight,0) + ")");
			}
			else
			if (PasswordOK(cTemporaryInspxPassword,false))
			{  //don't show reference location from learned image but show pixel values
				SetDlgItemText(IDC_SubFunction1Display,dtoa(vInspectionEditing->vROITop,2) + "(" + dtoa(vInspectionEditing->vROIPixelTop,0) + ")");

				SetDlgItemText(IDC_SubFunction3Display,dtoa(vInspectionEditing->vROIBottom,2) + "(" + dtoa(vInspectionEditing->vROIPixelBottom,0) + ")");

				SetDlgItemText(IDC_SubFunction5Display,dtoa(vInspectionEditing->vROILeft,2) + "(" + dtoa(vInspectionEditing->vROIPixelLeft,0) + ")");

				SetDlgItemText(IDC_SubFunction7Display,dtoa(vInspectionEditing->vROIRight,2) + "(" + dtoa(vInspectionEditing->vROIPixelRight,0) + ")");
			}
			else
			{ //don't show reference location from learned image
				SetDlgItemText(IDC_SubFunction1Display,dtoa(vInspectionEditing->vROITop,2));
				SetDlgItemText(IDC_SubFunction3Display,dtoa(vInspectionEditing->vROIBottom,2));
				SetDlgItemText(IDC_SubFunction5Display,dtoa(vInspectionEditing->vROILeft,2));
				SetDlgItemText(IDC_SubFunction7Display,dtoa(vInspectionEditing->vROIRight,2));
			}
		}
		if (vEditOnlyThisInspection)
		{
			SetDlgItemText(IDC_Function4Button, _T("Save and Exit"));
			m_Function2Button.ShowWindow(SW_HIDE);
			m_Function1Button.ShowWindow(SW_SHOW);
		}
	}
	else
	if ((vMode == cSetContaminantThreshold) || (vMode == cSetVoidThreshold))
	{
		//if (vLocalSystemData->vInAutoSetup)
			TempText.LoadString(IDS_NextToInspections);
		//else
		//	TempText.LoadString(IDS_ChangeInspections);
		if ((vAutoSetupType == cEvaluateAutoSetup) && 
			(vGlobalCurrentProduct->vNumberOfInspections == 1))
		{
			m_Function2Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
			if (vLocalSystemData->vInAutoSetup)
				TempText.LoadString(IDS_FinishAutoSetup);
			else
				TempText.LoadString(IDS_FinishSetup);
			SetDlgItemText(IDC_Function5Button,TempText);
		}
		else
			SetDlgItemText(IDC_Function5Button,TempText);
		if (vInspectionEditing)// &&
			//(vInspectionEditing->vInspectionType != cJarBottomContaminantInspection))
		{
			if (vInspectionEditing->vFindGoodIntensity)
				TempText.LoadString(IDS_StopProcessingGood);
			else
			{
				if ((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode))
					TempText = "Send Good Product";
				else
					TempText.LoadString(IDS_SendGoodContainers);
			}
			SetDlgItemText(IDC_SubFunction5Button,TempText);

			if (vInspectionEditing->vInspectionType == cJarBottomContaminantInspection)
			{
				m_SubFunction7Button.ShowWindow(SW_HIDE);
				this->InvalidateRect(&m_SubFunction7Button.GetRectToInvalidateArrow());
			}
			else
			{
				if ((vInspectionEditing) && (vInspectionEditing->vFindBadIntensity))
				{
					TempText.LoadString(IDS_StopProcessingBad);
					m_SubFunction7Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL); //3 lines of text
					m_SubFunction7Button.ShowWindow(SW_SHOW);
				}
				else
				if (PasswordOK(cAdvancedSetupPassword,false))
				{
					if ((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode))
						TempText = "Send Bad Product";
					else
						TempText.LoadString(IDS_SendBadContainersButton);
					m_SubFunction7Button.ShowWindow(SW_SHOW);
				}
				else
				{
					m_SubFunction7Button.ShowWindow(SW_HIDE);
					this->InvalidateRect(&m_SubFunction7Button.GetRectToInvalidateArrow());
				}
			}
			SetDlgItemText(IDC_SubFunction7Button,TempText);
		}
		else
		{
			m_SubFunction5Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_SubFunction5Button.GetRectToInvalidateArrow());
			m_SubFunction7Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_SubFunction7Button.GetRectToInvalidateArrow());
			m_SubFunction5Display.ShowWindow(SW_HIDE);
			CString TempText("");
			if ((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode))
				TempText = "Send Good Product";
			else
				TempText.LoadString(IDS_SendGoodContainers);
			SetStatusLine(" " + TempText);
		}

		if (((vInspectionEditing) && (vInspectionEditing->vFindBadIntensity)) || (vLocalSystemData->vMaximumBadAverage))
		{
			m_SubFunction8Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_SubFunction8Button.GetRectToInvalidateArrow());
			m_SubFunction8Button1.ShowWindow(SW_SHOW);
			m_SubFunction8Button2.ShowWindow(SW_SHOW);
			m_SubFunction8Button3.ShowWindow(SW_SHOW);
			m_SubFunction7Display.ShowWindow(SW_SHOW);
		}
		else
		{
			//m_SubFunction8Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
			//m_SubFunction8Button.ShowWindow(SW_SHOW);
			//m_ArrowRight4.ShowWindow(SW_SHOW);
			//TempText.LoadString(IDS_SetToDefaultROI);
			//SetDlgItemText(IDC_SubFunction8Button,TempText);
			m_SubFunction8Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_SubFunction8Button.GetRectToInvalidateArrow());
			m_SubFunction8Button1.ShowWindow(SW_HIDE);
			m_SubFunction8Button2.ShowWindow(SW_HIDE);
			m_SubFunction8Button3.ShowWindow(SW_HIDE);
			m_SubFunction7Display.ShowWindow(SW_HIDE);
		}
	}
	else
	if (vMode == cSetDensityThreshold) //update button text
	{
		if ((vInspectionEditing) && ((vInspectionEditing->vInspectionType == cCheckWeighInspection) || (vInspectionEditing->vInspectionType == cUnderfillByWeightInspection) || (vInspectionEditing->vInspectionType == cOverfillByWeightInspection)))
		{
			CString TempAbreviation = "-";
			if (vInspectionEditing->vCheckWeighUnits == "Ounces")
			{
				TempText.LoadString(IDS_MeasureInGrams);
				TempAbreviation = "oz";
			}
			else
			{	
				TempText.LoadString(IDS_MeasureInOunces);
				TempAbreviation = "g";
			}
			SetDlgItemText(IDC_Function2Button,TempText);

			//if (vInspectionEditing->vCheckWeighMaximumGoodWeight)
			if (vInspectionEditing->vCheckWeighMinimumGoodWeight)
				SetDlgItemText(IDC_SubFunction1Display, dtoa(vInspectionEditing->vCheckWeighMinimumGoodWeight,2) + TempAbreviation);
			else
				SetDlgItemText(IDC_SubFunction1Display,"In " + vInspectionEditing->vCheckWeighUnits);

			if (vInspectionEditing->vCheckWeighMaximumGoodWeight)
				SetDlgItemText(IDC_SubFunction4Display, dtoa(vInspectionEditing->vCheckWeighMaximumGoodWeight,2) + TempAbreviation);
			else
				SetDlgItemText(IDC_SubFunction4Display,"In " + vInspectionEditing->vCheckWeighUnits);

			if ((vInspectionEditing->vCollectingDensityAverage) && (!vCollectingBadDensityAverage))
				TempText.LoadString(IDS_StopSendingLight);
			else
				TempText.LoadString(IDS_SendLightSamples);
			SetDlgItemText(IDC_SubFunction5Button,TempText);

			if ((vInspectionEditing->vCollectingDensityAverage) && (vCollectingBadDensityAverage))
				TempText.LoadString(IDS_StopSendingHeavy);
			else
				TempText.LoadString(IDS_SendHeavySamples);
			m_SubFunction7Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
			SetDlgItemText(IDC_SubFunction7Button,TempText);

			if (!vInspectionEditing->vCheckWeighMinimumGoodWeight)
				vSubFunction5ButtonEnable = false;		//m_SubFunction5Button.EnableWindow(false);
			else
				vSubFunction5ButtonEnable = true;		//m_SubFunction5Button.EnableWindow(true);

			if (!vInspectionEditing->vCheckWeighMaximumGoodWeight)
				vSubFunction7ButtonEnable = false;			//m_SubFunction7Button.EnableWindow(false);
			else
				vSubFunction7ButtonEnable = true;			//m_SubFunction7Button.EnableWindow(true);

			if (vInspectionEditing)
			if (vInspectionEditing->vHasDensityUpperThreshold)
			if (((vInspectionEditing->vInspectionType == cUnderfillByWeightInspection) || (vInspectionEditing->vInspectionType == cCheckWeighInspection))	&& (!vInspectionEditing->vInspectionNameForWeightTrendingPointer) &&
				((!vInspectionEditing->vCheckWeighMinimumGoodWeight) || (!vInspectionEditing->vCheckWeighMaximumGoodWeight)))
			{
				vSubFunction2ButtonEnable = false;		//m_SubFunction2Button.EnableWindow(false);
				m_SubFunction2Button.Invalidate(false);
			}
			else
			{
				vSubFunction2ButtonEnable = true;		//m_SubFunction2Button.EnableWindow(true);
				m_SubFunction2Button.Invalidate(false);
			}
			if (vInspectionEditing)
			if (vInspectionEditing->vHasDensityLowerThreshold)
			if (((vInspectionEditing->vInspectionType == cOverfillByWeightInspection) || (vInspectionEditing->vInspectionType == cCheckWeighInspection)) && (!vInspectionEditing->vInspectionNameForWeightTrendingPointer) &&
				((!vInspectionEditing->vCheckWeighMinimumGoodWeight) || (!vInspectionEditing->vCheckWeighMaximumGoodWeight)))
				vSubFunction3ButtonEnable = false;		//m_SubFunction3Button.EnableWindow(false);
			else
				vSubFunction3ButtonEnable = true;		//m_SubFunction3Button.EnableWindow(true);
		}
		else
		{
			if ((vInspectionEditing) && (vInspectionEditing->vCollectingDensityAverage) && (!vCollectingBadDensityAverage))
				TempText.LoadString(IDS_StopAveragingDensity);
			else
			{
				if ((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode))
					TempText = "Send\nGood Product";
				else
					TempText.LoadString(IDS_StartAveragingDensity);
			}
			SetDlgItemText(IDC_SubFunction5Button,TempText);

			if ((vInspectionEditing) && (vInspectionEditing->vCollectingDensityAverage) && (vCollectingBadDensityAverage))
				TempText.LoadString(IDS_StopAveragingBad);
			else
			{
				if ((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode))
					TempText = "Send\nBad Product";
				else
					TempText.LoadString(IDS_StartAveragingBad);
			}
			m_SubFunction7Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
			SetDlgItemText(IDC_SubFunction7Button,TempText);
		}
		if ((vInspectionEditing) && (vInspectionEditing->vHasDensityLowerThreshold))
		{
			TempText.LoadString(IDS_ChangeUpperThreshold);
			if ((vInspectionEditing->vInspectionType == cUnderfillByWeightInspection) || (vInspectionEditing->vInspectionType == cCheckWeighInspection))
			{
				if ((vInspectionEditing->vCheckWeighMFactor == 1) && (!vInspectionEditing->vCheckWeighBFactor))
					TempText.LoadString(IDS_ChangeMinimumThreshold);
			}
			m_SubFunction2Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		}
		else
		{
			TempText.LoadString(IDS_Threshold);
			m_SubFunction2Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
		}
		SetDlgItemText(IDC_SubFunction2Button,TempText);

		if ((vInspectionEditing) && (vInspectionEditing->vHasDensityUpperThreshold))
		{
			TempText.LoadString(IDS_ChangeLowerThreshold);
			if ((vInspectionEditing->vInspectionType == cOverfillByWeightInspection) || (vInspectionEditing->vInspectionType == cCheckWeighInspection))
			{
				if ((vInspectionEditing->vCheckWeighMFactor == 1) && (!vInspectionEditing->vCheckWeighBFactor))
					TempText.LoadString(IDS_ChangeMaximumThreshold);
			}
			m_SubFunction3Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		}
		else
		{
			TempText.LoadString(IDS_Threshold);
			m_SubFunction3Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
		}
		SetDlgItemText(IDC_SubFunction3Button,TempText);
		if ((vInspectionEditing) && (vInspectionEditing->vInspectionType == cUnderfillByWeightInspection))
		{
			SetDlgItemText(IDC_SubFunction3Display, dtoa(vInspectionEditing->vCheckWeighNominalWeight));
			SetDlgItemText(IDC_SubFunction3Button, _T("Change Nominal Weight"));
		}
		if ((vInspectionEditing) && (vInspectionEditing->vInspectionType == cOverfillByWeightInspection))
		{
			SetDlgItemText(IDC_SubFunction2Display, dtoa(vInspectionEditing->vCheckWeighNominalWeight));
			SetDlgItemText(IDC_SubFunction2Button, _T("Change Nominal Weight"));
		}
		if (vEditOnlyThisInspection)
		{
			SetDlgItemText(IDC_Function5Button, _T("Save and Exit"));
			m_Function3Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_Function3Button.GetRectToInvalidateArrow());
			m_Function1Button.ShowWindow(SW_SHOW);
		}
	}
	CWnd *TempWindow = CWnd::GetDlgItem(IDC_Function5Button);
	if (TempWindow)
		TempWindow->Invalidate(false);

	if (vMode != cSetSizeSettings)
	if (vMode != cSetROI)
	if (vMode != cSetInspections)
	if (vMode != cInitializing)
	if (vMode != CMultiLanePositionAdjustment)
	if ((vMode != cSetDensityThreshold) && (vMode != cSetContaminantThreshold) && (vMode != cSetVoidThreshold))
	{
		if (vShowReturnToEditMenu)
		{
			m_Function1Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
			TempText.LoadString(IDS_AbortSetup);
		}
		else
		{
			m_Function1Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
			TempText.LoadString(IDS_Abort);
		}
		SetDlgItemText(IDC_Function1Button,TempText);
	}
	else
	{
		if ((vInspectionEditing) && ((vInspectionEditing->vInspectionType == cUnderfillByWeightInspection) || (vInspectionEditing->vInspectionType == cOverfillByWeightInspection)) &&
			(vGlobalCurrentProduct->AreThereAnyOtherWeightTrendingInspections(vInspectionNumberEditing)))
		{
			m_Function1Button.ShowWindow(SW_SHOW);
			m_Function1Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
			SetDlgItemText(IDC_Function1Button, _T("Trending Inspection"));
		}
		else
		if ((vInspectionEditing) && ((vInspectionEditing->vInspectionType == cItemCountInspection) || (vInspectionEditing->vInspectionType == cSizeInspection) || (vInspectionEditing->vInspectionType == cMissingLidInspectionOnSize)))
		{
			m_Function1Button.ShowWindow(SW_SHOW);
			m_Function1Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
			if (vInspectionEditing->vInspectionType == cItemCountInspection)
				SetDlgItemText(IDC_Function1Button, _T("Size\nSettings"));
			else
				SetDlgItemText(IDC_Function1Button, "Size\n" + dtoa(vInspectionEditing->vSize, 0));
		}
		//else
		//if (!vEditOnlyThisInspection)
		//{;
//			this->InvalidateRect(&m_Function1Button.GetRectToInvalidateArrow());
//			m_Function1Button.ShowWindow(SW_HIDE);

		//}
		//m_Function1Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		//if (vFreezeImageOnScreen)
		//	TempText.LoadString(IDS_ShowImages);
		//else
		//	TempText.LoadString(IDS_FreezeImage);
		//SetDlgItemText(IDC_Function1Button,TempText);
	}
}

void CAutoSetupImageDisplayDialog::DisplayWithPassword()
{
	if (vMode == cSetSizeSettings)
	{
		if (PasswordOK(cTemporaryInspxPassword,false))
		{
			m_SubFunction1Button.ShowWindow(SW_SHOW);
			m_SubFunction2Button.ShowWindow(SW_SHOW);
			m_SubFunction3Button.ShowWindow(SW_SHOW);
			m_SubFunction4Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
			m_SubFunction4Button.ShowWindow(SW_SHOW);
			SetDlgItemText(IDC_SubFunction1Button, _T("Add 10 to Container Threshold"));
			SetDlgItemText(IDC_SubFunction2Button, _T("Add 1 to Container Threshold"));
			SetDlgItemText(IDC_SubFunction3Button, _T("Container Threshold - 1"));
			SetDlgItemText(IDC_SubFunction4Button, _T("Container Threshold - 10"));
			m_SubFunction4DisplayA.ShowWindow(SW_SHOW);
			SetDlgItemText(IDC_SubFunction4DisplayA, dtoa(vGlobalCurrentProduct->vThresholdForContainerBounds,0));

			//m_Function2Button.ShowWindow(SW_SHOW);
			//m_Function2Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
			//SetDlgItemText(IDC_Function2Button,"Maximum Overlap");

			m_Function4Button.ShowWindow(SW_SHOW);
			m_Function4Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
			if (vLocalSystemData->vInAutoSetup)
				vFunction4ButtonEnable = true;		//m_Function4Button.EnableWindow(true);
			else
				vFunction4ButtonEnable = false;		//m_Function4Button.EnableWindow(false);
			SetDlgItemText(IDC_Function4Button, _T("Overscan"));
		}
		else
		{
			m_SubFunction1Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_SubFunction1Button.GetRectToInvalidateArrow());
			m_SubFunction2Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_SubFunction2Button.GetRectToInvalidateArrow());
			m_SubFunction3Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_SubFunction3Button.GetRectToInvalidateArrow());
			m_SubFunction4Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_SubFunction4Button.GetRectToInvalidateArrow());
			//m_Function2Button.ShowWindow(SW_HIDE);
			//this->InvalidateRect(&m_Function2Button.GetRectToInvalidateArrow());
			m_Function4Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_Function4Button.GetRectToInvalidateArrow());
			m_SubFunction4DisplayA.ShowWindow(SW_HIDE);
		}
	}
	if (vMode == cLearnMenu)
	{
		if (PasswordOK(cTemporaryInspxPassword,false))
		{
			m_SubFunction8Display.ShowWindow(SW_SHOW);
			CString TempSign = " ";
			if (vLocalSystemData->vCurrentBrightnessAdjustment > 0)
				TempSign = " +";
			SetDlgItemText(IDC_SubFunction8Display,"Corr:" + TempSign + 
				dtoa(vLocalSystemData->vCurrentBrightnessAdjustment,0));
			/*
			CString TempString = dtoa(vOldDriftCompensationADCOffset,4);
			TempString = "DO: " + TempString;
			SetDlgItemText(IDC_SubFunction8Display,TempString);
			*/
			SetDlgItemText(IDC_SubFunction4Display, dtoa(vLocalConfigurationData->vCalibrateBodyTriggerReferenceDisableCount, 0));
			m_SubFunction4Display.ShowWindow(SW_SHOW);
			m_SubFunction1Display.ShowWindow(SW_SHOW);
			SetDlgItemText(IDC_SubFunction1Display, dtoa(vLocalConfigurationData->vAverageDisableCount, 0));
			m_SubFunction2Display.ShowWindow(SW_SHOW);
			SetDlgItemText(IDC_SubFunction2Display, dtoa(vLocalConfigurationData->vStructureDisableCount,0));
			m_SubFunction3Display.ShowWindow(SW_SHOW);
			SetDlgItemText(IDC_SubFunction3Display, dtoa(vGlobalCurrentProduct->vHistogramThreshold,0));
			m_SubFunction1Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
			m_SubFunction2Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
			m_SubFunction3Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
			m_SubFunction4Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
			m_SubFunction1Button.ShowWindow(SW_SHOW);
			m_SubFunction2Button.ShowWindow(SW_SHOW);
			m_SubFunction3Button.ShowWindow(SW_SHOW);
			m_SubFunction4Button.ShowWindow(SW_SHOW);
			m_SubFunction6Button.ShowWindow(SW_SHOW);

			if ((vShowAverage) && (HaveAContaminantStructure()))
			{
				SetDlgItemText(IDC_SubFunction6Button, _T("Contam- inant Structure"));
			}
			else
			if (((vShowAverage) || (vShowStructure)) && (vLocalSystemData->vITIPCDig->vProductContaminant2StructureImage))
			{
				SetDlgItemText(IDC_SubFunction6Button, _T("Contam- inant2 Structure"));
			}
			else
			if (( (vShowAverage) || (vShowStructure) || (vShowContaminant2Structure)) && (HaveAVoidStructure()))
			{
				SetDlgItemText(IDC_SubFunction6Button, _T("Show\nVoid Structure"));
			}
			else
			if (((vShowContaminant2Structure) || (vShowAverage) || (vShowVoidStructure) || (vShowStructure)) && (vLocalSystemData->vITIPCDig->vProductHistogramImage))
			{
				SetDlgItemText(IDC_SubFunction6Button, _T("Show Histogram Data"));
			}
			else
			if ((vShowContaminant2Structure) || (vShowAverage) || (vShowVoidStructure) || (vShowStructure) || (vShowHistogram))//( || (vShowGlassHistogram))
			{
				SetDlgItemText(IDC_SubFunction6Button, _T("Show\nCurrent\nImage"));
			}
			else
			{
				SetDlgItemText(IDC_SubFunction6Button, _T("Show\nAverage\nData"));
			}

			CString TempStringName(cSpace);
			TempStringName = TempStringName + vProductEditingName;
			if (vShowStructure)
				SetStatusLine(" Contaminant Structure: " + TempStringName);
			else
			if (vShowVoidStructure)
				SetStatusLine(" Void Structure: " + TempStringName);
			else
			//if (vShowGlassStructure)
			//	SetStatusLine(" Glass Structure: " + TempStringName);
			//else
			if (vShowContaminant2Structure)
				SetStatusLine(" Contaminant2 Structure: " + TempStringName);
			else
			if (vShowAverage)
				SetStatusLine(" Average: " + TempStringName);
			else
			if (vShowHistogram)
				SetStatusLine(" Histogram: " + TempStringName);
			else
			//if (vShowGlassHistogram)
			//	SetStatusLine(" Glass Histogram: " + TempStringName);
			//else
				//SetStatusLine(" Image: " + TempStringName);
				SetStatusLine(TempStringName);
			//SetDlgItemText(IDC_Width2,"Showing");
			//m_Width2.ShowWindow(SW_SHOW);
			//SetDlgItemText(IDC_Reference,"Image");
			//m_Reference.ShowWindow(SW_SHOW);
			
			SetDlgItemText(IDC_SubFunction1Button, _T("Change Average Parameter"));
			SetDlgItemText(IDC_SubFunction2Button, _T("Change Structure Parameter"));
			SetDlgItemText(IDC_SubFunction3Button, _T("Change Histogram Parameter"));
			SetDlgItemText(IDC_SubFunction4Button, _T("Change Reference Parameter"));
		}
		else
		{
			m_SubFunction8Display.ShowWindow(SW_HIDE);
			m_SubFunction4Display.ShowWindow(SW_HIDE);
			m_SubFunction1Display.ShowWindow(SW_HIDE);
			m_SubFunction2Display.ShowWindow(SW_HIDE);
			m_SubFunction3Display.ShowWindow(SW_HIDE);
			m_SubFunction1Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_SubFunction1Button.GetRectToInvalidateArrow());
			m_SubFunction2Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_SubFunction2Button.GetRectToInvalidateArrow());
			m_SubFunction3Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_SubFunction3Button.GetRectToInvalidateArrow());
			m_SubFunction4Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_SubFunction4Button.GetRectToInvalidateArrow());
			//m_ArrowRight1.ShowWindow(SW_HIDE);
			m_SubFunction6Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_SubFunction6Button.GetRectToInvalidateArrow());
			m_Width2.ShowWindow(SW_HIDE);
			m_Reference.ShowWindow(SW_HIDE);
		}
		if ((PasswordOK(cAdvancedSetupPassword,false)) || (vLocalConfigurationData->vAutoImproveEnabled))
		{
			m_SubFunction7Button.ShowWindow(SW_SHOW);
			m_SubFunction7Display.ShowWindow(SW_SHOW);
			if (PasswordOK(cTemporaryInspxPassword,false))
			{
				if (vLocalConfigurationData->vNumberOfQuickLearnImagesToTake)
					SetDlgItemText(IDC_SubFunction7Display,"Quick:" +
						dtoa(vLocalConfigurationData->vNumberOfQuickLearnImagesToTake,0) + ", " +
						dtoa(vLocalConfigurationData->vQuickLearnNumberOfStructureDeltas,2));
				else
					SetDlgItemText(IDC_SubFunction7Display,  _T("Normal Learn"));
			}
			else
			{
				if (vLocalConfigurationData->vNumberOfQuickLearnImagesToTake)
					SetDlgItemText(IDC_SubFunction7Display, "Quick: " +
						dtoa(vLocalConfigurationData->vNumberOfQuickLearnImagesToTake,0));
				else
					SetDlgItemText(IDC_SubFunction7Display, "Normal: " +
						dtoa(vLocalConfigurationData->vAverageDisableCount + 
						vLocalConfigurationData->vStructureDisableCount + 
						vLocalConfigurationData->vCalibrateBodyTriggerReferenceDisableCount,0));
			}
		}
		else
		{
			m_SubFunction7Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_SubFunction7Button.GetRectToInvalidateArrow());
			m_SubFunction7Display.ShowWindow(SW_HIDE);

			//if (((!vLocalConfigurationData->vContinuousFeedContainerRate) && (!vLocalConfigurationData->vBulkProductMode)) || //Not Continuous Feed, Not a Pipeline
			//	(PasswordOK(cTemporaryInspxPassword,false)))//not a pipeline or inspx
			//{
			//	m_SubFunction1Button.ShowWindow(SW_SHOW);
			//	m_ArrowLeft1.ShowWindow(SW_SHOW);
			//	m_SubFunction1Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
			//	CString TempText;
			//	TempText.LoadString(IDS_Advanced);
			//	SetDlgItemText(IDC_SubFunction1Button,TempText);
			//}
			//else
			{
				m_SubFunction1Button.ShowWindow(SW_HIDE);
				this->InvalidateRect(&m_SubFunction1Button.GetRectToInvalidateArrow());
			}
		}
	}
	if (vMode == cSetImageSize) //DisplayWithPassword
	{
		if (PasswordOK(cTemporaryInspxPassword,false))
			SetDlgItemText(IDC_SubFunction8Display, dtoa(vGlobalCurrentProduct->vProductImageWidth,2) + " (" + dtoa(vGlobalCurrentProduct->vImageWidthPosition,0) + ")");
		else
			SetDlgItemText(IDC_SubFunction8Display, dtoa(vGlobalCurrentProduct->vProductImageWidth,2));

		if ((PasswordOK(cAdvancedSetupPassword,false)) || ((!vLocalConfigurationData->vContinuousFeedContainerRate) && (!vLocalConfigurationData->vBulkProductMode))) //Not Continuous Feed, Not a Pipeline or password in
		{
			CString TempText = "-";
			if ((PasswordOK(cTemporaryInspxPassword,false)) || ((!vLocalConfigurationData->vContinuousFeedContainerRate) && (!vLocalConfigurationData->vBulkProductMode))) //Not Continuous Feed, Not a Pipeline or password in
			{
				m_SubFunction1Button.ShowWindow(SW_SHOW);
				m_SubFunction1Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
				TempText.LoadString(IDS_SetupImageSettings);
				SetDlgItemText(IDC_SubFunction1Button,TempText);
			}
			else
			{
				m_SubFunction1Button.ShowWindow(SW_HIDE);
				this->InvalidateRect((&m_SubFunction1Button.GetRectToInvalidateArrow()));
			}

			if ((PasswordOK(cCertifiedPassword,false)) || ((!vLocalConfigurationData->vContinuousFeedContainerRate) && (!vLocalConfigurationData->vBulkProductMode) && (!vGlobalCurrentProduct->vBulkProductMode))) //Not Continuous Feed, Not a Pipeline or password in
			{
				if (!vGlobalCurrentProduct->vMultiLaneNumberOfLanes)
				{
					vSubFunction3ButtonEnable = true;		//m_SubFunction3Button.EnableWindow(true);
					m_SubFunction3Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
					TempText.LoadString(IDS_ContainerRetriggerLockout);
					SetDlgItemText(IDC_SubFunction3Button,TempText);
					m_SubFunction3Button.ShowWindow(SW_SHOW);
					m_SubFunction3Display.ShowWindow(SW_SHOW);

					if (PasswordOK(cTemporaryInspxPassword,false))
						SetDlgItemText(IDC_SubFunction3Display, dtoa(vGlobalCurrentProduct->vProductLockOutWidth,2) + " (" + dtoa(vGlobalCurrentProduct->vLockoutPosition, 0) + ")");
					else
						SetDlgItemText(IDC_SubFunction3Display, dtoa(vGlobalCurrentProduct->vProductLockOutWidth,2));

					m_SubFunction7Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
					vSubFunction7ButtonEnable = true;			//m_SubFunction7Button.EnableWindow(true);
					m_SubFunction7Button.ShowWindow(SW_SHOW);
					m_SubFunction7Display.ShowWindow(SW_SHOW);
					//Trigger To Detector Offset
					if (vLocalConfigurationData->vSystemBodyTriggerToXRayBeam == 0)
						TempText = "Trigger to X-Ray Distance";
					else 
						TempText.LoadString(IDS_ContainerTriggerToDetector);
			
					SetDlgItemText(IDC_SubFunction7Button,TempText);
				}
			}
			else
			{
				m_SubFunction3Button.ShowWindow(SW_HIDE); //hide re-trigger lockout button
				this->InvalidateRect(&m_SubFunction3Button.GetRectToInvalidateArrow());

				m_SubFunction7Button.ShowWindow(SW_HIDE); //hide re-trigger delay button
				this->InvalidateRect(&m_SubFunction7Button.GetRectToInvalidateArrow());
			}

			if (!vGlobalCurrentProduct->vMultiLaneNumberOfLanes) //not multi lane
				m_SubFunction5Display.ShowWindow(SW_SHOW);
			m_SubFunction5Button.ShowWindow(SW_SHOW);
			if ((vGlobalScanTracType == cBriosoScanTrac) && (vLocalSystemData->vITIPCDig->vOriginalBufferSizeY == vLocalSystemData->vITIPCDig->vOriginalBufferSizeX))
			{
				SetDlgItemText(IDC_SubFunction5Button, _T("Right"));
				SetDlgItemText(IDC_SubFunction6Button, _T("Left"));
				SetDlgItemText(IDC_SubFunction8Button, _T("Height"));

				if (PasswordOK(cSuperInspxPassword,false))
					vSubFunction8ButtonEnable = true;		//m_SubFunction8Button.EnableWindow(true);
				else
					vSubFunction8ButtonEnable = false;		//m_SubFunction8Button.EnableWindow(false);
			}
			else
			{
				if (vGlobalCurrentProduct->vMultiLaneNumberOfLanes)
					TempText.LoadString(IDS_AdjustLanePositions);
				else
					TempText.LoadString(IDS_Top);
				SetDlgItemText(IDC_SubFunction5Button,TempText);
				TempText.LoadString(IDS_Bottom);
				SetDlgItemText(IDC_SubFunction6Button,TempText);
				TempText.LoadString(IDS_Width);
				SetDlgItemText(IDC_SubFunction8Button,TempText);
			}

			//if (vGlobalCurrentProduct->vMultiLaneNumberOfLanes)
			//{
			//	m_SubFunction2Button.ShowWindow(SW_HIDE);
			//	m_SubFunction3Button.ShowWindow(SW_HIDE);
			//	m_SubFunction3Display.ShowWindow(SW_HIDE);
			//	m_SubFunction6Button.ShowWindow(SW_HIDE);
			//	m_SubFunction6Display.ShowWindow(SW_HIDE);
			//	m_SubFunction7Button.ShowWindow(SW_HIDE);
			//	m_SubFunction7Display.ShowWindow(SW_HIDE);
			//}
			//else
			if (!vGlobalCurrentProduct->vMultiLaneNumberOfLanes)
			{
				m_SubFunction6Display.ShowWindow(SW_SHOW);
				m_SubFunction6Button.ShowWindow(SW_SHOW);
			}

			if ((PasswordOK(cTemporaryInspxPassword,false)) && (vGlobalCurrentProduct->vProductImageHeightBottom))
				SetDlgItemText(IDC_SubFunction6Display, dtoa(vGlobalCurrentProduct->vProductImageHeightBottom,2) + " (" + dtoa(vGlobalCurrentProduct->vProductImageHeightBottom * vGlobalPixelsPerUnitInHeight,0) + ")");
			else
				SetDlgItemText(IDC_SubFunction6Display, dtoa(vGlobalCurrentProduct->vProductImageHeightBottom,2));

			m_SubFunction8Button.ShowWindow(SW_SHOW);
			m_SubFunction8Display.ShowWindow(SW_SHOW);

			if (PasswordOK(cTemporaryInspxPassword,false))
				SetDlgItemText(IDC_SubFunction8Display, dtoa(vGlobalCurrentProduct->vProductImageWidth,2) + " (" + dtoa(vGlobalCurrentProduct->vImageWidthPosition,0) + ")");
			else
				SetDlgItemText(IDC_SubFunction8Display, dtoa(vGlobalCurrentProduct->vProductImageWidth,2));

			double TempWhiteSpaceOnOneSideOfImage = (vGlobalCurrentProduct->vProductImageWidth / 1.6) * 0.3;
			if (TempWhiteSpaceOnOneSideOfImage > 4)
				TempWhiteSpaceOnOneSideOfImage = 4;

			double TempTriggerToBeamOffset = vGlobalCurrentProduct->vProductBodyTriggerToImageDistanceInInches;

			if (vLocalConfigurationData->vSystemBodyTriggerToXRayBeam > 1)
				TempTriggerToBeamOffset = -(vLocalConfigurationData->vSystemBodyTriggerToXRayBeam - vGlobalCurrentProduct->vProductBodyTriggerToImageDistanceInInches - TempWhiteSpaceOnOneSideOfImage);

			SetDlgItemText(IDC_SubFunction7Display, dtoa(TempTriggerToBeamOffset,2));
		}
		else
		{
			m_SubFunction1Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_SubFunction1Button.GetRectToInvalidateArrow());
			//m_SubFunction1Button.ShowWindow(SW_SHOW);
			//m_ArrowLeft1.ShowWindow(SW_SHOW);
			//m_SubFunction1Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
			//CString TempText;
			//TempText.LoadString(IDS_Advanced);
			//SetDlgItemText(IDC_SubFunction1Button,TempText);

			m_SubFunction3Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_SubFunction3Button.GetRectToInvalidateArrow());
			m_SubFunction3Display.ShowWindow(SW_HIDE);

			m_SubFunction5Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_SubFunction5Button.GetRectToInvalidateArrow());
			m_SubFunction5Display.ShowWindow(SW_HIDE);

			m_SubFunction6Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_SubFunction6Button.GetRectToInvalidateArrow());
			m_SubFunction6Display.ShowWindow(SW_HIDE);

			m_SubFunction7Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_SubFunction7Button.GetRectToInvalidateArrow());
			m_SubFunction7Display.ShowWindow(SW_HIDE);

			m_SubFunction8Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_SubFunction8Button.GetRectToInvalidateArrow());
			m_SubFunction8Display.ShowWindow(SW_HIDE);

			m_XScaleLabel.ShowWindow(SW_HIDE);
			m_XScale2.ShowWindow(SW_HIDE);
			m_YScaleLabel.ShowWindow(SW_HIDE);
			m_YScale2.ShowWindow(SW_HIDE);
			m_SizeX.ShowWindow(SW_HIDE);
			m_SizeLabelX.ShowWindow(SW_HIDE);
			m_SizeY.ShowWindow(SW_HIDE);
			m_SizeLabelY.ShowWindow(SW_HIDE);
		}
	}
	else
	if ((vMode == cSetVoidThreshold) || (vMode == cSetContaminantThreshold))
	{
		CString TempText("");
		if ((vInspectionEditing) && (vInspectionEditing->vFindBadIntensity))
		{
			TempText.LoadString(IDS_StopProcessingBad);
			m_SubFunction7Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL); //3 lines of text
			m_SubFunction7Button.ShowWindow(SW_SHOW);
		}
		else
		if (PasswordOK(cAdvancedSetupPassword,false))
		{
			if ((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode))
				TempText = "Send\nBad Product";
			else
				TempText.LoadString(IDS_SendBadContainersButton);
			m_SubFunction7Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
			m_SubFunction7Button.ShowWindow(SW_SHOW);
		}
		else
		{
			m_SubFunction7Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_SubFunction7Button.GetRectToInvalidateArrow());
		}
		SetDlgItemText(IDC_SubFunction7Button,TempText);

		if ((vInspectionEditing) && (((PasswordOK(cTemporaryInspxPassword,false)) && (vInspectionEditing->vHasAMinimumSize) && (vInspectionEditing->vInspectionType != cItemCountInspection) &&
			(vInspectionEditing->vInspectionType != cItemCountInspection) && (vInspectionEditing->vInspectionType != cSizeInspection) && (vInspectionEditing->vInspectionType != cMissingLidInspectionOnSize)) || 
			((PasswordOK(cSuperInspxPassword,false)) && (vInspectionEditing->vInspectionType != cItemCountInspection)) ||
			(vInspectionEditing->vInspectionType == cFloodedInspection)))
		{
			CString TempText;
			if ((PasswordOK(cSuperInspxPassword,false)) || (vInspectionEditing->vAllowChangingAdvancedOptions))
				TempText.LoadString(IDS_InspectionSettings);
			else
			if (vInspectionEditing->vInspectionType == cFloodedInspection)
				TempText = "Min. Spout Length";
			else
				TempText.LoadString(IDS_MinimumSize);
			SetDlgItemText(IDC_Function2Button,TempText);
			m_Function2Button.ShowWindow(SW_SHOW);
		}
		else
		{
			m_Function2Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_Function2Button.GetRectToInvalidateArrow());
		}
	}
}

void CAutoSetupImageDisplayDialog::ProcessTheImage()
{
	if (!vInspectionEditing)
	{
		vInspectionEditing = vGlobalCurrentProduct->vDriftDensityInspection;
		vMaximumThreshold = 255;
		if ((vInspectionEditing) && ((vInspectionEditing->vInspectionType == cUnderfillByWeightInspection) || (vInspectionEditing->vInspectionType == cOverfillByWeightInspection) || (vInspectionEditing->vInspectionType == cCheckWeighInspection)))
			vMaximumThreshold = vMaximumUnderfillByWeightThreshold;
	}

	if ((vLocalSystemData->vITIPCDig->vProcessManipulationImage) && (vLocalSystemData->vITIPCDig->vProcessManipulationImage->vGreyImage))
	{
		if (vLocalSystemData->vITIPCDig->vOriginalImage)
		{
			vLocalSystemData->vITIPCDig->vProcessManipulationImage->CopyImage(vLocalSystemData->vITIPCDig->vOriginalImage, vLocalSystemData->vITIPCDig->vOriginalBufferSize);

			int TempTopReferenceAdjust = 0;
			if (vMode != cSetSizeSettings)
			//if (vInspectionEditing)
			//{
			//	if ((vInspectionEditing->vReferenceToTop) || (vInspectionEditing->vReferenceToBottom))
			//		TempTopReferenceAdjust = vLocalSystemData->vITIPCDig->vProcessManipulationImage->vTopReferenceAdjust;

			//	if (vInspectionEditing->vReferenceToBottom)
			//		TempTopReferenceAdjust = vLocalSystemData->vITIPCDig->vProcessManipulationImage->vTopReferenceAdjust;
			//}

			if (vMode != cSetSizeSettings)
			if ((TempTopReferenceAdjust) || (vLocalSystemData->vITIPCDig->vProcessManipulationImage->vLeftReferenceAdjust))
					vLocalSystemData->vITIPCDig->MakeReferenceImageAdjustment(vLocalSystemData->vITIPCDig->vProcessManipulationImage->vGreyImage, vLocalSystemData->vITIPCDig->vASIDDScratchImage->pix,
						TempTopReferenceAdjust, vLocalSystemData->vITIPCDig->vProcessManipulationImage->vLeftReferenceAdjust + vLocalSystemData->vITIPCDig->vProcessManipulationImage->vFineReferenceAdjust);

			if (vMode == cSetSizeSettings)
			{
				CopyMemory(vLocalSystemData->vITIPCDig->vASIDDScratchImage->pix, vLocalSystemData->vITIPCDig->vProcessManipulationImage->vGreyImage, vLocalSystemData->vITIPCDig->vOriginalBufferSize);

				int TempError = mvt_threshold_down(vLocalSystemData->vITIPCDig->vASIDDScratchImage, vGlobalCurrentProduct->vThresholdForContainerBounds,0);

				CopyMemory(vLocalSystemData->vITIPCDig->vProcessManipulationImage->vGreyImage, vLocalSystemData->vITIPCDig->vASIDDScratchImage->pix, vLocalSystemData->vITIPCDig->vOriginalBufferSize);
			}
			else
			if (vInspectionEditing)
			{
				if ((!vLocalConfigurationData->vDemoMode) || (vInspectionEditing->vInspectionType == cKernelInspection))
				{
					if (vInspectionEditing->vFillBackground)
					{
						vInspectionEditing->FillBackground(vLocalSystemData->vITIPCDig->vASIDDScratchImage->pix, vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY);
					}

					if (vInspectionEditing->vFillInLightSpots)
					{
						vInspectionEditing->FillInLightSpots(
							vLocalSystemData->vITIPCDig->vProcessManipulationImage->vGreyImage, vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, 
							vLocalSystemData->vITIPCDig->vOriginalBufferSizeY, vInspectionEditing->vFillInLightSpots);
					}

					//if (vInspectionEditing->vFillInLightSpots)
					//{
					//	vInspectionEditing->CurveFitFillInLightSpots(TempWorkingImage, vLocalSystemData->vITIPCDig->vDisplayBufferHeightInPixels, vLocalSystemData->vITIPCDig->vDisplayBufferWidthInPixels);
					//}

					if (vInspectionEditing->vPreSmoothImage)
					{
						CopyMemory(vLocalSystemData->vITIPCDig->vASIDDScratchImage3->pix, vLocalSystemData->vITIPCDig->vProcessManipulationImage->vGreyImage, vLocalSystemData->vITIPCDig->vOriginalBufferSize);

						DoMedianSmooth(vLocalSystemData->vITIPCDig->vASIDDScratchImage3, vInspectionEditing->vPreSmoothImage,vInspectionEditing->vPreSmoothImage);

						CopyMemory(vLocalSystemData->vITIPCDig->vProcessManipulationImage->vGreyImage, vLocalSystemData->vITIPCDig->vASIDDScratchImage3->pix, 
							vLocalSystemData->vITIPCDig->vOriginalBufferSize);
					}

					//erode image if should
					if (vInspectionEditing->vErodeCount)
					{
						CopyMemory(vLocalSystemData->vITIPCDig->vASIDDScratchImage->pix, vLocalSystemData->vITIPCDig->vProcessManipulationImage->vGreyImage, vLocalSystemData->vITIPCDig->vOriginalBufferSize);

						for (BYTE TempLoop = 0; TempLoop < vInspectionEditing->vErodeCount; TempLoop++)
							vInspectionEditing->ErodeImage(vLocalSystemData->vITIPCDig->vASIDDScratchImage, vLocalSystemData->vITIPCDig->vASIDDScratchImage2);

						CopyMemory(vLocalSystemData->vITIPCDig->vProcessManipulationImage->vGreyImage, vLocalSystemData->vITIPCDig->vASIDDScratchImage->pix, vLocalSystemData->vITIPCDig->vOriginalBufferSize);
					}

					if (vInspectionEditing->vDilationCount)
					{
						CopyMemory(vLocalSystemData->vITIPCDig->vASIDDScratchImage->pix, vLocalSystemData->vITIPCDig->vProcessManipulationImage->vGreyImage, vLocalSystemData->vITIPCDig->vOriginalBufferSize);

						for (BYTE TempLoop = 0; TempLoop < vInspectionEditing->vDilationCount; TempLoop++)
							vInspectionEditing->DilateImage(vLocalSystemData->vITIPCDig->vASIDDScratchImage, vLocalSystemData->vITIPCDig->vASIDDScratchImage2);

						if (vInspectionEditing->vErodeCountAtStart)  //is variable to hold erode times count
						if ((vLocalSystemData->vITIPCDig->vASIDDScratchImage2) && (vLocalSystemData->vITIPCDig->vASIDDScratchImage2->pix))
						for (BYTE TempLoop = 0; TempLoop < vInspectionEditing->vErodeCountAtStart; TempLoop++)
						{
							vInspectionEditing->ErodeImage(vLocalSystemData->vITIPCDig->vASIDDScratchImage, vLocalSystemData->vITIPCDig->vASIDDScratchImage2);
						}
						CopyMemory(vLocalSystemData->vITIPCDig->vProcessManipulationImage->vGreyImage, vLocalSystemData->vITIPCDig->vASIDDScratchImage->pix, vLocalSystemData->vITIPCDig->vOriginalBufferSize);
					}

					if (vInspectionEditing->vFindSpotInSubROI)
					{
						if (vInspectionEditing->vDesensitizeIrregularArea)
						{
							CopyMemory(vLocalSystemData->vITIPCDig->vASIDDScratchImage->pix, vLocalSystemData->vITIPCDig->vProcessManipulationImage->vGreyImage, vLocalSystemData->vITIPCDig->vOriginalBufferSize);
						}
						vInspectionEditing->MakeVariablityImage(vLocalSystemData->vITIPCDig->vASIDDScratchImage, vLocalSystemData->vITIPCDig->vASIDDScratchImage2, vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY, 4);
					}

					if (vInspectionEditing->vRemoveJarPunt)
						vInspectionEditing->RemoveJarPunt(vLocalSystemData->vITIPCDig->vProcessManipulationImage->vGreyImage,
							vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY, 
							vLocalSystemData->vITIPCDig->vProcessManipulationImage->vLeft - vLocalSystemData->vITIPCDig->vProcessManipulationImage->vLeftReferenceAdjust, 
							vLocalSystemData->vITIPCDig->vProcessManipulationImage->vRight - vLocalSystemData->vITIPCDig->vProcessManipulationImage->vLeftReferenceAdjust, 
							vGlobalCurrentProduct->vSetupHistoryAverageDensity, 0);

					if (vInspectionEditing->vTrimEdgesOfJar)
					if (!vInspectionEditing->vDoContaminant22Algorithm)
					{
						vInspectionEditing->TrimEdgesOfJarAtROI(vLocalSystemData->vITIPCDig->vProcessManipulationImage->vGreyImage,
							vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY, true,
							vLocalSystemData->vITIPCDig->vProcessManipulationImage->vLeft - vLocalSystemData->vITIPCDig->vProcessManipulationImage->vLeftReferenceAdjust, 
							vLocalSystemData->vITIPCDig->vProcessManipulationImage->vRight - vLocalSystemData->vITIPCDig->vProcessManipulationImage->vLeftReferenceAdjust);
					}



					if (vInspectionEditing->vTakeDerivative)
						vInspectionEditing->TakeLineDerivative(vLocalSystemData->vITIPCDig->vProcessManipulationImage->vGreyImage, vLocalSystemData->vITIPCDig->vASIDDScratchImage->pix, 
							vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY, vGlobalCurrentProduct->vSetupHistoryAverageDensity, vInspectionEditing->vThreshold);

					if (vInspectionEditing->vBonesInspection)
					{
						WORD TempBottom = 0;
						WORD TempLeft = 0;
						WORD TempHeight = 0;
						WORD TempWidth = 0;
						BYTE TempAutoImproveState = 0; //not a learned inspection, so don't bring in ROI edges

						vLocalSystemData->vITIPCDig->GetInspectionROIBounds(vInspectionEditing, &TempBottom, &TempLeft, &TempHeight, &TempWidth, 
							vLocalSystemData->vITIPCDig->vOriginalImage->vLeftReferenceAdjust, vLocalConfigurationData->vReferenceROIsToEdges, TempAutoImproveState);

						vInspectionEditing->FindBonesInspection(vLocalSystemData->vITIPCDig->vProcessManipulationImage->vGreyImage, vLocalSystemData->vITIPCDig->vASIDDScratchImage, 
							vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY, 
							TempLeft, TempLeft + TempWidth, TempBottom, TempBottom + TempHeight);
					}

#ifdef UseIFCDLLs
					if (vInspectionEditing->vTakeDifferenceFromMedianFilter)
					{
						CopyMemory(vLocalSystemData->vITIPCDig->vASIDDScratchImage->pix, vLocalSystemData->vITIPCDig->vProcessManipulationImage->vGreyImage, vLocalSystemData->vITIPCDig->vOriginalBufferSize);
						CopyMemory(vLocalSystemData->vITIPCDig->vASIDDScratchImage2->pix, vLocalSystemData->vITIPCDig->vProcessManipulationImage->vGreyImage, vLocalSystemData->vITIPCDig->vOriginalBufferSize);

						BYTE TempMedian = vInspectionEditing->vTakeDifferenceFromMedianFilter & 0x3F;
						DoMedianSmooth(vLocalSystemData->vITIPCDig->vASIDDScratchImage, TempMedian, TempMedian);

						if (vInspectionEditing->vTakeDifferenceFromMedianFilter < 128)
						{
							mvt_subtract(vLocalSystemData->vITIPCDig->vASIDDScratchImage, vLocalSystemData->vITIPCDig->vASIDDScratchImage2);
							mvt_invert(vLocalSystemData->vITIPCDig->vASIDDScratchImage2);
							CopyMemory(vLocalSystemData->vITIPCDig->vProcessManipulationImage->vGreyImage, vLocalSystemData->vITIPCDig->vASIDDScratchImage2->pix, vLocalSystemData->vITIPCDig->vOriginalBufferSize);
						}
						else
							CopyMemory(vLocalSystemData->vITIPCDig->vProcessManipulationImage->vGreyImage, vLocalSystemData->vITIPCDig->vASIDDScratchImage->pix, vLocalSystemData->vITIPCDig->vOriginalBufferSize);
					}

					if (vInspectionEditing->vTakeDifferenceFromMedian)
					{
						vInspectionEditing->TakeDifferenceFromMedian(vLocalSystemData->vITIPCDig->vProcessManipulationImage->vGreyImage, vLocalSystemData->vITIPCDig->vASIDDScratchImage,
							vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY, vLocalSystemData->vITIPCDig->vOriginalImage->vGreyImage, vInspectionEditing->vThreshold);
					}

					if (vInspectionEditing->vFindDarkSpotOnLine)
						vInspectionEditing->FindDarkSpotOnLine(vLocalSystemData->vITIPCDig->vProcessManipulationImage->vGreyImage, vLocalSystemData->vITIPCDig->vASIDDScratchImage->pix, 
							vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY, vInspectionEditing->vThreshold);

					if (vInspectionEditing->vFindSpotInSubROI)
					{
						WORD TempBottom = 0;
						WORD TempLeft = 0;
						WORD TempHeight = 0;
						WORD TempWidth = 0;
						BYTE TempAutoImproveState = 0; //not a learned inspection, so don't bring in ROI edges

						vLocalSystemData->vITIPCDig->GetInspectionROIBounds(vInspectionEditing, &TempBottom, &TempLeft, &TempHeight, &TempWidth, 
							vLocalSystemData->vITIPCDig->vOriginalImage->vLeftReferenceAdjust, vLocalConfigurationData->vReferenceROIsToEdges, TempAutoImproveState);

						vInspectionEditing->FindSpotInSubROI(vLocalSystemData->vITIPCDig->vProcessManipulationImage->vGreyImage, vLocalSystemData->vITIPCDig->vASIDDScratchImage,
							vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY, 
							vInspectionEditing->vThreshold, vLocalSystemData->vITIPCDig->vDesensitizeImage, false, TempBottom, TempBottom + TempHeight, TempLeft, TempLeft + TempWidth);
					}

					if (vInspectionEditing->vPerformGlassAlgorithm == 1)
					{
						vInspectionEditing->ClipToCurveFit(vLocalSystemData->vITIPCDig->vProcessManipulationImage->vGreyImage,
							vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY);
					}

					if (vInspectionEditing->vPerformGlassAlgorithm == 2)
					{
						vInspectionEditing->CalculateLineDerivative(vLocalSystemData->vITIPCDig->vProcessManipulationImage->vGreyImage, vLocalSystemData->vITIPCDig->vASIDDScratchImage->pix,
							vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY, vGlobalCurrentProduct->vSetupHistoryAverageDensity);
					}

					if (vInspectionEditing->vLookForDarkSpots)
					{
						WORD TempBottom = 0;
						WORD TempLeft = 0;
						WORD TempHeight = 0;
						WORD TempWidth = 0;
						BYTE TempAutoImproveState = 0; //not a learned inspection, so don't bring in ROI edges

						vLocalSystemData->vITIPCDig->GetInspectionROIBounds(vInspectionEditing, &TempBottom, &TempLeft, &TempHeight, &TempWidth, 
							vLocalSystemData->vITIPCDig->vOriginalImage->vLeftReferenceAdjust, vLocalConfigurationData->vReferenceROIsToEdges, TempAutoImproveState);

						WORD TempTop = TempBottom + TempHeight;
						WORD TempRight = TempLeft + TempWidth;

						vInspectionEditing->LookForDarkSpotsOnLine(vLocalSystemData->vITIPCDig->vProcessManipulationImage->vGreyImage, vLocalSystemData->vITIPCDig->vASIDDScratchImage,
							vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY, TempLeft, TempRight, TempBottom, TempTop, vInspectionEditing->vThreshold);
					}

					if (vInspectionEditing->vDoContaminant22Algorithm)
					{
						BYTE TempThreshold = vInspectionEditing->vThreshold;	
						//BYTE TempThreshold = vThreshold;
						int TempResult22 = vInspectionEditing->InspxContaminant22InspectionB(vLocalSystemData->vITIPCDig->vProcessManipulationImage->vGreyImage,
							vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY, TempThreshold, 1);
					}

					if ((vLocalSystemData->vITIPCDig->vProductAverageImage) && (vInspectionEditing->vSubtractAverage))
					{
						if ((vInspectionEditing->vInspectionType == cContaminantInspection) ||
							(vInspectionEditing->vInspectionType == cWireContaminantInspection) ||
							(vInspectionEditing->vInspectionType == cSpotInROIContaminantInspection) ||
							(vInspectionEditing->vInspectionType == cGlass13mmContaminantInspection) ||
							(vInspectionEditing->vInspectionType == cTopContaminantInspection) ||
							(vInspectionEditing->vInspectionType == cBottomContaminantInspection) ||
							(vInspectionEditing->vInspectionType == cJarBottomContaminantInspection) ||
							(vInspectionEditing->vInspectionType == cKernelInspection) ||
							(vInspectionEditing->vInspectionType == cContaminant2Inspection) ||
							(vInspectionEditing->vInspectionType == cContaminantBonesInspection) ||
							(vInspectionEditing->vInspectionType == cIPContaminantInspection) ||
							(vInspectionEditing->vInspectionType == cContaminant22Inspection) ||
							(vInspectionEditing->vInspectionType == cAdvancedContaminant2Inspection))
							vLocalSystemData->vITIPCDig->SubtractProductAverageImage(vLocalSystemData->vITIPCDig->vProcessManipulationImage->vGreyImage, vLocalSystemData->vITIPCDig->vProductAverageImage);
						else
						if (vInspectionEditing->vInspectionType == cVoidInspection)
							vLocalSystemData->vITIPCDig->SubtractVoidProductAverageImage(vLocalSystemData->vITIPCDig->vProcessManipulationImage->vGreyImage);
					}

					if ((vLocalSystemData->vITIPCDig->vProductStructureImage) && (vInspectionEditing->vAddStructure) && (vInspectionEditing->vSubtractAverage))
					{
						if ((vInspectionEditing->vInspectionType != cVoidInspection) && (vLocalSystemData->vITIPCDig->vProductStructureImage))
							vLocalSystemData->vITIPCDig->AddProductStructureImage(vLocalSystemData->vITIPCDig->vProcessManipulationImage->vGreyImage);
						else
						if (vInspectionEditing->vInspectionType == cVoidInspection)
							vLocalSystemData->vITIPCDig->AddVoidProductStructureImage(vLocalSystemData->vITIPCDig->vProcessManipulationImage->vGreyImage);
					}
					else
					if ((vLocalSystemData->vITIPCDig->vProductContaminant2StructureImage) && (vInspectionEditing->vAddStructure) && (!vInspectionEditing->vSubtractAverage) && (vInspectionEditing->vInspectionType != cVoidInspection))
					{
						vLocalSystemData->vITIPCDig->AddProductContaminant2StructureImage(vLocalSystemData->vITIPCDig->vProcessManipulationImage->vGreyImage);
					}
#endif
				}

				if (vInspectionEditing->vIPContaminant)
				if (vLocalSystemData->vITIPCDig->vASIDDScratchImage3)
				if (vLocalSystemData->vITIPCDig->vASIDDScratchImage3->pix)
				{
					CopyMemory(vLocalSystemData->vITIPCDig->vASIDDScratchImage3->pix, vLocalSystemData->vITIPCDig->vProcessManipulationImage->vGreyImage, vLocalSystemData->vITIPCDig->vOriginalBufferSize);
					ThresholdImage(vLocalSystemData->vITIPCDig->vASIDDScratchImage3, vInspectionEditing->vIPContaminant); //create a mask of what is below the threshold
				}

				BYTE TempEnhanceMethod = 0;
				TempEnhanceMethod = vInspectionEditing->vEnhanceMethod;
				if (vLocalConfigurationData->vDemoMode)
					TempEnhanceMethod = 0;
				if (TempEnhanceMethod)
				{
					CopyMemory(vLocalSystemData->vITIPCDig->vASIDDScratchImage->pix, vLocalSystemData->vITIPCDig->vProcessManipulationImage->vGreyImage, vLocalSystemData->vITIPCDig->vOriginalBufferSize);

					vLocalSystemData->vITIPCDig->PerformEnhanceOnImage(vLocalSystemData->vITIPCDig->vASIDDScratchImage, vLocalSystemData->vITIPCDig->vASIDDScratchImage2, TempEnhanceMethod);

					CopyMemory( vLocalSystemData->vITIPCDig->vProcessManipulationImage->vGreyImage, vLocalSystemData->vITIPCDig->vASIDDScratchImage->pix, vLocalSystemData->vITIPCDig->vOriginalBufferSize);
				}

				if ((vLocalSystemData->vITIPCDig->vProductHistogramImage) && (vInspectionEditing->vAddHistogram))
					vLocalSystemData->vITIPCDig->AddProductHistogramImage(vLocalSystemData->vITIPCDig->vProcessManipulationImage->vGreyImage);

				if (vInspectionEditing)
				if (vInspectionEditing->vIPContaminant)
				if (vLocalSystemData->vITIPCDig->vASIDDScratchImage3)
				if (vLocalSystemData->vITIPCDig->vASIDDScratchImage3->pix)
				{
					CopyMemory(vLocalSystemData->vITIPCDig->vASIDDScratchImage2->pix, vLocalSystemData->vITIPCDig->vProcessManipulationImage->vGreyImage, vLocalSystemData->vITIPCDig->vOriginalBufferSize);
					ThresholdImage(vLocalSystemData->vITIPCDig->vASIDDScratchImage2, vInspectionEditing->vThreshold); //create a mask of what is below the threshold
					CopyMemory(vLocalSystemData->vITIPCDig->vProcessManipulationImage->vGreyImage, vLocalSystemData->vITIPCDig->vASIDDScratchImage2->pix, vLocalSystemData->vITIPCDig->vOriginalBufferSize);

					vLocalSystemData->vITIPCDig->AndImages(vLocalSystemData->vITIPCDig->vASIDDScratchImage3->pix, vLocalSystemData->vITIPCDig->vProcessManipulationImage->vGreyImage, vLocalSystemData->vITIPCDig->vOriginalBufferSize);
				}

				if (vInspectionEditing)
				if ((vLocalConfigurationData->vDemoMode) && (vInspectionEditing->vInspectionType != cKernelInspection))
				{
					if (vMode == cSetVoidThreshold)
					{
						BYTE *TempOriginalPointer = 
							vLocalSystemData->vITIPCDig->vProcessManipulationImage->vGreyImage;
						for (DWORD TempLoop = 0; TempLoop < vLocalSystemData->vITIPCDig->vOriginalBufferSize; TempLoop++)
						{
							if (*TempOriginalPointer == 255)
								*TempOriginalPointer = 0;//if original data was zero, set to one (not red)
							TempOriginalPointer++;
						}
						vLocalSystemData->vITIPCDig->ThresholdBufferDown(
						vLocalSystemData->vITIPCDig->vProcessManipulationImage->vGreyImage, 
						vLocalSystemData->vITIPCDig->vOriginalBufferSize,
						vInspectionEditing->vThreshold);
					}
					else
						vLocalSystemData->vITIPCDig->ThresholdBufferDown(
						vLocalSystemData->vITIPCDig->vProcessManipulationImage->vGreyImage, 
						vLocalSystemData->vITIPCDig->vOriginalBufferSize,
						vInspectionEditing->vThreshold);
				}
				else
				{ //kernel, or not demo
					{
						{
							CopyMemory(vLocalSystemData->vITIPCDig->vASIDDScratchImage->pix, vLocalSystemData->vITIPCDig->vProcessManipulationImage->vGreyImage, vLocalSystemData->vITIPCDig->vOriginalBufferSize);
						
							CopyMemory(vLocalSystemData->vITIPCDig->vProcessManipulationImage->vGreyImage, vLocalSystemData->vITIPCDig->vOriginalImage->vGreyImage, vLocalSystemData->vITIPCDig->vOriginalBufferSize);

							BYTE *TempOriginalPointer = vLocalSystemData->vITIPCDig->vProcessManipulationImage->vGreyImage;
							for (DWORD TempLoop = 0; TempLoop < vLocalSystemData->vITIPCDig->vOriginalBufferSize; TempLoop++)
							{
								if (*TempOriginalPointer < 2)
									*TempOriginalPointer = 2;//if original data was zero, set to one (not red)
								TempOriginalPointer++;
							}

							{
								if ((vInspectionEditing->vInspectionType == cVoidInspection) ||
									(vInspectionEditing->vInspectionType == cGlass13mmContaminantInspection) ||
									(vInspectionEditing->vInspectionType == cThresholdContaminantInspection) ||
									(vInspectionEditing->vInspectionType == cBadHeartInspection) ||
									(vInspectionEditing->vInspectionType == cDripBySpoutInspection) ||
									(vInspectionEditing->vInspectionType == cDripInspection) ||
									(vInspectionEditing->vInspectionType == cTopContaminantInspection) ||
									(vInspectionEditing->vInspectionType == cBottomContaminantInspection) ||
									(vInspectionEditing->vInspectionType == cJarBottomContaminantInspection) ||
									(vInspectionEditing->vInspectionType == cItemCountInspection) || (vInspectionEditing->vInspectionType == cSizeInspection) || 
									(vInspectionEditing->vInspectionType == cMissingLidInspectionOnSize) ||
									(vInspectionEditing->vInspectionType == cSpotInROIContaminantInspection) ||
									(vInspectionEditing->vInspectionType == cContaminantInspection) ||
									(vInspectionEditing->vInspectionType == cIPContaminantInspection) ||
									(vInspectionEditing->vInspectionType == cWireContaminantInspection) ||
									(vInspectionEditing->vInspectionType == cContaminant2Inspection) ||
									(vInspectionEditing->vInspectionType == cContaminantBonesInspection) ||
									(vInspectionEditing->vInspectionType == cAdvancedContaminant2Inspection))
								{
									BYTE TempThreshold = vInspectionEditing->vThreshold;
									if (TempThreshold)
										int TempError = mvt_threshold_down(vLocalSystemData->vITIPCDig->vASIDDScratchImage,TempThreshold,0);
									else
										FillMemory(vLocalSystemData->vITIPCDig->vASIDDScratchImage->pix,vLocalSystemData->vITIPCDig->vOriginalBufferSize,255);

									if ((vInspectionEditing->vInspectionType == cItemCountInspection) && (vInspectionEditing->vMinimumBrightnessAceptable > 0) && (vInspectionEditing->vMinimumBrightnessAceptable < 255))
									{
										BYTE *TempMaximumPointer = vLocalSystemData->vITIPCDig->vProcessManipulationImage->vGreyImage + vLocalSystemData->vITIPCDig->vOriginalBufferSize;
										BYTE *TempMaximumRedPointer = vLocalSystemData->vITIPCDig->vASIDDScratchImage->pix + vLocalSystemData->vITIPCDig->vOriginalBufferSize;

										int TempOffset = vLocalSystemData->vITIPCDig->vProcessManipulationImage->vLeftReferenceAdjust;
										BYTE *TempRedImagePointer = vLocalSystemData->vITIPCDig->vASIDDScratchImage->pix;
										BYTE *TempOriginalPointer = vLocalSystemData->vITIPCDig->vProcessManipulationImage->vGreyImage;
										if (TempOffset > 0)
											TempOriginalPointer = vLocalSystemData->vITIPCDig->vProcessManipulationImage->vGreyImage + (TempOffset * vLocalSystemData->vITIPCDig->vOriginalBufferSizeX) + TempTopReferenceAdjust;
										else
										if (TempOffset < 0)
											TempRedImagePointer = vLocalSystemData->vITIPCDig->vASIDDScratchImage->pix - (TempOffset * vLocalSystemData->vITIPCDig->vOriginalBufferSizeX) + TempTopReferenceAdjust;

										for (DWORD TempLoop = 0; TempLoop < vLocalSystemData->vITIPCDig->vOriginalBufferSize; TempLoop++)
										{
											if ((TempOriginalPointer < TempMaximumPointer) && (TempRedImagePointer < TempMaximumRedPointer) && (TempRedImagePointer >= vLocalSystemData->vITIPCDig->vASIDDScratchImage->pix))
											if (*TempOriginalPointer < vInspectionEditing->vMinimumBrightnessAceptable)
												*TempRedImagePointer = 255;	//if original data was below minimum Threshold, set to 255 (not red)
											TempOriginalPointer++;
											TempRedImagePointer++;
										}
									}
								}
								else
								if (vInspectionEditing->vInspectionType == cKernelInspection)
								{
									
									Image *TempKernelImage;
									//create an image to store the amount each pixel is different from the baseline
									TempKernelImage = im_create(IM_BYTE, 
										vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, 
										vLocalSystemData->vITIPCDig->vAquisitionSizeY);
									
									if ((TempKernelImage) && (TempKernelImage->pix))
									{
										//store the amount each pixel is different from the baseline in the pixel in the new image
										BYTE *TempDesitinationPointer = TempKernelImage->pix;
										BYTE *TempSourcePointer = vLocalSystemData->vITIPCDig->vASIDDScratchImage->pix;
										for (DWORD TempLoop = 0;TempLoop < vLocalSystemData->vITIPCDig->vOriginalBufferSize; 
											TempLoop++)
										{
											*TempDesitinationPointer++ = *TempSourcePointer++;// + TempMax;
										}
			//temp image is processed image
										//find out how many kernels high and wide will have to analyze
										WORD TempNumberOfKernelsHigh = (WORD)((((double)vInspectionEditing->vROIPixelTop - (double)vInspectionEditing->vROIPixelBottom) / (double)vInspectionEditing->vSize * 2.0) + .99);
										WORD TempNumberOfKernelsWide = (WORD)((((double)vInspectionEditing->vROIPixelRight - (double)vInspectionEditing->vROIPixelLeft) / (double)vInspectionEditing->vSize * 2.0) + .99);

										//analyze each kernel high and wide
										for (WORD TempKernelHighLoop = 0; TempKernelHighLoop < TempNumberOfKernelsHigh;
											TempKernelHighLoop++)
										for (WORD TempKernelWideLoop = 0; TempKernelWideLoop < TempNumberOfKernelsWide;
											TempKernelWideLoop++)
										{
											DWORD TempKernelSum = 0;
											//find the starting place for this kernel analysis
											WORD TempKernelWideStart = (WORD)(vInspectionEditing->vROIPixelLeft + (TempKernelWideLoop * vInspectionEditing->vSize / 2));
											if (TempKernelWideStart + vInspectionEditing->vSize > vInspectionEditing->vROIPixelRight)
												TempKernelWideStart = (WORD)(vInspectionEditing->vROIPixelRight - vInspectionEditing->vSize);

											WORD TempKernelHighStart = (WORD)(vInspectionEditing->vROIPixelBottom + (TempKernelHighLoop * vInspectionEditing->vSize / 2));
											if (TempKernelHighStart + vInspectionEditing->vSize > vInspectionEditing->vROIPixelTop)
												TempKernelHighStart = (WORD)(vInspectionEditing->vROIPixelTop - vInspectionEditing->vSize);
											
											//sum up all the pixels in this kernel
											WORD TempSumCount = 0;
											for (WORD TempSumWideLoop = 0; TempSumWideLoop < vInspectionEditing->vSize;
												TempSumWideLoop++)
											{
												BYTE *TempDataPointer = TempKernelImage->pix + 
													TempKernelHighStart + 
													((TempKernelWideStart + TempSumWideLoop) * 
													vLocalSystemData->vITIPCDig->vOriginalBufferSizeX);
												for (WORD TempSumHighLoop = 0; TempSumHighLoop < vInspectionEditing->vSize;
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
											if (TempKernelAverage < vInspectionEditing->vThreshold)
											{
												//put any pixels not in the baseline in the red image buffer since going to reject
												{
													BYTE *TempDestinationPointer;
													//look at all the pixels in this kernel, any that are not at baseline set red in red image
													for (WORD TempSumWideLoop = 0; TempSumWideLoop < vInspectionEditing->vSize;
														TempSumWideLoop++)
													{
														BYTE *TempDataPointer = TempKernelImage->pix + 
															TempKernelHighStart + 
															((TempKernelWideStart +	TempSumWideLoop) * 
															vLocalSystemData->vITIPCDig->vOriginalBufferSizeX);
														TempDestinationPointer = 
															vLocalSystemData->vITIPCDig->vASIDDScratchImage->pix +
															TempKernelHighStart + 
															((TempKernelWideStart + TempSumWideLoop) * 
															vLocalSystemData->vITIPCDig->vOriginalBufferSizeX);
														for (WORD TempSumHighLoop = 0; TempSumHighLoop < vInspectionEditing->vSize;
															TempSumHighLoop++)
														{
			//vLocalSystemData->vITIPCDig->vASIDDScratchImage->pix has red spots in it
															if (*TempDataPointer < vInspectionEditing->vThreshold)
																*TempDestinationPointer = 0; //red spot
															*TempDataPointer++;
															*TempDestinationPointer++;
														}
													}
												}
											}
										}
									}
									if (TempKernelImage)
										im_delete(TempKernelImage);
								}
							}
#ifdef UseIFCDLLs
							Erflag TempErrorFlag;
							MVT_BLOB_PARAMS *TempBlobParameters = mvt_blob_create_params(&TempErrorFlag);
							MVT_BLOB_RESULTS *TempBlobResults = mvt_blob_create_results(20, true, &TempErrorFlag);
							// blue
							if ((TempBlobParameters) && (TempBlobResults))
							{
								if (vInspectionEditing->vSize > 1)
									TempBlobParameters->min_area = 1;//show all blobs of any size. vInspectionShowing->vSize;
								else
									TempBlobParameters->min_area = -1;

								TempBlobParameters->connected = CONNECT8;
								TempBlobParameters->blob_color = 0x00;  //fewer xrays
								TempBlobParameters->min_height = -1;
								TempBlobParameters->max_area = -1;
								
								if ((vInspectionEditing->vInspectionType == cDripInspection) || (vInspectionEditing->vInspectionType == cDripBySpoutInspection))
								{
									if (vInspectionEditing->vMaximumSize) //for Drip Inspection
										TempBlobParameters->max_area = vInspectionEditing->vMaximumSize;

									if (vInspectionEditing->vMinimumWidth)
										TempBlobParameters->min_height = vInspectionEditing->vMinimumWidth;
								}

								if (vInspectionEditing->vInspectionType == cBadHeartInspection)
									TempBlobParameters->calc_results = AREA_BBOX + AREA_BBOX_CENT_PERIM;
								else
									TempBlobParameters->calc_results = AREA_BBOX;

								if (!vInspectionEditing->vDoContaminant22Algorithm)
									ThresholdImage(vLocalSystemData->vITIPCDig->vASIDDScratchImage, vInspectionEditing->vThreshold);

								Erflag TempResult = mvt_blob_find(vLocalSystemData->vITIPCDig->vASIDDScratchImage, TempBlobParameters, TempBlobResults, MVT_CF_DEFAULT);

								TempOriginalPointer = vLocalSystemData->vITIPCDig->vProcessManipulationImage->vGreyImage;

								BYTE *TempRedImagePointer = vLocalSystemData->vITIPCDig->vASIDDScratchImage->pix;

								BYTE *TempMaximumPointer = vLocalSystemData->vITIPCDig->vProcessManipulationImage->vGreyImage + vLocalSystemData->vITIPCDig->vOriginalBufferSize;
								BYTE *TempMaximumRedPointer = vLocalSystemData->vITIPCDig->vASIDDScratchImage->pix + vLocalSystemData->vITIPCDig->vOriginalBufferSize;

								int TempOffset = vLocalSystemData->vITIPCDig->vProcessManipulationImage->vLeftReferenceAdjust;
								
								if (TempOffset > 0)
									TempOriginalPointer = vLocalSystemData->vITIPCDig->vProcessManipulationImage->vGreyImage + (TempOffset * vLocalSystemData->vITIPCDig->vOriginalBufferSizeX) + TempTopReferenceAdjust;
								else
								if (TempOffset < 0)
									TempRedImagePointer = vLocalSystemData->vITIPCDig->vASIDDScratchImage->pix - (TempOffset * vLocalSystemData->vITIPCDig->vOriginalBufferSizeX) + TempTopReferenceAdjust;

								for (DWORD TempLoop = 0; TempLoop < vLocalSystemData->vITIPCDig->vOriginalBufferSize; TempLoop++)
								{
									if ((TempOriginalPointer < TempMaximumPointer) && (TempRedImagePointer < TempMaximumRedPointer) && (TempRedImagePointer >= vLocalSystemData->vITIPCDig->vASIDDScratchImage->pix))
									{
										if (*TempRedImagePointer == 0)  //if a defect, set original data red
											*TempOriginalPointer = 1; 
									}

									TempOriginalPointer++;
									TempRedImagePointer++;
								}

								//now find the blobs that are bigger than minimum size and make them red
								//showing the AS Displayed Image processing on the screen if changed any settings.
								if ((WORD)TempBlobResults->num_blobs_found)
								{
									for (WORD TempLoop = 0; TempLoop < (WORD)TempBlobResults->num_blobs_found; TempLoop++)
									{
										WORD TempBlobArea = (WORD)mvt_blob_get_area(TempBlobResults, TempLoop);
										if (TempBlobArea >= vInspectionEditing->vSize)
										{
											MVT_BLOB *TempBlob = TempBlobResults->blobs[TempLoop];

											int TempStart = TempBlob->ymin + TempOffset;
											if (TempStart < 0)
												TempStart = 0;
											int TempStop = TempBlob->ymax + TempOffset;
											if (TempStop >= vLocalSystemData->vITIPCDig->vOriginalBufferSizeY)
												TempStop = vLocalSystemData->vITIPCDig->vOriginalBufferSizeY - 1;

											for (int TempLoopY = TempStart; TempLoopY <= TempStop; TempLoopY++)
											{
												BYTE *TempPointer = vLocalSystemData->vITIPCDig->vProcessManipulationImage->vGreyImage + (TempLoopY * vLocalSystemData->vITIPCDig->vOriginalBufferSizeX) + TempBlob->xmin;

												for (DWORD TempLoopX = TempBlob->xmin; TempLoopX <= TempBlob->xmax; TempLoopX++)
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
							}
#endif
						}
					}
				}
			}
		}
	}
}

void CAutoSetupImageDisplayDialog::OnDestroy() 
{

	DeleteObject(vLightGrayBrush);
	CDialog::OnDestroy();
	

	// TODO: Add your message handler code here
	
}


void CAutoSetupImageDisplayDialog::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	WORD TempOffsetX = (WORD) ResizeXCoor(112);
	WORD TempOffsetY = (WORD) ResizeYCoor(43);
	BYTE *TempDataPoint;
	//if ((dc.m_ps.rcPaint.top <= vLocalSystemData->vITIPCDig->vMainDisplayImageSizeY +TempOffsetY) && (dc.m_ps.rcPaint.bottom >= TempOffsetY) && 
	//	(dc.m_ps.rcPaint.left <= vLocalSystemData->vITIPCDig->vMainDisplayImageSizeX + TempOffsetX) && (dc.m_ps.rcPaint.right >= TempOffsetX))
	if (vLocalSystemData->vITIPCDig->vImageBuffer)
	if ((vShowAlternateImage) || (vHaveAnImage) || (vHaveScaledImageToPaint))
	{
		TempDataPoint = vLocalSystemData->vITIPCDig->vImageBuffer;

		CDC TempCompatableDeviceContext;
		TempCompatableDeviceContext.CreateCompatibleDC(&dc);
		HBITMAP TempBitmapOfReject = CreateCompatibleBitmap ( dc, vLocalSystemData->vITIPCDig->vDisplayImageSizeX, vLocalSystemData->vITIPCDig->vDisplayImageSizeY );
		BITMAPINFO TempBitmapInfo = {0};		//http://stackoverflow.com/questions/17137100/c-changing-the-values-in-a-bitmap
		TempBitmapInfo.bmiHeader.biSize = sizeof(TempBitmapInfo.bmiHeader);
		int TempBitmapColorArraySize = vLocalSystemData->vITIPCDig->vDisplayImageSizeX * vLocalSystemData->vITIPCDig->vDisplayImageSizeY * 4;
		int TempIterator = TempBitmapColorArraySize;

		if (0 == GetDIBits(dc, TempBitmapOfReject, 0, 0, NULL, &TempBitmapInfo, DIB_RGB_COLORS))  //get just bitmap info header
		{
			int r = 0;
		}
		
		TempBitmapInfo.bmiHeader.biBitCount = 32;  
		TempBitmapInfo.bmiHeader.biCompression = BI_RGB;  

		BYTE* TempBitmapColorArray = new BYTE[TempBitmapColorArraySize];
		GetDIBits(dc, TempBitmapOfReject, 0, TempBitmapInfo.bmiHeader.biHeight, (LPVOID)TempBitmapColorArray, &TempBitmapInfo, DIB_RGB_COLORS);

		if (((vMode == cCenterExposureMenu) /*|| (vMode == cEdgeExposureMenu)*/) && (vShowImageInColor))
		{
			BYTE TempColor = 0;
			BYTE TempColor1 = 0;
			TempIterator = TempBitmapColorArraySize - vLocalSystemData->vITIPCDig->vDisplayImageSizeX * 4;
			for (WORD TempLoopY = 0; TempLoopY < vLocalSystemData->vITIPCDig->vDisplayImageSizeY; TempLoopY++)
			{
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
							TempBitmapColorArray[TempIterator + 2] = 0;//red
							TempBitmapColorArray[TempIterator + 3] = 255;//alpha
							//dc.SetPixelV(TempOffsetX + TempLoopX,TempOffsetY + TempLoopY, 
							//	RGB(0,0,0)); //draw pixel in black if 0
						}
					}
					else
					if (*TempDataPoint == 255)
					{
						if ((TempOffsetX + TempLoopX <= dc.m_ps.rcPaint.right) &&
							(TempOffsetX + TempLoopX >= dc.m_ps.rcPaint.left) &&
							(TempOffsetY + TempLoopY >= dc.m_ps.rcPaint.top) &&
							(TempOffsetY + TempLoopY <= dc.m_ps.rcPaint.bottom))
						{
							TempBitmapColorArray[TempIterator] = 255;//blUE
							TempBitmapColorArray[TempIterator + 1] = 255;//green
							TempBitmapColorArray[TempIterator + 2] = 255;//red
							TempBitmapColorArray[TempIterator + 3] = 255;//alpha
							//dc.SetPixelV(TempOffsetX + TempLoopX,TempOffsetY + TempLoopY, 
							//	RGB(0xFF,0xFF,0xFF)); //draw pixel in white if 255
						}
					}
					else
					if (*TempDataPoint < 100)
					{
						TempColor = *TempDataPoint * 2;
						TempColor1 = *TempDataPoint;
						if ((TempOffsetX + TempLoopX <= dc.m_ps.rcPaint.right) &&
							(TempOffsetX + TempLoopX >= dc.m_ps.rcPaint.left) &&
							(TempOffsetY + TempLoopY >= dc.m_ps.rcPaint.top) &&
							(TempOffsetY + TempLoopY <= dc.m_ps.rcPaint.bottom))
						{
							TempBitmapColorArray[TempIterator] = TempColor;//blUE
							TempBitmapColorArray[TempIterator + 1] = TempColor1;//green
							TempBitmapColorArray[TempIterator + 2] = TempColor1;//red
							TempBitmapColorArray[TempIterator + 3] = 255;//alpha
							//dc.SetPixelV(TempOffsetX + TempLoopX,TempOffsetY + TempLoopY, 
							//	RGB(TempColor1,TempColor1,TempColor)); 
						}	//draw pixel in shade of blue if below 100
					}
					else
					if (*TempDataPoint < 200)
					{
						TempColor = (BYTE)((*TempDataPoint) / 1.2) + 48;
						if ((TempOffsetX + TempLoopX <= dc.m_ps.rcPaint.right) &&
							(TempOffsetX + TempLoopX >= dc.m_ps.rcPaint.left) &&
							(TempOffsetY + TempLoopY >= dc.m_ps.rcPaint.top) &&
							(TempOffsetY + TempLoopY <= dc.m_ps.rcPaint.bottom))
						{
							TempBitmapColorArray[TempIterator] = 0;//blUE
							TempBitmapColorArray[TempIterator + 1] = TempColor;//green
							TempBitmapColorArray[TempIterator + 2] = 0;//red
							TempBitmapColorArray[TempIterator + 3] = 255;//alpha
							//dc.SetPixelV(TempOffsetX + TempLoopX,TempOffsetY + TempLoopY, 
							//	RGB(0,TempColor,0)); //draw pixel in shade of green if below 200
						}
					}
					else
					{
						TempColor = ((*TempDataPoint - 200) * 1) + 200;
						if ((TempOffsetX + TempLoopX <= dc.m_ps.rcPaint.right) &&
							(TempOffsetX + TempLoopX >= dc.m_ps.rcPaint.left) &&
							(TempOffsetY + TempLoopY >= dc.m_ps.rcPaint.top) &&
							(TempOffsetY + TempLoopY <= dc.m_ps.rcPaint.bottom))
						{
							TempBitmapColorArray[TempIterator] = 0;//blUE
							TempBitmapColorArray[TempIterator + 1] = TempColor;//green
							TempBitmapColorArray[TempIterator + 2] = TempColor;//red
							TempBitmapColorArray[TempIterator + 3] = 255;//alpha
							//dc.SetPixelV(TempOffsetX + TempLoopX,TempOffsetY + TempLoopY, 
							//	RGB(TempColor,TempColor,0)); //draw pixel yellow
						}
					}
					TempDataPoint++;
					TempIterator = TempIterator + 4;
				}
				TempIterator = TempIterator - vLocalSystemData->vITIPCDig->vDisplayImageSizeX * 8;
			}
		}
		else
		if (((vMode == cSetContaminantThreshold) || (vMode == cSetVoidThreshold) || ((vMode == cSetSizeSettings) && (vShowImageInColor))) && (vHaveAnImage))
		{
			TempIterator = TempBitmapColorArraySize - vLocalSystemData->vITIPCDig->vDisplayImageSizeX * 4;
			for (WORD TempLoopY = 0; TempLoopY < vLocalSystemData->vITIPCDig->vDisplayImageSizeY; TempLoopY++)
			{
				for (WORD TempLoopX = 0; TempLoopX < vLocalSystemData->vITIPCDig->vDisplayImageSizeX; TempLoopX++)
				{
					if ((*TempDataPoint == 0) || ((*TempDataPoint == 1) && (vInspectionEditing) && ((vInspectionEditing->vInspectionType == cItemCountInspection) || (vInspectionEditing->vInspectionType == cMissingLidInspectionOnSize) || (vInspectionEditing->vInspectionType == cSizeInspection)))) //black pixel
					{
						if ((TempOffsetX + TempLoopX <= dc.m_ps.rcPaint.right) && (TempOffsetX + TempLoopX >= dc.m_ps.rcPaint.left) &&
							(TempOffsetY + TempLoopY >= dc.m_ps.rcPaint.top) && (TempOffsetY + TempLoopY <= dc.m_ps.rcPaint.bottom))
						{
							TempBitmapColorArray[TempIterator] = 0;//blUE
							TempBitmapColorArray[TempIterator + 1] = 0;//green
							TempBitmapColorArray[TempIterator + 2] = 255;//red
							TempBitmapColorArray[TempIterator + 3] = 255;//alpha
							//dc.SetPixelV(TempOffsetX + TempLoopX,TempOffsetY + TempLoopY, 
							//	RGB(0xFF,0,0)); //draw pixel in red
						}
					}
					else
					if (*TempDataPoint == 1)
					{
						if ((TempOffsetX + TempLoopX <= dc.m_ps.rcPaint.right) && (TempOffsetX + TempLoopX >= dc.m_ps.rcPaint.left) &&
							(TempOffsetY + TempLoopY >= dc.m_ps.rcPaint.top) && (TempOffsetY + TempLoopY <= dc.m_ps.rcPaint.bottom))
						{
							TempBitmapColorArray[TempIterator] = 255;//blUE
							TempBitmapColorArray[TempIterator + 1] = 0;//green
							TempBitmapColorArray[TempIterator + 2] = 0;//red
							TempBitmapColorArray[TempIterator + 3] = 255;//alpha
							//dc.SetPixelV(TempOffsetX + TempLoopX,TempOffsetY + TempLoopY, RGB(0,0,0xFF)); //draw pixel in blue
						}
					}
					else
					{
						if ((TempOffsetX + TempLoopX <= dc.m_ps.rcPaint.right) && (TempOffsetX + TempLoopX >= dc.m_ps.rcPaint.left) &&
							(TempOffsetY + TempLoopY >= dc.m_ps.rcPaint.top) && (TempOffsetY + TempLoopY <= dc.m_ps.rcPaint.bottom))
						{
							TempBitmapColorArray[TempIterator] = *TempDataPoint;//blUE
							TempBitmapColorArray[TempIterator + 1] = *TempDataPoint;//green
							TempBitmapColorArray[TempIterator + 2] = *TempDataPoint;//red
							TempBitmapColorArray[TempIterator + 3] = 255;//alpha
							//dc.SetPixelV(TempOffsetX + TempLoopX,TempOffsetY + TempLoopY, 
							//	RGB(*TempDataPoint,*TempDataPoint,*TempDataPoint)); //draw pixel in intensity
						}
					}
					TempDataPoint++;
					TempIterator = TempIterator + 4;
				}
				TempIterator = TempIterator - vLocalSystemData->vITIPCDig->vDisplayImageSizeX * 8;
			}
		}
		else
		{
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
						TempBitmapColorArray[TempIterator + 3] = 255;//alphadc.SetPixelV(TempOffsetX + TempLoopX,TempOffsetY + TempLoopY, 
						//RGB(*TempDataPoint,*TempDataPoint,*TempDataPoint)); //draw pixel in intensity
					}
					TempDataPoint++;
					TempIterator = TempIterator + 4;
				}
				TempIterator = TempIterator - vLocalSystemData->vITIPCDig->vDisplayImageSizeX * 8;
			}
		}
		if (vLocalSystemData->vMultiLaneInSetupLanePositionsMenu)
		{
			for (BYTE TempLaneLoop = 0; TempLaneLoop < vGlobalCurrentProduct->vMultiLaneNumberOfLanes; TempLaneLoop++)
			{
				BYTE TempGreenColor = 0;
				BYTE TempRedColor = 0;
				BYTE TempBlueColor = 255;

				WORD TempStartLinePosition = vGlobalCurrentProduct->vMultiLaneStartPixel[TempLaneLoop] * vLocalSystemData->vITIPCDig->vDisplayImageSizeY / vLocalSystemData->vITIPCDig->vOriginalBufferSizeX;

				WORD TempEndLinePosition = (vGlobalCurrentProduct->vMultiLaneStartPixel[TempLaneLoop] + vGlobalCurrentProduct->vMultiLaneWidthInPixels[TempLaneLoop]) *
					vLocalSystemData->vITIPCDig->vDisplayImageSizeY / vLocalSystemData->vITIPCDig->vOriginalBufferSizeX;

				if (TempStartLinePosition >= vLocalSystemData->vITIPCDig->vDisplayImageSizeY - 1)
					TempStartLinePosition = vLocalSystemData->vITIPCDig->vDisplayImageSizeY - 2;

				if (TempEndLinePosition >= vLocalSystemData->vITIPCDig->vDisplayImageSizeY - 1)
					TempEndLinePosition = vLocalSystemData->vITIPCDig->vDisplayImageSizeY - 2;

				//draw the start line of each lane in blue
				TempIterator = TempStartLinePosition * vLocalSystemData->vITIPCDig->vDisplayImageSizeX * 4;
				for (WORD TempLoopX = 0; TempLoopX < vLocalSystemData->vITIPCDig->vDisplayImageSizeX; TempLoopX++)
				{
					if ((TempLoopX > 10) && (TempLoopX < vLocalSystemData->vITIPCDig->vDisplayImageSizeX - 10))
					{
						TempBitmapColorArray[TempIterator] = TempBlueColor;//blue
						TempBitmapColorArray[TempIterator + 1] = TempGreenColor;//green
						TempBitmapColorArray[TempIterator + 2] = TempRedColor;//red
						TempBitmapColorArray[TempIterator + 3] = 255;//alpha
					}
					TempIterator = TempIterator + 4;
				}

				//draw the end line of each lane in blue
				TempIterator = TempEndLinePosition * vLocalSystemData->vITIPCDig->vDisplayImageSizeX * 4;
				for (WORD TempLoopX = 0; TempLoopX < vLocalSystemData->vITIPCDig->vDisplayImageSizeX; TempLoopX++)
				{
					if ((TempLoopX > 10) && (TempLoopX < vLocalSystemData->vITIPCDig->vDisplayImageSizeX - 10))
					{
						TempBitmapColorArray[TempIterator] = TempBlueColor;//blue
						TempBitmapColorArray[TempIterator + 1] = TempGreenColor;//green
						TempBitmapColorArray[TempIterator + 2] = TempRedColor;//red
						TempBitmapColorArray[TempIterator + 3] = 255;//alpha
					}
					TempIterator = TempIterator + 4;
				}
			}

			BYTE TempGreenColor = 0;
			BYTE TempRedColor = 0;
			BYTE TempBlueColor = 0;

			WORD TempStartLinePosition = vGlobalCurrentProduct->vMultiLaneStartPixel[vMultiImageLaneEditing] * vLocalSystemData->vITIPCDig->vDisplayImageSizeY / 
				vLocalSystemData->vITIPCDig->vOriginalBufferSizeX;

			WORD TempEndLinePosition = (vGlobalCurrentProduct->vMultiLaneStartPixel[vMultiImageLaneEditing] + vGlobalCurrentProduct->vMultiLaneWidthInPixels[vMultiImageLaneEditing]) *
				vLocalSystemData->vITIPCDig->vDisplayImageSizeY / vLocalSystemData->vITIPCDig->vOriginalBufferSizeX;

			if (TempEndLinePosition >= vLocalSystemData->vITIPCDig->vDisplayImageSizeY)
				TempEndLinePosition = vLocalSystemData->vITIPCDig->vDisplayImageSizeY - 1;

			TempGreenColor = 255;

			//draw the start line of the lane that is being edited in Green
			TempIterator = TempStartLinePosition * vLocalSystemData->vITIPCDig->vDisplayImageSizeX * 4;
			for (WORD TempLoopX = 0; TempLoopX < vLocalSystemData->vITIPCDig->vDisplayImageSizeX; TempLoopX++)
			{
				if ((TempLoopX > 10) && (TempLoopX < vLocalSystemData->vITIPCDig->vDisplayImageSizeX - 10))
				{
					TempBitmapColorArray[TempIterator] = TempBlueColor;//blue
					TempBitmapColorArray[TempIterator + 1] = TempGreenColor;//green
					TempBitmapColorArray[TempIterator + 2] = TempRedColor;//red
					TempBitmapColorArray[TempIterator + 3] = 255;//alpha
				}
				TempIterator = TempIterator + 4;
			}

			TempGreenColor = 0;
			TempRedColor = 255;

			//draw the end line of the lane that is being edited in Red
			TempIterator = TempEndLinePosition * vLocalSystemData->vITIPCDig->vDisplayImageSizeX * 4;
			for (WORD TempLoopX = 0; TempLoopX < vLocalSystemData->vITIPCDig->vDisplayImageSizeX; TempLoopX++)
			{
				if ((TempLoopX > 10) && (TempLoopX < vLocalSystemData->vITIPCDig->vDisplayImageSizeX - 10))
				{
					TempBitmapColorArray[TempIterator] = TempBlueColor;//blue
					TempBitmapColorArray[TempIterator + 1] = TempGreenColor;//green
					TempBitmapColorArray[TempIterator + 2] = TempRedColor;//red
					TempBitmapColorArray[TempIterator + 3] = 255;//alpha
				}
				TempIterator = TempIterator + 4;
			}
		}

		//if (vDrawPurpleLine)
		if (vGlobalCurrentProduct)
		{
			//if ((dc.m_ps.rcPaint.top <= vLocalSystemData->vITIPCDig->vMainDisplayImageSizeY + TempOffsetY) &&
			//	(dc.m_ps.rcPaint.bottom >= TempOffsetY) && (dc.m_ps.rcPaint.left <= TempOffsetX) && (dc.m_ps.rcPaint.right >= TempOffsetX - 7))
			{
				WORD TempTop = (WORD)(vGlobalCurrentProduct->vEdgeLocationBottomPixel + vGlobalCurrentProduct->vEdgeLocationHeightPixel);
				TempTop = TempTop * vLocalSystemData->vITIPCDig->vDisplayImageSizeY / vLocalSystemData->vITIPCDig->vOriginalBufferSizeX;

				if (vLocalSystemData->vITIPCDig->vDisplayImageSizeY >= TempTop)
					TempTop = vLocalSystemData->vITIPCDig->vDisplayImageSizeY - TempTop;
				else
					TempTop = 0;

				WORD TempBottom = (WORD)(vGlobalCurrentProduct->vEdgeLocationBottomPixel);
				WORD TempHeight = (WORD)((vGlobalCurrentProduct->vEdgeLocationHeightPixel));

				TempHeight = TempHeight * vLocalSystemData->vITIPCDig->vDisplayImageSizeY / 
					vLocalSystemData->vITIPCDig->vOriginalBufferSizeX;

				TempBottom = TempBottom * vLocalSystemData->vITIPCDig->vDisplayImageSizeY / 
					vLocalSystemData->vITIPCDig->vOriginalBufferSizeX;

				int TempResize7XCoor = ResizeXCoor(7);
				int TempResize336YCoor = ResizeYCoor(336);

				if (vDrawPurpleLine) //only blank old purple line if it moved
				for (WORD TempLoopY = 0; TempLoopY < vLocalSystemData->vITIPCDig->vDisplayImageSizeY; TempLoopY++)
				for (WORD TempLoopX = 1; TempLoopX < TempResize7XCoor; TempLoopX++)
				{
					if ((TempOffsetX - TempLoopX <= dc.m_ps.rcPaint.right) && (TempOffsetX - TempLoopX >= dc.m_ps.rcPaint.left) &&
						(TempOffsetY + TempLoopY >= dc.m_ps.rcPaint.top) && (TempOffsetY + TempLoopY <= dc.m_ps.rcPaint.bottom))
					dc.SetPixelV(TempOffsetX - TempLoopX,TempOffsetY + TempLoopY, cLightGray); //draw pixel in Light Grey
				}
				for (WORD TempLoopY = 0; TempLoopY <= TempHeight; TempLoopY++)
				for (WORD TempLoopX = 1; TempLoopX < TempResize7XCoor; TempLoopX++)
				{
					if ((TempOffsetX - TempLoopX <= dc.m_ps.rcPaint.right) && (TempOffsetX - TempLoopX >= dc.m_ps.rcPaint.left) &&
						(TempOffsetY + TempLoopY + TempTop >= dc.m_ps.rcPaint.top) && (TempOffsetY + TempLoopY + TempTop <= dc.m_ps.rcPaint.bottom))
					dc.SetPixelV(TempOffsetX - TempLoopX,TempOffsetY  + TempResize336YCoor - TempLoopY - TempBottom, cPurple); //draw pixel in purple
				}

				//draw bottom purple line
				if (vLocalSystemData->vITIPCDig->vDoBottomReferencing)
				if (vGlobalCurrentProduct->vBottomLocationLengthPixel)
				{
					WORD TempLeft = (WORD)(vGlobalCurrentProduct->vBottomLocationLeftPixel);
					TempLeft = TempLeft * vLocalSystemData->vITIPCDig->vDisplayImageSizeX / vLocalSystemData->vITIPCDig->vOriginalBufferSizeY;

					//purple line is referenced to left edge of container
					if ((vLocalSystemData->vITIPCDig->vOriginalImage) && (vLocalSystemData->vITIPCDig->vOriginalImage->vGreyImage) && (vLocalSystemData->vITIPCDig->vOriginalImage->vLeft))
						TempLeft = TempLeft + (vLocalSystemData->vITIPCDig->vOriginalImage->vLeft * vLocalSystemData->vITIPCDig->vDisplayImageSizeX / vLocalSystemData->vITIPCDig->vOriginalBufferSizeY);

					if (vLocalSystemData->vITIPCDig->vDisplayImageSizeX < TempLeft)
						TempLeft = vLocalSystemData->vITIPCDig->vDisplayImageSizeX;

					WORD TempRight = (WORD)(vGlobalCurrentProduct->vBottomLocationLeftPixel + vGlobalCurrentProduct->vBottomLocationLengthPixel);
					TempRight = TempRight * vLocalSystemData->vITIPCDig->vDisplayImageSizeX / vLocalSystemData->vITIPCDig->vOriginalBufferSizeY;

					//purple line is referenced to left edge of container
					if ((vLocalSystemData->vITIPCDig->vOriginalImage) && (vLocalSystemData->vITIPCDig->vOriginalImage->vGreyImage) && (vLocalSystemData->vITIPCDig->vOriginalImage->vLeft))
						TempRight = TempRight + (vLocalSystemData->vITIPCDig->vOriginalImage->vLeft * vLocalSystemData->vITIPCDig->vDisplayImageSizeX / vLocalSystemData->vITIPCDig->vOriginalBufferSizeY);

					if (vLocalSystemData->vITIPCDig->vDisplayImageSizeX < TempRight)
						TempRight = vLocalSystemData->vITIPCDig->vDisplayImageSizeX;

					TempHeight = TempRight - TempLeft;

					WORD TempStartX = TempOffsetX;
					WORD TempStartY = TempOffsetY + vLocalSystemData->vITIPCDig->vDisplayImageSizeY;

					for (WORD TempLoopX = 0; TempLoopX < vLocalSystemData->vITIPCDig->vDisplayImageSizeX; TempLoopX++)
					for (WORD TempLoopY = 1; TempLoopY < TempResize7XCoor; TempLoopY++)
					{
						//if ((TempOffsetX - TempLoopX <= dc.m_ps.rcPaint.right) && (TempOffsetX - TempLoopX >= dc.m_ps.rcPaint.left) &&
						//	(TempOffsetY + TempLoopY >= dc.m_ps.rcPaint.top) && (TempOffsetY + TempLoopY <= dc.m_ps.rcPaint.bottom))
						dc.SetPixelV(TempStartX + TempLoopX, TempStartY + TempLoopY, cLightGray); //draw pixel in Light Grey
					}
					for (WORD TempLoopX = 0; TempLoopX <= TempHeight; TempLoopX++)
					for (WORD TempLoopY = 1; TempLoopY < TempResize7XCoor; TempLoopY++)
					{
						//if ((TempOffsetX - TempLoopX <= dc.m_ps.rcPaint.right) && (TempOffsetX - TempLoopX >= dc.m_ps.rcPaint.left) &&
						//	(TempOffsetY + TempLoopY + TempTop >= dc.m_ps.rcPaint.top) && (TempOffsetY + TempLoopY + TempTop <= dc.m_ps.rcPaint.bottom))
						dc.SetPixelV(TempStartX + TempLoopX + TempLeft, TempStartY + TempLoopY, cPurple); //draw pixel in purple
					}
				}
			}
			vDrawPurpleLine = false;
		}

		SetDIBits(dc, TempBitmapOfReject, 0, TempBitmapInfo.bmiHeader.biHeight, (LPVOID)TempBitmapColorArray, &TempBitmapInfo, DIB_RGB_COLORS);
		free(TempBitmapColorArray);
		
		TempCompatableDeviceContext.SelectObject(TempBitmapOfReject);
		dc.BitBlt(TempOffsetX, TempOffsetY, TempBitmapInfo.bmiHeader.biWidth, TempBitmapInfo.bmiHeader.biHeight, &TempCompatableDeviceContext, 0, 0, SRCCOPY);

		DeleteObject(TempCompatableDeviceContext);
		DeleteObject(TempBitmapOfReject);
	}
	
	if ((vMode == cSetContaminantThreshold) || (vMode == cSetROI))
	{
		//Paint sensitivity
		if (vSensitivity < 0xFF)
		{
			//blank area on screen
			if ((vSensitivity <= 100) && (vMode != cSetROI))
				dc.SelectObject(vLocalSystemData->vGreyPen);
			else
				dc.SelectObject(vLocalSystemData->vLightGreyPen);

			for (BYTE TempLoop = 0; TempLoop < vAutoSetupSensitivityHeight; TempLoop++)
			{
				dc.MoveTo(vAutoSetupSensitivityLeft, vAutoSetupSensitivityBottom - TempLoop);
				dc.LineTo(vAutoSetupRight, vAutoSetupSensitivityBottom - TempLoop); //draw background
			}
		}

		//draw learn sensitivity background
		if (vLocalSystemData->vLearnSensitivities[vInspectionNumberEditing] < 0xFF)
		{
			if ((vMode == cSetContaminantThreshold) || (vMode == cSetROI))
				dc.SelectObject(vLocalSystemData->vGreyPen);
			else
				dc.SelectObject(vLocalSystemData->vLightGreyPen);

			for (BYTE TempLoop = 0; TempLoop < vAutoSetupSensitivityHeight; TempLoop++) //draw background of learn sensitivity
			{
				dc.MoveTo(vAutoSetupSensitivityLeft, vAutoSetupLearnSensitivityBottom - TempLoop);
				dc.LineTo(vAutoSetupSensitivityRight, vAutoSetupLearnSensitivityBottom - TempLoop); //draw background
			}
		}

		//draw learn sensitivity
		if (vLocalSystemData->vLearnSensitivities[vInspectionNumberEditing] < 0xFF)
		{
			if (vLocalSystemData->vLearnSensitivities[vInspectionNumberEditing] >= 61)
				dc.SelectObject(vLocalSystemData->vMediumDarkGreenPen);
			else
			if (vLocalSystemData->vLearnSensitivities[vInspectionNumberEditing] >= 31)
				dc.SelectObject(vLocalSystemData->vYellowPen);
			else
				dc.SelectObject(vLocalSystemData->vRedPen);

			for (BYTE TempLoop = 0; TempLoop < vAutoSetupSensitivityHeight; TempLoop++)
			{
				dc.MoveTo(vAutoSetupSensitivityLeft, vAutoSetupLearnSensitivityBottom - TempLoop);
				dc.LineTo(vAutoSetupSensitivityLeft + ResizeXCoor((int)vLocalSystemData->vLearnSensitivities[vInspectionNumberEditing]), vAutoSetupLearnSensitivityBottom - TempLoop); //draw sensitivity
			}
		}

		if ((vGlobalCurrentProduct) && (vMode != cSetROI))
		{
			//draw inspection sensitivity 
			if (vInspectionEditing)
			if (vSensitivity < 0xFF)
			{
				if (vSensitivity >= 61)
					dc.SelectObject(vLocalSystemData->vMediumDarkGreenPen);
				else
				if (vSensitivity >= 31)
					dc.SelectObject(vLocalSystemData->vYellowPen);
				else
					dc.SelectObject(vLocalSystemData->vRedPen);

				BYTE TempBarHeight = (BYTE)ResizeXCoor((int)vSensitivity);
				int TempResize15X = ResizeXCoor(15);
				if (TempBarHeight < TempResize15X) //don't have the bar less than 6 pixels in height so can be easily seen
					TempBarHeight = TempResize15X;
				if ((vInspectionEditing->vThreshold == 0) || (vInspectionEditing->vThreshold == 255))//if threshold is zero, make the bar the full height
					TempBarHeight = (BYTE)ResizeXCoor(100);

				for (BYTE TempLoop = 0; TempLoop < vAutoSetupSensitivityHeight; TempLoop++)  //draw 8 lines for each inspection bar
				{
					dc.MoveTo(vAutoSetupSensitivityLeft, vAutoSetupSensitivityBottom - TempLoop);
					dc.LineTo(vAutoSetupSensitivityLeft + (int)TempBarHeight, vAutoSetupSensitivityBottom - TempLoop); //draw sensitivity
				}
			}
		}
	}
	// Do not call CDialog::OnPaint() for painting messages
}

void CAutoSetupImageDisplayDialog::StartAccumulatingStructure()
{
	if (vLocalConfigurationData->vStructureDisableCount)
	{
		if (PasswordOK(cTemporaryInspxPassword,false))
		{
			SetDlgItemText(IDC_Width2, _T("Showing"));
			m_Width2.ShowWindow(SW_SHOW);
			m_Reference.ShowWindow(SW_SHOW);
		}
		//if (PasswordOK(cTemporaryInspxPassword,false))
		//	vLocalSystemData->vFrameCount = 0;
		if (PasswordOK(cTemporaryInspxPassword,false))
		{
			vShowAverage = false;
			vShowStructure = true;
			vShowVoidStructure = false;
			//vShowGlassStructure = false;
			vShowContaminant2Structure = false;
			vShowHistogram = false;
			//vShowGlassHistogram = false;

			//SetDlgItemText(IDC_Reference,"CStruct");
		}
	}
}

void CAutoSetupImageDisplayDialog::StartAccumulatingAverage()
{
	if (vLocalSystemData->vInAutoSetup)
		ClearDensityFeedBackLoop();
	
	if (PasswordOK(cTemporaryInspxPassword,false))
	{
		if (vLocalSystemData->vInAutoSetup)
		if (!vShowReturnToEditMenu)
			vMainWindowPointer->ResetCounters();
		SetDlgItemText(IDC_Width2, _T("Showing"));
		m_Width2.ShowWindow(SW_SHOW);
		m_Reference.ShowWindow(SW_SHOW);
	}
	/*
	vLocalSystemData->vIncludeRejectsInAverageAndStructure = true;
	if (!vLocalSystemData->vInAutoSetup)
	{
		CYesNoDialog TempYesNoDialog;
		TempYesNoDialog.vNoticeText = "\n\n\nInclude Rejects in Learned Image?";
		TempYesNoDialog.vQuestionType = cConfirmQuestion;
		TempYesNoDialog.vYesButtonText = "Include Rejects";
		int TempResult = TempYesNoDialog.DoModal();
		if (TempResult != IDOK)
			vLocalSystemData->vIncludeRejectsInAverageAndStructure = false;
	}
	*/
	if (vLocalSystemData->vLogFile.vLogSerialData)
	{
		vLocalSystemData->vLogFile.WriteToLogFile("Start Accumulating Average",cDebugMessage);
	}
	//if (PasswordOK(cTemporaryInspxPassword,false))
	//	vLocalSystemData->vFrameCount = 0;
	vOldAverageEnabled = vLocalSystemData->vITIPCDig->vAverageEnabled;
	if (PasswordOK(cTemporaryInspxPassword,false))
	{
		vShowAverage = true;
		//SetDlgItemText(IDC_Reference,"Average");
		vShowStructure = false;
		vShowVoidStructure = false;
		//vShowGlassStructure = false;
		vShowContaminant2Structure = false;
		vShowHistogram = false;
		//vShowGlassHistogram = false;

	}
}

void CAutoSetupImageDisplayDialog::OnSubFunction8Button3() 
{
	//EditNotes();
}

void CAutoSetupImageDisplayDialog::OnSubFunction8Button2() 
{
	OnSubFunction8Button();
}

void CAutoSetupImageDisplayDialog::OnSubFunction8Button1() 
{
	OnSubFunction8Button();
}

void CAutoSetupImageDisplayDialog::OnSubFunction6Button3() 
{
	OnSubFunction6Button();
}

void CAutoSetupImageDisplayDialog::OnSubFunction6Button2() 
{
	OnSubFunction6Button();
}

void CAutoSetupImageDisplayDialog::OnSubFunction6Button1() 
{
	OnSubFunction6Button();
}

bool CAutoSetupImageDisplayDialog::NoSetupOrOKToChange() 
{
	bool TempOKToChange = true;
	CString TempText;
	CString TempText1;

	if (vGlobalCurrentProduct->ProductHasImageFiles(vGlobalCurrentDirectory))
	{
		CYesNoDialog TempYesNoDialog;
		TempText.LoadString(IDS_ThisproducthasbeenLearned);//"This product has been Learned.\nIf you change this setting, the Setup Data will be deleted,\nand you will have to do Learn again.\nDo you want to continue?"
		TempYesNoDialog.vNoticeText = TempText;
		TempText1.LoadString(IDS_DeleteLearn);
		TempYesNoDialog.vYesButtonText = TempText1;
		TempYesNoDialog.vQuestionType = cConfirmQuestion;
		int TempResult = TempYesNoDialog.DoModal();
		if (TempResult != IDOK)
			TempOKToChange = false;
	}
	if (TempOKToChange)
	{
		if (!vLocalSystemData->vInAutoSetup)
		if (vLocalConfigurationData->vEnableEjectors)
		{
			CYesNoDialog TempYesNoDialog;
			TempText.LoadString(IDS_ScanTracisinspectingandejectingrejects);//"ScanTrac is inspecting and ejecting rejects.\nIf you do not disable the ejectors, or abort, many containers may be ejected when this setting is changed.\nDisable Ejectors Now?";
			TempYesNoDialog.vNoticeText = TempText;
			CString TempString1;
			TempString1.LoadString(IDS_DisableEjectors);
			TempYesNoDialog.vYesButtonText = TempString1;
			TempString1.LoadString(IDS_ContinueInspecting);
			TempYesNoDialog.vNoButtonText = TempString1;
			TempYesNoDialog.vQuestionType = cConfirmQuestion;
			int TempResult = TempYesNoDialog.DoModal();
			if (TempResult == IDOK)
			{
				vMainWindowPointer->EnableDisableEjectors(false);
				ReportErrorMessage("Ejectors Disabled by operator going into Setup", cUserChanges,0);
			}
		}
		if (!vLocalSystemData->vInAutoSetup)
		if (vLocalConfigurationData->vEnableEjectors)
		{
			CYesNoDialog TempYesNoDialog;
			TempText.LoadString(IDS_ScanTracisinspectingandejectingdefectivecontainersAbort);//"ScanTrac is inspecting and ejecting defective containers.\nIf you do not abort many containers may be ejected\nwhen this setting is changed.\nAbort Now?";
			TempYesNoDialog.vNoticeText = TempText;
			CString TempString1;
			TempString1.LoadString(IDS_Abort);
			TempYesNoDialog.vYesButtonText = TempString1;
			TempString1.LoadString(IDS_Continue);
			TempYesNoDialog.vNoButtonText = TempString1;
			TempYesNoDialog.vQuestionType = cConfirmQuestion;
			int TempResult = TempYesNoDialog.DoModal();
			if (TempResult == IDOK)
				TempOKToChange = false;
		}
	}
	return TempOKToChange;
}

bool CAutoSetupImageDisplayDialog::NoSetupOrOKToMinorChange() 
{
	bool TempOKToChange = true;

	if (!vLocalSystemData->vInAutoSetup)
	//if (vGlobalCurrentProduct->ProductHasImageFiles(vGlobalCurrentDirectory))
	if (vLocalConfigurationData->vEnableEjectors)
	{
		CYesNoDialog TempYesNoDialog;
		CString TempText;
		TempText.LoadString(IDS_ScanTracisinspectingandejectingdefectivecontainersAbort);//"ScanTrac is inspecting and ejecting defective containers.\nIf you do not abort many containers may be ejected\nwhen this setting is changed.\nAbort Now?";
		TempYesNoDialog.vNoticeText = TempText;
		CString TempText1;
		TempText1.LoadString(IDS_Abort);
		TempYesNoDialog.vYesButtonText = TempText1;
		TempText1.LoadString(IDS_Continue);
		TempYesNoDialog.vNoButtonText = TempText1;
		TempYesNoDialog.vQuestionType = cConfirmQuestion;
		int TempResult = TempYesNoDialog.DoModal();
		if (TempResult == IDOK)
			TempOKToChange = false;
	}
	if (TempOKToChange)
	if (!vLocalSystemData->vInAutoSetup)
	if (vLocalConfigurationData->vEnableEjectors)
	{
		CYesNoDialog TempYesNoDialog;
		CString TempText;
		TempText.LoadString(IDS_ScanTracisinspectingandejectingrejects);//"ScanTrac is inspecting and ejecting rejects.\nIf you do not disable the ejectors, or abort, many containers may be ejected when this setting is changed.\nDisable Ejectors Now?";
		TempYesNoDialog.vNoticeText = TempText;
		CString TempText1;
		TempText1.LoadString(IDS_DisableEjectors);
		TempYesNoDialog.vYesButtonText = TempText1;
		TempText1.LoadString(IDS_ContinueInspecting);
		TempYesNoDialog.vNoButtonText = TempText1;
		TempYesNoDialog.vQuestionType = cConfirmQuestion;
		int TempResult = TempYesNoDialog.DoModal();
		if (TempResult == IDOK)
		{
			vMainWindowPointer->EnableDisableEjectors(false);
			ReportErrorMessage("Ejectors Disabled by operator during Setup", cUserChanges,0);
		}
	}
	return TempOKToChange;
}

void CAutoSetupImageDisplayDialog::YouCantChangeSettingBecauseInspecting()
{
	CNoticeDialog TempNoticeDialog;
	CString TempText;
	TempText.LoadString(IDS_YoucannotchangethissettingbecauseScanTracisinspectingcontainers);//"\nYou cannot change this setting because\nScanTrac is inspecting containers.\nYou must exit setup, stop running,\nthen enter Setup Product to change this setting.";
	TempNoticeDialog.vNoticeText = TempText;
	TempNoticeDialog.vType = cErrorMessage;
	TempNoticeDialog.DoModal();
}

void CAutoSetupImageDisplayDialog::StartingSystemPleaseTryAgain()
{
	CNoticeDialog TempNoticeDialog;
	CString TempText;
	TempText.LoadString(IDS_Startingsystemtryagaininafewseconds);//"\n\n\nStarting system, try again in a few seconds"

	if (PasswordOK(cSuperInspxPassword,false))
	{
			if (!vLocalSystemData->vXRayOnNotice)
				TempText = TempText + "\nX-Rays Not On";
			if (vStartingUpSource)
				TempText = TempText + "\nStarting Up Source";
			if ((vLocalSystemData->vSystemRunMode != cRunningSystemMode) && (vLocalSystemData->vSystemRunMode != cAutoSetupRunningSystem))
				TempText = TempText + "\nNot In Run Mode";
			if (vRestartingXRaysWithNewSettings)
				TempText = TempText + "\nRestarting X-Rays";
	}

	TempNoticeDialog.vNoticeText = TempText;
	TempNoticeDialog.vType = cNoticeMessage;
	TempNoticeDialog.vAutoClose = 2000;
	TempNoticeDialog.DoModal();
}

void CAutoSetupImageDisplayDialog::RestoreOriginalProductSettings()
{
	if (vHoldCurrentProduct)
	{
		if (vHoldCurrentProduct->vProductLockOutWidth != vGlobalCurrentProduct->vProductLockOutWidth)
		if (vMainWindowPointer)
			vMainWindowPointer->SendRetriggerLockout(vHoldCurrentProduct->vLockoutPosition);

		//restore integration time if it was changed
		if (vHoldCurrentProduct->vXRayIntegrationTime != vGlobalCurrentProduct->vXRayIntegrationTime)
		{
			vMainWindowPointer->SendProductIntegrationTime(vHoldCurrentProduct->vXRayIntegrationTime);//, 
				//vHoldCurrentProduct->vXRayIntegrationTimeAtEdge);
		}

		//restore body trigger to imager Conveyor position offset if changed
		if (vHoldCurrentProduct->vProductBodyTriggerToImageDistanceInInches != vGlobalCurrentProduct->vProductBodyTriggerToImageDistanceInInches)
		{
			vGlobaluCSerialPort->SendContainerTriggerToImageDelay(vHoldCurrentProduct);
		}

		//restore voltage and current if they were changed
		if ((vHoldCurrentProduct->vXRaySourceVoltage != vGlobalCurrentProduct->vXRaySourceVoltage) || (vHoldCurrentProduct->vXRaySourceCurrent != vGlobalCurrentProduct->vXRaySourceCurrent))
		{
			vLocalSystemData->vRampXRayCurrentAmount = vHoldCurrentProduct->vXRaySourceCurrent;
			vLocalSystemData->vRampXRayVoltageAmount = vHoldCurrentProduct->vXRaySourceVoltage;
			vMainWindowPointer->SendSourceSettingsToUController(vLocalSystemData->vRampXRayVoltageAmount,vLocalSystemData->vRampXRayCurrentAmount);
		}
		vLocalSystemData->vITIPCDig->RestoreLearnFilesFromBackups(vGlobalCurrentProduct);
		vLocalSystemData->vITIPCDig->LoadProductDataImages(vGlobalCurrentProduct);

		//restore original product settings
		//if have a checkweigh inspection, save its data, then fill data in copied inspection
		if (vLocalSystemData->vITIPCDig->vContainerCollectionLock.Lock())
		{
			//CInspectionCheckWeigh *TempInspection[cMaximumNumberOfInspections];
			//for (BYTE TempLoop = 0; TempLoop < cMaximumNumberOfInspections; TempLoop++)
			//	TempInspection[TempLoop] = NULL;

			//for (BYTE TempLoop = 0; TempLoop < vHoldCurrentProduct->vNumberOfInspections; TempLoop++)
			//if (vHoldCurrentProduct->vInspection[TempLoop])
			//if ((vHoldCurrentProduct->vInspection[TempLoop]->vInspectionType == cCheckWeighInspection) || (vHoldCurrentProduct->vInspection[TempLoop]->vInspectionType == cOverfillByWeightInspection) ||
			//	(vHoldCurrentProduct->vInspection[TempLoop]->vInspectionType == cUnderfillByWeightInspection))
			//{
			//	if (ThereIsEnoughMemory(sizeof(CInspectionCheckWeigh), "New Inspection"))
			//		TempInspection[TempLoop] = new CInspectionCheckWeigh;

			//	CInspectionCheckWeigh *TempInspectionToCopyPointer = (CInspectionCheckWeigh *)vHoldCurrentProduct->vInspection[TempLoop];
			//	TempInspection[TempLoop]->CopyInspection(TempInspectionToCopyPointer, vHoldCurrentProduct->vOverScanMultiplier);
			//}

			vGlobalCurrentProduct->CopyProduct(vHoldCurrentProduct);

			////restore any checkweigh data into its inspection
			//for (BYTE TempLoop = 0; TempLoop < vHoldCurrentProduct->vNumberOfInspections; TempLoop++)
			//if (TempInspection[TempLoop])
			//{
			//	vGlobalCurrentProduct->vInspection[TempLoop]->CopyInspection(TempInspection[TempLoop], vGlobalCurrentProduct->vOverScanMultiplier);
			//}
			vLocalSystemData->vITIPCDig->vContainerCollectionLock.Unlock();
		}
	}
}

void CAutoSetupImageDisplayDialog::AutoSizeImage()
{
	{
		CString TempText;
		if (vMode == cSetImageSize)
		//if (vNeedToSetSize)
		if (vHaveAnImage)
		{//check to see if should adjust image size
			//vNeedToSetSize = false;

			vMakeStatusLineYellowRed = false;
			vNeedToSetContainerThresholdBounds = 1;

			vHaveNotSetSize = false;
			bool TempImageGrew = false;

			float TempContainerWidth = (float)((
				(vLocalSystemData->vITIPCDig->vOriginalImage->vRight - vLocalSystemData->vITIPCDig->vOriginalImage->vLeft) 
				/ (vGlobalPixelsPerUnit * vGlobalCurrentProduct->vOverScanMultiplier)));

			bool TempFoundEdges = true;
			if (TempContainerWidth < .20)
			{
				TempFoundEdges = false;
				CNoticeDialog TempNoticeDialog;
				//TempText.LoadString(IDS_3Lines_WaitingForAGapOfAtleast2FeetToAutoSize);
				TempNoticeDialog.vNoticeText = "\nScanTrac could not find the edges of the container.\nCheck purple line settings or\nincrease the image width, and\nsend another container through to see if\nit can find both container edges.";
				TempNoticeDialog.vType = cErrorMessage;
				TempNoticeDialog.DoModal();
			}

			if (TempFoundEdges)
			if ((vLocalSystemData->vITIPCDig->vOriginalImage->vRight >= vLocalSystemData->vITIPCDig->vOriginalBufferSizeY - 3) ||
				(vLocalSystemData->vITIPCDig->vOriginalImage->vLeft <= 3))
			{
				CYesNoDialog TempYesNoDialog;
				TempYesNoDialog.vNoticeText = _T("\nDid not find both edges of the container.\nDo you want to AutoSize anyway?\nIf not, Manually increase width, and then send a container\nthrough to see if it can find both container edges.");
				TempYesNoDialog.vQuestionType = cConfirmQuestion;
				TempYesNoDialog.vYesButtonText = "AutoSize";
				TempYesNoDialog.vNoButtonText = "Don't AutoSize";
				int TempResult = TempYesNoDialog.DoModal();
				if (TempResult != IDOK)
					TempFoundEdges = false;
			}
			else
			if (TempContainerWidth < 1.5)
			{
				CYesNoDialog TempYesNoDialog;
				CString TempText1 = "\nContainer Width Found to be only: ";
				TempText1 = TempText1 + dtoa(TempContainerWidth, 2) +
					"\nDo you want to AutoSize anyway?\nIf not, Manually increase width, and then send a container\nthrough to see if it can find both container edges.";
				TempYesNoDialog.vNoticeText =  TempText1;
				TempYesNoDialog.vQuestionType = cConfirmQuestion;
				TempYesNoDialog.vYesButtonText = "AutoSize";
				TempYesNoDialog.vNoButtonText = "Don't AutoSize";
				int TempResult = TempYesNoDialog.DoModal();
				if (TempResult != IDOK)
					TempFoundEdges = false;
			}

			if (TempFoundEdges)
			{
				double TempNewProductImageWidth = (float)TempContainerWidth;

				//5/27/04 make image 30% wider than container on each side.
				vImageMargin = TempNewProductImageWidth * 0.30;

				if (vImageMargin > 2)
					vImageMargin = 2;
				TempNewProductImageWidth = TempNewProductImageWidth + (2 * vImageMargin);
				double TempWhiteSpaceOnOneSideOfImage = vImageMargin;
				if (TempWhiteSpaceOnOneSideOfImage > 4)
					TempWhiteSpaceOnOneSideOfImage = 4;
			
				//calculate new offset so width grows or shrinks from middle
				float TempNewProductBodyTriggerToImageBeltPositionOffset = (float)(vGlobalCurrentProduct->vProductBodyTriggerToImageDistanceInInches +
					vGlobalCurrentProduct->vProductImageWidth - vImageMargin - vLocalSystemData->vITIPCDig->vOriginalImage->vRight / (vGlobalPixelsPerUnit * vGlobalCurrentProduct->vOverScanMultiplier));

				if (TempNewProductBodyTriggerToImageBeltPositionOffset < 0)
					TempNewProductBodyTriggerToImageBeltPositionOffset = 0;

				if (vLocalConfigurationData->vSystemBodyTriggerToXRayBeam > 1)
				{
					double TempTriggerToBeamOffset = -(vLocalConfigurationData->vSystemBodyTriggerToXRayBeam - TempWhiteSpaceOnOneSideOfImage - TempNewProductBodyTriggerToImageBeltPositionOffset);

					//if within .25 inches of no offset, set no offset
					if ((TempTriggerToBeamOffset > -.25) && (TempTriggerToBeamOffset < .25))
					{
						ReportErrorMessage("TempNewProductBodyTriggerToImageBeltPositionOffset within .25inch: " + dtoa(TempTriggerToBeamOffset,2), cWriteToLog,0);

						ReportErrorMessage("TempNewProductBodyTriggerToImageBeltPositionOffset Original: " + dtoa(TempNewProductBodyTriggerToImageBeltPositionOffset,2), cWriteToLog,0);

						TempNewProductBodyTriggerToImageBeltPositionOffset = (float)(vLocalConfigurationData->vSystemBodyTriggerToXRayBeam - TempWhiteSpaceOnOneSideOfImage);

						ReportErrorMessage("TempNewProductBodyTriggerToImageBeltPositionOffset Final: " + dtoa(TempNewProductBodyTriggerToImageBeltPositionOffset,2), cWriteToLog,0);
					}
				}

				//container is at far right, so need to reduce the delay 2 inches
				if (vLocalSystemData->vITIPCDig->vOriginalImage->vRight >= vLocalSystemData->vITIPCDig->vOriginalBufferSizeY - 5)
				{  // if container was on left edge of image, enlarge image by two inches
					//then re-size again
					vHaveNotSetSize = true;
					TempImageGrew = true;
					//move the Conveyor position delay 1 inch over so container in center
					//if (vLocalConfigurationData->vUseMetric)
					//	TempNewProductBodyTriggerToImageBeltPositionOffset = 
					//		(vGlobalCurrentProduct->vProductBodyTriggerToImageDistanceInInches - (float)(1 * 25.4));
					//else
						TempNewProductBodyTriggerToImageBeltPositionOffset = (vGlobalCurrentProduct->vProductBodyTriggerToImageDistanceInInches -	1);

					if (TempNewProductBodyTriggerToImageBeltPositionOffset < 0)
						TempNewProductBodyTriggerToImageBeltPositionOffset = 0;

					//if (vLocalConfigurationData->vUseMetric)
					//{
					//	if (TempNewProductBodyTriggerToImageBeltPositionOffset > 25 * (float)25.4)
					//		TempNewProductBodyTriggerToImageBeltPositionOffset = 2 * (float)25.4;
					//}
					//else
					{
						if (TempNewProductBodyTriggerToImageBeltPositionOffset > 25)
							TempNewProductBodyTriggerToImageBeltPositionOffset = 2;
					}

					//if (vLocalConfigurationData->vUseMetric)
					//	TempNewProductImageWidth = (
					//		TempNewProductImageWidth + (2 * (float)25.4));
					//else
						TempNewProductImageWidth = (TempNewProductImageWidth + 2);
					//vNeedToSetSize = true;
					CString TempText;
					TempText.LoadString(IDS_WaitingForContainer);
					SetStatusLine(" " + TempText + ": " + vProductEditingName);
				}
				//else
				if (vLocalSystemData->vITIPCDig->vOriginalImage->vLeft <= 5)
				{//container on left edge, so add 2 inches to width
					vHaveNotSetSize = true;
					TempImageGrew = true;
					//if (vLocalConfigurationData->vUseMetric)
					//	TempNewProductImageWidth = (TempNewProductImageWidth + (2 * (float)25.4));
					//else
						TempNewProductImageWidth = (TempNewProductImageWidth + 2);
					CString TempText("");
					TempText.LoadString(IDS_WaitingForContainer);
					SetStatusLine(" " + TempText + ": " + vProductEditingName);
				}
									
				if (vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac)  //on Trio, can only have 672 lines in the image
				{
					WORD TempNumberOfLines = (WORD)(TempNewProductImageWidth * vGlobalPixelsPerUnit * vGlobalCurrentProduct->vOverScanMultiplier);
					if (TempNumberOfLines > 672)
					{
						TempNewProductImageWidth =  (float)((672 / vGlobalPixelsPerUnit) / vGlobalCurrentProduct->vOverScanMultiplier);
						CNoticeDialog TempNoticeDialog;
						TempNoticeDialog.vNoticeText = "\n\n\nProduct Width too many lines.\nSet to: " + dtoa(TempNewProductImageWidth,2);
						TempNoticeDialog.vType = cNoticeMessage;
						TempNoticeDialog.DoModal();
					}
				}

				double TempNewProductImageHeightTop = vGlobalCurrentProduct->vProductImageHeightTop;
				float TempNewProductImageHeightBottom = 0;

				if (vLocalConfigurationData->vScanTracType == cBriosoScanTrac)
					TempNewProductImageHeightBottom = vGlobalCurrentProduct->vProductImageHeightBottom;

				if ((vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vLocalConfigurationData->vScanTracType == cAllegroScanTrac) ||
					(vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac)) //TD
				{
					if ((vLocalSystemData->vITIPCDig->vOriginalImage->vBottom / vGlobalPixelsPerUnitInHeight) > 2)
					{
						TempNewProductImageHeightBottom = (float)((vLocalSystemData->vITIPCDig->vOriginalImage->vBottom / vGlobalPixelsPerUnitInHeight) - 2) + vGlobalCurrentProduct->vProductImageHeightBottom;
					}
					if (TempNewProductImageHeightBottom < 0)
						TempNewProductImageHeightBottom = 0;
					if (TempNewProductImageHeightBottom > vMaxDetectorHeight)
						TempNewProductImageHeightBottom = 0;

					if ((vLocalSystemData->vITIPCDig->vOriginalImage->vTop / vGlobalPixelsPerUnitInHeight) <= (vLocalConfigurationData->vNumberOfXRayDetectors * vLocalConfigurationData->vDetectorLength) - 2)
					{
						TempNewProductImageHeightTop = ((vLocalSystemData->vITIPCDig->vOriginalImage->vTop / vGlobalPixelsPerUnitInHeight) + 2 + TempNewProductImageHeightBottom + vGlobalCurrentProduct->vProductImageHeightBottom);
					}
				}
				else
				{
					TempNewProductImageHeightTop = ((vLocalSystemData->vITIPCDig->vOriginalImage->vTop / vGlobalPixelsPerUnitInHeight) + 0.4 + vGlobalCurrentProduct->vProductImageHeightBottom);
				}



				if (TempNewProductImageHeightTop > vMaxDetectorHeight)
					TempNewProductImageHeightTop = vMaxDetectorHeight;
				if (TempNewProductImageHeightTop < 0)
					TempNewProductImageHeightTop = vMaxDetectorHeight;
				if (TempNewProductImageHeightTop < TempNewProductImageHeightBottom)
					TempNewProductImageHeightTop = vMaxDetectorHeight;

				float TempOriginalProductBodyTriggerToImageBeltPositionOffset = vGlobalCurrentProduct->vProductBodyTriggerToImageDistanceInInches;

				vGlobalCurrentProduct->SetProductBodyTriggerToImageBeltPositionOffset(TempNewProductBodyTriggerToImageBeltPositionOffset);

				if (vGlobalCurrentProduct->vProductImageWidth + .3 < TempNewProductImageWidth)
				{
					vHaveNotSetSize = true;
					TempImageGrew = true;
				}

				vGlobalCurrentProduct->SetProductImageWidth((float)TempNewProductImageWidth);
				vGlobalCurrentProduct->vProductImageHeightTop = (float)TempNewProductImageHeightTop;
				vGlobalCurrentProduct->vProductImageHeightBottom = TempNewProductImageHeightBottom;

				double TempWidth = TempNewProductImageWidth;
				double TempDelta = vImageMargin * 2;
				if (TempDelta < TempWidth)
					TempWidth = (float)(TempWidth - TempDelta);

				//ask user if should figure out a new retrigger lockout width
				BYTE TempOKToCalculateNewLockout = true;
				if (vGlobalCurrentProduct->vHaveManuallySetRetriggerLockout)
				{
					CYesNoDialog TempYesNoDialog;
					TempYesNoDialog.vNoticeText = _T("The Retrigger Lockout was previously set manually.\nDo you want to keep the manual value or \nhave autosize calculate a new value based on the container size?");
					TempYesNoDialog.vQuestionType = cConfirmQuestion;
					TempYesNoDialog.vYesButtonText = "Keep Lockout";
					TempYesNoDialog.vNoButtonText = "Calculate Lockout";
					int TempResult = TempYesNoDialog.DoModal();
					if (TempResult == IDOK)
						TempOKToCalculateNewLockout = false;
					else
						vGlobalCurrentProduct->vHaveManuallySetRetriggerLockout = false;
				}

				//if ((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode) || (vLocalConfigurationData->vScanTracType == cTrioPipeLineScanTrac))
				if ((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode))
				{
					double TempNewBodyTriggerLockout = TempNewProductImageWidth / 1.93;
					vGlobalCurrentProduct->SetProductLockOutWidth((float)TempNewBodyTriggerLockout);
				}
				else
				if (TempOKToCalculateNewLockout)
				{
					double TempNewBodyTriggerLockout = TempContainerWidth;
					ReportErrorMessage("AutoSize, Set ReTrigger Lockout to Container Width in Image: " + dtoa(TempNewBodyTriggerLockout, 2), cWriteToLog, 0);

					double TempLastTriggerWidth = vLocalSystemData->vLastBodyTriggerLength / vGlobalPixelsPerUnit / vGlobalCurrentProduct->vOverScanMultiplier;
					double TempLastTriggerAverage = vLocalSystemData->vBodyTriggerWidthAverage / vGlobalPixelsPerUnit / vGlobalCurrentProduct->vOverScanMultiplier;

					if ((TempLastTriggerAverage > 1) && (vLocalSystemData->vBodyTriggerWidthStandardDeviation < 10))
					{
						if (TempNewBodyTriggerLockout < TempLastTriggerAverage)
						{
							TempNewBodyTriggerLockout = TempLastTriggerAverage;
							ReportErrorMessage("AutoSize, Set ReTrigger Lockout to Average of Optical Trigger: " + dtoa(TempNewBodyTriggerLockout, 2), cWriteToLog, 0);
						}
					}
					else
					if ((TempLastTriggerWidth > 1) && (TempLastTriggerWidth < 10))
					{
						if (TempNewBodyTriggerLockout < TempLastTriggerWidth)
						{
							TempNewBodyTriggerLockout = TempLastTriggerWidth;
							ReportErrorMessage("AutoSize, Set ReTrigger Lockout to Last Optical Trigger: " + dtoa(TempNewBodyTriggerLockout, 2), cWriteToLog, 0);
						}
					}

					if ((TempNewProductImageWidth / 1.93) > TempNewBodyTriggerLockout)
					{
						TempNewBodyTriggerLockout = TempNewProductImageWidth / 1.93;
						ReportErrorMessage("AutoSize, Set ReTrigger half of Image, Set to: " + dtoa(TempNewBodyTriggerLockout, 2), cWriteToLog, 0);
					}

					//if ((vLocalConfigurationData->vScanTracType == cMultiImageScanTrac) || (vLocalConfigurationData->vScanTracType == cTripleBeamScanTrac))
					//{
					//	TempNewBodyTriggerLockout = TempNewBodyTriggerLockout + (5 / vGlobalPixelsPerUnit / vGlobalCurrentProduct->vOverScanMultiplier); //add margin of 5 pixels on Duo and Trio
					//	ReportErrorMessage("AutoSize, Add 5 Pixels to ReTrigger Lockout: " + dtoa(TempNewBodyTriggerLockout, 2), cWriteToLog, 0);
					//}

					if (TempNewBodyTriggerLockout < 0.5)
						TempNewBodyTriggerLockout = 0.5;
								
					vGlobalCurrentProduct->SetProductLockOutWidth((float)TempNewBodyTriggerLockout);
				}
				DisplayWithPassword();

				CheckDensityCompensationInspection();

				if (vLocalSystemData->vInAutoSetup)
				{
					vLocalSystemData->vChangedSizeOfProduct = true;
					vLocalSystemData->vChangedSizeOfProductCheckDensityROI = 1;
				}
				vGlobalCurrentProduct->CheckAllROIs(vLocalSystemData->vITIPCDig->vProductAverageImage);
				CheckPurpleLineHeight();

				vLocalSystemData->vITIPCDig->CalculateAverageDriftDensity(vGlobalCurrentProduct);
				vLocalSystemData->vIndexForDriftDensityOfLastNImages = 0;

				ClearReferenceLocationData();

				vLocalSystemData->vITIPCDig->vDriftCompensationLocked = true;
				vGlobalCurrentProduct->ClearInspectionHistory();
				vAverageDensity = 0;
				SetDlgItemText(IDC_AverageDensity, _T(" "));

				if (TempNewProductImageHeightTop - TempNewProductImageHeightBottom - vGlobalCurrentProduct->vProductImageHeightBottom)
					vXScale = (float)(vLocalSystemData->vITIPCDig->vOriginalBufferSizeX / 
						((TempNewProductImageHeightTop - TempNewProductImageHeightBottom - vGlobalCurrentProduct->vProductImageHeightBottom) * vGlobalPixelsPerUnitInHeight));

				//don't show scaled image
				//vYScale = ((float)vLocalSystemData->vITIPCDig->vOriginalBufferSizeY / (float)(vGlobalCurrentProduct->vImageWidthPosition));
				//vXOffset = (WORD)(TempNewProductImageHeightBottom * vGlobalPixelsPerUnitInHeight);

				//if (TempNewProductBodyTriggerToImageBeltPositionOffset > TempOriginalProductBodyTriggerToImageBeltPositionOffset)
				//	vYOffset = (WORD)(vLocalSystemData->vITIPCDig->vOriginalImage->vLeft - (vImageMargin * vGlobalPixelsPerUnit * vGlobalCurrentProduct->vOverScanMultiplier));

				if (PasswordOK(cTemporaryInspxPassword,false))
					SetDlgItemText(IDC_SubFunction5Display, dtoa(vGlobalCurrentProduct->vProductImageHeightTop,2) + 
					" (" + dtoa(vGlobalCurrentProduct->vProductImageHeightTop * vGlobalPixelsPerUnitInHeight,0) + ")");
				else
					SetDlgItemText(IDC_SubFunction5Display, dtoa(vGlobalCurrentProduct->vProductImageHeightTop,2));

				if ((PasswordOK(cTemporaryInspxPassword,false)) && (vGlobalCurrentProduct->vProductImageHeightBottom))
					SetDlgItemText(IDC_SubFunction6Display, dtoa(vGlobalCurrentProduct->vProductImageHeightBottom,2) + " (" + dtoa(vGlobalCurrentProduct->vProductImageHeightBottom * vGlobalPixelsPerUnitInHeight,0) + ")");
				else
					SetDlgItemText(IDC_SubFunction6Display, dtoa(vGlobalCurrentProduct->vProductImageHeightBottom,2));
						
				double TempTriggerToBeamOffset = vGlobalCurrentProduct->vProductBodyTriggerToImageDistanceInInches;
				
				if (vLocalConfigurationData->vSystemBodyTriggerToXRayBeam > 1)
					TempTriggerToBeamOffset = -(vLocalConfigurationData->vSystemBodyTriggerToXRayBeam - vGlobalCurrentProduct->vProductBodyTriggerToImageDistanceInInches - TempWhiteSpaceOnOneSideOfImage);
				
				SetDlgItemText(IDC_SubFunction7Display, dtoa(TempTriggerToBeamOffset,2));
		
				if (PasswordOK(cTemporaryInspxPassword,false))
					SetDlgItemText(IDC_SubFunction8Display,
						dtoa(vGlobalCurrentProduct->vProductImageWidth,2) + " (" + dtoa(vGlobalCurrentProduct->vImageWidthPosition,0) + ")");
				else
					SetDlgItemText(IDC_SubFunction8Display, dtoa(vGlobalCurrentProduct->vProductImageWidth,2));

				vGlobalCurrentProduct->vMaximumImageOverlap = (vGlobalCurrentProduct->vProductImageWidth / 2) - .1;
				ClearReferenceLocationData();

				vLocalSystemData->vITIPCDig->RenameLearnFilesToBackup(vGlobalCurrentProduct);
				ChangedProductSettingsSoReload();
				vLocalSystemData->vITIPCDig->DeleteLearnImages();
				vLocalSystemData->vITIPCDig->DeleteLearnFiles(vGlobalCurrentProduct);
				ClearDensityFeedBackLoop();

				//DisplayScale();

				//if (TempImageGrew)
				//if (vLocalSystemData->vITIPCDig->vOriginalImage)
				//if (vLocalSystemData->vITIPCDig->vOriginalImage->vGreyImage)
				//	FillMemory(vLocalSystemData->vITIPCDig->vOriginalImage->vGreyImage,
				//		vLocalSystemData->vITIPCDig->vOriginalBufferSize,196);
				//LoadBackGroundIfNeeded();
			}
			vAutosizedContainer = true;
			TempText.LoadString(IDS_Waitingfornextimagetoupdatescreen);
			TempText = " " + TempText;
			if (!vChangedEdgeFindingMode)
				SetStatusLine(TempText);
			//CWnd *TempWindow = CWnd::GetDlgItem(IDC_Function5Button);
			//if (TempWindow)
			//	TempWindow->Invalidate(true);
			//TempWindow = CWnd::GetDlgItem(IDC_ArrowDown5);
			//TempWindow->Invalidate(false);
			UpdateButtonText();
			//DrawImageOnScreen();  //don't show scaled image
			vHaveScaledImageToPaint = true;
			vHaveAnImage = false;
			m_SubFunction2Button.Invalidate(false);
		}  //end of setting image size
	}
}

void CAutoSetupImageDisplayDialog::DisplayCurrentDensityValue()
{
	double TempDensity = 0;
	if ((vLocalSystemData->vITIPCDig->vOriginalImage) && (vLocalSystemData->vITIPCDig->vOriginalImage->vGreyImage))
	if (vHaveAnImage)
	if (vGlobalCurrentProduct->vDriftDensityInspection)
	{
		TempDensity = 
			vLocalSystemData->vITIPCDig->CalculateInspectionDensityAfterReference(
			vLocalSystemData->vITIPCDig->vOriginalImage->vGreyImage,
			vGlobalCurrentProduct->vDriftDensityInspection,
			vLocalSystemData->vITIPCDig->vOriginalImage->vLeftReferenceAdjust,
			vLocalSystemData->vITIPCDig->vOriginalImage->vTopReferenceAdjust, 
			vLocalConfigurationData->vReferenceROIsToEdges,
			vLocalSystemData->vITIPCDig->vOriginalImage->vLeft,
			vLocalSystemData->vITIPCDig->vOriginalImage->vRight); 
	}
	else
	{
		TempDensity = vLocalSystemData->vITIPCDig->CalculateFullDensity(
			vLocalSystemData->vITIPCDig->vOriginalImage->vGreyImage, 
			vLocalSystemData->vITIPCDig->vOriginalBufferSize); 
	}
	if (vCurrentDensity != TempDensity)
	{
		if (vMode == cCenterExposureMenu)
		if ((((vCurrentDensity > 80) && (vCurrentDensity < 180)) &&
			((TempDensity >= 180) || (TempDensity <= 80))) ||
			(((TempDensity > 80) && (TempDensity < 180)) &&
			((vCurrentDensity >= 180) || (vCurrentDensity <= 80))))
		{
			CWnd *TempWindow = CWnd::GetDlgItem(IDC_SubFunction6Button);
			if (TempWindow)
				TempWindow->Invalidate(true);

			TempWindow = CWnd::GetDlgItem(IDC_Function5Button);
			if (TempWindow)
				TempWindow->Invalidate(true);
		}

		if (vCurrentDensity != TempDensity)
		{
			if ((!TempDensity) && (vCurrentDensity))
				m_CurrentDensity.ShowWindow(SW_HIDE);
			if ((TempDensity) && (!vCurrentDensity))
				m_CurrentDensity.ShowWindow(SW_SHOW);
			vCurrentDensity = TempDensity;

			CString TempString = dtoa(vCurrentDensity, 1);
			CString TempText;
			TempText.LoadString(IDS_Cur);
			TempString = TempText + ":" + TempString;
			SetDlgItemText(IDC_CurrentDensity, TempString);
		}
	}
	if (vMode == cCenterExposureMenu)
	{
		BYTE TempDarkestPixelValue = 255;
		BYTE *TempPointer = vLocalSystemData->vITIPCDig->vOriginalImage->vGreyImage;
		for (DWORD TempLoop = 0; TempLoop < vLocalSystemData->vITIPCDig->vOriginalBufferSize; TempLoop++)
		{
			if (*TempPointer < TempDarkestPixelValue)
				TempDarkestPixelValue = *TempPointer;
			TempPointer++;
		}
		SetDlgItemText(IDC_SizeX,dtoa(TempDarkestPixelValue,0));
	}
}

void CAutoSetupImageDisplayDialog::UpdateAverageDensityDisplay()
{
	//9/9/2009
	if (!vGlobalCurrentProduct->vDriftDensityInspection)
		CheckDensityCompensationInspection();

	vGlobalCurrentProduct->CheckAllROIs(vLocalSystemData->vITIPCDig->vProductAverageImage);

	vLocalSystemData->vITIPCDig->CalculateAverageDriftDensity(vGlobalCurrentProduct);
	if (vAverageDensity != vGlobalCurrentProduct->vSetupHistoryAverageDensity)
	{
		vAverageDensity = vGlobalCurrentProduct->vSetupHistoryAverageDensity;
		SetDlgItemText(IDC_AverageDensity, "Avg:" + dtoa(vAverageDensity, 1));
		m_AverageDensity.ShowWindow(SW_SHOW);
	}
}

void CAutoSetupImageDisplayDialog::CheckDensityCompensationInspection()
{
	if (!vGlobalCurrentProduct->vDriftDensityInspection)
	{
		vGlobalCurrentProduct->vDriftDensityInspection = new CInspectionDensity;

		if (vGlobalCurrentProduct->vDriftDensityInspection)
		{
			WORD TempTop = (WORD)(vLocalSystemData->vITIPCDig->vOriginalBufferSizeX * .95);
			WORD TempBottom = (WORD)(vLocalSystemData->vITIPCDig->vOriginalBufferSizeX * .05);
			WORD TempRight = (WORD)(vLocalSystemData->vITIPCDig->vAquisitionSizeY * .7);
			WORD TempLeft = (WORD)(vLocalSystemData->vITIPCDig->vAquisitionSizeY * .3);

			if ((vGlobalCurrentProduct->vReferenceRight) &&
				(vGlobalCurrentProduct->vReferenceRight <= vGlobalCurrentProduct->vImageWidthPosition))
			{
				TempTop = vGlobalCurrentProduct->vReferenceTop;
				TempBottom = vGlobalCurrentProduct->vReferenceBottom;
				TempRight = vGlobalCurrentProduct->vReferenceRight;
				TempLeft = vGlobalCurrentProduct->vReferenceLeft;
			}
			else
			if ((vLocalSystemData->vITIPCDig->vLastRight) && (vLocalSystemData->vITIPCDig->vLastRight <= vGlobalCurrentProduct->vImageWidthPosition))
			{
				TempTop = vLocalSystemData->vITIPCDig->vLastTop;
				TempBottom = vLocalSystemData->vITIPCDig->vLastBottom;
				TempRight = vLocalSystemData->vITIPCDig->vLastRight;
				TempLeft = vLocalSystemData->vITIPCDig->vLastLeft;
			}
			WORD TempQuarterWidth = 0;
			if (TempRight > TempLeft + 3)
				TempQuarterWidth = (TempRight - TempLeft) / 4;
			
			if (!vGlobalCurrentProduct->vMultiLaneNumberOfLanes)
			if ((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode) ||
				(vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vLocalConfigurationData->vScanTracType == cAllegroScanTrac) ||
				(vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac) || (vLocalConfigurationData->vScanTracType == cBriosoScanTrac)) //Continuous Feed, or a Pipeline or TD
			{//set ROIs for to whole screen 
				vGlobalCurrentProduct->vDriftDensityInspection->SetupInspectionInPixels((WORD)((double)vLocalSystemData->vITIPCDig->vOriginalBufferSizeX * 0.65), 
					(WORD)((double)vLocalSystemData->vITIPCDig->vOriginalBufferSizeX * 0.35), 0, 
					vLocalSystemData->vITIPCDig->vOriginalBufferSizeY, vGlobalCurrentProduct->vOverScanMultiplier);
			}
			else
				vGlobalCurrentProduct->vDriftDensityInspection->SetupInspectionInPixels(
					(WORD)((double)TempTop * 0.25), (WORD)((double)TempTop * 0.1), TempLeft + TempQuarterWidth, TempRight - TempQuarterWidth, vGlobalCurrentProduct->vOverScanMultiplier);
			ReportErrorMessage(vGlobalCurrentProduct->vDriftDensityInspection->vName + " ROI Invalid, Set ROI to default", cWriteToLog, 0);
		}
	}
}

void CAutoSetupImageDisplayDialog::CheckIfShouldLockDensityFeedbackLoop()
{
	if (vLocalConfigurationData->vDriftCompensationEnabled)
	if ((vLocalSystemData->vKeepDensityFeedbackLoopLocked) ||
		(!vLocalSystemData->vITIPCDig->vDriftCompensationLocked))
	if (((vMode == cLearnMenu) || (vMode == cSetImageSize) || 
		(vMode == cCenterExposureMenu)/* || (vMode == cEdgeExposureMenu)*/) && ((!vLocalConfigurationData->vEnableEjectors) ||
		(vLocalSystemData->vInAutoSetup)))
	if (vLocalSystemData->vInAutoSetup)//8/23/2010
	{
		vLocalSystemData->vKeepDensityFeedbackLoopLocked = false;
		vLocalSystemData->vITIPCDig->vDriftCompensationLocked = true;
		//vLocalSystemData->vDriftCompensationThresholdIncreaseAmount = 0;
		//vLocalSystemData->vDriftCompensationThresholdReductionAmount = 0;
		if ((vLocalSystemData->vDriftCompensationADCOffset) ||
			(vLocalSystemData->vDriftCompensationDACOffset))
			if (vGlobaluCSerialPort)
		{
			vLocalSystemData->vDriftCompensationADCOffset = 0;
			vLocalSystemData->vDriftCompensationDACOffset = 0;
			vMainWindowPointer->SendAllADCOffsets();
			vMainWindowPointer->SendAllDACDetectorOffsets();
		}
	}
}

void CAutoSetupImageDisplayDialog::ClearDensityFeedBackLoop()
{
	if ((vLocalSystemData->vDriftCompensationADCOffset) || (vLocalSystemData->vDriftCompensationDACOffset))
	if (vGlobaluCSerialPort)
	{
		vLocalSystemData->vDriftCompensationADCOffset = 0;
		vLocalSystemData->vDriftCompensationDACOffset = 0;
		vMainWindowPointer->SendAllADCOffsets();
		vMainWindowPointer->SendAllDACDetectorOffsets();
	}
	vLocalSystemData->vITIPCDig->vDriftCompensationLocked = true;
	//vLocalSystemData->vDriftCompensationThresholdIncreaseAmount = 0;
	//vLocalSystemData->vDriftCompensationThresholdReductionAmount = 0;
	vGlobalCurrentProduct->ClearInspectionHistory();
	vAverageDensity = 0;
	SetDlgItemText(IDC_AverageDensity, _T(" "));
}

void CAutoSetupImageDisplayDialog::TakeBackgroundImageSample()
{
	SetStatusLine(" Sampling Background, Please Wait...");
	vLocalSystemData->vITIPCDig->RenameBackgroundFileToBackup(vGlobalCurrentProduct);
	if (vLocalSystemData->vITIPCDig->vProductBackgroundImage != NULL)
	{
		im_delete(vLocalSystemData->vITIPCDig->vProductBackgroundImage);
		vLocalSystemData->vITIPCDig->vProductBackgroundImage = NULL;
	}
	vShowingBackground = false;
	//vWaitingForBackgroundImage = true;
	
	int TimerResult = SetTimer(vGetBackgroundImageTimerHandle,7,NULL);
	if (!TimerResult)
		ReportErrorMessage("Error-Get Background Timer Failed",cEMailInspx, 32000);

	if (vLocalSystemData->vRadiationDoorManualControlRegister == cRadiationShutterAutoControl)
	if (vGlobaluCSerialPort)
		vGlobaluCSerialPort->SetRadiationDoorManualControl(cRadiationShutterOpen);

	if (!vLocalSystemData->vSourceRampedUp)
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\n\nSource Not Ramped Up Yet!";
		TempNoticeDialog.vType = cInformationMessage;
		TempNoticeDialog.DoModal();
	}
	::PostThreadMessage(vGlobalCallingThreadID,cSimulateOneContainerIn10MSMessage,0,0);
}
/*
void CAutoSetupImageDisplayDialog::LoadBackGroundIfNeeded()
{
	if (vLocalSystemData->vInLowExposureMode)
	{//use size exposure product's background image if have one and update for size change
		CProduct *TempProduct = vMainWindowPointer->GetProductByName("~SizeExposure");
		if (TempProduct)
		{
			if (vLocalSystemData->vITIPCDig->ReadBackgroundImageFromFile(TempProduct))
			{
				vAskedIfShouldRemoveBackground = true;
				vLocalSystemData->vITIPCDig->MakeProductBackgroundImageCorrectionFactors();
				vLocalSystemData->vRemoveBackgroundInAutoSize = true;
				if (vGlobalCurrentProduct->vThresholdForContainerBounds > 240)
					vGlobalCurrentProduct->vThresholdForContainerBounds = 240;
			}
		}
	}
}
*/

void CAutoSetupImageDisplayDialog::CalculateContainerThresholdBounds()
{
	//find the brightest line and that is the background, Set threshold bounds 15 below
	if (vGlobalCurrentProduct->vMultiLaneNumberOfLanes)
	{
		WORD TempShortestLane = vLocalSystemData->vITIPCDig->vOriginalBufferSizeX;
		for (int TempLaneLoop = 0; TempLaneLoop < vGlobalCurrentProduct->vMultiLaneNumberOfLanes; TempLaneLoop++)
		if (vGlobalCurrentProduct->vMultiLaneWidthInPixels[TempLaneLoop] < TempShortestLane)
			TempShortestLane = vGlobalCurrentProduct->vMultiLaneWidthInPixels[TempLaneLoop];

		if ((vLocalSystemData->vITIPCDig->vOriginalImage) && (vLocalSystemData->vITIPCDig->vOriginalImage->vGreyImage))
		{
			//vGlobalCurrentProduct->vEdgeLocationBottomPixel = 4;
			//vGlobalCurrentProduct->vEdgeLocationHeightPixel = vLocalSystemData->vITIPCDig->vOriginalBufferSizeX - 8;
			if (vGlobalCurrentProduct->vEdgeLocationHeightPixel >= vLocalSystemData->vITIPCDig->vOriginalBufferSizeX)
				vGlobalCurrentProduct->vEdgeLocationHeightPixel = vLocalSystemData->vITIPCDig->vOriginalBufferSizeX - 1;
			if (vGlobalCurrentProduct->vEdgeLocationBottomPixel + vGlobalCurrentProduct->vEdgeLocationBottomPixel >= vLocalSystemData->vITIPCDig->vOriginalBufferSizeX)
				vGlobalCurrentProduct->vEdgeLocationBottomPixel = 0;

			BYTE TempBrightestValue = 0;
			BYTE TempEdgeAverage = 0;

			for (int TempLoopY = vGlobalCurrentProduct->vMultiLaneNumberOfLinesBeforeContainer + 2; TempLoopY < vLocalSystemData->vITIPCDig->vOriginalBufferSizeY - 3; TempLoopY++)
			{
				WORD TempCount = 0;
				double TempEdgeSum = 0;
				BYTE *TempPointer = vLocalSystemData->vITIPCDig->vOriginalImage->vGreyImage + (TempLoopY * vLocalSystemData->vITIPCDig->vOriginalBufferSizeX) + vGlobalCurrentProduct->vEdgeLocationBottomPixel;
				for (int TempLoop = 4; TempLoop < TempShortestLane; TempLoop++)
				{
					TempEdgeSum = TempEdgeSum + *TempPointer++;
					TempCount++;
				}
				if (TempCount)
				{
					TempEdgeAverage = (BYTE)((TempEdgeSum / (double)TempCount) + .5);
					if (TempEdgeAverage > TempBrightestValue)
						TempBrightestValue = TempEdgeAverage;
				}
			}
			if (TempBrightestValue > 30)
			{
				if (TempBrightestValue > 253)
					vGlobalCurrentProduct->vThresholdForContainerBounds = TempBrightestValue - 15;
				else
					vGlobalCurrentProduct->vThresholdForContainerBounds = TempBrightestValue - 25;
			}
			else
				vGlobalCurrentProduct->vThresholdForContainerBounds = 15;

			// debug only
			//ReportErrorMessage("Calcualte Container Threshold Brightest Line: " + dtoa(TempBrightestValue, 0), cError, 0);
			//ReportErrorMessage("Calculate Multi LaneThreshold Bounds: " + dtoa(vGlobalCurrentProduct->vThresholdForContainerBounds, 0), cErrorMessage, 0);

			if (vMode == cSetSizeSettings)
				SetDlgItemText(IDC_SubFunction4DisplayA, dtoa(vGlobalCurrentProduct->vThresholdForContainerBounds,0));
		}
	}
	else
	if (((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode)) && 
		(!vGlobalCurrentProduct->vMultiLaneNumberOfLanes)) //Continuous Feed, or a Pipeline
	{
		vGlobalCurrentProduct->vThresholdForContainerBounds = 240;
		//ReportErrorMessage("Set Threshold Bounds: 240", cErrorMessage, 0);
	}
	else
	if ((vLocalSystemData->vITIPCDig->vOriginalImage) && (vLocalSystemData->vITIPCDig->vOriginalImage->vGreyImage))
	{
		if (vGlobalCurrentProduct->vEdgeLocationHeightPixel >= vLocalSystemData->vITIPCDig->vOriginalBufferSizeX)
			vGlobalCurrentProduct->vEdgeLocationHeightPixel = vLocalSystemData->vITIPCDig->vOriginalBufferSizeX - 1;

		BYTE TempBrightestValue = 0;
		BYTE TempEdgeAverage = 0;

		for (int TempLoopY = 0; TempLoopY < vLocalSystemData->vITIPCDig->vOriginalBufferSizeY; TempLoopY++)
		{
			WORD TempCount = 0;
			double TempEdgeSum = 0;
			BYTE *TempPointer = vLocalSystemData->vITIPCDig->vOriginalImage->vGreyImage + (TempLoopY * vLocalSystemData->vITIPCDig->vOriginalBufferSizeX) + vGlobalCurrentProduct->vEdgeLocationBottomPixel;
			for (int TempLoop = 0; TempLoop < vGlobalCurrentProduct->vEdgeLocationHeightPixel; TempLoop++)
			{
				TempEdgeSum = TempEdgeSum + *TempPointer++;
				TempCount++;
			}
			if (TempCount)
			{
				TempEdgeAverage = (BYTE)((TempEdgeSum / (double)TempCount) + .5);
				if (TempEdgeAverage > TempBrightestValue)
					TempBrightestValue = TempEdgeAverage;
			}
		}
		if (TempBrightestValue > 30)
		{
			if (TempBrightestValue > 253)
				vGlobalCurrentProduct->vThresholdForContainerBounds = TempBrightestValue - 15;
			else
				vGlobalCurrentProduct->vThresholdForContainerBounds = TempBrightestValue - 25;
		}
		else
			vGlobalCurrentProduct->vThresholdForContainerBounds = 15;

		// debug
		//ReportErrorMessage("Calculate Threshold Bounds: " + dtoa(vGlobalCurrentProduct->vThresholdForContainerBounds, 0), cErrorMessage, 0);
		//ReportErrorMessage("Calcualte Container Threshold Brightest Line: " + dtoa(TempBrightestValue, 0), cError, 0);

		if (vMode == cSetSizeSettings)
			SetDlgItemText(IDC_SubFunction4DisplayA, dtoa(vGlobalCurrentProduct->vThresholdForContainerBounds,0));
	}
}

void CAutoSetupImageDisplayDialog::CheckPurpleLineHeight()
{
	double TempNewHeight = vGlobalCurrentProduct->vProductImageHeightTop - vGlobalCurrentProduct->vProductImageHeightBottom;
	if ((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode) ||
		(vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vLocalConfigurationData->vScanTracType == cAllegroScanTrac) ||
		(vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac) || (vLocalConfigurationData->vScanTracType == cBriosoScanTrac)) //Continuous Feed, or a Pipeline or TD
	{ //set purple line automatically on pipeline if change top or bottom
		if (vGlobalCurrentProduct->vEdgeLocationBottom + vGlobalCurrentProduct->vEdgeLocationHeight > TempNewHeight)
		{
			vGlobalCurrentProduct->SetEdgeLocationBottom((float)(TempNewHeight * .20));
			vGlobalCurrentProduct->SetEdgeLocationHeight((float)((TempNewHeight - vGlobalCurrentProduct->vEdgeLocationBottom) * .80));
		}
		vNeedToSetContainerThresholdBounds = 2;
	}
	else
	if (TempNewHeight < vGlobalCurrentProduct->vEdgeLocationHeight + vGlobalCurrentProduct->vEdgeLocationBottom)
	{  //set to default if above top
		vGlobalCurrentProduct->SetEdgeLocationBottom((float)(TempNewHeight * .10));
		vGlobalCurrentProduct->SetEdgeLocationHeight((float)((TempNewHeight - vGlobalCurrentProduct->vEdgeLocationBottom) * .50));
		vNeedToSetContainerThresholdBounds = 2;
	}
}

//void CAutoSetupImageDisplayDialog::SetDefaultCheckPurpleLineHeight()
//{
//	double TempNewHeight = vGlobalCurrentProduct->vProductImageHeightTop - vGlobalCurrentProduct->vProductImageHeightBottom;
//	if ((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode) ||
//		(vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vLocalConfigurationData->vScanTracType == cAllegroScanTrac) ||
//		(vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac) || (vLocalConfigurationData->vScanTracType == cBriosoScanTrac)) //Continuous Feed, or a Pipeline or TD
//	{ //set purple line automatically on pipeline if change top or bottom
//		vGlobalCurrentProduct->SetEdgeLocationBottom((float)(TempNewHeight * .20));
//		vGlobalCurrentProduct->SetEdgeLocationHeight((float)((TempNewHeight - vGlobalCurrentProduct->vEdgeLocationBottom) * .80));
//	}
//	else
//	if (TempNewHeight < vGlobalCurrentProduct->vEdgeLocationHeight + vGlobalCurrentProduct->vEdgeLocationBottom)
//	{
//		if (vGlobalCurrentProduct->vProductImageHeightTop - vGlobalCurrentProduct->vProductImageHeightBottom < vGlobalCurrentProduct->vEdgeLocationHeight)
//		{ //if the purple line is taller than the image reset it completely
//			vGlobalCurrentProduct->SetEdgeLocationBottom((float)(TempNewHeight * .10));
//			vGlobalCurrentProduct->SetEdgeLocationHeight((float)((TempNewHeight - vGlobalCurrentProduct->vEdgeLocationBottom) * .60));
//		}
//		else
//		{ //purple line is shorter than image, so just lower so it is in the image
//			vGlobalCurrentProduct->SetEdgeLocationBottom((float)(vGlobalCurrentProduct->vProductImageHeightTop - 
//				vGlobalCurrentProduct->vProductImageHeightBottom - vGlobalCurrentProduct->vEdgeLocationHeight));
//		}
//
//		vNeedToSetContainerThresholdBounds = 2;
//	}
//}

//void CAutoSetupImageDisplayDialog::OnMouseMove(UINT nFlags, CPoint point) 
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

void CAutoSetupImageDisplayDialog::OnDialogTitleStaticText() 
{
	//if (vGlobalUnderRemoteControl)
	//	vGlobalUnderRemoteControl = 0;
	//else
	//	vGlobalUnderRemoteControl = 600;
}

void CAutoSetupImageDisplayDialog::DisplayDensityThresholdUpper()
{
	CString TempString = "";
	if (vInspectionEditing)
	{
		CString TempAbreviation = "";
		if ((vInspectionEditing->vInspectionType == cUnderfillByWeightInspection) || (vInspectionEditing->vInspectionType == cCheckWeighInspection))
		{
			if (vInspectionEditing->vCheckWeighUnits == "Ounces")
				TempAbreviation = "oz";
			else
				TempAbreviation = "g";
			if (vInspectionEditing->vDensityThresholdUpper)
				TempString = dtoa(vInspectionEditing->ConvertToWeight(vInspectionEditing->vDensityThresholdUpper),2) + TempAbreviation;
			else
				TempString = "0";
			SetDlgItemText(IDC_SubFunction2Display,TempString);
		}
		else
		if (vInspectionEditing->vInspectionType != cOverfillByWeightInspection)
			SetDlgItemText(IDC_SubFunction2Display, dtoa(vInspectionEditing->vDensityThresholdUpper,2));
	}
}

void CAutoSetupImageDisplayDialog::DisplayDensityThresholdLower()
{
	CString TempString = "";
	if (vInspectionEditing)
	{
		if ((vInspectionEditing->vInspectionType == cOverfillByWeightInspection) || (vInspectionEditing->vInspectionType == cCheckWeighInspection))
		{
			CString TempAbreviation = "";
			if (vInspectionEditing->vCheckWeighUnits == "Ounces")
				TempAbreviation = "oz";
			else
				TempAbreviation = "g";

			if (vInspectionEditing->vDensityThresholdLower)
				TempString = dtoa(vInspectionEditing->ConvertToWeight(vInspectionEditing->vDensityThresholdLower),2) + TempAbreviation;
			else
				TempString = "0";

			SetDlgItemText(IDC_SubFunction3Display, TempString);
		}
		else
		if (vInspectionEditing->vInspectionType != cUnderfillByWeightInspection)
			SetDlgItemText(IDC_SubFunction3Display,dtoa(vInspectionEditing->vDensityThresholdLower,2));
	}
}

void CAutoSetupImageDisplayDialog::DisplayAutoDensityThresholdUpper()
{
	if (vInspectionEditing)
	if ((vInspectionEditing->vInspectionType != cCheckWeighInspection) && (vInspectionEditing->vInspectionType != cUnderfillByWeightInspection) && (vInspectionEditing->vInspectionType != cOverfillByWeightInspection))
	{
		CString TempString = dtoa(vInspectionEditing->ConvertToWeight(vAutomaticDensityThresholdUpper),2);
		CString TempText;
		if ((vInspectionEditing) && (vInspectionEditing->vHasDensityLowerThreshold))
			TempText.LoadString(IDS_AutoU);
		else
			TempText.LoadString(IDS_Auto);
		TempString = TempText + ": " + TempString;
		SetDlgItemText(IDC_SubFunction4Display,TempString);
	}
}

void CAutoSetupImageDisplayDialog::DisplayAutoDensityThresholdLower()
{
	if (vInspectionEditing)
	if ((vInspectionEditing->vInspectionType != cCheckWeighInspection) && (vInspectionEditing->vInspectionType != cUnderfillByWeightInspection) && (vInspectionEditing->vInspectionType != cOverfillByWeightInspection))
	{
		CString TempString = dtoa(vAutomaticDensityThresholdLower,2);
		CString TempText;
		if (vInspectionEditing->vHasDensityUpperThreshold)
			TempText.LoadString(IDS_AutoL);
		else
			TempText.LoadString(IDS_Auto);
		TempString = TempText + ":" + TempString;
		SetDlgItemText(IDC_SubFunction4DisplayA,TempString);
	}
}

void CAutoSetupImageDisplayDialog::DisplayMinimumGoodMaximumBadDataValues(bool TempAlreadyUpdatedStatusLine)
{
	CString TempText;
	CString TempString;
	if (vInspectionEditing)
	if ((!vLocalSystemData->vITIPCDig->vDriftCompensationLocked) && (vInspectionEditing->vCollectingDensityAverage))  //if collecting average and brightness loop not locked
	{
		SetStatusLine(" Waiting for lock, Send Good Containers...");
		SetDlgItemText(IDC_SubFunction6Button3, _T("Evaluating"));
	}
	else
	if (((vInspectionEditing->vInspectionType == cCheckWeighInspection) || (vInspectionEditing->vInspectionType == cUnderfillByWeightInspection) || (vInspectionEditing->vInspectionType == cOverfillByWeightInspection)) &&
		((vInspectionEditing->vCheckWeighMFactor) && (vInspectionEditing->vCheckWeighMFactor != 1) && (vInspectionEditing->vCheckWeighBFactor)))
	{  //Weight Trending inspections only
		double TempFactor = vInspectionEditing->vCheckWeighMFactor;
		if (TempFactor < 0)
			TempFactor = -TempFactor;

		if (vInspectionEditing->vSetupHistoryMinimumGoodCount)
		{  //evaluating density
			//Minimum good sample data
			if ((vInspectionEditing->vInspectionType == cUnderfillByWeightInspection) || (vInspectionEditing->vInspectionType == cOverfillByWeightInspection) || (vInspectionEditing->vInspectionType == cCheckWeighInspection))
				SetDlgItemText(IDC_SubFunction5Display, "Min: " + dtoa(vInspectionEditing->ConvertToWeight(vInspectionEditing->vSetupHistoryMinimumValue),2));
			else
				SetDlgItemText(IDC_SubFunction5Display, "Max: " + dtoa(vInspectionEditing->ConvertToWeight(vInspectionEditing->vSetupHistoryMinimumValue),2));

			if ((vInspectionEditing->vInspectionType == cUnderfillByWeightInspection) || (vInspectionEditing->vInspectionType == cOverfillByWeightInspection) || (vInspectionEditing->vInspectionType == cCheckWeighInspection))
				SetDlgItemText(IDC_SubFunction6Button3, "Max: " + dtoa(vInspectionEditing->ConvertToWeight(vInspectionEditing->vSetupHistoryMaximumValue),2));
			else
				SetDlgItemText(IDC_SubFunction6Button3, "Min: " + dtoa(vInspectionEditing->ConvertToWeight(vInspectionEditing->vSetupHistoryMaximumValue),2));

			if (!vOldDensityAverageValue)
			if (vInspectionEditing->vSetupHistoryMinimumGoodAverage)
				UpdateButtonText();

			TempText.LoadString(IDS_Avg);
			TempString = dtoa(vInspectionEditing->ConvertToWeight(vInspectionEditing->vSetupHistoryMinimumGoodAverage),2);
			SetDlgItemText(IDC_SubFunction6Button1,TempText + ": " + TempString);
			vOldDensityAverageValue = vInspectionEditing->vSetupHistoryMinimumGoodAverage;

			TempText.LoadString(IDS_SD);
			TempString = dtoa((vInspectionEditing->vSetupHistoryMinimumGoodStandardDeviation * TempFactor),2);
			SetDlgItemText(IDC_SubFunction6Button2,TempText + ": " + TempString);
		}

		if (vInspectionEditing->vSetupHistoryMaximumBadCount)
		{  //evaluating density
			//display min, max, ave and Standard Deviation of Maximum samples
			if ((vInspectionEditing->vInspectionType == cUnderfillByWeightInspection) || (vInspectionEditing->vInspectionType == cOverfillByWeightInspection) || (vInspectionEditing->vInspectionType == cCheckWeighInspection))
				SetDlgItemText(IDC_SubFunction7Display, "Min: " + dtoa(vInspectionEditing->ConvertToWeight(vInspectionEditing->vSetupHistoryBadMinimumValue),2));
			else
				SetDlgItemText(IDC_SubFunction7Display, "Max: " + dtoa(vInspectionEditing->ConvertToWeight(vInspectionEditing->vSetupHistoryBadMinimumValue),2));

			if ((vInspectionEditing->vInspectionType == cUnderfillByWeightInspection) || (vInspectionEditing->vInspectionType == cOverfillByWeightInspection) || (vInspectionEditing->vInspectionType == cCheckWeighInspection))
				SetDlgItemText(IDC_SubFunction8Button3, "Max: " + dtoa(vInspectionEditing->ConvertToWeight(vInspectionEditing->vSetupHistoryBadMaximumValue),2));
			else
				SetDlgItemText(IDC_SubFunction8Button3, "Min: " + dtoa(vInspectionEditing->ConvertToWeight(vInspectionEditing->vSetupHistoryBadMaximumValue),2));

			if (!vOldDensityAverageValue)
			if (vInspectionEditing->vSetupHistoryMaximumBadAverage)
				UpdateButtonText();

			TempText.LoadString(IDS_Avg);
			TempString = dtoa(vInspectionEditing->ConvertToWeight(vInspectionEditing->vSetupHistoryMaximumBadAverage),2);

			SetDlgItemText(IDC_SubFunction8Button1,TempText + ": " + TempString);

			vOldDensityAverageValue = vInspectionEditing->vSetupHistoryMaximumBadAverage;

			TempText.LoadString(IDS_SD);
			TempString = dtoa((vInspectionEditing->vSetupHistoryMaximumBadStandardDeviation * TempFactor),2);
			SetDlgItemText(IDC_SubFunction8Button2,TempText + ": " + TempString);
		}
		else
		{
			SetDlgItemText(IDC_SubFunction7Display, "Min: " + dtoa(vInspectionEditing->ConvertToWeight(vInspectionEditing->vSetupHistoryBadMinimumValue),2));
			SetDlgItemText(IDC_SubFunction8Button1, _T(" "));
			SetDlgItemText(IDC_SubFunction8Button2, _T(" "));
			SetDlgItemText(IDC_SubFunction8Button3, _T(" "));
		}
	}
	else //not a Weight Trending inspection  or Weight Trending without a scale factor yet
	{//don't have scale factors yet for Weight Trending
		if ((vInspectionEditing) && ((vInspectionEditing->vInspectionType == cCheckWeighInspection) || (vInspectionEditing->vInspectionType == cUnderfillByWeightInspection) || 
			(vInspectionEditing->vInspectionType == cOverfillByWeightInspection)))
		{
			
			if (vInspectionEditing->vCollectingDensityAverage)
			{
				TempText.LoadString(IDS_CollectingWeightCalibrationSamples);
				if (!TempAlreadyUpdatedStatusLine)
				if (vLocalSystemData->vITIPCDig->vDriftCompensationLocked)
					SetStatusLine(" " + TempText + ": " + dtoa(vLocalSystemData->vDensityIndex,0));
			}
			else
			if (!TempAlreadyUpdatedStatusLine)
			{
				if (vLocalSystemData->vITIPCDig->vDriftCompensationLocked)
				{
					if (vGlobalCurrentProduct->vMultiLaneNumberOfLanes)
						SetStatusLine(" Lane: " + dtoa(vLocalSystemData->vITIPCDig->vOriginalImage->vMultiLaneLaneNumber, 0) + " Product: " + vProductEditingName);
					else
						SetStatusLine(" " + vProductEditingName);
				}
			}
			else
			{
				SetDlgItemText(IDC_SubFunction5Display, _T("Min: --"));
				SetDlgItemText(IDC_SubFunction6Button1, _T(" "));
				SetDlgItemText(IDC_SubFunction6Button2, _T(" "));
				SetDlgItemText(IDC_SubFunction6Button3, _T(" "));
				SetDlgItemText(IDC_SubFunction7Display, _T("Min: --"));
				SetDlgItemText(IDC_SubFunction8Button1, _T(" "));
				SetDlgItemText(IDC_SubFunction8Button2, _T(" "));
				SetDlgItemText(IDC_SubFunction8Button3, _T(" "));
			}
		}
		else 
		{ //not a Weight Trending inspection
			if ((vInspectionEditing) && (vInspectionEditing->vCollectingDensityAverage))
			{
				if (vCollectingBadDensityAverage)
				{
					TempText.LoadString(IDS_CollectingBadDensitySamples);
					if (!TempAlreadyUpdatedStatusLine)
						SetStatusLine(" " + TempText + ": " + dtoa(vLocalSystemData->vDensityIndex,0));
				}
				else
				{
					//show status line with count of how many evaluated to far
					TempText.LoadString(IDS_CollectingDensitySamples);
					if (!TempAlreadyUpdatedStatusLine)
						SetStatusLine(" " + TempText + ": " + dtoa(vLocalSystemData->vDensityIndex,0));
				}
			}
			//else
			//if (!TempAlreadyUpdatedStatusLine)
			//	SetStatusLine(" " + vProductEditingName);
		}

		if (vLocalSystemData->vITIPCDig->vDensityMaximumValue)
		{  //evaluating density
			CString TempText;
			CString TempString;
			//if (!vCollectingBadDensityAverage)
			//show good samples
			if (vInspectionEditing->vSetupHistoryMinimumGoodCount)
			{
				TempText.LoadString(IDS_Min);
				TempString = dtoa(vInspectionEditing->ConvertToWeight(vInspectionEditing->vSetupHistoryMinimumValue),2);
				SetDlgItemText(IDC_SubFunction5Display,TempText + ": " + TempString);

				if ((vInspectionEditing->vCheckWeighMFactor != 1) || (vInspectionEditing->vCheckWeighBFactor))
					SetDlgItemText(IDC_SubFunction6Button3, "Min: " + dtoa(vInspectionEditing->ConvertToWeight(vInspectionEditing->vSetupHistoryMaximumValue),2));
				else
					SetDlgItemText(IDC_SubFunction6Button3, "Max: " + dtoa(vInspectionEditing->ConvertToWeight(vInspectionEditing->vSetupHistoryMaximumValue),2));

				if (!vOldDensityAverageValue)
				if (vInspectionEditing->vSetupHistoryMinimumGoodAverage)
					UpdateButtonText();
				TempText.LoadString(IDS_Avg);

				TempString = dtoa(vInspectionEditing->ConvertToWeight(vInspectionEditing->vSetupHistoryMinimumGoodAverage),2);

				SetDlgItemText(IDC_SubFunction6Button1,TempText + ": " + TempString);
				vOldDensityAverageValue = vInspectionEditing->vSetupHistoryMinimumGoodAverage;

				TempText.LoadString(IDS_SD);
				TempString = dtoa(vInspectionEditing->ConvertToWeight(vInspectionEditing->vSetupHistoryMinimumGoodStandardDeviation),2);
				SetDlgItemText(IDC_SubFunction6Button2,TempText + ": " + TempString);
			}

			//show bad samples
			//display min, max, ave and Standard Deviation
			if (vInspectionEditing->vSetupHistoryMaximumBadCount)
			{
				if ((vInspectionEditing->vCheckWeighMFactor != 1) || (vInspectionEditing->vCheckWeighBFactor))
					SetDlgItemText(IDC_SubFunction7Display, "Max: " + dtoa(vInspectionEditing->ConvertToWeight(vInspectionEditing->vSetupHistoryBadMinimumValue),2));
				else
					SetDlgItemText(IDC_SubFunction7Display, "Min: " + dtoa(vInspectionEditing->ConvertToWeight(vInspectionEditing->vSetupHistoryBadMinimumValue),2));

				if ((vInspectionEditing->vCheckWeighMFactor != 1) || (vInspectionEditing->vCheckWeighBFactor))
					SetDlgItemText(IDC_SubFunction8Button3, "Min: " + dtoa(vInspectionEditing->ConvertToWeight(vInspectionEditing->vSetupHistoryBadMaximumValue),2));
				else
					SetDlgItemText(IDC_SubFunction8Button3, "Max: " + dtoa(vInspectionEditing->ConvertToWeight(vInspectionEditing->vSetupHistoryBadMaximumValue),2));

				if (!vOldDensityAverageValue)
				if (vInspectionEditing->vSetupHistoryMaximumBadAverage)
					UpdateButtonText();

				TempText.LoadString(IDS_Avg);

				TempString = dtoa(vInspectionEditing->ConvertToWeight(vInspectionEditing->vSetupHistoryMaximumBadAverage),2);

				SetDlgItemText(IDC_SubFunction8Button1,TempText + ": " + TempString);
				vOldDensityAverageValue = vInspectionEditing->vSetupHistoryMaximumBadAverage;

				TempText.LoadString(IDS_SD);

				TempString = dtoa(vInspectionEditing->vSetupHistoryMaximumBadStandardDeviation, 2);
				if ((vInspectionEditing->vCheckWeighMFactor) && (vInspectionEditing->vCheckWeighMFactor != 1))
					TempString = dtoa(vInspectionEditing->vSetupHistoryMaximumBadStandardDeviation * vInspectionEditing->vCheckWeighMFactor, 6);

				SetDlgItemText(IDC_SubFunction8Button2,TempText + ": " + TempString);
			}
		}
	}
}

void CAutoSetupImageDisplayDialog::StartLearning()
{
	CString TempText("");
	CString TempText1("");
	//if quick learn is setup 
	if (vGlobalProcessQuickLearnImagesThread)
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\nSaving Last Quick Learn\nTry again in a few seconds";
		TempNoticeDialog.vType = cErrorMessage;
		TempNoticeDialog.DoModal();
	}
	else
	if ((vLocalConfigurationData->vQuickLearnNumberOfStructureDeltas) &&
		(vLocalConfigurationData->vNumberOfQuickLearnImagesToTake))
	{
		//do quick learn
		if (ThereIsEnoughMemory(10000000 + vLocalSystemData->vITIPCDig->vOriginalBufferSize *
			((vLocalConfigurationData->vNumberOfQuickLearnImagesToTake * 2) + 
			vLocalConfigurationData->vNumberOfLastRejectImages), 
			"Quick Learn Images"))
		{
			vLocalSystemData->vIncludeRejectsInAverageAndStructure = true;
			if (!vLocalSystemData->vInAutoSetup)
			{
				CYesNoDialog TempYesNoDialog;
				TempText.LoadString(IDS_IncludeRejectsinLearnedImage);//"\n\n\nInclude Rejects in Learned Image?"
				TempYesNoDialog.vNoticeText = TempText;
				TempYesNoDialog.vQuestionType = cConfirmQuestion;
				TempText1.LoadString(IDS_IncludeRejects);
				TempYesNoDialog.vYesButtonText = TempText1;
				TempText1.LoadString(IDS_ExcludeRejects);
				TempYesNoDialog.vNoButtonText = TempText1;
				int TempResult = TempYesNoDialog.DoModal();
				if (TempResult != IDOK)
					vLocalSystemData->vIncludeRejectsInAverageAndStructure = false;

				//9/20/2010
				if (vLocalSystemData->vIncludeRejectsInAverageAndStructure)
				{
					if (vLocalSystemData->vPercentRejected < 1)
					{
						CYesNoDialog TempYesNoDialog;
						CString TempText = "Inspection is good with only " + dtoa(vLocalSystemData->vPercentRejected, 3) + " percent rejected.\nIt is better to exclude real rejects from the Learn.\nDo you want to Exclude Rejects from the Learn?";
						TempYesNoDialog.vNoticeText = TempText;
						TempYesNoDialog.vQuestionType = cConfirmQuestion;
						TempText1.LoadString(IDS_ExcludeRejects);
						TempYesNoDialog.vYesButtonText = TempText1;
						TempText1.LoadString(IDS_IncludeRejects);
						TempYesNoDialog.vNoButtonText = TempText1;
						int TempResult = TempYesNoDialog.DoModal();
						if (TempResult == IDOK)
							vLocalSystemData->vIncludeRejectsInAverageAndStructure = false;
					}
				}
				else
				{
					if (vLocalSystemData->vPercentRejected > 2)
					{
						CYesNoDialog TempYesNoDialog;
						CString TempText = "Reject Rate is high: " + dtoa(vLocalSystemData->vPercentRejected, 3) + " percent rejected.\nYou should Include the Rejects if many are false.\nDo you want to Include the Rejects in the Learn?";
						TempYesNoDialog.vNoticeText = TempText;
						TempYesNoDialog.vQuestionType = cConfirmQuestion;
						TempText1.LoadString(IDS_IncludeRejects);
						TempYesNoDialog.vYesButtonText = TempText1;
						TempText1.LoadString(IDS_ExcludeRejects);
						TempYesNoDialog.vNoButtonText = TempText1;
						int TempResult = TempYesNoDialog.DoModal();
						if (TempResult == IDOK)
							vLocalSystemData->vIncludeRejectsInAverageAndStructure = true;
					}
				}
			}
			vLocalSystemData->FreeQuickLearnImages();
			vLocalSystemData->vWrongWidthsDuringLearn = 0;
			vLocalSystemData->vRejectsDuringLearn = 0;
			vLocalSystemData->vWrongBrightnessDuringLearn = 0;

			//keep the reference as it is likely good, and if too far off will update
			//if (vLocalSystemData->vInAutoSetup)
			//{
			//	ClearReferenceLocationData();
			//}

			ReportErrorMessage("Start Quick Learn - Setup", cUserAction,0);
			vLocalSystemData->vTakingQuickLearnImages = true;
			vOldTakingQuickLearnImages = true;
			vTotalToLearn = vLocalConfigurationData->vNumberOfQuickLearnImagesToTake;
			UpdateButtonText();
			ReportErrorMessage("Start Taking Quick Learn Images", cAction,0);
		}
		else
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\nNot Enough Memory Available for Quick Learn\nTry Normal Learn";
			TempNoticeDialog.vType = cErrorMessage;
			TempNoticeDialog.DoModal();
		}
	}
	else
	//start normal learn
	{
		vLocalSystemData->vIncludeRejectsInAverageAndStructure = true;
		if (!vLocalSystemData->vInAutoSetup)
		{
			CYesNoDialog TempYesNoDialog;
			TempText.LoadString(IDS_IncludeRejectsinLearnedImage);//"\n\n\nInclude Rejects in Learned Image?"
			TempYesNoDialog.vNoticeText = TempText;
			TempYesNoDialog.vQuestionType = cConfirmQuestion;
			TempText1.LoadString(IDS_IncludeRejects);
			TempYesNoDialog.vYesButtonText = TempText1;
			TempText1.LoadString(IDS_ExcludeRejects);
			TempYesNoDialog.vNoButtonText = TempText1;
			int TempResult = TempYesNoDialog.DoModal();
			if (TempResult != IDOK)
				vLocalSystemData->vIncludeRejectsInAverageAndStructure = false;

			//9/20/2010
			if (vLocalSystemData->vIncludeRejectsInAverageAndStructure)
			{
				if (vLocalSystemData->vPercentRejected < 1)
				{
					CYesNoDialog TempYesNoDialog;
					CString TempText = "Inspection is good with only " + dtoa(vLocalSystemData->vPercentRejected, 3) + " percent rejected.\nIt is better to exclude real rejects from the Learn.\nDo you want to Exclude Rejects from the Learn?";
					TempYesNoDialog.vNoticeText = TempText;
					TempYesNoDialog.vQuestionType = cConfirmQuestion;
					TempText1.LoadString(IDS_ExcludeRejects);
					TempYesNoDialog.vYesButtonText = TempText1;
					TempText1.LoadString(IDS_IncludeRejects);
					TempYesNoDialog.vNoButtonText = TempText1;
					int TempResult = TempYesNoDialog.DoModal();
					if (TempResult == IDOK)
						vLocalSystemData->vIncludeRejectsInAverageAndStructure = false;
				}
			}
			else
			{
				if (vLocalSystemData->vPercentRejected > 2)
				{
					CYesNoDialog TempYesNoDialog;
					CString TempText = "Reject Rate is high: " + dtoa(vLocalSystemData->vPercentRejected, 3) + " percent rejected.\nYou should Include the Rejects if many are false.\nDo you want to Include the Rejects in the Learn?";
					TempYesNoDialog.vNoticeText = TempText;
					TempYesNoDialog.vQuestionType = cConfirmQuestion;
					TempText1.LoadString(IDS_IncludeRejects);
					TempYesNoDialog.vYesButtonText = TempText1;
					TempText1.LoadString(IDS_ExcludeRejects);
					TempYesNoDialog.vNoButtonText = TempText1;
					int TempResult = TempYesNoDialog.DoModal();
					if (TempResult == IDOK)
						vLocalSystemData->vIncludeRejectsInAverageAndStructure = true;
				}
			}
		}

		if (vLocalSystemData->vInAutoSetup)
		if (!vShowReturnToEditMenu)
			vMainWindowPointer->ResetCounters();
		if (PasswordOK(cTemporaryInspxPassword,false))
		{
			SetDlgItemText(IDC_Width2, _T("Showing"));
			m_Width2.ShowWindow(SW_SHOW);
			m_Reference.ShowWindow(SW_SHOW);
		}
		//don't find new reference location if you have one and are inspecting
		if ((vLocalConfigurationData->vCalibrateBodyTriggerReferenceDisableCount) && ((vLocalSystemData->vInAutoSetup) ||
			(!vLocalSystemData->vITIPCDig->HaveLearnImageFile("=Process 2")) || (!vGlobalCurrentProduct->vReferenceRight) ||
			(!vGlobalCurrentProduct->vReferenceTop)))
		{
			//vLocalConfigurationData->vReferenceImagePosition = 1;

			vLocalSystemData->vITIPCDig->vReferenceRightStandardDeviation = 0;
			vLocalSystemData->vITIPCDig->vReferenceRightAverage = 0;
			vLocalSystemData->vITIPCDig->vReferenceLeftStandardDeviation = 0;
			vLocalSystemData->vITIPCDig->vReferenceLeftAverage = 0;
			vLocalSystemData->vITIPCDig->vReferenceTopAverage = 0;
			vLocalSystemData->vITIPCDig->vReferenceTopStandardDeviation = 0;
			vLocalSystemData->vITIPCDig->vReferenceBottomAverage = 0;
			vLocalSystemData->vITIPCDig->vReferenceBottomStandardDeviation = 0;

			vOldMaximumBadIntensity = 0;
			vOldMaximumBadCount = 0;
			vOldMaximumBadStandardDeviation = 0;
			vOldMaximumBadAverage = 0;

			vOldMinimumGoodIntensity = 0;
			vOldMinimumGoodAverage = 0;
			vOldMinimumGoodStandardDeviation = 0;

			if (vLocalSystemData->vInAutoSetup)
			{
				vGlobalCurrentProduct->vSetupHistoryAverageDensity = 0;
				vAverageDensity = 0;
				SetDlgItemText(IDC_AverageDensity, _T(" "));
				ClearDensityFeedBackLoop();
			}
			vLocalSystemData->vFrameCount = 0;
			vLocalSystemData->vITIPCDig->vCalibrateBodyTriggerReferenceCount = 0;
			vLocalSystemData->vITIPCDig->vCalibrateBodyTriggerReferenceEnabled = true;
			ReportErrorMessage("Start Standard Learn", cAction,0);
			vOldCalibrateBodyTriggerReferenceEnabled = true;
			CString TempText("");
			if ((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode))
				TempText = "Send Good Product";
			else
				TempText.LoadString(IDS_SendGoodContainers);
			CString TempText1("");
			TempText1.LoadString(IDS_Product);

			SetStatusLine(TempText + ", " + TempText1 + ": " + vProductEditingName);
			if (PasswordOK(cTemporaryInspxPassword,false))
			{
				vShowAverage = false;
				vShowStructure = false;
				vShowVoidStructure = false;
				//vShowGlassStructure = false;
				vShowContaminant2Structure = false;
				vShowHistogram = false;
			}
		}
		else
		if (vLocalConfigurationData->vAverageDisableCount)
		{
			//vLocalSystemData->vITIPCDig->RenameAverageFileToBackup(vGlobalCurrentProduct);
			vLocalSystemData->vITIPCDig->MakeAverageImageBuffer();
			vLocalSystemData->vWrongWidthsDuringLearn = 0;
			vLocalSystemData->vWrongBrightnessDuringLearn = 0;
			vLocalSystemData->vRejectsDuringLearn = 0;
			StartAccumulatingAverage();
			vLocalSystemData->vITIPCDig->vAverageEnabled = true;
			vMainWindowPointer->vOldAverageEnabled = true;
		}
		else
		if (vLocalConfigurationData->vStructureDisableCount)
		{
			StartAccumulatingStructure();
			vLocalSystemData->vITIPCDig->vStructureCount = 0;

			if (vLocalSystemData->vInAutoSetup)
			{
				vLocalSystemData->vITIPCDig->RenameStructureFilesToBackup(vGlobalCurrentProduct);
			}
			vLocalSystemData->vITIPCDig->MakeStructureImageBuffer();
			/*
			if (vLocalSystemData->vITIPCDig->vProductGlassHistogramImage != NULL)
			{
				delete vLocalSystemData->vITIPCDig->vProductGlassHistogramImage;
				vLocalSystemData->vITIPCDig->vProductGlassHistogramImage = NULL;
			}
			vLocalSystemData->vITIPCDig->CreateProductGlassStructureImage();
			*/
			vLocalSystemData->vWrongWidthsDuringLearn = 0;
			vLocalSystemData->vRejectsDuringLearn = 0;
			vLocalSystemData->vWrongBrightnessDuringLearn = 0;

			vLocalSystemData->vITIPCDig->vStructureEnabled = true;
			vOldStructureEnabled = true;
			//vLocalSystemData->vITIPCDig->CreateProductGlassStructureImage();
			if (vLocalSystemData->vLogFile.vLogSerialData)
			{
				vLocalSystemData->vLogFile.WriteToLogFile("Start Accumulating Structure",cDebugMessage);
			}
		}
		else
		{
			CNoticeDialog TempNoticeDialog;
			CString TempText("");
			TempText.LoadString(IDS_AutoSetupNotConfigured);
			TempNoticeDialog.vNoticeText = TempText;
			TempNoticeDialog.vType = cErrorMessage;
			TempNoticeDialog.DoModal();
		}
	}

	if ((vLocalSystemData->vITIPCDig->vAverageEnabled) || (vLocalSystemData->vITIPCDig->vStructureEnabled) || 
		(vLocalSystemData->vITIPCDig->vCalibrateBodyTriggerReferenceEnabled) || (vLocalSystemData->vTakingQuickLearnImages))
	{
		vTotalToLearn = vLocalConfigurationData->vCalibrateBodyTriggerReferenceDisableCount + vLocalConfigurationData->vAverageDisableCount + vLocalConfigurationData->vStructureDisableCount;
		vLearningReference = true;

		CNoticeDialog TempNoticeDialog;
		CString TempString5 = "Done";
		if (vOldTakingQuickLearnImages)
		{
			TempText.LoadString(IDS_StartQuickLearn);//"\n\n\nStart Quick Learn."
			CString TempText1("");

			if ((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode)) //Continuous Feed, or a Pipeline
				TempText1 = "Images Required";
			else
				TempText1.LoadString(IDS_containersrequired); //"containers required"

			TempNoticeDialog.vNoticeText = TempText + " " + dtoa(vLocalConfigurationData->vNumberOfQuickLearnImagesToTake,0) + " " + TempText1 + ".";
		}
		else
		if (PasswordOK(cTemporaryInspxPassword,false))
		{
			if (vLearningReference)
				TempString5 = dtoa(vLocalConfigurationData->vCalibrateBodyTriggerReferenceDisableCount, 0);
			CString TempStringReference(TempString5);

			CString TempStringAverage(dtoa(vLocalConfigurationData->vAverageDisableCount, 0));

			CString TempStringStructure(dtoa(vLocalConfigurationData->vStructureDisableCount, 0));
			CString TempText("");
			TempText.LoadString(IDS_SendAllGoodContainersThroughScanTrac);
			CString TempText1("");
			TempText1 = "Containers For";
			CString TempText2("");
			if (PasswordOK(cTemporaryInspxPassword,false))
				TempText2 = "Reference";
			else
			{
				if ((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode))
					TempText2 = "Send Good Product";
				else
					TempText2.LoadString(IDS_SendGoodContainers);
			}
			CString TempText3("");
			if (PasswordOK(cTemporaryInspxPassword,false))
				TempText3 = "Average";
			else
			{
				if ((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode))
					TempText3 = "Send Good Product";
				else
					TempText3.LoadString(IDS_SendGoodContainers);
			}
			CString TempText4("");
			if (PasswordOK(cTemporaryInspxPassword,false))
				TempText4 = "Structure";
			else
			{
				if ((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode))
					TempText4 = "Send Good Product";
				else
					TempText4.LoadString(IDS_SendGoodContainers);
			}
			if (vOldTakingQuickLearnImages)
			{
				if ((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode)) //Continuous Feed, or a Pipeline
					TempText4 = "Start Learn. " + dtoa(vLocalConfigurationData->vNumberOfQuickLearnImagesToTake,0) + " images required.";
				else
					TempText4 = "Start Learn. " + dtoa(vLocalConfigurationData->vNumberOfQuickLearnImagesToTake,0) + " containers required.";
			}

			TempNoticeDialog.vNoticeText = " " + TempText + "\n" + TempStringReference + " " +TempText1 + " " + TempText2 +"\n" +
				TempStringAverage + " " +TempText1 + " " + TempText3 +"\n" + TempStringStructure + " " +TempText1 + " " + TempText4 +".\n";
		}
		else
		{
			CString TempString(dtoa(vTotalToLearn, 0));
			CString TempText;
			CString TempText1;
			if ((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode)) //Continuous Feed, or a Pipeline
				TempText1 = "Images Required";
			else
				TempText1.LoadString(IDS_containersrequired);

			TempText.LoadString(IDS_SendAllGoodContainersThroughScanTrac);
			TempNoticeDialog.vNoticeText = "\n" + TempText + "\n" + TempString + " " + TempText1;
		}
		TempNoticeDialog.vType = cInstructionsMessage;
		TempNoticeDialog.vAutoClose = 5000;
		TempNoticeDialog.DoModal();

		DWORD TempTotalCount = 0;
		if (vOldTakingQuickLearnImages)
			TempTotalCount = vLocalConfigurationData->vNumberOfQuickLearnImagesToTake;
		else
		{
			TempTotalCount = vLocalConfigurationData->vStructureDisableCount + vLocalConfigurationData->vAverageDisableCount;
			if (vLearningReference)
				TempTotalCount = TempTotalCount + vLocalConfigurationData->vCalibrateBodyTriggerReferenceDisableCount;
		}

		SetStatusLine(" Learn Started, send " + dtoa(TempTotalCount, 0) + " containers");
	}
	TempText.LoadString(IDS_Image);
	SetDlgItemText(IDC_Reference,TempText);
}
/*
void CAutoSetupImageDisplayDialog::DisplayEdgeExposureTimePercent()
{
	double TempPercentEdgeIntegrationTime = 
		((vGlobalCurrentProduct->vXRayIntegrationTimeAtEdge - 205.0) / (vGlobalCurrentProduct->vXRayIntegrationTime - 205.0)) * 100;
	CString TempString = dtoa(TempPercentEdgeIntegrationTime,1) + " %";
	SetDlgItemText(IDC_SubFunction6Display,TempString);
	UpdateButtonText();
}
*/
void CAutoSetupImageDisplayDialog::GiveMessageAlreadyAtThisValue(BYTE TempWhichOne)
{
	CNoticeDialog TempNoticeDialog;
	CString TempText;
	switch (TempWhichOne)
	{
		case 0: TempText.LoadString(IDS_AlreadyAtThisValue); break;
		case 1: TempText.LoadString(IDS_AlreadyAtMinimumValue); break;
		case 2: TempText.LoadString(IDS_AlreadyAtMaximumValue); break;
	}
	TempNoticeDialog.vNoticeText = TempText;
	TempNoticeDialog.vType = cErrorMessage;
	TempNoticeDialog.DoModal();
}

void CAutoSetupImageDisplayDialog::ManuallySetWidth(double TempNewWidth)
{
	vHaveNotSetSize = true;
	m_SubFunction2Button.Invalidate(false);
	float TempWidth = (float)TempNewWidth;
	
	if (vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac)  //on Trio, can only have 672 lines in the image
	{
		WORD TempNumberOfLines = (WORD)(TempWidth * vGlobalPixelsPerUnit * vGlobalCurrentProduct->vOverScanMultiplier);
		if (TempNumberOfLines > 672)
		{
			TempWidth =  (float)((672 / vGlobalPixelsPerUnit) / vGlobalCurrentProduct->vOverScanMultiplier);
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\n\nProduct Width too many lines.\nSet to: " + dtoa(TempWidth,2);
			TempNoticeDialog.vType = cNoticeMessage;
			TempNoticeDialog.DoModal();
		}
	}

	double TempDelta = 1;
	//if (vLocalConfigurationData->vUseMetric)
	//	TempDelta = 24.5;

	double TempDeltaWidth = TempWidth - vGlobalCurrentProduct->vProductImageWidth;

	double TempNewDelay = vGlobalCurrentProduct->vProductBodyTriggerToImageDistanceInInches - (TempDeltaWidth / 2);
	//if (vLocalConfigurationData->vUseMetric)
	//{
	//	if (TempNewDelay < 2 * 24.5)
	//		TempNewDelay = 2 * 24.5;
	//}
	//else
	{
		if (TempNewDelay < 2)
			TempNewDelay = 2;
	}
		
	if ((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode)) //Continuous Feed, or a Pipeline
	{  //set delay to 4 on pipeline systems
		//if (vLocalConfigurationData->vUseMetric)
		//	TempNewDelay = cForteTriggerToBeamOffset * 24.5;
		//else
			TempNewDelay = cForteTriggerToBeamOffset;
	}


	//if (PasswordOK(cSuperInspxPassword,false)) 
	//{
	//	SetDlgItemText(IDC_SubFunction7Display, dtoa(vGlobalCurrentProduct->vProductBodyTriggerToImageDistanceInInches,2));
	//}
	//else
	{
		double TempWhiteSpaceOnOneSideOfImage = (vGlobalCurrentProduct->vProductImageWidth / 1.6) * 0.3;
		if (TempWhiteSpaceOnOneSideOfImage > 4)
			TempWhiteSpaceOnOneSideOfImage = 4;
		
		double TempTriggerToBeamOffset = vGlobalCurrentProduct->vProductBodyTriggerToImageDistanceInInches;
		
		if (vLocalConfigurationData->vSystemBodyTriggerToXRayBeam > 1)
			TempTriggerToBeamOffset = -(vLocalConfigurationData->vSystemBodyTriggerToXRayBeam - vGlobalCurrentProduct->vProductBodyTriggerToImageDistanceInInches - TempWhiteSpaceOnOneSideOfImage);

		SetDlgItemText(IDC_SubFunction7Display, dtoa(TempTriggerToBeamOffset,2));
	}

	vGlobalCurrentProduct->SetProductBodyTriggerToImageBeltPositionOffset((float)(TempNewDelay));
	vGlobalCurrentProduct->SetProductImageWidth(TempWidth);
	vLocalSystemData->vChangedSizeOfProduct = true;

	//ask user if should figure out new retrigger lockout width
	//if (vHaveAnImage)
	if (vLocalSystemData->vITIPCDig->vOriginalImage)
	{
		BYTE TempOKToCalculateNewLockout = true;
		if (vGlobalCurrentProduct->vHaveManuallySetRetriggerLockout)
		{
			CYesNoDialog TempYesNoDialog;
			TempYesNoDialog.vNoticeText = _T("The Retrigger Lockout was previously set manually.\nDo you want to keep the manual value or \ncalculate a new value based on the new width?");
			TempYesNoDialog.vQuestionType = cConfirmQuestion;
			TempYesNoDialog.vYesButtonText = "Keep Lockout";
			TempYesNoDialog.vNoButtonText = "Calculate Lockout";
			int TempResult = TempYesNoDialog.DoModal();
			if (TempResult == IDOK)
				TempOKToCalculateNewLockout = false;
			else
				vGlobalCurrentProduct->vHaveManuallySetRetriggerLockout = false;
		}

		if (TempOKToCalculateNewLockout)
		{
			if ((TempNewWidth >= 10) || (vGlobalCurrentProduct->vProductLockOutWidth >= 10) || (vGlobalCurrentProduct->vProductLockOutWidth >= TempNewWidth))
			{
				BYTE TempOKToCalculateNewLockout = true;
				if (vGlobalCurrentProduct->vHaveManuallySetRetriggerLockout)
				{
					CYesNoDialog TempYesNoDialog;
					TempYesNoDialog.vNoticeText = _T("The Retrigger Lockout was previously set manually.\nDo you want to keep the manual value or \ncalculate a new value based on the new width?");
					TempYesNoDialog.vQuestionType = cConfirmQuestion;
					TempYesNoDialog.vYesButtonText = "Keep Lockout";
					TempYesNoDialog.vNoButtonText = "Calculate Lockout";
					int TempResult = TempYesNoDialog.DoModal();
					if (TempResult == IDOK)
						TempOKToCalculateNewLockout = false;
					else
						vGlobalCurrentProduct->vHaveManuallySetRetriggerLockout = false;
				}

				if (TempOKToCalculateNewLockout)
				{
					double TempNewBodyTriggerLockout = (TempWidth * 0.9);
						
					//if (vHaveAnImage)
					//	TempNewBodyTriggerLockout = (((vLocalSystemData->vITIPCDig->vOriginalImage->vRight - vLocalSystemData->vITIPCDig->vOriginalImage->vLeft) 
					//	/ (vGlobalPixelsPerUnit * vGlobalCurrentProduct->vOverScanMultiplier)));

					if ((TempNewWidth < 10) || (vLocalConfigurationData->vScanTracType == cSoloScanTrac) || (vLocalConfigurationData->vScanTracType == cSoloPlusScanTrac))
						TempNewBodyTriggerLockout = (TempWidth * 0.62);

					if (TempNewBodyTriggerLockout < vGlobalCurrentProduct->vProductImageWidth * .6)
						TempNewBodyTriggerLockout = vGlobalCurrentProduct->vProductImageWidth - (TempNewBodyTriggerLockout / 2);

					if (TempNewBodyTriggerLockout < vGlobalCurrentProduct->vProductImageWidth * .6)
						TempNewBodyTriggerLockout = vGlobalCurrentProduct->vProductImageWidth * .75;

					//make sure lockout is greater than half the image width
					if (TempNewBodyTriggerLockout <= TempNewWidth / 1.93)
						TempNewBodyTriggerLockout = (TempNewWidth / 1.93) + (1.0 / (vGlobalPixelsPerUnit * vGlobalCurrentProduct->vOverScanMultiplier));
					//if (vLocalConfigurationData->vUseMetric)
					//{
					//	if (TempNewBodyTriggerLockout < .5 * 25.4)
					//		TempNewBodyTriggerLockout = .5 * 25.4;
					//}
					//else
					{
						if (TempNewBodyTriggerLockout < .5)
							TempNewBodyTriggerLockout = .5;
					}

					if ((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode)) //Continuous Feed, or a Pipeline
					{
						//if (vLocalConfigurationData->vUseMetric)
						//	TempNewBodyTriggerLockout = 1 * 25.4;
						//else
							TempNewBodyTriggerLockout = 1;
					}
					vGlobalCurrentProduct->SetProductLockOutWidth((float)TempNewBodyTriggerLockout);
				}
			}
		}
	}
	DisplayWithPassword();
	SetChangeMade();
	if (PasswordOK(cTemporaryInspxPassword,false))
		SetDlgItemText(IDC_SubFunction8Display,
			dtoa(vGlobalCurrentProduct->vProductImageWidth,2) + " (" + dtoa(vGlobalCurrentProduct->vImageWidthPosition,0) + ")");
	else
		SetDlgItemText(IDC_SubFunction8Display, dtoa(vGlobalCurrentProduct->vProductImageWidth,2));

	CheckDensityCompensationInspection();

	if (vLocalSystemData->vInAutoSetup)
	{
		vLocalSystemData->vChangedSizeOfProduct = true;
		vLocalSystemData->vChangedSizeOfProductCheckDensityROI = 1;
	}
	bool TempMakeInspectionsFullImage = false;
	if ((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode) ||
		(vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vLocalConfigurationData->vScanTracType == cAllegroScanTrac) || 
		(vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac) || (vLocalConfigurationData->vScanTracType == cBriosoScanTrac)) //Continuous Feed, or a Pipeline or TD
		TempMakeInspectionsFullImage = true;

	vGlobalCurrentProduct->SetupAllInspectionROIsFromReference(vLocalSystemData->vITIPCDig->vOriginalImage, true, TempMakeInspectionsFullImage, vLocalSystemData->vITIPCDig->vProductAverageImage); 

	vLocalSystemData->vITIPCDig->CalculateAverageDriftDensity(vGlobalCurrentProduct);
	vLocalSystemData->vIndexForDriftDensityOfLastNImages = 0;

	vGlobalCurrentProduct->vMaximumImageOverlap = (vGlobalCurrentProduct->vProductImageWidth / 2) - .1;
	ClearReferenceLocationData();
	vLocalSystemData->vITIPCDig->RenameLearnFilesToBackup(vGlobalCurrentProduct);
	vLocalSystemData->vITIPCDig->DeleteLearnImages();
	vLocalSystemData->vITIPCDig->DeleteLearnFiles(vGlobalCurrentProduct);
	ClearDensityFeedBackLoop();
	vMainWindowPointer->CheckEjectorsConfigured(vGlobalCurrentProduct, 7);

	vMainWindowPointer->CalculatePipelineEncoderAndTrigger();
	ChangedProductSettingsSoReload();
}

//void CAutoSetupImageDisplayDialog::ShowPipeEmptyIndicator()
//{
//	vShowingPipeEmpty = true;
//	CString TempText = "";
//	if ((vLocalConfigurationData->vScanTracType == cFermataScanTrac) || (vLocalConfigurationData->vScanTracType == cLegatoScanTrac))
//		TempText = " Pipe Empty");
//	else
//		TempText = " Conveyor Empty");
//	BYTE TempLength = TempText.GetLength();
//	m_RejectReason.SetWindowPos(NULL,cMainBackgroundLeft + 2,	cMainBackgroundTop + 2, TempLength * 10,19,SWP_NOZORDER);
//	SetDlgItemText(IDC_RejectReason,TempText);
//	m_RejectReason.ShowWindow(SW_SHOW);
//}
//
//void CAutoSetupImageDisplayDialog::ShowAirInPipeIndicator()
//{
//	vShowingAirInPipe = true;
//	CString TempText = "Air In Pipe";
//	BYTE TempLength = TempText.GetLength();
//	m_RejectReason.SetWindowPos(NULL,cMainBackgroundLeft + 2,	cMainBackgroundTop + 2, TempLength * 10,19,SWP_NOZORDER);
//	SetDlgItemText(IDC_RejectReason,TempText);
//	m_RejectReason.ShowWindow(SW_SHOW);
//
//	//SetDlgItemText(IDC_StatusLine," Air In Pipe");
//	//int TimerResult = SetTimer(vRestoreStatusLineTimerHandle,1000,NULL);
//	//if (!TimerResult)
//	//	ReportErrorMessage("Error-vRestoreStatusLine Timer Failed",cEMailInspx,32000);
//}

void CAutoSetupImageDisplayDialog::ShowSensitivity()
{
	if (vInspectionEditing)
	if (vLocalConfigurationData->vShowSensitivityDisplays == cShowSensitivityDisplays)
	{
		double vOldSensitivity = vSensitivity;
		vSensitivity = vInspectionEditing->GetSensitivity(false, 0);
		if ((vOldSensitivity != vSensitivity) || (!vOldSensitivity) || (!vSensitivity))
		{
			if ((vMode == cSetDensityThreshold) || (vMode == cSetContaminantThreshold) || (vMode == cSetVoidThreshold))
			{
				if (vSensitivity > 2)
					SetDlgItemText(IDC_SubFunction1Display, "Sen: " + dtoa((BYTE)vSensitivity, 0) + "%");
				else
					SetDlgItemText(IDC_SubFunction1Display, _T("Sen: 0%"));
			}
			RECT TempRectangle;
			TempRectangle.left = vAutoSetupSensitivityLeft;
			TempRectangle.top = vAutoSetupSensitivityTop;
			TempRectangle.right = vAutoSetupSensitivityRight;
			TempRectangle.bottom = vAutoSetupSensitivityBottom + 1;
			this->InvalidateRect(&TempRectangle,false);

			TempRectangle.bottom = vAutoSetupLearnSensitivityBottom + 1;
			TempRectangle.top = TempRectangle.bottom - vAutoSetupSensitivityHeight - 1;
			this->InvalidateRect(&TempRectangle,false);
		}
	}
}

void CAutoSetupImageDisplayDialog::ShowLearnSensitivity()
{
	if (vInspectionEditing)
	{
		vMainWindowPointer->CalculateSensitivity();
		if (vLocalSystemData->vLearnSensitivities[vInspectionNumberEditing] < 255)
		{
			if (vLocalSystemData->vLearnSensitivities[vInspectionNumberEditing] == 100)
				SetDlgItemText(IDC_SubFunction2Display, _T("Lrn Sen:100%"));
			else
				SetDlgItemText(IDC_SubFunction2Display, "Lrn Sen: " + dtoa((BYTE)vLocalSystemData->vLearnSensitivities[vInspectionNumberEditing], 0) + "%");

			RECT TempRectangle;
			TempRectangle.left = vAutoSetupSensitivityLeft;
			TempRectangle.right = vAutoSetupSensitivityRight;

			TempRectangle.bottom = vAutoSetupLearnSensitivityBottom + 1;
			TempRectangle.top = TempRectangle.bottom - vAutoSetupSensitivityHeight - 1;
			this->InvalidateRect(&TempRectangle,false);
		}
	}
}

void CAutoSetupImageDisplayDialog::ClearLearnSensitivity()
{
	m_SubFunction2Display.ShowWindow(SW_HIDE);
//	m_SubFunction2Display.SetWindowPos(NULL,5,cLearnSensitivityBottom - 5,100,20,SWP_NOZORDER);
//	::SetupItem(&m_SubFunction2Display);

	RECT TempRectangle;
	TempRectangle.left = vAutoSetupSensitivityLeft;
	TempRectangle.right = vAutoSetupSensitivityRight;
	TempRectangle.bottom = vAutoSetupLearnSensitivityBottom + 1;
	TempRectangle.top = TempRectangle.bottom - vAutoSetupSensitivityHeight - 1;
	this->InvalidateRect(&TempRectangle,false);
}

void CAutoSetupImageDisplayDialog::ClearSensitivity()
{
	m_SubFunction1Display.ShowWindow(SW_HIDE);
//	m_SubFunction1Display.SetWindowPos(NULL,5,cSensitivityBottom - 5,100,20,SWP_NOZORDER);
	//::SetupItem(&m_SubFunction1Display);
	m_SubFunction2Display.ShowWindow(SW_HIDE);
//	m_SubFunction2Display.SetWindowPos(NULL,5,cLearnSensitivityBottom - 5,100,20,SWP_NOZORDER);
//	::SetupItem(&m_SubFunction2Display);

	RECT TempRectangle;
	TempRectangle.left = vAutoSetupSensitivityLeft;
	TempRectangle.top = vAutoSetupSensitivityTop;
	TempRectangle.right = vAutoSetupSensitivityRight;
	TempRectangle.bottom = vAutoSetupSensitivityBottom + 1;
	this->InvalidateRect(&TempRectangle,false);

	TempRectangle.bottom = vAutoSetupLearnSensitivityBottom + 1;
	TempRectangle.top = TempRectangle.bottom - vAutoSetupSensitivityHeight - 1;
	this->InvalidateRect(&TempRectangle,false);
	ClearLearnSensitivity();
}

void CAutoSetupImageDisplayDialog::EnterNominalWeight()
{
	if (vInspectionEditing)
	{
		CNumericEntryDialog INumericEntryDialog;  
			
		//Set dialog box data titles and number value
		INumericEntryDialog.vEditString = dtoa(vInspectionEditing->vCheckWeighNominalWeight,2);

		INumericEntryDialog.m_DialogTitleStaticText1 = "Enter Nominal Weight for Product";
		INumericEntryDialog.m_DialogTitleStaticText2 = "Nominal Weight is the Weight you are trying to fill to";
		INumericEntryDialog.m_DialogTitleStaticText3 = "This is a reference value only, not a Threshold";
		INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + INumericEntryDialog.vEditString;
		INumericEntryDialog.vMaxValue = 5000;
		INumericEntryDialog.vMinValue = 0;
		INumericEntryDialog.vIntegerOnly = false;
		INumericEntryDialog.m_UnitsString = "Units In " + vInspectionEditing->vCheckWeighUnits;

		int nResponse = INumericEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			vInspectionEditing->vCheckWeighNominalWeight = ATOF(INumericEntryDialog.vEditString);

			SetChangeMade();
			UpdateButtonText();
		}
		else 
		if (nResponse == 10)
		{
			//Main Menu button pressed
			ExitAutoSetupDialog(10);
		}
	}
}

bool CAutoSetupImageDisplayDialog::DeleteCheckWeighCalibrations(bool TempChangingROI)
{
	bool TempOK = true;
	if (vInspectionEditing)
	if ((vInspectionEditing->vInspectionType == cUnderfillByWeightInspection) || (vInspectionEditing->vInspectionType == cOverfillByWeightInspection))
	if ((vInspectionEditing->vCheckWeighMFactor != 1) || (vInspectionEditing->vCheckWeighBFactor))
	if ((vInspectionEditing->vSetupHistoryMaximumBadCount) || (vInspectionEditing->vSetupHistoryMinimumGoodCount))
	{
		if (vMouseState)
		{
			KillTimer(vMouseDownRepeatTimerHandle);
			//stop scrolling
			vMouseState = 0;
		}

		CYesNoDialog TempYesNoDialog;
		//if (vLocalConfigurationData->vScanTracType == cBriosoScanTrac)
		{
			if (TempChangingROI)
				TempYesNoDialog.vNoticeText = _T("\nThis Inspection has weight calibrations.\nChanging the ROI will change the readings making the current calibrations wrong.\nDo you want to delete the Weight Calibrations and recalibrate,\nor keep Weight Calibrations and Continue?");
			else
				TempYesNoDialog.vNoticeText = _T("\nThis Inspection has weight calibrations.\nChanging this value will re-calculate weight calibrations already taken so they are correct for the new weight you enter.\nDo you want to delete the Weight Calibrations and recalibrate,\nor keep Weight Calibrations and Continue?");
			TempYesNoDialog.vNoButtonText = "Keep and Continue";
		}
		TempYesNoDialog.vQuestionType = cQuestion;
		TempYesNoDialog.vYesButtonText = "Delete Calibrations";
		int TempResult = TempYesNoDialog.DoModal();
		if (TempResult == IDOK)
		{
			//convert total brighness values to weight as will be loosing what brightness values mean
			if (vInspectionEditing->vCheckWeighTrendAlarmMaximum)
				vInspectionEditing->vCheckWeighTrendAlarmMaximum = vInspectionEditing->ConvertToWeight(vInspectionEditing->vCheckWeighTrendAlarmMaximum);
			if (vInspectionEditing->vCheckWeighTrendAlarmMinimum)
				vInspectionEditing->vCheckWeighTrendAlarmMinimum = vInspectionEditing->ConvertToWeight(vInspectionEditing->vCheckWeighTrendAlarmMinimum);
			if (vInspectionEditing->vDensityThresholdLower)
				vInspectionEditing->SetDensityLowerThreshold((float)(vInspectionEditing->ConvertToWeight(vInspectionEditing->vDensityThresholdLower)));
			if (vInspectionEditing->vDensityThresholdUpper)
				vInspectionEditing->vDensityThresholdUpper = (float)(vInspectionEditing->ConvertToWeight(vInspectionEditing->vDensityThresholdUpper));

			vInspectionEditing->vCheckWeighOffset = 0;
			vInspectionEditing->vCheckWeighMFactor = 1;
			vInspectionEditing->vCheckWeighBFactor = 0;
			vInspectionEditing->vSetupHistoryMaximumBadCount = 0;
			vInspectionEditing->vSetupHistoryMinimumGoodCount = 0;
			vInspectionEditing->vSetupHistoryMaximumBadAverage = 0;
			vInspectionEditing->vSetupHistoryMaximumBadStandardDeviation = 0;
			vInspectionEditing->vSetupHistoryMaximumBad = 0;
			vInspectionEditing->vSetupHistoryMinimumGoodAverage = 0;
			vInspectionEditing->vSetupHistoryMinimumGoodStandardDeviation = 0;
			vInspectionEditing->vSetupHistoryMinimumGood = 0;
			vInspectionEditing->vSetupHistoryQualityFactor = 0;
			vLocalSystemData->vMaximumBadCount = 0;
			vLocalSystemData->vMaximumBadCount = 0;
			vLocalSystemData->vMinimumGoodIndex = 0;
			vLocalSystemData->vMinimumGoodStandardDeviation = 0;
			vLocalSystemData->vMinimumGoodAverage = 0;
			vLocalSystemData->vMaximumBadIndex = 0;
			vLocalSystemData->vMaximumBadStandardDeviation = 0;
			vLocalSystemData->vMaximumBadAverage = 0;
			vLocalSystemData->vQualityFactor = 0;

			UpdateButtonText();
			DisplayDensityThresholdUpper();
			DisplayDensityThresholdLower();
			DisplayMinimumGoodMaximumBadDataValues(true);
			DisplayWithPassword();
		}
		//else
		//if (vLocalConfigurationData->vScanTracType != cBriosoScanTrac)
		//	TempOK = false;
	}
	return TempOK;
}

void CAutoSetupImageDisplayDialog::GiveLinkedInspectionNotice()
{
	if (vInspectionEditing)
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\nThis Inspection: " + vInspectionEditing->vName + "\nUses the ROI and weight calibrations from\ninspection: " + vInspectionEditing->vInspectionNameForWeightTrendingPointer->vName;
		TempNoticeDialog.vType = cNoticeMessage;
		TempNoticeDialog.DoModal();
	}
}

void CAutoSetupImageDisplayDialog::AskForWeightTrendingInspectionToLinkThisOneTo(BYTE TempThisInspection)
{
	//Create dialog box
	CSelectItemDialog ISelectItemDialog;  
		
	//Set dialog box data titles and number value
	ISelectItemDialog.vTitleString = "Select Inspection for ROI and Weight Calibration";
	ISelectItemDialog.vBackgroundColor = cGreen;
	ISelectItemDialog.vNumberOfItems = 0;
	ISelectItemDialog.vItems[ISelectItemDialog.vNumberOfItems++] = "None - Uses Own ROI and Calibrations";
	ISelectItemDialog.vGreenItem = ISelectItemDialog.vNumberOfItems;

	for (BYTE TempLoop = 0; TempLoop < vGlobalCurrentProduct->vNumberOfInspections; TempLoop++)
	if (vGlobalCurrentProduct->vInspection[TempLoop])
	if (TempThisInspection != TempLoop) //ignore this inspection
	if ((vGlobalCurrentProduct->vInspection[TempLoop]->vInspectionType == cUnderfillByWeightInspection) || (vGlobalCurrentProduct->vInspection[TempLoop]->vInspectionType == cOverfillByWeightInspection))
	if (vGlobalCurrentProduct->vInspection[TempLoop]->vInspectionNameForWeightTrendingName.GetLength() == 0) //can not be pointing at another inspection
	{
		ISelectItemDialog.vItems[ISelectItemDialog.vNumberOfItems++] = vGlobalCurrentProduct->vInspection[TempLoop]->vName;
		if (vGlobalCurrentProduct->vInspection[TempThisInspection]->vInspectionNameForWeightTrendingName == vGlobalCurrentProduct->vInspection[TempLoop]->vName)
			ISelectItemDialog.vGreenItem = ISelectItemDialog.vNumberOfItems;
	}

	CString TempText = "-";
	TempText.LoadString(IDS_Product);
	ISelectItemDialog.m_DialogTitleStaticText2 = TempText + ": " + *(vGlobalCurrentProduct->GetProductName());
	ISelectItemDialog.m_DialogTitleStaticText3 = "";
	ISelectItemDialog.vInAutoSetup = vLocalSystemData->vInAutoSetup;
	//Pass control to dialog box and display
	int nResponse = ISelectItemDialog.DoModal();
	//dialog box is now closed, if user pressed select do this
	//if user pressed cancel, do nothing
	if (nResponse == IDOK)
	{
		if (ISelectItemDialog.vItemNumberSelected == 1)
			vGlobalCurrentProduct->vInspection[TempThisInspection]->vInspectionNameForWeightTrendingName = "";
		else
			vGlobalCurrentProduct->vInspection[TempThisInspection]->vInspectionNameForWeightTrendingName = ISelectItemDialog.vItems[ISelectItemDialog.vItemNumberSelected - 1];

		vGlobalCurrentProduct->MatchInspectionsForWeightTrending();
	}
}

void CAutoSetupImageDisplayDialog::ClearEvaluate()
{
	if (vInspectionEditing)
	{
		vInspectionEditing->vSetupHistoryMinimumGoodAverage = 0;
		vInspectionEditing->vSetupHistoryMinimumGoodStandardDeviation = 0;
		vInspectionEditing->vSetupHistoryMinimumGoodCount = 0;
		vInspectionEditing->vSetupHistoryMinimumGood = 0;
		vInspectionEditing->vSetupHistoryMaximumBad = 0;
		vInspectionEditing->vSetupHistoryMaximumBadCount = 0;
		vInspectionEditing->vSetupHistoryMaximumBadAverage = 0;
		vInspectionEditing->vSetupHistoryMaximumBadStandardDeviation = 0;
		vInspectionEditing->vSetupHistoryQualityFactor = 0;

		vLocalSystemData->vMaximumBadCount = 0;
		vLocalSystemData->vMinimumGoodIndex = 0;
		vLocalSystemData->vMinimumGoodStandardDeviation = 0;
		vLocalSystemData->vMinimumGoodAverage = 0;
		vLocalSystemData->vMaximumBadIndex = 0;
		vLocalSystemData->vMaximumBadStandardDeviation = 0;
		vLocalSystemData->vMaximumBadAverage = 0;
		vLocalSystemData->vQualityFactor = 0;
	}
}
void CAutoSetupImageDisplayDialog::ShowNoMouseClickDialogMessage() 
{
	CNoticeDialog TempNoticeDialog;
	CString TempText("");
	TempText = ("Mouse Click Not Available");
	TempNoticeDialog.vNoticeText = "\n\n\nMouse Click Not Available";
	TempNoticeDialog.vType = cInformationMessage;
	TempNoticeDialog.DoModal();
}

void CAutoSetupImageDisplayDialog::GiveSendGoodContainersNotice()
{
	if (!vLocalSystemData->vITIPCDig->vDriftCompensationLocked)
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\nSend Good Containers\nuntil this message is cleared.\nThen you can evaluate.";
		TempNoticeDialog.vCloseOnBrightnessLoopLock = true;
		TempNoticeDialog.vType = cNoticeMessage;
		TempNoticeDialog.DoModal();
	}
}

void CAutoSetupImageDisplayDialog::DisplayANewImage()
{
	CString TempXRayString = " ";
	GetDlgItemText(IDC_Calibrating, TempXRayString);
	if (TempXRayString != "X-RAY ON")
		SetDlgItemText(IDC_Calibrating, _T("X-RAY ON"));

	if (vLocalSystemData->vAdjustingBrightnessBeforeLearning != vOldAdjustingBrightnessBeforeLearning)
	{
		if (vOldAdjustingBrightnessBeforeLearning)
		{
			SetStatusLine(" Learning-Adjusting Image Complete");
			//StartLearning(); //done in ScanTracDlg
		}
		vOldAdjustingBrightnessBeforeLearning = vLocalSystemData->vAdjustingBrightnessBeforeLearning;
		UpdateButtonText();
	}
	else
	if ((vLocalSystemData->vAdjustingBrightnessBeforeLearning) && (!vLocalSystemData->vITIPCDig->vDriftCompensationLocked))
	{
		vToggleSign = !vToggleSign;
		if (vToggleSign)
			SetStatusLine(" Learning-Adjusting Image");
		else
		if (vLocalSystemData->vCurrentBrightnessAdjustment > 6)
			SetStatusLine(" Learning-Adjusting Image-");
		else
			SetStatusLine(" Learning-Adjusting Image+");
	}
	else
	if ((!vLocalSystemData->vITIPCDig->vDriftCompensationLocked) && (vInspectionEditing) && ((vInspectionEditing->vFindGoodIntensity) || (vInspectionEditing->vFindBadIntensity)))
	{
		vToggleSign = !vToggleSign;
		if (vToggleSign)
			SetStatusLine(" Evaluate-Adjusting Image");
		else
		if (vLocalSystemData->vCurrentBrightnessAdjustment > 6)
			SetStatusLine(" Evaluate-Adjusting Image-");
		else
			SetStatusLine(" Evaluate-Adjusting Image+");
	}
	else
	if ((vMode == cSetImageSize) && (vGlobalCurrentProduct->vMultiLaneNumberOfLanes))
			SetStatusLine(" Lane:" + dtoa(vLocalSystemData->vITIPCDig->vOriginalImage->vMultiLaneLaneNumber, 0) + " Product: " + *vGlobalCurrentProduct->GetProductName());

	CString TempText1;
	CString TempText;
	if ((!vFreezeImageOnScreen) || (!((vMode == cSetContaminantThreshold) || (vMode == cSetVoidThreshold) || (vMode == cSetDensityThreshold))))
	if ((vLocalSystemData->vITIPCDig->vOriginalImage) && (vLocalSystemData->vITIPCDig->vOriginalImage->vGreyImage))
	{
		//new image newframe
		if (vLocalSystemData->vITIPCDig->vNextImageToDisplayLock.Lock())
		{
			vLocalSystemData->vITIPCDig->vOriginalImage->CopyImage(vLocalSystemData->vITIPCDig->vNextImageToDisplay, vLocalSystemData->vITIPCDig->vOriginalBufferSize);
			vLocalSystemData->vITIPCDig->vNextImageToDisplayLock.Unlock();
		}

		if (vNeedToSetContainerThresholdBounds)
		{
			CalculateContainerThresholdBounds();
			vNeedToSetContainerThresholdBounds--;
		}

		if (vLocalSystemData->vITIPCDig->vOriginalImage->vContainerEjectReason != cPassContainerWrongWidth)
		if ((vLocalSystemData->vITIPCDig->vOriginalImage->vContainerEjectReason == 255) || //if no reason yet, or not ejecting, or autosetup
			(!vLocalConfigurationData->vEnableEjectors) || (vLocalSystemData->vInAutoSetup))//check for wrong width
		if ((!vLocalConfigurationData->vContinuousFeedContainerRate) && (!vLocalConfigurationData->vBulkProductMode) && (!vGlobalCurrentProduct->vBulkProductMode) && (vLocalConfigurationData->vScanTracType != cBriosoScanTrac) &&
			(vLocalConfigurationData->vScanTracType != cAtempoScanTrac) && (vLocalConfigurationData->vScanTracType != cForteScanTrac) && (vLocalConfigurationData->vScanTracType != cAllegroScanTrac) &&
			(vLocalConfigurationData->vScanTracType != cCaseInspectorScanTrac)) //Not Continuous Feed, or a Pipeline or TD
		if (vGlobalCurrentProduct->vReferenceWidth)
		{
			int TempWidthDelta = vLocalSystemData->vITIPCDig->vOriginalImage->vWidth - vGlobalCurrentProduct->vReferenceWidth;
			if (TempWidthDelta < 0)
				TempWidthDelta = -TempWidthDelta;

			WORD TempTolerance = vGlobalCurrentProduct->vReferenceWidth / 20;  //20% of container width
			if (vLocalConfigurationData->vShapeHandlingWidthTolerancePixels)
			if (TempTolerance > vLocalConfigurationData->vShapeHandlingWidthTolerancePixels)
				TempTolerance = vLocalConfigurationData->vShapeHandlingWidthTolerancePixels;
			if (TempTolerance < 5)
				TempTolerance = 5;
			if (TempWidthDelta > TempTolerance)
				vLocalSystemData->vITIPCDig->vOriginalImage->vContainerEjectReason = cPassContainerWrongWidth;
		}

		if ((vLocalSystemData->vPipeIsEmpty > vOldPipeIsEmpty) || (vLocalSystemData->vPipeIsEmpty > 5))
		{
			CString TempText = "";
			if ((vLocalConfigurationData->vScanTracType == cFermataScanTrac) || (vLocalConfigurationData->vScanTracType == cLegatoScanTrac))
				TempText = " Pipe Empty";
			else
				TempText = " Conveyor Empty";
			BYTE TempLength = TempText.GetLength();
			m_RejectReason.SetWindowPos(NULL,ResizeXCoor(cMainBackgroundLeft + 2 + 6),	ResizeYCoor(cMainBackgroundTop + 2), ResizeXCoor(TempLength * 10),ResizeYCoor(19),SWP_NOZORDER);
			SetDlgItemText(IDC_RejectReason,TempText);
			m_RejectReason.ShowWindow(SW_SHOW);
		}
		else
		if (((vLocalSystemData->vAirBubblesInPipe > vOldAirBubblesInPipe) || (vLocalSystemData->vAirBubblesInPipe > 5)) && 
			((vLocalConfigurationData->vScanTracType == cFermataScanTrac) || (vLocalConfigurationData->vScanTracType == cLegatoScanTrac)))
		{
			CString TempText = "Air In Pipe";
			BYTE TempLength = TempText.GetLength();
			m_RejectReason.SetWindowPos(NULL,ResizeXCoor(cMainBackgroundLeft + 2 + 6),	ResizeYCoor(cMainBackgroundTop + 2), ResizeXCoor(TempLength * 10),ResizeYCoor(19),SWP_NOZORDER);
			SetDlgItemText(IDC_RejectReason,TempText);
			m_RejectReason.ShowWindow(SW_SHOW);
		}
		else
		if (((vOldAverageEnabled) || (vOldStructureEnabled) || (vOldCalibrateBodyTriggerReferenceEnabled) || (vOldTakingQuickLearnImages) || (vOldAdjustingBrightnessBeforeLearning)) &&
			(vLocalSystemData->vITIPCDig->vOriginalImage->vImageIntensityTooFarOff) && (!vGlobalCurrentProduct->vBulkProductMode))
		{//if learning and image too dark or light, indicate on screen
			CString TempText = "Too Dark/Light";
			BYTE TempLength = TempText.GetLength();
			m_RejectReason.SetWindowPos(NULL,ResizeXCoor(cMainBackgroundLeft + 2 + 6),	ResizeYCoor(cMainBackgroundTop + 2), ResizeXCoor(TempLength * 10),ResizeYCoor(19),SWP_NOZORDER);
			SetDlgItemText(IDC_RejectReason,TempText);
			m_RejectReason.ShowWindow(SW_SHOW);
		}
		else
		if //(((vOldAverageEnabled) || (vOldStructureEnabled) || 
			//(vOldCalibrateBodyTriggerReferenceEnabled) || (vOldTakingQuickLearnImages) || (vOldAdjustingBrightnessBeforeLearning)) &&
			((vLocalSystemData->vITIPCDig->vOriginalImage->vContainerEjectReason == cPassContainerWrongWidth) && 
			((vLocalConfigurationData->vScanTracType == cSoloScanTrac) || (vLocalConfigurationData->vScanTracType == cSoloPlusScanTrac) || (vLocalConfigurationData->vScanTracType == cPiccoloScanTrac)))
		{//if learning and image wrong width, indicate on screen
			CString TempText = "Wrong Width";
			BYTE TempLength = TempText.GetLength();
			m_RejectReason.SetWindowPos(NULL,ResizeXCoor(cMainBackgroundLeft + 2 + 6),	ResizeYCoor(cMainBackgroundTop + 2), ResizeXCoor(TempLength * 10),ResizeYCoor(19),SWP_NOZORDER);
			SetDlgItemText(IDC_RejectReason,TempText);
			m_RejectReason.ShowWindow(SW_SHOW);
		}
		else
		//if bad container and real inspecting, or size problem and done size & exposure
		if (((vLocalSystemData->vITIPCDig->vOriginalImage->vContainerEjectReason < 255) && (!vLocalSystemData->vInAutoSetup)) || (((vMode == cLearnMenu) || (vMode == cSetVoidThreshold) || 
			(vMode == cSetDensityThreshold) || (vMode == cSetContaminantThreshold)) && ((vLocalSystemData->vITIPCDig->vOriginalImage->vContainerEjectReason > vGlobalCurrentProduct->vNumberOfInspections) && 
			(vLocalSystemData->vITIPCDig->vOriginalImage->vContainerEjectReason < cEjectAuxilaryDetectorOffset))))
		{
			BYTE TempLength = vLocalSystemData->vCriteriaString[vLocalSystemData->vCountToUseForEjectReason[vLocalSystemData->vITIPCDig->vOriginalImage->vContainerEjectReason]].GetLength();
			m_RejectReason.SetWindowPos(NULL,ResizeXCoor(cMainBackgroundLeft + 2 + 6),	ResizeYCoor(cMainBackgroundTop + 2), ResizeXCoor(TempLength * 10),ResizeYCoor(19),SWP_NOZORDER);
			SetDlgItemText(IDC_RejectReason, vLocalSystemData->vCriteriaString[vLocalSystemData->vCountToUseForEjectReason[vLocalSystemData->vITIPCDig->vOriginalImage->vContainerEjectReason]]);
			m_RejectReason.ShowWindow(SW_SHOW);
		}
		else
		if (((vLocalConfigurationData->vScanTracType == cFermataScanTrac) || (vLocalConfigurationData->vScanTracType == cLegatoScanTrac)) && (vLocalSystemData->vActualEncoderRate > 5))
		{
			CString TempText = "Pumping";
			BYTE TempLength = TempText.GetLength();
			m_RejectReason.SetWindowPos(NULL,ResizeXCoor(cMainBackgroundLeft + 2 + 6),	ResizeYCoor(cMainBackgroundTop + 2), ResizeXCoor(TempLength * 10),ResizeYCoor(19),SWP_NOZORDER);
			SetDlgItemText(IDC_RejectReason,TempText);
			m_RejectReason.ShowWindow(SW_SHOW);
		}
		else
			m_RejectReason.ShowWindow(SW_HIDE);

		vOldPipeIsEmpty = vLocalSystemData->vPipeIsEmpty;
		vOldAirBubblesInPipe = vLocalSystemData->vAirBubblesInPipe;

		if (!vHaveAnImage)
		{
			vHaveAnImage = true;
			m_Background.ShowWindow(SW_HIDE);
			WaitDisplayLocationEdge();

			if (vMode == cSetImageSize)
			if (vLocalSystemData->vInAutoSetup)
			{
				vSubFunction2ButtonEnable = true;		//m_SubFunction2Button.EnableWindow(true);
				m_SubFunction2Button.Invalidate(false);
			}
		}

		if (!vWaitingForBackgroundImage)
		if (vShowingBackground == 100)
		{
			vShowingBackground = false;
			UpdateButtonText();
		}

		DrawImageOnScreen();
			
		CString TempStringName(cSpace);
		TempStringName = TempStringName + vProductEditingName;
		if (vGlobalCurrentProduct->vMultiLaneNumberOfLanes)
			TempStringName = " " + dtoa(vLocalSystemData->vITIPCDig->vOriginalImage->vMultiLaneLaneNumber, 0) + " :" + TempStringName;

		if (vWaitingForBackgroundImage)
		{
			if ((vMode == cSetBackground) || (vMode == cSetImageSize))
			{
				vWaitingForBackgroundImage = false;
				if (vLocalSystemData->vRadiationDoorManualControlRegister == cRadiationShutterAutoControl)
				if (vGlobaluCSerialPort)
					vGlobaluCSerialPort->SetRadiationDoorManualControl(cRadiationShutterAutoControl);
				//vShowingBackground = true;
				CWnd *TempWindow = CWnd::GetDlgItem(IDC_SubFunction5Button);
				if (TempWindow)
					TempWindow->Invalidate(true);
				if (vLocalSystemData->vITIPCDig->vProductBackgroundImage != NULL)
				{
					im_delete(vLocalSystemData->vITIPCDig->vProductBackgroundImage);
					vLocalSystemData->vITIPCDig->vProductBackgroundImage = NULL;
					vShowingBackground = false;
				}
				if (ThereIsEnoughMemory(vLocalSystemData->vITIPCDig->vOriginalBufferSizeX * 
					vLocalSystemData->vITIPCDig->vOriginalBufferSizeY + sizeof(Image), "Background Image"))
					vLocalSystemData->vITIPCDig->vProductBackgroundImage = im_create(IM_BYTE, vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, 
						vLocalSystemData->vITIPCDig->vOriginalBufferSizeY);

				if ((vLocalSystemData->vITIPCDig->vProductBackgroundImage) && (vLocalSystemData->vITIPCDig->vProductBackgroundImage->pix))
				if ((vLocalSystemData->vITIPCDig->vOriginalImage) && (vLocalSystemData->vITIPCDig->vOriginalImage->vGreyImage))
				{
					CopyMemory(
						vLocalSystemData->vITIPCDig->vProductBackgroundImage->pix,
						vLocalSystemData->vITIPCDig->vOriginalImage->vGreyImage,
						vLocalSystemData->vITIPCDig->vOriginalBufferSize);
					SetStatusLine(" Showing Sampled Background");
					vShowingBackground = 100;
				}
				/*
				if (vMode == cSetImageSize)
				{
					CProduct *TempProduct = vMainWindowPointer->GetProductByName("~SizeExposure");
					if (TempProduct)
						vLocalSystemData->vITIPCDig->SaveBackgroundImageToFile(TempProduct);
					vAskedIfShouldRemoveBackground = true;
					vLocalSystemData->vRemoveBackgroundInAutoSize = true;
				}
				else
				*/
					vLocalSystemData->vITIPCDig->SaveBackgroundImageToFile(vGlobalCurrentProduct);

				vLocalSystemData->vITIPCDig->MakeProductBackgroundImageCorrectionFactors();
				CNoticeDialog TempNoticeDialog;
				TempNoticeDialog.vNoticeText = "\n\n\nBackground Sampled.";
				TempNoticeDialog.vType = cNoticeMessage;
				TempNoticeDialog.vAutoClose = 2000;
				TempNoticeDialog.DoModal();

				UpdateButtonText();

				tSerialCommand TempCommand;
				//After sending simulated container, must Send Interrupt Mask for body trigger
				TempCommand[0] = 0x1C;
				TempCommand[1] = 0x03;  //send mask for 16 digital lines in #3 , body triggers
				TempCommand[2] = 0x00;
				TempCommand[3] = vLocalSystemData->vDigitalInputLine3Mask;
				if (vGlobaluCSerialPort)
					vGlobaluCSerialPort->SendSerialCommand(TempCommand);
				Sleep(cSendCommandSleepTime);

				TempCommand[1] = 0x04;  //send mask so uC know which bit is body trigger
				TempCommand[3] = vLocalSystemData->vCurrentBodyTriggerMask;
				if (vGlobaluCSerialPort)
					vGlobaluCSerialPort->SendSerialCommand(TempCommand);
				Sleep(cSendCommandSleepTime);

				vNeedToSetContainerThresholdBounds = 2;
			}

			if (vMode == cSetBackground)
			{
				if (!vShowingBackground)
				if (vLocalSystemData->vITIPCDig->vProductBackgroundImage)
				if ((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode)) //Continuous Feed, or a Pipeline
					SetStatusLine(" Showing Image of Product");
				else
					SetStatusLine(" Showing Container Image");
			}
		}
		DisplayCurrentDensityValue();  //density compensation loop value for this image

		if ((vOldTakingQuickLearnImages) && (!vGlobalProcessQuickLearnImagesThread))
		{
			vTotalToLearn = vLocalConfigurationData->vNumberOfQuickLearnImagesToTake;
			TempText.LoadString(IDS_QuickLearn);
			TempText1.LoadString(IDS_Of);
			CString TempText2;
			TempText2.LoadString(IDS_Product);
			SetStatusLine(TempText + " " + dtoa(vLocalSystemData->vQuickLearnImageIndex,0) + " " + TempText1 + " " + 
				dtoa(vLocalConfigurationData->vNumberOfQuickLearnImagesToTake,0) + " " + TempText2 + ": " + TempStringName);
		}
		else
		if (vOldCalibrateBodyTriggerReferenceEnabled)
		{
			if (PasswordOK(cTemporaryInspxPassword,false))
				vTotalToLearn = vLocalConfigurationData->vCalibrateBodyTriggerReferenceDisableCount;
			else
				vTotalToLearn = vLocalConfigurationData->vCalibrateBodyTriggerReferenceDisableCount + 
					vLocalConfigurationData->vAverageDisableCount + vLocalConfigurationData->vStructureDisableCount;

			CString TempString(dtoa(vLocalSystemData->vITIPCDig->vCalibrateBodyTriggerReferenceCount, 0));
			CString TempText("");
			TempText.LoadString(IDS_Of);
			TempText1.LoadString(IDS_Product);

			if (PasswordOK(cTemporaryInspxPassword,false))
				TempString = TempString + " " + TempText + " " + dtoa(vLocalConfigurationData->vCalibrateBodyTriggerReferenceDisableCount, 0) + " " + TempText1 + ": ";
			else
				TempString = TempString + " " + TempText + " " + dtoa(vTotalToLearn, 0) + " " + TempText1 + ": ";

			if (PasswordOK(cTemporaryInspxPassword,false))
				TempText = "Reference";
			else
			{
				if ((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode))
					TempText = "Send Good Product";
				else
					TempText.LoadString(IDS_SendGoodContainers);
			}
			SetStatusLine(" " + TempText + ": " + TempString + TempStringName);
		}
		else
		if (vLocalSystemData->vITIPCDig->vAverageEnabled)
		{
			if (PasswordOK(cTemporaryInspxPassword,false))
				vTotalToLearn = vLocalConfigurationData->vAverageDisableCount;
			else
				vTotalToLearn = vLocalConfigurationData->vCalibrateBodyTriggerReferenceDisableCount + 
					vLocalConfigurationData->vAverageDisableCount + vLocalConfigurationData->vStructureDisableCount;

			DWORD TempSoFar = vLocalSystemData->vITIPCDig->vAverageCount;
			if (vLearningReference)
				TempSoFar = TempSoFar + vLocalConfigurationData->vCalibrateBodyTriggerReferenceDisableCount;

			CString TempString("");

			if (PasswordOK(cTemporaryInspxPassword,false))
				TempString = dtoa(vLocalSystemData->vITIPCDig->vAverageCount, 0);
			else
				TempString = dtoa(TempSoFar, 0);

			CString TempString1("");
			if (PasswordOK(cTemporaryInspxPassword,false))
				TempString1 = dtoa(vLocalConfigurationData->vAverageDisableCount, 0);
			else
				TempString1 = dtoa(vTotalToLearn, 0);

			TempText.LoadString(IDS_Of);
			TempText1.LoadString(IDS_Product);
			TempString = TempString + " " + TempText + " " + TempString1 + " " + TempText1 + ": ";

			if (PasswordOK(cTemporaryInspxPassword,false))
				TempText = "Average";
			else
			{
				if ((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode))
					TempText = "Send Good Product";
				else
					TempText.LoadString(IDS_SendGoodContainers);
			}

			SetStatusLine(" " + TempText + ": " + TempString + TempStringName);
		}
		else
		if (vOldStructureEnabled)
		{
			if (PasswordOK(cTemporaryInspxPassword,false))
				vTotalToLearn = vLocalConfigurationData->vStructureDisableCount;
			else
				vTotalToLearn = vLocalConfigurationData->vCalibrateBodyTriggerReferenceDisableCount + 
					vLocalConfigurationData->vAverageDisableCount + vLocalConfigurationData->vStructureDisableCount;

			DWORD TempSoFar = vLocalConfigurationData->vAverageDisableCount +
				vLocalSystemData->vITIPCDig->vStructureCount;
			if (vLearningReference)
				TempSoFar = TempSoFar + vLocalConfigurationData->vCalibrateBodyTriggerReferenceDisableCount;

			CString TempString("");

			if (PasswordOK(cTemporaryInspxPassword,false))
				TempString = dtoa(vLocalSystemData->vITIPCDig->vStructureCount, 0);
			else
				TempString = dtoa(TempSoFar, 0);

			CString TempString1("");

			if (PasswordOK(cTemporaryInspxPassword,false))
				TempString1 = dtoa(vLocalConfigurationData->vStructureDisableCount, 0);
			else
				TempString1 = dtoa(vTotalToLearn, 0);

			CString TempText("");
			CString TempText1("");
			CString TempText2("");
			CString TempText3("");
				TempText2.LoadString(IDS_Product);
			if (vShowStructure)
				TempText.LoadString(IDS_Contaminant);
			else
			if (vShowVoidStructure)
				TempText.LoadString(IDS_Void);
			if (PasswordOK(cTemporaryInspxPassword,false))
				TempText = "Structure";
			else
				TempText = "Learning";
			TempText3.LoadString(IDS_Of);
			SetStatusLine(" " + TempText + ": " + 
				TempString + " " + TempText3 + " " + TempString1 + " " + 
				TempText2 + ": " + TempStringName);
		}
		else	
		if (vMode == cSetDensityThreshold)
		{
			if (vInspectionEditing)
			{
				//get density value of this inspection for this container
				double TempDensity = vLocalSystemData->vITIPCDig->vOriginalImage->vDensityValue[vInspectionNumberEditing];
			
				if (TempDensity == 0)
				if ((vInspectionEditing->vInspectionType == cUnderfillByWeightInspection) ||
					(vInspectionEditing->vInspectionType == cCheckWeighInspection) ||
					(vInspectionEditing->vInspectionType == cOverfillByWeightInspection))
				{
					//remove the IV Bag if need to
					if (vInspectionEditing->vWhiteOutAnythingTouchingTheTop)
						vInspectionEditing->WhiteOutAnythingTouchingTheTop(vLocalSystemData->vITIPCDig->vOriginalImage->vGreyImage, vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY);

					if ((vLocalSystemData->vITIPCDig->vOriginalImage) && (vLocalSystemData->vITIPCDig->vOriginalImage->vGreyImage))
						TempDensity = vLocalSystemData->vITIPCDig->CalculateUnderfillByWeight(vLocalSystemData->vITIPCDig->vOriginalImage->vGreyImage, vInspectionEditing, vLocalSystemData->vITIPCDig->vOriginalImage->vLeftReferenceAdjust); 
					if (vInspectionEditing->vCheckWeighOffset)
					{
						double TempWeight = vInspectionEditing->ConvertToWeight(TempDensity) + vInspectionEditing->vCheckWeighOffset;
						TempDensity = vInspectionEditing->ConvertWeightToBrightness(TempWeight);
					}
				}
				else
				if ((vInspectionEditing->vInspectionType == cDensityInspection) ||
					//(vInspectionEditing->vInspectionType == cCheckWeighInspection) ||
					(vInspectionEditing->vInspectionType == cMissingLidInspection) ||
					(vInspectionEditing->vInspectionType == cUnderfillInspection) ||
					(vInspectionEditing->vInspectionType == cOverfillInspection))
				{
					if ((vLocalSystemData->vITIPCDig->vOriginalImage) && (vLocalSystemData->vITIPCDig->vOriginalImage->vGreyImage))
						TempDensity = 
							vLocalSystemData->vITIPCDig->CalculateInspectionDensityAfterReference(
							vLocalSystemData->vITIPCDig->vOriginalImage->vGreyImage,
							vInspectionEditing,
							vLocalSystemData->vITIPCDig->vOriginalImage->vLeftReferenceAdjust,
							vLocalSystemData->vITIPCDig->vOriginalImage->vTopReferenceAdjust,
							vLocalConfigurationData->vReferenceROIsToEdges,
							vLocalSystemData->vITIPCDig->vOriginalImage->vLeft,
							vLocalSystemData->vITIPCDig->vOriginalImage->vRight); 
				}
				else
				{
					if ((vLocalSystemData->vITIPCDig->vOriginalImage) && (vLocalSystemData->vITIPCDig->vOriginalImage->vGreyImage))
					{
						if (vInspectionEditing->vPreSmoothImage)
						{
							CopyMemory(vLocalSystemData->vITIPCDig->vASIDDScratchImage->pix, vLocalSystemData->vITIPCDig->vOriginalImage->vGreyImage, vLocalSystemData->vITIPCDig->vOriginalBufferSize);
							DoMedianSmooth(vLocalSystemData->vITIPCDig->vASIDDScratchImage, vInspectionEditing->vPreSmoothImage, vInspectionEditing->vPreSmoothImage);
							TempDensity = vLocalSystemData->vITIPCDig->CalculateInspectionStandardDeviationAfterReference(
								vLocalSystemData->vITIPCDig->vASIDDScratchImage->pix, vInspectionEditing, vLocalSystemData->vITIPCDig->vOriginalImage->vLeftReferenceAdjust); 
						}
						else
							TempDensity = vLocalSystemData->vITIPCDig->CalculateInspectionStandardDeviationAfterReference(
								vLocalSystemData->vITIPCDig->vOriginalImage->vGreyImage, vInspectionEditing, vLocalSystemData->vITIPCDig->vOriginalImage->vLeftReferenceAdjust); 
					}
				}

				//display density of this container
				SetDlgItemText(IDC_Reference,dtoa(vInspectionEditing->ConvertToWeight(TempDensity),2));

				CString TempString("");
				TempString = "";
				CString TempText("");
				TempText = "";
				if (vInspectionEditing->vCollectingDensityAverage)  //if collecting average
				if (vLocalSystemData->vITIPCDig->vDensityMaximumValue)
				{  //evaluating density
					if (!vCollectingBadDensityAverage)
					{
						vInspectionEditing->vSetupHistoryMinimumGoodStandardDeviation = (float)vLocalSystemData->vITIPCDig->vDensityStandardDeviation;
						vInspectionEditing->vSetupHistoryMinimumGoodAverage = (float)vLocalSystemData->vITIPCDig->vDensityAverage;

						vInspectionEditing->vSetupHistoryMinimumValue = (float)vLocalSystemData->vITIPCDig->vDensityMinimumValue;
						vInspectionEditing->vSetupHistoryMaximumValue = (float)vLocalSystemData->vITIPCDig->vDensityMaximumValue;

						vInspectionEditing->vSetupHistoryMinimumGoodCount = vLocalSystemData->vDensityIndex;
							
						if ((vInspectionEditing->vSetupHistoryMaximumBadCount <= 3) || (vInspectionEditing->vSetupHistoryMinimumGoodCount <= 3))
						{
							//set upper density threshold
							double TempDouble = 0;
							if (vInspectionEditing->vHasDensityUpperThreshold)
							{
								TempDouble = vInspectionEditing->vSetupHistoryMinimumGoodAverage + (6 * vInspectionEditing->vSetupHistoryMinimumGoodStandardDeviation);

								if (TempDouble < 0)
									TempDouble = 0;
								if (TempDouble > 255)
									TempDouble = 255;

								if (TempDouble)
								{
									vTookNewDensityData = true;
									vAutomaticDensityThresholdUpper = TempDouble;
									vInspectionEditing->vSetupHistoryDensityAutoThresholdUpper = (float)TempDouble;
									if (!vChangeMade)
									{
										//vInspectionEditing->vDensityThresholdUpper = TempDouble;
										if ((!vLocalConfigurationData->vEnableEjectors) || (vLocalSystemData->vInAutoSetup))
										{
											if ((vInspectionEditing->vInspectionType != cUnderfillByWeightInspection) && (vInspectionEditing->vInspectionType != cOverfillByWeightInspection) && (vInspectionEditing->vInspectionType != cCheckWeighInspection))
												vInspectionEditing->vDensityThresholdUpper = (float)TempDouble;
										}
										else
										if (vInspectionEditing->vDensityThresholdUpper != vInspectionEditing->vSetupHistoryDensityAutoThresholdUpper)
										{
											vHaveAnAutoThresholdValue = true;
											UpdateFunction3Button();
										}
									}
								}
							}

							//set lower density threshold
							if ((vInspectionEditing->vHasDensityLowerThreshold) || 
								(vInspectionEditing->vInspectionType == cCheckWeighInspection) || (vInspectionEditing->vInspectionType == cUnderfillByWeightInspection) || (vInspectionEditing->vInspectionType == cOverfillByWeightInspection))
							{
								TempDouble = vInspectionEditing->vSetupHistoryMinimumGoodAverage - (6 * vInspectionEditing->vSetupHistoryMinimumGoodStandardDeviation);

								if (TempDouble < 0)
									TempDouble = 0;
								if (vInspectionEditing->vInspectionType != cUnderfillByWeightInspection)
								if (vInspectionEditing->vInspectionType != cOverfillByWeightInspection)
								if (vInspectionEditing->vInspectionType != cCheckWeighInspection)
								if (TempDouble > 255)
									TempDouble = 255;

								if (TempDouble)
								{
									vTookNewDensityData = true;
									vAutomaticDensityThresholdLower = (float)TempDouble;
									vInspectionEditing->vSetupHistoryDensityAutoThresholdLower = (float)TempDouble;
									if (!vChangeMade)
									{
										if ((!vLocalConfigurationData->vEnableEjectors) || (vLocalSystemData->vInAutoSetup))
										{
											vInspectionEditing->SetDensityLowerThreshold(TempDouble);
										}
										else
										if (vInspectionEditing->vDensityThresholdLower != vInspectionEditing->vSetupHistoryDensityAutoThresholdLower)
										{
											vHaveAnAutoThresholdValue = true;
											UpdateFunction3Button();
										}
									}
								}
							}
						}
					}
					else
					{// collecting bad density data
						vInspectionEditing->vSetupHistoryMaximumBadStandardDeviation = (float)vLocalSystemData->vITIPCDig->vDensityStandardDeviation;
						vInspectionEditing->vSetupHistoryMaximumBadAverage = (float)vLocalSystemData->vITIPCDig->vDensityAverage;

						vInspectionEditing->vSetupHistoryBadMinimumValue = (float)vLocalSystemData->vITIPCDig->vDensityMinimumValue;
						vInspectionEditing->vSetupHistoryBadMaximumValue = (float)vLocalSystemData->vITIPCDig->vDensityMaximumValue;

						vInspectionEditing->vSetupHistoryMaximumBadCount = vLocalSystemData->vDensityIndex;
					}

					if (vInspectionEditing->vCollectingDensityAverage)  //if collecting average
					if (vLocalSystemData->vITIPCDig->vDensityMaximumValue)
					if ((vInspectionEditing->vSetupHistoryMaximumBadCount > 3) && (vInspectionEditing->vSetupHistoryMinimumGoodCount > 3))
					{
						float TempUpperThreshold = 0;
						if (vInspectionEditing->vHasDensityUpperThreshold)
						if ((vInspectionEditing->vInspectionType == cMissingLidInspection) || (vInspectionEditing->vInspectionType == cCheckWeighInspection) ||
							(vInspectionEditing->vInspectionType == cDensityInspection) || (vInspectionEditing->vInspectionType == cUnderfillInspection))
						{
							TempUpperThreshold = vInspectionEditing->vSetupHistoryMinimumGoodAverage + (6 * vInspectionEditing->vSetupHistoryMinimumGoodStandardDeviation);

							double TempQualityFactor = 0;
							if (vInspectionEditing->vSetupHistoryMinimumGoodStandardDeviation + vInspectionEditing->vSetupHistoryMaximumBadStandardDeviation)
								TempQualityFactor = (vInspectionEditing->vSetupHistoryMinimumGoodAverage - vInspectionEditing->vSetupHistoryMaximumBadAverage) /
									(vInspectionEditing->vSetupHistoryMinimumGoodStandardDeviation + vInspectionEditing->vSetupHistoryMaximumBadStandardDeviation);

							if (TempQualityFactor < 0)
								TempQualityFactor = -TempQualityFactor;
							vLocalSystemData->vQualityFactor = TempQualityFactor;

							if (TempQualityFactor >= 1)
								TempUpperThreshold = (float)(vInspectionEditing->vSetupHistoryMinimumGoodAverage + (TempQualityFactor * vInspectionEditing->vSetupHistoryMinimumGoodStandardDeviation));

							if (TempUpperThreshold < 0)
								TempUpperThreshold = 0;
							if (TempUpperThreshold > 255)
								TempUpperThreshold = 255;

							if (vInspectionEditing->vInspectionType == cCheckWeighInspection)
							{
								//figure out high fill or lower threshold
								TempUpperThreshold = (float)(vInspectionEditing->vSetupHistoryMaximumBadAverage - (3 * vInspectionEditing->vSetupHistoryMaximumBadStandardDeviation));
								if (TempUpperThreshold < 0)
									TempUpperThreshold = 0;
								if (TempUpperThreshold > 255)
									TempUpperThreshold = 255;

								if (TempUpperThreshold)
								{
									vTookNewDensityData = true;
									vAutomaticDensityThresholdLower = TempUpperThreshold;
									vInspectionEditing->vSetupHistoryDensityAutoThresholdLower = TempUpperThreshold;
									if (!vChangeMade)
									{
										//vInspectionEditing->SetDensityLowerThreshold(TempDouble);
										if ((!vLocalConfigurationData->vEnableEjectors) || (vLocalSystemData->vInAutoSetup))
											vInspectionEditing->SetDensityLowerThreshold(TempUpperThreshold);
										else
										if (vInspectionEditing->vDensityThresholdLower != vInspectionEditing->vSetupHistoryDensityAutoThresholdLower)
										{
											vHaveAnAutoThresholdValue = true;
											UpdateFunction3Button();
										}
									}
								}
								//figure out low fill or Upper threshold
								TempUpperThreshold = (float)(vInspectionEditing->vSetupHistoryMinimumGoodAverage + (3 * vInspectionEditing->vSetupHistoryMinimumGoodStandardDeviation));
								if (TempUpperThreshold < 0)
									TempUpperThreshold = 0;
								if (TempUpperThreshold > 255)
									TempUpperThreshold = 255;
							}

							if (TempUpperThreshold)
							{
								vTookNewDensityData = true;
								vAutomaticDensityThresholdUpper = TempUpperThreshold;
								vInspectionEditing->vSetupHistoryDensityAutoThresholdUpper = TempUpperThreshold;
								if (!vChangeMade)
								{
									if ((vInspectionEditing->vInspectionType != cUnderfillByWeightInspection) && (vInspectionEditing->vInspectionType != cOverfillByWeightInspection) && (vInspectionEditing->vInspectionType != cCheckWeighInspection))
									if ((!vLocalConfigurationData->vEnableEjectors) || (vLocalSystemData->vInAutoSetup))
									{
										vInspectionEditing->vDensityThresholdUpper = TempUpperThreshold;
									}
									else
									if (vInspectionEditing->vDensityThresholdUpper != vInspectionEditing->vSetupHistoryDensityAutoThresholdUpper)
									{
										vHaveAnAutoThresholdValue = true;
										UpdateFunction3Button();
									}
								}
							}
						}

						//set density threshold with good and bad evaluate
						if (vInspectionEditing->vHasDensityLowerThreshold)
						if ((vInspectionEditing->vInspectionType == cOverfillInspection) || (vInspectionEditing->vInspectionType == cDensityInspection) || (vInspectionEditing->vInspectionType == cCheckWeighInspection))
						{
							double TempLowerThreshold = vInspectionEditing->vSetupHistoryMinimumGoodAverage - (6 * vInspectionEditing->vSetupHistoryMinimumGoodStandardDeviation);

							double TempQualityFactor = 0;

							if (vInspectionEditing->vSetupHistoryMinimumGoodStandardDeviation + vInspectionEditing->vSetupHistoryMaximumBadStandardDeviation)
								TempQualityFactor =  (vInspectionEditing->vSetupHistoryMinimumGoodAverage - vInspectionEditing->vSetupHistoryMaximumBadAverage) /
									(vInspectionEditing->vSetupHistoryMinimumGoodStandardDeviation + vInspectionEditing->vSetupHistoryMaximumBadStandardDeviation);

							if (TempQualityFactor < 0)
								TempQualityFactor = -TempQualityFactor;
							vLocalSystemData->vQualityFactor = TempQualityFactor;

							if (TempQualityFactor >= 1)
								TempLowerThreshold = (float)(vInspectionEditing->vSetupHistoryMinimumGoodAverage - (TempQualityFactor * vInspectionEditing->vSetupHistoryMinimumGoodStandardDeviation));

							if (TempLowerThreshold < 0)
								TempLowerThreshold = 0;
							if (TempLowerThreshold > 255)
								TempLowerThreshold = 255;

							if (TempLowerThreshold)
							{
								vTookNewDensityData = true;
								vAutomaticDensityThresholdLower = TempLowerThreshold;
								vInspectionEditing->vSetupHistoryDensityAutoThresholdLower = (float)TempLowerThreshold;
								if (!vChangeMade)
								{
									if ((!vLocalConfigurationData->vEnableEjectors) || (vLocalSystemData->vInAutoSetup))
									{
										vInspectionEditing->SetDensityLowerThreshold(TempLowerThreshold);
									}
									else
									if (vInspectionEditing->vDensityThresholdLower != vInspectionEditing->vSetupHistoryDensityAutoThresholdLower)
									{
										vHaveAnAutoThresholdValue = true;
										UpdateFunction3Button();
									}
								}
							}
						}
					}
					else
						vLocalSystemData->vQualityFactor = 0;
/*
					//use good and bad numbers to calculate density threshold and quality factor
					if (vInspectionEditing->vCollectingDensityAverage)  //if collecting average
					if (vLocalSystemData->vITIPCDig->vDensityMaximumValue)
					if ((vInspectionEditing->vSetupHistoryMaximumBadCount > 3) &&
						(vInspectionEditing->vSetupHistoryMinimumGoodCount > 3))
					{
						double TempDouble = 0;
						if (vInspectionEditing->vHasDensityUpperThreshold)
						if ((vInspectionEditing->vInspectionType == cMissingLidInspection) || (vInspectionEditing->vInspectionType == cCheckWeighInspection) || 
							(vInspectionEditing->vInspectionType == cDensityInspection) ||
							(vInspectionEditing->vInspectionType == cUnderfillByWeightInspection) || (vInspectionEditing->vInspectionType == cOverfillByWeightInspection))
						{
							TempDouble = vInspectionEditing->vSetupHistoryMinimumGoodAverage + (6 * vInspectionEditing->vSetupHistoryMinimumGoodStandardDeviation);


							double TempQualityFactor = 0;
							if (vInspectionEditing->vSetupHistoryMinimumGoodStandardDeviation + vInspectionEditing->vSetupHistoryMaximumBadStandardDeviation)
								TempQualityFactor = (vInspectionEditing->vSetupHistoryMinimumGoodAverage - vInspectionEditing->vSetupHistoryMaximumBadAverage) / 
									(vInspectionEditing->vSetupHistoryMinimumGoodStandardDeviation + vInspectionEditing->vSetupHistoryMaximumBadStandardDeviation);
							if (TempQualityFactor < 0)
								TempQualityFactor = -TempQualityFactor;
							vLocalSystemData->vQualityFactor = TempQualityFactor;

							if (TempQualityFactor >= 1)
							{
								TempDouble = (float)(vInspectionEditing->vSetupHistoryMinimumGoodAverage + (TempQualityFactor * vInspectionEditing->vSetupHistoryMinimumGoodStandardDeviation));

								if (TempDouble < 0)
									TempDouble = 0;
								if (TempDouble > 255)
									TempDouble = 255;

								if ((vInspectionEditing->vInspectionType == cCheckWeighInspection) || (vInspectionEditing->vInspectionType == cUnderfillByWeightInspection) || (vInspectionEditing->vInspectionType == cOverfillByWeightInspection))
								{
									//figure out high fill or lower threshold
									TempDouble = (float)(vInspectionEditing->vSetupHistoryMaximumBadAverage - (3 * vInspectionEditing->vSetupHistoryMaximumBadStandardDeviation));
									if (TempDouble < 0)
										TempDouble = 0;
									if (TempDouble > 255)
										TempDouble = 255;

									if (TempDouble)
									{
										vTookNewDensityData = true;
										vAutomaticDensityThresholdLower = TempDouble;
										vInspectionEditing->vSetupHistoryDensityAutoThresholdLower = (float)TempDouble;
										if (!vChangeMade)
										{
											//vInspectionEditing->SetDensityLowerThreshold(TempDouble);
											if ((!vLocalConfigurationData->vEnableEjectors) || (vLocalSystemData->vInAutoSetup))
											{
												vInspectionEditing->SetDensityLowerThreshold(TempDouble);
											}
											else
											if (vInspectionEditing->vDensityThresholdLower != vInspectionEditing->vSetupHistoryDensityAutoThresholdLower)
											{
												vHaveAnAutoThresholdValue = true;
												UpdateFunction3Button();
											}
										}
									}
									//figure out low fill or Upper threshold
									TempDouble = (float)(vInspectionEditing->vSetupHistoryMinimumGoodAverage + (3 * vInspectionEditing->vSetupHistoryMinimumGoodStandardDeviation));
									if (TempDouble < 0)
										TempDouble = 0;
									if (TempDouble > 255)
										TempDouble = 255;

								}
							}

							if (TempDouble)
							{
								vTookNewDensityData = true;
								vAutomaticDensityThresholdUpper = TempDouble;
								vInspectionEditing->vSetupHistoryDensityAutoThresholdUpper = (float)TempDouble;
								if (!vChangeMade)
								{
									//vInspectionEditing->vDensityThresholdUpper = TempDouble;
									if ((!vLocalConfigurationData->vEnableEjectors) ||
										(vLocalSystemData->vInAutoSetup))
									{
										vInspectionEditing->vDensityThresholdUpper = (float)TempDouble;
									}
									else
									if (vInspectionEditing->vDensityThresholdUpper != vInspectionEditing->vSetupHistoryDensityAutoThresholdUpper)
									{
										vHaveAnAutoThresholdValue = true;
										UpdateFunction3Button();
									}
								}
							}
						}

						//set density threshold with good and bad evaluate
						if (vInspectionEditing->vHasDensityLowerThreshold)
						if ((vInspectionEditing->vInspectionType == cOverfillInspection) ||
							(vInspectionEditing->vInspectionType == cDensityInspection))
						{
							TempDouble = vInspectionEditing->vSetupHistoryMinimumGoodAverage - (6 * vInspectionEditing->vSetupHistoryMinimumGoodStandardDeviation);

							double TempQualityFactor = 
								(vInspectionEditing->vSetupHistoryMinimumGoodAverage -
								vInspectionEditing->vSetupHistoryMaximumBadAverage) /
								(vInspectionEditing->vSetupHistoryMinimumGoodStandardDeviation +
								vInspectionEditing->vSetupHistoryMaximumBadStandardDeviation);
							if (TempQualityFactor >= 1)
							{
								TempDouble = (float)(vInspectionEditing->vSetupHistoryMinimumGoodAverage -
									(TempQualityFactor * 
									vInspectionEditing->vSetupHistoryMinimumGoodStandardDeviation));
							}
							if (TempQualityFactor < 0)
								TempQualityFactor = -TempQualityFactor;
							vLocalSystemData->vQualityFactor = TempQualityFactor;

							if (TempDouble < 0)
								TempDouble = 0;
							if (TempDouble > 255)
								TempDouble = 255;

							if (TempDouble)
							{
								vTookNewDensityData = true;
								vAutomaticDensityThresholdLower = TempDouble;
								vInspectionEditing->vSetupHistoryDensityAutoThresholdLower = (float)TempDouble;
								if (!vChangeMade)
								{
									//vInspectionEditing->SetDensityLowerThreshold(TempDouble);
									if ((!vLocalConfigurationData->vEnableEjectors) ||
										(vLocalSystemData->vInAutoSetup))
									{
										vInspectionEditing->SetDensityLowerThreshold(TempDouble);
									}
									else
									if (vInspectionEditing->vDensityThresholdLower != vInspectionEditing->vSetupHistoryDensityAutoThresholdLower)
									{
										vHaveAnAutoThresholdValue = true;
										UpdateFunction3Button();
									}
								}
							}
						}
					}
					else
						vLocalSystemData->vQualityFactor = 0;
						*/
				}  //end of evaluate density

				bool TempSetStatusLine = false;
				if ((vInspectionEditing->vInspectionType == cCheckWeighInspection) || (vInspectionEditing->vInspectionType == cUnderfillByWeightInspection) || 
					(vInspectionEditing->vInspectionType == cOverfillByWeightInspection))
				{
					if ((vInspectionEditing->vCheckWeighMinimumGoodWeight) && (vInspectionEditing->vCheckWeighMaximumGoodWeight))
					{
						bool TempOldDisplayInDensity = true;
						if ((vInspectionEditing->vCheckWeighMFactor != 1) || (vInspectionEditing->vCheckWeighBFactor))
							TempOldDisplayInDensity = false;

						if (vInspectionEditing->vSetupHistoryMinimumGoodCount > 3)
						if (vInspectionEditing->vSetupHistoryMaximumBadCount > 3)
						if (vInspectionEditing->vSetupHistoryMinimumGoodAverage)
						if (vInspectionEditing->vSetupHistoryMaximumBadAverage)
						{
							if ((vInspectionEditing->vInspectionType == cCheckWeighInspection) || (vInspectionEditing->vInspectionType == cUnderfillByWeightInspection) || (vInspectionEditing->vInspectionType == cOverfillByWeightInspection))
							{
								//if (vInspectionEditing->vSetupHistoryMaximumBadAverage - (2 * vInspectionEditing->vSetupHistoryMaximumBadStandardDeviation) < vInspectionEditing->vSetupHistoryMinimumGoodAverage)
								if (vInspectionEditing->vSetupHistoryMaximumBadAverage - (2 * vInspectionEditing->vSetupHistoryMaximumBadStandardDeviation) < vInspectionEditing->vSetupHistoryMinimumGoodAverage)
								{
									if (vInspectionEditing->vCollectingDensityAverage)  //if evaluating
									{
										SetStatusLine(" Error-Heavy weight container not heavier then light one");
										TempSetStatusLine = true;
									}
								}
								else
								{
									double TempOriginalThresholdLower = vInspectionEditing->vDensityThresholdLower;
									if (TempOriginalThresholdLower > 5000)
										TempOriginalThresholdLower = vInspectionEditing->ConvertToWeight(TempOriginalThresholdLower);

									double TempOriginalThresholdUpper = vInspectionEditing->vDensityThresholdUpper;
									if (TempOriginalThresholdUpper > 5000)
										TempOriginalThresholdUpper = vInspectionEditing->ConvertToWeight(TempOriginalThresholdUpper);
									//now we have the threshold the customer entered in weight

									vInspectionEditing->vCheckWeighMFactor = (vInspectionEditing->vCheckWeighMinimumGoodWeight - vInspectionEditing->vCheckWeighMaximumGoodWeight) /
										(vInspectionEditing->vSetupHistoryMinimumGoodAverage - vInspectionEditing->vSetupHistoryMaximumBadAverage);
									vInspectionEditing->vCheckWeighOffset = 0;
												
									if ((vInspectionEditing->vSetupHistoryMaximumBadAverage) && (vInspectionEditing->vCheckWeighMFactor))
										vInspectionEditing->vCheckWeighBFactor = vInspectionEditing->vCheckWeighMinimumGoodWeight - (vInspectionEditing->vSetupHistoryMinimumGoodAverage * vInspectionEditing->vCheckWeighMFactor);

									if (vInspectionEditing->vHasDensityLowerThreshold)
									if ((TempOriginalThresholdLower) && (TempOriginalThresholdLower < 5000))
										vInspectionEditing->SetDensityLowerThreshold((float)vInspectionEditing->ConvertWeightToBrightness(TempOriginalThresholdLower));

									if (vInspectionEditing->vHasDensityUpperThreshold)
									if ((TempOriginalThresholdUpper) && (TempOriginalThresholdUpper < 5000))
										vInspectionEditing->vDensityThresholdUpper = (float)vInspectionEditing->ConvertWeightToBrightness(TempOriginalThresholdUpper);

									if (PasswordOK(cTemporaryInspxPassword,false))
										SetStatusLine(vInspectionEditing->vName + " " + dtoa(vLocalSystemData->vDensityIndex,0) + ": " + vInspectionEditing->vCheckWeighUnits + "=Density*" +
											dtoa(vInspectionEditing->vCheckWeighMFactor,6) + "+" + dtoa(vInspectionEditing->vCheckWeighBFactor,2));
									else
										SetStatusLine(vInspectionEditing->vName + " Weight calibration complete");
									TempSetStatusLine = true;

									if (vInspectionEditing->vCollectingDensityAverage)  //if collecting average
									{
										vAutomaticDensityThresholdUpper = vInspectionEditing->vSetupHistoryMinimumGoodAverage;
										vAutomaticDensityThresholdLower = vInspectionEditing->vSetupHistoryMaximumBadAverage;

										vInspectionEditing->vSetupHistoryDensityAutoThresholdUpper = (float)vAutomaticDensityThresholdUpper;
										vInspectionEditing->vSetupHistoryDensityAutoThresholdLower = (float)vAutomaticDensityThresholdLower;
									}
								}
							}
						}
							
						if ((vInspectionEditing->vCheckWeighMFactor != 1) || (vInspectionEditing->vCheckWeighBFactor))
						{
							SetDlgItemText(IDC_Reference,dtoa(vInspectionEditing->ConvertToWeight(TempDensity),2));

							//display upper threshold and auto threshold
							if (vInspectionEditing->vHasDensityUpperThreshold)
							{
								DisplayAutoDensityThresholdUpper();
								if (TempOldDisplayInDensity)
									DisplayDensityThresholdUpper();
							}
							//display Lower threshold and auto threshold
							if (vInspectionEditing->vHasDensityLowerThreshold)
							{
								DisplayAutoDensityThresholdLower();
								if (TempOldDisplayInDensity)
									DisplayDensityThresholdLower();
							}
						}
					}
				}//end Weight Trending
				if (vInspectionEditing->vCollectingDensityAverage)  //if collecting average
				{
					DisplayMinimumGoodMaximumBadDataValues(TempSetStatusLine);
					if (!vCollectingBadDensityAverage)
					{
						if (!vOldDensityAverageValue)
						if (vInspectionEditing->vSetupHistoryMinimumGoodAverage)
							UpdateButtonText();
						vOldDensityAverageValue = vInspectionEditing->vSetupHistoryMinimumGoodAverage;
					}
					else
					{
						if (!vOldDensityAverageValue)
						if (vInspectionEditing->vSetupHistoryMaximumBadAverage)
							UpdateButtonText();
						vOldDensityAverageValue = vInspectionEditing->vSetupHistoryMaximumBadAverage;
					}
				}


				//display upper threshold and auto threshold
				if (vInspectionEditing->vHasDensityUpperThreshold)
				{
					DisplayAutoDensityThresholdUpper();
					if (!vChangeMade)
					{
						DisplayDensityThresholdUpper();
					}
				}
				//display Lower threshold and auto threshold
				if (vInspectionEditing->vHasDensityLowerThreshold)
				{
					DisplayAutoDensityThresholdLower();
					if (!vChangeMade)
					{
						DisplayDensityThresholdLower();
					}
				}  //end of evaluate density

				if (vOldCollectingDensityAverage != vInspectionEditing->vCollectingDensityAverage)
				{
					//if (vOldCollectingDensityAverage) //was on, just changing to off
					//{
					//	TempText.LoadString(IDS_Product);
					//	SetStatusLine(" " + TempText + ": " + TempStringName);
					//}
					vOldCollectingDensityAverage = vInspectionEditing->vCollectingDensityAverage;
					UpdateButtonText();
				}
				if (vOldQualityFactor != vLocalSystemData->vQualityFactor)
				{
					vOldQualityFactor = vLocalSystemData->vQualityFactor;
					if (vLocalSystemData->vQualityFactor == 0)
						SetDlgItemText(IDC_Simulating, _T(""));
					else
					{
						CString TempString = dtoa(vLocalSystemData->vQualityFactor,2);
						TempText.LoadString(IDS_QF);
						SetDlgItemText(IDC_Simulating,TempText + ": " + TempString);
					}
				}
			}
		}
		else	
		if (((vMode == cSetContaminantThreshold) || (vMode == cSetVoidThreshold)) && (vInspectionEditing))
		{
			if (vInspectionEditing)
			{
				if (vInspectionEditing->vFindGoodIntensity)
				{
					if ((vInspectionEditing->vInspectionType == cItemCountInspection) || (vInspectionEditing->vInspectionType == cSizeInspection) || (vInspectionEditing->vInspectionType == cMissingLidInspectionOnSize))
					if (vLocalSystemData->vMinimumGoodIndex < 10)
					{
						SetDlgItemText(IDC_ThresholdForContainerBounds2, dtoa(vInspectionEditing->vThreshold,0));
					}

					if ((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode))
						TempText = "Send Good Product";
					else
						TempText.LoadString(IDS_SendGoodContainers);

					CString TempText1("");
					TempText1.LoadString(IDS_Product);
					if (vLocalSystemData->vITIPCDig->vDriftCompensationLocked)
						SetStatusLine(" " + TempText + ": " + dtoa(vLocalSystemData->vMinimumGoodIndex,0) + ", " + TempText1 + ": " + vProductEditingName);

					if ((vLocalSystemData->vFrameCount > 100) && (vLocalSystemData->vFrameCount < 200))
					{
						CWnd *TempWindow = CWnd::GetDlgItem(IDC_Function5Button);
						if (TempWindow)
							TempWindow->Invalidate(false);
					}
				}

				if ((vInspectionEditing) && ((vInspectionEditing->vInspectionType == cItemCountInspection) || (vInspectionEditing->vInspectionType == cSizeInspection) || (vInspectionEditing->vInspectionType == cMissingLidInspectionOnSize)))
				{
					CString TempString = dtoa(vLocalSystemData->vITIPCDig->vOriginalImage->vNumberOfBlobs[vInspectionNumberEditing],1);
					SetDlgItemText(IDC_Reference,TempString);
				} //end of missing lid or under fill types of inspections

				if (vInspectionEditing->vFindGoodIntensity)
				{
					vOldMinimumGoodCount = vLocalSystemData->vFrameCount;

					CString TempString = dtoa(vInspectionEditing->ConvertToWeight(vLocalSystemData->vITIPCDig->vOriginalImage->vMinimumGoodMaximumBad),1);
					if ((vInspectionEditing->vInspectionType != cItemCountInspection) && (vInspectionEditing->vInspectionType != cSizeInspection) && (vInspectionEditing->vInspectionType != cMissingLidInspectionOnSize))
						SetDlgItemText(IDC_Reference,TempString);
				}
				if (vInspectionEditing->vFindBadIntensity)
				{
					CString TempText("");

					if ((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode))
						TempText = "Send Bad Product";
					else
						TempText = "Send Bad Containers";

					CString TempText1("");
					TempText1.LoadString(IDS_Product);
					if (vLocalSystemData->vITIPCDig->vDriftCompensationLocked)
						SetStatusLine(" " + TempText + ": " + dtoa(vLocalSystemData->vMaximumBadCount,0) + ", " + TempText1 + ": " + vProductEditingName);
						
					CString TempString = dtoa(vInspectionEditing->ConvertToWeight(vLocalSystemData->vITIPCDig->vOriginalImage->vMinimumGoodMaximumBad),1);
					if ((vInspectionEditing->vInspectionType != cItemCountInspection) && (vInspectionEditing->vInspectionType != cSizeInspection) && (vInspectionEditing->vInspectionType != cMissingLidInspectionOnSize))
						SetDlgItemText(IDC_Reference,TempString);
				}
				if (vInspectionEditing->vFindBadIntensity)
					vOldMaximumBadCount = vLocalSystemData->vFrameCount;

				if (vOldMinimumGoodIntensity != vLocalSystemData->vMinimumGoodIntensity)
				{
					if ((vOldMinimumGoodIntensity == 256) || (vOldMinimumGoodIntensity == 999999999))
						UpdateButtonText();
					vOldMinimumGoodIntensity = vLocalSystemData->vMinimumGoodIntensity;
					CString TempText("");
					TempText.LoadString(IDS_Min);
					if ((vOldMinimumGoodIntensity > 5000) && (vInspectionEditing->vInspectionType != cItemCountInspection) && (vInspectionEditing->vInspectionType != cSizeInspection) && (vInspectionEditing->vInspectionType != cMissingLidInspectionOnSize))
						SetDlgItemText(IDC_SubFunction6Button3,TempText + ": " + dtoa(vInspectionEditing->ConvertToWeight(vOldMinimumGoodIntensity),0));
					else
					if (vOldMinimumGoodIntensity < 999999999)
						SetDlgItemText(IDC_SubFunction6Button3,TempText + ": " + dtoa(vOldMinimumGoodIntensity, 0));

					if (vInspectionEditing->vInspectionType == cJarBottomContaminantInspection)
					{
						BYTE TempAutoThreshold = 	(BYTE)(vLocalSystemData->vMinimumGoodIntensity - 4);
						/*
						if (vLocalSystemData->vMinimumGoodIndex > 20)
							TempAutoThreshold = 	(BYTE)(vLocalSystemData->vMinimumGoodIntensity - 15);
						if (vLocalSystemData->vMinimumGoodIndex > 100)
							TempAutoThreshold = 	(BYTE)(vLocalSystemData->vMinimumGoodIntensity - 10);
						if (vLocalSystemData->vMinimumGoodIndex > 200)
							TempAutoThreshold = 	(BYTE)(vLocalSystemData->vMinimumGoodIntensity - 5);
						if (vLocalSystemData->vMinimumGoodIndex > 500)
							TempAutoThreshold = 	(BYTE)(vLocalSystemData->vMinimumGoodIntensity - 2);
							*/

						vInspectionEditing->vSetupHistoryAutoThreshold = TempAutoThreshold;

						SetDlgItemText(IDC_Width2,dtoa(vInspectionEditing->ConvertToWeight(TempAutoThreshold), 0));
						if (!vChangeMade)
						{
							if ((!vLocalConfigurationData->vEnableEjectors) || (vLocalSystemData->vInAutoSetup))
							{
								vInspectionEditing->vThreshold = TempAutoThreshold;
		
								SetDlgItemText(IDC_ThresholdForContainerBounds2, dtoa(vInspectionEditing->ConvertToWeight(vInspectionEditing->vThreshold),0));
							}
							else
							if (vInspectionEditing->vSetupHistoryAutoThreshold != vInspectionEditing->vThreshold)
							{
								if (vMode == cSetContaminantThreshold)
									ShowSensitivity();
								if ((vInspectionEditing->vInspectionType != cItemCountInspection) && (vInspectionEditing->vInspectionType != cSizeInspection) && (vInspectionEditing->vInspectionType != cMissingLidInspectionOnSize))
									vHaveAnAutoThresholdValue = true;
								UpdateFunction3Button();
							}
						}
					}
				}
				if (vInspectionEditing->vInspectionType == cJarBottomContaminantInspection)
					SetDlgItemText(IDC_ThresholdForContainerBounds2, dtoa(vInspectionEditing->vThreshold,0));

				if (!vLocalSystemData->vMaximumBadCount)
					vLocalSystemData->vMaximumBadIntensity = 0;

				//figure out contaminant auto threshold if run good and bad
				if (((vLocalSystemData->vMaximumBadAverage) || ((vInspectionEditing) && ((vInspectionEditing->vInspectionType == cItemCountInspection) || (vInspectionEditing->vInspectionType == cSizeInspection) || (vInspectionEditing->vInspectionType == cMissingLidInspectionOnSize)))) && 
					(vLocalSystemData->vMinimumGoodAverage))
				if (vLocalSystemData->vMinimumGoodIntensity > vLocalSystemData->vMaximumBadIntensity)
				{
					double TempDifference = vLocalSystemData->vMinimumGoodIntensity - vLocalSystemData->vMaximumBadIntensity;

					WORD TempAutoThreshold = 0;
					if ((vInspectionEditing) && ((vInspectionEditing->vInspectionType == cItemCountInspection) || (vInspectionEditing->vInspectionType == cSizeInspection) || (vInspectionEditing->vInspectionType == cMissingLidInspectionOnSize)))
					{
						TempAutoThreshold = (WORD)(vLocalSystemData->vMinimumGoodIntensity - (TempDifference / 4));
						if ((vLocalSystemData->vMinimumGoodIntensity - (3 * vLocalSystemData->vMinimumGoodStandardDeviation) > TempAutoThreshold) && (vLocalSystemData->vMinimumGoodStandardDeviation > 1))
							TempAutoThreshold = (WORD)(vLocalSystemData->vMinimumGoodIntensity - (3 * vLocalSystemData->vMinimumGoodStandardDeviation));
					}
					else
						TempAutoThreshold = (WORD)(vLocalSystemData->vMaximumBadIntensity + (TempDifference / 2.0) + .5);

					if (TempAutoThreshold > 254)
						TempAutoThreshold = 254;

					if ((vLocalSystemData->vMaximumBadCount > 4) && (vLocalSystemData->vMinimumGoodIndex > 4))
						vLocalSystemData->CalculateQualityFactor();

					if (vLocalSystemData->vQualityFactor >= 1)
					{
						//if always got same value set SD to .6 to give a reasonable margin
						if (vLocalSystemData->vMinimumGoodIndex > 4)  //minimum number of containers before evaluate statistics shown
						if (vLocalSystemData->vMaximumBadStandardDeviation < .6)
							vLocalSystemData->vMaximumBadStandardDeviation = (float)(.6);

						//if always got same value set SD to .6 to give a reasonable margin
						if (vLocalSystemData->vMinimumGoodIndex > 4)  //minimum number of containers before evaluate statistics shown
						if (vLocalSystemData->vMinimumGoodStandardDeviation < .6)
							vLocalSystemData->vMinimumGoodStandardDeviation = (float)(.6);

						TempDifference = vLocalSystemData->vMinimumGoodIntensity - (5.0 * vLocalSystemData->vMinimumGoodStandardDeviation) - 
							(vLocalSystemData->vMaximumBadIntensity + (3.0 * vLocalSystemData->vMaximumBadStandardDeviation));

						if ((vInspectionEditing) && ((vInspectionEditing->vInspectionType == cItemCountInspection) || (vInspectionEditing->vInspectionType == cSizeInspection) || (vInspectionEditing->vInspectionType == cMissingLidInspectionOnSize)))
						{
							TempAutoThreshold = (WORD)(vLocalSystemData->vMaximumBadIntensity + ((vLocalSystemData->vMinimumGoodIntensity - vLocalSystemData->vMaximumBadIntensity) / 4));
							vInspectionEditing->vSize = TempAutoThreshold;
						}
						else
							TempAutoThreshold = (BYTE)(vLocalSystemData->vMaximumBadIntensity + (TempDifference / 2) + (3.0 * vLocalSystemData->vMaximumBadStandardDeviation) + .5);
					}
											
					if ((vInspectionEditing->vFindGoodIntensity) || (vInspectionEditing->vFindBadIntensity))
					{
						vInspectionEditing->vSetupHistoryAutoThreshold = (BYTE)TempAutoThreshold;

						SetDlgItemText(IDC_Width2,dtoa(vInspectionEditing->ConvertToWeight(TempAutoThreshold), 0));
						if (!vChangeMade)
						{
							if (((!vLocalConfigurationData->vEnableEjectors) || (vLocalSystemData->vInAutoSetup)) ||
								((vInspectionEditing) && ((vInspectionEditing->vInspectionType == cItemCountInspection) || (vInspectionEditing->vInspectionType == cSizeInspection) || (vInspectionEditing->vInspectionType == cMissingLidInspectionOnSize))))
							{
								if ((vInspectionEditing) && ((vInspectionEditing->vInspectionType == cItemCountInspection) || (vInspectionEditing->vInspectionType == cSizeInspection) || (vInspectionEditing->vInspectionType == cMissingLidInspectionOnSize)))
								{
									vInspectionEditing->vSize = TempAutoThreshold; //for size inspection, set the minimum size, not threshold from evaluate
									if (vInspectionEditing->vSize < 2)
										vInspectionEditing->vSize = 2;
									if (vInspectionEditing->vInspectionType != cItemCountInspection)
										SetDlgItemText(IDC_Function1Button, "Size\n" + dtoa(vInspectionEditing->vSize, 0));
								}
								else
								if (vInspectionEditing->vInspectionType != cItemCountInspection)
								{ //all contaminant inspections
									vInspectionEditing->vThreshold = (BYTE)TempAutoThreshold;
									SetDlgItemText(IDC_ThresholdForContainerBounds2, dtoa(vInspectionEditing->ConvertToWeight(vInspectionEditing->vThreshold),0));
									if (vMode == cSetContaminantThreshold)
										ShowSensitivity();
								}
							}
							else
							if ((vInspectionEditing->vInspectionType != cItemCountInspection) && (vInspectionEditing->vInspectionType != cSizeInspection) && (vInspectionEditing->vInspectionType != cMissingLidInspectionOnSize))
							if (vInspectionEditing->vSetupHistoryAutoThreshold != vInspectionEditing->vThreshold)
							{
								vHaveAnAutoThresholdValue = true;
								UpdateFunction3Button();
							}
						}
					}
				}
				//display values on screen if changed
				if (vOldQualityFactor != vLocalSystemData->vQualityFactor)
				//if ((vInspectionEditing->vInspectionType != cSizeInspection) && (vInspectionEditing->vInspectionType != cMissingLidInspectionOnSize))
				{
					vOldQualityFactor = vLocalSystemData->vQualityFactor;
					if (vOldQualityFactor == 0)
						SetDlgItemText(IDC_Simulating, _T(""));
					else
					{
						CString TempString = dtoa(vOldQualityFactor,2);
						TempText.LoadString(IDS_QF);
						SetDlgItemText(IDC_Simulating,TempText + ": " + TempString);
					}
				}
				if (vOldMinimumGoodAverage != vLocalSystemData->vMinimumGoodAverage)
				if (vInspectionEditing->vInspectionType != cJarBottomContaminantInspection)
				{
					if (!vOldMinimumGoodAverage)
					{
						UpdateButtonText();  //just filled in Average so show start bad containers button
						vOldMinimumGoodStandardDeviation = vLocalSystemData->vMinimumGoodStandardDeviation + 1;//change old to force screen update
					}
					vOldMinimumGoodAverage = vLocalSystemData->vMinimumGoodAverage;
					CString TempString = dtoa(vInspectionEditing->ConvertToWeight(vLocalSystemData->vMinimumGoodAverage),2);
					CString TempText("");
					TempText.LoadString(IDS_Avg);
					SetDlgItemText(IDC_SubFunction6Button1,TempText + ": " + TempString);
				}
				//if always got same value set SD to .6 to give a reasonable margin
				if (vLocalSystemData->vMinimumGoodIndex > 4)  //minimum number of containers before evaluate statistics shown
				if (vLocalSystemData->vMinimumGoodStandardDeviation < .6)
					vLocalSystemData->vMinimumGoodStandardDeviation = (float)(.6);

				//if good values have changed so display them on screen
				if (vOldMinimumGoodStandardDeviation != vLocalSystemData->vMinimumGoodStandardDeviation)
				if (vInspectionEditing->vInspectionType != cJarBottomContaminantInspection)
				{
					vOldMinimumGoodStandardDeviation = vLocalSystemData->vMinimumGoodStandardDeviation;

					CString TempString = dtoa(vLocalSystemData->vMinimumGoodStandardDeviation, 2);
					if ((vInspectionEditing->vCheckWeighMFactor) && (vInspectionEditing->vCheckWeighMFactor != 1))
						TempString = dtoa(vLocalSystemData->vMinimumGoodStandardDeviation * vInspectionEditing->vCheckWeighMFactor, 6);

					CString TempText("");
					TempText.LoadString(IDS_SD);
					SetDlgItemText(IDC_SubFunction6Button2,TempText + ": " + TempString);

					if (vLocalSystemData->vFrameCount > 4)
					if ((vLocalSystemData->vMinimumGoodStandardDeviation <= 16) || 
						((vInspectionEditing) && ((vInspectionEditing->vInspectionType == cItemCountInspection) || (vInspectionEditing->vInspectionType == cSizeInspection) || (vInspectionEditing->vInspectionType == cMissingLidInspectionOnSize))))
					if (vLocalSystemData->vMaximumBadAverage == 0)
					if ((vLocalSystemData->vMinimumGoodAverage > (vLocalSystemData->vMinimumGoodStandardDeviation * 5) + 5) ||
						((vInspectionEditing) && ((vInspectionEditing->vInspectionType == cItemCountInspection) || (vInspectionEditing->vInspectionType == cSizeInspection) || (vInspectionEditing->vInspectionType == cMissingLidInspectionOnSize))) ||
						((vInspectionEditing->vInspectionType == cThresholdContaminantInspection) && (vLocalSystemData->vMinimumGoodAverage > (vLocalSystemData->vMinimumGoodStandardDeviation * 3.7) + 5)))
					{
						BYTE TempAutoThreshold = (BYTE)(vLocalSystemData->vMinimumGoodAverage - (vLocalSystemData->vMinimumGoodStandardDeviation * 5));

						if (vInspectionEditing->vInspectionType == cThresholdContaminantInspection)
						{
							TempAutoThreshold = (BYTE)(vLocalSystemData->vMinimumGoodAverage - (vLocalSystemData->vMinimumGoodStandardDeviation * 3.7));
						}
						else
						if (vInspectionEditing->vInspectionType == cContaminantBonesInspection)
						{
							if (TempAutoThreshold > 245)
								TempAutoThreshold = 245;
							if (TempAutoThreshold < 150)
								TempAutoThreshold = 150;
						}
						else
						if (vInspectionEditing->vInspectionType == cGlass13mmContaminantInspection)
						{
							if (TempAutoThreshold > 246)
								TempAutoThreshold = 246;
						}
						else
						{
							if (TempAutoThreshold > 240)
								TempAutoThreshold = 240;
							if ((vInspectionEditing->vEnhanceMethod) || (!vInspectionEditing->vSubtractAverage))
							if (TempAutoThreshold < 150)
								TempAutoThreshold = 150;
						}

						if (vLocalSystemData->vQualityFactor >= 1)
						{
							//if always got same value set SD to .6 to give a reasonable margin
							if (vLocalSystemData->vMinimumGoodIndex > 4)  //minimum number of containers before evaluate statistics shown
							if (vLocalSystemData->vMaximumBadStandardDeviation < .6)
								vLocalSystemData->vMaximumBadStandardDeviation = (float)(.6);

							//if always got same value set SD to .6 to give a reasonable margin
							if (vLocalSystemData->vMinimumGoodIndex > 4)  //minimum number of containers before evaluate statistics shown
							if (vLocalSystemData->vMinimumGoodStandardDeviation < .6)
								vLocalSystemData->vMinimumGoodStandardDeviation = (float)(.6);

							double TempDifference = vLocalSystemData->vMinimumGoodAverage - (5.0 * vLocalSystemData->vMinimumGoodStandardDeviation) - 
								(vLocalSystemData->vMaximumBadAverage + (3.0 * vLocalSystemData->vMaximumBadStandardDeviation));

							if (TempDifference > 0)
								TempAutoThreshold = (BYTE)(vLocalSystemData->vMaximumBadAverage + (TempDifference / 2) + (3.0 * vLocalSystemData->vMaximumBadStandardDeviation) + .5);
						}

						if ((vInspectionEditing->vFindGoodIntensity) || (vInspectionEditing->vFindBadIntensity))
						{
							vInspectionEditing->vSetupHistoryAutoThreshold = TempAutoThreshold;
							SetDlgItemText(IDC_Width2,dtoa(vInspectionEditing->ConvertToWeight(TempAutoThreshold), 0));
							if ((!vChangeMade) || ((vInspectionEditing) && ((vInspectionEditing->vInspectionType == cItemCountInspection) || (vInspectionEditing->vInspectionType == cSizeInspection) || (vInspectionEditing->vInspectionType == cMissingLidInspectionOnSize))))
							{

								if (((!vLocalConfigurationData->vEnableEjectors) || (vLocalSystemData->vInAutoSetup)) || 
									((vInspectionEditing) && ((vInspectionEditing->vInspectionType == cItemCountInspection) || (vInspectionEditing->vInspectionType == cSizeInspection) || (vInspectionEditing->vInspectionType == cMissingLidInspectionOnSize))))
								{
									if ((vInspectionEditing) && ((vInspectionEditing->vInspectionType == cItemCountInspection) || (vInspectionEditing->vInspectionType == cSizeInspection) || (vInspectionEditing->vInspectionType == cMissingLidInspectionOnSize)))
									{ //insert inspection
										if (!vChangeMade)
										{
											double TempDifference = vLocalSystemData->vMinimumGoodIntensity - vLocalSystemData->vMaximumBadIntensity;
											WORD TempAutoThresholdW = (WORD)(vLocalSystemData->vMinimumGoodIntensity - (TempDifference / 4));
											if ((vLocalSystemData->vMinimumGoodIntensity - (3 * vLocalSystemData->vMinimumGoodStandardDeviation) > TempAutoThresholdW) && (vLocalSystemData->vMinimumGoodStandardDeviation > 1))
												TempAutoThresholdW = (WORD)(vLocalSystemData->vMinimumGoodIntensity - (3 * vLocalSystemData->vMinimumGoodStandardDeviation));
											vInspectionEditing->vSize = TempAutoThresholdW;
											if (vInspectionEditing->vSize < 2)
												vInspectionEditing->vSize = 2;
											if (vInspectionEditing->vInspectionType != cItemCountInspection)
												SetDlgItemText(IDC_Function1Button, "Size\n" + dtoa(vInspectionEditing->vSize, 0));

											if ((vGlobalCurrentProduct->vSetupHistoryAverageDensity > 25) && (vGlobalCurrentProduct->vSetupHistoryAverageDensity < 200))
											{
												TempAutoThreshold = (BYTE)(vGlobalCurrentProduct->vSetupHistoryAverageDensity * cMissingLidSizeThresholdRatio);

												//find the darkest pixel in the ROI

												BYTE TempDarkestPixel = 255;
												WORD TempBottom = 0;
												WORD TempLeft = 0;
												WORD TempHeight = 0;
												WORD TempWidth = 0;

												BYTE TempAutoImproveState = 0;

												vLocalSystemData->vITIPCDig->GetInspectionROIBounds(vInspectionEditing, 
													&TempBottom, &TempLeft, &TempHeight, &TempWidth, vLocalSystemData->vITIPCDig->vOriginalImage->vLeftReferenceAdjust, 
													vLocalConfigurationData->vReferenceROIsToEdges, TempAutoImproveState);

												for (WORD TempXLoop = 0; TempXLoop < TempWidth; TempXLoop++)
												{
													BYTE *TempPointer = vLocalSystemData->vITIPCDig->vOriginalImage->vGreyImage + TempBottom + ((TempLeft +
														TempXLoop) * vLocalSystemData->vITIPCDig->vOriginalBufferSizeX);

													for (WORD TempYLoop = 0; TempYLoop < TempHeight; TempYLoop++)
													{
														if (*TempPointer < TempDarkestPixel)
															TempDarkestPixel = *TempPointer;
														TempPointer++;
													}
												}

												if (TempDarkestPixel)
												{
													TempAutoThreshold = (BYTE)(TempDarkestPixel + ((vGlobalCurrentProduct->vSetupHistoryAverageDensity - TempDarkestPixel) / 4));
												}

												if (!vChangeMade)
												if (vInspectionEditing->vInspectionType != cItemCountInspection)
												if ((((vInspectionEditing->vThreshold - 7 > TempAutoThreshold) || (vInspectionEditing->vThreshold + 7 < TempAutoThreshold)) && (vLocalSystemData->vFrameCount < 10)) ||
													((vInspectionEditing->vThreshold - 20 > TempAutoThreshold) || (vInspectionEditing->vThreshold + 20 < TempAutoThreshold)))
												{//changing threshold value, so must clear evaluate results so far
													vInspectionEditing->vThreshold = TempAutoThreshold;
													ClearEvaluate();
												}
											}
											else
											if (vLocalSystemData->vITIPCDig->vOriginalImage)
											if (vLocalSystemData->vITIPCDig->vOriginalImage->vGreyImage)
											{
												double TempDensity = vLocalSystemData->vITIPCDig->vOriginalImage->vOriginalImageIntensity;//must use temporary variable to be sure value does not change after range check
												if ((TempDensity > 25) && (TempDensity < 200))
												{
													TempAutoThreshold = (BYTE)(TempDensity * cMissingLidSizeThresholdRatio);

													//find the darkest pixel in the ROI
													BYTE TempDarkestPixel = 255;
													WORD TempBottom = 0;
													WORD TempLeft = 0;
													WORD TempHeight = 0;
													WORD TempWidth = 0;
													BYTE TempAutoImproveState = 0; //not a learned inspection, so don't bring in ROI edges

													vLocalSystemData->vITIPCDig->GetInspectionROIBounds(vInspectionEditing, &TempBottom, &TempLeft, &TempHeight, &TempWidth, 
														vLocalSystemData->vITIPCDig->vOriginalImage->vLeftReferenceAdjust, vLocalConfigurationData->vReferenceROIsToEdges, TempAutoImproveState);

													for (WORD TempXLoop = 0; TempXLoop < TempWidth; TempXLoop++)
													{
														BYTE *TempPointer = vLocalSystemData->vITIPCDig->vOriginalImage->vGreyImage + TempBottom + ((TempLeft +
															TempXLoop) * vLocalSystemData->vITIPCDig->vOriginalBufferSizeX);

														for (WORD TempYLoop = 0; TempYLoop < TempHeight; TempYLoop++)
														{
															if (*TempPointer < TempDarkestPixel)
																TempDarkestPixel = *TempPointer;
															TempPointer++;
														}
													}

													if (TempDarkestPixel)
													{
														TempAutoThreshold = (BYTE)(TempDarkestPixel + ((TempDensity - TempDarkestPixel) / 4));
													}

													if (!vChangeMade)
													if ((((vInspectionEditing->vThreshold - 7 > TempAutoThreshold) || (vInspectionEditing->vThreshold + 7 < TempAutoThreshold)) && (vLocalSystemData->vFrameCount < 10)) ||
														((vInspectionEditing->vThreshold - 20 > TempAutoThreshold) || (vInspectionEditing->vThreshold + 20 < TempAutoThreshold)))
													{
														vInspectionEditing->vThreshold = TempAutoThreshold;
														ClearEvaluate();
													}
												}
											}
										}
									}
									else
										vInspectionEditing->vThreshold = TempAutoThreshold;

									SetDlgItemText(IDC_ThresholdForContainerBounds2,dtoa(vInspectionEditing->ConvertToWeight(vInspectionEditing->vThreshold), 0));
									if (vMode == cSetContaminantThreshold)
										ShowSensitivity();
								}
								else
								if ((vInspectionEditing->vInspectionType != cItemCountInspection) && (vInspectionEditing->vInspectionType != cSizeInspection) && (vInspectionEditing->vInspectionType != cMissingLidInspectionOnSize))
								if (vInspectionEditing->vSetupHistoryAutoThreshold != vInspectionEditing->vThreshold)
								{
									vHaveAnAutoThresholdValue = true;
									UpdateFunction3Button();
								}
							}
						}
					}
				}
				//if evaluating bad
				if ((vOldMaximumBadIntensity != vLocalSystemData->vMaximumBadIntensity) || ((vLocalSystemData->vMaximumBadCount) && (vInspectionEditing->vFindBadIntensity)))
				{
					vOldMaximumBadIntensity = vLocalSystemData->vMaximumBadIntensity;
					CString TempText("");
					TempText.LoadString(IDS_Max);
					if ((vInspectionEditing->vCheckWeighMFactor != 1) || (vInspectionEditing->vCheckWeighBFactor))
						SetDlgItemText(IDC_SubFunction8Button3, "Min: " + dtoa(vInspectionEditing->ConvertToWeight(vOldMaximumBadIntensity),0));
					else
						SetDlgItemText(IDC_SubFunction8Button3, "Max: " + dtoa(vInspectionEditing->ConvertToWeight(vOldMaximumBadIntensity),0));

					//if have evaluated good and bad for contaminant or void
					if ((vLocalSystemData->vMaximumBadAverage) && (vLocalSystemData->vMinimumGoodAverage))
					if (vLocalSystemData->vMinimumGoodIntensity > vLocalSystemData->vMaximumBadIntensity)
					{
						double TempDifference = vLocalSystemData->vMinimumGoodIntensity - vLocalSystemData->vMaximumBadIntensity;
						WORD TempAutoThreshold = (BYTE)(vLocalSystemData->vMaximumBadIntensity + (TempDifference / 2.0) + .5);

						if (vLocalSystemData->vQualityFactor >= 1)
						{
							//if always got same value set SD to .6 to give a reasonable margin
							if (vLocalSystemData->vMinimumGoodIndex > 4)  //minimum number of containers before evaluate statistics shown
							if (vLocalSystemData->vMaximumBadStandardDeviation < .6)
								vLocalSystemData->vMaximumBadStandardDeviation = (float)(.6);

							//if always got same value set SD to .6 to give a reasonable margin
							if (vLocalSystemData->vMinimumGoodIndex > 4)  //minimum number of containers before evaluate statistics shown
							if (vLocalSystemData->vMinimumGoodStandardDeviation < .6)
								vLocalSystemData->vMinimumGoodStandardDeviation = (float)(.6);

							TempDifference = (double)vLocalSystemData->vMinimumGoodIntensity - (5.0 * vLocalSystemData->vMinimumGoodStandardDeviation) - 
								((double)vLocalSystemData->vMaximumBadIntensity + (3.0 * vLocalSystemData->vMaximumBadStandardDeviation));
							TempAutoThreshold = (BYTE)(vLocalSystemData->vMaximumBadIntensity + (TempDifference / 2) + (3.0 * vLocalSystemData->vMaximumBadStandardDeviation) + .5);
						}

						//save automatic value, show on screen, and set into inspection if not edited
						if ((vInspectionEditing->vFindGoodIntensity) || (vInspectionEditing->vFindBadIntensity))
						{
							vInspectionEditing->vSetupHistoryAutoThreshold = (BYTE)TempAutoThreshold;
													
							SetDlgItemText(IDC_Width2,dtoa(vInspectionEditing->ConvertToWeight(TempAutoThreshold), 0));
							if (!vChangeMade)
							{
								if ((!vLocalConfigurationData->vEnableEjectors) || (vLocalSystemData->vInAutoSetup))
								{
									if ((vInspectionEditing) && ((vInspectionEditing->vInspectionType == cItemCountInspection) || (vInspectionEditing->vInspectionType == cSizeInspection) || (vInspectionEditing->vInspectionType == cMissingLidInspectionOnSize)))
									{ //insert inspection
										TempAutoThreshold = (WORD)(vLocalSystemData->vMaximumBadIntensity + ((vLocalSystemData->vMinimumGoodIntensity - vLocalSystemData->vMaximumBadIntensity) / 4));
										vInspectionEditing->vSize = TempAutoThreshold;

										if (vInspectionEditing->vSize < 2)
											vInspectionEditing->vSize = 2;
										if (vInspectionEditing->vInspectionType != cItemCountInspection)
											SetDlgItemText(IDC_Function1Button, "Size\n" + dtoa(vInspectionEditing->vSize, 0));
									}
									else
									{
										vInspectionEditing->vThreshold = (BYTE)TempAutoThreshold;

										SetDlgItemText(IDC_ThresholdForContainerBounds2,dtoa(vInspectionEditing->ConvertToWeight(TempAutoThreshold), 0));
										if (vMode == cSetContaminantThreshold)
											ShowSensitivity();
									}
								}
								else
								if ((vInspectionEditing->vInspectionType != cItemCountInspection) && (vInspectionEditing->vInspectionType != cSizeInspection) && (vInspectionEditing->vInspectionType != cMissingLidInspectionOnSize))
								if (vInspectionEditing->vSetupHistoryAutoThreshold != vInspectionEditing->vThreshold)
								{
									vHaveAnAutoThresholdValue = true;
									UpdateFunction3Button();
								}
							}
						}
					}
				}
				//display maximum bad values on screen if they have changed
				if ((vOldMaximumBadAverage != vLocalSystemData->vMaximumBadAverage) || ((vLocalSystemData->vMaximumBadCount) && (vInspectionEditing->vFindBadIntensity)))
				{
					if (vOldMaximumBadAverage == 0)
					{
						CWnd *TempWindow = CWnd::GetDlgItem(IDC_Function5Button);
						if (TempWindow)
							TempWindow->Invalidate(false);
						vOldMaximumBadStandardDeviation = vLocalSystemData->vMaximumBadStandardDeviation + 1; //change old to force screen update
					}
					vOldMaximumBadAverage = vLocalSystemData->vMaximumBadAverage;
					CString TempString = dtoa(vInspectionEditing->ConvertToWeight(vOldMaximumBadAverage),2);
					CString TempText("");
					TempText.LoadString(IDS_Avg);
					SetDlgItemText(IDC_SubFunction8Button1,TempText + ": " + TempString);
				}
				if ((vOldMaximumBadStandardDeviation != vLocalSystemData->vMaximumBadStandardDeviation) || ((vLocalSystemData->vMaximumBadCount) && (vInspectionEditing->vFindBadIntensity)))
				{
					vOldMaximumBadStandardDeviation = vLocalSystemData->vMaximumBadStandardDeviation;

					CString TempString = dtoa(vLocalSystemData->vMaximumBadStandardDeviation, 2);
					if ((vInspectionEditing->vCheckWeighMFactor) && (vInspectionEditing->vCheckWeighMFactor != 1))
						TempString = dtoa(vLocalSystemData->vMaximumBadStandardDeviation * vInspectionEditing->vCheckWeighMFactor, 6);

					CString TempText("");
					TempText.LoadString(IDS_SD);
					SetDlgItemText(IDC_SubFunction8Button2,TempText + ": " + TempString);
				}
			}//end of contaminant threshold menu
		}
		else
		if (vMode == cCenterExposureMenu)
		{
			TempText.LoadString(IDS_AdjustExposureAsNeeded);

			if ((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode)  || (vGlobalCurrentProduct->vBulkProductMode)||
				(vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vLocalConfigurationData->vScanTracType == cAllegroScanTrac) ||
				(vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac) || (vLocalConfigurationData->vScanTracType == cBriosoScanTrac)) //Not Continuous Feed, or a Pipeline or TD
				TempText = " Adjust Voltage to get Darkest Pixel between 50 and 70";
				
			SetStatusLine(TempText);
		}
		else
		if (vMode == cLearnMenu)
		{
			if (vChangeMade)
			if (!vShowAverage)
			if (!vShowStructure)
			if (!vShowVoidStructure)
			//if (!vShowGlassStructure)
			if (!vShowContaminant2Structure)
			if (!vShowHistogram)
			//if (!vShowGlassHistogram)
			if (!vGlobalProcessQuickLearnImagesThread)
			if (!vOldAdjustingBrightnessBeforeLearning)
				SetStatusLine(TempStringName);
		}
		else
		if (vMode == cSetImageSize)
		{
				//if (vLocalSystemData->vInLowExposureMode)
				//{
				//	TempText.LoadString(IDS_ExposurelowtoAutosize);
				//	SetStatusLine(TempText + ": " + vProductEditingName);
				//}
				//else
			if (!vNeedToAutoSizeNextContainer)
			if (!vChangedEdgeFindingMode)
				SetStatusLine(TempStringName);

			if (vLocalSystemData->vITIPCDig->vOriginalImage->vRight <	vLocalSystemData->vITIPCDig->vOriginalImage->vLeft)
			{
				vLocalSystemData->vITIPCDig->vOriginalImage->vLeft = 0;
				vLocalSystemData->vITIPCDig->vOriginalImage->vRight = vLocalSystemData->vITIPCDig->vOriginalBufferSizeY - 1;
			}
			float TempContainerWidth = (float)(( (vLocalSystemData->vITIPCDig->vOriginalImage->vRight - 
				vLocalSystemData->vITIPCDig->vOriginalImage->vLeft) / (vGlobalPixelsPerUnit * vGlobalCurrentProduct->vOverScanMultiplier)));
			TempText.LoadString(IDS_CW);

			if (PasswordOK(cTemporaryInspxPassword,false))
				SetDlgItemText(IDC_Width2, TempText + ": " + dtoa(TempContainerWidth,2) + " (" + dtoa(TempContainerWidth * (vGlobalPixelsPerUnit * vGlobalCurrentProduct->vOverScanMultiplier),0) + ")");
			else
				SetDlgItemText(IDC_Width2, TempText + ": " + dtoa(TempContainerWidth,2));

			DisplayLocationEdge(); //must redraw purple line incase moved automatically because it was too low
		}
		if (vMode == CMultiLanePositionAdjustment)
			SetStatusLine("Green = start of lane, Red = end of lane, Blue = other lanes");
		//SetDlgItemText(IDC_StatusLine2,"# " + vLocalSystemData->vFrameCountString + " ");

		//test only
		//SetDlgItemText(IDC_Simulating,
		//	dtoa(vLocalSystemData->vITIPCDig->vOriginalImage->vBodyTriggerGapLength / vGlobalPixelsPerUnit,2));

		if (vNeedToAutoSizeNextContainer)
		if (vLocalSystemData->vITIPCDig->vOriginalImage->vBodyTriggerGapLength > vGlobalPixelsPerUnit * 24)
		{
			vNeedToAutoSizeNextContainer = false;
			AutoSizeImage();
		}
	}
}

void CAutoSetupImageDisplayDialog::CalculateLearnedReferenceValues()
{
	if (vLocalSystemData->vITIPCDig->vProductAverageImage)
	{
		if (vLocalSystemData->vITIPCDig->vProductAverageImage->vGreyImage)
		{
			WORD TempOriginalReferenceRight = vGlobalCurrentProduct->vReferenceRight;
			WORD TempOriginalReferenceLeft = vGlobalCurrentProduct->vReferenceLeft;
			WORD TempOriginalReferenceTop = vGlobalCurrentProduct->vReferenceTop;
			WORD TempOriginalReferenceBottom = vGlobalCurrentProduct->vReferenceBottom;
			WORD TempWidth = 0;
			WORD TempHeight = 0;

			vGlobalCurrentProduct->vReferenceWidth = 0;

			vLocalSystemData->vITIPCDig->FindContainerDimensions(vLocalSystemData->vITIPCDig->vProductAverageImage->vGreyImage, vLocalSystemData->vITIPCDig->vASIDDScratchImage, &vGlobalCurrentProduct->vReferenceTop, 
				&vGlobalCurrentProduct->vReferenceBottom, &vGlobalCurrentProduct->vReferenceRight, &vGlobalCurrentProduct->vReferenceLeft,
				&TempHeight, &TempWidth, false, 10000); //10000 = finding new reference, so don't care how far off old reference new values are

			if ((TempOriginalReferenceRight > vGlobalCurrentProduct->vReferenceRight + 2) || ((TempOriginalReferenceRight + 2) < vGlobalCurrentProduct->vReferenceRight))
				ReportErrorMessage(" Image, ight Reference changed.  Was: " + dtoa(TempOriginalReferenceRight) + ", is: " + dtoa(vGlobalCurrentProduct->vReferenceRight), cAction, 0);

			if ((TempOriginalReferenceLeft > vGlobalCurrentProduct->vReferenceLeft + 2) || ((TempOriginalReferenceLeft + 2) < vGlobalCurrentProduct->vReferenceLeft))
				ReportErrorMessage("Left Reference changed.  Was: " + dtoa(TempOriginalReferenceLeft) + ", is: " + dtoa(vGlobalCurrentProduct->vReferenceLeft), cAction, 0);

			if ((TempOriginalReferenceTop > vGlobalCurrentProduct->vReferenceTop + 2) || ((TempOriginalReferenceTop + 2) < vGlobalCurrentProduct->vReferenceTop))
				ReportErrorMessage("Top Reference changed.  Was: " + dtoa(TempOriginalReferenceTop) + ", is: " + dtoa(vGlobalCurrentProduct->vReferenceTop), cAction, 0);

			if ((TempOriginalReferenceBottom > vGlobalCurrentProduct->vReferenceBottom + 2) || ((TempOriginalReferenceBottom + 2) < vGlobalCurrentProduct->vReferenceBottom))
				ReportErrorMessage("Bottom Reference changed.  Was: " + dtoa(TempOriginalReferenceBottom) + ", is: " + dtoa(vGlobalCurrentProduct->vReferenceBottom), cAction, 0);

			vGlobalCurrentProduct->vReferenceWidth = vGlobalCurrentProduct->vReferenceRight - vGlobalCurrentProduct->vReferenceLeft;

			int TempDifferenceInRightReference = TempOriginalReferenceRight - vGlobalCurrentProduct->vReferenceRight;
			int TempDifferenceInLeftReference = TempOriginalReferenceLeft - vGlobalCurrentProduct->vReferenceLeft;

			if (vLocalSystemData->vSystemRunMode == cAutoSetupRunningSystem)
			if (vAutoSetupType == cNewAutoSetup)
			{
				//correct all inspection ROIs since moved reference
				if (TempOriginalReferenceRight > 10)
				for (BYTE TempLoopA = 0; TempLoopA < vGlobalCurrentProduct->vNumberOfInspections; TempLoopA++)
				if (vGlobalCurrentProduct->vInspection[TempLoopA])
				{
					ReportErrorMessage(vGlobalCurrentProduct->vInspection[TempLoopA]->vName + " Inspection Left ROI Was: " + dtoa(vGlobalCurrentProduct->vInspection[TempLoopA]->vROIPixelLeft, 0), cAction, 0);

					vGlobalCurrentProduct->vInspection[TempLoopA]->SetROILeft(vGlobalCurrentProduct->vInspection[TempLoopA]->vROILeft + 
						(TempDifferenceInLeftReference / vGlobalPixelsPerUnit / vGlobalCurrentProduct->vOverScanMultiplier), vGlobalCurrentProduct->vOverScanMultiplier);

					if (vGlobalCurrentProduct->vInspection[TempLoopA]->vROILeft >= vGlobalCurrentProduct->vInspection[TempLoopA]->vROIRight)
						vGlobalCurrentProduct->vInspection[TempLoopA]->SetROILeft(0, vGlobalCurrentProduct->vOverScanMultiplier);

					ReportErrorMessage(vGlobalCurrentProduct->vInspection[TempLoopA]->vName + " Inspection Left ROI Is: " + dtoa(vGlobalCurrentProduct->vInspection[TempLoopA]->vROIPixelLeft, 0), cAction, 0);
					ReportErrorMessage(vGlobalCurrentProduct->vInspection[TempLoopA]->vName + " Inspection Right ROI Was: " + dtoa(vGlobalCurrentProduct->vInspection[TempLoopA]->vROIPixelRight, 0), cAction, 0);

					vGlobalCurrentProduct->vInspection[TempLoopA]->SetROIRight(vGlobalCurrentProduct->vInspection[TempLoopA]->vROIRight + 
						(TempDifferenceInRightReference / vGlobalPixelsPerUnit / vGlobalCurrentProduct->vOverScanMultiplier), vGlobalCurrentProduct->vOverScanMultiplier);

					ReportErrorMessage(vGlobalCurrentProduct->vInspection[TempLoopA]->vName + " Inspection Right ROI Is: " + dtoa(vGlobalCurrentProduct->vInspection[TempLoopA]->vROIPixelRight, 0), cAction, 0);
				}
				if (!vGlobalCurrentProduct->vMultiLaneNumberOfLanes)
				if (vGlobalCurrentProduct->vDriftDensityInspection)
				{
					ReportErrorMessage("Product Density Left ROI Was: " + dtoa(vGlobalCurrentProduct->vDriftDensityInspection->vROIPixelLeft, 0), cAction, 0);

					vGlobalCurrentProduct->vDriftDensityInspection->SetROILeft(vGlobalCurrentProduct->vDriftDensityInspection->vROILeft + 
						(TempDifferenceInLeftReference / vGlobalPixelsPerUnit / vGlobalCurrentProduct->vOverScanMultiplier), vGlobalCurrentProduct->vOverScanMultiplier);

					if (vGlobalCurrentProduct->vDriftDensityInspection->vROILeft >= vGlobalCurrentProduct->vDriftDensityInspection->vROIRight)
						vGlobalCurrentProduct->vDriftDensityInspection->SetROILeft(0, vGlobalCurrentProduct->vOverScanMultiplier);

					ReportErrorMessage("Product Density Left ROI Is: " + dtoa(vGlobalCurrentProduct->vDriftDensityInspection->vROIPixelLeft, 0), cAction, 0);
					ReportErrorMessage("Product Density Right ROI Was: " + dtoa(vGlobalCurrentProduct->vDriftDensityInspection->vROIPixelRight, 0), cAction, 0);

					vGlobalCurrentProduct->vDriftDensityInspection->SetROIRight(vGlobalCurrentProduct->vDriftDensityInspection->vROIRight + 
						(TempDifferenceInRightReference / vGlobalPixelsPerUnit / vGlobalCurrentProduct->vOverScanMultiplier), vGlobalCurrentProduct->vOverScanMultiplier);

					ReportErrorMessage("Product Density Right ROI Is: " + dtoa(vGlobalCurrentProduct->vDriftDensityInspection->vROIPixelRight, 0), cAction, 0);
				}
			}
		}
	}
}

void CAutoSetupImageDisplayDialog::OnDialogtitlestatictext1()
{
	BYTE TempHiddenKeys = 8;
	if (!PasswordOK(cTemporaryInspxPassword,false))
		TempHiddenKeys = 10;
	if ((vGlobalInAutoSize) && (vLocalConfigurationData->vResynchronizeEjectors))
	{
		TempHiddenKeys = 0; //8;
		if (!PasswordOK(cTemporaryInspxPassword,false))
			TempHiddenKeys = 2; //10;
	}

	PostThreadMessage(vGlobalCallingThreadID, cDisplaySoftKeyMenu, TempHiddenKeys, TempHiddenKeys);
}

void CAutoSetupImageDisplayDialog::OnSubFunction1ButtonDoubleClicked()
{
	if (vMode == cSetROI)
	{
		//ReportErrorMessage("Left Button Double Click on SubFunction 1",cAction, 32000);
		
		if (vMouseState > 8)	//if ((vMouseState >= 9) && (vMouseState <= 16))
		{
			KillTimer(vMouseDownRepeatTimerHandle);
			//stop scrolling
			vMouseState = 0;
		}
		else	//if (vMouseState == 0)
		{
			vMouseState = 9;	//toggle move ROI lines
			int TimerResult = SetTimer(vMouseDownRepeatTimerHandle,200,NULL);
			if (!TimerResult)
				ReportErrorMessage("Error-Second Timer Failed",cEMailInspx,32000);
		}
	}
}

void CAutoSetupImageDisplayDialog::OnSubFunction2ButtonDoubleClicked()
{
	if (vMode == cSetROI)
	{
		//ReportErrorMessage("Left Button Double Click on SubFunction 2",cAction, 32000);
		
		if (vMouseState > 8)	//if ((vMouseState >= 9) && (vMouseState <= 16))
		{
			KillTimer(vMouseDownRepeatTimerHandle);
			//stop scrolling
			vMouseState = 0;
		}
		else	//if (vMouseState == 0)
		{
			vMouseState = 10;	//toggle move ROI lines
			int TimerResult = SetTimer(vMouseDownRepeatTimerHandle,200,NULL);
			if (!TimerResult)
				ReportErrorMessage("Error-Second Timer Failed",cEMailInspx,32000);
		}
	}
}

void CAutoSetupImageDisplayDialog::OnSubFunction3ButtonDoubleClicked()
{
	if (vMode == cSetROI)
	{
		//ReportErrorMessage("Left Button Double Click on SubFunction 3",cAction, 32000);
		
		if (vMouseState > 8)	//if ((vMouseState >= 9) && (vMouseState <= 16))
		{
			KillTimer(vMouseDownRepeatTimerHandle);
			//stop scrolling
			vMouseState = 0;
		}
		else	//if (vMouseState == 0)
		{
			vMouseState = 11;	//toggle move ROI lines
			int TimerResult = SetTimer(vMouseDownRepeatTimerHandle,200,NULL);
			if (!TimerResult)
				ReportErrorMessage("Error-Second Timer Failed",cEMailInspx,32000);
		}
	}
}


void CAutoSetupImageDisplayDialog::OnSubFunction4ButtonDoubleClicked()
{
	if (vMode == cSetROI)
	{
		//ReportErrorMessage("Left Button Double Click on SubFunction 4",cAction, 32000);
		
		if (vMouseState > 8)	//if ((vMouseState >= 9) && (vMouseState <= 16))
		{
			KillTimer(vMouseDownRepeatTimerHandle);
			//stop scrolling
			vMouseState = 0;
		}
		else	//if (vMouseState == 0)
		{
			vMouseState = 12;	//toggle move ROI lines
			int TimerResult = SetTimer(vMouseDownRepeatTimerHandle,200,NULL);
			if (!TimerResult)
				ReportErrorMessage("Error-Second Timer Failed",cEMailInspx,32000);
		}
	}
}


void CAutoSetupImageDisplayDialog::OnSubFunction5ButtonDoubleClicked()
{
	if (vMode == cSetROI)
	{
		//ReportErrorMessage("Left Button Double Click on SubFunction 5",cAction, 32000);
		
		if (vMouseState > 8)	//if ((vMouseState >= 9) && (vMouseState <= 16))
		{
			KillTimer(vMouseDownRepeatTimerHandle);
			//stop scrolling
			vMouseState = 0;
		}
		else	//if (vMouseState == 0)
		{
			vMouseState = 13;	//toggle move ROI lines
			int TimerResult = SetTimer(vMouseDownRepeatTimerHandle,200,NULL);
			if (!TimerResult)
				ReportErrorMessage("Error-Second Timer Failed",cEMailInspx,32000);
		}
	}
}


void CAutoSetupImageDisplayDialog::OnSubFunction6ButtonDoubleClicked()
{
	if (vMode == cSetROI)
	{
		//ReportErrorMessage("Left Button Double Click on SubFunction 6",cAction, 32000);
		
		if (vMouseState > 8)	//if ((vMouseState >= 9) && (vMouseState <= 16))
		{
			KillTimer(vMouseDownRepeatTimerHandle);
			//stop scrolling
			vMouseState = 0;
		}
		else	//if (vMouseState == 0)
		{
			vMouseState = 14;	//toggle move ROI lines
			int TimerResult = SetTimer(vMouseDownRepeatTimerHandle,200,NULL);
			if (!TimerResult)
				ReportErrorMessage("Error-Second Timer Failed",cEMailInspx,32000);
		}
	}
}


void CAutoSetupImageDisplayDialog::OnSubFunction7ButtonDoubleClicked()
{
	if (vMode == cSetROI)
	{
		//ReportErrorMessage("Left Button Double Click on SubFunction 7",cAction, 32000);
		
		if (vMouseState > 8)	//if ((vMouseState >= 9) && (vMouseState <= 16))
		{
			KillTimer(vMouseDownRepeatTimerHandle);
			//stop scrolling
			vMouseState = 0;
		}
		else	//if (vMouseState == 0)
		{
			vMouseState = 15;	//toggle move ROI lines
			int TimerResult = SetTimer(vMouseDownRepeatTimerHandle,200,NULL);
			if (!TimerResult)
				ReportErrorMessage("Error-Second Timer Failed",cEMailInspx,32000);
		}
	}
}


void CAutoSetupImageDisplayDialog::OnSubFunction8ButtonDoubleClicked()
{
	if (vMode == cSetROI)
	{
		//ReportErrorMessage("Left Button Double Click on SubFunction 8",cAction, 32000);
		
		if (vMouseState > 8)	//if ((vMouseState >= 9) && (vMouseState <= 16))
		{
			KillTimer(vMouseDownRepeatTimerHandle);
			//stop scrolling
			vMouseState = 0;
		}
		else	//if (vMouseState == 0)
		{
			vMouseState = 16;	//toggle move ROI lines
			int TimerResult = SetTimer(vMouseDownRepeatTimerHandle,200,NULL);
			if (!TimerResult)
				ReportErrorMessage("Error-Second Timer Failed",cEMailInspx,32000);
		}
	}
}

void CAutoSetupImageDisplayDialog::StartMultiLanePositionMenu()
{
	vDrawPurpleLine = true;
	vChangedPurpleLine = true;
	DisplayLocationEdge();

	vMode = CMultiLanePositionAdjustment;
	vLocalSystemData->vMultiLaneInSetupLanePositionsMenu = true;
	vMaximumLaneWidth = (WORD)(vLocalConfigurationData->vNumberOfXRayDetectors * vLocalConfigurationData->vDetectorLength * vGlobalPixelsPerUnitInHeight);
	vMultiImageLaneEditing = 0;

	SetDlgItemText(IDC_SubFunction1Button, _T("Edit Next Lane"));
	m_SubFunction1Button.ShowWindow(SW_SHOW);
	SetDlgItemText(IDC_SubFunction2Button, _T("Edit Previous Lane"));
	m_SubFunction2Button.ShowWindow(SW_SHOW);
	m_SubFunction1Display.ShowWindow(SW_SHOW);

	SetDlgItemText(IDC_SubFunction5Button, _T("Move Start Up"));
	m_SubFunction5Button.ShowWindow(SW_SHOW);
	SetDlgItemText(IDC_SubFunction6Button, _T("Move Start Down"));
	m_SubFunction6Button.ShowWindow(SW_SHOW);
	m_SubFunction5Display.ShowWindow(SW_SHOW);

	SetDlgItemText(IDC_SubFunction7Button, _T("Make Lane Wider"));
	m_SubFunction7Button.ShowWindow(SW_SHOW);
	SetDlgItemText(IDC_SubFunction8Button, _T("Make Lane Narrower"));
	m_SubFunction8Button.ShowWindow(SW_SHOW);
	m_SubFunction7Display.ShowWindow(SW_SHOW);

	SetDlgItemText(IDC_Function1Button, _T("Return To Size Menu"));
	m_Function1Button.ShowWindow(SW_SHOW);
	SetDlgItemText(IDC_Function5Button, _T("Set To Default Lanes"));
	m_Function5Button.ShowWindow(SW_SHOW);

	m_SubFunction3Button.ShowWindow(SW_HIDE);
	this->InvalidateRect(&m_SubFunction3Button.GetRectToInvalidateArrow());
	m_SubFunction4Button.ShowWindow(SW_HIDE);
	this->InvalidateRect(&m_SubFunction4Button.GetRectToInvalidateArrow());
	m_Function2Button.ShowWindow(SW_HIDE);
	this->InvalidateRect(&m_Function2Button.GetRectToInvalidateArrow());
	m_Function3Button.ShowWindow(SW_HIDE);
	this->InvalidateRect(&m_Function3Button.GetRectToInvalidateArrow());
	m_Function4Button.ShowWindow(SW_HIDE);
	this->InvalidateRect(&m_Function4Button.GetRectToInvalidateArrow());

	SetDlgItemText(IDC_DialogTitleStaticText1, _T("Auto Setup Adjust Lane Positions"));
	ChangedProductSettingsSoReload();
}

void CAutoSetupImageDisplayDialog::FinishMultiLanePositionMenu()
{
	m_SubFunction1Display.ShowWindow(SW_HIDE);
	this->InvalidateRect(&m_SubFunction1Button.GetRectToInvalidateArrow());
	//m_SubFunction1Button.ShowWindow(SW_HIDE);
	//m_SubFunction2Button.ShowWindow(SW_HIDE);

	//m_SubFunction5Button.ShowWindow(SW_HIDE);
	//m_SubFunction6Button.ShowWindow(SW_HIDE);
	//m_SubFunction5Display.ShowWindow(SW_HIDE);

	//m_SubFunction7Button.ShowWindow(SW_HIDE);
	//m_SubFunction8Button.ShowWindow(SW_HIDE);
	//m_SubFunction7Display.ShowWindow(SW_HIDE);

	//m_Function1Button.ShowWindow(SW_HIDE);
	//m_Function5Button.ShowWindow(SW_HIDE);

	vLocalSystemData->vMultiLaneInSetupLanePositionsMenu = false;
	ChangedProductSettingsSoReload();
}

void CAutoSetupImageDisplayDialog::ClearReferenceLocationData()
{
	if (vGlobalCurrentProduct)
	{
		vGlobalCurrentProduct->vReferenceLeft = 0;
		vGlobalCurrentProduct->vReferenceRight = 0;
		vGlobalCurrentProduct->vReferenceTop = 0;
		vGlobalCurrentProduct->vReferenceBottom = 0;
		vGlobalCurrentProduct->vReferenceWidth = 0;
	}
}