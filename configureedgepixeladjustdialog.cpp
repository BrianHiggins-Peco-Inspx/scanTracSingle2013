//ScanTrac Side View Source File
// ConfigureEdgePixelAdjustDialog.cpp : implementation file
//

#include "stdafx.h"
#include "ScanTrac.h"
#include "ConfigureEdgePixelAdjustDialog.h"
#include "NumericEntryDialog.h"
#include "YesNoDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern BYTE vGlobalShiftKeyDown;
extern unsigned long vGlobalDialogTitleColor;
/////////////////////////////////////////////////////////////////////////////
// CConfigureEdgePixelAdjustDialog dialog


CConfigureEdgePixelAdjustDialog::CConfigureEdgePixelAdjustDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CConfigureEdgePixelAdjustDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConfigureEdgePixelAdjustDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	vUsePoint4mmXScanDetectors = 0; //1 is a multiplier of 1 so no change to the number of pixels per detector
	vUsePoint4mmXScanDetectorsMultiplier = 1; //1 is a multiplier of 1 so no change to the number of pixels per detector
	vNumberOfPixelsToAdjust = 6;
	vChangeMade = false;
	vFirst = 0;
	vCorrectDetectorEdges = 1;
	//vNumberOfDetectorsToCalibrate = 0;
	vNumberOfPixelAtBottomToIgnoreInCalibration = 10;
	vLocalCWndCollection.Add(this					);
	vLocalCWndCollection.Add(&m_Function3Display		);
	vLocalCWndCollection.Add(&m_Function5Button		);
	vLocalCWndCollection.Add(&m_Function3Button		);
	vLocalCWndCollection.Add(&m_Function4Button		);
	vLocalCWndCollection.Add(&m_Function2Button		);
	vLocalCWndCollection.Add(&m_Background			);
	vLocalCWndCollection.Add(&m_DialogTitleStaticText);
	vLocalCWndCollection.Add(&m_Function1Button		);
	vLocalCWndCollection.Add(&m_Pixel1				);
	vLocalCWndCollection.Add(&m_Pixel2				);
	vLocalCWndCollection.Add(&m_Pixel3				);
	vLocalCWndCollection.Add(&m_Pixel4				);
	vLocalCWndCollection.Add(&m_Pixel5				);
	vLocalCWndCollection.Add(&m_Pixel6				);
	vLocalCWndCollection.Add(&m_Pixel7				);
	vLocalCWndCollection.Add(&m_Pixel8				);
	vLocalCWndCollection.Add(&m_SubFunction1Button	);
	vLocalCWndCollection.Add(&m_SubFunction2Button	);
	vLocalCWndCollection.Add(&m_SubFunction3Button	);
	vLocalCWndCollection.Add(&m_SubFunction4Button	);
	vLocalCWndCollection.Add(&m_SubFunction5Button	);
	vLocalCWndCollection.Add(&m_SubFunction6Button	);
	vLocalCWndCollection.Add(&m_SubFunction7Button	);
	vLocalCWndCollection.Add(&m_SubFunction8Button	);
}

void CConfigureEdgePixelAdjustDialog::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	m_Background.SetWindowPos(NULL,cMainBackgroundLeft, cMainBackgroundTop,cMainBackgroundLength, cMainBackgroundHeight,SWP_NOZORDER);
	
	vCorrectDetectorEdges = vLocalConfigurationData->vCorrectDetectorEdges;
	//vNumberOfDetectorsToCalibrate = vLocalConfigurationData->vNumberOfDetectorsToCalibrate;
	vNumberOfPixelAtBottomToIgnoreInCalibration = vLocalConfigurationData->vNumberOfPixelAtBottomToIgnoreInCalibration;

	vNumberOfPixelsToAdjust = vLocalConfigurationData->vPixelsPerDetector * vUsePoint4mmXScanDetectorsMultiplier * vLocalConfigurationData->vNumberOfXRayDetectors;

	vCorrectDetectorEdges = vLocalConfigurationData->vCorrectDetectorEdges;

	for (WORD TempLoop = 0; TempLoop < vNumberOfPixelsToAdjust; TempLoop++)
		vPixelNumberString[TempLoop] = dtoa(TempLoop,0);

	for (WORD TempLoop = 0; TempLoop < cNumberOfPixelsToAdjustAllPixels1152; TempLoop++)
	{
		vPixelB[TempLoop] = vLocalConfigurationData->vDetectorEdgeAdjustOffset[TempLoop][vUsePoint4mmXScanDetectors];
		vPixelM[TempLoop] = vLocalConfigurationData->vDetectorEdgeAdjustGain[TempLoop][vUsePoint4mmXScanDetectors];
		vOverWriteBadPixel[TempLoop] = vLocalConfigurationData->vOverWriteBadPixel[TempLoop][vUsePoint4mmXScanDetectors];
	}
/*
	if (vLocalConfigurationData->vPixelsPerDetector == 64)
	{
		vPixelNumberString[0] = "1";
		vPixelNumberString[1] = "2";
		vPixelNumberString[2] = "3";
		vPixelNumberString[3] = "62";
		vPixelNumberString[4] = "63";
		vPixelNumberString[5] = "64";
		vPixelNumberString[6] = "65";
		vPixelNumberString[7] = "66";
		vPixelNumberString[8] = "67";
		vPixelNumberString[9] = "126";
		vPixelNumberString[10] = "127";
		vPixelNumberString[11] = "128";
		vPixelNumberString[12] = "129";
		vPixelNumberString[13] = "130";
		vPixelNumberString[14] = "131";
		vPixelNumberString[15] = "190";
		vPixelNumberString[16] = "191";
		vPixelNumberString[17] = "192";
		vPixelNumberString[18] = "193";
		vPixelNumberString[19] = "194";
		vPixelNumberString[20] = "195";
		vPixelNumberString[21] = "254";
		vPixelNumberString[22] = "255";
		vPixelNumberString[23] = "256";
		vPixelNumberString[24] = "257";
		vPixelNumberString[25] = "258";
		vPixelNumberString[26] = "259";
		vPixelNumberString[27] = "318";
		vPixelNumberString[28] = "319";
		vPixelNumberString[29] = "320";
		vPixelNumberString[30] = "321";
		vPixelNumberString[31] = "322";
		vPixelNumberString[32] = "323";
		vPixelNumberString[33] = "382";
		vPixelNumberString[34] = "383";
		vPixelNumberString[35] = "384";
		vPixelNumberString[36] = "385";
		vPixelNumberString[37] = "386";
		vPixelNumberString[38] = "387";
		vPixelNumberString[39] = "446";
		vPixelNumberString[40] = "447";
		vPixelNumberString[41] = "448";
		vPixelNumberString[42] = "449";
		vPixelNumberString[43] = "450";
		vPixelNumberString[44] = "451";
		vPixelNumberString[45] = "510";
		vPixelNumberString[46] = "511";
		vPixelNumberString[47] = "512";
	}
	else
	if (vLocalConfigurationData->vPixelsPerDetector == 128)
	{
		vPixelNumberString[0] = "1";
		vPixelNumberString[1] = "2";
		vPixelNumberString[2] = "3";
		vPixelNumberString[3] = "126";
		vPixelNumberString[4] = "127";
		vPixelNumberString[5] = "128";
		vPixelNumberString[6] = "129";
		vPixelNumberString[7] = "130";
		vPixelNumberString[8] = "131";
		vPixelNumberString[9] = "254";
		vPixelNumberString[10] = "255";
		vPixelNumberString[11] = "256";
		vPixelNumberString[12] = "257";
		vPixelNumberString[13] = "258";
		vPixelNumberString[14] = "259";
		vPixelNumberString[15] = "382";
		vPixelNumberString[16] = "383";
		vPixelNumberString[17] = "384";
		vPixelNumberString[18] = "385";
		vPixelNumberString[19] = "386";
		vPixelNumberString[20] = "387";
		vPixelNumberString[21] = "510";
		vPixelNumberString[22] = "511";
		vPixelNumberString[23] = "512";
		vPixelNumberString[24] = "513";
		vPixelNumberString[25] = "514";
		vPixelNumberString[26] = "515";
		vPixelNumberString[27] = "638";
		vPixelNumberString[28] = "639";
		vPixelNumberString[29] = "640";
		vPixelNumberString[30] = "641";
		vPixelNumberString[31] = "642";
		vPixelNumberString[32] = "643";
		vPixelNumberString[33] = "766";
		vPixelNumberString[34] = "767";
		vPixelNumberString[35] = "768";
		vPixelNumberString[36] = "769";
		vPixelNumberString[37] = "770";
		vPixelNumberString[38] = "771";
		vPixelNumberString[39] = "894";
		vPixelNumberString[40] = "895";
		vPixelNumberString[41] = "896";
		vPixelNumberString[42] = "897";
		vPixelNumberString[43] = "898";
		vPixelNumberString[44] = "899";
		vPixelNumberString[45] = "1022";
		vPixelNumberString[46] = "1023";
		vPixelNumberString[47] = "1024";
	}
	*/
	DisplayButtonLabels();
	ShowAdjustPixelValues();
	this->SetWindowText( _T("Edge Pixels"));
	SetupMenu(vLocalCWndCollection);
}

void CConfigureEdgePixelAdjustDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfigureEdgePixelAdjustDialog)
	DDX_Control(pDX, IDC_Function3Display, m_Function3Display);
	DDX_Control(pDX, IDC_Function5Button, m_Function5Button);
	DDX_Control(pDX, IDC_Function3Button, m_Function3Button);
	DDX_Control(pDX, IDC_Function4Button, m_Function4Button);
	DDX_Control(pDX, IDC_Function2Button, m_Function2Button);
	DDX_Control(pDX, IDC_Background, m_Background);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_DialogTitleStaticText1, m_DialogTitleStaticText);
	DDX_Control(pDX, IDC_Function1Button, m_Function1Button);
	DDX_Control(pDX, IDC_Pixel1, m_Pixel1);
	DDX_Control(pDX, IDC_Pixel2, m_Pixel2);
	DDX_Control(pDX, IDC_Pixel3, m_Pixel3);
	DDX_Control(pDX, IDC_Pixel4, m_Pixel4);
	DDX_Control(pDX, IDC_Pixel5, m_Pixel5);
	DDX_Control(pDX, IDC_Pixel6, m_Pixel6);
	DDX_Control(pDX, IDC_Pixel7, m_Pixel7);
	DDX_Control(pDX, IDC_Pixel8, m_Pixel8);
	DDX_Control(pDX, IDC_SubFunction1Button, m_SubFunction1Button);
	DDX_Control(pDX, IDC_SubFunction2Button, m_SubFunction2Button);
	DDX_Control(pDX, IDC_SubFunction3Button, m_SubFunction3Button);
	DDX_Control(pDX, IDC_SubFunction4Button, m_SubFunction4Button);
	DDX_Control(pDX, IDC_SubFunction5Button, m_SubFunction5Button);
	DDX_Control(pDX, IDC_SubFunction6Button, m_SubFunction6Button);
	DDX_Control(pDX, IDC_SubFunction7Button, m_SubFunction7Button);
	DDX_Control(pDX, IDC_SubFunction8Button, m_SubFunction8Button);
}


BEGIN_MESSAGE_MAP(CConfigureEdgePixelAdjustDialog, CDialog)
	//{{AFX_MSG_MAP(CConfigureEdgePixelAdjustDialog)
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
	ON_BN_CLICKED(IDC_SubFunction6Button, OnSubFunction6Button)
	ON_BN_CLICKED(IDC_SubFunction7Button, OnSubFunction7Button)
	ON_BN_CLICKED(IDC_SubFunction8Button, OnSubFunction8Button)
	ON_WM_SHOWWINDOW()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
	ON_STN_CLICKED(IDC_DialogTitleStaticText1, &CConfigureEdgePixelAdjustDialog::OnDialogtitlestatictext1)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfigureEdgePixelAdjustDialog message handlers

void CConfigureEdgePixelAdjustDialog::OnFunction1Button() 
{
	//Main Menu was pressed
	if ((!vChangeMade) || (AbortSetupChanges()))
		CDialog::EndDialog(10);
}

void CConfigureEdgePixelAdjustDialog::OnFunction2Button() 
{
	//Save and exit was pressed
	if ((!vChangeMade) || (KeepSetupChanges()))
	{
		vLocalConfigurationData->vCorrectDetectorEdges = vCorrectDetectorEdges;
		//vLocalConfigurationData->vNumberOfDetectorsToCalibrate = vNumberOfDetectorsToCalibrate;
		vLocalConfigurationData->vNumberOfPixelAtBottomToIgnoreInCalibration = vNumberOfPixelAtBottomToIgnoreInCalibration;

		for (WORD TempLoop = 0; TempLoop < cNumberOfPixelsToAdjustAllPixels1152; TempLoop++)
		{
			vLocalConfigurationData->vDetectorEdgeAdjustOffset[TempLoop][vUsePoint4mmXScanDetectors] = vPixelB[TempLoop];
			vLocalConfigurationData->vDetectorEdgeAdjustGain[TempLoop][vUsePoint4mmXScanDetectors] = vPixelM[TempLoop];
			vLocalConfigurationData->vOverWriteBadPixel[TempLoop][vUsePoint4mmXScanDetectors] = vOverWriteBadPixel[TempLoop];
		}
		bool TempHaveCopiedPixelsConfigured = false;
		for (WORD TempLoop = 1; TempLoop < vNumberOfPixelsToAdjust; TempLoop++)
		{
			if (vLocalConfigurationData->vOverWriteBadPixel[TempLoop][0] != 0)
				TempHaveCopiedPixelsConfigured = true;
		}
		if ((vLocalConfigurationData->vUseXScanDetectors) && (vLocalConfigurationData->vScanTracType != cForteScanTrac) && (vLocalConfigurationData->vScanTracType != cCaseInspectorScanTrac))
		if (!TempHaveCopiedPixelsConfigured)
		for (WORD TempLoop = 1; TempLoop < vNumberOfPixelsToAdjust; TempLoop++)
		{
			if (vLocalConfigurationData->vOverWriteBadPixel[TempLoop][1] != 0)
				TempHaveCopiedPixelsConfigured = true;
		}

		if (TempHaveCopiedPixelsConfigured)
		{
			CYesNoDialog TempYesNoDialog;
			TempYesNoDialog.vNoticeText = _T("\n\nThere are Pixels configured to be copied over.\nDo you want to copy over those pixels?");
			TempYesNoDialog.vYesButtonText = "Copy Pixels";
			TempYesNoDialog.vNoButtonText = "Don't Copy Pixels";
			TempYesNoDialog.vQuestionType = cConfirmQuestion;
			int TempResult = TempYesNoDialog.DoModal();
			if (TempResult == IDOK)
				vLocalConfigurationData->vOverWriteBadPixel[0][0] = 0xAAAB;
			else
				vLocalConfigurationData->vOverWriteBadPixel[0][0] = 0xAAAA;
		}
		else
			vLocalConfigurationData->vOverWriteBadPixel[0][0] = 0xAAAA;

		vLocalConfigurationData->vOverWriteBadPixel[0][1] = vLocalConfigurationData->vOverWriteBadPixel[0][0];
		CDialog::EndDialog(true);
	}
}

void CConfigureEdgePixelAdjustDialog::OnFunction3Button() 
{
	// Previous button pressed
	if (vFirst)
	{
		if (vFirst > 7)
			vFirst = vFirst - 8;
		else
			vFirst = 0;
		DisplayButtonLabels();
		ShowAdjustPixelValues();
	}
	else
	{
		vCorrectDetectorEdges++;
		if (vCorrectDetectorEdges > 2)
			vCorrectDetectorEdges = 0;
		if (vCorrectDetectorEdges == 2)
		{
			
			CNumericEntryDialog INumericEntryDialog;  
			
			////Set dialog box data titles and number value 1st
			//INumericEntryDialog.vEditString = dtoa(vNumberOfDetectorsToCalibrate,0);

			//INumericEntryDialog.m_DialogTitleStaticText1 = "Enter the number of X-Ray Detectors to Calibrate";
			//INumericEntryDialog.m_DialogTitleStaticText2 = "Calibrates detectors at the bottom of the image";
			//INumericEntryDialog.m_DialogTitleStaticText3 = "Uncalibrated detectors at the top of the image";
			//INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + 
			//	INumericEntryDialog.vEditString;

			//INumericEntryDialog.m_UnitsString = "0 is Disabled, all detectors calibrated";
			//INumericEntryDialog.vMaxValue = vLocalConfigurationData->vNumberOfXRayDetectors;
			//INumericEntryDialog.vIntegerOnly = true;
			////Pass control to dialog box and display
			//int nResponse = INumericEntryDialog.DoModal();
			////dialog box is now closed, if user pressed select do this
			////if user pressed cancel, do nothing
			//if (nResponse == IDOK)
			//{
				//vNumberOfDetectorsToCalibrate = _wtoi(INumericEntryDialog.vEditString);
				// if choose all detectors, disable function
				//if (vNumberOfDetectorsToCalibrate == vLocalConfigurationData->vNumberOfXRayDetectors)
				//	vNumberOfDetectorsToCalibrate = 0;

				
				//Set dialog box data titles and number value 1st
				INumericEntryDialog.vEditString = dtoa(vNumberOfPixelAtBottomToIgnoreInCalibration,0);

				INumericEntryDialog.m_DialogTitleStaticText1 = "Enter the number of X-Ray Detector pixels to ignore";
				INumericEntryDialog.m_DialogTitleStaticText2 = "at the bottom of the image where conveyor blocks them";
				INumericEntryDialog.m_DialogTitleStaticText3 = "Should be 10 or less unless it is a special situation";
				INumericEntryDialog.m_DialogTitleStaticText4 = "Default = 10, Original Value: " + INumericEntryDialog.vEditString;

				INumericEntryDialog.m_UnitsString = "0 is Disabled, all pixels included, no conveyor blockage";
				INumericEntryDialog.vMaxValue = 255;
				INumericEntryDialog.vIntegerOnly = true;
				//Pass control to dialog box and display
				int nResponse = INumericEntryDialog.DoModal();
				//dialog box is now closed, if user pressed select do this
				//if user pressed cancel, do nothing
				if (nResponse == IDOK)
				{
					vNumberOfPixelAtBottomToIgnoreInCalibration = _wtoi(INumericEntryDialog.vEditString);
					// if choose all detectors, disable function
				}
				else 
				if (nResponse == 10)
				{
					//Main Menu button pressed
					Sleep(1); //is the equivelent of a yeild statement;
					CDialog::EndDialog(10);
				}
			//}
			//else 
			//if (nResponse == 10)
			//{
			//	//Main Menu button pressed
			//	Sleep(1); //is the equivelent of a yeild statement;
			//	CDialog::EndDialog(10);
			//}
		}

		DisplayButtonLabels();
		SetChangeMade();
	}
}

void CConfigureEdgePixelAdjustDialog::OnFunction4Button() 
{
	//Cancel was pressed
	if (vChangeMade)
	if ((!vChangeMade) || (AbortSetupChanges()))
		CDialog::OnCancel();
}

void CConfigureEdgePixelAdjustDialog::OnFunction5Button() 
{
	// Next button pressed
	if (vFirst + 16 < vNumberOfPixelsToAdjust)
		vFirst = vFirst + 8;
	else
	if (vNumberOfPixelsToAdjust > 8)
		vFirst = vNumberOfPixelsToAdjust - 8;
	DisplayButtonLabels();
	ShowAdjustPixelValues();
}

void CConfigureEdgePixelAdjustDialog::DisplayButtonLabels() 
{
	if (vFirst > 0)
	{
		m_Function3Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
		SetDlgItemText(IDC_Function3Button, _T("Previous"));
		m_Function3Display.ShowWindow(SW_HIDE);
	}
	else
	{
		CString TempText = "";
		m_Function3Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		m_Function3Display.ShowWindow(SW_SHOW);
		if (vCorrectDetectorEdges == 1)
		{
			SetDlgItemText(IDC_Function3Button, _T("Correct All Pixels"));
			TempText = "Correcting Only Edge Pixels";
		}
		else
		if (vCorrectDetectorEdges == 2)
		{
			SetDlgItemText(IDC_Function3Button, _T("Correct No Pixels"));
			//CString TempString = " In All Detectors";
			//if (vNumberOfDetectorsToCalibrate)
			//	TempString = " In Lower " + dtoa(vNumberOfDetectorsToCalibrate,0) + " Detectors";
			TempText = "Correcting All Pixels";// + TempString;
		}
		else
		{
			SetDlgItemText(IDC_Function3Button, _T("Correct Edge Pixels"));
			TempText = "Not Correcting Any Pixels";
		}
		TempText = TempText + "\nConveyor blocks bottom " + 
			dtoa(vNumberOfPixelAtBottomToIgnoreInCalibration,0) + " pixels";
		SetDlgItemText(IDC_Function3Display,TempText);
	}


	if (vFirst < vNumberOfPixelsToAdjust - 8)
	{
		m_Function5Button.ShowWindow(SW_SHOW);
		SetDlgItemText(IDC_Function5Button, _T("Next"));
	}
	else
	{
		m_Function5Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_Function5Button.GetRectToInvalidateArrow());
		SetDlgItemText(IDC_Function5Button, _T(""));
	}

	SetDlgItemText(IDC_SubFunction1Button,"Pixel " + vPixelNumberString[1 + vFirst] + "\nDet " + dtoa(1 + ((0 + vFirst) / vLocalConfigurationData->vPixelsPerDetector / vUsePoint4mmXScanDetectorsMultiplier),0));
	SetDlgItemText(IDC_SubFunction2Button,"Pixel " + vPixelNumberString[2 + vFirst] + "\nDet " + dtoa(1 + ((1 + vFirst) / vLocalConfigurationData->vPixelsPerDetector / vUsePoint4mmXScanDetectorsMultiplier),0));
	SetDlgItemText(IDC_SubFunction3Button,"Pixel " + vPixelNumberString[3 + vFirst] + "\nDet " + dtoa(1 + ((2 + vFirst) / vLocalConfigurationData->vPixelsPerDetector / vUsePoint4mmXScanDetectorsMultiplier),0));
	SetDlgItemText(IDC_SubFunction4Button,"Pixel " + vPixelNumberString[4 + vFirst] + "\nDet " + dtoa(1 + ((3 + vFirst) / vLocalConfigurationData->vPixelsPerDetector / vUsePoint4mmXScanDetectorsMultiplier),0));
	SetDlgItemText(IDC_SubFunction5Button,"Pixel " + vPixelNumberString[5 + vFirst] + "\nDet " + dtoa(1 + ((4 + vFirst) / vLocalConfigurationData->vPixelsPerDetector / vUsePoint4mmXScanDetectorsMultiplier),0));
	SetDlgItemText(IDC_SubFunction6Button,"Pixel " + vPixelNumberString[6 + vFirst] + "\nDet " + dtoa(1 + ((5 + vFirst) / vLocalConfigurationData->vPixelsPerDetector / vUsePoint4mmXScanDetectorsMultiplier),0));
	SetDlgItemText(IDC_SubFunction7Button,"Pixel " + vPixelNumberString[7 + vFirst] + "\nDet " + dtoa(1 + ((6 + vFirst) / vLocalConfigurationData->vPixelsPerDetector / vUsePoint4mmXScanDetectorsMultiplier),0));
	SetDlgItemText(IDC_SubFunction8Button,"Pixel " + vPixelNumberString[8 + vFirst] + "\nDet " + dtoa(1 + ((7 + vFirst) / vLocalConfigurationData->vPixelsPerDetector / vUsePoint4mmXScanDetectorsMultiplier),0));
}

void CConfigureEdgePixelAdjustDialog::OnSubFunction1Button() 
{
	// Pixel 1 Detector Edge Adjust button pressed
	AdjustPixel(1 + vFirst);
	ShowAdjustPixelValues();
}

void CConfigureEdgePixelAdjustDialog::OnSubFunction2Button() 
{
	// Pixel 2 Detector Edge Adjust button pressed
	AdjustPixel(2 + vFirst);
	ShowAdjustPixelValues();
}

void CConfigureEdgePixelAdjustDialog::OnSubFunction3Button() 
{
	// Pixel 3 Detector Edge Adjust button pressed
	AdjustPixel(3 + vFirst);
	ShowAdjustPixelValues();
}

void CConfigureEdgePixelAdjustDialog::OnSubFunction4Button() 
{
	// Pixel 4 Detector Edge Adjust button pressed
	AdjustPixel(4 + vFirst);
	ShowAdjustPixelValues();
}

void CConfigureEdgePixelAdjustDialog::OnSubFunction5Button() 
{
	// Pixel 5 Detector Edge Adjust button pressed
	AdjustPixel(5 + vFirst);
	ShowAdjustPixelValues();
}

void CConfigureEdgePixelAdjustDialog::OnSubFunction6Button() 
{
	// Pixel 6 Detector Edge Adjust button pressed
	AdjustPixel(6 + vFirst);
	ShowAdjustPixelValues();
}

void CConfigureEdgePixelAdjustDialog::OnSubFunction7Button() 
{
	// Pixel 7 Detector Edge Adjust button pressed
	AdjustPixel(7 + vFirst);
	ShowAdjustPixelValues();
}

void CConfigureEdgePixelAdjustDialog::OnSubFunction8Button() 
{
	// Pixel 8 Detector Edge Adjust button pressed
	AdjustPixel(8 + vFirst);
	ShowAdjustPixelValues();
}


BOOL CConfigureEdgePixelAdjustDialog::PreTranslateMessage(MSG* pMsg) 
{
	CWnd * TempWindow = NULL;
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == cUpdateTitleBarMessage)
	{
		CWnd *TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText1);
		if (TempWindow)
			TempWindow->Invalidate(false);
		return true;  //true indicates it has been handled, so do not process
	}
	else
	if (pMsg->message == WM_KEYDOWN)
	{
		int TempKey = pMsg->wParam;
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
				return true;
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
				PostThreadMessage(vGlobalCallingThreadID, cShowHelp, 38, 0);
				{
					TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText1);
					if (TempWindow)
						TempWindow->Invalidate(false);
				}
			break;
			case 27: // Escape
				::PostThreadMessage(vGlobalCallingThreadID,	cStartChatWindowMessage, 0,0);
			break;	
		}
		return true;  //true indicates it has been handled, so do not process
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

HBRUSH CConfigureEdgePixelAdjustDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	HWND TempDialogHwnd = this->m_hWnd;
	int TextSize;
	pDC->SetAttribDC(pDC->m_hDC);
	
	// TODO: Change any attributes of the DC here
	if (pWnd->GetDlgCtrlID() == IDC_DialogTitleStaticText1)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_DialogTitleStaticText, 8);
		pDC->SetAttribDC(pDC->m_hDC);
		pDC->SetTextColor(vGlobalDialogTitleColor);
	}

	if (pWnd->GetDlgCtrlID() == IDC_Function3Display)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function3Display, 5);  //two lines so using function button for this display
		if (vNumberOfPixelAtBottomToIgnoreInCalibration > 10)
		{
			pDC->SetBkMode(TRANSPARENT);
			return vLocalSystemData->vYellowBrush;
		}
	}
	if (pWnd->GetDlgCtrlID() == IDC_Pixel1)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Pixel1, 5);
	if (pWnd->GetDlgCtrlID() == IDC_Pixel2)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Pixel2, 5);
	if (pWnd->GetDlgCtrlID() == IDC_Pixel3)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Pixel3, 5);
	if (pWnd->GetDlgCtrlID() == IDC_Pixel4)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Pixel4, 5);
	if (pWnd->GetDlgCtrlID() == IDC_Pixel5)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Pixel5, 5);
	if (pWnd->GetDlgCtrlID() == IDC_Pixel6)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Pixel6, 5);
	if (pWnd->GetDlgCtrlID() == IDC_Pixel7)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Pixel7, 5);
	if (pWnd->GetDlgCtrlID() == IDC_Pixel8)
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Pixel8, 5);

	if (pWnd->GetDlgCtrlID() == IDC_Background)
	{
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
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

void CConfigureEdgePixelAdjustDialog::SetChangeMade() 
{
	if (!vChangeMade)
	{
		m_Function2Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		m_Function4Button.ShowWindow(SW_SHOW);
		SetDlgItemText(IDC_Function2Button, _T("Save\nand Exit"));
		SetDlgItemText(IDC_Function4Button, _T("Cancel"));
		vChangeMade = true;
	}
}

void CConfigureEdgePixelAdjustDialog::AdjustPixel(WORD TempPixelNumber)
{
	CNumericEntryDialog INumericEntryDialog;  
	
	//Set dialog box data titles and number value 1st
	INumericEntryDialog.vEditString = dtoa(vPixelM[TempPixelNumber],4);

	INumericEntryDialog.m_DialogTitleStaticText1 = "Multipicative Pixel Factor for Detector Edge Adjust";
	INumericEntryDialog.m_DialogTitleStaticText2 = "Amount to multiply edge pixel: " + vPixelNumberString[TempPixelNumber] + " by";
	INumericEntryDialog.m_DialogTitleStaticText3 = "to compensate for lower X-Ray area";
	INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + 
		INumericEntryDialog.vEditString;

	INumericEntryDialog.m_UnitsString = "Default: 1, 0 is Disabled";
	INumericEntryDialog.vMaxValue = 50;
	INumericEntryDialog.vIntegerOnly = false;
	//Pass control to dialog box and display
	int nResponse = INumericEntryDialog.DoModal();
	//dialog box is now closed, if user pressed select do this
	//if user pressed cancel, do nothing
	if (nResponse == IDOK)
	{
		vPixelM[TempPixelNumber] = ATOF(INumericEntryDialog.vEditString);
		SetChangeMade();

		//Set dialog box data titles and number value 2nd
		INumericEntryDialog.vEditString = dtoa(vPixelB[TempPixelNumber],4);

		INumericEntryDialog.m_DialogTitleStaticText1 = "Additive Pixel Factor Detector Edge Adjust";
		INumericEntryDialog.m_DialogTitleStaticText2 = "Amount to add to pixel:" + vPixelNumberString[TempPixelNumber];
		INumericEntryDialog.m_DialogTitleStaticText3 = "to compensate for lower X-Ray area";
		INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + INumericEntryDialog.vEditString;

		INumericEntryDialog.m_UnitsString = "Default: 0";
		INumericEntryDialog.vMaxValue = 150;
		INumericEntryDialog.vIntegerOnly = false;
		INumericEntryDialog.vAllowNegative = true;
		//Pass control to dialog box and display
		int nResponse = INumericEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			vPixelB[TempPixelNumber] = ATOF(INumericEntryDialog.vEditString);
			SetChangeMade();

			//Set dialog box data titles and number value 2nd
			INumericEntryDialog.vEditString = dtoa(vOverWriteBadPixel[TempPixelNumber],4);

			INumericEntryDialog.m_DialogTitleStaticText1 = "**THIS COPIES ANOTHER PIXEL DATA ON TO THIS PIXEL**";
			INumericEntryDialog.m_DialogTitleStaticText2 = "**THIS IS TO HIDE A DEAD OR BAD PIXEL IN THE IMAGE**";
			INumericEntryDialog.m_DialogTitleStaticText3 = "Enter pixel number to copy (pixel above or below)";
			INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + INumericEntryDialog.vEditString;

			INumericEntryDialog.m_UnitsString = "Default: 0, 0 = Disabled";
			INumericEntryDialog.vMaxValue = vNumberOfPixelsToAdjust;
			INumericEntryDialog.vIntegerOnly = true;
			INumericEntryDialog.vAllowNegative = false;
			//Pass control to dialog box and display
			int nResponse = INumericEntryDialog.DoModal();
			//dialog box is now closed, if user pressed select do this
			//if user pressed cancel, do nothing
			if (nResponse == IDOK)
			{
				vOverWriteBadPixel[TempPixelNumber] = _wtoi(INumericEntryDialog.vEditString);
				if (vOverWriteBadPixel[TempPixelNumber] == TempPixelNumber)
				{
					vOverWriteBadPixel[TempPixelNumber] = 0;

					CNoticeDialog TempNoticeDialog;
					TempNoticeDialog.vNoticeText = "\nYou can not copy a pixel on to itself.\nCopy command deleted.";
					TempNoticeDialog.vType = cErrorMessage;
					TempNoticeDialog.DoModal();
				}

				SetChangeMade();
			}
			else 
			if (nResponse == 10)
			{
				//Main Menu button pressed
				Sleep(1); //is the equivelent of a yeild statement;
				CDialog::EndDialog(10);
			}
		}
		else 
		if (nResponse == 10)
		{
			//Main Menu button pressed
			Sleep(1); //is the equivelent of a yeild statement;
			CDialog::EndDialog(10);
		}
	}
	else 
	if (nResponse == 10)
	{
		//Main Menu button pressed
		Sleep(1); //is the equivelent of a yeild statement;
		CDialog::EndDialog(10);
	}
}

void CConfigureEdgePixelAdjustDialog::ShowAdjustPixelValues()
{
	if (vOverWriteBadPixel[1 + vFirst])
		SetDlgItemText(IDC_Pixel1,"COPY PIXEL: " + dtoa(vOverWriteBadPixel[1 + vFirst],0));
	else
		SetDlgItemText(IDC_Pixel1,dtoa(vPixelM[1 + vFirst],4) + "x + " + dtoa(vPixelB[1 + vFirst],4));

	if (vOverWriteBadPixel[2 + vFirst])
		SetDlgItemText(IDC_Pixel2,"COPY PIXEL: " + dtoa(vOverWriteBadPixel[2 + vFirst],0));
	else
		SetDlgItemText(IDC_Pixel2,dtoa(vPixelM[2 + vFirst],4) + "x + " + dtoa(vPixelB[2 + vFirst],4));

	if (vOverWriteBadPixel[3 + vFirst])
		SetDlgItemText(IDC_Pixel3,"COPY PIXEL: " + dtoa(vOverWriteBadPixel[3 + vFirst],0));
	else
		SetDlgItemText(IDC_Pixel3,dtoa(vPixelM[3 + vFirst],4) + "x + " + dtoa(vPixelB[3 + vFirst],4));

	if (vOverWriteBadPixel[4 + vFirst])
		SetDlgItemText(IDC_Pixel4,"COPY PIXEL: " + dtoa(vOverWriteBadPixel[4 + vFirst],0));
	else
		SetDlgItemText(IDC_Pixel4,dtoa(vPixelM[4 + vFirst],4) + "x + " + dtoa(vPixelB[4 + vFirst],4));

	if (vOverWriteBadPixel[5 + vFirst])
		SetDlgItemText(IDC_Pixel5,"COPY PIXEL: " + dtoa(vOverWriteBadPixel[5 + vFirst],0));
	else
		SetDlgItemText(IDC_Pixel5,dtoa(vPixelM[5 + vFirst],4) + "x + " + dtoa(vPixelB[5 + vFirst],4));

	if (vOverWriteBadPixel[6 + vFirst])
		SetDlgItemText(IDC_Pixel6,"COPY PIXEL: " + dtoa(vOverWriteBadPixel[6 + vFirst],0));
	else
		SetDlgItemText(IDC_Pixel6,dtoa(vPixelM[6 + vFirst],4) + "x + " + dtoa(vPixelB[6 + vFirst],4));

	if (vOverWriteBadPixel[7 + vFirst])
		SetDlgItemText(IDC_Pixel7,"COPY PIXEL: " + dtoa(vOverWriteBadPixel[7 + vFirst],0));
	else
		SetDlgItemText(IDC_Pixel7,dtoa(vPixelM[7 + vFirst],4) + "x + " + dtoa(vPixelB[7 + vFirst],4));

	if (vOverWriteBadPixel[8 + vFirst])
		SetDlgItemText(IDC_Pixel8,"COPY PIXEL: " + dtoa(vOverWriteBadPixel[8 + vFirst],0));
	else
		SetDlgItemText(IDC_Pixel8,dtoa(vPixelM[8 + vFirst],4) + "x + " + dtoa(vPixelB[8 + vFirst],4));
}


void CConfigureEdgePixelAdjustDialog::OnDialogtitlestatictext1()
{
	//bring up soft key menu for help, print screen, Image, Clear, and hidden shift key
	BYTE TempHiddenKeys = 8;
	if (!PasswordOK(cTemporaryInspxPassword,false))
		TempHiddenKeys = 10;
	PostThreadMessage(vGlobalCallingThreadID, cDisplaySoftKeyMenu, TempHiddenKeys, TempHiddenKeys);
}
