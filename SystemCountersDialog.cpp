// ScanTrac Side View
// SystemCountersDialog.cpp : implementation file
//

#include "stdafx.h"
#include "scantrac.h"
#include "SystemCountersDialog.h"
#include "YesNoDialog.h"
#include "AlphaEntryDialog.h"
#include "DiagnosticCountersDialog.h"
#include "PasswordEntryDialog.h"
#include "SelectItemDialog.h"
#include "NumericEntryDialog.h"
#include "HelpDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSystemCountersDialog dialog
extern BYTE vGlobalShiftKeyDown;
extern CProduct *vGlobalCurrentProduct;
extern CLifeTimeCounters *vGlobalLifeTimeCounters;
extern int vGlobalPasswordLevel;
extern unsigned long vGlobalDialogTitleColor;

CSystemCountersDialog::CSystemCountersDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CSystemCountersDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSystemCountersDialog)
	//}}AFX_DATA_INIT
	vOldXRaySourceRunDuration = 0;
	vOldDetectorsRunDuration = 0; //keeps track of how many seconds Detectors
	vOldScanTracRunDuration = 0; //keeps track of how many seconds ScanTrac
	vMakeExitButtonGreen = false;
	vUpdateDisplayTimerHandle = 57;
	vChangeMade = false;
	vOldXRayOnNotice = false;
	vOldXRaySourceWarrantyDuration = 0;
	vOldDetectorsWarrantyDuration = 0;
	vSupportContractStatus = 0;

	vLocalCWndCollection.Add(this);
	vLocalCWndCollection.Add(&m_SubFunction1eButton2);
	vLocalCWndCollection.Add(&m_SubFunction8eButton);
	vLocalCWndCollection.Add(&m_SubFunction6eButton);
	vLocalCWndCollection.Add(&m_SubFunction7eButton);
	vLocalCWndCollection.Add(&m_SubFunction5eButton);
	vLocalCWndCollection.Add(&m_Function5Button);
	vLocalCWndCollection.Add(&m_Function2Button);
	vLocalCWndCollection.Add(&m_Function1Button);
	vLocalCWndCollection.Add(&m_Background);
	vLocalCWndCollection.Add(&m_XRaysOn);
	vLocalCWndCollection.Add(&m_SubFunction8Button);
	vLocalCWndCollection.Add(&m_SubFunction7Button);
	vLocalCWndCollection.Add(&m_SubFunction6Button);
	vLocalCWndCollection.Add(&m_SubFunction5Button);
	vLocalCWndCollection.Add(&m_SubFunction4Button);
	vLocalCWndCollection.Add(&m_SubFunction3Button);
	vLocalCWndCollection.Add(&m_SubFunction2Button);
	vLocalCWndCollection.Add(&m_SubFunction1Button);
	vLocalCWndCollection.Add(&m_Function3Button);
	vLocalCWndCollection.Add(&m_DetectorRunDuration);
	vLocalCWndCollection.Add(&m_DetectorRunDuration2);
	vLocalCWndCollection.Add(&m_DetectorRunDuration3);
	vLocalCWndCollection.Add(&m_DetectorRunDurationLabel);
	vLocalCWndCollection.Add(&m_DetectorRunDurationLabel2);
	vLocalCWndCollection.Add(&m_DetectorRunDurationLabel3);
	vLocalCWndCollection.Add(&m_DetectorWarrantyDate);
	vLocalCWndCollection.Add(&m_DetectorWarrantyDateLabel);
	vLocalCWndCollection.Add(&m_DialogTitleStaticText);
	vLocalCWndCollection.Add(&m_Function4Button);
	vLocalCWndCollection.Add(&m_MonthsSinceCommissioned);
	vLocalCWndCollection.Add(&m_MonthsSinceCommissionedLabel);
	vLocalCWndCollection.Add(&m_ScanTracCommissionedDate);
	vLocalCWndCollection.Add(&m_ScanTracCommissionedDateLabel);
	vLocalCWndCollection.Add(&m_ScanTracID);
	vLocalCWndCollection.Add(&m_ScanTracRunDuration);
	vLocalCWndCollection.Add(&m_ScanTracRunDurationLabel);
	vLocalCWndCollection.Add(&m_SupportContract);
	vLocalCWndCollection.Add(&m_SupportContract2);
	vLocalCWndCollection.Add(&m_TodaysDate);
	vLocalCWndCollection.Add(&m_TubeWarrantyDate);
	vLocalCWndCollection.Add(&m_TubeWarrantyDateLabel);
	vLocalCWndCollection.Add(&m_XRaySourceDuration);
	vLocalCWndCollection.Add(&m_XRaySourceDuration2);
	vLocalCWndCollection.Add(&m_XRaySourceDuration3);
	vLocalCWndCollection.Add(&m_XRaySourceDurationLabel);
	vLocalCWndCollection.Add(&m_XRaySourceDurationLabel2);
	vLocalCWndCollection.Add(&m_XRaySourceDurationLabel3);
	vLocalCWndCollection.Add(&m_XRayTubeSerialNumber);
	vLocalCWndCollection.Add(&m_XRayTubeSerialNumberLabel);
}


void CSystemCountersDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSystemCountersDialog)
	DDX_Control(pDX, IDC_SubFunction1eButton, m_SubFunction1eButton2);
	DDX_Control(pDX, IDC_SubFunction8eButton, m_SubFunction8eButton);
	DDX_Control(pDX, IDC_SubFunction6eButton, m_SubFunction6eButton);
	DDX_Control(pDX, IDC_SubFunction7eButton, m_SubFunction7eButton);
	DDX_Control(pDX, IDC_SubFunction5eButton, m_SubFunction5eButton);
	DDX_Control(pDX, IDC_Function5Button, m_Function5Button);
	DDX_Control(pDX, IDC_Function2Button, m_Function2Button);
	DDX_Control(pDX, IDC_Function1Button, m_Function1Button);
	DDX_Control(pDX, IDC_Background, m_Background);
	DDX_Control(pDX, IDC_XRaysOn, m_XRaysOn);
	DDX_Control(pDX, IDC_SubFunction8Button, m_SubFunction8Button);
	DDX_Control(pDX, IDC_SubFunction7Button, m_SubFunction7Button);
	DDX_Control(pDX, IDC_SubFunction6Button, m_SubFunction6Button);
	DDX_Control(pDX, IDC_SubFunction5Button, m_SubFunction5Button);
	DDX_Control(pDX, IDC_SubFunction4Button, m_SubFunction4Button);
	DDX_Control(pDX, IDC_SubFunction3Button, m_SubFunction3Button);
	DDX_Control(pDX, IDC_SubFunction2Button, m_SubFunction2Button);
	DDX_Control(pDX, IDC_SubFunction1Button, m_SubFunction1Button);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_Function3Button, m_Function3Button);
	DDX_Control(pDX, IDC_DetectorRunDuration, m_DetectorRunDuration);
	DDX_Control(pDX, IDC_DetectorRunDuration2, m_DetectorRunDuration2);
	DDX_Control(pDX, IDC_DetectorRunDuration3, m_DetectorRunDuration3);
	DDX_Control(pDX, IDC_DetectorRunDurationLabel, m_DetectorRunDurationLabel);
	DDX_Control(pDX, IDC_DetectorRunDurationLabel2, m_DetectorRunDurationLabel2);
	DDX_Control(pDX, IDC_DetectorRunDurationLabel3, m_DetectorRunDurationLabel3);
	DDX_Control(pDX, IDC_DetectorWarrantyDate, m_DetectorWarrantyDate);
	DDX_Control(pDX, IDC_DetectorWarrantyDateLabel, m_DetectorWarrantyDateLabel);
	DDX_Control(pDX, IDC_DialogTitleStaticText1, m_DialogTitleStaticText);
	DDX_Control(pDX, IDC_Function4Button, m_Function4Button);
	DDX_Control(pDX, IDC_MonthsSinceCommissioned, m_MonthsSinceCommissioned);
	DDX_Control(pDX, IDC_MonthsSinceCommissionedLabel, m_MonthsSinceCommissionedLabel);
	DDX_Control(pDX, IDC_ScanTracCommissionedDate, m_ScanTracCommissionedDate);
	DDX_Control(pDX, IDC_ScanTracCommissionedDateLabel, m_ScanTracCommissionedDateLabel);
	DDX_Control(pDX, IDC_ScanTracID, m_ScanTracID);
	DDX_Control(pDX, IDC_ScanTracRunDuration, m_ScanTracRunDuration);
	DDX_Control(pDX, IDC_ScanTracRunDurationLabel, m_ScanTracRunDurationLabel);
	DDX_Control(pDX, IDC_SupportContract, m_SupportContract);
	DDX_Control(pDX, IDC_SupportContract2, m_SupportContract2);
	DDX_Control(pDX, IDC_TodaysDate, m_TodaysDate);
	DDX_Control(pDX, IDC_TubeWarrantyDate, m_TubeWarrantyDate);
	DDX_Control(pDX, IDC_TubeWarrantyDateLabel, m_TubeWarrantyDateLabel);
	DDX_Control(pDX, IDC_XRaySourceDuration, m_XRaySourceDuration);
	DDX_Control(pDX, IDC_XRaySourceDuration2, m_XRaySourceDuration2);
	DDX_Control(pDX, IDC_XRaySourceDuration3, m_XRaySourceDuration3);
	DDX_Control(pDX, IDC_XRaySourceDurationLabel, m_XRaySourceDurationLabel);
	DDX_Control(pDX, IDC_XRaySourceDurationLabel2, m_XRaySourceDurationLabel2);
	DDX_Control(pDX, IDC_XRaySourceDurationLabel3, m_XRaySourceDurationLabel3);
	DDX_Control(pDX, IDC_XRayTubeSerialNumber, m_XRayTubeSerialNumber);
	DDX_Control(pDX, IDC_XRayTubeSerialNumberLabel, m_XRayTubeSerialNumberLabel);
}


BEGIN_MESSAGE_MAP(CSystemCountersDialog, CDialog)
	//{{AFX_MSG_MAP(CSystemCountersDialog)
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
	ON_BN_CLICKED(IDC_SubFunction5eButton, OnSubFunction5eButton)
	ON_BN_CLICKED(IDC_SubFunction6eButton, OnSubFunction6eButton)
	ON_BN_CLICKED(IDC_SubFunction7eButton, OnSubFunction7eButton)
	ON_BN_CLICKED(IDC_SubFunction8eButton, OnSubFunction8eButton)
	ON_BN_CLICKED(IDC_SubFunction1eButton, OnSubFunction1eButton)
	//}}AFX_MSG_MAP
	ON_STN_CLICKED(IDC_DialogTitleStaticText1, &CSystemCountersDialog::OnDialogtitlestatictext1)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSystemCountersDialog message handlers

void CSystemCountersDialog::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	m_Background.SetWindowPos(NULL,cMainBackgroundLeft,
		cMainBackgroundTop,cMainBackgroundLength,
		cMainBackgroundHeight,SWP_NOZORDER);

	ShowDisplay();
	UpdateDisplay();

	if (vLocalSystemData->vInAutoSetup)
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
	SetDlgItemText(IDC_ScanTracID,"ScanTrac: " + vLocalConfigurationData->vScanTracID);

	UpdateButtons();
	
	int TempTimerResult = SetTimer(vUpdateDisplayTimerHandle,500,NULL);
	if (!TempTimerResult)
		ReportErrorMessage("Error-Update Display Timer Failed",cEMailInspx,32000);
	this->SetWindowText(_T("Counters"));
	
	::SetupMenu(vLocalCWndCollection);
}

void CSystemCountersDialog::OnFunction1Button() 
{
	// User clicked Main Menu Button to exit 
	if ((!vChangeMade) || (AbortSetupChanges()))
	{
		CDialog::EndDialog(10);
	}
}

void CSystemCountersDialog::OnFunction2Button() 
{
	// Save and Exit Button was pressed
	if ((!vChangeMade) || (KeepSetupChanges()))
	{
		CDialog::EndDialog(true);
	}
}

void CSystemCountersDialog::OnFunction3Button() 
{
	// Service Contract
	//Enter level, and expiration date

	if (PasswordOK(cTemporaryInspxPassword,false))
	{
		//set service contract type
		CSelectItemDialog ISelectItemDialog;  
		ISelectItemDialog.vNumberOfItems = 5;
		ISelectItemDialog.vItems[0] = "Contract Expired";
		ISelectItemDialog.vItems[1] = "Support Contract 8/5";
		ISelectItemDialog.vItems[2] = "Support Contract 8/16";
		ISelectItemDialog.vItems[3] = "Support Contract 24/7";
		ISelectItemDialog.vItems[4] = "None, and Hide Support Contract Display";
		//ISelectItemDialog.vItems[5] = "No Contract - Expired";
		//enum {cSupportContractInvalid, cSupportContract8_5, cSupportContract16_5, cSupportContract24_7, cHideSupportContractDisplay};
		//vSupportContractType
		//int vSupportContractDayExpires;
		//int vSupportContractMonthExpires;
		//int vSupportContractYearExpires;

		//ISelectItemDialog.vYellowItem = 2;

		//Set dialog box data titles and number value
		ISelectItemDialog.vTitleString = "Select the Service Contract Type";
		ISelectItemDialog.vBackgroundColor = cGreen;
		ISelectItemDialog.vShowExit = true;

		ISelectItemDialog.m_DialogTitleStaticText2 = "Previous Contract: " + vLocalSystemData->vSupportContractTypeStrings[vLocalConfigurationData->vSupportContractType];
		ISelectItemDialog.m_DialogTitleStaticText3 = "";
		//Pass control to dialog box and display
		int TempResult = ISelectItemDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (TempResult == IDOK)
		{
			vLocalConfigurationData->vSupportContractType = ISelectItemDialog.vItemNumberSelected - 1;

			if (vLocalConfigurationData->vSupportContractType == cSupportContractInvalid)
			{
				vLocalConfigurationData->vSupportContractYearExpires = 2013;
				vLocalConfigurationData->vSupportContractMonthExpires = 1;
				vLocalConfigurationData->vSupportContractDayExpires = 1;
				vLocalConfigurationData->vSupportContractType = cSupportContractExpired;
			}
		}
		else
		if (TempResult != 11)
		{
			TempResult = false;
			if (TempResult == 10)
			{
				Sleep(1); //is the equivelent of a yeild statement;
				CDialog::EndDialog(10);
			}
		}

		//set date
		if (vLocalConfigurationData->vSupportContractType != cHideSupportContractDisplay)
		if (vLocalConfigurationData->vSupportContractType != cSupportContractExpired)
		{
			WORD TempMonth = 0;
			WORD TempDay = 0;
			WORD TempYear = 0;

			CTime TempTime = CTime::GetCurrentTime();
			CString TempTimeString = TempTime.Format("%#I:%M:%S%p");
		
			CString TempDateString = vMainWindowPointer->SupportContracExpirationString();

			CString TempTimeString1 = TempTime.Format("%d, %H:%M:%S");

			SYSTEMTIME TempSystemTime1;
			GetLocalTime(&TempSystemTime1);
			TempMonth = TempSystemTime1.wMonth;
			TempDay = TempSystemTime1.wDay;
			TempYear = TempSystemTime1.wYear + 1;

			CSelectItemDialog ISelectItemDialog;  
			ISelectItemDialog.vNumberOfItems = 12;
			ISelectItemDialog.vItems[0] = "January";
			ISelectItemDialog.vItems[1] = "February";
			ISelectItemDialog.vItems[2] = "March";
			ISelectItemDialog.vItems[3] = "April";
			ISelectItemDialog.vItems[4] = "May";
			ISelectItemDialog.vItems[5] = "June";
			ISelectItemDialog.vItems[6] = "July";
			ISelectItemDialog.vItems[7] = "August";
			ISelectItemDialog.vItems[8] = "September";
			ISelectItemDialog.vItems[9] = "October";
			ISelectItemDialog.vItems[10] = "November";
			ISelectItemDialog.vItems[11] = "December";

			//ISelectItemDialog.vYellowItem = 1;

			//Set dialog box data titles and number value
			ISelectItemDialog.vTitleString = "Select the Month";
			ISelectItemDialog.vBackgroundColor = cGreen;
			ISelectItemDialog.vShowExit = true;

			ISelectItemDialog.m_DialogTitleStaticText2 = "Support Contract Expires";
			ISelectItemDialog.m_DialogTitleStaticText3 = "OK = this month, Original Date: " + TempDateString;
			//Pass control to dialog box and display
			int TempResult = ISelectItemDialog.DoModal();
			//dialog box is now closed, if user pressed select do this
			//if user pressed cancel, do nothing
			if (TempResult == IDOK)
			{
				TempMonth = ISelectItemDialog.vItemNumberSelected;
			}
			else
			if (TempResult != 11)
			{
				TempResult = false;
				if (TempResult == 10)
				{
					Sleep(1); //is the equivelent of a yeild statement;
					CDialog::EndDialog(10);
				}
			}

			{
				CNumericEntryDialog INumericEntryDialog;  
				if (TempResult)
				{
					//Set dialog box data titles and number value
					INumericEntryDialog.vEditString = dtoa(TempDay, 0); 
					INumericEntryDialog.m_DialogTitleStaticText1 = "Enter the Day of the Month";
					INumericEntryDialog.m_DialogTitleStaticText2 = "";
					INumericEntryDialog.m_UnitsString = "";

					INumericEntryDialog.m_DialogTitleStaticText3 = "Support Contract Expires";
					INumericEntryDialog.m_DialogTitleStaticText4 = "Original Date: " + TempDateString;

					INumericEntryDialog.vMaxValue = 31;
					INumericEntryDialog.vMinValue = 1;
					INumericEntryDialog.vIntegerOnly = true;
					//Pass control to dialog box and display
					TempResult = INumericEntryDialog.DoModal();
					//dialog box is now closed, if user pressed select do this
					//if user pressed cancel, do nothing
					if (TempResult == IDOK)
						TempDay = _wtoi(INumericEntryDialog.vEditString);
					else
					{
						TempResult = false;
						if (TempResult == 10)
						{
							Sleep(1); //is the equivelent of a yeild statement;
							CDialog::EndDialog(10);
						}
					}
				}
				
				if (TempResult)
				{
					//Set dialog box data titles and number value
					INumericEntryDialog.vEditString = dtoa(TempYear, 0); 
					INumericEntryDialog.m_DialogTitleStaticText1 = "Enter Year Support Contract Expires";
					INumericEntryDialog.m_DialogTitleStaticText2 = "";
					INumericEntryDialog.m_UnitsString = "";

					INumericEntryDialog.m_DialogTitleStaticText3 = "Support Contract Expires";
					INumericEntryDialog.m_DialogTitleStaticText4 = "Original Date: " + TempDateString;

					INumericEntryDialog.vMaxValue = 3000;
					if (PasswordOK(cSuperInspxPassword,false))
						INumericEntryDialog.vMinValue = 1970;
					else
						INumericEntryDialog.vMinValue = 1999;

					INumericEntryDialog.vMaxValue = 2037;
					INumericEntryDialog.vIntegerOnly = true;
					//Pass control to dialog box and display
					TempResult = INumericEntryDialog.DoModal();
					//dialog box is now closed, if user pressed select do this
					//if user pressed cancel, do nothing
					if (TempResult == IDOK)
					{
						TempYear = _wtoi(INumericEntryDialog.vEditString);
					}
					else
					{
						TempResult = false;
						if (TempResult == 10)
						{
							Sleep(1); //is the equivelent of a yeild statement;
							CDialog::EndDialog(10);
						}
					}
				}
			}
			
			if (TempResult)
			{
				if ((TempYear) && (TempDay) && (TempMonth))
				{

					vLocalConfigurationData->vSupportContractDayExpires = TempDay;
					vLocalConfigurationData->vSupportContractMonthExpires = TempMonth;
					vLocalConfigurationData->vSupportContractYearExpires = TempYear;

					//CTime TempTime2(TempYear,TempMonth,TempDay,1,1,1,1);
					//TempTime = TempTime2;
					//CTime TempCurrentTime;
					//TempCurrentTime = CTime::GetCurrentTime();
					//if (TempTime2 > TempCurrentTime)
					//{
					//	CNoticeDialog TempNoticeDialog;
					//	TempNoticeDialog.vNoticeText = "\n\n\nError - Future Dates Not Allowed\nCommission Date has NOT been set";
					//	TempNoticeDialog.vType = cErrorMessage;
					//	TempNoticeDialog.DoModal();
					//	TempResult = false;
					//}
				}
				else
				{
					CNoticeDialog TempNoticeDialog;
					TempNoticeDialog.vNoticeText = "\n\n\nError - Date not correct\nSupport Contract Expiration Date has NOT been set";
					TempNoticeDialog.vType = cErrorMessage;
					TempNoticeDialog.DoModal();
					TempResult = false;
				}
			}
		}
	}
	ShowDisplay();
	vMainWindowPointer->DisplaySupportContractStatus();
}

void CSystemCountersDialog::OnFunction4Button() 
{
	// TODO: Add your control notification handler code here
	
}

void CSystemCountersDialog::OnFunction5Button() 
{
	if (PasswordOK(cTemporaryInspxPassword,false))
	{
		//Create diagnostic dialog box
		CDiagnosticCountersDialog IDiagnosticCountersDialog;  

		//Set dialog box data like how many products there are
		IDiagnosticCountersDialog.vLocalSystemData = vLocalSystemData;
		IDiagnosticCountersDialog.vLocalConfigurationData = vLocalConfigurationData;
		IDiagnosticCountersDialog.vMainWindowPointer = vMainWindowPointer;

		//Pass control to dialog box and display
		int nResponse = IDiagnosticCountersDialog.DoModal();

		//if changes were made, save data to file
		if (nResponse == 10)
		{
			//Main Menu button pressed
			Sleep(1); //is the equivelent of a yeild statement;
			CDialog::EndDialog(10);
		}
		UpdateButtons();
	}
}

void CSystemCountersDialog::OnSubFunction1Button() 
{
	if (PasswordOK(cTemporaryInspxPassword,false))
	if (ResetPasswordOK())
	{
		//set date
		bool TempDoneWithDate = false;
		WORD TempMonth = 0;
		WORD TempDay = 0;
		WORD TempYear = 0;
		WORD TempHour = 0;
		WORD TempMinute = 0;
		WORD TempSecond = 0;

		CTime TempTime = CTime::GetCurrentTime();
		CString TempTimeString = TempTime.Format("%#I:%M:%S%p");
		
		CString TempDateString = TempTime.Format("%B %#d, %Y");

		CString TempTimeString1 = TempTime.Format("%d, %H:%M:%S");

		SYSTEMTIME TempSystemTime1;
		GetLocalTime(&TempSystemTime1);
		TempMonth = TempSystemTime1.wMonth;
		TempDay = TempSystemTime1.wDay;
		TempYear = TempSystemTime1.wYear;
		TempHour = TempSystemTime1.wHour;
		TempMinute = TempSystemTime1.wMinute;
		TempSecond = TempSystemTime1.wSecond;

		CSelectItemDialog ISelectItemDialog;  
		ISelectItemDialog.vNumberOfItems = 13;
		ISelectItemDialog.vItems[0] = "Use Today's Date";
		ISelectItemDialog.vItems[1] = "January";
		ISelectItemDialog.vItems[2] = "February";
		ISelectItemDialog.vItems[3] = "March";
		ISelectItemDialog.vItems[4] = "April";
		ISelectItemDialog.vItems[5] = "May";
		ISelectItemDialog.vItems[6] = "June";
		ISelectItemDialog.vItems[7] = "July";
		ISelectItemDialog.vItems[8] = "August";
		ISelectItemDialog.vItems[9] = "September";
		ISelectItemDialog.vItems[10] = "October";
		ISelectItemDialog.vItems[11] = "November";
		ISelectItemDialog.vItems[12] = "December";

		ISelectItemDialog.vYellowItem = 1;

		//Set dialog box data titles and number value
		ISelectItemDialog.vTitleString = "Select the Month";
		ISelectItemDialog.vBackgroundColor = cGreen;
		ISelectItemDialog.vShowExit = true;

		ISelectItemDialog.m_DialogTitleStaticText2 = "For Commission Date";
		ISelectItemDialog.m_DialogTitleStaticText3 = "Original Date: " + TempDateString;
		//Pass control to dialog box and display
		int TempResult = ISelectItemDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (TempResult == IDOK)
		{
			if (ISelectItemDialog.vItemNumberSelected == 1)
				TempDoneWithDate = true;
			else
				TempMonth = ISelectItemDialog.vItemNumberSelected - 1;
		}
		else
		if (TempResult != 11)
		{
			TempResult = false;
			if (TempResult == 10)
			{
				Sleep(1); //is the equivelent of a yeild statement;
				CDialog::EndDialog(10);
			}
		}

		if (!TempDoneWithDate)
		{
			CNumericEntryDialog INumericEntryDialog;  
			if (TempResult)
			{
				//Set dialog box data titles and number value
				INumericEntryDialog.vEditString = dtoa(TempDay, 0); 
				INumericEntryDialog.m_DialogTitleStaticText1 = "Enter the Day of the Month";
				INumericEntryDialog.m_DialogTitleStaticText2 = "";
				INumericEntryDialog.m_UnitsString = "";

				INumericEntryDialog.m_DialogTitleStaticText3 = "For Commission Date";
				INumericEntryDialog.m_DialogTitleStaticText4 = "Original Date: " + TempDateString;

				INumericEntryDialog.vMaxValue = 31;
				INumericEntryDialog.vMinValue = 1;
				INumericEntryDialog.vIntegerOnly = true;
				//Pass control to dialog box and display
				TempResult = INumericEntryDialog.DoModal();
				//dialog box is now closed, if user pressed select do this
				//if user pressed cancel, do nothing
				if (TempResult == IDOK)
					TempDay = _wtoi(INumericEntryDialog.vEditString);
				else
				{
					TempResult = false;
					if (TempResult == 10)
					{
						Sleep(1); //is the equivelent of a yeild statement;
						CDialog::EndDialog(10);
					}
				}
			}
				
			if (TempResult)
			{
				//Set dialog box data titles and number value
				INumericEntryDialog.vEditString = dtoa(TempYear, 0); 
				INumericEntryDialog.m_DialogTitleStaticText1 = "Enter Year";
				INumericEntryDialog.m_DialogTitleStaticText2 = "";
				INumericEntryDialog.m_UnitsString = "";

				INumericEntryDialog.m_DialogTitleStaticText3 = "For Commission Date";
				INumericEntryDialog.m_DialogTitleStaticText4 = "Original Date: " + TempDateString;

				INumericEntryDialog.vMaxValue = 3000;
				if (PasswordOK(cSuperInspxPassword,false))
					INumericEntryDialog.vMinValue = 1970;
				else
					INumericEntryDialog.vMinValue = 1999;

				INumericEntryDialog.vMaxValue = 2037;
				INumericEntryDialog.vIntegerOnly = true;
				//Pass control to dialog box and display
				TempResult = INumericEntryDialog.DoModal();
				//dialog box is now closed, if user pressed select do this
				//if user pressed cancel, do nothing
				if (TempResult == IDOK)
				{
					TempYear = _wtoi(INumericEntryDialog.vEditString);
					if (TempYear < 1970)
						TempYear = 1970;
				}
				else
				{
					TempResult = false;
					if (TempResult == 10)
					{
						Sleep(1); //is the equivelent of a yeild statement;
						CDialog::EndDialog(10);
					}
				}
			}
		}
			
		if (TempResult)
		{
			if ((TempYear) && (TempDay) && (TempMonth))
			{
				CTime TempTime2(TempYear,TempMonth,TempDay,1,1,1,1);
				TempTime = TempTime2;
				CTime TempCurrentTime;
				TempCurrentTime = CTime::GetCurrentTime();
				if (TempTime2 > TempCurrentTime)
				{
					CNoticeDialog TempNoticeDialog;
					TempNoticeDialog.vNoticeText = "\n\n\nError - Future Dates Not Allowed\nCommission Date has NOT been set";
					TempNoticeDialog.vType = cErrorMessage;
					TempNoticeDialog.DoModal();
					TempResult = false;
				}
			}
			else
			{
				CNoticeDialog TempNoticeDialog;
				TempNoticeDialog.vNoticeText = "\n\n\nError - Date not correct\nCommission Date has NOT been set";
				TempNoticeDialog.vType = cErrorMessage;
				TempNoticeDialog.DoModal();
				TempResult = false;
			}
		}
		bool TempClearCounters = false;
		
		CYesNoDialog TempYesNoDialog;
		TempYesNoDialog.vNoticeText = _T("\n\n\nClear All Run Time and Warranty Counters?");
		TempYesNoDialog.vYesButtonText = "Clear Counters";
		TempYesNoDialog.vQuestionType = cConfirmQuestion;
		TempResult = TempYesNoDialog.DoModal();
		if (TempResult == IDOK)
		{
			TempClearCounters = true;
		}
		

		CString TempName("");
		CString TempReason("");
		if (TempResult)
		if (GetName(&TempName))
		if (GetReason(&TempReason))
		{
			CString TempExtra = "";
			if (!TempClearCounters)
				TempExtra = " But Counters Not Cleared";

			ReportErrorMessage(TempName + " Commissioned ScanTrac " +
				vLocalConfigurationData->vScanTracID + " because " +
				TempReason + ".  New Commission Date is: " + 
				TempTime.Format("%B %#d, %Y") + 
				".  Previous Commission Date was: " + 
				vGlobalLifeTimeCounters->vCommisionDate.Format("%B %#d, %Y") + 
				" ScanTrac Run Time Counter was: " + 
				FormatRunTimeCounterString(vGlobalLifeTimeCounters->vScanTracRunDuration) + 
				TempExtra, 
				cEMailInspx, 32003);

			vGlobalLifeTimeCounters->vCommisionDate = TempTime;
			if (TempClearCounters)
			{
				ResetTubeWarranty(TempTime, TempReason, TempName);
				ResetDetectorWarranty(TempTime, TempReason, TempName);
				vGlobalLifeTimeCounters->vScanTracRunDuration = 0;
			}
			else
			{
				vGlobalLifeTimeCounters->vXRaySourceWarrantyDate = TempTime;
				vGlobalLifeTimeCounters->vDetectorsWarrantyDate = TempTime;
			}

		}
	}
	ShowDisplay();
}


void CSystemCountersDialog::OnSubFunction2Button() 
{
	if (PasswordOK(cTemporaryInspxPassword,false))
	{
		CString TempName("");
		CString TempReason("");
		if (GetName(&TempName))
		if (GetReason(&TempReason))
		{
			// Change Tube Serial Number
			CAlphaEntryDialog IAlphaEntryDialog;  
			IAlphaEntryDialog.vMainWindowPointer = vMainWindowPointer;
			//Set dialog box data titles and number value
			IAlphaEntryDialog.vEditString = vGlobalLifeTimeCounters->vXRayTubeSerialNumber;
			IAlphaEntryDialog.m_DialogTitleStaticText = "Enter X-Ray Tube Serial Number";
			IAlphaEntryDialog.vAllowMoreThan32Characters = true;
			IAlphaEntryDialog.vAllowPeriod = true;

			IAlphaEntryDialog.m_DialogTitleStaticText2 = "";
			IAlphaEntryDialog.vLocalSystemData = vLocalSystemData;
			//Pass control to dialog box and display
			int nResponse = IAlphaEntryDialog.DoModal();
			//dialog box is now closed, if user pressed select do this
			//if user pressed cancel, do nothing
			if (nResponse == IDOK)
			{

				ReportErrorMessage(TempName + " Changed X-Ray Tube Serial Number to " +
					IAlphaEntryDialog.vEditString + " because " +
					TempReason + ".  Previous X-Ray Tube Serial Number was: " +
					vGlobalLifeTimeCounters->vXRayTubeSerialNumber, 
					cEMailInspx, 32003);

				vGlobalLifeTimeCounters->vXRayTubeSerialNumber = IAlphaEntryDialog.vEditString;
				ShowDisplay();
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
}

void CSystemCountersDialog::OnSubFunction3Button() 
{
	if (PasswordOK(cTemporaryInspxPassword,false))
	{
		CString TempName("");
		CString TempReason("");
		//enter name, and reason for clearing
		CAlphaEntryDialog IAlphaEntryDialog;  
		IAlphaEntryDialog.vMainWindowPointer = vMainWindowPointer;
		//Set dialog box data titles and number value
		IAlphaEntryDialog.vEditString = "";
		IAlphaEntryDialog.m_DialogTitleStaticText = "Service Item Record Entry. Enter your name";
		IAlphaEntryDialog.vAllowMoreThan32Characters = true;
		IAlphaEntryDialog.vAllowPeriod = true;
		//IAlphaEntryDialog.vAllowComma = true; not available with vAllowPeriod
		IAlphaEntryDialog.vAllowDashes = true;
		IAlphaEntryDialog.vAllowParenthesis = true;
		IAlphaEntryDialog.vAllowSlashes = true;
		IAlphaEntryDialog.vAllowApproximate = true;

		IAlphaEntryDialog.m_DialogTitleStaticText2 = "";
		IAlphaEntryDialog.vLocalSystemData = vLocalSystemData;
		//Pass control to dialog box and display
		int nResponse = IAlphaEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			TempName = IAlphaEntryDialog.vEditString;
			IAlphaEntryDialog.vEditString = "";
			IAlphaEntryDialog.m_DialogTitleStaticText = "Enter Service Item(s) preformed for Service Log";
			//Pass control to dialog box and display
			int nResponse = IAlphaEntryDialog.DoModal();
			//dialog box is now closed, if user pressed select do this
			//if user pressed cancel, do nothing
			if (nResponse == IDOK)
			{
				TempReason = IAlphaEntryDialog.vEditString;

				ReportErrorMessage(TempName + " Performed Service Item: " +TempReason, 
					cEMailInspx, 32003);
			}
			else
			if (nResponse == 10)
			{
				//Main Menu button pressed
				Sleep(1); //is the equivelent of a yeild statement;
				CDialog::EndDialog(10);
			}
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

void CSystemCountersDialog::OnSubFunction4Button() 
{
	if (PasswordOK(cTemporaryInspxPassword,false))
	if (vGlobalLifeTimeCounters->vServiceRecordLog.GetLength() > 10)
	{
		CHelpDialog * IHelpDialog;
		IHelpDialog = new(CHelpDialog);
		IHelpDialog->vHelpContext = 16;
		IHelpDialog->vHelpType = 0;
		IHelpDialog->vWindowType = cTextWindow;
		//IHelpDialog->vLocalProductCollection = vLocalProductCollection;
		IHelpDialog->vLocalSystemData = vLocalSystemData;
		IHelpDialog->vMainWindowPointer = vMainWindowPointer;
		IHelpDialog->vLocalConfigurationData = vLocalConfigurationData;
		IHelpDialog->vProductPointer = vGlobalCurrentProduct;
		IHelpDialog->vTextString = vGlobalLifeTimeCounters->vServiceRecordLog;
		IHelpDialog->vTitleString = "Service Record Log for " + 
			vLocalConfigurationData->vScanTracID;
		int TempResult = IHelpDialog->DoModal();
		delete IHelpDialog;
	}
	else
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\n\nNo Entries in Service Record Log";
		TempNoticeDialog.vType = cInformationMessage;
		TempNoticeDialog.DoModal();
	}
}

void CSystemCountersDialog::OnSubFunction5Button() 
{
	if (PasswordOK(cTemporaryInspxPassword,false))
	if (ResetPasswordOK())
	{
		CYesNoDialog TempYesNoDialog;
		TempYesNoDialog.vNoticeText = _T("\n\n\nReset X-Ray Tube Warranty and Run Time Counters?\nThis is for a newly purchased warranty only.");
		TempYesNoDialog.vYesButtonText = "Clear Counters";
		TempYesNoDialog.vQuestionType = cConfirmQuestion;
		int TempResult = TempYesNoDialog.DoModal();
		if (TempResult == IDOK)
		{
			CString TempName("");
			CString TempReason("");
			if (GetName(&TempName))
			if (GetReason(&TempReason))
			{
				CTime TempTime = CTime::GetCurrentTime();
				ResetTubeWarranty(TempTime, TempReason, TempName);
			}
		}
	}
}

void CSystemCountersDialog::OnSubFunction5eButton() 
{
	if (PasswordOK(cSuperInspxPassword,false))
	{
		CNumericEntryDialog INumericEntryDialog;  
		//Set dialog box data titles and number value
		INumericEntryDialog.vEditString = 
			dtoa(vGlobalLifeTimeCounters->vXRaySourceWarrantyDuration / 3600,0); 
		INumericEntryDialog.m_DialogTitleStaticText1 = "X-Ray Tube Warranty Run Time Hours";
		INumericEntryDialog.m_DialogTitleStaticText2 = "";
		INumericEntryDialog.m_UnitsString = "";
		INumericEntryDialog.vMaxValue = 30000;

		INumericEntryDialog.m_DialogTitleStaticText3 = "Caution Manual Edit";
		INumericEntryDialog.m_DialogTitleStaticText4 = 
			"Original Value: " + INumericEntryDialog.vEditString;

		INumericEntryDialog.vIntegerOnly = true;
		//Pass control to dialog box and display
		int TempResult = INumericEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (TempResult == IDOK)
		{
			unsigned long int TempHours = _wtoi(INumericEntryDialog.vEditString);
			CString TempName("");
			CString TempReason("");
			if (GetName(&TempName))
			if (GetReason(&TempReason))
			{
				ReportErrorMessage(TempName + " Manually Set X-Ray Tube Warranty Run Time Counter to: " +
					INumericEntryDialog.vEditString + " hours because " +
					TempReason + ".  X-Ray Tube Warranty Run Time Counter was: " + 
					FormatRunTimeCounterString(vGlobalLifeTimeCounters->vXRaySourceWarrantyDuration), 
					cEMailInspx, 32003);
				vGlobalLifeTimeCounters->vXRaySourceWarrantyDuration = TempHours * 3600;
			}
		}
		else
		{
			if (TempResult == 10)
			{
				Sleep(1); //is the equivelent of a yeild statement;
				CDialog::EndDialog(10);
			}
		}
	}
}


void CSystemCountersDialog::OnSubFunction6Button() 
{
	if (PasswordOK(cTemporaryInspxPassword,false))
	if (ResetPasswordOK())
	{
		CYesNoDialog TempYesNoDialog;
		TempYesNoDialog.vNoticeText = _T("\n\n\nReset Detector Warranty and Run Time Counters?\nThis is for a newly purchased warranty only.");
		TempYesNoDialog.vYesButtonText = "Clear Counters";
		TempYesNoDialog.vQuestionType = cConfirmQuestion;
		int TempResult = TempYesNoDialog.DoModal();
		if (TempResult == IDOK)
		{
			CString TempName("");
			CString TempReason("");
			if (GetName(&TempName))
			if (GetReason(&TempReason))
			{
				CTime TempTime = CTime::GetCurrentTime();
				ResetDetectorWarranty(TempTime, TempReason, TempName);
			}
		}
	}
}

void CSystemCountersDialog::OnSubFunction6eButton() 
{
	if (PasswordOK(cSuperInspxPassword,false))
	{
		CNumericEntryDialog INumericEntryDialog;  
		//Set dialog box data titles and number value
		INumericEntryDialog.vEditString = 
			dtoa(vGlobalLifeTimeCounters->vDetectorsWarrantyDuration / 3600,0); 
		INumericEntryDialog.m_DialogTitleStaticText1 = "Detector Warranty Run Time Hours";
		INumericEntryDialog.m_DialogTitleStaticText2 = "";
		INumericEntryDialog.m_UnitsString = "";
		INumericEntryDialog.vMaxValue = 30000;

		INumericEntryDialog.m_DialogTitleStaticText3 = "Caution Manual Edit";
		INumericEntryDialog.m_DialogTitleStaticText4 = 
			"Original Value: " + INumericEntryDialog.vEditString;

		INumericEntryDialog.vIntegerOnly = true;
		//Pass control to dialog box and display
		int TempResult = INumericEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (TempResult == IDOK)
		{
			unsigned long int TempHours = _wtoi(INumericEntryDialog.vEditString);
			CString TempName("");
			CString TempReason("");
			if (GetName(&TempName))
			if (GetReason(&TempReason))
			{
				ReportErrorMessage(TempName + " Manually Set X-Ray Detector Warranty Run Time Counter to: " +
					INumericEntryDialog.vEditString + " hours because " +
					TempReason + ".  X-Ray Detector Warranty Run Time Counter was: " + 
					FormatRunTimeCounterString(vGlobalLifeTimeCounters->vDetectorsWarrantyDuration), 
					cEMailInspx, 32003);
				vGlobalLifeTimeCounters->vDetectorsWarrantyDuration = TempHours * 3600;
			}
		}
		else
		{
			if (TempResult == 10)
			{
				Sleep(1); //is the equivelent of a yeild statement;
				CDialog::EndDialog(10);
			}
		}
	}
}

void CSystemCountersDialog::OnSubFunction7Button() 
{
	if (PasswordOK(cTemporaryInspxPassword,false))
	if (ResetPasswordOK())
	{
		CYesNoDialog TempYesNoDialog;
		TempYesNoDialog.vNoticeText = _T("\n\n\nReset X-Ray Tube Run Time Counters?");
		TempYesNoDialog.vYesButtonText = "Clear Counters";
		TempYesNoDialog.vQuestionType = cConfirmQuestion;
		int TempResult = TempYesNoDialog.DoModal();
		if (TempResult == IDOK)
		{
			CString TempName("");
			CString TempReason("");
			if (GetName(&TempName))
			if (GetReason(&TempReason))
				ResetTubeRunTimeCounter(TempReason, TempName);
		}
	}
}

void CSystemCountersDialog::OnSubFunction7eButton() 
{
	if (PasswordOK(cSuperInspxPassword,false))
	{
		CNumericEntryDialog INumericEntryDialog;  
		//Set dialog box data titles and number value
		INumericEntryDialog.vEditString = 
			dtoa(vGlobalLifeTimeCounters->vXRaySourceRunDuration / 3600,0); 
		INumericEntryDialog.m_DialogTitleStaticText1 = "X-Ray Tube Run Time Hours";
		INumericEntryDialog.m_DialogTitleStaticText2 = "";
		INumericEntryDialog.m_UnitsString = "";
		INumericEntryDialog.vMaxValue = 30000;

		INumericEntryDialog.m_DialogTitleStaticText3 = "Caution Manual Edit";
		INumericEntryDialog.m_DialogTitleStaticText4 = 
			"Original Value: " + INumericEntryDialog.vEditString;

		INumericEntryDialog.vIntegerOnly = true;
		//Pass control to dialog box and display
		int TempResult = INumericEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (TempResult == IDOK)
		{
			unsigned long int TempHours = _wtoi(INumericEntryDialog.vEditString);
			CString TempName("");
			CString TempReason("");
			if (GetName(&TempName))
			if (GetReason(&TempReason))
			{
				ReportErrorMessage(TempName + " Manually Set X-Ray Tube Run Time Counter to: " +
					INumericEntryDialog.vEditString + " hours because " +
					TempReason + ".  X-Ray Tube Run Time Counter was: " + 
					FormatRunTimeCounterString(vGlobalLifeTimeCounters->vXRaySourceRunDuration), 
					cEMailInspx, 32003);
				vGlobalLifeTimeCounters->vXRaySourceRunDuration = TempHours * 3600;
			}
		}
		else
		{
			if (TempResult == 10)
			{
				Sleep(1); //is the equivelent of a yeild statement;
				CDialog::EndDialog(10);
			}
		}
	}
}

void CSystemCountersDialog::OnSubFunction8Button() 
{
	if (PasswordOK(cTemporaryInspxPassword,false))
	if (ResetPasswordOK())
	{
		CYesNoDialog TempYesNoDialog;
		TempYesNoDialog.vNoticeText = _T("\n\n\nReset Detector Run Time Counters?");
		TempYesNoDialog.vYesButtonText = "Clear Counters";
		TempYesNoDialog.vQuestionType = cConfirmQuestion;
		int TempResult = TempYesNoDialog.DoModal();
		if (TempResult == IDOK)
		{
			CString TempName("");
			CString TempReason("");
			if (GetName(&TempName))
			if (GetReason(&TempReason))
				ResetDetectorRunTimeCounter(TempReason, TempName);
		}
	}
}

void CSystemCountersDialog::OnSubFunction8eButton() 
{
	if (PasswordOK(cSuperInspxPassword,false))
	{
		CNumericEntryDialog INumericEntryDialog;  
		//Set dialog box data titles and number value
		INumericEntryDialog.vEditString = 
			dtoa(vGlobalLifeTimeCounters->vDetectorsRunDuration / 3600,0); 
		INumericEntryDialog.m_DialogTitleStaticText1 = "Detector Run Time Hours";
		INumericEntryDialog.m_DialogTitleStaticText2 = "";
		INumericEntryDialog.m_UnitsString = "";
		INumericEntryDialog.vMaxValue = 30000;

		INumericEntryDialog.m_DialogTitleStaticText3 = "Caution Manual Edit";
		INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + INumericEntryDialog.vEditString;

		INumericEntryDialog.vIntegerOnly = true;
		//Pass control to dialog box and display
		int TempResult = INumericEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (TempResult == IDOK)
		{
			unsigned long int TempHours = _wtoi(INumericEntryDialog.vEditString);
			CString TempName("");
			CString TempReason("");
			if (GetName(&TempName))
			if (GetReason(&TempReason))
			{
				ReportErrorMessage(TempName + " Manually Set Detector Run Time Counter to: " +
					INumericEntryDialog.vEditString + " hours because " +
					TempReason + ".  Detector Run Time Counter was: " + 
					FormatRunTimeCounterString(vGlobalLifeTimeCounters->vDetectorsRunDuration), 
					cEMailInspx, 32003);
				vGlobalLifeTimeCounters->vDetectorsRunDuration = TempHours * 3600;
			}
		}
		else
		{
			if (TempResult == 10)
			{
				Sleep(1); //is the equivelent of a yeild statement;
				CDialog::EndDialog(10);
			}
		}
	}
}

BOOL CSystemCountersDialog::PreTranslateMessage(MSG* pMsg) 
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
					cShowHelp, 42, 0);
				{
					TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText1);
					if (TempWindow)
						TempWindow->Invalidate(false);
				}
			break;
			case 27: // Escape
				::PostThreadMessage(vGlobalCallingThreadID,	cStartChatWindowMessage, 0,0);
			break;	
			case cPageUp: // Clear
				if (vGlobalPasswordLevel)
				{
					PasswordOK(cNoPassword, false);
				}
				else
				{
					PasswordOK(cAnyPasswordToEnter, true);
				}
				UpdateButtons();
				TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText1);
				if (TempWindow)
					TempWindow->Invalidate(false);
			break;	
		}
		return true;  //true indicates it has been handled, so do not process
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

HBRUSH CSystemCountersDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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

	if (pWnd->GetDlgCtrlID() == IDC_Function1Button)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function1Button, 5);  //5 is medium large
		if (vMakeExitButtonGreen)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cButtonTextColor);
			return vLocalSystemData->vGreenBrush;
		}
	}

	if (pWnd->GetDlgCtrlID() == IDC_SupportContract2)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SupportContract2, 5);
		pDC->SetBkMode(TRANSPARENT);

		if (vLocalConfigurationData->vSupportContractType == cHideSupportContractDisplay)
			pDC->SetTextColor(cDarkYellow);
		else
		if (vLocalConfigurationData->vSupportContractType == cSupportContractExpired)
			pDC->SetTextColor(cRed);
		else
		if (vSupportContractStatus == cExpired)
			pDC->SetTextColor(cRed);
		else
		if (vSupportContractStatus == cContractLastMonth)
			pDC->SetTextColor(cDarkYellow);
		else
			pDC->SetTextColor(cGreen);

		return vLocalSystemData->vLightBrownBrush;
  }
	
	if (pWnd->GetDlgCtrlID() == IDC_SupportContract)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SupportContract2, 5);
		pDC->SetBkMode(TRANSPARENT);
		//cSupportContractInvalid, cSupportContract8_5, cSupportContract16_5, cSupportContract24_7, cHideSupportContractDisplay, cSupportContractExpired
		if (vLocalConfigurationData->vSupportContractType == cHideSupportContractDisplay)
			pDC->SetTextColor(cDarkYellow);
		else
		if (vLocalConfigurationData->vSupportContractType == cSupportContractExpired)
			pDC->SetTextColor(cRed);
		else
		if (vLocalConfigurationData->vSupportContractType == cSupportContract8_5)
			pDC->SetTextColor(cDarkBlue);
		else
		if (vLocalConfigurationData->vSupportContractType == cSupportContract24_7)
			pDC->SetTextColor(cGreen);
		else
			pDC->SetTextColor(cBlack);

		return vLocalSystemData->vLightBrownBrush;
  }
	
	if (pWnd->GetDlgCtrlID() == IDC_Background)
	{
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_ScanTracCommissionedDateLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_ScanTracCommissionedDateLabel, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_MonthsSinceCommissionedLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_MonthsSinceCommissionedLabel, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_ScanTracID)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_ScanTracID, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_TodaysDate)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_TodaysDate, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_TubeWarrantyDateLabel) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_TubeWarrantyDateLabel, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DetectorWarrantyDateLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DetectorWarrantyDateLabel, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
		
	if (pWnd->GetDlgCtrlID() == IDC_XRayTubeSerialNumberLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_XRayTubeSerialNumberLabel, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_ScanTracRunDurationLabel) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_ScanTracRunDurationLabel, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DetectorRunDurationLabel) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DetectorRunDurationLabel, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DetectorRunDurationLabel2)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DetectorRunDurationLabel2, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DetectorRunDurationLabel3)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DetectorRunDurationLabel3, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_XRaySourceDurationLabel2)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_XRaySourceDurationLabel2, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_XRaySourceDurationLabel3)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_XRaySourceDurationLabel3, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_XRaySourceDurationLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_XRaySourceDurationLabel, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
  }

	if (pWnd->GetDlgCtrlID() == IDC_XRayTubeSerialNumber)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_XRayTubeSerialNumber, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_ScanTracCommissionedDate) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_ScanTracCommissionedDate, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_MonthsSinceCommissioned)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_MonthsSinceCommissioned, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_ScanTracRunDuration) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_ScanTracRunDuration, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_TubeWarrantyDate) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_TubeWarrantyDate, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SupportContract) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SupportContract, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SupportContract2)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SupportContract2, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}

	if (pWnd->GetDlgCtrlID() == IDC_DetectorWarrantyDate)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DetectorWarrantyDate, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_XRaySourceDuration)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_XRaySourceDuration, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_XRaySourceDuration2)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_XRaySourceDuration2, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_XRaySourceDuration3)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_XRaySourceDuration3, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DetectorRunDuration2) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DetectorRunDuration2, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DetectorRunDuration3) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DetectorRunDuration3, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DetectorRunDuration)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DetectorRunDuration, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
  }
	if (pWnd->GetDlgCtrlID() == IDC_XRaysOn)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_XRaysOn, 7);
		pDC->SetBkMode(TRANSPARENT);
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
	 
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction1eButton)
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction1eButton2, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}	
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction5eButton)
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction5eButton, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction6eButton)
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction6eButton, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction7eButton)
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction7eButton, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction8eButton)
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction8eButton, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	
	return hbr;
}

void CSystemCountersDialog::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
  if (nIDEvent == vUpdateDisplayTimerHandle)
  {
		UpdateDisplay();
	}
	
	CDialog::OnTimer(nIDEvent);
}

void CSystemCountersDialog::ShowDisplay()
{
	vSupportContractStatus = vMainWindowPointer->WhatIsSupportContractStatus();
	if (vLocalConfigurationData->vSupportContractType == cHideSupportContractDisplay)
	{
		SetDlgItemText(IDC_SupportContract, _T("Support Contract Display:"));
		SetDlgItemText(IDC_SupportContract2, _T("Hidden"));
	}
	else
	{
		SetDlgItemText(IDC_SupportContract, vLocalSystemData->vSupportContractTypeStrings[vLocalConfigurationData->vSupportContractType]);
		SetDlgItemText(IDC_SupportContract2, vMainWindowPointer->SupportContracExpirationString());
	}

	SetDlgItemText(IDC_XRayTubeSerialNumber,vGlobalLifeTimeCounters->vXRayTubeSerialNumber);

	if (vGlobalLifeTimeCounters->vCommisionDate.GetYear() > 1999)
		vLocalSystemData->vNotCommissioned = false;
	else
		vLocalSystemData->vNotCommissioned = true;

	CTime TempTime = CTime::GetCurrentTime();

	CString TempTodaysDateString = TempTime.Format("%B %#d, %Y");
	SetDlgItemText(IDC_TodaysDate,"Today is: " + TempTodaysDateString);

	if (vLocalSystemData->vNotCommissioned)
	{
		SetDlgItemText(IDC_ScanTracCommissionedDate, _T("Not Commissioned"));
		SetDlgItemText(IDC_TubeWarrantyDate, _T("Not Commissioned"));
		SetDlgItemText(IDC_DetectorWarrantyDate, _T("Not Commissioned"));
		SetDlgItemText(IDC_MonthsSinceCommissioned, _T("Not Commissioned"));
		SetDlgItemText(IDC_XRaySourceDuration3, _T("Not Commissioned"));
		SetDlgItemText(IDC_DetectorRunDuration3, _T("Not Commissioned"));
	}
	else
	{
		CString TempDateString = vGlobalLifeTimeCounters->vCommisionDate.Format("%B %#d, %Y");
		SetDlgItemText(IDC_ScanTracCommissionedDate,TempDateString);

		TempDateString = vGlobalLifeTimeCounters->vXRaySourceWarrantyDate.Format("%B %#d, %Y");
		SetDlgItemText(IDC_TubeWarrantyDate,TempDateString);

		TempDateString = vGlobalLifeTimeCounters->vDetectorsWarrantyDate.Format("%B %#d, %Y");
		SetDlgItemText(IDC_DetectorWarrantyDate,TempDateString);

		int TempMonths = (TempTime.GetMonth() - vGlobalLifeTimeCounters->vCommisionDate.GetMonth()) +
			((TempTime.GetYear() - vGlobalLifeTimeCounters->vCommisionDate.GetYear()) * 12);

		if (TempTime.GetDay() < vGlobalLifeTimeCounters->vCommisionDate.GetDay())
			TempMonths = TempMonths - 1;

		if (TempMonths < 0)
			SetDlgItemText(IDC_MonthsSinceCommissioned, _T("Not Available"));
		else
			SetDlgItemText(IDC_MonthsSinceCommissioned,dtoa(TempMonths));

		TempMonths = 
			(TempTime.GetMonth() - vGlobalLifeTimeCounters->vXRaySourceWarrantyDate.GetMonth()) +
			((TempTime.GetYear() - vGlobalLifeTimeCounters->vXRaySourceWarrantyDate.GetYear()) * 12);

		if (TempTime.GetDay() < vGlobalLifeTimeCounters->vXRaySourceWarrantyDate.GetDay())
			TempMonths = TempMonths - 1;

		if (TempMonths < 0)
			SetDlgItemText(IDC_XRaySourceDuration3, _T("Not Available"));
		else
			SetDlgItemText(IDC_XRaySourceDuration3,dtoa(TempMonths));

		TempMonths = 
			(TempTime.GetMonth() - vGlobalLifeTimeCounters->vDetectorsWarrantyDate.GetMonth()) +
			((TempTime.GetYear() - vGlobalLifeTimeCounters->vDetectorsWarrantyDate.GetYear()) * 12);

		if (TempTime.GetDay() < vGlobalLifeTimeCounters->vDetectorsWarrantyDate.GetDay())
			TempMonths = TempMonths - 1;

		if (TempMonths < 0)
			SetDlgItemText(IDC_DetectorRunDuration3, _T("Not Available"));
		else
			SetDlgItemText(IDC_DetectorRunDuration3,dtoa(TempMonths));
	}

}
/*
	vArchiveVersionNumber = 0;
	vXRaySourceRunDuration = 0; //keeps track of how many seconds x-ray source
	vDetectorsRunDuration = 0; //keeps track of how many seconds Detectors
	vScanTracRunDuration = 0; //keeps track of how many seconds ScanTrac

	vUsedDisposablePasswords = "";
	vServiceRecordLog = "";
	vCommisionDate = 0;
	vXRaySourceWarrantyDuration = 0;
	vDetectorsWarrantyDuration = 0;
	vXRaySourceWarrantyDate = 0;
	vDetectorsWarrantyDate = 0;
	CString TempTimeString;
	if (vLocalConfigurationData->vShow24HourTime)
		TempTimeString = TempTime.Format("%H:%M:%S");
	else
		TempTimeString = TempTime.Format("%#I:%M:%S%p");
	SetDlgItemText(IDC_Clock,TempTimeString);
	
	CString TempDateString = TempTime.Format("%B %#d, %Y");
	if (vLastDateString != TempDateString)
	{
		SetDlgItemText(IDC_Date,TempDateString);
		vLastDateString = TempDateString;
	}
*/
void CSystemCountersDialog::UpdateDisplay()
{
	if (vOldXRaySourceWarrantyDuration != vGlobalLifeTimeCounters->vXRaySourceWarrantyDuration)
	{
		vOldXRaySourceWarrantyDuration = vGlobalLifeTimeCounters->vXRaySourceWarrantyDuration;
		DWORD TempInt = (vOldXRaySourceWarrantyDuration / 3600);
		SetDlgItemText(IDC_XRaySourceDuration2,dtoa(TempInt,0));
	}

	if (vOldDetectorsWarrantyDuration != vGlobalLifeTimeCounters->vDetectorsWarrantyDuration)
	{
		vOldDetectorsWarrantyDuration = vGlobalLifeTimeCounters->vDetectorsWarrantyDuration;
		DWORD TempInt = (vOldDetectorsWarrantyDuration / 3600);
		SetDlgItemText(IDC_DetectorRunDuration2,dtoa(TempInt,0));
	}
	if (vOldXRaySourceRunDuration != vGlobalLifeTimeCounters->vXRaySourceRunDuration)
	{
		vOldXRaySourceRunDuration = vGlobalLifeTimeCounters->vXRaySourceRunDuration;
		SetDlgItemText(IDC_XRaySourceDuration, FormatRunTimeCounterString(vOldXRaySourceRunDuration));
	}

	if (vOldScanTracRunDuration != vGlobalLifeTimeCounters->vScanTracRunDuration)
	{
		vOldScanTracRunDuration = vGlobalLifeTimeCounters->vScanTracRunDuration;
		SetDlgItemText(IDC_ScanTracRunDuration, FormatRunTimeCounterString(vOldScanTracRunDuration));
	}

	if (vOldDetectorsRunDuration != vGlobalLifeTimeCounters->vDetectorsRunDuration)
	{
		vOldDetectorsRunDuration = vGlobalLifeTimeCounters->vDetectorsRunDuration;
		SetDlgItemText(IDC_DetectorRunDuration, FormatRunTimeCounterString(vOldDetectorsRunDuration));
	}

	if (vOldXRayOnNotice != vLocalSystemData->vXRayOnNotice)
	{
		vOldXRayOnNotice = vLocalSystemData->vXRayOnNotice;
		if (vOldXRayOnNotice)
			m_XRaysOn.ShowWindow(SW_SHOW);
		else
			m_XRaysOn.ShowWindow(SW_HIDE);
	}
}

void CSystemCountersDialog::SetChangeMade() 
{
	if (!vChangeMade)
	{
		m_Function2Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		SetDlgItemText(IDC_Function2Button, _T("Save\nand Exit"));
		SetDlgItemText(IDC_Function4Button, _T("Cancel"));
		vChangeMade = true;
	}
}

void CSystemCountersDialog::UpdateButtons()
{
	if (PasswordOK(cTemporaryInspxPassword,false))
	{
		m_Function3Button.ShowWindow(SW_SHOW);
		m_Function5Button.ShowWindow(SW_SHOW);
		m_SubFunction1Button.ShowWindow(SW_SHOW);
		m_SubFunction2Button.ShowWindow(SW_SHOW);
		m_SubFunction3Button.ShowWindow(SW_SHOW);
		m_SubFunction4Button.ShowWindow(SW_SHOW);
		m_SubFunction5Button.ShowWindow(SW_SHOW);
		m_SubFunction6Button.ShowWindow(SW_SHOW);
		m_SubFunction7Button.ShowWindow(SW_SHOW);
		m_SubFunction8Button.ShowWindow(SW_SHOW);
	}
	else
	{
		m_Function3Button.ShowWindow(SW_HIDE);
		m_Function5Button.ShowWindow(SW_HIDE);
		m_SubFunction1Button.ShowWindow(SW_HIDE);
		m_SubFunction2Button.ShowWindow(SW_HIDE);
		m_SubFunction3Button.ShowWindow(SW_HIDE);
		m_SubFunction4Button.ShowWindow(SW_HIDE);
		m_SubFunction5Button.ShowWindow(SW_HIDE);
		m_SubFunction6Button.ShowWindow(SW_HIDE);
		m_SubFunction7Button.ShowWindow(SW_HIDE);
		m_SubFunction8Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_Function3Button.GetRectToInvalidateArrow());
		this->InvalidateRect(&m_Function5Button.GetRectToInvalidateArrow());
		this->InvalidateRect(&m_SubFunction1Button.GetRectToInvalidateArrow());
		this->InvalidateRect(&m_SubFunction2Button.GetRectToInvalidateArrow());
		this->InvalidateRect(&m_SubFunction3Button.GetRectToInvalidateArrow());
		this->InvalidateRect(&m_SubFunction4Button.GetRectToInvalidateArrow());
		this->InvalidateRect(&m_SubFunction5Button.GetRectToInvalidateArrow());
		this->InvalidateRect(&m_SubFunction6Button.GetRectToInvalidateArrow());
		this->InvalidateRect(&m_SubFunction7Button.GetRectToInvalidateArrow());
		this->InvalidateRect(&m_SubFunction8Button.GetRectToInvalidateArrow());
	}
	if (PasswordOK(cSuperInspxPassword,false))
	{
		m_SubFunction1eButton2.ShowWindow(SW_SHOW);
		m_SubFunction5eButton.ShowWindow(SW_SHOW);
		m_SubFunction6eButton.ShowWindow(SW_SHOW);
		m_SubFunction7eButton.ShowWindow(SW_SHOW);
		m_SubFunction8eButton.ShowWindow(SW_SHOW);
	}
	else
	{
		m_SubFunction1eButton2.ShowWindow(SW_HIDE);
		m_SubFunction5eButton.ShowWindow(SW_HIDE);
		m_SubFunction6eButton.ShowWindow(SW_HIDE);
		m_SubFunction7eButton.ShowWindow(SW_HIDE);
		m_SubFunction8eButton.ShowWindow(SW_HIDE);
	}
}

bool CSystemCountersDialog::ResetPasswordOK()
{
	CPasswordEntryDialog IPasswordEntryDialog;  
	bool TempPasswordOK = false;

	//Set dialog box data titles and number value
	IPasswordEntryDialog.vEditString = "";
	IPasswordEntryDialog.m_DialogTitleStaticText1 = "Enter Inspx Reset Password";
	IPasswordEntryDialog.m_DialogTitleStaticText2 = "";
	//Pass control to dialog box and display
	int nResponse = IPasswordEntryDialog.DoModal();
	//dialog box is now closed, if user pressed select do this
	//if user pressed cancel, do nothing
	if (nResponse == IDOK)
	{
		if (("31415926" == IPasswordEntryDialog.vEditString) ||
			("5104682199" == IPasswordEntryDialog.vEditString) ||
			("72054328" == IPasswordEntryDialog.vEditString) ||
			("7205" == IPasswordEntryDialog.vEditString) ||
			(vLocalConfigurationData->vSystemSetupResetPassword == IPasswordEntryDialog.vEditString))
		{
			TempPasswordOK = true;
		}
		else
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\n\nPassword not correct";
			TempNoticeDialog.vType = cErrorMessage;
			TempNoticeDialog.DoModal();
		}
	}
	return TempPasswordOK;
}

void CSystemCountersDialog::ResetTubeWarranty(CTime TempDate, CString TempReason, 
																							CString TempPerson)
{
	ReportErrorMessage(TempPerson + " Reset X-Ray Tube Warranty Counter because " +
		TempReason + ".  X-Ray Tube Warranty Counter was: " + 
		FormatRunTimeCounterString(vGlobalLifeTimeCounters->vXRaySourceWarrantyDuration) + 
		".  Previous X-Ray Source Warranty Reset Date was: " + 
		vGlobalLifeTimeCounters->vXRaySourceWarrantyDate.Format("%B %#d, %Y"),
		cEMailInspx, 32003);

	vGlobalLifeTimeCounters->vXRaySourceWarrantyDate = TempDate;
	vGlobalLifeTimeCounters->vXRaySourceWarrantyDuration = 0;

	ResetTubeRunTimeCounter(TempReason, TempPerson);
	ShowDisplay();
}

void CSystemCountersDialog::ResetTubeRunTimeCounter(CString TempReason, CString TempPerson)
{
	ReportErrorMessage(TempPerson + " Reset X-Ray Tube Run Time Counter because " +
		TempReason + ".  X-Ray Tube Run Time Counter was: " + 
		FormatRunTimeCounterString(vGlobalLifeTimeCounters->vXRaySourceRunDuration), 
		cEMailInspx, 32003);

	vGlobalLifeTimeCounters->vXRaySourceRunDuration = 0;
}

void CSystemCountersDialog::ResetDetectorWarranty(CTime TempDate, CString TempReason, 
																							CString TempPerson)
{
	ReportErrorMessage(TempPerson + " Reset Detector Warranty Counter because " +
		TempReason + ".  Detector Warranty Counter was: " + 
		FormatRunTimeCounterString(vGlobalLifeTimeCounters->vDetectorsWarrantyDuration) + 
		".  Previous Detector Warranty Reset Date was: " + 
		vGlobalLifeTimeCounters->vDetectorsWarrantyDate.Format("%B %#d, %Y"),
		cEMailInspx, 32003);

	vGlobalLifeTimeCounters->vDetectorsWarrantyDate = TempDate;
	vGlobalLifeTimeCounters->vDetectorsWarrantyDuration = 0;

	ResetDetectorRunTimeCounter(TempReason, TempPerson);
	ShowDisplay();
}

void CSystemCountersDialog::ResetDetectorRunTimeCounter(CString TempReason, CString TempPerson)
{
	ReportErrorMessage(TempPerson + " Reset Detector Run Time Counter because " +
		TempReason + ".  Detector Run Time Counter was: " + 
		FormatRunTimeCounterString(vGlobalLifeTimeCounters->vDetectorsRunDuration), 
		cEMailInspx, 32003);

	vGlobalLifeTimeCounters->vDetectorsRunDuration = 0;
}

bool CSystemCountersDialog::GetName(CString *TempName)
{
	//enter name, and reason for clearing
	CAlphaEntryDialog IAlphaEntryDialog;  
	IAlphaEntryDialog.vMainWindowPointer = vMainWindowPointer;
	//Set dialog box data titles and number value
	IAlphaEntryDialog.vEditString = "";
	IAlphaEntryDialog.m_DialogTitleStaticText = "Enter your name to authorize this function";
	IAlphaEntryDialog.vAllowMoreThan32Characters = true;
	IAlphaEntryDialog.vMinimumNumberOfCharacters = 10;
	IAlphaEntryDialog.vAllowPeriod = true;

	IAlphaEntryDialog.m_DialogTitleStaticText2 = "";
	IAlphaEntryDialog.vLocalSystemData = vLocalSystemData;
	//Pass control to dialog box and display
	int nResponse = IAlphaEntryDialog.DoModal();
	//dialog box is now closed, if user pressed select do this
	//if user pressed cancel, do nothing
	if (nResponse == IDOK)
	{
		*TempName = IAlphaEntryDialog.vEditString;
		return true;
	}
	else
	{
		return false;
	}
}

bool CSystemCountersDialog::GetReason(CString *TempReason)
{
	//enter name, and reason for clearing
	CAlphaEntryDialog IAlphaEntryDialog;  
	IAlphaEntryDialog.vMainWindowPointer = vMainWindowPointer;
	//Set dialog box data titles and number value
	IAlphaEntryDialog.vEditString = "";
	IAlphaEntryDialog.m_DialogTitleStaticText = "Enter reason for executing this function";
	IAlphaEntryDialog.vAllowMoreThan32Characters = true;
	IAlphaEntryDialog.vMinimumNumberOfCharacters = 10;
	IAlphaEntryDialog.vAllowPeriod = true;

	IAlphaEntryDialog.m_DialogTitleStaticText2 = "";
	IAlphaEntryDialog.vLocalSystemData = vLocalSystemData;
	//Pass control to dialog box and display
	int nResponse = IAlphaEntryDialog.DoModal();
	//dialog box is now closed, if user pressed select do this
	//if user pressed cancel, do nothing
	if (nResponse == IDOK)
	{
		*TempReason = IAlphaEntryDialog.vEditString;
		return true;
	}
	else
	{
		if (nResponse == 10)
		{
			//Main Menu button pressed
			Sleep(1); //is the equivelent of a yeild statement;
			CDialog::EndDialog(10);
		}
		return false;
	}
}



void CSystemCountersDialog::OnSubFunction1eButton() 
{
	if (PasswordOK(cSuperInspxPassword,false))
	{
		//edit machine runtime hours
		CNumericEntryDialog INumericEntryDialog;  
		//Set dialog box data titles and number value
		INumericEntryDialog.vEditString = 
			dtoa(vGlobalLifeTimeCounters->vScanTracRunDuration / 3600,0); 
		INumericEntryDialog.m_DialogTitleStaticText1 = "ScanTrac Run Time Hours";
		INumericEntryDialog.m_DialogTitleStaticText2 = "";
		INumericEntryDialog.m_UnitsString = "";
		INumericEntryDialog.vMaxValue = 30000;

		INumericEntryDialog.m_DialogTitleStaticText3 = "Caution Manual Edit";
		INumericEntryDialog.m_DialogTitleStaticText4 = 
			"Original Value: " + INumericEntryDialog.vEditString;

		INumericEntryDialog.vIntegerOnly = true;
		//Pass control to dialog box and display
		int TempResult = INumericEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (TempResult == IDOK)
		{
			unsigned long int TempHours = _wtoi(INumericEntryDialog.vEditString);
			CString TempName("");
			CString TempReason("");
			if (GetName(&TempName))
			if (GetReason(&TempReason))
			{
				ReportErrorMessage(TempName + " Manually Set ScanTrac Run Time Counter to: " +
					INumericEntryDialog.vEditString + " hours because " +
					TempReason + ".  ScanTrac Run Time Counter was: " + 
					FormatRunTimeCounterString(vGlobalLifeTimeCounters->vScanTracRunDuration), 
					cEMailInspx, 32003);
				vGlobalLifeTimeCounters->vScanTracRunDuration = TempHours * 3600;
			}
		}
		else
		{
			if (TempResult == 10)
			{
				Sleep(1); //is the equivelent of a yeild statement;
				CDialog::EndDialog(10);
			}
		}
	}
}



void CSystemCountersDialog::OnDialogtitlestatictext1()
{
	BYTE TempHiddenKeys = 8;
	if (!PasswordOK(cTemporaryInspxPassword,false))
		TempHiddenKeys = 10;
	PostThreadMessage(vGlobalCallingThreadID, cDisplaySoftKeyMenu, TempHiddenKeys, TempHiddenKeys);
}
