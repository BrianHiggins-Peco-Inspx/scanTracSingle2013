// UsersDialog.h : Declaration of the CUsersDialog

#pragma once

#include "resource.h"       // main symbols
#include "SetupUserDialog.h"

// CUsersDialog

class CUsersDialog : public CDialog
{
public:
	//Variables
	bool vChangeMade;
	WORD vCurrentNumberOfUsers;
	WORD vCurrentMaxNumberOfUsers;
	WORD vNumberOfLevelList [cSuperInspxPassword];
	UserPasswords* vUserList; 
	tUserCollection vLocalUserCollection;
	CScanTracSystemRunningData *vLocalSystemData;
	CScanTracDlg *vMainWindowPointer;
	CSystemConfigurationData *vLocalConfigurationData;
	tCWndCollection vLocalCWndCollection;

	//Methods
	CUsersDialog(CWnd* pParent = NULL);
	~CUsersDialog();
	void UpdateDisplay();
	virtual void Serialize(CArchive& ar);
	void ResizeUserArray();
	void LoadUserData();
	void UpdateUsersData(CSetupUserDialog &TempSetupUserDialog);
	BYTE FindLevelFromPassword(CString Password);
	CString FindUserFromPassword(CString Password);
	void WriteUsersToCSVFile();
	void LoadUserFromCSVFile();
	bool FindUsername(CString TempUsername);

	enum { IDD = IDD_UsersDialog };
	CStatic	m_Background;
	CStatic m_DialogTitleStaticText;
	CDownFunctionButton m_Function1Button;
	CDownFunctionButton	m_Function2Button;
	CDownFunctionButton m_Function3Button;
	CDownFunctionButton	m_Function4Button;
	CDownFunctionButton m_Function5Button;
	CLeftFunctionButton m_SubFunction1Button;
	CLeftFunctionButton m_SubFunction2Button;
	CLeftFunctionButton m_SubFunction3Button;
	CLeftFunctionButton m_SubFunction4Button;
	CRightFunctionButton m_SubFunction5Button;
	CRightFunctionButton m_SubFunction6Button;
	CRightFunctionButton m_SubFunction7Button;
	CRightFunctionButton m_SubFunction8Button;
	CStatic m_Function3Display;
	CStatic m_SubFunction1Display;
	CStatic m_SubFunction2Display;
	CStatic m_SubFunction3Display;
	CStatic m_SubFunction4Display;
	CStatic m_SubFunction5Display;
	CStatic m_SubFunction6Display;
	CStatic m_SubFunction7Display;
	CStatic m_SubFunction8Display;
	CStatic m_CheckMarkGraphic1;
	CStatic m_CheckMarkGraphic2;
	CStatic m_CheckMarkGraphic3;
	CStatic m_CheckMarkGraphic4;
	CStatic m_CheckMarkGraphic5;
	CStatic m_CheckMarkGraphic6;
	CStatic m_CheckMarkGraphic7;
	CStatic m_CheckMarkGraphic8;

	//IDC_CheckMarkGraphic1

//BEGIN_MSG_MAP(CUsersDialog)
//	//MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
//	//COMMAND_HANDLER(IDOK, BN_CLICKED, OnClickedOK)
//	//COMMAND_HANDLER(IDCANCEL, BN_CLICKED, OnClickedCancel)
//	//CHAIN_MSG_MAP(CAxDialogImpl<CUsersDialog>)
//END_MSG_MAP()


	DECLARE_MESSAGE_MAP()
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual void DoDataExchange(CDataExchange* pDX);
	afx_msg void OnFunction1Button();
	afx_msg void OnFunction2Button();
	afx_msg void OnFunction3Button();
	afx_msg void OnFunction4Button();
	afx_msg void OnFunction5Button();
	afx_msg void OnSubFunction1Button();
	afx_msg void OnSubFunction2Button();
	afx_msg void OnSubFunction3Button();
	afx_msg void OnSubFunction4Button();
	afx_msg void OnSubFunction5Button();
	afx_msg void OnSubFunction6Button();
	afx_msg void OnSubFunction7Button();
	afx_msg void OnSubFunction8Button();
	
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnDialogtitlestatictext1();
};


