
#if !defined(AFX_SERIAL_PORT_REVIEW_H__BF9D794D_BB26_42B8_9FB2_61D4B072010D__INCLUDED_)
#define AFX_SERIAL_PORT_REVIEW_H__BF9D794D_BB26_42B8_9FB2_61D4B072010D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define cNO_ERRORS 0
#define cERROR_PORT_NUMBER_OUT_OF_RANGE 1
#define cPORT_NUMBER_OUT_OF_RANGE 1
#define cDEFAULT_MICRO_CONTROLLER_SERIAL_PORT_NUMBER 1
#define cDEFAULT_XRAY_SERIAL_PORT_NUMBER 3
#define cDEFAULT_AC_POWER_MONITOR_SERIAL_PORT_NUMBER 4


#define cMicroControllerPort 1
#define cUPS_Port	2
#define cXRayPort	3
#define cACPowerMonitorPort 4

#define cFOUND_PORT_NOT_AVALIBLE 0
#define cFOUND_PORT_AVALIBLE 1
#define cCURRENT_TRY_MICRO_CONTROLLER_SERIAL_PORT_NUMBER 2
#define cCURRENT_TRY_XRAY_SERIAL_PORT_NUMBER 3
#define cCURRENT_TRY_AC_POWER_MONITOR_SERIAL_PORT_NUMBER 4
#define cCURRENT_COMM_MICRO_CONTROLLER_SERIAL_PORT_NUMBER 5
#define cCURRENT_COMM_XRAY_SERIAL_PORT_NUMBER 6
#define cCURRENT_COMM_AC_POWER_MONITOR_SERIAL_PORT_NUMBER 7
#define cCURRENT_COMM_UPS_PORT_NUMBER 8


enum {cInitializeSearchForSerialPort, cStartSearchForSerialPort, cSearchingForNextSerialPort, 
					cDoneSearchForNextSerialPort, cFinishedSearchForNextSerialPort, cNoCommPortSettings};

#define PORTS_EVAL 32

void TestSerialPortsValid(void) ;
//int GetSerialPortNumber(int WhichDevice);
void PortSettingsSimple(void);
//bool CheckPortScanTracuC(void);
bool TryNextSerialPort(void);		// modify vGlobaluCSerialPort->vComPortNumber to next serial port
bool GetNextACPowerMonitorPort(int NextPort);
//bool GetNextXRayMonoPort(int NextPort);
bool GetNextXRayDXMPort(int NextPort);

//bool SetACPortHere(void);	// port value in vGlobalSerialPortNumberACPowerMonitor
//bool SetXRayPortHere(void);	// port value in vGlobalSerialPortNumberXRaySource

extern char vGlobalPortHereOK[PORTS_EVAL];

int ReviewPortsForDefault(void);

int SetSerialPortToDefault(void);
int SetACMonitorPortToDefault(void);
int SetXRayPortToDefault(void);

enum {cSetToDefault, cDefaultNotCurrentlyAvailable, cDefaultNeverAvailable};

#endif