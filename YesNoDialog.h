//ScanTrac Side View Source File
#if !defined(AFX_YESNODIALOG_H__6A785544_D90F_11D3_ABCE_00500466E305__INCLUDED_)
#define AFX_YESNODIALOG_H__6A785544_D90F_11D3_ABCE_00500466E305__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// YesNoDialog.h : header file
//

enum {cConfirmQuestion, cConfirmAbandon, cConfirmSave, cQuestion, cWarningQuestion};

/////////////////////////////////////////////////////////////////////////////
// CYesNoDialog dialog

class CYesNoDialog : public CDialog
{
public:
	unsigned long int vGrabFocusTimerHandle;
	bool vPlaceAtTopOfScreen;
	HBRUSH vYellowBrush;
	HBRUSH vLightYellowBrush;
	HBRUSH vLightBlueBrush;
	HBRUSH vVeryLightBlueBrush;
	HBRUSH vBlueBrush;
	HBRUSH vBrilliantBlueBrush;
	HBRUSH vLightBrilliantBlueBrush;
	LPCTSTR vNoticeText;
	BYTE vQuestionType;
	CString vYesButtonText;
	CString vNoButtonText;
	CFont vCurrentSmallFont;
	tCWndCollection vLocalCWndCollection;

	// Construction
	CYesNoDialog(CWnd* pParent = NULL);   // standard constructor
	DECLARE_DYNCREATE(CYesNoDialog)

	//destructor
	virtual ~CYesNoDialog();

	// Dialog Data
	//{{AFX_DATA(CYesNoDialog)
	enum { IDD = IDD_YesNoDialog };
	CDownFunctionButton	m_NoButtonControl;
	CDownFunctionButton	m_YesButtonControl;
	CButton	m_NoticeTextButtonControl;
	CStatic	m_MessageTypeStaticTextControl;
	CStatic m_CheckMarkGraphic;
	CBitmap vGraphicBitmap;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CYesNoDialog)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void SetupMenu(tCWndCollection&);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CYesNoDialog)
	afx_msg void OnNoButton();
	afx_msg void OnYesButton();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
void ShowNoHelpAvailable();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_YESNODIALOG_H__6A785544_D90F_11D3_ABCE_00500466E305__INCLUDED_)
