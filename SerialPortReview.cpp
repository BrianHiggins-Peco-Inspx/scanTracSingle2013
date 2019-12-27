//
//
// Files for new serial port determination
//
#include "stdafx.h"
//#include "winuser.h"
//#include "string.h"
//#include "io.h"
//#include "stdio.h"
//#include "stdlib.h"
//#include "math.h"
//#include "Winioctl.h"
#include "ScanTrac.h"
#include "SerialPort.h"
#include "SpellmanDXMXRaySourcePort.h"
#include "ACPowerMoniter.h"
#include "XRaySource.h"
#include "SerialPortGuardMaster.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "SerialPortReview.h" //needed for curve fitting

//
	// kjh added for com ports monitor
int vGlobalSerialPortNumberMicroController;	//default = 1
int vGlobalSerialPortNumberACPowerMonitor;	//default = 4
int vGlobalSerialPortNumberXRaySource;		//default = 3
int vGlobalSerialPortNumberGuardMaster;		//default = 5
int vGlobalXrayNoCommMessageRcvdEver;   // because of the two different objects vGlobalDXMXRayPort can point to, CSpellmanDXMXRaySourcePort and CSpellman100WattMonoBlock,
										// vNoCommMessageRcvdEver as an int in both objects does not resolve correctly, make a global

// kjh added for serial port control
int vGlobalFirstMicroControllerCommCheck;

//vTestACPowerMoniterExists other state of serial port for AC power monitor, part of AC power monitor object

char vGlobalPortHereOK[PORTS_EVAL];

extern CSerialPort *vGlobaluCSerialPort;
extern CSpellmanDXMXRaySourcePort *vGlobalDXMXRayPort;
extern CXRaySource *vGlobalMonoBlockXRayPort;
extern CACPowerMoniter *vGlobalACPowerMoniter;
extern CSerialPortGuardMaster *vGlobalGuardMaster;

void TestSerialPortsValid(void) 
{
	HANDLE AWinPortHandle = INVALID_HANDLE_VALUE;
	CString TempComPort;
	int SerialPortTry = 0;	//port value increments to 1 before first port read
	int index;

	// clear port Ok information
	for (index = 0; (index < PORTS_EVAL) ; index++)
	{
		vGlobalPortHereOK[index] = 0;
	}
	//set port 2 unavailable, always used for UPS
	//vGlobalPortHereOK[cUPS_Port] = cCURRENT_COMM_UPS_PORT_NUMBER; 

	while (SerialPortTry < (PORTS_EVAL-2)) 
	{
		SerialPortTry++;
		//if (SerialPortTry != cUPS_Port)
		{
			if (SerialPortTry < 10)
				TempComPort.Format(_T("COM%d"),SerialPortTry);
			else		// http://support.microsoft.com/kb/115831 info for TempComPort > Com9, for com10 or greater
				TempComPort.Format(_T("\\\\.\\COM%d"),SerialPortTry);
				
			AWinPortHandle = CreateFile(TempComPort,GENERIC_READ | GENERIC_WRITE,0,NULL,OPEN_EXISTING,FILE_FLAG_OVERLAPPED,NULL);
	// below done above for all ports
	//		if (AWinPortHandle == INVALID_HANDLE_VALUE)
	//		{
	//			printf("\nSerial Port %d is not available ", SerialPortTry);
	//			vGlobalPortHereOK[SerialPortTry] = 0;
	//		}
			if (AWinPortHandle != INVALID_HANDLE_VALUE)
			{
				COMMPROP CommProp;
				GetCommProperties(AWinPortHandle,&CommProp);
				CloseHandle(AWinPortHandle);
	//			printf("\nSerial Port COM%d: in registry ", SerialPortTry);
				vGlobalPortHereOK[SerialPortTry] = cFOUND_PORT_AVALIBLE;
			}
		}
	}
}

	// kjh added for com ports monitor
void PortSettingsSimple(void)
{
	int index;

	vGlobalSerialPortNumberMicroController = cDEFAULT_MICRO_CONTROLLER_SERIAL_PORT_NUMBER;	//#define cDEFAULT_MICRO_CONTROLLER_SERIAL_PORT_NUMBER 1
	vGlobalSerialPortNumberXRaySource = cDEFAULT_XRAY_SERIAL_PORT_NUMBER;					//#define cDEFAULT_XRAY_SERIAL_PORT_NUMBER 3
	vGlobalSerialPortNumberACPowerMonitor = cDEFAULT_AC_POWER_MONITOR_SERIAL_PORT_NUMBER;	//#define cDEFAULT_AC_POWER_MONITOR_SERIAL_PORT_NUMBER 4
	
	if (vGlobalPortHereOK[cDEFAULT_MICRO_CONTROLLER_SERIAL_PORT_NUMBER] == cFOUND_PORT_AVALIBLE)
		vGlobalPortHereOK[cDEFAULT_MICRO_CONTROLLER_SERIAL_PORT_NUMBER] = cCURRENT_TRY_MICRO_CONTROLLER_SERIAL_PORT_NUMBER;
	if (vGlobalPortHereOK[cDEFAULT_XRAY_SERIAL_PORT_NUMBER] == cFOUND_PORT_AVALIBLE)
		vGlobalPortHereOK[cDEFAULT_XRAY_SERIAL_PORT_NUMBER] = cCURRENT_TRY_XRAY_SERIAL_PORT_NUMBER;
	if (vGlobalPortHereOK[cDEFAULT_AC_POWER_MONITOR_SERIAL_PORT_NUMBER] == cFOUND_PORT_AVALIBLE)
		vGlobalPortHereOK[cDEFAULT_AC_POWER_MONITOR_SERIAL_PORT_NUMBER] = cCURRENT_TRY_AC_POWER_MONITOR_SERIAL_PORT_NUMBER;


	if (vGlobalPortHereOK[cDEFAULT_MICRO_CONTROLLER_SERIAL_PORT_NUMBER] != cCURRENT_TRY_MICRO_CONTROLLER_SERIAL_PORT_NUMBER)
	{
		index = 1;	// start looking for port at 
		while ((index < PORTS_EVAL) && (vGlobalSerialPortNumberMicroController == cDEFAULT_MICRO_CONTROLLER_SERIAL_PORT_NUMBER))
		{
			if (vGlobalPortHereOK[index] == cFOUND_PORT_AVALIBLE) 
			{
				vGlobalSerialPortNumberMicroController = index;
				vGlobalPortHereOK[index] = cCURRENT_TRY_MICRO_CONTROLLER_SERIAL_PORT_NUMBER;
			}
			index++;
		}
	}
	if ((vGlobalPortHereOK[cDEFAULT_XRAY_SERIAL_PORT_NUMBER] != cCURRENT_TRY_XRAY_SERIAL_PORT_NUMBER) && (vGlobalPortHereOK[cDEFAULT_XRAY_SERIAL_PORT_NUMBER] != cCURRENT_COMM_XRAY_SERIAL_PORT_NUMBER))
	{
		index = 1;	// start looking for port 
		while ((index < PORTS_EVAL) && (vGlobalSerialPortNumberXRaySource == cDEFAULT_XRAY_SERIAL_PORT_NUMBER))
		{
			if (vGlobalPortHereOK[index] == cFOUND_PORT_AVALIBLE)
			{
				vGlobalSerialPortNumberXRaySource = index;
				vGlobalPortHereOK[index] = cCURRENT_TRY_XRAY_SERIAL_PORT_NUMBER;
			}
			index++;
		}
	}
	if (vGlobalPortHereOK[cDEFAULT_AC_POWER_MONITOR_SERIAL_PORT_NUMBER] != cCURRENT_TRY_AC_POWER_MONITOR_SERIAL_PORT_NUMBER)
	{
		index = 1;	// start looking for port 
		while ((index < PORTS_EVAL) && (vGlobalSerialPortNumberACPowerMonitor == cDEFAULT_AC_POWER_MONITOR_SERIAL_PORT_NUMBER))
		{
			if (vGlobalPortHereOK[index] == cFOUND_PORT_AVALIBLE)
			{
				vGlobalSerialPortNumberACPowerMonitor = index;
				vGlobalPortHereOK[index] = cCURRENT_TRY_AC_POWER_MONITOR_SERIAL_PORT_NUMBER;
			}
			index++;
		}
	}
}

/*
bool CheckPortScanTracuC(void)
{
	bool ScanTracPortVerify;

	ScanTracPortVerify = false;
	if (vGlobaluCSerialPort)
	{
		CString TempxString;
		TempxString.Format(_T("\nWait For Response on Port\n"));
		bool WaitLoopDone = false;
		int WaitLoopCount = 0;
		while ((WaitLoopCount < 8) && (WaitLoopDone == false))
		{
			if (vGlobaluCSerialPort->SyncFromMicroControllerSeen > 2)
			{
				WaitLoopDone = true;
				ScanTracPortVerify = true;
			}
			Sleep(1000);
			WaitLoopCount++;
			printf(" . ");
		}
	}
	return(ScanTracPortVerify);
}
*/

//#define cFOUND_PORT_AVALIBLE 1
//#define cCURRENT_TRY_MICRO_CONTROLLER_SERIAL_PORT_NUMBER 2
//#define cCURRENT_TRY_XRAY_SERIAL_PORT_NUMBER 3
//#define cCURRENT_TRY_AC_POWER_MONITOR_SERIAL_PORT_NUMBER 4
//#define cCURRENT_COMM_MICRO_CONTROLLER_SERIAL_PORT_NUMBER 5
//#define cCURRENT_COMM_XRAY_SERIAL_PORT_NUMBER 6
//#define cCURRENT_COMM_AC_POWER_MONITOR_SERIAL_PORT_NUMBER 7

bool TryNextSerialPort(void)		// modify vGlobaluCSerialPort->vComPortNumber to next serial port
{
	int LookForNextPort;
	int SaveCurrentPortNumber;
	bool FoundNextSequentialPort;

	//do below elsewhere check if found ports need to be set
//	if (((vGlobalDXMXRayPort) && (vGlobalDXMXRayPort->vFoundXrayPort == true)) ||  ((vGlobalMonoBlockXRayPort) && (vGlobalMonoBlockXRayPort->vFoundXrayPort == true)))
//	{
//		int index = 1;
//		while ((vGlobalPortHereOK[index] != cCURRENT_TRY_XRAY_SERIAL_PORT_NUMBER) && (index < PORTS_EVAL))
//			index++;
//		vGlobalPortHereOK[index] = cCURRENT_COMM_XRAY_SERIAL_PORT_NUMBER;
//	}
	//do below elsewhere
//	if ((vGlobalACPowerMoniter))
//	{
//		int index = 1;
//		while ((vGlobalPortHereOK[index] != cCURRENT_TRY_AC_POWER_MONITOR_SERIAL_PORT_NUMBER) && (index < PORTS_EVAL)) 
//			index++;
//		if (vGlobalPortHereOK[index] == cCURRENT_TRY_AC_POWER_MONITOR_SERIAL_PORT_NUMBER)
//			vGlobalPortHereOK[index] = cCURRENT_COMM_AC_POWER_MONITOR_SERIAL_PORT_NUMBER;
//	}

	// release current port location
	vGlobalPortHereOK[vGlobalSerialPortNumberMicroController] = cFOUND_PORT_AVALIBLE;

	FoundNextSequentialPort = false;
	LookForNextPort = vGlobalSerialPortNumberMicroController + 1;
	SaveCurrentPortNumber = vGlobalSerialPortNumberMicroController;
	
	while ((LookForNextPort < PORTS_EVAL) && (FoundNextSequentialPort == false))
	{
		if ((vGlobalPortHereOK[LookForNextPort] >= cFOUND_PORT_AVALIBLE))
		{
			if (vGlobalPortHereOK[LookForNextPort] == cFOUND_PORT_AVALIBLE)
			{
				FoundNextSequentialPort = true;
				vGlobalSerialPortNumberMicroController = LookForNextPort;
//				vGlobaluCSerialPort->vComPortNumber = LookForNextPort;
				vGlobalPortHereOK[LookForNextPort] = cCURRENT_TRY_MICRO_CONTROLLER_SERIAL_PORT_NUMBER;
			}
			else if (vGlobalPortHereOK[LookForNextPort] == cCURRENT_TRY_XRAY_SERIAL_PORT_NUMBER)
			{	
				if (vGlobalDXMXRayPort)		//|| (vGlobalACPowerMoniter))
				{
					vGlobalDXMXRayPort->ClosePort();
				}
					// give current port to Micro Controller to test if it is DXM
	//				vGlobaluCSerialPort->vComPortNumber = LookForNextPort;
					if (GetNextXRayDXMPort((LookForNextPort + 1)) == true)
					{
						vGlobalDXMXRayPort->vComPortNumber = vGlobalSerialPortNumberXRaySource;
						vGlobalDXMXRayPort->OpenPort(vGlobalDXMXRayPort->vComPortNumber);
					}
					vGlobalPortHereOK[LookForNextPort] = cCURRENT_TRY_MICRO_CONTROLLER_SERIAL_PORT_NUMBER;
					FoundNextSequentialPort = true;
					vGlobalSerialPortNumberMicroController = LookForNextPort;

// currently not implemented
/*				else if (vGlobalMonoBlockXRayPort)
				{
					vGlobalMonoBlockXRayPort->ClosePort();
					if (GetNextXRayMonoPort((LookForNextPort + 1)) == true)
					{
						vGlobalMonoBlockXRayPort->vComPortNumber = vGlobalSerialPortNumberXRaySource;
						vGlobalMonoBlockXRayPort->OpenPort(vGlobalMonoBlockXRayPort->vComPortNumber);
					}
				}
*/
			}			
			else if (vGlobalPortHereOK[LookForNextPort] == cCURRENT_TRY_AC_POWER_MONITOR_SERIAL_PORT_NUMBER)
			{
				// give current port to Micro Controller to test
				FoundNextSequentialPort = true;
				vGlobalSerialPortNumberMicroController = LookForNextPort;
//				vGlobaluCSerialPort->vComPortNumber = LookForNextPort;
				vGlobalPortHereOK[LookForNextPort] = cCURRENT_TRY_MICRO_CONTROLLER_SERIAL_PORT_NUMBER;

				if (vGlobalACPowerMoniter)
				{
					vGlobalACPowerMoniter->ClosePort();
					if (GetNextACPowerMonitorPort((LookForNextPort + 1)) == true)
					{
						vGlobalACPowerMoniter->vComPortNumber = vGlobalSerialPortNumberACPowerMonitor;
						vGlobalACPowerMoniter->OpenPort(vGlobalACPowerMoniter->vComPortNumber);
					}
				}
			}
		}
		if (FoundNextSequentialPort != true)
			LookForNextPort++;
	}	
	return (FoundNextSequentialPort);
}

/*
bool GetNextXRayMonoPort(int NextPort)
{
	bool FindPortToTry;
	int SaveCurrentPortNumber;
	int LookForNextPort;		// <<-- index, start at NextPort

	FindPortToTry = false;
	SaveCurrentPortNumber = vGlobalSerialPortNumberXRaySource;
	LookForNextPort = NextPort;

	while ((FindPortToTry == false) && (LookForNextPort < PORTS_EVAL))
	{
		if ((vGlobalPortHereOK[LookForNextPort] == cFOUND_PORT_AVALIBLE))
			FindPortToTry = true;
		else
			LookForNextPort++;
	}
	if ((FindPortToTry == false) && (LookForNextPort >= PORTS_EVAL))
	{	// restart port search
		LookForNextPort = 1;
	}
	while ((FindPortToTry == false) && (LookForNextPort < PORTS_EVAL))
	{
		if ((vGlobalPortHereOK[LookForNextPort] == cFOUND_PORT_AVALIBLE))
			FindPortToTry = true;
		else
			LookForNextPort++;
	}
	if (FindPortToTry == true)
	{
		vGlobalSerialPortNumberXRaySource = LookForNextPort;
		vGlobalPortHereOK[LookForNextPort] = cCURRENT_TRY_XRAY_SERIAL_PORT_NUMBER;
		vGlobalMonoBlockXRayPort->vComPortNumber = LookForNextPort;
		vGlobalPortHereOK[SaveCurrentPortNumber] = cFOUND_PORT_AVALIBLE;
	}

	return(FindPortToTry);
}
*/
bool GetNextXRayDXMPort(int NextPort)
{
	bool FindPortToTry;
	int SaveOldPortNumber;
	int LookForNextPort;		// <<-- index, start at NextPort

	FindPortToTry = false;
	SaveOldPortNumber = vGlobalSerialPortNumberXRaySource;
	LookForNextPort = NextPort;

	while ((FindPortToTry == false) && (LookForNextPort < PORTS_EVAL))
	{
		if ((vGlobalPortHereOK[LookForNextPort] == cFOUND_PORT_AVALIBLE))
			FindPortToTry = true;
		else
			LookForNextPort++;
	}
	if ((FindPortToTry == false) && (LookForNextPort >= PORTS_EVAL))
	{	// restart port search
		LookForNextPort = 1;
	}
	while ((FindPortToTry == false) && (LookForNextPort < PORTS_EVAL))
	{
		if ((vGlobalPortHereOK[LookForNextPort] == cFOUND_PORT_AVALIBLE))
			FindPortToTry = true;
		else
			LookForNextPort++;
	}
	if (FindPortToTry == true)
	{
		if (vGlobalPortHereOK[SaveOldPortNumber] == cCURRENT_TRY_XRAY_SERIAL_PORT_NUMBER)	// if a port was identified at the old port location, clear and remember
			vGlobalPortHereOK[SaveOldPortNumber] = cFOUND_PORT_AVALIBLE;	// clear old port designation
		vGlobalSerialPortNumberXRaySource = LookForNextPort;
		vGlobalPortHereOK[LookForNextPort] = cCURRENT_TRY_XRAY_SERIAL_PORT_NUMBER;
	}

	return(FindPortToTry);
}

// send value for next sequential port number.  If no port found, false is returned
bool GetNextACPowerMonitorPort(int NextPort)
{
	bool FindPortToTry;
	int SaveOldPortNumber;
	int LookForNextPort;		// <<-- index, start at NextPort

	FindPortToTry = false;
	SaveOldPortNumber = vGlobalSerialPortNumberACPowerMonitor;
	LookForNextPort = NextPort;

	while ((FindPortToTry == false) && (LookForNextPort < PORTS_EVAL))
	{
		if ((vGlobalPortHereOK[LookForNextPort] == cFOUND_PORT_AVALIBLE))
			FindPortToTry = true;
		else if (vGlobalPortHereOK[LookForNextPort] == cCURRENT_TRY_XRAY_SERIAL_PORT_NUMBER)
		{
			if (vGlobalDXMXRayPort)		//|| (vGlobalACPowerMoniter))
			{
				vGlobalDXMXRayPort->ClosePort();
			}
			if (GetNextXRayDXMPort((LookForNextPort + 1)) == true)
			{
				vGlobalDXMXRayPort->vComPortNumber = vGlobalSerialPortNumberXRaySource;
				vGlobalDXMXRayPort->OpenPort(vGlobalDXMXRayPort->vComPortNumber);
			}
			FindPortToTry = true;	

// currently not implemented
/*			else if (vGlobalMonoBlockXRayPort)
			{
				vGlobalMonoBlockXRayPort->ClosePort();
				if (GetNextXRayMonoPort((LookForNextPort + 1)) == true)
				{
					vGlobalMonoBlockXRayPort->vComPortNumber = vGlobalSerialPortNumberXRaySource;
					vGlobalMonoBlockXRayPort->OpenPort(vGlobalMonoBlockXRayPort->vComPortNumber);
				}
			}
*/
		}
		else
			LookForNextPort++;
	}
	if ((FindPortToTry == false) && (LookForNextPort >= PORTS_EVAL))
	{	// restart port search
		LookForNextPort = 1;
	}
	while ((FindPortToTry == false) && (LookForNextPort < PORTS_EVAL))
	{
		if ((vGlobalPortHereOK[LookForNextPort] == cFOUND_PORT_AVALIBLE))
			FindPortToTry = true;
		else 
		if (vGlobalPortHereOK[LookForNextPort] == cCURRENT_TRY_XRAY_SERIAL_PORT_NUMBER)
		{
			if (vGlobalDXMXRayPort)		//|| (vGlobalACPowerMoniter))
			{
				vGlobalDXMXRayPort->ClosePort();
			}

			if (GetNextXRayDXMPort((LookForNextPort + 1)) == true)
			{
				vGlobalDXMXRayPort->vComPortNumber = vGlobalSerialPortNumberXRaySource;
				vGlobalDXMXRayPort->OpenPort(vGlobalDXMXRayPort->vComPortNumber);
			}
			FindPortToTry = true;	

// currently not implemented
/*			else if (vGlobalMonoBlockXRayPort)
			{
				vGlobalMonoBlockXRayPort->ClosePort();
				if (GetNextXRayMonoPort((LookForNextPort + 1)) == true)
				{
					vGlobalMonoBlockXRayPort->vComPortNumber = vGlobalSerialPortNumberXRaySource;
					vGlobalMonoBlockXRayPort->OpenPort(vGlobalMonoBlockXRayPort->vComPortNumber);
				}
			}
*/
		}
		else
			LookForNextPort++;
	}
	if (FindPortToTry == true)
	{
		if (vGlobalPortHereOK[SaveOldPortNumber] == cCURRENT_TRY_AC_POWER_MONITOR_SERIAL_PORT_NUMBER)	// if a port was identified at the old port location, clear and remember
			vGlobalPortHereOK[SaveOldPortNumber] = cFOUND_PORT_AVALIBLE;	// clear old port designation
		vGlobalSerialPortNumberACPowerMonitor = LookForNextPort;
		vGlobalPortHereOK[LookForNextPort] = cCURRENT_TRY_AC_POWER_MONITOR_SERIAL_PORT_NUMBER;
	}

	return(FindPortToTry);
}

// open ports with bits set.
int ReviewPortsForDefault(void)
{
	int ReturnValue = 0;
	bool bTryMicroTwiceForDefaultPort = false;
	bool bTryACPowerTwiceForDefaultPort = false;

	if ((vGlobaluCSerialPort)   && (vGlobalFirstMicroControllerCommCheck == cFinishedSearchForNextSerialPort))
	{
		if (vGlobalPortHereOK[vGlobalSerialPortNumberMicroController] != cCURRENT_COMM_MICRO_CONTROLLER_SERIAL_PORT_NUMBER)
		{
			if (vGlobalSerialPortNumberMicroController != 0)
			{
				vGlobaluCSerialPort->ClosePort();
				vGlobalPortHereOK[vGlobalSerialPortNumberMicroController] = cFOUND_PORT_AVALIBLE;	//free port
				vGlobalSerialPortNumberMicroController = 0;		// out of range specification
			}
			if (vGlobalPortHereOK[cDEFAULT_MICRO_CONTROLLER_SERIAL_PORT_NUMBER] == cFOUND_PORT_AVALIBLE)
			{	// set uC port to default and open				
				vGlobalPortHereOK[cDEFAULT_MICRO_CONTROLLER_SERIAL_PORT_NUMBER] = cCURRENT_COMM_MICRO_CONTROLLER_SERIAL_PORT_NUMBER;
				vGlobalSerialPortNumberMicroController = cDEFAULT_MICRO_CONTROLLER_SERIAL_PORT_NUMBER;
				ReturnValue |= 1;
			}
			else
				bTryMicroTwiceForDefaultPort = true;
		}
	}
	if ((vGlobalACPowerMoniter) && (vGlobalACPowerMoniter->vTestACPowerMoniterExists == cNoCommPortFound))
	{
		if (vGlobalSerialPortNumberACPowerMonitor != 0)
		{
			vGlobalACPowerMoniter->ClosePort();
			vGlobalPortHereOK[vGlobalSerialPortNumberACPowerMonitor] = cFOUND_PORT_AVALIBLE;
			vGlobalSerialPortNumberACPowerMonitor = 0;		// out of range specification
		}
		if (vGlobalPortHereOK[cDEFAULT_AC_POWER_MONITOR_SERIAL_PORT_NUMBER] == cFOUND_PORT_AVALIBLE)
		{
			vGlobalPortHereOK[cDEFAULT_AC_POWER_MONITOR_SERIAL_PORT_NUMBER] = cCURRENT_COMM_AC_POWER_MONITOR_SERIAL_PORT_NUMBER;
			vGlobalSerialPortNumberACPowerMonitor = cDEFAULT_AC_POWER_MONITOR_SERIAL_PORT_NUMBER;
			vGlobalACPowerMoniter->vTestACPowerMoniterExists = cYesCommSeen;
			ReturnValue |= 2;
		}
		else
		{
			bTryACPowerTwiceForDefaultPort = true;
		}
	}
		
	if ((vGlobalDXMXRayPort) && (vGlobalXrayNoCommMessageRcvdEver == cNoCommPortFound))
	{
		if (vGlobalSerialPortNumberXRaySource != 0)
		{
			vGlobalDXMXRayPort->ClosePort();
			vGlobalPortHereOK[vGlobalSerialPortNumberXRaySource] = cFOUND_PORT_AVALIBLE;
			vGlobalSerialPortNumberXRaySource = 0;	// out of range specification
		}
		if (vGlobalPortHereOK[cDEFAULT_XRAY_SERIAL_PORT_NUMBER] == cFOUND_PORT_AVALIBLE)		
		{
			vGlobalPortHereOK[cDEFAULT_XRAY_SERIAL_PORT_NUMBER] = cCURRENT_COMM_XRAY_SERIAL_PORT_NUMBER;
			vGlobalSerialPortNumberXRaySource = cDEFAULT_XRAY_SERIAL_PORT_NUMBER;
			vGlobalXrayNoCommMessageRcvdEver = cYesCommSeen;
			ReturnValue |= 4;
		}
	}

	if ((bTryMicroTwiceForDefaultPort == true) && (vGlobalPortHereOK[cDEFAULT_MICRO_CONTROLLER_SERIAL_PORT_NUMBER] == cFOUND_PORT_AVALIBLE))
	{
		vGlobalPortHereOK[cDEFAULT_MICRO_CONTROLLER_SERIAL_PORT_NUMBER] = cCURRENT_COMM_MICRO_CONTROLLER_SERIAL_PORT_NUMBER;
		vGlobalSerialPortNumberMicroController = cDEFAULT_MICRO_CONTROLLER_SERIAL_PORT_NUMBER;
		ReturnValue |= 1;
	}

	if ((bTryACPowerTwiceForDefaultPort == true) && (vGlobalPortHereOK[cDEFAULT_AC_POWER_MONITOR_SERIAL_PORT_NUMBER] == cFOUND_PORT_AVALIBLE))
	{
		vGlobalPortHereOK[cDEFAULT_AC_POWER_MONITOR_SERIAL_PORT_NUMBER] = cCURRENT_COMM_AC_POWER_MONITOR_SERIAL_PORT_NUMBER;
		vGlobalSerialPortNumberACPowerMonitor = cDEFAULT_AC_POWER_MONITOR_SERIAL_PORT_NUMBER;
		vGlobalACPowerMoniter->vTestACPowerMoniterExists = cYesCommSeen;
		ReturnValue |= 2;
	}

	return(ReturnValue);
}

// enum {cSetToDefault, cDefaultNotCurrentlyAvailable, cDefaultNeverAvailable};
int SetSerialPortToDefault(void)
{
	int iReturnValue;

	iReturnValue = cDefaultNeverAvailable;

	if ((vGlobalPortHereOK[cDEFAULT_MICRO_CONTROLLER_SERIAL_PORT_NUMBER] == cFOUND_PORT_AVALIBLE) ||
		(vGlobalPortHereOK[cDEFAULT_MICRO_CONTROLLER_SERIAL_PORT_NUMBER] == cCURRENT_TRY_MICRO_CONTROLLER_SERIAL_PORT_NUMBER) ||
		(vGlobalPortHereOK[cDEFAULT_MICRO_CONTROLLER_SERIAL_PORT_NUMBER] == cCURRENT_COMM_MICRO_CONTROLLER_SERIAL_PORT_NUMBER))
		iReturnValue = cSetToDefault;
	else if (vGlobalPortHereOK[cDEFAULT_MICRO_CONTROLLER_SERIAL_PORT_NUMBER] == cCURRENT_TRY_XRAY_SERIAL_PORT_NUMBER)
		iReturnValue = cDefaultNotCurrentlyAvailable;		
	else if (vGlobalPortHereOK[cDEFAULT_MICRO_CONTROLLER_SERIAL_PORT_NUMBER] == cCURRENT_TRY_AC_POWER_MONITOR_SERIAL_PORT_NUMBER)
	{
		if (vGlobalACPowerMoniter)
		{
			vGlobalACPowerMoniter->ClosePort();
		}
		if (GetNextACPowerMonitorPort((cDEFAULT_MICRO_CONTROLLER_SERIAL_PORT_NUMBER + 1)) == true)
		{
			vGlobalACPowerMoniter->vComPortNumber = vGlobalSerialPortNumberACPowerMonitor;
			vGlobalACPowerMoniter->OpenPort(vGlobalACPowerMoniter->vComPortNumber);
		}
		iReturnValue = cSetToDefault;
	}
	else //if (vGlobalPortHereOK[cDEFAULT_MICRO_CONTROLLER_SERIAL_PORT_NUMBER] == cFOUND_PORT_NOT_AVALIBLE)
		 //if (vGlobalPortHereOK[cDEFAULT_MICRO_CONTROLLER_SERIAL_PORT_NUMBER] == cCURRENT_COMM_AC_POWER_MONITOR_SERIAL_PORT_NUMBER)
		 //if (vGlobalPortHereOK[cDEFAULT_MICRO_CONTROLLER_SERIAL_PORT_NUMBER] == cCURRENT_COMM_XRAY_SERIAL_PORT_NUMBER)
		iReturnValue = cDefaultNeverAvailable;

	if (vGlobalSerialPortNumberMicroController != cDEFAULT_MICRO_CONTROLLER_SERIAL_PORT_NUMBER)
	{
		if ((vGlobalPortHereOK[vGlobalSerialPortNumberMicroController] == cCURRENT_TRY_MICRO_CONTROLLER_SERIAL_PORT_NUMBER) ||
		    (vGlobalPortHereOK[vGlobalSerialPortNumberMicroController] == cCURRENT_COMM_MICRO_CONTROLLER_SERIAL_PORT_NUMBER))
			 vGlobalPortHereOK[vGlobalSerialPortNumberMicroController] = cFOUND_PORT_AVALIBLE;
		vGlobalSerialPortNumberMicroController = 0;		// out of range specification
	}


	if (iReturnValue == cSetToDefault)
	{	// set uC port to default and open
		vGlobalPortHereOK[cDEFAULT_MICRO_CONTROLLER_SERIAL_PORT_NUMBER] = cCURRENT_COMM_MICRO_CONTROLLER_SERIAL_PORT_NUMBER;
		vGlobalSerialPortNumberMicroController = cDEFAULT_MICRO_CONTROLLER_SERIAL_PORT_NUMBER;
		vGlobaluCSerialPort->vComPortNumber = vGlobalSerialPortNumberMicroController;
	}

	return(iReturnValue);
}


// enum {cSetToDefault, cDefaultNotCurrentlyAvailable, cDefaultNeverAvailable};
int SetACMonitorPortToDefault(void)
{
	int iReturnValue;

	iReturnValue = cDefaultNeverAvailable;

	if ((vGlobalPortHereOK[cDEFAULT_AC_POWER_MONITOR_SERIAL_PORT_NUMBER] == cFOUND_PORT_AVALIBLE) ||
		(vGlobalPortHereOK[cDEFAULT_AC_POWER_MONITOR_SERIAL_PORT_NUMBER] == cCURRENT_TRY_AC_POWER_MONITOR_SERIAL_PORT_NUMBER) ||
		(vGlobalPortHereOK[cDEFAULT_AC_POWER_MONITOR_SERIAL_PORT_NUMBER] == cCURRENT_COMM_AC_POWER_MONITOR_SERIAL_PORT_NUMBER))
		iReturnValue = cSetToDefault;
	else if ((vGlobalPortHereOK[cDEFAULT_AC_POWER_MONITOR_SERIAL_PORT_NUMBER] == cCURRENT_TRY_XRAY_SERIAL_PORT_NUMBER) ||
		     (vGlobalPortHereOK[cDEFAULT_AC_POWER_MONITOR_SERIAL_PORT_NUMBER] == cCURRENT_TRY_MICRO_CONTROLLER_SERIAL_PORT_NUMBER))
		iReturnValue = cDefaultNotCurrentlyAvailable;		
	else //if ((vGlobalPortHereOK[cDEFAULT_AC_POWER_MONITOR_SERIAL_PORT_NUMBER] == cCURRENT_COMM_MICRO_CONTROLLER_SERIAL_PORT_NUMBER) ||
		 //    (vGlobalPortHereOK[cDEFAULT_AC_POWER_MONITOR_SERIAL_PORT_NUMBER] == cCURRENT_COMM_XRAY_SERIAL_PORT_NUMBER) ||
		 //	   (vGlobalPortHereOK[cDEFAULT_AC_POWER_MONITOR_SERIAL_PORT_NUMBER] == cFOUND_PORT_NOT_AVALIBLE))
		iReturnValue = cDefaultNeverAvailable;

	if (vGlobalSerialPortNumberACPowerMonitor != cDEFAULT_AC_POWER_MONITOR_SERIAL_PORT_NUMBER)
	{
		if ((vGlobalPortHereOK[vGlobalSerialPortNumberACPowerMonitor] == cCURRENT_TRY_AC_POWER_MONITOR_SERIAL_PORT_NUMBER) ||
		    (vGlobalPortHereOK[vGlobalSerialPortNumberACPowerMonitor] == cCURRENT_COMM_AC_POWER_MONITOR_SERIAL_PORT_NUMBER))
			vGlobalPortHereOK[vGlobalSerialPortNumberACPowerMonitor] = cFOUND_PORT_AVALIBLE;
		vGlobalSerialPortNumberACPowerMonitor = 0;		// out of range specification
	}


	if (iReturnValue == cSetToDefault)
	{	// set uC port to default and open
		//vGlobalACPowerMoniter-> ->ClosePort();
		vGlobalPortHereOK[cDEFAULT_AC_POWER_MONITOR_SERIAL_PORT_NUMBER] = cCURRENT_COMM_AC_POWER_MONITOR_SERIAL_PORT_NUMBER;
		vGlobalSerialPortNumberACPowerMonitor = cDEFAULT_AC_POWER_MONITOR_SERIAL_PORT_NUMBER;
		vGlobalACPowerMoniter->vComPortNumber = vGlobalSerialPortNumberACPowerMonitor;
//		vGlobalACPowerMoniter->OpenPort(vGlobalSerialPortNumberACPowerMonitor);
	}

	return(iReturnValue);
}

// enum {cSetToDefault, cDefaultNotCurrentlyAvailable, cDefaultNeverAvailable};
int SetXRayPortToDefault(void)
{
	int iReturnValue;

	iReturnValue = cDefaultNeverAvailable;

	if ((vGlobalPortHereOK[cDEFAULT_XRAY_SERIAL_PORT_NUMBER] == cFOUND_PORT_AVALIBLE) ||
		(vGlobalPortHereOK[cDEFAULT_XRAY_SERIAL_PORT_NUMBER] == cCURRENT_TRY_XRAY_SERIAL_PORT_NUMBER) ||
		(vGlobalPortHereOK[cDEFAULT_XRAY_SERIAL_PORT_NUMBER] == cCURRENT_COMM_XRAY_SERIAL_PORT_NUMBER))
		iReturnValue = cSetToDefault;
	else if ((vGlobalPortHereOK[cDEFAULT_XRAY_SERIAL_PORT_NUMBER] == cCURRENT_TRY_MICRO_CONTROLLER_SERIAL_PORT_NUMBER) ||
		     (vGlobalPortHereOK[cDEFAULT_XRAY_SERIAL_PORT_NUMBER] == cCURRENT_TRY_AC_POWER_MONITOR_SERIAL_PORT_NUMBER))
		iReturnValue = cDefaultNotCurrentlyAvailable;		
	else //if ((vGlobalPortHereOK[cDEFAULT_XRAY_SERIAL_PORT_NUMBER] == cCURRENT_COMM_MICRO_CONTROLLER_SERIAL_PORT_NUMBER) ||
		 //    (vGlobalPortHereOK[cDEFAULT_XRAY_SERIAL_PORT_NUMBER] == cCURRENT_COMM_AC_POWER_MONITOR_SERIAL_PORT_NUMBER) ||
		 //	   (vGlobalPortHereOK[cDEFAULT_XRAY_SERIAL_PORT_NUMBER] == cFOUND_PORT_NOT_AVALIBLE))
		iReturnValue = cDefaultNeverAvailable;

	if (vGlobalSerialPortNumberXRaySource != cDEFAULT_XRAY_SERIAL_PORT_NUMBER)
	{
		if ((vGlobalPortHereOK[vGlobalSerialPortNumberXRaySource] == cCURRENT_TRY_XRAY_SERIAL_PORT_NUMBER) ||
		    (vGlobalPortHereOK[vGlobalSerialPortNumberXRaySource] == cCURRENT_COMM_XRAY_SERIAL_PORT_NUMBER))
			 vGlobalPortHereOK[vGlobalSerialPortNumberXRaySource] = cFOUND_PORT_AVALIBLE;
		vGlobalSerialPortNumberXRaySource = 0;		// out of range specification
	}


	if (iReturnValue == cSetToDefault)
	{	// set uC port to default and open
		//vGlobalDXMXRayPort-> ->ClosePort();
		vGlobalPortHereOK[cDEFAULT_XRAY_SERIAL_PORT_NUMBER] = cCURRENT_COMM_XRAY_SERIAL_PORT_NUMBER;
		vGlobalSerialPortNumberXRaySource = cDEFAULT_XRAY_SERIAL_PORT_NUMBER;
		vGlobalDXMXRayPort->vComPortNumber = vGlobalSerialPortNumberXRaySource;
//		vGlobalDXMXRayPort->OpenPort(vGlobalSerialPortNumberXRaySource);
	}

	return(iReturnValue);
}

/*
bool SetACPortHere(void)	// port value in vGlobalSerialPortNumberACPowerMonitor
{
	bool PortSetSucceed;
	PortSetSucceed = false;

	if (vGlobalPortHereOK[vGlobalSerialPortNumberACPowerMonitor] == cCURRENT_TRY_AC_POWER_MONITOR_SERIAL_PORT_NUMBER)
	{
		vGlobalPortHereOK[vGlobalSerialPortNumberACPowerMonitor] = cCURRENT_COMM_AC_POWER_MONITOR_SERIAL_PORT_NUMBER;
		PortSetSucceed = true;
	}
	return(PortSetSucceed);
}

bool SetXRayPortHere(void)	// port value in vGlobalSerialPortNumberXRaySource
{
	bool PortSetSucceed;
	PortSetSucceed = false;

	if (vGlobalPortHereOK[vGlobalSerialPortNumberXRaySource] == cCURRENT_TRY_XRAY_SERIAL_PORT_NUMBER)
	{
		vGlobalPortHereOK[vGlobalSerialPortNumberXRaySource] = cCURRENT_COMM_XRAY_SERIAL_PORT_NUMBER;
	}
	return(PortSetSucceed);
}
*/
//int vGlobalSerialPortNumberMicroController;	//default = 1
//int vGlobalSerialPortNumberACPowerMonitor;	//default = 4
//int vGlobalSerialPortNumberXRaySource	;		//default = 3
