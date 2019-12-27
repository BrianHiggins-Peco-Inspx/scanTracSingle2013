//ScanTrac Side View Source File
// SerialPortBase.h: interface for the CSerialPortBase class.
//
//////////////////////////////////////////////////////////////////////
#include "Win32SerialPortThread.h"
#include "SystemConfigurationData.h"
//#include "ScanTracDlg.h"

#if !defined(AFX_SERIALPORTBASE_H__5C0738D2_40B0_11DA_B2BF_00500466E305__INCLUDED_)
#define AFX_SERIALPORTBASE_H__5C0738D2_40B0_11DA_B2BF_00500466E305__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

const WORD cCheckInterlocksIn200MSMessage = 65010;
const WORD cEmergencyShutXRaysDownMessage = 65100;
const WORD cDownStreamSignalChangedMessage = 65012;
const WORD cTurnOffEjectorMessage = 65013;
enum { cNOPARITY, cODDPARITY, cEVENPARITY };

class CSerialPortBase : public CWinThread  
{
	DECLARE_DYNCREATE(CSerialPortBase)
public:
	//variables
	bool vUse9600Baud;
	bool vConnectedDeviceVerified;
	int vParity;
	CString vDeviceName;
	int vFirstCommunicationTest;
	bool vHasTransmittedCommunication;
	bool vHasReceivedCommunication;
	DWORD vPreviousMilliSeconds;
	BYTE vWaitingForReply;
	bool vNoCommunications;
	BYTE vRetriesAfterNoAnswer;
	bool vOldNoCommunications;
	WORD vReceiveCount;
	CString vComPortString;
	OVERLAPPED vWriteOverlappedStructure;
	OVERLAPPED vReadOverlappedStructure;
	HANDLE vWinPortHandle;
	CWin32SerialPortThread *vComPortThreadObject;
	bool vPortOpen;
	BYTE vReceiveDataBuffer[cReceiveBufferSize];
	WORD vReceiveDataHead;
	WORD vReceiveDataTail;
	bool vReadPortDataError;
	WORD vLostMessageCounter;
	CScanTracSystemRunningData *vLocalSystemData;
	CSystemConfigurationData *vLocalConfigurationData;
	BYTE vTestByte;
	bool vSendingContinuous;
	BYTE vLastSent[4];
	WORD vSendCount;
	bool vLastCommTestStatusOK;
	BYTE vCommunicationsTimeOutCounter;
	BYTE vComPortNumber;
	int vBaudRate;
	WORD vCurrentReceiveCount;
	int vTryNextSerialPort;
	BYTE vLastCommandSent;

	// Construction
	CSerialPortBase();
	virtual ~CSerialPortBase();

	//methods
	virtual bool OpenPort(BYTE TempPort);
	virtual void ClosePort();
	//virtual bool IsMessageGood(); 
	virtual void ClearReceiver(); 
	virtual void ClearReceiveBuffer();
	virtual void SetBaudRate(int TempBaudRate);
	virtual void ClearPort();
	virtual void ProcessSerialMessage();
	virtual WORD ReceiveDataCount();
	virtual void ShowSerialData();
	virtual bool SerialMessageComplete();
	virtual void RemoveMessageFromBuffer(); 
	virtual void DecodeMessage();
	virtual void DoBackGroundWork();
	virtual void CSerialPortBase::WriteCommandToLogFile(tSerialCommand& cmd, int length);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSerialPortBase)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	//}}AFX_VIRTUAL
	// Generated message map functions
	//{{AFX_MSG(CSerialPortBase)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERIALPORTBASE_H__5C0738D2_40B0_11DA_B2BF_00500466E305__INCLUDED_)
