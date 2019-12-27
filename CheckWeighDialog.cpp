// CheckWeighDialog.cpp : implementation file
//

#include "stdafx.h"
#include "math.h"  //for sqrt
#include "scantrac.h"
#include "CheckWeighDialog.h"
#include "NumericEntryDialog.h"
#include "NoticeDialog.h"
#include "HistogramDisplayDialog.h"
#include "PasswordEntryDialog.h"
#include "CheckWeighOffsetDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CProduct *vGlobalCurrentProduct;
extern unsigned long vGlobalDialogTitleColor;
extern BYTE vGlobalShiftKeyDown;
extern int vGlobalPasswordLevel;
extern CString vGlobalCurrentUser;
/////////////////////////////////////////////////////////////////////////////
// CCheckWeighDialog dialog


CCheckWeighDialog::CCheckWeighDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CCheckWeighDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCheckWeighDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	vBlankLeft = 0;
	vBlankWidth = 640;
	vBlankTop = 42;
	vBlankHeight = 275;
	vGraphLeft = 75;
	vGraphWidth = 520;
	vGraphRight = vGraphLeft + vGraphWidth;
	vGraphTop = 46;
	vGraphHeight = 240;
	vDrawHalfTicks = false;
	vOldPasswordLevel = vGlobalPasswordLevel;
	vLastDisplayedIndex = 0xFFFF;
	vLastDisplayedTime = 0;
	vLastDisplayedTimeOld = 0;

	vGrowingSoDontBlank = true;
	vPlotMinimumY = 32000;
	vPlotMaximumY = 0;
	vPlotMinimumX = 32000;
	vPlotMaximumX = 0;
	vOldNumberOfPointsToPlot = 0;
	vPointerToAdjustedData = NULL;
	vNumberOfPointsToPlot = 0;
	vDisplayDataPointer = NULL;
	vDisplayIndex = 0;
	vDisplayThresholdLower = 0;
	vDisplayThresholdUpper = 0;
	vDisplayNumberOfPointsToPlot = 0;
	vDivideFactor = 0;
	vOldDivideFactor = 0;

	vRecentUnderOld = 0;
	vLastWeightOld = 0;
	vTotalUnderOld = 0;
	vTotalOverOld = 0;

	vCheckWeighDataIndexOld = -1;
	vCheckWeighTrendDataIndexOld = -1;
	vCheckWeighTrendDataIndexOnMinutesOld = -1;
	vRecentOverOld = 0;
	vTotalUnderOld = 0;
	vTotalOverOld = 0;
	vCheckWeighDataSizeOld = 0;
	vRunStartTimeOld = "--";
	vNumberOfAlarmsOld = 0;
	vDisplayAverage = 0;
	vDisplayAverageOld = 0;
	vDisplayStandardDeviation = 0;
	vDisplayStandardDeviationOld = 0;

	vOldTotalContainers = 0;
	vOldCheckWeighTrendDataIndex = -5;
	vOldCheckWeighDataIndex = -5;
	vUpdateCountDown = 100;

	vMinimumValue = 0xffffffffffff;
	vMaximumValue = 0;
	vOneSecondTimerHandle = 55;
	vCheckWeighInspectionNumber = 0;
	vCheckWeighInspection = NULL;
	vMainWindowPointer = NULL;
	vLocalSystemData = NULL;
	vLocalConfigurationData = NULL;
	vOldDataIndex = -2;

	vPlotBackGroundColor = cVeryLightGray;
	vPlotBackGroundPen.CreatePen(PS_SOLID, 1, cVeryLightGray);
	vVeryLightBluePen.CreatePen(PS_SOLID, 1, cLightGray);
	vRedPen.CreatePen(PS_SOLID, 1, cRed);
	vBluePen.CreatePen(PS_SOLID, 1, cBlue);
	vDarkBluePen.CreatePen(PS_SOLID, 1, cDarkBlue);

	vPlotDisplay = cTrendDisplay;
	vPlotDisplayOld = cTrendMinuteDisplay; //make other so causes it to update right away
	vLocalCWndCollection.Add(this					 );
	vLocalCWndCollection.Add(&m_AlarmIndicator		 );
	vLocalCWndCollection.Add(&m_Function4Button		 );
	vLocalCWndCollection.Add(&m_LastDisplayedLabel	 );
	vLocalCWndCollection.Add(&m_LastDisplayed		 );
	vLocalCWndCollection.Add(&m_TotalOver			 );
	vLocalCWndCollection.Add(&m_TotalOverLabel		 );
	vLocalCWndCollection.Add(&m_TotalUnder			 );
	vLocalCWndCollection.Add(&m_TotalUnderLabel		 );
	vLocalCWndCollection.Add(&m_Function3Button		 );
	vLocalCWndCollection.Add(&m_Function5Button		 );
	vLocalCWndCollection.Add(&m_Count				 );
	vLocalCWndCollection.Add(&m_CountLabel			 );
	vLocalCWndCollection.Add(&m_Function2Button		 );
	vLocalCWndCollection.Add(&m_LastLabel			 );
	vLocalCWndCollection.Add(&m_MaximumTrendThreshold);
	vLocalCWndCollection.Add(&m_MinimumTrendThreshold);
	vLocalCWndCollection.Add(&m_ConversionFormula	 );
	vLocalCWndCollection.Add(&m_XScale1				 );
	vLocalCWndCollection.Add(&m_XScale10			 );
	vLocalCWndCollection.Add(&m_XScale11			 );
	vLocalCWndCollection.Add(&m_XScale12			 );
	vLocalCWndCollection.Add(&m_XScale13			 );
	vLocalCWndCollection.Add(&m_XScale14			 );
	vLocalCWndCollection.Add(&m_XScale2				 );
	vLocalCWndCollection.Add(&m_XScale3				 );
	vLocalCWndCollection.Add(&m_XScale4				 );
	vLocalCWndCollection.Add(&m_XScale5				 );
	vLocalCWndCollection.Add(&m_XScale6				 );
	vLocalCWndCollection.Add(&m_XScale7				 );
	vLocalCWndCollection.Add(&m_XScale8				 );
	vLocalCWndCollection.Add(&m_XScale9				 );
	vLocalCWndCollection.Add(&m_YScale1				 );
	vLocalCWndCollection.Add(&m_YScale10			 );
	vLocalCWndCollection.Add(&m_YScale11			 );
	vLocalCWndCollection.Add(&m_YScale12			 );
	vLocalCWndCollection.Add(&m_YScale13			 );
	vLocalCWndCollection.Add(&m_YScale14			 );
	vLocalCWndCollection.Add(&m_YScale15			 );
	vLocalCWndCollection.Add(&m_YScale2				 );
	vLocalCWndCollection.Add(&m_YScale3				 );
	vLocalCWndCollection.Add(&m_YScale4				 );
	vLocalCWndCollection.Add(&m_YScale5				 );
	vLocalCWndCollection.Add(&m_YScale6				 );
	vLocalCWndCollection.Add(&m_YScale7				 );
	vLocalCWndCollection.Add(&m_YScale8				 );
	vLocalCWndCollection.Add(&m_YScale9				 );
	vLocalCWndCollection.Add(&m_XLabel3				 );
	vLocalCWndCollection.Add(&m_NumberOfAlarms		 );
	vLocalCWndCollection.Add(&m_NumberOfAlarmsLabel	 );
	vLocalCWndCollection.Add(&m_Average				 );
	vLocalCWndCollection.Add(&m_AverageLabel		 );
	vLocalCWndCollection.Add(&m_DialogTitleStaticText);
	vLocalCWndCollection.Add(&m_Function1Button		 );
	vLocalCWndCollection.Add(&m_Nominal				 );
	vLocalCWndCollection.Add(&m_NominalLabel		 );
	vLocalCWndCollection.Add(&m_RunTime				 );
	vLocalCWndCollection.Add(&m_RunTimeLabel		 );
	vLocalCWndCollection.Add(&m_StartTime			 );
	vLocalCWndCollection.Add(&m_StartTimeLabel		 );
	vLocalCWndCollection.Add(&m_Total				 );
	vLocalCWndCollection.Add(&m_TotalLabel			 );
	vLocalCWndCollection.Add(&m_Background			 );
}

void CCheckWeighDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCheckWeighDialog)
	DDX_Control(pDX, IDC_Function3Button, m_Function3Button);
	DDX_Control(pDX, IDC_Function5Button, m_Function5Button);
	DDX_Control(pDX, IDC_Background, m_Background);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_AlarmIndicator, m_AlarmIndicator);
	DDX_Control(pDX, IDC_Function4Button, m_Function4Button);
	DDX_Control(pDX, IDC_LastDisplayedLabel, m_LastDisplayedLabel);
	DDX_Control(pDX, IDC_LastDisplayed, m_LastDisplayed);
	DDX_Control(pDX, IDC_TotalOver, m_TotalOver);
	DDX_Control(pDX, IDC_TotalOverLabel, m_TotalOverLabel);
	DDX_Control(pDX, IDC_TotalUnder, m_TotalUnder);
	DDX_Control(pDX, IDC_TotalUnderLabel, m_TotalUnderLabel);
	DDX_Control(pDX, IDC_Count, m_Count);
	DDX_Control(pDX, IDC_CountLabel, m_CountLabel);
	DDX_Control(pDX, IDC_Function2Button, m_Function2Button);
	DDX_Control(pDX, IDC_LastLabel2, m_LastLabel);
	DDX_Control(pDX, IDC_MaximumTrendThreshold, m_MaximumTrendThreshold);
	DDX_Control(pDX, IDC_MinimumTrendThreshold, m_MinimumTrendThreshold);
	DDX_Control(pDX, IDC_ConversionFormula, m_ConversionFormula);
	DDX_Control(pDX, IDC_XScale1, m_XScale1);
	DDX_Control(pDX, IDC_XScale10, m_XScale10);
	DDX_Control(pDX, IDC_XScale11, m_XScale11);
	DDX_Control(pDX, IDC_XScale12, m_XScale12);
	DDX_Control(pDX, IDC_XScale13, m_XScale13);
	DDX_Control(pDX, IDC_XScale14, m_XScale14);
	DDX_Control(pDX, IDC_XScale2, m_XScale2);
	DDX_Control(pDX, IDC_XScale3, m_XScale3);
	DDX_Control(pDX, IDC_XScale4, m_XScale4);
	DDX_Control(pDX, IDC_XScale5, m_XScale5);
	DDX_Control(pDX, IDC_XScale6, m_XScale6);
	DDX_Control(pDX, IDC_XScale7, m_XScale7);
	DDX_Control(pDX, IDC_XScale8, m_XScale8);
	DDX_Control(pDX, IDC_XScale9, m_XScale9);
	DDX_Control(pDX, IDC_YScale1, m_YScale1);
	DDX_Control(pDX, IDC_YScale10, m_YScale10);
	DDX_Control(pDX, IDC_YScale11, m_YScale11);
	DDX_Control(pDX, IDC_YScale12, m_YScale12);
	DDX_Control(pDX, IDC_YScale13, m_YScale13);
	//  DDX_Control(pDX, IDC_YScale14, m_YScale15);
	DDX_Control(pDX, IDC_YScale14, m_YScale14);
	DDX_Control(pDX, IDC_YScale15, m_YScale15);
	DDX_Control(pDX, IDC_YScale2, m_YScale2);
	DDX_Control(pDX, IDC_YScale3, m_YScale3);
	DDX_Control(pDX, IDC_YScale4, m_YScale4);
	DDX_Control(pDX, IDC_YScale5, m_YScale5);
	DDX_Control(pDX, IDC_YScale6, m_YScale6);
	DDX_Control(pDX, IDC_YScale7, m_YScale7);
	DDX_Control(pDX, IDC_YScale8, m_YScale8);
	DDX_Control(pDX, IDC_YScale9, m_YScale9);
	DDX_Control(pDX, IDC_XLabel3, m_XLabel3);
	DDX_Control(pDX, IDC_NumberOfAlarms, m_NumberOfAlarms);
	DDX_Control(pDX, IDC_NumberOfAlarmsLabel, m_NumberOfAlarmsLabel);
	DDX_Control(pDX, IDC_Average, m_Average);
	DDX_Control(pDX, IDC_AverageLabel, m_AverageLabel);
	DDX_Control(pDX, IDC_DialogTitleStaticText1, m_DialogTitleStaticText);
	DDX_Control(pDX, IDC_Function1Button, m_Function1Button);
	DDX_Control(pDX, IDC_Nominal, m_Nominal);
	DDX_Control(pDX, IDC_NominalLabel, m_NominalLabel);
	DDX_Control(pDX, IDC_RunTime, m_RunTime);
	DDX_Control(pDX, IDC_RunTimeLabel, m_RunTimeLabel);
	DDX_Control(pDX, IDC_StartTime, m_StartTime);
	DDX_Control(pDX, IDC_StartTimeLabel, m_StartTimeLabel);
	DDX_Control(pDX, IDC_Total, m_Total);
	DDX_Control(pDX, IDC_TotalLabel, m_TotalLabel);
}


BEGIN_MESSAGE_MAP(CCheckWeighDialog, CDialog)
	//{{AFX_MSG_MAP(CCheckWeighDialog)
	ON_BN_CLICKED(IDC_Function1Button, OnFunction1Button)
	ON_WM_CTLCOLOR()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
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
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_STN_CLICKED(IDC_DialogTitleStaticText1, &CCheckWeighDialog::OnDialogtitlestatictext1)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCheckWeighDialog message handlers

void CCheckWeighDialog::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	//was top 41, height 367  , left 106, width  423
	WORD TempLeft = 20;
	WORD TempLength = 600;
	WORD TempTop = 317;
	WORD TempHeight = 100;

	m_Background.SetWindowPos(NULL,TempLeft, TempTop, TempLength, TempHeight, SWP_NOZORDER);

	SetDlgItemText(IDC_MemorySize,dtoa(vCheckWeighInspection->vCheckWeighDataSize,0));

	vWeightAbreviation = "g";
	if (vCheckWeighInspection->vCheckWeighUnits ==  "Ounces")
			vWeightAbreviation = "oz";

	SetDlgItemText(IDC_Nominal, dtoa(vCheckWeighInspection->vCheckWeighNominalWeight, 2) + vWeightAbreviation);

	this->SetWindowText(_T("Weight Trending"));
	SetupMenu(vLocalCWndCollection);

	if (vStartInCalibrateOffset)
	{
		vPlotDisplay = cRecentDisplay;
		OnFunction3Button();
	}

	OneSecondUpdate();
	int TimerResult = SetTimer(vOneSecondTimerHandle,1000,NULL);
	if (!TimerResult)
		ReportErrorMessage("Error-Second Timer Failed",cEMailInspx,32000);
}

void CCheckWeighDialog::OnFunction1Button() 
{
	//Main Menu was pressed
	PrepareAndExit(10);
}

void CCheckWeighDialog::OnFunction2Button() 
{ //Low Threshold Button Clicked
	if (vPlotDisplay == cTrendDisplay) //Trend Display
	if (PasswordOK(cSetupPassword,true))
	{
		CNumericEntryDialog INumericEntryDialog;  
			
		//Set dialog box data titles and number value
		INumericEntryDialog.vEditString = dtoa(vCheckWeighInspection->ConvertToWeight(vCheckWeighInspection->vCheckWeighTrendAlarmMinimum),2);

		INumericEntryDialog.m_DialogTitleStaticText1 = "Trend Alarm Minimum";
		INumericEntryDialog.m_DialogTitleStaticText2 = "Weight of Container";
		INumericEntryDialog.m_DialogTitleStaticText3 = "Values below this threshold cause an alarm";
		INumericEntryDialog.m_DialogTitleStaticText4 = "0 = Disabled, Original Value: " + INumericEntryDialog.vEditString;
		INumericEntryDialog.m_UnitsString = vCheckWeighInspection->vCheckWeighUnits;

		INumericEntryDialog.vMinValue = 0;
		if (vCheckWeighInspection->vCheckWeighTrendAlarmMaximum)
			INumericEntryDialog.vMaxValue = vCheckWeighInspection->ConvertToWeight(vCheckWeighInspection->vCheckWeighTrendAlarmMaximum);
		else
			INumericEntryDialog.vMaxValue = 4990;

		INumericEntryDialog.vIntegerOnly = false;

		//Pass control to dialog box and display
		int nResponse = INumericEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			vCheckWeighInspection->vCheckWeighTrendAlarmMinimum = (float)vCheckWeighInspection->ConvertWeightToBrightness(ATOF(INumericEntryDialog.vEditString));

			vCheckWeighTrendDataIndexOld = 0xFFFF;
			vPlotDisplayOld = 0xFF;
			UpdateScreen();

			CWnd * TempWindow; //force painting of button in case needs to remove green color
			TempWindow = CWnd::GetDlgItem(IDC_Function2Button);
			if (TempWindow)
				TempWindow->Invalidate(false);
		}
		else 
		if (nResponse == 10)
		{
			//Main Menu button pressed
			PrepareAndExit(10);
		}
	}
}

void CCheckWeighDialog::OnFunction3Button() 
{ //Upper Threshold Button Clicked
	if (vPlotDisplay == cTrendDisplay) //Trend Display
	{
		if (PasswordOK(cSetupPassword,true))
		{
			CNumericEntryDialog INumericEntryDialog;  
			
			//Set dialog box data titles and number value
			INumericEntryDialog.vEditString = dtoa(vCheckWeighInspection->ConvertToWeight(vCheckWeighInspection->vCheckWeighTrendAlarmMaximum),2);

			INumericEntryDialog.m_DialogTitleStaticText1 = "Trend Alarm Maximum";
			INumericEntryDialog.m_DialogTitleStaticText2 = "Weight of Container";
			INumericEntryDialog.m_DialogTitleStaticText3 = "Values above this threshold cause an alarm";
			INumericEntryDialog.m_DialogTitleStaticText4 = "0 = Disabled, Original Value: " + INumericEntryDialog.vEditString;
			INumericEntryDialog.m_UnitsString = vCheckWeighInspection->vCheckWeighUnits;

			INumericEntryDialog.vMinValue = 0;
			INumericEntryDialog.vAllowZero = true;
			if (vCheckWeighInspection->vCheckWeighTrendAlarmMinimum)
				INumericEntryDialog.vMinValue = vCheckWeighInspection->ConvertToWeight(vCheckWeighInspection->vCheckWeighTrendAlarmMinimum);

			INumericEntryDialog.vMaxValue = 4999;
			INumericEntryDialog.vIntegerOnly = false;

			//Pass control to dialog box and display
			int nResponse = INumericEntryDialog.DoModal();
			//dialog box is now closed, if user pressed select do this
			//if user pressed cancel, do nothing
			if (nResponse == IDOK)
			{
				vCheckWeighInspection->vCheckWeighTrendAlarmMaximum = (float)vCheckWeighInspection->ConvertWeightToBrightness(ATOF(INumericEntryDialog.vEditString));
				vCheckWeighTrendDataIndexOld = 0xFFFF;

				vPlotDisplayOld = 0xFF;
				UpdateScreen();

				CWnd * TempWindow; //force painting of button in case needs to remove green color
				TempWindow = CWnd::GetDlgItem(IDC_Function3Button);
				if (TempWindow)
					TempWindow->Invalidate(false);
			}
			else 
			if (nResponse == 10)
			{
				//Main Menu button pressed
				PrepareAndExit(10);
			}
		}
	}
	else
	if (vPlotDisplay == cRecentDisplay) //Recent Display
	{
		if (PasswordOK(cSetupPassword,true))
		if (vLocalSystemData->vSystemRunMode == cStoppedSystemMode)
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\n\nScanTrac must be in Inspecting mode\nto calibrate weight offset.";
			TempNoticeDialog.vType = cNoticeMessage;
			TempNoticeDialog.DoModal();
		}
		else
		{
			CCheckWeighOffsetDialog ICheckWeighOffsetDialog;
			ICheckWeighOffsetDialog.vCheckWeighInspection = vCheckWeighInspection;
			ICheckWeighOffsetDialog.vCheckWeighInspectionNumber = vCheckWeighInspectionNumber;
			ICheckWeighOffsetDialog.vLocalSystemData = vLocalSystemData;
			ICheckWeighOffsetDialog.vLocalConfigurationData = vLocalConfigurationData;
			double TempOriginalOffset = vCheckWeighInspection->vCheckWeighOffset;

			int nResponse = ICheckWeighOffsetDialog.DoModal();
			//dialog box is now closed, if user pressed select do this
			//if user pressed cancel, do nothing
			if (nResponse == IDOK)
			{
				double TempWeightOperatorEntered = ATOF(ICheckWeighOffsetDialog.vEditString);
				if ((vCheckWeighInspection->vCheckWeighNominalWeight) && (TempWeightOperatorEntered) &&
					((TempWeightOperatorEntered > vCheckWeighInspection->vCheckWeighNominalWeight * 1.3) || (TempWeightOperatorEntered < vCheckWeighInspection->vCheckWeighNominalWeight * 0.7)))
				{
					CNoticeDialog TempNoticeDialog;
					TempNoticeDialog.vNoticeText = "\n\n\nInput ignored.\nWeight entered more than 30% different than Nominal Weight.";
					TempNoticeDialog.vType = cErrorMessage;
					TempNoticeDialog.DoModal();
				}
				else
				{
					if (TempWeightOperatorEntered == 0)
						vCheckWeighInspection->vCheckWeighOffset = 0;
					else
						vCheckWeighInspection->vCheckWeighOffset = vCheckWeighInspection->vCheckWeighOffset + TempWeightOperatorEntered - ICheckWeighOffsetDialog.vAverageMeasuredWeightOfContainer; 

					ReportErrorMessage(vCheckWeighInspection->vName + " - Operator changed Offset from: " + dtoa(TempOriginalOffset, 2) + " to: " + dtoa(vCheckWeighInspection->vCheckWeighOffset, 2), cUserChanges,0);

					vLocalSystemData->vCheckWeighOffsetCalibrationNeeded = false;
					vMainWindowPointer->SetInterlockStatus();

					UpdateScreen();

					CWnd * TempWindow; //force painting of button in case needs to remove green color
					TempWindow = CWnd::GetDlgItem(IDC_Function2Button);
					if (TempWindow)
						TempWindow->Invalidate(false);
				}
			}
			else 
			if (nResponse == 10)
			{
				//Main Menu button pressed
				PrepareAndExit(10);
			}
		}
	}
		/*
		if (!vLastWeightOld)
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\n\nYou must send a container with a known weight first\nto enter that weight for offset calculation.";
			TempNoticeDialog.vType = cErrorMessage;
			TempNoticeDialog.DoModal();
		}
		else
		if (vLocalSystemData->vContainerRate > 60)
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\n\nProduction must be stopped and\nyou must send a container with a known weight first\nfor weight offset calculation.";
			TempNoticeDialog.vType = cErrorMessage;
			TempNoticeDialog.DoModal();
		}
		else
		{
			bool TempPasswordOK = (PasswordOK(cTemporaryInspxPassword,false));
			if (!TempPasswordOK)
			if (vLocalConfigurationData->vQAPassword == vLocalConfigurationData->vProductSetupPassword)
				TempPasswordOK = (PasswordOK(cSetupPassword,true));

			if (!TempPasswordOK) //must enter one time use QA password if different from setup password
			{
				CPasswordEntryDialog IPasswordEntryDialog;  

				//Set dialog box data titles and number value
				IPasswordEntryDialog.vEditString = "";
				IPasswordEntryDialog.m_DialogTitleStaticText2 = "Will be one time use only";
				IPasswordEntryDialog.m_DialogTitleStaticText1 = "Enter QA Password to Enter Weight for Offset";
					
				//Pass control to dialog box and display
				int nResponse = IPasswordEntryDialog.DoModal();
				//dialog box is now closed, if user pressed select do this
				//if user pressed cancel, do nothingd
				if (nResponse == IDOK)
				{			//super user
					if ((vLocalConfigurationData->vQAPassword == IPasswordEntryDialog.vEditString) ||
						("31415926" == IPasswordEntryDialog.vEditString) ||
						("72054328" == IPasswordEntryDialog.vEditString) ||
						("7205" == IPasswordEntryDialog.vEditString) ||
						("5104682199" == IPasswordEntryDialog.vEditString) ||
						(vLocalConfigurationData->vInspxPassword == IPasswordEntryDialog.vEditString) ||
						("10221022" == IPasswordEntryDialog.vEditString) ||
						(DisposablePasswordOK(IPasswordEntryDialog.vEditString, cTemporaryInspxPassword)))
							TempPasswordOK = true;
					else
					{
						CNoticeDialog TempNoticeDialog;
						TempNoticeDialog.vNoticeText = "\n\n\nPassword not correct.";
						TempNoticeDialog.vType = cErrorMessage;
						TempNoticeDialog.DoModal();
					}
				}
			}

			if (TempPasswordOK)
			{
				CNumericEntryDialog INumericEntryDialog;
				//Set dialog box data titles and number value
				INumericEntryDialog.vEditString = dtoa(vCheckWeighInspection->vCheckWeighNominalWeight,2);
				double TempLastWeight = vCheckWeighInspection->ConvertToWeight(vLastWeightOld);

				INumericEntryDialog.m_DialogTitleStaticText1 = "Enter Measured Weight of most recent container sent.";
				INumericEntryDialog.m_DialogTitleStaticText2 = "Weight Offset will be calcuated from Last: " + dtoa(TempLastWeight,2);
				if (vLocalSystemData->vShiftProductCode.GetLength() > 1)
					INumericEntryDialog.m_DialogTitleStaticText3 = "This offset applies to product code: " + vLocalSystemData->vShiftProductCode;
				INumericEntryDialog.m_DialogTitleStaticText4 = "0 = Disabled, Nominal Weight: " + INumericEntryDialog.vEditString;
				INumericEntryDialog.vAllowZero = true;
				INumericEntryDialog.m_UnitsString = vCheckWeighInspection->vCheckWeighUnits;

				INumericEntryDialog.vMinValue = 0;
				if (vCheckWeighInspection->vCheckWeighTrendAlarmMaximum)
					INumericEntryDialog.vMaxValue = vCheckWeighInspection->ConvertToWeight(vCheckWeighInspection->vCheckWeighTrendAlarmMaximum) + 10;
				else
					INumericEntryDialog.vMaxValue = 4990;

				INumericEntryDialog.vIntegerOnly = false;

				double TempOriginalOffset = vCheckWeighInspection->vCheckWeighOffset;
				//Pass control to dialog box and display
				int nResponse = INumericEntryDialog.DoModal();
				//dialog box is now closed, if user pressed select do this
				//if user pressed cancel, do nothing
				if (nResponse == IDOK)
				{
					double TempWeightOperatorEntered = ATOF(INumericEntryDialog.vEditString);
					if ((vCheckWeighInspection->vCheckWeighNominalWeight) && (TempWeightOperatorEntered) &&
						((TempWeightOperatorEntered > vCheckWeighInspection->vCheckWeighNominalWeight * 1.3) || (TempWeightOperatorEntered < vCheckWeighInspection->vCheckWeighNominalWeight * 0.7)))
					{
						CNoticeDialog TempNoticeDialog;
						TempNoticeDialog.vNoticeText = "\n\n\nInput ignored.\nWeight entered more than 30% different than Nominal Weight.";
						TempNoticeDialog.vType = cErrorMessage;
						TempNoticeDialog.DoModal();
					}
					else
					{
						if (TempWeightOperatorEntered == 0)
							vCheckWeighInspection->vCheckWeighOffset = 0;
						else
							vCheckWeighInspection->vCheckWeighOffset = vCheckWeighInspection->vCheckWeighOffset + TempWeightOperatorEntered - TempLastWeight; 

						ReportErrorMessage(vCheckWeighInspection->vName + " - Operator changed Offset from: " + dtoa(TempOriginalOffset, 2) + " to: " + dtoa(vCheckWeighInspection->vCheckWeighOffset, 2), cUserChanges,0);

						vLocalSystemData->vCheckWeighOffsetCalibrationNeeded = false;
						vMainWindowPointer->SetInterlockStatus();

						UpdateScreen();

						CWnd * TempWindow; //force painting of button in case needs to remove green color
						TempWindow = CWnd::GetDlgItem(IDC_Function2Button);
						if (TempWindow)
							TempWindow->Invalidate(false);
					}
				}
				else 
				if (nResponse == 10)
				{
					//Main Menu button pressed
					PrepareAndExit(10);
				}
			}
		}
		*/
}

void CCheckWeighDialog::OnFunction4Button() 
{
	// TODO: Add your control notification handler code here
	if (vPlotDisplay == cTrendDisplay)  //Trend Display
	{
		if (vCheckWeighInspection->vCheckWeighAlarmLatched)
		{
			bool TempPasswordOK = (PasswordOK(cTemporaryInspxPassword,false));
			if (!TempPasswordOK)
			if (vLocalConfigurationData->vQAPassword == vLocalConfigurationData->vProductSetupPassword)
				TempPasswordOK = (PasswordOK(cSetupPassword,true));

			if (!TempPasswordOK) //must enter one time use QA password if different from setup password
			{
				CPasswordEntryDialog IPasswordEntryDialog;  

				//Set dialog box data titles and number value
				IPasswordEntryDialog.vEditString = "";
				IPasswordEntryDialog.m_DialogTitleStaticText2 = "Will be one time use only";
				IPasswordEntryDialog.m_DialogTitleStaticText1 = "Enter QA Password to Clear Alarm";
					
				//Pass control to dialog box and display
				int nResponse = IPasswordEntryDialog.DoModal();
				//dialog box is now closed, if user pressed select do this
				//if user pressed cancel, do nothingd
				if (nResponse == IDOK)
				{			//super user
					if ((vLocalConfigurationData->vQAPassword == IPasswordEntryDialog.vEditString) ||
						("31415926" == IPasswordEntryDialog.vEditString) ||
						("72054328" == IPasswordEntryDialog.vEditString) ||
						("7205" == IPasswordEntryDialog.vEditString) ||
						("5104682199" == IPasswordEntryDialog.vEditString) ||
						(vLocalConfigurationData->vInspxPassword == IPasswordEntryDialog.vEditString) ||
						("10221022" == IPasswordEntryDialog.vEditString) ||
						(DisposablePasswordOK(IPasswordEntryDialog.vEditString, cTemporaryInspxPassword)))
							TempPasswordOK = true;
					else
					{
						CNoticeDialog TempNoticeDialog;
						TempNoticeDialog.vNoticeText = "\n\n\nPassword not correct.\nAlarm not cleared.";
						TempNoticeDialog.vType = cErrorMessage;
						TempNoticeDialog.DoModal();
					}
				}
			}

			if (TempPasswordOK)
			{
				vCheckWeighInspection->vCheckWeighAlarmLatched = false;
				vLocalSystemData->vWeightTrendingAlarm = false;
				ReportErrorMessage(vCheckWeighInspection->vName + " - Trending Alarm Cleared by Password", cUserChanges,0);
				vMainWindowPointer->SetInterlockStatus();
				UpdateScreen();
			}
		}
		else
		if (PasswordOK(cSuperInspxPassword,false))
		{
			CNumericEntryDialog INumericEntryDialog;
	
			//Set dialog box data titles and number value
			INumericEntryDialog.vEditString = dtoa(vCheckWeighInspection->vCheckWeighTrendDataSize,0);

			INumericEntryDialog.m_DialogTitleStaticText1 = "Enter the number of Trend Data Samples to keep";
			INumericEntryDialog.m_DialogTitleStaticText2 = "";
			INumericEntryDialog.m_DialogTitleStaticText3 = "For Weight Trending Inpection: " + vCheckWeighInspection->vName;
			INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + INumericEntryDialog.vEditString;

			INumericEntryDialog.m_UnitsString = "Maximum 15000 samples";
			INumericEntryDialog.vMaxValue = 15000;
			INumericEntryDialog.vIntegerOnly = true;
			//Pass control to dialog box and display
			int nResponse = INumericEntryDialog.DoModal();
			//dialog box is now closed, if user pressed select do this
			//if user pressed cancel, do nothing
			if (nResponse == IDOK)
			{
				vCheckWeighInspection->vCheckWeighTrendDataSize = _wtoi(INumericEntryDialog.vEditString);
				vCheckWeighInspection->CreateDataMemory();
				vMinimumValue = 0xffffffffffff;
				vMaximumValue = 0;
				SetDlgItemText(IDC_MemorySize,dtoa(vCheckWeighInspection->vCheckWeighDataSize,0));
				vOldDataIndex = -2;
				OneSecondUpdate();
				PreparePlotData();
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
	if (PasswordOK(cSuperInspxPassword,false))
	{
		CNumericEntryDialog INumericEntryDialog;
	
		//Set dialog box data titles and number value
		INumericEntryDialog.vEditString = dtoa(vCheckWeighInspection->vCheckWeighDataSize,0);

		INumericEntryDialog.m_DialogTitleStaticText1 = "Enter the number of Recent Data Samples to keep";
		INumericEntryDialog.m_DialogTitleStaticText2 = "";
		INumericEntryDialog.m_DialogTitleStaticText3 = "For Weight Trending Inpection: " + vCheckWeighInspection->vName;
		INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + INumericEntryDialog.vEditString;

		INumericEntryDialog.m_UnitsString = "Maximum 15000 samples";
		INumericEntryDialog.vMaxValue = 15000;
		INumericEntryDialog.vIntegerOnly = true;
		//Pass control to dialog box and display
		int nResponse = INumericEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			vCheckWeighInspection->vCheckWeighDataSize = _wtoi(INumericEntryDialog.vEditString);
			vCheckWeighInspection->CreateDataMemory();
			vMinimumValue = 0xffffffffffff;
			vMaximumValue = 0;
			SetDlgItemText(IDC_MemorySize,dtoa(vCheckWeighInspection->vCheckWeighDataSize,0));
			vOldDataIndex = -2;
			OneSecondUpdate();
			PreparePlotData();
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

void CCheckWeighDialog::OnFunction5Button() 
{
	if ((!vCheckWeighInspection->vCheckWeighTrendAlarmMinimum) || (!vCheckWeighInspection->vCheckWeighTrendAlarmMaximum))
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\n\nYou must configure the Low and High Thresholds\nto see check weighing data.";
		TempNoticeDialog.vType = cErrorMessage;
		TempNoticeDialog.DoModal();
	}
	else
	if (vPlotDisplay == cTrendDisplay)
		vPlotDisplay = cRecentDisplay;
	else
	if (vPlotDisplay == cRecentDisplay)
		vPlotDisplay = cTrendMinuteDisplay;
	else
		vPlotDisplay = cTrendDisplay;

	DrawCompletelyNewPlot();
	UpdateScreen();
}

void CCheckWeighDialog::OnSubFunction1Button() 
{
}

void CCheckWeighDialog::OnSubFunction2Button() 
{
	// TODO: Add your control notification handler code here
	
}

void CCheckWeighDialog::OnSubFunction3Button() 
{
	// TODO: Add your control notification handler code here
	
}

void CCheckWeighDialog::OnSubFunction4Button() 
{
	// TODO: Add your control notification handler code here
	
}

void CCheckWeighDialog::OnSubFunction5Button() 
{
}

void CCheckWeighDialog::OnSubFunction6Button() 
{
}

void CCheckWeighDialog::OnSubFunction7Button() 
{  
}

void CCheckWeighDialog::OnSubFunction8Button() 
{  //plot CheckWeight Plot of data
	if (vCheckWeighInspection->vCheckWeighDataIndex == -1)
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\n\nError - No data to plot";
		TempNoticeDialog.vType = cErrorMessage;
		TempNoticeDialog.DoModal();
	}
	else
	{
		WORD TempNumberOfSamples = 0;
		double *TempOriginalDataPointer = NULL;

		if (vPlotDisplay == cTrendDisplay)
		{
			TempNumberOfSamples = vCheckWeighInspection->vCheckWeighTrendNumberOfDataSamplesTaken;
			TempOriginalDataPointer = vCheckWeighInspection->vCheckWeighTrendData;
		}
		else
		if (vPlotDisplay == cRecentDisplay)
		{
			TempNumberOfSamples = vCheckWeighInspection->vCheckWeighNumberOfDataSamplesTaken;
			TempOriginalDataPointer = vCheckWeighInspection->vCheckWeighData;
		}
		else
		if (vPlotDisplay == cTrendMinuteDisplay)
		{
			TempNumberOfSamples = vCheckWeighInspection->vCheckWeighTrendNumberOfDataSamplesTakenOnMinutes;
			TempOriginalDataPointer = vCheckWeighInspection->vCheckWeighTrendDataOnMinutes;
		}

		double *TempData = NULL;
		if (ThereIsEnoughMemory(TempNumberOfSamples * sizeof(double), "Weight Trending Plot Data"))
		{
			TempData = (double *)malloc((TempNumberOfSamples) * sizeof(double));

			if (TempData)
			{
				double *TempSourcePointer = TempOriginalDataPointer;

				bool TempShowUnits = 	(((vCheckWeighInspection->vCheckWeighBFactor) || (vCheckWeighInspection->vCheckWeighMFactor != 1)));

				double *TempDestinationPointer = TempData;
				DWORD TempCount = 0;
				for (WORD TempLoop = 0; TempLoop < TempNumberOfSamples; TempLoop++)
				{
					if (*TempSourcePointer)
					{
						if (TempShowUnits)
							*TempDestinationPointer = (double)(*TempSourcePointer * vCheckWeighInspection->vCheckWeighMFactor + vCheckWeighInspection->vCheckWeighBFactor);
						else
							*TempDestinationPointer = *TempSourcePointer;

						TempDestinationPointer++;
						TempCount++;
					}
					TempSourcePointer++;
				}
				if (TempCount)
				{
					CHistogramDisplayDialog IHistogramDisplayDialog;
					IHistogramDisplayDialog.vNumberOfNumbers = TempCount;
					IHistogramDisplayDialog.vDataTitle = "Weight Trending Data";
					IHistogramDisplayDialog.vValuesAreFloats = true;
					IHistogramDisplayDialog.vPointerToFloats = (double *)&TempData[0];
					IHistogramDisplayDialog.vLocalSystemData = vLocalSystemData;
					IHistogramDisplayDialog.vLocalConfigurationData = vLocalConfigurationData;
					IHistogramDisplayDialog.vMainWindowPointer = vMainWindowPointer;
					int nResponse = IHistogramDisplayDialog.DoModal();
					if (nResponse == 10)
					{
						//Main Menu button pressed
						Sleep(1); //is the equivelent of a yeild statement;
						PrepareAndExit(10);
					}
				}
				else
				{
					CNoticeDialog TempNoticeDialog;
					TempNoticeDialog.vNoticeText = "\n\n\nNo Data to plot.";
					TempNoticeDialog.vType = cErrorMessage;
					TempNoticeDialog.DoModal();
				}
				free(TempData);
			}
		}
	}
}

BOOL CCheckWeighDialog::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	CWnd * TempWindow;
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
			case 35: // End Key, Left Right on duo but new Image Key on Universal Overlays, Image Button
				OnSubFunction8Button();
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
			case cPageUp: // Clear, but enter System setup password
			{
				if (vGlobalPasswordLevel)
				{
					vGlobalPasswordLevel = cNoPassword;
					PasswordOK(cNoPassword,false);
					if (vGlobalCurrentUser != "No User")
						ReportErrorMessage(vGlobalCurrentUser + " logged out.", cUserChanges, 0);
					vGlobalCurrentUser = "No User";
				}
				else
				{
					PasswordOK(cAnyPasswordToEnter, true);
				}
				TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText1);
				if (TempWindow)
					TempWindow->Invalidate(false);
			}
			break;
			case 16: // Shift Key
			case 0x60:  //0 key on KeyPad
				PostThreadMessage(vGlobalCallingThreadID,cHiddenKeyPressed, 1, 0);
			break;
			case 36: // Home Key, Screen Capture
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
	
	return CDialog::PreTranslateMessage(pMsg);
}

HBRUSH CCheckWeighDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	HWND TempDialogHwnd = this->m_hWnd;
	int TextSize;
	pDC->SetAttribDC(pDC->m_hDC);

	if (pWnd->GetDlgCtrlID() == IDC_DialogTitleStaticText1)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DialogTitleStaticText, 8);  //5 is medium large
		if (vGlobalShiftKeyDown)
			pDC->SetTextColor(cBlack);
		else
			pDC->SetTextColor(vGlobalDialogTitleColor);
	}

	if (pWnd->GetDlgCtrlID() == IDC_Function2Button)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function2Button, 5);  //5 is medium large
		if (!vCheckWeighInspection->vCheckWeighTrendAlarmMinimum)
		{
			pDC->SetTextColor(cButtonTextColor);
			pDC->SetBkMode(TRANSPARENT);
			return vLocalSystemData->vGreenBrush;
		}
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBlack);
		return vGlobalButtonColorBrush;

	}
	
	if (pWnd->GetDlgCtrlID() == IDC_Function3Button)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function3Button, 5);
		if (!vCheckWeighInspection->vCheckWeighTrendAlarmMaximum)
		{
			pDC->SetTextColor(cButtonTextColor);
			pDC->SetBkMode(TRANSPARENT);
			return vLocalSystemData->vGreenBrush;
		}
	}
	
	if (pWnd->GetDlgCtrlID() == IDC_ConversionFormula)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_ConversionFormula, 5);
		if (((!vCheckWeighInspection->vCheckWeighBFactor) && (vCheckWeighInspection->vCheckWeighMFactor == 1)) || 
			(!vCheckWeighInspection->vCheckWeighTrendAlarmMinimum) || (!vCheckWeighInspection->vCheckWeighTrendAlarmMaximum))
		{
			pDC->SetTextColor(cButtonTextColor);
			pDC->SetBkMode(TRANSPARENT);
			return vLocalSystemData->vRedBrush;
		}
	}
	
	if (pWnd->GetDlgCtrlID() == IDC_Nominal)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Nominal, 5);
		pDC->SetTextColor(cMediumDarkGreen);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if  (pWnd->GetDlgCtrlID() == IDC_NominalLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_NominalLabel, 5);
		pDC->SetTextColor(cMediumDarkGreen);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Average)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Average, 5);
		if (((vCheckWeighInspection->vCheckWeighTrendAlarmMaximum) && (vDisplayAverageOld > vCheckWeighInspection->vCheckWeighTrendAlarmMaximum)) 
			|| (vDisplayAverageOld < vDisplayThresholdLower))
			pDC->SetTextColor(cRed);
	}

	if (pWnd->GetDlgCtrlID() == IDC_AlarmIndicator)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_AlarmIndicator, 5);
		pDC->SetTextColor(cDarkBlue);
		
		if (vPlotDisplay) //either trend display
			return vLocalSystemData->vRedBrush;
		//else
		//	return vLocalSystemData->vLightGreenBrush;
	}

	if (pWnd->GetDlgCtrlID() == IDC_AverageLabel2)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_AverageLabel2, 5);
		pDC->SetTextColor(cBlue);
	}
	//if (pWnd->GetDlgCtrlID() == IDC_PlotBottom)
	//{
	//	TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_PlotBottom, 5);
	//	pDC->SetTextColor(cBlue);
	//}
	//if (pWnd->GetDlgCtrlID() == IDC_PlotTop)
	//{
	//	TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_PlotTop, 5);
	//	pDC->SetTextColor(cBlue);
	//}
	
	//if ((pWnd->GetDlgCtrlID() == IDC_PlotLowerThreshold) ||
	//	(pWnd->GetDlgCtrlID() == IDC_PlotUpperThreshold))
	//{
	//	pDC->SetTextColor(cYellow);
	//}
	
	if (pWnd->GetDlgCtrlID() == IDC_MinimumTrendThreshold)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_MinimumTrendThreshold, 5);
		pDC->SetTextColor(cBlue);pDC->SetTextColor(cBlack);
	}
	if (pWnd->GetDlgCtrlID() == IDC_MaximumTrendThreshold)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_MaximumTrendThreshold, 5);
		pDC->SetTextColor(cBlue);pDC->SetTextColor(cBlack);
	}

	if (pWnd->GetDlgCtrlID() == IDC_CountLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_CountLabel, 5);
		if (vCheckWeighInspection->vCheckWeighOffset != 0)
		{
			pDC->SetTextColor(cBlack);
			pDC->SetBkMode(TRANSPARENT);
			if (vLocalSystemData->vCheckWeighOffsetCalibrationNeeded)
				return vLocalSystemData->vRedBrush;
			else
				return vLocalSystemData->vYellowBrush;
		}
	}
	if (pWnd->GetDlgCtrlID() == IDC_Count)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Count, 5);
		if (vCheckWeighInspection->vCheckWeighOffset != 0)
		{
			pDC->SetTextColor(cBlack);
			pDC->SetBkMode(TRANSPARENT);
			if (vLocalSystemData->vCheckWeighOffsetCalibrationNeeded)
				return vLocalSystemData->vRedBrush;
			else
				return vLocalSystemData->vYellowBrush;
		}
	}

	if (pWnd->GetDlgCtrlID() == IDC_Background)
	{
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_ConversionFormula)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_ConversionFormula, 5);
		pDC->SetTextColor(cBlack);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_CountLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_CountLabel, 5);
		pDC->SetTextColor(cBlack);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_RunTimeLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_RunTimeLabel, 5);
		pDC->SetTextColor(cBlack);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_TotalLabel) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_TotalLabel, 5);
		pDC->SetTextColor(cBlack);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_StartTimeLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_StartTimeLabel, 5);
		pDC->SetTextColor(cBlack);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_LastDisplayedLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_LastDisplayedLabel, 5);
		pDC->SetTextColor(cBlack);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_TotalUnderLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_TotalUnderLabel, 5);
		pDC->SetTextColor(cBlack);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_TotalOverLabel) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_TotalOverLabel, 5);
		pDC->SetTextColor(cBlack);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_LastLabel2) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_LastLabel, 5);
		pDC->SetTextColor(cBlack);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_NumberOfAlarmsLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_NumberOfAlarmsLabel, 5);
		pDC->SetTextColor(cBlack);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_AverageLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_AverageLabel, 5);
		pDC->SetTextColor(cBlack);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_MinimumTrendThreshold)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_MinimumTrendThreshold, 5);
		pDC->SetTextColor(cBlack);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vPlotBackGroundBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_MaximumTrendThreshold)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_MaximumTrendThreshold, 5);
		pDC->SetTextColor(cBlack);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vPlotBackGroundBrush;
	}

	if (pWnd->GetDlgCtrlID() == IDC_YScale1) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_YScale1, 5);
		pDC->SetTextColor(cBlack);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vPlotBackGroundBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_YScale2) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_YScale2, 5);
		pDC->SetTextColor(cBlack);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vPlotBackGroundBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_YScale3) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_YScale3, 5);
		pDC->SetTextColor(cBlack);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vPlotBackGroundBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_YScale4) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_YScale4, 5);
		pDC->SetTextColor(cBlack);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vPlotBackGroundBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_YScale5) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_YScale5, 5);
		pDC->SetTextColor(cBlack);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vPlotBackGroundBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_YScale6) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_YScale6, 5);
		pDC->SetTextColor(cBlack);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vPlotBackGroundBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_YScale7) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_YScale7, 5);
		pDC->SetTextColor(cBlack);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vPlotBackGroundBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_YScale8) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_YScale8, 5);
		pDC->SetTextColor(cBlack);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vPlotBackGroundBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_YScale9) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_YScale9, 5);
		pDC->SetTextColor(cBlack);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vPlotBackGroundBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_YScale10) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_YScale10, 5);
		pDC->SetTextColor(cBlack);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vPlotBackGroundBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_YScale11) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_YScale11, 5);
		pDC->SetTextColor(cBlack);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vPlotBackGroundBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_YScale12) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_YScale12, 5);
		pDC->SetTextColor(cBlack);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vPlotBackGroundBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_YScale13) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_YScale13, 5);
		pDC->SetTextColor(cBlack);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vPlotBackGroundBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_YScale14) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_YScale14, 5);
		pDC->SetTextColor(cBlack);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vPlotBackGroundBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_YScale15) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_YScale15, 5);
		pDC->SetTextColor(cBlack);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vPlotBackGroundBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_XScale1) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_XScale1, 5);
		pDC->SetTextColor(cBlack);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vPlotBackGroundBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_XScale2) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_XScale2, 5);
		pDC->SetTextColor(cBlack);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vPlotBackGroundBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_XScale3) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_XScale3, 5);
		pDC->SetTextColor(cBlack);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vPlotBackGroundBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_XScale4) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_XScale4, 5);
		pDC->SetTextColor(cBlack);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vPlotBackGroundBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_XScale5) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_XScale5, 5);
		pDC->SetTextColor(cBlack);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vPlotBackGroundBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_XScale6) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_XScale6, 5);
		pDC->SetTextColor(cBlack);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vPlotBackGroundBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_XScale7) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_XScale7, 5);
		pDC->SetTextColor(cBlack);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vPlotBackGroundBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_XScale8) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_XScale8, 5);
		pDC->SetTextColor(cBlack);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vPlotBackGroundBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_XScale9) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_XScale9, 5);
		pDC->SetTextColor(cBlack);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vPlotBackGroundBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_XScale10) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_XScale10, 5);
		pDC->SetTextColor(cBlack);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vPlotBackGroundBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_XScale11) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_XScale11, 5);
		pDC->SetTextColor(cBlack);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vPlotBackGroundBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_XScale12) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_XScale12, 5);
		pDC->SetTextColor(cBlack);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vPlotBackGroundBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_XScale13) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_XScale13, 5);
		pDC->SetTextColor(cBlack);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vPlotBackGroundBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_XScale14) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_XScale14, 5);
		pDC->SetTextColor(cBlack);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vPlotBackGroundBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_XLabel3)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_XLabel3, 5);
		pDC->SetTextColor(cBlack);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vPlotBackGroundBrush;
	}

	if (pWnd->GetDlgCtrlID() == IDC_Count)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Count, 5);
		pDC->SetTextColor(cDarkBlue);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_RunTime)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_RunTime, 5);
		pDC->SetTextColor(cDarkBlue);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Total) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Total, 5);
		pDC->SetTextColor(cDarkBlue);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_StartTime) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_StartTime, 5);
		pDC->SetTextColor(cDarkBlue);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_LastDisplayed) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_LastDisplayed, 5);
		pDC->SetTextColor(cDarkBlue);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_TotalUnder) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_TotalUnder, 5);
		pDC->SetTextColor(cDarkBlue);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_TotalOver)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_TotalOver, 5);
		pDC->SetTextColor(cDarkBlue);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_NumberOfAlarms) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_NumberOfAlarms, 5);
		pDC->SetTextColor(cDarkBlue);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Average)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Average, 5);
		pDC->SetTextColor(cDarkBlue);
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
	if (pWnd->GetDlgCtrlID() == IDC_Function3Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function3Button, 5);  //5 is medium large
		pDC->SetTextColor(cBlack);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Function4Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function4Button, 5);  //5 is medium large
		pDC->SetTextColor(cBlack);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Function5Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function5Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	//if (pWnd->GetDlgCtrlID() == IDC_SubFunction1Button)
	//{
	//	pDC->SetBkMode(TRANSPARENT);
	//	TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction1Button, 5);  //5 is medium large
	//	pDC->SetTextColor(cButtonTextColor);
	//	return vGlobalButtonColorBrush;
	//}
	//if (pWnd->GetDlgCtrlID() == IDC_SubFunction2Button)
	//{
	//	pDC->SetBkMode(TRANSPARENT);
	//	TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction2Button, 5);  //5 is medium large
	//	pDC->SetTextColor(cButtonTextColor);
	//	return vGlobalButtonColorBrush;
	//}
	//if (pWnd->GetDlgCtrlID() == IDC_SubFunction3Button)
	//{
	//	pDC->SetBkMode(TRANSPARENT);
	//	TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction3Button, 5);  //5 is medium large
	//	pDC->SetTextColor(cButtonTextColor);
	//	return vGlobalButtonColorBrush;
	//}
	//if (pWnd->GetDlgCtrlID() == IDC_SubFunction4Button)
	//{
	//	pDC->SetBkMode(TRANSPARENT);
	//	TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction4Button, 5);  //5 is medium large
	//	pDC->SetTextColor(cButtonTextColor);
	//	return vGlobalButtonColorBrush;
	//}
	//if (pWnd->GetDlgCtrlID() == IDC_SubFunction5Button)
	//{
	//	pDC->SetBkMode(TRANSPARENT);
	//	TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction5Button, 5);  //5 is medium large
	//	pDC->SetTextColor(cButtonTextColor);
	//	return vGlobalButtonColorBrush;
	//}
	//if (pWnd->GetDlgCtrlID() == IDC_SubFunction6Button) 
	//{
	//	pDC->SetBkMode(TRANSPARENT);
	//	TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction6Button, 5);  //5 is medium large
	//	pDC->SetTextColor(cButtonTextColor);
	//	return vGlobalButtonColorBrush;
	//}
	//if (pWnd->GetDlgCtrlID() == IDC_SubFunction7Button) 
	//{
	//	pDC->SetBkMode(TRANSPARENT);
	//	TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction7Button, 5);  //5 is medium large
	//	pDC->SetTextColor(cButtonTextColor);
	//	return vGlobalButtonColorBrush;
	//}
	//if (pWnd->GetDlgCtrlID() == IDC_SubFunction8Button)
	//{
	//	pDC->SetBkMode(TRANSPARENT);
	//	TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction8Button, 5);  //5 is medium large
	//	pDC->SetTextColor(cButtonTextColor);
	//	return vGlobalButtonColorBrush;
	//}
	
	return hbr;
}

void CCheckWeighDialog::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
  if (nIDEvent == vOneSecondTimerHandle)
  {
		OneSecondUpdate();
	}
	CDialog::OnTimer(nIDEvent);
}

void CCheckWeighDialog::OneSecondUpdate()
{
	//vUpdateCountDown++;
	//BYTE TempRefreshRate = vLocalSystemData->vContainerRate / 200;
	
	//if (vUpdateCountDown > TempRefreshRate)
	//{
	//	vUpdateCountDown = 0;
		UpdateScreen();
	//}

	if (vOldPasswordLevel != vGlobalPasswordLevel)
	{
		vOldPasswordLevel = vGlobalPasswordLevel;
		CWnd *TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText1);
		if (TempWindow)
			TempWindow->Invalidate(false);
	}
}

void CCheckWeighDialog::SetupDisplays()
{
	vLastDisplayedIndex = 0xFFFF;
	vDisplayNumberOfPointsToPlot = 0;
	if (vPlotDisplay) //either Trend display
	{
		vDisplayThresholdLower = vCheckWeighInspection->vCheckWeighTrendAlarmMinimum;
		vDisplayThresholdUpper = vCheckWeighInspection->vCheckWeighTrendAlarmMaximum;

		vNumberOfPointsToPlot = 0;

		//if (vPlotDisplay == cTrendDisplay) //either Trend display
		//	SetDlgItemText(IDC_DialogTitleStaticText1,vCheckWeighInspection->vName + " Trend Data");
		//else
		//	SetDlgItemText(IDC_DialogTitleStaticText1,vCheckWeighInspection->vName + " Minute Trend Data");
		if (vPlotDisplay == cTrendDisplay) //either Trend display
			SetDlgItemText(IDC_DialogTitleStaticText1, _T("Weight Trend Data"));
		else
			SetDlgItemText(IDC_DialogTitleStaticText1, _T("Minute Weight Trend Data"));

		vCheckWeighInspection->vCheckWeighAlarmLatchedOld = false; //force screen update if should be displayed

		SetDlgItemText(IDC_NumberOfAlarmsLabel, _T("Number Of Alarms:"));
		vNumberOfAlarmsOld = -1; //force Screen Update

		SetDlgItemText(IDC_RunTimeLabel, _T("Run Time:"));
		vRunDurationOld = -1; //force Screen Update

		SetDlgItemText(IDC_StartTimeLabel, _T("Start Time:"));
		vRunStartTimeOld = "!"; //force Screen Update

		vDisplayAverageOld = -1;

		m_LastDisplayed.ShowWindow(SW_HIDE);
		m_LastDisplayedLabel.ShowWindow(SW_HIDE);
		m_TotalUnder.ShowWindow(SW_HIDE);
		m_TotalUnderLabel.ShowWindow(SW_HIDE);
		m_TotalOver.ShowWindow(SW_HIDE);
		m_TotalOverLabel.ShowWindow(SW_HIDE);
		if (vPlotDisplay == cTrendDisplay)
		{
			m_Function2Button.ShowWindow(SW_SHOW);
			m_Function3Button.ShowWindow(SW_SHOW);
			SetDlgItemText(IDC_Function3Button, _T("Trend High Limit"));
		}
		else
		{
			m_Function2Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_Function2Button.GetRectToInvalidateArrow());
			m_Function3Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_Function3Button.GetRectToInvalidateArrow());
		}

		m_XLabel3.ShowWindow(SW_HIDE);
		m_XScale1.ShowWindow(SW_HIDE);
		m_XScale2.ShowWindow(SW_HIDE);
		m_XScale3.ShowWindow(SW_HIDE);
		m_XScale4.ShowWindow(SW_HIDE);
		m_XScale5.ShowWindow(SW_HIDE);
		m_XScale6.ShowWindow(SW_HIDE);
		m_XScale7.ShowWindow(SW_HIDE);
		m_XScale8.ShowWindow(SW_HIDE);
		m_XScale9.ShowWindow(SW_HIDE);
		m_XScale10.ShowWindow(SW_HIDE);
		m_XScale11.ShowWindow(SW_HIDE);
		m_XScale12.ShowWindow(SW_HIDE);
		m_XScale13.ShowWindow(SW_HIDE);
		m_XScale14.ShowWindow(SW_HIDE);
		m_YScale1.ShowWindow(SW_HIDE);
		m_YScale2.ShowWindow(SW_HIDE);
		m_YScale3.ShowWindow(SW_HIDE);
		m_YScale4.ShowWindow(SW_HIDE);
		m_YScale5.ShowWindow(SW_HIDE);
		m_YScale6.ShowWindow(SW_HIDE);
		m_YScale7.ShowWindow(SW_HIDE);
		m_YScale8.ShowWindow(SW_HIDE);
		m_YScale9.ShowWindow(SW_HIDE);
		m_YScale10.ShowWindow(SW_HIDE);
		m_YScale11.ShowWindow(SW_HIDE);
		m_YScale12.ShowWindow(SW_HIDE);
		m_YScale13.ShowWindow(SW_HIDE);
		m_YScale14.ShowWindow(SW_HIDE);
		m_YScale15.ShowWindow(SW_HIDE);

		if (vCheckWeighInspection->vCheckWeighAlarmLatched)
		{
			m_Function4Button.ShowWindow(SW_SHOW);
			m_AlarmIndicator.ShowWindow(SW_SHOW);
		}
		else
		{
			m_Function4Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_Function4Button.GetRectToInvalidateArrow());
			m_AlarmIndicator.ShowWindow(SW_HIDE);
		}
		m_Count.ShowWindow(SW_HIDE);
		m_CountLabel.ShowWindow(SW_HIDE);

		m_LastLabel.ShowWindow(SW_HIDE);

		if (vPlotDisplay == cTrendMinuteDisplay)
			SetDlgItemText(IDC_Function5Button, _T("Show\nTrend"));
		else
			SetDlgItemText(IDC_Function5Button, _T("Show\nRecent"));

		if ((vCheckWeighInspection->vCheckWeighBFactor) || (vCheckWeighInspection->vCheckWeighMFactor != 1))
		{	
			if (PasswordOK(cTemporaryInspxPassword,false))
			{
				m_ConversionFormula.ShowWindow(SW_SHOW);
				SetDlgItemText(IDC_ConversionFormula, vCheckWeighInspection->vCheckWeighUnits + "=" + dtoa(vCheckWeighInspection->vCheckWeighMFactor, 6) + "X+" + dtoa(vCheckWeighInspection->vCheckWeighBFactor,2));
			}
			else
				m_ConversionFormula.ShowWindow(SW_HIDE);


			if (!vCheckWeighInspection->vCheckWeighTrendAlarmMinimum)
				SetDlgItemText(IDC_ConversionFormula, _T("Need to Set Low Limit"));
			else
			if (!vCheckWeighInspection->vCheckWeighTrendAlarmMaximum)
				SetDlgItemText(IDC_ConversionFormula, _T("Need to Set High Limit"));
		}
		else
		{
			SetDlgItemText(IDC_ConversionFormula, _T("Need to Evaluate"));
			m_ConversionFormula.ShowWindow(SW_SHOW);
		}

		if (vPlotDisplay == cTrendDisplay)
		if (!vCheckWeighInspection->vCheckWeighTrendNumberOfDataSamplesTaken)
		{
			m_MinimumTrendThreshold.ShowWindow(SW_HIDE);
			m_MaximumTrendThreshold.ShowWindow(SW_HIDE);
		}
		else
		{
			if (vCheckWeighInspection->vCheckWeighTrendAlarmMinimum)
				m_MinimumTrendThreshold.ShowWindow(SW_SHOW);
			else
				m_MinimumTrendThreshold.ShowWindow(SW_HIDE);

			if (vCheckWeighInspection->vCheckWeighTrendAlarmMaximum)
				m_MaximumTrendThreshold.ShowWindow(SW_SHOW);
			else
				m_MaximumTrendThreshold.ShowWindow(SW_HIDE);
		}
	}
	else
	if (vPlotDisplay == cRecentDisplay)
	{
		vNumberOfPointsToPlot = 0;

		vDisplayThresholdLower = 0;
		vDisplayThresholdUpper = 0;
		m_MinimumTrendThreshold.ShowWindow(SW_SHOW);
		m_MaximumTrendThreshold.ShowWindow(SW_SHOW);
		if (vCheckWeighInspection->vInspectionType == cUnderfillByWeightInspection)
		{
			vDisplayThresholdLower = vCheckWeighInspection->vDensityThresholdUpper; //threshold for too light
			BYTE TempInspectionLinkedToMe = FindInspectionLinkedToMe();
			if (TempInspectionLinkedToMe)
			{
				vDisplayThresholdUpper = vGlobalCurrentProduct->vInspection[TempInspectionLinkedToMe - 1]->vDensityThresholdLower; //threshold for too heavy
				SetDlgItemText(IDC_DialogTitleStaticText1, _T("Recent Weights"));
			}
			else
			{
				vDisplayThresholdUpper = 0;
				SetDlgItemText(IDC_DialogTitleStaticText1,vCheckWeighInspection->vName + " Recent Weights");
				m_MaximumTrendThreshold.ShowWindow(SW_HIDE);
			}
		}
		else
		if (vCheckWeighInspection->vInspectionType == cOverfillByWeightInspection)
		{
			vDisplayThresholdUpper = vCheckWeighInspection->vDensityThresholdLower; //threshold for too heavy 
			BYTE TempInspectionLinkedToMe = FindInspectionLinkedToMe();
			//if (TempInspectionLinkedToMe)
			//{
			//	vDisplayThresholdLower = vGlobalCurrentProduct->vInspection[TempInspectionLinkedToMe - 1]->vDensityThresholdUpper; //threshold for too light
			//	SetDlgItemText(IDC_DialogTitleStaticText1, "Recent Weights");
			//}
			//else
			{
				vDisplayThresholdLower = 0;
				SetDlgItemText(IDC_DialogTitleStaticText1,vCheckWeighInspection->vName + " Recent Weights");
				m_MinimumTrendThreshold.ShowWindow(SW_HIDE);
			}
		}

		vLastWeightOld = -1;
		vCheckWeighDataIndexOld = -1; //force screen update
		vCheckWeighTrendDataIndexOld = -1; //force screen update
		vCheckWeighTrendDataIndexOnMinutesOld = -1;

		SetDlgItemText(IDC_NumberOfAlarmsLabel, _T("Recent Under:"));
		vRecentUnderOld = -1; //force screen update

		SetDlgItemText(IDC_RunTimeLabel, _T("Recent Over:"));
		vRecentOverOld = -1; //force Screen Update

		SetDlgItemText(IDC_TotalUnderLabel, _T("Total Under:"));
		vTotalUnderOld = -1; //force screen update

		SetDlgItemText(IDC_TotalOverLabel, _T("Total Over:"));
		vTotalOverOld = -1; //force Screen Update

		SetDlgItemText(IDC_StartTimeLabel, _T("Recent Std Dev:"));
		vDisplayStandardDeviationOld = -1; //force Screen Update
		
		//SetDlgItemText(IDC_CountLabel, "Recent Samples: ");
		vCheckWeighDataSizeOld = -1; //force Screen Update
		m_Count.ShowWindow(SW_SHOW);
		m_CountLabel.ShowWindow(SW_SHOW);

		vDisplayAverageOld = -1;

		m_LastDisplayed.ShowWindow(SW_SHOW);
		m_LastDisplayedLabel.ShowWindow(SW_SHOW);
		m_TotalUnder.ShowWindow(SW_SHOW);
		m_TotalUnderLabel.ShowWindow(SW_SHOW);
		m_TotalOver.ShowWindow(SW_SHOW);
		m_TotalOverLabel.ShowWindow(SW_SHOW);
		m_Function2Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_Function2Button.GetRectToInvalidateArrow());

		m_Function3Button.ShowWindow(SW_SHOW);
		this->InvalidateRect(&m_Function3Button.GetRectToInvalidateArrow());
		SetDlgItemText(IDC_Function3Button, _T("Weight\nOffset"));

		m_Function4Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_Function4Button.GetRectToInvalidateArrow());

		m_LastLabel.ShowWindow(SW_SHOW);

		SetDlgItemText(IDC_Function5Button, _T("Show\nMinute"));

		m_AlarmIndicator.ShowWindow(SW_SHOW); //to display last weight
		m_ConversionFormula.ShowWindow(SW_HIDE);

		if (!vCheckWeighInspection->vCheckWeighNumberOfDataSamplesTaken)
		{
			m_MinimumTrendThreshold.ShowWindow(SW_HIDE);
			m_MaximumTrendThreshold.ShowWindow(SW_HIDE);
		}
		else
		{
			if (vDisplayThresholdLower)
				m_MinimumTrendThreshold.ShowWindow(SW_SHOW);
			else
				m_MinimumTrendThreshold.ShowWindow(SW_HIDE);

			if (vDisplayThresholdUpper)
				m_MaximumTrendThreshold.ShowWindow(SW_SHOW);
			else
				m_MaximumTrendThreshold.ShowWindow(SW_HIDE);
		}
	}
	UpdatePlot();
}

void CCheckWeighDialog::UpdateScreen()
{
	if (vPlotDisplay != vPlotDisplayOld)
	{
		vPlotDisplayOld = vPlotDisplay;
		SetupDisplays();
	}

	if (vOldTotalContainers != vLocalSystemData->vTotalContainers)
	{
		vOldTotalContainers = vLocalSystemData->vTotalContainers;
		if (vOldTotalContainers)
			SetDlgItemText(IDC_Total, dtoa(vOldTotalContainers, 0));
		else
			SetDlgItemText(IDC_Total, _T("--"));
	}

	if (vPlotDisplay)  //either Trend Display
	{
		if (vPlotDisplay == cTrendDisplay)  //Trend Display
		{
			if (vCheckWeighInspection->vCheckWeighAlarmLatched != vCheckWeighInspection->vCheckWeighAlarmLatchedOld)
			{
				vCheckWeighInspection->vCheckWeighAlarmLatchedOld = vCheckWeighInspection->vCheckWeighAlarmLatched;
				if (vCheckWeighInspection->vCheckWeighAlarmLatchedOld)
				{
					SetDlgItemText(IDC_AlarmIndicator, _T("Alarm"));
					m_AlarmIndicator.ShowWindow(SW_SHOW);
					m_Function4Button.ShowWindow(SW_SHOW);
				}
				else
				{
					m_AlarmIndicator.ShowWindow(SW_HIDE);
					m_Function4Button.ShowWindow(SW_HIDE);
					this->InvalidateRect(&m_Function4Button.GetRectToInvalidateArrow());
				}
			}
		}
		else
		{
			m_AlarmIndicator.ShowWindow(SW_HIDE);
			m_Function4Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_Function4Button.GetRectToInvalidateArrow());
		}

		if (vPlotDisplay == cTrendDisplay)
		{
			if (vCheckWeighInspection->vCheckWeighTrendNumberOfDataSamplesTaken)
			if (vCheckWeighTrendDataIndexOld != vCheckWeighInspection->vCheckWeighTrendDataIndex)
			{
				vCheckWeighTrendDataIndexOld = vCheckWeighInspection->vCheckWeighTrendDataIndex;
				vDisplayNumberOfPointsToPlot = 0;
				vDisplayAverage = 0;
				//double *TempSource = vCheckWeighInspection->vCheckWeighTrendData;
				//for (DWORD TempLoop = 0; TempLoop < vCheckWeighInspection->vCheckWeighTrendNumberOfDataSamplesTaken; TempLoop++)
				//{
				//	if (*TempSource < vMinimumValue)
				//		vMinimumValue = *TempSource;
				//	if (*TempSource > vMaximumValue)
				//		vMaximumValue = *TempSource;
				//	TempSource++;
				//}

				vDisplayStandardDeviation = CalculateStandardDeviation(vCheckWeighInspection->vCheckWeighTrendData, vCheckWeighInspection->vCheckWeighTrendNumberOfDataSamplesTaken, &vDisplayAverage);
				vDisplayAverage = ATOF(dtoa(vDisplayAverage, 2)); //clip to one decimal place to prevent screen update on insignificant changes

				//Get data ready to Plot trend data
				if (((vCheckWeighInspection->vCheckWeighBFactor) || (vCheckWeighInspection->vCheckWeighMFactor != 1)))
				{
					vDisplayNumberOfPointsToPlot = vCheckWeighInspection->vCheckWeighTrendNumberOfDataSamplesTaken;
					vDisplayDataPointer = vCheckWeighInspection->vCheckWeighTrendData;
				}
			}
			if ((vCheckWeighInspection->vCheckWeighTrendAlarmMinimum) && (vCheckWeighInspection->vCheckWeighTrendAlarmMaximum))
			{
				if (vLastDisplayedIndex != vCheckWeighInspection->vCheckWeighTrendDataIndex)
				{
					vLastDisplayedIndex = vCheckWeighInspection->vCheckWeighTrendDataIndex;
					PreparePlotData();
				}
			}
			m_NumberOfAlarmsLabel.ShowWindow(SW_SHOW);
			m_NumberOfAlarms.ShowWindow(SW_SHOW);
		}
		else
		{ //on minutes trend display
			if (vCheckWeighInspection->vCheckWeighTrendNumberOfDataSamplesTakenOnMinutes)
			if (vCheckWeighTrendDataIndexOnMinutesOld != vCheckWeighInspection->vCheckWeighTrendDataIndexOnMinutes)
			{
				vCheckWeighTrendDataIndexOnMinutesOld = vCheckWeighInspection->vCheckWeighTrendDataIndexOnMinutes;
				vDisplayNumberOfPointsToPlot = 0;
				vDisplayAverage = 0;
				vDisplayStandardDeviation = 0;
				WORD TempNumberOfPositiveMinuteDataPoints = 0;

				if (ThereIsEnoughMemory(vCheckWeighInspection->vCheckWeighTrendNumberOfDataSamplesTakenOnMinutes * sizeof(double), "Weight Trending Plot Data"))
				{
					double *TempWeightValues = (double *)malloc((vCheckWeighInspection->vCheckWeighTrendNumberOfDataSamplesTakenOnMinutes) * sizeof(double));

					if (TempWeightValues)
					{
						double *TempDestination = TempWeightValues;
						double *TempSource = vCheckWeighInspection->vCheckWeighTrendDataOnMinutes;
						for (DWORD TempLoop = 0; TempLoop < vCheckWeighInspection->vCheckWeighTrendNumberOfDataSamplesTakenOnMinutes; TempLoop++)
						{
							if (*TempSource)//don't want to include values that are zero
							{
								*TempDestination++ = *TempSource;
								TempNumberOfPositiveMinuteDataPoints++;
							}
							//if (*TempSource < vMinimumValue)
							//	vMinimumValue = *TempSource;
							//if (*TempSource > vMaximumValue)
							//	vMaximumValue = *TempSource;
							TempSource++;
						}

						vDisplayStandardDeviation = CalculateStandardDeviation(TempWeightValues, TempNumberOfPositiveMinuteDataPoints, &vDisplayAverage);
						vDisplayAverage = ATOF(dtoa(vDisplayAverage, 2)); //clip to one decimal place to prevent screen update on insignificant changes
						free(TempWeightValues);
					}
				}

				//Get data ready to Plot trend data
				if (((vCheckWeighInspection->vCheckWeighBFactor) || (vCheckWeighInspection->vCheckWeighMFactor != 1)))
				{
					vDisplayNumberOfPointsToPlot = vCheckWeighInspection->vCheckWeighTrendNumberOfDataSamplesTakenOnMinutes;
					vDisplayDataPointer = vCheckWeighInspection->vCheckWeighTrendDataOnMinutes;

					vDisplayIndex = vCheckWeighInspection->vCheckWeighTrendDataIndexOnMinutes;
				}
				m_MinimumTrendThreshold.ShowWindow(SW_HIDE);
				m_MaximumTrendThreshold.ShowWindow(SW_HIDE);

				if ((vCheckWeighInspection->vCheckWeighTrendAlarmMinimum) && (vCheckWeighInspection->vCheckWeighTrendAlarmMaximum))
				{
					if (vLastDisplayedIndex != vCheckWeighInspection->vCheckWeighTrendDataIndexOnMinutes)
					{
						vLastDisplayedIndex = vCheckWeighInspection->vCheckWeighTrendDataIndexOnMinutes;
						PreparePlotData();
					}
				}
				m_NumberOfAlarmsLabel.ShowWindow(SW_HIDE);
				m_NumberOfAlarms.ShowWindow(SW_HIDE);
			}
		}

		if (((vPlotDisplay == cTrendDisplay) && (vCheckWeighInspection->vCheckWeighTrendNumberOfDataSamplesTaken)) ||  //only if trend data take show these statistics
			((vPlotDisplay == cTrendMinuteDisplay) && (vCheckWeighInspection->vCheckWeighTrendNumberOfDataSamplesTakenOnMinutes)))
		{
			if (vCheckWeighInspection->vNumberOfAlarms != vNumberOfAlarmsOld)
			{
				vNumberOfAlarmsOld = vCheckWeighInspection->vNumberOfAlarms;
				SetDlgItemText(IDC_NumberOfAlarms, dtoa(vNumberOfAlarmsOld, 0));
			}

			if (vDisplayNumberOfPointsToPlot) //only have a value when calculate new one
			if (vDisplayAverage != vDisplayAverageOld)
			{
				vDisplayAverageOld = vDisplayAverage;
				SetDlgItemText(IDC_Average, dtoa(vCheckWeighInspection->ConvertToWeight(vDisplayAverageOld), 2) + vWeightAbreviation);
			}
		}
		else
		{
			//no data so everything dashes
			SetDlgItemText(IDC_Average, _T("--"));
			SetDlgItemText(IDC_NumberOfAlarms, _T("--"));
		}
				
		if (vCheckWeighInspection->vCheckWeighNumberOfDataSamplesTaken) //if any data taken show times
		{
			if (vRunDurationOld != vLocalSystemData->vCheckWeighRunDuration)
			{
				vRunDurationOld = vLocalSystemData->vCheckWeighRunDuration;
				if (vRunDurationOld)
					SetDlgItemText(IDC_RunTime, FormatRunTimeCounterString(vRunDurationOld));
			}

			if (vRunStartTimeOld != vLocalSystemData->vRunStartTime)
			{
				vRunStartTimeOld = vLocalSystemData->vRunStartTime;
				if (vRunStartTimeOld)
					SetDlgItemText(IDC_StartTime, vRunStartTimeOld);
			}
		}
		else
		{
			//no data so everything dashes
			SetDlgItemText(IDC_RunTime, _T("--"));
			SetDlgItemText(IDC_StartTime, _T("--"));
		}
		//UpdatePlot();
	}
	else
	if (vPlotDisplay == cRecentDisplay)
	{
		if (vLastWeightOld != vCheckWeighInspection->vLastWeight)
		{
			vLastWeightOld = vCheckWeighInspection->vLastWeight;
			if (vLastWeightOld)
				SetDlgItemText(IDC_AlarmIndicator, dtoa(vCheckWeighInspection->ConvertToWeight(vLastWeightOld), 2) + vWeightAbreviation); //Derek wants 2 digits
			else
				SetDlgItemText(IDC_AlarmIndicator, _T("--"));
		}

		if ((vCheckWeighInspection->vCheckWeighOffset) || (vLocalSystemData->vCheckWeighOffsetCalibrationNeeded))
		{
			SetDlgItemText(IDC_CountLabel, _T("Offset: "));
			if (vLocalSystemData->vCheckWeighOffsetCalibrationNeeded)
			{
				SetDlgItemText(IDC_Count, " " + dtoa(vCheckWeighInspection->vCheckWeighOffset, 2) + "-Cal Req.");
				SetDlgItemText(IDC_CountLabel, _T("Cal Req-Offset:"));
			}
			else
			{
				SetDlgItemText(IDC_Count, " " + dtoa(vCheckWeighInspection->vCheckWeighOffset, 2));
				SetDlgItemText(IDC_CountLabel, _T("Offset:"));
			}
		}
		else
		{
			SetDlgItemText(IDC_CountLabel, _T(""));
			SetDlgItemText(IDC_Count, _T(""));
		}
		vDisplayAverage = 0;
		DWORD TempRecentUnder = 0;
		DWORD TempRecentOver = 0;
		vDisplayNumberOfPointsToPlot = 0;

		if (vCheckWeighInspection->vCheckWeighNumberOfDataSamplesTaken)
		if (vCheckWeighDataIndexOld != vCheckWeighInspection->vCheckWeighDataIndex)
		{
			vCheckWeighDataIndexOld = vCheckWeighInspection->vCheckWeighDataIndex;
			double *TempSource = vCheckWeighInspection->vCheckWeighData;
			for (DWORD TempLoop = 0; TempLoop < vCheckWeighInspection->vCheckWeighNumberOfDataSamplesTaken; TempLoop++)
			{
				//if (*TempSource < vMinimumValue)
				//	vMinimumValue = *TempSource;
				//if (*TempSource > vMaximumValue)
				//	vMaximumValue = *TempSource;
				if (vDisplayThresholdLower)
				if (*TempSource < vDisplayThresholdLower)
					TempRecentUnder++;
				if (vDisplayThresholdUpper)
				if (*TempSource > vDisplayThresholdUpper)
					TempRecentOver++;
				TempSource++;
			}

			vDisplayStandardDeviation = CalculateStandardDeviation(vCheckWeighInspection->vCheckWeighData, vCheckWeighInspection->vCheckWeighNumberOfDataSamplesTaken, &vDisplayAverage);
			vDisplayAverage = ATOF(dtoa(vDisplayAverage, 2)); //clip to one decimal place to prevent screen update on insignificant changes

			//Get data ready to Plot recent data
			if (((vCheckWeighInspection->vCheckWeighBFactor) || (vCheckWeighInspection->vCheckWeighMFactor != 1)))
			{
				vDisplayNumberOfPointsToPlot = vCheckWeighInspection->vCheckWeighNumberOfDataSamplesTaken;
				vDisplayDataPointer = vCheckWeighInspection->vCheckWeighData;

				vDisplayIndex = vCheckWeighInspection->vCheckWeighDataIndex;

				vLastDisplayedTime = CTime::GetCurrentTime();
				if ((vDisplayThresholdLower) || (vDisplayThresholdUpper))
				{
					if (vLastDisplayedIndex != vCheckWeighInspection->vCheckWeighDataIndex)
					{
						vLastDisplayedIndex = vCheckWeighInspection->vCheckWeighDataIndex;
						PreparePlotData();
					}
				}
			}
		}
		if (vCheckWeighInspection->vCheckWeighNumberOfDataSamplesTaken)
		{
			if (vLastDisplayedTimeOld != vLastDisplayedTime)
			{
				vLastDisplayedTimeOld = vLastDisplayedTime;
				SetDlgItemText(IDC_LastDisplayed, vLastDisplayedTimeOld.Format("%H:%M:%S   %d-%b-%y"));
			}

			if (vDisplayNumberOfPointsToPlot) //only have a value when calculate new one
			if (vRecentUnderOld != TempRecentUnder)
			{
				vRecentUnderOld = TempRecentUnder;
				SetDlgItemText(IDC_NumberOfAlarms, dtoa(vRecentUnderOld, 0));
			}

			if (vDisplayNumberOfPointsToPlot) //only have a value when calculate new one
			if (vRecentOverOld != TempRecentOver)
			{
				vRecentOverOld = TempRecentOver;
				SetDlgItemText(IDC_RunTime, dtoa(vRecentOverOld, 0));
			}

			if (vTotalUnderOld != vCheckWeighInspection->vTotalUnder)
			{
				vTotalUnderOld = vCheckWeighInspection->vTotalUnder;
				SetDlgItemText(IDC_TotalUnder, dtoa(vTotalUnderOld, 0));
			}

			if (vTotalOverOld != vCheckWeighInspection->vTotalOver)
			{
				vTotalOverOld = vCheckWeighInspection->vTotalOver;
				SetDlgItemText(IDC_TotalOver, dtoa(vTotalOverOld, 0));
			}

			if (vDisplayNumberOfPointsToPlot) //only have a value when calculate new one
			if (vDisplayStandardDeviationOld != vDisplayStandardDeviation)
			{
				vDisplayStandardDeviationOld = vDisplayStandardDeviation;
				SetDlgItemText(IDC_StartTime, dtoa(vDisplayStandardDeviationOld * vCheckWeighInspection->vCheckWeighMFactor, 2) + vWeightAbreviation);
			}

			//if (vDisplayNumberOfPointsToPlot) //only have a value when calculate new one
			//if (vCheckWeighDataSizeOld != vDisplayNumberOfPointsToPlot)
			//{
			//	vCheckWeighDataSizeOld = vDisplayNumberOfPointsToPlot;
			//	SetDlgItemText(IDC_Count, dtoa(vCheckWeighDataSizeOld, 0));
			//}

			if (vDisplayNumberOfPointsToPlot) //only have a value when calculate new one
			if (vDisplayAverage != vDisplayAverageOld)
			{
				vDisplayAverageOld = vDisplayAverage;
				SetDlgItemText(IDC_Average, dtoa(vCheckWeighInspection->ConvertToWeight(vDisplayAverageOld), 2) + vWeightAbreviation);
			}
		}
		else
		{
			SetDlgItemText(IDC_Average, _T("--"));
			SetDlgItemText(IDC_LastDisplayed, _T("--"));
			SetDlgItemText(IDC_NumberOfAlarms, _T("--"));
			SetDlgItemText(IDC_TotalUnder, _T("--"));
			SetDlgItemText(IDC_TotalOver, _T("--"));
			SetDlgItemText(IDC_StartTime, _T("--"));
			if (vLocalSystemData->vCheckWeighOffsetCalibrationNeeded)
			{
				SetDlgItemText(IDC_Count, " " + dtoa(vCheckWeighInspection->vCheckWeighOffset, 2) + "-Cal Req.");
				SetDlgItemText(IDC_CountLabel, _T("Cal Req.-Offset:"));
			}
			else
			{
				SetDlgItemText(IDC_Count, _T(" --"));
				SetDlgItemText(IDC_CountLabel, _T("Offset:"));
			}
		}
		m_NumberOfAlarmsLabel.ShowWindow(SW_SHOW);
		m_NumberOfAlarms.ShowWindow(SW_SHOW);
	}
}

void CCheckWeighDialog::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	//blank graph display
	dc.SelectObject(vPlotBackGroundPen);
	for (WORD TempLoop = 0; TempLoop < ResizeYCoor(vBlankHeight); TempLoop++)
	{
		if (TempLoop == TempLoop / 100)
		if (vMainWindowPointer)
			vMainWindowPointer->TickleWatchDogs();
					
		dc.MoveTo(ResizeXCoor(vBlankLeft), ResizeYCoor(vBlankTop) + TempLoop);
		dc.LineTo(ResizeXCoor(vBlankWidth), ResizeYCoor(vBlankTop) + TempLoop);
	}

	if (vNumberOfPointsToPlot)
	if ((dc.m_ps.rcPaint.top) ||
		(dc.m_ps.rcPaint.bottom) ||
		(dc.m_ps.rcPaint.left) ||
		(dc.m_ps.rcPaint.right))
	if (vPlotMaximumX)
	if (vPlotMaximumX >= vPlotMinimumX)
	if (vPointerToAdjustedData)
	{
		if ((dc.m_ps.rcPaint.top == 0) &&
			(dc.m_ps.rcPaint.bottom >= (WORD)ResizeYCoor(475)) &&
			(dc.m_ps.rcPaint.left == 0) &&
			(dc.m_ps.rcPaint.right >= (WORD)ResizeXCoor(635)))
			vGrowingSoDontBlank = false;

		WORD TempXMiddle = 315;  //320
		WORD TempYMiddle = 175; //240
		WORD TempWidth = (WORD)(vPlotMaximumX * vXScale / 2.0);
		if (TempWidth > 298)
		{
			TempWidth = 298;
			ReportErrorMessage("CheckWeight Plot Error 5",cEMailInspx,32000);
		}
		//WORD TempHeight = (WORD)((vRangeYToShow - vPlotMinimumY) * vYScale / 2.0);
		WORD TempHalfHeight = (WORD)((vRangeYToShow) * vYScale / 2.0);
		if (TempHalfHeight * 2 > vGraphHeight)
		{
			TempHalfHeight = vGraphHeight / 2;
			ReportErrorMessage("CheckWeight Plot Error 6",cEMailInspx,32000);
		}
		WORD TempLeftSide = TempXMiddle - TempWidth;//200; //106;
		if (TempLeftSide < 8)
		{
			ReportErrorMessage("CheckWeight Plot Error 7",cEMailInspx,32000);
			TempLeftSide = 8;
		}
		WORD TempRightSide = TempXMiddle + TempWidth;//TempLeftSide + vPlotMaximumX;//529;
		WORD TempBottom = TempYMiddle + TempHalfHeight; //170; //408;
		WORD TempTop = TempYMiddle - TempHalfHeight;//TempBottom - (vYScale); //41;
		if (TempTop < vGraphTop)
		{
			ReportErrorMessage("CheckWeight Plot Error 8",cEMailInspx,32000);
			TempTop = vGraphTop;
		}
		//draw box around
		dc.SelectObject(vDarkBluePen);
		if (!vGrowingSoDontBlank)
		{
			//draw box around plot
			dc.MoveTo(ResizeXCoor(TempLeftSide - 2),(int)(ResizeYCoor(TempTop - 2)));
			dc.LineTo(ResizeXCoor(TempRightSide + 2),(int)(ResizeYCoor(TempTop - 2))); //top line

			dc.LineTo(ResizeXCoor(TempRightSide + 2),ResizeYCoor(TempBottom + 2)); //right line

			dc.LineTo(ResizeXCoor(TempLeftSide - 2),ResizeYCoor(TempBottom + 2)); //bottom line

			dc.LineTo(ResizeXCoor(TempLeftSide - 2),(int)ResizeYCoor(TempTop - 2)); //Left line
			
			//draw X scale lines
			if (vDrawHalfTicks)
			{
				for (DWORD TempLoop = 1; TempLoop < vXNumberOfScaleLines; TempLoop++)
				{
					WORD TempXPosition = (WORD)((double)TempLeftSide + ((double)TempLoop * (double)vXInterval * vXScale) - (vXScale / 2));
					if (TempXPosition < 638) //resolution of monitor display
					{
						if (TempLoop % 2 == 0)
							dc.MoveTo(ResizeXCoor(TempXPosition), ResizeYCoor(TempBottom - 10));
						else
							dc.MoveTo(ResizeXCoor(TempXPosition), ResizeYCoor(TempBottom - 2));
						dc.LineTo(ResizeXCoor(TempXPosition), ResizeYCoor(TempBottom + 6)); //lines along bottom
					}
				}
			}
			else
			{
				for (DWORD TempLoop = 0; TempLoop < vXNumberOfScaleLines; TempLoop++)
				{
					WORD TempXPosition = (WORD)((double)TempLeftSide + ((double)TempLoop * (double)vXInterval * vXScale) + (vXScale / 2));
					if (TempXPosition < 638) //resolution of monitor display
					{
						dc.MoveTo(ResizeXCoor(TempXPosition), ResizeYCoor(TempBottom - 2));
						dc.LineTo(ResizeXCoor(TempXPosition), ResizeYCoor(TempBottom + 6)); //lines along bottom
					}
				}
			}
			if (vMainWindowPointer)
				vMainWindowPointer->TickleWatchDogs();
					

			dc.SelectObject(vVeryLightBluePen);
			//draw faint horizontal lines
			for (DWORD TempLoop = 0; TempLoop < vYNumberOfScaleLines; TempLoop++)
			{
				WORD TempY = (WORD)(TempBottom - (((TempLoop) *  vYInterval) * vYScale));
				dc.MoveTo(ResizeXCoor(TempLeftSide), ResizeYCoor(TempY));
				dc.LineTo(ResizeXCoor(TempRightSide), ResizeYCoor(TempY)); //lines along Left side
			}

			dc.SelectObject(vDarkBluePen);
			//draw Y scale lines
			for (DWORD TempLoop = 0; TempLoop < vYNumberOfScaleLines; TempLoop++)
			{
				WORD TempY = (WORD)(TempBottom - (((TempLoop) *  vYInterval) * vYScale));
				dc.MoveTo(ResizeXCoor(TempLeftSide - 6), ResizeYCoor(TempY));
				dc.LineTo(ResizeXCoor(TempLeftSide + 2), ResizeYCoor(TempY)); //lines along Left side
			}
			if (vMainWindowPointer)
				vMainWindowPointer->TickleWatchDogs();
					
		}
		if (vPlotDisplay == cTrendDisplay) //Trend Display
		{
			WORD TempY = 0;
			if (vDisplayThresholdUpper)
			{ //draw Maximum threshold line
				dc.SelectObject(vLocalSystemData->vBlackPen);
				TempY = (WORD)(TempBottom - ((vCheckWeighInspection->ConvertToWeight(vDisplayThresholdUpper) - vPlotMinimumY) * vYScale));
				dc.MoveTo(ResizeXCoor(TempLeftSide - 1),ResizeYCoor(TempY));
				dc.LineTo(ResizeXCoor(TempRightSide + 1),ResizeYCoor(TempY));
			}

			//Set position of Maximum Threshold Label
			int TempStartX = 1;
			if (TempLeftSide > 60)
				TempStartX = TempLeftSide - (34);

			m_MaximumTrendThreshold.SetWindowPos(NULL, ResizeXCoor(TempRightSide + 6), ResizeYCoor(TempY - 8), ResizeXCoor(48), ResizeYCoor(16),SWP_NOZORDER);

			if (vDisplayThresholdLower)
			{ //draw Minimum threshold line
				TempY = (WORD)(TempBottom - ((vCheckWeighInspection->ConvertToWeight(vDisplayThresholdLower) - vPlotMinimumY) * vYScale));
				dc.MoveTo(ResizeXCoor(TempLeftSide - 1), ResizeYCoor(TempY));
				dc.LineTo(ResizeXCoor(TempRightSide + 1), ResizeYCoor(TempY)); 
			}

			//Set position of Minimum Threshold Label
			m_MinimumTrendThreshold.SetWindowPos(NULL, ResizeXCoor(TempRightSide + 6), ResizeYCoor(TempY - 8), ResizeXCoor(48), ResizeYCoor(16),SWP_NOZORDER);
		}

		//if (vPlotDisplay == cTrendMinuteDisplay) //Trend Display
		//{
		//	if (vDisplayThresholdUpper)
		//	{ //draw Maximum threshold line
		//		dc.SelectObject(vLocalSystemData->vBlackPen);
		//		WORD TempY = (WORD)(TempBottom - ((vCheckWeighInspection->ConvertToWeight(vDisplayThresholdUpper) - vPlotMinimumY) * vYScale));
		//		//dc.MoveTo(TempLeftSide - 1,TempY);
		//		//dc.LineTo(TempRightSide + 1,TempY);
		//		int TempDistance = TempRightSide - TempLeftSide + 2;
		//		int TempNumberOfSegments = TempDistance / 20;
		//		for (WORD TempLoop = 0; TempLoop < TempNumberOfSegments; TempLoop++)
		//		{
		//			dc.MoveTo(TempLeftSide - 1 + (TempLoop * 20) + 10,TempY);
		//			dc.LineTo(TempLeftSide - 1 + (TempLoop * 20) + 20,TempY);
		//		}
		//	}

		//	if (vDisplayThresholdLower)
		//	{ //draw Minimum threshold line
		//		WORD TempY = (WORD)(TempBottom - ((vCheckWeighInspection->ConvertToWeight(vDisplayThresholdLower) - vPlotMinimumY) * vYScale));
		//		//dc.MoveTo(TempLeftSide - 1,TempY);
		//		//dc.LineTo(TempRightSide + 1,TempY); 
		//		int TempDistance = TempRightSide - TempLeftSide + 2;
		//		int TempNumberOfSegments = TempDistance / 20;
		//		for (WORD TempLoop = 0; TempLoop < TempNumberOfSegments; TempLoop++)
		//		{
		//			dc.MoveTo(TempLeftSide - 1 + (TempLoop * 20) + 10,TempY);
		//			dc.LineTo(TempLeftSide - 1 + (TempLoop * 20) + 20,TempY);
		//		}
		//	}
		//}
		if (vPlotDisplay == cRecentDisplay)
		{
			WORD TempY = 0;
			if ((vDisplayThresholdUpper) && (vCheckWeighInspection->ConvertToWeight(vDisplayThresholdUpper) < vPlotMaximumY) && (vCheckWeighInspection->ConvertToWeight(vDisplayThresholdUpper) > vPlotMinimumY))
			{ //draw Maximum threshold line
				dc.SelectObject(vLocalSystemData->vBlackPen);
				TempY = (WORD)(TempBottom - ((vCheckWeighInspection->ConvertToWeight(vDisplayThresholdUpper) - vPlotMinimumY) * vYScale));
				dc.MoveTo(ResizeXCoor(TempLeftSide - 1), ResizeYCoor(TempY));
				dc.LineTo(ResizeXCoor(TempRightSide + 1), ResizeYCoor(TempY));
				m_MaximumTrendThreshold.ShowWindow(SW_SHOW);
				//Set position of Maximum Threshold Label
				int TempStartX = 1;
				if (TempLeftSide > 60)
					TempStartX = TempLeftSide - 34;
				m_MaximumTrendThreshold.SetWindowPos(NULL, ResizeXCoor(TempRightSide + 6), ResizeYCoor(TempY - 8), ResizeXCoor(48), ResizeYCoor(16), SWP_NOZORDER);
			}
			else
				m_MaximumTrendThreshold.ShowWindow(SW_HIDE);


			if ((vDisplayThresholdLower) && (vCheckWeighInspection->ConvertToWeight(vDisplayThresholdLower) < vPlotMaximumY) && (vCheckWeighInspection->ConvertToWeight(vDisplayThresholdLower) > vPlotMinimumY))
			{ //draw Minimum threshold line
				TempY = (WORD)(TempBottom - ((vCheckWeighInspection->ConvertToWeight(vDisplayThresholdLower) - vPlotMinimumY) * vYScale));
				dc.MoveTo(ResizeXCoor(TempLeftSide - 1),  ResizeYCoor(TempY));
				dc.LineTo(ResizeXCoor(TempRightSide + 1),  ResizeYCoor(TempY)); 
				m_MinimumTrendThreshold.ShowWindow(SW_SHOW);
				//Set position of Minimum Threshold Label
				m_MinimumTrendThreshold.SetWindowPos(NULL, ResizeXCoor(TempRightSide + 6), ResizeYCoor(TempY - 8), ResizeXCoor(48), ResizeYCoor(16), SWP_NOZORDER);
			}
			else
				m_MinimumTrendThreshold.ShowWindow(SW_HIDE);
		}

		if (vCheckWeighInspection->vCheckWeighNominalWeight)
		if ((vCheckWeighInspection->vCheckWeighNominalWeight > vPlotMinimumY) && (vCheckWeighInspection->vCheckWeighNominalWeight < vPlotMaximumY))
		{ //draw nominal weight line
			dc.SelectObject(vLocalSystemData->vMediumDarkGreenPen);
			WORD TempY = (WORD)(TempBottom - ((vCheckWeighInspection->ConvertToWeight(vCheckWeighInspection->vCheckWeighNominalWeight) - vPlotMinimumY) * vYScale));
			dc.MoveTo(ResizeXCoor(TempLeftSide - 1), ResizeYCoor(TempY));
			dc.LineTo(ResizeXCoor(TempRightSide + 1), ResizeYCoor(TempY));
		}
		//plot data
		if (vPointerToAdjustedData)
		{
			WORD TempStartDataPoint = 0;
			if (vGrowingSoDontBlank)
			if (vOldNumberOfPointsToPlot)
				TempStartDataPoint = vOldNumberOfPointsToPlot - 1;

			double *TempPointer = vPointerToAdjustedData + TempStartDataPoint;

			double TempDataPoint = 0;
			double TempPreviousDataPoint = 0;
			WORD TempDotOffsetX = 2;
			WORD TempDotOffsetY = 2;
			WORD TempDotSizeX = 5;
			WORD TempDotSizeY = 5;

				//Draw lines through data
			dc.SelectObject(vBluePen);
			for (WORD TempLoop = TempStartDataPoint; TempLoop < vNumberOfPointsToPlot; TempLoop++)
			{
				if (TempLoop == TempLoop / 100)
				if (vMainWindowPointer)
					vMainWindowPointer->TickleWatchDogs();
					
				TempDataPoint = *TempPointer;

				if (TempDataPoint > vPlotMaximumY)
					TempDataPoint = vPlotMaximumY;

				if (TempDataPoint < vPlotMinimumY)
					TempDataPoint = vPlotMinimumY;

				WORD TempX = (WORD)(TempLeftSide + ((TempLoop + vPlotMinuteOffsetWhereDataStarts) * vXScale) + (vXScale / 2));
				WORD TempY = (WORD)(TempBottom - (((TempDataPoint - vPlotMinimumY) * vYScale)));
				if (TempX >= TempRightSide)
				{
					//ReportErrorMessage("CheckWeight Plot Error 11",cEMailInspx,32000);
					TempX = TempRightSide;
				}
				//Draw lines through data
				if ((*TempPointer) && (TempPreviousDataPoint) && (TempLoop != TempStartDataPoint)) //if current point and previous point are not zero, then paint line, otherwise, start new line
					dc.LineTo(ResizeXCoor(TempX), ResizeYCoor(TempY)); 
				else
					dc.MoveTo(ResizeXCoor(TempX), ResizeYCoor(TempY));

				TempPreviousDataPoint = *TempPointer;
				TempPointer++;
			}
				//draw square dot around data if not off plot
			TempPointer = vPointerToAdjustedData + TempStartDataPoint;
			for (WORD TempLoop = TempStartDataPoint; TempLoop < vNumberOfPointsToPlot; TempLoop++)
			{
				if (TempLoop == TempLoop / 100)
				if (vMainWindowPointer)
					vMainWindowPointer->TickleWatchDogs();
					
				TempDataPoint = *TempPointer;

				if (TempDataPoint > vPlotMaximumY)
					TempDataPoint = vPlotMaximumY + .5;

				if (TempDataPoint < vPlotMinimumY)
					TempDataPoint = vPlotMinimumY - .5;

				if (vPlotDisplay == cRecentDisplay) //for recent data only, want a point at top or bottom of plot if off the top or bottom of plot
				{
					if (TempDataPoint > vPlotMaximumY)
						TempDataPoint = vPlotMaximumY;
					if (TempDataPoint <= vPlotMinimumY)
						TempDataPoint = vPlotMinimumY;
				}

				WORD TempX = (WORD)(TempLeftSide + ((TempLoop + vPlotMinuteOffsetWhereDataStarts) * vXScale) + (vXScale / 2));
				WORD TempY = (WORD)(TempBottom - (((TempDataPoint - vPlotMinimumY) * vYScale)));

				if (TempX >= TempRightSide)
				{
					//ReportErrorMessage("CheckWeight Plot Error 11",cEMailInspx,32000);
					TempX = TempRightSide;
				}

				//draw square dot around data if not off plot
				COLORREF TempColor = cDarkGreen;
				BYTE TempDotSize = 6;
				BYTE TempDotOffset = 3;

				if (((vDisplayThresholdUpper) && (TempDataPoint > vCheckWeighInspection->ConvertToWeight(vDisplayThresholdUpper))) ||
					((vDisplayThresholdUpper) && (TempDataPoint < vCheckWeighInspection->ConvertToWeight(vDisplayThresholdLower))))
				{
					TempColor = cRed;
					TempDotSize = 8;
					TempDotOffset = 4;
				}

				if (TempDataPoint <= vPlotMaximumY)
				if (TempDataPoint >= vPlotMinimumY)
				{
					for (DWORD TempLoopX = 0; TempLoopX < ResizeXCoor(TempDotSize); TempLoopX++)
					{
						if (TempLoopX == TempLoopX / 100)
						if (vMainWindowPointer)
							vMainWindowPointer->TickleWatchDogs();
					
						for (DWORD TempLoopY = 0; TempLoopY < ResizeYCoor(TempDotSize); TempLoopY++)
							dc.SetPixelV(ResizeXCoor(TempX - TempDotOffset) + TempLoopX, ResizeYCoor(TempY - TempDotOffset) + TempLoopY, TempColor);
					}
				}
				TempPointer++;
			}
		}
	}
	//SetDlgItemText(IDC_MaximumTrendThreshold, dtoa(vCheckWeighInspection->ConvertToWeight(vCheckWeighInspection->vCheckWeighTrendAlarmMaximum), 2));
}

void CCheckWeighDialog::UpdatePlot() //causes windows to re-draw the screen
{
	vLastDisplayedTime = CTime::GetCurrentTime();

	//Update Plot, Invalidate Plot
	RECT TempRectangle;
	TempRectangle.left = ResizeXCoor(vBlankLeft);
	TempRectangle.top = ResizeYCoor(vBlankTop);
	TempRectangle.right = ResizeXCoor(vBlankLeft + vBlankWidth);
	TempRectangle.bottom = ResizeYCoor(vBlankTop + vBlankHeight);
	this->InvalidateRect(&TempRectangle,false);
}

void CCheckWeighDialog::PreparePlotData()
{
	vPlotMinuteOffsetWhereDataStarts = 0;
	if (vPlotDisplay == cRecentDisplay)
	{
		SetDlgItemText(IDC_MinimumTrendThreshold, _T("Under"));
		SetDlgItemText(IDC_MaximumTrendThreshold, _T("Over"));
	}
	else
	if (vPlotDisplay == cTrendDisplay)
	{
		SetDlgItemText(IDC_MinimumTrendThreshold, dtoa(vCheckWeighInspection->ConvertToWeight(vDisplayThresholdLower), 2));
		SetDlgItemText(IDC_MaximumTrendThreshold, dtoa(vCheckWeighInspection->ConvertToWeight(vDisplayThresholdUpper), 2));
	}

	vPlotMinimumY = 999999999;
	vPlotMaximumY = 0;
	vPlotMinimumX = 1;
	vOldNumberOfPointsToPlot = vNumberOfPointsToPlot;
	vNumberOfPointsToPlot = vDisplayNumberOfPointsToPlot;

	vGrowingSoDontBlank = false;
	//first, copy data to a temporary array so in order of most recent data on left
	vOldDivideFactor = vDivideFactor;

	int TempIndex = vDisplayIndex;
	bool TempShowUnits = 	((vCheckWeighInspection->vCheckWeighBFactor) || (vCheckWeighInspection->vCheckWeighMFactor != 1));

	if (vNumberOfPointsToPlot <= 1200)
	{
		vDivideFactor = 1;
		if (ThereIsEnoughMemory(vNumberOfPointsToPlot * sizeof(double), "Weight Trending Plot Data"))
		{
			if (vPointerToAdjustedData)
				free(vPointerToAdjustedData);
			vPointerToAdjustedData = NULL;

			vPointerToAdjustedData = (double *)malloc((vNumberOfPointsToPlot) * sizeof(double));

			double *TempDestinationPointer = vPointerToAdjustedData;
			if (!TempIndex)
				TempIndex = vNumberOfPointsToPlot - 1;
			else
				TempIndex = TempIndex - 1;

			//start at oldest data
			double *TempSourcePointer = vDisplayDataPointer;

			if (((vPlotDisplay == cRecentDisplay) && (vNumberOfPointsToPlot == vCheckWeighInspection->vCheckWeighDataSize)) || 
				((vPlotDisplay == cTrendDisplay) && (vNumberOfPointsToPlot == vCheckWeighInspection->vCheckWeighTrendDataSize)) || 
				((vPlotDisplay == cTrendMinuteDisplay) && (vNumberOfPointsToPlot == vCheckWeighInspection->vCheckWeighTrendDataSizeOnMinutes)))
			{
				TempIndex++;
				TempSourcePointer = vDisplayDataPointer + TempIndex;
			}
			else
			{
				TempIndex = 0;
				TempSourcePointer = vDisplayDataPointer;
			}

			for (WORD TempLoop = 0; TempLoop < vNumberOfPointsToPlot; TempLoop++)
			{
				if ((TempShowUnits) && (*TempSourcePointer))
					*TempDestinationPointer = (*TempSourcePointer * vCheckWeighInspection->vCheckWeighMFactor) + vCheckWeighInspection->vCheckWeighBFactor;
				else
					*TempDestinationPointer = *TempSourcePointer;

				TempDestinationPointer++;
				TempSourcePointer++;

				TempIndex++;
				if (TempIndex >= vDisplayNumberOfPointsToPlot)
				{
					TempSourcePointer = vDisplayDataPointer;
					TempIndex = 0;
				}
			}
		}
	}
	else  //more than 1200 cells to plot, so divide down and average cell so between 600 and 1200
	{
		vDivideFactor = vNumberOfPointsToPlot / 600;
		WORD TempNewNumberOfPointsToPlot = vNumberOfPointsToPlot / vDivideFactor;
		if (ThereIsEnoughMemory(TempNewNumberOfPointsToPlot * sizeof(double), "Weight Trending Plot Data"))
		{
			if (vPointerToAdjustedData)
				free(vPointerToAdjustedData);
			vPointerToAdjustedData = NULL;

			vPointerToAdjustedData = (double *)malloc((TempNewNumberOfPointsToPlot) * sizeof(double));

			double *TempDestinationPointer = vPointerToAdjustedData;

			//start at oldest data
			double *TempSourcePointer = vDisplayDataPointer;

			//if data memory is full, start at the index so oldest data is first displayed
			if (((vPlotDisplay == cRecentDisplay) && (vNumberOfPointsToPlot == vCheckWeighInspection->vCheckWeighDataSize)) || 
				((vPlotDisplay == cTrendDisplay) && (vNumberOfPointsToPlot == vCheckWeighInspection->vCheckWeighTrendDataSize)) || 
				((vPlotDisplay == cTrendMinuteDisplay) && (vNumberOfPointsToPlot == vCheckWeighInspection->vCheckWeighTrendDataSizeOnMinutes)))
			{
				TempIndex++;
				TempSourcePointer = vDisplayDataPointer + TempIndex;
			}
			else
			{
				TempIndex = 0;
				TempSourcePointer = vDisplayDataPointer;
			}

			for (WORD TempLoop = 0; TempLoop < TempNewNumberOfPointsToPlot; TempLoop++)
			{
				double TempSum = 0;

				for (WORD TempLoopA = 0; TempLoopA < vDivideFactor; TempLoopA++)
				{
					if (TempShowUnits)
						TempSum = TempSum + (*TempSourcePointer * vCheckWeighInspection->vCheckWeighMFactor) + vCheckWeighInspection->vCheckWeighBFactor;
					else
						TempSum = TempSum + *TempSourcePointer;
					TempSourcePointer++;
					TempIndex++;
					if (TempIndex >= vDisplayNumberOfPointsToPlot)
					{
						TempSourcePointer = vDisplayDataPointer;
						TempIndex = 0;
					}
				}

				*TempDestinationPointer = TempSum / vDivideFactor;

				TempDestinationPointer++;

			}
			vNumberOfPointsToPlot = TempNewNumberOfPointsToPlot;
		}
	}

	if (vPointerToAdjustedData)
	if (vDisplayNumberOfPointsToPlot)
	{
		//if (vWatchGrow) //true
		//{
		//	if (vOldDivideFactor != vDivideFactor)
		//		vPlotMaximumX = 0;

		//	if (!vPlotMaximumX)
		//		vPlotMaximumX = (WORD)(vNumberOfPointsToPlot * 1.2) + 20;
		//	else
		//	{
		//		if (vPlotMaximumX < (WORD)vNumberOfPointsToPlot)
		//			vPlotMaximumX = (WORD)(vNumberOfPointsToPlot * 1.2) + 20;
		//		else
		//			vGrowingSoDontBlank = true;
		//	}
		//}
		//else
			vPlotMaximumX = (WORD)(vNumberOfPointsToPlot);

		WORD TempMaximumDataSamplesPossible = vDisplayNumberOfPointsToPlot;
		if (vDivideFactor > 1)
			TempMaximumDataSamplesPossible = vDisplayNumberOfPointsToPlot / vDivideFactor;

		if (vPlotMaximumX >= TempMaximumDataSamplesPossible)
		{
			vPlotMaximumX = TempMaximumDataSamplesPossible;
			if (vOldNumberOfPointsToPlot = vNumberOfPointsToPlot)
			if (vNumberOfPointsToPlot == TempMaximumDataSamplesPossible)
				vGrowingSoDontBlank = false;
		}

		if (vOldDivideFactor != vDivideFactor)
			vGrowingSoDontBlank = false;

		//SetDlgItemText(IDC_TotalSamples,"Total Samples: " + dtoa(vDisplayNumberOfPointsToPlot,0));

		double *TempPointer = vPointerToAdjustedData;
		for (WORD TempLoop = 0; TempLoop < vNumberOfPointsToPlot; TempLoop++)
		{  //find the minimum and maximum values of the data
			if (*TempPointer < vPlotMinimumY)
				vPlotMinimumY = *TempPointer;
			if (*TempPointer > vPlotMaximumY)
				vPlotMaximumY = *TempPointer;
			TempPointer++;
		}

		//if (vPlotMaximumY == vPlotMinimumY)
			vPlotMaximumY = vPlotMaximumY + 1;

		if (vPlotMaximumY < vPlotMinimumY + 4)
			vPlotMaximumY = vPlotMinimumY + 4;

		double TempQuarterPlotY = (vPlotMaximumY - vPlotMinimumY) / 4;

		if ((vDisplayThresholdUpper) && (vDisplayThresholdLower))
		{
			TempQuarterPlotY = (vCheckWeighInspection->ConvertToWeight(vDisplayThresholdUpper) - vCheckWeighInspection->ConvertToWeight(vDisplayThresholdLower)) / 2;
			vPlotMaximumY = vCheckWeighInspection->ConvertToWeight(vDisplayThresholdUpper) + TempQuarterPlotY;
			vPlotMinimumY = vCheckWeighInspection->ConvertToWeight(vDisplayThresholdLower) - TempQuarterPlotY;
			if (vPlotMinimumY < 0)
				vPlotMinimumY = 0;
			vPlotMaximumY = int(vPlotMaximumY + 2);
			vPlotMinimumY = int(vPlotMinimumY - .001);
		}
		else
		{
			vPlotMaximumY = int(vPlotMaximumY + 1);
			vPlotMinimumY = int(vPlotMinimumY - 1);
		}

		if (!vGrowingSoDontBlank)
		if (vPlotMaximumY >= vPlotMinimumY)
		{
			if (vNumberOfPointsToPlot)
			//if (ThereIsEnoughMemory(vNumberOfPointsToPlot * 8,"CheckWeight Plot Data Buffer"))
			{  //create a temporary array to contain the number of occurrences of each possible value
				double TempTimesFactor = 1;
				if (vPlotDisplay == cTrendDisplay)
					TempTimesFactor = TempTimesFactor * vCheckWeighInspection->vCheckWeighDataSize;

				if (vPlotDisplay == cTrendMinuteDisplay) //new way for alex show whole hours
				{
					vDrawHalfTicks = true;
					vRangeYToShow = (WORD)(vPlotMaximumY - vPlotMinimumY);

					if (vPlotMaximumY < vPlotMinimumY)
						vRangeYToShow = (WORD)vPlotMinimumY;
					//plot the data on the screen
					//x dimension is  top=41 bottom = 408 height = 367; vPlotMinimumX to vPlotMaximumX
					//y dimension is  left = 106 right=529 width= 423; vMinimumY to vMaximumY
					//vXScale = 423.0 / vNumberOfPointsToPlot;

					WORD TempRunDurationInMinutes = (vLocalSystemData->vCheckWeighRunDuration / 60) % 60; 
					WORD TempRunDurationInHours = (WORD)(vLocalSystemData->vCheckWeighRunDuration / 60 / 60); 
					vPlotMinuteOffsetWhereDataStarts = _wtoi(vLocalSystemData->vTimeStarted.Format("%M"));

					vPlotMaximumX = TempRunDurationInHours * 60; //vPlotMaximumX is in minutes
					if (TempRunDurationInMinutes) //if did not start on the first minute of the hour, add an hour in time to see the next hour.
						vPlotMaximumX = vPlotMaximumX + 60;
					if (TempRunDurationInMinutes + vPlotMinuteOffsetWhereDataStarts > 60) //if did not start plus remainder minutes of an even hour, then go into the next hour.
						vPlotMaximumX = vPlotMaximumX + 60;
					if (vPlotMaximumX < 60)
						vPlotMaximumX = 60; //minimum time in x direction is one hour //should never hapen as code above should give minimum of 60

					CTimeSpan TempBlankSecondsAtStartOfPlot = vPlotMinuteOffsetWhereDataStarts * 60;
					CTime TempTimePlotStarts = vLocalSystemData->vTimeStarted - TempBlankSecondsAtStartOfPlot;

					vXNumberOfScaleLines = 4; //for 1 hour

					if (vPlotMaximumX >= 120)  //for 2 or more hours
						vXNumberOfScaleLines = 8;

					if (((WORD)vPlotMaximumX) % 180 == 0)  //when hours are divisible by 3
						vXNumberOfScaleLines = 6;

					if (((WORD)vPlotMaximumX) % 300 == 0)  //when hours are divisible by 5
						vXNumberOfScaleLines = 10;

					if (((WORD)vPlotMaximumX) % 420 == 0)  //when hours are divisible by 7
						vXNumberOfScaleLines = 7;

					if (((WORD)vPlotMaximumX) % 540 == 0)  //when hours are divisible by 9
						vXNumberOfScaleLines = 9;

					if (((WORD)vPlotMaximumX) % 660 == 0)  //when hours are divisible by 11
						vXNumberOfScaleLines = 11;

					if (((WORD)vPlotMaximumX) % 760 == 0)  //when hours are divisible by 13
						vXNumberOfScaleLines = 13;

					vXScale = vGraphWidth / vPlotMaximumX;
					//vYScale = 367.0 / (vMaximumY - vMinimumY);
					if (vRangeYToShow > 5)
						vYScale = (double)vGraphHeight / (double)(vRangeYToShow);  //was 300
					else
						vYScale = 45;
					if (vXScale > 45)
						vXScale = 45;
					if (vYScale > 50)
						vYScale = 50;
					//vXScaleWord = (WORD)(vXScale);  //truncate
					//vXInterval = (WORD)((vPlotMaximumX / 10.0) + .5);
					vXInterval = (WORD)((vPlotMaximumX / (vXNumberOfScaleLines)) + .5);

					//if (vPlotDisplay == cTrendMinuteDisplay)
					//	vXInterval = (WORD)((vPlotMaximumX / 6.0) + .5);

					vYInterval = (WORD)(((vRangeYToShow) / 10.0) + .5);
					//if (!vXInterval)
					//	vXInterval = 1;
					if (!vYInterval)
						vYInterval = 1;
					//vXNumberOfScaleLines = (int)(((vPlotMaximumX) / vXInterval));

					//if (vPlotDisplay == cTrendMinuteDisplay)
					//if (vXNumberOfScaleLines > 12)
					//	vXNumberOfScaleLines = 12;

					vYNumberOfScaleLines = (WORD)((vRangeYToShow) / vYInterval + 1);

					////if ((vXNumberOfScaleLines * vXInterval) + vPlotMinimumX < vPlotMaximumX)
					////	vXNumberOfScaleLines++;

					SetDlgItemText(IDC_LineLabel3,"Y Scale: " + dtoa(vYInterval,0));

					SetDlgItemText(IDC_XLabel2,"X Scale: " + dtoa(vPlotMaximumX / (vXNumberOfScaleLines), 0));

					WORD TempXMiddle = 315;  //320
					WORD TempYMiddle = vGraphTop + (vGraphHeight / 2);
					//WORD TempWidth = (WORD)(vPlotMaximumX * vXScale / 2.0);
					WORD TempWidth = vGraphWidth / 2; //use the whole width as minimum time is an hour
					if (TempWidth > vGraphWidth)
					{
						TempWidth = vGraphWidth;
						ReportErrorMessage("CheckWeight Plot Error 5",cEMailInspx,32000);
					}
					WORD TempHeight = (WORD)((vRangeYToShow) * vYScale);
					if (TempHeight > vGraphHeight)
					{
						TempHeight = vGraphHeight;
						ReportErrorMessage("CheckWeight Plot Error 6",cEMailInspx,32000);
					}
					WORD TempLeftSide = TempXMiddle - TempWidth;//200; //106;
					if (TempLeftSide < 8)
					{
						//ReportErrorMessage("CheckWeight Plot Error 7",cEMailInspx,32000);
						TempLeftSide = 8;
					}
					WORD TempRightSide = TempXMiddle + TempWidth;//TempLeftSide + vPlotMaximumX;//529;
					WORD TempBottom = (WORD)(TempYMiddle + (TempHeight / 2.0)); //170; //408;
					WORD TempTop = (WORD)(TempYMiddle - (TempHeight / 2.0));//TempBottom - (vYScale); //41;
					if (TempTop < vGraphTop)
					{
						ReportErrorMessage("CheckWeight Plot Error 8",cEMailInspx,32000);
						TempTop = vGraphTop;
					}

					//m_LineLabel3.SetWindowPos(NULL,10,10,110,20,SWP_NOZORDER);

					m_XScale1.ShowWindow(SW_HIDE);
					m_XScale2.ShowWindow(SW_HIDE);
					m_XScale3.ShowWindow(SW_HIDE);
					m_XScale4.ShowWindow(SW_HIDE);
					m_XScale5.ShowWindow(SW_HIDE);
					m_XScale6.ShowWindow(SW_HIDE);
					m_XScale7.ShowWindow(SW_HIDE);
					m_XScale8.ShowWindow(SW_HIDE);
					m_XScale9.ShowWindow(SW_HIDE);
					m_XScale10.ShowWindow(SW_HIDE);
					m_XScale11.ShowWindow(SW_HIDE);
					m_XScale12.ShowWindow(SW_HIDE);
					m_XScale13.ShowWindow(SW_HIDE);
					m_XScale14.ShowWindow(SW_HIDE);

					//fill in scale values
					//if (!vPlotMinimumX)
					//	vPlotMinimumX = 1;
					WORD TempXPosition = 0;
					DWORD TempXValue = 0;
					for (int TempLoop = 0; TempLoop < vXNumberOfScaleLines + 1; TempLoop++)
					{

						TempXValue = (DWORD)((TempLoop * vXInterval) + vPlotMinimumX);

						//if (vPlotDisplay == cTrendMinuteDisplay) //if more than 500 minutes of data, correct scale numbers shown as compressed the data
						//if (vPlotMaximumX > 500)
						//	TempXValue = (DWORD)(TempXValue * (vPlotMaximumX / 500));

						if (TempXValue > vPlotMaximumX)
							TempXValue = (DWORD)vPlotMaximumX;

						if (TempXValue > 1)
							TempXValue = TempXValue * vDivideFactor;

						WORD TempXSize = 42;
						if (TempXValue > 9)
							TempXSize = 18;
						if (TempXValue > 99)
							TempXSize = 27;
						if (TempXValue > 999)
							TempXSize = 36;
						if (TempXValue > 9999)
							TempXSize = 42;
						if (vPlotDisplay == cTrendMinuteDisplay)
							TempXSize = 42;

						TempXPosition = (WORD)((double)TempLeftSide + ((double)TempLoop * (double)vXInterval * vXScale) + (vXScale / 2) - (TempXSize / 2));

						if (TempXPosition > 635 - TempXSize)
							TempXPosition = 635 - TempXSize;

						switch (TempLoop)
						{
							case 0:
							{
								m_XScale1.ShowWindow(SW_SHOW);
								m_XScale1.SetWindowPos(NULL, ResizeXCoor(TempXPosition), ResizeYCoor(TempBottom + 12), ResizeXCoor(TempXSize), ResizeYCoor(16), SWP_NOZORDER);

								unsigned long int TempTimeInterval = (unsigned long)((vPlotMaximumX * 60) / vXNumberOfScaleLines);  //seconds displayed across the chart by number of intervals
								CTimeSpan TempOffset = (TempLoop * TempTimeInterval); //TempOffset is seconds since start to this tick mark
								CTime TempIndexTime = TempTimePlotStarts + TempOffset;
								CString TempTimeString = TempIndexTime.Format("%H:%M");
								SetDlgItemText(IDC_XScale1, TempTimeString);
							}
							break;
							case 1:
							{
								m_XScale2.ShowWindow(SW_SHOW);
								m_XScale2.SetWindowPos(NULL, ResizeXCoor(TempXPosition), ResizeYCoor(TempBottom + 12), ResizeXCoor(TempXSize), ResizeYCoor(16), SWP_NOZORDER);

								unsigned long int TempTimeInterval = (unsigned long)((vPlotMaximumX * 60) / vXNumberOfScaleLines);  //seconds displayed across the chart by number of intervals
								CTimeSpan TempOffset = (TempLoop * TempTimeInterval); //TempOffset is seconds since start to this tick mark
								CTime TempIndexTime = TempTimePlotStarts + TempOffset;
								CString TempTimeString = TempIndexTime.Format("%H:%M");
								SetDlgItemText(IDC_XScale2, TempTimeString);
							}
							break;
							case 2:
							{
								m_XScale3.ShowWindow(SW_SHOW);
								m_XScale3.SetWindowPos(NULL, ResizeXCoor(TempXPosition), ResizeYCoor(TempBottom + 12), ResizeXCoor(TempXSize), ResizeYCoor(16), SWP_NOZORDER);

								unsigned long int TempTimeInterval = (unsigned long)((vPlotMaximumX * 60) / vXNumberOfScaleLines);  //seconds displayed across the chart by number of intervals
								CTimeSpan TempOffset = (TempLoop * TempTimeInterval); //TempOffset is seconds since start to this tick mark
								CTime TempIndexTime = TempTimePlotStarts + TempOffset;
								CString TempTimeString = TempIndexTime.Format("%H:%M");
								SetDlgItemText(IDC_XScale3, TempTimeString);
							}
							break;
							case 3:
							{
								m_XScale4.ShowWindow(SW_SHOW);
								m_XScale4.SetWindowPos(NULL, ResizeXCoor(TempXPosition), ResizeYCoor(TempBottom + 12), ResizeXCoor(TempXSize), ResizeYCoor(16), SWP_NOZORDER);

								unsigned long int TempTimeInterval = (unsigned long)((vPlotMaximumX * 60) / vXNumberOfScaleLines);  //seconds displayed across the chart by number of intervals
								CTimeSpan TempOffset = (TempLoop * TempTimeInterval); //TempOffset is seconds since start to this tick mark
								CTime TempIndexTime = TempTimePlotStarts + TempOffset;
								CString TempTimeString = TempIndexTime.Format("%H:%M");
								SetDlgItemText(IDC_XScale4, TempTimeString);
							}
							break;
							case 4:
							{
								m_XScale5.ShowWindow(SW_SHOW);
								m_XScale5.SetWindowPos(NULL, ResizeXCoor(TempXPosition), ResizeYCoor(TempBottom + 12), ResizeXCoor(TempXSize), ResizeYCoor(16), SWP_NOZORDER);

								unsigned long int TempTimeInterval = (unsigned long)((vPlotMaximumX * 60) / vXNumberOfScaleLines);  //seconds displayed across the chart by number of intervals
								CTimeSpan TempOffset = (TempLoop * TempTimeInterval); //TempOffset is seconds since start to this tick mark
								CTime TempIndexTime = TempTimePlotStarts + TempOffset;
								CString TempTimeString = TempIndexTime.Format("%H:%M");
								SetDlgItemText(IDC_XScale5, TempTimeString);
							}
							break;
							case 5:
							{
								m_XScale6.ShowWindow(SW_SHOW);
								m_XScale6.SetWindowPos(NULL, ResizeXCoor(TempXPosition), ResizeYCoor(TempBottom + 12), ResizeXCoor(TempXSize), ResizeYCoor(16), SWP_NOZORDER);

								unsigned long int TempTimeInterval = (unsigned long)((vPlotMaximumX * 60) / vXNumberOfScaleLines);  //seconds displayed across the chart by number of intervals
								CTimeSpan TempOffset = (TempLoop * TempTimeInterval); //TempOffset is seconds since start to this tick mark
								CTime TempIndexTime = TempTimePlotStarts + TempOffset;
								CString TempTimeString = TempIndexTime.Format("%H:%M");
								SetDlgItemText(IDC_XScale6, TempTimeString);
							}
							break;
							case 6:
							{
								m_XScale7.ShowWindow(SW_SHOW);
								m_XScale7.SetWindowPos(NULL, ResizeXCoor(TempXPosition), ResizeYCoor(TempBottom + 12), ResizeXCoor(TempXSize), ResizeYCoor(16), SWP_NOZORDER);

								unsigned long int TempTimeInterval = (unsigned long)((vPlotMaximumX * 60) / vXNumberOfScaleLines);  //seconds displayed across the chart by number of intervals
								CTimeSpan TempOffset = (TempLoop * TempTimeInterval); //TempOffset is seconds since start to this tick mark
								CTime TempIndexTime = TempTimePlotStarts + TempOffset;
								CString TempTimeString = TempIndexTime.Format("%H:%M");
								SetDlgItemText(IDC_XScale7, TempTimeString);
							}
							break;
							case 7:
							{
								m_XScale8.ShowWindow(SW_SHOW);
								m_XScale8.SetWindowPos(NULL, ResizeXCoor(TempXPosition), ResizeYCoor(TempBottom + 12), ResizeXCoor(TempXSize), ResizeYCoor(16), SWP_NOZORDER);

								unsigned long int TempTimeInterval = (unsigned long)((vPlotMaximumX * 60) / vXNumberOfScaleLines);  //seconds displayed across the chart by number of intervals
								CTimeSpan TempOffset = (TempLoop * TempTimeInterval); //TempOffset is seconds since start to this tick mark
								CTime TempIndexTime = TempTimePlotStarts + TempOffset;
								CString TempTimeString = TempIndexTime.Format("%H:%M");
								SetDlgItemText(IDC_XScale8, TempTimeString);
							}
							break;
							case 8:
							{
								m_XScale9.ShowWindow(SW_SHOW);
								m_XScale9.SetWindowPos(NULL, ResizeXCoor(TempXPosition), ResizeYCoor(TempBottom + 12), ResizeXCoor(TempXSize), ResizeYCoor(16), SWP_NOZORDER);

								unsigned long int TempTimeInterval = (unsigned long)((vPlotMaximumX * 60) / vXNumberOfScaleLines);  //seconds displayed across the chart by number of intervals
								CTimeSpan TempOffset = (TempLoop * TempTimeInterval); //TempOffset is seconds since start to this tick mark
								CTime TempIndexTime = TempTimePlotStarts + TempOffset;
								CString TempTimeString = TempIndexTime.Format("%H:%M");
								SetDlgItemText(IDC_XScale9, TempTimeString);
							}
							break;
							case 9:
							{
								m_XScale10.ShowWindow(SW_SHOW);
								m_XScale10.SetWindowPos(NULL, ResizeXCoor(TempXPosition), ResizeYCoor(TempBottom + 12), ResizeXCoor(TempXSize), ResizeYCoor(16), SWP_NOZORDER);

								unsigned long int TempTimeInterval = (unsigned long)((vPlotMaximumX * 60) / vXNumberOfScaleLines);  //seconds displayed across the chart by number of intervals
								CTimeSpan TempOffset = (TempLoop * TempTimeInterval); //TempOffset is seconds since start to this tick mark
								CTime TempIndexTime = TempTimePlotStarts + TempOffset;
								CString TempTimeString = TempIndexTime.Format("%H:%M");
								SetDlgItemText(IDC_XScale10, TempTimeString);
							}
							break;
							case 10:
							{
								m_XScale11.ShowWindow(SW_SHOW);
								m_XScale11.SetWindowPos(NULL, ResizeXCoor(TempXPosition), ResizeYCoor(TempBottom + 12), ResizeXCoor(TempXSize), ResizeYCoor(16), SWP_NOZORDER);

								unsigned long int TempTimeInterval = (unsigned long)((vPlotMaximumX * 60) / vXNumberOfScaleLines);  //seconds displayed across the chart by number of intervals
								CTimeSpan TempOffset = (TempLoop * TempTimeInterval); //TempOffset is seconds since start to this tick mark
								CTime TempIndexTime = TempTimePlotStarts + TempOffset;
								CString TempTimeString = TempIndexTime.Format("%H:%M");
								SetDlgItemText(IDC_XScale11, TempTimeString);
							}
							break;
							case 11:
							{
								m_XScale12.ShowWindow(SW_SHOW);
								m_XScale12.SetWindowPos(NULL, ResizeXCoor(TempXPosition), ResizeYCoor(TempBottom + 12), ResizeXCoor(TempXSize), ResizeYCoor(16), SWP_NOZORDER);

								unsigned long int TempTimeInterval = (unsigned long)((vPlotMaximumX * 60) / vXNumberOfScaleLines);  //seconds displayed across the chart by number of intervals
								CTimeSpan TempOffset = (TempLoop * TempTimeInterval); //TempOffset is seconds since start to this tick mark
								CTime TempIndexTime = TempTimePlotStarts + TempOffset;
								CString TempTimeString = TempIndexTime.Format("%H:%M");
								SetDlgItemText(IDC_XScale12, TempTimeString);
							}
							break;
							case 12:
							{
								m_XScale13.ShowWindow(SW_SHOW);
								m_XScale13.SetWindowPos(NULL, ResizeXCoor(TempXPosition), ResizeYCoor(TempBottom + 12), ResizeXCoor(TempXSize), ResizeYCoor(16), SWP_NOZORDER);

								unsigned long int TempTimeInterval = (unsigned long)((vPlotMaximumX * 60) / vXNumberOfScaleLines);  //seconds displayed across the chart by number of intervals
								CTimeSpan TempOffset = (TempLoop * TempTimeInterval); //TempOffset is seconds since start to this tick mark
								CTime TempIndexTime = TempTimePlotStarts + TempOffset;
								CString TempTimeString = TempIndexTime.Format("%H:%M");
								SetDlgItemText(IDC_XScale13, TempTimeString);
							}
							break;
							case 13:
							{
								m_XScale14.ShowWindow(SW_SHOW);
								m_XScale14.SetWindowPos(NULL, ResizeXCoor(TempXPosition), ResizeYCoor(TempBottom + 12), ResizeXCoor(TempXSize), ResizeYCoor(16), SWP_NOZORDER);

								unsigned long int TempTimeInterval = (unsigned long)((vPlotMaximumX * 60) / vXNumberOfScaleLines);  //seconds displayed across the chart by number of intervals
								CTimeSpan TempOffset = (TempLoop * TempTimeInterval); //TempOffset is seconds since start to this tick mark
								CTime TempIndexTime = TempTimePlotStarts + TempOffset;
								CString TempTimeString = TempIndexTime.Format("%H:%M");
								SetDlgItemText(IDC_XScale14, TempTimeString);
							}
							break;
						}
					}

					//if last x index value not the maximum value, display the maximum value at the end
					TempTimesFactor = 1;
					if (vPlotDisplay == cTrendDisplay)
						TempTimesFactor = TempTimesFactor * vCheckWeighInspection->vCheckWeighDataSize;

					if (TempTimesFactor > 1)
					{
						WORD TempXSize = 27;
						if (TempTimesFactor > 99)
							TempXSize = 36;
						if (TempTimesFactor > 999)
							TempXSize = 42;
						if (TempTimesFactor > 9999)
							TempXSize = 48;

						WORD TempNewXPosition = 635 - TempXSize;

						WORD TempMaxDisplayPosition = TempXMiddle + TempWidth + 10;
						if (TempMaxDisplayPosition + TempXSize > 635)
							TempMaxDisplayPosition = 635 - TempXSize;
						m_XLabel3.SetWindowPos(NULL, ResizeXCoor(TempMaxDisplayPosition), ResizeYCoor(TempBottom + 12), ResizeXCoor(TempXSize), ResizeYCoor(16), SWP_NOZORDER);

						m_XLabel3.ShowWindow(SW_SHOW);
						SetDlgItemText(IDC_XLabel3, "X" + dtoa(TempTimesFactor,0));
					}
					else
					if (vPlotDisplay != cTrendMinuteDisplay)
					if (TempXValue < vPlotMaximumX)
					{
						WORD TempNewXPosition = (WORD)(TempXPosition + vXInterval * vXScale);
						WORD TempXSize = 18;
						if (vPlotMaximumX > 99)
							TempXSize = 27;
						if (vPlotMaximumX > 999)
							TempXSize = 36;
						if (vPlotMaximumX > 9999)
							TempXSize = 42;
						if (vPlotDisplay == cTrendMinuteDisplay)
							TempXSize = 42;

						if (TempXPosition < 625 - (2 * TempXSize))
						{
							if (TempNewXPosition > 635 - TempXSize)
								TempNewXPosition = 635 - TempXSize;

							if (TempXPosition < 620 - (2 * TempXSize))
								TempNewXPosition = TempNewXPosition - 5;

							WORD TempMaxDisplayPosition = TempXMiddle + TempWidth + 10;
							if (TempMaxDisplayPosition + TempXSize > 635)
								TempMaxDisplayPosition = 635 - TempXSize;
							m_XLabel3.SetWindowPos(NULL, ResizeXCoor(TempMaxDisplayPosition), ResizeYCoor(TempBottom + 12), ResizeXCoor(TempXSize), ResizeYCoor(16), SWP_NOZORDER);

							m_XLabel3.ShowWindow(SW_SHOW);
							if (vPlotDisplay == cTrendMinuteDisplay)
							{
								CTime TempIndexTime = vLastDisplayedTime;
								SetDlgItemText(IDC_XLabel3, TempIndexTime.Format("%H:%M"));
							}
							else
								SetDlgItemText(IDC_XLabel3,dtoa(vPlotMaximumX,0));
						}
						else
							m_XLabel3.ShowWindow(SW_HIDE);
					}
					else
						m_XLabel3.ShowWindow(SW_HIDE);

					m_YScale7.ShowWindow(SW_HIDE);
					m_YScale8.ShowWindow(SW_HIDE);
					m_YScale9.ShowWindow(SW_HIDE);
					m_YScale10.ShowWindow(SW_HIDE);
					m_YScale11.ShowWindow(SW_HIDE);
					m_YScale12.ShowWindow(SW_HIDE);
					m_YScale13.ShowWindow(SW_HIDE);
					m_YScale14.ShowWindow(SW_HIDE);
					m_YScale15.ShowWindow(SW_HIDE);

					int TempLeftSidePosition = 1;
					if (TempLeftSide > 55)
						TempLeftSidePosition = TempLeftSide - 54;

					if (TempLeftSidePosition < 1)
						TempLeftSidePosition = -1;

					for (DWORD TempLoop = 0; TempLoop < vYNumberOfScaleLines; TempLoop++)
					{
						WORD TempY = (WORD)(TempBottom  - (((TempLoop) *  vYInterval) * vYScale));
						if (TempY < 44)
							TempY = 44;

						switch (TempLoop)
						{
							case 0:
								m_YScale1.ShowWindow(SW_SHOW);
								m_YScale1.SetWindowPos(NULL, ResizeXCoor(TempLeftSidePosition), ResizeYCoor(TempY - 4), ResizeXCoor(44), ResizeYCoor(16), SWP_NOZORDER);
								SetDlgItemText(IDC_YScale1,dtoa((TempLoop * vYInterval) + vPlotMinimumY,1));
							break;
							case 1:
								m_YScale2.ShowWindow(SW_SHOW);
								m_YScale2.SetWindowPos(NULL, ResizeXCoor(TempLeftSidePosition), ResizeYCoor(TempY - 4), ResizeXCoor(44), ResizeYCoor(16), SWP_NOZORDER);
								SetDlgItemText(IDC_YScale2,dtoa((TempLoop * vYInterval) + vPlotMinimumY,1));
							break;
							case 2:
								m_YScale3.ShowWindow(SW_SHOW);
								m_YScale3.SetWindowPos(NULL, ResizeXCoor(TempLeftSidePosition), ResizeYCoor(TempY - 4), ResizeXCoor(44), ResizeYCoor(16), SWP_NOZORDER);
								SetDlgItemText(IDC_YScale3,dtoa((TempLoop * vYInterval) + vPlotMinimumY,1));
							break;
							case 3:
								m_YScale4.ShowWindow(SW_SHOW);
								m_YScale4.SetWindowPos(NULL, ResizeXCoor(TempLeftSidePosition), ResizeYCoor(TempY - 4), ResizeXCoor(44), ResizeYCoor(16), SWP_NOZORDER);
								SetDlgItemText(IDC_YScale4,dtoa((TempLoop * vYInterval) + vPlotMinimumY,1));
							break;
							case 4:
								m_YScale5.ShowWindow(SW_SHOW);
								m_YScale5.SetWindowPos(NULL, ResizeXCoor(TempLeftSidePosition), ResizeYCoor(TempY - 4), ResizeXCoor(44), ResizeYCoor(16), SWP_NOZORDER);
								SetDlgItemText(IDC_YScale5,dtoa((TempLoop * vYInterval) + vPlotMinimumY,1));
							break;
							case 5:
								m_YScale6.ShowWindow(SW_SHOW);
								m_YScale6.SetWindowPos(NULL, ResizeXCoor(TempLeftSidePosition), ResizeYCoor(TempY - 4), ResizeXCoor(44), ResizeYCoor(16), SWP_NOZORDER);
								SetDlgItemText(IDC_YScale6,dtoa((TempLoop * vYInterval) + vPlotMinimumY,1));
							break;
							case 6:
								m_YScale7.ShowWindow(SW_SHOW);
								m_YScale7.SetWindowPos(NULL, ResizeXCoor(TempLeftSidePosition), ResizeYCoor(TempY - 4), ResizeXCoor(44), ResizeYCoor(16), SWP_NOZORDER);
								SetDlgItemText(IDC_YScale7,dtoa((TempLoop * vYInterval) + vPlotMinimumY,1));
							break;
							case 7:
								m_YScale8.ShowWindow(SW_SHOW);
								m_YScale8.SetWindowPos(NULL, ResizeXCoor(TempLeftSidePosition), ResizeYCoor(TempY - 4), ResizeXCoor(44), ResizeYCoor(16), SWP_NOZORDER);
								SetDlgItemText(IDC_YScale8,dtoa((TempLoop * vYInterval) + vPlotMinimumY,1));
							break;
							case 8:
								m_YScale9.ShowWindow(SW_SHOW);
								m_YScale9.SetWindowPos(NULL, ResizeXCoor(TempLeftSidePosition), ResizeYCoor(TempY - 4), ResizeXCoor(44), ResizeYCoor(16), SWP_NOZORDER);
								SetDlgItemText(IDC_YScale9,dtoa((TempLoop * vYInterval) + vPlotMinimumY,1));
							break;
							case 9:
								m_YScale10.ShowWindow(SW_SHOW);
								m_YScale10.SetWindowPos(NULL, ResizeXCoor(TempLeftSidePosition), ResizeYCoor(TempY - 4), ResizeXCoor(44), ResizeYCoor(16), SWP_NOZORDER);
								SetDlgItemText(IDC_YScale10,dtoa((TempLoop * vYInterval) + vPlotMinimumY,1));
							break;
							case 10:
								m_YScale11.ShowWindow(SW_SHOW);
								m_YScale11.SetWindowPos(NULL, ResizeXCoor(TempLeftSidePosition), ResizeYCoor(TempY - 4), ResizeXCoor(44), ResizeYCoor(16), SWP_NOZORDER);
								SetDlgItemText(IDC_YScale11,dtoa((TempLoop * vYInterval) + vPlotMinimumY,1));
							break;
							case 11:
								m_YScale12.ShowWindow(SW_SHOW);
								m_YScale12.SetWindowPos(NULL, ResizeXCoor(TempLeftSidePosition), ResizeYCoor(TempY - 4), ResizeXCoor(44), ResizeYCoor(16), SWP_NOZORDER);
								SetDlgItemText(IDC_YScale12,dtoa((TempLoop * vYInterval) + vPlotMinimumY,1));
							break;
							case 12:
								m_YScale13.ShowWindow(SW_SHOW);
								m_YScale13.SetWindowPos(NULL, ResizeXCoor(TempLeftSidePosition), ResizeYCoor(TempY - 4), ResizeXCoor(44), ResizeYCoor(16), SWP_NOZORDER);
								SetDlgItemText(IDC_YScale13,dtoa((TempLoop * vYInterval) + vPlotMinimumY,1));
							break;
							case 13:
								m_YScale14.ShowWindow(SW_SHOW);
								m_YScale14.SetWindowPos(NULL, ResizeXCoor(TempLeftSidePosition), ResizeYCoor(TempY - 4), ResizeXCoor(44), ResizeYCoor(16), SWP_NOZORDER);
								SetDlgItemText(IDC_YScale14,dtoa((TempLoop * vYInterval) + vPlotMinimumY,1));
							break;
							case 14:
								m_YScale15.ShowWindow(SW_SHOW);
								m_YScale15.SetWindowPos(NULL, ResizeXCoor(TempLeftSidePosition), ResizeYCoor(TempY - 4), ResizeXCoor(44), ResizeYCoor(16), SWP_NOZORDER);
								SetDlgItemText(IDC_YScale15,dtoa((TempLoop * vYInterval) + vPlotMinimumY,1));
							break;
						} //end switch
					}  //end for label
				} //end of minutes or < 11 numbers
				else
					//original version
				if ((vPlotDisplay == cTrendMinuteDisplay) || (vNumberOfPointsToPlot <= 10))
				{
					vDrawHalfTicks = false;
					vRangeYToShow = (WORD)(vPlotMaximumY - vPlotMinimumY);

					if (vPlotMaximumY < vPlotMinimumY)
						vRangeYToShow = (WORD)vPlotMinimumY;
					//plot the data on the screen
					//x dimension is  top=41 bottom = 408 height = 367; vPlotMinimumX to vPlotMaximumX
					//y dimension is  left = 106 right=529 width= 423; vMinimumY to vMaximumY
					//vXScale = 423.0 / vNumberOfPointsToPlot;
					vXScale = vGraphWidth / vPlotMaximumX;
					//vYScale = 367.0 / (vMaximumY - vMinimumY);
					if (vRangeYToShow > 5)
						vYScale = (double)vGraphHeight / (double)(vRangeYToShow);  //was 300
					else
						vYScale = 45;
					if (vXScale > 45)
						vXScale = 45;
					if (vYScale > 50)
						vYScale = 50;
					vXScaleWord = (WORD)(vXScale);  //truncate
					vXInterval = (WORD)((vPlotMaximumX / 10.0) + .5);

					if (vPlotDisplay == cTrendMinuteDisplay)
						vXInterval = (WORD)((vPlotMaximumX / 6.0) + .5);

					vYInterval = (WORD)(((vRangeYToShow) / 10.0) + .5);
					if (!vXInterval)
						vXInterval = 1;
					if (!vYInterval)
						vYInterval = 1;
					vXNumberOfScaleLines = (int)(((vPlotMaximumX) / vXInterval));

					if (vPlotDisplay == cTrendMinuteDisplay)
					if (vXNumberOfScaleLines > 12)
						vXNumberOfScaleLines = 12;

					vYNumberOfScaleLines = (WORD)((vRangeYToShow) / vYInterval + 1);

					if ((vXNumberOfScaleLines * vXInterval) + vPlotMinimumX < vPlotMaximumX)
						vXNumberOfScaleLines++;

					SetDlgItemText(IDC_LineLabel3,"Y Scale: " + dtoa(vYInterval,0));

					SetDlgItemText(IDC_XLabel2,"X Scale: " + dtoa(vXInterval * vDivideFactor,0));

					WORD TempXMiddle = 315;  //320
					WORD TempYMiddle = vGraphTop + (vGraphHeight / 2);
					WORD TempWidth = (WORD)(vPlotMaximumX * vXScale / 2.0);
					if (TempWidth > vGraphWidth)
					{
						TempWidth = vGraphWidth;
						ReportErrorMessage("CheckWeight Plot Error 5",cEMailInspx,32000);
					}
					WORD TempHeight = (WORD)((vRangeYToShow) * vYScale);
					if (TempHeight > vGraphHeight)
					{
						TempHeight = vGraphHeight;
						ReportErrorMessage("CheckWeight Plot Error 6",cEMailInspx,32000);
					}
					WORD TempLeftSide = TempXMiddle - TempWidth;//200; //106;
					if (TempLeftSide < 8)
					{
						//ReportErrorMessage("CheckWeight Plot Error 7",cEMailInspx,32000);
						TempLeftSide = 8;
					}
					WORD TempRightSide = TempXMiddle + TempWidth;//TempLeftSide + vPlotMaximumX;//529;
					WORD TempBottom = (WORD)(TempYMiddle + (TempHeight / 2.0)); //170; //408;
					WORD TempTop = (WORD)(TempYMiddle - (TempHeight / 2.0));//TempBottom - (vYScale); //41;
					if (TempTop < vGraphTop)
					{
						ReportErrorMessage("CheckWeight Plot Error 8",cEMailInspx,32000);
						TempTop = vGraphTop;
					}

					//m_LineLabel3.SetWindowPos(NULL,10,10,110,20,SWP_NOZORDER);

					m_XScale1.ShowWindow(SW_HIDE);
					m_XScale2.ShowWindow(SW_HIDE);
					m_XScale3.ShowWindow(SW_HIDE);
					m_XScale4.ShowWindow(SW_HIDE);
					m_XScale5.ShowWindow(SW_HIDE);
					m_XScale6.ShowWindow(SW_HIDE);
					m_XScale7.ShowWindow(SW_HIDE);
					m_XScale8.ShowWindow(SW_HIDE);
					m_XScale9.ShowWindow(SW_HIDE);
					m_XScale10.ShowWindow(SW_HIDE);
					m_XScale11.ShowWindow(SW_HIDE);
					m_XScale12.ShowWindow(SW_HIDE);
					m_XScale13.ShowWindow(SW_HIDE);
					m_XScale14.ShowWindow(SW_HIDE);

					//fill in scale values
					if (!vPlotMinimumX)
						vPlotMinimumX = 1;
					WORD TempXPosition = 0;
					DWORD TempXValue = 0;
					for (DWORD TempLoop = 0; TempLoop < vXNumberOfScaleLines; TempLoop++)
					{

						TempXValue = (DWORD)((TempLoop * vXInterval) + vPlotMinimumX);

						//if (vPlotDisplay == cTrendMinuteDisplay) //if more than 500 minutes of data, correct scale numbers shown as compressed the data
						//if (vPlotMaximumX > 500)
						//	TempXValue = (DWORD)(TempXValue * (vPlotMaximumX / 500));

						if (TempXValue > vPlotMaximumX)
							TempXValue = (DWORD)vPlotMaximumX;

						if (TempXValue > 1)
							TempXValue = TempXValue * vDivideFactor;

						WORD TempXSize = 42;
						if (TempXValue > 9)
							TempXSize = 18;
						if (TempXValue > 99)
							TempXSize = 27;
						if (TempXValue > 999)
							TempXSize = 36;
						if (TempXValue > 9999)
							TempXSize = 42;
						if (vPlotDisplay == cTrendMinuteDisplay)
							TempXSize = 42;

						TempXPosition = (WORD)((double)TempLeftSide + ((double)TempLoop * (double)vXInterval * vXScale) + (vXScale / 2) - (TempXSize / 2));

						if (TempXPosition > 635 - TempXSize)
							TempXPosition = 635 - TempXSize;


						switch (TempLoop)
						{
							case 0:
								m_XScale1.ShowWindow(SW_SHOW);
								m_XScale1.SetWindowPos(NULL, ResizeXCoor(TempXPosition), ResizeYCoor(TempBottom + 12), ResizeXCoor(TempXSize), ResizeYCoor(16),SWP_NOZORDER);

								if (vPlotDisplay == cTrendMinuteDisplay)
								{
									//CTimeSpan TempOffset = ((TempLoop * vXInterval) + 1) * 60;  // *60 converts to minutes as CTimeSpan is seconds, 1 minute after start plus intervals
									//CTime TempIndexTime = vLocalSystemData->vTimeStarted + TempOffset;
									//SetDlgItemText(IDC_XScale1, TempIndexTime.Format("%H:%M"));
									unsigned long int TempTimeInterval = vLocalSystemData->vCheckWeighRunDuration / (vXNumberOfScaleLines);
									CTimeSpan TempOffset = (TempLoop * TempTimeInterval) + 60; //TempOffset is seconds since start to this tick mark
									CTime TempIndexTime = vLocalSystemData->vTimeStarted + TempOffset;
									CString TempTimeString = TempIndexTime.Format("%H:%M");
									SetDlgItemText(IDC_XScale1, TempTimeString);
								}
								else
									SetDlgItemText(IDC_XScale1,dtoa(TempXValue,1));
							break;
							case 1:
								m_XScale2.ShowWindow(SW_SHOW);
								m_XScale2.SetWindowPos(NULL, ResizeXCoor(TempXPosition), ResizeYCoor(TempBottom + 12), ResizeXCoor(TempXSize), ResizeYCoor(16),SWP_NOZORDER);

								if (vPlotDisplay == cTrendMinuteDisplay)
								{
									//CTimeSpan TempOffset = ((TempLoop * vXInterval) + 1) * 60;  // *60 converts to minutes as CTimeSpan is seconds, 1 minute after start plus intervals
									//CTime TempIndexTime = vLocalSystemData->vTimeStarted + TempOffset;
									//SetDlgItemText(IDC_XScale2, TempIndexTime.Format("%H:%M"));
									unsigned long int TempTimeInterval = vLocalSystemData->vCheckWeighRunDuration / (vXNumberOfScaleLines);
									CTimeSpan TempOffset = (TempLoop * TempTimeInterval) + 60; //TempOffset is seconds since start to this tick mark
									CTime TempIndexTime = vLocalSystemData->vTimeStarted + TempOffset;
									CString TempTimeString = TempIndexTime.Format("%H:%M");
									SetDlgItemText(IDC_XScale2, TempTimeString);
								}
								else
									SetDlgItemText(IDC_XScale2,dtoa(TempXValue,1));
							break;
							case 2:
								m_XScale3.ShowWindow(SW_SHOW);
								m_XScale3.SetWindowPos(NULL, ResizeXCoor(TempXPosition), ResizeYCoor(TempBottom + 12), ResizeXCoor(TempXSize), ResizeYCoor(16),SWP_NOZORDER);

								if (vPlotDisplay == cTrendMinuteDisplay)
								{
									unsigned long int TempTimeInterval = vLocalSystemData->vCheckWeighRunDuration / (vXNumberOfScaleLines);
									CTimeSpan TempOffset = (TempLoop * TempTimeInterval) + 60; //TempOffset is seconds since start to this tick mark
									CTime TempIndexTime = vLocalSystemData->vTimeStarted + TempOffset;
									CString TempTimeString = TempIndexTime.Format("%H:%M");
									SetDlgItemText(IDC_XScale3, TempTimeString);
								}
								else
									SetDlgItemText(IDC_XScale3,dtoa(TempXValue,1));
							break;
							case 3:
								m_XScale4.ShowWindow(SW_SHOW);
								m_XScale4.SetWindowPos(NULL, ResizeXCoor(TempXPosition), ResizeYCoor(TempBottom + 12), ResizeXCoor(TempXSize), ResizeYCoor(16),SWP_NOZORDER);

								if (vPlotDisplay == cTrendMinuteDisplay)
								{
									unsigned long int TempTimeInterval = vLocalSystemData->vCheckWeighRunDuration / (vXNumberOfScaleLines);
									CTimeSpan TempOffset = (TempLoop * TempTimeInterval) + 60; //TempOffset is seconds since start to this tick mark
									CTime TempIndexTime = vLocalSystemData->vTimeStarted + TempOffset;
									CString TempTimeString = TempIndexTime.Format("%H:%M");
									SetDlgItemText(IDC_XScale4, TempTimeString);
								}
								else
									SetDlgItemText(IDC_XScale4,dtoa(TempXValue,1));
							break;
							case 4:
								m_XScale5.ShowWindow(SW_SHOW);
								m_XScale5.SetWindowPos(NULL, ResizeXCoor(TempXPosition), ResizeYCoor(TempBottom + 12), ResizeXCoor(TempXSize), ResizeYCoor(16),SWP_NOZORDER);

								if (vPlotDisplay == cTrendMinuteDisplay)
								{
									unsigned long int TempTimeInterval = vLocalSystemData->vCheckWeighRunDuration / (vXNumberOfScaleLines);
									CTimeSpan TempOffset = (TempLoop * TempTimeInterval) + 60; //TempOffset is seconds since start to this tick mark
									CTime TempIndexTime = vLocalSystemData->vTimeStarted + TempOffset;
									CString TempTimeString = TempIndexTime.Format("%H:%M");
									SetDlgItemText(IDC_XScale5, TempTimeString);
								}
								else
									SetDlgItemText(IDC_XScale5,dtoa(TempXValue,1));
							break;
							case 5:
								m_XScale6.ShowWindow(SW_SHOW);
								m_XScale6.SetWindowPos(NULL, ResizeXCoor(TempXPosition), ResizeYCoor(TempBottom + 12), ResizeXCoor(TempXSize), ResizeYCoor(16),SWP_NOZORDER);

								if (vPlotDisplay == cTrendMinuteDisplay)
								{
									unsigned long int TempTimeInterval = vLocalSystemData->vCheckWeighRunDuration / (vXNumberOfScaleLines);
									CTimeSpan TempOffset = (TempLoop * TempTimeInterval) + 60; //TempOffset is seconds since start to this tick mark
									CTime TempIndexTime = vLocalSystemData->vTimeStarted + TempOffset;
									CString TempTimeString = TempIndexTime.Format("%H:%M");
									SetDlgItemText(IDC_XScale6, TempTimeString);
								}
								else
									SetDlgItemText(IDC_XScale6,dtoa(TempXValue,1));
							break;
							case 6:
								m_XScale7.ShowWindow(SW_SHOW);
								m_XScale7.SetWindowPos(NULL, ResizeXCoor(TempXPosition), ResizeYCoor(TempBottom + 12), ResizeXCoor(TempXSize), ResizeYCoor(16),SWP_NOZORDER);

								if (vPlotDisplay == cTrendMinuteDisplay)
								{
									unsigned long int TempTimeInterval = vLocalSystemData->vCheckWeighRunDuration / (vXNumberOfScaleLines);
									CTimeSpan TempOffset = (TempLoop * TempTimeInterval) + 60; //TempOffset is seconds since start to this tick mark
									CTime TempIndexTime = vLocalSystemData->vTimeStarted + TempOffset;
									CString TempTimeString = TempIndexTime.Format("%H:%M");
									SetDlgItemText(IDC_XScale7, TempTimeString);
								}
								else
									SetDlgItemText(IDC_XScale7,dtoa(TempXValue,1));
							break;
							case 7:
								m_XScale8.ShowWindow(SW_SHOW);
								m_XScale8.SetWindowPos(NULL, ResizeXCoor(TempXPosition), ResizeYCoor(TempBottom + 12), ResizeXCoor(TempXSize), ResizeYCoor(16),SWP_NOZORDER);

								if (vPlotDisplay == cTrendMinuteDisplay)
								{
									unsigned long int TempTimeInterval = vLocalSystemData->vCheckWeighRunDuration / (vXNumberOfScaleLines);
									CTimeSpan TempOffset = (TempLoop * TempTimeInterval) + 60; //TempOffset is seconds since start to this tick mark
									CTime TempIndexTime = vLocalSystemData->vTimeStarted + TempOffset;
									CString TempTimeString = TempIndexTime.Format("%H:%M");
									SetDlgItemText(IDC_XScale8, TempTimeString);
								}
								else
									SetDlgItemText(IDC_XScale8,dtoa(TempXValue,1));
							break;
							case 8:
								m_XScale9.ShowWindow(SW_SHOW);
								m_XScale9.SetWindowPos(NULL, ResizeXCoor(TempXPosition), ResizeYCoor(TempBottom + 12), ResizeXCoor(TempXSize), ResizeYCoor(16),SWP_NOZORDER);

								if (vPlotDisplay == cTrendMinuteDisplay)
								{
									unsigned long int TempTimeInterval = vLocalSystemData->vCheckWeighRunDuration / (vXNumberOfScaleLines);
									CTimeSpan TempOffset = (TempLoop * TempTimeInterval) + 60; //TempOffset is seconds since start to this tick mark
									CTime TempIndexTime = vLocalSystemData->vTimeStarted + TempOffset;
									CString TempTimeString = TempIndexTime.Format("%H:%M");
									SetDlgItemText(IDC_XScale9, TempTimeString);
								}
								else
									SetDlgItemText(IDC_XScale9,dtoa(TempXValue,1));
							break;
							case 9:
								m_XScale10.ShowWindow(SW_SHOW);
								m_XScale10.SetWindowPos(NULL, ResizeXCoor(TempXPosition), ResizeYCoor(TempBottom + 12), ResizeXCoor(TempXSize), ResizeYCoor(16),SWP_NOZORDER);

								if (vPlotDisplay == cTrendMinuteDisplay)
								{
									unsigned long int TempTimeInterval = vLocalSystemData->vCheckWeighRunDuration / (vXNumberOfScaleLines);
									CTimeSpan TempOffset = (TempLoop * TempTimeInterval) + 60; //TempOffset is seconds since start to this tick mark
									CTime TempIndexTime = vLocalSystemData->vTimeStarted + TempOffset;
									CString TempTimeString = TempIndexTime.Format("%H:%M");
									SetDlgItemText(IDC_XScale10, TempTimeString);
								}
								else
									SetDlgItemText(IDC_XScale10,dtoa(TempXValue,1));
							break;
							case 10:
								m_XScale11.ShowWindow(SW_SHOW);
								m_XScale11.SetWindowPos(NULL, ResizeXCoor(TempXPosition), ResizeYCoor(TempBottom + 12), ResizeXCoor(TempXSize), ResizeYCoor(16),SWP_NOZORDER);

								if (vPlotDisplay == cTrendMinuteDisplay)
								{
									unsigned long int TempTimeInterval = vLocalSystemData->vCheckWeighRunDuration / (vXNumberOfScaleLines);
									CTimeSpan TempOffset = (TempLoop * TempTimeInterval) + 60; //TempOffset is seconds since start to this tick mark
									CTime TempIndexTime = vLocalSystemData->vTimeStarted + TempOffset;
									CString TempTimeString = TempIndexTime.Format("%H:%M");
									SetDlgItemText(IDC_XScale11, TempTimeString);
								}
								else
									SetDlgItemText(IDC_XScale11,dtoa(TempXValue,1));
							break;
							case 11:
								m_XScale12.ShowWindow(SW_SHOW);
								m_XScale12.SetWindowPos(NULL, ResizeXCoor(TempXPosition), ResizeYCoor(TempBottom + 12), ResizeXCoor(TempXSize), ResizeYCoor(16),SWP_NOZORDER);

								if (vPlotDisplay == cTrendMinuteDisplay)
								{
									unsigned long int TempTimeInterval = vLocalSystemData->vCheckWeighRunDuration / (vXNumberOfScaleLines);
									CTimeSpan TempOffset = (TempLoop * TempTimeInterval) + 60; //TempOffset is seconds since start to this tick mark
									CTime TempIndexTime = vLocalSystemData->vTimeStarted + TempOffset;
									CString TempTimeString = TempIndexTime.Format("%H:%M");
									SetDlgItemText(IDC_XScale12, TempTimeString);
								}
								else
									SetDlgItemText(IDC_XScale12,dtoa(TempXValue,1));
							break;
							case 12:
								m_XScale13.ShowWindow(SW_SHOW);
								m_XScale13.SetWindowPos(NULL, ResizeXCoor(TempXPosition), ResizeYCoor(TempBottom + 12), ResizeXCoor(TempXSize), ResizeYCoor(16),SWP_NOZORDER);

								if (vPlotDisplay == cTrendMinuteDisplay)
								{
									unsigned long int TempTimeInterval = vLocalSystemData->vCheckWeighRunDuration / (vXNumberOfScaleLines);
									CTimeSpan TempOffset = (TempLoop * TempTimeInterval) + 60; //TempOffset is seconds since start to this tick mark
									CTime TempIndexTime = vLocalSystemData->vTimeStarted + TempOffset;
									CString TempTimeString = TempIndexTime.Format("%H:%M");
									SetDlgItemText(IDC_XScale13, TempTimeString);
								}
								else
									SetDlgItemText(IDC_XScale13,dtoa(TempXValue,1));
							break;
							case 13:
								m_XScale14.ShowWindow(SW_SHOW);
								m_XScale14.SetWindowPos(NULL, ResizeXCoor(TempXPosition), ResizeYCoor(TempBottom + 12), ResizeXCoor(TempXSize), ResizeYCoor(16),SWP_NOZORDER);

								if (vPlotDisplay == cTrendMinuteDisplay)
								{
									unsigned long int TempTimeInterval = vLocalSystemData->vCheckWeighRunDuration / (vXNumberOfScaleLines);
									CTimeSpan TempOffset = (TempLoop * TempTimeInterval) + 60; //TempOffset is seconds since start to this tick mark
									CTime TempIndexTime = vLocalSystemData->vTimeStarted + TempOffset;
									CString TempTimeString = TempIndexTime.Format("%H:%M");
									SetDlgItemText(IDC_XScale14, TempTimeString);
								}
								else
									SetDlgItemText(IDC_XScale14,dtoa(TempXValue,1));
							break;
						}
					}

					//if last x index value not the maximum value, display the maximum value at the end
					TempTimesFactor = 1;
					if (vPlotDisplay == cTrendDisplay)
						TempTimesFactor = TempTimesFactor * vCheckWeighInspection->vCheckWeighDataSize;

					if (TempTimesFactor > 1)
					{
						WORD TempXSize = 27;
						if (TempTimesFactor > 99)
							TempXSize = 36;
						if (TempTimesFactor > 999)
							TempXSize = 42;
						if (TempTimesFactor > 9999)
							TempXSize = 48;

						WORD TempNewXPosition = 635 - TempXSize;

						WORD TempMaxDisplayPosition = TempXMiddle + TempWidth + 10;
						if (TempMaxDisplayPosition + TempXSize > 635)
							TempMaxDisplayPosition = 635 - TempXSize;
						m_XLabel3.SetWindowPos(NULL, ResizeXCoor(TempMaxDisplayPosition), ResizeYCoor(TempBottom + 12), ResizeXCoor(TempXSize), ResizeYCoor(16), SWP_NOZORDER);

						m_XLabel3.ShowWindow(SW_SHOW);
						SetDlgItemText(IDC_XLabel3, "X" + dtoa(TempTimesFactor,0));
					}
					else
					if (vPlotDisplay != cTrendMinuteDisplay)
					if (TempXValue < vPlotMaximumX)
					{
						WORD TempNewXPosition = (WORD)(TempXPosition + vXInterval * vXScale);
						WORD TempXSize = 18;
						if (vPlotMaximumX > 99)
							TempXSize = 27;
						if (vPlotMaximumX > 999)
							TempXSize = 36;
						if (vPlotMaximumX > 9999)
							TempXSize = 42;
						if (vPlotDisplay == cTrendMinuteDisplay)
							TempXSize = 42;

						if (TempXPosition < 625 - (2 * TempXSize))
						{
							if (TempNewXPosition > 635 - TempXSize)
								TempNewXPosition = 635 - TempXSize;

							if (TempXPosition < 620 - (2 * TempXSize))
								TempNewXPosition = TempNewXPosition - 5;

							WORD TempMaxDisplayPosition = TempXMiddle + TempWidth + 10;
							if (TempMaxDisplayPosition + TempXSize > 635)
								TempMaxDisplayPosition = 635 - TempXSize;
							m_XLabel3.SetWindowPos(NULL, ResizeXCoor(TempMaxDisplayPosition), ResizeYCoor(TempBottom + 12), ResizeXCoor(TempXSize), ResizeYCoor(16), SWP_NOZORDER);

							m_XLabel3.ShowWindow(SW_SHOW);
							if (vPlotDisplay == cTrendMinuteDisplay)
							{
								CTime TempIndexTime = vLastDisplayedTime;
								SetDlgItemText(IDC_XLabel3, TempIndexTime.Format("%H:%M"));
							}
							else
								SetDlgItemText(IDC_XLabel3,dtoa(vPlotMaximumX,0));
						}
						else
							m_XLabel3.ShowWindow(SW_HIDE);
					}
					else
						m_XLabel3.ShowWindow(SW_HIDE);

					m_YScale7.ShowWindow(SW_HIDE);
					m_YScale8.ShowWindow(SW_HIDE);
					m_YScale9.ShowWindow(SW_HIDE);
					m_YScale10.ShowWindow(SW_HIDE);
					m_YScale11.ShowWindow(SW_HIDE);
					m_YScale12.ShowWindow(SW_HIDE);
					m_YScale13.ShowWindow(SW_HIDE);
					m_YScale14.ShowWindow(SW_HIDE);
					m_YScale15.ShowWindow(SW_HIDE);

					int TempLeftSidePosition = 1;
					if (TempLeftSide > 55)
						TempLeftSidePosition = TempLeftSide - 54;

					if (TempLeftSidePosition < 1)
						TempLeftSidePosition = -1;

					for (DWORD TempLoop = 0; TempLoop < vYNumberOfScaleLines; TempLoop++)
					{
						WORD TempY = (WORD)(TempBottom  - (((TempLoop) *  vYInterval) * vYScale));
						if (TempY < 44)
							TempY = 44;

						switch (TempLoop)
						{
							case 0:
								m_YScale1.ShowWindow(SW_SHOW);
								m_YScale1.SetWindowPos(NULL, ResizeXCoor(TempLeftSidePosition), ResizeYCoor(TempY - 4), ResizeXCoor(44), ResizeYCoor(16),SWP_NOZORDER);
								SetDlgItemText(IDC_YScale1,dtoa((TempLoop * vYInterval) + vPlotMinimumY,1));
							break;
							case 1:
								m_YScale2.ShowWindow(SW_SHOW);
								m_YScale2.SetWindowPos(NULL, ResizeXCoor(TempLeftSidePosition), ResizeYCoor(TempY - 4), ResizeXCoor(44), ResizeYCoor(16),SWP_NOZORDER);
								SetDlgItemText(IDC_YScale2,dtoa((TempLoop * vYInterval) + vPlotMinimumY,1));
							break;
							case 2:
								m_YScale3.ShowWindow(SW_SHOW);
								m_YScale3.SetWindowPos(NULL, ResizeXCoor(TempLeftSidePosition), ResizeYCoor(TempY - 4), ResizeXCoor(44), ResizeYCoor(16),SWP_NOZORDER);
								SetDlgItemText(IDC_YScale3,dtoa((TempLoop * vYInterval) + vPlotMinimumY,1));
							break;
							case 3:
								m_YScale4.ShowWindow(SW_SHOW);
								m_YScale4.SetWindowPos(NULL, ResizeXCoor(TempLeftSidePosition), ResizeYCoor(TempY - 4), ResizeXCoor(44), ResizeYCoor(16),SWP_NOZORDER);
								SetDlgItemText(IDC_YScale4,dtoa((TempLoop * vYInterval) + vPlotMinimumY,1));
							break;
							case 4:
								m_YScale5.ShowWindow(SW_SHOW);
								m_YScale5.SetWindowPos(NULL, ResizeXCoor(TempLeftSidePosition), ResizeYCoor(TempY - 4), ResizeXCoor(44), ResizeYCoor(16),SWP_NOZORDER);
								SetDlgItemText(IDC_YScale5,dtoa((TempLoop * vYInterval) + vPlotMinimumY,1));
							break;
							case 5:
								m_YScale6.ShowWindow(SW_SHOW);
								m_YScale6.SetWindowPos(NULL, ResizeXCoor(TempLeftSidePosition), ResizeYCoor(TempY - 4), ResizeXCoor(44), ResizeYCoor(16),SWP_NOZORDER);
								SetDlgItemText(IDC_YScale6,dtoa((TempLoop * vYInterval) + vPlotMinimumY,1));
							break;
							case 6:
								m_YScale7.ShowWindow(SW_SHOW);
								m_YScale7.SetWindowPos(NULL, ResizeXCoor(TempLeftSidePosition), ResizeYCoor(TempY - 4), ResizeXCoor(44), ResizeYCoor(16),SWP_NOZORDER);
								SetDlgItemText(IDC_YScale7,dtoa((TempLoop * vYInterval) + vPlotMinimumY,1));
							break;
							case 7:
								m_YScale8.ShowWindow(SW_SHOW);
								m_YScale8.SetWindowPos(NULL, ResizeXCoor(TempLeftSidePosition), ResizeYCoor(TempY - 4), ResizeXCoor(44), ResizeYCoor(16),SWP_NOZORDER);
								SetDlgItemText(IDC_YScale8,dtoa((TempLoop * vYInterval) + vPlotMinimumY,1));
							break;
							case 8:
								m_YScale9.ShowWindow(SW_SHOW);
								m_YScale9.SetWindowPos(NULL, ResizeXCoor(TempLeftSidePosition), ResizeYCoor(TempY - 4), ResizeXCoor(44), ResizeYCoor(16),SWP_NOZORDER);
								SetDlgItemText(IDC_YScale9,dtoa((TempLoop * vYInterval) + vPlotMinimumY,1));
							break;
							case 9:
								m_YScale10.ShowWindow(SW_SHOW);
								m_YScale10.SetWindowPos(NULL, ResizeXCoor(TempLeftSidePosition), ResizeYCoor(TempY - 4), ResizeXCoor(44), ResizeYCoor(16),SWP_NOZORDER);
								SetDlgItemText(IDC_YScale10,dtoa((TempLoop * vYInterval) + vPlotMinimumY,1));
							break;
							case 10:
								m_YScale11.ShowWindow(SW_SHOW);
								m_YScale11.SetWindowPos(NULL, ResizeXCoor(TempLeftSidePosition), ResizeYCoor(TempY - 4), ResizeXCoor(44), ResizeYCoor(16),SWP_NOZORDER);
								SetDlgItemText(IDC_YScale11,dtoa((TempLoop * vYInterval) + vPlotMinimumY,1));
							break;
							case 11:
								m_YScale12.ShowWindow(SW_SHOW);
								m_YScale12.SetWindowPos(NULL, ResizeXCoor(TempLeftSidePosition), ResizeYCoor(TempY - 4), ResizeXCoor(44), ResizeYCoor(16),SWP_NOZORDER);
								SetDlgItemText(IDC_YScale12,dtoa((TempLoop * vYInterval) + vPlotMinimumY,1));
							break;
							case 12:
								m_YScale13.ShowWindow(SW_SHOW);
								m_YScale13.SetWindowPos(NULL, ResizeXCoor(TempLeftSidePosition), ResizeYCoor(TempY - 4), ResizeXCoor(44), ResizeYCoor(16),SWP_NOZORDER);
								SetDlgItemText(IDC_YScale13,dtoa((TempLoop * vYInterval) + vPlotMinimumY,1));
							break;
							case 13:
								m_YScale14.ShowWindow(SW_SHOW);
								m_YScale14.SetWindowPos(NULL, ResizeXCoor(TempLeftSidePosition), ResizeYCoor(TempY - 4), ResizeXCoor(44), ResizeYCoor(16),SWP_NOZORDER);
								SetDlgItemText(IDC_YScale14,dtoa((TempLoop * vYInterval) + vPlotMinimumY,1));
							break;
							case 14:
								m_YScale15.ShowWindow(SW_SHOW);
								m_YScale15.SetWindowPos(NULL, ResizeXCoor(TempLeftSidePosition), ResizeYCoor(TempY - 4), ResizeXCoor(44), ResizeYCoor(16),SWP_NOZORDER);
								SetDlgItemText(IDC_YScale15,dtoa((TempLoop * vYInterval) + vPlotMinimumY,1));
							break;
							default:
							break;
						} //end switch
					}  //end for label
				} //end of minutes or < 11 numbers
				else
				//not minutes and more than 11 numbers
				{ //alexs special modified scale
					vDrawHalfTicks = true;
					vRangeYToShow = (WORD)(vPlotMaximumY - vPlotMinimumY);

					if (vPlotMaximumY < vPlotMinimumY)
						vRangeYToShow = (WORD)vPlotMinimumY;
					//plot the data on the screen
					//x dimension is  top=41 bottom = 408 height = 367; vPlotMinimumX to vPlotMaximumX
					//y dimension is  left = 106 right=529 width= 423; vMinimumY to vMaximumY
					//vXScale = 423.0 / vNumberOfPointsToPlot;

					//want X range to be a multiple of 10
					if (vPlotMaximumX <= 20)
						vPlotMaximumX = 20;
					else
					if (vPlotMaximumX <= 40)
						vPlotMaximumX = 40;
					else
					if (vPlotMaximumX <= 80)
						vPlotMaximumX = 80;
					else
					if (vPlotMaximumX <= 100)
						vPlotMaximumX = 100;
					else
					if (vPlotMaximumX <= 200)
						vPlotMaximumX = 200;
					else
					if (vPlotMaximumX <= 400)
						vPlotMaximumX = 400;
					else
					if (vPlotMaximumX <= 500)
						vPlotMaximumX = 500;
					else
					if (vPlotMaximumX <= 1000)
						vPlotMaximumX = 1000;
					else
					if (vPlotMaximumX <= 2000)
						vPlotMaximumX = 2000;
					else
					if (vPlotMaximumX <= 4000)
						vPlotMaximumX = 4000;
					else
					if (vPlotMaximumX <= 8000)
						vPlotMaximumX = 8000;
					else
					if (vPlotMaximumX <= 16000)
						vPlotMaximumX = 16000;
					else
					if (vPlotMaximumX <= 32000)
						vPlotMaximumX = 32000;
					else
					if (vPlotMaximumX <= 64000)
						vPlotMaximumX = 64000;
					else
					if (vPlotMaximumX <= 128000)
						vPlotMaximumX = 128000;

					//if (vPlotMaximumX <= 50)
					//	vPlotMaximumX = 50;
					//else
					//if (vPlotMaximumX <= 100)
					//	vPlotMaximumX = 100;
					//else
					//if (vPlotMaximumX <= 500)
					//	vPlotMaximumX = 500;
					//else
					//if (vPlotMaximumX <= 1000)
					//	vPlotMaximumX = 1000;
					//else
					//if (vPlotMaximumX <= 5000)
					//	vPlotMaximumX = 5000;
					//else
					//if (vPlotMaximumX <= 10000)
					//	vPlotMaximumX = 10000;
					//else
					//if (vPlotMaximumX <= 20000)
					//	vPlotMaximumX = 20000;
					//else
					//if (vPlotMaximumX <= 40000)
					//	vPlotMaximumX = 40000;

					vXScale = vGraphWidth / vPlotMaximumX;
					//vYScale = 367.0 / (vMaximumY - vMinimumY);
					if (vRangeYToShow > 5)
						vYScale = (double)vGraphHeight / (double)(vRangeYToShow);  //was 300
					else
						vYScale = 45;
					if (vXScale > 45)
						vXScale = 45;
					if (vYScale > 50)
						vYScale = 50;
					vXScaleWord = (WORD)(vXScale);  //truncate
					vXInterval = (WORD)((vPlotMaximumX / 10.0) + .5);
					vYInterval = (WORD)(((vRangeYToShow) / 10.0) + .5);
					if (!vXInterval)
						vXInterval = 1;
					if (!vYInterval)
						vYInterval = 1;
					vXNumberOfScaleLines = 10;//(int)(((vPlotMaximumX) / vXInterval));
					vYNumberOfScaleLines = (WORD)((vRangeYToShow) / vYInterval + 1);

					WORD TempXMiddle = 315;  //320
					WORD TempYMiddle = vGraphTop + (vGraphHeight / 2);
					WORD TempWidth = (WORD)(vPlotMaximumX * vXScale / 2.0);
					if (TempWidth > vGraphWidth)
					{
						TempWidth = vGraphWidth;
						ReportErrorMessage("CheckWeight Plot Error 5",cEMailInspx,32000);
					}
					WORD TempHeight = (WORD)((vRangeYToShow) * vYScale);
					if (TempHeight > vGraphHeight)
					{
						TempHeight = vGraphHeight;
						ReportErrorMessage("CheckWeight Plot Error 6",cEMailInspx,32000);
					}
					WORD TempLeftSide = TempXMiddle - TempWidth;//200; //106;
					if (TempLeftSide < 8)
					{
						//ReportErrorMessage("CheckWeight Plot Error 7",cEMailInspx,32000);
						TempLeftSide = 8;
					}
					WORD TempRightSide = TempXMiddle + TempWidth;//TempLeftSide + vPlotMaximumX;//529;
					WORD TempBottom = (WORD)(TempYMiddle + (TempHeight / 2.0)); //170; //408;
					WORD TempTop = (WORD)(TempYMiddle - (TempHeight / 2.0));//TempBottom - (vYScale); //41;
					if (TempTop < vGraphTop)
					{
						ReportErrorMessage("CheckWeight Plot Error 8",cEMailInspx,32000);
						TempTop = vGraphTop;
					}

					//m_LineLabel3.SetWindowPos(NULL,10,10,110,20,SWP_NOZORDER);

					m_XScale1.ShowWindow(SW_HIDE);
					m_XScale2.ShowWindow(SW_HIDE);
					m_XScale3.ShowWindow(SW_HIDE);
					m_XScale4.ShowWindow(SW_HIDE);
					m_XScale5.ShowWindow(SW_HIDE);
					m_XScale6.ShowWindow(SW_HIDE);
					m_XScale7.ShowWindow(SW_HIDE);
					m_XScale8.ShowWindow(SW_HIDE);
					m_XScale9.ShowWindow(SW_HIDE);
					m_XScale10.ShowWindow(SW_HIDE);
					m_XScale11.ShowWindow(SW_HIDE);
					m_XScale12.ShowWindow(SW_HIDE);
					m_XScale13.ShowWindow(SW_HIDE);
					m_XScale14.ShowWindow(SW_HIDE);

					//fill in scale values
					if (!vPlotMinimumX)
						vPlotMinimumX = 1;
					WORD TempXPosition = 0;
					double TempXValue = 0;

					double TempPlotMaximumX = vPlotMaximumX;
					TempTimesFactor = 1;
					while (TempPlotMaximumX > 1000)
					{
						TempTimesFactor = TempTimesFactor * 100;
						TempPlotMaximumX = TempPlotMaximumX / 100;
					}

					for (DWORD TempLoop = 0; TempLoop < vXNumberOfScaleLines; TempLoop++)
					{

						//TempXValue = (DWORD)((TempLoop * vXInterval) + vPlotMinimumX);
						TempXValue = TempLoop * vXInterval;
						if (TempXValue > vPlotMaximumX)
							TempXValue = vPlotMaximumX;

						if (TempXValue > 1)
							TempXValue = TempXValue * vDivideFactor;

						TempXValue = TempXValue / TempTimesFactor;

						WORD TempXSize = 42;
						if (TempXValue > 9)
							TempXSize = 18;
						if (TempXValue > 99)
							TempXSize = 27;
						if (TempXValue > 999)
							TempXSize = 36;
						if (TempXValue > 9999)
							TempXSize = 42;
						if (vPlotDisplay == cTrendMinuteDisplay)
							TempXSize = 42;

						TempXPosition = (WORD)((double)TempLeftSide + ((double)TempLoop * (double)vXInterval * vXScale) - (TempXSize / 2));

						if (TempXPosition > 635 - TempXSize)
							TempXPosition = 635 - TempXSize;


						switch (TempLoop)
						{
							case 0:
								m_XScale1.ShowWindow(SW_HIDE);
								//m_XScale1.SetWindowPos(NULL, ResizeXCoor(TempXPosition), ResizeYCoor(TempBottom + 12), ResizeXCoor(TempXSize), ResizeYCoor(16),SWP_NOZORDER);
								//SetDlgItemText(IDC_XScale1,dtoa(TempXValue,1));
							break;
							case 1:
								m_XScale2.ShowWindow(SW_HIDE);
								//m_XScale2.SetWindowPos(NULL, ResizeXCoor(TempXPosition), ResizeYCoor(TempBottom + 12), ResizeXCoor(TempXSize), ResizeYCoor(16),SWP_NOZORDER);
								//SetDlgItemText(IDC_XScale2,dtoa(TempXValue,1));
							break;
							case 2:
								m_XScale3.ShowWindow(SW_SHOW);
								m_XScale3.SetWindowPos(NULL, ResizeXCoor(TempXPosition), ResizeYCoor(TempBottom + 12), ResizeXCoor(TempXSize), ResizeYCoor(16),SWP_NOZORDER);
								SetDlgItemText(IDC_XScale3,dtoa(TempXValue,1));
							break;
							case 3:
								m_XScale4.ShowWindow(SW_HIDE);
								//m_XScale4.SetWindowPos(NULL, ResizeXCoor(TempXPosition), ResizeYCoor(TempBottom + 12), ResizeXCoor(TempXSize), ResizeYCoor(16),SWP_NOZORDER);
								//SetDlgItemText(IDC_XScale4,dtoa(TempXValue,1));
							break;
							case 4:
								m_XScale5.ShowWindow(SW_SHOW);
								m_XScale5.SetWindowPos(NULL, ResizeXCoor(TempXPosition), ResizeYCoor(TempBottom + 12), ResizeXCoor(TempXSize), ResizeYCoor(16),SWP_NOZORDER);
								SetDlgItemText(IDC_XScale5,dtoa(TempXValue,1));
							break;
							case 5:
								m_XScale6.ShowWindow(SW_HIDE);
								//m_XScale6.SetWindowPos(NULL, ResizeXCoor(TempXPosition), ResizeYCoor(TempBottom + 12), ResizeXCoor(TempXSize), ResizeYCoor(16),SWP_NOZORDER);
								//SetDlgItemText(IDC_XScale6,dtoa(TempXValue,1));
							break;
							case 6:
								m_XScale7.ShowWindow(SW_SHOW);
								m_XScale7.SetWindowPos(NULL, ResizeXCoor(TempXPosition), ResizeYCoor(TempBottom + 12), ResizeXCoor(TempXSize), ResizeYCoor(16),SWP_NOZORDER);
								SetDlgItemText(IDC_XScale7,dtoa(TempXValue,1));
							break;
							case 7:
								m_XScale8.ShowWindow(SW_HIDE);
								//m_XScale8.SetWindowPos(NULL, ResizeXCoor(TempXPosition), ResizeYCoor(TempBottom + 12), ResizeXCoor(TempXSize), ResizeYCoor(16),SWP_NOZORDER);
								//SetDlgItemText(IDC_XScale8,dtoa(TempXValue,1));
							break;
							case 8:
								m_XScale9.ShowWindow(SW_SHOW);
								m_XScale9.SetWindowPos(NULL, ResizeXCoor(TempXPosition), ResizeYCoor(TempBottom + 12), ResizeXCoor(TempXSize), ResizeYCoor(16),SWP_NOZORDER);
								SetDlgItemText(IDC_XScale9,dtoa(TempXValue,1));
							break;
							case 9:
								m_XScale10.ShowWindow(SW_HIDE);
								//m_XScale10.SetWindowPos(NULL, ResizeXCoor(TempXPosition), ResizeYCoor(TempBottom + 12), ResizeXCoor(TempXSize), ResizeYCoor(16),SWP_NOZORDER);
								//SetDlgItemText(IDC_XScale10,dtoa(TempXValue,1));
							break;
							case 10:
								m_XScale11.ShowWindow(SW_HIDE);
								//m_XScale11.SetWindowPos(NULL, ResizeXCoor(TempXPosition), ResizeYCoor(TempBottom + 12), ResizeXCoor(TempXSize), ResizeYCoor(16),SWP_NOZORDER);
								//SetDlgItemText(IDC_XScale11,dtoa(TempXValue,1));
							break;
							case 11:
								m_XScale12.ShowWindow(SW_HIDE);
								//m_XScale12.SetWindowPos(NULL, ResizeXCoor(TempXPosition), ResizeYCoor(TempBottom + 12), ResizeXCoor(TempXSize), ResizeYCoor(16),SWP_NOZORDER);
								//SetDlgItemText(IDC_XScale12,dtoa(TempXValue,1));
							break;
							case 12:
								m_XScale13.ShowWindow(SW_HIDE);
								//m_XScale13.SetWindowPos(NULL, ResizeXCoor(TempXPosition), ResizeYCoor(TempBottom + 12), ResizeXCoor(TempXSize), ResizeYCoor(16),SWP_NOZORDER);
								//SetDlgItemText(IDC_XScale13,dtoa(TempXValue,1));
							break;
							case 13:
								m_XScale14.ShowWindow(SW_HIDE);
								//m_XScale14.SetWindowPos(NULL, ResizeXCoor(TempXPosition), ResizeYCoor(TempBottom + 12), ResizeXCoor(TempXSize), ResizeYCoor(16),SWP_NOZORDER);
								//SetDlgItemText(IDC_XScale14,dtoa(TempXValue,1));
							break;
						}
					}

					//if last x index value not the maximum value, display the maximum value at the end

					if (vPlotDisplay == cTrendDisplay)
						TempTimesFactor = TempTimesFactor * vCheckWeighInspection->vCheckWeighDataSize;

					if (TempTimesFactor > 1)
					{
						WORD TempXSize = 27;
						if (TempTimesFactor > 99)
							TempXSize = 36;
						if (TempTimesFactor > 999)
							TempXSize = 42;
						if (TempTimesFactor > 9999)
							TempXSize = 48;

						WORD TempNewXPosition = 635 - TempXSize;

						WORD TempMaxDisplayPosition = TempXMiddle + TempWidth + 10;
						if (TempMaxDisplayPosition + TempXSize > 635)
							TempMaxDisplayPosition = 635 - TempXSize;
						m_XLabel3.SetWindowPos(NULL, ResizeXCoor(TempMaxDisplayPosition), ResizeYCoor(TempBottom + 12), ResizeXCoor(TempXSize), ResizeYCoor(16), SWP_NOZORDER);

						m_XLabel3.ShowWindow(SW_SHOW);
						SetDlgItemText(IDC_XLabel3, "X" + dtoa(TempTimesFactor,0));
					}
					else
						m_XLabel3.ShowWindow(SW_HIDE);

					m_YScale7.ShowWindow(SW_HIDE);
					m_YScale8.ShowWindow(SW_HIDE);
					m_YScale9.ShowWindow(SW_HIDE);
					m_YScale10.ShowWindow(SW_HIDE);
					m_YScale11.ShowWindow(SW_HIDE);
					m_YScale12.ShowWindow(SW_HIDE);
					m_YScale13.ShowWindow(SW_HIDE);
					m_YScale14.ShowWindow(SW_HIDE);
					m_YScale15.ShowWindow(SW_HIDE);

					int TempLeftSidePosition = 1;
					if (TempLeftSide > 55)
						TempLeftSidePosition = TempLeftSide - 54;

					if (TempLeftSidePosition < 1)
						TempLeftSidePosition = -1;

					for (DWORD TempLoop = 0; TempLoop < vYNumberOfScaleLines; TempLoop++)
					{
						WORD TempY = (WORD)(TempBottom  - (((TempLoop) *  vYInterval) * vYScale));
						if (TempY < 44)
							TempY = 44;

						switch (TempLoop)
						{
							case 0:
								m_YScale1.ShowWindow(SW_SHOW);
								m_YScale1.SetWindowPos(NULL, ResizeXCoor(TempLeftSidePosition), ResizeYCoor(TempY - 4), ResizeXCoor(44), ResizeYCoor(16),SWP_NOZORDER);
								SetDlgItemText(IDC_YScale1,dtoa((TempLoop * vYInterval) + vPlotMinimumY,1));
							break;
							case 1:
								m_YScale2.ShowWindow(SW_SHOW);
								m_YScale2.SetWindowPos(NULL, ResizeXCoor(TempLeftSidePosition), ResizeYCoor(TempY - 4), ResizeXCoor(44), ResizeYCoor(16),SWP_NOZORDER);
								SetDlgItemText(IDC_YScale2,dtoa((TempLoop * vYInterval) + vPlotMinimumY,1));
							break;
							case 2:
								m_YScale3.ShowWindow(SW_SHOW);
								m_YScale3.SetWindowPos(NULL, ResizeXCoor(TempLeftSidePosition), ResizeYCoor(TempY - 4), ResizeXCoor(44), ResizeYCoor(16),SWP_NOZORDER);
								SetDlgItemText(IDC_YScale3,dtoa((TempLoop * vYInterval) + vPlotMinimumY,1));
							break;
							case 3:
								m_YScale4.ShowWindow(SW_SHOW);
								m_YScale4.SetWindowPos(NULL, ResizeXCoor(TempLeftSidePosition), ResizeYCoor(TempY - 4), ResizeXCoor(44), ResizeYCoor(16),SWP_NOZORDER);
								SetDlgItemText(IDC_YScale4,dtoa((TempLoop * vYInterval) + vPlotMinimumY,1));
							break;
							case 4:
								m_YScale5.ShowWindow(SW_SHOW);
								m_YScale5.SetWindowPos(NULL, ResizeXCoor(TempLeftSidePosition), ResizeYCoor(TempY - 4), ResizeXCoor(44), ResizeYCoor(16),SWP_NOZORDER);
								SetDlgItemText(IDC_YScale5,dtoa((TempLoop * vYInterval) + vPlotMinimumY,1));
							break;
							case 5:
								m_YScale6.ShowWindow(SW_SHOW);
								m_YScale6.SetWindowPos(NULL, ResizeXCoor(TempLeftSidePosition), ResizeYCoor(TempY - 4), ResizeXCoor(44), ResizeYCoor(16),SWP_NOZORDER);
								SetDlgItemText(IDC_YScale6,dtoa((TempLoop * vYInterval) + vPlotMinimumY,1));
							break;
							case 6:
								m_YScale7.ShowWindow(SW_SHOW);
								m_YScale7.SetWindowPos(NULL, ResizeXCoor(TempLeftSidePosition), ResizeYCoor(TempY - 4), ResizeXCoor(44), ResizeYCoor(16),SWP_NOZORDER);
								SetDlgItemText(IDC_YScale7,dtoa((TempLoop * vYInterval) + vPlotMinimumY,1));
							break;
							case 7:
								m_YScale8.ShowWindow(SW_SHOW);
								m_YScale8.SetWindowPos(NULL, ResizeXCoor(TempLeftSidePosition), ResizeYCoor(TempY - 4), ResizeXCoor(44), ResizeYCoor(16),SWP_NOZORDER);
								SetDlgItemText(IDC_YScale8,dtoa((TempLoop * vYInterval) + vPlotMinimumY,1));
							break;
							case 8:
								m_YScale9.ShowWindow(SW_SHOW);
								m_YScale9.SetWindowPos(NULL, ResizeXCoor(TempLeftSidePosition), ResizeYCoor(TempY - 4), ResizeXCoor(44), ResizeYCoor(16),SWP_NOZORDER);
								SetDlgItemText(IDC_YScale9,dtoa((TempLoop * vYInterval) + vPlotMinimumY,1));
							break;
							case 9:
								m_YScale10.ShowWindow(SW_SHOW);
								m_YScale10.SetWindowPos(NULL, ResizeXCoor(TempLeftSidePosition), ResizeYCoor(TempY - 4), ResizeXCoor(44), ResizeYCoor(16),SWP_NOZORDER);
								SetDlgItemText(IDC_YScale10,dtoa((TempLoop * vYInterval) + vPlotMinimumY,1));
							break;
							case 10:
								m_YScale11.ShowWindow(SW_SHOW);
								m_YScale11.SetWindowPos(NULL, ResizeXCoor(TempLeftSidePosition), ResizeYCoor(TempY - 4), ResizeXCoor(44), ResizeYCoor(16),SWP_NOZORDER);
								SetDlgItemText(IDC_YScale11,dtoa((TempLoop * vYInterval) + vPlotMinimumY,1));
							break;
							case 11:
								m_YScale12.ShowWindow(SW_SHOW);
								m_YScale12.SetWindowPos(NULL, ResizeXCoor(TempLeftSidePosition), ResizeYCoor(TempY - 4), ResizeXCoor(44), ResizeYCoor(16),SWP_NOZORDER);
								SetDlgItemText(IDC_YScale12,dtoa((TempLoop * vYInterval) + vPlotMinimumY,1));
							break;
							case 12:
								m_YScale13.ShowWindow(SW_SHOW);
								m_YScale13.SetWindowPos(NULL, ResizeXCoor(TempLeftSidePosition), ResizeYCoor(TempY - 4), ResizeXCoor(44), ResizeYCoor(16),SWP_NOZORDER);
								SetDlgItemText(IDC_YScale13,dtoa((TempLoop * vYInterval) + vPlotMinimumY,1));
							break;
							case 13:
								m_YScale14.ShowWindow(SW_SHOW);
								m_YScale14.SetWindowPos(NULL, ResizeXCoor(TempLeftSidePosition), ResizeYCoor(TempY - 4), ResizeXCoor(44), ResizeYCoor(16),SWP_NOZORDER);
								SetDlgItemText(IDC_YScale14,dtoa((TempLoop * vYInterval) + vPlotMinimumY,1));
							break;
							case 14:
								m_YScale15.ShowWindow(SW_SHOW);
								m_YScale15.SetWindowPos(NULL, ResizeXCoor(TempLeftSidePosition), ResizeYCoor(TempY - 4), ResizeXCoor(44), ResizeYCoor(16),SWP_NOZORDER);
								SetDlgItemText(IDC_YScale15,dtoa((TempLoop * vYInterval) + vPlotMinimumY,1));
							break;
							default:
							break;
						} //end switch
					}  //end for label
				} //end of minutes or < 9 numbers
			} //end have points to plot
		} //end max y > min y
		else
		{
			SetDlgItemText(IDC_LineLabel3, _T("X Data Problem or too few points to plot"));
		}
		UpdatePlot();
	}
	else
	{
		SetDlgItemText(IDC_LineLabel3, _T("Out of Memory to plot"));
	}
}

BYTE CCheckWeighDialog::FindInspectionLinkedToMe()
{
	BYTE TempReturn = 0;

	for (BYTE TempLoop = 0; TempLoop < vGlobalCurrentProduct->vNumberOfInspections; TempLoop++)
	if (vGlobalCurrentProduct->vInspection[TempLoop])
	if ((vGlobalCurrentProduct->vInspection[TempLoop]->vInspectionType == cUnderfillByWeightInspection) || (vGlobalCurrentProduct->vInspection[TempLoop]->vInspectionType == cOverfillByWeightInspection))
	if (vCheckWeighInspection != vGlobalCurrentProduct->vInspection[TempLoop])
	if (vCheckWeighInspection->vName == vGlobalCurrentProduct->vInspection[TempLoop]->vInspectionNameForWeightTrendingName) //if found an inspection that points to this inspection, and a different type
	if (vCheckWeighInspection->vInspectionType != vGlobalCurrentProduct->vInspection[TempLoop]->vInspectionType)
	{
		TempReturn = TempLoop + 1;
		TempLoop = vGlobalCurrentProduct->vNumberOfInspections;
	}

	return TempReturn;
}


void CCheckWeighDialog::PrepareAndExit(BYTE TempCode)
{
	if (vPointerToAdjustedData)
		free(vPointerToAdjustedData);

	CDialog::EndDialog(TempCode);
}

void CCheckWeighDialog::OnDialogtitlestatictext1()
{
	BYTE TempHiddenKeys = 8;
	if (!PasswordOK(cTemporaryInspxPassword,false))
		TempHiddenKeys = 10;
	PostThreadMessage(vGlobalCallingThreadID, cDisplaySoftKeyMenu, TempHiddenKeys, TempHiddenKeys);
}

void CCheckWeighDialog::DrawCompletelyNewPlot()
{
	vGrowingSoDontBlank = false;

	m_MaximumTrendThreshold.ShowWindow(SW_HIDE);
	m_MinimumTrendThreshold.ShowWindow(SW_HIDE);
	m_XLabel3.ShowWindow(SW_HIDE);
	m_XScale1.ShowWindow(SW_HIDE);
	m_XScale2.ShowWindow(SW_HIDE);
	m_XScale3.ShowWindow(SW_HIDE);
	m_XScale4.ShowWindow(SW_HIDE);
	m_XScale5.ShowWindow(SW_HIDE);
	m_XScale6.ShowWindow(SW_HIDE);
	m_XScale7.ShowWindow(SW_HIDE);
	m_XScale8.ShowWindow(SW_HIDE);
	m_XScale9.ShowWindow(SW_HIDE);
	m_XScale10.ShowWindow(SW_HIDE);
	m_XScale11.ShowWindow(SW_HIDE);
	m_XScale12.ShowWindow(SW_HIDE);
	m_XScale13.ShowWindow(SW_HIDE);
	m_XScale14.ShowWindow(SW_HIDE);
	m_YScale1.ShowWindow(SW_HIDE);
	m_YScale2.ShowWindow(SW_HIDE);
	m_YScale3.ShowWindow(SW_HIDE);
	m_YScale4.ShowWindow(SW_HIDE);
	m_YScale5.ShowWindow(SW_HIDE);
	m_YScale6.ShowWindow(SW_HIDE);
	m_YScale7.ShowWindow(SW_HIDE);
	m_YScale8.ShowWindow(SW_HIDE);
	m_YScale9.ShowWindow(SW_HIDE);
	m_YScale10.ShowWindow(SW_HIDE);
	m_YScale11.ShowWindow(SW_HIDE);
	m_YScale12.ShowWindow(SW_HIDE);
	m_YScale13.ShowWindow(SW_HIDE);
	m_YScale14.ShowWindow(SW_HIDE);
	m_YScale15.ShowWindow(SW_HIDE);
}
