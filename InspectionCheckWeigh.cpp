// InspectionCheckWeigh.cpp: implementation of the CInspectionCheckWeigh class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "scantrac.h"
#include "InspectionCheckWeigh.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern int vGlobalArchiveVersionNumber;
//IMPLEMENT_DYNCREATE(CInspectionCheckWeigh, CInspection)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CInspectionCheckWeigh::CInspectionCheckWeigh()
{
	vInspectionType = cCheckWeighInspection;
	vName = "Weight Trending";
	vSpareStringName = "";
	vThreshold = 0;
	vHasAMinimumSize = false;
	vCheckWeighAlarm2OverWeightOn = false;
	vCheckWeighAlarm2UnderWeightOn = false;
	vCheckWeighAlarmLatched = false;
	vCheckWeighAlarmLatchedOld = false;
	vNumberOfAlarms = 0;

	//vShowRejects = false;
	//vSaveRejects = false;

	vCheckWeighData = NULL;
	vCheckWeighDataSize = 500;
	vCheckWeighNumberOfDataSamplesTaken = 0;
	vCheckWeighDataIndex = -1;

	vCheckWeighCurrentMinuteNumberOfDataSamplesTaken = 0;
	vCheckWeighCurrentMinuteData = NULL;

	vCheckWeighTrendData = NULL;
	vCheckWeighTrendDataSize = 12000;
	vCheckWeighTrendNumberOfDataSamplesTaken = 0;
	vCheckWeighTrendDataIndex = -1;

	vCheckWeighTrendDataOnMinutes = NULL;
	vCheckWeighTrendDataSizeOnMinutes = 7000;
	vCheckWeighTrendNumberOfDataSamplesTakenOnMinutes = 0;
	vCheckWeighTrendDataIndexOnMinutes = -1;

	vCheckWeighNumberOfSamplesTakenForAlarm = 0;
	vCheckWeighTrendAlarmMaximum = 0;
	vCheckWeighTrendAlarmMinimum = 0;
	vLastWeight = 0;
	vTotalUnder = 0;
	vTotalOver = 0;

	vCheckWeighMFactorOld = 1;  //for y=mx+b calibration of density to weight
	vCheckWeighBFactorOld = 0;
	vCheckWeighUnitsOld = "Grams";
	vCheckWeighMaximumGoodWeightOld = 0;
	vCheckWeighMinimumGoodWeightOld = 0;
	CreateDataMemory();
}

CInspectionCheckWeigh::~CInspectionCheckWeigh()
{
	if (vCheckWeighData)
	{
		free(vCheckWeighData);
		vCheckWeighData = NULL;
	}
	
	if (vCheckWeighCurrentMinuteData)
	{
		free(vCheckWeighCurrentMinuteData);
		vCheckWeighCurrentMinuteData = NULL;
	}
	
	if (vCheckWeighTrendData)
	{
		free(vCheckWeighTrendData);
		vCheckWeighTrendData = NULL;
	}
	if (vCheckWeighTrendDataOnMinutes)
	{
		free(vCheckWeighTrendDataOnMinutes);
		vCheckWeighTrendDataOnMinutes = NULL;
	}
}

IMPLEMENT_SERIAL(CInspectionCheckWeigh, CInspectionDensity, VERSIONABLE_SCHEMA|cCurrentArchiveVersionNumber)

void CInspectionCheckWeigh::Serialize(CArchive& TempArchive)
{
  // call base class function first
  // base class is CObject in this case
  CInspectionDensity::Serialize(TempArchive);

  if (TempArchive.IsStoring())
	{
    TempArchive 
			<< vCheckWeighMFactorOld  //for y=mx+b calibration of density to weight
			<< vCheckWeighBFactorOld
			<< vCheckWeighUnitsOld
			<< vCheckWeighMaximumGoodWeightOld
			<< vCheckWeighMinimumGoodWeightOld
			<< vCheckWeighDataSize
			<< vCheckWeighTrendDataSize;

	}
  else  //reading archive data
	{
		// may read different versions of object if have them
		// int vVersion = archive.GetObjectSchema();
    int TempVersion = vGlobalArchiveVersionNumber; //TempArchive.GetObjectSchema();

		if (TempVersion > 148)
		{  // version 149 and up
			TempArchive 
				>> vCheckWeighMFactorOld  //for y=mx+b calibration of density to weight
				>> vCheckWeighBFactorOld
				>> vCheckWeighUnitsOld
				>> vCheckWeighMaximumGoodWeightOld
				>> vCheckWeighMinimumGoodWeightOld
				>> vCheckWeighDataSize
				>> vCheckWeighTrendDataSize;

		} //end version 149 and up
		else
		if (TempVersion > 97)
		{  // version 98 to 148
			TempArchive 
				//>> vSpareStringName
				//>> vDensityThresholdUpper
				//>> vDensityThresholdLower
				//>> vSetupHistoryDensityAutoThresholdUpper
				//>> vSetupHistoryDensityAutoThresholdLower
				//>> vSetupHistoryMinimumValue
				//>> vSetupHistoryMaximumValue
				//>> vSetupHistoryBadMinimumValue
				//>> vSetupHistoryBadMaximumValue;

				>> vCheckWeighMFactorOld  //for y=mx+b calibration of density to weight
				>> vCheckWeighBFactorOld
				>> vCheckWeighUnitsOld
				>> vCheckWeighMaximumGoodWeightOld
				>> vCheckWeighMinimumGoodWeightOld
				>> vCheckWeighDataSize;

				if ((vCheckWeighMFactorOld != 1) || (vCheckWeighBFactorOld != 0))
				{ //if old variables have values, copy to new variables and clear the old ones.
					if ((vCheckWeighMFactorOld > 1000000) || (vCheckWeighMFactorOld < -1000000))
					{
						vCheckWeighMFactorOld = 1;
						ReportErrorMessage("Bad float in stream corrected: vCheckWeighMFactorOld", cEMailInspx, 32000);
					}
					if ((vCheckWeighBFactorOld > 1000000) || (vCheckWeighBFactorOld < -1000000))
					{
						vCheckWeighBFactorOld = 0;
						ReportErrorMessage("Bad float in stream corrected: vCheckWeighBFactorOld", cEMailInspx, 32000);
					}

					if ((vCheckWeighMaximumGoodWeightOld > 1000000) || (vCheckWeighMaximumGoodWeightOld < -1000000))
					{
						vCheckWeighMaximumGoodWeightOld = 0;
						ReportErrorMessage("Bad float in stream corrected: vCheckWeighMaximumGoodWeightOld", cEMailInspx, 32000);
					}
					if ((vCheckWeighMinimumGoodWeightOld > 1000000) || (vCheckWeighMinimumGoodWeightOld < -1000000))
					{
						vCheckWeighMinimumGoodWeightOld = 0;
						ReportErrorMessage("Bad float in stream corrected: vCheckWeighMinimumGoodWeightOld", cEMailInspx, 32000);
					}
					vCheckWeighMFactor = vCheckWeighMFactorOld;  //for y=mx+b calibration of density to weight
					vCheckWeighBFactor = vCheckWeighBFactorOld;
					vCheckWeighUnits = vCheckWeighUnitsOld;
					vCheckWeighMaximumGoodWeight = vCheckWeighMaximumGoodWeightOld;
					vCheckWeighMinimumGoodWeight = vCheckWeighMinimumGoodWeightOld;

					vCheckWeighMFactorOld = 1;  //for y=mx+b calibration of density to weight
					vCheckWeighBFactorOld = 0;
					vCheckWeighUnitsOld = "Ounces";
					vCheckWeighMaximumGoodWeightOld = 0;
					vCheckWeighMinimumGoodWeightOld = 0;
				}
			}// version 98 to 148

		CreateDataMemory();
		if ((vCheckWeighDataSize > 15000) || (vCheckWeighDataSize < 10))
		{
			vCheckWeighDataSize = 500;
			ReportErrorMessage("Bad value in stream corrected: vDensityThresholdUpper", cEMailInspx, 32000);
		}

		if ((vCheckWeighTrendDataSize > 25000) || (vCheckWeighTrendDataSize < 10))
		{
			vCheckWeighTrendDataSize = 12000;
			ReportErrorMessage("Bad value in stream corrected: vCheckWeighTrendDataSize", cEMailInspx, 32000);
		}
	} //end reading in data
}

void CInspectionCheckWeigh::CopyInspection(CInspectionCheckWeigh *TempInspectionToCopy, BYTE TempOverScanMultiplier)
{
	if (TempInspectionToCopy)
	{
		CInspectionDensity::CopyInspection((CInspectionDensity *)TempInspectionToCopy, TempOverScanMultiplier);

		SetDensityLowerThreshold(TempInspectionToCopy->vDensityThresholdLower);

		if ((TempInspectionToCopy->vCheckWeighDataSize <= 15000) && (TempInspectionToCopy->vCheckWeighDataSize > 1))
			vCheckWeighDataSize = TempInspectionToCopy->vCheckWeighDataSize;
		else
			vCheckWeighDataSize = 500;

		if ((TempInspectionToCopy->vCheckWeighTrendDataSize <= 25000) && (TempInspectionToCopy->vCheckWeighTrendDataSize > 1))
			vCheckWeighTrendDataSize = TempInspectionToCopy->vCheckWeighTrendDataSize;
		else
			vCheckWeighTrendDataSize = 12000;

		vCheckWeighAlarm2OverWeightOn = TempInspectionToCopy->vCheckWeighAlarm2OverWeightOn;
		vCheckWeighAlarm2UnderWeightOn = TempInspectionToCopy->vCheckWeighAlarm2UnderWeightOn;
		vCheckWeighAlarmLatched = TempInspectionToCopy->vCheckWeighAlarmLatched;
		vCheckWeighAlarmLatchedOld = TempInspectionToCopy->vCheckWeighAlarmLatchedOld;
		vNumberOfAlarms = TempInspectionToCopy->vNumberOfAlarms;

		CreateDataMemory();

		vTotalUnder = TempInspectionToCopy->vTotalUnder;
		vTotalOver = TempInspectionToCopy->vTotalOver;
		vLastWeight = TempInspectionToCopy->vLastWeight;
		vCheckWeighDataIndex = TempInspectionToCopy->vCheckWeighDataIndex;
		vCheckWeighTrendDataIndex = TempInspectionToCopy->vCheckWeighTrendDataIndex;
		vCheckWeighNumberOfDataSamplesTaken = TempInspectionToCopy->vCheckWeighNumberOfDataSamplesTaken;
		vCheckWeighTrendNumberOfDataSamplesTaken = TempInspectionToCopy->vCheckWeighTrendNumberOfDataSamplesTaken;
		vCheckWeighNumberOfSamplesTakenForAlarm = TempInspectionToCopy->vCheckWeighNumberOfSamplesTakenForAlarm;
		vCheckWeighCurrentMinuteNumberOfDataSamplesTaken = TempInspectionToCopy->vCheckWeighCurrentMinuteNumberOfDataSamplesTaken;

		vCheckWeighTrendDataSizeOnMinutes = TempInspectionToCopy->vCheckWeighTrendDataSizeOnMinutes;
		vCheckWeighTrendNumberOfDataSamplesTakenOnMinutes = TempInspectionToCopy->vCheckWeighTrendNumberOfDataSamplesTakenOnMinutes;
		vCheckWeighTrendDataIndexOnMinutes = TempInspectionToCopy->vCheckWeighTrendDataIndexOnMinutes;
		
		if (vCheckWeighData)
		if (TempInspectionToCopy->vCheckWeighData)
			CopyMemory(vCheckWeighData,TempInspectionToCopy->vCheckWeighData, vCheckWeighDataSize * sizeof(double));

		if (vCheckWeighCurrentMinuteData)
		if (TempInspectionToCopy->vCheckWeighCurrentMinuteData)
			CopyMemory(vCheckWeighCurrentMinuteData,TempInspectionToCopy->vCheckWeighCurrentMinuteData, cCheckWeighMaximumSamplesInOneMinute * sizeof(double));

		if (vCheckWeighTrendData)
		if (TempInspectionToCopy->vCheckWeighTrendData)
			CopyMemory(vCheckWeighTrendData,TempInspectionToCopy->vCheckWeighTrendData, vCheckWeighTrendDataSize * sizeof(double));

		if (vCheckWeighTrendDataOnMinutes)
		if (TempInspectionToCopy->vCheckWeighTrendDataOnMinutes)
			CopyMemory(vCheckWeighTrendDataOnMinutes,TempInspectionToCopy->vCheckWeighTrendDataOnMinutes, vCheckWeighTrendDataSizeOnMinutes * sizeof(double));
	}
}

void CInspectionCheckWeigh::CopyDensityInspection(CInspectionDensity *TempInspectionToCopy, BYTE TempOverScanMultiplier)
{
	if (TempInspectionToCopy)
	{
		CInspectionDensity::CopyInspection((CInspectionDensity *)TempInspectionToCopy, TempOverScanMultiplier);

		SetDensityLowerThreshold(TempInspectionToCopy->vDensityThresholdLower);
	}
}

CString CInspectionCheckWeigh::MakeSetupDisplayString(bool TempReferenceEnabled, 
	BYTE TempOverScanFactor, CString TempEjectorNameString, CString TempUnitsString)
{
	CString vTextString = "";

	CString TempWeightAbreviation = "g";
	if (vCheckWeighUnits ==  "Ounces")
			TempWeightAbreviation = "oz";
	if (((vCheckWeighBFactor == 0) || (vCheckWeighMFactor == 1)) && (!vCheckWeighMFactor))
		TempWeightAbreviation = " in brightness";


	vTextString = vTextString + "\n    Name: " + vName;

	if (!vEnable)
		vTextString = vTextString + "\n    Inspection Disabled";
	vTextString = vTextString + "\n    Inspection Type: " + GetInspectionTypeString();

	if (vHasDensityLowerThreshold)
	{
		CString TempNumberString = "";
		if (vHasDensityUpperThreshold)
			TempNumberString = "Lower ";

		vTextString = vTextString + "\n\n    " + TempNumberString + "Threshold: " + dtoa(ConvertToWeight(vDensityThresholdLower),2) + TempWeightAbreviation;
	}

	if (vHasDensityUpperThreshold)
	{
		CString TempNumberString = "";
		if (vHasDensityLowerThreshold)
			TempNumberString = "Upper ";

		vTextString = vTextString + "\n\n    " + TempNumberString + "Threshold: " + dtoa(ConvertToWeight(vDensityThresholdUpper),2) + TempWeightAbreviation;
	}

	CString TempText = "";
	if (((vCheckWeighBFactor != 0) || (vCheckWeighMFactor != 1)) && (vCheckWeighMFactor))
		TempText.LoadString(IDS_AcceptableWeight);
	else
		TempText.LoadString(IDS_AcceptableDensity);

	if (vHasDensityLowerThreshold)
		vTextString = vTextString + "\n    " + TempText + ": " + dtoa(ConvertToWeight(vDensityThresholdLower),2) + TempWeightAbreviation + " and up";
	else
		vTextString = vTextString + "\n    " + TempText + ": 0 - " + dtoa(ConvertToWeight(vDensityThresholdUpper),2) + TempWeightAbreviation;

	if ((vCheckWeighMFactor != 1) || (vCheckWeighBFactor))
	{
		vTextString = vTextString + "\n\n    Calibration Formula: " + vCheckWeighUnits + " = " + dtoa(vCheckWeighMFactor,6) + " X Raw Density + " + dtoa(vCheckWeighBFactor,2);
		vTextString = vTextString + "\n    Light Sample Weight for calibration: " + dtoa(vCheckWeighMinimumGoodWeight,2) + " " + TempWeightAbreviation;
		vTextString = vTextString + "\n    Heavy Sample Weight for calibration: " + dtoa(vCheckWeighMaximumGoodWeight,2) + " " + TempWeightAbreviation;
	}
	else
		vTextString = vTextString + "\n\n    No Conversion Formula. Run Evaluate";

	if ((vCheckWeighTrendAlarmMaximum) && (vCheckWeighTrendAlarmMinimum))
	{
		vTextString = vTextString + "\n\n    Weight Trending Setup:";
		vTextString = vTextString + "\n        Under Weight Threshold: " + dtoa(ConvertToWeight(vCheckWeighTrendAlarmMinimum), 2) + " " + TempWeightAbreviation;
		vTextString = vTextString + "\n        Over Weight Threshold: " + dtoa(ConvertToWeight(vCheckWeighTrendAlarmMaximum), 2) + " " + TempWeightAbreviation;
		if (vCheckWeighNominalWeight)
			vTextString = vTextString + "\n        Nominal Weight: " + dtoa(vCheckWeighNominalWeight, 2) + " " + TempWeightAbreviation;
		else
			vTextString = vTextString + "\n        Nominal Weight: Not Entered";

		vTextString = vTextString + "\n\n        Number of Recent Data Samples kept: " + dtoa(vCheckWeighDataSize, 2);
		vTextString = vTextString + "\n        Number of Trend Data Samples kept: " + dtoa(vCheckWeighTrendDataSize, 2);
		vTextString = vTextString + "\n        Maximum Number of Data Samples in One Minute: " + dtoa(cCheckWeighMaximumSamplesInOneMinute, 2);
	}
	else
		vTextString = vTextString + "\n\n    Weight Trending Not Setup";

	if (vSetupHistoryMinimumGoodCount)
	{
		vTextString = vTextString + "\n\n    Minimum Good Average Density: " + dtoa(vSetupHistoryMinimumGoodAverage,2);
		vTextString = vTextString + "\n    Minimum Good Density Sigma: " + dtoa(vSetupHistoryMinimumGoodStandardDeviation,2);
		vTextString = vTextString + "\n    Minimum Good Minimum Density: " + dtoa(vSetupHistoryMinimumValue,2);
		vTextString = vTextString + "\n    Minimum Good Maximum Density: " + dtoa(vSetupHistoryMaximumValue,2);
		vTextString = vTextString + "\n    Good Density Count: " + dtoa(vSetupHistoryMinimumGoodCount, 0);
	}
	else
		vTextString = vTextString + "\n\n    Evaluate Minimum Good Not Done";

	if (vSetupHistoryMaximumBadCount)
	{
		vTextString = vTextString + "\n\n    Maximum Good Average Density: " + dtoa(vSetupHistoryMaximumBadAverage,2);
		vTextString = vTextString + "\n    Maximum Good Density Sigma: " + dtoa(vSetupHistoryMaximumBadStandardDeviation,2);
		vTextString = vTextString + "\n    Maximum Good Minimum Density: " + dtoa(vSetupHistoryBadMinimumValue,2);
		vTextString = vTextString + "\n    Maximum Good Maximum Density: " + dtoa(vSetupHistoryBadMaximumValue,2);
		vTextString = vTextString + "\n    Maximum Good Density Count: " + dtoa(vSetupHistoryMaximumBadCount, 0);
	}
	else
		vTextString = vTextString + "\n\n    Evaluate Maximum Good Not Done";

	vTextString = vTextString + "\n\n    Inspection ROI Top: " + dtoa(vROITop,2) + " " + TempUnitsString + ", " + dtoa(vROIPixelTop,0) + " Pixels";
	vTextString = vTextString + "\n    Inspection ROI Bottom: " + dtoa(vROIBottom,2) + " " + TempUnitsString + ", " + dtoa(vROIPixelBottom,0) + " Pixels";
	vTextString = vTextString + "\n    Inspection ROI Left: " + dtoa(vROILeft,2) + " " + TempUnitsString + ", " + dtoa(vROIPixelLeft,0) + " Pixels";
	vTextString = vTextString + "\n    Inspection ROI Right: " + dtoa(vROIRight,2) + " " + TempUnitsString + ", " + dtoa(vROIPixelRight,0) + " Pixels";
	vTextString = vTextString + "\n\n    Ejector: " + TempEjectorNameString;

	return vTextString;
}

void CInspectionCheckWeigh::CreateDataMemory()
{
	vLastWeight = 0;

	vCheckWeighDataIndex = -1;
	vCheckWeighNumberOfDataSamplesTaken = 0;

	vCheckWeighCurrentMinuteNumberOfDataSamplesTaken = 0;

	vCheckWeighTrendDataIndex = -1;
	vCheckWeighTrendNumberOfDataSamplesTaken = 0;

	vCheckWeighTrendDataIndexOnMinutes = -1;
	vCheckWeighTrendNumberOfDataSamplesTakenOnMinutes = 0;

	vCheckWeighNumberOfSamplesTakenForAlarm = 0;
	vTotalUnder = 0;
	vTotalOver = 0;
	
	if (vCheckWeighData)
	{
		free(vCheckWeighData);
		vCheckWeighData = NULL;
	}

	if (ThereIsEnoughMemory(vCheckWeighDataSize * sizeof(double), "Weight Trending Data"))
		vCheckWeighData = (double *)malloc(vCheckWeighDataSize * sizeof(double));

	if (vCheckWeighCurrentMinuteData)
	{
		free(vCheckWeighCurrentMinuteData);
		vCheckWeighCurrentMinuteData = NULL;
	}

	if (ThereIsEnoughMemory(cCheckWeighMaximumSamplesInOneMinute * sizeof(double), "Weight Current Minute Data"))
		vCheckWeighCurrentMinuteData = (double *)malloc(cCheckWeighMaximumSamplesInOneMinute * sizeof(double));

	if (vCheckWeighTrendData)
	{
		free(vCheckWeighTrendData);
		vCheckWeighTrendData = NULL;
	}

	if (ThereIsEnoughMemory(vCheckWeighTrendDataSize * sizeof(double), "Weight Trending Trend Data"))
		vCheckWeighTrendData = (double *)malloc(vCheckWeighTrendDataSize * sizeof(double));

	if (vCheckWeighTrendDataOnMinutes)
	{
		free(vCheckWeighTrendDataOnMinutes);
		vCheckWeighTrendDataOnMinutes = NULL;
	}

	if (ThereIsEnoughMemory(vCheckWeighTrendDataSizeOnMinutes * sizeof(double), "Weight Trending On Minutes Trend Data"))
		vCheckWeighTrendDataOnMinutes = (double *)malloc(vCheckWeighTrendDataSizeOnMinutes * sizeof(double));

	ClearDataMemory(false);
}

void CInspectionCheckWeigh::ClearDataMemory(bool TempHaveRealData)
{
	vNumberOfAlarms = 0;
	vLastWeight = 0;

	vCheckWeighDataIndex = -1;
	vCheckWeighNumberOfDataSamplesTaken = 0;

	vCheckWeighCurrentMinuteNumberOfDataSamplesTaken = 0;

	vCheckWeighTrendDataIndex = -1;
	vCheckWeighTrendNumberOfDataSamplesTaken = 0;

	vCheckWeighTrendDataIndexOnMinutes = 0;
	vCheckWeighTrendNumberOfDataSamplesTakenOnMinutes = 0;

	vCheckWeighNumberOfSamplesTakenForAlarm = 0;
	vCheckWeighAlarm2OverWeightOn = false;
	vCheckWeighAlarm2UnderWeightOn = false;

	if (TempHaveRealData)
	if (vCheckWeighAlarmLatched)
		ReportErrorMessage(vName + " - Trending Alarm Cleared by Resetting Counters", cAction,0);

	vCheckWeighAlarmLatched = false;
	vTotalUnder = 0;
	vTotalOver = 0;
	//vCheckWeighAlarmLatchedOld = false;
	if (vCheckWeighData)
		ZeroMemory(vCheckWeighData,vCheckWeighDataSize * sizeof(double));

	if (vCheckWeighCurrentMinuteData)
		ZeroMemory(vCheckWeighCurrentMinuteData,vCheckWeighDataSize * sizeof(double));

	if (vCheckWeighTrendData)
		ZeroMemory(vCheckWeighTrendData,cCheckWeighMaximumSamplesInOneMinute * sizeof(double));

	if (vCheckWeighTrendDataOnMinutes)
		ZeroMemory(vCheckWeighTrendData,vCheckWeighTrendDataSizeOnMinutes * sizeof(double));
}

double CInspectionCheckWeigh::GetSensitivity(BOOL TempUsePassedThreshold, double TempPassedThreshold)
{
	return 0xFF;
}
