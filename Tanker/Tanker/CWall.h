#pragma once
#include "CGameEntry.h"
#include "CBullet.h"
class CWall :
	public CGameEntryMoveable
{
public:
	enum  EWallDirection { EDirectionX, EDirectionY };//X-����,Y-����
	
	CWall();
	CWall(float x, float y, float width, float length, CWall::EWallDirection dir);
	virtual ~CWall();
	//����ǽ���Լ�
	virtual void Draw(Graphics& gh)const;
	//���÷���
	void SetWallDir(CWall::EWallDirection dir);
	//��ȡ����
	CWall::EWallDirection GetWallDir()const;

	// true-�ӵ�ײ��ǽ,��ı䷽���λ��(����Ϊ�պò�ײǽ��λ��),����û��ײǽ
	bool HitAction(CBullet& bullet) const;
	// �ж��Ƿ���ײ�����������ײ��������ײ�㣬�������
	bool CheckHit(const CBullet& bullet, PointF* ptHit = nullptr, float* dir = nullptr, float* pDelay = nullptr) const;
	// ��һ���Ƿ��ײ��ǽ
	bool IsWillHit(const CTankEntry& tanker, bool bForward) const;
private:
	// �ж�С��ײ��ǽ����һ��,�����ײ������������ײ����һ�ߵ���ײ��Ϣ
	bool CheckHitEx(const CBullet& bullet, PointF* ptHit = nullptr, float* dir = nullptr, float* pDelay = nullptr) const;

	EWallDirection m_direction;//ǽ�ķ���
	mutable Color m_color{ 0x4D,0x4D,0x4D };
};

