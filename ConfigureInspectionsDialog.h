//ScanTrac Side View Source File
#if !defined(AFX_CONFIGUREINSPECTIONSDIALOG_H__56CEFD53_4D5B_11D6_AFBF_00500466E305__INCLUDED_)
#define AFX_CONFIGUREINSPECTIONSDIALOG_H__56CEFD53_4D5B_11D6_AFBF_00500466E305__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConfigureInspectionsDialog.h : header file
//
#include "Inspection.h"

/////////////////////////////////////////////////////////////////////////////
// CConfigureInspectionsDialog dialog

class CConfigureInspectionsDialog : public CDialog
{
public:
	//variables
	bool vHaveExternalDetectorsConfigured;
	bool vInitialized;
	unsigned long int vGrabFocusTimerHandle;
	bool vShowReturnToEditMenu;
	bool vOldXRayOnNotice;
  unsigned long int vOneSecondTimerHandle;
	BYTE vNumberEjectorsConfigured;
	BYTE vOnlyEjectorNumberConfigured;
	bool vSelectedEvaluate;
	BYTE vEjector;
	bool ShowFinishAutoSetupButton;
	bool vNewInspection;
	bool vPickOneToEvaluate;
	CInspection *vInspectionToEdit;
	bool vChangeMade;
	CScanTracDlg *vMainWindowPointer;
	CScanTracSystemRunningData *vLocalSystemData;
	tProductCollection *vLocalProductCollection;
	CSystemConfigurationData *vLocalConfigurationData;
	CProduct *vProductEditing;
	tCWndCollection vLocalCWndCollection;
	
	// Construction
	CConfigureInspectionsDialog(CWnd* pParent = NULL);   // standard constructor

	//methods
	CProduct *GetProductByNumber(int TempProductNumber);
	void SelectAndEdit(int TempWindowTypeMode);
	void UpdateInspectionsOnDisplay();
	void UpdateCurrentProductOnDisplay(); 
	void SetChangeMade();
	void OneSecondUpdate();
	void ShowInspxButtons();
	void DisplayXofYAlarm();
	void EditAlarm(BYTE TempAlarm);
	BYTE SetupProductEjectors();
	void AskForWeightTrendingInspectionToLinkThisOneTo(BYTE TempThisInspection);

// Dialog Data
	//{{AFX_DATA(CConfigureInspectionsDialog)
	enum { IDD = IDD_ConfigureInspections };
	CStatic	m_InspectionList;
	CStatic	m_SubFunction4Display;
	CStatic	m_SubFunction3Display;
	CDownFunctionButton	m_Function1Button;
	CRightFunctionButton	m_SubFunction7Button;
	CRightFunctionButton	m_SubFunction6Button;
	CStatic	m_XRaysOn;
	CRightFunctionButton	m_SubFunction5Button;
	CLeftFunctionButton	m_SubFunction4Button;
	CLeftFunctionButton	m_SubFunction3Button;
	CLeftFunctionButton	m_SubFunction2Button;
	CLeftFunctionButton	m_SubFunction1Button;
	CDownFunctionButton	m_Function2Button;
	CDownFunctionButton	m_Function4Button;
	CDownFunctionButton	m_Function5Button;
	CStatic	m_Background;
	CDownFunctionButton m_Function3Button;
	CStatic m_CPProductLabel;
	CStatic m_DialogTitleStaticText;
	CRightFunctionButton m_SubFunction8Button;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConfigureInspectionsDialog)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CConfigureInspectionsDialog)
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
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDialogtitlestatictext1();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONFIGUREINSPECTIONSDIALOG_H__56CEFD53_4D5B_11D6_AFBF_00500466E305__INCLUDED_)
