// ************************************************************************
//  �ļ�����DlgIntensity.cpp
//
//  ͼ��Ҷȷֲ�ֱ��ͼ��ʾ����ࣺ
//
// ************************************************************************

#include "stdafx.h"
#include "DIP_System.h"
#include "DlgIntensity.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgIntensity dialog

CDlgIntensity::CDlgIntensity(CWnd* pParent /*=NULL*/) : CDialog(CDlgIntensity::IDD, pParent)
{
}

void CDlgIntensity::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDlgIntensity, CDialog)
	ON_WM_PAINT()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgIntensity message handlers

void CDlgIntensity::OnPaint() 
{
	// �ַ���
	CString str;
	
	LONG i;
	
	// ������
	float fMaxIntensity = 0;
	
	// �豸������
	CPaintDC dc(this);
	
	// ��ȡ��������ľ�̬��
	CWnd* pWnd = GetDlgItem(IDC_COORD);
	
	// ָ��
	CDC* pDC = pWnd->GetDC();
	pWnd->Invalidate();
	pWnd->UpdateWindow();
	
	pDC->Rectangle(0,0,330,300);
	
	// �������ʶ���
	CPen* pPenRed = new CPen;
	
	// ��ɫ����
	pPenRed->CreatePen(PS_SOLID,1,RGB(255,0,0));
	
	// �������ʶ���
	CPen* pPenBlue = new CPen;
	
	// ��ɫ����
	pPenBlue->CreatePen(PS_SOLID,1,RGB(0,0, 255));
	
	// ѡ�е�ǰ��ɫ���ʣ���������ǰ�Ļ���
	CGdiObject* pOldPen = pDC->SelectObject(pPenRed);
	
	// ����������
	pDC->MoveTo(10,10);
	
	// ��ֱ��
	pDC->LineTo(10,280);
	
	// ˮƽ��
	pDC->LineTo(320,280);
	
	// дX��̶�ֵ
	str.Format(L"0");
	pDC->TextOut(10, 283, str);
	str.Format(L"50");
	pDC->TextOut(60, 283, str);
	str.Format(L"100");
	pDC->TextOut(110, 283, str);
	str.Format(L"150");
	pDC->TextOut(160, 283, str);
	str.Format(L"200");
	pDC->TextOut(210, 283, str);
	str.Format(L"255");
	pDC->TextOut(265, 283, str);
	
	// ����X��̶�
	for (i = 0; i < 256; i += 5)
	{
		if ((i & 1) == 0)
		{
			// 10�ı���
			pDC->MoveTo(i + 10, 280);
			pDC->LineTo(i + 10, 284);
		}
		else
		{
			// 10�ı���
			pDC->MoveTo(i + 10, 280);
			pDC->LineTo(i + 10, 282);
		}
	}
	
	// ����X���ͷ
	pDC->MoveTo(315,275);
	pDC->LineTo(320,280);
	pDC->LineTo(315,285);
	
	// ����X���ͷ
	pDC->MoveTo(10,10);
	pDC->LineTo(5,15);
	pDC->MoveTo(10,10);
	pDC->LineTo(15,15);
	
	// ����������ֵ
	for (i = 0; i < 256; i ++)
	{
		// �ж��Ƿ���ڵ�ǰ���ֵ
		if (m_fIntensity[i] > fMaxIntensity)
		{
			// �������ֵ
			fMaxIntensity = m_fIntensity[i];
		}
	}
	
	// ���������ֵ
	pDC->MoveTo(10, 25);
	pDC->LineTo(14, 25);
	str.Format(L"%f", fMaxIntensity);
	pDC->TextOut(11, 26, str);
	
	// ���ĳ���ɫ����
	pDC->SelectObject(pPenBlue);	
	
	// �ж��Ƿ��м���
	if (fMaxIntensity > 0)
	{
		// ����ֱ��ͼ
		for (i = 0; i < 256; i ++)
		{
			pDC->MoveTo(i + 10, 280);
			pDC->LineTo(i + 10, 281 - (int) (m_fIntensity[i] * 256 / fMaxIntensity));
		}
	}
	
	// �ָ���ǰ�Ļ���
	pDC->SelectObject(pOldPen);	
	
	// ɾ���µĻ���
	delete pPenRed;
	delete pPenBlue;
}


