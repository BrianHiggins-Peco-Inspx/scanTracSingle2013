//ScanTrac Side View Source File
// FindParameterHelpDialog.cpp : implementation file
//

#include "stdafx.h"
#include "scantrac.h"
#include "FindParameterHelpDialog.h"
#include "HelpDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern double vGlobalPercentToResizeY;
extern bool vGlobalFPGAVersion10Point0OrHigher;
extern BYTE vGlobalShiftKeyDown;
extern unsigned long vGlobalDialogTitleColor;
extern CScanTracSystemRunningData *vGlobalRunningData;
/////////////////////////////////////////////////////////////////////////////
// CFindParameterHelpDialog dialog


CFindParameterHelpDialog::CFindParameterHelpDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CFindParameterHelpDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFindParameterHelpDialog)
	//}}AFX_DATA_INIT
	vSearchPattern = "";
	vFirstItemShown = 1;
	vNumberOfItems = 0;
	vMouseDownRepeatTimerHandle = 52890;
	vMouseState = 0;		//1 is left mouse button clicked down, 2 is left mouse button transistioned up, 3 is double clicked left mouse button
	vRecievedButtonPress = false;
	vWhiteBrush = ::CreateSolidBrush(cWhite);
	vBlueBrush = ::CreateSolidBrush(cLightBlue);
	vYellowBrush = ::CreateSolidBrush(cYellow);
	for (BYTE TempLoop = 0; TempLoop < 10; TempLoop++)
		vItemShowingOnButton[TempLoop] = 0;
	vLastKey = 0;
	vFirstOpenedFindParameterHelp = true;
	vLastKeyTimes = 0;
	vCursorPosition = 0;
	vLocalConfigurationData = NULL;
	vLocalSystemData = NULL;
	for (WORD TempLoop = 0;TempLoop < cNumberOfKeys1; TempLoop++)
		vLocalKey[TempLoop] = cLocalAlphaKey1[TempLoop];

	vLocalCWndCollection.Add(this);
	vLocalCWndCollection.Add(&m_Function5Button);
	vLocalCWndCollection.Add(&m_Function4Button);
	vLocalCWndCollection.Add(&m_Background);
	vLocalCWndCollection.Add(&m_SubFunction8Button);
	vLocalCWndCollection.Add(&m_SubFunction7Button);
	vLocalCWndCollection.Add(&m_SubFunction6Button);
	vLocalCWndCollection.Add(&m_SubFunction5Button);
	vLocalCWndCollection.Add(&m_SubFunction4Button);
	vLocalCWndCollection.Add(&m_SubFunction3Button);
	vLocalCWndCollection.Add(&m_SubFunction2Button);
	vLocalCWndCollection.Add(&m_SubFunction1Button);
	vLocalCWndCollection.Add(&m_DialogTitleStaticText);
	vLocalCWndCollection.Add(&m_DialogTitleStaticText2);
	vLocalCWndCollection.Add(&m_DialogTitleStaticText3);
	vLocalCWndCollection.Add(&m_Function1Button);
	vLocalCWndCollection.Add(&m_Function2Button);
	vLocalCWndCollection.Add(&m_Function3Button);

	vLocalDisplayCollection.Add(&m_SubFunction8Button);
	vLocalDisplayCollection.Add(&m_SubFunction7Button);
	vLocalDisplayCollection.Add(&m_SubFunction6Button);
	vLocalDisplayCollection.Add(&m_SubFunction5Button);
	vLocalDisplayCollection.Add(&m_SubFunction4Button);
	vLocalDisplayCollection.Add(&m_SubFunction3Button);
	vLocalDisplayCollection.Add(&m_SubFunction2Button);
	vLocalDisplayCollection.Add(&m_SubFunction1Button);
}

void CFindParameterHelpDialog::LoadSearchStrings()
{ 
	bool TempInspxPasswordOK = PasswordOK(cTemporaryInspxPassword,false);
	vNumberOfItems = 0;
	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Accept Client Connections";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Hardware Options > Accept Client Connections"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "AC Power Monitor Voltage Errors / Events Log";
	vText[vNumberOfItems] = "Main Menu > Diagnostics > Voltages > Show AC Voltage Problems";
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "AC Voltage Plot, AC Power Monitor";
	vText[vNumberOfItems] = "Main Menu > Diagnostics > Voltages > Show AC Voltage Plot";
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "AC Power Monitor Enable/Disable Yellow Messages";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > System Setup > Setup Hardware Options > Disable/Enable AC Messges"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Activity Level of Container Triggers, Invert, System";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup External Hardware > Setup Container Triggers > Make Active High/Low\n\nThe Container Trigger should be low when beam not blocked and\nHigh when beam blocked in Oscilloscope Display\nIf backward, toggle Make Active High/Low to other state and check."; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "ADC Offset, Change ADC Offsets, Manually";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup X-Ray Detectors > Change ADC Offset (only for development testing as when hit run finds offsets again"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Add Reject To Learn";
	vText[vNumberOfItems] = "Main Menu (running) > Rejects > Image Analysis (Setup Password) > Add Reject To Learn\n\nNote: There must be a reject showing on the screen";
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Add, Undo Add Reject To Learn";
	vText[vNumberOfItems] = "Main Menu (running) > Rejects > Image Analysis (Setup Password) > Edit While Running >Undo Last Add To Learn\n\nNote: There must be an Add To Learn to Undo";
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Adjust All Areas of Container";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Image Handling > Adjust All Areas of Container"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Adjust Frame Brightness";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Image Handling > Adjust Frame Brightness"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Adjust Temperature Readings";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Temperature Settings > (choice of four temperatures to change) Outside Temp Adjust, Inside Temp Adjust, Surface Temp Adjust, Coolant Temp Adjust"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Adjust X-Ray Source Monitors";
		vText[vNumberOfItems] = "Main Menu > Diagnostics > X-Ray > Clear (Inspx Password) > Source Settings";
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Adjustment, Image Intensity Adjustment";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Image Handling > Adjust Image Intensity"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Adjustment, Optical Container Trigger";
	vText[vNumberOfItems] = "Adjust the pot inside the optical sensor module so the\nYellow and Red LEDs are on when No container blocks the beam, and the\nGreen LED is on when the beam is blocked.\n\nIf the signal is inverted on the Oscilloscope Display, it can be inverted in the system setup:\nMain Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup External Hardware > Setup Container Triggers > Toggle - Make Active High / Make Active Low";
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Advanced Contaminant Inspection Settings";
	if (TempInspxPasswordOK)
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Evaluate (Select Product) (Select Inspection) > Inspection Settings > Minimum Defect Size\n\nor\n\nMain Menu (running) > Rejects > Image Analysis (Setup Password) > Edit While Running > Edit Inspection Settings > Minimum Defect Size\n\nNote: Only on Advanced Contaminant, Void, and Size Inspections. Not available on Density Inspections"; 
	else
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Evaluate (Select Product) (Select Inspection) > Inspection Settings > Minimum Defect Size\n\nor\n\nMain Menu (running) > Rejects > Image Analysis (Setup Password) > Edit While Running > Evaluate (Select Inspection) > Minimum Size\n\nNote: Only on Advanced Contaminant, Void, and Size Inspections. Not available on Density Inspections"; 
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Air Filter Clogged, Maximumum Temperature Delta Inside to Outside Air";
		if( vGlobalFPGAVersion10Point0OrHigher)
			vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Temperatures > In / Out Delta";
		else
			vText[vNumberOfItems] = (CString)"Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Temperatures > Toggle - Has Surface Temperature / Has Outside Temperature\n\n"
				+ (CString)"There are only 3 Temperature Sensors.  You can set up one of them as either a Surface Temperature Sensor or an Outside Air Temperature Sensor.  To configure which you have chosen in software you toggle the button mentioned above.  If you chose an Outside Air Sensor, you can set the Inside / Outside Air Delta Limit, and if the difference between the Outside and Inside Air goes above the Delta Limit, a Yellow Message will appear indicating Clogged Air Filter."; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Air Temperature, Maximumum Delta Inside to Out";
		if(vGlobalFPGAVersion10Point0OrHigher)
			vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Temperatures > In / Out Delta";
		else
			vText[vNumberOfItems] = (CString)"Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Temperatures > Toggle - Has Surface Temperature / Has Outside Temperature\n\n"
				+ (CString)"There are only 3 Temperature Sensors.  You can set up one of them as either a Surface Temperature Sensor or an Outside Air Temperature Sensor.  To configure which you have chosen in software you toggle the button mentioned above.  If you chose an Outside Air Sensor, you can set the Inside / Outside Air Delta Limit, and if the difference between the Outside and Inside Air goes above the Delta Limit, a Yellow Message will appear indicating Clogged Air Filter."; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Alarm Test";
		vText[vNumberOfItems] = "Main Menu > Diagnostics > Scope > Clear (Inspx Password) > Test Alarm Output";
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Alarm X of Y";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Edit (Select Product) > Inspections > Set X of Y Alarm1 or Set X of Y Alarm2\n\nor\n\nMain Menu (running) > Setup (Setup Password) > Edit While Running > Disable Ejectors > Set X of Y Alarm1 or Set X of Y Alarm2";
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Align X-Ray Tube";
	vText[vNumberOfItems] = "Main Menu > Diagnostics > X-Ray > Align X-Ray Tube (Certified Setup Password)"; 
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Auto Delete Old Log Files";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > System Setup > Setup System Options > Enable (or Disable) Auto Delete Logs, set which files to delete"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Auto Learn, Start Auto Learn";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Edit (Select Product) > Learn > Start Auto Learn\n\nor\n\nMain Menu (running) > Setup (Setup Password) > Edit While Running > Disable Ejectors > Back To Learn > Start Auto Learn\n\nor\n\nMain Menu > Rejects > Image Analysis (Setup Password) > Edit While Running > Re-Learn Container Image";
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Auto Off, X-Ray Auto Off, Enable or Disable";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Hardware Options > Enable (or Disable) Auto X-Ray Off"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Auto Size This Container";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Edit (Select Product) > Size > Auto Size This Container\n\nNote:\n\nAuto Size This Container cannot be perfomed in Edit While Running";
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Auxiliary Detectors, Product Setup";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Edit (Select Product) > Inspections > Setup Auxiliary Detectors\n\nor\n\nMain Menu (running) > Setup (Setup Password) > Edit While Running > Disable Ejectors > Setup Auxiliary Detectors";
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Auxiliary Detectors, System Setup";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup External Hardware > Setup Auxiliary Detectors"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Background Removal From Image";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Image Handling > Remove Background"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Backup Directory Name";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup System Options > Backup Directory Name"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Bad Evaluate Contaminant or Foreign Material Inspection";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Evaluate (Select Product) (Select Inspection) > Send Bad Containers\n\nor\n\nMain Menu (running) > Setup (Setup Password) > Evaluate > Evaluate (Select Inspection) > Send Bad Containers\n\nor\n\nMain Menu (running) > Rejects > Image Analysis (Setup Password) > Edit While Running Setup > Evaluate Inspection > Send Bad Containers"; 
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Bad Evaluate Density Inspection";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Evaluate (Select Product) (Select Inspection) > Send Bad Containers\n\nor\n\nMain Menu (running) > Setup (Setup Password) > Evaluate > Evaluate (Select Inspection) > Send Bad Containers\n\nor\n\nMain Menu (running) > Rejects > Image Analysis (Setup Password) > Edit While Running Setup > Evaluate Inspection > Send Bad Containers"; 
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Bad Evaluate Size Inspection";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Evaluate (Select Product) (Select Inspection) > Send Bad Containers\n\nor\n\nMain Menu (running) > Setup (Setup Password) > Evaluate > Evaluate (Select Inspection) > Send Bad Containers\n\nor\n\nMain Menu (running) > Rejects > Image Analysis (Setup Password) > Edit While Running Setup > Evaluate Inspection > Send Bad Containers"; 
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Bad Evaluate Void Inspection";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Evaluate (Select Product) (Select Inspection) > Send Bad Containers\n\nor\n\nMain Menu (running) > Setup (Setup Password) > Evaluate > Evaluate (Select Inspection) > Send Bad Containers\n\nor\n\nMain Menu (running) > Rejects > Image Analysis (Setup Password) > Edit While Running Setup > Evaluate Inspection > Send Bad Containers"; 
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Bad Pixel, Copy Pixel to Overwrite Bad Pixel";
		vText[vNumberOfItems] = "Main Menu > Setup (Inspx Password) > Setup System > Setup X-Ray Detectors > Setup Pixel Adjustments > Select the Pixel to Overwrite > Save and Exit to Multiplicative Factor > Save and Exit to Additive Factor > Enter the Pixel Number to Copy On Top of This One.\n\nNormally copy the closest good pixel.  May overwrite multiple pixels."; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Bad Pixels, Pixel Values";
	vText[vNumberOfItems] = "First Measure the Source Strength, then when viewing the Source Strength History shown at the end, press Show Pixel Values.\n\n Or if you have taken a Measure Source Strength since the last power down, Main Menu > Show Production Summary > Product Setup Summary > X-Ray Source Strength > Show Pixel Values";
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Backup Directory Name";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Options > Directory Names > Backup Directory Name"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Belt, Maximum Conveyor Speed";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Edit (Select Product) > Exposure > Maximum Conveyor Speed\n\nNote: this can not be changed while inspecting";
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Blocked, Number of Pixels Blocked By Conveyor";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup X-Ray Detectors > Setup Pixel Adjustments > Press middle bottom button to cycle through options until it asks for how many detectors to calibrate and how many pixels are being blocked by the conveyor"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Bottom of Product Image";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Edit (Select Product) > Size > Bottom\n\nNote:\n\nBottom cannot be changed in Edit While Running";
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Bounds, Container Threshold";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Edit (Select Product) > Size > Container Edge Finding (Inspx Password) > Container Threshold Add or Subtract 1 or 10 to adjust threshold\n\nor\n\nMain Menu (running) > Setup (Setup Password) > Edit While Running > Disable Ejectors > Jump To Start > Container Edge Finding (Inspx Password) > Container Threshold Add or Subtract 1 or 10 to adjust threshold\n\nNote: the software automatically sets this value each time the size, exposure, or purple line is changed, so the only way to manally set it, is to set it after changing all of thoses settings.";
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Bounds, Product Density ROI";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Edit (Select Product) > Exposure > Product Density ROI\n\nor\n\nMain Menu (running) > Setup (Setup Password) > Edit While Running > Disable Ejectors > Back To Learn > Back To Exposure > Product Density ROI";
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Bounds, ROI Bounds (for an Inspection)";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Edit (Select Product) > Inspection > Next To ROI (Select Inspection) > Use arrow keys to move ROI Boundaries\n\nor\n\nMain Menu (running) > Setup (Setup Password) > Edit While Running > Disable Ejectors > Next To ROI (Select Inspection) > Use arrow keys to move ROI Boundaries\n\nor\n\nMain Menu (running) > Rejects > Image Analysis (Setup Password) > Edit While Running > Change ROI Bounds > Use arrow keys to move ROI Boundaries";
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Box Rejects, Reject Display Control";
		vText[vNumberOfItems] = (CString)"Main Menu Reject Display Mode:\nMain Menu > Setup (Setup Password) > Clear (Inspx Password) > System Setup > Setup System Options > Toggle - Defect In Red Box / Defects In Red / Pseudo Color / Up To 10 Red Boxes / No Defect Indicator\n\n"
			+ (CString)"Reject Menu Reject Display Mode:\nMain Menu > (No) Rejects > Clear (Inspx Password) > Inspx Hidden Key (Shift) > Toggle - Defect In Red Box / Defects In Red Boxes / Defects In Red / Defects In Color"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Brightness, Adjust Frame Brightness";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Image Handling > Adjust Frame Brightness"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Brightness of Image";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Edit (Select Product) > Exposure\n\nor\n\nNote: this can not be changed while inspecting";
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Bulk Mode, No Container Trigger on Conveyor";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup External Hardware > Setup System Conveyor > Use Bulk Mode or Use Real Trigger"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Burn In X-Ray Tube";
	vText[vNumberOfItems] = "Main Menu > Diagnostics > X-Ray > Inspx Hidden Key (Shift) > Burn in Xray Tube";
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Calculate Continuous Feed Containers";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > System Setup > Setup External Hardware > Setup Conveyor > Calculate Continuous Feed Containers"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Calibrate Conveyor, Encoder Pulses Per Inch";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > System Setup > Setup External Hardware > Setup Conveyor > Encoder Pulses Per Inch"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Calibrate Offset While Running, Time Before Re-calibrate";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Radiation Shutter > Time Before Re-calibrate"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Calibrate Conveyor, System Setup";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup External Hardware > Setup Conveyor"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Calibrate Detectors";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Calibrate Detectors (Certified Setup Password)";
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Calibrate Detectors - How to Exclude Conveyor Calibration";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup X-Ray Detectors > Setup Pixel Adjustments > Correct No Pixels > Correct Edge Pixels > Correct All Pixels > Enter the number of detectors to Calibrate, then enter the number of pixels at the bottom to exclude if the conveyor is showing";
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Calibrate Detectors - How to Exclude Some Detectors From Calibration";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup X-Ray Detectors > Setup Pixel Adjustments > Correct No Pixels > Correct Edge Pixels > Correct All Pixels > Enter the number of detectors to Calibrate, then enter the number of pixels at the bottom to exclude if the conveyor is showing";
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Calibrate Encoder";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup External Hardware > Setup Conveyor > Calibrate Encoder";
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Calibrate Pixels or Adjust, Manual Setting";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup X-Ray Detectors > Setup Pixel Adjustments";
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Calibrate Pixels or Adjust, Automatically Find";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup X-Ray Detectors > Calibrate Detector Pixels";
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Calibrate X-Ray Detector A/D Offset";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup X-Ray Detectors > Auto Find Offset"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Calibrate X-Ray Detector Edges";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup X-Ray Detectors > Auto Calibrate Pixels"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Calibrate X-Ray Detectors, Complete Calibration";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Calibrate Detectors (Certified Setup Password)";
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Calibrate X-Ray Detector Gains, Equalize Gains";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup X-Ray Detectors > Auto Find Detector Gains"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Calibrate X-Ray Source Monitors";
		vText[vNumberOfItems] = "Main Menu > Diagnostics > X-Ray > Clear (Inspx Password) > Source Settings";
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Can Stop On/Off Test";
		vText[vNumberOfItems] = "Main Menu > Diagnostics > Scope > Clear (Inspx Password) > Can Stop 1 (or 2) On (or Off)";
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Capture Screen";
	vText[vNumberOfItems] = "Any Menu (except data entry menus)> 0 > 1\nAny Menu > Inspx Hidden Key (Shift) > 1"; 
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Chat";
	vText[vNumberOfItems] = "From ScanTrac Console:\nHelp (Located On Keypad) > Chat\n\nFrom Remote Connection:\nAny Menu > ESC (On Any Standard Keyboard)"; 
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Check Disk, Scan Hard Drive For Errors";
	vText[vNumberOfItems] = "Main Menu > Diagnostics > Comm. > Hard Drive Check Hard Disk"; 
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Check for more Memory or RAM, Have PC Check Amount of RAM";
		vText[vNumberOfItems] = "Main Menu > Diagnostics > Com (Setup Password) > Clear (Inspx Password) > uController Diagnostics > Memory Size Changed! > Reboot Now"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Check Weighing";
	vText[vNumberOfItems] = "Main Menu > Show Production Summary > Weight Trending"; 
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Check Weighing, Weight Offset";
	vText[vNumberOfItems] = "Main Menu > Show Production Summary > Weight Trending > Show Recent > Weight Offset"; 
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Clients, Accept Client Connections";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Hardware Options > Accept Client Connections"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Clock, Pixel Clock Divider";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup X-Ray Detectors > Pixel Clock Divider"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Clogged Filter, Maximumum Temperature Delta Inside to Out";
		if(vGlobalFPGAVersion10Point0OrHigher)
			vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Temperatures > In / Out Delta";
		else
			vText[vNumberOfItems] = (CString)"Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Temperatures > Toggle - Has Surface Temperature / Has Outside Temperature\n\n"
				+ (CString)"There are only 3 Temperature Sensors.  You can set up one of them as either a Surface Temperature Sensor or an Outside Air Temperature Sensor.  To configure which you have chosen in software you toggle the button mentioned above.  If you chose an Outside Air Sensor, you can set the Inside / Outside Air Delta Limit, and if the difference between the Outside and Inside Air goes above the Delta Limit, a Yellow Message will appear indicating Clogged Air Filter."; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Code, Product Code";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Options > Use Product Code";
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Color, Reject Display Control";
		vText[vNumberOfItems] = (CString)"Main Menu Reject Display Mode:\nMain Menu > Setup (Setup Password) > Clear (Inspx Password) > System Setup > Setup System Options > Toggle - Defect In Red Box / Defects In Red / Pseudo Color / Up To 10 Red Boxes / No Defect Indicator\n\n"
			+ (CString)"Reject Menu Reject Display Mode:\nMain Menu > (No) Rejects > Clear (Inspx Password) > Inspx Hidden Key (Shift) > Toggle - Defect In Red Box / Defects In Red Boxes / Defects In Red / Defects In Color"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Com Port for Micro Controller";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Hardware Options > Setup uController Com Port"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Com Ports";
	vText[vNumberOfItems] = "Main Menu > Diagnostics > Comm."; 
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Commission ScanTrac";
		vText[vNumberOfItems] = "Main Menu > Diagnostics > Counters (Setup Password) > Clear (Inspx Password) > Commission Date"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Communications Menu, Serial Device Communications Menu";
	vText[vNumberOfItems] = "Main Menu > Diagnostics > Comm."; 
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Computer, Shut Down Computer";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Press Hidden Key > Shut Down Computer"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Condition X-Ray Tube";
	vText[vNumberOfItems] = "Main Menu > Diagnostics > X-Ray > Condition X-Ray Tube";
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Confirm, Stop On Unconfirmed Eject";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup System Options > Stop On Unconfirmed Eject"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Container Bounds Threshold";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Edit (Select Product) > Size > Container Edge Finding (Inspx Password) > Container Threshold Add or Subtract 1 or 10 to adjust threshold\n\nor\n\nMain Menu (running) > Setup (Setup Password) > Edit While Running > Disable Ejectors > Jump To Start > Container Edge Finding (Inspx Password) > Container Threshold Add or Subtract 1 or 10 to adjust threshold\n\nNote: the software automatically sets this value each time the size, exposure, or purple line is changed, so the only way to manally set it, is to set it after changing all of thoses settings.";
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Container, Continuous Feed Containers";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > System Setup > Setup External Hardware > Setup System Conveyor > Continuous Feed Containers"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Container Edge Finding, Edge Location Area, Edge Finding, Purple Line";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Edit (Select Product) > Size > Container Edge Finding\n\nor\n\nMain Menu (running) > Setup (Setup Password) > Edit While Running > Disable Ejectors > Jump To Start > Container Edge Finding\n\nUse the Up, Down, Longer, and Shorter Buttons to adjust the Purple Line on the left side of the image to the location across from the container to look for the edges of the container.\n\nThis must be set manually\n\nAdjust Purple Line so ScanTrac finds the edges correctly of every image.";
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Container Goes Left to Right/Right to Left"; //dual beam only
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > (Inspx Password) > System Setup > Setup External Hardware > Setup Conveyor > Container Goes Left to Right/Right to Left\n\nNote: Must be set manually so ScanTrac knows which image appears first";
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Container, Simulate";
		vText[vNumberOfItems] = "Main Menu > Diagnostics > Clear (Inspx Password) > Test Modes Menu > Simulated Container Rate"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Containers Per Minute Time Period";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Options > CPM Time Period";
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Container Trigger Adjustment";
	vText[vNumberOfItems] = "Adjust the pot inside the optical sensor module so the\nYellow and Red LEDs are on when No container blocks the beam, and the\nGreen LED is on when the beam is blocked.";
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Container Triggers, Invert, System";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup External Hardware > Setup Container Triggers > Make Active High/Low\n\nThe Container Trigger should be low when beam not blocked and\nHigh when beam blocked in Oscilloscope Display\nIf backward, toggle Make Active High/Low to other state and check."; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Container Trigger Lockout";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Edit (Select Product) > Size > Container Retrigger Lockout\n\nor\n\nMain Menu (running) > Setup (Setup Password) > Edit While Running > Disable Ejectors > Jump To Start > Container Retrigger Lockout\n\nNote:\n\nNote: Auto Size This Container set the Container Trigger Lockout to the correct value so you should not need to set it here manually";
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Container Trigger To Detector Distance";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Edit (Select Product) > Size > Container Trigger To Detector\n\nor\n\nMain Menu (running) > Setup (Setup Password) > Edit While Running > Disable Ejectors > Jump To Start > Container Trigger To Detector";
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Container Triggers, System";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup External Hardware > Setup Container Triggers"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Container Trigger to Image Delay"; 
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Edit (Select Product) > Size > Container Trigger to Image Delay\n\nNote:\n\nNote: Auto Size This Container set the Container Trigger to Image Delay to the correct value so you should not need to set it here manually";
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Container Trigger Warning";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > System Setup > Setup External Hardware > Maximum Trigger Std. Dev"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Contaminant or Foreign Material Inspection, Evaluate";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Evaluate (Select Product) (Select Inspection) > Send Good Containers\n\nor\n\nMain Menu (running) > Setup (Setup Password) > Evaluate > Evaluate (Select Inspection) > Send Good Containers"; 
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Contaminant or Foreign Material Inspection, Minimum Defect Size";
	if (TempInspxPasswordOK)
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Evaluate (Select Product) (Select Inspection) > Inspection Settings > Minimum Defect Size\n\nor\n\nMain Menu (running) > Rejects > Image Analysis (Setup Password) > Edit While Running > Edit Inspection Settings > Minimum Defect Size\n\nNote: Only on Contaminant, Foreign Material, Void, and Size Inspections. Not available on Density Inspections"; 
	else
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Evaluate (Select Product) (Select Inspection) > Inspection Settings > Minimum Defect Size\n\nor\n\nMain Menu (running) > Rejects > Image Analysis (Setup Password) > Edit While Running > Evaluate (Select Inspection) > Minimum Size\n\nNote: Only on Contaminant, Foreign Material, Void, and Size Inspections. Not available on Density Inspections"; 
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Contaminant or Foreign Material Inspection Threshold";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Evaluate (Select Product) (Select Inspection) > +10 or +1 or -1 or -10\n\nor\n\nMain Menu (running) > Rejects > Image Analysis (Setup Password) > Edit While Running > Change Reject Threshold > +10 or +1 or -1 or -10"; 
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Continuous Feed Containers";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > System Setup > Setup External Hardware > Setup System Conveyor > Continuous Feed Containers"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Continuous Feed Encoder";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > System Setup > Setup External Hardware > Setup System Conveyor > Continuous Feed Encoder"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Convert ScanTrac System Type (Piccolo, Solo, Legato, 600 Watt Pipeline)";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup Hardware Options > Toggle - Convert to (Piccolo / Solo / Legato / 600 Watt Pipeline) ScanTrac"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Conveyor Stops";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Shape Handling > Inspect/Pass/Reject On Conveyor Stops"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Conveyor, Maximum Conveyor Speed";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Edit (Select Product) > Exposure > Maximum Conveyor Speed\n\nor\n\nNote: this can not be changed while inspecting";
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Conveyor, Number of Pixels Blocked By Conveyor";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup X-Ray Detectors > Setup Pixel Adjustments > Press middle bottom button to cycle through options until it asks for how many detectors to calibrate and how many pixels are being blocked by the conveyor"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Conveyor, Simulate";
		vText[vNumberOfItems] = "Main Menu > Diagnostics > Clear (Inspx Password) > Test Modes Menu > Simulated Encoder Rate"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Conveyor, Stop Inspecting When Conveyor Stops";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup System Options > Enable (or Disable) Conveyor Stop"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Conveyor, System Setup";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup External Hardware > Setup Conveyor"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Conveyor Speed Correction for Ejectors";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Edit (Select Product) > Inspections > Setup Ejector Timing > Ejector Timing Factor\n\nor\n\nMain Menu (running) > Setup (Setup Password) > Edit While Running > Disable Ejectors > Setup Ejector Timing > Conveyor Speed Correction";
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Conveyor Test";
		vText[vNumberOfItems] = "Main Menu > Diagnostics > Scope > Clear (Inspx Password) > Start (or Stop) Conveyor Now";
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Coolant Liquid Temperature, Maximum, Adjust, Delta";
		vText[vNumberOfItems] = (CString)"Adjust:\nMain Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Temperatures > Coolant Temp Adjust\n\n"
			+ (CString)"Maximum:\nMain Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Temperatures > Maximum Coolant Temp\n\n" 
			+ (CString)"Inside Air / Coolant Liquid Delta:\nSearch for 'Delta' in Command Finder"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Coolant Liquid Temperature, Current Values";
	vText[vNumberOfItems] = (CString)"Main Menu > Diagnostics >	X-Ray";
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Copy Pixel to Overwrite Bad Pixel";
		vText[vNumberOfItems] = "Main Menu > Setup (Inspx Password) > Setup System > Setup X-Ray Detectors > Setup Pixel Adjustments > Select the Pixel to Overwrite > Save and Exit to Multiplicative Factor > Save and Exit to Additive Factor > Enter the Pixel Number to Copy On Top of This One.\n\nNormally copy the closest good pixel.  May overwrite multiple pixels."; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Correction, Conveyor Speed Correction for Ejectors";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Edit (Select Product) > Inspections > Setup Ejector Timing > Ejector Timing Factor\n\nor\n\nMain Menu (running) > Setup (Setup Password) > Edit While Running > Disable Ejectors > Setup Ejector Timing > Conveyor Speed Correction";
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Correction, Density Correction";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Image Handling > Density Correction"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Correction, Image Intensity";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Image Handling > Adjust Image Intensity"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Counters, Life Time or Warranty Counters, Reset";
		vText[vNumberOfItems] = "Main Menu > Diagnostics > Counters (Setup Password) > Clear (Inspx Password) > Appropriate Reset Buttons (Requires Special Reset Password)"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Counters Reset Password Setup";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Options > Counters Password"; 
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "CPM, Containers Per Minute Time Period";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Options > CPM Time Period";
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Current, X-Ray Source Current";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Edit (Select Product) > Exposure > X-Ray Source Current\n\nor\n\nNote: this can not be changed while inspecting";
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Current, Maximum X-Ray Source Voltage";
		vText[vNumberOfItems] = "Main Menu > Diagnostics > X-Ray > Clear (Inspx Password) > Source Settings > Maximum Voltage/Current";
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Cursor, Mouse Cursor Show or Hide";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Hardware Options > Turn On (or Off) Mouse Cursor"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Customer Name";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Options > Customer and Line Name";
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "DAC Offset, Change DAC Offsets, Manually";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup X-Ray Detectors > Change DAC Offset (only for development testing as when hit run finds offsets again"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Dark Current, Measure";
		vText[vNumberOfItems] = "Main Menu > Diagnostics > X-Ray > Clear (Inspx Password) > Measure Dark Current"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Date";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Options > Set Date"; 
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Defect, Display Settings, Reject Display Control";
		vText[vNumberOfItems] = (CString)"Main Menu Reject Display Mode:\nMain Menu > Setup (Setup Password) > Clear (Inspx Password) > System Setup > Setup System Options > Toggle - Defect In Red Box / Defects In Red / Pseudo Color / Up To 10 Red Boxes / No Defect Indicator\n\n"
			+ (CString)"Reject Menu Reject Display Mode:\nMain Menu > (No) Rejects > Clear (Inspx Password) > Inspx Hidden Key (Shift) > Toggle - Defect In Red Box / Defects In Red Boxes / Defects In Red / Defects In Color"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Defect, Minimum Defect Size";
	if (TempInspxPasswordOK)
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Evaluate (Select Product) (Select Inspection) > Inspection Settings > Minimum Defect Size\n\nor\n\nMain Menu (running) > Rejects > Image Analysis (Setup Password) > Edit While Running > Edit Inspection Settings > Minimum Defect Size\n\nNote: Only on Contaminant, Foreign Material, Void, and Size Inspections. Not available on Density Inspections"; 
	else
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Evaluate (Select Product) (Select Inspection) > Inspection Settings > Minimum Defect Size\n\nor\n\nMain Menu (running) > Rejects > Image Analysis (Setup Password) > Edit While Running > Evaluate (Select Inspection) > Minimum Size\n\nNote: Only on Contaminant, Foreign Material, Void, and Size Inspections. Not available on Density Inspections"; 
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Defect -> Search for Reject"; 
	vText[vNumberOfItems] = "Defect -> Search for Reject";
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Delay, Container Trigger to Image Delay"; 
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Edit (Select Product) > Size > Container Trigger to Image Delay\n\nNote:\n\nNote: Auto Size This Container set the Container Trigger to Image Delay to the correct value so you should not need to set it here manually";
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Delete Files From Hard Disk";
		vText[vNumberOfItems] = "Main Menu > Diagnostics > Comm. (Inspx Password)> Delete Old Log Files From Disk and Delete Old Image Files"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Delete Image Files";
		vText[vNumberOfItems] = "Main Menu > Diagnostics > Comm. (Inspx Password) > Delete Old Image Files";
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Delete Inspection";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Edit (Select Product) > Inspection > Delete\n\nor\n\nMain Menu (running) > Setup (Setup Password) > Edit While Running > Disable Ejectors > Delete";
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Delete Log Files Automatically, Setup File Type and Age to Auto Delete";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > System Setup > Setup System Options > Auto Delete Logs";
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Delete Old Log Files Automatically";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > System Setup > Setup System Options > Enable (or Disable) Auto Delete Logs, set which files to delete"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Delta Temperature, Maximumum Inside to Coolant Delta";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Temperatures > In / Coolant Delta"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Delta Temperature, Maximumum Inside to Out Delta";
		if(vGlobalFPGAVersion10Point0OrHigher)
			vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Temperatures > In / Out Delta";
		else
			vText[vNumberOfItems] = (CString)"Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Temperatures > Toggle - Has Surface Temperature / Has Outside Temperature\n\n"
				+ (CString)"There are only 3 Temperature Sensors.  You can set up one of them as either a Surface Temperature Sensor or an Outside Air Temperature Sensor.  To configure which you have chosen in software you toggle the button mentioned above.  If you chose an Outside Air Sensor, you can set the Inside / Outside Air Delta Limit, and if the difference between the Outside and Inside Air goes above the Delta Limit, a Yellow Message will appear indicating Clogged Air Filter."; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Demo Mode, Enable or Disable";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Hardware Options > Enable (or Disable) Demo Mode"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Density Correction";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Image Handling > Density Correction"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Density, Evaluate Density Inspection";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Evaluate (Select Product) (Select Inspection) > Send Good Containers\n\nor\n\nMain Menu (running) > Setup (Setup Password) > Evaluate > Evaluate (Select Inspection) > Send Good Containers"; 
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Density Inspection Threshold";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Evaluate (Select Product) (Select Inspection) > Change Threshold on Missing Lid, Underfill and Overfill. Change Upper Threshold or Change Lower Threshold on Density Inspections\n\nor\n\nMain Menu (running) > Rejects > Image Analysis (Setup Password) > Edit While Running > Change Reject Threshold > +10 or +1 or -1 or -10\n\nNote: On Density Inspection pick Upper or Lower Threshold"; 
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Density, Product Density ROI";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Edit (Select Product) > Exposure > Product Density ROI\n\nor\n\nMain Menu (running) > Setup (Setup Password) > Edit While Running > Disable Ejectors > Back To Learn > Back To Exposure > Product Density ROI";
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Detector, Auxiliary";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Edit (Select Product) > Inspections > Setup Auxiliary Detectors\n\nor\n\nMain Menu (running) > Setup (Setup Password) > Edit While Running > Disable Ejectors > Setup Auxiliary Detectors";
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Detectors, Complete Calibration, Calibrate X-Ray Detectors";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Calibrate Detectors (Certified Setup Password)";
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Detector, Container Trigger To Detector Distance";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Edit (Select Product) > Size > Container Trigger To Detector\n\nor\n\nMain Menu (running) > Setup (Setup Password) > Edit While Running > Disable Ejectors > Jump To Start > Container Trigger To Detector";
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Detector Edges, Show Detector Edges";
		vText[vNumberOfItems] = "Main Menu > Rejects > Image Analysis (Setup Password) > Image Display Options > Clear (Inspx Password) > Show Bottom (Top, or Both, or Center, or Hide) Det. Edges"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Detector Reset Life Time or Warranty Counters";
		vText[vNumberOfItems] = "Main Menu > Diagnostics > Counters (Setup Password) > Clear (Inspx Password) > Appropriate Reset Buttons (Requires Special Reset Password)"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Detectors, System Auxiliary Detectors";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup External Hardware > Setup Auxiliary Detectors"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Detectors, System Container Triggers";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup External Hardware > Setup Container Triggers"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Detectors, System X-Ray Detectors";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup X-Ray Detectors\n\nNote:\n\nSet also X-Ray Detectors"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Diagnostic Test Modes";
		vText[vNumberOfItems] = "Main Menu > Diagnostics > Clear (Inspx Password) > Test Modes Menu"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Directory Names";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Options > Directory Names > press appropriate directory name"; 
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Directory, Screen Shot Directory Name";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Options > Directory Names > Screen Shot Directory"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Discover Mode, Auxiliary Detector";
		vText[vNumberOfItems] = "Main Menu > Setup > Edit > Current Product > Setup Auxiliary Detectors > Discover Mode"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Divert Uninspected Containers";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup System Options > Divert Uninspected Containers"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Diverter, Diverter Margin";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup System Options > Open/Close Diverter Margin"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Diverter, Ejector Output For Diverter";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup System Options > Ejector Output For Diverter"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Divider, Encoder Divider Factor";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > System Setup > Setup External Hardware > Setup System Conveyor > Encoder Divider Factor"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Divider, Pixel Clock Divider";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup X-Ray Detectors > Pixel Clock Divider"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Down Stream (Up Stream / Down Stream Interlock Signals)";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup External Hardware > Use Down Stream or Don't Use Down Stream"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Duration, Life Time or Warranty Counters, Reset";
		vText[vNumberOfItems] = "Main Menu > Diagnostics > Counters (Setup Password) > Clear (Inspx Password) > Appropriate Reset Buttons (Requires Special Reset Password)"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Dwell, Ejector Dwell";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Edit (Select Product) > Inspections > Setup Ejector Timing > Ejector Dwell Time\n\nor\n\nMain Menu (running) > Setup (Setup Password) > Edit While Running > Disable Ejectors > Setup Ejector Timing > Ejector Dwell Time";
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "DXM, Use Spellman DXM HVPS";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Hardware Options > Use DXM HVPS"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "E-Mail Addresses, System";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup E-Mail Addresses"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Edge Adjust, X-Ray Detector Edge Adjust";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup X-Ray Detectors > Edge Adjust"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Edge Calibrate, X-Ray Detector Edge Calibrate";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup X-Ray Detectors > Auto Calibrate Pixels"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Edge Location Area, Container Edge Finding, Edge Finding, Purple Line";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Edit (Select Product) > Size > Container Edge Finding\n\nor\n\nMain Menu (running) > Setup (Setup Password) > Edit While Running > Disable Ejectors > Jump To Start > Container Edge Finding\n\nUse the Up, Down, Longer, and Shorter Buttons to adjust the Purple Line on the left side of the image to the location across from the container to look for the edges of the container.\n\nThis must be set manually\n\nAdjust Purple Line so ScanTrac finds the edges correctly of every image.";
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Edge, Show Detector Edges";
		vText[vNumberOfItems] = "Main Menu > Rejects > Image Analysis (Setup Password) > Image Display Options > Clear (Inspx Password) > Show Bottom (Top, or Both, or Center, or Hide) Det. Edges"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Edges, Reference Inspections To Edges";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Image Handling > Adjust ROI Size To Edges"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Edit Inspection ROI";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Edit (Select Product) > Inspection > Edit\n\nor\n\nMain Menu (running) > Setup (Setup Password) > Edit While Running > Disable Ejectors > Next To ROI";
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Eject Containers at Edge, Top, Wrong Width, Height";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Shape Handling > Pass (or Eject, or Inspect Containers at Edges"; 
		vHidden[vNumberOfItems] = true;
	}
	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Eject Empty Containers that appear Slipped";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Shape Handling > Eject/Pass Empty Containers"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Eject, Stop On Unconfirmed Eject";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup System Options > Stop On Unconfirmed Eject"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Ejector Dwell";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Edit (Select Product) > Inspections > Setup Ejector Timing > Ejector Dwell Time\n\nor\n\nMain Menu (running) > Setup (Setup Password) > Edit While Running > Disable Ejectors > Setup Ejector Timing > Ejector Dwell Time";
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Ejector Position";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Edit (Select Product) > Inspections > Setup Ejector Timing > Position\n\nor\n\nMain Menu (running) > Setup (Setup Password) > Edit While Running > Disable Ejectors > Setup Ejector Timing > Position";
	vHidden[vNumberOfItems] = false;

//	if (TempInspxPasswordOK) //removed from menu
//	{
//		vNumberOfItems++;
//		vItem[vNumberOfItems] = "Ejector Sensor Connected";
//		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup System Options > Ejector Sensor Connected"; 
//		vHidden[vNumberOfItems] = true;
//	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Ejector Test On/Off";
		vText[vNumberOfItems] = "Main Menu > Diagnostics > Scope > Clear (Inspx Password) > Ejector 1 (or 2 or 3) On (or Off)";
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Ejector Test Pulsed";
		vText[vNumberOfItems] = "Main Menu > Diagnostics > Clear (Inspx Password)> Test Modes Menu > Test Fire Ejector Now (Test Fire Ejector Setup)\n\nor\n\nMain Menu (running) > Diagnostics > Clear (Inspx Password) > Test Modes Menu > Eject Next Container";
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Ejector, Test Fire Now!";
	vText[vNumberOfItems] = "Main Menu > Diagnostics > Test Fire Ejector Now";
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Ejector Test With Product";
	vText[vNumberOfItems] = "Main Menu > Setup > Edit (Select Product) > Inspections > Setup Ejector Timing > Test Eject Next Container or Test Eject All Containers";
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Ejector Timing";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Edit (Select Product) > Inspections > Setup Ejector Timing\n\nor\n\nMain Menu (running) > Setup (Setup Password) > Edit While Running > Disable Ejectors > Setup Ejector Timing";
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Ejector Timing Factor, Conveyor Speed Correction";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Edit (Select Product) > Inspections > Setup Ejector Timing > Ejector Timing Factor\n\nor\n\nMain Menu (running) > Setup (Setup Password) > Edit While Running > Disable Ejectors > Setup Ejector Timing > Conveyor Speed Correction";
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Ejectors Re-enable";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup System Options > Re-enable Ejectors"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Ejectors, Select Ejectors (for an Inspection)";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Edit (Select Product) > Inspection > Next To ROI (Select Inspection) > Select Ejectors\n\nor\n\nMain Menu (running) > Setup (Setup Password) > Edit While Running > Disable Ejectors > Next To ROI (Select Inspection) > Select Ejectors";
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Ejectors, System";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup External Hardware > Setup System Ejectors"; 
		vHidden[vNumberOfItems] = true;
	}
	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Empty, Eject Empty Containers that appear Slipped";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Shape Handling > Eject/Pass Empty Containers"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "EMail Addresses, System";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup E-Mail Addresses"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Enable Quick or Normal Learn";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Edit (Select Product) > Learn > Enable Quick/Normal Learn\n\nor\n\nMain Menu (running) > Setup (Setup Password) > Edit While Running > Disable Ejectors > Back To Learn > Enable Quick/Normal Learn";
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Encoder, Calibrate Encoder";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup External Hardware > Setup Conveyor > Calibrate Encoder";
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Encoder, Continuous Feed Encoder";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > System Setup > Setup External Hardware > Setup System Conveyor > Continuous Feed Encoder"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Encoder, Don't correct images if encoder pulses too close";
		vText[vNumberOfItems] = "Main Menu > Diagnostics > Clear (Inspx Password) > Comm. > uController Diagnostics > Encoder Correction"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Encoder Rate, Simulate Conveyor";
		vText[vNumberOfItems] = "Main Menu > Diagnostics > Clear (Inspx Password) > Test Modes Menu > Simulated Encoder Rate"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Encoder Data, Data Plot for Encoder Diagnostics";
	vText[vNumberOfItems] = "Main Menu > Diagnostics > Encoder Data";
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Encoder Divider Factor";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > System Setup > Setup External Hardware > Setup Conveyor > Encoder Divider Factor"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Encoder Pulses Per Inch";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > System Setup > Setup External Hardware > Setup Conveyor > Encoder Pulses Per Inch"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Encoder, Simulate";
		vText[vNumberOfItems] = "Main Menu > Diagnostics > Clear (Inspx Password) > Test Modes Menu > Simulated Encoder Rate"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Encoder Tolerance Before Generate Yellow Message";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > System Setup > Setup External Hardware > Setup System Conveyor > Encoder Tolerance"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Encoder, Use VCO Encoder, (like Bulk Mode but for Pipelines";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup External Hardware > Setup System Conveyor > Use VCO to Encoder or Use Continuous Feed"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "English Language (English, French, Spanish, Polish)";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup  Hardware Options > Language"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Equalize Gains, Calibrate X-Ray Detector Gains";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup X-Ray Detectors > Auto Find Detector Gains"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Errors Test";
		vText[vNumberOfItems] = "Main Menu > Diagnostics > Clear (Inspx Password)> Test Modes Menu > Generate Test Error";
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Errors, Warning Messages, Suppress or Show";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Hardware Options > Suppress (or Show) Warning Messages"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Evaluate Contaminant or Foreign Material Inspection";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Evaluate (Select Product) (Select Inspection) > Send Good Containers\n\nor\n\nMain Menu (running) > Setup (Setup Password) > Evaluate > Evaluate (Select Inspection) > Send Good Containers"; 
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Evaluate Density Inspection";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Evaluate (Select Product) (Select Inspection) > Send Good Containers\n\nor\n\nMain Menu (running) > Setup (Setup Password) > Evaluate > Evaluate (Select Inspection) > Send Good Containers"; 
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Evaluate Inspection";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Evaluate (Select Product) (Select Inspection)\n\nor\n\n>Main Menu (running) > Setup (Setup Password) > Evaluate > Evaluate (Select Inspection)\n\nor\n\nMain Menu > Setup (Setup Password) > Edit (Select Product) > Inspection > Evaluate\n\nor\n\nMain Menu (running) > Setup (Setup Password) > Edit While Running > Disable Ejectors > Evaluate";
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Evaluate Size Inspection";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Evaluate (Select Product) (Select Inspection) > Send Good Containers\n\nor\n\nMain Menu (running) > Setup (Setup Password) > Evaluate > Evaluate (Select Inspection) > Send Good Containers"; 
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Evaluate Void Inspection";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Evaluate (Select Product) (Select Inspection) > Send Good Containers\n\nor\n\nMain Menu (running) > Setup (Setup Password) > Evaluate > Evaluate (Select Inspection) > Send Good Containers"; 
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Event Log Directory Name";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Options > Directory Names > Event Log Directory"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Exclude Conveyor in Calibrate Detectors - How to Exclude Conveyor Calibration";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup X-Ray Detectors > Setup Pixel Adjustments > Correct No Pixels > Correct Edge Pixels > Correct All Pixels > Enter the number of detectors to Calibrate, then enter the number of pixels at the bottom to exclude if the conveyor is showing";
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Exclude Detectors in Calibrate Detectors - How to Exclude Some Detectors From Calibration";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup X-Ray Detectors > Setup Pixel Adjustments > Correct No Pixels > Correct Edge Pixels > Correct All Pixels > Enter the number of detectors to Calibrate, then enter the number of pixels at the bottom to exclude if the conveyor is showing";
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Exercise Radiation Shutter";
	vText[vNumberOfItems] = "Main Menu > Diagnostics > X-Ray > Exercise Shutter 1 Second";
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Exposure, Brightness of Image";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Edit (Select Product) > Exposure\n\nor\n\nNote: this can not be changed while inspecting";
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "External Detectors, System Auxiliary Detectors";
		vText[vNumberOfItems] = (CString)"Install: Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup External Hardware > Setup Auxiliary Detectors"
							+ (CString)"Adjust: Main Menu > Setup > Edit > Current Product > Setup Auxiliary Detectors > [Ejectors To Use]/[Change Window Start]/[Change Window End]"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Files, Delete Files From Hard Disk";
		vText[vNumberOfItems] = "Main Menu > Diagnostics > Comm. (Inspx Password)> Delete Old Log Files From Disk and Delete Old Image Files"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Filter Clogged, Maximumum Temperature Delta Inside to Out";
		if(vGlobalFPGAVersion10Point0OrHigher)
			vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Temperatures > In / Out Delta";
		else
			vText[vNumberOfItems] = (CString)"Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Temperatures > Toggle - Has Surface Temperature / Has Outside Temperature\n\n"
				+ (CString)"There are only 3 Temperature Sensors.  You can set up one of them as either a Surface Temperature Sensor or an Outside Air Temperature Sensor.  To configure which you have chosen in software you toggle the button mentioned above.  If you chose an Outside Air Sensor, you can set the Inside / Outside Air Delta Limit, and if the difference between the Outside and Inside Air goes above the Delta Limit, a Yellow Message will appear indicating Clogged Air Filter."; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "File Based Write Filter Hard Drive Protection";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Hard Drive Protection Menu > Disable or Enable Protection on Reboot"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Files, Setup Auto Delete of Files";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > System Setup > Setup System Options > Enable (or Disable) Auto Delete Logs, set which files to delete"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Finding Bad Pixels, Pixel Values";
	vText[vNumberOfItems] = "First Measure the Source Strength, then when viewing the Source Strength History shown at the end, press Show Pixel Values.\n\n Or if you have taken a Measure Source Strength since the last power down, Main Menu > Show Production Summary > Product Setup Summary > X-Ray Source Strength > Show Pixel Values";
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Flash All Lights";
		vText[vNumberOfItems] = "Any Menu > Help > Clear (Inspx Password) > Chat > Flash All Lights"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Foreign Languages (English, French, Spanish, Polish)";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup  Hardware Options > Language"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Foreign Material or Contaminant Inspection, Evaluate";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Evaluate (Select Product) (Select Inspection) > Send Good Containers\n\nor\n\nMain Menu (running) > Setup (Setup Password) > Evaluate > Evaluate (Select Inspection) > Send Good Containers"; 
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Foreign Material or Contaminant Inspection, Minimum Defect Size";
	if (TempInspxPasswordOK)
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Evaluate (Select Product) (Select Inspection) > Inspection Settings > Minimum Defect Size\n\nor\n\nMain Menu (running) > Rejects > Image Analysis (Setup Password) > Edit While Running > Edit Inspection Settings > Minimum Defect Size\n\nNote: Only on Contaminant, Foreign Material, Void, and Size Inspections. Not available on Density Inspections"; 
	else
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Evaluate (Select Product) (Select Inspection) > Inspection Settings > Minimum Defect Size\n\nor\n\nMain Menu (running) > Rejects > Image Analysis (Setup Password) > Edit While Running > Evaluate (Select Inspection) > Minimum Size\n\nNote: Only on Contaminant, Foreign Material, Void, and Size Inspections. Not available on Density Inspections"; 
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Foreign Material or Contaminant Inspection Threshold";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Evaluate (Select Product) (Select Inspection) > +10 or +1 or -1 or -10\n\nor\n\nMain Menu (running) > Rejects > Image Analysis (Setup Password) > Edit While Running > Change Reject Threshold > +10 or +1 or -1 or -10"; 
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "French Language (English, French, Spanish, Polish)";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup  Hardware Options > Language"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Gain, Preamp Gain of A/D";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup X-Ray Detectors > A/D Preamp Gain"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Gain, X-Ray A/D Gain, Manually Set Gains";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup X-Ray Detectors > Change A/D Gains (Manual value is overwritten if calibrate detectors, but if a detector is not calibrated, it should be manually set.)"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Gain, Calibrate X-Ray Detector Gains, Equalize Gains Automatically";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup X-Ray Detectors > Auto Find Detector Gains"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Generate Production Report";
	vText[vNumberOfItems] = "Main Menu > Show Production Summary > Show Production Report > Generate New Report";
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Good Evaluate for Density";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Evaluate (Select Product) (Select Inspection) > Send Good Containers"; 
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Good Evaluate Contaminant or Foreign Material Inspection";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Evaluate (Select Product) (Select Inspection) > Send Good Containers\n\nor\n\nMain Menu (running) > Setup (Setup Password) > Evaluate > Evaluate (Select Inspection) > Send Good Containers"; 
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Good Evaluate Size Inspection";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Evaluate (Select Product) (Select Inspection) > Send Good Containers\n\nor\n\nMain Menu (running) > Setup (Setup Password) > Evaluate > Evaluate (Select Inspection) > Send Good Containers"; 
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Good Evaluate Void Inspection";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Evaluate (Select Product) (Select Inspection) > Send Good Containers\n\nor\n\nMain Menu (running) > Setup (Setup Password) > Evaluate > Evaluate (Select Inspection) > Send Good Containers"; 
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Hard Disk, Delete Files From Hard Disk";
		vText[vNumberOfItems] = "Main Menu > Diagnostics > Comm. (Inspx Password)> Delete Old Log Files From Disk and Delete Old Image Files"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Hard Drive Protection";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Hard Drive Protection Menu > Disable or Enable Protection on Reboot"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Hard Drive, Scan Disk, Scan Hard Drive For Errors";
	vText[vNumberOfItems] = "Main Menu > Diagnostics > Comm. > Hard Drive Check Hard Disk"; 
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Hardware, System Hardware Options";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Hardware Options"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Height, Top of Product Image (Height of Image plus Bottom)";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Edit (Select Product) > Size > Top\n\nNote: this can not be changed while inspecting";
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "HVPS, Use Spellman DXM/XLF HVPS";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Hardware Options > Use DXM HVPS"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "ID, Machine ID for ScanTrac";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup System Options > ScanTrac Machine ID"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Image Directory Name";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Options > Directory Names > Container Image Directory"; 
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Image Handling";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Image Handling"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Image Load";
	vText[vNumberOfItems] = "Main Menu > Rejects > Image Analysis (Setup Password) > Image Display Options > Load Image From File"; 
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Image Overlap, Maximum";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Edit (Select Product) > Size > Container Edge Finding (Inspx Password) > Maximum Overlap\n\nor\n\nMain Menu (running) > Setup (Setup Password) > Edit While Running > Disable Ejectors > Jump To Start > Container Edge Finding (Inspx Password) > Maximum Overlap";
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Image, Save Rejected Images To Hard Disk";
	vText[vNumberOfItems] = (CString)"Main Menu > Setup (Setup Password) > Options > Directory Names > Enable\\Disable Saving Rejects\n\n"
		+ (CString)"Defaults to Saving Only Reject Images, to Save Good Images:\nMain Menu > (while viewing a good image press) Rejects > Show Main Menu Image > Image Analysis (Setup Password) > Image Display Options > Save Image To File";
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Image Save, Image of Container";
	vText[vNumberOfItems] = (CString)"Main Menu > Rejects > Image Analysis (Setup Password) > Image Display Options > Save Image To File\n\n"
		+ (CString)"Defaults to Saving Only Reject Images, to Save Good Images:\nMain Menu > (while viewing a good image press) Rejects > Show Main Menu Image > Image Analysis (Setup Password) > Image Display Options > Save Image To File";
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Image Save Series, Images of Containers";
		vText[vNumberOfItems] = (CString)"Main Menu > Rejects > Image Analysis (Setup Password) > Image Display Options > Clear (Inspx Password) > Save Image Series\n\n"
			+ (CString)"Defaults to Saving Only Reject Images, to Save Good Images:\nMain Menu > Rejects (Inspx Password) > Toggle - Hide Good Containers / Show Good Containers";
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Image Capture, Screen Capture Directory Name";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Options > Directory Names > Screen Shot Directory"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Image Save, Image on Screen";
		vText[vNumberOfItems] = "Main Menu > Rejects > Image Analysis (Setup Password) > Image Display Options > Clear (Inspx Password) > Save Screen To File"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Images Simulate";
		vText[vNumberOfItems] = "Main Menu > Diagnostics > Clear (Inspx Password) > Test Modes Menu > Enable (or Disable) H/W Test Images"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Image Intensity Correction";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Image Handling > Adjust Image Intensity"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Image Width (of Product)";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Edit (Select Product) > Size > Width\n\nNote: this can not be changed while inspecting\n\nNote:\n\nNote: Auto Size This Container set the Image Width to the correct value so you should not need to set it here manually";
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Improve Learn";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Image Handling > Use Improve Learn"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Improve Learn Menu";
	vText[vNumberOfItems] = "Main Menu > Rejects > Image Analysis > Improve Learn Menu"; 
	vHidden[vNumberOfItems] = true;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Inches / Milli-meters";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > System Setup > Setup External Hardware > Setup Conveyor > Inches / Milli-meters"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Indicator, Slipped Container Indicator Time";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Shape Handling > Slipped Indicator Time"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Inside Air Temperature, Maximum, Adjust, Deltas";
		vText[vNumberOfItems] = (CString)"Adjust:\nMain Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Temperatures > Inside Air Adjust\n\n"
			+ (CString)"Maximum:\nMain Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Temperatures > Maximum Inside Air\n\n"
			+ (CString)"Inside Air / Coolant Liquid Delta and Inside Air / Outside Air Delta:\nSearch for 'Delta' in Command Finder"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Inspect Wrong Width, Wrong Height, At Edge, At Top";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Shape Handling > Pass (or Eject, or Inspect Containers at Edges"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Inspection, Delete";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Edit (Select Product) > Inspection > Delete\n\nor\n\nMain Menu (running) > Setup (Setup Password) > Edit While Running > Disable Ejectors > Delete";
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Inspection, Edit";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Edit (Select Product) > Inspection > Edit\n\nor\n\nMain Menu (running) > Setup (Setup Password) > Edit While Running > Disable Ejectors > Nest To ROI";
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Inspection, Make Inspection Last";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Edit (Select Product) > Inspection > Make Inspection Last\n\nor\n\nMain Menu (running) > Setup (Setup Password) > Edit While Running > Disable Ejectors > Make Inspection Last";
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Inspection Name";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Edit (Select Product) > Inspections > Next To ROI (Select Inspection) > Inspection Name\n\nor\n\nMain Menu (running) > Setup (Setup Password) > Edit While Running > Next To ROI (Select Inspection) > Inspection Name"; 
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Inspection, New";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Edit (Select Product) > Inspection > New\n\nor\n\nMain Menu (running) > Setup (Setup Password) > Edit While Running > Disable Ejectors > New";
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Inspection Process Steps";
	if (TempInspxPasswordOK)
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Evaluate (Select Product) (Select Inspection) (Inspx Password) > Inspection Settings > Enable or Disable Steps\n\nor\n\nMain Menu (running) > Rejects > Image Analysis (Setup Password) > Edit While Running > Edit Inspection Settings > Enable or Disable Steps\n\nNote: Only on Advanced Contaminant, Void, and Size Inspections. Not available on Density Inspections"; 
	else
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Evaluate (Select Product) (Select Inspection) > Inspection Settings > Enable or Disable Steps\n\nNote: Only on Advanced Contaminant, Void, and Size Inspections. Not available on Density Inspections"; 
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Inspection Reference To Top";
	if (TempInspxPasswordOK)
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Evaluate (Select Product) (Select Inspection) (Inspx Password) > Inspection Settings > Reference > Reference Inspection To Top\n\nor\n\nMain Menu (running) > Setup (Setup Password) > Evaluate > Evaluate (Select Inspection) > Inspection Settings > Reference > Reference Inspection To Top\n\nNote: Reference to Top is only available on Advanced Contaminant and Void Inspections"; 
	else
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Evaluate (Select Product) (Select Inspection) > Inspection Settings > Step 1 > Reference Inspection To Top\n\nor\n\nMain Menu (running) > Setup (Setup Password) > Evaluate > Evaluate (Select Inspection) > Inspection Settings > Step 1 > Reference Inspection To Top\n\nNote: Reference to Top is only available on Advanced Contaminant and Void Inspections"; 
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Inspection, ROI Bounds (for an Inspection)";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Edit (Select Product) > Inspection > Next To ROI (Select Inspection) > Use arrow keys to move ROI Boundaries\n\nor\n\nMain Menu (running) > Setup (Setup Password) > Edit While Running > Disable Ejectors > Next To ROI (Select Inspection) > Use arrow keys to move ROI Boundaries\n\nor\n\nMain Menu (running) > Rejects > Image Analysis (Setup Password) > Edit While Running > Change ROI Bounds > Use arrow keys to move ROI Boundaries";
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Inspection Settings";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Evaluate (Select Product) (Select Inspection) > Inspection Settings > Enable or Disable Steps\n\nor\n\nMain Menu (running) > Rejects > Image Analysis (Setup Password) > Edit While Running > Edit Inspection Settings > Enable or Disable Steps\n\nNote: Only on Advanced Contaminant, Void, and Size Inspections. Not available on Density Inspections"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Inspx Password";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Options > Clear (Inspx Password) > Set Inspx Password"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Intensity, Image Intensity Correction";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Image Handling > Adjust Image Intensity"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Integration Time, Maximum Conveyor Speed";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Edit (Select Product) > Exposure > Maximum Conveyor Speed\n\nor\n\nNote: this can not be changed while inspecting";
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Invert Activity Level of Container Triggers, System";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup External Hardware > Setup Container Triggers > Make Active High/Low\n\nThe Container Trigger should be low when beam not blocked and\nHigh when beam blocked in Oscilloscope Display\nIf backward, toggle Make Active High/Low to other state and check."; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Jar Bottom, Inspection Settings";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Evaluate (Select Product) (Select Inspection) > Inspection Settings > Enable or Disable Steps\n\nor\n\nMain Menu (running) > Rejects > Image Analysis (Setup Password) > Edit While Running > Edit Inspection Settings > Enable or Disable Steps"; 
		vHidden[vNumberOfItems] = true;
	}
/*
	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Jitter Positions";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Image Handling > Number of Jitter Positions"; 
		vHidden[vNumberOfItems] = true;
	}
*/
	vNumberOfItems++;
	vItem[vNumberOfItems] = "Keypad Test";
	vText[vNumberOfItems] = "Main Menu > Diagnostics > Keypad"; 
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Languages (English, French, Spanish, Polish)";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup  Hardware Options > Language"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Last, Make Inspection Last";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Edit (Select Product) > Inspection > Make Inspection Last\n\nor\n\nMain Menu (running) > Setup (Setup Password) > Edit While Running > Disable Ejectors > Make Inspection Last";
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Last Rejects, Number of Last Rejects";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Image Handling > Number of Last Rejects"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Learn, Add Reject To Learn";
	vText[vNumberOfItems] = "Main Menu (running) > Rejects > Image Analysis (Setup Password) > Add Reject To Learn\n\nNote: There must be a reject showing on the screen";
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Learn, Enable Quick or Normal Learn";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Edit (Select Product) > Learn > Enable Quick/Normal Learn\n\nor\n\nMain Menu (running) > Setup (Setup Password) > Edit While Running > Disable Ejectors > Back To Learn > Enable Quick/Normal Learn";
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Learn, Improve Learn";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Image Handling > Use Improve Learn"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Learn, Improve Learn Menu";
	vText[vNumberOfItems] = "Main Menu > Rejects > Image Analysis > Improve Learn Menu"; 
	vHidden[vNumberOfItems] = true;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Learn Settings";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Edit (Select Product) > Learn > Clear (Inspx Password) > Reference, or Average or Structure or Histogram or Quick Learn\n\nor\n\nMain Menu (running) > Setup (Setup Password) > Edit While Running > Disable Ejectors > Back To Learn > Clear (Inspx Password) > Reference, or Average or Structure or Histogram or Quick Learn\n\nor\n\nMain Menu > Rejects > Image Analysis (Setup Password) > Edit While Running > Clear (Inspx Password) > Relearn Container Menu > Clear (Inspx Password) > Reference, or Average or Structure or Histogram or Quick Learn";
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Learn, Start Auto Learn";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Edit (Select Product) > Learn > Start Auto Learn\n\nor\n\nMain Menu (running) > Setup (Setup Password) > Edit While Running > Disable Ejectors > Back To Learn > Start Auto Learn\n\nor\n\nMain Menu > Rejects > Image Analysis (Setup Password) > Edit While Running > Re-Learn Container Image";
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Learn Quality Limit";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > System Setup > Setup System Options > Learn Quality Limit";
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Legato, Switch ScanTrac Type To Legato, 600 Watt Pipeline";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > System Setup > Setup Hardware Options > Toggle - Convert To Piccolo / Solo / Legato / 600 Watt Pipeline ScanTrac";
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Life Time or Warranty Counters, Reset";
		vText[vNumberOfItems] = "Main Menu > Diagnostics > Counters (Setup Password) > Clear (Inspx Password) > Appropriate Reset Buttons (Requires Special Reset Password)"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Light Control, Simulate Interlock Signals for testing";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Hardware Options > Simulate (or Use Real) Light Control"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Light Test";
		vText[vNumberOfItems] = "Main Menu > Diagnostics > X-Ray > Clear (Inspx Password) > Test Red (or Yellow or Blue) Light";
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Lights, Flash All Lights";
		vText[vNumberOfItems] = "Any Menu > Help > Clear (Inspx Password) > Chat > Flash All Lights"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Limits, X-Ray Source Limits";
		vText[vNumberOfItems] = "Main Menu > Diagnostics > X-Ray > Clear (Inspx Password) > Source Settings";
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Liquid Coolant Temperature, Maximum, Adjust, In-Coolant Delta";
		vText[vNumberOfItems] = (CString)"Adjust:\nMain Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Temperatures > Coolant Temp Adjust\n\n"
			+ (CString)"Maximum:\nMain Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Temperatures > Maximum Coolant Temp\n\n"
			+ (CString)"Inside Air / Coolant Liquid Delta:\nSearch for 'Delta' in Command Finder"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Load Image of Container";
	vText[vNumberOfItems] = "Main Menu > Rejects > Image Analysis (Setup Password) > Image Display Options > Load Image From File"; 
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Lockout, Container Trigger";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Edit (Select Product) > Size > Container Trigger Lockout\n\nor\n\nMain Menu (running) > Setup (Setup Password) > Edit While Running > Disable Ejectors > Jump To Start > Container Trigger Lockout\n\nNote: Auto Size This Container set the Container Trigger Lockout to the correct value so you should not need to set it here manually";
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Log File Auto Delete";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > System Setup > Setup System Options > Enable (or Disable) Auto Delete Logs, set which files to delete"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Log, Service Record Log";
		vText[vNumberOfItems] = "Main Menu > Diagnostics > Counters (Setup Password) > Clear (Inspx Password) > Enter New Service Item or Display Service Record"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Logging Serial Data";
		vText[vNumberOfItems] = "Main Menu > Diagnostics > Comm. (Inspx Password) > Toggle - Don't Log Serial Data / Log Serial Data";
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Lower Threshold, Density Inspection";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Evaluate (Select Product) (Select Inspection) > Change Lower Threshold\n\nor\n\nMain Menu (running) > Rejects > Image Analysis (Setup Password) > Edit While Running > Change Reject Threshold > +10 or +1 or -1 or -10"; 
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Machine ID for ScanTrac";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup System Options > ScanTrac Machine ID"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Main Menu Reject Display Control";
		vText[vNumberOfItems] = (CString)"Main Menu Reject Display Mode:\nMain Menu > Setup (Setup Password) > Clear (Inspx Password) > System Setup > Setup System Options > Toggle - Defect In Red Box / Defects In Red / Pseudo Color / Up To 10 Red Boxes / No Defect Indicator\n\n"
			+ (CString)"Reject Menu Reject Display Mode:\nMain Menu > (No) Rejects > Clear (Inspx Password) > Inspx Hidden Key (Shift) > Toggle - Defect In Red Box / Defects In Red Boxes / Defects In Red / Defects In Color"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Make Inspection Last";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Edit (Select Product) > Inspection > Make Inspection Last\n\nor\n\nMain Menu (running) > Setup (Setup Password) > Edit While Running > Disable Ejectors > Make Inspection Last";
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Manually Generate Production Report";
	vText[vNumberOfItems] = "Main Menu > Show Production Summary > Show Production Report > Generate New Report";
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Manual Control of Radiation Shutter";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Radiation Shutter > Open (or Close) Shutter"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Maximumum Delta Temperature Inside to Coolant";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Temperatures > In / Coolant Delta"; 
		vHidden[vNumberOfItems] = true;
	}
	
	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Maximumum Delta Temperature Inside to Out";
		if(vGlobalFPGAVersion10Point0OrHigher)
			vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Temperatures > In / Out Delta";
		else
			vText[vNumberOfItems] = (CString)"Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Temperatures > Toggle - Has Surface Temperature / Has Outside Temperature\n\n"
				+ (CString)"There are only 3 Temperature Sensors.  You can set up one of them as either a Surface Temperature Sensor or an Outside Air Temperature Sensor.  To configure which you have chosen in software you toggle the button mentioned above.  If you chose an Outside Air Sensor, you can set the Inside / Outside Air Delta Limit, and if the difference between the Outside and Inside Air goes above the Delta Limit, a Yellow Message will appear indicating Clogged Air Filter."; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Maximum Trigger Standard Deviation, Container Trigger Warning";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > System Setup > Setup External Hardware > Maximum Trigger Std. Dev"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Maximum Conveyor Speed";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Edit (Select Product) > Exposure > Maximum Conveyor Speed\n\nor\n\nNote: this can not be changed while inspecting";
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Maximum Image Overlap";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Edit (Select Product) > Size > Container Edge Finding (Inspx Password) > Maximum Overlap\n\nor\n\nMain Menu (running) > Setup (Setup Password) > Edit While Running > Disable Ejectors > Jump To Start > Container Edge Finding (Inspx Password) > Maximum Overlap";
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Maximum Temperatures and Adjustments, Micro Controller";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Temperatures"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Maximum Temperatures and Adjustments, System Temperatures";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Temperatures"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Maximum X-Ray Source Voltage or Current or Power";
		vText[vNumberOfItems] = "Main Menu > Diagnostics > X-Ray > Clear (Inspx Password) > Source Settings > Maximum Voltage/Current or Maximum Power Allowed";
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Measure Dark Current";
		vText[vNumberOfItems] = "Main Menu > Diagnostics > X-Ray > Clear (Inspx Password) > Measure Dark Current"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Measure X-Ray Source Strength";
	vText[vNumberOfItems] = "Main Menu > Diagnostics > X-Ray > Measure Source Strength (Setup Password)";
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Memory, Have PC Check Amount of RAM";
		vText[vNumberOfItems] = "Main Menu > Diagnostics > Com (Setup Password) > Clear (Inspx Password) > uController Diagnostics > Memory Size Changed! > Reboot Now"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Messages, Warning Messages, Suppress or Show";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Hardware Options > Suppress (or Show) Warning Messages"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Micro Controller Com Port";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Hardware Options > Setup uController Com Port"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Micro Controller Com Port Connectivity";
	vText[vNumberOfItems] = "Main Menu > Diagnostics > Comm."; 
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Micro Controller Reset";
		vText[vNumberOfItems] = "Main Menu > Diagnostics > Comm. > Clear (Inspx Password) > Reset uC (Watchdog Reset) (Confirm)"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Milli-meters / Inches";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > System Setup > Setup External Hardware > Setup Conveyor > Inches / Milli-meters"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Minimum Defect Size";
	if (TempInspxPasswordOK)
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Evaluate (Select Product) (Select Inspection) > Inspection Settings > Minimum Defect Size\n\nor\n\nMain Menu (running) > Rejects > Image Analysis (Setup Password) > Edit While Running > Edit Inspection Settings > Minimum Defect Size\n\nNote: Only on Contaminant or Foreign Material, Void, and Size Inspections. Not available on Density Inspections"; 
	else
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Evaluate (Select Product) (Select Inspection) > Inspection Settings > Minimum Defect Size\n\nor\n\nMain Menu (running) > Rejects > Image Analysis (Setup Password) > Edit While Running > Evaluate (Select Inspection) > Minimum Size\n\nNote: Only on Contaminant or Foreign Material, Void, and Size Inspections. Not available on Density Inspections"; 
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Mouse Cursor Show or Hide";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Hardware Options > Turn On (or Off) Mouse Cursor"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Name, Inspection Name";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Edit (Select Product) > Inspections > Next To ROI > (Select Inspection) > Inspection Name\n\nor\n\nMain Menu (running) > Setup (Setup Password) > Edit While Running > Next To ROI (Select Inspection) > Inspection Name"; 
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Network Clients, Accept Client Connections";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Hardware Options > Accept Client Connections"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "New Inspection";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Edit (Select Product) > Inspection > New\n\nor\n\nMain Menu (running) > Setup (Setup Password) > Edit While Running > Disable Ejectors > New";
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Normal Learn Enable";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Edit (Select Product) > Learn > Enable Quick/Normal Learn\n\nor\n\nMain Menu (running) > Setup (Setup Password) > Edit While Running > Disable Ejectors > Back To Learn > Enable Quick/Normal Learn";
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Number of Last Rejects";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Image Handling > Number of Last Rejects"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Number of Pixels Blocked By Conveyor";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup X-Ray Detectors > Setup Pixel Adjustments > Press middle bottom button to cycle through options until it asks for how many detectors to calibrate and how many pixels are being blocked by the conveyor"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Number of X-Ray Detectors";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup X-Ray Detectors > Number of X-Ray Detectors"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Offset, re-calibrate While Running, Time Before Re-calibrate";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Radiation Shutter > Time Before Re-calibrate"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Offset, X-Ray A/D Offset";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup X-Ray Detectors > Change A/D Offsets"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Operation Guardian Enable/Disable";
		vText[vNumberOfItems] = "Main Menu > Setup (Inspx Password) > Setup System > Setup External Hardware > Enable or Disable Operations Guardian"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Optical Container Trigger Adjustment";
	vText[vNumberOfItems] = "Adjust the pot inside the optical sensor module so the\nYellow and Red LEDs are on when No container blocks the beam, and the\nGreen LED is on when the beam is blocked.";
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Options, System Options";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup System Options"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Oscilloscope Test I/O";
	vText[vNumberOfItems] = "Main Menu > Diagnostics > Scope (Setup Password)"; 
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Offset, Check Weighing, Weight Offset";
	vText[vNumberOfItems] = "Main Menu > Show Production Summary > Weight Trending > Show Recent > Weight Offset"; 
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Outside Air Temperature, Maximum, Adjust, In - Out Delta";
		vText[vNumberOfItems] = (CString)"Adjust:\nMain Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Temperatures > Outside Air Adjust\n\n"
			+ (CString)"Maximum:\nMain Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Temperatures > Maximum Outside Air"
			+ (CString)"Inside Air / Outside Air Delta:\nSearch for 'Delta' in Command Finder"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Overlap, Maximum Image Overlap";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Edit (Select Product) > Size > Container Edge Finding (Inspx Password) > Maximum Overlap\n\nor\n\nMain Menu (running) > Setup (Setup Password) > Edit While Running > Disable Ejectors > Jump To Start > Container Edge Finding (Inspx Password) > Maximum Overlap";
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Overscan";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Edit (Select Product) > Size > Container Edge Finding (Inspx Password) > Overscan\n\nNote:\n\nOverscan cannot be changed in Edit While Running\n\nOverscan lowers the Encoder Divide Factor to get more encoder pulses in the image.\n\nMaximum Image Width is 12 inches in Overscan 2 and 6 inches in Overscan 4";
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Overwrite Pixel, Copy Pixel to Overwrite Bad Pixel";
		vText[vNumberOfItems] = "Main Menu > Setup (Inspx Password) > Setup System > Setup X-Ray Detectors > Setup Pixel Adjustments > Select the Pixel to Overwrite > Save and Exit to Multiplicative Factor > Save and Exit to Additive Factor > Enter the Pixel Number to Copy On Top of This One.\n\nNormally copy the closest good pixel.  May overwrite multiple pixels."; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Pass Containers at Edge, Top, Wrong Width, Wrong Height";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Shape Handling > Pass (or Eject, or Inspect Containers at Edges"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Password, Inspx";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Options > Clear (Inspx Password) > Set Inspx Password"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Password Setup";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Options > Set Setup Password"; 
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "PC RAM or Memory, Have PC Check Amount of RAM";
		vText[vNumberOfItems] = "Main Menu > Diagnostics > Com (Setup Password) > Clear (Inspx Password) > uController Diagnostics > Memory Size Changed! > Reboot Now"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Piccolo, Switch ScanTrac Type To Piccolo";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > System Setup > Setup Hardware Options > Toggle - Convert To Piccolo / Solo / Legato / 600 Watt Pipeline ScanTrac";
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Picture -> Search For Image";
	vText[vNumberOfItems] = "Picture -> Search For Image";
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Pipeline, Continuous Feed Containers";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > System Setup > Setup External Hardware > Setup System Conveyor > Continuous Feed Containers"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Pipeline, Continuous Feed Encoder";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > System Setup > Setup External Hardware > Setup System Conveyor > Continuous Feed Encoder"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Pixel Adjustments or Calibrations, Manual Setting";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup X-Ray Detectors > Setup Pixel Adjustments";
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Pixel Adjustments or Calibrations Automatically Find";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup X-Ray Detectors > Calibrate Detector Pixels";
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Pixel, Number of Pixels Blocked By Conveyor";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup X-Ray Detectors > Setup Pixel Adjustments > Press middle bottom button to cycle through options until it asks for how many detectors to calibrate and how many pixels are being blocked by the conveyor"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Pixel Calibration, Complete Calibration, Calibrate X-Ray Detectors";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Calibrate Detectors! (Certified Setup Password)"; 
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Pixel Clock Divider";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup X-Ray Detectors > Pixel Clock Divider"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Pixel, Copy Pixel to Overwrite Bad Pixel";
		vText[vNumberOfItems] = "Main Menu > Setup (Inspx Password) > Setup System > Setup X-Ray Detectors > Setup Pixel Adjustments > Select the Pixel to Overwrite > Save and Exit to Multiplicative Factor > Save and Exit to Additive Factor > Enter the Pixel Number to Copy On Top of This One.\n\nNormally copy the closest good pixel.  May overwrite multiple pixels."; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Pixel Size, X-Ray Detector Pixel Size";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup X-Ray Detectors > X-Ray Detector Pixel Size"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Pixel Values";
	vText[vNumberOfItems] = "First Measure the Source Strength, then when viewing the Source Strength History shown at the end, press Show Pixel Values.\n\n Or if you have taken a Measure Source Strength since the last power down, Main Menu > Show Production Summary > Product Setup Summary > X-Ray Source Strength > Show Pixel Values";
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Pointer, Mouse Cursor Show or Hide";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Hardware Options > Turn On (or Off) Mouse Cursor"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Polish Language (English, French, Spanish, Polish)";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup  Hardware Options > Language"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Position, Ejector Position";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Edit (Select Product) > Inspections > Setup Ejector Timing > Position\n\nor\n\nMain Menu (running) > Setup (Setup Password) > Edit While Running > Disable Ejectors > Setup Ejector Timing > Position";
	vHidden[vNumberOfItems] = false;
/*
	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Positions, Jitter Positions";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Image Handling > Number of Jitter Positions"; 
		vHidden[vNumberOfItems] = true;
	}
*/
	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Power, Maximum X-Ray Source Power";
		vText[vNumberOfItems] = "Main Menu > Diagnostics > X-Ray > Clear (Inspx Password) > Source Settings > Maximum Power Allowed";
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Power Errors, AC Power Monitor";
	vText[vNumberOfItems] = "Main Menu > Diagnostics > Voltages > Show AC Voltage Problems";
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Power Error Yellow Messages Enable/Disable, AC Power Monitor";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > System Setup > Setup Hardware Options > Disable/Enable AC Messges"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Power Monitor, AC Power Monitor";
	vText[vNumberOfItems] = "Main Menu > Diagnostics > Voltages > Show AC Voltage Plot";
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Power Off, Shut Down Computer";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Press Hidden Key > Shut Down Computer"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Preamp Gain of A/D";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup X-Ray Detectors > A/D Preamp Gain"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Processing, Inspection Process Steps";
	if (TempInspxPasswordOK)
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Evaluate (Select Product) (Select Inspection) (Inspx Password) > Inspection Settings > Enable or Disable Steps\n\nor\n\nMain Menu (running) > Rejects > Image Analysis (Setup Password) > Edit While Running > Edit Inspection Settings > Enable or Disable Steps\n\nNote: Only on Advanced Contaminant, Void, and Size Inspections. Not available on Density Inspections"; 
	else
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Evaluate (Select Product) (Select Inspection) > Inspection Settings > Enable or Disable Steps\n\nNote: Only on Advanced Contaminant, Void, and Size Inspections. Not available on Density Inspections"; 
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Product Alarm, X of Y Alarm";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Edit (Select Product) > Inspections > Set X of Y Alarm1 or Set X of Y Alarm2\n\nor\n\nMain Menu (running) > Setup (Setup Password) > Edit While Running > Disable Ejectors > Set X of Y Alarm1 or Set X of Y Alarm2";
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Product Code";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Options > Use Product Code";
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Product Density ROI";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Edit (Select Product) > Exposure > Product Density ROI\n\nor\n\nMain Menu (running) > Setup (Setup Password) > Edit While Running > Disable Ejectors > Back To Learn > Back To Exposure > Product Density ROI";
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Product Image, Start Auto Learn";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Edit (Select Product) > Learn > Start Auto Learn\n\nor\n\nMain Menu (running) > Setup (Setup Password) > Edit While Running > Disable Ejectors > Back To Learn > Start Auto Learn\n\nor\n\nMain Menu > Rejects > Image Analysis (Setup Password) > Edit While Running > Re-Learn Container Image";
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Production Report, Generate Manually";
	vText[vNumberOfItems] = "Main Menu > Show Production Summary > Show Production Report > Generate New Report";
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Production Report Directory Name";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Options > Directory Names > Production Report Directory"; 
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Production Stop Time Period";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Options > Production Stop Time Period";
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Protection, Hard Drive Protection";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Hard Drive Protection Menu > Disable or Enable Protection on Reboot"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Pseudo Color, Reject Display Control";
		vText[vNumberOfItems] = (CString)"Main Menu Reject Display Mode:\nMain Menu > Setup (Setup Password) > Clear (Inspx Password) > System Setup > Setup System Options > Toggle - Defect In Red Box / Defects In Red / Pseudo Color / Up To 10 Red Boxes / No Defect Indicator\n\n"
			+ (CString)"Reject Menu Reject Display Mode:\nMain Menu > (No) Rejects > Clear (Inspx Password) > Inspx Hidden Key (Shift) > Toggle - Defect In Red Box / Defects In Red Boxes / Defects In Red / Defects In Color"; 
		vHidden[vNumberOfItems] = true;
	}
	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Pulses Per Unit, Encoder Pulses Per Inch";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > System Setup > Setup External Hardware > Setup Conveyor > Encoder Pulses Per Inch"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Purple Line, Container Edge Finding, Edge Location Area, Edge Finding";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Edit (Select Product) > Size > Container Edge Finding\n\nor\n\nMain Menu (running) > Setup (Setup Password) > Edit While Running > Disable Ejectors > Jump To Start > Container Edge Finding\n\nUse the Up, Down, Longer, and Shorter Buttons to adjust the Purple Line on the left side of the image to the location across from the container to look for the edges of the container.\n\nThis must be set manually\n\nAdjust Purple Line so ScanTrac finds the edges correctly of every image.";
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Quick Learn Enable";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Edit (Select Product) > Learn > Enable Quick/Normal Learn\n\nor\n\nMain Menu (running) > Setup (Setup Password) > Edit While Running > Disable Ejectors > Back To Learn > Enable Quick/Normal Learn";
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Radiation Shutter";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Radiation Shutter"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Radiation Shutter Manual Control";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Radiation Shutter > Open (or Close) Shutter"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "RAM, Have PC Check Amount of RAM";
		vText[vNumberOfItems] = "Main Menu > Diagnostics > Com (Setup Password) > Clear (Inspx Password) > uController Diagnostics > Memory Size Changed! > Reboot Now"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Re-enable Ejectors";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup System Options > Re-enable Ejectors"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Re-Learn, Start Auto Learn";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Edit (Select Product) > Learn > Start Auto Learn\n\nor\n\nMain Menu (running) > Setup (Setup Password) > Edit While Running > Disable Ejectors > Back To Learn > Start Auto Learn\n\nor\n\nMain Menu > Rejects > Image Analysis (Setup Password) > Edit While Running > Re-Learn Container Image";
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Reboot Computer";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Hard Drive Protection Menu > Reboot PC!"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Recalibrate, Time Before Recalibrate While Running";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Radiation Shutter > Time Before Recalibrate"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Red Box, Reject Display Control";
		vText[vNumberOfItems] = (CString)"Main Menu Reject Display Mode:\nMain Menu > Setup (Setup Password) > Clear (Inspx Password) > System Setup > Setup System Options > Toggle - Defect In Red Box / Defects In Red / Pseudo Color / Up To 10 Red Boxes / No Defect Indicator\n\n"
			+ (CString)"Reject Menu Reject Display Mode:\nMain Menu > (No) Rejects > Clear (Inspx Password) > Inspx Hidden Key (Shift) > Toggle - Defect In Red Box / Defects In Red Boxes / Defects In Red / Defects In Color"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Reference Inspections To Edges";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Image Handling > Adjust ROI Size To Edges"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Reference Inspection To Top";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Evaluate (Select Product) (Select Inspection) > Inspection Settings > Step 1 > Reference Inspection To Top\n\nor\n\nMain Menu (running) > Setup (Setup Password) > Evaluate > Evaluate (Select Inspection) > Inspection Settings > Step 1 > Reference Inspection To Top\n\nNote: Reference to Top is only available on Advanced Contaminant and Void Inspections"; 
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Region Of Interest (ROI)";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Edit (Select Product) > Edit > (Select Inspection) > Arrow keys to move ROI lines\n\nor\n\nMain Menu (running) > Rejects > Image Analysis (Setup Password) > Edit While Running > Change ROI Bounds > Arrow keys to move ROI lines"; 
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Reject, Add Reject To Learn";
	vText[vNumberOfItems] = "Main Menu (running) > Rejects > Image Analysis (Setup Password) > Add Reject To Learn\n\nNote: There must be a reject showing on the screen";
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Reject Alarm X of Y";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Edit (Select Product) > Inspections > Set X of Y Alarm1 or Set X of Y Alarm2\n\nor\n\nMain Menu (running) > Setup (Setup Password) > Edit While Running > Disable Ejectors > Set X of Y Alarm1 or Set X of Y Alarm2";
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Reject Display Control";
		vText[vNumberOfItems] = (CString)"Main Menu Reject Display Mode:\nMain Menu > Setup (Setup Password) > Clear (Inspx Password) > System Setup > Setup System Options > Toggle - Defect In Red Box / Defects In Red / Pseudo Color / Up To 10 Red Boxes / No Defect Indicator\n\n"
			+ (CString)"Reject Menu Reject Display Mode:\nMain Menu > (No) Rejects > Clear (Inspx Password) > Inspx Hidden Key (Shift) > Toggle - Defect In Red Box / Defects In Red Boxes / Defects In Red / Defects In Color"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Rejects, Number of Last Rejects";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Image Handling > Number of Last Rejects"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Rejects, Image Save Series, Images of Containers";
		vText[vNumberOfItems] = (CString)"Main Menu > Rejects > Image Analysis (Setup Password) > Image Display Options > Clear (Inspx Password) > Save Image Series\n\n"
			+ (CString)"Defaults to Saving Only Reject Images, to Save Good Images:\nMain Menu > Rejects > Clear (Inspx Password) > Toggle - Hide Good Containers / Show Good Containers";
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Rejects, Save Image";
	vText[vNumberOfItems] = (CString)"Main Menu > Rejects > Image Analysis (Setup Password) > Image Display Options > Save Image To File\n\n"
		+ (CString)"Defaults to Saving Only Reject Images, to Save Good Images:\nMain Menu > (while viewing a good image press) Rejects > Show Main Menu Image > Image Analysis (Setup Password) > Image Display Options > Save Image To File";
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Rejects, Save Rejected Images To Hard Disk";
	vText[vNumberOfItems] = (CString)"Main Menu > Setup (Setup Password) > Options > Directory Names > Enable\\Disable Saving Rejects\n\n"
		+ (CString)"Defaults to Saving Only Reject Images, to Save Good Images:\nMain Menu > (while viewing a good image press) Rejects > Show Main Menu Image > Image Analysis (Setup Password) > Image Display Options > Save Image To File";
	vHidden[vNumberOfItems] = false;

	//if (TempInspxPasswordOK)
	//{
	//	vNumberOfItems++;
	//	vItem[vNumberOfItems] = "Remote Control Display";
	//	vText[vNumberOfItems] = "Main Menu > Using mouse, click on Title Bar where it says Main Menu, then move mouse off of it"; 
	//	vHidden[vNumberOfItems] = true;
	//}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Remove Background From Image";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Image Handling > Remove Background"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Remove -> Search For Delete";
	vText[vNumberOfItems] = "Remove -> Search For Delete";
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Report, Manually Generate Production Report";
	vText[vNumberOfItems] = "Main Menu > Show Production Summary > Show Production Report > Generate New Report";
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Replace Pixel, Copy Pixel to Overwrite Bad Pixel";
		vText[vNumberOfItems] = "Main Menu > Setup (Inspx Password) > Setup System > Setup X-Ray Detectors > Setup Pixel Adjustments > Select the Pixel to Overwrite > Save and Exit to Multiplicative Factor > Save and Exit to Additive Factor > Enter the Pixel Number to Copy On Top of This One.\n\nNormally copy the closest good pixel.  May overwrite multiple pixels."; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Reset Counters";
	vText[vNumberOfItems] = "Main Menu > Show Production Summary > Reset Counts (confirm) > Close Production Report when done viewing"; 
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Reset Counters Password Setup";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Options > Counters Password"; 
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Reset Micro Controller";
		vText[vNumberOfItems] = "Main Menu > Diagnostics > Comm. > Clear (Inspx Password) > Reset uC (Watchdog Reset) (Confirm)"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Reset Warranty or Life Time Counters";
		vText[vNumberOfItems] = "Main Menu > Diagnostics > Counters (Setup Password) > Clear (Inspx Password) > Appropriate Reset Buttons (Requires Special Reset Password)"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "ROI, Product Density ROI";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Edit (Select Product) > Exposure > Product Density ROI\n\nor\n\nMain Menu (running) > Setup (Setup Password) > Edit While Running > Disable Ejectors > Back To Learn > Back To Exposure > Product Density ROI";
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "ROI Bounds (for an Inspection)";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Edit (Select Product) > Inspection > Next To ROI (Select Inspection) > Use arrow keys to move ROI Boundaries\n\nor\n\nMain Menu (running) > Setup (Setup Password) > Edit While Running > Disable Ejectors > Next To ROI (Select Inspection)  > Use arrow keys to move ROI Boundaries\n\nor\n\nMain Menu (running) > Rejects > Image Analysis (Setup Password) > Edit While Running > Change ROI Bounds > Use arrow keys to move ROI Boundaries";
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "ROI Size Fixed / Adjust to Edges";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Image Handling > Fixed Inspection ROI Size / Adjust ROI Size to Edges"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Save Image of Container";
	vText[vNumberOfItems] = (CString)"Main Menu > Rejects > Image Analysis (Setup Password) > Image Display Options > Save Image To File\n\n"
		+ (CString)"Defaults to Saving Only Reject Images, to Save Good Images:\nMain Menu > (while viewing a good image press) Rejects > Show Main Menu Image > Image Analysis (Setup Password) > Image Display Options > Save Image To File";
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Save Image Series of Containers";
		vText[vNumberOfItems] = (CString)"Main Menu > Rejects > Image Analysis (Setup Password) > Image Display Options > Clear (Inspx Password) > Save Image Series\n\n"
			+ (CString)"Defaults to Saving Only Reject Images, to Save Good Images:\nMain Menu > Rejects > Clear (Inspx Password) > Toggle - Hide Good Containers / Show Good Containers";
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Save Rejected Images To Hard Disk";
	vText[vNumberOfItems] = (CString)"Main Menu > Setup (Setup Password) > Options > Directory Names > Enable\\Disable Saving Rejects\n\n"
		+ (CString)"Defaults to Saving Only Reject Images, to Save Good Images:\nMain Menu > (while viewing a good image press) Rejects > Show Main Menu Image > Image Analysis (Setup Password) > Image Display Options > Save Image To File";
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Save Screen Capture";
	vText[vNumberOfItems] = "Any Menu (except data entry menus)> 0 > 1\nAny Menu > Inspx Hidden Key (Shift) > 1"; 
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Save Screen Capture, Directory Name";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Options > Directory Names > Screen Shot Directory"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Scan Disk, Scan Hard Drive For Errors";
	vText[vNumberOfItems] = "Main Menu > Diagnostics > Comm. > Hard Drive Check Hard Disk"; 
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "ScanTrac Machine ID";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup System Options > ScanTrac Machine ID"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "ScanTrac System Type (Piccolo, Solo, Legato, 600 Watt Pipeline)";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Hardware Options > Toggle - Convert to (Piccolo / Solo / Legato / 600 Watt Pipeline) ScanTrac"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Screen Capture";
	vText[vNumberOfItems] = "Any Menu (except data entry menus)> 0 > 1\nAny Menu > Inspx Hidden Key (Shift) > 1"; 
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Screen Shot Directory Name";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Options > Directory Names > Screen Shot Directory"; 
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Select Ejectors (for an Inspection)";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Edit (Select Product) > Inspection > Next To ROI (Select Inspection) > Select Ejectors\n\nor\n\nMain Menu (running) > Setup (Setup Password) > Edit While Running > Disable Ejectors > Next To ROI (Select Inspection) > Select Ejectors";
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Serial Data Logging";
		vText[vNumberOfItems] = "Main Menu > Diagnostics > Comm. (Setup Password) > Toggle - Don't Log Serial Data / Log Serial Data";
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Service Record";
		vText[vNumberOfItems] = "Main Menu > Diagnostics > Counters (Setup Password) > Clear (Inspx Password) > Enter New Service Item or Display Service Record"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Setup Password";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Options > Set Setup Password"; 
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Shape Handling";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Shape Handling"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Show Pixel Values, To Find Bad Pixels";
	vText[vNumberOfItems] = "First Measure the Source Strength, then when viewing the Source Strength History shown at the end, press Show Pixel Values.\n\n Or if you have taken a Measure Source Strength since the last power down, Main Menu > Show Production Summary > Product Setup Summary > X-Ray Source Strength > Show Pixel Values";
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Shut Down Computer";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Press Hidden Key > Shut Down Computer"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Shutter, Exercise Radiation Shutter";
	vText[vNumberOfItems] = "Main Menu > Diagnostics > X-Ray > Exercise Shutter 1 Second";
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Shutter, Radiation Shutter";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Radiation Shutter"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "SideView, Switch ScanTrac Type";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > System Setup > Setup Hardware Options > Toggle - Convert To Piccolo / Solo / Legato / 600 Watt Pipeline ScanTrac";
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Simulate Container";
		vText[vNumberOfItems] = "Main Menu > Diagnostics > Clear (Inspx Password) > Test Modes Menu > Simulated Container Rate"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Simulate Conveyor";
		vText[vNumberOfItems] = "Main Menu > Diagnostics > Clear (Inspx Password) > Test Modes Menu > Simulated Encoder Rate"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Simulate, Demo Mode, Enable or Disable";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Hardware Options > Enable (or Disable) Demo Mode"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Simulate Encoder";
		vText[vNumberOfItems] = "Main Menu > Diagnostics > Clear (Inspx Password) > Test Modes Menu > Simulated Encoder Rate"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Simulate Images";
		vText[vNumberOfItems] = "Main Menu > Diagnostics > Clear (Inspx Password) > Test Modes Menu > Enable (or Disable) H/W Test Images"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Simulate Light Control Board Signals for testing";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Hardware Options > Simulate (or Use Real) Light Control"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Simulate, Continuous Feed Containers";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > System Setup > Setup External Hardware > Setup System Conveyor > Continuous Feed Containers"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Simulate, Continuous Feed Encoder";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > System Setup > Setup External Hardware > Setup System Conveyor > Continuous Feed Encoder"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Simulate, Test Modes";
		vText[vNumberOfItems] = "Main Menu > Diagnostics > Clear (Inspx Password) > Test Modes Menu"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Size, Auto Size This Container";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Edit (Select Product) > Size > Auto Size This Container\n\nNote:\n\nAuto Size This Container cannot be perfomed in Edit While Running";
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Size, Evaluate Size Inspection";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Evaluate (Select Product) (Select Inspection) > Send Good Containers\n\nor\n\nMain Menu (running) > Setup (Setup Password) > Evaluate > Evaluate (Select Inspection) > Send Good Containers"; 
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Size, Minimum Defect Size";
	if (TempInspxPasswordOK)
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Evaluate (Select Product) (Select Inspection) > Inspection Settings > Minimum Defect Size\n\nor\n\nMain Menu (running) > Rejects > Image Analysis (Setup Password) > Edit While Running > Edit Inspection Settings > Minimum Defect Size\n\nNote: Only on Contaminant or Foreign Material, Void, and Size Inspections. Not available on Density Inspections"; 
	else
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Evaluate (Select Product) (Select Inspection) > Inspection Settings > Minimum Defect Size\n\nor\n\nMain Menu (running) > Rejects > Image Analysis (Setup Password) > Edit While Running > Evaluate (Select Inspection) > Minimum Size\n\nNote: Only on Contaminant or Foreign Material, Void, and Size Inspections. Not available on Density Inspections"; 
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Size Inspection Threshold";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Evaluate (Select Product) (Select Inspection) > +10 or +1 or -1 or -10\n\nor\n\nMain Menu (running) > Rejects > Image Analysis (Setup Password) > Edit While Running > Change Reject Threshold > +10 or +1 or -1 or -10"; 
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Slipped Container Indicator Time";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Shape Handling > Slipped Indicator Time"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Software Version";
	vText[vNumberOfItems] = "Main Menu > Diagnostics > Version"; 
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Solo, Switch ScanTrac Type To Solo";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > System Setup > Setup Hardware Options > Toggle - Convert To Piccolo / Solo / Legato / 600 Watt Pipeline ScanTrac";
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Source, Measure X-Ray Source Strength";
	vText[vNumberOfItems] = "Main Menu > Diagnostics > X-Ray > Measure Source Strength (Setup Password)";
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Source, Reset X-Ray Source Duration";
		vText[vNumberOfItems] = "Main Menu > Diagnostics > Counters (Setup Password) > Clear (Inspx Password) > Appropriate Reset Buttons (Requires Special Reset Password)"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Source, X-Ray Source Current";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Edit (Select Product) > Exposure > X-Ray Source Current\n\nor\n\nNote: this can not be changed while inspecting";
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Source, X-Ray Source Limits";
		vText[vNumberOfItems] = "Main Menu > Diagnostics > X-Ray > Clear (Inspx Password) > Source Settings";
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Source, X-Ray Source Voltage";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Edit (Select Product) > Exposure > X-Ray Source Voltage\n\nor\n\nNote: this can not be changed while inspecting";
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Spanish Language (English, French, Spanish, Polish)";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup  Hardware Options > Language"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Speed Correction, Conveyor Speed Correction for Ejectors";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Edit (Select Product) > Inspections > Setup Ejector Timing > Ejector Timing Factor\n\nor\n\nMain Menu (running) > Setup (Setup Password) > Edit While Running > Disable Ejectors > Setup Ejector Timing > Conveyor Speed Correction";
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Speed, Maximum Conveyor Speed";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Edit (Select Product) > Exposure > Maximum Conveyor Speed\n\nor\n\nNote: this can not be changed while inspecting";
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Spellman, Use Spellman DXM/XLF HVPS";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Hardware Options > Use DXM HVPS"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Standard Deviation, Maximum Trigger Standard Deviation, Container Trigger Warning";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > System Setup > Setup External Hardware > Maximum Trigger Std. Dev"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Start Auto Learn";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Edit (Select Product) > Learn > Start Auto Learn\n\nor\n\nMain Menu (running) > Setup (Setup Password) > Edit While Running > Disable Ejectors > Back To Learn > Start Auto Learn\n\nor\n\nMain Menu (running) > Rejects > Image Analysis (Setup Password) > Edit While Running > Re-Learn Container Image";
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Send Bad Containers";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Evaluate (Select Product) (Select Inspection) > Send Bad Containers\n\nor\n\nMain Menu (running) > Setup (Setup Password) > Evaluate > Evaluate (Select Inspection) > Send Bad Containers\n\nor\n\nMain Menu (running) > Rejects > Image Analysis (Setup Password) > Edit While Running Setup > Evaluate Inspection > Send Bad Containers"; 
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Send Good Containers";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Evaluate (Select Product) (Select Inspection) > Send Good Containers\n\nor\n\nMain Menu (running) > Setup (Setup Password) > Evaluate > Evaluate (Select Inspection) > Send Good Containers"; 
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Stops, Conveyor Stops";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Shape Handling > Inspect/Pass/Reject On Conveyor Stops"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Stop, Conveyor Stops";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Shape Handling > Inspect/Pass/Reject On Conveyor Stops"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Stop Inspecting When Conveyor Stops";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup System Options > Enable (or Disable) Conveyor Stop"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Stop On Unconfirmed Eject";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup System Options > Stop On Unconfirmed Eject"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Stop Production Time Period";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Options > Production Stop Time Period";
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Stream (Up Stream / Down Stream Interlock Signals)";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup External Hardware > Use Down Stream or Don't Use Down Stream"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Strength, Measure X-Ray Source Strength";
	vText[vNumberOfItems] = "Main Menu > Diagnostics > X-Ray > Measure Source Strength (Setup Password)";
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Surface Temperature, Maximum, Adjust";
		vText[vNumberOfItems] = (CString)"Adjust:\nMain Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Temperatures > Surface Temp Adjust\n\n"
			+ (CString)"Maximum:\nMain Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Temperatures > Maximum Surface Temp";
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Surface Temperature, Current Values";
	vText[vNumberOfItems] = (CString)"Main Menu > Diagnostics >	X-Ray";
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "System Setup";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "System Type, ScanTrac Model Name (Piccolo, Solo, Legato, 600 Watt Pipeline)";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > System Setup > Setup Hardware Options > Toggle - Convert to (Piccolo / Solo / Legato / 600 Watt Pipeline) ScanTrac"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Temperature Adjustments and Limits, Micro Controller";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Temperatures > uC Temps";
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Temperature Adjustments and Limits, System Temperatures";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Temperatures";
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Temperature, Coolant Liquid Temperature";
		vText[vNumberOfItems] = (CString)"Adjust:\nMain Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Temperatures > Coolant Temp Adjust\n\n"
			+ (CString)"Maximum:\nMain Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Temperatures > Maximum Coolant Temp"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Temperature, Inside Air Temperature";
		vText[vNumberOfItems] = (CString)"Adjust:\nMain Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Temperatures > Inside Air Adjust\n\n"
			+ (CString)"Maximum:\nMain Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Temperatures > Maximum Inside Air"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Temperature, Maximumum Delta Inside Air to Coolant";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Temperatures > In / Coolant Delta"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Temperature, Maximumum Delta Inside to Outside Air";
		if(vGlobalFPGAVersion10Point0OrHigher)
			vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Temperatures > In / Out Delta";
		else
			vText[vNumberOfItems] = (CString)"Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Temperatures > Toggle - Has Surface Temperature / Has Outside Temperature\n\n"
				+ (CString)"There are only 3 Temperature Sensors.  You can set up one of them as either a Surface Temperature Sensor or an Outside Air Temperature Sensor.  To configure which you have chosen in software you toggle the button mentioned above.  If you chose an Outside Air Sensor, you can set the Inside / Outside Air Delta Limit, and if the difference between the Outside and Inside Air goes above the Delta Limit, a Yellow Message will appear indicating Clogged Air Filter."; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Temperature, Outside Air Temperature";
		vText[vNumberOfItems] = (CString)"Adjust:\nMain Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Temperatures > Outside Air Adjust\n\n"
			+ (CString)"Maximum:\nMain Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Temperatures > Maximum Outside Air"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Temperature, Surface Temperature";
		vText[vNumberOfItems] = (CString)"Adjust:\nMain Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Temperatures > Surface Temp Adjust\n\n"
			+ (CString)"Maximum:\nMain Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Temperatures > Maximum Surface Temp"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	if (!vGlobalFPGAVersion10Point0OrHigher)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Temperature, Toggle 'Has Surface / Outside Temperature'";
		vText[vNumberOfItems] = (CString)"Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Temperatures > Toggle - Has Surface Temperature / Has Outside Temperature\n\n"
			+ (CString)"There are only 3 Temperature Sensors.  You can set up one of them as either a Surface Temperature Sensor or an Outside Air Temperature Sensor.  To configure which you have chosen in software you toggle the button mentioned above.  If you chose an Outside Air Sensor, you can set the Inside / Outside Air Delta Limit, and if the difference between the Outside and Inside Air goes above the Delta Limit, a Yellow Message will appear indicating Clogged Air Filter."; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Temperatures, Current Values of Inside Air, Surface, Liquid Coolant, and Detector Temp";
	vText[vNumberOfItems] = (CString)"Main Menu > Diagnostics >	X-Ray";
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Test Alarms";
		vText[vNumberOfItems] = "Main Menu > Diagnostics > Scope > Clear (Inspx Password) > Test Alarm Output";
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Test Can Stop On/Off";
		vText[vNumberOfItems] = "Main Menu > Diagnostics > Scope > Clear (Inspx Password) > Can Stop 1 (or 2) On (or Off)";
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Test Conveyor";
		vText[vNumberOfItems] = "Main Menu > Diagnostics > Scope > Clear (Inspx Password) > Start (or Stop) Conveyor Now";
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Test Ejector On/Off";
		vText[vNumberOfItems] = "Main Menu > Diagnostics > Scope > Clear (Inspx Password) > Ejector 1 (or 2 or 3) On (or Off)";
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Test Ejector Pulsed";
		vText[vNumberOfItems] = "Main Menu > Diagnostics > Clear (Inspx Password)> Test Modes Menu > Test Fire Ejector Now (Test Fire Ejector Setup)\n\nor\n\nMain Menu (running) > Diagnostics > Clear (Inspx Password) > Test Modes Menu > Eject Next Container";
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Test Ejector, Test Fire Now!";
	vText[vNumberOfItems] = "Main Menu > Diagnostics > Test Fire Ejector Now";
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Test Ejector With Product";
	vText[vNumberOfItems] = "Main Menu > Setup > Edit (Select Product) > Inspections > Setup Ejector Timing > Test Eject Next Container or Test Eject All Containers";
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Test Errors";
		vText[vNumberOfItems] = "Main Menu > Diagnostics > Clear (Inspx Password)> Test Modes Menu > Generate Test Error";
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Test I/O Oscilloscope";
	vText[vNumberOfItems] = "Main Menu > Diagnostics > Scope (Setup Password)"; 
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Test Keypad";
	vText[vNumberOfItems] = "Main Menu > Diagnostics > Keypad"; 
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Test Lights";
		vText[vNumberOfItems] = "Main Menu > Diagnostics > X-Ray > Clear (Inspx Password) > Test Red (or Yellow or Blue) Light";
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Test Modes";
		vText[vNumberOfItems] = "Main Menu > Diagnostics > Clear (Inspx Password) > Test Modes Menu"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Test Trigger";
	vText[vNumberOfItems] = "Main Menu (not running) > Diagnostics > Start (or Stop) Trigger Test"; 
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Test X-Ray Shutter, Exercise Radiation Shutter";
	vText[vNumberOfItems] = "Main Menu > Diagnostics > X-Ray > Exercise Shutter 1 Second";
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Threshold, Contaminant or Foreign Material Inspection";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Evaluate (Select Product) (Select Inspection) > +10 or +1 or -1 or -10\n\nor\n\nMain Menu (running) > Rejects > Image Analysis (Setup Password) > Edit While Running > Change Reject Threshold > +10 or +1 or -1 or -10"; 
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Threshold, Density Inspection";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Evaluate (Select Product) (Select Inspection) > Change Threshold on Missing Lid, Underfill and Overfill, Change Upper Threshold or Change Lower Threshold on Density\n\nor\n\nMain Menu (running) > Rejects > Image Analysis (Setup Password) > Edit While Running > Change Reject Threshold > +10 or +1 or -1 or -10.  Note, On Density Inspection pick Upper or Lower Threshold"; 
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Threshold, Size Inspection";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Evaluate (Select Product) (Select Inspection) > +10 or +1 or -1 or -10\n\nor\n\nMain Menu (running) > Rejects > Image Analysis (Setup Password) > Edit While Running > Change Reject Threshold > +10 or +1 or -1 or -10"; 
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Threshold, Void Inspection";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Evaluate (Select Product) (Select Inspection) > +10 or +1 or -1 or -10\n\nor\n\nMain Menu (running) > Rejects > Image Analysis (Setup Password) > Edit While Running > Change Reject Threshold > +10 or +1 or -1 or -10"; 
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Time";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Options > Set Time"; 
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Time Before Re-calibrate Offset while running";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Radiation Shutter > Time Before Re-calibrate"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Time Period, Containers Per Minute";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Options > CPM Time Period";
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Time Period, Production Stop";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Options > Production Stop Time Period";
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Time, Slipped Container Indicator Time";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Shape Handling > Slipped Indicator Time"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Timing, Ejector Timing";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Edit (Select Product) > Inspections > Setup Ejector Timing\n\nor\n\nMain Menu (running) > Setup (Setup Password) > Edit While Running > Disable Ejectors > Setup Ejector Timing";
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Timing Factor, Conveyor Speed Correction";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Edit (Select Product) > Inspections > Setup Ejector Timing > Ejector Timing Factor\n\nor\n\nMain Menu (running) > Setup (Setup Password) > Edit While Running > Disable Ejectors > Setup Ejector Timing > Conveyor Speed Correction";
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Top of Product Image (Height of Image plus Bottom)";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Edit (Select Product) > Size > Top\n\nNote: this can not be changed while inspecting";
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Top, Reference Inspection To Top";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Evaluate (Select Product) (Select Inspection) > Inspection Settings > Step 1 > Reference Inspection To Top\n\nor\n\nMain Menu (running) > Setup (Setup Password) > Evaluate > Evaluate (Select Inspection) > Inspection Settings > Step 1 > Reference Inspection To Top\n\nNote: Reference to Top is only available on Advanced Contaminant and Void Inspections"; 
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Trigger Adjustment, Optical Container Trigger";
	vText[vNumberOfItems] = "Adjust the pot inside the optical sensor module so the\nYellow and Red LEDs are on when No container blocks the beam, and the\nGreen LED is on when the beam is blocked.";
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Trigger, Container Trigger Lockout";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Edit (Select Product) > Size > Container Trigger Lockout\n\nor\n\nMain Menu (running) > Setup (Setup Password) > Edit While Running > Disable Ejectors > Jump To Start > Container Trigger Lockout\n\nNote: Auto Size This Container set the Container Trigger Lockout to the correct value so you should not need to set it here manually";
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Trigger, Container Trigger To Detector Distance";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Edit (Select Product) > Size > Container Trigger To Detector\n\nor\n\nMain Menu (running) > Setup (Setup Password) > Edit While Running > Disable Ejectors > Jump To Start > Container Trigger To Detector";
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Trigger, Continuous Feed Containers";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > System Setup > Setup External Hardware > Setup System Conveyor > Continuous Feed Containers"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = " Trigger, Invert Activity Level of Container Triggers, System";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup External Hardware > Setup Container Triggers > Make Active High/Low\n\nThe Container Trigger should be low when beam not blocked and\nHigh when beam blocked in Oscilloscope Display\nIf backward, toggle Make Active High/Low to other state and check."; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Trigger, Maximum Trigger Standard Deviation, Container Trigger Warning";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > System Setup > Setup External Hardware > Maximum Trigger Std. Dev"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Trigger, System Container Trigger";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup External Hardware > Setup Container Triggers"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Trigger Test";
	vText[vNumberOfItems] = "Main Menu (not running) > Diagnostics > Start (or Stop) Trigger Test"; 
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Tube, Align X-Ray Tube";
	vText[vNumberOfItems] = "Main Menu > Diagnostics > X-Ray > Align X-Ray Tube (Certified Setup Password)"; 
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Tube, Condition X-Ray Tube";
	vText[vNumberOfItems] = "Main Menu > Diagnostics > X-Ray > Condition X-Ray Tube";
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "uController Com Port";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Hardware Options > Setup uController Com Port"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "uController Com Port Connectivity";
	vText[vNumberOfItems] = "Main Menu > Diagnostics > Comm."; 
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Unconfirmed Eject, Stop On Unconfirmed Eject";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup System Options > Stop On Unconfirmed Eject"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Undo Add Reject To Learn";
	vText[vNumberOfItems] = "Main Menu (running) > Rejects > Image Analysis (Setup Password) > Edit While Running > Undo Last Add To Learn\n\nNote: There must be an Add To Learn to Undo";
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Units, Inches / Milli-meters";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > System Setup > Setup External Hardware > Setup Conveyor > Inches / Milli-meters"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Up Stream (Up Stream / Down Stream Interlock Signals)";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup External Hardware > Use Down Stream or Don't Use Down Stream"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Upper Threshold, Density Inspection";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Evaluate (Select Product) (Select Inspection) > Change Upper Threshold\n\nor\n\nMain Menu (running) > Rejects > Image Analysis (Setup Password) > Edit While Running > Change Reject Threshold > +10 or +1 or -1 or -10\n\nNote: On Density Inspection Only"; 
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Values, Pixel Values";
	vText[vNumberOfItems] = "First Measure the Source Strength, then when viewing the Source Strength History shown at the end, press Show Pixel Values.\n\n Or if you have taken a Measure Source Strength since the last power down, Main Menu > Show Production Summary > Product Setup Summary > X-Ray Source Strength > Show Pixel Values";
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "VCO, Use VCO Encoder, (like Bulk Mode but for Pipelines";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup External Hardware > Setup System Conveyor > Use VCO to Encoder or Use Continuous Feed"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Verify X-Ray Detector Edge Calibration";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup X-Ray Detectors > Verify Pixels"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Version, Software";
	vText[vNumberOfItems] = "Main Menu > Diagnostics > Version"; 
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Void Inspection, Enable Void Inspection";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Image Handling > Enable Void Inspection"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Void Inspection, Evaluate Void Inspection";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Evaluate (Select Product) (Select Inspection) > Send Good Containers\n\nor\n\nMain Menu (running) > Setup (Setup Password) > Evaluate > Evaluate (Select Inspection) > Send Good Containers"; 
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Void Inspection, Minimum Defect Size";
	if (TempInspxPasswordOK)
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Evaluate (Select Product) (Select Inspection) > Inspection Settings > Minimum Defect Size\n\nor\n\nMain Menu (running) > Rejects > Image Analysis (Setup Password) > Edit While Running > Edit Inspection Settings > Minimum Defect Size\n\nNote: Only on Contaminant or Foreign Material, Void, and Size Inspections. Not available on Density Inspections"; 
	else
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Evaluate (Select Product) (Select Inspection) > Inspection Settings > Minimum Defect Size\n\nor\n\nMain Menu (running) > Rejects > Image Analysis (Setup Password) > Edit While Running > Evaluate (Select Inspection) > Minimum Size\n\nNote: Only on Contaminant or Foreign Material, Void, and Size Inspections. Not available on Density Inspections"; 
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Void Inspection Threshold";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Evaluate (Select Product) (Select Inspection) > +10 or +1 or -1 or -10\n\nor\n\nMain Menu (running) > Rejects > Image Analysis (Setup Password) > Edit While Running > Change Reject Threshold > +10 or +1 or -1 or -10"; 
	vHidden[vNumberOfItems] = false;


	vNumberOfItems++;
	vItem[vNumberOfItems] = "Voltage Errors, AC Power Monitor";
	vText[vNumberOfItems] = "Main Menu > Diagnostics > Voltages > Show AC Voltage Problems";
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Voltage Error Yellow Messages Enable/Disable, AC Power Monitor";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > System Setup > Setup Hardware Options > Disable/Enable AC Messges"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Voltage Plot, AC Power Monitor";
	vText[vNumberOfItems] = "Main Menu > Diagnostics > Voltages > Show AC Voltage Plot";
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Voltage, X-Ray Source Voltage";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Edit (Select Product) > Exposure > X-Ray Source Voltage\n\nor\n\nNote: this can not be changed while inspecting";
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Voltage, Maximum X-Ray Source Voltage";
		vText[vNumberOfItems] = "Main Menu > Diagnostics > X-Ray > Clear (Inspx Password) > Source Settings > Maximum Voltage/Current";
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Warning Messages, Suppress or Show";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Hardware Options > Suppress (or Show) Warning Messages"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Warning, Container Trigger Warning";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > System Setup > Setup External Hardware > Maximum Trigger Std. Dev"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Warranty or Life Time Counters, Reset";
		vText[vNumberOfItems] = "Main Menu > Diagnostics > Counters (Setup Password) > Clear (Inspx Password) > Appropriate Reset Buttons (Requires Special Reset Password)"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Weight Offset, Check Weighing";
	vText[vNumberOfItems] = "Main Menu > Show Production Summary > Weight Trending > Show Recent > Weight Offset"; 
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "Width of Product Image";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Edit (Select Product) > Size > Width\n\nNote: this can not be changed while inspecting\n\nNote:\n\nNote: Auto Size This Container set the Image Width to the correct value so you should not need to set it here manually";
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Wrong Width, Wrong Height, Wrong Shape";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Shape Handling > Pass (or Eject, or Inspect Containers at Edges"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "XLF, Use Spellman DXM/XLF HVPS";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Hardware Options > Use XLF HVPS"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "X of Y Alarm";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Edit (Select Product) > Inspections > Set X of Y Alarm1 or Set X of Y Alarm2\n\nor\n\nMain Menu (running) > Setup (Setup Password) > Edit While Running > Disable Ejectors > Set X of Y Alarm1 or Set X of Y Alarm2";
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "X-Ray Auto Off, Enable or Disable";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Hardware Options > Enable (or Disable) Auto X-Ray Off"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "X-Ray, Condition X-Ray Tube";
	vText[vNumberOfItems] = "Main Menu > Diagnostics > X-Ray > Condition X-Ray Tube";
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "X-Ray Detectors";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup X-Ray Detectors"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "X-Ray Detector A/D Offset, Calibrate";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup X-Ray Detectors > Auto Find Offset"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "X-Ray Detector Edge Adjust";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup X-Ray Detectors > Edge Adjust"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "X-Ray Detector Edge Calibrate";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup X-Ray Detectors > Auto Calibrate Pixels"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "X-Ray Detector Edge Verify";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup X-Ray Detectors > Verify Pixels"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "X-Ray Detector, Number of X-Ray Detectors";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup X-Ray Detectors > Number of X-Ray Detectors"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "X-Ray Detectors, Complete Calibration, Calibrate X-Ray Detectors";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Calibrate Detectors!"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "X-Ray Detector Pixel Size";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup X-Ray Detectors > X-Ray Detector Pixel Size"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "X-Ray Gain, X-Ray A/D Gain";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup X-Ray Detectors > Change A/D Gains"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "X-Ray Maximum Source Voltage, Current, or Power";
		vText[vNumberOfItems] = "Main Menu > Diagnostics > X-Ray > Clear (Inspx Password) > Source Settings > Maximum Voltage/Current or Maximum Power Allowed";
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "X-Ray Offset, X-Ray A/D Offset";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup X-Ray Detectors > Change A/D Offsets"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "X-Ray Radiation Shutter";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > Setup System > Setup Radiation Shutter"; 
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "X-Ray Shutter, Exercise Radiation Shutter";
	vText[vNumberOfItems] = "Main Menu > Diagnostics > X-Ray > Exercise Shutter 1 Second";
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "X-Ray Source Current";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Edit (Select Product) > Exposure > X-Ray Source Current\n\nor\n\n";
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "X-Ray Source Limits";
		vText[vNumberOfItems] = "Main Menu > Diagnostics > X-Ray > Clear (Inspx Password) > Source Settings";
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "X-Ray Source Monitor Calibration";
		vText[vNumberOfItems] = "Main Menu > Diagnostics > X-Ray > Clear (Inspx Password) > Source Settings";
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "X-Ray Source Strength, Measure";
	vText[vNumberOfItems] = "Main Menu > Diagnostics > X-Ray > Measure Source Strength (Setup Password)";
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "X-Ray Source Temporary Current";
		vText[vNumberOfItems] = "Main Menu > Diagnostics > X-Ray > Clear (Inspx Password) > Change Temporary Current";
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "X-Ray Source Temporary Voltage";
		vText[vNumberOfItems] = "Main Menu > Diagnostics > X-Ray > Clear (Inspx Password) > Change Temporary Voltage";
		vHidden[vNumberOfItems] = true;
	}

	vNumberOfItems++;
	vItem[vNumberOfItems] = "X-Ray Source Voltage";
	vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Edit (Select Product) > Exposure > X-Ray Source Voltage\n\nor\n\nNote: this can not be changed while inspecting";
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "X-Ray Tube Alignment";
	vText[vNumberOfItems] = "Main Menu > Diagnostics > X-Ray > Align X-Ray Tube (Certified Setup Password)"; 
	vHidden[vNumberOfItems] = false;

	vNumberOfItems++;
	vItem[vNumberOfItems] = "X-Ray Tube, Burn In";
	vText[vNumberOfItems] = "Main Menu > Diagnostics > X-Ray > Clear (Inspx Password) > Inspx Hidden Key (Shift) > Burn in Xray Tube";
	vHidden[vNumberOfItems] = false;

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "X-Ray Tube Type - DXM/HVPS";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > System Setup > Setup Hardware Options > Use DXM HVPS";
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "X-Ray Tube Reset Life Time or Warranty Counters";
		vText[vNumberOfItems] = "Main Menu > Diagnostics > Counters (Setup Password) > Clear (Inspx Password) > Appropriate Reset Buttons (Requires Special Reset Password)"; 
		vHidden[vNumberOfItems] = true;
	}

	if (TempInspxPasswordOK)
	{
		vNumberOfItems++;
		vItem[vNumberOfItems] = "Yellow Messages, AC Power Monitor Enable/Disable";
		vText[vNumberOfItems] = "Main Menu > Setup (Setup Password) > Clear (Inspx Password) > System Setup > Setup Hardware Options > Disable/Enable AC Messges"; 
		vHidden[vNumberOfItems] = true;
	}
}


void CFindParameterHelpDialog::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	//m_Background.SetWindowPos(NULL,cMainBackgroundLeft,
	//	cMainBackgroundTop,cMainBackgroundLength,
	//	cMainBackgroundHeight,SWP_NOZORDER);
	SetDlgItemText(IDC_DialogTitleStaticText3, _T("Using the keypad, enter first 3 letters of setting to find"));
	SetDlgItemText(IDC_DialogTitleStaticText2, _T(""));


	LoadSearchStrings();
	UpdateButtons();
	this->SetWindowText(_T("Find"));
	::SetupMenu(vLocalCWndCollection);
}

void CFindParameterHelpDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFindParameterHelpDialog)
	DDX_Control(pDX, IDC_Function5Button, m_Function5Button);
	DDX_Control(pDX, IDC_Function4Button, m_Function4Button);
	DDX_Control(pDX, IDC_Background, m_Background);
	DDX_Control(pDX, IDC_SubFunction8Button, m_SubFunction8Button);
	DDX_Control(pDX, IDC_SubFunction7Button, m_SubFunction7Button);
	DDX_Control(pDX, IDC_SubFunction6Button, m_SubFunction6Button);
	DDX_Control(pDX, IDC_SubFunction5Button, m_SubFunction5Button);
	DDX_Control(pDX, IDC_SubFunction4Button, m_SubFunction4Button);
	DDX_Control(pDX, IDC_SubFunction3Button, m_SubFunction3Button);
	DDX_Control(pDX, IDC_SubFunction2Button, m_SubFunction2Button);
	DDX_Control(pDX, IDC_SubFunction1Button, m_SubFunction1Button);
	DDX_Control(pDX, IDC_DialogTitleStaticText1, m_DialogTitleStaticText);
	DDX_Control(pDX, IDC_DialogTitleStaticText2, m_DialogTitleStaticText2);
	DDX_Control(pDX, IDC_DialogTitleStaticText3, m_DialogTitleStaticText3);
	DDX_Control(pDX, IDC_Function1Button, m_Function1Button);
	DDX_Control(pDX, IDC_Function2Button, m_Function2Button);
	DDX_Control(pDX, IDC_Function3Button, m_Function3Button);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFindParameterHelpDialog, CDialog)
	//{{AFX_MSG_MAP(CFindParameterHelpDialog)
	ON_BN_CLICKED(IDC_Function1Button, OnFunction1Button)
	ON_BN_CLICKED(IDC_Function2Button, OnFunction2Button)
	ON_BN_CLICKED(IDC_Function3Button, OnFunction3Button)
	ON_BN_CLICKED(IDC_Function4Button, OnFunction4Button)
	ON_BN_CLICKED(IDC_Function5Button, OnFunction5Button)
	ON_BN_CLICKED(IDC_SubFunction1Button, OnSubFunction1Button)
	ON_BN_CLICKED(IDC_SubFunction2Button, OnSubFunction2Button)
	ON_BN_CLICKED(IDC_SubFunction3Button, OnSubFunction3Button)
	ON_BN_CLICKED(IDC_SubFunction4Button, OnSubFunction4Button)
	ON_BN_CLICKED(IDC_SubFunction5Button, OnSubFunction5Button)
	ON_BN_CLICKED(IDC_SubFunction7Button, OnSubFunction7Button)
	ON_BN_CLICKED(IDC_SubFunction8Button, OnSubFunction8Button)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_SubFunction6Button, OnSubFunction6Button)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_STN_CLICKED(IDC_DialogTitleStaticText1, &CFindParameterHelpDialog::OnDialogtitlestatictext1)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFindParameterHelpDialog message handlers

void CFindParameterHelpDialog::OnFunction1Button() 
{
	// User clicked Main Menu Button to exit without saving
	CDialog::EndDialog(10);
}

void CFindParameterHelpDialog::OnFunction2Button() 
{
	//user pressed exit button
	CDialog::EndDialog(true);
}

void CFindParameterHelpDialog::OnFunction3Button() 
{
	vLastKeyTimes = 0;
	vLastKey = 0;
	
}

void CFindParameterHelpDialog::OnFunction4Button() 
{
	//previous button pressed to display previous group of items
	if (vFirstItemShown > 1)
	{
		if (!vRecievedButtonPress)
		{
			//ReportErrorMessage("Left Mouse Button Down Easy Select IN FUNCTION 3",cAction, 32000);
			if ((vMouseState == 0) || ((vMouseState == 3) || (vMouseState == 4)))	//if mouse is idle or in double click toggle on mode
			{
				vMouseState = 1;	//set mouse to on while pressed down
				int TimerResult = SetTimer(vMouseDownRepeatTimerHandle,200,NULL);
				if (!TimerResult)
					ReportErrorMessage("Error-Second Timer Failed",cEMailInspx,32000);
			}
		}
		if (vFirstItemShown > 8)
			vFirstItemShown = vFirstItemShown - 8;
		else
			vFirstItemShown = 1;
		UpdateButtons();
	}
}

void CFindParameterHelpDialog::OnFunction5Button() 
{
	//next button pressed to display next group of items
	if (vNumberOfItems > 8 + vFirstItemShown)
	{
		if (!vRecievedButtonPress)
		{
			//ReportErrorMessage("Left Mouse Button Down Easy Select IN FUNCTION 5",cAction, 32000);
			if ((vMouseState == 0) || ((vMouseState == 3) || (vMouseState == 4)))	//if mouse is idle or in double click toggle on mode
			{
				vMouseState = 2;	//set mouse to on while pressed down
				int TimerResult = SetTimer(vMouseDownRepeatTimerHandle,200,NULL);
				if (!TimerResult)
					ReportErrorMessage("Error-Second Timer Failed",cEMailInspx,32000);
			}
		}
		vFirstItemShown = vFirstItemShown + 8;
		UpdateButtons();
	}
}

void CFindParameterHelpDialog::OnSubFunction1Button() 
{
	ShowItem(1);
}

void CFindParameterHelpDialog::OnSubFunction2Button() 
{
	ShowItem(2);
}

void CFindParameterHelpDialog::OnSubFunction3Button() 
{
	ShowItem(3);
}

void CFindParameterHelpDialog::OnSubFunction4Button() 
{
	ShowItem(4);
}

void CFindParameterHelpDialog::OnSubFunction5Button() 
{
	ShowItem(5);
}

void CFindParameterHelpDialog::OnSubFunction6Button() 
{
	ShowItem(6);
}

void CFindParameterHelpDialog::OnSubFunction7Button() 
{
	ShowItem(7);
}

void CFindParameterHelpDialog::OnSubFunction8Button() 
{
	ShowItem(8);
}


BOOL CFindParameterHelpDialog::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	if (pMsg->message == cUpdateTitleBarMessage)
	{
		CWnd *TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText1);
		if (TempWindow)
			TempWindow->Invalidate(false);
		return true;  //true indicates it has been handled, so do not process
	}
	else
	if (pMsg->message == WM_LBUTTONDOWN)
	{
		if (vMouseState > 0)
		{
			//ReportErrorMessage("Screen Clicked, repeat ended",cWriteToLog, 0);
			KillTimer(vMouseDownRepeatTimerHandle);
			vMouseState = 0;
		}
	}
	else
	if (pMsg->message == WM_LBUTTONUP)
	{
		//ReportErrorMessage("Left Button Up",cAction, 32000);
		if (vMouseState < 3)
		{
			if (vMouseState > 0)
			{
				KillTimer(vMouseDownRepeatTimerHandle);
				vMouseState = 0;
			}
		}
	}
	else
	if (pMsg->message == WM_KEYDOWN)
	{
		int TempKey = pMsg->wParam;
		vRecievedButtonPress = true;
		if (TempKey == 189)  //convert a dash
			TempKey = 45;
		if (((TempKey > 105) && (TempKey < 123)) || (TempKey == 27) || (TempKey == cPageDown))
		{
			if (vGlobalShiftKeyDown)
				PostThreadMessage(vGlobalCallingThreadID,cHiddenKeyPressed, 0, pMsg->wParam);
			else
			switch (TempKey)
			{
				case 113: // F2:
					OnSubFunction5Button();
				break;
				case 114: // F3:
					OnSubFunction7Button();
				break;
				case 115: // F4:
					OnSubFunction6Button();
				break;
				case 116: // F5:
					OnSubFunction1Button();
				break;
				case 117: // F6 Key - Function 5:
					OnSubFunction4Button();
				break;
				case 118: // F7 Key - Sub Function 1:
					OnFunction1Button();
				break;
				case 119: // F8 Key - Sub Function 2:
					OnSubFunction8Button();
				break;
				case 120: // F9 Key - Sub Function 3:
					OnSubFunction2Button();
				break;
				case 122: // F11 Key - Sub Function 4:
					OnSubFunction3Button();
				break;
				case 111: // / Key - Sub Function 5:
					OnFunction2Button();
				break;
				case 106: // * Key - Sub Function 6:
					OnFunction4Button();
				break;
				case 109: // - Key - Sub Function 7  Decimal Point:
					OnFunction3Button();
				break;
				case 107: // + Key - Sub Function 8:
					OnFunction5Button();
				break;
				case 16: // Shift Key
				case 0x60:  //0 key on KeyPad
					PostThreadMessage(vGlobalCallingThreadID,cHiddenKeyPressed, 1, 0);
				break;
				case 36:  //Home key, Screen Capture
					vGlobalShiftKeyDown = true;
					PostThreadMessage(vGlobalCallingThreadID,cHiddenKeyPressed, 0, 0x61);
				break;
				case cPageDown: // Help
					//PostThreadMessage(vGlobalCallingThreadID,
					//	cShowHelp, 8, 0);
					CDialog::EndDialog(false);
				break;
				//case cPageUp: // PageUp:
				//	OnKeyClear();
				//break;
				/*
				case cPageUp: // Clear, but do inspx hidden dialog
					if (TempInspxPasswordOK)
					{
						vGlobalTemporaryPasswordOK = false;
						PasswordOK(cSuperInspxPassword,false) = false;
						TempInspxPasswordOK = false;
						LoadSearchStrings();
						UpdateButtons();
						TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText);
						if (TempWindow)
							TempWindow->Invalidate(false);
					}
					else
					{
						if (Pas sw ord OK ())
						{
							CWnd *TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText);
							TempWindow->Invalidate(false);
							LoadSearchStrings();
							UpdateButtons();
						}
					}
				break;	
				*/
				case 27: // Escape
					::PostThreadMessage(vGlobalCallingThreadID,	cStartChatWindowMessage, 0,0);
				break;	
			}
		}
		else
		//get an alpha numeric key press
		if (((TempKey >= 32) && (TempKey <= 126)) || 
			(TempKey == 8) || (TempKey == 46))
		{

			if (pMsg->wParam == 33) //Delete key
			{
				OnKeyClear();
			}
			else
			if (pMsg->wParam == 8) //back space key
			{
				vCursorPosition--;
				SetCursor();
				OnKeyClear();
			}
			else
				ProcessKey(TempKey);
		}
		vRecievedButtonPress = true;
		return true;  //true indicates it has been handled, so do not process
	}
	return CDialog::PreTranslateMessage(pMsg);
}

HBRUSH CFindParameterHelpDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	HWND TempDialogHwnd = this->m_hWnd;
	int TextSize;
	pDC->SetAttribDC(pDC->m_hDC);
	
	if (pWnd->GetDlgCtrlID() == IDC_DialogTitleStaticText1)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DialogTitleStaticText, 8);
		pDC->SetTextColor(vGlobalDialogTitleColor);
	}

	if (pWnd->GetDlgCtrlID() == IDC_Background)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBlack);
		return vWhiteBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DialogTitleStaticText3)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DialogTitleStaticText3, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBlack);
		return vWhiteBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DialogTitleStaticText2)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DialogTitleStaticText2, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBlack);
		return vWhiteBrush;
	}
	//if (pWnd->GetDlgCtrlID() == IDC_HelpText)
	//{
	//	TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_HelpText, 5);  //5 is medium large
	//	pDC->SetBkMode(TRANSPARENT);
	//	pDC->SetTextColor(cBlack);
	//	return vWhiteBrush;
	//}

	if (pWnd->GetDlgCtrlID() == IDC_HelpTitle)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBlack);
			return vBlueBrush;
  }

	if (pWnd->GetDlgCtrlID() == IDC_Function1Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function1Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Function2Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function2Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Function3Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function3Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Function4Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function4Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Function5Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function5Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction1Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction1Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction2Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction2Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction3Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction3Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction4Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction4Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction5Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction5Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction6Button) 
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction6Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction7Button) 
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction7Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction8Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction8Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	return hbr;
}

void CFindParameterHelpDialog::ShowItem(WORD TempItemNumber)
{
	if (vItemShowingOnButton[TempItemNumber])
	{
		CHelpDialog * IHelpDialog;
		IHelpDialog = new(CHelpDialog);
		IHelpDialog->vHelpContext = 27;
		IHelpDialog->vHelpType = 0;
		IHelpDialog->vLocalSystemData = vLocalSystemData;
		IHelpDialog->vLocalConfigurationData = vLocalConfigurationData;
		IHelpDialog->vWindowType = cTextWindow;
		//IHelpDialog->vLocalSystemData = vLocalSystemData;
		//IHelpDialog->vMainWindowPointer = vMainWindowPointer;
		//IHelpDialog->vLocalConfigurationData = vLocalConfigurationData;
		//IHelpDialog->vProductPointer = vGlobalCurrentProduct;
		IHelpDialog->vTextString = "\n\n" + vText[vItemShowingOnButton[TempItemNumber]];
		CString TempString(vItem[vItemShowingOnButton[TempItemNumber]]);
		if (vItem[vItemShowingOnButton[TempItemNumber]].GetLength() < 36)
			TempString = "Location of: " + TempString;
		if (vHidden[vItemShowingOnButton[TempItemNumber]])
			TempString = TempString + "*";

		IHelpDialog->vTitleString = TempString;
		int TempResult = IHelpDialog->DoModal();

		delete IHelpDialog;
	}
}

void CFindParameterHelpDialog::UpdateButtons()
{
	CWnd* pWnd;
	CDC* pDC;
	HWND TempDialogHwnd = this->m_hWnd;

	if (vFirstItemShown > 1)
	{
		m_Function4Button.ShowWindow(SW_SHOW);
	}
	else
	{
		m_Function4Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_Function4Button.GetRectToInvalidateArrow());
	}

	if (vFirstItemShown < vNumberOfItems - 8)
	{
		m_Function5Button.ShowWindow(SW_SHOW);
	}
	else
	{
		m_Function5Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_Function5Button.GetRectToInvalidateArrow());
	}

	for (BYTE TempLoop = 0; TempLoop < 10; TempLoop++)
		vItemShowingOnButton[TempLoop] = 0;

	BYTE TempButtonNumberToDisplayNext = 1;
	WORD TempItemToShowOnButton = vFirstItemShown;

	while (TempButtonNumberToDisplayNext < 9)
	{
		if (TempItemToShowOnButton > vNumberOfItems)
			TempButtonNumberToDisplayNext = 9;  //no more items to show, so end while loop
		else
		{ //show this item
			vItemShowingOnButton[TempButtonNumberToDisplayNext] = TempItemToShowOnButton;
			TempButtonNumberToDisplayNext++;
			TempItemToShowOnButton++;
		}
	}
	CString TempBlankSpace("  ");

	::SetupMenu(vLocalDisplayCollection);
	if (vItemShowingOnButton[1])
	{
		m_SubFunction1Button.ShowWindow(SW_SHOW);
		CString TempItemName = TempBlankSpace + vItem[vItemShowingOnButton[1]];
		if (vHidden[vItemShowingOnButton[1]])
			TempItemName = TempItemName + "*";
		SetDlgItemText(IDC_SubFunction1Button,TempItemName);

		pDC = m_SubFunction1Button.GetDC();
		pWnd = &m_SubFunction1Button;
 		int TempStringLength = GetTextSizeForASingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction1Button, 5) + 30;  //5 is medium large
		if (TempStringLength > (int)(550 * vGlobalPercentToResizeY))
			TempStringLength = (int)(550 * vGlobalPercentToResizeY);
		m_SubFunction1Button.SetWindowPos(NULL, 11, 52, (int)(TempStringLength / vGlobalPercentToResizeY), 28, SWP_NOZORDER);
	}
	else
	{
		m_SubFunction1Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction1Button.GetRectToInvalidateArrow());
	}

	if (vItemShowingOnButton[2])
	{
		m_SubFunction2Button.ShowWindow(SW_SHOW);
		CString TempItemName = TempBlankSpace + vItem[vItemShowingOnButton[2]];
		if (vHidden[vItemShowingOnButton[2]])
			TempItemName = TempItemName + "*";
		SetDlgItemText(IDC_SubFunction2Button,TempItemName);

		pDC = m_SubFunction2Button.GetDC();
		pWnd = &m_SubFunction2Button;
 		int TempStringLength = GetTextSizeForASingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction2Button, 5) + 30;  //5 is medium large
		if (TempStringLength > (int)(550 * vGlobalPercentToResizeY))
			TempStringLength = (int)(550 * vGlobalPercentToResizeY);
		m_SubFunction2Button.SetWindowPos(NULL, 11, 142, (int)(TempStringLength / vGlobalPercentToResizeY), 28, SWP_NOZORDER);
	}
	else
	{
		m_SubFunction2Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction2Button.GetRectToInvalidateArrow());
	}

	if (vItemShowingOnButton[3])
	{
		m_SubFunction3Button.ShowWindow(SW_SHOW);
		CString TempItemName = TempBlankSpace + vItem[vItemShowingOnButton[3]];
		if (vHidden[vItemShowingOnButton[3]])
			TempItemName = TempItemName + "*";
		SetDlgItemText(IDC_SubFunction3Button,TempItemName);

		pDC = m_SubFunction3Button.GetDC();
		pWnd = &m_SubFunction3Button;
 		int TempStringLength = GetTextSizeForASingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction3Button, 5) + 30;  //5 is medium large
		if (TempStringLength > (int)(550 * vGlobalPercentToResizeY))
			TempStringLength = (int)(550 * vGlobalPercentToResizeY);
		m_SubFunction3Button.SetWindowPos(NULL, 11, 232, (int)(TempStringLength / vGlobalPercentToResizeY), 28, SWP_NOZORDER);
	}
	else
	{
		m_SubFunction3Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction3Button.GetRectToInvalidateArrow());
	}

	if (vItemShowingOnButton[4])
	{
		m_SubFunction4Button.ShowWindow(SW_SHOW);
		CString TempItemName = TempBlankSpace + vItem[vItemShowingOnButton[4]];
		if (vHidden[vItemShowingOnButton[4]])
			TempItemName = TempItemName + "*";
		SetDlgItemText(IDC_SubFunction4Button,TempItemName);

		pDC = m_SubFunction4Button.GetDC();
		pWnd = &m_SubFunction4Button;
 		int TempStringLength = GetTextSizeForASingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction4Button, 5) + 30;  //5 is medium large
		if (TempStringLength > (int)(550 * vGlobalPercentToResizeY))
			TempStringLength = (int)(550 * vGlobalPercentToResizeY);
		m_SubFunction4Button.SetWindowPos(NULL, 11, 327, (int)(TempStringLength / vGlobalPercentToResizeY), 28, SWP_NOZORDER);
	}
	else
	{
		m_SubFunction4Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction4Button.GetRectToInvalidateArrow());
	}

	if (vItemShowingOnButton[5])
	{
		m_SubFunction5Button.ShowWindow(SW_SHOW);
		CString TempItemName = vItem[vItemShowingOnButton[5]];
		if (vHidden[vItemShowingOnButton[5]])
			TempItemName = TempItemName + "*";
		TempItemName = TempItemName + TempBlankSpace;
		SetDlgItemText(IDC_SubFunction5Button,TempItemName);

		pDC = m_SubFunction5Button.GetDC();
		pWnd = &m_SubFunction5Button;
 		int TempStringLength = GetTextSizeForASingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction5Button, 5) + 30;  //5 is medium large
		if (TempStringLength > (int)(550 * vGlobalPercentToResizeY))
			TempStringLength = (int)(550 * vGlobalPercentToResizeY);
		m_SubFunction5Button.SetWindowPos(NULL, 623 - (int)(TempStringLength / vGlobalPercentToResizeY), 79, (int)(TempStringLength / vGlobalPercentToResizeY), 28, SWP_NOZORDER);
	}
	else
	{
		m_SubFunction5Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction5Button.GetRectToInvalidateArrow());
	}

	if (vItemShowingOnButton[6])
	{
		m_SubFunction6Button.ShowWindow(SW_SHOW);
		CString TempItemName = vItem[vItemShowingOnButton[6]];
		if (vHidden[vItemShowingOnButton[6]])
			TempItemName = TempItemName + "*";
		TempItemName = TempItemName + TempBlankSpace;
		SetDlgItemText(IDC_SubFunction6Button,TempItemName);

		pDC = m_SubFunction6Button.GetDC();
		pWnd = &m_SubFunction6Button;
 		int TempStringLength = GetTextSizeForASingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction6Button, 5) + 30;  //5 is medium large
		if (TempStringLength > (int)(550 * vGlobalPercentToResizeY))
			TempStringLength = (int)(550 * vGlobalPercentToResizeY);
		m_SubFunction6Button.SetWindowPos(NULL, 623 - (int)(TempStringLength / vGlobalPercentToResizeY), 169, (int)(TempStringLength / vGlobalPercentToResizeY), 28, SWP_NOZORDER);
	}
	else
	{
		m_SubFunction6Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction6Button.GetRectToInvalidateArrow());
	}

	if (vItemShowingOnButton[7])
	{
		m_SubFunction7Button.ShowWindow(SW_SHOW);
		CString TempItemName = vItem[vItemShowingOnButton[7]];
		if (vHidden[vItemShowingOnButton[7]])
			TempItemName = TempItemName + "*";
		TempItemName = TempItemName + TempBlankSpace;
		SetDlgItemText(IDC_SubFunction7Button,TempItemName);

		pDC = m_SubFunction7Button.GetDC();
		pWnd = &m_SubFunction7Button;
 		int TempStringLength = GetTextSizeForASingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction7Button, 5) + 30;  //5 is medium large
		if (TempStringLength > (int)(550 * vGlobalPercentToResizeY))
			TempStringLength = (int)(550 * vGlobalPercentToResizeY);
		m_SubFunction7Button.SetWindowPos(NULL, 623 - (int)(TempStringLength / vGlobalPercentToResizeY), 259, (int)(TempStringLength / vGlobalPercentToResizeY), 28, SWP_NOZORDER);
	}
	else
	{
		m_SubFunction7Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction7Button.GetRectToInvalidateArrow());
	}

	if (vItemShowingOnButton[8])
	{
		m_SubFunction8Button.ShowWindow(SW_SHOW);
		CString TempItemName = vItem[vItemShowingOnButton[8]];
		if (vHidden[vItemShowingOnButton[8]])
			TempItemName = TempItemName + "*";
		TempItemName = TempItemName + TempBlankSpace;
		SetDlgItemText(IDC_SubFunction8Button,TempItemName);

		pDC = m_SubFunction8Button.GetDC();
		pWnd = &m_SubFunction8Button;
 		int TempStringLength = GetTextSizeForASingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction8Button, 5) + 30;  //5 is medium large
		if (TempStringLength > (int)(550 * vGlobalPercentToResizeY))
			TempStringLength = (int)(550 * vGlobalPercentToResizeY);
		m_SubFunction8Button.SetWindowPos(NULL, 623 - (int)(TempStringLength / vGlobalPercentToResizeY), 354, (int)(TempStringLength / vGlobalPercentToResizeY), 28, SWP_NOZORDER);
	}
	else
	{
		m_SubFunction8Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction8Button.GetRectToInvalidateArrow());
	}

	if (!vFirstOpenedFindParameterHelp)
	{
		::SetupMenu(vLocalDisplayCollection);
	}
	else
	{
		vFirstOpenedFindParameterHelp = false;
	}
}

BOOL CFindParameterHelpDialog::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::DestroyWindow();
}

void CFindParameterHelpDialog::OnDestroy() 
{
	CDialog::OnDestroy();
	DeleteObject(vWhiteBrush);
	DeleteObject(vBlueBrush);
	DeleteObject(vYellowBrush);
	
	// TODO: Add your message handler code here
	
}

void CFindParameterHelpDialog::OnKeyClear() 
{
	//if clear key pressed
	WORD TempLength = vSearchPattern.GetLength();
	if (vCursorPosition >= TempLength - 1)  // - 1 for the &
	{
		vSearchPattern.Delete(vCursorPosition - 1,1);
		if (vCursorPosition > 0)
			vCursorPosition--;
	}
	else
	{
		vSearchPattern.Delete(vCursorPosition + 1,1); //vCursorPosition is at the &
		if (vCursorPosition == TempLength)
			vCursorPosition--;
	}
	SetDlgItemText(IDC_EditStaticText,vSearchPattern);
	SetCursor();
	vLastKey = 0;
	vLastKeyTimes = 0;
}

void CFindParameterHelpDialog::SetCursor()
{
	WORD TempOldCursorPosition = vSearchPattern.Find(_T("&"));
	if (TempOldCursorPosition >= 0)
	{
		vSearchPattern.Delete(TempOldCursorPosition,1);
	}
	if (vCursorPosition > vSearchPattern.GetLength())
	{
		vCursorPosition = vSearchPattern.GetLength();
	}
	vSearchPattern.Insert(vCursorPosition, _T("&"));
	SetDlgItemText(IDC_DialogTitleStaticText2,vSearchPattern);
	//UpdateData(false);

	//set first item to see

	vFirstItemShown = 1;
	if (vSearchPattern != "&")
	{
		CString TempSearchPattern = vSearchPattern;
		BYTE TempCursorLocation = TempSearchPattern.Find(_T("&"),0);
		if (TempCursorLocation < 255)
			TempSearchPattern.Delete(TempCursorLocation,1);
		BYTE TempLength = TempSearchPattern.GetLength();
		CString TempTargetString = vItem[vFirstItemShown];
		TempTargetString.Delete(TempLength,TempTargetString.GetLength() - TempLength);
		TempTargetString.MakeUpper();
		while ((TempSearchPattern > TempTargetString) && 
			((vFirstItemShown + 1) < vNumberOfItems))
		{
			vFirstItemShown++;
			TempTargetString = vItem[vFirstItemShown];
			TempTargetString.Delete(TempLength,TempTargetString.GetLength() - TempLength);
			TempTargetString.MakeUpper();
		}
	}
	UpdateButtons();
}


bool CFindParameterHelpDialog::ProcessKey(BYTE TempKey)
{
	bool TempReturn = false;
	{
		if ((vLastKey == TempKey) && ((TempKey >= c0) && (TempKey <= c9)))
		{
			WORD TempLength = vSearchPattern.GetLength();
			if (vCursorPosition > 0)
			{
				vSearchPattern.Delete(vCursorPosition - 1,1);
				vCursorPosition--;
			}
			vLastKeyTimes++;
			//the zero key only has 3 items on it!!! so skip an item
			if ((TempKey == c0) && (vLastKeyTimes ==2))
							vLastKeyTimes++;

			if (vLastKeyTimes > 3)
				vLastKeyTimes = 0;
		}
		//hitting new key, so reset key hit counters
		else
		{
			vLastKeyTimes = 0;
			vLastKey = TempKey;
		}
		{
			WORD TempIndex = 0;

			if ((TempKey >= c0) && (TempKey <= c9))
			{
				TempIndex = TempKey - c0 + c0Key;
				vSearchPattern.Insert(vCursorPosition,(CHAR)vLocalKey[TempIndex + (vLastKeyTimes * cMaxKeys)]);
				vCursorPosition++;
				SetCursor();
				TempReturn = true;
			}
			else
			if ((TempKey >= ' ') && (TempKey <= '~'))
			{
				TempIndex = TempKey - c0 + c0Key;
				vSearchPattern.Insert(vCursorPosition,(CHAR)TempKey);
				vCursorPosition++;
				SetCursor();
				TempReturn = true;
			}
		}
	}
	return TempReturn;
}
/*
				if (vLastKey == TempKey)
			{
				WORD TempLength = vSearchPattern.GetLength();
				if (vCursorPosition > 0)
				{
					vSearchPattern.Delete(vCursorPosition - 1,1);
					vCursorPosition--;
				}
				vLastKeyTimes++;
				//the zero key only has 3 items on it!!! so skip an item
				if ((TempKey == c0) && (vLastKeyTimes ==2))
								vLastKeyTimes++;

				if (vLastKeyTimes > 3)
					vLastKeyTimes = 0;
			}
			//hitting new key, so reset key hit counters
			else
			{
				//make sure string plus & is not over 33 characters
				// so have room to add one more
				WORD TempLength = vSearchPattern.GetLength();
				vLastKeyTimes = 0;
				vLastKey = TempKey;
			}
			SetCursor();

			*/

void CFindParameterHelpDialog::OnDialogtitlestatictext1()
{
	//bring up soft key menu for help, print screen, Image, Clear, and hidden shift key
	BYTE TempHiddenKeys = 8;
	if (!PasswordOK(cTemporaryInspxPassword,false))
		TempHiddenKeys = 10;
	PostThreadMessage(vGlobalCallingThreadID, cDisplaySoftKeyMenu, TempHiddenKeys, TempHiddenKeys);
}

void CFindParameterHelpDialog::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == vMouseDownRepeatTimerHandle)
	{
	}

	CDialog::OnTimer(nIDEvent);
}
