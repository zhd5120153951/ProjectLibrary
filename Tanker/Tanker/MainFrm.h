/*!
 * \file: MainFrm.h
 * \author: Daito
 * \date: 2021/09/04 9:58
 * \version: 1.0.0
 * \adress: https://blog.csdn.net/zhd5120153951
 * \e-mail: 2462491568@qq.com
 */
// MainFrm.h: CMainFrame 类的接口

#pragma once
#include "CGame.h"
class CMainFrame : public CFrameWnd
{
	
public:
	//CMainFrame() noexcept;
	CMainFrame();
protected: 
	DECLARE_DYNAMIC(CMainFrame)

// 特性
public:

// 操作
public:

// 重写
public:
	

// 实现
public:
	virtual ~CMainFrame();


// 生成的消息映射函数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
	int m_Width = 800;//C++中允许赋值时不写=
	int m_Height{ 600 };
	enum ETimerID{ETimeIdGameLoop=1};//定时器ID,而且设为从1开始
	
	CGame m_Game;//声明一个游戏对象----用来接受鼠标\键盘\窗口传来的消息
public:
	//这里的函数都是需要CMainFrm窗口类来相应的函数
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//鼠标消息的响应函数声明
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);//鼠标移动处理函数
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);//鼠标左键抬起处理函数
};


