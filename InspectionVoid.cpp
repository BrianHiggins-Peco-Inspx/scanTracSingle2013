//ScanTrac Side View Source File
// InspectionVoid.cpp: implementation of the CInspectionVoid class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "scantrac.h"
#include "InspectionVoid.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//IMPLEMENT_DYNCREATE(CInspectionVoid, CInspection)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CInspectionVoid::CInspectionVoid()
{
	vName = "Void";
	vInspectionType = cVoidInspection;
	vSize = 10;
}

CInspectionVoid::~CInspectionVoid()
{

}

IMPLEMENT_SERIAL (CInspectionVoid, CInspection, VERSIONABLE_SCHEMA|cCurrentArchiveVersionNumber)

void CInspectionVoid::Serialize(CArchive& TempArchive)
{
  // call base class function first
  // base class is CObject in this case
  CInspection::Serialize(TempArchive);
}
