//ScanTrac Side View Source File
#if !defined(AFX_NOTICEDIALOG_H__6A785543_D90F_11D3_ABCE_00500466E305__INCLUDED_)
#define AFX_NOTICEDIALOG_H__6A785543_D90F_11D3_ABCE_00500466E305__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NoticeDialog.h : header file
//
typedef CTypedPtrArray<CObArray, CWnd*> tCWndCollection;
enum {cVersionMessage,cErrorMessage,cWarningMessage,cInformationMessage,
	cNoTypeMessage,cInstructionsMessage,cNoticeMessage,cSafetyMessage, cStartupInfo};

#include "PecoPictureBox.h"
/////////////////////////////////////////////////////////////////////////////
// CNoticeDialog dialog

class CNoticeDialog : public CDialog
{
public:
	unsigned int vSetWindowActiveTimerHandle;
	bool vPlaceAtTopOfScreen;
	bool vCloseOnBrightnessLoopLock;
	WORD vStep;
	WORD vAutoClose;
	WORD vDisplayCount;
	bool vCloseOnTimer;
	HBRUSH vButtonColorBrush;
	HBRUSH vGreenBrush;
	HBRUSH vLightGreenBrush;
	HBRUSH vYellowBrush;
	HBRUSH vLightYellowBrush;
	HBRUSH vRedBrush;
	HBRUSH vLightRedBrush;
	HBRUSH vBlueBrush;
	HBRUSH vLightBlueBrush;
	HBRUSH vVeryLightBlueBrush;
	unsigned long int vDisplayTimerHandle;
	CBitmap vGraphicBitmap;

	CScanTracSystemRunningData *vLocalSystemData;

	CString vNoticeText;
	//LPCTSTR vNoticeText;
	BYTE vType;
	tCWndCollection vLocalCWndCollection;

	// Construction
	CNoticeDialog(CWnd* pParent = NULL);   // standard constructor
	DECLARE_DYNCREATE(CNoticeDialog)

	//destructor
	virtual ~CNoticeDialog();

	//methods
	void SetupMenu(tCWndCollection& TempCWndCollection);
	void UpdateDisplay();
	void NextStep();
	void UpdateText();

// Dialog Data
	//{{AFX_DATA(CNoticeDialog)
	enum { IDD = IDD_NoticeDialog };
	CStatic	m_InformationGraphic;
	CStatic	m_CheckMarkGraphic;
	CStatic	m_ExclaimationGraphic;
	CStatic	m_ErrorGraphic;
	CStatic	m_InspxLogo;
	CPecoPictureBox m_SplashScreenLogo;
	CButton	m_NoticeTextButtonControl;
	CStatic	m_MessageTypeStaticTextControl;
	CStatic m_AnyKeyToContinueStaticText;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNoticeDialog)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNoticeDialog)
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnNoticeTextButton();
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NOTICEDIALOG_H__6A785543_D90F_11D3_ABCE_00500466E305__INCLUDED_)
