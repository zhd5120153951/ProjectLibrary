#pragma once
#include "CGameEntry.h"
#include <list>
class CTankEntry :public CGameEntryMoveable
{
public:
	CTankEntry();
	CTankEntry(float x, float y, PCTSTR szImgName);
	virtual ~CTankEntry();
	virtual void Draw(Graphics& gh)const;//CGameMoveable中的纯虚函数

	//设置开火时间
	void SetFireTimer(unsigned long timeVal);
	//开火函数--参数是一个子弹类对象的引用
	bool GoFire(CBullet& blt);
	//增加子弹
	void AddBullet(CBullet& bit);
	//获得坦克枪位置
	virtual PointF GetGunPos()const;
	//获得子弹出枪口的初始位置
	virtual PointF GetBulletStartPos(float bltRadius)const;
	//坦克爆炸毁灭
	void TankBomb();
	//子弹击中坦克是否有效
	bool IsActive()const;
	//坦克爆炸是否结束--这些涉及到图片的绘制
	virtual bool IsBombEnd()const;
	//子弹是否击中坦克
};

