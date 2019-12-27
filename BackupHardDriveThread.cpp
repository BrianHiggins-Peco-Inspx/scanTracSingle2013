//ScanTrac Side View Source File
// BackupHardDriveThread.cpp : implementation file
//

#include "stdafx.h"
#include "scantrac.h"
#include "BackupHardDriveThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBackupHardDriveThread

IMPLEMENT_DYNCREATE(CBackupHardDriveThread, CWinThread)

CBackupHardDriveThread::CBackupHardDriveThread()
{
	vLocalSystemData = NULL;
	vLocalConfigurationData = NULL;
	vMainWindowPointer = NULL;
	vEndThreadNow = false;
	vCount = 0;
	vErrors = 0;
	vFilesDeleted = 0;
	vFilesCopied = 0;
	vFoldersChecked = 0;
}

CBackupHardDriveThread::~CBackupHardDriveThread()
{
}

BOOL CBackupHardDriveThread::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return true;
}

int CBackupHardDriveThread::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CBackupHardDriveThread, CWinThread)
	//{{AFX_MSG_MAP(CBackupHardDriveThread)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBackupHardDriveThread message handlers

int CBackupHardDriveThread::Run() 
{
	// TODO: Add your specialized code here and/or call the base class
	//backup setup files and registry to backup hard drive
	//BackupRegistryToBackupHardDrive();

	ReportErrorMessage("Start Backup ScanTrac Files to Backup Hard Disk", cWriteToLog,0);

	CFile TempFile;
	CFileStatus TempFileStatus;
	CString TempFolderName = "C:\\ScanTrac*";
	
	DWORD TempCount = 0;
	{
		WIN32_FIND_DATA FindFileData;
		HANDLE hFind;
		hFind = FindFirstFile(TempFolderName, &FindFileData);

		if (hFind != INVALID_HANDLE_VALUE) 
		{
			TempFolderName = FindFileData.cFileName;
			if (TempFolderName.GetAt(TempFolderName.GetLength() - 1) != '.')
			{
				if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					CopyFolderToBackupHardDrive(TempFolderName);
			}
			while ((FindNextFile(hFind,&FindFileData)) && (!vEndThreadNow))
			{
				TempFolderName = FindFileData.cFileName;
				if (TempFolderName.GetAt(TempFolderName.GetLength() - 1) != '.')
				{
					if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
						CopyFolderToBackupHardDrive(TempFolderName);
				}
				
				if ((vLocalSystemData) && (vLocalSystemData->vSystemRunMode == cStoppedSystemMode))
				{
					TempCount++;
					if (!(TempCount % 20))
						Sleep(2);
				}
				else
					Sleep(10);
					
			}
			FindClose(hFind);
		}
	}
	if (vEndThreadNow)
		ReportErrorMessage("Aborted Backup ScanTrac Files to Backup Hard Disk", cWriteToLog,0);
	else
		ReportErrorMessage("Completed Backup ScanTrac Files to Backup Hard Disk", cWriteToLog,0);

	ReportErrorMessage("Folders Checked: " + dtoa(vFoldersChecked,0) + "     File Errors: " + dtoa(vErrors,0), cWriteToLog,0);
	ReportErrorMessage("Files Deleted: " + dtoa(vFilesDeleted,0) + "     Files Copied: " + dtoa(vFilesCopied,0), cWriteToLog,0);

	vMainWindowPointer->EndCopyAllScanTracFoldersToBackupHardDrive();

	return CWinThread::Run();
}

void CBackupHardDriveThread::DeleteFilesInDirectoryIfNotSameAsPrimary(CString TempDirectoryString) 
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;

	vFoldersChecked++;
	CString TempLastChar = TempDirectoryString.GetAt(TempDirectoryString.GetLength() - 1);
	while (TempLastChar == "\\")
	{
		TempDirectoryString.Delete(TempDirectoryString.GetLength() - 1, 1);
		TempLastChar = TempDirectoryString.GetAt(TempDirectoryString.GetLength() - 1);
	}
	TempDirectoryString = TempDirectoryString + "\\";

	//delete any files in backup directory
	CString TempAnyFileString(TempDirectoryString);
	TempAnyFileString = TempAnyFileString + "*.*";
	LPTSTR TempDirectoryName = TempAnyFileString.GetBuffer(TempAnyFileString.GetLength());

	hFind = FindFirstFile(TempDirectoryName, &FindFileData);

	if (hFind == INVALID_HANDLE_VALUE) 
	{
		//CNoticeDialog TempNoticeDialog;
		//TempNoticeDialog.vNoticeText = "\n\n\nNo files in image directory to backup";
		//TempNoticeDialog.vType = cInformationMessage;
		//TempNoticeDialog.DoModal();
	} 
	else 
	{
		BYTE TempPosition = TempDirectoryString.Find(_T("*.*"),0);
		if (TempPosition < 255)
			TempDirectoryString.Delete(TempPosition,TempDirectoryString.GetLength() - TempPosition);

		CString TempStringOld = FindFileData.cFileName;
		TempStringOld = TempDirectoryString + TempStringOld;
		LPTSTR TempOld = TempStringOld.GetBuffer(TempStringOld.GetLength());
		if (TempStringOld.GetAt(TempStringOld.GetLength() - 1) != '.')
		{
			if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				DeleteFilesInDirectoryIfNotSameAsPrimary(TempOld);
				RemoveDirectory(TempOld);
			}
			else
			{
				CString TempPrimaryName = TempStringOld;
				TempPrimaryName.Delete(0,1);
				TempPrimaryName = "C" + TempPrimaryName;

				CFileStatus TempSourceFileStatus;
				int TempHaveSource = CFile::GetStatus(TempPrimaryName, TempSourceFileStatus);
				CFileStatus TempDestinationFileStatus;
				int TempHaveDestination = CFile::GetStatus(TempStringOld, TempDestinationFileStatus);
				
				if (((!TempHaveSource) && (TempHaveDestination)) || //if file not in Primary folder anymore, delete backup file
					(TempSourceFileStatus.m_size != TempDestinationFileStatus.m_size))  //if different size, delete backup file
				{
					SetFileAttributes(TempOld,FILE_ATTRIBUTE_NORMAL);
					vFilesDeleted++;
					if (!DeleteFile(TempOld))
					{
						vErrors++;
						DWORD TempErrorReason = GetLastError();
						ReportErrorMessage("Failed to Delete File: " + TempStringOld, cEMailInspx, 32000);
					}
				}
			}
		}
		DWORD TempCount = 0;
		while ((FindNextFile(hFind,&FindFileData)) && (!vEndThreadNow))
		{
			TempStringOld = FindFileData.cFileName;
			TempStringOld = TempDirectoryString + TempStringOld;
			TempOld = TempStringOld.GetBuffer(TempStringOld.GetLength());
			if (TempStringOld.GetAt(TempStringOld.GetLength() - 1) != '.')
			{
				if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					DeleteFilesInDirectoryIfNotSameAsPrimary(TempOld);
					RemoveDirectory(TempOld);
				}
				else
				{
					CString TempPrimaryName = TempStringOld;
					TempPrimaryName.Delete(0,1);
					TempPrimaryName = "C" + TempPrimaryName;

					CFileStatus TempSourceFileStatus;
					int TempHaveSource = CFile::GetStatus(TempPrimaryName, TempSourceFileStatus);
					CFileStatus TempDestinationFileStatus;
					int TempHaveDestination = CFile::GetStatus(TempStringOld, TempDestinationFileStatus);
					
					bool TempModified = false;
					if ((TempHaveSource) && (TempHaveDestination))
					{
						CTimeSpan TempDeltaTime;
						TempDeltaTime = TempSourceFileStatus.m_mtime - TempDestinationFileStatus.m_mtime; //if file time different, delete backup file
						ULONGLONG TempSeconds = TempDeltaTime.GetTotalSeconds();
						if (TempSeconds > 30)
							TempModified = true;
					}
					if (((!TempHaveSource) && (TempHaveDestination)) || //if file not in Primary folder anymore, delete backup file
						(TempModified) ||
						(TempSourceFileStatus.m_size != TempDestinationFileStatus.m_size))  //if different size, delete backup file
					{
						SetFileAttributes(TempOld,FILE_ATTRIBUTE_NORMAL);
						vFilesDeleted++;
						if (!DeleteFile(TempOld))
						{
							vErrors++;
							DWORD TempErrorReason = GetLastError();
							ReportErrorMessage("Failed to Delete File: " + TempStringOld, cEMailInspx, 32000);
						}
					}
				}
			}
			
			if ((vLocalSystemData) && (vLocalSystemData->vSystemRunMode == cStoppedSystemMode))
			{
				TempCount++;
				if (!(TempCount % 20))
					Sleep(2);
			}
			else
				Sleep(10);
				
		}
		FindClose(hFind);
	}
}

void CBackupHardDriveThread::CopyFolderToBackupHardDrive(CString TempFolderName)
{
	CString TempPrimaryFolderName = ":\\" + TempFolderName;
	TempPrimaryFolderName = vLocalSystemData->vPrimaryDrive + TempPrimaryFolderName;
	CString TempBackupFolderName = ":\\" + TempFolderName;
	TempBackupFolderName = vLocalSystemData->vBackupDrive + TempBackupFolderName;

	CFileStatus TempFileStatus;
	LPTSTR TempString = TempPrimaryFolderName.GetBuffer(TempPrimaryFolderName.GetLength());
	int TempSourceDirectoryExists = CFile::GetStatus(TempString, TempFileStatus);

	if ((TempSourceDirectoryExists) && (!vEndThreadNow))
	{
		TempString = TempBackupFolderName.GetBuffer(TempBackupFolderName.GetLength());
		int TempDestinationDirectoryExists = CFile::GetStatus(TempString, TempFileStatus);

		if (TempDestinationDirectoryExists)
		{
			DeleteFilesInDirectoryIfNotSameAsPrimary(TempBackupFolderName);
			RemoveDirectory(TempBackupFolderName);
		}

		if (vLocalSystemData->vLogFile.vLogSerialData)
			vLocalSystemData->vLogFile.WriteToLogFile("Backup Folder to Backup Hard Disk: " + TempPrimaryFolderName,cDebugMessage);

		CopyFilesInFolderToBackupDrive(TempPrimaryFolderName, TempBackupFolderName);
	}
}


void CBackupHardDriveThread::CopyFilesInFolderToBackupDrive(CString TempPrimaryFolderName, CString TempBackupFolderName)
{
	CreateDirectory(TempBackupFolderName,NULL);
	CFile TempFile;
	CFileStatus TempFileStatus;
	CString TempSourceFileName = TempPrimaryFolderName + "\\*.*";
	
	DWORD TempCount = 0;
	if (CFile::GetStatus(TempPrimaryFolderName, TempFileStatus))
	{
		WIN32_FIND_DATA FindFileData;
		HANDLE hFind;
		hFind = FindFirstFile(TempSourceFileName, &FindFileData);

		if (hFind != INVALID_HANDLE_VALUE) 
		{
			CString TempSourceFileOrFolderName = TempPrimaryFolderName + "\\" + FindFileData.cFileName;
			CString TempDestinationFileOrFolderName = TempBackupFolderName + "\\" + FindFileData.cFileName;
			if (TempSourceFileOrFolderName.GetAt(TempSourceFileOrFolderName.GetLength() - 1) != '.')
			{
				if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					//SetFileAttributes(TempSourceFileOrFolderName,FILE_ATTRIBUTE_NORMAL);
					CopyFilesInFolderToBackupDrive(TempSourceFileOrFolderName,TempDestinationFileOrFolderName);
				}
				else
				{
					CFileStatus TempSourceFileStatus;
					int TempHaveSource = CFile::GetStatus(TempSourceFileOrFolderName, TempSourceFileStatus);
					CFileStatus TempDestinationFileStatus;
					int TempHaveDestination = CFile::GetStatus(TempDestinationFileOrFolderName, TempDestinationFileStatus);
					
					if ((TempHaveSource) && (!TempHaveDestination))
					{
						vFilesCopied++;
						SetFileAttributes(TempSourceFileOrFolderName,FILE_ATTRIBUTE_NORMAL);
			
						if (!CopyFile(TempSourceFileOrFolderName,TempDestinationFileOrFolderName,false))
						{
							DWORD ErrorNow = GetLastError();		// system error codes http://msdn.microsoft.com/en-us/library/ms681381(v=vs.85)
							ReportErrorMessage("Copy File Error 7: "+ dtoa(ErrorNow) +" File: " + TempSourceFileOrFolderName + " to " + TempDestinationFileOrFolderName, cEMailInspx, 32000);
							vErrors++;
						}
						else
						if (vLocalSystemData->vLogFile.vLogSerialData)
							vLocalSystemData->vLogFile.WriteToLogFile("Backup File to Backup Hard Disk: " + TempSourceFileOrFolderName,cDebugMessage);
					}
				}
			}
			while ((FindNextFile(hFind,&FindFileData)) && (!vEndThreadNow))
			{
				TempSourceFileOrFolderName = TempPrimaryFolderName + "\\" + FindFileData.cFileName;
				TempDestinationFileOrFolderName = TempBackupFolderName + "\\" + FindFileData.cFileName;
				if (TempSourceFileOrFolderName.GetAt(TempSourceFileOrFolderName.GetLength() - 1) != '.')
				{
					if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					{
						//SetFileAttributes(TempSourceFileOrFolderName,FILE_ATTRIBUTE_NORMAL);
						CopyFilesInFolderToBackupDrive(TempSourceFileOrFolderName,TempDestinationFileOrFolderName);
					}
					else
					{
						//will overflow buffer..ReportErrorMessage("Backup File to Backup Hard Disk: " + TempSourceFileOrFolderName, cWriteToLog,0);
	
						CFileStatus TempSourceFileStatus;
						int TempHaveSource = CFile::GetStatus(TempSourceFileOrFolderName, TempSourceFileStatus);
						CFileStatus TempDestinationFileStatus;
						int TempHaveDestination = CFile::GetStatus(TempDestinationFileOrFolderName, TempDestinationFileStatus);
						
						if ((TempHaveSource) && (!TempHaveDestination))
						{
							vFilesCopied++;
							SetFileAttributes(TempSourceFileOrFolderName,FILE_ATTRIBUTE_NORMAL);

							//b rh test
							//double TempStartTime = CIFCOS::GetSystimeMicrosecs();

							bool TempCopyCanceled = false;

							if (!CopyFile(TempSourceFileOrFolderName,TempDestinationFileOrFolderName,false))
							{
								DWORD ErrorNow = GetLastError();		// system error codes http://msdn.microsoft.com/en-us/library/ms681381(v=vs.85)
								ReportErrorMessage("Copy File Error 8: "+ dtoa(ErrorNow) +" File: " + TempSourceFileOrFolderName + " to " + TempDestinationFileOrFolderName, cEMailInspx, 32000);
								vErrors++;
							}
							else
							if (vLocalSystemData->vLogFile.vLogSerialData)
								vLocalSystemData->vLogFile.WriteToLogFile("Backup File to Backup Hard Disk: " + TempSourceFileOrFolderName,cDebugMessage);
						
							//b rh test
							//double TempEndTime = CIFCOS::GetSystimeMicrosecs();
							//double TempCurrentFileCopyTime = TempEndTime - TempStartTime;

							//ReportErrorMessage("Copy " + TempSourceFileOrFolderName + " in: " + dtoa(TempCurrentFileCopyTime / 1000,0) + 
							//	"mSec, Size: " + dtoa(TempSourceFileStatus.m_size,0), cWriteToLog,0);
						}
					}
				}
				
				if ((vLocalSystemData) && (vLocalSystemData->vSystemRunMode == cStoppedSystemMode))
				{
					TempCount++;
					if (!(TempCount % 20))
						Sleep(2);
				}
				else
					Sleep(10);
					
			}
			FindClose(hFind);
		}
	}
}

