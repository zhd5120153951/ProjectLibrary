#pragma once
#include "CGameEntry.h"
#include "CWall.h"
#include "CBullet.h"
#include <vector>
class CGamePathFinder;
/*
	代表中地图中的一个方块
	四周可能有墙不可通过， 中间是空地可以通过----类的细分--比如这里地图类还可以细分为地图元素类,
	这样可以更加细致的描述地图对象
*/
class CGameMapElement :
	public CGameEntryMoveable
{
public:
	CGameMapElement();
	CGameMapElement(float x, float y, bool bLeft, bool bRight, bool bBottom, float wallLen = 10, float spaceLen = 80);
	virtual ~CGameMapElement();
	virtual void Draw(Graphics& gh)const;
	//墙的方向
	typedef enum { EWLeft = 0, EWTop, EWRight, EWBottom, EWSize }EWallIndex;
	//重载运算符--可以像数组那样取到本地块的某个墙
	const CWall& operator[](CGameMapElement::EWallIndex eIndex)const;
	//设置空地的颜色
	void SetSpaceColor(Color clr);
private:
	Color m_spaceColor{ 0xE6,0xE6,0xE6 };//空地的颜色(这里还可以用RGB(12,12,12)这种表达)
	float m_wallWidth{ 10 };//墙的宽度
	float m_spaceWidth = 80;//空地的宽度
	CWall m_wallArray[4];//每面墙都是一个墙类,所以用墙类型数组
};
//整个地图类
class CGameMap
	:public CGameEntryMoveable
{
public:
	CGameMap();
	CGameMap(int x, int y, int width, int height);
	virtual ~CGameMap();
	//绘制地图
	virtual void Draw(Graphics& gh)const;
	//加载地图
	void LoadMap();
	//子弹撞墙处理
	bool HitWallProcess(const CBullet& bullet)const;
	//判断子弹是否撞墙--如查撞上了，返回-true和返射角,撞墙的时间
	bool GetHitWallInfo(const CBullet& bullet, float* pTime = nullptr, float* pDir = nullptr)const;
	// 坦克是否撞墙
	bool IsHitWall(const CTankEntry& tanker, bool bForward) const;
	// 获得一个可以放置物品的位置，位置是随机的
	bool FindRandomPosition(PointF& ptCenter) const;
	// 判断当前物所在的坐标, x : 返回在第几行,y : 返回在第几列
	bool FindObjPosition(const CGameEntryMoveable& obj, int& row, int& col) const;
	// 路径 搜索
	// path 返回路径 [起点，。。。， 终点]
	void FindPath(int startX, int startY, int targetX, int targetY, CPathFinderBFS::CPath& path)const;
	// 获得下一次的前进方向
	bool FindNextDirection(float* dir, int startX, int startY, int targetX, int targetY) const;
	// 获得某方块的中心点位置
	PointF GetElementAreaCenterPos(int row, int col) const;
	// 判断现在发射子弹是否可以打到玩家
	bool IsCounterTarget(const CGameEntryMoveable& bot, const CGameEntryMoveable& target, float* dir = nullptr) const;
private:
	//存放所有地图的基本元素的二维数组--二维数组的方式
	std::vector<std::vector<CGameMapElement>> m_mapArray;
	// 整个地图的范围(0,0)--(800,600)像素
	Gdiplus::Rect m_mapRect{ 0, 0, 800, 600 };
	// 地图中墙的宽度
	int m_wallWidth{ 7 };
	// 地图中中间空位的宽度
	int m_spaceLen{ 42 };
	// 路径搜索
	CGamePathFinder* m_finder{ nullptr };
};
