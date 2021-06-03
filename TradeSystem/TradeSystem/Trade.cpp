#include "Trade.h"
#include <fstream>
#include <ctime>
#define CAP 5
#define TLEN 10
#define ALEN 30
using std::ofstream;
using std::ifstream;

bool Trade::getInfo(int index)
{
	for (auto itor = dataList.begin();itor!=dataList.end();itor++)
	{
		if (itor->getId() == index)
		{
			cout << "商品编号:" << itor->getId() << "\n"
				<< "商品名字:" << itor->getName() << "\n"
				<< "买入价格:" << itor->getBuyMoney() << "\n"
				<< "销售价格:" << itor->getSellMoney() << "\n"
				<< "剩余库存:" << itor->getStock() << "\n";
			return true;
		}
	}
	return false;
}
//得到商品目录和名字
void Trade::getIndex()
{
	for (auto itor=dataList.begin();itor!=dataList.end();itor++)
	{
		cout << "\t" << "商品编号:" << itor->getId() << "   " << "商品名字:" << itor->getName() << "\n";
	}

}

bool Trade::Init()
{
	ifstream file("stock.txt");//读取文件
	if (!file.fail())
	{
		char titles[CAP][TLEN] = { "ID:","NAME:","COST:","VALUE:","STOCK:" };
		char saves[CAP][ALEN] = {};
		int tIndex = 0;
		char buf[128];
		int kinds = 0;//商品种类计数变量
		for (int i=0;i<3;i++)
		{
			file.getline(buf, 128);
		}
		while (!file.eof())//eof()函数表示文件指针是否到文件尾--没到(返回false)--到文件尾(true)
		{
			char temp[TLEN] = "";//读取文件内容的字符数组
			for (int i=0;i<TLEN&&!file.eof();i++)
			{
				file.get(temp[i]);
				if (strcmp(temp,titles[tIndex])==0)
				{
					for (int j=0;j<ALEN&&!file.eof();j++)
					{
						char c;
						file.get(c);
						if (c!='\t'&&c!='\n')
							saves[tIndex][j] = c;
						else if (c!='\n')
						{
							if (tIndex>4)
							{
								return false;//行参数结尾仍有有效字符存在,此时失效
							}
							tIndex++;
							break;
						}
						else
						{
							dataList.push_back(Commodity(atoi(saves[0]), saves[1], atof(saves[2]), atof(saves[3]), atoi(saves[4])));
							tIndex = 0;
							kinds++;
							break;
						}
						if (j == ALEN - 1)
							return false;
					}
					break;
				}
				if (i == TLEN - 1)
					return false;
			}
		}
		Commodity::kinds = kinds;
		return true;
	}
	return false;
}

void Trade::Save()
{
	ofstream file;
	file.open("stock.txt");
	if (!file.fail())
	{
		file << "===================================="<<"\n";
		file << "       ********商品信息********       "<<"\n";
		file << "===================================="<<"\n";
		for (auto itor=dataList.begin();itor!=dataList.end();itor++)
		{
			file << "ID:" << itor->getId()
				<< "\t"<<"NAME:" << itor->getName()
				<< "\t"<<"COST:" << itor->getBuyMoney()
				<< "\t"<<"VALUE:" << itor->getSellMoney()
				<< "\t"<<"STOCK:" << itor->getStock() << "\n";
		}
	}
	else
	{
		cout << "销售记录文件创建失败";
	}
	file.close();
	file.open("sellRecord.txt");
	if(!file.fail())
	{
		file << "====================================" << "\n";
		file << "     *********销售信息**********    " << "\n";
		file << "====================================" << "\n";
		for(auto itor =sellRecord.begin();itor!=sellRecord.end();itor++)
		{
			file << "NAME:" << itor->name
				<< "\tTIME:" << itor->sTime
				<< "\tCOUNT:" << itor->count << "\n";
		 }
	 }
	else
	{
		cout << "销售记录文件创建失败.";
	}
	file.close();
	file.open("buyRecord.txt");
	if (!file.fail())
	{
		file << "====================================" << "\n";
		file << "     *********购买信息**********    " << "\n";
		file << "====================================" << "\n";
		for (auto itor = buyRecord.begin(); itor != buyRecord.end(); itor++)
		{
			file << "NAME:" << itor->name
				<< "\tTIME:" << itor->sTime
				<< "\tCOUNT:" << itor->count << "\n";
		}
	}
	else
	{
		cout << "购买记录文件创建失败.";
	}
}

bool Trade::Buy(int id, int count)
{
	for (auto itor=dataList.begin();itor!=dataList.end();itor++)
	{
		if (itor->getId() == id)
		{
			itor->updateStock(count);
			time_t t = time(0);
			char temp[50];
			strftime(temp, sizeof(temp), "%Y年%m月%d日%X%A", localtime(&t));
			buyRecord.push_back(record(itor->name, count, temp));
			return true;//购买成功
		}
	}
	return false;//购买失败
}

bool Trade::Sell(int id, int count)
{
	for (auto itor=dataList.begin();itor!=dataList.end();itor++)
	{
		if (itor->getId() == id)
		{
			itor->updateStock(-count);
			time_t t = time(0);
			char temp[50];
			strftime(temp, sizeof(temp), "%Y年%m月%d日%X%A", localtime(&t));
			sellRecord.push_back(record(itor->name, count, temp));
			return true;
		}
	}
	return false;
}

void Trade::addNew(char name[], float buyMoney, float sellMoney)
{
	dataList.push_back(Commodity(name,buyMoney,sellMoney));//压入的必须是Commodity的类型的对象,而且是初始化
}

void Trade::getBuyRecord()
{
	for (auto itor=buyRecord.begin();itor!=buyRecord.end();itor++)
	{
		cout << "\t购入商品名称：" << itor->name << "\n";
		cout << "\t购入商品日期：" << itor->sTime << "\n";
		cout << "\t购入商品数量：" << itor->count << "\n";
		cout << "\t===================================="<<"\n";
	}
}

void Trade::getSellRecord()
{
	for (auto itor = sellRecord.begin(); itor != sellRecord.end(); itor++)
	{
		cout << "\t出售商品名称：" << itor->name << "\n";
		cout << "\t出售商品日期：" << itor->sTime << "\n";
		cout << "\t出售商品数量：" << itor->count << "\n";
		cout << "\t===================================="<<"\n";
	}
}

