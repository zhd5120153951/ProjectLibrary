#pragma once
#include "CGameEntry.h"
class CTankEntry;
class CBullet :
	public CGameEntryMoveable
{
public:
	CBullet();
	CBullet(float x, float y, float speed, float dirtion);
	CBullet(const CBullet& other);//拷贝构造函数一般和运算符重载一起的
	virtual ~CBullet();

	CBullet& operator=(const CBullet& other);//运算符重载
	//绘制自己的多态函数
	virtual void Draw(Graphics& gh)const;//const表示这个函数中不可修改成员变量
	//子弹移动
	void Move();
	//子弹是否超时--超时就销毁,否则继续运动
	bool IsTimeout()const;
	//设置子弹位置
	void SetBulletPos(PointF pfCenter);//其位置由中心坐标代表
	//设置子弹的拥有者--谁发射的子弹?
	void SetBulletOwner(CTankEntry* pOwner);//大型软件开发时--面向对象的思想--就是不同类对象之间的嵌套耦合关系
	//设置子弹存在时间--即飞行时间
	void SetKeepTime(unsigned long keepTime);//这个函数一般是在子弹发射后调用
	//设置子弹起始时间--这个时间要一直有效
	void SetBulletFirstTime(unsigned long firstTime);
	//设置子弹直径
	void SetBulletDiameter(float diameter);
	//获取子弹直径--仅仅获得不能改动
	float GetBulletDiameter()const;
	//设置子弹为首次发射
	void SetFirstLanch();
	//判断每个子弹是否首次发射?--因为子弹会反弹且有很多
	bool IsFirstLanch()const;
private:
	int  m_distance{ 0 };//子弹每次位移距离
	float m_diameter = 10;//子弹直径
	unsigned long m_keepTime = { 10 * 1000 };//这里写成10000是因为在函数调用时,函数参数的时间单位是毫秒ms
	unsigned long m_lanchTime{ 0 };//记录子弹发射时间
	CTankEntry* m_pOwner{ nullptr };//子弹属于谁
};

