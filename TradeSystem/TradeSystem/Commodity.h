#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include<cstring>
#include<iostream>
using namespace std;
/*!
 * \class: ��Ʒ��
 * \brief: ��������һ����Ʒ�Ļ�������--�۸�\���\����\����
 * \author: Daito
 * \date: 2021/05/27 10:53
 * \other:
 */
class Commodity
{
public:
	Commodity(char name[],float buyMoney,float sellMoney);        //��Ʒ��Ĺ��캯��
	Commodity(int id, char name[], float buyMoney, float sellMoney, int stock);//���캯��������
	~Commodity();       //��Ʒ�����������
	//��ȡ���˽������ֵ,���Ǽ�ӻ�ȡ,�޷��޸ĵ�
	int getId();           //��ȡ���
	int getStock();        //��ȡ���
	float getBuyMoney();   //��ȡ����۸�
	float getSellMoney();  //��ȡ���ۼ۸�
	char* getName();      //��ȡ��Ʒ����
	void updateStock(int newStock);//���¿��
	friend class Trade;  //��Ԫ��,˭Ҫ���ð�˭��Ϊ��Ԫ

private:
	int static kinds;    //��Ʒ����
	int id;              //��Ʒ���
	int stock;           //��Ʒ���
	float buyMoney;      //����۸�
	float sellMoney;     //�����۸�
	char name[30];       //��Ʒ����
};
/**
//�Ժ����д��ͳһ��дΪ:  .h�ļ�ֻд����.cpp�ļ�д����ʵ��
 
Commodity::Commodity()
{
}

Commodity::~Commodity()
{
}*/