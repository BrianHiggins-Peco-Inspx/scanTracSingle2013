//ScanTrac Side View Source File
// InspectionKernel.h: interface for the CInspectionKernel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INSPECTIONKERNEL_H__CE61E080_62C1_11D6_AFD1_00500466E305__INCLUDED_)
#define AFX_INSPECTIONKERNEL_H__CE61E080_62C1_11D6_AFD1_00500466E305__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Inspection.h"

class CInspectionKernel : public CInspection  
{
	//For serializing, writing to disk product info
DECLARE_SERIAL( CInspectionKernel )
//DECLARE_DYNCREATE(CInspectionKernel)
public:
	//variables

	//constructor
	CInspectionKernel();
	virtual ~CInspectionKernel();

	//methods
	virtual void Serialize( CArchive& TempArchive );
};

#endif // !defined(AFX_INSPECTIONKERNEL_H__CE61E080_62C1_11D6_AFD1_00500466E305__INCLUDED_)
