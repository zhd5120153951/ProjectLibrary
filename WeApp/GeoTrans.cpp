// ************************************************************************
//  �ļ�����GeoTrans.cpp
//
//  ͼ�񼸺α任�����⣺
//
//  ZoomDIB()				- ����λͼ
//	Rotate()				- ��תλͼ
//	Rectinication()			- �ռ����ͼ��
//
// ************************************************************************

#include "stdafx.h"
#include "DIP_System.h"
#include "GeoTrans.h"
#include "SpaceFilter.h"
#include "math.h"
#include "DIP_SystemDoc.h"
#include "DIP_SystemView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGeoTrans

CGeoTrans::CGeoTrans()
{
}

CGeoTrans::CGeoTrans(CDIB* pDIB)
{
	m_pDIB = pDIB;
}

CGeoTrans::~CGeoTrans()
{
}

BEGIN_MESSAGE_MAP(CGeoTrans, CWnd)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGeoTrans message handlers

BOOL CGeoTrans::Zoom(int widthnew, int heightnew)
{
	if (m_pDIB->GetImage() == NULL) return FALSE;

	CMDIFrameWnd* pFrame = (CMDIFrameWnd *)AfxGetApp()->GetMainWnd();
	CMDIChildWnd* pChild = (CMDIChildWnd *)pFrame->GetActiveFrame();
	CDIP_SystemDoc* pDoc = (CDIP_SystemDoc *)pChild->GetActiveDocument();
	CDIP_SystemView* pView = (CDIP_SystemView *)pChild->GetActiveView();
	int m_nChannel = m_pDIB->Channel();
	LONG m_nImageWidth = m_pDIB->Width();
	LONG m_nImageHeight = m_pDIB->Height();
	LONG m_nRowLen = m_pDIB->RowLen();
	LONG m_nDatalen = m_pDIB->Length();
	BYTE* pBits = m_pDIB->FindAddress();

	//�������ź��ͼ��ߴ�����
	long WidthCur = widthnew;
	long HeightCur = heightnew;
	long RowlenCur = (widthnew*m_nChannel * 8 + 31) / 32 * 4;
	long DataLenCur = HeightCur * RowlenCur;
	//�������ű���
	float ratewidth = 1.0f * WidthCur / m_nImageWidth;
	float rateheight = 1.0f * HeightCur / m_nImageHeight;
	//����ռ����ڱ���Ŀ��ͼ�����ݼ��м���
	BYTE* dataStep1 = new BYTE[m_nImageHeight * RowlenCur];
	BYTE* dataDes = new BYTE[DataLenCur];

	//˫���β�ֵ��
	int r = 0, c = 0, k = 0;
	//�������ֵ
	for (r = 0; r < m_nImageHeight; r++)
	{
		BYTE* pDes = dataStep1 + r * RowlenCur;
		for (c = 0; c < WidthCur; c++)
		{
			int col = (int)(c / ratewidth);
			float fRateX = c / ratewidth - col;
			BYTE* pSrc = pBits + r * m_nRowLen + col * m_nChannel;
			if (col < m_nImageWidth - 1)
			{
				for (k = 0; k<m_nChannel; k++)
				{
					*(pDes + k) = (BYTE)(*(pSrc + k) * (1 - fRateX) + *(pSrc + m_nChannel + k) * fRateX);
				}
			}
			else
			{
				for (k = 0; k<m_nChannel; k++)
				{
					*(pDes + k) = (BYTE)*(pSrc + k);
				}
			}
			pDes += m_nChannel;
		}
	}
	//�������ֵ
	for (c = 0; c < WidthCur; c++)
	{
		BYTE* pDes = dataDes + c * m_nChannel;
		for (r = 0; r < HeightCur; r++)
		{
			int row = (int)(r / rateheight);
			float fRateY = r / rateheight - row;
			BYTE* pSrc = dataStep1 + row * RowlenCur + c * m_nChannel;
			if (row < m_nImageHeight - 1)
			{
				for (k = 0; k<m_nChannel; k++)
				{
					*(pDes + k) = (BYTE)(*(pSrc + k) * (1 - fRateY) + *(pSrc + RowlenCur + k) * fRateY);
				}
			}
			else
			{
				for (k = 0; k<m_nChannel; k++)
				{
					*(pDes + k) = (BYTE)*(pSrc + k);
				}
			}
			pDes += RowlenCur;
		}
	}
	CImage* pImage = new CImage;
	if (m_nChannel == 1)//�Ҷ�ͼ�����⴦��
	{
		pImage->Create(widthnew, heightnew, 8);
		RGBQUAD ColorTab[256];
		for (int i = 0; i<256; i++)
		{
			ColorTab[i].rgbBlue = ColorTab[i].rgbGreen = ColorTab[i].rgbRed = i;
		}
		pImage->SetColorTable(0, 256, ColorTab);
	}
	else pImage->Create(widthnew, heightnew, m_nChannel * 8);
	pBits = (BYTE *)pImage->GetBits() + (pImage->GetPitch()*(heightnew - 1));
	memcpy(pBits, dataDes, DataLenCur);
	// �޸�ͼ��ָ�룺����գ�Ȼ�����¸�ֵ
	pDoc->m_pDIB->Clear();
	m_pDIB->m_UndoStack.SetTop(pImage);
	m_pDIB->SetImage(pImage);
	pDoc->SetDocSize();
	pView->Invalidate(TRUE);
	delete[] dataStep1;
	delete[] dataDes;
	return TRUE;
}

BOOL CGeoTrans::ZoomDIB(float fZoomRatio)
{
	CMDIFrameWnd* pFrame = (CMDIFrameWnd *)AfxGetApp()->GetMainWnd();
	CMDIChildWnd* pChild = (CMDIChildWnd *)pFrame->GetActiveFrame();
	CDIP_SystemView* pView = (CDIP_SystemView *)pChild->GetActiveView();
	LONG m_nImageWidth = m_pDIB->Width();
	LONG m_nImageHeight = m_pDIB->Height();
	long WidthNew = m_nImageWidth * fZoomRatio;
	long HeightNew = m_nImageHeight * fZoomRatio;
	// ����ͼ��̫С����ͼ��̫��
	if (16 <= WidthNew && WidthNew <= 16384 && 16 <= HeightNew && HeightNew <= 16384 )
		Zoom(WidthNew, HeightNew);
	return TRUE;
}

/*************************************************************************
 *
 * �������ƣ�
 *   Rotate()
 *
 * ����:
 *   int	nAngle		- ���ű���
 *
 * ����ֵ:
 *   void			    - �޷���ֵ
 *
 * ˵��:
 *   �ú���������תDIBͼ��
 *
 ************************************************************************/
void CGeoTrans::Rotate(float fAngle)
{
	LONG i0;
	LONG j0;
	LONG i1;
	LONG j1;
	
	CHECK_EDITABLE(m_pDIB->GetImage());
	
	BeginWaitCursor();
	
	int nChannel = m_pDIB->Channel();
	LONG lWidth = m_pDIB->Width();
	LONG lHeight = m_pDIB->Height();
	LONG lLineBytes = m_pDIB->RowLen();
	BYTE* lpDIBBits = m_pDIB->FindAddress();

	// ��ת�ǶȵĻ���
	fAngle = (float)(fAngle * PI / 180);

	// �Ƕȵ�������
	float fSinAngle = (float)sin(fAngle);
	float fCosAngle = (float)cos(fAngle);

	// ������char��Ҳ������::strcpy()
	BYTE* temp;
	temp = new BYTE [lLineBytes * lHeight];

	// ���ƿհ����ݵ��м仺��
	for (i0 = 0; i0 < lLineBytes * lHeight; i0++)
		temp[i0] = 255;

	// �Ը����ؽ�����ת����
	for (i0 = 0; i0 < lHeight; i0 ++)
	{
		for (j0 = 0; j0 < lWidth; j0 ++)
		{
			// ������ת�������λ��
			j1 = (LONG)((j0 - lWidth / 2) * fCosAngle + (lHeight / 2 - i0) * fSinAngle + lWidth / 2 + 0.5);
			i1 = (LONG)((j0 - lWidth / 2) * fSinAngle - (lHeight / 2 - i0) * fCosAngle + lHeight / 2 + 0.5);

			// ��ԭʼ���ظ��Ƶ�Ŀ��λ��
			if (i1 >= 0 && i1 < lHeight && j1 >= 0 && j1 < lWidth)
			{
				temp[lLineBytes * (lHeight - i1 - 1) + j1 * nChannel] = *((BYTE *)lpDIBBits + lLineBytes * (lHeight - i0 - 1) + j0 * nChannel);
				temp[lLineBytes * (lHeight - i1 - 1) + j1 * nChannel + 1] = *((BYTE *)lpDIBBits + lLineBytes * (lHeight - i0 - 1) + j0 * nChannel + 1);
				temp[lLineBytes * (lHeight - i1 - 1) + j1 * nChannel + 2] = *((BYTE *)lpDIBBits + lLineBytes * (lHeight - i0 - 1) + j0 * nChannel + 2);
			}
		}
	}
	
	// �ش洦������DIB
	for(i0 = 0; i0 < lLineBytes * lHeight; i0++)
		*(lpDIBBits + i0) = temp[i0];

	// �ͷ��ڴ�
	delete[] temp;
	
	EndWaitCursor();
}

/*************************************************************************
 *
 * �������ƣ�
 *   Rectinication()
 *
 * ����:
 *   int	nAngle		- ����б�Ƕ�
 *
 * ����ֵ:
 *   void			    - �޷���ֵ
 *
 * ˵��:
 *   �ú��������ڿռ����DIBͼ��
 *
 ************************************************************************/
void CGeoTrans::Rectinication(float fAngle)
{
	LONG i0;
	LONG j0;
	LONG i1;
	LONG j1;
	
	CHECK_EDITABLE(m_pDIB->GetImage());
	
	BeginWaitCursor();
	
	int nChannel = m_pDIB->Channel();
	LONG lWidth = m_pDIB->Width();
	LONG lHeight = m_pDIB->Height();
	LONG lLineBytes = m_pDIB->RowLen();
	BYTE* lpDIBBits = m_pDIB->FindAddress();

	// ��ת�ǶȵĻ���
	fAngle = (float)(fAngle * PI / 180);

	// �Ƕȵ�������
	float fSinAngle = (float)sin(fAngle);
	float fCosAngle = (float)cos(fAngle);
	float fTgAngle = fSinAngle / fCosAngle;

	// ������char��Ҳ������::strcpy()
	BYTE* m_temp;
	m_temp = new BYTE [lLineBytes * lHeight];

	// ���ƿհ����ݵ��м仺��
	for (i0 = 0; i0 < lLineBytes * lHeight; i0++)
		m_temp[i0] = 255;

	// �ȶ�X������н�������
	for (i0 = 0; i0 < lHeight; i0 ++)
	{
		for (j0 = 0; j0 < lWidth; j0 ++)
		{
			// ����Уֱ�������λ��
			j1 = (LONG)(lWidth - (lWidth - j0) / fCosAngle + 0.5f);
			i1 = i0;			

			// ��ԭʼ���ظ��Ƶ�Ŀ��λ��
			if (i1 >= 0 && i1 < lHeight && j1 >= 0 && j1 < lWidth)
			{
				m_temp[lLineBytes * (lHeight - i1 - 1) + j1 * nChannel] = *((BYTE *)lpDIBBits + lLineBytes * (lHeight - i0 - 1) + j0 * nChannel);
				m_temp[lLineBytes * (lHeight - i1 - 1) + j1 * nChannel + 1] = *((BYTE *)lpDIBBits + lLineBytes * (lHeight - i0 - 1) + j0 * nChannel + 1);
				m_temp[lLineBytes * (lHeight - i1 - 1) + j1 * nChannel + 2] = *((BYTE *)lpDIBBits + lLineBytes * (lHeight - i0 - 1) + j0 * nChannel + 2);
			}
		}
	}

	// �ش洦������DIB
	for(i0 = 0; i0 < lLineBytes * lHeight; i0++)
		*(lpDIBBits + i0) = m_temp[i0];

	// ������ֵ�˲�����ƽ������
	CSpaceFilter clsSpaceFilter(m_pDIB);
	clsSpaceFilter.MedianFilter();

	// �ٶ�y������н�������
	for (i0 = 0; i0 < lHeight; i0 ++)
	{
		for (j0 = 0; j0 < lWidth; j0 ++)
		{
			// ����Уֱ�������λ��
			j1 = j0;
			if (i0 < lHeight / 2)
				i1 = (LONG)(i0 - (lHeight / 2 - i0) * fSinAngle * (lWidth - j1) / lWidth  + 0.5f);
			else
				i1 = (LONG)(i0 + (i0 - lHeight / 2) * fSinAngle * (lWidth - j1) / lWidth + 0.5f);

			// ��ԭʼ���ظ��Ƶ�Ŀ��λ��
			if (i1 >= 0 && i1 < lHeight && j1 >= 0 && j1 < lWidth)
			{
				m_temp[lLineBytes * (lHeight - i1 - 1) + j1 * nChannel] = *((BYTE *)lpDIBBits + lLineBytes * (lHeight - i0 - 1) + j0 * nChannel);
				m_temp[lLineBytes * (lHeight - i1 - 1) + j1 * nChannel + 1] = *((BYTE *)lpDIBBits + lLineBytes * (lHeight - i0 - 1) + j0 * nChannel + 1);
				m_temp[lLineBytes * (lHeight - i1 - 1) + j1 * nChannel + 2] = *((BYTE *)lpDIBBits + lLineBytes * (lHeight - i0 - 1) + j0 * nChannel + 2);
			}
		}
	}

	// �ش洦������DIB
	for(i0 = 0; i0 < lLineBytes * lHeight; i0++)
		*(lpDIBBits + i0) = m_temp[i0];

	// ������ֵ�˲�����ƽ������
	clsSpaceFilter.MedianFilter();

	// �ͷ��ڴ�
	delete[] m_temp;
	
	EndWaitCursor();
}


