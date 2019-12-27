//ScanTrac Side View Source File
// DiagnosticCountersDialog.cpp : implementation file
//

#include "stdafx.h"
#include "ScanTrac.h"
#include "DiagnosticCountersDialog.h"
#include "YesNoDialog.h"
#include "AlphaEntryDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDiagnosticCountersDialog dialog

extern BYTE vGlobalShiftKeyDown;
extern CProduct *vGlobalCurrentProduct;
extern CLifeTimeCounters *vGlobalLifeTimeCounters;
extern int vGlobalPasswordLevel;
extern unsigned long vGlobalDialogTitleColor;

CDiagnosticCountersDialog::CDiagnosticCountersDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CDiagnosticCountersDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDiagnosticCountersDialog)
	//}}AFX_DATA_INIT
	vMakeExitButtonGreen = false;
	vUpdateDisplayTimerHandle = 56;
	vChangeMade = false;
	vOldXRayOnNotice = false;
	for (BYTE TempLoop = 0; TempLoop < cMaximumNumberOfCountsDisplayed; TempLoop++)
		vOldXRayCriteriaCount[TempLoop] = 0;
	vOldGoodCountTotal = 0;
	vOldEjectCountTotal = 0;
	vOldBodyTriggersCount = 0;
	vOldBackupBodyTriggersCount = 0;
	vOldValidBodyTriggersCount = 0;
	vOldIncompleteDataCount = 0;
	vOldUnConfirmedCount = 0;
	vOldImageReferenceErrorCount = 0;
	vOldProcessingErrorCount = 0;
	vOldTooLateToEjectErrorCount = 0;
	vOldMissedEjectsBecauseDisabledCount = 0;
	vOldRealImageCountTotal = 0;
	vOldMaximumExternalDetectorTime = 0;

	vLocalCWndCollection.Add(this);
	vLocalCWndCollection.Add(&m_SubFunction8Button);
	vLocalCWndCollection.Add(&m_SubFunction7Button);
	vLocalCWndCollection.Add(&m_Function1Button);
	vLocalCWndCollection.Add(&m_UnConfirmed);
	vLocalCWndCollection.Add(&m_UnConfirmedLabel);
	vLocalCWndCollection.Add(&m_BackupBodyTriggersLabel);
	vLocalCWndCollection.Add(&m_BackupBodyTriggers);
	vLocalCWndCollection.Add(&m_EjectedLabel);
	vLocalCWndCollection.Add(&m_ConfirmLabel);
	vLocalCWndCollection.Add(&m_Function2Button);
	vLocalCWndCollection.Add(&m_XRaysOn);
	vLocalCWndCollection.Add(&m_EjectorLabel3);
	vLocalCWndCollection.Add(&m_EjectorLabel2);
	vLocalCWndCollection.Add(&m_EjectorLabel1);
	vLocalCWndCollection.Add(&m_EjectorConfirm3);
	vLocalCWndCollection.Add(&m_EjectorConfirm2);
	vLocalCWndCollection.Add(&m_EjectorConfirm1);
	vLocalCWndCollection.Add(&m_Ejector3);
	vLocalCWndCollection.Add(&m_Ejector2);
	vLocalCWndCollection.Add(&m_Ejector1);
	vLocalCWndCollection.Add(&m_Background);
	vLocalCWndCollection.Add(&m_BodyTriggers);
	vLocalCWndCollection.Add(&m_BodyTriggersLabel);
	vLocalCWndCollection.Add(&m_DetectorRunDuration);
	vLocalCWndCollection.Add(&m_DetectorRunDurationLabel);
	vLocalCWndCollection.Add(&m_DialogTitleStaticText);
	vLocalCWndCollection.Add(&m_Function3Button);
	vLocalCWndCollection.Add(&m_Function4Button);
	vLocalCWndCollection.Add(&m_Function5Button);
	vLocalCWndCollection.Add(&m_ImageReferenceError);
	vLocalCWndCollection.Add(&m_ImageReferenceErrorLabel);
	vLocalCWndCollection.Add(&m_IncompleteData);
	vLocalCWndCollection.Add(&m_IncompleteDataLabel);
	vLocalCWndCollection.Add(&m_MaxTime);
	vLocalCWndCollection.Add(&m_MaxTimeLabel);
	vLocalCWndCollection.Add(&m_MissedEjectsBecauseDisabled);
	vLocalCWndCollection.Add(&m_MissedEjectsBecauseDisabledLabel);
	vLocalCWndCollection.Add(&m_ProcessingError);
	vLocalCWndCollection.Add(&m_ProcessingErrorLabel);
	vLocalCWndCollection.Add(&m_RealImages);
	vLocalCWndCollection.Add(&m_RealImagesLabel);
	vLocalCWndCollection.Add(&m_ScanTracRunDuration);
	vLocalCWndCollection.Add(&m_ScanTracRunDurationLabel);
	vLocalCWndCollection.Add(&m_SubFunction1Button);
	vLocalCWndCollection.Add(&m_SubFunction2Button);
	vLocalCWndCollection.Add(&m_SubFunction3Button);
	vLocalCWndCollection.Add(&m_SubFunction4Button);
	vLocalCWndCollection.Add(&m_SubFunction5Button);
	vLocalCWndCollection.Add(&m_SubFunction6Button);
	vLocalCWndCollection.Add(&m_TooLateToEjectError);
	vLocalCWndCollection.Add(&m_TooLateToEjectErrorLabel);
	vLocalCWndCollection.Add(&m_ValidBodyTriggers);
	vLocalCWndCollection.Add(&m_ValidBodyTriggersLabel);
}

void CDiagnosticCountersDialog::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	bool TempUsedOne = false;
	
	// TODO: Add your message handler code here
	m_Background.SetWindowPos(NULL,cMainBackgroundLeft,
		cMainBackgroundTop,cMainBackgroundLength,
		cMainBackgroundHeight,SWP_NOZORDER);

	SetDlgItemText(IDC_ValidBodyTriggersLabel, _T("Total Good:"));
	SetDlgItemText(IDC_RealImagesLabel, _T("Total Ejected:"));

	for (BYTE TempLoop = 0; TempLoop < cNumberOfEjectors; TempLoop++)
	{
		vOldEjector[TempLoop] = 0;
		vOldEjectorConfirm[TempLoop] = 0;
	}
	SetDlgItemText(IDC_EjectorLabel1,
		vLocalConfigurationData->vEjector[0].vName + ":");
	SetDlgItemText(IDC_EjectorLabel2,
		vLocalConfigurationData->vEjector[1].vName + ":");
	SetDlgItemText(IDC_EjectorLabel3,
		vLocalConfigurationData->vEjector[2].vName + ":");

	/*
	for ( TempLoop = 0; TempLoop < cNumberOfExternalDetectors; TempLoop++)
	{
		vOldExternalDetectorCount[TempLoop] = 0;
		vOldXRayCriteriaCount[TempLoop + cNumberOfMinorXRayImageCriteria] = 0;
	}
	SetDlgItemText(IDC_ExternalDetectorLabel1,
		vLocalConfigurationData->vExternalDetector[0].vName + ":");
	SetDlgItemText(IDC_ExternalDetectorLabel2,
		vLocalConfigurationData->vExternalDetector[1].vName + ":");
	SetDlgItemText(IDC_ExternalDetectorLabel3,
		vLocalConfigurationData->vExternalDetector[2].vName + ":");
	SetDlgItemText(IDC_ExternalDetectorLabel4,
		vLocalConfigurationData->vExternalDetector[3].vName + ":");
	SetDlgItemText(IDC_ExternalDetectorLabel5,
		vLocalConfigurationData->vExternalDetector[4].vName + ":");
*/
	if (vGlobalCurrentProduct != NULL)
	{
		if (vGlobalCurrentProduct->vBodyTrigger > 0)
			SetDlgItemText(IDC_BodyTriggersLabel,
				vLocalConfigurationData->vBodyTrigger[vGlobalCurrentProduct->vBodyTrigger - 1].vName + ":");
		if (vGlobalCurrentProduct->vBackupBodyTrigger > 0)
			SetDlgItemText(IDC_BackupBodyTriggersLabel,
				vLocalConfigurationData->vBodyTrigger[vGlobalCurrentProduct->vBackupBodyTrigger - 1].vName + ":");
		else
		{
			m_BackupBodyTriggersLabel.ShowWindow(SW_HIDE);
			m_BackupBodyTriggers.ShowWindow(SW_HIDE);
		}
	}

	//if (!vLocalConfigurationData->vExternalDetector[0].vEnabled)
	//	SetDlgItemText(IDC_ExternalDetector1,"Disabled");
	//if (!vLocalConfigurationData->vExternalDetector[1].vEnabled)
	//	SetDlgItemText(IDC_ExternalDetector2,"Disabled");
	//if (!vLocalConfigurationData->vExternalDetector[2].vEnabled)
	//	SetDlgItemText(IDC_ExternalDetector3,"Disabled");
	//if (!vLocalConfigurationData->vExternalDetector[3].vEnabled)
	//	SetDlgItemText(IDC_ExternalDetector4,"Disabled");
	//if (!vLocalConfigurationData->vExternalDetector[4].vEnabled)
	//	SetDlgItemText(IDC_ExternalDetector5,"Disabled");

	if (vGlobalCurrentProduct != NULL)
	{
		/*
		if (!vGlobalCurrentProduct->vExternalDetectorEnable[0])
		{
			SetDlgItemText(IDC_ExternalDetector1,"Not Used");
			SetDlgItemText(IDC_ExternalDetectorEjects1,"Not Used");
			m_ExternalDetector1.ShowWindow(SW_HIDE);
			m_ExternalDetectorEjects1.ShowWindow(SW_HIDE);
			m_ExternalDetectorLabel1.ShowWindow(SW_HIDE);
		}
		else
			TempUsedOne = true;
		if (!vGlobalCurrentProduct->vExternalDetectorEnable[1])
		{
			SetDlgItemText(IDC_ExternalDetector2,"Not Used");
			SetDlgItemText(IDC_ExternalDetectorEjects2,"Not Used");
			m_ExternalDetector2.ShowWindow(SW_HIDE);
			m_ExternalDetectorEjects2.ShowWindow(SW_HIDE);
			m_ExternalDetectorLabel2.ShowWindow(SW_HIDE);
		}
		else
			TempUsedOne = true;
		if (!vGlobalCurrentProduct->vExternalDetectorEnable[2])
		{
			SetDlgItemText(IDC_ExternalDetector3,"Not Used");
			SetDlgItemText(IDC_ExternalDetectorEjects3,"Not Used");
			m_ExternalDetector3.ShowWindow(SW_HIDE);
			m_ExternalDetectorEjects3.ShowWindow(SW_HIDE);
			m_ExternalDetectorLabel3.ShowWindow(SW_HIDE);
		}
		else
			TempUsedOne = true;
		if (!vGlobalCurrentProduct->vExternalDetectorEnable[3])
		{
			SetDlgItemText(IDC_ExternalDetector4,"Not Used");
			SetDlgItemText(IDC_ExternalDetectorEjects4,"Not Used");
			m_ExternalDetector4.ShowWindow(SW_HIDE);
			m_ExternalDetectorEjects4.ShowWindow(SW_HIDE);
			m_ExternalDetectorLabel4.ShowWindow(SW_HIDE);
		}
		else
			TempUsedOne = true;
		if (!vGlobalCurrentProduct->vExternalDetectorEnable[4])
		{
			SetDlgItemText(IDC_ExternalDetector5,"Not Used");
			SetDlgItemText(IDC_ExternalDetectorEjects5,"Not Used");
			m_ExternalDetector5.ShowWindow(SW_HIDE);
			m_ExternalDetectorEjects5.ShowWindow(SW_HIDE);
			m_ExternalDetectorLabel5.ShowWindow(SW_HIDE);
		}
		else
			TempUsedOne = true;
		if (!TempUsedOne)
			m_EjectedLabel.ShowWindow(SW_HIDE);
			*/

		if (!(vLocalSystemData->vEjectorUsed & 1)) 
		{
			SetDlgItemText(IDC_Ejector1, _T("Not Used"));
			SetDlgItemText(IDC_EjectorConfirm1, _T("Not Used"));
			m_Ejector1.ShowWindow(SW_HIDE);
			m_EjectorLabel1.ShowWindow(SW_HIDE);
			m_EjectorConfirm1.ShowWindow(SW_HIDE);
		}
		if (!(vLocalSystemData->vEjectorUsed & 2))
		{
			SetDlgItemText(IDC_Ejector2, _T("Not Used"));
			SetDlgItemText(IDC_EjectorConfirm2, _T("Not Used"));
			m_Ejector2.ShowWindow(SW_HIDE);
			m_EjectorLabel2.ShowWindow(SW_HIDE);
			m_EjectorConfirm2.ShowWindow(SW_HIDE);
		}
		if (!(vLocalSystemData->vEjectorUsed & 4))
		{
			SetDlgItemText(IDC_Ejector3, _T("Not Used"));
			SetDlgItemText(IDC_EjectorConfirm3, _T("Not Used"));
			m_Ejector3.ShowWindow(SW_HIDE);
			m_EjectorLabel3.ShowWindow(SW_HIDE);
			m_EjectorConfirm3.ShowWindow(SW_HIDE);
		}
	}

	if (!vLocalConfigurationData->vEjector[0].vEnabled) 
	{
		SetDlgItemText(IDC_Ejector1, _T("Disabled"));
		m_Ejector1.ShowWindow(SW_HIDE);
		m_EjectorLabel1.ShowWindow(SW_HIDE);
	}
	if (!vLocalConfigurationData->vEjector[1].vEnabled)
	{
		SetDlgItemText(IDC_Ejector2, _T("Disabled"));
		m_Ejector2.ShowWindow(SW_HIDE);
		m_EjectorLabel2.ShowWindow(SW_HIDE);
	}
	if (!vLocalConfigurationData->vEjector[2].vEnabled)
	{
		SetDlgItemText(IDC_Ejector3, _T("Disabled"));
		m_Ejector3.ShowWindow(SW_HIDE);
		m_EjectorLabel3.ShowWindow(SW_HIDE);
	}

	TempUsedOne = false;
	if ((!vLocalConfigurationData->vEjector[0].vConfirmEnabled) ||
		(!vLocalConfigurationData->vEjector[0].vEnabled))
	{
		SetDlgItemText(IDC_EjectorConfirm1, _T("Disabled"));
		m_EjectorConfirm1.ShowWindow(SW_HIDE);
	}
	if ((vLocalConfigurationData->vEjector[0].vConfirmEnabled) &&
		(vLocalConfigurationData->vEjector[0].vEnabled) &&
		(vLocalSystemData->vEjectorUsed & 1))
		TempUsedOne = true;
	if ((!vLocalConfigurationData->vEjector[1].vConfirmEnabled) ||
		(!vLocalConfigurationData->vEjector[1].vEnabled))
	{
		SetDlgItemText(IDC_EjectorConfirm2, _T("Disabled"));
		m_EjectorConfirm2.ShowWindow(SW_HIDE);
	}
	if ((vLocalConfigurationData->vEjector[1].vConfirmEnabled) &&
		(vLocalConfigurationData->vEjector[1].vEnabled) &&
		(vLocalSystemData->vEjectorUsed & 2))
		TempUsedOne = true;
	if ((!vLocalConfigurationData->vEjector[2].vConfirmEnabled) ||
		(!vLocalConfigurationData->vEjector[2].vEnabled))
	{
		SetDlgItemText(IDC_EjectorConfirm3, _T("Disabled"));
		m_EjectorConfirm3.ShowWindow(SW_HIDE);
	}
	if ((vLocalConfigurationData->vEjector[2].vConfirmEnabled) &&
		(vLocalConfigurationData->vEjector[2].vEnabled) &&
		(vLocalSystemData->vEjectorUsed & 4))
		TempUsedOne = true;
	if (!TempUsedOne)
	{
		if (vGlobalCurrentProduct)
			SetDlgItemText(IDC_UnConfirmed, _T("Not Used"));
		m_ConfirmLabel.ShowWindow(SW_HIDE);
		m_UnConfirmedLabel.ShowWindow(SW_HIDE);
		m_UnConfirmed.ShowWindow(SW_HIDE);
	}

	TempUsedOne = false;
	/*
	if (!vLocalConfigurationData->vExternalDetector[0].vEnabled)
	{
		SetDlgItemText(IDC_ExternalDetector1,"Disabled");
		m_ExternalDetector1.ShowWindow(SW_HIDE);
		m_ExternalDetectorLabel1.ShowWindow(SW_HIDE);
	}
	else
		TempUsedOne = true;
	if (!vLocalConfigurationData->vExternalDetector[1].vEnabled)
	{
		SetDlgItemText(IDC_ExternalDetector2,"Disabled");
		m_ExternalDetector2.ShowWindow(SW_HIDE);
		m_ExternalDetectorLabel2.ShowWindow(SW_HIDE);
	}
	else
		TempUsedOne = true;
	if (!vLocalConfigurationData->vExternalDetector[2].vEnabled)
	{
		SetDlgItemText(IDC_ExternalDetector3,"Disabled");
		m_ExternalDetector3.ShowWindow(SW_HIDE);
		m_ExternalDetectorLabel3.ShowWindow(SW_HIDE);
	}
	else
		TempUsedOne = true;
	if (!vLocalConfigurationData->vExternalDetector[3].vEnabled)
	{
		SetDlgItemText(IDC_ExternalDetector4,"Disabled");
		m_ExternalDetector4.ShowWindow(SW_HIDE);
		m_ExternalDetectorLabel4.ShowWindow(SW_HIDE);
	}
	else
		TempUsedOne = true;
	if (!vLocalConfigurationData->vExternalDetector[4].vEnabled)
	{
		SetDlgItemText(IDC_ExternalDetector5,"Disabled");
		m_ExternalDetector5.ShowWindow(SW_HIDE);
		m_ExternalDetectorLabel5.ShowWindow(SW_HIDE);
	}
	else
		TempUsedOne = true;
		*/
	if (!TempUsedOne)
		m_EjectedLabel.ShowWindow(SW_HIDE);
	UpdateDisplay();

	if (vLocalSystemData->vInAutoSetup)
	{
		vMakeExitButtonGreen = true;
		m_Function1Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		SetDlgItemText(IDC_Function1Button, _T("Return to Auto Setup"));
	}
	else
	{
		CWnd *TempWindow = NULL;
		TempWindow = FindWindow(NULL, _T("Help"));
		if (TempWindow) 
		{
			vMakeExitButtonGreen = true;
			m_Function1Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
			SetDlgItemText(IDC_Function1Button, _T("Exit Diagnostics"));
		}
	}
	UpdateButtons();
	
	int TempTimerResult = SetTimer(vUpdateDisplayTimerHandle,500,NULL);
	if (!TempTimerResult)
		ReportErrorMessage("Error-Update Display Timer Failed",cEMailInspx,32000);
	this->SetWindowText(_T("Counters"));
	::SetupMenu(vLocalCWndCollection);
}


void CDiagnosticCountersDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDiagnosticCountersDialog)
	DDX_Control(pDX, IDC_SubFunction8Button, m_SubFunction8Button);
	DDX_Control(pDX, IDC_SubFunction7Button, m_SubFunction7Button);
	DDX_Control(pDX, IDC_Function1Button, m_Function1Button);
	DDX_Control(pDX, IDC_UnConfirmed, m_UnConfirmed);
	DDX_Control(pDX, IDC_UnConfirmedLabel, m_UnConfirmedLabel);
	DDX_Control(pDX, IDC_BackupBodyTriggersLabel, m_BackupBodyTriggersLabel);
	DDX_Control(pDX, IDC_BackupBodyTriggers, m_BackupBodyTriggers);
	DDX_Control(pDX, IDC_EjectedLabel, m_EjectedLabel);
	DDX_Control(pDX, IDC_ConfirmLabel, m_ConfirmLabel);
	DDX_Control(pDX, IDC_Function2Button, m_Function2Button);
	DDX_Control(pDX, IDC_XRaysOn, m_XRaysOn);
	DDX_Control(pDX, IDC_EjectorLabel3, m_EjectorLabel3);
	DDX_Control(pDX, IDC_EjectorLabel2, m_EjectorLabel2);
	DDX_Control(pDX, IDC_EjectorLabel1, m_EjectorLabel1);
	DDX_Control(pDX, IDC_EjectorConfirm3, m_EjectorConfirm3);
	DDX_Control(pDX, IDC_EjectorConfirm2, m_EjectorConfirm2);
	DDX_Control(pDX, IDC_EjectorConfirm1, m_EjectorConfirm1);
	DDX_Control(pDX, IDC_Ejector3, m_Ejector3);
	DDX_Control(pDX, IDC_Ejector2, m_Ejector2);
	DDX_Control(pDX, IDC_Ejector1, m_Ejector1);
	DDX_Control(pDX, IDC_Background, m_Background);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_BodyTriggers, m_BodyTriggers);
	DDX_Control(pDX, IDC_BodyTriggersLabel, m_BodyTriggersLabel);
	DDX_Control(pDX, IDC_DetectorRunDuration, m_DetectorRunDuration);
	DDX_Control(pDX, IDC_DetectorRunDurationLabel, m_DetectorRunDurationLabel);
	DDX_Control(pDX, IDC_DialogTitleStaticText1, m_DialogTitleStaticText);
	DDX_Control(pDX, IDC_Function3Button, m_Function3Button);
	DDX_Control(pDX, IDC_Function4Button, m_Function4Button);
	DDX_Control(pDX, IDC_Function5Button, m_Function5Button);
	DDX_Control(pDX, IDC_ImageReferenceError, m_ImageReferenceError);
	DDX_Control(pDX, IDC_ImageReferenceErrorLabel, m_ImageReferenceErrorLabel);
	DDX_Control(pDX, IDC_IncompleteData, m_IncompleteData);
	DDX_Control(pDX, IDC_IncompleteDataLabel, m_IncompleteDataLabel);
	DDX_Control(pDX, IDC_MaxTime, m_MaxTime);
	DDX_Control(pDX, IDC_MaxTimeLabel, m_MaxTimeLabel);
	DDX_Control(pDX, IDC_MissedEjectsBecauseDisabled, m_MissedEjectsBecauseDisabled);
	DDX_Control(pDX, IDC_MissedEjectsBecauseDisabledLabel, m_MissedEjectsBecauseDisabledLabel);
	DDX_Control(pDX, IDC_ProcessingError, m_ProcessingError);
	DDX_Control(pDX, IDC_ProcessingErrorLabel, m_ProcessingErrorLabel);
	DDX_Control(pDX, IDC_RealImages, m_RealImages);
	DDX_Control(pDX, IDC_RealImagesLabel, m_RealImagesLabel);
	DDX_Control(pDX, IDC_ScanTracRunDuration, m_ScanTracRunDuration);
	DDX_Control(pDX, IDC_ScanTracRunDurationLabel, m_ScanTracRunDurationLabel);
	DDX_Control(pDX, IDC_SubFunction1Button, m_SubFunction1Button);
	DDX_Control(pDX, IDC_SubFunction2Button, m_SubFunction2Button);
	DDX_Control(pDX, IDC_SubFunction3Button, m_SubFunction3Button);
	DDX_Control(pDX, IDC_SubFunction4Button, m_SubFunction4Button);
	DDX_Control(pDX, IDC_SubFunction5Button, m_SubFunction5Button);
	DDX_Control(pDX, IDC_SubFunction6Button, m_SubFunction6Button);
	DDX_Control(pDX, IDC_TooLateToEjectError, m_TooLateToEjectError);
	DDX_Control(pDX, IDC_TooLateToEjectErrorLabel, m_TooLateToEjectErrorLabel);
	DDX_Control(pDX, IDC_ValidBodyTriggers, m_ValidBodyTriggers);
	DDX_Control(pDX, IDC_ValidBodyTriggersLabel, m_ValidBodyTriggersLabel);
}


BEGIN_MESSAGE_MAP(CDiagnosticCountersDialog, CDialog)
	//{{AFX_MSG_MAP(CDiagnosticCountersDialog)
	ON_WM_CTLCOLOR()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
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
	//}}AFX_MSG_MAP
	ON_STN_CLICKED(IDC_DialogTitleStaticText1, &CDiagnosticCountersDialog::OnDialogtitlestatictext1)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDiagnosticCountersDialog message handlers

void CDiagnosticCountersDialog::OnFunction1Button() 
{
	// User clicked Main Menu Button to exit 
	if ((!vChangeMade) || (AbortSetupChanges()))
		CDialog::EndDialog(10);
}

void CDiagnosticCountersDialog::OnFunction2Button() 
{
	// Save and Exit Button was pressed
	if ((!vChangeMade) || (KeepSetupChanges()))
	{
		CDialog::EndDialog(true);
	}
}

void CDiagnosticCountersDialog::OnFunction3Button() 
{
	// TODO: Add your control notification handler code here
	
}

void CDiagnosticCountersDialog::OnFunction4Button() 
{
}

void CDiagnosticCountersDialog::OnFunction5Button() 
{
	// TODO: Add your control notification handler code here
	
}

void CDiagnosticCountersDialog::OnSubFunction1Button() 
{
	// TODO: Add your control notification handler code here
	
}

void CDiagnosticCountersDialog::OnSubFunction2Button() 
{
	// TODO: Add your control notification handler code here
	
}

void CDiagnosticCountersDialog::OnSubFunction3Button() 
{
	// TODO: Add your control notification handler code here
	
}

void CDiagnosticCountersDialog::OnSubFunction4Button() 
{
	// TODO: Add your control notification handler code here
	
}

void CDiagnosticCountersDialog::OnSubFunction5Button() 
{
	// TODO: Add your control notification handler code here
	
}

void CDiagnosticCountersDialog::OnSubFunction6Button() 
{
	// TODO: Add your control notification handler code here
	
}

void CDiagnosticCountersDialog::OnSubFunction7Button() 
{
}

void CDiagnosticCountersDialog::OnSubFunction8Button() 
{
}

BOOL CDiagnosticCountersDialog::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	CWnd * TempWindow = NULL;
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
			case 16: // Shift Key
			case 0x60:  //0 key on KeyPad
				PostThreadMessage(vGlobalCallingThreadID,cHiddenKeyPressed, 1, 0);
			break;
			case 36:  //Home key, Screen Capture
				vGlobalShiftKeyDown = true;
				PostThreadMessage(vGlobalCallingThreadID,cHiddenKeyPressed, 0, 0x61);
			break;
			case cPageDown: // Help
				PostThreadMessage(vGlobalCallingThreadID,
					cShowHelp, 42, 0);
				{
					TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText1);
					if (TempWindow)
						TempWindow->Invalidate(false);
				}
			break;
			case 27: // Escape
				::PostThreadMessage(vGlobalCallingThreadID,	cStartChatWindowMessage, 0,0);
			break;	
			case cPageUp: // Clear
				if (vGlobalPasswordLevel)
				{
					PasswordOK(cNoPassword, false);
				}
				else
				{
					PasswordOK(cAnyPasswordToEnter, true);
				}	
				TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText1);
				if (TempWindow)
					TempWindow->Invalidate(false);
				UpdateButtons();
			break;	
		}
		return true;  //true indicates it has been handled, so do not process
	}
	return CDialog::PreTranslateMessage(pMsg);
}

HBRUSH CDiagnosticCountersDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	HWND TempDialogHwnd = this->m_hWnd;
	int TextSize;
	pDC->SetAttribDC(pDC->m_hDC);
	pDC->SetBkMode(TRANSPARENT);
	
	if (pWnd->GetDlgCtrlID() == IDC_DialogTitleStaticText1)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DialogTitleStaticText, 8);
		pDC->SetTextColor(vGlobalDialogTitleColor);
	}

	if (pWnd->GetDlgCtrlID() == IDC_Function1Button)
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function1Button, 5);  //5 is medium large
	if (vMakeExitButtonGreen)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vLocalSystemData->vGreenBrush;
  }
	}
	if 	(((pWnd->GetDlgCtrlID() == IDC_ProcessingError) && (vOldProcessingErrorCount)) ||
		((pWnd->GetDlgCtrlID() == IDC_TooLateToEjectError) && (vOldTooLateToEjectErrorCount)) ||
		((pWnd->GetDlgCtrlID() == IDC_UnConfirmed) && (vOldUnConfirmedCount)) ||
		((pWnd->GetDlgCtrlID() == IDC_IncompleteData) && (vOldIncompleteDataCount)))
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_ProcessingError, 5);  //5 is medium large
		if (vOldProcessingErrorCount)
		{
			pDC->SetBkMode(TRANSPARENT);
			return vLocalSystemData->vRedBrush;
		}
	}
	if (pWnd->GetDlgCtrlID() == IDC_TooLateToEjectError)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_TooLateToEjectError, 5);  //5 is medium large
		if (vOldTooLateToEjectErrorCount)
		{
			pDC->SetBkMode(TRANSPARENT);
			return vLocalSystemData->vRedBrush;
		}
	}
	if (pWnd->GetDlgCtrlID() == IDC_UnConfirmed)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_UnConfirmed, 5);  //5 is medium large
		if (vOldUnConfirmedCount)
		{
			pDC->SetBkMode(TRANSPARENT);
			return vLocalSystemData->vRedBrush;
		}
	}
	if (pWnd->GetDlgCtrlID() == IDC_IncompleteData)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_IncompleteData, 5);  //5 is medium large
		if (vOldIncompleteDataCount)
		{
			pDC->SetBkMode(TRANSPARENT);
			return vLocalSystemData->vRedBrush;
		}
	}

	if (pWnd->GetDlgCtrlID() == IDC_Background)
	{
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_ValidBodyTriggersLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_ValidBodyTriggersLabel, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_ConfirmLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_ConfirmLabel, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_ScanTracRunDurationLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_ScanTracRunDurationLabel, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DetectorRunDurationLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DetectorRunDurationLabel, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_EjectedLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_EjectedLabel, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_EjectorLabel1)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_EjectorLabel1, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_EjectorLabel2)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_EjectorLabel2, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_EjectorLabel3)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_EjectorLabel3, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_BodyTriggersLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_BodyTriggersLabel, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_BackupBodyTriggersLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_BackupBodyTriggersLabel, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_ProcessingErrorLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_ProcessingErrorLabel, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_RealImagesLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_RealImagesLabel, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_UnConfirmedLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_UnConfirmedLabel, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_TooLateToEjectErrorLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_TooLateToEjectErrorLabel, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_MissedEjectsBecauseDisabledLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_MissedEjectsBecauseDisabledLabel, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_ImageReferenceErrorLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_ImageReferenceErrorLabel, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_IncompleteDataLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_IncompleteDataLabel, 5);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
  }
	if (pWnd->GetDlgCtrlID() == IDC_ValidBodyTriggers)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_ValidBodyTriggers, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Ejector1)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Ejector1, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Ejector2)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Ejector2, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Ejector3)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Ejector3, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_ScanTracRunDuration)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_ScanTracRunDuration, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_DetectorRunDuration)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DetectorRunDuration, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_EjectorConfirm1)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_EjectorConfirm1, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_EjectorConfirm2)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_EjectorConfirm2, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_EjectorConfirm3)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_EjectorConfirm3, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_BodyTriggers)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_BodyTriggers, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_BackupBodyTriggers)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_BackupBodyTriggers, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_RealImages)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_RealImages, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_ProcessingError)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_ProcessingError, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_TooLateToEjectError)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_TooLateToEjectError, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_MissedEjectsBecauseDisabled)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_MissedEjectsBecauseDisabled, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_ImageReferenceError)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_ImageReferenceError, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_UnConfirmed) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_UnConfirmed, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_IncompleteData)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_IncompleteData, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cDarkBlue);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_XRaysOn)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_XRaysOn, 7);
		pDC->SetAttribDC(pDC->m_hDC);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cYellow);
		pDC->SetBkColor(cRed);
		return vLocalSystemData->vRedBrush;
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

void CDiagnosticCountersDialog::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
  if (nIDEvent == vUpdateDisplayTimerHandle)
  {
		UpdateDisplay();
	}
	CDialog::OnTimer(nIDEvent);
}


void CDiagnosticCountersDialog::UpdateDisplay()
{
	if (vOldXRayOnNotice != vLocalSystemData->vXRayOnNotice)
	{
		vOldXRayOnNotice = vLocalSystemData->vXRayOnNotice;
		if (vOldXRayOnNotice)
			m_XRaysOn.ShowWindow(SW_SHOW);
		else
			m_XRaysOn.ShowWindow(SW_HIDE);
	}
	if (vOldEjector[0] != vLocalSystemData->vEjectorCount[0])
	{
		vOldEjector[0] = vLocalSystemData->vEjectorCount[0];
		SetDlgItemText(IDC_Ejector1,IToAWithComma(vOldEjector[0]));
	}
	if (vOldEjector[1] != vLocalSystemData->vEjectorCount[1])
	{
		vOldEjector[1] = vLocalSystemData->vEjectorCount[1];
		SetDlgItemText(IDC_Ejector2,IToAWithComma(vOldEjector[1]));
	}
	if (vOldEjector[2] != vLocalSystemData->vEjectorCount[2])
	{
		vOldEjector[2] = vLocalSystemData->vEjectorCount[2];
		SetDlgItemText(IDC_Ejector3,IToAWithComma(vOldEjector[2]));
	}
	if (vOldEjectorConfirm[0] != vLocalSystemData->vEjectorConfirmCount[0])
	{
		vOldEjectorConfirm[0] = vLocalSystemData->vEjectorConfirmCount[0];
		SetDlgItemText(IDC_EjectorConfirm1,IToAWithComma(vOldEjectorConfirm[0]));
	}
	if (vOldEjectorConfirm[1] != vLocalSystemData->vEjectorConfirmCount[1])
	{
		vOldEjectorConfirm[1] = vLocalSystemData->vEjectorConfirmCount[1];
		SetDlgItemText(IDC_EjectorConfirm2,IToAWithComma(vOldEjectorConfirm[1]));
	}
	if (vOldEjectorConfirm[2] != vLocalSystemData->vEjectorConfirmCount[2])
	{
		vOldEjectorConfirm[2] = vLocalSystemData->vEjectorConfirmCount[2];
		SetDlgItemText(IDC_EjectorConfirm3,IToAWithComma(vOldEjectorConfirm[2]));
	}

	if (vOldBodyTriggersCount != vLocalSystemData->vBodyTriggersCount)
	{
		vOldBodyTriggersCount = vLocalSystemData->vBodyTriggersCount;
		SetDlgItemText(IDC_BodyTriggers,IToAWithComma(vOldBodyTriggersCount));
	}
	if (vOldBackupBodyTriggersCount != vLocalSystemData->vBackupBodyTriggersCount)
	{
		vOldBackupBodyTriggersCount = vLocalSystemData->vBackupBodyTriggersCount;
		SetDlgItemText(IDC_BackupBodyTriggers,IToAWithComma(vOldBackupBodyTriggersCount));
	}

	if (vOldGoodCountTotal != vLocalSystemData->vGoodCountTotal)
	{
		vOldGoodCountTotal = vLocalSystemData->vGoodCountTotal;
		SetDlgItemText(IDC_ValidBodyTriggers,IToAWithComma(vOldGoodCountTotal));
	}
	if (vOldEjectCountTotal != vLocalSystemData->vEjectCountTotal)
	{
		vOldEjectCountTotal = vLocalSystemData->vEjectCountTotal;
		SetDlgItemText(IDC_RealImages,IToAWithComma(vOldEjectCountTotal));
	}

	if (vOldIncompleteDataCount != vLocalSystemData->vIncompleteDataCount)
	{
		vOldIncompleteDataCount = vLocalSystemData->vIncompleteDataCount;
		SetDlgItemText(IDC_IncompleteData,IToAWithComma(vOldIncompleteDataCount));
	}
	
	if (vOldUnConfirmedCount != vLocalSystemData->vTotalUnConfirmed)
	{
		vOldUnConfirmedCount = vLocalSystemData->vTotalUnConfirmed;
		SetDlgItemText(IDC_UnConfirmed,IToAWithComma(vOldUnConfirmedCount));
	}
	
	if (vOldImageReferenceErrorCount != vLocalSystemData->vImageReferenceErrorCount)
	{
		vOldImageReferenceErrorCount = vLocalSystemData->vImageReferenceErrorCount;
		SetDlgItemText(IDC_ImageReferenceError,IToAWithComma(vOldImageReferenceErrorCount));
	}
	
	if (vOldProcessingErrorCount != vLocalSystemData->vProcessingErrorCount)
	{
		vOldProcessingErrorCount = vLocalSystemData->vProcessingErrorCount;
		SetDlgItemText(IDC_ProcessingError,IToAWithComma(vOldProcessingErrorCount));
	}
	if (vOldTooLateToEjectErrorCount != vLocalSystemData->vTooLateToEjectErrorCount)
	{
		vOldTooLateToEjectErrorCount = vLocalSystemData->vTooLateToEjectErrorCount;
		SetDlgItemText(IDC_TooLateToEjectError,IToAWithComma(vOldTooLateToEjectErrorCount));
	}
	if (vOldMissedEjectsBecauseDisabledCount != vLocalSystemData->vMissedEjectsBecauseDisabledCount)
	{
		vOldMissedEjectsBecauseDisabledCount = vLocalSystemData->vMissedEjectsBecauseDisabledCount;
		SetDlgItemText(IDC_MissedEjectsBecauseDisabled,
			IToAWithComma(vOldMissedEjectsBecauseDisabledCount));
	}
	//for (TempLoop = 0; TempLoop < cNumberOfExternalDetectors; TempLoop++)
	//{
	//	vLocalSystemData->vExternalDetectorCount[TempLoop]
}

void CDiagnosticCountersDialog::SetChangeMade() 
{
	if (!vChangeMade)
	{
		m_Function2Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		SetDlgItemText(IDC_Function2Button, _T("Save\nand Exit"));
		SetDlgItemText(IDC_Function4Button, _T("Cancel"));
		vChangeMade = true;
	}
}

void CDiagnosticCountersDialog::UpdateButtons()
{
	/*
	if (vGlobalResetPasswordOK)
	{
		m_SubFunction7Button.ShowWindow(SW_SHOW);
		m_ArrowRight3.ShowWindow(SW_SHOW);
		m_SubFunction8Button.ShowWindow(SW_SHOW);
		m_ArrowRight4.ShowWindow(SW_SHOW);
	}
	else
	*/
	{
		m_SubFunction7Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction7Button.GetRectToInvalidateArrow());
		m_SubFunction8Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction8Button.GetRectToInvalidateArrow());
	}
}



void CDiagnosticCountersDialog::OnDialogtitlestatictext1()
{
	BYTE TempHiddenKeys = 8;
	if (!PasswordOK(cTemporaryInspxPassword,false))
		TempHiddenKeys = 10;
	PostThreadMessage(vGlobalCallingThreadID, cDisplaySoftKeyMenu, TempHiddenKeys, TempHiddenKeys);
}
