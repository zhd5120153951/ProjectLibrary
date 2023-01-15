// ************************************************************************
//  �ļ�����GrayProc.cpp
//
//  ͼ��Ҷȴ������⣺
//
//  ToGray()				- ��ɫλͼת��Ϊ�Ҷ�λͼ
//	PointInvert()			- ��ͼ����з�ɫ�任
//	GetIntensity()			- ��ͼ�����ɫ�����ĻҶȷֲ�(��Ŀ���ܶ�)����ͳ��
//	PointEqua()				- ��ͼ����лҶȷֲ����⻯����
//  GrayStretch()			- ��ͼ����лҶ����߱任
//	WindowTrans()			- ��ͼ����лҶȴ��ڱ任
//	PointDZ()				- ��ͼ����лҶȴ���任
//	PointDT()				- ��ͼ����лҶȴ�ͨ�任
//	PointSML()				- ��ͼ����е�ӳ�����ֱ��ͼ�涨���任
//	PointGML()				- ��ͼ�������ӳ�����ֱ��ͼ�涨���任
//	DynamicCompress()		- ��ͼ����лҶȶ�̬��Χѹ������ 
//	CutWave()				- ��ͼ����лҶ��������� 
//
// ************************************************************************

#include "stdafx.h"
#include "DIP_System.h"
#include "GrayProc.h"
#include "math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGrayProc

// Ĭ�Ϲ��캯��
CGrayProc::CGrayProc()
{
}

// ��pDIB��ֵ���ڲ�����
CGrayProc::CGrayProc(CDIB* pDIB)
{
	m_pDIB = pDIB;
}

CGrayProc::~CGrayProc()
{
}

BEGIN_MESSAGE_MAP(CGrayProc, CWnd)
END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////////
//							����ģ�鿪ʼ								  //
////////////////////////////////////////////////////////////////////////////

// �ú�������ɫλͼת��Ϊ�Ҷ�ͼ��
void CGrayProc::ToGray()
{
	LONG i, j;
	
	CHECK_EDITABLE(m_pDIB->GetImage());

	BeginWaitCursor();
	
	int nChannel = m_pDIB->Channel();
	LONG lWidth = m_pDIB->Width();
	LONG lHeight = m_pDIB->Height();
	LONG lLineBytes = m_pDIB->RowLen();
	BYTE* lpDIBBits = m_pDIB->FindAddress();

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

	EndWaitCursor();
}

// �ú�����ͼ����з�ɫ�任����
void CGrayProc::PointInvert()
{
	LONG i, j;
	
	CHECK_EDITABLE(m_pDIB->GetImage());
	
	BeginWaitCursor();
	
	int nChannel = m_pDIB->Channel();
	LONG lWidth = m_pDIB->Width();
	LONG lHeight = m_pDIB->Height();
	LONG lLineBytes = m_pDIB->RowLen();
	BYTE* lpDIBBits = m_pDIB->FindAddress();
	
	// �Ը����ؽ��лҶ�ת��
	for (i = 0; i < lHeight; i ++)
	{
		for (j = 0; j < lWidth; j ++)
		{
			// �����ظ���ɫ�������з�ɫ����
			BYTE *pSrc = lpDIBBits + nChannel * j + lLineBytes * i;
			*pSrc = 255 - *pSrc;
			*(pSrc+1) = 255 - *(pSrc+1);
			*(pSrc+2) = 255 - *(pSrc+2);
		}
	}

	EndWaitCursor();
}

/*************************************************************************
 *
 * �������ƣ�
 *   GetIntensity()
 *
 * ����:
 *	 int  *nNs_R		- ��ɫ�����ĻҶȷֲ�ͳ��
 *   int  *nNs_G		- ��ɫ�����ĻҶȷֲ�ͳ��
 *   int  *nNs_B		- ��ɫ�����ĻҶȷֲ�ͳ��
 *
 * ����ֵ:
 *   void				- �޷���ֵ
 *
 * ˵��:
 *   �ú�����ͼ��RGB�������лҶȷֲ���ͳ��
 *
 ************************************************************************/
void CGrayProc::GetIntensity(int *nNs_R, int *nNs_G, int *nNs_B)
{
	LONG i, j;

	//������ʼ��
	memset(nNs_R,0,sizeof(int) * 256);
	memset(nNs_G,0,sizeof(int) * 256);
	memset(nNs_B,0,sizeof(int) * 256);

	CHECK_EDITABLE(m_pDIB->GetImage());
	
	BeginWaitCursor();
	
	int nChannel = m_pDIB->Channel();
	LONG lWidth = m_pDIB->Width();
	LONG lHeight = m_pDIB->Height();
	LONG lLineBytes = m_pDIB->RowLen();
	BYTE* lpDIBBits = m_pDIB->FindAddress();

	// �Ը����ؽ��лҶ�ת��
	for (i = 0; i < lHeight; i++)
	{
		for (j = 0; j < lWidth; j++)
		{
			BYTE *pSrc = lpDIBBits + nChannel * j + lLineBytes * i;
			nNs_R[*(pSrc + 2)]++;
			nNs_G[*(pSrc + 1)]++;
			nNs_B[*pSrc]++;
		}
	}

	EndWaitCursor();
}

/*************************************************************************
 *
 * �������ƣ�
 *   GetIntensity()
 *
 * ����:
 *	 float *fPs_R		- ��ɫ�����ĻҶȷֲ��ܶ�ͳ��
 *   float *fPs_G		- ��ɫ�����ĻҶȷֲ��ܶ�ͳ��
 *   float *fPs_B		- ��ɫ�����ĻҶȷֲ��ܶ�ͳ��
 *
 * ����ֵ:
 *   void				- �޷���ֵ
 *
 * ˵��:
 *   �ú�����ͼ��RGB�������лҶȷֲ��ܶȵ�ͳ��
 *
 ************************************************************************/
void CGrayProc::GetIntensity(float *fPs_R, float *fPs_G, float *fPs_B)
{
	LONG i, j;

	// �Ҷȼ���
	int nNs_R[256];
	int nNs_G[256];
	int nNs_B[256];

	// ������ʼ��
	memset(nNs_R,0,sizeof(nNs_R));
	memset(nNs_G,0,sizeof(nNs_G));
	memset(nNs_B,0,sizeof(nNs_B));
	
	CHECK_EDITABLE(m_pDIB->GetImage());

	BeginWaitCursor();
	
	int nChannel = m_pDIB->Channel();
	LONG lWidth = m_pDIB->Width();
	LONG lHeight = m_pDIB->Height();
	LONG lLineBytes = m_pDIB->RowLen();
	BYTE* lpDIBBits = m_pDIB->FindAddress();

	// �Ը����ؽ��лҶ�ת��
	for (i = 0; i < lHeight; i++)
	{
		for (j = 0; j < lWidth; j++)
		{
			BYTE *pSrc = lpDIBBits + nChannel * j + lLineBytes * i;
			nNs_R[*(pSrc + 2)]++;
			nNs_G[*(pSrc + 1)]++;
			nNs_B[*pSrc]++;
		}
	}

	// ����Ҷȷֲ��ܶ�
	for(i=0;i<256;i++)
	{
		fPs_R[i] = nNs_R[i] / (lHeight * lWidth * 1.0f);
		fPs_G[i] = nNs_G[i] / (lHeight * lWidth * 1.0f);
		fPs_B[i] = nNs_B[i] / (lHeight * lWidth * 1.0f);
	}

	EndWaitCursor();
}

/*************************************************************************
 *
 * �������ƣ�
 *   GetIntensity()
 *
 * ����:
 *	 float *fPs_Y		- Y �����ĻҶȷֲ��ܶ�ͳ��
 *
 * ����ֵ:
 *   void				- �޷���ֵ
 *
 * ˵��:
 *   �ú�����ͼ��Y�������лҶȷֲ��ܶȵ�ͳ��
 *
 ************************************************************************/
void CGrayProc::GetIntensity(float *fPs_Y)
{
	LONG i, j;

	// �Ҷȼ���
	int nNs_Y[256];

	// ������ʼ��
	memset(nNs_Y,0,sizeof(nNs_Y));

	CHECK_EDITABLE(m_pDIB->GetImage());
	
	BeginWaitCursor();

	int nChannel = m_pDIB->Channel();
	LONG lWidth = m_pDIB->Width();
	LONG lHeight = m_pDIB->Height();
	LONG lLineBytes = m_pDIB->RowLen();
	BYTE* lpDIBBits = m_pDIB->FindAddress();

	// �Ը����ؽ��лҶ�ת��
	for (i = 0; i < lHeight; i ++)
	{
		for (j = 0; j < lWidth; j ++)
		{
			BYTE *pSrc = lpDIBBits + nChannel * j + lLineBytes * i;
			BYTE R = *(pSrc + 2);
			BYTE G = *(pSrc + 1);
			BYTE B = *pSrc;
			BYTE Y = (9798 * R + 19235 * G + 3735 * B) / 32768;
			nNs_Y[Y]++;
		}
	}

	// ����Ҷȷֲ��ܶ�
	for(i=0;i<256;i++)
		fPs_Y[i] = nNs_Y[i] / (lHeight * lWidth * 1.0f);

	EndWaitCursor();
}

/*************************************************************************
 *
 * �������ƣ�
 *   GetIntensity()
 *
 * ����:
 *	 int *nNs_Y			- Y �����ĻҶȷֲ�ͳ��
 *
 * ����ֵ:
 *   void				- �޷���ֵ
 *
 * ˵��:
 *   �ú�����ͼ��Y�������лҶȷֲ�ͳ��
 *
 ************************************************************************/
void CGrayProc::GetIntensity(int *nNs_Y)
{
	LONG i, j;

	// ������ʼ��
	memset(nNs_Y, 0, sizeof(nNs_Y));
	
	CHECK_EDITABLE(m_pDIB->GetImage());
	
	BeginWaitCursor();
	
	int nChannel = m_pDIB->Channel();
	LONG lWidth = m_pDIB->Width();
	LONG lHeight = m_pDIB->Height();
	LONG lLineBytes = m_pDIB->RowLen();
	BYTE* lpDIBBits = m_pDIB->FindAddress();

	// �Ը����ؽ��лҶ�ת��
	for (i = 0; i < lHeight; i ++)
	{
		for (j = 0; j < lWidth; j ++)
		{
			// ����Ҷ�ֵ
			BYTE *pCur = lpDIBBits + j * nChannel + lLineBytes * i;
			BYTE Y = (9798 * *(pCur+2) + 19235 * *(pCur+1) + 3735 * *pCur) / 32768;
			// �Ҷ�ͳ�Ƽ���
			nNs_Y[Y]++;		
		}
	}

	EndWaitCursor();
}

// �ú�����ͼ����лҶȷֲ����⻯����
void CGrayProc::PointEqua()
{
	
	LONG i;
	LONG j;

	// �Ҷȷֲ��ܶ�
	float fPs_R[256];
	float fPs_G[256];
	float fPs_B[256];

	// �м����
	float temp_r[256];
	float temp_g[256];
	float temp_b[256];

	int nNs_R[256];
	int nNs_G[256];
	int nNs_B[256];
	
	// ��ʼ��
	memset(temp_r, 0, sizeof(temp_r));
	memset(temp_g, 0, sizeof(temp_g));
	memset(temp_b, 0, sizeof(temp_b));
	
	CHECK_EDITABLE(m_pDIB->GetImage());	
	
	BeginWaitCursor();
	
	// ��ȡͼ��ĻҶȷֲ��ܶ�
	GetIntensity(fPs_R, fPs_G, fPs_B);
	
	// ���о��⻯���� 
	for(i = 0; i < 256; i++)
	{
		if(i == 0)
		{
			temp_r[0] = fPs_R[0];
			temp_g[0] = fPs_G[0];
			temp_b[0] = fPs_B[0];
		}
		else
		{
			temp_r[i] = temp_r[i-1] + fPs_R[i];
			temp_g[i] = temp_g[i-1] + fPs_G[i];
			temp_b[i] = temp_b[i-1] + fPs_B[i];
		}
		nNs_R[i] = (int)(255.0f * temp_r[i] + 0.5f);
		nNs_G[i] = (int)(255.0f * temp_g[i] + 0.5f);
		nNs_B[i] = (int)(255.0f * temp_b[i] + 0.5f);
	}

	int nChannel = m_pDIB->Channel();
	LONG lWidth = m_pDIB->Width();
	LONG lHeight = m_pDIB->Height();
	LONG lLineBytes = m_pDIB->RowLen();
	BYTE* lpDIBBits = m_pDIB->FindAddress();

	// �Ը����ؽ��лҶ�ת��
	for (i = 0; i < lHeight; i++)
	{
		for (j = 0; j < lWidth; j++)
		{
			BYTE *pSrc = lpDIBBits + nChannel * j + lLineBytes * i;
			*(pSrc + 2) = nNs_R[*(pSrc + 2)];
			*(pSrc + 1) = nNs_G[*(pSrc + 1)];
			*pSrc = nNs_B[*pSrc]++;
		}
	}
	
	EndWaitCursor();
}

/*************************************************************************
 *
 * �������ƣ�
 *   GrayStretch()
 *
 * ����:
 *	 BYTE bX1			- �۵�һ��ԭʼ�Ҷ�
 *	 BYTE bY1			- �۵�һ�ı任�Ҷ�
 *	 BYTE bX2			- �۵����ԭʼ�Ҷ�
 *   BYTE bY2			- �۵���Ķ�Ӧ�Ҷ�
 *
 * ����ֵ:
 *   void				- �޷���ֵ
 *
 * ˵��:
 *   �ú�����ͼ����лҶ����߱任
 *
 ************************************************************************/
void CGrayProc::GrayStretch(BYTE bX1, BYTE bY1, BYTE bX2, BYTE bY2)
{
	LONG i;
	LONG j;
	
	CHECK_EDITABLE(m_pDIB->GetImage());
	
	BeginWaitCursor();

	// �Ҷ�ӳ���
	BYTE	bMap[256] = {0};

	// ����Ҷ�ӳ���
	for (i = 0; i <= bX1; i++)	//[0 ���� X1]
	{
		// �ж�bX1�Ƿ����0����ֹ��ĸΪ0��
		if (bX1 > 0)
		{
			// ���Ա任
			bMap[i] = (BYTE) (bY1 * i / bX1);
		}
		else
		{
			// ֱ�Ӹ�ֵΪ0
			bMap[i] = 0;
		}
	}
	for (; i <= bX2; i++)		//(X1 ���� X2]
	{
		// �ж�bX1�Ƿ����bX2����ֹ��ĸΪ0��
		if (bX2 != bX1)
		{
			// ���Ա任
			bMap[i] = bY1 + (BYTE) ((bY2 - bY1) * (i - bX1) / (bX2 - bX1));
		}
		else
		{
			// ֱ�Ӹ�ֵΪbY1
			bMap[i] = bY1;
		}
	}
	for (; i < 256; i++)		//(X2 ���� 256)
	{
		// �ж�bX2�Ƿ����255����ֹ��ĸΪ0��
		if (bX2 != 255)
		{
			// ���Ա任
			bMap[i] = bY2 + (BYTE) ((255 - bY2) * (i - bX2) / (255 - bX2));
		}
		else
		{
			// ֱ�Ӹ�ֵΪ255
			bMap[i] = 255;
		}
	}
	
	int nChannel = m_pDIB->Channel();
	LONG lWidth = m_pDIB->Width();
	LONG lHeight = m_pDIB->Height();
	LONG lLineBytes = m_pDIB->RowLen();
	BYTE* lpDIBBits = m_pDIB->FindAddress();

	// �Ը����ؽ��лҶ�ת��
	for (i = 0; i < lHeight; i ++)
	{
		for (j = 0; j < lWidth; j ++)
		{
			// �����ظ���ɫ�������лҶ�ӳ�䴦��
			BYTE *pSrc = lpDIBBits + nChannel * j + lLineBytes * i;
			*pSrc = bMap[*pSrc];
			*(pSrc + 1) = bMap[*(pSrc + 1)];
			*(pSrc + 2) = bMap[*(pSrc + 2)];
		}
	}
	
	EndWaitCursor();
}

/*************************************************************************
 *
 * �������ƣ�
 *   WindowTrans()
 *
 * ����:
 *	 BYTE bLow			- �����½�
 *	 BYTE bUp			- �����Ͻ�
 *
 * ����ֵ:
 *   void				- �޷���ֵ
 *
 * ˵��:
 *   �ú�����ͼ����лҶȴ��ڱ任
 *
 ************************************************************************/
void CGrayProc::WindowTrans(BYTE bLow, BYTE bUp)
{
	
	LONG i;
	LONG j;

	CHECK_EDITABLE(m_pDIB->GetImage());
	
	BeginWaitCursor();
	
	int nChannel = m_pDIB->Channel();
	LONG lWidth = m_pDIB->Width();
	LONG lHeight = m_pDIB->Height();
	LONG lLineBytes = m_pDIB->RowLen();
	BYTE* lpDIBBits = m_pDIB->FindAddress();

	// �Ը����ؽ��лҶ�ת��
	for (i = 0; i < lHeight; i ++)
	{
		for (j = 0; j < lWidth; j ++)
		{
			BYTE *pSrc = lpDIBBits + nChannel * j + lLineBytes * i;
			if (*pSrc < bLow)           *pSrc = 0;
			else if (*pSrc > bUp)       *pSrc = 255;
			if (*(pSrc + 1) < bLow)     *(pSrc + 1) = 0;
			else if (*(pSrc + 1) > bUp) *(pSrc + 1) = 255;
			if (*(pSrc + 2) < bLow)     *(pSrc + 2) = 0;
			else if (*(pSrc + 2) > bUp) *(pSrc + 2) = 255;
		}
	}

	EndWaitCursor();
}

/*************************************************************************
 *
 * �������ƣ�
 *   PointDZ()
 *
 * ����:
 *	 BYTE bLow			- �����½�
 *	 BYTE bUp			- �����Ͻ�
 *
 * ����ֵ:
 *   void				- �޷���ֵ
 *
 * ˵��:
 *   �ú�����ͼ����лҶȴ���任
 *
 ************************************************************************/
void CGrayProc::PointDZ(BYTE bLow, BYTE bUp)
{
	
	LONG i;
	LONG j;
	
	CHECK_EDITABLE(m_pDIB->GetImage());
	
	BeginWaitCursor();
	
	int nChannel = m_pDIB->Channel();
	LONG lWidth = m_pDIB->Width();
	LONG lHeight = m_pDIB->Height();
	LONG lLineBytes = m_pDIB->RowLen();
	BYTE* lpDIBBits = m_pDIB->FindAddress();

	// �Ը����ؽ��лҶ�ת��
	for (i = 0; i < lHeight; i++)
	{
		for (j = 0; j < lWidth; j++)
		{
			BYTE *pSrc = lpDIBBits + nChannel * j + lLineBytes * i;
			if (*pSrc < bLow || *pSrc > bUp)           *pSrc = 255;
			if (*(pSrc + 1) < bLow || *pSrc > bUp)     *(pSrc + 1) = 255;
			if (*(pSrc + 2) < bLow || *pSrc > bUp)     *(pSrc + 2) = 255;
		}
	}
	
	EndWaitCursor();
}

/*************************************************************************
 *
 * �������ƣ�
 *   PointDT()
 *
 * ����:
 *	 BYTE bLow			- �����½�
 *	 BYTE bUp			- �����Ͻ�
 *
 * ����ֵ:
 *   void				- �޷���ֵ
 *
 * ˵��:
 *   �ú�����ͼ����лҶȴ�ͨ�任
 *
 ************************************************************************/
void CGrayProc::PointDT(BYTE bLow, BYTE bUp)
{
	
	LONG i;
	LONG j;
	
	CHECK_EDITABLE(m_pDIB->GetImage());
	
	BeginWaitCursor();

	int nChannel = m_pDIB->Channel();
	LONG lWidth = m_pDIB->Width();
	LONG lHeight = m_pDIB->Height();
	LONG lLineBytes = m_pDIB->RowLen();
	BYTE* lpDIBBits = m_pDIB->FindAddress();
	
	// �Ը����ؽ��лҶ�ת��
	for (i = 0; i < lHeight; i++)
	{
		for (j = 0; j < lWidth; j++)
		{
			BYTE *pSrc = lpDIBBits + nChannel * j + lLineBytes * i;
			if (*pSrc < bLow || *pSrc > bUp)           *pSrc = 0;
			if (*(pSrc + 1) < bLow || *pSrc > bUp)     *(pSrc + 1) = 0;
			if (*(pSrc + 2) < bLow || *pSrc > bUp)     *(pSrc + 2) = 0;
		}
	}
	
	EndWaitCursor();
}

/*************************************************************************
 *
 * �������ƣ�
 *   PointSML()
 *
 * ����:
 *	 BYTE bNum			- �涨ֱ��ͼ�Ҷȼ���
 *	 int* npNu			- �涨ֱ��ͼ�Ҷ�ӳ���ϵ
 *	 float* fpPu		- �涨ֱ��ͼ���Ҷȵķֲ�����
 *
 * ����ֵ:
 *   void				- �޷���ֵ
 *
 * ˵��:
 *   �ú�����ͼ����е�ӳ�����ֱ��ͼ�涨���任
 *
 ************************************************************************/
void CGrayProc::PointSML(BYTE bNum, int *npNu, float *fpPu)
{
	
	LONG i;
	LONG j;

	// �Ҷȷֲ��ܶ�
	int ns_r[256];
	int ns_g[256];
	int ns_b[256];
	
	// �Ҷȷֲ�����
	float ps_r[256];
	float ps_g[256];
	float ps_b[256];

	// �м���ʱ����
	float temp_r[256];
	float temp_g[256];
	float temp_b[256];
	
	CHECK_EDITABLE(m_pDIB->GetImage());
	
	BeginWaitCursor();
	
	int nChannel = m_pDIB->Channel();
	LONG lWidth = m_pDIB->Width();
	LONG lHeight = m_pDIB->Height();
	LONG lLineBytes = m_pDIB->RowLen();
	BYTE* lpDIBBits = m_pDIB->FindAddress();
	
	// �ԻҶ��ܶȷֲ�����ͳ��	
	GetIntensity(ns_r, ns_g, ns_b);

	// �ԻҶȷֲ����ʽ���ͳ��
	GetIntensity(ps_r, ps_g, ps_b);
	
	// ����ԭʼ�ۼ�ֱ��ͼ
	for (i = 0; i < 256; i++)
	{
		if (i == 0)
		{
			temp_r[0] = ps_r[0];
			temp_g[0] = ps_g[0];
			temp_b[0] = ps_b[0];
		}
		else
		{
			temp_r[i] = temp_r[i-1] + ps_r[i];
			temp_g[i] = temp_g[i-1] + ps_g[i];
			temp_b[i] = temp_b[i-1] + ps_b[i];
		}
		ps_r[i] = temp_r[i];
		ps_g[i] = temp_g[i];
		ps_b[i] = temp_b[i];
	}
	
	// ����涨�ۼ�ֱ��ͼ
	for (i = 0; i < bNum; i++)
	{
		if (i == 0)
		{
			temp_r[0] = fpPu[0];
		}
		else
		{
			temp_r[i] = temp_r[i-1] + fpPu[i];
		}
		fpPu[i] = temp_r[i];
	}
	
	// ȷ��ӳ���Ӧ��ϵ
	for (i = 0; i < 256; i++)
	{
		// ��ӽ��Ĺ涨ֱ��ͼ�Ҷȼ�
		int m_r = 0;
		int m_g = 0;
		int m_b = 0;
		
		// ��С��ֵ
		float min_value_r = 1.0f;
		float min_value_g = 1.0f;
		float min_value_b = 1.0f;

		// �Թ涨ֱ��ͼ���ҶȽ���ö��
		for (j = 0; j < bNum; j++)
		{
			// ��ǰ��ֵ
			float now_value = 0.0f;
			
			// ��R���������ֵ
			if (ps_r[i] - fpPu[j] >= 0.0f)
				now_value = ps_r[i] - fpPu[j];
			else
				now_value = fpPu[j] - ps_r[i];

			// Ѱ����ӽ��Ĺ涨ֱ��ͼ�Ҷȼ�
			if (now_value < min_value_r)
			{
				// ��ӽ��ĻҶȼ�
				m_r = j;

				// �ݴ���С��ֵ
				min_value_r = now_value;
			}

			// ��G���������ֵ
			if (ps_g[i] - fpPu[j] >= 0.0f)
				now_value = ps_g[i] - fpPu[j];
			else
				now_value = fpPu[j] - ps_g[i];

			// Ѱ����ӽ��Ĺ涨ֱ��ͼ�Ҷȼ�
			if (now_value < min_value_g)
			{
				// ��ӽ��ĻҶȼ�
				m_g = j;
			
				// �ݴ���С��ֵ
				min_value_g = now_value;
			}
				
			// ��B���������ֵ
			if (ps_b[i] - fpPu[j] >= 0.0f)
				now_value = ps_b[i] - fpPu[j];
			else
				now_value = fpPu[j] - ps_b[i];
			
			// Ѱ����ӽ��Ĺ涨ֱ��ͼ�Ҷȼ�
			if (now_value < min_value_b)
			{
				// ��ӽ��ĻҶȼ�
				m_b = j;
			
				// �ݴ���С��ֵ
				min_value_b = now_value;
			}
		}

		// �����Ҷ�ӳ���
		ns_r[i] = npNu[m_r];
		ns_g[i] = npNu[m_g];
		ns_b[i] = npNu[m_b];
	}

	// �Ը����ؽ��лҶ�ת��
	for (i = 0; i < lHeight; i++)
	{
		for (j = 0; j < lWidth; j++)
		{
			BYTE *pSrc = lpDIBBits + nChannel * j + lLineBytes * i;
			*(pSrc + 2) = ns_r[*(pSrc + 2)];
			*(pSrc + 1) = ns_g[*(pSrc + 1)];
			*pSrc = ns_b[*pSrc];
		}
	}
	
	EndWaitCursor();
}

/*************************************************************************
 *
 * �������ƣ�
 *   PointGML()
 *
 * ����:
 *	 BYTE bNum			- �涨ֱ��ͼ�Ҷȼ���
 *	 int* npNu			- �涨ֱ��ͼ�Ҷ�ӳ���ϵ
 *	 float* fpPu		- �涨ֱ��ͼ���Ҷȵķֲ�����
 *
 * ����ֵ:
 *   void				- �޷���ֵ
 *
 * ˵��:
 *   �ú�����ͼ�������ӳ�����ֱ��ͼ�涨���任
 *
 ************************************************************************/
void CGrayProc::PointGML(BYTE bNum, int *npNu, float *fpPu)
{
	
	LONG i;
	LONG j;
	LONG k;

	// �Ҷȷֲ��ܶ�
	int ns_r[256];
	int ns_g[256];
	int ns_b[256];
	
	// �Ҷȷֲ�����
	float ps_r[256];
	float ps_g[256];
	float ps_b[256];

	// �м���ʱ����
	float temp_r[256];
	float temp_g[256];
	float temp_b[256];

	// �Ҷ�ӳ���������ʼ�Ҷ�
	int A1_r = 0;
	int A1_g = 0;
	int A1_b = 0;
	
	// �Ҷ�ӳ���������ֹ�Ҷ�
	int A2_r = 0;
	int A2_g = 0;
	int A2_b = 0;

	CHECK_EDITABLE(m_pDIB->GetImage());
	
	BeginWaitCursor();
	
	int nChannel = m_pDIB->Channel();
	LONG lWidth = m_pDIB->Width();
	LONG lHeight = m_pDIB->Height();
	LONG lLineBytes = m_pDIB->RowLen();
	BYTE* lpDIBBits = m_pDIB->FindAddress();

	// �ԻҶ��ܶȷֲ�����ͳ��	
	GetIntensity(ns_r, ns_g, ns_b);

	// �ԻҶȷֲ����ʽ���ͳ��
	GetIntensity(ps_r, ps_g, ps_b);
	
	// ����ԭʼ�ۼ�ֱ��ͼ
	for (i = 0; i < 256; i++)
	{
		if (i == 0)
		{
			temp_r[0] = ps_r[0];
			temp_g[0] = ps_g[0];
			temp_b[0] = ps_b[0];
		}
		else
		{
			temp_r[i] = temp_r[i-1] + ps_r[i];
			temp_g[i] = temp_g[i-1] + ps_g[i];
			temp_b[i] = temp_b[i-1] + ps_b[i];
		}
		ps_r[i] = temp_r[i];
		ps_g[i] = temp_g[i];
		ps_b[i] = temp_b[i];
	}
	
	// ����涨�ۼ�ֱ��ͼ
	for(i = 0; i < bNum; i++)
	{
		if(i == 0)
		{
			temp_r[0] = fpPu[0];
		}
		else
		{
			temp_r[i] = temp_r[i-1] + fpPu[i];
		}
		fpPu[i] = temp_r[i];
	}

	// ȷ��ӳ���Ӧ��ϵ
	for (i = 0; i < bNum; i++)
	{
		// ��С��ֵ
		float min_value_r = 1.0f;
		float min_value_g = 1.0f;
		float min_value_b = 1.0f;

		// ��ԭʼֱ��ͼ�ֲ�����ö�ټ���
		for (j = 0; j < 256; j++)
		{
			// ��ǰ��ֵ
			float now_value = 0.0f;
			
			// ��R���������ֵ
			if (ps_r[j] - fpPu[i] >= 0.0f)
				now_value = ps_r[j] - fpPu[i];
			else
				now_value = fpPu[i] - ps_r[j];
			
			// Ѱ����ӽ��Ĺ涨ֱ��ͼ�Ҷȼ�
			if (now_value < min_value_r)
			{
				// ��ӽ��ĻҶȼ�
				A2_r=j;

				// �ݴ���С��ֵ
				min_value_r = now_value;
			}

			// ��G���������ֵ
			if (ps_g[j] - fpPu[i] >= 0.0f)
				now_value = ps_g[j] - fpPu[i];
			else
				now_value = fpPu[i] - ps_g[j];
			
			// Ѱ����ӽ��Ĺ涨ֱ��ͼ�Ҷȼ�
			if (now_value < min_value_g)
			{
				// ��ӽ��ĻҶȼ�
				A2_g = j;

				// �ݴ���С��ֵ
				min_value_g = now_value;
			}
				
			// ��B���������ֵ
			if (ps_b[j] - fpPu[i] >= 0.0f)
				now_value = ps_b[j] - fpPu[i];
			else
				now_value = fpPu[i] - ps_b[j];
			
			// Ѱ����ӽ��Ĺ涨ֱ��ͼ�Ҷȼ�
			if (now_value < min_value_b)
			{
				// ��ӽ��ĻҶȼ�
				A2_b = j;
				
				// �ݴ���С��ֵ
				min_value_b = now_value;
			}
		}
			
		// ��[A1_r,A2_r]�����ԭʼ�Ҷ�kͨ������ӳ���ns_r[]ӳ�䵽�Ҷ�nu[i]
		for (k = A1_r; k <= A2_r; k++)
			ns_r[k] = npNu[i];
	
		// �ƶ�������ʼλ��
		A1_r = A2_r + 1;
			
		// ��[A1_g,A2_g]�����ԭʼ�Ҷ�kͨ������ӳ���ns_g[]ӳ�䵽�Ҷ�nu[i]
		for (k = A1_g; k <= A2_g; k++)
			ns_g[k] = npNu[i];
		
		// �ƶ�������ʼλ��
		A1_g = A2_g + 1;
		
		// ��[A1_b,A2_b]�����ԭʼ�Ҷ�kͨ������ӳ���ns_b[]ӳ�䵽�Ҷ�nu[i]
		for (k = A1_b; k <= A2_b; k++)
			ns_b[k] = npNu[i];
		
		// �ƶ�������ʼλ��
		A1_b = A2_b + 1;
	}

	// �Ը����ؽ��лҶ�ת��
	for (i = 0; i < lHeight; i++)
	{
		for (j = 0; j < lWidth; j++)
		{
			// �����ظ���ɫ�������е�ӳ������ֱ��ͼ�涨��ӳ�䴦��
			BYTE *pSrc = lpDIBBits + nChannel * j + lLineBytes * i;
			*(pSrc + 2) = ns_r[*(pSrc + 2)];
			*(pSrc + 1) = ns_g[*(pSrc + 1)];
			*pSrc = ns_b[*pSrc];
		}
	}
	
	EndWaitCursor();
}

/*************************************************************************
 *
 * �������ƣ�
 *   DynamicCompress()
 *
 * ����:
 *	 float fC			- �߶ȱ�������
 *
 * ����ֵ:
 *   void				- �޷���ֵ
 *
 * ˵��:
 *   �ú�����ͼ����лҶȶ�̬��Χѹ������
 *
 ************************************************************************/
void CGrayProc::DynamicCompress(int nC)
{
	
	LONG i;
	LONG j;

	CHECK_EDITABLE(m_pDIB->GetImage());
	
	BeginWaitCursor();

	int nChannel = m_pDIB->Channel();
	LONG lWidth = m_pDIB->Width();
	LONG lHeight = m_pDIB->Height();
	LONG lLineBytes = m_pDIB->RowLen();
	BYTE* lpDIBBits = m_pDIB->FindAddress();

	// �Ը����ؽ��лҶ�ת��
	for (i = 0; i < lHeight; i++)
	{
		for (j = 0; j < lWidth; j++)
		{
			BYTE *pSrc = lpDIBBits + nChannel * j + lLineBytes * i;
			*(pSrc + 2) = (BYTE)(nC * log(1.0 + *(pSrc + 2)));
			*(pSrc + 1) = (BYTE)(nC * log(1.0 + *(pSrc + 1)));
			*pSrc = (BYTE)(nC * log(1.0 + *pSrc));
		}
	}
	
	EndWaitCursor();
}

/*************************************************************************
 *
 * �������ƣ�
 *   CutWave()
 *
 * ����:
 *
 * ����ֵ:
 *   void				- �޷���ֵ
 *
 * ˵��:
 *   �ú�������ͼ����лҶ���������
 *
 ************************************************************************/
void CGrayProc::CutWave()
{
	
	LONG i;
	LONG j;

	CHECK_EDITABLE(m_pDIB->GetImage());
	
	BeginWaitCursor();

	int nChannel = m_pDIB->Channel();
	LONG lWidth = m_pDIB->Width();
	LONG lHeight = m_pDIB->Height();
	LONG lLineBytes = m_pDIB->RowLen();
	BYTE* lpDIBBits = m_pDIB->FindAddress();

	// �Ը����ؽ��лҶ�ת��
	for (i = 0; i < lHeight; i++)
	{
		for (j = 0; j < lWidth; j++)
		{
			BYTE *pSrc = lpDIBBits + nChannel * j + lLineBytes * i;
			BYTE B = *pSrc;
			BYTE G = *(pSrc + 1);
			BYTE R = *(pSrc + 2);
			BYTE Y = (9798 * R + 19235 * G + 3735 * B) / 32768;

			if (Y < 30)
			{
				R = G = B = 0;
			}

			if (Y >= 30 && Y < 200)
			{
				R = R * 256 / 200;
				G = G * 256 / 200;
				B = B * 256 / 200;

				// ���ޱ���
				if (R > 255)	R = 255;
				if (R < 0)		R = 0;
				if (G > 255)	G = 255;
				if (G < 0)		G = 0;
				if (B > 255)	B = 255;
				if (B < 0)		B = 0;
			}

			if (Y >= 200)
			{
				R = G = B = 255;
			}

			// ��д�Ҷ�ֵ
			*(pSrc) = B;
			*(pSrc + 1) = G;
			*(pSrc + 2) = R;
		}
	}
	
	EndWaitCursor();
}

