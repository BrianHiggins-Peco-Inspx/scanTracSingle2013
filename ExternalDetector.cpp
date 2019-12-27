//ScanTrac Side View Source File
// ExternalDetector.cpp: implementation of the CExternalDetector class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ScanTrac.h"
#include "ExternalDetector.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//IMPLEMENT_DYNCREATE(CExternalDetector, CDigitalLine)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL( CExternalDetector,CDigitalLine,VERSIONABLE_SCHEMA|cCurrentArchiveVersionNumber)

CExternalDetector::CExternalDetector()
{

}

CExternalDetector::~CExternalDetector()
{

}
