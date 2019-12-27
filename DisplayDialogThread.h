//ScanTrac Side View Source File
#if !defined(AFX_DISPLAYDIALOGTHREAD_H__1E6E4E20_2DC2_11D5_ADC3_00500466E305__INCLUDED_)
#define AFX_DISPLAYDIALOGTHREAD_H__1E6E4E20_2DC2_11D5_ADC3_00500466E305__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DisplayDialogThread.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CDisplayDialogThread thread

class CDisplayDialogThread : public CWinThread
{
	DECLARE_DYNCREATE(CDisplayDialogThread)
protected:
	CDisplayDialogThread();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:
	CString vMessageToDisplay;
	BYTE vType;
	bool vDisplaySplashScreenLogo;

	//bool vShutDownNow;
	CNoticeDialog *vWaitToLoadProductNoticeDialog;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDisplayDialogThread)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual int Run();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CDisplayDialogThread();

public:
	//methods
	void ExitDisplayThread();
	void NextStep();

	// Generated message map functions
	//{{AFX_MSG(CDisplayDialogThread)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DISPLAYDIALOGTHREAD_H__1E6E4E20_2DC2_11D5_ADC3_00500466E305__INCLUDED_)
