#pragma once

#include "UndoStack.h"

// dib.h

// DIB���
DECLARE_HANDLE(HDIB);

// DIB����
#define PALVERSION   0x300

/* DIB�� */

// �ж��Ƿ���Win 3.0��DIB
#define IS_WIN30_DIB(lpbi)  ((*(LPDWORD)(lpbi)) == sizeof(BITMAPINFOHEADER))

// �����������Ŀ��
#define RECTWIDTH(lpRect)     ((lpRect)->right - (lpRect)->left)

// �����������ĸ߶�
#define RECTHEIGHT(lpRect)    ((lpRect)->bottom - (lpRect)->top)

// �ڼ���ͼ���Сʱ�����ù�ʽ��biSizeImage = biWidth' �� biHeight��
// ��biWidth'��������biWidth�������biWidth'������4������������ʾ
// ���ڻ����biWidth�ģ���4�������������WIDTHBYTES������������
// biWidth'
#define WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4)

class CDIB
{
public:
	CDIB();
	~CDIB();

	CImage*    m_pImage;										// ͼ��ָ��
	CUndoStack m_UndoStack;										// ��ջ:����
	CUndoStack m_RedoStack;										// ��ջ:�ָ�

	// ����ԭ��
	CImage*	GetImage();											// ��ȡͼ��ָ��
	void	SetImage(CImage*);									// �޸�ͼ��ָ��
	void	SetCircleColor(CRect rect, COLORREF col);			// ��ָ������ɫ���ָ����Բ������
	void	SetRgnColor(CRect rect, COLORREF col);				// ��ָ������ɫ���ָ��������
	BOOL	Paint(HDC hDC,LPRECT lpDCRect,LPRECT lpDIBRect);	// ��hDC�л���ͼ��
	BYTE*   FindAddress ();										// ��ȡͼ���׵�ַ
	DWORD   Width();											// ��ȡͼ����
	DWORD   Height();											// ��ȡͼ��߶�
	DWORD	RowLen();											// ��ȡÿ���ֽ�
	DWORD	Length();											// ��ȡ���ֽ���
	WORD	Channel();											// ��ȡͼ���ͨ��
	WORD	BitCount();											// ��ȡͼ����ɫλ��
	BOOL    Save(CString sPath);								// ���浽ָ���ļ���
	BOOL    Read(CFile& file);									// ��ȡDIB����
	BOOL	Read(CString path);									// ��·���ж�ȡͼ��
	void	PushCopy();											// ����"����"��ջ
	CImage* MakeCopy();											// ���ص�ǰͼ��ĸ���
	void	Undo();												// ȡ��"����"��ջ
	void	Redo();												// �ָ�֮ǰ�Ĳ���
	BOOL	RGB2Gray();											// ��ɫתΪ�Ҷ�ͼ��
	BOOL	ChangeBPP(UINT bpp);								// �ı�ͼ��ͨ����
	void	Flip(BOOL bVertical = TRUE);						// ��תͼ��
	void	Clear();											// ���ͼ��
	void	SetBits(BYTE* pBits, int channel = 4, 
		int width = 0, int height = 0);							// ��������
	void	ReLoad();											// ��ջ�����漸�����¼�������
	BOOL	BinaryMap2Other(int bpp);							// ������ͼ��ת��Ϊ����

private:
	void	Bpp8To24();											// 8λת24λ
	void	Bpp8To32();											// 8λת32λ
	void	Bpp24To8();											// 24λת8λ
	void	Bpp24To32();										// 24λת32λ
	void	Bpp32To8();											// 32λת8λ
	void	Bpp32To24();										// 32λת24λ
	void	Bpp1To8();											// ����ת8λ
	void	Bpp1To24();											// ����ת24λ
	void	Bpp1To32();											// ����ת32λ

public://����
	BOOL	Paint(HDC hDC, LPRECT lpDCRect, HDIB hDIB, LPRECT lpDIBRect, CPalette* pPal);
	BOOL    CreatePalette(HDIB hDIB, CPalette* cPal);
	BYTE*   FindAddress(BYTE*  lpbi);
	DWORD   Width(BYTE*  lpDIB);
	DWORD   Height(BYTE*  lpDIB);
	WORD    PaletteSize(BYTE*  lpbi);
	WORD    NumColors(BYTE*  lpbi);
	WORD	BitCount(BYTE*  lpbi);
	HGLOBAL CopyHandle(HGLOBAL h);
	BOOL    Save(HDIB hDib, CFile& file);
	HDIB    Read(CFile& file, BOOL bReserved);
};
