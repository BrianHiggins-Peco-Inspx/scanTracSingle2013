//ScanTrac Side View Source File
// DetectorSetupDialog.cpp : implementation file
//

#include "stdafx.h"
#include "ScanTrac.h"
#include "DetectorSetupDialog1.h"
#include "NumericEntryDialog.h"
#include "AlphaEntryDialog.h"
#include "SelectItemDialog.h"
#include "ConfigureEdgePixelAdjustDialog.h"
#include "SerialPort.h"
#include "YesNoDialog.h"
#include "XRayTubeAlignmentDialog.h"
#include "SelectMultipleItemDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDetectorSetupDialog dialog

extern CString vGlobalTopString;
extern CString vGlobalBottomString;
extern bool vGlobalFPGAVersion16point0orAbove;
extern BYTE vGlobalShiftKeyDown;
extern CSerialPort *vGlobaluCSerialPort;
extern CProduct *vGlobalCurrentProduct;
extern bool vGlobalFPGAVersion10Point0OrHigher;
extern bool vGlobalFPGAVersion18point0orAbove;
extern unsigned long vGlobalDialogTitleColor;

CDetectorSetupDialog::CDetectorSetupDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CDetectorSetupDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDetectorSetupDialog)
	//}}AFX_DATA_INIT

	vOldCalibrating = false;
	//vClockString = "";
	vPixelsPerDetector = 64;
	vDetectorLength = 2; //2 inches
	vMainWindowPointer = NULL;
	vChangedDetectorSettings = false;
	vPreAmplifierGain = 1;
	vChangeMade = false;
	vUpdateButtonTimerHandle = 31899;
	vOldXRayOnNotice = false;
	vLocalCWndCollection.Add(this);
	vLocalCWndCollection.Add(&m_DetectorMeasuredOffset12);
	vLocalCWndCollection.Add(&m_DetectorMeasuredOffset11);
	vLocalCWndCollection.Add(&m_DetectorMeasuredOffset10);
	vLocalCWndCollection.Add(&m_DetectorMeasuredOffset9);
	vLocalCWndCollection.Add(&m_DetectorMeasuredOffset8);
	vLocalCWndCollection.Add(&m_DetectorMeasuredOffset7);
	vLocalCWndCollection.Add(&m_DetectorOffset12);
	vLocalCWndCollection.Add(&m_DetectorOffset11);
	vLocalCWndCollection.Add(&m_DetectorOffset10);
	vLocalCWndCollection.Add(&m_DetectorOffset9);
	vLocalCWndCollection.Add(&m_DetectorOffset8);
	vLocalCWndCollection.Add(&m_DetectorOffset7);
	vLocalCWndCollection.Add(&m_DetectorMeasuredGain12);
	vLocalCWndCollection.Add(&m_DetectorMeasuredGain11);
	vLocalCWndCollection.Add(&m_DetectorMeasuredGain10);
	vLocalCWndCollection.Add(&m_DetectorMeasuredGain9);
	vLocalCWndCollection.Add(&m_DetectorMeasuredGain8);
	vLocalCWndCollection.Add(&m_DetectorMeasuredGain7);
	vLocalCWndCollection.Add(&m_DetectorGain12);
	vLocalCWndCollection.Add(&m_DetectorGain11);
	vLocalCWndCollection.Add(&m_DetectorGain10);
	vLocalCWndCollection.Add(&m_DetectorGain9);
	vLocalCWndCollection.Add(&m_DetectorGain8);
	vLocalCWndCollection.Add(&m_DetectorGain7);
	vLocalCWndCollection.Add(&m_Detector12);
	vLocalCWndCollection.Add(&m_Detector11);
	vLocalCWndCollection.Add(&m_Detector10);
	vLocalCWndCollection.Add(&m_Detector9);
	vLocalCWndCollection.Add(&m_Detector8);
	vLocalCWndCollection.Add(&m_Detector7);
	vLocalCWndCollection.Add(&m_SubFunction4Button);
	vLocalCWndCollection.Add(&m_ModeButton);
	vLocalCWndCollection.Add(&m_SubFunction8Button);
	vLocalCWndCollection.Add(&m_SubFunction9Button);
	vLocalCWndCollection.Add(&m_SubFunction7Button);
	vLocalCWndCollection.Add(&m_Function3Button);
	vLocalCWndCollection.Add(&m_ADCRegister1Label);
	vLocalCWndCollection.Add(&m_ADCRegister1);
	vLocalCWndCollection.Add(&m_ADCRegister0Label);
	vLocalCWndCollection.Add(&m_ADCRegister0);
	vLocalCWndCollection.Add(&m_SubFunction2Button);
	vLocalCWndCollection.Add(&m_Function4Button);
	vLocalCWndCollection.Add(&m_Function2Button);
	vLocalCWndCollection.Add(&m_XRaysOn2);
	vLocalCWndCollection.Add(&m_Background);
	vLocalCWndCollection.Add(&m_ADCOffsetButton);
	vLocalCWndCollection.Add(&m_ClockDividerDisplay);
	vLocalCWndCollection.Add(&m_DACOffsetsButton);
	vLocalCWndCollection.Add(&m_Detector1);
	vLocalCWndCollection.Add(&m_Detector2);
	vLocalCWndCollection.Add(&m_Detector3);
	vLocalCWndCollection.Add(&m_Detector4);
	vLocalCWndCollection.Add(&m_Detector5);
	vLocalCWndCollection.Add(&m_Detector6);
	vLocalCWndCollection.Add(&m_DetectorGain1);
	vLocalCWndCollection.Add(&m_DetectorGain2);
	vLocalCWndCollection.Add(&m_DetectorGain3);
	vLocalCWndCollection.Add(&m_DetectorGain4);
	vLocalCWndCollection.Add(&m_DetectorGain5);
	vLocalCWndCollection.Add(&m_DetectorGain6);
	vLocalCWndCollection.Add(&m_DetectorMeasuredGain1);
	vLocalCWndCollection.Add(&m_DetectorMeasuredGain2);
	vLocalCWndCollection.Add(&m_DetectorMeasuredGain3);
	vLocalCWndCollection.Add(&m_DetectorMeasuredGain4);
	vLocalCWndCollection.Add(&m_DetectorMeasuredGain5);
	vLocalCWndCollection.Add(&m_DetectorMeasuredGain6);
	vLocalCWndCollection.Add(&m_DetectorMeasuredOffset1);
	vLocalCWndCollection.Add(&m_DetectorMeasuredOffset2);
	vLocalCWndCollection.Add(&m_DetectorMeasuredOffset3);
	vLocalCWndCollection.Add(&m_DetectorMeasuredOffset4);
	vLocalCWndCollection.Add(&m_DetectorMeasuredOffset5);
	vLocalCWndCollection.Add(&m_DetectorMeasuredOffset6);
	vLocalCWndCollection.Add(&m_DetectorOffset1);
	vLocalCWndCollection.Add(&m_DetectorOffset2);
	vLocalCWndCollection.Add(&m_DetectorOffset3);
	vLocalCWndCollection.Add(&m_DetectorOffset4);
	vLocalCWndCollection.Add(&m_DetectorOffset5);
	vLocalCWndCollection.Add(&m_DetectorOffset6);
	vLocalCWndCollection.Add(&m_DialogTitleStaticText);
	vLocalCWndCollection.Add(&m_Function1Button);
	vLocalCWndCollection.Add(&m_Function5Button);
	vLocalCWndCollection.Add(&m_Function5Display);
	vLocalCWndCollection.Add(&m_NumberOfXRayDetectorsDisplay);
	vLocalCWndCollection.Add(&m_PreAmplifierGain);
	vLocalCWndCollection.Add(&m_SubFunction1Button);
	vLocalCWndCollection.Add(&m_SubFunction2Display);
	vLocalCWndCollection.Add(&m_SubFunction3Button);
	vLocalCWndCollection.Add(&m_SubFunction5Button);
	vLocalCWndCollection.Add(&m_SubFunction6Button);
	vLocalCWndCollection.Add(&m_TableLabel);
}

void CDetectorSetupDialog::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	// TODO: Add your message handler code here
	m_Background.SetWindowPos(NULL,cMainBackgroundLeft,
		cMainBackgroundTop,cMainBackgroundLength,
		cMainBackgroundHeight+5,SWP_NOZORDER);
		vSubFunction4ButtonEnable = true;

	vNumberOfDetectors = vLocalConfigurationData->vNumberOfXRayDetectors;

	for (BYTE TempLoop = 0; TempLoop < vLocalConfigurationData->vNumberOfXRayDetectors; TempLoop++)
	{
		vDetectorGain[TempLoop] = vLocalConfigurationData->vDetectorGain[TempLoop];
		vDACDetectorOffset[TempLoop] = vLocalSystemData->vDACDetectorOffset[TempLoop];
		vADCDetectorOffset[TempLoop] = vLocalSystemData->vADCDetectorOffset[TempLoop];
	}
	//vClockString = dtoa(vLocalConfigurationData->vDetectorClockDivider, 0);
	vPixelsPerDetector = vLocalConfigurationData->vPixelsPerDetector;
	vUseXScanDetectors = vLocalConfigurationData->vUseXScanDetectors;
	vDetectorLength = vLocalConfigurationData->vDetectorLength;

	if (vLocalSystemData->vXRayOnNotice)
		vSubFunction4ButtonEnable = false;	//m_SubFunction4Button.EnableWindow(false);

	/*
	vADRange13 = vLocalSystemData->vADRange13;
	vADInvert = vLocalSystemData->vADInvert;
	if (vLocalSystemData->vADRange13)
		SetDlgItemText(IDC_SubFunction1Button,"0 - 2 Volt A/D Range!");
	else
		SetDlgItemText(IDC_SubFunction1Button,"1 - 3 Volt A/D Range!");
	if (vLocalSystemData->vADInvert)
		SetDlgItemText(IDC_SubFunction2Button,"Don't Invert A/D Output!");
	else
		SetDlgItemText(IDC_SubFunction2Button,"Invert A/D Converter Output!");
	*/
	//char TempChars[5];
	//vClockString = dtoa(vLocalConfigurationData->vDetectorClockDivider,0);
	//SetDlgItemText(IDC_ClockDividerDisplay,vClockString);
	SetDlgItemText(IDC_NumberOfXRayDetectorsDisplay,dtoa(vNumberOfDetectors, 0));
			
	if ((vLocalSystemData->vSystemRunMode == cCalibratingDetectorOffsetMode) ||
		(vLocalSystemData->vSystemRunMode == cCalibratingDetectorGainMode) ||
		(vLocalSystemData->vSystemRunMode == cCalibratingDetectorEdgesMode))
	{
		vOldCalibrating = true;
		SetDlgItemText(IDC_SubFunction8Button, _T("Abort Auto Calibrate!"));
		SetDlgItemText(IDC_SubFunction7Button, _T("Abort Auto Calibrate!"));
		SetDlgItemText(IDC_SubFunction9Button, _T("Abort Calibrate!"));
		SetDlgItemText(IDC_Function3Button, _T("Abort Auto Calibrate!"));
	}
	else
	{
		vOldCalibrating = false;
		SetDlgItemText(IDC_SubFunction9Button, _T("Verify Pixels!"));
		SetDlgItemText(IDC_SubFunction8Button, _T("Calibrate Detector Pixels! (3)"));
		SetDlgItemText(IDC_SubFunction7Button, _T("Auto Find Detector Gain! (2)"));
		SetDlgItemText(IDC_Function3Button, _T("Auto Find Offset! (1)"));
	}

	vPreAmplifierGain = vLocalConfigurationData->vPreAmplifierGain;
	CString TempString("Gain of ");
	switch (vPreAmplifierGain)
	{
		case 0: TempString = TempString + "1";
		break;
		case 1: TempString = TempString + "2";
		break;
		case 2: TempString = TempString + "4";
		break;
		case 3: TempString = TempString + "8";
		break;
	}
	SetDlgItemText(IDC_PreAmplifierGain,TempString);

	UpdateDisplay();

	int TempTimerResult = SetTimer(vUpdateButtonTimerHandle,500,NULL);
	if (!TempTimerResult)
		ReportErrorMessage("Error-Update Timer Failed",cEMailInspx,32000);
	this->SetWindowText( _T("Detector"));
	::SetupMenu(vLocalCWndCollection);
}

void CDetectorSetupDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDetectorSetupDialog)
	DDX_Control(pDX, IDC_DetectorMeasuredOffset12, m_DetectorMeasuredOffset12);
	DDX_Control(pDX, IDC_DetectorMeasuredOffset11, m_DetectorMeasuredOffset11);
	DDX_Control(pDX, IDC_DetectorMeasuredOffset10, m_DetectorMeasuredOffset10);
	DDX_Control(pDX, IDC_DetectorMeasuredOffset9, m_DetectorMeasuredOffset9);
	DDX_Control(pDX, IDC_DetectorMeasuredOffset8, m_DetectorMeasuredOffset8);
	DDX_Control(pDX, IDC_DetectorMeasuredOffset7, m_DetectorMeasuredOffset7);
	DDX_Control(pDX, IDC_DetectorOffset12, m_DetectorOffset12);
	DDX_Control(pDX, IDC_DetectorOffset11, m_DetectorOffset11);
	DDX_Control(pDX, IDC_DetectorOffset10, m_DetectorOffset10);
	DDX_Control(pDX, IDC_DetectorOffset9, m_DetectorOffset9);
	DDX_Control(pDX, IDC_DetectorOffset8, m_DetectorOffset8);
	DDX_Control(pDX, IDC_DetectorOffset7, m_DetectorOffset7);
	DDX_Control(pDX, IDC_DetectorMeasuredGain12, m_DetectorMeasuredGain12);
	DDX_Control(pDX, IDC_DetectorMeasuredGain11, m_DetectorMeasuredGain11);
	DDX_Control(pDX, IDC_DetectorMeasuredGain10, m_DetectorMeasuredGain10);
	DDX_Control(pDX, IDC_DetectorMeasuredGain9, m_DetectorMeasuredGain9);
	DDX_Control(pDX, IDC_DetectorMeasuredGain8, m_DetectorMeasuredGain8);
	DDX_Control(pDX, IDC_DetectorMeasuredGain7, m_DetectorMeasuredGain7);
	DDX_Control(pDX, IDC_DetectorGain12, m_DetectorGain12);
	DDX_Control(pDX, IDC_DetectorGain11, m_DetectorGain11);
	DDX_Control(pDX, IDC_DetectorGain10, m_DetectorGain10);
	DDX_Control(pDX, IDC_DetectorGain9, m_DetectorGain9);
	DDX_Control(pDX, IDC_DetectorGain8, m_DetectorGain8);
	DDX_Control(pDX, IDC_DetectorGain7, m_DetectorGain7);
	DDX_Control(pDX, IDC_Detector12, m_Detector12);
	DDX_Control(pDX, IDC_Detector11, m_Detector11);
	DDX_Control(pDX, IDC_Detector10, m_Detector10);
	DDX_Control(pDX, IDC_Detector9, m_Detector9);
	DDX_Control(pDX, IDC_Detector8, m_Detector8);
	DDX_Control(pDX, IDC_Detector7, m_Detector7);
	DDX_Control(pDX, IDC_SubFunction4Button, m_SubFunction4Button);
	DDX_Control(pDX, IDC_ModeButton, m_ModeButton);
	DDX_Control(pDX, IDC_SubFunction8Button, m_SubFunction8Button);
	DDX_Control(pDX, IDC_SubFunction9Button, m_SubFunction9Button);
	DDX_Control(pDX, IDC_SubFunction7Button, m_SubFunction7Button);
	DDX_Control(pDX, IDC_Function3Button, m_Function3Button);
	DDX_Control(pDX, IDC_ADCRegister1Label, m_ADCRegister1Label);
	DDX_Control(pDX, IDC_ADCRegister1, m_ADCRegister1);
	DDX_Control(pDX, IDC_ADCRegister0Label, m_ADCRegister0Label);
	DDX_Control(pDX, IDC_ADCRegister0, m_ADCRegister0);
	DDX_Control(pDX, IDC_SubFunction2Button, m_SubFunction2Button);
	DDX_Control(pDX, IDC_Function4Button, m_Function4Button);
	DDX_Control(pDX, IDC_Function2Button, m_Function2Button);
	DDX_Control(pDX, IDC_XRaysOn2, m_XRaysOn2);
	DDX_Control(pDX, IDC_Background, m_Background);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_ADCOffsetButton, m_ADCOffsetButton);
	DDX_Control(pDX, IDC_SubFunction6Display, m_ClockDividerDisplay);
	DDX_Control(pDX, IDC_DACOffsetsButton, m_DACOffsetsButton);
	DDX_Control(pDX, IDC_Detector1, m_Detector1);
	DDX_Control(pDX, IDC_Detector2, m_Detector2);
	DDX_Control(pDX, IDC_Detector3, m_Detector3);
	DDX_Control(pDX, IDC_Detector4, m_Detector4);
	DDX_Control(pDX, IDC_Detector5, m_Detector5);
	DDX_Control(pDX, IDC_Detector6, m_Detector6);
	DDX_Control(pDX, IDC_DetectorGain1, m_DetectorGain1);
	DDX_Control(pDX, IDC_DetectorGain2, m_DetectorGain2);
	DDX_Control(pDX, IDC_DetectorGain3, m_DetectorGain3);
	DDX_Control(pDX, IDC_DetectorGain4, m_DetectorGain4);
	DDX_Control(pDX, IDC_DetectorGain5, m_DetectorGain5);
	DDX_Control(pDX, IDC_DetectorGain6, m_DetectorGain6);
	DDX_Control(pDX, IDC_DetectorMeasuredGain1, m_DetectorMeasuredGain1);
	DDX_Control(pDX, IDC_DetectorMeasuredGain2, m_DetectorMeasuredGain2);
	DDX_Control(pDX, IDC_DetectorMeasuredGain3, m_DetectorMeasuredGain3);
	DDX_Control(pDX, IDC_DetectorMeasuredGain4, m_DetectorMeasuredGain4);
	DDX_Control(pDX, IDC_DetectorMeasuredGain5, m_DetectorMeasuredGain5);
	DDX_Control(pDX, IDC_DetectorMeasuredGain6, m_DetectorMeasuredGain6);
	DDX_Control(pDX, IDC_DetectorMeasuredOffset1, m_DetectorMeasuredOffset1);
	DDX_Control(pDX, IDC_DetectorMeasuredOffset2, m_DetectorMeasuredOffset2);
	DDX_Control(pDX, IDC_DetectorMeasuredOffset3, m_DetectorMeasuredOffset3);
	DDX_Control(pDX, IDC_DetectorMeasuredOffset4, m_DetectorMeasuredOffset4);
	DDX_Control(pDX, IDC_DetectorMeasuredOffset5, m_DetectorMeasuredOffset5);
	DDX_Control(pDX, IDC_DetectorMeasuredOffset6, m_DetectorMeasuredOffset6);
	DDX_Control(pDX, IDC_DetectorOffset1, m_DetectorOffset1);
	DDX_Control(pDX, IDC_DetectorOffset2, m_DetectorOffset2);
	DDX_Control(pDX, IDC_DetectorOffset3, m_DetectorOffset3);
	DDX_Control(pDX, IDC_DetectorOffset4, m_DetectorOffset4);
	DDX_Control(pDX, IDC_DetectorOffset5, m_DetectorOffset5);
	DDX_Control(pDX, IDC_DetectorOffset6, m_DetectorOffset6);
	DDX_Control(pDX, IDC_DialogTitleStaticText1, m_DialogTitleStaticText);
	DDX_Control(pDX, IDC_Function1Button, m_Function1Button);
	DDX_Control(pDX, IDC_Function5Button, m_Function5Button);
	DDX_Control(pDX, IDC_Function5Display, m_Function5Display);
	DDX_Control(pDX, IDC_NumberOfXRayDetectorsDisplay, m_NumberOfXRayDetectorsDisplay);
	DDX_Control(pDX, IDC_PreAmplifierGain, m_PreAmplifierGain);
	DDX_Control(pDX, IDC_SubFunction1Button, m_SubFunction1Button);
	DDX_Control(pDX, IDC_SubFunction2Display, m_SubFunction2Display);
	DDX_Control(pDX, IDC_SubFunction3Button, m_SubFunction3Button);
	DDX_Control(pDX, IDC_SubFunction5Button, m_SubFunction5Button);
	DDX_Control(pDX, IDC_SubFunction6Button, m_SubFunction6Button);
	DDX_Control(pDX, IDC_TableLabel, m_TableLabel);
}


BEGIN_MESSAGE_MAP(CDetectorSetupDialog, CDialog)
	//{{AFX_MSG_MAP(CDetectorSetupDialog)
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
	ON_BN_CLICKED(IDC_SubFunction9Button, OnSubFunction9Button)
	ON_WM_CTLCOLOR()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_ModeButton, OnModeButton)
	ON_BN_CLICKED(IDC_ADCOffsetButton, OnADCOffsetButton)
	ON_BN_CLICKED(IDC_DACOffsetsButton, OnDACOffsetsButton)
	//}}AFX_MSG_MAP
	ON_STN_CLICKED(IDC_DialogTitleStaticText1, &CDetectorSetupDialog::OnDialogtitlestatictext1)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDetectorSetupDialog message handlers

void CDetectorSetupDialog::OnFunction1Button() 
{
	//Main Menu Button Pressed
	CDialog::EndDialog(10);
}

void CDetectorSetupDialog::OnFunction2Button() 
{
	//Save and exit was pressed
	if ((!vChangeMade) || (KeepSetupChanges()))
	{
		PrepareToExit();
		CDialog::EndDialog(true);
	}
}

void CDetectorSetupDialog::OnFunction3Button() 
{
	//calibrate detector offset button pressed
	if (vLocalSystemData->vSystemRunMode == cStoppedSystemMode)
	{
		BYTE TempWhichToCalibrate = 0;
		if ((vLocalConfigurationData->vUseXScanDetectors) && (vLocalConfigurationData->vScanTracType != cForteScanTrac) && (vLocalConfigurationData->vScanTracType != cCaseInspectorScanTrac))
			TempWhichToCalibrate = 1;

		if (vMainWindowPointer->InterlocksOK())
		{
			bool TempHavePixelAdjustments = false;
			for (WORD TempDetectorTypeLoop = 0; TempDetectorTypeLoop <= TempWhichToCalibrate; TempDetectorTypeLoop++)
			for (DWORD TempLoop = 0; TempLoop < cNumberOfPixelsToAdjustAllPixels1152; TempLoop++)
			{
				if (vLocalConfigurationData->vDetectorEdgeAdjustGain[TempLoop][TempDetectorTypeLoop])
					TempHavePixelAdjustments = true;
				if (vLocalConfigurationData->vDetectorEdgeAdjustOffset[TempLoop][TempDetectorTypeLoop])
					TempHavePixelAdjustments = true;
			}

			if (TempHavePixelAdjustments)
			{
				CYesNoDialog TempYesNoDialog;
				TempYesNoDialog.vNoticeText = _T("\n\n\nClear Pixel Linearization?");
				TempYesNoDialog.vYesButtonText = "Clear Adjustments";
				TempYesNoDialog.vNoButtonText = "Keep Adjustments";
				TempYesNoDialog.vQuestionType = cConfirmQuestion;
				int TempResult = TempYesNoDialog.DoModal();
				if (TempResult == IDOK)
				for (WORD TempDetectorTypeLoop = cPoint8mmDetectors; TempDetectorTypeLoop <= cPoint4mmDetectors; TempDetectorTypeLoop++)
				for (DWORD TempLoop = 0; TempLoop < cNumberOfPixelsToAdjustAllPixels1152; TempLoop++)
				{
					vLocalConfigurationData->vDetectorEdgeAdjustGain[TempLoop][TempDetectorTypeLoop] = 0;
					vLocalConfigurationData->vDetectorEdgeAdjustOffset[TempLoop][TempDetectorTypeLoop] = 0;
				}
			}

			bool TempAllGainsAt10 = true;
			for (BYTE TempLoop = 0; TempLoop < vLocalConfigurationData->vNumberOfXRayDetectors; TempLoop++)
			if (vDetectorGain[TempLoop] != 10)  
				TempAllGainsAt10 = false;

			if (!TempAllGainsAt10)
			{
				CYesNoDialog TempYesNoDialog;
				TempYesNoDialog.vNoticeText = _T("\n\n\nSet All Gains To 10?");
				TempYesNoDialog.vYesButtonText = "Set Gains";
				TempYesNoDialog.vNoButtonText = "Keep Gains";
				TempYesNoDialog.vQuestionType = cConfirmQuestion;
				int TempResult = TempYesNoDialog.DoModal();
				if (TempResult == IDOK)
				{
					BYTE TempNumberOfDetectorsToCalibrate = vLocalConfigurationData->vNumberOfXRayDetectors;
					if (vLocalConfigurationData->vNumberOfDetectorsToCalibrate)
						TempNumberOfDetectorsToCalibrate = vLocalConfigurationData->vNumberOfDetectorsToCalibrate;

					for (DWORD TempLoop = 0; TempLoop < TempNumberOfDetectorsToCalibrate; TempLoop++)
						vDetectorGain[TempLoop] = 10;  //set all gains at maximum

					//if (TempNumberOfDetectorsToCalibrate > 4)
					//	vLocalConfigurationData->vDetectorGain[4] = 9;
					//if (TempNumberOfDetectorsToCalibrate > 5)
					//	vLocalConfigurationData->vDetectorGain[5] = 10;
					//vMainWindowPointer->SendADCGainsAndOffsets(false);
				}
			}
			PrepareToExit();

			vLocalSystemData->vFullDetectorCalibration = false;
			vMainWindowPointer->LoadCalibrateOffsetProduct();
			vLocalSystemData->vCalibrateErrorCount = 0;
			vLocalSystemData->vModeToRunAfterCalibrateOffset = cStoppedSystemMode;
			ReportErrorMessage("Just Detector Offset Calibration Started", cWriteToLog,0);
			vLocalSystemData->vCalibrateADCAfterDAC = true;
			vLocalSystemData->vAutoCalibrate = false;
			vLocalSystemData->vAutoCalibrateWhileRunning = false;
			if (vLocalConfigurationData->vUseDACAndADCinDensityCompensation)
				vMainWindowPointer->CalibrateDACDetectorOffset();
			else
				vMainWindowPointer->CalibrateADCDetectorOffset(vLocalSystemData->vAutoCalibrate, false);
			vOldCalibrating = true;
			Sleep(1); //is the equivelent of a yeild statement;
			CDialog::EndDialog(10);
		}
		else
		{
			CNoticeDialog TempNoticeDialog;
			CString TempText("\n\n\nX-Rays not on, Check Key Switch, E-Stop, Interlocks");
			if (PasswordOK(cSuperInspxPassword,false))
				TempText = TempText + "\nInterlocks State: " + WordToHex(vLocalSystemData->vSourceInterLocks);
			TempNoticeDialog.vNoticeText = TempText;
			TempNoticeDialog.vType = cNoticeMessage;
			TempNoticeDialog.DoModal();
		}
	}
	else
	if ((vLocalSystemData->vSystemRunMode == cCalibratingDetectorEdgesMode) ||
		(vLocalSystemData->vSystemRunMode == cCalibratingDetectorGainMode) ||
		(vLocalSystemData->vSystemRunMode == cCalibratingDetectorOffsetMode))
	{
		vMainWindowPointer->StopRunning(true);
	}
	else
	{
		CNoticeDialog TempNoticeDialog;
		CString TempText("\n\n\nSystem must be stopped to run this function");
		TempNoticeDialog.vNoticeText = TempText;
		TempNoticeDialog.vType = cNoticeMessage;
		TempNoticeDialog.DoModal();
	}
}

void CDetectorSetupDialog::OnFunction4Button() 
{
	//Cancel was pressed
	if (vChangeMade)
	if ((!vChangeMade) || (AbortSetupChanges()))
		CDialog::OnCancel();
}

void CDetectorSetupDialog::OnFunction5Button() 
{
	//configure edge pixel compensation button pressed
  //Create dialog box
  CConfigureEdgePixelAdjustDialog IConfigureEdgePixelAdjustDialog;  

  //Set dialog box data like how many products there are
	IConfigureEdgePixelAdjustDialog.vLocalConfigurationData = vLocalConfigurationData;
	IConfigureEdgePixelAdjustDialog.vLocalSystemData = vLocalSystemData;

	if ((vUseXScanDetectors) && (vLocalConfigurationData->vScanTracType != cForteScanTrac) && (vLocalConfigurationData->vScanTracType != cCaseInspectorScanTrac))
	{
		CYesNoDialog TempYesNoDialog;
		TempYesNoDialog.vNoticeText = _T("\n\nAdjust Calibrations for .8 or .4 mm Pixel Mode?");
		TempYesNoDialog.vYesButtonText = ".8 mm Mode";
		TempYesNoDialog.vNoButtonText = ".4 mm Mode";
		TempYesNoDialog.vQuestionType = cConfirmQuestion;
		int TempResult = TempYesNoDialog.DoModal();
		if (TempResult != IDOK)
		{
			IConfigureEdgePixelAdjustDialog.vUsePoint4mmXScanDetectors = 1;  //set to 2 so multiplies the number of pixels per detector by 2 everywhere to convert 64 to 128 pixels per detector
			IConfigureEdgePixelAdjustDialog.vUsePoint4mmXScanDetectorsMultiplier = 2;  //set to 2 so multiplies the number of pixels per detector by 2 everywhere to convert 64 to 128 pixels per detector
		}
	}

	//Pass control to dialog box and display
  int nResponse = IConfigureEdgePixelAdjustDialog.DoModal();
  //dialog box is now closed, if user pressed select do this
  //if user pressed cancel, do nothing
  if (nResponse == IDOK)
  {
	  // TODO: Place code here to handle when the dialog is closed
		UpdateDisplay();
  }
	else if (nResponse == 10)
	{
		Sleep(1); //is the equivelent of a yeild statement;
		CDialog::EndDialog(10);
	}
}

void CDetectorSetupDialog::OnSubFunction1Button() 
{
	//set preamplifier gain
	CSelectItemDialog ISelectItemDialog;  
	
	//Set dialog box data titles and number value
	ISelectItemDialog.vTitleString = "Preamplifier Gain for All Detectors";
	ISelectItemDialog.vNumberOfItems = 4;
	ISelectItemDialog.vBackgroundColor = cGreen;

	ISelectItemDialog.vItems[0] = "Gain of 1";
	ISelectItemDialog.vItems[1] = "Gain of 2";
	ISelectItemDialog.vItems[2] = "Gain of 4";
	ISelectItemDialog.vItems[3] = "Gain of 8";
	ISelectItemDialog.m_DialogTitleStaticText2 = "Setup System";
	ISelectItemDialog.m_DialogTitleStaticText3 = "Original Value: " + ISelectItemDialog.vItems[vPreAmplifierGain];
	if (vGlobalFPGAVersion10Point0OrHigher)
		ISelectItemDialog.m_DialogTitleStaticText3 = ISelectItemDialog.m_DialogTitleStaticText3 + ", Default = 2";
	else
		ISelectItemDialog.m_DialogTitleStaticText3 = ISelectItemDialog.m_DialogTitleStaticText3 + ", Default = 4";
	//Pass control to dialog box and display
	int nResponse = ISelectItemDialog.DoModal();
	//dialog box is now closed, if user pressed select do this
	//if user pressed cancel, do nothing
	if (nResponse == IDOK)
	{
		vPreAmplifierGain = ISelectItemDialog.vItemNumberSelected - 1;
		SetDlgItemText(IDC_PreAmplifierGain,ISelectItemDialog.vItems[vPreAmplifierGain]);
		UpdateDisplay();
		SetChangeMade();
	}
	else 
	if (nResponse == 10)
	{
		//Main Menu button pressed
		Sleep(1); //is the equivelent of a yeild statement;
		CDialog::EndDialog(10);
	}
}

void CDetectorSetupDialog::OnSubFunction2Button() 
{
	//X-Ray Detector Pixel Size
	if (vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac)
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\n\nCase Inspector Only Supports 1.6mm Detectors.";
		if (vPixelsPerDetector != 64)
			TempNoticeDialog.vNoticeText = TempNoticeDialog.vNoticeText + "Pixels Per Detector was: " + dtoa(vPixelsPerDetector, 0);
		if (vDetectorLength != 4)
			TempNoticeDialog.vNoticeText = TempNoticeDialog.vNoticeText + "Detector Length was: " + dtoa(vDetectorLength, 0);

		TempNoticeDialog.vType = cNoticeMessage;
		TempNoticeDialog.DoModal();

		vPixelsPerDetector = 64;
		vDetectorLength = 4;
	}
	else
	if ((!vGlobalFPGAVersion16point0orAbove) || (vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac))
	{
		vUseXScanDetectors = 0;
		vMainWindowPointer->ClearXScanProducts();

		vChangedDetectorSettings = true;
		if (vPixelsPerDetector == 64)
			vPixelsPerDetector = 128;
		else
			vPixelsPerDetector = 64;
		UpdateDisplay();
		vLocalSystemData->vCurrentProductChanged = true;
		SetChangeMade();
	}
	else
	{ //XScan Detectors possible in system, so ask which
		//X-Ray Detector Pixel Size
		CSelectItemDialog ISelectItemDialog;  
	
		//Set dialog box data titles and number value
		ISelectItemDialog.vTitleString = "X-Ray Detector Array Pixel Size";
		ISelectItemDialog.vNumberOfItems = 3;
		ISelectItemDialog.vBackgroundColor = cGreen;

		ISelectItemDialog.m_DialogTitleStaticText2 = "Setup System";
		ISelectItemDialog.vItems[0] = "DT  .4 mm Pixels";
		if  (vLocalConfigurationData->vScanTracType == cForteScanTrac)
			ISelectItemDialog.vItems[0] = "DT  .4 mm Pixels - Not Supported On Forte";

		ISelectItemDialog.vItems[1] = "DT  .8 mm Pixels";
		if  ((vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac))
			ISelectItemDialog.vItems[2] = "X-Scan";
		else
			ISelectItemDialog.vItems[2] = "X-Scan  .4 mm and .8mm Pixels (Product Selected)";
		//ISelectItemDialog.vItems[3] = "X-Scan  .8 mm Pixels";
		//ISelectItemDialog.vItems[2] = "1.6 mm Pixels";

		if ((vUseXScanDetectors) && (vLocalConfigurationData->vScanTracType != cForteScanTrac) && (vLocalConfigurationData->vScanTracType != cCaseInspectorScanTrac))
			ISelectItemDialog.vGreenItem = 3;
		else
		if (vPixelsPerDetector == 128)
			ISelectItemDialog.vGreenItem = 1;
		else
			ISelectItemDialog.vGreenItem = 2;
			
		CString TempDetectorType = "";
		if (vGlobalFPGAVersion16point0orAbove)
		{
			if (vUseXScanDetectors)
			{
				if  ((vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac))
					TempDetectorType = "X-Scan";
				else
				{
					TempDetectorType = "X-Scan both .4mm and .8mm mode";
					ISelectItemDialog.vGreenItem = ISelectItemDialog.vGreenItem + 2;
				}
			}
			else
			{
				TempDetectorType = "DT ";
				if (vPixelsPerDetector == 128)
					TempDetectorType = TempDetectorType + ".4mm Pixels";
				else
					TempDetectorType = TempDetectorType + ".8mm Pixels";
			}
		}

		ISelectItemDialog.m_DialogTitleStaticText3 = "Default Value: .8, Original Value: " + TempDetectorType;
		//Pass control to dialog box and display
		int nResponse = ISelectItemDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			if ((vLocalConfigurationData->vScanTracType == cForteScanTrac) && (ISelectItemDialog.vItemNumberSelected == 1) && (vGlobalFPGAVersion16point0orAbove))
				ISelectItemDialog.vItemNumberSelected = 2;

			vChangedDetectorSettings = true;
			if ((ISelectItemDialog.vItemNumberSelected == 1) || (ISelectItemDialog.vItemNumberSelected == 3))
				vPixelsPerDetector = 128;
			else
				vPixelsPerDetector = 64;

			if ((ISelectItemDialog.vItemNumberSelected == 1) || (ISelectItemDialog.vItemNumberSelected == 2))
				vUseXScanDetectors = 0;
			else
				vUseXScanDetectors = 1;

			UpdateDisplay();
			vLocalSystemData->vCurrentProductChanged = true;
			SetChangeMade();
		}
		else 
		if (nResponse == 10)
		{
			//Main Menu button pressed
			Sleep(1); //is the equivelent of a yeild statement;
			CDialog::EndDialog(10);
		}
	}
}

void CDetectorSetupDialog::OnSubFunction3Button() 
{
	//A/D Gain Button Pressed
	CNumericEntryDialog INumericEntryDialog;  
	for (BYTE TempLoop = 0; TempLoop < vNumberOfDetectors; TempLoop++)
	{
		//Set dialog box data titles and number value
		INumericEntryDialog.vEditString = dtoa(vDetectorGain[TempLoop],2);
		INumericEntryDialog.m_DialogTitleStaticText1 = "Analog to Digital Converter Gain";
		INumericEntryDialog.m_DialogTitleStaticText2 = "For Channel " + dtoa(TempLoop + 1);
		INumericEntryDialog.m_DialogTitleStaticText3 = "";
		INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + INumericEntryDialog.vEditString;

		INumericEntryDialog.m_UnitsString = "Gain from 1 to 10";
		INumericEntryDialog.vMaxValue = 10; //10 bit number
		INumericEntryDialog.vMinValue = 1; //10 bit number
		INumericEntryDialog.vIntegerOnly = false;
		//Pass control to dialog box and display
		int nResponse = INumericEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			vDetectorGain[TempLoop] = ATOF(INumericEntryDialog.vEditString);
			if (vDetectorGain[TempLoop] < 1)
				vDetectorGain[TempLoop] = 1;
			//vChangedDetectorSettings = true;
			SetChangeMade();
		}
		else 
		if (nResponse == 10)
		{
			//Main Menu button pressed
			TempLoop = vNumberOfDetectors;
			Sleep(1); //is the equivelent of a yeild statement;
			CDialog::EndDialog(10);
		}
		else
			TempLoop = vNumberOfDetectors;
	}
	UpdateDisplay();
}

void CDetectorSetupDialog::OnSubFunction4Button() 
{
  //Create dialog box
	if (vLocalSystemData->vXRayOnNotice)
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\n\nYou Can Not Align Tube When ScanTrac Is Running.";
		TempNoticeDialog.vType = cNoticeMessage;
		TempNoticeDialog.DoModal();
	}
	else
	{
		CXRayTubeAlignmentDialog IXRayTubeAlignmentDialog;  

		//Set dialog box data like how many products there are
		IXRayTubeAlignmentDialog.vLocalConfigurationData = vLocalConfigurationData;
		IXRayTubeAlignmentDialog.vLocalSystemData = vLocalSystemData;
		IXRayTubeAlignmentDialog.vMainWindowPointer = vMainWindowPointer;;

		//Pass control to dialog box and display
		int nResponse = IXRayTubeAlignmentDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		//if (nResponse == IDOK)
		//{
			// TODO: Place code here to handle when the dialog is closed
		//}
		//else 
		if (nResponse == 10)
		{
			Sleep(1); //is the equivelent of a yeild statement;
			CDialog::EndDialog(10);
		}
	}
}

void CDetectorSetupDialog::OnSubFunction5Button() 
{
	CYesNoDialog TempYesNoDialog;
	TempYesNoDialog.vNoticeText = _T("\n\nX-Ray Detector settings changed\nClear X-Ray Source Strength History?");
	TempYesNoDialog.vYesButtonText = "Clear History";
	TempYesNoDialog.vNoButtonText = "Keep History";
	TempYesNoDialog.vQuestionType = cConfirmQuestion;
	int TempResult = TempYesNoDialog.DoModal();
	if (TempResult == IDOK)
	{
		CSelectItemDialog ISelectItemDialog;  
	
		//Set dialog box data titles and number value
		ISelectItemDialog.vTitleString = "Are all X-Ray Detectors New?";
		ISelectItemDialog.vNumberOfItems = 2;
		ISelectItemDialog.vBackgroundColor = cGreen;

		ISelectItemDialog.vItems[0] = "Yes, All Detectors are New";
		ISelectItemDialog.vItems[1] = "No, Some Detectors are Used";
		ISelectItemDialog.m_DialogTitleStaticText2 = "If all detectors are new the next Source Strength";
		CString TempString = dtoa(vNumberOfDetectors, 0);
		ISelectItemDialog.m_DialogTitleStaticText3 = " Measurement will be saved as the 100% reference strength";
		//Pass control to dialog box and display
		int nResponse = ISelectItemDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			if (ISelectItemDialog.vItemNumberSelected == 1)
				vLocalConfigurationData->vSaveNextSourceStrengthValuesAsInitial = true;

			for (WORD TempLoop = 0; TempLoop < cMaximumNumberOfSourceStrengthValues21; TempLoop++)
				vLocalConfigurationData->vSourceStrengthInitialValues[TempLoop] = 0;


			vLocalConfigurationData->vSourceStrengthCount = 0;
			vChangedDetectorSettings = false;
		}
	}

	/*
	//Number of X-Ray Detectors
	CSelectItemDialog ISelectItemDialog;  
	
	//Set dialog box data titles and number value
	ISelectItemDialog.vTitleString = "Number of X-Ray Detectors in System";
	ISelectItemDialog.vNumberOfItems = 2;
	ISelectItemDialog.vBackgroundColor = cGreen;

	ISelectItemDialog.vItems[0] = "3 Detectors (6 inches high)";
	ISelectItemDialog.vItems[1] = "6 Detectors (12 inches high)";
	ISelectItemDialog.m_DialogTitleStaticText2 = "Setup System";
	CString TempString = dtoa(vNumberOfDetectors, 0);
	ISelectItemDialog.m_DialogTitleStaticText3 = "Original Value: " + 
		TempString;
	//Pass control to dialog box and display
	int nResponse = ISelectItemDialog.DoModal();
	//dialog box is now closed, if user pressed select do this
	//if user pressed cancel, do nothing
	if (nResponse == IDOK)
	{
		bool TempOK = true;
		if ((ISelectItemDialog.vItemNumberSelected == 1) && (vNumberOfDetectors == 6))
		{
			TempOK = false;
			CYesNoDialog TempYesNoDialog;
			TempYesNoDialog.vNoticeText = "You are reducing your detector height\nAll setups taller than the new height\nwill have to be done over.\nReduce Height?";
			TempYesNoDialog.vQuestionType = cConfirmQuestion;
			int TempResult = TempYesNoDialog.DoModal();
			if (TempResult == IDOK)
				TempOK = true;
		}
		if (TempOK)
		{
			vChangedDetectorSettings = true;
			if (ISelectItemDialog.vItemNumberSelected == 2)
				vNumberOfDetectors = 6; //6 detectors
			else
				vNumberOfDetectors = 3; // detectors
			SetDlgItemText(IDC_NumberOfXRayDetectorsDisplay,dtoa(vNumberOfDetectors,0));
			UpdateDisplay();
			SetChangeMade();
			vLocalSystemData->vCurrentProductChanged = true;
		}
	}
	else 
	if (nResponse == 10)
	{
		//Main Menu button pressed
		Sleep(1); //is the equivelent of a yeild statement;
		CDialog::EndDialog(10);
	}
	*/
}

void CDetectorSetupDialog::OnSubFunction6Button() 
{
	bool vDetectorEnables[cMaximumNumberOfDetectors12];

	for (BYTE TempLoop = 0; TempLoop < cMaximumNumberOfDetectors12; TempLoop++)
		vDetectorEnables[TempLoop] = true;

	if (vLocalConfigurationData->vNumberOfDetectorsToCalibrate)
	for (BYTE TempLoop = 0; TempLoop < vLocalConfigurationData->vNumberOfXRayDetectors - vLocalConfigurationData->vNumberOfDetectorsToCalibrate; TempLoop++)
		vDetectorEnables[TempLoop] = false;

	if (vLocalConfigurationData->vDetectorsAtBottomToIgnoreDuringCalibration)
	for (BYTE TempLoop = vLocalConfigurationData->vNumberOfXRayDetectors - 1; 
		TempLoop >= vLocalConfigurationData->vNumberOfXRayDetectors - vLocalConfigurationData->vDetectorsAtBottomToIgnoreDuringCalibration; TempLoop--)
		vDetectorEnables[TempLoop] = false;

	CSelectMultipleItemDialog ISelectItemDialog;  
			
	//Set dialog box data titles and number value
	ISelectItemDialog.m_DialogTitleStaticText = "Disable Detectors For Calibration";
	ISelectItemDialog.vNumberOfItems = vLocalConfigurationData->vNumberOfXRayDetectors;
	ISelectItemDialog.vOutboardItemMustBeDisabled = true;
	//ISelectItemDialog.vColor = cGreen;

	for (BYTE TempLoop = 0; TempLoop < vLocalConfigurationData->vNumberOfXRayDetectors; TempLoop++)
	{
		BYTE TempDetector = vLocalConfigurationData->vNumberOfXRayDetectors - 1 - TempLoop;
		ISelectItemDialog.vItems[TempLoop] = vDetectorEnables[TempLoop];
		ISelectItemDialog.vItemsYes[TempLoop] = "Enable Detector " + dtoa(TempDetector + 1, 0) + " Calibration";
		ISelectItemDialog.vItemsNo[TempLoop] = "Disable Detector " + dtoa(TempDetector + 1, 0) + " Calibration";
		if (TempLoop == vLocalConfigurationData->vNumberOfXRayDetectors - 1)
		{
			ISelectItemDialog.vItemsYes[TempLoop] = ISelectItemDialog.vItemsYes[TempLoop] + " (" + vGlobalBottomString + ")";
			ISelectItemDialog.vItemsNo[TempLoop] = ISelectItemDialog.vItemsNo[TempLoop] + " (" + vGlobalBottomString + ")";
		}
		if (TempLoop == 0)
		{
			ISelectItemDialog.vItemsYes[TempLoop] = ISelectItemDialog.vItemsYes[TempLoop] + " (" + vGlobalTopString + ")";
			ISelectItemDialog.vItemsNo[TempLoop] = ISelectItemDialog.vItemsNo[TempLoop] + " (" + vGlobalTopString + ")";
		}
	}

	//Pass control to dialog box and display
	int nResponse = ISelectItemDialog.DoModal();
	//dialog box is now closed, if user pressed select do this
	//if user pressed cancel, do nothing
	if (nResponse == IDOK)
	{
		if (ISelectItemDialog.vItems[0])
			vLocalConfigurationData->vNumberOfDetectorsToCalibrate = 0;
		else
		{
			vLocalConfigurationData->vNumberOfDetectorsToCalibrate = vLocalConfigurationData->vNumberOfXRayDetectors - 1;

			if (!ISelectItemDialog.vItems[1])
				vLocalConfigurationData->vNumberOfDetectorsToCalibrate = vLocalConfigurationData->vNumberOfXRayDetectors - 2;

			if (!ISelectItemDialog.vItems[2])
				vLocalConfigurationData->vNumberOfDetectorsToCalibrate = vLocalConfigurationData->vNumberOfXRayDetectors - 3;

			if ((vLocalConfigurationData->vNumberOfXRayDetectors > 7) && (!ISelectItemDialog.vItems[3]))
				vLocalConfigurationData->vNumberOfDetectorsToCalibrate = vLocalConfigurationData->vNumberOfXRayDetectors - 4;

			if ((vLocalConfigurationData->vNumberOfXRayDetectors > 9) && (!ISelectItemDialog.vItems[4]))
				vLocalConfigurationData->vNumberOfDetectorsToCalibrate = vLocalConfigurationData->vNumberOfXRayDetectors - 5;

			if ((vLocalConfigurationData->vNumberOfXRayDetectors > 11) && (!ISelectItemDialog.vItems[5]))
				vLocalConfigurationData->vNumberOfDetectorsToCalibrate = vLocalConfigurationData->vNumberOfXRayDetectors - 6;
		}

		if (ISelectItemDialog.vItems[vLocalConfigurationData->vNumberOfXRayDetectors - 1])
			vLocalConfigurationData->vDetectorsAtBottomToIgnoreDuringCalibration = 0;
		else
		{
			vLocalConfigurationData->vDetectorsAtBottomToIgnoreDuringCalibration = 1;

			if (!ISelectItemDialog.vItems[vLocalConfigurationData->vNumberOfXRayDetectors - 2])
				vLocalConfigurationData->vDetectorsAtBottomToIgnoreDuringCalibration = 2;

			if (!ISelectItemDialog.vItems[vLocalConfigurationData->vNumberOfXRayDetectors - 3])
				vLocalConfigurationData->vDetectorsAtBottomToIgnoreDuringCalibration = 3;

			if ((vLocalConfigurationData->vNumberOfXRayDetectors > 7) && (!ISelectItemDialog.vItems[vLocalConfigurationData->vNumberOfXRayDetectors - 4]))
				vLocalConfigurationData->vDetectorsAtBottomToIgnoreDuringCalibration = 4;

			if ((vLocalConfigurationData->vNumberOfXRayDetectors > 9) && (!ISelectItemDialog.vItems[vLocalConfigurationData->vNumberOfXRayDetectors - 5]))
				vLocalConfigurationData->vDetectorsAtBottomToIgnoreDuringCalibration = 5;

			if ((vLocalConfigurationData->vNumberOfXRayDetectors > 11) && (!ISelectItemDialog.vItems[vLocalConfigurationData->vNumberOfXRayDetectors - 6]))
				vLocalConfigurationData->vDetectorsAtBottomToIgnoreDuringCalibration = 6;
		}
		
		UpdateDisplay();
	}
	else 
	if (nResponse == 10)
	{
		//Main Menu button pressed
		Sleep(1); //is the equivelent of a yeild statement;
		CDialog::EndDialog(10);
	}
}
 // //setup Clock Divider button pressed
	//CNumericEntryDialog INumericEntryDialog;  
	//
	////Set dialog box data titles and number value
	//INumericEntryDialog.vEditString = vClockString;

	//INumericEntryDialog.m_DialogTitleStaticText1 = "";
	//INumericEntryDialog.m_DialogTitleStaticText2 = "Pixel Clock Divider";
	//INumericEntryDialog.m_DialogTitleStaticText3 = "Setup System";
	//INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + vClockString;

	////if ((vLocalConfigurationData->vScanTracType == cBoneAnalyzerScanTrac) && (vLocalConfigurationData->vAdagioUse384PixeluController == 0))
	////	INumericEntryDialog.m_UnitsString = "Default: 20";
	////else
	//	INumericEntryDialog.m_UnitsString = "Default: 8";

	//INumericEntryDialog.vMaxValue = 4096;
	//INumericEntryDialog.vMinValue = 1;
	//INumericEntryDialog.vIntegerOnly = true;
	////Pass control to dialog box and display
	//int nResponse = INumericEntryDialog.DoModal();
	////dialog box is now closed, if user pressed select do this
	////if user pressed cancel, do nothing
	//if (nResponse == IDOK)
	//{
	//	vClockString = INumericEntryDialog.vEditString;
	//	SetDlgItemText(IDC_ClockDividerDisplay,vClockString);
	//	SetChangeMade();
	//}
	//else 
	//if (nResponse == 10)
	//{
	//	//Main Menu button pressed
	//	Sleep(1); //is the equivelent of a yeild statement;
	//	CDialog::EndDialog(10);
	//}

void CDetectorSetupDialog::OnSubFunction7Button() 
{
	//Auto Find detector Gain button pressed
	if (vLocalSystemData->vSystemRunMode == cStoppedSystemMode)
	{
		if (vMainWindowPointer->InterlocksOK())
		{
			PrepareToExit();
			vMainWindowPointer->CreateBackupGainAndPixelAdjustments();

			for (WORD TempDetectorTypeLoop = cPoint4mmDetectors; TempDetectorTypeLoop <= cPoint8mmDetectors; TempDetectorTypeLoop++)
			for (DWORD TempLoop = 0; TempLoop < cNumberOfPixelsToAdjustAllPixels1152; TempLoop++)
			{
				vLocalConfigurationData->vDetectorEdgeAdjustGain[TempLoop][TempDetectorTypeLoop] = 0;
				vLocalConfigurationData->vDetectorEdgeAdjustOffset[TempLoop][TempDetectorTypeLoop] = 0;
			}
			if (vGlobalCurrentProduct)
				ReportErrorMessage("Save Original Product so can restore after calibration complete: " + *vGlobalCurrentProduct->GetProductName(), cWriteToLog, 0);
			vLocalSystemData->vFullDetectorCalibration = false;
			vMainWindowPointer->vCalibrateHoldCurrentProduct = vGlobalCurrentProduct;
			//vMainWindowPointer->CalibrateDetectorGain();
			vMainWindowPointer->LoadCalibrateOffsetProduct();
			vLocalSystemData->vModeToRunAfterCalibrateOffset = cCalibratingDetectorGainMode;
			vLocalSystemData->vCalibrateGainIterationNumber = 0;
			//this will calibrate detectors, then do gain adjustments
			vLocalSystemData->vCalibrateErrorCount = 0;
			//calibrate offsets, then it will calibrate gain
			//vMainWindowPointer->CalibrateADCDetectorOffset(true,false);
			ReportErrorMessage("Just Detector Gain Calibration Started", cWriteToLog,0);
			vLocalSystemData->vCalibrateADCAfterDAC = true;
			vLocalSystemData->vAutoCalibrate = true;
			vLocalSystemData->vAutoCalibrateWhileRunning = false;
			if (vLocalConfigurationData->vUseDACAndADCinDensityCompensation)
				vMainWindowPointer->CalibrateDACDetectorOffset();
			else
				vMainWindowPointer->CalibrateADCDetectorOffset(vLocalSystemData->vAutoCalibrate, false);
			vOldCalibrating = true;
			Sleep(1); //is the equivelent of a yeild statement;
			CDialog::EndDialog(10);
		}
		else
		{
			CNoticeDialog TempNoticeDialog;
			CString TempText("\n\n\nX-Rays not on, Check Key Switch, E-Stop, Interlocks");
			if (PasswordOK(cSuperInspxPassword,false))
				TempText = TempText + "\nInterlocks State: " + WordToHex(vLocalSystemData->vSourceInterLocks);
			TempNoticeDialog.vNoticeText = TempText;
			TempNoticeDialog.vType = cNoticeMessage;
			TempNoticeDialog.DoModal();
		}
	}
	else
	if ((vLocalSystemData->vSystemRunMode == cCalibratingDetectorEdgesMode) ||
		(vLocalSystemData->vSystemRunMode == cCalibratingDetectorGainMode) ||
		(vLocalSystemData->vSystemRunMode == cCalibratingDetectorOffsetMode))
	{
		vMainWindowPointer->StopRunning(true);
	}
	else
	{
		CNoticeDialog TempNoticeDialog;
		CString TempText("\n\n\nSystem must be stopped to run this function");
		TempNoticeDialog.vNoticeText = TempText;
		TempNoticeDialog.vType = cNoticeMessage;
		TempNoticeDialog.DoModal();
	}
}

void CDetectorSetupDialog::OnSubFunction8Button() 
{
	//calibrate detector edges button pressed
	if (vLocalSystemData->vSystemRunMode == cStoppedSystemMode)
	{
		if (vMainWindowPointer->InterlocksOK())
		{
			vLocalSystemData->vCalibratingXScanPoint4mmDetectors = 0; //calibrate saving in defalt calibration values
			if ((vLocalConfigurationData->vUseXScanDetectors) && (vLocalConfigurationData->vScanTracType != cForteScanTrac) && (vLocalConfigurationData->vScanTracType != cCaseInspectorScanTrac))
			{
				vLocalConfigurationData->vPixelsPerDetector = 64;  //if x-scan, .8mm detectors is default
				CYesNoDialog TempYesNoDialog;
				TempYesNoDialog.vNoticeText = _T("\n\nX-Scan Detectors can run in .4mm or .8mm mode\nCalibrate in .4mm or .8mm mode?");
				TempYesNoDialog.vYesButtonText = "Calibrate in .8mm Mode";
				TempYesNoDialog.vNoButtonText = "Calibrate in .4mm Mode";
				TempYesNoDialog.vQuestionType = cConfirmQuestion;
				int TempResult = TempYesNoDialog.DoModal();
				if (TempResult != IDOK)
				{
					vLocalSystemData->vCalibratingXScanPoint4mmDetectors = 1; //verify calibrations .4 value calibration values
					vLocalConfigurationData->vPixelsPerDetector = 128; //x-scan .4 mm mode piced
				}
			}
			if (vGlobalCurrentProduct)
				ReportErrorMessage("Save Original Product so can restore after calibration complete: " + *vGlobalCurrentProduct->GetProductName(), cWriteToLog, 0);
			vLocalSystemData->vFullDetectorCalibration = false;
			vMainWindowPointer->vCalibrateHoldCurrentProduct = vGlobalCurrentProduct;
			vGlobalPixelsPerUnitInHeight = vLocalConfigurationData->vPixelsPerDetector / vLocalConfigurationData->vDetectorLength;
			PrepareToExit();
			vMainWindowPointer->CreateBackupGainAndPixelAdjustments();

			vLocalSystemData->vApplyGainAndOffsetDuringCalibration = false;
			vMainWindowPointer->LoadCalibrateEdgesProduct();
			vLocalSystemData->vFindingProductBrightness = false;
			vLocalSystemData->vModeToRunAfterCalibrateOffset = cCalibratingDetectorEdgesMode;
			//this will calibrate detectors, then do gain adjustments
			vLocalSystemData->vCalibrateErrorCount = 0;
			//calibrate offsets, then it will calibrate pixels
			//vMainWindowPointer->CalibrateADCDetectorOffset(true,false);
			ReportErrorMessage("Just Detector Pixel Calibration Started", cWriteToLog,0);
			vLocalSystemData->vCalibrateADCAfterDAC = true;
			vLocalSystemData->vAutoCalibrate = true;
			vLocalSystemData->vAutoCalibrateWhileRunning = false;
			if (vLocalConfigurationData->vUseDACAndADCinDensityCompensation)
				vMainWindowPointer->CalibrateDACDetectorOffset();
			else
				vMainWindowPointer->CalibrateADCDetectorOffset(vLocalSystemData->vAutoCalibrate, false);
			vOldCalibrating = true;
			Sleep(1); //is the equivelent of a yeild statement;
			CDialog::EndDialog(10);

		}
		else
		{
			CNoticeDialog TempNoticeDialog;
			CString TempText("\n\n\nX-Rays not on, Check Key Switch, E-Stop, Interlocks");
			if (PasswordOK(cSuperInspxPassword,false))
				TempText = TempText + "\nInterlocks State: " + WordToHex(vLocalSystemData->vSourceInterLocks);
			TempNoticeDialog.vNoticeText = TempText;
			TempNoticeDialog.vType = cNoticeMessage;
			TempNoticeDialog.DoModal();
		}
	}
	else
	if ((vLocalSystemData->vSystemRunMode == cCalibratingDetectorEdgesMode) || (vLocalSystemData->vSystemRunMode == cCalibratingDetectorGainMode) ||
		(vLocalSystemData->vSystemRunMode == cCalibratingDetectorOffsetMode))
	{
		vMainWindowPointer->StopRunning(true);
	}
	else
	{
		CNoticeDialog TempNoticeDialog;
		CString TempText("\n\n\nSystem must be stopped to run this function");
		TempNoticeDialog.vNoticeText = TempText;
		TempNoticeDialog.vType = cNoticeMessage;
		TempNoticeDialog.DoModal();
	}
}

BOOL CDetectorSetupDialog::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == cUpdateTitleBarMessage)
	{
		CWnd *TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText1);
		if (TempWindow)
			TempWindow->Invalidate(false);
		return true;  //true indicates it has been handled, so do not process
	}
	else
	if (pMsg->message == WM_KEYDOWN)
	{
		CWnd *TempWindow = NULL;
		if (vGlobalShiftKeyDown)
			PostThreadMessage(vGlobalCallingThreadID,cHiddenKeyPressed, 0, pMsg->wParam);
		else
		switch (pMsg->wParam)
		{
			case 35: // End Key, Left Right on duo but new Image Key on Universal Overlays, Image Button
				OnSubFunction9Button();
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
			case 116: // F5:
				OnSubFunction1Button();
			break;
			case 117: // F6 Key - Function 5:
				OnSubFunction4Button();
			break;
			case 118: // F7 Key - Sub Function 1:
				OnFunction1Button();
				return true;
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
			case 36:  //Home key, Screen Capture
				vGlobalShiftKeyDown = true;
				PostThreadMessage(vGlobalCallingThreadID,cHiddenKeyPressed, 0, 0x61);
			break;
			case cPageDown: // Help
				PostThreadMessage(vGlobalCallingThreadID, cShowHelp, 41, 0);

				TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText1);
				if (TempWindow)
					TempWindow->Invalidate(false);
			break;
			case 27: // Escape
				::PostThreadMessage(vGlobalCallingThreadID,	cStartChatWindowMessage, 0,0);
			break;	
		}
		return true;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

HBRUSH CDetectorSetupDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	HWND TempDialogHwnd = this->m_hWnd;
	int TextSize;
	pDC->SetAttribDC(pDC->m_hDC);
	pDC->SetBkMode(TRANSPARENT);
	
	if (pWnd->GetDlgCtrlID() == IDC_DialogTitleStaticText1)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DialogTitleStaticText, 8);  //5 is medium large
		pDC->SetTextColor(vGlobalDialogTitleColor);
	}

	if (pWnd->GetDlgCtrlID() == IDC_Function5Display)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function5Button, 5);  //5 is medium large
		if (vLocalConfigurationData->vNumberOfPixelAtBottomToIgnoreInCalibration > 10)
		{
			pDC->SetBkMode(TRANSPARENT);
			return vLocalSystemData->vYellowBrush;
		}
	}

	if (pWnd->GetDlgCtrlID() == IDC_PreAmplifierGain)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_PreAmplifierGain, 5);
		if (vGlobalFPGAVersion10Point0OrHigher)
		{
			if (vPreAmplifierGain != 1) //gain of 2
			{
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(cButtonTextColor);
				return vLocalSystemData->vYellowBrush;
			}
		}
		else
		{
			if (vPreAmplifierGain != 2) //gain of 4
			{
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(cButtonTextColor);
				return vLocalSystemData->vYellowBrush;
			}
		}
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction2Display)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction2Display, 5);
		if (vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac)
		{
			if (!((vPixelsPerDetector == 64) && (vDetectorLength == 4)))
			{
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(cButtonTextColor);
				return vLocalSystemData->vYellowBrush;
			}
		}
		else
		{
			if (!((vPixelsPerDetector == 64) && (vDetectorLength == 2)))
			{
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(cButtonTextColor);
				return vLocalSystemData->vYellowBrush;
			}
		}
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction6Display)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_ClockDividerDisplay, 5);	//not a button but is two lines
	}

	//if (pWnd->GetDlgCtrlID() == IDC_ClockDividerDisplay)
	//if (vGlobalFPGAVersion10Point0OrHigher)
	//{
	//	if (vClockString != "8")
	//	{
	//		pDC->SetBkMode(TRANSPARENT);
	//		pDC->SetTextColor(cButtonTextColor);
	//		return vLocalSystemData->vYellowBrush;
	//	}
	//}
	//else
	//{
	//	if (vClockString != "20")
	//	{
	//		pDC->SetBkMode(TRANSPARENT);
	//		pDC->SetTextColor(cButtonTextColor);
	//		return vLocalSystemData->vYellowBrush;
	//	}
	//}

	if (pWnd->GetDlgCtrlID() == IDC_Background)
	{
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Function5Display)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function5Display, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_ADCRegister0Label)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_ADCRegister0Label, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_ADCRegister1Label)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_ADCRegister1Label, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Detector1)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Detector1, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Detector2)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Detector2, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Detector3)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Detector3, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Detector4)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Detector4, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Detector5)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Detector5, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Detector6)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Detector6, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Detector7)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Detector7, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Detector8)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Detector8, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Detector9)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Detector9, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Detector10) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Detector10, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Detector11) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Detector11, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Detector12) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Detector12, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DetectorGain1) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DetectorGain1, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DetectorGain2) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DetectorGain1, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DetectorGain3) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DetectorGain1, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DetectorGain4) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DetectorGain1, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DetectorGain5) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DetectorGain1, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DetectorGain6) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DetectorGain1, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DetectorGain7) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DetectorGain1, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DetectorGain8) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DetectorGain1, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DetectorGain9) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DetectorGain1, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DetectorGain10) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DetectorGain10, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DetectorGain11) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DetectorGain11, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DetectorGain12) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DetectorGain12, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DetectorOffset1) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DetectorOffset1, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DetectorOffset2) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DetectorOffset2, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DetectorOffset3) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DetectorOffset3, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DetectorOffset4) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DetectorOffset4, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DetectorOffset5) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DetectorOffset5, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DetectorOffset6) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DetectorOffset6, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DetectorOffset7) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DetectorOffset7, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DetectorOffset8) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DetectorOffset8, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DetectorOffset9) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DetectorOffset9, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DetectorOffset10) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DetectorOffset10, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DetectorOffset11) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DetectorOffset11, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DetectorOffset12) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DetectorOffset12, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DetectorMeasuredGain1)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DetectorMeasuredGain1, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DetectorMeasuredGain2)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DetectorMeasuredGain2, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DetectorMeasuredGain3)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DetectorMeasuredGain3, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DetectorMeasuredGain4)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DetectorMeasuredGain4, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DetectorMeasuredGain5)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DetectorMeasuredGain5, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DetectorMeasuredGain6)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DetectorMeasuredGain6, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DetectorMeasuredGain7)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DetectorMeasuredGain7, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DetectorMeasuredGain8)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DetectorMeasuredGain8, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DetectorMeasuredGain9)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DetectorMeasuredGain9, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DetectorMeasuredGain10) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DetectorMeasuredGain10, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DetectorMeasuredGain11) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DetectorMeasuredGain11, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DetectorMeasuredGain12) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DetectorMeasuredGain12, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DetectorMeasuredOffset1) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DetectorMeasuredOffset1, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DetectorMeasuredOffset2) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DetectorMeasuredOffset2, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DetectorMeasuredOffset3) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DetectorMeasuredOffset3, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DetectorMeasuredOffset4) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DetectorMeasuredOffset4, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DetectorMeasuredOffset5) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DetectorMeasuredOffset5, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DetectorMeasuredOffset6) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DetectorMeasuredOffset6, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DetectorMeasuredOffset7) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DetectorMeasuredOffset7, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DetectorMeasuredOffset8) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DetectorMeasuredOffset8, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DetectorMeasuredOffset9) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DetectorMeasuredOffset9, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DetectorMeasuredOffset10) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DetectorMeasuredOffset10, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DetectorMeasuredOffset11) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DetectorMeasuredOffset11, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DetectorMeasuredOffset12) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DetectorMeasuredOffset12, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_TableLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_TableLabel, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_XRaysOn2)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_XRaysOn2, 7);  //5 is medium large
		pDC->SetTextColor(cYellow);
		pDC->SetBkColor(cRed);
		return vLocalSystemData->vRedBrush;
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
		if (vOldCalibrating)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cWhite);
			pDC->SetTextColor(cButtonTextColor);
			return vLocalSystemData->vRedBrush;
		}
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
	if (pWnd->GetDlgCtrlID() == IDC_ADCOffsetButton)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_ADCOffsetButton, 5); 
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DACOffsetsButton) 
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_DACOffsetsButton, 5); 
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
		if (vSubFunction4ButtonEnable == true)
			pDC->SetTextColor(cButtonTextColor);
		else
			pDC->SetTextColor(cGray);
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
		if (vOldCalibrating)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cWhite);
			pDC->SetTextColor(cButtonTextColor);
			return vLocalSystemData->vRedBrush;
		}
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction8Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction8Button, 5);  //5 is medium large
		if (vOldCalibrating)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cWhite);
			pDC->SetTextColor(cButtonTextColor);
			return vLocalSystemData->vRedBrush;
		}
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction9Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction9Button, 5);
		if (vOldCalibrating)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cWhite);
			pDC->SetTextColor(cButtonTextColor);
			return vLocalSystemData->vRedBrush;
		}
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	
	return hbr;
}

void CDetectorSetupDialog::SetChangeMade() 
{
	if (!vChangeMade)
	{
		m_Function2Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		m_Function4Button.ShowWindow(SW_SHOW);
		SetDlgItemText(IDC_Function2Button, _T("Save\nand Exit"));
		SetDlgItemText(IDC_Function4Button, _T("Cancel"));
		vChangeMade = true;
	}
}

void CDetectorSetupDialog::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
  if (nIDEvent == vUpdateButtonTimerHandle)
  {
		if (vOldCalibrating)
		if (vLocalSystemData->vSystemRunMode == cStoppedSystemMode)
		{
			vOldCalibrating = false;
			SetDlgItemText(IDC_SubFunction9Button, _T("Verify Pixels!"));
			SetDlgItemText(IDC_SubFunction8Button, _T("Calibrate Detector Pixels! (3)"));
			SetDlgItemText(IDC_SubFunction7Button, _T("Auto Find Detector Gain! (2)"));
			SetDlgItemText(IDC_Function3Button, _T("Auto Find Offset! (1)"));
			UpdateDisplay();
		}
		if (vOldXRayOnNotice != vLocalSystemData->vXRayOnNotice)
		{
			vOldXRayOnNotice = vLocalSystemData->vXRayOnNotice;
			if (vOldXRayOnNotice)
				m_XRaysOn2.ShowWindow(SW_SHOW);
			else
				m_XRaysOn2.ShowWindow(SW_HIDE);
		}
	}
	
	CDialog::OnTimer(nIDEvent);
}

void CDetectorSetupDialog::UpdateDisplay() 
{
	CString TempText = "";
	CString TempDetectorType = "";

	if ((vPixelsPerDetector == 64) && (vDetectorLength == 4))
		SetDlgItemText(IDC_SubFunction2Display, _T("1.6mm Pixels"));
	else
	if (vGlobalFPGAVersion16point0orAbove)
	{
		if (vUseXScanDetectors)
		{
			TempDetectorType = "X-Scan";
			SetDlgItemText(IDC_SubFunction2Display, TempDetectorType);
		}
		else
		{
			TempDetectorType = "DT ";

			if (vPixelsPerDetector == 128)
				SetDlgItemText(IDC_SubFunction2Display, TempDetectorType + ".4mm Pixels");
			else
				SetDlgItemText(IDC_SubFunction2Display, TempDetectorType + ".8mm Pixels");
		}
		SetDlgItemText(IDC_SubFunction2Button, _T("\nDetector\nType"));
	}
	else
	{
		if (vPixelsPerDetector == 128)
		{
			SetDlgItemText(IDC_SubFunction2Display, _T("DT .4mm Pixels"));
			SetDlgItemText(IDC_SubFunction2Button, _T("System Has .8mm Pixels"));
		}
		else
		{
			SetDlgItemText(IDC_SubFunction2Display, _T("DT .8mm Pixels"));
			SetDlgItemText(IDC_SubFunction2Button, _T("System Has .4mm Pixels"));
		}
	}
	if (vLocalConfigurationData->vCorrectDetectorEdges == 1)
	{
		TempText = "Correcting Only Edge Pixels";
	}
	else
	if (vLocalConfigurationData->vCorrectDetectorEdges == 2)
	{
		//CString TempString = " In All Detectors";
		//if (vLocalConfigurationData->vNumberOfDetectorsToCalibrate)
		//	TempString = " In Lower " + 
		//	dtoa(vLocalConfigurationData->vNumberOfDetectorsToCalibrate,0) + " Detectors";
		TempText = "Correcting All Pixels";// + TempString;
	}
	else
	{
		TempText = "Not Correcting Any Pixels";
	}
	TempText = TempText + "\nConveyor blocks bottom " + 
		dtoa(vLocalConfigurationData->vNumberOfPixelAtBottomToIgnoreInCalibration,0) + " pixels";
	SetDlgItemText(IDC_Function5Display,TempText);

	if ((vLocalConfigurationData->vDetectorsAtBottomToIgnoreDuringCalibration == 0) && (vLocalConfigurationData->vNumberOfDetectorsToCalibrate == 0))
		SetDlgItemText(IDC_SubFunction6Display, _T("Calibrate All Detectors"));
	else
	{
		CString TempString = "";
		if ((vLocalConfigurationData->vNumberOfDetectorsToCalibrate) && (vLocalConfigurationData->vNumberOfDetectorsToCalibrate < vLocalConfigurationData->vNumberOfXRayDetectors))
		{
			if ((vLocalConfigurationData->vNumberOfXRayDetectors - vLocalConfigurationData->vNumberOfDetectorsToCalibrate) == 1)
				TempString = "Don't Calibrate top detector";
			else
				TempString = "Don't Calibrate top " + dtoa(vLocalConfigurationData->vNumberOfXRayDetectors - vLocalConfigurationData->vNumberOfDetectorsToCalibrate, 0) + " detectors";
		}

		if (vLocalConfigurationData->vDetectorsAtBottomToIgnoreDuringCalibration)
		{
			if (TempString.GetLength() > 5)
				TempString = TempString + "\n";

			if (vLocalConfigurationData->vDetectorsAtBottomToIgnoreDuringCalibration == 1)
				TempString = TempString + "Don't Calibrate bottom detector";
			else
				TempString = TempString + "Don't Calibrate bottom " + dtoa(vLocalConfigurationData->vDetectorsAtBottomToIgnoreDuringCalibration, 0) + " detectors";
		}
		SetDlgItemText(IDC_SubFunction6Display, TempString);
	}


	/*
	if (vLocalConfigurationData->vCorrectDetectorEdges == 1)
	{
		SetDlgItemText(IDC_Function5Display,"Correcting Only Edge Pixels");
	}
	else
	if (vLocalConfigurationData->vCorrectDetectorEdges == 2)
	{
		CString TempString = " In All Detectors";
		if (vLocalConfigurationData->vNumberOfDetectorsToCalibrate)
			TempString = " In Lower " + 
			dtoa(vLocalConfigurationData->vNumberOfDetectorsToCalibrate,0) + " Detectors";
		SetDlgItemText(IDC_Function5Display,"Correcting All Pixels" + TempString);
	}
	else
	{
		SetDlgItemText(IDC_Function5Display,"Not Correcting Any Pixels");
	}
	*/
	double TempMinimumGain = 100000;
	for (BYTE TempLoop = 0; TempLoop < vNumberOfDetectors; TempLoop++)
	{
		if (vLocalConfigurationData->vMeasuredDetectorGain[TempLoop] < TempMinimumGain)
			TempMinimumGain = vLocalConfigurationData->vMeasuredDetectorGain[TempLoop];
	}

	if (vNumberOfDetectors == 6)
	{
		m_Detector7.ShowWindow(SW_HIDE);
		m_DetectorGain7.ShowWindow(SW_HIDE);
		m_DetectorOffset7.ShowWindow(SW_HIDE);
		m_DetectorMeasuredGain7.ShowWindow(SW_HIDE);
		m_DetectorMeasuredOffset7.ShowWindow(SW_HIDE);
		m_Detector8.ShowWindow(SW_HIDE);
		m_DetectorGain8.ShowWindow(SW_HIDE);
		m_DetectorOffset8.ShowWindow(SW_HIDE);
		m_DetectorMeasuredGain8.ShowWindow(SW_HIDE);
		m_DetectorMeasuredOffset8.ShowWindow(SW_HIDE);
		m_Detector9.ShowWindow(SW_HIDE);
		m_DetectorGain9.ShowWindow(SW_HIDE);
		m_DetectorOffset9.ShowWindow(SW_HIDE);
		m_DetectorMeasuredGain9.ShowWindow(SW_HIDE);
		m_DetectorMeasuredOffset9.ShowWindow(SW_HIDE);
		m_Detector10.ShowWindow(SW_HIDE);
		m_DetectorGain10.ShowWindow(SW_HIDE);
		m_DetectorOffset10.ShowWindow(SW_HIDE);
		m_DetectorMeasuredGain10.ShowWindow(SW_HIDE);
		m_DetectorMeasuredOffset10.ShowWindow(SW_HIDE);
		m_Detector11.ShowWindow(SW_HIDE);
		m_DetectorGain11.ShowWindow(SW_HIDE);
		m_DetectorOffset11.ShowWindow(SW_HIDE);
		m_DetectorMeasuredGain11.ShowWindow(SW_HIDE);
		m_DetectorMeasuredOffset11.ShowWindow(SW_HIDE);
		m_Detector12.ShowWindow(SW_HIDE);
		m_DetectorGain12.ShowWindow(SW_HIDE);
		m_DetectorOffset12.ShowWindow(SW_HIDE);
		m_DetectorMeasuredGain12.ShowWindow(SW_HIDE);
		m_DetectorMeasuredOffset12.ShowWindow(SW_HIDE);
	}
	if (vNumberOfDetectors >= 9)
	{//must have 9 or 12 detectors
		m_Detector7.ShowWindow(SW_SHOW);
		m_DetectorGain7.ShowWindow(SW_SHOW);
		m_DetectorOffset7.ShowWindow(SW_SHOW);
		m_DetectorMeasuredGain7.ShowWindow(SW_SHOW);
		m_DetectorMeasuredOffset7.ShowWindow(SW_SHOW);
		m_Detector8.ShowWindow(SW_SHOW);
		m_DetectorGain8.ShowWindow(SW_SHOW);
		m_DetectorOffset8.ShowWindow(SW_SHOW);
		m_DetectorMeasuredGain8.ShowWindow(SW_SHOW);
		m_DetectorMeasuredOffset8.ShowWindow(SW_SHOW);
		m_Detector9.ShowWindow(SW_SHOW);
		m_DetectorGain9.ShowWindow(SW_SHOW);
		m_DetectorOffset9.ShowWindow(SW_SHOW);
		m_DetectorMeasuredGain9.ShowWindow(SW_SHOW);
		m_DetectorMeasuredOffset9.ShowWindow(SW_SHOW);
	}
	if (vNumberOfDetectors == 12)
	{
		m_Detector10.ShowWindow(SW_SHOW);
		m_DetectorGain10.ShowWindow(SW_SHOW);
		m_DetectorOffset10.ShowWindow(SW_SHOW);
		m_DetectorMeasuredGain10.ShowWindow(SW_SHOW);
		m_DetectorMeasuredOffset10.ShowWindow(SW_SHOW);
		m_Detector11.ShowWindow(SW_SHOW);
		m_DetectorGain11.ShowWindow(SW_SHOW);
		m_DetectorOffset11.ShowWindow(SW_SHOW);
		m_DetectorMeasuredGain11.ShowWindow(SW_SHOW);
		m_DetectorMeasuredOffset11.ShowWindow(SW_SHOW);
		m_Detector12.ShowWindow(SW_SHOW);
		m_DetectorGain12.ShowWindow(SW_SHOW);
		m_DetectorOffset12.ShowWindow(SW_SHOW);
		m_DetectorMeasuredGain12.ShowWindow(SW_SHOW);
		m_DetectorMeasuredOffset12.ShowWindow(SW_SHOW);
	}
	else
	{ //only 9 detectors, so hide
		m_Detector10.ShowWindow(SW_HIDE);
		m_DetectorGain10.ShowWindow(SW_HIDE);
		m_DetectorOffset10.ShowWindow(SW_HIDE);
		m_DetectorMeasuredGain10.ShowWindow(SW_HIDE);
		m_DetectorMeasuredOffset10.ShowWindow(SW_HIDE);
		m_Detector11.ShowWindow(SW_HIDE);
		m_DetectorGain11.ShowWindow(SW_HIDE);
		m_DetectorOffset11.ShowWindow(SW_HIDE);
		m_DetectorMeasuredGain11.ShowWindow(SW_HIDE);
		m_DetectorMeasuredOffset11.ShowWindow(SW_HIDE);
		m_Detector12.ShowWindow(SW_HIDE);
		m_DetectorGain12.ShowWindow(SW_HIDE);
		m_DetectorOffset12.ShowWindow(SW_HIDE);
		m_DetectorMeasuredGain12.ShowWindow(SW_HIDE);
		m_DetectorMeasuredOffset12.ShowWindow(SW_HIDE);
	}

	WORD TempMaxDetectors = 12;
	if (TempMaxDetectors > vNumberOfDetectors)
		TempMaxDetectors = vNumberOfDetectors;

	for (BYTE TempLoop = 0; TempLoop < TempMaxDetectors; TempLoop++)
	{
		CString TempStringDetector("");
		CString TempStringGain("");
		CString TempStringOffset("");
		CString TempStringMeasuredGain("");
		CString TempStringMeasuredGainDelta("");
		CString TempStringMeasuredOffset("");
		TempStringDetector = dtoa(TempLoop + 1, 0);
		TempStringGain = dtoa(vDetectorGain[TempLoop],2);
		TempStringOffset = dtoa(vDACDetectorOffset[TempLoop],2) + ", " + dtoa(vADCDetectorOffset[TempLoop],0);

		TempStringMeasuredGain = dtoa(vLocalConfigurationData->vMeasuredDetectorGain[TempLoop],2);
		TempStringMeasuredOffset = dtoa(vLocalConfigurationData->vMeasuredDetectorOffset[TempLoop],1);
		if (vLocalConfigurationData->vMeasuredDetectorOffset[TempLoop] > 99999)
			TempStringMeasuredOffset = "N/A";
		if (vLocalConfigurationData->vMeasuredDetectorOffset[TempLoop] < -99999)
			TempStringMeasuredOffset = "N/A";
		//find the percent difference of this gain from the minimum gain
		if (TempMinimumGain)
			TempStringMeasuredGainDelta = dtoa(((vLocalConfigurationData->vMeasuredDetectorGain[TempLoop] - TempMinimumGain) / TempMinimumGain) * 100, 0);
		TempStringMeasuredGain = TempStringMeasuredGain + " (" + TempStringMeasuredGainDelta + ")";

		BYTE TempDisplayRow = TempLoop;
		if (vNumberOfDetectors == 6)
			TempDisplayRow = 6 + TempLoop;
		if (vNumberOfDetectors == 9)
			TempDisplayRow = 3 + TempLoop;
		switch (TempDisplayRow)
		{
			case 11: 
				SetDlgItemText(IDC_Detector1,TempStringDetector);
				SetDlgItemText(IDC_DetectorGain1,TempStringGain);
				SetDlgItemText(IDC_DetectorOffset1,TempStringOffset);
				SetDlgItemText(IDC_DetectorMeasuredGain1,TempStringMeasuredGain);
				SetDlgItemText(IDC_DetectorMeasuredOffset1,TempStringMeasuredOffset);
			break;
			case 10: 
				SetDlgItemText(IDC_Detector2,TempStringDetector);
				SetDlgItemText(IDC_DetectorGain2,TempStringGain);
				SetDlgItemText(IDC_DetectorOffset2,TempStringOffset);
				SetDlgItemText(IDC_DetectorMeasuredGain2,TempStringMeasuredGain);
				SetDlgItemText(IDC_DetectorMeasuredOffset2,TempStringMeasuredOffset);
			break;
			case 9: 
				SetDlgItemText(IDC_Detector3,TempStringDetector);
				SetDlgItemText(IDC_DetectorGain3,TempStringGain);
				SetDlgItemText(IDC_DetectorOffset3,TempStringOffset);
				SetDlgItemText(IDC_DetectorMeasuredGain3,TempStringMeasuredGain);
				SetDlgItemText(IDC_DetectorMeasuredOffset3,TempStringMeasuredOffset);
			break;
			case 8: 
				SetDlgItemText(IDC_Detector4,TempStringDetector);
				SetDlgItemText(IDC_DetectorGain4,TempStringGain);
				SetDlgItemText(IDC_DetectorOffset4,TempStringOffset);
				SetDlgItemText(IDC_DetectorMeasuredGain4,TempStringMeasuredGain);
				SetDlgItemText(IDC_DetectorMeasuredOffset4,TempStringMeasuredOffset);
			break;
			case 7: 
				SetDlgItemText(IDC_Detector5,TempStringDetector);
				SetDlgItemText(IDC_DetectorGain5,TempStringGain);
				SetDlgItemText(IDC_DetectorOffset5,TempStringOffset);
				SetDlgItemText(IDC_DetectorMeasuredGain5,TempStringMeasuredGain);
				SetDlgItemText(IDC_DetectorMeasuredOffset5,TempStringMeasuredOffset);
			break;
			case 6: 
				SetDlgItemText(IDC_Detector6,TempStringDetector);
				SetDlgItemText(IDC_DetectorGain6,TempStringGain);
				SetDlgItemText(IDC_DetectorOffset6,TempStringOffset);
				SetDlgItemText(IDC_DetectorMeasuredGain6,TempStringMeasuredGain);
				SetDlgItemText(IDC_DetectorMeasuredOffset6,TempStringMeasuredOffset);
			break;
			case 5: 
				SetDlgItemText(IDC_Detector7,TempStringDetector);
				SetDlgItemText(IDC_DetectorGain7,TempStringGain);
				SetDlgItemText(IDC_DetectorOffset7,TempStringOffset);
				SetDlgItemText(IDC_DetectorMeasuredGain7,TempStringMeasuredGain);
				SetDlgItemText(IDC_DetectorMeasuredOffset7,TempStringMeasuredOffset);
			break;
			case 4: 				
				SetDlgItemText(IDC_Detector8,TempStringDetector);
				SetDlgItemText(IDC_DetectorGain8,TempStringGain);
				SetDlgItemText(IDC_DetectorOffset8,TempStringOffset);
				SetDlgItemText(IDC_DetectorMeasuredGain8,TempStringMeasuredGain);
				SetDlgItemText(IDC_DetectorMeasuredOffset8,TempStringMeasuredOffset);
			break;
			case 3: 
				SetDlgItemText(IDC_Detector9,TempStringDetector);
				SetDlgItemText(IDC_DetectorGain9,TempStringGain);
				SetDlgItemText(IDC_DetectorOffset9,TempStringOffset);
				SetDlgItemText(IDC_DetectorMeasuredGain9,TempStringMeasuredGain);
				SetDlgItemText(IDC_DetectorMeasuredOffset9,TempStringMeasuredOffset);
			break;
			case 2: 
				SetDlgItemText(IDC_Detector10,TempStringDetector);
				SetDlgItemText(IDC_DetectorGain10,TempStringGain);
				SetDlgItemText(IDC_DetectorOffset10,TempStringOffset);
				SetDlgItemText(IDC_DetectorMeasuredGain10,TempStringMeasuredGain);
				SetDlgItemText(IDC_DetectorMeasuredOffset10,TempStringMeasuredOffset);
			break;
			case 1: 
				SetDlgItemText(IDC_Detector11,TempStringDetector);
				SetDlgItemText(IDC_DetectorGain11,TempStringGain);
				SetDlgItemText(IDC_DetectorOffset11,TempStringOffset);
				SetDlgItemText(IDC_DetectorMeasuredGain11,TempStringMeasuredGain);
				SetDlgItemText(IDC_DetectorMeasuredOffset11,TempStringMeasuredOffset);
			break;
			case 0: 
				SetDlgItemText(IDC_Detector12,TempStringDetector);
				SetDlgItemText(IDC_DetectorGain12,TempStringGain);
				SetDlgItemText(IDC_DetectorOffset12,TempStringOffset);
				SetDlgItemText(IDC_DetectorMeasuredGain12,TempStringMeasuredGain);
				SetDlgItemText(IDC_DetectorMeasuredOffset12,TempStringMeasuredOffset);
			break;
		}
	}
}

CString CDetectorSetupDialog::PadLeft(CString TempString,BYTE TempSpaces) 
{
	int TempDelta = TempSpaces - TempString.GetLength();
	if (TempDelta > 0)
  for (WORD TempLoop = 0; TempLoop < TempDelta; TempLoop++)
		TempString = "  " + TempString;
	return TempString;
}

void CDetectorSetupDialog::ShowInspxFunctions()
{
	m_SubFunction4Button.ShowWindow(SW_SHOW);
	SetDlgItemText(IDC_SubFunction4Button, _T("Modify ADC Offset"));
}

void CDetectorSetupDialog::OnModeButton()
{
}


void CDetectorSetupDialog::OnADCOffsetButton() 
{
	// Detector ADC Offset button pressed
	CString TempString1("");
	CNumericEntryDialog INumericEntryDialog;  
	for (int TempLoop = 0; TempLoop < vNumberOfDetectors; TempLoop++)
	{
		//Set dialog box data titles and number value
		INumericEntryDialog.vEditString = dtoa(vADCDetectorOffset[TempLoop],0);
		INumericEntryDialog.m_DialogTitleStaticText1 = "Detector Offset";
		INumericEntryDialog.m_DialogTitleStaticText2 = "Setup System";
		INumericEntryDialog.m_DialogTitleStaticText3 = "Electronic ADC Offset for Detector " + dtoa(TempLoop + 1, 2);
		INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + INumericEntryDialog.vEditString;

		if (vGlobalFPGAVersion18point0orAbove)
		{
			INumericEntryDialog.m_UnitsString = "Default: 0, -255 to 255, measured in mV";
			INumericEntryDialog.vMaxValue = 255;
			INumericEntryDialog.vMinValue = -255;
		}
		else
		{
			INumericEntryDialog.m_UnitsString = "Default: 0, -700 to 500, measured in mV";
			INumericEntryDialog.vMaxValue = 500;
			INumericEntryDialog.vMinValue = -700;
		}
		INumericEntryDialog.vIntegerOnly = true;
		INumericEntryDialog.vAllowNegative = true;
		//Pass control to dialog box and display
		int nResponse = INumericEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			vADCDetectorOffset[TempLoop] = _wtoi(INumericEntryDialog.vEditString);
			SetChangeMade();
		}
		else 
		if (nResponse == 10)
		{
			//Main Menu button pressed
			TempLoop = vNumberOfDetectors;
			Sleep(1); //is the equivelent of a yeild statement;
			CDialog::EndDialog(10);
		}
		else
			TempLoop = vNumberOfDetectors;
	}
	UpdateDisplay();
}

void CDetectorSetupDialog::OnSubFunction9Button() 
{
	//verify detector edges button pressed
	if (vLocalSystemData->vSystemRunMode == cStoppedSystemMode)
	{
		if (vMainWindowPointer->InterlocksOK())
		{
			vLocalSystemData->vCalibratingXScanPoint4mmDetectors = 0; //verify calibrations in defalt calibration values
			if ((vLocalConfigurationData->vUseXScanDetectors) && (vLocalConfigurationData->vScanTracType != cForteScanTrac) && (vLocalConfigurationData->vScanTracType != cCaseInspectorScanTrac))
			{
				vLocalConfigurationData->vPixelsPerDetector = 64; //x-scan .8 mm by default
				CYesNoDialog TempYesNoDialog;
				TempYesNoDialog.vNoticeText = _T("\n\nX-Scan Detectors can run in .4mm or .8mm mode\nVerify in .4mm or .8mm mode?");
				TempYesNoDialog.vYesButtonText = "Verify in .8mm Mode";
				TempYesNoDialog.vNoButtonText = "Verify in .4mm Mode";
				TempYesNoDialog.vQuestionType = cConfirmQuestion;
				int TempResult = TempYesNoDialog.DoModal();
				if (TempResult != IDOK)
				{
					vLocalSystemData->vCalibratingXScanPoint4mmDetectors = 1; //verify calibrations .4 value calibration values
					vLocalConfigurationData->vPixelsPerDetector = 128; //x-scan .4 mm mode piced
				}
			}
			if (vGlobalCurrentProduct)
				ReportErrorMessage("Save Original Product so can restore after calibration complete: " + *vGlobalCurrentProduct->GetProductName(), cWriteToLog, 0);
			vLocalSystemData->vFullDetectorCalibration = false;
			vMainWindowPointer->vCalibrateHoldCurrentProduct = vGlobalCurrentProduct;
			vGlobalPixelsPerUnitInHeight = vLocalConfigurationData->vPixelsPerDetector / vLocalConfigurationData->vDetectorLength;
			vLocalSystemData->vApplyGainAndOffsetDuringCalibration = true;
			vMainWindowPointer->LoadCalibrateEdgesProduct();
			vLocalSystemData->vFindingProductBrightness = false;
			vLocalSystemData->vModeToRunAfterCalibrateOffset = cCalibratingDetectorEdgesMode;
			//this will calibrate detectors, then do gain adjustments
			vLocalSystemData->vCalibrateErrorCount = 0;
			//calibrate offsets, then it will verify pixels
			//vMainWindowPointer->CalibrateADCDetectorOffset(true,false);
			ReportErrorMessage("Just Detector Pixel Verification Started", cWriteToLog,0);
			vLocalSystemData->vCalibrateADCAfterDAC = true;
			vLocalSystemData->vAutoCalibrate = true;
			vLocalSystemData->vAutoCalibrateWhileRunning = false;
			if (vLocalConfigurationData->vUseDACAndADCinDensityCompensation)
				vMainWindowPointer->CalibrateDACDetectorOffset();
			else
				vMainWindowPointer->CalibrateADCDetectorOffset(vLocalSystemData->vAutoCalibrate, false);
			vOldCalibrating = true;
			Sleep(1); //is the equivelent of a yeild statement;
			CDialog::EndDialog(10);
		}
		else
		{
			CNoticeDialog TempNoticeDialog;
			CString TempText("\n\n\nX-Rays not on, Check Key Switch, E-Stop, Interlocks");
			if (PasswordOK(cSuperInspxPassword,false))
				TempText = TempText + "\nInterlocks State: " + WordToHex(vLocalSystemData->vSourceInterLocks);
			TempNoticeDialog.vNoticeText = TempText;
			TempNoticeDialog.vType = cNoticeMessage;
			TempNoticeDialog.DoModal();
		}
	}
	else
	if ((vLocalSystemData->vSystemRunMode == cCalibratingDetectorOffsetMode) ||
		(vLocalSystemData->vSystemRunMode == cCalibratingDetectorGainMode) ||
		(vLocalSystemData->vSystemRunMode == cCalibratingDetectorEdgesMode))
	{
		vMainWindowPointer->EndCalibrateDetectorEdges(true);
	}
	else
	{
		CNoticeDialog TempNoticeDialog;
		CString TempText("\n\n\nSystem must be stopped to run this function");
		TempNoticeDialog.vNoticeText = TempText;
		TempNoticeDialog.vType = cNoticeMessage;
		TempNoticeDialog.DoModal();
	}
}

void CDetectorSetupDialog::OnDACOffsetsButton() 
{
	// DACDetector Offset button pressed
	CString TempString1("");
	CNumericEntryDialog INumericEntryDialog;  
	for (int TempLoop = 0; TempLoop < vNumberOfDetectors; TempLoop++)
	{
		//Set dialog box data titles and number value
		INumericEntryDialog.vEditString = dtoa(vDACDetectorOffset[TempLoop],2);
		INumericEntryDialog.m_DialogTitleStaticText1 = "Detector Offset";
		INumericEntryDialog.m_DialogTitleStaticText2 = "Setup System";
		INumericEntryDialog.m_DialogTitleStaticText3 = "Electronic DAC Offset for Detector " + dtoa(TempLoop + 1,0);
		INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + INumericEntryDialog.vEditString;

		INumericEntryDialog.m_UnitsString = "Default: 0, measured in percent";
		INumericEntryDialog.vMaxValue = 100;
		INumericEntryDialog.vIntegerOnly = false;
		//Pass control to dialog box and display
		int nResponse = INumericEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			vDACDetectorOffset[TempLoop] = ATOF(INumericEntryDialog.vEditString);
			SetChangeMade();
		}
		else 
		if (nResponse == 10)
		{
			//Main Menu button pressed
			TempLoop = vNumberOfDetectors;
			Sleep(1); //is the equivelent of a yeild statement;
			CDialog::EndDialog(10);
		}
		else
			TempLoop = vNumberOfDetectors;
	}
	UpdateDisplay();
}

void CDetectorSetupDialog::PrepareToExit()
{
	/*
	if ((vLocalSystemData->vADRange13 != vADRange13) ||
		(vLocalSystemData->vADInvert != vADInvert))
	{
		vLocalSystemData->vADRange13 = vADRange13;
		vLocalSystemData->vADInvert = vADInvert;

		BYTE TempControlByte = 0;

		if (!vADInvert)
			TempControlByte = 0x04;
		if (vADRange13)
			TempControlByte = TempControlByte | 0x10;

		//Send A/D voltage range (0-2) and not invert
		for (BYTE TempLoop = 0; TempLoop < 3; TempLoop++)
		{
			tSerialCommand TempCommand;
			TempCommand[0] = 0x4C;
			TempCommand[1] = TempLoop + 1; //which ADC Chip
			TempCommand[2] = 0; //control register 1 on the chip
			TempCommand[3] = TempControlByte;  //not inverted, range 0 to 2 volts
			if (vGlobaluCSerialPort)
				vGlobaluCSerialPort->SendSerialCommand(TempCommand);
			Sleep(cSendCommandSleepTime);
		}
	}
	*/

	if (vLocalConfigurationData->vUseXScanDetectors != vUseXScanDetectors)
	{
		vLocalConfigurationData->vUseXScanDetectors = vUseXScanDetectors;
		if (!vUseXScanDetectors)
			vMainWindowPointer->ClearXScanProducts();
	}

	if (vLocalConfigurationData->vPreAmplifierGain != vPreAmplifierGain)
	{
		vLocalConfigurationData->vPreAmplifierGain = vPreAmplifierGain;

		vMainWindowPointer->SendPreAmpGain(vLocalConfigurationData->vPreAmplifierGain);
	}

	bool TempChangeMade = false;
	bool TempDACChangeMade = false;
	for (BYTE TempLoop = 0; TempLoop < vLocalConfigurationData->vNumberOfXRayDetectors; TempLoop++)
	{  
		if (vLocalSystemData->vDACDetectorOffset[TempLoop] != vDACDetectorOffset[TempLoop])
		{
			vLocalConfigurationData->vDACDetectorOffset[TempLoop] = vDACDetectorOffset[TempLoop];
			vLocalSystemData->vDACDetectorOffset[TempLoop] = vDACDetectorOffset[TempLoop];
			TempDACChangeMade = true;
		}
		if ((vLocalConfigurationData->vDetectorGain[TempLoop] != vDetectorGain[TempLoop]))
		{
			TempChangeMade = true;
			vLocalConfigurationData->vDetectorGain[TempLoop] = vDetectorGain[TempLoop];
		}
		if (vGlobaluCSerialPort)
		if (vLocalSystemData->vADCDetectorOffset[TempLoop] != vADCDetectorOffset[TempLoop])
		{
			vLocalConfigurationData->vADCDetectorOffset[TempLoop] = vADCDetectorOffset[TempLoop];
			vLocalSystemData->vADCDetectorOffset[TempLoop] = vADCDetectorOffset[TempLoop];
			vGlobaluCSerialPort->SendADCOffset(TempLoop, vLocalSystemData->vADCDetectorOffset[TempLoop]);
			TempChangeMade = true;
		}
	}
	if (TempChangeMade)
		vMainWindowPointer->SendADCGainsAndOffsets(false);
	if (TempDACChangeMade)
		vMainWindowPointer->SendAllDACDetectorOffsets();

	
	//vLocalConfigurationData->vDetectorClockDivider = _wtoi(vClockString);
	vLocalConfigurationData->vNumberOfXRayDetectors = vNumberOfDetectors;
	

	if ((vLocalConfigurationData->vPixelsPerDetector != vPixelsPerDetector) || (vLocalConfigurationData->vDetectorLength != vDetectorLength))
	{
		vLocalConfigurationData->SetDetectorLength(vDetectorLength);
		vLocalConfigurationData->SetPixelsPerDetector(vPixelsPerDetector);

			//Send Detector size .8 or .4 mm
		tSerialCommand TempCommand;
		TempCommand[0] = 0x23;
		TempCommand[1] = 0xEE;
		TempCommand[2] = 0;
		if (vLocalConfigurationData->vPixelsPerDetector == 128) //.4mm Detectors
			TempCommand[3] = 1;
		else
		if (vLocalConfigurationData->vDetectorLength == 4) //1.6mm Detectors
			TempCommand[3] = 2;
		else
		//if (vLocalConfigurationData->vPixelsPerDetector == 64)
			TempCommand[3] = 0; //.8mm Detectors
		if (vGlobaluCSerialPort)
			vGlobaluCSerialPort->SendSerialCommand(TempCommand);
		Sleep(cSendCommandSleepTimeSetup);
	}

	//if (vLocalConfigurationData->vPixelsPerDetector == 128) //uses .4mm detectors which are 128 pixels in 2 inches
	//	vGlobalPixelsPerUnitInHeight = 64;
	//else
	//if (vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac) //uses 1.6mm detectors which are 64 pixels in 4 inches
	//	vGlobalPixelsPerUnitInHeight = 16;
	//else
	//	vGlobalPixelsPerUnitInHeight = 32; //uses .8mm detectors which are 64 pixels in 2 inches

	vGlobalPixelsPerUnitInHeight = vPixelsPerDetector / vDetectorLength;

	if (vChangedDetectorSettings)
	if (vLocalConfigurationData->vSourceStrengthCount)
	{
		CYesNoDialog TempYesNoDialog;
		TempYesNoDialog.vNoticeText =  _T("\n\nX-Ray Detector settings changed\nClear X-Ray Source Strength History?");
		TempYesNoDialog.vYesButtonText = "Clear History";
		TempYesNoDialog.vNoButtonText = "Keep History";
		TempYesNoDialog.vQuestionType = cConfirmQuestion;
		int TempResult = TempYesNoDialog.DoModal();
		if (TempResult == IDOK)
			vLocalConfigurationData->vSourceStrengthCount = 0;
	}
}



void CDetectorSetupDialog::OnDialogtitlestatictext1()
{
	BYTE TempHiddenKeys = 8;
	if (!PasswordOK(cTemporaryInspxPassword,false))
		TempHiddenKeys = 10;
	PostThreadMessage(vGlobalCallingThreadID, cDisplaySoftKeyMenu, TempHiddenKeys, TempHiddenKeys);
}

