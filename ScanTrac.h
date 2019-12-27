//ScanTrac Side View Source File
// ScanTrac.h : main header file for the SCANTRAC application
//
#include "stdafx.h"
#pragma once

#if !defined(AFX_SCANTRAC_H__1C80CF19_CEB9_11D3_ABCC_00500466E305__INCLUDED_)
#define AFX_SCANTRAC_H__1C80CF19_CEB9_11D3_ABCC_00500466E305__INCLUDED_

#if _MSC_VER > 1000
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#define SetBit(IntIn,Bit) IntIn |= Bit 
#define ClrBit(IntIn,Bit) IntIn &= ~(Bit) 
#define CompareBit(IntIn,Bit) (IntIn & Bit)

#define cAirOutsideTemperature 1
#define cAirInsideTemperature 2
#define cSurfaceTemperature 4
#define cLiquidTemperature 8
#define cNoTemperatureSave 0

#define cMaxSerialLogDirectorySize 2 * 12884901888L		// for 12 GByte 12884901888L // for 512 KByte 428000L		//doubled 2/9/2017

#include "resource.h"		// main symbols
#include "Product.h"
#include "ScanTracDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CScanTracApp:
// See ScanTrac.cpp for the implementation of this class
//

//Put any Global constants Here

#ifdef CompileInHTTPClientOperationsGuardian
enum {cOGSevereMessageNone, cOGSevereMessageInterlockBroken, cOGSevereMessageXRayFault, cOGSevereMessageImageFault, cOGSevereMessageEjectorFault,
	cOGSevereMessageTemperatureFault, cOGSevereMessageComFault, cOGSevereMessagePowerFault};
#endif

enum {cF1Key,cF2Key,cF3Key,cF4Key,cF5Key,cSF1Key,cSF2Key,
			cSF3Key,cSF4Key,cSF5Key,cSF6Key,cSF7Key,cSF8Key,
			c0Key,c1Key,c2Key,c3Key,c4Key,c5Key,c6Key,c7Key,
			c8Key,c9Key};

enum {cWindowsUnknown, cWindowsNT, cWindowsXP, cWindows7};

enum {cPingResultNoReply, cPingResultNotSent, cPingResultReply}; 

enum {cPingThreadNull, cPingThreadSending, cPingThreadDone};

const BYTE cNormalKeys = 0;
const BYTE cAlphaKeys = 1;
//const BYTE cIntegerKeys = 2;
//const BYTE cNumericKeys = 3;
const BYTE cAllOKKeys = 4;
const BYTE cFunctionOnlyKeys = 5;
const BYTE cLowerAlphaKeys = 6;
const WORD cSlipArraySize = 1000;

//const WORD cKeySetMessage = 65000;
//const WORD cTestMessage = 65101;
const WORD cKillCalibrationTimerMessage = 65102;
const WORD cGotTestCommReplyMessage = 65103;
const WORD cSentCommTestMessage = 65104;
const WORD cBackupChangedMessage = 65105;
const WORD cSimulateOneContainerIn10MSMessage = 65106;
const WORD cCheckHVPSVoltageSoonMessage = 65107;
const WORD cCheckLearnQualityMessage = 65108;

#ifdef CompileInHTTPClientOperationsGuardian
	const WORD cOGSevereMessage = 65109;
#endif

const WORD cRFIDEASBadgeReadMessage = 65110;
const BYTE cPeriod = 46;
const BYTE cBackSpace = 8;
const char cSpace = 32;
const BYTE cOK = 13;
const BYTE cRightArrow = 39;
const BYTE cNothingKey = 0;
const BYTE cPageUp = 33;
const BYTE cPageDown = 34;

const BYTE cF1 = 118;
const BYTE cF2 = 111;
const BYTE cF3 = 109;
const BYTE cF4 = 106;
const BYTE cF5 = 107;
const BYTE cSF1 = 116;
const BYTE cSF2 = 120;
const BYTE cSF3 = 122;
const BYTE cSF4 = 117;
const BYTE cSF5 = 113;
const BYTE cSF6 = 115;
const BYTE cSF7 = 114;
const BYTE cSF8 = 119;
const BYTE c0 = 96;
const BYTE c1 = 97;
const BYTE c2 = 98;
const BYTE c3 = 99;
const BYTE c4 = 100;
const BYTE c5 = 101;
const BYTE c6 = 102;
const BYTE c7 = 103;
const BYTE c8 = 104;
const BYTE c9 = 105;
const BYTE cChar0 = 48;
const BYTE cChar1 = 49;
const BYTE cChar2 = 50;
const BYTE cChar3 = 51;
const BYTE cChar4 = 52;
const BYTE cChar5 = 53;
const BYTE cChar6 = 54;
const BYTE cChar7 = 55;
const BYTE cChar8 = 56;
const BYTE cChar9 = 57;
const BYTE cMaxKeys = 23;
/*

typedef  BYTE tKeyMap[cMaxKeys];

const tKeyMap vKey = {cF1,cF2,cF3,cF4,cF5,cSF1,cSF2,
	cSF3,cSF4,cSF5,cSF6,cSF7,cSF8,cChar0,cChar1,cChar2,cChar3,
	cChar4,cChar5,cChar6,cChar7,cChar8,cChar9};

const tKeyMap vAlphaKey = {cF1,cF2,cF3,cF4,cF5,cSF1,cSF2,
	cSF3,cSF4,cSF5,cSF6,cSF7,cSF8,c0,c1,c2,c3,
	c4,c5,c6,c7,c8,c9};

const tKeyMap vFunctionOnlyKey = {cF1,cF2,cF3,cF4,cF5,cSF1,cSF2,
	cSF3,cSF4,cSF5,cSF6,cSF7,cSF8,cNothingKey,cNothingKey,cNothingKey,cNothingKey,
	cNothingKey,cNothingKey,cNothingKey,cNothingKey,cNothingKey,cNothingKey};
*/
const WORD cMaxBeltOffset = 65500;

class CScanTracApp : public CWinApp
{
public:

	//variables
	//BYTE vCurrentKey[cMaxKeys];
	CScanTracDlg *dlg;

	//constructor
	CScanTracApp();

//void SetKeys(BYTE KeyType);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScanTracApp)
	public:
	virtual BOOL InitInstance();
	virtual void AddToRecentFileList(LPCTSTR lpszPathName);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
		virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CScanTracApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	BOOL m_bATLInited;
private:
	BOOL InitATL();
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCANTRAC_H__1C80CF19_CEB9_11D3_ABCC_00500466E305__INCLUDED_)
#include "ScanTrac_i.h"
