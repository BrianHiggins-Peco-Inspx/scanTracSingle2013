//ScanTrac Side View Source File
// XRaySource.h: interface for the CXRaySource class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XRAYSOURCE_H__66EDA153_E1A0_11D8_B1C6_00500466E305__INCLUDED_)
#define AFX_XRAYSOURCE_H__66EDA153_E1A0_11D8_B1C6_00500466E305__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SerialPortBase.h"

typedef BYTE tXRayCommand[20];

class CXRaySource : public CSerialPortBase  
{
	DECLARE_DYNCREATE(CXRaySource)
public:
	//variables
	CString vLatestResponse;
	BYTE vNextBackGroundTask;
	CString vModelNumber;
	BYTE vHadLowCurrent;
	BYTE vHadLowVoltage;
	bool vInterLockStatus;
//	int vNoCommMessageRcvdEver;     // because of the two different objects vGlobalDXMXRayPort can point to, CSpellmanDXMXRaySourcePort and CSpellman100WattMonoBlock,
										// vNoCommMessageRcvdEver as an int in both objects does not resolve correctly, make a global

	//constructor
	CXRaySource();
	virtual ~CXRaySource();

	//methods
	virtual bool SerialMessageComplete();
	virtual void DecodeMessage();
	virtual void RemoveMessageFromBuffer(); 
	virtual void ShowSerialData();
	CString GetNumberStringFromReply();
	CString GetHexNumberStringFromReply();
	void SendXRaySourceSettings(double TempVoltage, double TempCurrent);
	void SendSourceCurrentToUController(double TempCurrent);
	void VerifySourceWithinTolerance();
	BYTE CalculateCheckSum(tXRayCommand TempCommand);
	void TurnOnOffXRays(BYTE TempOn);
	void SendXRayCommand(tXRayCommand TempData, BYTE TempNumberOfBytes);
	void SetXRayVoltage(double TempVoltage);
	void SetXRayCurrent(double TempCurrent);
	void GetXRayVoltage();
	void GetXRayCurrent();
	virtual void DoBackGroundWork();
	void GetXRayStatus();
	void TurnOnXRayWatchDog();
	void TickleXRayWatchDog();
	void ClearXRayFault();

};

#endif // !defined(AFX_XRAYSOURCE_H__66EDA153_E1A0_11D8_B1C6_00500466E305__INCLUDED_)
