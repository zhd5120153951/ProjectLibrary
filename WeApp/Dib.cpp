// ************************************************************************
//  �ļ�����dib.cpp
//

#include "stdafx.h"
#include "dib.h"
#include <io.h>
#include <errno.h>

#include <math.h>
#include <direct.h>

//	Dib�ļ�ͷ��־���ַ���"BM"��дDIBʱ�õ��ó�����
#define DIB_HEADER_MARKER   ((WORD) ('M' << 8) | 'B')

// Ĭ�Ϲ��캯��
CDIB::CDIB()
{
	m_pImage = NULL;
}

// Ĭ����������
CDIB::~CDIB()
{
	// �Ѿ��ڶ�ջ�н�������
	// SAFE_DELETE(m_pImage);
}

// ��ȡͼ��ָ��
CImage*	CDIB::GetImage()
{
	return m_pImage;
}

// �޸�ͼ��ָ��ΪpImage
void	CDIB::SetImage(CImage* pImage)
{
	m_pImage = pImage;
}

////////////////////////////////////////////////////////////////////////////
//							����ģ�鿪ʼ								  //
////////////////////////////////////////////////////////////////////////////

/*************************************************************************
 *
 * �������ƣ�
 *   Paint()
 *
 * ����:
 *   HDC hDC            - �豸�������
 *   LPRECT lpDCRect    - ��������
 *   LPRECT lpDIBRect   - �����DIB����
 *
 * ����ֵ:
 *   BOOL               - ���Ƴɹ�����TRUE�����򷵻�FALSE��
 *
 * ˵��:
 *   ����DIB����
 *
 ************************************************************************/
BOOL CDIB::Paint(HDC hDC, LPRECT lpDCRect, LPRECT lpDIBRect)
{
	if (lpDCRect->right <= lpDCRect->left + 2 || lpDCRect->bottom <= lpDCRect->top + 2)
		return FALSE;
	return m_pImage->Draw(hDC, *lpDCRect, *lpDIBRect);
}

// ��ȡͼ���׵�ַ
BYTE* CDIB::FindAddress()
{
	if (m_pImage == NULL) return NULL;
	return (BYTE*)m_pImage->GetBits() + (m_pImage->GetPitch() * (m_pImage->GetHeight() - 1));
}

// ��ȡͼ����
DWORD CDIB::Width()
{
	return m_pImage->GetWidth();
}

// ��ȡͼ��߶�
DWORD CDIB::Height()
{
	return m_pImage->GetHeight();
}

// ��ȡͼ��ÿ���ֽ���
DWORD	CDIB::RowLen()
{
	return abs(m_pImage->GetPitch());
}

// ��ȡͼ���ͨ��(8λΪ1ͨ��)
WORD CDIB::Channel()
{
	return BitCount() / 8;
}

// ��ȡͼ���ܵ��ֽ���
DWORD CDIB::Length()
{
	return RowLen() * Height();
}

// ��ȡͼ����ɫλ��
WORD CDIB::BitCount()
{
	return m_pImage->GetBPP();
}

/*************************************************************************
 *
 * �������ƣ�
 *   Save()
 *
 * ����:
 *   Cstring sPath        - �ļ�����
 *
 * ����ֵ:
 *   BOOL               - �ɹ�����TRUE�����򷵻�FALSE
 *
 * ˵��:
 *   ��DIB���浽ָ���ļ���
 *
 *************************************************************************/
BOOL CDIB::Save(CString sPath)
{
	if (m_pImage == NULL) return FALSE;
	if (m_pImage->Save(sPath) != S_OK)
	{
		AfxMessageBox(L"����ͼƬʧ�ܣ�");
		return FALSE;
	}
	return TRUE;
}

/*************************************************************************
 *
 * �������ƣ�
 *   Read()
 *
 * ����:
 *   CFile& file        - Ҫ��ȡ���ļ��ļ�CFile
 *
 * ����ֵ:
 *   BOOL               - �ɹ�TRUE
 *
 * ˵��:
 *   ��ָ���ļ��ж�ȡDIB����
 *
 *************************************************************************/
BOOL CDIB::Read(CFile& file)
{
	CString path = file.GetFilePath();
	m_pImage = new CImage;
	m_pImage->Load(path);
	if (m_pImage->IsNull())
	{
		SAFE_DELETE(m_pImage);
		return FALSE;
	}
	// ��ԭʼͼ��ָ��ѹ��ջ��
	m_UndoStack.Push(m_pImage);
	return TRUE;
}

// ��ָ��·���ж�ȡͼ��
BOOL CDIB::Read(CString path)
{
	m_pImage = new CImage;
	m_pImage->Load(path);
	if (m_pImage->IsNull())
	{
		SAFE_DELETE(m_pImage);
		return FALSE;
	}
	// ��ԭʼͼ��ָ��ѹ��ջ��
	m_UndoStack.Push(m_pImage);
	// ���ͼ���Ƿ�Ϊ������λͼ
	if (BitCount() == 1)
	{
		AfxMessageBox(_T("������λͼ����ת��Ϊ8ɫͼ��"), MB_ICONINFORMATION | MB_OK);
		BinaryMap2Other(8);
	}
	return TRUE;
}

// ����ǰͼ�񸱱����롰��������ջ.ջ��ʼ���ǵ�ǰͼ��
void CDIB::PushCopy()
{
	CImage* pImage = MakeCopy();
	m_UndoStack.Push(pImage);
	SetImage(pImage);
}

// ���ص�ǰͼ��ĸ���ͼ�����ڼ��а�Ȳ���
CImage* CDIB::MakeCopy()
{
	int channel = Channel();
	int width = Width();
	int height = Height();
	long length = Length();
	// ��������ͼ��
	CImage* pImage = new CImage;
	if (channel == 1)//�Ҷ�ͼ�����⴦��
	{
		pImage->Create(width, height, 8);
		RGBQUAD ColorTab[256];
		for(int i = 0; i<256; i++)
		{
			ColorTab[i].rgbBlue = ColorTab[i].rgbGreen = ColorTab[i].rgbRed = i;
		}
		pImage->SetColorTable(0, 256, ColorTab);
	}
	else pImage->Create(width, height, channel*8);
	BYTE* pDst = (BYTE *)pImage->GetBits()+(pImage->GetPitch()*(height-1));
	BYTE* pSrc = (BYTE *)m_pImage->GetBits()+(m_pImage->GetPitch()*(height-1));
	memcpy(pDst, pSrc, length);
	return pImage;
}

// ����ǰͼ��ȡ������������ջ�����롰�ָ�����ջ
void CDIB::Undo()
{
	if (m_UndoStack.m_nSizeOfStack == 1) return;
	m_RedoStack.Push(m_UndoStack.Top());
	SetImage(m_UndoStack.GetTop());
}

// ����֮�󣬻ָ�������֮ǰ�Ĳ���
void	CDIB::Redo()
{
	if (m_RedoStack.m_nSizeOfStack == 0) return;
	m_UndoStack.Push(m_RedoStack.Top());
	SetImage(m_UndoStack.GetTop());
}

// RGBͼ��תΪ�Ҷ�ͼ��
BOOL	CDIB::RGB2Gray()
{
	LONG i, j;

	int nChannel = Channel();
	if (nChannel<3) return FALSE;

	LONG lWidth = Width();
	LONG lHeight = Height();
	LONG lLineBytes = RowLen();
	BYTE* lpDIBBits = FindAddress();

	// �Ը����ؽ��лҶ�ת��
	for (i = 0; i < lHeight; i++)
	{
		for (j = 0; j < lWidth; j++)
		{
			BYTE *pSrc = lpDIBBits + nChannel * j + i * lLineBytes;
			BYTE R = *(pSrc + 2);
			BYTE G = *(pSrc + 1);
			BYTE B = *pSrc;
			BYTE Y = (9798 * R + 19235 * G + 3735 * B) / 32768;
			*pSrc = *(pSrc + 1) = *(pSrc + 2) = Y;
		}
	}
	return TRUE;
}

// �ı�ͼ��ͨ����, bpp��Ŀ��ͼ���λɫ
BOOL	CDIB::ChangeBPP(UINT bpp)
{
	if(bpp != 8 && bpp != 24 && bpp != 32)
		return FALSE;

	int nBitCount = BitCount();
	if (nBitCount == bpp) return TRUE;
	switch (nBitCount)
	{
	case 8:
		if (bpp == 24)    Bpp8To24();
		else if(bpp == 32)Bpp8To32();
		break;
	case 16:
		break;
	case 24:
		if (bpp == 8)     Bpp24To8();
		else if(bpp == 32)Bpp24To32();
		break;
	case 32:
		if (bpp == 8)     Bpp32To8();
		else if(bpp == 24)Bpp32To24();
		break;
	default:
		break;
	}
	return TRUE;
}

// �ı������ͼ��ͨ����, bpp��Ŀ��ͼ���λɫ
BOOL	CDIB::BinaryMap2Other(int bpp)
{
	int nBitCount = BitCount();
	if (nBitCount != 1) return TRUE;
	switch (bpp)
	{
	case 8:
		Bpp1To8();
		break;
	case 16:
		break;
	case 24:
		Bpp1To24();
		break;
	case 32:
		Bpp1To32();
		break;
	default:
		break;
	}
	return TRUE;
}

// ��תͼ��Ĭ��Ϊ��ֱ��ת
void	CDIB::Flip(BOOL bVertical)
{
	if(bVertical)
	{

	}
	else
	{

	}
}

// ���ͼ��
void	CDIB::Clear()
{
	SAFE_DELETE(m_pImage);
}

// ����ͼ�����أ�Ĭ��ֵ �� int channel = 4, int width = 0, int height = 0
void	CDIB::SetBits(BYTE* pBits, int channel, int width, int height)
{
	// Ĭ�ϲ���
	int channelnew = Channel();
	int widthnew = Width();
	int heightnew = Height();

	// ������Ĭ�ϲ���
	if (width * height != 0)
	{
		widthnew = width;
		heightnew = height;
	}
	if (channel != 4)
		channelnew = channel;

	// ���ͼ��
	Clear();
	CImage *pNew = new CImage;
	pNew->Create(widthnew, heightnew, channel * 8);
	BYTE* pDst = (BYTE *)pNew->GetBits()+(pNew->GetPitch()*(heightnew - 1));
	memcpy(pDst, pBits, widthnew * heightnew * channel);
	m_UndoStack.SetTop(pNew);
	SetImage(pNew);
}

// 8λɫת24λɫ
void	CDIB::Bpp8To24()
{
	LONG lWidth = Width();
	LONG lHeight = Height();
	int nChannel = 1, nChannelNew = 3;
	LONG lRowLen = RowLen(), lRowLenNew = WIDTHBYTES(24 * lWidth);

	CImage *pNew = new CImage;
	pNew->Create(lWidth, lHeight, 24);

	BYTE *pSrc = FindAddress();
	BYTE *pDst = (BYTE*)pNew->GetBits() + (pNew->GetPitch()*(lHeight - 1));
	for (int i = 0; i < lHeight; i++)
	{
		for (int j = 0; j < lWidth; j++)
		{
			BYTE *pSrcTemp = pSrc + nChannel * j + i * lRowLen;
			BYTE *pDstTemp = pDst + nChannelNew * j + i * lRowLenNew;
			*(pDstTemp + 2) = *(pDstTemp + 1) = *pDstTemp = *pSrcTemp;
		}
	}
	Clear();
	m_UndoStack.SetTop(pNew);
	SetImage(pNew);
}

// 8λɫת32λɫ
void	CDIB::Bpp8To32()
{
	LONG lWidth = Width();
	LONG lHeight = Height();
	int nChannel = 1, nChannelNew = 4;
	LONG lRowLen = RowLen(), lRowLenNew = WIDTHBYTES(32 * lWidth);

	CImage *pNew = new CImage;
	pNew->Create(lWidth, lHeight, 32);

	BYTE *pSrc = FindAddress();
	BYTE *pDst = (BYTE*)pNew->GetBits() + (pNew->GetPitch()*(lHeight - 1));
	for (int i = 0; i < lHeight; i++)
	{
		for (int j = 0; j < lWidth; j++)
		{
			BYTE *pSrcTemp = pSrc + nChannel * j + i * lRowLen;
			BYTE *pDstTemp = pDst + nChannelNew * j + i * lRowLenNew;
			*(pDstTemp + 2) = *(pDstTemp + 1) = *pDstTemp = *pSrcTemp;
		}
	}
	Clear();
	m_UndoStack.SetTop(pNew);
	SetImage(pNew);
}

// 24λɫת8λɫ
void	CDIB::Bpp24To8()
{
	LONG lWidth = Width();
	LONG lHeight = Height();
	int nChannel = 3, nChannelNew = 1;
	LONG lRowLen = RowLen(), lRowLenNew = WIDTHBYTES(8 * lWidth);

	CImage *pNew = new CImage;
	pNew->Create(lWidth, lHeight, 8);
	RGBQUAD ColorTab[256];
	for(int i = 0; i<256; i++)
	{
		ColorTab[i].rgbBlue = ColorTab[i].rgbGreen = ColorTab[i].rgbRed = i;
	}
	pNew->SetColorTable(0, 256, ColorTab);

	BYTE *pSrc = FindAddress();
	BYTE *pDst = (BYTE*)pNew->GetBits() + (pNew->GetPitch()*(lHeight - 1));
	for (int i = 0; i < lHeight; i++)
	{
		for (int j = 0; j < lWidth; j++)
		{
			BYTE *pSrcTemp = pSrc + nChannel * j + i * lRowLen;
			BYTE *pDstTemp = pDst + nChannelNew * j + i * lRowLenNew;
			BYTE R = *(pSrcTemp + 2);
			BYTE G = *(pSrcTemp + 1);
			BYTE B = *pSrcTemp;
			*pDstTemp = (9798 * R + 19235 * G + 3735 * B) / 32768;
		}
	}
	Clear();
	m_UndoStack.SetTop(pNew);
	SetImage(pNew);
}

// 24λɫת32λɫ
void	CDIB::Bpp24To32()
{
	LONG lWidth = Width();
	LONG lHeight = Height();
	int nChannel = 3, nChannelNew = 4;
	LONG lRowLen = RowLen(), lRowLenNew = WIDTHBYTES(32 * lWidth);

	CImage *pNew = new CImage;
	pNew->Create(lWidth, lHeight, 32);

	BYTE *pSrc = FindAddress();
	BYTE *pDst = (BYTE*)pNew->GetBits() + (pNew->GetPitch()*(lHeight - 1));
	for (int i = 0; i < lHeight; i++)
	{
		for (int j = 0; j < lWidth; j++)
		{
			BYTE *pSrcTemp = pSrc + nChannel * j + i * lRowLen;
			BYTE *pDstTemp = pDst + nChannelNew * j + i * lRowLenNew;
			*(pDstTemp + 2) = *(pSrcTemp + 2);
			*(pDstTemp + 1) = *(pSrcTemp + 1);
			*pDstTemp = *pSrcTemp;
		}
	}
	Clear();
	m_UndoStack.SetTop(pNew);
	SetImage(pNew);
}

// 32λɫת8λɫ
void	CDIB::Bpp32To8()
{
	LONG lWidth = Width();
	LONG lHeight = Height();
	int nChannel = 4, nChannelNew = 1;
	LONG lRowLen = RowLen(), lRowLenNew = WIDTHBYTES(8 * lWidth);

	CImage *pNew = new CImage;
	pNew->Create(lWidth, lHeight, 8);
	RGBQUAD ColorTab[256];
	for(int i = 0; i<256; i++)
	{
		ColorTab[i].rgbBlue = ColorTab[i].rgbGreen = ColorTab[i].rgbRed = i;
	}
	pNew->SetColorTable(0, 256, ColorTab);

	BYTE *pSrc = FindAddress();
	BYTE *pDst = (BYTE*)pNew->GetBits() + (pNew->GetPitch()*(lHeight - 1));
	for (int i = 0; i < lHeight; i++)
	{
		for (int j = 0; j < lWidth; j++)
		{
			BYTE *pSrcTemp = pSrc + nChannel * j + i * lRowLen;
			BYTE *pDstTemp = pDst + nChannelNew * j + i * lRowLenNew;
			BYTE R = *(pSrcTemp + 2);
			BYTE G = *(pSrcTemp + 1);
			BYTE B = *pSrcTemp;
			*pDstTemp = (9798 * R + 19235 * G + 3735 * B) / 32768;
		}
	}
	Clear();
	m_UndoStack.SetTop(pNew);
	SetImage(pNew);
}

// 32λɫת24λɫ
void	CDIB::Bpp32To24()
{
	LONG lWidth = Width();
	LONG lHeight = Height();
	int nChannel = 4, nChannelNew = 3;
	LONG lRowLen = RowLen(), lRowLenNew = WIDTHBYTES(24 * lWidth);

	CImage *pNew = new CImage;
	pNew->Create(lWidth, lHeight, 24);

	BYTE *pSrc = FindAddress();
	BYTE *pDst = (BYTE*)pNew->GetBits() + (pNew->GetPitch()*(lHeight - 1));
	for (int i = 0; i < lHeight; i++)
	{
		for (int j = 0; j < lWidth; j++)
		{
			BYTE *pSrcTemp = pSrc + nChannel * j + i * lRowLen;
			BYTE *pDstTemp = pDst + nChannelNew * j + i * lRowLenNew;
			*(pDstTemp + 2) = *(pSrcTemp + 2);
			*(pDstTemp + 1) = *(pSrcTemp + 1);
			*pDstTemp = *pSrcTemp;
		}
	}
	Clear();
	m_UndoStack.SetTop(pNew);
	SetImage(pNew);
}

// ��������ͼ��ת��Ϊ8λɫͼ��
void	CDIB::Bpp1To8()
{
	LONG lWidth = Width();
	LONG lHeight = Height();
	int nChannelNew = 1;
	LONG lRowLen = RowLen(), lRowLenNew = WIDTHBYTES(8 * lWidth);

	CImage *pNew = new CImage;
	pNew->Create(lWidth, lHeight, 8);
	RGBQUAD ColorTab[256];
	for(int i = 0; i<256; i++)
	{
		ColorTab[i].rgbBlue = ColorTab[i].rgbGreen = ColorTab[i].rgbRed = i;
	}
	pNew->SetColorTable(0, 256, ColorTab);

	BYTE *pSrc = FindAddress();
	BYTE *pDst = (BYTE*)pNew->GetBits() + (pNew->GetPitch()*(lHeight - 1));
	for (int i = 0; i < lHeight; i++)
	{
		for (int j = 0; j < lWidth; j++)
		{
			int mod = j % 8;
			BYTE *pSrcTemp = pSrc + int(j / 8.f) + i * lRowLen;
			// *pSrcTemp�ĵ�mod������λ
			BYTE temp = ( *pSrcTemp>>(7 - mod) ) & 1;
			BYTE *pDstTemp = pDst + nChannelNew * j + i * lRowLenNew;
			*pDstTemp = 255 * temp;
		}
	}
	Clear();
	m_UndoStack.SetTop(pNew);
	SetImage(pNew);
}

// ��������ͼ��ת��Ϊ24λɫͼ��
void	CDIB::Bpp1To24()
{
	LONG lWidth = Width();
	LONG lHeight = Height();
	int nChannelNew = 3;
	LONG lRowLen = RowLen(), lRowLenNew = WIDTHBYTES(24 * lWidth);

	CImage *pNew = new CImage;
	pNew->Create(lWidth, lHeight, 24);

	BYTE *pSrc = FindAddress();
	BYTE *pDst = (BYTE*)pNew->GetBits() + (pNew->GetPitch()*(lHeight - 1));
	for (int i = 0; i < lHeight; i++)
	{
		for (int j = 0; j < lWidth; j++)
		{
			int mod = j % 8;
			BYTE *pSrcTemp = pSrc + int(j / 8.f) + i * lRowLen;
			// *pSrcTemp�ĵ�mod������λ
			BYTE temp = ( *pSrcTemp>>(7 - mod) ) & 1;
			BYTE *pDstTemp = pDst + nChannelNew * j + i * lRowLenNew;
			*pDstTemp = *(pDstTemp + 1) = *(pDstTemp + 2) = 255 * temp;
		}
	}
	Clear();
	m_UndoStack.SetTop(pNew);
	SetImage(pNew);
}

// ��������ͼ��ת��Ϊ32λɫͼ��
void	CDIB::Bpp1To32()
{
	LONG lWidth = Width();
	LONG lHeight = Height();
	int nChannelNew = 4;
	LONG lRowLen = RowLen(), lRowLenNew = WIDTHBYTES(32 * lWidth);

	CImage *pNew = new CImage;
	pNew->Create(lWidth, lHeight, 32);

	BYTE *pSrc = FindAddress();
	BYTE *pDst = (BYTE*)pNew->GetBits() + (pNew->GetPitch()*(lHeight - 1));
	for (int i = 0; i < lHeight; i++)
	{
		for (int j = 0; j < lWidth; j++)
		{
			int mod = j % 8;
			BYTE *pSrcTemp = pSrc + int(j / 8.f) + i * lRowLen;
			// *pSrcTemp�ĵ�mod������λ
			BYTE temp = ( *pSrcTemp>>(7 - mod) ) & 1;
			BYTE *pDstTemp = pDst + nChannelNew * j + i * lRowLenNew;
			*pDstTemp = *(pDstTemp + 1) = *(pDstTemp + 2) = 255 * temp;
		}
	}
	Clear();
	m_UndoStack.SetTop(pNew);
	SetImage(pNew);
}

// ��ʾ��ջ����һ�����ͼ�����ݣ�������makecopy֮��ʹ�ã������Ժ�Ĳ����������
void	CDIB::ReLoad()
{
	int next = m_UndoStack.m_nTopOfStack - 1;
	if (next == -1)
		next = MAX_UNDO_NUMBER - 1;
	CImage* pTop = m_UndoStack.GetTop();
	CImage* pNext = m_UndoStack.m_Array[next];
	BYTE *pDst = (BYTE*)pTop->GetBits() + (pTop->GetPitch()*(pTop->GetHeight() - 1));
	BYTE *pSrc = (BYTE*)pNext->GetBits() + (pNext->GetPitch()*(pNext->GetHeight() - 1));
	long length = pTop->GetHeight() * abs(pTop->GetPitch());
	memcpy(pDst, pSrc, length);
}
/*************************************************************************
 *
 * �������ƣ�
 *   SetRgnColor()
 *
 * ����:	
 *	 CRect rect			- ���õľ�����
 *	 COLORREF col		- ������ɫ
 *
 * ����ֵ:
 *   �޷���ֵ
 *
 * ˵��:
 *   �ú�����ָ������ɫ���ָ��������
 *
 *************************************************************************/
void CDIB::SetRgnColor(CRect rect, COLORREF col)
{
	LONG i, j;
	BYTE* lpDIBBits = FindAddress();
	LONG lWidth = Width();
	LONG lHeight = Height();
	LONG lLineBytes = RowLen();

	// �����������ɫ���
	for (i = lHeight - rect.bottom; i < lHeight - rect.top; i++)
	{
		for (j = rect.left * 3; j < rect.right * 3; j++)
		{
			*(BYTE *)(lpDIBBits + lLineBytes * i + j) = GetBValue(col);
			j++;
			*(BYTE *)(lpDIBBits + lLineBytes * i + j) = GetGValue(col);
			j++;
			*(BYTE *)(lpDIBBits + lLineBytes * i + j) = GetRValue(col);
		}
	}
}

/*************************************************************************
 *
 * �������ƣ�
 *   SetCircleColor()
 *
 * ����:	
 *	 CRect rect			- ���õľ�����
 *	 COLORREF col		- ������ɫ
 *
 * ����ֵ:
 *   �޷���ֵ
 *
 * ˵��:
 *   �ú�����ָ������ɫ���ָ����Բ������
 *
 *************************************************************************/
void CDIB::SetCircleColor(CRect rect, COLORREF col)
{
	LONG i, j;
	int cx, cy, dr;
	
	LONG lWidth = Width();
	LONG lHeight = Height();
	LONG lLineBytes = RowLen();
	int nChannel = Channel();
	BYTE* lpDIBBits = FindAddress();

	// ��Բ�����������ɫ���
	for (i = lHeight - rect.bottom; i < lHeight - rect.top; i++)
	{
		for (j = rect.left; j < rect.right; j ++)
		{
			cx = abs(j - rect.left - 8);
			cy = abs(i - lHeight + rect.bottom - 8);
			dr = cx * cx + cy * cy;
			if (dr > 64 || i < 0 || i > lHeight || j < 0 || j > lWidth)
				continue;
			*(BYTE *)(lpDIBBits + lLineBytes * i + j * nChannel) = GetBValue(col);
			*(BYTE *)(lpDIBBits + lLineBytes * i + j * nChannel + 1) = GetGValue(col);
			*(BYTE *)(lpDIBBits + lLineBytes * i + j * nChannel + 2) = GetRValue(col);
		}
	}
}

/**********************************************************/
/***********************************************************
/***********************************************************
/***********************************************************
/***********************************************************
/***********************************************************
/***********************************************************
/***********************************************************
/***********************************************************
/***********************************************************
/**********************************************************/

////////////////////////////////////////////////////////////////////////////
//							����ģ�鿪ʼ								  //
////////////////////////////////////////////////////////////////////////////

/*************************************************************************
*
* �������ƣ�
*   PaintDIB()
*
* ����:
*   HDC hDC            - �豸�������
*   LPRECT lpDCRect    - ��������
*   HDIB hDIB          - ָ��DIB�����ָ��
*   LPRECT lpDIBRect   - �����DIB����
*   CPalette* pPal     - ��ɫ��ָ��
*
* ����ֵ:
*   BOOL               - ���Ƴɹ�����TRUE�����򷵻�FALSE��
*
* ˵��:
*   ����DIB����
*
************************************************************************/
BOOL CDIB::Paint(HDC hDC, LPRECT lpDCRect, HDIB hDIB, LPRECT lpDIBRect, CPalette* pPal)
{
	BYTE*    lpDIBINFOHEADER;             // BITMAPINFOHEADERָ��
	BYTE*    lpDIBBits;				      // DIB����ָ��
	BOOL	  bRet = FALSE;				  // �ɹ���־
	HPALETTE  hNewPal = NULL;			  // ��ǰ�ĵ�ɫ��
	HPALETTE  hOldPal = NULL;			  // ��ǰ�ĵ�ɫ��

	// �ж�DIB�����Ƿ�Ϊ��
	if (hDIB == NULL)
	{
		return FALSE;
	}

	// ����DIB
	lpDIBINFOHEADER = (LPBYTE) ::GlobalLock((HGLOBAL)hDIB);

	lpDIBBits = FindAddress(lpDIBINFOHEADER);

	// ��ȡDIB��ɫ�壬��ѡ��
	if (pPal != NULL)
	{
		hNewPal = (HPALETTE)pPal->m_hObject;

		// ѡ�е�ɫ��
		hOldPal = ::SelectPalette(hDC, hNewPal, TRUE);
	}

	// ������ʾģʽ
	::SetStretchBltMode(hDC, COLORONCOLOR);

	// �ж��ǵ���StretchDIBits()����SetDIBitsToDevice()������DIB����
	if ((RECTWIDTH(lpDCRect) == RECTWIDTH(lpDIBRect)) &&
		(RECTHEIGHT(lpDCRect) == RECTHEIGHT(lpDIBRect)))
	{
		// ԭʼ��С���������졣
		bRet = ::SetDIBitsToDevice(hDC,	// hDC
			lpDCRect->left,				// DestX
			lpDCRect->top,			    // DestY
			RECTWIDTH(lpDCRect),		// nDestWidth
			RECTHEIGHT(lpDCRect),		// nDestHeight
			lpDIBRect->left,			// SrcX
			(int)Height(lpDIBINFOHEADER) -
			lpDIBRect->top -
			RECTHEIGHT(lpDIBRect),		// SrcY
			0,							// nStartScan
			(WORD)Height(lpDIBINFOHEADER),  // nNumScans
			lpDIBBits,					// lpBits
			(LPBITMAPINFO)lpDIBINFOHEADER,  // lpBitsInfo
			DIB_RGB_COLORS);			// wUsage
	}
	else
	{
		// ��ԭʼ��С�����졣
		bRet = ::StretchDIBits(hDC,			// hDC
			lpDCRect->left,					// DestX
			lpDCRect->top,					// DestY
			RECTWIDTH(lpDCRect),			// nDestWidth
			RECTHEIGHT(lpDCRect),			// nDestHeight
			lpDIBRect->left,				// SrcX
			lpDIBRect->top,					// SrcY
			RECTWIDTH(lpDIBRect),			// wSrcWidth
			RECTHEIGHT(lpDIBRect),			// wSrcHeight
			lpDIBBits,						// lpBits
			(LPBITMAPINFO)lpDIBINFOHEADER,  // lpBitsInfo
			DIB_RGB_COLORS,					// wUsage
			SRCCOPY);						// dwROP
	}

	// �������
	::GlobalUnlock((HGLOBAL)hDIB);

	// �ָ���ǰ�ĵ�ɫ��
	if (hOldPal != NULL)
	{
		::SelectPalette(hDC, hOldPal, TRUE);
	}

	return bRet;
}

/*************************************************************************
*
* �������ƣ�
*   CreateDIBPalette()
*
* ����:
*   HDIB hDIB          - ָ��DIB�����ָ��
*   CPalette* pPal     - ��ɫ��ָ��
*
* ����ֵ:
*   BOOL               - �����ɹ�����TRUE�����򷵻�FALSE��
*
* ˵��:
*   ����DIB�����ɫ��
*
************************************************************************/
BOOL CDIB::CreatePalette(HDIB hDIB, CPalette* pPal)
{
	// ָ���߼���ɫ���ָ��
	LPLOGPALETTE lpPal;

	// �߼���ɫ��ľ��
	HANDLE hLogPal;

	// ��ɫ��ľ��
	HPALETTE hPal = NULL;

	int i;

	// ��ɫ���е���ɫ��Ŀ
	int nNumColors;

	// ָ��DIB��ָ��
	BYTE* lpbi;

	// ָ��BITMAPINFO�ṹ��ָ�루Win3.0��
	LPBITMAPINFO lpbmi;

	// ָ��BITMAPCOREINFO�ṹ��ָ��
	LPBITMAPCOREINFO lpbmc;

	// �����Ƿ���Win3.0 DIB�ı��
	BOOL bWinStyleDIB;

	// �������
	BOOL bRet = FALSE;

	// �ж�DIB�Ƿ�Ϊ��
	if (hDIB == NULL)
	{
		return FALSE;
	}

	// ����DIB
	lpbi = (LPBYTE) ::GlobalLock((HGLOBAL)hDIB);

	// ��ȡָ��BITMAPINFO�ṹ��ָ�루Win3.0��
	lpbmi = (LPBITMAPINFO)lpbi;

	// ��ȡָ��BITMAPCOREINFO�ṹ��ָ��
	lpbmc = (LPBITMAPCOREINFO)lpbi;

	// ��ȡDIB����ɫ���е���ɫ��Ŀ
	nNumColors = NumColors(lpbi);

	if (nNumColors != 0)
	{
		// ����Ϊ�߼���ɫ���ڴ�
		hLogPal = ::GlobalAlloc(GHND, sizeof(LOGPALETTE)
			+sizeof(PALETTEENTRY)
			* nNumColors);

		// ����ڴ治�㣬�˳�
		if (hLogPal == 0)
		{
			// �������
			::GlobalUnlock((HGLOBAL)hDIB);

			return FALSE;
		}

		lpPal = (LPLOGPALETTE) ::GlobalLock((HGLOBAL)hLogPal);

		// ���ð汾��
		lpPal->palVersion = PALVERSION;

		// ������ɫ��Ŀ
		lpPal->palNumEntries = (WORD)nNumColors;

		// �ж��Ƿ���WIN3.0��DIB
		bWinStyleDIB = IS_WIN30_DIB(lpbi);

		// ��ȡ��ɫ��
		for (i = 0; i < nNumColors; i++)
		{
			if (bWinStyleDIB)
			{
				// ��ȡ��ɫ����
				lpPal->palPalEntry[i].peRed = lpbmi->bmiColors[i].rgbRed;

				// ��ȡ��ɫ����
				lpPal->palPalEntry[i].peGreen = lpbmi->bmiColors[i].rgbGreen;

				// ��ȡ��ɫ����
				lpPal->palPalEntry[i].peBlue = lpbmi->bmiColors[i].rgbBlue;

				// ����λ
				lpPal->palPalEntry[i].peFlags = 0;
			}
			else
			{
				// ��ȡ��ɫ����
				lpPal->palPalEntry[i].peRed = lpbmc->bmciColors[i].rgbtRed;

				// ��ȡ��ɫ����
				lpPal->palPalEntry[i].peGreen = lpbmc->bmciColors[i].rgbtGreen;

				// ��ȡ��ɫ����
				lpPal->palPalEntry[i].peBlue = lpbmc->bmciColors[i].rgbtBlue;

				// ����λ
				lpPal->palPalEntry[i].peFlags = 0;
			}
		}

		// �����߼���ɫ�崴����ɫ�壬������ָ��
		bRet = pPal->CreatePalette(lpPal);

		// �������
		::GlobalUnlock((HGLOBAL)hLogPal);

		// �ͷ��߼���ɫ��
		::GlobalFree((HGLOBAL)hLogPal);
	}

	// �������
	::GlobalUnlock((HGLOBAL)hDIB);

	return bRet;
}

/*************************************************************************
*
* �������ƣ�
*   FindDIBBits()
*
* ����:
*   BYTE* lpbi        - ָ��DIB�����ָ��
*
* ����ֵ:
*   BYTE*             - ָ��DIBͼ��������ʼλ��
*
* ˵��:
*   ����DIBͼ��������ʼλ��
*
************************************************************************/
BYTE* CDIB::FindAddress(BYTE* lpbi)
{
	return (lpbi + *(LPDWORD)lpbi + PaletteSize(lpbi));
}

/*************************************************************************
*
* �������ƣ�
*   DIBWidth()
*
* ����:
*   BYTE* lpbi        - ָ��DIB�����ָ��
*
* ����ֵ:
*   DWORD              - DIBλ����
*
* ˵��:
*   ��ȡDIBλͼ�Ŀ��
*
************************************************************************/
DWORD CDIB::Width(BYTE* lpDIB)
{
	// ָ��BITMAPINFO�ṹ��ָ�루Win3.0��
	LPBITMAPINFOHEADER lpbmi;

	// ָ��BITMAPCOREINFO�ṹ��ָ��
	LPBITMAPCOREHEADER lpbmc;

	// ��ȡָ��
	lpbmi = (LPBITMAPINFOHEADER)lpDIB;
	lpbmc = (LPBITMAPCOREHEADER)lpDIB;

	if (IS_WIN30_DIB(lpDIB))
	{
		// ����Windows 3.0 DIB������lpbmi->biWidth
		return lpbmi->biWidth;
	}
	else
	{
		// ����������ʽ��DIB������lpbmc->bcWidth
		return (DWORD)lpbmc->bcWidth;
	}
}

/*************************************************************************
*
* �������ƣ�
*   DIBHeight()
*
* ����:
*   BYTE* lpDIB       - ָ��DIB�����ָ��
*
* ����ֵ:
*   DWORD              - DIBλͼ�߶�
*
* ˵��:
*   ��ȡDIBλͼ�߶�
*
************************************************************************/
DWORD CDIB::Height(BYTE* lpDIB)
{
	// ָ��BITMAPINFO�ṹ��ָ�루Win3.0��
	LPBITMAPINFOHEADER lpbmi;

	// ָ��BITMAPCOREINFO�ṹ��ָ��
	LPBITMAPCOREHEADER lpbmc;

	// ��ȡָ��
	lpbmi = (LPBITMAPINFOHEADER)lpDIB;
	lpbmc = (LPBITMAPCOREHEADER)lpDIB;

	if (IS_WIN30_DIB(lpDIB))
	{
		// ����Windows 3.0 DIB������lpbmi->biHeight
		return lpbmi->biHeight;
	}
	else
	{
		// ����������ʽ��DIB������lpbmc->bcHeight
		return (DWORD)lpbmc->bcHeight;
	}
}

/*************************************************************************
*
* �������ƣ�
*   PaletteSize()
*
* ����:
*   BYTE* lpbi        - ָ��DIB�����ָ��
*
* ����ֵ:
*   WORD               - DIB�е�ɫ��Ĵ�С
*
* ˵��:
*   ����DIB��ɫ���С
*
************************************************************************/
WORD CDIB::PaletteSize(BYTE* lpbi)
{
	// ����DIB�е�ɫ��Ĵ�С
	if (IS_WIN30_DIB(lpbi))
	{
		//������ɫ��Ŀ��RGBQUAD�Ĵ�С
		return (WORD)(NumColors(lpbi) * sizeof(RGBQUAD));
	}
	else
	{
		//������ɫ��Ŀ��RGBTRIPLE�Ĵ�С
		return (WORD)(NumColors(lpbi) * sizeof(RGBTRIPLE));
	}
}

/*************************************************************************
*
* �������ƣ�
*   DIBNumColors()
*
* ����:
*   BYTE* lpbi        - ָ��DIB�����ָ��
*
* ����ֵ:
*   WORD               - ���ص�ɫ������ɫ������
*
* ˵��:
*   ����DIB��ɫ����ɫ��Ŀ
*
************************************************************************/
WORD CDIB::NumColors(BYTE* lpbi)
{
	int nBitCount;

	// ����Windows��DIB, ʵ����ɫ����Ŀ���Ա����ص�λ��Ҫ�١�
	// ��������������򷵻�һ�����Ƶ���ֵ��

	// �ж��Ƿ���WIN3.0 DIB
	if (IS_WIN30_DIB(lpbi))
	{
		// ��ȡdwClrUsedֵ
		DWORD dwClrUsed = ((LPBITMAPINFOHEADER)lpbi)->biClrUsed;

		if (dwClrUsed != 0)
		{
			// ���dwClrUsed��ʵ���õ�����ɫ������Ϊ0��ֱ�ӷ��ظ�ֵ
			return (WORD)dwClrUsed;
		}
	}

	// ��ȡ���ص�λ��
	if (IS_WIN30_DIB(lpbi))
	{
		// ��ȡbiBitCountֵ
		nBitCount = ((LPBITMAPINFOHEADER)lpbi)->biBitCount;
	}
	else
	{
		// ��ȡbiBitCountֵ
		nBitCount = ((LPBITMAPCOREHEADER)lpbi)->bcBitCount;
	}

	// �������ص�λ��������ɫ��Ŀ
	switch (nBitCount)
	{
	case 1:
		return 2;

	case 4:
		return 16;

	case 8:
		return 256;
	default:
		return 0;
	}
}

/*************************************************************************
*
* �������ƣ�
*   DIBBitCount()
*
* ����:
*   BYTE* lpbi        - ָ��DIB�����ָ��
*
* ����ֵ:
*   WORD               - ���ص�ɫ������ɫ������
*
* ˵��:
*   �ú�������DIBBitCount��
*
************************************************************************/
WORD CDIB::BitCount(BYTE* lpbi)
{
	int nBitCount;

	// ��ȡ���ص�λ��
	if (IS_WIN30_DIB(lpbi))
	{
		// ��ȡbiBitCountֵ
		nBitCount = ((LPBITMAPINFOHEADER)lpbi)->biBitCount;
	}
	else
	{
		// ��ȡbiBitCountֵ
		nBitCount = ((LPBITMAPCOREHEADER)lpbi)->bcBitCount;
	}

	return nBitCount;
}

/*************************************************************************
*
* �������ƣ�
*   CopyHandle()
*
* ����:
*   HGLOBAL h          - Ҫ���Ƶ��ڴ�����
*
* ����ֵ:
*   HGLOBAL            - ���ƺ�����ڴ�����
*
* ˵��:
*   �����ڴ��
*
************************************************************************/
HGLOBAL CDIB::CopyHandle(HGLOBAL h)
{
	if (h == NULL)
		return NULL;

	// ��ȡָ���ڴ������С
	DWORD dwLen = ::GlobalSize((HGLOBAL)h);

	// �������ڴ�ռ�
	HGLOBAL hCopy = ::GlobalAlloc(GHND, dwLen);

	// �жϷ����Ƿ�ɹ�
	if (hCopy != NULL)
	{
		// ����
		void* lpCopy = ::GlobalLock((HGLOBAL)hCopy);
		void* lp = ::GlobalLock((HGLOBAL)h);

		// ����
		memcpy(lpCopy, lp, dwLen);

		// �������
		::GlobalUnlock(hCopy);
		::GlobalUnlock(h);
	}

	return hCopy;
}

/*************************************************************************
*
* �������ƣ�
*   SaveDIB()
*
* ����:
*   HDIB hDib          - Ҫ�����DIB
*   CFile& file        - �����ļ�CFile
*
* ����ֵ:
*   BOOL               - �ɹ�����TRUE�����򷵻�FALSE����CFileException
*
* ˵��:
*   ��DIB���浽ָ���ļ���
*
*************************************************************************/
BOOL CDIB::Save(HDIB hDib, CFile& file)
{
	// Bitmap�ļ�ͷ
	BITMAPFILEHEADER bmfHdr;

	// ָ��BITMAPINFOHEADER��ָ��
	LPBITMAPINFOHEADER lpBI;

	if (hDib == NULL)
	{
		// ���DIBΪ�գ�����FALSE
		return FALSE;
	}

	// ��ȡBITMAPINFO�ṹ��������
	lpBI = (LPBITMAPINFOHEADER) ::GlobalLock((HGLOBAL)hDib);

	if (lpBI == NULL)
	{
		// Ϊ�գ�����FALSE
		return FALSE;
	}

	// �ж��Ƿ���WIN3.0 DIB
	if (!IS_WIN30_DIB(lpBI))
	{
		// ��֧���������͵�DIB����

		// �������
		::GlobalUnlock((HGLOBAL)hDib);

		return FALSE;
	}

	// ����ļ�ͷ

	// �ļ�����"BM"
	bmfHdr.bfType = DIB_HEADER_MARKER;

	// ����DIB��Сʱ����򵥵ķ����ǵ���GlobalSize()����������ȫ���ڴ��С��
	// ����DIB�����Ĵ�С�������Ƕ༸���ֽڡ���������Ҫ����һ��DIB����ʵ��С��

	// �ļ�ͷ��С����ɫ���С
	// ��BITMAPINFOHEADER��BITMAPCOREHEADER�ṹ�ĵ�һ��DWORD���Ǹýṹ�Ĵ�С��
	DWORD dwDIBSize = *(LPDWORD)lpBI + PaletteSize((LPBYTE)lpBI);

	// ����ͼ���С
	if ((lpBI->biCompression == BI_RLE8) || (lpBI->biCompression == BI_RLE4))
	{
		// ����RLEλͼ��û�������С��ֻ������biSizeImage�ڵ�ֵ
		dwDIBSize += lpBI->biSizeImage;
	}
	else
	{
		// ��СΪWidth * Height
		DWORD dwBmBitsSize = WIDTHBYTES((lpBI->biWidth)*((DWORD)lpBI->biBitCount)) * lpBI->biHeight;

		// �����DIB�����Ĵ�С
		dwDIBSize += dwBmBitsSize;

		// ����biSizeImage���ܶ�BMP�ļ�ͷ��biSizeImage��ֵ�Ǵ���ģ�
		lpBI->biSizeImage = dwBmBitsSize;
	}

	// �����ļ���С��DIB��С��BITMAPFILEHEADER�ṹ��С
	bmfHdr.bfSize = dwDIBSize + sizeof(BITMAPFILEHEADER);

	// ����������
	bmfHdr.bfReserved1 = 0;
	bmfHdr.bfReserved2 = 0;

	// ����ƫ����bfOffBits�����Ĵ�СΪBitmap�ļ�ͷ��С��DIBͷ��С����ɫ���С
	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER)+lpBI->biSize + PaletteSize((LPBYTE)lpBI);

	// ����д�ļ�
	TRY
	{
		// д�ļ�ͷ
		file.Write((LPBYTE)&bmfHdr, sizeof(BITMAPFILEHEADER));

		// дDIBͷ������
		file.Write(lpBI, dwDIBSize);
	}
	CATCH(CFileException, e)
	{
		// �������
		::GlobalUnlock((HGLOBAL)hDib);

		// �׳��쳣
		THROW_LAST();
	}
	END_CATCH

		// �������
		::GlobalUnlock((HGLOBAL)hDib);

	return TRUE;
}

/*************************************************************************
*
* �������ƣ�
*   ReadDIBFile()
*
* ����:
*   CFile& file        - Ҫ��ȡ���ļ��ļ�CFile
*
* ����ֵ:
*   HDIB               - �ɹ�����DIB�ľ�������򷵻�NULL��
*
* ˵��:
*   ��ָ���ļ��ж�ȡDIB����
*
*************************************************************************/
HDIB CDIB::Read(CFile& file, BOOL bReserved)
{
	BITMAPFILEHEADER bmfHeader;
	HDIB hDIB;
	BYTE* lpDIB;

	// ��ȡDIB���ļ������ȣ��ֽڣ�
	DWORD dwBitsSize = (DWORD)file.GetLength();

	// ���Զ�ȡDIB�ļ�ͷ
	if (file.Read((LPBYTE)&bmfHeader, sizeof(bmfHeader)) != sizeof(bmfHeader))
	{
		// ��С���ԣ�����NULL��
		return NULL;
	}

	// �ж��Ƿ���DIB���󣬼��ͷ�����ֽ��Ƿ���"BM"
	if (bmfHeader.bfType != DIB_HEADER_MARKER)
	{
		// ��DIB���󣬷���NULL��
		return NULL;
	}

	// ΪDIB�����ڴ�
	hDIB = (HDIB) ::GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, dwBitsSize);
	if (hDIB == 0)
	{
		// �ڴ����ʧ�ܣ�����NULL��
		return NULL;
	}

	// ����
	lpDIB = (LPBYTE) ::GlobalLock((HGLOBAL)hDIB);

	// ������
	if (file.Read(lpDIB, dwBitsSize - sizeof(BITMAPFILEHEADER)) != dwBitsSize - sizeof(BITMAPFILEHEADER))
	{
		// ��С���ԡ�

		// �������
		::GlobalUnlock((HGLOBAL)hDIB);

		// �ͷ��ڴ�
		::GlobalFree((HGLOBAL)hDIB);

		return NULL;
	}

	// �������
	::GlobalUnlock((HGLOBAL)hDIB);

	return hDIB;
}