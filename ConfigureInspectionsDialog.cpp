//ScanTrac Side View Source File
// ConfigureInspectionsDialog.cpp : implementation file
//

#include "stdafx.h"
#include "scantrac.h"
#include "ConfigureInspectionsDialog.h"
#include "SelectInspectionDialog.h"
#include "SelectItemDialog.h"
#include "AlphaEntryDialog.h"
#include "NoticeDialog.h"
#include "YesNoDialog.h"
#include "Inspection.h"
#include "InspectionContaminant.h"
#include "InspectionVoid.h"
#include "InspectionDensity.h"
#include "InspectionStandardDeviation.h"
#include "InspectionKernel.h"
#include "InspectionSize.h"
#include "InspectionCheckWeigh.h"
#include "ConfigureProductContaminantProcessesDialog.h"
#include "NewConfigProductEjectorDialog.h"
#include "NewConfigureProductExternalDetectorDialog.h"
#include "InspectionBottomContaminant.h"
//#include "InspectionGlassContaminant.h"
#include "NumericEntryDialog.h"
#include "ConfigureProductAlarmsDialog.h"
#include "ProductFillerSeamerSampling.h"
#include "ConfigureProductInspectionOptions.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern BYTE vGlobalNumberOfImagesUsing;
extern BYTE vGlobalScanTracType;
extern BYTE vGlobalShiftKeyDown;
extern CProduct *vGlobalCurrentProduct;
extern int vGlobalPasswordLevel;
extern unsigned long vGlobalDialogTitleColor;
/////////////////////////////////////////////////////////////////////////////
// CConfigureInspectionsDialog dialog


CConfigureInspectionsDialog::CConfigureInspectionsDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CConfigureInspectionsDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConfigureInspectionsDialog)
	//}}AFX_DATA_INIT
	vHaveExternalDetectorsConfigured = false;
	vInitialized = false;
	vGrabFocusTimerHandle = 31334;
	vShowReturnToEditMenu = false;
	ShowFinishAutoSetupButton = false;
	vNewInspection = false;
	vPickOneToEvaluate = false;
	vInspectionToEdit = NULL;
	vChangeMade = false;
	vMainWindowPointer = NULL;
	vLocalSystemData = NULL;
	vLocalProductCollection = NULL;
	vLocalConfigurationData = NULL;
	vProductEditing = NULL;
	vEjector = 0;
	vSelectedEvaluate = false;
	vOneSecondTimerHandle = 31335;
	vOldXRayOnNotice = false;
	vLocalCWndCollection.Add(this);
	vLocalCWndCollection.Add(&m_InspectionList);
	vLocalCWndCollection.Add(&m_SubFunction4Display);
	vLocalCWndCollection.Add(&m_SubFunction3Display);
	vLocalCWndCollection.Add(&m_Function1Button);
	vLocalCWndCollection.Add(&m_SubFunction7Button);
	vLocalCWndCollection.Add(&m_SubFunction6Button);
	vLocalCWndCollection.Add(&m_XRaysOn);
	vLocalCWndCollection.Add(&m_SubFunction5Button);
	vLocalCWndCollection.Add(&m_SubFunction4Button);
	vLocalCWndCollection.Add(&m_SubFunction3Button);
	vLocalCWndCollection.Add(&m_SubFunction2Button);
	vLocalCWndCollection.Add(&m_SubFunction1Button);
	vLocalCWndCollection.Add(&m_Function2Button);
	vLocalCWndCollection.Add(&m_Function4Button);
	vLocalCWndCollection.Add(&m_Function5Button);
	vLocalCWndCollection.Add(&m_Background);
	vLocalCWndCollection.Add(&m_Function3Button);
	vLocalCWndCollection.Add(&m_CPProductLabel);
	vLocalCWndCollection.Add(&m_DialogTitleStaticText);
	vLocalCWndCollection.Add(&m_SubFunction8Button);
}

void CConfigureInspectionsDialog::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	m_Background.SetWindowPos(NULL,cMainBackgroundLeft,
		cMainBackgroundTop,cMainBackgroundLength,
		cMainBackgroundHeight,SWP_NOZORDER);
	if (vProductEditing)
	{  //if there are no inspections, then add one
		if (vProductEditing->vNumberOfInspections == 0)
			OnSubFunction8Button();
	}
	else
		OnFunction1Button();  //exit if no product editing

	for (BYTE TempLoop = 0; TempLoop < cNumberOfExternalDetectors; TempLoop++)
	if (vLocalConfigurationData->vExternalDetector[TempLoop].vEnabled)
		vHaveExternalDetectorsConfigured = true;

	vNumberEjectorsConfigured = 0;
	vOnlyEjectorNumberConfigured = 0;
	for (BYTE TempLoop = 0; TempLoop < cNumberOfEjectors; TempLoop++)
	{
		if ((vLocalConfigurationData->vEjector[TempLoop].vEnabled) &&
			(vLocalConfigurationData->vEjector[TempLoop].vLineBitMask))
		{
			vNumberEjectorsConfigured++;
			vOnlyEjectorNumberConfigured = TempLoop;
		}
	}
	CString TempText = " ";
	CString TempText1 = " ";
	TempText.LoadString(IDS_XRAYON);
	SetDlgItemText(IDC_XRaysOn,TempText);

	UpdateCurrentProductOnDisplay();
	m_Function2Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
	m_Function2Button.ShowWindow(SW_SHOW);
	if (vLocalSystemData->vInAutoSetup)
	{
		TempText.LoadString(IDS_AutoSetupProductInspections);
		TempText1.LoadString(IDS_FinishAutoSetup);
	}
	else
	{
		TempText.LoadString(IDS_SetupProductInspections);
		TempText1.LoadString(IDS_FinishAutoSetup);
	}
	SetDlgItemText(IDC_DialogTitleStaticText1, TempText);
	SetDlgItemText(IDC_Function2Button,TempText1);
	m_Function4Button.ShowWindow(SW_SHOW);


	if (!vProductEditing)
		vProductEditing = vGlobalCurrentProduct;
	if (vProductEditing)
	if (!vInspectionToEdit)
		vInspectionToEdit = vProductEditing->vInspection[0];

	TempText.LoadString(IDS_BackToLearn);
	SetDlgItemText(IDC_Function4Button,TempText);

	m_Function5Button.ShowWindow(SW_SHOW);

	//if (vLocalSystemData->vInAutoSetup)
		TempText.LoadString(IDS_NextToROI);
	//else
	//	TempText.LoadString(IDS_ChangeROI);
	SetDlgItemText(IDC_Function5Button,TempText);

	if ((vShowReturnToEditMenu) || (!vLocalSystemData->vInAutoSetup))
	{
		m_Function1Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		TempText.LoadString(IDS_FinishSetup);
		SetDlgItemText(IDC_Function2Button,TempText);
		TempText.LoadString(IDS_AbortSetup);
	}
	else
	{
		m_Function1Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
		TempText.LoadString(IDS_Abort);
	}
	SetDlgItemText(IDC_Function1Button,TempText);

	TempText.LoadString(IDS_Evaluate);
	SetDlgItemText(IDC_SubFunction5Button,TempText);

	if ((vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType]))
	{
		SetDlgItemText(IDC_Function3Button, _T("Filler Offsets"));
	}
	else if ((vLocalConfigurationData->vNumberOfHeadsToMonitor[cSeamerSamplingType]))
	{
		SetDlgItemText(IDC_Function3Button, _T("Seamer Offsets"));
	}
	else
	{
		TempText.LoadString(IDS_JumpToStart);
		SetDlgItemText(IDC_Function3Button,TempText);
	}

	//TempText.LoadString(IDS_Edit);  //edit inspection ROI
	//SetDlgItemText(IDC_SubFunction1Button,TempText);

	TempText.LoadString(IDS_MakeInspectionLast);
	SetDlgItemText(IDC_SubFunction6Button,TempText);

	TempText.LoadString(IDS_Delete);
	SetDlgItemText(IDC_SubFunction7Button,TempText);

	TempText.LoadString(IDS_New);
	SetDlgItemText(IDC_SubFunction8Button,TempText);

	TempText.LoadString(IDS_SetupEjectorTiming);
	SetDlgItemText(IDC_SubFunction1Button,TempText);

	TempText.LoadString(IDS_SetXofYAlarm);
	SetDlgItemText(IDC_SubFunction3Button,TempText + " 1");

	SetDlgItemText(IDC_SubFunction4Button, TempText + " 2");

	TempText.LoadString(IDS_Options);
	SetDlgItemText(IDC_SubFunction2Button, TempText);

	this->SetWindowText(_T("Inspections"));
	if (vPickOneToEvaluate)
	{
		vSelectedEvaluate = true;
		SelectAndEdit(cSelectInspectionForEvaluate);
	}

	DisplayXofYAlarm();
	OneSecondUpdate();
	int TimerResult = SetTimer(vOneSecondTimerHandle,1000,NULL);
	if (!TimerResult)
		ReportErrorMessage("Error-Second Timer Failed",cEMailInspx,32000);
	TimerResult = SetTimer(vGrabFocusTimerHandle,100,NULL); //was 1000
	if (!TimerResult)
		ReportErrorMessage("Error-GrabFocus Timer Failed",cEMailInspx, 32000);

	SetupMenu(vLocalCWndCollection);

	//do after setup menu as it has modified sizes in it
	ShowInspxButtons();
}

void CConfigureInspectionsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfigureInspectionsDialog)
	DDX_Control(pDX, IDC_InspectionList, m_InspectionList);
	DDX_Control(pDX, IDC_SubFunction4Display, m_SubFunction4Display);
	DDX_Control(pDX, IDC_SubFunction3Display, m_SubFunction3Display);
	DDX_Control(pDX, IDC_Function1Button, m_Function1Button);
	DDX_Control(pDX, IDC_SubFunction7Button, m_SubFunction7Button);
	DDX_Control(pDX, IDC_SubFunction6Button, m_SubFunction6Button);
	DDX_Control(pDX, IDC_XRaysOn, m_XRaysOn);
	DDX_Control(pDX, IDC_SubFunction5Button, m_SubFunction5Button);
	DDX_Control(pDX, IDC_SubFunction4Button, m_SubFunction4Button);
	DDX_Control(pDX, IDC_SubFunction3Button, m_SubFunction3Button);
	DDX_Control(pDX, IDC_SubFunction2Button, m_SubFunction2Button);
	DDX_Control(pDX, IDC_SubFunction1Button, m_SubFunction1Button);
	DDX_Control(pDX, IDC_Function2Button, m_Function2Button);
	DDX_Control(pDX, IDC_Function4Button, m_Function4Button);
	DDX_Control(pDX, IDC_Function5Button, m_Function5Button);
	DDX_Control(pDX, IDC_Function3Button, m_Function3Button);
	DDX_Control(pDX, IDC_Background, m_Background);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_CPProductLabel, m_CPProductLabel);
	DDX_Control(pDX, IDC_DialogTitleStaticText1, m_DialogTitleStaticText);
	DDX_Control(pDX, IDC_SubFunction8Button, m_SubFunction8Button);
}


BEGIN_MESSAGE_MAP(CConfigureInspectionsDialog, CDialog)
	//{{AFX_MSG_MAP(CConfigureInspectionsDialog)
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
	ON_WM_CTLCOLOR()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_STN_CLICKED(IDC_DialogTitleStaticText1, &CConfigureInspectionsDialog::OnDialogtitlestatictext1)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfigureInspectionsDialog message handlers

void CConfigureInspectionsDialog::OnFunction1Button() 
{
	// User clicked Main Menu Button to exit 
	if (vInitialized)
	if (AbortSetupChanges())
	{
		CDialog::EndDialog(10);
	}
}

void CConfigureInspectionsDialog::OnFunction2Button() 
{
	//Exit button pressed
	if (vInitialized)
	if (KeepSetupChanges())
	{
		vInspectionToEdit = NULL;
		CDialog::EndDialog(20);
	}
}

void CConfigureInspectionsDialog::OnFunction3Button() 
{
	//Jump To Start Button
	if ((vLocalConfigurationData->vNumberOfHeadsToMonitor[cFillerSamplingType]) || (vLocalConfigurationData->vNumberOfHeadsToMonitor[cSeamerSamplingType]))
	{
		CProductFillerSeamerSampling TempProductFillerSeamerSampling;
		TempProductFillerSeamerSampling.vProductEditing = vGlobalCurrentProduct;
		TempProductFillerSeamerSampling.vLocalProductCollection = vLocalProductCollection;
		TempProductFillerSeamerSampling.vMainWindowPointer = vMainWindowPointer;
		TempProductFillerSeamerSampling.vLocalConfigurationData = vLocalConfigurationData;
		TempProductFillerSeamerSampling.vLocalSystemData = vLocalSystemData;

		int TempResult = TempProductFillerSeamerSampling.DoModal();
		if (TempResult == IDOK)  //indicates selected an inspection to edit or evaluate
		{
			SetChangeMade();
		}
		else
		if (TempResult == 20)
		{
//			ExitAutoSetupDialog(20);
		}
		else
		if (TempResult == IDCANCEL)
		{
	//		StartLearnMenu();
		}
		else 
		if (TempResult == 10)
		{
			//abort button pressed
			OnFunction1Button();
		}
	}
	else
	if (vInitialized)
	{
		vInspectionToEdit = NULL;
		CDialog::EndDialog(5);
	}
}

void CConfigureInspectionsDialog::OnFunction4Button() 
{
	// Previous Menu
	if (vInitialized)
	{
		vProductEditing = NULL;
		vInspectionToEdit = NULL;
		CDialog::OnCancel();
	}
}

void CConfigureInspectionsDialog::OnFunction5Button() 
{
	// Edit Inspection ROI
	if (vInitialized)
	if (vProductEditing->vNumberOfInspections)
		SelectAndEdit(cSelectInspectionForEdit);
}

void CConfigureInspectionsDialog::OnSubFunction1Button() 
{
	//if (((!vLocalSystemData->vXRayOnNotice) || (vStartingUpSource)) &&
	//	(vMainWindowPointer->InterlocksOK()))
	//	StartingSystemPleaseTryAgain();
	//else
	if (vInitialized)
		SetupProductEjectors();
}

void CConfigureInspectionsDialog::OnSubFunction2Button() 
{
	if (PasswordOK(cSuperInspxPassword,false))
	{
		CConfigureProductInspectionOptions IConfigureProductInspectionOptions;
		//Set dialog box data like how many products there are
		IConfigureProductInspectionOptions.vProductEditing = vProductEditing;
		IConfigureProductInspectionOptions.vLocalConfigurationData = vLocalConfigurationData;
		IConfigureProductInspectionOptions.vLocalSystemData = vLocalSystemData;
		IConfigureProductInspectionOptions.vMainWindowPointer = vMainWindowPointer;
		IConfigureProductInspectionOptions.vLocalProductCollection = vLocalProductCollection;
		//Pass control to dialog box and display
		//IEditProductDialog.vEPLocalNumberOfProducts = vCPLocalNumberOfProducts;
		int nResponse = IConfigureProductInspectionOptions.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if ((nResponse == IDOK) || (nResponse == 20))
		{
			// TODO: Place code here to handle when the dialog is accepted
			if (IConfigureProductInspectionOptions.vChangeMade)
				SetChangeMade();
			UpdateInspectionsOnDisplay();
		}
		else
		if (nResponse == 10)
		{
			//  Main Menu button was pressed
			CDialog::EndDialog(10);
		}
	}
	else
	if (vHaveExternalDetectorsConfigured)
	{
		CNewConfigureProductExternalDetectorDialog IConfigureProductExternalDetectorDialog;

		//Set dialog box data like how many products there are
		IConfigureProductExternalDetectorDialog.vLocalCurrentProductPointer = vProductEditing;
		IConfigureProductExternalDetectorDialog.vLocalConfigurationData = vLocalConfigurationData;
		IConfigureProductExternalDetectorDialog.vLocalSystemData = vLocalSystemData;
		IConfigureProductExternalDetectorDialog.vInAutoSetup = vLocalSystemData->vInAutoSetup;
		IConfigureProductExternalDetectorDialog.vMainWindowPointer = vMainWindowPointer;

		//Pass control to dialog box and display
		//IEditProductDialog.vEPLocalNumberOfProducts = vCPLocalNumberOfProducts;
		int nResponse = IConfigureProductExternalDetectorDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if ((nResponse == IDOK) || ((vLocalSystemData->vInAutoSetup) && (nResponse == 2)))
		{
			// TODO: Place code here to handle when the dialog is accepted
			if (IConfigureProductExternalDetectorDialog.vChangeMade)
				SetChangeMade();
		}
		else
		if (nResponse == 10)
		{
			//  Main Menu button was pressed
			CDialog::EndDialog(10);
		}
	}
	else
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\n\nNo Auxiliary Detectors configured in System.";
		TempNoticeDialog.vType = cErrorMessage;
		TempNoticeDialog.DoModal();
	}
}

void CConfigureInspectionsDialog::OnSubFunction3Button() 
{
	if (vInitialized)
	{
		EditAlarm(0);
		DisplayXofYAlarm();
	}
}

void CConfigureInspectionsDialog::OnSubFunction4Button() 
{
	if (vInitialized)
	{
		EditAlarm(1);
		DisplayXofYAlarm();
	}
}

void CConfigureInspectionsDialog::OnSubFunction5Button() 
{
	if (vInitialized)
	if (vProductEditing->vNumberOfInspections)
	{
		vSelectedEvaluate = true;
		SelectAndEdit(cSelectInspectionForEvaluate);
	}
}

void CConfigureInspectionsDialog::OnSubFunction6Button() 
{
	// Make Inspection Last Priority
	if (vInitialized)
	if(vProductEditing)
	//user cannot delete the last inspection, but Inspx personel with password may.
	if (vProductEditing->vNumberOfInspections > 1)
	{
		//Create dialog box
		CSelectInspectionDialog ISelectInspectionDialog;  
		//Set dialog box data like which square to start on
		ISelectInspectionDialog.vLocalSystemData = vLocalSystemData;
		ISelectInspectionDialog.vProductEditing = vProductEditing;
		ISelectInspectionDialog.SelectWindowForStyle = cSelectInspectionForLastPriority;
		//Pass control to dialog box and display
		int nResponse = ISelectInspectionDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{ //  closed with OK
			int TempInspectionNumberToMove = ISelectInspectionDialog.vInspectionNumberSelected;
			if (TempInspectionNumberToMove > 0)
			if (TempInspectionNumberToMove < vProductEditing->vNumberOfInspections)
			{
				vInspectionToEdit = NULL;
				TempInspectionNumberToMove--; //make it zero based
				if (vProductEditing->vInspection[TempInspectionNumberToMove])
				{
					int TempResult = IDOK;
					CYesNoDialog TempYesNoDialog;
					CString TempText;
					CString TempText1;
					TempText.LoadString(IDS_MoveInspection);
					TempText1.LoadString(IDS_toLastPriority);
					CString TempString("\n\n" + TempText + ": " + vProductEditing->vInspection[TempInspectionNumberToMove]->vName + " " + TempText1 + "?");

						if (vLocalSystemData->vSystemRunMode == cRunningSystemMode)
							TempString = TempString + "\nThis will generate a Production Report and clear the counters.";

					TempYesNoDialog.vNoticeText = TempString;
					TempYesNoDialog.vYesButtonText = TempText;
					TempYesNoDialog.vQuestionType = cConfirmQuestion;
					TempResult = TempYesNoDialog.DoModal();
					if (TempResult == IDOK)
					{
						if (vLocalSystemData->vSystemRunMode == cRunningSystemMode)
						{
							vMainWindowPointer->MakeProductionReportIfNeeded();
							vMainWindowPointer->ResetCounters();
						}

						BYTE TempLastInspection = vProductEditing->vNumberOfInspections;
						vLocalSystemData->vInspectionNameChanged = true;
						vLocalSystemData->vInspectionNameMoved = true;
						CInspection *TempInspection = vProductEditing->vInspection[TempInspectionNumberToMove];
						if (TempLastInspection - 1 > TempInspectionNumberToMove) //only move inspection if it was not last already
						if (vLocalSystemData->vITIPCDig->vContainerCollectionLock.Lock())
						{ //move all the inspection pointers down one 
							vProductEditing->vTypesOfRejectsToView = 0xFFFF; //enable all inspections for types of rejects to display
							DWORD TempBitMaskOfInspectionToMove = 1;
							for (BYTE TempLoop = 0; TempLoop < TempInspectionNumberToMove; TempLoop++)
								TempBitMaskOfInspectionToMove = TempBitMaskOfInspectionToMove * 2;

							bool TempInspectionMovingAlarmOneSelected = false;
							if ((vProductEditing->vInspectionAlarmSettings[0] & TempBitMaskOfInspectionToMove) > 0) //remember setting of inspection to make last on first alarm
								TempInspectionMovingAlarmOneSelected = true;

							bool TempInspectionMovingAlarmTwoSelected = false;
							if ((vProductEditing->vInspectionAlarmSettings[1] & TempBitMaskOfInspectionToMove) > 0) //remember setting of inspection to make last on second alarm
								TempInspectionMovingAlarmTwoSelected = true;

							for (BYTE TempLoop = TempInspectionNumberToMove; TempLoop < TempLastInspection - 1; TempLoop++)
								vProductEditing->vInspection[TempLoop] = vProductEditing->vInspection[TempLoop + 1];

							for (BYTE TempLoop = TempInspectionNumberToMove + 1; TempLoop < TempLastInspection; TempLoop++)
							{
								DWORD TempBitVector = 1;
								DWORD TempOriginalBitVector = 1;
								for (BYTE TempBitLoop = 0; TempBitLoop < TempLoop; TempBitLoop++)
									TempBitVector = TempBitVector * 2;
								TempOriginalBitVector = TempBitVector;  //this is the bit for the inspection you are about to copy down one slot

								for (BYTE TempAlarmLoop = 0; TempAlarmLoop < 2; TempAlarmLoop++)
								{
									TempBitVector = TempOriginalBitVector & vProductEditing->vInspectionAlarmSettings[TempAlarmLoop];  //This is the bit for the inspection above to move down one
									TempBitVector = TempBitVector / 2; //move that bit down one
									if (TempBitVector > 0)  //if the bit is positive, OR it so it sets this bit
										vProductEditing->vInspectionAlarmSettings[TempAlarmLoop] = vProductEditing->vInspectionAlarmSettings[TempAlarmLoop] | TempBitVector;
									else
									{
										TempBitVector = ~TempOriginalBitVector;  //if the bit is to be cleared, invert the original bit so can AND the mask
										TempBitVector = TempBitVector / 2;  //move it down one
										TempBitVector = TempBitVector | cDWORDLastSignificantDigitValue; //mask on the bit to say use this mask
										vProductEditing->vInspectionAlarmSettings[TempAlarmLoop] = vProductEditing->vInspectionAlarmSettings[TempAlarmLoop] & TempBitVector; //clear that bit by AND with inverted mask
									}
								}
							}
							//add inspection in at end
							vProductEditing->vInspection[TempLastInspection - 1] = TempInspection; //point to last inspection

							DWORD TempBitMaskOfLastInspection = 1;
							for (BYTE TempBitLoop = 0; TempBitLoop < TempLastInspection-1; TempBitLoop++)
								TempBitMaskOfLastInspection = TempBitMaskOfLastInspection * 2;
							DWORD TempOriginalBitVector = TempBitMaskOfLastInspection;  //bit of last inspection
							if (TempInspectionMovingAlarmOneSelected)
								vProductEditing->vInspectionAlarmSettings[0] = vProductEditing->vInspectionAlarmSettings[0] | TempOriginalBitVector;
							else
							{
								TempBitMaskOfLastInspection = ~TempOriginalBitVector;
								vProductEditing->vInspectionAlarmSettings[0] = vProductEditing->vInspectionAlarmSettings[0] & TempBitMaskOfLastInspection;
							}

							if (TempInspectionMovingAlarmTwoSelected)
								vProductEditing->vInspectionAlarmSettings[1] = vProductEditing->vInspectionAlarmSettings[1] | TempOriginalBitVector;
							else
							{
								TempBitMaskOfLastInspection = ~TempOriginalBitVector;
								vProductEditing->vInspectionAlarmSettings[1] = vProductEditing->vInspectionAlarmSettings[1] & TempBitMaskOfLastInspection;
							}
							vLocalSystemData->vITIPCDig->vContainerCollectionLock.Unlock();
						}
					}
					vProductEditing->MatchInspectionsForWeightTrending(); //if an inspection is moved, the weight trending matching inspection must be re-paired
				}
			}
			else
			{
				CNoticeDialog TempNoticeDialog;
				CString TempText;
				TempText.LoadString(IDS_Inspectionislastalready);//"\n\n\nInspection is last already."
				TempNoticeDialog.vNoticeText = TempText;
				TempNoticeDialog.vType = cNoticeMessage;
				TempNoticeDialog.DoModal();
			}
			UpdateInspectionsOnDisplay();
			vMainWindowPointer->SetupRejectReasonNames();
		}
		else 
		if (nResponse == 10)
		{
			//Main Menu Button Pressed
			CDialog::EndDialog(10);
		}
	}
}

void CConfigureInspectionsDialog::OnSubFunction7Button() 
{
	// Delete Inspection
	if (vInitialized)
	if(vProductEditing)
		//user cannot delete the last inspection, but Inspx personel with password may.
  {
		if ((vProductEditing->vNumberOfInspections > 1) ||
			((PasswordOK(cTemporaryInspxPassword,false)) && (vProductEditing->vNumberOfInspections)))
		{
			//Create dialog box
			CSelectInspectionDialog ISelectInspectionDialog;  
			//Set dialog box data like which square to start on
			ISelectInspectionDialog.vLocalSystemData = vLocalSystemData;
			ISelectInspectionDialog.vProductEditing = vProductEditing;
			ISelectInspectionDialog.SelectWindowForStyle = cSelectInspectionForDelete;
			//Pass control to dialog box and display
			int nResponse = ISelectInspectionDialog.DoModal();
			//dialog box is now closed, if user pressed select do this
			//if user pressed cancel, do nothing
			if (nResponse == IDOK)
			{ //  closed with OK
				int TempInspectionNumberToDelete = ISelectInspectionDialog.vInspectionNumberSelected;
				if (TempInspectionNumberToDelete > 0)
				{
					vLocalSystemData->vInspectionNameChanged = true;
					//if (ISelectInspectionDialog.vInspectionNumberSelected < vProductEditing->vNumberOfInspections)
					vLocalSystemData->vInspectionNameMoved = true; //inspections changed so reset counters

					TempInspectionNumberToDelete--;
					if (vProductEditing->vInspection[TempInspectionNumberToDelete])
					{
						int TempResult = IDOK;
						CYesNoDialog TempYesNoDialog;
						CString TempText;
						TempText.LoadString(IDS_DeleteInspection);
						CString TempString("\n" + TempText + ": " + vProductEditing->vInspection[TempInspectionNumberToDelete]->vName + "?");

						if (vLocalSystemData->vSystemRunMode == cRunningSystemMode)
							TempString = TempString + "\nThis will generate a Production Report and clear the counters.";

						TempYesNoDialog.vNoticeText = TempString;
						CString TempText1;
						TempText1.LoadString(IDS_DeleteInspection);
						TempYesNoDialog.vYesButtonText = TempText1;
						TempYesNoDialog.vQuestionType = cConfirmQuestion;
						TempResult = TempYesNoDialog.DoModal();
						if (TempResult == IDOK)
						{
							if (vLocalSystemData->vITIPCDig->vContainerCollectionLock.Lock())
							{
								if (vLocalSystemData->vSystemRunMode == cRunningSystemMode)
								{
									vMainWindowPointer->MakeProductionReportIfNeeded();
									vMainWindowPointer->ResetCounters();
								}

								vInspectionToEdit = NULL;
								DWORD TempBitMask = 1;
								BYTE TempLastInspection = vProductEditing->vNumberOfInspections;
								for (BYTE TempLoop = TempInspectionNumberToDelete + 1; TempLoop < TempLastInspection; TempLoop++)
								{
									DWORD TempBitVector = 1;
									DWORD TempOriginalBitVector = 1;
									for (BYTE TempBitLoop = 0; TempBitLoop < TempLoop; TempBitLoop++)
										TempBitVector = TempBitVector * 2;
									TempOriginalBitVector = TempBitVector;
									TempBitVector = TempBitVector & vProductEditing->vInspectionAlarmSettings[0];  //make bit vector the enable/disable state
									TempBitVector = TempBitVector / 2; //move it down one inspection
									if (TempBitVector > 0)
										vProductEditing->vInspectionAlarmSettings[0] = vProductEditing->vInspectionAlarmSettings[0] | TempBitVector; //if it is enabled, set this bit
									else
									{
										TempBitVector = ~TempOriginalBitVector; //if it is disabled, invert this bit so can mask it off
										TempBitVector = TempBitVector / 2; //move it down one inspection
										TempBitVector = TempBitVector | cDWORDLastSignificantDigitValue; //put in the bit that says use the bit mask
										vProductEditing->vInspectionAlarmSettings[0] = vProductEditing->vInspectionAlarmSettings[0] & TempBitVector; //AND it to clear the bit and disable the inspection
									}

									TempBitVector = TempOriginalBitVector & vProductEditing->vInspectionAlarmSettings[1];
									TempBitVector = TempBitVector / 2;
									if (TempBitVector > 0)
										vProductEditing->vInspectionAlarmSettings[1] = vProductEditing->vInspectionAlarmSettings[1] | TempBitVector;
									else
									{
										TempBitVector = ~TempOriginalBitVector;
										TempBitVector = TempBitVector / 2;
										TempBitVector = TempBitVector | cDWORDLastSignificantDigitValue;
										vProductEditing->vInspectionAlarmSettings[1] = vProductEditing->vInspectionAlarmSettings[1] & TempBitVector;
									}
								}
								if (vProductEditing->vNumberOfInspections)
									vProductEditing->vNumberOfInspections--;
								delete vProductEditing->vInspection[TempInspectionNumberToDelete];
								vProductEditing->vInspection[TempInspectionNumberToDelete] = NULL;
								vProductEditing->vTypesOfRejectsToView = 0xFFFF;

								if (vProductEditing->vNumberOfInspections >= TempInspectionNumberToDelete)
								{ //move all the inspection pointers down one if just deleted
									for (BYTE TempLoop = TempInspectionNumberToDelete; TempLoop < vProductEditing->vNumberOfInspections; TempLoop++)
									{
										vProductEditing->vInspection[TempLoop] = vProductEditing->vInspection[TempLoop + 1];
									}
								}
								vProductEditing->vInspection[vProductEditing->vNumberOfInspections] = NULL;
								vMainWindowPointer->SetupRejectReasonNames();
								UpdateInspectionsOnDisplay();
								vLocalSystemData->vITIPCDig->vContainerCollectionLock.Unlock();
							}
						}
					}
				}
			}
			else 
			if (nResponse == 10)
			{
				//Main Menu Button Pressed
				CDialog::EndDialog(10);
			}
		}
		else
		{
			CNoticeDialog TempNoticeDialog;
			CString TempText;
			TempText.LoadString(IDS_Youcannotdeletethelastinspection);//"\n\nYou cannot delete the last inspection.\nAdd a new inspection,\nthen delete this one if desired.";
			TempNoticeDialog.vNoticeText = TempText;
			TempNoticeDialog.vType = cErrorMessage;
			TempNoticeDialog.DoModal();
		}
	}
}

void CConfigureInspectionsDialog::OnSubFunction8Button() 
{
  //New Inspection button was pressed.
	if (vInitialized)
	if (vProductEditing->vNumberOfInspections < cMaximumNumberOfInspections)// - TempReductionFactor)
	{
		//Create dialog box
		CSelectItemDialog ISelectItemDialog;  
		
		//Set dialog box data titles and number value
		CString TempText;
		TempText.LoadString(IDS_SelectInspectionTypetoAdd);
		ISelectItemDialog.vTitleString = TempText;
		ISelectItemDialog.vBackgroundColor = cGreen;
		ISelectItemDialog.vNumberOfItems = 0;

		if (!((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode) ||
			(vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vLocalConfigurationData->vScanTracType == cAllegroScanTrac) ||
			(vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac) ||(vLocalConfigurationData->vScanTracType == cBriosoScanTrac))) //Not Continuous Feed, or a Pipeline or TD
		{
			TempText.LoadString(IDS_MissingLid);
			ISelectItemDialog.vItems[ISelectItemDialog.vNumberOfItems++] = TempText;
		}

		TempText.LoadString(IDS_Underfill);
		ISelectItemDialog.vItems[ISelectItemDialog.vNumberOfItems++] = TempText;

		TempText.LoadString(IDS_UnderfillByWeight);
		ISelectItemDialog.vItems[ISelectItemDialog.vNumberOfItems++] = TempText;

		TempText.LoadString(IDS_ForeignMaterial);//Contaminant 2
		ISelectItemDialog.vItems[ISelectItemDialog.vNumberOfItems++] = TempText;

		if (!((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode) ||
			(vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vLocalConfigurationData->vScanTracType == cAllegroScanTrac) ||
			(vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac) ||(vLocalConfigurationData->vScanTracType == cBriosoScanTrac))) //Not Continuous Feed, or a Pipeline or TD
		{
			TempText.LoadString(IDS_ContaminantTop);//Contaminant 2
			TempText = TempText + " (Top FM)";
			ISelectItemDialog.vItems[ISelectItemDialog.vNumberOfItems++] = TempText;
		}

		TempText.LoadString(IDS_Contaminant);
		ISelectItemDialog.vItems[ISelectItemDialog.vNumberOfItems++] = "Small " +TempText; //"Small Contaminant (5mm maximum)"; //Old Contaminant P

		ISelectItemDialog.vItems[ISelectItemDialog.vNumberOfItems++] = "Medium " + TempText; //"Medium Contaminant (10mm maximum)"; //Old Contaminant Q

		ISelectItemDialog.vItems[ISelectItemDialog.vNumberOfItems++] = "Large " + TempText; //"Large Contaminant (20mm maximum)"; //Old Contaminant R

		//ISelectItemDialog.vItems[ISelectItemDialog.vNumberOfItems++] = "Thin Bones"; //look for edge down, then edge up

		//ISelectItemDialog.vItems[ISelectItemDialog.vNumberOfItems++] = "Large Bones"; //look for edge down, then edge up

		//ISelectItemDialog.vItems[ISelectItemDialog.vNumberOfItems++] = "Small Bones"; //Original - Median

		//TempText.LoadString(IDS_SmallBones);
		//ISelectItemDialog.vItems[ISelectItemDialog.vNumberOfItems++] = TempText; //difference algorithm 4,2

		//TempText.LoadString(IDS_Bones);
		//ISelectItemDialog.vItems[ISelectItemDialog.vNumberOfItems++] = TempText; //Chicken bones in Adagio;

		TempText.LoadString(IDS_Contaminant);
		ISelectItemDialog.vItems[ISelectItemDialog.vNumberOfItems++] = "Metal " + TempText; //Old Contaminant T

		//TempText.LoadString(IDS_GlassinBody);
		ISelectItemDialog.vItems[ISelectItemDialog.vNumberOfItems++] = "Wire";

		TempText.LoadString(IDS_IPContaminant);
		ISelectItemDialog.vItems[ISelectItemDialog.vNumberOfItems++] = TempText;

		TempText.LoadString(IDS_GlassinBody);
		ISelectItemDialog.vItems[ISelectItemDialog.vNumberOfItems++] = TempText;

		TempText.LoadString(IDS_GlassonBottom);
		ISelectItemDialog.vItems[ISelectItemDialog.vNumberOfItems++] = TempText;

		TempText.LoadString(IDS_GlassonPunt);
		ISelectItemDialog.vItems[ISelectItemDialog.vNumberOfItems++] = TempText;

		TempText.LoadString(IDS_Overfill);
		ISelectItemDialog.vItems[ISelectItemDialog.vNumberOfItems++] = TempText;

		TempText.LoadString(IDS_OverWeight);
		ISelectItemDialog.vItems[ISelectItemDialog.vNumberOfItems++] = TempText;

		if (!vLocalConfigurationData->vAllowVoidInspection)
			ISelectItemDialog.vItemEnabled[ISelectItemDialog.vNumberOfItems] = false;
		TempText.LoadString(IDS_Void);
		ISelectItemDialog.vItems[ISelectItemDialog.vNumberOfItems++] = TempText;

		//TempText.LoadString(IDS_CheckWeighDoesNotEject);
		//ISelectItemDialog.vItems[ISelectItemDialog.vNumberOfItems++] = TempText;

		//TempText.LoadString(IDS_Size);
		TempText = "Insert";
		ISelectItemDialog.vItems[ISelectItemDialog.vNumberOfItems++] = TempText;

		TempText = "Item Count";
		ISelectItemDialog.vItems[ISelectItemDialog.vNumberOfItems++] = TempText;

		TempText = "Drip";
		ISelectItemDialog.vItems[ISelectItemDialog.vNumberOfItems++] = TempText;

		//TempText = "Drip By Spout";
		//ISelectItemDialog.vItems[ISelectItemDialog.vNumberOfItems++] = TempText;

		TempText = "Flooded";
		ISelectItemDialog.vItems[ISelectItemDialog.vNumberOfItems++] = TempText;

		TempText = "Too Much Water";
		ISelectItemDialog.vItems[ISelectItemDialog.vNumberOfItems++] = TempText;


		TempText = "Bad Heart";
		ISelectItemDialog.vItems[ISelectItemDialog.vNumberOfItems++] = TempText;

		if (PasswordOK(cTemporaryInspxPassword,false))
		{
			//TempText.LoadString(IDS_Contaminant);
			//ISelectItemDialog.vItems[ISelectItemDialog.vNumberOfItems++] = TempText + " 22";

			//TempText.LoadString(IDS_JarBottom);
			//ISelectItemDialog.vItems[ISelectItemDialog.vNumberOfItems++] = TempText + "*";

			////TempText.LoadString(IDS_Glass3Plusmm);
			//ISelectItemDialog.vItems[ISelectItemDialog.vNumberOfItems++] = "Glass Advanced*";

			//TempText.LoadString(IDS_Density);
			//ISelectItemDialog.vItems[ISelectItemDialog.vNumberOfItems++] = TempText + "*";

			//TempText.LoadString(IDS_ContaminantAdvanced);
			//ISelectItemDialog.vItems[ISelectItemDialog.vNumberOfItems++] = TempText + "*";

			TempText.LoadString(IDS_Texture);
			ISelectItemDialog.vItems[ISelectItemDialog.vNumberOfItems++] = TempText + "*";

			//TempText.LoadString(IDS_ContaminantKernel);
			//ISelectItemDialog.vItems[ISelectItemDialog.vNumberOfItems++] = TempText + "*";
		}
/*
		TempText.LoadString(IDS_ContaminantGlass);
		if (!vLocalConfigurationData->vAllowGlassInspection)
			ISelectItemDialog.vItemEnabled[ISelectItemDialog.vNumberOfItems] = false;
		ISelectItemDialog.vItems[ISelectItemDialog.vNumberOfItems++] = TempText;

		if (PasswordOK(cTemporaryInspxPassword,false))
		{
			TempText.LoadString(IDS_ContaminantGlass);
			if (!vLocalConfigurationData->vAllowGlassInspection)
				ISelectItemDialog.vItemEnabled[ISelectItemDialog.vNumberOfItems] = false;
			ISelectItemDialog.vItems[ISelectItemDialog.vNumberOfItems++] = TempText + " Advanced (Reed)*";
		}
*/
		//TempText.LoadString(IDS_Contaminant3);
		//ISelectItemDialog.vItems[ISelectItemDialog.vNumberOfItems++] = TempText;

		TempText.LoadString(IDS_Product);
		ISelectItemDialog.m_DialogTitleStaticText2 = TempText + ": " + 
			*(vProductEditing->GetProductName());
		ISelectItemDialog.m_DialogTitleStaticText3 = "";

		if (vLocalSystemData->vSystemRunMode == cRunningSystemMode)
			ISelectItemDialog.m_DialogTitleStaticText3 = "This will generate a Production Report and clear the counters.";

		ISelectItemDialog.vInAutoSetup = vLocalSystemData->vInAutoSetup;
		//Pass control to dialog box and display
		int nResponse = ISelectItemDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			if (vLocalSystemData->vSystemRunMode == cRunningSystemMode)
			{
				vMainWindowPointer->MakeProductionReportIfNeeded();
				vMainWindowPointer->ResetCounters();
			}

			//add on if not inspx above advanced contaminant so select correct item since don't have
			BYTE TempInspectionType = ISelectItemDialog.vItemNumberSelected;

			if ((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode) ||
				(vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vLocalConfigurationData->vScanTracType == cAllegroScanTrac) ||
			(vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac) ||(vLocalConfigurationData->vScanTracType == cBriosoScanTrac)) //Continuous Feed, or a Pipeline or TD
				TempInspectionType++;

			if ((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode) ||
				(vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vLocalConfigurationData->vScanTracType == cAllegroScanTrac) ||
			(vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac) ||(vLocalConfigurationData->vScanTracType == cBriosoScanTrac)) //Continuous Feed, or a Pipeline or TD
			if (TempInspectionType > 4)
				TempInspectionType++;

			/*
			if ((!PasswordOK(cTemporaryInspxPassword,false)) && (ISelectItemDialog.vItemNumberSelected > 11))
				TempInspectionType = TempInspectionType + 2;
			if ((!PasswordOK(cTemporaryInspxPassword,false)) && (ISelectItemDialog.vItemNumberSelected > 14))
				TempInspectionType++;
			if ((!PasswordOK(cTemporaryInspxPassword,false)) && (ISelectItemDialog.vItemNumberSelected > 15))
				TempInspectionType++;
				*/

			vLocalSystemData->vInspectionNameChanged = true;
			CInspection *TempInspection = NULL;
			switch (TempInspectionType)
			{
				case 1: 
				{  //missing lid density inspection new way using size inspection
					//CInspectionDensity *TempInspectionToEditPointer = NULL;
					//if (ThereIsEnoughMemory(sizeof(CInspectionDensity), "New Inspection"))
					//	TempInspectionToEditPointer = new CInspectionDensity;
					//TempInspection->vHasDensityUpperThreshold = true;
					//TempInspection->vDensityThresholdUpper = 250;

					CInspectionSize *TempInspectionToEditPointer = NULL;
					if (ThereIsEnoughMemory(sizeof(CInspectionSize), "New Inspection"))
						TempInspectionToEditPointer = new CInspectionSize;
					TempInspection = TempInspectionToEditPointer;

					TempInspection->vThreshold = 50;
					if (vGlobalCurrentProduct->vSetupHistoryAverageDensity)
						TempInspection->vThreshold = (BYTE)(vGlobalCurrentProduct->vSetupHistoryAverageDensity * .5);

					TempInspection->vSize = 20;
					
					TempInspection->vInspectionType = cMissingLidInspectionOnSize;

					TempInspection->vName.LoadString(IDS_MissingLid);//ISelectItemDialog.vItems[ISelectItemDialog.vItemNumberSelected - 1];
				}
				break;
				case 2: 
				{  //underfill density inspection
					CInspectionDensity *TempInspectionToEditPointer = NULL;
					if (ThereIsEnoughMemory(sizeof(CInspectionDensity), "New Inspection"))
						TempInspectionToEditPointer = new CInspectionDensity;
					TempInspection = TempInspectionToEditPointer;

					TempInspection->vInspectionType = cUnderfillInspection;
					TempInspection->vHasDensityUpperThreshold = true;
					TempInspection->vDensityThresholdUpper = 240;
					TempInspection->vName.LoadString(IDS_Underfill);// = ISelectItemDialog.vItems[ISelectItemDialog.vItemNumberSelected - 1];
				}
				break;
				case 3: 
				{  //underfill weight inspection.  It inverts each pixel, then sums them up
					CInspectionCheckWeigh *TempInspectionToEditPointer = NULL;
					if (ThereIsEnoughMemory(sizeof(CInspectionCheckWeigh), "New Inspection"))
						TempInspectionToEditPointer = new CInspectionCheckWeigh;
					TempInspection = TempInspectionToEditPointer;

					TempInspection->vInspectionType = cUnderfillByWeightInspection;
					TempInspection->vDensityThresholdUpper = 0;//don't want it to start ejecting until calibrated, so set to zero, don't know what is good value, user must enter
					TempInspection->vHasDensityUpperThreshold = true;
					TempInspection->vHasDensityLowerThreshold = false;
					TempInspection->vName.LoadString(IDS_UnderfillByWeight);// = ISelectItemDialog.vItems[ISelectItemDialog.vItemNumberSelected - 1];
				}
				break;
				case 4:
				{  //Learned contaminant 2 inspection
					CInspectionContaminant *TempInspectionToEditPointer = NULL;
					if (ThereIsEnoughMemory(sizeof(CInspectionContaminant), "New Inspection"))
						TempInspectionToEditPointer = new CInspectionContaminant;
					TempInspection = TempInspectionToEditPointer;
					TempInspection->vSubtractAverage = false;
					TempInspection->vAddStructure = true;
					TempInspection->vEnhanceMethod = 3;

					if ((vLocalSystemData->vSystemRunMode == cRunningSystemMode) && 
						(vLocalConfigurationData->vEnableEjectors))
						TempInspection->vThreshold = 100;
					else
						TempInspection->vThreshold = 210;

					TempInspection->vSize = 10;

					TempInspection->vInspectionType = cBottomContaminantInspection;
					TempInspection->vName.LoadString(IDS_ForeignMaterial);// = ISelectItemDialog.vItems[ISelectItemDialog.vItemNumberSelected - 1];
				}
				break;
				case 5:
				{  //Top Learned contaminant 2 inspection
					CInspectionContaminant *TempInspectionToEditPointer = NULL;
					if (ThereIsEnoughMemory(sizeof(CInspectionContaminant), "New Inspection"))
						TempInspectionToEditPointer = new CInspectionContaminant;
					TempInspection = TempInspectionToEditPointer;
					TempInspection->vSubtractAverage = false;
					TempInspection->vAddStructure = true;
					TempInspection->vEnhanceMethod = 3;

					if ((vLocalSystemData->vSystemRunMode == cRunningSystemMode) && 
						(vLocalConfigurationData->vEnableEjectors))
						TempInspection->vThreshold = 10;
					else
						TempInspection->vThreshold = 200;

					TempInspection->vSize = 10;
					//TempInspection->vReferenceToTop = true;

					TempInspection->vInspectionType = cTopContaminantInspection;
					TempInspection->vName = "Top FM";
				}
				break;
				case 6: // Small Contaminant P
				{ //small contaminant, check for spot in sub ROIs
					CInspectionContaminant *TempInspectionToEditPointer = NULL;
					if (ThereIsEnoughMemory(sizeof(CInspectionContaminant), "New Inspection"))
						TempInspectionToEditPointer = new CInspectionContaminant;
					TempInspection = TempInspectionToEditPointer;
					TempInspection->vSubtractAverage = false;
					TempInspection->vAddStructure = false;
					TempInspection->vEnhanceMethod = 0;
					TempInspection->vSize = 2;
					TempInspection->vFindSpotInSubROI = 10;
					TempInspection->vPixelsStepToNextROI = 1;
					TempInspection->vDesensitizeEdgePixels = 30;
					if ((vLocalSystemData->vSystemRunMode == cRunningSystemMode) && 
						(vLocalConfigurationData->vEnableEjectors))
						TempInspection->vThreshold = 130;
					else
						TempInspection->vThreshold = 230;

					TempInspection->vInspectionType = cSpotInROIContaminantInspection;
					TempInspection->vName.LoadString(IDS_Contaminant);// = ISelectItemDialog.vItems[ISelectItemDialog.vItemNumberSelected - 1];
					TempInspection->vName = "Small " + TempInspection->vName;
				}
				break;
				case 7: //Medium Contaminant Q
				{ //Medium contaminant, check for spot in sub ROIs
					CInspectionContaminant *TempInspectionToEditPointer = NULL;
					if (ThereIsEnoughMemory(sizeof(CInspectionContaminant), "New Inspection"))
						TempInspectionToEditPointer = new CInspectionContaminant;
					TempInspection = TempInspectionToEditPointer;
					TempInspection->vSubtractAverage = false;
					TempInspection->vAddStructure = false;
					TempInspection->vEnhanceMethod = 0;
					TempInspection->vSize = 8;
					TempInspection->vPreSmoothImage = 3;
					TempInspection->vFindSpotInSubROI = 15;
					TempInspection->vPixelsStepToNextROI = 3;
					TempInspection->vDesensitizeEdgePixels = 30;
					if ((vLocalSystemData->vSystemRunMode == cRunningSystemMode) && 
						(vLocalConfigurationData->vEnableEjectors))
						TempInspection->vThreshold = 130;
					else
						TempInspection->vThreshold = 230;

					TempInspection->vInspectionType = cSpotInROIContaminantInspection;
					TempInspection->vName.LoadString(IDS_Contaminant);// = ISelectItemDialog.vItems[ISelectItemDialog.vItemNumberSelected - 1];
					TempInspection->vName = "Medium " + TempInspection->vName;
				}
				break;
				case 8:
				{ //Large contaminant R, check for spot in sub ROIs
					CInspectionContaminant *TempInspectionToEditPointer = NULL;
					if (ThereIsEnoughMemory(sizeof(CInspectionContaminant), "New Inspection"))
						TempInspectionToEditPointer = new CInspectionContaminant;
					TempInspection = TempInspectionToEditPointer;
					TempInspection->vSubtractAverage = false;
					TempInspection->vAddStructure = false;
					TempInspection->vEnhanceMethod = 0;
					TempInspection->vSize = 15;
					TempInspection->vPreSmoothImage = 3;
					TempInspection->vFindSpotInSubROI = 25;
					TempInspection->vPixelsStepToNextROI = 3;
					TempInspection->vDesensitizeEdgePixels = 30;
					if ((vLocalSystemData->vSystemRunMode == cRunningSystemMode) && 
						(vLocalConfigurationData->vEnableEjectors))
						TempInspection->vThreshold = 130;
					else
						TempInspection->vThreshold = 230;

					TempInspection->vInspectionType = cSpotInROIContaminantInspection;
					TempInspection->vName.LoadString(IDS_Contaminant);// = ISelectItemDialog.vItems[ISelectItemDialog.vItemNumberSelected - 1];
					TempInspection->vName = "Large " + TempInspection->vName;
				}
				break;
				//case 9:
				//{  //Thin bones look for edge down, then edge up
				//	CInspectionContaminant *TempInspectionToEditPointer = NULL;
				//	if (ThereIsEnoughMemory(sizeof(CInspectionContaminant), "New Inspection"))
				//		TempInspectionToEditPointer = new CInspectionContaminant;
				//	TempInspection = TempInspectionToEditPointer;
				//	TempInspection->vBonesInspection = 6;  //check only 6 pixels ahead
				//	TempInspection->vPixelLookAhead = 2; //each transition can only be two pixels ahead
				//	TempInspection->vPreSmoothImage = 0;
				//	TempInspection->vErodeCount = 1;
				//	TempInspection->vDilationCount = 0;
				//	TempInspection->vErodeCountAtEnd = 1;
				//	TempInspection->vDilationCountAtEnd = 1;
				//	TempInspection->vIgnoreEdgePixels = 1;

				//	if ((vLocalSystemData->vSystemRunMode == cRunningSystemMode) && 
				//		(vLocalConfigurationData->vEnableEjectors))
				//		TempInspection->vThreshold = 200;
				//	else
				//		TempInspection->vThreshold = 240;

				//	TempInspection->vSize = 100;

				//	TempInspection->vInspectionType = cContaminantBonesInspection;
				//	TempInspection->vName = ISelectItemDialog.vItems[ISelectItemDialog.vItemNumberSelected - 1];
				//}
				//break;
				//case 10:
				//{  //large bones look for edge down, then edge up
				//	CInspectionContaminant *TempInspectionToEditPointer = NULL;
				//	if (ThereIsEnoughMemory(sizeof(CInspectionContaminant), "New Inspection"))
				//		TempInspectionToEditPointer = new CInspectionContaminant;
				//	TempInspection = TempInspectionToEditPointer;
				//	TempInspection->vBonesInspection = 7;  //check for chicken bones 11 pixels wide at max
				//	TempInspection->vPixelLookAhead = 2;
				//	TempInspection->vPreSmoothImage = 3;
				//	TempInspection->vIgnoreEdgePixels = 2;

				//	if ((vLocalSystemData->vSystemRunMode == cRunningSystemMode) && 
				//		(vLocalConfigurationData->vEnableEjectors))
				//		TempInspection->vThreshold = 200;
				//	else
				//		TempInspection->vThreshold = 240;

				//	TempInspection->vSize = 125;

				//	TempInspection->vInspectionType = cContaminantBonesInspection;
				//	TempInspection->vName = ISelectItemDialog.vItems[ISelectItemDialog.vItemNumberSelected - 1];
				//}
				//break;
				//case 11:
				//{ //Small Bones Original - Median
				//	CInspectionContaminant *TempInspectionToEditPointer = NULL;
				//	if (ThereIsEnoughMemory(sizeof(CInspectionContaminant), "New Inspection"))
				//		TempInspectionToEditPointer = new CInspectionContaminant;
				//	TempInspection = TempInspectionToEditPointer;
				//	TempInspection->vSubtractAverage = false;
				//	TempInspection->vAddStructure = false;
				//	TempInspection->vEnhanceMethod = 3;
				//	TempInspection->vSize = 50;
				//	//TempInspection->vPreSmoothImage = 3;  //done by wire inspection value
				//	TempInspection->vTakeDifferenceFromMedianFilter = 5;
				//	if ((vLocalSystemData->vSystemRunMode == cRunningSystemMode) && 
				//		(vLocalConfigurationData->vEnableEjectors))
				//		TempInspection->vThreshold = 130;
				//	else
				//		TempInspection->vThreshold = 230;

				//	TempInspection->vInspectionType = cContaminantBonesInspection;
				//	TempInspection->vName = ISelectItemDialog.vItems[ISelectItemDialog.vItemNumberSelected - 1];
				//}
				//break;
				//case 10:
				//{  //Small Bones using difference algorithm 4,2
				//	CInspectionContaminant *TempInspectionToEditPointer = NULL;
				//	if (ThereIsEnoughMemory(sizeof(CInspectionContaminant), "New Inspection"))
				//		TempInspectionToEditPointer = new CInspectionContaminant;
				//	TempInspection = TempInspectionToEditPointer;
				//	TempInspection->vSubtractAverage = false;
				//	TempInspection->vAddStructure = false;
				//	TempInspection->vEnhanceMethod = 0;
				//	TempInspection->vAllowChangingAdvancedOptions = false;
				//	TempInspection->vPerformGlassAlgorithm = 0;  //1 is curve fittin, 2 = line derivative
				//	TempInspection->vPreSmoothImage = 0;
				//	TempInspection->vTrimEdgesOfJar = false;
				//	TempInspection->vFillBackground = 128;  //fill background with middle value to reduce edge effects.
				//	TempInspection->vTakeDifferenceFromMedian = 4 | 0x80;  //check for tuna fish bones 4 pixels wide at max
				//	TempInspection->vPixelsStepToNextROI = 2;

				//	if ((vLocalSystemData->vSystemRunMode == cRunningSystemMode) && 
				//		(vLocalConfigurationData->vEnableEjectors))
				//		TempInspection->vThreshold = 150;
				//	else
				//		TempInspection->vThreshold = 240;

				//	TempInspection->vSize = 20;

				//	TempInspection->vInspectionType = cContaminantBonesInspection;
				//	TempInspection->vName = ISelectItemDialog.vItems[ISelectItemDialog.vItemNumberSelected - 1];
				//}
				//break;
				//case 12:
				//{  //Chicken Bones
				//	CInspectionContaminant *TempInspectionToEditPointer = NULL;
				//	if (ThereIsEnoughMemory(sizeof(CInspectionContaminant), "New Inspection"))
				//		TempInspectionToEditPointer = new CInspectionContaminant;
				//	TempInspection = TempInspectionToEditPointer;
				//	TempInspection->vSubtractAverage = false;
				//	TempInspection->vAddStructure = false;
				//	TempInspection->vEnhanceMethod = 0;
				//	TempInspection->vAllowChangingAdvancedOptions = false;
				//	TempInspection->vTakeDifferenceFromMedian = 8 | 0x80;  //check for chicken bones 8 pixels wide at max
				//	TempInspection->vPixelsStepToNextROI = 2;
				//	TempInspection->vIgnoreEdgePixels = 3;

				//	if ((vLocalSystemData->vSystemRunMode == cRunningSystemMode) && 
				//		(vLocalConfigurationData->vEnableEjectors))
				//		TempInspection->vThreshold = 200;
				//	else
				//		TempInspection->vThreshold = 240;

				//	TempInspection->vSize = 75;

				//	TempInspection->vInspectionType = cContaminantBonesInspection;
				//	TempInspection->vName = ISelectItemDialog.vItems[ISelectItemDialog.vItemNumberSelected - 1];
				//}
				//break;
				case 9:
				{ //Metal contaminant T, straight threshold
					CInspectionContaminant *TempInspectionToEditPointer = NULL;
					if (ThereIsEnoughMemory(sizeof(CInspectionContaminant), "New Inspection"))
						TempInspectionToEditPointer = new CInspectionContaminant;
					TempInspection = TempInspectionToEditPointer;
					TempInspection->vSubtractAverage = false;
					TempInspection->vAddStructure = false;
					TempInspection->vEnhanceMethod = 0;
					//TempInspection->vPreSmoothImage = 3; //median filter
					TempInspection->vSize = 9;
					if ((vLocalSystemData->vSystemRunMode == cRunningSystemMode) && 
						(vLocalConfigurationData->vEnableEjectors))
						TempInspection->vThreshold = 0;
					else
						TempInspection->vThreshold = 60;

					TempInspection->vInspectionType = cThresholdContaminantInspection;
					TempInspection->vName.LoadString(IDS_Contaminant);// = ISelectItemDialog.vItems[ISelectItemDialog.vItemNumberSelected - 1];
					TempInspection->vName = "Metal " + TempInspection->vName;
				}
				break;
				case 10:
				{  //Wire with Minimum Length and Aspect Ratio 
					CInspectionContaminant *TempInspectionToEditPointer = NULL;
					if (ThereIsEnoughMemory(sizeof(CInspectionContaminant), "New Inspection"))
						TempInspectionToEditPointer = new CInspectionContaminant;
					TempInspection = TempInspectionToEditPointer;

					TempInspection->vLookForDarkSpots = 1;

					TempInspection->vErodeCount = 2;
					TempInspection->vDilationCount = 1;
					TempInspection->vErodeCountAtEnd = 2;
					TempInspection->vDilationCountAtEnd = 1;

					TempInspection->vPixelLookAhead = 8;
					TempInspection->vOverlap = 16;

					TempInspection->vMinimumLength = 10;  
					//TempInspection->vMinimumAspectRatio = 4.0;//4.0;  //Larger numbers indicate Fatter Bone, default Max 4.0

					if ((vLocalSystemData->vSystemRunMode == cRunningSystemMode) && (vLocalConfigurationData->vEnableEjectors))
						TempInspection->vThreshold = 150;
					else
						TempInspection->vThreshold = 240;

					TempInspection->vSize = 50;

					TempInspection->vInspectionType = cWireContaminantInspection;
					TempInspection->vName = "Wire"; //ISelectItemDialog.vItems[ISelectItemDialog.vItemNumberSelected - 1];
				}
				break;
				case 11:
				{  //IP Contaminant Or Irregular Product Contaminant
					CInspectionContaminant *TempInspectionToEditPointer = NULL;
					if (ThereIsEnoughMemory(sizeof(CInspectionContaminant), "New Inspection"))
						TempInspectionToEditPointer = new CInspectionContaminant;
					TempInspection = TempInspectionToEditPointer;

					TempInspection->vIPContaminant = 100;
					TempInspection->vEnhanceMethod = 3;
					TempInspection->vErodeCountAtEnd = 1;

					if ((vLocalSystemData->vSystemRunMode == cRunningSystemMode) && (vLocalConfigurationData->vEnableEjectors))
						TempInspection->vThreshold = 150;
					else
						TempInspection->vThreshold = 240;

					TempInspection->vSize = 6;

					TempInspection->vInspectionType = cIPContaminantInspection;
					TempInspection->vName.LoadString(IDS_IPContaminant); // = ISelectItemDialog.vItems[ISelectItemDialog.vItemNumberSelected - 1];
				}
				break;
				case 12:
				{  //Glass in Body
					CInspectionContaminant *TempInspectionToEditPointer = NULL;
					if (ThereIsEnoughMemory(sizeof(CInspectionContaminant), "New Inspection"))
						TempInspectionToEditPointer = new CInspectionContaminant;
					TempInspection = TempInspectionToEditPointer;
					TempInspection->vSubtractAverage = false;
					TempInspection->vAddStructure = false;
					TempInspection->vEnhanceMethod = 0;
					TempInspection->vAllowChangingAdvancedOptions = false;
					TempInspection->vRemoveJarPunt = true;
					TempInspection->vPerformGlassAlgorithm = 1;  //1 is curve fittin, 2 = line derivative
					TempInspection->vPreSmoothImage = 3;
					TempInspection->vTrimEdgesOfJar = true;
					//TempInspection->vRequire2InARow = false;
					//TempInspection->vOverlap = 0;
					//TempInspection->vFillInLightSpots = 10;

					if ((vLocalSystemData->vSystemRunMode == cRunningSystemMode) && (vLocalConfigurationData->vEnableEjectors))
						TempInspection->vThreshold = 150;
					else
						TempInspection->vThreshold = 240;

					TempInspection->vSize = 8;

					TempInspection->vInspectionType = cGlass13mmContaminantInspection;
					TempInspection->vName = ISelectItemDialog.vItems[ISelectItemDialog.vItemNumberSelected - 1];
				}
				break;
				case 13:
				{  //Glass on Bottom
					CInspectionContaminant *TempInspectionToEditPointer = NULL;
					if (ThereIsEnoughMemory(sizeof(CInspectionContaminant), "New Inspection"))
						TempInspectionToEditPointer = new CInspectionContaminant;
					TempInspection = TempInspectionToEditPointer;
					TempInspection->vSubtractAverage = false;
					TempInspection->vAddStructure = false;
					TempInspection->vEnhanceMethod = 0;
					TempInspection->vAllowChangingAdvancedOptions = false;
					TempInspection->vRemoveJarPunt = false;
					TempInspection->vPerformGlassAlgorithm = 2;  //1 is curve fittin, 2 = line derivative
					TempInspection->vPreSmoothImage = 3;
					TempInspection->vTrimEdgesOfJar = true;
					//TempInspection->vRequire2InARow = false;
					//TempInspection->vOverlap = 0;
					//TempInspection->vFillInLightSpots = 10;

					if ((vLocalSystemData->vSystemRunMode == cRunningSystemMode) && (vLocalConfigurationData->vEnableEjectors))
						TempInspection->vThreshold = 150;
					else
						TempInspection->vThreshold = 240;

					TempInspection->vSize = 4;

					TempInspection->vInspectionType = cGlass13mmContaminantInspection;
					TempInspection->vName = ISelectItemDialog.vItems[ISelectItemDialog.vItemNumberSelected - 1];
				}
				break;
				case 14:
				{  //Glass on Punt
					CInspectionContaminant *TempInspectionToEditPointer = NULL;
					if (ThereIsEnoughMemory(sizeof(CInspectionContaminant), "New Inspection"))
						TempInspectionToEditPointer = new CInspectionContaminant;
					TempInspection = TempInspectionToEditPointer;
					TempInspection->vSubtractAverage = false;
					TempInspection->vAddStructure = false;
					TempInspection->vEnhanceMethod = 0;
					TempInspection->vAllowChangingAdvancedOptions = false;
					TempInspection->vRemoveJarPunt = false;
					TempInspection->vPerformGlassAlgorithm = 2;  //1 is curve fittin, 2 = line derivative
					TempInspection->vPreSmoothImage = 0;
					TempInspection->vTrimEdgesOfJar = false;
					//TempInspection->vRequire2InARow = false;
					//TempInspection->vOverlap = 0;
					//TempInspection->vFillInLightSpots = 10;

					if ((vLocalSystemData->vSystemRunMode == cRunningSystemMode) && (vLocalConfigurationData->vEnableEjectors))
						TempInspection->vThreshold = 150;
					else
						TempInspection->vThreshold = 240;

					TempInspection->vSize = 2;

					TempInspection->vInspectionType = cGlass13mmContaminantInspection;
					TempInspection->vName.LoadString(IDS_GlassonPunt);// = ISelectItemDialog.vItems[ISelectItemDialog.vItemNumberSelected - 1];
				}
				break;
				case 15: 
				{  //Overfill density inspection
					CInspectionDensity *TempInspectionToEditPointer = NULL;
					if (ThereIsEnoughMemory(sizeof(CInspectionDensity), "New Inspection"))
						TempInspectionToEditPointer = new CInspectionDensity;
					TempInspection = TempInspectionToEditPointer;

					TempInspection->vInspectionType = cOverfillInspection;
					TempInspection->vHasDensityLowerThreshold = true;
					TempInspection->vHasDensityUpperThreshold = false;
					if ((vLocalSystemData->vSystemRunMode == cRunningSystemMode) && 
						(vLocalConfigurationData->vEnableEjectors))
						TempInspection->SetDensityLowerThreshold(20);
					else
						TempInspection->SetDensityLowerThreshold(80);
					TempInspection->vName.LoadString(IDS_Overfill);// = ISelectItemDialog.vItems[ISelectItemDialog.vItemNumberSelected - 1];

					TempInspection->SetDensityLowerThreshold(TempInspection->vDensityThresholdLower);
				}
				break;
				case 16: 
				{  //Overfill weight inspection.  It inverts each pixel, then sums them up
					CInspectionCheckWeigh *TempInspectionToEditPointer = NULL;
					if (ThereIsEnoughMemory(sizeof(CInspectionCheckWeigh), "New Inspection"))
						TempInspectionToEditPointer = new CInspectionCheckWeigh;
					TempInspection = TempInspectionToEditPointer;

					TempInspection->vInspectionType = cOverfillByWeightInspection;
					TempInspection->SetDensityLowerThreshold(0); //don't want it to start ejecting until calibrated, so set to zero, don't know what is good value, user must enter
					TempInspection->vHasDensityLowerThreshold = true;
					TempInspection->vHasDensityUpperThreshold = false;
					TempInspection->vName.LoadString(IDS_OverWeight);// = ISelectItemDialog.vItems[ISelectItemDialog.vItemNumberSelected - 1];
				}
				break;
				case 17:  //void inspection
				{
					if (vLocalConfigurationData->vAllowVoidInspection)
					{
						CInspectionVoid *TempInspectionToEditPointer = NULL;
						if (ThereIsEnoughMemory(sizeof(CInspectionVoid), "New Inspection"))
							TempInspectionToEditPointer = new CInspectionVoid;
						TempInspection = TempInspectionToEditPointer;
						TempInspection->vSubtractAverage = true;
						TempInspection->vAddStructure = true;
						TempInspection->vEnhanceMethod = 0;
						if ((vLocalSystemData->vSystemRunMode == cRunningSystemMode) && 
							(vLocalConfigurationData->vEnableEjectors))
							TempInspection->vThreshold = 0;
						else
							TempInspection->vThreshold = 50;
						TempInspection->vSize = 40;

						//TempInspection->vInspectionType = cVoidInspection;
						TempInspection->vAllowChangingAdvancedOptions = true;
						TempInspection->vName.LoadString(IDS_Void);// = ISelectItemDialog.vItems[ISelectItemDialog.vItemNumberSelected - 1];
					}
				}
				break;
				case 18:  //size inspection Now called "Insert" inspection
				{
					CInspectionSize *TempInspectionToEditPointer = NULL;
					if (ThereIsEnoughMemory(sizeof(CInspectionSize), "New Inspection"))
						TempInspectionToEditPointer = new CInspectionSize;
					TempInspection = TempInspectionToEditPointer;
					//TempInspection->vName.LoadString(IDS_Size);
					TempInspection->vName = "Insert";

					if ((vGlobalCurrentProduct->vSetupHistoryAverageDensity > 30) && (vGlobalCurrentProduct->vSetupHistoryAverageDensity < 220))
						TempInspection->vThreshold = (BYTE)(vGlobalCurrentProduct->vSetupHistoryAverageDensity * .6);
					else
						TempInspection->vThreshold = 70;
					TempInspection->vSize = 30;

					TempInspection->vInspectionType = cSizeInspection;
				}
				break;
				case 19:  //Item Count inspection
				{
					CInspectionSize *TempInspectionToEditPointer = NULL;
					if (ThereIsEnoughMemory(sizeof(CInspectionSize), "New Inspection"))
						TempInspectionToEditPointer = new CInspectionSize;
					TempInspection = TempInspectionToEditPointer;
					//TempInspection->vName.LoadString(IDS_Size);
					TempInspection->vName = "Item Count";

					if ((vGlobalCurrentProduct->vSetupHistoryAverageDensity > 30) && (vGlobalCurrentProduct->vSetupHistoryAverageDensity < 220))
						TempInspection->vThreshold = (BYTE)(vGlobalCurrentProduct->vSetupHistoryAverageDensity * .6);
					else
						TempInspection->vThreshold = 70;
					TempInspection->vSize = 30;
					TempInspection->vMaximumSize = 100;
					TempInspection->vMinimumNumberOfItemsRequired = 5;
					TempInspection->vMaximumNumberOfItemsRequired = 10;
					TempInspection->vMinimumBrightnessAceptable = 0;
					
					TempInspection->vInspectionType = cItemCountInspection;
				}
				break;
				case 20: //Drip Inspection
				{ //simple contaminant, straight threshold with a minimum and maximum size
					CInspectionContaminant *TempInspectionToEditPointer = NULL;
					if (ThereIsEnoughMemory(sizeof(CInspectionContaminant), "New Inspection"))
						TempInspectionToEditPointer = new CInspectionContaminant;
					TempInspection = TempInspectionToEditPointer;
					TempInspection->vSize = 2;
					TempInspection->vMaximumSize = 2000;
					TempInspection->vMaximumHeightAboveBottom = 25;
					TempInspection->vThreshold = 254;

					TempInspection->vInspectionType = cDripInspection;
					TempInspection->vName = ISelectItemDialog.vItems[ISelectItemDialog.vItemNumberSelected - 1];
				}
				break;
				//case 19: //Drip By Spout Inspection
				//{ //simple contaminant, straight threshold with a minimum and maximum size
				//	CInspectionContaminant *TempInspectionToEditPointer = NULL;
				//	if (ThereIsEnoughMemory(sizeof(CInspectionContaminant), "New Inspection"))
				//		TempInspectionToEditPointer = new CInspectionContaminant;
				//	TempInspection = TempInspectionToEditPointer;
				//	TempInspection->vSize = 2;
				//	TempInspection->vMaximumSize = 0;
				//	TempInspection->vMinimumWidth = 20;
				//	TempInspection->vThreshold = 240;

				//	TempInspection->vInspectionType = cDripBySpoutInspection;
				//	TempInspection->vName = ISelectItemDialog.vItems[ISelectItemDialog.vItemNumberSelected - 1];
				//}
				//break;
				case 21: 
				{ //Flooded inspection
					CInspectionContaminant *TempInspectionToEditPointer = NULL;
					if (ThereIsEnoughMemory(sizeof(CInspectionContaminant), "New Inspection"))
						TempInspectionToEditPointer = new CInspectionContaminant;
					TempInspection = TempInspectionToEditPointer;

					TempInspection->vInspectionType = cFloodedInspection;
					TempInspection->vSize = 22; //almost an 3/4 inch.
					TempInspection->vThreshold = 100; //only want dark area of product
					TempInspection->vName = ISelectItemDialog.vItems[ISelectItemDialog.vItemNumberSelected - 1];
				}
				break;
				case 22: 
				{ //Too Much Water inspection
					CInspectionCheckWeigh *TempInspectionToEditPointer = NULL;
					if (ThereIsEnoughMemory(sizeof(CInspectionCheckWeigh), "New Inspection"))
						TempInspectionToEditPointer = new CInspectionCheckWeigh;
					TempInspection = TempInspectionToEditPointer;

					TempInspection->vInspectionType = cOverfillByWeightInspection;
					TempInspection->SetDensityLowerThreshold(0); //don't want it to start ejecting until calibrated, so set to zero, don't know what is good value, user must enter
					TempInspection->vHasDensityLowerThreshold = true;
					TempInspection->vHasDensityUpperThreshold = false;
					TempInspection->vWhiteOutAnythingTouchingTheTop = true;
					TempInspection->vName = ISelectItemDialog.vItems[ISelectItemDialog.vItemNumberSelected - 1];
				}
				break;
				
				case 23:
				{ //Bad Heart for Pomagranets
					CInspectionContaminant *TempInspectionToEditPointer = NULL;
					if (ThereIsEnoughMemory(sizeof(CInspectionContaminant), "New Inspection"))
						TempInspectionToEditPointer = new CInspectionContaminant;
					TempInspection = TempInspectionToEditPointer;
					TempInspection->vSize = 200;
					TempInspection->vThreshold = 240;

					TempInspection->vInspectionType = cBadHeartInspection;
					TempInspection->vName = ISelectItemDialog.vItems[ISelectItemDialog.vItemNumberSelected - 1];
				}
				break;
				//case 17:
				//{  //Weight Trending inspection
				//	CInspectionCheckWeigh *TempInspectionToEditPointer = NULL;
				//	if (ThereIsEnoughMemory(sizeof(CInspectionCheckWeigh), "New Inspection"))
				//		TempInspectionToEditPointer = new CInspectionCheckWeigh;
				//	TempInspection = TempInspectionToEditPointer;

				//	TempInspection->vInspectionType = cCheckWeighInspection;
				//	TempInspection->vThreshold = 210;
				//	TempInspection->vHasDensityLowerThreshold = true;
				//	TempInspection->vHasDensityUpperThreshold = true;
				//	TempInspection->vDensityThresholdUpper = 250;
				//	TempInspection->vEjector = 0;

				//	CYesNoDialog TempYesNoDialog;
				//	TempYesNoDialog.vNoticeText = "\nWeight Trending always alarms on average under weight.\n(Alarm #2 and Yellow Message)\nDo you want to alarm on average over weight too?";
				//	TempYesNoDialog.vYesButtonText = "Don't Alarm on Over";
				//	TempYesNoDialog.vNoButtonText = "Alarm on Over Too";
				//	TempYesNoDialog.vQuestionType = cConfirmQuestion;
				//	int TempResult = TempYesNoDialog.DoModal();
				//	if (TempResult == IDOK)
				//	{
				//		TempInspection->vHasDensityLowerThreshold = false;
				//		TempInspection->SetDensityLowerThreshold(0);
				//	}

				//	if ((vLocalSystemData->vSystemRunMode == cRunningSystemMode) && 
				//		(vLocalConfigurationData->vEnableEjectors))
				//		TempInspection->SetDensityLowerThreshold(20);
				//	else
				//		TempInspection->SetDensityLowerThreshold(80);

				//	TempInspection->vName = ISelectItemDialog.vItems[ISelectItemDialog.vItemNumberSelected - 1];
				//	TempInspection->SetDensityLowerThreshold(TempInspection->vDensityThresholdLower);
				//}
				//break;
				/*
				case 6:
					{  //Advanced Threshold contaminant inspection  AKA Contaminant 2
					CInspectionContaminant *TempInspectionToEditPointer = NULL;
					if (ThereIsEnoughMemory(sizeof(CInspectionContaminant), "New Inspection"))
						TempInspectionToEditPointer = new CInspectionContaminant;
					TempInspection = TempInspectionToEditPointer;
					TempInspection->vSubtractAverage = false;
					TempInspection->vAddStructure = true;
					TempInspection->vEnhanceMethod = 3;

					if ((vLocalSystemData->vSystemRunMode == cRunningSystemMode) && 
						(vLocalConfigurationData->vEnableEjectors))
						TempInspection->vThreshold = 0;
					else
						TempInspection->vThreshold = 210;

					TempInspection->vSize = 10;

					TempInspection->vInspectionType = cContaminant2Inspection;
					TempInspection->vName = "FM2";
						//ISelectItemDialog.vItems[ISelectItemDialog.vItemNumberSelected - 1];
				}
				break;
				*/
				//case 18:
				//{ //Contaminant 22, Latvian Algorithm
				//	CInspectionContaminant *TempInspectionToEditPointer = NULL;
				//	if (ThereIsEnoughMemory(sizeof(CInspectionContaminant), "New Inspection"))
				//		TempInspectionToEditPointer = new CInspectionContaminant;
				//	TempInspection = TempInspectionToEditPointer;
				//	TempInspection->vSubtractAverage = false;
				//	TempInspection->vAddStructure = false;
				//	TempInspection->vEnhanceMethod = 0;
				//	TempInspection->vSize = 1;
				//	TempInspection->vPreSmoothImage = 0;
				//	TempInspection->vFindSpotInSubROI = 0;
				//	TempInspection->vPixelsStepToNextROI = 0;
				//	//TempInspection->vDesensitizeIrregularArea = 0;  //this is a scale factor of 3.0
				//	//TempInspection->vDesensitizeEdgePixels = 0;  //this is adding 30 to threshold if on edge of product
				//	TempInspection->vDoContaminant22Algorithm = 1;  
				//	TempInspection->vTrimEdgesOfJar = false;
				//	if ((vLocalSystemData->vSystemRunMode == cRunningSystemMode) && 
				//		(vLocalConfigurationData->vEnableEjectors))
				//		TempInspection->vThreshold = 180;
				//	else
				//		TempInspection->vThreshold = 220;

				//	TempInspection->vInspectionType = cContaminant22Inspection;
				//	TempInspection->vName = ISelectItemDialog.vItems[ISelectItemDialog.vItemNumberSelected - 1];

				//	TempInspection->InspxContaminant22CreateBuffers(vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY);
				//}
				//break;
				//case 27:
				//	{   //jar bottom
				//	CInspectionBottomContaminant *TempInspectionToEditPointer = NULL;
				//	if (ThereIsEnoughMemory(sizeof(CInspectionBottomContaminant), "New Inspection"))
				//		TempInspectionToEditPointer = new CInspectionBottomContaminant;
				//	TempInspection = TempInspectionToEditPointer;

				//	TempText.LoadString(IDS_JarBottom);
				//	TempInspection->vName = TempText;
				//	if ((vLocalSystemData->vSystemRunMode == cRunningSystemMode) && 
				//		(vLocalConfigurationData->vEnableEjectors))
				//		TempInspection->vThreshold = 0;
				//	else
				//		TempInspection->vThreshold = 80;
				//}
				//break;
				//case 28:
				//{  //Advanced Glass
				//	CInspectionContaminant *TempInspectionToEditPointer = NULL;
				//	if (ThereIsEnoughMemory(sizeof(CInspectionContaminant), "New Inspection"))
				//		TempInspectionToEditPointer = new CInspectionContaminant;
				//	TempInspection = TempInspectionToEditPointer;
				//	TempInspection->vSubtractAverage = false;
				//	TempInspection->vAddStructure = false;
				//	TempInspection->vEnhanceMethod = 0;
				//	TempInspection->vAllowChangingAdvancedOptions = true;
				//	TempInspection->vRemoveJarPunt = true;
				//	TempInspection->vPerformGlassAlgorithm = 1;  //1 is curve fittin, 2 = line derivative
				//	TempInspection->vPreSmoothImage = 3;
				//	TempInspection->vTrimEdgesOfJar = false;

				//	//TempInspection->vRequire2InARow = false;
				//	//TempInspection->vOverlap = 0;
				//	//TempInspection->vFillInLightSpots = 10;

				//	if ((vLocalSystemData->vSystemRunMode == cRunningSystemMode) && 
				//		(vLocalConfigurationData->vEnableEjectors))
				//		TempInspection->vThreshold = 150;
				//	else
				//		TempInspection->vThreshold = 240;

				//	TempInspection->vSize = 8;

				//	TempInspection->vInspectionType = cGlass13mmContaminantInspection;
				//	TempInspection->vName = "GlassA"; //ISelectItemDialog.vItems[ISelectItemDialog.vItemNumberSelected - 1];
				//}
				//break;
				//case 29:
				//{  //advanced density inspection
				//	CInspectionDensity *TempInspectionToEditPointer = NULL;
				//	if (ThereIsEnoughMemory(sizeof(CInspectionDensity), "New Inspection"))
				//		TempInspectionToEditPointer = new CInspectionDensity;
				//	TempInspection = TempInspectionToEditPointer;

				//	TempInspection->vInspectionType = cDensityInspection;
				//	TempInspection->vThreshold = 210;
				//	TempInspection->vHasDensityLowerThreshold = true;
				//	TempInspection->vHasDensityUpperThreshold = true;
				//	TempInspection->vDensityThresholdUpper = 250;
				//	if ((vLocalSystemData->vSystemRunMode == cRunningSystemMode) && 
				//		(vLocalConfigurationData->vEnableEjectors))
				//		TempInspection->SetDensityLowerThreshold(20);
				//	else
				//		TempInspection->SetDensityLowerThreshold(80);
				//	TempInspection->vName.LoadString(IDS_Density);
				//	TempInspection->SetDensityLowerThreshold(TempInspection->vDensityThresholdLower);
				//}
				//break;
				//case 30:
				//{  //advanced contaminant inspection
				//	CInspectionContaminant *TempInspectionToEditPointer = NULL;
				//	if (ThereIsEnoughMemory(sizeof(CInspectionContaminant), "New Inspection"))
				//		TempInspectionToEditPointer = new CInspectionContaminant;
				//	TempInspection = TempInspectionToEditPointer;
				//	TempInspection->vSubtractAverage = true;
				//	TempInspection->vAddStructure = true;
				//	TempInspection->vEnhanceMethod = 3;
				//	if ((vLocalSystemData->vSystemRunMode == cRunningSystemMode) && 
				//		(vLocalConfigurationData->vEnableEjectors))
				//		TempInspection->vThreshold = 0;
				//	else
				//		TempInspection->vThreshold = 220;
				//	TempInspection->vSize = 10;

				//	//TempInspection->vInspectionType = cContaminantInspection;
				//	TempInspection->vAllowChangingAdvancedOptions = true;
				//	//TempText.LoadString(IDS_Contaminant);
				//	//TempText = TempText + "a";
				//	TempInspection->vName = "FMa";
				//}
				//break;
				/*
				case 13:
					{  //Advanced contaminant 2 inspection AKA Contaminant2 Advanced
					CInspectionContaminant *TempInspectionToEditPointer = NULL;
					if (ThereIsEnoughMemory(sizeof(CInspectionContaminant), "New Inspection"))
						TempInspectionToEditPointer = new CInspectionContaminant;
					TempInspection = TempInspectionToEditPointer;
					TempInspection->vSubtractAverage = false;
					TempInspection->vAddStructure = true;
					TempInspection->vEnhanceMethod = 3;

					if ((vLocalSystemData->vSystemRunMode == cRunningSystemMode) && 
						(vLocalConfigurationData->vEnableEjectors))
						TempInspection->vThreshold = 0;
					else
						TempInspection->vThreshold = 220;

					TempInspection->vSize = 10;

					TempInspection->vInspectionType = cAdvancedContaminant2Inspection;
					//TempText.LoadString(IDS_Contaminant);
					//TempText = TempText + "2a";
					TempInspection->vName = "FM2a";
					TempInspection->vAllowChangingAdvancedOptions = true;
				}
				break;
				*/
				case 24:  //texture inspection
				{
					CInspectionStandardDeviation *TempInspectionToEditPointer = NULL;
					if (ThereIsEnoughMemory(sizeof(CInspectionStandardDeviation), "New Inspection"))
						TempInspectionToEditPointer = new CInspectionStandardDeviation;
					TempInspection = TempInspectionToEditPointer;
					TempInspection->vPreSmoothImage = 3;

					TempInspection->vThreshold = 0;
					//TempInspection->vInspectionType = cStandardDeviationInspection;
					TempInspection->vName.LoadString(IDS_Texture);
				}
				break;
				//case 25: //kernel contaminant inspection
				//{
				//	CInspectionKernel *TempInspectionToEditPointer = NULL;
				//	if (ThereIsEnoughMemory(sizeof(CInspectionKernel), "New Inspection"))
				//		TempInspectionToEditPointer = new CInspectionKernel;
				//	TempInspection = TempInspectionToEditPointer;
				//	TempInspection->vThreshold = 64;
				//	TempInspection->vSize = 6;

				//	//TempInspection->vInspectionType = cKernelInspection;
				//	TempInspection->vAllowChangingAdvancedOptions = true;
				//	TempText.LoadString(IDS_ContaminantKernel);
				//	TempInspection->vName = TempText;
				//}
				//break;
				/*
				case 18:  //Reed algorith inspection
				case 19:  //Reed algorith inspection advanced
				{
					if (vLocalConfigurationData->vAllowGlassInspection)
					{
						CInspectionGlassContaminant *TempInspectionToEditPointer = NULL;
						if (ThereIsEnoughMemory(sizeof(CInspectionGlassContaminant), "New Inspection"))
							TempInspectionToEditPointer = new CInspectionGlassContaminant;
						TempInspection = TempInspectionToEditPointer;

						if (TempInspectionType == 15)
							TempInspection->vAllowChangingAdvancedOptions = true;
						else
							TempInspection->vAllowChangingAdvancedOptions = false;
						TempText.LoadString(IDS_ContaminantGlass);
						TempInspection->vName = TempText;
						if ((vLocalSystemData->vSystemRunMode == cRunningSystemMode) && 
							(vLocalConfigurationData->vEnableEjectors))
							TempInspection->vThreshold = 0;
						else
							TempInspection->vThreshold = 55;
						TempInspection->vEnhancedImageFactor = 1.5;
						TempInspection->vOriginalImageFactor = 25;
						TempInspection->vAddHistogram = true;
						TempInspection->vSize = 6;
						TempInspection->vPreSmoothImage = 5;
					}
				}
				break;
				*/
			}  //end switch inspection type
			if (TempInspection)
			{
				vProductEditing->CalculateLowerRimHeight(vLocalSystemData->vITIPCDig->vProductAverageImage, NULL);

				if ((vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || (vGlobalCurrentProduct->vBulkProductMode) ||
					(vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac) || (vLocalConfigurationData->vScanTracType == cAllegroScanTrac) ||
					(vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac) ||(vLocalConfigurationData->vScanTracType == cBriosoScanTrac)) //Continuous Feed, or a Pipeline or TD
				{
					TempInspection->SetupInspectionInPixels(vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, 0, 0, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY, vProductEditing->vOverScanMultiplier);
				}
				else
				if (vProductEditing->vReferenceRight)
				{
					WORD TempHeight = vProductEditing->vReferenceTop - vProductEditing->vReferenceBottom;
					if ((!TempHeight) || (TempHeight > vLocalSystemData->vITIPCDig->vOriginalBufferSizeX))
						TempHeight = (WORD)(vLocalSystemData->vITIPCDig->vOriginalBufferSizeX * .95);

					if (TempInspection->vInspectionType == cTopContaminantInspection)
						TempInspection->SetupInspectionROIFromReference(vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, 0, vProductEditing->vReferenceLeft, vProductEditing->vReferenceRight, vProductEditing->vLowerRimHeightInPixels, vProductEditing->vOverScanMultiplier);
					else
					if ((TempInspection->vInspectionType == cDripInspection) || (TempInspection->vInspectionType == cDripBySpoutInspection) ||(TempInspection->vInspectionType == cBadHeartInspection))
						TempInspection->SetupInspectionROIFromReference(vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, 0, 0, vLocalSystemData->vITIPCDig->vOriginalBufferSizeY, vProductEditing->vLowerRimHeightInPixels, vProductEditing->vOverScanMultiplier);
					else
						TempInspection->SetupInspectionROIFromReference(TempHeight, 0, vProductEditing->vReferenceLeft, vProductEditing->vReferenceRight, vProductEditing->vLowerRimHeightInPixels, vProductEditing->vOverScanMultiplier);
				}
				else
				if ((vLocalSystemData->vITIPCDig->vOriginalImage) && (vLocalSystemData->vITIPCDig->vOriginalImage->vRight) && (vLocalSystemData->vITIPCDig->vOriginalImage->vTop))
				{
					if (TempInspection->vInspectionType == cTopContaminantInspection)
						TempInspection->SetupInspectionROIFromReference(vLocalSystemData->vITIPCDig->vOriginalBufferSizeX, 0, vLocalSystemData->vITIPCDig->vOriginalImage->vLeft, 
							vLocalSystemData->vITIPCDig->vOriginalImage->vRight, vProductEditing->vLowerRimHeightInPixels, vGlobalCurrentProduct->vOverScanMultiplier);
					else
						TempInspection->SetupInspectionROIFromReference(vLocalSystemData->vITIPCDig->vOriginalImage->vTop, 0, vLocalSystemData->vITIPCDig->vOriginalImage->vLeft, 
							vLocalSystemData->vITIPCDig->vOriginalImage->vRight, vProductEditing->vLowerRimHeightInPixels, vGlobalCurrentProduct->vOverScanMultiplier);
				}
				else
				{
					if (TempInspection->vInspectionType == cTopContaminantInspection)
						TempInspection->SetupInspectionROIFromReference((WORD)(vProductEditing->vProductImageHeightTop * .95 * vGlobalPixelsPerUnitInHeight), 0, 
							vProductEditing->vImageWidthPosition / 4, (WORD)((double)vProductEditing->vImageWidthPosition * .75), vProductEditing->vLowerRimHeightInPixels, vProductEditing->vOverScanMultiplier);
					else
						TempInspection->SetupInspectionROIFromReference((WORD)(vProductEditing->vProductImageHeightTop * vGlobalPixelsPerUnitInHeight), 0, 
							vProductEditing->vImageWidthPosition / 4, (WORD)((double)vProductEditing->vImageWidthPosition * .75), vProductEditing->vLowerRimHeightInPixels, vProductEditing->vOverScanMultiplier);
				}
				if (TempInspection->vInspectionType != cCheckWeighInspection)
				if (vLocalConfigurationData->vEjector[0].vEnabled)
					TempInspection->vEjector = 1;

				bool TempNameADuplicate = true;
				BYTE TempTrialNumber = 0;
				CString TempTrialName = TempInspection->vName;
				while (TempNameADuplicate)
				{
					TempNameADuplicate = false;
					for (BYTE TempLoop = 0; TempLoop < vProductEditing->vNumberOfInspections; TempLoop++)
					if (vProductEditing->vInspection[TempLoop])
					{
						if (TempTrialName == vProductEditing->vInspection[TempLoop]->vName)
						{
							TempNameADuplicate = true;
							TempTrialNumber++;
							TempTrialName = TempInspection->vName + " " + dtoa(TempTrialNumber, 0);
						}
					}
				}

				CAlphaEntryDialog IAlphaEntryDialog;  
				IAlphaEntryDialog.vLocalSystemData = vLocalSystemData;
				IAlphaEntryDialog.vMainWindowPointer = vMainWindowPointer;
				IAlphaEntryDialog.vEditString = TempTrialName;
				
				TempNameADuplicate = true;

				while (TempNameADuplicate)
				{
					//Set dialog box data titles and number value
					TempText.LoadString(IDS_InspectionName);
					IAlphaEntryDialog.m_DialogTitleStaticText = TempText;
					TempText.LoadString(IDS_OriginalValue);
					IAlphaEntryDialog.m_DialogTitleStaticText2 = TempText + ": " + IAlphaEntryDialog.vEditString;
					IAlphaEntryDialog.vAllowDashes = true;
					IAlphaEntryDialog.vInAutoSetup = vLocalSystemData->vInAutoSetup;
					IAlphaEntryDialog.vMaximumNumberOfCharacters = 20;
					//Pass control to dialog box and display
					int nResponse = IAlphaEntryDialog.DoModal();
					//dialog box is now closed, if user pressed select do this
					//if user pressed cancel, do nothing
					if (nResponse == IDOK)
					{
						CString TempNewNameInUpperCase = IAlphaEntryDialog.vEditString;
						TempNewNameInUpperCase.MakeUpper();

						TempNameADuplicate = false;
						for (BYTE TempLoop = 0; TempLoop < vProductEditing->vNumberOfInspections; TempLoop++)
						if (vProductEditing->vInspection[TempLoop])
						{
							CString TempInspectionNameInUpperCase = vProductEditing->vInspection[TempLoop]->vName;
							TempInspectionNameInUpperCase.MakeUpper();
							if (TempInspectionNameInUpperCase == TempNewNameInUpperCase)
								TempNameADuplicate = true;
						}

						if (!TempNameADuplicate)
						{
							if (vLocalSystemData->vITIPCDig->vContainerCollectionLock.Lock())
							{
								BYTE TempPlaceToInsertNewInspection = vProductEditing->vNumberOfInspections;
								TempInspection->vName = IAlphaEntryDialog.vEditString;
								vProductEditing->vInspection[TempPlaceToInsertNewInspection] = TempInspection;
								vProductEditing->vNumberOfInspections++;
								//exit dialog so calling dialog can edit the inspection
								vInspectionToEdit = TempInspection;
								if (vProductEditing->AreThereAnyOtherWeightTrendingInspections(TempPlaceToInsertNewInspection))
								{
									AskForWeightTrendingInspectionToLinkThisOneTo(TempPlaceToInsertNewInspection);
								}

								vProductEditing->vTypesOfRejectsToView = 0xFFFF; //view all inspections

								vNewInspection = true;
								vLocalSystemData->vITIPCDig->vContainerCollectionLock.Unlock();
							}
							CDialog::EndDialog(true);
						}
						else
						{
							CNoticeDialog TempNoticeDialog;
							TempNoticeDialog.vNoticeText =  "\n\nThis product already has an inspection with that name.\nPlease enter a new inspection name.";
							TempNoticeDialog.vType = cErrorMessage;
							TempNoticeDialog.DoModal();
						}
					}
					else
					if (nResponse == 10)
					{
						//Main Menu button pressed
						TempNameADuplicate = false;
						CDialog::EndDialog(10);
					}
					else
					if (nResponse == IDCANCEL)
					{
						//Cancel button pressed
						TempNameADuplicate = false;
					}
				}
				vLocalSystemData->vInspectionNameMoved = true; //inspections changed so reset counters
				vMainWindowPointer->SetupRejectReasonNames();

				//make sure the bit is set for both alarms to use that inspection
				DWORD TempBitMask = 1;
				BYTE TempLastInspection = vProductEditing->vNumberOfInspections;
				DWORD TempBitVector = 1;
				for (BYTE TempBitLoop = 1; TempBitLoop < TempLastInspection; TempBitLoop++)
					TempBitVector = TempBitVector * 2;

				for (BYTE TempLoop = 0; TempLoop < 2; TempLoop++)
					vProductEditing->vInspectionAlarmSettings[TempLoop] = vProductEditing->vInspectionAlarmSettings[TempLoop] | TempBitVector; //enabled this new inspection by setting this bit
			}
		}
		else 
		if (nResponse == 10)
		{
			//Main Menu button pressed
			CDialog::EndDialog(10);
		}
	}
	else
	{
		CNoticeDialog TempNoticeDialog;
		CString TempText;
		CString TempText1;
		TempText.LoadString(IDS_YoucannotaddanymoreInspections);//"\n\nYou cannot add any more Inspections.\nThis Product already has the maximum of"
		TempText1.LoadString(IDS_Inspections);
		TempNoticeDialog.vNoticeText =  TempText + dtoa(cMaximumNumberOfInspections,0) + " " + TempText1 + ".";
		TempNoticeDialog.vType = cErrorMessage;
		TempNoticeDialog.DoModal();
	}
}

BOOL CConfigureInspectionsDialog::PreTranslateMessage(MSG* pMsg) 
{
	CWnd * TempWindow = NULL;
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == cUpdateTitleBarMessage)
	{
		ShowInspxButtons();
		CWnd *TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText1);
		if (TempWindow)
			TempWindow->Invalidate(false);
		return true;  //true indicates it has been handled, so do not process
	}
	else
	if (pMsg->message == WM_KEYDOWN)
	{
		if (vGlobalShiftKeyDown)
			PostThreadMessage(vGlobalCallingThreadID,cHiddenKeyPressed, 0, pMsg->wParam);
		else
		switch (pMsg->wParam)
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
			case cPageUp: // Clear, but enter System setup password
			{
				if (vGlobalPasswordLevel)
				{
					PasswordOK (cNoPassword, false);
					ShowInspxButtons();
				}
				else
				{
					PasswordOK(cAnyPasswordToEnter, true); //allow any password to be entered

					if (PasswordOK(cTemporaryInspxPassword, false))
						ShowInspxButtons();
				}
				TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText1);
				if (TempWindow)
					TempWindow->Invalidate(false);
			}
			break;
			case 16: // Shift Key
			case 0x60:  //0 key on KeyPad
				PostThreadMessage(vGlobalCallingThreadID,cHiddenKeyPressed, 1, 0);
			break;
			case 36: // Home Key, Screen Capture
				vGlobalShiftKeyDown = true;
				PostThreadMessage(vGlobalCallingThreadID,cHiddenKeyPressed, 0, 0x61);
			break;
			case cPageDown: // Help
				PostThreadMessage(vGlobalCallingThreadID,
					cShowHelp, 9, 0);
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

HBRUSH CConfigureInspectionsDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	HWND TempDialogHwnd = this->m_hWnd;
	int TextSize;
	pDC->SetAttribDC(pDC->m_hDC);
	
	// TODO: Change any attributes of the DC here
	if (pWnd->GetDlgCtrlID() == IDC_DialogTitleStaticText1)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DialogTitleStaticText, 8);  //5 is medium large
		pDC->SetTextColor(vGlobalDialogTitleColor);
	}

	if (pWnd->GetDlgCtrlID() == IDC_XRaysOn)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_XRaysOn, 7);  //5 is medium large
		pDC->SetTextColor(cYellow);
		pDC->SetBkColor(cRed);
		return vLocalSystemData->vRedBrush;
	}

	if (pWnd->GetDlgCtrlID() == IDC_Background)
	{
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}

	if (pWnd->GetDlgCtrlID() == IDC_InspectionList)
	{
		TextSize = SetTextSizeMultilineDisplay(TempDialogHwnd, pWnd, pDC, &m_InspectionList, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_CPProductLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_CPProductLabel, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction3Display)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction3Display, 5);
		pDC->SetBkMode(TRANSPARENT);
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction4Display)
	{
		TextSize = SetTextSizeMultilineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction4Display, 5);
		pDC->SetBkMode(TRANSPARENT);
	}
	if (pWnd->GetDlgCtrlID() == IDC_Function2Button)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function2Button, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vLocalSystemData->vGreenBrush;
  }
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction5Button)
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction5Button, 5);  //5 is medium large
		if (vProductEditing)
		if (vProductEditing->vNumberOfInspections)
		if (!vProductEditing->vInspection[0]->vSetupHistoryMinimumGoodAverage)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cButtonTextColor);
			return vLocalSystemData->vGreenBrush;
		}

		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	
	if (pWnd->GetDlgCtrlID() == IDC_Function1Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function1Button, 5);  //5 is medium large
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
		if (vGlobalCurrentProduct->vNumberOfInspectionsRequiredToEject > 1)
			return vLocalSystemData->vYellowBrush;
		else
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

void CConfigureInspectionsDialog::UpdateCurrentProductOnDisplay() 
{
  if (vProductEditing != NULL)
	{
		CString TempString;
		TempString.LoadString(IDS_ProductEditing);
		CString TempString3(*vProductEditing->GetProductName());
		if (TempString3.GetLength() > 0)
			TempString = TempString + ": " + TempString3;
		//copy product name text into Label control on screen 
		SetDlgItemText(IDC_CPProductLabel,TempString);
	}
}

void CConfigureInspectionsDialog::SelectAndEdit(int TempWindowTypeMode)
{
	if (vProductEditing)
	if (vProductEditing->vNumberOfInspections)
  {
    //Create dialog box
		int nResponse = IDOK;
		int TempInspectionNumberToEdit = 1;
		if (vProductEditing->vNumberOfInspections > 1)
		{
			CSelectInspectionDialog ISelectInspectionDialog;  
			//Set dialog box data like which square to start on
			ISelectInspectionDialog.vLocalSystemData = vLocalSystemData;
			ISelectInspectionDialog.vProductEditing = vProductEditing;
			ISelectInspectionDialog.SelectWindowForStyle = TempWindowTypeMode;
			//Pass control to dialog box and display
			nResponse = ISelectInspectionDialog.DoModal();
			//dialog box is now closed, if user pressed select do this
			//if user pressed cancel, do nothing
			if (nResponse == IDOK) //  closed with OK
				TempInspectionNumberToEdit = ISelectInspectionDialog.vInspectionNumberSelected;
		}
    if (nResponse == IDOK)
		{ //  closed with OK
			if(TempInspectionNumberToEdit > 0)
			{
				TempInspectionNumberToEdit--;
				if(vProductEditing->vInspection[TempInspectionNumberToEdit])
				{
					vInspectionToEdit = vProductEditing->vInspection[TempInspectionNumberToEdit];
					EndDialog(true);
				}
		  }
		}
		else 
		if (nResponse == 10)
		{
			//Main Menu Button Pressed
			CDialog::EndDialog(10);
		}
  }
	else
	{
		CNoticeDialog TempNoticeDialog;
		CString TempString;
		TempString.LoadString(IDS_NoInspectionstoEdit);//"\n\n\nNo Inspections to Edit"
		TempNoticeDialog.vNoticeText = TempString;
		TempNoticeDialog.vType = cErrorMessage;
		TempNoticeDialog.DoModal();
	}
}

void CConfigureInspectionsDialog::SetChangeMade() 
{
}

void CConfigureInspectionsDialog::UpdateInspectionsOnDisplay()
{
	if (vProductEditing)
	{
		CString TempString1 = "";
		if (vProductEditing->vNumberOfInspections == 0)
			TempString1 = "No Inspections Configured";
		else
		{
			for (BYTE TempLoop = 0; TempLoop < vProductEditing->vNumberOfInspections; TempLoop++)
				TempString1 = TempString1 + dtoa(TempLoop + 1, 0) + ". " + vProductEditing->vInspection[TempLoop]->vName + "\n";
		}
		SetDlgItemText(IDC_InspectionList,TempString1);
		if (vProductEditing->vNumberOfInspections > 1)
		{
			m_SubFunction6Button.ShowWindow(SW_SHOW);
		}
		else
		{
			m_SubFunction6Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_SubFunction6Button.GetRectToInvalidateArrow());
		}
		if ((vProductEditing->vNumberOfInspections > 1) || ((PasswordOK(cTemporaryInspxPassword,false)) && (vProductEditing->vNumberOfInspections)))
		{
			m_SubFunction7Button.ShowWindow(SW_SHOW);
			this->InvalidateRect(&m_SubFunction7Button.GetRectToInvalidateArrow());
		}
		else
		{
			m_SubFunction7Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_SubFunction7Button.GetRectToInvalidateArrow());
		}
		if (vProductEditing->vNumberOfInspections)
		{
			m_SubFunction5Button.ShowWindow(SW_SHOW);
		}
		else
		{
			m_SubFunction5Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_SubFunction5Button.GetRectToInvalidateArrow());
		}
		if (vProductEditing->vNumberOfInspections > 12)
		{
			m_SubFunction4Display.SetWindowPos(NULL, 122, 98 + 19 * vProductEditing->vNumberOfInspections, 313, 22 , SWP_NOZORDER);
			::SetupItem(&m_SubFunction4Display);
		}

		if (vProductEditing->vNumberOfInspections > 9)
		{
			m_SubFunction3Display.SetWindowPos(NULL, 122, 68 + (19 * vProductEditing->vNumberOfInspections), 313, 22, SWP_NOZORDER);
			::SetupItem(&m_SubFunction3Display);
		}

		if (vProductEditing->vNumberOfInspections > 8)
		{
			m_InspectionList.SetWindowPos(NULL, 125, 72, 400, 20 * vProductEditing->vNumberOfInspections, SWP_NOZORDER);
			::SetupItem(&m_InspectionList);
		}

		DisplayXofYAlarm();

		Invalidate(true);
	}
}

void CConfigureInspectionsDialog::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
  if (nIDEvent == vOneSecondTimerHandle)
  {
		OneSecondUpdate();
	}
	else
  if (nIDEvent == vGrabFocusTimerHandle)  
  {
		KillTimer(vGrabFocusTimerHandle);
		SetForegroundWindow();
		vInitialized = true;
	}
	CDialog::OnTimer(nIDEvent);
}

void CConfigureInspectionsDialog::OneSecondUpdate() 
{
	if (vOldXRayOnNotice != vLocalSystemData->vXRayOnNotice)
	{
		vOldXRayOnNotice = vLocalSystemData->vXRayOnNotice;
		if (vOldXRayOnNotice)
			m_XRaysOn.ShowWindow(SW_SHOW);
		else
		{
			m_XRaysOn.ShowWindow(SW_HIDE);
			//vStartingUpSource = false;
		}
	}
}

void CConfigureInspectionsDialog::ShowInspxButtons()
{
	if (PasswordOK(cSuperInspxPassword,false))
	{
		m_SubFunction2Button.ShowWindow(SW_SHOW);
		CString TempText = " ";
		TempText.LoadString(IDS_Options);
		SetDlgItemText(IDC_SubFunction2Button, TempText);
	}
	else
	if (vHaveExternalDetectorsConfigured)
	{
		m_SubFunction2Button.ShowWindow(SW_SHOW);
		CString TempText = " ";
		TempText.LoadString(IDS_SetupAuxiliaryDetectors);
		SetDlgItemText(IDC_SubFunction2Button, TempText);
	}
	else
		m_SubFunction2Button.ShowWindow(SW_HIDE);


	UpdateInspectionsOnDisplay();
	CWnd *TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText1);
	if (TempWindow)
		TempWindow->Invalidate(false);
}

void CConfigureInspectionsDialog::DisplayXofYAlarm()
{
	CString TempString1 = dtoa(vProductEditing->vTooManyRejectsAlarmX[0],0) + " of " + 
		dtoa(vProductEditing->vTooManyRejectsAlarmY[0],0);
	CString TempString2 = dtoa(vProductEditing->vTooManyRejectsAlarmX[1],0) + " of " + 
		dtoa(vProductEditing->vTooManyRejectsAlarmY[1],0);

	if (vProductEditing->vTooManyRejectsAlarmDisableEjectors[0])
	{
		if (vLocalConfigurationData->vEjectContainersNotInspected)
			TempString1 = TempString1 + " Stop On Alarm";
		else
			TempString1 = TempString1 + " Disable Ejectors On Alarm";

	}
	else
			TempString1 = TempString1 + " Yellow Message Only";

	if (vProductEditing->vTooManyRejectsAlarmDisableEjectors[1])
	{
		if (vLocalConfigurationData->vEjectContainersNotInspected)
			TempString2 = TempString2 + " Stop On Alarm";
		else
			TempString2 = TempString2 + " Disable Ejectors On Alarm";

	}
	else
			TempString2 = TempString2 + " Yellow Message Only";

	BYTE TempNumberSelectedOne = 0;
	BYTE TempNumberSelectedTwo = 0;
	for (BYTE TempLoop = 0; TempLoop < vProductEditing->vNumberOfInspections; TempLoop++)
	{
		DWORD TempBitMask = 1;
		for (BYTE TempBitLoop = 0; TempBitLoop < TempLoop ; TempBitLoop++)
			TempBitMask = TempBitMask * 2;
		if (vProductEditing->vInspectionAlarmSettings[0] & TempBitMask)
			TempNumberSelectedOne++;
		if (vProductEditing->vInspectionAlarmSettings[1] & TempBitMask)
			TempNumberSelectedTwo++;
	}	
	char TempBuffer[4];
	_itoa_s(TempNumberSelectedOne,TempBuffer,4,10);
	TempString1 = TempString1 + " (" + TempBuffer + " selected)";

	_itoa_s(TempNumberSelectedTwo,TempBuffer,4,10);
	TempString2 = TempString2 + " (" + TempBuffer + " selected)";

	if (!(vProductEditing->vTooManyRejectsAlarmX[0]) || TempNumberSelectedOne == 0)
		TempString1.LoadString(IDS_Disabled);

	if (!(vProductEditing->vTooManyRejectsAlarmX[1]) || TempNumberSelectedTwo == 0)
		TempString2.LoadString(IDS_Disabled);

	SetDlgItemText(IDC_SubFunction3Display,TempString1);
	
	if (vLocalConfigurationData->vUseXofYAlarmOutputForUnconfirmedEject != 0)
	{
		m_SubFunction4Display.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		SetDlgItemText(IDC_SubFunction4Display, TempString2 + "\nOutput used by Unconfirmed Eject");
	}
	else
	{
		m_SubFunction4Display.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
		SetDlgItemText(IDC_SubFunction4Display,TempString2);
	}
}

void CConfigureInspectionsDialog::EditAlarm(BYTE TempAlarm) 
{
	CConfigureProductAlarmsDialog IConfigureProductAlarmsDialog;
	//IConfigureProductAlarmsDialog.vNumberOfItems = vProductEditing->vNumberOfInspections;
	IConfigureProductAlarmsDialog.vXValue = vProductEditing->vTooManyRejectsAlarmX[TempAlarm];
	IConfigureProductAlarmsDialog.vYValue = vProductEditing->vTooManyRejectsAlarmY[TempAlarm];
	IConfigureProductAlarmsDialog.vEjectorsEnabled = vProductEditing->vTooManyRejectsAlarmDisableEjectors[TempAlarm];
	IConfigureProductAlarmsDialog.LoadSettings(vProductEditing->vInspectionAlarmSettings[TempAlarm]);
	IConfigureProductAlarmsDialog.vAlarmNumber = TempAlarm + 1;

	int nResponse = IConfigureProductAlarmsDialog.DoModal();
	if (nResponse == 10)
	{
		CDialog::EndDialog(10);
	}
	else if (nResponse == IDOK)
	{
		vProductEditing->vTooManyRejectsAlarmX[TempAlarm] = IConfigureProductAlarmsDialog.vXValue;
		vProductEditing->vTooManyRejectsAlarmY[TempAlarm] = IConfigureProductAlarmsDialog.vYValue;
		vProductEditing->vTooManyRejectsAlarmDisableEjectors[TempAlarm] = IConfigureProductAlarmsDialog.vEjectorsEnabled;
		vProductEditing->vInspectionAlarmSettings[TempAlarm] = IConfigureProductAlarmsDialog.vConfigurationSettings;
	}
}

BYTE CConfigureInspectionsDialog::SetupProductEjectors()
{
	//setup product ejectors button pressed
	bool TempHaveEjectorConfigured = false;
	for (BYTE TempLoop = 0; TempLoop < cNumberOfEjectors; TempLoop++)
	{
		if (vLocalConfigurationData->vEjector[TempLoop].vEnabled)
			TempHaveEjectorConfigured = true;
	}
		
	if (TempHaveEjectorConfigured)
	{
		//Create dialog box
		CNewConfigProductEjectorDialog IConfigureProductEjectorsDialog;  
		
		//Set dialog box data like how many products there are
		IConfigureProductEjectorsDialog.vLocalConfigurationData = vLocalConfigurationData;
		IConfigureProductEjectorsDialog.vLocalSystemData = vLocalSystemData;
		//IConfigureProductEjectorsDialog.vInSideTrip = true;
		IConfigureProductEjectorsDialog.vMainWindowPointer = vMainWindowPointer;
		IConfigureProductEjectorsDialog.vLocalProductCollection = vLocalProductCollection;
		IConfigureProductEjectorsDialog.vShowPreviousAndNextButtons = false;
		//Pass control to dialog box and display
		//IEditProductDialog.vEPLocalNumberOfProducts = vCPLocalNumberOfProducts;
		int nResponse = IConfigureProductEjectorsDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if ((nResponse == IDOK) || (nResponse == 20))
		{
			// TODO: Place code here to handle when the dialog is accepted
			if (IConfigureProductEjectorsDialog.vChangeMade)
				SetChangeMade();
			return nResponse;
		}
		else 
		if (nResponse == 10)
		{
			//  Main Menu button was pressed
			CDialog::EndDialog(10);
			return 10;
		}
		else
			return 0;
	}
	else
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\n\nNo Ejectors configured in System.";
		TempNoticeDialog.vType = cErrorMessage;
		TempNoticeDialog.DoModal();
		return 1;
	}
}

void CConfigureInspectionsDialog::AskForWeightTrendingInspectionToLinkThisOneTo(BYTE TempThisInspection)
{
	//Create dialog box
	CSelectItemDialog ISelectItemDialog;  
		
	//Set dialog box data titles and number value
	ISelectItemDialog.vTitleString = "Select Inspection for ROI and Weight Calibration";
	ISelectItemDialog.vBackgroundColor = cGreen;
	ISelectItemDialog.vNumberOfItems = 0;
	ISelectItemDialog.vItems[ISelectItemDialog.vNumberOfItems++] = "None - Uses Own ROI and Calibrations";
	ISelectItemDialog.vGreenItem = ISelectItemDialog.vNumberOfItems;

	for (BYTE TempLoop = 0; TempLoop < vGlobalCurrentProduct->vNumberOfInspections; TempLoop++)
	if (vGlobalCurrentProduct->vInspection[TempLoop])
	if (TempThisInspection != TempLoop) //ignore this inspection
	if ((vGlobalCurrentProduct->vInspection[TempLoop]->vInspectionType == cUnderfillByWeightInspection) || (vGlobalCurrentProduct->vInspection[TempLoop]->vInspectionType == cOverfillByWeightInspection))
	if (vGlobalCurrentProduct->vInspection[TempLoop]->vInspectionNameForWeightTrendingName.GetLength() == 0) //can not be pointing at another inspection
	{
		ISelectItemDialog.vItems[ISelectItemDialog.vNumberOfItems++] = vGlobalCurrentProduct->vInspection[TempLoop]->vName;
		if (vGlobalCurrentProduct->vInspection[TempThisInspection]->vInspectionNameForWeightTrendingName == vGlobalCurrentProduct->vInspection[TempLoop]->vName)
			ISelectItemDialog.vGreenItem = ISelectItemDialog.vNumberOfItems;
	}

	CString TempText = "-";
	TempText.LoadString(IDS_Product);
	ISelectItemDialog.m_DialogTitleStaticText2 = TempText + ": " + *(vGlobalCurrentProduct->GetProductName());
	ISelectItemDialog.m_DialogTitleStaticText3 = "";
	ISelectItemDialog.vInAutoSetup = vLocalSystemData->vInAutoSetup;
	//Pass control to dialog box and display
	int nResponse = ISelectItemDialog.DoModal();
	//dialog box is now closed, if user pressed select do this
	//if user pressed cancel, do nothing
	if (nResponse == IDOK)
	{
		if (ISelectItemDialog.vItemNumberSelected == 1)
			vGlobalCurrentProduct->vInspection[TempThisInspection]->vInspectionNameForWeightTrendingName = "";
		else
			vGlobalCurrentProduct->vInspection[TempThisInspection]->vInspectionNameForWeightTrendingName = ISelectItemDialog.vItems[ISelectItemDialog.vItemNumberSelected - 1];

		vGlobalCurrentProduct->MatchInspectionsForWeightTrending();
	}
}

void CConfigureInspectionsDialog::OnDialogtitlestatictext1()
{
	//bring up soft key menu for help, print screen, Image, Clear, and hidden shift key
	BYTE TempHiddenKeys = 8;
	if (!PasswordOK(cTemporaryInspxPassword,false))
		TempHiddenKeys = 10;
	PostThreadMessage(vGlobalCallingThreadID, cDisplaySoftKeyMenu, TempHiddenKeys, TempHiddenKeys);
}
