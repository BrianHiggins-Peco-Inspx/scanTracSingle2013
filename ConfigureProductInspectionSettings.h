//ScanTrac Side View Source File
#if !defined(AFX_CONFIGUREPRODUCTINSPECTIONSETTINGS_H__F04C7C11_D0EE_11D8_B1B1_00500466E305__INCLUDED_)
#define AFX_CONFIGUREPRODUCTINSPECTIONSETTINGS_H__F04C7C11_D0EE_11D8_B1B1_00500466E305__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConfigureProductInspectionSettings.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConfigureProductInspectionSettings dialog

class CConfigureProductInspectionSettings : public CDialog
{
public:
	//variables
	BYTE vMinimumLength;
	BYTE vIgnoreEdgePixels;
	BYTE vTakeDerivative;
	BYTE vTrimEdgesOfJar;
	BYTE vSwungBaffleTolerance;
	BYTE vRemoveJarPunt;
	BYTE vPixelLookAhead;
	BYTE vFollowBottomUpAndDown;
	BYTE vTryThresholdsDownToThreshold;
	BYTE vMaximumDrop;
	BYTE vEnable;
	BYTE vFillInLightSpots;
	BYTE vTakeDifferenceFromMedian;
	BYTE vBonesInspection;
	BYTE vPreSmoothImage;
	BYTE vFindSpotInSubROI;
	BYTE vFindDarkSpotOnLine;
	BYTE vPixelsStepToNextROI;
	BYTE vDesensitizeEdgePixels;
	BYTE vIPContaminant;

	BYTE vDesensitizeIrregularArea;
	//BYTE vPreBlurImage;
	//BYTE vPreSelectDarkSpotsInImage;
	//BYTE vRequire2InARow;
	BYTE vOverlap;
	CInspection *vInspectionEditing;
	BYTE vInvertEjectLogic;
	bool vChangeMade;
	CString *vProductName;
	CProduct *vPointerToProductEditing;
	CSystemConfigurationData *vLocalConfigurationData;
	CScanTracSystemRunningData *vLocalSystemData;
	tCWndCollection vLocalCWndCollection;

// Construction
	CConfigureProductInspectionSettings(CWnd* pParent = NULL);   // standard constructor

	//methods
	void SetChangeMade();
	void UpdateDisplay();

// Dialog Data
	//{{AFX_DATA(CConfigureProductInspectionSettings)
	enum { IDD = IDD_ConfigureProductInspctionSettings };
	CStatic	m_Function3Display;
	CDownFunctionButton	m_Function3Button;
	CStatic	m_Function5Display;
	CDownFunctionButton	m_Function5Button;
	CStatic	m_SubFunction7Display;
	CRightFunctionButton	m_SubFunction7Button;
	CStatic	m_SubFunction6Display;
	CRightFunctionButton	m_SubFunction6Button;
	CStatic	m_SubFunction1Display;
	CLeftFunctionButton	m_SubFunction1Button;
	CStatic	m_SubFunction2Display;
	CLeftFunctionButton	m_SubFunction2Button;
	CStatic	m_SubFunction4Display;
	CStatic	m_SubFunction3Display;
	CLeftFunctionButton	m_SubFunction4Button;
	CLeftFunctionButton	m_SubFunction3Button;
	CDownFunctionButton	m_Function4Button;
	CDownFunctionButton	m_Function2Button;
	CStatic	m_Background;
	CStatic m_DialogTitleStaticText;
	CStatic m_Enable;
	CDownFunctionButton m_Function1Button;
	CStatic m_ProcessStatus3;
	CStatic m_ProductNameTitle;
	CStatic m_ProductNameTitle2;
	CRightFunctionButton m_SubFunction5Button;
	CStatic m_SubFunction5Display;
	CRightFunctionButton m_SubFunction8Button;
	CStatic m_SubFunction8Display;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConfigureProductInspectionSettings)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CConfigureProductInspectionSettings)
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
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDialogtitlestatictext1();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONFIGUREPRODUCTINSPECTIONSETTINGS_H__F04C7C11_D0EE_11D8_B1B1_00500466E305__INCLUDED_)
