//ScanTrac Side View Source File
#include "NewConfigureSystemDialog.h"
#include "ConfigureProduct.h"
#include "SystemConfigurationData.h"
#include "afxwin.h"

#if !defined(AFX_SCANTRACCONFIGUREDIALOG_H__54309CC1_D2A5_11D3_ABCE_00500466E305__INCLUDED_)
#define AFX_SCANTRACCONFIGUREDIALOG_H__54309CC1_D2A5_11D3_ABCE_00500466E305__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ScanTracConfigureDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CScanTracConfigureDialog dialog

class CScanTracConfigureDialog : public CDialog
{
public:
	//variables
	bool vChangedLanguage;
	BYTE vLanguage;
	BYTE vReadCount;
	BYTE PingTimeOut;
	bool vPingDNS;
	BYTE vOldShiftKeyDown;
	CString vRegistryKeyPath;
	CString vIPAddressString;
	CString vIPSubNetMaskString;
	CString vIPDefaultGatewayString;
	CString vIPDefaultDNSAddressString;
	BYTE vIPDHCPEnabled;

	BYTE vAutoStop;
	CString vScanTracCustomerName;
	bool vOldGlobalPasswordOK;
	CString vLastDateString;
	BYTE vOldSystemRunMode;
	unsigned long int vOneSecondTimerHandle;
	unsigned long int vPingTimerHandle;
	unsigned long int vUpdateIPAddressTimerHandle;
	BYTE vAskForProductionReportProductCode;
	BYTE vAutoProductionReportHour;
	BYTE vAutoProductionReportHour2;
	BYTE vAutoProductionReportHour3;
	bool vProductPasswordOK;
	bool vChangeMade;
	//CString vCPMTimePeriodString;
	char vTempChars4[6];
	CScanTracSystemRunningData *vLocalSystemData;
	CSystemConfigurationData *vLocalConfigurationData;
	tProductCollection *vLocalProductCollection;
	//CProduct *vpLocalCurrentProduct;
	CScanTracDlg *vMainWindowPointer;
	BOOL vTeamViewerInstalled;
	tCWndCollection vLocalCWndCollection;
	bool vSubFunction6ButtonEnable;
	bool vFunction3ButtonEnable;

	// Construction
	CScanTracConfigureDialog(CWnd* pParent = NULL);   // standard constructor
	DECLARE_DYNCREATE(CScanTracConfigureDialog)
	//methods
	void SetChangeMade();
	void UpdateTimeDisplay();
	void UpdateDisplay();
	void UpdatePingDisplay();
	void UpdateIPAddressDisplay();
	void CreatePingTestThread();


// Dialog Data
	//{{AFX_DATA(CScanTracConfigureDialog)
	enum { IDD = IDD_ConfigureDialog };
	CStatic	m_Network2RedFlash;
	CStatic	m_Network2GreenFlash;
	CRightFunctionButton	m_SubFunction6Button;
	CStatic	m_Network1RedFlash;
	CStatic	m_Network1GreenFlash;
	CStatic	m_SubFunction3Display;
	CStatic	m_SubFunction2Display;
	CStatic m_SubFunction5Display;
	CLeftFunctionButton	m_SubFunction2Button;
	CLeftFunctionButton	m_SubFunction4Button;
	CLeftFunctionButton	m_SubFunction3Button;
	CLeftFunctionButton	m_SubFunction1Button;
	CRightFunctionButton	m_SubFunction8Button;
	CRightFunctionButton	m_SubFunction7Button;
	CDownFunctionButton	m_Function5Button;
	CDownFunctionButton	m_Function3Button;
	CDownFunctionButton	m_Function4Button;
	CDownFunctionButton	m_Function2Button;
	CStatic	m_Background;
	CStatic m_SubFunction1Display;
	CStatic m_SubFunction4Display;
	CRightFunctionButton m_SubFunction5Button;
	CStatic m_Clock;
	CStatic m_CPMTimePeriodDisplay;
	CStatic m_Date;
	CStatic m_DialogTitleStaticText;
	CStatic m_DNSLabel;
	CDownFunctionButton m_Function1Button;
	CStatic m_Function5Display;
	CStatic m_GatewayLabel;
	CStatic m_TeamViewLabel;
	CStatic m_WorkStopTimePeriodDisplay;
	CStatic m_BlackBox1;
	CStatic m_BlackBox2;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScanTracConfigureDialog)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CScanTracConfigureDialog)
	afx_msg void OnFunction2Button();
	afx_msg void OnFunction4Button();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnFunction3Button();
	afx_msg void OnFunction1Button();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSubFunction1Button();
	afx_msg void OnFunction5Button();
	afx_msg void OnSubFunction2Button();
	afx_msg void OnSubFunction3Button();
	afx_msg void OnSubFunction4Button();
	afx_msg void OnSubFunction5Button();
	afx_msg void OnSubFunction6Button();
	afx_msg void OnSubFunction7Button();
	afx_msg void OnSubFunction8Button();
	afx_msg void OnTimer(UINT nIDEvent);
	//afx_msg void OnRestorePrimaryHardDriveButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDialogtitlestatictext1();
	CStatic m_SubFunction7Display;
	bool HardDriveProtectionIsOff();
	ULONG DisableFBWF(void);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCANTRACCONFIGUREDIALOG_H__54309CC1_D2A5_11D3_ABCE_00500466E305__INCLUDED_)
