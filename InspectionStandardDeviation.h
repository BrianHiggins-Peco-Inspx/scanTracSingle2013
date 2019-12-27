//ScanTrac Side View Source File
// InspectionStandardDeviation.h: interface for the CInspectionStandardDeviation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INSPECTIONSTANDARDDEVIATION_H__CC5718C2_9D1A_11D8_B18A_00500466E305__INCLUDED_)
#define AFX_INSPECTIONSTANDARDDEVIATION_H__CC5718C2_9D1A_11D8_B18A_00500466E305__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Inspection.h"

class CInspectionStandardDeviation : public CInspection  
{
	//For serializing, writing to disk product info
	DECLARE_SERIAL( CInspectionStandardDeviation )
	//DECLARE_DYNCREATE(CInspectionStandardDeviation)
	public:
	//variables

	//construction
	CInspectionStandardDeviation();

	//methods
	virtual ~CInspectionStandardDeviation();
	virtual CString MakeSetupDisplayString(bool TempReferenceEnabled, BYTE TempOverScanFactor, CString TempEjectorNameString, CString TempUnitsString);
	virtual void Serialize(CArchive& TempArchive);
	virtual double GetSensitivity(BOOL TempUsePassedThreshold, double TempPassedThreshold);
};

#endif // !defined(AFX_INSPECTIONSTANDARDDEVIATION_H__CC5718C2_9D1A_11D8_B18A_00500466E305__INCLUDED_)
