#include "Commodity.h"

//��ʼ����ƷID
int Commodity::kinds = 0;
Commodity::Commodity(char name[], float buyMoney, float sellMoney)
{
	kinds++;  //�����������ʽ���
	id = kinds;
	this->buyMoney = buyMoney;
	this->sellMoney = sellMoney;
	strcpy(this->name, name);
}

Commodity::Commodity(int id, char name[], float buyMoney, float sellMoney, int stock)
{
	kinds++;
	this->id = id;//���������ı��治ͬ,����һ��
	this->stock = stock;
	this->buyMoney = buyMoney;
	this->sellMoney = sellMoney;
	strcpy(this->name, name);
}

Commodity::~Commodity()
{

}
//��Щ���ǽӿں���,��Ϊ�˷����û�ʹ�����˽�����Ե��޷��޸�.
int Commodity::getId()
{
	return id;
}

int Commodity::getStock()
{
	return stock;
}

float Commodity::getBuyMoney()
{
	return buyMoney;
}

float Commodity::getSellMoney()
{
	return sellMoney;
}

char* Commodity::getName()
{
	return name;
}

void Commodity::updateStock(int newStock)
{
	stock += newStock;
}
