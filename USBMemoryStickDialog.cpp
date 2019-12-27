// USBMemoryStickDialog.cpp : implementation file
//

#include "stdafx.h"
#include "ScanTrac.h"
#include "USBMemoryStickDialog.h"
#include "afxdialogex.h"
#include "SelectItemDialog.h"
#include "YesNoDialog.h"

// CUSBMemoryStickDialog dialog

IMPLEMENT_DYNAMIC(CUSBMemoryStickDialog, CDialogEx)

extern bool vGlobalInWindowsDialog;
extern DWORD vGlobalCopyFileCount;
extern BYTE vGlobalCopyDone;
extern CString vGlobalCurrentDirectory;
extern double vGlobalHardDriveTotalBytes;
extern int vGlobalPasswordLevel;
extern BYTE vGlobalShiftKeyDown;
extern unsigned long vGlobalDialogTitleColor;

CUSBMemoryStickDialog::CUSBMemoryStickDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUSBMemoryStickDialog::IDD, pParent)
{
	vOldDriveNameC = "Drive 1";
	vOldDriveNameD = "Drive 2";
	vOldDriveNameE = "Drive 3";
	vOldDriveNameF = "Drive 4";
	vOldDriveNameG = "Drive 5";
	vOldDriveNameH = "Drive 6";
	vOldDriveNameI = "Drive 7";

	vNeedToBackupSetup = false;
	vNeedToCopyAllScreenShots = false;
	vCopyProgress = 0;
	vLastSourceDirectory = "-";
	vLastDestinationDirectory = "-";
	vMemoryStickDriveLetter = "None";
	vNumberOfPossibleMemorySticks = 0;
	vOldNumberOfPossibleMemorySticks = 0;
	vHaveAUSBMemoryStick = false;
	vOldHaveAUSBMemoryStick = false;
	vUpdateDisplayTimerHandle = 31315;
	vOldXRayOnNotice = false;

	vOldCSize = 0;
	vOldDSize = 0;
	vOldESize = 0;
	vOldFSize = 0;
	vOldGSize = 0;
	vOldHSize = 0;
	vOldISize = 0;

	vOldCFreeSpace = 0;
	vOldDFreeSpace = 0;
	vOldEFreeSpace = 0;
	vOldFFreeSpace = 0;
	vOldGFreeSpace = 0;
	vOldHFreeSpace = 0;
	vOldIFreeSpace = 0;

	vLocalCWndCollection.Add(this);
	vLocalCWndCollection.Add(&m_PleaseWait);
	vLocalCWndCollection.Add(&m_AllFilesLabel);
	vLocalCWndCollection.Add(&m_Background);
	vLocalCWndCollection.Add(&m_DialogTitleStaticText1);
	vLocalCWndCollection.Add(&m_DriveFreeSpace1);
	vLocalCWndCollection.Add(&m_DriveFreeSpace2);
	vLocalCWndCollection.Add(&m_DriveFreeSpace3);
	vLocalCWndCollection.Add(&m_DriveFreeSpace4);
	vLocalCWndCollection.Add(&m_DriveFreeSpace5);
	vLocalCWndCollection.Add(&m_DriveFreeSpace6);
	vLocalCWndCollection.Add(&m_DriveFreeSpace7);
	vLocalCWndCollection.Add(&m_DriveFreeSpaceLabel);
	vLocalCWndCollection.Add(&m_DriveLetter1);
	vLocalCWndCollection.Add(&m_DriveLetter2);
	vLocalCWndCollection.Add(&m_DriveLetter3);
	vLocalCWndCollection.Add(&m_DriveLetter4);
	vLocalCWndCollection.Add(&m_DriveLetter5);
	vLocalCWndCollection.Add(&m_DriveLetter6);
	vLocalCWndCollection.Add(&m_DriveLetter7);
	vLocalCWndCollection.Add(&m_DriveLetterLabel);
	vLocalCWndCollection.Add(&m_DriveName1);
	vLocalCWndCollection.Add(&m_DriveName2);
	vLocalCWndCollection.Add(&m_DriveName3);
	vLocalCWndCollection.Add(&m_DriveName4);
	vLocalCWndCollection.Add(&m_DriveName5);
	vLocalCWndCollection.Add(&m_DriveName6);
	vLocalCWndCollection.Add(&m_DriveName7);
	vLocalCWndCollection.Add(&m_DriveNameLabel);
	vLocalCWndCollection.Add(&m_DriveSize1);
	vLocalCWndCollection.Add(&m_DriveSize2);
	vLocalCWndCollection.Add(&m_DriveSize3);
	vLocalCWndCollection.Add(&m_DriveSize4);
	vLocalCWndCollection.Add(&m_DriveSize5);
	vLocalCWndCollection.Add(&m_DriveSize6);
	vLocalCWndCollection.Add(&m_DriveSize7);
	vLocalCWndCollection.Add(&m_DriveSizeLabel);
	vLocalCWndCollection.Add(&m_Function1Button);
	vLocalCWndCollection.Add(&m_Function2Button);
	vLocalCWndCollection.Add(&m_Function3Button);
	vLocalCWndCollection.Add(&m_Function4Button);
	vLocalCWndCollection.Add(&m_Function5Button);
	vLocalCWndCollection.Add(&m_MemoryStickDriveLabel);
	vLocalCWndCollection.Add(&m_SubFunction1Button);
	vLocalCWndCollection.Add(&m_SubFunction2Button);
	vLocalCWndCollection.Add(&m_SubFunction3Button);
	vLocalCWndCollection.Add(&m_SubFunction4Button);
	vLocalCWndCollection.Add(&m_SubFunction5Button);
	vLocalCWndCollection.Add(&m_SubFunction6Button);
	vLocalCWndCollection.Add(&m_SubFunction7Button);
	vLocalCWndCollection.Add(&m_SubFunction8Button);
	vLocalCWndCollection.Add(&m_TodaysFilesLabel);
	vLocalCWndCollection.Add(&m_XRaysOn);
}

CUSBMemoryStickDialog::~CUSBMemoryStickDialog()
{
}

void CUSBMemoryStickDialog::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	m_Background.SetWindowPos(NULL,cMainBackgroundLeft,
		cMainBackgroundTop,cMainBackgroundLength,
		cMainBackgroundHeight,SWP_NOZORDER);

	// TODO: Add your message handler code here
	UpdateDisplay();

	this->SetWindowText(_T("USB Memory"));
	::SetupMenu(vLocalCWndCollection);
	int TempTimerResult = SetTimer(vUpdateDisplayTimerHandle,1000,NULL);
	if (!TempTimerResult)
		ReportErrorMessage("Error-Display Timer Failed",cEMailInspx,32000);
}


void CUSBMemoryStickDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_AllFilesLabel, m_AllFilesLabel);
	DDX_Control(pDX, IDC_Background, m_Background);
	DDX_Control(pDX, IDC_DialogTitleStaticText1, m_DialogTitleStaticText1);
	DDX_Control(pDX, IDC_DriveFreeSpace1, m_DriveFreeSpace1);
	DDX_Control(pDX, IDC_DriveFreeSpace2, m_DriveFreeSpace2);
	DDX_Control(pDX, IDC_DriveFreeSpace3, m_DriveFreeSpace3);
	DDX_Control(pDX, IDC_DriveFreeSpace4, m_DriveFreeSpace4);
	DDX_Control(pDX, IDC_DriveFreeSpace5, m_DriveFreeSpace5);
	DDX_Control(pDX, IDC_DriveFreeSpace6, m_DriveFreeSpace6);
	DDX_Control(pDX, IDC_DriveFreeSpace7, m_DriveFreeSpace7);
	DDX_Control(pDX, IDC_DriveFreeSpaceLabel, m_DriveFreeSpaceLabel);
	DDX_Control(pDX, IDC_DriveLetter1, m_DriveLetter1);
	DDX_Control(pDX, IDC_DriveLetter2, m_DriveLetter2);
	DDX_Control(pDX, IDC_DriveLetter3, m_DriveLetter3);
	DDX_Control(pDX, IDC_DriveLetter4, m_DriveLetter4);
	DDX_Control(pDX, IDC_DriveLetter5, m_DriveLetter5);
	DDX_Control(pDX, IDC_DriveLetter6, m_DriveLetter6);
	DDX_Control(pDX, IDC_DriveLetter7, m_DriveLetter7);
	DDX_Control(pDX, IDC_DriveLetterLabel, m_DriveLetterLabel);
	DDX_Control(pDX, IDC_DriveName1, m_DriveName1);
	DDX_Control(pDX, IDC_DriveName2, m_DriveName2);
	DDX_Control(pDX, IDC_DriveName3, m_DriveName3);
	DDX_Control(pDX, IDC_DriveName4, m_DriveName4);
	DDX_Control(pDX, IDC_DriveName5, m_DriveName5);
	DDX_Control(pDX, IDC_DriveName6, m_DriveName6);
	DDX_Control(pDX, IDC_DriveName7, m_DriveName7);
	DDX_Control(pDX, IDC_DriveNameLabel, m_DriveNameLabel);
	DDX_Control(pDX, IDC_DriveSize1, m_DriveSize1);
	DDX_Control(pDX, IDC_DriveSize2, m_DriveSize2);
	DDX_Control(pDX, IDC_DriveSize3, m_DriveSize3);
	DDX_Control(pDX, IDC_DriveSize4, m_DriveSize4);
	DDX_Control(pDX, IDC_DriveSize5, m_DriveSize5);
	DDX_Control(pDX, IDC_DriveSize6, m_DriveSize6);
	DDX_Control(pDX, IDC_DriveSize7, m_DriveSize7);
	DDX_Control(pDX, IDC_DriveSizeLabel, m_DriveSizeLabel);
	DDX_Control(pDX, IDC_Function1Button, m_Function1Button);
	DDX_Control(pDX, IDC_Function2Button, m_Function2Button);
	DDX_Control(pDX, IDC_Function3Button, m_Function3Button);
	DDX_Control(pDX, IDC_Function4Button, m_Function4Button);
	DDX_Control(pDX, IDC_Function5Button, m_Function5Button);
	DDX_Control(pDX, IDC_MemoryStickDriveLabel, m_MemoryStickDriveLabel);
	DDX_Control(pDX, IDC_SubFunction1Button, m_SubFunction1Button);
	DDX_Control(pDX, IDC_SubFunction2Button, m_SubFunction2Button);
	DDX_Control(pDX, IDC_SubFunction3Button, m_SubFunction3Button);
	DDX_Control(pDX, IDC_SubFunction4Button, m_SubFunction4Button);
	DDX_Control(pDX, IDC_SubFunction5Button, m_SubFunction5Button);
	DDX_Control(pDX, IDC_SubFunction6Button, m_SubFunction6Button);
	DDX_Control(pDX, IDC_SubFunction7Button, m_SubFunction7Button);
	DDX_Control(pDX, IDC_SubFunction8Button, m_SubFunction8Button);
	DDX_Control(pDX, IDC_TodaysFilesLabel, m_TodaysFilesLabel);
	DDX_Control(pDX, IDC_XRaysOn, m_XRaysOn);
	DDX_Control(pDX, IDC_PleaseWait, m_PleaseWait);
}


BEGIN_MESSAGE_MAP(CUSBMemoryStickDialog, CDialogEx)
	ON_STN_CLICKED(IDC_Function1Button, &CUSBMemoryStickDialog::OnStnClickedFunction1button)
	ON_STN_CLICKED(IDC_Function2Button, &CUSBMemoryStickDialog::OnStnClickedFunction2button)
	ON_STN_CLICKED(IDC_Function3Button, &CUSBMemoryStickDialog::OnStnClickedFunction3button)
	ON_STN_CLICKED(IDC_Function4Button, &CUSBMemoryStickDialog::OnStnClickedFunction4button)
	ON_STN_CLICKED(IDC_Function5Button, &CUSBMemoryStickDialog::OnStnClickedFunction5button)
	ON_STN_CLICKED(IDC_SubFunction1Button, &CUSBMemoryStickDialog::OnStnClickedSubfunction1button)
	ON_STN_CLICKED(IDC_SubFunction2Button, &CUSBMemoryStickDialog::OnStnClickedSubfunction2button)
	ON_STN_CLICKED(IDC_SubFunction3Button, &CUSBMemoryStickDialog::OnStnClickedSubfunction3button)
	ON_STN_CLICKED(IDC_SubFunction4Button, &CUSBMemoryStickDialog::OnStnClickedSubfunction4button)
	ON_STN_CLICKED(IDC_SubFunction5Button, &CUSBMemoryStickDialog::OnStnClickedSubfunction5button)
	ON_STN_CLICKED(IDC_SubFunction6Button, &CUSBMemoryStickDialog::OnStnClickedSubfunction6button)
	ON_STN_CLICKED(IDC_SubFunction7Button, &CUSBMemoryStickDialog::OnStnClickedSubfunction7button)
	ON_STN_CLICKED(IDC_SubFunction8Button, &CUSBMemoryStickDialog::OnStnClickedSubfunction8button)
	ON_WM_CTLCOLOR()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_STN_CLICKED(IDC_DialogTitleStaticText1, &CUSBMemoryStickDialog::OnStnClickedDialogtitlestatictext1)
END_MESSAGE_MAP()


// CUSBMemoryStickDialog message handlers


void CUSBMemoryStickDialog::OnStnClickedFunction1button()
{
	// Main Menu Button Pressed
	CDialog::EndDialog(10);
}


void CUSBMemoryStickDialog::OnStnClickedFunction2button()
{
	// copy all folders
	//OnStnClickedSubfunction2button();
	//OnStnClickedSubfunction3button();
	//OnStnClickedSubfunction4button();
	//OnStnClickedFunction5button();

	OnStnClickedSubfunction1button(); //production report gets everything in one file
	CopyFilesToUSBStick(vLocalConfigurationData->vScanTracEventLogDirectory, false);
	CopySavedImagesToUSBStick(vLocalConfigurationData->vScanTracRejectedImagesDirectory, false);
	CopyFilesToUSBStick(vLocalConfigurationData->vScanTracScreenShotDirectory, false);
	OnStnClickedFunction5button();
}


void CUSBMemoryStickDialog::OnStnClickedFunction3button()
{
	// Restore Setup Files From USB Stick
	if (vLocalSystemData->vSystemRunMode != cStoppedSystemMode)
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\nX-Rays must be off\nto restore program and setup files from USB Drive";
		TempNoticeDialog.vType = cInformationMessage;
		TempNoticeDialog.DoModal();
	}
	else
	{
		CYesNoDialog TempYesNoDialog;
		TempYesNoDialog.vNoticeText = _T("Do you want to restore ScanTrac program and\nsetup files from USB Memory Stick?\nThis will reboot computer to complete.\nAny changes to setups after backup was made\nwill be lost.");
		TempYesNoDialog.vQuestionType = cConfirmAbandon;
		TempYesNoDialog.vYesButtonText = "Continue";
		TempYesNoDialog.vNoButtonText = "Abort";
		int TempResult = TempYesNoDialog.DoModal();
		//if user said OK
		if (TempResult == IDOK)
			RestoreSetupsFromBackup();
	}
}


void CUSBMemoryStickDialog::OnStnClickedFunction4button()
{
	//Select drive button pressed when more than one drive available
	if (vNumberOfPossibleMemorySticks > 1)
	{
		CSelectItemDialog ISelectItemDialog;  
		//Set dialog box data titles and number value
		ISelectItemDialog.vTitleString = "Choose USB Memory Stick";
		ISelectItemDialog.m_DialogTitleStaticText2 = "";
		ISelectItemDialog.m_DialogTitleStaticText3 = "";
		ISelectItemDialog.vBackgroundColor = cGreen;
		//ISelectItemDialog.vGreenItem = 1;
		ISelectItemDialog.vShowExit = 0;
		ISelectItemDialog.vInAutoSetup = vLocalSystemData->vInAutoSetup;

		ISelectItemDialog.vNumberOfItems = vNumberOfPossibleMemorySticks;
		BYTE TempIndexToUse = 0;

		double TempDriveSpaceFree = CheckDiskSpaceFree("C:\\");
		if (vGlobalHardDriveTotalBytes > 1000000) //greater than a megabyte
		if (vGlobalHardDriveTotalBytes < 20000000000) //less than 35 gigabytes
			ISelectItemDialog.vItems[TempIndexToUse++] = "C:\\";

		TempDriveSpaceFree = CheckDiskSpaceFree("D:\\");
		if (vGlobalHardDriveTotalBytes > 1000000) //greater than a megabyte
		if (vGlobalHardDriveTotalBytes < 35000000000) //less than 35 gigabytes
			ISelectItemDialog.vItems[TempIndexToUse++] = "D:\\";

		TempDriveSpaceFree = CheckDiskSpaceFree("E:\\");
		if (vGlobalHardDriveTotalBytes > 1000000) //greater than a megabyte
			ISelectItemDialog.vItems[TempIndexToUse++] = "E:\\";

		TempDriveSpaceFree = CheckDiskSpaceFree("F:\\");
		if (vGlobalHardDriveTotalBytes > 1000000) //greater than a megabyte
			ISelectItemDialog.vItems[TempIndexToUse++] = "F:\\";

		TempDriveSpaceFree = CheckDiskSpaceFree("G:\\");
			ISelectItemDialog.vItems[TempIndexToUse++] = "G:\\";

		TempDriveSpaceFree = CheckDiskSpaceFree("H:\\");
		if (vGlobalHardDriveTotalBytes > 1000000) //greater than a megabyte
			ISelectItemDialog.vItems[TempIndexToUse++] = "H:\\";

		TempDriveSpaceFree = CheckDiskSpaceFree("I:\\");
			ISelectItemDialog.vItems[TempIndexToUse++] = "I:\\";

		//ISelectItemDialog.vMakeTitle3Big = true;
		ISelectItemDialog.m_DialogTitleStaticText3 = "Original Value: " + vMemoryStickDriveLetter;

		//Pass control to dialog box and display
		int nResponse = ISelectItemDialog.DoModal();
		//dialog box is now closed, if user pressed select do this
		//if user pressed cancel, do nothing
		if (nResponse == IDOK)
		{
			vMemoryStickDriveLetter = ISelectItemDialog.vItems[ISelectItemDialog.vItemNumberSelected - 1];
			SetDlgItemText(IDC_MemoryStickDriveLabel, "Memory Stick on Drive Letter " + vMemoryStickDriveLetter);
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
		CNoticeDialog TempNoticeDialog;
		if (vNumberOfPossibleMemorySticks == 0)
			TempNoticeDialog.vNoticeText = "\n\n\nNo USB Drives Detected";
		else
			TempNoticeDialog.vNoticeText = "\n\n\nOnly USB Drive Detected is Selected";
		TempNoticeDialog.vType = cInformationMessage;
		TempNoticeDialog.DoModal();
	}
}


void CUSBMemoryStickDialog::OnStnClickedFunction5button()
{
	//backup setup files
	if (vGlobalCopyDone)
		vNeedToBackupSetup = true;
	else
	{
		CString TempSourceDirectory = vGlobalCurrentDirectory;
		CString TempDestinationFolder = " ScanTrac Backup";
		CTime TempTime = CTime::GetCurrentTime();
		CString TempTimeString = TempTime.Format("%Y-%m-%d");
		TempDestinationFolder = TempTimeString + TempDestinationFolder;

		vGlobalCopyFileCount = 0;  //setup variables to give message when done
		vGlobalCopyDone = 0xAA;
		vLastSourceDirectory = TempSourceDirectory;
		vLastDestinationDirectory = vMemoryStickDriveLetter + TempDestinationFolder;

		CopyFolderToUSBStick(TempSourceDirectory, TempDestinationFolder);
	}
}


void CUSBMemoryStickDialog::OnStnClickedSubfunction1button()
{
	//Copy todays production reports to Memory Stick
	CString TempSourceDirectory = vLocalConfigurationData->vScanTracProductionReportDirectory;
	CString TempDestinationFolder = vLocalConfigurationData->vScanTracProductionReportDirectory;
	TempDestinationFolder.Delete(0, 3);
	//TempDestinationFolder.Delete(TempDestinationFolder.GetLength() - 1, 2);

	CString TempFileName = "ScanTracProductionReportSummary.CSV";
	m_PleaseWait.ShowWindow(SW_SHOW);
	SetDlgItemText(IDC_PleaseWait, "Copying: " + TempFileName);
	CopyFileToUSBStick(TempSourceDirectory, TempFileName, TempDestinationFolder);
	Sleep(1000);
	m_PleaseWait.ShowWindow(SW_HIDE);
}

void CUSBMemoryStickDialog::OnStnClickedSubfunction2button()
{
	CopyFilesToUSBStick(vLocalConfigurationData->vScanTracScreenShotDirectory, false);
	//Copy Today's Screen Shots
	//CString TempSourceDirectory = vLocalConfigurationData->vScanTracScreenShotDirectory;
	//CString TempDestinationFolder = vLocalConfigurationData->vScanTracScreenShotDirectory;
	//TempDestinationFolder.Delete(0, 3); //remove the c:
	//TempDestinationFolder.Delete(TempDestinationFolder.GetLength() - 1, 2);
	////TempDestinationFolder = vMemoryStickDriveLetter + TempDestinationFolder;
	//TempSourceDirectory.Delete(TempSourceDirectory.GetLength() - 1, 2);

	//CTime TempTime = CTime::GetCurrentTime();
	//CString TempTodaysDateString = TempTime.Format("%Y-%m-%d");
	//CString TempSourceFindDirectory = vLocalConfigurationData->vScanTracScreenShotDirectory + "*" + TempTodaysDateString + "*.*";

	//WIN32_FIND_DATA FindFileData;
	//HANDLE hFind;

	//LPTSTR TempDirectoryName = TempSourceFindDirectory.GetBuffer(TempSourceFindDirectory.GetLength());

	//hFind = FindFirstFile(TempDirectoryName, &FindFileData);

	//if (hFind == INVALID_HANDLE_VALUE) 
	//{
	//	CNoticeDialog TempNoticeDialog;
	//	TempNoticeDialog.vNoticeText = "\n\n\nNo files to copy in\n" + TempSourceFindDirectory;
	//	TempNoticeDialog.vType = cInformationMessage;
	//	TempNoticeDialog.DoModal();
	//} 
	//else 
	//{
	//	CString TempFileNameString = FindFileData.cFileName;
	//	if (!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
	//	{
	//		//int TempPosition = TempFileNameString.Find(TempTodaysDateString);
	//		//if (TempPosition >= 0)
	//		//{
	//			CString TempFileNameText = FindFileData.cFileName;
	//			m_PleaseWait.ShowWindow(SW_SHOW);
	//			SetDlgItemText(IDC_PleaseWait, "Copying: " + TempFileNameText);
 //				CopyFileToUSBStick(TempSourceDirectory, TempFileNameText, TempDestinationFolder);
	//		//}
	//	}
	//	while (FindNextFile(hFind, &FindFileData))
	//	{
	//		CString TempFileNameString = FindFileData.cFileName;
	//		if (!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
	//		{
	//			//int TempPosition = TempFileNameString.Find(TempTodaysDateString);
	//			//if (TempPosition >= 0)
	//			//{
	//				CString TempFileNameText = FindFileData.cFileName;
	//				m_PleaseWait.ShowWindow(SW_SHOW);
	//				SetDlgItemText(IDC_PleaseWait, "Copying: " + TempFileNameText);
	//				CopyFileToUSBStick(TempSourceDirectory, TempFileNameText, TempDestinationFolder);
	//				Sleep(10);
	//				//if (vMainWindowPointer)
	//				//	vMainWindowPointer->TickleWatchDog();
	//			//}
	//		}
	//	}
	//}
	//FindClose(hFind);
	//Sleep(1000);
	//m_PleaseWait.ShowWindow(SW_HIDE);
}


void CUSBMemoryStickDialog::OnStnClickedSubfunction3button()
{
	// Copy todays saved images 
		// Copy todays saved images 
	CopySavedImagesToUSBStick(vLocalConfigurationData->vScanTracRejectedImagesDirectory, false);
//CTime TempTime = CTime::GetCurrentTime();
	//CString TempString = TempTime.Format("%Y-%m-%d");
	//CString TempSourceDirectory = vLocalConfigurationData->vScanTracRejectedImagesDirectory + TempString;
	//CString TempDestinationFolder = vLocalConfigurationData->vScanTracRejectedImagesDirectory + TempString;
	//TempDestinationFolder.Delete(0, 3);

	////make sure USB Stick has ScanTracRejectedImages folder
	//CString TempPrimaryFolderName = vLocalConfigurationData->vScanTracRejectedImagesDirectory;
	//TempPrimaryFolderName.Delete(0, 3);
	//TempPrimaryFolderName.Delete(TempPrimaryFolderName.GetLength() - 1, 2);
	//TempPrimaryFolderName = vMemoryStickDriveLetter + TempPrimaryFolderName;
	//CFile TempFile;
	//CFileStatus TempFileStatus;
	//bool TempDestinationDirectoryOK = true;
	//if (!CFile::GetStatus(TempPrimaryFolderName, TempFileStatus))
	//if (!CreateDirectory(TempPrimaryFolderName,NULL))
	//{
	//	TempDestinationDirectoryOK = false;
	//	CNoticeDialog TempNoticeDialog1;
	//	CString TempText("\n\nError, Could not create directory:\n");
	//	TempText = TempText + TempPrimaryFolderName;
	//	TempNoticeDialog1.vNoticeText = TempText;
	//	TempNoticeDialog1.vType = cErrorMessage;
	//	TempNoticeDialog1.DoModal();
	//}

	//if (TempDestinationDirectoryOK)
	//{
	//	vGlobalCopyFileCount = 0;  //setup variables to give message when done
	//	vGlobalCopyDone = 0xAA;
	//	vLastSourceDirectory = TempSourceDirectory;
	//	vLastDestinationDirectory = vMemoryStickDriveLetter + TempDestinationFolder;

	//	CopyFolderToUSBStick(TempSourceDirectory, TempDestinationFolder);
	//}
}


void CUSBMemoryStickDialog::OnStnClickedSubfunction4button()
{
	//Copy Today's Event logs
	CopyFilesToUSBStick(vLocalConfigurationData->vScanTracEventLogDirectory, false);
	//CString TempSourceDirectory = vLocalConfigurationData->vScanTracEventLogDirectory;
	//CString TempDestinationFolder = vLocalConfigurationData->vScanTracEventLogDirectory;
	//TempDestinationFolder.Delete(0, 3); //remove the c:
	//TempDestinationFolder.Delete(TempDestinationFolder.GetLength() - 1, 2);
	//TempSourceDirectory.Delete(TempSourceDirectory.GetLength() - 1, 2);

	//CTime TempTime = CTime::GetCurrentTime();
	//CString TempTodaysDateString = TempTime.Format("%Y-%m-%d");
	//CString TempSourceFindDirectory = vLocalConfigurationData->vScanTracEventLogDirectory + "*" + TempTodaysDateString + "*.*";

	//WIN32_FIND_DATA FindFileData;
	//HANDLE hFind;

	//LPTSTR TempDirectoryName = TempSourceFindDirectory.GetBuffer(TempSourceFindDirectory.GetLength());

	//hFind = FindFirstFile(TempDirectoryName, &FindFileData);

	//if (hFind == INVALID_HANDLE_VALUE)
	//{
	//	CNoticeDialog TempNoticeDialog;
	//	TempNoticeDialog.vNoticeText = "\n\n\nNo files to copy in\n" + TempSourceFindDirectory;
	//	TempNoticeDialog.vType = cInformationMessage;
	//	TempNoticeDialog.DoModal();
	//}
	//else
	//{
	//	CString TempFileNameString = FindFileData.cFileName;
	//	if (!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
	//	{
	//		CString TempFileNameText = FindFileData.cFileName;
	//		m_PleaseWait.ShowWindow(SW_SHOW);
	//		SetDlgItemText(IDC_PleaseWait, "Copying: " + TempFileNameText);
	//		CopyFileToUSBStick(TempSourceDirectory, TempFileNameText, TempDestinationFolder);
	//	}
	//	while (FindNextFile(hFind, &FindFileData))
	//	{
	//		CString TempFileNameString = FindFileData.cFileName;
	//		if (!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
	//		{
	//			CString TempFileNameText = FindFileData.cFileName;
	//			m_PleaseWait.ShowWindow(SW_SHOW);
	//			SetDlgItemText(IDC_PleaseWait, "Copying: " + TempFileNameText);
	//			CopyFileToUSBStick(TempSourceDirectory, TempFileNameText, TempDestinationFolder);
	//			Sleep(10);
	//		}
	//	}
	//}
	//FindClose(hFind);
	//Sleep(1000);
	//m_PleaseWait.ShowWindow(SW_HIDE);
}

void CUSBMemoryStickDialog::OnStnClickedSubfunction5button()
{
	//copy all folders
	OnStnClickedSubfunction6button();
	OnStnClickedSubfunction7button();
	OnStnClickedFunction5button();
}


void CUSBMemoryStickDialog::OnStnClickedSubfunction6button()
{
	//Copy All Files production reports to Memory Stick
	CString TempSourceDirectory = vLocalConfigurationData->vScanTracProductionReportDirectory;
	CString TempDestinationFolder = vLocalConfigurationData->vScanTracProductionReportDirectory;
	TempDestinationFolder.Delete(0, 3); //remove the c:\

	vGlobalCopyFileCount = 0;  //setup variables to give message when done
	vGlobalCopyDone = 0xAA;
	vLastSourceDirectory = TempSourceDirectory;
	vLastDestinationDirectory = vMemoryStickDriveLetter + TempDestinationFolder;

	CopyFolderToUSBStick(TempSourceDirectory, TempDestinationFolder);
}


void CUSBMemoryStickDialog::OnStnClickedSubfunction7button()
{
	if (vGlobalCopyDone)
		vNeedToCopyAllScreenShots = true;
	else
		{
		//Copy All Screen Shots
		CString TempSourceDirectory = vLocalConfigurationData->vScanTracScreenShotDirectory;
		CString TempDestinationFolder = vLocalConfigurationData->vScanTracScreenShotDirectory;
		TempDestinationFolder.Delete(0, 3); //remove the c:\

		vGlobalCopyFileCount = 0;  //setup variables to give message when done
		vGlobalCopyDone = 0xAA;
		vLastSourceDirectory = TempSourceDirectory;
		vLastDestinationDirectory = vMemoryStickDriveLetter + TempDestinationFolder;

		CopyFolderToUSBStick(TempSourceDirectory, TempDestinationFolder);
	}
}


void CUSBMemoryStickDialog::OnStnClickedSubfunction8button()
{
	// copy Yesterdays files
	OnStnClickedSubfunction1button(); //production report gets everything in one file
	CopyFilesToUSBStick(vLocalConfigurationData->vScanTracEventLogDirectory, true);
	CopyFilesToUSBStick(vLocalConfigurationData->vScanTracScreenShotDirectory, true);
	CopySavedImagesToUSBStick(vLocalConfigurationData->vScanTracRejectedImagesDirectory, true);
	OnStnClickedFunction5button(); //production report gets everything in one file
}


HBRUSH CUSBMemoryStickDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here
	HWND TempDialogHwnd = this->m_hWnd;
	int TextSize;
		pDC->SetAttribDC(pDC->m_hDC);
	pDC->SetBkMode(TRANSPARENT);
		
	if (pWnd->GetDlgCtrlID() == IDC_DialogTitleStaticText1)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DialogTitleStaticText1, 8);  //5 is medium large
		if (vGlobalShiftKeyDown)
			pDC->SetTextColor(cBlack);
		else
			pDC->SetTextColor(vGlobalDialogTitleColor);
	}

	if (pWnd->GetDlgCtrlID() == IDC_Function1Button)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function1Button, 5);  //5 is medium large
		pDC->SetAttribDC(pDC->m_hDC);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}

	if (pWnd->GetDlgCtrlID() == IDC_Function2Button)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function2Button, 5);  //5 is medium large
		pDC->SetAttribDC(pDC->m_hDC);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}

	if (pWnd->GetDlgCtrlID() == IDC_Function3Button)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function3Button, 5);  //5 is medium large
		pDC->SetAttribDC(pDC->m_hDC);
		pDC->SetBkMode(TRANSPARENT);

		if (vLocalSystemData->vSystemRunMode == cStoppedSystemMode)
			pDC->SetTextColor(cButtonTextColor);
		else
			pDC->SetTextColor(cGray);

		return vLocalSystemData->vYellowBrush;
	}

	if (pWnd->GetDlgCtrlID() == IDC_Function4Button)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function4Button, 5);  //5 is medium large
		pDC->SetAttribDC(pDC->m_hDC);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_Function5Button)
	{
		TextSize = SetTextSizeFunctionButton(TempDialogHwnd, pWnd, pDC, &m_Function5Button, 5);  //5 is medium large
		pDC->SetAttribDC(pDC->m_hDC);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}

	if (pWnd->GetDlgCtrlID() == IDC_SubFunction1Button)
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction1Button, 5);  //5 is medium large
		pDC->SetAttribDC(pDC->m_hDC);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}

	if (pWnd->GetDlgCtrlID() == IDC_SubFunction2Button) 
	{   
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction2Button, 5);  //5 is medium large
		pDC->SetAttribDC(pDC->m_hDC);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction3Button) 
	{   
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction3Button, 5);  //5 is medium large
		pDC->SetAttribDC(pDC->m_hDC);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction4Button) 
	{   
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction4Button, 5);  //5 is medium large
		pDC->SetAttribDC(pDC->m_hDC);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction5Button)
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction5Button, 5);  //5 is medium large
		pDC->SetAttribDC(pDC->m_hDC);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction6Button) 
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction6Button, 5);  //5 is medium large
		pDC->SetAttribDC(pDC->m_hDC);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction7Button) 
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction7Button, 5);  //5 is medium large
		pDC->SetAttribDC(pDC->m_hDC);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_SubFunction8Button)
	{
		TextSize = SetTextSizeSubFunctionButton(TempDialogHwnd, pWnd, pDC, &m_SubFunction8Button, 5);  //5 is medium large
		pDC->SetAttribDC(pDC->m_hDC);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cButtonTextColor);
		return vGlobalButtonColorBrush;
	}

	if (pWnd->GetDlgCtrlID() == IDC_Background)
	{
		pDC->SetAttribDC(pDC->m_hDC);
		pDC->SetBkMode(TRANSPARENT);
		return vLocalSystemData->vLightBrownBrush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_XRaysOn)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_XRaysOn, 7);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cYellow);
		pDC->SetBkColor(cRed);
		return vLocalSystemData->vRedBrush;
	}
	
	if (pWnd->GetDlgCtrlID() == IDC_PleaseWait)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_PleaseWait, 4);
		pDC->SetAttribDC(pDC->m_hDC);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cRed);
		return vLocalSystemData->vYellowBrush;
  }

	if (pWnd->GetDlgCtrlID() == IDC_AllFilesLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_AllFilesLabel, 4);
		pDC->SetAttribDC(pDC->m_hDC);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBlack);
		//return vLocalSystemData->vLightBrownBrush;
  }

	if (pWnd->GetDlgCtrlID() == IDC_DriveFreeSpace1)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DriveFreeSpace1, 4);
		pDC->SetAttribDC(pDC->m_hDC);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBlack);
		return vLocalSystemData->vLightBrownBrush;
  }

	if (pWnd->GetDlgCtrlID() == IDC_DriveFreeSpace2)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DriveFreeSpace2, 4);
		pDC->SetAttribDC(pDC->m_hDC);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBlack);
		return vLocalSystemData->vLightBrownBrush;
  }

	if (pWnd->GetDlgCtrlID() == IDC_DriveFreeSpace3)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DriveFreeSpace3, 4);
		pDC->SetAttribDC(pDC->m_hDC);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBlack);
		return vLocalSystemData->vLightBrownBrush;
  }

	if (pWnd->GetDlgCtrlID() == IDC_DriveFreeSpace4)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DriveFreeSpace4, 4);
		pDC->SetAttribDC(pDC->m_hDC);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBlack);
		return vLocalSystemData->vLightBrownBrush;
  }

	if (pWnd->GetDlgCtrlID() == IDC_DriveFreeSpace5)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DriveFreeSpace5, 4);
		pDC->SetAttribDC(pDC->m_hDC);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBlack);
		return vLocalSystemData->vLightBrownBrush;
  }

	if (pWnd->GetDlgCtrlID() == IDC_DriveFreeSpace6)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DriveFreeSpace6, 4);
		pDC->SetAttribDC(pDC->m_hDC);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBlack);
		return vLocalSystemData->vLightBrownBrush;
  }

	if (pWnd->GetDlgCtrlID() == IDC_DriveFreeSpace7)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DriveFreeSpace7, 4);
		pDC->SetAttribDC(pDC->m_hDC);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBlack);
		return vLocalSystemData->vLightBrownBrush;
  }

	if (pWnd->GetDlgCtrlID() == IDC_DriveFreeSpaceLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DriveFreeSpaceLabel, 4);
		pDC->SetAttribDC(pDC->m_hDC);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBlack);
		return vLocalSystemData->vLightBrownBrush;
  }

	if (pWnd->GetDlgCtrlID() == IDC_DriveLetter1)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DriveLetter1, 4);
		pDC->SetAttribDC(pDC->m_hDC);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBlack);
		return vLocalSystemData->vLightBrownBrush;
  }

	if (pWnd->GetDlgCtrlID() == IDC_DriveLetter2)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DriveLetter2, 4);
		pDC->SetAttribDC(pDC->m_hDC);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBlack);
		return vLocalSystemData->vLightBrownBrush;
  }

	if (pWnd->GetDlgCtrlID() == IDC_DriveLetter3)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DriveLetter3, 4);
		pDC->SetAttribDC(pDC->m_hDC);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBlack);
		return vLocalSystemData->vLightBrownBrush;
  }

	if (pWnd->GetDlgCtrlID() == IDC_DriveLetter4)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DriveLetter4, 4);
		pDC->SetAttribDC(pDC->m_hDC);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBlack);
		return vLocalSystemData->vLightBrownBrush;
  }

	if (pWnd->GetDlgCtrlID() == IDC_DriveLetter5)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DriveLetter5, 4);
		pDC->SetAttribDC(pDC->m_hDC);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBlack);
		return vLocalSystemData->vLightBrownBrush;
  }

	if (pWnd->GetDlgCtrlID() == IDC_DriveLetter6)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DriveLetter6, 4);
		pDC->SetAttribDC(pDC->m_hDC);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBlack);
		return vLocalSystemData->vLightBrownBrush;
  }

	if (pWnd->GetDlgCtrlID() == IDC_DriveLetter7)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DriveLetter7, 4);
		pDC->SetAttribDC(pDC->m_hDC);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBlack);
		return vLocalSystemData->vLightBrownBrush;
  }

	if (pWnd->GetDlgCtrlID() == IDC_DriveLetterLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DriveLetterLabel, 4);
		pDC->SetAttribDC(pDC->m_hDC);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBlack);
		return vLocalSystemData->vLightBrownBrush;
  }

	if (pWnd->GetDlgCtrlID() == IDC_DriveName1)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DriveName1, 4);
		pDC->SetAttribDC(pDC->m_hDC);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBlack);
		return vLocalSystemData->vLightBrownBrush;
  }

	if (pWnd->GetDlgCtrlID() == IDC_DriveName2)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DriveName2, 4);
		pDC->SetAttribDC(pDC->m_hDC);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBlack);
		return vLocalSystemData->vLightBrownBrush;
  }

	if (pWnd->GetDlgCtrlID() == IDC_DriveName3)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DriveName3, 4);
		pDC->SetAttribDC(pDC->m_hDC);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBlack);
		return vLocalSystemData->vLightBrownBrush;
  }

	if (pWnd->GetDlgCtrlID() == IDC_DriveName4)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DriveName4, 4);
		pDC->SetAttribDC(pDC->m_hDC);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBlack);
		return vLocalSystemData->vLightBrownBrush;
  }

	if (pWnd->GetDlgCtrlID() == IDC_DriveName5)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DriveName5, 4);
		pDC->SetAttribDC(pDC->m_hDC);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBlack);
		return vLocalSystemData->vLightBrownBrush;
  }

	if (pWnd->GetDlgCtrlID() == IDC_DriveName6)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DriveName6, 4);
		pDC->SetAttribDC(pDC->m_hDC);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBlack);
		return vLocalSystemData->vLightBrownBrush;
  }

	if (pWnd->GetDlgCtrlID() == IDC_DriveName7)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DriveName7, 4);
		pDC->SetAttribDC(pDC->m_hDC);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBlack);
		return vLocalSystemData->vLightBrownBrush;
  }

	if (pWnd->GetDlgCtrlID() == IDC_DriveNameLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DriveNameLabel, 4);
		pDC->SetAttribDC(pDC->m_hDC);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBlack);
		return vLocalSystemData->vLightBrownBrush;
  }

	if (pWnd->GetDlgCtrlID() == IDC_DriveSize1)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DriveSize1, 4);
		pDC->SetAttribDC(pDC->m_hDC);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBlack);
		return vLocalSystemData->vLightBrownBrush;
  }

	if (pWnd->GetDlgCtrlID() == IDC_DriveSize2)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DriveSize2, 4);
		pDC->SetAttribDC(pDC->m_hDC);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBlack);
		return vLocalSystemData->vLightBrownBrush;
  }

	if (pWnd->GetDlgCtrlID() == IDC_DriveSize3)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DriveSize3, 4);
		pDC->SetAttribDC(pDC->m_hDC);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBlack);
		return vLocalSystemData->vLightBrownBrush;
  }

	if (pWnd->GetDlgCtrlID() == IDC_DriveSize4)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DriveSize4, 4);
		pDC->SetAttribDC(pDC->m_hDC);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBlack);
		return vLocalSystemData->vLightBrownBrush;
  }

	if (pWnd->GetDlgCtrlID() == IDC_DriveSize5)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DriveSize5, 4);
		pDC->SetAttribDC(pDC->m_hDC);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBlack);
		return vLocalSystemData->vLightBrownBrush;
  }

	if (pWnd->GetDlgCtrlID() == IDC_DriveSize6)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DriveSize6, 4);
		pDC->SetAttribDC(pDC->m_hDC);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBlack);
		return vLocalSystemData->vLightBrownBrush;
  }

	if (pWnd->GetDlgCtrlID() == IDC_DriveSize7)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DriveSize7, 4);
		pDC->SetAttribDC(pDC->m_hDC);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBlack);
		return vLocalSystemData->vLightBrownBrush;
  }

	if (pWnd->GetDlgCtrlID() == IDC_DriveSizeLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_DriveSizeLabel, 4);
		pDC->SetAttribDC(pDC->m_hDC);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBlack);
		return vLocalSystemData->vLightBrownBrush;
  }

	if (pWnd->GetDlgCtrlID() == IDC_MemoryStickDriveLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_MemoryStickDriveLabel, 4);
		pDC->SetAttribDC(pDC->m_hDC);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBlack);
		return vLocalSystemData->vLightBrownBrush;
  }

	if (pWnd->GetDlgCtrlID() == IDC_TodaysFilesLabel)
	{
		TextSize = SetTextSizeSingleLineDisplay(TempDialogHwnd, pWnd, pDC, &m_TodaysFilesLabel, 4);
		pDC->SetAttribDC(pDC->m_hDC);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(cBlack);
		//return vLocalSystemData->vLightBrownBrush;
  }
	// TODO:  Return a different brush if the default is not desired
	return hbr;
}

void CUSBMemoryStickDialog::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
  if (nIDEvent == vUpdateDisplayTimerHandle)
		UpdateDisplay();

	CDialogEx::OnTimer(nIDEvent);
}


BOOL CUSBMemoryStickDialog::PreTranslateMessage(MSG* pMsg)
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
		if (vGlobalShiftKeyDown)
			PostThreadMessage(vGlobalCallingThreadID,cHiddenKeyPressed, 0, pMsg->wParam);
		else
		switch (pMsg->wParam)
		{
			//case 35: // End Key, Left Right on duo but new Image Key on Universal Overlays, Image Button
			//	OnTestdacsbutton();
			//break;
			case 113: // F2:
				OnStnClickedSubfunction5button();
			break;
			case 114: // F3:
				OnStnClickedSubfunction6button();
			break;
			case 115: // F4:
				OnStnClickedSubfunction7button();
			break;
			case 116: // F5:
				OnStnClickedSubfunction1button();
			break;
			case 117: // F6 Key - Function 5:
				OnStnClickedSubfunction4button();
			break;
			case 118: // F7 Key - Sub Function 1:
				OnStnClickedFunction1button();
			break;
			case 119: // F8 Key - Sub Function 2:
				OnStnClickedSubfunction8button();
			break;
			case 120: // F9 Key - Sub Function 3:
				OnStnClickedSubfunction2button();
			break;
			case 122: // F11 Key - Sub Function 4:
				OnStnClickedSubfunction3button();
			break;
			case 111: // / Key - Sub Function 5:
				OnStnClickedFunction2button();
			break;
			case 106: // * Key - Sub Function 6:
				OnStnClickedFunction4button();
			break;
			case 109: // - Key - Sub Function 7  Decimal Point:
				OnStnClickedFunction3button();
			break;
			case 107: // + Key - Sub Function 8:
				OnStnClickedFunction5button();
			break;
			case cPageUp: // Clear, but do inspx hidden dialog
			{
				if (vGlobalPasswordLevel)
				{
					PasswordOK(cNoPassword, false);	//todo, search on PasswordOK (cNoPassword)
				}
				else
				{
					PasswordOK(cAnyPasswordToEnter, true);
				}
			}
			break;	
			case 16: // Shift Key
			case 0x60:  //0 key on KeyPad
				PostThreadMessage(vGlobalCallingThreadID,cHiddenKeyPressed, 1, 0);
			break;
			case 36:  //Home key, Screen Capture
				vGlobalShiftKeyDown = true;
				vGlobalDialogTitleColor = cBlack;
				TempWindow = CWnd::GetDlgItem(IDC_DialogTitleStaticText1);
				if (TempWindow)
					TempWindow->Invalidate(false);
				PostThreadMessage(vGlobalCallingThreadID,cHiddenKeyPressed, 0, 0x61);
			break;
			case cPageDown: // Help
				PostThreadMessage(vGlobalCallingThreadID, cShowHelp, 43, 0);
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

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CUSBMemoryStickDialog::UpdateDisplay()
{
		CString TempSourceFileName = "*.*";
		//CFileStatus TempFileStatus;
		//if (CFile::GetStatus(TempSourceFileName, TempFileStatus))
		//	SetDlgItemText(IDC_DriveName1, TempFileStatus.m_szFullName);

				//WIN32_FIND_DATA FindFileData;
				//HANDLE hFind;

				//CString TempString = "\\\\";
				//hFind = FindFirstFile(TempString, &FindFileData);

				//if (hFind == INVALID_HANDLE_VALUE) 
				//{
				//	CNoticeDialog TempNoticeDialog;
				//	TempNoticeDialog.vNoticeText = "\n\n\nNo files in image directory to delete";
				//	TempNoticeDialog.vType = cInformationMessage;
				//	TempNoticeDialog.DoModal();
				//} 
				//else 
				//	SetDlgItemText(IDC_DriveName1, FindFileData.cFileName);

//				UINT TempDriveType GetDriveType("C:\\");
//To determine whether a drive is a USB-type drive, call SetupDiGetDeviceRegistryProperty and specify the SPDRP_REMOVAL_POLICY property.
TCHAR volumeName[MAX_PATH + 1] = { 0 };
TCHAR fileSystemName[MAX_PATH + 1] = { 0 };
DWORD serialNumber = 0;
DWORD maxComponentLen = 0;
DWORD fileSystemFlags = 0;



//BOOL SetupDiGetDeviceRegistryProperty(
//  _In_      HDEVINFO         DeviceInfoSet,
//  _In_      PSP_DEVINFO_DATA DeviceInfoData,
//  _In_      DWORD            Property,
//  _Out_opt_ PDWORD           PropertyRegDataType,
//  _Out_opt_ PBYTE            PropertyBuffer,
//  _In_      DWORD            PropertyBufferSize,
//  _Out_opt_ PDWORD           RequiredSize
//);
//

//if (!SetupDiGetDeviceRegistryProperty(hDevInfo,
//                                      &spDevInfoData,
//                                      SPDRP_CLASS, //Retrieve property type,
//                                      0L,
//                                      (PBYTE)szBuf,
//                                      2048,
//                                      0)) {
//    wIndex++;
//    continue;

//if (GetVolumeInformation(_T("C:\\"), volumeName, ARRAYSIZE(volumeName), &serialNumber, &maxComponentLen, &fileSystemFlags, fileSystemName, ARRAYSIZE(fileSystemName)))
//{
//	if (vOldDriveNameC != volumeName)
//	{
//		vOldDriveNameC = volumeName;
//		SetDlgItemText(IDC_DriveName1, volumeName);
//	}
//}

//  _In_opt_  LPCTSTR lpRootPathName,
//  _Out_opt_ LPTSTR  lpVolumeNameBuffer,
//  _In_      DWORD   nVolumeNameSize,
//  _Out_opt_ LPDWORD lpVolumeSerialNumber,
//  _Out_opt_ LPDWORD lpMaximumComponentLength,
//  _Out_opt_ LPDWORD lpFileSystemFlags,
//  _Out_opt_ LPTSTR  lpFileSystemNameBuffer,
//  _In_      DWORD   nFileSystemNameSize
//);


	if ((vGlobalCopyDone == 0x55) || (vGlobalCopyDone == 0x54)) //most recent copy operation finished 55=done, copied files, 54=done error so no files copied
	{
		if (vGlobalCopyDone == 0x55)
		{
			vGlobalCopyDone = 0;
			CNoticeDialog TempNoticeDialog1;
			CString TempText = "\n\n" + dtoa(vGlobalCopyFileCount, 0) + " Files Copied\nFrom: " + vLastSourceDirectory + "\nTo: " + vLastDestinationDirectory;
			TempNoticeDialog1.vNoticeText = TempText;
			TempNoticeDialog1.vType = cInformationMessage;
			TempNoticeDialog1.DoModal();
		}
		
		m_PleaseWait.ShowWindow(SW_HIDE);
		vGlobalCopyDone = 0;

		if (vNeedToCopyAllScreenShots)
		{
			vNeedToCopyAllScreenShots = false;
			OnStnClickedSubfunction7button();
		}
		else
		if (vNeedToBackupSetup)
		{
			vNeedToBackupSetup = false;
			OnStnClickedFunction5button();
		}
	}
	if (vGlobalCopyDone == 0xAA) //copying files
	{
		vCopyProgress++;
		if (vCopyProgress > 10)
			vCopyProgress = 0;
		CString TempText = "";
		for (BYTE TempLoop = 0; TempLoop < vCopyProgress; TempLoop++)
			TempText = TempText + ".";

		TempText = "Copying Files, Please Wait" + TempText;

		SetDlgItemText(IDC_PleaseWait, TempText);
	}


	if (vOldXRayOnNotice != vLocalSystemData->vXRayOnNotice)
	{
		vOldXRayOnNotice = vLocalSystemData->vXRayOnNotice;
		if (vOldXRayOnNotice)
			m_XRaysOn.ShowWindow(SW_SHOW);
		else
			m_XRaysOn.ShowWindow(SW_HIDE);
	}

	CString TempNewDriveLetter = "None";
	vHaveAUSBMemoryStick = false;
	vNumberOfPossibleMemorySticks = 0;

	double TempDriveSpaceFree = CheckDiskSpaceFree("C:\\");
	if (vGlobalHardDriveTotalBytes > 1000000) //greater than a megabyte
	if (vGlobalHardDriveTotalBytes < 20000000000) //less than 35 gigabytes
	{
		vHaveAUSBMemoryStick = true;
		TempNewDriveLetter = "C:\\";
		vNumberOfPossibleMemorySticks++;
	}
	if (vOldCFreeSpace != TempDriveSpaceFree)
	{
		vOldCFreeSpace = TempDriveSpaceFree;
		SetDlgItemText(IDC_DriveFreeSpace1, dtoaWithCommas(vOldCFreeSpace / 1000000, 0));
	}
	if (vOldCSize != vGlobalHardDriveTotalBytes)
	{
		vOldCSize = vGlobalHardDriveTotalBytes;
		SetDlgItemText(IDC_DriveSize1, dtoaWithCommas(vOldCSize / 1000000, 0));

		if (!vOldCSize)
			SetDlgItemText(IDC_DriveName1, _T("Drive 1"));
		else
		if (GetVolumeInformation(_T("C:\\"), volumeName, ARRAYSIZE(volumeName), &serialNumber, &maxComponentLen, &fileSystemFlags, fileSystemName, ARRAYSIZE(fileSystemName)))
		{
			SetDlgItemText(IDC_DriveName1, volumeName);
		}
	}
	
	TempDriveSpaceFree = CheckDiskSpaceFree("D:\\");
	if (vGlobalHardDriveTotalBytes > 1000000) //greater than a megabyte
	if (vGlobalHardDriveTotalBytes < 35000000000) //less than 35 gigabytes
	{
		vHaveAUSBMemoryStick = true;
		TempNewDriveLetter = "D:\\";
		vNumberOfPossibleMemorySticks++;
	}
	if (vOldDFreeSpace != TempDriveSpaceFree)
	{
		vOldDFreeSpace = TempDriveSpaceFree;
		SetDlgItemText(IDC_DriveFreeSpace2, dtoaWithCommas(vOldDFreeSpace / 1000000, 0));
	}
	if (vOldDSize != vGlobalHardDriveTotalBytes)
	{
		vOldDSize = vGlobalHardDriveTotalBytes;
		SetDlgItemText(IDC_DriveSize2, dtoaWithCommas(vOldDSize / 1000000, 0));

		if (!vOldDSize)
			SetDlgItemText(IDC_DriveName2, _T("Drive 2"));
		else
		if (GetVolumeInformation(_T("D:\\"), volumeName, ARRAYSIZE(volumeName), &serialNumber, &maxComponentLen, &fileSystemFlags, fileSystemName, ARRAYSIZE(fileSystemName)))
		{
			SetDlgItemText(IDC_DriveName2, volumeName);
		}
	}
	
	TempDriveSpaceFree = CheckDiskSpaceFree("E:\\");
	if (vGlobalHardDriveTotalBytes > 1000000) //greater than a megabyte
	{
		vHaveAUSBMemoryStick = true;
		TempNewDriveLetter = "E:\\";
		vNumberOfPossibleMemorySticks++;
	}
	if (vOldEFreeSpace != TempDriveSpaceFree)
	{
		vOldEFreeSpace = TempDriveSpaceFree;
		SetDlgItemText(IDC_DriveFreeSpace3, dtoaWithCommas(vOldEFreeSpace / 1000000, 0));
	}
	if (vOldESize != vGlobalHardDriveTotalBytes)
	{
		vOldESize = vGlobalHardDriveTotalBytes;
		SetDlgItemText(IDC_DriveSize3, dtoaWithCommas(vOldESize / 1000000, 0));

		if (!vOldESize)
			SetDlgItemText(IDC_DriveName3, _T("Drive 3"));
		else
		if (GetVolumeInformation(_T("E:\\"), volumeName, ARRAYSIZE(volumeName), &serialNumber, &maxComponentLen, &fileSystemFlags, fileSystemName, ARRAYSIZE(fileSystemName)))
		{
			SetDlgItemText(IDC_DriveName3, volumeName);
		}
	}

	TempDriveSpaceFree = CheckDiskSpaceFree("F:\\");
	if (vGlobalHardDriveTotalBytes > 1000000) //greater than a megabyte
	{
		vHaveAUSBMemoryStick = true;
		TempNewDriveLetter = "F:\\";
		vNumberOfPossibleMemorySticks++;
	}
	if (vOldFFreeSpace != TempDriveSpaceFree)
	{
		vOldFFreeSpace = TempDriveSpaceFree;
		SetDlgItemText(IDC_DriveFreeSpace4, dtoaWithCommas(vOldFFreeSpace / 1000000, 0));
	}
	if (vOldFSize != vGlobalHardDriveTotalBytes)
	{
		vOldFSize = vGlobalHardDriveTotalBytes;
		SetDlgItemText(IDC_DriveSize4, dtoaWithCommas(vOldFSize / 1000000, 0));

		if (!vOldFSize)
			SetDlgItemText(IDC_DriveName4, _T("Drive 4"));
		else
		if (GetVolumeInformation(_T("F:\\"), volumeName, ARRAYSIZE(volumeName), &serialNumber, &maxComponentLen, &fileSystemFlags, fileSystemName, ARRAYSIZE(fileSystemName)))
		{
			SetDlgItemText(IDC_DriveName4, volumeName);
		}
	}

	TempDriveSpaceFree = CheckDiskSpaceFree("G:\\");
	if (vGlobalHardDriveTotalBytes > 1000000) //greater than a megabyte
	{
		vHaveAUSBMemoryStick = true;
		TempNewDriveLetter = "G:\\";
		vNumberOfPossibleMemorySticks++;
	}
	if (vOldGFreeSpace != TempDriveSpaceFree)
	{
		vOldGFreeSpace = TempDriveSpaceFree;
		SetDlgItemText(IDC_DriveFreeSpace5, dtoaWithCommas(vOldGFreeSpace / 1000000, 0));
	}
	if (vOldGSize != vGlobalHardDriveTotalBytes)
	{
		vOldGSize = vGlobalHardDriveTotalBytes;
		SetDlgItemText(IDC_DriveSize5, dtoaWithCommas(vOldGSize / 1000000, 0));

		if (!vOldGSize)
			SetDlgItemText(IDC_DriveName5, _T("Drive 5"));
		else
		if (GetVolumeInformation(_T("G:\\"), volumeName, ARRAYSIZE(volumeName), &serialNumber, &maxComponentLen, &fileSystemFlags, fileSystemName, ARRAYSIZE(fileSystemName)))
		{
			SetDlgItemText(IDC_DriveName5, volumeName);
		}
	}

	TempDriveSpaceFree = CheckDiskSpaceFree("H:\\");
	if (vGlobalHardDriveTotalBytes > 1000000) //greater than a megabyte
	{
		vHaveAUSBMemoryStick = true;
		TempNewDriveLetter = "H:\\";
		vNumberOfPossibleMemorySticks++;
	}
	if (vOldHFreeSpace != TempDriveSpaceFree)
	{
		vOldHFreeSpace = TempDriveSpaceFree;
		SetDlgItemText(IDC_DriveFreeSpace6, dtoaWithCommas(vOldHFreeSpace / 1000000, 0));
	}
	if (vOldHSize != TempDriveSpaceFree)
	{
		vOldHSize = vGlobalHardDriveTotalBytes;
		SetDlgItemText(IDC_DriveSize6, dtoaWithCommas(vOldHSize / 1000000, 0));

		if (!vOldHSize)
			SetDlgItemText(IDC_DriveName6, _T("Drive 6"));
		else
		if (GetVolumeInformation(_T("H:\\"), volumeName, ARRAYSIZE(volumeName), &serialNumber, &maxComponentLen, &fileSystemFlags, fileSystemName, ARRAYSIZE(fileSystemName)))
		{
			SetDlgItemText(IDC_DriveName6, vOldDriveNameH);
		}
	}

	TempDriveSpaceFree = CheckDiskSpaceFree("I:\\");
	if (vGlobalHardDriveTotalBytes > 1000000) //greater than a megabyte
	{
		vHaveAUSBMemoryStick = true;
		TempNewDriveLetter = "I:\\";
		vNumberOfPossibleMemorySticks++;
	}
	if (vOldIFreeSpace != TempDriveSpaceFree)
	{
		vOldIFreeSpace = TempDriveSpaceFree;
		SetDlgItemText(IDC_DriveFreeSpace7, dtoaWithCommas(vOldIFreeSpace / 1000000, 0));
	}
	if (vOldISize != vGlobalHardDriveTotalBytes)
	{
		vOldISize = vGlobalHardDriveTotalBytes;
		SetDlgItemText(IDC_DriveSize7, dtoaWithCommas(vOldISize / 1000000, 0));

		if (!vOldISize)
			SetDlgItemText(IDC_DriveName7, _T("Drive 7"));
		else
		if (GetVolumeInformation(_T("I:\\"), volumeName, ARRAYSIZE(volumeName), &serialNumber, &maxComponentLen, &fileSystemFlags, fileSystemName, ARRAYSIZE(fileSystemName)))
		{
			SetDlgItemText(IDC_DriveName7, volumeName);
		}
	}

	//TempDriveSpaceFree = CheckDiskSpaceFree("J:\\");
	//if (vGlobalHardDriveTotalBytes > 1000000) //greater than a megabyte
	//{
	//	vHaveAUSBMemoryStick = true;
	//	TempNewDriveLetter = "J:\\";
	//}
	//if (vOldDFreeSpace != TempDriveSpaceFree)
	//{
	//	vOldDFreeSpace = TempDriveSpaceFree;
	//	SetDlgItemText(IDC_DriveFreeSpace1, dtoaWithCommas(vOldDFreeSpace, 0));
	//}
	//if (vOldDSize != vGlobalHardDriveTotalBytes)
	//{
	//	vOldDSize = vGlobalHardDriveTotalBytes;
	//	SetDlgItemText(IDC_DriveSize2, dtoaWithCommas(vOldDSize, 0));
	//}

	if (vOldNumberOfPossibleMemorySticks != vNumberOfPossibleMemorySticks)
	{
		if ((vOldHaveAUSBMemoryStick != vHaveAUSBMemoryStick) || (vMemoryStickDriveLetter != TempNewDriveLetter))
		{
			vOldHaveAUSBMemoryStick = vHaveAUSBMemoryStick;
			vMemoryStickDriveLetter = TempNewDriveLetter;
			if (vOldHaveAUSBMemoryStick)
				SetDlgItemText(IDC_MemoryStickDriveLabel, "Memory Stick on Drive Letter " + vMemoryStickDriveLetter);
			else
				SetDlgItemText(IDC_MemoryStickDriveLabel, _T("No USB Memory Stick Detected")); 
		}

		vOldNumberOfPossibleMemorySticks = vNumberOfPossibleMemorySticks;
		if (vOldNumberOfPossibleMemorySticks > 1)
			m_Function4Button.ShowWindow(SW_SHOW);
		else
		{
			m_Function4Button.ShowWindow(SW_HIDE);
			this->InvalidateRect(&m_Function4Button.GetRectToInvalidateArrow());
		}
	}
}

//void CUSBMemoryStickDialog::UpdateButtons()
//{
//}

void CUSBMemoryStickDialog::CopyFileToUSBStick(CString TempSourceFolder, CString TempFileName, CString TempDestinationFolder) 
{ //source should have drive letter and \\ at end.  Destination just has folder name without \\ at end
	if ((vNumberOfPossibleMemorySticks) && (vMemoryStickDriveLetter.GetLength()))
	{
		if (TempSourceFolder.Right(1) != "\\")
			TempSourceFolder = TempSourceFolder + "\\";

		if (TempDestinationFolder.Right(1) != "\\")
			TempDestinationFolder = TempDestinationFolder + "\\";

		CString TempDestinationFileName = vMemoryStickDriveLetter + TempDestinationFolder + TempFileName;
		CString TempDestinationFolderName = vMemoryStickDriveLetter + TempDestinationFolder;
		TempDestinationFolderName.Delete(TempDestinationFolderName.GetLength() - 1, 2);
		CString TempSourceFileName = TempSourceFolder + TempFileName;
		CFile TempFile;
		CFileStatus TempFileStatus;
		if (CFile::GetStatus(TempSourceFileName, TempFileStatus))
		{
			SetFileAttributes(TempSourceFileName,FILE_ATTRIBUTE_NORMAL);

			bool TempDestinationDirectoryOK = true;
			if (!CFile::GetStatus(TempDestinationFolderName, TempFileStatus))
			if (!CreateDirectory(TempDestinationFolderName,NULL))
			{
				TempDestinationDirectoryOK = false;
				CNoticeDialog TempNoticeDialog1;
				CString TempText("\n\nError, Could not create directory:\n");
				TempText = TempText + TempDestinationFolderName;
				TempNoticeDialog1.vNoticeText = TempText;
				TempNoticeDialog1.vType = cErrorMessage;
				TempNoticeDialog1.DoModal();
			}

			LPTSTR TempSourceName = TempSourceFileName.GetBuffer(TempSourceFileName.GetLength());
			LPTSTR TempDestinationName = TempDestinationFileName.GetBuffer(TempDestinationFileName.GetLength());

			if (TempDestinationDirectoryOK)
			if (!CopyFile(TempSourceName, TempDestinationName, false))
			{
				DWORD ErrorNow = GetLastError();		// system error codes http://msdn.microsoft.com/en-us/library/ms681381(v=vs.85)
				//ReportErrorMessage("Copy File Error: " + dtoa(ErrorNow) +" File: " + TempFileName + " to " + TempDestinationName, cEMailInspx, 32000);
				CNoticeDialog TempNoticeDialog;
				if ((ErrorNow == 3) || (ErrorNow == 112))
					TempNoticeDialog.vNoticeText = "\nUSB Copy File Error: "+ dtoa(ErrorNow) +"-Drive Full.\nFile: " + TempFileName + "\nto: " + vMemoryStickDriveLetter + TempDestinationFolder;
				else
					TempNoticeDialog.vNoticeText = "\nUSB Copy File Error: "+ dtoa(ErrorNow) +"\nFile: " + TempFileName + "\nto: " + vMemoryStickDriveLetter + TempDestinationFolder;
				TempNoticeDialog.vType = cErrorMessage;
				TempNoticeDialog.DoModal();
			}
			//else
			//{
			//	CNoticeDialog TempNoticeDialog;
			//	TempNoticeDialog.vNoticeText = "\n\nCopy Successful\nFile: " + TempFileName + "\nto " + TempDestinationFolderName;
			//	TempNoticeDialog.vType = cInformationMessage;
			//	TempNoticeDialog.DoModal();
			//}
		}
		else
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\nFile: " +TempFileName + "\n\nNot Found";
			TempNoticeDialog.vType = cErrorMessage;
			TempNoticeDialog.DoModal();
		}
	}
	else
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\nNo USB Memory Stick to copy to";
		TempNoticeDialog.vType = cErrorMessage;
		TempNoticeDialog.DoModal();
	}
}

void CUSBMemoryStickDialog::CopyFolderToUSBStick(CString TempSourceDirectory, CString TempDestinationDirectory)
{
	if ((vNumberOfPossibleMemorySticks) && (vMemoryStickDriveLetter.GetLength()))
	{
		CString TempDestinationFolderName = vMemoryStickDriveLetter + TempDestinationDirectory;
		CFile TempFile;
		CFileStatus TempFileStatus;
		if (CFile::GetStatus(TempSourceDirectory, TempFileStatus))
		{
			//SetFileAttributes(TempSourceDirectory,FILE_ATTRIBUTE_NORMAL);

			bool TempDestinationDirectoryOK = true;
			if (!CFile::GetStatus(TempDestinationFolderName, TempFileStatus))
			if (!CreateDirectory(TempDestinationFolderName,NULL))
			{
				TempDestinationDirectoryOK = false;
				vGlobalCopyDone = 0x54;  //done with this copy, it failed
				CNoticeDialog TempNoticeDialog1;
				CString TempText("\n\nError, Could not create directory:\n");
				TempText = TempText + TempDestinationFolderName;
				TempNoticeDialog1.vNoticeText = TempText;
				TempNoticeDialog1.vType = cErrorMessage;
				TempNoticeDialog1.DoModal();
			}

			if (TempDestinationDirectoryOK)
			{
				if (TempSourceDirectory.Right(1) == "\\")
					TempSourceDirectory.Delete(TempSourceDirectory.GetLength() - 1, 2);
				if (TempDestinationFolderName.Right(1) == "\\")
					TempDestinationFolderName.Delete(TempDestinationFolderName.GetLength() - 1, 2);
				//if (TempDestinationFolderName.Right(1) != "\\")
				//	TempDestinationFolderName = TempDestinationFolderName + "\\";
				vGlobalCopyDone = 0xAA;
				m_PleaseWait.ShowWindow(SW_SHOW);
				SetDlgItemText(IDC_PleaseWait, _T("Copying Files.  Please Wait"));

				if (vMainWindowPointer)
					vMainWindowPointer->CopyFilesInDirectoryToDirectory(TempSourceDirectory, TempDestinationFolderName);
			}
		}
		else
		{
			vGlobalCopyDone = 0x54;  //done with this copy, it failed
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\nFolder: " +TempSourceDirectory + "\n\nNot Found";
			TempNoticeDialog.vType = cErrorMessage;
			TempNoticeDialog.DoModal();
		}
	}
	else
	{
		vGlobalCopyDone = 0x54; //done with this copy, it failed
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\nNo USB Memory Stick to copy to";
		TempNoticeDialog.vType = cErrorMessage;
		TempNoticeDialog.DoModal();
	}
}


void CUSBMemoryStickDialog::OnStnClickedDialogtitlestatictext1()
{
	if (vGlobalShiftKeyDown)
	{
		if (PasswordOK(cTemporaryInspxPassword,false))
		{
			LPCTSTR szFilter =  _T("All Files (*.*)|*.*||; //CSV Files (*.CSV)|*.CSV|Text Files (*.TXT)|*.TXT||");
			CString TempFileString = "";

			CFileDialog *TempFileDialog = new CFileDialog(true, NULL, TempFileString, OFN_HIDEREADONLY | OFN_ENABLESIZING | OFN_EXPLORER, szFilter, this);

			vGlobalInWindowsDialog = true;
			int nResponse = TempFileDialog->DoModal();
			vGlobalInWindowsDialog = false;
			//if (nResponse == IDOK)
			//{
			//	TempPath = TempFileDialog->GetPathName();
			//	TempReturn = TempFileDialog->GetFileName();
			//}
			delete TempFileDialog;
		}
	}
	else
	{
		BYTE TempHiddenKeys = 8;
		if (!PasswordOK(cTemporaryInspxPassword,false))
			TempHiddenKeys = 10;
		PostThreadMessage(vGlobalCallingThreadID, cDisplaySoftKeyMenu, TempHiddenKeys, TempHiddenKeys);
	}
}

void CUSBMemoryStickDialog::RestoreSetupsFromBackup()
{
	CString TempFolderNames[100];
	BYTE TempNumberOfBackupFoldersToPickFrom = 0;
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;

	CString TempSearchName = vMemoryStickDriveLetter + "*ScanTrac Backup";
	LPTSTR TempDirectoryName = TempSearchName.GetBuffer(TempSearchName.GetLength());

	hFind = FindFirstFile(TempDirectoryName, &FindFileData);

	if (hFind == INVALID_HANDLE_VALUE) 
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\n\nNo ScanTrac backups found on USB Memory Stick to restore";
		TempNoticeDialog.vType = cInformationMessage;
		TempNoticeDialog.DoModal();
	} 
	else 
	{
		CString TempFileNameString = FindFileData.cFileName;
		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			TempFolderNames[TempNumberOfBackupFoldersToPickFrom] = FindFileData.cFileName;
			TempNumberOfBackupFoldersToPickFrom++;
		}
		while (FindNextFile(hFind, &FindFileData))
		{
			CString TempFileNameString = FindFileData.cFileName;
			if ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				if (TempNumberOfBackupFoldersToPickFrom < 100)
				{
					TempFolderNames[TempNumberOfBackupFoldersToPickFrom] = FindFileData.cFileName;
					TempNumberOfBackupFoldersToPickFrom++;
				}
			}
		}
	}
	FindClose(hFind);
	if (TempNumberOfBackupFoldersToPickFrom)
	{
		int TempBackupFolderChoice = 0;
		if (TempNumberOfBackupFoldersToPickFrom == 1)
		{
			CYesNoDialog TempYesNoDialog;
			CString TempText =  "Do you want to restore ScanTrac program and\nsetup files from backup folder: " + TempFolderNames[0] + 
				"?\nThis will reboot computer to complete.\nAny changes to setups after backup was made\nwill be lost.";
			TempYesNoDialog.vNoticeText =TempText;
			TempYesNoDialog.vQuestionType = cConfirmAbandon;
			TempYesNoDialog.vYesButtonText = "Continue";
			TempYesNoDialog.vNoButtonText = "Abort";
			int TempResult = TempYesNoDialog.DoModal();
			//if user said OK
			if (TempResult != IDOK)
				TempBackupFolderChoice = -1;
		}
		else
		if (TempNumberOfBackupFoldersToPickFrom > 1)
		{
			CSelectItemDialog ISelectItemDialog;  
			//Set dialog box data titles and number value
			ISelectItemDialog.vTitleString = "Choose folder to restore ScanTrac configuration files and program from";
			ISelectItemDialog.m_DialogTitleStaticText2 = "";
			ISelectItemDialog.m_DialogTitleStaticText3 = "";
			ISelectItemDialog.vBackgroundColor = cGreen;
			ISelectItemDialog.vInAutoSetup = vLocalSystemData->vInAutoSetup;

			ISelectItemDialog.vNumberOfItems = TempNumberOfBackupFoldersToPickFrom;

			for (BYTE TempLoop = 0; TempLoop < TempNumberOfBackupFoldersToPickFrom; TempLoop++)
				ISelectItemDialog.vItems[TempLoop] = TempFolderNames[TempLoop];

			//ISelectItemDialog.m_DialogTitleStaticText3 = "Original Value: " + vMemoryStickDriveLetter;

			//Pass control to dialog box and display
			int nResponse = ISelectItemDialog.DoModal();
			//dialog box is now closed, if user pressed select do this
			//if user pressed cancel, do nothing
			if (nResponse == IDOK)
			{
				TempBackupFolderChoice = ISelectItemDialog.vItemNumberSelected - 1;
			}
			else
			if (nResponse == 10)
			{
				//Main Menu button pressed
				Sleep(1);
				CDialog::EndDialog(10);
			}
			else
				TempBackupFolderChoice = -1;
		}
		if (TempBackupFolderChoice >= 0)
		{
			//do restore from USB backup
			CopyFolderToRestoreBackup(TempFolderNames[TempBackupFolderChoice]);
		}
	}
	else
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\n\nNo ScanTrac backups found on USB Memory Stick to restore";
		TempNoticeDialog.vType = cInformationMessage;
		TempNoticeDialog.DoModal();
	}
}

void CUSBMemoryStickDialog::CopyFolderToRestoreBackup(CString TempBackupFolderName)
{
	if ((vNumberOfPossibleMemorySticks) && (vMemoryStickDriveLetter.GetLength()))
	{
		WORD TempFilesCopied = 0;
		CString TempSourceFolderName = vMemoryStickDriveLetter + TempBackupFolderName;
		CFile TempFile;
		CFileStatus TempFileStatus;
		if (CFile::GetStatus(TempSourceFolderName, TempFileStatus))
		{
			bool TempDestinationDirectoryOK = true;
			//SetFileAttributes(TempSourceDirectory,FILE_ATTRIBUTE_NORMAL);
			CString TempDestinationFolder = "C:\\ScanTracExtra\\ScanTrac_New_Release";
			if (!CFile::GetStatus(TempDestinationFolder, TempFileStatus))
			if (!CreateDirectory(TempDestinationFolder,NULL))
			{
				TempDestinationDirectoryOK = false;
				CNoticeDialog TempNoticeDialog1;
				CString TempText("\n\nError, Could not create directory:\n");
				TempText = TempText + TempDestinationFolder;
				TempNoticeDialog1.vNoticeText = TempText;
				TempNoticeDialog1.vType = cErrorMessage;
				TempNoticeDialog1.DoModal();
			}
			TempDestinationFolder = TempDestinationFolder + "\\";

			CString TempCheckFileName = TempSourceFolderName + "\\Products.STR";
			if (TempDestinationDirectoryOK)
			if (!CFile::GetStatus(TempCheckFileName, TempFileStatus))
			{
				CNoticeDialog TempNoticeDialog1;
				CString TempText("\n\nError, Could not find Products.STR\nin: ");
				TempText = TempText + TempSourceFolderName;
				TempNoticeDialog1.vNoticeText = TempText;
				TempNoticeDialog1.vType = cErrorMessage;
				TempNoticeDialog1.DoModal();
			}
			else
			{
				//restore from backup on USB Stick
				WIN32_FIND_DATA FindFileData;
				HANDLE hFind;

				CString TempCopyFileName = TempSourceFolderName + "\\*.*";
				LPTSTR TempDirectoryName = TempCopyFileName.GetBuffer(TempCopyFileName.GetLength());

				hFind = FindFirstFile(TempDirectoryName, &FindFileData);

				if (hFind == INVALID_HANDLE_VALUE) 
				{
					CNoticeDialog TempNoticeDialog;
					CString TempString = "\n\nNo files to copy in\n";
					TempString = TempString + TempDirectoryName;
					TempNoticeDialog.vNoticeText = TempString;
					TempNoticeDialog.vType = cInformationMessage;
					TempNoticeDialog.DoModal();
				} 
				else 
				{
					CString TempFileNameString = FindFileData.cFileName;
					if (!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
					{
						CString TempFileNameText = FindFileData.cFileName;
						m_PleaseWait.ShowWindow(SW_SHOW);
						SetDlgItemText(IDC_PleaseWait, "Copying: " + TempFileNameText);

						CString TempSourceNameString = TempSourceFolderName + "\\" + FindFileData.cFileName;
						CString TempDestinationNameString = TempDestinationFolder + FindFileData.cFileName;
						LPTSTR TempSourceName = TempSourceNameString.GetBuffer(TempSourceNameString.GetLength());
						LPTSTR TempDestinationName = TempDestinationNameString.GetBuffer(TempDestinationNameString.GetLength());

						if (!CopyFile(TempSourceName, TempDestinationName, false))
						{
							DWORD ErrorNow = GetLastError();		// system error codes http://msdn.microsoft.com/en-us/library/ms681381(v=vs.85)
							CNoticeDialog TempNoticeDialog;
							TempNoticeDialog.vNoticeText = "\nUSB Copy File Error: "+ dtoa(ErrorNow) +"\nFile: " + TempFileNameText + "\nto: " + TempDestinationFolder;
							TempNoticeDialog.vType = cErrorMessage;
							TempNoticeDialog.DoModal();
						}
						else
							TempFilesCopied++;
					}
					while (FindNextFile(hFind, &FindFileData))
					{
						CString TempFileNameString = FindFileData.cFileName;
						if (!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
						{
							CString TempFileNameText = FindFileData.cFileName;
							m_PleaseWait.ShowWindow(SW_SHOW);
							SetDlgItemText(IDC_PleaseWait, "Copying: " + TempFileNameText);

							CString TempSourceNameString = TempSourceFolderName + "\\" + FindFileData.cFileName;
							CString TempDestinationNameString = TempDestinationFolder + FindFileData.cFileName;
							LPTSTR TempSourceName = TempSourceNameString.GetBuffer(TempSourceNameString.GetLength());
							LPTSTR TempDestinationName = TempDestinationNameString.GetBuffer(TempDestinationNameString.GetLength());

							if (!CopyFile(TempSourceName, TempDestinationName, false))
							{
								DWORD ErrorNow = GetLastError();		// system error codes http://msdn.microsoft.com/en-us/library/ms681381(v=vs.85)
								CNoticeDialog TempNoticeDialog;
								TempNoticeDialog.vNoticeText = "\nUSB Copy File Error: "+ dtoa(ErrorNow) +"\nFile: " + TempFileNameText + "\nto: " + TempDestinationFolder;
								TempNoticeDialog.vType = cErrorMessage;
								TempNoticeDialog.DoModal();
							}
							else
								TempFilesCopied++;
						}
					}
				}
				FindClose(hFind);
				m_PleaseWait.ShowWindow(SW_SHOW);
				if (TempFilesCopied)
				{
					SetDlgItemText(IDC_PleaseWait, dtoa(TempFilesCopied, 0) + " Files Copied, Preparing ScanTrac Software/Setup Restore");
					bool TempAllOK = true;

					//copy files to update folder then reboot
					BOOL ZipFileExists = PathFileExists(_T("C:\\ScanTracExtra\\ScanTrac_New_Release\\ScanTrac.exe"));
					BOOL UnZipExeFileExists = PathFileExists(_T("C:\\ScanTracExtra\\ScanTrac_New_Release\\Products.STR"));

					if ((ZipFileExists == TRUE) && (UnZipExeFileExists == TRUE))
					{
						try
						{
							HANDLE hBatchFileConfigInfo = CreateFile(_T("C:\\ScanTracExtra\\ScanTrac_New_Release\\Restore.bat"), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ |FILE_SHARE_WRITE, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);

							if (hBatchFileConfigInfo != INVALID_HANDLE_VALUE) // returns invalid handle if file exists, don't need ((ErrorMsg = GetLastError()) != ERROR_FILE_EXISTS))
							{	// if file does not exist, write it
								CString BufferToWrite = "echo OFF";
								BufferToWrite += _T("\n cls");
								BufferToWrite += _T("\n echo Please Wait, Copying Files...");
								BufferToWrite += _T("\n PING 127.0.0.1 -n 10 >nul");
								BufferToWrite += _T("\n del /q C:\\ScanTracExtra\\ScanTracPreRestoreBackup\\*.*");
								BufferToWrite += _T("\n xcopy C:\\ScanTrac\\*.* C:\\ScanTracExtra\\ScanTracPreRestoreBackup\\*.*");
								BufferToWrite += _T("\n del /q C:\\ScanTrac\\*.*");
								BufferToWrite += _T("\n xcopy C:\\ScanTracExtra\\ScanTrac_New_Release\\*.* C:\\ScanTrac\\*.*");
								//BufferToWrite += _T("\n del /q C:\\ScanTracExtra\\ScanTrac_New_Release\\*.*");
								BufferToWrite += _T("\n shutdown.exe /r /t 5");

								DWORD BytesWritten = 0; 
			
								USES_CONVERSION;
								LPSTR TempStringToWrite = W2A(BufferToWrite);

								WriteFile(hBatchFileConfigInfo, TempStringToWrite, BufferToWrite.GetLength(), &BytesWritten, NULL);
							}
							CloseHandle(hBatchFileConfigInfo);

							//NOTE FILE NAME IN OVERLAYDETAIL.BAT MUST MATCH file name in CString FileNameToDisplay = "FBWFOverlayDetail.txt"
							//NOTE DIRECTORY NAME IN OVERLAYDETAIL.BAT MUST MATCH directory name in cLogDirectory c:\ScanTracLogs
							// OverlayDetail batch file is -- fbwfmgr /overlaydetail >C:\ScanTracLogs\FBWFOverlayDetail.txt
							intptr_t TempSpawnOk = _spawnl(_P_NOWAIT, "C:\\ScanTracExtra\\ScanTrac_New_Release\\Restore.bat", "C:\\ScanTracExtra\\ScanTrac_New_Release\\Restore.bat", NULL);

							if (TempSpawnOk != NULL)
							{
								vMainWindowPointer->ExitProgram();
								CDialog::EndDialog(10);
							}
							else
							{
								CNoticeDialog TempNoticeDialog;
								TempNoticeDialog.vNoticeText = "\n\nCould not restore ScanTrac program and setup files from USB Backup";
								TempNoticeDialog.vType = cErrorMessage;
								TempNoticeDialog.DoModal();
								SetDlgItemText(IDC_PleaseWait, _T("Could not restore ScanTrac program and setup files from USB Backup"));
							}
						}
						catch ( char *strException)
						{
							CNoticeDialog TempNoticeDialog;
							CString TempAString;
							CString TempNoticeString;
							TempAString.Format(_T("\nException raised: %s"), strException);
							TempNoticeString = "\nUnzip file caused an Exception error." + TempAString + "\nCall Service";
							TempNoticeDialog.vNoticeText = TempNoticeString;
							TempNoticeDialog.vType = cErrorMessage;
							TempNoticeDialog.DoModal();
						}

						}
						else
						{
							CNoticeDialog TempNoticeDialog;
							TempNoticeDialog.vNoticeText = "\n\nCould not restore ScanTrac program and setup files from USB Backup";
							TempNoticeDialog.vType = cErrorMessage;
							TempNoticeDialog.DoModal();
							SetDlgItemText(IDC_PleaseWait, _T("Could not restore ScanTrac program and setup files from USB Backup"));
						}
				}
				else
					SetDlgItemText(IDC_PleaseWait, _T("No Files Copied, canTrac Software/Setup Restore Failed"));
			}
		}
		else
		{
			CNoticeDialog TempNoticeDialog;
			TempNoticeDialog.vNoticeText = "\n\nFolder: " +TempSourceFolderName + "\n\nNot Found";
			TempNoticeDialog.vType = cErrorMessage;
			TempNoticeDialog.DoModal();
		}
	}
	else
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\nNo USB Memory Stick to copy to";
		TempNoticeDialog.vType = cErrorMessage;
		TempNoticeDialog.DoModal();
		vGlobalCopyDone = 0x54; //done with this copy, it failed
	}
}

void CUSBMemoryStickDialog::CopyFilesToUSBStick(CString TempFolder, bool TempYesterday)
{
	//Copy Today's Screen Shots
	CString TempSourceDirectory = TempFolder;
	CString TempDestinationFolder = TempFolder;
	TempDestinationFolder.Delete(0, 3); //remove the c:
	TempDestinationFolder.Delete(TempDestinationFolder.GetLength() - 1, 2);
	TempSourceDirectory.Delete(TempSourceDirectory.GetLength() - 1, 2);

	CTime TempTime = CTime::GetCurrentTime();
	if (TempYesterday)
	{
		CTimeSpan TempTimeForOneDay = 86400;
		TempTime = TempTime - TempTimeForOneDay; //subtract one day
	}

	CString TempTodaysDateString = TempTime.Format("%Y-%m-%d");
	CString TempSourceFindDirectory = TempFolder + "*" + TempTodaysDateString + "*.*";

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;

	LPTSTR TempDirectoryName = TempSourceFindDirectory.GetBuffer(TempSourceFindDirectory.GetLength());

	hFind = FindFirstFile(TempDirectoryName, &FindFileData);

	if (hFind == INVALID_HANDLE_VALUE)
	{
		CNoticeDialog TempNoticeDialog;
		TempNoticeDialog.vNoticeText = "\n\n\nNo files to copy in\n" + TempSourceFindDirectory;
		TempNoticeDialog.vType = cInformationMessage;
		TempNoticeDialog.DoModal();
	}
	else
	{
		CString TempFileNameString = FindFileData.cFileName;
		if (!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			CString TempFileNameText = FindFileData.cFileName;
			m_PleaseWait.ShowWindow(SW_SHOW);
			SetDlgItemText(IDC_PleaseWait, "Copying: " + TempFileNameText);
			CopyFileToUSBStick(TempSourceDirectory, TempFileNameText, TempDestinationFolder);
		}
		while (FindNextFile(hFind, &FindFileData))
		{
			CString TempFileNameString = FindFileData.cFileName;
			if (!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				CString TempFileNameText = FindFileData.cFileName;
				m_PleaseWait.ShowWindow(SW_SHOW);
				SetDlgItemText(IDC_PleaseWait, "Copying: " + TempFileNameText);
				CopyFileToUSBStick(TempSourceDirectory, TempFileNameText, TempDestinationFolder);
				Sleep(10);
			}
		}
	}
	FindClose(hFind);
	Sleep(1000);
	m_PleaseWait.ShowWindow(SW_HIDE);
}

void CUSBMemoryStickDialog::CopySavedImagesToUSBStick(CString TempFolder, bool TempYesterday)
{
	CTime TempTime = CTime::GetCurrentTime();
	if (TempYesterday)
	{
		CTimeSpan TempTimeForOneDay = 86400;
		TempTime = TempTime - TempTimeForOneDay; //subtract one day
	}

	CString TempString = TempTime.Format("%Y-%m-%d");
	CString TempSourceDirectory = TempFolder + TempString;
	CString TempDestinationFolder = TempFolder + TempString;
	TempDestinationFolder.Delete(0, 3);

	//make sure USB Stick has ScanTracRejectedImages folder
	CString TempPrimaryFolderName = TempFolder;
	TempPrimaryFolderName.Delete(0, 3);
	TempPrimaryFolderName.Delete(TempPrimaryFolderName.GetLength() - 1, 2);
	TempPrimaryFolderName = vMemoryStickDriveLetter + TempPrimaryFolderName;
	CFile TempFile;
	CFileStatus TempFileStatus;
	bool TempDestinationDirectoryOK = true;
	if (!CFile::GetStatus(TempPrimaryFolderName, TempFileStatus))
	if (!CreateDirectory(TempPrimaryFolderName,NULL))
	{
		TempDestinationDirectoryOK = false;
		CNoticeDialog TempNoticeDialog1;
		CString TempText("\n\nError, Could not create directory:\n");
		TempText = TempText + TempPrimaryFolderName;
		TempNoticeDialog1.vNoticeText = TempText;
		TempNoticeDialog1.vType = cErrorMessage;
		TempNoticeDialog1.DoModal();
	}

	if (TempDestinationDirectoryOK)
	{
		vGlobalCopyFileCount = 0;  //setup variables to give message when done
		vGlobalCopyDone = 0xAA;
		vLastSourceDirectory = TempSourceDirectory;
		vLastDestinationDirectory = vMemoryStickDriveLetter + TempDestinationFolder;

		CopyFolderToUSBStick(TempSourceDirectory, TempDestinationFolder);
	}
}

