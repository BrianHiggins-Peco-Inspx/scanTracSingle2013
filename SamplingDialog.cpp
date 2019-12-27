// SamplingDialog.cpp : implementation file
//

#include "stdafx.h"
#include "ScanTrac.h"
#include "SamplingDialog.h"
#include "afxdialogex.h"
#include "NumericEntryDialog.h"
#include "YesNoDialog.h"

// CSamplingDialog dialog

IMPLEMENT_DYNAMIC(CSamplingDialog, CDialogEx)

extern BYTE vGlobalShiftKeyDown;
extern CProduct *vGlobalCurrentProduct;
extern unsigned long vGlobalDialogTitleColor;

CSamplingDialog::CSamplingDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSamplingDialog::IDD, pParent)
{
	vMainWindowPointer = NULL;
	//vLocalProductCollection = NULL;
	vLocalSystemData = NULL;
	vLocalConfigurationData = NULL;
	vSamplingType = cRandomSamplingType;
	vSamplesToTake = 0;
	vOldSamplesToTake = -1;
	vValve_HeadTextString = "Valve";

	vChangeMade = false;
	vOldValveSamplesEjectedCount = 0;
	vOldHeadSamplesEjectedCount = 0;
	vUpdateDisplayTimerHandle = 29982;
	for (BYTE TempLoop = 0; TempLoop < 3; TempLoop++)
	{
		vOldSamplesEjectedCount[TempLoop] = 0xFFFF;
		vOldNextHeadToProcess[TempLoop] = 0xFFFF;
		vOldSamplingIndexErrorDataHave[TempLoop] = 0xFFFF;
		vOldLastIndexPulseWasCorrect[TempLoop] = false;
	}

	vLocalCWndCollection.Add(this);
	vLocalCWndCollection.Add(&m_Background);
	vLocalCWndCollection.Add(&m_Function1Button);
	vLocalCWndCollection.Add(&m_Function2Button);
	vLocalCWndCollection.Add(&m_Function3Button);
	vLocalCWndCollection.Add(&m_Function4Button);
	vLocalCWndCollection.Add(&m_Function5Button);
	vLocalCWndCollection.Add(&m_SubFunction1Button);
	vLocalCWndCollection.Add(&m_SubFunction2Button);
	vLocalCWndCollection.Add(&m_SubFunction3Button);
	vLocalCWndCollection.Add(&m_SubFunction4Button);
	vLocalCWndCollection.Add(&m_SubFunction5Button);
	vLocalCWndCollection.Add(&m_SubFunction6Button);
	vLocalCWndCollection.Add(&m_SubFunction7Button);
	vLocalCWndCollection.Add(&m_SubFunction8Button);
	vLocalCWndCollection.Add(&m_SubFunction1Display);
	vLocalCWndCollection.Add(&m_SubFunction3Display);
	vLocalCWndCollection.Add(&m_SubFunction5Display);
	vLocalCWndCollection.Add(&m_SubFunction6Display);
	vLocalCWndCollection.Add(&m_NumberOfRotationsDisplay);
	vLocalCWndCollection.Add(&m_LastIndexPulseDisplay);
	vLocalCWndCollection.Add(&m_SeamerStatusDisplay);
	vLocalCWndCollection.Add(&m_LastHeadProcessedDisplay);
	vLocalCWndCollection.Add(&m_FillerStatusDisplay2);
	vLocalCWndCollection.Add(&m_ClockDisplay2);
	vLocalCWndCollection.Add(&m_DialogStaticText1);
	vLocalCWndCollection.Add(&m_FillerStatusDisplay);
	vLocalCWndCollection.Add(&m_SamplesToTakeDisplay);
}

void CSamplingDialog::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (vSamplingType == cSeamerSamplingType)
		vValve_HeadTextString = "Head";

	vNumberOfEjectors = 0;
	for (BYTE TempLoop = 0; TempLoop < cNumberOfEjectors; TempLoop++)
	if (vLocalConfigurationData->vEjector[TempLoop].vEnabled)
		vNumberOfEjectors++;

	if (vNumberOfEjectors < 2)
	{
		m_SubFunction5Button.ShowWindow(SW_HIDE);
		m_SubFunction5Display.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction5Button.GetRectToInvalidateArrow());
	}

	m_Background.SetWindowPos(NULL,cMainBackgroundLeft, cMainBackgroundTop, cMainBackgroundLength, cMainBackgroundHeight, SWP_NOZORDER);
	if (vSamplingType > cSeamerSamplingType)
		vSamplingType = cSeamerSamplingType;
	//if (vSamplingType < cFillerSamplingType)
	//	vSamplingType = cFillerSamplingType;

	SetDlgItemText(IDC_DialogTitleStaticText1, vLocalSystemData->vSamplingName[vSamplingType] + " Sampling");
	CString TempText = vLocalSystemData->vSamplingName[vSamplingType] +" has " + dtoa(vLocalConfigurationData->vNumberOfHeadsToMonitor[vSamplingType], 0);
	SetDlgItemText(IDC_FillerStatusDisplay2, TempText + " " + vValve_HeadTextString + "s");

	if (vGlobalCurrentProduct->vSamplingEjector[vSamplingType] == 0)
		vGlobalCurrentProduct->vSamplingEjector[vSamplingType] = 1;

	if (!(vLocalConfigurationData->vShowSampleAllButton & vSamplingType)) //if showing this button
	{
		m_SubFunction2Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction2Button.GetRectToInvalidateArrow());
	}

	if (vLocalSystemData->vSamplingIndexErrorDataHave[vSamplingType])  //if there are revs, then have data, so make old different so shows on display
		vOldLastIndexPulseWasCorrect[vSamplingType] = !vLocalSystemData->vLastIndexPulseWasCorrect[vSamplingType];;

	if (vSamplingType == cRandomSamplingType)
	{
		SetDlgItemText(IDC_DialogTitleStaticText1, _T("Sample Containers"));
		m_SubFunction1Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction1Button.GetRectToInvalidateArrow());
		m_SubFunction2Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction2Button.GetRectToInvalidateArrow());

		m_SubFunction1Display.ShowWindow(SW_HIDE);
		m_LastHeadProcessedDisplay.ShowWindow(SW_HIDE);
		m_NumberOfRotationsDisplay.ShowWindow(SW_HIDE);
		m_LastIndexPulseDisplay.ShowWindow(SW_HIDE);
		m_SeamerStatusDisplay.ShowWindow(SW_HIDE);
		m_FillerStatusDisplay2.ShowWindow(SW_HIDE);

		m_SubFunction3Display.SetWindowPos(NULL,130,252,160,20,SWP_NOZORDER);
		m_SubFunction5Display.SetWindowPos(NULL,260,78,250,20,SWP_NOZORDER);
	}

	UpdateDisplay();
	OneSecondDisplayUpdate();
	this->SetWindowText(_T("Sampling"));

	int TempTimerResult = SetTimer(vUpdateDisplayTimerHandle,1000,NULL);
	if (!TempTimerResult)
		ReportErrorMessage("Error-Display Timer SD Failed",cEMailInspx,32000);

	::SetupMenu(vLocalCWndCollection);
}

CSamplingDialog::~CSamplingDialog()
{
}

void CSamplingDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_Background, m_Background);
	DDX_Control(pDX, IDC_SubFunction1Button, m_SubFunction1Button);
	DDX_Control(pDX, IDC_SubFunction2Button, m_SubFunction2Button);
	DDX_Control(pDX, IDC_SubFunction3Button, m_SubFunction3Button);
	DDX_Control(pDX, IDC_SubFunction4Button, m_SubFunction4Button);
	DDX_Control(pDX, IDC_SubFunction5Button, m_SubFunction5Button);
	DDX_Control(pDX, IDC_SubFunction6Button, m_SubFunction6Button);
	DDX_Control(pDX, IDC_SubFunction7Button, m_SubFunction7Button);
	DDX_Control(pDX, IDC_SubFunction8Button, m_SubFunction8Button);
	DDX_Control(pDX, IDC_Function1Button, m_Function1Button);
	DDX_Control(pDX, IDC_Function2Button, m_Function2Button);
	DDX_Control(pDX, IDC_Function3Button, m_Function3Button);
	DDX_Control(pDX, IDC_Function4Button, m_Function4Button);
	DDX_Control(pDX, IDC_Function5Button, m_Function5Button);
	DDX_Control(pDX, IDC_SubFunction1Display, m_SubFunction1Display);
	DDX_Control(pDX, IDC_SubFunction3Display, m_SubFunction3Display);
	DDX_Control(pDX, IDC_SubFunction5Display, m_SubFunction5Display);
	DDX_Control(pDX, IDC_SubFunction6Display, m_SubFunction6Display);
	//  DDX_Control(pDX, IDC_LastHeadProcessedDisplay, m_LastHeadprocessedDisplay);
	DDX_Control(pDX, IDC_NumberOfRotationsDisplay, m_NumberOfRotationsDisplay);
	DDX_Control(pDX, IDC_LastIndexPulseDisplay, m_LastIndexPulseDisplay);
	DDX_Control(pDX, IDC_SeamerStatusDisplay, m_SeamerStatusDisplay);
	DDX_Control(pDX, IDC_LastHeadProcessedDisplay, m_LastHeadProcessedDisplay);
	DDX_Control(pDX, IDC_FillerStatusDisplay2, m_FillerStatusDisplay2);
	DDX_Control(pDX, IDC_ClockDisplay2, m_ClockDisplay2);
	DDX_Control(pDX, IDC_DialogTitleStaticText1, m_DialogStaticText1);
	DDX_Control(pDX, IDC_FillerStatusDisplay, m_FillerStatusDisplay);
	DDX_Control(pDX, IDC_SamplesToTakeDisplay, m_SamplesToTakeDisplay);
	
}


BEGIN_MESSAGE_MAP(CSamplingDialog, CDialogEx)
	ON_STN_CLICKED(IDC_Function1Button, &CSamplingDialog::OnFunction1Button)
	ON_STN_CLICKED(IDC_Function2Button, &CSamplingDialog::OnFunction2Button)
	ON_STN_CLICKED(IDC_Function3Button, &CSamplingDialog::OnFunction3Button)
	ON_STN_CLICKED(IDC_Function4Button, &CSamplingDialog::OnFunction4Button)
	ON_STN_CLICKED(IDC_Function5Button, &CSamplingDialog::OnFunction5Button)
	ON_STN_CLICKED(IDC_SubFunction1Button, &CSamplingDialog::OnSubFunction1Button)
	ON_STN_CLICKED(IDC_SubFunction2Button, &CSamplingDialog::OnSubFunction2Button)
	ON_STN_CLICKED(IDC_SubFunction3Button, &CSamplingDialog::OnSubFunction3Button)
	ON_STN_CLICKED(IDC_SubFunction4Button, &CSamplingDialog::OnSubFunction4Button)
	ON_STN_CLICKED(IDC_SubFunction5Button, &CSamplingDialog::OnSubFunction5Button)
	ON_STN_CLICKED(IDC_SubFunction6Button, &CSamplingDialog::OnSubFunction6Button)
	ON_STN_CLICKED(IDC_SubFunction7Button, &CSamplingDialog::OnSubFunction7Button)
	ON_STN_CLICKED(IDC_SubFunction8Button, &CSamplingDialog::OnSubFunction8Button)
	ON_WM_CTLCOLOR()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_STN_CLICKED(IDC_DialogTitleStaticText1, &CSamplingDialog::OnDialogtitlestatictext1)
END_MESSAGE_MAP()


// CSamplingDialog message handlers


void CSamplingDialog::OnFunction1Button()
{
	//Main Menu was pressed
	if ((!vChangeMade) || (AbortSetupChanges()))
	{
		KillTimer(vUpdateDisplayTimerHandle);
		CDialog::EndDialog(10);
	}
}


void CSamplingDialog::OnFunction2Button()
{
	CDialog::EndDialog(1);
}


void CSamplingDialog::OnFunction3Button()
{
}


void CSamplingDialog::OnFunction4Button()
{
	// TODO: Add your control notification handler code here
}


void CSamplingDialog::OnFunction5Button()
{
}


void CSamplingDialog::OnSubFunction1Button()
{ //sample a number of containers from same head
	if (vSamplingType != cRandomSamplingType)
	if (vSamplesToTake > 0) //cancel taking current samples
	{
		for (BYTE TempLoop = 0; TempLoop < 3; TempLoop++)
		{
			vLocalSystemData->vSamplingNowNumberOfSamplesToTakeFromSeries[TempLoop] = 0;
			vLocalSystemData->vSamplingNowNumberOfSamplesToTakeFromSameHead[TempLoop] = 0;

			if (vLocalSystemData->vNeedToDisableEjectorsAfterSampleAll) 
			{
				vMainWindowPointer->EnableDisableEjectors(false);
				vLocalSystemData->vNeedToDisableEjectorsAfterSampleAll = false;
				ReportErrorMessage("Disable Ejectors after Sampling containers and aborted", cWriteToLog,0);
			}

			ReportErrorMessage("Operator Aborted Sampling", cUserAction,0);
			UpdateDisplay();
		}
	}
	else
	if (vLocalSystemData->vSystemRunMode != cRunningSystemMode)
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\nScanTrac must be inspecting to sample";
		TempNoticeDialog.vType = cInformationMessage;
		TempNoticeDialog.DoModal();
	}
	else
	if (vLocalConfigurationData->vNumberOfHeadsToMonitor[vSamplingType])
	{
		CNumericEntryDialog INumericEntryDialog;  
				
		//Set dialog box data titles and number value
		INumericEntryDialog.vEditString = dtoa(vLocalSystemData->vSamplingParameterSingleHead[vSamplingType], 0);

		INumericEntryDialog.m_DialogTitleStaticText1 = "Enter " + vValve_HeadTextString + " to take samples from";
		INumericEntryDialog.m_DialogTitleStaticText2 = "";
		CString TempText = " ";
		TempText.LoadString(IDS_OriginalValue);
		INumericEntryDialog.m_DialogTitleStaticText4 = TempText + ": " + INumericEntryDialog.vEditString;

		INumericEntryDialog.m_UnitsString = "Maximum: " + dtoa(vLocalConfigurationData->vNumberOfHeadsToMonitor[vSamplingType], 0);
		INumericEntryDialog.vIntegerOnly = true;
		INumericEntryDialog.vAllowZero = false;
		INumericEntryDialog.vMaxValue = vLocalConfigurationData->vNumberOfHeadsToMonitor[vSamplingType];
		INumericEntryDialog.vMinValue = 1;
		//Pass control to dialog box and display
		int nResponse = INumericEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			vLocalSystemData->vSamplingParameterSingleHead[vSamplingType] = _wtoi(INumericEntryDialog.vEditString);
			//SetChangeMade();
			UpdateDisplay();
				
			//Set dialog box data titles and number value
			INumericEntryDialog.vEditString = dtoa(vLocalSystemData->vSamplingParameterSingleHeadQuantity[vSamplingType], 0);

			INumericEntryDialog.m_DialogTitleStaticText1 = "Enter Number of Samples to eject";
			INumericEntryDialog.m_DialogTitleStaticText2 = "";
			CString TempText = " ";
			TempText.LoadString(IDS_OriginalValue);
			INumericEntryDialog.m_DialogTitleStaticText4 = TempText + ": " + INumericEntryDialog.vEditString;

			INumericEntryDialog.m_UnitsString = "";
			INumericEntryDialog.vIntegerOnly = true;
			INumericEntryDialog.vMaxValue = 100;
			INumericEntryDialog.vMinValue = 1;
			//Pass control to dialog box and display
			int nResponse = INumericEntryDialog.DoModal();
			//dialog box is now closed, if user pressed select do this
			//if user pressed cancel, do nothing
			if (nResponse == IDOK)
			{
				vLocalSystemData->vSamplingParameterSingleHeadQuantity[vSamplingType] = _wtoi(INumericEntryDialog.vEditString);
				//SetChangeMade();
				UpdateDisplay();

				bool TempConfirmSampling = (vLocalSystemData->vSamplingParameterSingleHeadQuantity[vSamplingType] < 13);

				if (!TempConfirmSampling)  //if they asked to sample more than 12 containers, have them confirm it
				{
					CYesNoDialog TempYesNoDialog;
					CString TempText = "\n\nAre you sure you want to sample " + dtoa(vLocalSystemData->vSamplingParameterSingleHeadQuantity[vSamplingType], 0);
					TempText = TempText + "\ncontainers?";
					TempYesNoDialog.vNoticeText = TempText;
					CString TempText1 = "Sample " + dtoa(vLocalSystemData->vSamplingParameterSingleHeadQuantity[vSamplingType], 0);
					TempYesNoDialog.vYesButtonText = TempText1;
					TempYesNoDialog.vNoButtonText = "Cancel";
					TempYesNoDialog.vQuestionType = cConfirmQuestion;
					int TempResult = TempYesNoDialog.DoModal();
					if (TempResult == IDOK)
					{
						TempConfirmSampling = true;
					}
				}
				if (TempConfirmSampling)
				{
					vLocalSystemData->vSamplingNowWhichHeadToSample[vSamplingType] = vLocalSystemData->vSamplingParameterSingleHead[vSamplingType]; //load which valve to sample
					vLocalSystemData->vSamplingNowNumberOfSamplesToTakeFromSameHead[vSamplingType] = vLocalSystemData->vSamplingParameterSingleHeadQuantity[vSamplingType];
					ReportErrorMessage("Operator Started " + vLocalSystemData->vSamplingName[vSamplingType] + " Sampling single " + vValve_HeadTextString + ": " + dtoa(vLocalSystemData->vSamplingParameterSingleHead[vSamplingType], 0) + 
						" Quantity: " + dtoa(vLocalSystemData->vSamplingParameterSingleHeadQuantity[vSamplingType], 0), cUserAction,0);
				}
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
		if (nResponse == 10)
		{
			//Main Menu button pressed
			KillTimer(vUpdateDisplayTimerHandle);
			CDialog::EndDialog(10);
		}
	}
}

void CSamplingDialog::OnSubFunction2Button()
{ //sample one container from each head
	if (vSamplingType != cRandomSamplingType)
	if (vLocalConfigurationData->vShowSampleAllButton & vSamplingType) //if showing this button
	if (vSamplesToTake > 0) //cancel taking current samples
	{
		for (BYTE TempLoop = 0; TempLoop < 3; TempLoop++)
		{
			vLocalSystemData->vSamplingNowNumberOfSamplesToTakeFromSeries[TempLoop] = 0;
			vLocalSystemData->vSamplingNowNumberOfSamplesToTakeFromSameHead[TempLoop] = 0;

			if (vLocalSystemData->vNeedToDisableEjectorsAfterSampleAll) 
			{
				vMainWindowPointer->EnableDisableEjectors(false);
				vLocalSystemData->vNeedToDisableEjectorsAfterSampleAll = false;
				ReportErrorMessage("Disable Ejectors after Sampling containers and aborted", cWriteToLog,0);
			}

			ReportErrorMessage("Operator Aborted Sampling", cUserAction,0);
			UpdateDisplay();
		}
	}
	else
	if (vLocalSystemData->vSystemRunMode != cRunningSystemMode)
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\nScanTrac must be inspecting to sample";
		TempNoticeDialog.vType = cInformationMessage;
		TempNoticeDialog.DoModal();
	}
	else
	{
		bool TempConfirmSampling = (vLocalConfigurationData->vNumberOfHeadsToMonitor[vSamplingType] < 13);

		if (!TempConfirmSampling)  //if they asked to sample more than 12 containers, have them confirm it
		{
			CYesNoDialog TempYesNoDialog;
			CString TempText = "\n\nAre you sure you want to sample " + dtoa(vLocalConfigurationData->vNumberOfHeadsToMonitor[vSamplingType], 0);
			TempText = TempText + "\ncontainers?";
			TempYesNoDialog.vNoticeText = TempText;
			CString TempText1 = "Sample " + dtoa(vLocalConfigurationData->vNumberOfHeadsToMonitor[vSamplingType], 0);
			TempYesNoDialog.vYesButtonText = TempText1;
			TempYesNoDialog.vNoButtonText = "Cancel";
			TempYesNoDialog.vQuestionType = cConfirmQuestion;
			int TempResult = TempYesNoDialog.DoModal();
			if (TempResult == IDOK)
			{
				TempConfirmSampling = true;
			}
		}
		if (TempConfirmSampling)
		{
			vLocalSystemData->vSamplingNowWhichHeadToSample[vSamplingType] = 1;
			vLocalSystemData->vSamplingNowNumberOfSamplesToTakeFromSeries[vSamplingType] = vLocalConfigurationData->vNumberOfHeadsToMonitor[vSamplingType];
			ReportErrorMessage("Operator Started " + vLocalSystemData->vSamplingName[vSamplingType] + " Sampling all " + vValve_HeadTextString + "s", cUserAction,0);
		}
	}
}


void CSamplingDialog::OnSubFunction3Button()
{
	if (vSamplesToTake > 0) //cancel taking current samples
	{
		for (BYTE TempLoop = 0; TempLoop < 3; TempLoop++)
		{
			vLocalSystemData->vSamplingNowNumberOfSamplesToTakeFromSeries[TempLoop] = 0;
			vLocalSystemData->vSamplingNowNumberOfSamplesToTakeFromSameHead[TempLoop] = 0;

			if (vLocalSystemData->vNeedToDisableEjectorsAfterSampleAll) 
			{
				vMainWindowPointer->EnableDisableEjectors(false);
				vLocalSystemData->vNeedToDisableEjectorsAfterSampleAll = false;
				ReportErrorMessage("Disable Ejectors after Sampling containers and aborted", cWriteToLog,0);
			}

			ReportErrorMessage("Operator Aborted Sampling", cUserAction,0);
			UpdateDisplay();
		}
	}
	else
	if (vLocalSystemData->vSystemRunMode != cRunningSystemMode)
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\nScanTrac must be inspecting to sample";
		TempNoticeDialog.vType = cInformationMessage;
		TempNoticeDialog.DoModal();
	}
	else
	if (vSamplingType != cRandomSamplingType)
	{
		if (vLocalConfigurationData->vNumberOfHeadsToMonitor[vSamplingType])
		{
			CNumericEntryDialog INumericEntryDialog;  
				
			//Set dialog box data titles and number value
			INumericEntryDialog.vEditString = dtoa(vLocalSystemData->vSamplingParameterSetStartHead[vSamplingType], 0);

			INumericEntryDialog.m_DialogTitleStaticText1 = "Enter " + vValve_HeadTextString + " to start taking samples from";
			INumericEntryDialog.m_DialogTitleStaticText2 = "";
			CString TempText = " ";
			TempText.LoadString(IDS_OriginalValue);
			INumericEntryDialog.m_DialogTitleStaticText4 = TempText + ": " + INumericEntryDialog.vEditString;

			INumericEntryDialog.m_UnitsString = "Maximum: " + dtoa(vLocalConfigurationData->vNumberOfHeadsToMonitor[vSamplingType], 0);
			INumericEntryDialog.vIntegerOnly = true;
			INumericEntryDialog.vAllowZero = false;
			INumericEntryDialog.vMaxValue = vLocalConfigurationData->vNumberOfHeadsToMonitor[vSamplingType];
			INumericEntryDialog.vMinValue = 1;
			//Pass control to dialog box and display
			int nResponse = INumericEntryDialog.DoModal();
			//dialog box is now closed, if user pressed select do this
			//if user pressed cancel, do nothing
			if (nResponse == IDOK)
			{
				vLocalSystemData->vSamplingParameterSetStartHead[vSamplingType] = _wtoi(INumericEntryDialog.vEditString);
				//SetChangeMade();
				UpdateDisplay();
				
				//Set dialog box data titles and number value
				INumericEntryDialog.vEditString = dtoa(vLocalSystemData->vSamplingParameterSetHeadQuantity[vSamplingType], 0);

				INumericEntryDialog.m_DialogTitleStaticText1 = "Enter Number of Samples to eject";
				INumericEntryDialog.m_DialogTitleStaticText2 = "";
				CString TempText = " ";
				TempText.LoadString(IDS_OriginalValue);
				INumericEntryDialog.m_DialogTitleStaticText4 = TempText + ": " + INumericEntryDialog.vEditString;

				INumericEntryDialog.m_UnitsString = "";
				INumericEntryDialog.vIntegerOnly = true;
				INumericEntryDialog.vMaxValue = 100;
				INumericEntryDialog.vMinValue = 0;
				//Pass control to dialog box and display
				int nResponse = INumericEntryDialog.DoModal();
				//dialog box is now closed, if user pressed select do this
				//if user pressed cancel, do nothing
				if (nResponse == IDOK)
				{
					vLocalSystemData->vSamplingParameterSetHeadQuantity[vSamplingType] = _wtoi(INumericEntryDialog.vEditString);
					//SetChangeMade();
					UpdateDisplay();

					bool TempConfirmSampling = (vLocalSystemData->vSamplingParameterSetHeadQuantity[vSamplingType] < 13);

					if (!TempConfirmSampling)  //if they asked to sample more than 12 containers, have them confirm it
					{
						CYesNoDialog TempYesNoDialog;
						CString TempText = "\n\nAre you sure you want to sample " + dtoa(vLocalSystemData->vSamplingParameterSetHeadQuantity[vSamplingType], 0);
						TempText = TempText + "\ncontainers?";
						TempYesNoDialog.vNoticeText = TempText;
						CString TempText1 = "Sample " + dtoa(vLocalSystemData->vSamplingParameterSetHeadQuantity[vSamplingType], 0);
						TempYesNoDialog.vYesButtonText = TempText1;
						TempYesNoDialog.vNoButtonText = "Cancel";
						TempYesNoDialog.vQuestionType = cConfirmQuestion;
						int TempResult = TempYesNoDialog.DoModal();
						if (TempResult == IDOK)
						{
							TempConfirmSampling = true;
						}
					}
					if (TempConfirmSampling)
					{
						vLocalSystemData->vSamplingNowWhichHeadToSample[vSamplingType] = vLocalSystemData->vSamplingParameterSetStartHead[vSamplingType];
						vLocalSystemData->vSamplingNowNumberOfSamplesToTakeFromSeries[vSamplingType] = vLocalSystemData->vSamplingParameterSetHeadQuantity[vSamplingType];
						ReportErrorMessage("Operator Started " + vLocalSystemData->vSamplingName[vSamplingType] + " Sampling " + vValve_HeadTextString + " Set Starting at: " + dtoa(vLocalSystemData->vSamplingNowWhichHeadToSample[vSamplingType], 0) + 
							" Quantity: " + dtoa(vLocalSystemData->vSamplingNowNumberOfSamplesToTakeFromSeries[vSamplingType], 0), cUserAction,0);
					}
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
			if (nResponse == 10)
			{
				//Main Menu button pressed
				KillTimer(vUpdateDisplayTimerHandle);
				CDialog::EndDialog(10);
			}
		}
	}
	else
	{
		//Set dialog box data titles and number value
		CNumericEntryDialog INumericEntryDialog;  
		INumericEntryDialog.vEditString = dtoa(vLocalSystemData->vSamplingParameterSetHeadQuantity[vSamplingType], 0);

		INumericEntryDialog.m_DialogTitleStaticText1 = "Enter Number of Samples to eject";
		INumericEntryDialog.m_DialogTitleStaticText2 = "";
		CString TempText = " ";
		TempText.LoadString(IDS_OriginalValue);
		INumericEntryDialog.m_DialogTitleStaticText4 = TempText + ": " + INumericEntryDialog.vEditString;

		INumericEntryDialog.m_UnitsString = "";
		INumericEntryDialog.vIntegerOnly = true;
		INumericEntryDialog.vMaxValue = 100;
		INumericEntryDialog.vMinValue = 0;
		//Pass control to dialog box and display
		int nResponse = INumericEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			vLocalSystemData->vSamplingParameterSetHeadQuantity[vSamplingType] = _wtoi(INumericEntryDialog.vEditString);
			//SetChangeMade();
			UpdateDisplay();

			bool TempConfirmSampling = (vLocalSystemData->vSamplingParameterSetHeadQuantity[vSamplingType] < 13);

			if (!TempConfirmSampling)  //if they asked to sample more than 12 containers, have them confirm it
			{
				CYesNoDialog TempYesNoDialog;
				CString TempText = "\n\nAre you sure you want to sample " + dtoa(vLocalSystemData->vSamplingParameterSetHeadQuantity[vSamplingType], 0);
				TempText = TempText + "\ncontainers?";
				TempYesNoDialog.vNoticeText = TempText;
				CString TempText1 = "Sample " + dtoa(vLocalSystemData->vSamplingParameterSetHeadQuantity[vSamplingType], 0);
				TempYesNoDialog.vYesButtonText = TempText1;
				TempYesNoDialog.vNoButtonText = "Cancel";
				TempYesNoDialog.vQuestionType = cConfirmQuestion;
				int TempResult = TempYesNoDialog.DoModal();
				if (TempResult == IDOK)
				{
					TempConfirmSampling = true;
				}
			}
			if (TempConfirmSampling)
			{
				vLocalSystemData->vLastIndexPulseWasCorrect[cRandomSamplingType] = true;
				vLocalSystemData->vSamplingNowNumberOfSamplesToTakeFromSeries[vSamplingType] = vLocalSystemData->vSamplingParameterSetHeadQuantity[vSamplingType];
				ReportErrorMessage("Operator Started Random Sampling Quantity: " + dtoa(vLocalSystemData->vSamplingNowNumberOfSamplesToTakeFromSeries[vSamplingType], 0), cUserAction,0);
			}
		}
		else 
		if (nResponse == 10)
		{
			//Main Menu button pressed
			KillTimer(vUpdateDisplayTimerHandle);
			CDialog::EndDialog(10);
		}
	}
}


void CSamplingDialog::OnSubFunction4Button()
{
	// TODO: Add your control notification handler code here
}


void CSamplingDialog::OnSubFunction5Button()
{
	if (vNumberOfEjectors > 1)
	{
		int TempResult = vLocalConfigurationData->SelectEjectors(vLocalSystemData->vSamplingName[vSamplingType] + " Sampling", &vGlobalCurrentProduct->vSamplingEjector[vSamplingType], 
			vLocalConfigurationData, vLocalSystemData->vInAutoSetup);
		if (TempResult == IDOK)
		{
			UpdateDisplay();
			//SetChangeMade();
		}
		else
		if (TempResult == 10)
		{
			//Main Menu Button pressed
			Sleep(1); //is the equivelent of a yeild statement;
			CDialog::EndDialog(10);
		}
	}
}


void CSamplingDialog::OnSubFunction6Button()
{
	vLocalConfigurationData->vStaggerSampling = !vLocalConfigurationData->vStaggerSampling;
	UpdateDisplay();

}


void CSamplingDialog::OnSubFunction7Button()
{
	// TODO: Add your control notification handler code here
}


void CSamplingDialog::OnSubFunction8Button()
{
	// TODO: Add your control notification handler code here
}


HBRUSH CSamplingDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	HWND TempDialogHwnd = this->m_hWnd;
	int TextSize;
	pDC->SetAttribDC(pDC->m_hDC);
	
	if (pWnd->GetDlgCtrlID() == IDC_DialogTitleStaticText1)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DialogStaticText1, 8);
		pDC->SetTextColor(vGlobalDialogTitleColor);
	}

	if (pWnd->GetDlgCtrlID() == IDC_LastIndexPulseDisplay)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_LastIndexPulseDisplay, 5);
		if ((!vOldLastIndexPulseWasCorrect[vSamplingType]) && (vLocalSystemData->vSamplingIndexErrorDataHave[vSamplingType]))
		{
			pDC->SetBkMode(TRANSPARENT);
			return vLocalSystemData->vRedBrush;
		}
	}
	
	if (pWnd->GetDlgCtrlID() == IDC_Background)
	{
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_FillerStatusDisplay) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_FillerStatusDisplay, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SamplesToTakeDisplay) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SamplesToTakeDisplay, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_LastHeadProcessedDisplay)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_LastHeadProcessedDisplay, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SeamerStatusDisplay) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SeamerStatusDisplay, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction1Display)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction1Display, 5);
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction3Display)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction3Display, 5);
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction5Display)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction5Display, 5);
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction6Display)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction6Display, 5);
	if (pWnd->GetDlgCtrlID() == IDC_ClockDisplay2)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_ClockDisplay2, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_FillerStatusDisplay2) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_FillerStatusDisplay2, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_LastIndexPulseDisplay) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_LastIndexPulseDisplay, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_NumberOfRotationsDisplay)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_NumberOfRotationsDisplay, 5);
		pDC->SetBkMode(TRANSPARENT);
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
		if (vOldShowAbortSamplingOnButtons)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cButtonTextColor);
			return vLocalSystemData->vRedBrush;
		}
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction2Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction2Button, 5);  //5 is medium large
		if (vOldShowAbortSamplingOnButtons)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cButtonTextColor);
			return vLocalSystemData->vRedBrush;
		}
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction3Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction3Button, 5);  //5 is medium large
		if (vOldShowAbortSamplingOnButtons)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cButtonTextColor);
			return vLocalSystemData->vRedBrush;
		}
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


void CSamplingDialog::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	if (nIDEvent == vUpdateDisplayTimerHandle)
	{
		KillTimer(vUpdateDisplayTimerHandle);
		OneSecondDisplayUpdate();
		int TempTimerResult = SetTimer(vUpdateDisplayTimerHandle,500,NULL);
		if (!TempTimerResult)
			ReportErrorMessage("Error-Display Timer Failed",cEMailInspx,32000);
	}
	CDialogEx::OnTimer(nIDEvent);
}


BOOL CSamplingDialog::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class

	CWnd * TempWindow = NULL;
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
			case 36:  //Home key, Screen Capture
				vGlobalShiftKeyDown = true;
				PostThreadMessage(vGlobalCallingThreadID,cHiddenKeyPressed, 0, 0x61);
			break;
			case cPageDown: // Help
				PostThreadMessage(vGlobalCallingThreadID, cShowHelp, 0, 0);
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
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CSamplingDialog::UpdateDisplay()
{
	if (vSamplingType == cRandomSamplingType)
	{
		SetDlgItemText(IDC_SubFunction3Display, "Quantity: " + dtoa(vLocalSystemData->vSamplingParameterSetHeadQuantity[vSamplingType], 0));

		if (vOldShowAbortSamplingOnButtons)
			SetDlgItemText(IDC_SubFunction3Button, _T("Abort Current Sampling"));
		else
			SetDlgItemText(IDC_SubFunction3Button, _T("Sample Containers Now"));
	}
	else
	{
		SetDlgItemText(IDC_SubFunction1Display, vValve_HeadTextString + " # " + dtoa(vLocalSystemData->vSamplingParameterSingleHead[vSamplingType], 0) + "  Quantity: " + dtoa(vLocalSystemData->vSamplingParameterSingleHeadQuantity[vSamplingType], 0));
		SetDlgItemText(IDC_SubFunction3Display, "Start " + vValve_HeadTextString + " # " + dtoa(vLocalSystemData->vSamplingParameterSetStartHead[vSamplingType], 0) + "  Quantity: " + dtoa(vLocalSystemData->vSamplingParameterSetHeadQuantity[vSamplingType], 0));

		if (vOldShowAbortSamplingOnButtons)
		{
			SetDlgItemText(IDC_SubFunction1Button, _T("Abort Current Sampling"));
			SetDlgItemText(IDC_SubFunction2Button, _T("Abort Current Sampling"));
			SetDlgItemText(IDC_SubFunction3Button, _T("Abort Current Sampling"));
		}
		else
		{
			CString TempText = _T("Sample From One ") + vValve_HeadTextString;
			SetDlgItemText(IDC_SubFunction1Button, TempText);
			TempText = _T("Sample\nAll\n") + vValve_HeadTextString + "s";
			SetDlgItemText(IDC_SubFunction2Button, TempText);
			TempText = _T("Sample Set Of ") + vValve_HeadTextString + "s";
			SetDlgItemText(IDC_SubFunction3Button, TempText);
		}
	}

	if (vLocalConfigurationData->vStaggerSampling)
	{
		if (vSamplingType == cRandomSamplingType)
			SetDlgItemText(IDC_SubFunction6Display, _T("Staggered Sampling (every tenth)"));
		else
			SetDlgItemText(IDC_SubFunction6Display, _T("Staggered Sampling (1 / revolution)"));
		SetDlgItemText(IDC_SubFunction6Button, _T("Consec- utive Sampling"));
	}
	else
	{
		SetDlgItemText(IDC_SubFunction6Display, _T("Consecutive Sampling"));
		SetDlgItemText(IDC_SubFunction6Button, _T("Stagger Sampling"));
	}

	SetDlgItemText(IDC_SubFunction5Display, vLocalConfigurationData->GetEjectorNames(vGlobalCurrentProduct->vSamplingEjector[vSamplingType]));
}

void CSamplingDialog::OneSecondDisplayUpdate()
{
	if (vSamplingType == cRandomSamplingType)
	{
		if (vOldSamplesEjectedCount[cRandomSamplingType] != vLocalSystemData->vSamplesEjectedCount[cRandomSamplingType])
		{
			vOldSamplesEjectedCount[cRandomSamplingType] = vLocalSystemData->vSamplesEjectedCount[cRandomSamplingType];

			if (vOldSamplesEjectedCount[cRandomSamplingType] == 1)
				SetDlgItemText(IDC_FillerStatusDisplay, dtoa(vOldSamplesEjectedCount[cRandomSamplingType], 0) + " sample ejected");
			else
				SetDlgItemText(IDC_FillerStatusDisplay, dtoa(vOldSamplesEjectedCount[cRandomSamplingType], 0) + " samples ejected");
		}
	}
	else
	{
		if (vOldNextHeadToProcess[vSamplingType] != vLocalSystemData->vNextHeadToProcess[vSamplingType])
		{
			vOldNextHeadToProcess[vSamplingType] = vLocalSystemData->vNextHeadToProcess[vSamplingType];
			SetDlgItemText(IDC_LastHeadProcessedDisplay, "Most recent " + vValve_HeadTextString + " analyzed: " + dtoa(vMainWindowPointer->CalculateHeadNumber(vOldNextHeadToProcess[vSamplingType], vSamplingType), 0));
		}

		if (vOldSamplingIndexErrorDataHave[vSamplingType] != vLocalSystemData->vSamplingIndexErrorDataHave[vSamplingType])
		{
			vOldSamplingIndexErrorDataHave[vSamplingType] = vLocalSystemData->vSamplingIndexErrorDataHave[vSamplingType];
			SetDlgItemText(IDC_NumberOfRotationsDisplay, "Number Of Rotations: " + dtoa(vOldSamplingIndexErrorDataHave[vSamplingType], 0));
		}

		if (vOldLastIndexPulseWasCorrect[vSamplingType] != vLocalSystemData->vLastIndexPulseWasCorrect[vSamplingType])
		{
			vOldLastIndexPulseWasCorrect[vSamplingType] = vLocalSystemData->vLastIndexPulseWasCorrect[vSamplingType];
			if (vOldLastIndexPulseWasCorrect[vSamplingType])
				SetDlgItemText(IDC_LastIndexPulseDisplay, _T("Last Index Pulse: OK"));
			else
				SetDlgItemText(IDC_LastIndexPulseDisplay, _T("Last Index Pulse: OFF"));
		}
	

		if (vOldSamplesEjectedCount[cFillerSamplingType] != vLocalSystemData->vSamplesEjectedCount[cFillerSamplingType])
		{
			vOldSamplesEjectedCount[cFillerSamplingType] = vLocalSystemData->vSamplesEjectedCount[cFillerSamplingType];

			if (vOldSamplesEjectedCount[cFillerSamplingType] == 1)
				SetDlgItemText(IDC_FillerStatusDisplay, dtoa(vOldSamplesEjectedCount[cFillerSamplingType], 0) + " " + vLocalSystemData->vSamplingName[cFillerSamplingType] + " sample ejected");
			else
				SetDlgItemText(IDC_FillerStatusDisplay, dtoa(vOldSamplesEjectedCount[cFillerSamplingType], 0) + " " + vLocalSystemData->vSamplingName[cFillerSamplingType] + " samples ejected");
		}
		if (vOldSamplesEjectedCount[cSeamerSamplingType] != vLocalSystemData->vSamplesEjectedCount[cSeamerSamplingType])
		{
			vOldSamplesEjectedCount[cSeamerSamplingType] = vLocalSystemData->vSamplesEjectedCount[cSeamerSamplingType];

			if (vOldSamplesEjectedCount[cSeamerSamplingType] == 1)
				SetDlgItemText(IDC_SeamerStatusDisplay, dtoa(vOldSamplesEjectedCount[cSeamerSamplingType], 0) + " " + vLocalSystemData->vSamplingName[cSeamerSamplingType] + " sample ejected");
			else
				SetDlgItemText(IDC_SeamerStatusDisplay, dtoa(vOldSamplesEjectedCount[cSeamerSamplingType], 0) + " " + vLocalSystemData->vSamplingName[cSeamerSamplingType] + " samples ejected");
		}
	}

	bool vShowAbortSamplingOnButtons = false;
	for (BYTE TempLoop = 0; TempLoop < 3; TempLoop++)
	if ((vLocalSystemData->vSamplingNowNumberOfSamplesToTakeFromSeries[TempLoop]) || (vLocalSystemData->vSamplingNowNumberOfSamplesToTakeFromSameHead[TempLoop]))
		vShowAbortSamplingOnButtons = true;

	if (vOldShowAbortSamplingOnButtons != vShowAbortSamplingOnButtons)
	{
		vOldShowAbortSamplingOnButtons = vShowAbortSamplingOnButtons;
		UpdateDisplay();
	}

	vSamplesToTake = 0;
	for (BYTE TempLoop = 0; TempLoop < 3; TempLoop++)
	{
		vSamplesToTake = vSamplesToTake + vLocalSystemData->vSamplingNowNumberOfSamplesToTakeFromSeries[TempLoop];
		vSamplesToTake = vSamplesToTake + vLocalSystemData->vSamplingNowNumberOfSamplesToTakeFromSameHead[TempLoop];
	}

	if (vOldSamplesToTake != vSamplesToTake)
	{
		vOldSamplesToTake = vSamplesToTake;

		if (vOldSamplesToTake == 0)
			SetDlgItemText(IDC_SamplesToTakeDisplay, _T(""));
		else
		if (vOldSamplesToTake == 1)
			SetDlgItemText(IDC_SamplesToTakeDisplay, _T("1 sample to take"));
		else
			SetDlgItemText(IDC_SamplesToTakeDisplay, dtoa(vOldSamplesToTake, 0) + " samples to take");
	}

	CTime TempTime;
	TempTime = CTime::GetCurrentTime();
	CString TempTimeString = " ";
	if (vLocalConfigurationData->vShow24HourTime)
		TempTimeString = TempTime.Format("%H:%M:%S");//24 hour time format
	else
		TempTimeString = TempTime.Format("%#I:%M:%S%p");//12 hour time format with am pm
	SetDlgItemText(IDC_ClockDisplay2, TempTimeString);
}

void CSamplingDialog::SetChangeMade() 
{
	if (!vChangeMade)
	{
		m_Function2Button.ShowWindow(SW_SHOW);
		m_Function2Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		m_Function4Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
		m_Function4Button.ShowWindow(SW_SHOW);
		CString TempText;
		TempText.LoadString(IDS_SaveAndExit);
		SetDlgItemText(IDC_Function2Button,TempText);
		TempText.LoadString(IDS_Cancel);
		SetDlgItemText(IDC_Function4Button,TempText);
		vChangeMade = true;
	}
}



void CSamplingDialog::OnDialogtitlestatictext1()
{
	BYTE TempHiddenKeys = 8;
	if (!PasswordOK(cTemporaryInspxPassword,false))
		TempHiddenKeys = 10;
	PostThreadMessage(vGlobalCallingThreadID, cDisplaySoftKeyMenu, TempHiddenKeys, TempHiddenKeys);
}
