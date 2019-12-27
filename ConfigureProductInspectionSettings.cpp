//ScanTrac Side View Source File
// ConfigureProductInspectionSettings.cpp : implementation file
//

#include "stdafx.h"
#include "scantrac.h"
#include "ConfigureProductInspectionSettings.h"
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
// CConfigureProductInspectionSettings dialog


CConfigureProductInspectionSettings::CConfigureProductInspectionSettings(CWnd* pParent /*=NULL*/)
	: CDialog(CConfigureProductInspectionSettings::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConfigureProductInspectionSettings)
	//}}AFX_DATA_INIT
	vChangeMade = false;
	vInspectionEditing = NULL;
	vFillInLightSpots = 0;
	vTakeDifferenceFromMedian = 0;
	vFindSpotInSubROI = 0;
	vMinimumLength = 0;
	vFindDarkSpotOnLine = 0;
	vPixelsStepToNextROI = 1;
	vIgnoreEdgePixels = 0;
	vDesensitizeEdgePixels = 0;
	vIPContaminant = 0;
	vDesensitizeIrregularArea = 0;
	vPreSmoothImage = false;
	vPixelLookAhead = 0;
	vBonesInspection = 0;
	//vPreBlurImage = false;
	//vPreSelectDarkSpotsInImage = false;
	//vRequire2InARow = false;
	vOverlap = 0;
	vRemoveJarPunt = 0;
	vEnable = true;
	vLocalCWndCollection.Add(this);
	vLocalCWndCollection.Add(&m_Function3Display);
	vLocalCWndCollection.Add(&m_Function3Button);
	vLocalCWndCollection.Add(&m_Function5Display);
	vLocalCWndCollection.Add(&m_Function5Button);
	vLocalCWndCollection.Add(&m_SubFunction7Display);
	vLocalCWndCollection.Add(&m_SubFunction7Button);
	vLocalCWndCollection.Add(&m_SubFunction6Display);
	vLocalCWndCollection.Add(&m_SubFunction6Button);
	vLocalCWndCollection.Add(&m_SubFunction1Display);
	vLocalCWndCollection.Add(&m_SubFunction1Button);
	vLocalCWndCollection.Add(&m_SubFunction2Display);
	vLocalCWndCollection.Add(&m_SubFunction2Button);
	vLocalCWndCollection.Add(&m_SubFunction4Display);
	vLocalCWndCollection.Add(&m_SubFunction3Display);
	vLocalCWndCollection.Add(&m_SubFunction4Button);
	vLocalCWndCollection.Add(&m_SubFunction3Button);
	vLocalCWndCollection.Add(&m_Function4Button);
	vLocalCWndCollection.Add(&m_Function2Button);
	vLocalCWndCollection.Add(&m_Background);
	vLocalCWndCollection.Add(&m_DialogTitleStaticText);
	vLocalCWndCollection.Add(&m_Enable);
	vLocalCWndCollection.Add(&m_Function1Button);
	vLocalCWndCollection.Add(&m_ProcessStatus3);
	vLocalCWndCollection.Add(&m_ProductNameTitle);
	vLocalCWndCollection.Add(&m_ProductNameTitle2);
	vLocalCWndCollection.Add(&m_SubFunction5Button);
	vLocalCWndCollection.Add(&m_SubFunction5Display);
	vLocalCWndCollection.Add(&m_SubFunction8Button);
	vLocalCWndCollection.Add(&m_SubFunction8Display);
}

void CConfigureProductInspectionSettings::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	m_Background.SetWindowPos(NULL,cMainBackgroundLeft,
		cMainBackgroundTop,cMainBackgroundLength,
		cMainBackgroundHeight,SWP_NOZORDER);

	vPreSmoothImage = vInspectionEditing->vPreSmoothImage;
	vPixelLookAhead = vInspectionEditing->vPixelLookAhead;
	//vPreBlurImage = vInspectionEditing->vPreBlurImage;
	//vPreSelectDarkSpotsInImage = vInspectionEditing->vPreSelectDarkSpotsInImage;
	//vRequire2InARow = vInspectionEditing->vRequire2InARow;
	vOverlap = vInspectionEditing->vOverlap;
	vSwungBaffleTolerance = vInspectionEditing->vSwungBaffleTolerance;
	vRemoveJarPunt = vInspectionEditing->vRemoveJarPunt;
	vFillInLightSpots = vInspectionEditing->vFillInLightSpots;
	vTakeDifferenceFromMedian = vInspectionEditing->vTakeDifferenceFromMedian;
	vBonesInspection = vInspectionEditing->vBonesInspection;
	vFindSpotInSubROI = vInspectionEditing->vFindSpotInSubROI;
	vMinimumLength = vInspectionEditing->vMinimumLength;
	vFindDarkSpotOnLine = vInspectionEditing->vFindDarkSpotOnLine;
	vPixelsStepToNextROI = vInspectionEditing->vPixelsStepToNextROI;
	vDesensitizeEdgePixels = vInspectionEditing->vDesensitizeEdgePixels;
	vIPContaminant = vInspectionEditing->vIPContaminant;
	vDesensitizeIrregularArea = vInspectionEditing->vDesensitizeIrregularArea;
	vTrimEdgesOfJar = vInspectionEditing->vTrimEdgesOfJar;
	vTakeDerivative = vInspectionEditing->vTakeDerivative;
	vIgnoreEdgePixels = vInspectionEditing->vIgnoreEdgePixels;

	vInvertEjectLogic = vInspectionEditing->vInvertEjectLogic;
	vEnable = vInspectionEditing->vEnable;
	vMaximumDrop = vInspectionEditing->vMaximumDrop;
	vFollowBottomUpAndDown = vInspectionEditing->vFollowBottomUpAndDown;
	vTryThresholdsDownToThreshold = vInspectionEditing->vTryThresholdsDownToThreshold;

	CString TempText = vInspectionEditing->vName + ", Type: " +
		vInspectionEditing->GetInspectionTypeString();
	if (vInspectionEditing->vInspectionType == cJarBottomContaminantInspection)
	{
		m_SubFunction4Button.ShowWindow(SW_SHOW);
		m_SubFunction4Display.ShowWindow(SW_SHOW);

		m_SubFunction6Button.ShowWindow(SW_SHOW);
		m_SubFunction6Display.ShowWindow(SW_SHOW);

		m_SubFunction3Button.ShowWindow(SW_SHOW);
		m_SubFunction3Display.ShowWindow(SW_SHOW);

		m_SubFunction2Button.ShowWindow(SW_SHOW);
		m_SubFunction2Display.ShowWindow(SW_SHOW);
	}
	m_SubFunction1Button.ShowWindow(SW_SHOW);
	m_SubFunction1Display.ShowWindow(SW_SHOW);
	if ((vInspectionEditing->vInspectionType == cContaminantInspection) ||
		(vInspectionEditing->vInspectionType == cIPContaminantInspection) ||
		(vInspectionEditing->vInspectionType == cSpotInROIContaminantInspection) ||
		(vInspectionEditing->vInspectionType == cContaminantBonesInspection) ||
		(vInspectionEditing->vInspectionType == cContaminant22Inspection) ||
		(vInspectionEditing->vInspectionType == cGlass13mmContaminantInspection))
	{
		m_SubFunction2Button.ShowWindow(SW_SHOW);
		m_SubFunction2Display.ShowWindow(SW_SHOW);
		m_SubFunction3Button.ShowWindow(SW_SHOW);
		m_SubFunction3Display.ShowWindow(SW_SHOW);
		m_SubFunction4Button.ShowWindow(SW_SHOW);
		m_SubFunction4Display.ShowWindow(SW_SHOW);
		m_SubFunction6Button.ShowWindow(SW_SHOW);
		m_SubFunction6Display.ShowWindow(SW_SHOW);
		//m_SubFunction7Button.ShowWindow(SW_SHOW);
		//m_ArrowRight3.ShowWindow(SW_SHOW);
		//m_SubFunction7Display.ShowWindow(SW_SHOW);
		m_Function3Button.ShowWindow(SW_SHOW);
		m_Function3Display.ShowWindow(SW_SHOW);
		m_Function5Button.ShowWindow(SW_SHOW);
		m_Function5Display.ShowWindow(SW_SHOW);
	}
		m_SubFunction1Button.ShowWindow(SW_SHOW);
		m_SubFunction1Display.ShowWindow(SW_SHOW);
	if ((vInspectionEditing->vInspectionType == cContaminantInspection) ||
		(vInspectionEditing->vInspectionType == cIPContaminantInspection) ||
		(vInspectionEditing->vInspectionType == cWireContaminantInspection) ||
		(vInspectionEditing->vInspectionType == cContaminantBonesInspection) ||
		(vInspectionEditing->vInspectionType == cSpotInROIContaminantInspection))
	{
		m_SubFunction7Button.ShowWindow(SW_SHOW);
		m_SubFunction7Display.ShowWindow(SW_SHOW);
	}
	if ((vInspectionEditing->vInspectionType == cContaminantInspection) ||
		(vInspectionEditing->vInspectionType == cIPContaminantInspection) ||
		(vInspectionEditing->vInspectionType == cWireContaminantInspection) ||
		(vInspectionEditing->vInspectionType == cSpotInROIContaminantInspection) ||
		(vInspectionEditing->vInspectionType == cThresholdContaminantInspection) ||
		(vInspectionEditing->vInspectionType == cBadHeartInspection) ||
		(vInspectionEditing->vInspectionType == cDripBySpoutInspection) ||
		(vInspectionEditing->vInspectionType == cDripInspection) ||
		(vInspectionEditing->vInspectionType == cFloodedInspection) ||
		(vInspectionEditing->vInspectionType == cContaminantBonesInspection) ||
		(vInspectionEditing->vInspectionType == cContaminant22Inspection) ||
		(vInspectionEditing->vInspectionType == cGlass13mmContaminantInspection))
	{
		m_SubFunction6Button.ShowWindow(SW_SHOW);
		m_SubFunction6Display.ShowWindow(SW_SHOW);
	}

	SetDlgItemText(IDC_ProductNameTitle2,TempText);
	SetDlgItemText(IDC_ProductNameTitle,"Product: " + *(vPointerToProductEditing->GetProductName()));

	this->SetWindowText( _T("Params2"));
	
	UpdateDisplay();
	::SetupMenu(vLocalCWndCollection);
}


void CConfigureProductInspectionSettings::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfigureProductInspectionSettings)
	DDX_Control(pDX, IDC_Function3Display, m_Function3Display);
	DDX_Control(pDX, IDC_Function3Button, m_Function3Button);
	DDX_Control(pDX, IDC_Function5Display, m_Function5Display);
	DDX_Control(pDX, IDC_Function5Button, m_Function5Button);
	DDX_Control(pDX, IDC_SubFunction7Display, m_SubFunction7Display);
	DDX_Control(pDX, IDC_SubFunction7Button, m_SubFunction7Button);
	DDX_Control(pDX, IDC_SubFunction6Display, m_SubFunction6Display);
	DDX_Control(pDX, IDC_SubFunction6Button, m_SubFunction6Button);
	DDX_Control(pDX, IDC_SubFunction1Display, m_SubFunction1Display);
	DDX_Control(pDX, IDC_SubFunction1Button, m_SubFunction1Button);
	DDX_Control(pDX, IDC_SubFunction2Display, m_SubFunction2Display);
	DDX_Control(pDX, IDC_SubFunction2Button, m_SubFunction2Button);
	DDX_Control(pDX, IDC_SubFunction4Display, m_SubFunction4Display);
	DDX_Control(pDX, IDC_SubFunction3Display, m_SubFunction3Display);
	DDX_Control(pDX, IDC_SubFunction4Button, m_SubFunction4Button);
	DDX_Control(pDX, IDC_SubFunction3Button, m_SubFunction3Button);
	DDX_Control(pDX, IDC_Function4Button, m_Function4Button);
	DDX_Control(pDX, IDC_Function2Button, m_Function2Button);
	DDX_Control(pDX, IDC_Background, m_Background);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_DialogTitleStaticText1, m_DialogTitleStaticText);
	DDX_Control(pDX, IDC_Enable, m_Enable);
	DDX_Control(pDX, IDC_Function1Button, m_Function1Button);
	DDX_Control(pDX, IDC_ProcessStatus3, m_ProcessStatus3);
	DDX_Control(pDX, IDC_ProductNameTitle, m_ProductNameTitle);
	DDX_Control(pDX, IDC_ProductNameTitle2, m_ProductNameTitle2);
	DDX_Control(pDX, IDC_SubFunction5Button, m_SubFunction5Button);
	DDX_Control(pDX, IDC_SubFunction5Display, m_SubFunction5Display);
	DDX_Control(pDX, IDC_SubFunction8Button, m_SubFunction8Button);
	DDX_Control(pDX, IDC_SubFunction8Display, m_SubFunction8Display);
}


BEGIN_MESSAGE_MAP(CConfigureProductInspectionSettings, CDialog)
	//{{AFX_MSG_MAP(CConfigureProductInspectionSettings)
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
	ON_STN_CLICKED(IDC_DialogTitleStaticText1, &CConfigureProductInspectionSettings::OnDialogtitlestatictext1)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfigureProductInspectionSettings message handlers

void CConfigureProductInspectionSettings::OnFunction1Button() 
{
	// User clicked Main Menu Button to exit without saving
	if ((!vChangeMade) || (vLocalSystemData->vInAutoSetup) || (AbortSetupChanges()))
		CDialog::EndDialog(10);
	if ((!vChangeMade) || (vLocalSystemData->vInAutoSetup) || (AbortSetupChanges()))
		CDialog::EndDialog(10);
}

void CConfigureProductInspectionSettings::OnFunction2Button() 
{
	//Save and Exit button pressed
	vInspectionEditing->vPreSmoothImage = vPreSmoothImage;
	//vInspectionEditing->vPreBlurImage = vPreBlurImage;
	//vInspectionEditing->vPreSelectDarkSpotsInImage = vPreSelectDarkSpotsInImage;
	//vInspectionEditing->vRequire2InARow = vRequire2InARow;
	vInspectionEditing->vSwungBaffleTolerance = vSwungBaffleTolerance;
	vInspectionEditing->vRemoveJarPunt = vRemoveJarPunt;
	vInspectionEditing->vOverlap = vOverlap;
	vInspectionEditing->vPixelLookAhead = vPixelLookAhead;
	vInspectionEditing->vFillInLightSpots = vFillInLightSpots;
	vInspectionEditing->vTakeDifferenceFromMedian = vTakeDifferenceFromMedian;
	vInspectionEditing->vBonesInspection = vBonesInspection;
	vInspectionEditing->vFindSpotInSubROI = vFindSpotInSubROI;
	vInspectionEditing->vMinimumLength = vMinimumLength;
	vInspectionEditing->vFindDarkSpotOnLine = vFindDarkSpotOnLine;
	vInspectionEditing->vPixelsStepToNextROI = vPixelsStepToNextROI;
	vInspectionEditing->vDesensitizeEdgePixels = vDesensitizeEdgePixels;
	vInspectionEditing->vIPContaminant = vIPContaminant;
	vInspectionEditing->vDesensitizeIrregularArea = vDesensitizeIrregularArea;
	vInspectionEditing->vTrimEdgesOfJar = vTrimEdgesOfJar;
	vInspectionEditing->vTakeDerivative = vTakeDerivative;
	vInspectionEditing->vInvertEjectLogic = vInvertEjectLogic;
	vInspectionEditing->vEnable = vEnable;
	vInspectionEditing->vMaximumDrop = vMaximumDrop;
	vInspectionEditing->vFollowBottomUpAndDown = vFollowBottomUpAndDown;
	vInspectionEditing->vTryThresholdsDownToThreshold = vTryThresholdsDownToThreshold;
	vInspectionEditing->vIgnoreEdgePixels = vIgnoreEdgePixels;

	CDialog::EndDialog(true);
}

void CConfigureProductInspectionSettings::OnFunction3Button() 
{
	if ((vInspectionEditing->vInspectionType == cAdvancedContaminant2Inspection) ||
		(vInspectionEditing->vInspectionType == cContaminantBonesInspection))
	{
		if (vTakeDifferenceFromMedian)
		{
			BYTE TempUsingDifferenceBonesAlgorithm = vTakeDifferenceFromMedian & 0x80;

			CYesNoDialog TempYesNoDialog;
			CString TempText = "";
			if (TempUsingDifferenceBonesAlgorithm)
				TempText = "\n\nCurrently Using Difference Bones Algorithm.";
			else
				TempText = "\n\nCurrently Using Bones Median Algorithm.";

			TempText = "\nUse Median or Difference Bones Algorithm?" + TempText;
			TempYesNoDialog.vNoticeText = TempText;

			TempYesNoDialog.vQuestionType = cConfirmQuestion;
			TempYesNoDialog.vYesButtonText = "Difference Algorithm";
			TempYesNoDialog.vNoButtonText = "Median Algorithm";
			int TempResult = TempYesNoDialog.DoModal();

			if (TempResult == IDOK)
				vTakeDifferenceFromMedian = vTakeDifferenceFromMedian | 0x80;
			else
				vTakeDifferenceFromMedian = vTakeDifferenceFromMedian & 0x7F;

			CNumericEntryDialog INumericEntryDialog;  
			INumericEntryDialog.vEditString = dtoa(vTakeDifferenceFromMedian & 0x7F, 0); //in 7 Least Significant Bits

			INumericEntryDialog.m_DialogTitleStaticText1 = "Enter Matrix Size for Bones Algorithm";
			INumericEntryDialog.m_DialogTitleStaticText2 = "7 thin, 21 large";
			INumericEntryDialog.m_DialogTitleStaticText3 = "Inspection: " + vInspectionEditing->vName;
			INumericEntryDialog.m_DialogTitleStaticText4 = "Default Value: 11";

			INumericEntryDialog.m_UnitsString = "Units in Pixels, 0 = Disabled";
			INumericEntryDialog.vMaxValue = 50;
			INumericEntryDialog.vMinValue = 0;
			INumericEntryDialog.vIntegerOnly = true;
			//Pass control to dialog box and display
			int nResponse = INumericEntryDialog.DoModal();
			//dialog box is now closed, if user pressed select do this
			//if user pressed cancel, do nothing
			if (nResponse == IDOK)
			{
				vTakeDifferenceFromMedian = (vTakeDifferenceFromMedian & 0x80) + _wtoi(INumericEntryDialog.vEditString);

				if (!(vTakeDifferenceFromMedian & 0x7F)) //clear difference bit if no matrix size
					vTakeDifferenceFromMedian = 0;

				UpdateDisplay();
				SetChangeMade();

				if (vTakeDifferenceFromMedian & 0x80) //doing difference algorithm
				{
					INumericEntryDialog.vEditString = dtoa(vPixelsStepToNextROI,0);
					INumericEntryDialog.m_DialogTitleStaticText1 = "Enter the step to the pixel to take the difference";
					INumericEntryDialog.m_DialogTitleStaticText2 = " from each pixel.  1 = next pixel location.";
					INumericEntryDialog.m_DialogTitleStaticText3 = "Inspection: " + vInspectionEditing->vName;
					INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " +
						INumericEntryDialog.vEditString + ", Default Value: 2";

					INumericEntryDialog.m_UnitsString = "Units in Pixels";
					INumericEntryDialog.vMaxValue = (vTakeDifferenceFromMedian & 0x7F) - 1;
					INumericEntryDialog.vMinValue = 1;
					INumericEntryDialog.vIntegerOnly = true;
					//Pass control to dialog box and display
					int nResponse = INumericEntryDialog.DoModal();
					//dialog box is now closed, if user pressed select do this
					//if user pressed cancel, do nothing
					if (nResponse == IDOK)
					{
						vPixelsStepToNextROI = _wtoi(INumericEntryDialog.vEditString);
						UpdateDisplay();
						SetChangeMade();
	/*
						INumericEntryDialog.vEditString = dtoa(vMaximumScaleFactor,0);
						INumericEntryDialog.m_DialogTitleStaticText1 = "Enter the Maximum Scale Factor";
						INumericEntryDialog.m_DialogTitleStaticText2 = " to make dark areas more sensitive";
						INumericEntryDialog.m_DialogTitleStaticText3 = "Inspection: " + vInspectionEditing->vName;
						INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " +
							INumericEntryDialog.vEditString + ", 0 and 1 = Disabled, Default Value: 2";

						INumericEntryDialog.m_UnitsString = "Units in Pixels";
						INumericEntryDialog.vMaxValue = 10;
						INumericEntryDialog.vMinValue = 0;
						INumericEntryDialog.vIntegerOnly = true;
						//Pass control to dialog box and display
						int nResponse = INumericEntryDialog.DoModal();
						//dialog box is now closed, if user pressed select do this
						//if user pressed cancel, do nothing
						if (nResponse == IDOK)
						{
							vMaximumScaleFactor = _wtoi(INumericEntryDialog.vEditString);
							UpdateDisplay();
							SetChangeMade();
						}
						else 
						if (nResponse == 10)
						{
							//Main Menu button pressed
							Sleep(1);
							CDialog::EndDialog(10);
						}
						*/
					}
					else 
					if (nResponse == 10)
					{
						//Main Menu button pressed
						Sleep(1);
						CDialog::EndDialog(10);
					}
				}

				/*
				INumericEntryDialog.vEditString = dtoa((vTakeDifferenceFromMedian & 0xF0) >> 4, 0);  //in 4 Most Significant Bits rotate to LSBits

				INumericEntryDialog.m_DialogTitleStaticText1 = "Enter value for Vertical Bones Algorithm";
				INumericEntryDialog.m_DialogTitleStaticText2 = "Enter number of Pixels to Median in Vertical";
				INumericEntryDialog.m_DialogTitleStaticText3 = "Inspection: " + vInspectionEditing->vName;
				INumericEntryDialog.m_DialogTitleStaticText4 = "Default Value: 11";

				INumericEntryDialog.m_UnitsString = "Units in Pixels, 0 = Disabled";
				INumericEntryDialog.vMaxValue = 15;
				INumericEntryDialog.vMinValue = 0;
				INumericEntryDialog.vIntegerOnly = true;
				//Pass control to dialog box and display
				int nResponse = INumericEntryDialog.DoModal();
				//dialog box is now closed, if user pressed select do this
				//if user pressed cancel, do nothing
				if (nResponse == IDOK)
				{
					vTakeDifferenceFromMedian = ((vTakeDifferenceFromMedian & 0x0F) | (_wtoi(INumericEntryDialog.vEditString) << 4));
					UpdateDisplay();
					SetChangeMade();
				}
				else 
				if (nResponse == 10)
				{
					//Main Menu button pressed
					Sleep(1);
					CDialog::EndDialog(10);
				}
				*/
			}
			else 
			if (nResponse == 10)
			{
				//Main Menu button pressed
				Sleep(1);
				CDialog::EndDialog(10);
			}
		}
		else
		{
			CNumericEntryDialog INumericEntryDialog;  
			INumericEntryDialog.vEditString = dtoa(vBonesInspection, 0);

			INumericEntryDialog.m_DialogTitleStaticText1 = "Enter Look Ahead For Large Bones Algorithm";
			INumericEntryDialog.m_DialogTitleStaticText2 = "7 thin, 15 large";
			INumericEntryDialog.m_DialogTitleStaticText3 = "Inspection: " + vInspectionEditing->vName;
			INumericEntryDialog.m_DialogTitleStaticText4 = "Default Value: 11";

			INumericEntryDialog.m_UnitsString = "Units in Pixels";
			INumericEntryDialog.vMaxValue = 50;
			INumericEntryDialog.vMinValue = 2;
			INumericEntryDialog.vIntegerOnly = true;
			//Pass control to dialog box and display
			int nResponse = INumericEntryDialog.DoModal();
			//dialog box is now closed, if user pressed select do this
			//if user pressed cancel, do nothing
			if (nResponse == IDOK)
			{
				vBonesInspection = _wtoi(INumericEntryDialog.vEditString);

				UpdateDisplay();
				SetChangeMade();
				INumericEntryDialog.vEditString = dtoa(vPixelLookAhead, 0);

				INumericEntryDialog.m_DialogTitleStaticText1 = "Enter Transition Look Ahead For Large Bones Algorithm";
				INumericEntryDialog.m_DialogTitleStaticText2 = "1 sharp, 3 gradual";
				INumericEntryDialog.m_DialogTitleStaticText3 = "Inspection: " + vInspectionEditing->vName;
				INumericEntryDialog.m_DialogTitleStaticText4 = "Default Value: 2";

				INumericEntryDialog.m_UnitsString = "Units in Pixels";
				INumericEntryDialog.vMaxValue = vBonesInspection / 2;
				INumericEntryDialog.vMinValue = 1;
				INumericEntryDialog.vIntegerOnly = true;
				//Pass control to dialog box and display
				int nResponse = INumericEntryDialog.DoModal();
				//dialog box is now closed, if user pressed select do this
				//if user pressed cancel, do nothing
				if (nResponse == IDOK)
				{
					vPixelLookAhead = _wtoi(INumericEntryDialog.vEditString);

					UpdateDisplay();
					SetChangeMade();
				}
				else 
				if (nResponse == 10)
				{
					//Main Menu button pressed
					Sleep(1);
					CDialog::EndDialog(10);
				}
			}
			else 
			if (nResponse == 10)
			{
				//Main Menu button pressed
				Sleep(1);
				CDialog::EndDialog(10);
			}
		}
	}
	else
	if ((vInspectionEditing->vInspectionType == cSpotInROIContaminantInspection) || (vInspectionEditing->vInspectionType == cGlass13mmContaminantInspection))
	{
		CNumericEntryDialog INumericEntryDialog;  
		INumericEntryDialog.vEditString = dtoa(vFindDarkSpotOnLine,0);

		INumericEntryDialog.m_DialogTitleStaticText1 = "Number of Pixels to look for dark spot on line";
		INumericEntryDialog.m_DialogTitleStaticText2 = "Spot must be narrower than this value";
		INumericEntryDialog.m_DialogTitleStaticText3 = "Inspection: " + vInspectionEditing->vName;
		INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " +
			INumericEntryDialog.vEditString + ", Default Value: 10";

		INumericEntryDialog.m_UnitsString = "Units in Pixels, 0 is Disabled";
		INumericEntryDialog.vMaxValue = 250;
		INumericEntryDialog.vMinValue = 0;
		INumericEntryDialog.vIntegerOnly = true;
		//Pass control to dialog box and display
		int nResponse = INumericEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			vFindDarkSpotOnLine = _wtoi(INumericEntryDialog.vEditString);
			UpdateDisplay();
			SetChangeMade();
			if (vFindDarkSpotOnLine)
			{
				INumericEntryDialog.vEditString = dtoa(vPixelsStepToNextROI,0);
				INumericEntryDialog.m_DialogTitleStaticText1 = "Enter the next pixel to move over for each test";
				INumericEntryDialog.m_DialogTitleStaticText2 = "1 = test every pixel location, 2 = skip every other";
				INumericEntryDialog.m_DialogTitleStaticText3 = "Inspection: " + vInspectionEditing->vName;
				INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " +
					INumericEntryDialog.vEditString + ", Default Value: 1";

				INumericEntryDialog.m_UnitsString = "Units in Pixels, 1 takes more time";
				INumericEntryDialog.vMaxValue = vFindDarkSpotOnLine;
				INumericEntryDialog.vMinValue = 1;
				INumericEntryDialog.vIntegerOnly = true;
				//Pass control to dialog box and display
				int nResponse = INumericEntryDialog.DoModal();
				//dialog box is now closed, if user pressed select do this
				//if user pressed cancel, do nothing
				if (nResponse == IDOK)
				{
					vPixelsStepToNextROI = _wtoi(INumericEntryDialog.vEditString);
					UpdateDisplay();
					SetChangeMade();
				}
				else 
				if (nResponse == 10)
				{
					//Main Menu button pressed
					Sleep(1);
					CDialog::EndDialog(10);
				}
			}
		}
	}
	else
	if (vInspectionEditing->vInspectionType == cWireContaminantInspection)
	{
		CNumericEntryDialog INumericEntryDialog;  
		INumericEntryDialog.vEditString = dtoa(vPixelLookAhead,0);

		INumericEntryDialog.m_DialogTitleStaticText1 = "Number of Pixels to look ahead for dark spot on line";
		INumericEntryDialog.m_DialogTitleStaticText2 = "Wire must be narrower than this value";
		INumericEntryDialog.m_DialogTitleStaticText3 = "Inspection: " + vInspectionEditing->vName;
		INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " +
			INumericEntryDialog.vEditString + ", Default Value: 8";

		INumericEntryDialog.m_UnitsString = "Units in Pixels, 0 is Disabled";
		INumericEntryDialog.vMaxValue = 250;
		INumericEntryDialog.vMinValue = 0;
		INumericEntryDialog.vIntegerOnly = true;
		//Pass control to dialog box and display
		int nResponse = INumericEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			vPixelLookAhead = _wtoi(INumericEntryDialog.vEditString);
			UpdateDisplay();
			SetChangeMade();
			if (vPixelLookAhead)
			{
				INumericEntryDialog.vEditString = dtoa(vDesensitizeEdgePixels,0);
				INumericEntryDialog.m_DialogTitleStaticText1 = "Enter the amount to desensitize edge areas";
				INumericEntryDialog.m_DialogTitleStaticText2 = "Lowers the Threshold by this amount near edges";
				INumericEntryDialog.m_DialogTitleStaticText3 = "Inspection: " + vInspectionEditing->vName;
				INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + INumericEntryDialog.vEditString + ", Default Value: 30";

				INumericEntryDialog.m_UnitsString = "Units in Pixels, 1 will miss things in the center ";
				INumericEntryDialog.vMaxValue = 200;
				INumericEntryDialog.vMinValue = 0;
				INumericEntryDialog.vIntegerOnly = true;
				//Pass control to dialog box and display
				int nResponse = INumericEntryDialog.DoModal();
				//dialog box is now closed, if user pressed select do this
				//if user pressed cancel, do nothing
				if (nResponse == IDOK)
				{
					vDesensitizeEdgePixels = _wtoi(INumericEntryDialog.vEditString);
					UpdateDisplay();
					SetChangeMade();
				}
				else 
				if (nResponse == 10)
				{
					//Main Menu button pressed
					Sleep(1);
					CDialog::EndDialog(10);
				}
			}
		}
		else 
		if (nResponse == 10)
		{
			//Main Menu button pressed
			Sleep(1);
			CDialog::EndDialog(10);
		}
	}
}

void CConfigureProductInspectionSettings::OnFunction4Button() 
{
	// pressed cancel button
	if (vChangeMade)
	if ((!vChangeMade) || (AbortSetupChanges()))
		CDialog::OnCancel();
}

void CConfigureProductInspectionSettings::OnFunction5Button() 
{
	if (vInspectionEditing->vInspectionType == cIPContaminantInspection)
	{
		CNumericEntryDialog INumericEntryDialog;  
		INumericEntryDialog.vEditString = dtoa(vIPContaminant,0);

		INumericEntryDialog.m_DialogTitleStaticText1 = "Enter the Contrast Threshold";
		INumericEntryDialog.m_DialogTitleStaticText2 = "Pixels below this value will be in the Mask";
		INumericEntryDialog.m_DialogTitleStaticText3 = "Inspection: " + vInspectionEditing->vName;
		INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " +
			INumericEntryDialog.vEditString + ", Default Value: 100";

		INumericEntryDialog.m_UnitsString = "Units in brightness, 0 is Disables Inspection";
		INumericEntryDialog.vMaxValue = 250;
		INumericEntryDialog.vMinValue = 0;
		INumericEntryDialog.vAllowZero = true;
		INumericEntryDialog.vIntegerOnly = true;
		//Pass control to dialog box and display
		int nResponse = INumericEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			vIPContaminant = _wtoi(INumericEntryDialog.vEditString);
			UpdateDisplay();
			SetChangeMade();
			if (vFindSpotInSubROI)
			{
				INumericEntryDialog.vEditString = dtoa(vPixelsStepToNextROI,0);
				INumericEntryDialog.m_DialogTitleStaticText1 = "Enter the next pixel to move ROI for each test";
				INumericEntryDialog.m_DialogTitleStaticText2 = "1 = test every pixel location, 2 = skip every other";
				INumericEntryDialog.m_DialogTitleStaticText3 = "Inspection: " + vInspectionEditing->vName;
				INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " +
					INumericEntryDialog.vEditString + ", Default Value: 3";

				INumericEntryDialog.m_UnitsString = "Units in Pixels, 1 takes more time";
				INumericEntryDialog.vMaxValue = vFindSpotInSubROI;
				INumericEntryDialog.vMinValue = 1;
				INumericEntryDialog.vIntegerOnly = true;
				//Pass control to dialog box and display
				int nResponse = INumericEntryDialog.DoModal();
				//dialog box is now closed, if user pressed select do this
				//if user pressed cancel, do nothing
				if (nResponse == IDOK)
				{
					vPixelsStepToNextROI = _wtoi(INumericEntryDialog.vEditString);
					UpdateDisplay();
					SetChangeMade();

					INumericEntryDialog.vEditString = dtoa(vDesensitizeIrregularArea / 10.0,1);
					INumericEntryDialog.m_DialogTitleStaticText1 = "Enter the Scale Factor for the Desensitize Irregular Area";
					INumericEntryDialog.m_DialogTitleStaticText2 = "0 = Disable Desensitize Irregular Area";
					INumericEntryDialog.m_DialogTitleStaticText3 = "Inspection: " + vInspectionEditing->vName;
					INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " +
						INumericEntryDialog.vEditString + ", Default Value: 3.0";

					INumericEntryDialog.m_UnitsString = "Is a multiplicative factor (0-6), 0=Disabled";
					INumericEntryDialog.vMaxValue = 6;
					INumericEntryDialog.vMinValue = 0;
					INumericEntryDialog.vIntegerOnly = false;
					//Pass control to dialog box and display
					int nResponse = INumericEntryDialog.DoModal();
					//dialog box is now closed, if user pressed select do this
					//if user pressed cancel, do nothing
					if (nResponse == IDOK)
					{
						vDesensitizeIrregularArea = (BYTE)(ATOF(INumericEntryDialog.vEditString) * 10.0);
						
						UpdateDisplay();
						SetChangeMade();
						INumericEntryDialog.vEditString = dtoa(vDesensitizeEdgePixels, 0);
						INumericEntryDialog.m_DialogTitleStaticText1 = "Enter the amount to reduce Threshold by on Edge Pixels.";
						INumericEntryDialog.m_DialogTitleStaticText2 = "0 = Disable Desensitize";
						INumericEntryDialog.m_DialogTitleStaticText3 = "Inspection: " + vInspectionEditing->vName;
						INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " +
							INumericEntryDialog.vEditString + ", Default Value: 0 or 30";

						INumericEntryDialog.m_UnitsString = "Units in pixel brightness (0-255), 0=Disabled";
						INumericEntryDialog.vMaxValue = 255;
						INumericEntryDialog.vMinValue = 0;
						INumericEntryDialog.vIntegerOnly = true;
						//Pass control to dialog box and display
						int nResponse = INumericEntryDialog.DoModal();
						//dialog box is now closed, if user pressed select do this
						//if user pressed cancel, do nothing
						if (nResponse == IDOK)
						{
							vDesensitizeEdgePixels = (BYTE)(ATOF(INumericEntryDialog.vEditString));
							UpdateDisplay();
							SetChangeMade();
						}
						else 
						if (nResponse == 10)
						{
							//Main Menu button pressed
							Sleep(1);
							CDialog::EndDialog(10);
						}
					}
					else 
					if (nResponse == 10)
					{
						//Main Menu button pressed
						Sleep(1);
						CDialog::EndDialog(10);
					}
				}
				else 
				if (nResponse == 10)
				{
					//Main Menu button pressed
					Sleep(1);
					CDialog::EndDialog(10);
				}
			}
		}
		else 
		if (nResponse == 10)
		{
			//Main Menu button pressed
			Sleep(1);
			CDialog::EndDialog(10);
		}
	}
	else
	if ((vInspectionEditing->vInspectionType == cContaminantInspection) ||
		(vInspectionEditing->vInspectionType == cSpotInROIContaminantInspection) ||
		(vInspectionEditing->vInspectionType == cContaminantBonesInspection) ||
		(vInspectionEditing->vInspectionType == cContaminant22Inspection) ||
		(vInspectionEditing->vInspectionType == cGlass13mmContaminantInspection))
	{
		CNumericEntryDialog INumericEntryDialog;  
		INumericEntryDialog.vEditString = dtoa(vFindSpotInSubROI,0);

		INumericEntryDialog.m_DialogTitleStaticText1 = "Number of Pixels to on one edge of Sub ROI to seach for spot";
		INumericEntryDialog.m_DialogTitleStaticText2 = "Spot must be smaller on all sides than this value";
		INumericEntryDialog.m_DialogTitleStaticText3 = "Inspection: " + vInspectionEditing->vName;
		INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " +
			INumericEntryDialog.vEditString + ", Default Value: 10";

		INumericEntryDialog.m_UnitsString = "Units in Pixels, 0 is Disabled";
		INumericEntryDialog.vMaxValue = 250;
		INumericEntryDialog.vMinValue = 7;
		INumericEntryDialog.vAllowZero = true;
		INumericEntryDialog.vIntegerOnly = true;
		//Pass control to dialog box and display
		int nResponse = INumericEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			vFindSpotInSubROI = _wtoi(INumericEntryDialog.vEditString);
			UpdateDisplay();
			SetChangeMade();
			if (vFindSpotInSubROI)
			{
				INumericEntryDialog.vEditString = dtoa(vPixelsStepToNextROI,0);
				INumericEntryDialog.m_DialogTitleStaticText1 = "Enter the next pixel to move ROI for each test";
				INumericEntryDialog.m_DialogTitleStaticText2 = "1 = test every pixel location, 2 = skip every other";
				INumericEntryDialog.m_DialogTitleStaticText3 = "Inspection: " + vInspectionEditing->vName;
				INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " +
					INumericEntryDialog.vEditString + ", Default Value: 3";

				INumericEntryDialog.m_UnitsString = "Units in Pixels, 1 takes more time";
				INumericEntryDialog.vMaxValue = vFindSpotInSubROI;
				INumericEntryDialog.vMinValue = 1;
				INumericEntryDialog.vIntegerOnly = true;
				//Pass control to dialog box and display
				int nResponse = INumericEntryDialog.DoModal();
				//dialog box is now closed, if user pressed select do this
				//if user pressed cancel, do nothing
				if (nResponse == IDOK)
				{
					vPixelsStepToNextROI = _wtoi(INumericEntryDialog.vEditString);
					UpdateDisplay();
					SetChangeMade();

					INumericEntryDialog.vEditString = dtoa(vDesensitizeIrregularArea / 10.0,1);
					INumericEntryDialog.m_DialogTitleStaticText1 = "Enter the Scale Factor for the Desensitize Irregular Area";
					INumericEntryDialog.m_DialogTitleStaticText2 = "0 = Disable Desensitize Irregular Area";
					INumericEntryDialog.m_DialogTitleStaticText3 = "Inspection: " + vInspectionEditing->vName;
					INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " +
						INumericEntryDialog.vEditString + ", Default Value: 3.0";

					INumericEntryDialog.m_UnitsString = "Is a multiplicative factor (0-6), 0=Disabled";
					INumericEntryDialog.vMaxValue = 6;
					INumericEntryDialog.vMinValue = 0;
					INumericEntryDialog.vIntegerOnly = false;
					//Pass control to dialog box and display
					int nResponse = INumericEntryDialog.DoModal();
					//dialog box is now closed, if user pressed select do this
					//if user pressed cancel, do nothing
					if (nResponse == IDOK)
					{
						vDesensitizeIrregularArea = (BYTE)(ATOF(INumericEntryDialog.vEditString) * 10.0);
						
						UpdateDisplay();
						SetChangeMade();
						INumericEntryDialog.vEditString = dtoa(vDesensitizeEdgePixels, 0);
						INumericEntryDialog.m_DialogTitleStaticText1 = "Enter the amount to reduce Threshold by on Edge Pixels.";
						INumericEntryDialog.m_DialogTitleStaticText2 = "0 = Disable Desensitize";
						INumericEntryDialog.m_DialogTitleStaticText3 = "Inspection: " + vInspectionEditing->vName;
						INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " +
							INumericEntryDialog.vEditString + ", Default Value: 0 or 30";

						INumericEntryDialog.m_UnitsString = "Units in pixel brightness (0-255), 0=Disabled";
						INumericEntryDialog.vMaxValue = 255;
						INumericEntryDialog.vMinValue = 0;
						INumericEntryDialog.vIntegerOnly = true;
						//Pass control to dialog box and display
						int nResponse = INumericEntryDialog.DoModal();
						//dialog box is now closed, if user pressed select do this
						//if user pressed cancel, do nothing
						if (nResponse == IDOK)
						{
							vDesensitizeEdgePixels = (BYTE)(ATOF(INumericEntryDialog.vEditString));
							UpdateDisplay();
							SetChangeMade();
						}
						else 
						if (nResponse == 10)
						{
							//Main Menu button pressed
							Sleep(1);
							CDialog::EndDialog(10);
						}
					}
					else 
					if (nResponse == 10)
					{
						//Main Menu button pressed
						Sleep(1);
						CDialog::EndDialog(10);
					}
				}
				else 
				if (nResponse == 10)
				{
					//Main Menu button pressed
					Sleep(1);
					CDialog::EndDialog(10);
				}
			}
		}
		else 
		if (nResponse == 10)
		{
			//Main Menu button pressed
			Sleep(1);
			CDialog::EndDialog(10);
		}
	}
}

void CConfigureProductInspectionSettings::OnSubFunction1Button() 
{
	//if ((vInspectionEditing->vInspectionType == cContaminantInspection) ||
	//	(vInspectionEditing->vInspectionType == cContaminantBonesInspection) ||
	//	(vInspectionEditing->vInspectionType == cAdvancedContaminant2Inspection) ||
	//	(vInspectionEditing->vInspectionType == cSpotInROIContaminantInspection) ||
	//	(vInspectionEditing->vInspectionType == cContaminant22Inspection) ||
	//	(vInspectionEditing->vInspectionType == cGlass13mmContaminantInspection))
	{
		if (vFillInLightSpots >= 2)
			vFillInLightSpots = 0;
		else
			vFillInLightSpots++;

		UpdateDisplay();
		SetChangeMade();
	}
}


void CConfigureProductInspectionSettings::OnSubFunction2Button() 
{
	if (vInspectionEditing->vInspectionType == cJarBottomContaminantInspection)
	{
		//vTryThresholdsDownToThreshold
		CNumericEntryDialog INumericEntryDialog;  
		INumericEntryDialog.vEditString = dtoa(vTryThresholdsDownToThreshold,0);

		INumericEntryDialog.m_DialogTitleStaticText1 = "Jar Bottom Contaminant Search";
		INumericEntryDialog.m_DialogTitleStaticText2 = "Try Thresholds Down To";
		INumericEntryDialog.m_DialogTitleStaticText3 = "Zero is Disabled, Suggest half of Threshold";
		INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + 
			INumericEntryDialog.vEditString;

		INumericEntryDialog.m_UnitsString = "Units in Pixel Intensity" + 
			vLocalConfigurationData->vUnitsString;
		INumericEntryDialog.vMaxValue = 255;
		INumericEntryDialog.vMinValue = 0;
		INumericEntryDialog.vIntegerOnly = true;
		//Pass control to dialog box and display
		int nResponse = INumericEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			vTryThresholdsDownToThreshold = _wtoi(INumericEntryDialog.vEditString);
			UpdateDisplay();
			SetChangeMade();
		}
		else 
		if (nResponse == 10)
		{
			//Main Menu button pressed
			Sleep(1);
			CDialog::EndDialog(10);
		}
	}
	else
	if ((vInspectionEditing->vInspectionType == cContaminantInspection) ||
		(vInspectionEditing->vInspectionType == cGlass13mmContaminantInspection))
	{
		vRemoveJarPunt = !vRemoveJarPunt;
		UpdateDisplay();
		SetChangeMade();
	}
	/*
	else
	if ((vInspectionEditing->vInspectionType == cContaminantInspection) ||
		(vInspectionEditing->vInspectionType == cContaminantBonesInspection) ||
		(vInspectionEditing->vInspectionType == cContaminant22Inspection) ||
		(vInspectionEditing->vInspectionType == cGlass13mmContaminantInspection))
	{
		if (vPreSelectDarkSpotsInImage)
		{
			vPreSelectDarkSpotsInImage = 0;
			SetChangeMade();
			UpdateDisplay();
		}
		else
		{
			CNumericEntryDialog INumericEntryDialog;  
			INumericEntryDialog.vEditString = "3";

			INumericEntryDialog.m_DialogTitleStaticText1 = "Select Dark Spots in image for this inspection";
			INumericEntryDialog.m_DialogTitleStaticText2 = "Enter kernel size to Select Dark Spots in image";
			INumericEntryDialog.m_DialogTitleStaticText3 = "Inspection: " + vInspectionEditing->vName;
			INumericEntryDialog.m_DialogTitleStaticText4 = "Default Value: 3";

			INumericEntryDialog.m_UnitsString = "";
			INumericEntryDialog.vMaxValue = 25;
			INumericEntryDialog.vMinValue = 0;
			INumericEntryDialog.vIntegerOnly = true;
			//Pass control to dialog box and display
			int nResponse = INumericEntryDialog.DoModal();
			//dialog box is now closed, if user pressed select do this
			//if user pressed cancel, do nothing
			if (nResponse == IDOK)
			{
				vPreSelectDarkSpotsInImage = _wtoi(INumericEntryDialog.vEditString);
				UpdateDisplay();
				SetChangeMade();
			}
			else 
			if (nResponse == 10)
			{
				//Main Menu button pressed
				Sleep(1);
				CDialog::EndDialog(10);
			}
		}
	}
	*/
}

void CConfigureProductInspectionSettings::OnSubFunction3Button() 
{
	//bones Median algorithm, 

	if ((vInspectionEditing->vInspectionType != cJarBottomContaminantInspection) &&
		(vInspectionEditing->vInspectionType != cContaminantInspection) &&
		(vInspectionEditing->vInspectionType != cGlass13mmContaminantInspection))
	{
		CNumericEntryDialog INumericEntryDialog;  
		INumericEntryDialog.vEditString = dtoa((vTakeDifferenceFromMedian & 0x0F), 0); //in 4 Least Significant Bits

		INumericEntryDialog.m_DialogTitleStaticText1 = "Enter value for Horizontal Find Bones Algorithm";
		INumericEntryDialog.m_DialogTitleStaticText2 = "Enter number of Pixels to Median in Horizontal";
		INumericEntryDialog.m_DialogTitleStaticText3 = "Inspection: " + vInspectionEditing->vName;
		INumericEntryDialog.m_DialogTitleStaticText4 = "Default Value: 11";

		INumericEntryDialog.m_UnitsString = "Units in Pixels, 0 = Disabled";
		INumericEntryDialog.vMaxValue = 15;
		INumericEntryDialog.vMinValue = 0;
		INumericEntryDialog.vIntegerOnly = true;
		//Pass control to dialog box and display
		int nResponse = INumericEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			vTakeDifferenceFromMedian = ((vTakeDifferenceFromMedian & 0xF0) | (_wtoi(INumericEntryDialog.vEditString) & 0x0F));
			UpdateDisplay();
			SetChangeMade();

			INumericEntryDialog.vEditString = dtoa((vTakeDifferenceFromMedian & 0xF0) >> 4, 0);  //in 4 Most Significant Bits rotate to LSBits

			INumericEntryDialog.m_DialogTitleStaticText1 = "Enter value for Vertical Bones Algorithm";
			INumericEntryDialog.m_DialogTitleStaticText2 = "Enter number of Pixels to Median in Vertical";
			INumericEntryDialog.m_DialogTitleStaticText3 = "Inspection: " + vInspectionEditing->vName;
			INumericEntryDialog.m_DialogTitleStaticText4 = "Default Value: 11";

			INumericEntryDialog.m_UnitsString = "Units in Pixels, 0 = Disabled";
			INumericEntryDialog.vMaxValue = 15;
			INumericEntryDialog.vMinValue = 0;
			INumericEntryDialog.vIntegerOnly = true;
			//Pass control to dialog box and display
			int nResponse = INumericEntryDialog.DoModal();
			//dialog box is now closed, if user pressed select do this
			//if user pressed cancel, do nothing
			if (nResponse == IDOK)
			{
				vTakeDifferenceFromMedian = ((vTakeDifferenceFromMedian & 0x0F) | (_wtoi(INumericEntryDialog.vEditString) << 4));
				UpdateDisplay();
				SetChangeMade();
	/*
				CYesNoDialog TempYesNoDialog;
				TempYesNoDialog.vNoticeText = "\n\n\nDo one Matix, or 2- 1 by X Matrixes?";
				TempYesNoDialog.vQuestionType = cConfirmAbandon;
				TempYesNoDialog.vYesButtonText = "Do One Matrix";
				TempYesNoDialog.vNoButtonText = "Do Two Matrixes";
				int TempResult = TempYesNoDialog.DoModal();
				//if user said OK
				if (TempResult == IDOK)
				{
					vIgnoreQualifier = false; //one matrix
				}
				else
					vIgnoreQualifier = true; // two matrixes

				UpdateDisplay();
				*/
			}
			else 
			if (nResponse == 10)
			{
				//Main Menu button pressed
				Sleep(1);
				CDialog::EndDialog(10);
			}
		}
		else 
		if (nResponse == 10)
		{
			//Main Menu button pressed
			Sleep(1);
			CDialog::EndDialog(10);
		}
	}
	else
	if (vInspectionEditing->vInspectionType == cJarBottomContaminantInspection)
	{
		// vFollowBottomUpAndDown
		vFollowBottomUpAndDown = !vFollowBottomUpAndDown;
		UpdateDisplay();
		SetChangeMade();
	}
	else
	if ((vInspectionEditing->vInspectionType == cContaminantInspection) ||
		(vInspectionEditing->vInspectionType == cGlass13mmContaminantInspection))
	{
		CNumericEntryDialog INumericEntryDialog;  
		INumericEntryDialog.vEditString = dtoa(vSwungBaffleTolerance,0);

		INumericEntryDialog.m_DialogTitleStaticText1 = "Swung Baffle Tolerance Factor";
		INumericEntryDialog.m_DialogTitleStaticText2 = "Number of Pixels accept Threshold Punt above Fitted Punt";
		INumericEntryDialog.m_DialogTitleStaticText3 = "Inspection: " + vInspectionEditing->vName;
		INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " +
			INumericEntryDialog.vEditString + ", Default Value: 0";

		INumericEntryDialog.m_UnitsString = "Units in Pixels, 0 is Disabled";
		INumericEntryDialog.vMaxValue = 100;
		INumericEntryDialog.vMinValue = 0;
		INumericEntryDialog.vIntegerOnly = true;
		//Pass control to dialog box and display
		int nResponse = INumericEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			vSwungBaffleTolerance = _wtoi(INumericEntryDialog.vEditString);
			UpdateDisplay();
			SetChangeMade();
		}
		else 
		if (nResponse == 10)
		{
			//Main Menu button pressed
			Sleep(1);
			CDialog::EndDialog(10);
		}
	}
	/*
	else
	if ((vInspectionEditing->vInspectionType == cContaminantInspection) ||
		(vInspectionEditing->vInspectionType == cGlass13mmContaminantInspection))
	{
		vRequire2InARow = !vRequire2InARow;
		UpdateDisplay();
		SetChangeMade();
	}
	*/
}

void CConfigureProductInspectionSettings::OnSubFunction4Button() 
{
	if (vInspectionEditing->vInspectionType == cJarBottomContaminantInspection)
	{
		//vMaximumDrop
		CNumericEntryDialog INumericEntryDialog;  
		
		//Set dialog box data titles and number value
		INumericEntryDialog.vEditString = dtoa(vMaximumDrop,0);

		INumericEntryDialog.m_DialogTitleStaticText1 = "Jar Bottom Contaminant Maximum Drop in Bottom";
		INumericEntryDialog.m_DialogTitleStaticText2 = "Between Lines";
		INumericEntryDialog.m_DialogTitleStaticText3 = "Zero is Disabled, Default: 4";
		INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + 
			INumericEntryDialog.vEditString;

		INumericEntryDialog.m_UnitsString = "Units in Pixels" + 
			vLocalConfigurationData->vUnitsString;
		INumericEntryDialog.vMaxValue = 100;
		INumericEntryDialog.vMinValue = 0;
		INumericEntryDialog.vIntegerOnly = true;
		//Pass control to dialog box and display
		int nResponse = INumericEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			vMaximumDrop = _wtoi(INumericEntryDialog.vEditString);
			UpdateDisplay();
			SetChangeMade();
		}
		else 
		if (nResponse == 10)
		{
			//Main Menu button pressed
			Sleep(1);
			CDialog::EndDialog(10);
		}
	}
	else
	if ((vInspectionEditing->vInspectionType == cContaminantInspection) ||
		(vInspectionEditing->vInspectionType == cSpotInROIContaminantInspection) ||
		(vInspectionEditing->vInspectionType == cContaminant22Inspection) ||
		(vInspectionEditing->vInspectionType == cGlass13mmContaminantInspection))
	{
		vTrimEdgesOfJar = !vTrimEdgesOfJar;
		UpdateDisplay();
		SetChangeMade();
	}
	else
	if (vInspectionEditing->vInspectionType == cContaminantBonesInspection)
	{
		CNumericEntryDialog INumericEntryDialog;  
		
		//Set dialog box data titles and number value
		INumericEntryDialog.vEditString = dtoa(vIgnoreEdgePixels,0);

		INumericEntryDialog.m_DialogTitleStaticText1 = "Ignore N Pixels as Edge of Product";
		INumericEntryDialog.m_DialogTitleStaticText2 = "Inspection: " + vInspectionEditing->vName;
		INumericEntryDialog.m_DialogTitleStaticText3 = "Zero is Disabled, Default: 0";
		INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + INumericEntryDialog.vEditString;

		INumericEntryDialog.m_UnitsString = "Units in Pixels" + 
			vLocalConfigurationData->vUnitsString;
		INumericEntryDialog.vMaxValue = 100;
		INumericEntryDialog.vMinValue = 0;
		INumericEntryDialog.vIntegerOnly = true;
		//Pass control to dialog box and display
		int nResponse = INumericEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			vIgnoreEdgePixels = _wtoi(INumericEntryDialog.vEditString);
			UpdateDisplay();
			SetChangeMade();
		}
		else 
		if (nResponse == 10)
		{
			//Main Menu button pressed
			Sleep(1);
			CDialog::EndDialog(10);
		}
	}
}

void CConfigureProductInspectionSettings::OnSubFunction5Button() 
{
	vEnable = !vEnable;
	vLocalSystemData->vInspectionNameChanged = true;
	SetChangeMade();
	UpdateDisplay();
}

void CConfigureProductInspectionSettings::OnSubFunction6Button() 
{
	if ((vInspectionEditing->vInspectionType == cContaminantInspection) ||
		(vInspectionEditing->vInspectionType == cIPContaminantInspection) ||
		(vInspectionEditing->vInspectionType == cWireContaminantInspection) ||
		(vInspectionEditing->vInspectionType == cSpotInROIContaminantInspection) ||
		(vInspectionEditing->vInspectionType == cGlass13mmContaminantInspection) ||
		(vInspectionEditing->vInspectionType == cThresholdContaminantInspection) ||
		(vInspectionEditing->vInspectionType == cBadHeartInspection) ||
		(vInspectionEditing->vInspectionType == cDripBySpoutInspection) ||
		(vInspectionEditing->vInspectionType == cDripInspection) ||
		(vInspectionEditing->vInspectionType == cFloodedInspection) ||
		(vInspectionEditing->vInspectionType == cContaminantBonesInspection) ||
		(vInspectionEditing->vInspectionType == cContaminant22Inspection) ||
		(vInspectionEditing->vInspectionType == cJarBottomContaminantInspection))
	{
		{
			CNumericEntryDialog INumericEntryDialog;  
			INumericEntryDialog.vEditString = dtoa(vPreSmoothImage,0);

			INumericEntryDialog.m_DialogTitleStaticText1 = "Pre-smooth image for this inspection";
			INumericEntryDialog.m_DialogTitleStaticText2 = "Enter kernel size to smooth image";
			INumericEntryDialog.m_DialogTitleStaticText3 = "Inspection: " + vInspectionEditing->vName;
			INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " +
				INumericEntryDialog.vEditString + ", Default Value: 3";

			INumericEntryDialog.m_UnitsString = "Units in Pixels, 0 is Disabled";
			INumericEntryDialog.vMaxValue = 25;
			INumericEntryDialog.vMinValue = 0;
			INumericEntryDialog.vIntegerOnly = true;
			//Pass control to dialog box and display
			int nResponse = INumericEntryDialog.DoModal();
			//dialog box is now closed, if user pressed select do this
			//if user pressed cancel, do nothing
			if (nResponse == IDOK)
			{
				vPreSmoothImage = _wtoi(INumericEntryDialog.vEditString);
				UpdateDisplay();
				SetChangeMade();
			}
			else 
			if (nResponse == 10)
			{
				//Main Menu button pressed
				Sleep(1);
				CDialog::EndDialog(10);
			}
		}
	}
}

void CConfigureProductInspectionSettings::OnSubFunction7Button() 
{
	if ((vInspectionEditing->vInspectionType == cContaminantInspection) ||
		(vInspectionEditing->vInspectionType == cIPContaminantInspection) ||
		(vInspectionEditing->vInspectionType == cWireContaminantInspection) ||
		(vInspectionEditing->vInspectionType == cContaminantBonesInspection) ||
		(vInspectionEditing->vInspectionType == cSpotInROIContaminantInspection))
	{
		vTakeDerivative = !vTakeDerivative;
		UpdateDisplay();
		SetChangeMade();
	}
}

void CConfigureProductInspectionSettings::OnSubFunction8Button() 
{
	vInvertEjectLogic = !vInvertEjectLogic;
	SetChangeMade();
	UpdateDisplay();
}

BOOL CConfigureProductInspectionSettings::PreTranslateMessage(MSG* pMsg) 
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
	if (pMsg->message == WM_KEYDOWN)
	{
		CWnd *TempWindow = NULL;
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
				PostThreadMessage(vGlobalCallingThreadID, cShowHelp, 40, 0);

				TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText1);
				if (TempWindow)
					TempWindow->Invalidate(false);
			break;
			case 27: // Escape
				::PostThreadMessage(vGlobalCallingThreadID,	cStartChatWindowMessage, 0,0);
			break;	
		}
		return true;  //true indicates it has been handled, so do not process
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CConfigureProductInspectionSettings::UpdateDisplay() 
{
	if (vFillInLightSpots == 2)
	{
		SetDlgItemText(IDC_SubFunction1Display, _T("Advanced Fill In Light Spots Enabled"));
		SetDlgItemText(IDC_SubFunction1Button, _T("Disable Fill Light Spots"));
	}
	else
	if (vFillInLightSpots == 1)
	{
		SetDlgItemText(IDC_SubFunction1Display, _T("Standard Fill In Light Spots Enabled"));
		SetDlgItemText(IDC_SubFunction1Button, _T("Use Advanced Fill Light Spots"));
	}
	else
	{
		SetDlgItemText(IDC_SubFunction1Display, _T("Fill In Light Spots Disabled"));
		SetDlgItemText(IDC_SubFunction1Button, _T("Enable Fill Light Spots"));
	}

	//if (vInspectionEditing->vPerformGlassAlgorithm == 2)
	int TempShowFunctionButton3 = false;
	if ((vInspectionEditing->vInspectionType == cContaminantInspection) ||
		(vInspectionEditing->vInspectionType == cWireContaminantInspection) ||
		(vInspectionEditing->vInspectionType == cSpotInROIContaminantInspection) ||
		(vInspectionEditing->vInspectionType == cContaminantBonesInspection) ||
		(vInspectionEditing->vInspectionType == cContaminant22Inspection) ||
		(vInspectionEditing->vInspectionType == cIPContaminantInspection) ||
		(vInspectionEditing->vInspectionType == cGlass13mmContaminantInspection))
	{
		if (vInspectionEditing->vInspectionType == cContaminant22Inspection)
		{
			if (vTrimEdgesOfJar)
			{
				SetDlgItemText(IDC_SubFunction4Button, _T("Disable Remove\nBox"));
				SetDlgItemText(IDC_SubFunction4Display, _T("Remove Box Enabled"));
			}
			else
			{
				SetDlgItemText(IDC_SubFunction4Button, _T("Enable Remove Box"));
				SetDlgItemText(IDC_SubFunction4Display, _T("Remove Box Disabled"));
			}
		}
		else
		{
			if (vTrimEdgesOfJar)
			{
				SetDlgItemText(IDC_SubFunction4Button, _T("Disable Trim Jar Edge"));
				SetDlgItemText(IDC_SubFunction4Display, _T("Trim Jar Edge Enabled"));
			}
			else
			{
				SetDlgItemText(IDC_SubFunction4Button, _T("Enable\nTrim Jar Edge"));
				SetDlgItemText(IDC_SubFunction4Display, _T("Trim Jar Edge Disabled"));
			}
		}
		if (vRemoveJarPunt)
		{
			SetDlgItemText(IDC_SubFunction2Button, _T("Don't Remove Jar\nPunt"));
			SetDlgItemText(IDC_SubFunction2Display, _T("Removing Jar Punt"));
		}
		else
		{
			SetDlgItemText(IDC_SubFunction2Button, _T("Remove\nJar\nPunt"));
			SetDlgItemText(IDC_SubFunction2Display, _T("Not Removing Jar Punt"));
		}
		SetDlgItemText(IDC_SubFunction3Button, _T("Swung Baffle Tolerance"));
		if (vSwungBaffleTolerance)
		{
			SetDlgItemText(IDC_SubFunction3Display,"Swung Baffle Tolerance: " + dtoa(vSwungBaffleTolerance,0) + " Pixels");
		}
		else
		{
			SetDlgItemText(IDC_SubFunction3Display, _T("Swung Baffle Tolerance Disabled"));
		}
		

		
		if (vTakeDerivative)
		{
			SetDlgItemText(IDC_SubFunction7Button, _T("Disable Take Derivative"));
			SetDlgItemText(IDC_SubFunction7Display, _T("Take Derivative Enabled"));
		}
		else
		{
			SetDlgItemText(IDC_SubFunction7Button, _T("Enable Take Derivative"));
			SetDlgItemText(IDC_SubFunction7Display, _T("Take Derivative Disabled"));
		}
/*
		SetDlgItemText(IDC_SubFunction4Button,"Glass Center Overlap");
		if (vOverlap)
		{
			//SetDlgItemText(IDC_SubFunction4Button,"Don't Require 2 In a Row");
			SetDlgItemText(IDC_SubFunction4Display,"Center Overlap: " + dtoa(vOverlap,0) + " Pixels");
		}
		else
		{
			//SetDlgItemText(IDC_SubFunction4Button,"Require 2 In a Row");
			SetDlgItemText(IDC_SubFunction4Display,"Center Overlap Disabled");
		}

		if (vRequire2InARow)
		{
			SetDlgItemText(IDC_SubFunction3Button,"Don't Require 2 In a Row");
			SetDlgItemText(IDC_SubFunction3Display,"Requiring 2 In a Row");
		}
		else
		{
			SetDlgItemText(IDC_SubFunction3Button,"Require 2 In a Row");
			SetDlgItemText(IDC_SubFunction3Display,"Not Requiring 2 In a Row");
		}
		if (vPreBlurImage)
		{
			m_SubFunction7Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
			SetDlgItemText(IDC_SubFunction7Button,"Don't\nBlur Image");
			SetDlgItemText(IDC_SubFunction7Display,"Blur Image: " + 
				dtoa(vPreBlurImage,0));
		}
		else
		{
			m_SubFunction7Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
			SetDlgItemText(IDC_SubFunction7Button,"Blur Image");
			SetDlgItemText(IDC_SubFunction7Display,"Not Blurring Image");
		}
		*/
	
		if (vPreSmoothImage)
		{
			m_SubFunction6Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
			SetDlgItemText(IDC_SubFunction6Button, _T("Pre-Smooth"));
			SetDlgItemText(IDC_SubFunction6Display, _T("Smooth Image: ") + dtoa(vPreSmoothImage,0));
		}
		else
		{
			m_SubFunction6Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
			SetDlgItemText(IDC_SubFunction6Button, _T("Pre-Smooth"));
			SetDlgItemText(IDC_SubFunction6Display, _T("Not Smoothing Image"));
		}

		if (vInspectionEditing->vInspectionType == cIPContaminantInspection)
		{
			SetDlgItemText(IDC_Function5Display, "Contrast Threshold: " + dtoa(vIPContaminant, 0));
			SetDlgItemText(IDC_Function5Button, _T("Irregular Product Contamiant"));
		}
		else
		if (vInspectionEditing->vInspectionType == cWireContaminantInspection)
		{
			if (vMinimumLength)
			{
				SetDlgItemText(IDC_Function5Display,"Minimum Length Accepted: " + dtoa(vMinimumLength, 0));
			}
			else
			{
				SetDlgItemText(IDC_Function5Display, _T("Not Checking Length"));
			}
			SetDlgItemText(IDC_Function5Button, _T("Minimum Length"));
		}
		else
		if (vFindSpotInSubROI)
		{
			SetDlgItemText(IDC_Function5Display,"ROI: " + dtoa(vFindSpotInSubROI,0) + ", Step: " + dtoa(vPixelsStepToNextROI,0) +
				", DIA: " + dtoa(vDesensitizeIrregularArea / 10.0,1) + ", DEP: " + dtoa(vDesensitizeEdgePixels,0));
			SetDlgItemText(IDC_Function5Button, _T("Spot in\nSub ROI"));
		}
		else
		{
			SetDlgItemText(IDC_Function5Display, _T("Spot in Sub ROI Disabled"));
			SetDlgItemText(IDC_Function5Button, _T("Spot in\nSub ROI"));
		}

		if ((vInspectionEditing->vInspectionType == cSpotInROIContaminantInspection) || (vInspectionEditing->vInspectionType == cGlass13mmContaminantInspection))
		{
			if (vFindDarkSpotOnLine)
			{
				SetDlgItemText(IDC_Function3Display,"Spot on Line: " + dtoa(vFindDarkSpotOnLine,0) + ", " + dtoa(vPixelsStepToNextROI,0));
				SetDlgItemText(IDC_Function3Button, _T("Disable Spot on Line"));
			}
			else
			{
				SetDlgItemText(IDC_Function3Display, _T("Spot on Line Disabled"));
				SetDlgItemText(IDC_Function3Button, _T("Enable Spot on Line"));
			}
			m_Function3Button.ShowWindow(SW_SHOW);
			m_Function3Display.ShowWindow(SW_SHOW);
		}
		else
		if (vInspectionEditing->vInspectionType == cWireContaminantInspection)
		{
			m_Function3Button.ShowWindow(SW_SHOW);
			m_Function3Display.ShowWindow(SW_SHOW);
			if ((vInspectionEditing->vLookForDarkSpots) && (vPixelLookAhead))
			{
				SetDlgItemText(IDC_Function3Display,"Wire: " + dtoa(vPixelLookAhead,0) + ", " + dtoa(vDesensitizeEdgePixels,0));
				SetDlgItemText(IDC_Function3Button, _T("Wire Settings"));
			}
			else
			{
				SetDlgItemText(IDC_Function3Display, _T("Wire Disabled"));
				SetDlgItemText(IDC_Function3Button, _T("Enable Wire"));
			}
		}
		else
		if ((vInspectionEditing->vInspectionType == cAdvancedContaminant2Inspection) || (vInspectionEditing->vInspectionType == cContaminantBonesInspection))
		{
			if (vIgnoreEdgePixels)
			{
				SetDlgItemText(IDC_SubFunction4Display, "Trim Edges: " + dtoa(vIgnoreEdgePixels,0));
				SetDlgItemText(IDC_SubFunction4Button, _T("Trim Edges of Product"));
			}
			else
			{
				SetDlgItemText(IDC_SubFunction4Display, _T("Trim Edges Disabled"));
				SetDlgItemText(IDC_SubFunction4Button, _T("Trim Edges of Product"));
			}

			if (vTakeDifferenceFromMedian)
			{
				CString TempText = "";
				if (vTakeDifferenceFromMedian & 0x80)
					SetDlgItemText(IDC_Function3Display,"Bones Diff." + TempText + ": " + dtoa(vTakeDifferenceFromMedian & 0x7F, 0) + ", " + dtoa(vPixelsStepToNextROI, 0));
				else
					SetDlgItemText(IDC_Function3Display,"Bones Median" + TempText + ": " + dtoa(vTakeDifferenceFromMedian & 0x7F, 0));
			}
			else
			if (vBonesInspection)
			{
				SetDlgItemText(IDC_Function3Display, "Large Bones: " + dtoa(vBonesInspection,0) + ", Transition: " + dtoa(vPixelLookAhead,0));
				SetDlgItemText(IDC_Function3Button, _T("Disable Spot on Line"));
			}
			else
			{
				SetDlgItemText(IDC_Function3Display, _T("Bones Disabled"));
			}
			SetDlgItemText(IDC_Function3Button, _T("Configure Bones"));
			m_Function3Button.ShowWindow(SW_SHOW);
			m_Function3Display.ShowWindow(SW_SHOW);
		}
		else
		{
			m_Function3Button.ShowWindow(SW_HIDE);
			m_Function3Display.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_Function3Button.GetRectToInvalidateArrow());
		}
	}
	/*
	if ((vInspectionEditing->vInspectionType != cJarBottomContaminantInspection) &&
		(vInspectionEditing->vInspectionType != cContaminantInspection) &&
		(vInspectionEditing->vInspectionType != cGlass13mmContaminantInspection))
	{
		SetDlgItemText(IDC_SubFunction3Button,"\nBones");
		if (vTakeDifferenceFromMedian)
		{
			CString TempText = "";
			if (vTakeDifferenceFromMedian & 0x0F)
				TempText = dtoa(vTakeDifferenceFromMedian & 0x0F, 0) + "H";
			if (vTakeDifferenceFromMedian & 0xF0)
			{
				if (vTakeDifferenceFromMedian & 0x0F)
					TempText = TempText + ", ";
				TempText = TempText + dtoa((vTakeDifferenceFromMedian & 0xF0) >> 4, 0) + "V";
			}
			SetDlgItemText(IDC_SubFunction3Display,"Find Bones: " + TempText);
			SetDlgItemText(IDC_SubFunction3Button,"\nBones");
		}
		else
		{
			SetDlgItemText(IDC_SubFunction3Display,"Find Bones Disabled");
		}
	}
	*/
	
	if (vInspectionEditing->vInspectionType == cJarBottomContaminantInspection)
	{
		if (vPreSmoothImage)
		{
			m_SubFunction6Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
			SetDlgItemText(IDC_SubFunction6Button, _T("Pre-Smooth"));
			SetDlgItemText(IDC_SubFunction6Display, _T("Smooth Image: ") + dtoa(vPreSmoothImage,0));
		}
		else
		{
			m_SubFunction6Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
			SetDlgItemText(IDC_SubFunction6Button, _T("Pre-Smooth"));
			SetDlgItemText(IDC_SubFunction6Display, _T("Not Smoothing Image"));
		}

		if (vTryThresholdsDownToThreshold)
		{
			SetDlgItemText(IDC_SubFunction2Button, _T("Don't Try Lower Thresholds"));
			SetDlgItemText(IDC_SubFunction2Display,"Trying Threshold Down To: " + dtoa(vTryThresholdsDownToThreshold,0));
		}
		else
		{
			SetDlgItemText(IDC_SubFunction2Button, _T("Try Thresholds Down To"));
			SetDlgItemText(IDC_SubFunction2Display, _T("Trying only set Threshold"));
		}
		if (vFollowBottomUpAndDown)
		{
			SetDlgItemText(IDC_SubFunction3Button, _T("Follow Bottom Down Only"));
			SetDlgItemText(IDC_SubFunction3Display, _T("Following Bottom Up and Down"));
		}
		else
		{
			SetDlgItemText(IDC_SubFunction3Button, _T("Follow Bottom Up and Down"));
			SetDlgItemText(IDC_SubFunction3Display, _T("Following Bottom Down Only"));
		}
		SetDlgItemText(IDC_SubFunction4Display,dtoa(vMaximumDrop,0));
	}
	
	if (vInvertEjectLogic)
	{
		SetDlgItemText(IDC_SubFunction8Button, _T("Normal\nEject\nLogic"));
		SetDlgItemText(IDC_SubFunction8Display, _T("Eject Logic Inverted"));
	}
	else
	{
		SetDlgItemText(IDC_SubFunction8Button, _T("Invert\nEject\nLogic"));
		SetDlgItemText(IDC_SubFunction8Display, _T("Eject Logic Normal"));
	}
	if (vEnable)
	{
		SetDlgItemText(IDC_SubFunction5Button, _T("Disable\nThis\nInspection"));
		SetDlgItemText(IDC_SubFunction5Display, _T("Inspection Enabled"));
	}
	else
	{
		SetDlgItemText(IDC_SubFunction5Button, _T("Enable\nThis\nInspection"));
		SetDlgItemText(IDC_SubFunction5Display, _T("Inspection Disabled"));
	}
}

void CConfigureProductInspectionSettings::SetChangeMade() 
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


HBRUSH CConfigureProductInspectionSettings::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	HWND TempDialogHwnd = this->m_hWnd;
	int TextSize;
	pDC->SetAttribDC(pDC->m_hDC);
	
	// TODO: Change any attributes of the DC here
	if (pWnd->GetDlgCtrlID() == IDC_DialogTitleStaticText1)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DialogTitleStaticText, 8); 
		pDC->SetTextColor(vGlobalDialogTitleColor);
	}

  if (pWnd->GetDlgCtrlID() == IDC_SubFunction5Display)
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction5Button, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		if (!vEnable)
			return vLocalSystemData->vYellowBrush;
		else
			return vLocalSystemData->vGreenBrush;
	}

  if (pWnd->GetDlgCtrlID() == IDC_SubFunction8Display)
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction8Button, 5);  //5 is medium large
	if (vInvertEjectLogic)
	{
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vYellowBrush;
	}
	}

	if (pWnd->GetDlgCtrlID() == IDC_Background)
	{
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_ProductNameTitle)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_ProductNameTitle, 5); 
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_ProductNameTitle2)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_ProductNameTitle, 5); 
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
  }
	if (pWnd->GetDlgCtrlID() == IDC_Function3Display)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Function3Display, 5);
	if (pWnd->GetDlgCtrlID() == IDC_Function5Display)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Function5Display, 5);
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction1Display)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction1Display, 5);
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction2Display)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction2Display, 5);
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction3Display)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction3Display, 5);  
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction4Display)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction4Display, 5); 
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction5Display)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction5Display, 5);
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction6Display)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction6Display, 5);
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction7Display)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction7Display, 5); 		
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction8Display)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction8Display, 5); 
	if (pWnd->GetDlgCtrlID() == IDC_ProcessStatus3)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_ProcessStatus3, 5); 
	
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
		if (vFillInLightSpots)
			return vLocalSystemData->vYellowBrush;
		else
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


void CConfigureProductInspectionSettings::OnDialogtitlestatictext1()
{
	BYTE TempHiddenKeys = 8;
	if (!PasswordOK(cTemporaryInspxPassword,false))
		TempHiddenKeys = 10;
	PostThreadMessage(vGlobalCallingThreadID, cDisplaySoftKeyMenu, TempHiddenKeys, TempHiddenKeys);
}
