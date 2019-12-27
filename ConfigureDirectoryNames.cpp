// ConfigureDirectoryNames.cpp : implementation file
//

#include "stdafx.h"
#include "scantrac.h"
#include "ConfigureDirectoryNames.h"
#include "AlphaEntryDialog.h"
#include "SystemConfigurationData.h"
#include "YesNoDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern BYTE vGlobalShiftKeyDown;
extern int vGlobalPasswordLevel;
extern unsigned long vGlobalDialogTitleColor;
extern BYTE vGlobalPCRunningWindowsXP;
extern unsigned int vGlobalShadowImages;
extern int vGlobalCountErrorMsgPossibleDirectoryNotAvailableSent;
extern BOOL vGlobalHaveDDrive;
////////////////////////////////////////////////////////////////////////////
// CConfigureDirectoryNames dialog


CConfigureDirectoryNames::CConfigureDirectoryNames(CWnd* pParent /*=NULL*/)
	: CDialog(CConfigureDirectoryNames::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConfigureDirectoryNames)
	//}}AFX_DATA_INIT
	vWaitingToExit = false;
	vWaitToExitTimerHandle = 62066;
	vLocalCWndCollection.Add(this					 );
	vLocalCWndCollection.Add(&m_SubFunction4Display	 );
	vLocalCWndCollection.Add(&m_SubFunction4Button	 );
	vLocalCWndCollection.Add(&m_SubFunction3Display	 );
	vLocalCWndCollection.Add(&m_SubFunction3Button	 );
	vLocalCWndCollection.Add(&m_SubFunction2Display	 );
	vLocalCWndCollection.Add(&m_SubFunction2Button	 );
	vLocalCWndCollection.Add(&m_SubFunction1Display	 );
	vLocalCWndCollection.Add(&m_SubFunction1Button	 );
	vLocalCWndCollection.Add(&m_Function4Button		 );
	vLocalCWndCollection.Add(&m_Function2Button		 );
	vLocalCWndCollection.Add(&m_Background			 );
	vLocalCWndCollection.Add(&m_DialogTitleStaticText);
	vLocalCWndCollection.Add(&m_Function1Button		 );
	vLocalCWndCollection.Add(&m_Function3Button		 );
	vLocalCWndCollection.Add(&m_Function5Button		 );
	vLocalCWndCollection.Add(&m_Info2				 );
	vLocalCWndCollection.Add(&m_SubFunction5Button	 );
	vLocalCWndCollection.Add(&m_SubFunction5Display	 );
	vLocalCWndCollection.Add(&m_SubFunction6Button	 );
	vLocalCWndCollection.Add(&m_SubFunction6Display	 );
	vLocalCWndCollection.Add(&m_SubFunction7Button	 );
	vLocalCWndCollection.Add(&m_SubFunction7Display	 );
	vLocalCWndCollection.Add(&m_SubFunction8Button	 );
	vLocalCWndCollection.Add(&m_SubFunction8Display	 );
	vLocalCWndCollection.Add(&m_SubFunction8Displayz );
	vLocalCWndCollection.Add(&m_TextDisplay			 );
}


void CConfigureDirectoryNames::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfigureDirectoryNames)
	DDX_Control(pDX, IDC_SubFunction4Display, m_SubFunction4Display);
	DDX_Control(pDX, IDC_SubFunction4Button, m_SubFunction4Button);
	DDX_Control(pDX, IDC_SubFunction3Display, m_SubFunction3Display);
	DDX_Control(pDX, IDC_SubFunction3Button, m_SubFunction3Button);
	DDX_Control(pDX, IDC_SubFunction2Display, m_SubFunction2Display);
	DDX_Control(pDX, IDC_SubFunction2Button, m_SubFunction2Button);
	DDX_Control(pDX, IDC_SubFunction1Display, m_SubFunction1Display);
	DDX_Control(pDX, IDC_SubFunction1Button, m_SubFunction1Button);
	DDX_Control(pDX, IDC_Function4Button, m_Function4Button);
	DDX_Control(pDX, IDC_Function2Button, m_Function2Button);
	DDX_Control(pDX, IDC_Background, m_Background);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_DialogTitleStaticText1, m_DialogTitleStaticText);
	DDX_Control(pDX, IDC_Function1Button, m_Function1Button);
	DDX_Control(pDX, IDC_Function3Button, m_Function3Button);
	DDX_Control(pDX, IDC_Function5Button, m_Function5Button);
	DDX_Control(pDX, IDC_Info2, m_Info2);
	DDX_Control(pDX, IDC_SubFunction5Button, m_SubFunction5Button);
	DDX_Control(pDX, IDC_SubFunction5Display, m_SubFunction5Display);
	DDX_Control(pDX, IDC_SubFunction6Button, m_SubFunction6Button);
	DDX_Control(pDX, IDC_SubFunction6Display, m_SubFunction6Display);
	DDX_Control(pDX, IDC_SubFunction7Button, m_SubFunction7Button);
	DDX_Control(pDX, IDC_SubFunction7Display, m_SubFunction7Display);
	DDX_Control(pDX, IDC_SubFunction8Button, m_SubFunction8Button);
	DDX_Control(pDX, IDC_SubFunction8Display, m_SubFunction8Display);
	DDX_Control(pDX, IDC_SubFunction8Displayz, m_SubFunction8Displayz);
	DDX_Control(pDX, IDC_TextDisplay, m_TextDisplay);
}


BEGIN_MESSAGE_MAP(CConfigureDirectoryNames, CDialog)
	//{{AFX_MSG_MAP(CConfigureDirectoryNames)
	ON_WM_SHOWWINDOW()
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
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_STN_CLICKED(IDC_DialogTitleStaticText1, &CConfigureDirectoryNames::OnDialogtitlestatictext1)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfigureDirectoryNames message handlers

void CConfigureDirectoryNames::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	vChangeMade = false;
	m_Background.SetWindowPos(NULL,cMainBackgroundLeft, cMainBackgroundTop,cMainBackgroundLength, cMainBackgroundHeight,SWP_NOZORDER);

	vScanTracScreenShotDirectory = vLocalConfigurationData->vScanTracScreenShotDirectory;
	vScanTracProductionReportDirectory = vLocalConfigurationData->vScanTracProductionReportDirectory;
	vScanTracEventLogDirectory = vLocalConfigurationData->vScanTracEventLogDirectory;
	vScanTracRejectedImagesDirectory = vLocalConfigurationData->vScanTracRejectedImagesDirectory;
	vWriteRejectsToFiles = vLocalConfigurationData->vWriteRejectsToFiles;
	vSaveAllImagesToRejectsFolder = vLocalConfigurationData->vSaveAllImagesToRejectsFolder;
	vScanTracImagesDirectory = vLocalConfigurationData->vScanTracImagesDirectory;
	vScanTracCalibrationImagesDirectory = vLocalConfigurationData->vScanTracCalibrationImagesDirectory;
	vScanTracSerialLogsDirectory = vLocalConfigurationData->vScanTracSerialLogsDirectory;
	vScanTracSetupBackupDirectory = vLocalConfigurationData->vScanTracSetupBackupDirectory;

	UpdateDisplay();
	UpdateInspxDisplay();

	this->SetWindowText( _T("Inspx-Directories"));
	SetupMenu(vLocalCWndCollection);
}

void CConfigureDirectoryNames::OnFunction1Button() 
{
	//Main Menu was pressed
	if ((!vChangeMade) || (AbortSetupChanges()))
		CDialog::EndDialog(10);
}

void CConfigureDirectoryNames::OnFunction2Button() 
{
	//Save and exit was pressed
	if ((!vChangeMade) || (KeepSetupChanges()))
	{
		vLocalConfigurationData->vScanTracScreenShotDirectory = vScanTracScreenShotDirectory;
		vLocalConfigurationData->vScanTracProductionReportDirectory = vScanTracProductionReportDirectory;
		vLocalConfigurationData->vScanTracEventLogDirectory = vScanTracEventLogDirectory;
		vLocalConfigurationData->vScanTracImagesDirectory = vScanTracImagesDirectory;
		vLocalConfigurationData->vScanTracRejectedImagesDirectory = vScanTracRejectedImagesDirectory;
		vLocalConfigurationData->vWriteRejectsToFiles = vWriteRejectsToFiles;
		vLocalConfigurationData->vSaveAllImagesToRejectsFolder = vSaveAllImagesToRejectsFolder;
		vLocalConfigurationData->vScanTracCalibrationImagesDirectory = vScanTracCalibrationImagesDirectory;
		vLocalConfigurationData->vScanTracSerialLogsDirectory = vScanTracSerialLogsDirectory;
		vLocalConfigurationData->vScanTracSetupBackupDirectory = vScanTracSetupBackupDirectory;

		CString TempTestString = vLocalConfigurationData->vScanTracRejectedImagesDirectory;
		TempTestString.MakeUpper();
		if (TempTestString == "D:\\SCANTRACREJECTEDIMAGES\\")
		{
			CString TempFolder = "C:\\ScanTracRejectedImages";  //if saving rejects to D drive, can't have folder on C drive, or nightly backup will delete all the files
			RemoveDirectory(TempFolder);
		}

		vWaitingToExit = true;
		SetDlgItemText(IDC_TextDisplay, _T("\nCreating Directories, Please Wait....."));

		int TimerResult = SetTimer(vWaitToExitTimerHandle,100,NULL);
		if (!TimerResult)
			ReportErrorMessage("Error-WaitToExit Timer Failed",cEMailInspx,32000);
	}
}

void CConfigureDirectoryNames::OnFunction3Button() 
{
	// TODO: Add your control notification handler code here
	
}

void CConfigureDirectoryNames::OnFunction4Button() 
{
	//Cancel was pressed
	if (vChangeMade)
	if ((!vChangeMade) || (AbortSetupChanges()))
		CDialog::OnCancel();
}

void CConfigureDirectoryNames::OnFunction5Button() 
{
	// TODO: Add your control notification handler code here
	
}

void CConfigureDirectoryNames::OnSubFunction1Button() 
{  //event log directory
	if (PasswordOK(cTemporaryInspxPassword,false))
	{
		CAlphaEntryDialog IAlphaEntryDialog;  
		//Set dialog box data titles and number value
		IAlphaEntryDialog.vLocalSystemData = vLocalSystemData;
		IAlphaEntryDialog.vEditString = vScanTracEventLogDirectory;
		IAlphaEntryDialog.vMainWindowPointer = vMainWindowPointer;

		IAlphaEntryDialog.m_DialogTitleStaticText = "Event Log Directory Name";

		IAlphaEntryDialog.m_DialogTitleStaticText2 = "Default: c:\\ScanTracEventLogs";
		IAlphaEntryDialog.vAllowSlashes = true;
		IAlphaEntryDialog.vAllowMoreThan32Characters = true;
		IAlphaEntryDialog.vBlankNameOK = true;
		//Pass control to dialog box and display
		int nResponse = IAlphaEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			vScanTracEventLogDirectory = IAlphaEntryDialog.vEditString;
			if (!vScanTracEventLogDirectory.IsEmpty())
			{
				wchar_t TempChar = vScanTracEventLogDirectory.GetAt(vScanTracEventLogDirectory.GetLength() - 1);
				if (TempChar != '\\')
					vScanTracEventLogDirectory = vScanTracEventLogDirectory + "\\";
				if (IsDirectoryDefaultNameFor_C_D_Drive(vScanTracEventLogDirectory,cScanTracEventLogDirectory) != true)
				{
					vScanTracEventLogDirectory = cScanTracEventLogDirectory;
					//default directories on C: and D: can not be changed
					CNoticeDialog TempNoticeDialog;
					TempNoticeDialog.vNoticeText = "\n\nDirectory on ScanTrac must use default name.";
					TempNoticeDialog.vType = cErrorMessage;
					TempNoticeDialog.DoModal();
				}
//vLocalConfigurationData->vScanTracEventLogDirectory
			}
			UpdateDisplay();
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
}

void CConfigureDirectoryNames::OnSubFunction2Button() 
{ //serial log directory name
	if (PasswordOK(cTemporaryInspxPassword,false))
	{
		CAlphaEntryDialog IAlphaEntryDialog;  
		
		//Set dialog box data titles and number value
		IAlphaEntryDialog.vLocalSystemData = vLocalSystemData;
		IAlphaEntryDialog.vEditString = vScanTracSerialLogsDirectory;
		IAlphaEntryDialog.vMainWindowPointer = vMainWindowPointer;

		IAlphaEntryDialog.m_DialogTitleStaticText = "Serail Logs Directory Name";

		IAlphaEntryDialog.m_DialogTitleStaticText2 = "Default: c:\\ScanTracSerialLogs";
		IAlphaEntryDialog.vAllowSlashes = true;
		IAlphaEntryDialog.vAllowMoreThan32Characters = true;
		IAlphaEntryDialog.vBlankNameOK = true;
		//Pass control to dialog box and display
		int nResponse = IAlphaEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			vScanTracSerialLogsDirectory = IAlphaEntryDialog.vEditString;
			if (!vScanTracSerialLogsDirectory.IsEmpty())
			{
				wchar_t TempChar = vScanTracSerialLogsDirectory.GetAt(vScanTracSerialLogsDirectory.GetLength() - 1);
				if (TempChar != '\\')
					vScanTracSerialLogsDirectory = vScanTracSerialLogsDirectory + "\\";
				if (IsDirectoryDefaultNameFor_C_D_Drive(vScanTracSerialLogsDirectory,cScanTracSerialLogsDirectory) != true)
				{
					vScanTracSerialLogsDirectory = cScanTracSerialLogsDirectory;
					//default directories on C: and D: can not be changed
					CNoticeDialog TempNoticeDialog;
					TempNoticeDialog.vNoticeText = "\n\nDirectory on ScanTrac must use default name.";
					TempNoticeDialog.vType = cErrorMessage;
					TempNoticeDialog.DoModal();
				}
			}
			UpdateDisplay();
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
}

void CConfigureDirectoryNames::OnSubFunction3Button() 
{  //calibration images directory name
	if (PasswordOK(cTemporaryInspxPassword,false))
	{
		CAlphaEntryDialog IAlphaEntryDialog;  
		
		//Set dialog box data titles and number value
		IAlphaEntryDialog.vLocalSystemData = vLocalSystemData;
		IAlphaEntryDialog.vEditString = vScanTracCalibrationImagesDirectory;
		IAlphaEntryDialog.vMainWindowPointer = vMainWindowPointer;
		IAlphaEntryDialog.m_DialogTitleStaticText = "Calibration Images Directory Name";

		CString TempString = "Default: ";
		IAlphaEntryDialog.m_DialogTitleStaticText2 = TempString + cScanTracCalibrationImagesDirectory;
		IAlphaEntryDialog.vAllowSlashes = true;
		IAlphaEntryDialog.vAllowMoreThan32Characters = true;
		IAlphaEntryDialog.vBlankNameOK = true;
		//Pass control to dialog box and display
		int nResponse = IAlphaEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			vScanTracCalibrationImagesDirectory = IAlphaEntryDialog.vEditString;
			if (!vScanTracCalibrationImagesDirectory.IsEmpty())
			{
				wchar_t TempChar = vScanTracCalibrationImagesDirectory.GetAt(vScanTracCalibrationImagesDirectory.GetLength() - 1);
				if (TempChar != '\\')
					vScanTracCalibrationImagesDirectory = vScanTracCalibrationImagesDirectory + "\\";
				if (IsDirectoryDefaultNameFor_C_D_Drive(vScanTracCalibrationImagesDirectory,cScanTracCalibrationImagesDirectory) != true)
				{
					vScanTracCalibrationImagesDirectory = cScanTracCalibrationImagesDirectory;
					//default directories on C: and D: can not be changed
					CNoticeDialog TempNoticeDialog;
					TempNoticeDialog.vNoticeText = "\n\nDirectory on ScanTrac must use default name.";
					TempNoticeDialog.vType = cErrorMessage;
					TempNoticeDialog.DoModal();
				}
			}
			UpdateDisplay();
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
}

void CConfigureDirectoryNames::OnSubFunction4Button() 
{
	CAlphaEntryDialog IAlphaEntryDialog;  
	//Set dialog box data titles and number value
	IAlphaEntryDialog.vLocalSystemData = vLocalSystemData;
	IAlphaEntryDialog.vEditString = vScanTracSetupBackupDirectory;
	IAlphaEntryDialog.m_DialogTitleStaticText = "Edit Backup Directory Name";
	IAlphaEntryDialog.vMainWindowPointer = vMainWindowPointer;
	IAlphaEntryDialog.m_DialogTitleStaticText2 = "Original Value: " + vScanTracSetupBackupDirectory;

	IAlphaEntryDialog.m_DialogTitleStaticText2 = "Default: ";
	IAlphaEntryDialog.m_DialogTitleStaticText3 = cScanTracSetupBackupDirectory;
	IAlphaEntryDialog.vAllowSlashes = true;
	IAlphaEntryDialog.vAllowDashes = true;
	IAlphaEntryDialog.vAllowMoreThan32Characters = true;
	IAlphaEntryDialog.vBlankNameOK = true;
	//Pass control to dialog box and display
	int nResponse = IAlphaEntryDialog.DoModal();
	//dialog box is now closed, if user pressed select do this
	//if user pressed cancel, do nothing
	if (nResponse == IDOK)
	{
		vScanTracSetupBackupDirectory = IAlphaEntryDialog.vEditString;
		if (!vScanTracSetupBackupDirectory.IsEmpty())
		{
			wchar_t TempChar = vScanTracSetupBackupDirectory.GetAt(vScanTracSetupBackupDirectory.GetLength() - 1);
			if (TempChar != '\\')
				vScanTracSetupBackupDirectory = vScanTracSetupBackupDirectory + "\\";
				if (IsDirectoryDefaultNameFor_C_D_Drive(vScanTracSetupBackupDirectory,cScanTracSetupBackupDirectory) != true)
				{
					vScanTracSetupBackupDirectory = cScanTracSetupBackupDirectory;
					//default directories on C: and D: can not be changed
					CNoticeDialog TempNoticeDialog;
					TempNoticeDialog.vNoticeText = "\n\nDirectory on ScanTrac must use default name.";
					TempNoticeDialog.vType = cErrorMessage;
					TempNoticeDialog.DoModal();
				}
		}
		UpdateDisplay();
		SetChangeMade();
	}
	else
	if (nResponse == 10)  //main menu pressed
		CDialog::EndDialog(10);
}

void CConfigureDirectoryNames::OnSubFunction5Button() 
{
	CAlphaEntryDialog IAlphaEntryDialog;  
	
	//Set dialog box data titles and number value
	IAlphaEntryDialog.vLocalSystemData = vLocalSystemData;
	IAlphaEntryDialog.vEditString = vScanTracProductionReportDirectory;
	IAlphaEntryDialog.vMainWindowPointer = vMainWindowPointer;

	IAlphaEntryDialog.m_DialogTitleStaticText = "Production Report Directory Name";

	IAlphaEntryDialog.m_DialogTitleStaticText2 = "Default: ";
	IAlphaEntryDialog.m_DialogTitleStaticText2 = IAlphaEntryDialog.m_DialogTitleStaticText2 + cScanTracProductionReportDirectory;
	IAlphaEntryDialog.vAllowSlashes = true;
	IAlphaEntryDialog.vAllowMoreThan32Characters = true;
	IAlphaEntryDialog.vBlankNameOK = true;
	//Pass control to dialog box and display
	int nResponse = IAlphaEntryDialog.DoModal();
	//dialog box is now closed, if user pressed select do this
	//if user pressed cancel, do nothing
	if (nResponse == IDOK)
	{
		vScanTracProductionReportDirectory = IAlphaEntryDialog.vEditString;
		if (!vScanTracProductionReportDirectory.IsEmpty())
		{
			wchar_t TempChar = vScanTracProductionReportDirectory.GetAt(vScanTracProductionReportDirectory.GetLength() - 1);
			if (TempChar != '\\')
				vScanTracProductionReportDirectory = vScanTracProductionReportDirectory + "\\";
			if (IsDirectoryDefaultNameFor_C_D_Drive(vScanTracProductionReportDirectory,cScanTracProductionReportDirectory) != true)
			{
				vScanTracProductionReportDirectory = cScanTracProductionReportDirectory;
				//default directories on C: and D: can not be changed
				CNoticeDialog TempNoticeDialog;
				TempNoticeDialog.vNoticeText = "\n\nDirectory on ScanTrac must use default name.";
				TempNoticeDialog.vType = cErrorMessage;
				TempNoticeDialog.DoModal();
			}

		}
		UpdateDisplay();
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

void CConfigureDirectoryNames::OnSubFunction6Button() 
{
	CAlphaEntryDialog IAlphaEntryDialog;  
	
	//Set dialog box data titles and number value
	IAlphaEntryDialog.vLocalSystemData = vLocalSystemData;
	IAlphaEntryDialog.vEditString = vScanTracScreenShotDirectory;
	IAlphaEntryDialog.vMainWindowPointer = vMainWindowPointer;

	IAlphaEntryDialog.m_DialogTitleStaticText = "Screen Shot Directory Name";

	IAlphaEntryDialog.m_DialogTitleStaticText2 = "Default: ";
	IAlphaEntryDialog.m_DialogTitleStaticText2 = IAlphaEntryDialog.m_DialogTitleStaticText2
		+ cScanTracScreenShotDirectory;
	IAlphaEntryDialog.vAllowSlashes = true;
	IAlphaEntryDialog.vAllowMoreThan32Characters = true;
	IAlphaEntryDialog.vBlankNameOK = true;
	//Pass control to dialog box and display
	int nResponse = IAlphaEntryDialog.DoModal();
	//dialog box is now closed, if user pressed select do this
	//if user pressed cancel, do nothing
	if (nResponse == IDOK)
	{
		vScanTracScreenShotDirectory = IAlphaEntryDialog.vEditString;
		if (!vScanTracScreenShotDirectory.IsEmpty())
		{
			wchar_t TempChar = vScanTracScreenShotDirectory.GetAt(vScanTracScreenShotDirectory.GetLength() - 1);
			if (TempChar != '\\')
				vScanTracScreenShotDirectory = vScanTracScreenShotDirectory + "\\";
			if (IsDirectoryDefaultNameFor_C_D_Drive(vScanTracScreenShotDirectory,cScanTracScreenShotDirectory) != true)
			{
				vScanTracScreenShotDirectory = cScanTracScreenShotDirectory;
				//default directories on C: and D: can not be changed
				CNoticeDialog TempNoticeDialog;
				TempNoticeDialog.vNoticeText = "\n\nDirectory on ScanTrac must use default name.";
				TempNoticeDialog.vType = cErrorMessage;
				TempNoticeDialog.DoModal();
			}
		}
		UpdateDisplay();
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

void CConfigureDirectoryNames::OnSubFunction7Button() 
{
	CAlphaEntryDialog IAlphaEntryDialog;  
	
	//Set dialog box data titles and number value
	IAlphaEntryDialog.vLocalSystemData = vLocalSystemData;
	IAlphaEntryDialog.vEditString = vScanTracImagesDirectory;
	IAlphaEntryDialog.vMainWindowPointer = vMainWindowPointer;

	IAlphaEntryDialog.m_DialogTitleStaticText = "Images Directory Name";

	IAlphaEntryDialog.m_DialogTitleStaticText2 = "Default: ";
	IAlphaEntryDialog.m_DialogTitleStaticText2 = IAlphaEntryDialog.m_DialogTitleStaticText2 + cImageDirectory;

	IAlphaEntryDialog.vAllowSlashes = true;
	IAlphaEntryDialog.vAllowMoreThan32Characters = true;
	IAlphaEntryDialog.vBlankNameOK = true;
	//Pass control to dialog box and display
	int nResponse = IAlphaEntryDialog.DoModal();
	//dialog box is now closed, if user pressed select do this
	//if user pressed cancel, do nothing
	if (nResponse == IDOK)
	{
		vScanTracImagesDirectory = IAlphaEntryDialog.vEditString;
		if (!vScanTracImagesDirectory.IsEmpty())
		{
			wchar_t TempChar = vScanTracImagesDirectory.GetAt(vScanTracImagesDirectory.GetLength() - 1);
			if (TempChar != '\\')
				vScanTracImagesDirectory = vScanTracImagesDirectory + "\\";
				if (IsDirectoryDefaultNameFor_C_D_Drive(vScanTracImagesDirectory,cImageDirectory) != true)
				{
					vScanTracImagesDirectory = cImageDirectory;
					//default directories on C: and D: can not be changed
					CNoticeDialog TempNoticeDialog;
					TempNoticeDialog.vNoticeText = "\n\nDirectory on ScanTrac must use default name.";
					TempNoticeDialog.vType = cErrorMessage;
					TempNoticeDialog.DoModal();
				}
		}
		UpdateDisplay();
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

void CConfigureDirectoryNames::OnSubFunction8Button() 
{
	// TODO: Add your control notification handler code here
	if (vWriteRejectsToFiles)
	{
		vWriteRejectsToFiles = false;
		UpdateDisplay();
		SetChangeMade();
	}
	else
	{
		CAlphaEntryDialog IAlphaEntryDialog;  
		
		//Set dialog box data titles and number value
		IAlphaEntryDialog.vLocalSystemData = vLocalSystemData;
		CString TempDirectory = vScanTracRejectedImagesDirectory;
		CString TempDefaultDirectory; // = cScanTracRejectedImagesDirectory;
		IAlphaEntryDialog.vMainWindowPointer = vMainWindowPointer;

		CFileStatus TempFileStatus;
		CString TempStringSSD = "D:\\";
		LPTSTR TempStringSSDP = TempStringSSD.GetBuffer(TempStringSSD.GetLength());
		if (CFile::GetStatus(TempStringSSDP, TempFileStatus))
		{  // if true above D: drive exists use it first
			TempDefaultDirectory = cRejectedImagesDirectoryDDrive;
		}
		else
		{
			TempDefaultDirectory = cRejectedImagesDirectoryCDrive;
		}
//		if (TempFileStatus.m_szFullName == TempStringSSD)
//		{
//			IAlphaEntryDialog.m_DialogTitleStaticText3 = "Do Not Use C Drive";
//		}

		//if (vLocalSystemData->vBackupDrive == "D")
		//{
		//	TempStringSSD = "D:\\";
		//	TempStringSSDP = TempStringSSD.GetBuffer(TempStringSSD.GetLength());
		//	CFile::GetStatus(TempStringSSDP, TempFileStatus);
		//	if (TempFileStatus.m_szFullName == TempStringSSD)
		//	{
		//		TempDefaultDirectory = "D:\\ScanTracRejectedImages\\";
		//		IAlphaEntryDialog.m_DialogTitleStaticText3 = "Do Not Use C Drive (D = Backup Drive)";
		//	}
		//}

		IAlphaEntryDialog.vEditString = TempDirectory;

		IAlphaEntryDialog.m_DialogTitleStaticText = "Rejected Images Directory Name";

		IAlphaEntryDialog.m_DialogTitleStaticText2 = "Default: " + TempDefaultDirectory;

		IAlphaEntryDialog.vAllowSlashes = true;
		IAlphaEntryDialog.vAllowMoreThan32Characters = true;
		IAlphaEntryDialog.vBlankNameOK = true;
		//Pass control to dialog box and display
		int nResponse = IAlphaEntryDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			vScanTracRejectedImagesDirectory = IAlphaEntryDialog.vEditString;
			if (vScanTracRejectedImagesDirectory.GetLength() > 5)
				vWriteRejectsToFiles = true;
			if (!vScanTracRejectedImagesDirectory.IsEmpty())
			{
				wchar_t TempChar = vScanTracRejectedImagesDirectory.GetAt(vScanTracRejectedImagesDirectory.GetLength() - 1);
				if (TempChar != '\\')
					vScanTracRejectedImagesDirectory = vScanTracRejectedImagesDirectory + "\\";

		//		if ((IsDirectoryDefaultNameFor_C_D_Drive(vScanTracRejectedImagesDirectory,cRejectedImagesDirectoryDDrive) != true) && 
		//			(IsDirectoryDefaultNameFor_C_D_Drive(vScanTracRejectedImagesDirectory,cRejectedImagesDirectoryCDrive) != true) && 
		//			(vGlobalPCRunningWindowsXP != cWindowsNT))
		//	TempDefaultDirectory = cRejectedImagesDirectoryDDrive;
		//}
		//else
		//{
			TempDefaultDirectory = cRejectedImagesDirectoryCDrive;
			if (vGlobalPCRunningWindowsXP != cWindowsNT)
			if ((('C' == vScanTracRejectedImagesDirectory.GetAt(0)) && (!IsDirectoryDefaultNameFor_C_D_Drive(vScanTracRejectedImagesDirectory,cRejectedImagesDirectoryCDrive))) || 
				(('c' == vScanTracRejectedImagesDirectory.GetAt(0)) && (!IsDirectoryDefaultNameFor_C_D_Drive(vScanTracRejectedImagesDirectory,cRejectedImagesDirectoryCDrive))) || 
				(('d' == vScanTracRejectedImagesDirectory.GetAt(0)) && (!IsDirectoryDefaultNameFor_C_D_Drive(vScanTracRejectedImagesDirectory,cRejectedImagesDirectoryDDrive))) || 
				(('D' == vScanTracRejectedImagesDirectory.GetAt(0)) && (!IsDirectoryDefaultNameFor_C_D_Drive(vScanTracRejectedImagesDirectory,cRejectedImagesDirectoryDDrive))))
				{
					vScanTracRejectedImagesDirectory = TempDefaultDirectory;
					//default directories on C: and D: can not be changed
					CNoticeDialog TempNoticeDialog;
					TempNoticeDialog.vNoticeText = "\n\nDirectory on ScanTrac must use default name.";
					TempNoticeDialog.vType = cErrorMessage;
					TempNoticeDialog.DoModal();
				}
			}
			UpdateDisplay();
			SetChangeMade();
			if (vWriteRejectsToFiles)
			{
				CYesNoDialog TempYesNoDialog;
				TempYesNoDialog.vNoticeText = _T("\n\nDo you want to write all images to files,\nor just rejects?");
				TempYesNoDialog.vYesButtonText = "Just  Rejects";
				TempYesNoDialog.vNoButtonText = "All Images";
				TempYesNoDialog.vQuestionType = cConfirmQuestion;
				int TempResult = TempYesNoDialog.DoModal();
				if (TempResult == IDOK)
					vSaveAllImagesToRejectsFolder = false;
				else
					vSaveAllImagesToRejectsFolder = true;

				UpdateDisplay();
			}
			CString TempTestString = vScanTracRejectedImagesDirectory;
			TempTestString.MakeUpper();
			int TempPosition = TempTestString.Find(_T("C:\\"), 0);
			if (vWriteRejectsToFiles)
			if (vLocalSystemData->vBackupDrive.GetLength())  //if have a backup drive
			if (TempPosition >= 0)
			{
				CNoticeDialog TempNoticeDialog;
				TempNoticeDialog.vNoticeText = "\n\nSaving Images should not be done to the primary drive\nbecause backing up to the backup drive will take hours.\nSave images to backup drive.";
				TempNoticeDialog.vType = cErrorMessage;
				TempNoticeDialog.DoModal();
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
}

BOOL CConfigureDirectoryNames::PreTranslateMessage(MSG* pMsg) 
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
		int TempKey = pMsg->wParam;
		if (vGlobalShiftKeyDown)
			PostThreadMessage(vGlobalCallingThreadID,cHiddenKeyPressed, 0, TempKey);
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
				PostThreadMessage(vGlobalCallingThreadID, cShowHelp, 0, 0);
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
			{	
				if (vGlobalPasswordLevel)
				{
					PasswordOK(cNoPassword,false);
				}
				else
				{
					PasswordOK(cAnyPasswordToEnter, true);
				}
				TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText1);
				if (TempWindow)
					TempWindow->Invalidate(false);
				UpdateInspxDisplay();
			}
			break;
		}
		return true;  //true indicates it has been handled, so do not process
	}
		
	return CDialog::PreTranslateMessage(pMsg);
}

HBRUSH CConfigureDirectoryNames::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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

	if (pWnd->GetDlgCtrlID() == IDC_TextDisplay)
	{
		TextSize = SetTextSizeMultilineDisplay(TempDialogHwnd, pWnd, pDC, &m_TextDisplay, 5);
	if (vWaitingToExit)
	{
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vYellowBrush;
  }
		else
		{
			pDC->SetBkMode(TRANSPARENT);
			return vLocalSystemData->vLightGreenBrush;
		}
	}

	if (pWnd->GetDlgCtrlID() == IDC_Background)
	{
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightGreenBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Info2)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_Info2, 5);
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
	
	return hbr;
}

void CConfigureDirectoryNames::SetChangeMade() 
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

void CConfigureDirectoryNames::UpdateDisplay() 
{
	SetDlgItemText(IDC_SubFunction5Display,vScanTracProductionReportDirectory + " ");
	SetDlgItemText(IDC_SubFunction6Display,vScanTracScreenShotDirectory + " ");
	SetDlgItemText(IDC_SubFunction7Display,vScanTracImagesDirectory + " ");
	SetDlgItemText(IDC_SubFunction1Display," " + vScanTracEventLogDirectory);
	SetDlgItemText(IDC_SubFunction2Display," " + vScanTracSerialLogsDirectory);
	SetDlgItemText(IDC_SubFunction3Display," " + vScanTracCalibrationImagesDirectory);
	SetDlgItemText(IDC_SubFunction4Display," " + vScanTracSetupBackupDirectory);


	if (vWriteRejectsToFiles)
	{
		SetDlgItemText(IDC_SubFunction8Display,vScanTracRejectedImagesDirectory + " ");
		if (vSaveAllImagesToRejectsFolder)
			SetDlgItemText(IDC_SubFunction8Button, _T("Disable Saving All Images"));
		else
			SetDlgItemText(IDC_SubFunction8Button, _T("Disable Saving Rejects"));
	}
	else
	{
		SetDlgItemText(IDC_SubFunction8Display, _T("Saving Images to Disk Disabled "));
		SetDlgItemText(IDC_SubFunction8Button, _T("Enable Saving Images"));
	}

}

void CConfigureDirectoryNames::UpdateInspxDisplay()
{
	if (PasswordOK(cTemporaryInspxPassword,false))
	{
		m_SubFunction1Button.ShowWindow(SW_SHOW);
		m_SubFunction1Display.ShowWindow(SW_SHOW);
		m_SubFunction2Button.ShowWindow(SW_SHOW);
		m_SubFunction2Display.ShowWindow(SW_SHOW);
		m_SubFunction3Button.ShowWindow(SW_SHOW);
		m_SubFunction3Display.ShowWindow(SW_SHOW);
		m_SubFunction4Button.ShowWindow(SW_SHOW);
		m_SubFunction4Display.ShowWindow(SW_SHOW);
	}
	else
	{
		m_SubFunction1Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction1Button.GetRectToInvalidateArrow());
		m_SubFunction1Display.ShowWindow(SW_HIDE);
		m_SubFunction2Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction2Button.GetRectToInvalidateArrow());
		m_SubFunction2Display.ShowWindow(SW_HIDE);
		m_SubFunction3Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction3Button.GetRectToInvalidateArrow());
		m_SubFunction3Display.ShowWindow(SW_HIDE);
		m_SubFunction4Button.ShowWindow(SW_HIDE);
		this->InvalidateRect(&m_SubFunction4Button.GetRectToInvalidateArrow());
		m_SubFunction4Display.ShowWindow(SW_HIDE);
	}
	CWnd *TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText1);
	if (TempWindow)
		TempWindow->Invalidate(false);
}


void CConfigureDirectoryNames::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == vWaitToExitTimerHandle)
	{
		KillTimer(vWaitToExitTimerHandle);
		//check screen shot director exists, if not, create it
		CString TempStringSSD = vLocalConfigurationData->vScanTracScreenShotDirectory;
		TempStringSSD.Delete(TempStringSSD.GetLength() -1, 1);
		LPTSTR TempStringSSDP = TempStringSSD.GetBuffer(TempStringSSD.GetLength());
		CFileStatus TempFileStatus;
		if (vLocalConfigurationData->vScanTracScreenShotDirectory.GetLength() > 4)
		if (!(CFile::GetStatus(TempStringSSDP, TempFileStatus)))
		if (!CreateDirectory(vLocalConfigurationData->vScanTracScreenShotDirectory,NULL))
		{
			CNoticeDialog TempNoticeDialog1;
			CString TempText("\n\nError, Could not create directory:\n");
			TempText = TempText + vLocalConfigurationData->vScanTracScreenShotDirectory;
			TempNoticeDialog1.vNoticeText = TempText;
			TempNoticeDialog1.vType = cErrorMessage;
			TempNoticeDialog1.DoModal();
		}
		// kjh 8/28/2012 added to create a shadow directory for Screen shots if images stored on server
		// check if directory was created on C: or D: drive, if not create a directory on C:
		// set bit 1 in variable vGlobalShadowImages if true
		// clear bit 1 if false, cShadowScreenShotsBit
		if ((vLocalConfigurationData->vScanTracScreenShotDirectory.GetAt(0) != 'C') && (vLocalConfigurationData->vScanTracScreenShotDirectory.GetAt(0) != 'D') &&
			(vLocalConfigurationData->vScanTracScreenShotDirectory.GetAt(0) != 'c') && (vLocalConfigurationData->vScanTracScreenShotDirectory.GetAt(0) != 'd'))
		{
			SetBit(vGlobalShadowImages,cShadowScreenShotsBit);		// not copying files to local directory, try to set up shadow directory
			CString TempStringScreenShotShadowDirectory;
			if  (CheckDiskSpaceFree("C:\\") == 0)
			{
				ClrBit(vGlobalShadowImages,cShadowScreenShotsBit);	// dont echo to C: drive
			}
			else
			{
				TempStringScreenShotShadowDirectory = cScanTracScreenShotDirectory;  //"c:\\ScanTracScreenShots\\";	//				
				if (!(CFile::GetStatus(TempStringScreenShotShadowDirectory, TempFileStatus)))
				{
					BOOL TempDirectoryCreate = CreateDirectory(TempStringScreenShotShadowDirectory,NULL);
					if (TempDirectoryCreate == false)
						ClrBit(vGlobalShadowImages,cShadowScreenShotsBit);	// if directory is not created, no need to shadow
				}
			}
		}
		else
		{
			ClrBit(vGlobalShadowImages,cShadowScreenShotsBit);
		}


		//check if director exists, if not, create it
		TempStringSSD = vLocalConfigurationData->vScanTracProductionReportDirectory;
		TempStringSSD.Delete(TempStringSSD.GetLength() -1, 1);
		TempStringSSDP = TempStringSSD.GetBuffer(TempStringSSD.GetLength());
		if (vLocalConfigurationData->vScanTracProductionReportDirectory.GetLength() > 4)
		if (!(CFile::GetStatus(TempStringSSDP, TempFileStatus)))
		if (!CreateDirectory(vLocalConfigurationData->vScanTracProductionReportDirectory,NULL))
		{
			CNoticeDialog TempNoticeDialog1;
			CString TempText("\n\nError, Could not create directory:\n");
			TempText = TempText + vLocalConfigurationData->vScanTracProductionReportDirectory;
			TempNoticeDialog1.vNoticeText = TempText;
			TempNoticeDialog1.vType = cErrorMessage;
			TempNoticeDialog1.DoModal();
		}
		// kjh 8/28/2012 added to create a shadow directory for production report if files stored on server
		// check if directory was created on C: or D: drive, if not create a directory on C:
		// set bit 3 in variable vGlobalShadowImages if true
		// clear bit 3 if false, cShadowProductionReportBit
		if ((vLocalConfigurationData->vScanTracProductionReportDirectory.GetAt(0) != 'C') && (vLocalConfigurationData->vScanTracProductionReportDirectory.GetAt(0) != 'D') &&
			(vLocalConfigurationData->vScanTracProductionReportDirectory.GetAt(0) != 'c') && (vLocalConfigurationData->vScanTracProductionReportDirectory.GetAt(0) != 'd'))
		{
			SetBit(vGlobalShadowImages,cShadowProductionReportBit);		// not copying files to local directory, try to set up shadow directory
			CString TempStringProductionReportShadowDirectory;
			if  (CheckDiskSpaceFree("C:\\") == 0)
			{
				ClrBit(vGlobalShadowImages,cShadowProductionReportBit);	// don't echo to C: drive
			}
			else
			{
				TempStringProductionReportShadowDirectory = cScanTracProductionReportDirectory;  // "c:\\ScanTracProductionReports\\";					
				if (!(CFile::GetStatus(TempStringProductionReportShadowDirectory, TempFileStatus)))
				{
					BOOL TempDirectoryCreate = CreateDirectory(cScanTracProductionReportDirectory,NULL);
					if (TempDirectoryCreate == false)
						ClrBit(vGlobalShadowImages,cShadowProductionReportBit);	// if directory is not created, no need to shadow
				}
			}
		}
		else
		{
			ClrBit(vGlobalShadowImages,cShadowProductionReportBit);
		}

		//check if director exists, if not, create it
		TempStringSSD = vLocalConfigurationData->vScanTracEventLogDirectory;
		TempStringSSD.Delete(TempStringSSD.GetLength() -1, 1);
		TempStringSSDP = TempStringSSD.GetBuffer(TempStringSSD.GetLength());
		if (vLocalConfigurationData->vScanTracEventLogDirectory.GetLength() > 4)
		if (!(CFile::GetStatus(TempStringSSDP, TempFileStatus)))
		if (!CreateDirectory(vLocalConfigurationData->vScanTracEventLogDirectory,NULL))
		{
			CNoticeDialog TempNoticeDialog1;
			CString TempText("\n\nError, Could not create directory:\n");
			TempText = TempText + vLocalConfigurationData->vScanTracEventLogDirectory;
			TempNoticeDialog1.vNoticeText = TempText;
			TempNoticeDialog1.vType = cErrorMessage;
			TempNoticeDialog1.DoModal();
		}
		// kjh 8/28/2012 added to create a shadow directory for event log if files stored on server
		// check if directory was created on C: or D: drive, if not create a directory on C:
		// set bit 2 in variable vGlobalShadowImages if true
		// clear bit 2 if false, cShadowEventLogBit
		if ((vLocalConfigurationData->vScanTracEventLogDirectory.GetAt(0) != 'C') && (vLocalConfigurationData->vScanTracEventLogDirectory.GetAt(0) != 'D') &&
			(vLocalConfigurationData->vScanTracEventLogDirectory.GetAt(0) != 'c') && (vLocalConfigurationData->vScanTracEventLogDirectory.GetAt(0) != 'd'))
		{
			SetBit(vGlobalShadowImages,cShadowEventLogBit);		// not copying files to local directory, try to set up shadow directory
			CString TempStringEventLogShadowDirectory;
			if  (CheckDiskSpaceFree("C:\\") == 0)
			{
				ClrBit(vGlobalShadowImages,cShadowEventLogBit);	// don't echo to C: drive
			}
			else
			{
				TempStringEventLogShadowDirectory = cScanTracEventLogDirectory;  // "c:\\ScanTracEventLogs\\";					
				if (!(CFile::GetStatus(TempStringEventLogShadowDirectory, TempFileStatus)))
				{
					BOOL TempDirectoryCreate = CreateDirectory(TempStringEventLogShadowDirectory,NULL);
					if (TempDirectoryCreate == false)
						ClrBit(vGlobalShadowImages,cShadowEventLogBit);	// if directory is not created, no need to shadow
				}
			}
		}
		else
		{
			ClrBit(vGlobalShadowImages,cShadowEventLogBit);
		}

		if (vLocalConfigurationData->vWriteRejectsToFiles)
		{
			//check if director exists, if not, create it
			TempStringSSD = vLocalConfigurationData->vScanTracRejectedImagesDirectory;
			TempStringSSD.Delete(TempStringSSD.GetLength() -1, 1);
			TempStringSSDP = TempStringSSD.GetBuffer(TempStringSSD.GetLength());
			if (vLocalConfigurationData->vScanTracRejectedImagesDirectory.GetLength() > 4)
			if (!(CFile::GetStatus(TempStringSSDP, TempFileStatus)))
			if (!CreateDirectory(vLocalConfigurationData->vScanTracRejectedImagesDirectory,NULL))
			{
				CNoticeDialog TempNoticeDialog1;
				CString TempText("\n\nError, Could not create directory:\n");
				TempText = TempText + vLocalConfigurationData->vScanTracRejectedImagesDirectory;
				TempNoticeDialog1.vNoticeText = TempText;
				TempNoticeDialog1.vType = cErrorMessage;
				TempNoticeDialog1.DoModal();
			}
			// kjh 8/28/2012 added to create a shadow directory for Rejected Images if images stored on server
			// check if directory was created on C: or D: drive, if not create a directory on D:
			// set variable bit in vGlobalShadowImages true or false
			if ((vLocalConfigurationData->vScanTracRejectedImagesDirectory.GetAt(0) != 'C') && (vLocalConfigurationData->vScanTracRejectedImagesDirectory.GetAt(0) != 'D') &&
				(vLocalConfigurationData->vScanTracRejectedImagesDirectory.GetAt(0) != 'c') && (vLocalConfigurationData->vScanTracRejectedImagesDirectory.GetAt(0) != 'd'))
			{
				SetBit(vGlobalShadowImages,cShadowRejectedImageBit);		// not copying files to local directory, try to set up shadow directory
				vGlobalCountErrorMsgPossibleDirectoryNotAvailableSent = 0;
				CString TempStringRejectedShadowDirectory;
				if  (CheckDiskSpaceFree("D:\\") == 0)
				{
					TempStringRejectedShadowDirectory = cRejectedImagesDirectoryCDrive;  //"c:\\ScanTracRejectedImages\\";	//
					vGlobalHaveDDrive = false;
				}
				else
				{
					TempStringRejectedShadowDirectory = cRejectedImagesDirectoryDDrive;  //"d:\\ScanTracRejectedImages\\";	//
					vGlobalHaveDDrive = true;
				}
				if (!(CFile::GetStatus(TempStringRejectedShadowDirectory, TempFileStatus)))
				{
					BOOL TempDirectoryCreate = CreateDirectory(TempStringRejectedShadowDirectory,NULL);
					if (TempDirectoryCreate == false)
						ClrBit(vGlobalShadowImages,cShadowRejectedImageBit);		// if directory is not created, no need to shadow
				}
			}
			else
			{
				ClrBit(vGlobalShadowImages,cShadowRejectedImageBit);
			}
		}

		//check if director exists, if not, create it
		TempStringSSD = vLocalConfigurationData->vScanTracImagesDirectory;
		TempStringSSD.Delete(TempStringSSD.GetLength() -1, 1);
		TempStringSSDP = TempStringSSD.GetBuffer(TempStringSSD.GetLength());
		if (vLocalConfigurationData->vScanTracImagesDirectory.GetLength() > 4)
		if (!(CFile::GetStatus(TempStringSSDP, TempFileStatus)))
		if (!CreateDirectory(vLocalConfigurationData->vScanTracImagesDirectory,NULL))
		{
			CNoticeDialog TempNoticeDialog1;
			CString TempText("\n\nError, Could not create directory:\n");
			TempText = TempText + vLocalConfigurationData->vScanTracImagesDirectory;
			TempNoticeDialog1.vNoticeText = TempText;
			TempNoticeDialog1.vType = cErrorMessage;
			TempNoticeDialog1.DoModal();
		}

		//check if director exists, if not, create it
		TempStringSSD = vLocalConfigurationData->vScanTracCalibrationImagesDirectory;
		TempStringSSD.Delete(TempStringSSD.GetLength() -1, 1);
		TempStringSSDP = TempStringSSD.GetBuffer(TempStringSSD.GetLength());
		if (vLocalConfigurationData->vScanTracCalibrationImagesDirectory.GetLength() > 4)
		if (!(CFile::GetStatus(TempStringSSDP, TempFileStatus)))
		if (!CreateDirectory(vLocalConfigurationData->vScanTracCalibrationImagesDirectory,NULL))
		{
			CNoticeDialog TempNoticeDialog1;
			CString TempText("\n\nError, Could not create directory:\n");
			TempText = TempText + vLocalConfigurationData->vScanTracCalibrationImagesDirectory;
			TempNoticeDialog1.vNoticeText = TempText;
			TempNoticeDialog1.vType = cErrorMessage;
			TempNoticeDialog1.DoModal();
		}

		//check if director exists, if not, create it
		TempStringSSD = vLocalConfigurationData->vScanTracSerialLogsDirectory;
		TempStringSSD.Delete(TempStringSSD.GetLength() -1, 1);
		TempStringSSDP = TempStringSSD.GetBuffer(TempStringSSD.GetLength());
		if (vLocalConfigurationData->vScanTracSerialLogsDirectory.GetLength() > 4)
		if (!(CFile::GetStatus(TempStringSSDP, TempFileStatus)))
		if (!CreateDirectory(vLocalConfigurationData->vScanTracSerialLogsDirectory,NULL))
		{
			CNoticeDialog TempNoticeDialog1;
			CString TempText("\n\nError, Could not create directory:\n");
			TempText = TempText + vLocalConfigurationData->vScanTracSerialLogsDirectory;
			TempNoticeDialog1.vNoticeText = TempText;
			TempNoticeDialog1.vType = cErrorMessage;
			TempNoticeDialog1.DoModal();
		}
/*
		//check if director exists, if not, create it
		TempStringSSD = vLocalConfigurationData->vScanTracSetupBackupDirectory;
		TempStringSSD.Delete(TempStringSSD.GetLength() -1, 1);
		TempStringSSDP = TempStringSSD.GetBuffer(TempStringSSD.GetLength());
		if (vLocalConfigurationData->vScanTracSetupBackupDirectory.GetLength() > 4)
		if (!(CFile::GetStatus(TempStringSSDP, TempFileStatus)))
		if (!CreateDirectory(vLocalConfigurationData->vScanTracSetupBackupDirectory,NULL))
		{
			CNoticeDialog TempNoticeDialog1;
			CString TempText("\n\nError, Could not create directory:\n");
			TempText = TempText + vLocalConfigurationData->vScanTracSetupBackupDirectory;
			TempNoticeDialog1.vNoticeText = TempText;
			TempNoticeDialog1.vType = cErrorMessage;
			TempNoticeDialog1.DoModal();
		}
		*/

		CDialog::EndDialog(true);
	}
	
	CDialog::OnTimer(nIDEvent);
}

bool CConfigureDirectoryNames::IsDirectoryDefaultNameFor_C_D_Drive(CString variableDirectory,CString constDirectory)
{
	bool Result = true;

	if (variableDirectory.CompareNoCase(constDirectory) != 0)		// if the strings are the same, done, else compare
	{  //if directory is on c or d drive and not same name return false
		if ((variableDirectory.GetAt(0) == 'C') || (variableDirectory.GetAt(0) == 'D') ||
			(variableDirectory.GetAt(0) == 'c') || (variableDirectory.GetAt(0) == 'd') )
		{
			Result = false;
		}
	}
	return(Result);
}

void CConfigureDirectoryNames::OnDialogtitlestatictext1()
{
	//bring up soft key menu for help, print screen, Image, Clear, and hidden shift key
	BYTE TempHiddenKeys = 8;
	if (!PasswordOK(cTemporaryInspxPassword,false))
		TempHiddenKeys = 10;
	PostThreadMessage(vGlobalCallingThreadID, cDisplaySoftKeyMenu, TempHiddenKeys, TempHiddenKeys);
}
