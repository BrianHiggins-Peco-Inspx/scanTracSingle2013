//ScanTrac Side View Source File
// XRayTubeAlignmentDialog.cpp : implementation file
//

#include "stdafx.h"
#include "scantrac.h"
#include "XRayTubeAlignmentDialog.h"
#include "SerialPort.h"
#include "YesNoDialog.h"
#include "HelpDialog.h"
#include "AlphaEntryDialog.h"
#include "NumericEntryDialog.h"
#include "SpellmanDXMXRaySourcePort.h"
#include "XRaySource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern BYTE vGlobalScanTracType;
extern CSpellmanDXMXRaySourcePort *vGlobalDXMXRayPort;
extern bool vGlobalFPGAVersion10Point0OrHigher;
extern CLifeTimeCounters *vGlobalLifeTimeCounters;
extern BYTE vGlobalShiftKeyDown;
extern CProduct *vGlobalCurrentProduct;
extern CSerialPort *vGlobaluCSerialPort;
extern unsigned long vGlobalDialogTitleColor;
extern int vGlobalPasswordLevel;
/////////////////////////////////////////////////////////////////////////////
// CXRayTubeAlignmentDialog dialog


CXRayTubeAlignmentDialog::CXRayTubeAlignmentDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CXRayTubeAlignmentDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CXRayTubeAlignmentDialog)
	//}}AFX_DATA_INIT
	vHaveAnImage = false;
	vUpdateButtonTimerHandle = 99;
	vSavedData = false;
	vMainWindowPointer = NULL;
	vLeftSide = 77;
	vRightSide = 206;
	vTop = 56; 
	vBottom = 326;
	vHeight = vBottom - vTop;
	vWidth = vRightSide - vLeftSide;
	vTraceHeight = 20;

	vLeftSide = ResizeYCoor(vLeftSide);
	vRightSide = ResizeYCoor(vRightSide);
	vTop = ResizeYCoor(vTop); 
	vBottom = ResizeYCoor(vBottom);
	vHeight = ResizeYCoor(vHeight);
	vWidth = ResizeXCoor(vWidth);
	vTraceHeight = ResizeYCoor(vTraceHeight);
	
	vGrabFocusTimerHandle = 28101;
	vOldXRayOnNotice = false;
	vDarkBluePen.CreatePen(PS_SOLID, 1, cDarkBlue);
	vBluePen.CreatePen(PS_SOLID, 1, cBlue);

	vLocalCWndCollection.Add(this);
	vLocalCWndCollection.Add(&m_BackLabel);
	vLocalCWndCollection.Add(&m_MaximumDetectorAverage9);
	vLocalCWndCollection.Add(&m_MaximumDetectorAverage8);
	vLocalCWndCollection.Add(&m_MaximumDetectorAverage7);
	vLocalCWndCollection.Add(&m_MaximumDetectorAverage12);
	vLocalCWndCollection.Add(&m_MaximumDetectorAverage11);
	vLocalCWndCollection.Add(&m_MaximumDetectorAverage10);
	vLocalCWndCollection.Add(&m_CurrentDetectorAverage9);
	vLocalCWndCollection.Add(&m_CurrentDetectorAverage8);
	vLocalCWndCollection.Add(&m_CurrentDetectorAverage7);
	vLocalCWndCollection.Add(&m_CurrentDetectorAverage12);
	vLocalCWndCollection.Add(&m_CurrentDetectorAverage11);
	vLocalCWndCollection.Add(&m_CurrentDetectorAverage10);
	vLocalCWndCollection.Add(&m_Detector9);
	vLocalCWndCollection.Add(&m_Detector8);
	vLocalCWndCollection.Add(&m_Detector7);
	vLocalCWndCollection.Add(&m_Detector12);
	vLocalCWndCollection.Add(&m_Detector11);
	vLocalCWndCollection.Add(&m_Detector10);
	vLocalCWndCollection.Add(&m_MaximumDetectorAverage6);
	vLocalCWndCollection.Add(&m_MaximumDetectorAverage5);
	vLocalCWndCollection.Add(&m_Detector6);
	vLocalCWndCollection.Add(&m_Detector5);
	vLocalCWndCollection.Add(&m_CurrentDetectorAverage5);
	vLocalCWndCollection.Add(&m_CurrentDetectorAverage6);
	vLocalCWndCollection.Add(&m_VoltageButton);
	vLocalCWndCollection.Add(&m_CurrentButton);
	vLocalCWndCollection.Add(&m_SubFunction8Button);
	vLocalCWndCollection.Add(&m_SubFunction4Display);
	vLocalCWndCollection.Add(&m_Function5Button);
	vLocalCWndCollection.Add(&m_Function4Button);
	vLocalCWndCollection.Add(&m_SubFunction4Button);
	vLocalCWndCollection.Add(&m_Calibrating);
	vLocalCWndCollection.Add(&m_XRaysOn);
	vLocalCWndCollection.Add(&m_FrontLabel);
	vLocalCWndCollection.Add(&m_Maximum);
	vLocalCWndCollection.Add(&m_Current);
	vLocalCWndCollection.Add(&m_Black);
	vLocalCWndCollection.Add(&m_CurrentDetectorAverage1);
	vLocalCWndCollection.Add(&m_CurrentDetectorAverage2);
	vLocalCWndCollection.Add(&m_CurrentDetectorAverage3);
	vLocalCWndCollection.Add(&m_CurrentDetectorAverage4);
	vLocalCWndCollection.Add(&m_Detector1);
	vLocalCWndCollection.Add(&m_Detector2);
	vLocalCWndCollection.Add(&m_Detector3);
	vLocalCWndCollection.Add(&m_Detector4);
	vLocalCWndCollection.Add(&m_DialogTitleStaticText);
	vLocalCWndCollection.Add(&m_Function1Button);
	vLocalCWndCollection.Add(&m_Function2Button);
	vLocalCWndCollection.Add(&m_Function3Button);
	vLocalCWndCollection.Add(&m_Function6Button);
	vLocalCWndCollection.Add(&m_Function6Display);
	vLocalCWndCollection.Add(&m_MaximumDetectorAverage1);
	vLocalCWndCollection.Add(&m_MaximumDetectorAverage2);
	vLocalCWndCollection.Add(&m_MaximumDetectorAverage3);
	vLocalCWndCollection.Add(&m_MaximumDetectorAverage4);
	vLocalCWndCollection.Add(&m_SubFunction8DisplayA);
	vLocalCWndCollection.Add(&m_White);
}

void CXRayTubeAlignmentDialog::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: Add your message handler code here
	CString TempText;
	TempText.LoadString(IDS_XRAYSON);
	SetDlgItemText(IDC_XRaysOn,TempText);

	vOriginalCurrentProduct = vGlobalCurrentProduct;
	
	bool TempMadeANewProduct = false;
	CProduct *TempProduct = vMainWindowPointer->GetProductByName("~AlignTube");
	if (!TempProduct)
	{
		if (ThereIsEnoughMemory(sizeof(CProduct), "New Measure Product "))
			TempProduct = new CProduct;
		TempMadeANewProduct = true;
	}

	if ((vGlobalScanTracType == cSoloScanTrac) || (vLocalConfigurationData->vScanTracType == cSoloPlusScanTrac) || (vGlobalScanTracType == cUpSideDownSoloScanTrac))
	{
		m_SubFunction4Button.ShowWindow(SW_SHOW);
		m_SubFunction4Display.ShowWindow(SW_SHOW);
	}
		
	if ((vGlobalScanTracType == cAtempoScanTrac) || (vGlobalScanTracType == cForteScanTrac) || (vGlobalScanTracType == cAllegroScanTrac) ||
		(vGlobalScanTracType == cCaseInspectorScanTrac))
	{
		m_BackLabel.ShowWindow(SW_SHOW);
		m_FrontLabel.ShowWindow(SW_SHOW);
	}

	m_SubFunction8Button.ShowWindow(SW_SHOW);
	m_Function5Button.ShowWindow(SW_SHOW);
	m_Function4Button.ShowWindow(SW_SHOW);

	if (vLocalConfigurationData->vNumberOfXRayDetectors == 9)
	{
		m_Detector6.ShowWindow(SW_HIDE);
		m_Detector7.ShowWindow(SW_SHOW);
		m_Detector8.ShowWindow(SW_SHOW);
		m_Detector9.ShowWindow(SW_SHOW);
		m_Detector10.ShowWindow(SW_SHOW);

		m_CurrentDetectorAverage6.ShowWindow(SW_HIDE);
		m_MaximumDetectorAverage6.ShowWindow(SW_HIDE);
		m_CurrentDetectorAverage7.ShowWindow(SW_SHOW);
		m_MaximumDetectorAverage7.ShowWindow(SW_SHOW);
		m_CurrentDetectorAverage8.ShowWindow(SW_SHOW);
		m_MaximumDetectorAverage8.ShowWindow(SW_SHOW);
		m_CurrentDetectorAverage9.ShowWindow(SW_SHOW);
		m_MaximumDetectorAverage9.ShowWindow(SW_SHOW);
		m_CurrentDetectorAverage10.ShowWindow(SW_SHOW);
		m_MaximumDetectorAverage10.ShowWindow(SW_SHOW);

		SetDlgItemText(IDC_Detector1, _T("Detector 1"));//front
		SetDlgItemText(IDC_Detector7, _T("Detector 2"));
		SetDlgItemText(IDC_Detector2, _T("Detector 3"));
		SetDlgItemText(IDC_Detector8, _T("Detector 4"));
		SetDlgItemText(IDC_Detector3, _T("Detector 5"));
		SetDlgItemText(IDC_Detector9, _T("Detector 6"));
		SetDlgItemText(IDC_Detector4, _T("Detector 7"));
		SetDlgItemText(IDC_Detector10, _T("Detector 8"));
		SetDlgItemText(IDC_Detector5, _T("Detector 9"));

		m_BackLabel.SetWindowPos(NULL, 3, 100, 100, 20, SWP_NOZORDER);
		m_Current.SetWindowPos(NULL, 470, 100, 60, 20, SWP_NOZORDER);
		m_Maximum.SetWindowPos(NULL, 545, 100, 70, 20, SWP_NOZORDER);
	}
	if (vLocalConfigurationData->vNumberOfXRayDetectors == 12)
	{
		m_Detector7.ShowWindow(SW_SHOW);
		m_Detector8.ShowWindow(SW_SHOW);
		m_Detector9.ShowWindow(SW_SHOW);
		m_Detector10.ShowWindow(SW_SHOW);
		m_Detector11.ShowWindow(SW_SHOW);
		m_Detector12.ShowWindow(SW_SHOW);

		m_CurrentDetectorAverage7.ShowWindow(SW_SHOW);
		m_MaximumDetectorAverage7.ShowWindow(SW_SHOW);
		m_CurrentDetectorAverage8.ShowWindow(SW_SHOW);
		m_MaximumDetectorAverage8.ShowWindow(SW_SHOW);
		m_CurrentDetectorAverage9.ShowWindow(SW_SHOW);
		m_MaximumDetectorAverage9.ShowWindow(SW_SHOW);
		m_CurrentDetectorAverage10.ShowWindow(SW_SHOW);
		m_MaximumDetectorAverage10.ShowWindow(SW_SHOW);
		m_CurrentDetectorAverage11.ShowWindow(SW_SHOW);
		m_MaximumDetectorAverage11.ShowWindow(SW_SHOW);
		m_CurrentDetectorAverage12.ShowWindow(SW_SHOW);
		m_MaximumDetectorAverage12.ShowWindow(SW_SHOW);

		SetDlgItemText(IDC_Detector1, _T("Detector 1"));//front
		SetDlgItemText(IDC_Detector7, _T("Detector 2"));
		SetDlgItemText(IDC_Detector2, _T("Detector 3"));
		SetDlgItemText(IDC_Detector8, _T("Detector 4"));
		SetDlgItemText(IDC_Detector3, _T("Detector 5"));
		SetDlgItemText(IDC_Detector9, _T("Detector 6"));
		SetDlgItemText(IDC_Detector4, _T("Detector 7"));
		SetDlgItemText(IDC_Detector10, _T("Detector 8"));
		SetDlgItemText(IDC_Detector5, _T("Detector 9"));
		SetDlgItemText(IDC_Detector11, _T("Detector 10"));
		SetDlgItemText(IDC_Detector6, _T("Detector 11"));
		SetDlgItemText(IDC_Detector12, _T("Detector 12"));//back
	}
	if (vLocalConfigurationData->vNumberOfXRayDetectors < 6)
	{
		m_CurrentDetectorAverage6.ShowWindow(SW_HIDE);
		m_MaximumDetectorAverage6.ShowWindow(SW_HIDE);
		m_Detector6.ShowWindow(SW_HIDE);
	}
	if (vLocalConfigurationData->vNumberOfXRayDetectors < 5)
	{
		m_CurrentDetectorAverage5.ShowWindow(SW_HIDE);
		m_MaximumDetectorAverage5.ShowWindow(SW_HIDE);
		m_Detector5.ShowWindow(SW_HIDE);
	}

	if (TempProduct)
	{
		TempProduct->SetProductName("~AlignTube");

		TempProduct->vProductImageHeightTop = (float)(vLocalConfigurationData->vDetectorLength * vLocalConfigurationData->vNumberOfXRayDetectors);
		//if (vLocalConfigurationData->vUseMetric)
		//	TempProduct->vProductImageHeightTop = (WORD)(TempProduct->vProductImageHeightTop * 25.4);

		double TempDelay = 2;
		//if (vLocalConfigurationData->vUseMetric)
		//	TempDelay = TempDelay * 25.4;
		TempProduct->SetProductBodyTriggerToImageBeltPositionOffset((float)TempDelay);
		if (TempProduct->vProductImageWidth < 2)
			TempProduct->SetProductImageWidth((float)TempDelay * 2);
		TempProduct->SetProductLockOutWidth((float)TempDelay);

		TempProduct->vEndOfLineTimeOut = (float)(16);
		//if (vLocalConfigurationData->vUseMetric)
		//	TempProduct->vEndOfLineTimeOut = (float)(16 * 25.4);
		TempProduct->SetEndOfLineTimeOut(TempProduct->vEndOfLineTimeOut);

		TempProduct->vEdgeLocationBottom = (float)(.5);
		TempProduct->SetEdgeLocationBottom(TempProduct->vEdgeLocationBottom);
		TempProduct->vEdgeLocationHeight = (float)(1);
		TempProduct->SetEdgeLocationHeight(TempProduct->vEdgeLocationHeight);

		if ((TempProduct->vXRaySourceVoltage * 6.0 > (float)(vMainWindowPointer->GetDefaultAlignTubeVoltage() + 5)) || 
			(TempProduct->vXRaySourceVoltage * 6.0 < (float)(vMainWindowPointer->GetDefaultAlignTubeVoltage() - 5)) || (TempMadeANewProduct))
			TempProduct->vXRaySourceVoltage = (float)(vMainWindowPointer->GetDefaultAlignTubeVoltage() / 6.0);

		if ((TempProduct->vXRaySourceCurrent > (float)(vMainWindowPointer->GetDefaultAlignTubeCurrent(vLocalConfigurationData->vXRaySourceFilter) + 0.5)) || 
			(TempProduct->vXRaySourceCurrent < (float)(vMainWindowPointer->GetDefaultAlignTubeCurrent(vLocalConfigurationData->vXRaySourceFilter) - 0.5)) || (TempMadeANewProduct))
			TempProduct->vXRaySourceCurrent = (float)vMainWindowPointer->GetDefaultAlignTubeCurrent(vLocalConfigurationData->vXRaySourceFilter);

			if (vLocalConfigurationData->vMaximumCurrent < TempProduct->vXRaySourceCurrent)
				TempProduct->vXRaySourceCurrent = (float)vLocalConfigurationData->vMaximumCurrent;

			if (vLocalConfigurationData->vMaximumVoltage < TempProduct->vXRaySourceVoltage)
				TempProduct->vXRaySourceVoltage = (float)vLocalConfigurationData->vMaximumVoltage;

		TempProduct->vXRayIntegrationTime = 3750; //1.5 ms
		//TempProduct->vXRayIntegrationTimeAtEdge = TempProduct->vXRayIntegrationTime;
		TempProduct->SetOverScanFactor(0);
		TempProduct->vBodyTrigger = 1;
		TempProduct->SetEjectorBeltPositionOffset(0,(float)40);
		TempProduct->SetEjectorBeltPositionOffset(1,(float)39);
		TempProduct->SetEjectorBeltPositionOffset(2,(float)38);
		//if (vLocalConfigurationData->vUseMetric)
		//	TempProduct->SetEjectorBeltPositionOffset(0,(float)(40 * 25.4));
		TempProduct->vNotes = "Auto Product for Tube Alignment";
		TempProduct->CalculateEndOfLineTimeOut();

		if (TempMadeANewProduct)
			vMainWindowPointer->AddProduct(((CProduct *)TempProduct));

		vMainWindowPointer->SetupProduct(TempProduct,false);
	}

	ShowInspxItems();

	if (vLocalSystemData->vSystemRunMode == cStoppedSystemMode)
	{
		SetDlgItemText(IDC_SubFunction6Button,_T("Run"));
	}
	else
	{
		SetDlgItemText(IDC_SubFunction6Button,_T("Stop"));
	}
	vMainWindowPointer->SendADCGainsAndOffsets(true);

	if (vGlobalFPGAVersion10Point0OrHigher)
		vMainWindowPointer->SendPreAmpGain(1);//preamp gain of 2 
	else
		vMainWindowPointer->SendPreAmpGain(4);//preamp gain of 8 

	for (BYTE TempLoop = 0; TempLoop < cMaximumNumberOfDetectors12; TempLoop++)
	{
		vLocalSystemData->vCurrentDetectorAverage[TempLoop] = 0;
		vLocalSystemData->vMaximumDetectorAverage[TempLoop] = 0;
	}
	vLocalSystemData->vFrameCount = 0;
	vLocalSystemData->vHaveAlignmentDataToSave = false;

	UpdateButtons();
	UpdateDisplay();

	int TempTimerResult = SetTimer(vUpdateButtonTimerHandle,500,NULL);
	if (!TempTimerResult)
		ReportErrorMessage("Error-Update Timer Failed",cEMailInspx,32000);

	this->SetWindowText(_T("Tube Alignment"));

	vMainWindowPointer->SetFastestSimulatedEncoder(1);

	int TimerResult = SetTimer(vGrabFocusTimerHandle,800,NULL);
	if (!TimerResult)
		ReportErrorMessage("Error-GrabFocus Timer Failed",cEMailInspx, 32000);
	::SetupMenu(vLocalCWndCollection);
}

void CXRayTubeAlignmentDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CXRayTubeAlignmentDialog)
	DDX_Control(pDX, IDC_BackLabel, m_BackLabel);
	DDX_Control(pDX, IDC_MaximumDetectorAverage9, m_MaximumDetectorAverage9);
	DDX_Control(pDX, IDC_MaximumDetectorAverage8, m_MaximumDetectorAverage8);
	DDX_Control(pDX, IDC_MaximumDetectorAverage7, m_MaximumDetectorAverage7);
	DDX_Control(pDX, IDC_MaximumDetectorAverage12, m_MaximumDetectorAverage12);
	DDX_Control(pDX, IDC_MaximumDetectorAverage11, m_MaximumDetectorAverage11);
	DDX_Control(pDX, IDC_MaximumDetectorAverage10, m_MaximumDetectorAverage10);
	DDX_Control(pDX, IDC_CurrentDetectorAverage9, m_CurrentDetectorAverage9);
	DDX_Control(pDX, IDC_CurrentDetectorAverage8, m_CurrentDetectorAverage8);
	DDX_Control(pDX, IDC_CurrentDetectorAverage7, m_CurrentDetectorAverage7);
	DDX_Control(pDX, IDC_CurrentDetectorAverage12, m_CurrentDetectorAverage12);
	DDX_Control(pDX, IDC_CurrentDetectorAverage11, m_CurrentDetectorAverage11);
	DDX_Control(pDX, IDC_CurrentDetectorAverage10, m_CurrentDetectorAverage10);
	DDX_Control(pDX, IDC_Detector9, m_Detector9);
	DDX_Control(pDX, IDC_Detector8, m_Detector8);
	DDX_Control(pDX, IDC_Detector7, m_Detector7);
	DDX_Control(pDX, IDC_Detector12, m_Detector12);
	DDX_Control(pDX, IDC_Detector11, m_Detector11);
	DDX_Control(pDX, IDC_Detector10, m_Detector10);
	DDX_Control(pDX, IDC_MaximumDetectorAverage6, m_MaximumDetectorAverage6);
	DDX_Control(pDX, IDC_MaximumDetectorAverage5, m_MaximumDetectorAverage5);
	DDX_Control(pDX, IDC_Detector6, m_Detector6);
	DDX_Control(pDX, IDC_Detector5, m_Detector5);
	DDX_Control(pDX, IDC_CurrentDetectorAverage5, m_CurrentDetectorAverage5);
	DDX_Control(pDX, IDC_CurrentDetectorAverage6, m_CurrentDetectorAverage6);
	DDX_Control(pDX, IDC_VoltageButton, m_VoltageButton);
	DDX_Control(pDX, IDC_CurrentButton, m_CurrentButton);
	DDX_Control(pDX, IDC_SubFunction8Button, m_SubFunction8Button);
	DDX_Control(pDX, IDC_SubFunction4Display, m_SubFunction4Display);
	DDX_Control(pDX, IDC_Function5Button, m_Function5Button);
	DDX_Control(pDX, IDC_Function4Button, m_Function4Button);
	DDX_Control(pDX, IDC_SubFunction4Button, m_SubFunction4Button);
	DDX_Control(pDX, IDC_Calibrating, m_Calibrating);
	DDX_Control(pDX, IDC_XRaysOn, m_XRaysOn);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_FrontLabel, m_FrontLabel);
	DDX_Control(pDX, IDC_Maximum, m_Maximum);
	DDX_Control(pDX, IDC_Current, m_Current);
	DDX_Control(pDX, IDC_Black, m_Black);
	DDX_Control(pDX, IDC_CurrentDetectorAverage1, m_CurrentDetectorAverage1);
	DDX_Control(pDX, IDC_CurrentDetectorAverage2, m_CurrentDetectorAverage2);
	DDX_Control(pDX, IDC_CurrentDetectorAverage3, m_CurrentDetectorAverage3);
	DDX_Control(pDX, IDC_CurrentDetectorAverage4, m_CurrentDetectorAverage4);
	DDX_Control(pDX, IDC_Detector1, m_Detector1);
	DDX_Control(pDX, IDC_Detector2, m_Detector2);
	DDX_Control(pDX, IDC_Detector3, m_Detector3);
	DDX_Control(pDX, IDC_Detector4, m_Detector4);
	DDX_Control(pDX, IDC_DialogTitleStaticText1, m_DialogTitleStaticText);
	DDX_Control(pDX, IDC_Function1Button, m_Function1Button);
	DDX_Control(pDX, IDC_Function2Button, m_Function2Button);
	DDX_Control(pDX, IDC_Function3Button, m_Function3Button);
	DDX_Control(pDX, IDC_Function6Button, m_Function6Button);
	DDX_Control(pDX, IDC_Function6Display, m_Function6Display);
	DDX_Control(pDX, IDC_MaximumDetectorAverage1, m_MaximumDetectorAverage1);
	DDX_Control(pDX, IDC_MaximumDetectorAverage2, m_MaximumDetectorAverage2);
	DDX_Control(pDX, IDC_MaximumDetectorAverage3, m_MaximumDetectorAverage3);
	DDX_Control(pDX, IDC_MaximumDetectorAverage4, m_MaximumDetectorAverage4);
	DDX_Control(pDX, IDC_SubFunction8DisplayA, m_SubFunction8DisplayA);
	DDX_Control(pDX, IDC_White, m_White);
}


BEGIN_MESSAGE_MAP(CXRayTubeAlignmentDialog, CDialog)
	//{{AFX_MSG_MAP(CXRayTubeAlignmentDialog)
	ON_BN_CLICKED(IDC_Function1Button, OnFunction1Button)
	ON_BN_CLICKED(IDC_Function2Button, OnFunction2Button)
	ON_BN_CLICKED(IDC_Function3Button, OnFunction3Button)
	ON_BN_CLICKED(IDC_Function4Button, OnFunction4Button)
	ON_BN_CLICKED(IDC_Function5Button, OnFunction5Button)
	ON_WM_CTLCOLOR()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_SubFunction4Button, OnSubFunction4Button)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_SubFunction8Button, OnSubFunction8Button)
	ON_BN_CLICKED(IDC_Function6Button, OnFunction6Button)
	ON_BN_CLICKED(IDC_CurrentButton, OnCurrentButton)
	ON_BN_CLICKED(IDC_VoltageButton, OnVoltageButton)
	//}}AFX_MSG_MAP
	ON_STN_CLICKED(IDC_DialogTitleStaticText1, &CXRayTubeAlignmentDialog::OnDialogtitlestatictext1)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXRayTubeAlignmentDialog message handlers

void CXRayTubeAlignmentDialog::OnFunction1Button() 
{
	PrepareToExit();

	CDialog::EndDialog(10);
}

void CXRayTubeAlignmentDialog::OnFunction2Button() 
{
	// Clear Maximum Values
	for (BYTE TempLoop = 0; TempLoop < cMaximumNumberOfDetectors12; TempLoop++)
	{
		vLocalSystemData->vMaximumDetectorAverage[TempLoop] = 0;
	}
	vLocalSystemData->vHaveAlignmentDataToSave = false;
	vLocalSystemData->vFrameCount = 0;
	SetDlgItemText(IDC_MainStatusLine,dtoa(vLocalSystemData->vFrameCount,0));

	UpdateDisplay();
}

void CXRayTubeAlignmentDialog::OnFunction3Button() 
{
	// run or stop
	vMainWindowPointer->SetFastestSimulatedEncoder(1);

	if ((!vLocalSystemData->vXRayOnNotice) && 
		(vLocalSystemData->vSystemRunMode == cStoppedSystemMode))
	{
		if (vMainWindowPointer->InterlocksOK())
		{
			CString TempText = "Calibrate";
			m_Calibrating.ShowWindow(SW_SHOW);
			SetDlgItemText(IDC_Calibrating,TempText);
			//SetDlgItemText(IDC_Function3Button,"Wait Calibrating");
			vMainWindowPointer->SendADCGainsAndOffsets(true);

			vLocalSystemData->vApplyGainAndOffsetDuringCalibration = false;
			vLocalSystemData->vModeToRunAfterCalibrateOffset = cAlignTubeMode;
			//this will calibrate detectors, then do measure Align Tube
			vLocalSystemData->vCalibrateErrorCount = 0;
			vLocalSystemData->vCalibrateADCAfterDAC = true;
			vLocalSystemData->vAutoCalibrate = true;
			vLocalSystemData->vAutoCalibrateWhileRunning = false;
			vLocalSystemData->vFullDetectorCalibration = false;
			if (vLocalConfigurationData->vUseDACAndADCinDensityCompensation)
				vMainWindowPointer->CalibrateDACDetectorOffset();
			else
				vMainWindowPointer->CalibrateADCDetectorOffset(vLocalSystemData->vAutoCalibrate, false);
			UpdateButtons();
		}
		else
		{
			CNoticeDialog TempNoticeDialog;
			CString TempText("\n\n\nX-Rays not on, Check Key Switch, E-Stop, Interlocks");
			if (PasswordOK(cSuperInspxPassword,false))
				TempText = TempText + "\nInterlocks State: " + WordToHex(vLocalSystemData->vSourceInterLocks);
			TempNoticeDialog.vNoticeText = TempText;
			TempNoticeDialog.vType = cNoticeMessage;
			TempNoticeDialog.DoModal();
		}
	}
	else
	{
		vMainWindowPointer->StopRunning(true);
		m_Calibrating.ShowWindow(SW_HIDE);
/*
		vLocalSystemData->vSystemRunMode = cStoppedSystemMode;

		::PostThreadMessage(vGlobalCallingThreadID,cXRaysOnMessage,0,0);
		vMainWindowPointer->SendXRayPowerOnOffToUController(0);

		vMainWindowPointer->StopImageAcquisition();
		*/
	}
	UpdateButtons();
}

void CXRayTubeAlignmentDialog::OnFunction4Button() 
{
	if (m_Function4Button.IsWindowVisible())
	if (vLocalSystemData->vHaveAlignmentDataToSave)
	{
		vSavedData = true;
		vLocalSystemData->vHaveAlignmentDataToSave = false;
		UpdateButtons();

		CString TempText("");
		for (WORD TempLoop = 0; TempLoop < vLocalConfigurationData->vNumberOfXRayDetectors; TempLoop++)
		{
			TempText = "\nDetector " + dtoa(TempLoop + 1,0) + "    " +
				dtoa(vLocalSystemData->vCurrentDetectorAverage[TempLoop],1) + "    " +
				dtoa(vLocalSystemData->vMaximumDetectorAverage[TempLoop],1) + TempText;
		}

		CString TempPostText = dtoa(vLocalSystemData->vRampXRayCurrentAmount,2) + "mA,  " +
			dtoa(vLocalSystemData->vRampXRayVoltageAmount * 6,2) + "kV";
		if (vLocalSystemData->vTemporarilyChangeSource)
			TempPostText = "**" + TempPostText;
		TempPostText = "\n" + TempPostText;

		ReportErrorMessage("X-Ray Tube Alignment Data for S/N: " + vGlobalLifeTimeCounters->vXRayTubeSerialNumber + 
			"\nDetector        Cur    Max" + TempText + TempPostText, cEMailInspx, 32003);

		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\n\nAlignment Data saved in Service Log";
		TempNoticeDialog.vType = cInformationMessage;
		TempNoticeDialog.vAutoClose = 1000;
		TempNoticeDialog.DoModal();
	}
	else
	{
		CNoticeDialog TempNoticeDialog;
		CString TempText("\n\n\nNo Data Taken To Save");
		TempNoticeDialog.vNoticeText = TempText;
		TempNoticeDialog.vType = cNoticeMessage;
		TempNoticeDialog.DoModal();
	}
	
}

void CXRayTubeAlignmentDialog::OnFunction5Button() 
{
	//if (m_Function5Button.IsWindowVisible())
	if (vGlobalLifeTimeCounters->vServiceRecordLog.GetLength() > 10)
	{
		CHelpDialog * IHelpDialog;
		IHelpDialog = new(CHelpDialog);
		IHelpDialog->vHelpContext = 16;
		IHelpDialog->vHelpType = 0;
		IHelpDialog->vWindowType = cTextWindow;
		//IHelpDialog->vLocalProductCollection = vLocalProductCollection;
		IHelpDialog->vLocalSystemData = vLocalSystemData;
		IHelpDialog->vMainWindowPointer = vMainWindowPointer;
		IHelpDialog->vLocalConfigurationData = vLocalConfigurationData;
		IHelpDialog->vProductPointer = vGlobalCurrentProduct;
		IHelpDialog->vTextString = vGlobalLifeTimeCounters->vServiceRecordLog;
		IHelpDialog->vTitleString = "Service Record Log for " + 
			vLocalConfigurationData->vScanTracID;
		int TempResult = IHelpDialog->DoModal();
	}
	else
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\n\nNo Entries in Service Record Log";
		TempNoticeDialog.vType = cInformationMessage;
		TempNoticeDialog.DoModal();
	}
}

void CXRayTubeAlignmentDialog::OnSubFunction4Button() 
{
	if (m_SubFunction4Button.IsWindowVisible())
	{
		// Toggle X-Ray Source Aluminum Filter In System
		vLocalConfigurationData->vXRaySourceFilter = !vLocalConfigurationData->vXRaySourceFilter;

		vGlobalCurrentProduct->vXRaySourceVoltage = (float)(vMainWindowPointer->GetDefaultAlignTubeVoltage() / 6.0);
		vGlobalCurrentProduct->vXRaySourceCurrent = (float)(vMainWindowPointer->GetDefaultAlignTubeCurrent(vLocalConfigurationData->vXRaySourceFilter));

		vLocalSystemData->vRampXRayCurrentAmount = vGlobalCurrentProduct->vXRaySourceCurrent;
		vLocalSystemData->vRampXRayVoltageAmount = vGlobalCurrentProduct->vXRaySourceVoltage;
		if (vLocalSystemData->vXRayOnNotice)
		{
			vMainWindowPointer->SendSourceSettingsToUController(vLocalSystemData->vRampXRayVoltageAmount,vLocalSystemData->vRampXRayCurrentAmount);
			//::PostThreadMessage(vGlobalCallingThreadID,cRampingSourceMessage,0,0);
		}
		UpdateButtons();
	}
}

void CXRayTubeAlignmentDialog::OnSubFunction8Button() 
{
	//Service Log Comment
	if (m_SubFunction8Button.IsWindowVisible())
	{
		CAlphaEntryDialog IAlphaEntryDialog;  
		IAlphaEntryDialog.vMainWindowPointer = vMainWindowPointer;
		IAlphaEntryDialog.vEditString = "Enter Comment Here";
		//Set dialog box data titles and number value
		IAlphaEntryDialog.vAllowMoreThan32Characters = true;
		IAlphaEntryDialog.vAllowPeriod = true;
		//IAlphaEntryDialog.vAllowComma = true; //not available with vAllowPeriod
		IAlphaEntryDialog.vAllowDashes = true;
		IAlphaEntryDialog.vAllowParenthesis = true;
		IAlphaEntryDialog.vAllowSlashes = true;
		IAlphaEntryDialog.vAllowApproximate = true;

		IAlphaEntryDialog.m_DialogTitleStaticText2 = "";
		IAlphaEntryDialog.vLocalSystemData = vLocalSystemData;

		IAlphaEntryDialog.vEditString = "";
		IAlphaEntryDialog.m_DialogTitleStaticText = "Enter Service Log Comment";
		//Pass control to dialog box and display
		int nResponse = IAlphaEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			ReportErrorMessage(" Service Log Comment: " + IAlphaEntryDialog.vEditString,
				cEMailInspx, 32003);
		}
		else
		if (nResponse == 10)
		{
			//Main Menu button pressed
			Sleep(1); //is the equivelent of a yeild statement;
			CDialog::EndDialog(10);
		}
	}
}

BOOL CXRayTubeAlignmentDialog::PreTranslateMessage(MSG* pMsg) 
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
	if (pMsg->message == cOffsetCalibrationNextStep)
	{
		CString TempText;
		TempText.LoadString(IDS_Wait);
		m_Calibrating.ShowWindow(SW_SHOW);
		if (vLocalSystemData->vCalibrateUsingDAC)
			SetDlgItemText(IDC_Calibrating,TempText + " " + 
				dtoa(vLocalSystemData->vCalibrationImageNumber ,0));
		else
			SetDlgItemText(IDC_Calibrating,TempText + " " + 
				dtoa(vLocalSystemData->vCalibrationImageNumber + 12 ,0));
		return true;
	}
	else
	if (pMsg->message == cNewImageReadyToDisplay)
	{
		//newimage newframe to display on screen
		if (vLocalSystemData->vLogFile.vLogSerialData)
			vLocalSystemData->vLogFile.WriteToLogFile("Process Align Tube Image",cDebugMessage);

		SetDlgItemText(IDC_MainStatusLine,dtoa(vLocalSystemData->vFrameCount,0));
		vHaveAnImage = true;
		UpdateDisplay();
	}
	if (pMsg->message == WM_KEYDOWN)
	{
		CWnd *TempWindow = NULL;
		if (vGlobalShiftKeyDown)
			PostThreadMessage(vGlobalCallingThreadID,cHiddenKeyPressed, 0, pMsg->wParam);
		else
		switch (pMsg->wParam)
		{
			//case 113: // F2:
			//	OnSubFunction5Button();
			//break;
			//case 114: // F3:
			//	OnSubFunction7Button();
			//break;
			//case 115: // F4:
			//	OnSubFunction6Button();
			//break;
			//case 116: // F5:
			//	OnSubFunction1Button();
			//break;
			case 117: // F6 Key - Function 5:
				OnSubFunction4Button();
			break;
			case 118: // F7 Key - Sub Function 1:
				OnFunction1Button();
				return true;
			break;
			case 119: // F8 Key - Sub Function 2:
				OnSubFunction8Button();
			break;
			//case 120: // F9 Key - Sub Function 3:
			//	OnSubFunction2Button();
			//break;
			//case 122: // F11 Key - Sub Function 4:
			//	OnSubFunction3Button();
			//break;
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
				PostThreadMessage(vGlobalCallingThreadID,cShowHelp, 18, 0);
				{
					TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText1);
					if (TempWindow)
						TempWindow->Invalidate(false);
				}
			break;
			case 27: // Escape
				::PostThreadMessage(vGlobalCallingThreadID,	cStartChatWindowMessage, 0,0);
			break;	
			case c1:
				OnFunction6Button();
			break;
			case cPageUp: // Clear, but do inspx hidden dialog
			{
				if (vGlobalPasswordLevel)
				{
					PasswordOK(cNoPassword, false);
				}
				else
				{
					PasswordOK(cAnyPasswordToEnter, true);
				}
				ShowInspxItems();
				TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText1);
				if (TempWindow)
					TempWindow->Invalidate(false);
			}
			break;
		}
		return true;  //true indicates it has been handled, so do not process
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

HBRUSH CXRayTubeAlignmentDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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

	if (pWnd->GetDlgCtrlID() == IDC_Function3Button)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function3Button, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		//if (vOldXRayOnNotice)
		if ((vLocalSystemData->vXRayOnNotice) || 
			(vLocalSystemData->vSystemRunMode != cStoppedSystemMode))
		{
			pDC->SetAttribDC(pDC->m_hDC);
			return vLocalSystemData->vRedBrush;
		}
		else
			return vLocalSystemData->vGreenBrush;
  }
/*
	if ((pWnd->GetDlgCtrlID() == IDC_Background) ||
		(pWnd->GetDlgCtrlID() == IDC_LineLabel) ||
		(pWnd->GetDlgCtrlID() == IDC_LineLabel13) ||
		(pWnd->GetDlgCtrlID() == IDC_CurrentLabel))
	{
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
  }
	*/
	if (pWnd->GetDlgCtrlID() == IDC_Calibrating)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Calibrating, 7);
		pDC->SetTextColor(cYellow);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vRedBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_XRaysOn)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_XRaysOn, 7);
		pDC->SetTextColor(cYellow);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vRedBrush;
	}

	if (pWnd->GetDlgCtrlID() == IDC_FrontLabel)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_FrontLabel, 5);
	if (pWnd->GetDlgCtrlID() == IDC_BackLabel)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_BackLabel, 5);
	if (pWnd->GetDlgCtrlID() == IDC_Detector1)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Detector1, 5);
	if (pWnd->GetDlgCtrlID() == IDC_Detector2)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Detector2, 5);
	if (pWnd->GetDlgCtrlID() == IDC_Detector3)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Detector3, 5);
	if (pWnd->GetDlgCtrlID() == IDC_Detector4)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Detector4, 5);
	if (pWnd->GetDlgCtrlID() == IDC_Detector5)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Detector5, 5);
	if (pWnd->GetDlgCtrlID() == IDC_Detector6)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Detector6, 5);
	if (pWnd->GetDlgCtrlID() == IDC_Detector7)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Detector7, 5);
	if (pWnd->GetDlgCtrlID() == IDC_Detector8)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Detector8, 5);
	if (pWnd->GetDlgCtrlID() == IDC_Detector9)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Detector9, 5);
	if (pWnd->GetDlgCtrlID() == IDC_Detector10)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Detector10, 5);
	if (pWnd->GetDlgCtrlID() == IDC_Detector11)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Detector11, 5);
	if (pWnd->GetDlgCtrlID() == IDC_Detector12)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Detector12, 5);
	if (pWnd->GetDlgCtrlID() == IDC_White)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_White, 5);
	if (pWnd->GetDlgCtrlID() == IDC_Black)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Black, 5);
	if (pWnd->GetDlgCtrlID() == IDC_Current)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Current, 5);
	if (pWnd->GetDlgCtrlID() == IDC_Maximum)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Maximum, 5);
	if (pWnd->GetDlgCtrlID() == IDC_CurrentDetectorAverage1)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_CurrentDetectorAverage1, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBlue);
		//return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_CurrentDetectorAverage2) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_CurrentDetectorAverage2, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBlue);
		//return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_CurrentDetectorAverage3) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_CurrentDetectorAverage3, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBlue);
		//return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_CurrentDetectorAverage4) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_CurrentDetectorAverage4, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBlue);
		//return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_CurrentDetectorAverage5) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_CurrentDetectorAverage5, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBlue);
		//return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_CurrentDetectorAverage6) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_CurrentDetectorAverage6, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBlue);
		//return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_CurrentDetectorAverage7) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_CurrentDetectorAverage7, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBlue);
		//return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_CurrentDetectorAverage8) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_CurrentDetectorAverage8, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBlue);
		//return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_CurrentDetectorAverage9) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_CurrentDetectorAverage9, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBlue);
		//return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_CurrentDetectorAverage10)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_CurrentDetectorAverage10, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBlue);
		//return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_CurrentDetectorAverage11)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_CurrentDetectorAverage11, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBlue);
		//return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_CurrentDetectorAverage12)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_CurrentDetectorAverage12, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBlue);
		//return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_MaximumDetectorAverage1) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_MaximumDetectorAverage1, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(0x0000B000);
		//return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_MaximumDetectorAverage2) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_MaximumDetectorAverage2, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(0x0000B000);
		//return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_MaximumDetectorAverage3) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_MaximumDetectorAverage3, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(0x0000B000);
		//return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_MaximumDetectorAverage4) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_MaximumDetectorAverage4, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(0x0000B000);
		//return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_MaximumDetectorAverage5) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_MaximumDetectorAverage5, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(0x0000B000);
		//return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_MaximumDetectorAverage6) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_MaximumDetectorAverage6, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(0x0000B000);
		//return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_MaximumDetectorAverage7) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_MaximumDetectorAverage7, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(0x0000B000);
		//return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_MaximumDetectorAverage8) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_MaximumDetectorAverage8, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(0x0000B000);
		//return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_MaximumDetectorAverage9) 
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_MaximumDetectorAverage9, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(0x0000B000);
		//return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_MaximumDetectorAverage10)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_MaximumDetectorAverage10, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(0x0000B000);
		//return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_MaximumDetectorAverage11)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_MaximumDetectorAverage11, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(0x0000B000);
		//return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_MaximumDetectorAverage12)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_MaximumDetectorAverage12, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(0x0000B000);
		//return vGlobalButtonColorBrush;
	}
	
	if (pWnd->GetDlgCtrlID() == IDC_CurrentButton)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_CurrentButton, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_VoltageButton)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_VoltageButton, 5);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Function6Display)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Function6Display, 5);
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction8DisplayA)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction8DisplayA, 5);
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction4Display)
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_SubFunction4Display, 5);
	
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
	if (pWnd->GetDlgCtrlID() == IDC_Function6Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function6Button, 5);  
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
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction8Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction8Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	return hbr;
}

void CXRayTubeAlignmentDialog::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
  if (nIDEvent == vUpdateButtonTimerHandle)
  {
		if ((vLocalSystemData->vRampXRayCurrentAmount != vOldRampXRayCurrentAmount) ||
			(vLocalSystemData->vRampXRayVoltageAmount != vOldRampXRayVoltageAmount))
		{
			vOldRampXRayVoltageAmount = vLocalSystemData->vRampXRayVoltageAmount;
			vOldRampXRayCurrentAmount = vLocalSystemData->vRampXRayCurrentAmount;
			UpdateButtons();
		}

		if (vLocalSystemData->vSystemRunMode != vOldSystemRunMode)
		{
			vOldSystemRunMode = vLocalSystemData->vSystemRunMode;
			if (vOldSystemRunMode == cStoppedSystemMode)
			{
				m_XRaysOn.ShowWindow(SW_HIDE);
				m_Calibrating.ShowWindow(SW_HIDE);
			}
			UpdateButtons();
		}

		if (vOldXRayOnNotice != vLocalSystemData->vXRayOnNotice)
		{
			vOldXRayOnNotice = vLocalSystemData->vXRayOnNotice;
			if (vOldXRayOnNotice)
			{
				m_XRaysOn.ShowWindow(SW_SHOW);
				m_Calibrating.ShowWindow(SW_SHOW);
				CString TempText;
				TempText.LoadString(IDS_XRAYSON);
				SetDlgItemText(IDC_Calibrating,TempText);
			}
			else
			{
				m_XRaysOn.ShowWindow(SW_HIDE);
				m_Calibrating.ShowWindow(SW_HIDE);
			}
			UpdateButtons();
		}
	}
	else
  if (nIDEvent == vGrabFocusTimerHandle)  
  {
		KillTimer(vGrabFocusTimerHandle);
		CProduct *TempProduct = vMainWindowPointer->GetProductByName("~AlignTube");

		if (TempProduct)
			vMainWindowPointer->SetupProduct(TempProduct,false);

		SetForegroundWindow();
	}

	CDialog::OnTimer(nIDEvent);
}

void CXRayTubeAlignmentDialog::UpdateDisplay()
{
	if (vLocalConfigurationData->vNumberOfXRayDetectors == 12)
	{
		SetDlgItemText(IDC_CurrentDetectorAverage1,dtoa(vLocalSystemData->vCurrentDetectorAverage[0],0));
		SetDlgItemText(IDC_CurrentDetectorAverage2,dtoa(vLocalSystemData->vCurrentDetectorAverage[2],0));
		SetDlgItemText(IDC_CurrentDetectorAverage3,dtoa(vLocalSystemData->vCurrentDetectorAverage[4],0));
		SetDlgItemText(IDC_CurrentDetectorAverage4,dtoa(vLocalSystemData->vCurrentDetectorAverage[6],0));
		SetDlgItemText(IDC_CurrentDetectorAverage5,dtoa(vLocalSystemData->vCurrentDetectorAverage[8],0));
		SetDlgItemText(IDC_CurrentDetectorAverage6,dtoa(vLocalSystemData->vCurrentDetectorAverage[10],0));

		SetDlgItemText(IDC_MaximumDetectorAverage1,dtoa(vLocalSystemData->vMaximumDetectorAverage[0],0));
		SetDlgItemText(IDC_MaximumDetectorAverage2,dtoa(vLocalSystemData->vMaximumDetectorAverage[2],0));
		SetDlgItemText(IDC_MaximumDetectorAverage3,dtoa(vLocalSystemData->vMaximumDetectorAverage[4],0));
		SetDlgItemText(IDC_MaximumDetectorAverage4,dtoa(vLocalSystemData->vMaximumDetectorAverage[6],0));
		SetDlgItemText(IDC_MaximumDetectorAverage5,dtoa(vLocalSystemData->vMaximumDetectorAverage[8],0));
		SetDlgItemText(IDC_MaximumDetectorAverage6,dtoa(vLocalSystemData->vMaximumDetectorAverage[10],0));

		SetDlgItemText(IDC_CurrentDetectorAverage7,dtoa(vLocalSystemData->vCurrentDetectorAverage[1],0));
		SetDlgItemText(IDC_CurrentDetectorAverage8,dtoa(vLocalSystemData->vCurrentDetectorAverage[3],0));
		SetDlgItemText(IDC_CurrentDetectorAverage9,dtoa(vLocalSystemData->vCurrentDetectorAverage[5],0));
		SetDlgItemText(IDC_CurrentDetectorAverage10,dtoa(vLocalSystemData->vCurrentDetectorAverage[7],0));
		SetDlgItemText(IDC_CurrentDetectorAverage11,dtoa(vLocalSystemData->vCurrentDetectorAverage[9],0));
		SetDlgItemText(IDC_CurrentDetectorAverage12,dtoa(vLocalSystemData->vCurrentDetectorAverage[11],0));

		SetDlgItemText(IDC_MaximumDetectorAverage7,dtoa(vLocalSystemData->vMaximumDetectorAverage[1],0));
		SetDlgItemText(IDC_MaximumDetectorAverage8,dtoa(vLocalSystemData->vMaximumDetectorAverage[3],0));
		SetDlgItemText(IDC_MaximumDetectorAverage9,dtoa(vLocalSystemData->vMaximumDetectorAverage[5],0));
		SetDlgItemText(IDC_MaximumDetectorAverage10,dtoa(vLocalSystemData->vMaximumDetectorAverage[7],0));
		SetDlgItemText(IDC_MaximumDetectorAverage11,dtoa(vLocalSystemData->vMaximumDetectorAverage[9],0));
		SetDlgItemText(IDC_MaximumDetectorAverage12,dtoa(vLocalSystemData->vMaximumDetectorAverage[11],0));
	}
	else
	if (vLocalConfigurationData->vNumberOfXRayDetectors == 9)
	{
		SetDlgItemText(IDC_CurrentDetectorAverage1,dtoa(vLocalSystemData->vCurrentDetectorAverage[0],0));
		SetDlgItemText(IDC_CurrentDetectorAverage2,dtoa(vLocalSystemData->vCurrentDetectorAverage[2],0));
		SetDlgItemText(IDC_CurrentDetectorAverage3,dtoa(vLocalSystemData->vCurrentDetectorAverage[4],0));
		SetDlgItemText(IDC_CurrentDetectorAverage4,dtoa(vLocalSystemData->vCurrentDetectorAverage[6],0));
		SetDlgItemText(IDC_CurrentDetectorAverage5,dtoa(vLocalSystemData->vCurrentDetectorAverage[8],0));

		SetDlgItemText(IDC_MaximumDetectorAverage1,dtoa(vLocalSystemData->vMaximumDetectorAverage[0],0));
		SetDlgItemText(IDC_MaximumDetectorAverage2,dtoa(vLocalSystemData->vMaximumDetectorAverage[2],0));
		SetDlgItemText(IDC_MaximumDetectorAverage3,dtoa(vLocalSystemData->vMaximumDetectorAverage[4],0));
		SetDlgItemText(IDC_MaximumDetectorAverage4,dtoa(vLocalSystemData->vMaximumDetectorAverage[6],0));
		SetDlgItemText(IDC_MaximumDetectorAverage5,dtoa(vLocalSystemData->vMaximumDetectorAverage[8],0));

		SetDlgItemText(IDC_CurrentDetectorAverage7,dtoa(vLocalSystemData->vCurrentDetectorAverage[1],0));
		SetDlgItemText(IDC_CurrentDetectorAverage8,dtoa(vLocalSystemData->vCurrentDetectorAverage[3],0));
		SetDlgItemText(IDC_CurrentDetectorAverage9,dtoa(vLocalSystemData->vCurrentDetectorAverage[5],0));
		SetDlgItemText(IDC_CurrentDetectorAverage10,dtoa(vLocalSystemData->vCurrentDetectorAverage[7],0));

		SetDlgItemText(IDC_MaximumDetectorAverage7,dtoa(vLocalSystemData->vMaximumDetectorAverage[1],0));
		SetDlgItemText(IDC_MaximumDetectorAverage8,dtoa(vLocalSystemData->vMaximumDetectorAverage[3],0));
		SetDlgItemText(IDC_MaximumDetectorAverage9,dtoa(vLocalSystemData->vMaximumDetectorAverage[5],0));
		SetDlgItemText(IDC_MaximumDetectorAverage10,dtoa(vLocalSystemData->vMaximumDetectorAverage[7],0));
	}
	else
	{
		SetDlgItemText(IDC_CurrentDetectorAverage1,dtoa(vLocalSystemData->vCurrentDetectorAverage[0],0));
		SetDlgItemText(IDC_CurrentDetectorAverage2,dtoa(vLocalSystemData->vCurrentDetectorAverage[1],0));
		SetDlgItemText(IDC_CurrentDetectorAverage3,dtoa(vLocalSystemData->vCurrentDetectorAverage[2],0));
		SetDlgItemText(IDC_CurrentDetectorAverage4,dtoa(vLocalSystemData->vCurrentDetectorAverage[3],0));
		SetDlgItemText(IDC_CurrentDetectorAverage5,dtoa(vLocalSystemData->vCurrentDetectorAverage[4],0));
		SetDlgItemText(IDC_CurrentDetectorAverage6,dtoa(vLocalSystemData->vCurrentDetectorAverage[5],0));

		SetDlgItemText(IDC_MaximumDetectorAverage1,dtoa(vLocalSystemData->vMaximumDetectorAverage[0],0));
		SetDlgItemText(IDC_MaximumDetectorAverage2,dtoa(vLocalSystemData->vMaximumDetectorAverage[1],0));
		SetDlgItemText(IDC_MaximumDetectorAverage3,dtoa(vLocalSystemData->vMaximumDetectorAverage[2],0));
		SetDlgItemText(IDC_MaximumDetectorAverage4,dtoa(vLocalSystemData->vMaximumDetectorAverage[3],0));
		SetDlgItemText(IDC_MaximumDetectorAverage5,dtoa(vLocalSystemData->vMaximumDetectorAverage[4],0));
		SetDlgItemText(IDC_MaximumDetectorAverage6,dtoa(vLocalSystemData->vMaximumDetectorAverage[5],0));
	}

	vLeftSide = 105;
	vRightSide = 425;

	vLeftSide = ResizeXCoor(vLeftSide);
	vRightSide = ResizeXCoor(vRightSide);
	
	//vTop = 56; 
	//vBottom = 326;
	RECT TempRectangle;

	//redraw the image part, but do not erase previous data
	TempRectangle.left = vLeftSide;
	TempRectangle.top = vTop;
	TempRectangle.right = vRightSide + ResizeXCoor(15);
	TempRectangle.bottom = vBottom;
	this->InvalidateRect(&TempRectangle,true);
}

void CXRayTubeAlignmentDialog::UpdateButtons() 
{
	SetDlgItemText(IDC_Function6Display,vGlobalLifeTimeCounters->vXRayTubeSerialNumber);

	SetDlgItemText(IDC_SubFunction8DisplayA,dtoa(vLocalSystemData->vRampXRayCurrentAmount,2) + 
		" mA (" + dtoa(vLocalSystemData->vRampXRayVoltageAmount * 6,2) + " kV)");

	if ((vOldXRayOnNotice) ||
		(vLocalSystemData->vSystemRunMode != cStoppedSystemMode))
	{
		m_XRaysOn.ShowWindow(SW_SHOW);
		SetDlgItemText(IDC_Function3Button, _T("Stop\nX-Rays"));
	}
	else
	{
		m_XRaysOn.ShowWindow(SW_HIDE);
		SetDlgItemText(IDC_Function3Button, _T("Generate\nX-Rays"));
	}

	if ((vGlobalScanTracType == cSoloScanTrac) || (vLocalConfigurationData->vScanTracType == cSoloPlusScanTrac) || (vGlobalScanTracType == cUpSideDownSoloScanTrac))
	if (vLocalConfigurationData->vXRaySourceFilter)
	{
		m_SubFunction4Button.ModifyStyle(NULL,SS_CENTERIMAGE,NULL);
		SetDlgItemText(IDC_SubFunction4Button, _T("No Filter"));
		SetDlgItemText(IDC_SubFunction4Display, _T("2mm Aluminum X-Ray Filter Installed"));
	}
	else
	{
		m_SubFunction4Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		SetDlgItemText(IDC_SubFunction4Button, _T("2mm X-Ray Filter"));
		SetDlgItemText(IDC_SubFunction4Display, _T("No Filter"));
	}
}

void CXRayTubeAlignmentDialog::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	// Draw Diagnostic Data On Screen
	BYTE TempMask = 1;
	BYTE TempLoop = 0;
	WORD TempHeightOffset = 0;

	BYTE TempNumberOfLinesToDraw = vLocalConfigurationData->vNumberOfXRayDetectors;
	
	BYTE TempOffset = 6 - vLocalConfigurationData->vNumberOfXRayDetectors;  //if using less than 6 detectors, must offset
	BYTE TempDetectorNumber = 0;
	WORD TempLoopS = 0;
	int TempDetectorValue = 0;
	int Temp100ResizedY = ResizeYCoor(100);
	int Temp10ResizedX = ResizeYCoor(10);
	int Temp3ResizedX = ResizeXCoor(3);

	vLeftSide = 170;
	vRightSide = 425;
	vTop = 56; 
	vBottom = 326;
	vHeight = vBottom - vTop;
	vWidth = vRightSide - vLeftSide;
	vTraceHeight = 45;
	dc.SelectObject(vDarkBluePen);
	
	if (vLocalConfigurationData->vNumberOfXRayDetectors == 9)
	{
		TempOffset = 0;
		vTraceHeight = 22;
		TempHeightOffset = 71;
	}

	if (vLocalConfigurationData->vNumberOfXRayDetectors == 12)
	{
		TempOffset = 0;
		vTraceHeight = 22;
		TempHeightOffset = 5;
	}

	vLeftSide = ResizeXCoor(vLeftSide);
	vRightSide = ResizeXCoor(vRightSide);
	vTop = ResizeYCoor(vTop); 
	vBottom = ResizeYCoor(vBottom);
	vHeight = ResizeYCoor(vHeight);
	vWidth = ResizeXCoor(vWidth);
	vTraceHeight = ResizeYCoor(vTraceHeight);
	TempHeightOffset = (WORD)ResizeYCoor(TempHeightOffset);
	int TempLineStartX, TempLineStartY, TempLineEndX, TempLineEndY;

	for (TempLoop = 0; TempLoop <= TempNumberOfLinesToDraw; TempLoop++)
	{
		TempLineStartX = vLeftSide + Temp10ResizedX;
		TempLineEndX = vRightSide + 1 + Temp10ResizedX;
		TempLineStartY = vTop + TempHeightOffset + ((TempLoop + TempOffset) * vTraceHeight);
		TempLineEndY = TempLineStartY;
		
		dc.MoveTo(TempLineStartX,TempLineStartY);
		dc.LineTo(TempLineEndX,TempLineEndY); //horizonal lines
	}
	vBottom = TempLineEndY;
	dc.MoveTo(vLeftSide+Temp10ResizedX,vTop + TempHeightOffset + (TempOffset * vTraceHeight));
	dc.LineTo(vLeftSide+Temp10ResizedX,vBottom); //Left line
	dc.MoveTo(vRightSide + 1+Temp10ResizedX,vTop + TempHeightOffset + (TempOffset * vTraceHeight));
	dc.LineTo(vRightSide + 1+Temp10ResizedX,vBottom); //Right line

	dc.SelectObject(vLocalSystemData->vGreenPen);
	
	if (vHaveAnImage)	//for debug
		int x = 0;

	//Draw Maximum Lines
	for (TempLoop = 0; TempLoop < TempNumberOfLinesToDraw; TempLoop++)
	{
		TempDetectorNumber = TempNumberOfLinesToDraw - TempLoop - 1;
		for (TempLoopS = 0; TempLoopS < Temp3ResizedX * 2; TempLoopS++)
		{
			TempDetectorValue = ResizeXCoor(vLocalSystemData->vMaximumDetectorAverage[TempDetectorNumber]);

			TempLineStartX = vLeftSide + TempLoopS + TempDetectorValue + Temp10ResizedX;
			TempLineEndX = TempLineStartX;
			TempLineStartY = vTop + TempHeightOffset + 3 + ((TempLoop + TempOffset) * vTraceHeight);
			TempLineEndY = vTop + TempHeightOffset - 2 + (((TempLoop + TempOffset) + 1) * vTraceHeight);

			dc.MoveTo(TempLineStartX, TempLineStartY); //division lines up and down
			dc.LineTo(TempLineEndX, TempLineEndY);
		}
	}

	//Draw Current Lines
	dc.SelectObject(vBluePen);
	for (TempLoop = 0; TempLoop < TempNumberOfLinesToDraw; TempLoop++)
	{
		TempDetectorNumber = vLocalConfigurationData->vNumberOfXRayDetectors - TempLoop - 1;
		for (TempLoopS = 0; TempLoopS < Temp3ResizedX; TempLoopS++)
		{
			TempDetectorValue = ResizeXCoor(vLocalSystemData->vCurrentDetectorAverage[TempDetectorNumber]);
			
			TempLineStartX = vLeftSide + TempLoopS + TempDetectorValue + Temp10ResizedX;
			TempLineEndX = TempLineStartX;
			TempLineStartY = vTop + TempHeightOffset + 3 + ((TempLoop + TempOffset) * vTraceHeight);
			TempLineEndY = vTop + TempHeightOffset - 2 + (((TempLoop + TempOffset) + 1) * vTraceHeight);

			dc.MoveTo(TempLineStartX, TempLineStartY); //division lines up and down
			dc.LineTo(TempLineEndX, TempLineEndY);
		}
	}

	//draw image on screen strip
	if ((vLocalSystemData->vITIPCDig->vOriginalImage) && (vLocalSystemData->vITIPCDig->vOriginalImage->vGreyImage))
	if (vHaveAnImage)
	{
		
		double TempScaleFactor = .70 * Temp100ResizedY / 100;
		if (vLocalConfigurationData->vNumberOfXRayDetectors == 12)
			TempScaleFactor = .35 * Temp100ResizedY / 100;
		if (vLocalConfigurationData->vNumberOfXRayDetectors == 9)
			TempScaleFactor = .34 * Temp100ResizedY / 100;

		vLeftSide = ResizeXCoor(105);
		WORD Temp60XResized = (WORD) ResizeXCoor(60);
		
		WORD TempWidth = vLocalSystemData->vITIPCDig->vOriginalBufferSizeY;
		if (TempWidth > Temp60XResized)
			TempWidth = Temp60XResized;

		WORD TempPixelsHigh = (WORD)((double)vLocalSystemData->vITIPCDig->vOriginalBufferSizeX * TempScaleFactor);

		CDC TempCompatableDeviceContext;
		TempCompatableDeviceContext.CreateCompatibleDC(&dc);
		HBITMAP TempBitmapOfReject = CreateCompatibleBitmap ( dc, TempWidth, TempPixelsHigh );
		BITMAPINFO TempBitmapInfo = {0};
		TempBitmapInfo.bmiHeader.biSize = sizeof(TempBitmapInfo.bmiHeader);
		int TempBitmapColorArraySize = TempWidth * TempPixelsHigh * 4;
		int TempIterator = TempBitmapColorArraySize;

		if(0 == GetDIBits(dc, TempBitmapOfReject, 0, 0, NULL, &TempBitmapInfo, DIB_RGB_COLORS))  //get just bitmap info header
		{
			int r = 0;
		}
		TempBitmapInfo.bmiHeader.biBitCount = 32;  
		TempBitmapInfo.bmiHeader.biCompression = BI_RGB;  

		BYTE* TempBitmapColorArray = new BYTE[TempBitmapColorArraySize];
		GetDIBits(dc, TempBitmapOfReject, 0, TempBitmapInfo.bmiHeader.biHeight, (LPVOID)TempBitmapColorArray, &TempBitmapInfo, DIB_RGB_COLORS);
		
		BYTE *TempDataPoint = vLocalSystemData->vITIPCDig->vOriginalImage->vGreyImage;

		TempIterator = TempBitmapColorArraySize - vLocalSystemData->vITIPCDig->vDisplayImageSizeX * 4;
		for (WORD TempLoopY = 0; TempLoopY < TempWidth; TempLoopY++)
		{
			//for (WORD TempLoopX = 0; TempLoopX < vLocalSystemData->vITIPCDig->vOriginalBufferSizeX; TempLoopX++)
			for (WORD TempLoopX = 0; TempLoopX < TempPixelsHigh; TempLoopX++)
			{
				TempDataPoint = vLocalSystemData->vITIPCDig->vOriginalImage->vGreyImage + (WORD)(TempLoopX / TempScaleFactor) 
																						+ (TempLoopY * vLocalSystemData->vITIPCDig->vOriginalBufferSizeX);
				//TempBitmapColorArray[TempIterator] = *TempDataPoint;//blUE
				//TempBitmapColorArray[TempIterator + 1] = *TempDataPoint;//green
				//TempBitmapColorArray[TempIterator + 2] = *TempDataPoint;//red
				//TempBitmapColorArray[TempIterator + 3] = 255;//alpha
				dc.SetPixelV(vLeftSide + TempLoopY,vBottom - TempLoopX, RGB(*TempDataPoint,*TempDataPoint,*TempDataPoint)); //draw pixel in intensity

				TempIterator = TempIterator + 4;
			}
			TempIterator = TempIterator - vLocalSystemData->vITIPCDig->vDisplayImageSizeX * 8;  //multiply by 8, 4 points per pixel, go back two rows and forward one	
		}

		SetDIBits(dc, TempBitmapOfReject, 0, TempBitmapInfo.bmiHeader.biHeight, (LPVOID)TempBitmapColorArray, &TempBitmapInfo, DIB_RGB_COLORS);
		free(TempBitmapColorArray);

		TempCompatableDeviceContext.SelectObject(TempBitmapOfReject);
//		dc.BitBlt(vLeftSide, vBottom - TempPixelsHigh, TempBitmapInfo.bmiHeader.biWidth, TempBitmapInfo.bmiHeader.biHeight, &TempCompatableDeviceContext, 0, 0, SRCCOPY);
		
		DeleteObject(TempCompatableDeviceContext);
		DeleteObject(TempBitmapOfReject);
		
	}
// Do not call CDialog::OnPaint() for painting messages
}

void CXRayTubeAlignmentDialog::PrepareToExit()
{
	if (m_Function4Button.IsWindowVisible())
	{
		vGlobalCurrentProduct->vXRaySourceVoltage = (float)(vLocalSystemData->vRampXRayVoltageAmount);
		vGlobalCurrentProduct->vXRaySourceCurrent = (float)(vLocalSystemData->vRampXRayCurrentAmount);
	}

	if (vLocalSystemData->vHaveAlignmentDataToSave)
	if (!vSavedData)
	{
		CYesNoDialog TempYesNoDialog;
		TempYesNoDialog.vNoticeText = _T("\n\nWrite Tube Alignment Data in Service Log?");
		TempYesNoDialog.vQuestionType = cQuestion;
		int TempResult = TempYesNoDialog.DoModal();
		//if user said OK
		if (TempResult == IDOK)
			OnFunction4Button();
	}

	vHaveAnImage = false;

	if ((vLocalSystemData->vXRayOnNotice) || (vLocalSystemData->vSystemRunMode != cStoppedSystemMode))
		OnFunction3Button();

	vMainWindowPointer->SendADCGainsAndOffsets(false);

	vMainWindowPointer->SendPreAmpGain(vLocalConfigurationData->vPreAmplifierGain);//preamp gain of 2 

	vMainWindowPointer->SetupProduct(vOriginalCurrentProduct,false);
	vMainWindowPointer->TurnSimulatedEncoderOff();
}

void CXRayTubeAlignmentDialog::OnFunction6Button() 
{
	//if (PasswordOK(cTemporaryInspxPassword,false))
	{
		CString TempName("");
		CString TempReason("");
		if (GetName(&TempName))
		if (GetReason(&TempReason))
		{
			// Change Tube Serial Number
			CAlphaEntryDialog IAlphaEntryDialog;  
			IAlphaEntryDialog.vMainWindowPointer = vMainWindowPointer;
			//Set dialog box data titles and number value
			IAlphaEntryDialog.vEditString = vGlobalLifeTimeCounters->vXRayTubeSerialNumber;
			IAlphaEntryDialog.m_DialogTitleStaticText = "Enter X-Ray Tube Serial Number";
			IAlphaEntryDialog.vAllowMoreThan32Characters = true;
			IAlphaEntryDialog.vAllowPeriod = true;

			IAlphaEntryDialog.m_DialogTitleStaticText2 = "";
			IAlphaEntryDialog.vLocalSystemData = vLocalSystemData;
			//Pass control to dialog box and display
			int nResponse = IAlphaEntryDialog.DoModal();
			//dialog box is now closed, if user pressed select do this
			//if user pressed cancel, do nothing
			if (nResponse == IDOK)
			{

				ReportErrorMessage(TempName + " Changed X-Ray Tube Serial Number to " +
					IAlphaEntryDialog.vEditString + " because " +
					TempReason + ".  Previous X-Ray Tube Serial Number was: " +
					vGlobalLifeTimeCounters->vXRayTubeSerialNumber, 
					cEMailInspx, 32003);

				vGlobalLifeTimeCounters->vXRayTubeSerialNumber = IAlphaEntryDialog.vEditString;
				UpdateButtons();
			}
			else
			if (nResponse == 10)
			{
				//Main Menu button pressed
				Sleep(1); //is the equivelent of a yeild statement;
				CDialog::EndDialog(10);
			}
		}
	}
}

bool CXRayTubeAlignmentDialog::GetName(CString *TempName)
{
	//enter name, and reason for clearing
	CAlphaEntryDialog IAlphaEntryDialog;  
	//Set dialog box data titles and number value
	IAlphaEntryDialog.vEditString = "";
	IAlphaEntryDialog.m_DialogTitleStaticText = "Enter your name to authorize this function";
	IAlphaEntryDialog.vAllowMoreThan32Characters = true;
	IAlphaEntryDialog.vMinimumNumberOfCharacters = 10;
	IAlphaEntryDialog.vAllowPeriod = true;

	IAlphaEntryDialog.m_DialogTitleStaticText2 = "";
	IAlphaEntryDialog.vLocalSystemData = vLocalSystemData;
	//Pass control to dialog box and display
	int nResponse = IAlphaEntryDialog.DoModal();
	//dialog box is now closed, if user pressed select do this
	//if user pressed cancel, do nothing
	if (nResponse == IDOK)
	{
		*TempName = IAlphaEntryDialog.vEditString;
		return true;
	}
	else
	{
		return false;
	}
}

bool CXRayTubeAlignmentDialog::GetReason(CString *TempReason)
{
	//enter name, and reason for clearing
	CAlphaEntryDialog IAlphaEntryDialog;  
	IAlphaEntryDialog.vMainWindowPointer = vMainWindowPointer;
	//Set dialog box data titles and number value
	IAlphaEntryDialog.vEditString = "";
	IAlphaEntryDialog.m_DialogTitleStaticText = "Enter reason for executing this function";
	IAlphaEntryDialog.vAllowMoreThan32Characters = true;
	IAlphaEntryDialog.vMinimumNumberOfCharacters = 10;
	IAlphaEntryDialog.vAllowPeriod = true;

	IAlphaEntryDialog.m_DialogTitleStaticText2 = "";
	IAlphaEntryDialog.vLocalSystemData = vLocalSystemData;
	//Pass control to dialog box and display
	int nResponse = IAlphaEntryDialog.DoModal();
	//dialog box is now closed, if user pressed select do this
	//if user pressed cancel, do nothing
	if (nResponse == IDOK)
	{
		*TempReason = IAlphaEntryDialog.vEditString;
		return true;
	}
	else
	{
		if (nResponse == 10)
		{
			//Main Menu button pressed
			Sleep(1); //is the equivelent of a yeild statement;
			CDialog::EndDialog(10);
		}
		return false;
	}
}

void CXRayTubeAlignmentDialog::OnCurrentButton() 
{
	if (m_CurrentButton.IsWindowVisible())
	{
		CString TempCurrentString = dtoa(vLocalSystemData->vRampXRayCurrentAmount,2);
		CNumericEntryDialog INumericEntryDialog;  
		
		//Set dialog box data titles and number value
		INumericEntryDialog.vEditString = TempCurrentString;

		CString TempText;
		CString TempText1("");
		CString TempText2("");
		TempText.LoadString(IDS_Product);
		INumericEntryDialog.m_DialogTitleStaticText1 = "Alignment X-Ray Source Current";
		TempText.LoadString(IDS_OriginalValue);
		INumericEntryDialog.m_DialogTitleStaticText4 = TempText + ": " + INumericEntryDialog.vEditString;

		TempText.LoadString(IDS_MeasuredIn);
		TempText1.LoadString(IDS_MilliAmps);
		INumericEntryDialog.m_UnitsString = TempText + " " + TempText1;

		INumericEntryDialog.m_DialogTitleStaticText4 = "Default setting: .4 mA, Product Setting: " + 
			dtoa(vGlobalCurrentProduct->vXRaySourceCurrent,2);

		TempText.LoadString(IDS_OriginalValue);
		//INumericEntryDialog.m_DialogTitleStaticText4 = TempText + ": " + TempVoltageString;
		TempText2.LoadString(IDS_MeasuredIn);
		TempText1.LoadString(IDS_MilliAmps);
		INumericEntryDialog.m_UnitsString = TempText + ": " + INumericEntryDialog.vEditString + " " + TempText2 + " " + TempText1;

		INumericEntryDialog.vMaxValue = GetMaximumCurrentFromVoltage(vLocalSystemData->vRampXRayVoltageAmount * 6);
		if (INumericEntryDialog.vMaxValue > vLocalConfigurationData->vMaximumCurrent)
			INumericEntryDialog.vMaxValue = vLocalConfigurationData->vMaximumCurrent;
		//INumericEntryDialog.m_DialogTitleStaticText2 = 
		//	"Maximum Current from voltage: " + dtoa(INumericEntryDialog.vMaxValue,2);
		if (vLocalConfigurationData->vHVPSType == c100WMonoBlockHVPS)
		{
			INumericEntryDialog.m_DialogTitleStaticText2 = "Minimum Current for Mono Block is: " + dtoa(vGlobalDXMXRayPort->vMinimumCurrent, 2);
			INumericEntryDialog.vMinValue = vGlobalDXMXRayPort->vMinimumCurrent;
		}
		else
			INumericEntryDialog.m_DialogTitleStaticText2 = "";

		double TempMaxValue = GetMaximumCurrentFromPower(vLocalSystemData->vRampXRayVoltageAmount * 6, vLocalConfigurationData->vMaximumPower);
		if (INumericEntryDialog.vMaxValue > vLocalConfigurationData->vMaximumCurrent)
			INumericEntryDialog.vMaxValue = vLocalConfigurationData->vMaximumCurrent;

		//if (vLocalSystemData->vSystemRunMode == cAlignTubeMode)
		if (INumericEntryDialog.vMaxValue > 2)
			INumericEntryDialog.vMaxValue = 2;

		if (TempMaxValue < INumericEntryDialog.vMaxValue)
			INumericEntryDialog.vMaxValue = TempMaxValue;

		INumericEntryDialog.m_DialogTitleStaticText3 = "Maximum Current: " + dtoa(INumericEntryDialog.vMaxValue,2);
		INumericEntryDialog.vIntegerOnly = false;
		//Pass control to dialog box and display
		int nResponse = INumericEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			TempCurrentString = INumericEntryDialog.vEditString;
			vLocalSystemData->vRampXRayCurrentAmount = ATOF(INumericEntryDialog.vEditString);

			vLocalSystemData->vTemporarilyChangeSource = true;
			if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)
			{
				vMainWindowPointer->SendSourceSettingsToUController(
					vLocalSystemData->vRampXRayVoltageAmount,vLocalSystemData->vRampXRayCurrentAmount);
			}
		}
		else 
		if (nResponse == 10)
		{
			//Main Menu button pressed
			PrepareToExit();
			Sleep(1); //is the equivelent of a yeild statement;
			CDialog::EndDialog(10);
		}
	}
}

void CXRayTubeAlignmentDialog::OnVoltageButton() 
{
	if (m_VoltageButton.IsWindowVisible())
	{
		double TempOriginalVoltage = vLocalSystemData->vRampXRayVoltageAmount;
		CString TempVoltageString = dtoa(vLocalSystemData->vRampXRayVoltageAmount * 6,1); //convert from 0 to 10 volt to 0 to 60 KV
		CNumericEntryDialog INumericEntryDialog;  
		
		//Set dialog box data titles and number value
		INumericEntryDialog.vEditString = TempVoltageString;

		CString TempText("");
		TempText.LoadString(IDS_XRaySourceVoltage);

		TempText.LoadString(IDS_Product);
		//INumericEntryDialog.m_DialogTitleStaticText1 = "X-Ray Source Voltage for " + TempText + ": " + vProductEditingName;
		INumericEntryDialog.m_DialogTitleStaticText1 = "Alignment X-Ray Source Voltage";

		INumericEntryDialog.m_DialogTitleStaticText4 = "Default value: 32 kV, Product Setting: " + 
			dtoa(vGlobalCurrentProduct->vXRaySourceVoltage * 6,2);

		TempText.LoadString(IDS_OriginalValue);
		//INumericEntryDialog.m_DialogTitleStaticText4 = TempText + ": " + TempVoltageString;
		CString TempText1("");
		TempText1.LoadString(IDS_MeasuredInVolts);
		INumericEntryDialog.m_UnitsString = TempText + ": " + TempVoltageString + " " + TempText1;
		INumericEntryDialog.vMinValue = cMinimumXRayVoltage;  
		INumericEntryDialog.vAllowZero = true;
		INumericEntryDialog.vMaxValue = vLocalConfigurationData->vMaximumVoltage;

		INumericEntryDialog.vMaxValue = vLocalConfigurationData->vMaximumVoltage;
		if (INumericEntryDialog.vMaxValue > 40)
			INumericEntryDialog.vMaxValue = 40;

		if (INumericEntryDialog.vMaxValue < cMinimumXRayVoltage)
			INumericEntryDialog.vMaxValue = cMinimumXRayVoltage;
		//INumericEntryDialog.m_DialogTitleStaticText3 = "Maximum Voltage from power limit: " + 
		//	dtoa(TempMaximumVoltage,2);
		INumericEntryDialog.m_DialogTitleStaticText3 = "";
		INumericEntryDialog.m_DialogTitleStaticText2 = "Maximum Voltage: " + dtoa(INumericEntryDialog.vMaxValue,2);

		INumericEntryDialog.vIntegerOnly = false;
		//Pass control to dialog box and display
		int nResponse = INumericEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			TempVoltageString = INumericEntryDialog.vEditString;

			double TempXRayVoltageAmount = ATOF(TempVoltageString);
			double TempMaximumCurrentV = GetMaximumCurrentFromVoltage(TempXRayVoltageAmount);
			double TempMaximumCurrentP = GetMaximumCurrentFromPower(TempXRayVoltageAmount,
				vLocalConfigurationData->vMaximumPower);

			double TempMaximumCurrent = TempMaximumCurrentV;
			if (TempMaximumCurrentP < TempMaximumCurrentV)
				TempMaximumCurrent = TempMaximumCurrentP;

			if (TempMaximumCurrent > vLocalConfigurationData->vMaximumCurrent)
				TempMaximumCurrent = vLocalConfigurationData->vMaximumCurrent;
			if (TempXRayVoltageAmount == 0)
			if (vLocalSystemData->vRampXRayCurrentAmount)
			{ // lower current first with old voltage
				vLocalSystemData->vRampXRayCurrentAmount = 0;
				vMainWindowPointer->SendSourceCurrentToUController(vLocalSystemData->vRampXRayCurrentAmount);
				SetDlgItemText(IDC_Current,dtoa(vLocalSystemData->vRampXRayCurrentAmount,2));
				CNoticeDialog TempNoticeDialog;
				TempNoticeDialog.vNoticeText = "\n\n\nReduced current to maximum for new voltage";
				TempNoticeDialog.vType = cInformationMessage;
				TempNoticeDialog.DoModal();
				Sleep(1);//was 100 2/9/2005
			}
			if (vLocalSystemData->vRampXRayCurrentAmount > TempMaximumCurrent)
			{// lower current first with old voltage
				vMainWindowPointer->SendSourceCurrentToUController(TempMaximumCurrent);
				vLocalSystemData->vRampXRayCurrentAmount = TempMaximumCurrent;
				//SetDlgItemText(IDC_Current,dtoa(vLocalSystemData->vRampXRayVoltageAmount,2));
				CNoticeDialog TempNoticeDialog;
				TempNoticeDialog.vNoticeText = "\n\n\nReduced current to maximum for new voltage";
				TempNoticeDialog.vType = cInformationMessage;
				TempNoticeDialog.DoModal();
				Sleep(1);//was 100 2/9/2005
			}
			vLocalSystemData->vRampXRayVoltageAmount = ATOF(TempVoltageString);
			vLocalSystemData->vRampXRayVoltageAmount = vLocalSystemData->vRampXRayVoltageAmount / 6;

			vLocalSystemData->vTemporarilyChangeSource = true;
			if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)
			{
				vMainWindowPointer->SendSourceSettingsToUController(
					vLocalSystemData->vRampXRayVoltageAmount,vLocalSystemData->vRampXRayCurrentAmount);
			}
		}
		else 
		if (nResponse == 10)
		{
			//Main Menu button pressed
			PrepareToExit();
			Sleep(1); //is the equivelent of a yeild statement;
			CDialog::EndDialog(10);
		}
	}
}

void CXRayTubeAlignmentDialog::ShowInspxItems()
{
	if (PasswordOK(cTemporaryInspxPassword,false))
	{
		m_CurrentButton.ShowWindow(SW_SHOW);
		m_VoltageButton.ShowWindow(SW_SHOW);
	}
	else
	{
		m_CurrentButton.ShowWindow(SW_HIDE);
		m_VoltageButton.ShowWindow(SW_HIDE);
	}
}


void CXRayTubeAlignmentDialog::OnDialogtitlestatictext1()
{
	BYTE TempHiddenKeys = 8;
	if (!PasswordOK(cTemporaryInspxPassword,false))
		TempHiddenKeys = 10;
	PostThreadMessage(vGlobalCallingThreadID, cDisplaySoftKeyMenu, TempHiddenKeys, TempHiddenKeys);
}
