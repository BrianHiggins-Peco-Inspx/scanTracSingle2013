//ScanTrac Side View Source File
// Spellman100WattMonoBlock.h: implementation of the Spellman100WattMonoBlock class.

#if !defined(AFX_CheyneyMonoBlock_H__5C0738D1_40B0_11DA_B2BF_00500466E309__INCLUDED_)
#define AFX_CheyneyMonoBlock_H__5C0738D1_40B0_11DA_B2BF_00500466E309__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SerialPortBase.h"
#include "SerialPort.h"
#include "SpellmanDXMXRaySourcePort.h"
#include "Spellman100WattMonoBlock.h"

class CCheyneyMonoBlock : public CSpellman100WattMonoBlock
{
	DECLARE_DYNCREATE(CCheyneyMonoBlock)
	public:
	CCheyneyMonoBlock();
	~CCheyneyMonoBlock();

	virtual void DecodeMessage();
	virtual void SetXRayVoltage(double TempVoltage);
	virtual void SetXRayCurrent(double TempCurrent);
	virtual void CompleteInitialization();

	void SetMaximumVoltageSupported(double TempVoltage);
	void SetMaximumCurrentSupported(double TempCurrent);
};

#endif // !defined(AFX_CheyneyMonoBlock_H__5C0738D1_40B0_11DA_B2BF_00500466E309__INCLUDED_)
