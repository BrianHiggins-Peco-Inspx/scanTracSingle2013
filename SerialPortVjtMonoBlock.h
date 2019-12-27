//Shield Side View Source File

#if !defined(AFX_SERIALPORTVJTMONOBLOCK_H__EBF785E3_08E8_11C6_AD6D_00500466E305__INCLUDED_)
#define AFX_SERIALPORTVJTMONOBLOCK_H__EBF785E3_08E8_11C6_AD6D_00500466E305__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CSerialPortVjtMonoBlock.h : header file
//
#include "SerialPortBase.h"
#include "SerialPortModBus.h"
#include "SpellmanDXMXRaySourcePort.h"

class CSerialPortVjtMonoBlock : public CSpellmanDXMXRaySourcePort
{
	DECLARE_DYNCREATE(CSerialPortVjtMonoBlock)

	////////////////////////////////////////////////
	//
	// VJT 320w Monoblock hard limits:
	//   Tube Current : 0.2mA to 8mA
	//   Tube High Voltage : 30kV to 80kV
	//
	////////////////////////////////////////////////

/// VJT protocol uses these constants to frame it's reply.
#define STX 0x02
#define CR 0x0D
#define SP 0x20

public:
	//variables
	BYTE vGetStatusDelay;
	BYTE vDetectedValidBannerDevice;
	int vSerialPortParity;
	bool vThreadStarted;
	bool vDetectedValidVjtMonoBlockDevice;

	// public data values for application consumption.
	double vVoltage;
	double vCurrent;
	//double vTemperature; // In Spellman 
	double vFilament;

	// Fault flags
	bool vRegulationFault;
	bool vInterlockOpenFault;
	bool vCathodeOverVoltageFault;
	bool vAnodeOverVoltageFault;
	bool vOverTemperatureFault;
	bool vArcFault;
	bool vOverCurrentFault;
	bool vPowerLimitFault;
	bool vOverVoltageFault;

	bool vHvStatusOn;
	bool vWatchdogStatusOn;
	bool vWatchdogEnabledActive;
	bool vXraysPoweredOnStatus;


	bool vXraysOnSent;
	bool vProgramVoltageSent;
	bool vProgramCurrentSent;
	bool vClearFaultsSent;
	bool vXraysOnAcknowledged;
	bool vProgramVoltageAcknowledged;
	bool vProgramCurrentAcknowledged;
	bool vClearFaultsAcknowledged;

	bool vInterlockFaultReported;
	bool vRegulationFaultReported;
	bool vPowerLimitFaultReported;
	bool vCathodeOverVoltageFaultReported;
	bool vAnodeOverVoltageFaultReported;
	bool vOverTemperatureFaultReported;
	bool vArcFaultReported;
	bool vOverCurrentFaultReported;
	bool vOverVoltageFaultReported;

	int vXraysOnAcknowledgedWaitCount;
	int vProgramVoltageAcknowledgedWaitCount;
	int vProgramCurrentAcknowledgedWaitCount;
	int vClearFaultsAcknowledgedWaitCount;
	int vFaultPresentCount;

	double vVoltageToProgram;
	double vCurrentToProgram;

	CEvent vDataNotificationEvent;

	virtual void ResetStoredComPortId();

	//constructor
protected:
	CSerialPortVjtMonoBlock(void);
	~CSerialPortVjtMonoBlock(void);

private:
	BYTE vNextWatchDogEnable;
	bool vTemporarilyTurnedOffHVForLowCurrent;
	typedef BYTE tSerialCommand[10];
	int vInterlockFaultDelayCounter;
	bool vSetupDataSent;
	bool vSendWatchdogHeartBeat; // 750ms timeout
	bool vSentDeviceSetup;
	bool vSentCommLossMessage;
	bool vRecoveredFromComLoss;
	WORD vLastMessageSent;
	// Do the floating point initialization only once.
	const double vTemperatureConvertConstant = 9.0 / 5.0;
	BYTE vBackgroundMessageIntervalCount;

	CStringArray vMonitorMessageData;
	CStringArray vFaultMessageData;

	static CSemaphore vSyncSect;
	static CSerialPortVjtMonoBlock* vMyPointer;
	CWinThread * vWorkerThread;

	void SendSerialCommand(tSerialCommand TheData, BYTE TempLength);

	enum Constants
	{// These constants are command indexes.
		NO_COMMAND = 0x0,
		VOLTAGE_PROGRAM = 0x01, // "VP"
		CURRENT_PROGRAM = 0x02, // "CP"
		MONITOR = 0x03, // "MON"
		CLEAR_FAULT = 0x04, // "CLR"
		REPORT_FAULT = 0x05, // "FLT"
		HV_STATUS = 0x06, //"STAT"
		XRAY_ENABLE = 0x07, //"ENBL"
		COMM_PORT_ECHO = 0x08, //"FREV"
		WATCH_DOG_HEARTBEAT = 0x09, //"WDTE"
		WATCH_DOG_ENABLE = 0x0A, // "WDOGX"
		WATCH_DOG_STATUS = 0x0B //"WSTAT"
	};

	enum Faults
	{// Faults disable XRays amd require sending the ClearFault command.
		REGULATION_FAULT,
		INTERLOCK_OPEN_FAULT,
		CATHODE_OVER_VOLTAGE_FAULT,
		ANODE_OVER_VOLTAGE_FAULT,
		OVER_TEMPERATURE_FAULT,
		ARC_FAULT,
		OVER_CURRENT_FAULT,
		POWER_LIMIT_FAULT,
		OVER_VOLTAGE_FAULT
	};

	Constants vPendingCommand[10];
	void DemoModeMockUp();
	void ProcessMessageContents(BYTE *MessageBuffer, int MessageByteCount);
	void MessageToArray(CString message, CStringArray& array);

	void PutVjtCommandInQueue(Constants CommandId, BYTE parameters[]);
	void SendVjtCommand();
	int ComposeVoltageProgramCommand(BYTE data[]);
	int ComposeCurrentProgramCommand(BYTE data[]);
	int ComposeMonitorCommand(BYTE data[]);
	int ComposeClearFaultCommand(BYTE data[]);
	int ComposeReportFaultCommand(BYTE data[]);
	int ComposeHvStatusCommand(BYTE data[]);
	int ComposeXayEnableCommand(BYTE data[]);
	int ComposeCommPortEchoCommand(BYTE data[]);
	int ComposeGetVoltageMonitor(BYTE data[]);
	int ComposeGetCurrentMonitor(BYTE data[]);

	int ComposeWatchDogOkCommand(BYTE data[]);
	int ComposeWatchEnableCommand(BYTE data[]);
	int ComposeWatchDogStatusCommand(BYTE data[]);
	int ComposeParameterBytes(BYTE data[], double param);

	void CheckXraysOnOffAcked();
	void CheckProgramVoltageAcked();
	void CheckProgramCurrentAcked();
	void CheckClearFaultsAcked();
	void	CheckInterlockOpenFault();
	void	CheckRegulationFault();
	void	CheckPowerLimitFault();
	void	CheckCathodeOverVoltageFault();
	void	CheckAnodeOverVoltageFault();
	void	CheckOverTemperatureFault();
	void	CheckArcFault();
	void	CheckOverCurrentFault();
	void	CheckOverVoltageFault();

	void SendInitialSetupData();
	void ProcessDataMessages();
	static UINT ThreadProc(LPVOID param);
	UINT DoBackGroundWorkThreadProc();

	bool SerialMessageComplete();
	bool FrameMessageAtHeaderByte();
	bool FrameMessageAtTailByte();
	void ProcessFaultConditions();
	void ResetFaultConditions();
	void CheckFaultsWithTimeouts();
	void CheckMonitoredFaults();
	void PostErrorMessage(CString msg, bool reportError);
	void ShutDownXrays(CString reason);

	//methods
public:
	virtual void DecodeMessage();
	virtual void DoBackGroundWork();
	virtual void PauseBackGroundWork();
	virtual void ResumeBackGroundWork();
	void VerifyConnectedDevice();

	//bool GetEnableHeatbeat();
	//void SetEnableHeartbeat(bool enable);
	//void SendClearFaults();
	////void SendDeviceProgamValues();
	//void SendEnableXrays(bool OnOff);

	// Spellman compatibility API's
	virtual void SetXRayVoltage(double TempVoltage);
	virtual void SetXRayCurrent(double TempCurrent);
	virtual void TurnOnOffXRays(BYTE TempOn);
	// Override for safety
	virtual void SetInRemoteMode(bool TempRemote);
	virtual void SetXRayFilamentLimit(double TempLimit);
	virtual void SetXRayFilamentPreHeat(double TempPreHeat);
	virtual void SetFilamentLimitToDefault();
	virtual void SetFilamentLimitToZero();
	// Override for safety
	virtual void VerifySourceWithinTolerance();
	virtual void SendXRayCommand(tXRayCommand TempData, BYTE TempNumberOfBytes, BYTE TempCommandSending);
	virtual void GetXRayVoltage();
	virtual void GetXRayCurrent();
	virtual void GetXRayStatus();
	virtual void GetXRaySourceFaultData();
	virtual void TurnOnXRayWatchDog();
	virtual void TickleXRayWatchDog();
	virtual void ClearXRayFault();
	virtual BYTE CalculateCheckSum(tXRayCommand TempCommand, BYTE TempNumberOfBytes);
	virtual void GetFilamentFeedback();
	virtual void Get15VoltMonitor();
	virtual void GetInterlockStatus();
	virtual BYTE GetFirstEmptyCommandQueueSpot();
	virtual void ClearCommandQueue();
	virtual void PopFirstCommandOffQueue();
	virtual void GetSoftwareVersion();
	virtual void GetHardwareVersion();
	virtual void GetWebServerVersion();
	virtual void GetModelNumber();
	virtual void CompleteInitialization();
	virtual void GetTemperature();
	virtual void GetVoltageScale();
	virtual void GetCurrentScale();
	virtual void EnableWatchDog(bool TempEnable);
	virtual void TickleWatchDog();
	virtual void DisableWatchDogTimer();
	virtual void InitializeDXM();
	virtual void ShowSerialData();  //data received
	virtual void ProcessSerialMessage();
	virtual void RemoveMessageFromBuffer() ;
	virtual WORD ReceiveDataCount();
	void PutCommandOnQueue(Constants TempCommand);
	void RemoveCommandFromQueue();

	// Generated message map functions
	//{{AFX_MSG(CSerialPortVjtMonoBlock)
	// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSerialPortVjtMonoBlock)
public:
	//}}AFX_VIRTUAL

	// Implementation
	//Destructor
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERIALPORTVJTMONOBLOCK_H__EBF785E3_08E8_11C6_AD6D_00500466E305__INCLUDED_)
