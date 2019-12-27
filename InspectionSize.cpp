//ScanTrac Side View Source File
// InspectionSize.cpp: implementation of the CInspectionSize class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "scantrac.h"
#include "InspectionSize.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CInspectionSize::CInspectionSize()
{
	vInspectionType = cSizeInspection;
	vName = "Size";
	vThreshold = 120;
	vSize = 100;
	vHasAMinimumSize = true;
}

CInspectionSize::~CInspectionSize()
{

}

IMPLEMENT_SERIAL( CInspectionSize,CInspection,VERSIONABLE_SCHEMA|cCurrentArchiveVersionNumber)

void CInspectionSize::Serialize( CArchive& TempArchive )
{
  // call base class function first
  // base class is CObject in this case
  CInspection::Serialize( TempArchive );

}

void CInspectionSize::CopyInspection(CInspectionSize *TempInspectionToCopy, BYTE TempOverScanMultiplier)
{
	CInspection::CopyInspection((CInspection *)TempInspectionToCopy, TempOverScanMultiplier);
	SetDensityLowerThreshold(TempInspectionToCopy->vDensityThresholdLower);
}

CString CInspectionSize::MakeSetupDisplayString(bool TempReferenceEnabled, 
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

	vTextString = vTextString + "\n\n    Threshold: " + dtoa(vThreshold,0);
	vTextString = vTextString + "\n    Size: " + dtoa(vSize, 0);

	if (vInspectionType != cJarBottomContaminantInspection)
	if (vSetupHistoryMinimumGoodCount)
	{
		vTextString = vTextString + "\n\n    Good Average Size: " + dtoa(vSetupHistoryMinimumGoodAverage,2);
		vTextString = vTextString + "\n    Good Size Sigma: " + dtoa(vSetupHistoryMinimumGoodStandardDeviation,2);
		vTextString = vTextString + "\n    Good Minimum Size: " + dtoa(vSetupHistoryMinimumGood,2);
		vTextString = vTextString + "\n    Good Size Count: " + dtoa(vSetupHistoryMinimumGoodCount, 0);
	}
	else
		vTextString = vTextString + "\n\n    Evaluate Good Not Done";

	if (vSetupHistoryMaximumBadCount)
	{
		vTextString = vTextString + "\n\n    Bad Average Size: " + dtoa(vSetupHistoryMaximumBadAverage,2);
		vTextString = vTextString + "\n    Bad Size Sigma: " + dtoa(vSetupHistoryMaximumBadStandardDeviation,2);
		vTextString = vTextString + "\n    Bad Maximum Size: " + dtoa(vSetupHistoryMaximumBad,2);
		vTextString = vTextString + "\n    Bad Size Count: " + dtoa(vSetupHistoryMaximumBadCount, 0);
	}
	else
		vTextString = vTextString + "\n\n    Evaluate Bad Not Done";

	vTextString = vTextString + "\n\n    Minimum Acceptable Object Size: " + dtoa(vSize, 0) + " pixels";

	vTextString = vTextString + "\n\n    Inspection ROI Top: " + dtoa(vROITop,2) + " " + TempUnitsString + ", " + dtoa(vROIPixelTop,0) + " Pixels";
	vTextString = vTextString + "\n    Inspection ROI Bottom: " + dtoa(vROIBottom,2) + " " + TempUnitsString + ", " + dtoa(vROIPixelBottom,0) + " Pixels";
	vTextString = vTextString + "\n    Inspection ROI Left: " + dtoa(vROILeft,2) + " " + TempUnitsString + ", " + dtoa(vROIPixelLeft,0) + " Pixels";
	vTextString = vTextString + "\n    Inspection ROI Right: " + dtoa(vROIRight,2) + " " + TempUnitsString + ", " + dtoa(vROIPixelRight,0) + " Pixels";

	if (vInvertEjectLogic)
		vTextString = vTextString + "\n\n    Eject when object present";
	else
		vTextString = vTextString + "\n\n    Eject when object NOT present";

	vTextString = vTextString + "\n\n    Ejector: " + TempEjectorNameString;

	return vTextString;
}

double CInspectionSize::GetSensitivity(BOOL TempUsePassedThreshold, double TempPassedThreshold)
{
	return 0xFF;
}
