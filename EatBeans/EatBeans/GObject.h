/*!
 * \file: GObject.h
 * \author: Daito
 * \date: 2021/06/17 19:47
 * \version: 1.0.0
 * \adress: https://blog.csdn.net/zhd5120153951
 * \e-mail: 2462491568@qq.com
 */

#pragma once
#include "framework.h"
#include "GMap.h"
#include <ctime>
#define PLAYERSPEED 6  //玩家移动速度
#define ENERMYSPEED 4  //敌人移动速度
#define LEGCOUNT    5  //敌人大腿数量
#define DISTANCE    10 //图形范围
#define BULE_ALERT  8  //蓝色警戒范围
#define DRAWBIAS    2  //绘图误差像素
#define RD (DISTANCE+DRAWBIAS) //绘图范围=图形范围+误差范围=10+2=12
//方向枚举定义
enum toWards
{
	UP,DOWN,LEFT,RIGHT,OVER
};//枚举默认从0开始的,依次加1
//游戏中的对象父类
/*!
 * \class: GObject
 * \brief: 所有的游戏对象的父类---包含了他的基本共性,然后不同的子类来继承,展示各自的特性
 * \author: Daito
 * \date: 2021/06/04 11:01
 * \other:
 */
class GObject
{
public:
	//GObject();
	GObject(int Row, int Array);  //带参构造
	//~GObject();
	void SetPosition(int Row, int Array);//设置逻辑位置
	void DrawBlank(HDC& hdc); //绘画地图
	void virtual Draw(HDC& hdc) = 0;//纯虚函数,绘制不同继承而来的子类对象
	static GMap* pStage; //地图类类型的指针,可以设置不通关卡,且不可更改(static),敌人和玩家都能使用相同地图
	void virtual Action() = 0;//数据变更的实现函数(各自子类对象的变更不同,所以用到多态--父类虚函数--子类重写)
	int GetRow();  //得到行数
	int GetArray(); //得到列数
protected:
	int mX, mY;  //对象坐标
	toWards twCommand; //方向指令缓存(这个很重要:从键盘输入方向,存入缓冲区,cpu来读取)
	POINT mPoint; //每个小方块中心坐标
	int mLRow, mLArray;//逻辑横(对应mY)纵(对应mX)坐标
	int mSpeed;  //对象移动速度
	toWards mToWard;   //方向
	int mFrame;  //帧数
	//成员函数
	bool IsArrive();  //判断对象是否到达逻辑坐标
	bool Collision(); //碰撞检测,把对象放于合理位置
	int PtTransform(int k); //把对象的实际坐标转换为逻辑坐标(x,y)--(行,列)
	virtual void ArriveCtrl();//到达逻辑点后,更新数据(之所以设为virtul是有不同的继承之类都要各自实现)
};
//子类和父类按理来说,不应该写在同一个.h文件中,但是如果子类内容太少,也可以写在一起
/*!
 * \class: BeanMan
 * \brief: 豆子人类,继承于GObject,其对象即为玩家控制的吃豆子人物
 * \author: Daito
 * \date: 2021/06/04 15:21
 * \other:
 */
class BeanMan:public GObject
{
public:
	BeanMan(int x,int y);//豆子人的带参初始化
	POINT GetPos();//得到自己位置
	toWards GetTW();//得到自己的前进方向toWards是枚举类型
	bool IsWin();//判断玩家是否赢了
	void Draw(HDC& hdc)override;//绘制豆子人--多态--同一个父类虚函数,不同子类的的各自实现,即不同对象调用同一函数,实现不同功能
	void SetTWCommand(toWards command);//更改前进方向()
	void Over();//方向超出边界
	void Action()override;  //父类虚函数,子类重写---形成多态
protected:
	virtual void ArriveCtrl()override;//重写父类虚函数(父类虚,子类重写加override)
};
/*!
 * \class: Enemy
 * \brief: 敌人类,追扑豆子人的子类对象,也是抽象类
 * \author: Daito
 * \date: 2021/06/04 15:38
 * \other:隔代继承时必须要注意virtual该不该去掉哦
 */
class Enemy:public GObject
{
public:
	Enemy(int x, int y);//敌人带参构造(跟豆子人一样都是调用父类构造来初始化自己)
	static BeanMan* mPlayer;//声明一个静态的豆子人类类型的指针mPlayer
	//思考一个问题:子类也为抽象类,那么其还需要实现父类的虚函数吗?
	//答案是:也需要实现,尽管子类对象无法创建并调用它,但是子类的子类可以调用啊
	void virtual Draw(HDC& hdc)override;//绘制敌人(这里设为虚函数-是因为子类有不同的属性,需要在调用时才画出)
	void virtual Action()override;  //这是隔代继承关系,这里在Enemy角度,可以不写virtual,但是enemy的子类却不知道其为虚函数,会导致重载而非重写
protected:
	void Catch();//是否抓住豆子人
	void virtual MakeDecision(bool b) = 0;  //敌人的做决定纯虚函数-----给其子类(Ai)来实现
	COLORREF mColor;
};
/*!
 * \class: RandEnemy
 * \brief: 随机型敌人
 * \author: Daito
 * \date: 2021/06/04 16:00
 * \other:
 */
class RandEnemy:public Enemy
{
public:
	RandEnemy(int x,int y);//构造函数
	void Draw(HDC& hdc)override;//多态,但是这里不需要多态,而是相同态,所以后面的其他类都会一样直接调用父类的Draw函数
protected:
	void virtual MakeDecision(bool b)override;//父类纯虚函数--子类重写--实现多态
};
/*!
 * \class: DefendEnemy
 * \brief: 防御型敌人
 * \author: Daito
 * \date: 2021/06/04 16:00
 * \other:
 */
class DefendEnemy :public RandEnemy
{
public:
	DefendEnemy(int x, int y);//构造函数
	void Draw(HDC& hdc)override;//多态--而且这个函数继承于GObject类,而不是Enemy/RandEnemy类(他自己的都是继承于GObject)
protected:
	void virtual MakeDecision(bool b)override;//父类纯虚函数--子类重写--实现多态
};
/*!
 * \class: AttackEnemy
 * \brief: 进攻型敌人
 * \author: Daito
 * \date: 2021/06/04 16:00
 * \other:
 */
class AttackEnemy :public RandEnemy
{
public:
	AttackEnemy(int x, int y);//构造函数
	void Draw(HDC& hdc)override;//多态--而且这个函数继承于GObject类,而不是Enemy/RandEnemy类(他自己的都是继承于GObject)
protected:
	void virtual MakeDecision(bool b)override;//父类纯虚函数--子类重写--实现多态
};