//ScanTrac Side View Source File
// DiagnosticLineDialog.cpp : implementation file
//

#include "stdafx.h"
#include "ScanTrac.h"
#include "DiagnosticLineDialog.h"
#include "NumericEntryDialog.h"
#include "NoticeDialog.h"
#include "SelectItemDialog.h"
#include "SerialPort.h"
#include "Product.h"
#include "ImageAquisitionThread.h"
#include "NumericEntryDialog.h"
#include "YesNoDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDiagnosticLineDialog dialog
extern BYTE vGlobalShiftKeyDown;
extern 	CSerialPort *vGlobaluCSerialPort;
extern int vGlobalPasswordLevel;
extern unsigned long vGlobalDialogTitleColor;
extern CProduct *vGlobalCurrentProduct;
extern CImageAquisitionThread *vGlobalImageAquisitionThread;
extern BYTE vGlobalFrameGrabberType;
extern CScanTracSystemRunningData *vGlobalRunningData;
CDiagnosticLineDialog::CDiagnosticLineDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CDiagnosticLineDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDiagnosticLineDialog)
	//}}AFX_DATA_INIT
	vEjectorToChange = 1;
	vHalfSecondCount = 0;
	vOldCurrentConveyorOutput = 0;
	vMakeExitButtonGreen = false;
	vLeftSide = (WORD) ResizeXCoor(118);
	vRightSide = (WORD) ResizeXCoor(517);
	vTop = (WORD) ResizeYCoor(75); //46;
	vBottom = (WORD) ResizeYCoor(356);
	vHeight = vBottom - vTop;
	vWidth = vRightSide - vLeftSide;
	vTraceHeight = (WORD) ResizeYCoor(58);
	vTriggerMode = cContinuousTrigger;
	vTakingData = false;
	vTriggerArmed = false;
	vTriggerChannel = 0;
	vDisplayTimerHandle = 22;
	vClearTimerHandle = 23;
	vRefreshCount = 0;
	vOscilloscopeTimerHandle = 24;
	//vOscilloscope = false;
	vOldEntry =3;
	//vLogDiagnosticLines = false;
	vOldXRayOnNotice = false;
	vSampleSpeed = 1;
	vSampleSpeedDelayMagnitude = 0;
	vTimeIntervalFor50SamplesInMicroSeconds = 0;
	vLVDSActive = 0;
	vCamLinkActive = 0;
	vOldLVDSActive = 0;
	vOldCamLinkActive = 0;
	vNumberOfDataPoints = ((int)(ResizeXCoor(400) / 100)) * 100;

	/* old digital line selection order
	DiagnosticLineIndexArray[1] = 1;	//1 red lamp filament broken, x-ray source	
	DiagnosticLineIndexArray[2] = 2;	//2 yellow lamp filament broken, Alarm		
	DiagnosticLineIndexArray[3] = 3;	//3 blue lamp filament broken, eject		
	DiagnosticLineIndexArray[4] = 4;	//4 xray source on/off (0/1)
	DiagnosticLineIndexArray[5] = 5;	//5 high voltage power supply fault
	DiagnosticLineIndexArray[6] = 6;	//6 high voltage power supply interlock
	DiagnosticLineIndexArray[7] = 9;	//9 key switch
	DiagnosticLineIndexArray[8] = 10;	//10 e-stop switch
	DiagnosticLineIndexArray[9] = 11;	//11 coolant flow
	DiagnosticLineIndexArray[10] = 12;	//12 Interlock 1 - Side Shield			/
	DiagnosticLineIndexArray[11] = 13;	//13 Interlock 2 - Front or Rear Door	/
	for (BYTE TempLoop = 0; TempLoop < 8; TempLoop++)	
			DiagnosticLineIndexArray[TempLoop + 12] = 17 + TempLoop;
		//17 aux. Detector 1: Missing Cap			/
		//18 Unused Aux. Detector					/
			//19 aux 3 - overwritten in old style	/
		//20 Aux. Detector 4: Ext Det 4-Disabled	/
		//21 Aux. Detector 5: Ext Det 5-Disabled	/
		//22 Unused Eject Confirmer					/
		//23 Unused Eject Confirmer					/
		//24 Unused Eject Confirmer					/

	for (TempLoop = 0; TempLoop < 5; TempLoop++)
			DiagnosticLineIndexArray[TempLoop + 20] = 33 + TempLoop;
		//33 Container Trigger 1: Trigger 1				/
		//34 Container Trigger 2: Trigger 2-Disabled	/
		//35 Container Trigger 3: C Trigger 3-Disabled	/
		//36 Container Trigger 4: Trigger 4-Disabled	/
		//37 Container Trigger 5: Trigger 5-Disabled	/
		
	DiagnosticLineIndexArray[14] = 14;
		//14 Opto Module 1, Input 4

	DiagnosticLineIndexArray[25] = 49;	//49 Conveyor On/Off (1/0)	/
	DiagnosticLineIndexArray[26] = 50;	//50 Ejector 1:Ejector 1	/
	DiagnosticLineIndexArray[27] = 51;	//51 Ejector 2:Ejector 2	/
	DiagnosticLineIndexArray[28] = 52;	//52 Ejector 3:Ejector 3	/
	DiagnosticLineIndexArray[29] = 53;	//53 X of Y Alarm 1			/
	DiagnosticLineIndexArray[30] = 54;	//54 X of Y Alarm 2			/
	DiagnosticLineIndexArray[31] = 55;	//55 Critical Alarm			/
	DiagnosticLineIndexArray[32] = 56;	//56 Can Stop 1			/
	DiagnosticLineIndexArray[33] = 57;	//57 Can Stop 2			/
	DiagnosticLineIndexArray[34] = 58;	//58 Ejectors Enabled	/
	DiagnosticLineIndexArray[35] = 59;	//59 Red Light Output		/
	DiagnosticLineIndexArray[36] = 60;	//60 Yellow Light Output	/
	DiagnosticLineIndexArray[37] = 61;	//61 Blue Light Output		/
	DiagnosticLineIndexArray[38] = 62;	//62 Spellman X-Ray HV Enable Relay		/
	DiagnosticLineIndexArray[39] = 63;	//63 Spellman X-Ray HV Latch On Relay	/
	DiagnosticLineIndexArray[40] = 64;	//64 XR-50 HV On						/
	DiagnosticLineIndexArray[41] = 65;	//65 Frame Enable (From FPGA)	/
	DiagnosticLineIndexArray[42] = 66;	//66 Line Enable				/
	DiagnosticLineIndexArray[43] = 67;	//67 Conveyor Encoder			/
	DiagnosticLineIndexArray[44] = 68;	//68 Frame Start (From uC)		/
	DiagnosticLineIndexArray[45] = 69;	//69 Frame Enable B				/
	DiagnosticLineIndexArray[46] = 70;	//70 Frame Enable A				/
	DiagnosticLineIndexArray[47] = 71;	//71 Radiation Shutter Output	/
	DiagnosticLineIndexArray[48] = 72;	//72 System Inspecting Output	/
	DiagnosticLineIndexArray[49] = 73;	//73 Spare 1
	DiagnosticLineIndexArray[50] = 74;	//74 Radiation Shutter Container Trigger Input
	*/

	//new digital line selection order
	DiagnosticLineIndexArray[1] = 33;	//33 Container Trigger 1: Trigger 1
	DiagnosticLineIndexArray[2] = 34;	//34 Container Trigger 2: Trigger 2-Disabled
	DiagnosticLineIndexArray[3] = 65;	//65 Frame Enable (From FPGA)
	DiagnosticLineIndexArray[4] = 68;	//68 Frame Start (From uC)
	DiagnosticLineIndexArray[5] = 66;	//66 Line Enable
	DiagnosticLineIndexArray[6] = 67;	//67 Conveyor Encoder
	DiagnosticLineIndexArray[7] = 17;	//17 aux. Detector 1: Missing Cap
	DiagnosticLineIndexArray[8] = 22;	//22 Unused Eject Confirmer	- 1
	DiagnosticLineIndexArray[9] = 58;	//58 Ejectors Enabled
	DiagnosticLineIndexArray[10] = 50;	//50 Ejector 1:Ejector 1
	DiagnosticLineIndexArray[11] = 51;	//51 Ejector 2:Ejector 2
	DiagnosticLineIndexArray[12] = 52;	//52 Ejector 3:Ejector 3
	DiagnosticLineIndexArray[13] = 53;	//53 X of Y Alarm 1
	DiagnosticLineIndexArray[14] = 54;	//54 X of Y Alarm 2
	DiagnosticLineIndexArray[15] = 55;	//55 Critical Alarm
	DiagnosticLineIndexArray[16] = 72;	//72 System Inspecting Output
	DiagnosticLineIndexArray[17] = 49;	//49 Conveyor On/Off (1/0)
	DiagnosticLineIndexArray[18] = 71;	//71 Radiation Shutter Output
	DiagnosticLineIndexArray[19] = 56;	//56 Can Stop 1
	DiagnosticLineIndexArray[20] = 57;	//57 Can Stop 2
	DiagnosticLineIndexArray[21] = 1;	//1 red lamp filament broken, x-ray source	
	DiagnosticLineIndexArray[22] = 2;	//2 yellow lamp filament broken, Alarm		
	DiagnosticLineIndexArray[23] = 3;	//3 blue lamp filament broken, eject		
	DiagnosticLineIndexArray[24] = 4;	//4 xray source on/off (0/1)
	DiagnosticLineIndexArray[25] = 5;	//5 high voltage power supply fault
	DiagnosticLineIndexArray[26] = 6;	//6 high voltage power supply interlock
	DiagnosticLineIndexArray[27] = 9;	//9 key switch
	DiagnosticLineIndexArray[28] = 10;	//10 e-stop switch
	DiagnosticLineIndexArray[29] = 11;	//11 coolant flow
	DiagnosticLineIndexArray[30] = 12;	//12 Interlock 1 - Side Shield
	DiagnosticLineIndexArray[31] = 13;	//13 Interlock 2 - Front or Rear Door
	DiagnosticLineIndexArray[32] = 59;	//59 Red Light Output
	DiagnosticLineIndexArray[33] = 60;	//60 Yellow Light Output
	DiagnosticLineIndexArray[34] = 61;	//61 Blue Light Output
	DiagnosticLineIndexArray[35] = 62;	//62 Spellman X-Ray HV Enable Relay
	DiagnosticLineIndexArray[36] = 63;	//63 Spellman X-Ray HV Latch On Relay
	DiagnosticLineIndexArray[37] = 64;	//64 XR-50 HV On
	DiagnosticLineIndexArray[38] = 70;	//70 Frame Enable A
	DiagnosticLineIndexArray[39] = 69;	//69 Frame Enable B
	DiagnosticLineIndexArray[40] = 35;	//35 Container Trigger 3: C Trigger 3-Disabled
	DiagnosticLineIndexArray[41] = 36;	//36 Container Trigger 4: Trigger 4-Disabled
	DiagnosticLineIndexArray[42] = 37;	//37 Container Trigger 5: Trigger 5-Disabled
	DiagnosticLineIndexArray[43] = 18;	//18 Aux. Detector 2: Ext Det 2-Disabled
	DiagnosticLineIndexArray[44] = 19;	//19 Aux. Detector 3: Ext Det 3-Disabled
	DiagnosticLineIndexArray[45] = 20;	//20 Aux. Detector 4: Ext Det 4-Disabled
	DiagnosticLineIndexArray[46] = 21;	//21 Aux. Detector 5: Ext Det 5-Disabled
	DiagnosticLineIndexArray[47] = 23;	//23 Unused Eject Confirmer - 2
	DiagnosticLineIndexArray[48] = 24;	//24 Unused Eject Confirmer - 3
	DiagnosticLineIndexArray[49] = 73;	//73 Spare 1
	DiagnosticLineIndexArray[50] = 74;	//74 Radiation Shutter Container Trigger Input
	DiagnosticLineIndexArray[51] = 101;	//LVDS PCLK
	DiagnosticLineIndexArray[52] = 102;	//CamLink Board Clock

	vBlackPen.CreatePen(PS_SOLID, 1, RGB(0,0,0));
	vRedPen.CreatePen(PS_SOLID, 1, cRed);
	vBluePen.CreatePen(PS_SOLID, 1, cBlue);
	vDarkBluePen.CreatePen(PS_SOLID, 1, cDarkBlue);

	vLocalCWndCollection.Add(this);
	vLocalCWndCollection.Add(&m_LVDS);
	vLocalCWndCollection.Add(&m_CamLink);
	vLocalCWndCollection.Add(&m_RefreshCountLabel);
	vLocalCWndCollection.Add(&m_DeltaCountLabel);
	vLocalCWndCollection.Add(&m_ZoomMagnitude);
	vLocalCWndCollection.Add(&m_Function1Button);
	vLocalCWndCollection.Add(&m_SubFunction6Button);
	vLocalCWndCollection.Add(&m_SubFunction5Button);
	vLocalCWndCollection.Add(&m_SubFunction7Button);
	vLocalCWndCollection.Add(&m_TriggerChannel);
	vLocalCWndCollection.Add(&m_TriggerMode);
	vLocalCWndCollection.Add(&m_SubFunction8Button);
	vLocalCWndCollection.Add(&m_SubFunction4Button);
	vLocalCWndCollection.Add(&m_SubFunction3Button);
	vLocalCWndCollection.Add(&m_SubFunction2Button);
	vLocalCWndCollection.Add(&m_Function5Button);
	vLocalCWndCollection.Add(&m_Function4Button);
	vLocalCWndCollection.Add(&m_Function3Button);
	vLocalCWndCollection.Add(&m_XRaysOn);
	vLocalCWndCollection.Add(&m_DeltaCount);
	vLocalCWndCollection.Add(&m_DialogTitleStaticText);
	vLocalCWndCollection.Add(&m_Function2Button);
	vLocalCWndCollection.Add(&m_LineLabel1);
	vLocalCWndCollection.Add(&m_LineLabel2);
	vLocalCWndCollection.Add(&m_LineLabel3);
	vLocalCWndCollection.Add(&m_LineLabel4);
	vLocalCWndCollection.Add(&m_LineLabel5);
	vLocalCWndCollection.Add(&m_LineLabel6);
	vLocalCWndCollection.Add(&m_RefreshCount);
	vLocalCWndCollection.Add(&m_SubFunction1Button);
	vLocalCWndCollection.Add(&m_SubFunction0Button);
}

void CDiagnosticLineDialog::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	// TODO: Add your message handler code here

	vMainWindowPointer->SetFirstTraceOfOscilloscopeToContainerTrigger();
	//12/4/2009
	vLocalSystemData->vNextDiagnosticArrayEntry = 0;

	vFunction5ButtonEnable = true;
	vSubFunction2ButtonEnable = true;
	vSubFunction3ButtonEnable = true;
	vSubFunction4ButtonEnable = true;
	vSubFunction5ButtonEnable = true;		// currently not used in code, only set true
	vOldDiagnosticLineValue = 0;
	vOldXRayOnNotice = !vLocalSystemData->vXRayOnNotice;
	UpdateDisplay();
	UpdateSignalName();
	DisplayEjectorOnOffButtons();
	if (PasswordOK(cTemporaryInspxPassword,false))
	{
		DisplayEjectorOnOffButtons();
		m_SubFunction6Button.ShowWindow(SW_SHOW);
	}
	
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

	SetDlgItemText(IDC_DeltaCountLabel, _T(".5 Second Screen Refresh, 50 Samples"));

	int TimerResult = SetTimer(vOscilloscopeTimerHandle,500,NULL);
	if (!TimerResult)
		ReportErrorMessage("Error-OScope Timer Failed",cEMailInspx, 32000);

	TimerResult = SetTimer(vLocalSystemData->vDiagnosticDataReadTimerHandle,1,NULL);
	vLocalSystemData->vCaptureDiagnosticData = true;
	if (!TimerResult)
		ReportErrorMessage("Error-Data Timer Failed",cEMailInspx, 32000);

	vOldEjectorOnStatus = vLocalSystemData->vCurrentEjectorOnStatus;

	if (vGlobalFrameGrabberType == cPC2CamLinkFrameGrabber)
	{
		m_LVDS.ShowWindow(SW_SHOW);
		m_CamLink.ShowWindow(SW_SHOW);
	}

	TimerResult = SetTimer(vDisplayTimerHandle,500,NULL);
	if (!TimerResult)
		ReportErrorMessage("Error-Update Display Timer Failed",cEMailInspx, 32000);
	TimerResult = SetTimer(vClearTimerHandle,1000,NULL);
	this->SetWindowText(_T("OScope"));
	DisplayEjectorOnOffButtons();
	::SetupMenu(vLocalCWndCollection);
}

void CDiagnosticLineDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDiagnosticLineDialog)
	DDX_Control(pDX, IDC_LVDS, m_LVDS);
	DDX_Control(pDX, IDC_CamLink, m_CamLink);
	DDX_Control(pDX, IDC_RefreshCountLabel, m_RefreshCountLabel);
	DDX_Control(pDX, IDC_DeltaCountLabel, m_DeltaCountLabel);
	DDX_Control(pDX, IDC_ZoomMagnitude, m_ZoomMagnitude);
	DDX_Control(pDX, IDC_Function1Button, m_Function1Button);
	DDX_Control(pDX, IDC_SubFunction6Button, m_SubFunction6Button);
	DDX_Control(pDX, IDC_SubFunction5Button, m_SubFunction5Button);
	DDX_Control(pDX, IDC_SubFunction7Button, m_SubFunction7Button);
	DDX_Control(pDX, IDC_TriggerChannel, m_TriggerChannel);
	DDX_Control(pDX, IDC_TriggerMode, m_TriggerMode);
	DDX_Control(pDX, IDC_SubFunction8Button, m_SubFunction8Button);
	DDX_Control(pDX, IDC_SubFunction4Button, m_SubFunction4Button);
	DDX_Control(pDX, IDC_SubFunction3Button, m_SubFunction3Button);
	DDX_Control(pDX, IDC_SubFunction2Button, m_SubFunction2Button);
	DDX_Control(pDX, IDC_SubFunction1Button, m_SubFunction1Button);
	DDX_Control(pDX, IDC_Function5Button, m_Function5Button);
	DDX_Control(pDX, IDC_Function4Button, m_Function4Button);
	DDX_Control(pDX, IDC_Function3Button, m_Function3Button);
	DDX_Control(pDX, IDC_Function2Button, m_Function2Button);
	DDX_Control(pDX, IDC_XRaysOn, m_XRaysOn);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_DeltaCount, m_DeltaCount);
	DDX_Control(pDX, IDC_DialogTitleStaticText1, m_DialogTitleStaticText);
	DDX_Control(pDX, IDC_Function2Button, m_Function2Button);
	DDX_Control(pDX, IDC_LineLabel1, m_LineLabel1);
	DDX_Control(pDX, IDC_LineLabel2, m_LineLabel2);
	DDX_Control(pDX, IDC_LineLabel3, m_LineLabel3);
	DDX_Control(pDX, IDC_LineLabel4, m_LineLabel4);
	DDX_Control(pDX, IDC_LineLabel5, m_LineLabel5);
	DDX_Control(pDX, IDC_LineLabel6, m_LineLabel6);
	DDX_Control(pDX, IDC_RefreshCount, m_RefreshCount);
	DDX_Control(pDX, IDC_SubFunction1Button, m_SubFunction1Button);
	DDX_Control(pDX, IDC_SubFunction0Display, m_SubFunction0Button);
}


BEGIN_MESSAGE_MAP(CDiagnosticLineDialog, CDialog)
	//{{AFX_MSG_MAP(CDiagnosticLineDialog)
	ON_BN_CLICKED(IDC_Function1Button, OnFunction1Button)
	ON_BN_CLICKED(IDC_Function2Button, OnFunction2Button)
	ON_BN_CLICKED(IDC_Function3Button, OnFunction3Button)
	ON_BN_CLICKED(IDC_Function4Button, OnFunction4Button)
	ON_BN_CLICKED(IDC_Function5Button, OnFunction5Button)
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_SubFunction4Button, OnSubFunction4Button)
	ON_BN_CLICKED(IDC_SubFunction3Button, OnSubFunction3Button)
	ON_BN_CLICKED(IDC_SubFunction2Button, OnSubFunction2Button)
	ON_BN_CLICKED(IDC_SubFunction5Button, OnSubFunction5Button)
	ON_BN_CLICKED(IDC_SubFunction1Button, OnSubFunction1Button)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_SubFunction6Button, OnSubFunction6Button)
	ON_BN_CLICKED(IDC_SubFunction7Button, OnSubFunction7Button)
	ON_BN_CLICKED(IDC_SubFunction8Button, OnSubFunction8Button)
	//}}AFX_MSG_MAP
	ON_STN_CLICKED(IDC_DialogTitleStaticText1, &CDiagnosticLineDialog::OnDialogtitlestatictext1)
	ON_STN_CLICKED(IDC_SubFunction0Button, &CDiagnosticLineDialog::OnStnClickedSubfunction0button)
	ON_STN_CLICKED(IDC_SubFunction0Display, &CDiagnosticLineDialog::OnStnClickedSubfunction0display)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDiagnosticLineDialog message handlers

void CDiagnosticLineDialog::OnFunction1Button() 
{
	// Main Menu Button was pressed
	PrepareToExit();
	CDialog::EndDialog(10);
}

void CDiagnosticLineDialog::OnFunction2Button() 
{
	// Save and Exit Button was pressed
	PrepareToExit();
	CDialog::EndDialog(true);
}

void CDiagnosticLineDialog::OnFunction3Button() 
{
	//toggle can stop 1 button pressed
	if (PasswordOK(cTemporaryInspxPassword,false))
	{
		vGlobaluCSerialPort->TurnCanStopOnOff(1, (!(vLocalSystemData->vCurrentCanStopStatus & 1)));
		DisplayEjectorOnOffButtons();
	}
}

void CDiagnosticLineDialog::OnFunction4Button() 
{
	//Cancel was pressed
	//if (vLogDiagnosticLines)
	//	OnFunction5Button();
	//CDialog::OnCancel();

	//toggle can stop 2 button pressed
	if (PasswordOK(cTemporaryInspxPassword,false))
	{
		vGlobaluCSerialPort->TurnCanStopOnOff(2, (!(vLocalSystemData->vCurrentCanStopStatus & 2)));
		DisplayEjectorOnOffButtons();
	}
}

void CDiagnosticLineDialog::OnFunction5Button() 
{
	if (vTriggerMode == cContinuousTrigger)
	{
		if (vSampleSpeed != 1)	//speed is high res
		{
			vSampleSpeed = 1;
			vSampleSpeedDelayMagnitude = 0;  //set to fastest speed sample rate

			//12/4/2009
			//SetDlgItemText(IDC_Function5Button, "High Resolution");
			
			m_RefreshCountLabel.ShowWindow(SW_SHOW);

			SetDlgItemText(IDC_DeltaCountLabel, _T(".5 Second Screen Refresh, 50 Samples"));

			//12/4/2009
			/*
			if (vLocalSystemData->vSystemRunMode == cRunningSystemMode)
			{
				if (PasswordOK(cTemporaryInspxPassword, false) && (!vLocalConfigurationData->vEnableEjectors))
					vFunction5ButtonEnable = true;	//m_Function5Button.EnableWindow(true);
				else
					vFunction5ButtonEnable = false;	//m_Function5Button.EnableWindow(false);
			}
			else
				vFunction5ButtonEnable = true;	//m_Function5Button.EnableWindow(true);
				*/
		}
		else	//speed is normal resolution
		{
			//10/7/2009
			if ((vLocalSystemData->vSystemRunMode == cRunningSystemMode) || 
				(vLocalSystemData->vSystemRunMode == cCalibratingDetectorOffsetMode))
			{
				if (PasswordOK(cTemporaryInspxPassword, false))
				{
					if (vLocalConfigurationData->vEnableEjectors)
					{
						CNoticeDialog TempNoticeDialog;
						CString TempString = "\n\nSystem must not be running with the Ejectors Enabled\nduring High Resolution Mode.";
						TempNoticeDialog.vNoticeText = TempString;
						TempNoticeDialog.vType = cNoticeMessage;
						TempNoticeDialog.DoModal();
					}
					else
					{
						//12/4/2009
						//SetDlgItemText(IDC_Function5Button, "Normal Resolution");

						m_RefreshCountLabel.ShowWindow(SW_HIDE);
						SetDlgItemText(IDC_DeltaCountLabel, _T(".5 Second Screen Refresh, 50 Samples"));

						vSampleSpeed = 400;
						vSampleSpeedDelayMagnitude = 0;  //set to fastest speed sample rate
					}
				}
				else
				{
					CNoticeDialog TempNoticeDialog;
					CString TempString = "\n\n\nSystem must not be running during High Resolution Mode";
					TempNoticeDialog.vNoticeText = TempString;
					TempNoticeDialog.vType = cNoticeMessage;
					TempNoticeDialog.DoModal();
				}
			}
			else
			{
				m_RefreshCountLabel.ShowWindow(SW_HIDE);
				SetDlgItemText(IDC_DeltaCountLabel, _T(".5 Sec Screen Refresh, Scale is 50 Samples"));

				vSampleSpeed = vNumberOfDataPoints;
				vSampleSpeedDelayMagnitude = 0;  //set to fastest speed sample rate
			}
		}
		DisplayEjectorOnOffButtons();
	}
	else
	{
		CSelectItemDialog ISelectItemDialog;  
		//Set dialog box data titles and number value
		ISelectItemDialog.vTitleString = "Select Trigger Channel";
		ISelectItemDialog.m_DialogTitleStaticText2 = "";
		ISelectItemDialog.m_DialogTitleStaticText3 = "";
		ISelectItemDialog.vBackgroundColor = cYellow;
		ISelectItemDialog.vShowExit = true;

		ISelectItemDialog.vNumberOfItems = 6;

		for (BYTE TempLoop = 0; TempLoop < ISelectItemDialog.vNumberOfItems; TempLoop++)
			ISelectItemDialog.vItems[TempLoop] = dtoa(TempLoop + 1, 0);

		ISelectItemDialog.m_DialogTitleStaticText3 = "Original Value: " + dtoa(vTriggerChannel + 1, 0);

		//Pass control to dialog box and display
		int nResponse = ISelectItemDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			vTriggerChannel = ISelectItemDialog.vItemNumberSelected - 1;
			DisplayEjectorOnOffButtons();
		}
		else 
		if (nResponse == 10)
		{
			//Main Menu button pressed
			PrepareToExit();
			CDialog::EndDialog(10);
		}
	}
}

void CDiagnosticLineDialog::OnSubFunction1Button() 
{
	// Select Diagnostic Lines
	if (PasswordOK(cSetupPassword, true))
	{
		BYTE TempDiagnosticLineSelect[cNumberOfDiagnosticLines];
		BYTE TempLoop = 0;
		CString TempString;
		CSelectItemDialog ISelectItemDialog;  
		bool TempChangedDiagnosticLines = false;
		//Set dialog box data titles and number value
		ISelectItemDialog.vTitleString = "Select Diagnostic Input Line For";
		ISelectItemDialog.m_DialogTitleStaticText2 = "";
		ISelectItemDialog.m_DialogTitleStaticText3 = "";
		ISelectItemDialog.vBackgroundColor = cYellow;
		ISelectItemDialog.vShowExit = true;
		for (TempLoop = 0; TempLoop < cNumberOfDiagnosticLines; TempLoop++)
			TempDiagnosticLineSelect[TempLoop] = vLocalConfigurationData->vDiagnosticLineSelect[TempLoop];

		ISelectItemDialog.vNumberOfItems = cNumberOfDiagnosticSignals;

		for (TempLoop = 0; TempLoop < ISelectItemDialog.vNumberOfItems; TempLoop++)
		{
			ISelectItemDialog.vItems[TempLoop] = dtoa(TempLoop + 1) + ". " + GetInputName(DiagnosticLineIndexArray[TempLoop + 1]);
		}

		for (TempLoop = 1; TempLoop < cNumberOfDiagnosticLines; TempLoop++)
		{
			//if (TempLoop > 2)
			if (vGlobalFrameGrabberType == cPC2CamLinkFrameGrabber)
			{
				ISelectItemDialog.vNumberOfItems = cNumberOfDiagnosticSignals + 2;
				for (BYTE TempLoopA = cNumberOfDiagnosticSignals; TempLoopA < ISelectItemDialog.vNumberOfItems; TempLoopA++)
				{
					ISelectItemDialog.vItems[TempLoopA] = dtoa(TempLoopA + 1) + ". " + GetInputName(DiagnosticLineIndexArray[TempLoopA + 1]);
				}
			}

			ISelectItemDialog.m_DialogTitleStaticText2 = "Diagnostic Channel " + dtoa(TempLoop + 1);
			BYTE TempLineSelect = 1;
			for (BYTE TempLoop2 = 1; TempLoop2 <= ISelectItemDialog.vNumberOfItems; TempLoop2++)
			if (vLocalConfigurationData->vDiagnosticLineSelect[TempLoop] == DiagnosticLineIndexArray[TempLoop2])
			{
				TempLineSelect = TempLoop2;
			}

			ISelectItemDialog.m_DialogTitleStaticText3 = "Original Value: " + GetInputName(vLocalConfigurationData->vDiagnosticLineSelect[TempLoop]);

			//Pass control to dialog box and display
			if ((TempLoop > 2) || (PasswordOK(cTemporaryInspxPassword,false)))
			{
				int nResponse = ISelectItemDialog.DoModal();
				//dialog box is now closed, if user pressed select do this
				//if user pressed cancel, do nothing
				if (nResponse == IDOK)
				{
					TempChangedDiagnosticLines = true;
					//vLocalDiagnosticLineSelect[TempLoop] = INumericEntryDialog.vEditString;

					if (vLocalConfigurationData->vDiagnosticLineSelect[TempLoop] != DiagnosticLineIndexArray[ISelectItemDialog.vItemNumberSelected])
					{
						TempDiagnosticLineSelect[TempLoop] = DiagnosticLineIndexArray[ISelectItemDialog.vItemNumberSelected];
					}
				}
				else 
				if (nResponse == 11)
				{
					TempChangedDiagnosticLines = true;
				}
				else
				if (nResponse == 10)
				{
					//Main Menu button pressed
					TempChangedDiagnosticLines = false;
					TempLoop = cNumberOfDiagnosticLines;
					PrepareToExit();
					CDialog::EndDialog(10);
				}
				else
				if (nResponse == IDCANCEL)
				{
					//Cancel button pressed
					TempLoop = cNumberOfDiagnosticLines;
				}
				else 
				{
					//exit - do nothing but continue
				}
			}
			else
			{
				switch (TempLoop)
				{
					case 0:
					{
						if (vLocalConfigurationData->vDiagnosticLineSelect[TempLoop] != 33)
						{
							TempDiagnosticLineSelect[TempLoop] = 33;
							TempChangedDiagnosticLines = true;
						}
					}
					break;
					case 1:
					{
						if (vLocalConfigurationData->vDiagnosticLineSelect[TempLoop] != 65)
						{
							TempDiagnosticLineSelect[TempLoop] = 65;
							TempChangedDiagnosticLines = true;
						}
					}
					break;
					case 2:
					{
						if (vLocalConfigurationData->vDiagnosticLineSelect[TempLoop] != 66)
						{
							TempDiagnosticLineSelect[TempLoop] = 66;
							TempChangedDiagnosticLines = true;
						}
					}
					break;
				}
			}
		}
		if (TempChangedDiagnosticLines)
		{
			for (BYTE TempLoop = 0; TempLoop < cNumberOfDiagnosticLines; TempLoop++)
				vLocalConfigurationData->vDiagnosticLineSelect[TempLoop] = TempDiagnosticLineSelect[TempLoop];
			vMainWindowPointer->SendDiagnosticLineSelect();
//			DisplayLineNumbers();
			UpdateDisplay();
			UpdateSignalName();
		}
		UpdateSignalName();
	}
	CWnd *TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText);
	if (TempWindow)
		TempWindow->Invalidate(false);
}

void CDiagnosticLineDialog::OnSubFunction2Button() 
{
	if (PasswordOK(cTemporaryInspxPassword,false))
	{
		if (!vLocalConfigurationData->vEnableEjectors)		
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\n\nEjectors are Disabled.";
			TempNoticeDialog.vType = cNoticeMessage;
			TempNoticeDialog.DoModal();
		}
		else
		{
			//vLocalSystemData->vCurrentEjectorOnStatus = vLocalSystemData->vCurrentEjectorOnStatus ^ 1;  //exclusive or to toggle bit
			vGlobaluCSerialPort->SendEjectorOnOffCommand(1, (!(vLocalSystemData->vCurrentEjectorOnStatus & 1)));
			DisplayEjectorOnOffButtons();
		}
	}
}

void CDiagnosticLineDialog::OnSubFunction3Button() 
{
	if (PasswordOK(cTemporaryInspxPassword,false) && (vSampleSpeed == 1))
	{
		if (!vLocalConfigurationData->vEnableEjectors)		
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\n\nEjectors are Disabled.";
			TempNoticeDialog.vType = cNoticeMessage;
			TempNoticeDialog.DoModal();
		}
		else
		{
			//vLocalSystemData->vCurrentEjectorOnStatus = vLocalSystemData->vCurrentEjectorOnStatus ^ 2;  //exclusive or to toggle bit
			vGlobaluCSerialPort->SendEjectorOnOffCommand(2, (!(vLocalSystemData->vCurrentEjectorOnStatus & 2)));
			DisplayEjectorOnOffButtons();
		}
	}
	else
	{
		if (vSampleSpeedDelayMagnitude < 11)
		{
			vSampleSpeedDelayMagnitude++;
			SetDlgItemText(IDC_ZoomMagnitude,("Zoom: " + dtoa(12 - vSampleSpeedDelayMagnitude) + "x"));
			vSubFunction4ButtonEnable = true;	//m_SubFunction4Button.EnableWindow(true);
			if (vSampleSpeedDelayMagnitude == 11)
				vSubFunction3ButtonEnable = false;	//m_SubFunction3Button.EnableWindow(false);
		}
		else
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\n\nAll the way Zoomed Out";
			TempNoticeDialog.vType = cNoticeMessage;
			TempNoticeDialog.DoModal();
		}
	}
}

void CDiagnosticLineDialog::OnSubFunction4Button() 
{
	if (PasswordOK(cTemporaryInspxPassword,false) && (vSampleSpeed == 1))
	{
		if (!vLocalConfigurationData->vEnableEjectors)		
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\n\nEjectors are Disabled.";
			TempNoticeDialog.vType = cNoticeMessage;
			TempNoticeDialog.DoModal();
		}
		else
		{
			//vLocalSystemData->vCurrentEjectorOnStatus = vLocalSystemData->vCurrentEjectorOnStatus ^ 4;  //exclusive or to toggle bit
			vGlobaluCSerialPort->SendEjectorOnOffCommand(3, (!(vLocalSystemData->vCurrentEjectorOnStatus & 4)));
			DisplayEjectorOnOffButtons();
		}
	}
	else
	{
		if (vSampleSpeedDelayMagnitude > 0)
		{
			vSampleSpeedDelayMagnitude--;
			vSubFunction3ButtonEnable = true;	//m_SubFunction3Button.EnableWindow(true);
			SetDlgItemText(IDC_ZoomMagnitude,("Zoom: " + dtoa(12 - vSampleSpeedDelayMagnitude) + "x"));
			
			if (vSampleSpeedDelayMagnitude == 0)
				vSubFunction4ButtonEnable = false;	//m_SubFunction4Button.EnableWindow(false);
		}
		else
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\n\nAll the way Zoomed In";
			TempNoticeDialog.vType = cNoticeMessage;
			TempNoticeDialog.DoModal();
		}
	}
}

void CDiagnosticLineDialog::DisplayEjectorOnOffButtons()
{
	CString TempString = " ";
	if (!PasswordOK(cTemporaryInspxPassword,false))
	{
		GetDlgItemText(IDC_Function5Button, TempString);
		if (TempString != "Trigger Channel")
			SetDlgItemText(IDC_Function5Button, _T("Trigger Channel"));
	}
	SetDlgItemText(IDC_TriggerChannel,dtoa(vTriggerChannel + 1));

	switch (vTriggerMode)
	{
		case cContinuousTrigger:
			GetDlgItemText(IDC_TriggerMode, TempString);
			if (TempString != "Continuous")
				SetDlgItemText(IDC_TriggerMode, _T("Continuous"));
			m_TriggerChannel.ShowWindow(SW_HIDE);

			if (vSampleSpeed == 1)	//speed is normal
			{
				GetDlgItemText(IDC_Function5Button, TempString);
				if (TempString != "High\nResolution")
					SetDlgItemText(IDC_Function5Button, _T("High\nResolution"));
				if ((vLocalSystemData->vSystemRunMode == cRunningSystemMode) || 
					(vLocalSystemData->vSystemRunMode == cCalibratingDetectorOffsetMode))
				{
					if (PasswordOK(cTemporaryInspxPassword, false) && (!vLocalConfigurationData->vEnableEjectors))
						vFunction5ButtonEnable = true;	//m_Function5Button.EnableWindow(true);
					else
						vFunction5ButtonEnable = false;	//m_Function5Button.EnableWindow(false);
				}
				else
					vFunction5ButtonEnable = true;	//m_Function5Button.EnableWindow(true);
			}
			else	//already in high speed
			{
				GetDlgItemText(IDC_Function5Button, TempString);
				//12/4/2009
				vFunction5ButtonEnable = true;	//m_Function5Button.EnableWindow(true);
				if (TempString != "Normal Resolution")
					SetDlgItemText(IDC_Function5Button, _T("Normal Resolution"));
			}

			m_SubFunction7Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_SubFunction7Button.GetRectToInvalidateArrow());
		break;
		case cTriggerOnChannel:
			GetDlgItemText(IDC_TriggerMode, TempString);
			if (TempString != "Trigger")
				SetDlgItemText(IDC_TriggerMode, _T("Trigger"));
			GetDlgItemText(IDC_Function5Button, TempString);
			if (TempString != "Trigger Channel")
				SetDlgItemText(IDC_Function5Button, _T("Trigger Channel"));
			vSubFunction5ButtonEnable = true;	//m_SubFunction5Button.EnableWindow(true);
			m_TriggerChannel.ShowWindow(SW_SHOW);
			m_Function5Button.ShowWindow(SW_SHOW);
			//12/4/2009
			vFunction5ButtonEnable = true;	//m_Function5Button.EnableWindow(true);
			if (!PasswordOK(cTemporaryInspxPassword,false))
			{
				m_SubFunction7Button.ShowWindow(SW_HIDE);
				this->InvalidateRect(&m_SubFunction7Button.GetRectToInvalidateArrow());
			}
		break;
		case cSingleTriggerOnChannel:
			GetDlgItemText(IDC_TriggerMode, TempString);
			if (TempString != "Single")
				SetDlgItemText(IDC_TriggerMode, _T("Single"));
			SetDlgItemText(IDC_Function5Button, _T("Trigger Channel"));
			vSubFunction5ButtonEnable = true;	//m_SubFunction5Button.EnableWindow(true);
			m_TriggerChannel.ShowWindow(SW_SHOW);
			m_Function5Button.ShowWindow(SW_SHOW);
			vFunction5ButtonEnable = true;	//m_Function5Button.EnableWindow(true);
			m_SubFunction7Button.ShowWindow(SW_SHOW);
			if (vTriggerArmed)
			{
				GetDlgItemText(IDC_SubFunction7Button, TempString);
				if (TempString != "Clear\nArmed\nTrigger")
					SetDlgItemText(IDC_SubFunction7Button,_T("Clear\nArmed\nTrigger"));
			}
			else
			{
				GetDlgItemText(IDC_SubFunction7Button, TempString);
				if (TempString != "Arm\nSingle\nTrigger")
					SetDlgItemText(IDC_SubFunction7Button, _T("Arm\nSingle\nTrigger"));
			}
		break;
	}

	if (PasswordOK(cTemporaryInspxPassword,false))
	{
		if (vTriggerMode != cSingleTriggerOnChannel)
		{
			m_SubFunction7Button.ShowWindow(SW_SHOW);
			if (vLocalSystemData->vCurrentConveyorOutput & 2)
			{
				GetDlgItemText(IDC_SubFunction7Button, TempString);
				if (TempString != "Stop\nConveyor\nNow!")
					SetDlgItemText(IDC_SubFunction7Button, _T("Stop\nConveyor\nNow!"));
			}
			else
			{
				GetDlgItemText(IDC_SubFunction7Button, TempString);
				if (TempString != "Start\nConveyor\nNow!")
					SetDlgItemText(IDC_SubFunction7Button, _T("Start\nConveyor\nNow!"));
			}
		}

		m_Function3Button.ShowWindow(SW_SHOW);
		m_Function4Button.ShowWindow(SW_SHOW);
		m_SubFunction0Button.ShowWindow(SW_SHOW);
		m_SubFunction2Button.ShowWindow(SW_SHOW);
		m_SubFunction3Button.ShowWindow(SW_SHOW);
		m_SubFunction4Button.ShowWindow(SW_SHOW);
		m_SubFunction5Button.ShowWindow(SW_SHOW);

		if (vLocalSystemData->vCurrentEjectorOnStatus & 1)
		{
			GetDlgItemText(IDC_SubFunction2Button, TempString);
			if (TempString != "Ejector\n1\nOff")
				SetDlgItemText(IDC_SubFunction2Button, _T("Turn Ejector 1 Off"));
		}
		else
		{
			GetDlgItemText(IDC_SubFunction2Button, TempString);
			if (TempString != "Ejector\n1\nOn")
				SetDlgItemText(IDC_SubFunction2Button, _T("Turn\nEjector\n1 On"));
		}

		if (vLocalConfigurationData->vEnableEjectors)	
			vSubFunction2ButtonEnable = true;	//m_SubFunction2Button.EnableWindow(true);
		else
			vSubFunction2ButtonEnable = false;	//m_SubFunction2Button.EnableWindow(false);
		

		if (vSampleSpeed == 1)
		{
			m_SubFunction3Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
			m_SubFunction4Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
			m_ZoomMagnitude.ShowWindow(SW_HIDE);

			if (vLocalSystemData->vCurrentEjectorOnStatus & 2)
			{	
				GetDlgItemText(IDC_SubFunction3Button, TempString);
				if (TempString != "Ejector\n2\nOff")
					SetDlgItemText(IDC_SubFunction3Button, _T("Ejector\n2\nOff"));
			}
			else
			{
				GetDlgItemText(IDC_SubFunction3Button, TempString);
				if (TempString != "Ejector\n2\nOn")
					SetDlgItemText(IDC_SubFunction3Button, _T("Ejector\n2\nOn"));
			}

			if (vLocalSystemData->vCurrentEjectorOnStatus & 4)
			{
				GetDlgItemText(IDC_SubFunction4Button, TempString);
				if (TempString != "Ejector\n3\nOff")
					SetDlgItemText(IDC_SubFunction4Button, _T("Ejector\n3\nOff"));
			}
			else
			{
				GetDlgItemText(IDC_SubFunction4Button, TempString);
				if (TempString != "Ejector\n3\nOn")
					SetDlgItemText(IDC_SubFunction4Button, _T("Ejector\n3\nOn"));
			}

			if (vLocalConfigurationData->vEnableEjectors)	
			{
				vSubFunction3ButtonEnable = true;	//m_SubFunction3Button.EnableWindow(true);
				vSubFunction4ButtonEnable = true;	//m_SubFunction4Button.EnableWindow(true);
			}
			else
			{
				vSubFunction3ButtonEnable = false;	//m_SubFunction3Button.EnableWindow(false);
				vSubFunction4ButtonEnable = false;	//m_SubFunction4Button.EnableWindow(false);
			}
		}
		else
		{
			m_SubFunction3Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
			m_SubFunction4Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
			m_ZoomMagnitude.ShowWindow(SW_SHOW);

			GetDlgItemText(IDC_ZoomMagnitude, TempString);
			if (TempString != ("Zoom: " + dtoa(12-vSampleSpeedDelayMagnitude)))
				SetDlgItemText(IDC_ZoomMagnitude,("Zoom: " + dtoa(12-vSampleSpeedDelayMagnitude) + "x"));
			
			GetDlgItemText(IDC_SubFunction3Button, TempString);
			if (TempString != "Zoom Out")
				SetDlgItemText(IDC_SubFunction3Button, _T("Zoom Out"));

			if (vSampleSpeedDelayMagnitude < 1 )
				vSubFunction4ButtonEnable = false;	//m_SubFunction4Button.EnableWindow(false);
			else
				vSubFunction4ButtonEnable = true;	//m_SubFunction4Button.EnableWindow(true);
			
			GetDlgItemText(IDC_SubFunction4Button, TempString);
			if (TempString != "Zoom In")
				SetDlgItemText(IDC_SubFunction4Button, _T("Zoom In"));

			if (vSampleSpeedDelayMagnitude == 11)
				vSubFunction3ButtonEnable = false;	//m_SubFunction3Button.EnableWindow(false);
			else
				vSubFunction3ButtonEnable = true;	//m_SubFunction3Button.EnableWindow(true);
		}
		m_SubFunction3Button.Invalidate(false);
		m_SubFunction4Button.Invalidate(false);

		if (vLocalSystemData->vCurrentCanStopStatus & 1)
		{
			GetDlgItemText(IDC_Function3Button, TempString);
			if (TempString != "Can\nStop 1 Off")
				SetDlgItemText(IDC_Function3Button, _T("Can\nStop 1 Off"));
		}
		else
		{
			GetDlgItemText(IDC_Function3Button, TempString);
			if (TempString != "Can\nStop 1 On")
				SetDlgItemText(IDC_Function3Button, _T("Can\nStop 1 On"));
		}

		if (vLocalSystemData->vCurrentCanStopStatus & 2)
		{
			GetDlgItemText(IDC_Function4Button, TempString);
			if (TempString != "Can\nStop 2 Off")
				SetDlgItemText(IDC_Function4Button, _T("Can\nStop 2 Off"));
		}
		else
		{
			GetDlgItemText(IDC_Function4Button, TempString);
			if (TempString != "Can\nStop 2 On")
				SetDlgItemText(IDC_Function4Button, _T("Can\nStop 2 On"));
		}

		GetDlgItemText(IDC_SubFunction5Button, TempString);
		if (TempString != "Test\nAlarm\nOutput")
			SetDlgItemText(IDC_SubFunction5Button, _T("Test\nAlarm\nOutput"));
	}
	else
	{
		if (vSampleSpeed == 1)
		{
			m_SubFunction3Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_SubFunction3Button.GetRectToInvalidateArrow());
			m_SubFunction4Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_SubFunction4Button.GetRectToInvalidateArrow());
			m_ZoomMagnitude.ShowWindow(SW_HIDE);
		}
		else
		{
			m_SubFunction3Button.ShowWindow(SW_SHOW);
			m_SubFunction4Button.ShowWindow(SW_SHOW);
			m_SubFunction3Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
			m_SubFunction4Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
			m_ZoomMagnitude.ShowWindow(SW_SHOW);

			GetDlgItemText(IDC_ZoomMagnitude, TempString);
			if (TempString != ("Zoom: " + dtoa(12-vSampleSpeedDelayMagnitude)))
				SetDlgItemText(IDC_ZoomMagnitude,("Zoom: " + dtoa(12-vSampleSpeedDelayMagnitude) + "x"));
			
			GetDlgItemText(IDC_SubFunction3Button, TempString);
			if (TempString != "Zoom Out")
				SetDlgItemText(IDC_SubFunction3Button, _T("Zoom Out"));

			if (vSampleSpeedDelayMagnitude < 1 )
				vSubFunction4ButtonEnable = false;	//m_SubFunction4Button.EnableWindow(false);
			else
				vSubFunction4ButtonEnable = true;	//m_SubFunction4Button.EnableWindow(true);
			
			GetDlgItemText(IDC_SubFunction4Button, TempString);
			if (TempString != "Zoom In")
				SetDlgItemText(IDC_SubFunction4Button, _T("Zoom In"));

			if (vSampleSpeedDelayMagnitude == 11)
				vSubFunction3ButtonEnable = false;	//m_SubFunction3Button.EnableWindow(false);
			else
				vSubFunction3ButtonEnable = true;	//m_SubFunction3Button.EnableWindow(true);
		}
	}
}

void CDiagnosticLineDialog::OnSubFunction5Button() 
{
	if (PasswordOK(cTemporaryInspxPassword,false))
	{
		CString TempString;
		CSelectItemDialog ISelectItemDialog;  
		//Set dialog box data titles and number value
		ISelectItemDialog.vTitleString = "Select Which Alarm To Test";
		ISelectItemDialog.m_DialogTitleStaticText2 = "For Testing Only";
		ISelectItemDialog.m_DialogTitleStaticText3 = "";
		ISelectItemDialog.vBackgroundColor = cYellow;
		ISelectItemDialog.vShowExit = true;

		ISelectItemDialog.vNumberOfItems = 4;
		ISelectItemDialog.vItems[0] = "X of Y Rejects Alarm 1";
		ISelectItemDialog.vItems[1] = "X of Y Rejects Alarm 2";
		ISelectItemDialog.vItems[2] = "Critical Alarm";
		ISelectItemDialog.vItems[3] = "System Inspecting Output";

		//Pass control to dialog box and display
		int nResponse = ISelectItemDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			if (ISelectItemDialog.vItemNumberSelected == 1)
			{
				ReportErrorMessage("Test X of Y Rejects Alarm 1",cEMailPlantOperations,11);
				vGlobaluCSerialPort->TurnAlarmOnOff(1,true);
			}
			else
			if (ISelectItemDialog.vItemNumberSelected == 2)
			{
				ReportErrorMessage("Test X of Y Rejects Alarm 2",cEMailPlantOperations,13);
				vGlobaluCSerialPort->TurnAlarmOnOff(2,true);
			}
			else
			if (ISelectItemDialog.vItemNumberSelected == 3)
			{
				ReportErrorMessage("Test Critical Alarm",cEMailMaintenance,22);
				//ReportErrorMessage("Test Critical Alarm",cEMailInspx,22);
				vGlobaluCSerialPort->TurnAlarmOnOff(3,true);
			}
			else
			if (ISelectItemDialog.vItemNumberSelected == 4)
			{
				ReportErrorMessage("Test System Inspecting Output",cEMailMaintenance,0);
				//ReportErrorMessage("Test System Inspecting Output",cEMailInspx, 32000);
				vGlobaluCSerialPort->TurnAlarmOnOff(4,true);
			}
		}
		else 
		if (nResponse == 10)
		{
			//Main Menu button pressed
			PrepareToExit();
			CDialog::EndDialog(10);
		}
	}
}

void CDiagnosticLineDialog::OnSubFunction6Button() 
{
	//Turn on X-Rays button was pressed
	if (PasswordOK(cTemporaryInspxPassword,false))
	if (vGlobalCurrentProduct)
	if (vGlobalImageAquisitionThread)
	{
		if (vLocalSystemData->vSystemRunMode == cStoppedSystemMode)
		{
			if (vMainWindowPointer->PrepareToRun())
			{
				if (vLocalConfigurationData->vEnableEjectors)
				{
					vMainWindowPointer->OnSubFunction2Button();

					CNoticeDialog TempNoticeDialog;
					TempNoticeDialog.vNoticeText = "\n\nEjectors have been Disabled\nfor Run Diagnostic Mode.";
					TempNoticeDialog.vType = cNoticeMessage;
					TempNoticeDialog.DoModal();
				}

				vMainWindowPointer->StartRunning(true, true);
				if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)
				{
					//9/11/2009
					m_SubFunction6Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
					SetDlgItemText(IDC_SubFunction6Button, _T("Starting"));
				}
			}
		}
		else
		{
			vMainWindowPointer->StopRunning(true);
			//9/11/2009
			m_SubFunction6Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
			SetDlgItemText(IDC_SubFunction6Button, _T("Stoping"));
		}
	}
	/*
	// Show Signal Names
	bool TempNeedTwoDisplays = false;
	CString TempStringL("");
	for (BYTE TempLoop = 1; TempLoop < 7; TempLoop++)
	{
		TempStringL = GetSignalName(TempLoop);
		if (TempStringL.GetLength() > 55)
			TempNeedTwoDisplays = true;
	}

	CString TempString("");
	TempString = "";
	TempString = GetSignalName(1) + "\n";
	TempString = TempString + GetSignalName(2) + "\n";
	TempString = TempString + GetSignalName(3) + "\n";
	if (!TempNeedTwoDisplays)
	{
		TempString = TempString + GetSignalName(4) + "\n";
		TempString = TempString + GetSignalName(5) + "\n";
		TempString = TempString + GetSignalName(6);
	}
	CNoticeDialog TempNoticeDialog;
	TempNoticeDialog.vNoticeText = TempString;
	TempNoticeDialog.vType = cInformationMessage;
	TempNoticeDialog.DoModal();

	if (TempNeedTwoDisplays)
	{
		TempString = GetSignalName(4) + "\n";
		TempString = TempString + GetSignalName(5) + "\n";
		TempString = TempString + GetSignalName(6);
		TempNoticeDialog.vNoticeText = TempString;
		TempNoticeDialog.DoModal();
	}
	*/
	// Show Oscilloscope Button Pressed
	/*
	if (vOscilloscope)
	{
		SetDlgItemText(IDC_SubFunction6Button,"Show I/O Line Display!");
		KillTimer(vOscilloscopeTimerHandle);
		vOscilloscope = false;
	}
	else
	{
		SetDlgItemText(IDC_SubFunction6Button,"Stop I/O Line Display!");
		int TimerResult = SetTimer(vOscilloscopeTimerHandle,1000,NULL);
		vOscilloscope = true;
		if (!TimerResult)
			ReportErrorMessage("Error-OScope Timer Failed",cEMailInspx, 32000);
	}
	*/
}

void CDiagnosticLineDialog::OnSubFunction7Button() 
{
	if (vTriggerMode == cSingleTriggerOnChannel)
	{
		vTriggerArmed = !vTriggerArmed;
		DisplayEjectorOnOffButtons();
	}
	else
	{
		if (vLocalSystemData->vCurrentConveyorOutput & 2)
			vMainWindowPointer->SendManualConveyorOnOffToUController(0);
		else
			vMainWindowPointer->SendManualConveyorOnOffToUController(1);
		DisplayEjectorOnOffButtons();
	}
}

void CDiagnosticLineDialog::OnSubFunction8Button() 
{
	//change trigger mode
	switch (vTriggerMode)
	{
		case cContinuousTrigger:
			vTriggerMode = cTriggerOnChannel;
		break;
		case cTriggerOnChannel:
			vTriggerMode = cSingleTriggerOnChannel;
		break;
		case cSingleTriggerOnChannel:
			vTriggerMode = cContinuousTrigger;
		break;
	}
	//12/4/2009
	//vTakingData = false;
	vTriggerArmed = false;
	DisplayEjectorOnOffButtons();
}

CString CDiagnosticLineDialog::GetSignalName(BYTE TempChannel)
{
	return GetInputName(DiagnosticLineIndexArray[TempChannel]);
}

CString CDiagnosticLineDialog::GetInputName(BYTE TempLineNumber)	// TempLineNumber is the Hardware Number sent to select the signal
{
	CString TempString("Undefined");
	CString TempNameString("");
	BYTE TempLoop = 0;
	switch(TempLineNumber)
	{
		case 1:
			TempString = "Red Lamp Filament Broken, X-Ray Source";
		break;
		case 2:
			TempString = "Yellow Lamp Filament Broken, Alarm";
		break;
		case 3:
			TempString = "Blue Lamp Filament Broken, Eject";
		break;
		case 4:
			TempString = "X-Ray Source On/Off (0/1)";
		break;
		case 5:
			TempString = "High Voltage Power Supply Fault";
		break;
		case 6:
			TempString = "High Voltage Power Supply Interlock";
		break;
		case 9:  //Diag code 9 to FPGA
			TempString = "Key Switch";
		break;
		case 10:  //Diag code 10 to FPGA
			TempString = "E-Stop Switch";
		break;
		case 11:  //11
			if ((vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vLocalConfigurationData->vScanTracType == cAllegroScanTrac) ||
				(vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac))
				TempString = "Spare";  //coolant flow on side view system
			else
				TempString = "Coolant Flow";
		break;
		case 12:  //12
			if ((vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vLocalConfigurationData->vScanTracType == cAllegroScanTrac) ||
				(vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac))
				TempString = "Interlock 1 - Center Conveyor Door";
			else
			if (vLocalConfigurationData->vScanTracType == cBriosoScanTrac)
				TempString = "Interlock 1 - Tunnel Door";
			else
				TempString = "Interlock 1 - Side Shield";
		break;
		case 13:  //13
			if ((vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vLocalConfigurationData->vScanTracType == cAllegroScanTrac) ||
				(vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac))
				TempString = "Interlock 2 - Cabinet Door";
			else
			if (vLocalConfigurationData->vScanTracType == cBriosoScanTrac)
				TempString = "Interlock 1 - Front or Side Door";
			else
				TempString = "Interlock 2 - Front or Rear Door";
		break;

		case 14:
			if ((vLocalConfigurationData->vScanTracType == cLegatoScanTrac) || (vLocalConfigurationData->vScanTracType == cFermataScanTrac))//a pipeline
				TempString = "Pipe Flow Indicator Input";
			else
				TempString = "Opto Module 1, Input 4";
		break;
		case 15:
				TempString = "Unused 15";
		break;
		case 16:
				TempString = "Unused 16";
		break;
		case 17: //normally Aux Detector 1
			TempNameString = "Unused Aux. Detector " + dtoa(TempLineNumber - 16, 0);
			for (TempLoop = 0;TempLoop < cNumberOfEjectors; TempLoop++)
			if ((vLocalConfigurationData->vEjector[TempLoop].vEnabled) && (vLocalConfigurationData->vEjector[TempLoop].vConfirmDigitalInputLine == 2))
			{
				TempNameString = "Eject Confirmer " + dtoa(TempLoop + 1, 0) + " for: " + vLocalConfigurationData->vEjector[TempLoop].vName;
			}
			for (TempLoop = 0;TempLoop < cNumberOfExternalDetectors; TempLoop++)
			if (vLocalConfigurationData->vExternalDetector[TempLoop].vEnabled)
			if (vLocalConfigurationData->vExternalDetector[TempLoop].vDigitalInputLine == 1)
			{
				TempNameString = "Aux. Detector ";
				TempNameString = TempNameString + dtoa(TempLoop + 1, 0) + ": " + vLocalConfigurationData->vExternalDetector[TempLoop].vName;
			}

			TempString = TempNameString;
		break;
		case 18: //normally Aux Detector 2
			if (vLocalConfigurationData->vResynchronizeEjectors)
			{
				TempString = "Resynchronizing Sensor for Ejector 1";
			}
			else
			{
				TempNameString = "Unused Aux. Detector " + dtoa(TempLineNumber - 16, 0);
				//for (TempLoop = 0;TempLoop < cNumberOfEjectors; TempLoop++)
				//if ((vLocalConfigurationData->vEjector[TempLoop].vEnabled) && (vLocalConfigurationData->vEjector[TempLoop].vConfirmDigitalInputLine == 2))
				//{
				//	TempNameString = "Eject Confirmer " + dtoa(TempLoop + 2, 0) + " for: " + vLocalConfigurationData->vEjector[TempLoop].vName;
				//}
				for (TempLoop = 0;TempLoop < cNumberOfExternalDetectors; TempLoop++)
				if (vLocalConfigurationData->vExternalDetector[TempLoop].vEnabled)
				if (vLocalConfigurationData->vExternalDetector[TempLoop].vDigitalInputLine == 2)
				{
					TempNameString = "Aux. Detector ";
					TempNameString = TempNameString + dtoa(TempLoop + 1, 0) + ": " + vLocalConfigurationData->vExternalDetector[TempLoop].vName;
				}

				TempString = TempNameString;
			}
		break;
		case 19: //normally Aux Detector 3
			TempNameString = "Unused Aux. Detector " + dtoa(TempLineNumber - 16, 0);
			for (TempLoop = 0;TempLoop < cNumberOfExternalDetectors; TempLoop++)
			if (vLocalConfigurationData->vExternalDetector[TempLoop].vEnabled)
			if (vLocalConfigurationData->vExternalDetector[TempLoop].vDigitalInputLine == 3)
			{
				TempNameString = "Aux. Detector ";
				TempNameString = TempNameString + dtoa(TempLoop + 1, 0) + ": " + vLocalConfigurationData->vExternalDetector[TempLoop].vName;
			}

			TempString = TempNameString;
		break;
		case 20: //normally Aux Detector 4
			TempNameString = "Unused Aux. Detector " + dtoa(TempLineNumber - 16, 0);
			for (TempLoop = 0;TempLoop < cNumberOfExternalDetectors; TempLoop++)
			if (vLocalConfigurationData->vExternalDetector[TempLoop].vEnabled)
			if (vLocalConfigurationData->vExternalDetector[TempLoop].vDigitalInputLine == 4)
			{
				TempNameString = "Aux. Detector ";
				TempNameString = TempNameString + dtoa(TempLoop + 1, 0) + ": " + vLocalConfigurationData->vExternalDetector[TempLoop].vName;
			}

			TempString = TempNameString;
		break;
		case 21: //normally Aux Detector 5
			TempNameString = "Unused Aux. Detector " + dtoa(TempLineNumber - 16, 0);
			for (TempLoop = 0;TempLoop < cNumberOfExternalDetectors; TempLoop++)
			if (vLocalConfigurationData->vExternalDetector[TempLoop].vEnabled)
			if (vLocalConfigurationData->vExternalDetector[TempLoop].vDigitalInputLine == 5)
			{
				TempNameString = "Aux. Detector ";
				TempNameString = TempNameString + dtoa(TempLoop + 1, 0) + ": " + vLocalConfigurationData->vExternalDetector[TempLoop].vName;
			}

			TempString = TempNameString;
		break;
		case 22: //normally Eject Confirmer 1
			if (vLocalConfigurationData->vResynchronizeEjectors)
			{
				TempString = "Resynchronizing Sensor for Ejector 2";
			}
			else
			{
				TempNameString = "Unused Eject Confirmer " + dtoa(TempLineNumber - 21, 0);
				if (vLocalConfigurationData->vEjector[TempLineNumber - 22].vConfirmDigitalInputLine == 1)
				{
					TempNameString = "Eject Confirmer " + dtoa(TempLineNumber - 21, 0) + " for: " + vLocalConfigurationData->vEjector[TempLineNumber - 22].vName;
				}
				if (TempLineNumber == 22)
				for (TempLoop = 0;TempLoop < cNumberOfExternalDetectors; TempLoop++)
				if ((vLocalConfigurationData->vExternalDetector[TempLoop].vEnabled) && (vLocalConfigurationData->vExternalDetector[TempLoop].vDigitalInputLine == 6))
				{
					TempNameString = "Aux. Detector ";
					TempNameString = TempNameString + dtoa(TempLoop + 1, 0) + ": " + vLocalConfigurationData->vExternalDetector[TempLoop].vName;
				}
				TempString = TempNameString;
			}
		break;
		case 23:   //normally Eject Confirmer 2
			TempNameString = "Unused Eject Confirmer B";// + dtoa(TempLineNumber - 21, 0);
			//if (vLocalConfigurationData->vEjector[TempLineNumber - 22].vConfirmDigitalInputLine == 2)
			//{
			//	TempNameString = "Eject Confirmer " + dtoa(TempLineNumber - 21, 0) + " for: " + vLocalConfigurationData->vEjector[TempLineNumber - 22].vName;
			//}
			//if (TempLineNumber == 22)
			//for (TempLoop = 0;TempLoop < cNumberOfExternalDetectors; TempLoop++)
			//if ((vLocalConfigurationData->vExternalDetector[TempLoop].vEnabled) && (vLocalConfigurationData->vExternalDetector[TempLoop].vDigitalInputLine == 7))
			//{
			//	TempNameString = "Aux. Detector ";
			//	TempNameString = TempNameString + dtoa(TempLoop + 1, 0) + ": " + vLocalConfigurationData->vExternalDetector[TempLoop].vName;
			//}
			TempString = TempNameString;
		break;
		case 24: //normally Eject Confirmer 3
			TempNameString = "Unused Eject Confirmer C";// + dtoa(TempLineNumber - 21, 0);
			//if (vLocalConfigurationData->vEjector[TempLineNumber - 22].vConfirmDigitalInputLine == 3)
			//{
			//	TempNameString = "Eject Confirmer " + dtoa(TempLineNumber - 21, 0) + " for: " + vLocalConfigurationData->vEjector[TempLineNumber - 22 + 2].vName;
			//}
			//if (TempLineNumber == 22)
			//for (TempLoop = 0;TempLoop < cNumberOfExternalDetectors; TempLoop++)
			//if ((vLocalConfigurationData->vExternalDetector[TempLoop].vEnabled) && (vLocalConfigurationData->vExternalDetector[TempLoop].vDigitalInputLine == 8))
			//{
			//	TempNameString = "Aux. Detector ";
			//	TempNameString = TempNameString + dtoa(TempLoop + 1, 0) + ": " + vLocalConfigurationData->vExternalDetector[TempLoop].vName;
			//}
			TempString = TempNameString;
		break;
		case 33:
		case 34:
		case 35:
		case 36:  // 33 - 48
		case 37:
			TempNameString = "Unused Container Trigger or Spare";
			for (TempLoop = 0;TempLoop < cNumberOfBodyTriggers; TempLoop++)
			{
				if (vLocalConfigurationData->vBodyTrigger[TempLoop].vDigitalInputLine == 
					TempLineNumber - 32)
				{
					TempNameString = "Container Trigger ";
					TempNameString = TempNameString + dtoa(TempLoop + 1, 0) + ": " + vLocalConfigurationData->vBodyTrigger[TempLoop].vName;

					if (!vLocalConfigurationData->vBodyTrigger[TempLoop].vEnabled)
						TempNameString = TempNameString + "-Disabled";
				}
			}
			TempString = TempNameString;
		break;

		case 49:	  //49
			TempString = "Conveyor On/Off (1/0)";
		break;
		case 50:	  //50
			TempNameString = vLocalConfigurationData->vEjector[0].vName;
			TempString = "Ejector 1:" + TempNameString;
		break;
		case 51:	  //51
			TempNameString = vLocalConfigurationData->vEjector[1].vName;
			TempString = "Ejector 2:" + TempNameString;
		break;
		case 52:	  //52
			TempNameString = vLocalConfigurationData->vEjector[2].vName;
			TempString = "Ejector 3:" + TempNameString;
		break;
		case 25:	  //25
			TempNameString = vLocalConfigurationData->vEjector[3].vName;
			TempString = "Ejector 4:" + TempNameString;
		break;
		case 26:	  //26
			TempNameString = vLocalConfigurationData->vEjector[4].vName;
			TempString = "Ejector 5:" + TempNameString;
		break;
		case 27:	  //27
			TempNameString = vLocalConfigurationData->vEjector[5].vName;
			TempString = "Ejector 6:" + TempNameString;
		break;
		case 53:  //53
			TempString = "X of Y Alarm 1";
		break;
		case 54:  //54
			TempString = "X of Y Alarm 2";
		break;
		case 55:  //55
			TempString = "Critical Alarm";
		break;
		case 56:  //56
			TempString = "Can Stop 1";
		break;
		case 57:  //57
			TempString = "Can Stop 2";
		break;
		case 58:  //58
			TempString = "Ejectors Enabled";
		break;
		case 59:  //59
			TempString = "Red Light Output";
		break;
		case 60:  //60
			TempString = "Yellow Light Output";
		break;
		case 61:  //61
			TempString = "Blue Light Output";
		break;
		case 62:  //62
			TempString = "Spellman X-Ray HV Enable Relay";
		break;
		case 63:  //63
			TempString = "Spellman X-Ray HV Latch On Relay";
		break;
		case 64:  //64
			TempString = "XR-50 HV On";
		break;
		case 65:  //65
			TempString = "Frame Enable (From FPGA)";
		break;
		case 66:  //66
			TempString = "Line Enable";
		break;
		case 67:  //67
			TempString = "Conveyor Encoder";
		break;
		case 68:  //68
			TempString = "Frame Start (From uC)";
		break;
		case 69:  //69
			TempString = "Frame Enable B";
		break;
		case 70:  //70
			TempString = "Frame Enable A";
		break;
		case 71:  //71
			TempString = "Radiation Shutter Output";
		break;
		case 72:  //72
			TempString = "System Inspecting Output";
		break;
		case 73:  //73
			TempString = "Spare 1";
		break;
		case 74:  //74
			TempString = "Radiation Shutter Container Trigger Input";
		break;
		case 101:
			TempString = "LVDS Clock";
		break;
		case 102:
			TempString = "CamLink Board Clock";
		break;
		default:
			TempString = "Und-Undefined";

	}
	return TempString;
}

void CDiagnosticLineDialog::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == vLocalSystemData->vDiagnosticDataReadTimerHandle)
	{
		CFileException TempFileException;
	//	SYSTEMTIME TempSystemTime;
		double TempStartMicroSecond = 0;
		double TempEndMicroSecond = 0;

		if (vLocalSystemData->vITIPCDig->vCaptureModule != NULL)
		{
			if (vSampleSpeed != 1)
				vLocalSystemData->vNextDiagnosticArrayEntry = 0;
			
			GetOScopeDataFromGrabber();

			if (!vTakingData)
			{
				switch (vTriggerMode)
				{
					case cContinuousTrigger:
						vTakingData = true;
						if (vSampleSpeed != 1)
							vLocalSystemData->vNextDiagnosticArrayEntry = 0;
					break;
					case cTriggerOnChannel:
					{	//try to find a trigger
						int TempDebugCount = 0;
						BYTE TempValue1 = 0;
						BYTE TempValue2 = 0;
							
						for (int i = 0; i < vSampleSpeed * 25; i++)
						{
							//12/4/2009 if (vTriggerArmed)
							{
								//if the previous value is a zero, and new value is a one
								if ((!(vOldDiagnosticLineValue & (1 << vTriggerChannel)))
									&& (vLocalSystemData->vDiagnosticLineValue & (1 << vTriggerChannel)))
								{
									vTakingData = true;
									//12/4/2009vTriggerArmed = false;

									// you can't see vDiagnosticData[0] on screen
									vLocalSystemData->vDiagnosticData[1] = TempValue1;
									vLocalSystemData->vDiagnosticData[2] = TempValue2;
									vLocalSystemData->vDiagnosticData[3] = vLocalSystemData->vDiagnosticLineValue;
									vLocalSystemData->vNextDiagnosticArrayEntry = 4;
									//12/4/2009
									DisplayEjectorOnOffButtons();
									break;
								}
								else
								{
									TempDebugCount++;
									TempValue1 = TempValue2;
									TempValue2 = vLocalSystemData->vDiagnosticLineValue; //replace first value if second value does not hit
								}
							}
							//12/4/2009 
							/*
							else
							if (!(vLocalSystemData->vDiagnosticLineValue & (1 << vTriggerChannel)))
							{
								vTriggerArmed = true;
									
								TempValue2 = vLocalSystemData->vDiagnosticLineValue;
							}
							*/
							GetOScopeDataFromGrabber();
						}
					}
					break;
					case cSingleTriggerOnChannel:
					{
						if (vTriggerArmed)
						{
							for (int i = 0; i < vSampleSpeed*100; i++)
							{
								//if the previous value is a zero, and new value is a one
								if (!(vOldDiagnosticLineValue & (1 << vTriggerChannel)))
								if (vLocalSystemData->vDiagnosticLineValue & (1 << vTriggerChannel))
								{
									vLocalSystemData->vDiagnosticData[1] = vOldDiagnosticLineValue;
									vLocalSystemData->vDiagnosticData[2] = vLocalSystemData->vDiagnosticLineValue;
									vLocalSystemData->vNextDiagnosticArrayEntry = 3;
									
									vTakingData = true;
									vTriggerArmed = false;
									//12/4/2009
									DisplayEjectorOnOffButtons();
									break;
								}
								GetOScopeDataFromGrabber();
							}
						}
					}
					break;
				}
			}


			if (vTakingData)
			{
				TempStartMicroSecond = CIFCOS::GetSystimeMicrosecs();

				for (int i = 0; i < vSampleSpeed; i++)// 400 is high resolution mode, 1 continuous or normal resolution (slow), no other values
				{
					if (vSampleSpeedDelayMagnitude != 0)
					{
						unsigned long int i = 0, j = 0;
						unsigned long int TempWaitTime = 1;

						for (i = 0; i < vSampleSpeedDelayMagnitude; i++)
							TempWaitTime = TempWaitTime * 2;//figuring magnitude of delay time

						for (j = 0; j < TempWaitTime; j++)
						{
							BYTE TempValue = (BYTE)vLocalSystemData->vITIPCDig->vCaptureModule->InportVal(0);

							for (i = 0; i < 6; i++)
							{
								int TempBitMask = 1 << i;
								if ((vOldDiagnosticLineValue & TempBitMask) == (vLocalSystemData->vDiagnosticLineValue & TempBitMask))
								if ((TempValue & TempBitMask) != (vOldDiagnosticLineValue & TempBitMask))
								{
									vLocalSystemData->vDiagnosticLineValue = vLocalSystemData->vDiagnosticLineValue ^ TempBitMask;
								}
								TempBitMask = vLocalSystemData->vDiagnosticLineValue ^ TempBitMask;//this line does nothing, attempting to even out timing
							}
						}	
					}
					//get next value
					GetOScopeDataFromGrabber();
					
					//put this value in array
					if (vLocalSystemData->vNextDiagnosticArrayEntry < vNumberOfDataPoints)
					{
						vLocalSystemData->vDiagnosticData[vLocalSystemData->vNextDiagnosticArrayEntry] = vLocalSystemData->vDiagnosticLineValue;
						vLocalSystemData->vNextDiagnosticArrayEntry++;
					}
					else
					{
						vLocalSystemData->vNextDiagnosticArrayEntry = 0;
						if (vSampleSpeed == vNumberOfDataPoints)
							break;
					}

					//BYTE *TempPointer;
					/*
					if (vLogDiagnosticLines)
					{
						TRY
						{
							TempPointer = &vLocalSystemData->vDiagnosticData[vLocalSystemData->vNextDiagnosticArrayEntry];
							vLocalSystemData->vDiagnosticLineDataFile->Write( 
								TempPointer, 1);
						}
						CATCH_ALL(TempFileException)
						{
						 #ifdef _DEBUG
								afxDump << "Could Not Write to Diagnostic Line File"  << "\n";
						 #endif
						}
						END_CATCH_ALL
					}
					*/
	
				}
				TempEndMicroSecond = CIFCOS::GetSystimeMicrosecs();

				double TempTimeIntervalFor400SamplesInMicroSeconds = //(TempEndMilliSecond - TempStartMilliSecond) * 1000 
					+ (TempEndMicroSecond - TempStartMicroSecond);
				if (TempTimeIntervalFor400SamplesInMicroSeconds >= 0)	//condition filters samples that are bad because of the counter overflow in the milliseconds counter
					vTimeIntervalFor50SamplesInMicroSeconds = (double)TempTimeIntervalFor400SamplesInMicroSeconds/(double)8000;
				else
					vTimeIntervalFor50SamplesInMicroSeconds = 0;

				if (vSampleSpeed != 1)
				{
					//12/4/2009
					vTakingData = false;
					vLocalSystemData->vNextDiagnosticArrayEntry = 0;
					UpdateOscilloscope();
				}
				
				if (vLocalSystemData->vNextDiagnosticArrayEntry >= vNumberOfDataPoints)			//antiquated - cDiagnosticLineArraySize)
				{
					if (vTriggerMode != cContinuousTrigger)
					{
						vLocalSystemData->vNextDiagnosticArrayEntry = 0;
						vTakingData = false;
						vTriggerArmed = false;
						//12/4/2009
						DisplayEjectorOnOffButtons();
						UpdateOscilloscope();
					}
				}
			}
		}
		//12/3/2009 
		//if (vTriggerMode != cContinuousTrigger)
		//	DisplayEjectorOnOffButtons();
	}
	if (nIDEvent == vDisplayTimerHandle)
  {
		// Read input port and display
		UpdateDisplay();
	}
	if (nIDEvent == vOscilloscopeTimerHandle)
		if (vSampleSpeed == 1)
			UpdateOscilloscope();
	CDialog::OnTimer(nIDEvent);
}

void CDiagnosticLineDialog::UpdateOscilloscope()
{
	//12/4/2009
	//if (vTakingData)
	DWORD TempHeightOffset26 = ResizeYCoor(26);
	BYTE TempUpdateOscilloscopeLoop;

	for (TempUpdateOscilloscopeLoop = 0; TempUpdateOscilloscopeLoop < 6; TempUpdateOscilloscopeLoop++)
	{
		RECT TempRectangle;
		TempRectangle.left = vLeftSide;
		TempRectangle.top = vTop + (TempUpdateOscilloscopeLoop * vTraceHeight);
		TempRectangle.right = vRightSide;
		TempRectangle.bottom = vTop + TempHeightOffset26 + (TempUpdateOscilloscopeLoop * vTraceHeight);
		this->InvalidateRect(&TempRectangle,true);
	}
	if (vSampleSpeed != 1)
	{
		SetDlgItemText(IDC_RefreshCount, dtoa(vTimeIntervalFor50SamplesInMicroSeconds,3,2));
		SetDlgItemText(IDC_DeltaCountLabel, _T(" Millisecond Intervals, 50 Samples per Interval"));
	}
}

void CDiagnosticLineDialog::UpdateDisplay() 
{
		if (vLVDSActive != vOldLVDSActive)
		{
			if ((vOldLVDSActive) ^ (vLVDSActive)) //use XOR to see if it is different
			{
				CWnd *TempWindow = CWnd::GetDlgItem(IDC_LVDS);
				if (TempWindow)
					TempWindow->Invalidate(false);
			}
			vOldLVDSActive = vLVDSActive;
		}
		vLVDSActive = 0;

		if (vCamLinkActive != vOldCamLinkActive)
		{
			if ((vOldCamLinkActive) ^ (vCamLinkActive)) //use XOR to see if it is different
			{
				CWnd *TempWindow = CWnd::GetDlgItem(IDC_CamLink);
				if (TempWindow)
					TempWindow->Invalidate(false);
			}
			vOldCamLinkActive = vCamLinkActive;
		}
		vCamLinkActive = 0;
	if (vOldCurrentConveyorOutput != (vLocalSystemData->vCurrentConveyorOutput & 2))
	{
		vOldCurrentConveyorOutput = vLocalSystemData->vCurrentConveyorOutput & 2;
		DisplayEjectorOnOffButtons();
	}

	if (vOldXRayOnNotice != vLocalSystemData->vXRayOnNotice)
	{
		vOldXRayOnNotice = vLocalSystemData->vXRayOnNotice;
		if (vOldXRayOnNotice)
		{
			m_XRaysOn.ShowWindow(SW_SHOW);
			m_SubFunction6Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
			SetDlgItemText(IDC_SubFunction6Button, _T("Stop"));
		}
		else
		{
			m_XRaysOn.ShowWindow(SW_HIDE);
			m_SubFunction6Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
			SetDlgItemText(IDC_SubFunction6Button, _T("Run Diagnostic Mode"));
		}
	}
	if (vOldEntry != vLocalSystemData->vNextDiagnosticArrayEntry)
	{
		WORD TempDeltaCount = 0;
		if (vLocalSystemData->vNextDiagnosticArrayEntry > vOldEntry)
			TempDeltaCount = vLocalSystemData->vNextDiagnosticArrayEntry - vOldEntry;
		else
			TempDeltaCount = vNumberOfDataPoints + //cDiagnosticLineArraySize + 
				vLocalSystemData->vNextDiagnosticArrayEntry - vOldEntry;
		
		SetDlgItemText(IDC_DeltaCount, dtoa(TempDeltaCount * 10, 0));

		vOldEntry = vLocalSystemData->vNextDiagnosticArrayEntry;
		SetDlgItemText(IDC_RefreshCount, dtoa(vOldEntry, 0));
	}
	int TempID;
	BYTE TempMask;
	for (BYTE TempLoop = 0; TempLoop < 6; TempLoop++)
	{
		switch (TempLoop)
		{
			case 0:
				TempID = IDC_Line1;
				TempMask = 1;
			break;
			case 1:
				TempID = IDC_Line2;
				TempMask = 2;
			break;
			case 2:
				TempID = IDC_Line3;
				TempMask = 4;
			break;
			case 3:
				TempID = IDC_Line4;
				TempMask = 8;
			break;
			case 4:
				TempID = IDC_Line5;
				TempMask = 0x10;
			break;
			case 5:
				TempID = IDC_Line6;
				TempMask = 0x20;
			break;
		}
		if ((vOldDiagnosticLineValue & TempMask) != 
			(vLocalSystemData->vDiagnosticLineValue & TempMask))
		{
			if ((vLocalSystemData->vDiagnosticLineValue & TempMask) == 0)
			{
				SetDlgItemText(TempID, _T("0"));
			}
			else
			{
				SetDlgItemText(TempID, _T("1"));
			}
		}
	}
	vOldDiagnosticLineValue = vLocalSystemData->vDiagnosticLineValue;
}


BOOL CDiagnosticLineDialog::PreTranslateMessage(MSG* pMsg) 
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
/*			case 38:	
			case 98:	//arrow up on scantrac
				//increase interval in oscilloscope graph by 2x
				if (vSampleSpeedDelayMagnitude < 11)
				{
					vSampleSpeedDelayMagnitude++;
					SetDlgItemText(IDC_ZoomMagnitude,("Zoom: " + dtoa(12 - vSampleSpeedDelayMagnitude) + "x"));
					vSubFunction4ButtonEnable = true;	//m_SubFunction4Button.EnableWindow(true);
					if (vSampleSpeedDelayMagnitude == 11)
						vSubFunction3ButtonEnable = false;	//m_SubFunction3Button.EnableWindow(false);
				}
				else
				{
					CNoticeDialog TempNoticeDialog;
					TempNoticeDialog.vNoticeText = "\n\n\nAll the way Zoomed Out";
					TempNoticeDialog.vType = cNoticeMessage;
					TempNoticeDialog.DoModal();
				}
			break;
			case 40:
			case 104:	//arrow down on scantrac
				//decrease interval
				if (vSampleSpeedDelayMagnitude > 0)
				{
					vSampleSpeedDelayMagnitude--;
					SetDlgItemText(IDC_ZoomMagnitude,("Zoom: " + dtoa(12 - vSampleSpeedDelayMagnitude) + "x"));
					vSubFunction3ButtonEnable = true;	//m_SubFunction3Button.EnableWindow(true);
					if (vSampleSpeedDelayMagnitude == 0)
						vSubFunction4ButtonEnable = false;	//m_SubFunction4Button.EnableWindow(false);
				}
				else
				{
					CNoticeDialog TempNoticeDialog;
					TempNoticeDialog.vNoticeText = "\n\n\nAll the way Zoomed In";
					TempNoticeDialog.vType = cNoticeMessage;
					TempNoticeDialog.DoModal();
				}
			break;
			case 102: // side arrow debug only
			{
				CNoticeDialog TempNoticeDialog;
				TempNoticeDialog.vNoticeText = vDebugString;
				TempNoticeDialog.vType = cNoticeMessage;
				TempNoticeDialog.DoModal();
				vDebugString = " ";
			}
			break;*/
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
				PostThreadMessage(vGlobalCallingThreadID, cShowHelp, 18, 0);

				TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText1);
				if (TempWindow)
					TempWindow->Invalidate(false);
			break;
			case cPageUp: // Clear, but do inspx hidden dialog
			{ 
				if (vGlobalPasswordLevel)
				{
					PasswordOK(cNoPassword, false);
					DisplayEjectorOnOffButtons();
					m_SubFunction6Button.ShowWindow(SW_HIDE);
					this->InvalidateRect(&m_SubFunction6Button.GetRectToInvalidateArrow());
					m_Function3Button.ShowWindow(SW_HIDE);
					this->InvalidateRect(&m_Function3Button.GetRectToInvalidateArrow());
					m_Function4Button.ShowWindow(SW_HIDE);
					this->InvalidateRect(&m_Function4Button.GetRectToInvalidateArrow());
					m_SubFunction2Button.ShowWindow(SW_HIDE);
					this->InvalidateRect(&m_SubFunction2Button.GetRectToInvalidateArrow());
					m_SubFunction5Button.ShowWindow(SW_HIDE);
					this->InvalidateRect(&m_SubFunction5Button.GetRectToInvalidateArrow());
				}
				else
				{
					PasswordOK(cAnyPasswordToEnter, true); //allow any password to be entered
					if (PasswordOK(cTemporaryInspxPassword, false))
					{
						m_SubFunction6Button.ShowWindow(SW_SHOW);
					}
				}
				DisplayEjectorOnOffButtons();
				TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText1);
				if (TempWindow)
					TempWindow->Invalidate(false);
			}
			break;	
			case 27: // Escape
				::PostThreadMessage(vGlobalCallingThreadID,	cStartChatWindowMessage, 0,0);
			break;	
			//case c1:
			//	vMainWindowPointer->ReCalibrateWhileRunning();
			//break;
		}
		return true;  //true indicates it has been handled, so do not process
	}
	return CDialog::PreTranslateMessage(pMsg);
}

HBRUSH CDiagnosticLineDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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

	if (pWnd->GetDlgCtrlID() == IDC_SubFunction0Display)
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction0Button, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBlack);
		return vGlobalButtonColorBrush;
	}

	if (pWnd->GetDlgCtrlID() == IDC_SubFunction1Button)
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction1Button, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBlack);
		return vGlobalButtonColorBrush;
	}

	if (pWnd->GetDlgCtrlID() == IDC_SubFunction2Button)
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction2Button, 5);  //5 is medium large
		if (PasswordOK(cTemporaryInspxPassword,false))
		{
			pDC->SetBkMode(TRANSPARENT);
			if (vSubFunction2ButtonEnable == true)
				pDC->SetTextColor(cButtonTextColor);
			else
				pDC->SetTextColor(cGray);
			if (vLocalSystemData->vCurrentEjectorOnStatus & 1)		
				return vLocalSystemData->vGreenBrush;
			else
				return vLocalSystemData->vYellowBrush;
	  }
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction3Button)
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction3Button, 5);  //5 is medium large
		if (PasswordOK(cTemporaryInspxPassword,false))
		{
			pDC->SetBkMode(TRANSPARENT);
			if (vSubFunction3ButtonEnable == true)
				pDC->SetTextColor(cButtonTextColor);
			else
				pDC->SetTextColor(cGray);
			if (vLocalSystemData->vCurrentEjectorOnStatus & 2)		
				return vLocalSystemData->vGreenBrush;
			else
				return vLocalSystemData->vYellowBrush;
	  }
	}

	if (pWnd->GetDlgCtrlID() == IDC_SubFunction4Button)
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction4Button, 5);  //5 is medium large
		if (PasswordOK(cTemporaryInspxPassword,false))
		{
			pDC->SetBkMode(TRANSPARENT);
			if (vSubFunction4ButtonEnable == true)
				pDC->SetTextColor(cButtonTextColor);
			else
				pDC->SetTextColor(cGray);
			//pDC->SetTextColor(cButtonTextColor);
			if (vLocalSystemData->vCurrentEjectorOnStatus & 4)		
				return vLocalSystemData->vGreenBrush;
			else
				return vLocalSystemData->vYellowBrush;
	  }
	}

	if (pWnd->GetDlgCtrlID() == IDC_SubFunction6Button)
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction6Button, 7);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		if (vOldXRayOnNotice)
		{
			return vLocalSystemData->vRedBrush;
		}
		else
			return vLocalSystemData->vYellowBrush;
	}

	if (pWnd->GetDlgCtrlID() == IDC_SubFunction7Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction7Button, 5);  //5 is medium large
		if (vTriggerMode == cSingleTriggerOnChannel)
		{
			if (vTriggerArmed)
				return vLocalSystemData->vRedBrush;
			else
				return vLocalSystemData->vGreenBrush;
		}
		else
		{
			if (vLocalSystemData->vCurrentConveyorOutput & 2)
				return vLocalSystemData->vRedBrush;
			else
				return vLocalSystemData->vGreenBrush;
		}
	}

	if (pWnd->GetDlgCtrlID() == IDC_CamLink)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_CamLink, 5);
		pDC->SetBkMode(TRANSPARENT);
		if (vOldCamLinkActive)
			pDC->SetTextColor(cGreen);
		else
			pDC->SetTextColor(cRed);
			
	}

	if (pWnd->GetDlgCtrlID() == IDC_LVDS)
	{
		pDC->SetBkMode(TRANSPARENT);
		if (vOldLVDSActive)
			pDC->SetTextColor(cGreen);
		else
			pDC->SetTextColor(cRed);
			
	}

	if (pWnd->GetDlgCtrlID() == IDC_Background)
	{
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_LineLabel1)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_LineLabel1, 5);
		pDC->SetBkMode(TRANSPARENT);
	}
	if (pWnd->GetDlgCtrlID() == IDC_LineLabel2)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_LineLabel2, 5);
		pDC->SetBkMode(TRANSPARENT);
	}
	if (pWnd->GetDlgCtrlID() == IDC_LineLabel3)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_LineLabel3, 5);
		pDC->SetBkMode(TRANSPARENT);
	}
	if (pWnd->GetDlgCtrlID() == IDC_LineLabel4)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_LineLabel4, 5);
		pDC->SetBkMode(TRANSPARENT);
	}
	if (pWnd->GetDlgCtrlID() == IDC_LineLabel5)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_LineLabel5, 5);
		pDC->SetBkMode(TRANSPARENT);
	}
	if (pWnd->GetDlgCtrlID() == IDC_LineLabel6)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_LineLabel6, 5);
		pDC->SetBkMode(TRANSPARENT);
	}
	if (pWnd->GetDlgCtrlID() == IDC_DeltaCountLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DeltaCountLabel, 5);
		pDC->SetBkMode(TRANSPARENT);
	}
	if (pWnd->GetDlgCtrlID() == IDC_RefreshCountLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_RefreshCountLabel, 5);
		pDC->SetBkMode(TRANSPARENT);
	}
	if (pWnd->GetDlgCtrlID() == IDC_TriggerMode)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_TriggerMode, 5);
		pDC->SetBkMode(TRANSPARENT);
	}
	if (pWnd->GetDlgCtrlID() == IDC_RefreshCount)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_RefreshCount, 5);
		pDC->SetBkMode(TRANSPARENT);
	}
	if (pWnd->GetDlgCtrlID() == IDC_LVDS)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_LVDS, 5);
		pDC->SetBkMode(TRANSPARENT);
	}
	//if (pWnd->GetDlgCtrlID() == IDC_CurrentLabel)
	//{
	//	TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_CurrentLabel, 5);
	//	pDC->SetBkMode(TRANSPARENT);
	//	return vLocalSystemData->vLightBrownBrush;
	//}
	if (pWnd->GetDlgCtrlID() == IDC_XRaysOn)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_XRaysOn, 7);
		pDC->SetTextColor(cYellow);
		pDC->SetBkMode(TRANSPARENT);
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
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function5Button, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		if (vFunction5ButtonEnable == true)
			pDC->SetTextColor(cButtonTextColor);
		else
			pDC->SetTextColor(cGray);
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
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction3Button, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		if (vSubFunction3ButtonEnable == true)
			pDC->SetTextColor(cButtonTextColor);
		else
			pDC->SetTextColor(cGray);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction4Button) 
	{   
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction4Button, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		if (vSubFunction4ButtonEnable == true)
			pDC->SetTextColor(cButtonTextColor);
		else
			pDC->SetTextColor(cGray);
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

void CDiagnosticLineDialog::OnPaint() 
{
	// Draw Diagnostic Data On Screen
	CPaintDC dc(this); // device context for painting
	{
		BYTE TempOffset = 0;
		BYTE TempMask = 1;
		BYTE TempLoop = 0;

		DWORD TempHeightOffset26 = ResizeYCoor(26);
		DWORD TempHeightOffset20 = ResizeYCoor(20);
		DWORD TempXIncrement50 = ResizeXCoor(50); 
		BYTE TempYHighTraceOffset15 = (BYTE) ResizeYCoor(15);
		vRightSide = vLeftSide + vNumberOfDataPoints;
		
		for (TempLoop = 0; TempLoop < 6; TempLoop++)
		{
			dc.SelectObject(vBluePen);
			dc.MoveTo(vLeftSide,vTop + (TempLoop * vTraceHeight));
			dc.LineTo(vRightSide + 1,vTop + (TempLoop * vTraceHeight)); //top line
			dc.MoveTo(vLeftSide,vTop + TempHeightOffset26 + (TempLoop * vTraceHeight));
			dc.LineTo(vRightSide + 1,vTop + TempHeightOffset26 + (TempLoop * vTraceHeight)); //bottom line

			dc.MoveTo(vLeftSide,vTop + (TempLoop * vTraceHeight));  //top left corner
			dc.LineTo(vLeftSide,vTop + TempHeightOffset26 + (TempLoop * vTraceHeight)); //left edge line
			dc.MoveTo(vRightSide,vTop + (TempLoop * vTraceHeight)); //top right corner
			dc.LineTo(vRightSide,vTop + TempHeightOffset26 + (TempLoop * vTraceHeight));  //right edge line

			dc.SelectObject(vRedPen);
			for (WORD TempLoopS = 50; TempLoopS < vNumberOfDataPoints; TempLoopS = TempLoopS + 50) //origionally to 400
			{
				dc.MoveTo(vLeftSide + TempLoopS,vTop + 1 +(TempLoop * vTraceHeight)); //division lines up and down
				dc.LineTo(vLeftSide + TempLoopS, vTop + TempHeightOffset26 + (TempLoop * vTraceHeight));
				//ReportErrorMessage("RedSeg ("+ dtoa(vLeftSide + TempLoopS) 
				//		+ ", " + dtoa(vTop + 1 +(TempLoop * vTraceHeight)) + ") to (" + dtoa(vLeftSide + TempLoopS) + ", " 
				//	+ dtoa(vTop + TempHeightOffset26 + (TempLoop * vTraceHeight)) + ")", cWriteToLog, 32000);
			}

			dc.SelectObject(vDarkBluePen);
			//12/4/2009
			if (vLocalSystemData->vNextDiagnosticArrayEntry)
			{
				dc.MoveTo(vLocalSystemData->vNextDiagnosticArrayEntry + vLeftSide,vTop + (TempLoop * vTraceHeight));
				dc.LineTo(vLocalSystemData->vNextDiagnosticArrayEntry + vLeftSide, vTop + TempHeightOffset26 + (TempLoop * vTraceHeight));  //current time line

				//ReportErrorMessage("BlueCurse ("+ dtoa(vLocalSystemData->vNextDiagnosticArrayEntry + vLeftSide) +
				//	", " + dtoa(vTop + (TempLoop * vTraceHeight)) + ") to (" 
				//	+ dtoa(vLocalSystemData->vNextDiagnosticArrayEntry + vLeftSide) + ", " 
				//	+ dtoa(vTop + TempHeightOffset26 + (TempLoop * vTraceHeight)) + ")", cWriteToLog, 32000);
			}
		}

		//data lines or scope traces
		for (BYTE TempLineLoop = 0; TempLineLoop < 6; TempLineLoop++)
		{
			if (TempLineLoop > 0)
				TempMask = TempMask << 1;
			for (WORD TempPixelLoop = 0; TempPixelLoop < vNumberOfDataPoints; TempPixelLoop++) //cDiagnosticLineArraySize
			{
				if ((vLocalSystemData->vDiagnosticData[TempPixelLoop] & TempMask) == TempMask)
					TempOffset = TempYHighTraceOffset15;
				else
					TempOffset = 0;
				if ((TempPixelLoop + vLeftSide <= dc.m_ps.rcPaint.right) &&
					(TempPixelLoop + vLeftSide >= dc.m_ps.rcPaint.left) &&
					((WORD)(vTop + TempHeightOffset20 - TempOffset + (vTraceHeight * TempLineLoop)) >= dc.m_ps.rcPaint.top) &&
					((WORD)(vTop + TempHeightOffset20 - TempOffset + (vTraceHeight * TempLineLoop)) <= dc.m_ps.rcPaint.bottom))
				{
					dc.SetPixelV(TempPixelLoop + vLeftSide,
						vTop + TempHeightOffset20 - TempOffset + (vTraceHeight * TempLineLoop),
						cBlack);
					dc.SetPixelV(TempPixelLoop + vLeftSide,
						vTop + TempHeightOffset20 + 1 - TempOffset + (vTraceHeight * TempLineLoop),
						cBlack);
					dc.SetPixelV(TempPixelLoop + vLeftSide,
						vTop + TempHeightOffset20 + 2 - TempOffset + (vTraceHeight * TempLineLoop),
						cBlack);
				}
			}
		}
		/*
		for (TempLoop = 0; TempLoop < 215; TempLoop++)
			dc.SetPixelV(vLocalSystemData->vNextDiagnosticArrayEntry + vLeftSide,
			173 + TempLoop,cDarkBlue);
			*/
	}
	// Do not call CDialog::OnPaint() for painting messages
}


void CDiagnosticLineDialog::UpdateSignalName()
{
	// Show Signal Names
	CString TempString("");
	TempString = "";
	TempString = "1. " + GetInputName(vLocalConfigurationData->vDiagnosticLineSelect[0]);
	SetDlgItemText(IDC_LineLabel1,TempString);
	TempString = "2. " + GetInputName(vLocalConfigurationData->vDiagnosticLineSelect[1]);
	SetDlgItemText(IDC_LineLabel2,TempString);
	TempString = "3. " + GetInputName(vLocalConfigurationData->vDiagnosticLineSelect[2]);
	SetDlgItemText(IDC_LineLabel3,TempString);
	TempString = "4. " + GetInputName(vLocalConfigurationData->vDiagnosticLineSelect[3]);
	SetDlgItemText(IDC_LineLabel4,TempString);
	TempString = "5. " + GetInputName(vLocalConfigurationData->vDiagnosticLineSelect[4]);
	SetDlgItemText(IDC_LineLabel5,TempString);
	TempString = "6. " + GetInputName(vLocalConfigurationData->vDiagnosticLineSelect[5]);
	SetDlgItemText(IDC_LineLabel6,TempString);
}

void CDiagnosticLineDialog::PrepareToExit()
{
	if (vOldEjectorOnStatus != vLocalSystemData->vCurrentEjectorOnStatus)
	{
		if ((vOldEjectorOnStatus & 1) != (vLocalSystemData->vCurrentEjectorOnStatus & 1))
			vGlobaluCSerialPort->SendEjectorOnOffCommand(1, (vOldEjectorOnStatus & 1));
		if ((vOldEjectorOnStatus & 2) != (vLocalSystemData->vCurrentEjectorOnStatus & 2))
			vGlobaluCSerialPort->SendEjectorOnOffCommand(2, (vOldEjectorOnStatus & 2));
		if ((vOldEjectorOnStatus & 4) != (vLocalSystemData->vCurrentEjectorOnStatus & 4))
			vGlobaluCSerialPort->SendEjectorOnOffCommand(3, (vOldEjectorOnStatus & 4));
	}

	if ((!((vLocalSystemData->vSystemRunMode == cRunningSystemMode) && 
		(vLocalConfigurationData->vEnableEjectors))) && (vLocalSystemData->vSystemRunMode != cAutoSetupRunningSystem)) //turn on while in setup so conveyor or pump runs if interlocked
		vGlobaluCSerialPort->TurnAlarmOnOff(4,false);  //turn off System Inspecting output if tested
	Sleep(1); //is the equivelent of a yeild statement;
}

void CDiagnosticLineDialog::GetOScopeDataFromGrabber()
{
	vOldDiagnosticLineValue = vLocalSystemData->vDiagnosticLineValue;
	vLocalSystemData->vDiagnosticLineValue = (BYTE)vLocalSystemData->vITIPCDig->vCaptureModule->InportVal(0);

	//bit 0 to 5 are the selectable oscilloscope data from the uController
	//bit 6 of the parallel input data is the LVDS clock from the uController
	//bit 7 of the parallel input data is the LVDS to CamLink reciever board local clock

	if (vLocalConfigurationData->vDiagnosticLineSelect[3] > 100)
	{
		if (vLocalConfigurationData->vDiagnosticLineSelect[3] == 101)  //copy bit 6 data to bit 3 also
		{
			BYTE TempData = ((vLocalSystemData->vDiagnosticLineValue >> 3) & 0x08);
			if (TempData)
				vLocalSystemData->vDiagnosticLineValue = vLocalSystemData->vDiagnosticLineValue | TempData; //bit was set in bit 6, so set it in bit 3
			else
				vLocalSystemData->vDiagnosticLineValue = vLocalSystemData->vDiagnosticLineValue & (255 - TempData); //bit was cleared in bit 6, so clear it in bit 3
		}
		if (vLocalConfigurationData->vDiagnosticLineSelect[3] == 102)  //copy bit 7 data to bit 3 also
		{
			BYTE TempData = ((vLocalSystemData->vDiagnosticLineValue >> 4) & 0x08);
			if (TempData)
				vLocalSystemData->vDiagnosticLineValue = vLocalSystemData->vDiagnosticLineValue | TempData; //bit was set in bit 7, so set it in bit 3
			else
				vLocalSystemData->vDiagnosticLineValue = vLocalSystemData->vDiagnosticLineValue & (255 - TempData); //bit was cleared in bit 7, so clear it in bit 3
		}
	}

	if (vLocalConfigurationData->vDiagnosticLineSelect[4] > 100)
	{
		if (vLocalConfigurationData->vDiagnosticLineSelect[4] == 101)  //copy bit 6 data to bit 3 also
		{
			BYTE TempData = ((vLocalSystemData->vDiagnosticLineValue >> 2) & 0x10);
			if (TempData)
				vLocalSystemData->vDiagnosticLineValue = vLocalSystemData->vDiagnosticLineValue | TempData; //bit was set in bit 6, so set it in bit 3
			else
				vLocalSystemData->vDiagnosticLineValue = vLocalSystemData->vDiagnosticLineValue & (255 - TempData);//Take Two's Compliment //bit was cleared in bit 6, so clear it in bit 3
		}
		if (vLocalConfigurationData->vDiagnosticLineSelect[4] == 102)  //copy bit 7 data to bit 3 also
		{
			BYTE TempData = ((vLocalSystemData->vDiagnosticLineValue >> 3) & 0x10);
			if (TempData)
				vLocalSystemData->vDiagnosticLineValue = vLocalSystemData->vDiagnosticLineValue | TempData; //bit was set in bit 7, so set it in bit 3
			else
				vLocalSystemData->vDiagnosticLineValue = vLocalSystemData->vDiagnosticLineValue & (255 - TempData); //bit was cleared in bit 7, so clear it in bit 3
		}
	}

	if (vLocalConfigurationData->vDiagnosticLineSelect[5] > 100)
	{
		if (vLocalConfigurationData->vDiagnosticLineSelect[5] == 101)  //copy bit 6 data to bit 3 also
		{
			BYTE TempData = ((vLocalSystemData->vDiagnosticLineValue >> 1) & 0x20);
			if (TempData)
				vLocalSystemData->vDiagnosticLineValue = vLocalSystemData->vDiagnosticLineValue | TempData; //bit was set in bit 6, so set it in bit 3
			else
				vLocalSystemData->vDiagnosticLineValue = vLocalSystemData->vDiagnosticLineValue & (255 - TempData); //bit was cleared in bit 6, so clear it in bit 3
		}
		if (vLocalConfigurationData->vDiagnosticLineSelect[5] == 102)  //copy bit 7 data to bit 3 also
		{
			BYTE TempData = ((vLocalSystemData->vDiagnosticLineValue >> 2) & 0x20);
			if (TempData)
				vLocalSystemData->vDiagnosticLineValue = vLocalSystemData->vDiagnosticLineValue | TempData; //bit was set in bit 7, so set it in bit 3
			else
				vLocalSystemData->vDiagnosticLineValue = vLocalSystemData->vDiagnosticLineValue & (255 - TempData); //bit was cleared in bit 7, so clear it in bit 3
		}
	}

	if ((vOldDiagnosticLineValue & 0x40) != (vLocalSystemData->vDiagnosticLineValue & 0x40))
		vLVDSActive = 1;

	if ((vOldDiagnosticLineValue & 0x80) != (vLocalSystemData->vDiagnosticLineValue & 0x840))
		vCamLinkActive = 1;
}



void CDiagnosticLineDialog::OnDialogtitlestatictext1()
{
	BYTE TempHiddenKeys = 8;
	if (!PasswordOK(cTemporaryInspxPassword,false))
		TempHiddenKeys = 10;
	PostThreadMessage(vGlobalCallingThreadID, cDisplaySoftKeyMenu, TempHiddenKeys, TempHiddenKeys);
}


void CDiagnosticLineDialog::OnStnClickedSubfunction0button()
{
}

void CDiagnosticLineDialog::OnStnClickedSubfunction0display()
{
	CNumericEntryDialog INumericEntryDialog;  
	
	INumericEntryDialog.vEditString = dtoa(vEjectorToChange, 0);

	//Set dialog box data titles and number value
	INumericEntryDialog.m_DialogTitleStaticText1 = "Enter Ejector to Turn On or Off";
	INumericEntryDialog.m_DialogTitleStaticText2 = "Used for Testing Only";
	INumericEntryDialog.m_UnitsString = "";

	INumericEntryDialog.m_DialogTitleStaticText3 = "";
	INumericEntryDialog.m_DialogTitleStaticText4 = "";

	INumericEntryDialog.vMaxValue = cNumberOfEjectorsForUser;
	INumericEntryDialog.vMinValue = 1;
	INumericEntryDialog.vIntegerOnly = true;
	INumericEntryDialog.vAllowZero = false;
	//Pass control to dialog box and display
	int nResponse = INumericEntryDialog.DoModal();
	//dialog box is now closed, if user pressed select do this
	//if user pressed cancel, do nothing
	if (nResponse == IDOK)
	{
		vEjectorToChange = (DWORD)ATOF(INumericEntryDialog.vEditString);
		CYesNoDialog TempYesNoDialog;
		TempYesNoDialog.vNoticeText = _T("\n\n\nTurn Ejector On or Off?");
		TempYesNoDialog.vQuestionType = cQuestion;
		TempYesNoDialog.vYesButtonText = "Turn Ejector On";
		TempYesNoDialog.vNoButtonText = "Turn Ejector Off";
		int TempResult = TempYesNoDialog.DoModal();
		//if user said OK
		if (TempResult == IDOK)
			vGlobaluCSerialPort->SendEjectorOnOffCommand(vEjectorToChange, true);
		else
			vGlobaluCSerialPort->SendEjectorOnOffCommand(vEjectorToChange, false);
	}
}
