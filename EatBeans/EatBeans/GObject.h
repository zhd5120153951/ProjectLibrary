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
#define PLAYERSPEED 6  //����ƶ��ٶ�
#define ENERMYSPEED 4  //�����ƶ��ٶ�
#define LEGCOUNT    5  //���˴�������
#define DISTANCE    10 //ͼ�η�Χ
#define BULE_ALERT  8  //��ɫ���䷶Χ
#define DRAWBIAS    2  //��ͼ�������
#define RD (DISTANCE+DRAWBIAS) //��ͼ��Χ=ͼ�η�Χ+��Χ=10+2=12
//����ö�ٶ���
enum toWards
{
	UP,DOWN,LEFT,RIGHT,OVER
};//ö��Ĭ�ϴ�0��ʼ��,���μ�1
//��Ϸ�еĶ�����
/*!
 * \class: GObject
 * \brief: ���е���Ϸ����ĸ���---���������Ļ�������,Ȼ��ͬ���������̳�,չʾ���Ե�����
 * \author: Daito
 * \date: 2021/06/04 11:01
 * \other:
 */
class GObject
{
public:
	//GObject();
	GObject(int Row, int Array);  //���ι���
	//~GObject();
	void SetPosition(int Row, int Array);//�����߼�λ��
	void DrawBlank(HDC& hdc); //�滭��ͼ
	void virtual Draw(HDC& hdc) = 0;//���麯��,���Ʋ�ͬ�̳ж������������
	static GMap* pStage; //��ͼ�����͵�ָ��,�������ò�ͨ�ؿ�,�Ҳ��ɸ���(static),���˺���Ҷ���ʹ����ͬ��ͼ
	void virtual Action() = 0;//���ݱ����ʵ�ֺ���(�����������ı����ͬ,�����õ���̬--�����麯��--������д)
	int GetRow();  //�õ�����
	int GetArray(); //�õ�����
protected:
	int mX, mY;  //��������
	toWards twCommand; //����ָ���(�������Ҫ:�Ӽ������뷽��,���뻺����,cpu����ȡ)
	POINT mPoint; //ÿ��С������������
	int mLRow, mLArray;//�߼���(��ӦmY)��(��ӦmX)����
	int mSpeed;  //�����ƶ��ٶ�
	toWards mToWard;   //����
	int mFrame;  //֡��
	//��Ա����
	bool IsArrive();  //�ж϶����Ƿ񵽴��߼�����
	bool Collision(); //��ײ���,�Ѷ�����ں���λ��
	int PtTransform(int k); //�Ѷ����ʵ������ת��Ϊ�߼�����(x,y)--(��,��)
	virtual void ArriveCtrl();//�����߼����,��������(֮������Ϊvirtul���в�ͬ�ļ̳�֮�඼Ҫ����ʵ��)
};
//����͸��ఴ����˵,��Ӧ��д��ͬһ��.h�ļ���,���������������̫��,Ҳ����д��һ��
/*!
 * \class: BeanMan
 * \brief: ��������,�̳���GObject,�����Ϊ��ҿ��ƵĳԶ�������
 * \author: Daito
 * \date: 2021/06/04 15:21
 * \other:
 */
class BeanMan:public GObject
{
public:
	BeanMan(int x,int y);//�����˵Ĵ��γ�ʼ��
	POINT GetPos();//�õ��Լ�λ��
	toWards GetTW();//�õ��Լ���ǰ������toWards��ö������
	bool IsWin();//�ж�����Ƿ�Ӯ��
	void Draw(HDC& hdc)override;//���ƶ�����--��̬--ͬһ�������麯��,��ͬ����ĵĸ���ʵ��,����ͬ�������ͬһ����,ʵ�ֲ�ͬ����
	void SetTWCommand(toWards command);//����ǰ������()
	void Over();//���򳬳��߽�
	void Action()override;  //�����麯��,������д---�γɶ�̬
protected:
	virtual void ArriveCtrl()override;//��д�����麯��(������,������д��override)
};
/*!
 * \class: Enemy
 * \brief: ������,׷�˶����˵��������,Ҳ�ǳ�����
 * \author: Daito
 * \date: 2021/06/04 15:38
 * \other:�����̳�ʱ����Ҫע��virtual�ò���ȥ��Ŷ
 */
class Enemy:public GObject
{
public:
	Enemy(int x, int y);//���˴��ι���(��������һ�����ǵ��ø��๹������ʼ���Լ�)
	static BeanMan* mPlayer;//����һ����̬�Ķ����������͵�ָ��mPlayer
	//˼��һ������:����ҲΪ������,��ô�仹��Ҫʵ�ָ�����麯����?
	//����:Ҳ��Ҫʵ��,������������޷�������������,���������������Ե��ð�
	void virtual Draw(HDC& hdc)override;//���Ƶ���(������Ϊ�麯��-����Ϊ�����в�ͬ������,��Ҫ�ڵ���ʱ�Ż���)
	void virtual Action()override;  //���Ǹ����̳й�ϵ,������Enemy�Ƕ�,���Բ�дvirtual,����enemy������ȴ��֪����Ϊ�麯��,�ᵼ�����ض�����д
protected:
	void Catch();//�Ƿ�ץס������
	void virtual MakeDecision(bool b) = 0;  //���˵����������麯��-----��������(Ai)��ʵ��
	COLORREF mColor;
};
/*!
 * \class: RandEnemy
 * \brief: ����͵���
 * \author: Daito
 * \date: 2021/06/04 16:00
 * \other:
 */
class RandEnemy:public Enemy
{
public:
	RandEnemy(int x,int y);//���캯��
	void Draw(HDC& hdc)override;//��̬,�������ﲻ��Ҫ��̬,������̬ͬ,���Ժ���������඼��һ��ֱ�ӵ��ø����Draw����
protected:
	void virtual MakeDecision(bool b)override;//���ി�麯��--������д--ʵ�ֶ�̬
};
/*!
 * \class: DefendEnemy
 * \brief: �����͵���
 * \author: Daito
 * \date: 2021/06/04 16:00
 * \other:
 */
class DefendEnemy :public RandEnemy
{
public:
	DefendEnemy(int x, int y);//���캯��
	void Draw(HDC& hdc)override;//��̬--������������̳���GObject��,������Enemy/RandEnemy��(���Լ��Ķ��Ǽ̳���GObject)
protected:
	void virtual MakeDecision(bool b)override;//���ി�麯��--������д--ʵ�ֶ�̬
};
/*!
 * \class: AttackEnemy
 * \brief: �����͵���
 * \author: Daito
 * \date: 2021/06/04 16:00
 * \other:
 */
class AttackEnemy :public RandEnemy
{
public:
	AttackEnemy(int x, int y);//���캯��
	void Draw(HDC& hdc)override;//��̬--������������̳���GObject��,������Enemy/RandEnemy��(���Լ��Ķ��Ǽ̳���GObject)
protected:
	void virtual MakeDecision(bool b)override;//���ി�麯��--������д--ʵ�ֶ�̬
};