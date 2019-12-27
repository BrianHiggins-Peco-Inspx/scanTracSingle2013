//ScanTrac Side View Source File
#include "afxwin.h"
#if !defined(AFX_FRONTENDDIAGNOSTICDIALOG_H__42073131_FB7B_11D4_AD4C_00500466E305__INCLUDED_)
#define AFX_FRONTENDDIAGNOSTICDIALOG_H__42073131_FB7B_11D4_AD4C_00500466E305__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FrontEndDiagnosticDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFrontEndDiagnosticDialog dialog

class CFrontEndDiagnosticDialog : public CDialog
{
public:
	//variables
	bool vHaveAUSBMemoryStick;
	bool vOldHaveAUSBMemoryStick;
	WORD vOldDACTestErrorCount;
	WORD vOldDACTestCount;
	bool vTestingDACs;

	WORD vOldADCTestErrorCount;
	WORD vOldADCTestCount;
	bool vTestingADCs;
	int vOldPasswordLevel;
	BYTE PingTimeOut;
	BYTE vLightsOnTime;
	bool vPingDNS;
	bool vOldTesting;
	bool vMakeExitButtonGreen;
	bool vHoldingSentTestDisplay;
	unsigned int vHoldingSentTestDisplayTimerHandle;
	bool vHoldingTestDisplay;
	unsigned int vHoldingTestDisplayTimerHandle;
	bool vHoldingReceivedDisplay;
	unsigned int vHoldingReceivedDisplayTimerHandle;
	bool vHoldingSentDisplay;
	unsigned int vHoldingSentDisplayTimerHandle;
	bool vSendingTimerContinuous;
	WORD vOlduCWatchDogCount;
	WORD vOlduCResetCount;
	bool vAlternateFunctions;
	bool vOldNoCommunicationsWithUController;
	bool vOldXRayNoCommuncations;
	bool vTimerOn;
	unsigned int vTestCommTimerHandle;
	unsigned int vPingTimerHandle;
	bool vOldXRayOnNotice;
	bool vChangeMade;
	unsigned long int vUpdateDisplayTimerHandle;
	bool vSerialLogDirectoryFull;
	CScanTracDlg *vMainWindowPointer;
	tProductCollection *vLocalProductCollection;
	CScanTracSystemRunningData *vLocalSystemData;
	CSystemConfigurationData *vLocalConfigurationData;
	tCWndCollection vLocalCWndCollection;
	bool vSubFunction1ButtonEnable;
	bool vSubFunction2ButtonEnable;
	bool vSubFunction3ButtonEnable;
	bool vSubFunction4ButtonEnable;

	// Construction
	CFrontEndDiagnosticDialog(CWnd* pParent = NULL);   // standard constructor

	//methods
	void SetChangeMade();
	void SendGenerate16HzCommand(BYTE TempOn);
	void SenduCSerialDebug(bool TempOn);
	void UpdateButtons78();
	void StopContinuousCommunicationTests(); 
	void UpdateDisplay();
	void ShowCommand();
	void PrepareAndExit(int TempExitCode);
	void CreatePingTestThread();
	void UpdatePingDisplay();
	//void DeleteFiles(CString TempFileType);
	void UpdateInspxItems();

// Dialog Data
	//{{AFX_DATA(CFrontEndDiagnosticDialog)
	enum { IDD = IDD_FrontEndDiagnosticDialog };
	CStatic	m_Network2RedFlash2;
	CStatic	m_Network2GreenFlash2;
	CStatic	m_GatewayLabel;
	CStatic	m_Network1RedFlash;
	CStatic	m_Network1GreenFlash;
	CRightFunctionButton	m_SubFunction7Button;
	CStatic	m_Com4GreenFlash;
	CStatic	m_Com3RedFlash;
	CStatic	m_Com3GreenFlash;
	CStatic	m_Com1RedFlash;
	CStatic	m_Com1GreenFlash;
	CStatic	m_XRayCommunications;
	CStatic	m_XRayLabel;
	CStatic	m_XRayPort;
	CDownFunctionButton	m_Function5Button;
	CStatic	m_LoggingSyncData;
	CStatic	m_LoggingSerialData;
	CDownFunctionButton	m_Function1Button;
	CLeftFunctionButton	m_SubFunction3Button;
	CLeftFunctionButton	m_SubFunction2Button;
	CLeftFunctionButton	m_SubFunction1Button;
	CStatic	m_Command4;
	CStatic	m_Command3;
	CStatic	m_Command2;
	CStatic	m_Command1;
	CRightFunctionButton	m_SubFunction6Button;
	CRightFunctionButton	m_SubFunction5Button;
	CDownFunctionButton	m_Function3Button;
	CRightFunctionButton	m_SubFunction8Button;
	CLeftFunctionButton	m_SubFunction4Button;
	CDownFunctionButton	m_Function4Button;
	CDownFunctionButton	m_Function2Button;
	CStatic	m_XRaysOn;
	CStatic	m_Background;
	CStatic m_IPAddressDisplay;
	CStatic m_CommStatus;
	CStatic m_SendCount;
	CStatic m_ReceivedSerialDataStaticText;
	CStatic m_uCResetCount;
	CStatic m_uCResetCountLabel;
	CStatic m_uCWatchDogCount;
	CStatic m_uCWatchDogCountLabel;
	CStatic m_DataSentLabel;
	CStatic m_DataReceivedLabel;
	CStatic m_SentSerialDataStaticText;
	CStatic m_HVPSType;
	CStatic m_TxBackGroundCom3;
	CStatic m_TxBackGroundCom1;
	CStatic m_RxBackGroundCom1;
	CStatic m_RxBackGroundCom3;
	CStatic m_RxBackGroundCom4;
	CStatic m_PowerMonitorLabel;
	CStatic m_uCLabel;
	CStatic m_TestDACsButton;
	CStatic m_TestADCsButton;
	CStatic m_Tx;
	CStatic m_MicroCPortLabel;
	CStatic m_Com2;
	CStatic m_DXM_MonoPortLabel;
	CStatic m_PowerMonitorPortLabel;
	CStatic m_ComPort;
	CStatic m_ComPortLabel;
	CStatic m_DialogTitleStaticText;
	CStatic m_DNSCommLight;
	CStatic m_GatewayCommLight;
	CStatic m_GatewayLabel2;
	CStatic m_NotMonitored;
	CStatic m_Rx;
	CStatic m_UControllerCommunications;
	CStatic m_UControllerCommunicationsLabel;
	CStatic m_UPS;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFrontEndDiagnosticDialog)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFrontEndDiagnosticDialog)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT nIDEvent);
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
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTestdacsbutton();
	afx_msg void OnTestadcsbutton();
	afx_msg void OnDialogtitlestatictext1();
	CStatic m_Com5GreenFlash;
	CStatic m_Com5Label;
	CStatic m_Com5RedFlash;
	CStatic m_TxBackGroundCom5;
	CStatic m_RxBackGroundCom5;
	CStatic m_InterlocksLabel;
	CStatic m_RxBackGroundCom2;
	CStatic m_TxBackGroundCom2;
	CStatic m_Com2GreenFlash;
	CStatic m_Com2RedFlash;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FRONTENDDIAGNOSTICDIALOG_H__42073131_FB7B_11D4_AD4C_00500466E305__INCLUDED_)
