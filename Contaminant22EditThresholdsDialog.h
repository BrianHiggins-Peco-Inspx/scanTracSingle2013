#if !defined(AFX_CONTAMINANT22EDITTHRESHOLDSDIALOG_H__3BA0C1EC_3E4C_4130_866F_8C325287BA20__INCLUDED_)
#define AFX_CONTAMINANT22EDITTHRESHOLDSDIALOG_H__3BA0C1EC_3E4C_4130_866F_8C325287BA20__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Contaminant22EditThresholdsDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CContaminant22EditThresholdsDialog dialog

class CContaminant22EditThresholdsDialog : public CDialog
{
public:
	//variables
	bool vChangeMade;
	bool vHorizontal;
	short _ThresholdsH[7];
	short _ThresholdsV[7];

	CInspection *vInspectionEditing;
	CScanTracSystemRunningData *vLocalSystemData;
	CSystemConfigurationData *vLocalConfigurationData;
	tCWndCollection vLocalCWndCollection;

// Construction
	CContaminant22EditThresholdsDialog(CWnd* pParent = NULL);   // standard constructor

	//methods
	void SetChangeMade();
	void ShowThresholdValues();
	void DisplayButtonLabels();
	void EditThresholdValue(BYTE TempNumber);
	CString GetName(BYTE TempNumber);
	short GetDefault(BYTE TempNumber);

// Dialog Data
	//{{AFX_DATA(CContaminant22EditThresholdsDialog)
	enum { IDD = IDD_Contaminant22EditThresholdsDialog };
	CDownFunctionButton	m_Function4Button;
	CDownFunctionButton	m_Function2Button;
	CStatic	m_ArrowDown4;
	CStatic	m_Background;
	CStatic m_ArrowDown1;
	CStatic m_ArrowDown2;
	CStatic m_ArrowDown3;
	CStatic m_ArrowDown5;
	CStatic m_ArrowLeft1;
	CStatic m_ArrowLeft2;
	CStatic m_ArrowLeft3;
	CStatic m_ArrowLeft4;
	CStatic m_ArrowRight1;
	CStatic m_ArrowRight2;
	CStatic m_ArrowRight3;
	CStatic m_ArrowRight4;
	CStatic m_DialogTitleStaticText;
	CDownFunctionButton m_Function1Button;
	CDownFunctionButton m_Function3Button;
	CStatic m_Function3Display;
	CDownFunctionButton m_Function5Button;
	CFunctionButton m_SubFunction1Button;
	CStatic m_SubFunction1Display;
	CFunctionButton m_SubFunction2Button;
	CStatic m_SubFunction2Display;
	CFunctionButton m_SubFunction3Button;
	CStatic m_SubFunction3Display;
	CFunctionButton m_SubFunction4Button;
	CStatic m_SubFunction4Display;
	CFunctionButton m_SubFunction5Button;
	CStatic m_SubFunction5Display;
	CFunctionButton m_SubFunction6Button;
	CStatic m_SubFunction6Display;
	CFunctionButton m_SubFunction7Button;
	CStatic m_SubFunction7Display;
	CFunctionButton m_SubFunction8Button;
	CStatic m_SubFunction8Display;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CContaminant22EditThresholdsDialog)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CContaminant22EditThresholdsDialog)
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnFunction1Button();
	afx_msg void OnFunction2Button();
	afx_msg void OnFunction3Button();
	afx_msg void OnFunction4Button();
	afx_msg void OnSubFunction1Button();
	afx_msg void OnFunction5Button();
	afx_msg void OnSubFunction2Button();
	afx_msg void OnSubFunction3Button();
	afx_msg void OnSubFunction4Button();
	afx_msg void OnSubFunction5Button();
	afx_msg void OnSubFunction6Button();
	afx_msg void OnSubFunction7Button();
	afx_msg void OnSubFunction8Button();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONTAMINANT22EDITTHRESHOLDSDIALOG_H__3BA0C1EC_3E4C_4130_866F_8C325287BA20__INCLUDED_)
