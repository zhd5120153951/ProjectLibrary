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
GObject::GObject(int Row, int Array)//���ǳ�ʼ����
{
	mFrame = 1;
	pStage = NULL;
	this->mLRow = Row;//����thisҲ����,������this��װ��һ��
	this->mLArray = Array;  
	this->mPoint.x = mLArray*pStage->mHinderSize + pStage->mHinderSize / 2;//���ǰѶ�ά�������תΪ��Ӧ�ĺ�����x
	this->mPoint.y = mLRow*pStage->mHinderSize + pStage->mHinderSize / 2;//��תΪ������y
	this->mX = mPoint.x;//�õ���ʵ����
	this->mY = mPoint.y;
}
//�������Ϸ�и��ĵ�
void GObject::SetPosition(int Row, int Array)
{
	mLRow = Row;
	mLArray = Array;
	this->mPoint.x = mLArray * pStage->mHinderSize + pStage->mHinderSize / 2;
	this->mPoint.y = mLRow*pStage->mHinderSize + pStage->mHinderSize / 2;
}

void GObject::DrawBlank(HDC& hdc)
{
	RECT rect;//���νṹ��
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
	ArriveCtrl();//��������,Ҫ�Ƕ�����,���ִ��BeanMan��д��ArriveCtrl()��������
	//�ж�ָ�����Ч��
	if (mLArray<0||mLArray>MAPSIZE-1||mLRow<0||mLRow>MAPSIZE-1)
	{
		b = true;//��ײ��
	}
	else if(IsArrive())
	{
		switch (twCommand)//�ж϶����˵�ǰ������
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
			if (mLArray>0&&!pStage->mapData[mLRow][mLArray-1])//�ж�дһ�������Ƿ���ͨ��(1--��ǽ,0--��ǽ)
				b = true;//�޷�ͨ��,ײǽ��
			break;
		case RIGHT:
			if (mLArray < MAPSIZE - 1 && !pStage->mapData[mLRow][mLArray + 1])
				b = true;
			break;
		}
		if(!b)mToWard = twCommand;//�������µĳ�����ȷ,ûײǽ
	}
	//������ʵ����ǰ��
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
		mLArray = PtTransform(mPoint.x);//������
		mLRow = PtTransform(mPoint.y);  //������
	}
}

//�����˵Ĺ��캯��
BeanMan::BeanMan(int x, int y):GObject(x,y)//�̳�ʱ,�ø��๹�캯��,��ʼ���Լ�
{
	this->mSpeed = PLAYERSPEED;
	twCommand = mToWard = LEFT;//�����˳�ʼ�ƶ��ٶȺͷ���
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
			if (pStage->beanMapData[i][j] == true)return false;//A=true(�ж���)B=false(�޶���)
	return true;//�����ʤ
}
//��������
void BeanMan::Draw(HDC& hdc)
{
	if (mToWard==OVER)
	{

	}
	else if(mFrame%2==0)//ÿ����֡�Ѷ����˻�����
	{
		int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
		int offsetX = DISTANCE / 2 + DRAWBIAS;//���������(���ҽ���)
		int offsetY = DISTANCE / 2 + DRAWBIAS;//ͬ��
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
	else if(mFrame%3==0)//ÿ����֡
	{
		Ellipse(hdc, mPoint.x - DISTANCE, mPoint.y - DISTANCE, 
			mPoint.x + DISTANCE, mPoint.y + DISTANCE);//����Ƿ���ײ
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
	mFrame++;//������һ֡
}

void BeanMan::SetTWCommand(toWards command)
{
	twCommand = command;//�����˵ķ����������Ϊ:���÷�����--�õ����򻺴�ָ��---�õ�������---�õ����շ���
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
	GObject::ArriveCtrl();//���ø���ĵ��ﴦ����
	if (IsArrive())
	{
		if (mLRow >= 0 && mLRow < MAPSIZE&&mLArray >= 0 && mLArray < MAPSIZE)//��ֹ����Խ��
		{
			if (pStage->beanMapData[mLRow][mLArray])
				pStage->beanMapData[mLRow][mLArray] = false;//����������൱�ڳԶ���
		}
	}
}
//���˸����Ա����
BeanMan* Enemy::mPlayer = NULL;//�����������͵�ָ�븳��ֵ,��������ʱ����Enemy����,����Ҫ��::
Enemy::Enemy(int x, int y):GObject(x,y)
{
	this->mSpeed = ENERMYSPEED;
	mToWard = LEFT;//���˳�ʼ������
	twCommand = UP;
}

void Enemy::Draw(HDC& hdc)
{
	HPEN pen = ::CreatePen(0, 0, mColor);//�����color��Ϊ��ͬ���˵���ɫ,�������Ǹ��ຯ��,��������color����Ҫ������,����Ƕ�̬
	HPEN oldPen = (HPEN)SelectObject(hdc, pen);
	Arc(hdc, mPoint.x - DISTANCE, mPoint.y - DISTANCE, 
		mPoint.x + DISTANCE, mPoint.y + DISTANCE, 
		mPoint.x + DISTANCE, mPoint.y, 
		mPoint.x - DISTANCE, mPoint.y);//��Բ״��ͷ--���ǵ��˵Ĺ���(�����麯����ʵҲ���ǻ�����)
	int const LEGLENGTH = (DISTANCE) / (LEGCOUNT);
	//����֡�������Ƶ���������Ȳ�
	if (mFrame%2==0)
	{
		MoveToEx(hdc, mPoint.x - DISTANCE, mPoint.y, NULL);//����״������
		LineTo(hdc, mPoint.x - DISTANCE, mPoint.y + DISTANCE - LEGLENGTH);
		MoveToEx(hdc, mPoint.x + DISTANCE, mPoint.y, NULL);
		LineTo(hdc, mPoint.x + DISTANCE, mPoint.y + DISTANCE - LEGLENGTH);
		for (int i=0;i<LEGCOUNT;i++)//�������һ���
		{
			Arc(hdc, mPoint.x - DISTANCE + i * 2 * LEGLENGTH, mPoint.y + DISTANCE - 2 * LEGLENGTH,
				mPoint.x - DISTANCE + (i + 1) * 2 * LEGLENGTH, mPoint.y + DISTANCE,
				mPoint.x - DISTANCE + i * 2 * LEGLENGTH, mPoint.y + DISTANCE - LEGLENGTH,
				mPoint.x - DISTANCE + (i + 1) * 2 * LEGLENGTH, mPoint.y + DISTANCE - LEGLENGTH);
		}
	}
	else
	{
		MoveToEx(hdc, mPoint.x - DISTANCE, mPoint.y, NULL);//��������
		LineTo(hdc, mPoint.x - DISTANCE, mPoint.y + DISTANCE);
		MoveToEx(hdc, mPoint.x + DISTANCE, mPoint.y, NULL);
		LineTo(hdc, mPoint.x + DISTANCE, mPoint.y + DISTANCE);
		//�������һ����Ȳ�
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
	//���ݷ�������۾�
	int R = DISTANCE / 5;//�۾��뾶
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
	mFrame++;//׼����һ֡�Ļ���
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
//������˵Ĺ��캯��
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
	Enemy::Draw(hdc);//���Ĺ��ܶ�һ��,���Բ�ͬ�����඼���ø���(Enemy����)�Ļ�ͼ����
}

void RandEnemy::MakeDecision(bool b)
{
	int i = rand();
	if (b)//bool���͵Ĳ��������ж��Ƿ�ı䷽��(ײǽʱ)
	{
		//������ʱ����ת��---���ǵ��˵�ת������(AI��)
		if (i%4==0)
		{
			mToWard == UP ? twCommand = LEFT : twCommand = UP;//�����������ײǽ,����ת--left,�������������
		}else if (i%3==0)
		{
			mToWard == DOWN ? twCommand = RIGHT : twCommand = DOWN;//�����������ײǽ,����ת--right,�������������
		}else if (i%2==0)
		{
			mToWard == RIGHT ? twCommand = UP : twCommand = RIGHT;//ͬ��ɵ�
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
	Enemy::Draw(hdc);//��̬
}

void DefendEnemy::MakeDecision(bool b)
{
	const int DR = this->mLRow - mPlayer->GetRow();
	const int DA = this->mLArray - mPlayer->GetArray();
	if (!b&&DR==0)
	{
		if (DA<=BULE_ALERT&&DA>0)//��Ҵ�����ྯ�䷶Χ
		{
			twCommand = LEFT;//�����Զ������ƶ��ۼ�
			return;
		}
		if (DA<0&&DA>=-BULE_ALERT)//��Ҵ����Ҳྯ�䷶Χ
		{
			twCommand = RIGHT;
			return;
		}
	}
	if (!b&&DA==0)
	{
		if (DR<=BULE_ALERT&&DR>0)//��Ҵ����·����䷶Χ
		{
			twCommand = UP;
			return;
		}
		if (DR<0&&DR>=-BULE_ALERT)//��Ҵ����Ϸ����䷶Χ
		{
			twCommand = DOWN;
			return;
		}
	}
	RandEnemy::MakeDecision(b);//���˲��ڷ���ģʽʱ,����ͨ���������һ��
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
			if (DA > 0)//�������ྯ�䷶Χ
			{
				twCommand = LEFT;//�����Զ������ƶ��ۼ�
				return;
			}else if(DA<0)//������Ҳྯ�䷶Χ
			{
				twCommand = RIGHT;
				return;
			 }
		}
		else
		{
			if (DR > 0)//��Ҵ����²ྯ�䷶Χ
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
