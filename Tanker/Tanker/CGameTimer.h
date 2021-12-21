#pragma once
#include <windows.h>
//游戏整体时间计数类
class CGameTimer
{
public:
	explicit CGameTimer(unsigned int timeout = 3000, unsigned int timeval = 300)
		:m_timeout(timeout), m_timeval(timeval) {
	}
	virtual ~CGameTimer(){}
	//设置时间间隔
	void SetTimeval(unsigned int timeval);
	//设置上一次时间值
	void SetLastTime();
	//重设起始时间为当前时间---这个是因为有多个子弹,每个子弹的起始时间不一样
	void SetStartTimer();
	//设置总的超时时间
	void SetAllTimeOut(unsigned int timeout);
	//判断是否超时
	bool IsTimeout()const;
	//判断子弹是否达到飞行时间间隔,到了-是否更新为上次时间值
	bool IsTimeval(bool bUpdate = true);//默认更新
private:
	unsigned int m_timeStart{ ::GetTickCount() };//计时器开始时间--系统定时器随机记录一个时间值
	unsigned int m_timeout{ 3000 };//总共超时时间值
	unsigned int m_timeLast{ m_timeStart };//上一次更新时间
	unsigned int m_timeval{ 300 };//时间间隔
};
//判断时间间隔的类--主要用来判断两次旋转的时间差
class CGameTimeval
{
public:
	explicit CGameTimeval(unsigned int timeval = 300):m_timeval(timeval)
	{}
	virtual ~CGameTimeval();
	void SetTimeval(unsigned int timeval);//设置时间间隔
	void SetLastTime();//重设起始时间:当前时间
	bool IsTimeval(bool bUpdate = true);//是否达到时间间隔,到了则更新上次时间为最新时间
private:
	unsigned int m_timeval{ 300 };//时间间隔
	unsigned int m_timeLast{ ::GetTickCount() };//上次更新时间
};