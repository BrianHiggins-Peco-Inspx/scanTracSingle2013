//ScanTrac Side View Source File
// InspectionVoid.h: interface for the CInspectionVoid class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INSPECTIONVOID_H__B28B9524_4BEE_11D6_AFBD_00500466E305__INCLUDED_)
#define AFX_INSPECTIONVOID_H__B28B9524_4BEE_11D6_AFBD_00500466E305__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Inspection.h"

class CInspectionVoid : public CInspection  
{
	//For serializing, writing to disk product info
DECLARE_SERIAL( CInspectionVoid )
//DECLARE_DYNCREATE(CInspectionVoid)
public:
	//variables

	//constructor
	CInspectionVoid();
	virtual ~CInspectionVoid();

	//methods
	virtual void Serialize( CArchive& TempArchive );

};

#endif // !defined(AFX_INSPECTIONVOID_H__B28B9524_4BEE_11D6_AFBD_00500466E305__INCLUDED_)
