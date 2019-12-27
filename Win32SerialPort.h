// Win32SerialPort.h: interface for the CWin32SerialPort class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WIN32SERIALPORT_H__4EFB89B3_2DD7_11D4_AC4E_00500466E305__INCLUDED_)
#define AFX_WIN32SERIALPORT_H__4EFB89B3_2DD7_11D4_AC4E_00500466E305__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Win32SerialPortThread.h"

const BYTE cuControllerMessageLength = 5;
const BYTE cETX = 5;

extern BYTE *vGlobalReceiveDataBufferPointer;
extern WORD *vGlobalReceiveTailPointer;
extern WORD *vGlobalReceiveHeadPointer;
extern BYTE vGlobalReadDataBuffer[cReceiveBufferSize];
extern DWORD vGlobalCallingThreadID;
extern bool *vGlobalReadError;
//extern int vSendingACommand;
//extern int vWaitingForRead;

typedef BYTE tSerialCommand[10];

class CWin32SerialPort  : public CObject   
{
	DECLARE_SERIAL( CWin32SerialPort )
public:
	//need for config file compatibility upto version 45
	//variables
	BYTE vLogSyncData;
	BYTE vLogSerialData;

	// Construction
	CWin32SerialPort();
	//Destructor
	virtual ~CWin32SerialPort();

	//methods
	virtual void Serialize( CArchive& TempArchive );
	//int WriteConfigurationDataToINIFile();
	//int ReadConfigurationDataFromINIFile();

	//	DECLARE_DYNCREATE(CWin32SerialPort)
};

#endif // !defined(AFX_WIN32SERIALPORT_H__4EFB89B3_2DD7_11D4_AC4E_00500466E305__INCLUDED_)
