// FillerSeamerSamplingDialog.cpp : implementation file
//

#include "stdafx.h"
#include "ScanTrac.h"
#include "FillerSeamerSamplingDialog.h"
#include "afxdialogex.h"
#include "NumericEntryDialog.h"
#include "SamplingDialog.h"
#include "PasswordEntryDialog.h"
#include "AutoSetupImageDisplayDialog.h"
#include "YesNoDialog.h"
#include "FillerValveDialog.h"
#include "SerialPort.h"
#include "HelpDialog.h"

// CFillerSeamerSamplingDialog dialog

IMPLEMENT_DYNAMIC(CFillerSeamerSamplingDialog, CDialogEx)

extern CSerialPort *vGlobaluCSerialPort;
extern BYTE vGlobalShiftKeyDown;
extern CProduct *vGlobalCurrentProduct;
extern unsigned long vGlobalDialogTitleColor;

CFillerSeamerSamplingDialog::CFillerSeamerSamplingDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFillerSeamerSamplingDialog::IDD, pParent)
{
	vMainWindowPointer = NULL;
	//vLocalProductCollection = NULL;
	vLocalSystemData = NULL;
	vLocalConfigurationData = NULL;
	vOldFillerAverageWeight = -1;

	vChangeMade = false;
	vOriginalFillerTolerance = 0;
	vOldNumberOfValvesThatNeedAttention = -1;
	vFillerIndexErrorsX = 0;
	vOldFillerIndexErrorsX = 0;
	vFillerIndexErrorsY = 0;
	vOldFillerIndexErrorsY = 0;
	vSeamerIndexErrorsX = 0;
	vOldSeamerIndexErrorsX = 0;
	vSeamerIndexErrorsY = 0;
	vOldSeamerIndexErrorsY = 0;
	vUpdateDisplayTimerHandle = 31982;
	for (BYTE TempLoop = 0; TempLoop < 3; TempLoop++)
	{
		vOldSampleIndexErrorsX[TempLoop] = -1;
		vOldSampleIndexErrorsY[TempLoop] = -1;
		vOldNextHeadToProcess[TempLoop] = -1;
		vOldMostRecentTimeBeforeIndexPulse[TempLoop] = 0;
		vOldMostRecentTimeAfterIndexPulse[TempLoop] = 0;
	}
	vOldValveDataRowIndex = 0xFFFF;
	vOldNumberValveRowDataHave = 0;

	vLocalCWndCollection.Add(this);
	vLocalCWndCollection.Add(&m_Function2Button);
	vLocalCWndCollection.Add(&m_Function3Button);
	vLocalCWndCollection.Add(&m_Function4Button);
	vLocalCWndCollection.Add(&m_Function5Button);
	vLocalCWndCollection.Add(&m_SubFunction8Button);
	vLocalCWndCollection.Add(&m_Background);
	vLocalCWndCollection.Add(&m_Function3Display);
	vLocalCWndCollection.Add(&m_FillerBeforeIndexDisplay);
	vLocalCWndCollection.Add(&m_SeamerAfterIndexDisplay);
	vLocalCWndCollection.Add(&m_SeamerBeforeIndexDisplay);
	vLocalCWndCollection.Add(&m_FillerAfterIndexDisplay);
	vLocalCWndCollection.Add(&m_Function1Button);
	vLocalCWndCollection.Add(&m_SubFunction1Button);
	vLocalCWndCollection.Add(&m_SubFunction2Button);
	vLocalCWndCollection.Add(&m_SubFunction1Display);
	vLocalCWndCollection.Add(&m_SubFunction2Display);
	vLocalCWndCollection.Add(&m_SubFunction3Button);
	vLocalCWndCollection.Add(&m_SubFunction3Display);
	vLocalCWndCollection.Add(&m_SubFunction4Button);
	vLocalCWndCollection.Add(&m_SubFunction4Display);
	vLocalCWndCollection.Add(&m_SubFunction5Button);
	vLocalCWndCollection.Add(&m_SubFunction6Button);
	vLocalCWndCollection.Add(&m_SubFunction7Button);
	vLocalCWndCollection.Add(&m_ClockDisplay);
	vLocalCWndCollection.Add(&m_DialogTitleStaticText1);
	vLocalCWndCollection.Add(&m_ConveyorSpeedError);
}

CFillerSeamerSamplingDialog::~CFillerSeamerSamplingDialog()
{
}

void CFillerSeamerSamplingDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_Function3Button, m_Function3Button);
	DDX_Control(pDX, IDC_Function2Button, m_Function2Button);
	DDX_Control(pDX, IDC_Function4Button, m_Function4Button);
	DDX_Control(pDX, IDC_Background, m_Background);
	DDX_Control(pDX, IDC_Function3Display, m_Function3Display);
	//  DDX_Control(pDX, IDC_FillerAfterIndexDisplay, m_FillAfterIndexDisplay);
	DDX_Control(pDX, IDC_FillerBeforeIndexDisplay, m_FillerBeforeIndexDisplay);
	DDX_Control(pDX, IDC_SeamerAfterIndexDisplay, m_SeamerAfterIndexDisplay);
	DDX_Control(pDX, IDC_SeamerBeforeIndexDisplay, m_SeamerBeforeIndexDisplay);
	DDX_Control(pDX, IDC_FillerAfterIndexDisplay, m_FillerAfterIndexDisplay);
	DDX_Control(pDX, IDC_SubFunction8Button, m_SubFunction8Button);
	DDX_Control(pDX, IDC_Function5Button, m_Function5Button);
	DDX_Control(pDX, IDC_Function1Button, m_Function1Button);
	DDX_Control(pDX, IDC_SubFunction1Button, m_SubFunction1Button);
	DDX_Control(pDX, IDC_SubFunction2Button, m_SubFunction2Button);
	DDX_Control(pDX, IDC_SubFunction1Display, m_SubFunction1Display);
	DDX_Control(pDX, IDC_SubFunction2Display, m_SubFunction2Display);
	DDX_Control(pDX, IDC_SubFunction3Button, m_SubFunction3Button);
	DDX_Control(pDX, IDC_SubFunction3Display, m_SubFunction3Display);
	DDX_Control(pDX, IDC_SubFunction4Button, m_SubFunction4Button);
	DDX_Control(pDX, IDC_SubFunction4Display, m_SubFunction4Display);
	DDX_Control(pDX, IDC_SubFunction5Button, m_SubFunction5Button);
	DDX_Control(pDX, IDC_SubFunction6Button, m_SubFunction6Button);
	DDX_Control(pDX, IDC_SubFunction7Button, m_SubFunction7Button);
	DDX_Control(pDX, IDC_ClockDisplay, m_ClockDisplay);
	DDX_Control(pDX, IDC_DialogTitleStaticText1, m_DialogTitleStaticText1);
	DDX_Control(pDX, IDC_ConveyorSpeedError, m_ConveyorSpeedError);
}


BEGIN_MESSAGE_MAP(CFillerSeamerSamplingDialog, CDialogEx)
	ON_STN_CLICKED(IDC_Function1Button, &CFillerSeamerSamplingDialog::OnFunction1Button)
	ON_STN_CLICKED(IDC_Function2Button, &CFillerSeamerSamplingDialog::OnFunction2Button)
	ON_STN_CLICKED(IDC_Function3Button, &CFillerSeamerSamplingDialog::OnFunction3Button)
	ON_STN_CLICKED(IDC_Function4Button, &CFillerSeamerSamplingDialog::OnFunction4Button)
	ON_STN_CLICKED(IDC_Function5Button, &CFillerSeamerSamplingDialog::OnFunction5Button)
	ON_STN_CLICKED(IDC_SubFunction1Button, &CFillerSeamerSamplingDialog::OnSubFunction1Button)
	ON_STN_CLICKED(IDC_SubFunction2Button, &CFillerSeamerSamplingDialog::OnSubFunction2Button)
	ON_STN_CLICKED(IDC_SubFunction3Button, &CFillerSeamerSamplingDialog::OnSubFunction3Button)
	ON_STN_CLICKED(IDC_SubFunction4Button, &CFillerSeamerSamplingDialog::OnSubFunction4Button)
	ON_STN_CLICKED(IDC_SubFunction5Button, &CFillerSeamerSamplingDialog::OnSubFunction5Button)
	ON_STN_CLICKED(IDC_SubFunction6Button, &CFillerSeamerSamplingDialog::OnSubFunction6Button)
	ON_STN_CLICKED(IDC_SubFunction7Button, &CFillerSeamerSamplingDialog::OnSubFunction7Button)
	ON_STN_CLICKED(IDC_SubFunction8Button, &CFillerSeamerSamplingDialog::OnSubFunction8Button)
	ON_WM_SHOWWINDOW()
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_STN_CLICKED(IDC_DialogTitleStaticText1, &CFillerSeamerSamplingDialog::OnDialogtitlestatictext1)
END_MESSAGE_MAP()


void CFillerSeamerSamplingDialog::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: Add your message handler code here
	m_Background.SetWindowPos(NULL,cMainBackgroundLeft, cMainBackgroundTop, cMainBackgroundLength, cMainBackgroundHeight, SWP_NOZORDER);
	vOriginalFillerTolerance = vGlobalCurrentProduct->vFillerTolerance;

	m_Function3Button.ShowWindow(SW_SHOW);
	m_Function3Display.ShowWindow(SW_SHOW);

	if (!PasswordOK(cTemporaryInspxPassword,false))
	{
		m_FillerBeforeIndexDisplay.ShowWindow(SW_HIDE);
		m_FillerAfterIndexDisplay.ShowWindow(SW_HIDE);
		m_SeamerBeforeIndexDisplay.ShowWindow(SW_HIDE);
		m_SeamerAfterIndexDisplay.ShowWindow(SW_HIDE);
	}

	UpdateDisplay();
	OneSecondDisplayUpdate();
	this->SetWindowText(_T("FillSeamSample"));

	int TempTimerResult = SetTimer(vUpdateDisplayTimerHandle,1000,NULL);
	if (!TempTimerResult)
		ReportErrorMessage("Error-Display Timer SD Failed",cEMailInspx,32000);

	if ((vGlobalCurrentProduct->vFillerMonitorInspection) || (PasswordOK(cSuperInspxPassword,false)))
	{
		m_Function5Button.ShowWindow(SW_SHOW);
	}
	else
	{
		m_Function5Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_Function5Button.GetRectToInvalidateArrow());
	}

	//if right conveyor speed hide error status box else wrong conveyor speed show message
	if (vLocalSystemData->vUseSamplingFIFO)
		m_ConveyorSpeedError.ShowWindow(SW_HIDE);
	else
	if (vGlobaluCSerialPort->GetIndexForThisEncoderRate(vLocalSystemData->vActualEncoderRate) < cNumberOfFillerOffsetSpeeds) //if not running one of configured encoder rates, indicate an error
	//if ((((double)(100 + vLocalConfigurationData->vEncoderRateTolerance) / 100.0) * (double)vGlobalCurrentProduct->vEncoderRateIndexesForFillerSeamerSampling[0] > vLocalSystemData->vActualEncoderRate)
	//		&& (((double)(100 - vLocalConfigurationData->vEncoderRateTolerance) / 100.0) * (double)vGlobalCurrentProduct->vEncoderRateIndexesForFillerSeamerSampling[0] < vLocalSystemData->vActualEncoderRate)
	//	|| (((double)(100 + vLocalConfigurationData->vEncoderRateTolerance) / 100.0) * (double)vGlobalCurrentProduct->vEncoderRateIndexesForFillerSeamerSampling[1] > vLocalSystemData->vActualEncoderRate)
	//		&& (((double)(100 - vLocalConfigurationData->vEncoderRateTolerance) / 100.0) * (double)vGlobalCurrentProduct->vEncoderRateIndexesForFillerSeamerSampling[1] < vLocalSystemData->vActualEncoderRate))
	{
		m_ConveyorSpeedError.ShowWindow(SW_HIDE);
	}
	else if (vLocalSystemData->vActualEncoderRate == 0)
	{
	//	SetDlgItemText(IDC_ConveyorSpeedError, "\nConveyor is not running, turn on to get filler data");
		m_ConveyorSpeedError.ShowWindow(SW_HIDE);
	}
	else
	{
		SetDlgItemText(IDC_ConveyorSpeedError, "Conveyor Speed out of Range; Encoder Rate is " + dtoa(vLocalSystemData->vActualEncoderRate) + "\nAcceptable Rates are "
			+ dtoa( ((100 + ((double)vLocalConfigurationData->vEncoderRateTolerance))/100) * ((double) vGlobalCurrentProduct->vEncoderRateIndexesForFillerSeamerSampling[0]), 0 ) + " - "
			+ dtoa( ((100 - ((double)vLocalConfigurationData->vEncoderRateTolerance))/100) * ((double) vGlobalCurrentProduct->vEncoderRateIndexesForFillerSeamerSampling[0]), 0 ) + "Hz, " 
			+ dtoa( ((100 + ((double)vLocalConfigurationData->vEncoderRateTolerance))/100) * ((double) vGlobalCurrentProduct->vEncoderRateIndexesForFillerSeamerSampling[1]), 0 ) + " - "
			+ dtoa( ((100 - ((double)vLocalConfigurationData->vEncoderRateTolerance))/100) * ((double) vGlobalCurrentProduct->vEncoderRateIndexesForFillerSeamerSampling[1]), 0 ) + "Hz, " 
			+ dtoa( ((100 + ((double)vLocalConfigurationData->vEncoderRateTolerance))/100) * ((double) vGlobalCurrentProduct->vEncoderRateIndexesForFillerSeamerSampling[2]), 0 ) + " - "
			+ dtoa( ((100 - ((double)vLocalConfigurationData->vEncoderRateTolerance))/100) * ((double) vGlobalCurrentProduct->vEncoderRateIndexesForFillerSeamerSampling[2]), 0 ) + "Hz, and " 
			+ dtoa( ((100 + ((double)vLocalConfigurationData->vEncoderRateTolerance))/100) * ((double) vGlobalCurrentProduct->vEncoderRateIndexesForFillerSeamerSampling[3]), 0 ) + " - "
			+ dtoa( ((100 - ((double)vLocalConfigurationData->vEncoderRateTolerance))/100) * ((double) vGlobalCurrentProduct->vEncoderRateIndexesForFillerSeamerSampling[3]), 0 ) + "Hz." );
		m_ConveyorSpeedError.ShowWindow(SW_SHOW);
	}
	SetupMenu(vLocalCWndCollection);
}


// CFillerSeamerSamplingDialog message handlers


void CFillerSeamerSamplingDialog::OnFunction1Button()
{
	//Main Menu was pressed
	if ((!vChangeMade) || (AbortSetupChanges()))
	{
		vGlobalCurrentProduct->vFillerTolerance = vOriginalFillerTolerance;
		KillTimer(vUpdateDisplayTimerHandle);
		CDialog::EndDialog(10);
	}
}


void CFillerSeamerSamplingDialog::OnFunction2Button()
{
	//Save and exit was pressed
	if (vChangeMade) 
	if (KeepSetupChanges())
	{
		CDialog::EndDialog(11);
	}
}


void CFillerSeamerSamplingDialog::OnFunction3Button()
{
	if (PasswordOK(cSetupPassword,true))
	if (vLocalSystemData->vFillerIndexPulseBitMap)
	{
		CInspection *TempInspection = NULL;
		if (vLocalSystemData->vValveMonitorInspection <= vGlobalCurrentProduct->vNumberOfInspections)
		if ((vLocalSystemData->vValveMonitorInspection < vGlobalCurrentProduct->vNumberOfInspections) && (vGlobalCurrentProduct) && (vGlobalCurrentProduct->vInspection[vLocalSystemData->vValveMonitorInspection]))
			TempInspection = vGlobalCurrentProduct->vInspection[vLocalSystemData->vValveMonitorInspection];
		if (!TempInspection)
			TempInspection = vGlobalCurrentProduct->vFillerMonitorInspection;

		CNumericEntryDialog INumericEntryDialog;  
				
		//Set dialog box data titles and number value
		INumericEntryDialog.vEditString = dtoa(vGlobalCurrentProduct->vFillerTolerance, 2);

		INumericEntryDialog.m_DialogTitleStaticText1 = "Enter Filler Tolerance";
		INumericEntryDialog.m_DialogTitleStaticText2 = "When Valve SD exceeds this value, Valve marked 'Var'";
		CString TempText = " ";
		TempText.LoadString(IDS_Product);
		INumericEntryDialog.m_DialogTitleStaticText3 = TempText + ": " + *vGlobalCurrentProduct->GetProductName();
		TempText.LoadString(IDS_OriginalValue);
		INumericEntryDialog.m_DialogTitleStaticText4 = TempText + ": " + INumericEntryDialog.vEditString + ", 0 = Disabled (uses 3 sigma)";

		if (TempInspection)
			INumericEntryDialog.m_UnitsString = "Measured in " + TempInspection->vCheckWeighUnits;
		else
			INumericEntryDialog.m_UnitsString = "Measured in brightness units 0 to 255";
		INumericEntryDialog.vIntegerOnly = false;
		INumericEntryDialog.vMaxValue = 60000;
		INumericEntryDialog.vMinValue = 0;
		//Pass control to dialog box and display
		int nResponse = INumericEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			vGlobalCurrentProduct->vFillerTolerance = ATOF(INumericEntryDialog.vEditString);
			vOldValveDataRowIndex = 0xFFFF; //force it to re-calculate the valve status
			SetChangeMade();
			UpdateDisplay();
		}
		else 
		if (nResponse == 10)
		{
			//Main Menu button pressed
			KillTimer(vUpdateDisplayTimerHandle);
			CDialog::EndDialog(10);
		}
	}
	else
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\n\nSeamer Sampling Input Not Configured.\nContact Inspx.";
		TempNoticeDialog.vType = cNoticeMessage;
		TempNoticeDialog.DoModal();
	}
}


void CFillerSeamerSamplingDialog::OnFunction4Button()
{
	//Cancel was pressed
	if (vChangeMade)
	if ((!vChangeMade) || (AbortSetupChanges()))
	{
		//PrepareToExit();
		vGlobalCurrentProduct->vFillerTolerance = vOriginalFillerTolerance;
		CDialog::OnCancel();
	}
}


void CFillerSeamerSamplingDialog::OnFunction5Button()
{

	if ((vGlobalShiftKeyDown) && (vGlobalCurrentProduct->vFillerMonitorInspection) && (PasswordOK(cSuperInspxPassword,false)))
	{ //delete inspection
		CYesNoDialog TempYesNoDialog;
		TempYesNoDialog.vNoticeText = _T("\nAre you sure you want to delete\nthe Filler Monitor Inspection?\nAn under fill or under weight inspection\nwill be used if available.");
		TempYesNoDialog.vYesButtonText = "Delete Inspection!";
		TempYesNoDialog.vNoButtonText = "Cancel";
		TempYesNoDialog.vQuestionType = cConfirmQuestion;
		int TempResult = TempYesNoDialog.DoModal();
		if (TempResult == IDOK)
		{
			if (vLocalSystemData->vITIPCDig->vContainerCollectionLock.Lock())
			{
				delete vGlobalCurrentProduct->vFillerMonitorInspection;
				vGlobalCurrentProduct->vFillerMonitorInspection = NULL;
				vMainWindowPointer->SelectValveMonitoringInspection();
				vMainWindowPointer->ResetValveMonitoringData();
				vLocalSystemData->vITIPCDig->vContainerCollectionLock.Unlock();
			}
		vOldValveDataRowIndex = 0xFFFF; //force it to update the status of the valves
		UpdateDisplay();
		}
	}
	else
	if ((!vGlobalCurrentProduct->vFillerMonitorInspection))//don't have an inspection, so create it
	{
		if  (PasswordOK(cSuperInspxPassword,true))
		{
			CYesNoDialog TempYesNoDialog;
			TempYesNoDialog.vNoticeText = _T("\nAre you sure you want to create\na Filler Monitor Inspection?\nYou must evaluate it before it can work, and it does not eject anything. It just monitors the fill weights.");
			TempYesNoDialog.vYesButtonText = "Create Inspection!";
			TempYesNoDialog.vNoButtonText = "Cancel";
			TempYesNoDialog.vQuestionType = cConfirmQuestion;
			int TempResult = TempYesNoDialog.DoModal();
			if (TempResult == IDOK)
			if (vLocalSystemData->vITIPCDig->vContainerCollectionLock.Lock())
			{
				if (ThereIsEnoughMemory(sizeof(CInspectionDensity), "New Inspection"))
					vGlobalCurrentProduct->vFillerMonitorInspection = new CInspectionDensity;
				if (vGlobalCurrentProduct->vFillerMonitorInspection) //have an inspection, so edit it
				{
	/*				vGlobalCurrentProduct->vFillerMonitorInspection->vInspectionType = cDensityInspection;
					vGlobalCurrentProduct->vFillerMonitorInspection->vHasDensityLowerThreshold = true;
					vGlobalCurrentProduct->vFillerMonitorInspection->vHasDensityUpperThreshold = true;
					vGlobalCurrentProduct->vFillerMonitorInspection->vName = "Filler Monitor";
					if ((vGlobalCurrentProduct->vReferenceRight) && (vGlobalCurrentProduct->vReferenceTop))
						vGlobalCurrentProduct->vFillerMonitorInspection->SetupInspectionInPixels((WORD)((double)vGlobalCurrentProduct->vReferenceTop), (WORD)((double)vGlobalCurrentProduct->vReferenceTop * 0.5), 
						vGlobalCurrentProduct->vReferenceLeft, vGlobalCurrentProduct->vReferenceRight, vGlobalCurrentProduct->vOverScanMultiplier);
					else
					{
						int TempHeight = (int)((vGlobalCurrentProduct->vProductImageHeightTop - vGlobalCurrentProduct->vProductImageHeightBottom) * vGlobalPixelsPerUnitInHeight);
						vGlobalCurrentProduct->vFillerMonitorInspection->SetupInspectionInPixels((int)((double)TempHeight), (int)((double)TempHeight * 0.5), 2, vGlobalCurrentProduct->vImageWidthPosition - 2, vGlobalCurrentProduct->vOverScanMultiplier);
					}
					vGlobalCurrentProduct->vFillerMonitorInspection->vDensityThresholdUpper = 220;
					vGlobalCurrentProduct->vFillerMonitorInspection->vDensityThresholdLower = 90; 
					vGlobalCurrentProduct->vFillerMonitorInspection->vThreshold = 220;
					vGlobalCurrentProduct->vFillerMonitorInspection->SetDensityLowerThreshold(90);
					*/
					CInspection *TempInspection = NULL;
					CInspectionCheckWeigh *TempInspectionToEditPointer = NULL;
					if (ThereIsEnoughMemory(sizeof(CInspectionCheckWeigh), "New Inspection"))
						TempInspectionToEditPointer = new CInspectionCheckWeigh;
					vGlobalCurrentProduct->vFillerMonitorInspection = TempInspectionToEditPointer;

					vGlobalCurrentProduct->vFillerMonitorInspection->vInspectionType = cUnderfillByWeightInspection;
					vGlobalCurrentProduct->vFillerMonitorInspection->vDensityThresholdUpper = 0;//don't want it to start ejecting until calibrated, so set to zero, don't know what is good value, user must enter
					vGlobalCurrentProduct->vFillerMonitorInspection->vHasDensityUpperThreshold = true;
					vGlobalCurrentProduct->vFillerMonitorInspection->vHasDensityLowerThreshold = false;
					vGlobalCurrentProduct->vFillerMonitorInspection->vName = "Fill Monitor Under Weight";

					if ((vGlobalCurrentProduct->vReferenceRight) && (vGlobalCurrentProduct->vReferenceTop))
						vGlobalCurrentProduct->vFillerMonitorInspection->SetupInspectionInPixels((WORD)((double)vGlobalCurrentProduct->vReferenceTop), (WORD)((double)vGlobalCurrentProduct->vReferenceTop * 0.5), 
						vGlobalCurrentProduct->vReferenceLeft, vGlobalCurrentProduct->vReferenceRight, vGlobalCurrentProduct->vOverScanMultiplier);
					else
					{
						int TempHeight = (int)((vGlobalCurrentProduct->vProductImageHeightTop - vGlobalCurrentProduct->vProductImageHeightBottom) * vGlobalPixelsPerUnitInHeight);
						vGlobalCurrentProduct->vFillerMonitorInspection->SetupInspectionInPixels((int)((double)TempHeight), (int)((double)TempHeight * 0.5), 2, vGlobalCurrentProduct->vImageWidthPosition - 2, vGlobalCurrentProduct->vOverScanMultiplier);
					}
				}
				vMainWindowPointer->SelectValveMonitoringInspection();
				vMainWindowPointer->ResetValveMonitoringData();
				vLocalSystemData->vITIPCDig->vContainerCollectionLock.Unlock();
			}
		}
	}
	if ((vGlobalCurrentProduct->vFillerMonitorInspection) && (PasswordOK(cSetupPassword,true))) //have an inspection, so edit it
	{
		CAutoSetupImageDisplayDialog IAutoSetupImageDisplayDialog;
		IAutoSetupImageDisplayDialog.vLocalConfigurationData = vLocalConfigurationData;
		IAutoSetupImageDisplayDialog.vLocalSystemData = vLocalSystemData;
		IAutoSetupImageDisplayDialog.vMainWindowPointer = vMainWindowPointer;
		IAutoSetupImageDisplayDialog.vShowReturnToEditMenu = true;
		IAutoSetupImageDisplayDialog.vInspectionEditing = vGlobalCurrentProduct->vFillerMonitorInspection;
		IAutoSetupImageDisplayDialog.vAutoSetupType = cEvaluateAutoSetup;
		IAutoSetupImageDisplayDialog.vEditOnlyThisInspection = true;
		//when go into auto setup, it will go into evaluate inspection
		//IAutoSetupImageDisplayDialog.vMode = cSetDensityThreshold;
		IAutoSetupImageDisplayDialog.vMode = cSetROI;
		int nResponse = IAutoSetupImageDisplayDialog.DoModal();
		if (nResponse == 10) //abort
		{
			IAutoSetupImageDisplayDialog.vInspectionEditing = vGlobalCurrentProduct->vFillerMonitorInspection;
		}
	}
	vOldValveDataRowIndex = 0xFFFF; //force it to update the status of the valves
	vOldFillerAverageWeight = 0xFFFF; ////force it to update the average weight
	UpdateDisplay();
}

void CFillerSeamerSamplingDialog::OnSubFunction1Button()
{
	if ((vLocalSystemData->vFillerIndexPulseBitMap) && (vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType]))

	{
		if (vLocalSystemData->vNumberValveRowDataHave > 4)
		{
			ReportErrorMessage("Operator pressed Filler Status Display Button", cUserAction,0);
			vMainWindowPointer->ShowValveMonitorData(0);
		}
		else
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\n\nFiller status not acquired yet";
			TempNoticeDialog.vType = cNoticeMessage;
			TempNoticeDialog.DoModal();
		}
	}
	else
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\n\nSeamer Sampling Input Not Configured.\nContact Inspx.";
		TempNoticeDialog.vType = cNoticeMessage;
		TempNoticeDialog.DoModal();
	}
}


void CFillerSeamerSamplingDialog::OnSubFunction2Button()
{
	if ((vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType]) || (PasswordOK(cSuperInspxPassword,false)))
	{
		if ((vLocalSystemData->vNumberValveRowDataHave > 4) || (PasswordOK(cSuperInspxPassword,false)))
		{
			//Create dialog box
			ReportErrorMessage("Operator pressed Filler Value Monitoring System", cUserAction,0);
			CFillerValveDialog IFillerValveDialog;  
			//Set dialog box data like how pointers to system data
			//IFillerSeamerSamplingDialog.vLocalProductCollection = &vProductCollection;
			IFillerValveDialog.vLocalSystemData = vLocalSystemData;
			IFillerValveDialog.vLocalConfigurationData = vLocalConfigurationData;
			IFillerValveDialog.vMainWindowPointer = vMainWindowPointer;
		//	IFillerValveDialog.vSamplingType = cFillerSamplingType;
	//		//Pass control to dialog box and display
			int nResponse = IFillerValveDialog.DoModal();

			if (nResponse == 10)
			{
				KillTimer(vUpdateDisplayTimerHandle);
				CDialog::EndDialog(10);
			}
		}
		else
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\n\nFiller status not acquired yet";
			TempNoticeDialog.vType = cNoticeMessage;
			TempNoticeDialog.DoModal();
		}
	}
	else
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\n\nFiller Valve Monitor Input Not Configured.\nContact Inspx.";
		TempNoticeDialog.vType = cNoticeMessage;
		TempNoticeDialog.DoModal();
	}
}


void CFillerSeamerSamplingDialog::OnSubFunction3Button()
{
	if (vGlobalShiftKeyDown)
	{
		if (vLocalSystemData->vSamplingPulseLog[cFillerSamplingType].GetLength())
		{
					CHelpDialog IHelpDialog;
					IHelpDialog.vHelpContext = 16;
					IHelpDialog.vHelpType = 0;
					//IHelpDialog.vColor = TempColors; //TempColors is freed by HelpDialog
					IHelpDialog.vMakeRedTextBoldAndUnderLined = true;
					IHelpDialog.vWindowType = cTextWindow;
					//IHelpDialog.vLocalProductCollection = &vProductCollection;
					IHelpDialog.vLocalSystemData = vLocalSystemData;
					IHelpDialog.vMainWindowPointer = vMainWindowPointer;
					IHelpDialog.vLocalConfigurationData = vLocalConfigurationData;
					IHelpDialog.vProductPointer = vGlobalCurrentProduct;
					IHelpDialog.vTextString = vLocalSystemData->vSamplingPulseLog[cFillerSamplingType];
					IHelpDialog.vTitleString = "Filler Sensor History";
					int TempResult = IHelpDialog.DoModal();

					CWnd *TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText1);
					if (TempWindow)
						TempWindow->Invalidate(false);
					UpdateDisplay();
		}
	}
	else
	if ((vLocalSystemData->vFillerIndexPulseBitMap) && (vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType]))
	{
		ReportErrorMessage("Operator pressed Filler Index Error Display Button", cUserAction,0);
		vMainWindowPointer->ShowSamplingIndexErrors(cFillerSamplingType);
	}
	else
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\n\nSeamer Sampling Input Not Configured.\nContact Inspx.";
		TempNoticeDialog.vType = cNoticeMessage;
		TempNoticeDialog.DoModal();
	}
}


void CFillerSeamerSamplingDialog::OnSubFunction4Button()
{
	if (vGlobalShiftKeyDown)
	{
		if (vLocalSystemData->vSamplingPulseLog[cSeamerSamplingType].GetLength())
		{
			CHelpDialog IHelpDialog;
			IHelpDialog.vHelpContext = 16;
			IHelpDialog.vHelpType = 0;
			//IHelpDialog.vColor = TempColors; //TempColors is freed by HelpDialog
			IHelpDialog.vMakeRedTextBoldAndUnderLined = true;
			IHelpDialog.vWindowType = cTextWindow;
			//IHelpDialog.vLocalProductCollection = &vProductCollection;
			IHelpDialog.vLocalSystemData = vLocalSystemData;
			IHelpDialog.vMainWindowPointer = vMainWindowPointer;
			IHelpDialog.vLocalConfigurationData = vLocalConfigurationData;
			IHelpDialog.vProductPointer = vGlobalCurrentProduct;
			IHelpDialog.vTextString = vLocalSystemData->vSamplingPulseLog[cSeamerSamplingType];
			IHelpDialog.vTitleString = "Seamer Sensor History";
			int TempResult = IHelpDialog.DoModal();

			CWnd *TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText1);
			if (TempWindow)
				TempWindow->Invalidate(false);
			UpdateDisplay();
		}
	}
	else
	if ((vLocalSystemData->vSeamerIndexPulseBitMap) && (vLocalConfigurationData->vNumberOfHeadsToMonitor[cSeamerSamplingType]))
	{
		ReportErrorMessage("Operator pressed Seamer Index Error Display Button", cUserAction,0);
		vMainWindowPointer->ShowSamplingIndexErrors(0);
	}
	else
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\n\nSeamer Sampling Input Not Configured.\nContact Inspx.";
		TempNoticeDialog.vType = cNoticeMessage;
		TempNoticeDialog.DoModal();
	}
}


void CFillerSeamerSamplingDialog::OnSubFunction5Button()
{
	if ((vLocalSystemData->vFillerIndexPulseBitMap) && (vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType]))
	{
		if (PasswordOK(cSamplePassword,true))
		{
			//Create dialog box
			ReportErrorMessage("Operator pressed Filler Sampling Menu Button", cUserAction,0);
			CSamplingDialog ISamplingDialog;  
			//Set dialog box data like how pointers to system data
			//IFillerSeamerSamplingDialog.vLocalProductCollection = &vProductCollection;
			ISamplingDialog.vLocalSystemData = vLocalSystemData;
			ISamplingDialog.vLocalConfigurationData = vLocalConfigurationData;
			ISamplingDialog.vMainWindowPointer = vMainWindowPointer;
			ISamplingDialog.vSamplingType = cFillerSamplingType;
			//Pass control to dialog box and display
			int nResponse = ISamplingDialog.DoModal();

			if (nResponse == 10)
			{
				KillTimer(vUpdateDisplayTimerHandle);
				CDialog::EndDialog(10);
			}
		}
	}
	else
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\n\nSeamer Sampling Input Not Configured.\nContact Inspx.";
		TempNoticeDialog.vType = cNoticeMessage;
		TempNoticeDialog.DoModal();
	}
}


void CFillerSeamerSamplingDialog::OnSubFunction6Button()
{
	if (vGlobalShiftKeyDown)
	{
		if (vLocalSystemData->vSamplingPulseLog[cSeamerSamplingType].GetLength())
		{
			CHelpDialog IHelpDialog;
			IHelpDialog.vHelpContext = 16;
			IHelpDialog.vHelpType = 0;
			//IHelpDialog.vColor = TempColors; //TempColors is freed by HelpDialog
			IHelpDialog.vMakeRedTextBoldAndUnderLined = true;
			IHelpDialog.vWindowType = cTextWindow;
			//IHelpDialog.vLocalProductCollection = &vProductCollection;
			IHelpDialog.vLocalSystemData = vLocalSystemData;
			IHelpDialog.vMainWindowPointer = vMainWindowPointer;
			IHelpDialog.vLocalConfigurationData = vLocalConfigurationData;
			IHelpDialog.vProductPointer = vGlobalCurrentProduct;
			IHelpDialog.vTextString = vLocalSystemData->vSamplingPulseLog[0];
			IHelpDialog.vTitleString = "Seamer/ScanTrac Sensor History";
			int TempResult = IHelpDialog.DoModal();

			CWnd *TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText1);
			if (TempWindow)
				TempWindow->Invalidate(false);
			UpdateDisplay();
		}
	}
	else
	if ((vLocalSystemData->vSeamerIndexPulseBitMap) && (vLocalConfigurationData->vNumberOfHeadsToMonitor[cSeamerSamplingType]))
	{
		if (PasswordOK(cSamplePassword,true))
		{
			//Create dialog box
			ReportErrorMessage("Operator pressed Seamer Sampling Menu Button", cUserAction,0);
			CSamplingDialog ISamplingDialog;  
			//Set dialog box data like how pointers to system data
			//IFillerSeamerSamplingDialog.vLocalProductCollection = &vProductCollection;
			ISamplingDialog.vLocalSystemData = vLocalSystemData;
			ISamplingDialog.vLocalConfigurationData = vLocalConfigurationData;
			ISamplingDialog.vMainWindowPointer = vMainWindowPointer;
			ISamplingDialog.vSamplingType = cSeamerSamplingType;
			//Pass control to dialog box and display
			int nResponse = ISamplingDialog.DoModal();

			if (nResponse == 10)
			{
				KillTimer(vUpdateDisplayTimerHandle);
				CDialog::EndDialog(10);
			}
		}
	}
	else
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\n\nSeamer Sampling Input Not Configured.\nContact Inspx.";
		TempNoticeDialog.vType = cNoticeMessage;
		TempNoticeDialog.DoModal();
	}
}


void CFillerSeamerSamplingDialog::OnSubFunction7Button()
{
	if ((vLocalSystemData->vFillerIndexPulseBitMap) && (vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType]))
	{
		if (vLocalSystemData->vNumberValveRowDataHave)
		{
			ReportErrorMessage("Operator pressed Filler Data Display Button", cUserAction,0);
			vMainWindowPointer->ShowValveMonitorData(vLocalSystemData->vNumberValveRowDataHave);
		}
		else
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\n\nNo Filler data collected yet";
			TempNoticeDialog.vType = cNoticeMessage;
			TempNoticeDialog.DoModal();
		}
	}
	else
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\n\nSeamer Sampling Input Not Configured.\nContact Inspx.";
		TempNoticeDialog.vType = cNoticeMessage;
		TempNoticeDialog.DoModal();
	}
}


void CFillerSeamerSamplingDialog::OnSubFunction8Button()
{
	CYesNoDialog TempYesNoDialog;
	TempYesNoDialog.vNoticeText = _T("\nAre you sure you would like to clear the Filler Data?");
	TempYesNoDialog.vQuestionType = cConfirmAbandon;
	TempYesNoDialog.vYesButtonText = "Yes";
	TempYesNoDialog.vNoButtonText = "No";
	int TempResult = TempYesNoDialog.DoModal();
	if (TempResult == IDOK)
	{
		vMainWindowPointer->ResetValveMonitoringData();
		ReportErrorMessage("User pressed Reset Valve Monitoring Data and confirmed" + *vGlobalCurrentProduct->GetProductName(), cUserAction,0);
	}
	else
	{
		ReportErrorMessage("User pressed Reset Valve Monitoring Data was prompted to confirm and then pressed no" , cUserAction,0);
	}
}


HBRUSH CFillerSeamerSamplingDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	HWND TempDialogHwnd = this->m_hWnd;
	int TextSize;
	pDC->SetAttribDC(pDC->m_hDC);

	if (pWnd->GetDlgCtrlID() == IDC_DialogTitleStaticText1)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DialogTitleStaticText1, 8);
		if (vGlobalShiftKeyDown)
			pDC->SetTextColor(cBlack);
		else
		pDC->SetTextColor(vGlobalDialogTitleColor);
	}
	
	if (pWnd->GetDlgCtrlID() == IDC_Background)
	{
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction1Display)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction1Display, 5);
		if (vOldNumberOfValvesThatNeedAttention)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cBlack);
			return vLocalSystemData->vYellowBrush;
		}
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}

	if (pWnd->GetDlgCtrlID() == IDC_ConveyorSpeedError)
	{
		TextSize = SetTextSizeMultilineDisplay(TempDialogHwnd, pWnd, pDC, &m_ConveyorSpeedError, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBlack);
		return vLocalSystemData->vYellowBrush;
	}

	if (pWnd->GetDlgCtrlID() == IDC_SubFunction2Display) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction2Display, 5);
		if (vGlobalCurrentProduct->vFillerMonitorInspection)
		if (vLocalSystemData->vNumberValveRowDataHave > 4)
		if ((vOldFillerAverageWeight < vGlobalCurrentProduct->vFillerMonitorInspection->vDensityThresholdLower) || (vOldFillerAverageWeight > vGlobalCurrentProduct->vFillerMonitorInspection->vDensityThresholdUpper))
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cBlack);
			return vLocalSystemData->vYellowBrush;
		}
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Function3Display)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Function3Display, 5);
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction3Display)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction3Display, 5);
		if (vOldSampleIndexErrorsY[cFillerSamplingType] > 4)
		if (vOldSampleIndexErrorsX[cFillerSamplingType] > vOldSampleIndexErrorsY[cFillerSamplingType] / 2)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cBlack);
			return vLocalSystemData->vYellowBrush;
		}
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_FillerAfterIndexDisplay)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_FillerAfterIndexDisplay, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_FillerBeforeIndexDisplay)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_FillerBeforeIndexDisplay, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SeamerAfterIndexDisplay)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SeamerAfterIndexDisplay, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SeamerBeforeIndexDisplay) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SeamerBeforeIndexDisplay, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_ClockDisplay) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_ClockDisplay, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction4Display)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction4Display, 5);
		pDC->SetBkMode(TRANSPARENT);
		if (vOldSampleIndexErrorsY[cSeamerSamplingType] > 4)
		if (vOldSampleIndexErrorsX[cSeamerSamplingType] > vOldSampleIndexErrorsY[cSeamerSamplingType] / 2)
		{
			pDC->SetTextColor(cBlack);
			return vLocalSystemData->vYellowBrush;
		}
		return vLocalSystemData->vLightGreenBrush;
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
		if (!vLocalSystemData->vFillerIndexPulseBitMap)//make button grey filler sampling not configured
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cGray);
			return vGlobalButtonColorBrush;
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
		if (!vGlobalCurrentProduct->vFillerMonitorInspection)	//make button green if over 1/2 of rotations have index errors
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
		if (vOldNumberOfValvesThatNeedAttention)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cButtonTextColor);
			return vLocalSystemData->vGreenBrush;
		}
		if (!vLocalSystemData->vFillerIndexPulseBitMap)//make button grey filler sampling not configured
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cGray);
			return vGlobalButtonColorBrush;
		}
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction2Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction2Button, 5);  //5 is medium large
		if (!vLocalSystemData->vFillerIndexPulseBitMap)//make button grey filler sampling not configured
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cGray);
			return vGlobalButtonColorBrush;
		}
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction3Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction3Button, 5);  //5 is medium large
		if (vOldSampleIndexErrorsX[cFillerSamplingType] > (vOldSampleIndexErrorsY[cFillerSamplingType] / 2))	//make button green if over 1/2 of rotations have index errors
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cButtonTextColor);
			return vLocalSystemData->vGreenBrush;
		}
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction4Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction4Button, 5);  //5 is medium large
		if (!vLocalSystemData->vFillerIndexPulseBitMap)//make button grey filler sampling not configured
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cGray);
			return vGlobalButtonColorBrush;
		}
		if (vOldSampleIndexErrorsX[cSeamerSamplingType] > (vOldSampleIndexErrorsY[cSeamerSamplingType] / 2))	//make button green if over 1/2 of rotations have index errors
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cButtonTextColor);
			return vLocalSystemData->vGreenBrush;
		}
		if (!vLocalSystemData->vSeamerIndexPulseBitMap)	//make button grey Seamer sampling not configured
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cGray);
			return vGlobalButtonColorBrush;
		}
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction5Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction5Button, 5);  //5 is medium large
		if (!vLocalSystemData->vFillerIndexPulseBitMap)//make button grey filler sampling not configured
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cGray);
			return vGlobalButtonColorBrush;
		}
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction6Button) 
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction6Button, 5);  //5 is medium large
		if (!vLocalSystemData->vSeamerIndexPulseBitMap)	//make button grey Seamer sampling not configured
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cGray);
			return vGlobalButtonColorBrush;
		}
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction7Button) 
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction7Button, 5);  //5 is medium large
		if (!vLocalSystemData->vFillerIndexPulseBitMap)//make button grey filler sampling not configured
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cGray);
			return vGlobalButtonColorBrush;
		}
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


void CFillerSeamerSamplingDialog::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == vUpdateDisplayTimerHandle)
	{
		KillTimer(vUpdateDisplayTimerHandle);
		OneSecondDisplayUpdate();
		int TempTimerResult = SetTimer(vUpdateDisplayTimerHandle,1000,NULL);
		if (!TempTimerResult)
			ReportErrorMessage("Error-Display Timer Failed",cEMailInspx,32000);
	}

	CDialogEx::OnTimer(nIDEvent);
}


BOOL CFillerSeamerSamplingDialog::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	CWnd * TempWindow = NULL;
	if (pMsg->message == cUpdateTitleBarMessage)
	{
		CWnd *TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText1);
		if (TempWindow)
			TempWindow->Invalidate(false);
		UpdateDisplay();
		return true;  //true indicates it has been handled, so do not process
	}
	else
	if (pMsg->message == WM_KEYDOWN)
	{
		if (vGlobalShiftKeyDown)
		{
			PostThreadMessage(vGlobalCallingThreadID,cHiddenKeyPressed, 0, pMsg->wParam);
			UpdateDisplay();
		}
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
			case 36:  //Home key, Screen Capture
				vGlobalShiftKeyDown = true;
				PostThreadMessage(vGlobalCallingThreadID,cHiddenKeyPressed, 0, 0x61);
				UpdateDisplay();
			break;
			case cPageDown: // Help
				PostThreadMessage(vGlobalCallingThreadID, cShowHelp, 0, 0);
				{
					TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText);
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
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CFillerSeamerSamplingDialog::SetChangeMade() 
{
	if (!vChangeMade)
	{
		m_Function2Button.ShowWindow(SW_SHOW);
		m_Function2Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		m_Function4Button.ShowWindow(SW_SHOW);
		CString TempText;
		TempText.LoadString(IDS_SaveAndExit);
		SetDlgItemText(IDC_Function2Button,TempText);
		TempText.LoadString(IDS_Cancel);
		SetDlgItemText(IDC_Function4Button,TempText);
		vChangeMade = true;
	}
}

void CFillerSeamerSamplingDialog::OneSecondDisplayUpdate()
{
	if (vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType])
	if (vOldMostRecentTimeAfterIndexPulse[cFillerSamplingType] != vLocalSystemData->vMostRecentTimeAfterIndexPulse[cFillerSamplingType])
	{
		vOldMostRecentTimeAfterIndexPulse[cFillerSamplingType] = vLocalSystemData->vMostRecentTimeAfterIndexPulse[cFillerSamplingType];
		BYTE TempDigits = 6;
		if (vOldMostRecentTimeAfterIndexPulse[cFillerSamplingType] > .01)
			TempDigits = 3;
		if (vOldMostRecentTimeAfterIndexPulse[cFillerSamplingType] > 9000000)
			TempDigits = 0;
		SetDlgItemText(IDC_FillerAfterIndexDisplay, dtoa(vOldMostRecentTimeAfterIndexPulse[cFillerSamplingType] / 1000000, TempDigits) + " after index");
	}

	if (vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType])
	if (vOldMostRecentTimeBeforeIndexPulse[cFillerSamplingType] != vLocalSystemData->vMostRecentTimeBeforeIndexPulse[cFillerSamplingType])
	{
		vOldMostRecentTimeBeforeIndexPulse[cFillerSamplingType] = vLocalSystemData->vMostRecentTimeBeforeIndexPulse[cFillerSamplingType];
		BYTE TempDigits = 6;
		if (vOldMostRecentTimeBeforeIndexPulse[cFillerSamplingType] > .01)
			TempDigits = 3;
		if (vOldMostRecentTimeBeforeIndexPulse[cFillerSamplingType] > 9000000)
			TempDigits = 0;
		SetDlgItemText(IDC_FillerBeforeIndexDisplay, dtoa(vOldMostRecentTimeBeforeIndexPulse[cFillerSamplingType] / 1000000, TempDigits) + " before index");
	}

	if (vLocalConfigurationData->vNumberOfHeadsToMonitor[cSeamerSamplingType])
	if (vOldMostRecentTimeAfterIndexPulse[cSeamerSamplingType] != vLocalSystemData->vMostRecentTimeAfterIndexPulse[cSeamerSamplingType])
	{
		vOldMostRecentTimeAfterIndexPulse[cSeamerSamplingType] = vLocalSystemData->vMostRecentTimeAfterIndexPulse[cSeamerSamplingType];
		BYTE TempDigits = 6;
		if (vOldMostRecentTimeAfterIndexPulse[cSeamerSamplingType] > .01)
			TempDigits = 3;
		if (vOldMostRecentTimeAfterIndexPulse[cSeamerSamplingType] > 9000000)
			TempDigits = 0;
		SetDlgItemText(IDC_SeamerAfterIndexDisplay, dtoa(vOldMostRecentTimeAfterIndexPulse[cSeamerSamplingType] / 1000000, TempDigits) + " after index");
	}

	if (vLocalConfigurationData->vNumberOfHeadsToMonitor[cSeamerSamplingType])
	if (vOldMostRecentTimeBeforeIndexPulse[cSeamerSamplingType] != vLocalSystemData->vMostRecentTimeBeforeIndexPulse[cSeamerSamplingType])
	{
		vOldMostRecentTimeBeforeIndexPulse[cSeamerSamplingType] = vLocalSystemData->vMostRecentTimeBeforeIndexPulse[cSeamerSamplingType];
		BYTE TempDigits = 6;
		if (vOldMostRecentTimeBeforeIndexPulse[cSeamerSamplingType] > .01)
			TempDigits = 3;
		if (vOldMostRecentTimeBeforeIndexPulse[cSeamerSamplingType] > 9000000)
			TempDigits = 0;
		SetDlgItemText(IDC_SeamerBeforeIndexDisplay, dtoa(vOldMostRecentTimeBeforeIndexPulse[cSeamerSamplingType] / 1000000, TempDigits) + " before index");
	}

	for (BYTE TempLoop = cFillerSamplingType; TempLoop <= cSeamerSamplingType; TempLoop++)
	if (vLocalConfigurationData->vNumberOfHeadsToMonitor[TempLoop])
	if (vOldNextHeadToProcess[TempLoop] != vLocalSystemData->vSamplingIndexErrorDataIndex[TempLoop]) //if index changed, data changed, so calculate index errors
	{
		vOldNextHeadToProcess[TempLoop] = vLocalSystemData->vSamplingIndexErrorDataIndex[TempLoop];

		vLocalSystemData->vSampleIndexErrorsX[TempLoop] = 0;
		vLocalSystemData->vSampleIndexErrorsY[TempLoop] = vLocalSystemData->vSamplingIndexErrorDataHave[TempLoop];

		int *TempDataPointer = vLocalSystemData->vSamplingIndexErrorData[TempLoop];
		for (WORD TempDataLoop = 0; TempDataLoop < vLocalSystemData->vSamplingIndexErrorDataHave[TempLoop]; TempDataLoop++)
		{
			if (*TempDataPointer != 0)
				vLocalSystemData->vSampleIndexErrorsX[TempLoop]++;

			TempDataPointer++;
		}
	}

	if (vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType])
	if (vOldValveDataRowIndex != vLocalSystemData->vValveDataRowIndex) //if rotation count changed, check valve status
	{
		if (((vOldValveDataRowIndex == 0) || (vOldValveDataRowIndex == 0xFFFF)) && (vLocalSystemData->vValveDataRowIndex))
		{
			m_SubFunction8Button.ShowWindow(SW_SHOW);
		}
		else
		if ((vLocalSystemData->vValveDataRowIndex == 0) || ((vOldValveDataRowIndex == 0xFFFF) && (vLocalSystemData->vValveDataRowIndex)))
		{
			m_SubFunction8Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_SubFunction8Button.GetRectToInvalidateArrow());
		}

		vOldValveDataRowIndex = vLocalSystemData->vValveDataRowIndex;
		vMainWindowPointer->CalculateValveStatus();

		if ((vLocalSystemData->vNumberOfValvesThatNeedAttention != vOldNumberOfValvesThatNeedAttention) ||
			((vLocalSystemData->vNumberValveRowDataHave > 4) && (vOldNumberValveRowDataHave <= 4)) ||
			((vLocalSystemData->vNumberValveRowDataHave < 4) && (vOldNumberValveRowDataHave > 4)))
		{
			vOldNumberValveRowDataHave = vLocalSystemData->vNumberValveRowDataHave;
			vOldNumberOfValvesThatNeedAttention = vLocalSystemData->vNumberOfValvesThatNeedAttention;
			if (vLocalSystemData->vNumberValveRowDataHave <= 4)
				SetDlgItemText(IDC_SubFunction1Display, _T(" Filler status not acquired"));
			else
			if (vOldNumberOfValvesThatNeedAttention == 1)
				SetDlgItemText(IDC_SubFunction1Display, _T(" 1 Valve needs attention"));
			else
			if (vOldNumberOfValvesThatNeedAttention)
			{
				CString TempText = " ";
				TempText = dtoa(vOldNumberOfValvesThatNeedAttention, 0);
				TempText = " " + TempText  + " valves need attention";
				SetDlgItemText(IDC_SubFunction1Display, TempText);
			}
			else
				SetDlgItemText(IDC_SubFunction1Display, _T(" All valves OK"));

			CWnd * TempWindow = NULL;
			TempWindow = CWnd::GetDlgItem(IDC_SubFunction1Button);
			if (TempWindow)
				TempWindow->Invalidate(false);
		}
	}

	if (vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType])
	if (dtoa(vOldFillerAverageWeight, 2) != dtoa(vLocalSystemData->vFillerAverageWeight, 2))
	{
		vOldFillerAverageWeight = vLocalSystemData->vFillerAverageWeight;
		if (vLocalSystemData->vNumberValveRowDataHave <= 4)
			SetDlgItemText(IDC_SubFunction2Display, _T(" Average Fill not acquired"));
		else
			SetDlgItemText(IDC_SubFunction2Display, " Average Fill: " + dtoa(vOldFillerAverageWeight, 2));
	}

	if (vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType])
	if ((vLocalSystemData->vSampleIndexErrorsX[cFillerSamplingType] != vOldSampleIndexErrorsX[cFillerSamplingType]) || 
		(vLocalSystemData->vSampleIndexErrorsY[cFillerSamplingType] != vOldSampleIndexErrorsY[cFillerSamplingType]))
	{
		vOldSampleIndexErrorsX[cFillerSamplingType] = vLocalSystemData->vSampleIndexErrorsX[cFillerSamplingType];
		vOldSampleIndexErrorsY[cFillerSamplingType] = vLocalSystemData->vSampleIndexErrorsY[cFillerSamplingType];

		CString TempPluralString = "";
		if (vOldSampleIndexErrorsX[cFillerSamplingType] != 1)
			TempPluralString = "s";

		if (vOldSampleIndexErrorsY[cFillerSamplingType] == 0)
			SetDlgItemText(IDC_SubFunction3Display, _T(" No Filler rotations"));
		else
		if (vOldSampleIndexErrorsY[cFillerSamplingType] == 1)
			SetDlgItemText(IDC_SubFunction3Display, " " + dtoa(vOldSampleIndexErrorsX[cFillerSamplingType], 0) + " error" + TempPluralString + " in last rotation");
		else
		if (vOldSampleIndexErrorsX[cFillerSamplingType])
			SetDlgItemText(IDC_SubFunction3Display, " " + dtoa(vOldSampleIndexErrorsX[cFillerSamplingType], 0) + " error" + TempPluralString + " in last " + dtoa(vOldSampleIndexErrorsY[cFillerSamplingType], 0) + " rotations");
		else
			SetDlgItemText(IDC_SubFunction3Display, " No errors in last " + dtoa(vOldSampleIndexErrorsY[cFillerSamplingType], 0) + " rotations");
	}

	if (vLocalConfigurationData->vNumberOfHeadsToMonitor[cSeamerSamplingType])
	if ((vLocalSystemData->vSampleIndexErrorsX[cSeamerSamplingType] != vOldSampleIndexErrorsX[cSeamerSamplingType]) || 
		(vLocalSystemData->vSampleIndexErrorsY[cSeamerSamplingType] != vOldSampleIndexErrorsY[cSeamerSamplingType]))
	{
		vOldSampleIndexErrorsX[cSeamerSamplingType] = vLocalSystemData->vSampleIndexErrorsX[cSeamerSamplingType];
		vOldSampleIndexErrorsY[cSeamerSamplingType] = vLocalSystemData->vSampleIndexErrorsY[cSeamerSamplingType];

		CString TempPluralString = "";
		if (vOldSampleIndexErrorsX[cSeamerSamplingType] != 1)
			TempPluralString = "s";

		if (vOldSampleIndexErrorsY[cSeamerSamplingType] == 0)
			SetDlgItemText(IDC_SubFunction4Display, _T(" No Seamer rotations"));
		else
		if (vOldSampleIndexErrorsY[cSeamerSamplingType] == 1)
			SetDlgItemText(IDC_SubFunction4Display, " " + dtoa(vOldSampleIndexErrorsX[cSeamerSamplingType], 0) + " error" + TempPluralString + " in last rotation");
		else
		if (vOldSampleIndexErrorsX[cSeamerSamplingType])
			SetDlgItemText(IDC_SubFunction4Display, " " + dtoa(vOldSampleIndexErrorsX[cSeamerSamplingType], 0) + " error" + TempPluralString + " in last " + dtoa(vOldSampleIndexErrorsY[cSeamerSamplingType], 0) + " rotations");
		else
			SetDlgItemText(IDC_SubFunction4Display, " No errors in last " + dtoa(vOldSampleIndexErrorsY[cSeamerSamplingType], 0) + " rotations");
	}

	CTime TempTime;
	TempTime = CTime::GetCurrentTime();
	CString TempTimeString = " ";
	if (vLocalConfigurationData->vShow24HourTime)
		TempTimeString = TempTime.Format("%H:%M:%S");//24 hour time format
	else
		TempTimeString = TempTime.Format("%#I:%M:%S%p");//12 hour time format with am pm
	SetDlgItemText(IDC_ClockDisplay, TempTimeString);
	
		//if right conveyor speed hide error status box else wrong conveyor speed show message
	if (!vLocalSystemData->vUseSamplingFIFO)
	if (vGlobaluCSerialPort->GetIndexForThisEncoderRate(vLocalSystemData->vActualEncoderRate) < cNumberOfFillerOffsetSpeeds) //if not running one of configured encoder rates, indicate an error
	//if ((((double)(100 + vLocalConfigurationData->vEncoderRateTolerance) / 100.0) * (double)vGlobalCurrentProduct->vEncoderRateIndexesForFillerSeamerSampling[0] > vLocalSystemData->vActualEncoderRate)
	//		&& (((double)(100 - vLocalConfigurationData->vEncoderRateTolerance) / 100.0) * (double)vGlobalCurrentProduct->vEncoderRateIndexesForFillerSeamerSampling[0] < vLocalSystemData->vActualEncoderRate)
	//	|| (((double)(100 + vLocalConfigurationData->vEncoderRateTolerance) / 100.0) * (double)vGlobalCurrentProduct->vEncoderRateIndexesForFillerSeamerSampling[1] > vLocalSystemData->vActualEncoderRate)
	//		&& (((double)(100 - vLocalConfigurationData->vEncoderRateTolerance) / 100.0) * (double)vGlobalCurrentProduct->vEncoderRateIndexesForFillerSeamerSampling[1] < vLocalSystemData->vActualEncoderRate)
	//	|| (((double)(100 + vLocalConfigurationData->vEncoderRateTolerance) / 100.0) * (double)vGlobalCurrentProduct->vEncoderRateIndexesForFillerSeamerSampling[2] > vLocalSystemData->vActualEncoderRate)
	//		&& (((double)(100 - vLocalConfigurationData->vEncoderRateTolerance) / 100.0) * (double)vGlobalCurrentProduct->vEncoderRateIndexesForFillerSeamerSampling[2] < vLocalSystemData->vActualEncoderRate)
	//	|| (((double)(100 + vLocalConfigurationData->vEncoderRateTolerance) / 100.0) * (double)vGlobalCurrentProduct->vEncoderRateIndexesForFillerSeamerSampling[3] > vLocalSystemData->vActualEncoderRate)
	//		&& (((double)(100 - vLocalConfigurationData->vEncoderRateTolerance) / 100.0) * (double)vGlobalCurrentProduct->vEncoderRateIndexesForFillerSeamerSampling[3] < vLocalSystemData->vActualEncoderRate))
	{
		m_ConveyorSpeedError.ShowWindow(SW_HIDE);
	}
	else if (vLocalSystemData->vActualEncoderRate == 0)
	{
	//	SetDlgItemText(IDC_ConveyorSpeedError, "\nConveyor is not running, turn on to get filler data");
		m_ConveyorSpeedError.ShowWindow(SW_HIDE);
	}
	else
	{
		SetDlgItemText(IDC_ConveyorSpeedError, "Conveyor Speed out of Range; Encoder Rate is " + dtoa(vLocalSystemData->vActualEncoderRate) + "\nAcceptable Rates are "
			+ dtoa( ((100 + ((double)vLocalConfigurationData->vEncoderRateTolerance))/100) * ((double) vGlobalCurrentProduct->vEncoderRateIndexesForFillerSeamerSampling[0]), 0 ) + " - "
			+ dtoa( ((100 - ((double)vLocalConfigurationData->vEncoderRateTolerance))/100) * ((double) vGlobalCurrentProduct->vEncoderRateIndexesForFillerSeamerSampling[0]), 0 ) + "Hz, " 
			+ dtoa( ((100 + ((double)vLocalConfigurationData->vEncoderRateTolerance))/100) * ((double) vGlobalCurrentProduct->vEncoderRateIndexesForFillerSeamerSampling[1]), 0 ) + " - "
			+ dtoa( ((100 - ((double)vLocalConfigurationData->vEncoderRateTolerance))/100) * ((double) vGlobalCurrentProduct->vEncoderRateIndexesForFillerSeamerSampling[1]), 0 ) + "Hz, " 
			+ dtoa( ((100 + ((double)vLocalConfigurationData->vEncoderRateTolerance))/100) * ((double) vGlobalCurrentProduct->vEncoderRateIndexesForFillerSeamerSampling[2]), 0 ) + " - "
			+ dtoa( ((100 - ((double)vLocalConfigurationData->vEncoderRateTolerance))/100) * ((double) vGlobalCurrentProduct->vEncoderRateIndexesForFillerSeamerSampling[2]), 0 ) + "Hz, and " 
			+ dtoa( ((100 + ((double)vLocalConfigurationData->vEncoderRateTolerance))/100) * ((double) vGlobalCurrentProduct->vEncoderRateIndexesForFillerSeamerSampling[3]), 0 ) + " - "
			+ dtoa( ((100 - ((double)vLocalConfigurationData->vEncoderRateTolerance))/100) * ((double) vGlobalCurrentProduct->vEncoderRateIndexesForFillerSeamerSampling[3]), 0 ) + "Hz." );
		m_ConveyorSpeedError.ShowWindow(SW_SHOW);
	}
}

void CFillerSeamerSamplingDialog::UpdateDisplay()
{
	if (vGlobalShiftKeyDown)
		SetDlgItemText(IDC_SubFunction3Button, _T("Filler\nSensor\nLog"));
	else
		SetDlgItemText(IDC_SubFunction3Button, _T("Filler\nIndex\nErrors"));

	if (vGlobalShiftKeyDown)
		SetDlgItemText(IDC_SubFunction4Button, _T("Seamer\nSensor\nLog"));
	else
		SetDlgItemText(IDC_SubFunction4Button, _T("Seamer\nIndex\nErrors"));

	if (vGlobalShiftKeyDown)
		SetDlgItemText(IDC_SubFunction6Button, _T("Seamer/ScanTrac\nSensor\nLog"));
	else
		SetDlgItemText(IDC_SubFunction6Button, _T("Seamer\nSampling"));

	if (vGlobalShiftKeyDown)
		SetDlgItemText(IDC_SubFunction7Button, _T("Filler\nData\n(All)"));
	else
	if (PasswordOK(cTemporaryInspxPassword,false))
		SetDlgItemText(IDC_SubFunction7Button, _T("Filler\nData\n(Last 21)"));
	else
		SetDlgItemText(IDC_SubFunction7Button, _T("Filler\nData"));

	SetDlgItemText(IDC_Function3Display, dtoa(vGlobalCurrentProduct->vFillerTolerance, 2));

	if ((vGlobalCurrentProduct->vFillerMonitorInspection) && (vGlobalShiftKeyDown) && (PasswordOK(cSuperInspxPassword,false)))
		SetDlgItemText(IDC_Function5Button, _T("Delete Fill Inspection"));
	else
	if (vGlobalCurrentProduct->vFillerMonitorInspection)
		SetDlgItemText(IDC_Function5Button, _T("Fill Monitor Settings"));
	else
		SetDlgItemText(IDC_Function5Button, _T("Create Fill Inspection"));
}

void CFillerSeamerSamplingDialog::OnDialogtitlestatictext1()
{
	BYTE TempHiddenKeys = 8;
	if (!PasswordOK(cTemporaryInspxPassword,false))
		TempHiddenKeys = 10;
	PostThreadMessage(vGlobalCallingThreadID, cDisplaySoftKeyMenu, TempHiddenKeys, TempHiddenKeys);
}
