//ScanTrac Side View Source File

#if !defined(AFX_SerialPortPowerFlex40_H__EBA785C3_08E8_11D5_AD6D_00500466E305__INCLUDED_)
#define AFX_SerialPortPowerFlex40_H__EBA785C3_08E8_11D5_AD6D_00500466E305__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CSerialPortGuardMaster.h : header file
//
#include "SerialPortBase.h"
#include "SerialPortModBus.h"

enum {cNoPowerFlex40Command, cPowerFlex40ReadStatus, cPowerFlex40StartCommand, cPowerFlex40StopCommand, cPowerFlex40SetSpeedCommand, cPowerFlex40GetSpeed, cPowerFlex40ReadFaultCodes, 
	cPowerFlex40RemoteControlCommand, cPowerFlex40ButtonControlCommand};

const BYTE cMaximumCommandsInQueue = 20;

class CSerialPortPowerFlex40 : public CSerialPortModBus
{
	DECLARE_DYNCREATE(CSerialPortPowerFlex40)
	public:
		//variables
	BYTE vBackgroundCount;
	WORD vCurrentSpeed;
	WORD vSpeedMonitor;
	bool vDetectedValidPowerFlex40Device;
	bool vDirectionIsReverse;
	WORD vPowerFlex40Status;
	WORD vPowerFlex40FaultCodes;
	BYTE vCommandQueueu[cMaximumCommandsInQueue];

	//constructor
	CSerialPortPowerFlex40(void);
	~CSerialPortPowerFlex40(void);

	//methods
	virtual void DecodeMessage();
	void DoBackGroundWork();
	void StartStopConveyor(bool TempStart);
	void PutCommandOnQueueToSend(BYTE TempCommand);
	void SetSpeed(WORD TempSpeed);

private:
	void SendGetDeviceErrorCode();
	void SendGetStatusCommand();
	void SendInitialization();
	void SendStartGoingForwardCommand();
	void SendStartGoingBackwardCommand();
	void SendNextCommandInQueue();
	void SendSpeedCommand();
	void SendGetSpeed();
	void SendStopCommand();
	void ClearCommandQueue();
	void SendRemoteStartCommand();
	void SendStartButtonStartControlCommand();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERIALPORTGUARDMASTER_H__EBA785C3_08E8_11D5_AD6D_00500466E305__INCLUDED_)
