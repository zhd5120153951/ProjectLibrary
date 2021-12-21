#pragma once
#include "pch.h"
#include "CGameMenuPanel.h"    //①在游戏类中加入菜单和背景子类头文件
#include "CGameMenuBackground.h"
#include "CTankEntry.h" //游戏主体类对象需要调用坦克类对象(是其一部分也应该视为一个对象)来完成相关功能--这里充分体现着面向对象的思维方式
//#include "CPlayer.h"
//#include "CBullet.h"
//#include "CBot.h"
class CGame
{
public:
	CGame::CGame();
	CGame::~CGame();
	void SetHandle(HWND hWnd);//设置输出窗口的句柄
	bool EnterFrame(DWORD dwTime);//进入游戏帧
	void OnMouseMove(UINT nFlags, CPoint point);//处理鼠标移动事件
	void OnLButtonUp(UINT nFlags, CPoint point);//处理鼠标左键抬起事件
private:
	HWND m_hWnd;//窗口句柄变量--确定那个窗口的
	// ②声明菜单类的相关变量
	CGameMenuPanel m_Menu;//开始菜单对象
	CGameMenuBackground m_Background;//开始菜单背景对象
	//游戏绘图处理
	void GameRunDraw();//游戏对象绘制函数
	void DrawFps(Graphics &gh);    //绘制帧率
	int m_Fps{ 0 };   //记录每秒多少帧
};

