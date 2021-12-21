/*!
 * \file: GMap.h
 * \author: Daito
 * \date: 2021/06/17 19:46
 * \version: 1.0.0
 * \adress: https://blog.csdn.net/zhd5120153951
 * \e-mail: 2462491568@qq.com
 */

#pragma once
#include "framework.h"
#include <list>
#define MAPSIZE 19  //逻辑地图大小
#define PLAYROW 10 //玩家所在行
#define PLAYARRAY 9 //玩家所在列
#define ENEMYROW 8 //敌人所在行
#define ENEMYARRAY 9 //敌人所在列
using std::list;  //打开std域,以使用其中的list
//地图类的定义--由于需要被多个关卡继承,故设计为抽象类
class GMap
{
public:
	GMap();            //构造函数
	virtual ~GMap();  //析构函数
	void DrawMap(HDC& hdc);//绘制地图
	void DrawBean(HDC& hdc);//绘制豆子
	friend class GObject; //物体父类要调用到GMap的成员---设置为友元
	friend class BeanMan; //子类也要调用---同样设为友元
protected:
	static int mHinderSize; //障碍物尺寸
	static int mBeanSize;   //豆子半径
	void InitOP();         //初始化豆子,且玩家与敌人所在地无豆子
	bool mapData[MAPSIZE][MAPSIZE]; //障碍物逻辑地图--二位矩阵(二维数组)
	bool beanMapData[MAPSIZE][MAPSIZE];//豆子逻辑地图---二维矩阵(这里障碍物/豆子地图肯定都在同一张地图,故一样的)
	COLORREF mColor;//色彩变量color,其中COLORREF类型为双字DWORD
};
//地图父类构建完成,等待对应的子类来继承(而且每个地图关卡用类来表示,方便后面定义类类型的指针数组来装所有关卡)
/*!
 * \class: stage_1
 * \brief: 关卡1
 * \author: Daito
 * \date: 2021/06/04 14:55
 * \other:
 */
class stage_1:public GMap
{
public:
	stage_1();//子类构造函数
private:
	bool static initMapData[MAPSIZE][MAPSIZE];
};
//第二关
class stage_2:public GMap
{
public:
	stage_2();//子类构造函数
private:
	bool static initMapData[MAPSIZE][MAPSIZE];
};
//第三关
class stage_3 :public GMap
{
public:
	stage_3();//子类构造函数
private:
	bool static initMapData[MAPSIZE][MAPSIZE];
};

