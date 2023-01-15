// ************************************************************************
//  �ļ�����SpaceFilter.cpp
//
//  ͼ������˲��������⣺
//
//  MakeEmboss()			- ���񻯴���
//	PixelFilter()			- ��ͼ�����С����ģ���˲�
//	StencilFilter()			- ��ͼ�����ģ���˲�
//  MedianFilter()			- ��ͼ�������ֵ�˲���������ƽ���˲���
//	OverRun()				- ��ͼ����г���ģ���˲�����
//
// ************************************************************************

#include "stdafx.h"
#include "dip_system.h"
#include "SpaceFilter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MAX(a,b) ((a)>(b))?(a):(b) 
#define MIN(a,b) ((a)<(b))?(a):(b)
#define BOUND(c,i,j)  (MAX((i),MIN((c),(j))))

/////////////////////////////////////////////////////////////////////////////
// CSpaceFilter

CSpaceFilter::CSpaceFilter()
{
}

CSpaceFilter::CSpaceFilter(CDIB* pDIB)
{
	m_pDIB = pDIB;
}

CSpaceFilter::~CSpaceFilter()
{
}

BEGIN_MESSAGE_MAP(CSpaceFilter, CWnd)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSpaceFilter message handlers

/*************************************************************************
 *
 * �������ƣ�
 *   PixelFilter()
 *
 * ����:
 *   BYTE* lpDst			- Ŀ��ͼ������
 *   BYTE* lpSrc			- Դͼ������
 *   LPKERNEL lpKernel		- ָ��KERNEL�ṹ�����ָ��
 *   DWORD dwWidthBytes		- һ��ͼ����ռ�ֽ���
 *   BOOL bEmboss			- �Ƿ���Ҫ��ǿ����
 *
 * ����ֵ:
 *   �޷���ֵ
 *
 * ˵��:
 *   �ú�������ͼ�����С����ģ���˲�
 *
 ************************************************************************/
void CSpaceFilter::PixelFilter(BYTE* lpDst, BYTE* lpSrc, LPKERNEL lpKernel, DWORD dwWidthBytes, BOOL bEmboss)
{
	
	LONG   i, j;

	// ��ʱ����
	LONG temp, R, G, B;
	BYTE *lpTemp1, *lpTemp2;

	// ��ʼ��
	R = G = B = 0;

	// lpTemp1 = lpSrc + dwWidthBytes - 3
	lpTemp1 = lpSrc + dwWidthBytes - lpKernel->Dimention; 
	
	// ��С�������ģ������˲�
	for(i = 0; i < lpKernel->Dimention; i++)
	{
		lpTemp2 = lpTemp1;
		for(j = 0; j < lpKernel->Dimention; j++)
		{
			temp = lpKernel->Element[i][j];
			B += (*lpTemp2++) * temp;
			G += (*lpTemp2++) * temp;
			R += (*lpTemp2++) * temp;
		}
		lpTemp1 += dwWidthBytes;
	}

	// ȡģ�����
	temp = lpKernel->Divisor;

	if(temp != 1)
	{
		R /= temp;
		G /= temp;
		B /= temp;
	}

	// �縡�������轫����ɫ��ΪRGB(128,128,128)
	if(bEmboss)
	{
		R += 128;
		G += 128;
		B += 128;
	}

	// ��ֹ����
	R = BOUND(R,0,255);
	G = BOUND(G,0,255);
	B = BOUND(B,0,255);

	// ������������浽DIB
	*lpDst++ = (BYTE)B;
	*lpDst++ = (BYTE)G;
	*lpDst   = (BYTE)R;
}

/*************************************************************************
 *
 * �������ƣ�
 *   MakeEmboss()
 *
 * ����:
 *   int  nEmbossMode   - ��ȡ�ĸ���ģʽ
 *
 * ����ֵ:
 *   HDIB				- �µ�DIB
 *
 * ˵��:
 *   �ú�������ɫλͼ���и��񻯴���
 *
 ************************************************************************/
void CSpaceFilter::MakeEmboss(int nEmbossMode)
{
	
	LONG i, j, k;

	// ��ȡָ��KERNEL�ṹ�����ָ��
	LPKERNEL  lpKernel = &Emboss[nEmbossMode];
	
	CHECK_EDITABLE(m_pDIB->GetImage());
	
	BeginWaitCursor();

	LONG lWidth = m_pDIB->Width();
	LONG lHeight = m_pDIB->Height();
	LONG lLength = m_pDIB->Length();
	LONG lLineBytes = m_pDIB->RowLen();
	BYTE* lpDIBBits = (BYTE*)m_pDIB->FindAddress();
	BYTE* lpNewDIB = new BYTE[lLength];

	// ��ʱ����
    BYTE *lpSrcTemp, *lpDstTemp;
	BYTE *lpSrcPixel, *lpDstPixel;

	// ͼ���һ�е�ַ
	lpSrcTemp = lpDIBBits;
	lpDstTemp = lpNewDIB;

	// ������ͼ��ʵʩС��������˲�����
	for(i = 0; i< lHeight - 3; i++)
	{
		lpSrcPixel = lpSrcTemp;
		lpDstPixel = lpDstTemp;
		
		// ����ÿ�еĵ�һ������;
		for(k = 0; k < 3; k++)
		{
			*lpDstPixel = *lpSrcPixel;
			lpDstPixel++;
			lpSrcPixel++;
		}

		// ����С�����˲������Ը������ݽ��д���
		for(j = 0; j < lWidth - 2; j++)
		{
			PixelFilter(lpDstPixel, lpSrcPixel, lpKernel, lLineBytes, TRUE);
			lpSrcPixel += 3;
			lpDstPixel += 3;
		}

		// ����ÿ�е����һ������;
		for(k = 0; k < 3; k++)
		{
			*lpDstPixel = *lpSrcPixel;
			lpDstPixel++;
			lpSrcPixel++;
		}

		// ����ָ��
		lpSrcTemp += lLineBytes;
		lpDstTemp += lLineBytes;
	}

	// ����ͼ������һ��;
	memcpy(lpDstTemp, lpSrcTemp, lLineBytes);

	// �����滻ԭͼ��
	memcpy(lpDIBBits, lpNewDIB, lLength);

	delete[] lpNewDIB;

	EndWaitCursor();
}

/*************************************************************************
 *
 * �������ƣ�
 *   StencilFilter()
 *
 * ����:
 *	 LPKERNEL lpKernel	- ��ȡָ��KERNEL�ṹ�����ָ��
 *
 * ����ֵ:
 *	 �޷���ֵ
 *
 * ˵��:
 *   �ú�������ɫλͼ����ģ���˲�����
 *
 ************************************************************************/
void CSpaceFilter::StencilFilter(LPKERNEL  lpKernel)
{
	
	LONG i;
	LONG j;
	LONG k;
	LONG l;

	// ��ʱ����
	LONG R, G, B;
	
	CHECK_EDITABLE(m_pDIB->GetImage());
	
	BeginWaitCursor();
	
	int nChannel = m_pDIB->Channel();
	LONG lWidth = m_pDIB->Width();
	LONG lHeight = m_pDIB->Height();
	LONG lLineBytes = m_pDIB->RowLen();
	LONG lLength = m_pDIB->Length();
	BYTE* lpDIBBits = m_pDIB->FindAddress();

	// ���벢�����м仺��
	BYTE* temp = new BYTE[lLength];

	// ����ͼ�����ݵ��м仺��
	memcpy(temp, lpDIBBits, lLength);
	
	// ģ���˲�
	for (i = 0; i < lWidth; i++)			//������������i��
	{
		for (j = 0; j < lHeight; j++)		//������������j��
		{
			// ��������
			R = G = B = 0;

			// ����С����ģ���˲�
			for (k = i - (int)(lpKernel->Dimention / 2); k < i + (int)(lpKernel->Dimention / 2) + 1; k++)
			{
				for(l = j - (int)(lpKernel->Dimention / 2); l < j + (int)(lpKernel->Dimention / 2) + 1; l++)
				{
					// ��ֹ�ڴ����
					if (k >= 0 && l >= 0 && k < lWidth && l < lHeight)
					{
						BYTE TR = *((BYTE *)lpDIBBits + l * lLineBytes + k * nChannel);
						R += lpKernel->Element[k - i + (int)(lpKernel->Dimention / 2)][l - j + (int)(lpKernel->Dimention / 2)] * TR;

						BYTE TG = *((BYTE *)lpDIBBits + l * lLineBytes + k * nChannel + 1);
						G += lpKernel->Element[k - i + (int)(lpKernel->Dimention / 2)][l - j + (int)(lpKernel->Dimention / 2)] * TG;

						BYTE TB = *((BYTE *)lpDIBBits + l * lLineBytes + k * nChannel + 2);
						B += lpKernel->Element[k - i + (int)(lpKernel->Dimention / 2)][l - j + (int)(lpKernel->Dimention / 2)] * TB;
					}
				}
			}

			// ����ģ��ƽ��
			R /= lpKernel->Divisor;
			G /= lpKernel->Divisor;
			B /= lpKernel->Divisor;

			// �洢���������м仺��
			*(temp + j * lLineBytes + i * nChannel)     = (BYTE)R;
			*(temp + j * lLineBytes + i * nChannel + 1) = (BYTE)G;
			*(temp + j * lLineBytes + i * nChannel + 2) = (BYTE)B;
		}
	}

	// ��ת������м仺�����ݻش浽DIB
	memcpy(lpDIBBits, temp, lLength);

	delete [] temp;
	
	EndWaitCursor();
}

/*************************************************************************
 *
 * �������ƣ�
 *   MedianFilter()
 *
 * ����:
 *
 * ����ֵ:
 *   �޷���ֵ
 *
 * ˵��:
 *   �ú�������ɫλͼ������ֵ�˲�����
 *
 ************************************************************************/
void CSpaceFilter::MedianFilter()
{
	
	LONG i;
	LONG j;
	LONG k;
	LONG l;

	// ����ģ��
	BYTE mask_r[9];
	BYTE mask_g[9];
	BYTE mask_b[9];
	COLORREF mask[9];

	CHECK_EDITABLE(m_pDIB->GetImage());
	
	BeginWaitCursor();
	
	int nChannel = m_pDIB->Channel();
	LONG lWidth = m_pDIB->Width();
	LONG lHeight = m_pDIB->Height();
	LONG lLineBytes = m_pDIB->RowLen();
	LONG lLength = m_pDIB->Length();
	BYTE* lpDIBBits = m_pDIB->FindAddress();

	// ���벢�����м仺��
	BYTE* temp = new BYTE[lLength];

	// ����ͼ�����ݵ��м仺��
	memcpy(temp, lpDIBBits, lLength);

	// ģ���˲�
	for (i = 0; i < lWidth; i++)			//������������i��
	{
		for (j = 0; j < lHeight; j++)		//������������j��
		{
			// ����
			int	id = 0;

			// ����С����ģ���˲�
			for (k = i - 1; k < i + 2; k++)
			{
				for(l = j - 1; l < j + 2; l++)
				{
					// ��ֹ�ڴ����
					if (k >= 0 && l >= 0 && k < lWidth && l < lHeight)
					{
						mask_r[id] = *((BYTE *)lpDIBBits + l * lLineBytes + k * nChannel);
						mask_g[id] = *((BYTE *)lpDIBBits + l * lLineBytes + k * nChannel + 1);
						mask_b[id] = *((BYTE *)lpDIBBits + l * lLineBytes + k * nChannel + 2);
				
						mask[id] = RGB(mask_r[id], mask_g[id], mask_b[id]);
						id++;
					}
				}
			}
			
			// �м����
			BYTE T;
			
			// ð������
			for (k = 0; k < 8; k++)		
			{
				for (l = 8; l > k; l--)					
				{
					if (mask_r[l] < mask_r[l - 1])
					{
						T = mask_r[l];
						mask_r[l] = mask_r[l - 1];
						mask_r[l - 1] = T;
					}
					if (mask_g[l] < mask_g[l - 1])
					{
						T = mask_g[l];
						mask_g[l] = mask_g[l - 1];
						mask_g[l - 1] = T;
					}
					if (mask_b[l] < mask_b[l - 1])
					{
						T = mask_b[l];
						mask_b[l] = mask_b[l - 1];
						mask_b[l - 1] = T;
					}
				}
			}
			temp[j * lLineBytes + i * nChannel] = mask_r[4];
			temp[j * lLineBytes + i * nChannel + 1] = mask_g[4];
			temp[j * lLineBytes + i * nChannel + 2] = mask_b[4];
		}
	}

	// ��ת������м仺�����ݻش浽DIB
	memcpy(lpDIBBits, temp, lLength);
	
	delete [] temp;

	EndWaitCursor();
}

/*************************************************************************
 *
 * �������ƣ�
 *   OverRun()
 *
 * ����:
 *	 LPKERNEL lpKernel	- ��ȡָ��KERNEL�ṹ�����ָ��
 *   int T				- ���޷�ֵ
 *
 * ����ֵ:
 *	 �޷���ֵ
 *
 * ˵��:
 *   �ú���λͼ���г���ģ���˲�����
 *	
 ************************************************************************/
void CSpaceFilter::OverRun(LPKERNEL lpKernel, int T)
{
	
	LONG i;
	LONG j;
	LONG k;
	LONG l;

	// ��ʱ����
	LONG R, G, B;
	LONG MR, MG, MB;
	
	CHECK_EDITABLE(m_pDIB->GetImage());
	
	BeginWaitCursor();
	
	int nChannel = m_pDIB->Channel();
	LONG lWidth = m_pDIB->Width();
	LONG lHeight = m_pDIB->Height();
	LONG lLineBytes = m_pDIB->RowLen();
	LONG lLength = m_pDIB->Length();
	BYTE* lpDIBBits = m_pDIB->FindAddress();

	// ���벢�����м仺��
	BYTE* temp = new BYTE[lLength];

	// ����ͼ�����ݵ��м仺��
	memcpy(temp, lpDIBBits, lLength);

	// ģ���˲�
	for (i = 0; i < lWidth; i++)			//������������i��
	{
		for (j = 0; j < lHeight; j++)		//������������j��
		{
			// ��������
			R = G = B = 0;

			// ����С����ģ���˲�
			for (k = i - (int)(lpKernel->Dimention / 2); k < i + (int)(lpKernel->Dimention / 2) + 1; k++)
			{
				for(l = j - (int)(lpKernel->Dimention / 2); l < j + (int)(lpKernel->Dimention / 2) + 1; l++)
				{
					// ��ֹ�ڴ����
					if (k >= 0 && l >= 0 && k < lWidth && l < lHeight)
					{
						BYTE TR = *((BYTE *)lpDIBBits + l * lLineBytes + k * nChannel);
						BYTE TG = *((BYTE *)lpDIBBits + l * lLineBytes + k * nChannel + 1);
						BYTE TB = *((BYTE *)lpDIBBits + l * lLineBytes + k * nChannel + 2);
						
						if (k == i && l == j)
						{
							MR = TR;
							MG = TG;
							MB = TB;
						}
						
						R += lpKernel->Element[k - i + (int)(lpKernel->Dimention / 2)][l - j + (int)(lpKernel->Dimention / 2)] * TR;
						G += lpKernel->Element[k - i + (int)(lpKernel->Dimention / 2)][l - j + (int)(lpKernel->Dimention / 2)] * TG;
						B += lpKernel->Element[k - i + (int)(lpKernel->Dimention / 2)][l - j + (int)(lpKernel->Dimention / 2)] * TB;
					}
				}
			}
			
			// ����ģ��ƽ��
			R /= lpKernel->Divisor;
			G /= lpKernel->Divisor;
			B /= lpKernel->Divisor;
			
			// ���г������ж�
			if (abs(MR - R) > T)
				MR = R;
			if (abs(MG - G) > T)
				MG = G;
			if (abs(MB - B) > T)
				MB = B;

			// �洢���������м仺��
			*(temp + j * lLineBytes + i * nChannel)     = (BYTE)MR;
			*(temp + j * lLineBytes + i * nChannel + 1) = (BYTE)MG;
			*(temp + j * lLineBytes + i * nChannel + 2) = (BYTE)MB;
		}
	}

	// ��ת������м仺�����ݻش浽DIB
	memcpy(lpDIBBits, temp, lLength);

	delete [] temp;

	EndWaitCursor();
}