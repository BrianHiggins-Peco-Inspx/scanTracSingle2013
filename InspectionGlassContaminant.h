// InspectionGlassContaminant.h: interface for the CInspectionGlassContaminant class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INSPECTIONGLASSCONTAMINANT_H__BAB71F10_4479_11D9_B209_00500466E305__INCLUDED_)
#define AFX_INSPECTIONGLASSCONTAMINANT_H__BAB71F10_4479_11D9_B209_00500466E305__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Inspection.h"

class CInspectionGlassContaminant : public CInspection  
{
public:
	//variables

	CInspectionGlassContaminant();
	virtual ~CInspectionGlassContaminant();

	//methods
	//For serializing, writing to disk product info
	DECLARE_SERIAL( CInspectionGlassContaminant )
	//DECLARE_DYNCREATE(CInspectionGlassContaminant)
	virtual void Serialize( CArchive& TempArchive );
};

#endif // !defined(AFX_INSPECTIONGLASSCONTAMINANT_H__BAB71F10_4479_11D9_B209_00500466E305__INCLUDED_)
