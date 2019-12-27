//ScanTrac Side View Source File
#include "afxwin.h"
#if !defined(AFX_INSPXHIDDENDIALOG_H__56B4BE51_A697_11D4_ACDD_00500466E305__INCLUDED_)
#define AFX_INSPXHIDDENDIALOG_H__56B4BE51_A697_11D4_ACDD_00500466E305__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InspxHiddenDialog.h : header file
//
extern BYTE vGlobalShowMouseCursor;

/////////////////////////////////////////////////////////////////////////////
// CInspxHiddenDialog dialog

class CInspxHiddenDialog : public CDialog
{
public:
	// variables
	bool vOldModBusSocketOpen;
	bool vOldModBusClientConnected;
	DWORD vOldModBusClientConnectionCounts;
	DWORD vOldModBusClientRequestCounts;
	DWORD vOldModBusTCPIPErrorCounter;

	unsigned long int vUpdateDisplayTimerHandle;
	BYTE vHVPSType;
//	BYTE vDeltaTemperatureForFilterCloggedError;
//	BYTE vDeltaTemperatureForCoolantProblem;
//	BYTE vHasOutsideAirTemp;
	CScanTracDlg *vMainWindowPointer;
	BYTE vAllowClientConnections;
	BYTE vEnableModBusTCPIPInterface;
	//     0=Sideview,   1=50 watt sideview, 2=50 watt pipeline, 3=600 watt pipeline, 4=12 inch TD,    24 inch TD, 
	BYTE vScanTracType;	
	BYTE vAutoXRaySourceOffEnabled;
	WORD vEnableACMessages;
	BYTE vDemoMode;
	//CString vRampXRayDurationString;
	bool vChangeMade;
	BYTE vSimulateLightControlBoard;
	BYTE vSuppressWarningMessages;
	BYTE vShowingMouseCursor;

	CScanTracSystemRunningData *vLocalSystemData;
	CSystemConfigurationData *vLocalConfigurationData;
	tCWndCollection vLocalCWndCollection;

	// Construction
	CInspxHiddenDialog(CWnd* pParent = NULL);   // standard constructor

	//methods
	void SetChangeMade();
	void UpdateDisplay();
	void UpdateModBusDisplay();

// Dialog Data
	//{{AFX_DATA(CInspxHiddenDialog)
	enum { IDD = IDD_InspxHiddenDialog };
	CStatic	m_SubFunction4Display;
	CDownFunctionButton	m_Function4Button;
	CDownFunctionButton	m_Function2Button;
	CStatic	m_Background;
	CStatic m_AutoXRaySourceOffEnabled;
	CStatic m_BlankExtraLines;
	CStatic m_DemoMode;
	CStatic m_DialogTitleStaticText;
	CDownFunctionButton m_Function1Button;
	CDownFunctionButton m_Function3Button;
	CStatic m_Function3Display;
	CDownFunctionButton m_Function5Button;
	CStatic m_Function5Display;
	CStatic m_SimulateLightControlBoard;
	CLeftFunctionButton m_SubFunction1Button;
	CLeftFunctionButton m_SubFunction2Button;
	CLeftFunctionButton m_SubFunction3Button;
	CLeftFunctionButton m_SubFunction4Button;
	CRightFunctionButton m_SubFunction5Button;
	CRightFunctionButton m_SubFunction6Button;
	CStatic m_SubFunction6Display;
	CRightFunctionButton m_SubFunction7Button;
	CRightFunctionButton m_SubFunction8Button;
	CStatic m_SubFunctionDisplay3;
	CStatic m_SuppressWarningMessages;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInspxHiddenDialog)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInspxHiddenDialog)
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
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnFunction5Display();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDialogtitlestatictext1();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnStnClickedSubfunction9button();
	CStatic m_SubFunction9Button;
	CStatic m_SubFunction6DisplayA;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INSPXHIDDENDIALOG_H__56B4BE51_A697_11D4_ACDD_00500466E305__INCLUDED_)
