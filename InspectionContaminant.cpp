//ScanTrac Side View Source File
// InspectionContaminant.cpp: implementation of the CInspectionContaminant class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "scantrac.h"
#include "InspectionContaminant.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//IMPLEMENT_DYNCREATE(CInspectionContaminant, CInspection)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CInspectionContaminant::CInspectionContaminant()
{
	vName = "Contaminant";
	vInspectionType = cContaminantInspection;
}

CInspectionContaminant::~CInspectionContaminant()
{

}

IMPLEMENT_SERIAL( CInspectionContaminant,CInspection,VERSIONABLE_SCHEMA|cCurrentArchiveVersionNumber)

void CInspectionContaminant::Serialize( CArchive& TempArchive )
{
  // call base class function first
  // base class is CObject in this case
  CInspection::Serialize( TempArchive );
}

