// InspectionGlassContaminant.cpp: implementation of the CInspectionGlassContaminant class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "scantrac.h"
#include "InspectionGlassContaminant.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern int vGlobalArchiveVersionNumber;

CInspectionGlassContaminant::CInspectionGlassContaminant()
{
	vName = "ContaminantG";
	//vInspectionType = cGlassContaminantInspection;
	vSize = 9;
	vThreshold = 55;
	SetDensityLowerThreshold(55);
	vDensityThresholdUpper = 55;
}

CInspectionGlassContaminant::~CInspectionGlassContaminant()
{

}

IMPLEMENT_SERIAL( CInspectionGlassContaminant,CInspection,VERSIONABLE_SCHEMA|cCurrentArchiveVersionNumber)

void CInspectionGlassContaminant::Serialize( CArchive& TempArchive )
{
  // call base class function first
  // base class is CObject in this case
  CInspection::Serialize( TempArchive );

  if( TempArchive.IsStoring() )
	{
    TempArchive << vSpareStringName
			<< vDensityThresholdUpper
			<< vDensityThresholdLower //obsolete
			<< vSetupHistoryDensityAutoThresholdUpper
			<< vSetupHistoryDensityAutoThresholdLower //obsolete
			<< vSetupHistoryMinimumValue
			<< vSetupHistoryMaximumValue
			<< vSetupHistoryBadMinimumValue
			<< vSetupHistoryBadMaximumValue;
	}
  else  //reading archive data
	{
		// may read different versions of object if have them
		// int vVersion = archive.GetObjectSchema();
    int TempVersion = vGlobalArchiveVersionNumber; //TempArchive.GetObjectSchema();

		if ((TempVersion > 141)) // && (TempVersion < 31))
		{  // version 141
			TempArchive >> vSpareStringName
				>> vDensityThresholdUpper
				>> vDensityThresholdLower
				>> vSetupHistoryDensityAutoThresholdUpper
				>> vSetupHistoryDensityAutoThresholdLower
				>> vSetupHistoryMinimumValue
				>> vSetupHistoryMaximumValue
				>> vSetupHistoryBadMinimumValue
				>> vSetupHistoryBadMaximumValue;
		} //end version 141
		vSpareStringName = "";
		if ((vDensityThresholdUpper > 255) || (vDensityThresholdUpper < 0))
		{
			vDensityThresholdUpper = 0;
			ReportErrorMessage("Bad float in stream corrected: vDensityThresholdUpper", cEMailInspx, 32000);
		}
		if ((vDensityThresholdLower > 255) || (vDensityThresholdLower < 0))
		{
			SetDensityLowerThreshold(0);
			ReportErrorMessage("Bad float in stream corrected: vDensityThresholdLower", cEMailInspx, 32000);
		}
		if ((vSetupHistoryDensityAutoThresholdUpper > 255) || (vSetupHistoryDensityAutoThresholdUpper < 0))
		{
			vSetupHistoryDensityAutoThresholdUpper = 0;
			ReportErrorMessage("Bad float in stream corrected: vSetupHistoryDensityAutoThresholdUpper", cEMailInspx, 32000);
		}
		if ((vSetupHistoryDensityAutoThresholdLower > 255) || (vSetupHistoryDensityAutoThresholdLower < 0))
		{
			vSetupHistoryDensityAutoThresholdLower = 0;
			ReportErrorMessage("Bad float in stream corrected: vSetupHistoryDensityAutoThresholdLower", cEMailInspx, 32000);
		}
		if ((vSetupHistoryMinimumValue > 255) || (vSetupHistoryMinimumValue < 0))
		{
			vSetupHistoryMinimumValue = 0;
			ReportErrorMessage("Bad float in stream corrected: vSetupHistoryMinimumValue", cEMailInspx, 32000);
		}
		if ((vSetupHistoryMaximumValue > 255) || (vSetupHistoryMaximumValue < 0))
		{
			vSetupHistoryMaximumValue = 0;
			ReportErrorMessage("Bad float in stream corrected: vSetupHistoryMaximumValue", cEMailInspx, 32000);
		}
		if ((vSetupHistoryBadMinimumValue > 255) || (vSetupHistoryBadMinimumValue < 0))
		{
			vSetupHistoryBadMinimumValue = 0;
			ReportErrorMessage("Bad float in stream corrected: vSetupHistoryBadMinimumValue", cEMailInspx, 32000);
		}
		if ((vSetupHistoryBadMaximumValue > 255) || (vSetupHistoryBadMaximumValue < 0))
		{
			vSetupHistoryBadMaximumValue = 0;
			ReportErrorMessage("Bad float in stream corrected: vSetupHistoryBadMaximumValue", cEMailInspx, 32000);
		}
	} //end reading in data
}

