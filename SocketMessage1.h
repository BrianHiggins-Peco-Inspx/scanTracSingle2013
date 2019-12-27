//Source code for Side View ScanTrac
// SocketMessage1.h : interface of the CSocketMessage class

const BYTE cSocketGeneralText = 0;
const BYTE cSocketCloseConnection = 1;
const BYTE cSocketTestMessage = 2;
const BYTE cSocketGetStatus = 100;
const BYTE cSocketGetStatusNumbers = 101;
const BYTE cSocketGetCounts = 102;
const BYTE cSocketGetCountNumbers = 103;
const BYTE cSocketGetInspectionCounts = 104;
const BYTE cSocketGetInspectionCountNumbers = 105;
const BYTE cSocketGetCurrentProductName = 106;
const BYTE cSocketGetCurrentProductInspectionNames = 107;
const BYTE cSocketErrorMessage = 200;
const BYTE cSocketEventMessage = 201;
const BYTE cSocketRejectMessage = 202;
const BYTE cSocketRejectButDisabledMessage = 203;
const BYTE cSocketPrivateMessage = 204;
const BYTE cSocketChangedProductsMessage = 205;
const BYTE cSocketChangedStatusMessage = 206;
const BYTE cSocketResetAllCounter = 0x55;


class CSocketMessage : public CObject
{
protected:
	DECLARE_DYNCREATE(CSocketMessage)
public:
	CSocketMessage();

// Attributes
public:
	WORD vMessageFormat;
	WORD vMessageCommand;
	DWORD vMessageDWordArray[100];
	CString vMessageText;
	CStringList vMessageList;

// Operations
public:
	void Init();

// Implementation
public:
	virtual ~CSocketMessage();
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

/////////////////////////////////////////////////////////////////////////////
