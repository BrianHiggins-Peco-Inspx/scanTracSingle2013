//ScanTrac Side View Source File
// InspectionBottomContaminant.h: interface for the CInspectionBottomContaminant class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INSPECTIONBOTTOMCONTAMINANT_H__EAC15670_D687_11D8_B1B6_00500466E305__INCLUDED_)
#define AFX_INSPECTIONBOTTOMCONTAMINANT_H__EAC15670_D687_11D8_B1B6_00500466E305__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Inspection.h"

class CInspectionBottomContaminant : public CInspection  
{
public:
	//variables

	//constructor
	CInspectionBottomContaminant();
	virtual ~CInspectionBottomContaminant();

	//methods
	//For serializing, writing to disk product info
	DECLARE_SERIAL(CInspectionBottomContaminant)
	//DECLARE_DYNCREATE(CInspectionBottomContaminant)
	virtual void Serialize(CArchive& TempArchive);

};

#endif // !defined(AFX_INSPECTIONBOTTOMCONTAMINANT_H__EAC15670_D687_11D8_B1B6_00500466E305__INCLUDED_)
