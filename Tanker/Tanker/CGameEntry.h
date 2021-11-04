#pragma once
#include "pch.h"
#include <cmath>
#include <gdiplus.h>
#include <d2d1_1helper.h>
using namespace std;
using namespace Gdiplus;
/*!
 * \class: CGameEntry
 * \brief: 代表游戏中的所有对象,包括可移动的对象和不可以移动对象的抽象--抽象类
 * \author: Daito
 * \date: 2021/06/24 16:30
 * \other:
 */
class CGameEntry
{
public:
	CGameEntry(){}
	virtual ~CGameEntry(){}
public:
	//这里反复用::域运算符,如果不想用可以直接使用using namespace Gdiplus(头文件已经加了,所以这里用不用都一样)
	virtual void Draw(Graphics& gh) const = 0;//绘制自己的纯虚函数--多态
};
//移动类
class CGameEntryMoveable:public CGameEntry
{
public:
	CGameEntryMoveable();//默认构造
	virtual ~CGameEntryMoveable();//析构
	CGameEntryMoveable(const CGameEntryMoveable& rhs);//拷贝构造
	CGameEntryMoveable& operator=(const CGameEntryMoveable& rhs);//=运算符重载
public:
	virtual void Draw(Graphics& gh)const = 0;//子类继承父类,按理说应该重写(就是去实现这个函数)这个纯虚函数的,但是子类又要成为抽象类则可以不写
	/*可移动对象的共同属性--成员变量及成员函数*/
	virtual void RotateRight();//右转
	virtual void RotateLeft(); //左转
	virtual void Forward();    //前进
	virtual void Backward();  //后退
	virtual float GetSpeedX()const;//获得水平方向的速度
	virtual float GetSpeedY()const;//获得垂直方向的速度
	virtual RectF ForwardNextRect()const;//下一步前进的位置(由于移动单位是以一个RECT矩形结构为单元,所以返回值为Rect类型)
	virtual RectF BackwardNextRect()const;//下一步后退的位置
	virtual PointF GetHeadPos()const;//获取头部位置,绘制的图为矩形,头部位置即角度为0
	                                 //具体算法:距离中心点为半径(对角线/2),重置于自己的X方向的位置
	virtual PointF GetCenterPoint()const;//获取中心点
	virtual void SetCenterPoint(const PointF& CenterPoint);//设置中心点

	//属性存取器
#ifndef PI
#define PI 3.1415926f
#endif // 
	//设置角度单位->弧度制:a*PI
	virtual void SetDirectionArc(float dirArc);
	//设置角度单位->角度制:度
	virtual void SetDirection(float dir);
	//获得当前弧度
	virtual float GetDirectionArc()const;
	//获得当前角度
	virtual float GetDirecton()const;
	//设置每次旋转的弧度
	virtual void SetTurnDirectionArc(float turnDir);
	//设置每次旋转的角度
	virtual void SetTurnDirection(float turnDir);
	//获得当前旋转的弧度
	virtual float GetTurnDirectionArc()const;
	//获得当前旋转的角度
	virtual float GetTurnDirection()const;
	
	//判断设置与获取方向\旋转方向是否有效
	virtual bool IsActive()const;
	//判断设置于获取旋转方向是否有效
	virtual void SetActive(bool bActive);
	//设置每个绘图单元所占用范围
	virtual void SetRect(const RectF rect);
	//获取每个绘图单元所占用范围
	virtual RectF GetRect()const;
	//设置对象移动速度
	virtual void SetObjectSpeed(float speed);
	//获取对象移动速度
	virtual float GetObjectSpeed()const;
private:
	//子类的私有属性了(学到这里也要逐渐建立面向对象的编程思维了)--共有属性\方法抽象到父类,通过继承派生出各不相同的子类
	float m_Direction{0};   //当前方向角度
	//float m_DirectionArc{0};//当前方向弧度
	float m_TurnDirection{5};//旋转方向角度
	//float m_TurnDirectionArc;//旋转方向弧度
	bool m_bActive{false};      //是否有效
	RectF m_Rect{0,0,100,100};        //绘图单元
	float m_Speed{10};       //移动速度
};