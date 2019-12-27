#include "afxwin.h"
#if !defined(AFX_SETUPSYSTEMEXTERNALHARDWARE_H__D566E325_0C99_4DE1_8E58_84E497B5D209__INCLUDED_)
#define AFX_SETUPSYSTEMEXTERNALHARDWARE_H__D566E325_0C99_4DE1_8E58_84E497B5D209__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetupSystemExternalHardware.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetupSystemExternalHardware dialog

class CSetupSystemExternalHardware : public CDialog
{
// Construction
public:
	//variables
	BYTE vUseExternalInterlock;
	//BYTE vHideNeedDetectorCal;
	BYTE vEnableOperationsGuardian;
	CString vOperationsGuardianServer;
	BYTE vUseDownStreamSignalThatStopsConveyor;
	BYTE vBackupDetectionUsingBodyTriggerTimeOut;
	bool vChangeMade;
	BYTE vRFIDEASBadgeReaderCommPort;

	CScanTracDlg *vMainWindowPointer;
	CScanTracSystemRunningData *vLocalSystemData;
	CSystemConfigurationData *vLocalConfigurationData;
	tProductCollection *vLocalProductCollection;
	tCWndCollection vLocalCWndCollection;

	CSetupSystemExternalHardware(CWnd* pParent = NULL);   // standard constructor

	void UpdateButtons();
	void SetChangeMade();

// Dialog Data
	//{{AFX_DATA(CSetupSystemExternalHardware)
	enum { IDD = IDD_SetupSystemExternalHardware };
	CDownFunctionButton	m_Function4Button;
	CDownFunctionButton	m_Function2Button;
	CStatic	m_SubFunction3Display;
	CLeftFunctionButton	m_SubFunction3Button;
	CStatic	m_Background;
	CStatic m_DialogTitleStaticText;
	CDownFunctionButton m_Function1Button;
	CDownFunctionButton m_Function3Button;
	CDownFunctionButton m_Function5Button;
	CStatic m_Function5Display;
	CLeftFunctionButton m_SubFunction1Button;
	CLeftFunctionButton m_SubFunction2Button;
	CLeftFunctionButton m_SubFunction4Button;
	CRightFunctionButton m_SubFunction5Button;
	CRightFunctionButton m_SubFunction6Button;
	CRightFunctionButton m_SubFunction7Button;
	CStatic m_SubFunction7Display;
	CRightFunctionButton m_SubFunction8Button;
	CStatic m_SubFunction8Display;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetupSystemExternalHardware)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CSetupSystemExternalHardware)
	afx_msg void OnSubFunction1Button();
	afx_msg void OnSubFunction2Button();
	afx_msg void OnSubFunction3Button();
	afx_msg void OnSubFunction4Button();
	afx_msg void OnSubFunction5Button();
	afx_msg void OnSubFunction6Button();
	afx_msg void OnSubFunction7Button();
	afx_msg void OnSubFunction8Button();
	afx_msg void OnFunction1Button();
	afx_msg void OnFunction2Button();
	afx_msg void OnFunction3Button();
	afx_msg void OnFunction4Button();
	afx_msg void OnFunction5Button();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDialogtitlestatictext1();
	CStatic m_SubFunction4Display;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETUPSYSTEMEXTERNALHARDWARE_H__D566E325_0C99_4DE1_8E58_84E497B5D209__INCLUDED_)
