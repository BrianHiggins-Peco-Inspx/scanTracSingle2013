//ScanTrac Side View Source File
#if !defined(AFX_PINGTESTTHREAD_H__0C30E805_7AC8_49AA_8CA5_34E56098953A__INCLUDED_)
#define AFX_PINGTESTTHREAD_H__0C30E805_7AC8_49AA_8CA5_34E56098953A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PingTestThread.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CPingTestThread thread

class CPingTestThread : public CWinThread
{
	DECLARE_DYNCREATE(CPingTestThread)
protected:
	CPingTestThread();           // protected constructor used by dynamic creation

// Attributes
public:

	bool vPingDNS;
	CString vIPAddressToPingString;
	CScanTracDlg *vMainWindowPointer;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPingTestThread)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual int Run();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CPingTestThread();

	// Generated message map functions
	//{{AFX_MSG(CPingTestThread)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PINGTESTTHREAD_H__0C30E805_7AC8_49AA_8CA5_34E56098953A__INCLUDED_)
