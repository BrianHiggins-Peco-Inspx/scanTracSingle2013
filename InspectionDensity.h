//ScanTrac Side View Source File
// InspectionDensity.h: interface for the CInspectionDensity class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INSPECTIONDENSITY_H__B28B9522_4BEE_11D6_AFBD_00500466E305__INCLUDED_)
#define AFX_INSPECTIONDENSITY_H__B28B9522_4BEE_11D6_AFBD_00500466E305__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Inspection.h"

class CInspectionDensity : public CInspection  
{
	//For serializing, writing to disk product info
DECLARE_SERIAL(CInspectionDensity)
//DECLARE_DYNCREATE(CInspectionDensity)
public:
	//variables

	//construction
	CInspectionDensity();
	virtual ~CInspectionDensity();

	//methods
	virtual void Serialize(CArchive& TempArchive);
	virtual void CopyInspection(CInspectionDensity *TempInspectionToCopy, BYTE TempOverScanMultiplier);
	virtual CString MakeSetupDisplayString(bool TempReferenceEnabled, BYTE TempOverScanFactor, CString TempEjectorNameString, CString TempUnitsString);
	virtual double GetSensitivity(BOOL TempUsePassedThreshold, double TempPassedThreshold);
	virtual double AnalyzeImage(BYTE *TempImageIn, BYTE *TempImageReturn, BYTE TempThreshold);
};

#endif // !defined(AFX_INSPECTIONDENSITY_H__B28B9522_4BEE_11D6_AFBD_00500466E305__INCLUDED_)
