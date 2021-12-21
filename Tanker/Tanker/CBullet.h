#pragma once
#include "CGameEntry.h"
class CTankEntry;
class CBullet :
	public CGameEntryMoveable
{
public:
	CBullet();
	CBullet(float x, float y, float speed, float dirtion);
	CBullet(const CBullet& other);//�������캯��һ������������һ���
	virtual ~CBullet();

	CBullet& operator=(const CBullet& other);//���������
	//�����Լ��Ķ�̬����
	virtual void Draw(Graphics& gh)const;//const��ʾ��������в����޸ĳ�Ա����
	//�ӵ��ƶ�
	void Move();
	//�ӵ��Ƿ�ʱ--��ʱ������,��������˶�
	bool IsTimeout()const;
	//�����ӵ�λ��
	void SetBulletPos(PointF pfCenter);//��λ���������������
	//�����ӵ���ӵ����--˭������ӵ�?
	void SetBulletOwner(CTankEntry* pOwner);//�����������ʱ--��������˼��--���ǲ�ͬ�����֮���Ƕ����Ϲ�ϵ
	//�����ӵ�����ʱ��--������ʱ��
	void SetKeepTime(unsigned long keepTime);//�������һ�������ӵ���������
	//�����ӵ���ʼʱ��--���ʱ��Ҫһֱ��Ч
	void SetBulletFirstTime(unsigned long firstTime);
	//�����ӵ�ֱ��
	void SetBulletDiameter(float diameter);
	//��ȡ�ӵ�ֱ��--������ò��ܸĶ�
	float GetBulletDiameter()const;
	//�����ӵ�Ϊ�״η���
	void SetFirstLanch();
	//�ж�ÿ���ӵ��Ƿ��״η���?--��Ϊ�ӵ��ᷴ�����кܶ�
	bool IsFirstLanch()const;
private:
	int  m_distance{ 0 };//�ӵ�ÿ��λ�ƾ���
	float m_diameter = 10;//�ӵ�ֱ��
	unsigned long m_keepTime = { 10 * 1000 };//����д��10000����Ϊ�ں�������ʱ,����������ʱ�䵥λ�Ǻ���ms
	unsigned long m_lanchTime{ 0 };//��¼�ӵ�����ʱ��
	CTankEntry* m_pOwner{ nullptr };//�ӵ�����˭
};

