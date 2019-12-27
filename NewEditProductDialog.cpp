//ScanTrac Side View Source File
// NewEditProductDialog.cpp : implementation file
//

#include "stdafx.h"
#include "ScanTrac.h"
#include "NewEditProductDialog.h"
#include "NumericEntryDialog.h"
#include "AlphaEntryDialog.h"
#include "EasySelectProductDialog.h"
#include "NoticeDialog.h"
#include "ConfigureProductBodyTriggerDialog.h"
#include "NewConfigureProductExternalDetectorDialog.h"
#include "NewConfigProductEjectorDialog.h"
#include "AutoSetupImageDisplayDialog.h"
#include "SelectItemDialog.h"
#include "SelectMultipleItemDialog.h"
#include "SerialPort.h"
#include "YesNoDialog.h"
#include "SpellmanDXMXRaySourcePort.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewEditProductDialog dialog
extern CSpellmanDXMXRaySourcePort *vGlobalDXMXRayPort;
extern BYTE vGlobalShiftKeyDown;
extern CSerialPort *vGlobaluCSerialPort;
extern CProduct *vGlobalCurrentProduct;
extern CString vGlobalCurrentDirectory;
extern unsigned long vGlobalDialogTitleColor;

CNewEditProductDialog::CNewEditProductDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CNewEditProductDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewEditProductDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	vMainWindowPointer = NULL;
	vChangeMade = false;
	vEditExistingProduct = 0;
	vOpenNewWindowTimerHandle = 79;

	vLocalCWndCollection.Add(this);
	vLocalCWndCollection.Add(&m_Function5Button);
	vLocalCWndCollection.Add(&m_SubFunction8Button);
	vLocalCWndCollection.Add(&m_SubFunction3Button);
	vLocalCWndCollection.Add(&m_SubFunction1Button);
	vLocalCWndCollection.Add(&m_SubFunction2Button);
	vLocalCWndCollection.Add(&m_SubFunction5Button);
	vLocalCWndCollection.Add(&m_Function4Button);
	vLocalCWndCollection.Add(&m_Function2Button);
	vLocalCWndCollection.Add(&m_Background);
	vLocalCWndCollection.Add(&m_SubFunction7Button);
	vLocalCWndCollection.Add(&m_BodyTriggerToImager);
	vLocalCWndCollection.Add(&m_DialogTitleStaticText);
	vLocalCWndCollection.Add(&m_EndOfLine);
	vLocalCWndCollection.Add(&m_Function1Button);
	vLocalCWndCollection.Add(&m_Function3Button);
	vLocalCWndCollection.Add(&m_ProductName);
	vLocalCWndCollection.Add(&m_SubFunction4Button);
	vLocalCWndCollection.Add(&m_SubFunction6Button);
}

void CNewEditProductDialog::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	m_Background.SetWindowPos(NULL,cMainBackgroundLeft, cMainBackgroundTop, cMainBackgroundLength, cMainBackgroundHeight, SWP_NOZORDER);
	vLocalSystemData->vITIPCDig->DeleteBackupLearnFiles();
	vProductNameString = *(vGlobalCurrentProduct->GetProductName());
	SetDlgItemText(IDC_ProductName,vProductNameString);


	vNumberOfBodyTriggersInSystem = 0;
	for (BYTE TempLoop = 0; TempLoop < cNumberOfBodyTriggers; TempLoop++)
	{
		if ((vLocalConfigurationData->vBodyTrigger[TempLoop].vEnabled) &&
			(vLocalConfigurationData->vBodyTrigger[TempLoop].vDigitalInputLine))
			vNumberOfBodyTriggersInSystem++;
	}

	CString TempText;
	TempText.LoadString(IDS_EditProductSetup);
	SetDlgItemText(IDC_DialogTitleStaticText1,TempText);

	TempText.LoadString(IDS_SetupProductName);
	SetDlgItemText(IDC_SubFunction1Button,TempText);

	TempText.LoadString(IDS_Size);
	SetDlgItemText(IDC_SubFunction2Button,TempText);

	TempText.LoadString(IDS_Exposure);
	SetDlgItemText(IDC_SubFunction3Button,TempText);

	TempText.LoadString(IDS_Learn);
	SetDlgItemText(IDC_SubFunction4Button,TempText);

	TempText.LoadString(IDS_Inspection);
	SetDlgItemText(IDC_SubFunction5Button,TempText);

	TempText.LoadString(IDS_Evaluate);
	SetDlgItemText(IDC_SubFunction6Button,TempText);

	TempText.LoadString(IDS_Ejectors);
	SetDlgItemText(IDC_SubFunction8Button,TempText);

//	TempText.LoadString(IDS_SetupAuxiliaryDetectors);
	TempText = "Auxiliary Detectors";
	SetDlgItemText(IDC_Function3Button,TempText);

	TempText.LoadString(IDS_MainMenu);
	SetDlgItemText(IDC_Function1Button,TempText);

	TempText.LoadString(IDS_Exit);
	SetDlgItemText(IDC_Function2Button,TempText);

	TempText.LoadString(IDS_Triggers);
	SetDlgItemText(IDC_Function5Button,TempText);
	if (vNumberOfBodyTriggersInSystem > 1)
	{
		m_Function5Button.ShowWindow(SW_SHOW);
	}

	/*
	bool TempHaveExternalDetectorsConfigured = false;
	for (BYTE TempLoop = 0; TempLoop < cNumberOfExternalDetectors; TempLoop++)
		if (vLocalConfigurationData->vExternalDetector[TempLoop].vEnabled)
			TempHaveExternalDetectorsConfigured = true;

	if (!TempHaveExternalDetectorsConfigured)
		//m_SubFunction2Button.EnableWindow(false);
*/

	bool TempHaveEjectorConfigured = false;
	for (BYTE TempLoop = 0; TempLoop < cNumberOfEjectors; TempLoop++)
		if (vLocalConfigurationData->vEjector[TempLoop].vEnabled)
			TempHaveEjectorConfigured = true;
		
	if (vLocalSystemData->vNumberOfAuxiliaryDetectorsConfigured)
	{
		m_Function3Button.ShowWindow(SW_SHOW);
	}

	//if (!TempHaveEjectorConfigured)
	//	m_SubFunction3Button.EnableWindow(false);

	this->SetWindowText(_T("Edit"));

	vOriginalName = *vGlobalCurrentProduct->GetProductName();

	if (vProductNameString == "Enter Product Name")
	{
		//DoProductAutoSetup(0,0);
		int TempTimerResult = SetTimer(vOpenNewWindowTimerHandle,100,NULL);
		if (!TempTimerResult)
			ReportErrorMessage("Error-Open New Window Timer Failed",cEMailInspx,32000);
	}

	if (vEditExistingProduct == 4) //evaluate product
	{
		//EvaluateProduct();
		int TempTimerResult = SetTimer(vOpenNewWindowTimerHandle,100,NULL);
		if (!TempTimerResult)
			ReportErrorMessage("Error-Open New Window Timer Failed",cEMailInspx,32000);
	}
	else//edit product, so don't allow change name
	if (vEditExistingProduct == 1)
	{
	}
	else //copy product, so changed name already
	if (vEditExistingProduct == 2)
		SetChangeMade();
	::SetupMenu(vLocalCWndCollection);
}

void CNewEditProductDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewEditProductDialog)
	DDX_Control(pDX, IDC_Function5Button, m_Function5Button);
	DDX_Control(pDX, IDC_SubFunction8Button, m_SubFunction8Button);
	DDX_Control(pDX, IDC_SubFunction3Button, m_SubFunction3Button);
	DDX_Control(pDX, IDC_SubFunction1Button, m_SubFunction1Button);
	DDX_Control(pDX, IDC_SubFunction2Button, m_SubFunction2Button);
	DDX_Control(pDX, IDC_SubFunction5Button, m_SubFunction5Button);
	DDX_Control(pDX, IDC_Function4Button, m_Function4Button);
	DDX_Control(pDX, IDC_Function2Button, m_Function2Button);
	DDX_Control(pDX, IDC_Background, m_Background);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_SubFunction7Button, m_SubFunction7Button);
	DDX_Control(pDX, IDC_BodyTriggerToImager, m_BodyTriggerToImager);
	DDX_Control(pDX, IDC_DialogTitleStaticText1, m_DialogTitleStaticText);
	DDX_Control(pDX, IDC_EndOfLine, m_EndOfLine);
	DDX_Control(pDX, IDC_Function1Button, m_Function1Button);
	DDX_Control(pDX, IDC_Function3Button, m_Function3Button);
	DDX_Control(pDX, IDC_ProductName, m_ProductName);
	DDX_Control(pDX, IDC_SubFunction4Button, m_SubFunction4Button);
	DDX_Control(pDX, IDC_SubFunction6Button, m_SubFunction6Button);
}


BEGIN_MESSAGE_MAP(CNewEditProductDialog, CDialog)
	//{{AFX_MSG_MAP(CNewEditProductDialog)
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
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_SubFunction6Button, OnSubFunction6Button)
	ON_BN_CLICKED(IDC_SubFunction7Button, OnSubFunction7Button)
	ON_BN_CLICKED(IDC_SubFunction8Button, OnSubFunction8Button)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_STN_CLICKED(IDC_DialogTitleStaticText1, &CNewEditProductDialog::OnDialogtitlestatictext1)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewEditProductDialog message handlers

void CNewEditProductDialog::OnFunction1Button() 
{
	//Main Menu was pressed
	if ((!vChangeMade) || (AbortSetupChanges()))
		AbortDialog(10);
}

void CNewEditProductDialog::AbortDialog(WORD TempExitCode) 
{
	//if (vEditExistingProduct == 2)  //copied product
	//{
	//	vGlobalCurrentProduct->SetProductName(vProductNameString);
	//	vLocalSystemData->vITIPCDig->DeleteLearnFiles(vGlobalCurrentProduct);
	//}
	vLocalSystemData->vInAutoSetup = false;

	CDialog::EndDialog(TempExitCode);
}

void CNewEditProductDialog::OnFunction2Button() 
{
	//Save and exit was pressed
	vLocalSystemData->vITIPCDig->DeleteBackupLearnFiles();
	if (!vChangeMade) 
	{
		vLocalSystemData->vInAutoSetup = false;
		CDialog::EndDialog(11);
	}
	else
	if ((vProductNameString != "Default Product") && 
		(vProductNameString != "Enter Product Name") &&
		((vProductNameString != "~Default") || (PasswordOK(cTemporaryInspxPassword,false))))
	{
		if (KeepSetupChanges())
		{
			SaveAndExit();
		}
	}
	else
	{
		//Did not give product a name
		CNoticeDialog TempNoticeDialog;
		CString TempText;
		TempText.LoadString(IDS_Youmustgivetheproductanewname);//"\n\n\nYou must give the product a new name."
		TempNoticeDialog.vNoticeText = TempText;
		TempNoticeDialog.vType = cErrorMessage;
		TempNoticeDialog.DoModal();
	}
}

void CNewEditProductDialog::SaveAndExit() 
{
	if (vOriginalName != vProductNameString)
	if (vEditExistingProduct == 1)  //if editing product that was saved already must change now
	{
		int TempNumberOfProducts = vLocalProductCollection->GetSize();
		if (TempNumberOfProducts > 0)  //if have any products configured
		{
			CProduct *TempProductPointer = NULL;
			int TempNumberOfProductRenaming = 0;
			for (WORD TempLoop = 0; TempLoop < TempNumberOfProducts; TempLoop++)
			{
				TempProductPointer = vLocalProductCollection->GetAt(TempLoop);
				CString TempName = *TempProductPointer->GetProductName();
				if (TempName == vOriginalName)
				{
					TempNumberOfProductRenaming = TempLoop;
					TempLoop = TempNumberOfProducts;
				}
			}

			if ((TempProductPointer) && (*TempProductPointer->GetProductName() == vOriginalName))
			{
				//rename learn files
				vLocalSystemData->vITIPCDig->ChangeNameOfLearnFiles(vOriginalName, vProductNameString);

				vLocalProductCollection->RemoveAt(TempNumberOfProductRenaming, 1);

				CString TempNameRightNow = *vGlobalCurrentProduct->GetProductName();

				vGlobalCurrentProduct->SetProductName(vProductNameString);

				AddProduct(TempProductPointer);
				vLocalSystemData->vCurrentProductChanged = true;
				vLocalSystemData->vCurrentProductName = *vGlobalCurrentProduct->GetProductName();
			}
		}
	}
	vGlobalCurrentProduct->CalculateEndOfLineTimeOut();
	vLocalSystemData->vInAutoSetup = false;
	CDialog::EndDialog(11);
}

void CNewEditProductDialog::OnFunction3Button() 
{
	//setup Auxiliary Detectors was pressed
	if (vLocalSystemData->vNumberOfAuxiliaryDetectorsConfigured)
	{
		//DoProductAutoSetup(4,0);
		BYTE TempReturn = SetupExternalDetectors(true);
	}
}

void CNewEditProductDialog::EvaluateProduct() 
{
	//EvaluateProduct button was pressed
	if (vChangeMade)
	{
		vGlobalCurrentProduct->SetProductName(vProductNameString);
		vMainWindowPointer->CheckProductFullyConfigured(vGlobalCurrentProduct);
	}
	CAutoSetupImageDisplayDialog IAutoSetupImageDisplayDialog;
	IAutoSetupImageDisplayDialog.vLocalConfigurationData = vLocalConfigurationData;
	IAutoSetupImageDisplayDialog.vLocalSystemData = vLocalSystemData;
	IAutoSetupImageDisplayDialog.vMainWindowPointer = vMainWindowPointer;
	IAutoSetupImageDisplayDialog.vAutoSetupType = cEvaluateAutoSetup;
	IAutoSetupImageDisplayDialog.vPickOneToEvaluate = true;
	IAutoSetupImageDisplayDialog.vLocalProductCollection = vLocalProductCollection;
	//when go into auto setup, it will go into inspection setup
	IAutoSetupImageDisplayDialog.vMode = cSetContaminantThreshold;
	int nResponse = IAutoSetupImageDisplayDialog.DoModal();

	if ((nResponse == IDOK) || (nResponse == 20))
	{
		// TODO: Place code here to handle when the dialog is accepted
		if (IAutoSetupImageDisplayDialog.vAnyChangeMade)
		{
			SetChangeMade();
		}
		if (IAutoSetupImageDisplayDialog.vSavedImageProcessingData)
		{
			vLocalSystemData->vChangedSizeOfProduct = false;
			vLocalSystemData->vChangedSizeOfProductCheckDensityROI = 0;
		}
		SaveAndExit();
	}
	else
		AbortDialog(10);
}

void CNewEditProductDialog::OnFunction4Button() 
{
	//Cancel was pressed
	if (vChangeMade)
	if ((!vChangeMade) || (AbortSetupChanges()))
		CDialog::OnCancel();
}

void CNewEditProductDialog::OnFunction5Button() 
{
	//BYTE TempReturn = SetupBodyTrigger(false, vEditExistingProduct);
	if (vNumberOfBodyTriggersInSystem > 1)
		DoProductAutoSetup(1,0);
}

void CNewEditProductDialog::OnSubFunction1Button() 
{
  //Edit Product Name button was pressed
	if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\n\nSystem Must Be Stopped to change product name";
		TempNoticeDialog.vType = cErrorMessage;
		TempNoticeDialog.DoModal();
	}
	else
	{
		//Edit Product Name button was pressed
		CAlphaEntryDialog IAlphaEntryDialog;  
		
		//Set dialog box data titles and number value
		IAlphaEntryDialog.vEditString = vProductNameString;
		IAlphaEntryDialog.vLocalSystemData = vLocalSystemData;
		CString TempText;
		TempText.LoadString(IDS_ProductName);
		IAlphaEntryDialog.m_DialogTitleStaticText = TempText;
		TempText.LoadString(IDS_OriginalValue);
		IAlphaEntryDialog.m_DialogTitleStaticText2 = TempText + ": " + vProductNameString;
		IAlphaEntryDialog.vAllowDashes = true;
		IAlphaEntryDialog.vAllowPeriod = false;
		IAlphaEntryDialog.vAcceptAllKeys = false;
		if (PasswordOK(cTemporaryInspxPassword,false))
			IAlphaEntryDialog.vAllowApproximate = true;
		//Pass control to dialog box and display
		int nResponse = IAlphaEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			if (IAlphaEntryDialog.vEditString != "Enter Product Name")
			{
				//check if name is used as a product already
				CProduct *TempProductPointer = vMainWindowPointer->GetProductByName(IAlphaEntryDialog.vEditString);
				if ((TempProductPointer) && (IAlphaEntryDialog.vEditString != vProductNameString))
				{
					CNoticeDialog TempNoticeDialog;
					CString TempText;
					TempText.LoadString(IDS_ProductNameinusealreadyinputignored);//"\n\n\nProduct Name in use already, input ignored"
					TempNoticeDialog.vNoticeText = TempText;
					TempNoticeDialog.vType = cErrorMessage;
					TempNoticeDialog.DoModal();
				}
				else
				if (vProductNameString != IAlphaEntryDialog.vEditString)
				{
					if (vEditExistingProduct == 1)  //if change name of an existing product, must save and exit
					{
						CYesNoDialog TempYesNoDialog;
						CString TempText = "\nRename Product from: " + vProductNameString + " to " + IAlphaEntryDialog.vEditString;
						TempYesNoDialog.vNoticeText = TempText;
						TempYesNoDialog.vYesButtonText = "Rename Product";
						TempYesNoDialog.vNoButtonText = "Keep Original";
						TempYesNoDialog.vQuestionType = cConfirmQuestion;
						int TempResult =TempYesNoDialog.DoModal();

						if (TempResult == IDOK)
						{
							ReportErrorMessage("Operator Renamed Product from: " + vProductNameString + " to " + IAlphaEntryDialog.vEditString, cUserChanges,0);
							vOriginalName = vProductNameString;
							vProductNameString = IAlphaEntryDialog.vEditString;

							SaveAndExit();
						}
					}
					else//if a new product or copy product
					{
						vOriginalName = vProductNameString;
						vProductNameString = IAlphaEntryDialog.vEditString;
						vLocalSystemData->vITIPCDig->ChangeNameOfLearnFiles(vOriginalName, vProductNameString);
						vGlobalCurrentProduct->SetProductName(vProductNameString);
						vLocalSystemData->vITIPCDig->DeleteLearnFiles(vOriginalName);
					}
					SetChangeMade();
					SetDlgItemText(IDC_ProductName, vProductNameString);
				}
			}
		}
		else 
		if (nResponse == 10)
		{
			//Main Menu button pressed
			AbortDialog(10);
		}
	}
}

void CNewEditProductDialog::DoProductAutoSetup(BYTE TempStartStep, BYTE TempStartWindow) 
{
	//Auto Product Setup Button Pressed
	bool TempFirstTimeIn = true;
	if (vChangeMade)
	{
		vGlobalCurrentProduct->SetProductName(vProductNameString);
	}
	BYTE TempAutoSetupStep = TempStartStep;

	bool TempStepedBack = false;
	for (;TempAutoSetupStep < 50;)
	{
		switch (TempAutoSetupStep)
		{
			case 0:
			{
				if (!vEditExistingProduct)  //if a new product
				{
					//Edit Product Name button was pressed
					CAlphaEntryDialog IAlphaEntryDialog;  
					IAlphaEntryDialog.vMainWindowPointer = vMainWindowPointer;
					//Set dialog box data titles and number value
					IAlphaEntryDialog.vEditString = vProductNameString;
					IAlphaEntryDialog.vLocalSystemData = vLocalSystemData;
					CString TempText;
					TempText.LoadString(IDS_ProductName);
					IAlphaEntryDialog.m_DialogTitleStaticText = TempText;
					IAlphaEntryDialog.vAllowDashes = true;
					IAlphaEntryDialog.vAllowPeriod = false;
					IAlphaEntryDialog.vAcceptAllKeys = false;
					if (PasswordOK(cTemporaryInspxPassword,false))
						IAlphaEntryDialog.vAllowApproximate = true;

					TempText.LoadString(IDS_OriginalValue);
					IAlphaEntryDialog.m_DialogTitleStaticText2 = TempText + ": " + vProductNameString;
					//Pass control to dialog box and display
					int nResponse = IAlphaEntryDialog.DoModal();
					//dialog box is now closed, if user pressed select do this
					//if user pressed cancel, do nothing
					if (nResponse == IDOK)
					{
						//check if name is used as a product already
						CProduct *TempProductPointer = 
							vMainWindowPointer->GetProductByName(IAlphaEntryDialog.vEditString);
						if (TempProductPointer)
						{
							CNoticeDialog TempNoticeDialog;
							CString TempText;
							TempText.LoadString(IDS_ProductNameinusealreadyyoumustchangeit);//"\n\nProduct Name in use already\nyou must change it"
							TempNoticeDialog.vNoticeText = TempText;
							TempNoticeDialog.vType = cErrorMessage;
							TempNoticeDialog.DoModal();
						}
						else
						if (IAlphaEntryDialog.vEditString == "Enter Product Name")
						{
							CNoticeDialog TempNoticeDialog;
							TempText.LoadString(IDS_MustchangeProductName);//"\n\n\nMust change Product Name"
							TempNoticeDialog.vNoticeText = TempText;
							TempNoticeDialog.vType = cErrorMessage;
							TempNoticeDialog.DoModal();
						}
						else
						{  //user changed name and pressed next step
							vProductNameString = IAlphaEntryDialog.vEditString;

							SetChangeMade();
							SetDlgItemText(IDC_ProductName,vProductNameString);
							vGlobalCurrentProduct->SetProductName(vProductNameString);
							vLocalSystemData->vITIPCDig->DeleteLearnImages();
							if ((vLocalSystemData->vDriftCompensationADCOffset) ||
								(vLocalSystemData->vDriftCompensationDACOffset))
							if (vGlobaluCSerialPort)
							{
								vLocalSystemData->vDriftCompensationADCOffset = 0;
								vLocalSystemData->vDriftCompensationDACOffset = 0;
								vMainWindowPointer->SendAllADCDetectorOffsetsToZero();
							}
							vLocalSystemData->vITIPCDig->DeleteBackupLearnFiles();
							vLocalSystemData->vITIPCDig->DeleteLearnFiles(vGlobalCurrentProduct);
							TempStepedBack = false;
							TempAutoSetupStep++;
						}
						//if user did not change name, will not go on, will show name entry again
					}
					else 
					if ((nResponse == 10) ||(nResponse == 2))
					{  //user pressed main menu
						TempAutoSetupStep = 255;
					}
					else  //user previous menu
						TempAutoSetupStep = 100;//save and exit
				}
				else
				{
					if (TempStepedBack)
					{  //user previous menu
						TempAutoSetupStep = 100; //save and exit
					}
					else
					{
						TempStepedBack = false;
						TempAutoSetupStep++;
					}
				}
			}
			break;
			case 1:
			{
				BYTE TempNumberBodyTriggersConfigured = 0;
				BYTE TempBodyTriggerNumber = 0;
				for (BYTE TempLoop = 0; TempLoop < cNumberOfBodyTriggers; TempLoop++)
				{
					if ((vLocalConfigurationData->vBodyTrigger[TempLoop].vEnabled) &&
						(vLocalConfigurationData->vBodyTrigger[TempLoop].vLineBitMask))
					{
						TempNumberBodyTriggersConfigured++;
						TempBodyTriggerNumber = TempLoop;
					}
				}
				if (TempNumberBodyTriggersConfigured == 0)
				{
					CNoticeDialog TempNoticeDialog;
					TempNoticeDialog.vNoticeText = "\n\nNo Container Trigger Configured.\nYou Must configure an ejector in the System Setup";
					TempNoticeDialog.vType = cErrorMessage;
					TempNoticeDialog.DoModal();
					TempAutoSetupStep = 200;
				}
				else
				if ((TempNumberBodyTriggersConfigured == 1) && (TempStartStep != 1))
				{
					vGlobalCurrentProduct->vBodyTrigger = TempBodyTriggerNumber + 1;
					if (TempStepedBack)
						TempAutoSetupStep--;
					else
						TempAutoSetupStep++;
				}
				else
				{
					BYTE TempReturn = SetupBodyTrigger(true, vEditExistingProduct);
					//if editing existing product, show "Exit" on body trigger menu, not previous setup
					if (TempReturn == 1)
					{
						TempStepedBack = false;
						TempAutoSetupStep++;
					}
					else
					if (TempReturn == 11)
					{
						TempAutoSetupStep = 100;
					}
					else
					if (TempReturn == 0)
					{
						TempStepedBack = true;
						TempAutoSetupStep--;
					}
					else
						TempAutoSetupStep = 255;
				}
			}
			break;
			case 2:
			{
				CAutoSetupImageDisplayDialog IAutoSetupImageDisplayDialog;
				IAutoSetupImageDisplayDialog.vLocalConfigurationData = vLocalConfigurationData;
				IAutoSetupImageDisplayDialog.vLocalSystemData = vLocalSystemData;
				IAutoSetupImageDisplayDialog.vMainWindowPointer = vMainWindowPointer;

				IAutoSetupImageDisplayDialog.vLocalProductCollection = vLocalProductCollection;
				if (vEditExistingProduct == 1)  //edit
					IAutoSetupImageDisplayDialog.vAutoSetupType = cProductAutoSetup;
				else
				if (vEditExistingProduct == 2) //copy
				{
					IAutoSetupImageDisplayDialog.vAutoSetupType = cCopyAutoSetup;
				}
				else
				if (vEditExistingProduct == 4) //eval
					IAutoSetupImageDisplayDialog.vAutoSetupType = cEvaluateAutoSetup;
				else //new
					IAutoSetupImageDisplayDialog.vAutoSetupType = cNewAutoSetup;

				if (TempStepedBack)
				{
					IAutoSetupImageDisplayDialog.vPickOneToEvaluate = false;
					IAutoSetupImageDisplayDialog.vMode = cSetInspections;
				}
				else
					IAutoSetupImageDisplayDialog.vMode = cSetImageSize;

				if ((TempStartStep == 2) && (TempFirstTimeIn))
				{
					TempFirstTimeIn = false;
					IAutoSetupImageDisplayDialog.vMode = TempStartWindow;
					if (TempStartWindow == cSetVoidThreshold)
						IAutoSetupImageDisplayDialog.vPickOneToEvaluate = true;
				}

				TempStepedBack = false;
				if (!vEditExistingProduct)  //new product
				{
					IAutoSetupImageDisplayDialog.vHaveNotSetSize = true;
					//set standard integration time and voltage
					vGlobalCurrentProduct->vThresholdForContainerBounds = 240;
					CProduct *TempCalibrateProduct = vMainWindowPointer->GetProductByName("~Calibrate Gain");
					if ((TempCalibrateProduct) && (vGlobalCurrentProduct))
					{
						// want integration time from ~Default not  this one vGlobalCurrentProduct->vXRayIntegrationTime = TempCalibrateProduct->vXRayIntegrationTime;
						vGlobalCurrentProduct->vXRaySourceCurrent = 5;

						if (vLocalConfigurationData->vHVPSType == cCheyneyMonoBlock)
						{
								if (vGlobalCurrentProduct->vXRaySourceCurrent > vGlobalDXMXRayPort->vCurrentMaximumScale)
										vGlobalCurrentProduct->vXRaySourceCurrent = (float)vGlobalDXMXRayPort->vCurrentMaximumScale;
								if (vGlobalCurrentProduct->vXRaySourceCurrent > 20)
										vGlobalCurrentProduct->vXRaySourceCurrent = 20;
						}

						if (vLocalConfigurationData->vHVPSType == c100WVJTMonoBlockHVPS)
						if (vGlobalCurrentProduct->vXRaySourceCurrent > 2.5)
								vGlobalCurrentProduct->vXRaySourceCurrent = 2.5;

						if (vLocalConfigurationData->vHVPSType == c100WMonoBlockHVPS)
						if (vGlobalCurrentProduct->vXRaySourceCurrent > 2)
							vGlobalCurrentProduct->vXRaySourceCurrent = 2;
						if (vLocalConfigurationData->vHVPSType == c210WMonoBlockHVPS)
						if (vGlobalCurrentProduct->vXRaySourceCurrent > 3)
							vGlobalCurrentProduct->vXRaySourceCurrent = 3;
						if (vLocalConfigurationData->vHVPSType == cMNXHVPS)
						if (vGlobalCurrentProduct->vXRaySourceCurrent > 1)
							vGlobalCurrentProduct->vXRaySourceCurrent = 1;
					}
				}
				if (vEditExistingProduct == 2) //copy
					vGlobalCurrentProduct->vThresholdForContainerBounds = 240;

				int nResponse = IAutoSetupImageDisplayDialog.DoModal();
				if ((nResponse == IDOK) || (nResponse == 20))
				{
					if (IAutoSetupImageDisplayDialog.vAnyChangeMade)
						SetChangeMade();
					TempStepedBack = false;
					TempAutoSetupStep++;
					if (IAutoSetupImageDisplayDialog.vSavedImageProcessingData)
					{
						vLocalSystemData->vChangedSizeOfProduct = false;
						vLocalSystemData->vChangedSizeOfProductCheckDensityROI = 0;
					}
					//save and exit
					TempAutoSetupStep = 100;
				}
				else
				if (nResponse == 10)
				{
					AbortDialog(10);
					TempAutoSetupStep = 255;
				}
				else
				{
					TempStepedBack = true;
					TempAutoSetupStep--;
				}
			}
			break;
			case 3:
			{
				bool TempHaveEjectorConfigured = false;
				for (BYTE TempLoop = 0; TempLoop < cNumberOfEjectors; TempLoop++)
				{
					if (vLocalConfigurationData->vEjector[TempLoop].vEnabled)
						TempHaveEjectorConfigured = true;
				}
				if (TempHaveEjectorConfigured)
				{
					BYTE TempReturn = SetupProductEjectors(true, false);
					if ((TempReturn == IDOK) || (TempReturn == 20))
					{
						TempStepedBack = false;
						TempAutoSetupStep++;
						if (TempReturn == 20)
							TempAutoSetupStep = 100;
					}
					else
					if (TempReturn == 0)
					{
						TempAutoSetupStep--;
						TempStepedBack = true;
					}
					else
						TempAutoSetupStep = 255;
				}
				else
				{
					if (TempStepedBack)
						TempAutoSetupStep--;
					else
						TempAutoSetupStep++;
				}
			}
			break;
			case 4:
			{
				bool TempHaveExternalDetectorsConfigured = false;
				for (BYTE TempLoop = 0; TempLoop < cNumberOfExternalDetectors; TempLoop++)
					if (vLocalConfigurationData->vExternalDetector[TempLoop].vEnabled)
						TempHaveExternalDetectorsConfigured = true;

				if (TempHaveExternalDetectorsConfigured)
				{
					BYTE TempReturn = SetupExternalDetectors(true);
					if (TempReturn == 1)
						TempAutoSetupStep = 100;
					else
					if (TempReturn == 2)
					{
						TempStepedBack = true;
						TempAutoSetupStep--;
					}
					else
						TempAutoSetupStep = 255;
				}
				else
				{
					TempAutoSetupStep = 100;
				}
			}
			break;
			default:
					TempAutoSetupStep = 255;
		}
	}
	if (TempAutoSetupStep == 100)
		//if use completed auto setup completely, then save and exit
			SaveAndExit();
	else
	if (TempAutoSetupStep == 255)
		//if use completed auto setup completely, then abort and exit
		AbortDialog(10);
	else
		EndDialog(11);
}

BYTE CNewEditProductDialog::SetupBodyTrigger(bool TempInAutoSetup, BYTE TempShowExit)
{
	bool TempHaveBodyTriggersConfigured = false;
	for (BYTE TempLoop = 0; TempLoop < cNumberOfBodyTriggers; TempLoop++)
		if (vLocalConfigurationData->vBodyTrigger[TempLoop].vEnabled)
			TempHaveBodyTriggersConfigured = true;
		
	if (TempHaveBodyTriggersConfigured)
	{
		//Create dialog box
		CConfigureProductBodyTriggerDialog IConfigureProductBodyTriggerDialog;  
		
		//Set dialog box data like how many products there are
		vGlobalCurrentProduct->SetProductName(vProductNameString);
		IConfigureProductBodyTriggerDialog.vCPBTLocalCurrentProductPointer = vGlobalCurrentProduct;
		IConfigureProductBodyTriggerDialog.vLocalConfigurationData = vLocalConfigurationData;
		IConfigureProductBodyTriggerDialog.vLocalSystemData = vLocalSystemData;
		IConfigureProductBodyTriggerDialog.vInAutoSetup = TempInAutoSetup;
		IConfigureProductBodyTriggerDialog.vShowExit = TempShowExit;

		//Pass control to dialog box and display
		//IEditProductDialog.vEPLocalNumberOfProducts = vCPLocalNumberOfProducts;
		int nResponse = IConfigureProductBodyTriggerDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if ((nResponse == IDOK) || (nResponse == 11))
		{
			// TODO: Place code here to handle when the dialog is accepted

			if (IConfigureProductBodyTriggerDialog.vChangeMade)
				SetChangeMade();
			return nResponse;
		}
		else 
		if (nResponse == 10)
		{
			//  Main Menu button was pressed
			AbortDialog(10);
			return 10;
		}
		else 
			return 0;
	}
	else
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\n\nNo Container Triggers configured in System.";
		TempNoticeDialog.vType = cErrorMessage;
		TempNoticeDialog.DoModal();
		return 0;
	}
}

void CNewEditProductDialog::OnSubFunction2Button() 
{
	//Auto Size Button Pressed
	DoProductAutoSetup(2,cSetImageSize);
	/*
	if (vChangeMade)
	{
		vGlobalCurrentProduct->SetProductName(vProductNameString);
		CheckProductFullyConfigured(vGlobalCurrentProduct,vLocalConfigurationData);
	}
	CAutoSetupImageDisplayDialog IAutoSetupImageDisplayDialog;
	IAutoSetupImageDisplayDialog.vLocalConfigurationData = vLocalConfigurationData;
	IAutoSetupImageDisplayDialog.vLocalSystemData = vLocalSystemData;
	IAutoSetupImageDisplayDialog.vMainWindowPointer = vMainWindowPointer;
	IAutoSetupImageDisplayDialog.vGlobalCurrentProduct = vGlobalCurrentProduct;
	IAutoSetupImageDisplayDialog.vAutoSetupType = cEditSizeAutoSetup;
	IAutoSetupImageDisplayDialog.vMode = cSetImageSize;
	IAutoSetupImageDisplayDialog.vLocalProductCollection = vLocalProductCollection;

	BYTE TempEnableEjectors = vLocalConfigurationData->vEnableEjectors;
	vLocalConfigurationData->vEnableEjectors = false;
	int nResponse = IAutoSetupImageDisplayDialog.DoModal();
	vLocalConfigurationData->vEnableEjectors = TempEnableEjectors;

	if ((nResponse == IDOK) || (nResponse == 0))
	{
		// TODO: Place code here to handle when the dialog is accepted
		if (IAutoSetupImageDisplayDialog.vAnyChangeMade)
		{
			SetChangeMade();
		}
		if (IAutoSetupImageDisplayDialog.vSavedImageProcessingData)
			vLocalSystemData->vChangedSizeOfProduct = false;
		OnFunction2Button();
	}
	else
	if (nResponse == 10)
		AbortDialog(10);
		*/
}

BYTE CNewEditProductDialog::SetupExternalDetectors(bool TempInAutoSetup)
{	//Create dialog box

	bool TempHaveExternalDetectorsConfigured = false;
	for (BYTE TempLoop = 0; TempLoop < cNumberOfExternalDetectors; TempLoop++)
		if (vLocalConfigurationData->vExternalDetector[TempLoop].vEnabled)
			TempHaveExternalDetectorsConfigured = true;

	if (TempHaveExternalDetectorsConfigured)
	{
		CNewConfigureProductExternalDetectorDialog IConfigureProductExternalDetectorDialog;  
		
		//Set dialog box data like how many products there are
		vGlobalCurrentProduct->SetProductName(vProductNameString);
		IConfigureProductExternalDetectorDialog.vLocalCurrentProductPointer = vGlobalCurrentProduct;
		IConfigureProductExternalDetectorDialog.vLocalConfigurationData = vLocalConfigurationData;
		IConfigureProductExternalDetectorDialog.vLocalSystemData = vLocalSystemData;
		IConfigureProductExternalDetectorDialog.vInAutoSetup = TempInAutoSetup;
		IConfigureProductExternalDetectorDialog.vMainWindowPointer = vMainWindowPointer;

		//Pass control to dialog box and display
		//IEditProductDialog.vEPLocalNumberOfProducts = vCPLocalNumberOfProducts;
		int nResponse = IConfigureProductExternalDetectorDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			// TODO: Place code here to handle when the dialog is accepted
			if (IConfigureProductExternalDetectorDialog.vChangeMade)
				SetChangeMade();
			return nResponse;
		}
		else 
		if (nResponse == 10)
		{
			//  Main Menu button was pressed
			AbortDialog(10);
			return 10;
		}
		else
			return 0;
	}
	else
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\n\nNo Auxiliary Detectors configured in System.";
		TempNoticeDialog.vType = cErrorMessage;
		TempNoticeDialog.DoModal();
		return 1;
	}
}

void CNewEditProductDialog::OnSubFunction3Button() 
{
	//setup product exposure button pressed
	DoProductAutoSetup(2,cCenterExposureMenu);
	/*
	if (vChangeMade)
	{
		vGlobalCurrentProduct->SetProductName(vProductNameString);
		CheckProductFullyConfigured(vGlobalCurrentProduct,vLocalConfigurationData);
	}
	CAutoSetupImageDisplayDialog IAutoSetupImageDisplayDialog;
	IAutoSetupImageDisplayDialog.vLocalConfigurationData = vLocalConfigurationData;
	IAutoSetupImageDisplayDialog.vLocalSystemData = vLocalSystemData;
	IAutoSetupImageDisplayDialog.vMainWindowPointer = vMainWindowPointer;
	IAutoSetupImageDisplayDialog.vGlobalCurrentProduct = vGlobalCurrentProduct;
	IAutoSetupImageDisplayDialog.vAutoSetupType = cEditExposureAutoSetup;
	IAutoSetupImageDisplayDialog.vMode = cCenterExposureMenu;
	IAutoSetupImageDisplayDialog.vLocalProductCollection = vLocalProductCollection;

	BYTE TempEnableEjectors = vLocalConfigurationData->vEnableEjectors;
	vLocalConfigurationData->vEnableEjectors = false;
	int nResponse = IAutoSetupImageDisplayDialog.DoModal();
	vLocalConfigurationData->vEnableEjectors = TempEnableEjectors;

	if ((nResponse == IDOK) || (nResponse == 0))
	{
		// TODO: Place code here to handle when the dialog is accepted
		if (IAutoSetupImageDisplayDialog.vAnyChangeMade)
		{
			SetChangeMade();
		}
		if (IAutoSetupImageDisplayDialog.vSavedImageProcessingData)
			vLocalSystemData->vChangedSizeOfProduct = false;
		OnFunction2Button();
	}
	else
	if (nResponse == 10)
		AbortDialog(10);
		*/
}

BYTE CNewEditProductDialog::SetupProductEjectors(bool TempInAutoSetup, 
	bool TempStopRunningWhenExit)
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
		bool TempExitAutoSetupModeWhenExit = false;
		
		//Set dialog box data like how many products there are
		vGlobalCurrentProduct->SetProductName(vProductNameString);
		IConfigureProductEjectorsDialog.vLocalConfigurationData = vLocalConfigurationData;
		IConfigureProductEjectorsDialog.vLocalSystemData = vLocalSystemData;
		IConfigureProductEjectorsDialog.vMainWindowPointer = vMainWindowPointer;
		IConfigureProductEjectorsDialog.vLocalProductCollection = vLocalProductCollection;
		IConfigureProductEjectorsDialog.vStopRunningWhenExit = TempStopRunningWhenExit;
		IConfigureProductEjectorsDialog.vShowPreviousAndNextButtons = false;
		if (!vLocalSystemData->vInAutoSetup)
		if (TempInAutoSetup)
		{
			vLocalSystemData->vInAutoSetup = true;
			TempExitAutoSetupModeWhenExit = true;
		}
		//Pass control to dialog box and display
		//IEditProductDialog.vEPLocalNumberOfProducts = vCPLocalNumberOfProducts;
		int nResponse = IConfigureProductEjectorsDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (TempExitAutoSetupModeWhenExit)
			vLocalSystemData->vInAutoSetup = false;
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
			AbortDialog(10);
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

void CNewEditProductDialog::OnSubFunction4Button() 
{
	//Auto Learn setup button was pressed
	DoProductAutoSetup(2,cLearnMenu);
	/*
	if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\n\nSystem Must Be Stopped to Run Auto Setup";
		TempNoticeDialog.vType = cErrorMessage;
		TempNoticeDialog.DoModal();
	}
	else
	{
		if (vChangeMade)
		{
			vGlobalCurrentProduct->SetProductName(vProductNameString);
			CheckProductFullyConfigured(vGlobalCurrentProduct,vLocalConfigurationData);
		}
		CAutoSetupImageDisplayDialog IAutoSetupImageDisplayDialog;
		IAutoSetupImageDisplayDialog.vLocalConfigurationData = vLocalConfigurationData;
		IAutoSetupImageDisplayDialog.vLocalSystemData = vLocalSystemData;
		IAutoSetupImageDisplayDialog.vMainWindowPointer = vMainWindowPointer;
		IAutoSetupImageDisplayDialog.vGlobalCurrentProduct = vGlobalCurrentProduct;
		IAutoSetupImageDisplayDialog.vAutoSetupType = cEditIPAutoSetup;
		IAutoSetupImageDisplayDialog.vLocalProductCollection = vLocalProductCollection;

		BYTE TempEnableEjectors = vLocalConfigurationData->vEnableEjectors;
		vLocalConfigurationData->vEnableEjectors = false;
		int nResponse = IAutoSetupImageDisplayDialog.DoModal();
		vLocalConfigurationData->vEnableEjectors = TempEnableEjectors;

		if ((nResponse == IDOK) || (nResponse == 0))
		{
			// TODO: Place code here to handle when the dialog is accepted
			if (IAutoSetupImageDisplayDialog.vAnyChangeMade)
			{
				SetChangeMade();
			}
			if (IAutoSetupImageDisplayDialog.vSavedImageProcessingData)
				vLocalSystemData->vChangedSizeOfProduct = false;
			OnFunction2Button();
		}
		else
		if (nResponse == 10)
			AbortDialog(10);
	}
	*/
}

void CNewEditProductDialog::OnSubFunction5Button() 
{
	//evaluate button pressed
	DoProductAutoSetup(2,cSetInspections);
}


void CNewEditProductDialog::OnSubFunction6Button() 
{
	//edit inspection button pressed
	DoProductAutoSetup(2,cSetVoidThreshold);
	/*
	if (vChangeMade)
	{
		vGlobalCurrentProduct->SetProductName(vProductNameString);
		CheckProductFullyConfigured(vGlobalCurrentProduct,vLocalConfigurationData);
	}
	CAutoSetupImageDisplayDialog IAutoSetupImageDisplayDialog;
	IAutoSetupImageDisplayDialog.vLocalConfigurationData = vLocalConfigurationData;
	IAutoSetupImageDisplayDialog.vLocalSystemData = vLocalSystemData;
	IAutoSetupImageDisplayDialog.vMainWindowPointer = vMainWindowPointer;
	IAutoSetupImageDisplayDialog.vGlobalCurrentProduct = vGlobalCurrentProduct;
	IAutoSetupImageDisplayDialog.vAutoSetupType = cEditInspectionAutoSetup;
	IAutoSetupImageDisplayDialog.vLocalProductCollection = vLocalProductCollection;

	BYTE TempEnableEjectors = vLocalConfigurationData->vEnableEjectors;
	vLocalConfigurationData->vEnableEjectors = false;
	int nResponse = IAutoSetupImageDisplayDialog.DoModal();
	vLocalConfigurationData->vEnableEjectors = TempEnableEjectors;

	if ((nResponse == IDOK) || (nResponse == 0))
	{
		// TODO: Place code here to handle when the dialog is accepted
		if (IAutoSetupImageDisplayDialog.vAnyChangeMade)
		{
			SetChangeMade();
		}
		if (IAutoSetupImageDisplayDialog.vSavedImageProcessingData)
			vLocalSystemData->vChangedSizeOfProduct = false;
		OnFunction2Button();
	}
	else
	if (nResponse == 10)
		AbortDialog(10);
		*/
}

void CNewEditProductDialog::OnSubFunction7Button() 
{
}

void CNewEditProductDialog::OnSubFunction8Button() 
{
	//setup ejector timing
	bool TempHaveEjectorConfigured = false;
	for (BYTE TempLoop = 0; TempLoop < cNumberOfEjectors; TempLoop++)
	{
		if (vLocalConfigurationData->vEjector[TempLoop].vEnabled)
			TempHaveEjectorConfigured = true;
	}
	if (TempHaveEjectorConfigured)
	{
		BYTE TempReturn = SetupProductEjectors(true, true);
	}
}

HBRUSH CNewEditProductDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	HWND TempDialogHwnd = this->m_hWnd;
	int TextSize;
		pDC->SetAttribDC(pDC->m_hDC);
	
	if (pWnd->GetDlgCtrlID() == IDC_DialogTitleStaticText1)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DialogTitleStaticText, 8);  //5 is medium large
		pDC->SetTextColor(vGlobalDialogTitleColor);
	}


	if (pWnd->GetDlgCtrlID() == IDC_Background)
	{
		//pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
  }

	if (pWnd->GetDlgCtrlID() == IDC_ProductName)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_ProductName, 5);
		pDC->SetBkMode(TRANSPARENT);
		//return vWhiteBrush;
	}
	//if (pWnd->GetDlgCtrlID() == IDC_NumberOfXRayDetectorsDisplay) 
	//{
	//	TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_NumberOfXRayDetectorsDisplay, 5);
	//	pDC->SetBkMode(TRANSPARENT);
	//	//return vWhiteBrush;
	//}
	if (pWnd->GetDlgCtrlID() == IDC_BodyTriggerToImager) 
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_BodyTriggerToImager, 5);
		pDC->SetBkMode(TRANSPARENT);
		//return vWhiteBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_EndOfLine)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_EndOfLine, 5);
		pDC->SetBkMode(TRANSPARENT);
		//return vWhiteBrush;
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
		if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(cLightGray);
			return vGlobalButtonColorBrush;
		}
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
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction5Button, 5);  //5 is medium large
		pDC->SetBkMode(TRANSPARENT);
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
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

BOOL CNewEditProductDialog::PreTranslateMessage(MSG* pMsg) 
{
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
					cShowHelp, 6, 0);
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

void CNewEditProductDialog::SetChangeMade() 
{
	if (!vChangeMade)
	{
		m_Function2Button.ModifyStyle(SS_CENTERIMAGE,NULL,NULL);
		m_Function4Button.ShowWindow(SW_SHOW);
		CString TempText;
		TempText.LoadString(IDS_SaveAndExit);
		SetDlgItemText(IDC_Function2Button,TempText);
		TempText.LoadString(IDS_Cancel);
		SetDlgItemText(IDC_Function4Button,TempText);
		vChangeMade = true;
	}
}

void CNewEditProductDialog::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == vOpenNewWindowTimerHandle)
  {
		KillTimer(vOpenNewWindowTimerHandle);
		
		if (vProductNameString == "Enter Product Name")
		{
			DoProductAutoSetup(0,0);
		}
		else
		if (vEditExistingProduct == 4) //evaluate product
		{
			EvaluateProduct();
		}
	}
	
	CDialog::OnTimer(nIDEvent);
}

void CNewEditProductDialog::AddProduct(CProduct *TempProduct)
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



void CNewEditProductDialog::OnDialogtitlestatictext1()
{
	BYTE TempHiddenKeys = 8;
	if (!PasswordOK(cTemporaryInspxPassword,false))
		TempHiddenKeys = 10;
	PostThreadMessage(vGlobalCallingThreadID, cDisplaySoftKeyMenu, TempHiddenKeys, TempHiddenKeys);
}
