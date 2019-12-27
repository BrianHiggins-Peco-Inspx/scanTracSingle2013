//ScanTrac Side View Source File
// Ejector.cpp: implementation of the CEjector class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ScanTrac.h"
#include "Ejector.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//IMPLEMENT_DYNCREATE(CEjector, CDigitalLine)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL( CEjector,CDigitalLine,VERSIONABLE_SCHEMA|cCurrentArchiveVersionNumber)


CEjector::CEjector()
{
	vConfirmDigitalInputLine = 0;
	vConfirmEnabled = 0;
	vConfirmActiveHigh = 1;

}

CEjector::~CEjector()
{

}

void CEjector::Serialize( CArchive& TempArchive )
{
  // call base class function first
  // base class is CObject in this case
  CDigitalLine::Serialize( TempArchive );

  // now do the stuff for our specific class
  if( TempArchive.IsStoring() )
	{
    TempArchive << 	vConfirmDigitalInputLine
			<< vConfirmEnabled
			<< vConfirmActiveHigh;
	}
  else
	{
		// may read different versions of object if have them
		// int vVersion = archive.GetObjectSchema();
		TempArchive >> vConfirmDigitalInputLine
			>> vConfirmEnabled
			>> vConfirmActiveHigh;

		SetConfirmDigitalInputLine(vConfirmDigitalInputLine);
	}
}
                                      
void CEjector::SetConfirmDigitalInputLine(BYTE TempByte)
{
	vConfirmDigitalInputLine = TempByte;
	vConfirmLineBitMask = 0;
	switch (TempByte)
	{
		case 1: vConfirmLineBitMask = 0x20; break;
		case 2: vConfirmLineBitMask = 0x01; break;
	}
}
