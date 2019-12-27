#pragma once
#include "afxwin.h"
class CBottomButton :
	public CButton
{
public:
	CBottomButton(void);
	~CBottomButton(void);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void PreSubclassWindow();

};

