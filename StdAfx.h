//ScanTrac Side View Source File
// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__1C80CF1D_CEB9_11D3_ABCC_00500466E305__INCLUDED_)
#define AFX_STDAFX_H__1C80CF1D_CEB9_11D3_ABCC_00500466E305__INCLUDED_
//#define WINNT
#define _WIN32_WINNT _WIN32_WINNT_MAXVER


//#define CompileInHTTPClientOperationsGuardian

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#include <afxtempl.h>       // MFC Templates for collections
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

	#define _ATL_APARTMENT_THREADED
#include <atlbase.h>
//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
class CScanTracModule : public CComModule
{
public:
	LONG Unlock();
	LONG Lock();
	LPCTSTR FindOneOf(LPCTSTR p1, LPCTSTR p2);
	DWORD dwThreadID;
};
extern CScanTracModule _Module;
#include <atlcom.h>
#include <afxdisp.h> // CG: added by ActiveX Control Containment component

//added below for ping function
#include <afxwin.h>     
#include <afxext.h>     

#define CPING_USE_ICMP
//#define CPING_USE_WINSOCK2

#ifdef CPING_USE_WINSOCK2
#include <winsock2.h>
#endif
#ifdef CPING_USE_ICMP
#include <winsock.h>
#endif

#include <afxsock.h>
#include <afxdisp.h>
#include <afxpriv.h>
#include <afxcontrolbars.h>
// end for ping function

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__1C80CF1D_CEB9_11D3_ABCC_00500466E305__INCLUDED_)


//include to use halcon libraries
//define UseHalconLibraries
//#ifdef UseHalconLibraries
//#include "HalconCpp.h"
//#include <afxcontrolbars.h>
//using namespace Halcon;
//#endif

