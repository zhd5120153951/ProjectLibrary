/*!
 * \file: CGameMenuBackground.h
 * \author: Daito
 * \date: 2021/09/09 16:53
 * \version: 1.0.0
 * \CSDN: https://blog.csdn.net/zhd5120153951
 * \GitHub: https://github.com/zhd5120153951
 * \e-mail: 2462491568@qq.com
 * \description:这是游戏类包含下的一个背景图类--用于实现背景图的绘制
 */

#pragma once
#include "CGameEntry.h"
class CGame;
class CGameMenuBackground :
	public CGameEntryMoveable
{
public:
	CGame* m_Parent = NULL;//游戏类指针
	RectF m_rect{ 0,0,800,600 };//背景图范围
	Image* m_ImgBackground = NULL;//背景图指针
	CGameMenuBackground();
	~CGameMenuBackground();
	void SetParent(CGame *g);
	virtual void Draw(Graphics& gh)const;//绘制背景图的函数
};

