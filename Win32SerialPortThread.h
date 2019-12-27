//ScanTrac Side View Source File
// Win32SerialPortThread.h: interface for the CWin32SerialPortThread class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WIN32SERIALPORTTHREAD_H__4EFB89B4_2DD7_11D4_AC4E_00500466E305__INCLUDED_)
#define AFX_WIN32SERIALPORTTHREAD_H__4EFB89B4_2DD7_11D4_AC4E_00500466E305__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

const WORD cReceiveBufferSize = 600;
const WORD cSerialReceivedDataMessage = 2421;
const WORD cNewDataToLogMessage = 2422;
const WORD cSerialInputOverflowMessage = 2423;

//extern BYTE *vGlobalReceiveDataBufferPointer;
//extern WORD *vGlobalReceiveTailPointer;
//extern WORD *vGlobalReceiveHeadPointer;
//extern BYTE vGlobalReadDataBuffer[cReceiveBufferSize];
//extern DWORD vGlobalCallingThreadID;
//extern bool *vGlobalReadError;
//extern int vSendingACommand;
//extern int vWaitingForRead;

class CWin32SerialPortThread : public CObject 
{
public:
	//variables
	BYTE *vMyComPortReceiveDataBufferPointer;
	WORD *vMyComPortReceiveTailPointer;
	WORD *vMyComPortReceiveHeadPointer;
	BYTE vMyComPortReadDataBuffer[cReceiveBufferSize];
	DWORD vMyComPortCallingThreadID;
	bool *vMyComPortReadError;

	BOOLEAN vShutDown;
	HANDLE vComPortThreadHandle;
	HANDLE vComPortHandle;
	DWORD vReadCount;
	BYTE *vThreadReceiveDataBufferPointer;
	WORD *vThreadReceiveDataCountPointer;
	BOOLEAN vOverFlowError;
	OVERLAPPED *vPointerToThreadOverlappedStructure;
	BYTE TempReadDataBuffer[cReceiveBufferSize];
	DWORD vSerialPortThreadID;
	BYTE vSerialPortNumber;
	//void *vMySerialPortOwner;

	//constructor
	CWin32SerialPortThread::CWin32SerialPortThread(HANDLE TempComPortHandle, 
		WORD *TempBufferHead, WORD *TempBufferTail, BYTE *TempDataBuffer, 
		DWORD TempCallingThreadID, OVERLAPPED *TempOverlappedStructure,
		bool *TempReadError);

	//destructor
	virtual ~CWin32SerialPortThread();

	//methods
	void CloseSerialPortReadThread();
	friend DWORD WINAPI ReadComPortThread(LPVOID owner);
	/*
	VOID CALLBACK FinishReadRoutine(DWORD TempErrorCode,
		DWORD TempNumberOfBytesTransfered,
		LPOVERLAPPED TempOverlappedStructure);
	*/
	friend int NEAR ReadCommBlock( HANDLE TempCommHandle, LPSTR TempBuffer, int TempMaxLength, OVERLAPPED *TempOverlappedStructure );


};

#endif // !defined(AFX_WIN32SERIALPORTTHREAD_H__4EFB89B4_2DD7_11D4_AC4E_00500466E305__INCLUDED_)
