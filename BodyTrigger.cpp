//ScanTrac Side View Source File
// BodyTrigger.cpp: implementation of the CBodyTrigger class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ScanTrac.h"
#include "BodyTrigger.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//IMPLEMENT_DYNCREATE(CBodyTrigger, CDigitalLine)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL( CBodyTrigger,CDigitalLine,VERSIONABLE_SCHEMA|cCurrentArchiveVersionNumber)

CBodyTrigger::CBodyTrigger()
{

}

CBodyTrigger::~CBodyTrigger()
{

}

