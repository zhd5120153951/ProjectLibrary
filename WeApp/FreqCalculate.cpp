// ************************************************************************
//  �ļ�����FreqCalculate.cpp
//
//  ͼ�������任�����⣺
//
//	FFT()				- һά���ٸ���Ҷ�任
//  IFFT()				- һά���ٸ���Ҷ��任
//  Fourier()			- ��ά���ٸ���Ҷ�任
//  IFourier()			- ��ά���ٸ���Ҷ��任
//  DCT()				- һά������ɢ���ұ任
//  IDCT()				- һά������ɢ������任
//	FreqDCT()			- ��ά������ɢ���ұ任
//  IFreqDCT()			- ��ά������ɢ������任
//  WALSH()				- һά�ֶ�ʲ��������任
//  IWALSH()			- һά�ֶ�ʲ����������任
//	FreqWALSH()			- ��ά�ֶ�ʲ��������任
//	IFreqWALSH()		- ��ά�ֶ�ʲ����������任
//	DWT()				- ��ά�����С���ֽ�
//	IDWT()				- ��ά�����С���ع�
//	
//  DIBFourier()		- ͼ��ĸ���Ҷ�任
//  DIBDCT()			- ͼ�����ɢ���ұ任
//  DIBWalsh()			- ͼ����ֶ�ʲ��������任
//  DIBDWT()			- ͼ��Ķ�ά��ɢС���任
//
//*************************************************************************

#include "stdafx.h"
#include "dip_system.h"
#include "FreqCalculate.h"
#include "math.h"
#include <complex>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ������
#ifndef PI
#define PI 3.1415926535
#endif

/////////////////////////////////////////////////////////////////////////////
// CFreqCalculate

CFreqCalculate::CFreqCalculate()
{
}

// ��pDIB��ֵ���ڲ�����
CFreqCalculate::CFreqCalculate(CDIB* pDIB)
{
	m_pDIB = pDIB;
}

CFreqCalculate::~CFreqCalculate()
{
}

BEGIN_MESSAGE_MAP(CFreqCalculate, CWnd)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFreqCalculate message handlers

/*************************************************************************
 *
 * �������ƣ�
 *   FFT()
 *
 * ����:
 *   complex<double> * TD	- ָ��ʱ�������ָ��
 *   complex<double> * FD	- ָ��Ƶ�������ָ��
 *   r						��2������������������
 *
 * ����ֵ:
 *   �ޡ�
 *
 * ˵��:
 *   �ú�������ʵ�ֿ��ٸ���Ҷ�任��
 *
 ************************************************************************/
void CFreqCalculate::FFT(complex<double> * TD, complex<double> * FD, int r)
{
	
	LONG	i;
	LONG	j;
	LONG	k;
	
	// �м����
	int		p;
	
	// �Ƕ�
	double	angle;
	
	complex<double> *W,*X1,*X2,*X;
	
	// ���㸶��Ҷ�任����
	LONG N = 1 << r;
	
	// ������������洢��
	W  = new complex<double>[N / 2];
	X1 = new complex<double>[N];
	X2 = new complex<double>[N];
	
	// �����Ȩϵ��
	for(i = 0; i < N / 2; i++)
	{
		angle = -i * PI * 2 / N;
		W[i] = complex<double> (cos(angle), sin(angle));
	}
	
	// ��ʱ���д��X1
	memcpy(X1, TD, sizeof(complex<double>) * N);
	
	// ���õ����㷨���п��ٸ���Ҷ�任
	for(k = 0; k < r; k++)
	{
		for(j = 0; j < 1 << k; j++)
		{
			for(i = 0; i < 1<<(r - k -1); i++)
			{
				p = j * (1<<(r - k));
				X2[i + p] = X1[i + p] + X1[i + p + (int)(1<<(r - k -1))];
				X2[i + p + (int)(1<<(r - k -1))] = (X1[i + p] - X1[i + p + (int)(1<<(r - k -1))]) * W[i * (1<<k)];
			}
		}
		X  = X1;
		X1 = X2;
		X2 = X;
	}
	
	// ��������
	for(j = 0; j < N; j++)
	{
		p = 0;
		for(i = 0; i < r; i++)
		{
			if (j&(1<<i))
			{
				p+=1<<(r - i - 1);
			}
		}
		FD[j] = X1[p];
	}
	
	// �ͷ��ڴ�
	delete W;
	delete X1;
	delete X2;
}

/*************************************************************************
 *
 * �������ƣ�
 *   IFFT()
 *
 * ����:
 *   complex<double> * FD	- ָ��Ƶ��ֵ��ָ��
 *   complex<double> * TD	- ָ��ʱ��ֵ��ָ��
 *   r						��2������
 *
 * ����ֵ:
 *   �ޡ�
 *
 * ˵��:
 *   �ú�������ʵ�ֿ��ٸ���Ҷ��任��
 *
 ************************************************************************/
void CFreqCalculate::IFFT(complex<double> * FD, complex<double> * TD, int r)
{
	
	int		i;
	
	complex<double> *X;
	
	// ���㸶��Ҷ�任����
	LONG N = 1<<r;
	
	// ������������洢��
	X = new complex<double>[N];
	
	// ��Ƶ���д��X
	memcpy(X, FD, sizeof(complex<double>) * N);
	
	// ����
	for (i = 0; i < N; i++)
	{
		X[i] = complex<double> (X[i].real(), -X[i].imag());
	}
	
	// ���ÿ��ٸ���Ҷ�任
	FFT(X, TD, r);
	
	// ��ʱ���Ĺ���
	for (i = 0; i < N; i++)
	{
		TD[i] = complex<double> (TD[i].real() / N, -TD[i].imag() / N);
	}
	
	// �ͷ��ڴ�
	delete X;
}

/*************************************************************************
 *
 * �������ƣ�
 *   Fourier()
 *
 * ����:
 *   complex* TD		- �����ʱ������
 *	 LONG lWidth		- ͼ����
 *	 LONG lHeight		- ͼ��߶�
 *	 complex* FD		- �����Ƶ������
 *
 * ����ֵ:
 *   BOOL               - �ɹ�����TRUE�����򷵻�FALSE��
 *
 * ˵��:
 *   �ú������ж�ά���ٸ���Ҷ�任��
 *
 ************************************************************************/
BOOL CFreqCalculate::Fourier(complex<double> * TD, LONG lWidth, LONG lHeight, complex<double> * FD)
{
	
	LONG	i;
	LONG	j;
	LONG    k;

	BeginWaitCursor();
	
	// ���и���Ҷ�任�Ŀ�Ⱥ͸߶ȣ�2�������η���
	LONG w = 1;
	LONG h = 1;
	int wp = 0;
	int hp = 0;
	
	// ������и���Ҷ�任�Ŀ�Ⱥ͸߶ȣ�2�������η���
	while (w < lWidth)
	{
		w *= 2;
		wp++;
	}
	         
	while (h < lHeight)
	{
		h *= 2;
		hp++;
	}
		
	// �����ڴ�
	complex<double> *TempT, *TempF;
	TempT = new complex<double>[h];
	TempF = new complex<double>[h];
	
	// ��y������п��ٸ���Ҷ�任
	for (i = 0; i < w * 3; i++)
	{
		// ��ȡ����
		for (j = 0; j < h; j++)
			TempT[j] = TD[j * w * 3 + i];
		
		// һά���ٸ���Ҷ�任
		FFT(TempT, TempF, hp);

		// ����任���
		for (j = 0; j < h; j++)
			TD[j * w * 3 + i] = TempF[j];
	}
	
	// �ͷ��ڴ�
	delete TempT;
	delete TempF;

	// �����ڴ�
	TempT = new complex<double>[w];
	TempF = new complex<double>[w];
	
	// ��x������п��ٸ���Ҷ�任
	for (i = 0; i < h; i++)
	{
		for (k = 0; k < 3; k++)
		{
			// ��ȡ����
			for (j = 0; j < w; j++)
				TempT[j] = TD[i * w * 3 + j * 3 + k];

			// һά���ٸ���Ҷ�任
			FFT(TempT, TempF, wp);

			// ����任���
			for (j = 0; j < w; j++)
				FD[i * w * 3 + j * 3 + k] = TempF[j];
		}
	}

	// �ͷ��ڴ�
	delete TempT;
	delete TempF;

	return TRUE;
}

/*************************************************************************
 *
 * �������ƣ�
 *   IFourier()
 *
 * ����:
 *   BYTE* TD			- ���صĿ�������
 *   LONG lWidth		- ����ͼ��Ŀ��
 *	 LONG lHeight		- ����ͼ��ĸ߶�
 *	 complex* FD		- �����Ƶ������
 *
 * ����ֵ:
 *   BOOL               - �ɹ�����TRUE�����򷵻�FALSE��
 *
 * ˵��:
 *   �ú������ж�ά���ٸ���Ҷ��任��
 *
 ************************************************************************/
BOOL CFreqCalculate::IFourier(BYTE* TD, LONG lWidth, LONG lHeight, complex<double> * FD)
{
	
	LONG	i;
	LONG	j;
	LONG    k;

	BeginWaitCursor();
	
	// ���и���Ҷ�任�Ŀ�Ⱥ͸߶ȣ�2�������η���
	LONG w = 1;
	LONG h = 1;
	int wp = 0;
	int hp = 0;
	
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

	LONG lLineBytes = m_pDIB->RowLen();

	// �����ڴ�
	complex<double> *TempT, *TempF;
	TempT = new complex<double>[w];
	TempF = new complex<double>[w];
	
	// ��x������п��ٸ���Ҷ�任
	for (i = 0; i < h; i++)
	{
		for (k = 0; k < 3; k++)
		{
			// ��ȡ����
			for (j = 0; j < w; j++)
				TempF[j] = FD[i * w * 3 + j * 3 + k];

			// һά���ٸ���Ҷ�任
			IFFT(TempF, TempT, wp);

			// ����任���
			for (j = 0; j < w; j++)
				FD[i * w * 3 + j * 3 + k] = TempT[j];
		}
	}

	// �ͷ��ڴ�
	delete TempT;
	delete TempF;
	
	TempT = new complex<double>[h];
	TempF = new complex<double>[h];

	// ��y������п��ٸ���Ҷ�任
	for (i = 0; i < w * 3; i++)
	{
		// ��ȡ����
		for (j = 0; j < h; j++)
			TempF[j] = FD[j * w * 3 + i];
		
		// һά���ٸ���Ҷ�任
		IFFT(TempF, TempT, hp);

		// ����任���
		for (j = 0; j < h; j++)
			FD[j * w * 3 + i] = TempT[j];
	}
	
	// �ͷ��ڴ�
	delete TempT;
	delete TempF;

	for (i = 0; i < h; i++)
	{
		for (j = 0; j < w * 3; j++)
		{
			if (i < lHeight && j < lLineBytes)
				*(TD + i * lLineBytes + j) = (BYTE)FD[i * w * 3 + j].real();
		}
	}

	return TRUE;
}

/*************************************************************************
 *
 * �������ƣ�
 *   DIBFourier()
 *
 * ����:
 *   HDIB	hDIB		- �������DIB
 *
 * ����ֵ:
 *   BOOL               - �ɹ�����TRUE�����򷵻�FALSE��
 *
 * ˵��:
 *   �ú���������ͼ����и���Ҷ�任��
 *
 ************************************************************************/
BOOL CFreqCalculate::DIBFourier()
{
	// ָ��Դͼ���ָ��
	BYTE*	lpSrc;

	// �м����
	double	dTemp;
	LONG TI,TJ;
	
	LONG	i;
	LONG	j;
	
	BYTE* lpDIBBits = m_pDIB->FindAddress();
	
	if (m_pDIB->Channel() < 3)
	{
		MessageBox(L"���Ƚ���ת��Ϊ24λɫ(��32λɫ)���ٽ��д���", L"ϵͳ��ʾ" , MB_ICONINFORMATION | MB_OK);
		return FALSE;
	}

	BeginWaitCursor();
	
	LONG lWidth = m_pDIB->Width();
	
	LONG lHeight = m_pDIB->Height();

	LONG lLineBytes = m_pDIB->RowLen();

	// ���и���Ҷ�任�Ŀ�Ⱥ͸߶ȣ�2�������η���
	LONG w = 1;
	LONG h = 1;
	int wp = 0;
	int hp = 0;
	
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

	// �����ڴ�
	complex<double> *FD, *TD, *TempD;
	FD = new complex<double>[w * h * 3];
	TD = new complex<double>[w * h * 3];
	TempD =  new complex<double>[w * h * 3];
	
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

	// ����Ƶ�׷���
	if (Fourier(TD, lWidth, lHeight, FD) == FALSE)
		return FALSE;

	// �ͷ��ڴ�
	delete []TD;

	// ��ԭ�������ͼ������λ��
	for(i = 0; i < h; i++)
	{
		for(j = 0; j < 3 * w; j++)
		{
			if (i < h / 2)
				TI = i + h / 2;
			else
				TI = i - h / 2;

			if (j < w * 3 /2)
				TJ = j + 3 * w / 2;
			else 
				TJ = j - 3 * w / 2;

			// ����ת�����Ƶ��ͼ��
			TempD[i * w * 3 + j] = FD[TI * w * 3 + TJ];
		}
	}

	// ��
	for(i = (int)(h - lHeight) / 2; i < (int)(h + lHeight) / 2; i++)
	{
		// ��
		for(j = (int)(w * 3 - lLineBytes) / 2; j < (int)(w * 3 + lLineBytes) / 2; j += 3)
		{
			// ����Ƶ��
			dTemp = sqrt(1.0*TempD[w * 3 * i + j].real() * TempD[w * 3 * i + j].real() + 
				         TempD[w * 3 * i + j].imag() * TempD[w * 3 * i + j].imag()) / 100;

			// �ж��Ƿ񳬹�255
			if (dTemp > 255)
			{
				// ���ڳ����ģ�ֱ������Ϊ255
				dTemp = 255;
			}

			// ����Ϊԭͼ��С��Χ
			TI = i - (h - lHeight) / 2;
			TJ = j / 3 - (w - lWidth) / 2;
			
			// ��Ӧ����ָ��
			lpSrc = (BYTE*)lpDIBBits + lLineBytes * TI + TJ * 3;

			// ����Դͼ��
			* (lpSrc) = (BYTE) (dTemp);
			* (lpSrc + 1) = (BYTE) (dTemp);
			* (lpSrc + 2) = (BYTE) (dTemp);
		}
	}

	// ɾ����ʱ����
	delete []FD;
	delete []TempD;
	
	EndWaitCursor();
	
	return TRUE;
}

/*************************************************************************
 *
 * �������ƣ�
 *   DCT()
 *
 * ����:
 *   double * f				- ָ��ʱ��ֵ��ָ��
 *   double * F				- ָ��Ƶ��ֵ��ָ��
 *   r						��2������
 *
 * ����ֵ:
 *   �ޡ�
 *
 * ˵��:
 *   �ú�������ʵ��һά������ɢ���ұ任
 *
 ************************************************************************/
void CFreqCalculate::DCT(double *f, double *F, int r)
{
	
	int		i;
	
	// �м����
	double	dTemp;
	
	// ������ɢ���ұ任����
	LONG N = 1<<r;
	
	// ���벢�����ڴ�
	complex<double> *XIn;
	complex<double> *XOut;
	XIn = new complex<double>[N * 2];
	XOut = new complex<double>[N * 2];
	
	// ����ֵΪ0
	memset(XIn, 0, sizeof(complex<double>) * N * 2);
	memset(XOut, 0, sizeof(complex<double>) * N * 2);
	
	// ��ʱ���д������X
	for (i = 0; i < N; i++)
		XIn[i] = complex<double>(*(f + i), 0);
	
	// ���ÿ��ٸ���Ҷ�任
	FFT(XIn, XOut, r + 1);
	
	// ����ϵ��
	dTemp = 1 / sqrt(1.0*N);
	
	// ��F[0]
	F[0] = XOut[0].real() * dTemp;
	
	dTemp *= sqrt(1.0*2);
	
	// ��F[u]
	for (i = 1; i < N; i++)
		*(F + i) = (XOut[i].real() * cos(i * PI / (N * 2)) + XOut[i].imag() * sin(i * PI / (N * 2))) * dTemp;
	
	// �ͷ��ڴ�
	delete[] XIn;
	delete[] XOut;
}

/*************************************************************************
 *
 * �������ƣ�
 *   IDCT()
 *
 * ����:
 *   double * F				- ָ��Ƶ��ֵ��ָ��
 *   double * f				- ָ��ʱ��ֵ��ָ��
 *   r						��2������
 *
 * ����ֵ:
 *   �ޡ�
 *
 * ˵��:
 *   �ú���ʵ��һά������ɢ������任
 *
 ************************************************************************/
void CFreqCalculate::IDCT(double *F, double *f, int r)
{
	
	int		i;
	
	// �м����
	double	dTemp, d0;
		
	// ������ɢ���ұ任����
	LONG N = 1<<r;
	
	// �����ڴ�
	complex<double> *XIn;
	complex<double> *XOut;
	XIn = new complex<double>[N * 2];
	XOut = new complex<double>[N * 2];

	// ����ֵΪ0
	memset(XIn, 0, sizeof(complex<double>) * N * 2);
	memset(XOut, 0, sizeof(complex<double>) * N * 2);
	
	// ��Ƶ��任���д������X
	for (i = 0; i < N; i++)
		XIn[i] = complex<double>(F[i] * cos(i * PI / (N * 2)), F[i] * sin(i * PI / (N * 2)));
	
	// ���ÿ��ٸ���Ҷ���任
	IFFT(XIn, XOut, r + 1);
	
	// ����ϵ��
	dTemp = sqrt(1.0*2.0 / N);
	d0 = (sqrt(1.0*1.0 / N) - dTemp) * F[0];
	
	// ����f(x)
	for (i = 0; i < N; i++)
		f[i] = d0 + XOut[i].real()* dTemp * 2 * N;
	
	// �ͷ��ڴ�
	delete[] XIn;
	delete[] XOut;
}

/*************************************************************************
 *
 * �������ƣ�
 *   FreqDCT()
 *
 * ����:
 *   double* f			- �����ʱ������
 *   double* F			- �����Ƶ������
 *	 LONG lWidth		- ͼ����
 *	 LONG lHeight		- ͼ��߶�
 *
 * ����ֵ:
 *   BOOL               - �ɹ�����TRUE�����򷵻�FALSE��
 *
 * ˵��:
 *   �ú������ж�ά������ɢ���ұ任��
 *
 ************************************************************************/
BOOL CFreqCalculate::FreqDCT(double *f, double *F, LONG lWidth, LONG lHeight)
{
	
	LONG	i;
	LONG	j;
	LONG    k;

	BeginWaitCursor();
	
	// ������ɢ���ұ任�Ŀ�Ⱥ͸߶ȣ�2�������η���
	LONG w = 1;
	LONG h = 1;
	int wp = 0;
	int hp = 0;
	
	// ���������ɢ���ұ任�Ŀ�Ⱥ͸߶ȣ�2�������η���
	while (w < lWidth)
	{
		w *= 2;
		wp++;
	}
	
	while (h < lHeight)
	{
		h *= 2;
		hp++;
	}
		
	// �����ڴ�
	double* TempIn = new double[h];
	double* TempOut = new double[h];
	
	// ��y���������ɢ���ұ任
	for (i = 0; i < w * 3; i++)
	{
		// ��ȡ����
		for (j = 0; j < h; j++)
			TempIn[j] = f[j * w * 3 + i];
		
		// һά������ɢ���ұ任
		DCT(TempIn, TempOut, hp);

		// ����任���
		for (j = 0; j < h; j++)
			f[j * w * 3 + i] = TempOut[j];
	}
	
	// �ͷ��ڴ�
	delete TempIn;
	delete TempOut;

	// �����ڴ�
	TempIn = new double[w];
	TempOut = new double[w];
	
	// ��x������п�����ɢ���ұ任
	for (i = 0; i < h; i++)
	{
		for (k = 0; k < 3; k++)
		{
			// ��ȡ����
			for (j = 0; j < w; j++)
				TempIn[j] = f[i * w * 3 + j * 3 + k];

			// һά������ɢ���ұ任
			DCT(TempIn, TempOut, wp);

			// ����任���
			for (j = 0; j < w; j++)
				F[i * w * 3 + j * 3 + k] = TempOut[j];
		}
	}

	// �ͷ��ڴ�
	delete TempIn;
	delete TempOut;

	return TRUE;
}

/*************************************************************************
 *
 * �������ƣ�
 *   IFreqDCT()
 *
 * ����:
 *   double* f			- �����ʱ������
 *   double* F			- �����Ƶ������
 *	 LONG lWidth		- ͼ����
 *	 LONG lHeight		- ͼ��߶�
 *
 * ����ֵ:
 *   BOOL               - �ɹ�����TRUE�����򷵻�FALSE��
 *
 * ˵��:
 *   �ú������ж�ά������ɢ������任��
 *
 ************************************************************************/
BOOL CFreqCalculate::IFreqDCT(double *f, double *F, LONG lWidth, LONG lHeight)
{
	
	LONG	i;
	LONG	j;
	LONG    k;

	BeginWaitCursor();
	
	// ������ɢ���ұ任�Ŀ�Ⱥ͸߶ȣ�2�������η���
	LONG w = 1;
	LONG h = 1;
	int wp = 0;
	int hp = 0;
	
	// ������и���Ҷ�任�Ŀ�Ⱥ͸߶ȣ�2�������η���
	while (w < lWidth)
	{
		w *= 2;
		wp++;
	}
	
	while (h < lHeight)
	{
		h *= 2;
		hp++;
	}

	LONG lLineBytes = m_pDIB->RowLen();

	// �����ڴ�
	double* TempIn = new double[w];
	double* TempOut = new double[w];
	
	// ��x������п��ٸ���Ҷ�任
	for (i = 0; i < h; i++)
	{
		for (k = 0; k < 3; k++)
		{
			// ��ȡ����
			for (j = 0; j < w; j++)
				TempIn[j] = F[i * w * 3 + j * 3 + k];

			// һά���ٸ���Ҷ�任
			IDCT(TempIn, TempOut, wp);

			// ����任���
			for (j = 0; j < w; j++)
				F[i * w * 3 + j * 3 + k] = TempOut[j];
		}
	}

	// �ͷ��ڴ�
	delete TempIn;
	delete TempOut;
	
	TempIn = new double[h];
	TempOut = new double[h];

	// ��y������п��ٸ���Ҷ�任
	for (i = 0; i < w * 3; i++)
	{
		// ��ȡ����
		for (j = 0; j < h; j++)
			TempIn[j] = F[j * w * 3 + i];
		
		// һά���ٸ���Ҷ�任
		IDCT(TempIn, TempOut, hp);

		// ����任���
		for (j = 0; j < h; j++)
			F[j * w * 3 + i] = TempOut[j];
	}
	
	// �ͷ��ڴ�
	delete TempIn;
	delete TempOut;
	
	for (i = 0; i < h; i++)
	{
		for (j = 0; j < w * 3; j++)
		{
			if (i < lHeight && j < lLineBytes)
				*(f + i * lLineBytes + j) = F[i * w * 3 + j];
		}
	}

	return TRUE;
}

/*************************************************************************
 *
 * �������ƣ�
 *   DIBDCT()
 *
 * ����:
 *   HDIB  hDIB		    - �������DIB
 *
 * ����ֵ:
 *   BOOL               - �ɹ�����TRUE�����򷵻�FALSE��
 *
 * ˵��:
 *   �ú���������ͼ�������ɢ���ұ任��
 *
 ************************************************************************/
BOOL CFreqCalculate::DIBDCT()
{
	// ������ɢ���ұ任�Ŀ�Ⱥ͸߶ȣ�2�������η���
	LONG	i;
	LONG	j;
	
	BYTE* lpDIBBits = m_pDIB->FindAddress();
	
	if (m_pDIB->Channel() < 3)
	{
		MessageBox(L"���Ƚ���ת��Ϊ24λɫ(��32λɫ)���ٽ��д���", L"ϵͳ��ʾ" , MB_ICONINFORMATION | MB_OK);
		return FALSE;
	}

	LONG lWidth = m_pDIB->Width();
	
	LONG lHeight = m_pDIB->Height();
	
	LONG lLineBytes = m_pDIB->RowLen();
	
	// ������ɢ���ұ任�Ŀ�Ⱥ͸߶ȣ�2�������η���
	LONG w = 1;
	LONG h = 1;
	int wp = 0;
	int hp = 0;
	
	// ���������ɢ���ұ任�Ŀ�Ⱥ͸߶ȣ�2�������η���
	while (w < lWidth)
	{
		w *= 2;
		wp++;
	}
	
	while (h < lHeight)
	{
		h *= 2;
		hp++;
	}

	// �����ڴ�
	double *f = new double[w * h * 3];
	double *F = new double[w * h * 3];

	// ��ʱ��ֵ������
	for (i = 0; i < h; i++)
	{
		for (j = 0; j < w * 3; j++)
		{
			if (i < lHeight && j < lLineBytes)
				f[i * w * 3 + j] = *(BYTE*)(lpDIBBits + lLineBytes * i + j);
			else
				f[w * i * 3 + j] = 0.0f;
		}
	}

	// ����Ƶ�׷���
	if (FreqDCT(f, F,lWidth, lHeight) == FALSE)
		return FALSE;

	// ������������
	for (i = 0; i < lHeight; i++)
	{
		for (j = 0; j < lLineBytes; j++)
		{
			// �ж��Ƿ񳬹�255
			if (fabs(F[i * w * 3 + j]) > 255)
			{
				// ���ڳ����ģ�ֱ������Ϊ255
				*(BYTE*)(lpDIBBits + lLineBytes * (lHeight - 1 - i) + j) = 255;
			}
			else
			{
				// ���û�г�������ʵ�ʼ�������ֵ
				*(BYTE*)(lpDIBBits + lLineBytes * (lHeight - 1 - i) + j) = (BYTE)fabs(F[i * w * 3 + j]);
			}
		}
	}

	// �ͷ��ڴ�
	delete []f;
	delete[] F;

	return TRUE;
}

/*************************************************************************
 *
 * �������ƣ�
 *   WALSH()
 *
 * ����:
 *   double* f			- �����ʱ������
 *   double* F			- �����Ƶ������
 *   int r				- 2������		
 *
 * ����ֵ:
 *   BOOL               - �ɹ�����TRUE�����򷵻�FALSE��
 *
 * ˵��:
 *   �ú�������һά�����ֶ�ʲ����������任��
 *
 ************************************************************************/
void CFreqCalculate::WALSH(double *f, double *F, int r)
{
	
	LONG	i;
	LONG	j;
	LONG	k;
	
	// �м����
	int		p;	
	double* X;
	
	// ��������ֶ�ʲ�任����
	LONG N = 1 << r;
	
	// �����������������
	double* X1 = new double[N];
	double* X2 = new double[N];
	
	// ��ʱ���д������X1
	memcpy(X1, f, sizeof(double) * N);
	
	// ��������
	for (k = 0; k < r; k++)
	{
		for (j = 0; j < 1<<k; j++)
		{
			for (i = 0; i < 1<<(r - k - 1); i++)
			{
				p = j * (1<<(r - k));
				X2[i + p] = X1[i + p] + X1[i + p + (int)(1<<(r - k - 1))];
				X2[i + p + (int)(1<<(r - k - 1))] = X1[i + p] - X1[i + p + (int)(1<<(r - k - 1))];
			}
		}
		
		// ����X1��X2  
		X = X1;
		X1 = X2;
		X2 = X;
	}
	
	// ����ϵ��
	for (j = 0; j < N; j++)
	{
		p = 0;
		for (i = 0; i < r; i++)
		{
			if (j & (1<<i))
			{
				p += 1<<(r - i - 1);
			}
		}

		F[j] = X1[p] / N;
	}
	
	// �ͷ��ڴ�
	delete X1;
	delete X2;
}

/*************************************************************************
 *
 * �������ƣ�
 *   IWALSH()
 *
 * ����:
 *   double* f			- �����ʱ������
 *   double* F			- �����Ƶ������
 *   int r				- 2������		
 *
 * ����ֵ:
 *   BOOL               - �ɹ�����TRUE�����򷵻�FALSE��
 *
 * ˵��:
 *   �ú�������һά�����ֶ�ʲ������������任��
 *
 ************************************************************************/
void CFreqCalculate::IWALSH(double *F, double *f, int r)
{
	
	int		i;
	
	// ����任����
	LONG N = 1 << r;
	
	// ���ÿ����ֶ�ʲ��������任���з��任
	WALSH(F, f, r);
	
	// ����ϵ��
	for (i = 0; i < N; i++)
		f[i] *= N;
}

/*************************************************************************
 *
 * �������ƣ�
 *   FreqWALSH()
 *
 * ����:
 *   double* f			- �����ʱ������
 *   double* F			- �����Ƶ������
 *	 LONG lWidth		- ͼ����
 *	 LONG lHeight		- ͼ��߶�
 *
 * ����ֵ:
 *   BOOL               - �ɹ�����TRUE�����򷵻�FALSE��
 *
 * ˵��:
 *   �ú������ж�ά�����ֶ�ʲ����������任��
 *
 ************************************************************************/
BOOL CFreqCalculate::FreqWALSH(double *f, double *F, LONG lWidth, LONG lHeight)
{
	
	LONG	i;
	LONG	j;
	LONG    k;

	BeginWaitCursor();
	
	// ������ɢ���ұ任�Ŀ�Ⱥ͸߶ȣ�2�������η���
	LONG w = 1;
	LONG h = 1;
	int wp = 0;
	int hp = 0;
	
	// ���������ɢ���ұ任�Ŀ�Ⱥ͸߶ȣ�2�������η���
	while (w < lWidth)
	{
		w *= 2;
		wp++;
	}
	
	while (h < lHeight)
	{
		h *= 2;
		hp++;
	}
		
	// �����ڴ�
	double* TempIn = new double[h];
	double* TempOut = new double[h];
	
	// ��y���������ɢ���ұ任
	for (i = 0; i < w * 3; i++)
	{
		// ��ȡ����
		for (j = 0; j < h; j++)
			TempIn[j] = f[j * w * 3 + i];
		
		// һά������ɢ���ұ任
		WALSH(TempIn, TempOut, hp);

		// ����任���
		for (j = 0; j < h; j++)
			f[j * w * 3 + i] = TempOut[j];
	}
	
	// �ͷ��ڴ�
	delete TempIn;
	delete TempOut;

	// �����ڴ�
	TempIn = new double[w];
	TempOut = new double[w];
	
	// ��x������п�����ɢ���ұ任
	for (i = 0; i < h; i++)
	{
		for (k = 0; k < 3; k++)
		{
			// ��ȡ����
			for (j = 0; j < w; j++)
				TempIn[j] = f[i * w * 3 + j * 3 + k];

			// һά������ɢ���ұ任
			WALSH(TempIn, TempOut, wp);

			// ����任���
			for (j = 0; j < w; j++)
				F[i * w * 3 + j * 3 + k] = TempOut[j];
		}
	}

	// �ͷ��ڴ�
	delete TempIn;
	delete TempOut;

	return TRUE;
}

/*************************************************************************
 *
 * �������ƣ�
 *   IFreqWALSH()
 *
 * ����:
 *   double* f			- �����ʱ������
 *   double* F			- �����Ƶ������
 *	 LONG lWidth		- ͼ����
 *	 LONG lHeight		- ͼ��߶�
 *
 * ����ֵ:
 *   BOOL               - �ɹ�����TRUE�����򷵻�FALSE��
 *
 * ˵��:
 *   �ú������ж�ά�����ֶ�ʲ������������任��
 *
 ************************************************************************/
BOOL CFreqCalculate::IFreqWALSH(double *f, double *F, LONG lWidth, LONG lHeight)
{
	
	LONG	i;
	LONG	j;
	LONG    k;

	BeginWaitCursor();
	
	// ����ֵ
	LONG w = 1;
	LONG h = 1;
	int wp = 0;
	int hp = 0;
	
	// ������и���Ҷ�任�Ŀ�Ⱥ͸߶ȣ�2�������η���
	while (w < lWidth)
	{
		w *= 2;
		wp++;
	}
	
	while (h < lHeight)
	{
		h *= 2;
		hp++;
	}

	LONG lLineBytes = m_pDIB->RowLen();

	// �����ڴ�
	double* TempIn = new double[w];
	double* TempOut = new double[w];
	
	// ��x������п��ٸ���Ҷ�任
	for (i = 0; i < h; i++)
	{
		for (k = 0; k < 3; k++)
		{
			// ��ȡ����
			for (j = 0; j < w; j++)
				TempIn[j] = F[i * w * 3 + j * 3 + k];

			// һά���ٸ���Ҷ�任
			IWALSH(TempIn, TempOut, wp);

			// ����任���
			for (j = 0; j < w; j++)
				F[i * w * 3 + j * 3 + k] = TempOut[j];
		}
	}

	// �ͷ��ڴ�
	delete TempIn;
	delete TempOut;
	
	TempIn = new double[h];
	TempOut = new double[h];

	// ��y������п��ٸ���Ҷ�任
	for (i = 0; i < w * 3; i++)
	{
		// ��ȡ����
		for (j = 0; j < h; j++)
			TempIn[j] = F[j * w * 3 + i];
		
		// һά���ٸ���Ҷ�任
		IWALSH(TempIn, TempOut, hp);

		// ����任���
		for (j = 0; j < h; j++)
			F[j * w * 3 + i] = TempOut[j];
	}
	
	// �ͷ��ڴ�
	delete TempIn;
	delete TempOut;

	for (i = 0; i < h; i++)
	{
		for (j = 0; j < w * 3; j++)
		{
			if (i < lHeight && j < lLineBytes)
				*(f + i * lLineBytes + j) = F[i * w * 3 + j];
		}
	}

	return TRUE;
}

/*************************************************************************
 *
 * �������ƣ�
 *   DIBWalsh()
 *
 * ����:
 *   HDIB	hDIB		- �������DIB
 *
 * ����ֵ:
 *   BOOL               - �ɹ�����TRUE�����򷵻�FALSE��
 *
 * ˵��:
 *   �ú�����ͼ����ж�ά�����ֶ�ʲ����������任��
 *
 ************************************************************************/
BOOL CFreqCalculate::DIBWalsh()
{
	
	LONG	i;
	LONG	j;
	
	BYTE* lpDIBBits = m_pDIB->FindAddress();
	
	if (m_pDIB->Channel() < 3)
	{
		MessageBox(L"���Ƚ���ת��Ϊ24λɫ(��32λɫ)���ٽ��д���", L"ϵͳ��ʾ" , MB_ICONINFORMATION | MB_OK);
		return FALSE;
	}
	
	LONG lWidth = m_pDIB->Width();
	
	LONG lHeight = m_pDIB->Height();

	LONG lLineBytes = m_pDIB->RowLen();
	
	// �����ֶ�ʲ����������任�Ŀ�Ⱥ͸߶ȣ�2�������η���
	LONG w = 1;
	LONG h = 1;
	int wp = 0;
	int hp = 0;
	
	// ���������ɢ���ұ任�Ŀ�Ⱥ͸߶ȣ�2�������η���
	while (w < lWidth)
	{
		w *= 2;
		wp++;
	}
	
	while (h < lHeight)
	{
		h *= 2;
		hp++;
	}
	
	// �����ڴ�
	double *f = new double[w * h * 3];
	double *F = new double[w * h * 3];
	
	// ��ʱ��ֵ������
	for (i = 0; i < h; i++)
	{
		for (j = 0; j < w * 3; j++)
		{
			if (i < lHeight && j < lLineBytes)
				f[i * w * 3 + j] = *(BYTE*)(lpDIBBits + lLineBytes * i + j);
			else
				f[w * i * 3 + j] = 0.0f;
		}
	}

	// ����Ƶ�׷���
	if (FreqWALSH(f, F,lWidth, lHeight) == FALSE)
		return FALSE;
	
	// ������������
	for (i = 0; i < lHeight; i++)
	{
		for (j = 0; j < lLineBytes; j++)
		{
			// �ж��Ƿ񳬹�255
			if (fabs(F[i * w * 3 + j] * 1000) > 255)
			{
				// ���ڳ����ģ�ֱ������Ϊ255
				*(BYTE*)(lpDIBBits + lLineBytes * (lHeight - 1 - i) + j) = 255;
			}
			else
			{
				// ���û�г�������ʵ�ʼ�������ֵ
				*(BYTE*)(lpDIBBits + lLineBytes * (lHeight - 1 - i) + j) = (BYTE)fabs(F[i * w * 3 + j] * 1000);
			}
		}
	}

	//�ͷ��ڴ�
	delete[] f;
	delete[] F;

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////
//																	     //
//							С���任									 //
//																		 //
///////////////////////////////////////////////////////////////////////////

#define DD 13
float h[DD] = { -0.00332761f,0.00569794f,	0.0196637f,	-0.0482603f,	-0.0485391f,
				0.292562f,	0.564406f,	0.292562f,	-0.0485391f,	-0.0482602f,
				-0.0196637f,	0.00569794f,	-0.0033276f};
float g[DD] = {0.00332761f,  0.00569794f,	-0.0196637f,	-0.0482603f,	0.0485391f,
			   0.292562f,	-0.564406f,	0.292562f,	0.0485391f,	-0.0482602f,
			   0.0196637f,	0.00569794f,	0.0033276f};
float hi[DD];
float gi[DD];

int CFreqCalculate::a(int nX, int nXSize)
{
	if (nX < 0)
		nX = -nX;
	if (nX >= nXSize)
		nX = nXSize * 2 - nX - 2;
	return nX;
}

/*************************************************************************
 *
 * �������ƣ�
 *   DWT()
 *
 * ����:
 *   HDIB	hDIB		- �������DIB
 *
 * ����ֵ:
 *   void	            - �޷���ֵ
 *
 * ˵��:
 *   �ú����Զ�ά���������ɢС���任��
 *
 ************************************************************************/
void CFreqCalculate::DWT(BYTE* lpData, int nX, int nY, int nXSize, int nYSize)
{
	
	int i, j, k, n;

	// �м����
	float temp1, temp2;
	float* BufferX;
	float* BufferY;

	BufferX = new float[nXSize];
	BufferY = new float[nYSize];

	// ˮƽ����
	for (n = 0; n < nY; n++)
	{
		for (i = 0; i < nX; i += 2)
		{
			temp1 = temp2 = 0;
			for (j = -(DD - 1) / 2; j <= (DD - 1) / 2; j++)
				temp1 = temp1 + *(lpData + n * nX + a(i + j, nX)) * h[j + (DD - 1) / 2];
			for (j = -(DD - 1) / 2 + 1; j <= (DD - 1) / 2 + 1; j++)
				temp2 = temp2 + *(lpData + n * nX + a(i + j, nX)) * g[j + (DD - 1) / 2 - 1];
			BufferX[i / 2] = temp1;
			BufferX[i / 2 + nX / 2] = temp2;
		}

		// �ش�����
		for (k = 0; k < nX; k++)
			*(lpData + n * nX + k) = (BYTE)BufferX[k];
	}

	// ��ֱ����
	for (n = 0; n < nX; n++)
	{
		for (i = 0; i < nY; i += 2)
		{
			temp1 = temp2 = 0;
			for (j = -(DD - 1) / 2; j <= (DD - 1) / 2; j++)
				temp1 = temp1 + *(lpData + a(i + j, nY) * nX + n) * h[j + (DD - 1) / 2];
			for (j = -(DD - 1) / 2 + 1; j <= (DD - 1) / 2 + 1; j++)
				temp2 = temp2 + *(lpData + a(i + j, nY) * nX + n) * g[j + (DD - 1) / 2 - 1];
			BufferY[i / 2] = temp2;
			BufferY[i / 2 + nY / 2] = temp1;
		}

		// �ش�����
		for (k = 0; k < nY; k++)
			*(lpData + k * nX + n) = (BYTE)BufferY[k];
	}
	
	delete[] BufferX;
	delete[] BufferY;
}

/*************************************************************************
 *
 * �������ƣ�
 *   IDWT()
 *
 * ����:
 *   HDIB	hDIB		- �������DIB
 *
 * ����ֵ:
 *   void	            - �޷���ֵ
 *
 * ˵��:
 *   �ú����Զ�ά�������С���ع���
 *
 ************************************************************************/
void CFreqCalculate::IDWT(BYTE* lpData, int nX, int nY, int nXSize, int nYSize)
{
	
	int i, j, k, n;

	// �м����
	float temp1, temp2;
	float* Buffer1;
	float* Buffer2;

	// ����ռ�ʱ�����һ��1��2014��12��7���޸������������
	Buffer1 = new float[nXSize + 1];
	Buffer2 = new float[nYSize + 1];

	for (i = 0; i < DD; i++)
	{
		hi[i] = h[DD - 1 - i];
		gi[i] = g[DD - 1 - i];
	}

	// ��ֱ����
	for (n = 0; n < nX; n++)
	{
		for (k = 0; k < nY / 2; k++)
		{
			Buffer1[k * 2] = *(lpData + k * nX + n);
			Buffer1[k * 2 + 1] = 0;
		}
		for (i = 0; i <  nY; i++)
		{
			temp1 = 0;
			for (j = -(DD - 1) / 2; j <= (DD - 1) / 2; j++)
			{
				temp1 = temp1 + Buffer1[a(i + j, nY)] * hi[j + (DD - 1) / 2];
				Buffer2[i] = temp1;
			}
		}
		for (k = 0; k < nY / 2; k++)
		{
			Buffer1[k * 2] = *(lpData + (k + nY / 2) * nX + n);
			Buffer1[k * 2 + 1] = 0;
		}
		for (i = 0; i < nY; i++)
		{
			temp1 = 0;
			for (j = -(DD - 1) / 2 - 1; j <= (DD - 1) / 2 - 1; j++)
				temp1 = temp1 + Buffer1[a(i + j, nY)] * gi[j + (DD - 1) / 2 + 1];
			temp2 = temp1 + Buffer2[i];
			Buffer2[i] = temp2;
		}

		// �ش�����
		for (k = 0; k < nY; k++)
			*(lpData + k * nX + n) = (BYTE)Buffer2[k];
	}

	// ˮƽ����
	for (n = 0; n < nY; n++)
	{
		for (k = 0; k < nX / 2; k++)
		{
			Buffer1[k * 2] = *(lpData + n * nX + k);
			Buffer1[k * 2 + 1] = 0;
		}
		
		for (i = 0; i < nX; i++)
		{
			temp1 = 0;
			for (j = -(DD - 1) / 2; j <= (DD - 1) / 2; j++)
				temp1 = temp1 + Buffer1[a(i + j, nX)] * hi[j + (DD - 1) / 2];
			Buffer2[i] = temp1;
		}
		
		for (k = 0; k < nX / 2; k++)
		{
			Buffer1[k * 2] = *(lpData + n * nX + k + nX / 2);
			Buffer1[k * 2 + 1] = 0;
		}

		for (i = 0; i < nX; i++)
		{
			temp1 = 0;
			for (j = -(DD - 1) / 2 - 1; j <= (DD - 1) / 2 - 1; j++)
				temp1 = temp1 + gi[j + (DD - 1) / 2 + 1] * Buffer1[a(i + j, nX)];
			temp2 = temp1 + Buffer2[i];
			Buffer2[i] = temp2;
		}

		// �ش�����
		for (k = 0; k < nX; k++)
			*(lpData + n * nX + k) = (BYTE)(Buffer2[k] * 4);
	}

	delete [] Buffer1;
	delete [] Buffer2;
}

/*************************************************************************
 *
 * �������ƣ�
 *   DIBDWT()
 *
 * ����:
 *   HDIB	hDIB		- �������DIB
 *
 * ����ֵ:
 *   void	            - �޷���ֵ
 *
 * ˵��:
 *   �ú�����ͼ����ж�ά��ɢС���任��
 *
 ************************************************************************/
void CFreqCalculate::DIBDWT()
{
	
	BYTE* lpDIBBits = m_pDIB->FindAddress();
	
	// �ж��Ƿ��ǿ��Դ���
	if (m_pDIB->BitCount() != 8)
	{
		
		MessageBox(L"���Ƚ���ת��Ϊ8λɫλͼ���ٽ��д���", L"ϵͳ��ʾ" , MB_ICONINFORMATION | MB_OK);
		
		return;
	}
	
	BeginWaitCursor();
	
	LONG lWidth = m_pDIB->Width();
	
	LONG lHeight = m_pDIB->Height();

	// ����С���ֽ�
	DWT(lpDIBBits, lWidth, lHeight, lWidth, lHeight);

	EndWaitCursor();
}

/*************************************************************************
 *
 * �������ƣ�
 *   DIBIDWT()
 *
 * ����:
 *   HDIB	hDIB		- �������DIB
 *
 * ����ֵ:
 *   void	            - �޷���ֵ
 *
 * ˵��:
 *   �ú�����ͼ����ж�άС���ع���
 *
 ************************************************************************/
void CFreqCalculate::DIBIDWT()
{
	
	BYTE* lpDIBBits = m_pDIB->FindAddress();
	
	// �ж��Ƿ��ǿ��Դ���
	if (m_pDIB->BitCount() != 8)
	{
		
		MessageBox(L"���Ƚ���ת��Ϊ8λɫλͼ���ٽ��д���", L"ϵͳ��ʾ" , MB_ICONINFORMATION | MB_OK);
		
		return;
	}
	
	BeginWaitCursor();
	
	LONG lWidth = m_pDIB->Width();
	
	LONG lHeight = m_pDIB->Height();

	// ����С���ع�	
	IDWT(lpDIBBits, lWidth, lHeight, lWidth, lHeight);

	EndWaitCursor();
}


