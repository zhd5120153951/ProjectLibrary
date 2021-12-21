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
	// ·�� ����
   // ����֮���Ѱ·:�ж�������ͼ��֮���Ƿ���ͨ
   // ʹ�ù�������㷨��������С·��
   // x,y:��ʼλ��
   // dx,dy:����λ��
	virtual void FindPath(int startX, int startY, int targetX, int targetY, CPath &path) = 0;
protected:
	int m_targetX, m_targetY;//Ŀ�������
	int m_row, m_column;//��ͼ��������
#define MAX_STEP 99999999
	//��¼��С���� : ��ʼ����һ���ܴ��ֵ�����������ֵ�������ֵ����˵��������ͨ��
	int m_minStep{ MAX_STEP };
	// ��� ��Щ·���Ƿ��߹� : �߹�=true,����=false
	vector<vector<bool>> m_mark;
	// ��¼��ʱ����·��
	CPathFinderBFS::CPath m_tempPath;
	// ��ͼ������
	const vector<vector<CGameMapElement>>& m_mapArray;
};
//�����������bfs
class CPathFinderBFS
	:public CGamePathFinder
{
public:
	//��¼·���Ľڵ���
	class CPath
	{
	public:
		int m_x, m_y;//�ڵ�����
		int m_s;//����
		shared_ptr<CPath> m_parent;//����ָ��--���ڵ�
		CPath();
		CPath(int x, int y, int s, CPath* ptr = nullptr);
		CPath(const CPath& other);
		CPath& operator=(const CPath& other);
		//virtual ~CPath();
	};
public:
	CPathFinderBFS(const vector<vector<CGameMapElement>>& mapArray);
	virtual ~CPathFinderBFS();
	// ·�� ����
	// ʹ�ù�������㷨��������С·��
	// x,y:��ʼλ��
	// dx,dy:����λ��--��д�����·����Ѱ����
	virtual void FindPath(int startX, int startY, int targetX, int targetY, CPath& path);
private:
	vector<vector<CPath>>m_pathNodes;//·���ڵ�����
	// �������������������
	//  x,y ��ǰ���ڵ��У���
	bool dfs_Item(int x, int y);
	//bfs�����������
	bool bfs(int x, int y, int targetX, int targetY, CPath& path);
};

