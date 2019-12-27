#include "StdAfx.h"
#include "PecoPictureBox.h"
#include <GdiPlus.h>

using namespace Gdiplus;

#ifdef SAFE_RELEASE
#undef SAFE_RELEASE
#endif
#define SAFE_RELEASE(x) do{\
							if((x) != NULL)\
							{\
								while((x)->Release() != 0);\
								(x) = NULL;\
							}\
						}while(0)

CPecoPictureBox::CPecoPictureBox() :CStatic()
{
	m_pStream = NULL;
	m_bIsPicLoaded = FALSE;
	m_gdiplusToken = 0;
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);
}

CPecoPictureBox::~CPecoPictureBox(void)
{
	FreeData();
	GdiplusShutdown(m_gdiplusToken);
}

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/// Public image file loading methods
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
void CPecoPictureBox::LoadJpgImageFile(CString fileName)
{
	Load(fileName);
}

void CPecoPictureBox::LoadGifImageFile(CString fileName)
{
	IStream* pStream = NULL;
	if (CreateStreamOnHGlobal(NULL, TRUE, &pStream) == S_OK)
	{
		CFile picFile;
		if (picFile.Open(fileName, CFile::modeRead | CFile::typeBinary))
		{
			BYTE pBuffer[1024];
			UINT uiRead = 0;
			while ((uiRead = picFile.Read(pBuffer, sizeof(pBuffer))) != 0)
			{
				ULONG ulWritten = 0;
				pStream->Write(pBuffer, uiRead, &ulWritten);
			}
			Load(pStream);
		}
		while (pStream->Release());
	}
}

void CPecoPictureBox::LoadPngImageFile(CString fileName)
{
	CFile picFile;
	if (picFile.Open(fileName, CFile::modeRead | CFile::typeBinary))
	{
		BYTE* pBuffer = new BYTE[(unsigned int)picFile.GetLength()];
		if (pBuffer != NULL)
		{
			picFile.Read(pBuffer, (UINT)picFile.GetLength());
			Load(pBuffer, (size_t)picFile.GetLength());
			delete pBuffer;
		}
	}
}
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/// Public image file loading methods
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

BOOL CPecoPictureBox::LoadFromStream(IStream *piStream)
{
	SetLastError(ERROR_SUCCESS);
	FreeData();
	if (piStream == NULL)
	{
		SetLastError(ERROR_INVALID_ADDRESS);
		return FALSE;
	}
	DWORD dwResult = CreateStreamOnHGlobal(NULL, TRUE, &m_pStream);
	if (dwResult != S_OK)
	{
		SetLastError(dwResult);
		return FALSE;
	}
	LARGE_INTEGER lInt;
	lInt.QuadPart = 0;
	piStream->Seek(lInt, STREAM_SEEK_SET, NULL);
	STATSTG statSTG;
	dwResult = piStream->Stat(&statSTG, STATFLAG_DEFAULT);
	if (dwResult != S_OK)
	{
		SetLastError(dwResult);
		SAFE_RELEASE(m_pStream);
		return FALSE;
	}
	piStream->CopyTo(m_pStream, statSTG.cbSize, NULL, NULL);
	m_bIsPicLoaded = TRUE;
	Invalidate();
	RedrawWindow();
	return TRUE;
}

BOOL CPecoPictureBox::LoadFromStream(BYTE* pData, size_t nSize)
{
	SetLastError(ERROR_SUCCESS);
	FreeData();
	DWORD dwResult = CreateStreamOnHGlobal(NULL, TRUE, &m_pStream);
	if (dwResult != S_OK)
	{
		SetLastError(dwResult);
		return FALSE;
	}
	dwResult = m_pStream->Write(pData, (ULONG)nSize, NULL);
	if (dwResult != S_OK)
	{
		SetLastError(dwResult);
		SAFE_RELEASE(m_pStream);
		return FALSE;
	}
	m_bIsPicLoaded = TRUE;
	Invalidate();
	RedrawWindow();
	return TRUE;
}

BOOL CPecoPictureBox::LoadFromFile(CString &szFilePath)
{
	SetLastError(ERROR_SUCCESS);
	FreeData();
	DWORD dwResult = CreateStreamOnHGlobal(NULL, TRUE, &m_pStream);
	if (dwResult != S_OK)
	{
		SetLastError(dwResult);
		return FALSE;
	}
	CFile cFile;
	CFileException cFileException;
	if (!cFile.Open(szFilePath, CStdioFile::modeRead | CStdioFile::typeBinary, &cFileException))
	{
		SetLastError(cFileException.m_lOsError);
		SAFE_RELEASE(m_pStream);
		return FALSE;
	}
	BYTE pBuffer[1024] = { 0 };
	while (UINT dwRead = cFile.Read(pBuffer, 1024))
	{
		dwResult = m_pStream->Write(pBuffer, dwRead, NULL);
		if (dwResult != S_OK)
		{
			SetLastError(dwResult);
			SAFE_RELEASE(m_pStream);
			cFile.Close();
			return FALSE;
		}
	}
	cFile.Close();
	m_bIsPicLoaded = TRUE;
	Invalidate();
	RedrawWindow();
	return TRUE;
}

BOOL CPecoPictureBox::Load(CString &szFilePath)
{
	return LoadFromFile(szFilePath);
}

BOOL CPecoPictureBox::Load(IStream* piStream)
{
	return LoadFromStream(piStream);
}

BOOL CPecoPictureBox::Load(BYTE* pData, size_t nSize)
{
	return LoadFromStream(pData, nSize);
}

void CPecoPictureBox::FreeData()
{
	m_bIsPicLoaded = FALSE;
	SAFE_RELEASE(m_pStream);
}

void CPecoPictureBox::PreSubclassWindow()
{
	CStatic::PreSubclassWindow();
	ModifyStyle(0, SS_OWNERDRAW);
}

void CPecoPictureBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if (m_bIsPicLoaded)
	{
		RECT rc;
		this->GetClientRect(&rc);
		Graphics graphics(lpDrawItemStruct->hDC);
		Image image(m_pStream);
		graphics.DrawImage(&image, (INT)rc.left, (INT)rc.top, (INT)(rc.right - rc.left), (INT)(rc.bottom - rc.top));
	}
}

BOOL CPecoPictureBox::OnEraseBkgnd(CDC *pDC)
{
	if (m_bIsPicLoaded)
	{
		RECT rc;
		this->GetClientRect(&rc);
		Graphics graphics(pDC->GetSafeHdc());
		LARGE_INTEGER liSeekPos;
		liSeekPos.QuadPart = 0;
		m_pStream->Seek(liSeekPos, STREAM_SEEK_SET, NULL);
		Image image(m_pStream);
		graphics.DrawImage(&image, (INT)rc.left, (INT)rc.top, (INT)(rc.right - rc.left), (INT)(rc.bottom - rc.top));
		return TRUE;
	}
	else
	{
		return CStatic::OnEraseBkgnd(pDC);
	}
}
