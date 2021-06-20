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
#define MAPSIZE 19  //�߼���ͼ��С
#define PLAYROW 10 //���������
#define PLAYARRAY 9 //���������
#define ENEMYROW 8 //����������
#define ENEMYARRAY 9 //����������
using std::list;  //��std��,��ʹ�����е�list
//��ͼ��Ķ���--������Ҫ������ؿ��̳�,�����Ϊ������
class GMap
{
public:
	GMap();            //���캯��
	virtual ~GMap();  //��������
	void DrawMap(HDC& hdc);//���Ƶ�ͼ
	void DrawBean(HDC& hdc);//���ƶ���
	friend class GObject; //���常��Ҫ���õ�GMap�ĳ�Ա---����Ϊ��Ԫ
	friend class BeanMan; //����ҲҪ����---ͬ����Ϊ��Ԫ
protected:
	static int mHinderSize; //�ϰ���ߴ�
	static int mBeanSize;   //���Ӱ뾶
	void InitOP();         //��ʼ������,�������������ڵ��޶���
	bool mapData[MAPSIZE][MAPSIZE]; //�ϰ����߼���ͼ--��λ����(��ά����)
	bool beanMapData[MAPSIZE][MAPSIZE];//�����߼���ͼ---��ά����(�����ϰ���/���ӵ�ͼ�϶�����ͬһ�ŵ�ͼ,��һ����)
	COLORREF mColor;//ɫ�ʱ���color,����COLORREF����Ϊ˫��DWORD
};
//��ͼ���๹�����,�ȴ���Ӧ���������̳�(����ÿ����ͼ�ؿ���������ʾ,������涨�������͵�ָ��������װ���йؿ�)
/*!
 * \class: stage_1
 * \brief: �ؿ�1
 * \author: Daito
 * \date: 2021/06/04 14:55
 * \other:
 */
class stage_1:public GMap
{
public:
	stage_1();//���๹�캯��
private:
	bool static initMapData[MAPSIZE][MAPSIZE];
};
//�ڶ���
class stage_2:public GMap
{
public:
	stage_2();//���๹�캯��
private:
	bool static initMapData[MAPSIZE][MAPSIZE];
};
//������
class stage_3 :public GMap
{
public:
	stage_3();//���๹�캯��
private:
	bool static initMapData[MAPSIZE][MAPSIZE];
};

