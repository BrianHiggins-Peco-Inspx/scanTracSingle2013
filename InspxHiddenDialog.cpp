//ScanTrac Side View Source File
// InspxHiddenDialog.cpp : implementation file
//

#include "stdafx.h"
#include "ScanTrac.h"
#include "InspxHiddenDialog.h"
#include "NumericEntryDialog.h"
#include "SelectItemDialog.h"
#include "NoticeDialog.h"
#include "AlphaEntryDialog.h"
#include "SystemConfigurationData.h"
#include "ACPowerMoniter.h"
#include "SelectMultipleItemDialog.h"
#include "YesNoDialog.h"
#include "SerialPortGuardMaster.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CSerialPortGuardMaster *vGlobalGuardMaster;
extern BYTE vGlobalScanTracType;
extern BYTE vGlobalShiftKeyDown;
extern BYTE vGlobalDemoMode;
extern CACPowerMoniter * vGlobalACPowerMoniter;
extern unsigned long vGlobalDialogTitleColor;
/////////////////////////////////////////////////////////////////////////////
// CInspxHiddenDialog dialog


CInspxHiddenDialog::CInspxHiddenDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CInspxHiddenDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInspxHiddenDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	vUpdateDisplayTimerHandle = 982;
	vChangeMade = false;
	vShowingMouseCursor = false;
	vAllowClientConnections = false;
	vEnableModBusTCPIPInterface = false;
	vScanTracType = 0;	//0 = solo, 1 = piccolo, 2 = legato 3 = 600 Watt Pipeline
	//vRampXRayDurationString = "0";

	vLocalCWndCollection.Add(this);
	vLocalCWndCollection.Add(&m_SubFunction4Display);
	vLocalCWndCollection.Add(&m_Function4Button);
	vLocalCWndCollection.Add(&m_Function2Button);
	vLocalCWndCollection.Add(&m_Background);
	vLocalCWndCollection.Add(&m_AutoXRaySourceOffEnabled);
	vLocalCWndCollection.Add(&m_BlankExtraLines);
	vLocalCWndCollection.Add(&m_DemoMode);
	vLocalCWndCollection.Add(&m_DialogTitleStaticText);
	vLocalCWndCollection.Add(&m_Function1Button);
	vLocalCWndCollection.Add(&m_Function3Button);
	vLocalCWndCollection.Add(&m_Function3Display);
	vLocalCWndCollection.Add(&m_Function5Button);
	vLocalCWndCollection.Add(&m_Function5Display);
	vLocalCWndCollection.Add(&m_SimulateLightControlBoard);
	vLocalCWndCollection.Add(&m_SubFunction1Button);
	vLocalCWndCollection.Add(&m_SubFunction2Button);
	vLocalCWndCollection.Add(&m_SubFunction3Button);
	vLocalCWndCollection.Add(&m_SubFunction4Button);
	vLocalCWndCollection.Add(&m_SubFunction5Button);
	vLocalCWndCollection.Add(&m_SubFunction6Button);
	vLocalCWndCollection.Add(&m_SubFunction6Display);
	vLocalCWndCollection.Add(&m_SubFunction7Button);
	vLocalCWndCollection.Add(&m_SubFunction8Button);
	vLocalCWndCollection.Add(&m_SubFunction9Button);
	vLocalCWndCollection.Add(&m_SubFunctionDisplay3);
	vLocalCWndCollection.Add(&m_SubFunction6DisplayA);
	vLocalCWndCollection.Add(&m_SuppressWarningMessages);
}

void CInspxHiddenDialog::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	m_Background.SetWindowPos(NULL,cMainBackgroundLeft,
		cMainBackgroundTop,cMainBackgroundLength,
		cMainBackgroundHeight,SWP_NOZORDER);

	vAllowClientConnections = vLocalConfigurationData->vAllowClientConnections;
	vEnableModBusTCPIPInterface = vLocalConfigurationData->vEnableModBusTCPIPInterface;
	vScanTracType = vLocalConfigurationData->vScanTracType;
	vGlobalScanTracType = vLocalConfigurationData->vScanTracType;
	vSuppressWarningMessages = vLocalConfigurationData->vSuppressWarningMessages;
	
	vSimulateLightControlBoard = vLocalConfigurationData->vSimulateLightControlBoard;
	vHVPSType = vLocalConfigurationData->vHVPSType;

	vEnableACMessages = vLocalConfigurationData->vEnableACMessages;

	vShowingMouseCursor = vLocalConfigurationData->vShowMouseCursor;

	vDemoMode = vLocalConfigurationData->vDemoMode;

	vAutoXRaySourceOffEnabled = vLocalConfigurationData->vAutoXRaySourceOffEnabled;

	if (vShowingMouseCursor)
		SetDlgItemText(IDC_SubFunction1Button, _T("Turn Off Mouse Cursor!"));
	else
		SetDlgItemText(IDC_SubFunction1Button, _T("Turn On Mouse Cursor!"));

	UpdateDisplay();
	UpdateModBusDisplay();

	int TempTimerResult = (int)SetTimer(vUpdateDisplayTimerHandle, 1000, NULL);
	this->SetWindowText(_T("Hardware"));

	::SetupMenu(vLocalCWndCollection);
}

void CInspxHiddenDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInspxHiddenDialog)
	DDX_Control(pDX, IDC_SubFunction4Display, m_SubFunction4Display);
	DDX_Control(pDX, IDC_Function4Button, m_Function4Button);
	DDX_Control(pDX, IDC_Function2Button, m_Function2Button);
	DDX_Control(pDX, IDC_Background, m_Background);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_AutoXRaySourceOffEnabled, m_AutoXRaySourceOffEnabled);
	DDX_Control(pDX, IDC_BlankExtraLines, m_BlankExtraLines);
	DDX_Control(pDX, IDC_DemoMode, m_DemoMode);
	DDX_Control(pDX, IDC_DialogTitleStaticText1, m_DialogTitleStaticText);
	DDX_Control(pDX, IDC_Function1Button, m_Function1Button);
	DDX_Control(pDX, IDC_Function3Button, m_Function3Button);
	DDX_Control(pDX, IDC_Function3Display, m_Function3Display);
	DDX_Control(pDX, IDC_Function5Button, m_Function5Button);
	DDX_Control(pDX, IDC_Function5Display, m_Function5Display);
	DDX_Control(pDX, IDC_SimulateLightControlBoard, m_SimulateLightControlBoard);
	DDX_Control(pDX, IDC_SubFunction1Button, m_SubFunction1Button);
	DDX_Control(pDX, IDC_SubFunction2Button, m_SubFunction2Button);
	DDX_Control(pDX, IDC_SubFunction3Button, m_SubFunction3Button);
	DDX_Control(pDX, IDC_SubFunction4Button, m_SubFunction4Button);
	DDX_Control(pDX, IDC_SubFunction5Button, m_SubFunction5Button);
	DDX_Control(pDX, IDC_SubFunction6Button, m_SubFunction6Button);
	DDX_Control(pDX, IDC_SubFunction6Display, m_SubFunction6Display);
	DDX_Control(pDX, IDC_SubFunction7Button, m_SubFunction7Button);
	DDX_Control(pDX, IDC_SubFunction8Button, m_SubFunction8Button);
	DDX_Control(pDX, IDC_SubFunctionDisplay3, m_SubFunctionDisplay3);
	DDX_Control(pDX, IDC_SuppressWarningMessages, m_SuppressWarningMessages);
	DDX_Control(pDX, IDC_SubFunction9Button, m_SubFunction9Button);
	DDX_Control(pDX, IDC_SubFunction6DisplayA, m_SubFunction6DisplayA);
}


BEGIN_MESSAGE_MAP(CInspxHiddenDialog, CDialog)
	//{{AFX_MSG_MAP(CInspxHiddenDialog)
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
	ON_WM_SHOWWINDOW()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
	ON_STN_CLICKED(IDC_DialogTitleStaticText1, &CInspxHiddenDialog::OnDialogtitlestatictext1)
	ON_STN_CLICKED(IDC_SubFunction9Button, &CInspxHiddenDialog::OnStnClickedSubfunction9button)
	ON_WM_TIMER()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInspxHiddenDialog message handlers

void CInspxHiddenDialog::OnFunction1Button() 
{
	//Main Menu Button was pressed
	if ((!vChangeMade) || (AbortSetupChanges()))
		CDialog::EndDialog(10);
}

void CInspxHiddenDialog::OnFunction2Button() 
{
	// Save and Exit Button was pressed
	if ((((vScanTracType == cPiccoloScanTrac) || (vScanTracType == cLegatoScanTrac)) && (vHVPSType != cMNXHVPS) && (!vSimulateLightControlBoard)) && (!PasswordOK(cSuperInspxPassword,false)))
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\nMNX HVPS is required for Piccolo or Legato ScanTracs.";
		TempNoticeDialog.vType = cErrorMessage;
		TempNoticeDialog.DoModal();
	}
	else
	if ((((vScanTracType == cAtempoScanTrac) || (vScanTracType == cForteScanTrac) || (vScanTracType == cCaseInspectorScanTrac)) && 
		((vHVPSType != c210WMonoBlockHVPS) && (vHVPSType != c100WMonoBlockHVPS) && (vHVPSType != cCheyneyMonoBlock) && (vHVPSType != c100WVJTMonoBlockHVPS)) && (!vSimulateLightControlBoard)) && (!PasswordOK(cSuperInspxPassword,false)))
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\nMonoBlock HVPS is required for Atempo, Forte, and Largo ScanTracs.";
		TempNoticeDialog.vType = cErrorMessage;
		TempNoticeDialog.DoModal();
	}
	else
	if ((((vScanTracType == cBriosoScanTrac) || (vScanTracType == cAllegroScanTrac)) && (vHVPSType != c100WMonoBlockHVPS) && (vHVPSType != cCheyneyMonoBlock) && (vHVPSType != c100WVJTMonoBlockHVPS) && (!vSimulateLightControlBoard)) && (!PasswordOK(cSuperInspxPassword,false)))
	{
		CNoticeDialog TempNoticeDialog;
		if (vScanTracType == cAllegroScanTrac)
			TempNoticeDialog.vNoticeText = "\nXRB100W HVPS is required for Allegro ScanTrac.";
		else
			TempNoticeDialog.vNoticeText = "\nXRB100W HVPS is required for Brioso ScanTrac.";
		TempNoticeDialog.vType = cErrorMessage;
		TempNoticeDialog.DoModal();
	}
	else
	if ((((vScanTracType == cSoloScanTrac) || (vScanTracType == cFermataScanTrac) || (vScanTracType == cUpSideDownSoloScanTrac) || (vLocalConfigurationData->vScanTracType == cSoloPlusScanTrac)) && 
		((vHVPSType != cXLFHVPS) && (vHVPSType != cDXMHVPS)) && (!vSimulateLightControlBoard)) && (!PasswordOK(cSuperInspxPassword,false)))
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\nXLF or DXM HVPS is required for Solo and Fermata ScanTracs.";
		TempNoticeDialog.vType = cErrorMessage;
		TempNoticeDialog.DoModal();
	}
	else
	if ((!vChangeMade) || (KeepSetupChanges()))
	{
		if (vLocalConfigurationData->vHVPSType != vHVPSType)
		{
			vLocalConfigurationData->vHVPSType = vHVPSType;
			switch (vLocalConfigurationData->vHVPSType)
			{
				case cDXMHVPS: 
						vLocalConfigurationData->vMaximumPower = 600;
						vLocalConfigurationData->vMaximumVoltage = 70;
						vLocalConfigurationData->vMaximumCurrent = 8.55;
				break;
				case c100WMonoBlockHVPS: 
						vLocalConfigurationData->vMaximumPower = 100;
						vLocalConfigurationData->vMaximumVoltage = 80;
						vLocalConfigurationData->vMaximumCurrent = 2;
				break;
				case cCheyneyMonoBlock:
						vLocalConfigurationData->vMaximumPower = 210;
						vLocalConfigurationData->vMaximumVoltage = 80;
						vLocalConfigurationData->vMaximumCurrent = 2;
				break;
				case c100WVJTMonoBlockHVPS:
						vLocalConfigurationData->vMaximumPower = 100;
						vLocalConfigurationData->vMaximumVoltage = 80;
						vLocalConfigurationData->vMaximumCurrent = 2.5;
				break;
				case c210WMonoBlockHVPS: 
						vLocalConfigurationData->vMaximumPower = 210;
						vLocalConfigurationData->vMaximumVoltage = 80;
						vLocalConfigurationData->vMaximumCurrent = 6;
				break;
				case cXLFHVPS: 
						vLocalConfigurationData->vMaximumPower = 480;
						vLocalConfigurationData->vMaximumVoltage = 60;
						vLocalConfigurationData->vMaximumCurrent = 8;
				break;
				case cMNXHVPS: 
						vLocalConfigurationData->vMaximumPower = 50;
						vLocalConfigurationData->vMaximumVoltage = 50;
						vLocalConfigurationData->vMaximumCurrent = 2;
				break;
			}
		}
		vLocalConfigurationData->vAllowClientConnections = vAllowClientConnections;

		if (vLocalConfigurationData->vScanTracType != vScanTracType)
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\nAfter Saving and Exiting and returning to the Main Menu, you must Exit the ScanTrac Program, and restart it\nto effect the change in the system settings.";
			TempNoticeDialog.vType = cNoticeMessage;
			TempNoticeDialog.DoModal();

			vLocalConfigurationData->vScanTracType = vScanTracType;

			BYTE TempNumberOfXRayDetectors = vLocalConfigurationData->vNumberOfXRayDetectors;
			
			vLocalConfigurationData->vDetectorLength = 2;
			if (vScanTracType == cCaseInspectorScanTrac)
			{
				vLocalConfigurationData->vNumberOfXRayDetectors = 9;
				vLocalConfigurationData->vDetectorLength = 4;
				vGlobalPixelsPerUnitInHeight = 16;
				if (vGlobalPixelsPerUnit > 25)
				{
					vGlobalPixelsPerUnit = 16;
					vLocalConfigurationData->vPixelsPerUnit = 16;
				}
			}
			else
			{
				if (vLocalConfigurationData->vPixelsPerDetector == 128) //uses .4mm detectors which are 128 pixels in 2 inches
					vGlobalPixelsPerUnitInHeight = 64;
				else
					vGlobalPixelsPerUnitInHeight = 32; //uses .8mm detectors which are 64 pixels in 2 inches

				if (vGlobalPixelsPerUnit < 25)
				{
					vGlobalPixelsPerUnit = 32;
					vLocalConfigurationData->vPixelsPerUnit = 32;
				}
				
				if (vScanTracType == cForteScanTrac)
					vLocalConfigurationData->vNumberOfXRayDetectors = 12;
				else
				if (vScanTracType == cAllegroScanTrac)
				{
					vLocalConfigurationData->vNumberOfXRayDetectors = 9;
					//vLocalConfigurationData->vNumberOfXRayDetectorsToReadInImage = 12;
					
				}
				else
					vLocalConfigurationData->vNumberOfXRayDetectors = 6;
			}

			if (vLocalConfigurationData->vNumberOfXRayDetectors != TempNumberOfXRayDetectors)
			{
				vLocalSystemData->vCurrentProductChanged = true; //must do, reloads the product
				CNoticeDialog TempNoticeDialog;
				CString TempText("\nYou must delete all the Inspx Products,\nand Exit the ScanTrac program to initialize properly\nfor " + dtoa(vLocalConfigurationData->vNumberOfXRayDetectors, 0) + " detectors");
				TempNoticeDialog.vNoticeText = TempText;
				TempNoticeDialog.vType = cNoticeMessage;
				TempNoticeDialog.DoModal();
			}
			vLocalConfigurationData->vSystemEjectorPositionOffset = 0; //changed ScanTrac types, so clear offsets
			vLocalConfigurationData->vSystemBodyTriggerToXRayBeam = 0; //changed ScanTrac types, so clear offsets
			if ((vScanTracType == cAtempoScanTrac) || (vScanTracType == cForteScanTrac) || (vScanTracType == cAllegroScanTrac) ||(vScanTracType == cCaseInspectorScanTrac) || 
				(vScanTracType == cBriosoScanTrac)) 
				vLocalConfigurationData->vNumberOfQuickLearnImagesToTake = 1;

			if ((vScanTracType != cAtempoScanTrac) && (vScanTracType != cForteScanTrac)) //Only TDs can use motor signal
			{
				vLocalConfigurationData->vUseRealEncoder = true;
			}

			if ((vScanTracType != cLegatoScanTrac) && (vScanTracType != cFermataScanTrac)) //not a pipeline scantrac
			{
				vLocalConfigurationData->vPipeLineFlowRate = 0;
				vLocalConfigurationData->vUsePipeFlowInput = 0;
			}
			if ((vScanTracType == cLegatoScanTrac) || (vScanTracType == cFermataScanTrac) || (vScanTracType == cAtempoScanTrac) || (vScanTracType == cForteScanTrac) || 
				(vScanTracType == cAllegroScanTrac) || (vScanTracType == cCaseInspectorScanTrac)) //is a pipeline scantrac turn off slip checking
			{
				vLocalConfigurationData->vShapeHandlingProcessIfAtSides = 0;  //disable slip checking
				//vLocalConfigurationData->vShapeHandlingProcessIfAtTop = 0;
				vLocalConfigurationData->vShapeHandlingProcessIfConveyorStops = 0;
				vLocalConfigurationData->vShapeHandlingProcessIfWrongWidth = 0;
				vLocalConfigurationData->vShapeHandlingProcessIfWrongHeight = 0;
				vLocalConfigurationData->vRadiationShutterInSystem = 0; //disable shutter
				vLocalConfigurationData->vEncoderTolerance = 0;
				vLocalConfigurationData->vNumberOfEncoderRateReversalsForError = 0;
				vLocalSystemData->vContainerString = "Image";
				if ((vScanTracType == cLegatoScanTrac) || (vScanTracType == cFermataScanTrac)) //is a pipeline scantrac turn off slip checking
					vLocalConfigurationData->vSystemBodyTriggerToXRayBeam = cForteTriggerToBeamOffset;
				
				vLocalConfigurationData->vDriftCompensationEnabled = false;
				vLocalConfigurationData->vAdjustImageIntensity = false;
				vLocalConfigurationData->vMaxImageIntensityAdjust = 0;
				vLocalConfigurationData->vMaxImageIntensityAdjustBeforeLocked = 0;  
			}
			else  
			{//is a Side View ScanTrac with a rigid container, or Brioso web fed product
				if (vScanTracType == cBriosoScanTrac)				
					vLocalConfigurationData->vShapeHandlingProcessIfAtSides = 0;  //disable slip checking
				else
					vLocalConfigurationData->vShapeHandlingProcessIfAtSides = 2;  //enable slip checking
				vLocalSystemData->vContainerString = "Container";
				//vLocalConfigurationData->vShapeHandlingProcessIfAtTop = 0;
				vLocalConfigurationData->vShapeHandlingProcessIfConveyorStops = 0;
				vLocalConfigurationData->vShapeHandlingProcessIfWrongWidth = 0;
				vLocalConfigurationData->vShapeHandlingProcessIfWrongHeight = 0;
				if (vScanTracType == cBriosoScanTrac)				
					vLocalConfigurationData->vRadiationShutterInSystem = 0; //disable shutter
				else
					vLocalConfigurationData->vRadiationShutterInSystem = 1; //enable shutter
				vLocalConfigurationData->vContinuousFeedEncoderRate = 0;  //turn off continuous feed encoder and conveyor
				vLocalConfigurationData->vContinuousFeedContainerRate = 0;
				vLocalConfigurationData->vBulkProductMode = false;
				
				vLocalConfigurationData->vDriftCompensationEnabled = true;
				vLocalConfigurationData->vAdjustImageIntensity = true;
				vLocalConfigurationData->vMaxImageIntensityAdjust = 20;
				vLocalConfigurationData->vMaxImageIntensityAdjustBeforeLocked = 50;  
			}
		}
		vLocalConfigurationData->vEnableModBusTCPIPInterface = vEnableModBusTCPIPInterface;
		vLocalConfigurationData->vSimulateLightControlBoard = vSimulateLightControlBoard;
		vLocalConfigurationData->vSuppressWarningMessages = vSuppressWarningMessages;
		//vLocalConfigurationData->vRampXRayDuration = _wtoi(vRampXRayDurationString);
		if ( vLocalConfigurationData->vEnableACMessages != vEnableACMessages )
		{
			vGlobalACPowerMoniter->vGaveACVoltageWarning = 0;
			vLocalConfigurationData->vEnableACMessages = vEnableACMessages;
		}
		vLocalConfigurationData->vAutoXRaySourceOffEnabled = vAutoXRaySourceOffEnabled;
		vLocalConfigurationData->vDemoMode = vDemoMode;
		vGlobalDemoMode = vDemoMode;
		CDialog::EndDialog(true);
	}
}

void CInspxHiddenDialog::OnFunction3Button() 
{
	vHVPSType++;
	if (vHVPSType > c100WVJTMonoBlockHVPS)
		vHVPSType = cXLFHVPS;

	CNoticeDialog TempNoticeDialog;
	TempNoticeDialog.vNoticeText = "\nAfter Saving and Exiting and returning to the Main Menu, you must Exit the ScanTrac Program, and restart it\nto effect the change in X-Ray HVPS selection.";
	TempNoticeDialog.vType = cNoticeMessage;
	TempNoticeDialog.DoModal();

	SetChangeMade();
	UpdateDisplay();
}

void CInspxHiddenDialog::OnFunction4Button() 
{
	//Cancel was pressed
	if (vChangeMade)
	if ((!vChangeMade) || (AbortSetupChanges()))
		CDialog::OnCancel();
}

void CInspxHiddenDialog::OnFunction5Button() 
{
	//choose which error messages to disable and enable AC Power Monitor message warnings

	CSelectMultipleItemDialog ISelectItemDialog;  
			
	//Set dialog box data titles and number value
	ISelectItemDialog.m_DialogTitleStaticText = "Select AC Power Monitor Messages to Enable";
	ISelectItemDialog.vNumberOfItems = 15;
	ISelectItemDialog.vInAutoSetup = false;
	ISelectItemDialog.vUseSelectText = false;
	//ISelectItemDialog.vColor = cGreen;

	ISelectItemDialog.vItems[0] = !(vEnableACMessages & 0x01); //bitmask to show first bit, active low
	ISelectItemDialog.vItemsYes[0] = "Selected-Dropout Message Enabled";
	ISelectItemDialog.vItemsNo[0] = "Not Selected-Dropout Message Disbled";

	ISelectItemDialog.vItems[4] = !(vEnableACMessages & 0x1000);
	if (vLocalConfigurationData->vUse101VACFor10SecondsShutDownLimit)
	{
		ISelectItemDialog.vItemsYes[4] = "Selected-Brownout (<101) Message Enabled";
		ISelectItemDialog.vItemsNo[4] = "Not Selected-Brownout (<101) Message Disabled";
	}
	else
	{
		ISelectItemDialog.vItemsYes[4] = "Selected-Brownout (<106) Message Enabled";
		ISelectItemDialog.vItemsNo[4] = "Not Selected-Brownout (<106) Message Disabled";
	}

	ISelectItemDialog.vItems[1] = !(vEnableACMessages & 0x02);
	ISelectItemDialog.vItemsYes[1] = "Selected-AC Sag (<95) Message Enabled";
	ISelectItemDialog.vItemsNo[1] = "Not Selected-AC Sag (<95) Message Disabled";

	ISelectItemDialog.vItems[5] = !(vEnableACMessages & 0x04);
	ISelectItemDialog.vItemsYes[5] = "Selected-Voltage Surge (>135) Message Enabled";
	ISelectItemDialog.vItemsNo[5] = "Not Selected-Voltage Surge (>135) Message Disabled";

	ISelectItemDialog.vItems[2] = !(vEnableACMessages & 0x008);
	ISelectItemDialog.vItemsYes[2] = "Selected-Hot/Neut. Reversed Message Enabled";
	ISelectItemDialog.vItemsNo[2] = "Not Selected-Hot/Neut. Reversed Message Disabled";

	ISelectItemDialog.vItems[6] = !(vEnableACMessages & 0x0010);
	ISelectItemDialog.vItemsYes[6] = "Selected-5V Spike on Hot Message Enabled";
	ISelectItemDialog.vItemsNo[6] = "Not Selected-5V Spike on Hot Message Disabled";

	ISelectItemDialog.vItems[3] = !(vEnableACMessages & 0x0020);
	ISelectItemDialog.vItemsYes[3] = "Selected-50V Spike on Hot Message Enabled";
	ISelectItemDialog.vItemsNo[3] = "Not Selected-50V Spike on Hot Message Disabled";

	ISelectItemDialog.vItems[7] = !(vEnableACMessages & 0x0040);
	ISelectItemDialog.vItemsYes[7] = "Selected-100V Spike on Hot Message Enabled";
	ISelectItemDialog.vItemsNo[7] = "Not Selected-100V Spike on Hot Message Disabled";

	ISelectItemDialog.vItems[8] = !(vEnableACMessages & 0x0080);
	ISelectItemDialog.vItemsYes[8] = "Selected-500V Spike on Hot Message Enabled";
	ISelectItemDialog.vItemsNo[8] = "Not Selected-500V Spike on Hot Message Disabled";

	ISelectItemDialog.vItems[12] = !(vEnableACMessages & 0x0100);
	ISelectItemDialog.vItemsYes[12] = "Selected-5V Spike on Neut. Message Enabled";
	ISelectItemDialog.vItemsNo[12] = "Not Selected-5V Spike on Neut. Message Disabled";

	ISelectItemDialog.vItems[9] = !(vEnableACMessages & 0x0200);
	ISelectItemDialog.vItemsYes[9] = "Selected-50V Spike on Neut. Message Enabled";
	ISelectItemDialog.vItemsNo[9] = "Not Selected-50V Spike on Neut. Message Disabled";

	ISelectItemDialog.vItems[10] = !(vEnableACMessages & 0x0400);
	ISelectItemDialog.vItemsYes[10] = "Selected-100V Spike on Neut. Message Enabled";
	ISelectItemDialog.vItemsNo[10] = "Not Selected-100V Spike on Neut. Message Disabled";

	ISelectItemDialog.vItems[11] = !(vEnableACMessages & 0x0800);
	ISelectItemDialog.vItemsYes[11] = "Selected-500V Spike on Neut. Message Enabled";
	ISelectItemDialog.vItemsNo[11] = "Not Selected-500V Spike on Neut. Message Disabled";

	ISelectItemDialog.vItems[13] = !(vEnableACMessages & 0x2000);
	ISelectItemDialog.vItemsYes[13] = "Selected-Open Ground Message Enabled";
	ISelectItemDialog.vItemsNo[13] = "Not Selected-Open Ground Message Disabled";

	ISelectItemDialog.vItems[14] = !(vEnableACMessages & 0x4000);
	ISelectItemDialog.vItemsYes[14] = "Selected-High Frequency Noise Enabled";
	ISelectItemDialog.vItemsNo[14] = "Not Selected-High Frequency Noise Disabled";

	//Pass control to dialog box and display
	int nResponse = ISelectItemDialog.DoModal();
	
	//dialog box is now closed, if user pressed select do this
	//if user pressed cancel, do nothing
	if (nResponse == IDOK)
	{
		vEnableACMessages = 0x7FFF;	//active low
		if (ISelectItemDialog.vItems[0])
			vEnableACMessages = vEnableACMessages ^ 0x01; //dropout
		if (ISelectItemDialog.vItems[4])
			vEnableACMessages = vEnableACMessages ^ 0x1000; //brownout < 106
		if (ISelectItemDialog.vItems[1])
			vEnableACMessages = vEnableACMessages ^ 0x02; //brownout < 95
		if (ISelectItemDialog.vItems[5])
			vEnableACMessages = vEnableACMessages ^ 0x04; //surge > 135
		if (ISelectItemDialog.vItems[2])
			vEnableACMessages = vEnableACMessages ^ 0x8; //hot/neut reversed
		if (ISelectItemDialog.vItems[6])
			vEnableACMessages = vEnableACMessages ^ 0x10; //5V on hot
		if (ISelectItemDialog.vItems[3])
			vEnableACMessages = vEnableACMessages ^ 0x20; //50v on hot
		if (ISelectItemDialog.vItems[7])
			vEnableACMessages = vEnableACMessages ^ 0x40; //100v on hot
		if (ISelectItemDialog.vItems[8])
			vEnableACMessages = vEnableACMessages ^ 0x80; //500v on hot
		if (ISelectItemDialog.vItems[12])
			vEnableACMessages = vEnableACMessages ^ 0x100; //5v on neut
		if (ISelectItemDialog.vItems[9])
			vEnableACMessages = vEnableACMessages ^ 0x200; //50v on neut
		if (ISelectItemDialog.vItems[10])
			vEnableACMessages = vEnableACMessages ^ 0x400; //100V on neut
		if (ISelectItemDialog.vItems[11])
			vEnableACMessages = vEnableACMessages ^ 0x800; //500V on neut
		if (ISelectItemDialog.vItems[13])
			vEnableACMessages = vEnableACMessages ^ 0x2000; //Open Ground
		if (ISelectItemDialog.vItems[14])
			vEnableACMessages = vEnableACMessages ^ 0x4000; //High Frequency Noise
	}

	SetChangeMade();
	UpdateDisplay();
}

void CInspxHiddenDialog::OnSubFunction1Button() 
{
	SetChangeMade();
	vShowingMouseCursor = !vShowingMouseCursor;

	vLocalConfigurationData->vShowMouseCursor = vShowingMouseCursor;
	vGlobalShowMouseCursor = vShowingMouseCursor;
	if (vShowingMouseCursor)
		int TempDisplayCursorCounter = ShowCursor(true);
	else
		int TempDisplayCursorCounter = ShowCursor(false);

	if (vShowingMouseCursor)
		SetDlgItemText(IDC_SubFunction1Button, _T("Turn Off Mouse Cursor!"));
	else
		SetDlgItemText(IDC_SubFunction1Button, _T("Turn On Mouse Cursor!"));
}

void CInspxHiddenDialog::OnSubFunction2Button() 
{
	// toggle auto X-Ray off enable button pressed
	SetChangeMade();
	if (vAutoXRaySourceOffEnabled == 1)
		vAutoXRaySourceOffEnabled = 2;
	else
		vAutoXRaySourceOffEnabled = 1;

	UpdateDisplay();
}

void CInspxHiddenDialog::OnSubFunction3Button() 
{
	vAllowClientConnections = !vAllowClientConnections;
	SetChangeMade();
	UpdateDisplay();
}

void CInspxHiddenDialog::OnSubFunction4Button() 
{
	vScanTracType++;
	if (vScanTracType > cSoloPlusScanTrac)
		vScanTracType = cSoloScanTrac;

	switch (vScanTracType)
	{
		case cSoloScanTrac: 
		case cSoloPlusScanTrac: 
		case cUpSideDownSoloScanTrac:
		case cFermataScanTrac:
			vHVPSType = cDXMHVPS;
		break;
		case cPiccoloScanTrac: 
		case cLegatoScanTrac:
			vHVPSType = cMNXHVPS;
		break;
		case cAtempoScanTrac: 
		case cForteScanTrac:
		case cCaseInspectorScanTrac:
			vHVPSType = c210WMonoBlockHVPS;
		break;
		case cAllegroScanTrac:
		case cBriosoScanTrac:
			vHVPSType = c100WMonoBlockHVPS;
		break;
	}
	//CNoticeDialog TempNoticeDialog;
	//TempNoticeDialog.vNoticeText = "\nAfter Saving and Exiting and returning to the Main Menu, you must Exit the ScanTrac Program, and restart it\nto effect the change in X-Ray HVPS selection.";
	//TempNoticeDialog.vType = cNoticeMessage;
	//TempNoticeDialog.DoModal();

	SetChangeMade();
	UpdateDisplay();
}

void CInspxHiddenDialog::OnSubFunction5Button() 
{
	vSuppressWarningMessages = !vSuppressWarningMessages;
	SetChangeMade();
	UpdateDisplay();
}

void CInspxHiddenDialog::OnSubFunction6Button() 
{  
	vEnableModBusTCPIPInterface = !vEnableModBusTCPIPInterface;
	SetChangeMade();
	UpdateDisplay();
}

void CInspxHiddenDialog::OnSubFunction7Button() 
{
	// Simulate Light control Board Button Pressed
	vSimulateLightControlBoard = !vSimulateLightControlBoard;
	SetChangeMade();
	UpdateDisplay();
}

void CInspxHiddenDialog::OnSubFunction8Button() 
{
	//toggle Demo Mode
	SetChangeMade();
	vDemoMode = !vDemoMode;
	UpdateDisplay();
}

HBRUSH CInspxHiddenDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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

	if (pWnd->GetDlgCtrlID() == IDC_Function3Display)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Function3Display, 5);  //5 is medium large
		if ((vHVPSType == cXLFHVPS) ||
			(((vScanTracType == cPiccoloScanTrac) || (vScanTracType == cLegatoScanTrac)) && (vHVPSType != cMNXHVPS)) ||
			(((vScanTracType == cSoloScanTrac) || (vScanTracType == cSoloPlusScanTrac) || (vScanTracType == cUpSideDownSoloScanTrac) || (vScanTracType == cFermataScanTrac)) && (vHVPSType != cDXMHVPS)) ||
			(((vScanTracType == cAtempoScanTrac) || (vScanTracType == cForteScanTrac) || (vScanTracType == cCaseInspectorScanTrac)) && (vHVPSType != c210WMonoBlockHVPS) && (vHVPSType != c100WVJTMonoBlockHVPS)) ||
			(((vScanTracType == cBriosoScanTrac) || (vScanTracType == cAllegroScanTrac)) && (vHVPSType != c100WMonoBlockHVPS) && (vHVPSType != cCheyneyMonoBlock) && (vHVPSType != c100WVJTMonoBlockHVPS)))
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cButtonTextColor);
			return vLocalSystemData->vYellowBrush;
		}
	}

	if (pWnd->GetDlgCtrlID() == IDC_SubFunction6DisplayA)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction6DisplayA, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBlack);
		return vLocalSystemData->vLightGreenBrush;
	}
	

	if (pWnd->GetDlgCtrlID() == IDC_SubFunctionDisplay3)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunctionDisplay3, 5); 
		if (pWnd->GetDlgCtrlID() == IDC_SubFunction6Display)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction6Display, 5); 
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction4Display)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction4Display, 5);

	if (pWnd->GetDlgCtrlID() == IDC_Function5Button)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function5Button, 5);  //5 is medium large
	if(vEnableACMessages)	//active low, so if any bits are high, some error messages are being ignored
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vLocalSystemData->vGreenBrush;
	}
	}
	if (pWnd->GetDlgCtrlID() == IDC_Function5Display)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function5Display, 5);  //5 is medium large
	if (vEnableACMessages & 0xECF)	// ignore 5 and 50 V Hot and 5 V Neutral.  active low, so if any bits are high, some error messages are being ignored
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vLocalSystemData->vYellowBrush;
	}
	}

	if (pWnd->GetDlgCtrlID() == IDC_SubFunction1Button)  
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction1Button, 5);  //5 is medium large
	if (vShowingMouseCursor)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vLocalSystemData->vGreenBrush;
	}
	}
	if (pWnd->GetDlgCtrlID() == IDC_SuppressWarningMessages)  
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SuppressWarningMessages, 5);  //5 is medium large
	if (vSuppressWarningMessages)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vLocalSystemData->vYellowBrush;
	}
	}

	if (pWnd->GetDlgCtrlID() == IDC_SubFunction5Button)  
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction5Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		if (vSuppressWarningMessages)
			return vLocalSystemData->vGreenBrush;
		else
			return vLocalSystemData->vYellowBrush;
	}

	if (pWnd->GetDlgCtrlID() == IDC_AutoXRaySourceOffEnabled)  // subfunction2
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_AutoXRaySourceOffEnabled, 5);  //5 is medium large
	if (vAutoXRaySourceOffEnabled != 1)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vLocalSystemData->vYellowBrush;
	}
	}

	if (pWnd->GetDlgCtrlID() == IDC_DemoMode)  
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_DemoMode, 5);  //5 is medium large
		if (vDemoMode)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cButtonTextColor);
			return vLocalSystemData->vYellowBrush;
		}
	}

	if (pWnd->GetDlgCtrlID() == IDC_SubFunction8Button)  
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction8Button, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		if (vDemoMode)
			return vLocalSystemData->vGreenBrush;
		else
			return vLocalSystemData->vYellowBrush;
	}

	if (pWnd->GetDlgCtrlID() == IDC_SubFunction9Button)  
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction9Button, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		if (vDemoMode)
			return vLocalSystemData->vGreenBrush;
		else
			return vLocalSystemData->vYellowBrush;
	}

	if (pWnd->GetDlgCtrlID() == IDC_SimulateLightControlBoard)  
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SimulateLightControlBoard, 5);
	if (vSimulateLightControlBoard)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vLocalSystemData->vYellowBrush;
	}
	}

	if (pWnd->GetDlgCtrlID() == IDC_SubFunction7Button)  
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction7Button, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		if (vSimulateLightControlBoard)
			return vLocalSystemData->vGreenBrush;
		else
			return vLocalSystemData->vYellowBrush;
	}

	if (pWnd->GetDlgCtrlID() == IDC_Background)
	{
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

BOOL CInspxHiddenDialog::PreTranslateMessage(MSG* pMsg) 
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
				PostThreadMessage(vGlobalCallingThreadID, cShowHelp, 31, 0);

				TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText1);
				if (TempWindow)
					TempWindow->Invalidate(false);
			break;
			case 27: // Escape
				::PostThreadMessage(vGlobalCallingThreadID,	cStartChatWindowMessage, 0,0);
			break;	
		}
		return true;  //true indicates it has been handled, so do not process
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CInspxHiddenDialog::SetChangeMade() 
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

void CInspxHiddenDialog::UpdateDisplay()
{
	if (vAutoXRaySourceOffEnabled == 1)
	{
		SetDlgItemText(IDC_AutoXRaySourceOffEnabled, _T("Auto X-Ray Off Enabled"));
		SetDlgItemText(IDC_SubFunction2Button, _T("Disable Auto X-Ray Off"));
	}
	else
	if (vAutoXRaySourceOffEnabled == 2)
	{
		SetDlgItemText(IDC_AutoXRaySourceOffEnabled, _T("Auto X-Ray Off Disabled"));
		SetDlgItemText(IDC_SubFunction2Button, _T("Enable Auto X-Ray Off"));
	}
	else
	{
		SetDlgItemText(IDC_AutoXRaySourceOffEnabled, _T("Auto X-Ray Off Invalid Value"));
		SetDlgItemText(IDC_SubFunction2Button, _T("Enable Auto X-Ray Off"));
	}

	if (vHVPSType == cXLFHVPS)
	{
		SetDlgItemText(IDC_Function3Display, _T("Using Spellman XLF HVPS and RTW MCD120 Tube"));
		SetDlgItemText(IDC_Function3Button, _T("Use DXM HVPS"));
	}
	else
	if (vHVPSType == cDXMHVPS)
	{
		SetDlgItemText(IDC_Function3Display, _T("Using Spellman DXM HVPS and RTW H3 Tube"));
		SetDlgItemText(IDC_Function3Button, _T("Use MNX HVPS"));
	}
	else
	if (vHVPSType == cMNXHVPS)
	{
		SetDlgItemText(IDC_Function3Display, _T("Using Spellman MNX HVPS and 50W Tube"));
		SetDlgItemText(IDC_Function3Button, _T("Use XRB210W "));
	}
	else
	if (vHVPSType == c210WMonoBlockHVPS)
	{
		SetDlgItemText(IDC_Function3Display, _T("Using Spellman XRB210W MonoBlock HVPS/Tube"));
		SetDlgItemText(IDC_Function3Button, _T("Use XRB100W"));
	}
	else
	if (vHVPSType == c100WMonoBlockHVPS)
	{
		SetDlgItemText(IDC_Function3Display, _T("Using Spellman XRB100W MonoBlock HVPS/Tube"));
		SetDlgItemText(IDC_Function3Button, _T("Use Cheyney MonoBlock"));
	}
	else
	if (vHVPSType == cCheyneyMonoBlock)
	{
		SetDlgItemText(IDC_Function3Display, _T("Using Cheyney MonoBlock HVPS/Tube"));
		SetDlgItemText(IDC_Function3Button, _T("Use VJT 100W MonoBlock"));
	}
	else
	if (vHVPSType == c100WVJTMonoBlockHVPS)
	{
		SetDlgItemText(IDC_Function3Display, _T("Using VJT 100W MonoBlock HVPS/Tube"));
		SetDlgItemText(IDC_Function3Button, _T("Use XLF HVPS"));
	}
	else
	{
		SetDlgItemText(IDC_Function3Display, _T("Unknown HVPS/Tube"));
		SetDlgItemText(IDC_Function3Button, _T("Use XLF HVPS"));
	}

	if (vLocalConfigurationData->vDetectedValidGuardMasterDevice)
	{
		m_SubFunction9Button.ShowWindow(SW_SHOW);
		SetDlgItemText(IDC_SubFunction9Button, _T("No Guard Master In ScanTrac"));
	}
	else
		m_SubFunction9Button.ShowWindow(SW_HIDE);

	if (vSuppressWarningMessages)
	{
		SetDlgItemText(IDC_SuppressWarningMessages, _T("Suppressing Warning Messages"));
		SetDlgItemText(IDC_SubFunction5Button, _T("Show Warning Messages"));
	}
	else
	{
		SetDlgItemText(IDC_SuppressWarningMessages, _T("Showing Warning Messages"));
		SetDlgItemText(IDC_SubFunction5Button, _T("Suppress Warning Messages"));
	}

	if (vEnableModBusTCPIPInterface)
	{
		CString TempText = "ModBus TCP/IP Interface Enabled";
		TempText = TempText + ", " + vLocalSystemData->vModBusTCPIPAddress;

		SetDlgItemText(IDC_SubFunction6Display, TempText);
		SetDlgItemText(IDC_SubFunction6Button, _T("Disable ModBus TCP/IP"));

		m_SubFunction6DisplayA.ShowWindow(SW_SHOW);
	}
	else
	{
		SetDlgItemText(IDC_SubFunction6Display, _T("ModBus TCP/IP Interface Disabled"));
		SetDlgItemText(IDC_SubFunction6Button, _T("Enable ModBus TCP/IP"));
		m_SubFunction6DisplayA.ShowWindow(SW_HIDE);
	}

	if (vAllowClientConnections)
	{
		SetDlgItemText(IDC_SubFunctionDisplay3, _T("Accepting Client Connections"));
		SetDlgItemText(IDC_SubFunction3Button, _T("Do Not Accept Clients"));
	}
	else
	{
		SetDlgItemText(IDC_SubFunctionDisplay3, _T("Not Accepting Client Connections"));
		SetDlgItemText(IDC_SubFunction3Button, _T("Accept Client Connection"));
	}

	if (vScanTracType == cSoloScanTrac)
	{
		SetDlgItemText(IDC_SubFunction4Display, _T("Solo-600 Watt SideView"));
		SetDlgItemText(IDC_SubFunction4Button, _T("Convert to Piccolo 50 Watt SV"));
	}
	else
	if (vScanTracType == cPiccoloScanTrac)
	{
		SetDlgItemText(IDC_SubFunction4Display, _T("Piccolo-50 Watt SideView"));
		SetDlgItemText(IDC_SubFunction4Button, _T("Convert to Legato 50W Pipe"));
	}
	else
	if (vScanTracType == cLegatoScanTrac)
	{
		SetDlgItemText(IDC_SubFunction4Display, _T("Legato-50 Watt PipeLine"));
		SetDlgItemText(IDC_SubFunction4Button, _T("Convert to Fermata 600W Pipe"));
	}
	else
	if (vScanTracType == cFermataScanTrac)
	{
		SetDlgItemText(IDC_SubFunction4Display, _T("Fermata-600 Watt Pipe"));
		SetDlgItemText(IDC_SubFunction4Button, _T("Convert to Atempo 12\" TD"));
	}
	else
	if (vScanTracType == cAtempoScanTrac)
	{
		SetDlgItemText(IDC_SubFunction4Display, _T("Atempo-210W 12 inch TD"));
		SetDlgItemText(IDC_SubFunction4Button, _T("Convert to Forte 24\" TD"));
	}
	else
	if (vScanTracType == cForteScanTrac)
	{
		SetDlgItemText(IDC_SubFunction4Display, _T("Forte-210W 24 inch TD"));
		SetDlgItemText(IDC_SubFunction4Button, _T("Convert to Largo 36\" TD"));
	}
	else
	if (vScanTracType == cCaseInspectorScanTrac)
	{
		SetDlgItemText(IDC_SubFunction4Display, _T("Largo-210W 36 inch TD"));
		SetDlgItemText(IDC_SubFunction4Button, _T("Convert to Brioso 100 W Web"));
	}
	else
	if (vScanTracType == cBriosoScanTrac)
	{
		SetDlgItemText(IDC_SubFunction4Display, _T("Brioso-100 Watt Web"));
		SetDlgItemText(IDC_SubFunction4Button, _T("Convert to Up Side Down Solo"));
	}
	else
	if (vScanTracType == cUpSideDownSoloScanTrac)
	{
		SetDlgItemText(IDC_SubFunction4Display, _T("Up Side Down Solo 600 Watt"));
		SetDlgItemText(IDC_SubFunction4Button, _T("Convert to Allegro 18\" TD"));
	}
	else
	if (vScanTracType == cAllegroScanTrac)
	{
		SetDlgItemText(IDC_SubFunction4Display, _T("Allegro-100W 18 inch TD"));
		SetDlgItemText(IDC_SubFunction4Button, _T("Convert to 600 Watt Solo Plus SideView"));
	}
	else
	if (vScanTracType == cSoloPlusScanTrac)
	{
		SetDlgItemText(IDC_SubFunction4Display, _T("Solo Plus-600 Watt SideView, 18\" tall"));
		SetDlgItemText(IDC_SubFunction4Button, _T("Convert to 600 Watt Solo SideView"));
	}
	else
	{
		SetDlgItemText(IDC_SubFunction4Display, _T("Unknown ScanTrac Type"));
		SetDlgItemText(IDC_SubFunction4Button, _T("Convert to 600 Watt Solo SideView"));
	}

	//9/30/2009
	if (vSimulateLightControlBoard)
	{
		SetDlgItemText(IDC_SimulateLightControlBoard, _T("Simulating Interlocks and X-Rays"));
		SetDlgItemText(IDC_SubFunction7Button, _T("Use Real Interlocks + X-Rays"));
	}
	else
	{
		SetDlgItemText(IDC_SimulateLightControlBoard, _T("Using Real Interlocks and X-Rays"));
		SetDlgItemText(IDC_SubFunction7Button, _T("Simulate Interlocks + X-Rays"));
	}

	if (vDemoMode)
	{
		SetDlgItemText(IDC_DemoMode, _T("Demo Mode Enabled"));
		SetDlgItemText(IDC_SubFunction8Button, _T("Disable Demo Mode"));
	}
	else
	{
		SetDlgItemText(IDC_DemoMode, _T("Demo Mode Disabled"));
		SetDlgItemText(IDC_SubFunction8Button, _T("Enable Demo Mode"));
	}

	if (vEnableACMessages == 0x7FFF)
	{
		SetDlgItemText(IDC_Function5Display, _T("All AC Voltage Yellow Messages Disabled"));
	}
	else 
	if (vEnableACMessages) //vEnableACMessages is active low, if any bits are high, some of the messages are disabled
	{
		SetDlgItemText(IDC_Function5Display, _T("Some AC Voltage Yellow Messages Disabled"));
	}
	else
	{
		SetDlgItemText(IDC_Function5Display, _T("All AC Voltage Yellow Messages Enabled"));
	}

	CWnd *TempWindow = CWnd::GetDlgItem(IDC_Function5Button);
	if (TempWindow)
		TempWindow->Invalidate(false);
}


void CInspxHiddenDialog::OnDialogtitlestatictext1()
{
	BYTE TempHiddenKeys = 8;
	if (!PasswordOK(cTemporaryInspxPassword,false))
		TempHiddenKeys = 10;
	PostThreadMessage(vGlobalCallingThreadID, cDisplaySoftKeyMenu, TempHiddenKeys, TempHiddenKeys);
}

void CInspxHiddenDialog::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	if (nIDEvent == vUpdateDisplayTimerHandle)
	{
		UpdateModBusDisplay();
	}
	CDialog::OnTimer(nIDEvent);
}

void CInspxHiddenDialog::UpdateModBusDisplay()
{
	if ((vOldModBusClientConnected != vLocalSystemData->vModBusClientConnected) || (vOldModBusSocketOpen != vLocalSystemData->vModBusSocketOpen) ||
		(vOldModBusClientConnectionCounts != vLocalSystemData->vModBusClientConnectionCounts) || (vOldModBusClientRequestCounts != vLocalSystemData->vModBusClientRequestCounts) ||
		(vLocalSystemData->vModBusTCPIPErrorCounter != vOldModBusTCPIPErrorCounter))
	{
		vOldModBusClientConnected = vLocalSystemData->vModBusClientConnected;
		vOldModBusSocketOpen = vLocalSystemData->vModBusSocketOpen;
		vOldModBusClientConnectionCounts = vLocalSystemData->vModBusClientConnectionCounts;
		vOldModBusClientRequestCounts = vLocalSystemData->vModBusClientRequestCounts;
		vOldModBusTCPIPErrorCounter = vLocalSystemData->vModBusTCPIPErrorCounter;

		CString TempText = "";
		if (vOldModBusClientConnected)
			TempText = TempText + "Connected";
		else
			if (vOldModBusSocketOpen)
				TempText = TempText + "Listening";
			else
				TempText = TempText + "Not Listening";

		TempText = TempText + ", Requests: " + dtoa(vOldModBusClientRequestCounts, 0);
		TempText = TempText + ", Errors: " + dtoa(vOldModBusTCPIPErrorCounter, 0);
		TempText = TempText + ", Connections: " + dtoa(vOldModBusClientConnectionCounts, 0);
		SetDlgItemText(IDC_SubFunction6DisplayA, TempText);
	}
}


void CInspxHiddenDialog::OnStnClickedSubfunction9button()
{
	if (vLocalConfigurationData->vDetectedValidGuardMasterDevice)
	{
		CYesNoDialog TempYesNoDialog;
		TempYesNoDialog.vNoticeText =  _T("\nForget that we have detected\na valid Guard Master Device?\nIf Guard Master is connected,\nit will detect it immediately.");
		TempYesNoDialog.vYesButtonText = "Forget Device";
		TempYesNoDialog.vNoButtonText = "Cancel";
		TempYesNoDialog.vQuestionType = cConfirmQuestion;
		int TempResult = TempYesNoDialog.DoModal();
		if (TempResult == IDOK)
		{
			vLocalConfigurationData->vDetectedValidGuardMasterDevice = false;
			if (vGlobalGuardMaster)
				vGlobalGuardMaster->vDetectedValidGuardMasterDevice = false;
				
			UpdateDisplay();
		}
	}
}
