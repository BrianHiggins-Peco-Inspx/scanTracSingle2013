//ScanTrac Side View Source File

#if !defined(AFX_SERIALPORTGUARDMASTER_H__EBA785C3_08E8_11D5_AD6D_00500466E305__INCLUDED_)
#define AFX_SERIALPORTGUARDMASTER_H__EBA785C3_08E8_11D5_AD6D_00500466E305__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CSerialPortGuardMaster.h : header file
//
#include "SerialPortBase.h"
#include "SerialPortModBus.h"

enum {cNoGuardMasterCommand, cReadInterLockState, cReadEStop, cResetInterlockSystem, cReadOutputState, cReadFaults, cReadHardwareFaults, cSendResetCommand, cReadFaultLED, cReadFirmwareVersion};

const WORD cFrontDoorMaskBits = 0x0003;
const WORD cBackDoorMaskBits = 0x000C;
const WORD cEStopMaskBits = 0x0030;
const WORD cKeySwitchMaskBits = 0x0040; //was 0x00C0 when had 2 inputs for keyswitch, but now only have one input for key switch
//const WORD cDetectorXRayShieldsMaskBits = 0x0300;
const WORD cTubeCoverMaskBits = 0x0C00;
const WORD cLeftConveyorCoversMaskBits = 0xC000;
const DWORD cRightConveyorCoversMaskBits = 0x30000;
//const DWORD cCurtainsMaskBits = 0x30000;
const DWORD cInterlockOutputBits = 0xC0000;

const WORD cFrontDoorMaskBitA = 0x0001;
const WORD cBackDoorMaskBitA = 0x0004;
const WORD cEStopMaskBitA = 0x0010;
//const WORD cKeySwitchMaskBitA = 0x0040;
//const WORD cDetectorXRayShieldsMaskBitA = 0x0100;
const WORD cTubeCoverMaskBitA = 0x0400;
const WORD cLeftConveyorCoversMaskBitA = 0x4000;
const DWORD cRightConveyorCoversMaskBitA = 0x10000;
//const DWORD cCurtainsMaskBitA = 0x10000;

const WORD cFrontDoorMaskBitB = 0x0002;
const WORD cBackDoorMaskBitB = 0x0008;
const WORD cEStopMaskBitB = 0x0020;
//const WORD cKeySwitchMaskBitB = 0x0080; //only one input for key switch, so don't need this one
//const WORD cDetectorXRayShieldsMaskBitB = 0x0200;
const WORD cTubeCoverMaskBitB = 0x0800;
const WORD cLeftConveyorCoversMaskBitB = 0x8000;
const DWORD cRightConveyorCoversMaskBitB = 0x20000;
//const DWORD cCurtainsMaskBitB = 0x20000;

const WORD cFrontDoorFaultMaskBit = 0x0001;
const WORD cBackDoorFaultMaskBit = 0x0002;
const WORD cEStopFaultMaskBit = 0x0004;
const WORD cKeySwitchFaultMaskBit = 0x0008;
const WORD cDetectorXRayShieldsFaultMaskBit = 0x0010;
const WORD cTubeCoverFaultMaskBit = 0x0020;
const WORD cLeftConveyorCoversFaultMaskBit = 0x0040;
const WORD cRightConveyorCoversFaultMaskBit = 0x0080;

class CSerialPortGuardMaster : public CSerialPortModBus
{
	DECLARE_DYNCREATE(CSerialPortGuardMaster)
	public:
	//variables
	BYTE vDetectedValidGuardMasterDevice;
	DWORD vGuardMasterRawInterlockData; //uses 18 bits for 9 interlocks with dual safety inputs
	DWORD vOldGuardMasterRawInterlockData; //uses 18 bits for 9 interlocks with dual safety inputs
	DWORD vGuardMasterInterlockFaults;
	//BYTE vGuardMasterOuputData;
	//BYTE vOldGuardMasterOuputData;
	DWORD vGuardMasterFaultsData[6];
	DWORD vOldGuardMasterFaultsData[6];
	BYTE vBackgroundCount;
	BYTE vGuardMasterHardwareFaultsData;
	BYTE vOldGuardMasterHardwareFaultsData;
	BYTE vBackgroundFaultCount;
	BYTE vGuardMasterFaultLEDData;
	BYTE vOldGuardMasterFaultLEDData;
	WORD vGuardMasterFirmWareVersion;
	bool vGuardMasterGaveFirmwareError;

	//constructor
protected:
	CSerialPortGuardMaster(void);
	~CSerialPortGuardMaster(void);

	//methods
public:
	virtual void DecodeMessage();
	void SendClearFaultCommand();
	void SendGetInterlockStatusCommand();
	//void SendGetInterlockOutputCommand();
	void SendGetFaultsCommand();
	void SendGetFaultLEDCommand();
	void SendGetHardwareFaultsCommand();
	void SendGetFirmwareVersionCommand();
	virtual void DoBackGroundWork();
	void SendGetDeviceIDCommand();
	void SetRedStatusIndicator();
	void SendResetToGuardMaster();

	// Generated message map functions
	//{{AFX_MSG(CSerialPortGuardMaster)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSerialPortGuardMaster)
	public:
	//}}AFX_VIRTUAL

	// Implementation
	//Destructor
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERIALPORTGUARDMASTER_H__EBA785C3_08E8_11D5_AD6D_00500466E305__INCLUDED_)
