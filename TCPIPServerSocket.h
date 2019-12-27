//Side View ScanTrac, ScanTrac Source File
// TCPIPServerSocket.h: interface for the CTCPIPServerSocket class.
//
//////////////////////////////////////////////////////////////////////
#include "SocketMessage1.h"
#include "AFXSock.h"

#if !defined(AFX_TCPIPSERVERSOCKET_H__443FF0A1_A834_11D8_B196_00500466E305__INCLUDED_)
#define AFX_TCPIPSERVERSOCKET_H__443FF0A1_A834_11D8_B196_00500466E305__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CScanTracDlg;

class CTCPIPServerSocket : public CSocket  
{
	DECLARE_DYNAMIC(CTCPIPServerSocket);
//private:
	//CTCPIPServerSocket(const CTCPIPServerSocket& rSrc);
	//void operator=(const CTCPIPServerSocket& rSrc);  // no implementation
public:
	//variables
	CScanTracDlg *vMainWindowPointer;
	int vNumberOfSocketMessageToSend;
	CSocketFile* vSocketFileForArchive;
	CArchive* vSocketArchiveIn;
	CArchive* vSocketArchiveOut;
	BOOL IsAborted() { return vSocketArchiveOut == NULL; }

// Construction
	CTCPIPServerSocket(CScanTracDlg* TempMainWindowPointer);
	virtual ~CTCPIPServerSocket();

//methods
	void Init();
	void Abort();
	void SendSocketMessage(CSocketMessage* TempSocketMessage);
	void ReceiveMsg(CSocketMessage* TempSocketMessage);

// Overridable callbacks
protected:
	virtual void OnReceive(int nErrorCode);

// Implementation
public:

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& TempDumpContext) const;
#endif
};

#endif // !defined(AFX_TCPIPSERVERSOCKET_H__443FF0A1_A834_11D8_B196_00500466E305__INCLUDED_)

