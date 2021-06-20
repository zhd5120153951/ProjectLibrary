/*!
 * \file: GObject.cpp
 * \author: Daito
 * \date: 2021/06/17 19:46
 * \version: 1.0.0
 * \adress: https://blog.csdn.net/zhd5120153951
 * \e-mail: 2462491568@qq.com
 */

#include "GObject.h"
#include "framework.h"
GMap* GObject::pStage = NULL;
GObject::GObject(int Row, int Array)//这是初始化的
{
	mFrame = 1;
	pStage = NULL;
	this->mLRow = Row;//不用this也可以,但是用this更装逼一点
	this->mLArray = Array;  
	this->mPoint.x = mLArray*pStage->mHinderSize + pStage->mHinderSize / 2;//这是把二维矩阵的列转为对应的横坐标x
	this->mPoint.y = mLRow*pStage->mHinderSize + pStage->mHinderSize / 2;//行转为纵坐标y
	this->mX = mPoint.x;//得到真实坐标
	this->mY = mPoint.y;
}
//这个是游戏中更改的
void GObject::SetPosition(int Row, int Array)
{
	mLRow = Row;
	mLArray = Array;
	this->mPoint.x = mLArray * pStage->mHinderSize + pStage->mHinderSize / 2;
	this->mPoint.y = mLRow*pStage->mHinderSize + pStage->mHinderSize / 2;
}

void GObject::DrawBlank(HDC& hdc)
{
	RECT rect;//矩形结构体
	rect.left = mX - RD;
	rect.top = mY - RD;
	rect.right = mX + RD;
	rect.bottom = mY + RD;
	FillRect(hdc, &rect, ::CreateSolidBrush(RGB(255, 255, 255)));
}

int GObject::GetRow()
{
	return mLRow;
}

int GObject::GetArray()
{
	return mLArray;
}

bool GObject::IsArrive()
{
	int x = (mPoint.x - pStage->mHinderSize / 2) % pStage->mHinderSize;
	int y = (mPoint.y - pStage->mHinderSize / 2) % pStage->mHinderSize;
	bool i = (x == 0 && y == 0);
	return i;
}

bool GObject::Collision()
{
	bool b = false;
	ArriveCtrl();//更新数据,要是豆子人,则会执行BeanMan重写的ArriveCtrl()消除豆子
	//判断指令的有效性
	if (mLArray<0||mLArray>MAPSIZE-1||mLRow<0||mLRow>MAPSIZE-1)
	{
		b = true;//碰撞了
	}
	else if(IsArrive())
	{
		switch (twCommand)//判断豆子人的前进方向
		{
		case UP:
			if (mLRow > 0 && !pStage->mapData[mLRow - 1][mLArray])
				b = true;
			break;
		case DOWN:
			if (mLRow < MAPSIZE - 1 && !pStage->mapData[mLRow + 1][mLArray])
				b = true;
			break;
		case LEFT:
			if (mLArray>0&&!pStage->mapData[mLRow][mLArray-1])//判断写一个格子是否能通过(1--无墙,0--有墙)
				b = true;//无法通过,撞墙了
			break;
		case RIGHT:
			if (mLArray < MAPSIZE - 1 && !pStage->mapData[mLRow][mLArray + 1])
				b = true;
			break;
		}
		if(!b)mToWard = twCommand;//按键按下的朝向正确,没撞墙
	}
	//按照真实方向前进
	mX = mPoint.x;
	mY = mPoint.y;
	int MAX = pStage->mHinderSize*MAPSIZE + pStage->mHinderSize / 2;
	int MIN = pStage->mHinderSize / 2;
	switch (mToWard)
	{
	case UP:
		if (mLRow > 0 && !pStage->mapData[mLRow - 1][mLArray])
		{
			b = true;
			break;
		}
		mPoint.y -= mSpeed;
		if (mPoint.y < MIN)
		{
			mPoint.y = MAX;
		}
		break;
	case DOWN:
		if (mLRow < MAPSIZE-1 && !pStage->mapData[mLRow + 1][mLArray])
		{
			b = true;
			break;
		}
		mPoint.y += mSpeed;
		if (mPoint.y > MAX)
		{
			mPoint.y = MIN;
		}
		break;
	case LEFT:
		if(mLArray>0&&!pStage->mapData[mLRow][mLArray-1])
		{
			b = true;
			break;
		}
		mPoint.x -= mSpeed;
		if (mPoint.x<MIN)
		{
			mPoint.x = MAX;
		}
		break;
	case RIGHT:
		if (mLArray <MAPSIZE-1 && !pStage->mapData[mLRow][mLArray + 1])
		{
			b = true;
			break;
		}
		mPoint.x += mSpeed;
		if (mPoint.x>MAX)
		{
			mPoint.x = MIN;
		}
		break;
	}
	return b;
}

int GObject::PtTransform(int k)
{
	return (k - (pStage->mHinderSize) / 2) / pStage->mHinderSize;
}

void GObject::ArriveCtrl()
{
	if (IsArrive())
	{
		mLArray = PtTransform(mPoint.x);//更新行
		mLRow = PtTransform(mPoint.y);  //更新列
	}
}

//豆子人的构造函数
BeanMan::BeanMan(int x, int y):GObject(x,y)//继承时,用父类构造函数,初始化自己
{
	this->mSpeed = PLAYERSPEED;
	twCommand = mToWard = LEFT;//豆子人初始移动速度和方向
}

POINT BeanMan::GetPos()
{
	return mPoint;
}

toWards BeanMan::GetTW()
{
	return mToWard;
}

bool BeanMan::IsWin()
{
	for(int i =0;i<=MAPSIZE;i++)
		for(int j=0;j<=MAPSIZE;j++)
			if (pStage->beanMapData[i][j] == true)return false;//A=true(有豆子)B=false(无豆子)
	return true;//否则获胜
}
//画豆子人
void BeanMan::Draw(HDC& hdc)
{
	if (mToWard==OVER)
	{

	}
	else if(mFrame%2==0)//每隔两帧把豆子人画出来
	{
		int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
		int offsetX = DISTANCE / 2 + DRAWBIAS;//豆子人嘴巴(弧弦交点)
		int offsetY = DISTANCE / 2 + DRAWBIAS;//同上
		switch (mToWard)
		{
		case UP:
			x1 = mPoint.x - offsetX;
			x2 = mPoint.x + offsetX;
			y1 = y2 = mPoint.y - offsetY;
			break;
		case DOWN:
			x1 = mPoint.x + offsetX;
			x2 = mPoint.x - offsetX;
			y1 = y2 = mPoint.y + offsetY;
			break;
		case LEFT:
			y1 = mPoint.y + offsetY;
			y2 = mPoint.y - offsetY;
			x1 = x2 = mPoint.x - offsetX;
			break;
		case RIGHT:
			y1 = mPoint.y - offsetY;
			y2 = mPoint.y + offsetY;
			x1 = x2 = mPoint.x + offsetX;
			break;
		}
		Arc(hdc, mPoint.x - DISTANCE, mPoint.y - DISTANCE, 
			mPoint.x + DISTANCE, mPoint.y + DISTANCE, 
			x1, y1, x2, y2);
		MoveToEx(hdc, x1, y1, NULL);
		LineTo(hdc, mPoint.x, mPoint.y);
		LineTo(hdc, x2, y2);
	}
	else if(mFrame%3==0)//每隔三帧
	{
		Ellipse(hdc, mPoint.x - DISTANCE, mPoint.y - DISTANCE, 
			mPoint.x + DISTANCE, mPoint.y + DISTANCE);//检测是否碰撞
	}
	else
	{
		int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
		switch (mToWard)
		{
		case UP:
			x1 = mPoint.x - DISTANCE;
			x2 = mPoint.x + DISTANCE;
			y1 = y2 = mPoint.y;
			break;
		case DOWN:
			x1 = mPoint.x + DISTANCE;
			x2 = mPoint.x - DISTANCE;
			y1 = y2 = mPoint.y;
			break;
		case LEFT:
			y1 = mPoint.y + DISTANCE;
			y2 = mPoint.y - DISTANCE;
			x1 = x2 = mPoint.x;
			break;
		case RIGHT:
			y1 = mPoint.y - DISTANCE;
			y2 = mPoint.y + DISTANCE;
			x1 = x2 = mPoint.x;
			break;
		}
		Arc(hdc, mPoint.x - DISTANCE, mPoint.y - DISTANCE, 
			mPoint.x + DISTANCE, mPoint.y + DISTANCE, 
			x1, y1, x2, y2);
		MoveToEx(hdc, x1, y1, NULL);
		LineTo(hdc, mPoint.x, mPoint.y);
		LineTo(hdc, x2, y2);
	 }
	mFrame++;//绘制下一帧
}

void BeanMan::SetTWCommand(toWards command)
{
	twCommand = command;//豆子人的方向更新流程为:设置方向函数--得到方向缓存指令---得到方向函数---得到最终方向
}

void BeanMan::Over()
{
	mToWard = OVER;
}

void BeanMan::Action()
{
	Collision();
}

void BeanMan::ArriveCtrl()
{
	GObject::ArriveCtrl();//调用父类的到达处理函数
	if (IsArrive())
	{
		if (mLRow >= 0 && mLRow < MAPSIZE&&mLArray >= 0 && mLArray < MAPSIZE)//防止数组越界
		{
			if (pStage->beanMapData[mLRow][mLArray])
				pStage->beanMapData[mLRow][mLArray] = false;//这里的作用相当于吃豆子
		}
	}
}
//敌人父类成员函数
BeanMan* Enemy::mPlayer = NULL;//给豆子人类型的指针赋初值,但是声明时是在Enemy类中,所以要加::
Enemy::Enemy(int x, int y):GObject(x,y)
{
	this->mSpeed = ENERMYSPEED;
	mToWard = LEFT;//敌人初始化方向
	twCommand = UP;
}

void Enemy::Draw(HDC& hdc)
{
	HPEN pen = ::CreatePen(0, 0, mColor);//这里的color即为不同敌人的颜色,但是这是父类函数,在子类中color就需要具体了,这就是多态
	HPEN oldPen = (HPEN)SelectObject(hdc, pen);
	Arc(hdc, mPoint.x - DISTANCE, mPoint.y - DISTANCE, 
		mPoint.x + DISTANCE, mPoint.y + DISTANCE, 
		mPoint.x + DISTANCE, mPoint.y, 
		mPoint.x - DISTANCE, mPoint.y);//半圆状的头--这是敌人的共性(父类虚函数其实也就是画共性)
	int const LEGLENGTH = (DISTANCE) / (LEGCOUNT);
	//根据帧数来绘制敌人身体和腿部
	if (mFrame%2==0)
	{
		MoveToEx(hdc, mPoint.x - DISTANCE, mPoint.y, NULL);//矩形状的身子
		LineTo(hdc, mPoint.x - DISTANCE, mPoint.y + DISTANCE - LEGLENGTH);
		MoveToEx(hdc, mPoint.x + DISTANCE, mPoint.y, NULL);
		LineTo(hdc, mPoint.x + DISTANCE, mPoint.y + DISTANCE - LEGLENGTH);
		for (int i=0;i<LEGCOUNT;i++)//从左往右画腿
		{
			Arc(hdc, mPoint.x - DISTANCE + i * 2 * LEGLENGTH, mPoint.y + DISTANCE - 2 * LEGLENGTH,
				mPoint.x - DISTANCE + (i + 1) * 2 * LEGLENGTH, mPoint.y + DISTANCE,
				mPoint.x - DISTANCE + i * 2 * LEGLENGTH, mPoint.y + DISTANCE - LEGLENGTH,
				mPoint.x - DISTANCE + (i + 1) * 2 * LEGLENGTH, mPoint.y + DISTANCE - LEGLENGTH);
		}
	}
	else
	{
		MoveToEx(hdc, mPoint.x - DISTANCE, mPoint.y, NULL);//绘制身体
		LineTo(hdc, mPoint.x - DISTANCE, mPoint.y + DISTANCE);
		MoveToEx(hdc, mPoint.x + DISTANCE, mPoint.y, NULL);
		LineTo(hdc, mPoint.x + DISTANCE, mPoint.y + DISTANCE);
		//从左往右绘制腿部
		MoveToEx(hdc, mPoint.x - DISTANCE, mPoint.y + DISTANCE, NULL);
		LineTo(hdc, mPoint.x - DISTANCE + LEGLENGTH, mPoint.y + DISTANCE - LEGLENGTH);
		for (int i=0;i<LEGCOUNT-1;i++)
		{
			Arc(hdc, mPoint.x - DISTANCE + (1 + i * 2)*LEGLENGTH, mPoint.y + DISTANCE - 2 * LEGLENGTH,
				mPoint.x - DISTANCE + (3 + i * 2) * LEGLENGTH, mPoint.y + DISTANCE,
				mPoint.x - DISTANCE + (1 + i * 2) * LEGLENGTH, mPoint.y + DISTANCE - LEGLENGTH,
				mPoint.x - DISTANCE + (3 + i * 2) * LEGLENGTH, mPoint.y + DISTANCE - LEGLENGTH);
		}
		MoveToEx(hdc, mPoint.x + DISTANCE, mPoint.y + DISTANCE, NULL);
		LineTo(hdc, mPoint.x + DISTANCE - LEGLENGTH, mPoint.y + DISTANCE - LEGLENGTH);
	}
	//根据方向绘制眼睛
	int R = DISTANCE / 5;//眼睛半径
	switch (mToWard)
	{
	case UP:
		Ellipse(hdc, mPoint.x - 2 * R, mPoint.y - 2 * R, mPoint.x, mPoint.y);
		Ellipse(hdc, mPoint.x, mPoint.y - 2 * R, mPoint.x + 2 * R, mPoint.y);
		break;
	case DOWN:
		Ellipse(hdc, mPoint.x - 2 * R, mPoint.y, mPoint.x, mPoint.y + 2 * R);
		Ellipse(hdc, mPoint.x, mPoint.y, mPoint.x + 2 * R, mPoint.y + 2 * R);
		break;
	case LEFT:
		Ellipse(hdc, mPoint.x - 3 * R, mPoint.y - R, mPoint.x - R, mPoint.y + R);
		Ellipse(hdc, mPoint.x - R, mPoint.y - R, mPoint.x + R, mPoint.y + R);
		break;
	case RIGHT:
		Ellipse(hdc, mPoint.x - R, mPoint.y - R, mPoint.x + R, mPoint.y + R);
		Ellipse(hdc, mPoint.x + R, mPoint.y - R, mPoint.x + 3 * R, mPoint.y + R);
		break;
	}
	mFrame++;//准备下一帧的绘制
	SelectObject(hdc, oldPen);
	DeleteObject(pen);
	return;
}

void Enemy::Action()
{
	bool b = Collision();
	MakeDecision(b);
	Catch();
}
//随机敌人的构造函数
void Enemy::Catch()
{
	int DX = mPoint.x - mPlayer->GetPos().x;
	int DY = mPoint.y - mPlayer->GetPos().y;
	if ((-RD < DX&&DX < RD) && (-RD < DY&&DY < RD))
		mPlayer->Over();
	
}

RandEnemy::RandEnemy(int x, int y):Enemy(x,y)
{
	mColor = RGB(255, 0, 0);
}

void RandEnemy::Draw(HDC& hdc)
{
	Enemy::Draw(hdc);//画的功能都一样,所以不同的子类都调用父类(Enemy父类)的画图函数
}

void RandEnemy::MakeDecision(bool b)
{
	int i = rand();
	if (b)//bool类型的参数用来判断是否改变方向(撞墙时)
	{
		//方向按逆时针来转动---这是敌人的转动方向(AI的)
		if (i%4==0)
		{
			mToWard == UP ? twCommand = LEFT : twCommand = UP;//敌人如果向上撞墙,则左转--left,否则就让它向上
		}else if (i%3==0)
		{
			mToWard == DOWN ? twCommand = RIGHT : twCommand = DOWN;//敌人如果向下撞墙,则左转--right,否则就让其向下
		}else if (i%2==0)
		{
			mToWard == RIGHT ? twCommand = UP : twCommand = RIGHT;//同理可得
		}
		else
		{
			mToWard == LEFT ? twCommand = DOWN : twCommand = LEFT;
		}
		return;
	}
	if (i%4==0)
	{
		twCommand != UP ? mToWard == DOWN : twCommand == UP;
	}
	else if (i % 3 == 0)
	{
		mToWard != DOWN ? twCommand = UP : twCommand = DOWN;
	}
	else if (i % 2 == 0)
	{
		mToWard != RIGHT ? twCommand = LEFT : twCommand = RIGHT;
	}
	else
	{
		mToWard != LEFT ? twCommand = RIGHT : twCommand = LEFT;
	}
}

DefendEnemy::DefendEnemy(int x, int y):RandEnemy(x,y)
{
	mColor = RGB(0, 0, 255);
}

void DefendEnemy::Draw(HDC& hdc)
{
	Enemy::Draw(hdc);//多态
}

void DefendEnemy::MakeDecision(bool b)
{
	const int DR = this->mLRow - mPlayer->GetRow();
	const int DA = this->mLArray - mPlayer->GetArray();
	if (!b&&DR==0)
	{
		if (DA<=BULE_ALERT&&DA>0)//玩家处于左侧警戒范围
		{
			twCommand = LEFT;//敌人自动向左移动聚集
			return;
		}
		if (DA<0&&DA>=-BULE_ALERT)//玩家处于右侧警戒范围
		{
			twCommand = RIGHT;
			return;
		}
	}
	if (!b&&DA==0)
	{
		if (DR<=BULE_ALERT&&DR>0)//玩家处于下方警戒范围
		{
			twCommand = UP;
			return;
		}
		if (DR<0&&DR>=-BULE_ALERT)//玩家处于上方警戒范围
		{
			twCommand = DOWN;
			return;
		}
	}
	RandEnemy::MakeDecision(b);//敌人不在防守模式时,与普通的随机敌人一样
}

AttackEnemy::AttackEnemy(int x, int y):RandEnemy(x,y)
{
	mColor = RGB(200, 200, 150);
}

void AttackEnemy::Draw(HDC& hdc)
{
	Enemy::Draw(hdc);
}

void AttackEnemy::MakeDecision(bool b)
{
	const int DR = this->mLRow - mPlayer->GetRow();
	const int DA = this->mLArray - mPlayer->GetArray();
	if (!b)
	{
		if (DR*DA>DA*DA)
		{
			if (DA > 0)//玩家在左侧警戒范围
			{
				twCommand = LEFT;//敌人自动向左移动聚集
				return;
			}else if(DA<0)//玩家在右侧警戒范围
			{
				twCommand = RIGHT;
				return;
			 }
		}
		else
		{
			if (DR > 0)//玩家处于下侧警戒范围
			{
				twCommand = UP;
				return;
			}
			if (DR<0)
			{
				twCommand = DOWN;
				return;
			}
		}
	}
	RandEnemy::MakeDecision(b);
}
