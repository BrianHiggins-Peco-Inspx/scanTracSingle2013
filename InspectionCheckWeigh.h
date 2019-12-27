// InspectionCheckWeigh.h: interface for the CInspectionCheckWeigh class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INSPECTIONCHECKWEIGH_H__4A5B9E14_702A_4E99_96C8_72E24E9F5DD0__INCLUDED_)
#define AFX_INSPECTIONCHECKWEIGH_H__4A5B9E14_702A_4E99_96C8_72E24E9F5DD0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Inspection.h"
#include "InspectionDensity.h"

class CInspectionCheckWeigh : public CInspectionDensity  
{
	//For serializing, writing to disk product info
DECLARE_SERIAL(CInspectionCheckWeigh)
//DECLARE_DYNCREATE(CInspectionDensity)
public:
	//variables
	bool vCheckWeighAlarm2OverWeightOn;
	bool vCheckWeighAlarm2UnderWeightOn;
	bool vCheckWeighAlarmLatched;
	bool vCheckWeighAlarmLatchedOld;
	WORD vCheckWeighDataSize;

	WORD vCheckWeighNumberOfSamplesTakenForAlarm;

	double *vCheckWeighData;
	WORD vCheckWeighNumberOfDataSamplesTaken;
	int vCheckWeighDataIndex;

	double *vCheckWeighCurrentMinuteData;
	WORD vCheckWeighCurrentMinuteNumberOfDataSamplesTaken;

	double *vCheckWeighTrendData;
	WORD vCheckWeighTrendNumberOfDataSamplesTaken;
	int vCheckWeighTrendDataIndex;
	WORD vCheckWeighTrendDataSize;

	double *vCheckWeighTrendDataOnMinutes;
	WORD vCheckWeighTrendNumberOfDataSamplesTakenOnMinutes;
	int vCheckWeighTrendDataIndexOnMinutes;
	WORD vCheckWeighTrendDataSizeOnMinutes;

	double vCheckWeighMFactorOld;  //for y=mx+b calibration of density to weight
	double vCheckWeighBFactorOld;
	CString vCheckWeighUnitsOld;
	double vCheckWeighMaximumGoodWeightOld;
	double vCheckWeighMinimumGoodWeightOld;
	DWORD vNumberOfAlarms;
	double vLastWeight;

	DWORD vTotalUnder;
	DWORD vTotalOver;

	//construction
	CInspectionCheckWeigh();
	virtual ~CInspectionCheckWeigh();

	//methods
	virtual void Serialize(CArchive& TempArchive);
	virtual void CopyInspection(CInspectionCheckWeigh *TempInspectionToCopy, BYTE TempOverScanMultiplier);
	void CopyDensityInspection(CInspectionDensity *TempInspectionToCopy, BYTE TempOverScanMultiplier);
	virtual CString MakeSetupDisplayString(bool TempReferenceEnabled, BYTE TempOverScanFactor, 
		CString TempEjectorNameString, CString TempUnitsString);
	void CreateDataMemory();
	void ClearDataMemory(bool TempHaveRealData);
	virtual double GetSensitivity(BOOL TempUsePassedThreshold, double TempPassedThreshold);
};

#endif // !defined(AFX_INSPECTIONCHECKWEIGH_H__4A5B9E14_702A_4E99_96C8_72E24E9F5DD0__INCLUDED_)
