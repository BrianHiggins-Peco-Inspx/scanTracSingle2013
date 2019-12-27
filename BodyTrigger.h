//ScanTrac Side View Source File
// BodyTrigger.h: interface for the CBodyTrigger class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BODYTRIGGER_H__D72D37D6_DA8D_11D3_ABCE_00500466E305__INCLUDED_)
#define AFX_BODYTRIGGER_H__D72D37D6_DA8D_11D3_ABCE_00500466E305__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DigitalLine.h"

class CBodyTrigger : public CDigitalLine  
{
DECLARE_SERIAL( CBodyTrigger )
public:
	//methods
	CBodyTrigger();
//	DECLARE_DYNCREATE(CBodyTrigger)
	virtual ~CBodyTrigger();

};

#endif // !defined(AFX_BODYTRIGGER_H__D72D37D6_DA8D_11D3_ABCE_00500466E305__INCLUDED_)
