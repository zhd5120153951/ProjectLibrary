#pragma once
#include "CGameEntry.h"
class CGame;
class CGameMenuPanel2 :
	public CGameEntryMoveable
{
public:
	CGameMenuPanel2();
	virtual ~CGameMenuPanel2();
	
	//�����Լ����麯��
	virtual void Draw(Gdiplus::Graphics& gh)const;
	//��������ƶ��¼�
	void OnMouseMove(UINT nFlags, CPoint point);
	//����������̧���¼�
	void OnLButtonUp(UINT nFlags, CPoint point);
public:
	CGame* m_pParent{ nullptr };//��Ϸ��ָ��
	//�˵�ͼƬ
	Image* m_Img{ nullptr };
	//��ͼ״̬
	bool m_bNormal{ true };//Ĭ������
};

