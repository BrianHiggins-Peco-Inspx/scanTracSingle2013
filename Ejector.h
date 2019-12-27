//ScanTrac Side View Source File
// Ejector.h: interface for the CEjector class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EJECTOR_H__D72D37DC_DA8D_11D3_ABCE_00500466E305__INCLUDED_)
#define AFX_EJECTOR_H__D72D37DC_DA8D_11D3_ABCE_00500466E305__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DigitalLine.h"

class CEjector : public CDigitalLine  
{
DECLARE_SERIAL( CEjector )
public:
	//variables
	BYTE vConfirmDigitalInputLine;
	BYTE vConfirmLineBitMask;
	BYTE vConfirmEnabled;
	BYTE vConfirmActiveHigh;
	//methods
	CEjector();
//	DECLARE_DYNCREATE(CEjector)
	virtual ~CEjector();
	virtual void Serialize( CArchive& TempArchive );
	void SetConfirmDigitalInputLine(BYTE TempByte);

};

#endif // !defined(AFX_EJECTOR_H__D72D37DC_DA8D_11D3_ABCE_00500466E305__INCLUDED_)
