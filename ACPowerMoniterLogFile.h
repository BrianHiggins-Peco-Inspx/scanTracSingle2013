//ScanTrac Side View Source File
// ACPowerMoniterLogFile.h: interface for the CACPowerMoniterLogFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACPOWERMONITERLOGFILE_H__4EC118D3_B2AC_4BEC_B1F5_43F970872F14__INCLUDED_)
#define AFX_ACPOWERMONITERLOGFILE_H__4EC118D3_B2AC_4BEC_B1F5_43F970872F14__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

const BYTE cPowerOnMessage = 13;
const BYTE cErrorPowerOnMessage = 14;
const BYTE cResolveErrorMessages = 15;
const BYTE cTimePerLineFeedIncrimentInLog = 15; 
const BYTE cERRORLOG = 2;
const BYTE cSTANDARDLOG = 1;
const WORD cWidth = 570;  //the width of a the data plot in number of pixels
const BYTE cZoomIn = 21;
const BYTE cZoomOut = 22;
const BYTE cFullZoomIn = 23;
const BYTE cFullZoomOut = 24;
const BYTE cShowZeros = 25;
const BYTE cHideZeros = 26;
const BYTE cPanRight = 27;
const BYTE cPanLeft = 28;

class CACPowerMoniterLogFile  
{
public:
	CString vLogDirectory;
	CString vTodaysDate;
	CString vCurrentFileDateString;
	//CString vLastDataReceived;
	CString vLastDataSent;
	CString vLogFileName;
	CString vErrorStrings[51];
	CString vCurrentDayFileTimeRange;
	CString vPreviousDataLogMessageTime;
	CString vTimeOffMessage;
	tOutputStringCollection vACPowerMoniterOutputStringCollection;
	CCriticalSection vACPowerMoniterStringCollectionLock;
//	bool vShowingSerialDataOnScreen;
	bool vFoundFiles;
	bool vShowAllErrorMessages;
	bool vDoResolveErrors;
	bool vShowZeros;
	bool vMaxZoomedOut;
	BYTE vLogSyncData;
	BYTE vLogACPowerMoniterFormat;
	BYTE vNumOfErrors;
	DWORD vPreviousTime;
	DWORD vReceiveCount;
	DWORD vSendCount;
	int vDateIndex;
	ULONGLONG vCurrentFileSize;
	int vZoomScale;
	int vMaxGroupSize;
	int vPanIndex;
	BYTE *vValues;
	DWORD *vTimes;
	DWORD *vCurrentGraphTimes;
	bool vStartUp;
	double *vDataFromLogPointer;
	unsigned int vNumberOfValues;
	unsigned int vOrigionalNumberOfValues;
	bool vGaveInspxMessageOverflowWarning;
	CFile vSerialDataLogFile;
	CFile vSerialDataLogFileToWrite;
	CTime vTime;

	//constructor
	CACPowerMoniterLogFile();

	//Destructor
	virtual ~CACPowerMoniterLogFile();

	//methods
	BYTE SetupLogFileName(); 
	void WriteToLogFile(CString TempStringToWrite, BYTE TempuCMessage);
	void OutputStringsToFile();
	void LogFileObjectBackground();
	void SetACPowerMoniterFormat(bool Set, BYTE TempLogType);
	bool DataLogChangeZoomOrZeros(BYTE TempType);
	void GetACPowerMoniterLogData(int TempDateOffset);
	CString GetACPowerMoniterDataLogData(CString TempLogFileName);
	void FreeGlobalDataPointer();
	void GetLastDateTimeStamp(CString * TempReturnStrings);
	bool UseFile();
	CString GetACPowerMoniterErrorLogData(int TempDateOffset);
	CString GetACPowerMoniterErrorDataFromLogFile(CString TempLogFile);
	void DisplayReadingInErrorLog();
	void CloseReadingInErrorLog();
};

#endif // !defined(AFX_ACPOWERMONITERLOGFILE_H__4EC118D3_B2AC_4BEC_B1F5_43F970872F14__INCLUDED_)
