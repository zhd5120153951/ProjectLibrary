/*!
 * \file: GMap.cpp
 * \author: Daito
 * \date: 2021/06/17 19:44
 * \version: 1.0.0
 * \adress: https://blog.csdn.net/zhd5120153951
 * \e-mail: 2462491568@qq.com
 * \description:这是地图父类以及子类的成员函数的实现cpp
 */

#include "GMap.h"

int GMap::mHinderSize = 36;//障碍物尺寸
int GMap::mBeanSize = 3;    //豆子人尺寸
//玩家和敌人所在的豆子地图没有豆子,置为false
void GMap::InitOP()
{
	beanMapData[ENEMYROW][ENEMYARRAY] = false;
	beanMapData[PLAYROW][PLAYARRAY] = false;
}

GMap::GMap()
{
	//这个不需要构造
}

GMap::~GMap()
{
	//自然也不需要析构了
}

void GMap::DrawMap(HDC& hdc)
{
	for (int i=0;i<MAPSIZE;i++)
	{
		for (int j=0;j<MAPSIZE;j++)
		{
			//绘制墙壁
			if (!mapData[i][j])
			{
				RECT rect;
				rect.left = j * mHinderSize;
				rect.top = i * mHinderSize;
				rect.right = (j + 1) * mHinderSize;//上下左右刚好差1就组成了一个方格
				rect.bottom = (i + 1) * mHinderSize;
				FillRect(hdc, &rect, CreateSolidBrush(mColor));
			}
		}
	}
}

void GMap::DrawBean(HDC& hdc)
{
	for (int i=0;i<MAPSIZE;i++)
	{
		for (int j=0;j<MAPSIZE;j++)
		{
			if (beanMapData[i][j])
			{
				Ellipse(hdc, (mHinderSize / 2 - mBeanSize) + j * mHinderSize, (mHinderSize / 2 - mBeanSize) + i * mHinderSize,
					(mHinderSize / 2 + mBeanSize) + j * mHinderSize, (mHinderSize / 2 + mBeanSize) + i * mHinderSize);
			}
		}
	}
}
//关卡的制作
#define A true
#define B false
bool stage_1::initMapData[MAPSIZE][MAPSIZE] = {
	B,B,B,B,B,B,B,B,B,A,B,B,B,B,B,B,B,B,B,//0行
	B,A,A,A,A,A,A,A,A,A,A,A,A,A,A,A,A,A,B,//1行
	B,A,A,B,A,A,B,B,B,A,B,B,B,A,A,B,A,A,B,//2行
	B,A,B,B,A,A,A,A,A,A,A,A,A,A,A,B,B,A,B,//3行
	B,A,B,A,A,A,B,B,B,A,B,B,B,A,A,A,B,A,B,//4行
	B,A,B,A,A,A,A,A,A,A,A,A,A,A,A,A,B,A,B,//5行
	B,A,A,A,A,A,B,B,A,A,A,B,B,A,A,A,A,A,B,//6行
	B,A,B,A,A,A,A,A,A,A,A,A,A,A,A,A,B,A,B,//7行
	B,A,B,A,A,A,A,A,B,A,B,A,A,A,A,A,B,A,B,//8行
	A,A,A,A,A,A,A,A,B,B,B,A,A,A,A,A,A,A,A,//9行
	B,A,B,A,A,A,A,A,A,A,A,A,A,A,A,A,B,A,B,//10行
	B,A,B,A,A,B,A,A,A,A,A,A,A,B,A,A,B,A,B,//11行
	B,A,B,A,B,B,B,A,A,A,A,A,B,B,B,A,B,A,B,//12行
	B,A,A,A,A,B,A,A,A,A,A,A,A,B,A,A,A,A,B,//13行
	B,A,B,B,A,A,A,A,A,A,A,A,A,A,A,B,B,A,B,//14行
	B,A,A,A,A,A,A,A,A,A,A,A,A,A,A,A,A,A,B,//15行
	B,A,A,A,A,B,B,B,A,B,A,B,B,B,A,A,A,A,B,//16行
	B,A,A,A,A,B,A,A,A,A,A,A,A,B,A,A,A,A,B,//17行
	B,B,B,B,B,B,B,B,B,A,B,B,B,B,B,B,B,B,B//18行
};
#undef A
#undef B
stage_1::stage_1()
{
	mColor = RGB(140, 240, 240);
	for (int i=0;i<MAPSIZE;i++)
	{
		for (int j=0;j<MAPSIZE;j++)
		{
			this->mapData[i][j] = this->initMapData[i][j];
			this->beanMapData[i][j] = this->initMapData[i][j];
		}
	}
	//要消除敌我位置的豆子
	this->InitOP();
}
#define A true
#define B false
bool stage_2::initMapData[MAPSIZE][MAPSIZE] = {
	B,B,B,A,B,B,B,B,B,A,B,B,B,B,B,B,B,B,B,//0行
	B,A,A,A,A,A,A,A,A,A,A,A,A,A,A,A,A,A,B,//1行
	B,A,A,B,A,A,B,B,B,A,B,B,B,A,A,B,A,A,B,//2行
	B,A,B,B,A,A,A,A,A,A,A,A,A,A,A,B,B,A,B,//3行
	B,A,B,A,A,A,B,B,B,A,B,B,B,A,A,A,B,A,B,//4行
	B,A,B,A,A,A,A,A,A,B,A,A,A,A,A,A,B,A,B,//5行
	B,A,A,A,A,A,B,B,A,A,A,B,B,A,A,A,A,A,B,//6行
	B,A,B,A,B,A,A,A,A,A,A,A,A,A,B,A,B,A,B,//7行
	B,A,B,A,A,A,A,A,B,A,B,A,A,A,A,A,B,A,B,//8行
	A,A,A,A,A,A,A,A,B,B,B,A,A,A,A,A,A,A,A,//9行
	B,A,B,A,A,A,B,A,A,A,A,A,A,A,A,A,B,A,B,//10行
	B,A,B,A,A,B,A,A,A,A,A,A,A,B,A,A,B,A,B,//11行
	B,A,B,A,B,B,B,A,A,A,A,A,B,B,B,A,B,A,B,//12行
	B,A,A,A,A,B,A,A,A,B,A,A,A,B,A,A,A,A,B,//13行
	B,A,B,B,A,A,A,A,A,B,A,A,A,A,A,B,B,A,B,//14行
	B,A,A,A,A,A,A,A,A,A,A,A,A,A,A,A,A,A,B,//15行
	B,A,A,A,A,B,B,B,A,B,A,B,B,B,A,A,A,A,B,//16行
	B,A,A,A,A,B,A,A,A,A,A,A,A,B,A,A,A,A,B,//17行
	B,B,B,B,B,B,B,B,B,A,B,B,B,B,A,B,B,B,B//18行
};
#undef A
#undef B
stage_2::stage_2()
{
	mColor = RGB(240, 140, 40);
	for (int i = 0; i < MAPSIZE; i++)
	{
		for (int j = 0; j < MAPSIZE; j++)
		{
			this->mapData[i][j] = this->initMapData[i][j];
			this->beanMapData[i][j] = this->initMapData[i][j];
		}
	}
	//要消除敌我位置的豆子
	this->InitOP();
}
#define A true
#define B false
bool stage_3::initMapData[MAPSIZE][MAPSIZE] = {
	B,B,B,A,B,B,B,B,B,A,B,B,B,B,A,B,B,B,B,//0行
	B,A,A,A,A,A,A,A,A,A,A,A,A,B,A,A,A,A,B,//1行
	B,A,A,B,A,A,B,B,B,A,B,B,B,A,A,B,A,A,B,//2行
	B,A,B,B,A,A,A,A,A,A,A,A,A,A,A,B,B,A,B,//3行
	B,A,B,A,B,A,B,B,B,A,B,B,B,A,A,A,B,A,B,//4行
	B,A,B,A,A,A,A,A,A,B,A,A,A,B,A,A,B,A,B,//5行
	B,A,A,A,A,A,B,B,A,A,A,B,B,A,A,A,A,A,B,//6行
	B,A,B,A,B,A,A,A,A,A,A,A,A,A,B,A,B,A,B,//7行
	B,A,B,A,A,A,A,A,B,A,B,A,A,A,A,A,B,A,B,//8行
	A,A,A,A,A,A,A,A,B,B,B,A,A,A,A,A,A,A,A,//9行
	B,A,B,A,A,A,B,A,A,A,A,A,A,A,A,A,B,A,B,//10行
	B,A,B,A,A,B,A,A,B,A,A,A,B,B,A,A,B,A,B,//11行
	A,A,B,A,B,B,B,A,A,A,A,A,B,B,B,A,B,A,B,//12行
	B,A,A,A,A,B,A,A,A,B,A,B,A,B,A,A,A,A,B,//13行
	B,A,B,B,A,A,A,A,A,B,A,A,A,A,A,B,B,A,B,//14行
	B,A,A,A,B,A,A,A,B,A,A,B,A,A,A,A,A,A,B,//15行
	B,A,A,A,A,B,B,B,A,B,A,B,B,B,A,A,A,A,B,//16行
	A,A,A,A,A,B,A,A,A,A,A,A,A,B,A,A,A,A,A,//17行
	B,B,B,B,B,B,B,B,B,A,B,B,B,B,A,B,B,B,B//18行
};
#undef A
#undef B
stage_3::stage_3()
{
	mColor = RGB(240, 140, 40);
	for (int i = 0; i < MAPSIZE; i++)
	{
		for (int j = 0; j < MAPSIZE; j++)
		{
			this->mapData[i][j] = this->initMapData[i][j];
			this->beanMapData[i][j] = this->initMapData[i][j];
		}
	}
	//要消除敌我位置的豆子
	this->InitOP();
}
