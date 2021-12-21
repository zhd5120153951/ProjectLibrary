#pragma once
#include "CTankEntry.h"
#include "CGameTimer.h"
class CPlayer :
	public CTankEntry
{
public:
	CPlayer();
	CPlayer(float x, float y, PCTSTR szImgName);
	virtual ~CPlayer();
	//������ת:����ת���������̫��
	virtual void RotateLeft();
	virtual void RotateRight();//�̳���CTankEntry��,��CTankEntry�ּ̳���CGameEntry��
private:
	CGameTimeval m_timeVal{ 100 };//У��������ת��ʱ���(ʱ��������)
};

