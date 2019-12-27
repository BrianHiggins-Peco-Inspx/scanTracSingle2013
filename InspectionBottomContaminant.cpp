//ScanTrac Side View Source File
// InspectionBottomContaminant.cpp: implementation of the CInspectionBottomContaminant class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "scantrac.h"
#include "InspectionBottomContaminant.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CInspectionBottomContaminant::CInspectionBottomContaminant()
{
	vName = "Jar Bottom";
	vInspectionType = cJarBottomContaminantInspection;
	vSize = 2;
	vThreshold = 64;
	vAllowChangingAdvancedOptions = true;
}

CInspectionBottomContaminant::~CInspectionBottomContaminant()
{

}

IMPLEMENT_SERIAL( CInspectionBottomContaminant,CInspection,VERSIONABLE_SCHEMA|cCurrentArchiveVersionNumber)

void CInspectionBottomContaminant::Serialize( CArchive& TempArchive )
{
  // call base class function first
  // base class is CObject in this case
  CInspection::Serialize( TempArchive );
}
