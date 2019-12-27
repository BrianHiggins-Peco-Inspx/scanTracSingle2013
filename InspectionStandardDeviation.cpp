//ScanTrac Side View Source File
// InspectionStandardDeviation.cpp: implementation of the CInspectionStandardDeviation class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "scantrac.h"
#include "InspectionStandardDeviation.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern int vGlobalArchiveVersionNumber;
//IMPLEMENT_DYNCREATE(CInspectionStandardDeviation, CInspection)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CInspectionStandardDeviation::CInspectionStandardDeviation()
{
	vName = "Texture";
	vInspectionType = cStandardDeviationInspection;
	vThreshold = 1;
	vDensityThresholdUpper = 10;
	SetDensityLowerThreshold(1);
	vHasAMinimumSize = 0;
	vHasDensityLowerThreshold = true;
	vHasDensityUpperThreshold = true;
}

CInspectionStandardDeviation::~CInspectionStandardDeviation()
{

}

IMPLEMENT_SERIAL( CInspectionStandardDeviation,CInspection,VERSIONABLE_SCHEMA|cCurrentArchiveVersionNumber)

void CInspectionStandardDeviation::Serialize( CArchive& TempArchive )
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

		if ((TempVersion > 97)) // && (TempVersion < 31))
		{  // version 98
			TempArchive >> vSpareStringName
				>> vDensityThresholdUpper
				>> vDensityThresholdLower
				>> vSetupHistoryDensityAutoThresholdUpper
				>> vSetupHistoryDensityAutoThresholdLower
				>> vSetupHistoryMinimumValue
				>> vSetupHistoryMaximumValue
				>> vSetupHistoryBadMinimumValue
				>> vSetupHistoryBadMaximumValue;
		} //end version 92
		vSpareStringName = "";
	}
}

CString CInspectionStandardDeviation::MakeSetupDisplayString(bool TempReferenceEnabled, 
	BYTE TempOverScanFactor, CString TempEjectorNameString, CString TempUnitsString)
{
	CString TempStep1 = "Step 1";
	CString TempStep2 = "Step 2";
	CString TempStep3 = "Step 3";
	CString TempStep3a = "Step 3a";
	CString TempStep3b = "Step 3b";
	CString TempStep4 = "Step 4";
	CString TempStep5 = "Step 5";
	CString TempStep6 = "Step 6";
	CString TempStep7 = "Step 7";
	if (PasswordOK(cTemporaryInspxPassword,false))
	{
		TempStep1 = "Reference";
		TempStep2 = "Average";
		TempStep3 = "Structure";
		TempStep3a = "Contaminant Structure";
		TempStep3b = "Void Structure";
		TempStep4 = "Histogram";
		TempStep5 = "Enhance";
		TempStep6 = "Dilation";
		TempStep7 = "Image Factors";
	}
	CString vTextString = "";

	vTextString = vTextString + "\n    Name: " + vName;

	if (!vEnable)
		vTextString = vTextString + "\n    Inspection Disabled";
	vTextString = vTextString + "\n    Inspection Type: " + GetInspectionTypeString();

	CString TempNumberString = dtoa(vDensityThresholdUpper,2);
	vTextString = vTextString + "\n\n    Light Texture Threshold (Upper): " + TempNumberString;

	if (vSetupHistoryDensityAutoThresholdUpper)
		TempNumberString = dtoa(vSetupHistoryDensityAutoThresholdUpper,2);
	else
	if (vSetupHistoryMinimumGoodCount)
		TempNumberString = "Evaluate Not Successful";
	else
		TempNumberString = "Evaluate Not Done";
	vTextString = vTextString + "\n    Auto Light Texture Threshold (Upper): " + TempNumberString;

	TempNumberString = dtoa(vDensityThresholdLower,2);
	vTextString = vTextString + "\n\n    Heavy Texture Threshold (Lower): " + TempNumberString;

	if (vSetupHistoryDensityAutoThresholdLower)
		TempNumberString = dtoa(vSetupHistoryDensityAutoThresholdLower,2);
	else
	if (vSetupHistoryMinimumGoodCount)
		TempNumberString = "Evaluate Not Successful";
	else
		TempNumberString = "Evaluate Not Done";
	vTextString = vTextString + "\n    Auto Threshold: " + TempNumberString;

	if (vSetupHistoryMinimumGoodCount)
	{
		vTextString = vTextString + "\n\n    Good Average Texture: " + dtoa(vSetupHistoryMinimumGoodAverage,2);
		vTextString = vTextString + "\n    Good Texture Sigma: " + dtoa(vSetupHistoryMinimumGoodStandardDeviation,2);
		vTextString = vTextString + "\n    Good Minimum Texture: " + dtoa(vSetupHistoryMinimumValue,2);
		vTextString = vTextString + "\n    Good Maximum Texture: " + dtoa(vSetupHistoryMaximumValue,2);
		vTextString = vTextString + "\n    Good Texture Count: " + dtoa(vSetupHistoryMinimumGoodCount, 0);
	}
	else
		vTextString = vTextString + "\n\n    Evaluate Good Not Done";

	if (vSetupHistoryMaximumBadCount)
	{
		vTextString = vTextString + "\n\n    Bad Average Texture: " + dtoa(vSetupHistoryMaximumBadAverage,2);
		vTextString = vTextString + "\n    Bad Texture Sigma: " + dtoa(vSetupHistoryMaximumBadStandardDeviation,2);
		vTextString = vTextString + "\n    Bad Minimum Texture: " + dtoa(vSetupHistoryBadMinimumValue,2);
		vTextString = vTextString + "\n    Bad Maximum Texture: " + dtoa(vSetupHistoryBadMaximumValue,2);
		vTextString = vTextString + "\n    Bad Texture Count: " + dtoa(vSetupHistoryMaximumBadCount, 0);
	}
	else
		vTextString = vTextString + "\n\n    Evaluate Bad Not Done";

	vTextString = vTextString + "\n\n    Inspection ROI Top: " + dtoa(vROITop,2) + " " + TempUnitsString + ", " + dtoa(vROIPixelTop,0) + " Pixels";
	vTextString = vTextString + "\n    Inspection ROI Bottom: " + dtoa(vROIBottom,2) + " " + TempUnitsString + ", " + dtoa(vROIPixelBottom,0) + " Pixels";
	vTextString = vTextString + "\n    Inspection ROI Left: " + dtoa(vROILeft,2) + " " + TempUnitsString + ", " + dtoa(vROIPixelLeft,0) + " Pixels";
	vTextString = vTextString + "\n    Inspection ROI Right: " + dtoa(vROIRight,2) + " " + TempUnitsString + ", " + dtoa(vROIPixelRight,0) + " Pixels";
	vTextString = vTextString + "\n\n    Ejector: " + TempEjectorNameString;

	return vTextString;
}
double CInspectionStandardDeviation::GetSensitivity(BOOL TempUsePassedThreshold, double TempPassedThreshold)
{
	return 0xFF;
}
