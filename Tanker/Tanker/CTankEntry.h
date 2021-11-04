#pragma once
#include "CGameEntry.h"
#include <list>
class CTankEntry :public CGameEntryMoveable
{
public:
	CTankEntry();
	CTankEntry(float x, float y, PCTSTR szImgName);
	virtual ~CTankEntry();
	virtual void Draw(Graphics& gh)const;//CGameMoveable�еĴ��麯��

	//���ÿ���ʱ��
	void SetFireTimer(unsigned long timeVal);
	//������--������һ���ӵ�����������
	bool GoFire(CBullet& blt);
	//�����ӵ�
	void AddBullet(CBullet& bit);
	//���̹��ǹλ��
	virtual PointF GetGunPos()const;
	//����ӵ���ǹ�ڵĳ�ʼλ��
	virtual PointF GetBulletStartPos(float bltRadius)const;
	//̹�˱�ը����
	void TankBomb();
	//�ӵ�����̹���Ƿ���Ч
	bool IsActive()const;
	//̹�˱�ը�Ƿ����--��Щ�漰��ͼƬ�Ļ���
	virtual bool IsBombEnd()const;
	//�ӵ��Ƿ����̹��
};

