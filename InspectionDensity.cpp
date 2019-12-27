//ScanTrac Side View Source File
// InspectionDensity.cpp: implementation of the CInspectionDensity class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "scantrac.h"
#include "InspectionDensity.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern CProduct *vGlobalCurrentProduct;
extern CSystemConfigurationData *vGlobalConfigurationData;
extern CScanTracSystemRunningData *vGlobalRunningData;
extern CITIPCDig *vGlobalITIPCDig;
extern int vGlobalArchiveVersionNumber;
//IMPLEMENT_DYNCREATE(CInspectionDensity, CInspection)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CInspectionDensity::CInspectionDensity()
{
	vInspectionType = cDensityInspection;
	vName = "Density";
	vSpareStringName = "";
	vThreshold = 0;
	vHasAMinimumSize = false;
}

CInspectionDensity::~CInspectionDensity()
{

}

IMPLEMENT_SERIAL(CInspectionDensity,CInspection,VERSIONABLE_SCHEMA|cCurrentArchiveVersionNumber)

void CInspectionDensity::Serialize(CArchive& TempArchive)
{
  // call base class function first
  // base class is CObject in this case
  CInspection::Serialize(TempArchive);

  if( TempArchive.IsStoring())
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

		if (TempVersion > 97)
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
		if (vDensityThresholdUpper < 0)
		{
			vDensityThresholdUpper = 180;
			ReportErrorMessage("Bad float in stream corrected: vDensityThresholdUpper", cEMailInspx, 32000);
		}
		if (vDensityThresholdLower < 0)
		{
			SetDensityLowerThreshold(0);
			ReportErrorMessage("Bad float in stream corrected: vDensityThresholdLower", cEMailInspx, 32000);
		}
		if (vDensityThresholdLower > 1000000000)
		{
			SetDensityLowerThreshold(0);
			ReportErrorMessage("Bad float in stream corrected: vDensityThresholdLower", cEMailInspx, 32000);
		}
		if (vSetupHistoryDensityAutoThresholdUpper < 0)
		{
			vSetupHistoryDensityAutoThresholdUpper = 0;
			ReportErrorMessage("Bad float in stream corrected: vSetupHistoryDensityAutoThresholdUpper", cEMailInspx, 32000);
		}
		if (vSetupHistoryDensityAutoThresholdLower < 0)
		{
			vSetupHistoryDensityAutoThresholdLower = 0;
			ReportErrorMessage("Bad float in stream corrected: vSetupHistoryDensityAutoThresholdLower", cEMailInspx, 32000);
		}
		if (vSetupHistoryMinimumValue < 0)
		{
			vSetupHistoryMinimumValue = 0;
			ReportErrorMessage("Bad float in stream corrected: vSetupHistoryMinimumValue", cEMailInspx, 32000);
		}
		if (vSetupHistoryMaximumValue < 0)
		{
			vSetupHistoryMaximumValue = 0;
			ReportErrorMessage("Bad float in stream corrected: vSetupHistoryMaximumValue", cEMailInspx, 32000);
		}
		if (vSetupHistoryBadMinimumValue < 0)
		{
			vSetupHistoryBadMinimumValue = 0;
			ReportErrorMessage("Bad float in stream corrected: vSetupHistoryBadMinimumValue", cEMailInspx, 32000);
		}
		if (vSetupHistoryBadMaximumValue < 0)
		{
			vSetupHistoryBadMaximumValue = 0;
			ReportErrorMessage("Bad float in stream corrected: vSetupHistoryBadMaximumValue", cEMailInspx, 32000);
		}
		vSpareStringName = "";

		if (vDensityThresholdLower != 0)
		{
			CString TempFloatString = dtoa(vDensityThresholdLower, 2);
			vDensityThresholdLower = (float)ATOF(TempFloatString);

			if (vDensityThresholdLower < 0)
				vDensityThresholdLower = 0;

			if (vDensityThresholdLower > 1000000000)
				vDensityThresholdLower = 0;
		}
	} //end reading in data
}

void CInspectionDensity::CopyInspection(CInspectionDensity *TempInspectionToCopy, BYTE TempOverScanMultiplier)
{
	CInspection::CopyInspection((CInspection *)TempInspectionToCopy, TempOverScanMultiplier);
	SetDensityLowerThreshold(TempInspectionToCopy->vDensityThresholdLower);
}

CString CInspectionDensity::MakeSetupDisplayString(bool TempReferenceEnabled, BYTE TempOverScanFactor, CString TempEjectorNameString, CString TempUnitsString)
{
	CString vTextString = "";
	vTextString = vTextString + "\n    Name: " + vName;
	vTextString = vTextString + "\n    Inspection Type: " + GetInspectionTypeString();

	if (!vEnable)
		vTextString = vTextString + "\n    Inspection Disabled";

	if ((vInspectionType == cUnderfillByWeightInspection) || (vInspectionType == cOverfillByWeightInspection) || (vInspectionType == cCheckWeighInspection))
	{
		CString TempAbreviation = "g";
		if (vCheckWeighUnits == "Ounces")
			TempAbreviation = "oz";

		if (vHasDensityUpperThreshold)
			vTextString = vTextString + "\n\n    Under Weight Threshold: " + dtoa(ConvertToWeight(vDensityThresholdUpper),2) + TempAbreviation;

		if (vHasDensityLowerThreshold)
			vTextString = vTextString + "\n\n    Over Weight Threshold: " + dtoa(ConvertToWeight(vDensityThresholdLower),2) + TempAbreviation;

		if (vWhiteOutAnythingTouchingTheTop)
			vTextString = vTextString + "\n\n    White Out Bag Touching Top";

		if ((vCheckWeighMFactor != 1) || (vCheckWeighBFactor))
		{
			if ((vHasDensityLowerThreshold) && (vHasDensityUpperThreshold))
				vTextString = vTextString + "\n    Acceptable Weight: " + dtoa(ConvertToWeight(vDensityThresholdUpper),2) + TempAbreviation + " - " + dtoa(ConvertToWeight(vDensityThresholdLower),2) + TempAbreviation;
			else
			if (vHasDensityLowerThreshold)
				vTextString = vTextString + "\n    Acceptable Weight: 0 - " + dtoa(ConvertToWeight(vDensityThresholdLower),2) + TempAbreviation;
			else
				vTextString = vTextString + "\n    Acceptable Weight: " + dtoa(ConvertToWeight(vDensityThresholdUpper),2) + TempAbreviation + " and up";

			vTextString = vTextString + "\n\n    Weight Calibration Formula: " + vCheckWeighUnits + " = " + dtoa(vCheckWeighMFactor, 6) + " * Brightness + " + dtoa(vCheckWeighBFactor, 2);

			vTextString = vTextString + "\n    Light Sample Weight: " + dtoa(vCheckWeighMinimumGoodWeight,2) + " " + vCheckWeighUnits;
			vTextString = vTextString + "\n    Heavy Sample Weight: " + dtoa(vCheckWeighMaximumGoodWeight,2) + " " + vCheckWeighUnits;
		}
		else
			vTextString = vTextString + "\n\n    *No Weight Conversion Formula. Run Evaluate";
	} //end of Check Weighing
	else
	{ //start of density inspection
		if (vHasDensityUpperThreshold)
		{
			vTextString = vTextString + "\n\n    Upper Threshold: " + dtoa(vDensityThresholdUpper,2);

			CString TempText = "";
			TempText.LoadString(IDS_AcceptableDensity);

			if (vHasDensityLowerThreshold)
				vTextString = vTextString + "\n    " + TempText + ": " + dtoa(vDensityThresholdUpper,2) + " - " + dtoa(vDensityThresholdUpper,2);
			else
				vTextString = vTextString + "\n    " + TempText + ": 0 - " + dtoa(vDensityThresholdUpper,2);

			CString TempNumberString = "-";
			if (vSetupHistoryDensityAutoThresholdUpper)
				TempNumberString = dtoa(vSetupHistoryDensityAutoThresholdUpper,2);
			else
			if (vSetupHistoryMinimumGoodCount)
				TempNumberString = "*Evaluate Not Successful";
			else
				TempNumberString = "*Evaluate Not Done";

			vTextString = vTextString + "\n    Auto Threshold: " + TempNumberString;
		}
		if (vHasDensityLowerThreshold)
		{
			vTextString = vTextString + "\n\n    Lower Threshold: " + dtoa(vDensityThresholdLower,2);

			CString TempText = "";
			TempText.LoadString(IDS_AcceptableDensity);

			if (vHasDensityUpperThreshold)
				vTextString = vTextString + "\n    " + TempText + ": " + dtoa(vDensityThresholdLower,2) + " - " + dtoa(vDensityThresholdUpper,2);
			else
				vTextString = vTextString + "\n    " + TempText + ": " + dtoa(vDensityThresholdLower,2) + " and up";

			CString TempNumberString = "-";
			if (vSetupHistoryDensityAutoThresholdLower)
				TempNumberString = dtoa(vSetupHistoryDensityAutoThresholdLower,2);
			else
			if (vSetupHistoryMinimumGoodCount)
				TempNumberString = "*Evaluate Not Successful";
			else
				TempNumberString = "*Evaluate Not Done";
				vTextString = vTextString + "\n    Auto Threshold: " + TempNumberString;
		}
	}

	CString TempType = "Good";
	if ((vInspectionType == cUnderfillByWeightInspection) || (vInspectionType == cOverfillByWeightInspection) || (vInspectionType == cCheckWeighInspection))
		TempType = "Light Sample";
	if (vSetupHistoryMinimumGoodCount)
	{
		double TempFactor = vCheckWeighMFactor;
		if (TempFactor < 0)
			TempFactor = -TempFactor;

		vTextString = vTextString + "\n\n    " + TempType + " Average: " + dtoa(ConvertToWeight(vSetupHistoryMinimumGoodAverage),2);
		vTextString = vTextString + "\n    " + TempType + " Standard Deviation: " + dtoa(vSetupHistoryMinimumGoodStandardDeviation * TempFactor,2);
		vTextString = vTextString + "\n    " + TempType + " Minimum: " + dtoa(ConvertToWeight(vSetupHistoryMinimumValue),2);
		vTextString = vTextString + "\n    " + TempType + " Maximum: " + dtoa(ConvertToWeight(vSetupHistoryMaximumValue),2);
		vTextString = vTextString + "\n    " + TempType + " Count: " + dtoa(vSetupHistoryMinimumGoodCount, 0);
	}

	TempType = "Bad";
	if ((vInspectionType == cUnderfillByWeightInspection) || (vInspectionType == cOverfillByWeightInspection) || (vInspectionType == cCheckWeighInspection))
		TempType = "Heavy Sample";
	if (vSetupHistoryMaximumBadCount)
	{
		double TempFactor = vCheckWeighMFactor;
		if (TempFactor < 0)
			TempFactor = -TempFactor;

		vTextString = vTextString + "\n\n    " + TempType + " Average: " + dtoa(ConvertToWeight(vSetupHistoryMaximumBadAverage),2);
		vTextString = vTextString + "\n    " + TempType + " Standard Deviation: " + dtoa(vSetupHistoryMaximumBadStandardDeviation * vCheckWeighMFactor,2);
		vTextString = vTextString + "\n    " + TempType + " Minimum: " + dtoa(ConvertToWeight(vSetupHistoryBadMinimumValue),2);
		vTextString = vTextString + "\n    " + TempType + " Maximum: " + dtoa(ConvertToWeight(vSetupHistoryBadMaximumValue),2);
		vTextString = vTextString + "\n    " + TempType + " Density Count: " + dtoa(vSetupHistoryMaximumBadCount, 0);
	}

	vTextString = vTextString + "\n\n    Inspection ROI Top: " + dtoa(vROITop,2) + " " + TempUnitsString + ", " + dtoa(vROIPixelTop,0) + " Pixels";
	vTextString = vTextString + "\n    Inspection ROI Bottom: " + dtoa(vROIBottom,2) + " " + TempUnitsString + ", " + dtoa(vROIPixelBottom,0) + " Pixels";
	vTextString = vTextString + "\n    Inspection ROI Left: " + dtoa(vROILeft,2) + " " + TempUnitsString + ", " + dtoa(vROIPixelLeft,0) + " Pixels";
	vTextString = vTextString + "\n    Inspection ROI Right: " + dtoa(vROIRight,2) + " " + TempUnitsString + ", " + dtoa(vROIPixelRight,0) + " Pixels";
	vTextString = vTextString + "\n\n    Ejector: " + TempEjectorNameString;

	return vTextString;
}

double CInspectionDensity::GetSensitivity(BOOL TempUsePassedThreshold, double TempPassedThreshold)
{
	return CInspection::GetSensitivity(TempUsePassedThreshold, TempPassedThreshold);
}

double CInspectionDensity::AnalyzeImage(BYTE *TempImageIn, BYTE *TempImageReturn, BYTE TempThreshold)
{
	double TempDensityValue = 0;
	if (vEnable)
	//don't do if Tom contaminant, and learned and learn state above 3
	{
		int TempReferenceSideAdjustAmount = 0;
		WORD TempBottom = 0;
		WORD TempLeft = 0;
		WORD TempHeight = 0;
		WORD TempWidth = 0;

		vGlobalITIPCDig->GetInspectionROIBounds(this, &TempBottom, &TempLeft, &TempHeight, &TempWidth, 0, vGlobalConfigurationData->vReferenceROIsToEdges, 0);
		WORD TempRight = TempLeft + TempWidth;

		TempReferenceSideAdjustAmount = vGlobalITIPCDig->CalculateSideToSideReference(vGlobalCurrentProduct->vReferenceRight, vGlobalCurrentProduct->vReferenceLeft, &TempRight, &TempLeft);
				
		TempDensityValue = vGlobalITIPCDig->CalculateInspectionDensityAfterReference(TempImageIn, this, TempReferenceSideAdjustAmount, 0, vGlobalConfigurationData->vReferenceROIsToEdges, TempLeft, TempRight); 
	}
	return TempDensityValue;
}
