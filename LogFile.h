//ScanTrac Side View Source File
// LogFile.h: interface for the CLogFile class.
//
//////////////////////////////////////////////////////////////////////
#include <afxmt.h>  //for syncronization objects like critical sections

#if !defined(AFX_LOGFILE_H__0389C4F1_0BFE_11D5_AD70_00500466E305__INCLUDED_)
#define AFX_LOGFILE_H__0389C4F1_0BFE_11D5_AD70_00500466E305__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

const BYTE cInputMessage = 10;
const BYTE cOutputMessage = 11;
const BYTE cDebugMessage = 12;
const WORD cDataReceivedMessage = 25237;
const BYTE cDataSentMessage = 236;
const LPCTSTR cLogDirectory = _T("C:\\ScanTracLogs\\");

class CStringBufferObject : public CObject  
{
public:
	//variables
	BYTE vTheType;
	//WORD vTheStringLength;
	CString vTheString;

	//constructor
	CStringBufferObject();

	//Destructor
	virtual ~CStringBufferObject();

	//methods
};

typedef CTypedPtrList<CObList, CStringBufferObject*> tOutputStringCollection;

class CLogFile : public CObject  
{
public:
	//variables
	CString vLogDirectory;
	CCriticalSection vStringCollectionLock;
	tOutputStringCollection vOutputStringCollection;
	BYTE vLogSyncData;
	CString vLastDataReceived;
	CString vLastDataSent;
	DWORD vReceiveCount;
	DWORD vSendCount;
	BYTE vLogSerialData;
	bool vShowingSerialDataOnScreen;
	CFile *vSerialDataLogFile;

	//constructor
	CLogFile();

	//Destructor
	virtual ~CLogFile();

	//methods
	BYTE OpenLogFile(); 
	void CloseLogFile(); 
	void WriteToLogFile(CString TempStringToWrite, BYTE TempuCMessage);
	void OutputStringsToFile();
	void LogFileObjectBackground();
};

#endif // !defined(AFX_LOGFILE_H__0389C4F1_0BFE_11D5_AD70_00500466E305__INCLUDED_)
