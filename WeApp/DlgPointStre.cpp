// ************************************************************************
//  �ļ�����DlgPointStre.cpp
//
//  ͼ��Ҷ����߱任��������ࣺ
//
// ************************************************************************

#include "stdafx.h"
#include "DIP_System.h"
#include "DlgPointStre.h"
#include "GrayProc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgPointStre dialog

CDlgPointStre::CDlgPointStre(CWnd* pParent /*=NULL*/) : CDialog(CDlgPointStre::IDD, pParent)
{
	m_bX1 = 50;
	m_bY1 = 50;
	m_bX2 = 200;
	m_bY2 = 200;
	m_pDoc = NULL;
}

void CDlgPointStre::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDlgPointStre, CDialog)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPointStre message handlers

BOOL CDlgPointStre::OnInitDialog() 
{
	// ����Ĭ��OnInitDialog����
	CDialog::OnInitDialog();
	
	// ��ȡ����ֱ��ͼ�ı�ǩ
	CWnd* pWnd = GetDlgItem(IDC_COORD);
	
	// �����������¼�����Ч����
	pWnd->GetClientRect(m_MouseRect);
	pWnd->ClientToScreen(&m_MouseRect);
	
	CRect rect;
	GetClientRect(rect);
	ClientToScreen(&rect);
	
	m_MouseRect.top -= rect.top;
	m_MouseRect.left -= rect.left;
	
	// ���ý�������¼�����Ч����
	m_MouseRect.top += 25;
	m_MouseRect.left += 10;
	m_MouseRect.bottom = m_MouseRect.top + 255;
	m_MouseRect.right = m_MouseRect.left + 256;
	
	// ��ʼ���϶�״̬
	m_iIsDraging = 0;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgPointStre::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// ���û�������������ʼ�϶�
	if(m_MouseRect.PtInRect(point))
	{
		CRect	rectTemp;
		
		// �����1�ٽ�����
		rectTemp.left = m_MouseRect.left + m_bX1 - 2;
		rectTemp.right = m_MouseRect.left + m_bX1 + 2;
		rectTemp.top = 255 + m_MouseRect.top - m_bY1 - 2;
		rectTemp.bottom = 255 + m_MouseRect.top - m_bY1 + 2;
		
		// �ж��û��ǲ������϶���1
		if (rectTemp.PtInRect(point))
		{
			// �����϶�״̬1���϶���1
			m_iIsDraging = 1;
			
			// ���Ĺ��
			::SetCursor(::LoadCursor(NULL, IDC_SIZEALL));
		}
		else
		{
			// �����2�ٽ�����
			rectTemp.left = m_MouseRect.left + m_bX2 - 2;
			rectTemp.right = m_MouseRect.left + m_bX2 + 2;
			rectTemp.top = 255 + m_MouseRect.top - m_bY2 - 2;
			rectTemp.bottom = 255 + m_MouseRect.top - m_bY2 + 2;
			
			// �ж��û��ǲ������϶���2
			if (rectTemp.PtInRect(point))
			{
				// �����϶�״̬Ϊ2���϶���2
				m_iIsDraging = 2;
				
				// ���Ĺ��
				::SetCursor(::LoadCursor(NULL, IDC_SIZEALL));
			}
		}
	}
	
	// Ĭ�ϵ��������������¼�
	CDialog::OnLButtonDown(nFlags, point);
}

void CDlgPointStre::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// ���û��ͷ�������ֹͣ�϶�
	if (m_iIsDraging != 0)
	{
		// �����϶�״̬
		m_iIsDraging = 0;

		// ��ʼ������ΪFALSE
 		m_pDoc->SetModifiedFlag(FALSE);
 
		// ����װ������
		m_pDoc->m_pDIB->ReLoad();

		// ����GrayStretch()�������лҶ�����
		CGrayProc clsGrayProc(m_pDoc->m_pDIB);
		clsGrayProc.GrayStretch(m_bX1, m_bY1, m_bX2, m_bY2);
	
		// ��������
		m_pDoc->SetModifiedFlag(TRUE);
	
		// ������ͼ
		m_pDoc->UpdateAllViews(NULL);
	}
	
	// Ĭ���ͷ������������¼�
	CDialog::OnLButtonUp(nFlags, point);
}

void CDlgPointStre::OnMouseMove(UINT nFlags, CPoint point) 
{
	// �жϵ�ǰ����Ƿ��ڻ�������
	if(m_MouseRect.PtInRect(point))
	{
		// �ж��Ƿ������϶�
		if (m_iIsDraging != 0)
		{
			// �ж������϶���1���ǵ�2
			if (m_iIsDraging == 1)
			{
				// �ж��Ƿ�����<����
				if (point.x - m_MouseRect.left < m_bX2)
				{
					// ��������
					m_bX1 = (BYTE) (point.x - m_MouseRect.left);
				}
				else
				{
					// �����Ϲ����ޣ�����Ϊ����-1
					m_bX1 = m_bX2 - 1;
					
					// �������λ��
					point.x = m_MouseRect.left + m_bX2 - 1;
				}
				
				// ����Y����
				m_bY1 = (BYTE) (255 + m_MouseRect.top - point.y);
			}
			else
			{
				// �����϶���2
				
				// �ж��Ƿ�����>����
				if (point.x - m_MouseRect.left > m_bX1)
				{
					// ��������
					m_bX2 = (BYTE) (point.x - m_MouseRect.left);
				}
				else
				{
					// �����Ϲ����ޣ�����Ϊ���ޣ�1
					m_bX2 = m_bX1 + 1;
					
					// �������λ��
					point.x = m_MouseRect.left + m_bX1 + 1;
				}
				
				// ����Y����
				m_bY2 = (BYTE) (255 + m_MouseRect.top - point.y);
			}
			
			// ���Ĺ��
			::SetCursor(::LoadCursor(NULL, IDC_SIZEALL));
			
			// ����
			UpdateData(FALSE);
			
			// �ػ�
			InvalidateRect(m_MouseRect, TRUE);
		}
		else
		{
			CRect	rectTemp1;
			CRect	rectTemp2;
			
			// �����1�ٽ�����
			rectTemp1.left = m_MouseRect.left + m_bX1 - 2;
			rectTemp1.right = m_MouseRect.left + m_bX1 + 2;
			rectTemp1.top = 255 + m_MouseRect.top - m_bY1 - 2;
			rectTemp1.bottom = 255 + m_MouseRect.top - m_bY1 + 2;
			
			// �����2�ٽ�����
			rectTemp2.left = m_MouseRect.left + m_bX2 - 2;
			rectTemp2.right = m_MouseRect.left + m_bX2 + 2;
			rectTemp2.top = 255 + m_MouseRect.top - m_bY2 - 2;
			rectTemp2.bottom = 255 + m_MouseRect.top - m_bY2 + 2;
			
			// �ж��û��ڵ�1���2�Ա�
			if ((rectTemp1.PtInRect(point)) || (rectTemp2.PtInRect(point)))
			{
				// ���Ĺ��
				::SetCursor(::LoadCursor(NULL, IDC_SIZEALL));
			}
		}
	}
	
	// Ĭ������ƶ������¼�
	CDialog::OnMouseMove(nFlags, point);
}

void CDlgPointStre::OnPaint() 
{
	// �ַ���
	CString str;
	
	LONG i;

	// ������
	float fMaxIntensity = 0;

	// �豸������
	CPaintDC dc(this);
	
	// ��ȡ����������ı���
	CWnd* pWnd = GetDlgItem(IDC_COORD);
	
	// ָ��
	CDC* pDC = pWnd->GetDC();
	pWnd->Invalidate();
	pWnd->UpdateWindow();

	pDC->Rectangle(0,0,330,300);
	
	// �������ʶ���
	CPen* pPenRed = new CPen;
	
	// ��ɫ����
	pPenRed->CreatePen(PS_SOLID, 2, RGB(255,0,0));
	
	// �������ʶ���
	CPen* pPenBlue = new CPen;
	
	// ��ɫ����
	pPenBlue->CreatePen(PS_SOLID, 1, RGB(0,0, 255));
	
	// ѡ�е�ǰ��ɫ���ʣ���������ǰ�Ļ���
	CGdiObject* pOldPen = pDC->SelectObject(pPenRed);
	
	// ����������
	pDC->MoveTo(10,10);
	
	// ��ֱ��
	pDC->LineTo(10,280);
	
	// ˮƽ��
	pDC->LineTo(320,280);
	
	// д����
	str.Format(L"0");
	pDC->TextOut(10, 281, str);
	
	str.Format(L"255");
	pDC->TextOut(265, 281, str);
	pDC->TextOut(11, 25, str);
	
	// ����X���ͷ
	pDC->LineTo(315,275);
	pDC->MoveTo(320,280);
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
	
	// ��������ֵ
	str.Format(L"(%d, %d)", m_bX1, m_bY1);
	pDC->TextOut(m_bX1 + 10, 281 - m_bY1, str);
	str.Format(L"(%d, %d)", m_bX2, m_bY2);
	pDC->TextOut(m_bX2 + 10, 281 - m_bY2, str);

	// ���ĳɺ�ɫ����
	pDC->SelectObject(pPenRed);	

	// �����û�ָ���ı任ֱ��
	pDC->MoveTo(10, 280);
	pDC->LineTo(m_bX1 + 10, 280 - m_bY1);
	pDC->LineTo(m_bX2 + 10, 280 - m_bY2);
	pDC->LineTo(265, 25);

	// ���Ƶ��Ե��С����
	CBrush  brush;
	brush.CreateSolidBrush(RGB(0,255,0));

	// ѡ��ˢ��
	CGdiObject* pOldBrush = pDC->SelectObject(&brush);

	// ����С����
	pDC->Rectangle(m_bX1 + 10 - 2, 280 - m_bY1 - 2, m_bX1 + 12, 280 - m_bY1 + 2);
	pDC->Rectangle(m_bX2 + 10 - 2, 280 - m_bY2 - 2, m_bX2 + 12, 280 - m_bY2 + 2);

	// �ָ���ǰ�Ļ���
	pDC->SelectObject(pOldPen);	

	// ���Ʊ�Ե
	pDC->MoveTo(10,25);
	pDC->LineTo(265,25);
	pDC->LineTo(265,280);
	
	// ɾ���µĻ���
	delete pPenRed;
	delete pPenBlue;
}


