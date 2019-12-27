//ScanTrac Side View Source File
// ACPowerMoniter.h: interface for the CACPowerMoniter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACPOWERMONITER_H__F8320643_454D_434E_B9BD_637A218C1CED__INCLUDED_)
#define AFX_ACPOWERMONITER_H__F8320643_454D_434E_B9BD_637A218C1CED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SerialPortBase.h"
#include "ACPowerMoniterLogFile.h"

//some constants declared in SpellmanDXMXRaySourcePort.h
const BYTE cMinimumVoltage = 105;
const BYTE cToday = 0;
const int cPreviousDay = -1;
const BYTE cNextDay = 1;
const BYTE cNumberOfACVoltageSamples = 30;

class CACPowerMoniter : public CSerialPortBase  
{
	DECLARE_DYNCREATE(CACPowerMoniter)
public:
	//variables
	//10/26/2009
	CString vLastError;
	DWORD vACErrorCounter[17];
	CString vOldHour;
	int vCurrentACInputVoltage;
	int vCurrentACInputVoltageMinimum;
	int vCurrentACInputVoltageMaximum;

	int vACInputVoltageData[cNumberOfACVoltageSamples];
	BYTE vDataIndex;
	WORD vGaveBelow108VoltsError;
	WORD vGaveBrownOutError;

	CACPowerMoniterLogFile IACPowerMoniterLogFile;
	CACPowerMoniterLogFile IErrorACPowerMoniterLogFile;
	CString vComPortString;
	CString vLatestResponse;
	int vLength;
	int vFilePosition;
	int vACVoltMonitor;
	BYTE vComPortNumber;
	BYTE vResponse[250];
	BYTE vErrorsOccuredWait;
	//int vPowerFailure;
	//int vPowerError;
	bool vLogACPowerMoniterData;
	BYTE vGaveACVoltageWarning;	//0, not given, 1 about to be given, 2 given
	int vCommunicationsACPowerMonitorDataCount;
	int SyncFromACPowerMonitorSeen;
	
	//track ACPower Monitor status
	int vTestACPowerMoniterExists;
	bool ReportComNotDefaultDone;

	//constructor
	CACPowerMoniter();
	virtual ~CACPowerMoniter();

	//methods
	virtual bool SerialMessageComplete();
	virtual void DecodeMessage();
	virtual void RemoveMessageFromBuffer(); 
	virtual void ShowSerialData(); //also log data
	void InitializeACPowerMoniter();
	void OpenACPowerMoniterLogFiles();
	virtual void DoBackGroundWork();
	void ResolveErrors();
	CString ReadACPowerMoniterLogFiles();
};

#endif // !defined(AFX_ACPOWERMONITER_H__F8320643_454D_434E_B9BD_637A218C1CED__INCLUDED_)
