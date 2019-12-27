//ScanTrac Side View Source File
#if !defined(AFX_PROCESSEVALUATEIMAGESTHREAD_H__18EDE323_CA33_11DA_B32C_00500466E305__INCLUDED_)
#define AFX_PROCESSEVALUATEIMAGESTHREAD_H__18EDE323_CA33_11DA_B32C_00500466E305__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CProcessEvaluateImagesThread.h : header file
//

//#include "afxwin.h"

/////////////////////////////////////////////////////////////////////////////
// CProcessEvaluateImagesThread thread

class CProcessEvaluateImagesThread : public CWinThread
{
	DECLARE_DYNCREATE(CProcessEvaluateImagesThread)
public:
	//variables
	CScanTracSystemRunningData *vLocalSystemData;
	CSystemConfigurationData *vLocalConfigurationData;
	CScanTracDlg *vMainWindowPointer;
	BOOLEAN vShutDownNow;

protected:
	CProcessEvaluateImagesThread();// protected constructor used by dynamic creation
	virtual ~CProcessEvaluateImagesThread();

	//methods
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProcessEvaluateImagesThread)
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual int Run();
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CProcessQuickLearnImagesThread)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROCESSEVALUATEIMAGESTHREAD_H__18EDE323_CA33_11DA_B32C_00500466E305__INCLUDED_)
