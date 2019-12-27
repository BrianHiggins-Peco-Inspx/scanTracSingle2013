//ScanTrac Side View Source File
// SystemValuesDialog.cpp : implementation file
//
//
// vCalibratingWithXRays, if true calibrating with old Function3Button
//
// vLocalSystemData->vTestingTrigger, if true calibrating with old OnCalibrateEncoderWithoutXRays


#include "stdafx.h"
#include "scantrac.h"
#include "SystemValuesDialog.h"
#include "NumericEntryDialog.h"
#include "YesNoDialog.h"
#include "SerialPort.h"
#include "SelectItemDialog.h"
#include "ImageAquisitionThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern BYTE vGlobalEndOfLineTimeOutMargin;
extern BYTE vGlobalScanTracType;
extern CImageAquisitionThread *vGlobalImageAquisitionThread;
extern BYTE vGlobalInAutoSize;
extern CSerialPort *vGlobaluCSerialPort;
extern BYTE vGlobalShiftKeyDown;
extern CProduct *vGlobalCurrentProduct;
//extern BYTE vGlobalUseMetric;
extern unsigned long vGlobalDialogTitleColor;
/////////////////////////////////////////////////////////////////////////////
// CSystemValuesDialog dialog


CSystemValuesDialog::CSystemValuesDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CSystemValuesDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSystemValuesDialog)
	//}}AFX_DATA_INIT
	vOldImageNumberToDisplay = 0xFFFF;
	vOldXRayState = 0;
	vMaximumSuggestedPPI = 37;
	vMinimumSuggestedPPI = 27;
	vChangeMade = false;
	vPipeLineFlowRate = 0;
	vOldLastBodyTriggerLength = 0;
	vOldBodyTriggerWidthStandardDeviation = 0;
	vOldBodyTriggerWidthAverage = 0;
	vCalibrateEncoderContainerWidth = 0;
	vWaitingForContainer = false;
	vOldContainerCount = 0;
	vNeedToCalibrateEncoder = 0;
	vOriginalProduct = NULL;
	vSystemBodyTriggerToXRayBeam = 0;
	vSystemEjectorPositionOffset = 0;
	vCalibratingWithXRays = false;
	vCalibrateEncoderMethod = cNotCalibrating;		//enum {cNotCalibrating, cCalXrayOnNoProduction, cCalXrayOffYesProduction};
	vEncoderDividerChanged = false;					// if encoder divider changed in this function, dont change back for encoder calibration
	vLockOutFunction3Button = false;				// create lockout for Function3Button, wait for encoder timeout
	for (BYTE TempLoop = 0; TempLoop < cNumberOfEjectors; TempLoop++)
		vSystemEjectorDistance[TempLoop] = 0;

	vLocalCWndCollection.Add(this);
	vLocalCWndCollection.Add(&m_SubFunction9Display);
	vLocalCWndCollection.Add(&m_SubFunction9Button);
	vLocalCWndCollection.Add(&m_SubFunction2Display);
	vLocalCWndCollection.Add(&m_SubFunction2Button);
	vLocalCWndCollection.Add(&m_SubFunction1Display);
	vLocalCWndCollection.Add(&m_SubFunction1Button);
	vLocalCWndCollection.Add(&m_SubFunction3Button);
	vLocalCWndCollection.Add(&m_SubFunction3Display);
	vLocalCWndCollection.Add(&m_Function5Button);
	vLocalCWndCollection.Add(&m_SubFunction7Button);
	vLocalCWndCollection.Add(&m_SubFunction6Display);
	vLocalCWndCollection.Add(&m_SubFunction6Button);
	vLocalCWndCollection.Add(&m_SubFunction5Display);
	vLocalCWndCollection.Add(&m_SubFunction5Button);
	vLocalCWndCollection.Add(&m_Calibrating);
	vLocalCWndCollection.Add(&m_XRaysOn);
	vLocalCWndCollection.Add(&m_Function3Button);
	vLocalCWndCollection.Add(&m_Background);
	vLocalCWndCollection.Add(&m_Function4Button);
	vLocalCWndCollection.Add(&m_Function2Button);
	vLocalCWndCollection.Add(&m_SubFunction7Display);
	vLocalCWndCollection.Add(&m_DialogTitleStaticText);
	vLocalCWndCollection.Add(&m_EncoderCountsOnLastTrigger);
	vLocalCWndCollection.Add(&m_Function1Button);
	vLocalCWndCollection.Add(&m_EncoderCountsOnLastTriggerLabel3);
	vLocalCWndCollection.Add(&m_Function3Display);
	vLocalCWndCollection.Add(&m_SubFunction4Button);
	vLocalCWndCollection.Add(&m_SubFunction4Display);
	vLocalCWndCollection.Add(&m_SubFunction8Button);
	vLocalCWndCollection.Add(&m_SubFunction8Display);
	vLocalCWndCollection.Add(&m_YellowNoticeLabel);
}

void CSystemValuesDialog::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	m_Background.SetWindowPos(NULL,cMainBackgroundLeft,
		cMainBackgroundTop,cMainBackgroundLength,
		cMainBackgroundHeight,SWP_NOZORDER);

	CString TempText;
		TempText.LoadString(IDS_Inches);
	vUnitsString = TempText;
	SetDlgItemText(IDC_MetricDisplay,vUnitsString);
	vUpdateDisplayTimerHandle = 56;
	vUpdateEncoderRateTimerHandle = 57;
	vSubFunction5ButtonEnable = true;
	vSubFunction6ButtonEnable = true;

	vBulkProductMode = vLocalConfigurationData->vBulkProductMode;
	vContinuousFeedContainerRate = vLocalConfigurationData->vContinuousFeedContainerRate;
	vContinuousFeedEncoderRate = vLocalConfigurationData->vContinuousFeedEncoderRate;
	vOriginalEncoderDivider = vLocalConfigurationData->vEncoderDivider;
	vOriginalEncoderDividerForPoint4mmMode = vLocalConfigurationData->vEncoderDividerForPoint4mmMode;
	vEncoderTolerance = vLocalConfigurationData->vEncoderTolerance;
	vNumberOfEncoderRateReversalsForError = vLocalConfigurationData->vNumberOfEncoderRateReversalsForError;
	vPixelsPerUnit = vLocalConfigurationData->vPixelsPerUnit;
	vPixelsPerUnitForPoint4mmMode = vLocalConfigurationData->vPixelsPerUnitForPoint4mmMode;
	vPipeLineFlowRate = vLocalConfigurationData->vPipeLineFlowRate;
	vUsePipeFlowInput = vLocalConfigurationData->vUsePipeFlowInput;
	vSystemBodyTriggerToXRayBeam = vLocalConfigurationData->vSystemBodyTriggerToXRayBeam;
	vSystemEjectorPositionOffset = vLocalConfigurationData->vSystemEjectorPositionOffset;
	for (BYTE TempLoop = 0; TempLoop < cNumberOfEjectors; TempLoop++)
		vSystemEjectorDistance[TempLoop] = vLocalConfigurationData->vSystemEjectorDistance[TempLoop];

	vUseRealEncoder = vLocalConfigurationData->vUseRealEncoder;
	vEncoderDividerChanged = false;					// if encoder divider changed in this function, dont change back for encoder calibration

	if (vGlobalScanTracType == cCaseInspectorScanTrac) //uses 1.6mm detectors which are 64 pixels in 4 inches
	{
		vMaximumSuggestedPPI = 21;
		vMinimumSuggestedPPI = 11;
	}

	vNeedToCalibrateEncoder = vLocalConfigurationData->vNeedToCalibrateEncoder;

	TempText.LoadString(IDS_EncoderCountsOnLastTrigger);
	SetDlgItemText(IDC_EncoderCountsOnLastTriggerLabel3,TempText);

	TempText.LoadString(IDS_XRAYON);
	SetDlgItemText(IDC_XRaysOn,TempText);
	if ((vGlobalScanTracType == cLegatoScanTrac) || (vGlobalScanTracType == cFermataScanTrac))//a pipeline
	{
		vEncoderTolerance = 0;
		vNumberOfEncoderRateReversalsForError = 0;

		vSubFunction5ButtonEnable = false;	//m_SubFunction5Button.EnableWindow(false);
		vSubFunction6ButtonEnable = false;	//m_SubFunction6Button.EnableWindow(false);
		m_SubFunction7Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction7Button.GetRectToInvalidateArrow());

		SetDlgItemText(IDC_Function3Button, _T("ScanTrac Type"));
	}
	else
	{
//		m_CalibrateEncoderWithoutXRays.ShowWindow(SW_SHOW);
	}

	UpdateButtons();
	UpdateDisplay();

	int TempTimerResult = SetTimer(vUpdateDisplayTimerHandle,500,NULL);
	if (!TempTimerResult)
		ReportErrorMessage("Error-Display Timer Failed",cEMailInspx,32000);

	this->SetWindowText(_T("Conveyor"));
	::SetupMenu(vLocalCWndCollection);
}

void CSystemValuesDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSystemValuesDialog)
	DDX_Control(pDX, IDC_SubFunction9Display, m_SubFunction9Display);
	DDX_Control(pDX, IDC_SubFunction9Button, m_SubFunction9Button);
	DDX_Control(pDX, IDC_SubFunction2Display, m_SubFunction2Display);
	DDX_Control(pDX, IDC_SubFunction2Button, m_SubFunction2Button);
	DDX_Control(pDX, IDC_SubFunction1Display, m_SubFunction1Display);
	DDX_Control(pDX, IDC_SubFunction1Button, m_SubFunction1Button);
	DDX_Control(pDX, IDC_SubFunction3Button, m_SubFunction3Button);
	DDX_Control(pDX, IDC_SubFunction3Display, m_SubFunction3Display);
	DDX_Control(pDX, IDC_Function5Button, m_Function5Button);
	DDX_Control(pDX, IDC_SubFunction7Button, m_SubFunction7Button);
	DDX_Control(pDX, IDC_SubFunction6Display, m_SubFunction6Display);
	DDX_Control(pDX, IDC_SubFunction6Button, m_SubFunction6Button);
	DDX_Control(pDX, IDC_SubFunction5Display, m_SubFunction5Display);
	DDX_Control(pDX, IDC_SubFunction5Button, m_SubFunction5Button);
	DDX_Control(pDX, IDC_Calibrating, m_Calibrating);
	//	DDX_Control(pDX, IDC_CalibrateEncoderWithoutXRays, m_CalibrateEncoderWithoutXRays);
	DDX_Control(pDX, IDC_XRaysOn, m_XRaysOn);
	DDX_Control(pDX, IDC_Function3Button, m_Function3Button);
	DDX_Control(pDX, IDC_Background, m_Background);
	DDX_Control(pDX, IDC_Function4Button, m_Function4Button);
	DDX_Control(pDX, IDC_Function2Button, m_Function2Button);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_SubFunction7Display, m_SubFunction7Display);
	DDX_Control(pDX, IDC_DialogTitleStaticText1, m_DialogTitleStaticText);
	DDX_Control(pDX, IDC_EncoderCountsOnLastTrigger3, m_EncoderCountsOnLastTrigger);
	DDX_Control(pDX, IDC_Function1Button, m_Function1Button);
	DDX_Control(pDX, IDC_EncoderCountsOnLastTriggerLabel3, m_EncoderCountsOnLastTriggerLabel3);
	DDX_Control(pDX, IDC_Function3Display, m_Function3Display);
	DDX_Control(pDX, IDC_SubFunction4Button, m_SubFunction4Button);
	DDX_Control(pDX, IDC_SubFunction4Display, m_SubFunction4Display);
	DDX_Control(pDX, IDC_SubFunction8Button, m_SubFunction8Button);
	DDX_Control(pDX, IDC_SubFunction8Display, m_SubFunction8Display);
	DDX_Control(pDX, IDC_YellowNoticeLabel, m_YellowNoticeLabel);
}


BEGIN_MESSAGE_MAP(CSystemValuesDialog, CDialog)
	//{{AFX_MSG_MAP(CSystemValuesDialog)
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
//	ON_BN_CLICKED(IDC_CalibrateEncoderWithoutXRays, OnCalibrateEncoderWithoutXRays)
	ON_BN_CLICKED(IDC_SubFunction9Button, OnSubFunction9Button)
	//}}AFX_MSG_MAP
	ON_STN_CLICKED(IDC_DialogTitleStaticText1, &CSystemValuesDialog::OnDialogtitlestatictext1)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSystemValuesDialog message handlers

void CSystemValuesDialog::OnFunction1Button() 
{
	// Exit to Main Menu
	if ((!vChangeMade) || (AbortSetupChanges()))
	{
		RestoreValues();
		PrepareToExit();
		CDialog::EndDialog(10);
	}
}

void CSystemValuesDialog::OnFunction2Button() 
{
	//Exit button pressed
	if ((!vChangeMade) || (KeepSetupChanges()))
	{
		if (vContinuousFeedEncoderRate != vLocalConfigurationData->vContinuousFeedEncoderRate)
		{
			vLocalConfigurationData->vContinuousFeedEncoderRate = vContinuousFeedEncoderRate;
			if ((vLocalSystemData->vFPGASimulateEncoderRate) || (vLocalConfigurationData->vContinuousFeedEncoderRate))
			{
				if ((vLocalSystemData->vSystemRunMode == cRunningSystemMode) || (vLocalSystemData->vSystemRunMode == cAutoSetupRunningSystem))
					vMainWindowPointer->SendConveyorOnOffToUController(0);
				vMainWindowPointer->TurnSimulatedEncoderOn();
			}
			else
			{
				vMainWindowPointer->TurnSimulatedEncoderOff();
				if ((vLocalSystemData->vSystemRunMode == cRunningSystemMode) || (vLocalSystemData->vSystemRunMode == cAutoSetupRunningSystem))
					vMainWindowPointer->SendConveyorOnOffToUController(1);
			}
		}
		if (vLocalConfigurationData->vContinuousFeedContainerRate != vContinuousFeedContainerRate)
		{
			vLocalConfigurationData->vContinuousFeedContainerRate = vContinuousFeedContainerRate;
			if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)
				vMainWindowPointer->TurnSimulatedContainersOn();
		}

		if ((vGlobalScanTracType != cLegatoScanTrac) && (vGlobalScanTracType != cFermataScanTrac))//not a pipeline
			vLocalConfigurationData->vUsePipeFlowInput = false;

		if (vLocalConfigurationData->vUsePipeFlowInput != vUsePipeFlowInput)
		{
			vLocalConfigurationData->vUsePipeFlowInput = vUsePipeFlowInput;
			vMainWindowPointer->SetUsePipeFlowInput(vLocalConfigurationData->vUsePipeFlowInput);
			vLocalSystemData->vCurrentProductChanged = true;
		}
		vLocalConfigurationData->vUseRealEncoder = vUseRealEncoder;

		//changed ConfigurationData copy so if done and saving don't need to do anything as that is already the new value
		//vLocalConfigurationData->vEncoderDivider = vEncoderDivider;
		//vLocalConfigurationData->vEncoderDividerForPoint4mmMode = vEncoderDividerForPoint4mmMode;
		vLocalConfigurationData->vEncoderTolerance = vEncoderTolerance;
		vLocalConfigurationData->vNeedToCalibrateEncoder = vNeedToCalibrateEncoder;
		vLocalConfigurationData->vSystemBodyTriggerToXRayBeam = vSystemBodyTriggerToXRayBeam;
		vLocalConfigurationData->vSystemEjectorPositionOffset = vSystemEjectorPositionOffset;
		for (BYTE TempLoop = 0; TempLoop < cNumberOfEjectors; TempLoop++)
			vLocalConfigurationData->vSystemEjectorDistance[TempLoop] = vSystemEjectorDistance[TempLoop];

		vLocalConfigurationData->vNumberOfEncoderRateReversalsForError = vNumberOfEncoderRateReversalsForError;

		if (vLocalConfigurationData->vBulkProductMode != vBulkProductMode)
		{
			vLocalConfigurationData->vBulkProductMode = vBulkProductMode;
			if (vBulkProductMode)
				vLocalSystemData->vCurrentConveyorOutput = vLocalSystemData->vCurrentConveyorOutput | 0x100; //set bulk mode bit
			else
				vLocalSystemData->vCurrentConveyorOutput = vLocalSystemData->vCurrentConveyorOutput & 0xFFEF;

			vMainWindowPointer->Send0x9ACommand();

			if (vBulkProductMode)
			{
				if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)
					vMainWindowPointer->TurnSimulatedContainersOn();

					vLocalConfigurationData->vShapeHandlingProcessIfAtSides = 0; // inspect product at edges
				CNoticeDialog TempNoticeDialog;
				TempNoticeDialog.vNoticeText = "After Saving and Exiting and returning to the Main Menu, you must Exit the ScanTrac Program, and restart it\nto effect the changes. Requires FPGA version 10.86.\nBulk Mode uses a real encoder, FPGA simulates images after correct number of lines.";
				TempNoticeDialog.vType = cNoticeMessage;
				TempNoticeDialog.DoModal();

				vLocalConfigurationData->vShapeHandlingProcessIfAtSides = 0;  //disable slip checking
				//vLocalConfigurationData->vShapeHandlingProcessIfAtTop = 0;
				vLocalConfigurationData->vShapeHandlingProcessIfConveyorStops = 0;
				vLocalConfigurationData->vShapeHandlingProcessIfWrongWidth = 0;
				vLocalConfigurationData->vShapeHandlingProcessIfWrongHeight = 0;
				vLocalConfigurationData->vRadiationShutterInSystem = 0; //disable shutter
				vLocalConfigurationData->vEncoderTolerance = 0;
				vLocalConfigurationData->vNumberOfEncoderRateReversalsForError = 0;
			}
			vLocalConfigurationData->vSystemBodyTriggerToXRayBeam = 0; //changed ScanTrac types, so clear offsets
			vLocalConfigurationData->vSystemEjectorPositionOffset = 0; //changed ScanTrac types, so clear offsets
		}
		//bool TempConvertToMetric = false;
		//bool TempConvertToInches = false;
		bool TempScaleChanged = false;

		//if (vLocalConfigurationData->vUnitsString != vUnitsString)
		//{
		//	CString TempText;
		//	TempText.LoadString(IDS_Inches);
		//	if (vUnitsString == TempText)
		//		TempConvertToInches = true;
		//	else
		//		TempConvertToMetric = true;
		//}
		//else
		if (vLocalConfigurationData->vPixelsPerUnit != vPixelsPerUnit)
			TempScaleChanged = true;
		if (vLocalConfigurationData->vPixelsPerUnitForPoint4mmMode != vPixelsPerUnitForPoint4mmMode)
			TempScaleChanged = true;

		vLocalConfigurationData->vPipeLineFlowRate = vPipeLineFlowRate;

		CString TempText;
		TempText.LoadString(IDS_Inches);
		//if (vUnitsString == TempText)
		//	vLocalConfigurationData->vUseMetric = 0;
		//else
		//	vLocalConfigurationData->vUseMetric = 1;
		//vGlobalUseMetric = vLocalConfigurationData->vUseMetric;
		vLocalConfigurationData->vPixelsPerUnit = vPixelsPerUnit;
		vLocalConfigurationData->vPixelsPerUnitForPoint4mmMode = vPixelsPerUnitForPoint4mmMode;
		vLocalConfigurationData->vUnitsString = vUnitsString;
		vGlobalPixelsPerUnit = vLocalConfigurationData->vPixelsPerUnit;
		if (vGlobalCurrentProduct)
		if (vGlobalCurrentProduct->vUsingXScanPoint4mmDetectors)
			vGlobalPixelsPerUnit = vLocalConfigurationData->vPixelsPerUnitForPoint4mmMode;

		if (vLocalConfigurationData->vPixelsPerDetector == 128) //uses .4mm detectors which are 128 pixels in 2 inches
			vGlobalPixelsPerUnitInHeight = 64;
		else
		if (vGlobalScanTracType == cCaseInspectorScanTrac) //uses 1.6mm detectors which are 64 pixels in 4 inches
			vGlobalPixelsPerUnitInHeight = 16;
		else
			vGlobalPixelsPerUnitInHeight = 32; //uses .8mm detectors which are 64 pixels in 2 inches
		
		//if (TempConvertToInches)
		//	ConvertToInches();
		//if (TempConvertToMetric)
		//	ConvertToMetric();
		if (TempScaleChanged)
			ChangeScale();

		if (vGlobaluCSerialPort)
			vGlobaluCSerialPort->SendEncoderDividerTouC();

		PrepareToExit();
		CDialog::EndDialog(true);
	}
}

void CSystemValuesDialog::OnFunction3Button() 
{  
// check for mode Function3Button is in either Stop Calibrating or Calibrate
// if in stop calibrating, stop calibration
	CString TempString = " ";
	GetDlgItemText(IDC_Function3Button,TempString);
	if (TempString == "Stop Calibrating")
	{
		if ((vCalibrateEncoderMethod == cCalXrayOffYesProduction) || (vLocalSystemData->vTestingTrigger))
		{
			vCalibrateEncoderMethod = cNotCalibrating;
//			SetDlgItemText(IDC_Function3Button,"Calibrate Encoder");
			CalibrateEncoderWithoutXRaysYesProductionSelection(); //kjh wasOnCalibrateEncoderWithoutXRays();
		}
		else if ((vCalibrateEncoderMethod == cCalXrayOnNoProduction) || (vCalibratingWithXRays))
		{
			vCalibrateEncoderMethod = cNotCalibrating;
			CalibrateEncoderWithXRaysNoProductionSelection(); // equivalent was Function3
		}		
		UpdateButtons();
		UpdateDisplay();
	}
	else 
	if (TempString == "Calibrate Encoder")
	{
		// once started lock out this button until SelectTypeOfEncoder is displayed, timer set also calls.
		if (vLockOutFunction3Button == false)
		{
			vLockOutFunction3Button = true;
			// check if encoder divider is 4, if not change to 4. Unless it has been changed in this object
			if ((vEncoderDividerChanged == false) && (vLocalConfigurationData->vEncoderDivider != 4) && ((vGlobalScanTracType == cSoloScanTrac) || (vGlobalScanTracType == cSoloScanTrac))) 
			{
				vEncoderDividerChanged = true;
				vLocalConfigurationData->vEncoderDivider = 4;

				if (vGlobaluCSerialPort)
					vGlobaluCSerialPort->SendEncoderDividerTouC();

				UpdateDisplay();

				int TempTimerResult = SetTimer(vUpdateEncoderRateTimerHandle, 2000, NULL);	//Set Timer for 2 seconds for update of Encoder Output Frequency
				if (!TempTimerResult)
					ReportErrorMessage("Error-Encoder New Value Wait Timer Failed",cEMailInspx,32000);
			//	UpdateDisplay();
			}
			else
			{
				SelectTypeOfEncoder();
			}
		}
	}
}

void CSystemValuesDialog::SelectTypeOfEncoder()
{
	// check encoder ok
	if (vLocalSystemData->vActualEncoderRate > 5000)
	{	// notice, not ok on encoder rate and exit
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\nEncoder is " + dtoa(vLocalSystemData->vActualEncoderRate,2) + " Hz\n" + " Maximum should be 5000 Hz \n Increase Encoder Divider";
		TempNoticeDialog.vType = cInformationMessage;
		TempNoticeDialog.DoModal();	
	}
	else
	{
		// kjh here something like below
		CSelectItemDialog ISelectItemDialog;  
		//Set dialog box data titles and number value
		ISelectItemDialog.vTitleString = "Select Encoder Calibration Method";
		ISelectItemDialog.vNumberOfItems = 2;
		ISelectItemDialog.m_DialogTitleStaticText2 = "";
		ISelectItemDialog.m_DialogTitleStaticText3 = "";
		ISelectItemDialog.m_DialogTitleStaticText4 = "";
		ISelectItemDialog.m_DialogTitleStaticText5 = "";
		ISelectItemDialog.vBackgroundColor = cGreen;
		ISelectItemDialog.vGreenItem = 2;
		ISelectItemDialog.vShowExit = false;
		ISelectItemDialog.vInAutoSetup = false;
		ISelectItemDialog.vStayTillDecissionSetup = false;

		ISelectItemDialog.vItems[0] = "NO Production";
		ISelectItemDialog.vItems[1] = "With Production Running (best)";

		//Pass control to dialog box and display
		vCalibrateEncoderMethod = cNotCalibrating;
		int nResponse = ISelectItemDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		vLockOutFunction3Button = false;		// end button lockout
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			if (ISelectItemDialog.vItemNumberSelected == 1) // do old calibration -- Calibrate Encoder, with x-rays, No production
			{
#ifdef _DEBUG
			CNoticeDialog TempNoticeDialog;
			CString TempText("\n\nScanTrac was compiled in Debug Mode.\n Calibrate with X-rays does not work.");
			TempNoticeDialog.vNoticeText = TempText;
			TempNoticeDialog.vType = cInformationMessage;
			TempNoticeDialog.DoModal();
#else
// add this to function CalibrateEncoderWithXRaysNoProductionSelection --	vCalibrateEncoderMethod = cCalXrayOnNoProduction;		//enum {cNotCalibrating, cCalXrayOnNoProduction, cCalXrayOffYesProduction};
				SetDlgItemText(IDC_Function3Button, _T("Stop Calibrating"));
				CalibrateEncoderWithXRaysNoProductionSelection();
#endif
			}
			else
			if (ISelectItemDialog.vItemNumberSelected == 2)// do old calibration -- Calibrate Encoder, without x-rays, yes production
			{
// add this to function CalibrateEncoderWithoutXRaysYesProductionSelection --	vCalibrateEncoderMethod = cCalXrayOffYesProduction;		//enum {cNotCalibrating, cCalXrayOnNoProduction, cCalXrayOffYesProduction};
				SetDlgItemText(IDC_Function3Button,_T("Stop Calibrating"));
				CalibrateEncoderWithoutXRaysYesProductionSelection();
			}
			UpdateButtons();
			UpdateDisplay();
		}
		else 
		if (nResponse == 10)
		{
			//Main Menu button pressed
			Sleep(1); //is the equivelent of a yield statement;
			RestoreValues();
			PrepareToExit();
			CDialog::EndDialog(10);
		}
	}
}

void CSystemValuesDialog::OnFunction4Button() 
{
	// Cancel
	if (vChangeMade)
	if (AbortSetupChanges())
	{
		RestoreValues();
		PrepareToExit();
		CDialog::OnCancel();
	}
}

void CSystemValuesDialog::OnFunction5Button() 
{
	if (vNeedToCalibrateEncoder)
	{
		if ((vGlobalScanTracType == cLegatoScanTrac) || (vGlobalScanTracType == cFermataScanTrac))//a pipeline
		{
			CYesNoDialog TempYesNoDialog;
			TempYesNoDialog.vNoticeText = _T("You should Select ScanTrac Type\nby presseing ScanTrac Type\nIf a special situation does not allow selection,\n you can turn off Type Required Notice.");
			TempYesNoDialog.vYesButtonText = "Make No Change";
			TempYesNoDialog.vNoButtonText = "Clear Type Required";
			TempYesNoDialog.vQuestionType = cConfirmSave;
			int TempResult = TempYesNoDialog.DoModal();
			//if user said OK
			if (TempResult != IDOK)
			{
				vNeedToCalibrateEncoder = false;
				vLocalConfigurationData->vCalibratedPixelsPerUnit = 0;
				UpdateDisplay();
				SetChangeMade();
			}
		}
		else
		{
			CYesNoDialog TempYesNoDialog;
			TempYesNoDialog.vNoticeText = _T("You should Calibrate the Encoder\nby presseing Calibrate Encoder\nIf a special situation does not allow calibration,\n you can turn off Encoder Calibration Required Notice.");
			TempYesNoDialog.vYesButtonText = "Make No Change";
			TempYesNoDialog.vNoButtonText = "Clear Cal Required";
			TempYesNoDialog.vQuestionType = cConfirmSave;
			int TempResult = TempYesNoDialog.DoModal();
			//if user said OK
			if (TempResult != IDOK)
			{
				vNeedToCalibrateEncoder = false;
				vLocalConfigurationData->vCalibratedPixelsPerUnit = 0;
				UpdateDisplay();
				SetChangeMade();

				CNoticeDialog TempNoticeDialog;
				TempNoticeDialog.vNoticeText = "\n\n\nThe ScanTrac can now Run\nwithout calibrating the encoder.";
				TempNoticeDialog.vType = cNoticeMessage;
				//TempNoticeDialog.vAutoClose = 2000;
				TempNoticeDialog.DoModal();
			}
		}
	}
	else
	{
		vNeedToCalibrateEncoder = true;
		UpdateDisplay();
		SetChangeMade();

		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\n\nThe ScanTrac can NOT Run\nuntil the encoder is calibrated.";
		TempNoticeDialog.vType = cNoticeMessage;
		//TempNoticeDialog.vAutoClose = 2000;
		TempNoticeDialog.DoModal();
	}
}

void CSystemValuesDialog::OnSubFunction1Button() 
{
	if ((vGlobalScanTracType == cLegatoScanTrac) || (vGlobalScanTracType == cFermataScanTrac))// a pipeline
	{
		vUsePipeFlowInput = !vUsePipeFlowInput;
		if (vUsePipeFlowInput)
		{
			CNoticeDialog TempNoticeDialog;
			CString TempText("Wire Pipe Flow Indicator to Opto Module 1, input 4.\nUse Container Trigger 1 Activity Level\nto Invert Signal If Needed.\nSignal should be high when pump on.\nNeeds FPGA Version 10.84 or higher, See OScope Input 14");
			TempNoticeDialog.vNoticeText = TempText;
			TempNoticeDialog.vType = cInformationMessage;
			TempNoticeDialog.DoModal();
		}
		UpdateDisplay();
		SetChangeMade();
	}
	else
	if ((vGlobalScanTracType == cAtempoScanTrac) || (vGlobalScanTracType == cForteScanTrac))
	{
		vUseRealEncoder = !vUseRealEncoder;
		SetChangeMade();
		UpdateButtons();
	}
	else
	{				//ask user to enter distance to each ejector on a Solo
		CNumericEntryDialog INumericEntryDialog;  
		for (BYTE TempLoop = 0; TempLoop < cNumberOfEjectors; TempLoop++)
		if (vLocalConfigurationData->vEjector[TempLoop].vEnabled)
		{
			INumericEntryDialog.vEditString = dtoa(vSystemEjectorDistance[TempLoop], 2);

			//Set dialog box data titles and number value
			INumericEntryDialog.m_DialogTitleStaticText1 = "Enter the distance from the ";
			INumericEntryDialog.m_DialogTitleStaticText2 = "ScanTrac Output Wall to center of Ejector " + dtoa(TempLoop + 1, 0);

			INumericEntryDialog.m_UnitsString = "Units in Inches. Original Value: " + INumericEntryDialog.vEditString;

			INumericEntryDialog.m_DialogTitleStaticText3 = "This is fixed for this ScanTrac";
			INumericEntryDialog.m_DialogTitleStaticText4 = "";

			INumericEntryDialog.vMaxValue = 200;
			INumericEntryDialog.vMinValue = 0;
			INumericEntryDialog.vIntegerOnly = false;
			//Pass control to dialog box and display
			int nResponse = INumericEntryDialog.DoModal();
			//dialog box is now closed, if user pressed select do this
			//if user pressed cancel, do nothing
			if (nResponse == IDOK)
			{
				vSystemEjectorDistance[TempLoop] = ATOF(INumericEntryDialog.vEditString);
				UpdateDisplay();
				SetChangeMade();
			}
		}
	}
}

void CSystemValuesDialog::OnSubFunction2Button() 
{
	if (!vLocalConfigurationData->vContinuousFeedEncoderRate)
	{
		//vEncoderTolerance button pressed
		CNumericEntryDialog INumericEntryDialog;  
		
		//Set dialog box data titles and number value
		INumericEntryDialog.vEditString = dtoa(vEncoderTolerance,0);

		INumericEntryDialog.m_DialogTitleStaticText1 = "";
		INumericEntryDialog.m_DialogTitleStaticText2 = "Encoder Tolerance for Diagnostic Encoder Test";
		INumericEntryDialog.m_DialogTitleStaticText3 = "Setup System";
		INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + 
			INumericEntryDialog.vEditString;

		INumericEntryDialog.m_UnitsString = "Default: 12, 0 = Disabled, Value in Hertz";
		INumericEntryDialog.vMaxValue = 255;
		INumericEntryDialog.vMinValue = 0;
		INumericEntryDialog.vIntegerOnly = true;
		//Pass control to dialog box and display
		int nResponse = INumericEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			vEncoderTolerance = _wtoi(INumericEntryDialog.vEditString);
			UpdateDisplay();
			SetChangeMade();
			//Set dialog box data titles and number value
			if (vEncoderTolerance)
			{
				INumericEntryDialog.vEditString = dtoa(vNumberOfEncoderRateReversalsForError,0);

				INumericEntryDialog.m_DialogTitleStaticText1 = "Enter the Number of Encoder Rate Reversals to ";
				INumericEntryDialog.m_DialogTitleStaticText2 = "Generate Yellow Message";
				INumericEntryDialog.m_DialogTitleStaticText3 = "Setup System";
				INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + 
					INumericEntryDialog.vEditString;

				INumericEntryDialog.m_UnitsString = "Default: 4";
				INumericEntryDialog.vMaxValue = 255;
				INumericEntryDialog.vMinValue = 0;
				INumericEntryDialog.vIntegerOnly = true;
				//Pass control to dialog box and display
				int nResponse = INumericEntryDialog.DoModal();
				//dialog box is now closed, if user pressed select do this
				//if user pressed cancel, do nothing
				if (nResponse == IDOK)
				{
					vNumberOfEncoderRateReversalsForError = _wtoi(INumericEntryDialog.vEditString);
					UpdateDisplay();
					SetChangeMade();
				}
				else 
				if (nResponse == 10)
				{
					//Main Menu button pressed
					Sleep(1); //is the equivelent of a yeild statement;
					RestoreValues();
					PrepareToExit();
					CDialog::EndDialog(10);
				}
			}
		}
		else 
		if (nResponse == 10)
		{
			//Main Menu button pressed
			Sleep(1); //is the equivelent of a yeild statement;
			RestoreValues();
			PrepareToExit();
			CDialog::EndDialog(10);
		}
	}
}

void CSystemValuesDialog::OnSubFunction3Button() 
{
  //Encoder Divider button pressed
	//if (!vLocalConfigurationData->vContinuousFeedEncoderRate)
	if ((vGlobalCurrentProduct) && (vGlobalCurrentProduct->vUsingXScanPoint4mmDetectors))
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\nThe Current Product is using X-Scan .4mm mode.\nThe encoder must be calibrated in .8 mm mode.\nSelect a .8 mm mode product to calibrate encoder.\n.4 mm mode automatically adjusts encoder parameters.";
		TempNoticeDialog.vType = cErrorMessage;
		TempNoticeDialog.DoModal();
	}
	else
	{
		CNumericEntryDialog INumericEntryDialog;  
		
		//Set dialog box data titles and number value
		INumericEntryDialog.vEditString = dtoa(vLocalConfigurationData->vEncoderDivider,0);

		INumericEntryDialog.m_DialogTitleStaticText1 = "";
		INumericEntryDialog.m_DialogTitleStaticText2 = "Encoder Divide Factor";
		INumericEntryDialog.m_DialogTitleStaticText3 = "Setup System";
		INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + 
			INumericEntryDialog.vEditString;

		INumericEntryDialog.m_UnitsString = "Default: 4, Minimum: 1";
		INumericEntryDialog.vMaxValue = 255;
		INumericEntryDialog.vMinValue = 1;
		INumericEntryDialog.vIntegerOnly = true;
		//Pass control to dialog box and display
		int nResponse = INumericEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			vLocalConfigurationData->vEncoderDivider = _wtoi(INumericEntryDialog.vEditString);
			if (vLocalConfigurationData->vEncoderDivider == 0)
				vLocalConfigurationData->vEncoderDivider = 4;
			UpdateDisplay();
			SetChangeMade();
			vEncoderDividerChanged = true;			// if encoder divider changed here, dont change back for encoder calibration

			//Send Encoder Divisor
			if (vGlobaluCSerialPort)
				vGlobaluCSerialPort->SendEncoderDividerTouC();

			vMainWindowPointer->ResetCounters();

			if ((vLocalConfigurationData->vUseXScanDetectors) && (vLocalConfigurationData->vScanTracType != cForteScanTrac) && (vLocalConfigurationData->vScanTracType != cCaseInspectorScanTrac))
			{
				//Set dialog box data titles and number value
				INumericEntryDialog.vEditString = dtoa(vLocalConfigurationData->vEncoderDividerForPoint4mmMode,0);

				INumericEntryDialog.m_DialogTitleStaticText1 = "Encoder Divide Factor for .4mm Detector Mode";
				INumericEntryDialog.m_DialogTitleStaticText2 = "0 = Auto = half of .8mm Mode value";
				INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + INumericEntryDialog.vEditString;

				INumericEntryDialog.m_UnitsString = "Default: 0 = Auto, Minimum: 1";
				//Pass control to dialog box and display
				int nResponse = INumericEntryDialog.DoModal();
				//dialog box is now closed, if user pressed select do this
				//if user pressed cancel, do nothing
				if (nResponse == IDOK)
				{
					vLocalConfigurationData->vEncoderDividerForPoint4mmMode = _wtoi(INumericEntryDialog.vEditString);
					if (vLocalConfigurationData->vEncoderDivider == 0)
						vLocalConfigurationData->vEncoderDivider = 4;
					UpdateDisplay();
					SetChangeMade();
					vEncoderDividerChanged = true;			// if encoder divider changed here, dont change back for encoder calibration

					//Send Encoder Divisor
					if (vGlobaluCSerialPort)
						vGlobaluCSerialPort->SendEncoderDividerTouC();

					vMainWindowPointer->ResetCounters();

				}
				else 
				if (nResponse == 10)
				{
					//Main Menu button pressed
					Sleep(1); //is the equivelent of a yeild statement;
					RestoreValues();
					PrepareToExit();
					CDialog::EndDialog(10);
				}
			}
		}
		else 
		if (nResponse == 10)
		{
			//Main Menu button pressed
			Sleep(1); //is the equivelent of a yeild statement;
			RestoreValues();
			PrepareToExit();
			CDialog::EndDialog(10);
		}
	}
}

void CSystemValuesDialog::OnSubFunction4Button() 
{
	if ((vGlobalScanTracType == cLegatoScanTrac) || (vGlobalScanTracType == cFermataScanTrac)) //ScanTrac is a pipeline
	{
		CNumericEntryDialog INumericEntryDialog;  
		
		INumericEntryDialog.vEditString = dtoa(vPipeLineFlowRate, 1);

		//Set dialog box data titles and number value
		CString TempText;
		TempText.LoadString(IDS_Inches);
		if (vUnitsString == TempText)
		{
			INumericEntryDialog.m_DialogTitleStaticText1 = "Enter Pipe Line Flow Rate in Ft/Min";
			INumericEntryDialog.m_DialogTitleStaticText2 = "This will calculate Scan Lines per Inch of flow";
			INumericEntryDialog.m_UnitsString = "Units in Feet/Minute, 0 to enter raw encoder conversion";
		}
		else
		{
			INumericEntryDialog.m_DialogTitleStaticText1 = "Enter Pipe Line Flow Rate in Meters/Min";
			INumericEntryDialog.m_DialogTitleStaticText2 = "This will calculate Scan Lines per MilliMeter of flow";
			INumericEntryDialog.m_UnitsString = "Units in Meters/Minute, 0 to enter raw encoder conversion";
		}

		INumericEntryDialog.m_DialogTitleStaticText3 = "Setup System";
		INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + INumericEntryDialog.vEditString;

		INumericEntryDialog.vMaxValue = 1000;
		INumericEntryDialog.vMinValue = 0;
		INumericEntryDialog.vIntegerOnly = false;
		//Pass control to dialog box and display
		int nResponse = INumericEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			vPipeLineFlowRate = ATOF(INumericEntryDialog.vEditString);
			CString TempText;
			TempText.LoadString(IDS_Inches);
			if (vPipeLineFlowRate)
			if (vUnitsString == TempText)
			{
				//convert ft/min into pulses per inch
				vPixelsPerUnit = vContinuousFeedEncoderRate / ((vPipeLineFlowRate * 12) / 60);
			}
			else
			{
				//convert meters/min into pulses per millimeter
				vPixelsPerUnit = vContinuousFeedEncoderRate / ((vPipeLineFlowRate * 1000) / 60);//metric
			}
			SetChangeMade();
		}
		else 
		{
			if (nResponse == 10)
			{
				//Main Menu button pressed
				Sleep(1); //is the equivelent of a yeild statement;
				RestoreValues();
				PrepareToExit();
				CDialog::EndDialog(10);
			}
		}
		UpdateDisplay();
	}
	else
		vPipeLineFlowRate = 0;


	if (vPipeLineFlowRate == 0)
	if ((vGlobalCurrentProduct) && (vGlobalCurrentProduct->vUsingXScanPoint4mmDetectors))
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\nThe Current Product is using X-Scan .4mm mode.\nThe encoder must be calibrated in .8 mm mode.\nSelect a .8 mm mode product to calibrate encoder.\n.4 mm mode automatically adjusts encoder parameters.";
		TempNoticeDialog.vType = cErrorMessage;
		TempNoticeDialog.DoModal();
	}
	else
	{
		//pulses per unit button pressed
		CNumericEntryDialog INumericEntryDialog;  
		
		//Set dialog box data titles and number value
		INumericEntryDialog.vEditString = dtoa(vPixelsPerUnit,2); //"32";
		INumericEntryDialog.m_DialogTitleStaticText1 = "Encoder Pulses Per Inch";
		INumericEntryDialog.m_DialogTitleStaticText2 = "Target Value: 32 +-5";
		INumericEntryDialog.m_UnitsString = "";
		INumericEntryDialog.m_DialogTitleStaticText3 = "Setup System";
		INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + INumericEntryDialog.vEditString;

		INumericEntryDialog.vMaxValue = 1000;
		INumericEntryDialog.vMinValue = .1;
		INumericEntryDialog.vIntegerOnly = false;
		//Pass control to dialog box and display
		int nResponse = INumericEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			vPixelsPerUnit = ATOF(INumericEntryDialog.vEditString);
			CString TempText;
			TempText.LoadString(IDS_Inches);
			if (vPixelsPerUnit == 0)
			if (vUnitsString == TempText)
				vPixelsPerUnit = 32;
			else
				vPixelsPerUnit = 1.26;
			SetChangeMade();
			if ((vLocalConfigurationData->vUseXScanDetectors) && (vLocalConfigurationData->vScanTracType != cForteScanTrac) && (vLocalConfigurationData->vScanTracType != cCaseInspectorScanTrac))
			{
				//Set dialog box data titles and number value
				INumericEntryDialog.vEditString = dtoa(vPixelsPerUnitForPoint4mmMode,2); //"32";
				INumericEntryDialog.m_DialogTitleStaticText1 = "Encoder Pulses Per Inch for .4mm Mode";
				INumericEntryDialog.m_DialogTitleStaticText2 = "Auto is double the .8 mm Mode value";
				INumericEntryDialog.m_UnitsString = "Default Value: 0 = Auto";
				INumericEntryDialog.m_DialogTitleStaticText3 = "Setup System";
				INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + INumericEntryDialog.vEditString;

				INumericEntryDialog.vMaxValue = 1000;
				INumericEntryDialog.vMinValue = .1;
				INumericEntryDialog.vIntegerOnly = false;
				//Pass control to dialog box and display
				int nResponse = INumericEntryDialog.DoModal();
				//dialog box is now closed, if user pressed select do this
				//if user pressed cancel, do nothing
				if (nResponse == IDOK)
				{
					vPixelsPerUnitForPoint4mmMode = ATOF(INumericEntryDialog.vEditString);
					SetChangeMade();
				}
				else 
				{
					if (nResponse == 10)
					{
						//Main Menu button pressed
						Sleep(1); //is the equivelent of a yeild statement;
						RestoreValues();
						PrepareToExit();
						CDialog::EndDialog(10);
					}
				}
				UpdateDisplay();
			}
		}
		else 
		{
			if (nResponse == 10)
			{
				//Main Menu button pressed
				Sleep(1); //is the equivelent of a yeild statement;
				RestoreValues();
				PrepareToExit();
				CDialog::EndDialog(10);
			}
		}
		UpdateDisplay();
	}
}

void CSystemValuesDialog::OnSubFunction5Button() 
{
	if ((vGlobalScanTracType != cLegatoScanTrac) && (vGlobalScanTracType != cFermataScanTrac))// not a pipeline
	{
		// Continuous Feed Encoder Rate
		CNumericEntryDialog INumericEntryDialog;  
		
		//Set dialog box data titles and number value
		INumericEntryDialog.vEditString = 
			dtoa(vContinuousFeedEncoderRate,0);

		INumericEntryDialog.m_DialogTitleStaticText1 = "Enter the Continuous Feed Encoder Frequency to use";
		INumericEntryDialog.m_DialogTitleStaticText2 = "Use fastest encoder speed allowed for Integration Time";
		INumericEntryDialog.m_DialogTitleStaticText3 = "Setup System";


		INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + INumericEntryDialog.vEditString + ", Product Maximum: " + dtoa(vLocalSystemData->vMaximumEncoderRate,0);

		if (vLocalSystemData->vFPGAVersion9Point0OrHigher)
		{
			INumericEntryDialog.m_UnitsString = "0=Disabled, 50,000 Maximum, Units in Hertz";
			INumericEntryDialog.vMaxValue = 50000;
		}
		else
		{
			INumericEntryDialog.m_UnitsString = "0=Disabled, 2307 Maximum, Units in Hertz";
			INumericEntryDialog.vMaxValue = 2307;
		}
		INumericEntryDialog.vMinValue = 0;
		INumericEntryDialog.vIntegerOnly = true;
		//Pass control to dialog box and display
		int nResponse = INumericEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			vContinuousFeedEncoderRate = _wtoi(INumericEntryDialog.vEditString);
			UpdateDisplay();
			SetChangeMade();
		}
		else 
		if (nResponse == 10)
		{
			//Main Menu button pressed
			Sleep(1); //is the equivelent of a yeild statement;
			RestoreValues();
			PrepareToExit();
			CDialog::EndDialog(10);
		}
	}
	else
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\nThis is set automatically on Pipeline ScanTracs.\nYou cannot change manually.";
		TempNoticeDialog.vType = cErrorMessage;
		TempNoticeDialog.DoModal();
	}
}

void CSystemValuesDialog::OnSubFunction6Button() 
{
	if ((vGlobalScanTracType != cLegatoScanTrac) && (vGlobalScanTracType != cFermataScanTrac))// not a pipeline
	{
		// Continuous Feed Container Rate
		CNumericEntryDialog INumericEntryDialog;  
		
		//Set dialog box data titles and number value
		INumericEntryDialog.vEditString = dtoa(vContinuousFeedContainerRate,0);

		WORD TempContinuousFeedContainerRate = 0;
		if (vGlobalCurrentProduct->vProductImageWidth * vGlobalPixelsPerUnit)
			TempContinuousFeedContainerRate = (WORD)(1.1 * (60.05 * vContinuousFeedEncoderRate / (vGlobalCurrentProduct->vProductImageWidth * vPixelsPerUnit)));

		INumericEntryDialog.m_DialogTitleStaticText1 = "Enter the Continuous Feed Container Rate to use";
		INumericEntryDialog.m_DialogTitleStaticText2 = "Use MAXIMUM rate so images will overlap so inspect all";
		INumericEntryDialog.m_DialogTitleStaticText3 = "Setup System, Original Value: " + INumericEntryDialog.vEditString;
		INumericEntryDialog.m_DialogTitleStaticText4 = "WARNING: " + 
			dtoa(TempContinuousFeedContainerRate,0) + " is MAXIMUM value for this product setup";

		INumericEntryDialog.m_UnitsString = "0=Disabled, Units in Containers Per Minute";
		INumericEntryDialog.vMaxValue = 3000;
		INumericEntryDialog.vMinValue = 0;
		INumericEntryDialog.vIntegerOnly = true;
		//Pass control to dialog box and display
		int nResponse = INumericEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			vContinuousFeedContainerRate = _wtoi(INumericEntryDialog.vEditString);
			UpdateDisplay();
			SetChangeMade();
		}
		else 
		if (nResponse == 10)
		{
			//Main Menu button pressed
			Sleep(1); //is the equivelent of a yeild statement;
			RestoreValues();
			PrepareToExit();
			CDialog::EndDialog(10);
		}
	}
	else
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\nThis is set automatically on Pipeline ScanTracs.\nYou cannot change manually.";
		TempNoticeDialog.vType = cErrorMessage;
		TempNoticeDialog.DoModal();
	}
}

void CSystemValuesDialog::OnSubFunction7Button() 
{
	// Automatically calculate Continuous Feed Container Rate
	if ((vGlobalScanTracType != cLegatoScanTrac) && (vGlobalScanTracType != cFermataScanTrac))// not a pipeline
	{
		if ((vGlobalCurrentProduct) && (vGlobalCurrentProduct->vProductImageWidth))
		{
		WORD TempContinuousFeedContainerRate = (WORD)(1.1 * (60.05 * vContinuousFeedEncoderRate / (vGlobalCurrentProduct->vProductImageWidth * vPixelsPerUnit)));
			CYesNoDialog TempYesNoDialog;
			CString TempString = "\n\nSet Continuous Feed Container Rate to: " +
				dtoa(TempContinuousFeedContainerRate,0) + "\n Based on Product: " +
				*vGlobalCurrentProduct->GetProductName() + "?";
			TempYesNoDialog.vNoticeText = TempString;
			TempYesNoDialog.vYesButtonText = "Use Value";
			TempYesNoDialog.vNoButtonText = "Cancel";
			TempYesNoDialog.vQuestionType = cConfirmSave;
			int TempResult = TempYesNoDialog.DoModal();
			//if user said OK
			if (TempResult == IDOK)
			{
				vContinuousFeedContainerRate = TempContinuousFeedContainerRate;
				UpdateDisplay();
				SetChangeMade();
			}
		}
		else
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "You must Select A Product for this feature";
			TempNoticeDialog.vType = cErrorMessage;
			TempNoticeDialog.DoModal();
		}
	}
}

void CSystemValuesDialog::OnSubFunction8Button() 
{
	if (!vBulkProductMode)
	{
		if (vLocalSystemData->vFPGAVersion10Point86OrHigher)
		{
			vBulkProductMode = !vBulkProductMode;
			CNoticeDialog TempNoticeDialog;
			if ((vGlobalScanTracType == cLegatoScanTrac) || (vGlobalScanTracType == cFermataScanTrac))// a Pipeline
				TempNoticeDialog.vNoticeText = "\n\nVCO to Encoder uses the VCO ouput to the encoder input,\nFPGA simulates images after correct number of lines.\nThis takes images corresponding to pipe flow speed.";
			else
				TempNoticeDialog.vNoticeText = "\n\nBulk Mode uses a real encoder,\nFPGA simulates images after correct number of lines.\nThis takes images corresponding to conveyor speed.";
			TempNoticeDialog.vType = cNoticeMessage;
			TempNoticeDialog.DoModal();
		}
		else
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\n\nRequires FPGA version 10.86 or higher";
			TempNoticeDialog.vType = cInformationMessage;
			TempNoticeDialog.DoModal();
		}
	}
	else
		vBulkProductMode = !vBulkProductMode;
			
	vLocalSystemData->vCurrentProductChanged = true;
	SetChangeMade();
	UpdateDisplay();
	/*
	//English Metric button was pressed.
	CString TempText;
	TempText.LoadString(IDS_Inches);
	if (vUnitsString == TempText)
	{
		TempText.LoadString(IDS_Millimeters);
		vUnitsString = TempText;
	}
	else
	{
		vUnitsString = TempText;
	}
	SetChangeMade();
	UpdateDisplay();
	*/
}

BOOL CSystemValuesDialog::PreTranslateMessage(MSG* pMsg) 
{
	CWnd * TempWindow = NULL;
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == cUpdateTitleBarMessage)
	{
		CWnd *TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText1);
		if (TempWindow)
			TempWindow->Invalidate(false);
		return true;  //true indicates it has been handled, so do not process
	}
	else
	if (pMsg->message == cOffsetCalibrationNextStep)
	{
		CString TempText;
		TempText.LoadString(IDS_Wait);
		m_Calibrating.ShowWindow(SW_SHOW);
		if (vLocalSystemData->vCalibrateUsingDAC)
			SetDlgItemText(IDC_Calibrating,TempText + " " + 
				dtoa(vLocalSystemData->vCalibrationImageNumber ,0));
		else
			SetDlgItemText(IDC_Calibrating,TempText + " " + 
				dtoa(vLocalSystemData->vCalibrationImageNumber + 12 ,0));
		return true;
	}
	else
	if (pMsg->message == cNewImageReadyToDisplay)
	{
		//newimage newframe to display on screen
		if (vLocalSystemData->vImageNumberToDisplay != vOldImageNumberToDisplay)
		if ((vLocalSystemData->vITIPCDig->vOriginalImage) && (vLocalSystemData->vITIPCDig->vOriginalImage->vGreyImage))
		{
			vOldImageNumberToDisplay = vLocalSystemData->vImageNumberToDisplay;
			if (vCalibratingWithXRays)
			{
				if (vLocalSystemData->vLogFile.vLogSerialData)
					vLocalSystemData->vLogFile.WriteToLogFile("Calibrate Encoder-Process Calibrate Encoder Image",cDebugMessage);

				CalculateCalibration();
			}
		}
		return true;
	}
	if (pMsg->message == WM_KEYDOWN)
	{
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
			case 36:  //Home key, Screen Capture
				vGlobalShiftKeyDown = true;
				PostThreadMessage(vGlobalCallingThreadID,cHiddenKeyPressed, 0, 0x61);
			break;
			case cPageDown: // Help
				PostThreadMessage(vGlobalCallingThreadID,
					cShowHelp, 11, 0);//setup conveyor
				{
					TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText1);
					if (TempWindow)
						TempWindow->Invalidate(false);
				}
			break;
			case 27: // Escape
				::PostThreadMessage(vGlobalCallingThreadID,	cStartChatWindowMessage, 0,0);
			break;	
		}
		return true;  //true indicates it has been handled, so do not process
	}
	return CDialog::PreTranslateMessage(pMsg);
}

HBRUSH CSystemValuesDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	HWND TempDialogHwnd = this->m_hWnd;
	int TextSize;
		pDC->SetAttribDC(pDC->m_hDC);
	
	if (pWnd->GetDlgCtrlID() == IDC_DialogTitleStaticText1)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DialogTitleStaticText, 8);  //5 is medium large
		pDC->SetTextColor(vGlobalDialogTitleColor);
	}

	if ((pWnd->GetDlgCtrlID() == IDC_Calibrating) || (pWnd->GetDlgCtrlID() == IDC_XRaysOn))
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Calibrating, 5);
		pDC->SetTextColor(cYellow);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vRedBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_XRaysOn)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_XRaysOn, 7);
		pDC->SetTextColor(cYellow);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vRedBrush;
	}

//	if (pWnd->GetDlgCtrlID() == IDC_CalibrateEncoderWithoutXRays)
//	{
//		if (vLocalSystemData->vTestingTrigger)
//		{
//			pDC->SetBkMode(TRANSPARENT);
//			pDC->SetTextColor(cButtonTextColor);
//			return vLocalSystemData->vRedBrush;
//		}
//		/*
//		else
//		if (vNeedToCalibrateEncoder)
//		{
//			pDC->SetBkMode(TRANSPARENT);
//			pDC->SetTextColor(cButtonTextColor);
//			return vLocalSystemData->vGreenBrush;
//		}
//		*/
//	}
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
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function3Button, 5);  //5 is medium large
		if (vCalibratingWithXRays)
//		if ((vCalibrateEncoderMethod == cCalXrayOnNoProduction) || (vCalibrateEncoderMethod == cCalXrayOffYesProduction))
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cButtonTextColor);
			return vLocalSystemData->vRedBrush;
		}
		else if (vLocalSystemData->vTestingTrigger)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cButtonTextColor);
			return vLocalSystemData->vRedBrush;
		}
		else
		if ((vNeedToCalibrateEncoder) || (!vSystemBodyTriggerToXRayBeam) || (!vSystemEjectorPositionOffset))
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cButtonTextColor);
			return vLocalSystemData->vGreenBrush;
		}
	}

	if (pWnd->GetDlgCtrlID() == IDC_YellowNoticeLabel)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_YellowNoticeLabel, 5);	//not button but is two lines
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBlack);
		return vLocalSystemData->vYellowBrush;
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
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function2Button, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
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
		if (vNeedToCalibrateEncoder)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cButtonTextColor);
			return vLocalSystemData->vYellowBrush;
		}
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
		if ((vGlobalScanTracType != cLegatoScanTrac) && (vGlobalScanTracType != cFermataScanTrac))// not a pipeline
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cButtonTextColor);
			return vLocalSystemData->vYellowBrush;
		}
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction5Button)
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction5Button, 5); 
		if ((vGlobalScanTracType != cLegatoScanTrac) && (vGlobalScanTracType != cFermataScanTrac))// not a pipeline
		{
			pDC->SetBkMode(TRANSPARENT);
			if (vSubFunction5ButtonEnable == true)
				pDC->SetTextColor(cButtonTextColor);
			else
				pDC->SetTextColor(cGray);
			return vLocalSystemData->vYellowBrush;
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
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction6Button, 5); 
		if ((vGlobalScanTracType != cLegatoScanTrac) && (vGlobalScanTracType != cFermataScanTrac))// not a pipeline
		{
			pDC->SetBkMode(TRANSPARENT);
			if (vSubFunction6ButtonEnable == true)
				pDC->SetTextColor(cButtonTextColor);
			else
				pDC->SetTextColor(cGray);
			return vLocalSystemData->vYellowBrush;
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
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction7Button, 5);  //5 is medium large
		if ((vGlobalScanTracType != cLegatoScanTrac) && (vGlobalScanTracType != cFermataScanTrac))// not a pipeline
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cButtonTextColor);
			return vLocalSystemData->vYellowBrush;
		}
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction8Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction8Button, 5);  //5 is medium large
		if (vBulkProductMode)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cBlack);
			return vLocalSystemData->vYellowBrush;
		}
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Background) 
	{
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_EncoderCountsOnLastTrigger3) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_EncoderCountsOnLastTrigger, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Function3Display)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function3Display, 5);	//not button but is two lines
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_EncoderCountsOnLastTriggerLabel3)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_EncoderCountsOnLastTriggerLabel3, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
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
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction8Display) 
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction8Display, 5);
	if (pWnd->GetDlgCtrlID() == IDC_Function3Display) 
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Function3Display, 5);
	return hbr;
}

void CSystemValuesDialog::UpdateDisplay()
{
	if (vBulkProductMode)
	{
		if ((vGlobalScanTracType == cLegatoScanTrac) || (vGlobalScanTracType == cFermataScanTrac))//a pipeline
		{
			SetDlgItemText(IDC_SubFunction8Button, _T("Use Continuous Feed"));
			SetDlgItemText(IDC_SubFunction8Display, _T("Using VCO to Encoder"));
		}
		else
		{
			SetDlgItemText(IDC_SubFunction8Button, _T("Use Real Trigger Sensor"));
			SetDlgItemText(IDC_SubFunction8Display, _T("Using Bulk Mode"));
		}
	}
	else
	{
		if ((vGlobalScanTracType == cLegatoScanTrac) || (vGlobalScanTracType == cFermataScanTrac))//a pipeline
		{
			SetDlgItemText(IDC_SubFunction8Button, _T("Use VCO to Encoder"));
			SetDlgItemText(IDC_SubFunction8Display, _T("Using Continuous Feed"));
		}
		else
		{
			SetDlgItemText(IDC_SubFunction8Button, _T("Use\nBulk\nMode"));
			SetDlgItemText(IDC_SubFunction8Display, _T("Using Real Trigger"));
		}
	}

	if ((vGlobalScanTracType == cLegatoScanTrac) || (vGlobalScanTracType == cFermataScanTrac))//a pipeline
	{
		SetDlgItemText(IDC_Function3Button, _T("ScanTrac Type"));
		SetDlgItemText(IDC_Function5Button, _T("Type Required"));
	}
	else
	{
		SetDlgItemText(IDC_Function3Button, _T("Calibrate Encoder"));
		SetDlgItemText(IDC_Function5Button, _T("Calibration Required"));
	}

	CString TempDisplayString = "Trigger to Beam Distance: " + dtoa(vSystemBodyTriggerToXRayBeam,2) +  ", Ejector Offset: " + dtoa(vSystemEjectorPositionOffset,2);

	if (vSystemEjectorPositionOffset)
	{
		for (BYTE TempLoop = 0; TempLoop < cNumberOfEjectors; TempLoop++)
		if (vLocalConfigurationData->vEjector[TempLoop].vEnabled)
		if (vSystemEjectorDistance[TempLoop])
		{
			if (TempLoop)
				TempDisplayString = TempDisplayString + ",    ";
			else
				TempDisplayString = TempDisplayString + "\n";

			TempDisplayString = TempDisplayString + "Ejector " + dtoa(TempLoop + 1, 0) + ": " + dtoa(vSystemEjectorDistance[TempLoop], 2);
		}
	}
	SetDlgItemText(IDC_Function3Display, TempDisplayString);

	SetDlgItemText(IDC_SubFunction2Display,dtoa(vEncoderTolerance,0) + ", " + dtoa(vNumberOfEncoderRateReversalsForError,0));
	SetDlgItemText(IDC_SubFunction5Display,dtoa(vContinuousFeedEncoderRate,0));
	SetDlgItemText(IDC_SubFunction6Display,dtoa(vContinuousFeedContainerRate,0));

	if ((vGlobalScanTracType == cLegatoScanTrac) || (vGlobalScanTracType == cFermataScanTrac))//a pipeline
	{
		if (vNeedToCalibrateEncoder)
			SetDlgItemText(IDC_Function5Button, _T("Clear Type Requirement"));
		else
			SetDlgItemText(IDC_Function5Button, _T("Set Type Required"));
	}
	else
	{
		if (vNeedToCalibrateEncoder)
			SetDlgItemText(IDC_Function5Button, _T("Remove Cal Requirement"));
		else
			SetDlgItemText(IDC_Function5Button, _T("Set Cal Required"));
	}

	if (((vGlobalScanTracType == cLegatoScanTrac) || (vGlobalScanTracType == cLegatoScanTrac)) && 	(vPipeLineFlowRate))//ScanTrac is a pipeline
		SetDlgItemText(IDC_SubFunction4Display,"Flow: " + dtoa(vPipeLineFlowRate, 1) + " gives: " + dtoa(vPixelsPerUnit,2) + " Encoder Pulses Per Inch");
	else
	{
		if ((vLocalConfigurationData->vUseXScanDetectors) && (vLocalConfigurationData->vScanTracType != cForteScanTrac) && (vLocalConfigurationData->vScanTracType != cCaseInspectorScanTrac))
		{
			if (vPixelsPerUnitForPoint4mmMode)
				SetDlgItemText(IDC_SubFunction4Display, ".8mm : " + dtoa(vPixelsPerUnit,0) + ", .4mm : " + dtoa(vPixelsPerUnitForPoint4mmMode,0));
			else
				SetDlgItemText(IDC_SubFunction4Display, ".8mm : " + dtoa(vPixelsPerUnit,0) + ", .4mm : Auto");
		}
		else
			SetDlgItemText(IDC_SubFunction4Display,dtoa(vPixelsPerUnit,2));
	}

	if ((vLocalConfigurationData->vUseXScanDetectors) && (vLocalConfigurationData->vScanTracType != cForteScanTrac) && (vLocalConfigurationData->vScanTracType != cCaseInspectorScanTrac))
	{
		if (vLocalConfigurationData->vEncoderDividerForPoint4mmMode)
			SetDlgItemText(IDC_SubFunction3Display, ".8mm : " + dtoa(vLocalConfigurationData->vEncoderDivider,0) + ", .4mm : " + dtoa(vLocalConfigurationData->vEncoderDividerForPoint4mmMode,0));
		else
			SetDlgItemText(IDC_SubFunction3Display, ".8mm : " + dtoa(vLocalConfigurationData->vEncoderDivider,0) + ", .4mm : Auto");
	}
	else
		SetDlgItemText(IDC_SubFunction3Display,dtoa(vLocalConfigurationData->vEncoderDivider,0));

	UpdateButtons();
}

void CSystemValuesDialog::SetChangeMade() 
{
	if (!vChangeMade)
	{
		m_Function2Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		SetDlgItemText(IDC_Function2Button, _T("Save\nand Exit"));
		SetDlgItemText(IDC_Function4Button, _T("Cancel"));
		m_Function4Button.ShowWindow(SW_SHOW);
		vChangeMade = true;
	}
}

//void CSystemValuesDialog::ConvertToMetric()
//{
//	vLocalSystemData->vCurrentProductChanged = true;
//	WORD TempNumberOfProducts = vLocalProductCollection->GetSize();
//	CProduct *TempProduct = NULL;
//	for (WORD TempLoop = 0; TempLoop < TempNumberOfProducts;  TempLoop++)
//	{
//		TempProduct = vLocalProductCollection->GetAt(TempLoop);
//		if (TempProduct)
//		{
//			TempProduct->SetEndOfLineTimeOut((float)(TempProduct->vEndOfLineTimeOut * 25.4));
//			TempProduct->SetProductImageWidth((float)(TempProduct->vProductImageWidth * 25.4));
//			TempProduct->SetProductLockOutWidth((float)(TempProduct->vProductLockOutWidth * 25.4));
//			TempProduct->vMaximumImageOverlap = TempProduct->vMaximumImageOverlap * 25.4;
//
//			TempProduct->SetProductBodyTriggerToImageBeltPositionOffset(
//				(float)(TempProduct->vProductBodyTriggerToImageBeltPositionOffset * 25.4));
//
//			TempProduct->SetEdgeLocationBottom((float)(TempProduct->vEdgeLocationBottom * 25.4));
//			TempProduct->SetEdgeLocationHeight((float)(TempProduct->vEdgeLocationHeight * 25.4));
//
//			TempProduct->vProductImageHeightTop = (float)(TempProduct->vProductImageHeightTop * 25.4);
//			TempProduct->vProductImageHeightBottom = 
//				(float)(TempProduct->vProductImageHeightBottom * 25.4);
//
//			for (BYTE TempLoopD = 0; TempLoopD < cNumberOfExternalDetectors; TempLoopD++)
//			{
//				TempProduct->SetExternalDetectorWindowStart(TempLoopD, 
//				(float)(	TempProduct->vExternalDetectorWindowStart[TempLoopD] * 25.4));
//				TempProduct->SetExternalDetectorWindowEnd(TempLoopD, 
//					(float)(TempProduct->vExternalDetectorWindowEnd[TempLoopD] * 25.4));
//			}
//			for (BYTE TempLoopD = 0; TempLoopD < cNumberOfEjectors; TempLoopD++)
//				TempProduct->SetEjectorBeltPositionOffset(TempLoopD,
//				(float)(TempProduct->vEjectorDelayPosition[TempLoopD] * 25.4));
//		}
//	}
//}
//
//void CSystemValuesDialog::ConvertToInches()
//{
//	vLocalSystemData->vCurrentProductChanged = true;
//	WORD TempNumberOfProducts = vLocalProductCollection->GetSize();
//	CProduct *TempProduct = NULL;
//	for (WORD TempLoop = 0; TempLoop < TempNumberOfProducts;  TempLoop++)
//	{
//		TempProduct = vLocalProductCollection->GetAt(TempLoop);
//		if (TempProduct)
//		{
//			TempProduct->SetEndOfLineTimeOut((float)(TempProduct->vEndOfLineTimeOut / 25.4));
//			TempProduct->SetProductImageWidth((float)(TempProduct->vProductImageWidth / 25.4));
//			TempProduct->SetProductLockOutWidth((float)(TempProduct->vProductLockOutWidth / 25.4));
//			TempProduct->vMaximumImageOverlap = TempProduct->vMaximumImageOverlap / 25.4;
//
//			TempProduct->SetProductBodyTriggerToImageBeltPositionOffset(
//				(float)(TempProduct->vProductBodyTriggerToImageBeltPositionOffset / 25.4));
//
//			TempProduct->SetEdgeLocationBottom((float)(TempProduct->vEdgeLocationBottom / 25.4));
//			TempProduct->SetEdgeLocationHeight((float)(TempProduct->vEdgeLocationHeight / 25.4));
//
//			TempProduct->vProductImageHeightTop = (float)(TempProduct->vProductImageHeightTop / 25.4);
//			TempProduct->vProductImageHeightBottom = 
//				(float)(TempProduct->vProductImageHeightBottom / 25.4);
//
//			for (BYTE TempLoopD = 0; TempLoopD < cNumberOfExternalDetectors; TempLoopD++)
//			{
//				TempProduct->SetExternalDetectorWindowStart(TempLoopD, 
//				(float)(	TempProduct->vExternalDetectorWindowStart[TempLoopD] / 25.4));
//				TempProduct->SetExternalDetectorWindowEnd(TempLoopD, 
//					(float)(TempProduct->vExternalDetectorWindowEnd[TempLoopD] / 25.4));
//			}
//			for (BYTE TempLoopD = 0; TempLoopD < cNumberOfEjectors; TempLoopD++)
//				TempProduct->SetEjectorBeltPositionOffset(TempLoopD,
//				(float)(TempProduct->vEjectorDelayPosition[TempLoopD] / 25.4));
//		}
//	}
//}
//
void CSystemValuesDialog::ChangeScale()
{
	vLocalSystemData->vCurrentProductChanged = true;
	WORD TempNumberOfProducts = vLocalProductCollection->GetSize();
	CProduct *TempProduct = NULL;
	for (WORD TempLoop = 0; TempLoop < TempNumberOfProducts;  TempLoop++)
	{
		TempProduct = vLocalProductCollection->GetAt(TempLoop);
		if (TempProduct)
		{
			TempProduct->SetEndOfLineTimeOut(TempProduct->vEndOfLineTimeOut);
			TempProduct->SetProductImageWidth(TempProduct->vProductImageWidth);
			TempProduct->SetProductLockOutWidth(TempProduct->vProductLockOutWidth);

			TempProduct->SetProductBodyTriggerToImageBeltPositionOffset(
				TempProduct->vProductBodyTriggerToImageBeltPositionOffset);

			for (BYTE TempLoopD = 0; TempLoopD < cNumberOfEjectors; TempLoopD++)
				TempProduct->SetEjectorBeltPositionOffset(TempLoopD,
				TempProduct->vEjectorDelayPosition[TempLoopD]);

			for (BYTE TempLoopD = 0; TempLoopD < cNumberOfExternalDetectors; TempLoopD++)
			{
				TempProduct->SetExternalDetectorWindowStart(TempLoopD, TempProduct->vExternalDetectorWindowStart[TempLoopD]);
				TempProduct->SetExternalDetectorWindowEnd(TempLoopD, TempProduct->vExternalDetectorWindowEnd[TempLoopD]);
			}
		}
	}
}

void CSystemValuesDialog::UpdateButtons()
{
//	if ((vGlobalScanTracType != cLegatoScanTrac) && (vGlobalScanTracType != cFermataScanTrac))// not a pipeline
//	{
//		if (vLocalSystemData->vTestingTrigger)
//		{
//			m_CalibrateEncoderWithoutXRays.EnableWindow(true);
//			SetDlgItemText(IDC_CalibrateEncoderWithoutXRays,"Stop Calibrate Without X-Rays");
//		}
//		else
//		{
//			if ((vLocalSystemData->vSystemRunMode == cStoppedSystemMode) && (vGlobalCurrentProduct) && (!vCalibratingWithXRays))
//				m_CalibrateEncoderWithoutXRays.EnableWindow(true);
//			else
//				m_CalibrateEncoderWithoutXRays.EnableWindow(false);
//			SetDlgItemText(IDC_CalibrateEncoderWithoutXRays,"Calibrate Encoder Without X-Rays");
//		}
//	}

	if ((vGlobalScanTracType != cLegatoScanTrac) && (vGlobalScanTracType != cFermataScanTrac))// not a pipeline
	{
		if ((vLocalSystemData->vSystemRunMode == cCalibrateEncoderMode) || 
			(vLocalSystemData->vSystemRunMode == cCalibratingDetectorOffsetMode) ||
			(vLocalSystemData->vTestingTrigger))
		{
//			m_Function3Button.EnableWindow(true);
			SetDlgItemText(IDC_Function3Button, _T("Stop Calibrating"));
		}
		else
		{
//			if ((vLocalSystemData->vSystemRunMode == cStoppedSystemMode) && (!vLocalSystemData->vTestingTrigger))
//				m_Function3Button.EnableWindow(true);
//			else
//				m_Function3Button.EnableWindow(false);
			SetDlgItemText(IDC_Function3Button, _T("Calibrate Encoder"));
		}
//		if (vCalibrateEncoderMethod != cNotCalibrating)
//			SetDlgItemText(IDC_Function3Button,"Stop Calibrating");
//		else
//			SetDlgItemText(IDC_Function3Button,"Calibrate Encoder");
	}
	if ((vGlobalScanTracType == cLegatoScanTrac) || (vGlobalScanTracType == cFermataScanTrac))//a pipeline
	{
		m_SubFunction1Display.ShowWindow(SW_SHOW);
		m_SubFunction1Button.ShowWindow(SW_SHOW);
		if (vUsePipeFlowInput)
		{
			SetDlgItemText(IDC_SubFunction1Display, _T("Using Pipe Flow Input"));
			SetDlgItemText(IDC_SubFunction1Button, _T("Don't Use Pipe Flow Input"));
		}
		else
		{
			SetDlgItemText(IDC_SubFunction1Display, _T("Not Using Pipe Flow Input"));
			SetDlgItemText(IDC_SubFunction1Button, _T("Use Pipe Flow Input"));
		}
	}
	else
	if ((vGlobalScanTracType == cAtempoScanTrac) || (vGlobalScanTracType == cForteScanTrac))
	{
		m_SubFunction1Display.ShowWindow(SW_SHOW);
		m_SubFunction1Button.ShowWindow(SW_SHOW);
		if (vUseRealEncoder)
		{
			SetDlgItemText(IDC_SubFunction1Display, _T("Using Real Encoder"));
			SetDlgItemText(IDC_SubFunction1Button, _T("Use\nMotor Signal"));
		}
		else
		{
			SetDlgItemText(IDC_SubFunction1Display, _T("Using Motor Signal"));
			SetDlgItemText(IDC_SubFunction1Button, _T("Use\nReal Encoder"));
		}
	}
	else
	{
		SetDlgItemText(IDC_SubFunction1Button, _T("Distance to Ejectors"));
		m_SubFunction1Display.ShowWindow(SW_HIDE);
		m_SubFunction1Button.ShowWindow(SW_SHOW);
	}

}

void CSystemValuesDialog::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == vUpdateEncoderRateTimerHandle)
	{
		KillTimer(vUpdateEncoderRateTimerHandle);
		SelectTypeOfEncoder();		
	}
	if (nIDEvent == vUpdateDisplayTimerHandle)
	{
		KillTimer(vUpdateDisplayTimerHandle);

		//if (vOldXRayState != vLocalSystemData->vXRayState)
		//{
		//	vOldXRayState = vLocalSystemData->vXRayState;

		//	if (vLocalSystemData->vLogFile.vLogSerialData)
		//		vLocalSystemData->vLogFile.WriteToLogFile("Conveyor Setup X-Ray Step: " + dtoa(vOldXRayState, 0),cDebugMessage);

		//	CString TempText;
		//	TempText.LoadString(IDS_Wait);
		//	m_Calibrating.ShowWindow(SW_SHOW);
		//	if (vLocalSystemData->vCalibrateUsingDAC)
		//		SetDlgItemText(IDC_Calibrating,TempText + " " + dtoa(vOldXRayState ,0));
		//	else
		//		SetDlgItemText(IDC_Calibrating,TempText + " " + dtoa(vOldXRayState + 12 ,0));

		//}

		if (vLocalSystemData->vSystemRunMode != vOldSystemRunMode)
		{
			vOldSystemRunMode = vLocalSystemData->vSystemRunMode;
			if (vOldSystemRunMode == cStoppedSystemMode)
			{
				m_XRaysOn.ShowWindow(SW_HIDE);
				m_Calibrating.ShowWindow(SW_HIDE);
			}
			UpdateButtons();
		}

		if (vOldXRayOnNotice != vLocalSystemData->vXRayOnNotice)
		{
			vOldXRayOnNotice = vLocalSystemData->vXRayOnNotice;
			if (vOldXRayOnNotice)
			{
				m_XRaysOn.ShowWindow(SW_SHOW);
				m_Calibrating.ShowWindow(SW_SHOW);
				CString TempText;
				TempText.LoadString(IDS_XRAYSON);
				SetDlgItemText(IDC_Calibrating,TempText);
			}
			else
			{
				m_XRaysOn.ShowWindow(SW_HIDE);
				m_Calibrating.ShowWindow(SW_HIDE);
			}
			UpdateButtons();
		}

		if ((vOldLastBodyTriggerLength != vLocalSystemData->vLastBodyTriggerLength) ||
			(vOldBodyTriggerWidthStandardDeviation != vLocalSystemData->vBodyTriggerWidthStandardDeviation) ||
			(vOldBodyTriggerWidthAverage != vLocalSystemData->vBodyTriggerWidthAverage))
		{
			vOldLastBodyTriggerLength = vLocalSystemData->vLastBodyTriggerLength;
			vOldBodyTriggerWidthAverage = vLocalSystemData->vBodyTriggerWidthAverage;
			vOldBodyTriggerWidthStandardDeviation = vLocalSystemData->vBodyTriggerWidthStandardDeviation;
			CString TempString = dtoa(vOldLastBodyTriggerLength,0) +
				", A: " + dtoa(vLocalSystemData->vBodyTriggerWidthAverage,1) + ", S: " +
				dtoa(vLocalSystemData->vBodyTriggerWidthStandardDeviation,1);
			SetDlgItemText(IDC_EncoderCountsOnLastTrigger3,TempString);

		}
		if (vOldContainerCount != vLocalSystemData->vTotalContainers)
		{
			vOldContainerCount = vLocalSystemData->vTotalContainers;
			if (vWaitingForContainer)
			{//calculate the pulses per unit
				if (vOldBodyTriggerWidthAverage)
				if (vOldBodyTriggerWidthStandardDeviation > 4)
				{	// was notice dialog, make yes/no  kjh 11/26/12
//					CNoticeDialog TempNoticeDialog;
//					TempNoticeDialog.vNoticeText = "\n\nEncoder Data varies too much,\ncheck optical sensor and encoder,\n Please Try Again";
//					TempNoticeDialog.vType = cInformationMessage;
//					TempNoticeDialog.DoModal();
					CYesNoDialog TempYesNoDialog;
					TempYesNoDialog.vNoticeText = _T("\n\nEncoder Data varies too much,\ncheck optical sensor and encoder");
					TempYesNoDialog.vYesButtonText = "Try Again";
					TempYesNoDialog.vNoButtonText = "Cancel";
					int TempResult = TempYesNoDialog.DoModal();
					// if user said OK
					if (TempResult == IDOK)
					{
						vEncoderProblemTryAgain = true;
					}
					else
					{
						vEncoderProblemTryAgain = false;
						//stop conveyer if started it
						//stop looking for containers
						EndCalibrateEncoderWithoutXRaysYesProductionSelection();
						UpdateDisplay();
					}	
					vMainWindowPointer->ResetCounters();
				}
				else
				{
					double TempPulsesPerUnit = 32;
					if (vGlobalScanTracType == cCaseInspectorScanTrac) //uses 1.6mm detectors which are 64 pixels in 4 inches
						TempPulsesPerUnit = 16;

					if (vCalibrateEncoderContainerWidth)
						TempPulsesPerUnit = vOldBodyTriggerWidthAverage / vCalibrateEncoderContainerWidth;
					bool TempEncoderCalibrationDone = false;

					if (TempPulsesPerUnit > vMaximumSuggestedPPI)  //too many pulses per unit
					{
						CYesNoDialog TempYesNoDialog;
						CString TempString = "\n" + dtoa(TempPulsesPerUnit,2) + 
							" is too many Pulses Per Inch.\nBest to increase the Encoder Divider and test again";

						TempYesNoDialog.vNoticeText = TempString;
						TempYesNoDialog.vYesButtonText = "Increase Divider";
						TempYesNoDialog.vNoButtonText = "Leave As Is";
						TempYesNoDialog.vQuestionType = cConfirmSave;
						int TempResult = TempYesNoDialog.DoModal();
						//if user said OK
						if (TempResult == IDOK)
						{
							vLocalConfigurationData->vEncoderDivider++;
							vEncoderDividerChanged = true;

							//Send Encoder Divisor
							if (vGlobaluCSerialPort)
								vGlobaluCSerialPort->SendEncoderDividerTouC();

							UpdateDisplay();
							SetChangeMade();

							CNoticeDialog TempNoticeDialog;
							TempNoticeDialog.vNoticeText = "\n\nEncoder Divider has been increased to: " + dtoa(vLocalConfigurationData->vEncoderDivider,0) +
									".\nSend container through 6 more times.";
							TempNoticeDialog.vType = cInformationMessage;
							TempNoticeDialog.DoModal();

							vMainWindowPointer->ResetCounters();

						}
						else
							TempEncoderCalibrationDone = true;
					}
					else
					if (TempPulsesPerUnit < vMinimumSuggestedPPI)  //too few pulses per unit
					{
						if (vLocalConfigurationData->vEncoderDivider > 1)
						{
							CYesNoDialog TempYesNoDialog;
							CString TempString = "\n" + dtoa(TempPulsesPerUnit,2) + 
								" is too few PPI (Pulses Per Inch).\nBest to decrease the Encoder Divider and test again";

							TempYesNoDialog.vNoticeText = TempString;
							TempYesNoDialog.vYesButtonText = "Decrease Divider";
							TempYesNoDialog.vNoButtonText = "Leave As Is";
							TempYesNoDialog.vQuestionType = cConfirmSave;
							int TempResult = TempYesNoDialog.DoModal();
							//if user said OK
							if (TempResult == IDOK)
							{
								vLocalConfigurationData->vEncoderDivider--;
								vEncoderDividerChanged = true;

								//Send Encoder Divisor
								if (vGlobaluCSerialPort)
									vGlobaluCSerialPort->SendEncoderDividerTouC();

								UpdateDisplay();
								SetChangeMade();

								CNoticeDialog TempNoticeDialog;
								TempNoticeDialog.vNoticeText = "\n\nEncoder Divider has been decreased to: " + dtoa(vLocalConfigurationData->vEncoderDivider,0) +
									".\nSend container through 6 more times.";
								TempNoticeDialog.vType = cInformationMessage;
								TempNoticeDialog.DoModal();

								vMainWindowPointer->ResetCounters();
							}
							else
								TempEncoderCalibrationDone = true;
						}
						else
						{
							CNoticeDialog TempNoticeDialog;
							TempNoticeDialog.vNoticeText = "Encoder Divider already minimum and\ntoo few pulses.\nChange encoder for one with more pulses per rev,\nor mount on smaller diameter conveyor shaft.\\nAborting Calibration";
							TempNoticeDialog.vType = cErrorMessage;
							TempNoticeDialog.DoModal();

							//abort calibration
							CalibrateEncoderWithoutXRaysYesProductionSelection(); //kjh was  OnCalibrateEncoderWithoutXRays();
						}
					}
					else  //just right for the pulses per unit
					{
						CNoticeDialog TempNoticeDialog;
						TempNoticeDialog.vNoticeText = "\n\n" + dtoa(TempPulsesPerUnit,2) + " Encoder Pulses Per Inch is in acceptable range.";
						TempNoticeDialog.vType = cInformationMessage;
						TempNoticeDialog.DoModal();

						TempEncoderCalibrationDone = true;
					}
					if (TempEncoderCalibrationDone)
					{
						CSelectItemDialog ISelectItemDialog;  
						//Set dialog box data titles and number value
						ISelectItemDialog.vTitleString = "Encoder Calibration Acceptance";
						CString TempCString = dtoa(vCalibrateEncoderContainerWidth, 2);
						ISelectItemDialog.m_DialogTitleStaticText4 = "Encoder Calibration PPI: " + dtoa(TempPulsesPerUnit,2) + ", (" + dtoa(vMinimumSuggestedPPI,0) +
																		" to " + dtoa(vMaximumSuggestedPPI,0) + " recommended, 32 is ideal)";
						ISelectItemDialog.vNumberOfItems = 3;
						if ((vLocalSystemData->vLastBodyTriggerLength != 0 ) && (TempPulsesPerUnit != 0))
							TempCString = dtoa((vLocalSystemData->vLastBodyTriggerLength / TempPulsesPerUnit), 2);
						else
							TempCString = "0";
						ISelectItemDialog.m_DialogTitleStaticText2 = "Calibrated Can Diameter: " + TempCString + " inches.";

						TempCString = dtoa(vEnteredContainerWidth, 2);
						ISelectItemDialog.m_DialogTitleStaticText5 = "Entered Can Diameter: " + TempCString + " inches" ;
						if (((vEnteredContainerWidth - (vLocalSystemData->vLastBodyTriggerLength / TempPulsesPerUnit)) != 0) && ((vLocalSystemData->vLastBodyTriggerLength / TempPulsesPerUnit) !=0))	// make sure not 0
						{
							double TempWidthPercent = (((vEnteredContainerWidth - (vLocalSystemData->vLastBodyTriggerLength / TempPulsesPerUnit)) / (vLocalSystemData->vLastBodyTriggerLength / TempPulsesPerUnit))*100);
							TempCString = dtoa(TempWidthPercent, 1);
							if ((TempWidthPercent > 10) || (TempWidthPercent < -10))
							{
								ISelectItemDialog.vMakeTitle3Highlighted = true;
							}
						}
						else
						{
							TempCString = "0";
						}
						ISelectItemDialog.m_DialogTitleStaticText3 = "Difference between Entered and Calibrated " + TempCString + " %";
						ISelectItemDialog.vBackgroundColor = cGreen;
						ISelectItemDialog.vGreenItem = 1;
						ISelectItemDialog.vInAutoSetup = false;
						ISelectItemDialog.vStayTillDecissionSetup = false;
						
						ISelectItemDialog.vItems[0] = "Accept";
						ISelectItemDialog.vItems[1] = "Return To Calibrate";
						ISelectItemDialog.vItems[2] = "Enter PPI Manually";
						
						//Pass control to dialog box and display
						vCalibrateEncoderMethod = cNotCalibrating;
						int nResponse = ISelectItemDialog.DoModal();
						//dialog box is now closed, if user pressed select do this
						//if user pressed cancel, do nothing
						if (nResponse == IDOK)
						{
							if (ISelectItemDialog.vItemNumberSelected == 1) // if user said OK use this value
							{
//set other cal only			vLocalConfigurationData->vCalibratedPixelsPerUnit = TempPulsesPerUnit;
								//  below orginal code for except new encoder ppi value 11/20/2012 kjh 
								vPixelsPerUnit = TempPulsesPerUnit;
//	set				vGlobalPixelsPerUnit = vPixelsPerUnit;
//   in
//	other cal 		//must set when change pixels per inch
//	method accept	vGlobalCurrentProduct->SetProductBodyTriggerToImageBeltPositionOffset(
//	cal, ask		vGlobalCurrentProduct->vProductBodyTriggerToImageBeltPositionOffset);
//					vGlobalCurrentProduct->SetProductImageWidth(vGlobalCurrentProduct->vProductImageWidth);
//
//					vGlobalCurrentProduct->SetProductLockOutWidth(vGlobalCurrentProduct->vProductLockOutWidth);
//
								vNeedToCalibrateEncoder = false;
								UpdateDisplay();
								SetChangeMade();
								//stop encoder calibration
								CalibrateEncoderWithoutXRaysYesProductionSelection(); //kjh wasOnCalibrateEncoderWithoutXRays();
							}
							if (ISelectItemDialog.vItemNumberSelected == 2) // restore values and back to function
							{
								//stop encoder calibration
								//CalibrateEncoderWithoutXRaysYesProductionSelection(); //kjh wasOnCalibrateEncoderWithoutXRays();
								UpdateDisplay();
								CalibrateEncoderWithoutXRaysYesProductionSelection(); //kjh wasOnCalibrateEncoderWithoutXRays();
							}
							if (ISelectItemDialog.vItemNumberSelected == 3) // Enter PPI Manually
							{
								//RestoreValues();
								//stop encoder calibration
								CalibrateEncoderWithoutXRaysYesProductionSelection(); //kjh wasOnCalibrateEncoderWithoutXRays();
								OnSubFunction4Button();
							}
						}
						else if (nResponse == 10)
						{	//stop encoder calibration								
							CalibrateEncoderWithoutXRaysYesProductionSelection(); //kjh wasOnCalibrateEncoderWithoutXRays();
							UpdateDisplay();
							//Main Menu button pressed
							Sleep(1); //is the equivelent of a yield statement;
							RestoreValues();
							PrepareToExit();
							CDialog::EndDialog(10);
						}
						else //#define IDCANCEL            2
						{
							//stop encoder calibration
							CalibrateEncoderWithoutXRaysYesProductionSelection(); //kjh wasOnCalibrateEncoderWithoutXRays();
							UpdateDisplay();
						}
					}		// end if (TempEncoderCalibrationDone)
					//end b
				}
			}			// end if (vWaitingForContainer)
		}		//end 		if (vOldContainerCount != vLocalSystemData->vTotalContainers)
		int TempTimerResult = SetTimer(vUpdateDisplayTimerHandle,500,NULL);
		if (!TempTimerResult)
			ReportErrorMessage("Error-Display Timer Failed",cEMailInspx,32000);

	}
	CDialog::OnTimer(nIDEvent);
}

void CSystemValuesDialog::RestoreValues()
{
	vGlobalPixelsPerUnit = vLocalConfigurationData->vPixelsPerUnit;

	if ((vOriginalEncoderDivider != vLocalConfigurationData->vEncoderDivider) || (vLocalConfigurationData->vEncoderDividerForPoint4mmMode = vOriginalEncoderDividerForPoint4mmMode))
	{ //if changed, set back to what it was
		vLocalConfigurationData->vEncoderDivider = vOriginalEncoderDivider;
		vLocalConfigurationData->vEncoderDividerForPoint4mmMode = vOriginalEncoderDividerForPoint4mmMode;

		if (vGlobaluCSerialPort)
			vGlobaluCSerialPort->SendEncoderDividerTouC();
		UpdateDisplay();
	}
}

void CSystemValuesDialog::PrepareToExit()
{
	if (vLocalSystemData->vTestingTrigger)
	{
		CYesNoDialog TempYesNoDialog;
		TempYesNoDialog.vNoticeText = _T("\n\nTesting Triggers, Exit Testing Triggers?");
		TempYesNoDialog.vQuestionType = cConfirmQuestion;
		TempYesNoDialog.vYesButtonText = "Exit Test";
		TempYesNoDialog.vNoButtonText = "Continue Test";
		int TempResult = TempYesNoDialog.DoModal();
		//if user said OK, 
		if (TempResult == IDOK)
			CalibrateEncoderWithoutXRaysYesProductionSelection(); //kjh wasOnCalibrateEncoderWithoutXRays();
	}

	if ((vLocalSystemData->vSystemRunMode == cCalibrateEncoderMode) || (vCalibratingWithXRays))
	{
		CalibrateEncoderWithXRaysNoProductionSelection();  //was OnFunction3Button();
	}

	if (vOriginalProduct)
	{
		vMainWindowPointer->SetupProduct(vOriginalProduct,false);
		vOriginalProduct = NULL;
	}
}


//was OnCalibrateEncoderWithoutXRays() then CalibrateEncoderWithoutXRaysYesProductionSelection(), removed just finish part
void CSystemValuesDialog::EndCalibrateEncoderWithoutXRaysYesProductionSelection() 
{
	vWaitingForContainer = false;
	tSerialCommand TempCommand;
	vLocalSystemData->vTestingTrigger = false;
	vCalibrateEncoderMethod = cNotCalibrating;		//enum {cNotCalibrating, cCalXrayOnNoProduction, cCalXrayOffYesProduction};
	vLocalSystemData->vDigitalInputLine3Mask = 0x00;
	vLocalSystemData->vCurrentBodyTriggerMask = 0;
	TempCommand[0] = 0x1C;
	TempCommand[1] = 0x03;  //send mask for 16 digital lines in #3
	TempCommand[2] = 0x00;
	TempCommand[3] = 0x00;
	if (vGlobaluCSerialPort)
		vGlobaluCSerialPort->SendSerialCommand(TempCommand);
	Sleep(cSendCommandSleepTime);

	TempCommand[1] = 0x04;  //send mask so uC know which bit is body trigger
	TempCommand[2] = 0x00;
	TempCommand[3] = 0x00;
	if (vGlobaluCSerialPort)
		vGlobaluCSerialPort->SendSerialCommand(TempCommand);
	Sleep(cSendCommandSleepTime);
	vMainWindowPointer->SendConveyorOnOffToUController(false);
	vMainWindowPointer->TurnSimulatedEncoderOff();
	vMainWindowPointer->SendRetriggerLockout(vGlobalCurrentProduct->vLockoutPosition);
}

//was OnCalibrateEncoderWithoutXRays()
void CSystemValuesDialog::CalibrateEncoderWithoutXRaysYesProductionSelection() 
{
	//Calibrate the encoder
	if ((vGlobalScanTracType != cLegatoScanTrac) && (vGlobalScanTracType != cFermataScanTrac))// not a pipeline
	if (vCalibratingWithXRays)
	{	// if the Xrays are on, they should be off, send a notice and terminate
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\n\nCalibrating Encoder With X-Rays.";
		TempNoticeDialog.vType = cNoticeMessage;
		TempNoticeDialog.DoModal();
	}
	else
	if ((!vLocalSystemData->vXRayOnNotice) && (vLocalSystemData->vSystemRunMode == cStoppedSystemMode))
	{
		if (vGlobalCurrentProduct)
		{
			if (!vLocalSystemData->vTestingTrigger)
			{
				CNumericEntryDialog INumericEntryDialog;  
				
				INumericEntryDialog.vEditString = dtoa(vCalibrateEncoderContainerWidth, 2);

				//Set dialog box data titles and number value
				INumericEntryDialog.m_DialogTitleStaticText1 = "Enter the width (diameter) of a sample container";
				INumericEntryDialog.m_DialogTitleStaticText2 = " at the optical sensor height.";
				INumericEntryDialog.m_UnitsString = "Units in inches. Original Value: " + INumericEntryDialog.vEditString;

				INumericEntryDialog.m_DialogTitleStaticText3 = "Send container through 6 times after Save and Exit";
				INumericEntryDialog.m_DialogTitleStaticText4 = "You must re-size and learn all products after";

				INumericEntryDialog.vMaxValue = 100;
				INumericEntryDialog.vMinValue = 0;
				INumericEntryDialog.vIntegerOnly = false;
				//Pass control to dialog box and display
				int nResponse = INumericEntryDialog.DoModal();
				//dialog box is now closed, if user pressed select do this
				//if user pressed cancel, do nothing
				if (nResponse == IDOK)
				{
					vCalibrateEncoderContainerWidth = ATOF(INumericEntryDialog.vEditString);
					vEnteredContainerWidth = ATOF(INumericEntryDialog.vEditString);
					if (vCalibrateEncoderContainerWidth)
						vWaitingForContainer = true;
				}
				else 
				if (nResponse == 10)
				{
					//Main Menu button pressed
					Sleep(1); //is the equivelent of a yeild statement;
					RestoreValues();
					PrepareToExit();
					CDialog::EndDialog(10);
				}
				if (nResponse == IDOK)
				{
	// added ask type of ScanTrac for calibrate encoder
					CSelectItemDialog ISelectItemDialog;  
					//Set dialog box data titles and number value
					ISelectItemDialog.vTitleString = "Choose ScanTrac Configuration";
					ISelectItemDialog.m_DialogTitleStaticText2 = "";
					ISelectItemDialog.m_DialogTitleStaticText3 = "";
					ISelectItemDialog.m_DialogTitleStaticText4 = "";
					ISelectItemDialog.vBackgroundColor = cGreen;
					ISelectItemDialog.vGreenItem = 1;
					ISelectItemDialog.vShowExit = 0;
					ISelectItemDialog.vInAutoSetup = vLocalSystemData->vInAutoSetup;
					ISelectItemDialog.m_DialogTitleStaticText3 = "Original Value: Custom, Distance: " + dtoa(vSystemBodyTriggerToXRayBeam,2);

					if ((vGlobalScanTracType == cForteScanTrac) || (vGlobalScanTracType == cAtempoScanTrac) || (vGlobalScanTracType == cAllegroScanTrac))
					{
						ISelectItemDialog.vNumberOfItems = 3;
						ISelectItemDialog.vItems[0] = "Atempo or Forte: " + dtoa(cForteTriggerToBeamOffset,2);
						ISelectItemDialog.vItems[1] = "Allegro: " + dtoa(cAllegroTriggerToBeamOffset,2);
						ISelectItemDialog.vItems[2] = "Custom";
						
						if (vGlobalScanTracType == cAllegroScanTrac)
							ISelectItemDialog.vGreenItem = 2;

						if (vSystemBodyTriggerToXRayBeam == cForteTriggerToBeamOffset)
						{
							ISelectItemDialog.m_DialogTitleStaticText3 = "Original Value: Atempo or Forte, BT to X-Ray Distance: " + dtoa(cForteTriggerToBeamOffset,2);
						}
						if (vSystemBodyTriggerToXRayBeam == cAllegroTriggerToBeamOffset)
						{
							ISelectItemDialog.m_DialogTitleStaticText3 = "Original Value: Allegro, BT to X-Ray Distance: " + dtoa(cAllegroTriggerToBeamOffset,2);
						}
					}
					else
					{
						ISelectItemDialog.vNumberOfItems = 4;
						ISelectItemDialog.vItems[0] = "Solo Containers Left To Right, BT to X-Ray Distance: " + dtoa(cSoloLeftToRightDistance,2);
						ISelectItemDialog.vItems[1] = "Solo Containers Right To Left, BT to X-Ray Distance: " + dtoa(cSoloRightToLeftDistance,2);
						ISelectItemDialog.vItems[2] = "Piccolo BT to X-Ray Distance: " + dtoa(cPiccoloDistance,2);
						ISelectItemDialog.vItems[3] = "Custom";

						if (vSystemBodyTriggerToXRayBeam == cSoloLeftToRightDistance)
							ISelectItemDialog.m_DialogTitleStaticText3 = "Original Value: Solo Containers Left To Right, BT to X-Ray Distance: " + dtoa(cSoloLeftToRightDistance,2);
						else
						if (vSystemBodyTriggerToXRayBeam == cSoloRightToLeftDistance)
						{
							ISelectItemDialog.m_DialogTitleStaticText3 = "Original Value: Solo Containers Right To Left, BT to X-Ray Distance: " + dtoa(cSoloRightToLeftDistance,2);
							ISelectItemDialog.vGreenItem = 2;
						}
						else
						if (vSystemBodyTriggerToXRayBeam == cPiccoloDistance)
						{
							ISelectItemDialog.m_DialogTitleStaticText3 = "Original Value: Piccolo, BT to X-Ray Distance: " + dtoa(cPiccoloDistance,2);
							ISelectItemDialog.vGreenItem = 3;
						}
					}


					//Pass control to dialog box and display
					nResponse = ISelectItemDialog.DoModal();
					//dialog box is now closed, if user pressed select do this
					//if user pressed cancel, do nothing
					if (nResponse == IDOK)
					{
						SetChangeMade();
						bool TempDoCustom = false;
						if ((vGlobalScanTracType == cAtempoScanTrac) || (vGlobalScanTracType == cForteScanTrac) || (vGlobalScanTracType == cAllegroScanTrac))
						{
							if (ISelectItemDialog.vItemNumberSelected == 1) //atempo or forte
							{
								vSystemBodyTriggerToXRayBeam = cForteTriggerToBeamOffset;
								//TempProduct->SetProductBodyTriggerToImageBeltPositionOffset((float)2);
								vSystemEjectorPositionOffset = cForteLeftToRightEjectorOffset;
							}
							else
							if (ISelectItemDialog.vItemNumberSelected == 2) //allegro
							{
								vSystemBodyTriggerToXRayBeam = cAllegroTriggerToBeamOffset;
								vSystemEjectorPositionOffset = cAllegroEjectorOffset;
							}
							else
								TempDoCustom = true;
						}
						else
						{
							if (ISelectItemDialog.vItemNumberSelected == 1)
							{
								vSystemBodyTriggerToXRayBeam = cSoloLeftToRightDistance;
								//TempProduct->SetProductBodyTriggerToImageBeltPositionOffset((float)12);
								vSystemEjectorPositionOffset = cSoloEjectorOffset;
							}
							else
							if (ISelectItemDialog.vItemNumberSelected == 2)
							{
								vSystemBodyTriggerToXRayBeam = cSoloRightToLeftDistance;
								//TempProduct->SetProductBodyTriggerToImageBeltPositionOffset((float)2);
								vSystemEjectorPositionOffset = cSoloEjectorOffset;
							}
							else
							if (ISelectItemDialog.vItemNumberSelected == 3)
							{
								vSystemBodyTriggerToXRayBeam = cPiccoloDistance;
								//TempProduct->SetProductBodyTriggerToImageBeltPositionOffset((float)2);
								vSystemEjectorPositionOffset = cPiccoloEjectorOffset;
							}
							else
								TempDoCustom = true;
						}
						if (TempDoCustom)
						{
							//enter distance from X-Ray Beam to body trigger
							CNumericEntryDialog INumericEntryDialog;  
						
							INumericEntryDialog.vEditString = dtoa(vSystemBodyTriggerToXRayBeam, 2);

							//Set dialog box data titles and number value
							INumericEntryDialog.m_DialogTitleStaticText1 = "Enter the distance from the ";
							INumericEntryDialog.m_DialogTitleStaticText2 = "Container Trigger to the X-Ray Beam";
							INumericEntryDialog.m_UnitsString = "Units in Inches. Original Value: " + INumericEntryDialog.vEditString;

							INumericEntryDialog.m_DialogTitleStaticText3 = "This is fixed for this ScanTrac";
							INumericEntryDialog.m_DialogTitleStaticText4 = "";

							INumericEntryDialog.vMaxValue = 100;
							INumericEntryDialog.vMinValue = 0;
							INumericEntryDialog.vIntegerOnly = false;
							//Pass control to dialog box and display
							nResponse = INumericEntryDialog.DoModal();
							//dialog box is now closed, if user pressed select do this
							//if user pressed cancel, do nothing
							if (nResponse == IDOK)
							{
								vSystemBodyTriggerToXRayBeam = ATOF(INumericEntryDialog.vEditString);
								//if (vSystemBodyTriggerToXRayBeam > 15)
								//	TempProduct->SetProductBodyTriggerToImageBeltPositionOffset((float)vSystemBodyTriggerToXRayBeam - 12);
								//else
								//	TempProduct->SetProductBodyTriggerToImageBeltPositionOffset((float)2);

								//enter the ejector offset
								INumericEntryDialog.vEditString = dtoa(vSystemEjectorPositionOffset, 2);

								//Set dialog box data titles and number value
								INumericEntryDialog.m_DialogTitleStaticText1 = "Enter the ejector offset";
								INumericEntryDialog.m_DialogTitleStaticText2 = "Distance from Container Trigger to the wall";
								INumericEntryDialog.m_UnitsString = "Units in Inches. Original Value: " + INumericEntryDialog.vEditString;

								INumericEntryDialog.m_DialogTitleStaticText3 = "of the output side of the ScanTrac box.";
								INumericEntryDialog.m_DialogTitleStaticText4 = "This is fixed for this ScanTrac.  Normally 37.5";

								INumericEntryDialog.vMaxValue = 100;
								INumericEntryDialog.vMinValue = 0;
								INumericEntryDialog.vIntegerOnly = false;
								//Pass control to dialog box and display
								nResponse = INumericEntryDialog.DoModal();
								//dialog box is now closed, if user pressed select do this
								//if user pressed cancel, do nothing
								if (nResponse == IDOK)
								{
									vSystemEjectorPositionOffset = ATOF(INumericEntryDialog.vEditString);
								}
								else 
								if (nResponse == 10)
								{
									//Main Menu button pressed
									Sleep(1); //is the equivelent of a yeild statement;
									RestoreValues();
									PrepareToExit();
									CDialog::EndDialog(10);
								}
							}
							else 
							if (nResponse == 10)
							{
								//Main Menu button pressed
								Sleep(1); //is the equivelent of a yeild statement;
								RestoreValues();
								PrepareToExit();
								CDialog::EndDialog(10);
							}
						}
						//ask user to enter distance to each ejector
						for (BYTE TempLoop = 0; TempLoop < cNumberOfEjectors; TempLoop++)
						if (vLocalConfigurationData->vEjector[TempLoop].vEnabled)
						{
							INumericEntryDialog.vEditString = dtoa(vSystemEjectorDistance[TempLoop], 2);

							//Set dialog box data titles and number value
							INumericEntryDialog.m_DialogTitleStaticText1 = "Enter the distance from the ";

							INumericEntryDialog.m_DialogTitleStaticText2 = "ScanTrac Output Wall to center of Ejector " + dtoa(TempLoop + 1, 0);

							INumericEntryDialog.m_UnitsString = "Units in Inches. Original Value: " + INumericEntryDialog.vEditString;

							INumericEntryDialog.m_DialogTitleStaticText3 = "This is fixed for this ScanTrac";
							INumericEntryDialog.m_DialogTitleStaticText4 = "";

							INumericEntryDialog.vMaxValue = 200;
							INumericEntryDialog.vMinValue = 0;
							INumericEntryDialog.vIntegerOnly = false;
							//Pass control to dialog box and display
							nResponse = INumericEntryDialog.DoModal();
							//dialog box is now closed, if user pressed select do this
							//if user pressed cancel, do nothing
							if (nResponse == IDOK)
							{
								vSystemEjectorDistance[TempLoop] = ATOF(INumericEntryDialog.vEditString);
							}
						}
					}
					else 
					if (nResponse == 10)
					{
						//Main Menu button pressed
						Sleep(1); //is the equivelent of a yeild statement;
						RestoreValues();
						PrepareToExit();
						CDialog::EndDialog(10);
					}
		// end add type of encoder for calibrate encoder
					if (nResponse == IDOK)
					{
						if (vWaitingForContainer)
						{
							vMainWindowPointer->ResetCounters();
							vLocalSystemData->vBodyTriggerWidthSamples = 0;
							vLocalSystemData->vTestingTrigger = true;
							vCalibrateEncoderMethod = cCalXrayOffYesProduction;		//enum {cNotCalibrating, cCalXrayOnNoProduction, cCalXrayOffYesProduction};

							if ((vLocalSystemData->vFPGASimulateEncoderRate) ||
								(vLocalConfigurationData->vContinuousFeedEncoderRate))
								vMainWindowPointer->TurnSimulatedEncoderOn();
							else
								vMainWindowPointer->SendConveyorOnOffToUController(true);
							vLocalSystemData->vCurrentBodyTriggerMask = vLocalConfigurationData->
								vBodyTrigger[vGlobalCurrentProduct->vBodyTrigger - 1].vLineBitMask;

							if (!vLocalConfigurationData->vBodyTrigger[vGlobalCurrentProduct->vBodyTrigger - 1].vActiveHigh)
								vLocalSystemData->vCurrentBodyTriggerActiveLowMask = 
									vLocalSystemData->vCurrentBodyTriggerActiveLowMask | vLocalConfigurationData->
								vBodyTrigger[vGlobalCurrentProduct->vBodyTrigger - 1].vLineBitMask;

							vLocalSystemData->vBackupBodyTriggerMask = 0;
							if (vGlobalCurrentProduct->vBackupBodyTrigger)
							{
								vLocalSystemData->vBackupBodyTriggerMask = vLocalConfigurationData->
									vBodyTrigger[vGlobalCurrentProduct->vBackupBodyTrigger - 1].vLineBitMask;
								if (!vLocalConfigurationData->vBodyTrigger[vGlobalCurrentProduct->vBackupBodyTrigger - 1].vActiveHigh)
									vLocalSystemData->vCurrentBodyTriggerActiveLowMask = 
										vLocalSystemData->vCurrentBodyTriggerActiveLowMask | vLocalConfigurationData->
									vBodyTrigger[vGlobalCurrentProduct->vBackupBodyTrigger - 1].vLineBitMask;
							}
							vLocalSystemData->vDigitalInputLine3Mask = vLocalSystemData->vBackupBodyTriggerMask | 
								vLocalSystemData->vCurrentBodyTriggerMask;

							if (vLocalSystemData->vBackupBodyTriggerMask && vLocalSystemData->vCurrentBodyTriggerMask)
								vLocalSystemData->vOnlyOneBodyTriggerEnabled = false;
							else
								vLocalSystemData->vOnlyOneBodyTriggerEnabled = true;

							vMainWindowPointer->SendRetriggerLockout(3); //turn off retigger lockout

							//Send Interrupt Mask for body trigger
							tSerialCommand TempCommand;
							TempCommand[0] = 0x1C;
							TempCommand[1] = 0x03;  //send mask for 16 digital lines in #3 , body triggers
							TempCommand[2] = 0x00;
							TempCommand[3] = vLocalSystemData->vDigitalInputLine3Mask;
							if (vGlobaluCSerialPort)
								vGlobaluCSerialPort->SendSerialCommand(TempCommand);
							Sleep(cSendCommandSleepTime);

							TempCommand[1] = 0x04;  //send mask so uC know which bit is body trigger
							TempCommand[2] = 0x00;
							TempCommand[3] = vLocalSystemData->vCurrentBodyTriggerMask;
							if (vGlobaluCSerialPort)
								vGlobaluCSerialPort->SendSerialCommand(TempCommand);
							Sleep(cSendCommandSleepTime);
							SetDlgItemText(IDC_EncoderCountsOnLastTrigger3,"Send " + dtoa(vCalibrateEncoderContainerWidth,2) + " \" container through");
						}
					}
				}
			}
			else
			{
				EndCalibrateEncoderWithoutXRaysYesProductionSelection();			
			}			
			UpdateButtons();
		}
		else
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "You must Select A Product to Calibrate the Encoder";
			TempNoticeDialog.vType = cErrorMessage;
			TempNoticeDialog.DoModal();
		}
	}
	else
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "System must be Stopped to Calibrate Encoder";
		TempNoticeDialog.vType = cErrorMessage;
		TempNoticeDialog.DoModal();
	}
}

void CSystemValuesDialog::CalculateCalibration()  //calibrating encoder with X-Ray Image
{
	if (vLocalSystemData->vITIPCDig->vOriginalImage)
	{
		if (vGlobalImageAquisitionThread)
		{
			vGlobalImageAquisitionThread->WriteImageToHardDisk(vLocalSystemData->vITIPCDig->vOriginalImage->vGreyImage, vLocalConfigurationData->vScanTracCalibrationImagesDirectory +"CalibrateEncoder.bmp", 
				vLocalSystemData->vITIPCDig->vAquisitionSizeX, vLocalSystemData->vITIPCDig->vAquisitionSizeY);
		}
		WORD TempTop = 0;
		WORD TempBottom = 0;
		WORD TempRight = 0;
		WORD TempLeft = 0;
		WORD TempHeight = 0;
		WORD TempWidth = 0;
		vGlobalInAutoSize = true;
		vLocalSystemData->vITIPCDig->FindContainerDimensions(vLocalSystemData->vITIPCDig->vOriginalImage->vGreyImage, vLocalSystemData->vITIPCDig->vNIDDScratchImage,
			&TempTop, &TempBottom, &TempRight, &TempLeft, &TempHeight, &TempWidth, true, 10000); //vLocalConfigurationData->vShapeHandlingWidthTolerancePixels); //10000 = Finding random edges, don't use reference edges
		vGlobalInAutoSize = false;

		bool TempChangedDivider = false;
		WORD TempEncoderPulsesFromTriggerToContainer = (WORD)(vGlobalCurrentProduct->vBTToImagePosition + (vGlobalCurrentProduct->vImageWidthPosition - TempRight));

		double TempPulsesPerUnit = 32;
		if (vGlobalScanTracType == cCaseInspectorScanTrac) //uses 1.6mm detectors which are 64 pixels in 4 inches
			TempPulsesPerUnit = 16;

		if (vSystemBodyTriggerToXRayBeam)
			TempPulsesPerUnit = TempEncoderPulsesFromTriggerToContainer / vSystemBodyTriggerToXRayBeam;

		if (vLocalSystemData->vLogFile.vLogSerialData)
		{
			vLocalSystemData->vLogFile.WriteToLogFile("Calibrate Encoder Right Side:" + dtoa(TempRight, 0),cDebugMessage);
			vLocalSystemData->vLogFile.WriteToLogFile("Calibrate Encoder Left Side:" + dtoa(TempLeft, 0),cDebugMessage);
			vLocalSystemData->vLogFile.WriteToLogFile("Calibrate Encoder vBTToImagePosition:" + dtoa(vGlobalCurrentProduct->vBTToImagePosition, 0),cDebugMessage);
			vLocalSystemData->vLogFile.WriteToLogFile("Calibrate Encoder vImageWidthPosition:" + dtoa(vGlobalCurrentProduct->vImageWidthPosition, 0),cDebugMessage);
			vLocalSystemData->vLogFile.WriteToLogFile("TempEncoderPulsesFromTriggerToContainer:" + dtoa(TempEncoderPulsesFromTriggerToContainer, 0),cDebugMessage);
			vLocalSystemData->vLogFile.WriteToLogFile("TempPulsesPerUnit:" + dtoa(TempPulsesPerUnit, 0),cDebugMessage);
		}

		bool TempEncoderCalibrationDone = false;
/*
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = 
				dtoa((vGlobalCurrentProduct->vImageWidthPosition - TempRight),0) + 
				" Encoder Pulses in Image to Right.\n" +
				dtoa(vGlobalCurrentProduct->vBTToImagePosition,0) + " Encoder Pulses BT to Image.\n" +
				dtoa(TempEncoderPulsesFromTriggerToContainer,0) + " Encoder Pulses BT to Beam.\n" + 
				dtoa(vSystemBodyTriggerToXRayBeam,2) + " Inches BT to Beam\n" +
				dtoa(TempPulsesPerUnit,2) + " Encoder Pulses Per Inch.";
			TempNoticeDialog.vType = cInformationMessage;
			TempNoticeDialog.DoModal();
			*/

		if ((vGlobalCurrentProduct->vImageWidthPosition < TempRight + 4) || //at right or left edge
			(TempRight < 3))
		{
			CString TempString = "to 4";
			if (vLocalConfigurationData->vEncoderDivider == 4)
				TempString = "higher or lower";
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\nDid not see edges of container in image.\nSet Encoder Divider " + TempString + 
				" and try again\nor Check ~Calibrate Encoder Product sees container.";
			TempNoticeDialog.vType = cErrorMessage;
			TempNoticeDialog.DoModal();

			if (vLocalSystemData->vLogFile.vLogSerialData)
				vLocalSystemData->vLogFile.WriteToLogFile("Calibrate Encoder-Did not find edge in the correct place",cDebugMessage);

			//abort Calibration
			CalibrateEncoderWithXRaysNoProductionSelection();		//OnFunction3Button();
		}
		else
		if (TempPulsesPerUnit > vMaximumSuggestedPPI)  //too many pulses per unit
		{
			CYesNoDialog TempYesNoDialog;
			CString TempString = "\n" + dtoa(TempPulsesPerUnit,2) + 
				" is too many Pulses Per Inch.\nBest to increase the Encoder Divider and test again";

			TempYesNoDialog.vNoticeText = TempString;
			TempYesNoDialog.vYesButtonText = "Increase Divider";
			TempYesNoDialog.vNoButtonText = "Leave As Is";
			TempYesNoDialog.vQuestionType = cConfirmSave;
			int TempResult = TempYesNoDialog.DoModal();
			//if user said OK
			if (TempResult == IDOK)
			{
				vLocalConfigurationData->vEncoderDivider++;
				TempChangedDivider = true;

				//Send Encoder Divisor
				if (vGlobaluCSerialPort)
					vGlobaluCSerialPort->SendEncoderDividerTouC();

				UpdateDisplay();
				SetChangeMade();

				CNoticeDialog TempNoticeDialog;
				TempNoticeDialog.vNoticeText = "\n\nEncoder Divider has been increased to: " + dtoa(vLocalConfigurationData->vEncoderDivider,0) +
						".\nSend container through again.";
				TempNoticeDialog.vType = cInformationMessage;
				TempNoticeDialog.DoModal();

				vMainWindowPointer->ResetCounters();

			}
			else
				TempEncoderCalibrationDone = true;
		}
		else
		if (TempPulsesPerUnit < vMinimumSuggestedPPI)  //too few pulses per unit
		{
			if (vLocalConfigurationData->vEncoderDivider > 1)
			{
				CYesNoDialog TempYesNoDialog;
				CString TempString = "\n" + dtoa(TempPulsesPerUnit,2) + 
					" is too few PPI (Pulses Per Inch).\nBest to decrease the Encoder Divider and test again";

				TempYesNoDialog.vNoticeText = TempString;
				TempYesNoDialog.vYesButtonText = "Decrease Divider";
				TempYesNoDialog.vNoButtonText = "Leave As Is";
				TempYesNoDialog.vQuestionType = cConfirmSave;
				int TempResult = TempYesNoDialog.DoModal();
				//if user said OK
				if (TempResult == IDOK)
				{
					vLocalConfigurationData->vEncoderDivider--;
					TempChangedDivider = true;

					//Send Encoder Divisor
					if (vGlobaluCSerialPort)
						vGlobaluCSerialPort->SendEncoderDividerTouC();

					UpdateDisplay();
					SetChangeMade();

					CNoticeDialog TempNoticeDialog;
					TempNoticeDialog.vNoticeText = "\n\nEncoder Divider has been decreased to: " + dtoa(vLocalConfigurationData->vEncoderDivider,0) +
						".\nSend container through again.";
					TempNoticeDialog.vType = cInformationMessage;
					TempNoticeDialog.DoModal();

					vMainWindowPointer->ResetCounters();
				}
				else
					TempEncoderCalibrationDone = true;
			}
			else
			{
				CNoticeDialog TempNoticeDialog;
				TempNoticeDialog.vNoticeText = "Encoder Divider already minimum and\ntoo few pulses.\nTry larger Encoder Divider or Check ~Calibrate Encoder Product sees container or change encoder for one with more pulses per rev,\nor mount on smaller diameter conveyor shaft";
				TempNoticeDialog.vType = cErrorMessage;
				TempNoticeDialog.DoModal();

				//abort Calibration
				CalibrateEncoderWithXRaysNoProductionSelection();		//OnFunction3Button();
			}
		}
		else  //just right for the pulses per unit
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\n" + dtoa(TempPulsesPerUnit,2) + " Encoder Pulses Per Inch is in acceptable range.";
			TempNoticeDialog.vType = cInformationMessage;
			TempNoticeDialog.DoModal();

			TempEncoderCalibrationDone = true;
		}
		if (TempEncoderCalibrationDone)
		{
/*			vLocalConfigurationData->vCalibratedPixelsPerUnit = TempPulsesPerUnit;
			CYesNoDialog TempYesNoDialog;
			CString TempString = "\nEncoder Calibration Resulted in: " + dtoa(TempPulsesPerUnit,2) + 
				" PPI\n(" + dtoa(vMinimumSuggestedPPI,0) + " to " + dtoa(vMaximumSuggestedPPI,0) + 
				" recommended, 32 is ideal)\nUse new Encoder PPI (Pulse Per Inch)?";

			TempYesNoDialog.vNoticeText = TempString;
			TempYesNoDialog.vYesButtonText = "Use New Value";
			TempYesNoDialog.vNoButtonText = "Leave As Was";
			TempYesNoDialog.vQuestionType = cConfirmSave;
			int TempResult = TempYesNoDialog.DoModal();
			//if user said OK
*/  //want different dialog, three choices
			CSelectItemDialog ISelectItemDialog;  
			//Set dialog box data titles and number value
			ISelectItemDialog.vTitleString = "Encoder Calibration Acceptance";
			//CString TempCString = dtoa(vCalibrateEncoderContainerWidth, 2);
//			CString TempString = "\nEncoder Calibration Resulted in: " + dtoa(TempPulsesPerUnit,2) + 
//				" PPI\n(" + dtoa(vMinimumSuggestedPPI,0) + " to " + dtoa(vMaximumSuggestedPPI,0) + 
//				" recommended, 32 is ideal)\nUse new Encoder PPI (Pulse Per Inch)?";
			ISelectItemDialog.m_DialogTitleStaticText4 = "Encoder Calibration PPI: " + dtoa(TempPulsesPerUnit,2) + ", (" + dtoa(vMinimumSuggestedPPI,0) +
															" to " + dtoa(vMaximumSuggestedPPI,0) + " recommended, 32 is ideal)";
			ISelectItemDialog.vNumberOfItems = 3;
			//TempCString = dtoa(vGlobalCurrentProduct->vImageWidthPosition, 2);
			CString TempCString;
			if ((vLocalSystemData->vLastBodyTriggerLength != 0) && (TempPulsesPerUnit != 0))				
				TempCString = dtoa(((vLocalSystemData->vLastBodyTriggerLength) / TempPulsesPerUnit), 2);
			else
				TempCString = "0";
			ISelectItemDialog.m_DialogTitleStaticText2 = "Calibrated Can Diameter: " + TempCString + " inches.";

			TempCString = dtoa(vEnteredContainerWidth, 2);
			ISelectItemDialog.m_DialogTitleStaticText5 = "Entered Can Diameter: " + TempCString + " inches" ;
			if ((vEnteredContainerWidth != 0) && ((vLocalSystemData->vLastBodyTriggerLength) != 0) && (TempPulsesPerUnit != 0))	// make sure not 0
			{
				double TempWidthPercent = 
					(((vEnteredContainerWidth - ((vLocalSystemData->vLastBodyTriggerLength) /TempPulsesPerUnit)) / vEnteredContainerWidth)*100);
				TempCString = dtoa(TempWidthPercent, 1);
				if ((TempWidthPercent > 10) || (TempWidthPercent < -10))
				{
					ISelectItemDialog.vMakeTitle3Highlighted = true;
				}
			}
			else
			{
				TempCString = "0";
			}
			ISelectItemDialog.m_DialogTitleStaticText3 = "Difference between Entered and Calibrated " + TempCString + " %";
			ISelectItemDialog.vBackgroundColor = cGreen;
			ISelectItemDialog.vGreenItem = 1;
			ISelectItemDialog.vInAutoSetup = false;
			ISelectItemDialog.vStayTillDecissionSetup = false;
			
			ISelectItemDialog.vItems[0] = "Accept";
			ISelectItemDialog.vItems[1] = "Return To Calibrate";
			ISelectItemDialog.vItems[2] = "Enter PPI Manually";

			//Pass control to dialog box and display
			vCalibrateEncoderMethod = cNotCalibrating;
			int nResponse = ISelectItemDialog.DoModal();
			//dialog box is now closed, if user pressed select do this
			//if user pressed cancel, do nothing
			if (nResponse == IDOK)
			{
				if (ISelectItemDialog.vItemNumberSelected == 1) // if user said OK use this value
				{
					vLocalConfigurationData->vCalibratedPixelsPerUnit = TempPulsesPerUnit;
					vPixelsPerUnit = TempPulsesPerUnit;
					vGlobalPixelsPerUnit = vPixelsPerUnit;

					//must set when change pixels per inch
					vGlobalCurrentProduct->SetProductBodyTriggerToImageBeltPositionOffset(vGlobalCurrentProduct->vProductBodyTriggerToImageBeltPositionOffset);

					vGlobalCurrentProduct->SetProductImageWidth(vGlobalCurrentProduct->vProductImageWidth);

					vGlobalCurrentProduct->SetProductLockOutWidth(vGlobalCurrentProduct->vProductLockOutWidth);

					vNeedToCalibrateEncoder = false;
					UpdateDisplay();
					SetChangeMade();
					//stop encoder calibration
					CalibrateEncoderWithXRaysNoProductionSelection();		//OnFunction3Button();
				}
				if (ISelectItemDialog.vItemNumberSelected == 2) // restore values and back to function
				{
					//stop encoder calibration
					CalibrateEncoderWithXRaysNoProductionSelection();		//OnFunction3Button();
					UpdateDisplay();
					
					CalibrateEncoderWithXRaysNoProductionSelection();		//OnFunction3Button();
				}
				if (ISelectItemDialog.vItemNumberSelected == 3) // Enter PPI Manually
				{
					//RestoreValues();
					//stop encoder calibration
					CalibrateEncoderWithXRaysNoProductionSelection();		//OnFunction3Button();
					OnSubFunction4Button();
				}
			}
			else 
			if (nResponse == 10)
			{	//stop encoder calibration								
				CalibrateEncoderWithXRaysNoProductionSelection();		//OnFunction3Button();
				UpdateDisplay();
				//Main Menu button pressed
				Sleep(1); //is the equivelent of a yield statement;
				RestoreValues();
				PrepareToExit();
				CDialog::EndDialog(10);
			}
			else //#define IDCANCEL            2
			{
				//stop encoder calibration
				CalibrateEncoderWithXRaysNoProductionSelection();		//OnFunction3Button();
				UpdateDisplay();
			}
//end a
			if ((vOriginalProduct) && (!((nResponse == IDOK) && (ISelectItemDialog.vItemNumberSelected == 2))))
			{
				vMainWindowPointer->SetupProduct(vOriginalProduct,false);
				vOriginalProduct = NULL;
			}
		}	// end if (TempEncoderCalibrationDone)
	}
}

void CSystemValuesDialog::OnSubFunction9Button() 
{
}

//was OnFunction3Button()
void CSystemValuesDialog::CalibrateEncoderWithXRaysNoProductionSelection() 
{
	// Verify no containers are going through


	// below original code for OnFunction3Button
	// run or stop calibrating encoder
	if (vLocalSystemData->vTestingTrigger)
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\n\nCalibrating Encoder Without X-Rays.";
		TempNoticeDialog.vType = cNoticeMessage;
		TempNoticeDialog.DoModal();
	}
	else
	if ((!vLocalSystemData->vXRayOnNotice) && (vLocalSystemData->vSystemRunMode == cStoppedSystemMode) && (!vCalibratingWithXRays))
	{
		if (vMainWindowPointer->InterlocksOK())
		{
//			SetDlgItemText(IDC_Function3Button,"Stop Calibrating");
			//add can size here
			// ask for can diameter
			CNumericEntryDialog INumericEntryDialog;  				
			INumericEntryDialog.vEditString = dtoa(vCalibrateEncoderContainerWidth, 2);
			double TempSavevCalibrateEncoderContainerWidth;
			TempSavevCalibrateEncoderContainerWidth = vCalibrateEncoderContainerWidth;

			//Set dialog box data titles and number value
			INumericEntryDialog.m_DialogTitleStaticText1 = "Enter the width (diameter) of a sample container";
			INumericEntryDialog.m_DialogTitleStaticText2 = " at the optical sensor height.";
			INumericEntryDialog.m_UnitsString = "Units in inches. Original Value: " + INumericEntryDialog.vEditString;

			INumericEntryDialog.m_DialogTitleStaticText3 = "Send container through after Save and Exit";
			INumericEntryDialog.m_DialogTitleStaticText4 = "You must re-size and learn all products after";

			INumericEntryDialog.vMaxValue = 100;
			INumericEntryDialog.vMinValue = 0;
			INumericEntryDialog.vIntegerOnly = false;
			//Pass control to dialog box and display
			int nResponse = INumericEntryDialog.DoModal();
			//dialog box is now closed, if user pressed select do this
			//if user pressed cancel, do nothing
			if (nResponse == IDOK)
			{
				vCalibrateEncoderContainerWidth = ATOF(INumericEntryDialog.vEditString);
				vEnteredContainerWidth = ATOF(INumericEntryDialog.vEditString);
			}
			else 
			if (nResponse == 10)
			{
				//Main Menu button pressed
				Sleep(1); //is the equivelent of a yeild statement;
				vCalibrateEncoderContainerWidth = TempSavevCalibrateEncoderContainerWidth;
				RestoreValues();
				PrepareToExit();
				CDialog::EndDialog(10);
			}
			if (nResponse == IDOK)
			{
				CProduct *TempProduct = vMainWindowPointer->GetProductByName("~Calibrate Encoder");

				if (!TempProduct)
				{

					//if don't find a calibrate product, create and save it, then use it
					if (ThereIsEnoughMemory(sizeof(CProduct), "New Calibrate Product "))
					{
						TempProduct = new CProduct;
						if (TempProduct)
						{
							TempProduct->SetProductName("~Calibrate Encoder");
							TempProduct->vProductImageHeightTop = (float)(vLocalConfigurationData->vDetectorLength * vLocalConfigurationData->vNumberOfXRayDetectors);
							TempProduct->SetProductBodyTriggerToImageBeltPositionOffset((float)4);
							TempProduct->SetProductImageWidth((float)96);
							TempProduct->SetProductLockOutWidth((float)100);
							TempProduct->SetEndOfLineTimeOut((float)(120 * vGlobalPixelsPerUnit + vGlobalEndOfLineTimeOutMargin));

							for (BYTE TempLoop = 0; TempLoop < cNumberOfEjectors; TempLoop++)
							{
									TempProduct->SetEjectorBeltPositionOffset(TempLoop,(float)(43));
							}

							TempProduct->vEdgeLocationBottom = (float)(.5);
							TempProduct->SetEdgeLocationBottom(TempProduct->vEdgeLocationBottom);

							if ((vLocalConfigurationData->vScanTracType == cSoloScanTrac) || (vGlobalScanTracType == cSoloScanTrac) || (vLocalConfigurationData->vScanTracType == cPiccoloScanTrac))//side view scantrac
								TempProduct->vEdgeLocationHeight = (float)(1);
							else
								TempProduct->vEdgeLocationHeight = (float)((vLocalConfigurationData->vDetectorLength * vLocalConfigurationData->vNumberOfXRayDetectors) - 2);
							TempProduct->SetEdgeLocationHeight(TempProduct->vEdgeLocationHeight);

							TempProduct->vXRaySourceVoltage = 50 / 6;  //50 kv

							//if (!vLocalConfigurationData->vScanTracType == cSoloScanTrac)
							//	TempProduct->vXRaySourceVoltage = 55 / 6;  //55 kv

							TempProduct->vXRaySourceCurrent = 4;	//2 mA
							if (TempProduct->vXRaySourceCurrent > vLocalConfigurationData->vMaximumCurrent)
								TempProduct->vXRaySourceCurrent = (float)vLocalConfigurationData->vMaximumCurrent;

							if (vLocalConfigurationData->vMaximumCurrent < TempProduct->vXRaySourceCurrent)
								TempProduct->vXRaySourceCurrent = (float)vLocalConfigurationData->vMaximumCurrent;

							if (vLocalConfigurationData->vMaximumVoltage < TempProduct->vXRaySourceVoltage)
								TempProduct->vXRaySourceVoltage = (float)vLocalConfigurationData->vMaximumVoltage;

							TempProduct->vXRayIntegrationTime = 750;  //.3 ms
							//TempProduct->vXRayIntegrationTimeAtEdge = TempProduct->vXRayIntegrationTime;
							TempProduct->vBodyTrigger = 1;
							TempProduct->CalculateEndOfLineTimeOut();
							vMainWindowPointer->AddProduct(((CProduct *)TempProduct));
						}
					}
				}
				if (TempProduct)
				{
					if (TempProduct->vProductImageWidth < 36)
					{
						TempProduct->SetProductImageWidth((float)96);
						TempProduct->SetProductLockOutWidth((float)100);
					}

					int nResponse = 0;
					//const double cSoloRightToLeftDistance = 16.5 //was 12.25;
					//const double cSoloLeftToRightDistance = 23;
					//const double cPiccoloDistance = 13;

					CSelectItemDialog ISelectItemDialog;  
					//Set dialog box data titles and number value
					ISelectItemDialog.vTitleString = "Choose ScanTrac Configuration";
					ISelectItemDialog.m_DialogTitleStaticText2 = "";
					ISelectItemDialog.m_DialogTitleStaticText3 = "";
					ISelectItemDialog.m_DialogTitleStaticText4 = "";
					ISelectItemDialog.vBackgroundColor = cGreen;
					ISelectItemDialog.vGreenItem = 1;
					ISelectItemDialog.vShowExit = 0;
					ISelectItemDialog.vInAutoSetup = vLocalSystemData->vInAutoSetup;
					ISelectItemDialog.m_DialogTitleStaticText3 = "Original Value: Custom, Distance: " + dtoa(vSystemBodyTriggerToXRayBeam,2);

					if ((vGlobalScanTracType == cAtempoScanTrac) || (vGlobalScanTracType == cAllegroScanTrac) || (vGlobalScanTracType == cForteScanTrac) || 
							(vGlobalScanTracType == cCaseInspectorScanTrac))
					{
						ISelectItemDialog.vNumberOfItems = 3;
						ISelectItemDialog.vItems[0] = "Atempo or Forte Trigger to Beam Distance: " + dtoa(cForteTriggerToBeamOffset,2);
						ISelectItemDialog.vItems[1] = "Allegro Trigger to Beam Distance: " + dtoa(cAllegroTriggerToBeamOffset,2);
						ISelectItemDialog.vItems[2] = "Custom";

						if (vGlobalScanTracType == cAllegroScanTrac)
							ISelectItemDialog.vGreenItem = 2;

						if (vSystemBodyTriggerToXRayBeam == cForteTriggerToBeamOffset)
							ISelectItemDialog.m_DialogTitleStaticText3 = "Original Value: Atempo or Forte, BT to X-Ray Distance: " + dtoa(cForteTriggerToBeamOffset,2);
						if (vSystemBodyTriggerToXRayBeam == cAllegroTriggerToBeamOffset)
							ISelectItemDialog.m_DialogTitleStaticText3 = "Original Value: Allegro, BT to X-Ray Distance: " + dtoa(cAllegroTriggerToBeamOffset,2);
					}
					else
					{
						ISelectItemDialog.vNumberOfItems = 4;
						ISelectItemDialog.vItems[0] = "Solo Containers Left To Right, BT to X-Ray Distance: " + dtoa(cSoloLeftToRightDistance,2);
						ISelectItemDialog.vItems[1] = "Solo Containers Right To Left, BT to X-Ray Distance: " + dtoa(cSoloRightToLeftDistance,2);
						ISelectItemDialog.vItems[2] = "Piccolo BT to X-Ray Distance: " + dtoa(cPiccoloDistance,2);
						ISelectItemDialog.vItems[3] = "Custom";
						if (vSystemBodyTriggerToXRayBeam == cSoloLeftToRightDistance)
							ISelectItemDialog.m_DialogTitleStaticText3 = "Original Value: Solo Containers Left To Right, BT to X-Ray Distance: " + 
								dtoa(cSoloLeftToRightDistance,2);
						else
						if (vSystemBodyTriggerToXRayBeam == cSoloRightToLeftDistance)
						{
							ISelectItemDialog.m_DialogTitleStaticText3 = "Original Value: Solo Containers Right To Left, BT to X-Ray Distance: " + 
								dtoa(cSoloRightToLeftDistance,2);
							ISelectItemDialog.vGreenItem = 2;
						}
						else
						if (vSystemBodyTriggerToXRayBeam == cPiccoloDistance)
						{
							ISelectItemDialog.m_DialogTitleStaticText3 = "Original Value: Piccolo, BT to X-Ray Distance: " + 
								dtoa(cPiccoloDistance,2);
							ISelectItemDialog.vGreenItem = 3;
						}
					}

					//Pass control to dialog box and display
					nResponse = ISelectItemDialog.DoModal();
					//dialog box is now closed, if user pressed select do this
					//if user pressed cancel, do nothing
					if (nResponse == IDOK)
					{
						SetChangeMade();
						bool TempDoCustom = false;
						if ((vGlobalScanTracType == cAtempoScanTrac) || (vGlobalScanTracType == cForteScanTrac) || (vGlobalScanTracType == cAllegroScanTrac) ||
							(vGlobalScanTracType == cCaseInspectorScanTrac))
						{
							if (ISelectItemDialog.vItemNumberSelected == 1)
							{
								vSystemBodyTriggerToXRayBeam = cForteTriggerToBeamOffset;
								//TempProduct->SetProductBodyTriggerToImageBeltPositionOffset((float)2);
								vSystemEjectorPositionOffset = cForteLeftToRightEjectorOffset;
							}
							else
							if (ISelectItemDialog.vItemNumberSelected == 2)
							{
								vSystemBodyTriggerToXRayBeam = cAllegroTriggerToBeamOffset;
								vSystemEjectorPositionOffset = cAllegroEjectorOffset;
							}
							else
								TempDoCustom = true;
						}
						else
						{
							if (ISelectItemDialog.vItemNumberSelected == 1)
							{
								vSystemBodyTriggerToXRayBeam = cSoloLeftToRightDistance;
								//TempProduct->SetProductBodyTriggerToImageBeltPositionOffset((float)12);
								vSystemEjectorPositionOffset = cSoloEjectorOffset;
							}
							else
							if (ISelectItemDialog.vItemNumberSelected == 2)
							{
								vSystemBodyTriggerToXRayBeam = cSoloRightToLeftDistance;
								//TempProduct->SetProductBodyTriggerToImageBeltPositionOffset((float)2);
								vSystemEjectorPositionOffset = cSoloEjectorOffset;
							}
							else
							if (ISelectItemDialog.vItemNumberSelected == 3)
							{
								vSystemBodyTriggerToXRayBeam = cPiccoloDistance;
								//TempProduct->SetProductBodyTriggerToImageBeltPositionOffset((float)2);
								vSystemEjectorPositionOffset = cPiccoloEjectorOffset;
							}
							else
								TempDoCustom = true;
						}
						if (TempDoCustom)
						{
							//enter distance from X-Ray Beam to body trigger
							CNumericEntryDialog INumericEntryDialog;  
						
							INumericEntryDialog.vEditString = dtoa(vSystemBodyTriggerToXRayBeam, 2);

							//Set dialog box data titles and number value
							INumericEntryDialog.m_DialogTitleStaticText1 = "Enter the distance from the ";
							INumericEntryDialog.m_DialogTitleStaticText2 = "Container Trigger to the X-Ray Beam";
							INumericEntryDialog.m_UnitsString = "Units in Inches. Original Value: " + INumericEntryDialog.vEditString;

							INumericEntryDialog.m_DialogTitleStaticText3 = "This is fixed for this ScanTrac";
							INumericEntryDialog.m_DialogTitleStaticText4 = "";

							INumericEntryDialog.vMaxValue = 100;
							INumericEntryDialog.vMinValue = 0;
							INumericEntryDialog.vIntegerOnly = false;
							//Pass control to dialog box and display
							nResponse = INumericEntryDialog.DoModal();
							//dialog box is now closed, if user pressed select do this
							//if user pressed cancel, do nothing
							if (nResponse == IDOK)
							{
								vSystemBodyTriggerToXRayBeam = ATOF(INumericEntryDialog.vEditString);
								//if (vSystemBodyTriggerToXRayBeam > 15)
								//	TempProduct->SetProductBodyTriggerToImageBeltPositionOffset((float)vSystemBodyTriggerToXRayBeam - 12);
								//else
								//	TempProduct->SetProductBodyTriggerToImageBeltPositionOffset((float)2);

								//enter the ejector offset
								INumericEntryDialog.vEditString = dtoa(vSystemEjectorPositionOffset, 2);

								//Set dialog box data titles and number value
								INumericEntryDialog.m_DialogTitleStaticText1 = "Enter the ejector offset";
								INumericEntryDialog.m_DialogTitleStaticText2 = "Distance from Container Trigger to the wall";
								INumericEntryDialog.m_UnitsString = "Units in Inches. Original Value: " + INumericEntryDialog.vEditString;

								INumericEntryDialog.m_DialogTitleStaticText3 = "of the output side of the ScanTrac box.";
								INumericEntryDialog.m_DialogTitleStaticText4 = "This is fixed for this ScanTrac.  Normally 37.5";

								INumericEntryDialog.vMaxValue = 100;
								INumericEntryDialog.vMinValue = 0;
								INumericEntryDialog.vIntegerOnly = false;
								//Pass control to dialog box and display
								nResponse = INumericEntryDialog.DoModal();
								//dialog box is now closed, if user pressed select do this
								//if user pressed cancel, do nothing
								if (nResponse == IDOK)
								{
									vSystemEjectorPositionOffset = ATOF(INumericEntryDialog.vEditString);
								}
								else 
								if (nResponse == 10)
								{
									//Main Menu button pressed
									Sleep(1); //is the equivelent of a yeild statement;
									RestoreValues();
									PrepareToExit();
									CDialog::EndDialog(10);
								}
							}
							else 
							if (nResponse == 10)
							{
								//Main Menu button pressed
								Sleep(1); //is the equivelent of a yeild statement;
								RestoreValues();
								PrepareToExit();
								CDialog::EndDialog(10);
							}
						}
						//ask user to enter distance to each ejector
						for (BYTE TempLoop = 0; TempLoop < cNumberOfEjectors; TempLoop++)
						if (vLocalConfigurationData->vEjector[TempLoop].vEnabled)
						{
							INumericEntryDialog.vEditString = dtoa(vSystemEjectorDistance[TempLoop], 2);

							//Set dialog box data titles and number value
							INumericEntryDialog.m_DialogTitleStaticText1 = "Enter the distance from the ";

							INumericEntryDialog.m_DialogTitleStaticText2 = "ScanTrac Output Wall to center of Ejector " + dtoa(TempLoop + 1, 0);

							INumericEntryDialog.m_UnitsString = "Units in Inches. Original Value: " + INumericEntryDialog.vEditString;

							INumericEntryDialog.m_DialogTitleStaticText3 = "This is fixed for this ScanTrac";
							INumericEntryDialog.m_DialogTitleStaticText4 = "";

							INumericEntryDialog.vMaxValue = 200;
							INumericEntryDialog.vMinValue = 0;
							INumericEntryDialog.vIntegerOnly = false;
							//Pass control to dialog box and display
							nResponse = INumericEntryDialog.DoModal();
							//dialog box is now closed, if user pressed select do this
							//if user pressed cancel, do nothing
							if (nResponse == IDOK)
							{
								vSystemEjectorDistance[TempLoop] = ATOF(INumericEntryDialog.vEditString);
							}
						}
					}
					else 
					if (nResponse == 10)
					{
						//Main Menu button pressed
						Sleep(1); //is the equivelent of a yeild statement;
						RestoreValues();
						PrepareToExit();
						CDialog::EndDialog(10);
					}

					//calibrate encoder, don't do on pipelines
					if (nResponse == IDOK)
					{
						if ((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || 
							(vGlobalScanTracType == cLegatoScanTrac) || (vGlobalScanTracType == cFermataScanTrac))//a Continuous Feed, or a Pipeline
						{
							vNeedToCalibrateEncoder = false;
						}
						else
						{
							//must set when change pixels per inch so set incase user changed
							vOriginalProduct = vGlobalCurrentProduct;

							TempProduct->SetProductBodyTriggerToImageBeltPositionOffset(TempProduct->vProductBodyTriggerToImageBeltPositionOffset);

							TempProduct->SetProductImageWidth((float)96);
							TempProduct->SetProductLockOutWidth((float)100);
							//TempProduct->SetProductImageWidth(TempProduct->vProductImageWidth);

							//TempProduct->SetProductLockOutWidth(TempProduct->vProductLockOutWidth);

	//							if ((vLocalSystemData->vShiftRateCPM) <= 0)	//check no cans being processed
	//							{
								CNoticeDialog TempNoticeDialog;
								TempNoticeDialog.vNoticeText = "\n\n\nOnce X-Rays are On,\nplease send one container through the ScanTrac";
								TempNoticeDialog.vType = cNoticeMessage;
								TempNoticeDialog.DoModal();

								vCalibratingWithXRays = true;
								vCalibrateEncoderMethod = cCalXrayOnNoProduction;		//enum {cNotCalibrating, cCalXrayOnNoProduction, cCalXrayOffYesProduction};
								SetDlgItemText(IDC_Function3Button, _T("Stop Calibrating"));

								vMainWindowPointer->SetupProduct(TempProduct,false );

								vMainWindowPointer->ResetCounters();

								//Send Encoder Divisor
								// already sent kjh 11/27/12 SendEncoderDivider();

								if (vMainWindowPointer->PrepareToRun())
								{
									CString TempText = "Calibrate";
									m_Calibrating.ShowWindow(SW_SHOW);
									SetDlgItemText(IDC_Calibrating,TempText);
									//SetDlgItemText(IDC_Function3Button,"Wait Calibrating");

									vLocalSystemData->vApplyGainAndOffsetDuringCalibration = false;
									vLocalSystemData->vModeToRunAfterCalibrateOffset = cCalibrateEncoderMode;
									//this will calibrate detectors, then do measure Align Tube
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
								//UpdateButtons();
								//CalculateCalibration();	// line temp for testing kjh
	//							}
	//							else		// cans running, cancel
	//							{
	//								vCalibratingWithXRays = false;
	//								vCalibrateEncoderMethod = cNotCalibrating;		//enum {cNotCalibrating, cCalXrayOnNoProduction, cCalXrayOffYesProduction};
	//								vMainWindowPointer->StopRunning(true);
	//								CNoticeDialog TempNoticeDialog;
	//								TempNoticeDialog.vNoticeText = "\n\n\nProduction is running,\nplease Stop Production and restart Encoder Calibration";
	//								TempNoticeDialog.vType = cNoticeMessage;
	//								TempNoticeDialog.DoModal();
	//							}
						}	// end else -- not a pipeline type
					}		//end if for response ok from ScanTrac type selection
				}			// end if Temp Product exists or was created
			}				// end if for response ok from container size create numeric dialog
		}					// end if interlocks are okay
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
	}	//end else for triggers on, so no triggers on at start of this else, also if (!vLocalSystemData->vXRayOnNotice) && (vLocalSystemData->vSystemRunMode == cStoppedSystemMode) && (!vCalibratingWithXRays)
	else
	{
		vCalibratingWithXRays = false;
		vCalibrateEncoderMethod = cNotCalibrating;		//enum {cNotCalibrating, cCalXrayOnNoProduction, cCalXrayOffYesProduction};
		vMainWindowPointer->StopRunning(true);
		m_Calibrating.ShowWindow(SW_HIDE);

		//CNoticeDialog TempNoticeDialog;
		//TempNoticeDialog.vNoticeText = "\n\n\nScanTrac was not Setup for Calibration.";
		//TempNoticeDialog.vType = cNoticeMessage;
		//TempNoticeDialog.DoModal();

//		vLocalSystemData->vSystemRunMode = cStoppedSystemMode;
//
//		::PostThreadMessage(vGlobalCallingThreadID,cXRaysOnMessage,0,0);
//		vMainWindowPointer->SendXRayPowerOnOffToUController(0);
//
//		vMainWindowPointer->StopImageAcquisition();
//		
	}
//	UpdateButtons();
//	UpdateDisplay();
}
//

// send value in vEncoderDivider to uC 
//void CSystemValuesDialog::SendEncoderDivider()
//{
//	//Send Encoder Divisor
//	//vLocalConfigurationData->vEncoderDivider = vEncoderDivider;		// save that it was changed on uC board
//	tSerialCommand TempCommand;
//	TempCommand[0] = 0x4F;
//	TempCommand[1] = (BYTE)vEncoderDivider;
//	TempCommand[2] = 0;
//	TempCommand[3] = 0;
//	vGlobaluCSerialPort->SendSerialCommand(TempCommand);
//	Sleep(cSendCommandSleepTimeSetup);
//}



void CSystemValuesDialog::OnDialogtitlestatictext1()
{
	BYTE TempHiddenKeys = 8;
	if (!PasswordOK(cTemporaryInspxPassword,false))
		TempHiddenKeys = 10;
	PostThreadMessage(vGlobalCallingThreadID, cDisplaySoftKeyMenu, TempHiddenKeys, TempHiddenKeys);
}
