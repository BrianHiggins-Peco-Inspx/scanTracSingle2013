//ScanTrac Side View Source File
#if !defined(AFX_BACKUPHARDDRIVETHREAD_H__42AFD225_8631_4434_B7FE_2506D1DD7545__INCLUDED_)
#define AFX_BACKUPHARDDRIVETHREAD_H__42AFD225_8631_4434_B7FE_2506D1DD7545__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BackupHardDriveThread.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CBackupHardDriveThread thread

class CBackupHardDriveThread : public CWinThread
{
	DECLARE_DYNCREATE(CBackupHardDriveThread)
protected:
	CBackupHardDriveThread();           // protected constructor used by dynamic creation

// Attributes
public:
	//variables
	CScanTracSystemRunningData *vLocalSystemData;
	CSystemConfigurationData *vLocalConfigurationData;
	CScanTracDlg *vMainWindowPointer;
	BOOLEAN vEndThreadNow;
	DWORD vCount;
	DWORD vFilesDeleted;
	DWORD vFilesCopied;
	DWORD vFoldersChecked;
	DWORD vErrors;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBackupHardDriveThread)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual int Run();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CBackupHardDriveThread();

	//Methods
	void DeleteFilesInDirectoryIfNotSameAsPrimary(CString TempDirectoryString);
	void CopyFolderToBackupHardDrive(CString TempFolderName);
	void CopyFilesInFolderToBackupDrive(CString TempPrimaryFolderName, CString TempBackupFolderName);



	// Generated message map functions
	//{{AFX_MSG(CBackupHardDriveThread)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BACKUPHARDDRIVETHREAD_H__42AFD225_8631_4434_B7FE_2506D1DD7545__INCLUDED_)
