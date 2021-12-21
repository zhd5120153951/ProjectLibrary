#include "pch.h"
#include "CGameMenuPanel.h"
#include "CGame.h"

//�˵����캯��
CGameMenuPanel::CGameMenuPanel()
{
	//����Ĭ�ϱ���ͼ
	m_ImgBackGround = Image::FromFile(_T("menu_background.png"));//��ʱ��ͼ��cpp��ͬ��Ŀ¼��,����������Ҫ��·���������
	//���ز˵���һ:�˻���սͼ
	m_MenuList[0].index = 0;//index��ʾ��һ��
	for(auto i=0;i<2;i++)
	{
		TCHAR path[MAX_PATH];//MAX_PATHĬ�Ͽ��е����·������
		_stprintf_s(path, _T("menu_1player_%d.png"), i);//�ѵ�i��ͼ��ʽ���ļ���,�����path������
		auto imgPtr = Image::FromFile(path);//����֮ǰ��ͼƬ,��ŵ�ָ����
		RectF rc(0, 300, static_cast<float>(imgPtr->GetWidth()), static_cast<float>(imgPtr->GetHeight()));//�õ�ÿ��ͼ�Ŀ��ֵ
		m_MenuList[0].vImgInfoPtr.push_back(make_tuple(imgPtr, rc));//�Ѹղż��ص�ͼƬ��ϳ�һ��Ԫ��(C++��Ҳ�Ǽ�ֵ��,python�в���),
		                                                            //��ѹ�뵽�˵�������
	 }
	//���ز˵���һ:˫�˶�ս
	m_MenuList[1].index = 0;//ͬ��
	for (int i = 0; i < 2; i++)
	{
		TCHAR path[MAX_PATH];
		_stprintf_s(path, _T("menu_2player_%d.png"), i);
		auto imgPtr = Image::FromFile(path);
		RectF rc(400, 300, static_cast<float>(imgPtr->GetWidth()), static_cast<float>(imgPtr->GetHeight()));
		m_MenuList[1].vImgInfoPtr.push_back(make_tuple(imgPtr, rc));
	}
}
//�˵�����������
CGameMenuPanel::~CGameMenuPanel()
{

}
//���ò˵����ı�--�Ѹ����ָ�������ı���
void CGameMenuPanel::SetParent(CGame *ptr_G)
{
	m_PtrParent = ptr_G;
}
//���Ʊ����Լ�
void CGameMenuPanel::Draw(Graphics &gh) const
{
	gh.DrawImage(m_ImgBackGround, m_Rect);//��������ͼ(��ʼ��ΪNULL��0,0,800,600)
	//�����Ӳ˵�
	for(auto menuList:m_MenuList)//����Ҳ������:��������
	{
		auto img = get<0>(menuList.vImgInfoPtr[menuList.index]);//��ò˵����ͼƬ
		auto rect = get<1>(menuList.vImgInfoPtr[menuList.index]);//��ȡ�˵���Ĵ�С
		gh.DrawImage(img, rect);
	 }
}
//��������ƶ��¼�
void CGameMenuPanel::OnMouseMove(UINT nFlags, CPoint point)
{
	PointF pt(static_cast<float>(point.x), static_cast<float>(point.y));
	//�����Ӳ˵�
	for (auto &menuList:m_MenuList)
	{
		auto img = get<0>(menuList.vImgInfoPtr[menuList.index]);//��ȡ�˵����ͼƬ
		auto rect = get<1>(menuList.vImgInfoPtr[menuList.index]);//��ȡ�˵���ͼƬ�Ĵ�С
		if (rect.Contains(pt))//�ж��Ƿ������ǰ���λ��
			menuList.index = 1;//Contains()Ϊ��--����--��ô��ʾ��һ��ͼƬ
		else menuList.index = 0;//������ʾ�ڶ���ͼƬ
	}
}
//����������̧���¼�
void CGameMenuPanel::OnLButtonUp(UINT nFlags, CPoint point)
{
	PointF pt(static_cast<float>(point.x), static_cast<float>(point.y));
	{
		auto &menuList = m_MenuList[0];//��ȡ��һ��ͼƬ(�±�Ϊ0)
		auto img = get<0>(menuList.vImgInfoPtr[menuList.index]);//��ȡͼƬָ��
		auto rect = get<1>(menuList.vImgInfoPtr[menuList.index]);//��ȡͼƬָ��
		if (rect.Contains(pt))//�ж��Ƿ�����õ�
		{
			//��ʱ�������˼Ҷ�ս����
			return;
		}
	}
	{
		auto &menuList = m_MenuList[1];//��ȡ�ڶ���ͼƬ(�±�Ϊ1)
		auto img = get<0>(menuList.vImgInfoPtr[menuList.index]);//��ȡͼƬָ��
		auto rect = get<1>(menuList.vImgInfoPtr[menuList.index]);//��ȡͼƬָ��
		if (rect.Contains(pt))//�ж��Ƿ�����õ�
		{
			//��ʱ�������˼Ҷ�ս����
			return;
		}
	}
}
