#pragma once
#include "CGameEntry.h"
class CGame;
class CGameMenuPanel2 :
	public CGameEntryMoveable
{
public:
	CGameMenuPanel2();
	virtual ~CGameMenuPanel2();
	
	//绘制自己的虚函数
	virtual void Draw(Gdiplus::Graphics& gh)const;
	//处理鼠标移动事件
	void OnMouseMove(UINT nFlags, CPoint point);
	//处理鼠标左键抬起事件
	void OnLButtonUp(UINT nFlags, CPoint point);
public:
	CGame* m_pParent{ nullptr };//游戏类指针
	//菜单图片
	Image* m_Img{ nullptr };
	//绘图状态
	bool m_bNormal{ true };//默认正常
};

