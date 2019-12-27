//ScanTrac Side View Source File
// DigitalLine.cpp: implementation of the CDigitalLine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ScanTrac.h"
#include "DigitalLine.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//IMPLEMENT_DYNCREATE(CDigitalLine, CObject)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL( CDigitalLine,CObject,VERSIONABLE_SCHEMA|cCurrentArchiveVersionNumber)

CDigitalLine::CDigitalLine()
{
	vNumber = 0;
	vName = "Un-Named";
	vDigitalInputLine = 0;
	vLineBitMask = 0;
	vEnabled = false;
	vActiveHigh = true;
}

CDigitalLine::~CDigitalLine()
{

}

void CDigitalLine::Serialize(CArchive& TempArchive)
{
  // call base class function first
  // base class is CObject in this case
  CObject::Serialize(TempArchive);

  // now do the stuff for our specific class
  if( TempArchive.IsStoring())
	{
    TempArchive << vNumber
			<< vName
			<< vDigitalInputLine
			<< vEnabled
			<< vActiveHigh;
	}
  else
	{
		// may read different versions of object if have them
		// int vVersion = archive.GetObjectSchema();
		TempArchive >> vNumber
			>> vName
			>> vDigitalInputLine
			>> vEnabled
			>> vActiveHigh;

		SetDigitalInputLine(vDigitalInputLine);
	}
}
                                      
void CDigitalLine::SetDigitalInputLine(BYTE TempByte)
{
	vDigitalInputLine = TempByte;
	vLineBitMask = 0x01;
	if (TempByte > 1)
		for (BYTE TempLoop = 1; TempLoop < TempByte; TempLoop++)
		  vLineBitMask = vLineBitMask << 1;
}
