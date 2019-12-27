//ScanTrac Side View Source File

#if !defined(AFX_SERIALPORTMODBUS_H__EBA785C3_08E8_11D5_AD6D_00500466E305__INCLUDED_)
#define AFX_SERIALPORTMODBUS_H__EBA785C3_08E8_11D5_AD6D_00500466E305__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CSerialPortModBus.h : header file
//
#include "serialportbase.h"


class CSerialPortModBus : public CSerialPortBase
{
	DECLARE_DYNCREATE(CSerialPortModBus)
	//variables
	CScanTracDlg *vMainWindowPointer;
	BYTE vNumberOfCharactersToExpectInReply;


protected:

	//Constructor
	CSerialPortModBus(void);
	~CSerialPortModBus(void);

public:

	//methods
	void SendSerialCommand(tSerialCommand TheData, BYTE TempLength);
	WORD MakeCRC(tSerialCommand TheData, BYTE TempLength);
	virtual bool SerialMessageComplete();
	virtual void ShowSerialData();  //data received
	virtual void RemoveMessageFromBuffer(); 

	// Generated message map functions
	//{{AFX_MSG(CSerialPortModBus)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSerialPortModBus)
	public:
	//}}AFX_VIRTUAL

	// Implementation
	//Destructor
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERIALPORTMODBUS_H__EBA785C3_08E8_11D5_AD6D_00500466E305__INCLUDED_)
