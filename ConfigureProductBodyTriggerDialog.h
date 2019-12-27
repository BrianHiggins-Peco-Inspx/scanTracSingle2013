//ScanTrac Side View Source File
#if !defined(AFX_CONFIGUREPRODUCTBODYTRIGGERDIALOG_H__4E9D91A1_DE5C_11D3_ABCE_00500466E305__INCLUDED_)
#define AFX_CONFIGUREPRODUCTBODYTRIGGERDIALOG_H__4E9D91A1_DE5C_11D3_ABCE_00500466E305__INCLUDED_

#include "SystemConfigurationData.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConfigureProductBodyTriggerDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConfigureProductBodyTriggerDialog dialog

class CConfigureProductBodyTriggerDialog : public CDialog
{
public:
	BYTE vNumberOfBodyTriggersInSystem;
	BYTE vShowExit;
	bool vInAutoSetup;
	bool vChangeMade;
	BYTE vBodyTriggerToUse;
	BYTE vBackupBodyTriggerToUse;
	BYTE m_ConfigureProductBodyTriggerEditBoxValueByte;
	CProduct *vCPBTLocalCurrentProductPointer;
	CSystemConfigurationData *vLocalConfigurationData;
	CScanTracSystemRunningData *vLocalSystemData;
	tCWndCollection vLocalCWndCollection;
// Construction
	CConfigureProductBodyTriggerDialog(CWnd* pParent = NULL);   // standard constructor
	DECLARE_DYNCREATE(CConfigureProductBodyTriggerDialog)
	//methods
	void UpdateBodyTriggerDisplay(); 
	void SetChangeMade();
	void SaveAndExit();

// Dialog Data
	//{{AFX_DATA(CConfigureProductBodyTriggerDialog)
	enum { IDD = IDD_ConfigureProductBodyTriggerDialog };
	CRightFunctionButton	m_SubFunction8Button;;
	CStatic	m_BackupBodyTriggerToUse;
	CStatic	m_BackupBodyTriggerToUseLabel;
	CDownFunctionButton	m_Function5Button;
	CDownFunctionButton	m_Function4Button;
	CDownFunctionButton	m_Function2Button;
	CStatic	m_Background;
	CStatic m_BodyTriggerName;
	CStatic m_BodyTriggerToUseLabel;
	CStatic m_DialogTitleStaticText;
	CDownFunctionButton m_Function1Button;
	CDownFunctionButton m_Function3Button;
	CStatic m_ProductNameTitleStaticText;
	CLeftFunctionButton m_SubFunction1Button;
	CLeftFunctionButton m_SubFunction2Button;
	CLeftFunctionButton m_SubFunction3Button;
	CLeftFunctionButton m_SubFunction4Button;
	CRightFunctionButton m_SubFunction5Button;
	CRightFunctionButton m_SubFunction6Button;
	CRightFunctionButton m_SubFunction7Button;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConfigureProductBodyTriggerDialog)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CConfigureProductBodyTriggerDialog)
	afx_msg void OnFunction3Button();
	afx_msg void OnFunction5Button();
	afx_msg void OnFunction1Button();
	afx_msg void OnSubFunction5Button();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSubFunction8Button();
	afx_msg void OnFunction2Button();
	afx_msg void OnFunction4Button();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSubFunction6Button();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDialogtitlestatictext1();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONFIGUREPRODUCTBODYTRIGGERDIALOG_H__4E9D91A1_DE5C_11D3_ABCE_00500466E305__INCLUDED_)
