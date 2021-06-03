#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include<cstring>
#include<iostream>
using namespace std;
/*!
 * \class: 商品类
 * \brief: 用来代表一个商品的基本属性--价格\编号\名字\库存等
 * \author: Daito
 * \date: 2021/05/27 10:53
 * \other:
 */
class Commodity
{
public:
	Commodity(char name[],float buyMoney,float sellMoney);        //商品类的构造函数
	Commodity(int id, char name[], float buyMoney, float sellMoney, int stock);//构造函数的重载
	~Commodity();       //商品类的析构函数
	//获取类的私有属性值,但是间接获取,无法修改的
	int getId();           //获取编号
	int getStock();        //获取库存
	float getBuyMoney();   //获取买入价格
	float getSellMoney();  //获取出售价格
	char* getName();      //获取商品名字
	void updateStock(int newStock);//更新库存
	friend class Trade;  //友元类,谁要调用把谁设为友元

private:
	int static kinds;    //商品种类
	int id;              //商品编号
	int stock;           //商品库存
	float buyMoney;      //买进价格
	float sellMoney;     //卖出价格
	char name[30];       //商品名字
};
/**
//以后类的写法统一改写为:  .h文件只写声明.cpp文件写函数实现
 
Commodity::Commodity()
{
}

Commodity::~Commodity()
{
}*/