//ScanTrac Side View Source File
#if !defined(AFX_PROCESSQUICKLEARNIMAGESTHREAD_H__18EDE323_CA33_11DA_B32B_00500466E305__INCLUDED_)
#define AFX_PROCESSQUICKLEARNIMAGESTHREAD_H__18EDE323_CA33_11DA_B32B_00500466E305__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProcessQuickLearnImagesThread.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CProcessQuickLearnImagesThread thread

class CProcessQuickLearnImagesThread : public CWinThread
{
	DECLARE_DYNCREATE(CProcessQuickLearnImagesThread)
public:
	//variables
	CScanTracSystemRunningData *vLocalSystemData;
	CSystemConfigurationData *vLocalConfigurationData;
	CScanTracDlg *vMainWindowPointer;
	BOOLEAN vShutDownNow;

protected:
	CProcessQuickLearnImagesThread();           // protected constructor used by dynamic creation
	virtual ~CProcessQuickLearnImagesThread();

	//methods
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProcessQuickLearnImagesThread)
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

#endif // !defined(AFX_PROCESSQUICKLEARNIMAGESTHREAD_H__18EDE323_CA33_11DA_B32B_00500466E305__INCLUDED_)
