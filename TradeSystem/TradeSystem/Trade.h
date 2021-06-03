#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <list>//stl�е�����ģ���
#include "Commodity.h"
using namespace std;
/*!
 * \class: Trade��
 * \brief: ������,����洢����ȡ��Ʒ��Ϣ,���Ҵ��������ύ���ͻ���
 * \author: Daito
 * \date: 2021/05/27 11:28
 * \other:
 */
class Trade
{
	//������ĳ�Ա(�����ͺ���������Ȩ���޶�Ҳ����û��)
	struct record
	{
		char name[30];  //��¼��Ʒ����
		int count;      //��¼��Ʒ��������
		char sTime[70]; //��¼��Ʒ����ʱ��
		//C++�нṹ���ڿ��Զ��庯��,C���Բ�����,����Ҫ����:C++�Ľṹ���÷���������
		//record�ǽṹ����,Ҳ����ṹ�庯��,�൱�ڹ��캯��,��ʼ���õ�
		record(char* name, int count, char* time) 
		{
			strcpy(this->name, name);
			this->count = count;
			strcpy(sTime, time);//��Ҫ����:this������˭�Ķ���ָ��
		}
	};//����ṹ���������,Ҳ������Ϊ����
public:
	bool getInfo(int index); //��ȡ��Ʒ��Ϣ
	void getIndex();         //��ȡ��ƷĿ¼
	bool Init();             //�ӱ����ļ���ȡ��Ʒ��Ϣ����ʼ��
	void Save();             //������Ʒ��Ϣ������
	bool Buy(int id, int count);//������Ʒ(�������������ݼ��)
	bool Sell(int id, int count);//������Ʒ(�������������ݼ��)
	void addNew(char name[], float buyMoney, float sellMoney);//����µ���Ʒ
	//��ȡ��Ʒ�������������ļ�¼
	void getBuyRecord();
	void getSellRecord();

private:
	list<Commodity>dataList;//��Ʒ��������,������ÿ������Ԫ��Ϊ:Commodity����
	list<record>buyRecord;  //�����¼�ṹ��������
	list<record>sellRecord; //���ۼ�¼�ṹ��������
};
