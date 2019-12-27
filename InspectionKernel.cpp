//ScanTrac Side View Source File
// InspectionKernel.cpp: implementation of the CInspectionKernel class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "scantrac.h"
#include "InspectionKernel.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CInspectionKernel::CInspectionKernel()
{
	vName = "C Kernel";
	vInspectionType = cKernelInspection;
	vSize = 5;
	vThreshold = 128;
}

CInspectionKernel::~CInspectionKernel()
{

}

IMPLEMENT_SERIAL( CInspectionKernel,CInspection,VERSIONABLE_SCHEMA|cCurrentArchiveVersionNumber)

void CInspectionKernel::Serialize( CArchive& TempArchive )
{
  // call base class function first
  // base class is CObject in this case
  CInspection::Serialize( TempArchive );
}
