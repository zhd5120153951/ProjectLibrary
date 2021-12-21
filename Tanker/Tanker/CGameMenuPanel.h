/*!
 * \file: CGameMenuPanel.h
 * \author: Daito
 * \date: 2021/09/09 16:55
 * \version: 1.0.0
 * \CSDN: https://blog.csdn.net/zhd5120153951
 * \GitHub: https://github.com/zhd5120153951
 * \e-mail: 2462491568@qq.com
 * \description:������Ϸ��CGame������--�˵���--���Ĺ����Ƕ�������Ϣ��Ӧ,����ͼָ��,��źͲ˵�������
 */

#pragma once
#include "CGameEntry.h"
#include <tuple>
#include <vector>
using namespace std;
class CGame;
class CGameMenuPanel :CGameEntryMoveable
{	
public:
	CGameMenuPanel();
	~CGameMenuPanel();
	void SetParent(CGame *ptr_G);//���һ����Ϸ���ָ�����ı���
	virtual void Draw(Graphics &gh)const;//�����Լ�
	void OnMouseMove(UINT nFlags, CPoint point);//��������ƶ��¼�����
	void OnLButtonUp(UINT nFlags, CPoint point);//�����������ɿ��¼�����

	CGame *m_PtrParent{ NULL };//����һ��ָ����Ϸ���ָ��--������ı���
	RectF m_Rect{ 0,0,800,600 };//��ʼ��߷�Χ
	Image *m_ImgBackGround{ NULL };//����ͼָ��
	struct 
	{
		vector<tuple<Image *, RectF>>vImgInfoPtr;//C++��Ҳ��Ԫ������,�ǲ���tuple���ֵ�--һ��key:value(python����Ԫ��)
		int index;//��ʾ�˵��ĵڼ���,ÿ����vector������Ϣ,���һ��ͳһΪͼƬ��Ϣ,�ڶ���Ϊ����--��Ϊ��ͼƬ������
	}m_MenuList[2];
};

