#pragma once
#include "SerialPortBase.h"

typedef BYTE tSerialResponse[30];

class CSerialPortRFIDEASBadgeReader :	public CSerialPortBase
{
	DECLARE_DYNCREATE(CSerialPortRFIDEASBadgeReader)
public:
	CSerialPortRFIDEASBadgeReader();
	~CSerialPortRFIDEASBadgeReader();

	CString vBadgeReadEmployeeCode;
	CString vBadgeReadFacilityCode;
	BYTE vBackGroundCount;

	virtual void DecodeMessage();
	virtual	void DoBackGroundWork();
	virtual bool SerialMessageComplete();
	virtual void RemoveMessageFromBuffer();
	virtual void ShowSerialData();
};

