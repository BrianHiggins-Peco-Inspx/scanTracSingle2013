//ScanTrac Side View Source File
#if !defined(AFX_DIAGNOSTICLINEDIALOG_H__639CB1E1_278B_11D4_AC45_00500466E305__INCLUDED_)
#define AFX_DIAGNOSTICLINEDIALOG_H__639CB1E1_278B_11D4_AC45_00500466E305__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DiagnosticLineDialog.h : header file
//

#include "SelectItemDialog.h"
#include "afxwin.h"

const BYTE cNumberOfDiagnosticSignals = 50;
enum {cContinuousTrigger, cTriggerOnChannel,cSingleTriggerOnChannel};

/////////////////////////////////////////////////////////////////////////////
// CDiagnosticLineDialog dialog

class CDiagnosticLineDialog : public CDialog
{
public:
	//variables
	DWORD vEjectorToChange;
	BYTE vHalfSecondCount;
	BYTE vLVDSActive;
	BYTE vCamLinkActive;
	BYTE vOldLVDSActive;
	BYTE vOldCamLinkActive;
	DWORD vOldEjectorOnStatus;
	BYTE vOldCurrentConveyorOutput;
	bool vMakeExitButtonGreen;
	WORD vLeftSide;
	WORD vRightSide;
	WORD vTop;
	WORD vBottom;
	WORD vHeight;
	WORD vWidth;
	WORD vTraceHeight;
	int vNumberOfDataPoints;
	bool vTakingData;
	bool vTriggerArmed;
	BYTE vTriggerMode;
	BYTE vTriggerChannel;
	CPen vRedPen;
	CPen vBluePen;
	CPen vDarkBluePen;
	CPen vBlackPen;
	BYTE DiagnosticLineIndexArray[cMaxSelectDialogItems];
	bool vOldXRayOnNotice;
	BYTE vRefreshCount;
	//bool vOscilloscope;
	bool vLogDiagnosticLines;
	WORD vOldEntry;
	WORD vSampleSpeed;
	WORD vSampleSpeedDelayMagnitude;
	double vTimeIntervalFor50SamplesInMicroSeconds;
	unsigned long int vDisplayTimerHandle;
	unsigned long int vClearTimerHandle;
	unsigned long int vOscilloscopeTimerHandle;
	BYTE vOldDiagnosticLineValue;
	CScanTracSystemRunningData *vLocalSystemData;
	CSystemConfigurationData *vLocalConfigurationData;
	CScanTracDlg *vMainWindowPointer;
	tCWndCollection vLocalCWndCollection;
	bool vFunction5ButtonEnable;
	bool vSubFunction2ButtonEnable;
	bool vSubFunction3ButtonEnable;
	bool vSubFunction4ButtonEnable;
	bool vSubFunction5ButtonEnable;


	// Construction
	CDiagnosticLineDialog(CWnd* pParent = NULL);   // standard constructor
	//methods
	void UpdateDisplay();
	void DisplayEjectorOnOffButtons();
	CString GetSignalName(BYTE TempChannel);
	CString GetInputName(BYTE TempLineNumber);
	CString GetJustInputName(BYTE TempChannel);
	void UpdateOscilloscope();
	void UpdateSignalName();
	void PrepareToExit();
	void GetOScopeDataFromGrabber();

// Dialog Data
	//{{AFX_DATA(CDiagnosticLineDialog)
	enum { IDD = IDD_DiagnosticLineDialog };
	CStatic	m_LVDS;
	CStatic	m_CamLink;
	CStatic	m_RefreshCountLabel;
	CStatic	m_DeltaCountLabel;
	CStatic	m_ZoomMagnitude;
	CDownFunctionButton	m_Function1Button;
	CRightFunctionButton	m_SubFunction6Button;
	CRightFunctionButton	m_SubFunction5Button;
	CRightFunctionButton	m_SubFunction7Button;
	CStatic	m_TriggerChannel;
	CStatic	m_TriggerMode;
	CRightFunctionButton	m_SubFunction8Button;
	CLeftFunctionButton	m_SubFunction4Button;
	CLeftFunctionButton	m_SubFunction3Button;
	CLeftFunctionButton	m_SubFunction2Button;
	CDownFunctionButton	m_Function5Button;
	CDownFunctionButton	m_Function4Button;
	CDownFunctionButton	m_Function3Button;
	CStatic	m_XRaysOn;
	CStatic m_DeltaCount;
	CStatic m_DialogTitleStaticText;
	CDownFunctionButton m_Function2Button;
	CStatic m_LineLabel1;
	CStatic m_LineLabel2;
	CStatic m_LineLabel3;
	CStatic m_LineLabel4;
	CStatic m_LineLabel5;
	CStatic m_LineLabel6;
	CStatic m_RefreshCount;
	CLeftFunctionButton m_SubFunction1Button;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDiagnosticLineDialog)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDiagnosticLineDialog)
	afx_msg void OnFunction1Button();
	afx_msg void OnFunction2Button();
	afx_msg void OnFunction3Button();
	afx_msg void OnFunction4Button();
	afx_msg void OnFunction5Button();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSubFunction4Button();
	afx_msg void OnSubFunction3Button();
	afx_msg void OnSubFunction2Button();
	afx_msg void OnSubFunction5Button();
	afx_msg void OnSubFunction1Button();
	afx_msg void OnPaint();
	afx_msg void OnSubFunction6Button();
	afx_msg void OnSubFunction7Button();
	afx_msg void OnSubFunction8Button();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDialogtitlestatictext1();
	afx_msg void OnStnClickedSubfunction0button();
	CStatic m_SubFunction0Button;
	afx_msg void OnStnClickedSubfunction0display();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIAGNOSTICLINEDIALOG_H__639CB1E1_278B_11D4_AC45_00500466E305__INCLUDED_)
