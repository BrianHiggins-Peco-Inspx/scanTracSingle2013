//ScanTrac Side View Source File
#if !defined(AFX_CONFIGUREPRODUCTCONTAMINANTPROCESSESDIALOG_H__A5CAB4F1_F94B_11D4_AD4A_00500466E305__INCLUDED_)
#define AFX_CONFIGUREPRODUCTCONTAMINANTPROCESSESDIALOG_H__A5CAB4F1_F94B_11D4_AD4A_00500466E305__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConfigureProductContaminantProcessesDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConfigureProductContaminantProcessesDialog dialog
enum {cContaminant, cVoid, cDensity, cKernel, cSize};

class CConfigureProductContaminantProcessesDialog : public CDialog
{
public:
	//variables
	BYTE vPerformGlassAlgorithm;
	CInspection *vInspectionEditing;
	CString vStep1;
	CString vStep2;
	CString vStep3;
	CString vStep4;
	CString vStep5;
	CString vStep6;
	CString vStep7;
	CString vStep8;
	CString vName;
	BYTE vThreshold;
	BYTE vInvertEjectLogic;
	DWORD vSize;
	WORD vMaximumSize;
	WORD vMinimumWidth;
	WORD vMaximumHeightAboveBottom;
	WORD vMinimumNumberOfItemsRequired;
	WORD vMaximumNumberOfItemsRequired;
	float vMinimumBrightnessAceptable;

	//double vOriginalImageFactor;
	//double vEnhancedImageFactor;

	//BYTE vDoImageFactors;
	BYTE vDoDilation;
	BYTE vDilationCount;
	BYTE vErodeCount;
	BYTE vErodeCountAtStart;
	BYTE vDilationCountAtEnd;
	BYTE vErodeCountAtEnd;
	BYTE vSubtractAverage;
	BYTE vAddStructure;
	BYTE vAddHistogram;
	bool vChangeMade;
	BYTE vEnhanceMethod;
	CString *vProductName;
	CProduct *vPointerToProductEditing;
	CSystemConfigurationData *vLocalConfigurationData;
	CScanTracSystemRunningData *vLocalSystemData;
	tCWndCollection vLocalCWndCollection;

	// Construction
	CConfigureProductContaminantProcessesDialog(CWnd* pParent = NULL);   // standard constructor

	//methods
	void SetChangeMade();
	void UpdateDisplay();
	bool CheckConfiguredOK();

// Dialog Data
	//{{AFX_DATA(CConfigureProductContaminantProcessesDialog)
	enum { IDD = IDD_ConfigureProductContaminantProcesses };
	CStatic	m_SubFunction5bDisplay;
	CStatic	m_SubFunction5aDisplay;
	CStatic	m_SubFunction6bDisplay;
	CStatic	m_SubFunction6aDisplay;
	CStatic	m_SubFunction2Display;
	CStatic	m_SubFunction8Display;
	CRightFunctionButton	m_SubFunction8Button;
	CStatic	m_SubFunction7Display;
	CRightFunctionButton	m_SubFunction6Button;
	CRightFunctionButton	m_SubFunction5Button;
	CLeftFunctionButton	m_SubFunction4Button;
	CLeftFunctionButton	m_SubFunction3Button;
	CLeftFunctionButton	m_SubFunction2Button;
	CLeftFunctionButton	m_SubFunction1Button;
	CStatic	m_SubFunction3Display;
	CStatic	m_ProcessStatus6;
	CStatic	m_ProcessStatus5;
	CStatic	m_ProcessStatus4;
	CStatic	m_ProcessStatus3;
	CStatic	m_ProcessStatus2;
	CStatic	m_ProcessStatus1;
	CDownFunctionButton	m_Function5Button;
//	CStatic	m_ContaminantThreshold;
	CRightFunctionButton	m_SubFunction7Button;
	CDownFunctionButton	m_Function4Button;
	CDownFunctionButton	m_Function2Button;
	CStatic	m_Background;
	CStatic m_DialogTitleStaticText;
	CDownFunctionButton m_Function1Button;
	CDownFunctionButton m_Function3Button;
	CStatic m_ProductNameTitle;
	CStatic m_ProductNameTitle2;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConfigureProductContaminantProcessesDialog)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CConfigureProductContaminantProcessesDialog)
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
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDialogtitlestatictext1();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONFIGUREPRODUCTCONTAMINANTPROCESSESDIALOG_H__A5CAB4F1_F94B_11D4_AD4A_00500466E305__INCLUDED_)
