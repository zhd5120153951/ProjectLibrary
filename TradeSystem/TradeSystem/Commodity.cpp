#include "Commodity.h"

//初始化商品ID
int Commodity::kinds = 0;
Commodity::Commodity(char name[], float buyMoney, float sellMoney)
{
	kinds++;  //编号以自增方式添加
	id = kinds;
	this->buyMoney = buyMoney;
	this->sellMoney = sellMoney;
	strcpy(this->name, name);
}

Commodity::Commodity(int id, char name[], float buyMoney, float sellMoney, int stock)
{
	kinds++;
	this->id = id;//这里和上面的表面不同,本质一样
	this->stock = stock;
	this->buyMoney = buyMoney;
	this->sellMoney = sellMoney;
	strcpy(this->name, name);
}

Commodity::~Commodity()
{

}
//这些都是接口函数,是为了方便用户使用类的私有属性但无法修改.
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
