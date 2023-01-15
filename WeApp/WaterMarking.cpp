// ************************************************************************
//  �ļ�����WaterMarking.cpp
//
//   ˮӡ��Ӻ����⣺
//
//  Set()				- ˮӡǶ�뺯��
//  Get()				- ˮӡ��⺯��
//
// ************************************************************************

#include "stdafx.h"
#include "dip_system.h"
#include "WaterMarking.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WIDTH	100
#define HEIGHT	100

/////////////////////////////////////////////////////////////////////////////
// CWaterMarking

CWaterMarking::CWaterMarking()
{
}

CWaterMarking::CWaterMarking(CDIB* pDIB)
{
	m_pDIB = pDIB;
}

CWaterMarking::~CWaterMarking()
{
}

BEGIN_MESSAGE_MAP(CWaterMarking, CWnd)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWaterMarking message handlers

// �ú���ΪDIBͼ�����ˮӡ
void CWaterMarking::Set()
{
	CHECK_EDITABLE(m_pDIB->GetImage());

	BeginWaitCursor();
		
	LONG lWidth = m_pDIB->Width();
	LONG lHeight = m_pDIB->Height();
	LONG lLineBytes = m_pDIB->RowLen();
	BYTE* lpDIBBits = m_pDIB->FindAddress();
	
	// ��ˮӡ�ļ���ȡ�����Ϣ
	CFileDialog hFileDlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_READONLY, TEXT("�����ļ�(*.*)|*.* | ˮӡ�ļ�(*.bmp)|*.bmp||"), NULL);
	hFileDlg.m_ofn.nFilterIndex = 2;
	hFileDlg.m_ofn.hwndOwner = m_hWnd;
	hFileDlg.m_ofn.lStructSize = sizeof(OPENFILENAME);
	hFileDlg.m_ofn.lpstrTitle = TEXT("��ˮӡ�ļ�...\0");
	hFileDlg.m_ofn.nMaxFile = MAX_PATH;

	if (hFileDlg.DoModal() != IDOK) return;
	CString strPath = hFileDlg.GetPathName();

	CFile file;
	file.Open(strPath, CFile::modeReadWrite);
	int nFileLen = file.GetLength();
	BYTE* lpBuf;
	lpBuf = new BYTE[nFileLen];
	nFileLen = file.Read(lpBuf, nFileLen);
	file.Close();

	if (lLineBytes * lHeight < 4 * nFileLen)
	{
		MessageBox(L"����λͼ̫С����������ˮӡ��", L"ϵͳ��ʾ" , MB_ICONINFORMATION | MB_OK);
		delete lpBuf;
		return;
	}

	// ������λʱ����λ����
	int move1[13] = {6, 5, 2, 0, 7, 4, 2, 1, -2, 6, 4, 3, 0};
	
	// �������ļ����ֽ���������
    BYTE mask1[13]={192, 32, 28, 3, 128, 112, 12, 2, 1, 192, 48, 8, 7};
	
	// λͼ�ļ����ֽ���������
	BYTE mask2[13]={252, 254, 248, 252, 254, 248, 252, 254, 251, 252, 252, 254, 248};
	
	// Ϊ1ʱpointer1ָ���һ
	int add1[13]={0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1};

	// Ϊ1ʱpointer2ָ���һ
	int add2[13]={1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1};

	// ��ʱָ��
	int pointer1 = 0;
	int pointer2 = 0;
	int pointer3 = 0;

	while(pointer1 <= nFileLen)
	{
		// ��ˮӡ������Ƕ�뵽����λͼ
		if (move1[pointer3] > 0)
			*(lpDIBBits + pointer2) = (*(lpDIBBits + pointer2) & mask2[pointer3])|((lpBuf[pointer1] & mask1[pointer3]) >> move1[pointer3]);
		else
			*(lpDIBBits + pointer2) = (*(lpDIBBits + pointer2) & mask2[pointer3])|((lpBuf[pointer1] & mask1[pointer3]) << move1[pointer3] * (-1));

		//����ָ��
		if (add1[pointer3] == 1)
			pointer1++;
		if (add2[pointer3] == 1)
			pointer2++;
		pointer3++;
		pointer3 %= 13;
	}
	delete lpBuf;//�ͷŻ���

	EndWaitCursor();
}

/*************************************************************************
 *
 * �������ƣ�
 *   Get() 
 * 
 * ����:
 *	 CFile& file		- ˮӡ�ļ�
 *
 * ����ֵ:
 *   void				- �޷���ֵ
 *
 * ˵��:
 *   �ú�����DIBͼ����ȡ��ˮӡ
 *
 ************************************************************************/
void CWaterMarking::Get(CFile &file)
{
	// Ƕ��ˮӡ�ĳ���
	int nFileLen = 54 + 3 * WIDTH * HEIGHT;
	
	CHECK_EDITABLE(m_pDIB->GetImage());

	BeginWaitCursor();
		
	LONG lWidth = m_pDIB->Width();
	LONG lHeight = m_pDIB->Height();
	LONG lLineBytes = m_pDIB->RowLen();
	BYTE* lpDIBBits = m_pDIB->FindAddress();

	// ���䲢��ʼ���ڴ�
	BYTE* lpBuf1;
	BYTE* lpBuf2;

	lpBuf1 = new BYTE [lLineBytes * lHeight];
	lpBuf2 = new BYTE [lLineBytes * lHeight];

	memset(lpBuf1, 0, lLineBytes * lHeight);
	memset(lpBuf2, 0, lLineBytes * lHeight);

	//ƴ���ļ���Ϣ�ֽ�ʱ����λ����
	int move2[13] = {6, 5, 2, 0, 7, 4, 2, 1, -2, 6, 4, 3, 0};

	//λͼ�ļ��ֽڵ���������
    BYTE mask2[13]={3, 1, 7, 3, 1, 7, 3, 1, 4, 3, 3, 1, 7};

	// Ϊ1ʱpointer1ָ���һ
	int add1[13]={0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1};

	// Ϊ1ʱpointer2ָ���һ
	int add2[13]={1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1};

	// ��ʱָ��
	int pointer1 = 0;
	int pointer2 = 0;
	int pointer3 = 0;
	int pointer4 = 0;

	// ��ȡˮӡ����
	while(true)
	{
		// ��DIB�ֽ�������ȡ��ˮӡ����
		if(move2[pointer3] > 0)
			lpBuf2[pointer1] |= (*(lpDIBBits + pointer2) & mask2[pointer3]) << move2[pointer3];
		else
			lpBuf2[pointer1] |= (*(lpDIBBits + pointer2) & mask2[pointer3]) >> (move2[pointer3] * (-1));
		
		if(add1[pointer3] == 1)
		{
			lpBuf1[pointer4] = lpBuf2[pointer1];
			pointer4++;

			// ˮӡ��ȡ��ϣ�����ѭ��
			if(pointer4 > nFileLen)
				break;

			//����ָ��
			pointer1++;
		}
		
		//����ָ��
		if(add2[pointer3] == 1)
			pointer2++;
		pointer3++;
		pointer3 %= 13;
	}
	file.Write(lpBuf1, nFileLen);

	//�ͷŻ���
	delete lpBuf1;
	delete lpBuf2;
	
	EndWaitCursor();
}

