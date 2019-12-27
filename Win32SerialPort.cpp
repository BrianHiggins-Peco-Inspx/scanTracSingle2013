// Win32SerialPort.cpp: implementation of the CWin32SerialPort class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ScanTrac.h"
#include "Win32SerialPort.h"
#include "NoticeDialog.h"
#include "AlphaEntryDialog.h"
#include "SelectItemDialog.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CWin32SerialPort,CObject,VERSIONABLE_SCHEMA|cCurrentArchiveVersionNumber)

CWin32SerialPort::CWin32SerialPort()
{
}

CWin32SerialPort::~CWin32SerialPort()
{

}

//IMPLEMENT_DYNCREATE(CWin32SerialPort, CObject)
/*
int CWin32SerialPort::WriteConfigurationDataToINIFile()
{
	int TempOK = 1;
	return TempOK;
}

int CWin32SerialPort::ReadConfigurationDataFromINIFile()
{
	int TempOK = 1;
	return TempOK;
}
*/

void CWin32SerialPort::Serialize(CArchive& TempArchive)
{
  // call base class function first
  // base class is CObject in this case
  CObject::Serialize( TempArchive );

  // now do the stuff for our specific class
  if( TempArchive.IsStoring() )
	{
		TempArchive << vLogSerialData
			<< vLogSyncData;
	}
  else  //reading archive data
	{
		// may read different versions of object if have them
		// int vVersion = archive.GetObjectSchema();
		int TempArchiveVersionNumber;
    TempArchiveVersionNumber = TempArchive.GetObjectSchema();

		if (TempArchiveVersionNumber > 36) //version 37
		{
			TempArchive >> vLogSerialData
				>> vLogSyncData;
		}  //end archive version 37
	}
}
