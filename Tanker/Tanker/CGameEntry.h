#pragma once
#include "pch.h"
#include <cmath>
#include <gdiplus.h>
#include <d2d1_1helper.h>
using namespace std;
using namespace Gdiplus;
/*!
 * \class: CGameEntry
 * \brief: ������Ϸ�е����ж���,�������ƶ��Ķ���Ͳ������ƶ�����ĳ���--������
 * \author: Daito
 * \date: 2021/06/24 16:30
 * \other:
 */
class CGameEntry
{
public:
	CGameEntry(){}
	virtual ~CGameEntry(){}
public:
	//���ﷴ����::�������,��������ÿ���ֱ��ʹ��using namespace Gdiplus(ͷ�ļ��Ѿ�����,���������ò��ö�һ��)
	virtual void Draw(Graphics& gh) const = 0;//�����Լ��Ĵ��麯��--��̬
};
//�ƶ���
class CGameEntryMoveable:public CGameEntry
{
public:
	CGameEntryMoveable();//Ĭ�Ϲ���
	virtual ~CGameEntryMoveable();//����
	CGameEntryMoveable(const CGameEntryMoveable& rhs);//��������
	CGameEntryMoveable& operator=(const CGameEntryMoveable& rhs);//=���������
public:
	virtual void Draw(Graphics& gh)const = 0;//����̳и���,����˵Ӧ����д(����ȥʵ���������)������麯����,����������Ҫ��Ϊ����������Բ�д
	/*���ƶ�����Ĺ�ͬ����--��Ա��������Ա����*/
	virtual void RotateRight();//��ת
	virtual void RotateLeft(); //��ת
	virtual void Forward();    //ǰ��
	virtual void Backward();  //����
	virtual float GetSpeedX()const;//���ˮƽ������ٶ�
	virtual float GetSpeedY()const;//��ô�ֱ������ٶ�
	virtual RectF ForwardNextRect()const;//��һ��ǰ����λ��(�����ƶ���λ����һ��RECT���νṹΪ��Ԫ,���Է���ֵΪRect����)
	virtual RectF BackwardNextRect()const;//��һ�����˵�λ��
	virtual PointF GetHeadPos()const;//��ȡͷ��λ��,���Ƶ�ͼΪ����,ͷ��λ�ü��Ƕ�Ϊ0
	                                 //�����㷨:�������ĵ�Ϊ�뾶(�Խ���/2),�������Լ���X�����λ��
	virtual PointF GetCenterPoint()const;//��ȡ���ĵ�
	virtual void SetCenterPoint(const PointF& CenterPoint);//�������ĵ�

	//���Դ�ȡ��
#ifndef PI
#define PI 3.1415926f
#endif // 
	//���ýǶȵ�λ->������:a*PI
	virtual void SetDirectionArc(float dirArc);
	//���ýǶȵ�λ->�Ƕ���:��
	virtual void SetDirection(float dir);
	//��õ�ǰ����
	virtual float GetDirectionArc()const;
	//��õ�ǰ�Ƕ�
	virtual float GetDirecton()const;
	//����ÿ����ת�Ļ���
	virtual void SetTurnDirectionArc(float turnDir);
	//����ÿ����ת�ĽǶ�
	virtual void SetTurnDirection(float turnDir);
	//��õ�ǰ��ת�Ļ���
	virtual float GetTurnDirectionArc()const;
	//��õ�ǰ��ת�ĽǶ�
	virtual float GetTurnDirection()const;
	
	//�ж��������ȡ����\��ת�����Ƿ���Ч
	virtual bool IsActive()const;
	//�ж������ڻ�ȡ��ת�����Ƿ���Ч
	virtual void SetActive(bool bActive);
	//����ÿ����ͼ��Ԫ��ռ�÷�Χ
	virtual void SetRect(const RectF rect);
	//��ȡÿ����ͼ��Ԫ��ռ�÷�Χ
	virtual RectF GetRect()const;
	//���ö����ƶ��ٶ�
	virtual void SetObjectSpeed(float speed);
	//��ȡ�����ƶ��ٶ�
	virtual float GetObjectSpeed()const;
private:
	//�����˽��������(ѧ������ҲҪ�𽥽����������ı��˼ά��)--��������\�������󵽸���,ͨ���̳�������������ͬ������
	float m_Direction{0};   //��ǰ����Ƕ�
	//float m_DirectionArc{0};//��ǰ���򻡶�
	float m_TurnDirection{5};//��ת����Ƕ�
	//float m_TurnDirectionArc;//��ת���򻡶�
	bool m_bActive{false};      //�Ƿ���Ч
	RectF m_Rect{0,0,100,100};        //��ͼ��Ԫ
	float m_Speed{10};       //�ƶ��ٶ�
};