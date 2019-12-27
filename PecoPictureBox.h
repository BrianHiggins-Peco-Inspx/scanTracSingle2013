#pragma once
#include "afxwin.h"

class CPecoPictureBox : public CStatic
{
public:
	CPecoPictureBox(void);
	~CPecoPictureBox(void);

private:

public:
	// Image file load methods
	void LoadJpgImageFile(CString fileName);
	void LoadGifImageFile(CString fileName);
	void LoadPngImageFile(CString fileName);

private:
	IStream* m_pStream;
	BOOL m_bIsPicLoaded;
	ULONG_PTR m_gdiplusToken;

	// File based image prep methods
	BOOL LoadFromFile(CString &szFilePath);
	BOOL LoadFromStream(IStream* piStream);
	BOOL LoadFromStream(BYTE* pData, size_t nSize);
	BOOL Load(CString &szFilePath);
	BOOL Load(IStream* piStream);
	BOOL Load(BYTE* pData, size_t nSize);

	// Windows GUI utility functions
	void FreeData();
	virtual void PreSubclassWindow();
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual BOOL OnEraseBkgnd(CDC* pDC);
};
