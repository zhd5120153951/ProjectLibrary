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
	//左右旋转:两次转动间隔不能太快
	virtual void RotateLeft();
	virtual void RotateRight();//继承于CTankEntry类,而CTankEntry又继承于CGameEntry类
private:
	CGameTimeval m_timeVal{ 100 };//校对两次旋转的时间差(时间间隔对象)
};

