#pragma once
#include "afxwin.h"


// CUSBMemoryStickDialog dialog

class CUSBMemoryStickDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CUSBMemoryStickDialog)

public:

	//variables
	CString vLastSourceDirectory;
	CString vLastDestinationDirectory;

	BYTE vNumberOfPossibleMemorySticks;
	BYTE vOldNumberOfPossibleMemorySticks;
	BYTE vCopyProgress;

	double vOldCSize;
	double vOldDSize;
	double vOldESize;
	double vOldFSize;
	double vOldGSize;
	double vOldHSize;
	double vOldISize;

	double vOldCFreeSpace;
	double vOldDFreeSpace;
	double vOldEFreeSpace;
	double vOldFFreeSpace;
	double vOldGFreeSpace;
	double vOldHFreeSpace;
	double vOldIFreeSpace;

	CString vOldDriveNameC;
	CString vOldDriveNameD;
	CString vOldDriveNameE;
	CString vOldDriveNameF;
	CString vOldDriveNameG;
	CString vOldDriveNameH;
	CString vOldDriveNameI;

	CString vMemoryStickDriveLetter;
	bool vHaveAUSBMemoryStick;
	bool vOldHaveAUSBMemoryStick;
	bool vOldXRayOnNotice;
	bool vChangeMade;
	unsigned long int vUpdateDisplayTimerHandle;
	CScanTracDlg *vMainWindowPointer;
	tProductCollection *vLocalProductCollection;
	CScanTracSystemRunningData *vLocalSystemData;
	CSystemConfigurationData *vLocalConfigurationData;
	tCWndCollection vLocalCWndCollection;
	bool vSubFunction1ButtonEnable;
	bool vSubFunction2ButtonEnable;
	bool vSubFunction3ButtonEnable;
	bool vSubFunction4ButtonEnable;
	bool vNeedToBackupSetup;
	bool vNeedToCopyAllScreenShots;

	CUSBMemoryStickDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CUSBMemoryStickDialog();

	//void UpdateButtons();
	void UpdateDisplay();
	void CopyFileToUSBStick(CString TempSourceFolder, CString TempFileName, CString TempDestinationFolder);
	void CopyFolderToUSBStick(CString TempSourceDirectory, CString TempDestinationDirectory);
	void RestoreSetupsFromBackup();
	void CopyFolderToRestoreBackup(CString TempBackupFolderName);
	void CopySavedImagesToUSBStick(CString TempFolder, bool TempYesterday);
	void CopyFilesToUSBStick(CString TempFolder, bool TempYesterday);

// Dialog Data
	enum { IDD = IDD_USBMemoryStickDialog };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnStnClickedFunction1button();
	afx_msg void OnStnClickedFunction2button();
	afx_msg void OnStnClickedFunction3button();
	afx_msg void OnStnClickedFunction4button();
	afx_msg void OnStnClickedFunction5button();
	afx_msg void OnStnClickedSubfunction1button();
	afx_msg void OnStnClickedSubfunction2button();
	afx_msg void OnStnClickedSubfunction3button();
	afx_msg void OnStnClickedSubfunction4button();
	afx_msg void OnStnClickedSubfunction5button();
	afx_msg void OnStnClickedSubfunction6button();
	afx_msg void OnStnClickedSubfunction7button();
	afx_msg void OnStnClickedSubfunction8button();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CStatic m_AllFilesLabel;
	CStatic m_Background;
	CStatic m_DialogTitleStaticText1;
	CStatic m_DriveFreeSpace1;
	CStatic m_DriveFreeSpace2;
	CStatic m_DriveFreeSpace3;
	CStatic m_DriveFreeSpace4;
	CStatic m_DriveFreeSpace5;
	CStatic m_DriveFreeSpace6;
	CStatic m_DriveFreeSpace7;
	CStatic m_DriveFreeSpaceLabel;
	CStatic m_DriveLetter1;
	CStatic m_DriveLetter2;
	CStatic m_DriveLetter3;
	CStatic m_DriveLetter4;
	CStatic m_DriveLetter5;
	CStatic m_DriveLetter6;
	CStatic m_DriveLetter7;
	CStatic m_DriveLetterLabel;
	CStatic m_DriveName1;
	CStatic m_DriveName2;
	CStatic m_DriveName3;
	CStatic m_DriveName4;
	CStatic m_DriveName5;
	CStatic m_DriveName6;
	CStatic m_DriveName7;
	CStatic m_DriveNameLabel;
	CStatic m_DriveSize1;
	CStatic m_DriveSize2;
	CStatic m_DriveSize3;
	CStatic m_DriveSize4;
	CStatic m_DriveSize5;
	CStatic m_DriveSize6;
	CStatic m_DriveSize7;
	CStatic m_DriveSizeLabel;
	CStatic m_MemoryStickDriveLabel;
	CDownFunctionButton m_Function1Button;
	CDownFunctionButton m_Function2Button;
	CDownFunctionButton m_Function3Button;
	CDownFunctionButton m_Function4Button;
	CDownFunctionButton m_Function5Button;
	CLeftFunctionButton m_SubFunction1Button;
	CLeftFunctionButton m_SubFunction2Button;
	CLeftFunctionButton m_SubFunction3Button;
	CLeftFunctionButton m_SubFunction4Button;
	CRightFunctionButton m_SubFunction5Button;
	CRightFunctionButton m_SubFunction6Button;
	CRightFunctionButton m_SubFunction7Button;
	CRightFunctionButton m_SubFunction8Button;
	CStatic m_TodaysFilesLabel;
	CStatic m_XRaysOn;
	CStatic m_PleaseWait;
	afx_msg void OnStnClickedDialogtitlestatictext1();
};
