// ************************************************************************
//  �ļ�����Morph.cpp
//
//   ��ѧ��̬ѧ�����⣺
//
//  VErosion()				- ��ͼ����д�ֱ��ʴ
//  HErosion()				- ��ͼ�����ˮƽ��ʴ
//	VDilation()				- ��ͼ����д�ֱ����
//	HDilation()				- ��ͼ�����ˮƽ����
//	VHErosion()				- ��ͼ�����ȫ����ʴ
//	VHDilation()			- ��ͼ�����ȫ��������
//	OpenOperate()			- ��ͼ����п����㴦��
//	CloseOperate()			- ��ͼ����б����㴦��
//	Thin()					- ��ͼ�����ϸ������
//	Thick()					- ��ͼ����дֻ�����
//	MAT()					- ��ͼ���������任
//
// ************************************************************************

#include "stdafx.h"
#include "dip_system.h"
#include "Morph.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMorph

CMorph::CMorph()
{
}

CMorph::CMorph(CDIB* pDIB)
{
	m_pDIB = pDIB;
}

CMorph::~CMorph()
{
}

BEGIN_MESSAGE_MAP(CMorph, CWnd)
	//{{AFX_MSG_MAP(CMorph)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMorph message handlers

/*************************************************************************
 *
 * �������ƣ�
 *   VErosion()
 *
 * ����:
 *   HDIB hDIB          - �������DIB
 *
 * ����ֵ:
 *   void				- �޷���ֵ
 *
 * ˵��:
 *   �ú�����ͼ����д�ֱ�����ϵĸ�ʴ
 *
 ************************************************************************/

void CMorph::VErosion()
{
	//ѭ������
	LONG i;
	LONG j;
	LONG n;

	// ָ��Դͼ���ָ��
	BYTE*	lpSrc;
	
	// ָ�򻺴�ͼ���ָ��
	BYTE*	lpDst;

	// ָ�򻺴�DIBͼ���ָ��
	BYTE*	lpNewDIBBits;
	HLOCAL	hNewDIBBits;

	BYTE* lpDIBBits = m_pDIB->FindAddress();
	
	CHECK_EDITABLE(m_pDIB->GetImage());
	
	BeginWaitCursor();
	
	LONG lWidth = m_pDIB->Width();
	
	LONG lHeight = m_pDIB->Height();
	
	LONG lLineBytes = m_pDIB->RowLen();

	// ��ʱ�����ڴ棬�Ա�����ͼ��
	hNewDIBBits = LocalAlloc(LHND, lLineBytes * lHeight);

	if (hNewDIBBits == NULL)
	{
		// �����ڴ�ʧ��
		return;
	}
	
	// �����ڴ�
	lpNewDIBBits = (BYTE*)LocalLock(hNewDIBBits);

	// ��ʼ���·�����ڴ棬�趨��ʼֵΪ255
	lpDst = (BYTE*)lpNewDIBBits;
	memset(lpDst, (BYTE)255, lLineBytes * lHeight);

	//ʹ�ô�ֱ����ĽṹԪ�ؽ��и�ʴ
	for(i = 1; i < lHeight - 1; i++)
	{
		for(j = 0; j < lLineBytes; j += 3)
		{
			//����ʹ��3��1�ĽṹԪ�أ�Ϊ��ֹԽ�磬���Բ��������ϱߺ����±ߵ���������
			// ָ��Դͼ������j�У���i�����ص�ָ��			
			lpSrc = (BYTE *)(lpDIBBits + lLineBytes * i + j);

			// ָ��Ŀ��ͼ������j�У���i�����ص�ָ��			
			lpDst = (BYTE *)(lpNewDIBBits + lLineBytes * i + j);

			//Ŀ��ͼ���еĵ�ǰ���ȸ��ɺ�ɫ
			*lpDst = 0;
			*(lpDst + 1) = 0;
			*(lpDst + 2) = 0;

			// ���Դͼ����(0,-1)��(0,0)��(0,1)������֮һ�а׵㣬
			// ��Ŀ��ͼ���е�(0,0)�㸳�ɰ�ɫ
			for (n = 0; n < 3; n++)
			{
				if (*(lpSrc + (n - 1) * lLineBytes) > 128)
				{
					*lpDst = 255;
					*(lpDst + 1) = 255;
					*(lpDst + 2) = 255;
					break;
				}
			}				
		}
	}

	// ���Ƹ�ʴ���ͼ��
	memcpy(lpDIBBits, lpNewDIBBits, lLineBytes * lHeight);

	// �ͷ��ڴ�
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);

	EndWaitCursor();
}

/*************************************************************************
 *
 * �������ƣ�
 *   HErosion()
 *
 * ����:
 *   HDIB hDIB          - �������DIB
 *
 * ����ֵ:
 *   void				- �޷���ֵ
 *
 * ˵��:
 *   �ú�����ͼ�����ˮƽ�����ϵĸ�ʴ
 *
 ************************************************************************/

void CMorph::HErosion()
{
	//ѭ������
	LONG i;
	LONG j;
	LONG n;

	// ָ��Դͼ���ָ��
	BYTE*	lpSrc;
	
	// ָ�򻺴�ͼ���ָ��
	BYTE*	lpDst;

	// ָ�򻺴�DIBͼ���ָ��
	BYTE*	lpNewDIBBits;
	HLOCAL	hNewDIBBits;

	BYTE* lpDIBBits = m_pDIB->FindAddress();
	
	CHECK_EDITABLE(m_pDIB->GetImage());
	
	BeginWaitCursor();
	
	LONG lWidth = m_pDIB->Width();
	
	LONG lHeight = m_pDIB->Height();
	
	LONG lLineBytes = m_pDIB->RowLen();

	// ��ʱ�����ڴ棬�Ա�����ͼ��
	hNewDIBBits = LocalAlloc(LHND, lLineBytes * lHeight);

	if (hNewDIBBits == NULL)
	{
		// �����ڴ�ʧ��
		return;
	}
	
	// �����ڴ�
	lpNewDIBBits = (BYTE*)LocalLock(hNewDIBBits);

	// ��ʼ���·�����ڴ棬�趨��ʼֵΪ255
	lpDst = (BYTE*)lpNewDIBBits;
	memset(lpDst, (BYTE)255, lLineBytes * lHeight);

	// ʹ��ˮƽ����ĽṹԪ�ؽ��и�ʴ
	for (i = 0; i < lHeight; i++)
	{
		for (j = 3; j < lLineBytes - 3; j += 3)
		{
			// ����ʹ��1��3�ĽṹԪ�أ�Ϊ��ֹԽ�磬���Բ���������ߺ����ұߵ���������
			// ָ��Դͼ������j�У���i�����ص�ָ��			
			lpSrc = (BYTE *)(lpDIBBits + lLineBytes * i + j);

			// ָ��Ŀ��ͼ������j�У���i�����ص�ָ��			
			lpDst = (BYTE *)(lpNewDIBBits + lLineBytes * i + j);

			// Ŀ��ͼ���еĵ�ǰ���ȸ��ɺ�ɫ
			*lpDst = 0;
			*(lpDst + 1) = 0;
			*(lpDst + 2) = 0;
 
			// ���Դͼ����(-1,0)��(0,0)��(1,0)������֮һ�а׵㣬
			// ��Ŀ��ͼ���е�(0,0)�㸳�ɰ�ɫ
			for (n = 0; n < 3; n++)
			{
				if (*(lpSrc + (n - 1) * 3) > 128)
				{
					*lpDst = 255;
					*(lpDst + 1) = 255;
					*(lpDst + 2) = 255;
					break;
				}
			}				
		}
	}
	// ���Ƹ�ʴ���ͼ��
	memcpy(lpDIBBits, lpNewDIBBits, lLineBytes * lHeight);

	// �ͷ��ڴ�
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);

	EndWaitCursor();
}

/*************************************************************************
 *
 * �������ƣ�
 *   VDilation()
 *
 * ����:
 *   HDIB hDIB          - �������DIB
 *
 * ����ֵ:
 *   void				- �޷���ֵ
 *
 * ˵��:
 *   �ú�����ͼ����д�ֱ�����ϵ�����
 *
 ************************************************************************/

void CMorph::VDilation()
{
	//ѭ������
	LONG i;
	LONG j;
	LONG n;

	// ָ��Դͼ���ָ��
	BYTE*	lpSrc;
	
	// ָ�򻺴�ͼ���ָ��
	BYTE*	lpDst;

	// ָ�򻺴�DIBͼ���ָ��
	BYTE*	lpNewDIBBits;
	HLOCAL	hNewDIBBits;

	BYTE* lpDIBBits = m_pDIB->FindAddress();
	
	CHECK_EDITABLE(m_pDIB->GetImage());
	
	BeginWaitCursor();
	
	LONG lWidth = m_pDIB->Width();
	
	LONG lHeight = m_pDIB->Height();
	
	LONG lLineBytes = m_pDIB->RowLen();

	// ��ʱ�����ڴ棬�Ա�����ͼ��
	hNewDIBBits = LocalAlloc(LHND, lLineBytes * lHeight);

	if (hNewDIBBits == NULL)
	{
		// �����ڴ�ʧ��
		return;
	}
	
	// �����ڴ�
	lpNewDIBBits = (BYTE*)LocalLock(hNewDIBBits);

	// ��ʼ���·�����ڴ棬�趨��ʼֵΪ255
	lpDst = (BYTE*)lpNewDIBBits;
	memset(lpDst, (BYTE)255, lLineBytes * lHeight);

	//ʹ�ô�ֱ����ĽṹԪ�ؽ�������
	for(i = 1; i < lHeight - 1; i++)
	{
		for(j = 0; j < lLineBytes; j += 3)
		{
			//����ʹ��3��1�ĽṹԪ�أ�Ϊ��ֹԽ�磬���Բ��������ϱߺ����±ߵ���������
			// ָ��Դͼ������j�У���i�����ص�ָ��			
			lpSrc = (BYTE *)(lpDIBBits + lLineBytes * i + j);

			// ָ��Ŀ��ͼ������j�У���i�����ص�ָ��			
			lpDst = (BYTE *)(lpNewDIBBits + lLineBytes * i + j);

			//Ŀ��ͼ���еĵ�ǰ���ȸ��ɰ�ɫ
			*lpDst = 255;
			*(lpDst + 1) = 255;
			*(lpDst + 2) = 255;

			// ���Դͼ����(0,-1)��(0,0)��(0,1)������֮һ�кڵ㣬
			// ��Ŀ��ͼ���е�(0,0)�㸳�ɺ�ɫ
			for (n = 0; n < 3; n++)
			{
				if (*(lpSrc + (n - 1) * lLineBytes) < 128)
				{
					*lpDst = 0;
					*(lpDst + 1) = 0;
					*(lpDst + 2) = 0;
					break;
				}
			}				
		}
	}

	// ���Ƹ�ʴ���ͼ��
	memcpy(lpDIBBits, lpNewDIBBits, lLineBytes * lHeight);

	// �ͷ��ڴ�
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);

	EndWaitCursor();
}

/*************************************************************************
 *
 * �������ƣ�
 *   HDilation()
 *
 * ����:
 *   HDIB hDIB          - �������DIB
 *
 * ����ֵ:
 *   void				- �޷���ֵ
 *
 * ˵��:
 *   �ú�����ͼ�����ˮƽ�����ϵ�����
 *
 ************************************************************************/

void CMorph::HDilation()
{
	//ѭ������
	LONG i;
	LONG j;
	LONG n;
	
	// ָ��Դͼ���ָ��
	BYTE*	lpSrc;
	
	// ָ�򻺴�ͼ���ָ��
	BYTE*	lpDst;

	// ָ�򻺴�DIBͼ���ָ��
	BYTE*	lpNewDIBBits;
	HLOCAL	hNewDIBBits;

	BYTE* lpDIBBits = m_pDIB->FindAddress();
	
	CHECK_EDITABLE(m_pDIB->GetImage());
	
	BeginWaitCursor();
	
	LONG lWidth = m_pDIB->Width();
	
	LONG lHeight = m_pDIB->Height();
	
	LONG lLineBytes = m_pDIB->RowLen();

	// ��ʱ�����ڴ棬�Ա�����ͼ��
	hNewDIBBits = LocalAlloc(LHND, lLineBytes * lHeight);

	if (hNewDIBBits == NULL)
	{
		// �����ڴ�ʧ��
		return;
	}
	
	// �����ڴ�
	lpNewDIBBits = (BYTE*)LocalLock(hNewDIBBits);

	// ��ʼ���·�����ڴ棬�趨��ʼֵΪ255
	lpDst = (BYTE*)lpNewDIBBits;
	memset(lpDst, (BYTE)255, lLineBytes * lHeight);

	// ʹ��ˮƽ����ĽṹԪ�ؽ�������
	for (i = 0; i < lHeight; i++)
	{
		for (j = 3; j < lLineBytes - 3; j += 3)
		{
			// ����ʹ��1��3�ĽṹԪ�أ�Ϊ��ֹԽ�磬���Բ���������ߺ����ұߵ���������
			// ָ��Դͼ������j�У���i�����ص�ָ��			
			lpSrc = (BYTE *)(lpDIBBits + lLineBytes * i + j);

			// ָ��Ŀ��ͼ������j�У���i�����ص�ָ��			
			lpDst = (BYTE *)(lpNewDIBBits + lLineBytes * i + j);

			// Ŀ��ͼ���еĵ�ǰ���ȸ��ɰ�ɫ
			*lpDst = 255;
			*(lpDst + 1) = 255;
			*(lpDst + 2) = 255;
 
			// ���Դͼ����(-1,0)��(0,0)��(1,0)������֮һ�кڵ㣬
			// ��Ŀ��ͼ���е�(0,0)�㸳�ɺ�ɫ
			for (n = 0; n < 3; n++)
			{
				if (*(lpSrc + (n - 1) * 3) < 128)
				{
					*lpDst = 0;
					*(lpDst + 1) = 0;
					*(lpDst + 2) = 0;
					break;
				}
			}				
		}
	}

	// ���Ƹ�ʴ���ͼ��
	memcpy(lpDIBBits, lpNewDIBBits, lLineBytes * lHeight);

	// �ͷ��ڴ�
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);

	EndWaitCursor();
}

/*************************************************************************
 *
 * �������ƣ�
 *   VHDilation()
 *
 * ����:
 *   HDIB hDIB          - �������DIB
 *
 * ����ֵ:
 *   void				- �޷���ֵ
 *
 * ˵��:
 *   �ú�����ͼ�����ȫ��������
 *
 ************************************************************************/

void CMorph::VHDilation()
{
	//ѭ������
	LONG i;
	LONG j;
	LONG m;
	LONG n;

	// ָ��Դͼ���ָ��
	BYTE*	lpSrc;
	
	// ָ�򻺴�ͼ���ָ��
	BYTE*	lpDst;

	// ָ�򻺴�DIBͼ���ָ��
	BYTE*	lpNewDIBBits;
	HLOCAL	hNewDIBBits;

	BYTE* lpDIBBits = m_pDIB->FindAddress();
	
	CHECK_EDITABLE(m_pDIB->GetImage());
	
	BeginWaitCursor();
	
	LONG lWidth = m_pDIB->Width();
	
	LONG lHeight = m_pDIB->Height();
	
	LONG lLineBytes = m_pDIB->RowLen();

	// ��ʱ�����ڴ棬�Ա�����ͼ��
	hNewDIBBits = LocalAlloc(LHND, lLineBytes * lHeight);

	if (hNewDIBBits == NULL)
	{
		// �����ڴ�ʧ��
		return;
	}
	
	// �����ڴ�
	lpNewDIBBits = (BYTE*)LocalLock(hNewDIBBits);

	// ��ʼ���·�����ڴ棬�趨��ʼֵΪ255
	lpDst = (BYTE*)lpNewDIBBits;
	memset(lpDst, (BYTE)255, lLineBytes * lHeight);

	// 3��3�ĽṹԪ��
	int T[9] = {0, 1, 0,
				1, 1, 1,
				0, 1, 0};

	// ʹ��ˮƽ����ĽṹԪ�ؽ��и�ʴ
	for (i = 1; i < lHeight - 1; i++)
	{
		for (j = 3; j < lLineBytes - 3; j += 3)
		{
			// ����ʹ��3��3�ĽṹԪ�أ�Ϊ��ֹԽ�磬���Բ����������ҡ��ϡ����ıߵ�����
			
			// ָ��Դͼ������j�У���i�����ص�ָ��			
			lpSrc = (BYTE *)(lpDIBBits + lLineBytes * i + j);

			// ָ��Ŀ��ͼ������j�У���i�����ص�ָ��			
			lpDst = (BYTE *)(lpNewDIBBits + lLineBytes * i + j);

			// Ŀ��ͼ���еĵ�ǰ���ȸ��ɰ�ɫ
			*lpDst = 255;
			*(lpDst + 1) = 255;
			*(lpDst + 2) = 255;
 
			// ���Դͼ����3��3�ṹԪ�ض�Ӧλ���кڵ�	
			// ��Ŀ��ͼ���е�(0,0)�㸳�ɺ�ɫ
			for (m = 0; m < 3; m++)
			{
				for (n = 0; n < 3; n++)
				{
					if (T[m * 3 + n] == 0)
						continue;

					if (*(lpSrc + (1 - m) * lLineBytes +(n - 1) * 3) < 128)
					{
						*lpDst = 0;
						*(lpDst + 1) = 0;
						*(lpDst + 2) = 0;
						break;
					}
				}
			}				
		}
	}
	// ���Ƹ�ʴ���ͼ��
	memcpy(lpDIBBits, lpNewDIBBits, lLineBytes * lHeight);

	// �ͷ��ڴ�
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);

	EndWaitCursor();
}

/*************************************************************************
 *
 * �������ƣ�
 *   VHErosion()
 *
 * ����:
 *   HDIB hDIB          - �������DIB
 *
 * ����ֵ:
 *   void				- �޷���ֵ
 *
 * ˵��:
 *   �ú�����ͼ�����ȫ����ʴ
 *
 ************************************************************************/
void CMorph::VHErosion()
{
	//ѭ������
	LONG i;
	LONG j;
	LONG m;
	LONG n;

	// ָ��Դͼ���ָ��
	BYTE*	lpSrc;
	
	// ָ�򻺴�ͼ���ָ��
	BYTE*	lpDst;

	// ָ�򻺴�DIBͼ���ָ��
	BYTE*	lpNewDIBBits;
	HLOCAL	hNewDIBBits;

	BYTE* lpDIBBits = m_pDIB->FindAddress();
	
	CHECK_EDITABLE(m_pDIB->GetImage());
	
	BeginWaitCursor();
	
	LONG lWidth = m_pDIB->Width();
	
	LONG lHeight = m_pDIB->Height();
	
	LONG lLineBytes = m_pDIB->RowLen();

	// ��ʱ�����ڴ棬�Ա�����ͼ��
	hNewDIBBits = LocalAlloc(LHND, lLineBytes * lHeight);

	if (hNewDIBBits == NULL)
	{
		// �����ڴ�ʧ��
		return;
	}
	
	// �����ڴ�
	lpNewDIBBits = (BYTE*)LocalLock(hNewDIBBits);

	// ��ʼ���·�����ڴ棬�趨��ʼֵΪ255
	lpDst = (BYTE*)lpNewDIBBits;
	memset(lpDst, (BYTE)255, lLineBytes * lHeight);

	// 3��3�ĽṹԪ��
	int T[9] = {0, 1, 0,
				1, 1, 1,
				0, 1, 0};

	// ʹ��ˮƽ����ĽṹԪ�ؽ��и�ʴ
	for (i = 1; i < lHeight - 1; i++)
	{
		for (j = 3; j < lLineBytes - 3; j += 3)
		{
			// ����ʹ��3��3�ĽṹԪ�أ�Ϊ��ֹԽ�磬���Բ����������ҡ��ϡ����ıߵ�����
			
			// ָ��Դͼ������j�У���i�����ص�ָ��			
			lpSrc = (BYTE *)(lpDIBBits + lLineBytes * i + j);

			// ָ��Ŀ��ͼ������j�У���i�����ص�ָ��			
			lpDst = (BYTE *)(lpNewDIBBits + lLineBytes * i + j);

			// Ŀ��ͼ���еĵ�ǰ���ȸ��ɺ�ɫ
			*lpDst = 0;
			*(lpDst + 1) = 0;
			*(lpDst + 2) = 0;
 
			// ���Դͼ����3��3�ṹԪ�ض�Ӧλ���а׵�	
			// ��Ŀ��ͼ���е�(0,0)�㸳�ɰ�ɫ
			for (m = 0; m < 3; m++)
			{
				for (n = 0; n < 3; n++)
				{
					if (T[m * 3 + n] == 0)
						continue;

					if (*(lpSrc + (1 - m) * lLineBytes +(n - 1) * 3) > 128)
					{
						*lpDst = 255;
						*(lpDst + 1) = 255;
						*(lpDst + 2) = 255;
						break;
					}
				}
			}				
		}
	}
	// ���Ƹ�ʴ���ͼ��
	memcpy(lpDIBBits, lpNewDIBBits, lLineBytes * lHeight);

	// �ͷ��ڴ�
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);

	EndWaitCursor();
}

/*************************************************************************
 *
 * �������ƣ�
 *   OpenOperate()
 *
 * ����:
 *   HDIB hDIB          - �������DIB
 *
 * ����ֵ:
 *   void				- �޷���ֵ
 *
 * ˵��:
 *   �ú�����ͼ����п����㴦��
 *
 ************************************************************************/

void CMorph::OpenOperate()
{
	VHErosion();
	VHDilation();
}

/*************************************************************************
 *
 * �������ƣ�
 *   CloseOperate()
 *
 * ����:
 *   HDIB hDIB          - �������DIB
 *
 * ����ֵ:
 *   void				- �޷���ֵ
 *
 * ˵��:
 *   �ú�����ͼ����б����㴦��
 *
 ************************************************************************/

void CMorph::CloseOperate()
{
	VHDilation();
	VHErosion();
}

/*************************************************************************
 *
 * �������ƣ�
 *   Thin()
 *
 * ����:
 *   HDIB hDIB          - �������DIB
 *
 * ����ֵ:
 *   void				- �޷���ֵ
 *
 * ˵��:
 *   �ú�����ͼ�����ϸ������
 *
 ************************************************************************/

void CMorph::Thin()
{
	
	LONG i;
	LONG j;
	LONG m;
	LONG n;

	// 5��5������������ֵ
	BYTE S[5][5];

	// ������
	BYTE nCount;

	// ѭ��������־
	BOOL bJump = TRUE;

	// ָ��Դͼ���ָ��
	BYTE*	lpSrc;
	
	// ָ�򻺴�ͼ���ָ��
	BYTE*	lpDst;

	// ָ�򻺴�DIBͼ���ָ��
	BYTE*	lpNewDIBBits;
	HLOCAL	hNewDIBBits;

	BYTE* lpDIBBits = m_pDIB->FindAddress();
	
	CHECK_EDITABLE(m_pDIB->GetImage());
	
	BeginWaitCursor();
	
	LONG lWidth = m_pDIB->Width();
	
	LONG lHeight = m_pDIB->Height();
	
	LONG lLineBytes = m_pDIB->RowLen();

	// ��ʱ�����ڴ棬�Ա�����ͼ��
	hNewDIBBits = LocalAlloc(LHND, lLineBytes * lHeight);

	if (hNewDIBBits == NULL)
	{
		// �����ڴ�ʧ��
		return;
	}
	
	// �����ڴ�
	lpNewDIBBits = (BYTE*)LocalLock(hNewDIBBits);

	// ��ʼ���·�����ڴ棬�趨��ʼֵΪ255
	lpDst = (BYTE*)lpNewDIBBits;
	memset(lpDst, (BYTE)255, lLineBytes * lHeight);
	
	while (bJump)
	{
		bJump = FALSE;
		
		// ��ʼ���·�����ڴ棬�趨��ʼֵΪ255
		lpDst = (BYTE*)lpNewDIBBits;
		memset(lpDst, (BYTE)255, lLineBytes * lHeight);

		// ����ʹ��5��5�ĽṹԪ�أ�Ϊ��Խ�磬��������Χ��2�С�2������
		for (i = 2; i < lHeight - 2; i++)
		{
			for (j = 2 * 3; j < lLineBytes - 2 * 3; j += 3)
			{
				// ָ��Դͼ������j�У���i�����ص�ָ��			
				lpSrc = (BYTE*)(lpDIBBits + lLineBytes * i + j);

				// ָ��Ŀ��ͼ������j�У���i�����ص�ָ��			
				lpDst = (BYTE*)(lpNewDIBBits + lLineBytes * i + j);

				// ���Դͼ���е�ǰ��Ϊ��ɫ��������
				if (*lpSrc > 127)
					continue;

				// ��õ�ǰ�����ڵ�5��5����������ֵ����ɫ��0������ɫ��1����
				for (m = 0; m < 5; m++)
				{
					for (n = 0; n < 5; n++)
					{
						if (*(lpSrc + (2 - m) * lLineBytes + (n - 2) * 3) > 127)
							S[m][n] = 0;
						else
							S[m][n] = 1;
					}
				}

				// �ж�����һ�Ƿ������
				nCount =  S[1][1] + S[1][2] + S[1][3] + S[2][1] 
						+ S[2][3] + S[3][1]	+ S[3][2] + S[3][3];

				if (nCount < 2 || nCount >6)
				{
					*lpDst = 0;
					*(lpDst + 1) = 0;
					*(lpDst + 2) = 0;
					continue;
				}

				// �ж��������Ƿ������
				nCount = 0;
				
				if (S[1][2] == 0 && S[1][1] == 1) 
					nCount++;
				if (S[1][1] == 0 && S[2][1] == 1)
					nCount++;
				if (S[2][1] == 0 && S[3][1] == 1)
					nCount++;
				if (S[3][1] == 0 && S[3][2] == 1)
					nCount++;
				if (S[3][2] == 0 && S[3][3] == 1)
					nCount++;
				if (S[3][3] == 0 && S[2][3] == 1)
					nCount++;
				if (S[2][3] == 0 && S[1][3] == 1)
					nCount++;
				if (S[1][3] == 0 && S[1][2] == 1)
					nCount++;

				if (nCount != 1)
				{
					*lpDst = 0;
					*(lpDst + 1) = 0;
					*(lpDst + 2) = 0;
					continue;
				}

				// �ж��������Ƿ������
				if (S[1][2] * S[2][1] * S[2][3] != 0)
				{
					nCount = 0;
					if (S[0][2] == 0 && S[0][1] == 1)
						nCount++;
					if (S[0][1] == 0 && S[1][1] == 1)
						nCount++;
					if (S[1][1] == 0 && S[2][1] == 1)
						nCount++;
					if (S[2][1] == 0 && S[2][2] == 1)
						nCount++;
					if (S[2][2] == 0 && S[2][3] == 1)
						nCount++;
					if (S[2][3] == 0 && S[1][3] == 1)
						nCount++;
					if (S[1][3] == 0 && S[0][3] == 1)
						nCount++;
					if (S[0][3] == 0 && S[0][2] == 1)
						nCount++;

					if (nCount == 1)
					{
						*lpDst = 0;
						*(lpDst + 1) = 0;
						*(lpDst + 2) = 0;
						continue;
					}
				}

				// �ж��������Ƿ������
				if (S[1][2] * S[2][1] * S[3][2] != 0)
				{
					nCount = 0;
					if (S[1][1] == 0 && S[1][0] == 1)
						nCount++;
					if (S[1][0] == 0 && S[2][0] == 1)
						nCount++;
					if (S[2][0] == 0 && S[3][0] == 1)
						nCount++;
					if (S[3][0] == 0 && S[3][1] == 1)
						nCount++;
					if (S[3][1] == 0 && S[3][2] == 1)
						nCount++;
					if (S[3][2] == 0 && S[2][2] == 1)
						nCount++;
					if (S[2][2] == 0 && S[1][2] == 1)
						nCount++;
					if (S[1][2] == 0 && S[1][1] == 1)
						nCount++;
					if (nCount == 1)
					{
						*lpDst = 0;
						*(lpDst + 1) = 0;
						*(lpDst + 2) = 0;
						continue;
					}
				}

				// ���������������ɾ���õ�
				*lpDst = 255;
				*(lpDst + 1) = 255;
				*(lpDst + 2) = 255;
				bJump = TRUE;
			}
		}

		// ���Ƹ�ʴ���ͼ��
		memcpy(lpDIBBits, lpNewDIBBits, lLineBytes * lHeight);
	}
	
	// ���Ƹ�ʴ���ͼ��
	memcpy(lpDIBBits, lpNewDIBBits, lLineBytes * lHeight);

	// �ͷ��ڴ�
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);

	EndWaitCursor();
}

/*************************************************************************
 *
 * �������ƣ�
 *   Thick() 
 * 
 * ����:
 *   HDIB hDIB          - �������DIB
 *
 * ����ֵ:
 *   void				- �޷���ֵ
 *
 * ˵��:
 *   �ú�����ͼ����дֻ�����
 *
 ************************************************************************/

void CMorph::Thick()
{
	
	LONG i;
	LONG j;
	
	CHECK_EDITABLE(m_pDIB->GetImage());
	
	BeginWaitCursor();
	
	LONG lWidth = m_pDIB->Width();
	LONG lHeight = m_pDIB->Height();
	LONG lLineBytes = m_pDIB->RowLen();
	int nChannel = m_pDIB->Channel();
	BYTE* lpDIBBits = m_pDIB->FindAddress();
	
	// �Ը����ؽ��лҶ�ת��
	for (i = 0; i < lHeight; i ++)
	{
		for (j = 0; j < lWidth; j ++)
		{
			BYTE *pSrc = lpDIBBits + nChannel * j + i * lLineBytes;
			// �����ظ���ɫ�������ж�ֵ���󲹴���
			if (*(pSrc + 2) > 127) *(pSrc + 2) = 0;
			else                   *(pSrc + 2) = 255;
			if (*(pSrc + 1) > 127) *(pSrc + 1) = 0;
			else                   *(pSrc + 1) = 255;
			if (*pSrc > 127)       *pSrc = 0;
			else                   *pSrc = 255;
		}
	}

	// ���󲹺��ٶ�ͼ�����ϸ��
	Thin();

	EndWaitCursor();
}

/*************************************************************************
 *
 * �������ƣ�
 *   MAT() 
 * 
 * ����:
 *   HDIB hDIB          - �������DIB
 *
 * ����ֵ:
 *   void				- �޷���ֵ
 *
 * ˵��:
 *   �ú�����ͼ���������任
 *
 ************************************************************************/

void CMorph::MAT()
{
	 
	LONG i;
	LONG j;
	LONG m;
	LONG n;

	// 5��5������������ֵ
	BYTE S[5][5];

	// ������
	BYTE nCount;

	// ѭ��������־ 
	BOOL bJump = TRUE;

	// ָ��Դͼ���ָ��
	BYTE*	lpSrc;
	
	// ָ�򻺴�ͼ���ָ��
	BYTE*	lpDst;

	// ָ�򻺴�DIBͼ���ָ��
	BYTE*	lpNewDIBBits;
	HLOCAL	hNewDIBBits;

	BYTE* lpDIBBits = m_pDIB->FindAddress();
	
	CHECK_EDITABLE(m_pDIB->GetImage());
	
	BeginWaitCursor();
	
	LONG lWidth = m_pDIB->Width();
	
	LONG lHeight = m_pDIB->Height();
	
	LONG lLineBytes = m_pDIB->RowLen();

	// ��ʱ�����ڴ棬�Ա�����ͼ��
	hNewDIBBits = LocalAlloc(LHND, lLineBytes * lHeight);

	if (hNewDIBBits == NULL)
	{
		// �����ڴ�ʧ��
		return;
	}
	
	// �����ڴ�	
	lpNewDIBBits = (BYTE*)LocalLock(hNewDIBBits);

	// ��ʼ���·�����ڴ棬�趨��ʼֵΪ255
	lpDst = (BYTE*)lpNewDIBBits;
	memset(lpDst, (BYTE)255, lLineBytes * lHeight);
	
	while (bJump)
	{
		bJump = FALSE;
		
		// ��ʼ���·�����ڴ棬�趨��ʼֵΪ255
		lpDst = (BYTE*)lpNewDIBBits;
		memset(lpDst, (BYTE)255, lLineBytes * lHeight);

		// ����ʹ��5��5�ĽṹԪ�أ�Ϊ��Խ�磬��������Χ��2�С�2������
		for (i = 2; i < lHeight - 2; i++)
		{
			for (j = 2 * 3; j < lLineBytes - 2 * 3; j += 3)
			{
				// ָ��Դͼ������j�У���i�����ص�ָ��			
				lpSrc = (BYTE*)(lpDIBBits + lLineBytes * i + j);

				// ָ��Ŀ��ͼ������j�У���i�����ص�ָ��			
				lpDst = (BYTE*)(lpNewDIBBits + lLineBytes * i + j);

				// ���Դͼ���е�ǰ��Ϊ��ɫ��������
				if (*lpSrc > 127)
					continue;

				// ��õ�ǰ�����ڵ�5��5����������ֵ����ɫ��0������ɫ��1����
				for (m = 0; m < 5; m++)
				{
					for (n = 0; n < 5; n++)
					{
						if (*(lpSrc + (2 - m) * lLineBytes + (n - 2) * 3) > 127)
							S[m][n] = 0;
						else
							S[m][n] = 1;
					}
				}

				// �ж�����1-1�Ƿ������
				nCount =  S[1][1] + S[1][2] + S[1][3] + S[2][1] 
						+ S[2][3] + S[3][1]	+ S[3][2] + S[3][3];

				if (nCount < 2 || nCount >6)
				{
					*lpDst = 0;
					*(lpDst + 1) = 0;
					*(lpDst + 2) = 0;
					continue;
				}

				// �ж�����1-2�Ƿ������
				nCount = 0;
				
				if (S[1][2] == 0 && S[1][1] == 1) 
					nCount++;
				if (S[1][1] == 0 && S[2][1] == 1)
					nCount++;
				if (S[2][1] == 0 && S[3][1] == 1)
					nCount++;
				if (S[3][1] == 0 && S[3][2] == 1)
					nCount++;
				if (S[3][2] == 0 && S[3][3] == 1)
					nCount++;
				if (S[3][3] == 0 && S[2][3] == 1)
					nCount++;
				if (S[2][3] == 0 && S[1][3] == 1)
					nCount++;
				if (S[1][3] == 0 && S[1][2] == 1)
					nCount++;

				if (nCount != 1)
				{
					*lpDst = 0;
					*(lpDst + 1) = 0;
					*(lpDst + 2) = 0;
					continue;
				}

				// �ж�����1-3�Ƿ������
				if (S[1][2] * S[2][1] * S[3][2] != 0)
				{
					*lpDst = 0;
					*(lpDst + 1) = 0;
					*(lpDst + 2) = 0;
					continue;
				}

				// �ж�����1-4�Ƿ������
				if (S[2][1] * S[3][2] * S[2][3] != 0)
				{
					*lpDst = 0;
					*(lpDst + 1) = 0;
					*(lpDst + 2) = 0;
					continue;
				}

				// ���������������ɾ���õ�
				*lpDst = 255;
				*(lpDst + 1) = 255;
				*(lpDst + 2) = 255;
			}
		}

		// ����ʹ��5��5�ĽṹԪ�أ�Ϊ��Խ�磬��������Χ��2�С�2������
		for (i = 2; i < lHeight - 2; i++)
		{
			for (j = 2 * 3; j < lLineBytes - 2 * 3; j += 3)
			{
				// ָ��Դͼ������j�У���i�����ص�ָ��			
				lpSrc = (BYTE*)(lpDIBBits + lLineBytes * i + j);

				// ָ��Ŀ��ͼ������j�У���i�����ص�ָ��			
				lpDst = (BYTE*)(lpNewDIBBits + lLineBytes * i + j);

				// ���Դͼ���е�ǰ��Ϊ��ɫ��������
				if (*lpSrc > 127)
					continue;

				// ��õ�ǰ�����ڵ�5��5����������ֵ����ɫ��0������ɫ��1����
				for (m = 0; m < 5; m++)
				{
					for (n = 0; n < 5; n++)
					{
						if (*(lpSrc + (2 - m) * lLineBytes + (n - 2) * 3) > 127)
							S[m][n] = 0;
						else
							S[m][n] = 1;
					}
				}

				// �ж�����2-1�Ƿ������
				nCount =  S[1][1] + S[1][2] + S[1][3] + S[2][1] 
						+ S[2][3] + S[3][1]	+ S[3][2] + S[3][3];

				if (nCount < 2 || nCount >6)
				{
					*lpDst = 0;
					*(lpDst + 1) = 0;
					*(lpDst + 2) = 0;
					continue;
				}

				// �ж�����2-2�Ƿ������
				nCount = 0;
				
				if (S[1][2] == 0 && S[1][1] == 1) 
					nCount++;
				if (S[1][1] == 0 && S[2][1] == 1)
					nCount++;
				if (S[2][1] == 0 && S[3][1] == 1)
					nCount++;
				if (S[3][1] == 0 && S[3][2] == 1)
					nCount++;
				if (S[3][2] == 0 && S[3][3] == 1)
					nCount++;
				if (S[3][3] == 0 && S[2][3] == 1)
					nCount++;
				if (S[2][3] == 0 && S[1][3] == 1)
					nCount++;
				if (S[1][3] == 0 && S[1][2] == 1)
					nCount++;

				if (nCount != 1)
				{
					*lpDst = 0;
					*(lpDst + 1) = 0;
					*(lpDst + 2) = 0;
					continue;
				}

				// �ж�����2-3�Ƿ������
				if (S[1][2] * S[2][1] * S[2][3] != 0)
				{
					*lpDst = 0;
					*(lpDst + 1) = 0;
					*(lpDst + 2) = 0;
					continue;
				}

				// �ж�����2-4�Ƿ������
				if (S[1][2] * S[3][2] * S[2][3] != 0)
				{
					*lpDst = 0;
					*(lpDst + 1) = 0;
					*(lpDst + 2) = 0;
					continue;
				}

				// ���������������ɾ���õ�
				*lpDst = 255;
				*(lpDst + 1) = 255;
				*(lpDst + 2) = 255;
				bJump = TRUE;
			}
		}

		// ���Ƹ�ʴ���ͼ��
		memcpy(lpDIBBits, lpNewDIBBits, lLineBytes * lHeight);
	}
	
	// ���Ƹ�ʴ���ͼ��
	memcpy(lpDIBBits, lpNewDIBBits, lLineBytes * lHeight);

	// �ͷ��ڴ�
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);

	EndWaitCursor();
}


