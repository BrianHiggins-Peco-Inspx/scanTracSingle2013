//ScanTrac Side View Source File
// ExternalDetector.h: interface for the CExternalDetector class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXTERNALDETECTOR_H__D72D37DB_DA8D_11D3_ABCE_00500466E305__INCLUDED_)
#define AFX_EXTERNALDETECTOR_H__D72D37DB_DA8D_11D3_ABCE_00500466E305__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DigitalLine.h"

class CExternalDetector : public CDigitalLine  
{
DECLARE_SERIAL( CExternalDetector )
public:
	CExternalDetector();
//	DECLARE_DYNCREATE(CExternalDetector)
	virtual ~CExternalDetector();

};

#endif // !defined(AFX_EXTERNALDETECTOR_H__D72D37DB_DA8D_11D3_ABCE_00500466E305__INCLUDED_)
