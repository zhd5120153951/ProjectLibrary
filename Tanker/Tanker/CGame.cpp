#include "pch.h"
#include "CGame.h"

CGame::CGame()
{

}

CGame::~CGame()
{

}

void CGame::SetHandle(HWND hWnd)
{
	m_hWnd = hWnd;//�ѵ�ǰ���ڸ����ڱ���
}
//������Ϸ
bool CGame::EnterFrame(DWORD dwTime)
{
	//��ʼ���ƶ���
	GameRunDraw();//�����ʵ�ֹ����ɴ˺������
	GameRunDraw();//���û�ͼ����
	return false;
}

void CGame::OnMouseMove(UINT nFlags, CPoint point)
{
	m_Menu.OnMouseMove(nFlags, point);//�˵�ҳ��--ѡ����Ϸ����--�ƶ�ѡ��
}

void CGame::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_Menu.OnLButtonUp(nFlags, point);//�˵�ҳ��--ѡ����Ϸ����--̧��ȷ��
}
//��Ϸ����ͼ����
void CGame::GameRunDraw()
{
	HDC hdc = ::GetDC(m_hWnd);//�õ���ǰ���ڵľ��
	CRect rc;
	GetClientRect(m_hWnd, &rc);//�õ���ǰ���ڵĴ�С

	CDC *dc = CClientDC::FromHandle(hdc);

	CDC m_dcMemory;          //˫������,��ͼ��
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(dc, rc.Width(), rc.Height());
	m_dcMemory.CreateCompatibleDC(dc);
	CBitmap *pOldBitmap = m_dcMemory.SelectObject(&bmp);

	Graphics gh(m_dcMemory.GetSafeHdc());//����һ��ͼ�ζ���gh
	gh.Clear(Color::White);//�������
	gh.ResetClip();

	//�����ڴ���
	{
		m_Menu.Draw(gh);//����ʼ�˵�
		m_Background.Draw(gh);//������
	}
	//���Ƶ��ڴ���
	::BitBlt(hdc, 0, 0, rc.Width(), rc.Height(), m_dcMemory.GetSafeHdc(), 0, 0, SRCCOPY);
	dc->DeleteDC();//�ͷ�
	return;
}

//����FPS
void CGame::DrawFps(Graphics &gh)
{
	static int fps = 0;//��̬����,ÿ�ε����������ʱ�������ϴε�ֵ
	m_Fps++;    //֡���Լ�,��¼�Ѿ����˶���֡
	static DWORD dwLast = GetTickCount();//��¼�ϴ������֡��ʱ��,--��Ϊ�˼����´ε�ʱ���,�������´�֡��
	if (GetTickCount()-dwLast>=1000)//֡��=֡��/ʱ��(s)--��������Ҫ����1000ms
	{
		fps = m_Fps;
		m_Fps = 0;//ÿ���ڵ�֡��������Ҫ��0
		dwLast = GetTickCount();//��¼�±��������ʱ��
	}
	//���fps
	{
		CString s;
		s.Format(_T("FPS:%d"), fps);//��fps��ʽ�����ַ���s
		//SolidBrush brush(Color(0x00, 0x00, 0xFF));//����һ����ɫ��ˢ
		SolidBrush brush(Color(rand() % 256, rand() % 256, rand() % 256));//����һ�������ɫ��ˢ
		Gdiplus::Font font(_T("����"), 10.5);//�������������
		CRect rc;
		GetClientRect(m_hWnd, &rc);//��ȡ������ڵĴ�С,������λ���ֵ����λ��
		PointF origin(static_cast<float>(rc.right - 50), static_cast<float>(rc.top + 2));//�����Ͻǻ�֡��
		//ǰ�涼��ΪͼƬ��λ����׼��,�����ǿ�ʼ��ͼ��
		gh.DrawString(s.GetString(), -1, &font, origin, &brush);
	}
}
