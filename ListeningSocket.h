//ScanTrac Side View Source File
// ListeningSocket.h: interface for the CListeningSocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LISTENINGSOCKET_H__443FF0A4_A834_11D8_B196_00500466E305__INCLUDED_)
#define AFX_LISTENINGSOCKET_H__443FF0A4_A834_11D8_B196_00500466E305__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//class CScanTracDlg;

class CListeningSocket : public CSocket  
{
	DECLARE_DYNAMIC(CListeningSocket);

public:

	//variables
	CScanTracDlg *vPointerToMainWindow;

	//constructors
private:
	CListeningSocket(const CListeningSocket& rSrc);         // no implementation
	void operator=(const CListeningSocket& rSrc);  // no implementation

public:
	CListeningSocket(CScanTracDlg* TempMainWindowPointer);
	virtual ~CListeningSocket();

	//methods
protected:
	virtual void OnAccept(int nErrorCode);

// Implementation
public:

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& TempDumpContext) const;
#endif
};

#endif // !defined(AFX_LISTENINGSOCKET_H__443FF0A4_A834_11D8_B196_00500466E305__INCLUDED_)
