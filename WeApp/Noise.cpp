// ************************************************************************
//  �ļ�����Noise.cpp
//
//  ����ģ�ͺ����⣺
//
//  RandomNoise()			- �������ģ��
//	SaltNoise()				- ��������ģ��
//
// ************************************************************************

#include "stdafx.h"
#include "dip_system.h"
#include "Noise.h"
#include "math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNoise

CNoise::CNoise()
{
}

CNoise::CNoise(CDIB* pDIB)
{
	m_pDIB = pDIB;
}

CNoise::~CNoise()
{
}

BEGIN_MESSAGE_MAP(CNoise, CWnd)
	//{{AFX_MSG_MAP(CNoise)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNoise message handlers

/*************************************************************************
 *
 * �������ƣ�
 *   RandomNoise()
 *
 * ����:
 *   HDIB hDIB          - �������DIB
 *
 * ����ֵ:
 *   HDIB				- �µ�DIB
 *
 * ˵��:
 *   �ú�����ͼ�������������ģ��
 *
 ************************************************************************/

void CNoise::RandomNoise()
{
	
	LONG i;
	LONG j;

	BYTE* lpDIBBits = m_pDIB->FindAddress();
	
	CHECK_EDITABLE(m_pDIB->GetImage());
	
	BeginWaitCursor();
	
	LONG lWidth = m_pDIB->Width();
	
	LONG lHeight = m_pDIB->Height();

	LONG lLineBytes = m_pDIB->RowLen();
	
	// ����α�������
	srand((unsigned)time(NULL));

	// �Ը����ؽ��лҶ�ת��
	for (i = 0; i < lHeight; i ++)
	{
		for (j = 0; j < lLineBytes; j ++)
		{
			// ȡ���ֵ
			LONG Temp = rand();

			// �����ظ���ɫ��������������봦��
			BYTE R = *((BYTE *)lpDIBBits + lLineBytes * i + j);
			*((BYTE *)lpDIBBits + lLineBytes * i + j) = (BYTE)(R * 224 / 256 + Temp / 1024);
			j++;
			BYTE G = *((BYTE *)lpDIBBits + lLineBytes * i + j);
			*((BYTE *)lpDIBBits + lLineBytes * i + j) = (BYTE)(G * 224 / 256 + Temp / 1024);
			j++;
			BYTE B = *((BYTE *)lpDIBBits + lLineBytes * i + j);
			*((BYTE *)lpDIBBits + lLineBytes * i + j) = (BYTE)(B * 224 / 256 + Temp / 1024);
		}
	}

	EndWaitCursor();
}

/*************************************************************************
 *
 * �������ƣ�
 *   SaltNoise()
 *
 * ����:
 *   HDIB hDIB          - �������DIB
 *
 * ����ֵ:
 *   HDIB				- �µ�DIB
 *
 * ˵��:
 *   �ú�����ͼ��������������ģ��
 *
 ************************************************************************/

void CNoise::SaltNoise()
{
	
	LONG i;
	LONG j;

	BYTE* lpDIBBits = m_pDIB->FindAddress();
	
	CHECK_EDITABLE(m_pDIB->GetImage());
	
	BeginWaitCursor();
	
	LONG lWidth = m_pDIB->Width();
	
	LONG lHeight = m_pDIB->Height();

	LONG lLineBytes = m_pDIB->RowLen();
	
	// ����α�������
	srand((unsigned)time(NULL));

	// �Ը����ؽ��лҶ�ת��
	for (i = 0; i < lHeight; i ++)
	{
		for (j = 0; j < lLineBytes; j ++)
		{
			// ȡ���ֵ
			if (rand() > 32500)
			{
				// �����ظ���ɫ��������������봦��
				*((BYTE *)lpDIBBits + lLineBytes * i + j) = 0;
				j++;
				*((BYTE *)lpDIBBits + lLineBytes * i + j) = 0;
				j++;
				*((BYTE *)lpDIBBits + lLineBytes * i + j) = 0;
			}
		}
	}

	EndWaitCursor();
}


