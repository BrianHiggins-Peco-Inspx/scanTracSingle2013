//ScanTrac Side View Source File

#if !defined(AFX_SERIALPORT_H__EBA785C3_08E8_11D5_AD6D_00500466E305__INCLUDED_)
#define AFX_SERIALPORT_H__EBA785C3_08E8_11D5_AD6D_00500466E305__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SerialPort.h : header file
//
#include "SerialPortBase.h"

#define SetBit(IntIn,Bit) IntIn |= Bit 
#define ClrBit(IntIn,Bit) IntIn &= ~(Bit) 
#define CompareBit(IntIn,Bit) (IntIn & Bit)

#define cAirOutsideTemperature 1
#define cAirInsideTemperature 2
#define cSurfaceTemperature 4
#define cLiquidTemperature 8
#define cNoTemperatureSave 0

const WORD cuControllerReadyToRunMessage = 65003;
const WORD cuControllerResetMessage = 65004;
const WORD cNextSourceRampStepMessage = 65005;
const WORD cuControllerStopRunningMessage = 65006;
const WORD cCheckLampsMessage = 65007;
const WORD cRampingSourceMessage = 65008;
const WORD cAuxiliaryDetectorEjectMessage = 65009;
const BYTE cNumberOfDiscoverSamples = 20;

/////////////////////////////////////////////////////////////////////////////
// CSerialPort thread

class CSerialPort : public CSerialPortBase
{
	DECLARE_DYNCREATE(CSerialPort)
protected:
	CSerialPort();           // protected constructor used by dynamic creation

// Attributes
public:
	double vPreviousSeamerObjectPulseTime;
	BYTE vNumberOfExternalDetectors;
	//variables
	//CString vSerialDisplayString;
	//WORD vBeltPositionBodyTriggerWentActive;
	bool vWaitingForContainerTriggerEnd;
	WORD vBeltPositionToEnableTriggerAfterTemporaryDisable;
	WORD vBeltPositionDisabledTrigger;
	CScanTracDlg *vMainWindowPointer;
	double vLastRecievedDataTime;
	BYTE vOldSimulatingFastestEncoder;
	bool vGaveEncoderWarning;
	WORD vPreviousAnalogChannelReadMSB;
	BYTE vAnalogMonitorChannelToRead;
	WORD vOldActualEncoderRate;
	WORD vPreviousEncoderReading;
	WORD vReceiveCount;
	CContainer *vPreviousContainer;
	CContainer *vCurrentContainer;

	bool vFlashingLights;
	bool vDiscoverModeDataIsActive[cNumberOfDiscoverSamples];
	WORD vDiscoverModeDataBeltPosition[cNumberOfDiscoverSamples];
	BYTE vDiscoverModeDataIndex;
	BYTE vDiscoverModeDataOldIndex;
	BYTE vDiscoverModeActiveDetectorNumber;
	bool vDiscoverModeOn;
	BYTE vLastDiscoverModeBits;
	int SyncFromMicroControllerSeen;
	bool ReportComNotDefaultDone;
	bool vGotBodyTriggerActiveMessage;

public:
	//methods
	//bool OpenPort(BYTE TempPort);
	//void ClosePort();
	virtual void RemoveMessageFromBuffer(); 
	//virtual bool IsMessageGood(); 
	virtual void ClearReceiver(); 
	virtual WORD ReceiveDataCount();
	virtual void ClearReceiveBuffer();

	virtual void ShowSerialData();
	//bool SerialMessageComplete();
	virtual void ClearPort();
	virtual void ProcessSerialMessage();
	virtual void DecodeMessage();
	void ProcessBeltPosition(WORD TempBeltPosition, bool TempSyncValue);
	void ProcessEjectNotification(WORD TempBeltPosition, BYTE TempEjector);
	void ProcessResynchronizedEjectNotification(WORD TempBeltPosition, BYTE TempContainerEjectNumber);
	void SendXRayPowerOnOffToUController(BYTE TempOn);
	void HandleExternalDetectorLineChange(WORD TempBeltPosition,BYTE TempDigitalLines);
	void TestContainersForEject(WORD TempCurrentBeltPosition);
	void HandleBodyTriggerLineChange(WORD TempBeltPosition, BYTE TempBodyTriggerLines);
	void HandleBodyTriggerEnd(WORD TempBeltPosition,BYTE TempBodyTriggerLines);
	CContainer *FindContainerAtStartImage(WORD TempBeltPosition);
	void VerifySourceWithinTolerance();
	void CheckuCPowerSupplyVoltages();
	void TurnCanStopOnOff(DWORD TempStopNumber, bool TempOn);
	void CheckContainers(UINT TempBTPosition);
	void SendSerialCommand(tSerialCommand TheData);
	void SetXRayVoltage(double TempVoltage);
	void SetXRayCurrent(double TempCurrent);
	bool IsBeltPositionInWindow(WORD TempPosition, WORD TempStart, WORD TempEnd);
	void EjectContainer(BYTE TempReason, CContainer *TempContainer, CInspection *TempInspection);
	bool IsPastBeltPostion(WORD TempCurrentBeltPosition, 
		WORD TempDecisionPoint, WORD TempBodyTrigger);
	CContainer* AddNewContainer(UINT TempBTPosition);
	void SendDigitalLineInterruptEnableDisable(BYTE TempLine, BYTE TempEnable, WORD TempPosition);
	void TallyEject(CContainer *TempContainer);
	void RemoveContainerFromCollection();
	void SendEjectCommand(BYTE TempEjector, WORD TempPosition);
	void SendResynchronizedEjectCommand(BYTE TempContainerNumber, BYTE TempEjector, WORD TempDelay);
	void WriteToLogFileWithBeltPosition(CString TempString, WORD TempContainerPosition);
	//void EndTheThread();
	void SetSimulatedEncoderRate(WORD TempRate);
	void SendCommTest();
	void SendEjectorOnOffCommand(DWORD TempEjectorNumber,BYTE TempOn);
	void EnableDisableEjectors(BYTE TempEnable);
	void TurnAlarmOnOff(BYTE TempAlarmNumber,bool TempOn);
	void TurnOnOffYellowLamp(BYTE TempOn);
	void TurnOnOffRedLamp(BYTE TempOn);
	void TurnOnOffBlueLamp(BYTE TempOn);
	void HandleBackupBodyTriggerChange(WORD TempBeltPosition, BYTE TempBodyTriggerLines);
	void SendEncoderDividerTouC();
	void CheckInterlocks(bool TempJustRecievedValues);
	void SendDACDetectorOffset(BYTE TempDetectorNumber);
	void SendAbsoluteDACDetectorOffset(BYTE TempDetectorNumber, double TempOffset);
	void ReadDACDetectorOffset(BYTE TempDetectorNumber);
	void ReadADCDetectorOffset(BYTE TempDetectorNumber);
	void SetRadiationDoorManualControl(WORD TempControl);
	void SendContainerTriggerToImageDelay(CProduct *TempProduct);
	void SendADCOffset(BYTE TempWhichDetectorForOffset, int TempOffsetIn);
	void SendAbsoluteADCDetectorOffset(BYTE TempWhichDetector, int TempOffsetIn);
	void ReadADCAnalogStatusValue();
	void ClearEncoderHistory();
	void TemporarilyDisableContainerTrigger();
	void EnableTriggerAfterTemporaryDisable(bool TempForceRemoveContainers);
	virtual void DoBackGroundWork();
	void StartMiddleBeamControl();
	void EndMiddleBeamControl();
	void ReadDACErrorCount();
	void ReadADCErrorCount();
#ifdef CompileInHTTPClientOperationsGuardian
	void TestOGMessage();
#endif
	void ClearEjectorSynchronizationCount();
	void CalculateDistanceToFirstSensor(WORD TempBeltPosition, BYTE TempContainerEjectNumber, BYTE TempEjectorNumber);
	BYTE GetIndexForThisEncoderRate(WORD TempEncoderRate);
	void HandleCompleteFillerMonitorDataSet();
	void HandleCompleteSeamerDataSet();
	void HandleSeamerIndexPulse();
	void HandleFillerIndexPulse();
	void HandleSeamerObjectPulse();
	void HandlePocketSensorPulse();
	void AddedNewContainer_FillInData(CContainer *TempContainer, WORD TempBeltPosition);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSerialPort)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

	// Implementation
	//Destructor
protected:
	virtual ~CSerialPort();

	// Generated message map functions
	//{{AFX_MSG(CSerialPort)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERIALPORT_H__EBA785C3_08E8_11D5_AD6D_00500466E305__INCLUDED_)
