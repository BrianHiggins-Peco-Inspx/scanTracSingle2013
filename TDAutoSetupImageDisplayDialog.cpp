// TDAutoSetupImageDisplayDialog.cpp : implementation file
//
//ScanTrac Side View Source File

#include "stdafx.h"
#include "ScanTrac.h"
#include "TDAutoSetupImageDisplayDialog.h"
#include "afxdialogex.h"

#include "SystemConfigurationData.h"
#include "NoticeDialog.h"
#include "NumericEntryDialog.h"
#include "PasswordEntryDialog.h"
#include "YesNoDialog.h"
#include "SerialPort.h"
#include "SelectMultipleItemDialog.h"
#include "AlphaEntryDialog.h"
#include "HelpDialog.h"
#include "NewDiagnosticDialog.h"
#include "ImageAquisitionThread.h"
#include "SelectItemDialog.h"
#include "NewConfigProductEjectorDialog.h"
#include "ScanTracDlg.h"
#include "ProcessEvaluateImagesThread.h"
#include "ProcessQuickLearnImagesThread.h"

extern BYTE vGlobalScanTracType;
extern CProcessQuickLearnImagesThread *vGlobalProcessQuickLearnImagesThread;
extern CITIPCDig *vGlobalITIPCDig;
extern bool vGlobalFPGAVersion11Point0OrHigher;
extern CString vGlobalTopString;
extern CString vGlobalBottomString;
extern WORD vGlobalPasswordTimeOut;
extern bool vGlobalFPGAVersion10Point0OrHigher;
extern BYTE vGlobalLearnState;
extern bool vGlobalEvenSecond;
//extern BYTE vGlobalMouseMoveCounter;
//extern WORD vGlobalUnderRemoteControl;
extern BYTE vGlobalShiftKeyDown;
extern CSerialPort *vGlobaluCSerialPort;
extern CProduct *vGlobalCurrentProduct;
extern CString vGlobalCurrentDirectory;
extern CImageAquisitionThread *vGlobalImageAquisitionThread;
extern CProcessEvaluateImagesThread *vGlobalProcessEvaluateImagesThread;
extern BYTE vGlobalInAutoSize;
extern bool vGlobalInAutoExposure;
extern int vGlobalPasswordLevel;
extern unsigned long vGlobalDialogTitleColor;

// CTDAutoSetupImageDisplayDialog dialog

IMPLEMENT_DYNAMIC(CTDAutoSetupImageDisplayDialog, CDialog)

CTDAutoSetupImageDisplayDialog::CTDAutoSetupImageDisplayDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CTDAutoSetupImageDisplayDialog::IDD, pParent)
{
	vOldImageNumberToDisplay = 0xFFFF;
	vOldXRayState = 0;
	vOldCalibrationImageNumber = 0;
	vGaveTurnOnConveyorMessage = 0;
	vOldTestEjectNextContainer = 0;
	vGaveTooDarkWarning = 0;
	vStartedResize = false;
	vMakeStatusLineGreen = false;
	vExposureIsCorrect = false;
	vSanityCheckFailureCount = 0;
	vIntegrationTimeSet = false;
	vSamplingConveyorSpeed = false;

	vAdjustedExposureForAutoSize = 0;
	vAdjustingImageBrightnessBelowSaturation = false;
	vAdjustingImageBrightnessJustAboveSaturation = false;
	vAdjustBrightnessFrameCount = 0;
	vAdjustBrightnessDotCount = 0;
	vMode = cInitializingDisplay;  //cSetSizeMenu, cExposureMenu, cEvaluateMenu
	vLocalProductCollection = NULL;
	vProductEditingName = "Enter Product Name";
	vMainWindowPointer = NULL;
	vLocalSystemData = NULL;
	vLocalConfigurationData = NULL;

	vCurrentStatusLineText = "";
	vHaveAnImage = false;
	vWaitUntilDoneRunningForReloadTimer = 53210;
	vWaitUntilDoneRunningForExitTimer = 53211;
	vWaitToUpdateScreenTimerHandle = 53212;
	vCheckKeySwitchTimerHandle = 53213;
	vStartSetupTimerHandle = 53214;
	vOneSecondTimerHandle = 53215;
	vStartTimerHandle = 53216;
	vSetupTimerHandle = 53217;
	vGrabFocusTimerHandle = 53218;
	vShowStatusLineTimerHandle = 53219;
	vCalculateIntegrationTimerHandle = 53220;
	vSimulateContainerAfterXRaysAreOnTimerHandle = 53221;

	vStartingUpSource = false;
	vRestartingXRaysWithNewSettings = false;
	vStartedSource = false;
	vInitialized = false;
	vExitCode = 0;
	vMakeStatusLineYellowRed = false;
	vMaxDetectorHeight = 0;
	vHaveScaledImageToPaint = false;
	vWaitingToUpdateScreen = false;
	//vOldUnderRemoteControl = vGlobalUnderRemoteControl;
	vOldButton2Yellow = false;
	vOldErrorMessage = "None";
	vPercentBeltSpeed = 0;
	vOldXRayOnNotice = false;
	vImageMargin = 0;
	vXOffset = 0;
	vYOffset = 0;
	vXScale = 1;
	vYScale = 1;
	vOldQuickLearnImageIndex = 0;
	vProcessingEvaluateImagesStep = 0;
	vEvaluationComplete = false;
	vGotImageToAutoSize = false;

	vLocalCWndCollection.Add(this);
	vLocalCWndCollection.Add(&m_Calibrating);
	vLocalCWndCollection.Add(&m_XRaysOn);
	vLocalCWndCollection.Add(&m_Background);
	vLocalCWndCollection.Add(&m_SubFunction8Button);
	vLocalCWndCollection.Add(&m_Function3Button);
	vLocalCWndCollection.Add(&m_SubFunction1Button);
	vLocalCWndCollection.Add(&m_Width2);
	vLocalCWndCollection.Add(&m_Function5Button);
	vLocalCWndCollection.Add(&m_SubFunction4Display);
	vLocalCWndCollection.Add(&m_SubFunction8Display);
	vLocalCWndCollection.Add(&m_SubFunction7Display);
	vLocalCWndCollection.Add(&m_SubFunction5Button);
	vLocalCWndCollection.Add(&m_StatusLine);
	vLocalCWndCollection.Add(&m_SubFunction4Button);
	vLocalCWndCollection.Add(&m_DialogTitleStaticText);
	vLocalCWndCollection.Add(&m_Function1Button);
	vLocalCWndCollection.Add(&m_Function2Button);
	vLocalCWndCollection.Add(&m_Function4Button);
	vLocalCWndCollection.Add(&m_Reference);
	vLocalCWndCollection.Add(&m_RejectReason);
	vLocalCWndCollection.Add(&m_Simulating);
	vLocalCWndCollection.Add(&m_StatusLine2);
	vLocalCWndCollection.Add(&m_SubFunction1Display);
	vLocalCWndCollection.Add(&m_SubFunction2Button);
	vLocalCWndCollection.Add(&m_SubFunction2Display);
	vLocalCWndCollection.Add(&m_SubFunction3Button);
	vLocalCWndCollection.Add(&m_SubFunction3Display);
	vLocalCWndCollection.Add(&m_SubFunction5Display);
	vLocalCWndCollection.Add(&m_ThresholdForContainerBounds2);
	vLocalCWndCollection.Add(&m_SubFunction7Button);
	vLocalCWndCollection.Add(&m_SubFunction6Display);
	vLocalCWndCollection.Add(&m_SubFunction6Button);
}

CTDAutoSetupImageDisplayDialog::~CTDAutoSetupImageDisplayDialog()
{
}

void CTDAutoSetupImageDisplayDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_Calibrating, m_Calibrating);
	DDX_Control(pDX, IDC_XRaysOn, m_XRaysOn);
	DDX_Control(pDX, IDC_Background, m_Background);
	DDX_Control(pDX, IDC_SubFunction8Button, m_SubFunction8Button);
	DDX_Control(pDX, IDC_Function3Button, m_Function3Button);
	DDX_Control(pDX, IDC_SubFunction1Button, m_SubFunction1Button);
	DDX_Control(pDX, IDC_Width2, m_Width2);
	DDX_Control(pDX, IDC_Function5Button, m_Function5Button);
	DDX_Control(pDX, IDC_SubFunction4DisplayA, m_SubFunction4Display);
	DDX_Control(pDX, IDC_SubFunction8Display, m_SubFunction8Display);
	DDX_Control(pDX, IDC_SubFunction7Display, m_SubFunction7Display);
	DDX_Control(pDX, IDC_SubFunction5Button, m_SubFunction5Button);
	DDX_Control(pDX, IDC_StatusLine, m_StatusLine);
	DDX_Control(pDX, IDC_SubFunction4Button, m_SubFunction4Button);
	DDX_Control(pDX, IDC_DialogTitleStaticText1, m_DialogTitleStaticText);
	DDX_Control(pDX, IDC_Function1Button, m_Function1Button);
	DDX_Control(pDX, IDC_Function2Button, m_Function2Button);
	DDX_Control(pDX, IDC_Function4Button, m_Function4Button);
	DDX_Control(pDX, IDC_Reference, m_Reference);
	DDX_Control(pDX, IDC_RejectReason, m_RejectReason);
	DDX_Control(pDX, IDC_Simulating, m_Simulating);
	DDX_Control(pDX, IDC_StatusLine2, m_StatusLine2);
	DDX_Control(pDX, IDC_SubFunction1Display, m_SubFunction1Display);
	DDX_Control(pDX, IDC_SubFunction2Button, m_SubFunction2Button);
	DDX_Control(pDX, IDC_SubFunction2Display, m_SubFunction2Display);
	DDX_Control(pDX, IDC_SubFunction3Button, m_SubFunction3Button);
	DDX_Control(pDX, IDC_SubFunction3Display, m_SubFunction3Display);
	DDX_Control(pDX, IDC_SubFunction5Display, m_SubFunction5Display);
	DDX_Control(pDX, IDC_ThresholdForContainerBounds2, m_ThresholdForContainerBounds2);
	DDX_Control(pDX, IDC_SubFunction7Button, m_SubFunction7Button);
	DDX_Control(pDX, IDC_SubFunction6Display, m_SubFunction6Display);
	DDX_Control(pDX, IDC_SubFunction6Button, m_SubFunction6Button);
}


BEGIN_MESSAGE_MAP(CTDAutoSetupImageDisplayDialog, CDialog)
	//{{AFX_MSG_MAP(CTDAutoSetupImageDisplayDialog)
	ON_WM_SHOWWINDOW()
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
	ON_WM_PAINT()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_STN_CLICKED(IDC_DialogTitleStaticText1, &CTDAutoSetupImageDisplayDialog::OnDialogtitlestatictext1)
END_MESSAGE_MAP()


// CTDAutoSetupImageDisplayDialog message handlers


void CTDAutoSetupImageDisplayDialog::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	//if (vLocalSystemData->vActualEncoderRate) //can not calculate integration time from maximum encoder speed if encoder not moving
	//	CalculateBestIntegrationTime();

	//int TimerResult = SetTimer(vCalculateIntegrationTimerHandle,2000,NULL);  //wait 1 second for conveyor to get moving then calculate best integration time will keep calculating every second until 2 the same
	//if (!TimerResult)
	//	ReportErrorMessage("Error-Calculate Integration Timer Failed",cEMailInspx,32000);

	vLocalSystemData->vSetupANewProduct = true;

	bool TempNameOK = false;
	while ((vProductEditingName == "Enter Product Name") || (!TempNameOK) || (vProductEditingName == " "))
	{
		//Edit Product Name button was pressed
		CAlphaEntryDialog IAlphaEntryDialog;  
		IAlphaEntryDialog.vLocalSystemData = vLocalSystemData;
		IAlphaEntryDialog.vMainWindowPointer = vMainWindowPointer;
		//Set dialog box data titles and number value
		IAlphaEntryDialog.vEditString = vProductEditingName;
		CString TempText;
		TempText.LoadString(IDS_ProductName);
		IAlphaEntryDialog.m_DialogTitleStaticText = TempText;
		IAlphaEntryDialog.vInAutoSetup = true;
		IAlphaEntryDialog.vAllowDashes = true;
		IAlphaEntryDialog.vAllowPeriod = true;
		if (PasswordOK(cTemporaryInspxPassword,false))
			IAlphaEntryDialog.vAllowApproximate = true;
		TempText.LoadString(IDS_OriginalValue);
		IAlphaEntryDialog.m_DialogTitleStaticText2 = TempText + ": " + vProductEditingName;
		//Pass control to dialog box and display
		int nResponse = IAlphaEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			TempNameOK = true;
			//check if name is used as a product already
			CProduct *TempProductPointer = vMainWindowPointer->GetProductByName(IAlphaEntryDialog.vEditString);
			if (TempProductPointer)
			{
				CNoticeDialog TempNoticeDialog;
				CString TempText;
				TempText.LoadString(IDS_ProductNameinusealreadyyoumustchangeit);//"\n\nProduct Name in use already\nyou must change it"
				TempNoticeDialog.vNoticeText = TempText;
				TempNoticeDialog.vType = cErrorMessage;
				TempNoticeDialog.DoModal();
				TempNameOK = false;
			}
			else
			if (IAlphaEntryDialog.vEditString == "Enter Product Name")
			{
				CNoticeDialog TempNoticeDialog;
				TempText.LoadString(IDS_MustchangeProductName);//"\n\n\nMust change Product Name"
				TempNoticeDialog.vNoticeText = TempText;
				TempNoticeDialog.vType = cErrorMessage;
				TempNoticeDialog.DoModal();
				TempNameOK = false;
			}
			else
			{  //user changed name and pressed next step
				vProductEditingName = IAlphaEntryDialog.vEditString;

				SetDlgItemText(IDC_ProductName,vProductEditingName);
				vGlobalCurrentProduct->SetProductName(vProductEditingName);
				vLocalSystemData->vITIPCDig->DeleteLearnImages();
				if ((vLocalSystemData->vDriftCompensationADCOffset) || (vLocalSystemData->vDriftCompensationDACOffset))
				if (vGlobaluCSerialPort)
				{
					vLocalSystemData->vDriftCompensationADCOffset = 0;
					vLocalSystemData->vDriftCompensationDACOffset = 0;
					vMainWindowPointer->SendAllADCDetectorOffsetsToZero();
				}

				vLocalSystemData->vITIPCDig->DeleteBackupLearnFiles();
				vLocalSystemData->vITIPCDig->DeleteLearnFiles(vGlobalCurrentProduct);
			}
		}
		else
		{
			TempNameOK = true;
			vProductEditingName = "set to some value so will exit loop";
			ExitAutoSetupDialog(10);
		}
	}

	// TODO: Add your message handler code here
	vLocalSystemData->vTakingQuickLearnImages = false;
	//vLocalSystemData->vCollectingImagesToEvaluate = 0;
	vLocalSystemData->vCompletedEvaluation = false;
	if (vLocalSystemData->vXRayOnNotice)
	{
		CString TempText("");
		TempText.LoadString(IDS_XRAYON);
		SetDlgItemText(IDC_Calibrating, TempText);
		ReportErrorMessage("IDC_Calibrating 8: " + TempText, cWriteToLog,0);
		m_Calibrating.ShowWindow(SW_SHOW);
	}

	vLocalSystemData->vChangedSizeOfProduct = false;
	vLocalSystemData->vChangedSizeOfProductCheckDensityROI = 0;
	vMaxDetectorHeight = (float)(vLocalConfigurationData->vNumberOfXRayDetectors * vLocalConfigurationData->vDetectorLength);

	CString TempText;

	ReportErrorMessage("Start Express Auto Setup New Product", cUserAction,0);

	//set ejector position so be sure to get image and process it before passes ejector
	for (BYTE TempLoop = 0; TempLoop < cNumberOfEjectors; TempLoop++)
  	if (vGlobalCurrentProduct->vEjectorDistanceFromTriggerInInches[TempLoop] < vGlobalCurrentProduct->vProductBodyTriggerToImageDistanceInInches + vGlobalCurrentProduct->vProductImageWidth + 4)
		vGlobalCurrentProduct->SetEjectorBeltPositionOffset(TempLoop, (float)(vGlobalCurrentProduct->vProductBodyTriggerToImageDistanceInInches + vGlobalCurrentProduct->vProductImageWidth + 4 + (TempLoop * 10)));

	vGlobalCurrentProduct->vTypesOfRejectsToView = 0xFFFF; //view all reject types

	vLocalSystemData->vITIPCDig->DeleteLearnImages();
	vLocalSystemData->vITIPCDig->DeleteLearnFiles(vGlobalCurrentProduct);

	if (vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac)
	{
		vGlobalCurrentProduct->SetProductImageWidth(48);
		vGlobalCurrentProduct->SetProductLockOutWidth(48);
	}
	else
	if (vLocalConfigurationData->vScanTracType == cForteScanTrac)
	{
		vGlobalCurrentProduct->SetProductImageWidth(36);
		vGlobalCurrentProduct->SetProductLockOutWidth(36);
	}
	else
	if ((vLocalConfigurationData->vScanTracType == cSoloScanTrac) || (vGlobalScanTracType == cSoloPlusScanTrac))
	{
		vGlobalCurrentProduct->SetProductImageWidth(9); //solo new express autosize will start with 9 inch image
		vGlobalCurrentProduct->SetProductLockOutWidth(9);
	}
	else
	{
		vGlobalCurrentProduct->SetProductImageWidth(24);
		vGlobalCurrentProduct->SetProductLockOutWidth(24);
	}

	//set offset to zero if using offset
	if (vLocalConfigurationData->vSystemBodyTriggerToXRayBeam > 1)
	{
		if (vLocalConfigurationData->vScanTracType == cSoloScanTrac)
		{
			vGlobalCurrentProduct->SetProductBodyTriggerToImageBeltPositionOffset((float)(vLocalConfigurationData->vSystemBodyTriggerToXRayBeam - 2)); //set offset so leading edge of can will be 2" inside image
			ReportErrorMessage("Express New BT to Image Delay:" + dtoa(vGlobalCurrentProduct->vProductBodyTriggerToImageDistanceInInches, 2), cWriteToLog, 0);

			SetDlgItemText(IDC_SubFunction7Display, _T("0"));
		}
		else
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

	if (vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac)  //on Trio, can only have 672 lines in the image
	{
		float TempNewWidth = vGlobalCurrentProduct->vProductImageWidth;
		WORD TempNumberOfLines = (WORD)(TempNewWidth * vGlobalPixelsPerUnit * vGlobalCurrentProduct->vOverScanMultiplier);
		if (TempNumberOfLines > 672)
		{
			TempNewWidth =  (float)((672 / vGlobalPixelsPerUnit) / vGlobalCurrentProduct->vOverScanMultiplier);
			vGlobalCurrentProduct->SetProductImageWidth(TempNewWidth);
			vGlobalCurrentProduct->SetProductLockOutWidth(TempNewWidth);
		}
	}

	vLocalSystemData->vChangedSizeOfProduct = true;
	//vGlobalCurrentProduct->vProductImageHeightBottom = 0;
	//vGlobalCurrentProduct->vProductImageHeightTop = (float)(vLocalConfigurationData->vDetectorLength * vLocalConfigurationData->vNumberOfXRayDetectors);


	if (vGlobalCurrentProduct->vProductBodyTriggerToImageDistanceInInches < 2)
	{
		vGlobalCurrentProduct->vProductBodyTriggerToImageDistanceInInches = 2;
		if (vLocalConfigurationData->vSystemBodyTriggerToXRayBeam > 1)
		{
			double TempWhiteSpaceOnOneSideOfImage = (vGlobalCurrentProduct->vProductImageWidth / 1.6) * 0.3;
			if (TempWhiteSpaceOnOneSideOfImage > 4)
				TempWhiteSpaceOnOneSideOfImage = 4;

			vGlobalCurrentProduct->vProductBodyTriggerToImageDistanceInInches = (float)(vLocalConfigurationData->vSystemBodyTriggerToXRayBeam - TempWhiteSpaceOnOneSideOfImage);
		}

		vGlobalCurrentProduct->SetProductBodyTriggerToImageBeltPositionOffset(vGlobalCurrentProduct->vProductBodyTriggerToImageDistanceInInches);
	}

	vGlobalCurrentProduct->vHaveManuallySetRetriggerLockout = false;

	vLocalSystemData->vInExpressSetup = true;

	if (vLocalSystemData->vSystemRunMode == cStoppedSystemMode)
		vLocalSystemData->vInAutoSetup = true;

	//vLocalSystemData->vITIPCDig->vDisplayImageSizeX = 416;
	vLocalSystemData->vITIPCDig->vDisplayImageSizeY = (WORD) ResizeYCoor(338); //360 is regular image display;

	if (vLocalSystemData->vITIPCDig->vImage)
		delete vLocalSystemData->vITIPCDig->vImage;
	vLocalSystemData->vITIPCDig->vImage = NULL;

	vLocalSystemData->vITIPCDig->SetupImageBuffer(vLocalSystemData->vITIPCDig->vDisplayImageSizeX, vLocalSystemData->vITIPCDig->vDisplayImageSizeY);

	if (!vLocalSystemData->vITIPCDig->vImage)
	if (vLocalSystemData->vITIPCDig->vImageBuffer)
	if (ThereIsEnoughMemory(sizeof(CImgConn), "Frame Grabber Interface3"))
		vLocalSystemData->vITIPCDig->vImage = new CImgConn(vLocalSystemData->vITIPCDig->vImageBuffer,
			(WORD)vLocalSystemData->vITIPCDig->vDisplayImageSizeX, (WORD)vLocalSystemData->vITIPCDig->vDisplayImageSizeY,
			(WORD)vLocalSystemData->vITIPCDig->vBitsPerPixel, vLocalSystemData->vITIPCDig->vWindowHandle);

		if (vMainWindowPointer->InterlocksOK())
			vStartingUpSource = true;

	//source current is taken from the ~Default product
	if ((vLocalConfigurationData->vScanTracType == cSoloScanTrac) || (vGlobalScanTracType == cSoloPlusScanTrac))
	{
		//vGlobalCurrentProduct->vXRaySourceVoltage =	(float)(vLocalConfigurationData->vMaximumVoltage / 6);
		vGlobalCurrentProduct->vXRaySourceVoltage =	(float)(40.0 / 6.0); //was 50kV
		vGlobalCurrentProduct->vXRaySourceCurrent =	3;

		if ((vGlobalCurrentProduct->vEdgeLocationBottom >= 2) || (vGlobalCurrentProduct->vEdgeLocationHeight >= 2)) //if purple line values extreme, set them to one inch each
		{
			vGlobalCurrentProduct->SetEdgeLocationBottom(1);
			vGlobalCurrentProduct->SetEdgeLocationHeight(1);
		}
		if (vGlobalCurrentProduct->vReferenceWidth)
		{
			if ((vGlobalCurrentProduct->vBottomLocationLeftPixel + vGlobalCurrentProduct->vBottomLocationLengthPixel >= vGlobalCurrentProduct->vReferenceWidth)) //if purple line values extreme, set them to one inch each
			{
				vGlobalCurrentProduct->SetEdgeLocationBottom(0);
				vGlobalCurrentProduct->SetEdgeLocationHeight((float)(vGlobalCurrentProduct->vReferenceWidth / vGlobalPixelsPerUnit));
			}
		}
		else
		{
			if ((vGlobalCurrentProduct->vBottomLocationLeftPixel + vGlobalCurrentProduct->vBottomLocationLengthPixel >= vLocalSystemData->vITIPCDig->vOriginalBufferSizeY)) //if purple line values extreme, set them to one inch each
			{
				vGlobalCurrentProduct->SetEdgeLocationBottom(0);
				vGlobalCurrentProduct->SetEdgeLocationHeight((float)((vLocalSystemData->vITIPCDig->vOriginalBufferSizeY - 4) / vGlobalPixelsPerUnit));
			}
		}

	}
	else
	{
		if (vGlobalCurrentProduct->vProductImageHeightTop == 12)
		{
			vGlobalCurrentProduct->SetEdgeLocationHeight((float)(8));
			vGlobalCurrentProduct->SetEdgeLocationBottom((float)(2));
		}
		else
		{
			vGlobalCurrentProduct->SetEdgeLocationHeight((float)(vGlobalCurrentProduct->vProductImageHeightTop * 0.8));
			vGlobalCurrentProduct->SetEdgeLocationBottom((float)(vGlobalCurrentProduct->vProductImageHeightTop * 0.1));
		}
		ReportErrorMessage("SetEdgeLocationHeight: " + dtoa(vGlobalCurrentProduct->vEdgeLocationHeight, 2), cWriteToLog,0);
		ReportErrorMessage("vEdgeLocationBottom: " + dtoa(vGlobalCurrentProduct->vEdgeLocationBottom, 2), cWriteToLog,0);

		vGlobalCurrentProduct->SetBottomLocationLeft((float)(vLocalSystemData->vITIPCDig->vOriginalBufferSizeY * .1 / vGlobalPixelsPerUnit));
		vGlobalCurrentProduct->SetBottomLocationLength((float)(vLocalSystemData->vITIPCDig->vOriginalBufferSizeY * .9 / vGlobalPixelsPerUnit));
	}
	
	vLocalSystemData->vRampXRayCurrentAmount = vGlobalCurrentProduct->vXRaySourceCurrent;
	vLocalSystemData->vRampXRayVoltageAmount = vGlobalCurrentProduct->vXRaySourceVoltage;

	vMainWindowPointer->MakeProductionReportIfNeeded();
	vMainWindowPointer->ResetCounters();
	vMainWindowPointer->SetupProduct(vGlobalCurrentProduct, true);

	if (!vGlobalCurrentProduct->ProductHasImageFiles(vGlobalCurrentDirectory))
		vGlobalCurrentProduct->vSetupHistoryAverageDensity = 0;

	int TimerResult = SetTimer(vStartSetupTimerHandle,20,NULL);
	if (!TimerResult)
		ReportErrorMessage("Error-Start Timer Failed",cEMailInspx,32000);

	vHoldImageDisplayOnlyRejects = vLocalSystemData->vITIPCDig->vImageDisplayOnlyRejects;
	vLocalSystemData->vITIPCDig->vImageDisplayOnlyRejects = false;
	
	if (vLocalSystemData->vInAutoSetup)
	{
		vLocalSystemData->vITIPCDig->vCalibrateBodyTriggerReferenceEnabled = false;
		vLocalSystemData->vITIPCDig->vAverageEnabled = false;
		vLocalSystemData->vITIPCDig->vStructureEnabled = false;
	}
	
	vProductEditingName = *vGlobalCurrentProduct->GetProductName();
	vLocalSystemData->vCurrentProductName = vProductEditingName;

	CWnd *TempWindow = CWnd::GetDlgItem(IDC_Background);
	if (TempWindow)
		vLocalSystemData->vITIPCDig->vWindowHandle = TempWindow->m_hWnd;

	TempText.LoadString(IDS_XRAYON);
	SetDlgItemText(IDC_XRaysOn,TempText);

	vMode = cSizeMenu;
	m_SubFunction1Button.ShowWindow(SW_SHOW);
	m_SubFunction1Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
	TempText.LoadString(IDS_Abandon);
	SetDlgItemText(IDC_SubFunction1Button,TempText);

	if (!vLocalSystemData->vInAutoSetup)
	{	//started later when in autosetup	
		int TimerResult = SetTimer(vOneSecondTimerHandle,1000,NULL);
		if (!TimerResult)
			ReportErrorMessage("Error-One Second Timer Failed",cEMailInspx,32000);
	}

	if (vLocalSystemData->vInAutoSetup)
		this->SetWindowText(_T("TDAutoSetup"));
	else
		this->SetWindowText(_T("TDSetup"));

	if (vLocalSystemData->vInAutoSetup)
		ReportErrorMessage("Start Auto Setup Product :" + *vGlobalCurrentProduct->GetProductName(), cUserAction,0);
	else
		ReportErrorMessage("Start Setup Product :" + *vGlobalCurrentProduct->GetProductName(), cUserAction,0);

	StartSizeMenu();

	UpdateSubFunction4Button();

	//vGlobalMouseMoveCounter = 0;

	vLocalConfigurationData->vNumberOfQuickLearnImagesToTake = 10;
	vLocalSystemData->vQuickLearnImageIndex = 0;

	if (vLocalSystemData->vXRayOnNotice)
	{
		if (vIntegrationTimeSet)
			SetStatusLine("Send one container to AutoSize.", 0, false);
		else
			SetStatusLine("Reading conveyor speed.\nPlease wait...", 0, false);

		TempText.LoadString(IDS_XRAYON);
		SetDlgItemText(IDC_XRaysOn,TempText);
		ReportErrorMessage("IDC_Calibrating 10: " + TempText, cWriteToLog,0);
		SetDlgItemText(IDC_Calibrating, TempText);
	}
	else
		SetStatusLine("Starting X-Rays.  Please wait...", 0, false);

	if (!vLocalSystemData->vITIPCDig->vShowOnlyRealImages) //if simulating in office skip integration time and exposure adjust for size
	{
		vAdjustedExposureForAutoSize = 1;
		vIntegrationTimeSet = true;
	}

	TimerResult = SetTimer(vGrabFocusTimerHandle,50,NULL);  //WAS 100
	if (!TimerResult)
		ReportErrorMessage("Error-GrabFocus Timer Failed",cEMailInspx, 32000);

	::SetupMenu(vLocalCWndCollection);
	vLocalSystemData->vShiftProductCode = " ";
}

void CTDAutoSetupImageDisplayDialog::OnFunction1Button()
{
	// TODO: Add your control notification handler code here
}

void CTDAutoSetupImageDisplayDialog::OnFunction2Button()
{
	// TODO: Add your control notification handler code here
}

void CTDAutoSetupImageDisplayDialog::OnFunction3Button()
{
}

void CTDAutoSetupImageDisplayDialog::OnFunction4Button()
{
	// TODO: Add your control notification handler code here
}

afx_msg void CTDAutoSetupImageDisplayDialog::OnFunction5Button()
{
	// TODO: Add your control notification handler code here
}

afx_msg void CTDAutoSetupImageDisplayDialog::OnSubFunction1Button()
{
	// Abandon button pressed
	CYesNoDialog TempYesNoDialog;
	CString TempText;
	TempText.LoadString(IDS_AbandonSetupChangesandExit);//"\n\nAbandon Setup Changes and Exit?"
	TempYesNoDialog.vNoticeText = TempText;
	TempYesNoDialog.vQuestionType = cConfirmAbandon;
	int TempResult = TempYesNoDialog.DoModal();
	if (TempResult == IDOK)
	{
		ReportErrorMessage("Operator pressed Abort Button from Express Setup Menu", cUserAction, 0);
		ExitAutoSetupDialog(10);
	}
	else
		this->SetActiveWindow();
}

void CTDAutoSetupImageDisplayDialog::OnSubFunction2Button()
{
	// TODO: Add your control notification handler code here
}

void CTDAutoSetupImageDisplayDialog::OnSubFunction3Button()
{
	if (vMode == cTestEjectorsMenu)
	if (PasswordOK(cSetupPassword,true))
	{
		//Create dialog box
		CNewConfigProductEjectorDialog IConfigureProductEjectorsDialog;  
		
		//Set dialog box data like how many products there are
		IConfigureProductEjectorsDialog.vLocalConfigurationData = vLocalConfigurationData;
		IConfigureProductEjectorsDialog.vLocalSystemData = vLocalSystemData;
		//IConfigureProductEjectorsDialog.vInSideTrip = true;
		IConfigureProductEjectorsDialog.vMainWindowPointer = vMainWindowPointer;
		IConfigureProductEjectorsDialog.vLocalProductCollection = vLocalProductCollection;
		IConfigureProductEjectorsDialog.vShowPreviousAndNextButtons = false;
		//Pass control to dialog box and display
		//IEditProductDialog.vEPLocalNumberOfProducts = vCPLocalNumberOfProducts;
		int nResponse = IConfigureProductEjectorsDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if ((nResponse == IDOK) || (nResponse == 20))
		{
			// TODO: Place code here to handle when the dialog is accepted
		}
		else 
		if (nResponse == 10)
		{
			//  Main Menu button was pressed
			CDialog::EndDialog(10);
		}
	}
}

void CTDAutoSetupImageDisplayDialog::OnSubFunction4Button()
{
	// TODO: Add your control notification handler code here
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
	if (vMode != cInitializingDisplay)
	if ((!vStartingUpSource) && (!vRestartingXRaysWithNewSettings) &&
		((!vLocalSystemData->vXRayOnNotice) || (vLocalSystemData->vSystemRunMode == cStoppedSystemMode)))
	{  //run system if not running
		if (vMainWindowPointer->InterlocksOK())
		{
			int TimerResult = SetTimer(vStartTimerHandle,20,NULL);
			if (!TimerResult)
				ReportErrorMessage("Error-Start Timer Failed",cEMailInspx,32000);
			vStartingUpSource = true;
		}
	}
}

void CTDAutoSetupImageDisplayDialog::OnSubFunction5Button()
{
	if ((vMode == cSizeMenu) && (!vNeedToAutoSizeNextContainer))
	{ //have autosized, but operator wants to try sizing again
		vNeedToAutoSizeNextContainer = true;
		m_SubFunction5Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction5Button.GetRectToInvalidateArrow());

		m_SubFunction8Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction8Button.GetRectToInvalidateArrow());

		if (vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac)
		{
			vGlobalCurrentProduct->SetProductImageWidth(48);
			vGlobalCurrentProduct->SetProductLockOutWidth(48);
		}
		else
		if (vLocalConfigurationData->vScanTracType == cForteScanTrac)
		{
			vGlobalCurrentProduct->SetProductImageWidth(36);
			vGlobalCurrentProduct->SetProductLockOutWidth(36);
		}
		else
		if ((vLocalConfigurationData->vScanTracType == cSoloScanTrac) || (vGlobalScanTracType == cSoloPlusScanTrac))
		{
			vGlobalCurrentProduct->SetProductImageWidth(9); //solo new express autosize will start with 9 inch image
			vGlobalCurrentProduct->SetProductLockOutWidth(9);
		}
		else
		{
			vGlobalCurrentProduct->SetProductImageWidth(24);
			vGlobalCurrentProduct->SetProductLockOutWidth(24);
		}

		//set offset to zero if using offset
		if (vLocalConfigurationData->vSystemBodyTriggerToXRayBeam > 1)
		{
			if ((vLocalConfigurationData->vScanTracType == cSoloScanTrac) || (vGlobalScanTracType == cSoloPlusScanTrac))
			{
				vGlobalCurrentProduct->SetProductBodyTriggerToImageBeltPositionOffset((float)(vLocalConfigurationData->vSystemBodyTriggerToXRayBeam - 2)); //set offset so leading edge of can will be 2" inside image
				ReportErrorMessage("Express New BT to Image Delay:" + dtoa(vGlobalCurrentProduct->vProductBodyTriggerToImageDistanceInInches, 2), cWriteToLog, 0);

				SetDlgItemText(IDC_SubFunction7Display, _T("0"));
			}
			else
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

		if (vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac)  //on Trio, can only have 672 lines in the image
		{
			float TempNewWidth = vGlobalCurrentProduct->vProductImageWidth;
			WORD TempNumberOfLines = (WORD)(TempNewWidth * vGlobalPixelsPerUnit * vGlobalCurrentProduct->vOverScanMultiplier);
			if (TempNumberOfLines > 672)
			{
				TempNewWidth =  (float)((672 / vGlobalPixelsPerUnit) / vGlobalCurrentProduct->vOverScanMultiplier);
				vGlobalCurrentProduct->SetProductImageWidth(TempNewWidth);
				vGlobalCurrentProduct->SetProductLockOutWidth(TempNewWidth);
			}
		}

		if (vGlobalCurrentProduct->vProductBodyTriggerToImageDistanceInInches < 2)
		{
			vGlobalCurrentProduct->vProductBodyTriggerToImageDistanceInInches = 2;
			if (vLocalConfigurationData->vSystemBodyTriggerToXRayBeam > 1)
			{
				double TempWhiteSpaceOnOneSideOfImage = (vGlobalCurrentProduct->vProductImageWidth / 1.6) * 0.3;
				if (TempWhiteSpaceOnOneSideOfImage > 4)
					TempWhiteSpaceOnOneSideOfImage = 4;

				vGlobalCurrentProduct->vProductBodyTriggerToImageDistanceInInches = (float)(vLocalConfigurationData->vSystemBodyTriggerToXRayBeam - TempWhiteSpaceOnOneSideOfImage);
			}

			vGlobalCurrentProduct->SetProductBodyTriggerToImageBeltPositionOffset(vGlobalCurrentProduct->vProductBodyTriggerToImageDistanceInInches);
		}
		ReportErrorMessage("Operator pressed Redo Size Button", cUserAction, 0);
		vStartedResize = true;
		ChangedProductSettingsSoReload();
	}
	else
	if ((vMode == cEvaluateMenu) && ((vLocalSystemData->vCompletedEvaluation) || (vLocalSystemData->vQuickLearnImageIndex)))
	{ //have started to evaluate, or finished evaluation, and want to start over
		m_SubFunction5Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction5Button.GetRectToInvalidateArrow());

		m_SubFunction8Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction8Button.GetRectToInvalidateArrow());

		vLocalSystemData->vCompletedEvaluation = false;
		vEvaluationComplete = false;

		vOldQuickLearnImageIndex = 0;
		vLocalSystemData->vQuickLearnImageIndex = 0;

		vLocalConfigurationData->vNumberOfQuickLearnImagesToTake = 10;

		vLocalSystemData->vIncludeRejectsInAverageAndStructure = true;

		if ((vLocalConfigurationData->vScanTracType == cSoloScanTrac) || (vGlobalScanTracType == cSoloPlusScanTrac))
			vLocalSystemData->vEvaluateAndOrQuickLearn = cEvaluateAndQuickLearn;  //cQuickLearnOnly, cEvaluateAndQuickLearn, cEvaluateOnly
		else
			vLocalSystemData->vEvaluateAndOrQuickLearn = cEvaluateOnly;  //cQuickLearnOnly, cEvaluateAndQuickLearn, cEvaluateOnly

		vLocalSystemData->vTakingQuickLearnImages = true;
		SetStatusLine("Send " + dtoa(cNumberOfImagesToEvaluate, 0) + " containers to evaluate.", 2000, false);
		ReportErrorMessage("Operator pressed Redo Evaluate Button", cUserAction, 0);
	}
	else //in SubFunction5Button
	if (vMode == cTestEjectorsMenu)
	{
		if (vLocalSystemData->vTestEjectNextContainer)
		{ //abort Test Eject Next
			vLocalSystemData->vTestEjectNextContainer = 0;
			UpdateButtons();
		}
		else
		{
			m_SubFunction8Button.ShowWindow(SW_SHOW);
			CString TempText;
			TempText.LoadString(IDS_Save);
			SetDlgItemText(IDC_SubFunction8Button,TempText);
			TestEjectNextContainer(0, 0);  //next container, first ejector

			SetStatusLine("Press Save if ejected properly,\nelse press Adjust Ejector Timing.", 2000, true);

			m_SubFunction3Button.ShowWindow(SW_SHOW);
			SetDlgItemText(IDC_SubFunction3Button, _T("Adjust Ejector Timing"));
		}
	}
}

void CTDAutoSetupImageDisplayDialog::OnSubFunction6Button()
{
	if (vMode == cTestEjectorsMenu)
	if ((vLocalConfigurationData->vEjector[1].vEnabled) && (vEjectorUsed[1]))
	{
		if (vLocalSystemData->vTestEjectNextContainer)
		{ //abort Test Eject Next
			vLocalSystemData->vTestEjectNextContainer = 0;
			UpdateButtons();
		}
		else
		{
			m_SubFunction8Button.ShowWindow(SW_SHOW);
			CString TempText;
			TempText.LoadString(IDS_Save);
			SetDlgItemText(IDC_SubFunction8Button,TempText);
			TestEjectNextContainer(0, 1);  //next container, second ejector

			SetStatusLine("Press Save if ejected properly,\nelse press Adjust Ejector Timing.", 2000, true);

			m_SubFunction3Button.ShowWindow(SW_SHOW);
			SetDlgItemText(IDC_SubFunction3Button, _T("Adjust Ejector Timing"));
		}
	}
}

void CTDAutoSetupImageDisplayDialog::OnSubFunction7Button()
{
	if (vMode == cTestEjectorsMenu)
	if ((vLocalConfigurationData->vEjector[2].vEnabled) && (vEjectorUsed[2]))
	{
		if (vLocalSystemData->vTestEjectNextContainer)
		{ //abort Test Eject Next
			vLocalSystemData->vTestEjectNextContainer = 0;
			UpdateButtons();
		}
		else
		{
			m_SubFunction8Button.ShowWindow(SW_SHOW);
			CString TempText;
			TempText.LoadString(IDS_Save);
			SetDlgItemText(IDC_SubFunction8Button,TempText);
			TestEjectNextContainer(0, 2);  //next container, third ejector

			SetStatusLine("Press Save if ejected properly,\nelse press Adjust Ejector Timing.", 2000, true);

			m_SubFunction3Button.ShowWindow(SW_SHOW);
			SetDlgItemText(IDC_SubFunction3Button, _T("Adjust Ejector Timing"));
		}
	}
}

void CTDAutoSetupImageDisplayDialog::OnSubFunction8Button()
{
	// TODO: Add your control notification handler code here
	if (vMode == cSizeMenu)
	{
		if (!vNeedToAutoSizeNextContainer)
		{
			ReportErrorMessage("Operator pressed Next Button from Express Size Menu", cUserAction, 0);
			FinishSizeMenu(false, false);
			StartExposureMenu();
		}
	}
	else
	if (vMode == cExposureMenu)
	{
		ReportErrorMessage("Operator pressed Next Button from Express Exposure Menu", cUserAction, 0);
		FinishExposureMenu();
		StartEvaluateMenu();
	}
	else
	if (vMode == cEvaluateMenu)
	{
		if ((vEvaluationComplete) && (!vGlobalProcessEvaluateImagesThread))
		{
			FinishEvaluateMenu();
			StartTestEjectorsMenu();
			ReportErrorMessage("Operator pressed Next Button from Express Evaluate Menu", cUserAction, 0);
		}
	}
	else
	if (vMode == cTestEjectorsMenu)
	{
		if ((vEvaluationComplete) && (!vGlobalProcessEvaluateImagesThread))
		{
			//FinishEvaluateMenu();
			
			ReportErrorMessage("Operator pressed Finish Button from Express Test Ejector Menu", cUserAction, 0);
			ExitAutoSetupDialog(1);
		}
	}
}

HBRUSH CTDAutoSetupImageDisplayDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
	//if (TempControlID == IDC_ErrorMessage)
	//{
	//	TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_ErrorMessage, 5);
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

	if (TempControlID == IDC_StatusLine)
	{
		TextSize = SetTextSizeMultilineDisplay(TempDialogHwnd, pWnd, pDC, &m_StatusLine, 5);
		pDC->SetTextColor(cDarkBlue);
		pDC->SetBkMode(TRANSPARENT);
		if ((vLocalSystemData->vYellowMessageButtonYellow) || (vMakeStatusLineYellowRed))
		{
			pDC->SetTextColor(cRed);
			return vLocalSystemData->vYellowBrush;
		}
		else
		if (vMakeStatusLineGreen)
		{
			return vLocalSystemData->vGreenBrush; //vLightGreenBrush;
		}
		else
		{
			return vLocalSystemData->vGrayBrush;
		}
	}

	if (TempControlID == IDC_XRaysOn)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_XRaysOn, 5);
		pDC->SetTextColor(cYellow);
		pDC->SetBkColor(cRed);
		return vLocalSystemData->vRedBrush;
	}
	if (TempControlID == IDC_Calibrating)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Calibrating, 5);
		pDC->SetTextColor(cYellow);
		pDC->SetBkColor(cRed);
		return vLocalSystemData->vRedBrush;
	}

	if (TempControlID == IDC_SubFunction4Button)
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction4Button, 5);  //5 is medium large
		if (vLocalSystemData->vYellowMessageButtonYellow)
		{
			pDC->SetTextColor(cButtonTextColor);
			pDC->SetBkMode(TRANSPARENT);
			return vLocalSystemData->vYellowBrush;
		}
		if ((!vStartingUpSource) && (!vRestartingXRaysWithNewSettings) && ((!vLocalSystemData->vXRayOnNotice) || //changed from and to or
			(vLocalSystemData->vSystemRunMode == cStoppedSystemMode)))
		{
			pDC->SetAttribDC(pDC->m_hDC);
			pDC->SelectObject(vLocalSystemData->vCurrentLargeFont);
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cButtonTextColor);
			return vLocalSystemData->vGreenBrush;
		}
	}

	if (TempControlID == IDC_SubFunction5Button)
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction5Button, 5);  //5 is medium large
		if (vMode == cTestEjectorsMenu)
		{
			if (vLocalSystemData->vTestEjectNextContainer)
			{ 
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(cButtonTextColor);
				return vLocalSystemData->vRedBrush;
			}
			else
			{
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(cButtonTextColor);
				return vLocalSystemData->vGreenBrush;
			}
		}
	}

	if (TempControlID == IDC_SubFunction8Button)
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction8Button, 5);  //5 is medium large
		//if (vEvaluationComplete)
		{
			pDC->SetTextColor(cButtonTextColor);
			pDC->SetBkMode(TRANSPARENT);
			return vLocalSystemData->vGreenBrush;
		}
	}

	if (pWnd->GetDlgCtrlID() == IDC_Function1Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function1Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Function2Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function2Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Function3Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function3Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Function4Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function4Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Function5Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function5Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction1Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction1Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction2Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction2Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction3Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction3Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction4Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction4Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction5Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction5Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction6Button) 
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction6Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction7Button) 
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction7Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction8Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction8Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	
	return hbr;
}

void CTDAutoSetupImageDisplayDialog::OnPaint()
{
	// TODO: Add your message handler code here
	// Do not call CDialog::OnPaint() for painting messages
	CPaintDC dc(this); // device context for painting
	WORD TempOffsetX = (WORD) ResizeXCoor(108);
	WORD TempOffsetY = (WORD) ResizeYCoor(43);
	BYTE *TempDataPoint;
	if (vAdjustedExposureForAutoSize > 1)
	if ((dc.m_ps.rcPaint.top <= vLocalSystemData->vITIPCDig->vMainDisplayImageSizeY + TempOffsetY) && (dc.m_ps.rcPaint.bottom >= TempOffsetY) &&
		(dc.m_ps.rcPaint.left <= vLocalSystemData->vITIPCDig->vMainDisplayImageSizeX + TempOffsetX) && (dc.m_ps.rcPaint.right >= TempOffsetX))
	if (vLocalSystemData->vITIPCDig->vImageBuffer)
	if ((vHaveAnImage) || (vHaveScaledImageToPaint))
	{
		TempDataPoint = vLocalSystemData->vITIPCDig->vImageBuffer;
		
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

		TempIterator = TempBitmapColorArraySize - vLocalSystemData->vITIPCDig->vDisplayImageSizeX * 4;

		for (WORD TempLoopY = 0; TempLoopY < vLocalSystemData->vITIPCDig->vDisplayImageSizeY; TempLoopY++)
		{
			for (WORD TempLoopX = 0; TempLoopX < vLocalSystemData->vITIPCDig->vDisplayImageSizeX; TempLoopX++)
			{
				if ((TempOffsetX + TempLoopX <= dc.m_ps.rcPaint.right) && (TempOffsetX + TempLoopX >= dc.m_ps.rcPaint.left) &&
					(TempOffsetY + TempLoopY >= dc.m_ps.rcPaint.top) && (TempOffsetY + TempLoopY <= dc.m_ps.rcPaint.bottom))
				{
					TempBitmapColorArray[TempIterator] = *TempDataPoint;//blUE
					TempBitmapColorArray[TempIterator + 1] = *TempDataPoint;//green
					TempBitmapColorArray[TempIterator + 2] = *TempDataPoint;//red
					TempBitmapColorArray[TempIterator + 3] = 255;//alpha
					//dc.SetPixelV(TempOffsetX + TempLoopX,TempOffsetY + TempLoopY, RGB(*TempDataPoint,*TempDataPoint,*TempDataPoint)); //draw pixel in intensity
				}
				TempDataPoint++;
				TempIterator = TempIterator + 4;
			}
			TempIterator = TempIterator - vLocalSystemData->vITIPCDig->vDisplayImageSizeX * 8;  //multiply by 8, 4 points per pixel, go back two rows and forward one
		}

		SetDIBits(dc, TempBitmapOfReject, 0, TempBitmapInfo.bmiHeader.biHeight, (LPVOID)TempBitmapColorArray, &TempBitmapInfo, DIB_RGB_COLORS);
		free(TempBitmapColorArray);

		TempCompatableDeviceContext.SelectObject(TempBitmapOfReject);
		dc.BitBlt(TempOffsetX, TempOffsetY, TempBitmapInfo.bmiHeader.biWidth, TempBitmapInfo.bmiHeader.biHeight, &TempCompatableDeviceContext, 0, 0, SRCCOPY);
		
		DeleteObject(TempCompatableDeviceContext);
		DeleteObject(TempBitmapOfReject);

		////just for testing
		//if (vNeedToAutoSizeNextContainer)
		//	ReportErrorMessage("vNeedToAutoSizeNextContainer = true", cWriteToLog,0);
		//else
		//	ReportErrorMessage("vNeedToAutoSizeNextContainer = false", cWriteToLog,0);

		//if (vGotImageToAutoSize)
		//	ReportErrorMessage("vGotImageToAutoSize = true", cWriteToLog,0);
		//else
		//	ReportErrorMessage("vGotImageToAutoSize = false", cWriteToLog,0);

		if ((vNeedToAutoSizeNextContainer) && (vGotImageToAutoSize))
		{
			AutoSizeImage();
		}
	}
	// Do not call CDialog::OnPaint() for painting messages
}


void CTDAutoSetupImageDisplayDialog::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
  if (nIDEvent == vOneSecondTimerHandle)
  {
		OneSecondUpdate();
	}
	else	
  if (nIDEvent == vWaitUntilDoneRunningForReloadTimer)
  {
		CString TempText = vCurrentStatusLineText + ".";
		SetStatusLine(TempText, 1000, vMakeStatusLineGreen);
		if (((vLocalSystemData->vContainerCollection->GetCount() == 0) || (vLocalSystemData->vActualEncoderRate == 0)) || (vLocalSystemData->vSystemRunMode == cStoppedSystemMode))
		{
			if (vLocalSystemData->vContainerCollection->GetCount())
			vMainWindowPointer->RemoveAllContainers();
			if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)
				vMainWindowPointer->StopRunning(false);

			KillTimer(vWaitUntilDoneRunningForReloadTimer);
			DoneRunningSoReload();
		}
	}
	else
  if (nIDEvent == vWaitUntilDoneRunningForExitTimer)
  {
		CString TempText = vCurrentStatusLineText + ".";
		SetStatusLine(TempText, 1000, vMakeStatusLineGreen);
		if (((vLocalSystemData->vContainerCollection->GetCount() == 0) ||
			(vLocalSystemData->vActualEncoderRate == 0)) ||
			(vLocalSystemData->vSystemRunMode == cStoppedSystemMode))
		{
			KillTimer(vWaitUntilDoneRunningForExitTimer);
			DoneRunningSoExit();
		}
	}
	else
  if (nIDEvent == vShowStatusLineTimerHandle)
  {
		KillTimer(vShowStatusLineTimerHandle);
		m_StatusLine.ShowWindow(SW_SHOW);
	}
	else
  if (nIDEvent == vCalculateIntegrationTimerHandle)
  {
		if (!vIntegrationTimeSet)
		{
			CalculateBestIntegrationTime();
			if (vIntegrationTimeSet)
				KillTimer(vCalculateIntegrationTimerHandle); //stop timer if integration time was set and verified
		}
		else
		{
			KillTimer(vCalculateIntegrationTimerHandle); //stop timer if integration time was set and verified
			if (vLocalSystemData->vLogFile.vLogSerialData)
				vLocalSystemData->vLogFile.WriteToLogFile("TDAutoSetup product, Express Setup, Simulate a container in one second for start up",cDebugMessage);

			if (vGlobalScanTracType == cAllegroScanTrac)
			{
				ReportErrorMessage("Allegro Express Started X-Rays, Simulate container in 2 seconds", cWriteToLog,0);
				::PostThreadMessage(vGlobalCallingThreadID, cSimulateOneContainerIn10MSMessage, 2000, 0);
			}
		}
	}
	else
  if (nIDEvent == vSetupTimerHandle) // called to re-setup the product
  {
		//if done processing quick learn images
		if (vLocalSystemData->vSystemRunMode == cStoppedSystemMode)
		{

			//9/30/2013
			//if (vMode == cSizeMenu)
			//if (!vNeedToAutoSizeNextContainer)
			//	TempText = "Press NEXT.  Image was sized.";

			if (vMode != cSizeMenu)
			{
				CString TempText;
				TempText.LoadString(IDS_StartingSystem);
				SetStatusLine(TempText, 2000, true);
			}

			KillTimer(vSetupTimerHandle);
			KillTimer(vCheckKeySwitchTimerHandle);
			//DrawImageOnScreen();
			//OnPaint();
			//save the image
			DWORD TempBufferSize = vLocalSystemData->vITIPCDig->vOriginalBufferSize;
			WORD TempBufferSizeX = vLocalSystemData->vITIPCDig->vOriginalBufferSizeX;
			WORD TempBufferSizeY = vLocalSystemData->vITIPCDig->vOriginalBufferSizeY;

			if (vStartedSource)
			{
				if (vLocalSystemData->vLogFile.vLogSerialData)
					vLocalSystemData->vLogFile.WriteToLogFile("Setup Product Source running",cDebugMessage);
				CString TempText("");
				TempText.LoadString(IDS_XRAYON);
				ReportErrorMessage("IDC_Calibrating 11: " + TempText, cWriteToLog,0);
				SetDlgItemText(IDC_Calibrating,TempText);
				m_Calibrating.ShowWindow(SW_SHOW);
				vStartingUpSource = false;
			}
			else
			if (vLocalSystemData->vLogFile.vLogSerialData)
				vLocalSystemData->vLogFile.WriteToLogFile("Setup Product Source not running",cDebugMessage);

			vMainWindowPointer->SetupProduct(vGlobalCurrentProduct, vStartedSource);//leave source on

			if ((vLocalSystemData->vITIPCDig->vOriginalImage) && (vLocalSystemData->vITIPCDig->vOriginalImage->vGreyImage))
			{
				FillMemory(vLocalSystemData->vITIPCDig->vOriginalImage->vGreyImage,
					vLocalSystemData->vITIPCDig->vOriginalBufferSize,255);
			}

			if (vMainWindowPointer->InterlocksOK())
			{
				int TimerResult = SetTimer(vStartTimerHandle,100,NULL);
				if (!TimerResult)
					ReportErrorMessage("Error-Start Timer Failed",cEMailInspx,32000);
			}
			//WaitThenDrawImageOnScreen();
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
				if ((vMode == cSizeMenu) && (vStartedResize))
				{
					vStartedResize = false;
					SetStatusLine("Send one container to AutoSize.", 2000, false);
				}
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
					ReportErrorMessage("IDC_Calibrating 1: " + TempText, cWriteToLog,0);
					SetDlgItemText(IDC_Calibrating,TempText);
					m_Calibrating.ShowWindow(SW_SHOW);
					vLocalSystemData->vModeToRunAfterCalibrateOffset = cAutoSetupRunningSystem;
					vLocalSystemData->vCalibrateErrorCount = 0;
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
		}
		UpdateSubFunction4Button();
	}
	else
	if (nIDEvent == vWaitToUpdateScreenTimerHandle)
	{
		KillTimer(vWaitToUpdateScreenTimerHandle);

		vWaitingToUpdateScreen = false;
		DrawImageOnScreen();
	}
	else
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
					vStartedSource = true;
				}
			}
		}
	}
	else
	if (nIDEvent == vSimulateContainerAfterXRaysAreOnTimerHandle)
	{
		ReportErrorMessage("Express Setup, Need to Simulate an image after X-Rays On.  State: " + dtoa(vLocalSystemData->vXRayState, 0), cWriteToLog, 0);
		if ((vLocalSystemData->vXRayOnNotice) && (!vStartingUpSource))
		{
			if (vLocalSystemData->vLogFile.vLogSerialData)
				vLocalSystemData->vLogFile.WriteToLogFile("TDAutoSetup product, Express Setup, Simulate a container in one second for start up",cDebugMessage);
			::PostThreadMessage(vGlobalCallingThreadID, cSimulateOneContainerIn10MSMessage, 1000, 0);
			KillTimer(vSimulateContainerAfterXRaysAreOnTimerHandle);
		}
	}
	else
	if (nIDEvent == vStartSetupTimerHandle)  //done only when enter the window
	{
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
			}
			//DrawImageOnScreen();
		}
	}
	CDialog::OnTimer(nIDEvent);
}

void CTDAutoSetupImageDisplayDialog::SetStatusLine(CString TempStatusLineText, WORD TempBlink, bool TempMakeGreen) 
{
	if (!vLocalSystemData->vYellowMessageButtonYellow)
	{
		WORD TempLength = TempStatusLineText.GetLength();
		int TempCarrageReturnPosition = TempStatusLineText.Find(_T("\n"));
		if ((TempLength > 36) || (TempCarrageReturnPosition != -1))
			m_StatusLine.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		else
			m_StatusLine.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
		vMakeStatusLineGreen = TempMakeGreen;
		SetDlgItemText(IDC_StatusLine,TempStatusLineText);

		if (TempBlink) 
		if ((vMode == cSizeMenu) || (vLocalSystemData->vContainerRate < 25))
		{
			m_StatusLine.ShowWindow(SW_HIDE);
			int TimerResult = SetTimer(vShowStatusLineTimerHandle, TempBlink, NULL);
			if (!TimerResult)
				ReportErrorMessage("Error-ShowStatusLine Timer Failed",cEMailInspx,32000);
		}
	}
	vMakeStatusLineGreen = TempMakeGreen;
	vCurrentStatusLineText = TempStatusLineText;
}

BOOL CTDAutoSetupImageDisplayDialog::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message != 275)
	if (pMsg->message == cOffsetCalibrationNextStep)
	{
		CString TempText;
		TempText.LoadString(IDS_Wait);
		m_Calibrating.ShowWindow(SW_SHOW);
		if (vLocalSystemData->vCalibrateUsingDAC)
			SetDlgItemText(IDC_Calibrating,TempText + " " + dtoa(vLocalSystemData->vCalibrationImageNumber ,0));
		else
			SetDlgItemText(IDC_Calibrating,TempText + " " + dtoa(vLocalSystemData->vCalibrationImageNumber + 12 ,0));

		ReportErrorMessage("IDC_Calibrating 2: " + TempText + " " + dtoa(vLocalSystemData->vCalibrationImageNumber ,0), cWriteToLog,0);
		return true;
	}
	else
	if (pMsg->message == cEvaluateImagesProcessingFinishedMessage)
	{
		vEvaluationComplete = true;
		CString TempText;
		TempText = "Press Next.  Evaluate Complete.";
		SetStatusLine(TempText, 2000, true);
		m_SubFunction8Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
		m_SubFunction8Button.ShowWindow(SW_SHOW);
		TempText.LoadString(IDS_Next);
		SetDlgItemText(IDC_SubFunction8Button,TempText);

		//m_SubFunction5Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		//m_SubFunction5Button.ShowWindow(SW_SHOW);
		//TempText.LoadString(IDS_EvaluateAgain);
		//SetDlgItemText(IDC_SubFunction5Button,TempText);
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
		/*
	if (pMsg->message == cXRaysOnMessage)
	{
		if (vLocalSystemData->vLogFile.vLogSerialData)
			vLocalSystemData->vLogFile.WriteToLogFile("Autosetup X-Ray Step: " + dtoa(pMsg->lParam,0),cDebugMessage);
		if (pMsg->lParam == 26)
		{
			vStartingUpSource = false;
			CString TempText("");
			TempText.LoadString(IDS_XRAYON);
			ReportErrorMessage("IDC_Calibrating 3: " + TempText, cWriteToLog,0);
			SetDlgItemText(IDC_Calibrating, TempText);
			m_Calibrating.ShowWindow(SW_SHOW);
		
			if (vMode == cSizeMenu)
			{
				if (vNeedToAutoSizeNextContainer)
				if (vLocalSystemData->vXRayOnNotice)
				{
					if (vIntegrationTimeSet)
					{
						vMakeStatusLineGreen = " ";
						SetStatusLine(" ", 0, false);

						SetStatusLine("Send one container to AutoSize.", 0, false); //this leaves a dot on the line below the status line???? why
					}
					else
					{
						SetStatusLine("Reading conveyor speed.\nPlease wait...", 0, false);
						vSamplingConveyorSpeed = true;
					}
				}
			}
			else
			if (vMode == cSizeMenu)
				SetStatusLine("Send one container\n to verify exposure.", 2000, false);
			else
				SetStatusLine("Send one container.", 2000, false);

			int TimerResult = SetTimer(vCalculateIntegrationTimerHandle,3000,NULL);  //wait 1 second for conveyor to get moving then calculate best integration time will keep calculating every second until 2 the same
			if (!TimerResult)
				ReportErrorMessage("Error-Calculate Integration Timer Failed",cEMailInspx,32000);

		}
		else
		if (pMsg->lParam)
		{
			CString TempText("");
			TempText.LoadString(IDS_XRAY);
			ReportErrorMessage("IDC_Calibrating 4: " + TempText + " " + dtoa(pMsg->lParam + 13), cWriteToLog,0);
			SetDlgItemText(IDC_Calibrating, TempText + " " + dtoa(pMsg->lParam + 13, 0));
			m_Calibrating.ShowWindow(SW_SHOW);
		}
		else
		{
			switch (pMsg->wParam)
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
					ReportErrorMessage("IDC_Calibrating 5: " + TempText, cWriteToLog,0);
					SetDlgItemText(IDC_Calibrating,TempText);
					m_Calibrating.ShowWindow(SW_SHOW);
				}
				break;
				case 25:
				{
					CString TempText("");
					if (vLocalSystemData->vXRayOnNotice)
					{
						TempText.LoadString(IDS_XRAYON);
						//if (vMode == cSizeMenu)
						//{
						//	if (vNeedToAutoSizeNextContainer)
						//	{
						//		if (vIntegrationTimeSet)
						//			SetStatusLine("Send one container to AutoSize.", 0, false);
						//		else
						//		{
						//			SetStatusLine("Reading conveyor speed.\nPlease Wait.", 0, false);
						//			vSamplingConveyorSpeed = true;
						//		}
						//	}
						//}
						TempText.LoadString(IDS_Wait);
					}
					else
					{
						TempText.LoadString(IDS_XRAY);
						TempText = TempText + " **";
					}
					ReportErrorMessage("IDC_Calibrating 6: " + TempText, cWriteToLog,0);
					SetDlgItemText(IDC_Calibrating,TempText);
					m_Calibrating.ShowWindow(SW_SHOW);
				}
				break;
				case 26:
				{
					CString TempText("");
					if (vLocalSystemData->vXRayOnNotice)
					{
						TempText.LoadString(IDS_XRAYON);
						if (vMode == cSizeMenu)
						{
							if (vNeedToAutoSizeNextContainer)
							{
								if (vIntegrationTimeSet)
									SetStatusLine("Send one container to AutoSize.", 0, false);
								else
								{
									SetStatusLine("Reading conveyor speed.\nPlease wait", 0, false);
									vSamplingConveyorSpeed = true;
								}
							}
						}
					}
					else
					{
						TempText.LoadString(IDS_XRAY);
						TempText = TempText + " **";
					}
					ReportErrorMessage("IDC_Calibrating 7: " + TempText, cWriteToLog,0);
					SetDlgItemText(IDC_Calibrating,TempText);
					m_Calibrating.ShowWindow(SW_SHOW);
				}
				break;
			}
		}
		return true;
	}
	else
	*/
	if (pMsg->message == cNewImageReadyToDisplay)
	{ //new image
		vXOffset = 0;
		vYOffset = 0;
		vXScale = 1;
		vYScale = 1;

		ReportErrorMessage("Got Image in Express Setup, Frame:" + dtoa(vLocalSystemData->vImageNumberToDisplay, 0) + ", Old: " + dtoa(vOldImageNumberToDisplay, 0), cWriteToLog, 0);

		CString TempText1;
		CString TempText;
		if (vLocalSystemData->vImageNumberToDisplay != vOldImageNumberToDisplay)
		if ((vLocalSystemData->vITIPCDig->vOriginalImage) && (vLocalSystemData->vITIPCDig->vOriginalImage->vGreyImage))
		{
			vOldImageNumberToDisplay = vLocalSystemData->vImageNumberToDisplay;

			if (vLocalSystemData->vLogFile.vLogSerialData)
				vLocalSystemData->vLogFile.WriteToLogFile("New Image, Frame: " + dtoa(vAdjustBrightnessFrameCount, 0),cDebugMessage);

			//new image newframe
			if (vLocalSystemData->vITIPCDig->vNextImageToDisplayLock.Lock())
			{
				vLocalSystemData->vITIPCDig->vOriginalImage->CopyImage(vLocalSystemData->vITIPCDig->vNextImageToDisplay, vLocalSystemData->vITIPCDig->vOriginalBufferSize);
				vLocalSystemData->vITIPCDig->vNextImageToDisplayLock.Unlock();
			}

			if (!vHaveAnImage)
			if (vAdjustedExposureForAutoSize > 0)
			{ //new image
				vHaveAnImage = true;
 
				m_Background.ShowWindow(SW_HIDE);
			}

			if (vMode == cEvaluateMenu)
			{
				//Save Images to Evaluate
			}
			else
			if (vMode == cExposureMenu)
			{
				ReportErrorMessage("Got Image in Express Exposure, Voltage:" + dtoa(vGlobalCurrentProduct->vXRaySourceVoltage * 6.0, 1) + ", Current: " + dtoa(vGlobalCurrentProduct->vXRaySourceCurrent, 2), cWriteToLog, 0);
				AutoAdjustExposure();

				if (vExposureIsCorrect)
				{
					m_SubFunction8Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
					m_SubFunction8Button.ShowWindow(SW_SHOW);
					TempText.LoadString(IDS_Proceed);
					SetDlgItemText(IDC_SubFunction8Button,TempText);
				}
			}
			else
			if (vMode == cSizeMenu) //new image
			{
				ReportErrorMessage("Got Image in Size Menu, Frame:" + dtoa(vAdjustBrightnessFrameCount, 0) + ", Current: " + dtoa(vGlobalCurrentProduct->vXRaySourceCurrent, 2), cWriteToLog, 0);
				
				if (!vLocalSystemData->vXRayOnNotice)
					ReportErrorMessage("X-RAYS NOT ON", cWriteToLog, 0);
				if (vStartingUpSource)
					ReportErrorMessage("vStartingUpSource", cWriteToLog, 0);

				if ((vLocalSystemData->vXRayOnNotice) && (!vStartingUpSource))
				if ((vAdjustingImageBrightnessBelowSaturation) || (vAdjustingImageBrightnessJustAboveSaturation))
				{
					vAdjustBrightnessFrameCount++;
					ReportErrorMessage("Adjusting Brightness to size, Frame:" + dtoa(vAdjustBrightnessFrameCount, 0) + ", Current: " + dtoa(vGlobalCurrentProduct->vXRaySourceCurrent, 2), cWriteToLog, 0);
					if (vGlobalImageAquisitionThread)
					{
						vGlobalImageAquisitionThread->WriteImageToHardDisk(vLocalSystemData->vITIPCDig->vOriginalImage->vGreyImage, 
							vLocalConfigurationData->vScanTracCalibrationImagesDirectory +"AdjustBrightnessImage" + dtoa(vAdjustBrightnessFrameCount, 0) + ".bmp", 
							vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY);
					}

					//calculate image brightness
					//CopyMemory(vLocalSystemData->vITIPCDig->vASIDDScratchImage2->pix, vLocalSystemData->vITIPCDig->vOriginalImage->vGreyImage, vLocalSystemData->vITIPCDig->vOriginalBufferSize);
					////sort pixels in image by their brightness
					//vLocalSystemData->vITIPCDig->SortImage(vLocalSystemData->vITIPCDig->vASIDDScratchImage2->pix, vLocalSystemData->vITIPCDig->vOriginalBufferSize);

					//DWORD Temp5PercentSize = (DWORD)(vLocalSystemData->vITIPCDig->vOriginalBufferSize * 0.05);
					//DWORD Temp10PercentSize = (DWORD)(vLocalSystemData->vITIPCDig->vOriginalBufferSize * 0.1);
					////average the pixels between 5% of brightest to 15% of brightest
					//BYTE *TempPointer = vLocalSystemData->vITIPCDig->vASIDDScratchImage2->pix + Temp5PercentSize;
					//DWORD TempSum = 0;
					//for (DWORD TempLoop = 0; TempLoop < Temp10PercentSize; TempLoop++)
					//	TempSum = TempSum + *TempPointer++;

					//double TempAverageImageBrightness = TempSum / Temp10PercentSize;

					CopyMemory(vLocalSystemData->vITIPCDig->vASIDDScratchImage2->pix, vLocalSystemData->vITIPCDig->vOriginalImage->vGreyImage, vLocalSystemData->vITIPCDig->vOriginalBufferSize);
					//make first line of pixels brightest pixel for each line
					for (WORD TempLoopX = 0; TempLoopX < vLocalSystemData->vITIPCDig->vOriginalBufferSizeX; TempLoopX++)
					{
						BYTE *TempFirstPixelPointer = vLocalSystemData->vITIPCDig->vASIDDScratchImage2->pix + TempLoopX;
						BYTE *TempPointer = TempFirstPixelPointer + vLocalSystemData->vITIPCDig->vOriginalBufferSizeX;
						for (WORD TempLoopY = 0; TempLoopY < vLocalSystemData->vITIPCDig->vOriginalBufferSizeY - 1; TempLoopY++)
						{
							if (*TempPointer > *TempFirstPixelPointer)
								*TempFirstPixelPointer = *TempPointer;

							TempPointer = TempPointer + vLocalSystemData->vITIPCDig->vOriginalBufferSizeX;
						}
					}

					//sort the brightest value for each pixel
					for (DWORD TempMainLoop = 0; TempMainLoop < vLocalSystemData->vITIPCDig->vOriginalBufferSizeX; TempMainLoop++)
					{
						bool TempMadeASwap = false;
						BYTE *TempPointer = vLocalSystemData->vITIPCDig->vASIDDScratchImage2->pix;
						BYTE *TempPointerToNext = TempPointer + 1;
						for (WORD TempSortLoop = 0; TempSortLoop < vLocalSystemData->vITIPCDig->vOriginalBufferSizeX - 1; TempSortLoop++)
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
							TempMainLoop = vLocalSystemData->vITIPCDig->vOriginalBufferSizeX;
					}

					bool TempAllSaturated = true;
					DWORD Temp5PercentSize = (DWORD)(vLocalSystemData->vITIPCDig->vOriginalBufferSizeX * 0.05);
					if (vGlobalScanTracType == cAllegroScanTrac) //ignore bottom 3 detectors as they have black pixels when saturated too much
						 Temp5PercentSize = (DWORD)(vLocalSystemData->vITIPCDig->vOriginalBufferSizeX * (0.05 + (3/9) ));

					DWORD Temp95PercentSize = (DWORD)(vLocalSystemData->vITIPCDig->vOriginalBufferSize * 0.95);
					WORD TempNumber = (WORD)(Temp95PercentSize - Temp5PercentSize);
					////average the pixels between 5% of brightest to 95% of brightest
					BYTE *TempPointer = vLocalSystemData->vITIPCDig->vASIDDScratchImage2->pix + Temp5PercentSize;
					DWORD TempSum = 0;
					for (DWORD TempLoop = 0; TempLoop < TempNumber; TempLoop++)
					{
						if (*TempPointer < 255)
							TempAllSaturated = false;
						TempSum = TempSum + *TempPointer++;
					}

					double TempAverageImageBrightness = (double)TempSum / (double)TempNumber;

					ReportErrorMessage("Adjusting Brightness to size, Average Brightness:" + dtoa(TempAverageImageBrightness, 4), cWriteToLog, 0);

					if (vAdjustingImageBrightnessBelowSaturation) // Stage 1
					{
						ReportErrorMessage("vAdjustingImageBrightnessBelowSaturation", cWriteToLog, 0);
						if (TempAverageImageBrightness >= 252) //image is still saturated
						{ //reduce the current by half and take another image
							if ((vLocalConfigurationData->vScanTracType == cSoloScanTrac) || (vGlobalScanTracType == cSoloPlusScanTrac))
							{
								vLocalSystemData->vRampXRayCurrentAmount = vLocalSystemData->vRampXRayCurrentAmount / 2.0;
								ReportErrorMessage("Brightness >= 255, Divide current by 2", cWriteToLog, 0); 
							}
							else
							if (vLocalSystemData->vRampXRayVoltageAmount > (20.0 / 6.0))
							{
								vLocalSystemData->vRampXRayVoltageAmount = vLocalSystemData->vRampXRayVoltageAmount - (5.0 / 6.0);
								ReportErrorMessage("Brightness >= 255, Reduce KV by 5", cWriteToLog, 0); 
							}

							vLocalConfigurationData->LimitToMaximumCurrent(vLocalSystemData->vRampXRayVoltageAmount, vLocalSystemData->vRampXRayCurrentAmount);
							vGlobalCurrentProduct->vXRaySourceCurrent = (float)vLocalSystemData->vRampXRayCurrentAmount;
							vMainWindowPointer->SendSourceSettingsToUController(vLocalSystemData->vRampXRayVoltageAmount, vLocalSystemData->vRampXRayCurrentAmount);	
							::PostThreadMessage(vGlobalCallingThreadID, cSimulateOneContainerIn10MSMessage, 1000, 0); //simulate next image in 1 second

							if (vLocalSystemData->vLogFile.vLogSerialData)
								vLocalSystemData->vLogFile.WriteToLogFile("TDAutoSetup product, Express Setup, Simulate a container to adjust exposure below saturation",cDebugMessage);

							if (vGlobalCurrentProduct->vXRaySourceCurrent <= 0)
							{
								CNoticeDialog TempNoticeDialog;
								TempNoticeDialog.vNoticeText = "\n\nCurrent at Zero and picture still too bright.\nCall Service";
								TempNoticeDialog.vType = cErrorMessage;
								TempNoticeDialog.DoModal();
							}
						}
						else
						{ //image is not saturated, so now make it just above saturation
							vAdjustingImageBrightnessBelowSaturation = false;
							if ((vGlobalScanTracType == cSoloScanTrac) || (vGlobalScanTracType == cSoloPlusScanTrac)) //on solos, bring up brightness above saturation
							{
								ReportErrorMessage("Express Setup Start Background Brighness Above Adjust, Voltage:" + dtoa(vGlobalCurrentProduct->vXRaySourceVoltage * 6.0, 1) + ", Current: " + 
									dtoa(vGlobalCurrentProduct->vXRaySourceCurrent, 2), cWriteToLog, 0);
								vAdjustingImageBrightnessJustAboveSaturation = true;
								ReportErrorMessage("Brightness is below Saturation, so bring up", cWriteToLog, 0); 
								double TempCurrentCorrectionFactor = 255 * 1.1 / TempAverageImageBrightness;
								if (TempCurrentCorrectionFactor > 1.2)
									TempCurrentCorrectionFactor = 1.2;

								vLocalSystemData->vRampXRayCurrentAmount = (float)(vLocalSystemData->vRampXRayCurrentAmount * TempCurrentCorrectionFactor);
								vLocalConfigurationData->LimitToMaximumCurrent(vLocalSystemData->vRampXRayVoltageAmount, vLocalSystemData->vRampXRayCurrentAmount);
								//ReportErrorMessage("Status Line--Adjusting Brightness to size, Stage 2=" + dtoa(vLocalSystemData->vRampXRayCurrentAmount, 2), cWriteToLog, 0);
								vGlobalCurrentProduct->vXRaySourceCurrent = (float)vLocalSystemData->vRampXRayCurrentAmount;
								vMainWindowPointer->SendSourceSettingsToUController(vLocalSystemData->vRampXRayVoltageAmount, vLocalSystemData->vRampXRayCurrentAmount);	
								::PostThreadMessage(vGlobalCallingThreadID, cSimulateOneContainerIn10MSMessage, 1000, 0); //simulate next image in 1 second

								if (vLocalSystemData->vLogFile.vLogSerialData)
									vLocalSystemData->vLogFile.WriteToLogFile("TDAutoSetup product, Express Setup, Simulate a container to get brightness to saturation",cDebugMessage);
							}
							else
							{
								ReportErrorMessage("Brightness is below Saturation, so good for sizing, start Image Acquisition", cWriteToLog, 0);
								//on TD models, want exposure just under saturated to autosize so can handle bags of chips and salad
								vAdjustedExposureForAutoSize = 1;
								vNeedToAutoSizeNextContainer = true;

								vGlobaluCSerialPort->SetRadiationDoorManualControl(cRadiationShutterAutoControl);
								vMainWindowPointer->StartImageAcquisition();
								StartSizeMenu();
							}
						}
					}
					else
					if (vAdjustingImageBrightnessJustAboveSaturation) //Stage 2
					{
						ReportErrorMessage("vAdjustingImageBrightnessJustAboveSaturation", cWriteToLog, 0);
						if (TempAverageImageBrightness >= 254) //image is now saturated, so can continue to autosize
						{

							if (vLocalConfigurationData->LimitToMaximumCurrent(vLocalSystemData->vRampXRayVoltageAmount, vLocalSystemData->vRampXRayCurrentAmount))
							{
								if (vGaveTooDarkWarning < 200)
									vGaveTooDarkWarning++;
								if (vGaveTooDarkWarning == 10)
								{
									CNoticeDialog TempNoticeDialog;
									TempNoticeDialog.vNoticeText = "\n\nCurrent at Maximum and picture still too dark.\nCall Service if no containers are in the tunnel.";
									TempNoticeDialog.vType = cErrorMessage;
									TempNoticeDialog.DoModal();
								}
							}

							double TempCurrentCorrectionFactor = 1.2;
							ReportErrorMessage("Adjusting Brightness gives 255, increase by: " + dtoa(TempCurrentCorrectionFactor, 2), cWriteToLog, 0);
							KillTimer(vSimulateContainerAfterXRaysAreOnTimerHandle);

							vLocalSystemData->vRampXRayCurrentAmount = (float)(vLocalSystemData->vRampXRayCurrentAmount * TempCurrentCorrectionFactor);
							vLocalConfigurationData->LimitToMaximumCurrent(vLocalSystemData->vRampXRayVoltageAmount, vLocalSystemData->vRampXRayCurrentAmount);

							vGlobalCurrentProduct->vXRaySourceCurrent = (float)vLocalSystemData->vRampXRayCurrentAmount;
							vMainWindowPointer->SendSourceSettingsToUController(vLocalSystemData->vRampXRayVoltageAmount, vLocalSystemData->vRampXRayCurrentAmount);	

							vAdjustingImageBrightnessJustAboveSaturation = false;
							ReportErrorMessage("Adjusting Brightness to size, Complete.  Current: " + dtoa(vGlobalCurrentProduct->vXRaySourceCurrent, 2), cWriteToLog, 0);
							if (vLocalSystemData->vLogFile.vLogSerialData)
								vLocalSystemData->vLogFile.WriteToLogFile("Adjusting Brightness to size, Complete.  Frame: " + dtoa(vAdjustBrightnessFrameCount, 0),cDebugMessage);
							vAdjustedExposureForAutoSize = 1;
							vHaveAnImage = false;

							vGlobaluCSerialPort->SetRadiationDoorManualControl(cRadiationShutterAutoControl);
							vMainWindowPointer->StartImageAcquisition();
							SetStatusLine("Send one container to AutoSize.", 0, false);
						}
						else
						{
							if (vLocalConfigurationData->LimitToMaximumCurrent(vLocalSystemData->vRampXRayVoltageAmount, vLocalSystemData->vRampXRayCurrentAmount))
							{
								if (vGaveTooDarkWarning < 200)
									vGaveTooDarkWarning++;
								if (vGaveTooDarkWarning == 10)
								{
									CNoticeDialog TempNoticeDialog;
									TempNoticeDialog.vNoticeText = "\n\nCurrent at Maximum and picture still too dark.\nCall Service if no containers are in the tunnel.";
									TempNoticeDialog.vType = cErrorMessage;
									TempNoticeDialog.DoModal();
								}
							}

							double TempCurrentCorrectionFactor = 255 * 1.3 / TempAverageImageBrightness;
							ReportErrorMessage("Adjusting Brightness to size, Original Current Correction Factor: " + dtoa(TempCurrentCorrectionFactor, 2), cWriteToLog, 0);
							if (TempCurrentCorrectionFactor > 1.9)
								TempCurrentCorrectionFactor = 1.9;

							vLocalSystemData->vRampXRayCurrentAmount = (float)(vLocalSystemData->vRampXRayCurrentAmount * TempCurrentCorrectionFactor);
							vLocalConfigurationData->LimitToMaximumCurrent(vLocalSystemData->vRampXRayVoltageAmount, vLocalSystemData->vRampXRayCurrentAmount);

							vGlobalCurrentProduct->vXRaySourceCurrent = (float)vLocalSystemData->vRampXRayCurrentAmount;
							vMainWindowPointer->SendSourceSettingsToUController(vLocalSystemData->vRampXRayVoltageAmount, vLocalSystemData->vRampXRayCurrentAmount);	
							::PostThreadMessage(vGlobalCallingThreadID, cSimulateOneContainerIn10MSMessage, 1000, 0); //simulate next image in 1 second

							if (PasswordOK(cSuperInspxPassword,false))
							{
								SetDlgItemText(IDC_SubFunction7Display, "V: " + dtoa(vGlobalCurrentProduct->vXRaySourceVoltage * 6.0, 1) + ", C: " + dtoa(vGlobalCurrentProduct->vXRaySourceCurrent, 2));
								m_SubFunction7Display.ShowWindow(SW_SHOW);
							}
							if (vLocalSystemData->vLogFile.vLogSerialData)
								vLocalSystemData->vLogFile.WriteToLogFile("TDAutoSetup product, Express Setup, Simulate a container to adjust exposure to saturation",cDebugMessage);
						}
					}
				}
				else
				if (vIntegrationTimeSet)
				{
					//9/30/2013
					if (!vNeedToAutoSizeNextContainer)
					{
						//autosized container, so tell customer to press Next
						//int TempPosition = vCurrentStatusLineText.Find("Press NEXT", 0);
						//if (TempPosition == -1) //second  or more container after sizing
						int TempPosition = vCurrentStatusLineText.Find(_T("Image was sized"), 0);
						if (TempPosition == -1) //second  or more container after sizing
						//if (vCurrentStatusLineText != "Press Next.  Image was sized.");
						{
							SetStatusLine("Press Next.  Image was sized.", 2000, true);
							SetDlgItemText(IDC_SubFunction5Button, _T("Redo Size"));
							m_SubFunction5Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
							m_SubFunction5Button.ShowWindow(SW_SHOW);

							m_SubFunction8Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
							m_SubFunction8Button.ShowWindow(SW_SHOW);
							TempText.LoadString(IDS_Proceed);
							SetDlgItemText(IDC_SubFunction8Button,TempText);
						}
						//else
						//{ //first container after sizing
						//	SetStatusLine("Send another container\nto see new size", 2000, true);
						//}
					}

					//autosize image now
					if (vLocalConfigurationData->vSimulateLightControlBoard)
						vLocalSystemData->vITIPCDig->vOriginalImage->vBodyTriggerGapLength = (WORD)(vGlobalPixelsPerUnit * 100);

					WORD TempTop = 0;
					WORD TempBottom = 0;
					WORD TempHeight = 0;
					WORD TempWidth = 0;
					bool TempFindContainerEdgesRelativeToImageEdges = ((vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac) || 
						(vGlobalScanTracType == cAllegroScanTrac) ||(vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac));

					//ReportErrorMessage("TDAS-pre-Found Edges Left: " + dtoa(vLocalSystemData->vITIPCDig->vOriginalImage->vLeft, 2) + ", Right: " + dtoa(vLocalSystemData->vITIPCDig->vOriginalImage->vRight, 2), cWriteToLog, 0);
					//if (TempFindContainerEdgesRelativeToImageEdges)
					//	ReportErrorMessage("AutoSize find container edges Relative to image edges", cWriteToLog, 0);
					//else
					//	ReportErrorMessage("AutoSize find container edges normal method", cWriteToLog, 0);

					vLocalSystemData->vITIPCDig->FindContainerDimensions(vLocalSystemData->vITIPCDig->vOriginalImage->vGreyImage, vLocalSystemData->vITIPCDig->vNIDDScratchImage, &vLocalSystemData->vITIPCDig->vOriginalImage->vTop, 
						&vLocalSystemData->vITIPCDig->vOriginalImage->vBottom, &vLocalSystemData->vITIPCDig->vOriginalImage->vRight, &vLocalSystemData->vITIPCDig->vOriginalImage->vLeft, 
						&vLocalSystemData->vITIPCDig->vOriginalImage->vHeight, &vLocalSystemData->vITIPCDig->vOriginalImage->vWidth, TempFindContainerEdgesRelativeToImageEdges, 10000); //vLocalConfigurationData->vShapeHandlingWidthTolerancePixels); //10000 = Finding new reference so ignore old one

					//ReportErrorMessage("TDAS-Found Edges Left: " + dtoa(vLocalSystemData->vITIPCDig->vOriginalImage->vLeft, 2) + ", Right: " + dtoa(vLocalSystemData->vITIPCDig->vOriginalImage->vRight, 2), cWriteToLog, 0);

					if (!vLocalSystemData->vITIPCDig->vOriginalImage->vFinalImageIntensity)
					{
						vLocalSystemData->vITIPCDig->vOriginalImage->vFinalImageIntensity = vLocalSystemData->vITIPCDig->CalculateFullDensity(
							vLocalSystemData->vITIPCDig->vOriginalImage->vGreyImage, vLocalSystemData->vITIPCDig->vOriginalBufferSize); 
					}

					if (vAdjustedExposureForAutoSize > 1)
					if (vHaveAnImage)
					{
						vGotImageToAutoSize = true;
						ReportErrorMessage("Got Image to AutoSize", cWriteToLog, 0);
						if (vLocalSystemData->vLogFile.vLogSerialData)
							vLocalSystemData->vLogFile.WriteToLogFile("Got Image to AutoSize: " + dtoa(vAdjustBrightnessFrameCount, 0), cDebugMessage);

						double TempContainerWidth = (float)(((vLocalSystemData->vITIPCDig->vOriginalImage->vRight - vLocalSystemData->vITIPCDig->vOriginalImage->vLeft) 
							/ (vGlobalPixelsPerUnit * vGlobalCurrentProduct->vOverScanMultiplier)));

						if (PasswordOK(cTemporaryInspxPassword,false))
						{
							SetDlgItemText(IDC_SubFunction7Display, "V: " + dtoa(vGlobalCurrentProduct->vXRaySourceVoltage * 6.0, 1) + ", C: " + dtoa(vGlobalCurrentProduct->vXRaySourceCurrent, 2));
							m_SubFunction7Display.ShowWindow(SW_SHOW);
							m_SubFunction4Display.ShowWindow(SW_SHOW);
							m_SubFunction8Display.ShowWindow(SW_SHOW);
						}
						SetDlgItemText(IDC_SubFunction8Display, "CW: " + dtoa(TempContainerWidth, 1));
					}
				}
				else
					ReportErrorMessage("Got Image in size, but ignore as still to set Exposure Time", cWriteToLog, 0);
			}
			
			if (vAdjustedExposureForAutoSize > 1)
			{
				//ReportErrorMessage("vAdjustedExposureForAutoSize > 1", cWriteToLog, 0);
				DrawImageOnScreen();
			}
			else
			if (vAdjustedExposureForAutoSize == 1)
			//if (vHaveAnImage)
			{
				vAdjustedExposureForAutoSize = 2; //do after DrawImage so don't show the last white image of exposure adjusting
				if (vLocalSystemData->vLogFile.vLogSerialData)
					vLocalSystemData->vLogFile.WriteToLogFile("vAdjustedExposureForAutoSize 1 -> 2  Frame: " + dtoa(vAdjustBrightnessFrameCount, 0),cDebugMessage);
			}
			//else
			//	ReportErrorMessage("vAdjustedExposureForAutoSize < 1", cWriteToLog, 0);

			CString TempStringName(cSpace);
			TempStringName = TempStringName + vProductEditingName;

		}
		return true;  //true indicates it has been handled, so do not process
	}
	else
	if (pMsg->message == WM_KEYDOWN)
	{
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
				if (vMode == cSizeMenu)
					PostThreadMessage(vGlobalCallingThreadID, cShowHelp, 33, 0);
				else
				if (vMode == cExposureMenu)
					PostThreadMessage(vGlobalCallingThreadID, cShowHelp, 34, 0);
				else
				if (vMode == cEvaluateMenu)
					PostThreadMessage(vGlobalCallingThreadID, cShowHelp, 36, 0);
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
				TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText1);
				if (TempWindow)
					TempWindow->Invalidate(false);
			break;
			case 27: // Escape
				::PostThreadMessage(vGlobalCallingThreadID,	cStartChatWindowMessage, 0,0);
			break;	
			case 0x0d: // F2:
				OnFunction2Button();
			break;
		}
		return true;  //true indicates it has been handled, so do not process
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CTDAutoSetupImageDisplayDialog::DrawImageOnScreen()
{
	if (vAdjustedExposureForAutoSize > 1)
	if ((vLocalSystemData->vITIPCDig->vProcessManipulationImage) && (vLocalSystemData->vITIPCDig->vProcessManipulationImage->vGreyImage))
	if ((vLocalSystemData->vITIPCDig->vOriginalImage) && (vLocalSystemData->vITIPCDig->vOriginalImage->vGreyImage))
	{
		if (vHaveAnImage)
		{  //show acquired image
			vLocalSystemData->vITIPCDig->ScaleImageSideways(vLocalSystemData->vITIPCDig->vDisplayImageSizeX,
				vLocalSystemData->vITIPCDig->vDisplayImageSizeY, vYScale, vXScale, vXOffset, vYOffset, 
				vLocalSystemData->vITIPCDig->vOriginalImage->vGreyImage, vLocalConfigurationData->vPixelsPerDetector, (vMode == cSizeMenu));

			if (vNeedToAutoSizeNextContainer)
			{
				BYTE TempShowReferenceAdjustedImage = false;
				//if (vMode == cSetSizeSettings)
				//	TempShowReferenceAdjustedImage = true;
				vLocalSystemData->vITIPCDig->AddLinesToImageSideways(  //draws found edges of container lines
					vLocalSystemData->vITIPCDig->vDisplayImageSizeX,
					vLocalSystemData->vITIPCDig->vDisplayImageSizeY, 
					1, 0, 0,TempShowReferenceAdjustedImage,
					vLocalSystemData->vITIPCDig->vOriginalImage, NULL, vLocalConfigurationData->vScanTracType);
			}

			WORD TempOffsetX = (WORD) ResizeXCoor(108);
			BYTE TempOffsetY = (BYTE) ResizeXCoor(43);
			RECT TempRectangle;
			TempRectangle.left = TempOffsetX;
			TempRectangle.top = TempOffsetY;
			TempRectangle.right = TempOffsetX + vLocalSystemData->vITIPCDig->vDisplayImageSizeX + 8;
			TempRectangle.bottom = TempOffsetY + vLocalSystemData->vITIPCDig->vDisplayImageSizeY;
			// could set a timer to invalidate in 200 milliseconds
			this->InvalidateRect(&TempRectangle,false);
		}
		vXScale = 1;
		vYScale = 1;
	}
}

void CTDAutoSetupImageDisplayDialog::OneSecondUpdate()
{
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

		if (vLocalSystemData->vLogFile.vLogSerialData)
			vLocalSystemData->vLogFile.WriteToLogFile("Autosetup X-Ray Step: " + dtoa(vOldXRayState, 0),cDebugMessage);

		if ((vOldXRayState == 26) || (vOldXRayState == 25))
		{
			vStartingUpSource = false;
			CString TempText("");
			TempText.LoadString(IDS_XRAYON);
			ReportErrorMessage("IDC_Calibrating 3: " + TempText, cWriteToLog,0);
			SetDlgItemText(IDC_Calibrating, TempText);
			m_Calibrating.ShowWindow(SW_SHOW);

			ReportErrorMessage("X-Rays On State:" + dtoa(vOldXRayState, 0) + ", OnNotice: " + dtoa(vLocalSystemData->vXRayOnNotice, 0) + ", vStartingUpSource: " + dtoa(vStartingUpSource, 0), cWriteToLog, 0);

		
			if (vMode == cSizeMenu)
			{
				if (vNeedToAutoSizeNextContainer)
				if (vLocalSystemData->vXRayOnNotice)
				{
					if (vIntegrationTimeSet)
					{
						vMakeStatusLineGreen = " ";
						SetStatusLine(" ", 0, false);

						SetStatusLine("Send one container to AutoSize.", 0, false); //this leaves a dot on the line below the status line???? why
					}
					else
					{
						SetStatusLine("Reading conveyor speed.\nPlease wait...", 0, false);
						vSamplingConveyorSpeed = true;
					}
				}
			}
			else
			if (vMode == cExposureMenu)
				SetStatusLine("Send one container\n to verify exposure.", 2000, false);
			else
				SetStatusLine("Send one container.", 2000, false);

			if (!vIntegrationTimeSet)
			{
				int TimerResult = SetTimer(vCalculateIntegrationTimerHandle,3000,NULL);  //wait 1 second for conveyor to get moving then calculate best integration time will keep calculating every second until 2 the same
				if (!TimerResult)
					ReportErrorMessage("Error-Calculate Integration Timer Failed",cEMailInspx,32000);
			}
		}
		else
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
			{
				CString TempText("");
				if (vLocalSystemData->vXRayOnNotice)
				{
					TempText.LoadString(IDS_XRAYON);
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
			default:
				CString TempText("");
				TempText.LoadString(IDS_XRAY);
				SetDlgItemText(IDC_Calibrating,TempText + " " + dtoa(vOldXRayState + 13, 0));
				m_Calibrating.ShowWindow(SW_SHOW);
			break;
		}
	}

	if (vMode == cTestEjectorsMenu)
	{
		if (vOldTestEjectNextContainer != vLocalSystemData->vTestEjectNextContainer)
		{
			vOldTestEjectNextContainer = vLocalSystemData->vTestEjectNextContainer;
			UpdateButtons();
		}
	}

	if (vSamplingConveyorSpeed)
	{
		vAdjustBrightnessDotCount++;
		BYTE TempNumberOfDots = (vAdjustBrightnessDotCount % 6);
		CString TempDots = ".";
		for (BYTE TempLoop = 0; TempLoop < TempNumberOfDots; TempLoop++)
			TempDots = TempDots + ".";
		SetStatusLine("Reading conveyor speed.\nPlease wait.." + TempDots, 0, false);
	}
	if ((vAdjustingImageBrightnessBelowSaturation) || (vAdjustingImageBrightnessJustAboveSaturation))
	{
		vAdjustBrightnessDotCount++;
		BYTE TempNumberOfDots = (vAdjustBrightnessDotCount % 6);
		CString TempDots = ".";
		for (BYTE TempLoop = 0; TempLoop < TempNumberOfDots; TempLoop++)
			TempDots = TempDots + ".";
		SetStatusLine("Adjusting brightness.\nPlease wait.." + TempDots, 0, false);
	}

	if (vLocalSystemData->vQuickLearnImageIndex != vOldQuickLearnImageIndex)
	{
		CString TempText = " ";
		vOldQuickLearnImageIndex = vLocalSystemData->vQuickLearnImageIndex;
		if (vLocalSystemData->vQuickLearnImageIndex > 0)
		{
			TempText = "Send another to evaluate thresholds.\n " + dtoa(vOldQuickLearnImageIndex,0) + " of " + dtoa(cNumberOfImagesToEvaluate, 0);
			SetStatusLine(TempText, 300, false);
		}
		else
		if (vLocalSystemData->vCompletedEvaluation)
		{
			vEvaluationComplete = true;
			TempText = "Press Next. Evaluation complete.";
			SetStatusLine(TempText, 300, true);
			m_SubFunction8Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
			m_SubFunction8Button.ShowWindow(SW_SHOW);
			TempText.LoadString(IDS_Next);
			SetDlgItemText(IDC_SubFunction8Button,TempText);
			SetDlgItemText(IDC_SubFunction5Button, _T("Redo Evaluate"));
			m_SubFunction5Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
			m_SubFunction5Button.ShowWindow(SW_SHOW);
		}
		else
		{
			TempText = "Send " + dtoa(cNumberOfImagesToEvaluate, 0) + " containers\n to evaluate thresholds.";
			SetStatusLine(TempText, 300, false);
		}
		if ((vOldQuickLearnImageIndex >= 1) && (!m_SubFunction5Button.IsWindowVisible()))
		{
			SetDlgItemText(IDC_SubFunction5Button, _T("Redo Evaluate"));
			m_SubFunction5Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
			m_SubFunction5Button.ShowWindow(SW_SHOW);
		}
	}
			
	//if (vEvaluationComplete != vOldEvaluationComplete)
	//{
	//}

	if ((vGlobalProcessQuickLearnImagesThread) || (vGlobalProcessEvaluateImagesThread))
	{
		CString TempText = " ";
		if (vGlobalProcessEvaluateImagesThread)
			TempText = "Processing evaluation images.\n Please Wait...";
		else
			TempText = "Processing Quick Learn images.\n Please Wait...";

		vProcessingEvaluateImagesStep++;
		if (vProcessingEvaluateImagesStep > 3)
			vProcessingEvaluateImagesStep = 0;
		switch (vProcessingEvaluateImagesStep)
		{
			case 0: TempText = TempText + "|"; break;
			case 1: TempText = TempText + "/"; break;
			case 2: TempText = TempText + "-"; break;
			case 3: TempText = TempText + "\\"; break;
		}
		SetStatusLine(TempText, 0, false);
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

	if (vLocalSystemData->vYellowMessageButtonYellow != vOldButton2Yellow)
	{  //if just get an error message, display it and update buttons
		if (vLocalSystemData->vYellowMessageButtonYellow)
		{
			SetDlgItemText(IDC_StatusLine,vLocalSystemData->vLastErrorMessage);
			vOldErrorMessage = vLocalSystemData->vLastErrorMessage;
		}
		UpdateSubFunction4Button();
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

	if (vMode == cExposureMenu)
	{
		//get integration time in mSec
		if (vLocalSystemData->vMaximumEncoderRate)
		if (!vLocalConfigurationData->vContinuousFeedEncoderRate)
			vPercentBeltSpeed = vLocalSystemData->vActualEncoderRate / vLocalSystemData->vMaximumEncoderRate * 111.0;
		else
			vPercentBeltSpeed = vLocalSystemData->vActualEncoderRate / vLocalSystemData->vMaximumEncoderRate * 100.0;

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
				ReportErrorMessage("Express Setup failed to start X-Rays, so put in stopped mode", cWriteToLog, 0);
			}
		}
		//source went on or off so update button 3 to run
		UpdateSubFunction4Button();
	}
}

void CTDAutoSetupImageDisplayDialog::UpdateSubFunction4Button() 
{
	CString TempText("");
	if (vLocalSystemData->vYellowMessageButtonYellow)
	{
		m_SubFunction4Button.ShowWindow(SW_SHOW);
		TempText.LoadString(IDS_ClearYellowMessage);
		m_SubFunction4Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		SetDlgItemText(IDC_SubFunction4Button,TempText);
	}
	else
	{
		if ((!vStartingUpSource) && (!vRestartingXRaysWithNewSettings) && ((!vLocalSystemData->vXRayOnNotice) || (vLocalSystemData->vSystemRunMode == cStoppedSystemMode)))
		{
			m_SubFunction4Button.ShowWindow(SW_SHOW);
			TempText.LoadString(IDS_Run);
			SetDlgItemText(IDC_SubFunction4Button,TempText);
			m_SubFunction4Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
		}
		else
		{
			m_SubFunction4Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_SubFunction4Button.GetRectToInvalidateArrow());
		}
	}
}

void CTDAutoSetupImageDisplayDialog::ExitAutoSetupDialog(int TempExitCode) 
{
	KillTimer(vStartTimerHandle);
	KillTimer(vWaitUntilDoneRunningForReloadTimer);
	KillTimer(vCheckKeySwitchTimerHandle);
	vLocalSystemData->vTakingQuickLearnImages = 0;
	vLocalSystemData->vCompletedEvaluation = false;
	vLocalSystemData->vQuickLearnImageIndex = 0;

	vLocalSystemData->vEvaluateAndOrQuickLearn = cQuickLearnOnly;

	bool TempOKToExit = true;
	bool TempWarn = true;
	if (TempExitCode == 10)
		TempWarn = false;

	if (vMode == cSizeMenu)
		TempOKToExit = FinishSizeMenu(TempWarn, true);
	else
	if (vMode == cExposureMenu)
		TempOKToExit = FinishExposureMenu(); //was TempWarn for go back if not finding edges
	else
	if (vMode == cEvaluateMenu)
		FinishEvaluateMenu();

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

		vLocalSystemData->vAdjustingSourceSettings = false;
		CString TempText;
		TempText.LoadString(IDS_PleaseWait);
		SetStatusLine(TempText, 0, false);

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

		if ((TempExitCode == 10) || (TempExitCode == 0))
		{
			vLocalSystemData->vSetupANewProduct = false;

			ReportErrorMessage("Abort Auto Setup TD Product: " + *vGlobalCurrentProduct->GetProductName(), cUserAction,0);
		}
		if ((TempExitCode == 1) || (TempExitCode == 20))
		{
			ReportErrorMessage("Save and Exit Auto Setup TD Product: " + *vGlobalCurrentProduct->GetProductName(), cUserAction,0);
		}

		vLocalSystemData->vStartRunningAfterLoadProduct = false;

		if (TempExitCode == 1)
		{
			CYesNoDialog TempYesNoDialog;
			TempYesNoDialog.vNoticeText = _T("\n\n\nStart Inspecting or Finish");
			TempYesNoDialog.vQuestionType = cConfirmAbandon;
			TempYesNoDialog.vYesButtonText = "Start Inspecting";
			TempYesNoDialog.vNoButtonText = "Finish";
			int TempResult = TempYesNoDialog.DoModal();
			if (TempResult == IDOK)
				vLocalSystemData->vStartRunningAfterLoadProduct = true;

			vMainWindowPointer->StopRunning(true);
		}
		else
			vMainWindowPointer->StopRunning(true);

		if (vLocalSystemData->vTotalContainers > 0)
		{
			vMainWindowPointer->ResetCounters();
		}

		int TimerResult = SetTimer(vWaitUntilDoneRunningForExitTimer,100,NULL);
		if (!TimerResult)
			ReportErrorMessage("Error-Done Running Timer Failed",cEMailInspx,32000);
	}
}

void CTDAutoSetupImageDisplayDialog::ChangedProductSettingsSoReload()
{
	//Stop running, setup new product, start running
	if (vLocalSystemData->vInAutoSetup)
	{
		KillTimer(vCheckKeySwitchTimerHandle);
		CString TempText;
		TempText.LoadString(IDS_PleaseWait);
		SetStatusLine(TempText, 0, false);
		vStartingUpSource = true;
		vRestartingXRaysWithNewSettings = true;
		vMainWindowPointer->StopImageAcquisition();
		vMainWindowPointer->StopRunning(false);
		int TimerResult = SetTimer(vWaitUntilDoneRunningForReloadTimer,500,NULL);
		if (!TimerResult)
			ReportErrorMessage("Error-WaitForReload Timer Failed",cEMailInspx,32000);
	}
}

void CTDAutoSetupImageDisplayDialog::DoneRunningSoReload()
{
	KillTimer(vCheckKeySwitchTimerHandle);
	int TimerResult = SetTimer(vSetupTimerHandle,500,NULL);
	if (!TimerResult)
		ReportErrorMessage("Error-Setup Timer Failed",cEMailInspx,32000);
}

void CTDAutoSetupImageDisplayDialog::AutoSizeImage()
{
	Sleep(2000);
	CString TempText;
	//if (vMode == cSizeMenu)
	//if (vHaveAnImage)
	{//check to see if should adjust image size
		ReportErrorMessage("Start Express AutoSize image", cWriteToLog, 0);
		vMakeStatusLineYellowRed = false;
		bool TempImageGrew = false;

		if (vGlobalImageAquisitionThread)
		{
			vGlobalImageAquisitionThread->WriteImageToHardDisk(vLocalSystemData->vITIPCDig->vOriginalImage->vGreyImage, vLocalConfigurationData->vScanTracCalibrationImagesDirectory +"AutoSizeImage" + 
				dtoa(vSanityCheckFailureCount, 0) + ".bmp", vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY);
		}

		ReportErrorMessage("Start AutoSize Image Width: " + dtoa(vGlobalCurrentProduct->vProductImageWidth, 2), cWriteToLog, 0);

		float TempTriggerWidth = (float)(vLocalSystemData->vLastBodyTriggerLength / (vGlobalPixelsPerUnit * vGlobalCurrentProduct->vOverScanMultiplier));
		float TempContainerWidth = (float)(((vLocalSystemData->vITIPCDig->vOriginalImage->vRight - vLocalSystemData->vITIPCDig->vOriginalImage->vLeft) 
			/ (vGlobalPixelsPerUnit * vGlobalCurrentProduct->vOverScanMultiplier)));

		ReportErrorMessage("Easy New Setup Product: " + vLocalSystemData->vCurrentProductName, cWriteToLog, 0);
		ReportErrorMessage("Image Left: " + dtoa(vLocalSystemData->vITIPCDig->vOriginalImage->vLeft, 2) + ", Right: " + dtoa(vLocalSystemData->vITIPCDig->vOriginalImage->vRight, 2), cWriteToLog, 0);
		ReportErrorMessage("Image Left: " + dtoa(vLocalSystemData->vITIPCDig->vOriginalImage->vLeft / vGlobalPixelsPerUnit, 2) + ", Right: " + dtoa(vLocalSystemData->vITIPCDig->vOriginalImage->vRight / vGlobalPixelsPerUnit, 2), cWriteToLog, 0);
		ReportErrorMessage("Container Width From Image: " + dtoa(TempContainerWidth, 2), cWriteToLog, 0);
		ReportErrorMessage("Container Width From Trigger: " + dtoa(TempTriggerWidth, 2), cWriteToLog, 0);

		//Sanity check 1:the leading edge of the container must be at 2.0"+/-0.5" from the leading edge of the frame.  
		//If not, ask to repeat. If still not right, message: "Encoder calibration may be incorrect.  
		//Please have the encoder calibrated"  Abort.

		//Sanity check 2: the container width to be between 2" and 6".  If not, the first time ask to repeat, the second time - abort with message.
		bool TempSanityChecksOK = true;
		if ((vGlobalScanTracType == cSoloScanTrac) || (vGlobalScanTracType == cSoloPlusScanTrac))
		{
			double TempRightEdgeFound = vLocalSystemData->vITIPCDig->vOriginalImage->vRight / vGlobalPixelsPerUnit;

			if ((TempRightEdgeFound < 6) || (TempRightEdgeFound > 8)) //was 6.5 and 7.5
			{
				ReportErrorMessage("Autosize Sanity Check Failure.  Right Edge at: " + dtoa(TempRightEdgeFound, 2), cWriteToLog, 0);

				//if ((TempRightEdgeFound < 2.5) || (vSanityCheckFailureCount == 0))
				{
					TempSanityChecksOK = false;
					vSanityCheckFailureCount++;
					CNoticeDialog TempNoticeDialog;
					CString TempText = "Right Edge: " + dtoa(TempRightEdgeFound, 2) + ", should be 7 inches";
					if (vSanityCheckFailureCount == 1)
						TempNoticeDialog.vNoticeText = "\nLeading edge of container not in correct place.\nSend another container through\nmaking sure it does not slide on conveyor.\n" + TempText;
					else
						TempNoticeDialog.vNoticeText = "Leading edge of container not in correct place.\nSend another container through\nmaking sure it does not slide on conveyor, or\nabort and call service to check Encoder Calibration or use Advance New setup.\n" + TempText;
					TempNoticeDialog.vType = cErrorMessage;
					TempNoticeDialog.DoModal();
				}
			}
			if (TempSanityChecksOK)
			{
				if (TempContainerWidth > 6.75)
				{
					ReportErrorMessage("Autosize Sanity Check Failure.  Width: " + dtoa(TempContainerWidth, 2), cWriteToLog, 0);
					TempSanityChecksOK = false;
					vSanityCheckFailureCount++;
					CNoticeDialog TempNoticeDialog;
					if (vSanityCheckFailureCount == 1)
						TempNoticeDialog.vNoticeText = "\n\nContainer Width too wide: " + dtoa(TempContainerWidth, 0) + ".\nSend another container through\nmaking sure it does not slide on conveyor.";
					else
						TempNoticeDialog.vNoticeText = "\nContainer Width too wide: " + dtoa(TempContainerWidth, 0) + ".\nSend another container through\nmaking sure it does not slide on conveyor, or abort and call service to check Encoder Calibration or use Advance New setup.";
					TempNoticeDialog.vType = cErrorMessage;
					TempNoticeDialog.DoModal();
				}
			}
		}
		if (TempSanityChecksOK)
		if (TempContainerWidth < 1.5)
		{
			ReportErrorMessage("Autosize Sanity Check Failure.  Width: " + dtoa(TempContainerWidth, 2), cWriteToLog, 0);
			TempSanityChecksOK = false;
			vSanityCheckFailureCount++;
			CNoticeDialog TempNoticeDialog;
			if (vSanityCheckFailureCount == 1)
				TempNoticeDialog.vNoticeText = "\n\nContainer Width too narrow: " + dtoa(TempContainerWidth, 0) + ".\nSend another container through\nmaking sure it does not slide on conveyor.";
			else
				TempNoticeDialog.vNoticeText = "\nContainer Width too narrow: " + dtoa(TempContainerWidth, 0) + ".\nSend another container through\nmaking sure it does not slide on conveyor, or abort and call service to check Encoder Calibration or use Advance New setup.";
			TempNoticeDialog.vType = cErrorMessage;
			TempNoticeDialog.DoModal();
		}

		if (TempSanityChecksOK)
		{
			if (TempContainerWidth < TempTriggerWidth)
			{
				if ((vGlobalScanTracType == cSoloScanTrac) || (vGlobalScanTracType == cSoloPlusScanTrac))
				{
					if (7 > TempTriggerWidth)
						TempContainerWidth = TempTriggerWidth;
				}
				else
				{
					if (36 > TempTriggerWidth)
						TempContainerWidth = TempTriggerWidth;
				}
			}

			ReportErrorMessage("Container Width Using: " + dtoa(TempContainerWidth, 2), cWriteToLog, 0);

			bool TempFoundEdges = true;
			if ((TempContainerWidth < .50) || (TempContainerWidth > 46))
			{
				TempFoundEdges = false;
				CNoticeDialog TempNoticeDialog;
				TempNoticeDialog.vNoticeText = "\n\nScanTrac could not find the edges of the container.\nsend another container through to see if\nit can find both container edges.";
				TempNoticeDialog.vType = cErrorMessage;
				TempNoticeDialog.DoModal();
			}
			else
			if (vLocalSystemData->vITIPCDig->vOriginalImage->vLeft < 3)
			{
				TempFoundEdges = false;
				CNoticeDialog TempNoticeDialog;
				TempNoticeDialog.vNoticeText = "\n\nScanTrac did not find the left edge of the container.\nsend another container through to see if\nit can find both container edges.";
				TempNoticeDialog.vType = cErrorMessage;
				TempNoticeDialog.DoModal();
			}
			else
			if (vLocalSystemData->vITIPCDig->vOriginalImage->vRight >= vLocalSystemData->vITIPCDig->vOriginalBufferSizeY - 2)
			{
				TempFoundEdges = false;
				CNoticeDialog TempNoticeDialog;
				TempNoticeDialog.vNoticeText = "\n\nScanTrac did not find the Right edge of the container.\nsend another container through to see if\nit can find both container edges.";
				TempNoticeDialog.vType = cErrorMessage;
				TempNoticeDialog.DoModal();
			}

			//if (TempFoundEdges)
			//if ((vLocalSystemData->vITIPCDig->vOriginalImage->vRight >= vLocalSystemData->vITIPCDig->vOriginalBufferSizeY - 3) || (vLocalSystemData->vITIPCDig->vOriginalImage->vLeft <= 3))
			//{
			//	CNoticeDialog TempNoticeDialog;
			//	//TempText.LoadString(IDS_3Lines_WaitingForAGapOfAtleast2FeetToAutoSize);
			//	TempNoticeDialog.vNoticeText = "\n\nScanTrac could not find the edges of the container.\nsend another container through to see if\nit can find both container edges.";
			//	TempNoticeDialog.vType = cErrorMessage;
			//	TempNoticeDialog.vAutoClose = 900;
			//	TempNoticeDialog.DoModal();
			//	TempFoundEdges = false;
			//}
			//else
			//if (TempContainerWidth < 1.5)
			//{
			//	CYesNoDialog TempYesNoDialog;
			//	CString TempText1 = "\nContainer Width Found to be only: ";
			//	TempText1 = TempText1 + dtoa(TempContainerWidth, 2) +
			//		"\nDo you want to AutoSize anyway?\nIf not, Manually increase width, and then send a container\nthrough to see if it can find both container edges.";
			//	TempYesNoDialog.vNoticeText =  TempText1;
			//	TempYesNoDialog.vQuestionType = cConfirmQuestion;
			//	TempYesNoDialog.vYesButtonText = "AutoSize";
			//	TempYesNoDialog.vNoButtonText = "Don't AutoSize";
			//	int TempResult = TempYesNoDialog.DoModal();
			//	if (TempResult != IDOK)
			//		TempFoundEdges = false;
			//}

			if (TempFoundEdges)
			{
				double TempNewProductImageWidth = (float)TempContainerWidth;

				//5/27/04 make image 30% wider than container on each side.
				vImageMargin = TempNewProductImageWidth * 0.30;

				if (vImageMargin > 4)
					vImageMargin = 4;
				TempNewProductImageWidth = TempNewProductImageWidth + (2 * vImageMargin);
						
				//ReportErrorMessage("Prliminary TempNewProductImageWidth: " + dtoa(TempNewProductImageWidth,2), cWriteToLog,0);

				////make new width divisible by 8
				int TempCorrectDelayAmount = 0;
				WORD TempImageWidthInPixels = (WORD)(TempNewProductImageWidth * vGlobalPixelsPerUnit * vGlobalCurrentProduct->vOverScanMultiplier);
				//double TempRoundMargin = .5 / (vGlobalPixelsPerUnit * vGlobalCurrentProduct->vOverScanMultiplier);
				for (BYTE TempLoop = 0; TempLoop < 4; TempLoop++)
				{  //width must be divisible by 8
					int TempMod = TempImageWidthInPixels % 8; //mod 8
					if (TempMod > 4)
					{
						TempImageWidthInPixels++;
						TempCorrectDelayAmount++;
					}
					else
					if (TempMod > 0)
					{
						TempImageWidthInPixels--;
						TempCorrectDelayAmount--;
					}
				}
				TempCorrectDelayAmount = TempCorrectDelayAmount / 2;
				ReportErrorMessage("TempCorrectDelayAmount: " + dtoa(TempCorrectDelayAmount,2), cWriteToLog,0);

				if (vGlobalPixelsPerUnit * vGlobalCurrentProduct->vOverScanMultiplier)
					//TempNewProductImageWidth = (float)(((double)TempImageWidthInPixels + TempRoundMargin) / (vGlobalPixelsPerUnit * vGlobalCurrentProduct->vOverScanMultiplier));
					TempNewProductImageWidth = (float)(((double)TempImageWidthInPixels) / (vGlobalPixelsPerUnit * vGlobalCurrentProduct->vOverScanMultiplier));

				//ReportErrorMessage("Final TempNewProductImageWidth: " + dtoa(TempNewProductImageWidth,2), cWriteToLog,0);

				////TempNewProductImageWidth = 17.68; // for testing only
				//vImageMargin = TempNewProductImageWidth * 0.30;
				//if (vImageMargin > 4)
				//	vImageMargin = 4;
				double TempWhiteSpaceOnOneSideOfImage = vImageMargin;
			
				float TempNewProductBodyTriggerToImageBeltPositionOffset = (float)(vGlobalCurrentProduct->vProductBodyTriggerToImageDistanceInInches +
					vGlobalCurrentProduct->vProductImageWidth - vImageMargin - (vLocalSystemData->vITIPCDig->vOriginalImage->vRight / (vGlobalPixelsPerUnit * vGlobalCurrentProduct->vOverScanMultiplier)));

				TempNewProductBodyTriggerToImageBeltPositionOffset = (float)(TempNewProductBodyTriggerToImageBeltPositionOffset - (TempCorrectDelayAmount / (vGlobalPixelsPerUnit * vGlobalCurrentProduct->vOverScanMultiplier)));

				if (TempNewProductBodyTriggerToImageBeltPositionOffset < 0)
					TempNewProductBodyTriggerToImageBeltPositionOffset = 0;

				if (vLocalConfigurationData->vSystemBodyTriggerToXRayBeam > 1)
				{
					double TempTriggerToBeamOffset = -(vLocalConfigurationData->vSystemBodyTriggerToXRayBeam - vGlobalCurrentProduct->vProductBodyTriggerToImageDistanceInInches - TempWhiteSpaceOnOneSideOfImage);

					//if within .25 inches of no offset, set no offset
					if ((TempTriggerToBeamOffset > -.25) && (TempTriggerToBeamOffset < .25))
						ReportErrorMessage("TempNewProductBodyTriggerToImageBeltPositionOffset within .25inch: " + dtoa(TempTriggerToBeamOffset,2), cWriteToLog,0);
					else
						ReportErrorMessage("TempNewProductBodyTriggerToImageBeltPositionOffset OFF MORE THAN .25inch: " + dtoa(TempTriggerToBeamOffset,2), cWriteToLog,0);

						//ReportErrorMessage("TempNewProductBodyTriggerToImageBeltPositionOffset Original: " + dtoa(TempNewProductBodyTriggerToImageBeltPositionOffset,2), cWriteToLog,0);

						//TempNewProductBodyTriggerToImageBeltPositionOffset = (float)(vLocalConfigurationData->vSystemBodyTriggerToXRayBeam - TempWhiteSpaceOnOneSideOfImage);

						ReportErrorMessage("TempNewProductBodyTriggerToImageBeltPositionOffset Final: " + dtoa(TempNewProductBodyTriggerToImageBeltPositionOffset,2), cWriteToLog,0);

						ReportErrorMessage("vImageMargin: " + dtoa(vImageMargin, 2), cWriteToLog,0);
				}

				//container is at far right, so need to reduce the delay 2 inches
				if (vLocalSystemData->vITIPCDig->vOriginalImage->vRight >= vLocalSystemData->vITIPCDig->vOriginalBufferSizeY - 5)
				{  // if container was on left edge of image, enlarge image by two inches
					//then re-size again
					TempImageGrew = true;
					//move the Conveyor position delay 1 inch over so container in center
					TempNewProductBodyTriggerToImageBeltPositionOffset = (vGlobalCurrentProduct->vProductBodyTriggerToImageDistanceInInches -	1);

					if (TempNewProductBodyTriggerToImageBeltPositionOffset < 0)
						TempNewProductBodyTriggerToImageBeltPositionOffset = 0;

					if (TempNewProductBodyTriggerToImageBeltPositionOffset > 25)
						TempNewProductBodyTriggerToImageBeltPositionOffset = 2;

					TempNewProductImageWidth = (TempNewProductImageWidth + 2);
					//vNeedToSetSize = true;
					CString TempText;
					TempText.LoadString(IDS_WaitingForContainer);
					SetStatusLine(TempText + ".", 0, false);
				}
				//else
				if (vLocalSystemData->vITIPCDig->vOriginalImage->vLeft <= 5)
				{//container on left edge, so add 2 inches to width
					TempImageGrew = true;
					TempNewProductImageWidth = (TempNewProductImageWidth + 2);
					CString TempText("");
					TempText.LoadString(IDS_WaitingForContainer);
					SetStatusLine(TempText + ".", 0, false);
				}
									
				if (vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac)  //on Trio, can only have 672 lines in the image
				{
					WORD TempNumberOfLines = (WORD)(TempNewProductImageWidth * vGlobalPixelsPerUnit * vGlobalCurrentProduct->vOverScanMultiplier);
					if (TempNumberOfLines > 672)
					{
						TempNewProductImageWidth =  (float)((672 / vGlobalPixelsPerUnit) / vGlobalCurrentProduct->vOverScanMultiplier);
					}
				}

				double TempNewProductImageHeightTop = ((vLocalSystemData->vITIPCDig->vOriginalImage->vTop / vGlobalPixelsPerUnitInHeight) + 2);  //give 2 inches of white space above containers

				float TempNewProductImageHeightBottom = 0;

				if ((vGlobalScanTracType != cSoloScanTrac) && (vGlobalScanTracType != cSoloPlusScanTrac))
					TempNewProductImageHeightBottom = (float)((vLocalSystemData->vITIPCDig->vOriginalImage->vBottom / vGlobalPixelsPerUnitInHeight) - 2);  //on TD give 2 inches of white space below containers

				//TempNewProductImageHeightBottom = 0; // temp for test
				//TempNewProductImageHeightTop = vMaxDetectorHeight; // temp for test

				if (TempNewProductImageHeightBottom < 0)
					TempNewProductImageHeightBottom = 0;
				if (TempNewProductImageHeightBottom > vMaxDetectorHeight * .75)
					TempNewProductImageHeightBottom = 0;
				if (TempNewProductImageHeightTop > vMaxDetectorHeight)
					TempNewProductImageHeightTop = vMaxDetectorHeight;
				if (TempNewProductImageHeightTop < 0)
					TempNewProductImageHeightTop = vMaxDetectorHeight;
				if (TempNewProductImageHeightTop < TempNewProductImageHeightBottom)
					TempNewProductImageHeightBottom = 0;

				float TempOriginalProductBodyTriggerToImageBeltPositionOffset = vGlobalCurrentProduct->vProductBodyTriggerToImageDistanceInInches;

				vGlobalCurrentProduct->SetProductBodyTriggerToImageBeltPositionOffset(TempNewProductBodyTriggerToImageBeltPositionOffset);

				if (vGlobalCurrentProduct->vProductImageWidth + .3 < TempNewProductImageWidth)
				{
					TempImageGrew = true;
				}
				vGotImageToAutoSize = false;

				vGlobalCurrentProduct->SetProductImageWidth((float)TempNewProductImageWidth);
				vGlobalCurrentProduct->vProductImageHeightTop = (float)TempNewProductImageHeightTop;
				vGlobalCurrentProduct->vProductImageHeightBottom = TempNewProductImageHeightBottom;

				if (((TempNewProductImageWidth < 12) && (TempNewProductImageWidth > 2)) || (vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || 
					(vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vGlobalScanTracType == cAllegroScanTrac) ||(vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac))
					vNeedToAutoSizeNextContainer = false;

				double TempWidth = TempNewProductImageWidth;
				double TempDelta = vImageMargin * 2;
				if (TempDelta < TempWidth)
					TempWidth = (float)(TempWidth - TempDelta);

				//ask user if should figure out a new retrigger lockout width
				BYTE TempOKToCalculateNewLockout = true;
				vGlobalCurrentProduct->vHaveManuallySetRetriggerLockout = false;

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

					if (TempNewBodyTriggerLockout < 0.5)
						TempNewBodyTriggerLockout = 0.5;
								
					vGlobalCurrentProduct->SetProductLockOutWidth((float)TempNewBodyTriggerLockout);
				}
				vLocalSystemData->vChangedSizeOfProduct = true;
				vLocalSystemData->vChangedSizeOfProductCheckDensityROI = 1;
				vGlobalCurrentProduct->CheckAllROIs(vLocalSystemData->vITIPCDig->vProductAverageImage);

				vLocalSystemData->vITIPCDig->CalculateAverageDriftDensity(vGlobalCurrentProduct);
				vLocalSystemData->vIndexForDriftDensityOfLastNImages = 0;

				vGlobalCurrentProduct->vReferenceLeft = 0;
				vGlobalCurrentProduct->vReferenceRight = 0;
				vGlobalCurrentProduct->vReferenceTop = 0;
				vGlobalCurrentProduct->vReferenceBottom = 0;
				vGlobalCurrentProduct->vReferenceWidth = 0;

				vLocalSystemData->vITIPCDig->vDriftCompensationLocked = true;
				vGlobalCurrentProduct->ClearInspectionHistory();

				//9/30/2013
				//if (TempNewProductImageHeightTop - TempNewProductImageHeightBottom - vGlobalCurrentProduct->vProductImageHeightBottom)
				//	vXScale = (float)(vLocalSystemData->vITIPCDig->vOriginalBufferSizeX / 
				//		((TempNewProductImageHeightTop - TempNewProductImageHeightBottom - vGlobalCurrentProduct->vProductImageHeightBottom) * vGlobalPixelsPerUnitInHeight));

				//vYScale = ((float)vLocalSystemData->vITIPCDig->vOriginalBufferSizeY / (float)(vGlobalCurrentProduct->vImageWidthPosition));
				//vXOffset = (WORD)(TempNewProductImageHeightBottom * vGlobalPixelsPerUnitInHeight);

				//if (TempNewProductBodyTriggerToImageBeltPositionOffset > TempOriginalProductBodyTriggerToImageBeltPositionOffset)
				//	vYOffset = (WORD)(vLocalSystemData->vITIPCDig->vOriginalImage->vLeft - (vImageMargin * vGlobalPixelsPerUnit * vGlobalCurrentProduct->vOverScanMultiplier));

				double TempTriggerToBeamOffset = vGlobalCurrentProduct->vProductBodyTriggerToImageDistanceInInches;
				
				if (vLocalConfigurationData->vSystemBodyTriggerToXRayBeam > 1)
					TempTriggerToBeamOffset = -(vLocalConfigurationData->vSystemBodyTriggerToXRayBeam - vGlobalCurrentProduct->vProductBodyTriggerToImageDistanceInInches - TempWhiteSpaceOnOneSideOfImage);
				
				if ((vGlobalScanTracType != cSoloScanTrac) && (vGlobalScanTracType != cSoloPlusScanTrac))
				{
					vGlobalCurrentProduct->SetEdgeLocationHeight((float)(vGlobalCurrentProduct->vProductImageHeightTop * 0.8));
					vGlobalCurrentProduct->SetEdgeLocationBottom((float)(vGlobalCurrentProduct->vProductImageHeightTop * 0.1));
				}

				vGlobalCurrentProduct->vMaximumImageOverlap = (vGlobalCurrentProduct->vProductImageWidth / 2) - .1;
				vGlobalCurrentProduct->vReferenceLeft = 0;
				vGlobalCurrentProduct->vReferenceWidth = 0;

				//set the ejector timing based on system setting and container width
				if (vLocalConfigurationData->vSystemEjectorPositionOffset > 10)
				{
					float TempEjectorDelayPosition = 0;
					if (vLocalConfigurationData->vSystemEjectorDistance[0] > 10)
					{
						TempEjectorDelayPosition = (float)(vLocalConfigurationData->vSystemEjectorDistance[0] + vLocalConfigurationData->vSystemEjectorPositionOffset + (TempContainerWidth / 2));
						vGlobalCurrentProduct->SetEjectorBeltPositionOffset(0, TempEjectorDelayPosition);
					}
					if (vLocalConfigurationData->vSystemEjectorDistance[1] > 10)
					{
						TempEjectorDelayPosition = (float)(vLocalConfigurationData->vSystemEjectorDistance[1] + vLocalConfigurationData->vSystemEjectorPositionOffset + (TempContainerWidth / 2));
						vGlobalCurrentProduct->SetEjectorBeltPositionOffset(1, TempEjectorDelayPosition);
					}
					if (vLocalConfigurationData->vSystemEjectorDistance[2] > 10)
					{
						TempEjectorDelayPosition = (float)(vLocalConfigurationData->vSystemEjectorDistance[2] + vLocalConfigurationData->vSystemEjectorPositionOffset + (TempContainerWidth / 2));
						vGlobalCurrentProduct->SetEjectorBeltPositionOffset(0, TempEjectorDelayPosition);
					}
					vGlobalCurrentProduct->CalculateEndOfLineTimeOut();
				}

				vLocalSystemData->vITIPCDig->RenameLearnFilesToBackup(vGlobalCurrentProduct);
				ChangedProductSettingsSoReload();
				vLocalSystemData->vITIPCDig->DeleteLearnImages();
				vLocalSystemData->vITIPCDig->DeleteLearnFiles(vGlobalCurrentProduct);
			}
			//9/30/2013
			//TempText = "Press NEXT.  Image was sized." + TempText;
			SetStatusLine("Send another container\nto see new size", 2000, true);
			//9/30/2013
			//DrawImageOnScreen();
			//vHaveScaledImageToPaint = true;

			//m_SubFunction8Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
			//m_SubFunction8Button.ShowWindow(SW_SHOW);
			//TempText.LoadString(IDS_Proceed);
			//SetDlgItemText(IDC_SubFunction8Button,TempText);

			ReportErrorMessage("Finish AutoSize Image Width: " + dtoa(vGlobalCurrentProduct->vProductImageWidth, 2), cWriteToLog, 0);
		}
	}  //end of setting image size
}

void CTDAutoSetupImageDisplayDialog::StartSizeMenu()
{
	ReportErrorMessage("Express Setup Start Size Menu, Voltage:" + dtoa(vGlobalCurrentProduct->vXRaySourceVoltage * 6.0, 1) + ", Current: " + dtoa(vGlobalCurrentProduct->vXRaySourceCurrent, 2), cWriteToLog, 0);

	vMode = cSizeMenu;
	CString TempText("");
	//if in autosetup and not a pipeline, find edges from right side only
	vGlobalInAutoSize = 2; 

	vDialogTitleString.LoadString(IDS_AutoSetupImageSize);
	SetDlgItemText(IDC_DialogTitleStaticText1,vDialogTitleString);

	if (vLocalSystemData->vXRayOnNotice)
		SetStatusLine("Send one container to size image.", 2000, false);
	else
		SetStatusLine("Starting X-Rays.  Please wait...", 0, false);

	if (vLocalSystemData->vXRayOnNotice)  //may not have updated display while name menu on top
	{
		TempText.LoadString(IDS_XRAYON);
		SetDlgItemText(IDC_XRaysOn,TempText);
	}

	vNeedToAutoSizeNextContainer = true;
}

bool CTDAutoSetupImageDisplayDialog::FinishSizeMenu(bool TempWarn, bool TempExiting)
{
	//bool TempFinishedImageSize = true;

	//if (TempFinishedImageSize)
	//if (vLocalSystemData->vChangedSizeOfProduct)

	if (vHaveAnImage)
	{
		if ((vLocalSystemData->vITIPCDig->vOriginalImage->vLeft) && (!vGlobalCurrentProduct->vReferenceLeft))
		{
			vGlobalCurrentProduct->vReferenceLeft = vLocalSystemData->vITIPCDig->vOriginalImage->vLeft;
			vGlobalCurrentProduct->vReferenceRight = vLocalSystemData->vITIPCDig->vOriginalImage->vRight;
			vGlobalCurrentProduct->vReferenceTop = vLocalSystemData->vITIPCDig->vOriginalImage->vTop;
			vGlobalCurrentProduct->vReferenceBottom = vLocalSystemData->vITIPCDig->vOriginalImage->vBottom;
			ReportErrorMessage("Finish Size Menu-Set Reference From Image Left: " + dtoa(vLocalSystemData->vITIPCDig->vOriginalImage->vLeft, 2) + ", Right: " + 
				dtoa(vLocalSystemData->vITIPCDig->vOriginalImage->vRight, 2), cWriteToLog, 0);
		}
		else
		if (!vGlobalCurrentProduct->vReferenceLeft)
			ReportErrorMessage("Finish Size Menu-No Reference No Left: " + dtoa(vLocalSystemData->vITIPCDig->vOriginalImage->vLeft, 2) + ", Right: " + dtoa(vLocalSystemData->vITIPCDig->vOriginalImage->vRight, 2), cWriteToLog, 0);
	}
	else
		ReportErrorMessage("Finish Size Menu-No Image Reference Left: " + dtoa(vGlobalCurrentProduct->vReferenceLeft, 2) + ", Right: " + dtoa(vGlobalCurrentProduct->vReferenceRight, 2), cWriteToLog, 0);

		ReportErrorMessage("End Size Menu-Reference Left: " + dtoa(vGlobalCurrentProduct->vReferenceLeft, 2) + ", Right: " + dtoa(vGlobalCurrentProduct->vReferenceRight, 2), cWriteToLog, 0);

	if (vExitCode == 0) //don't do if aborting from name menu
	{
		bool TempMakeInspectionsFullImage = true;
		if ((vGlobalScanTracType == cSoloScanTrac) || (vGlobalScanTracType == cSoloPlusScanTrac))
			TempMakeInspectionsFullImage = false;
		vGlobalCurrentProduct->SetupAllInspectionROIsFromReference(vLocalSystemData->vITIPCDig->vOriginalImage, true, TempMakeInspectionsFullImage, vLocalSystemData->vITIPCDig->vProductAverageImage);
	}

	//if (TempFinishedImageSize)
	{
		vNeedToAutoSizeNextContainer = false;
		vLocalSystemData->vRemoveBackgroundInAutoSize = false;

		vHaveScaledImageToPaint = false;
		vGlobalInAutoSize = false;

		vMakeStatusLineYellowRed = false;
	}

	m_SubFunction5Button.ShowWindow(SW_HIDE);
	this->InvalidateRect(&m_SubFunction5Button.GetRectToInvalidateArrow());

	m_SubFunction8Button.ShowWindow(SW_HIDE);
	this->InvalidateRect(&m_SubFunction8Button.GetRectToInvalidateArrow());
	m_SubFunction8Display.ShowWindow(SW_HIDE);
	
	//return TempFinishedImageSize;
	return true;
}

void CTDAutoSetupImageDisplayDialog::DoneRunningSoExit()
{  //not done if edit while running, only done in auto setup
	vStartingUpSource = true;

	if ((vExitCode != 1) && (vExitCode != 20))//1 = save and exit 
	{  //hit abort, so set no current product
		//vMainWindowPointer->SendSourceSettingsToUController(0,0);		
		//was a new product so delete data files and set no current product
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
		
		if (vLocalSystemData->vITIPCDig->vProductAverageImage != NULL)
		{
			delete vLocalSystemData->vITIPCDig->vProductAverageImage;
			vLocalSystemData->vITIPCDig->vProductAverageImage = NULL;
		}
	}
	else
	{
		vLocalSystemData->vCurrentProductChanged = true;
		vLocalSystemData->vEjectTooLateMessageGivenThisRun = false;
	}
	vLocalSystemData->vITIPCDig->vMainDisplayIsDefaultImage = false;
	vMainWindowPointer->ResetCounters();

	vLocalSystemData->vInExpressSetup = false;
	vLocalSystemData->vInAutoSetup = false;
	CDialog::EndDialog(vExitCode);
}

//void CTDAutoSetupImageDisplayDialog::WaitThenDrawImageOnScreen() 
//{
//	if (vWaitingToUpdateScreen)
//		KillTimer(vWaitToUpdateScreenTimerHandle);
//
//	vWaitingToUpdateScreen = true;
//	int TimerResult = SetTimer(vWaitToUpdateScreenTimerHandle,300,NULL);
//	if (!TimerResult)
//		ReportErrorMessage("Error-Wait to update screen Timer Failed",cEMailInspx,32000);
//}
//
void CTDAutoSetupImageDisplayDialog::StartExposureMenu()
{
	vMode = cExposureMenu;
	CString TempText("");

	if ((vGlobalScanTracType == cSoloScanTrac) || (vGlobalScanTracType == cSoloPlusScanTrac))
	if (vNeedToAutoSizeNextContainer == false)
	if (vIntegrationTimeSet)
		//AutoAdjustExposure();
	{
		vLocalSystemData->vRampXRayVoltageAmount = 70.0 / 6.0;
		vLocalConfigurationData->LimitToMaximumVoltage(vLocalSystemData->vRampXRayVoltageAmount);

		vGlobalCurrentProduct->vXRaySourceVoltage = (float)vLocalSystemData->vRampXRayVoltageAmount;

		if (vLocalSystemData->vRampXRayCurrentAmount > 5)
			vLocalSystemData->vRampXRayCurrentAmount = 5;
		if (vLocalSystemData->vRampXRayCurrentAmount < 2)
			vLocalSystemData->vRampXRayCurrentAmount = 2;

		vLocalConfigurationData->LimitToMaximumCurrent(vLocalSystemData->vRampXRayVoltageAmount, vLocalSystemData->vRampXRayCurrentAmount);
		vGlobalCurrentProduct->vXRaySourceCurrent = (float)vLocalSystemData->vRampXRayCurrentAmount;
		vMainWindowPointer->SendSourceSettingsToUController(vLocalSystemData->vRampXRayVoltageAmount, vLocalSystemData->vRampXRayCurrentAmount);	
	}

	vDialogTitleString.LoadString(IDS_AutoSetupExposure);
	SetDlgItemText(IDC_DialogTitleStaticText1,vDialogTitleString);

	SetStatusLine("Send one container\n to adjust exposure.", 2000, false);

	//SetDlgItemText(IDC_SubFunction7Display, "Voltage: " + dtoa(vGlobalCurrentProduct->vXRaySourceVoltage * 6.0, 1));
	SetDlgItemText(IDC_SubFunction7Display, "V: " + dtoa(vGlobalCurrentProduct->vXRaySourceVoltage * 6.0, 1) + ", C: " + dtoa(vGlobalCurrentProduct->vXRaySourceCurrent, 2));

	SetDlgItemText(IDC_SubFunction4DisplayA, _T("Ave: ---"));
	SetDlgItemText(IDC_SubFunction8Display, _T("DP: ---"));
	if (PasswordOK(cTemporaryInspxPassword,false))
	{
		m_SubFunction7Display.ShowWindow(SW_SHOW);
		m_SubFunction4Display.ShowWindow(SW_SHOW);
		m_SubFunction8Display.ShowWindow(SW_SHOW);
	}

	//if (!vIntegrationTimeSet)
	//	CalculateBestIntegrationTime();
}

bool CTDAutoSetupImageDisplayDialog::FinishExposureMenu()
{
	bool TempMakeInspectionsFullImage = ((vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vGlobalScanTracType == cAllegroScanTrac) ||
		(vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac) || (vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode)); // Continuous Feed, or a Pipeline or TD

	vGlobalCurrentProduct->SetupAllInspectionROIsFromReference(vLocalSystemData->vITIPCDig->vOriginalImage, true, TempMakeInspectionsFullImage, vLocalSystemData->vITIPCDig->vProductAverageImage);

	m_SubFunction8Button.ShowWindow(SW_HIDE);
	this->InvalidateRect(&m_SubFunction8Button.GetRectToInvalidateArrow());
	m_SubFunction4Display.ShowWindow(SW_HIDE);
	m_SubFunction7Display.ShowWindow(SW_HIDE);
	m_SubFunction8Display.ShowWindow(SW_HIDE);
	return true;
}

void CTDAutoSetupImageDisplayDialog::StartEvaluateMenu()
{
	vMode = cEvaluateMenu;
								
	//must save final souce settings into product setup
	vGlobalCurrentProduct->vXRaySourceCurrent = (float)vLocalSystemData->vRampXRayCurrentAmount;
	vGlobalCurrentProduct->vXRaySourceVoltage = (float)vLocalSystemData->vRampXRayVoltageAmount;

	vDialogTitleString.LoadString(IDS_AutoSetupEvaluate);
	SetDlgItemText(IDC_DialogTitleStaticText1,vDialogTitleString);
	SetStatusLine("Send " + dtoa(cNumberOfImagesToEvaluate, 0) + " containers to evaluate.", 2000, false);

	vLocalSystemData->vQuickLearnImageIndex = 0;

	vLocalConfigurationData->vNumberOfQuickLearnImagesToTake = 10;

	vLocalSystemData->vIncludeRejectsInAverageAndStructure = true;

	if ((vGlobalScanTracType == cSoloScanTrac) || (vGlobalScanTracType == cSoloPlusScanTrac))
		vLocalSystemData->vEvaluateAndOrQuickLearn = cEvaluateAndQuickLearn;  //cQuickLearnOnly, cEvaluateAndQuickLearn, cEvaluateOnly
	else
		vLocalSystemData->vEvaluateAndOrQuickLearn = cEvaluateOnly;  //cQuickLearnOnly, cEvaluateAndQuickLearn, cEvaluateOnly

	vLocalSystemData->vTakingQuickLearnImages = true;
}

void CTDAutoSetupImageDisplayDialog::StartTestEjectorsMenu()
{
	vMode = cTestEjectorsMenu;

	SetDlgItemText(IDC_DialogTitleStaticText1, _T("Auto Setup Verify Eject Timing"));

	SetStatusLine("Press Test Ejector 1\nthen send a container.", 2000, true);

	for (BYTE TempEjectorLoop = 0; TempEjectorLoop < cNumberOfEjectors; TempEjectorLoop++)
		vEjectorUsed[TempEjectorLoop] = false;

	for (BYTE TempLoop = 0; TempLoop < vGlobalCurrentProduct->vNumberOfInspections; TempLoop++)
	{
		if (vGlobalCurrentProduct->vInspection[TempLoop])
		if (vGlobalCurrentProduct->vInspection[TempLoop]->vEjector)
		{
			for (BYTE TempEjectorLoop = 0; TempEjectorLoop < cNumberOfEjectors; TempEjectorLoop++)
			if (vGlobalCurrentProduct->vInspection[TempLoop]->vEjector & (1 << TempEjectorLoop))
			{
				vEjectorUsed[TempEjectorLoop] = true;
				if (vGlobalCurrentProduct->vMultiLaneNumberOfLanes)
				for (BYTE TempLaneLoop = 1; TempLaneLoop < vGlobalCurrentProduct->vMultiLaneNumberOfLanes; TempLaneLoop++)
				if (TempEjectorLoop + TempLaneLoop < cNumberOfEjectors)
					vEjectorUsed[TempEjectorLoop + TempLaneLoop] = true;
			}
		}
	}

	for (BYTE TempLoop = 0; TempLoop < cNumberOfExternalDetectors; TempLoop++)
	{
		if (vGlobalCurrentProduct->vExternalDetectorEnable[TempLoop])
		if (vGlobalCurrentProduct->vExternalDetectorMode[TempLoop])
		if (vGlobalCurrentProduct->vExternalDetectorEjector[TempLoop])
		{
			for (BYTE TempEjectorLoop = 0; TempEjectorLoop < cNumberOfEjectors; TempEjectorLoop++)
			if (vGlobalCurrentProduct->vExternalDetectorEjector[TempLoop] & (1 << TempEjectorLoop))
				vEjectorUsed[TempEjectorLoop] = true;
		}
	}

	for (BYTE TempLoop = cFillerSamplingType; TempLoop <= cSeamerSamplingType; TempLoop++)
	if (vLocalConfigurationData->vNumberOfHeadsToMonitor[TempLoop])
	if (vGlobalCurrentProduct->vSamplingEjector[TempLoop])
	{
		for (BYTE TempEjectorLoop = 0; TempEjectorLoop < cNumberOfEjectors; TempEjectorLoop++)
		if (vGlobalCurrentProduct->vSamplingEjector[TempLoop] & (1 << TempEjectorLoop))
			vEjectorUsed[TempEjectorLoop] = true;
	}

	if (vLocalConfigurationData->vEjector[0].vEnabled)
	{
		m_SubFunction5Button.ShowWindow(SW_SHOW);
		SetDlgItemText(IDC_SubFunction5Button, _T("Test Ejector\n1"));
	}
	else
	{
		m_SubFunction8Button.ShowWindow(SW_SHOW);
		CString TempText("");
		TempText.LoadString(IDS_Save);
		SetDlgItemText(IDC_SubFunction8Button,TempText);
	}

	if ((vLocalConfigurationData->vEjector[1].vEnabled) && (vEjectorUsed[1]))
	{
		m_SubFunction5Button.ShowWindow(SW_SHOW);
		SetDlgItemText(IDC_SubFunction6Button, _T("Test Ejector\n2"));

	}

	if ((vLocalConfigurationData->vEjector[2].vEnabled) && (vEjectorUsed[2]))
	{
		m_SubFunction5Button.ShowWindow(SW_SHOW);
		SetDlgItemText(IDC_SubFunction7Button, _T("Test Ejector\n3"));
	}
}


void CTDAutoSetupImageDisplayDialog::FinishEvaluateMenu()
{
	bool TempMakeInspectionsFullImage = true;
	if ((vGlobalScanTracType == cSoloScanTrac) || (vGlobalScanTracType == cSoloPlusScanTrac))
		TempMakeInspectionsFullImage = false;
	vGlobalCurrentProduct->SetupAllInspectionROIsFromReference(vLocalSystemData->vITIPCDig->vOriginalImage, true, TempMakeInspectionsFullImage, vLocalSystemData->vITIPCDig->vProductAverageImage);

		//set the ejector timing based on system setting and container width
	if (vLocalConfigurationData->vSystemEjectorPositionOffset > 10)
	{
		float TempEjectorDelayPosition = 0;
		if (vLocalConfigurationData->vSystemEjectorDistance[0] > 10)
		{
			TempEjectorDelayPosition = (float)(vLocalConfigurationData->vSystemEjectorDistance[0] + vLocalConfigurationData->vSystemEjectorPositionOffset + (vGlobalCurrentProduct->vReferenceWidth / vGlobalPixelsPerUnit / 2));
			vGlobalCurrentProduct->SetEjectorBeltPositionOffset(0, TempEjectorDelayPosition);
		}
		if (vLocalConfigurationData->vSystemEjectorDistance[1] > 10)
		{
			TempEjectorDelayPosition = (float)(vLocalConfigurationData->vSystemEjectorDistance[1] + vLocalConfigurationData->vSystemEjectorPositionOffset + (vGlobalCurrentProduct->vReferenceWidth / vGlobalPixelsPerUnit / 2));
			vGlobalCurrentProduct->SetEjectorBeltPositionOffset(1, TempEjectorDelayPosition);
		}
		if (vLocalConfigurationData->vSystemEjectorDistance[2] > 10)
		{
			TempEjectorDelayPosition = (float)(vLocalConfigurationData->vSystemEjectorDistance[2] + vLocalConfigurationData->vSystemEjectorPositionOffset + (vGlobalCurrentProduct->vReferenceWidth / vGlobalPixelsPerUnit / 2));
			vGlobalCurrentProduct->SetEjectorBeltPositionOffset(2, TempEjectorDelayPosition);
		}
		vGlobalCurrentProduct->CalculateEndOfLineTimeOut();
	}

	m_SubFunction5Button.ShowWindow(SW_HIDE);
	this->InvalidateRect(&m_SubFunction5Button.GetRectToInvalidateArrow());

	m_SubFunction8Button.ShowWindow(SW_HIDE);
	this->InvalidateRect(&m_SubFunction8Button.GetRectToInvalidateArrow());
}

void CTDAutoSetupImageDisplayDialog::ClearYellowWarningButton() 
{
	vMainWindowPointer->ClearYellowWarningButton();
	UpdateSubFunction4Button();
	SetDlgItemText(IDC_StatusLine,vCurrentStatusLineText);
}

void CTDAutoSetupImageDisplayDialog::AutoAdjustExposure()
{
	//these numbers were taken on a Solo at 70KV.  They are the multiplicative factor that the brightness increases as the current increases
	const double TempFactorFromIntegrationTime[19] = {0, 2.2, 4.5, 6.9, 9.3, 11.7, 14.2, 16.5, 19, 21.4, 23.8, 26.1, 28.5, 30.6, 33.2, 35.6, 38.1, 40.2, 42.7};

	//find the darkest pixel in the image, then try to adjust the exposure to make it 60
//kV	0	3.5	6.8	12.5	19.2	26	32.5	38.3	48.3	51.8	58.2	63.6	74
//represents 26 to 70 KV

//these numbers were taken on a 12 inch TD, Atempo
const double TempIntensityPoint8mmDetectors[45][13] = {																				//KV-KV offset
17.5,		14.5,		12.7,		9.6,  	7.5,  	5.4,		3.9,	 	3.3,		2.2,		1.9,		1.5,		1.1,		0.9, //26-0	
29,			24.7,		21.7,		17.2,		13.4,		10.2,		7.9,		5.9,		4.4,		3.6,		2.8,		2.2,		1.5,//27-1
36.3,		31,			27.6,		21.8,		17.1,		13.4,		10.4,		8.1,		5.6,		4.9,		3.9,		3.2,		2.3,//28-2
44.6,		38.5,		34.3,		27.2,		21.5,		16.7,		13.4,		10.6,		7.6,		6.4,		5.4,		4.1,		3.1,//29-3
54,			47,			41.7,		33.2,		26.6,		21,			16.8,		13.4,		9.4,		8.7,		7.1,		5.4,		4,//30-4
64.4,		56.1,		50.2,		40.7,		32.5,		25.8,		20.6,		16.6,		11.9,		10.6,		8.7,		6.9,		5,//31-5
75.8,		66.4,		59.6,		48.3,		38.9,		31,			24.9,		20.4,		14.3,		13.1,		10.7,		8.8,		6.1,//32-6
88.5,		77.5,		70.2,		56.9,		45.8,		36.9,		29.8,		24.5,		17.6,		16,			13,			10.4,		7.7,//33-7
102.1,	89.9,		81.4,		66.3,		53.7,		43.4,		35.2,		28.7,		20.8,		19.1,		15.6,		12.9,		10,//34-8

116.8,  103.1,	92.2,	  76.2,		61.9,	  50.1,   40.9,   34,			24.8,		22.3,		18.3,		15.4,		11.1,//35-9
149.1,  132.5,	120,	  97.8,		80.6,	  65.7,		53.7,		44.8,		33,			30,			24.6,		20.8,		15.2,//36-10
167,    148.8,  134.6,  108.6,  90.8,    74.3,   61,     51,     37.5,   34.3,  28.5,		23.7,		17.5,//37-11
186,    163.9,  150.3,  124.6,  101.7,   83.8,   68.7,   57.5,   42.7,   39.2,  32.4,		27.2,		20.4,//38-12
205.9,  183.6,  166.9,  137.4,  113.9,   93.9,   76.6,   64.6,   48,     44.1,  36.4,		30.5,		22.9,//39-13
227.1,  202.6,  184.3,  152.6,  125.1,  103.8,   85.4,   71.7,   53.6,   49.2,  41,			34.3,		27.2,//40-14
248.1,  223,    202.3,  168.4,  138.4,  114.1,   94.5,   79.4,   59.5,   54.6,  45.3,		38.4,		29,//41-15
254.9,  243.2,  221.5,  184.2,  151.9,  125.8,  103.6,   87.7,   65.7,   60.4,  50.4,		42.7,		32.4,//42-16
255,    254.5,  241.5,  199.2,  165.5,  136.9,  113.7,   95.6,   72,     66,    55.3,		46.9,		36.2,//43-17
255,    255,    253.9,  218.2,  180.4,  148.7,  124,    103.9,   78.8,   72.3,  60.6,		51.7,		43.6,//44-18	
255,    255,    255,    251.1,  210.3,  174.9,  145.2,  122.9,   93.1,   85.8,  71.6,		61,			46.9,//45-19
255,    255,    255,    254.9,  226.8,  188.1,  157,    132.9,  100.9,   91.9,  77.9,		65.9,		50.4,//46-20
255,    255,    255,    255,    243.1,  201.9,  168.4,  142.6,  108.5,   99.7,  84.1,		71.3,		55.1,//47-21
255,    255,    255,    255,    252.9,  216.1,  180.5,  152.7,  116.4,  107,    89.8,		76.8,		58.9,//48-22

255,		255,		255,		255,		255,		230.8,  192.7,  162,		124.3,  114.6,  97.5,		81.9,		63.7,//49	
255,		255,		255,		255,		255,		245.1,  205.5,  174.1,  132.9,  122.4,  103.3,	88,			68.2,//50	
255,		255,		255,		255,		255,		253.5,  218.6,  185,		141.2,  130.3,  110.3,  93.5,		73.1,//51	
255,		255,		255,		255,		255,		255,		232.2,  197,		149.1,  138.4,  116.9,  99.9,		77.6,//52	
255,		255,		255,		255,		255,		255,		245.5,  209.6,  159.7,  147.4,  124.6,  105.6,	82.6,//53
255,		255,		255,		255,		255,		255,		255,		235.1,  180.7,  165.8,  141.2,  120.1,  93.5,//54	
255,		255,		255,		255,		255,		255,		255,		247.6,  190.7,  175.8,  149.1,  127.4,  99.5,//55	
255,		255,		255,		255,		255,		255,		255,		253.8,  202.3,  186,		157.8,  135.1,  105,//56	
255,		255,		255,		255,		255,		255,		255,		255,		213.6,  197,		166.8,  142.8,  112.1,//57	
255,		255,		255,		255,		255,		255,		255,		255,		225.7,  208.1,  176.3,  150.8,  118.6,//58	
255,		255,		255,		255,		255,		255,		255,		255,		237.9,  218.9,  186.1,  159.2,  125.4,//59	
255,		255,		255,		255,		255,		255,		255,		255,		248.8,  231.5,  196.4,  168.3,  132.3,//60	
255,		255,		255,		255,		255,		255,		255,		255,		254,		242.8,  207.2,  177.7,  139.7,//61	
255,		255,		255,		255,		255,		255,		255,		255,		255,		251.6,  218.2,  186.8,  147.2,//62	
255,		255,		255,		255,		255,		255,		255,		255,		255,		255,		240.1,  206.5,  162.7,//63	
255,		255,		255,		255,		255,		255,		255,		255,		255,		255,		249.9,  216.5,  171.7,//64	
255,		255,		255,		255,		255,		255,		255,		255,		255,		255,		254.1,  227.2,  179.4,//65	
255,		255,		255,		255,		255,		255,		255,		255,		255,		255,		255,		238.2,  188,//66	
255,		255,		255,		255,		255,		255,		255,		255,		255,		255,		255,		247.5,  197,//67	
255,		255,		255,		255,		255,		255,		255,		255,	 255,			255,		255,		253,		206.9,//68	
255,		255,		255,		255,		255,		255,		255,		255,		255,		255,		255,		254.8,  217,//69	
255,		255,		255,		255,		255,		255,		255,		255,		255,		255,		255,		255,		225.3};//70	- Index 46

const double TempIntensityFortePoint8mmDetectors[45][13] = {																					//KV-KV offset
16.3,		15.4,		14.8,		13.6,		12.6,		11.8,		11,			10.5,		9.6,		9.4,		8.9,		8.5,		7.7,		//26
22,			20.6,		19.4,		17.5,		15.7,		14.3,		13.1,		12.2,		11.3,		11,			10.6,		10.1,		9.4,		//27
25.9,		24,			22.7,		20,			17.8,		16,			14.6,		13.5,		12.2,		11.9,		11.3,		10.8,		10.1,	//28
30.1,		28.2,		26.3,		23.1,		20.2,		18.1,		16.1,		14.9,		13.1,		12.9,		12.1,		11.5,		10.8,	//29
35,			31.8,		30.4,		26.6,		23.1,		20.4,		18.2,		16.6,		14.4,		14,			13.1,		12.4,		11.5,	//30
40.7,		37.8,		35.1,		30.5,		26.4,		23.3,		20.5,		18.4,		15.9,		15.3,		14.2,		13.2,		12.2,	//31
47,			43.5,		40.4,		35.1,		30.1,		26.2,		23.1,		20.7,		17.6,		16.9,		15.4,		14.3,		13,		//32
53.7,		49.9,		45.6,		40,			34.1,		29.9,		26,			23.2,		19.4,		18.6,		17,			15.5,		14,		//33
61.5,		57,			51.6,		45.5,		38.8,		33.7,		29.3,		25.9,		21.5,		20.5,		18.6,		17,			15,		//34
69.7,		64.2,		59.8,		57.4,		43.7,		37.9,		32.8,		28.9,		23.8,		22.7,		20.4,		18.6,		16.2,	//35
80.8,		82.5,		75.5,		64.8,		55,			47.5,		40.8,		35.8,		29.2,		27.6,		24.7,		22.3,		19.1,	//36
98.8,		91.1,		84.4,		72.2,		61.3,		52.8,		45.4,		39.6,		32.2,		30.2,		27,			24.2,		20.7,	//37
109.6,	101.1,	93.6,		79.3,		67.9,		58.4,		50.3,		43.9,		35.6,		33.5,		29.6,		26.5,		22.4,	//38
121.3,	111.9,	103.5,	88.6,		75,			64.5,		55.4,		48.3,		39.1,		36.7,		32.3,		28.8,		24.3,	//39
134,		121.1,	113.8,	97.4,		82.7,		70.8,		60.9,		52.9,		41.9,		40.2,		34.7,		31.4,		26.1,	//40
147,		135.1,	124.7,	106.3,	90.5,		77.5,		66.4,		57.9,		46.5,		43.7,		38.3,		34,			28.5,	//41
161.3,	147.8,	135.4,	114.4,	98.8,		84.8,		72.5,		62.8,		50.6,		47.7,		41.6,		36.4,		30.6,	//42
171.2,	161,		148.2,	126.9,	107.5,	91.8,		78.8,		67.8,		54.8,		51.6,		45,			39.3,		33,		//43
191.5,	174.8,	159.3,	137.5,	114.6,	99.6,		85.3,		73.2,		59.4,		55.6,		48.6,		42.7,		35.4,	//44
226.1,	205.4,	188.8,	159,		136,		115.8,	99.1,		85.8,		67.7,		64.4,		56.1,		49.7,		40.7,	//45
246.1,	219,		201,		172.8,	146.4,	124.5,	106.4,	92.5,		73.7,		68.8,		60.3,		53,			43.5,	//46
254.9,	240,		219.7,	182.4,	157,		133.4,	113.9,	99,			79,			73.8,		64.4,		56.6,		45.9,	//47
255,		253.6,	237.5,	199.3,	168.2,	140.2,	122,		105.7,	84.3,		78.9,		68.6,		60.4,		49.3,	//48
255,		255,		252.8,	214,		179.9,	152.6,	130.1,	112.9,	89.9,		84.1,		73.1,		64,			52.5,	//49
255,		255,		255,		229.3,	192.2,	162.8,	138.8,	118.6,	95.5,		89.4,		77.7,		68.2,		55.7,	//50
255,		255,		255,		245.8,	205.1,	172.9,	147.3,	127.4,	101.3,	94.9,		82.4,		72.3,		58.2,	//51
255,		255,		255,		254.7,	218.6,	183.7,	156.4,	135.3,	107.7,	100.6,	87.3,		76.6,		61.7,	//52
255,		255,		255,		255,		233.7,	191.8,	165.9,	143.6,	114.1,	106.7,	92.7,		80.7,		66,		//53
255,		255,		255,		255,		254.9,	222.1,	187.2,	161.5,	128,		119.5,	103.7,	90.7,		73.9,	//54
255,		255,		255,		255,		255,		236.9,	199.1,	171.3,	135.6,	126.6,	109.6,	96,			77.5,	//55
255,		255,		255,		255,		255,		251.4,	211.7,	179.8,	143.6,	134,		116.1,	100.8,	82.6,	//56
255,		255,		255,		255,		255,		255,		224.9,	192.2,	152.1,	141.9,	122.8,	107.2,	87,		//57
255,		255,		255,		255,		255,		255,		239,		203.7,	160.5,	149.8,	129.6,	113.2,	91.7,	//58
255,		255,		255,		255,		255,		255,		252.3,	216.1,	169.7,	158,		136.7,	119.2,	95.2,	//59
255,		255,		255,		255,		255,		255,		255,		229.5,	179.2,	166.9,	144,		125.6,	101.8,	//60
255,		255,		255,		255,		255,		255,		255,		243.4,	188.9,	176,		151.8,	132.3,	107.2,	//61
255,		255,		255,		255,		255,		255,		255,		253.7,	199.2,	185.3,	159.8,	139.4,	112.7,	//62
255,		255,		255,		255,		255,		255,		255,		255,		221.9,	205.7,	176.8,	153.9,	124.2,	//63
255,		255,		255,		255,		255,		255,		255,		255,		234.7,	215.8,	185.8,	161.7,	130.4,	//64
255,		255,		255,		255,		255,		255,		255,		255,		247.6,	228.6,	195.3,	169.8,	136.8,	//65
255,		255,		255,		255,		255,		255,		255,		255,		254.4,	241.5,	205.2,	178.1,	143.3,	//66
255,		255,		255,		255,		255,		255,		255,		255,		255,		252.1,	215.8,	183.9,	150,		//67
255,		255,		255,		255,		255,		255,		255,		255,		255,		254.9,	226.6,	195,		157,		//68
255,		255,		255,		255,		255,		255,		255,		255,		255,		255,		238.6,	205.2,	164.2,	//69
255,		255,		255,		255,		255,		255,		255,		255,		255,		255,		250,		215.2,	171.7};	//70

//taken on an Atempo 12 inch TD
const double TempIntensityPoint4mmDetectors[45][13] = {																				//KV-KV offset
17.8, 	17, 	  15.9, 	15.3, 	14.3, 	13.4, 	12.2, 	11.7, 	10.7,		9.4,		9.1,		9.2,		9,				//26KV - Index 0
22.1, 	20.8, 	19.8, 	17.8, 	16.7, 	15.5, 	14.7, 	13.9, 	12.7,		11.6,		11.3,		10.4,		9.5,			//27KV
24.9, 	23.2, 	22.1, 	20.1, 	17.9, 	16.9, 	15.8, 	14.3, 	13.7,		12.8,		12.3,		11.3,		10.5,		//28KV
28.1, 	26.1, 	24.8, 	22.2, 	20.1, 	18.3, 	17, 	  15.8, 	14.5,		14,			13.2,		12.4,		11.5,		//29KV
31.9, 	29.6, 	27.7, 	24.7, 	22, 	  19.9, 	18.4, 	17.1, 	15.5,		15,			13.9,		13.5,		12.1,		//30KV
36.2, 	33.4, 	31.2, 	27.5, 	24.3, 	21.9, 	19.9, 	18.4, 	16.6,		15.8,		15,			14.3,		13.1,		//31KV
40.8, 	37.6, 	34.1, 	30.8, 	27, 	  24, 	  21.7, 	19.9, 	17.7,		16.9,		16,			15.1,		14,			//32KV
45.9, 	42.1, 	39.3, 	34.3, 	30, 	  26.3, 	23.8, 	21.2, 	19,			18,			17,			15.9,		14.6,		//33KV
51.5, 	47.2, 	43.9, 	38.3, 	33.2, 	29.2, 	26, 	  23.5, 	20.1,		18.9,		17.8,		17,			15.6,		//34KV
57.4, 	52.7, 	47.1, 	42.5, 	36.9, 	32.2, 	28.4, 	25.6, 	22,			20.9,		19.3,		18.1,		16.4,		//35KV
70.9, 	64.7, 	60, 	  51.9, 	44.7, 	38.8, 	34, 	  30.2, 	25.5,		24.2,		22.1,		20.2,		18.2,		//36KV - Index 10
78.3, 	71.6, 	63.7, 	57.1, 	49, 	  42.5, 	37, 	  32.8, 	27.6,		26,			23.6,		21.7,		19.2,		//37KV
86.1, 	78.8, 	70.4, 	62.5, 	53.7, 	46.3, 	40.4, 	35.9, 	29.8,		27.9,		25.3,		23,			20.4,		//38KV
90.4, 	86.2, 	79.6, 	68.5, 	58.6, 	50.5, 	43.7, 	38.9, 	32,			29.9,		27.1,		24.6,		21.5,		//39KV
103, 	  94.1, 	87, 	  74.9, 	63.8, 	54.8, 	47.6, 	41.9, 	34.6,		32.3,		29,			26.3,		22.8,		//40KV
112.2, 	102.6, 	94.7, 	81.2, 	69.4, 	59.4, 	51.5, 	45.2, 	37.2,		34.5,		31,			28,			24.1,		//41KV
121.7, 	111,  	102.5, 	85.3, 	74.9, 	64.2, 	55.5, 	48.6, 	39.8,		37.1,		32.5,		29.8,		25.6,		//42KV
131.4, 	119.8, 	110.7, 	94.9, 	81, 	  69.3, 	59.6, 	52.2, 	41.5,		39.8,		35.4,		31.7,		27.1,		//43KV
141.3, 	129.2, 	119.2, 	98.7, 	86.9, 	74.3, 	64.1, 	56.1, 	45.5,		42.5,		37.7,		33.8,		28.7,		//44KV
163.3, 	148.8, 	137.5, 	117.6, 	100,  	85.5, 	73.5, 	64.1, 	51.8,		48.3,		42.6,		37.4,		32,			//45KV
174.5, 	159.4, 	146.8, 	125.9, 	107,  	91.4, 	78.5, 	68.3, 	55.2,		51.4,		45.3,		40.2,		33.8,		//46KV - Index 20
186.5, 	170.2, 	157.3, 	134.2, 	113.9, 	97.4, 	83.6, 	72.7, 	58.6,		54.5,		48,			42.6,		35.3,		//47KV
198.5, 	181.1, 	166.7, 	143, 	  121.4, 	103.6, 	88.8, 	77.2, 	62.2,		57.7,		50.1,		45.1,		37.7,		//48KV
210.9, 	192.7, 	177.4, 	151.8, 	128.5, 	109.9, 	94.3, 	81.7, 	65.9,		61.1,		53.7,		47.6,		39.7,		//49KV
223.8, 	203.9, 	188, 	  160.8, 	136.4, 	116.1, 	98.8, 	86.6, 	69.4,		64.5,		56.6,		50,			41.7,		//50KV
237.2, 	216.1, 	198.7, 	170,  	144.1, 	122.9, 	102.3,	91.5, 	73.4,		68.1,		59.7,		52.7,		43.8,		//51KV
250.6, 	228.6, 	210.5, 	179.3, 	152.5, 	129.8, 	111.1,	95.4, 	77.3,		71.8,		62.8,		55.4,		45.9,		//52KV
255,    241.9, 	222.3, 	189.8, 	160.8, 	136.9, 	117.2,	101.7,	81.5,		75.6,		66.2,		58.3,		48.3,		//53KV
255,    254.9, 	248.1, 	211.5, 	178.9, 	152.4, 	130.3,	113.1,	90.4,		83.9,		73.2,		64.3,		53.2,		//54KV
255,    255,    254.8, 	223.1, 	188.8, 	160.6, 	137.3,	118.9,	94.9,		87.6,		77,			67.4,		55.8,		//55KV
255,    255,    255,    235.2, 	198.8, 	168.9, 	144.6,	125.2,	100.2,	91.8,		80.9,		69.9,		58.4,		//56KV - Index 30
255,    255,    255,    247.3, 	209, 		177.9, 	151.7,	131.6,	105.2,	97.5,		84.9,		74.5,		61.1,		//57KV
255,    255,    255,    254.5, 	220.2, 	186.9, 	159.7,	138.1,	110.4,	102.4,	89.2,		78,			64.2,		//58KV
255,    255,    255,    255,    231.4, 	196.4, 	167.7,	145.1,	115.9,	107.3,	93.6,		80.5,		67.2,		//59KV
255,    255,    255,    255,    243.2, 	206.1, 	176.1,	152.3,	121.5,	112.6,	98.2,		85.9,		70.3,		//60KV
255,    255,    255,    255,    253.1, 	216.5, 	184.5,	159.8,	127.2,	117.9,	102.8,	90,			73.6,		//61KV
255,    255,    255,    255,    255,    226.9, 	193.3,	167.4,	133.2,	123.6,	107.6,	94.1,		77.2,		//62KV
255,    255,    255,    255,    255,    248.9, 	212.1,	178.9,	146.1,	135.1,	117.7,	101.7,	84.3,		//63KV
255,    255,    255,    255,    255,    254.5, 	221.7,	191.6,	152.4,	139.3,	122.7,	107.3,	87.9,		//64KV
255,    255,    255,    255,    255,    255,    231.7,	200.2,	157.5,	147.3,	128.2,	112.1,	91.4,		//65KV
255,    255,    255,    255,    255,    255,    242.7,  209.4,	163.3,	153.9,	134,		116.9,	95.8,		//66KV - Index 40
255,    255,    255,    255,    255,    255,    251.6,  218.3,	173.2,	160.5,	139.5,	121.9,	99.8,		//67KV
255,    255,    255,    255,    255,    255,    254.9,  225.8,	180.3,	167.2,	142.1,	126.9,	102.1,	//68KV
255,    255,    255,    255,    255,    255,    255,    237.2,	188,		174,		151.4,	132,		107.9,	//69KV
255,    255,    255,    255,    255,    255,    255,    247.2,	195.8,	179.8,	157.4,	137.3,	112.2}; //70KV = Index 44


//taken on an Largo 36 inch TD
const double TempIntensityLargo1Point6mmDetectors[45][13] = {																				//KV-KV offset
/*72.2,		66,			55.1,*/		44.9,		38.1,		32.3,		28.1,		23.1,		21.9,		19.7,		19,			16.5,		12.6,		11.1,		10.3,		10.4,		//26 kv
/*116,		104.8,	86.7,*/		71.1,		57.4,		48.7,		41.5,		33,			31.2,		27.4,		25.5,		21.1,		15.2,		12.5,		11,			10.9,		//27 
/*144.5,	130.6,	107.6,*/	87.2,		72.7,		59.8,		51,			39.8,		37.2,		32.3,		30.4,		24.2,		16.8,		13.6,		11.5,		11.1,		//28
/*179.5,	161.7,	131.9,*/	107.8,	87.8,		72.9,		61.9,		47.9,		44.8,		38.4,		35.5,		28.2,		18.2,		14.8,		12.4,		11.5,		//29
/*222.3,	191.2,	161,	*/	130.6,	107.7,	88.1,		74.6,		57.5,		53.5,		45.4,		42.3,		32.2,		20.9,		16			,13.7,	11.8,		//30
/*255,		251.6,	195.7,*/	157.6,	129.5,	105.7,	89.5,		68.4,		63.3,		54,			49.7,		38,			23.3,		17.3,		14.3,		12.7,		//31
/*255,		255,		245.3,*/	189.1,	154.2,	124.3,	105.7,	80.8,		74.7,		63.5,		58.2,		44.3,		26.7,		19,			15.2,		13,			//32
/*255,		255,		255,	*/	230,		182.1,	148.6,	122.5,	94.7,		87.6,		74,			68.1,		51.2,		30.2,		20.8,		16.1,		14.1,		//33
/*255,		255,		255,	*/	255,		217,		174.6,	146,		110.6,	102.3,	86.1,		79,			57.9,		34,			23,			17.3,		14.8,		//34
/*255,		255,		255,	*/	255,		254.7,	205,		165.5,	128.3,	118.5,	99.5,		91.4,		67.8,		38.7,		25.6,		18.8,		15.4,		//35
/*255,		255,		255,	*/	255,		255,		255,		233.7,	170,		156.1,	130.7,	118.2,	88.1,		49.4,		31.4,		22.2,		17.3,		//36
/*255,		255,		255,	*/	255,		255,		255,		255,		195,		178.3,	148.7,	136,		100,		55.8,		34.6,		24,			18.6,		//37
/*255,		255,		255,	*/	255,		255,		255,		255,		223.4,	203.1,	168.5,	153.8,	112.7,	62.7,		38.9,		26.3,		19.7,		//38
/*255,		255,		255,	*/	255,		255,		255,		255,		254.5,	236.6,	190.8,	173.7,	125.9,	70,			43.1,		29,			21.3,		//39
/*255,		255,		255,	*/	255,		255,		255,		255,		255,		254.9,	215.2,	195.2,	140.7,	77.8,		47.6,		31.7,		23.1,		//40
/*255,		255,		255,	*/	255,		255,		255,		255,		255,		255,		250.1,	220,		153.6,	86.2,		52.8,		34,			24.5,		//41
/*255,		255,		255,	*/	255,		255,		255,		255,		255,		255,		255,		252.4,	174.1,	95.9,		57.8,		37.7,		26.5,		//42
/*255,		255,		255,	*/	255,		255,		255,		255,		255,		255,		255,		255,		192.8,	105.2,	63.5,		40.8,		28.6,		//43
/*255,		255,		255,	*/	255,		255,		255,		255,		255,		255,		255,		255,		213.7,	114.8,	69.5,		44.4,		30.6,		//44
/*255,		255,		255,	*/	255,		255,		255,		255,		255,		255,		255,		255,		254.7,	137.2,	82.2,		51.9,		35.5,		//45
/*255,		255,		255,	*/	255,		255,		255,		255,		255,		255,		255,		255,		255,		149.1,	88.7,		57.2,		37.9,		//46
/*255,		255,		255,	*/	255,		255,		255,		255,		255,		255,		255,		255,		255,		161.8,	96.1,		60.4,		40.4,		//47
/*255,		255,		255,	*/	255,		255,		255,		255,		255,		255,		255,		255,		255,		174.9,	103.6,	64.9,		42.7,		//48
/*255,		255,		255,	*/	255,		255,		255,		255,		255,		255,		255,		255,		255,		186.9,	112,		69.4,		45.8,		//49
/*255,		255,		255,	*/	255,		255,		255,		255,		255,		255,		255,		255,		255,		204.9,	118.8,	74.2,		48.8,		//50
/*255,		255,		255,	*/	255,		255,		255,		255,		255,		255,		255,		255,		255,		221.9,	128.3,	79.5,		51.6,		//51
/*255,		255,		255,	*/	255,		255,		255,		255,		255,		255,		255,		255,		255,		246,		137.5,	84.3,		55.2,		//52
/*255,		255,		255,	*/	255,		255,		255,		255,		255,		255,		255,		255,		255,		254.6,	147.5,	90.7,		58.2,		//53
/*255,		255,		255,	*/	255,		255,		255,		255,		255,		255,		255,		255,		255,		255,		170.8,	103.7,	66.5,		//54
/*255,		255,		255,	*/	255,		255,		255,		255,		255,		255,		255,		255,		255,		255,		182.8,	111,		71.4,		//55
/*255,		255,		255,	*/	255,		255,		255,		255,		255,		255,		255,		255,		255,		255,		196.3,	118.5,	75.6,		//56
/*255,		255,		255,	*/	255,		255,		255,		255,		255,		255,		255,		255,		255,		255,		211.7,	126.5,	80.5,		//57
/*255,		255,		255,	*/	255,		255,		255,		255,		255,		255,		255,		255,		255,		255,		229.2,	133,		85.4,		//58
/*255,		255,		255,	*/	255,		255,		255,		255,		255,		255,		255,		255,		255,		255,		249.3,	141.1,	91.1,		//59
/*255,		255,		255,	*/	255,		255,		255,		255,		255,		255,		255,		255,		255,		255,		254.7,	149.1,	96.9,		//60
/*255,		255,		255,	*/	255,		255,		255,		255,		255,		255,		255,		255,		255,		255,		255,		160.7,	102.9,	//61
/*255,		255,		255,	*/	255,		255,		255,		255,		255,		255,		255,		255,		255,		255,		255,		171.4,	108.7,	//62
/*255,		255,		255,	*/	255,		255,		255,		255,		255,		255,		255,		255,		255,		255,		255,		193.8,	122.3,	//63
/*255,		255,		255,	*/	255,		255,		255,		255,		255,		255,		255,		255,		255,		255,		255,		206.9,	129.2,	//64
/*255,		255,		255,	*/	255,		255,		255,		255,		255,		255,		255,		255,		255,		255,		255,		220.5,	136.9,	//65
/*255,		255,		255,	*/	255,		255,		255,		255,		255,		255,		255,		255,		255,		255,		255,		240,		145.2,	//66
/*255,		255,		255,	*/	255,		255,		255,		255,		255,		255,		255,		255,		255,		255,		255,		252.2,	153.1,	//67
/*255,		255,		255,	*/	255,		255,		255,		255,		255,		255,		255,		255,		255,		255,		255,		255,		161.8,	//68
/*255,		255,		255,	*/	255,		255,		255,		255,		255,		255,		255,		255,		255,		255,		255,		255,		170.2,	//69
/*255,		255,		255,	*/	255,		255,		255,		255,		255,		255,		255,		255,		255,		255,		255,		255,		179.9};	//70

const BYTE cTargetBrightnessForDarkestPixel = 60;
const BYTE cTargetBrightnessForAverage = 133;
const BYTE cNumberOfVoltages = 31;
const BYTE cNumberOfBrightnesses = 13;

	BYTE TempTargetBrightness = cTargetBrightnessForDarkestPixel;

	if ((vLocalSystemData->vITIPCDig->vOriginalImage) && (vLocalSystemData->vITIPCDig->vOriginalImage->vGreyImage))
	{
		double TempIntensity[45][13];
		
		if ((vLocalConfigurationData->vPixelsPerDetector == 64) && (vGlobalScanTracType == cCaseInspectorScanTrac))
		{
			for (WORD TempLoopX = 0; TempLoopX < 45; TempLoopX++)
			for (WORD TempLoopY = 0; TempLoopY < 13; TempLoopY++)
				TempIntensity[TempLoopX][TempLoopY] = TempIntensityLargo1Point6mmDetectors[TempLoopX][TempLoopY];
		}
		else
		if ((vLocalConfigurationData->vPixelsPerDetector == 64) && (vGlobalScanTracType == cAllegroScanTrac))
		{
			for (WORD TempLoopX = 0; TempLoopX < 45; TempLoopX++)
			for (WORD TempLoopY = 0; TempLoopY < 13; TempLoopY++)
				TempIntensity[TempLoopX][TempLoopY] = TempIntensityFortePoint8mmDetectors[TempLoopX][TempLoopY];
		}
		else
		if ((vLocalConfigurationData->vPixelsPerDetector == 64) && (vGlobalScanTracType == cForteScanTrac))
		{
			for (WORD TempLoopX = 0; TempLoopX < 45; TempLoopX++)
			for (WORD TempLoopY = 0; TempLoopY < 13; TempLoopY++)
				TempIntensity[TempLoopX][TempLoopY] = TempIntensityFortePoint8mmDetectors[TempLoopX][TempLoopY];
		}
		else
		if (vLocalConfigurationData->vPixelsPerDetector == 128)
		{
			for (WORD TempLoopX = 0; TempLoopX < 45; TempLoopX++)
			for (WORD TempLoopY = 0; TempLoopY < 13; TempLoopY++)
				TempIntensity[TempLoopX][TempLoopY] = TempIntensityPoint4mmDetectors[TempLoopX][TempLoopY];
		}
		else
		//if (vLocalConfigurationData->vPixelsPerDetector == 64)
		{
			for (WORD TempLoopX = 0; TempLoopX < 45; TempLoopX++)
			for (WORD TempLoopY = 0; TempLoopY < 13; TempLoopY++)
				TempIntensity[TempLoopX][TempLoopY] = TempIntensityPoint8mmDetectors[TempLoopX][TempLoopY];
		}

		ReportErrorMessage("Start AutoExposure Original Product V: " + dtoa(vGlobalCurrentProduct->vXRaySourceVoltage * 6.0, 1) + ", I: " + dtoa(vGlobalCurrentProduct->vXRaySourceCurrent, 2), cWriteToLog, 0);
		//ReportErrorMessage("Start AutoExposure Original HVPS Voltage: " + dtoa(vLocalSystemData->vRampXRayVoltageAmount * 6.0, 1), cWriteToLog, 0);

		double TempOriginalVoltage = vLocalSystemData->vRampXRayVoltageAmount;
		double TempOriginalCurrent = vLocalSystemData->vRampXRayCurrentAmount;
		BYTE TempDarkestPixel = 255;
		BYTE *TempPointer = vLocalSystemData->vITIPCDig->vOriginalImage->vGreyImage;
		BYTE *TempPointerMaximum = vLocalSystemData->vITIPCDig->vOriginalImage->vGreyImage + vLocalSystemData->vITIPCDig->vOriginalBufferSize;
		double TempAverage = 0;
		double TempSum = 0;
		DWORD TempCount = 0;
		WORD TempHistogram[256];
		ZeroMemory(&TempHistogram[0], 256 * sizeof(WORD));

		//go through each pixel find lowest value
		for (DWORD TempLoopY = 0; TempLoopY < vLocalSystemData->vITIPCDig->vOriginalBufferSizeY; TempLoopY++)
		for (DWORD TempLoopX = 0; TempLoopX < vLocalSystemData->vITIPCDig->vOriginalBufferSizeX; TempLoopX++)
		{
			WORD TempHeightOfDetectorInPixels = (WORD)(vLocalConfigurationData->vDetectorLength * vGlobalPixelsPerUnitInHeight);
			if ((vLocalConfigurationData->vScanTracType != cAllegroScanTrac) || (TempLoopX > (WORD)(3 * TempHeightOfDetectorInPixels))) //ignore the bottom 3 detectors as they have funny saturation problem
			if ((TempLoopX % TempHeightOfDetectorInPixels > 4) && (TempLoopX % TempHeightOfDetectorInPixels < vGlobalPixelsPerUnitInHeight - 4)) //ignore 4 pixels on either edge of detector
			if ((TempPointer >= vLocalSystemData->vITIPCDig->vOriginalImage->vGreyImage) && (TempPointer < TempPointerMaximum))
			{
				if (*TempPointer > 0)
				{
					TempHistogram[*TempPointer]++;  //put in Histogram
					if (*TempPointer < TempDarkestPixel)
						TempDarkestPixel = *TempPointer;

					if (*TempPointer < 251)
					{
						TempSum = TempSum + *TempPointer;
						TempCount++;
					}
				}
			}
			else
				ReportErrorMessage("Pointer Error in AutoAdjustExposure-Loop: " + dtoa(TempLoopY, 0),cEMailInspx,32000);
			TempPointer++;
		}

		if (TempCount)
			TempAverage = TempSum / TempCount;
		else
			TempAverage = -1;
			
		ReportErrorMessage("AutoAdjustExposure TD Average: " + dtoa(TempAverage, 0), cWriteToLog, 0);

		if ((vGlobalScanTracType == cSoloScanTrac) || (vGlobalScanTracType == cSoloPlusScanTrac))
		{
			if (vGlobalCurrentProduct->vDriftDensityInspection)
			{
				TempAverage = vLocalSystemData->vITIPCDig->CalculateInspectionDensityAfterReference(vLocalSystemData->vITIPCDig->vOriginalImage->vGreyImage, vGlobalCurrentProduct->vDriftDensityInspection,
					vLocalSystemData->vITIPCDig->vOriginalImage->vLeftReferenceAdjust, vLocalSystemData->vITIPCDig->vOriginalImage->vTopReferenceAdjust, vLocalConfigurationData->vReferenceROIsToEdges, 
					vLocalSystemData->vITIPCDig->vOriginalImage->vLeft, vLocalSystemData->vITIPCDig->vOriginalImage->vRight);

				ReportErrorMessage("AutoAdjustExposure Solo Average: " + dtoa(TempAverage, 0), cWriteToLog, 0);
				ReportErrorMessage("AutoAdjustExposure Solo Image Left: " + dtoa(vLocalSystemData->vITIPCDig->vOriginalImage->vLeft, 0) + ", Image Right: " + dtoa(vLocalSystemData->vITIPCDig->vOriginalImage->vRight, 0), cWriteToLog, 0);
				ReportErrorMessage("AutoAdjustExposure Solo Image Reference Left: " + dtoa(vGlobalCurrentProduct->vReferenceLeft, 0) + ", Image Right: " + dtoa(vGlobalCurrentProduct->vReferenceRight, 0), cWriteToLog, 0);
				ReportErrorMessage("AutoAdjustExposure Solo Left Reference Adjust: " + dtoa(vLocalSystemData->vITIPCDig->vOriginalImage->vLeftReferenceAdjust, 0), cWriteToLog, 0);
				ReportErrorMessage("AutoAdjustExposure Solo Denisty Inspection Left: " + dtoa(vGlobalCurrentProduct->vDriftDensityInspection->vROILeft, 2) + ", Right: " + dtoa(vGlobalCurrentProduct->vDriftDensityInspection->vROIRight, 0), cWriteToLog, 0);
			}
			else
				ReportErrorMessage("AutoAdjustExposure Solo-No Density Inspection", cWriteToLog, 0);
		}

		//if (TempAverage < 5)
		//if (TempDarkestPixel > 200)
		//if (TempCount < 4000)
		//	TempAverage = -1;

		SetDlgItemText(IDC_SubFunction8Display,"DP: " + dtoa(TempDarkestPixel, 0));
		ReportErrorMessage("AutoAdjustExposure Darkest Pixel is: " + dtoa(TempDarkestPixel,0), cWriteToLog, 0);

		SetDlgItemText(IDC_SubFunction4DisplayA,"Ave: " + dtoa(TempAverage, 0));

		DWORD TempRunningTotal = 0;
		for (BYTE TempLoop = 0; TempLoop < 255; TempLoop++)
		{
			TempRunningTotal = TempRunningTotal + TempHistogram[TempLoop];

			BYTE TempNumberInCellToBeMinimumValue = 5;
			if (TempCount > 9000)
				TempNumberInCellToBeMinimumValue = 15;
			//if (TempRunningTotal > 9)
			if (TempHistogram[TempLoop] > TempNumberInCellToBeMinimumValue)
			{
				TempDarkestPixel = TempLoop;
				TempLoop = 254;
			}
		}
		SetDlgItemText(IDC_SubFunction8Display,"DP: " + dtoa(TempDarkestPixel, 0));
		ReportErrorMessage("AutoAdjustExposure Darkest Pixel (>25 counts) is: " + dtoa(TempDarkestPixel,0), cWriteToLog, 0);

		double TempImageReferenceValueToAdjustBy = TempDarkestPixel;

		//if ((((TempDarkestPixel < 10) || (TempDarkestPixel > 230)) && ((TempAverage > 10) && (TempAverage < 230))) || (vLocalConfigurationData->vScanTracType == cSoloScanTrac))
		if ((((TempDarkestPixel < 2) || (TempDarkestPixel > 230)) && ((TempAverage > 10) && (TempAverage < 230))) || (vGlobalScanTracType == cSoloScanTrac) || (vGlobalScanTracType == cSoloPlusScanTrac))
		{
			TempImageReferenceValueToAdjustBy = TempAverage;
			TempTargetBrightness = cTargetBrightnessForAverage;
			ReportErrorMessage("AutoAdjustExposure using Average Brightness, Average is: " + dtoa(TempAverage,0) + " Target: " + dtoa(TempTargetBrightness,0), cWriteToLog, 0);
		}

		bool TempExposureWasAdjusted = false;
		if ((TempImageReferenceValueToAdjustBy < 0) || (TempImageReferenceValueToAdjustBy > 255))
		{
			ReportErrorMessage("AutoAdjustExposure Brightness not calculated, skipping image", cWriteToLog, 0);
			TempExposureWasAdjusted = true;
		}
		else
		if (vLocalConfigurationData->vScanTracType == cSoloScanTrac)
		{
			//double TempAdjustmentFactor = 1;
			//
			//if ((TempTargetBrightness + 20 < TempImageReferenceValueToAdjustBy) || (TempTargetBrightness > TempImageReferenceValueToAdjustBy + 20))
			//TempAdjustmentFactor = TempTargetBrightness / TempImageReferenceValueToAdjustBy;

			double TempBrightnessToChangeBy = TempTargetBrightness - TempImageReferenceValueToAdjustBy;
			ReportErrorMessage("AutoAdjustExposure TempBrightnessToChangeBy: " + dtoa(TempBrightnessToChangeBy,0), cWriteToLog, 0);

			if ((TempBrightnessToChangeBy > 5) || (TempBrightnessToChangeBy < -10))
			{
				BYTE TempIntegrationIndex = vGlobalCurrentProduct->vXRayIntegrationTime / 250;  // vXRayIntegrationTime / 250 = integration time, * 10 is 1 for every .1mSec
				ReportErrorMessage("AutoAdjustExposure TempIntegrationIndex: " + dtoa(TempIntegrationIndex,0), cWriteToLog, 0);

				if ((TempIntegrationIndex < 1) || (TempIntegrationIndex > 18))
				{
					ReportErrorMessage("AutoAdjustExposure TempIntegrationIndex OUT OF RANGE: " + dtoa(TempIntegrationIndex,0), cEMailInspx, 0);
					TempIntegrationIndex = 6;
				}

				double TempFactorToMultiplyByCurrentToGetBrightnessChange = TempFactorFromIntegrationTime[TempIntegrationIndex];
				ReportErrorMessage("AutoAdjustExposure Correction factor at this integration time: " + dtoa(TempFactorToMultiplyByCurrentToGetBrightnessChange,2), cWriteToLog, 0);
				//TempFactorToMultiplyByCurrentToGetBrightnessChange = 1;

				double TempContainerWidth = (vLocalSystemData->vITIPCDig->vOriginalImage->vRight - vLocalSystemData->vITIPCDig->vOriginalImage->vLeft) / vGlobalPixelsPerUnit / vGlobalCurrentProduct->vOverScanMultiplier;
				ReportErrorMessage("AutoAdjustExposure TempContainerWidth: " + dtoa(TempContainerWidth,2), cWriteToLog, 0);

				//since brightness change from current change data was taken for at each integration time on a 3.4" diameter can at 70 KV, must correct if can diameter not 3.4"
				if ((TempContainerWidth > 1) && (TempContainerWidth < 7)) 
				if (TempContainerWidth > 3.5) //inches  container is wider than the 3.3" can did current rates on, so modify the rates to factor the width of the container
				{
					double TempWidthDifference = TempContainerWidth - 3.1; //was 3.3
					//TempFactorToMultiplyByCurrentToGetBrightnessChange = TempFactorToMultiplyByCurrentToGetBrightnessChange / (3.43 * TempWidthDifference);
					TempFactorToMultiplyByCurrentToGetBrightnessChange = TempFactorToMultiplyByCurrentToGetBrightnessChange / (4.0 * TempWidthDifference);
				}
				else
				if ((TempContainerWidth < 3.1) && (TempContainerWidth > 2.1)) //inches container is narrower than the 3.3" can did current rates on, so modify the rates to factor the width of the container
				{
					double TempWidthDifference = 3.5 - TempContainerWidth; //was 3.3
					TempFactorToMultiplyByCurrentToGetBrightnessChange = TempFactorToMultiplyByCurrentToGetBrightnessChange * (4 * TempWidthDifference);
				}
				ReportErrorMessage("AutoAdjustExposure Correction factor adjusted for Container Width: " + dtoa(TempFactorToMultiplyByCurrentToGetBrightnessChange,2), cWriteToLog, 0);

				ReportErrorMessage("AutoAdjustExposure Original Current: " + dtoa(vLocalSystemData->vRampXRayCurrentAmount,2), cWriteToLog, 0);
				double TempCurrentAmountToMakeChange = TempBrightnessToChangeBy / TempFactorToMultiplyByCurrentToGetBrightnessChange / 2; //the /2 needed as took current data every .5 mA
				ReportErrorMessage("AutoAdjustExposure TempCurrentAmountToMakeChange: " + dtoa(TempCurrentAmountToMakeChange,2), cWriteToLog, 0);

				//double TempVoltageFactor = 1;
				//if (vLocalSystemData->vRampXRayVoltageAmount * 6 <= 40)
				//	TempVoltageFactor = 1.2;
				//else
				//if (vLocalSystemData->vRampXRayVoltageAmount * 6 <= 50)
				//	TempVoltageFactor = 1.1;

				double TempCurrent = vLocalSystemData->vRampXRayCurrentAmount + TempCurrentAmountToMakeChange;
				if ((TempCurrent < 2) && (vLocalSystemData->vRampXRayVoltageAmount * 6 > 25)) //if the current would go below 2mA, drop the voltage by 5 KV instead
				{
					TempCurrent = vLocalSystemData->vRampXRayCurrentAmount;
					vLocalSystemData->vRampXRayVoltageAmount = (((vLocalSystemData->vRampXRayVoltageAmount * 6) - 5) / 6);
					vLocalConfigurationData->LimitToMaximumVoltage(vLocalSystemData->vRampXRayVoltageAmount);
					vGlobalCurrentProduct->vXRaySourceVoltage = (float)vLocalSystemData->vRampXRayVoltageAmount;
					ReportErrorMessage("AutoAdjustExposure New Voltage: " + dtoa(vLocalSystemData->vRampXRayVoltageAmount * 6,2), cWriteToLog, 0);
					TempExposureWasAdjusted = true;
				}
				else
				if (TempCurrent <= .1)
				{
					TempCurrent = vLocalSystemData->vRampXRayCurrentAmount / 2;
					TempExposureWasAdjusted = true;
				}

				ReportErrorMessage("AutoAdjustExposure New Current TempCurrent: " + dtoa(TempCurrent,2), cWriteToLog, 0);

				if (vLocalConfigurationData->LimitToMaximumCurrent(vLocalSystemData->vRampXRayVoltageAmount, TempCurrent))
				{
					ReportErrorMessage("AutoExposure new Voltage limits Current : " + dtoa(TempCurrent, 1), cWriteToLog, 0);
				
					if ((vGlobalCurrentProduct->vXRaySourceCurrent + .2 < (float)TempCurrent) || (vGlobalCurrentProduct->vXRaySourceCurrent - .2 > (float)TempCurrent))
						TempExposureWasAdjusted = true;
				}
				else
				{
					ReportErrorMessage("AutoExposure Final Current: " + dtoa(TempCurrent, 2), cWriteToLog, 0);
				
					if ((vGlobalCurrentProduct->vXRaySourceCurrent + .03 < (float)TempCurrent) || (vGlobalCurrentProduct->vXRaySourceCurrent - .03 > (float)TempCurrent))
						TempExposureWasAdjusted = true;
				}
				vGlobalCurrentProduct->vXRaySourceCurrent = (float)TempCurrent;
				vLocalSystemData->vRampXRayCurrentAmount = vGlobalCurrentProduct->vXRaySourceCurrent;
				vMainWindowPointer->SendSourceSettingsToUController(vLocalSystemData->vRampXRayVoltageAmount, vLocalSystemData->vRampXRayCurrentAmount);	
			}
		}
		else 
		if (vGlobalScanTracType == cAllegroScanTrac)
		{ //ScanTrac is an Allegro
			double TempBrightnessToChangeBy = TempTargetBrightness - TempImageReferenceValueToAdjustBy;
			ReportErrorMessage("AutoAdjustExposure TempBrightnessToChangeBy: " + dtoa(TempBrightnessToChangeBy,0), cWriteToLog, 0);

			if (TempDarkestPixel < 10)
				ReportErrorMessage("AutoAdjustExposure Darkest Pixel is very dark: " + dtoa(TempDarkestPixel,0), cWriteToLog, 0);
			if (TempDarkestPixel > 230)
				ReportErrorMessage("AutoAdjustExposure Darkest Pixel is very bright: " + dtoa(TempDarkestPixel,0), cWriteToLog, 0);

			ReportErrorMessage("AutoAdjustExposure-Image brightness is: " + dtoa(TempImageReferenceValueToAdjustBy, 2), cWriteToLog, 0);
			ReportErrorMessage("AutoAdjustExposure-Image brightness want: " + dtoa(TempTargetBrightness, 1), cWriteToLog, 0);
			ReportErrorMessage("AutoAdjustExposure-Original Voltage: " + dtoa(vLocalSystemData->vRampXRayVoltageAmount * 6.0, 4) + " Current: " + dtoa(vLocalSystemData->vRampXRayCurrentAmount, 4), cWriteToLog, 0);

			int TempAountNeedToAdjustInBrightness = (int)(TempTargetBrightness - TempImageReferenceValueToAdjustBy);

			if (TempImageReferenceValueToAdjustBy >= 240)
			{
				TempExposureWasAdjusted = true;
				if (vLocalSystemData->vRampXRayVoltageAmount > (30.0 / 6.0))
					vLocalSystemData->vRampXRayVoltageAmount = vLocalSystemData->vRampXRayVoltageAmount - (10.0 / 6.0); //reduce voltage by 10 KV
				else
				if (vLocalSystemData->vRampXRayVoltageAmount > (20.5 / 6.0))
					vLocalSystemData->vRampXRayVoltageAmount = 20.0 / 6.0;
				else
				{
					if (vLocalSystemData->vRampXRayCurrentAmount >.36)
						vLocalSystemData->vRampXRayCurrentAmount = vLocalSystemData->vRampXRayCurrentAmount - .2; //reduce voltage by 5 KV
					else
					if (vLocalSystemData->vRampXRayCurrentAmount > .165)
						vLocalSystemData->vRampXRayCurrentAmount = .165;
					else
						TempExposureWasAdjusted = false;
				}
			}
			else
			if (TempImageReferenceValueToAdjustBy <= 10)
			{
				TempExposureWasAdjusted = true;
				if (vLocalSystemData->vRampXRayVoltageAmount < ((float)((vLocalConfigurationData->vMaximumVoltage - 10) / 6)))
					vLocalSystemData->vRampXRayVoltageAmount = vLocalSystemData->vRampXRayVoltageAmount + (10.0 / 6.0); //increase voltage by 10 KV
				else
				if (vLocalSystemData->vRampXRayVoltageAmount < ((float)((vLocalConfigurationData->vMaximumVoltage - .5) / 6)))
					vLocalSystemData->vRampXRayVoltageAmount = (float)(vLocalConfigurationData->vMaximumVoltage / 6); //set to maximum amount
				else
				{
					if (vLocalSystemData->vRampXRayCurrentAmount < vLocalConfigurationData->vMaximumCurrent - 0.2)
						vLocalSystemData->vRampXRayCurrentAmount = vLocalSystemData->vRampXRayCurrentAmount + .2; //
					else
					if (vLocalSystemData->vRampXRayCurrentAmount < vLocalConfigurationData->vMaximumCurrent)
						vLocalSystemData->vRampXRayCurrentAmount = vLocalConfigurationData->vMaximumCurrent;
					else
						TempExposureWasAdjusted = false;
				}
			}
			else
			if (TempImageReferenceValueToAdjustBy >= 200)
			{
				TempExposureWasAdjusted = true;
				if (vLocalSystemData->vRampXRayVoltageAmount > (25.0 / 6.0))
					vLocalSystemData->vRampXRayVoltageAmount = vLocalSystemData->vRampXRayVoltageAmount - (5.0 / 6.0); //reduce voltage by 5 KV
				else
				if (vLocalSystemData->vRampXRayVoltageAmount > (20.5 / 6.0))
					vLocalSystemData->vRampXRayVoltageAmount = 20.0 / 6.0;
				else
				{
					if (vLocalSystemData->vRampXRayCurrentAmount >.3)
						vLocalSystemData->vRampXRayCurrentAmount = vLocalSystemData->vRampXRayCurrentAmount - .1; //reduce voltage by 5 KV
					else
					if (vLocalSystemData->vRampXRayCurrentAmount > .165)
						vLocalSystemData->vRampXRayCurrentAmount = .165;
					else
						TempExposureWasAdjusted = false;
				}
			}
			else
			if (TempImageReferenceValueToAdjustBy <= 20)
			{
				TempExposureWasAdjusted = true;
				if (vLocalSystemData->vRampXRayVoltageAmount < ((float)((vLocalConfigurationData->vMaximumVoltage - 5) / 6)))
					vLocalSystemData->vRampXRayVoltageAmount = vLocalSystemData->vRampXRayVoltageAmount + (5.0 / 6.0); //increase voltage by 10 KV
				else
				if (vLocalSystemData->vRampXRayVoltageAmount < ((float)((vLocalConfigurationData->vMaximumVoltage - .5) / 6)))
					vLocalSystemData->vRampXRayVoltageAmount = (float)(vLocalConfigurationData->vMaximumVoltage / 6); //set to maximum amount
				else
				{
					if (vLocalSystemData->vRampXRayCurrentAmount < vLocalConfigurationData->vMaximumCurrent - 0.1)
						vLocalSystemData->vRampXRayCurrentAmount = vLocalSystemData->vRampXRayCurrentAmount + .1; //
					else
					if (vLocalSystemData->vRampXRayCurrentAmount < vLocalConfigurationData->vMaximumCurrent)
						vLocalSystemData->vRampXRayCurrentAmount = vLocalConfigurationData->vMaximumCurrent;
					else
						TempExposureWasAdjusted = false;
				}
			}
			else
			if ((TempBrightnessToChangeBy > 5) || (TempBrightnessToChangeBy < -10))
			{ //target brightness not off by too much in Allegro
				if (TempBrightnessToChangeBy < 0)
				{
					double TempKVToAdjust = - TempBrightnessToChangeBy / 8.0;

					TempExposureWasAdjusted = true;
					if (vLocalSystemData->vRampXRayVoltageAmount > (20.0 + TempKVToAdjust) / 6.0)
						vLocalSystemData->vRampXRayVoltageAmount = vLocalSystemData->vRampXRayVoltageAmount - (TempKVToAdjust / 6.0); //reduce voltage by x KV
					else
					if (vLocalSystemData->vRampXRayVoltageAmount > (20.5 / 6.0))
						vLocalSystemData->vRampXRayVoltageAmount = 20.0 / 6.0;
					else
					{
						double TempmAToAdjust = TempBrightnessToChangeBy / 500.0;

						if (vLocalSystemData->vRampXRayCurrentAmount > .16 + TempmAToAdjust)
							vLocalSystemData->vRampXRayCurrentAmount = vLocalSystemData->vRampXRayCurrentAmount - TempmAToAdjust; //reduce voltage by 5 KV
						else
						if (vLocalSystemData->vRampXRayCurrentAmount > .165)
							vLocalSystemData->vRampXRayCurrentAmount = .165;
						else
							TempExposureWasAdjusted = false;
					}
				}
				else
				if (TempBrightnessToChangeBy > 5)
				{
					TempExposureWasAdjusted = true;
					double TempKVToAdjust = TempBrightnessToChangeBy / 8.0;

					if (vLocalSystemData->vRampXRayVoltageAmount < ((float)((vLocalConfigurationData->vMaximumVoltage - TempKVToAdjust) / 6)))
						vLocalSystemData->vRampXRayVoltageAmount = vLocalSystemData->vRampXRayVoltageAmount + (TempKVToAdjust / 6.0); //increase voltage by 10 KV
					else
					if (vLocalSystemData->vRampXRayVoltageAmount < ((float)((vLocalConfigurationData->vMaximumVoltage - .5) / 6)))
						vLocalSystemData->vRampXRayVoltageAmount = (float)(vLocalConfigurationData->vMaximumVoltage / 6); //set to maximum amount
					else
					{
						double TempmAToAdjust = TempBrightnessToChangeBy / 500.0;

						if (vLocalSystemData->vRampXRayCurrentAmount < vLocalConfigurationData->vMaximumCurrent - TempmAToAdjust)
							vLocalSystemData->vRampXRayCurrentAmount = vLocalSystemData->vRampXRayCurrentAmount + TempmAToAdjust;
						else
						if (vLocalSystemData->vRampXRayCurrentAmount < vLocalConfigurationData->vMaximumCurrent)
							vLocalSystemData->vRampXRayCurrentAmount = vLocalConfigurationData->vMaximumCurrent;
						else
							TempExposureWasAdjusted = false;
					}
				}
			}
			if (TempExposureWasAdjusted)
			{
				vLocalConfigurationData->LimitToMaximumVoltage(vLocalSystemData->vRampXRayVoltageAmount);
				vGlobalCurrentProduct->vXRaySourceVoltage = (float)vLocalSystemData->vRampXRayVoltageAmount;
				vGlobalCurrentProduct->vXRaySourceCurrent = (float)vLocalSystemData->vRampXRayCurrentAmount;
				vMainWindowPointer->SendSourceSettingsToUController(vLocalSystemData->vRampXRayVoltageAmount, vLocalSystemData->vRampXRayCurrentAmount);
				ReportErrorMessage("AutoAdjustExposure-New Voltage: " + dtoa(vLocalSystemData->vRampXRayVoltageAmount * 6.0, 4) + " Current: " + dtoa(vLocalSystemData->vRampXRayCurrentAmount, 4), cWriteToLog, 0);
			}
			else
				vExposureIsCorrect = true;
		}
		else 
		{ //ScanTrac is a Top Down
			if (TempDarkestPixel < 10)
				ReportErrorMessage("AutoAdjustExposure Darkest Pixel is very dark: " + dtoa(TempDarkestPixel,0), cWriteToLog, 0);
			if (TempDarkestPixel > 230)
				ReportErrorMessage("AutoAdjustExposure Darkest Pixel is very bright: " + dtoa(TempDarkestPixel,0), cWriteToLog, 0);

			ReportErrorMessage("AutoAdjustExposure-Image brightness is: " + dtoa(TempImageReferenceValueToAdjustBy, 2), cWriteToLog, 0);
			ReportErrorMessage("AutoAdjustExposure-Image brightness want: " + dtoa(TempTargetBrightness, 1), cWriteToLog, 0);

			int TempAountNeedToAdjustInBrightness = (int)(TempTargetBrightness - TempImageReferenceValueToAdjustBy);

			if (TempImageReferenceValueToAdjustBy >= 254)
			{
				if (vLocalSystemData->vRampXRayVoltageAmount > (30.0 / 6.0))
					vLocalSystemData->vRampXRayVoltageAmount = vLocalSystemData->vRampXRayVoltageAmount - (10.0 / 6.0); //reduce voltage by 10 KV
				else
					vLocalSystemData->vRampXRayVoltageAmount = 20.0 / 6.0;

				vLocalConfigurationData->LimitToMaximumVoltage(vLocalSystemData->vRampXRayVoltageAmount);

				TempExposureWasAdjusted = true;
			}
			else
			if (TempImageReferenceValueToAdjustBy <= 1)
			{
				vLocalSystemData->vRampXRayVoltageAmount = vLocalSystemData->vRampXRayVoltageAmount + (10.0 / 6.0); //increase voltage by 10 KV
				vLocalConfigurationData->LimitToMaximumVoltage(vLocalSystemData->vRampXRayVoltageAmount);

				TempExposureWasAdjusted = true;
			}
			else
			{
				BYTE TempCurrentVoltage = (BYTE)((vLocalSystemData->vRampXRayVoltageAmount * 6.0) + .5);  //make into KV and round off
				BYTE TempVoltageIndex = 0;

				if (TempCurrentVoltage <= 26)
					TempVoltageIndex = 0;
				else
				if (TempCurrentVoltage > 70)
					TempVoltageIndex = cNumberOfVoltages - 1;
				else
					TempVoltageIndex = TempCurrentVoltage - 26; //make 26 KV the first row in the table
				//look up voltage in table and find new desired voltage
				ReportErrorMessage("AutoAdjustExposure-Voltage Index: " + dtoa(TempVoltageIndex, 0), cWriteToLog, 0);

				BYTE TempBrightnessIndex = 0;

				BYTE TempBrightnessIndexToFollow = 0;
				for (BYTE TempLoop = 0; TempLoop < cNumberOfBrightnesses; TempLoop++)
				{
					if (TempIntensity[TempVoltageIndex][TempLoop] < TempImageReferenceValueToAdjustBy)
						TempLoop = cNumberOfBrightnesses;
					else
						TempBrightnessIndexToFollow = TempLoop;
				}
				//TempBrightnessIndexToFollow is the index below the darkest pixel
			
				ReportErrorMessage("AutoExposure, Preliminary TempBrightnessIndexToFollow: " + dtoa(TempBrightnessIndexToFollow, 0), cWriteToLog, 0);

				//if darkest pixel is closer to cell before TempBrightnessIndexToFollow then use that row
				if (TempBrightnessIndexToFollow)
				if (TempImageReferenceValueToAdjustBy > TempIntensity[TempVoltageIndex][TempBrightnessIndexToFollow])
				if (TempIntensity[TempVoltageIndex][TempBrightnessIndexToFollow - 1] > TempImageReferenceValueToAdjustBy)
				if ((TempImageReferenceValueToAdjustBy - TempIntensity[TempVoltageIndex][TempBrightnessIndexToFollow]) > (TempIntensity[TempVoltageIndex][TempBrightnessIndexToFollow - 1] - TempImageReferenceValueToAdjustBy))
					TempBrightnessIndexToFollow--;

				if (TempBrightnessIndexToFollow < cNumberOfBrightnesses - 1)
				if (TempImageReferenceValueToAdjustBy < TempIntensity[TempVoltageIndex][TempBrightnessIndexToFollow])
				if (TempIntensity[TempVoltageIndex][TempBrightnessIndexToFollow + 1] < TempImageReferenceValueToAdjustBy)
				if ((TempIntensity[TempVoltageIndex][TempBrightnessIndexToFollow] - TempImageReferenceValueToAdjustBy) > (TempImageReferenceValueToAdjustBy - TempIntensity[TempVoltageIndex][TempBrightnessIndexToFollow + 1]))
					TempBrightnessIndexToFollow++;

				ReportErrorMessage("AutoExposure, Final TempBrightnessIndexToFollow: " + dtoa(TempBrightnessIndexToFollow, 0), cWriteToLog, 0);

				bool TempScanning = false;
					
				if (vLocalConfigurationData->vScanTracType == cAtempoScanTrac)
					TempScanning = true;

				BYTE TempPreviousIndex = TempVoltageIndex;
				//scan down the row of the table looking for the closest value to the minimum pixel darkness
				while (TempScanning)
				{
					double TempBrightnessAbove = 255;
					TempPreviousIndex = TempVoltageIndex;

					if (TempVoltageIndex < cNumberOfVoltages - 1)
						TempBrightnessAbove = TempIntensity[TempVoltageIndex + 1][TempBrightnessIndexToFollow];

					double TempBrighnessBelow = 0;

					if (TempVoltageIndex > 0)
						TempBrighnessBelow = TempIntensity[TempVoltageIndex - 1][TempBrightnessIndexToFollow];

					if (TempTargetBrightness > TempBrightnessAbove)
					{
						if (TempVoltageIndex >= cNumberOfVoltages - 1)
							TempScanning = false;
						else
							TempVoltageIndex++;
					}
					else
					if (TempTargetBrightness < TempBrighnessBelow)
					{
						if (TempVoltageIndex == 0)
							TempScanning = false;
						else
							TempVoltageIndex--;
					}
					else
					{
						TempScanning = false;
					}
								
					//ReportErrorMessage("AutoExposure, Finished Scanning: " + dtoa(TempVoltageIndex, 0), cWriteToLog, 0);

					if (!TempScanning) //finished scanning, so see which voltage index is closest to brightness
					{
						if ((TempVoltageIndex == 0) && (TempTargetBrightness < TempIntensity[TempVoltageIndex][TempBrightnessIndexToFollow]))
						{ //TempVoltageIndex = 0 is closest pick, so use that
							ReportErrorMessage("Target brightness below lowest table brightness", cWriteToLog, 0);
						}
						else
						if ((TempVoltageIndex == cNumberOfVoltages - 1) && (TempTargetBrightness > TempIntensity[TempVoltageIndex][TempBrightnessIndexToFollow]))
						{ //TempVoltageIndex = 30 is closest pick, so use that
							ReportErrorMessage("Target brightness above highest table brightness", cWriteToLog, 0);
						}
						else
						{//find which voltage is closest to the brightness
							if (TempTargetBrightness > TempIntensity[TempVoltageIndex][TempBrightnessIndexToFollow])
							{
								if ((TempTargetBrightness - TempIntensity[TempVoltageIndex][TempBrightnessIndexToFollow]) > (TempIntensity[TempVoltageIndex + 1][TempBrightnessIndexToFollow] - TempTargetBrightness))
									TempVoltageIndex++;
							}
							else
							if (TempTargetBrightness < TempIntensity[TempVoltageIndex][TempBrightnessIndexToFollow])
							{
								if ((TempTargetBrightness - TempIntensity[TempVoltageIndex - 1][TempBrightnessIndexToFollow]) < (TempIntensity[TempVoltageIndex][TempBrightnessIndexToFollow] - TempTargetBrightness))
									TempVoltageIndex--;
							}
						}
					}
				} //end while scanning
				double TempRampXRayVoltageAmount = (TempVoltageIndex + 26) / 6.0; // set the voltage to the correct voltage from the best index
				vLocalConfigurationData->LimitToMaximumVoltage(TempRampXRayVoltageAmount);

				if ((vLocalSystemData->vRampXRayVoltageAmount + (2 / 6) < TempRampXRayVoltageAmount) || (vLocalSystemData->vRampXRayVoltageAmount - (2 / 6) > TempRampXRayVoltageAmount))
					TempExposureWasAdjusted = true;

				vLocalSystemData->vRampXRayVoltageAmount = TempRampXRayVoltageAmount;
				ReportErrorMessage("AutoExposure-Final Voltage Index: " + dtoa(TempVoltageIndex, 1) + ", Voltage: " + dtoa(vLocalSystemData->vRampXRayVoltageAmount * 6.0, 1), cWriteToLog, 0);

				if (TempCurrentVoltage == vLocalSystemData->vRampXRayVoltageAmount * 6.0)
				if ((TempAountNeedToAdjustInBrightness > 5) || (TempAountNeedToAdjustInBrightness < -5))
				{ //if the voltage did not change, but had significant change needed, make a change
					ReportErrorMessage("AutoExposure-Voltage did not change.  Need to adjust by: " + dtoa(TempAountNeedToAdjustInBrightness, 0), cWriteToLog, 0);
					if (vLocalSystemData->vRampXRayVoltageAmount < vLocalConfigurationData->vMaximumVoltage / 6.0)
					{
						vLocalSystemData->vRampXRayVoltageAmount = vLocalSystemData->vRampXRayVoltageAmount + (TempAountNeedToAdjustInBrightness / 20);
						vLocalConfigurationData->LimitToMaximumVoltage(vLocalSystemData->vRampXRayVoltageAmount);
					}
					else
					if (vLocalSystemData->vRampXRayCurrentAmount < vLocalConfigurationData->vMaximumCurrent)
					{
						vLocalSystemData->vRampXRayCurrentAmount = vLocalSystemData->vRampXRayCurrentAmount + (TempAountNeedToAdjustInBrightness / 20);
						vLocalConfigurationData->LimitToMaximumCurrent(vLocalSystemData->vRampXRayVoltageAmount, vLocalSystemData->vRampXRayCurrentAmount);
						vGlobalCurrentProduct->vXRaySourceCurrent = (float)vLocalSystemData->vRampXRayCurrentAmount;
					}
				}
			}
			vGlobalCurrentProduct->vXRaySourceVoltage = (float)vLocalSystemData->vRampXRayVoltageAmount;
			vGlobalCurrentProduct->vXRaySourceCurrent = (float)vLocalSystemData->vRampXRayCurrentAmount;
			vMainWindowPointer->SendSourceSettingsToUController(vLocalSystemData->vRampXRayVoltageAmount, vLocalSystemData->vRampXRayCurrentAmount);	
		}

		SetDlgItemText(IDC_SubFunction7Display, "V: " + dtoa(vGlobalCurrentProduct->vXRaySourceVoltage * 6.0, 1) + ", C: " + dtoa(vGlobalCurrentProduct->vXRaySourceCurrent, 2));
		if (vMode == cExposureMenu)
		{
			if (!TempExposureWasAdjusted)
			{
				SetStatusLine("Press Next.  Exposure is correct.", 2000, true);
				vExposureIsCorrect = true;
			}
			else
			{
				if (PasswordOK(cTemporaryInspxPassword,false))
				{
					//if (vLocalConfigurationData->vScanTracType == cSoloScanTrac)					
					//	SetStatusLine("Press NEXT.\nExposure was adjusted. " + dtoa(TempOriginalCurrent, 2) + ">" + dtoa(vLocalSystemData->vRampXRayCurrentAmount, 2) + " KV", 2000, false);
					//else
					//	SetStatusLine("Press NEXT.\nExposure was adjusted. " + dtoa(TempOriginalVoltage * 6.0, 0) + ">" + dtoa(vLocalSystemData->vRampXRayVoltageAmount * 6.0, 0) + " KV", 2000, false);
					if ((vGlobalScanTracType == cSoloScanTrac) || (vGlobalScanTracType == cSoloPlusScanTrac))
					{
						if (TempOriginalCurrent == vLocalSystemData->vRampXRayCurrentAmount)
							SetStatusLine("Send one more container.\nExposure was adjusted. " + dtoa(TempOriginalVoltage * 6.0, 1) + ">" + dtoa(vLocalSystemData->vRampXRayVoltageAmount * 6.0, 1) + " KV", 2000, false);
						else
							SetStatusLine("Send one more container.\nExposure was adjusted. " + dtoa(TempOriginalCurrent, 2) + ">" + dtoa(vLocalSystemData->vRampXRayCurrentAmount, 2) + " mA", 2000, false);
					}
					else
					{ //top down ScanTrac
						if (TempOriginalVoltage == vLocalSystemData->vRampXRayVoltageAmount)
							SetStatusLine("Exposure needs more adjusting.\nSend one more container. " + dtoa(TempOriginalCurrent, 2) + ">" + dtoa(vLocalSystemData->vRampXRayCurrentAmount, 2) + " mA", 2000, false);
						else
							SetStatusLine("Exposure needs more adjusting.\nSend one more container. " + dtoa(TempOriginalVoltage * 6.0, 1) + ">" + dtoa(vLocalSystemData->vRampXRayVoltageAmount * 6.0, 1) + " KV", 2000, false);
					}
				}
				else
				{
					SetStatusLine("Exposure needs more adjusting.\nSend one more container.", 2000, false);
				}
				ReportErrorMessage("AutoAdjustExposure-Image brightness is: " + dtoa(TempImageReferenceValueToAdjustBy, 2), cWriteToLog, 0);
				ReportErrorMessage("AutoAdjustExposure-Image brightness want: " + dtoa(TempTargetBrightness, 1), cWriteToLog, 0);
			}
			ReportErrorMessage("Finish AutoExposure V: " + dtoa(vGlobalCurrentProduct->vXRaySourceVoltage * 6.0, 1) + ", I: " + dtoa(vGlobalCurrentProduct->vXRaySourceCurrent, 2), cWriteToLog, 0);
		}
	}
	else
		ReportErrorMessage("Bad Pointer AutoExposure on entry", cEMailInspx, 0);
}

void CTDAutoSetupImageDisplayDialog::CalculateBestIntegrationTime()
{ //250 = .1 mSec
	if (vLocalSystemData->vSystemRunMode == cAutoSetupRunningSystem)
	if (vLocalSystemData->vActualEncoderRate) //can not calculate integration time from maximum encoder speed if encoder not moving
	{
		if (!vLocalConfigurationData->vUseRealEncoder)
		if (vLocalSystemData->vActualEncoderRate < 100)
		{
			vLocalSystemData->vActualEncoderRate = vLocalSystemData->vActualEncoderRate * 16;

			//if .4mm detectors, want to set to 32 times
			if (vLocalConfigurationData->vPixelsPerDetector == 128)
				vLocalSystemData->vActualEncoderRate = vLocalSystemData->vActualEncoderRate * 2;
		}
		CString TempOriginalIntegrationTime = dtoa(vGlobalCurrentProduct->vXRayIntegrationTime, 2); //save original integration time with two decimal places. 
		double TempMilliSecondsPerPulse = 10000 / vLocalSystemData->vActualEncoderRate; //10 time milliseconds, to count tenths of MSec
		TempMilliSecondsPerPulse = TempMilliSecondsPerPulse * 0.87;
		double TempMaximumMilliSecondsPerPulse = TempMilliSecondsPerPulse - 1.1; //10 times 0.11 which is the overhead to read out a line
		WORD TempIntegrationTime = (WORD)(TempMaximumMilliSecondsPerPulse * 250.0);

		if (TempIntegrationTime > 4500) //1.8 mSec Max
			TempIntegrationTime = 4500;
		if (TempIntegrationTime < 500) //.2 mSec Max
			TempIntegrationTime = 500;

		TempIntegrationTime = _wtoi(dtoa(TempIntegrationTime / 100, 0)) * 100; //round off to hundreds digit //5/29/2015 was tenths digit

		if (vGlobalCurrentProduct->vXRayIntegrationTime != TempIntegrationTime)
		{
			//CNoticeDialog TempNoticeDialog;
			//TempNoticeDialog.vNoticeText = "\n\nCalculated Integration Time: " + dtoa(TempIntegrationTime * .0004, 2) + "\nEncoder Rate is: " + dtoa(vLocalSystemData->vActualEncoderRate, 0) + 
			//	"\nOld Integration Time Was: " + dtoa(vGlobalCurrentProduct->vXRayIntegrationTime * .0004, 2);
			//TempNoticeDialog.vType = cErrorMessage;
			//TempNoticeDialog.DoModal();

			double TempScaleFactor = (double)TempIntegrationTime / (double)vGlobalCurrentProduct->vXRayIntegrationTime;

			ReportErrorMessage("Calculated Integration Time: " + dtoa(TempIntegrationTime * .0004, 2), cWriteToLog, 0);
			ReportErrorMessage("Encoder rate: " + dtoa(vLocalSystemData->vActualEncoderRate, 0) + "Hz, Original IT: " + dtoa(vGlobalCurrentProduct->vXRayIntegrationTime * .0004, 2), cWriteToLog, 0);

			if ((TempScaleFactor > 1.2) || (TempScaleFactor < 0.8))
			if ((vGlobalScanTracType == cSoloScanTrac) || (vGlobalScanTracType == cSoloPlusScanTrac))
			{
				vLocalSystemData->vRampXRayCurrentAmount = (float)(vGlobalCurrentProduct->vXRaySourceCurrent / TempScaleFactor);
				vLocalConfigurationData->LimitToMaximumCurrent(vLocalSystemData->vRampXRayVoltageAmount, vLocalSystemData->vRampXRayCurrentAmount);

				if (vLocalSystemData->vRampXRayCurrentAmount < .5)
					vLocalSystemData->vRampXRayCurrentAmount = .5;

				ReportErrorMessage("Adjust Current because Integration Time Changed : " + dtoa(vLocalSystemData->vRampXRayCurrentAmount, 2), cWriteToLog, 0);
				vGlobalCurrentProduct->vXRaySourceCurrent = (float)vLocalSystemData->vRampXRayCurrentAmount;
				vMainWindowPointer->SendSourceSettingsToUController(vLocalSystemData->vRampXRayVoltageAmount, vLocalSystemData->vRampXRayCurrentAmount);	
			}
			vGlobalCurrentProduct->vXRayIntegrationTime = TempIntegrationTime;

			vMainWindowPointer->SendProductIntegrationTime(vGlobalCurrentProduct->vXRayIntegrationTime);

			CString TempNewIntegrationTime = dtoa(vGlobalCurrentProduct->vXRayIntegrationTime, 2);
			if (TempNewIntegrationTime == TempOriginalIntegrationTime) //if same with two decimal places
			{
				vIntegrationTimeSet = true;
				vSamplingConveyorSpeed = false;
				//ReportErrorMessage("Integration Time Set Done : " + TempNewIntegrationTime + ", " + TempOriginalIntegrationTime, cWriteToLog, 0);
				//SetStatusLine("Send one container to AutoSize.", 0, false);
				//BYTE TempNumberOfDots = (vAdjustBrightnessFrameCount % 6);
				//CString TempDots = ".";
				//for (BYTE TempLoop = 0; TempLoop < TempNumberOfDots; TempLoop++)
				//	TempDots = TempDots + ".";
				//SetStatusLine("Adjusting Brightness\nPlease Wait.." + TempDots, 0, false);
				//ReportErrorMessage("Status Line--Adjusting Brightness to size, Stage 1, Please Wait...", cWriteToLog, 0);

				vGlobaluCSerialPort->SetRadiationDoorManualControl(cRadiationShutterOpen);

				ReportErrorMessage("Express Setup Start Background Brighness Below Adjust A, Voltage:" + dtoa(vGlobalCurrentProduct->vXRaySourceVoltage * 6.0, 1) + ", Current: " + 
					dtoa(vGlobalCurrentProduct->vXRaySourceCurrent, 2), cWriteToLog, 0);
			
				vAdjustingImageBrightnessBelowSaturation = true;


				if ((vLocalSystemData->vXRayOnNotice) && (!vStartingUpSource))
				{
					::PostThreadMessage(vGlobalCallingThreadID, cSimulateOneContainerIn10MSMessage, 1000, 0);

					ReportErrorMessage("Express Setup Simulate a container to adjust Brightness A", cWriteToLog, 0);
				}
				else 
				{
					int TimerResult = SetTimer(vSimulateContainerAfterXRaysAreOnTimerHandle,1000,NULL);
					if (!TimerResult)
						ReportErrorMessage("Error-vSimulateContainerAfterXRaysAreOn Timer Failed",cEMailInspx,32000);

					ReportErrorMessage("Express Setup Simulate a container to find brightness after X-Rays are on in 1 sec check", cWriteToLog, 0);
				}
			}
		}
		else
		{
			vIntegrationTimeSet = true;
			vSamplingConveyorSpeed = false;
			ReportErrorMessage("Integration Time OK : " + dtoa(vGlobalCurrentProduct->vXRayIntegrationTime * .0004, 2), cWriteToLog, 0);

			//BYTE TempNumberOfDots = (vAdjustBrightnessFrameCount % 6);
			//CString TempDots = ".";
			//for (BYTE TempLoop = 0; TempLoop < TempNumberOfDots; TempLoop++)
			//	TempDots = TempDots + ".";
			//SetStatusLine("Adjusting Brightness\nPlease Wait.." + TempDots, 0, false);
			//ReportErrorMessage("Status Line--Adjusting Brightness to size, Stage 1, Please Wait...", cWriteToLog, 0);

			vGlobaluCSerialPort->SetRadiationDoorManualControl(cRadiationShutterOpen);
			
			ReportErrorMessage("Express Setup Start Background Brighness Below Adjust B, Voltage:" + dtoa(vGlobalCurrentProduct->vXRaySourceVoltage * 6.0, 1) + ", Current: " + 
				dtoa(vGlobalCurrentProduct->vXRaySourceCurrent, 2), cWriteToLog, 0);
			vAdjustingImageBrightnessBelowSaturation = true;

			if ((vLocalSystemData->vXRayOnNotice) && (!vStartingUpSource))
			{
				::PostThreadMessage(vGlobalCallingThreadID, cSimulateOneContainerIn10MSMessage, 5000, 0);

				ReportErrorMessage("Express Setup Simulate a container to adjust Brightness B", cWriteToLog, 0);
			}
			else 
			{
				int TimerResult = SetTimer(vSimulateContainerAfterXRaysAreOnTimerHandle,1000,NULL);
				if (!TimerResult)
					ReportErrorMessage("Error-vSimulateContainerAfterXRaysAreOn Timer Failed",cEMailInspx,32000);

				ReportErrorMessage("Express Setup Simulate a container to find brightness after X-Rays are on in 1 sec check", cWriteToLog, 0);
			}
		}
	}
	else
	{
		if (vGaveTurnOnConveyorMessage)
			vGaveTurnOnConveyorMessage--;
		else
		{
			vGaveTurnOnConveyorMessage = 5;
			ReportErrorMessage("Conveyor Not Running, can not calculate Integration Time", cWriteToLog, 0);
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\n\nPlease Turn On Conveyor";
			TempNoticeDialog.vAutoClose = 2500;
			TempNoticeDialog.vType = cNoticeMessage;
			TempNoticeDialog.DoModal();
		}
	}
}

void CTDAutoSetupImageDisplayDialog::UpdateButtons()
{
	if (vMode == cTestEjectorsMenu)
	{
		if (vLocalSystemData->vTestEjectNextContainer)
		{ 
			SetDlgItemText(IDC_SubFunction5Button, _T("Abort Ejector Test"));
			SetDlgItemText(IDC_SubFunction6Button, _T("Abort Ejector Test"));
			SetDlgItemText(IDC_SubFunction7Button, _T("Abort Ejector Test"));
		}
		else
		{
			SetDlgItemText(IDC_SubFunction5Button, _T("Test Ejector\n1"));
			SetDlgItemText(IDC_SubFunction6Button, _T("Test Ejector\n2"));
			SetDlgItemText(IDC_SubFunction7Button, _T("Test Ejector\n3"));
		}
	}
}

void CTDAutoSetupImageDisplayDialog::TestEjectNextContainer(BYTE TempWaitCount, BYTE TempEjectorNumber)
{
	//eject next containter button pressed
	//if (vLocalConfigurationData->vEnableEjectors)
	{
		if ((vLocalSystemData->vSourceRampedUp) || (vLocalSystemData->vLastXRayCommandOn) || (vLocalSystemData->vSystemRunMode == cAutoSetupRunningSystem))
		{
			vLocalSystemData->vTestEjectWaitCount = TempWaitCount;
			vLocalSystemData->vTestEjectNextContainer = 1 << TempEjectorNumber;
			vOldTestEjectNextContainer = vLocalSystemData->vTestEjectNextContainer;
			UpdateButtons();
		}
		else
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\n\nSystem must be running to test ejectors.";
			TempNoticeDialog.vType = cErrorMessage;
			TempNoticeDialog.DoModal();
		}
	}
}



void CTDAutoSetupImageDisplayDialog::OnDialogtitlestatictext1()
{
	BYTE TempHiddenKeys = 8;
	if (!PasswordOK(cTemporaryInspxPassword,false))
		TempHiddenKeys = 10;
	PostThreadMessage(vGlobalCallingThreadID, cDisplaySoftKeyMenu, TempHiddenKeys, TempHiddenKeys);
}
