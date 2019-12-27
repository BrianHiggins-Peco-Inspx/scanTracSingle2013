//ScanTrac Side View Source File
// InspectionContaminant.h: interface for the CInspectionContaminant class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INSPECTIONCONTAMINANT_H__B28B9523_4BEE_11D6_AFBD_00500466E305__INCLUDED_)
#define AFX_INSPECTIONCONTAMINANT_H__B28B9523_4BEE_11D6_AFBD_00500466E305__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Inspection.h"

class CInspectionContaminant : public CInspection  
{
	//For serializing, writing to disk product info
DECLARE_SERIAL( CInspectionContaminant )
//DECLARE_DYNCREATE(CInspectionContaminant)
public:
	//variables

	//constructor
	CInspectionContaminant();
	virtual ~CInspectionContaminant();

	//methods
	virtual void Serialize( CArchive& TempArchive );
};

#endif // !defined(AFX_INSPECTIONCONTAMINANT_H__B28B9523_4BEE_11D6_AFBD_00500466E305__INCLUDED_)
