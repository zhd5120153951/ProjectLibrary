// ************************************************************************
//  �ļ�����FreqFilter.cpp
//
//  ͼ��Ƶ���˲������⣺
//
//	ButterworthL()		- ������˹��ͨ�˲���
//	ButterworthH()		- ������˹��ͨ�˲���
//	MutualFilter()		- ����ʽ�����˲���
//	RetrorseFilter()	- ������˹��ͨ�˲��������˲�
//	WienerFilter()		- ��Լ���ָ���ά���˲�
//	PSE_Filter()		- ��Լ���ָ��Ĺ����׾����˲�
//	MinPower()			- ��Լ���ָ�����Сƽ���˲�
//
//*************************************************************************

#include "stdafx.h"
#include "dip_system.h"
#include "FreqFilter.h"
#include "FreqCalculate.h"
#include <complex>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFreqFilter

CFreqFilter::CFreqFilter()
{
}

CFreqFilter::CFreqFilter(CDIB* pDIB)
{
	m_pDIB = pDIB;
}

CFreqFilter::~CFreqFilter()
{
}

BEGIN_MESSAGE_MAP(CFreqFilter, CWnd)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFreqFilter message handlers

/*************************************************************************
 *
 * �������ƣ�
 *   ButterworthL()
 *
 * ����:
 *   HDIB	hDIB		- �������DIB
 *	 float  fD			- ��ͨ�˲���ֵ
 *
 * ����ֵ:
 *   void			    - �޷���ֵ
 *
 * ˵��:
 *   �ú�����ͼ����а�����˹��ͨ�˲�
 *
 ************************************************************************/
void CFreqFilter::ButterworthL(float fD)
{
	// ��ʱ����
	LONG	i;
	LONG	j;

	// ���и���Ҷ�任�Ŀ�Ⱥ͸߶ȣ�2�������η���
	LONG	w;
	LONG	h;
	
	int		wp;
	int		hp;

	BYTE* lpDIBBits = m_pDIB->FindAddress();
	
	CHECK_EDITABLE(m_pDIB->GetImage());
	
	BeginWaitCursor();
	
	LONG lWidth = m_pDIB->Width();
	
	LONG lHeight = m_pDIB->Height();

	LONG lLineBytes = m_pDIB->RowLen();

	// ����ֵ
	w = 1;
	h = 1;
	wp = 0;
	hp = 0;
	
	// ������и���Ҷ�任�Ŀ�Ⱥ͸߶ȣ�2�������η���
	while(w < lWidth)
	{
		w *= 2;
		wp++;
	}
	
	while(h < lHeight)
	{
		h *= 2;
		hp++;
	}

	CFreqCalculate clsFreqCalculate(m_pDIB);

	// ����Ƶ������
	complex<double> *FD = NULL, *TD = NULL;
	FD = new complex<double>[w * h * 3];
	TD = new complex<double>[w * h * 3];

	// ��
	for(i = 0; i < h; i++)
	{
		// ��
		for(j = 0; j < 3 * w; j++)
		{
			if(i < lHeight && j < lLineBytes)
			{
				// ��ȡʱ����ֵ
				BYTE Value = *((BYTE *)lpDIBBits + lLineBytes * i + j);
			
				// ʱ��ֵ
				TD[w * i * 3 + j] = complex<double>(Value, 0.0f);
			}
			else
			{
				// ������
				TD[w * i * 3 + j] = complex<double>(0.0f, 0.0f);
			}
		}
	}

	// ����Ƶ�׷��� ʱ��->Ƶ��
	if (clsFreqCalculate.Fourier(TD, lWidth, lHeight, FD) == FALSE)
		return;

	// �ͷ��ڴ�
	delete[] TD;

	// ��ǰƵ��
	float fDN;

	// ��ͼ����е�ͨ�˲�
	for(i = 0; i < h; i++)
	{
		for(j = 0; j < w * 3; j++)
		{
			// �������
			int k = (int)(j / 3);
			fDN = (float)sqrt(1.0* i * i + k * k);		

			// ���������˹��ͨ�˲������˲�
			FD[i * 3 * w + j] *= complex<double>(1 / (1 + 0.414 * (fDN / fD) * (fDN / fD)), 0.0f);
		}
	}

	// ����Ƶ�׷��� Ƶ��->ʱ��
	if (clsFreqCalculate.IFourier(lpDIBBits, lWidth, lHeight, FD) == FALSE)
		return;

	// �ͷ��ڴ�
	delete[] FD;

	EndWaitCursor();
}

/*************************************************************************
 *
 * �������ƣ�
 *   ButterworthH()
 *
 * ����:
 *   HDIB	hDIB		- �������DIB
 *	 float  fD			- ��ͨ�˲���ֵ
 *
 * ����ֵ:
 *   void			    - �޷���ֵ
 *
 * ˵��:
 *   �ú�����ͼ����а�����˹��ͨ�˲�
 *
 ************************************************************************/
void CFreqFilter::ButterworthH(float fD)
{
	// ��ʱ����
	LONG	i;
	LONG	j;

	// ���и���Ҷ�任�Ŀ�Ⱥ͸߶ȣ�2�������η���
	LONG	w;
	LONG	h;
	
	int		wp;
	int		hp;

	BYTE* lpDIBBits = m_pDIB->FindAddress();
	
	CHECK_EDITABLE(m_pDIB->GetImage());
	
	BeginWaitCursor();
	
	LONG lWidth = m_pDIB->Width();
	
	LONG lHeight = m_pDIB->Height();

	LONG lLineBytes = m_pDIB->RowLen();

	// ����ֵ
	w = 1;
	h = 1;
	wp = 0;
	hp = 0;
	
	// ������и���Ҷ�任�Ŀ�Ⱥ͸߶ȣ�2�������η���
	while(w < lWidth)
	{
		w *= 2;
		wp++;
	}
	
	while(h < lHeight)
	{
		h *= 2;
		hp++;
	}

	CFreqCalculate clsFreqCalculate(m_pDIB);

	// ����Ƶ������
	complex<double> *FD, *TD;
	FD = new complex<double>[w * h * 3];
	TD = new complex<double>[w * h * 3];

	// ��
	for(i = 0; i < h; i++)
	{
		// ��
		for(j = 0; j < 3 * w; j++)
		{
			if(i < lHeight && j < lLineBytes)
			{
				// ��ȡʱ����ֵ
				BYTE Value = *((BYTE *)lpDIBBits + lLineBytes * i + j);
			
				// ʱ��ֵ
				TD[w * i * 3 + j] = complex<double>(Value, 0.0f);
			}
			else
			{
				// ������
				TD[w * i * 3 + j] = complex<double>(0.0f, 0.0f);
			}
		}
	}

	// ����Ƶ�׷��� ʱ��->Ƶ��
	if (clsFreqCalculate.Fourier(TD, lWidth, lHeight, FD) == FALSE)
		return;

	// �ͷ��ڴ�
	delete[] TD;

	// ��ǰƵ��
	float fDN;

	// ��ͼ����и�ͨ�˲�
	for(i = 0; i < h; i++)
	{
		for(j = 0; j < w * 3; j++)
		{
			// �������
			int k = (int)(j / 3);
			fDN = (float)sqrt(1.0* i * i + k * k);		

			// ���������˹��ͨ�˲������˲�
			FD[i * 3 * w + j] *= complex<double>(1.0 / (1.0 + 0.414 * (fD / fDN) * (fD / fDN) + 0.5), 0.0f);
		}
	}

	// ����Ƶ�׷��� Ƶ��->ʱ��
	if (clsFreqCalculate.IFourier(lpDIBBits, lWidth, lHeight, FD) == FALSE)
		return;

	// �ͷ��ڴ�
	delete[] FD;

	EndWaitCursor();
}

/*************************************************************************
 *
 * �������ƣ�
 *   MutualFilter()
 *
 * ����:
 *   HDIB	hDIB		- �������DIB
 *	 CRect* pRect		- Ƶ�������������
 *	 int	nSum		- ����������Ŀ
 *
 * ����ֵ:
 *   void			    - �޷���ֵ
 *
 * ˵��:
 *   �ú�����ͼ����н���ʽ�����˲�
 *
 ************************************************************************/
void CFreqFilter::MutualFilter(CRect* pRect, int nSum)
{
	// ��ʱ����
	LONG	i;
	LONG	j;
	LONG	k;

	// ���и���Ҷ�任�Ŀ�Ⱥ͸߶ȣ�2�������η���
	LONG	w;
	LONG	h;
	
	int		wp;
	int		hp;

	BYTE* lpDIBBits = m_pDIB->FindAddress();
	
	CHECK_EDITABLE(m_pDIB->GetImage());
	
	BeginWaitCursor();
	
	LONG lWidth = m_pDIB->Width();
	
	LONG lHeight = m_pDIB->Height();

	LONG lLineBytes = m_pDIB->RowLen();

	// ����ֵ
	w = 1;
	h = 1;
	wp = 0;
	hp = 0;
	
	// ������и���Ҷ�任�Ŀ�Ⱥ͸߶ȣ�2�������η���
	while(w < lWidth)
	{
		w *= 2;
		wp++;
	}
	
	while(h < lHeight)
	{
		h *= 2;
		hp++;
	}

	CFreqCalculate clsFreqCalculate(m_pDIB);

	// ����Ƶ������
	complex<double> *FD, *TD;
	FD = new complex<double>[w * h * 3];
	TD = new complex<double>[w * h * 3];

	// ��
	for(i = 0; i < h; i++)
	{
		// ��
		for(j = 0; j < 3 * w; j++)
		{
			if(i < lHeight && j < lLineBytes)
			{
				// ��ȡʱ����ֵ
				BYTE Value = *((BYTE *)lpDIBBits + lLineBytes * i + j);
			
				// ʱ��ֵ
				TD[w * i * 3 + j] = complex<double>(Value, 0.0f);
			}
			else
			{
				// ������
				TD[w * i * 3 + j] = complex<double>(0.0f, 0.0f);
			}
		}
	}

	// ����Ƶ�׷��� ʱ��->Ƶ��
	if (clsFreqCalculate.Fourier(TD, lWidth, lHeight, FD) == FALSE)
		return;

	// �ͷ��ڴ�
	delete[] TD;

	// ����ѡ����ʵʩ�����˲�
	for(k = 0; k < nSum; k++)
	{
		// ������Ƶ������ƽ���ڵ�����
		pRect[k].top += (h - lHeight) / 2;
		pRect[k].bottom += (h - lHeight) / 2;
		pRect[k].left *= 3;
		pRect[k].right *= 3;
		pRect[k].left += (w - lWidth) * 3 / 2;
		pRect[k].right += (w - lWidth) * 3 / 2;

		// �ָ����任ǰ������λ��
		if (pRect[k].top < h / 2)
			pRect[k].top += h / 2;
		else
			pRect[k].top -= h / 2;

		if (pRect[k].bottom < h / 2)
			pRect[k].bottom += h / 2;
		else
			pRect[k].bottom -= h / 2;

		if (pRect[k].left < w * 3 /2)
			pRect[k].left += 3 * w / 2;
		else 
			pRect[k].left -= 3 * w / 2;

		if (pRect[k].right < w * 3 /2)
			pRect[k].right += 3 * w / 2;
		else 
			pRect[k].right -= 3 * w / 2;
	
		// �����ѡ������ͬһ����
		if (pRect[k].top < pRect[k].bottom && pRect[k].left < pRect[k].right)
		{
			// ��������д����˲�
			for (i = h - pRect[k].bottom; i < h - pRect[k].top; i++)
			{
				for (j = pRect[k].left * 3; j < pRect[k].right * 3; j++)
					FD[i * w * 3 + j] = complex<double>(0, 0);
			}
		}

		// �����ѡ����Ϊһ����������������
		if (pRect[k].top < pRect[k].bottom && pRect[k].left > pRect[k].right)
		{
			// ��������д����˲�
			for (i = h - pRect[k].bottom; i < h - pRect[k].top; i++)
			{
				// �Զ��������޽����˲�
				for (j = 0; j < pRect[k].right * 3; j++)
					FD[i * w * 3 + j] = complex<double>(0, 0);

				// ��һ�������޽����˲�
				for (j = pRect[k].left * 3; j < h; j++)
					FD[i * w * 3 + j] = complex<double>(0, 0);
			}
		}

		// �����ѡ����Ϊһ���Ļ����������
		if (pRect[k].top > pRect[k].bottom && pRect[k].left < pRect[k].right)
		{
			// ��������д����˲�
			// �����������޽����˲�
			for (i = 0; i < h - pRect[k].top; i++)
			{
				for (j = pRect[k].left * 3; j < pRect[k].right * 3; j++)
					FD[i * w * 3 + j] = complex<double>(0, 0);
			}
			// ��һ������޽����˲�
			for (i = h - pRect[k].bottom; i < h; i++)
			{
				for (j = pRect[k].left * 3; j < pRect[k].right * 3; j++)
					FD[i * w * 3 + j] = complex<double>(0, 0);
			}
		}

		// �����ѡ����Ϊһ����������������
		if (pRect[k].top > pRect[k].bottom && pRect[k].left > pRect[k].right)
		{
			// ��������д����˲�
			for (i = 0; i < h - pRect[k].top; i++)
			{
				// �������޽����˲�
				for (j = 0; j < pRect[k].right * 3; j++)
					FD[i * w * 3 + j] = complex<double>(0, 0);
				
				// �������޽����˲�
				for (j = pRect[k].left * 3; j < h; j++)
					FD[i * w * 3 + j] = complex<double>(0, 0);
			}
			for (i = h - pRect[k].bottom; i < h; i++)
			{
				// �Զ����޽����˲�
				for (j = 0; j < pRect[k].right * 3; j++)
					FD[i * w * 3 + j] = complex<double>(0, 0);

				// ��һ���޽����˲�
				for (j = pRect[k].left * 3; j < h; j++)
					FD[i * w * 3 + j] = complex<double>(0, 0);
			}
		}
	}

	// ����Ƶ�׷��� Ƶ��->ʱ��
	if (clsFreqCalculate.IFourier(lpDIBBits, lWidth, lHeight, FD) == FALSE)
		return;

	// �ͷ��ڴ�
	delete[] FD;

	EndWaitCursor();
}

/*************************************************************************
 *
 * �������ƣ�
 *   RetrorseFilter()
 *
 * ����:
 *   HDIB	hDIB		- �������DIB
 *	 float  fD			- ��ͨ�˲���ֵ
 *
 * ����ֵ:
 *   void			    - �޷���ֵ
 *
 * ˵��:
 *   �ú�����ͼ����а�����˹��ͨ�˲������˲�����
 *
 ************************************************************************/
void CFreqFilter::RetrorseFilter(float fD)
{
	// ��ʱ����
	LONG	i;
	LONG	j;

	// ���и���Ҷ�任�Ŀ�Ⱥ͸߶ȣ�2�������η���
	LONG	w;
	LONG	h;
	
	int		wp;
	int		hp;

	BYTE* lpDIBBits = m_pDIB->FindAddress();
	
	CHECK_EDITABLE(m_pDIB->GetImage());
	
	BeginWaitCursor();
	
	LONG lWidth = m_pDIB->Width();
	
	LONG lHeight = m_pDIB->Height();

	LONG lLineBytes = m_pDIB->RowLen();

	// ����ֵ
	w = 1;
	h = 1;
	wp = 0;
	hp = 0;
	
	// ������и���Ҷ�任�Ŀ�Ⱥ͸߶ȣ�2�������η���
	while(w < lWidth)
	{
		w *= 2;
		wp++;
	}
	
	while(h < lHeight)
	{
		h *= 2;
		hp++;
	}

	CFreqCalculate clsFreqCalculate(m_pDIB);

	// ����Ƶ������
	complex<double> *FD, *TD;
	FD = new complex<double>[w * h * 3];
	TD = new complex<double>[w * h * 3];

	// ��
	for(i = 0; i < h; i++)
	{
		// ��
		for(j = 0; j < 3 * w; j++)
		{
			if(i < lHeight && j < lLineBytes)
			{
				// ��ȡʱ����ֵ
				BYTE Value = *((BYTE *)lpDIBBits + lLineBytes * i + j);
			
				// ʱ��ֵ
				TD[w * i * 3 + j] = complex<double>(Value, 0.0f);
			}
			else
			{
				// ������
				TD[w * i * 3 + j] = complex<double>(0.0f, 0.0f);
			}
		}
	}

	// ����Ƶ�׷��� ʱ��->Ƶ��
	if (clsFreqCalculate.Fourier(TD, lWidth, lHeight, FD) == FALSE)
		return;

	// �ͷ��ڴ�
	delete[] TD;

	// ��ǰƵ��
	float fDN;

	// ��ͼ����е�ͨ�˲�
	for(i = 0; i < h; i++)
	{
		for(j = 0; j < w * 3; j++)
		{
			// �������
			int k = (int)(j / 3);
			fDN = (float)sqrt(1.0* i * i + k * k);		
			
			float fH = 1 / (1 + 0.414f * (fDN / fD) * (fDN / fD));

			// ���˲��ָ�
			if(fH > 0.25f)
				FD[i * 3 * w + j] /= complex<double>(fH, 0.0f);
			else
				FD[i * 3 * w + j] *= complex<double>(0.2f, 0.0f);
		}
	}

	// ����Ƶ�׷��� Ƶ��->ʱ��
	if (clsFreqCalculate.IFourier(lpDIBBits, lWidth, lHeight, FD) == FALSE)
		return;

	// �ͷ��ڴ�
	delete[] FD;

	EndWaitCursor();
}

/*************************************************************************
 *
 * �������ƣ�
 *   WienerFilter()
 *
 * ����:
 *   HDIB	hDIB		- �������DIB
 *	 float  fD			- ��ͨ�˲���ֵ
 *
 * ����ֵ:
 *   void			    - �޷���ֵ
 *
 * ˵��:
 *   �ú�����ͼ�������Լ���ָ���ά���˲�����
 *
 ************************************************************************/
void CFreqFilter::WienerFilter(float fD)
{
	// ��ʱ����
	LONG	i;
	LONG	j;

	// ���и���Ҷ�任�Ŀ�Ⱥ͸߶ȣ�2�������η���
	LONG	w;
	LONG	h;
	
	int		wp;
	int		hp;

	BYTE* lpDIBBits = m_pDIB->FindAddress();
	
	CHECK_EDITABLE(m_pDIB->GetImage());
	
	BeginWaitCursor();
	
	LONG lWidth = m_pDIB->Width();
	
	LONG lHeight = m_pDIB->Height();

	LONG lLineBytes = m_pDIB->RowLen();

	// ����ֵ
	w = 1;
	h = 1;
	wp = 0;
	hp = 0;
	
	// ������и���Ҷ�任�Ŀ�Ⱥ͸߶ȣ�2�������η���
	while(w < lWidth)
	{
		w *= 2;
		wp++;
	}
	
	while(h < lHeight)
	{
		h *= 2;
		hp++;
	}

	CFreqCalculate clsFreqCalculate(m_pDIB);

	// ����Ƶ������
	complex<double> *FD, *TD;
	FD = new complex<double>[w * h * 3];
	TD = new complex<double>[w * h * 3];

	// ��
	for(i = 0; i < h; i++)
	{
		// ��
		for(j = 0; j < 3 * w; j++)
		{
			if(i < lHeight && j < lLineBytes)
			{
				// ��ȡʱ����ֵ
				BYTE Value = *((BYTE *)lpDIBBits + lLineBytes * i + j);
			
				// ʱ��ֵ
				TD[w * i * 3 + j] = complex<double>(Value, 0.0f);
			}
			else
			{
				// ������
				TD[w * i * 3 + j] = complex<double>(0.0f, 0.0f);
			}
		}
	}

	// ����Ƶ�׷��� ʱ��->Ƶ��
	if (clsFreqCalculate.Fourier(TD, lWidth, lHeight, FD) == FALSE)
		return;

	// �ͷ��ڴ�
	delete[] TD;

	// ��ǰƵ��
	float fDN;

	// Ԥ���趨�ľ��鳣��
	float K = 0.05f;

	// ��ͼ����е�ͨ�˲�
	for(i = 0; i < h; i++)
	{
		for(j = 0; j < w * 3; j++)
		{
			// �������
			int k = (int)(j / 3);
			fDN = (float)sqrt(1.0* i * i + k * k);		

			float fH = 1 / (1 + 0.414f * (fDN / fD) * (fDN / fD));

			// ά���˲��ָ�
			FD[i * 3 * w + j] *= complex<double>(fH / (fH * fH + K), 0.0f);
		}
	}

	// ����Ƶ�׷��� Ƶ��->ʱ��
	if (clsFreqCalculate.IFourier(lpDIBBits, lWidth, lHeight, FD) == FALSE)
		return;

	// �ͷ��ڴ�
	delete[] FD;

	EndWaitCursor();
}

/*************************************************************************
 *
 * �������ƣ�
 *   PSE_Filter()
 *
 * ����:
 *   HDIB	hDIB		- �������DIB
 *	 float  fD			- ��ͨ�˲���ֵ
 *
 * ����ֵ:
 *   void			    - �޷���ֵ
 *
 * ˵��:
 *   �ú�����ͼ����й����׾����˲�
 *
 ************************************************************************/
void CFreqFilter::PSE_Filter(float fD)
{
	// ��ʱ����
	LONG	i;
	LONG	j;

	// ���и���Ҷ�任�Ŀ�Ⱥ͸߶ȣ�2�������η���
	LONG	w;
	LONG	h;
	
	int		wp;
	int		hp;

	BYTE* lpDIBBits = m_pDIB->FindAddress();
	
	CHECK_EDITABLE(m_pDIB->GetImage());
	
	BeginWaitCursor();
	
	LONG lWidth = m_pDIB->Width();
	
	LONG lHeight = m_pDIB->Height();

	LONG lLineBytes = m_pDIB->RowLen();

	// ����ֵ
	w = 1;
	h = 1;
	wp = 0;
	hp = 0;
	
	// ������и���Ҷ�任�Ŀ�Ⱥ͸߶ȣ�2�������η���
	while(w < lWidth)
	{
		w *= 2;
		wp++;
	}
	
	while(h < lHeight)
	{
		h *= 2;
		hp++;
	}

	CFreqCalculate clsFreqCalculate(m_pDIB);

	// ����Ƶ������
	complex<double> *FD, *TD;
	FD = new complex<double>[w * h * 3];
	TD = new complex<double>[w * h * 3];

	// ��
	for(i = 0; i < h; i++)
	{
		// ��
		for(j = 0; j < 3 * w; j++)
		{
			if(i < lHeight && j < lLineBytes)
			{
				// ��ȡʱ����ֵ
				BYTE Value = *((BYTE *)lpDIBBits + lLineBytes * i + j);
			
				// ʱ��ֵ
				TD[w * i * 3 + j] = complex<double>(Value, 0.0f);
			}
			else
			{
				// ������
				TD[w * i * 3 + j] = complex<double>(0.0f, 0.0f);
			}
		}
	}

	// ����Ƶ�׷��� ʱ��->Ƶ��
	if (clsFreqCalculate.Fourier(TD, lWidth, lHeight, FD) == FALSE)
		return;

	// �ͷ��ڴ�
	delete[] TD;

	// ��ǰƵ��
	float fDN;

	// Ԥ���趨�ľ��鳣��
	float K=0.05f;

	// ��ͼ����е�ͨ�˲�
	for(i = 0; i < h; i++)
	{
		for(j = 0; j < w * 3; j++)
		{
			// �������
			int k = (int)(j / 3);
			fDN = (float)sqrt(1.0* i * i + k * k);		
			
			float fH = 1 / (1 + 0.414f * (fDN / fD) * (fDN / fD));

			// �����׾����˲�
			FD[i * 3 * w + j] *= complex<double>(1 / sqrt(1.0*fH * fH + K), 0.0f);
		}
	}

	// ����Ƶ�׷��� Ƶ��->ʱ��
	if (clsFreqCalculate.IFourier(lpDIBBits, lWidth, lHeight, FD) == FALSE)
		return;

	// �ͷ��ڴ�
	delete[] FD;

	EndWaitCursor();
}

/*************************************************************************
 *
 * �������ƣ�
 *   MinPower()
 *
 * ����:
 *   HDIB	hDIB		- �������DIB
 *	 float  fD			- ��ͨ�˲���ֵ
 *
 * ����ֵ:
 *   void			    - �޷���ֵ
 *
 * ˵��:
 *   �ú�����ͼ�������Լ����Сƽ���˲��˲�
 *
 ************************************************************************/
void CFreqFilter::MinPower(float fD)
{
	// ��ʱ����
	LONG	i;
	LONG	j;

	// ���и���Ҷ�任�Ŀ�Ⱥ͸߶ȣ�2�������η���
	LONG	w;
	LONG	h;
	
	int		wp;
	int		hp;

	BYTE* lpDIBBits = m_pDIB->FindAddress();
	
	CHECK_EDITABLE(m_pDIB->GetImage());
	
	BeginWaitCursor();
	
	LONG lWidth = m_pDIB->Width();
	
	LONG lHeight = m_pDIB->Height();

	LONG lLineBytes = m_pDIB->RowLen();

	// ����ֵ
	w = 1;
	h = 1;
	wp = 0;
	hp = 0;
	
	// ������и���Ҷ�任�Ŀ�Ⱥ͸߶ȣ�2�������η���
	while(w < lWidth)
	{
		w *= 2;
		wp++;
	}
	
	while(h < lHeight)
	{
		h *= 2;
		hp++;
	}

	CFreqCalculate clsFreqCalculate(m_pDIB);

	// ����Ƶ������
	complex<double> *FD, *TD;
	FD = new complex<double>[w * h * 3];
	TD = new complex<double>[w * h * 3];

	// ��
	for(i = 0; i < h; i++)
	{
		// ��
		for(j = 0; j < 3 * w; j++)
		{
			if(i < lHeight && j < lLineBytes)
			{
				// ��ȡʱ����ֵ
				BYTE Value = *((BYTE *)lpDIBBits + lLineBytes * i + j);
			
				// ʱ��ֵ
				TD[w * i * 3 + j] = complex<double>(Value, 0.0f);
			}
			else
			{
				// ������
				TD[w * i * 3 + j] = complex<double>(0.0f, 0.0f);
			}
		}
	}

	// ����Ƶ�׷��� ʱ��->Ƶ��
	if (clsFreqCalculate.Fourier(TD, lWidth, lHeight, FD) == FALSE)
		return;

	// �ͷ��ڴ�
	delete[] TD;

	// ��ǰƵ��
	float fDN;

	// Ԥ���趨�ľ��鳣��
	float S = 0.00003f;

	// ��ͼ����е�ͨ�˲�
	for(i = 0; i < h; i++)
	{
		for(j = 0; j < w * 3; j++)
		{
			// �������
			int k = (int)(j / 3);
			fDN = (float)sqrt(1.0* i * i + k * k);		

			// ������Qʵ�ֵĸ�ͨ�˲�����ת�ƺ���P(u,v)
			float P = (float) (2 * PI * fDN);
			
			// ����H(u,v)
			float fH = 1 / (1 + 0.414f * (fDN / fD) * (fDN / fD));

			// ��Сƽ���˲��ָ�
			FD[i * 3 * w + j] *= complex<double>(fH / (fH * fH + S * P), 0.0f);
		}
	}

	// ����Ƶ�׷��� Ƶ��->ʱ��
	if (clsFreqCalculate.IFourier(lpDIBBits, lWidth, lHeight, FD) == FALSE)
		return;

	// �ͷ��ڴ�
	delete[] FD;

	EndWaitCursor();
}


