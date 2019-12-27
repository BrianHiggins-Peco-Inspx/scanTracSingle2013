

#ifdef CompileInHTTPClientOperationsGuardian
#pragma once

#include <string>

// CSendNextOGMessageThread
const WORD cOGQueueLength = 1000;

class CSendNextOGMessageThread : public CWinThread
{
	DECLARE_DYNCREATE(CSendNextOGMessageThread)

public:
	std::wstring vOGMessageToSend;
	std::wstring vOGMessageKeyWord;
	bool vShutDown;


protected:
	CSendNextOGMessageThread();           // protected constructor used by dynamic creation
	virtual ~CSendNextOGMessageThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//methods
	virtual int Run();
	void WriteToDebugLog(std::wstring TempURL, std::wstring TempMessage, int TempCode, std::wstring TempResponse);
	void SendNextMessage();
	CString dtoa(double TempData, BYTE TempClipLength);

protected:
	DECLARE_MESSAGE_MAP()
};

#endif
