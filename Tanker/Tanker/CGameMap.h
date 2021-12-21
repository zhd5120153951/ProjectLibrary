#pragma once
#include "CGameEntry.h"
#include "CWall.h"
#include "CBullet.h"
#include <vector>
class CGamePathFinder;
/*
	�����е�ͼ�е�һ������
	���ܿ�����ǽ����ͨ���� �м��ǿյؿ���ͨ��----���ϸ��--���������ͼ�໹����ϸ��Ϊ��ͼԪ����,
	�������Ը���ϸ�µ�������ͼ����
*/
class CGameMapElement :
	public CGameEntryMoveable
{
public:
	CGameMapElement();
	CGameMapElement(float x, float y, bool bLeft, bool bRight, bool bBottom, float wallLen = 10, float spaceLen = 80);
	virtual ~CGameMapElement();
	virtual void Draw(Graphics& gh)const;
	//ǽ�ķ���
	typedef enum { EWLeft = 0, EWTop, EWRight, EWBottom, EWSize }EWallIndex;
	//���������--��������������ȡ�����ؿ��ĳ��ǽ
	const CWall& operator[](CGameMapElement::EWallIndex eIndex)const;
	//���ÿյص���ɫ
	void SetSpaceColor(Color clr);
private:
	Color m_spaceColor{ 0xE6,0xE6,0xE6 };//�յص���ɫ(���ﻹ������RGB(12,12,12)���ֱ��)
	float m_wallWidth{ 10 };//ǽ�Ŀ��
	float m_spaceWidth = 80;//�յصĿ��
	CWall m_wallArray[4];//ÿ��ǽ����һ��ǽ��,������ǽ��������
};
//������ͼ��
class CGameMap
	:public CGameEntryMoveable
{
public:
	CGameMap();
	CGameMap(int x, int y, int width, int height);
	virtual ~CGameMap();
	//���Ƶ�ͼ
	virtual void Draw(Graphics& gh)const;
	//���ص�ͼ
	void LoadMap();
	//�ӵ�ײǽ����
	bool HitWallProcess(const CBullet& bullet)const;
	//�ж��ӵ��Ƿ�ײǽ--���ײ���ˣ�����-true�ͷ����,ײǽ��ʱ��
	bool GetHitWallInfo(const CBullet& bullet, float* pTime = nullptr, float* pDir = nullptr)const;
	// ̹���Ƿ�ײǽ
	bool IsHitWall(const CTankEntry& tanker, bool bForward) const;
	// ���һ�����Է�����Ʒ��λ�ã�λ���������
	bool FindRandomPosition(PointF& ptCenter) const;
	// �жϵ�ǰ�����ڵ�����, x : �����ڵڼ���,y : �����ڵڼ���
	bool FindObjPosition(const CGameEntryMoveable& obj, int& row, int& col) const;
	// ·�� ����
	// path ����·�� [��㣬�������� �յ�]
	void FindPath(int startX, int startY, int targetX, int targetY, CPathFinderBFS::CPath& path)const;
	// �����һ�ε�ǰ������
	bool FindNextDirection(float* dir, int startX, int startY, int targetX, int targetY) const;
	// ���ĳ��������ĵ�λ��
	PointF GetElementAreaCenterPos(int row, int col) const;
	// �ж����ڷ����ӵ��Ƿ���Դ����
	bool IsCounterTarget(const CGameEntryMoveable& bot, const CGameEntryMoveable& target, float* dir = nullptr) const;
private:
	//������е�ͼ�Ļ���Ԫ�صĶ�ά����--��ά����ķ�ʽ
	std::vector<std::vector<CGameMapElement>> m_mapArray;
	// ������ͼ�ķ�Χ(0,0)--(800,600)����
	Gdiplus::Rect m_mapRect{ 0, 0, 800, 600 };
	// ��ͼ��ǽ�Ŀ��
	int m_wallWidth{ 7 };
	// ��ͼ���м��λ�Ŀ��
	int m_spaceLen{ 42 };
	// ·������
	CGamePathFinder* m_finder{ nullptr };
};
