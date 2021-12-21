#pragma once
#include "CGameEntry.h"
#include "CBullet.h"
class CWall :
	public CGameEntryMoveable
{
public:
	enum  EWallDirection { EDirectionX, EDirectionY };//X-横向,Y-纵向
	
	CWall();
	CWall(float x, float y, float width, float length, CWall::EWallDirection dir);
	virtual ~CWall();
	//绘制墙壁自己
	virtual void Draw(Graphics& gh)const;
	//设置方向
	void SetWallDir(CWall::EWallDirection dir);
	//获取方向
	CWall::EWallDirection GetWallDir()const;

	// true-子弹撞上墙,则改变方向和位置(调整为刚好不撞墙的位置),否则没有撞墙
	bool HitAction(CBullet& bullet) const;
	// 判断是否碰撞，如果发生碰撞，返回碰撞点，及反射角
	bool CheckHit(const CBullet& bullet, PointF* ptHit = nullptr, float* dir = nullptr, float* pDelay = nullptr) const;
	// 下一步是否会撞上墙
	bool IsWillHit(const CTankEntry& tanker, bool bForward) const;
private:
	// 判断小球撞上墙的哪一边,如果相撞，返回最先碰撞的那一边的碰撞信息
	bool CheckHitEx(const CBullet& bullet, PointF* ptHit = nullptr, float* dir = nullptr, float* pDelay = nullptr) const;

	EWallDirection m_direction;//墙的方向
	mutable Color m_color{ 0x4D,0x4D,0x4D };
};

