// ProductFillerSeamerSampling.cpp : implementation file
//

#include "stdafx.h"
#include "ScanTrac.h"
#include "ProductFillerSeamerSampling.h"
#include "afxdialogex.h"
#include "NumericEntryDialog.h"
#include "SelectItemDialog.h"

// ProductFillerSeamerSampling dialog

IMPLEMENT_DYNAMIC(CProductFillerSeamerSampling, CDialogEx)

extern BYTE vGlobalShiftKeyDown;
extern unsigned long vGlobalDialogTitleColor;
extern CProduct *vGlobalCurrentProduct;

CProductFillerSeamerSampling::CProductFillerSeamerSampling(CWnd* pParent /*=NULL*/)
	: CDialogEx(CProductFillerSeamerSampling::IDD, pParent)
{
	vMainWindowPointer = NULL;
	//vLocalProductCollection = NULL;
	vLocalSystemData = NULL;
	vLocalConfigurationData = NULL;
	vChangeMade = false;
	vUpdateDisplayTimerHandle = 5050;

	vOldFindFillerOffset[0] = false;
	vOldFindFillerOffset[1] = false;
	vOldFindFillerOffset[2] = false;
	vOldFindFillerOffset[3] = false;
	vOldCurrentEncoderRate = 0;

	vLocalCWndCollection.Add(this);
	vLocalCWndCollection.Add(&m_Background);
	vLocalCWndCollection.Add(&m_DialogTitleStaticText);
	vLocalCWndCollection.Add(&m_Function1Button);
	vLocalCWndCollection.Add(&m_Function2Button);
	vLocalCWndCollection.Add(&m_Function3Button);
	vLocalCWndCollection.Add(&m_Function4Button);
	vLocalCWndCollection.Add(&m_Function5Button);
	vLocalCWndCollection.Add(&m_SubFunction1Button);
	vLocalCWndCollection.Add(&m_SubFunction1Display);
	vLocalCWndCollection.Add(&m_SubFunction2Button);
	vLocalCWndCollection.Add(&m_SubFunction2Display);
	vLocalCWndCollection.Add(&m_SubFunction3Button);
	vLocalCWndCollection.Add(&m_SubFunction3Display);
	vLocalCWndCollection.Add(&m_SubFunction4Button);
	vLocalCWndCollection.Add(&m_SubFunction4Display);
	vLocalCWndCollection.Add(&m_SubFunction5Button);
	vLocalCWndCollection.Add(&m_SubFunction5Display);
	vLocalCWndCollection.Add(&m_SubFunction6Button);
	vLocalCWndCollection.Add(&m_SubFunction6Display);
	vLocalCWndCollection.Add(&m_SubFunction7Button);
	vLocalCWndCollection.Add(&m_SubFunction7Display);
	vLocalCWndCollection.Add(&m_SubFunction8Button);
	vLocalCWndCollection.Add(&m_SubFunction8Display);
	vLocalCWndCollection.Add(&m_Display1);
	vLocalCWndCollection.Add(&m_Display3);
	vLocalCWndCollection.Add(&m_Display4);
	vLocalCWndCollection.Add(&m_Display5);
	vLocalCWndCollection.Add(&m_Display6);
	vLocalCWndCollection.Add(&m_Display7);
	vLocalCWndCollection.Add(&m_Function3Display1);
	vLocalCWndCollection.Add(&m_FrequencyRange1Display);
	vLocalCWndCollection.Add(&m_FrequencyRange2Display);
	vLocalCWndCollection.Add(&m_FrequencyRange3Display);
	vLocalCWndCollection.Add(&m_FrequencyRange4Display);
	vLocalCWndCollection.Add(&m_SubFunction1DisplayAbove);
	vLocalCWndCollection.Add(&m_SubFunction5DisplayAbove);
	vLocalCWndCollection.Add(&m_EncoderRate);
	vLocalCWndCollection.Add(&m_EncoderRateLabel);
	vLocalCWndCollection.Add(&m_Time2);
	vLocalCWndCollection.Add(&m_Date);

}

CProductFillerSeamerSampling::~CProductFillerSeamerSampling()
{

}

void CProductFillerSeamerSampling::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_Background, m_Background);
	DDX_Control(pDX, IDC_DialogTitleStaticText1, m_DialogTitleStaticText);
	DDX_Control(pDX, IDC_Display1, m_Display1);
	DDX_Control(pDX, IDC_Display3, m_Display3);
	DDX_Control(pDX, IDC_Display4, m_Display4);
	DDX_Control(pDX, IDC_Display5, m_Display5);
	DDX_Control(pDX, IDC_Display6, m_Display6);
	DDX_Control(pDX, IDC_Display7, m_Display7);
	DDX_Control(pDX, IDC_SubFunction1Display, m_SubFunction1Display);
	DDX_Control(pDX, IDC_SubFunction2Display, m_SubFunction2Display);
	DDX_Control(pDX, IDC_SubFunction3Display, m_SubFunction3Display);
	DDX_Control(pDX, IDC_SubFunction4Display, m_SubFunction4Display);
	DDX_Control(pDX, IDC_SubFunction5Display, m_SubFunction5Display);
	DDX_Control(pDX, IDC_SubFunction6Display, m_SubFunction6Display);
	DDX_Control(pDX, IDC_SubFunction7Display, m_SubFunction7Display);
	DDX_Control(pDX, IDC_SubFunction8Display, m_SubFunction8Display);
	DDX_Control(pDX, IDC_Function3Display1, m_Function3Display1);
	DDX_Control(pDX, IDC_SubFunction1DisplayAbove, m_SubFunction1DisplayAbove);
	DDX_Control(pDX, IDC_SubFunction5DisplayAbove, m_SubFunction5DisplayAbove);
	DDX_Control(pDX, IDC_Function1Button, m_Function1Button);
	DDX_Control(pDX, IDC_Function2Button, m_Function2Button);
	DDX_Control(pDX, IDC_Function3Button, m_Function3Button);
	DDX_Control(pDX, IDC_Function4Button, m_Function4Button);
	DDX_Control(pDX, IDC_Function5Button, m_Function5Button);
	DDX_Control(pDX, IDC_SubFunction1Button, m_SubFunction1Button);
	DDX_Control(pDX, IDC_SubFunction2Button, m_SubFunction2Button);
	DDX_Control(pDX, IDC_SubFunction3Button, m_SubFunction3Button);
	DDX_Control(pDX, IDC_SubFunction4Button, m_SubFunction4Button);
	DDX_Control(pDX, IDC_SubFunction5Button, m_SubFunction5Button);
	DDX_Control(pDX, IDC_SubFunction6Button, m_SubFunction6Button);
	DDX_Control(pDX, IDC_SubFunction7Button, m_SubFunction7Button);
	DDX_Control(pDX, IDC_SubFunction8Button, m_SubFunction8Button);
	DDX_Control(pDX, IDC_Time2, m_Time2);
	DDX_Control(pDX, IDC_Date, m_Date);
	DDX_Control(pDX, IDC_FrequencyRange1Display, m_FrequencyRange1Display);
	DDX_Control(pDX, IDC_FrequencyRange2Display, m_FrequencyRange2Display);
	DDX_Control(pDX, IDC_FrequencyRange3Display, m_FrequencyRange3Display);
	DDX_Control(pDX, IDC_FrequencyRange4Display, m_FrequencyRange4Display);
	DDX_Control(pDX, IDC_EncoderRate, m_EncoderRate);
	DDX_Control(pDX, IDC_EncoderRateLabel, m_EncoderRateLabel);
}


BEGIN_MESSAGE_MAP(CProductFillerSeamerSampling, CDialogEx)
	ON_WM_SHOWWINDOW()
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_STN_CLICKED(IDC_SubFunction1Button, &CProductFillerSeamerSampling::OnSubFunction1Button)
	ON_STN_CLICKED(IDC_SubFunction2Button, &CProductFillerSeamerSampling::OnSubFunction2Button)
	ON_STN_CLICKED(IDC_SubFunction3Button, &CProductFillerSeamerSampling::OnSubFunction3Button)
	ON_STN_CLICKED(IDC_SubFunction4Button, &CProductFillerSeamerSampling::OnSubFunction4Button)
	ON_STN_CLICKED(IDC_SubFunction5Button, &CProductFillerSeamerSampling::OnSubFunction5Button)
	ON_STN_CLICKED(IDC_SubFunction6Button, &CProductFillerSeamerSampling::OnSubFunction6Button)
	ON_STN_CLICKED(IDC_SubFunction7Button, &CProductFillerSeamerSampling::OnSubFunction7Button)
	ON_STN_CLICKED(IDC_SubFunction8Button, &CProductFillerSeamerSampling::OnSubFunction8Button)
	ON_STN_CLICKED(IDC_Function1Button, &CProductFillerSeamerSampling::OnFunction1Button)
	ON_STN_CLICKED(IDC_Function2Button, &CProductFillerSeamerSampling::OnFunction2Button)
	ON_STN_CLICKED(IDC_Function3Button, &CProductFillerSeamerSampling::OnFunction3Button)
	ON_STN_CLICKED(IDC_Function4Button, &CProductFillerSeamerSampling::OnFunction4Button)
	ON_STN_CLICKED(IDC_Function5Button, &CProductFillerSeamerSampling::OnFunction5Button)
	ON_STN_CLICKED(IDC_DialogTitleStaticText1, &CProductFillerSeamerSampling::OnDialogTitleStaticText1)
END_MESSAGE_MAP()



void CProductFillerSeamerSampling::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	m_Background.SetWindowPos(NULL,cMainBackgroundLeft, cMainBackgroundTop, cMainBackgroundLength, cMainBackgroundHeight, SWP_NOZORDER);
	// TODO: Add your message handler code here

	//put config variables in local copies
	vEncoderRateTolerance = vLocalConfigurationData->vEncoderRateTolerance;
	//put original config variables in backup local copies to revert to if cancel
	vOldEncoderRateTolerance = vEncoderRateTolerance;

	vOldEncoderRate[0] = vGlobalCurrentProduct->vEncoderRateIndexesForFillerSeamerSampling[0];
	vOldEncoderRate[1] = vGlobalCurrentProduct->vEncoderRateIndexesForFillerSeamerSampling[1];
	vOldEncoderRate[2] = vGlobalCurrentProduct->vEncoderRateIndexesForFillerSeamerSampling[0];
	vOldEncoderRate[3] = vGlobalCurrentProduct->vEncoderRateIndexesForFillerSeamerSampling[1];

	vDistanceFromSeamerObjectToScanTracTrigger = vLocalConfigurationData->vDistanceFromSeamerObjectToScanTracTrigger;

	UpdateDisplay();
	this->SetWindowText(_T("ProductFillSeamSetup"));

	vTime =CTime::GetCurrentTime();
	int TempDateToday = vTime.GetDay();
	if (vDateToday != TempDateToday)
	{
		CString TempDateString = vTime.Format("%B %#d, %Y");
		CString TempChar = TempDateString.GetAt(0);
		TempChar.MakeUpper();
		TempDateString.SetAt(0,TempChar.GetAt(0));
		
		SetDlgItemText(IDC_Date,TempDateString);

		vDateToday=TempDateToday;
	}
	CString TempTimeString = " ";
	if (vLocalConfigurationData->vShow24HourTime)
		TempTimeString = vTime.Format("%H:%M:%S");//24 hour time format
	else
		TempTimeString = vTime.Format("%#I:%M:%S%p");//12 hour time format with am pm

	SetDlgItemText(IDC_Time2,TempTimeString);

	int TempTimerResult = SetTimer(vUpdateDisplayTimerHandle,1000,NULL);
	if (!TempTimerResult)
		ReportErrorMessage("Error-Display Timer SD Failed",cEMailInspx,32000);

	ShowHideInspxButtons(PasswordOK(cTemporaryInspxPassword, false));
	
	if (vLocalSystemData->vUseSamplingFIFO)
	{
		m_Function3Button.ShowWindow(SW_HIDE);
		m_Function5Button.ShowWindow(SW_HIDE);
		SetDlgItemText(IDC_SubFunction1Button, _T("Auto Find Filler Offset"));
		SetDlgItemText(IDC_SubFunction2Button, _T("Set Filler Offset"));
		m_SubFunction2Display.ShowWindow(SW_HIDE);

		m_SubFunction3Button.ShowWindow(SW_SHOW);
		m_SubFunction3Display.ShowWindow(SW_HIDE);
		SetDlgItemText(IDC_SubFunction3Button, _T("Calculate Filler Offset"));

		m_SubFunction4Button.ShowWindow(SW_HIDE);
		m_SubFunction4Display.ShowWindow(SW_HIDE);
		SetDlgItemText(IDC_SubFunction5Button, _T("Set Seamer Offset"));

		m_SubFunction6Button.ShowWindow(SW_SHOW);
		SetDlgItemText(IDC_SubFunction6Button, _T("Calculate Seamer Offset"));
		m_SubFunction6Display.ShowWindow(SW_HIDE);

		m_SubFunction7Button.ShowWindow(SW_HIDE);
		m_SubFunction7Display.ShowWindow(SW_HIDE);
		m_SubFunction8Button.ShowWindow(SW_SHOW);
		m_SubFunction8Display.ShowWindow(SW_SHOW);
		SetDlgItemText(IDC_SubFunction8Button, _T("Tracking FIFO Timeout Distance"));

		m_FrequencyRange1Display.ShowWindow(SW_HIDE);
		m_FrequencyRange2Display.ShowWindow(SW_HIDE);
		m_FrequencyRange3Display.ShowWindow(SW_HIDE);
		m_FrequencyRange4Display.ShowWindow(SW_HIDE);
		SetDlgItemText(IDC_SubFunction1DisplayAbove, _T("Filler Valve Monitor Index Pulse Offset"));
		SetDlgItemText(IDC_SubFunction5DisplayAbove, _T("Seamer Sampling Index Pulse Offset"));
		m_EncoderRate.ShowWindow(SW_HIDE);
		m_EncoderRateLabel.ShowWindow(SW_HIDE);
		m_Display1.ShowWindow(SW_HIDE);
		m_Display3.ShowWindow(SW_HIDE);
		m_Display4.ShowWindow(SW_HIDE);
		m_Display5.ShowWindow(SW_HIDE);
		m_Display6.ShowWindow(SW_HIDE);
		m_Display7.ShowWindow(SW_HIDE);
		m_Function3Display1.ShowWindow(SW_HIDE);
	}

	SetupMenu(vLocalCWndCollection);
}

void CProductFillerSeamerSampling::OnFunction1Button()
{
	//revert changes - clicked aborted
	vLocalConfigurationData->vEncoderRateTolerance = vOldEncoderRateTolerance;

	for (BYTE TempLoop = 0; TempLoop < cNumberOfFillerOffsetSpeeds; TempLoop++)
		vLocalSystemData->vFindFillerOffset[TempLoop] = 0; //if looking for an offset, stop operation

	CDialog::EndDialog(10);
}


void CProductFillerSeamerSampling::OnFunction2Button()
{
	//Save and exit was pressed
	if (!vChangeMade) 
	{
		KillTimer(vUpdateDisplayTimerHandle);
		for (BYTE TempLoop = 0; TempLoop < cNumberOfFillerOffsetSpeeds; TempLoop++)
			vLocalSystemData->vFindFillerOffset[TempLoop] = 0; //if looking for an offset, stop operation

		CDialog::EndDialog(1);
	}
	else
	if (KeepSetupChanges())
	{
		KillTimer(vUpdateDisplayTimerHandle);
		for (BYTE TempLoop = 0; TempLoop < cNumberOfFillerOffsetSpeeds; TempLoop++)
			vLocalSystemData->vFindFillerOffset[TempLoop] = 0; //if looking for an offset, stop operation

		vLocalConfigurationData->vDistanceFromSeamerObjectToScanTracTrigger = vDistanceFromSeamerObjectToScanTracTrigger;

		CDialog::EndDialog(1);
	}
}

void CProductFillerSeamerSampling::OnFunction3Button()
{
	if (!vLocalSystemData->vUseSamplingFIFO)
	if (PasswordOK(cTemporaryInspxPassword, false) || (vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType] == 0))
	{
		CNumericEntryDialog INumericEntryDialog;  
	
		char TempStringBuffer[11];
		int TempErr = _itoa_s(vLocalConfigurationData->vEncoderRateTolerance,TempStringBuffer,11,10);
		INumericEntryDialog.vEditString = TempStringBuffer;

		INumericEntryDialog.m_DialogTitleStaticText1 = "Enter Encoder Rate Tolerance";
		INumericEntryDialog.m_DialogTitleStaticText2 = "Global Setting; effects all products";
		INumericEntryDialog.m_DialogTitleStaticText3 = "Encoder Rate is in Hertz (Hz)";
		INumericEntryDialog.m_DialogTitleStaticText4 = "Default: 5%";

		INumericEntryDialog.m_UnitsString = "Value in %";
		INumericEntryDialog.vIntegerOnly = true;
		INumericEntryDialog.vAllowNegative = false;
		INumericEntryDialog.vMaxValue = vLocalSystemData->vMaximumEncoderRate;
		INumericEntryDialog.vMinValue = 0;
		//Pass control to dialog box and display
		int nResponse = INumericEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			int TempInt = _wtoi(INumericEntryDialog.vEditString);
			vLocalConfigurationData->vEncoderRateTolerance = TempInt;

			UpdateDisplay();
			SetChangeMade();

		}
		else 
		if (nResponse == 10)
		{
			//Main Menu Button pressed
			KillTimer(vUpdateDisplayTimerHandle);
			for (BYTE TempLoop = 0; TempLoop < cNumberOfFillerOffsetSpeeds; TempLoop++)
				vLocalSystemData->vFindFillerOffset[TempLoop] = 0; //if looking for an offset, stop operation
			CDialog::EndDialog(10);
		}
	}
}


void CProductFillerSeamerSampling::OnFunction4Button()
{
	//cancel
	vLocalConfigurationData->vEncoderRateTolerance = vOldEncoderRateTolerance;
	for (BYTE TempLoop = 0; TempLoop < cNumberOfFillerOffsetSpeeds; TempLoop++)
		vLocalSystemData->vFindFillerOffset[TempLoop] = 0; //if looking for an offset, stop operation
	CDialog::OnCancel();
}


void CProductFillerSeamerSampling::OnFunction5Button()
{
	if (!vLocalSystemData->vUseSamplingFIFO)
	if (PasswordOK(cTemporaryInspxPassword, false))
	{
		CNumericEntryDialog INumericEntryDialog;  
		//Set dialog box data titles and number value
		INumericEntryDialog.vEditString = dtoa(vGlobalCurrentProduct->vEncoderRateIndexesForFillerSeamerSampling[0], 0);

		INumericEntryDialog.m_DialogTitleStaticText1 = "Enter Encoder Rate 1.";
		INumericEntryDialog.m_DialogTitleStaticText2 = "Sets Mid Speed of Good Range for Index 1 Pulse Offsets";
		INumericEntryDialog.m_DialogTitleStaticText3 = "Encoder Rate 1 is a Product Setting which effects:";
		INumericEntryDialog.m_DialogTitleStaticText4 = vGlobalCurrentProduct->vProductName;

		INumericEntryDialog.m_UnitsString = "Example: 2000 Hertz (Hz)";
		INumericEntryDialog.vIntegerOnly = true;
		INumericEntryDialog.vAllowNegative = true;
		INumericEntryDialog.vMaxValue = vLocalSystemData->vMaximumEncoderRate;
		INumericEntryDialog.vMinValue = 0;
		//Pass control to dialog box and display
		int nResponse = INumericEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			int TempInt = _wtoi(INumericEntryDialog.vEditString);
		
			//Set dialog box data titles and number value
			INumericEntryDialog.vEditString = dtoa(vGlobalCurrentProduct->vEncoderRateIndexesForFillerSeamerSampling[1], 0);

			INumericEntryDialog.m_DialogTitleStaticText1 = "Enter Encoder Rate 2";
			INumericEntryDialog.m_DialogTitleStaticText2 = "Sets Mid Speed of Good Range for Index 2 Pulse Offsets";
			INumericEntryDialog.m_DialogTitleStaticText3 = "Encoder Rate 2 is a Product Setting which effects:";
			INumericEntryDialog.m_DialogTitleStaticText4 = vGlobalCurrentProduct->vProductName;

			INumericEntryDialog.m_UnitsString = "Example: 1500 Hertz (Hz)";
			INumericEntryDialog.vIntegerOnly = true;
			INumericEntryDialog.vAllowNegative = true;
			INumericEntryDialog.vMaxValue = vLocalSystemData->vMaximumEncoderRate;
			INumericEntryDialog.vMinValue = 0;
			//Pass control to dialog box and display
			int nResponse = INumericEntryDialog.DoModal();
			//dialog box is now closed, if user pressed select do this
			//if user pressed cancel, do nothing
			if (nResponse == IDOK)
			{
				int TempInt1 = _wtoi(INumericEntryDialog.vEditString);
				
				//SetDlgItemText(IDC_Display3, "Encoder Frequency: Full Speed\nSet To");
				//SetDlgItemText(IDC_Display2, "Half Speed\nSet To");
			
				CNumericEntryDialog INumericEntryDialog;  
				//Set dialog box data titles and number value
				INumericEntryDialog.vEditString = dtoa(vGlobalCurrentProduct->vEncoderRateIndexesForFillerSeamerSampling[2], 0);

				INumericEntryDialog.m_DialogTitleStaticText1 = "Enter Encoder Rate 3";
				INumericEntryDialog.m_DialogTitleStaticText2 = "Sets Mid Speed of Good Range for Index 3 Pulse Offsets";
				INumericEntryDialog.m_DialogTitleStaticText3 = "Encoder Rate 4 is a Product Setting which effects:";
				INumericEntryDialog.m_DialogTitleStaticText4 = vGlobalCurrentProduct->vProductName;

				INumericEntryDialog.m_UnitsString = "Example: 1000 Hertz (Hz)";
				INumericEntryDialog.vIntegerOnly = true;
				INumericEntryDialog.vAllowNegative = true;
				INumericEntryDialog.vMaxValue = vLocalSystemData->vMaximumEncoderRate;
				INumericEntryDialog.vMinValue = 0;
				//Pass control to dialog box and display
				int nResponse = INumericEntryDialog.DoModal();
				//dialog box is now closed, if user pressed select do this
				//if user pressed cancel, do nothing
				if (nResponse == IDOK)
				{
					int TempInt2 = _wtoi(INumericEntryDialog.vEditString);
					
					//Set dialog box data titles and number value
					INumericEntryDialog.vEditString = dtoa(vGlobalCurrentProduct->vEncoderRateIndexesForFillerSeamerSampling[3], 0);

					INumericEntryDialog.m_DialogTitleStaticText1 = "Enter Encoder Rate 4";
					INumericEntryDialog.m_DialogTitleStaticText2 = "Sets Mid Speed of Good Range for Index 4 Pulse Offsets";
					INumericEntryDialog.m_DialogTitleStaticText3 = "Encoder Rate 4 is a Product Setting which effects:";
					INumericEntryDialog.m_DialogTitleStaticText4 = vGlobalCurrentProduct->vProductName;

					INumericEntryDialog.m_UnitsString = "Example: 500 Hertz (Hz)";
					INumericEntryDialog.vIntegerOnly = true;
					INumericEntryDialog.vAllowNegative = true;
					INumericEntryDialog.vMaxValue = vLocalSystemData->vMaximumEncoderRate;
					INumericEntryDialog.vMinValue = 0;
					//Pass control to dialog box and display
					int nResponse = INumericEntryDialog.DoModal();
					//dialog box is now closed, if user pressed select do this
					//if user pressed cancel, do nothing
					if (nResponse == IDOK)
					{
						int TempInt3 = _wtoi(INumericEntryDialog.vEditString);
						vGlobalCurrentProduct->vEncoderRateIndexesForFillerSeamerSampling[0] = TempInt;
						vGlobalCurrentProduct->vEncoderRateIndexesForFillerSeamerSampling[1] = TempInt1;
						vGlobalCurrentProduct->vEncoderRateIndexesForFillerSeamerSampling[2] = TempInt2;
						vGlobalCurrentProduct->vEncoderRateIndexesForFillerSeamerSampling[3] = TempInt3;

						//SetDlgItemText(IDC_Display3, "Encoder Frequency: Full Speed\nSet To");
						//SetDlgItemText(IDC_Display2, "Half Speed\nSet To");
			
						UpdateDisplay();
						SetChangeMade();
					}
					else 
					if (nResponse == 10)
					{
						//Main Menu Button pressed
						KillTimer(vUpdateDisplayTimerHandle);
						for (BYTE TempLoop = 0; TempLoop < cNumberOfFillerOffsetSpeeds; TempLoop++)
							vLocalSystemData->vFindFillerOffset[TempLoop] = 0; //if looking for an offset, stop operation
						CDialog::EndDialog(10);
					}
				}
				else 
				if (nResponse == 10)
				{
					//Main Menu Button pressed
					KillTimer(vUpdateDisplayTimerHandle);
					for (BYTE TempLoop = 0; TempLoop < cNumberOfFillerOffsetSpeeds; TempLoop++)
						vLocalSystemData->vFindFillerOffset[TempLoop] = 0; //if looking for an offset, stop operation
					CDialog::EndDialog(10);
				}
			}
			else 
			if (nResponse == 10)
			{
				//Main Menu Button pressed
				KillTimer(vUpdateDisplayTimerHandle);
				for (BYTE TempLoop = 0; TempLoop < cNumberOfFillerOffsetSpeeds; TempLoop++)
					vLocalSystemData->vFindFillerOffset[TempLoop] = 0; //if looking for an offset, stop operation
				CDialog::EndDialog(10);
			}
		}
		else 
		if (nResponse == 10)
		{
			//Main Menu Button pressed
			KillTimer(vUpdateDisplayTimerHandle);
			for (BYTE TempLoop = 0; TempLoop < cNumberOfFillerOffsetSpeeds; TempLoop++)
				vLocalSystemData->vFindFillerOffset[TempLoop] = 0; //if looking for an offset, stop operation
			CDialog::EndDialog(10);
		}
	}
}

void CProductFillerSeamerSampling::OnSubFunction1Button()
{
	if ((vLocalSystemData->vFindFillerOffset[1]) || (vLocalSystemData->vFindFillerOffset[2]) || (vLocalSystemData->vFindFillerOffset[3]))//if we were already trying to find half speed offset
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\nYou can only find one offset at a time, please wait until the Half Speed Filler Sampler Offset is found.";
		TempNoticeDialog.vType = cNoticeMessage;
		TempNoticeDialog.DoModal();
	}
	else
	if (vLocalSystemData->vFindFillerOffset[0])	//if already calculating full speed offset, stop
	{
		vLocalSystemData->vFindFillerOffset[0] = false;
		UpdateDisplay();
	}
	else
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\nThe concept is to make the filler in valve #1 different than the other valves so the software can find valve 1 because it has a different fill or weight.  Then it can calculate the offset needed to put valve one in the first spot)";
		TempNoticeDialog.vType = cNoticeMessage;
		TempNoticeDialog.DoModal();

		vGlobalCurrentProduct->vEncoderRateIndexesForFillerSeamerSampling[0] = vLocalSystemData->vActualEncoderRate;
		vLocalSystemData->vFindFillerOffset[0] = AskIfContainerIsHighLowOrVariable();

		SetChangeMade();
		UpdateDisplay();
	}
}


void CProductFillerSeamerSampling::OnSubFunction2Button()
{
	if (vLocalSystemData->vUseSamplingFIFO)
		SetFillerOffsets();
	else
	if ((vLocalSystemData->vFindFillerOffset[0]) || (vLocalSystemData->vFindFillerOffset[2]) || (vLocalSystemData->vFindFillerOffset[3]))//if we were already trying to find half speed offset
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\nYou can only find one offset at a time, please wait until the Full Speed Filler Sampler Offset is found.";
		TempNoticeDialog.vType = cNoticeMessage;
		TempNoticeDialog.DoModal();
	}
	else
	if (vLocalSystemData->vFindFillerOffset[1])	//if we were already trying to find the half speed offset
	{
		vLocalSystemData->vFindFillerOffset[1] = 0;	//then turn it off
		UpdateDisplay();
	}
	else	//all ok, we want to find the filler half speed offset
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\nSet filler valve #1 to highest amount or\nput dense material in or around continers for valve #1\nThe software finds the heaviest container and\nadjusts the offset so it is valve #1.\nRequires at least 3 revolutions (2 to sync index pulse)";
		TempNoticeDialog.vType = cNoticeMessage;
		TempNoticeDialog.DoModal();

		vGlobalCurrentProduct->vEncoderRateIndexesForFillerSeamerSampling[1] = vLocalSystemData->vActualEncoderRate;
		vLocalSystemData->vFindFillerOffset[1] = AskIfContainerIsHighLowOrVariable();
		SetChangeMade();
		UpdateDisplay();
	}
}


void CProductFillerSeamerSampling::OnSubFunction3Button()
{
	if (vLocalSystemData->vUseSamplingFIFO)
	{ //calculate filler offset if person enters head that shows low or different fill 
		CNumericEntryDialog INumericEntryDialog;  
				
		//Set dialog box data titles and number value
		INumericEntryDialog.vEditString = "0";

		INumericEntryDialog.m_DialogTitleStaticText1 = "Enter the valve number you saw the low (or different)";
		INumericEntryDialog.m_DialogTitleStaticText2 = " filled container appear in";
		INumericEntryDialog.m_DialogTitleStaticText3 = "This will calculate the offset you should enter above";
		INumericEntryDialog.m_DialogTitleStaticText4 = "";

		INumericEntryDialog.m_UnitsString = "Filler has " + dtoa(vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType], 0) + " valves";
		INumericEntryDialog.vIntegerOnly = true;
		INumericEntryDialog.vAllowNegative = false;
		INumericEntryDialog.vAllowZero = false;
		INumericEntryDialog.vMaxValue = vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType];
		INumericEntryDialog.vMinValue = 1;
		//Pass control to dialog box and display
		int nResponse = INumericEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			int TempHeadEntered = _wtoi(INumericEntryDialog.vEditString);
			int TempOffsetTheyShouldEnter = vGlobalCurrentProduct->vHeadOffsetFromIndexPulse[0] + vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType] - TempHeadEntered + 1;

			if (TempOffsetTheyShouldEnter >= vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType])
				TempOffsetTheyShouldEnter = TempOffsetTheyShouldEnter - vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType];
			CNoticeDialog TempNoticeDialog;
			CString TempText = "\n\nYou should set the filler offset to: " + dtoa(TempOffsetTheyShouldEnter, 0) + "\nYou entered the observed different valve location: "  + dtoa(TempHeadEntered, 0);
			TempText = TempText + "\n was calculated by: Current Offset + Number Of valves - Observed valve + 1";
			TempNoticeDialog.vNoticeText = TempText;
			TempNoticeDialog.vType = cNoticeMessage;
			TempNoticeDialog.DoModal();

		}
		else 
		if (nResponse == 10)
		{
			//Main Menu Button pressed
			KillTimer(vUpdateDisplayTimerHandle);
			for (BYTE TempLoop = 0; TempLoop < cNumberOfFillerOffsetSpeeds; TempLoop++)
				vLocalSystemData->vFindFillerOffset[TempLoop] = false;
			CDialog::EndDialog(10);
		}
	}
	else
	if ((vLocalSystemData->vFindFillerOffset[0]) || (vLocalSystemData->vFindFillerOffset[1]) || (vLocalSystemData->vFindFillerOffset[3]))//if we were already trying to find half speed offset
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\nYou can only find one offset at a time, please wait until the Full Speed Filler Sampler Offset is found.";
		TempNoticeDialog.vType = cNoticeMessage;
		TempNoticeDialog.DoModal();
	}
	else
	if (vLocalSystemData->vFindFillerOffset[2])	//if we were already trying to find the half speed offset
	{
		vLocalSystemData->vFindFillerOffset[2] = 0;	//then turn it off
		UpdateDisplay();
	}
	else	//all ok, we want to find the filler half speed offset
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\nSet filler valve #1 to highest amount or\nput dense material in or around continers for valve #1\nThe software finds the heaviest container and\nadjusts the offset so it is valve #1.\nRequires at least 3 revolutions (2 to sync index pulse)";
		TempNoticeDialog.vType = cNoticeMessage;
		TempNoticeDialog.DoModal();

		vGlobalCurrentProduct->vEncoderRateIndexesForFillerSeamerSampling[2] = vLocalSystemData->vActualEncoderRate;
		vLocalSystemData->vFindFillerOffset[2] = AskIfContainerIsHighLowOrVariable();
		SetChangeMade();
		UpdateDisplay();
	}
}


void CProductFillerSeamerSampling::OnSubFunction4Button()
{
	if (!vLocalSystemData->vUseSamplingFIFO)
	if ((vLocalSystemData->vFindFillerOffset[0]) || (vLocalSystemData->vFindFillerOffset[1]) || (vLocalSystemData->vFindFillerOffset[2]))//if we were already trying to find half speed offset
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\nYou can only find one offset at a time, please wait until the Full Speed Filler Sampler Offset is found.";
		TempNoticeDialog.vType = cNoticeMessage;
		TempNoticeDialog.DoModal();
	}
	else
	if (vLocalSystemData->vFindFillerOffset[3])	//if we were already trying to find the half speed offset
	{
		vLocalSystemData->vFindFillerOffset[3] = 0;	//then turn it off
		UpdateDisplay();
	}
	else	//all ok, we want to find the filler half speed offset
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\nSet filler valve #1 to highest amount or\nput dense material in or around continers for valve #1\nThe software finds the heaviest container and\nadjusts the offset so it is valve #1.\nRequires at least 3 revolutions (2 to sync index pulse)";
		TempNoticeDialog.vType = cNoticeMessage;
		TempNoticeDialog.DoModal();

		vGlobalCurrentProduct->vEncoderRateIndexesForFillerSeamerSampling[3] = vLocalSystemData->vActualEncoderRate;
		vLocalSystemData->vFindFillerOffset[3] = AskIfContainerIsHighLowOrVariable();
		SetChangeMade();
		UpdateDisplay();
	}
}


void CProductFillerSeamerSampling::OnSubFunction5Button()
{
	if (vLocalSystemData->vUseSamplingFIFO)
		SetSeamerOffsets();
	else
		SetFillerOffsets();
}


void CProductFillerSeamerSampling::OnSubFunction6Button()
{
	if (vLocalSystemData->vUseSamplingFIFO)
	{ //calculate Seamer offset if person enters head that ejected if ejected head 1 
		CNumericEntryDialog INumericEntryDialog;  
				
		//Set dialog box data titles and number value
		INumericEntryDialog.vEditString = "0";

		INumericEntryDialog.m_DialogTitleStaticText1 = "Enter the Head Number that actually ejected when you ";
		INumericEntryDialog.m_DialogTitleStaticText2 = "sampled head 1";
		INumericEntryDialog.m_DialogTitleStaticText3 = "This will calculate the offset you should enter above";
		INumericEntryDialog.m_DialogTitleStaticText4 = "";

		INumericEntryDialog.m_UnitsString = "Seamer has " + dtoa(vLocalConfigurationData->vNumberOfHeadsToMonitor[cSeamerSamplingType], 0) + " heads";
		INumericEntryDialog.vIntegerOnly = true;
		INumericEntryDialog.vAllowNegative = false;
		INumericEntryDialog.vAllowZero = false;
		INumericEntryDialog.vMaxValue = vLocalConfigurationData->vNumberOfHeadsToMonitor[cSeamerSamplingType];
		INumericEntryDialog.vMinValue = 1;
		//Pass control to dialog box and display
		int nResponse = INumericEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			int TempHeadEntered = _wtoi(INumericEntryDialog.vEditString);
			//offset 4 is first one for seamer
			int TempOffsetTheyShouldEnter = vGlobalCurrentProduct->vHeadOffsetFromIndexPulse[4] + vLocalConfigurationData->vNumberOfHeadsToMonitor[cSeamerSamplingType] - TempHeadEntered + 1;

			if (TempOffsetTheyShouldEnter >= vLocalConfigurationData->vNumberOfHeadsToMonitor[cSeamerSamplingType])
				TempOffsetTheyShouldEnter = TempOffsetTheyShouldEnter - vLocalConfigurationData->vNumberOfHeadsToMonitor[cSeamerSamplingType];
			CNoticeDialog TempNoticeDialog;
			CString TempText = "\n\nYou should set the seamer offset to: " + dtoa(TempOffsetTheyShouldEnter, 0) + "\nYou entered the observed head ejected: "  + dtoa(TempHeadEntered, 0);
			TempText = TempText + "\n was calculated by: current offset + number of heads - observed head + 1";
			TempNoticeDialog.vNoticeText = TempText;
			TempNoticeDialog.vType = cNoticeMessage;
			TempNoticeDialog.DoModal();
		}
		else 
		if (nResponse == 10)
		{
			//Main Menu Button pressed
			KillTimer(vUpdateDisplayTimerHandle);
			for (BYTE TempLoop = 0; TempLoop < cNumberOfFillerOffsetSpeeds; TempLoop++)
				vLocalSystemData->vFindFillerOffset[TempLoop] = false;
			CDialog::EndDialog(10);
		}
	}
	else
		SetSeamerOffsets();
}


void CProductFillerSeamerSampling::OnSubFunction7Button()
{
	
}


void CProductFillerSeamerSampling::OnSubFunction8Button()
{
	if (vLocalSystemData->vUseSamplingFIFO)
	{
		CNumericEntryDialog INumericEntryDialog;  
				
		//Set dialog box data titles and number value
		INumericEntryDialog.vEditString = dtoa(vDistanceFromSeamerObjectToScanTracTrigger, 2);

		INumericEntryDialog.m_DialogTitleStaticText1 = "Enter timeout distance from Seamer Object to ScanTrac Trigger";
		INumericEntryDialog.m_DialogTitleStaticText2 = "System Setting, same value for all products";
		INumericEntryDialog.m_DialogTitleStaticText3 = "Filler Monitor Tracking FIFO cleared after this distance";
		INumericEntryDialog.m_DialogTitleStaticText4 = "";

		INumericEntryDialog.m_UnitsString = "Units in Inches, 0 = disabled";
		INumericEntryDialog.vIntegerOnly = false;
		INumericEntryDialog.vAllowNegative = false;
		INumericEntryDialog.vMaxValue = 480;
		INumericEntryDialog.vMinValue = 0;
		//Pass control to dialog box and display
		int nResponse = INumericEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			vDistanceFromSeamerObjectToScanTracTrigger = ATOF(INumericEntryDialog.vEditString);

			UpdateDisplay();
			SetChangeMade();
		}
		else 
		if (nResponse == 10)
		{
			//Main Menu Button pressed
			KillTimer(vUpdateDisplayTimerHandle);
			for (BYTE TempLoop = 0; TempLoop < cNumberOfFillerOffsetSpeeds; TempLoop++)
				vLocalSystemData->vFindFillerOffset[TempLoop] = false;
			CDialog::EndDialog(10);
		}
	}
}

void CProductFillerSeamerSampling::OnTimer(UINT_PTR nIDEvent)
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

void CProductFillerSeamerSampling::OneSecondDisplayUpdate()
{
	if (vOldFindFillerOffset[0] != vLocalSystemData->vFindFillerOffset[0])
	{
		vOldFindFillerOffset[0] = vLocalSystemData->vFindFillerOffset[0];
		if (!vOldFindFillerOffset[0])
		{
			//vHeadOffsetFromIndexPulse[cFillerSamplingType] = vGlobalCurrentProduct->vHeadOffsetFromIndexPulse[cFillerSamplingType];
			SetDlgItemText(IDC_Display3, "Range 1 Measured\n"
				+ dtoa( ((100 + ((double)vLocalConfigurationData->vEncoderRateTolerance))/100) * ((double) vGlobalCurrentProduct->vEncoderRateIndexesForFillerSeamerSampling[0]), 2 ) + " - "
				+ dtoa( ((100 - ((double)vLocalConfigurationData->vEncoderRateTolerance))/100) * ((double) vGlobalCurrentProduct->vEncoderRateIndexesForFillerSeamerSampling[0]), 2 ) + "Hz" );
		}
		UpdateDisplay();
	}
	if (vOldFindFillerOffset[1] != vLocalSystemData->vFindFillerOffset[1])
	{
		vOldFindFillerOffset[1] = vLocalSystemData->vFindFillerOffset[1];
		if (!vOldFindFillerOffset[1])
		{
			//vHeadOffsetFromIndexPulse[0] = vGlobalCurrentProduct->vHeadOffsetFromIndexPulse[0];
			SetDlgItemText(IDC_Display7, "Range 2 Measured\n"
				+ dtoa( ((100 + ((double)vLocalConfigurationData->vEncoderRateTolerance))/100) * ((double) vGlobalCurrentProduct->vEncoderRateIndexesForFillerSeamerSampling[1]), 2 ) + " - "
				+ dtoa( ((100 - ((double)vLocalConfigurationData->vEncoderRateTolerance))/100) * ((double) vGlobalCurrentProduct->vEncoderRateIndexesForFillerSeamerSampling[1]),2 ) + "Hz" );

		}
		UpdateDisplay();
	}
	if (vOldFindFillerOffset[2] != vLocalSystemData->vFindFillerOffset[2])
	{
		vOldFindFillerOffset[2] = vLocalSystemData->vFindFillerOffset[2];
		if (!vOldFindFillerOffset[2])
		{
			//vHeadOffsetFromIndexPulse[0] = vGlobalCurrentProduct->vHeadOffsetFromIndexPulse[0];
			SetDlgItemText(IDC_Display7, "Range 3 Measured\n"
				+ dtoa( ((100 + ((double)vLocalConfigurationData->vEncoderRateTolerance))/100) * ((double) vGlobalCurrentProduct->vEncoderRateIndexesForFillerSeamerSampling[2]), 2 ) + " - "
				+ dtoa( ((100 - ((double)vLocalConfigurationData->vEncoderRateTolerance))/100) * ((double) vGlobalCurrentProduct->vEncoderRateIndexesForFillerSeamerSampling[2]), 2 ) + "Hz" );
		}
		UpdateDisplay();
	}
	if (vOldFindFillerOffset[3] != vLocalSystemData->vFindFillerOffset[3])
	{
		vOldFindFillerOffset[3] = vLocalSystemData->vFindFillerOffset[3];
		if (!vOldFindFillerOffset[3])
		{
			//vHeadOffsetFromIndexPulse[0] = vGlobalCurrentProduct->vHeadOffsetFromIndexPulse[0];
			SetDlgItemText(IDC_Display7, "Range 4 Measured\n"
				+ dtoa( ((100 + ((double)vLocalConfigurationData->vEncoderRateTolerance))/100) * ((double) vGlobalCurrentProduct->vEncoderRateIndexesForFillerSeamerSampling[3]), 2 ) + " - "
				+ dtoa( ((100 - ((double)vLocalConfigurationData->vEncoderRateTolerance))/100) * ((double) vGlobalCurrentProduct->vEncoderRateIndexesForFillerSeamerSampling[3]),2 ) + "Hz" );
		}
		UpdateDisplay();
	}

	vTime =CTime::GetCurrentTime();
	int TempDateToday = vTime.GetDay();
	if (vDateToday != TempDateToday)
	{
		CString TempDateString = vTime.Format("%B %#d, %Y");
		CString TempChar = TempDateString.GetAt(0);
		TempChar.MakeUpper();
		TempDateString.SetAt(0,TempChar.GetAt(0));
		
		SetDlgItemText(IDC_Date,TempDateString);

		vDateToday=TempDateToday;
	}

	CString TempTimeString = " ";
	if (vLocalConfigurationData->vShow24HourTime)
		TempTimeString = vTime.Format("%H:%M:%S");//24 hour time format
	else
		TempTimeString = vTime.Format("%#I:%M:%S%p");//12 hour time format with am pm

	if (vOldCurrentEncoderRate != vLocalSystemData->vActualEncoderRate)
	{
		vOldCurrentEncoderRate = vLocalSystemData->vActualEncoderRate;
		SetDlgItemText(IDC_EncoderRate, dtoa(vOldCurrentEncoderRate, 0) + " Hz");
	}

	SetDlgItemText(IDC_Time2,TempTimeString);
}

void CProductFillerSeamerSampling::UpdateDisplay()
{
	SetDlgItemText(IDC_SubFunction1Display, dtoa(vGlobalCurrentProduct->vHeadOffsetFromIndexPulse[0], 0));
	SetDlgItemText(IDC_SubFunction2Display, dtoa(vGlobalCurrentProduct->vHeadOffsetFromIndexPulse[1], 0));	//half speed offset filler
	SetDlgItemText(IDC_SubFunction3Display, dtoa(vGlobalCurrentProduct->vHeadOffsetFromIndexPulse[2], 0));
	SetDlgItemText(IDC_SubFunction4Display, dtoa(vGlobalCurrentProduct->vHeadOffsetFromIndexPulse[3], 0));	//half speed offset filler
	SetDlgItemText(IDC_SubFunction5Display, dtoa(vGlobalCurrentProduct->vHeadOffsetFromIndexPulse[4], 0));
	SetDlgItemText(IDC_SubFunction6Display, dtoa(vGlobalCurrentProduct->vHeadOffsetFromIndexPulse[5], 0));	//half speed offset seamer
	SetDlgItemText(IDC_SubFunction7Display, dtoa(vGlobalCurrentProduct->vHeadOffsetFromIndexPulse[6], 0));
	SetDlgItemText(IDC_SubFunction8Display, dtoa(vGlobalCurrentProduct->vHeadOffsetFromIndexPulse[7], 0));	//half speed offset seamer
	
	SetDlgItemText(IDC_FrequencyRange1Display, dtoa(vGlobalCurrentProduct->vEncoderRateIndexesForFillerSeamerSampling[0])+"Hz");
	SetDlgItemText(IDC_FrequencyRange2Display, dtoa(vGlobalCurrentProduct->vEncoderRateIndexesForFillerSeamerSampling[1])+"Hz");
	SetDlgItemText(IDC_FrequencyRange3Display, dtoa(vGlobalCurrentProduct->vEncoderRateIndexesForFillerSeamerSampling[2])+"Hz");
	SetDlgItemText(IDC_FrequencyRange4Display, dtoa(vGlobalCurrentProduct->vEncoderRateIndexesForFillerSeamerSampling[3])+"Hz");

	SetDlgItemText(IDC_Function3Display1, dtoa(vLocalConfigurationData->vEncoderRateTolerance,0) + "%");

	if (vLocalSystemData->vUseSamplingFIFO)
	{
		SetDlgItemText(IDC_SubFunction8Display, dtoa(vDistanceFromSeamerObjectToScanTracTrigger, 2));

		if (vOldFindFillerOffset[0])
			SetDlgItemText(IDC_SubFunction1Button, _T("Stop Find\nFiller Offset"));
		else
			SetDlgItemText(IDC_SubFunction1Button, _T("Find\nFiller Offset"));
	}
	else
	{
		if (vOldFindFillerOffset[0])
			SetDlgItemText(IDC_SubFunction1Button, _T("Stop Find\nFiller Offset 1"));
		else
			SetDlgItemText(IDC_SubFunction1Button, _T("Find\nFiller Offset 1"));
		if (vOldFindFillerOffset[1])
			SetDlgItemText(IDC_SubFunction2Button, _T("Stop Find\nFiller Offset 2"));
		else
			SetDlgItemText(IDC_SubFunction2Button, _T("Find\nFiller Offset 2"));
		if (vOldFindFillerOffset[2])
			SetDlgItemText(IDC_SubFunction3Button, _T("Stop Find\nFiller Offset 3"));
		else
			SetDlgItemText(IDC_SubFunction3Button, _T("Find\nFiller Offset 3"));
		if (vOldFindFillerOffset[3])
			SetDlgItemText(IDC_SubFunction4Button, _T("Stop Find\nFiller Offset 4"));
		else
			SetDlgItemText(IDC_SubFunction4Button, _T("Find\nFiller Offset 4"));
	}

	SetDlgItemText(IDC_Display6, "Range 1\n"
							+ dtoa( ((100 - ((double)vLocalConfigurationData->vEncoderRateTolerance))/100) * ((double) vGlobalCurrentProduct->vEncoderRateIndexesForFillerSeamerSampling[0]), 0 ) + " - "
							+ dtoa( ((100 + ((double)vLocalConfigurationData->vEncoderRateTolerance))/100) * ((double) vGlobalCurrentProduct->vEncoderRateIndexesForFillerSeamerSampling[0]), 0 ) + "Hz");
	SetDlgItemText(IDC_Display7, "Range 2\n"
							+ dtoa( ((100 - ((double)vLocalConfigurationData->vEncoderRateTolerance))/100) * ((double) vGlobalCurrentProduct->vEncoderRateIndexesForFillerSeamerSampling[1]), 0) + " - "
							+ dtoa( ((100 + ((double)vLocalConfigurationData->vEncoderRateTolerance))/100) * ((double) vGlobalCurrentProduct->vEncoderRateIndexesForFillerSeamerSampling[1]), 0) + "Hz" );
	SetDlgItemText(IDC_Display4, "Range 3\n"
							+ dtoa( ((100 - ((double)vLocalConfigurationData->vEncoderRateTolerance))/100) * ((double) vGlobalCurrentProduct->vEncoderRateIndexesForFillerSeamerSampling[2]), 0) + " - "
							+ dtoa( ((100 + ((double)vLocalConfigurationData->vEncoderRateTolerance))/100) * ((double) vGlobalCurrentProduct->vEncoderRateIndexesForFillerSeamerSampling[2]), 0) + "Hz" );
	SetDlgItemText(IDC_Display5, "Range 4\n"
							+ dtoa( ((100 - ((double)vLocalConfigurationData->vEncoderRateTolerance))/100) * ((double) vGlobalCurrentProduct->vEncoderRateIndexesForFillerSeamerSampling[3]), 0) + " - "
							+ dtoa( ((100 + ((double)vLocalConfigurationData->vEncoderRateTolerance))/100) * ((double) vGlobalCurrentProduct->vEncoderRateIndexesForFillerSeamerSampling[3]), 0) + "Hz" );

}

void CProductFillerSeamerSampling::SetChangeMade() 
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

// ProductFillerSeamerSampling message handlers
HBRUSH CProductFillerSeamerSampling::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	HWND TempDialogHwnd = this->m_hWnd;
	int TextSize;
	pDC->SetAttribDC(pDC->m_hDC);

	if (pWnd->GetDlgCtrlID() == IDC_DialogTitleStaticText1)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DialogTitleStaticText, 8);
		pDC->SetTextColor(vGlobalDialogTitleColor);
	}

	if (pWnd->GetDlgCtrlID() == IDC_Background)
	{
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction1DisplayAbove) 
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeMultilineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction1DisplayAbove, 5);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction5DisplayAbove)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeMultilineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction5DisplayAbove, 5);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Display1)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Display1, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Display3)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Display3, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Display4)
	{
		TextSize = SetTextSizeMultilineDisplay(TempDialogHwnd, pWnd, pDC, &m_Display4, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Display5)
	{
		TextSize = SetTextSizeMultilineDisplay(TempDialogHwnd, pWnd, pDC, &m_Display5, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Display6)
	{
		TextSize = SetTextSizeMultilineDisplay(TempDialogHwnd, pWnd, pDC, &m_Display6, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Display7)
	{
		TextSize = SetTextSizeMultilineDisplay(TempDialogHwnd, pWnd, pDC, &m_Display7, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_FrequencyRange1Display)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_FrequencyRange1Display, 5);
	if (pWnd->GetDlgCtrlID() == IDC_FrequencyRange2Display)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_FrequencyRange1Display, 5);
	if (pWnd->GetDlgCtrlID() == IDC_FrequencyRange3Display)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_FrequencyRange1Display, 5);
	if (pWnd->GetDlgCtrlID() == IDC_FrequencyRange4Display)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_FrequencyRange1Display, 5);
	if (pWnd->GetDlgCtrlID() == IDC_Time2)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Time2, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_EncoderRate)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_EncoderRate, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_EncoderRateLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_EncoderRateLabel, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Date)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Date, 5);
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
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction1Button, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		if (vLocalSystemData->vFindFillerOffset[0])
		{
			pDC->SetTextColor(cWhite);
			return vLocalSystemData->vRedBrush;
		}
		else if ((vLocalSystemData->vFindFillerOffset[1]) || (vLocalSystemData->vFindFillerOffset[2])
			|| (vLocalSystemData->vFindFillerOffset[3]))
		{
			pDC->SetTextColor(cGray);
			return vGlobalButtonColorBrush;
		}
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction2Button)
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction2Button, 5);  
		pDC->SetBkMode(TRANSPARENT);
		if (vLocalSystemData->vFindFillerOffset[1])
		{
			pDC->SetTextColor(cWhite);
			return vLocalSystemData->vRedBrush;
		}
		else if ((vLocalSystemData->vFindFillerOffset[0]) || (vLocalSystemData->vFindFillerOffset[2])
			|| (vLocalSystemData->vFindFillerOffset[3]))
		{
			pDC->SetTextColor(cGray);
			return vGlobalButtonColorBrush;
		}
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction3Button)
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction3Button, 5);  //5 is medium large
		if (vLocalSystemData->vFindFillerOffset[2])
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cWhite);
			return vLocalSystemData->vRedBrush;
		}
		else if ((vLocalSystemData->vFindFillerOffset[0]) || (vLocalSystemData->vFindFillerOffset[1])
			|| (vLocalSystemData->vFindFillerOffset[3]))
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cGray);
			return vGlobalButtonColorBrush;
		}
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction4Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction4Button, 5);  //5 is medium large
		if (vLocalSystemData->vFindFillerOffset[3])
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cWhite);
			return vLocalSystemData->vRedBrush;
		}
		else if ((vLocalSystemData->vFindFillerOffset[0]) || (vLocalSystemData->vFindFillerOffset[1])
			|| (vLocalSystemData->vFindFillerOffset[2]))
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
	if (pWnd->GetDlgCtrlID() == IDC_Function3Display1)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Function3Display1, 5);

	return hbr;
}

void CProductFillerSeamerSampling::OnDialogTitleStaticText1()
{
	BYTE TempHiddenKeys = 8;
	if (!PasswordOK(cTemporaryInspxPassword,false))
		TempHiddenKeys = 10;
	PostThreadMessage(vGlobalCallingThreadID, cDisplaySoftKeyMenu, TempHiddenKeys, TempHiddenKeys);
}


BOOL CProductFillerSeamerSampling::PreTranslateMessage(MSG* pMsg)
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
				PostThreadMessage(vGlobalCallingThreadID,
					cShowHelp, 6, 0);
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

void CProductFillerSeamerSampling::ShowHideInspxButtons(bool vShowInspxButtons)
{
	if (vShowInspxButtons)
	{
		m_Function3Button.ShowWindow(SW_SHOW);
		m_Function3Display1.ShowWindow(SW_SHOW);

		if (vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType] != 0)
		{
			m_Function5Button.ShowWindow(SW_SHOW);
		}
	}
	else //if Hide Inspx Buttons
	{
		m_Function3Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_Function3Button.GetRectToInvalidateArrow());
		m_Function3Display1.ShowWindow(SW_HIDE);

		if (vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType] != 0) //Do not hide if only seamer setup and not filler setup
		{
			m_Function5Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_Function5Button.GetRectToInvalidateArrow());
		}
	}
}

BYTE CProductFillerSeamerSampling::AskIfContainerIsHighLowOrVariable()
{
	BYTE TempReturn = 0;
	CSelectItemDialog ISelectItemDialog;

	//Set dialog box data titles and number value
	ISelectItemDialog.vTitleString = "Select Container Type";
	ISelectItemDialog.vNumberOfItems = 4;
	ISelectItemDialog.vBackgroundColor = cGreen;

	ISelectItemDialog.vItems[0] = "Cancel";
	ISelectItemDialog.vItems[1] = "Highest Weight";
	ISelectItemDialog.vItems[2] = "Lowest Weight";
	ISelectItemDialog.vItems[3] = "Most Variable Weight";

	ISelectItemDialog.m_DialogTitleStaticText2 = "Best to set valve 1 to over fill to make Highest Weight,";
	ISelectItemDialog.m_DialogTitleStaticText3 = "if can't, set valve 1 so lowest weight or variable";

	//Pass control to dialog box and display
	int nResponse = ISelectItemDialog.DoModal();
	//dialog box is now closed, if user pressed select do this
	//if user pressed cancel, do nothing
	if (nResponse == IDOK)
		TempReturn = ISelectItemDialog.vItemNumberSelected - 1;

	return TempReturn;
}

void CProductFillerSeamerSampling::SetFillerOffsets()
{
	CNumericEntryDialog INumericEntryDialog;  
				
	//Set dialog box data titles and number value
	INumericEntryDialog.vEditString = dtoa(vGlobalCurrentProduct->vHeadOffsetFromIndexPulse[0], 0);

	INumericEntryDialog.m_DialogTitleStaticText1 = "Enter " + vLocalSystemData->vSamplingName[cFillerSamplingType] + " Index 1 Pulse Offset To First Valve";
	INumericEntryDialog.m_DialogTitleStaticText2 = "Product Setting; effects " + vGlobalCurrentProduct->vProductName;
	INumericEntryDialog.m_DialogTitleStaticText3 = "Example: Valve 1 appears in 5 enter -4";
	INumericEntryDialog.m_DialogTitleStaticText4 = "Example: Valve 1 appears in 28 of 30 enter 3";

	INumericEntryDialog.m_UnitsString = "Valve 1 should appear in first position";
	INumericEntryDialog.vIntegerOnly = true;
	INumericEntryDialog.vAllowNegative = true;
	INumericEntryDialog.vMaxValue = vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType] - 1;
	INumericEntryDialog.vMinValue = 0;
	//Pass control to dialog box and display
	int nResponse = INumericEntryDialog.DoModal();
	//dialog box is now closed, if user pressed select do this
	//if user pressed cancel, do nothing
	if (nResponse == IDOK)
	{
		int TempInt = _wtoi(INumericEntryDialog.vEditString);
		if (TempInt < 0)
			vGlobalCurrentProduct->vHeadOffsetFromIndexPulse[0] = BYTE(TempInt + vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType]);
		else
			vGlobalCurrentProduct->vHeadOffsetFromIndexPulse[0] = BYTE(TempInt);
		
		if (!vLocalSystemData->vUseSamplingFIFO)
		{
			//Set dialog box data titles and number value
			INumericEntryDialog.vEditString = dtoa(vGlobalCurrentProduct->vHeadOffsetFromIndexPulse[1], 0);

			INumericEntryDialog.m_DialogTitleStaticText1 = "Enter " + vLocalSystemData->vSamplingName[cFillerSamplingType] + " Index 2 Pulse Offset To First Valve";
			INumericEntryDialog.m_DialogTitleStaticText2 = "Product Setting; effects " + vGlobalCurrentProduct->vProductName;
			INumericEntryDialog.m_DialogTitleStaticText3 = "Example: Valve 1 appears in 5 enter -4";
			INumericEntryDialog.m_DialogTitleStaticText4 = "Example: Valve 1 appears in 28 of 30 enter 3";

			INumericEntryDialog.m_UnitsString = "Valve 1 should appear in first position";
			INumericEntryDialog.vIntegerOnly = true;
			INumericEntryDialog.vAllowNegative = true;
			INumericEntryDialog.vMaxValue = vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType] - 1;
			INumericEntryDialog.vMinValue = 0;
			//Pass control to dialog box and display
			int nResponse = INumericEntryDialog.DoModal();
			//dialog box is now closed, if user pressed select do this
			//if user pressed cancel, do nothing
			if (nResponse == IDOK)
			{
				int TempInt = _wtoi(INumericEntryDialog.vEditString);
				if (TempInt < 0)
					vGlobalCurrentProduct->vHeadOffsetFromIndexPulse[1] = BYTE(TempInt + vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType]);
				else
					vGlobalCurrentProduct->vHeadOffsetFromIndexPulse[1] = BYTE(TempInt);

				//Set dialog box data titles and number value
				INumericEntryDialog.vEditString = dtoa(vGlobalCurrentProduct->vHeadOffsetFromIndexPulse[2], 0);

				INumericEntryDialog.m_DialogTitleStaticText1 = "Enter " + vLocalSystemData->vSamplingName[cFillerSamplingType] + " Index 3 Pulse Offset To First Valve";
				INumericEntryDialog.m_DialogTitleStaticText2 = "Product Setting; effects " + vGlobalCurrentProduct->vProductName;
				INumericEntryDialog.m_DialogTitleStaticText3 = "Example: Valve 1 appears in 5 enter -4";
				INumericEntryDialog.m_DialogTitleStaticText4 = "Example: Valve 1 appears in 28 of 30 enter 3";

				INumericEntryDialog.m_UnitsString = "Valve 1 should appear in first position";
				INumericEntryDialog.vIntegerOnly = true;
				INumericEntryDialog.vAllowNegative = true;
				INumericEntryDialog.vMaxValue = vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType] - 1;
				INumericEntryDialog.vMinValue = 0;
				//Pass control to dialog box and display
				int nResponse = INumericEntryDialog.DoModal();
				//dialog box is now closed, if user pressed select do this
				//if user pressed cancel, do nothing
				if (nResponse == IDOK)
				{
					int TempInt = _wtoi(INumericEntryDialog.vEditString);
					if (TempInt < 0)
						vGlobalCurrentProduct->vHeadOffsetFromIndexPulse[2] = BYTE(TempInt + vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType]);
					else
						vGlobalCurrentProduct->vHeadOffsetFromIndexPulse[2] = BYTE(TempInt);

					//Set dialog box data titles and number value
					INumericEntryDialog.vEditString = dtoa(vGlobalCurrentProduct->vHeadOffsetFromIndexPulse[3], 0);

					INumericEntryDialog.m_DialogTitleStaticText1 = "Enter " + vLocalSystemData->vSamplingName[cFillerSamplingType] + " Index 4 Pulse Offset To First Valve";
					INumericEntryDialog.m_DialogTitleStaticText2 = "Product Setting; effects " + vGlobalCurrentProduct->vProductName;
					INumericEntryDialog.m_DialogTitleStaticText3 = "Example: Valve 1 appears in 5 enter -4";
					INumericEntryDialog.m_DialogTitleStaticText4 = "Example: Valve 1 appears in 28 of 30 enter 3";

					INumericEntryDialog.m_UnitsString = "Valve 1 should appear in first position";
					INumericEntryDialog.vIntegerOnly = true;
					INumericEntryDialog.vAllowNegative = true;
					INumericEntryDialog.vMaxValue = vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType] - 1;
					INumericEntryDialog.vMinValue = 0;
					//Pass control to dialog box and display
					int nResponse = INumericEntryDialog.DoModal();
					//dialog box is now closed, if user pressed select do this
					//if user pressed cancel, do nothing
					if (nResponse == IDOK)
					{
						int TempInt = _wtoi(INumericEntryDialog.vEditString);
						if (TempInt < 0)
							vGlobalCurrentProduct->vHeadOffsetFromIndexPulse[3] = BYTE(TempInt + vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType]);
						else
							vGlobalCurrentProduct->vHeadOffsetFromIndexPulse[3] = BYTE(TempInt);

						SetDlgItemText(IDC_Display4, "Range 4 Set To\n"
							+ dtoa( ((100 - ((double)vLocalConfigurationData->vEncoderRateTolerance))/100) * ((double) vGlobalCurrentProduct->vEncoderRateIndexesForFillerSeamerSampling[3]), 0) + " - "
							+ dtoa( ((100 + ((double)vLocalConfigurationData->vEncoderRateTolerance))/100) * ((double) vGlobalCurrentProduct->vEncoderRateIndexesForFillerSeamerSampling[3]), 0) + "Hz" );


					}
					else 
					if (nResponse == 10)
					{
						//Main Menu Button pressed
						KillTimer(vUpdateDisplayTimerHandle);
						for (BYTE TempLoop = 0; TempLoop < cNumberOfFillerOffsetSpeeds; TempLoop++)
							vLocalSystemData->vFindFillerOffset[TempLoop] = false;
						CDialog::EndDialog(10);
					}
					SetDlgItemText(IDC_Display5, "Range 3 Set To\n"
						+ dtoa( ((100 - ((double)vLocalConfigurationData->vEncoderRateTolerance))/100) * ((double) vGlobalCurrentProduct->vEncoderRateIndexesForFillerSeamerSampling[2]), 0) + " - "
						+ dtoa( ((100 + ((double)vLocalConfigurationData->vEncoderRateTolerance))/100) * ((double) vGlobalCurrentProduct->vEncoderRateIndexesForFillerSeamerSampling[2]), 0) + "Hz" );
				}
				else 
				if (nResponse == 10)
				{
					//Main Menu Button pressed
					KillTimer(vUpdateDisplayTimerHandle);
					CDialog::EndDialog(10);
				}
				SetDlgItemText(IDC_Display7, "Range 2 Set To\n"
					+ dtoa( ((100 - ((double)vLocalConfigurationData->vEncoderRateTolerance))/100) * ((double) vGlobalCurrentProduct->vEncoderRateIndexesForFillerSeamerSampling[1]), 0) + " - "
					+ dtoa( ((100 + ((double)vLocalConfigurationData->vEncoderRateTolerance))/100) * ((double) vGlobalCurrentProduct->vEncoderRateIndexesForFillerSeamerSampling[1]), 0) + "Hz" );

			}
			else 
			if (nResponse == 10)
			{
				//Main Menu Button pressed
				KillTimer(vUpdateDisplayTimerHandle);
				for (BYTE TempLoop = 0; TempLoop < cNumberOfFillerOffsetSpeeds; TempLoop++)
					vLocalSystemData->vFindFillerOffset[TempLoop] = false;
				CDialog::EndDialog(10);
			}
			
			SetDlgItemText(IDC_Display6, "Range 1 Set To\n"
				+ dtoa( ((100 - ((double)vLocalConfigurationData->vEncoderRateTolerance))/100) * ((double) vGlobalCurrentProduct->vEncoderRateIndexesForFillerSeamerSampling[0]), 0) + " - "
				+ dtoa( ((100 + ((double)vLocalConfigurationData->vEncoderRateTolerance))/100) * ((double) vGlobalCurrentProduct->vEncoderRateIndexesForFillerSeamerSampling[0]), 0) + "Hz" );
		}

		UpdateDisplay();
		SetChangeMade();
	}
	else 
	if (nResponse == 10)
	{
		//Main Menu Button pressed
		KillTimer(vUpdateDisplayTimerHandle);
		for (BYTE TempLoop = 0; TempLoop < cNumberOfFillerOffsetSpeeds; TempLoop++)
			vLocalSystemData->vFindFillerOffset[TempLoop] = false;
		CDialog::EndDialog(10);
	}
}

void CProductFillerSeamerSampling::SetSeamerOffsets()
{
	CNumericEntryDialog INumericEntryDialog;  
				
	//Set dialog box data titles and number value
	INumericEntryDialog.vEditString = dtoa(vGlobalCurrentProduct->vHeadOffsetFromIndexPulse[4], 0);

	INumericEntryDialog.m_DialogTitleStaticText1 = "Enter Seamer Index 1 Pulse Offset To First Head";
	INumericEntryDialog.m_DialogTitleStaticText2 = "Product Settting; effects all " + vGlobalCurrentProduct->vProductName;
	INumericEntryDialog.m_DialogTitleStaticText3 = "Example: Head 1 appears in 5 enter -4";
	INumericEntryDialog.m_DialogTitleStaticText4 = "Example: Head 1 appears in 28 of 30 enter 3";

	INumericEntryDialog.m_UnitsString = "Head 1 should appear in first position";
	INumericEntryDialog.vIntegerOnly = true;
	INumericEntryDialog.vAllowNegative = true;
	INumericEntryDialog.vMaxValue = vLocalConfigurationData->vNumberOfHeadsToMonitor[cSeamerSamplingType] - 1;
	INumericEntryDialog.vMinValue = -vLocalConfigurationData->vNumberOfHeadsToMonitor[cSeamerSamplingType] - 1;
	//Pass control to dialog box and display
	int nResponse = INumericEntryDialog.DoModal();
	//dialog box is now closed, if user pressed select do this
	//if user pressed cancel, do nothing
	if (nResponse == IDOK)
	{
		int TempInt = _wtoi(INumericEntryDialog.vEditString);
		if (TempInt < 0)
			vGlobalCurrentProduct->vHeadOffsetFromIndexPulse[4] = BYTE(TempInt + vLocalConfigurationData->vNumberOfHeadsToMonitor[cSeamerSamplingType]);
		else
			vGlobalCurrentProduct->vHeadOffsetFromIndexPulse[4] = BYTE(TempInt);
				//Set dialog box data titles and number value
		
		UpdateDisplay();
		SetChangeMade();

		if (!vLocalSystemData->vUseSamplingFIFO)
		{
			CNumericEntryDialog INumericEntryDialog;
			INumericEntryDialog.vEditString = dtoa(vGlobalCurrentProduct->vHeadOffsetFromIndexPulse[5], 0);

			INumericEntryDialog.m_DialogTitleStaticText1 = "Enter Seamer Index 2 Pulse Offset To First Head";
			INumericEntryDialog.m_DialogTitleStaticText2 = "Product Settting; effects " + vGlobalCurrentProduct->vProductName;
			INumericEntryDialog.m_DialogTitleStaticText3 = "Example: Head 1 appears in 5 enter -4";
			INumericEntryDialog.m_DialogTitleStaticText4 = "Example: Head 1 appears in 28 of 30 enter 3";

			INumericEntryDialog.m_UnitsString = "Head 1 should appear in first position";
			INumericEntryDialog.vIntegerOnly = true;
			INumericEntryDialog.vAllowNegative = true;
			INumericEntryDialog.vMaxValue = vLocalConfigurationData->vNumberOfHeadsToMonitor[cSeamerSamplingType];
			INumericEntryDialog.vMinValue = 0;
			//Pass control to dialog box and display
			int nResponse = INumericEntryDialog.DoModal();
			//dialog box is now closed, if user pressed select do this
			//if user pressed cancel, do nothing
			if (nResponse == IDOK)
			{
				int TempInt = _wtoi(INumericEntryDialog.vEditString);
				if (TempInt < 0)
					vGlobalCurrentProduct->vHeadOffsetFromIndexPulse[5] = BYTE(TempInt + vLocalConfigurationData->vNumberOfHeadsToMonitor[cSeamerSamplingType]);
				else
					vGlobalCurrentProduct->vHeadOffsetFromIndexPulse[5] = BYTE(TempInt);

				CNumericEntryDialog INumericEntryDialog;
				INumericEntryDialog.vEditString = dtoa(vGlobalCurrentProduct->vHeadOffsetFromIndexPulse[6], 0);

				INumericEntryDialog.m_DialogTitleStaticText1 = "Enter Seamer Index 3 Pulse Offset To First Head";
				INumericEntryDialog.m_DialogTitleStaticText2 = "Product Settting; effects " + vGlobalCurrentProduct->vProductName;
				INumericEntryDialog.m_DialogTitleStaticText3 = "Example: Head 1 appears in 5 enter -4";
				INumericEntryDialog.m_DialogTitleStaticText4 = "Example: Head 1 appears in 28 of 30 enter 3";

				INumericEntryDialog.m_UnitsString = "Head 1 should appear in first position";
				INumericEntryDialog.vIntegerOnly = true;
				INumericEntryDialog.vAllowNegative = true;
				INumericEntryDialog.vMaxValue = vLocalConfigurationData->vNumberOfHeadsToMonitor[cSeamerSamplingType];
				INumericEntryDialog.vMinValue = 0;
				//Pass control to dialog box and display
				int nResponse = INumericEntryDialog.DoModal();
				//dialog box is now closed, if user pressed select do this
				//if user pressed cancel, do nothing
				if (nResponse == IDOK)
				{
					int TempInt = _wtoi(INumericEntryDialog.vEditString);
					if (TempInt < 0)
						vGlobalCurrentProduct->vHeadOffsetFromIndexPulse[6] = BYTE(TempInt + vLocalConfigurationData->vNumberOfHeadsToMonitor[cSeamerSamplingType]);
					else
						vGlobalCurrentProduct->vHeadOffsetFromIndexPulse[6] = BYTE(TempInt);

					INumericEntryDialog.vEditString = dtoa(vGlobalCurrentProduct->vHeadOffsetFromIndexPulse[7], 0);

					INumericEntryDialog.m_DialogTitleStaticText1 = "Enter Seamer Index 4 Pulse Offset To First Head";
					INumericEntryDialog.m_DialogTitleStaticText2 = "Product Settting; effects " + vGlobalCurrentProduct->vProductName;
					INumericEntryDialog.m_DialogTitleStaticText3 = "Example: Head 1 appears in 5 enter -4";
					INumericEntryDialog.m_DialogTitleStaticText4 = "Example: Head 1 appears in 28 of 30 enter 3";

					INumericEntryDialog.m_UnitsString = "Head 1 should appear in first position";
					INumericEntryDialog.vIntegerOnly = true;
					INumericEntryDialog.vAllowNegative = true;
					INumericEntryDialog.vMaxValue = vLocalConfigurationData->vNumberOfHeadsToMonitor[cSeamerSamplingType];
					INumericEntryDialog.vMinValue = 0;
					//Pass control to dialog box and display
					int nResponse = INumericEntryDialog.DoModal();
					//dialog box is now closed, if user pressed select do this
					//if user pressed cancel, do nothing
					if (nResponse == IDOK)
					{
						int TempInt = _wtoi(INumericEntryDialog.vEditString);
						if (TempInt < 0)
							vGlobalCurrentProduct->vHeadOffsetFromIndexPulse[7] = BYTE(TempInt + vLocalConfigurationData->vNumberOfHeadsToMonitor[cSeamerSamplingType]);
						else
							vGlobalCurrentProduct->vHeadOffsetFromIndexPulse[7] = BYTE(TempInt);

						UpdateDisplay();
						SetChangeMade();
					}
					else 
					if (nResponse == 10)
					{
						//Main Menu Button pressed
						KillTimer(vUpdateDisplayTimerHandle);
						for (BYTE TempLoop = 0; TempLoop < cNumberOfFillerOffsetSpeeds; TempLoop++)
							vLocalSystemData->vFindFillerOffset[TempLoop] = false;
						CDialog::EndDialog(10);
					}
				}
				else 
				if (nResponse == 10)
				{
					//Main Menu Button pressed
					KillTimer(vUpdateDisplayTimerHandle);
					for (BYTE TempLoop = 0; TempLoop < cNumberOfFillerOffsetSpeeds; TempLoop++)
						vLocalSystemData->vFindFillerOffset[TempLoop] = false;
					CDialog::EndDialog(10);
				}
			}
			else 
			if (nResponse == 10)
			{
				//Main Menu Button pressed
				KillTimer(vUpdateDisplayTimerHandle);
				for (BYTE TempLoop = 0; TempLoop < cNumberOfFillerOffsetSpeeds; TempLoop++)
					vLocalSystemData->vFindFillerOffset[TempLoop] = false;
				CDialog::EndDialog(10);
			}
		}
	}
	else 
	if (nResponse == 10)
	{
		//Main Menu Button pressed
		KillTimer(vUpdateDisplayTimerHandle);
		for (BYTE TempLoop = 0; TempLoop < cNumberOfFillerOffsetSpeeds; TempLoop++)
			vLocalSystemData->vFindFillerOffset[TempLoop] = false;
		CDialog::EndDialog(10);
	}
}
