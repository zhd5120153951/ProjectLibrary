#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <list>//stl中的链表模板库
#include "Commodity.h"
using namespace std;
/*!
 * \class: Trade类
 * \brief: 交易类,负责存储和提取商品信息,并且处理数据提交给客户端
 * \author: Daito
 * \date: 2021/05/27 11:28
 * \other:
 */
class Trade
{
	//类里面的成员(变量和函数可以有权限限定也可以没有)
	struct record
	{
		char name[30];  //记录商品名字
		int count;      //记录商品交易数量
		char sTime[70]; //记录商品交易时间
		//C++中结构体内可以定义函数,C语言不可以,最重要的是:C++的结构体用法和类相似
		//record是结构体名,也是其结构体函数,相当于构造函数,初始化用的
		record(char* name, int count, char* time) 
		{
			strcpy(this->name, name);
			this->count = count;
			strcpy(sTime, time);//需要区分:this是属于谁的对象指针
		}
	};//这个结构体跟类相似,也可以作为对象
public:
	bool getInfo(int index); //获取商品信息
	void getIndex();         //获取商品目录
	bool Init();             //从本地文件获取商品信息来初始化
	void Save();             //保存商品信息到本地
	bool Buy(int id, int count);//购买商品(包含操作与数据检查)
	bool Sell(int id, int count);//销售商品(包含操作与数据检查)
	void addNew(char name[], float buyMoney, float sellMoney);//添加新的商品
	//获取商品的买入与卖出的记录
	void getBuyRecord();
	void getSellRecord();

private:
	list<Commodity>dataList;//商品对象链表,链表中每个数据元素为:Commodity对象
	list<record>buyRecord;  //购买记录结构对象链表
	list<record>sellRecord; //出售记录结构对象链表
};
