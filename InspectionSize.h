//ScanTrac Side View Source File
// InspectionSize.h: interface for the CInspectionSize class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INSPECTIONSIZE_H__C5A0B751_12D5_11D7_B056_00500466E305__INCLUDED_)
#define AFX_INSPECTIONSIZE_H__C5A0B751_12D5_11D7_B056_00500466E305__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Inspection.h"

class CInspectionSize : public CInspection  
{
	//For serializing, writing to disk product info
DECLARE_SERIAL( CInspectionSize )
//DECLARE_DYNCREATE(CInspectionSize)

public:
	//variables

	//construction
	CInspectionSize();
	virtual ~CInspectionSize();

	//methods
	virtual void Serialize( CArchive& TempArchive );
	virtual void CopyInspection(CInspectionSize *TempInspectionToCopy, BYTE TempOverScanMultiplier);
	virtual CString MakeSetupDisplayString(bool TempReferenceEnabled, BYTE TempOverScanFactor, 
		CString TempEjectorNameString, CString TempUnitsString);
	virtual double GetSensitivity(BOOL TempUsePassedThreshold, double TempPassedThreshold);
};

#endif // !defined(AFX_INSPECTIONSIZE_H__C5A0B751_12D5_11D7_B056_00500466E305__INCLUDED_)
