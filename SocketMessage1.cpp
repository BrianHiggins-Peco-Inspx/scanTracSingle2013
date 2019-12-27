//Source code for Side View ScanTrac
// SocketMessage.cpp : implementation of the CSocketMessage class

#include "stdafx.h"
#include "SocketMessage1.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSocketMessage

IMPLEMENT_DYNCREATE(CSocketMessage, CObject)

/////////////////////////////////////////////////////////////////////////////
// CSocketMessage construction/destruction

CSocketMessage::CSocketMessage()
{
	Init();
}

CSocketMessage::~CSocketMessage()
{
}

/////////////////////////////////////////////////////////////////////////////
// CSocketMessage Operations

void CSocketMessage::Init()
{
	vMessageFormat = 0;
	vMessageCommand = 0;
	vMessageText = _T("");
	vMessageList.RemoveAll();
}

/////////////////////////////////////////////////////////////////////////////
// CSocketMessage serialization

void CSocketMessage::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << vMessageFormat;
		ar << vMessageCommand;

		if (vMessageFormat == 1)
			for (BYTE TempLoop = 0; TempLoop < 10; TempLoop++)
				ar << vMessageDWordArray[TempLoop];

		if (vMessageFormat == 2)
			ar << vMessageText;

		if (vMessageFormat == 3)
		{
			for (BYTE TempLoop = 0; TempLoop < 5; TempLoop++)
				ar << vMessageDWordArray[TempLoop];
			ar << vMessageText;
		}

		if (vMessageFormat == 4)
			for (BYTE TempLoop = 0; TempLoop < 100; TempLoop++)
				ar << vMessageDWordArray[TempLoop];

	}
	else
	{
		ar >> vMessageFormat;
		ar >> vMessageCommand;

		if (vMessageFormat == 1)
			for (BYTE TempLoop = 0; TempLoop < 10; TempLoop++)
				ar >> vMessageDWordArray[TempLoop];

		if (vMessageFormat == 2)
			ar >> vMessageText;

		if (vMessageFormat == 3)
		{
			for (BYTE TempLoop = 0; TempLoop < 5; TempLoop++)
				ar >> vMessageDWordArray[TempLoop];
			ar >> vMessageText;
		}

		if (vMessageFormat == 4)
			for (BYTE TempLoop = 0; TempLoop < 100; TempLoop++)
				ar >> vMessageDWordArray[TempLoop];

	}
	if (vMessageFormat == 5)
		vMessageList.Serialize(ar);
}

/////////////////////////////////////////////////////////////////////////////
// CSocketMessage diagnostics

#ifdef _DEBUG
void CSocketMessage::AssertValid() const
{
	CObject::AssertValid();
}

void CSocketMessage::Dump(CDumpContext& dc) const
{
	CObject::Dump(dc);
}
#endif //_DEBUG
