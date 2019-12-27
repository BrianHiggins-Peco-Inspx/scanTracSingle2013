//ScanTrac Side View Source File
// DigitalLine.h: interface for the CDigitalLine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIGITALLINE_H__D72D37D4_DA8D_11D3_ABCE_00500466E305__INCLUDED_)
#define AFX_DIGITALLINE_H__D72D37D4_DA8D_11D3_ABCE_00500466E305__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDigitalLine : public CObject  
{

DECLARE_SERIAL( CDigitalLine )

public:
	//variables
	BYTE vNumber;
	CString vName;
	BYTE vDigitalInputLine;
	BYTE vLineBitMask;
	BYTE vEnabled;
	BYTE vActiveHigh;

	//Methods
	CDigitalLine();
//	DECLARE_DYNCREATE(CDigitalLine)
	virtual ~CDigitalLine();
	virtual void Serialize( CArchive& TempArchive );
	void SetDigitalInputLine(BYTE TempByte);

};

#endif // !defined(AFX_DIGITALLINE_H__D72D37D4_DA8D_11D3_ABCE_00500466E305__INCLUDED_)
