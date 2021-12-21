/*!
 * \file: CGameMenuPanel.h
 * \author: Daito
 * \date: 2021/09/09 16:55
 * \version: 1.0.0
 * \CSDN: https://blog.csdn.net/zhd5120153951
 * \GitHub: https://github.com/zhd5120153951
 * \e-mail: 2462491568@qq.com
 * \description:这是游戏内CGame的子类--菜单类--核心功能是对鼠标的消息响应,背景图指针,存放和菜单的数组
 */

#pragma once
#include "CGameEntry.h"
#include <tuple>
#include <vector>
using namespace std;
class CGame;
class CGameMenuPanel :CGameEntryMoveable
{	
public:
	CGameMenuPanel();
	~CGameMenuPanel();
	void SetParent(CGame *ptr_G);//存放一个游戏类的指针在文本中
	virtual void Draw(Graphics &gh)const;//绘制自己
	void OnMouseMove(UINT nFlags, CPoint point);//处理鼠标移动事件函数
	void OnLButtonUp(UINT nFlags, CPoint point);//处理鼠标左键松开事件函数

	CGame *m_PtrParent{ NULL };//声明一个指向游戏类的指针--存放于文本中
	RectF m_Rect{ 0,0,800,600 };//初始宽高范围
	Image *m_ImgBackGround{ NULL };//背景图指针
	struct 
	{
		vector<tuple<Image *, RectF>>vImgInfoPtr;//C++中也有元组类型,是不过tuple是字典--一个key:value(python中是元组)
		int index;//表示菜单的第几项,每项由vector包含信息,其第一项统一为图片信息,第二项为索引--即为该图片的索引
	}m_MenuList[2];
};

