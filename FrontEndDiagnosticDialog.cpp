//ScanTrac Side View Source File
// FrontEndDiagnosticDialog.cpp : implementation file
//

#include "stdafx.h"
#include "ScanTrac.h"
#include "FrontEndDiagnosticDialog.h"
#include "YesNoDialog.h"
#include "PasswordEntryDialog.h"
#include "SerialPort.h"
#include "NumericEntryDialog.h"
#include "AlphaEntryDialog.h"
#include "SpellmanDXMXRaySourcePort.h"
#include "ACPowerMoniter.h"
#include "XRaySource.h"
#include "SelectItemDialog.h"
#include "StartCheckDisk.h"
#include "NoticeDialog.h"
#include "PingTestThread.h"
#include "SerialPortReview.h"
#include "SerialPortGuardMaster.h"
#include "SerialPortPowerFlex40.h"
#include "USBMemoryStickDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFrontEndDiagnosticDialog dialog
extern bool vGlobalRunningCheckDisk;
extern CSerialPortPowerFlex40 *vGlobalPowerFlex40;
extern BYTE vGlobalScanTracType;
extern CSerialPortGuardMaster *vGlobalGuardMaster;
extern bool vGlobalFPGAVersion15Point31OrHigher;
extern CString vGlobalIPAddressString;
extern CXRaySource *vGlobalMonoBlockXRayPort;
extern CString vGlobalDHCPString[2];
extern CString vGlobalIPSubNetMaskString;
extern CString vGlobalIPDefaultGatewayString;
extern CString vGlobalIPDNSAddressString1;
extern BYTE vGlobalIPDHCPEnabled;
extern CString vGlobalIPDHCPServer;
extern CSpellmanDXMXRaySourcePort *vGlobalDXMXRayPort;
extern BYTE vGlobalShiftKeyDown;
extern 	CSerialPort *vGlobaluCSerialPort;
extern CCriticalSection vGlobalWriteToLogLock;
extern bool vGlobalFPGAVersion10Point0OrHigher;
extern CACPowerMoniter *vGlobalACPowerMoniter;
extern CSerialPort *vGlobaluCSerialPort;
extern int vGlobalPasswordLevel;
extern unsigned long vGlobalDialogTitleColor;
extern BYTE vGlobalPCRunningWindowsXP;
extern CPingTestThread *vGlobalPingTestThread;
extern BYTE vGlobalPingStatus;
extern BYTE vGlobalPingReply;
extern BYTE vGlobalPCRunningWindowsXP;
extern const LPCTSTR cScanTracScreenShotDirectory;
extern int vGlobalSerialPortNumberMicroController;	//default = 1
extern int vGlobalSerialPortNumberACPowerMonitor;		//default = 4
extern int vGlobalSerialPortNumberXRaySource;		//default = 3
extern int vGlobalGuardMasterPortNumber;		//default = 5

extern int vGlobalFirstMicroControllerCommCheck;
extern int vGlobalXrayNoCommMessageRcvdEver;

CFrontEndDiagnosticDialog::CFrontEndDiagnosticDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CFrontEndDiagnosticDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFrontEndDiagnosticDialog)
	//}}AFX_DATA_INIT
	vHaveAUSBMemoryStick = false;
	vOldHaveAUSBMemoryStick = false;
	vOldDACTestErrorCount = 0;
	vOldDACTestCount = 0;
	vTestingDACs = false;

	vOldADCTestErrorCount = 0;
	vOldADCTestCount = 0;
	vTestingADCs = false;

	PingTimeOut = 0;
	vLightsOnTime = 5;
	vPingDNS = false;
	vOldXRayNoCommuncations = false;
	vOldTesting = false;
	vMakeExitButtonGreen = false;
	vMainWindowPointer = NULL;
	vSendingTimerContinuous = false;
	vUpdateDisplayTimerHandle = 31315;
	vTestCommTimerHandle = 31314;
	vPingTimerHandle = 52432;
	vChangeMade = false;
	vOldXRayOnNotice = false;
	vTimerOn = false;
	if (vGlobaluCSerialPort)
		vGlobaluCSerialPort->vTestByte = 0;
	vOldNoCommunicationsWithUController = true;
	vAlternateFunctions = false;
	vOlduCWatchDogCount = 0;
	vOlduCResetCount = 0;
	vHoldingReceivedDisplay = false;
	vHoldingReceivedDisplayTimerHandle = 31343;
	vHoldingSentDisplay = false;
	vHoldingSentDisplayTimerHandle = 31344;
	vHoldingTestDisplay = false;
	vHoldingTestDisplayTimerHandle = 31345;
	vHoldingSentTestDisplay = false;
	vHoldingSentTestDisplayTimerHandle = 31346;
	vLocalCWndCollection.Add(this);
	vLocalCWndCollection.Add(&m_Network2RedFlash2);
	vLocalCWndCollection.Add(&m_Network2GreenFlash2);
	vLocalCWndCollection.Add(&m_GatewayLabel);
	vLocalCWndCollection.Add(&m_Network1RedFlash);
	vLocalCWndCollection.Add(&m_Network1GreenFlash);
	vLocalCWndCollection.Add(&m_SubFunction7Button);
	vLocalCWndCollection.Add(&m_Com4GreenFlash);
	vLocalCWndCollection.Add(&m_Com3RedFlash);
	vLocalCWndCollection.Add(&m_Com3GreenFlash);
	vLocalCWndCollection.Add(&m_Com1RedFlash);
	vLocalCWndCollection.Add(&m_Com1GreenFlash);

	vLocalCWndCollection.Add(&m_Com5Label);
	vLocalCWndCollection.Add(&m_InterlocksLabel);
	vLocalCWndCollection.Add(&m_RxBackGroundCom5);
	vLocalCWndCollection.Add(&m_TxBackGroundCom5);
	vLocalCWndCollection.Add(&m_Com5RedFlash);
	vLocalCWndCollection.Add(&m_Com5GreenFlash);

	vLocalCWndCollection.Add(&m_XRayCommunications);
	vLocalCWndCollection.Add(&m_XRayLabel);
	vLocalCWndCollection.Add(&m_XRayPort);
	vLocalCWndCollection.Add(&m_Function5Button);
	vLocalCWndCollection.Add(&m_LoggingSyncData);
	vLocalCWndCollection.Add(&m_LoggingSerialData);
	vLocalCWndCollection.Add(&m_Function1Button);
	vLocalCWndCollection.Add(&m_SubFunction3Button);
	vLocalCWndCollection.Add(&m_SubFunction2Button);
	vLocalCWndCollection.Add(&m_SubFunction1Button);
	vLocalCWndCollection.Add(&m_Command4);
	vLocalCWndCollection.Add(&m_Command3);
	vLocalCWndCollection.Add(&m_Command2);
	vLocalCWndCollection.Add(&m_Command1);
	vLocalCWndCollection.Add(&m_SubFunction6Button);
	vLocalCWndCollection.Add(&m_SubFunction5Button);
	vLocalCWndCollection.Add(&m_Function3Button);
	vLocalCWndCollection.Add(&m_SubFunction8Button);
	vLocalCWndCollection.Add(&m_SubFunction4Button);
	vLocalCWndCollection.Add(&m_Function4Button);
	vLocalCWndCollection.Add(&m_Function2Button);
	vLocalCWndCollection.Add(&m_XRaysOn);
	vLocalCWndCollection.Add(&m_Background);
	vLocalCWndCollection.Add(&m_IPAddressDisplay);
	vLocalCWndCollection.Add(&m_CommStatus);
	vLocalCWndCollection.Add(&m_SendCount);
	vLocalCWndCollection.Add(&m_ReceivedSerialDataStaticText);
	vLocalCWndCollection.Add(&m_uCResetCount);
	vLocalCWndCollection.Add(&m_uCResetCountLabel);
	vLocalCWndCollection.Add(&m_uCWatchDogCount);
	vLocalCWndCollection.Add(&m_uCWatchDogCountLabel);
	vLocalCWndCollection.Add(&m_DataSentLabel);
	vLocalCWndCollection.Add(&m_DataReceivedLabel);
	vLocalCWndCollection.Add(&m_SentSerialDataStaticText);
	vLocalCWndCollection.Add(&m_HVPSType);
	vLocalCWndCollection.Add(&m_TxBackGroundCom3);
	vLocalCWndCollection.Add(&m_TxBackGroundCom1);
	vLocalCWndCollection.Add(&m_RxBackGroundCom1);
	vLocalCWndCollection.Add(&m_RxBackGroundCom3);
	vLocalCWndCollection.Add(&m_RxBackGroundCom4);
	vLocalCWndCollection.Add(&m_PowerMonitorLabel);
	vLocalCWndCollection.Add(&m_uCLabel);
	vLocalCWndCollection.Add(&m_TestDACsButton);
	vLocalCWndCollection.Add(&m_TestADCsButton);
	vLocalCWndCollection.Add(&m_Tx);
	vLocalCWndCollection.Add(&m_MicroCPortLabel);
	vLocalCWndCollection.Add(&m_Com2);
	vLocalCWndCollection.Add(&m_DXM_MonoPortLabel);
	vLocalCWndCollection.Add(&m_PowerMonitorPortLabel);
	vLocalCWndCollection.Add(&m_ComPort);
	vLocalCWndCollection.Add(&m_ComPortLabel);
	vLocalCWndCollection.Add(&m_DialogTitleStaticText);
	vLocalCWndCollection.Add(&m_DNSCommLight);
	vLocalCWndCollection.Add(&m_GatewayCommLight);
	vLocalCWndCollection.Add(&m_GatewayLabel2);
	vLocalCWndCollection.Add(&m_NotMonitored);
	vLocalCWndCollection.Add(&m_Rx);
	vLocalCWndCollection.Add(&m_UControllerCommunications);
	vLocalCWndCollection.Add(&m_UControllerCommunicationsLabel);
	vLocalCWndCollection.Add(&m_UPS);
	vLocalCWndCollection.Add(&m_TxBackGroundCom2);
	vLocalCWndCollection.Add(&m_RxBackGroundCom2);
	vLocalCWndCollection.Add(&m_Com2GreenFlash);
	vLocalCWndCollection.Add(&m_Com2RedFlash);
}

void CFrontEndDiagnosticDialog::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	m_Background.SetWindowPos(NULL,cMainBackgroundLeft,
		cMainBackgroundTop,cMainBackgroundLength,
		cMainBackgroundHeight,SWP_NOZORDER);

	vSubFunction1ButtonEnable = true;
	vSubFunction2ButtonEnable = true;
	vSubFunction3ButtonEnable = true;
	vSubFunction4ButtonEnable = true;

	// kjh 8/11/10 turn LED OFF in either mode
	m_Network1GreenFlash.ShowWindow(SW_HIDE);	// turn green light off
	m_Network1RedFlash.ShowWindow(SW_HIDE);		// turn red light off
	vGlobalPingReply = cPingResultNotSent;
	m_Com1RedFlash.ShowWindow(SW_HIDE);
	m_Com1GreenFlash.ShowWindow(SW_HIDE);
	m_Com3RedFlash.ShowWindow(SW_HIDE);
	m_Com3GreenFlash.ShowWindow(SW_HIDE);
	m_Com4GreenFlash.ShowWindow(SW_HIDE);

	int TimerResult = SetTimer(vPingTimerHandle,1000,NULL);
	if (!TimerResult)
	{
		ReportErrorMessage("Error-Starting Ping Timer Failed",cEMailInspx,32000);
	}

	m_LoggingSerialData.ShowWindow(SW_HIDE);
	m_LoggingSyncData.ShowWindow(SW_HIDE);

	if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)
	if (vAlternateFunctions)
	{
		vSubFunction2ButtonEnable = false;	//m_SubFunction2Button.EnableWindow(false);
		vSubFunction3ButtonEnable = false;	//m_SubFunction3Button.EnableWindow(false);
	}

	if (vGlobalPCRunningWindowsXP >= cWindowsXP)	// Is the system XP
	{
		vSubFunction4ButtonEnable = true;	//m_SubFunction4Button.EnableWindow(true);
	}
	else
	{
		vSubFunction4ButtonEnable = false;	//m_SubFunction4Button.EnableWindow(false);
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

	vLocalSystemData->vLogFile.vShowingSerialDataOnScreen = true;
	int TempTimerResult = SetTimer(vUpdateDisplayTimerHandle,500,NULL);
	if (!TempTimerResult)
		ReportErrorMessage("Error-Display Timer Failed",cEMailInspx,32000);
	if (vGlobaluCSerialPort)
	{
		vGlobaluCSerialPort->vSendCount = 0;
		vGlobaluCSerialPort->vReceiveCount = 0;
		vGlobaluCSerialPort->vHasReceivedCommunication = false;//clear flag
		vGlobaluCSerialPort->vHasTransmittedCommunication = false;//clear flag
		SetDlgItemText(IDC_ComPort,vGlobaluCSerialPort->vComPortString);
	}
	if (vGlobalDXMXRayPort)
	{
		vGlobalDXMXRayPort->vHasReceivedCommunication = false;//clear flag
		vGlobalDXMXRayPort->vHasTransmittedCommunication = false;//clear flag
		SetDlgItemText(IDC_XRayPort,vGlobalDXMXRayPort->vComPortString);
		SetDlgItemText(IDC_XRayCommunications, _T("OK"));

		SetDlgItemText(IDC_HVPSType, vGlobalDXMXRayPort->vHVPSType + ":");


		if ((!vGlobalDXMXRayPort->vPortOpen) && (vGlobalXrayNoCommMessageRcvdEver == cNoCommPortFound))
		{
			SetDlgItemText(IDC_HVPSType, vGlobalDXMXRayPort->vHVPSType + ": NO PORT");
			//m_HVPSType.SetWindowPos(NULL,266,139,190,20,SWP_NOZORDER);
			m_RxBackGroundCom3.ShowWindow(SW_HIDE);
			m_TxBackGroundCom3.ShowWindow(SW_HIDE);
		}
		else
		{
			CString TempText;
			TempText.Format( _T("Com %d"),vGlobalSerialPortNumberXRaySource);
			SetDlgItemText(IDC_DXM_MonoPortLabel,TempText);
		}
	}
	else
	if (vGlobalMonoBlockXRayPort)
	{
		vGlobalMonoBlockXRayPort->vHasReceivedCommunication = false;//clear flag
		vGlobalMonoBlockXRayPort->vHasTransmittedCommunication = false;//clear flag
		SetDlgItemText(IDC_XRayPort,vGlobalMonoBlockXRayPort->vComPortString);
		SetDlgItemText(IDC_XRayCommunications, _T("OK")); //not used anymore
		SetDlgItemText(IDC_HVPSType, _T("XRB210W:"));
		if ((!vGlobalMonoBlockXRayPort->vPortOpen) /*&& (vGlobalMonoBlockXRayPort->vGlobalXrayNoCommMessageRcvdEver == cGiveUpOnCommPort)*/)
		{
			SetDlgItemText(IDC_HVPSType, _T(" XRB210W-NO PORT"));
			//m_HVPSType.SetWindowPos(NULL,266,139,190,20,SWP_NOZORDER);
			m_RxBackGroundCom3.ShowWindow(SW_HIDE);
			m_TxBackGroundCom3.ShowWindow(SW_HIDE);
		}
		else
		{
			CString TempText;
			TempText.Format( _T("Com %d"),vGlobalSerialPortNumberXRaySource);
			SetDlgItemText(IDC_DXM_MonoPortLabel,TempText);
		}

	}
	else
	{//XLF HVPS
		if (vLocalConfigurationData->vHVPSType == cXLFHVPS)
			SetDlgItemText(IDC_HVPSType, _T("Not Used, XLF"));
	}
	if (vGlobaluCSerialPort)
	{
		if ((!vGlobaluCSerialPort->vPortOpen) && (vGlobalFirstMicroControllerCommCheck == cNoCommPortSettings))
		{
			SetDlgItemText(IDC_uCLabel, _T(" uC-NO PORT"));
			m_uCLabel.SetWindowPos(NULL,266,69,190,20,SWP_NOZORDER);
			m_RxBackGroundCom1.ShowWindow(SW_HIDE);
			m_TxBackGroundCom1.ShowWindow(SW_HIDE);
		}
		else
		{
			CString TempText;
			TempText.Format( _T("Com %d"),vGlobalSerialPortNumberMicroController);
			SetDlgItemText(IDC_MicroCPortLabel,TempText);
		}
	}
	if (vGlobalACPowerMoniter) 
	{
		vGlobalACPowerMoniter->vHasReceivedCommunication = false;//clear communication flag
		if ((!vGlobalACPowerMoniter->vPortOpen) && (vGlobalACPowerMoniter->vTestACPowerMoniterExists == cNoCommPortFound))
		{
			SetDlgItemText(IDC_PowerMonitorLabel, _T(" Power Monitor-NO PORT"));
			//m_PowerMonitorLabel.SetWindowPos(NULL,266,171,190,20,SWP_NOZORDER);
			m_RxBackGroundCom4.ShowWindow(SW_HIDE);
		}
		else
		{
			CString TempText;
			TempText.Format( _T("Com %d"),vGlobalSerialPortNumberACPowerMonitor);
			SetDlgItemText(IDC_PowerMonitorPortLabel,TempText);
		}
	}
	if ((vGlobalGuardMaster) && (vGlobalGuardMaster->vDetectedValidGuardMasterDevice)) //the Interlock System exists
	{
		m_Com5Label.ShowWindow(SW_SHOW);
		m_InterlocksLabel.ShowWindow(SW_SHOW);
		m_RxBackGroundCom5.ShowWindow(SW_SHOW);
		m_TxBackGroundCom5.ShowWindow(SW_SHOW);

		vGlobalGuardMaster->vHasReceivedCommunication = false;//clear communication flag
		vGlobalGuardMaster->vHasTransmittedCommunication = false;//clear flag
		SetDlgItemText(IDC_Com5Label,vGlobalGuardMaster->vComPortString);
		//SetDlgItemText(IDC_XRayCommunications,"OK");
		//SetDlgItemText(IDC_HVPSType, vGlobalDXMXRayPort->vHVPSType);
		if (!vGlobalGuardMaster->vPortOpen)// && (vGlobalGuardMaster->vFirstCommunicationTest == cNoCommPortFound))
		{
			SetDlgItemText(IDC_Com5Label, _T(" Interlocks-NO PORT"));
			//m_Com5Label.SetWindowPos(NULL,266,171,190,20,SWP_NOZORDER);
			m_RxBackGroundCom5.ShowWindow(SW_HIDE);
		}
		else
		{
			CString TempText;
			TempText.Format( _T("Com %d"),vGlobalGuardMaster->vComPortNumber);
			SetDlgItemText(IDC_Com5Label,TempText);
		}
	}
	vSerialLogDirectoryFull = false;
	vLocalSystemData->vDACTestErrorCount = 0;
	vOldDACTestErrorCount = 0;
	vLocalSystemData->vADCTestErrorCount = 0;
	vOldADCTestErrorCount = 0;

	//if ((vGlobalScanTracType == cAtempoScanTrac) || (vGlobalScanTracType == cForteScanTrac) || (vGlobalScanTracType == cAllegroScanTrac) || 
	//	(vGlobalScanTracType == cCaseInspectorScanTrac))
	//{
	//	SetDlgItemText(IDC_UPS, _T("Motor Controller"));

	//	m_NotMonitored.ShowWindow(SW_HIDE);
	//	m_TxBackGroundCom2.ShowWindow(SW_SHOW);
	//	m_RxBackGroundCom2.ShowWindow(SW_SHOW);
	//}
	//else
		m_NotMonitored.ShowWindow(SW_SHOW);

	UpdateButtons78();
	UpdateDisplay();

	this->SetWindowText(_T("Serial Comm"));
	m_IPAddressDisplay.SetWindowPos(NULL,145,195,350,115,SWP_NOZORDER);
	UpdateInspxItems();
	::SetupMenu(vLocalCWndCollection);
}

void CFrontEndDiagnosticDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFrontEndDiagnosticDialog)
	DDX_Control(pDX, IDC_Network2RedFlash2, m_Network2RedFlash2);
	DDX_Control(pDX, IDC_Network2GreenFlash2, m_Network2GreenFlash2);
	DDX_Control(pDX, IDC_GatewayLabel, m_GatewayLabel);
	DDX_Control(pDX, IDC_Network1RedFlash, m_Network1RedFlash);
	DDX_Control(pDX, IDC_Network1GreenFlash, m_Network1GreenFlash);
	//  DDX_Control(pDX, IDC_CommStatus, m_HiddenCount4);
	//  DDX_Control(pDX, IDC_SendCount, m_HiddenCount3);
	//  DDX_Control(pDX, IDC_DataSentLabel, m_HiddenLabel6);
	//  DDX_Control(pDX, IDC_DataReceivedLabel, m_HiddenLabel5);
	//  DDX_Control(pDX, IDC_SentSerialDataStaticText, m_HiddenLabel4);
	//  DDX_Control(pDX, IDC_ReceivedSerialDataStaticText, m_HiddenLabel3);
	//  DDX_Control(pDX, IDC_uCWatchDogCountLabel, m_HiddenLabel2);
	//  DDX_Control(pDX, IDC_uCResetCountLabel, m_HiddenLabel1);
	//  DDX_Control(pDX, IDC_uCWatchDogCount, m_HiddenCount2);
	//  DDX_Control(pDX, IDC_uCResetCount, m_HiddenCount1);
	DDX_Control(pDX, IDC_SubFunction7Button, m_SubFunction7Button);
	DDX_Control(pDX, IDC_Com4GreenFlash, m_Com4GreenFlash);
	DDX_Control(pDX, IDC_Com3RedFlash, m_Com3RedFlash);
	DDX_Control(pDX, IDC_Com3GreenFlash, m_Com3GreenFlash);
	DDX_Control(pDX, IDC_Com1RedFlash, m_Com1RedFlash);
	DDX_Control(pDX, IDC_Com1GreenFlash, m_Com1GreenFlash);
	DDX_Control(pDX, IDC_XRayCommunications, m_XRayCommunications);
	DDX_Control(pDX, IDC_XRayLabel, m_XRayLabel);
	DDX_Control(pDX, IDC_XRayPort, m_XRayPort);
	DDX_Control(pDX, IDC_Function5Button, m_Function5Button);
	DDX_Control(pDX, IDC_LoggingSyncData, m_LoggingSyncData);
	DDX_Control(pDX, IDC_LoggingSerialData, m_LoggingSerialData);
	DDX_Control(pDX, IDC_Function1Button, m_Function1Button);
	DDX_Control(pDX, IDC_SubFunction3Button, m_SubFunction3Button);
	DDX_Control(pDX, IDC_SubFunction2Button, m_SubFunction2Button);
	DDX_Control(pDX, IDC_SubFunction1Button, m_SubFunction1Button);
	DDX_Control(pDX, IDC_Command4, m_Command4);
	DDX_Control(pDX, IDC_Command3, m_Command3);
	DDX_Control(pDX, IDC_Command2, m_Command2);
	DDX_Control(pDX, IDC_Command1, m_Command1);
	DDX_Control(pDX, IDC_SubFunction6Button, m_SubFunction6Button);
	DDX_Control(pDX, IDC_SubFunction5Button, m_SubFunction5Button);
	DDX_Control(pDX, IDC_Function3Button, m_Function3Button);
	DDX_Control(pDX, IDC_SubFunction8Button, m_SubFunction8Button);
	DDX_Control(pDX, IDC_SubFunction4Button, m_SubFunction4Button);
	DDX_Control(pDX, IDC_Function4Button, m_Function4Button);
	DDX_Control(pDX, IDC_Function2Button, m_Function2Button);
	DDX_Control(pDX, IDC_XRaysOn, m_XRaysOn);
	DDX_Control(pDX, IDC_Background, m_Background);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_IPAddressDisplay, m_IPAddressDisplay);
	DDX_Control(pDX, IDC_CommStatus, m_CommStatus);	//  DDX_Text(pDX, IDC_CommStatus, m_CommStatus);
	DDX_Control(pDX, IDC_SendCount, m_SendCount);
	//  DDX_Control(pDX, IDC_SentSerialDataStaticText, m_SendSerialDataStaticText);
	DDX_Control(pDX, IDC_ReceivedSerialDataStaticText, m_ReceivedSerialDataStaticText);
	DDX_Control(pDX, IDC_uCResetCount, m_uCResetCount);
	DDX_Control(pDX, IDC_uCResetCountLabel, m_uCResetCountLabel);
	DDX_Control(pDX, IDC_uCWatchDogCount, m_uCWatchDogCount);
	DDX_Control(pDX, IDC_uCWatchDogCountLabel, m_uCWatchDogCountLabel);
	DDX_Control(pDX, IDC_DataSentLabel, m_DataSentLabel);
	DDX_Control(pDX, IDC_DataReceivedLabel, m_DataReceivedLabel);
	DDX_Control(pDX, IDC_SentSerialDataStaticText, m_SentSerialDataStaticText);
	DDX_Control(pDX, IDC_HVPSType, m_HVPSType);
	DDX_Control(pDX, IDC_TxBackGroundCom3, m_TxBackGroundCom3);
	DDX_Control(pDX, IDC_TxBackGroundCom1, m_TxBackGroundCom1);
	DDX_Control(pDX, IDC_RxBackGroundCom1, m_RxBackGroundCom1);
	DDX_Control(pDX, IDC_RxBackGroundCom3a, m_RxBackGroundCom3);
	DDX_Control(pDX, IDC_RxBackGroundCom4, m_RxBackGroundCom4);
	DDX_Control(pDX, IDC_PowerMonitorLabel, m_PowerMonitorLabel);
	DDX_Control(pDX, IDC_uCLabel, m_uCLabel);
	DDX_Control(pDX, IDC_TestDACsButton, m_TestDACsButton);
	DDX_Control(pDX, IDC_TestADCsButton, m_TestADCsButton);
	DDX_Control(pDX, IDC_Tx, m_Tx);
	DDX_Control(pDX, IDC_TestADCsButton, m_TestADCsButton);
	DDX_Control(pDX, IDC_MicroCPortLabel, m_MicroCPortLabel);
	DDX_Control(pDX, IDC_Com2, m_Com2);
	DDX_Control(pDX, IDC_DXM_MonoPortLabel, m_DXM_MonoPortLabel);
	DDX_Control(pDX, IDC_PowerMonitorPortLabel, m_PowerMonitorPortLabel);
	DDX_Control(pDX, IDC_ComPort, m_ComPort);
	DDX_Control(pDX, IDC_ComPortLabel, m_ComPortLabel);
	DDX_Control(pDX, IDC_DialogTitleStaticText1, m_DialogTitleStaticText);
	DDX_Control(pDX, IDC_DNSCommLight, m_DNSCommLight);
	DDX_Control(pDX, IDC_GatewayCommLight, m_GatewayCommLight);
	DDX_Control(pDX, IDC_GatewayLabel2, m_GatewayLabel2);
	DDX_Control(pDX, IDC_NotMonitored, m_NotMonitored);
	DDX_Control(pDX, IDC_Rx, m_Rx);
	DDX_Control(pDX, IDC_UControllerCommunications, m_UControllerCommunications);
	DDX_Control(pDX, IDC_UControllerCommunicationsLabel, m_UControllerCommunicationsLabel);
	DDX_Control(pDX, IDC_UPS, m_UPS);
	DDX_Control(pDX, IDC_Com5GreenFlash, m_Com5GreenFlash);
	DDX_Control(pDX, IDC_Com5Label, m_Com5Label);
	DDX_Control(pDX, IDC_Com5RedFlash, m_Com5RedFlash);
	//  DDX_Control(pDX, IDC_RxBackGroundCom5, m_RXBackGroundCom5);
	DDX_Control(pDX, IDC_TxBackGroundCom5, m_TxBackGroundCom5);
	DDX_Control(pDX, IDC_RxBackGroundCom5, m_RxBackGroundCom5);
	DDX_Control(pDX, IDC_InterlocksLabel, m_InterlocksLabel);
	DDX_Control(pDX, IDC_RxBackGroundCom2, m_RxBackGroundCom2);
	DDX_Control(pDX, IDC_TxBackGroundCom2, m_TxBackGroundCom2);
	DDX_Control(pDX, IDC_Com2GreenFlash, m_Com2GreenFlash);
	DDX_Control(pDX, IDC_Com2RedFlash, m_Com2RedFlash);
}


BEGIN_MESSAGE_MAP(CFrontEndDiagnosticDialog, CDialog)
	//{{AFX_MSG_MAP(CFrontEndDiagnosticDialog)
	ON_WM_CTLCOLOR()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
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
	//}}AFX_MSG_MAP
	ON_STN_CLICKED(IDC_TestDACsButton, &CFrontEndDiagnosticDialog::OnTestdacsbutton)
	ON_STN_CLICKED(IDC_TestADCsButton, &CFrontEndDiagnosticDialog::OnTestadcsbutton)
	ON_STN_CLICKED(IDC_DialogTitleStaticText1, &CFrontEndDiagnosticDialog::OnDialogtitlestatictext1)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFrontEndDiagnosticDialog message handlers

void CFrontEndDiagnosticDialog::OnFunction1Button() 
{
	// User clicked Main Menu Button to exit 
	//if ((!vChangeMade) || (AbortSetupChanges()))
	//{
		PrepareAndExit(10);
	//}
}

void CFrontEndDiagnosticDialog::StopContinuousCommunicationTests()
{
	if (vSendingTimerContinuous)
	{
		vSendingTimerContinuous = false;
		vTimerOn = false;
		SetDlgItemText(IDC_SubFunction3Button, _T("Slow Test uC Command!"));
		KillTimer(vTestCommTimerHandle);
	}
	if (vGlobaluCSerialPort)
	if (vGlobaluCSerialPort->vSendingContinuous)
	{
		vGlobaluCSerialPort->vSendingContinuous = false;
		vTimerOn = false;
		SetDlgItemText(IDC_SubFunction2Button, _T("Fast Test uC Command!"));
		KillTimer(vTestCommTimerHandle);
	}
}

void CFrontEndDiagnosticDialog::OnFunction2Button() 
{
	// Save and Exit Button was pressed
	//if ((!vChangeMade) || (KeepSetupChanges()))
	//{
		PrepareAndExit(true);
	//}
}

void CFrontEndDiagnosticDialog::OnFunction3Button() 
{
	// Edit Test Command
	if (vAlternateFunctions)
	{ // if getting Faulty Encoder can show uncorrected images
		CSelectItemDialog ISelectItemDialog;  
			//Set dialog box data titles and number value
		ISelectItemDialog.vTitleString = "Image Correction If Encoder Too Fast";

		ISelectItemDialog.vBackgroundColor = cGreen;
		ISelectItemDialog.vShowExit = true;
		ISelectItemDialog.vInAutoSetup = false;
		ISelectItemDialog.m_DialogTitleStaticText2 = "Default: Correct Image, Enabled when Select Product";

		if (vLocalSystemData->vFPGADontCorrectForEncoderErrors)
			ISelectItemDialog.m_DialogTitleStaticText4 = "Currently Not Correcting Images for Encoder Overspeed";
		else
			ISelectItemDialog.m_DialogTitleStaticText4 = "Currently Correcting Images for Encoder Overspeed";

		ISelectItemDialog.m_DialogTitleStaticText3 = "Must have FPGA Version 15.44 or higher";

		ISelectItemDialog.vNumberOfItems = 2;
		ISelectItemDialog.vItems[0] = "Correct Image if encoder pulses too close";
		ISelectItemDialog.vItems[1] = "Don't Correct Image if encoder pulses too close";
		ISelectItemDialog.vGreenItem = 0;
		//Pass control to dialog box and display
		int nResponse = ISelectItemDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			vLocalSystemData->vFPGADontCorrectForEncoderErrors = ISelectItemDialog.vItemNumberSelected - 1; // <<<--- answer select back here
			tSerialCommand TempCommand;
			TempCommand[0] = 0x23;
			TempCommand[1] = 0xE5;
			TempCommand[2] = 0;
			TempCommand[3] = vLocalSystemData->vFPGADontCorrectForEncoderErrors;
			if (vGlobaluCSerialPort)
				vGlobaluCSerialPort->SendSerialCommand(TempCommand);
		}
		else if (nResponse != 11)
		{
			int TempOK = false;
			if (nResponse == 10)
			{
				Sleep(1); //is the equivelent of a yeild statement;
				PrepareAndExit(10);
			}
		}
	}
	else
	if (PasswordOK(cTemporaryInspxPassword,false))
	{
		CAlphaEntryDialog IAlphaEntryDialog;  
		IAlphaEntryDialog.vMainWindowPointer = vMainWindowPointer;
		//Set dialog box data titles and number value
		IAlphaEntryDialog.vEditString = ByteToHex(vLocalConfigurationData->vCommand1);
		IAlphaEntryDialog.m_DialogTitleStaticText = "Enter First Byte of Command in Hex";
		IAlphaEntryDialog.m_DialogTitleStaticText2 = "Original Value: " + IAlphaEntryDialog.vEditString + " hex, " + dtoa(vLocalConfigurationData->vCommand1,0) + " decimal";
		//Pass control to dialog box and display
		int nResponse = IAlphaEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			vLocalConfigurationData->vCommand1 = HexToByte(IAlphaEntryDialog.vEditString);

			IAlphaEntryDialog.vEditString = ByteToHex(vLocalConfigurationData->vCommand2);
			IAlphaEntryDialog.m_DialogTitleStaticText = "Enter Second Byte of Command in Hex";
			IAlphaEntryDialog.m_DialogTitleStaticText2 = "Original Value: " + IAlphaEntryDialog.vEditString + " hex, " + dtoa(vLocalConfigurationData->vCommand2,0) + " decimal";
			//Pass control to dialog box and display
			int nResponse = IAlphaEntryDialog.DoModal();
			//dialog box is now closed, if user pressed select do this
			//if user pressed cancel, do nothing
			if (nResponse == IDOK)
			{
				vLocalConfigurationData->vCommand2 = HexToByte(IAlphaEntryDialog.vEditString);

				IAlphaEntryDialog.vEditString = ByteToHex(vLocalConfigurationData->vCommand3);
				IAlphaEntryDialog.m_DialogTitleStaticText = "Enter Third Byte of Command in Hex";
				IAlphaEntryDialog.m_DialogTitleStaticText2 = "Original Value: " + IAlphaEntryDialog.vEditString + " hex, " + dtoa(vLocalConfigurationData->vCommand3,0) + " decimal";
				//Pass control to dialog box and display
				int nResponse = IAlphaEntryDialog.DoModal();
				//dialog box is now closed, if user pressed select do this
				//if user pressed cancel, do nothing
				if (nResponse == IDOK)
				{
					vLocalConfigurationData->vCommand3 = HexToByte(IAlphaEntryDialog.vEditString);

					IAlphaEntryDialog.vEditString = dtoa(vLocalConfigurationData->vCommand4, 0);
					IAlphaEntryDialog.m_DialogTitleStaticText = "Enter Fourth Byte of Command in Hex";
					IAlphaEntryDialog.m_DialogTitleStaticText2 = "Original Value: " + IAlphaEntryDialog.vEditString + " hex, " + dtoa(vLocalConfigurationData->vCommand4,0) + " decimal";
					//Pass control to dialog box and display
					int nResponse = IAlphaEntryDialog.DoModal();
					//dialog box is now closed, if user pressed select do this
					//if user pressed cancel, do nothing
					if (nResponse == IDOK)
					{
						vLocalConfigurationData->vCommand4 = HexToByte(IAlphaEntryDialog.vEditString);
					}
					else 
					if (nResponse == 10)
					{
						//Main Menu button pressed
						PrepareAndExit(10);
					}
				}
				else 
				if (nResponse == 10)
				{
					//Main Menu button pressed
					PrepareAndExit(10);
				}
			}
			else 
			if (nResponse == 10)
			{
				//Main Menu button pressed
				PrepareAndExit(10);
			}
		}
		else 
		if (nResponse == 10)
		{
			//Main Menu button pressed
			PrepareAndExit(10);
		}
	}

	ShowCommand();
}

void CFrontEndDiagnosticDialog::OnFunction4Button() 
{
	//Send Test Command
	if (vGlobaluCSerialPort)
	if (PasswordOK(cTemporaryInspxPassword,false))
	{
		tSerialCommand TempCommand;
		TempCommand[0] = vLocalConfigurationData->vCommand1;
		TempCommand[1] = vLocalConfigurationData->vCommand2;
		TempCommand[2] = vLocalConfigurationData->vCommand3;
		TempCommand[3] = vLocalConfigurationData->vCommand4;
		vGlobaluCSerialPort->SendSerialCommand(TempCommand);
	}
}

void CFrontEndDiagnosticDialog::OnFunction5Button() 
{
	StopContinuousCommunicationTests(); 
	if (PasswordOK(cTemporaryInspxPassword,false))
	if (vAlternateFunctions)
	{
		if (PasswordOK(cTemporaryInspxPassword,false))
		{
			SetDlgItemText(IDC_Function4Button, _T("Send Test Command"));
			m_Function4Button.ShowWindow(SW_SHOW);
			SetDlgItemText(IDC_Function3Button, _T("Edit Test Command"));
			m_Function3Button.ShowWindow(SW_SHOW);
		}
		m_LoggingSyncData.ShowWindow(SW_HIDE);
		vAlternateFunctions = false;
		m_SubFunction4Button.ShowWindow(SW_SHOW);
		m_SubFunction8Button.ShowWindow(SW_SHOW);
		SetDlgItemText(IDC_Function5Button, _T("uController Diagnostics"));
		SetDlgItemText(IDC_SubFunction1Button, _T("Burst Test uC Command!"));
		SetDlgItemText(IDC_SubFunction2Button, _T("Fast Test uC Command!"));
		SetDlgItemText(IDC_SubFunction3Button, _T("Slow Test uC Command!"));
		SetDlgItemText(IDC_SubFunction4Button, _T("Run Check Disk"));		// kjh 07/14/10
		SetDlgItemText(IDC_SubFunction6Button, _T("Delete Old Log Files From Disk"));
		SetDlgItemText(IDC_SubFunction8Button, _T("Reset uC (Watchdog Reset)!"));
		SetDlgItemText(IDC_SubFunction5Button, _T("Delete Old Image Files"));
		m_SubFunction5Button.ShowWindow(SW_SHOW);
		m_SubFunction6Button.ShowWindow(SW_SHOW);
		if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)
		{
			vSubFunction2ButtonEnable = false;	//m_SubFunction2Button.EnableWindow(false);
			vSubFunction3ButtonEnable = false;	//m_SubFunction3Button.EnableWindow(false);
		}
		UpdateButtons78();
		m_Command1.ShowWindow(SW_SHOW);
		m_Command2.ShowWindow(SW_SHOW);
		m_Command3.ShowWindow(SW_SHOW);
		m_Command4.ShowWindow(SW_SHOW);
		m_LoggingSerialData.ShowWindow(SW_SHOW);
	}
	else
	{
		if (PasswordOK(cTemporaryInspxPassword,false))
		{
//			
//			CNoticeDialog TempNoticeDialog;
//			TempNoticeDialog.vNoticeText = 
//				"\nThese Micro Controller Diagnostics can put\nthe Micro Controller in NON operation modes!\n\nDo not use them unless you are sure\nyou know what you are doing!";
//			TempNoticeDialog.vType = cWarningMessage;
//			TempNoticeDialog.DoModal();
//			
			m_LoggingSyncData.ShowWindow(SW_HIDE);

			m_Function3Button.ShowWindow(SW_SHOW);
			SetDlgItemText(IDC_Function3Button, _T("Encoder Correction"));

			m_Function4Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_Function4Button.GetRectToInvalidateArrow());
			m_LoggingSerialData.ShowWindow(SW_HIDE);
			vSubFunction2ButtonEnable = true;	//m_SubFunction2Button.EnableWindow(true);
			vSubFunction3ButtonEnable = true;	//m_SubFunction3Button.EnableWindow(true);
			vAlternateFunctions = true;
			m_SubFunction4Button.ShowWindow(SW_SHOW);
			m_SubFunction6Button.ShowWindow(SW_SHOW);
			m_SubFunction8Button.ShowWindow(SW_SHOW);
			SetDlgItemText(IDC_SubFunction8Button, _T("Memory Size Changed!"));
			m_Command1.ShowWindow(SW_HIDE);
			m_Command2.ShowWindow(SW_HIDE);
			m_Command3.ShowWindow(SW_HIDE);
			m_Command4.ShowWindow(SW_HIDE);

			UpdateButtons78();

			SetDlgItemText(IDC_Function5Button, _T("Normal Functions"));
			
			if (vLocalSystemData->vuControllerSendExtraDiagnosticMessages)
				SetDlgItemText(IDC_SubFunction2Button, _T("Stop Diagnostic Messages!"));
			else
				SetDlgItemText(IDC_SubFunction2Button, _T("Send Diagnostic Messages!"));
			if (vLocalSystemData->vuCOutputSerialDebugData)
				SetDlgItemText(IDC_SubFunction3Button, _T("uC Serial Debug\n Off!"));
			else
				SetDlgItemText(IDC_SubFunction3Button, _T("uC Serial Debug\n On!"));

			SetDlgItemText(IDC_SubFunction6Button, _T("Verify\nDAC\nValues!"));
			SetDlgItemText(IDC_SubFunction7Button, _T("Verify\nADC\nValues!"));
			//if ((vGlobaluCSerialPort) && (vGlobaluCSerialPort->vPortOpen))
			//	SetDlgItemText(IDC_SubFunction6Button,"Close Serial Port!");
			//else
			//	SetDlgItemText(IDC_SubFunction6Button,"Open Serial Port!");

			SetDlgItemText(IDC_SubFunction4Button, _T(""));
			SetDlgItemText(IDC_SubFunction5Button, _T("Stop AutoCheck on Bootup"));

			//m_SubFunction5Button.ShowWindow(SW_HIDE);
			//m_ArrowRight1.ShowWindow(SW_HIDE);

//#ifdef UseNewTestModeCode
//			if (vLocalSystemData->vuCSimulate16Hz)
//				SetDlgItemText(IDC_SubFunction7Button,"uC Stop Generate 16 Hz!");
//			else
//				SetDlgItemText(IDC_SubFunction7Button,"uC Generate 16 Hz!");
//#endif
#ifdef UseTestModeCode
			SetDlgItemText(IDC_Function4Button,"Initialize uC for Product!");
			SetDlgItemText(IDC_SubFunction4Button,"Generate Queue Error!");

		
			if (vLocalSystemData->vEnableduC16HzInterrupt)
				SetDlgItemText(IDC_SubFunction8Button,"Disable 16Hz Interrupt!");
			else
				SetDlgItemText(IDC_SubFunction8Button,"Enable 16Hz Interrupt!");
#endif

			if (vGlobalDXMXRayPort)
			{
				m_SubFunction4Button.ShowWindow(SW_SHOW);
				SetDlgItemText(IDC_SubFunction4Button, _T("Set DXM Filament Limit!"));
			}
		}
	}
}

void CFrontEndDiagnosticDialog::OnSubFunction1Button() 
{
	if (vAlternateFunctions)
	{
		if (PasswordOK(cTemporaryInspxPassword,false))
		if ((vGlobaluCSerialPort) && (vGlobaluCSerialPort->vPortOpen) && (vGlobalFirstMicroControllerCommCheck == cFinishedSearchForNextSerialPort))
		{
			// User pressed Log Serial Data button
			vLocalSystemData->vLogFile.vLogSyncData = !vLocalSystemData->vLogFile.vLogSyncData;
			UpdateButtons78();
		}
		else
		{
		// port is valid
			if (vLocalSystemData->vSystemRunMode == cStoppedSystemMode)
			{
				//Burst comm test
				StopContinuousCommunicationTests(); 

				if (vGlobaluCSerialPort)
					for (BYTE TempLoop = 0; TempLoop < 100; TempLoop++)
						vGlobaluCSerialPort->SendCommTest();
			}
			else
			{
				CNoticeDialog TempNoticeDialog;
				TempNoticeDialog.vNoticeText = "\n\n\nYou cannot do this test while the system is running";
				TempNoticeDialog.vType = cErrorMessage;
				TempNoticeDialog.DoModal();
			}
		}
		else
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\n\nYou cannot do this test while determining port";
			TempNoticeDialog.vType = cErrorMessage;
			TempNoticeDialog.DoModal();
		}
	}
	else
	{ //usb memory stick functions
		if (vHaveAUSBMemoryStick)
		{
			if (PasswordOK(cSetupPassword,true))
			{
				//uController Communications Menu button pressed
				//Create diagnostic dialog box
				CUSBMemoryStickDialog IUSBMemoryStickDialog;  
				//Set dialog box data like how many products there are
				//IMoreDiagnosticsDialog.vLocalProductCollection = &vProductCollection;
				IUSBMemoryStickDialog.vLocalSystemData = vLocalSystemData;
				IUSBMemoryStickDialog.vLocalConfigurationData = vLocalConfigurationData;
				IUSBMemoryStickDialog.vMainWindowPointer = vMainWindowPointer;

				//Pass control to dialog box and display
				int nResponse = IUSBMemoryStickDialog.DoModal();

				//if changes were made, save data to file
				if (nResponse == 10)
				{
					//Main Menu button pressed
					Sleep(1); //is the equivelent of a yeild statement;
					CDialog::EndDialog(10);
				}
			}
		}
		else
		{
				CNoticeDialog TempNoticeDialog;
				TempNoticeDialog.vNoticeText = "\n\nInsert USB Memory Stick,\nwait 10 seconds,\nthen try again.";
				TempNoticeDialog.vType = cInformationMessage;
				TempNoticeDialog.DoModal();
		}
	}
	UpdateButtons78();
}

void CFrontEndDiagnosticDialog::OnSubFunction2Button() 
{ 
	if ((vGlobaluCSerialPort) && (vGlobaluCSerialPort->vPortOpen) && (vGlobalFirstMicroControllerCommCheck == cFinishedSearchForNextSerialPort))
	{
		if (PasswordOK(cTemporaryInspxPassword,false))
		if (vAlternateFunctions)
		{
			// toggle send extra diagnostic messages
			tSerialCommand TempCommand;
			TempCommand[0] = 0x33;
			TempCommand[1] = 0;
			TempCommand[2] = 0;
			if (vLocalSystemData->vuControllerSendExtraDiagnosticMessages)
			{
				vLocalSystemData->vuControllerSendExtraDiagnosticMessages = false;
				TempCommand[3] = 2;
				SetDlgItemText(IDC_SubFunction2Button, _T("Send Diagnostic Messages!"));
			}
			else
			{
				vLocalSystemData->vuControllerSendExtraDiagnosticMessages = true;
				TempCommand[3] = 1;
				SetDlgItemText(IDC_SubFunction2Button, _T("Stop Diagnostic Messages!"));

				CNoticeDialog TempNoticeDialog;
				TempNoticeDialog.vNoticeText = "\n\nThe Micro Controller is now outputing\nextra diagnostic messages that will be\nwritten to the serial data log if logging is enabled";
				TempNoticeDialog.vType = cInformationMessage;
				TempNoticeDialog.DoModal();
			}
			if (vGlobaluCSerialPort)
				vGlobaluCSerialPort->SendSerialCommand(TempCommand);
		}
		else
		{
			if (vLocalSystemData->vSystemRunMode == cStoppedSystemMode)
			{
				if (vSendingTimerContinuous)
				{
					vSendingTimerContinuous = false;
					vTimerOn = false;
					SetDlgItemText(IDC_SubFunction3Button, _T("Slow Test uC Command!"));
					KillTimer(vTestCommTimerHandle);
				}

				if (vGlobaluCSerialPort)
				if (vGlobaluCSerialPort->vSendingContinuous)
				{
					vGlobaluCSerialPort->vSendingContinuous = false;
					vTimerOn = false;
					SetDlgItemText(IDC_SubFunction2Button, _T("Fast Test uC Command!"));
					KillTimer(vTestCommTimerHandle);
				}
				else
				{
					vGlobaluCSerialPort->vSendingContinuous = true;
					SetDlgItemText(IDC_SubFunction2Button, _T("Stop Fast Command Test!"));
					vGlobaluCSerialPort->SendCommTest();
				}
			}
			else
			{
				CNoticeDialog TempNoticeDialog;
				TempNoticeDialog.vNoticeText = "\n\n\nYou cannot do this test while the system is running";
				TempNoticeDialog.vType = cErrorMessage;
				TempNoticeDialog.DoModal();
			}
		}
		UpdateButtons78();
	}
}

void CFrontEndDiagnosticDialog::OnSubFunction3Button() 
{
	if ((vGlobaluCSerialPort) && (vGlobaluCSerialPort->vPortOpen) && (vGlobalFirstMicroControllerCommCheck == cFinishedSearchForNextSerialPort))
	{
		if (PasswordOK(cTemporaryInspxPassword,false))
		if (vAlternateFunctions)
		{
			// uController Serial Debug Data command
			if (vLocalSystemData->vuCOutputSerialDebugData)
			{
				// uController Stop Generate 16 Hertz command
				SenduCSerialDebug(false);	
				vLocalSystemData->vuCOutputSerialDebugData = false;
				SetDlgItemText(IDC_SubFunction3Button, _T("uC Serial Debug\n On!"));
			}
			else
			{
				SenduCSerialDebug(true);
				vLocalSystemData->vuCOutputSerialDebugData = true;
				SetDlgItemText(IDC_SubFunction3Button, _T("uC Serial Debug\n Off!"));

				CNoticeDialog TempNoticeDialog;
				TempNoticeDialog.vNoticeText = "\n\nThe Micro Controller is now outputing\n diagnostic messages on its connector P2\nthat can be viewed with a terminal or Breakout";
				TempNoticeDialog.vType = cInformationMessage;
				TempNoticeDialog.DoModal();
			}
		}
		else
		{
			if (vLocalSystemData->vSystemRunMode == cStoppedSystemMode)
			{
				// continue sending test commands
				if (vGlobaluCSerialPort)
				if (vGlobaluCSerialPort->vSendingContinuous)
				{
					vGlobaluCSerialPort->vSendingContinuous = false;
					vTimerOn = false;
					SetDlgItemText(IDC_SubFunction2Button, _T("Fast Test uC Command!"));
					KillTimer(vTestCommTimerHandle);
				}

				if (vSendingTimerContinuous)
				{
					vSendingTimerContinuous = false;
					vTimerOn = false;
					SetDlgItemText(IDC_SubFunction3Button, _T("Slow Test uC Command!"));
					KillTimer(vTestCommTimerHandle);
				}
				else
				{
					vSendingTimerContinuous = true;
					vTimerOn = true;
					SetDlgItemText(IDC_SubFunction3Button, _T("Stop Slow Command Test!"));
					int TempTimerResult = SetTimer(vTestCommTimerHandle,1,NULL);
					if (!TempTimerResult)
						ReportErrorMessage("Error-Comm Timer Failed",cEMailInspx,32000);
				}
			}
			else
			{
				CNoticeDialog TempNoticeDialog;
				TempNoticeDialog.vNoticeText = "\n\n\nYou cannot do this test while the system is running";
				TempNoticeDialog.vType = cErrorMessage;
				TempNoticeDialog.DoModal();
			}
		}
		UpdateButtons78();
	}
}

		// kjh 07/14/10 change from run serial test once to check disk on reboot
void CFrontEndDiagnosticDialog::OnSubFunction4Button() 
{
//	if (PasswordOK(cTemporaryInspxPassword,false))  // check disk any mode
	if (vAlternateFunctions)
	{
		if (PasswordOK(cTemporaryInspxPassword,false))  
		if (vGlobalDXMXRayPort)
		{
			if (PasswordOK(cSuperInspxPassword,false))
			{
				CYesNoDialog TempYesNoDialog;
				TempYesNoDialog.vNoticeText =  _T("\nSet Filament Limit to Default (4.6A) or Zero?\nChange will occur next time the current is set.\nZero will shut down X-Rays.");
				TempYesNoDialog.vYesButtonText = "Set to Default";
				TempYesNoDialog.vNoButtonText = "Set to Zero";
				TempYesNoDialog.vQuestionType = cConfirmQuestion;
				int TempResult = TempYesNoDialog.DoModal();
				//if user said OK cause watchdog timer reset 
				if (TempResult == IDOK)
				{
					vGlobalDXMXRayPort->SetFilamentLimitToDefault();
					CNoticeDialog TempNoticeDialog;
					TempNoticeDialog.vNoticeText = "\n\n\nSet DXM Filament Current to Default of 4.6 amps";
					TempNoticeDialog.vType = cInformationMessage;
					TempNoticeDialog.vAutoClose = 3000;
					TempNoticeDialog.DoModal();
				}
				else
				{
					vGlobalDXMXRayPort->SetFilamentLimitToZero();
					CNoticeDialog TempNoticeDialog;
					TempNoticeDialog.vNoticeText = "\nSet DXM Filament Current to Zero amps\nImportant, this will permanently shut off the X-Rays\nuntil you set it back to the default, or restart program.";
					TempNoticeDialog.vType = cInformationMessage;
					//TempNoticeDialog.vAutoClose = 3000;
					TempNoticeDialog.DoModal();
				}
			}
			else
			{
				vGlobalDXMXRayPort->SetFilamentLimitToDefault();
				CNoticeDialog TempNoticeDialog;
				TempNoticeDialog.vNoticeText = "\n\n\nSet DXM Filament Current to Default of 4.6 amps";
				TempNoticeDialog.vType = cInformationMessage;
				TempNoticeDialog.vAutoClose = 3000;
				TempNoticeDialog.DoModal();
			}
		}
		/*
#ifdef UseTestModeCode
		//simulate a queue overflow error
		CYesNoDialog TempYesNoDialog;
		TempYesNoDialog.vNoticeText = "\n\nGenerate a uC Queue Overflow Error Message?";
		TempYesNoDialog.vYesButtonText = "Generate Error";
		TempYesNoDialog.vQuestionType = cConfirmQuestion;
		int TempResult = TempYesNoDialog.DoModal();
		//if user said OK cause watchdog timer reset 
		if (TempResult == IDOK)
		{
			tSerialCommand TempCommand;
			TempCommand[0] = 0x3D;
			TempCommand[1] = 0x20;
			TempCommand[2] = 0;
			TempCommand[3] = 0;
			if (vGlobaluCSerialPort)
				vGlobaluCSerialPort->SendSerialCommand(TempCommand);
		}
#endif
		*/
	}
	else
	{
// 7/14/2010 kjh below 4 lines for testing single communication to uC board
// changed code to run check disk
//		StopContinuousCommunicationTests(); 
//
//		if (vGlobaluCSerialPort)
//			vGlobaluCSerialPort->SendCommTest();
		if (vGlobalPCRunningWindowsXP >= cWindowsXP)	// Is the system XP
		{			
			// first want to ask run check disk for c: or d: or skip
			// is there a d:?
			CFileStatus TempFileStatus;
			CString TempBackupDriveString = "D:\\0_Backup_Hard_Drive";
			BOOL TempFoundSecondBackupDrive = false;
			if ((CFile::GetStatus(TempBackupDriveString, TempFileStatus)))
			{
				TempFoundSecondBackupDrive = true;
			}
			else
			{
				double TempDriveSpaceFree = 0;
				TempDriveSpaceFree = CheckDiskSpaceFree("D:\\");
				if (TempDriveSpaceFree > 900000)
					TempFoundSecondBackupDrive = true;
			}

			CSelectItemDialog ISelectItemDialog;  
				//Set dialog box data titles and number value
			ISelectItemDialog.vBackgroundColor = cGreen;
			ISelectItemDialog.vShowExit = true;
			ISelectItemDialog.vInAutoSetup = false;
			ISelectItemDialog.vTitleString = "Select Hard Disk Drive to Check Disk";
			ISelectItemDialog.m_DialogTitleStaticText2 = "Select Drive to Check";

			ISelectItemDialog.vNumberOfItems = 1;
			ISelectItemDialog.vItems[0] = "Check Disk on C:";
			if (TempFoundSecondBackupDrive)	// c: and d: available for check disk
			{
				ISelectItemDialog.vNumberOfItems ++;
				ISelectItemDialog.vItems[1] = "Check Disk on D:";
			}
			//Pass control to dialog box and display
			int nResponse = ISelectItemDialog.DoModal();
			//dialog box is now closed, if user pressed select do this
			//if user pressed cancel, do nothing
			if (nResponse == IDOK)
			{
				int WhichDriveToCheckDisk = ISelectItemDialog.vItemNumberSelected; // <<<--- answer select back here
				if (WriteFilesToRunCheckDisk(WhichDriveToCheckDisk) && (WhichDriveToCheckDisk!=2))	//if chkdsk cmd sent successfully and C:
				{	//Ask if shutdown scantrac now
					// shutdown the system
					if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)
					{
						CNoticeDialog TempNoticeDialog;
						TempNoticeDialog.vNoticeText = "\n\nTo run the Check Disk program,\nthe system must be rebooted.\nReboot after inspection is finished.";
						TempNoticeDialog.vType = cInformationMessage;
						TempNoticeDialog.DoModal();
					}
					else
					{
						CYesNoDialog TempYesNoDialog;
						TempYesNoDialog.vNoticeText =  _T("\nTo run the Check Disk program,\nthe system must reboot");
						TempYesNoDialog.vYesButtonText = "Reboot Now";
						TempYesNoDialog.vNoButtonText = "Reboot Later";
						TempYesNoDialog.vQuestionType = cConfirmSave;
						int TempResult = TempYesNoDialog.DoModal();
						//if user said OK
						if (TempResult == IDOK)
						{
							vMainWindowPointer->SendXRayPowerOnOffToUController(0);
							vMainWindowPointer->vSystemData.vShuttingDownXRays = true;
							if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)
								vMainWindowPointer->StopRunning(true);

							SystemShutDown(cReboot);
						}
					}
				}
			}
			else if (nResponse != 11)
			{
				int TempOK = false;
				if (nResponse == 10)
				{
					Sleep(1); //is the equivelent of a yeild statement;
					PrepareAndExit(10);
				}
			}
		} // end if windows xp
		else	// if not xp or after, put up a notice
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\n\nCheck Disk is not available on WindowsNT";
			TempNoticeDialog.vType = cInformationMessage;
			TempNoticeDialog.DoModal();
		}
	}
}

void CFrontEndDiagnosticDialog::OnSubFunction5Button() 
{
	if (PasswordOK(cTemporaryInspxPassword,false))
	if (vAlternateFunctions)
	{
		CString TempRegistryValue = "00";
		CRegKey TempRegistryKey;

		//LONG TempErrorCode = TempRegistryKey.Open(HKEY_LOCAL_MACHINE,"SYSTEM",KEY_ALL_ACCESS );
		//LONG TempErrorCode = TempRegistryKey.Open(HKEY_LOCAL_MACHINE,"SYSTEM",KEY_QUERY_VALUE );

		//if (TempErrorCode == ERROR_SUCCESS)
		{
			LONG TempErrorCode = TempRegistryKey.Open(HKEY_LOCAL_MACHINE, _T("System\\CurrentControlSet\\Control\\Session Manager"));

			if (TempErrorCode == ERROR_SUCCESS)
			{
				TempErrorCode = TempRegistryKey.SetKeyValue(_T(""), TempRegistryValue, _T("BootExecute"));
				if (TempErrorCode == ERROR_SUCCESS)
				{
					CNoticeDialog TempNoticeDialog;
					TempNoticeDialog.vNoticeText = "\n\n\nSuccessfully Cleared Run Autocheck";
					TempNoticeDialog.vType = cInformationMessage;
					TempNoticeDialog.DoModal();
				}
				else
				{
					CNoticeDialog TempNoticeDialog;
					TempNoticeDialog.vNoticeText = "\n\n\nFailed to clear Autocheck";
					TempNoticeDialog.vType = cErrorMessage;
					TempNoticeDialog.DoModal();
				}
			}
		}
	}
	else
	{
		StopContinuousCommunicationTests(); 
		//Delete old image files from the disk
		CYesNoDialog TempYesNoDialog;
		CString TempDirectory(vLocalConfigurationData->vScanTracImagesDirectory);
		TempDirectory = "\n\nDelete all files from the " + 
			TempDirectory + " directory?";
		TempYesNoDialog.vNoticeText = TempDirectory;
		TempYesNoDialog.vQuestionType = cConfirmQuestion;
		int TempResult = TempYesNoDialog.DoModal();
		//if user said OK delete files in log directory 
		if (TempResult == IDOK)
		{
			CString TempFileType = vLocalConfigurationData->vScanTracImagesDirectory;
			vMainWindowPointer->DeleteFilesInDirectory(TempFileType);
		}
		if (TempResult != IDOK)
		{
			CString TempFileType = "*.*";
			CYesNoDialog TempYesNoDialog;
			CString TempDirectory(vLocalConfigurationData->vScanTracImagesDirectory);
			TempDirectory = "\n\nDelete Selected files from the " + 
				TempDirectory + " directory?";
			TempYesNoDialog.vNoticeText = TempDirectory;
			TempYesNoDialog.vQuestionType = cConfirmQuestion;
			int TempResult = TempYesNoDialog.DoModal();
			//if user said OK delete files in log directory 
			if (TempResult == IDOK)
			{
				WIN32_FIND_DATA FindFileData;
				HANDLE hFind;

				CString TempString(vLocalConfigurationData->vScanTracImagesDirectory);
				TempString = TempString + TempFileType;
				LPTSTR TempDirectoryName = TempString.GetBuffer(TempString.GetLength());

				hFind = FindFirstFile(TempDirectoryName, &FindFileData);

				if (hFind == INVALID_HANDLE_VALUE) 
				{
					CNoticeDialog TempNoticeDialog;
					TempNoticeDialog.vNoticeText = "\n\n\nNo files in image directory to delete";
					TempNoticeDialog.vType = cInformationMessage;
					TempNoticeDialog.DoModal();
				} 
				else 
				{
					CString TempString;
					CString TempDirectory(vLocalConfigurationData->vScanTracImagesDirectory);
					TempString = TempDirectory + FindFileData.cFileName;
					CString TempFileNameString = TempDirectory + FindFileData.cFileName;
					if (!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
					//if (TempFileNameString.GetAt(TempString.GetLength() - 1) != '.')
					{
						CYesNoDialog TempYesNoDialog;
						CString TempString = "\n\nDelete file: " + TempFileNameString + "?";
						TempYesNoDialog.vNoticeText = TempString;
						TempYesNoDialog.vQuestionType = cConfirmQuestion;
						int TempResult = TempYesNoDialog.DoModal();
						if (TempResult == IDOK)
						if (!DeleteFile(TempString))
							ReportErrorMessage("Failed to Delete File: " + TempString, cEMailInspx, 32000);
					}
					while (FindNextFile(hFind,&FindFileData))
					{
						CString TempFileNameString = TempDirectory + FindFileData.cFileName;
						if (!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
						//if (TempFileNameString.GetAt(TempString.GetLength() - 1) != '.')
						{
							CYesNoDialog TempYesNoDialog;
							CString TempString = "\n\nDelete file: " + TempFileNameString + "?";
							TempYesNoDialog.vNoticeText = TempString;
							TempYesNoDialog.vQuestionType = cConfirmQuestion;
							int TempResult = TempYesNoDialog.DoModal();
							if (TempResult == IDOK)
							if (!DeleteFile(TempFileNameString))
								ReportErrorMessage("Failed to Delete File: " + TempFileNameString, cEMailInspx, 32000);
						}
					}
				}
				FindClose(hFind);
			}
			else
			if (vLocalConfigurationData->vScanTracScreenShotDirectory.GetLength() > 4)
			{
				CString TempDirectory(vLocalConfigurationData->vScanTracScreenShotDirectory);
				TempDirectory = "\n\nDelete all files from the " + 
					TempDirectory + " directory?";
				TempYesNoDialog.vNoticeText = TempDirectory;
				TempYesNoDialog.vQuestionType = cConfirmQuestion;
				int TempResult = TempYesNoDialog.DoModal();
				//if user said OK delete files in log directory 
				if (TempResult == IDOK)
				{
					CString TempFileType = vLocalConfigurationData->vScanTracScreenShotDirectory;
					vMainWindowPointer->DeleteFilesInDirectory(TempFileType);

					if ((vLocalConfigurationData->vScanTracScreenShotDirectory.GetAt(0) != 'C') && (vLocalConfigurationData->vScanTracScreenShotDirectory.GetAt(0) != 'D') &&
						(vLocalConfigurationData->vScanTracScreenShotDirectory.GetAt(0) != 'c') && (vLocalConfigurationData->vScanTracScreenShotDirectory.GetAt(0) != 'd')) 
					{
						TempDirectory = "\n\nDelete all files from the " ;
						TempDirectory += cScanTracScreenShotDirectory;
						TempDirectory += " directory?";
						TempYesNoDialog.vNoticeText = TempDirectory;
						TempYesNoDialog.vQuestionType = cConfirmQuestion;
						int TempResult = TempYesNoDialog.DoModal();
						//if user said OK delete files in log directory 
						if (TempResult == IDOK)
						{
							CString TempFileType = cScanTracScreenShotDirectory;
							vMainWindowPointer->DeleteFilesInDirectory(TempFileType);
						}
					}
				}
			}
		}
	}
}

void CFrontEndDiagnosticDialog::OnSubFunction6Button() 
{
	if (PasswordOK(cTemporaryInspxPassword,false))
	if (vAlternateFunctions)
	{
		if ((vGlobaluCSerialPort) && (vGlobaluCSerialPort->vPortOpen) && (vGlobalFirstMicroControllerCommCheck == cFinishedSearchForNextSerialPort))
		{
			if (vGlobalFPGAVersion15Point31OrHigher) //does a verify of the DACs against the last programmed value
			if (vLocalConfigurationData->vShowOnlyRealImages)
			{
				vLocalSystemData->vDACChannelWrote = vLocalConfigurationData->vNumberOfXRayDetectors;

				for (BYTE TempLoopD = 0; TempLoopD < cMaximumNumberOfDetectors12; TempLoopD++)
					vLocalSystemData->vLatestDACValueRead[TempLoopD]= 0;

				vLocalSystemData->vVerifyDACValues = vLocalConfigurationData->vNumberOfXRayDetectors + 1; //add one extra, so when done, value is one and can generate report
				vLocalSystemData->vGenerateDACVerifyReport = true;
				vLocalSystemData->vOldVerifyDACValues = vLocalSystemData->vVerifyDACValues;
				ReportErrorMessage("Start DAC Verify Values - Manual Test", cAction, 0);

				vMainWindowPointer->StartDACTest();
			}
			else
			{
				CNoticeDialog TempNoticeDialog;
				TempNoticeDialog.vNoticeText = "\n\n\nRequires FPGA Version 15.33 or higher\nto read DAC values";
				TempNoticeDialog.vType = cNoticeMessage;
				TempNoticeDialog.DoModal();
			}
		}
	}
	else
	{
		//Delete old log files from the disk
		StopContinuousCommunicationTests(); 
		CYesNoDialog TempYesNoDialog;
		CString TempDirectory(vLocalConfigurationData->vScanTracSerialLogsDirectory);
		TempDirectory = "\n\nDelete all files from the " + 
			TempDirectory + " directory?";
		TempYesNoDialog.vNoticeText = TempDirectory;
		TempYesNoDialog.vQuestionType = cConfirmQuestion;
		int TempResult = 0;
			TempResult = TempYesNoDialog.DoModal();
		//if user said OK delete files in log directory 
		if (TempResult == IDOK)
		{
			CString TempFileType = vLocalConfigurationData->vScanTracSerialLogsDirectory;
			vMainWindowPointer->DeleteFilesInDirectory(TempFileType);
			vSerialLogDirectoryFull = false;
			UpdateButtons78();
		}
		else
		{
			TempDirectory = (vLocalConfigurationData->vScanTracProductionReportDirectory);
			TempDirectory = "\n\nDelete all files from the " + TempDirectory + " directory?";
			TempYesNoDialog.vNoticeText = TempDirectory;
			TempYesNoDialog.vQuestionType = cConfirmQuestion;
			TempResult = 0;
				TempResult = TempYesNoDialog.DoModal();
			//if user said OK delete files in log directory 
			if (TempResult == IDOK)
			{
				CString TempFileType = 	vLocalConfigurationData->vScanTracProductionReportDirectory;
				vMainWindowPointer->DeleteFilesInDirectory(TempFileType);
		// if there is a shadow directory, delete those files too
				if ((vLocalConfigurationData->vScanTracProductionReportDirectory.GetAt(0) != 'C') && (vLocalConfigurationData->vScanTracProductionReportDirectory.GetAt(0) != 'D') &&
					(vLocalConfigurationData->vScanTracProductionReportDirectory.GetAt(0) != 'c') && (vLocalConfigurationData->vScanTracProductionReportDirectory.GetAt(0) != 'd')) 
				{
					TempDirectory = "\n\nDelete all files from the " ;
					TempDirectory += cScanTracProductionReportDirectory;
					TempDirectory += " directory?";
					TempYesNoDialog.vNoticeText = TempDirectory;
					TempYesNoDialog.vQuestionType = cConfirmQuestion;
					int TempResult = TempYesNoDialog.DoModal();
					//if user said OK delete files in log directory 
					if (TempResult == IDOK)
					{
						CString TempFileType = cScanTracProductionReportDirectory;
						vMainWindowPointer->DeleteFilesInDirectory(TempFileType);
					}
				}
			}
			else
			{
				TempDirectory = vLocalConfigurationData->vScanTracEventLogDirectory;
				TempDirectory = "\n\nDelete all files from the " + TempDirectory + " directory?";
				TempYesNoDialog.vNoticeText = TempDirectory;
				TempYesNoDialog.vQuestionType = cConfirmQuestion;
				TempResult = 0;
					TempResult = TempYesNoDialog.DoModal();
				//if user said OK delete files in log directory 
				if (TempResult == IDOK)
				{
					CString TempFileType = vLocalConfigurationData->vScanTracEventLogDirectory;
					vMainWindowPointer->DeleteFilesInDirectory(TempFileType);

			// if there is a shadow directory, delete those files too
					if ((vLocalConfigurationData->vScanTracEventLogDirectory.GetAt(0) != 'C') && (vLocalConfigurationData->vScanTracEventLogDirectory.GetAt(0) != 'D') &&
						(vLocalConfigurationData->vScanTracEventLogDirectory.GetAt(0) != 'c') && (vLocalConfigurationData->vScanTracEventLogDirectory.GetAt(0) != 'd')) 
					{
						TempDirectory = "\n\nDelete all files from the " ;
						TempDirectory += cScanTracEventLogDirectory;
						TempDirectory += " directory?";
						TempYesNoDialog.vNoticeText = TempDirectory;
						TempYesNoDialog.vQuestionType = cConfirmQuestion;
						int TempResult = TempYesNoDialog.DoModal();
						//if user said OK delete files in log directory 
						if (TempResult == IDOK)
						{
							CString TempFileType = cScanTracEventLogDirectory;
							vMainWindowPointer->DeleteFilesInDirectory(TempFileType);
						}
					}
				}
			}
		}
	}
}

void CFrontEndDiagnosticDialog::OnSubFunction7Button() 
{
	//if ((vGlobaluCSerialPort) && (vGlobaluCSerialPort->vPortOpen) && (vGlobalFirstMicroControllerCommCheck == cFinishedSearchForNextSerialPort))
	//{
	if (PasswordOK(cTemporaryInspxPassword,false))
	if (vAlternateFunctions)
	{
		if (vGlobalFPGAVersion15Point31OrHigher)
		{
			vLocalSystemData->vADCChannelWrote = vLocalConfigurationData->vNumberOfXRayDetectors;

			for (BYTE TempLoopD = 0; TempLoopD < cMaximumNumberOfDetectors12; TempLoopD++)
				vLocalSystemData->vLatestADCValueRead[TempLoopD] = 0;

			vLocalSystemData->vVerifyADCValues = vLocalConfigurationData->vNumberOfXRayDetectors + 1; //add one extra, so when done, value is one and can generate report
			vLocalSystemData->vOldVerifyADCValues = vLocalSystemData->vVerifyADCValues;
			ReportErrorMessage("Start ADC Verify Values", cAction, 0);

			vMainWindowPointer->StartADCTest();
		}
		else
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\n\nRequires FPGA Version 15.33 or higher\nto read ADC values";
			TempNoticeDialog.vType = cNoticeMessage;
			TempNoticeDialog.DoModal();
		}
//#ifdef UseNewTestModeCode
//		// uController simulate 16 hz
//		if (vLocalSystemData->vuCSimulate16Hz)
//		{
//			SendGenerate16HzCommand(2);	
//			vLocalSystemData->vuCSimulate16Hz = false;
//			SetDlgItemText(IDC_SubFunction7Button,"uC Generate 16 Hz!");
//		}
//		else
//		{
//			SendGenerate16HzCommand(1);	
//			vLocalSystemData->vuCSimulate16Hz = true;
//			SetDlgItemText(IDC_SubFunction7Button,"uC Stop Generate 16 Hz!");
//		}
//#endif
	}
	else
	{
		vSerialLogDirectoryFull = false;
		// User pressed Log Serial Data button
		if (vLocalSystemData->vLogFile.vLogSerialData)
		{
			vLocalSystemData->vLogFile.CloseLogFile();
			vLocalSystemData->vLogFile.vLogSerialData = false;
		}
		else
		{
			if (!vLocalConfigurationData->vDemoMode)
			{
				// check directory size ??
				//if (CalculateDirectorySize(vLocalConfigurationData->vScanTracSerialLogsDirectory) > cMaxSerialLogDirectorySize)
				//{
				//	vLocalSystemData->vLogFile.vLogSerialData = false;
				//	vSerialLogDirectoryFull = true;
				//	ReportErrorMessage("No New Serial Log File, 12 GB Directory Size Exceeded", cEMailInspx,32002); //write message to inpsx log file 
				//	
				//	CString TempDirectoryCString = "\n\nSerial Log Directory > ";
				//	if (cMaxSerialLogDirectorySize >= 0x40000000)
				//		TempDirectoryCString += (dtoaWithCommas(cMaxSerialLogDirectorySize / 0x40000000, 0) + " GB");
				//	else
				//	if (cMaxSerialLogDirectorySize >= 0x100000)
				//		TempDirectoryCString += (dtoaWithCommas(cMaxSerialLogDirectorySize / 0x100000, 0) + " MB");
				//	else
				//		TempDirectoryCString += (dtoaWithCommas(cMaxSerialLogDirectorySize, 0) + " Bytes");
				//	TempDirectoryCString += "\nStart Serial Log Fail";

				//	CNoticeDialog TempNoticeDialog;
				//	TempNoticeDialog.vNoticeText = TempDirectoryCString;
				//	TempNoticeDialog.vType = cNoticeMessage;
				//	TempNoticeDialog.DoModal();
				//}
				//else
				{
					vLocalSystemData->vLogFile.vLogDirectory = vLocalConfigurationData->vScanTracSerialLogsDirectory;
					if (!vLocalSystemData->vLogFile.OpenLogFile())
					{
						ReportErrorMessage("Error-Could Not Open Serial Log File", cEMailInspx,32000); 
					}
					else
						CheckDiskSpaceFree("C:\\");

					if (!vLocalSystemData->vLogFile.vSerialDataLogFile)
					{
						//vLocalSystemData->vLogFile.vLogSerialData = false;
						//ReportErrorMessage("Could Not Open Serial Log File On Server",cError,0);
						CNoticeDialog TempNoticeDialog;
						TempNoticeDialog.vNoticeText = "\n\n\nCould Not Open Serial Log File On Server";
						TempNoticeDialog.vType = cEMailInspx;
						TempNoticeDialog.DoModal();
					}
				}
			}
		}
		UpdateButtons78();
	}
	//}
}

void CFrontEndDiagnosticDialog::OnSubFunction8Button() 
{
	if (PasswordOK(cTemporaryInspxPassword,false))
	if (vAlternateFunctions)
	{
		vMainWindowPointer->ClearDesignatedFrameGrabberMemorySettings(true);
	}
	else
	{
	if ((vGlobaluCSerialPort) && (vGlobaluCSerialPort->vPortOpen) && (vGlobalFirstMicroControllerCommCheck == cFinishedSearchForNextSerialPort))
	{
		//trigger watch dog timmer
		CYesNoDialog TempYesNoDialog;
		TempYesNoDialog.vNoticeText =  _T("\n\nReset Micro Controller by Triggering Watchdog?");
		TempYesNoDialog.vYesButtonText = "Reset uC";
		TempYesNoDialog.vQuestionType = cConfirmQuestion;
		int TempResult = TempYesNoDialog.DoModal();
		//if user said OK cause watchdog timer reset 
		if (TempResult == IDOK)
		{
			vMainWindowPointer->StopRunning(true);
			ReportErrorMessage("Operator sent uC a WatchDog Reset command", cUserAction,0); //cWriteToLog);
			ReportErrorMessage("Operator sent uC a WatchDog Reset command", cEMailInspx,0); //cWriteToLog);

			tSerialCommand TempCommand;
			TempCommand[0] = 0x35;
			TempCommand[1] = 0x34;
			TempCommand[2] = 0x33;
			TempCommand[3] = 0x32;
			if (vGlobaluCSerialPort)
				vGlobaluCSerialPort->SendSerialCommand(TempCommand);
			//go to main menu
			PrepareAndExit(10);
		}
	}
	}
}

BOOL CFrontEndDiagnosticDialog::PreTranslateMessage(MSG* pMsg) 
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
	if (pMsg->message == cSentCommTestMessage)
	{
		if (vGlobaluCSerialPort)
		if ((!vHoldingSentTestDisplay) || ((!vGlobaluCSerialPort->vSendingContinuous) && (!vSendingTimerContinuous)))
		{
			vHoldingSentTestDisplay = true;
			KillTimer(vHoldingSentTestDisplayTimerHandle);
			int TempTimerResult = SetTimer(vHoldingSentTestDisplayTimerHandle,500,NULL);
			if (!TempTimerResult)
				ReportErrorMessage("Error-Hold Sent Test Display Timer Failed",cEMailInspx,32000);

			SetDlgItemText(IDC_SendCount, dtoa(vGlobaluCSerialPort->vSendCount, 0) + " Sent");
		}
		return true;  //true indicates it has been handled, so do not process
	}
	else
	if (pMsg->message == cGotTestCommReplyMessage)
	{
		if (vGlobaluCSerialPort)
		if ((!vHoldingTestDisplay) ||  ((!vGlobaluCSerialPort->vSendingContinuous) && (!vSendingTimerContinuous)))
		{
			vHoldingTestDisplay = true;
			KillTimer(vHoldingTestDisplayTimerHandle);
			int TempTimerResult = SetTimer(vHoldingTestDisplayTimerHandle,500,NULL);
			if (!TempTimerResult)
				ReportErrorMessage("Error-Hold Test Display Timer Failed",cEMailInspx,32000);

			if ((vHoldingTestDisplay) || (vGlobaluCSerialPort->vSendingContinuous) || (vSendingTimerContinuous))
			{
				SetDlgItemText(IDC_CommStatus,dtoa(vGlobaluCSerialPort->vReceiveCount,0) + " Testing");
			}
			else
			{
				if (vGlobaluCSerialPort->vSendCount == vGlobaluCSerialPort->vReceiveCount)
					SetDlgItemText(IDC_CommStatus,dtoa(vGlobaluCSerialPort->vReceiveCount,0) + " Comm OK");
				else
					SetDlgItemText(IDC_CommStatus,dtoa(vGlobaluCSerialPort->vReceiveCount,0) + " Received Bad");

				SetDlgItemText(IDC_SendCount,dtoa(vGlobaluCSerialPort->vSendCount,0) + " Sent");
			}
		}
		return true;  //true indicates it has been handled, so do not process
	}
	else
	if (pMsg->message == cDataReceivedMessage)
	{
		if (!vHoldingReceivedDisplay)
		{
			vHoldingReceivedDisplay = true;
			if (vGlobalWriteToLogLock.Lock())
			{
				SetDlgItemText(IDC_ReceivedSerialDataStaticText, vLocalSystemData->vLogFile.vLastDataReceived);
				vGlobalWriteToLogLock.Unlock();
			}
			KillTimer(vHoldingReceivedDisplayTimerHandle);
			int TempTimerResult = SetTimer(vHoldingReceivedDisplayTimerHandle,500,NULL);
			if (!TempTimerResult)
				ReportErrorMessage("Error-Hold Received Display Timer Failed",cEMailInspx, 32000);
		}
		return true;  //Message was handled, so do not process anymore
	}
	if (pMsg->message == cDataSentMessage)
	{
		if (!vHoldingSentDisplay)
		{
			vHoldingSentDisplay = true;
			if (vGlobalWriteToLogLock.Lock())
			{
				SetDlgItemText(IDC_SentSerialDataStaticText,vLocalSystemData->vLogFile.vLastDataSent);
				vGlobalWriteToLogLock.Unlock();
			}
			KillTimer(vHoldingSentDisplayTimerHandle);
			int TempTimerResult = SetTimer(vHoldingSentDisplayTimerHandle,500,NULL);
			if (!TempTimerResult)
				ReportErrorMessage("Error-Hold Sent Display Timer Failed",cEMailInspx, 32000);
		}
		return true;  //Message was handled, so do not process anymore
	}
	if (pMsg->message == WM_KEYDOWN)
	{
		if (vGlobalShiftKeyDown)
			PostThreadMessage(vGlobalCallingThreadID,cHiddenKeyPressed, 0, pMsg->wParam);
		else
		switch (pMsg->wParam)
		{
			case 35: // End Key, Left Right on duo but new Image Key on Universal Overlays, Image Button
				OnTestdacsbutton();
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
			case cPageUp: // Clear, but do inspx hidden dialog
			{
				if (vGlobalPasswordLevel)
				{
					if (vAlternateFunctions)
						OnFunction5Button();
					PasswordOK(cNoPassword, false);	//todo, search on PasswordOK (cNoPassword)
				}
				else
				{
					PasswordOK(cAnyPasswordToEnter, true);

					if (PasswordOK(cTemporaryInspxPassword, false))
					{
						// kjh 8/11/10 in password mode, kill timer for ping, change text in message
						// hide LED components
						vGlobalPingReply = cPingResultNotSent;

						//m_Network1GreenFlash.ShowWindow(SW_HIDE);	// turn green light off
						//m_Network1RedFlash.ShowWindow(SW_HIDE);		// turn red light off
					}
				}
				UpdateInspxItems();
			}
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
				StopContinuousCommunicationTests(); 
				PostThreadMessage(vGlobalCallingThreadID,
					cShowHelp, 43, 0);
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

HBRUSH CFrontEndDiagnosticDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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

	if (pWnd->GetDlgCtrlID() == IDC_SendCount)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SendCount, 4);  //5 is medium large
		if (vOldDACTestErrorCount)
		{
			pDC->SetBkMode(TRANSPARENT);
			return vLocalSystemData->vRedBrush;
		}
	}
	

	if (pWnd->GetDlgCtrlID() == IDC_CommStatus)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_CommStatus, 4);  //5 is medium large
		if (vGlobaluCSerialPort->vSendCount != vGlobaluCSerialPort->vReceiveCount)
		if ((!vGlobaluCSerialPort->vSendingContinuous) || 
			(vGlobaluCSerialPort->vReceiveCount + 3 < vGlobaluCSerialPort->vSendCount))
		if (!((vHoldingTestDisplay) || (vGlobaluCSerialPort->vSendingContinuous) || (vSendingTimerContinuous)))
		{
			pDC->SetBkMode(TRANSPARENT);
			return vLocalSystemData->vRedBrush;
		}
	}
	if (pWnd->GetDlgCtrlID() == IDC_GatewayLabel2)	
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_GatewayLabel2, 4);  //5 is medium large
		if (vGlobaluCSerialPort->vSendCount != vGlobaluCSerialPort->vReceiveCount)
		if ((!vGlobaluCSerialPort->vSendingContinuous) || 
			(vGlobaluCSerialPort->vReceiveCount + 3 < vGlobaluCSerialPort->vSendCount))
		if (!((vHoldingTestDisplay) || (vGlobaluCSerialPort->vSendingContinuous) || (vSendingTimerContinuous)))
		{
			pDC->SetBkMode(TRANSPARENT);
			return vLocalSystemData->vRedBrush;
		}
	}
	if (pWnd->GetDlgCtrlID() == IDC_IPAddressDisplay)
		TextSize = SetTextSizeMultilineDisplay(TempDialogHwnd, pWnd, pDC, &m_IPAddressDisplay, 4);
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
	if (pWnd->GetDlgCtrlID() == IDC_XRayCommunications)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_XRayCommunications, 4);  //5 is medium large
		if (vOldXRayNoCommuncations)
		{
			pDC->SetBkMode(TRANSPARENT);
			return vLocalSystemData->vRedBrush;
		}
	}	

	if (pWnd->GetDlgCtrlID() == IDC_UControllerCommunications)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_UControllerCommunications, 4);  //5 is medium large
		if (vOldNoCommunicationsWithUController)
		{
			pDC->SetBkMode(TRANSPARENT);
			return vLocalSystemData->vRedBrush;
		}
	}
	if (pWnd->GetDlgCtrlID() == IDC_uCResetCount)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_uCResetCount, 4);  //5 is medium large
		if (vOlduCResetCount > 1)
		{
			pDC->SetBkMode(TRANSPARENT);
			return vLocalSystemData->vRedBrush;
		}
	}
	if (pWnd->GetDlgCtrlID() == IDC_uCWatchDogCount)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_uCWatchDogCount, 4);  //5 is medium large
		if (vOlduCWatchDogCount > 1)
		{
			pDC->SetBkMode(TRANSPARENT);
			return vLocalSystemData->vRedBrush;
		}
	}

	if (pWnd->GetDlgCtrlID() == IDC_Function5Button)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function5Button, 5);  //5 is medium large
		if (vAlternateFunctions)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cButtonTextColor);
			return vLocalSystemData->vGreenBrush;
		}

		if (!vAlternateFunctions)
		if ((vLocalSystemData->vuCSimulate16Hz) ||
			(vLocalSystemData->vuControllerSendExtraDiagnosticMessages) || 
			(vLocalSystemData->vuCOutputSerialDebugData))
		{
			pDC->SetAttribDC(pDC->m_hDC);
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cButtonTextColor);
			return vLocalSystemData->vGreenBrush;
		}
	}

	if (pWnd->GetDlgCtrlID() == IDC_SubFunction7Button)
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction7Button, 5);  //5 is medium large
		if (vLocalSystemData->vLogFile.vLogSerialData)
		if (!vAlternateFunctions)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cButtonTextColor);
			return vLocalSystemData->vGreenBrush;
		}
	}

	if (pWnd->GetDlgCtrlID() == IDC_SubFunction1Button)
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction1Button, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		if (vSubFunction1ButtonEnable == true)
			pDC->SetTextColor(cBlack);
		else
			pDC->SetTextColor(cGray);

		if (vAlternateFunctions)
		{
			if (!vLocalSystemData->vLogFile.vLogSyncData)
			{
				return vLocalSystemData->vGreenBrush;
			}
		}
		return vGlobalButtonColorBrush;
	}

	if (pWnd->GetDlgCtrlID() == IDC_SubFunction2Button)
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction2Button, 5);  //5 is medium large
		if (vLocalSystemData->vuControllerSendExtraDiagnosticMessages)
		if (vAlternateFunctions)
		{
			pDC->SetBkMode(TRANSPARENT);
			if (vSubFunction2ButtonEnable == true)
				pDC->SetTextColor(cButtonTextColor);
			else
				pDC->SetTextColor(cGray);
			return vLocalSystemData->vGreenBrush;
		}


		if (vGlobaluCSerialPort)
		if (vGlobaluCSerialPort->vSendingContinuous)
		if (!vAlternateFunctions)
		{
			pDC->SetAttribDC(pDC->m_hDC);
			pDC->SetBkMode(TRANSPARENT);
			if (vSubFunction2ButtonEnable == true)
				pDC->SetTextColor(cButtonTextColor);
			else
				pDC->SetTextColor(cGray);
			return vLocalSystemData->vGreenBrush;
		}
	}

	if (pWnd->GetDlgCtrlID() == IDC_SubFunction3Button)
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction3Button, 5);  
		if (vLocalSystemData->vuCOutputSerialDebugData)
		if (vAlternateFunctions)
		{
			pDC->SetAttribDC(pDC->m_hDC);
			pDC->SetBkMode(TRANSPARENT);
			if (vSubFunction3ButtonEnable == true)
				pDC->SetTextColor(cButtonTextColor);
			else
				pDC->SetTextColor(cGray);
			return vLocalSystemData->vGreenBrush;
		}
		if (vSendingTimerContinuous)
		if (!vAlternateFunctions)
		{
			pDC->SetAttribDC(pDC->m_hDC);
			pDC->SetBkMode(TRANSPARENT);
			if (vSubFunction3ButtonEnable == true)
				pDC->SetTextColor(cButtonTextColor);
			else
				pDC->SetTextColor(cGray);
			return vLocalSystemData->vGreenBrush;
		}
	}

	if (pWnd->GetDlgCtrlID() == IDC_SubFunction7Button)
	{
		if (vAlternateFunctions)
		if (vLocalSystemData->vuCSimulate16Hz)
		{
			pDC->SetAttribDC(pDC->m_hDC);
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cButtonTextColor);
			return vLocalSystemData->vGreenBrush;
		}
	}
	if (pWnd->GetDlgCtrlID() == IDC_uCLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_uCLabel, 4);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		if ((vGlobaluCSerialPort) && (!vGlobaluCSerialPort->vPortOpen) && (vGlobalFirstMicroControllerCommCheck == cNoCommPortSettings))
			return vLocalSystemData->vRedBrush;
		else
			return vLocalSystemData->vLightBrownBrush;
  }
	if (pWnd->GetDlgCtrlID() == IDC_Tx)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Tx, 4);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Com2)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Com2, 4);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_NotMonitored)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_NotMonitored, 4);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Rx)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Rx, 4);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_UPS)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_UPS, 4);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_PowerMonitorLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_PowerMonitorLabel, 4);
		pDC->SetBkMode(TRANSPARENT);
		if ((vGlobalACPowerMoniter) && (!vGlobalACPowerMoniter->vPortOpen) && (vGlobalACPowerMoniter->vTestACPowerMoniterExists == cNoCommPortFound))
			return vLocalSystemData->vRedBrush;
		else
			return vLocalSystemData->vLightBrownBrush;
  }

	if (pWnd->GetDlgCtrlID() == IDC_PowerMonitorPortLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_PowerMonitorPortLabel, 4);
			pDC->SetBkMode(TRANSPARENT);
		if (vGlobalSerialPortNumberACPowerMonitor == cDEFAULT_AC_POWER_MONITOR_SERIAL_PORT_NUMBER) 
			return vLocalSystemData->vLightBrownBrush;
		else
			return vLocalSystemData->vYellowBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DXM_MonoPortLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DXM_MonoPortLabel, 4);
			pDC->SetBkMode(TRANSPARENT);
		if (vGlobalSerialPortNumberXRaySource	== cDEFAULT_XRAY_SERIAL_PORT_NUMBER) 
			return vLocalSystemData->vLightBrownBrush;
		else
			return vLocalSystemData->vYellowBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_MicroCPortLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_MicroCPortLabel, 4);
			pDC->SetBkMode(TRANSPARENT);
			if (vGlobalSerialPortNumberMicroController == cDEFAULT_MICRO_CONTROLLER_SERIAL_PORT_NUMBER)
				return vLocalSystemData->vLightBrownBrush;
			else
				return vLocalSystemData->vYellowBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_HVPSType)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_HVPSType, 4);
		pDC->SetBkMode(TRANSPARENT);
		if ((vGlobalDXMXRayPort) && (!vGlobalDXMXRayPort->vPortOpen) && (vGlobalXrayNoCommMessageRcvdEver == cNoCommPortFound))
			return vLocalSystemData->vRedBrush;
		else
		if ((vGlobalMonoBlockXRayPort) && (!vGlobalMonoBlockXRayPort->vPortOpen) /*&& (vGlobalXrayNoCommMessageRcvdEver == cGiveUpOnCommPort)*/)
			return vLocalSystemData->vRedBrush;
		else
			return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Background)
	{
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Com5Label) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Com5Label, 4);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_InterlocksLabel) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_InterlocksLabel, 4);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_XRayLabel) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_XRayLabel, 4);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_UControllerCommunicationsLabel) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_UControllerCommunicationsLabel, 4);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_ComPortLabel) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_ComPortLabel, 4);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_uCResetCountLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_uCResetCountLabel, 4);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_uCWatchDogCountLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_uCWatchDogCountLabel, 4);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DataSentLabel) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DataSentLabel, 4);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DataReceivedLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DataReceivedLabel, 4);
		pDC->SetAttribDC(pDC->m_hDC);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_HVPSType) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_HVPSType, 4);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_GatewayLabel) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_GatewayLabel, 4);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_GatewayLabel2) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_GatewayLabel2, 4);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_MicroCPortLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_MicroCPortLabel, 4);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_PowerMonitorPortLabel) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_PowerMonitorPortLabel, 4);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_ReceivedSerialDataStaticText) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_ReceivedSerialDataStaticText, 4);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_XRayPort) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_XRayPort, 4);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_XRayCommunications) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_XRayCommunications, 4);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_UControllerCommunications) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_UControllerCommunications, 4);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_ComPort) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_ComPort, 4);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_uCResetCount) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SendCount, 4);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_uCWatchDogCount) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SendCount, 4);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SendCount) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SendCount, 4);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_CommStatus)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_CommStatus, 4);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SentSerialDataStaticText)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SentSerialDataStaticText, 4);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_LoggingSerialData)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_LoggingSerialData, 4);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
	}
	if (pWnd->GetDlgCtrlID() == IDC_LoggingSyncData)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_LoggingSyncData, 4);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
	}
	if (pWnd->GetDlgCtrlID() == IDC_Command1)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Command1, 4);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
	}
	if (pWnd->GetDlgCtrlID() == IDC_Command2)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Command2, 4);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
	}
	if (pWnd->GetDlgCtrlID() == IDC_Command3)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Command3, 4);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
	}
	if (pWnd->GetDlgCtrlID() == IDC_Command4)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Command4, 4);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
	}
	/*
	if ((pWnd->GetDlgCtrlID() == IDC_LoggingSyncData) ||
		(pWnd->GetDlgCtrlID() == IDC_ImageDataType) ||
		(pWnd->GetDlgCtrlID() == IDC_LoggingSerialData))
	{
		pDC->SetBkMode(TRANSPARENT);
		//return vWhiteBrush;
  }
	*/
	if (pWnd->GetDlgCtrlID() == IDC_XRaysOn)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_XRaysOn, 7);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cYellow);
		pDC->SetBkColor(cRed);
		return vLocalSystemData->vRedBrush;
	}

	if (pWnd->GetDlgCtrlID() == IDC_TestDACsButton)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_TestDACsButton, 5);
		if (vTestingDACs)
		{
			pDC->SetAttribDC(pDC->m_hDC);
			pDC->SetBkMode(TRANSPARENT);
			return vLocalSystemData->vRedBrush;
		}
		else
		{
			pDC->SetTextColor(cButtonTextColor);
			return vGlobalButtonColorBrush;
		}
	}

	if (pWnd->GetDlgCtrlID() == IDC_TestADCsButton)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_TestADCsButton, 5);
		pDC->SetBkMode(TRANSPARENT);
		if (vTestingADCs)
		{
		return vLocalSystemData->vRedBrush;
		}
		else
		{
			pDC->SetTextColor(cButtonTextColor);
			return vGlobalButtonColorBrush;
		}
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

void CFrontEndDiagnosticDialog::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	
	if (nIDEvent == vHoldingSentTestDisplayTimerHandle)
	{
		vHoldingSentTestDisplay = false;
		KillTimer(vHoldingSentTestDisplayTimerHandle);
	}
	else
	if (nIDEvent == vHoldingTestDisplayTimerHandle)
	{
		vHoldingTestDisplay = false;
		KillTimer(vHoldingTestDisplayTimerHandle);
	}
	else
	if (nIDEvent == vHoldingReceivedDisplayTimerHandle)
	{
		vHoldingReceivedDisplay = false;
		KillTimer(vHoldingReceivedDisplayTimerHandle);
	}
	else
	if (nIDEvent == vHoldingSentDisplayTimerHandle)
	{
		vHoldingSentDisplay = false;
		KillTimer(vHoldingSentDisplayTimerHandle);
	}
	else
	if (nIDEvent == vTestCommTimerHandle)
	{
		if (vGlobaluCSerialPort)
			vGlobaluCSerialPort->SendCommTest();
	}
	else
  if (nIDEvent == vUpdateDisplayTimerHandle)
		UpdateDisplay();

// add ping timer if not in password protected mode
	if (nIDEvent == vPingTimerHandle)	
  {
		// if there is an answer from the thread, update results and run the thread again
		if (vGlobalPingStatus == cPingThreadDone)
		{
			PingTimeOut = 0;
			UpdatePingDisplay();
			vGlobalPingTestThread = NULL;
			vGlobalPingStatus = cPingThreadNull;
		}
		else
		if (!vGlobalPingTestThread)
		{
			// start Thread
			PingTimeOut = 0;
			CreatePingTestThread();
		}
		else
		{
			PingTimeOut++;
			if (PingTimeOut > 9)
			{
				vGlobalPingStatus = cPingThreadDone;
				ReportErrorMessage("Error-Ping Time Out after 10 seconds 1", cEMailInspx,32000); 
			}
		}
	}

	CDialog::OnTimer(nIDEvent);
}

void CFrontEndDiagnosticDialog::SetChangeMade() 
{
	/*
	if (!vChangeMade)
	{
		if (!vAlternateFunctions)
		{
			m_Function2Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
			m_Function4Button.ShowWindow(SW_SHOW);
			m_ArrowDown4.ShowWindow(SW_SHOW);
			SetDlgItemText(IDC_Function2Button,"Save\nand Exit");
			SetDlgItemText(IDC_Function4Button,"Cancel");
		}
		vChangeMade = true;
	}
	*/
}

void CFrontEndDiagnosticDialog::UpdateDisplay()
{
	vHaveAUSBMemoryStick = false;

	double TempDriveSpaceFree = CheckDiskSpaceFree("D:\\");
	if (TempDriveSpaceFree > 1000000) //greater than a megabyte
	if (TempDriveSpaceFree < 35000000000) //less than 35 gigabytes
		vHaveAUSBMemoryStick = true;

	TempDriveSpaceFree = CheckDiskSpaceFree("E:\\");
	if (TempDriveSpaceFree > 1000000) //greater than a megabyte
		vHaveAUSBMemoryStick = true;

	TempDriveSpaceFree = CheckDiskSpaceFree("F:\\");
	if (TempDriveSpaceFree > 1000000) //greater than a megabyte
		vHaveAUSBMemoryStick = true;

	TempDriveSpaceFree = CheckDiskSpaceFree("G:\\");
	if (TempDriveSpaceFree > 1000000) //greater than a megabyte
		vHaveAUSBMemoryStick = true;

	TempDriveSpaceFree = CheckDiskSpaceFree("H:\\");
	if (TempDriveSpaceFree > 1000000) //greater than a megabyte
		vHaveAUSBMemoryStick = true;

	TempDriveSpaceFree = CheckDiskSpaceFree("I:\\");
	if (TempDriveSpaceFree > 1000000) //greater than a megabyte
		vHaveAUSBMemoryStick = true;

	TempDriveSpaceFree = CheckDiskSpaceFree("J:\\");
	if (TempDriveSpaceFree > 1000000) //greater than a megabyte
		vHaveAUSBMemoryStick = true;

	if (vOldHaveAUSBMemoryStick != vHaveAUSBMemoryStick)
	{
		vOldHaveAUSBMemoryStick = vHaveAUSBMemoryStick;
		UpdateButtons78();
	}

	if (vOldDACTestCount != vLocalSystemData->vDACTestCount)
	{
		if ((vOldDACTestCount == 0) && (vOldDACTestErrorCount == 0))
			SetDlgItemText(IDC_SendCount, _T("DAC Test Errors:  0"));

		vOldDACTestCount = vLocalSystemData->vDACTestCount;
		SetDlgItemText(IDC_CommStatus,"DAC Test: " + dtoa(vOldDACTestCount,0));
	}

	if (vOldDACTestErrorCount != vLocalSystemData->vDACTestErrorCount)
	{
		vOldDACTestErrorCount = vLocalSystemData->vDACTestErrorCount;
		SetDlgItemText(IDC_SendCount,"DAC Test Errors:  " + dtoa(vOldDACTestErrorCount,0));
	}

	if (vOldADCTestCount != vLocalSystemData->vADCTestCount)
	{
		if ((vOldADCTestCount == 0) && (vOldADCTestErrorCount == 0))
			SetDlgItemText(IDC_SendCount, _T("ADC Test Errors:  0"));

		vOldADCTestCount = vLocalSystemData->vADCTestCount;
		SetDlgItemText(IDC_CommStatus,"ADC Test: " + dtoa(vOldADCTestCount,0));
	}

	if (vOldADCTestErrorCount != vLocalSystemData->vADCTestErrorCount)
	{
		vOldADCTestErrorCount = vLocalSystemData->vADCTestErrorCount;
		SetDlgItemText(IDC_SendCount,"ADC Test Errors:  " + dtoa(vOldADCTestErrorCount,0));
	}

	if (vOldPasswordLevel != vGlobalPasswordLevel)
		UpdateInspxItems();

	bool TempTesting = ((vHoldingTestDisplay) || (vGlobaluCSerialPort->vSendingContinuous) || (vSendingTimerContinuous));
	if (vOldTesting != TempTesting)
	{
		vOldTesting = TempTesting;

		if (!vOldTesting)
		{
			if (vGlobaluCSerialPort)
			if (vGlobaluCSerialPort->vSendCount == vGlobaluCSerialPort->vReceiveCount)
				SetDlgItemText(IDC_CommStatus,dtoa(vGlobaluCSerialPort->vReceiveCount,0) + " Comm OK");
			else
				SetDlgItemText(IDC_CommStatus,dtoa(vGlobaluCSerialPort->vReceiveCount,0) + " Received Bad");

			SetDlgItemText(IDC_SendCount,dtoa(vGlobaluCSerialPort->vSendCount,0) + " Sent");
		}
	}
	if (vGlobalDXMXRayPort)
	{
		if (vGlobalDXMXRayPort->vNoCommunications != vOldXRayNoCommuncations)
		{
			vOldXRayNoCommuncations = vGlobalDXMXRayPort->vNoCommunications;
			if (vOldXRayNoCommuncations)
				SetDlgItemText(IDC_XRayCommunications, _T("No Comm"));
			else
				SetDlgItemText(IDC_XRayCommunications, _T("OK"));
		}
	}
	else
	if (vGlobalMonoBlockXRayPort)
	{
		if (vGlobalMonoBlockXRayPort->vNoCommunications != vOldXRayNoCommuncations)
		{
			vOldXRayNoCommuncations = vGlobalMonoBlockXRayPort->vNoCommunications;
			if (vOldXRayNoCommuncations)
				SetDlgItemText(IDC_XRayCommunications, _T("No Comm"));
			else
				SetDlgItemText(IDC_XRayCommunications, _T("OK"));
		}
	}

	if (vOldXRayOnNotice != vLocalSystemData->vXRayOnNotice)
	{
		vOldXRayOnNotice = vLocalSystemData->vXRayOnNotice;
		if (vOldXRayOnNotice)
			m_XRaysOn.ShowWindow(SW_SHOW);
		else
			m_XRaysOn.ShowWindow(SW_HIDE);
	}
	if (vOldNoCommunicationsWithUController != vLocalSystemData->vNoCommunicationsWithUController)
	{
		vOldNoCommunicationsWithUController = vLocalSystemData->vNoCommunicationsWithUController;
		if ((vLocalSystemData->vSourceInterLocks & 0x0040) != 0x0040)
		{
			if ((!vLocalSystemData->vFPGAVersion9Point1OrHigher) && (vLocalSystemData->vFPGAMajorVersionNumber))
				SetDlgItemText(IDC_UControllerCommunications, _T("Shutting Down"));
			else
				SetDlgItemText(IDC_UControllerCommunications, _T("Fault"));

			if (vGlobalFPGAVersion10Point0OrHigher)
				SetDlgItemText(IDC_UControllerCommunications, _T("OKo"));
		}
		else
		if (!vOldNoCommunicationsWithUController)
			SetDlgItemText(IDC_UControllerCommunications, _T("OK"));
		else
			SetDlgItemText(IDC_UControllerCommunications, _T("Fault"));
	}
	if (vOlduCWatchDogCount != vLocalSystemData->vuCWatchDogCount)
	{
		vOlduCWatchDogCount = vLocalSystemData->vuCWatchDogCount;
		if (vOlduCWatchDogCount)
			SetDlgItemText(IDC_uCWatchDogCount, dtoa(vOlduCWatchDogCount - 1, 0));
	}
	if (vOlduCResetCount != vLocalSystemData->vuCResetCount)
	{
		vOlduCResetCount = vLocalSystemData->vuCResetCount;
		//subtract one from reset count since reset when start up
		if (vOlduCResetCount)
			SetDlgItemText(IDC_uCResetCount, dtoa(vOlduCResetCount - 1, 0));
	}

// monitor port changes here
	if (vGlobalACPowerMoniter)//the power monitor module exists
	{
		if ((!vGlobalACPowerMoniter->vPortOpen) && (vGlobalACPowerMoniter->vTestACPowerMoniterExists == cNoCommPortFound))
		{
			CString TempAString;
			CString TempBString;
			TempAString = " Power Monitor-NO PORT";
			GetDlgItemText(IDC_PowerMonitorPortLabel, TempBString);
			if (TempAString.CompareNoCase(TempBString) != 0)
			{
				SetDlgItemText(IDC_PowerMonitorLabel, TempAString);
				//m_PowerMonitorLabel.SetWindowPos(NULL,266,171,190,20,SWP_NOZORDER);
				m_RxBackGroundCom4.ShowWindow(SW_HIDE);
			}
		}
		else
		{
			CString TempAString;
			TempAString.Format(_T("Com %d"),vGlobalSerialPortNumberACPowerMonitor);
			CString TempBString;
			GetDlgItemText(IDC_PowerMonitorPortLabel, TempBString);
			if (TempAString.CompareNoCase(TempBString) != 0)	// compare, no case
			{
				SetDlgItemText(IDC_PowerMonitorPortLabel, TempAString);
			}		
		}
	}
	if (vGlobalDXMXRayPort) //the DXM exists
	{
		if ((!vGlobalDXMXRayPort->vPortOpen) && (vGlobalXrayNoCommMessageRcvdEver == cNoCommPortFound))
		{
			CString TempAString;
			CString TempBString;
			TempAString = vGlobalDXMXRayPort->vHVPSType + ": NO PORT";
			GetDlgItemText(IDC_HVPSType,TempBString);
			if (TempAString.CompareNoCase(TempBString) != 0)
			{
				SetDlgItemText(IDC_HVPSType, vGlobalDXMXRayPort->vHVPSType + ": NO PORT");
				//m_HVPSType.SetWindowPos(NULL,266,139,190,20,SWP_NOZORDER);
				m_RxBackGroundCom3.ShowWindow(SW_HIDE);
				m_TxBackGroundCom3.ShowWindow(SW_HIDE);
			}
		}
		else
		{
			CString TempAString;
			TempAString.Format(_T("Com %d"),vGlobalSerialPortNumberXRaySource);
			CString TempBString;
			GetDlgItemText(IDC_DXM_MonoPortLabel, TempBString);
			if (TempAString.CompareNoCase(TempBString) != 0)	// compare, no case
			{
				SetDlgItemText(IDC_DXM_MonoPortLabel, TempAString);
			}
		}
	}
	if (vGlobalMonoBlockXRayPort)
	{
		if ((!vGlobalMonoBlockXRayPort->vPortOpen) /*&& (vGlobalMonoBlockXRayPort->vGlobalXrayNoCommMessageRcvdEver == cGiveUpOnCommPort)*/)
		{
			CString TempAString;
			CString TempBString;
			TempAString = " XRB210W-NO PORT";
			GetDlgItemText(IDC_HVPSType,TempBString);
			if (TempAString.CompareNoCase(TempBString) != 0)
			{
				SetDlgItemText(IDC_HVPSType, TempAString);
				//m_HVPSType.SetWindowPos(NULL,266,139,190,20,SWP_NOZORDER);
				m_RxBackGroundCom3.ShowWindow(SW_HIDE);
				m_TxBackGroundCom3.ShowWindow(SW_HIDE);
			}
		}
		else
		{
			CString TempAString;
			TempAString.Format(_T("Com %d"),vGlobalSerialPortNumberXRaySource);
			CString TempBString;
			GetDlgItemText(IDC_DXM_MonoPortLabel, TempBString);
			if (TempAString.CompareNoCase(TempBString) != 0)	// compare, no case
			{
				SetDlgItemText(IDC_DXM_MonoPortLabel, TempAString);
			}
		}
	}
	if (vGlobaluCSerialPort)
	{
		if ((!vGlobaluCSerialPort->vPortOpen) && (vGlobalFirstMicroControllerCommCheck == cNoCommPortSettings))
		{
			CString TempAString;
			CString TempBString;
			TempAString.Format(_T(" uC-NO PORT"));
			GetDlgItemText(IDC_uCLabel,TempBString);
			if (TempAString.CompareNoCase(TempBString) != 0)	// compare, no case
			{
				SetDlgItemText(IDC_uCLabel,TempAString);
				//m_uCLabel.SetWindowPos(NULL,266,69,190,20,SWP_NOZORDER);
				m_RxBackGroundCom1.ShowWindow(SW_HIDE);
				m_TxBackGroundCom1.ShowWindow(SW_HIDE);
			}
		}
		else
		{
			CString TempAString;
			CString TempBString;
			TempAString.Format(_T("Com %d"),vGlobalSerialPortNumberMicroController);
			GetDlgItemText(IDC_MicroCPortLabel, TempBString);
			if (TempAString.CompareNoCase(TempBString) != 0)	// compare, no case
			{
				SetDlgItemText(IDC_MicroCPortLabel, TempAString);
			}
		}
	}
	if ((vGlobalMonoBlockXRayPort) && (!vGlobalMonoBlockXRayPort->vPortOpen))
	{
		CString TempAString;
		CString TempBString;
		TempAString.Format(_T(" XRB210W-NO PORT"));
		GetDlgItemText(IDC_HVPSType,TempBString);
		if (TempAString.CompareNoCase(TempBString) != 0)	// compare, no case
		{
			SetDlgItemText(IDC_HVPSType, _T(" XRB210W-NO PORT"));
			//m_HVPSType.SetWindowPos(NULL,266,139,190,20,SWP_NOZORDER);
			m_RxBackGroundCom3.ShowWindow(SW_HIDE);
			m_TxBackGroundCom3.ShowWindow(SW_HIDE);
		}
	}

	vLightsOnTime++;
	if (vLightsOnTime == 7)
	{
		//flash lights if there has been communication with that device	
		if (vGlobalACPowerMoniter)//the power monitor module exists
		{
			if (vGlobalACPowerMoniter->vHasReceivedCommunication)//communication occurred
			{
				vGlobalACPowerMoniter->vHasReceivedCommunication = false;//clear communication flag
				m_Com4GreenFlash.ShowWindow(SW_SHOW);//turn red light on
			}
			else
				m_Com4GreenFlash.ShowWindow(SW_HIDE);//turn red light off
		}

		if (vGlobalDXMXRayPort)//the DXM exists
		{
			if (vGlobalDXMXRayPort->vHasReceivedCommunication)//communication was recieved
			{
				vGlobalDXMXRayPort->vHasReceivedCommunication = false;//clear flag
				m_Com3GreenFlash.ShowWindow(SW_SHOW);//turn green light on
			}
			else
				m_Com3GreenFlash.ShowWindow(SW_HIDE);//turn red light off

			if (vGlobalDXMXRayPort->vHasTransmittedCommunication)//communication was transmitted
			{
				vGlobalDXMXRayPort->vHasTransmittedCommunication = false;//clear flag
				m_Com3RedFlash.ShowWindow(SW_SHOW);//turn red light on //show flash
			}
			else
				m_Com3RedFlash.ShowWindow(SW_HIDE);//turn red light off
		}

		if (vGlobalMonoBlockXRayPort)
		{
			if (vGlobalMonoBlockXRayPort->vHasReceivedCommunication)//communication was recieved
			{
				vGlobalMonoBlockXRayPort->vHasReceivedCommunication = false;//clear flag
				m_Com3GreenFlash.ShowWindow(SW_SHOW);//turn green light on
			}
			else
				m_Com3GreenFlash.ShowWindow(SW_HIDE);//turn red light off

			if (vGlobalMonoBlockXRayPort->vHasTransmittedCommunication)//communication was transmitted
			{
				vGlobalMonoBlockXRayPort->vHasTransmittedCommunication = false;//clear flag
				m_Com3RedFlash.ShowWindow(SW_SHOW);//turn red light on //show flash
			}
			else
				m_Com3RedFlash.ShowWindow(SW_HIDE);//turn red light off
		}

		if (vGlobaluCSerialPort) //the uC exists
		{
			if (vGlobaluCSerialPort->vHasReceivedCommunication)//communication was recieved
			{
				vGlobaluCSerialPort->vHasReceivedCommunication = false;//clear flag
				m_Com1GreenFlash.ShowWindow(SW_SHOW);//turn green light on
			}
			else
				m_Com1GreenFlash.ShowWindow(SW_HIDE);//turn red light off

			if (vGlobaluCSerialPort->vHasTransmittedCommunication)//communication was transmitted
			{
				vGlobaluCSerialPort->vHasTransmittedCommunication = false;//clear flag
				m_Com1RedFlash.ShowWindow(SW_SHOW);//turn red light on //show flash
			}
			else
				m_Com1RedFlash.ShowWindow(SW_HIDE);//turn red light off
		}

		if ((vGlobalGuardMaster) && (vGlobalGuardMaster->vDetectedValidGuardMasterDevice)) //the Interlock System exists
		{
			if (vGlobalGuardMaster->vHasReceivedCommunication)//communication was recieved
			{
				vGlobalGuardMaster->vHasReceivedCommunication = false;//clear flag
				m_Com5GreenFlash.ShowWindow(SW_SHOW);//turn green light on
			}
			else
				m_Com5GreenFlash.ShowWindow(SW_HIDE);//turn red light off

			if (vGlobalGuardMaster->vHasTransmittedCommunication)//communication was transmitted
			{
				vGlobalGuardMaster->vHasTransmittedCommunication = false;//clear flag
				m_Com5RedFlash.ShowWindow(SW_SHOW);//turn red light on //show flash
			}
			else
				m_Com5RedFlash.ShowWindow(SW_HIDE);//turn red light off
		}

		if ((vGlobalScanTracType == cAtempoScanTrac) || (vGlobalScanTracType == cForteScanTrac) || (vGlobalScanTracType == cAllegroScanTrac) || 
			(vGlobalScanTracType == cCaseInspectorScanTrac))
		//if (vGlobalPowerFlex40) //the motor controller is there
		//{
		//	if (vGlobalPowerFlex40->vHasReceivedCommunication)//communication was recieved
		//	{
		//		vGlobalPowerFlex40->vHasReceivedCommunication = false;//clear flag
		//		m_Com2GreenFlash.ShowWindow(SW_SHOW);//turn green light on
		//	}
		//	else
		//		m_Com2GreenFlash.ShowWindow(SW_HIDE);//turn red light off

		//	if (vGlobalPowerFlex40->vHasTransmittedCommunication)//communication was transmitted
		//	{
		//		vGlobalPowerFlex40->vHasTransmittedCommunication = false;//clear flag
		//		m_Com2RedFlash.ShowWindow(SW_SHOW);//turn red light on //show flash
		//	}
		//	else
		//		m_Com2RedFlash.ShowWindow(SW_HIDE);//turn red light off
		//}
		vLightsOnTime = 0;
	}
	/*
	else //if lights off time
	{
		m_Com1RedFlash.ShowWindow(SW_HIDE);
		m_Com1GreenFlash.ShowWindow(SW_HIDE);
		m_Com3RedFlash.ShowWindow(SW_HIDE);
		m_Com3GreenFlash.ShowWindow(SW_HIDE);
		m_Com4GreenFlash.ShowWindow(SW_HIDE);
		vLightsOnTime = 0;
	}
	*/
}

void CFrontEndDiagnosticDialog::SenduCSerialDebug(bool TempOn)
{
	tSerialCommand TempCommand;
	TempCommand[0] = 0x33;
	TempCommand[1] = 0;
	if (TempOn)
		TempCommand[2] = 1;
	else
		TempCommand[2] = 2;
	TempCommand[3] = 0;
	if (vGlobaluCSerialPort)
		vGlobaluCSerialPort->SendSerialCommand(TempCommand);
}

void CFrontEndDiagnosticDialog::SendGenerate16HzCommand(BYTE TempOn)
{
	tSerialCommand TempCommand;
	TempCommand[0] = 0x33;
	TempCommand[1] = TempOn;
	TempCommand[2] = 0;
	TempCommand[3] = 0;
	if (vGlobaluCSerialPort)
		vGlobaluCSerialPort->SendSerialCommand(TempCommand);
}

void CFrontEndDiagnosticDialog::UpdateButtons78()
{
	//display button SubFunction1
	if (vAlternateFunctions)
	{
		if (PasswordOK(cTemporaryInspxPassword,false))
		{
			CString TempString = "";
			if (PasswordOK(cTemporaryInspxPassword,false))
				vSubFunction1ButtonEnable = true;
			else
				vSubFunction1ButtonEnable = false;

			if (vLocalSystemData->vLogFile.vLogSyncData)
			{

				SetDlgItemText(IDC_LoggingSyncData,_T("Sending Sync Data"));
				GetDlgItemText(IDC_SubFunction1Button, TempString);
				if (TempString != "Don't Send Sync\nData!")
					SetDlgItemText(IDC_SubFunction1Button,_T("Don't Send Sync\nData!"));
			}
			else
			{
				SetDlgItemText(IDC_LoggingSyncData,_T("Not Sending Sync Data"));
				GetDlgItemText(IDC_SubFunction1Button, TempString);
				if (TempString != "Send\nSync\nData!")
					SetDlgItemText(IDC_SubFunction1Button,_T("Send\nSync\nData!"));
			}
		}
		else
		{
			m_LoggingSerialData.ShowWindow(SW_HIDE);
		}
	}
	else
	{
		if (vOldHaveAUSBMemoryStick)
			vSubFunction1ButtonEnable = true;
		else
			vSubFunction1ButtonEnable = false;
		SetDlgItemText(IDC_SubFunction1Button, _T("USB Memory Stick Functions"));
	}

	if ((!vAlternateFunctions) && (PasswordOK(cTemporaryInspxPassword,false)))
	{
		if (vLocalSystemData->vLogFile.vLogSerialData)
		{
			m_LoggingSerialData.ShowWindow(SW_SHOW);
			SetDlgItemText(IDC_LoggingSerialData,_T(" Logging Serial Data"));
			SetDlgItemText(IDC_SubFunction7Button,_T("Don't Log Serial Data!"));
		}
		else
		{
			if (vSerialLogDirectoryFull == true)
			{
				CString TempDirectoryCString = "Serial Dir > ";
				if (cMaxSerialLogDirectorySize >= 0x40000000)
					TempDirectoryCString += (dtoaWithCommas(cMaxSerialLogDirectorySize / 0x40000000, 0) + " GB");
				else
				if (cMaxSerialLogDirectorySize >= 0x100000)
					TempDirectoryCString += (dtoaWithCommas(cMaxSerialLogDirectorySize / 0x100000, 0) + " MB");
				else
					TempDirectoryCString += (dtoaWithCommas(cMaxSerialLogDirectorySize, 0) + " Bytes");
				SetDlgItemText(IDC_LoggingSerialData,TempDirectoryCString);
				m_LoggingSerialData.ShowWindow(SW_SHOW);
				SetDlgItemText(IDC_SubFunction7Button,_T("Log\nSerial Data!"));
			}
			else
			{
				m_LoggingSerialData.ShowWindow(SW_SHOW);
				SetDlgItemText(IDC_LoggingSerialData,_T(" Not Logging Serial Data"));
				SetDlgItemText(IDC_SubFunction7Button,_T("Log\nSerial Data!"));
			}
		}
	}
}

void CFrontEndDiagnosticDialog::ShowCommand()
{
	if (PasswordOK(cTemporaryInspxPassword,false))
	{
		SetDlgItemText(IDC_Command1,"0x" + ByteToHex(vLocalConfigurationData->vCommand1) + " (" + dtoa(vLocalConfigurationData->vCommand1, 0) + ")");
		SetDlgItemText(IDC_Command2,"0x" + ByteToHex(vLocalConfigurationData->vCommand2) + " (" + dtoa(vLocalConfigurationData->vCommand2, 0) + ")");
		SetDlgItemText(IDC_Command3,"0x" + ByteToHex(vLocalConfigurationData->vCommand3) + " (" + dtoa(vLocalConfigurationData->vCommand3, 0) + ")");
		SetDlgItemText(IDC_Command4,"0x" + ByteToHex(vLocalConfigurationData->vCommand4) + " (" + dtoa(vLocalConfigurationData->vCommand4, 0) + ")");
	}
}

//	CString EchoDefaultGatewayString = "224.0.0.1" ; address current not pinged,was Network2RedFlash and Network2GreenFlash
void CFrontEndDiagnosticDialog::UpdatePingDisplay() 
{
	// visual of Ping results
	if (vGlobalPingReply == cPingResultReply)
	{
		if (vPingDNS)
		{
			m_Network2GreenFlash2.ShowWindow(SW_SHOW);	// turn green light on
			m_Network2RedFlash2.ShowWindow(SW_HIDE);		// turn red light off
		}
		else
		{
			m_Network1GreenFlash.ShowWindow(SW_SHOW);	// turn green light on
			m_Network1RedFlash.ShowWindow(SW_HIDE);		// turn red light off
		}
	}
	else if (vGlobalPingReply == cPingResultNoReply) // network seen, no reply on ping 
	{
		if (vPingDNS)
		{
			m_Network2RedFlash2.ShowWindow(SW_SHOW);	// turn red light on
			m_Network2GreenFlash2.ShowWindow(SW_HIDE);	// turn red light on
		}
		else
		{
			m_Network1RedFlash.ShowWindow(SW_SHOW);	// turn red light on
			m_Network1GreenFlash.ShowWindow(SW_HIDE);	// turn green light on
		}
	}
	/*
	else	// (vIPPingGatewayReply == cPingResultNotSent)  network contection fail
	{
		m_Network1GreenFlash.ShowWindow(SW_HIDE);	// turn green light off
		m_Network1RedFlash.ShowWindow(SW_HIDE);		// turn red light off
	}
	*/
}

void CFrontEndDiagnosticDialog::CreatePingTestThread()
{
	//m_Network1GreenFlash.ShowWindow(SW_HIDE);	// turn green light off
	//m_Network1RedFlash.ShowWindow(SW_HIDE);		// turn red light off
	//m_Network2GreenFlash.ShowWindow(SW_HIDE);	// turn green light off
	//m_Network2RedFlash.ShowWindow(SW_HIDE);		// turn red light off

	vGlobalPingTestThread = (CPingTestThread *)AfxBeginThread(
		RUNTIME_CLASS(CPingTestThread),
		//THREAD_PRIORITY_LOWEST  // dont use lowest, makes it too slow
		0 // THREAD_PRIORITY_BELOW_NORMAL
		//THREAD_PRIORITY_NORMAL
		,0,CREATE_SUSPENDED);

	if (!vGlobalPingTestThread)
	{
		ReportErrorMessage("Error-Could Not Create Process Ping Test Thread 1", cEMailInspx,32000); 
		vGlobalPingStatus = cPingThreadNull;
	}
	else
	{
		vGlobalPingStatus = cPingThreadSending;
		vPingDNS = !vPingDNS;
		vGlobalPingTestThread->vPingDNS = vPingDNS;
		vGlobalPingTestThread->ResumeThread();
	}
}


void CFrontEndDiagnosticDialog::PrepareAndExit(int TempExitCode)
{
	vGlobalRunningCheckDisk = false;

	StopContinuousCommunicationTests(); 
	vLocalSystemData->vLogFile.vShowingSerialDataOnScreen = false;

	if (vTestingDACs)
		OnTestdacsbutton(); //turn off testing DACS

	if (vTestingADCs)
		OnTestadcsbutton(); //turn off testing DACS

	CDialog::EndDialog(TempExitCode);
}

void CFrontEndDiagnosticDialog::UpdateInspxItems()
{
	vOldPasswordLevel = vGlobalPasswordLevel;

	if (PasswordOK(cTemporaryInspxPassword, false))
	{
		// kjh 8/11/10 in password mode, kill timer for ping, change text in message
		// hide LED components
		vGlobalPingReply = cPingResultNotSent;

		//m_Network1GreenFlash.ShowWindow(SW_HIDE);	// turn green light off
		//m_Network1RedFlash.ShowWindow(SW_HIDE);		// turn red light off

		m_Command1.ShowWindow(SW_SHOW);
		m_Command2.ShowWindow(SW_SHOW);
		m_Command3.ShowWindow(SW_SHOW);
		m_Command4.ShowWindow(SW_SHOW);

		m_SubFunction1Button.ShowWindow(SW_SHOW);
		m_SubFunction2Button.ShowWindow(SW_SHOW);
		m_SubFunction3Button.ShowWindow(SW_SHOW);
		m_SubFunction4Button.ShowWindow(SW_SHOW);
		m_SubFunction5Button.ShowWindow(SW_SHOW);
		m_SubFunction6Button.ShowWindow(SW_SHOW);
		m_SubFunction7Button.ShowWindow(SW_SHOW);
		m_SubFunction8Button.ShowWindow(SW_SHOW);

		if (vGlobalFPGAVersion15Point31OrHigher)
		{
			m_TestDACsButton.ShowWindow(SW_SHOW);
			m_TestADCsButton.ShowWindow(SW_SHOW);
		}
		else
		{
			m_TestDACsButton.ShowWindow(SW_HIDE);
			m_TestADCsButton.ShowWindow(SW_HIDE);
		}

		m_CommStatus.ShowWindow(SW_SHOW);
		m_SendCount.ShowWindow(SW_SHOW);
		m_uCResetCountLabel.ShowWindow(SW_SHOW);
		m_uCWatchDogCountLabel.ShowWindow(SW_SHOW);
		m_DataSentLabel.ShowWindow(SW_SHOW);
		m_DataReceivedLabel.ShowWindow(SW_SHOW);
		m_uCResetCount.ShowWindow(SW_SHOW);
		m_uCWatchDogCount.ShowWindow(SW_SHOW);
		m_SentSerialDataStaticText.ShowWindow(SW_SHOW);
		m_ReceivedSerialDataStaticText.ShowWindow(SW_SHOW);
	
		m_Function5Button.ShowWindow(SW_SHOW);
		m_Function3Button.ShowWindow(SW_SHOW);
		m_Function4Button.ShowWindow(SW_SHOW);
		SetDlgItemText(IDC_Function4Button,_T("Send Test Command"));
		ShowCommand();

		m_LoggingSerialData.ShowWindow(SW_SHOW);
		m_IPAddressDisplay.ShowWindow(SW_HIDE);
	}
	else
	{
		m_Command1.ShowWindow(SW_HIDE);
		m_Command2.ShowWindow(SW_HIDE);
		m_Command3.ShowWindow(SW_HIDE);
		m_Command4.ShowWindow(SW_HIDE);
		if (!vAlternateFunctions)
		{
			m_Function5Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_Function5Button.GetRectToInvalidateArrow());
			m_Function3Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_Function3Button.GetRectToInvalidateArrow());
			m_Function4Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_Function4Button.GetRectToInvalidateArrow());
		}
		m_SubFunction2Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction2Button.GetRectToInvalidateArrow());
		m_SubFunction3Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction3Button.GetRectToInvalidateArrow());
		m_SubFunction4Button.ShowWindow(SW_SHOW);
		m_SubFunction5Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction5Button.GetRectToInvalidateArrow());
		m_SubFunction6Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction6Button.GetRectToInvalidateArrow());
		m_SubFunction7Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction7Button.GetRectToInvalidateArrow());
		m_SubFunction8Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction8Button.GetRectToInvalidateArrow());
		
		m_TestDACsButton.ShowWindow(SW_HIDE);
		m_TestADCsButton.ShowWindow(SW_HIDE);

		m_CommStatus.ShowWindow(SW_HIDE);
		m_SendCount.ShowWindow(SW_HIDE);
		m_uCResetCountLabel.ShowWindow(SW_HIDE);
		m_uCWatchDogCountLabel.ShowWindow(SW_HIDE);
		m_DataSentLabel.ShowWindow(SW_HIDE);
		m_DataReceivedLabel.ShowWindow(SW_HIDE);
		m_uCResetCount.ShowWindow(SW_HIDE);
		m_uCWatchDogCount.ShowWindow(SW_HIDE);
		m_SentSerialDataStaticText.ShowWindow(SW_HIDE);
		m_ReceivedSerialDataStaticText.ShowWindow(SW_HIDE);
		
		m_LoggingSerialData.ShowWindow(SW_HIDE);
		m_LoggingSyncData.ShowWindow(SW_HIDE);

		CString vTextString = " Windows IP Settings: " + vGlobalDHCPString[vGlobalIPDHCPEnabled] + "\n    IP Address: " + vGlobalIPAddressString
			+ "\n    SubNet Mask: " + vGlobalIPSubNetMaskString + "\n    Default Gateway: " + vGlobalIPDefaultGatewayString +
			"\n    DNS Servers: " + vGlobalIPDNSAddressString1;

		SetDlgItemText(IDC_IPAddressDisplay, vTextString);
		m_IPAddressDisplay.ShowWindow(SW_SHOW);
	}
	CWnd *TempWindow = NULL;
	TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText1);
	if (TempWindow)
		TempWindow->Invalidate(false);
}


void CFrontEndDiagnosticDialog::OnTestdacsbutton()
{
	// TODO: Add your control notification handler code here
	if ((vGlobaluCSerialPort) && (vGlobaluCSerialPort->vPortOpen) && (vGlobalFirstMicroControllerCommCheck == cFinishedSearchForNextSerialPort))
	{
	if (vGlobalFPGAVersion15Point31OrHigher)
	if (vTestingDACs)
	{
		vMainWindowPointer->StopDACTest();
		vTestingDACs = false;
		SetDlgItemText(IDC_TestDACsButton, _T("Test DACs"));
	}
	else
	if (PasswordOK(cTemporaryInspxPassword,false))
	if (vLocalSystemData->vSystemRunMode == cStoppedSystemMode)
	{
		if (vGlobalFPGAVersion15Point31OrHigher)
		{
			vLocalSystemData->vDACTestValue = 0;
			vLocalSystemData->vDACChannelWrote = vLocalConfigurationData->vNumberOfXRayDetectors;
			vLocalSystemData->vDACTestErrorCount = 0;
			vLocalSystemData->vDACTestCount = 0;
			vLocalSystemData->vOldVerifyDACValues = vLocalSystemData->vVerifyDACValues;
			vTestingDACs = true;
			vMainWindowPointer->StartDACTest();
			SetDlgItemText(IDC_TestDACsButton, _T("Stop Test"));
		}
		else
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\n\nRequires FPGA Version 15.31 or higher for DAC Test";
			TempNoticeDialog.vType = cErrorMessage;
			TempNoticeDialog.DoModal();
		}
	}
	else
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\n\nYou cannot do this test while the system is running";
		TempNoticeDialog.vType = cErrorMessage;
		TempNoticeDialog.DoModal();
	}
	}
}

void CFrontEndDiagnosticDialog::OnTestadcsbutton()
{
	// TODO: Add your control notification handler code here
	if ((vGlobaluCSerialPort) && (vGlobaluCSerialPort->vPortOpen) && (vGlobalFirstMicroControllerCommCheck == cFinishedSearchForNextSerialPort))
	{
	if (vGlobalFPGAVersion15Point31OrHigher)
	if (vTestingADCs)
	{
		vMainWindowPointer->StopADCTest();
		vTestingADCs = false;
		SetDlgItemText(IDC_TestADCsButton, _T("Test ADCs"));
	}
	else
	if (PasswordOK(cTemporaryInspxPassword,false))
	if (vLocalSystemData->vSystemRunMode == cStoppedSystemMode)
	{
		if (vGlobalFPGAVersion15Point31OrHigher)
		{
			vLocalSystemData->vADCTestValue = 0;
			vLocalSystemData->vADCChannelWrote = 6;
			vLocalSystemData->vADCTestErrorCount = 0;
			vLocalSystemData->vADCTestCount = 0;
			vLocalSystemData->vOldVerifyADCValues = vLocalSystemData->vVerifyADCValues;
			vTestingADCs = true;
			vMainWindowPointer->StartADCTest();
			SetDlgItemText(IDC_TestADCsButton, _T("Stop Test"));
		}
		else
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\n\nRequires FPGA Version 15.31 or higher for ADC Test";
			TempNoticeDialog.vType = cErrorMessage;
			TempNoticeDialog.DoModal();
		}
	}
	else
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\n\nYou cannot do this test while the system is running";
		TempNoticeDialog.vType = cErrorMessage;
		TempNoticeDialog.DoModal();
	}
	}
}


void CFrontEndDiagnosticDialog::OnDialogtitlestatictext1()
{
	BYTE TempHiddenKeys = 8;
	if (!PasswordOK(cTemporaryInspxPassword,false))
		TempHiddenKeys = 10;
	PostThreadMessage(vGlobalCallingThreadID, cDisplaySoftKeyMenu, TempHiddenKeys, TempHiddenKeys);
}
