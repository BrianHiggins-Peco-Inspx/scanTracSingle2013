//ScanTrac Side View Source File
// Spellman100WattMonoBlock.h: implementation of the Spellman100WattMonoBlock class.

#if !defined(AFX_Spellman100WattMonoBlock_H__5C0738D1_40B0_11DA_B2BF_00500466E309__INCLUDED_)
#define AFX_Spellman100WattMonoBlock_H__5C0738D1_40B0_11DA_B2BF_00500466E309__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SerialPortBase.h"
#include "SpellmanDXMXRaySourcePort.h"

class CSpellman100WattMonoBlock : public CSpellmanDXMXRaySourcePort
{
	DECLARE_DYNCREATE(CSpellman100WattMonoBlock)
public:
	//variabled
	CTime vLastTickleTime;
	bool vWatchDogEnabled;
	bool vFoundXrayPort;
//	int vNoCommMessageRcvdEver;

	//constructor
	CSpellman100WattMonoBlock(void);
	~CSpellman100WattMonoBlock(void);

	//methods
	virtual void SendXRayCommand(tXRayCommand TempData, BYTE TempNumberOfBytes, BYTE TempCommandSending);
	virtual void SetXRayVoltage(double TempVoltage);
	virtual void TurnOnOffXRays(BYTE TempOn);
	virtual void GetXRaySourceFaultData();
	virtual void ClearXRayFault();
	virtual void SetFilamentLimitToDefault();
	virtual void SetFilamentLimitToZero();
	virtual void SetXRayCurrent(double TempCurrent);
	virtual void SetXRayFilamentLimit(double TempLimit);
	virtual void SetXRayFilamentPreHeat(double TempPreHeat);
	virtual void GetXRayVoltage();
	virtual void GetXRayCurrent();
	virtual void GetXRayStatus();
	virtual void GetFilamentFeedback();
	virtual void Get15VoltMonitor();
	virtual void GetSoftwareVersion();
	virtual void GetHardwareVersion();
	virtual void GetWebServerVersion();
	virtual void GetModelNumber();
	virtual void SetInRemoteMode(bool TempRemote);
	virtual WORD ReceiveDataCount();
	virtual void GetTemperature();
	virtual bool SerialMessageComplete();
	virtual void RemoveMessageFromBuffer();
	virtual void DecodeMessage();
	virtual CString GetNumberStringFromReply();
	virtual void ShowSerialData();
	virtual void GetInterlockStatus();
	virtual void GetVoltageScale();
	virtual void GetCurrentScale();
	virtual void EnableWatchDog(bool TempEnable);
	virtual void TickleWatchDog();
	virtual void DoBackGroundWork();
	virtual void DisableWatchDogTimer();
	virtual BOOL OnIdle(LONG lCount);
	virtual int Run();
	virtual BOOL PumpMessage();
};


#endif // !defined(AFX_Spellman100WattMonoBlock_H__5C0738D1_40B0_11DA_B2BF_00500466E309__INCLUDED_)
