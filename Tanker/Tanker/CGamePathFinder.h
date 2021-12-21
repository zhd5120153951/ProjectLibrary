#pragma once
#include "CWall.h"
#include "CBullet.h"
#include "CGameMap.h"
#include <memory>
class CGamePathFinder
{
public:
	CGamePathFinder(const vector<vector<CGameMapElement>>& mapArray);
	virtual ~CGamePathFinder();
	// 路径 搜索
   // 两点之间的寻路:判断两个地图块之间是否连通
   // 使用广度优先算法，查找最小路径
   // x,y:起始位置
   // dx,dy:最终位置
	virtual void FindPath(int startX, int startY, int targetX, int targetY, CPath &path) = 0;
protected:
	int m_targetX, m_targetY;//目标坐标点
	int m_row, m_column;//地图的行列数
#define MAX_STEP 99999999
	//记录最小步数 : 初始设置一个很大的值，如果查找完值还是这个值，则说明不是连通的
	int m_minStep{ MAX_STEP };
	// 标记 哪些路径是否走过 : 走过=true,否则=false
	vector<vector<bool>> m_mark;
	// 记录临时搜索路径
	CPathFinderBFS::CPath m_tempPath;
	// 地图的索引
	const vector<vector<CGameMapElement>>& m_mapArray;
};
//广度优先搜索bfs
class CPathFinderBFS
	:public CGamePathFinder
{
public:
	//记录路径的节点类
	class CPath
	{
	public:
		int m_x, m_y;//节点坐标
		int m_s;//步数
		shared_ptr<CPath> m_parent;//智能指针--父节点
		CPath();
		CPath(int x, int y, int s, CPath* ptr = nullptr);
		CPath(const CPath& other);
		CPath& operator=(const CPath& other);
		//virtual ~CPath();
	};
public:
	CPathFinderBFS(const vector<vector<CGameMapElement>>& mapArray);
	virtual ~CPathFinderBFS();
	// 路径 搜索
	// 使用广度优先算法，查找最小路径
	// x,y:起始位置
	// dx,dy:最终位置--重写父类的路径搜寻函数
	virtual void FindPath(int startX, int startY, int targetX, int targetY, CPath& path);
private:
	vector<vector<CPath>>m_pathNodes;//路径节点数组
	// 定义深度优先搜索函数
	//  x,y 当前所在的行，列
	bool dfs_Item(int x, int y);
	//bfs广度优先搜索
	bool bfs(int x, int y, int targetX, int targetY, CPath& path);
};

