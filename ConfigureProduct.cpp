//ScanTrac Side View Source File
// ConfigureProduct.cpp : implementation file
//

#include "stdafx.h"
#include "ScanTrac.h"
#include "ConfigureProduct.h"
#include "Product.h"
#include "NewEditProductDialog.h"
#include "ScanTracConfigureDialog.h"
#include "EasySelectProductDialog.h"
#include "NoticeDialog.h"
#include "YesNoDialog.h"
#include "SerialPort.h"
#include "AutoSetupImageDisplayDialog.h"
#include "NumericEntryDialog.h"
#include "PasswordEntryDialog.h"
#include "FBWF_Info.h"
#include "HelpDialog.h"
#include "TDAutoSetupImageDisplayDialog.h"
#include "InspectionSize.h"
#include "EditUser.h"
#include "SpellmanDXMXRaySourcePort.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CString vGlobalCurrentUser;
extern CString cLevelName[];

IMPLEMENT_DYNCREATE(CConfigureProduct, CDialog)
/////////////////////////////////////////////////////////////////////////////
// CConfigureProduct dialog

extern CSpellmanDXMXRaySourcePort *vGlobalDXMXRayPort;
extern BYTE vGlobalAllowVoidInspection;
extern HMODULE vGlobalhLibFBWFNetwork;
extern bool vGlobalFPGAVersion10Point0OrHigher;
extern BYTE vGlobalShiftKeyDown;
extern CProduct *vGlobalCurrentProduct;
extern CString vGlobalCurrentDirectory;
extern CSerialPort *vGlobaluCSerialPort;
extern int vGlobalPasswordLevel;
extern unsigned long vGlobalDialogTitleColor;
extern DWORD vGlobalFBWFEnable;

CConfigureProduct::CConfigureProduct(CWnd* pParent /*=NULL*/)
	: CDialog(CConfigureProduct::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConfigureProduct)
	//}}AFX_DATA_INIT
	vMainWindowPointer = NULL;
	vOneSecondTimerHandle = 18997;
	vOldSystemRunMode = 0xFF;
	vLocalCWndCollection.Add(this);
	vLocalCWndCollection.Add(&m_SubFunction2Button);
	vLocalCWndCollection.Add(&m_SubFunction5Button);
	vLocalCWndCollection.Add(&m_SubFunction8Button);
	vLocalCWndCollection.Add(&m_SubFunction7Button);
	vLocalCWndCollection.Add(&m_SubFunction6Button);
	vLocalCWndCollection.Add(&m_Function3Button);
	vLocalCWndCollection.Add(&m_SubFunction3Button);
	vLocalCWndCollection.Add(&m_SubFunction4Button);
	vLocalCWndCollection.Add(&m_Function4Button);
	vLocalCWndCollection.Add(&m_Background);
	vLocalCWndCollection.Add(&m_UserLabel);
	vLocalCWndCollection.Add(&m_CPProductLabel);
	vLocalCWndCollection.Add(&m_Function1Button);
	vLocalCWndCollection.Add(&m_DialogTitleStaticText);
	vLocalCWndCollection.Add(&m_Function2Button);
	vLocalCWndCollection.Add(&m_Function5Button);
	vLocalCWndCollection.Add(&m_SubFunction1Button);
	//vLocalCWndCollection.Add(&m_CurrentUserLabel);
}

void CConfigureProduct::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfigureProduct)
	//  DDX_Control(pDX, IDC_ArrowLeft2, m_ArrowLeft2);
	DDX_Control(pDX, IDC_SubFunction2Button, m_SubFunction2Button);
	DDX_Control(pDX, IDC_SubFunction5Button, m_SubFunction5Button);
	DDX_Control(pDX, IDC_SubFunction8Button, m_SubFunction8Button);
	DDX_Control(pDX, IDC_SubFunction7Button, m_SubFunction7Button);
	DDX_Control(pDX, IDC_SubFunction6Button, m_SubFunction6Button);
	DDX_Control(pDX, IDC_Function3Button, m_Function3Button);
	//  DDX_Control(pDX, IDC_ArrowDown3, m_ArrowDown3);
	DDX_Control(pDX, IDC_SubFunction3Button, m_SubFunction3Button);
	//  DDX_Control(pDX, IDC_ArrowLeft3, m_ArrowLeft3);
	DDX_Control(pDX, IDC_SubFunction4Button, m_SubFunction4Button);
	//  DDX_Control(pDX, IDC_ArrowLeft4, m_ArrowLeft4);
	//  DDX_Control(pDX, IDC_ArrowDown4, m_ArrowDown4);
	DDX_Control(pDX, IDC_Function4Button, m_Function4Button);
	DDX_Control(pDX, IDC_Background, m_Background);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_UserLabel, m_UserLabel);
	DDX_Control(pDX, IDC_CPProductLabel, m_CPProductLabel);
	DDX_Control(pDX, IDC_Function1Button, m_Function1Button);
	DDX_Control(pDX, IDC_DialogTitleStaticText1, m_DialogTitleStaticText);
	DDX_Control(pDX, IDC_Function2Button, m_Function2Button);
	DDX_Control(pDX, IDC_Function5Button, m_Function5Button);
	DDX_Control(pDX, IDC_SubFunction1Button, m_SubFunction1Button);
	//  DDX_Control(pDX, IDC_ArrowRight3, m_ArrowRight3);
}


BEGIN_MESSAGE_MAP(CConfigureProduct, CDialog)
	//{{AFX_MSG_MAP(CConfigureProduct)
	ON_BN_CLICKED(IDC_Function1Button, OnFunction1Button)
	ON_BN_CLICKED(IDC_Function2Button, OnFunction2Button)
	ON_BN_CLICKED(IDC_Function4Button, OnFunction4Button)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_Function3Button, OnFunction3Button)
	ON_BN_CLICKED(IDC_Function5Button, OnFunction5Button)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_SubFunction1Button, OnSubFunction1Button)
	ON_BN_CLICKED(IDC_SubFunction5Button, OnSubFunction5Button)
	ON_BN_CLICKED(IDC_SubFunction2Button, OnSubFunction2Button)
	ON_BN_CLICKED(IDC_SubFunction7Button, OnSubFunction7Button)
	ON_BN_CLICKED(IDC_SubFunction8Button, OnSubFunction8Button)
	ON_BN_CLICKED(IDC_SubFunction6Button, OnSubFunction6Button)
	ON_BN_CLICKED(IDC_SubFunction4Button, OnSubFunction4Button)
	ON_BN_CLICKED(IDC_SubFunction3Button, OnSubFunction3Button)
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
	ON_STN_CLICKED(IDC_DialogTitleStaticText1, &CConfigureProduct::OnDialogtitlestatictext1)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfigureProduct message handlers

void CConfigureProduct::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	// TODO: Add your message handler code here
	m_Background.SetWindowPos(NULL,cMainBackgroundLeft,
		cMainBackgroundTop,cMainBackgroundLength,
		cMainBackgroundHeight,SWP_NOZORDER);

	//if (!vLocalSystemData->vBackupDrive.GetLength()) //if don't have a backup drive
	//{
	//	m_Function2Button.ShowWindow(SW_SHOW);
	//	SetDlgItemText(IDC_Function2Button, "Fix Backup Drive");
	//}

	vFunction4ButtonEnable = true;	
	vSubFunction6ButtonEnable = true;	
	vSubFunction7ButtonEnable = true;
	vSubFunction8ButtonEnable = true;

	CString TempText;
	TempText.LoadString(IDS_Evaluate);
	SetDlgItemText(IDC_SubFunction1Button,TempText);

	TempText.LoadString(IDS_Edit);
	SetDlgItemText(IDC_SubFunction5Button,TempText);

	TempText.LoadString(IDS_Copy);
	SetDlgItemText(IDC_SubFunction6Button,TempText);

	TempText.LoadString(IDS_Delete);
	SetDlgItemText(IDC_SubFunction4Button,TempText);

	TempText.LoadString(IDS_New);
	SetDlgItemText(IDC_SubFunction8Button,TempText);

	TempText.LoadString(IDS_MainMenu);
	SetDlgItemText(IDC_Function1Button,TempText);

	TempText.LoadString(IDS_Options);
	SetDlgItemText(IDC_Function5Button,TempText);

	TempText.LoadString(IDS_SetupProducts);
	SetDlgItemText(IDC_DialogTitleStaticText1,TempText);

	TempText.LoadString(IDS_NoCurrentProduct);
	SetDlgItemText(IDC_CPProductLabel,TempText);

	if (vGlobalCurrentUser != "No User")
		TempText= "User: " +  vGlobalCurrentUser + "\nLevel: " + cLevelName[vGlobalPasswordLevel];
	else
		TempText= vGlobalCurrentUser + "\nLevel: " + cLevelName[vGlobalPasswordLevel];

	vOldCurrentUser = vGlobalCurrentUser;

	SetDlgItemText(IDC_UserLabel,TempText);
	m_UserLabel.ShowWindow(SW_SHOW);

	UpdateButtons();

	if (((vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac) || (vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac) ||  
		(vLocalConfigurationData->vScanTracType == cAllegroScanTrac) || (vLocalConfigurationData->vScanTracType == cSoloScanTrac) || (vLocalConfigurationData->vScanTracType == cSoloPlusScanTrac)) && 
		(!vLocalConfigurationData->vContinuousFeedContainerRate) && (!vLocalConfigurationData->vBulkProductMode)) //Not Continuous Feed, or a Pipeline or TD
	{
		TempText.LoadString(IDS_New);
		SetDlgItemText(IDC_SubFunction8Button, TempText);
		TempText.LoadString(IDS_AdvancedNew);
		SetDlgItemText(IDC_SubFunction7Button, TempText);
		m_SubFunction7Button.ShowWindow(SW_SHOW);
	}
	else
	{
		m_SubFunction7Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction7Button.GetRectToInvalidateArrow());
	}

	CDialog::OnShowWindow(bShow, nStatus);
	this->SetWindowText(_T("Products"));

	int TimerResult = SetTimer(vOneSecondTimerHandle,1000,NULL);
	if (!TimerResult)
		ReportErrorMessage("Error-One Second Timer Failed",cEMailInspx,32000);
	SetupMenu(vLocalCWndCollection);
}

void CConfigureProduct::OnFunction1Button() 
{
	// User clicked Main Menu Button to exit 
	CDialog::EndDialog(10);
}

void CConfigureProduct::OnFunction2Button() 
{
	//if (!vLocalSystemData->vBackupDrive.GetLength()) //if don't have a backup drive
	//{
	//	int TempError = _spawnl(_P_NOWAIT, "C:\\Fix.bat", "C:\\Fix.bat", NULL);
	//	CNoticeDialog TempNoticeDialog;
	//	TempNoticeDialog.vNoticeText = "\n\nReboot.\nThen check for the backup drive.";
	//	TempNoticeDialog.vType = cNoticeMessage;
	//	TempNoticeDialog.DoModal();

	//	m_Function2Button.ShowWindow(SW_HIDE);
	//	this->InvalidateRect(&m_Function2Button.GetRectToInvalidateArrow());
	//}
}

void CConfigureProduct::OnFunction3Button() 
{
  //Function 5 is the setup System button
	if (PasswordOK(cTemporaryInspxPassword,false))
	{
		//Create dialog box
		CNewConfigureSystemDialog IConfigureSystem;  

		//Set dialog box data like how many products there are
		IConfigureSystem.vMainWindowPointer = vMainWindowPointer;
		IConfigureSystem.vLocalConfigurationData = vLocalConfigurationData;
		IConfigureSystem.vLocalSystemData = vLocalSystemData;
		IConfigureSystem.vLocalProductCollection = vLocalProductCollection;			

		//Pass control to dialog box and display
		int nResponse = IConfigureSystem.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
		}
		else if (nResponse == 10)
		{
			Sleep(1); //is the equivelent of a yeild statement;
			CDialog::OnCancel();
		}
		else if (nResponse == IDCANCEL)
		{
			//  dismissed with Cancel
			CDialog::OnCancel();
		}
	}
}

void CConfigureProduct::SelectAndEdit(int TempWindowTypeMode)
{
	// TODO: Add your control notification handler code here
  //Edit Product button, so open pick product, then open editor
	//Function 1 is the Select Product button
	vLocalSystemData->vChangedSizeOfProduct = false;
	vLocalSystemData->vChangedSizeOfProductCheckDensityROI = 0;
	int TempInt = vLocalProductCollection->GetSize();
  if (TempInt > 0)  //if have any products configured
  {
		int nResponse = IDOK;
		int TempInt2 = 1;
		if (TempInt > 1)
		{
			//Create dialog box
			CEasySelectProductDialog ISelectProductDialog;  

			//Set dialog box data like which square to start on
			ISelectProductDialog.vLocalProductCollection = vLocalProductCollection;
			ISelectProductDialog.vLocalSystemData = vLocalSystemData;
			ISelectProductDialog.SelectWindowForStyle = TempWindowTypeMode;
			//Pass control to dialog box and display
			nResponse = ISelectProductDialog.DoModal();
			if (nResponse == IDOK)
				TempInt2 = ISelectProductDialog.vProductNumberSelected;
			//dialog box is now closed, if user pressed select do this
			//if user pressed cancel, do nothing
		}
		if (nResponse == IDOK)
		{
		  //Get user's selection 
			if (TempInt2 > 0)
			{
				CProduct *TempProductPointer = GetProductByNumber(TempInt2);
				if (TempProductPointer)
				{
					if (vGlobalCurrentProduct)
					if (*(vGlobalCurrentProduct->GetProductName()) != *(TempProductPointer->GetProductName()))
					{
						vLocalSystemData->vShiftProductCode = " "; // product changed, clear product code kjh 8/8/2012
					}
					if ((vGlobalCurrentProduct == NULL) || (vLocalSystemData->vSystemRunMode == cStoppedSystemMode) ||
					 (*(vGlobalCurrentProduct->GetProductName()) != *(TempProductPointer->GetProductName())))
					{
						EditTheProduct(TempProductPointer, TempWindowTypeMode);
					}
					vLocalSystemData->vITIPCDig->DeleteBackupLearnFiles();
				}
		  }
		}
		else 
		{
			if (nResponse == 10)
			{
				// TODO: Place code here to handle when the dialog is
				//  Main Menu Button pressed
				Sleep(1);
				CDialog::EndDialog(10);
			}
		}
  }
	else
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\n\nNo products are configured to select from";
		TempNoticeDialog.vType = cErrorMessage;
		TempNoticeDialog.DoModal();
	}
}

void CConfigureProduct::OnFunction4Button() 
{
	//calibrate detectors button pressed
	if (vLocalSystemData->vSystemRunMode == cStoppedSystemMode)
	{
		if (vMainWindowPointer->InterlocksOK())
		{
			if (PasswordOK(cCertifiedPassword, true))
			{
				CYesNoDialog TempYesNoDialog;
				TempYesNoDialog.vNoticeText = _T("\n\nAre you sure you want to Calibrate the Detectors?\nAll products may need to be re-learned.");
				CString TempText;
				TempText.LoadString(IDS_CalibrateDetectors);
				TempYesNoDialog.vYesButtonText = TempText;
				CString TempText1;
				TempText1.LoadString(IDS_Cancel);
				TempYesNoDialog.vNoButtonText = TempText1;
				TempYesNoDialog.vQuestionType = cConfirmQuestion;
				int TempResult = TempYesNoDialog.DoModal();
				if (TempResult == IDOK)
				{
					//get integration time from default product and put in all system products
					WORD TempIntegrationTime = 750;  //.3 milli second
					float TempVoltage = 10;  //.60 KV

					CProduct *TempProduct = NULL;

					//for Express setup, use ~Default Express, for other new setups use ~Default
					if (((vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac) || (vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac) || 
						(vLocalConfigurationData->vScanTracType == cAllegroScanTrac) ||	(vLocalConfigurationData->vScanTracType == cSoloScanTrac) || (vLocalConfigurationData->vScanTracType == cSoloPlusScanTrac)) && 
						(!vLocalConfigurationData->vContinuousFeedContainerRate) && (!vLocalConfigurationData->vBulkProductMode) && ((vGlobalCurrentProduct) && (!vGlobalCurrentProduct->vBulkProductMode))) //Not Continuous Feed, or a Pipeline or TD
							TempProduct = vMainWindowPointer->GetProductByName("~Default Express");
					else
						TempProduct = vMainWindowPointer->GetProductByName("~Default");

					if (TempProduct)
					{
						TempIntegrationTime = TempProduct->vXRayIntegrationTime;
						TempVoltage = TempProduct->vXRaySourceVoltage;
					}
					if (vGlobalCurrentProduct)
					{
						TempIntegrationTime = vGlobalCurrentProduct->vXRayIntegrationTime;
						TempVoltage = vGlobalCurrentProduct->vXRaySourceVoltage;
					}

					//if (PasswordOK(cTemporaryInspxPassword,false))
					{
						if (vLocalConfigurationData->vCorrectDetectorEdges != 2)
						{
							CYesNoDialog TempYesNoDialog;
							TempYesNoDialog.vNoticeText = _T("\n\nNot correcting all Pixels.\nCorrect all Pixels?");
							TempYesNoDialog.vYesButtonText = "Correct All Pixels";
							TempYesNoDialog.vNoButtonText = "Keep As Is";
							TempYesNoDialog.vQuestionType = cConfirmQuestion;
							int TempResult = TempYesNoDialog.DoModal();
							if (TempResult == IDOK)
								vLocalConfigurationData->vCorrectDetectorEdges = 2;
						}

						CNumericEntryDialog INumericEntryDialog;  
						
						//Set dialog box data titles and number value
						double TempUserIntegrationTime =((double)TempIntegrationTime / 2500.0);
						INumericEntryDialog.vEditString = dtoa(TempUserIntegrationTime,3);

						if (vGlobalCurrentProduct)
							INumericEntryDialog.m_DialogTitleStaticText1 = "Use Current Product Integration Time";
						else
							INumericEntryDialog.m_DialogTitleStaticText1 = "Use Default Product Integration Time";
						INumericEntryDialog.m_DialogTitleStaticText2 = "or Enter New integration time";
						INumericEntryDialog.m_DialogTitleStaticText3 = "to use in all calibration products";
						INumericEntryDialog.m_DialogTitleStaticText4 = "Original Value: " + 
							INumericEntryDialog.vEditString;

						if (vGlobalFPGAVersion10Point0OrHigher)
						{
							INumericEntryDialog.m_UnitsString = "Typical: .3, Minimum: .082 Milli-Seconds";
							INumericEntryDialog.vMinValue = .082;
						}
						else
						{
							INumericEntryDialog.m_UnitsString = "Typical: .3, Minimum: .3 Milli-Seconds";
							INumericEntryDialog.vMinValue = .3;
						}
						INumericEntryDialog.vMaxValue = 10;
						INumericEntryDialog.vIntegerOnly = false;
						//Pass control to dialog box and display
						TempResult = INumericEntryDialog.DoModal();
						//dialog box is now closed, if user pressed select do this
						//if user pressed cancel, do nothing
						if (TempResult == IDOK)
						{
							TempUserIntegrationTime = ATOF(INumericEntryDialog.vEditString);
							TempIntegrationTime = (WORD)(TempUserIntegrationTime * 2500);

							//ask Inspx person for voltage to use
							//Set dialog box data titles and number value
							CString TempText;
							CString TempText1;
							INumericEntryDialog.vEditString = dtoa(TempVoltage * 6,2);
							INumericEntryDialog.m_DialogTitleStaticText1 = "Use Default Product X-Ray Voltage";
							INumericEntryDialog.m_DialogTitleStaticText2 = "or Enter X-Ray Voltage";
							INumericEntryDialog.m_DialogTitleStaticText3 = "to use in all calibration products";
							TempText.LoadString(IDS_OriginalValue);
							INumericEntryDialog.m_DialogTitleStaticText4 = TempText + ": " + INumericEntryDialog.vEditString;

							TempText.LoadString(IDS_MeasuredIn);
							//TempText1.LoadString(IDS_Volts);
							TempText1 = "KV";
							INumericEntryDialog.m_UnitsString = TempText + " " + TempText1;

							INumericEntryDialog.vMaxValue = vLocalConfigurationData->vMaximumVoltage;
							INumericEntryDialog.vMinValue = cMinimumXRayVoltage;
							INumericEntryDialog.vAllowZero = true;
							INumericEntryDialog.vIntegerOnly = false;
							//Pass control to dialog box and display
							TempResult = INumericEntryDialog.DoModal();
							//dialog box is now closed, if user pressed select do this
							//if user pressed cancel, do nothing
							if (TempResult == IDOK)
							{
								TempVoltage = (float)(ATOF(INumericEntryDialog.vEditString) / 6.0);
							}
							else 
							if (TempResult == 10)
							{
								//Main Menu button pressed
								TempResult = 0;
								Sleep(1); //is the equivelent of a yeild statement;
								CDialog::EndDialog(10);
							}
						}
						else 
						if (TempResult == 10)
						{
							//Main Menu button pressed
							TempResult = 0;
							Sleep(1); //is the equivelent of a yeild statement;
							CDialog::EndDialog(10);
						}
					}
					if (TempResult == IDOK)
					{
						ReportErrorMessage("Save Original Product so can restore after calibration complete", cWriteToLog, 0);
						vMainWindowPointer->vCalibrateHoldCurrentProduct = vGlobalCurrentProduct;

						vLocalSystemData->vCalibratingXScanPoint4mmDetectors = 0; //calibrate saving in defalt calibration values
						if (vLocalConfigurationData->vUseXScanDetectors)
							vLocalConfigurationData->SetPixelsPerDetector(64);  //if using X-Scan, calibrate .8 mm mode first

						vMainWindowPointer->CreateBackupGainAndPixelAdjustments();
						//set all detector gains to 10
						BYTE TempNumberOfDetectorsToCalibrate = vLocalConfigurationData->vNumberOfXRayDetectors;
						if (vLocalConfigurationData->vNumberOfDetectorsToCalibrate)
							TempNumberOfDetectorsToCalibrate = vLocalConfigurationData->vNumberOfDetectorsToCalibrate;

						for (DWORD TempLoop = 0; TempLoop < TempNumberOfDetectorsToCalibrate; TempLoop++)
							vLocalConfigurationData->vDetectorGain[TempLoop] = 10;  //set all gains at maximum


						vMainWindowPointer->SendADCGainsAndOffsets(false);

						//clear all pixel calibrations

						for (WORD TempDetectorTypeLoop = cPoint4mmDetectors; TempDetectorTypeLoop <= cPoint8mmDetectors; TempDetectorTypeLoop++)
						for (WORD TempLoop = 0; TempLoop < cNumberOfPixelsToAdjustAllPixels1152; TempLoop++)
						{
							vLocalConfigurationData->vDetectorEdgeAdjustGain[TempLoop][TempDetectorTypeLoop] = 0;
							vLocalConfigurationData->vDetectorEdgeAdjustOffset[TempLoop][TempDetectorTypeLoop] = 0;
						}
						if (TempProduct)
						{
							TempProduct->vXRayIntegrationTime = TempIntegrationTime;
							//TempProduct->vXRayIntegrationTimeAtEdge = TempProduct->vXRayIntegrationTime;
						}

						TempProduct = vMainWindowPointer->GetProductByName("~Calibrate Offset");
						if (TempProduct)
							TempProduct->SetProductName("~Calibrate Gain");
						else
							TempProduct = vMainWindowPointer->GetProductByName("~Calibrate Gain");

						double TempDelay = 2;
						//if (vLocalConfigurationData->vUseMetric)
						//	TempDelay = TempDelay * 25.4;
						if (TempProduct)
						{
							if (PasswordOK(cTemporaryInspxPassword,false))
							{
								TempProduct->vXRayIntegrationTime = TempIntegrationTime;
								//TempProduct->vXRayIntegrationTimeAtEdge = TempProduct->vXRayIntegrationTime;

								TempProduct->vXRaySourceVoltage = TempVoltage;

								if (vLocalConfigurationData->vMaximumCurrent < TempProduct->vXRaySourceCurrent)
									TempProduct->vXRaySourceCurrent = (float)vLocalConfigurationData->vMaximumCurrent;

								if (vLocalConfigurationData->vMaximumVoltage < TempProduct->vXRaySourceVoltage)
									TempProduct->vXRaySourceVoltage = (float)vLocalConfigurationData->vMaximumVoltage;
									
								if (vLocalConfigurationData->vHVPSType == cCheyneyMonoBlock)
								{
									if (TempProduct->vXRaySourceCurrent > vGlobalDXMXRayPort->vCurrentMaximumScale)
										TempProduct->vXRaySourceCurrent = (float)vGlobalDXMXRayPort->vCurrentMaximumScale;
									if (TempProduct->vXRaySourceCurrent > 20)
										TempProduct->vXRaySourceCurrent = 20;
								}
								
								if (vLocalConfigurationData->vHVPSType == c100WVJTMonoBlockHVPS)
								if (TempProduct->vXRaySourceCurrent > 2.5)
										TempProduct->vXRaySourceCurrent = 2.5;

								if (vLocalConfigurationData->vHVPSType == c100WMonoBlockHVPS)
								if (TempProduct->vXRaySourceCurrent > 2)
										TempProduct->vXRaySourceCurrent = 2;

								if (vLocalConfigurationData->vHVPSType == c210WMonoBlockHVPS)
								if (TempProduct->vXRaySourceCurrent > 3)
									TempProduct->vXRaySourceCurrent = 3;

								if (vLocalConfigurationData->vHVPSType == cMNXHVPS)
								if (TempProduct->vXRaySourceCurrent > 1)
									TempProduct->vXRaySourceCurrent = 1;

								if (TempProduct->vBTToImagePosition > TempDelay * 5)
									TempProduct->SetProductBodyTriggerToImageBeltPositionOffset((float)TempDelay);

								if (TempProduct->vProductImageWidth > TempDelay * 5)
									TempProduct->SetProductImageWidth((float)TempDelay * 2);

								if (TempProduct->vProductLockOutWidth > TempDelay * 5)
									TempProduct->SetProductLockOutWidth((float)TempDelay);
							}
						}
										
						if ((vLocalConfigurationData->vUseXScanDetectors) && (vLocalConfigurationData->vScanTracType != cForteScanTrac) && (vLocalConfigurationData->vScanTracType != cCaseInspectorScanTrac))
							TempProduct = vMainWindowPointer->GetProductByName("~Calibrate Pixels .8mm");

						if (!TempProduct)
							TempProduct = vMainWindowPointer->GetProductByName("~Calibrate Pixels");

						if (!TempProduct)
						{
							TempProduct = vMainWindowPointer->GetProductByName("~Calibrate");
							if (TempProduct)
								TempProduct->SetProductName("~Calibrate Pixels");
						}
						if (TempProduct)
						{
							if ((vLocalConfigurationData->vUseXScanDetectors) && (vLocalConfigurationData->vScanTracType != cForteScanTrac) && (vLocalConfigurationData->vScanTracType != cCaseInspectorScanTrac))
								TempProduct->SetProductName("~Calibrate Pixels .8mm");

							if (PasswordOK(cTemporaryInspxPassword,false))
							{
								TempProduct->vXRayIntegrationTime = TempIntegrationTime;
								//TempProduct->vXRayIntegrationTimeAtEdge = TempProduct->vXRayIntegrationTime;
								TempProduct->vXRaySourceVoltage = TempVoltage;

								double TempMaximumCurrent = vLocalConfigurationData->vMaximumCurrent;
								double TempMaxValue = GetMaximumCurrentFromVoltage(TempProduct->vXRaySourceVoltage * 6);
								if (TempMaximumCurrent > TempMaxValue)
									TempMaximumCurrent = TempMaxValue;

								TempMaxValue = GetMaximumCurrentFromPower(TempProduct->vXRaySourceVoltage * 6, vLocalConfigurationData->vMaximumPower);
								if (TempMaximumCurrent > TempMaxValue)
									TempMaximumCurrent = TempMaxValue;

								TempProduct->vXRaySourceCurrent = (float)(TempMaximumCurrent * 0.8); //80% of maximum current

								if (vLocalConfigurationData->vMaximumCurrent < TempProduct->vXRaySourceCurrent)
									TempProduct->vXRaySourceCurrent = (float)vLocalConfigurationData->vMaximumCurrent;
									
								if (vLocalConfigurationData->vHVPSType == cCheyneyMonoBlock)
								{
									if (TempProduct->vXRaySourceCurrent > vGlobalDXMXRayPort->vCurrentMaximumScale)
										TempProduct->vXRaySourceCurrent = (float)vGlobalDXMXRayPort->vCurrentMaximumScale;
									if (TempProduct->vXRaySourceCurrent > 20)
										TempProduct->vXRaySourceCurrent = 20;
								}

								if (vLocalConfigurationData->vHVPSType == c100WVJTMonoBlockHVPS)
								if (TempProduct->vXRaySourceCurrent > 2.5)
										TempProduct->vXRaySourceCurrent = 2.5;

								if (vLocalConfigurationData->vHVPSType == c100WMonoBlockHVPS)
								if (TempProduct->vXRaySourceCurrent > 2)
									TempProduct->vXRaySourceCurrent = 2;

								if (vLocalConfigurationData->vHVPSType == c210WMonoBlockHVPS)
								if (TempProduct->vXRaySourceCurrent > 3)
									TempProduct->vXRaySourceCurrent = 3;

								if (vLocalConfigurationData->vHVPSType == cMNXHVPS)
								if (TempProduct->vXRaySourceCurrent > 1)
									TempProduct->vXRaySourceCurrent = 1;

								if (vLocalConfigurationData->vMaximumVoltage < TempProduct->vXRaySourceVoltage)
									TempProduct->vXRaySourceVoltage = (float)vLocalConfigurationData->vMaximumVoltage;

								if (TempProduct->vBTToImagePosition > TempDelay * 5)
									TempProduct->SetProductBodyTriggerToImageBeltPositionOffset((float)TempDelay);
								if (TempProduct->vProductImageWidth > TempDelay * 5)
									TempProduct->SetProductImageWidth((float)TempDelay * 2);
								if (TempProduct->vProductLockOutWidth > TempDelay * 5)
									TempProduct->SetProductLockOutWidth((float)TempDelay);
							}
						}

						if ((vLocalConfigurationData->vUseXScanDetectors) && (vLocalConfigurationData->vScanTracType != cForteScanTrac) && (vLocalConfigurationData->vScanTracType != cCaseInspectorScanTrac))
						{
							TempProduct = vMainWindowPointer->GetProductByName("~Calibrate Pixels .4mm");
							if (TempProduct)
							{
								if (PasswordOK(cTemporaryInspxPassword,false))
								{
									TempProduct->vXRayIntegrationTime = TempIntegrationTime;
									//TempProduct->vXRayIntegrationTimeAtEdge = TempProduct->vXRayIntegrationTime;
									TempProduct->vXRaySourceVoltage = TempVoltage;

									double TempMaximumCurrent = vLocalConfigurationData->vMaximumCurrent;

									double TempMaxValue = GetMaximumCurrentFromVoltage(TempProduct->vXRaySourceVoltage * 6);
									if (TempMaximumCurrent > TempMaxValue)
										TempMaximumCurrent = TempMaxValue;

									TempMaxValue = GetMaximumCurrentFromPower(TempProduct->vXRaySourceVoltage * 6, vLocalConfigurationData->vMaximumPower);
									if (TempMaximumCurrent > TempMaxValue)
										TempMaximumCurrent = TempMaxValue;

									TempProduct->vXRaySourceCurrent = (float)(TempMaximumCurrent * 0.8); //80% of maximum current

									if (vLocalConfigurationData->vMaximumCurrent < TempProduct->vXRaySourceCurrent)
										TempProduct->vXRaySourceCurrent = (float)vLocalConfigurationData->vMaximumCurrent;
									
									if (vLocalConfigurationData->vHVPSType == cCheyneyMonoBlock)
									{
										if (TempProduct->vXRaySourceCurrent > vGlobalDXMXRayPort->vCurrentMaximumScale)
											TempProduct->vXRaySourceCurrent = (float)vGlobalDXMXRayPort->vCurrentMaximumScale;
										if (TempProduct->vXRaySourceCurrent > 20)
											TempProduct->vXRaySourceCurrent = 20;
									}

									if (vLocalConfigurationData->vHVPSType == c100WVJTMonoBlockHVPS)
									if (TempProduct->vXRaySourceCurrent > 2.5)
										TempProduct->vXRaySourceCurrent = 2.5;

									if (vLocalConfigurationData->vHVPSType == c100WMonoBlockHVPS)
									if (TempProduct->vXRaySourceCurrent > 2)
										TempProduct->vXRaySourceCurrent = 2;

									if (vLocalConfigurationData->vHVPSType == c210WMonoBlockHVPS)
									if (TempProduct->vXRaySourceCurrent > 3)
										TempProduct->vXRaySourceCurrent = 3;

									if (vLocalConfigurationData->vHVPSType == cMNXHVPS)
									if (TempProduct->vXRaySourceCurrent > 1)
										TempProduct->vXRaySourceCurrent = 1;

									if (vLocalConfigurationData->vMaximumVoltage < TempProduct->vXRaySourceVoltage)
										TempProduct->vXRaySourceVoltage = (float)vLocalConfigurationData->vMaximumVoltage;

									if (TempProduct->vBTToImagePosition > TempDelay * 5)
										TempProduct->SetProductBodyTriggerToImageBeltPositionOffset((float)TempDelay);
									if (TempProduct->vProductImageWidth > TempDelay * 5)
										TempProduct->SetProductImageWidth((float)TempDelay * 2);
									if (TempProduct->vProductLockOutWidth > TempDelay * 5)
										TempProduct->SetProductLockOutWidth((float)TempDelay);
								}
							}
						}

						TempProduct = vMainWindowPointer->GetProductByName("~SizeExposure");
						if (TempProduct)
						{
							if (PasswordOK(cTemporaryInspxPassword,false))
							{
								TempProduct->vXRayIntegrationTime = TempIntegrationTime;
								//TempProduct->vXRayIntegrationTimeAtEdge = TempProduct->vXRayIntegrationTime;
								if (vLocalConfigurationData->vHVPSType == cMNXHVPS)
									TempProduct->vXRaySourceCurrent = 1;
								else
									TempProduct->vXRaySourceCurrent = 4;

								TempProduct->vXRaySourceVoltage = TempVoltage;

								if (vLocalConfigurationData->vMaximumCurrent < TempProduct->vXRaySourceCurrent)
									TempProduct->vXRaySourceCurrent = (float)vLocalConfigurationData->vMaximumCurrent;

								if (vLocalConfigurationData->vMaximumVoltage < TempProduct->vXRaySourceVoltage)
									TempProduct->vXRaySourceVoltage = (float)vLocalConfigurationData->vMaximumVoltage;


								if (TempProduct->vBTToImagePosition > TempDelay * 5)
									TempProduct->SetProductBodyTriggerToImageBeltPositionOffset((float)TempDelay);
								if (TempProduct->vProductImageWidth > TempDelay * 5)
									TempProduct->SetProductImageWidth((float)TempDelay * 2);
								if (TempProduct->vProductLockOutWidth > TempDelay * 5)
									TempProduct->SetProductLockOutWidth((float)TempDelay);
							}
						}

						TempProduct = vMainWindowPointer->GetProductByName("~Default");
						if (TempProduct)
						{
							if (PasswordOK(cTemporaryInspxPassword,false))
							{
								TempProduct->vXRayIntegrationTime = TempIntegrationTime;
								//TempProduct->vXRayIntegrationTimeAtEdge = TempProduct->vXRayIntegrationTime;
								if (vLocalConfigurationData->vHVPSType == cMNXHVPS)
									TempProduct->vXRaySourceCurrent = 1;
								else
									TempProduct->vXRaySourceCurrent = 8;

								TempProduct->vXRaySourceVoltage = TempVoltage;

								if (vLocalConfigurationData->vMaximumCurrent < TempProduct->vXRaySourceCurrent)
									TempProduct->vXRaySourceCurrent = (float)vLocalConfigurationData->vMaximumCurrent;

								if (vLocalConfigurationData->vMaximumVoltage < TempProduct->vXRaySourceVoltage)
									TempProduct->vXRaySourceVoltage = (float)vLocalConfigurationData->vMaximumVoltage;
							}
						}

						TempProduct = vMainWindowPointer->GetProductByName("~Default Express");
						if (TempProduct)
						{
							if (PasswordOK(cTemporaryInspxPassword,false))
							{
								TempProduct->vXRayIntegrationTime = TempIntegrationTime;
								//TempProduct->vXRayIntegrationTimeAtEdge = TempProduct->vXRayIntegrationTime;
								if (vLocalConfigurationData->vHVPSType == cMNXHVPS)
									TempProduct->vXRaySourceCurrent = 1;
								else
									TempProduct->vXRaySourceCurrent = 8;

								TempProduct->vXRaySourceVoltage = TempVoltage;

								if (vLocalConfigurationData->vMaximumCurrent < TempProduct->vXRaySourceCurrent)
									TempProduct->vXRaySourceCurrent = (float)vLocalConfigurationData->vMaximumCurrent;

								if (vLocalConfigurationData->vMaximumVoltage < TempProduct->vXRaySourceVoltage)
									TempProduct->vXRaySourceVoltage = (float)vLocalConfigurationData->vMaximumVoltage;
							}
						}

						if (vLocalConfigurationData->vCorrectDetectorEdges)
						{
							vLocalSystemData->vCalibrateGainIterationNumber = 0;
							if (vLocalConfigurationData->vCorrectDetectorEdges == 2)
							{
								vMainWindowPointer->LoadCalibrateOffsetProduct();
								vLocalSystemData->vModeToRunAfterCalibrateOffset = cCalibratingDetectorGainMode; //correct gains then all pixels
								ReportErrorMessage("Complete Detector Calibration Started", cUserAction,0);
								vLocalSystemData->vFullDetectorCalibration = true;
							}
							else
							//if  (vLocalConfigurationData->vCorrectDetectorEdges == 1)
							{
								vMainWindowPointer->LoadCalibrateEdgesProduct();
								vLocalSystemData->vFindingProductBrightness = true;
								vLocalSystemData->vModeToRunAfterCalibrateOffset = cCalibratingDetectorEdgesMode;  //correcting just edge pixels, no gain adj
								ReportErrorMessage("Edge Pixel Only Calibration Started", cUserAction,0);
								vLocalSystemData->vFullDetectorCalibration = false;
							}
							//this will calibrate detector offsets, then do gain adjustments
							vLocalSystemData->vCalibrateErrorCount = 0;
							//vMainWindowPointer->CalibrateADCDetectorOffset(true,false);
							vLocalSystemData->vCalibrateADCAfterDAC = true;
							vLocalSystemData->vAutoCalibrate = true;
							vLocalSystemData->vAutoCalibrateWhileRunning = false;
							if (vLocalConfigurationData->vUseDACAndADCinDensityCompensation)
								vMainWindowPointer->CalibrateDACDetectorOffset();
							else
								vMainWindowPointer->CalibrateADCDetectorOffset(vLocalSystemData->vAutoCalibrate, false);
							Sleep(1); //is the equivelent of a yeild statement;
							CDialog::EndDialog(10);
						}
						else
						{
							CNoticeDialog TempNoticeDialog;
							TempNoticeDialog.vNoticeText = "\n\n\nSystem Setup not configured to Calibrate Detector";
							TempNoticeDialog.vType = cErrorMessage;
							TempNoticeDialog.DoModal();
						}
					}
				}
			}
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
		CNoticeDialog TempNoticeDialog;
		CString TempText("\n\n\nSystem must be stopped to run this function");
		TempNoticeDialog.vNoticeText = TempText;
		TempNoticeDialog.vType = cNoticeMessage;
		TempNoticeDialog.DoModal();
	}
	CWnd *TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText1);
	if (TempWindow)
		TempWindow->Invalidate(false);
}

void CConfigureProduct::OnFunction5Button() 
{
	if (PasswordOK(cSetupPassword,true))
	{
		CScanTracConfigureDialog IScanTracConfigureDialog;  
		//Set dialog box data like how many products there are
		IScanTracConfigureDialog.vLocalSystemData = vLocalSystemData;
		IScanTracConfigureDialog.vLocalConfigurationData = vLocalConfigurationData;
		IScanTracConfigureDialog.vLocalProductCollection = vLocalProductCollection;
		//IScanTracConfigureDialog.vpLocalCurrentProduct = vGlobalCurrentProduct;
		IScanTracConfigureDialog.vMainWindowPointer = vMainWindowPointer;

		//Pass control to dialog box and display
		int nResponse = IScanTracConfigureDialog.DoModal();
		//dialog box is now closed, 
		if (nResponse == IDOK)
		{
		}
		else if (nResponse == 10)
		{
			Sleep(1); //is the equivelent of a yeild statement;
			CDialog::OnCancel();
		}
		CWnd *TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText1);
		if (TempWindow)
			TempWindow->Invalidate(false);
	}
}

void CConfigureProduct::OnSubFunction1Button() 
{
	if (PasswordOK(cSetupPassword,true))
	{
		// Evaluate Product Button Pressed
		//User selected copy product
		if (vLocalSystemData->vSystemRunMode == cStoppedSystemMode)
		{
			SelectAndEdit(SelectProductForEvaluate);
			//Sleep(1);
			//CDialog::EndDialog(10);
		}
		else
		if (vGlobalCurrentProduct)
		{  //can only evaluate the current product if already running
			//Evaluate Product button was pressed
			CAutoSetupImageDisplayDialog IAutoSetupImageDisplayDialog;
			IAutoSetupImageDisplayDialog.vLocalConfigurationData = vLocalConfigurationData;
			IAutoSetupImageDisplayDialog.vLocalSystemData = vLocalSystemData;
			IAutoSetupImageDisplayDialog.vMainWindowPointer = vMainWindowPointer;
			IAutoSetupImageDisplayDialog.vShowReturnToEditMenu = true;
			IAutoSetupImageDisplayDialog.vAutoSetupType = cEvaluateAutoSetup;
			//when go into auto setup, it will go into select inspection
			IAutoSetupImageDisplayDialog.vMode = cSetInspections;
			IAutoSetupImageDisplayDialog.vPickOneToEvaluate = true;

			IAutoSetupImageDisplayDialog.vInspectionEditing = NULL;
			IAutoSetupImageDisplayDialog.vLocalProductCollection = vLocalProductCollection;
			BYTE TempEnableEjectors = vLocalConfigurationData->vEnableEjectors;
			int nResponse = IAutoSetupImageDisplayDialog.DoModal();

			if ((nResponse == IDOK) || (nResponse == 20))
			{
				// TODO: Place code here to handle when the dialog is accepted
				if (TempEnableEjectors != vLocalConfigurationData->vEnableEjectors)
				{
					CString TempProductName = *vGlobalCurrentProduct->GetProductName();
					CYesNoDialog TempYesNoDialog;
					CString TempText;//("\n\nEjectors were disabled when entered Edit While Running\nRe-enable Ejectors Now?");
					TempText.LoadString(IDS_EjectorsweredisabledwhenenteredEditWhileRunning);
					TempYesNoDialog.vNoticeText = TempText;
					CString TempText1;
					TempText1.LoadString(IDS_EnableEjectors);
					TempYesNoDialog.vYesButtonText = TempText1;
					TempText1.LoadString(IDS_KeepDisabled);
					TempYesNoDialog.vNoButtonText = TempText1;
					TempYesNoDialog.vQuestionType = cConfirmQuestion;
					int TempResult = TempYesNoDialog.DoModal();
					if (TempResult == IDOK)
					{
						//vLocalConfigurationData->vEnableEjectors = true;
						//vGlobaluCSerialPort->EnableDisableEjectors(vLocalConfigurationData->vEnableEjectors);
						//vMainWindowPointer->UpdateDisableEjectorsButton();
						vMainWindowPointer->EnableDisableEjectors(true);
						ReportErrorMessage("Ejectors re-enabled by operator Leaving Setup", cUserChanges,0);
					}
				}
			}
			CDialog::EndDialog(true);
		}
	}
}

void CConfigureProduct::OnSubFunction3Button() 
{
	//reboot computer if nothing running
	if (PasswordOK(cSetupPassword,true))
	if (vLocalSystemData->vSystemRunMode == cStoppedSystemMode)
	{
		CYesNoDialog TempYesNoDialog;
		TempYesNoDialog.vNoticeText = _T("\n\nReboot Computer Now?\nRebooting takes about 2 minutes.");
		TempYesNoDialog.vYesButtonText = "Reboot Now!";
		TempYesNoDialog.vNoButtonText = "Cancel Reboot";
		TempYesNoDialog.vQuestionType = cConfirmQuestion;
		int TempResult = TempYesNoDialog.DoModal();
		if (TempResult == IDOK)
		{
			if (vMainWindowPointer)
				vMainWindowPointer->WriteConfigurationDataToFile(false);
			SystemShutDown(cReboot);
		}
	}
	else
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\n\nX-Rays must be off to Reboot";
		TempNoticeDialog.vType = cNoticeMessage;
		TempNoticeDialog.DoModal();
	}
}

void CConfigureProduct::OnSubFunction4Button() 
{

	if (PasswordOK(cSetupPassword,true))
	{
		//User pressed Delete Product button
		if (vLocalSystemData->vSystemRunMode == cStoppedSystemMode)
		{
			int TempInt = vLocalProductCollection->GetSize();
			if (TempInt > 0)  //if have any products configured
			{
				//Create dialog box
				CEasySelectProductDialog ISelectProductDialog;  
				//Set dialog box data like which square to start on
				ISelectProductDialog.vLocalSystemData = vLocalSystemData;
				ISelectProductDialog.vLocalProductCollection = vLocalProductCollection;
				ISelectProductDialog.SelectWindowForStyle = SelectProductForDelete;
		
				//Pass control to dialog box and display
				int nResponse = ISelectProductDialog.DoModal();
				//dialog box is now closed, if user pressed select do this
				//if user pressed cancel, do nothing
				if (nResponse == IDOK)
				{
					//  closed with OK
					//Get users selection
					int TempInt2 = ISelectProductDialog.vProductNumberSelected;
					if(TempInt2 > 0)
					{
						CProduct *TempProductPointer = GetProductByNumber(TempInt2);
						if(TempProductPointer != NULL)
						{
							CString TempProductName = *TempProductPointer->GetProductName();
							if ((TempProductPointer == vGlobalCurrentProduct) && 
								(vLocalSystemData->vSystemRunMode != cStoppedSystemMode))
							{
								//can't delete current product
								CNoticeDialog TempNoticeDialog;
								TempNoticeDialog.vNoticeText = "\n\nYou cannot delete the current product\nwhen the system is running";
								TempNoticeDialog.vType = cNoticeMessage;
								TempNoticeDialog.DoModal();
							}
							else
							if (((TempProductName == "~Default") || (TempProductName == "~Default Express")) && (!PasswordOK(cTemporaryInspxPassword,false)))
							{
								//can't delete current product
								CNoticeDialog TempNoticeDialog;
								TempNoticeDialog.vNoticeText = "\n\n\nYou cannot delete the product: Default";
								TempNoticeDialog.vType = cNoticeMessage;
								TempNoticeDialog.DoModal();
							}
							else
							{
								//Delete the Product
								CString TempProductName = *TempProductPointer->GetProductName();
								CYesNoDialog TempYesNoDialog;
								CString TempString("\n\nDelete Product: " + TempProductName + "?");
								TempYesNoDialog.vYesButtonText = "Delete Product";
								if (TempProductPointer == vGlobalCurrentProduct)
									TempString = "\nThis is the Current Product!: " + TempProductName + "\n\nAre you sure you want to delete it?";
								if ((TempProductName == "~Default") || (TempProductName == "~Default Express"))
								{
									TempString = "\nDefault product is used in product setup!\n\nAre you sure you want to delete it?";
									TempYesNoDialog.vYesButtonText = "Delete Default";
								}
								TempYesNoDialog.vNoticeText = TempString;
								TempYesNoDialog.vNoButtonText = "";
								TempYesNoDialog.vQuestionType = cConfirmQuestion;
								int TempResult = TempYesNoDialog.DoModal();
								if (TempResult == IDOK)
								{
									if (TempProductPointer == vGlobalCurrentProduct)
									{
										vGlobalCurrentProduct = NULL;
										vLocalSystemData->vCurrentProductName = "No Current Product";
										vLocalSystemData->vCurrentProductChanged = true;
										// delete any image files
										vLocalSystemData->vITIPCDig->ChangedSizeOfProduct(TempProductPointer);
									}
									else
										vLocalSystemData->vITIPCDig->DeleteLearnFiles(TempProductPointer);

									ReportErrorMessage("Operator Deleted Product: " + TempProductName, cUserChanges,0);
									//TempInt = vLocalProductCollection->GetSize();
									vLocalProductCollection->RemoveAt(TempInt2 - 1,1);
									//TempInt = vLocalProductCollection->GetSize();
									delete(TempProductPointer);
								}
							}
						}
					}
					//UpdateNumberOfProductsDisplay();
				}
				else 
				if (nResponse == 10)
				{
					//Main Menu Button Pressed
					Sleep(1);
					CDialog::EndDialog(10);
				}
			}
			else
			{
				CNoticeDialog TempNoticeDialog;
				TempNoticeDialog.vNoticeText = "\n\n\nNo products are configured to select from";
				TempNoticeDialog.vType = cNoticeMessage;
				TempNoticeDialog.DoModal();
			}
		}
		else
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\n\nYou cannot do this function while the system is running";
			TempNoticeDialog.vType = cNoticeMessage;
			TempNoticeDialog.DoModal();
		}
	}
}

void CConfigureProduct::OnSubFunction5Button() 
{
	if (PasswordOK(cSetupPassword,true))
	{
		//User select Edit product
		if (vLocalSystemData->vSystemRunMode == cStoppedSystemMode)
		{
			SelectAndEdit(SelectProductForEdit);
		}
		else
		if (vGlobalCurrentProduct)
		{  //can only edit the current product if already running
			BYTE TempEnableEjectors = vLocalConfigurationData->vEnableEjectors;
			if (vLocalConfigurationData->vEnableEjectors)
			{
				CString TempProductName = *vGlobalCurrentProduct->GetProductName();
				CYesNoDialog TempYesNoDialog;
				CString TempString("\nYou are about to edit the currently\ninspecting Product: " + 
					TempProductName + "\nDo you want to disable the ejectors\nin case a change causes rejects?");
				TempYesNoDialog.vYesButtonText = "Disable\nEjectors";
				TempYesNoDialog.vNoButtonText = "Keep\nEnabled";
				TempYesNoDialog.vNoticeText = TempString;
				TempYesNoDialog.vQuestionType = cConfirmQuestion;
				int TempResult = TempYesNoDialog.DoModal();
				if (TempResult == IDOK)
				{
					vMainWindowPointer->EnableDisableEjectors(false);
					ReportErrorMessage("Ejectors Disabled by operator entering Setup", cUserChanges,0);
				}
			}
			//EditTheProduct(vGlobalCurrentProduct, SelectProductForEdit);
			//Evaluate Product button was pressed
			CAutoSetupImageDisplayDialog IAutoSetupImageDisplayDialog;
			IAutoSetupImageDisplayDialog.vLocalConfigurationData = vLocalConfigurationData;
			IAutoSetupImageDisplayDialog.vLocalSystemData = vLocalSystemData;
			IAutoSetupImageDisplayDialog.vMainWindowPointer = vMainWindowPointer;
			IAutoSetupImageDisplayDialog.vShowReturnToEditMenu = true;
			IAutoSetupImageDisplayDialog.vAutoSetupType = cEvaluateAutoSetup;
			//when go into auto setup, it will go into select inspection
			IAutoSetupImageDisplayDialog.vMode = cSetInspections;

			IAutoSetupImageDisplayDialog.vInspectionEditing = NULL;
			IAutoSetupImageDisplayDialog.vLocalProductCollection = vLocalProductCollection;
			int nResponse = IAutoSetupImageDisplayDialog.DoModal();

			if (TempEnableEjectors != vLocalConfigurationData->vEnableEjectors)
			{
				CString TempProductName = *vGlobalCurrentProduct->GetProductName();
				CYesNoDialog TempYesNoDialog;
					CString TempText;//("\n\nEjectors were disabled when entered Edit While Running\nRe-enable Ejectors Now?");
					TempText.LoadString(IDS_EjectorsweredisabledwhenenteredEditWhileRunning);
					TempYesNoDialog.vNoticeText = TempText;
					CString TempText1;
					TempText1.LoadString(IDS_EnableEjectors);
					TempYesNoDialog.vYesButtonText = TempText1;
					TempText1.LoadString(IDS_KeepDisabled);
					TempYesNoDialog.vNoButtonText = TempText1;
				TempYesNoDialog.vQuestionType = cConfirmQuestion;
				int TempResult = TempYesNoDialog.DoModal();
				if (TempResult == IDOK)
				{
					//vLocalConfigurationData->vEnableEjectors = true;
					//vGlobaluCSerialPort->EnableDisableEjectors(vLocalConfigurationData->vEnableEjectors);
					//vMainWindowPointer->UpdateDisableEjectorsButton();
					vMainWindowPointer->EnableDisableEjectors(true);
					ReportErrorMessage("Ejectors enabled by operator leaving Setup", cUserChanges,0);
				}
			}
			CDialog::EndDialog(true);
		}
	}
}

void CConfigureProduct::OnSubFunction6Button() 
{
	if (PasswordOK(cSetupPassword,true))
	{
		//User selected copy product
		if (vLocalSystemData->vSystemRunMode == cStoppedSystemMode)
		{
			SelectAndEdit(SelectProductForCopy);
		}
		else
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\n\nYou cannot do this function while the system is running";
			TempNoticeDialog.vType = cNoticeMessage;
			TempNoticeDialog.DoModal();
		}
	}
}

void CConfigureProduct::OnSubFunction7Button() 
{
  //Advanced New Product button was pressed (only if have Express New Product button below).
  //Create dialog box
	//if you have an Express New Product Button
	if (PasswordOK(cSetupPassword,true))
	if (((vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac) || (vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac) || 
		(vLocalConfigurationData->vScanTracType == cAllegroScanTrac) || (vLocalConfigurationData->vScanTracType == cSoloScanTrac) || (vLocalConfigurationData->vScanTracType == cSoloPlusScanTrac)) && 
		(!vLocalConfigurationData->vContinuousFeedContainerRate) && (!vLocalConfigurationData->vBulkProductMode)) //Not Continuous Feed, or a Pipeline or TD
	if (vLocalSystemData->vSystemRunMode == cStoppedSystemMode)
	{
		CProduct *TempProduct = NULL;

		TempProduct = vMainWindowPointer->GetProductByName("~Default");

		if (!TempProduct)
		{  // no default product, so make one
			if (ThereIsEnoughMemory(sizeof(CProduct), "New default Product "))
			{
				TempProduct = new CProduct;
				TempProduct->vBodyTrigger = 1;
		
				TempProduct->SetProductName("~Default");
					if (TempProduct->vEjectorDelayPosition[0] < 45)
						TempProduct->SetEjectorBeltPositionOffset(0,72);
					if (TempProduct->vEjectorDelayPosition[1] < 45)
						TempProduct->SetEjectorBeltPositionOffset(1,82);
					if (TempProduct->vEjectorDelayPosition[2] < 45)
						TempProduct->SetEjectorBeltPositionOffset(2,92);
					//if (TempProduct->vEjectorDelayPosition[3] < 45)
						//TempProduct->SetEjectorBeltPositionOffset(3,102);
					//if (TempProduct->vEjectorDelayPosition[4] < 45)
						//TempProduct->SetEjectorBeltPositionOffset(4,112);
					//if (TempProduct->vEjectorDelayPosition[5] < 45)
						//TempProduct->SetEjectorBeltPositionOffset(5,122);

				TempProduct->vThresholdForContainerBounds = 220;
				TempProduct->vProductImageHeightTop = (float)(vLocalConfigurationData->vNumberOfXRayDetectors * vLocalConfigurationData->vDetectorLength);
				//if (vLocalConfigurationData->vUseMetric)
				//	TempProduct->vProductImageHeightTop = (WORD)(TempProduct->vProductImageHeightTop * 25.4);

				if (vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac)
					TempProduct->vProductImageWidth = 48;
				else
				if (vLocalConfigurationData->vScanTracType == cForteScanTrac)
					TempProduct->vProductImageWidth = 36;
				else
					TempProduct->vProductImageWidth = 24;
				TempProduct->vProductImageHeightBottom = 0;
				TempProduct->vProductImageHeightTop = (float)(vLocalConfigurationData->vDetectorLength * vLocalConfigurationData->vNumberOfXRayDetectors);
				//if (vLocalConfigurationData->vUseMetric)
				//	TempProduct->vProductImageHeightTop = (WORD)(TempProduct->vProductImageHeightTop * 25.4);

				if (vLocalConfigurationData->vSystemBodyTriggerToXRayBeam > 1)
				{
					TempProduct->vProductBodyTriggerToImageBeltPositionOffset = (float)(vLocalConfigurationData->vSystemBodyTriggerToXRayBeam - 4);
				}
				else
				if ((vLocalConfigurationData->vScanTracType == cSoloScanTrac) || (vLocalConfigurationData->vScanTracType == cSoloPlusScanTrac))
					TempProduct->vProductBodyTriggerToImageBeltPositionOffset = (float)(cSoloRightToLeftDistance - 2);
				else
					TempProduct->vProductBodyTriggerToImageBeltPositionOffset = 8;

				TempProduct->SetProductImageWidth(TempProduct->vProductImageWidth);

				TempProduct->SetProductLockOutWidth(TempProduct->vProductImageWidth);

				TempProduct->SetProductBodyTriggerToImageBeltPositionOffset(TempProduct->vProductBodyTriggerToImageBeltPositionOffset);
				
				if (vLocalConfigurationData->vScanTracType == cUpSideDownSoloScanTrac) //Not Continuous Feed, or a Pipeline or TD
				{
					TempProduct->SetEdgeLocationBottom((float)(TempProduct->vProductImageHeightTop * 0.8));
					TempProduct->SetEdgeLocationHeight((float)(TempProduct->vProductImageHeightTop * 0.1));
				}
				else
				if ((vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac) || (vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac) ||
					(vLocalConfigurationData->vScanTracType == cAllegroScanTrac) ||
					(vLocalConfigurationData->vScanTracType == cBriosoScanTrac) || (vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || ((vGlobalCurrentProduct) && (vGlobalCurrentProduct->vBulkProductMode))) //Not Continuous Feed, or a Pipeline or TD
				{
					TempProduct->SetEdgeLocationBottom((float)(TempProduct->vProductImageHeightTop * 0.2));
					TempProduct->SetEdgeLocationHeight((float)(TempProduct->vProductImageHeightTop * 0.8));
				}
				else
				{
					TempProduct->SetEdgeLocationBottom((float)(1));
					TempProduct->SetEdgeLocationHeight((float)(1));
				}

				TempProduct->vEndOfLineTimeOut = (float)(50);
				TempProduct->SetEndOfLineTimeOut(TempProduct->vEndOfLineTimeOut);

				TempProduct->vTooManyRejectsAlarmX[0] = 5;
				TempProduct->vTooManyRejectsAlarmY[0] = 5;
				TempProduct->vTooManyRejectsAlarmDisableEjectors[0] = true;

				if ((vLocalConfigurationData->vScanTracType == cSoloScanTrac) || (vLocalConfigurationData->vScanTracType == cSoloPlusScanTrac))
				{
					TempProduct->vXRaySourceVoltage = (float)(70.0 / 6.0);  //good values for Solo Express Auto Setup
					TempProduct->vXRaySourceCurrent = 1;
					TempProduct->vXRayIntegrationTime = 1500;  //.6 milli second
				}
				else
				//if (vLocalConfigurationData->vScanTracType == cAtempoScanTrac)
				{
					TempProduct->vXRaySourceVoltage = (float)(43.0 / 6.0);  //good values for TD Express Auto Setup
					TempProduct->vXRaySourceCurrent = 3;
					TempProduct->vXRayIntegrationTime = 1500;  //.6 milli second
				}

				if (vLocalConfigurationData->vMaximumCurrent < TempProduct->vXRaySourceCurrent)
					TempProduct->vXRaySourceCurrent = (float)vLocalConfigurationData->vMaximumCurrent;

				if (vLocalConfigurationData->vMaximumVoltage < TempProduct->vXRaySourceVoltage)
					TempProduct->vXRaySourceVoltage = (float)vLocalConfigurationData->vMaximumVoltage;

				TempProduct->CalculateEndOfLineTimeOut();
				TempProduct->vNumberOfInspections = 0;

				if ((vLocalConfigurationData->vScanTracType == cSoloScanTrac) || (vLocalConfigurationData->vScanTracType == cSoloPlusScanTrac))
				if (ThereIsEnoughMemory(sizeof(CInspectionSize), "New Inspection"))
				{
					TempProduct->vNumberOfInspections++;
					TempProduct->vInspection[TempProduct->vNumberOfInspections - 1] = new CInspectionSize;
					TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->vName = "Missing Lid";
					TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->vThreshold = 50;
					TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->vSize = 20;
					TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->vInspectionType = cMissingLidInspectionOnSize;
					TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->SetupInspection(TempProduct->vProductImageHeightTop,
						TempProduct->vProductImageHeightBottom, 0, TempProduct->vProductImageWidth, TempProduct->vOverScanMultiplier);
					if (vLocalConfigurationData->vEjector[0].vEnabled)
						TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->vEjector = 1;
				}
				if (ThereIsEnoughMemory(sizeof(CInspectionDensity), "New Inspection"))
				{
					TempProduct->vNumberOfInspections++;
					TempProduct->vInspection[TempProduct->vNumberOfInspections - 1] = new CInspectionDensity;
					TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->vHasDensityUpperThreshold = true;
					TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->vDensityThresholdUpper = 240;
					TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->vName = "Underfill";
					TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->vInspectionType = cUnderfillInspection;
					TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->SetupInspection(TempProduct->vProductImageHeightTop,
						TempProduct->vProductImageHeightBottom, 0, TempProduct->vProductImageWidth, TempProduct->vOverScanMultiplier);
					if (vLocalConfigurationData->vEjector[0].vEnabled)
						TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->vEjector = 1;
				}

				if ((vLocalConfigurationData->vScanTracType == cSoloScanTrac) || (vLocalConfigurationData->vScanTracType == cSoloPlusScanTrac))
				if (ThereIsEnoughMemory(sizeof(CInspectionContaminant), "New Inspection"))
				{
					TempProduct->vNumberOfInspections++;
					TempProduct->vInspection[TempProduct->vNumberOfInspections - 1] = new CInspectionContaminant;
					TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->vAddStructure = true;
					TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->vEnhanceMethod = 3;
					TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->vThreshold = 240;
					TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->vSize = 10;
					TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->vName = "Foreign Material";
					TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->vInspectionType = cBottomContaminantInspection;
					TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->SetupInspection(TempProduct->vProductImageHeightTop,
						TempProduct->vProductImageHeightBottom, 0, TempProduct->vProductImageWidth, TempProduct->vOverScanMultiplier);
					if (vLocalConfigurationData->vEjector[0].vEnabled)
						TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->vEjector = 1;
				}

				if ((vLocalConfigurationData->vScanTracType == cSoloScanTrac) || (vLocalConfigurationData->vScanTracType == cSoloPlusScanTrac))
				if (ThereIsEnoughMemory(sizeof(CInspectionContaminant), "New Inspection"))
				{
					TempProduct->vNumberOfInspections++;
					TempProduct->vInspection[TempProduct->vNumberOfInspections - 1] = new CInspectionContaminant;
					TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->vAddStructure = true;
					TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->vEnhanceMethod = 3;
					TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->vThreshold = 220;
					TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->vSize = 10;
					TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->vName = "Top FM";
					TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->vInspectionType = cTopContaminantInspection;
					TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->SetupInspection(TempProduct->vProductImageHeightTop,
						TempProduct->vProductImageHeightBottom, 0, TempProduct->vProductImageWidth, TempProduct->vOverScanMultiplier);
					if (vLocalConfigurationData->vEjector[0].vEnabled)
						TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->vEjector = 1;
				}

				if ((vLocalConfigurationData->vScanTracType != cSoloScanTrac) && (vLocalConfigurationData->vScanTracType != cSoloPlusScanTrac))
				if (ThereIsEnoughMemory(sizeof(CInspectionContaminant), "New Inspection"))
				{
					TempProduct->vNumberOfInspections++;
					TempProduct->vInspection[TempProduct->vNumberOfInspections - 1] = new CInspectionContaminant;
					TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->vSize = 2;
					TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->vFindSpotInSubROI = 10;
					TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->vPixelsStepToNextROI = 3;
					TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->vDesensitizeEdgePixels = 30;
					TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->vThreshold = 240;
					TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->vName = "Small Contaminant";
					TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->vInspectionType = cSpotInROIContaminantInspection;
					TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->SetupInspection(TempProduct->vProductImageHeightTop, 0, 0, TempProduct->vProductImageWidth, TempProduct->vOverScanMultiplier);
					if (vLocalConfigurationData->vEjector[0].vEnabled)
						TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->vEjector = 1;
				}

				if ((vLocalConfigurationData->vScanTracType != cSoloScanTrac) && (vLocalConfigurationData->vScanTracType != cSoloPlusScanTrac))
				if (ThereIsEnoughMemory(sizeof(CInspectionContaminant), "New Inspection"))
				{
					TempProduct->vNumberOfInspections++;
					TempProduct->vInspection[TempProduct->vNumberOfInspections - 1] = new CInspectionContaminant;
					TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->vSize = 8;
					TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->vPreSmoothImage = 3;
					TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->vFindSpotInSubROI = 15;
					TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->vPixelsStepToNextROI = 3;
					TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->vDesensitizeEdgePixels = 30;
					TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->vThreshold = 240;
					TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->vName = "Medium Contaminant";
					TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->vInspectionType = cSpotInROIContaminantInspection;
					TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->SetupInspection(TempProduct->vProductImageHeightTop, 0, 0, TempProduct->vProductImageWidth, TempProduct->vOverScanMultiplier);
					if (vLocalConfigurationData->vEjector[0].vEnabled)
						TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->vEjector = 1;
				}

				if ((vLocalConfigurationData->vScanTracType != cSoloScanTrac) && (vLocalConfigurationData->vScanTracType != cSoloPlusScanTrac))
				if (ThereIsEnoughMemory(sizeof(CInspectionContaminant), "New Inspection"))
				{
					TempProduct->vNumberOfInspections++;
					TempProduct->vInspection[TempProduct->vNumberOfInspections - 1] = new CInspectionContaminant;
					TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->vSize = 9;
					TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->vThreshold = 50;
					TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->vName = "Metal Contaminant";
					TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->vInspectionType = cThresholdContaminantInspection;
					TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->SetupInspection(TempProduct->vProductImageHeightTop, 0, 0, TempProduct->vProductImageWidth, TempProduct->vOverScanMultiplier);
					if (vLocalConfigurationData->vEjector[0].vEnabled)
						TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->vEjector = 1;
				}

				AddProduct(((CProduct *)TempProduct));
			}
		}
		CProduct *TempProductToEditPointer = NULL;
		if (TempProduct != NULL)
		if (ThereIsEnoughMemory(sizeof(CProduct), "New Product"))
			TempProductToEditPointer = new CProduct;
		if (TempProductToEditPointer != NULL)
		{
			TempProductToEditPointer->CopyProduct(TempProduct);
		}

		if (TempProductToEditPointer)
		{
			int nResponse = 0;
			CNewEditProductDialog IEditProductDialog;  
			//Set dialog box data 
			TempProductToEditPointer->SetProductName("Enter Product Name");
			IEditProductDialog.vLocalProductCollection = vLocalProductCollection;

			IEditProductDialog.vLocalConfigurationData = vLocalConfigurationData;
			IEditProductDialog.vMainWindowPointer = vMainWindowPointer;
			IEditProductDialog.vLocalSystemData = vLocalSystemData;

			//set dialog to edit copy of product
			vGlobalCurrentProduct = TempProductToEditPointer;
			if (vGlobalCurrentProduct)
				vGlobalCurrentProduct->vSetupHistoryAverageDensity = 0;
			
			//IEditProductDialog.SelectWindowForStyle = SelectProductForNew;
			//Pass control to dialog box and display
			nResponse = IEditProductDialog.DoModal();

			if (nResponse == 10)
			if (vGlobalCurrentProduct)
			{
				if (vLocalSystemData->vITIPCDig->vContainerCollectionLock.Lock())
				{
					vLocalSystemData->vITIPCDig->DeleteLearnFiles(vGlobalCurrentProduct);
					delete vGlobalCurrentProduct;
					vGlobalCurrentProduct = NULL;
					vLocalSystemData->vITIPCDig->vContainerCollectionLock.Unlock(); 
				}
			}

			if ((nResponse == IDOK) || (nResponse == 11))
			if (vGlobalCurrentProduct)
			{//if hit save before going into autosetup, need to save product
				vLocalSystemData->vCurrentProductChanged = true;
				vLocalSystemData->vCurrentProductName = *vGlobalCurrentProduct->GetProductName();
				CProduct *TempDefaultProductPointer = vMainWindowPointer->GetProductByName(vLocalSystemData->vCurrentProductName);
				if(!TempDefaultProductPointer)  //could not find product in collection
				{
					vGlobalCurrentProduct->CalculateEndOfLineTimeOut();
					AddProduct(vGlobalCurrentProduct);
				}
			}

			CDialog::EndDialog(10);
			//dialog box is now closed, if user pressed select do this
			//if user pressed cancel, do nothing
			//if (nResponse == IDOK)
		}
	}
	else
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\n\nYou cannot do this function while the system is running";
		TempNoticeDialog.vType = cNoticeMessage;
		TempNoticeDialog.DoModal();
	}
}

void CConfigureProduct::OnSubFunction8Button() 
{
  //New Product button was pressed.
  //Create dialog box
	if (vLocalSystemData->vSystemRunMode == cStoppedSystemMode)
	{
		CProduct *TempProduct = NULL;

		//for Express setup, use ~Default Express, for other new setups use ~Default
		if (((vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac) || (vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac) ||
			(vLocalConfigurationData->vScanTracType == cAllegroScanTrac) || (vLocalConfigurationData->vScanTracType == cSoloPlusScanTrac) || 
			(vLocalConfigurationData->vScanTracType == cSoloScanTrac)) && (!vLocalConfigurationData->vContinuousFeedContainerRate) && (!vLocalConfigurationData->vBulkProductMode)) //Not Continuous Feed, or a Pipeline or TD
				TempProduct = vMainWindowPointer->GetProductByName("~Default Express");
		else
			TempProduct = vMainWindowPointer->GetProductByName("~Default");

		if (!TempProduct)
		{  // no default product, so make one
			if (ThereIsEnoughMemory(sizeof(CProduct), "New default Product "))
			{
				TempProduct = new CProduct;
				TempProduct->vBodyTrigger = 1;
		
				//for Express setup, use ~Default Express, for other new setups use ~Default
				if (((vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac) || (vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac) ||
					(vLocalConfigurationData->vScanTracType == cAllegroScanTrac) || (vLocalConfigurationData->vScanTracType == cSoloPlusScanTrac) ||
					(vLocalConfigurationData->vScanTracType == cSoloScanTrac)) && (!vLocalConfigurationData->vContinuousFeedContainerRate) && (!vLocalConfigurationData->vBulkProductMode) && ((vGlobalCurrentProduct) && (!vGlobalCurrentProduct->vBulkProductMode))) //Not Continuous Feed, or a Pipeline or TD
				{
					CProduct *TempOriginalProduct = vMainWindowPointer->GetProductByName("~Default");
					if (TempOriginalProduct)
					{
						TempProduct->CopyProduct(TempOriginalProduct);  //Get any ejector position or other custom changes customer may have setup
					}
					TempProduct->SetProductName("~Default Express");
				}
				else
				{
					TempProduct->SetProductName("~Default");
					if (TempProduct->vEjectorDelayPosition[0] < 45)
						TempProduct->SetEjectorBeltPositionOffset(0,72);
					if (TempProduct->vEjectorDelayPosition[1] < 45)
						TempProduct->SetEjectorBeltPositionOffset(1,82);
					if (TempProduct->vEjectorDelayPosition[2] < 45)
						TempProduct->SetEjectorBeltPositionOffset(2,92);
					//if (TempProduct->vEjectorDelayPosition[3] < 45)
						//TempProduct->SetEjectorBeltPositionOffset(3,102);
					//if (TempProduct->vEjectorDelayPosition[4] < 45)
						//TempProduct->SetEjectorBeltPositionOffset(4,112);
					//if (TempProduct->vEjectorDelayPosition[5] < 45)
						//TempProduct->SetEjectorBeltPositionOffset(5,122);
				}

				TempProduct->vThresholdForContainerBounds = 220;
				TempProduct->vProductImageHeightTop = (float)(vLocalConfigurationData->vNumberOfXRayDetectors * vLocalConfigurationData->vDetectorLength);
				//if (vLocalConfigurationData->vUseMetric)
				//	TempProduct->vProductImageHeightTop = (WORD)(TempProduct->vProductImageHeightTop * 25.4);

				if (vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac)
					TempProduct->vProductImageWidth = 48;
				else
				if (vLocalConfigurationData->vScanTracType == cForteScanTrac)
					TempProduct->vProductImageWidth = 36;
				else
					TempProduct->vProductImageWidth = 24;
				TempProduct->vProductImageHeightBottom = 0;
				TempProduct->vProductImageHeightTop = (float)(vLocalConfigurationData->vDetectorLength * vLocalConfigurationData->vNumberOfXRayDetectors);
				//if (vLocalConfigurationData->vUseMetric)
				//	TempProduct->vProductImageHeightTop = (WORD)(TempProduct->vProductImageHeightTop * 25.4);

				if (vLocalConfigurationData->vSystemBodyTriggerToXRayBeam > 1)
				{
					TempProduct->vProductBodyTriggerToImageBeltPositionOffset = (float)(vLocalConfigurationData->vSystemBodyTriggerToXRayBeam - 4);
				}
				else
				if ((vLocalConfigurationData->vScanTracType == cSoloScanTrac) || (vLocalConfigurationData->vScanTracType == cSoloPlusScanTrac))
					TempProduct->vProductBodyTriggerToImageBeltPositionOffset = (float)(cSoloRightToLeftDistance - 2);
				else
					TempProduct->vProductBodyTriggerToImageBeltPositionOffset = 8;

				TempProduct->SetProductImageWidth(TempProduct->vProductImageWidth);

				TempProduct->SetProductLockOutWidth(TempProduct->vProductImageWidth);

				TempProduct->SetProductBodyTriggerToImageBeltPositionOffset(TempProduct->vProductBodyTriggerToImageBeltPositionOffset);

				if ((vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac) || (vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac) ||
					(vLocalConfigurationData->vScanTracType == cAllegroScanTrac) ||
					(vLocalConfigurationData->vScanTracType == cBriosoScanTrac) || (vLocalConfigurationData->vContinuousFeedContainerRate) || (vLocalConfigurationData->vBulkProductMode) || ((vGlobalCurrentProduct) && (vGlobalCurrentProduct->vBulkProductMode))) //Not Continuous Feed, or a Pipeline or TD
				{
					TempProduct->SetEdgeLocationBottom((float)(TempProduct->vProductImageHeightTop * 0.2));
					TempProduct->SetEdgeLocationHeight((float)(TempProduct->vProductImageHeightTop * 0.8));
				}
				else
				{
					TempProduct->SetEdgeLocationBottom((float)(1));
					TempProduct->SetEdgeLocationHeight((float)(1));
				}

				TempProduct->vEndOfLineTimeOut = (float)(50);
				TempProduct->SetEndOfLineTimeOut(TempProduct->vEndOfLineTimeOut);

				TempProduct->vTooManyRejectsAlarmX[0] = 5;
				TempProduct->vTooManyRejectsAlarmY[0] = 5;
				TempProduct->vTooManyRejectsAlarmDisableEjectors[0] = true;

				if ((vLocalConfigurationData->vScanTracType == cSoloScanTrac) || (vLocalConfigurationData->vScanTracType == cSoloPlusScanTrac))
				{
					TempProduct->vXRaySourceVoltage = (float)(70.0 / 6.0);  //good values for Solo Express Auto Setup
					TempProduct->vXRaySourceCurrent = 1;
					TempProduct->vXRayIntegrationTime = 1500;  //.6 milli second
				}
				else
				//if (vLocalConfigurationData->vScanTracType == cAtempoScanTrac)
				{
					TempProduct->vXRaySourceVoltage = (float)(43.0 / 6.0);  //good values for TD Express Auto Setup
					TempProduct->vXRaySourceCurrent = 3;
					TempProduct->vXRayIntegrationTime = 1500;  //.6 milli second
				}

				if (vLocalConfigurationData->vMaximumCurrent < TempProduct->vXRaySourceCurrent)
					TempProduct->vXRaySourceCurrent = (float)vLocalConfigurationData->vMaximumCurrent;

				if (vLocalConfigurationData->vMaximumVoltage < TempProduct->vXRaySourceVoltage)
					TempProduct->vXRaySourceVoltage = (float)vLocalConfigurationData->vMaximumVoltage;

				TempProduct->CalculateEndOfLineTimeOut();
				if (TempProduct->vNumberOfInspections == 0) //if don't have any inspections, make normal ones
				{
				if ((vLocalConfigurationData->vScanTracType == cSoloScanTrac) || (vLocalConfigurationData->vScanTracType == cSoloPlusScanTrac))
					if (ThereIsEnoughMemory(sizeof(CInspectionSize), "New Inspection"))
					{
						TempProduct->vNumberOfInspections++;
						TempProduct->vInspection[TempProduct->vNumberOfInspections - 1] = new CInspectionSize;
						TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->vName = "Missing Lid";
						TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->vThreshold = 50;
						TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->vSize = 20;
						TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->vInspectionType = cMissingLidInspectionOnSize;
						TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->SetupInspection(TempProduct->vProductImageHeightTop,
							TempProduct->vProductImageHeightBottom, 0, TempProduct->vProductImageWidth, TempProduct->vOverScanMultiplier);
						if (vLocalConfigurationData->vEjector[0].vEnabled)
							TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->vEjector = 1;
					}
					if (ThereIsEnoughMemory(sizeof(CInspectionDensity), "New Inspection"))
					{
						TempProduct->vNumberOfInspections++;
						TempProduct->vInspection[TempProduct->vNumberOfInspections - 1] = new CInspectionDensity;
						TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->vHasDensityUpperThreshold = true;
						TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->vDensityThresholdUpper = 240;
						TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->vName = "Underfill";
						TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->vInspectionType = cUnderfillInspection;
						TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->SetupInspection(TempProduct->vProductImageHeightTop,
							TempProduct->vProductImageHeightBottom, 0, TempProduct->vProductImageWidth, TempProduct->vOverScanMultiplier);
						if (vLocalConfigurationData->vEjector[0].vEnabled)
							TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->vEjector = 1;
					}

				if ((vLocalConfigurationData->vScanTracType == cSoloScanTrac) || (vLocalConfigurationData->vScanTracType == cSoloPlusScanTrac))
					if (ThereIsEnoughMemory(sizeof(CInspectionContaminant), "New Inspection"))
					{
						TempProduct->vNumberOfInspections++;
						TempProduct->vInspection[TempProduct->vNumberOfInspections - 1] = new CInspectionContaminant;
						TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->vAddStructure = true;
						TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->vEnhanceMethod = 3;
						TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->vThreshold = 240;
						TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->vSize = 10;
						TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->vName = "Foreign Material";
						TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->vInspectionType = cBottomContaminantInspection;
						TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->SetupInspection(TempProduct->vProductImageHeightTop,
							TempProduct->vProductImageHeightBottom, 0, TempProduct->vProductImageWidth, TempProduct->vOverScanMultiplier);
						if (vLocalConfigurationData->vEjector[0].vEnabled)
							TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->vEjector = 1;
					}

				if ((vLocalConfigurationData->vScanTracType == cSoloScanTrac) || (vLocalConfigurationData->vScanTracType == cSoloPlusScanTrac))
					if (ThereIsEnoughMemory(sizeof(CInspectionContaminant), "New Inspection"))
					{
						TempProduct->vNumberOfInspections++;
						TempProduct->vInspection[TempProduct->vNumberOfInspections - 1] = new CInspectionContaminant;
						TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->vAddStructure = true;
						TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->vEnhanceMethod = 3;
						TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->vThreshold = 220;
						TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->vSize = 10;
						TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->vName = "Top FM";
						TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->vInspectionType = cTopContaminantInspection;
						TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->SetupInspection(TempProduct->vProductImageHeightTop,
							TempProduct->vProductImageHeightBottom, 0, TempProduct->vProductImageWidth, TempProduct->vOverScanMultiplier);
						if (vLocalConfigurationData->vEjector[0].vEnabled)
							TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->vEjector = 1;
					}

					if ((vLocalConfigurationData->vScanTracType != cSoloScanTrac) && (vLocalConfigurationData->vScanTracType != cSoloPlusScanTrac))
					if (ThereIsEnoughMemory(sizeof(CInspectionContaminant), "New Inspection"))
					{
						TempProduct->vNumberOfInspections++;
						TempProduct->vInspection[TempProduct->vNumberOfInspections - 1] = new CInspectionContaminant;
						TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->vSize = 2;
						TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->vFindSpotInSubROI = 10;
						TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->vPixelsStepToNextROI = 3;
						TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->vDesensitizeEdgePixels = 30;
						TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->vThreshold = 240;
						TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->vName = "Small Contaminant";
						TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->vInspectionType = cSpotInROIContaminantInspection;
						TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->SetupInspection(TempProduct->vProductImageHeightTop, 0, 0, TempProduct->vProductImageWidth, TempProduct->vOverScanMultiplier);
						if (vLocalConfigurationData->vEjector[0].vEnabled)
							TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->vEjector = 1;
					}

					if ((vLocalConfigurationData->vScanTracType != cSoloScanTrac) && (vLocalConfigurationData->vScanTracType != cSoloPlusScanTrac))
					if (ThereIsEnoughMemory(sizeof(CInspectionContaminant), "New Inspection"))
					{
						TempProduct->vNumberOfInspections++;
						TempProduct->vInspection[TempProduct->vNumberOfInspections - 1] = new CInspectionContaminant;
						TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->vSize = 8;
						TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->vPreSmoothImage = 3;
						TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->vFindSpotInSubROI = 15;
						TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->vPixelsStepToNextROI = 3;
						TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->vDesensitizeEdgePixels = 30;
						TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->vThreshold = 240;
						TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->vName = "Medium Contaminant";
						TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->vInspectionType = cSpotInROIContaminantInspection;
						TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->SetupInspection(TempProduct->vProductImageHeightTop, 0, 0, TempProduct->vProductImageWidth, TempProduct->vOverScanMultiplier);
						if (vLocalConfigurationData->vEjector[0].vEnabled)
							TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->vEjector = 1;
					}

					if ((vLocalConfigurationData->vScanTracType != cSoloScanTrac) && (vLocalConfigurationData->vScanTracType != cSoloPlusScanTrac))
					if (ThereIsEnoughMemory(sizeof(CInspectionContaminant), "New Inspection"))
					{
						TempProduct->vNumberOfInspections++;
						TempProduct->vInspection[TempProduct->vNumberOfInspections - 1] = new CInspectionContaminant;
						TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->vSize = 9;
						TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->vThreshold = 50;
						TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->vName = "Metal Contaminant";
						TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->vInspectionType = cThresholdContaminantInspection;
						TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->SetupInspection(TempProduct->vProductImageHeightTop, 0, 0, TempProduct->vProductImageWidth, TempProduct->vOverScanMultiplier);
						if (vLocalConfigurationData->vEjector[0].vEnabled)
							TempProduct->vInspection[TempProduct->vNumberOfInspections - 1]->vEjector = 1;
					}
				}

				AddProduct(((CProduct *)TempProduct));
			}
		}
		CProduct *TempProductToEditPointer = NULL;
		if (TempProduct != NULL)
		if (ThereIsEnoughMemory(sizeof(CProduct), "New Product"))
			TempProductToEditPointer = new CProduct;
		if (TempProductToEditPointer != NULL)
		{
			TempProductToEditPointer->CopyProduct(TempProduct);
		}

		if (TempProductToEditPointer)
		{
			int nResponse = 0;
			if (((vLocalConfigurationData->vScanTracType == cCaseInspectorScanTrac) || (vLocalConfigurationData->vScanTracType == cAtempoScanTrac) || (vLocalConfigurationData->vScanTracType == cForteScanTrac) ||
				(vLocalConfigurationData->vScanTracType == cAllegroScanTrac) || (vLocalConfigurationData->vScanTracType == cSoloScanTrac) || (vLocalConfigurationData->vScanTracType == cSoloPlusScanTrac)) && 
				(!vLocalConfigurationData->vContinuousFeedContainerRate) && (!vLocalConfigurationData->vBulkProductMode)) //Not Continuous Feed, or a Pipeline or TD
			{// do express setup
				CTDAutoSetupImageDisplayDialog ITDAutoSetupImageDisplayDialog;  
				//Set dialog box data 
				TempProductToEditPointer->SetProductName("Enter Product Name");
				ITDAutoSetupImageDisplayDialog.vLocalProductCollection = vLocalProductCollection;

				ITDAutoSetupImageDisplayDialog.vLocalConfigurationData = vLocalConfigurationData;
				ITDAutoSetupImageDisplayDialog.vMainWindowPointer = vMainWindowPointer;
				ITDAutoSetupImageDisplayDialog.vLocalSystemData = vLocalSystemData;

				//set dialog to edit copy of product
				vGlobalCurrentProduct = TempProductToEditPointer;
				if (vGlobalCurrentProduct)
					vGlobalCurrentProduct->vSetupHistoryAverageDensity = 0;
			
				//IEditProductDialog.SelectWindowForStyle = SelectProductForNew;
				//Pass control to dialog box and display
				nResponse = ITDAutoSetupImageDisplayDialog.DoModal();

				if (nResponse == 10)
				if (vGlobalCurrentProduct)
				{
					if (vLocalSystemData->vITIPCDig->vContainerCollectionLock.Lock())
					{
						vLocalSystemData->vITIPCDig->DeleteLearnFiles(vGlobalCurrentProduct);
						delete vGlobalCurrentProduct;
						vGlobalCurrentProduct = NULL;
						vLocalSystemData->vITIPCDig->vContainerCollectionLock.Unlock(); 
					}
				}
			}
			else
			{
				CNewEditProductDialog IEditProductDialog;  
				//Set dialog box data 
				TempProductToEditPointer->SetProductName("Enter Product Name");
				IEditProductDialog.vLocalProductCollection = vLocalProductCollection;

				IEditProductDialog.vLocalConfigurationData = vLocalConfigurationData;
				IEditProductDialog.vMainWindowPointer = vMainWindowPointer;
				IEditProductDialog.vLocalSystemData = vLocalSystemData;

				//set dialog to edit copy of product
				vGlobalCurrentProduct = TempProductToEditPointer;
				if (vGlobalCurrentProduct)
					vGlobalCurrentProduct->vSetupHistoryAverageDensity = 0;
			
				//IEditProductDialog.SelectWindowForStyle = SelectProductForNew;
				//Pass control to dialog box and display
				nResponse = IEditProductDialog.DoModal();

				if (nResponse == 10)
				if (vGlobalCurrentProduct)
				{
					if (vLocalSystemData->vITIPCDig->vContainerCollectionLock.Lock())
					{
						vLocalSystemData->vITIPCDig->DeleteLearnFiles(vGlobalCurrentProduct);
						delete vGlobalCurrentProduct;
						vGlobalCurrentProduct = NULL;
						vLocalSystemData->vITIPCDig->vContainerCollectionLock.Unlock(); 
					}
				}
			}

			if ((nResponse == IDOK) || (nResponse == 11))
			if (vGlobalCurrentProduct)
			{//if hit save before going into autosetup, need to save product
				vLocalSystemData->vCurrentProductChanged = true;
				vLocalSystemData->vCurrentProductName = *vGlobalCurrentProduct->GetProductName();
				CProduct *TempDefaultProductPointer = vMainWindowPointer->GetProductByName(vLocalSystemData->vCurrentProductName);
				if(!TempDefaultProductPointer)  //could not find product in collection
				{
					vGlobalCurrentProduct->CalculateEndOfLineTimeOut();
					AddProduct(vGlobalCurrentProduct);
				}
			}

			CDialog::EndDialog(10);
			//dialog box is now closed, if user pressed select do this
			//if user pressed cancel, do nothing
			//if (nResponse == IDOK)
		}
	}
	else
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\n\nYou cannot do this function while the system is running";
		TempNoticeDialog.vType = cNoticeMessage;
		TempNoticeDialog.DoModal();
	}
}

CProduct *CConfigureProduct::GetProductByNumber(int TempProductNumber)
{
  int TempInt = vLocalProductCollection->GetUpperBound();
	if (TempProductNumber < 1 || TempProductNumber > vLocalProductCollection->GetUpperBound() + 1)
		return NULL;
	return vLocalProductCollection->GetAt(TempProductNumber - 1);
}

void CConfigureProduct::AddProduct(CProduct *TempProduct)
{
	if (vLocalProductCollection->GetSize() > 0)
	{
		int TempIndex = 0;
		CString *NewProductName = TempProduct->GetProductName();
		CString *CompareProductName = vLocalProductCollection->GetAt(TempIndex)->GetProductName();
		while ((*NewProductName > *CompareProductName) &&
			(TempIndex < vLocalProductCollection->GetSize()))
		{
			TempIndex++;
			if (TempIndex <= vLocalProductCollection->GetUpperBound())
		  CompareProductName = vLocalProductCollection->GetAt(TempIndex)->GetProductName();
		}
	  vLocalProductCollection->InsertAt(TempIndex, TempProduct);
	}
	else
	  vLocalProductCollection->Add(TempProduct);
}

BOOL CConfigureProduct::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	CWnd * TempWindow = NULL;

	if (pMsg->message == WM_KEYDOWN)
	{
		if (vGlobalShiftKeyDown)
		{
			if (pMsg->wParam == 109) // - Key - Sub Function 7  Decimal Point:
				OnFunction3Button();

			PostThreadMessage(vGlobalCallingThreadID,cHiddenKeyPressed, 0, pMsg->wParam);
			UpdateButtons();
		}
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
				UpdateButtons();
			break;
			case cPageDown: // Help
				PostThreadMessage(vGlobalCallingThreadID, cShowHelp, 4, 0);
				{
					TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText1);
					if (TempWindow)
						TempWindow->Invalidate(false);
				}
			break;
			case cPageUp: // Clear, but do inspx hidden dialog
			{
				if (vGlobalPasswordLevel)
				{
					PasswordOK(cNoPassword, false);
					UpdateButtons();
				}
				else
				{
					if (PasswordOK(cAnyPasswordToEnter, true))
					{
						UpdateButtons();
					}
				}
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
	else
	if (pMsg->message == cUpdateTitleBarMessage)  //the hidden key was pressed or cleared so update buttons and title bar
	{
		CWnd *TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText1);
		if (TempWindow)
			TempWindow->Invalidate(false);
		UpdateButtons();
		return true;  //true indicates it has been handled, so do not process
	}
	return CDialog::PreTranslateMessage(pMsg);
}

HBRUSH CConfigureProduct::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	HWND TempDialogHwnd = this->m_hWnd;
	int TextSize;
	pDC->SetAttribDC(pDC->m_hDC);
	
	if (pWnd->GetDlgCtrlID() == IDC_Function4Button)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function4Button, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)
			pDC->SetTextColor(cGray);
		else
			pDC->SetTextColor(cBrown);
		return vGlobalButtonColorBrush;
	}

	if (pWnd->GetDlgCtrlID() == IDC_DialogTitleStaticText1)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DialogTitleStaticText, 8);  //5 is medium large
		pDC->SetTextColor(vGlobalDialogTitleColor);

		if (vGlobalShiftKeyDown)
			pDC->SetTextColor(cBlack);
		else
			pDC->SetTextColor(vGlobalDialogTitleColor);
	}
	if (pWnd->GetDlgCtrlID() == IDC_Background)
	{
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_CPProductLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_CPProductLabel, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_UserLabel)
	{
		TextSize = SetTextSizeMultilineDisplay(TempDialogHwnd, pWnd, pDC, &m_UserLabel, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
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
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function4Button, 5);  //5 is medium large
		if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cGray);
			return vGlobalButtonColorBrush;
		}
		pDC->SetBkMode(TRANSPARENT);
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
		if (PasswordOK(cTemporaryInspxPassword, false))
			return vGlobalButtonColorBrush;
		else
			return vLocalSystemData->vGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction3Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction3Button, 5);  //5 is medium large
		pDC->SetTextColor(cButtonTextColor);
		return vLocalSystemData->vYellowBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction4Button)
	{
		pDC->SetBkMode(TRANSPARENT);
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction4Button, 5);  //5 is medium large
		if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)
		{
			pDC->SetTextColor(cGray);
			return vLocalSystemData->vYellowBrush;
		}
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
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction6Button, 5);  //5 is medium large
		if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cGray);
			return vGlobalButtonColorBrush;
		}
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction7Button)
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction7Button, 5);  //5 is medium large
		if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cGray);
			return vGlobalButtonColorBrush;
		}
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBlack);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction8Button)
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction8Button, 5);  //5 is medium large
		if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cGray);
			return vGlobalButtonColorBrush;
		}
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	
	return hbr;
}

void CConfigureProduct::OnSubFunction2Button() 
{
	if (PasswordOK(cTemporaryInspxPassword, false))
	{
		FBWF_Info IFBWF_InfoDisplayDialog;
		IFBWF_InfoDisplayDialog.vLocalSystemData = vLocalSystemData;
		IFBWF_InfoDisplayDialog.vMainWindowPointer = vMainWindowPointer;
		IFBWF_InfoDisplayDialog.vLocalConfigurationData = vLocalConfigurationData;

		int nResponse = IFBWF_InfoDisplayDialog.DoModal();
		if (nResponse == 10)
			CDialog::EndDialog(10);
	}
	else
	if (HardDriveProtectionCanBeEnabled())
	{
		ULONG TempReturnVal = 0;
		if ((TempReturnVal = EnableFBWF()) == NO_ERROR)  //try to enable hard drive protection
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\n\nHard Drive Protection Will Be Enabled After Reboot";
			TempNoticeDialog.vType = cNoticeMessage;
			TempNoticeDialog.vAutoClose = 3000;
			TempNoticeDialog.DoModal();

			UpdateButtons();
		}
		else
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\nError-Disable Hard Drive Protection Did Not Work\nProtection not Enabled, rtn = %ld";
			TempNoticeDialog.vType = cNoticeMessage;
			TempNoticeDialog.DoModal();
		}
	}
}

bool CConfigureProduct::HardDriveProtectionCanBeEnabled()
{
	bool TempReturn = false;
	CString TempFileName = "\\WINDOWS\\system32\\fbwfmgr.exe";
	CFileStatus TempFileStatus;
	if (CFile::GetStatus(TempFileName, TempFileStatus))  //FBWF is installed
	{
		CString TempCString;
		ULONG TempReturnVal;
		bool TempFBWFNextEnabled = true;


		//check if enable state change in FBWF  //
		unsigned long TempCurrentSession;
		unsigned long TempNextSession;
		TempReturnVal = IsFBWFEnabled(&TempCurrentSession,&TempNextSession);

		// FBWF  Enabled, Next session Enabled
		if (TempReturnVal == NO_ERROR)		//read of enable/disable failed
		{
			TempReturn = !TempNextSession;  //if next session not enabled, allow it to be enabled
		}
	}
	return TempReturn;
}

// return call succeed or falled
ULONG CConfigureProduct::EnableFBWF(void)
{
	DWORD dwRetVal = 0;

	typedef ULONG (CALLBACK* mytry) ();
	mytry NewFbwfEnableFilter;    // Function pointer

// free Library is causing an MFC exception error.  Load Library once when start, make a global
//	HMODULE hLibNetwork = LoadLibrary( ("fbwflib.dll")); // file name of module
//	HINSTANCE hLibNetwork = LoadLibrary( ("fbwflib.dll")); // file name of module

	if (vGlobalhLibFBWFNetwork == NULL)
	{
		vGlobalhLibFBWFNetwork = LoadLibrary(_T("fbwflib.dll")); // file name of module
	}

	if (vGlobalhLibFBWFNetwork != NULL)
	{

		LPCSTR lpProcName = "FbwfEnableFilter";		// handle to DLL module // function name		 
		NewFbwfEnableFilter = (mytry)GetProcAddress(vGlobalhLibFBWFNetwork,lpProcName);  
		
		ReportErrorMessage("FBWFCP-Try to Enable Filter",cWriteToLog,0);

		if (NewFbwfEnableFilter != NULL)
		{
			dwRetVal = NewFbwfEnableFilter();
			if (dwRetVal == NO_ERROR)			//reboot only changes status of FBWF enable
				ReportErrorMessage("FBWFCP-Enable FBWFilter Worked",cWriteToLog,0);
			else
				ReportErrorMessage("FBWFCP-Enable FBWFilter DID NOT WORK",cWriteToLog,0);
//			if (dwRetVal == NO_ERROR)			reboot only changes status of FBWF enable
//				vGlobalFBWFEnable = 1;			this is 1 only after a reboot
		}
		else
			ReportErrorMessage("FBWF-NewFbwfEnableFilter is NULL",cEMailInspx,32000);
	}		
	else
		ReportErrorMessage("FBWF-vGlobalhLibFBWFNetwork is NULL",cEMailInspx,32000);

	return(dwRetVal);
}

ULONG CConfigureProduct::IsFBWFEnabled(unsigned long * currentSession,unsigned long * nextSession)
{
	DWORD dwRetVal = 0;
//	DWORD vTempStore = 0;
	*currentSession = 0;
	*nextSession = 0; 

	typedef ULONG (CALLBACK* mytry) (unsigned long * ,unsigned long * );
	mytry NewFbwfIsFilterEnabled;    // Function pointer

// free Library is causing an MFC exception error.  Load Library once when start, make a global
//	HMODULE hLibNetwork = LoadLibrary( ("fbwflib.dll")); // file name of module
//	HINSTANCE hLibNetwork = LoadLibrary( ("fbwflib.dll")); // file name of module

	if (vGlobalhLibFBWFNetwork != NULL)
	{
		LPCSTR lpProcName = "FbwfIsFilterEnabled";		// handle to DLL module // function name		 
		NewFbwfIsFilterEnabled = (mytry)GetProcAddress(vGlobalhLibFBWFNetwork,lpProcName);   

		if (NewFbwfIsFilterEnabled != NULL)
		{
			dwRetVal = NewFbwfIsFilterEnabled(currentSession, nextSession );
		}
	}		
	return(dwRetVal);
}

void CConfigureProduct::EditTheProduct(CProduct *TempProductPointer, int TempWindowTypeMode)
{
	if (ThereIsEnoughMemory(sizeof(CProduct), "Edit Product Copy"))
	{
		//set dialog to edit copy of product
		//Bring up edit box on screen and fill in data
		CNewEditProductDialog IEditProductDialog;  
		//Set dialog box data like how many products there are
		IEditProductDialog.vMainWindowPointer = vMainWindowPointer;
		IEditProductDialog.vLocalSystemData = vLocalSystemData;
		IEditProductDialog.vLocalProductCollection = vLocalProductCollection;
		IEditProductDialog.vLocalConfigurationData = vLocalConfigurationData;
		IEditProductDialog.vMainWindowPointer = vMainWindowPointer;
		if (TempWindowTypeMode == SelectProductForEdit)
		{  //if editing the product, and not current product, set to be current and load
			if (vGlobalCurrentProduct != TempProductPointer)
			{
				vGlobalCurrentProduct = TempProductPointer;
				if (vGlobalCurrentProduct)
					vMainWindowPointer->SetupProduct(vGlobalCurrentProduct, false);
			}
			IEditProductDialog.vEditExistingProduct = 1;
		}
		else
		if (TempWindowTypeMode == SelectProductForCopy)  //if was copy, copy image files
		{
			CString TempOriginalProductName("None");
			TempOriginalProductName = *(TempProductPointer->GetProductName());
			CProduct *TempProductToEditPointer = new CProduct;
			TempProductToEditPointer->CopyProduct(TempProductPointer);
			TempProductToEditPointer->vHaveManuallySetRetriggerLockout = false;

			CString TestName = "Copy of " + *TempProductPointer->GetProductName();
			CProduct *TempProductPointer = vMainWindowPointer->GetProductByName(TestName);
			WORD TempTry = 0;
			while ((TempProductPointer) && (TempTry < 9999))
			{
				TempTry++;
				TestName = "Copy " + dtoa(TempTry, 0) + " of " + TempOriginalProductName;
				TempProductPointer = vMainWindowPointer->GetProductByName(TestName);
			}

			if (!TempProductPointer)
			{
				TempProductToEditPointer->SetProductName(TestName);
				CString TempNewProductName = *(TempProductToEditPointer->GetProductName());
				vLocalSystemData->vITIPCDig->CopyLearnFiles(TempOriginalProductName,	TempNewProductName);
				vGlobalCurrentProduct = TempProductToEditPointer;
				IEditProductDialog.vEditExistingProduct = 2;
			}
			else
			{
				CNoticeDialog TempNoticeDialog;
				TempNoticeDialog.vNoticeText = "\n\n\nName Invalid, Copy Failed";
				TempNoticeDialog.vType = cErrorMessage;
				TempNoticeDialog.DoModal();
			}
		}
		else
		if (TempWindowTypeMode == SelectProductForEvaluate)
		{
			if (vGlobalCurrentProduct != TempProductPointer)
			{
				vGlobalCurrentProduct = TempProductPointer;
				if (vGlobalCurrentProduct)
					vMainWindowPointer->SetupProduct(vGlobalCurrentProduct, false);
			}
			IEditProductDialog.vEditExistingProduct = 4;
		}

		//IEditProductDialog.SelectWindowForStyle = TempWindowTypeMode;
		//Pass control to dialog box and display
		int nResponse = IEditProductDialog.DoModal();


		if (IEditProductDialog.vEditExistingProduct == 2) //copy product
		{
			if ((nResponse == 10) || (nResponse == 2))
			if (vGlobalCurrentProduct)
			{
				if (vLocalSystemData->vITIPCDig->vContainerCollectionLock.Lock())
				{
					vLocalSystemData->vITIPCDig->DeleteLearnFiles(vGlobalCurrentProduct);
					delete vGlobalCurrentProduct;
					vGlobalCurrentProduct = NULL;
					vLocalSystemData->vITIPCDig->vContainerCollectionLock.Unlock();
				}
			}

			if ((nResponse == IDOK) || (nResponse == 11))
			if (vGlobalCurrentProduct)
			{//if hit save before going into autosetup, need to save product
				vLocalSystemData->vCurrentProductChanged = true;
				vLocalSystemData->vCurrentProductName = *vGlobalCurrentProduct->GetProductName();
				CProduct *TempDefaultProductPointer = vMainWindowPointer->GetProductByName(vLocalSystemData->vCurrentProductName);
				if(!TempDefaultProductPointer)  //could not find product in collection
				{
					vGlobalCurrentProduct->CalculateEndOfLineTimeOut();
					AddProduct(vGlobalCurrentProduct);
				}
			}
		}
		Sleep(1);
		CDialog::EndDialog(10);

		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
	}
}

void CConfigureProduct::UpdateButtons()
{
	CString TempText = "-";
	if (vGlobalCurrentUser != "No User")
		TempText= "User: " +  vGlobalCurrentUser + " Level: " + cLevelName[vGlobalPasswordLevel];
	else
		TempText= "No User logged in"; //" Level: " + cLevelName[vGlobalPasswordLevel];

	SetDlgItemText(IDC_UserLabel,TempText);
	m_UserLabel.ShowWindow(SW_SHOW);

	CWnd * TempWindow = NULL;
	TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText1);
	if (TempWindow)
		TempWindow->Invalidate(false);

	if (PasswordOK(cTemporaryInspxPassword,false))
	{
		m_SubFunction2Button.ShowWindow(SW_SHOW);
		SetDlgItemText(IDC_SubFunction2Button, _T("Hard Drive Protection Menu"));

		m_Function3Button.ShowWindow(SW_SHOW);
		SetDlgItemText(IDC_Function3Button, _T("System\nSetup"));
	}
	else
	{
		if (HardDriveProtectionCanBeEnabled())
		{
			m_SubFunction2Button.ShowWindow(SW_SHOW);
			SetDlgItemText(IDC_SubFunction2Button, _T("Enable Protection on Reboot"));
		}
		else
		{
			m_SubFunction2Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_SubFunction2Button.GetRectToInvalidateArrow());
		}

		this->InvalidateRect(&m_Function3Button.GetRectToInvalidateArrow());
		this->InvalidateRect(&m_Function3Button.GetRectToInvalidateArrow());
	}

	if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)
	{
		//m_Function4Button.EnableWindow(false);
		//m_SubFunction6Button.EnableWindow(false);
		//m_SubFunction4Button.EnableWindow(false);
		//m_SubFunction8Button.EnableWindow(false);
		m_SubFunction5Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		CString TempText;
		TempText.LoadString(IDS_EditWhileRunning);//"Edit\nWhile Running"
		SetDlgItemText(IDC_SubFunction5Button,TempText);
	}

	if (vLocalSystemData->vSystemRunMode  == cStoppedSystemMode)
	{
		m_SubFunction3Button.ShowWindow(SW_SHOW);
	}
	else
	{
		m_SubFunction3Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction3Button.GetRectToInvalidateArrow());
	}
}



void CConfigureProduct::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
  if (nIDEvent == vOneSecondTimerHandle)
  {
		OneSecondUpdate();
	}

	CDialog::OnTimer(nIDEvent);
}

void CConfigureProduct::OneSecondUpdate()
{
		if (vLocalSystemData->vSystemRunMode != vOldSystemRunMode)
		{
			vOldSystemRunMode = vLocalSystemData->vSystemRunMode;
			UpdateButtons();
		}

	if (vOldCurrentUser != vGlobalCurrentUser)
	{
		vOldCurrentUser = vGlobalCurrentUser;
		UpdateButtons();
		CWnd *TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText1);
		if (TempWindow)
			TempWindow->Invalidate(false);
	}
}


void CConfigureProduct::OnDialogtitlestatictext1()
{
	BYTE TempHiddenKeys = 8;
	if (!PasswordOK(cTemporaryInspxPassword,false))
		TempHiddenKeys = 10;
	PostThreadMessage(vGlobalCallingThreadID, cDisplaySoftKeyMenu, TempHiddenKeys, TempHiddenKeys);
}
