/*!
 * \file: GMap.cpp
 * \author: Daito
 * \date: 2021/06/17 19:44
 * \version: 1.0.0
 * \adress: https://blog.csdn.net/zhd5120153951
 * \e-mail: 2462491568@qq.com
 * \description:���ǵ�ͼ�����Լ�����ĳ�Ա������ʵ��cpp
 */

#include "GMap.h"

int GMap::mHinderSize = 36;//�ϰ���ߴ�
int GMap::mBeanSize = 3;    //�����˳ߴ�
//��Һ͵������ڵĶ��ӵ�ͼû�ж���,��Ϊfalse
void GMap::InitOP()
{
	beanMapData[ENEMYROW][ENEMYARRAY] = false;
	beanMapData[PLAYROW][PLAYARRAY] = false;
}

GMap::GMap()
{
	//�������Ҫ����
}

GMap::~GMap()
{
	//��ȻҲ����Ҫ������
}

void GMap::DrawMap(HDC& hdc)
{
	for (int i=0;i<MAPSIZE;i++)
	{
		for (int j=0;j<MAPSIZE;j++)
		{
			//����ǽ��
			if (!mapData[i][j])
			{
				RECT rect;
				rect.left = j * mHinderSize;
				rect.top = i * mHinderSize;
				rect.right = (j + 1) * mHinderSize;//�������Ҹպò�1�������һ������
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
//�ؿ�������
#define A true
#define B false
bool stage_1::initMapData[MAPSIZE][MAPSIZE] = {
	B,B,B,B,B,B,B,B,B,A,B,B,B,B,B,B,B,B,B,//0��
	B,A,A,A,A,A,A,A,A,A,A,A,A,A,A,A,A,A,B,//1��
	B,A,A,B,A,A,B,B,B,A,B,B,B,A,A,B,A,A,B,//2��
	B,A,B,B,A,A,A,A,A,A,A,A,A,A,A,B,B,A,B,//3��
	B,A,B,A,A,A,B,B,B,A,B,B,B,A,A,A,B,A,B,//4��
	B,A,B,A,A,A,A,A,A,A,A,A,A,A,A,A,B,A,B,//5��
	B,A,A,A,A,A,B,B,A,A,A,B,B,A,A,A,A,A,B,//6��
	B,A,B,A,A,A,A,A,A,A,A,A,A,A,A,A,B,A,B,//7��
	B,A,B,A,A,A,A,A,B,A,B,A,A,A,A,A,B,A,B,//8��
	A,A,A,A,A,A,A,A,B,B,B,A,A,A,A,A,A,A,A,//9��
	B,A,B,A,A,A,A,A,A,A,A,A,A,A,A,A,B,A,B,//10��
	B,A,B,A,A,B,A,A,A,A,A,A,A,B,A,A,B,A,B,//11��
	B,A,B,A,B,B,B,A,A,A,A,A,B,B,B,A,B,A,B,//12��
	B,A,A,A,A,B,A,A,A,A,A,A,A,B,A,A,A,A,B,//13��
	B,A,B,B,A,A,A,A,A,A,A,A,A,A,A,B,B,A,B,//14��
	B,A,A,A,A,A,A,A,A,A,A,A,A,A,A,A,A,A,B,//15��
	B,A,A,A,A,B,B,B,A,B,A,B,B,B,A,A,A,A,B,//16��
	B,A,A,A,A,B,A,A,A,A,A,A,A,B,A,A,A,A,B,//17��
	B,B,B,B,B,B,B,B,B,A,B,B,B,B,B,B,B,B,B//18��
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
	//Ҫ��������λ�õĶ���
	this->InitOP();
}
#define A true
#define B false
bool stage_2::initMapData[MAPSIZE][MAPSIZE] = {
	B,B,B,A,B,B,B,B,B,A,B,B,B,B,B,B,B,B,B,//0��
	B,A,A,A,A,A,A,A,A,A,A,A,A,A,A,A,A,A,B,//1��
	B,A,A,B,A,A,B,B,B,A,B,B,B,A,A,B,A,A,B,//2��
	B,A,B,B,A,A,A,A,A,A,A,A,A,A,A,B,B,A,B,//3��
	B,A,B,A,A,A,B,B,B,A,B,B,B,A,A,A,B,A,B,//4��
	B,A,B,A,A,A,A,A,A,B,A,A,A,A,A,A,B,A,B,//5��
	B,A,A,A,A,A,B,B,A,A,A,B,B,A,A,A,A,A,B,//6��
	B,A,B,A,B,A,A,A,A,A,A,A,A,A,B,A,B,A,B,//7��
	B,A,B,A,A,A,A,A,B,A,B,A,A,A,A,A,B,A,B,//8��
	A,A,A,A,A,A,A,A,B,B,B,A,A,A,A,A,A,A,A,//9��
	B,A,B,A,A,A,B,A,A,A,A,A,A,A,A,A,B,A,B,//10��
	B,A,B,A,A,B,A,A,A,A,A,A,A,B,A,A,B,A,B,//11��
	B,A,B,A,B,B,B,A,A,A,A,A,B,B,B,A,B,A,B,//12��
	B,A,A,A,A,B,A,A,A,B,A,A,A,B,A,A,A,A,B,//13��
	B,A,B,B,A,A,A,A,A,B,A,A,A,A,A,B,B,A,B,//14��
	B,A,A,A,A,A,A,A,A,A,A,A,A,A,A,A,A,A,B,//15��
	B,A,A,A,A,B,B,B,A,B,A,B,B,B,A,A,A,A,B,//16��
	B,A,A,A,A,B,A,A,A,A,A,A,A,B,A,A,A,A,B,//17��
	B,B,B,B,B,B,B,B,B,A,B,B,B,B,A,B,B,B,B//18��
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
	//Ҫ��������λ�õĶ���
	this->InitOP();
}
#define A true
#define B false
bool stage_3::initMapData[MAPSIZE][MAPSIZE] = {
	B,B,B,A,B,B,B,B,B,A,B,B,B,B,A,B,B,B,B,//0��
	B,A,A,A,A,A,A,A,A,A,A,A,A,B,A,A,A,A,B,//1��
	B,A,A,B,A,A,B,B,B,A,B,B,B,A,A,B,A,A,B,//2��
	B,A,B,B,A,A,A,A,A,A,A,A,A,A,A,B,B,A,B,//3��
	B,A,B,A,B,A,B,B,B,A,B,B,B,A,A,A,B,A,B,//4��
	B,A,B,A,A,A,A,A,A,B,A,A,A,B,A,A,B,A,B,//5��
	B,A,A,A,A,A,B,B,A,A,A,B,B,A,A,A,A,A,B,//6��
	B,A,B,A,B,A,A,A,A,A,A,A,A,A,B,A,B,A,B,//7��
	B,A,B,A,A,A,A,A,B,A,B,A,A,A,A,A,B,A,B,//8��
	A,A,A,A,A,A,A,A,B,B,B,A,A,A,A,A,A,A,A,//9��
	B,A,B,A,A,A,B,A,A,A,A,A,A,A,A,A,B,A,B,//10��
	B,A,B,A,A,B,A,A,B,A,A,A,B,B,A,A,B,A,B,//11��
	A,A,B,A,B,B,B,A,A,A,A,A,B,B,B,A,B,A,B,//12��
	B,A,A,A,A,B,A,A,A,B,A,B,A,B,A,A,A,A,B,//13��
	B,A,B,B,A,A,A,A,A,B,A,A,A,A,A,B,B,A,B,//14��
	B,A,A,A,B,A,A,A,B,A,A,B,A,A,A,A,A,A,B,//15��
	B,A,A,A,A,B,B,B,A,B,A,B,B,B,A,A,A,A,B,//16��
	A,A,A,A,A,B,A,A,A,A,A,A,A,B,A,A,A,A,A,//17��
	B,B,B,B,B,B,B,B,B,A,B,B,B,B,A,B,B,B,B//18��
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
	//Ҫ��������λ�õĶ���
	this->InitOP();
}
