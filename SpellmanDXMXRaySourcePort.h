//ScanTrac Side View Source File
// SpellmanDXMXRaySourcePort.h: interface for the CSpellmanDXMXRaySourcePort class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SPELLMANDXMXRAYSOURCEPORT_H__5C0738D1_40B0_11DA_B2BF_00500466E305__INCLUDED_)
#define AFX_SPELLMANDXMXRAYSOURCEPORT_H__5C0738D1_40B0_11DA_B2BF_00500466E305__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SerialPortBase.h"

typedef BYTE tXRayCommand[20];
typedef BYTE tXRayReply[50];

const WORD cInitializeDXMMessage = 65188;
const BYTE cSemiColon = 0x3B;
const BYTE cSTX = 0x02;
const BYTE cCarriageReturn = 0x0D;
const BYTE cLineFeed = 0x0A;
const BYTE cSpellmanETX = 3;
const BYTE cComma = 0x2c;

enum {cNoCommand, cGetKV, cGetMA, cGetFilament, cGetFaults, cGetOnStatus, cGetSWVer, cGetModel, cGetHWVer, cGetSWBuild, cGet15V, cGetTemp,
	cSetKV, cSetMA, cSetOn, cResetFaults, cSetEnableWatchDog, cTickleWatchDog, cGetVoltageScale, cGetCurrentScale, cSetKVMax, cSetMAMax};  //cGetCurrentScale must be last as code checks no command is above

class CSpellmanDXMXRaySourcePort : public CSerialPortBase  
{
	DECLARE_DYNCREATE(CSpellmanDXMXRaySourcePort)
public:
	//variables
	bool vEveryOther;
	double vMinimumCurrent;
	double vVoltageMaximumScale; //only used on 100 Watt MonoBlock
	double vCurrentMaximumScale; //only used on 100 Watt MonoBlock
	double vTemperature;
	double vTemperatureMaximum;
	double vTemperatureMinimum;
	double vOldTemperature;
	double vOldTemperatureMaximum;
	double vOldTemperatureMinimum;
	bool vControllingAnMNX;
	tXRayCommand vCommandQueue[20];
	BYTE vCommandQueueCommand[20];
	CString vSoftwareVersion;
	CString vHardwareVersion;
	CString vWebserverVersion;
	CString vModelNumber;
	CString vHVPSType;

	bool vRemoteMode;
	bool vInterLockStatus;
	CString vLatestResponse;
	BYTE vNextBackGroundTask;
	double v15VoltMonitor;
	double vHVPSTemperatureMonitor;
	double vFilamentCurrentFeedbackMonitor;
	double vOldFilamentCurrentFeedbackMonitor;
	double vFilamentVoltageFeedbackMonitor;
	BYTE vHaveNewFilamentFeedbackMonitorReading;
	double vFilamentPreHeatMonitor;

	double vFilamentCurrentMin;
	double vFilamentCurrentMax;
	double vOldFilamentCurrentMin;
	double vOldFilamentCurrentMax;

	double vMultplierTemperatureMonitor;
	bool vFoundXrayPort;
//	int vNoCommMessageRcvdEver;
	bool vMaybeSendPortMessage;
	bool ReportComNotDefaultDone;

	//constructor
	CSpellmanDXMXRaySourcePort();
	virtual ~CSpellmanDXMXRaySourcePort();

	//methods
	virtual bool SerialMessageComplete();
	virtual void DecodeMessage();
	virtual void RemoveMessageFromBuffer(); 
	virtual void ShowSerialData();
	virtual WORD ReceiveDataCount();
	virtual CString GetNumberStringFromReply();
	virtual CString GetNthNumberStringFromReply(BYTE TempWhichNumber);
	//virtual CString GetHexNumberStringFromReply();
	virtual void SetXRayVoltage(double TempVoltage);
	virtual void SetXRayCurrent(double TempCurrent);
	virtual void VerifySourceWithinTolerance();
	virtual void TurnOnOffXRays(BYTE TempOn);
	virtual void SendXRayCommand(tXRayCommand TempData, BYTE TempNumberOfBytes, BYTE TempCommandSending);
	virtual void GetXRayVoltage();
	virtual void GetXRayCurrent();
	virtual void DoBackGroundWork();
	virtual void GetXRayStatus();
	virtual void GetXRaySourceFaultData();
	virtual void TurnOnXRayWatchDog();
	virtual void TickleXRayWatchDog();
	virtual void ClearXRayFault();
	virtual BYTE CalculateCheckSum(tXRayCommand TempCommand, BYTE TempNumberOfBytes);
	virtual void SetInRemoteMode(bool TempRemote);
	virtual void GetFilamentFeedback();
	//virtual void GetFilamentPreHeat();
	virtual void Get15VoltMonitor();
	virtual void SetXRayFilamentLimit(double TempLimit);
	virtual void SetXRayFilamentPreHeat(double TempPreHeat);
	virtual void GetInterlockStatus();
	virtual BYTE GetFirstEmptyCommandQueueSpot();
	virtual void ClearCommandQueue();
	virtual void PopFirstCommandOffQueue();
	virtual void GetSoftwareVersion();
	virtual void GetHardwareVersion();
	virtual void GetWebServerVersion();
	virtual void GetModelNumber();
	virtual void SetFilamentLimitToDefault();
	virtual void SetFilamentLimitToZero();
	virtual void CompleteInitialization();
	virtual void GetTemperature();
	virtual void GetVoltageScale();
	virtual void GetCurrentScale();
	virtual void EnableWatchDog(bool TempEnable);
	virtual void TickleWatchDog();
	virtual void DisableWatchDogTimer();
	//virtual virtual BOOL OnIdle(LONG lCount);
};

#endif // !defined(AFX_SPELLMANDXMXRAYSOURCEPORT_H__5C0738D1_40B0_11DA_B2BF_00500466E305__INCLUDED_)
