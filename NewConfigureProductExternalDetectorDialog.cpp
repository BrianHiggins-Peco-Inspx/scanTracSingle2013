//ScanTrac Side View Source File
// NewConfigureProductExternalDetectorDialog.cpp : implementation file
//

#include "stdafx.h"
#include "ScanTrac.h"
#include "NewConfigureProductExternalDetectorDialog.h"
#include "NumericEntryDialog.h"
#include "NoticeDialog.h"
#include "SerialPort.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern BYTE vGlobalShiftKeyDown;
extern CSerialPort *vGlobaluCSerialPort;
extern unsigned long vGlobalDialogTitleColor;
/////////////////////////////////////////////////////////////////////////////
// CNewConfigureProductExternalDetectorDialog dialog


CNewConfigureProductExternalDetectorDialog::CNewConfigureProductExternalDetectorDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CNewConfigureProductExternalDetectorDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewConfigureProductExternalDetectorDialog)
	//}}AFX_DATA_INIT
	vEnableMode2 = true;
	vEnableMode1 = true;
	vMainWindowPointer = NULL;
	vChangeMade = false;
	vDetectorNumberEditing = 0;
	vFirstShown = 0;
	vInAutoSetup = false;
	vStartTimerHandle = 98;
	vUpdateDisplayTimerHandle = 16;
	vOldXRayOnNotice = false;
	//vInSideTrip = false;

	vStopSystemOnExit = false;
	
	vOldWindowStartDetector1 = 0;
	vOldWindowStartDetector2 = 0;
	vOldWindowEndDetector1 = 0;
	vOldWindowEndDetector2 = 0;
	vOldDetectorMode1 = 0;
	vOldDetectorMode2 = 0;
	vOldDetectorEjector1 = 0;
	vOldDetectorEjector2 = 0;

	vDiscoverModeTextBoxString = "";
	vDiscoverModeTextPosition = 0;

	vLocalCWndCollection.Add(this);
	vLocalCWndCollection.Add(&m_DiscoverModeTextBox);
	vLocalCWndCollection.Add(&m_Function3Button);
	vLocalCWndCollection.Add(&m_Calibrating);
	vLocalCWndCollection.Add(&m_XRaysOn);
	vLocalCWndCollection.Add(&m_Function5Button);
	vLocalCWndCollection.Add(&m_WindowStart2);
	vLocalCWndCollection.Add(&m_WindowStart1);
	vLocalCWndCollection.Add(&m_WindowEnd2);
	vLocalCWndCollection.Add(&m_WindowEnd1);
	vLocalCWndCollection.Add(&m_SubFunction8Button);
	vLocalCWndCollection.Add(&m_SubFunction7Button);
	vLocalCWndCollection.Add(&m_SubFunction6Button);
	vLocalCWndCollection.Add(&m_SubFunction5Button);
	vLocalCWndCollection.Add(&m_SubFunction4Button);
	vLocalCWndCollection.Add(&m_SubFunction3Button);
	vLocalCWndCollection.Add(&m_SubFunction2Button);
	vLocalCWndCollection.Add(&m_SubFunction1Button);
	vLocalCWndCollection.Add(&m_DetectorName2);
	vLocalCWndCollection.Add(&m_DetectorName1);
	vLocalCWndCollection.Add(&m_DetectorMode2);
	vLocalCWndCollection.Add(&m_DetectorMode1);
	vLocalCWndCollection.Add(&m_Function4Button);
	vLocalCWndCollection.Add(&m_Function2Button);
	vLocalCWndCollection.Add(&m_Background);
	vLocalCWndCollection.Add(&m_DialogTitleStaticText);
	vLocalCWndCollection.Add(&m_Function1Button);
	vLocalCWndCollection.Add(&m_Title1);
	vLocalCWndCollection.Add(&m_Title2);
	vLocalCWndCollection.Add(&m_Title3);
	vLocalCWndCollection.Add(&m_Title4);
	vLocalCWndCollection.Add(&m_Title5);
	vLocalCWndCollection.Add(&m_Title6);
	vLocalCWndCollection.Add(&m_Title7);
	vLocalCWndCollection.Add(&m_Title8);
}

void CNewConfigureProductExternalDetectorDialog::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	m_Background.SetWindowPos(NULL,cMainBackgroundLeft,
		cMainBackgroundTop,cMainBackgroundLength,
		cMainBackgroundHeight,SWP_NOZORDER);
	vSubFunction1ButtonEnable = true;
	vSubFunction2ButtonEnable = true;
	vSubFunction5ButtonEnable = true;
	vSubFunction6ButtonEnable = true;
	vSubFunction7ButtonEnable = true;
	vCurrentDetectorName = vLocalConfigurationData->vExternalDetector[0].vName;
	CString TempText("");
	TempText.LoadString(IDS_XRAYON);
	SetDlgItemText(IDC_XRaysOn,TempText);
	vNumberEjectorsConfigured = 0;
	vOnlyEjectorNumberConfigured = 0;
	for (BYTE TempLoop = 0; TempLoop < cNumberOfEjectors; TempLoop++)
	{
		if ((vLocalConfigurationData->vEjector[TempLoop].vEnabled) &&
			(vLocalConfigurationData->vEjector[TempLoop].vLineBitMask))
		{
			vNumberEjectorsConfigured++;
			vOnlyEjectorNumberConfigured = TempLoop;
		}
	}

	for (BYTE TempLoop = 0; TempLoop < cNumberOfExternalDetectors; TempLoop++)
	{
		vWindowStart[TempLoop] = vLocalCurrentProductPointer->vExternalDetectorWindowStart[TempLoop];
		vWindowEnd[TempLoop] = vLocalCurrentProductPointer->vExternalDetectorWindowEnd[TempLoop];
		vEnabled[TempLoop] = vLocalCurrentProductPointer->vExternalDetectorEnable[TempLoop];
		vDetectorMode[TempLoop] = vLocalCurrentProductPointer->vExternalDetectorMode[TempLoop];
		vOriginalDetectorMode[TempLoop] = vLocalCurrentProductPointer->vExternalDetectorMode[TempLoop];
		vDetectorEjector[TempLoop] = vLocalCurrentProductPointer->vExternalDetectorEjector[TempLoop];
	}

	if (vGlobaluCSerialPort)
	{
		vGlobaluCSerialPort->vNumberOfExternalDetectors = 0;
		for (BYTE TempLoop = 0; TempLoop < cNumberOfExternalDetectors; TempLoop++)
		{
			if (vDetectorEjector[TempLoop] == 0)
			{
				if (vLocalConfigurationData->vEjector[0].vEnabled)
					vDetectorEjector[TempLoop] = 1;
			}
			if ((vLocalConfigurationData->vExternalDetector[TempLoop].vEnabled) && (vLocalConfigurationData->vExternalDetector[TempLoop].vLineBitMask))
			{
				vGlobaluCSerialPort->vNumberOfExternalDetectors++;
			}
		}
	}

	if (vGlobaluCSerialPort->vNumberOfExternalDetectors < 3)
	{
		m_Function3Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_Function3Button.GetRectToInvalidateArrow());
	}
	else
	{
		m_Function3Button.ShowWindow(SW_SHOW);
	}

	if (vInAutoSetup)
	{
		SetDlgItemText(IDC_DialogTitleStaticText1,  _T("Auto Setup Product Auxiliary Detectors"));
		//m_Function2Button.ShowWindow(SW_HIDE);
		//m_ArrowDown2.ShowWindow(SW_HIDE);
		/*
		m_Function2Button.ShowWindow(SW_SHOW);
		m_ArrowDown2.ShowWindow(SW_SHOW);
		m_Function2Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		SetDlgItemText(IDC_Function2Button,"Auto Setup Detector");
		*/
		//m_Function4Button.ShowWindow(SW_SHOW);
		//m_ArrowDown4.ShowWindow(SW_SHOW);
		//m_Function5Button.ShowWindow(SW_SHOW);
		//m_ArrowDown5.ShowWindow(SW_SHOW);
		//m_Function4Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		//TempText.LoadString(IDS_PreviousAutoSetup);
		//SetDlgItemText(IDC_Function4Button,TempText);
		//m_Function5Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		//TempText.LoadString(IDS_FinishAutoSetup);
		//SetDlgItemText(IDC_Function5Button,TempText);
	}
	UpdateDisplay();

	vProductName = vLocalCurrentProductPointer->GetProductName();
	SetDlgItemText(IDC_Title5,"Product: " + *vProductName);
	this->SetWindowText( _T("Aux. Detectors"));

	int TempTimerResult = SetTimer(vUpdateDisplayTimerHandle,1000,NULL);
	if (!TempTimerResult)
		ReportErrorMessage("Error-Update Display Timer Failed",cEMailInspx,32000);

	m_DiscoverModeTextBox.ShowWindow(SW_HIDE);

	vOldWindowStartDetector1 = vWindowStart[0];
	vOldWindowStartDetector2 = vWindowStart[1];
	vOldWindowEndDetector1 = vWindowEnd[0];
	vOldWindowEndDetector2 = vWindowEnd[1];
	vOldDetectorMode1 = vDetectorMode[0];
	vOldDetectorMode2 = vDetectorMode[1];
	vOldDetectorEjector1 = vDetectorEjector[0];
	vOldDetectorEjector2 = vDetectorEjector[1];

	::SetupMenu(vLocalCWndCollection);

	m_SubFunction3Button.ShowNavigationArrow(cUpArrowBig);
	m_SubFunction4Button.ShowNavigationArrow(cDownArrowBig);
}


void CNewConfigureProductExternalDetectorDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewConfigureProductExternalDetectorDialog)
	DDX_Control(pDX, IDC_DiscoverModeTextBox, m_DiscoverModeTextBox);
	DDX_Control(pDX, IDC_Function3Button, m_Function3Button);
	DDX_Control(pDX, IDC_Calibrating, m_Calibrating);
	DDX_Control(pDX, IDC_XRaysOn, m_XRaysOn);
	DDX_Control(pDX, IDC_Function5Button, m_Function5Button);
	DDX_Control(pDX, IDC_WindowStart2, m_WindowStart2);
	DDX_Control(pDX, IDC_WindowStart1, m_WindowStart1);
	DDX_Control(pDX, IDC_WindowEnd2, m_WindowEnd2);
	DDX_Control(pDX, IDC_WindowEnd1, m_WindowEnd1);
	DDX_Control(pDX, IDC_SubFunction8Button, m_SubFunction8Button);
	DDX_Control(pDX, IDC_SubFunction7Button, m_SubFunction7Button);
	DDX_Control(pDX, IDC_SubFunction6Button, m_SubFunction6Button);
	DDX_Control(pDX, IDC_SubFunction5Button, m_SubFunction5Button);
	DDX_Control(pDX, IDC_SubFunction4Button, m_SubFunction4Button);
	DDX_Control(pDX, IDC_SubFunction3Button, m_SubFunction3Button);
	DDX_Control(pDX, IDC_SubFunction2Button, m_SubFunction2Button);
	DDX_Control(pDX, IDC_SubFunction1Button, m_SubFunction1Button);
	DDX_Control(pDX, IDC_DetectorName2, m_DetectorName2);
	DDX_Control(pDX, IDC_DetectorName1, m_DetectorName1);
	DDX_Control(pDX, IDC_DetectorMode2, m_DetectorMode2);
	DDX_Control(pDX, IDC_DetectorMode1, m_DetectorMode1);
	DDX_Control(pDX, IDC_Function4Button, m_Function4Button);
	DDX_Control(pDX, IDC_Function2Button, m_Function2Button);
	DDX_Control(pDX, IDC_Background, m_Background);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_DialogTitleStaticText1, m_DialogTitleStaticText);
	DDX_Control(pDX, IDC_Function1Button, m_Function1Button);
	DDX_Control(pDX, IDC_Title1, m_Title1);
	DDX_Control(pDX, IDC_Title2, m_Title2);
	DDX_Control(pDX, IDC_Title3, m_Title3);
	DDX_Control(pDX, IDC_Title4, m_Title4);
	DDX_Control(pDX, IDC_Title5, m_Title5);
	DDX_Control(pDX, IDC_Title6, m_Title6);
	DDX_Control(pDX, IDC_Title7, m_Title7);
	DDX_Control(pDX, IDC_Title8, m_Title8);
}


BEGIN_MESSAGE_MAP(CNewConfigureProductExternalDetectorDialog, CDialog)
	//{{AFX_MSG_MAP(CNewConfigureProductExternalDetectorDialog)
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
	ON_WM_TIMER()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_STN_CLICKED(IDC_DialogTitleStaticText1, &CNewConfigureProductExternalDetectorDialog::OnDialogtitlestatictext1)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewConfigureProductExternalDetectorDialog message handlers

void CNewConfigureProductExternalDetectorDialog::OnFunction1Button() 
{
	//Main Menu was pressed
	if (vInAutoSetup)
	{
		if(AbortSetupChanges())	
		{
			if (vGlobaluCSerialPort->vDiscoverModeOn)
				OnFunction5Button();
			CDialog::EndDialog(10);
		}
	}
	else
	if ((!vChangeMade) || (AbortSetupChanges()))
	{
		if (vGlobaluCSerialPort->vDiscoverModeOn)
			OnFunction5Button();
		CDialog::EndDialog(10);
	}
}

void CNewConfigureProductExternalDetectorDialog::OnFunction2Button() 
{
	//Save and exit was pressed
	SaveAndExit(1);
	//else
	//{
		/*
		//do autosetup
		if (vMainWindowPointer->InterlocksOK())
		{
			vLocalSystemData->vAutoSetupAuxiliaryDetectorNumber = 
				vDetectorNumberEditing + vFirstShown;
			if (vLocalConfigurationData->vUseMetric)
			{
				vLocalCurrentProductPointer->SetExternalDetectorWindowStart(
					vLocalSystemData->vAutoSetupAuxiliaryDetectorNumber,(float)(1 * 25.4));
				vLocalCurrentProductPointer->SetExternalDetectorWindowEnd(
					vLocalSystemData->vAutoSetupAuxiliaryDetectorNumber,(float)(100 * 25.4));
			}
			else
			{
				vLocalCurrentProductPointer->SetExternalDetectorWindowStart(
					vLocalSystemData->vAutoSetupAuxiliaryDetectorNumber,(float)1);
				vLocalCurrentProductPointer->SetExternalDetectorWindowEnd(
					vLocalSystemData->vAutoSetupAuxiliaryDetectorNumber,(float)100);
			}
			vMainWindowPointer->SetupProduct(vLocalCurrentProductPointer,false);

			vLocalSystemData->vLastAutoSetupAuxiliaryDetectorActivePosition = 0;
			vLocalSystemData->vLastAutoSetupAuxiliaryDetectorInActivePosition = 0;

			if (vLocalSystemData->vXRayOnNotice)
			{
			}
			else
			{

				int TimerResult = SetTimer(vStartTimerHandle,2200,NULL);
				if (!TimerResult)
					ReportErrorMessage("Error-Start Timer Failed",cEMailInspx,32000);
			}
		}
		else
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\nSystem not started\nCheck Interlock status on Main Menu";
			TempNoticeDialog.vType = cErrorMessage;
			TempNoticeDialog.DoModal();
		}
		*/
	//}
}

void CNewConfigureProductExternalDetectorDialog::SaveAndExit(int TempReturn) 
{
	//Save and exit was pressed
	if (ConfigurationOK(vDetectorNumberEditing + vFirstShown))
	{
		//if ((!vChangeMade) || (KeepSetupChanges()))
		{
			if (vGlobaluCSerialPort->vDiscoverModeOn)
				OnFunction5Button();

			BYTE TempLoop = 0;
			BYTE TempNumberOfBackupDetectors = 0;
			for (; TempLoop < cNumberOfExternalDetectors; TempLoop++)
			{
				//if enabled or disabled any sensors must reload names
				if ((vLocalCurrentProductPointer->vExternalDetectorMode[TempLoop] == cExternalDetectorDisabledMode) != (vOriginalDetectorMode[TempLoop] == cExternalDetectorDisabledMode))
				{
					vLocalSystemData->vInspectionNameChanged = true;
					vLocalSystemData->vInspectionNameMoved = true;
				}

				if (vDetectorMode[TempLoop] == cBackupDetectorMode)
					TempNumberOfBackupDetectors++;

				vLocalCurrentProductPointer->SetExternalDetectorWindowStart(TempLoop,(float)vWindowStart[TempLoop]);
				vLocalCurrentProductPointer->SetExternalDetectorWindowEnd(TempLoop,(float)vWindowEnd[TempLoop]);
				//vLocalCurrentProductPointer->vExternalDetectorEnable[TempLoop] = vEnabled[TempLoop];
				vLocalCurrentProductPointer->vExternalDetectorMode[TempLoop] = vDetectorMode[TempLoop];
				vLocalCurrentProductPointer->vExternalDetectorEjector[TempLoop] = vDetectorEjector[TempLoop];

				if (((vLocalCurrentProductPointer->vExternalDetectorWindowStart[TempLoop] > 0) &&
					(vLocalCurrentProductPointer->vExternalDetectorMode[TempLoop] > 0)) || (vLocalCurrentProductPointer->vExternalDetectorMode[TempLoop] == cBackupDetectorMode) || 
					(vLocalCurrentProductPointer->vExternalDetectorMode[TempLoop] == cValveMonitorIndexPulseInput) || (vLocalCurrentProductPointer->vExternalDetectorMode[TempLoop] == cSeamerIndexPulseInput) || 
					(vLocalCurrentProductPointer->vExternalDetectorMode[TempLoop] == cSeamerObjectPulseInput) || (vLocalCurrentProductPointer->vExternalDetectorMode[TempLoop] == cSeamerPocketPulseInput) ||
					(vLocalCurrentProductPointer->vExternalDetectorMode[TempLoop] == cSampleEjectNextContainerMode))
					vLocalCurrentProductPointer->vExternalDetectorEnable[TempLoop] = true;
				else
					vLocalCurrentProductPointer->vExternalDetectorEnable[TempLoop] = false;

				if ((vLocalCurrentProductPointer->vExternalDetectorMode[TempLoop] == 3) ||
					(vLocalCurrentProductPointer->vExternalDetectorMode[TempLoop] == 4))
				{
					vLocalCurrentProductPointer->SetExternalDetectorWindowEnd(TempLoop,
						vLocalCurrentProductPointer->vExternalDetectorWindowStart[TempLoop]);
				}
			}
			if (!((TempNumberOfBackupDetectors == 0) || (TempNumberOfBackupDetectors == 2)))
			{
				CNoticeDialog TempNoticeDialog;
				TempNoticeDialog.vNoticeText = "\nIt is highly recommended\nthat two backup detectors\nbe used at half the container width apart\nto assure a container backup detection.";
				TempNoticeDialog.vType = cErrorMessage;
				TempNoticeDialog.DoModal();
			}

			if (vGlobaluCSerialPort->vDiscoverModeOn)
				OnFunction5Button();
			CDialog::EndDialog(TempReturn);
		}
	}
}

void CNewConfigureProductExternalDetectorDialog::OnFunction3Button() 
{
	//Next or previous button was pressed
	if (vGlobaluCSerialPort)
	if (ConfigurationOK(vDetectorNumberEditing + vFirstShown))
	if (vGlobaluCSerialPort->vNumberOfExternalDetectors > 2)
	{
		if (vFirstShown == 0)
		{
			vFirstShown = 2;
		}
		else
		if (vFirstShown + 2 < vGlobaluCSerialPort->vNumberOfExternalDetectors)
		{
			vFirstShown = vFirstShown + 1;
		}
		else
		{
			vFirstShown = 0;
		}
		vDetectorNumberEditing = 0;

		if (vGlobaluCSerialPort->vDiscoverModeOn)
		{
			OnFunction5Button(); //turn off
			OnFunction5Button(); //turn back on
		}

		UpdateDisplay();
	}
}

void CNewConfigureProductExternalDetectorDialog::OnFunction4Button() 
{
	//Cancel was pressed
	if (!vChangeMade)
		CDialog::OnCancel();
	if (AbortSetupChanges())
	{
		vWindowStart[0] = vOldWindowStartDetector1;
		vWindowStart[1] = vOldWindowStartDetector2;
		vWindowEnd[0] = vOldWindowEndDetector1;
		vWindowEnd[1] = vOldWindowEndDetector2;
		vDetectorMode[0] = vOldDetectorMode1;
		vDetectorMode[1] = vOldDetectorMode2;
		vDetectorEjector[0] = vOldDetectorEjector1;
		vDetectorEjector[1] = vOldDetectorEjector2;
		UpdateSetupWithNewChanges();
		CDialog::OnCancel();
	}
}

void CNewConfigureProductExternalDetectorDialog::OnFunction5Button() 
{
	if (vGlobaluCSerialPort)
	if ((vDetectorMode[vDetectorNumberEditing + vFirstShown] < cRejectInWindowMode)  || (vDetectorMode[vDetectorNumberEditing + vFirstShown] > cAcceptAtStartMode))
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\n\nDiscover Mode is for Detector Modes 1 to 4 only.";
		TempNoticeDialog.vType = cNoticeMessage;
		TempNoticeDialog.DoModal();
	}
	else
	if (!vGlobaluCSerialPort->vDiscoverModeOn)
	{	//enable discover mode
		vGlobaluCSerialPort->vDiscoverModeActiveDetectorNumber = vDetectorNumberEditing + vFirstShown;

		if (vLocalSystemData->vSystemRunMode == cStoppedSystemMode)
		{
			int TimerResult = SetTimer(vStartSetupTimerHandle,20,NULL);
			if (!TimerResult)
				ReportErrorMessage("Error-Start Timer Failed",cEMailInspx,32000);
		}

		vLocalSystemData->vKeepExternalDetectorsEnabled = true;

		if (vLocalSystemData->vKeepExternalDetectorsEnabled)
		{
			tSerialCommand TempCommand;
			TempCommand[0] = 0x1C;
			TempCommand[1] = 0x02;  //group 2 is Auxiliary Detectors
			TempCommand[2] = 0x00;
			if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)
			{
				if (vLocalSystemData->vKeepExternalDetectorsEnabled)
					TempCommand[3] = vLocalSystemData->vCurrentExternalDetectorMask;
				else
					TempCommand[3] = vLocalSystemData->vCurrentAlwaysOnExternalDetectorMask;
			}
			else
				TempCommand[3] = 0;

			vGlobaluCSerialPort->SendSerialCommand(TempCommand);
			TempCommand[0] = 0x33;
			TempCommand[1] = 0x00;
			TempCommand[2] = 0x00;
			if (vLocalSystemData->vKeepExternalDetectorsEnabled)
				TempCommand[3] = 3;
			else
				TempCommand[3] = 4;
			vGlobaluCSerialPort->SendSerialCommand(TempCommand);
	
			//initialize Discover Mode Variables to zero
			for (int TempLoop = 0; TempLoop < cNumberOfDiscoverSamples; TempLoop++)
			{
				vGlobaluCSerialPort->vDiscoverModeDataIsActive[TempLoop] = false;
				vGlobaluCSerialPort->vDiscoverModeDataBeltPosition[TempLoop] = 0;
			}
			vGlobaluCSerialPort->vDiscoverModeDataIndex = 0;
			vGlobaluCSerialPort->vDiscoverModeDataOldIndex = 255; //force screen update

			//cnotice message to send product through ScanTrac
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\nSend One Container Through The ScanTrac\nTo Update Screen";
			TempNoticeDialog.vType = cNoticeMessage;
			TempNoticeDialog.DoModal();

			//display where detector is on, off, and sensing.
			vDiscoverModeTextBoxString = " " + vLocalConfigurationData->vExternalDetector[vDetectorNumberEditing + vFirstShown].vName + " Detector Changes (Active ";
			
			if (vLocalConfigurationData->vExternalDetector[vDetectorNumberEditing + vFirstShown].vActiveHigh)	
				vDiscoverModeTextBoxString = vDiscoverModeTextBoxString + "High) :\n\n";
			else
				vDiscoverModeTextBoxString = vDiscoverModeTextBoxString + "Low) :\n\n";
			/*
			double TempFirstActive = 0;
			double TempFirstInactiveAfterFirstActive = 0;
			
			for (i = 0; i < vGlobaluCSerialPort->vDiscoverModeDataIndex; i++)
			{
				double TempInches = BPSubtract(vGlobaluCSerialPort->vDiscoverModeDataBeltPosition[i+1] - vGlobaluCSerialPort->vDiscoverModeDataBeltPosition[0]) / vGlobalPixelsPerUnit;

				CString TempInchesString = dtoa(TempInches);
				int TempDecimalIndex = TempInchesString.Find('.');
				if (TempInchesString.GetLength() > TempDecimalIndex + 1)
					TempInchesString.Left(TempDecimalIndex + 1);
				
				if ((ATOF(vWindowStart[vDetectorNumberEditing + vFirstShown]) <= TempInches + 0.03)
					&& (ATOF(vWindowStart[vDetectorNumberEditing + vFirstShown]) >= TempInches - 0.03))
				{
					//make window end 2 decimal digits
					TempInchesString = vWindowStart[vDetectorNumberEditing + vFirstShown] + " (Window Start)";
				}
				if (ATOF(vWindowEnd[vDetectorNumberEditing + vFirstShown]) == TempInches)
				{
					//make window end 2 decimal digits
					TempInchesString = vWindowEnd[vDetectorNumberEditing + vFirstShown] + " (Window End)";
				}
				
				if (vGlobaluCSerialPort->vDiscoverModeDataIsActive[vDetectorNumberEditing + vFirstShown][i])
				{
					vDiscoverModeTextBoxString = vDiscoverModeTextBoxString + " Sensor is Active, at " + TempInchesString;
					if (!TempFirstActive)
						TempFirstActive = TempInches;
				}
				else
				{
					vDiscoverModeTextBoxString = vDiscoverModeTextBoxString + " Sensor is Inactive, at " + TempInchesString;
					if (!TempFirstInactiveAfterFirstActive)
					if (TempFirstActive)
						TempFirstInactiveAfterFirstActive = TempInches;
				}

				vDiscoverModeTextBoxString = vDiscoverModeTextBoxString + "\n";
			}

			if (TempFirstActive)
			if (TempFirstInactiveAfterFirstActive)
				vDiscoverModeTextBoxString = vDiscoverModeTextBoxString + "\n Recommended Trigger Start Value: " 
					+ dtoa((TempFirstActive + TempFirstInactiveAfterFirstActive)/2);
			*/

			vDiscoverModeTextBoxString = " Waiting for Container";
			SetDlgItemText(IDC_DiscoverModeTextBox, vDiscoverModeTextBoxString);
			m_DiscoverModeTextBox.ShowWindow(SW_SHOW);
			
			m_SubFunction3Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_SubFunction3Button.GetRectToInvalidateArrow());
		
			CString TempString = vDiscoverModeTextBoxString;
			if (!(TempString.Remove('\n') > (8 + vDiscoverModeTextPosition)))
			{
				m_SubFunction4Button.ShowWindow(SW_HIDE);
				this->InvalidateRect(&m_SubFunction4Button.GetRectToInvalidateArrow());
			}
			else
			{
				m_SubFunction4Button.ShowWindow(SW_SHOW);
			}
			vGlobaluCSerialPort->vDiscoverModeOn = true;
			SetDlgItemText(IDC_Function5Button, _T("Disable Discover"));
		}
		else
		{
			//cnotice message 
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\n\nDiscover Mode is Off";
			TempNoticeDialog.vType = cNoticeMessage;
			TempNoticeDialog.DoModal();
		}
	}
	else //if discover mode off
	{
		
		//set variable
		vGlobaluCSerialPort->vDiscoverModeOn = false;
		vGlobaluCSerialPort->vDiscoverModeDataIndex = 0; //stop taking data

		SetDlgItemText(IDC_Function5Button, _T("Discover Mode"));
		//turn off force enable of external detectors
		if (vLocalSystemData->vKeepExternalDetectorsEnabled == true)
		{
			vLocalSystemData->vKeepExternalDetectorsEnabled = false;
			tSerialCommand TempCommand;
			TempCommand[0] = 0x1C;
			TempCommand[1] = 0x02;  //group 2 is Auxiliary Detectors
			TempCommand[2] = 0x00;
			if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)
			{
				if (vLocalSystemData->vKeepExternalDetectorsEnabled)
					TempCommand[3] = vLocalSystemData->vCurrentExternalDetectorMask;
				else
					TempCommand[3] = vLocalSystemData->vCurrentAlwaysOnExternalDetectorMask;
			}
			else
				TempCommand[3] = 0;
			if (vGlobaluCSerialPort)
				vGlobaluCSerialPort->SendSerialCommand(TempCommand);
			TempCommand[0] = 0x33;
			TempCommand[1] = 0x00;
			TempCommand[2] = 0x00;
			if (vLocalSystemData->vKeepExternalDetectorsEnabled)
				TempCommand[3] = 3;
			else
				TempCommand[3] = 4;
			vGlobaluCSerialPort->SendSerialCommand(TempCommand);
		}

		if (vStopSystemOnExit)
		{
			vStopSystemOnExit = false;
			//vMainWindowPointer->TurnSimulatedEncoderOff();
			vMainWindowPointer->StopRunning(true);
			m_XRaysOn.ShowWindow(SW_HIDE);
		}
		//blank text window
		m_DiscoverModeTextBox.ShowWindow(SW_HIDE);
		m_SubFunction3Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction3Button.GetRectToInvalidateArrow());
		m_SubFunction4Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction4Button.GetRectToInvalidateArrow());
		m_SubFunction3Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction3Button.GetRectToInvalidateArrow());
	//	m_SubFunction4Button.ShowWindow(SW_HIDE);

		UpdateDisplay();
		
		
	}
}

void CNewConfigureProductExternalDetectorDialog::OnSubFunction1Button() 
{
	//Select ExternalDetector 1 button pressed
	if (vLocalConfigurationData->vExternalDetector[0 + vFirstShown].vEnabled)
	if (ConfigurationOK(vDetectorNumberEditing + vFirstShown))
	{
		if (vDetectorNumberEditing == 0)
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\n\nThis detector already selected to edit.";
			TempNoticeDialog.vType = cErrorMessage;
			TempNoticeDialog.DoModal();
		}
		else
		{
			vDetectorNumberEditing = 0;
			if (vGlobaluCSerialPort->vDiscoverModeOn)
			{
				OnFunction5Button();//stop discover mode
				OnFunction5Button();//start discover mode
			}
			UpdateDisplay();
		}
	}
}

void CNewConfigureProductExternalDetectorDialog::OnSubFunction2Button() 
{
	//Select ExternalDetector 2 button pressed
	if (vLocalConfigurationData->vExternalDetector[1 + vFirstShown].vEnabled)
	if (ConfigurationOK(vDetectorNumberEditing + vFirstShown))
	{
		if (vDetectorNumberEditing == 1)
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\n\nThis detector already selected to edit.";
			TempNoticeDialog.vType = cErrorMessage;
			TempNoticeDialog.DoModal();
		}
		else
		{
			vDetectorNumberEditing = 1;
			if (vGlobaluCSerialPort->vDiscoverModeOn)
			{
				OnFunction5Button();//stop discover mode
				OnFunction5Button();//start discover mode
			}
			UpdateDisplay();
		}
	}
}

void CNewConfigureProductExternalDetectorDialog::OnSubFunction3Button() 
{	//scroll up one line

	if (vDiscoverModeTextPosition)
	{
		int i = 0;
		int TempEnterIndex = 0;
		CString TempString = vDiscoverModeTextBoxString;
	//	int TempNumber = 0;

		if ((TempString.Remove('\n')) == (8 + vDiscoverModeTextPosition))
		{
			m_SubFunction4Button.ShowWindow(SW_SHOW);
		}

		TempString = vDiscoverModeTextBoxString;

		vDiscoverModeTextPosition--;

		for (i = 0; i < vDiscoverModeTextPosition; i++)
		{
			TempEnterIndex = TempString.Find('\n');
			if (TempEnterIndex != -1)
			if (TempString.GetLength() - TempEnterIndex > 0)
				TempString = TempString.Right(TempString.GetLength() - TempEnterIndex - 1);
		}

		SetDlgItemText(IDC_DiscoverModeTextBox, TempString);

		if (!vDiscoverModeTextPosition)
		{
			m_SubFunction3Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_SubFunction3Button.GetRectToInvalidateArrow());
		}
	}
}

void CNewConfigureProductExternalDetectorDialog::OnSubFunction4Button() 
{	//scroll down one line

	CString TempString = vDiscoverModeTextBoxString;
	int TempNumber= 0;
	if ((TempNumber = TempString.Remove('\n')) > (8 + vDiscoverModeTextPosition))
	{
		int i = 0;
		int TempEnterIndex = 0;
		
		TempString = vDiscoverModeTextBoxString;

		if (!vDiscoverModeTextPosition)
		{
			m_SubFunction3Button.ShowWindow(SW_SHOW);
		}

		vDiscoverModeTextPosition++;

		for (i = 0; i < vDiscoverModeTextPosition; i++)
		{
			TempEnterIndex = TempString.Find('\n');
			if (TempEnterIndex != -1)
			if (TempString.GetLength() - TempEnterIndex > 0)
				TempString = TempString.Right(TempString.GetLength() - TempEnterIndex - 1);
		}

		SetDlgItemText(IDC_DiscoverModeTextBox, TempString);

		if (!((TempNumber) > (8 + vDiscoverModeTextPosition)))
		{
			m_SubFunction4Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_SubFunction4Button.GetRectToInvalidateArrow());
		}
	}
}

void CNewConfigureProductExternalDetectorDialog::OnSubFunction5Button() 
{
	//Change Ejector button was pressed
	if (vDetectorMode[vDetectorNumberEditing + vFirstShown])
	{
		if ((vDetectorMode[vDetectorNumberEditing + vFirstShown] < cBackupDetectorMode) || (vDetectorMode[vDetectorNumberEditing + vFirstShown] == cSampleEjectNextContainerMode))
		{  //use ejector
			SelectAuxiliaryDetectorEjectors();
		}
		else  //backup detector, use can stop
		if (vDetectorMode[vDetectorNumberEditing + vFirstShown] == cBackupDetectorMode)
		{
			vDetectorEjector[vDetectorNumberEditing + vFirstShown]++;
			if (vDetectorEjector[vDetectorNumberEditing + vFirstShown] >= cNumberOfCanStops)
				vDetectorEjector[vDetectorNumberEditing + vFirstShown] = 0;
			UpdateDisplay();
			SetChangeMade();
		}
	}
	else
	{
		MustSelectActiveModeDialog();
	}
}

void CNewConfigureProductExternalDetectorDialog::OnSubFunction6Button() 
{
	//Edit Window Start Button Was Pressed
	if (vDetectorMode[vDetectorNumberEditing + vFirstShown])
	{
		CNumericEntryDialog INumericEntryDialog;  
		
		//Set dialog box data titles and number value
		INumericEntryDialog.vEditString = dtoa(vWindowStart[vDetectorNumberEditing + vFirstShown], 1);

		INumericEntryDialog.m_DialogTitleStaticText1 = "Distance From Container Trigger To Window Start";
		INumericEntryDialog.m_DialogTitleStaticText2 = "Ejector: " + vCurrentDetectorName;
		INumericEntryDialog.m_DialogTitleStaticText3 = "Product: " + *vProductName;
		INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + INumericEntryDialog.vEditString;

		INumericEntryDialog.m_UnitsString = "Measured in " + vLocalConfigurationData->vUnitsString;
		INumericEntryDialog.vMaxValue = 500; //cMaxBeltPositionOffset / vGlobalPixelsPerUnit;
		INumericEntryDialog.vIntegerOnly = false;
		//Pass control to dialog box and display
		int nResponse = INumericEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			vWindowStart[vDetectorNumberEditing + vFirstShown] = ATOF(INumericEntryDialog.vEditString);
								
			if ((vDetectorMode[vDetectorNumberEditing + vFirstShown] == cRejectAtStartMode) || (vDetectorMode[vDetectorNumberEditing + vFirstShown] == cAcceptAtStartMode))  //not using end, so set it
				vWindowEnd[vDetectorNumberEditing + vFirstShown] = vWindowStart[vDetectorNumberEditing + vFirstShown];
			else
			if (vWindowEnd[vDetectorNumberEditing + vFirstShown] < vWindowStart[vDetectorNumberEditing + vFirstShown])
				vWindowEnd[vDetectorNumberEditing + vFirstShown] = vWindowStart[vDetectorNumberEditing + vFirstShown] + 1;

			if ((vWindowStart[vDetectorNumberEditing + vFirstShown] == 0) && (vLocalCurrentProductPointer->vExternalDetectorMode[vDetectorNumberEditing + vFirstShown] > 0))
				vEnabled[vDetectorNumberEditing + vFirstShown] = false;
			else
				vEnabled[vDetectorNumberEditing + vFirstShown] = true;

			UpdateDisplay();
			SetChangeMade();
		}
		else 
		if (nResponse == 10)
		{
			//Main Menu button pressed
			if (vGlobaluCSerialPort->vDiscoverModeOn)
				OnFunction5Button();
			CDialog::EndDialog(10);
		}
	}
	else
	{
		MustSelectActiveModeDialog();
	}
}

void CNewConfigureProductExternalDetectorDialog::OnSubFunction7Button() 
{
	//Edit Window End Button Was Pressed
	if (vDetectorMode[vDetectorNumberEditing + vFirstShown])
	if (!((vDetectorMode[vDetectorNumberEditing + vFirstShown] == 3) || (vDetectorMode[vDetectorNumberEditing + vFirstShown] == 4)))
	{
		CNumericEntryDialog INumericEntryDialog;  
		
		//Set dialog box data titles and number value
		INumericEntryDialog.vEditString = dtoa(vWindowEnd[vDetectorNumberEditing + vFirstShown], 1);

		INumericEntryDialog.m_DialogTitleStaticText1 = "Distance From Container Trigger To Window End";
		INumericEntryDialog.m_DialogTitleStaticText2 = "Ejector: " + vCurrentDetectorName;
		INumericEntryDialog.m_DialogTitleStaticText3 = "Product: " + *vProductName;
		INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + INumericEntryDialog.vEditString;

		INumericEntryDialog.m_UnitsString = "Measured in " + vLocalConfigurationData->vUnitsString;
		INumericEntryDialog.vMaxValue = 65000 / vGlobalPixelsPerUnit;
		INumericEntryDialog.vIntegerOnly = false;
		//Pass control to dialog box and display
		int nResponse = INumericEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			vWindowEnd[vDetectorNumberEditing + vFirstShown] = ATOF(INumericEntryDialog.vEditString);

			SetDlgItemText(IDC_WindowEnd1,dtoa(vWindowEnd[0 + vFirstShown], 1));
			SetDlgItemText(IDC_WindowEnd2,dtoa(vWindowEnd[1 + vFirstShown], 1));
			SetChangeMade();
			UpdateDisplay();
		}
		else 
		if (nResponse == 10)
		{
			//Main Menu button pressed
			if (vGlobaluCSerialPort->vDiscoverModeOn)
				OnFunction5Button();
			CDialog::EndDialog(10);
		}
	}
	else
	{
		MustSelectActiveModeDialog();
	}
	else
	{
		MustSelectActiveModeDialog();
	}
}

void CNewConfigureProductExternalDetectorDialog::OnSubFunction8Button() 
{
	//Change Detector Mode Button Was Pressed
	
	if (vGlobaluCSerialPort->vDiscoverModeOn)
		OnFunction5Button(); //turn discover mode off

	vDetectorMode[vDetectorNumberEditing + vFirstShown]++;
	if (vDetectorMode[vDetectorNumberEditing + vFirstShown] > cSampleEjectNextContainerMode)
		vDetectorMode[vDetectorNumberEditing + vFirstShown] = 0;

	if ((vWindowStart[vDetectorNumberEditing + vFirstShown] == 0) &&
		(vLocalCurrentProductPointer->vExternalDetectorMode[vDetectorNumberEditing + vFirstShown] > 0))
		vEnabled[vDetectorNumberEditing + vFirstShown] = false;
	else
		vEnabled[vDetectorNumberEditing + vFirstShown] = true;

	UpdateDisplay();
	SetChangeMade();
}

BOOL CNewConfigureProductExternalDetectorDialog::PreTranslateMessage(MSG* pMsg) 
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
	if (pMsg->message == cXRaysOnMessage)
	{
		if (pMsg->lParam)
		{
			CString TempText("");
			TempText.LoadString(IDS_XRAY);
			SetDlgItemText(IDC_Calibrating,TempText + " " + dtoa(pMsg->lParam, 0));
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
						TempText.LoadString(IDS_XRAYON);
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
						TempText.LoadString(IDS_XRAYON);
					else
					{
						TempText.LoadString(IDS_XRAY);
						TempText = TempText + " **";
					}
					SetDlgItemText(IDC_Calibrating,TempText);
					m_Calibrating.ShowWindow(SW_SHOW);
				}
				break;
			}
		}
		return true;
	}
	if (pMsg->message == cNewImageReadyToDisplay)
	{ //new image newframe

	}
	if (pMsg->message == WM_KEYDOWN)
	{
		int TempKey = pMsg->wParam;
		if (vGlobalShiftKeyDown)
			PostThreadMessage(vGlobalCallingThreadID,cHiddenKeyPressed, 0, pMsg->wParam);
		else
		switch (TempKey)
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
					cShowHelp, 8, 0);
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

HBRUSH CNewConfigureProductExternalDetectorDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	HWND TempDialogHwnd = this->m_hWnd;
	int TextSize;
	pDC->SetAttribDC(pDC->m_hDC);

	if (pWnd->GetDlgCtrlID() == IDC_DialogTitleStaticText1)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DialogTitleStaticText, 8);
		pDC->SetTextColor(vGlobalDialogTitleColor);
	}

	if (pWnd->GetDlgCtrlID() == IDC_XRaysOn)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_XRaysOn, 7);
		pDC->SetTextColor(cYellow);
		pDC->SetBkColor(cRed);
		return vLocalSystemData->vRedBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Calibrating)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Calibrating, 7);
		pDC->SetTextColor(cYellow);
		pDC->SetBkColor(cRed);
		return vLocalSystemData->vRedBrush;
	}

	if (pWnd->GetDlgCtrlID() == IDC_WindowStart1)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_WindowStart1, 5);
		pDC->SetBkMode(TRANSPARENT);
		if (vDetectorNumberEditing == 0)
			return vLocalSystemData->vGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_WindowEnd1) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_WindowEnd1, 5);
		pDC->SetBkMode(TRANSPARENT);
		if (vDetectorNumberEditing == 0)
			return vLocalSystemData->vGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DetectorName1)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DetectorName1, 5);
		pDC->SetBkMode(TRANSPARENT);
		if (vDetectorNumberEditing == 0)
			return vLocalSystemData->vGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DetectorMode1)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_DetectorMode1, 5);		//not function button but using this call because 2 line display
		pDC->SetBkMode(TRANSPARENT);
		if (vEnableMode1)
			pDC->SetTextColor(cButtonTextColor);
		else
			pDC->SetTextColor(cGray);

		if (vDetectorNumberEditing == 0)
			return vLocalSystemData->vGreenBrush;
	}

	if (pWnd->GetDlgCtrlID() == IDC_WindowStart2) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_WindowStart2, 5);
		pDC->SetBkMode(TRANSPARENT);
		if (vDetectorNumberEditing == 1)
			return vLocalSystemData->vGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_WindowEnd2) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_WindowEnd2, 5);
		pDC->SetBkMode(TRANSPARENT);
		if (vDetectorNumberEditing == 1)
			return vLocalSystemData->vGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DetectorName2)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DetectorName2, 5);
		pDC->SetBkMode(TRANSPARENT);
		if (vDetectorNumberEditing == 1)
			return vLocalSystemData->vGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DetectorMode2)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_DetectorMode2, 5);	//not function button but using this call because 2 line display
		pDC->SetBkMode(TRANSPARENT);
		if (vEnableMode2)
			pDC->SetTextColor(cButtonTextColor);
		else
			pDC->SetTextColor(cGray);

		if (vDetectorNumberEditing == 1)
			return vLocalSystemData->vGreenBrush;
	}

	// TODO: Change any attributes of the DC here
	if (pWnd->GetDlgCtrlID() == IDC_Background)
	{
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	//if (pWnd->GetDlgCtrlID() == IDC_UnitsNote)
	//{
	//	TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_UnitsNote, 5);
	//	pDC->SetBkMode(TRANSPARENT);
	//	return vLocalSystemData->vLightGreenBrush;
	//}
	if (pWnd->GetDlgCtrlID() == IDC_Title1) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Title1, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Title2)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Title2, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Title3)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Title3, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Title4)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Title4, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Title5)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Title5, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Title6)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Title6, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Title7)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Title7, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Title8)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Title8, 5);
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
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function5Button, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		if (!vFunction5ButtonEnable)
		{
			pDC->SetTextColor(cGray);
			return vGlobalButtonColorBrush;
		}
		else
		{
			pDC->SetTextColor(cButtonTextColor);
			if (vGlobaluCSerialPort->vDiscoverModeOn)
				return vLocalSystemData->vRedBrush;
			else
				return vLocalSystemData->vGreenBrush;
		}
	}

	if (pWnd->GetDlgCtrlID() == IDC_SubFunction1Button)
	{   
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction1Button, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		if (vSubFunction1ButtonEnable == true)
			pDC->SetTextColor(cButtonTextColor);
		else
			pDC->SetTextColor(cGray);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction2Button)
	{   
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction2Button, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		if (vSubFunction2ButtonEnable == true)
			pDC->SetTextColor(cButtonTextColor);
		else
			pDC->SetTextColor(cGray);
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
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction5Button, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		if (vSubFunction5ButtonEnable == true)
			pDC->SetTextColor(cButtonTextColor);
		else
			pDC->SetTextColor(cGray);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction6Button)
	{   
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction6Button, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		if (vSubFunction6ButtonEnable == true)
			pDC->SetTextColor(cButtonTextColor);
		else
			pDC->SetTextColor(cGray);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction7Button)
	{   
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction7Button, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		if (vSubFunction7ButtonEnable == true)
			pDC->SetTextColor(cButtonTextColor);
		else
			pDC->SetTextColor(cGray);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction8Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction8Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CNewConfigureProductExternalDetectorDialog::DisplayEjectorNames()
{
	CString TempString("");
	TempString = "";

	if ((vDetectorMode[0 + vFirstShown] < cBackupDetectorMode) || (vDetectorMode[vDetectorNumberEditing + vFirstShown] == cSampleEjectNextContainerMode))
	{
			/*
		for (BYTE TempLoop = 0; TempLoop < cNumberOfEjectors; TempLoop++)
		{
			if ((vDetectorEjector[0 + vFirstShown] >> TempLoop) & 1)  //if selected this ejector
			{
				if (TempString.GetLength())
					TempString = TempString + "\n";
				TempString = 
					vLocalConfigurationData->GetEjectorNames(TempLoop + 1);
				if (!vLocalConfigurationData->vEjector[TempLoop].vEnabled)
					TempString = TempString + " - Disabled";
			}
		}
			*/
		TempString = vLocalConfigurationData->GetEjectorNames(vDetectorEjector[0 + vFirstShown]);

		if (TempString.GetLength() > 18)
			m_DetectorName1.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		else
			m_DetectorName1.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
		if (!TempString.GetLength())
			TempString = "None";

		SetDlgItemText(IDC_DetectorName1,TempString);
	}
	else
	if (vDetectorMode[0 + vFirstShown] == cBackupDetectorMode)
	{
		m_DetectorName1.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
		if (vDetectorEjector[0 + vFirstShown] == 0)
			SetDlgItemText(IDC_DetectorName1, _T("Can Stop 1"));
		if (vDetectorEjector[0 + vFirstShown] == 1)
			SetDlgItemText(IDC_DetectorName1, _T("Can Stop 2"));
	}
	else
	if ((vDetectorMode[0 + vFirstShown] == cValveMonitorIndexPulseInput) || (vDetectorMode[0 + vFirstShown] == cSeamerIndexPulseInput) || (vDetectorMode[0 + vFirstShown] == cSeamerObjectPulseInput) || 
		(vDetectorMode[0 + vFirstShown] == cSeamerPocketPulseInput))
	{
		m_DetectorName1.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
		SetDlgItemText(IDC_DetectorName1, _T("Not Used"));
	}

	TempString = "";
	if ((vDetectorMode[1 + vFirstShown] < cBackupDetectorMode) || (vDetectorMode[vDetectorNumberEditing + vFirstShown] == cSampleEjectNextContainerMode))
	{
		TempString = vLocalConfigurationData->GetEjectorNames(vDetectorEjector[1 + vFirstShown]);
		if (TempString.GetLength() > 18)
			m_DetectorName2.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		else
			m_DetectorName2.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
		if (!TempString.GetLength())
			TempString = "None";
		SetDlgItemText(IDC_DetectorName2,TempString);
	}
	else
	if (vDetectorMode[1 + vFirstShown] == cBackupDetectorMode)
	{
		m_DetectorName2.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
		if (vDetectorEjector[1 + vFirstShown] == 0)
			SetDlgItemText(IDC_DetectorName2, _T("Can Stop 1"));
		if (vDetectorEjector[1 + vFirstShown] == 1)
			SetDlgItemText(IDC_DetectorName2, _T("Can Stop 2"));
	}
	else
	if ((vDetectorMode[1 + vFirstShown] == cValveMonitorIndexPulseInput) || (vDetectorMode[1 + vFirstShown] == cSeamerIndexPulseInput) || (vDetectorMode[1 + vFirstShown] == cSeamerObjectPulseInput) || 
		(vDetectorMode[0 + vFirstShown] == cSeamerPocketPulseInput))
	{
		m_DetectorName2.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
		SetDlgItemText(IDC_DetectorName2, _T("Not Used"));
	}
}

void CNewConfigureProductExternalDetectorDialog::DisplayDetectorNames()
{
	char vTempChars[8] = "";

//	if (vFirstShown == 0)
//	  SetDlgItemText(IDC_Function3Button,"Next");
//	else
//	  SetDlgItemText(IDC_Function3Button,"Previous");

	for (BYTE TempLoop = 0; TempLoop < 2;  TempLoop++)
	{
		CString TempString(vLocalConfigurationData->vExternalDetector[TempLoop + vFirstShown].vName);
		if (!vLocalConfigurationData->vExternalDetector[TempLoop + vFirstShown].vEnabled)
			TempString = TempString + " - Disabled";
		switch (TempLoop)
		{
			case 0:
			{
				if (TempString.GetLength() > 10)
					m_SubFunction1Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
				else
					m_SubFunction1Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
				SetDlgItemText(IDC_SubFunction1Button,TempString);
			}
			break;
			case 1:
			{
				if (TempString.GetLength() > 10)
					m_SubFunction2Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
				else
					m_SubFunction2Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
				SetDlgItemText(IDC_SubFunction2Button,TempString);
			}
			break;
			case 2:
			{
//				if (TempString.GetLength() > 10)
//					m_SubFunction3Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
//				else
//					m_SubFunction3Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
//				SetDlgItemText(IDC_SubFunction3Button,TempString);
			}
			break;
			case 3:
			{
//				if (TempString.GetLength() > 10)
//					m_SubFunction4Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
//				else
//					m_SubFunction4Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
//				SetDlgItemText(IDC_SubFunction4Button,TempString);
			}
			break;
		}
	}

	if (vLocalConfigurationData->vExternalDetector[1 + vFirstShown].vEnabled)
	SetDlgItemText(IDC_SubFunction2Button,vLocalConfigurationData->vExternalDetector[1 + vFirstShown].vName);
	else
	{
		CString TempString("Auxiliary Detector ");
		TempString = TempString + dtoa(2 + vFirstShown, 0) + " - Disabled";
		SetDlgItemText(IDC_SubFunction2Button,TempString);
	}
}

bool CNewConfigureProductExternalDetectorDialog::ConfigurationOK(BYTE TempDetector)
{
	bool TempOK = true;

	float TempStart = (float)vWindowStart[TempDetector];
	float TempStop = (float)vWindowEnd[TempDetector];

	if ((vDetectorMode[TempDetector] > cExternalDetectorDisabledMode) && (vDetectorMode[TempDetector] < cBackupDetectorMode) && (TempStart == 0))
	{
		TempOK = false;
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\n\nMust enter Start Position for this detector Mode";
		TempNoticeDialog.vType = cErrorMessage;
		TempNoticeDialog.DoModal();
	}

	if (TempOK)
	if (((vDetectorMode[TempDetector] == cRejectInWindowMode) || (vDetectorMode[TempDetector] == cAcceptInWindowMode)) && (TempStop == 0))
	{
		TempOK = false;
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\n\nMust enter End Position for this detector mode";
		TempNoticeDialog.vType = cErrorMessage;
		TempNoticeDialog.DoModal();
	}

	if (TempOK)
	if (((vDetectorMode[TempDetector] == cRejectInWindowMode) || (vDetectorMode[TempDetector] == cAcceptInWindowMode)) && (TempStop < TempStart))
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\n\nEnd Position must be larger than start position";
		TempNoticeDialog.vType = cErrorMessage;
		TempNoticeDialog.DoModal();
		TempOK = false;
	}

	if (TempOK)
	if ((vDetectorMode[TempDetector] > cExternalDetectorDisabledMode) && (!vDetectorEjector[TempDetector]) && 
		((vDetectorMode[TempDetector] < cBackupDetectorMode) || (vDetectorMode[vDetectorNumberEditing + vFirstShown] == cSampleEjectNextContainerMode)))
	{
		TempOK = false;
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\n\nMust have an ejector for this detector mode";
		TempNoticeDialog.vType = cErrorMessage;
		TempNoticeDialog.DoModal();
	}
	/* can stop is zero based so always have one configured
	if (TempOK)
	if ((vDetectorMode[TempDetector] == cBackupDetectorMode) &&
		(!vDetectorEjector[TempDetector]))
	{
		TempOK = false;
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\n\nMust have a can stop for this detector mode";
		TempNoticeDialog.vType = cErrorMessage;
		TempNoticeDialog.DoModal();
	}
	*/
	for (BYTE TempLoop = 0; TempLoop < cNumberOfEjectors; TempLoop++)
	if ((vDetectorEjector[TempDetector] >> TempLoop) & 1)
	if (TempOK)
	if ((vDetectorMode[TempDetector] > cExternalDetectorDisabledMode) && (!vLocalConfigurationData->vEjector[TempLoop].vEnabled) && 
		((vDetectorMode[TempDetector] < cBackupDetectorMode) || (vDetectorMode[vDetectorNumberEditing + vFirstShown] == cSampleEjectNextContainerMode)))
	{
		TempOK = false;
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\n\nSelected ejector is disabled";
		TempNoticeDialog.vType = cErrorMessage;
		TempNoticeDialog.DoModal();
	}
	if (TempOK)
	if ((vDetectorMode[TempDetector] == cBackupDetectorMode) && (!vLocalConfigurationData->vEjector[vDetectorEjector[TempDetector]].vEnabled))
	{
		TempOK = false;
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\n\nSelected can stop is disabled";
		TempNoticeDialog.vType = cErrorMessage;
		TempNoticeDialog.DoModal();
	}
	return TempOK;
}

void CNewConfigureProductExternalDetectorDialog::SetChangeMade() 
{
	if (!vChangeMade)
	{
		//if ((!vInAutoSetup) || (vLocalSystemData->vSystemRunMode == cStoppedSystemMode))
		{
			m_Function2Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
			SetDlgItemText(IDC_Function2Button, _T("Save\nand Exit"));
			m_Function4Button.ShowWindow(SW_SHOW);
			SetDlgItemText(IDC_Function4Button, _T("Cancel"));
		}
		vChangeMade = true;
	}
}

void CNewConfigureProductExternalDetectorDialog::UpdateDisplay() 
{
	if ((vDetectorMode[vDetectorNumberEditing + vFirstShown] < cRejectInWindowMode)  || (vDetectorMode[vDetectorNumberEditing + vFirstShown] > cAcceptAtStartMode))
		vFunction5ButtonEnable = false;
	else
		vFunction5ButtonEnable = true;
		
	m_Function5Button.Invalidate(false);

	vCurrentDetectorName = vLocalConfigurationData->vExternalDetector[vDetectorNumberEditing + vFirstShown].vName;

	DisplayEjectorNames();
	DisplayDetectorNames();

	if (vDetectorMode[0 + vFirstShown] < cBackupDetectorMode)
	{
		SetDlgItemText(IDC_WindowStart1, dtoa(vWindowStart[0 + vFirstShown],1));
		SetDlgItemText(IDC_WindowEnd1, dtoa(vWindowEnd[0 + vFirstShown], 1));
	}
	else
	{
		SetDlgItemText(IDC_WindowStart1, _T("Not Used"));
		SetDlgItemText(IDC_WindowEnd1, _T("Not Used"));
	}

	if (vDetectorMode[1 + vFirstShown] < cBackupDetectorMode)
	{
		SetDlgItemText(IDC_WindowStart2, dtoa(vWindowStart[1 + vFirstShown], 1));
		SetDlgItemText(IDC_WindowEnd2, dtoa(vWindowEnd[1 + vFirstShown], 1));
	}
	else
	{
		SetDlgItemText(IDC_WindowStart2, _T("Not Used"));
		SetDlgItemText(IDC_WindowEnd2, _T("Not Used"));
	}

	if (vDetectorMode[0 + vFirstShown] == cSampleEjectNextContainerMode)
	{
		vEnableMode1 = true;
		m_WindowStart1.EnableWindow(false);
		m_WindowEnd1.EnableWindow(false);
		m_DetectorMode1.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		vEnableMode1 = true;
	}
	else
	if ((vDetectorMode[0 + vFirstShown] == cValveMonitorIndexPulseInput) || (vDetectorMode[0 + vFirstShown] == cSeamerIndexPulseInput) || (vDetectorMode[0 + vFirstShown] == cSeamerObjectPulseInput) || 
		(vDetectorMode[0 + vFirstShown] == cSeamerPocketPulseInput))
	{
		m_WindowStart1.EnableWindow(false);
		m_WindowEnd1.EnableWindow(false);
		m_DetectorName1.EnableWindow(false);
		m_DetectorMode1.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		vEnableMode1 = true;
	}
	else
	if (vDetectorMode[0 + vFirstShown] == cBackupDetectorMode)
	{
		m_WindowStart1.EnableWindow(false);
		m_WindowEnd1.EnableWindow(false);
		m_DetectorName1.EnableWindow(true);
		m_DetectorMode1.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		vEnableMode1 = true;
	}
	else
	if ((vDetectorMode[0 + vFirstShown]) &&//is enabled
		(vWindowStart[0 + vFirstShown] != 0) && ((vWindowEnd[0 + vFirstShown] != 0) || (vDetectorMode[0 + vFirstShown] == cRejectAtStartMode) || (vDetectorMode[0 + vFirstShown] == cAcceptAtStartMode)))
	{
		m_DetectorMode1.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		m_DetectorName1.EnableWindow(true);
		m_WindowStart1.EnableWindow(true);
		m_WindowEnd1.EnableWindow(true);
		vEnableMode1 = true;

		if ((vDetectorMode[0 + vFirstShown] == cRejectAtStartMode) || (vDetectorMode[0 + vFirstShown] == cAcceptAtStartMode))
			m_WindowEnd1.EnableWindow(false);
		else
			m_WindowEnd1.EnableWindow(true);
	}
	else
	{
		m_DetectorName1.EnableWindow(false);
		m_WindowStart1.EnableWindow(false);
		m_WindowEnd1.EnableWindow(false);
		if (vDetectorMode[0 + vFirstShown])		
			m_DetectorMode1.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		else
			m_DetectorMode1.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
		vEnableMode1 = false;
	}


	if (!vLocalConfigurationData->vExternalDetector[0 + vFirstShown].vEnabled)
	{
		m_DetectorName1.EnableWindow(false);
		m_WindowStart1.EnableWindow(false);
		m_WindowEnd1.EnableWindow(false);
		vEnableMode1 = false;
	}

	if (vDetectorMode[1 + vFirstShown] == cSampleEjectNextContainerMode)
	{
		m_DetectorName2.EnableWindow(true);
		m_WindowStart2.EnableWindow(false);
		m_WindowEnd2.EnableWindow(false);
		m_DetectorName2.EnableWindow(false);
		m_DetectorMode2.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		vEnableMode2 = true;
	}
	else
	if ((vDetectorMode[1 + vFirstShown] == cValveMonitorIndexPulseInput) || (vDetectorMode[1 + vFirstShown] == cSeamerIndexPulseInput) || (vDetectorMode[1 + vFirstShown] == cSeamerObjectPulseInput) || 
		(vDetectorMode[1 + vFirstShown] == cSeamerPocketPulseInput))
	{
		m_DetectorName2.EnableWindow(true);
		m_WindowStart2.EnableWindow(false);
		m_WindowEnd2.EnableWindow(false);
		m_DetectorName2.EnableWindow(false);
		m_DetectorMode2.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		vEnableMode2 = true;
	}
	else
	if (vDetectorMode[1 + vFirstShown] == cBackupDetectorMode)
	{
		m_DetectorName2.EnableWindow(true);
		m_WindowStart2.EnableWindow(false);
		m_WindowEnd2.EnableWindow(false);
		m_DetectorName2.EnableWindow(true);
		m_DetectorMode2.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		vEnableMode2 = true;
	}
	else
	if ((vDetectorMode[1 + vFirstShown]) &&//is enabled
		(vWindowStart[1 + vFirstShown] != 0) && ((vWindowEnd[1 + vFirstShown] != 0) || (vDetectorMode[1 + vFirstShown] == cRejectAtStartMode) || (vDetectorMode[1 + vFirstShown] == cAcceptAtStartMode)))
	{
		m_DetectorMode2.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		m_DetectorName2.EnableWindow(true);
		m_WindowStart2.EnableWindow(true);
		vEnableMode2 = true;

		if ((vDetectorMode[1 + vFirstShown] == cRejectAtStartMode) || (vDetectorMode[1 + vFirstShown] == cAcceptAtStartMode))
			m_WindowEnd2.EnableWindow(false);
		else
			m_WindowEnd2.EnableWindow(true);
	}
	else
	{
		if (vDetectorMode[1 + vFirstShown])		
			m_DetectorMode2.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		else
			m_DetectorMode2.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
		m_DetectorName2.EnableWindow(false);
		m_WindowStart2.EnableWindow(false);
		m_WindowEnd2.EnableWindow(false);
		vEnableMode2 = false;
	}
	if (!vLocalConfigurationData->vExternalDetector[1 + vFirstShown].vEnabled)
	{
		m_DetectorName2.EnableWindow(false);
		m_WindowStart2.EnableWindow(false);
		m_WindowEnd2.EnableWindow(false);
		vEnableMode2 = false;
	}

	if (vDetectorNumberEditing == 0)
		vSubFunction1ButtonEnable = false;	//m_SubFunction1Button.EnableWindow(false);
	else
		vSubFunction1ButtonEnable = true;	//m_SubFunction1Button.EnableWindow(true);

	if (vDetectorNumberEditing == 1)
		vSubFunction2ButtonEnable = false;	//m_SubFunction2Button.EnableWindow(false);
	else
		vSubFunction2ButtonEnable = true;	//m_SubFunction2Button.EnableWindow(true);

	if (vDetectorMode[vDetectorNumberEditing + vFirstShown])
	{
		if (vNumberEjectorsConfigured > 0)
			vSubFunction5ButtonEnable = true;	//m_SubFunction5Button.EnableWindow(true);
		else
			vSubFunction5ButtonEnable = false;	//m_SubFunction5Button.EnableWindow(false);
		//vSubFunction5ButtonEnable = true;		//m_SubFunction5Button.EnableWindow(true);
		vSubFunction6ButtonEnable = true;	//m_SubFunction6Button.EnableWindow(true);

		if ((vDetectorMode[vDetectorNumberEditing + vFirstShown] == cRejectAtStartMode) || (vDetectorMode[vDetectorNumberEditing + vFirstShown] == cAcceptAtStartMode))
			vSubFunction7ButtonEnable = false;	//m_SubFunction7Button.EnableWindow(false);
		else
			vSubFunction7ButtonEnable = true;	//m_SubFunction7Button.EnableWindow(true);
	}
	else
	{
		vSubFunction5ButtonEnable = false;	//m_SubFunction5Button.EnableWindow(false);
		vSubFunction6ButtonEnable = false;	//m_SubFunction6Button.EnableWindow(false);
		vSubFunction7ButtonEnable = false;	//m_SubFunction7Button.EnableWindow(false);
	}
	m_SubFunction1Button.Invalidate(false);
	m_SubFunction2Button.Invalidate(false);
	m_SubFunction5Button.Invalidate(false);
	m_SubFunction6Button.Invalidate(false);
	m_SubFunction7Button.Invalidate(false);

	m_DetectorName1.Invalidate(false);
	m_DetectorName2.Invalidate(false);

	SetDlgItemText(IDC_DetectorMode1,vLocalSystemData->vDetectorModeString[vDetectorMode[0 + vFirstShown]]);
	SetDlgItemText(IDC_DetectorMode2,vLocalSystemData->vDetectorModeString[vDetectorMode[1 + vFirstShown]]);

	switch (vDetectorMode[vDetectorNumberEditing + vFirstShown])
	{
		case cExternalDetectorDisabledMode:
		{
			m_SubFunction8Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
			SetDlgItemText(IDC_SubFunction8Button, _T("Make Reject In Window"));
		}
		break;
		case cRejectInWindowMode:
		{
			m_SubFunction8Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
			SetDlgItemText(IDC_SubFunction8Button, _T("Make Accept In Window"));
		}
		break;
		case cAcceptInWindowMode:
		{
			m_SubFunction8Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
			SetDlgItemText(IDC_SubFunction8Button, _T("Make Reject At Start"));
		}
		break;
		case cRejectAtStartMode:
		{
			m_SubFunction8Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
			SetDlgItemText(IDC_SubFunction8Button, _T("Make Accept At Start"));
		}
		break;
		case cAcceptAtStartMode:
		{
			m_SubFunction8Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
			SetDlgItemText(IDC_SubFunction8Button, _T("Make Backup Detector"));
		}
		break;
		case cBackupDetectorMode:
		{
			m_SubFunction8Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
			SetDlgItemText(IDC_SubFunction8Button, "Make " + vLocalSystemData->vSamplingName[cFillerSamplingType] + " Monitor Index");
		}
		break;
		case cValveMonitorIndexPulseInput:
		{
			m_SubFunction8Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
			SetDlgItemText(IDC_SubFunction8Button, "Make " + vLocalSystemData->vSamplingName[cSeamerSamplingType] + " Index Input");
		}
		break;
		case cSeamerIndexPulseInput:
		{
			m_SubFunction8Button.ModifyStyle(SS_CENTERIMAGE, NULL, NULL);
			SetDlgItemText(IDC_SubFunction8Button, _T("Make Seamer Object Input"));
		}
		break;
		case cSeamerObjectPulseInput:
		{
			m_SubFunction8Button.ModifyStyle(SS_CENTERIMAGE, NULL, NULL);
			SetDlgItemText(IDC_SubFunction8Button, _T("Make Seamer Pocket Input"));
		}
		break;
		case cSeamerPocketPulseInput:
		{
			m_SubFunction8Button.ModifyStyle(SS_CENTERIMAGE, NULL, NULL);
			SetDlgItemText(IDC_SubFunction8Button, _T("Make Sample Eject Next Container"));
		}
		break;
		case cSampleEjectNextContainerMode:
		{
			m_SubFunction8Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
			SetDlgItemText(IDC_SubFunction8Button, _T("Disable"));
		}
		break;
	}
	DWORD TempNextEjector = 2 + vDetectorEjector[vDetectorNumberEditing + vFirstShown];
	if ((vDetectorMode[vDetectorNumberEditing + vFirstShown] < cBackupDetectorMode) || (vDetectorMode[0 + vFirstShown] == cSampleEjectNextContainerMode))
	{
		if (TempNextEjector > cNumberOfEjectors)
			TempNextEjector = 1;
	}
	else
	{
		if (TempNextEjector > cNumberOfCanStops)
			TempNextEjector = 1;
	}

	if ((vDetectorMode[vDetectorNumberEditing + vFirstShown] < cBackupDetectorMode) || (vDetectorMode[0 + vFirstShown] == cSampleEjectNextContainerMode))
	{
		SetDlgItemText(IDC_SubFunction5Button, _T("Ejectors\nTo\nUse"));
	}
	else
	if (vDetectorMode[vDetectorNumberEditing + vFirstShown] == cBackupDetectorMode)
		SetDlgItemText(IDC_SubFunction5Button,"Can Stop\nTo\nUse" + dtoa(TempNextEjector, 0));
	else
		SetDlgItemText(IDC_SubFunction5Button,"No\nEjector\nNeeded" + dtoa(TempNextEjector, 0));

	if (!vLocalConfigurationData->vExternalDetector[0 + vFirstShown].vEnabled)
	{
		m_DetectorMode1.ShowWindow(SW_HIDE);
		m_DetectorName1.ShowWindow(SW_HIDE);
		m_WindowStart1.ShowWindow(SW_HIDE);
		m_WindowEnd1.ShowWindow(SW_HIDE);
		m_SubFunction1Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction1Button.GetRectToInvalidateArrow());
	}
	else
	{
		m_DetectorMode1.ShowWindow(SW_SHOW);
		m_DetectorName1.ShowWindow(SW_SHOW);
		m_WindowStart1.ShowWindow(SW_SHOW);
		m_WindowEnd1.ShowWindow(SW_SHOW);
		m_SubFunction1Button.ShowWindow(SW_SHOW);
	}
	if (!vLocalConfigurationData->vExternalDetector[1 + vFirstShown].vEnabled)
	{
		m_DetectorMode2.ShowWindow(SW_HIDE);
		m_DetectorName2.ShowWindow(SW_HIDE);
		m_WindowStart2.ShowWindow(SW_HIDE);
		m_WindowEnd2.ShowWindow(SW_HIDE);
		m_SubFunction2Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction2Button.GetRectToInvalidateArrow());
	}
	else
	{
		m_DetectorMode2.ShowWindow(SW_SHOW);
		m_DetectorName2.ShowWindow(SW_SHOW);
		m_WindowStart2.ShowWindow(SW_SHOW);
		m_WindowEnd2.ShowWindow(SW_SHOW);
		m_SubFunction2Button.ShowWindow(SW_SHOW);
	}
	if (vFirstShown == 0)
		SetDlgItemText(IDC_Function3Button, _T("Next"));
	else
		SetDlgItemText(IDC_Function3Button, _T("Previous"));
}

void CNewConfigureProductExternalDetectorDialog::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == vUpdateDisplayTimerHandle)  //called to run system, after setup, to allow uC time
	{
	}
	if (nIDEvent == vStartTimerHandle)  //called to run system, after setup, to allow uC time
	{
		KillTimer(vStartTimerHandle);
		if (vMainWindowPointer->InterlocksOK())
		{
			if (vLocalSystemData->vLogFile.vLogSerialData)
				vLocalSystemData->vLogFile.WriteToLogFile("Start running source not already running",cDebugMessage);
			vMainWindowPointer->StartRunning(true, false);//must turn on source

			vLocalSystemData->vSystemRunMode = cAutoSetupRunningSystem;
		}
	}
	else
	if (nIDEvent == vStartSetupTimerHandle)  //called to run system, after setup, to allow uC time
	{
		KillTimer(vStartSetupTimerHandle);
		if (vMainWindowPointer->InterlocksOK())
		{
			if (vLocalSystemData->vLogFile.vLogSerialData)
				vLocalSystemData->vLogFile.WriteToLogFile("Start running source not already running",cDebugMessage);
			vMainWindowPointer->StartRunning(false, false);//must turn on source
			
			vStopSystemOnExit = true;
			vLocalSystemData->vSystemRunMode = cAutoSetupRunningSystem;
			int TimerResult = SetTimer(vStartSetupTimerHandle,3000,NULL);
			if (!TimerResult)
				ReportErrorMessage("Error-Start Timer Failed",cEMailInspx,32000);
		}
	}
	else
	if (nIDEvent == vUpdateDisplayTimerHandle)
		OneSecondUpdate();
	
	CDialog::OnTimer(nIDEvent);
}

void CNewConfigureProductExternalDetectorDialog::OneSecondUpdate()
{
	if (vOldXRayOnNotice != vLocalSystemData->vXRayOnNotice)
	{
		vOldXRayOnNotice = vLocalSystemData->vXRayOnNotice;
		if (vOldXRayOnNotice)
		{
			m_XRaysOn.ShowWindow(SW_SHOW);
			m_Calibrating.ShowWindow(SW_HIDE);
		}
		else
		{
			m_XRaysOn.ShowWindow(SW_HIDE);
			m_Calibrating.ShowWindow(SW_HIDE);
		}
	}
	if (vGlobaluCSerialPort)
	if (vGlobaluCSerialPort->vDiscoverModeOn)
	{
		if (vGlobaluCSerialPort->vDiscoverModeDataIndex != vGlobaluCSerialPort->vDiscoverModeDataOldIndex)
		{
			vGlobaluCSerialPort->vDiscoverModeDataOldIndex = vGlobaluCSerialPort->vDiscoverModeDataIndex;
			int i;
			vDiscoverModeTextPosition = 0;
			
			vDiscoverModeTextBoxString = " " + vLocalConfigurationData->vExternalDetector[vDetectorNumberEditing + vFirstShown].vName + " Detector Changes (Active ";
						
			if (vLocalConfigurationData->vExternalDetector[vDetectorNumberEditing + vFirstShown].vActiveHigh)
				vDiscoverModeTextBoxString = vDiscoverModeTextBoxString + "High) :\n\n";
			else
				vDiscoverModeTextBoxString = vDiscoverModeTextBoxString + "Low) :\n\n";
				
			double TempFirstActive = 0;
			double TempFirstInactiveAfterFirstActive = 0;

			if (vGlobaluCSerialPort->vDiscoverModeDataIndex == 0)
				vDiscoverModeTextBoxString = vDiscoverModeTextBoxString + " Waiting for Container...\n";
			else
			{
				if (vGlobaluCSerialPort->vDiscoverModeDataIsActive[0])
					vDiscoverModeTextBoxString = vDiscoverModeTextBoxString + " At Trigger Sensor is Active\n";
				else
					vDiscoverModeTextBoxString = vDiscoverModeTextBoxString + " At Trigger Sensor is Inactive\n";
				
				if (vGlobaluCSerialPort->vDiscoverModeDataIndex >  1)
				for (i = 1; i < vGlobaluCSerialPort->vDiscoverModeDataIndex; i++)
				{
					double TempInches = BPSubtract(vGlobaluCSerialPort->vDiscoverModeDataBeltPosition[i], vGlobaluCSerialPort->vDiscoverModeDataBeltPosition[0]) / vGlobalPixelsPerUnit;

					CString TempInchesString = dtoa(TempInches, 1);
					int TempDecimalIndex = TempInchesString.Find('.'); //remove excess digits of precision
					if (TempInchesString.GetLength() > TempDecimalIndex + 3)
						TempInchesString = TempInchesString.Left(TempDecimalIndex + 3);
					
					if ((vWindowStart[vDetectorNumberEditing + vFirstShown] <= (double)(TempInches + 0.09))
						&& (vWindowStart[vDetectorNumberEditing + vFirstShown] >= (double)(TempInches - 0.09)))
					//{
						TempInchesString = dtoa(vWindowStart[vDetectorNumberEditing + vFirstShown], 1) + " (Window Start)";
					//}
					//else
					//{
					//	//if (vDetectorNumberEditing == 0)
					//	//{
					//	//	if ((vWindowStart[1] <= (double)(TempInches + 0.09)) && (vWindowStart[1] >= (double)(TempInches - 0.09)))
					//	//	{
					//	//		if (TempInchesString.GetLength() > TempDecimalIndex + 3)
					//	//			TempInchesString = dtoa(vWindowStart[1], 1).Left(TempDecimalIndex + 3);
					//	//	}
					//	//}
					//	//else
					//	//{
					//	//	if ((vWindowStart[0] <= (double)(TempInches + 0.09)) && (vWindowStart[0] >= (double)(TempInches - 0.09)))
					//	//	{
					//	//		if (TempInchesString.GetLength() > TempDecimalIndex + 3)
					//	//			TempInchesString = dtoa(vWindowStart[0], 1).Left(TempDecimalIndex + 3);
					//	//	}
					//	//}
					//}


					if (((double)vWindowEnd[vDetectorNumberEditing + vFirstShown] <= (double)(TempInches + 0.09))
						&& ((double)vWindowEnd[vDetectorNumberEditing + vFirstShown] >= (double)(TempInches - 0.09)))
					//{
						//make window end 2 decimal digits
						TempInchesString = dtoa(vWindowEnd[vDetectorNumberEditing + vFirstShown], 1) + " (Window End)";
					//}
					//else
					//{
					//	//if (vDetectorNumberEditing == 0)
					//	//{
					//	//	if (((double)vWindowEnd[1] <= (double)(TempInches + 0.09)) && ((double)vWindowEnd[1] >= (double)(TempInches - 0.09)))
					//	//	{
					//	//		TempInchesString = dtoa(vWindowEnd[1], 1);
					//	//	}
					//	//}
					//	//else
					//	//{
					//	//	if (((double)vWindowEnd[0] <= (double)(TempInches + 0.09)) && ((double)vWindowEnd[0] >= (double)(TempInches - 0.09)))
					//	//	{
					//	//		TempInchesString = dtoa(vWindowEnd[0], 1);
					//	//	}
					//	//}
					//}
					
					if (vGlobaluCSerialPort->vDiscoverModeDataIsActive[i])
					{
						vDiscoverModeTextBoxString = vDiscoverModeTextBoxString + " Sensor is Active, at " + TempInchesString;
						if (!TempFirstActive)
							TempFirstActive = TempInches;
					}
					else
					{
						vDiscoverModeTextBoxString = vDiscoverModeTextBoxString + " Sensor is Inactive, at " + TempInchesString;
						if (!TempFirstInactiveAfterFirstActive)
						if (TempFirstActive)
							TempFirstInactiveAfterFirstActive = TempInches;
					}

					vDiscoverModeTextBoxString = vDiscoverModeTextBoxString + "\n";

				}
				if (vGlobaluCSerialPort->vDiscoverModeDataIndex >  1)
				if ((TempFirstActive) && (TempFirstInactiveAfterFirstActive))
				{
					if ((vDetectorMode[vDetectorNumberEditing + vFirstShown] == cRejectInWindowMode) || (vDetectorMode[vDetectorNumberEditing + vFirstShown] == cAcceptInWindowMode))  //show start and end of window
					{
						vDiscoverModeTextBoxString = vDiscoverModeTextBoxString + "\n Recommended Window Start Position: " + dtoa(TempFirstActive, 1);
						vDiscoverModeTextBoxString = vDiscoverModeTextBoxString + "\n Recommended Window End Position: " + dtoa(TempFirstInactiveAfterFirstActive, 1);
					}
					else
						vDiscoverModeTextBoxString = vDiscoverModeTextBoxString + "\n Recommended Window Start Position: " + dtoa((TempFirstActive + TempFirstInactiveAfterFirstActive)/2, 1);
				}
				else
					vDiscoverModeTextBoxString = vDiscoverModeTextBoxString + "\n No Recommended Window Start Value: No Changes";
			}

			SetDlgItemText(IDC_DiscoverModeTextBox, vDiscoverModeTextBoxString);

			m_SubFunction3Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_SubFunction3Button.GetRectToInvalidateArrow());
			CString TempString = vDiscoverModeTextBoxString;
			
			if (!(TempString.Remove('\n') > (8 + vDiscoverModeTextPosition)))
			{
				m_SubFunction4Button.ShowWindow(SW_HIDE);
				this->InvalidateRect(&m_SubFunction4Button.GetRectToInvalidateArrow());
			}
			else
			{
				m_SubFunction4Button.ShowWindow(SW_SHOW);
			}
		}
	}
}

void CNewConfigureProductExternalDetectorDialog::SelectAuxiliaryDetectorEjectors()
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
		if (vNumberEjectorsConfigured == 1)
		{
			vDetectorEjector[vDetectorNumberEditing + vFirstShown] = 1 << vOnlyEjectorNumberConfigured;
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\nOnly one ejector configured in the system.\nIt has been automatically selected.";
			TempNoticeDialog.vType = cNoticeMessage;
			TempNoticeDialog.DoModal();
			UpdateDisplay();
			SetChangeMade();
		}
		else
		{
			int TempResult = vLocalConfigurationData->SelectEjectors("Auxiliary Detector", &vDetectorEjector[vDetectorNumberEditing + vFirstShown], vLocalConfigurationData, false);
			if (TempResult == IDOK)
			{
				UpdateDisplay();
				SetChangeMade();
			}
			else
			if (TempResult == 10)
			{
				//Main Menu button pressed
				if (vGlobaluCSerialPort->vDiscoverModeOn)
					OnFunction5Button();
				CDialog::EndDialog(10);
			}
		}
	}
}

void CNewConfigureProductExternalDetectorDialog::OnDestroy() 
{
	CDialog::OnDestroy();

	
	
	if (vLocalSystemData->vKeepExternalDetectorsEnabled == true)
	{
		//vMainWindowPointer->StopRunning(true);
		vLocalSystemData->vKeepExternalDetectorsEnabled = false;
		tSerialCommand TempCommand;
		TempCommand[0] = 0x1C;
		TempCommand[1] = 0x02;  //group 2 is Auxiliary Detectors
		TempCommand[2] = 0x00;
		if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)
		{
			if (vLocalSystemData->vKeepExternalDetectorsEnabled)
				TempCommand[3] = vLocalSystemData->vCurrentExternalDetectorMask;
			else
				TempCommand[3] = vLocalSystemData->vCurrentAlwaysOnExternalDetectorMask;
		}
		else
			TempCommand[3] = 0;
		if (vGlobaluCSerialPort)
			vGlobaluCSerialPort->SendSerialCommand(TempCommand);
		TempCommand[0] = 0x33;
		TempCommand[1] = 0x00;
		TempCommand[2] = 0x00;
		if (vLocalSystemData->vKeepExternalDetectorsEnabled)
			TempCommand[3] = 3;
		else
			TempCommand[3] = 4;
		if (vGlobaluCSerialPort)
			vGlobaluCSerialPort->SendSerialCommand(TempCommand);
	}

	if (vStopSystemOnExit)
	{
		vStopSystemOnExit = false;
		vMainWindowPointer->StopRunning(true);
		vMainWindowPointer->TurnSimulatedEncoderOff();
//		CNoticeDialog TempNoticeDialog;
//		TempNoticeDialog.vNoticeText = "\n\nturn encoder off";
//		TempNoticeDialog.vType = cNoticeMessage;
//		TempNoticeDialog.DoModal();
	}
}

void CNewConfigureProductExternalDetectorDialog::UpdateSetupWithNewChanges()
{
	if (ConfigurationOK(vDetectorNumberEditing + vFirstShown))
	{
		//if ((!vChangeMade) || (KeepSetupChanges()))
		{
			BYTE TempNumberOfBackupDetectors = 0;
			for (BYTE TempLoop = 0; TempLoop < cNumberOfExternalDetectors; TempLoop++)
			{
				if (vDetectorMode[TempLoop] == cBackupDetectorMode)
					TempNumberOfBackupDetectors++;

				vLocalCurrentProductPointer->SetExternalDetectorWindowStart(TempLoop, (float)vWindowStart[TempLoop]);
				vLocalCurrentProductPointer->SetExternalDetectorWindowEnd(TempLoop, (float)vWindowEnd[TempLoop]);
				//vLocalCurrentProductPointer->vExternalDetectorEnable[TempLoop] = vEnabled[TempLoop];
				vLocalCurrentProductPointer->vExternalDetectorMode[TempLoop] = vDetectorMode[TempLoop];
				vLocalCurrentProductPointer->vExternalDetectorEjector[TempLoop] = vDetectorEjector[TempLoop];

				if (((vLocalCurrentProductPointer->vExternalDetectorWindowStart[TempLoop] > 0) &&
					(vLocalCurrentProductPointer->vExternalDetectorMode[TempLoop] > 0)) ||
					(vLocalCurrentProductPointer->vExternalDetectorMode[TempLoop] == cBackupDetectorMode))
					vLocalCurrentProductPointer->vExternalDetectorEnable[TempLoop] = true;
				else
				if ((vLocalCurrentProductPointer->vExternalDetectorMode[TempLoop] == cValveMonitorIndexPulseInput) || (vLocalCurrentProductPointer->vExternalDetectorMode[TempLoop] == cSeamerIndexPulseInput) ||
					(vLocalCurrentProductPointer->vExternalDetectorMode[TempLoop] == cSeamerObjectPulseInput) || (vLocalCurrentProductPointer->vExternalDetectorMode[TempLoop] == cSeamerPocketPulseInput) || 
					(vLocalCurrentProductPointer->vExternalDetectorMode[TempLoop] == cSampleEjectNextContainerMode))
					vLocalCurrentProductPointer->vExternalDetectorEnable[TempLoop] = true;
				else
					vLocalCurrentProductPointer->vExternalDetectorEnable[TempLoop] = false;

				if ((vLocalCurrentProductPointer->vExternalDetectorMode[TempLoop] == 3) ||
					(vLocalCurrentProductPointer->vExternalDetectorMode[TempLoop] == 4))
				{
					vLocalCurrentProductPointer->SetExternalDetectorWindowEnd(TempLoop,
						vLocalCurrentProductPointer->vExternalDetectorWindowStart[TempLoop]);
				}
			}
			if (!((TempNumberOfBackupDetectors == 0) || (TempNumberOfBackupDetectors == 2)))
			{
				CNoticeDialog TempNoticeDialog;
				TempNoticeDialog.vNoticeText = "\nIt is highly recommended\nthat two backup detectors\nbe used at half the container width apart\nto assure a container backup detection.";
				TempNoticeDialog.vType = cErrorMessage;
				TempNoticeDialog.DoModal();
			}
		}
	}
}

void CNewConfigureProductExternalDetectorDialog::MustSelectActiveModeDialog()
{
	CNoticeDialog TempNoticeDialog;
	TempNoticeDialog.vNoticeText = "\n\n\nDetector mode disabled, must select an active mode.";
	TempNoticeDialog.vType = cErrorMessage;
	TempNoticeDialog.DoModal();
}

void CNewConfigureProductExternalDetectorDialog::OnDialogtitlestatictext1()
{
	//bring up soft key menu for help, print screen, Image, Clear, and hidden shift key
	BYTE TempHiddenKeys = 8;
	if (!PasswordOK(cTemporaryInspxPassword,false))
		TempHiddenKeys = 10;
	PostThreadMessage(vGlobalCallingThreadID, cDisplaySoftKeyMenu, TempHiddenKeys, TempHiddenKeys);
}
