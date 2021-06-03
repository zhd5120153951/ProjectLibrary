#include "Commodity.h"
#include "Trade.h"
#include <iostream>
using namespace std;
void toBuy(int& id,int& count)
{
	cout << "请输入购买商品编号：" << endl;
	cin >> id;
	getchar();
	cout << "请输入购买商品数量:" << endl;
	cin >> count;
	getchar();
}
void toSell(int& id,int& count)
{
	cout << "请输入卖出商品编号：" << endl;
	cin >> id;
	getchar();
	cout << "请输入卖出商品数量:" << endl;
	cin >> count;
	getchar();
}
void Operate()
{
	cout << "按任意键继续!";
	getchar();
	system("cls");
}
void drawWindows()
{
	cout << "======================================"<<"\n";
	cout << "       ********商品目录************       " << "\n";
	cout << "======================================" << "\n";
}
void drawLine()
{
	cout << "======================================" << "\n";
}
void drawMainMenu()
{
	cout << "\t" << "=====================================" << "\n";
	cout << "\t" << "||        欢迎使用商品销售系统     ||" << "\n";
	cout << "\t" << "=====================================" << "\n";
	cout << "\t" << "||             1-买进商品          ||" << "\n";
	cout << "\t" << "||             2-卖出商品          ||" << "\n";
	cout << "\t" << "||             3-增加新品          ||" << "\n";
	cout << "\t" << "||             4-查看商品信息      ||" << "\n";
	cout << "\t" << "||             5-查看采购记录      ||" << "\n";
	cout << "\t" << "||             6-查看销售记录      ||" << "\n";
	cout << "\t" << "||             7-退出系统          ||" << "\n";
	cout << "\t" << "=====================================" << "\n";
}
void drawBuyRecord()
{
	system("cls");
	cout << "\t" << "=====================================" << "\n";
	cout << "\t" << "         ******采购记录***********         " << "\n";
	cout << "\t" << "=====================================" << "\n";
}
void drawSellRecord()
{
	system("cls");
	cout << "\t" << "=====================================" << "\n";
	cout << "\t" << "         ******销售记录***********         " << "\n";
	cout << "\t" << "=====================================" << "\n";
}
int main(int argc,char* argv[])
{
	Trade myTrade;
	//刚开始没有txt,初始化一定失败
	if (!myTrade.Init())
		myTrade = Trade();//但是必须要构造myTrade
	bool quitFlag = false;
	while (!quitFlag)
	{
		drawMainMenu();
		cout << "请输入你的选择:";
		int choice;
		cin >> choice;
		getchar();
		int id, count;//用来存放购买和出售的编号和数量
		switch (choice)
		{
		case 1:
			drawWindows();
			myTrade.getIndex();
			drawLine();
			toBuy(id, count);//其实就是输入id和count
			if (myTrade.Buy(id,count))
			{
				system("cls");
				cout << "买进成功";
			}
			else
			{
				system("cls");
				cout << "买进失败";
			}
			Operate();
			break;
		case 2:
			drawWindows();
			myTrade.getIndex();//得到商品编号,但一开始没有,所以不会显示
			drawLine();
			toSell(id, count);
			if (myTrade.Sell(id,count))
			{
				system("cls");
				cout << "卖出成功";
			}
			else
			{
				system("cls");
				cout << "您的输入有误!";
			}
			Operate();
			break;
		case 3:
			char name[30];
			float iPrice, oPrice;
			system("cls");
			cout << "请输入购入商品名称:" << endl;
			cin >> name;
			getchar();
			cout << "请输入购入商品价格:" << endl;
			cin >> iPrice;
			getchar();
			cout << "请输入商品出售价格:" << endl;
			cin >> oPrice;
			getchar();
			myTrade.addNew(name, iPrice, oPrice);
			system("cls");
			cout << "操作成功";
			Operate();
			break;
		case 4:
			drawWindows();
			myTrade.getIndex();//得到商品编号,但一开始没有,所以不会显示
			drawLine();
			cout << "请输入商品编号:";
			int index;
			cin >> index;
			getchar();
			system("cls");
			if (!myTrade.getInfo(index))
			{
				cout << "无效的商品编号!";
				Operate();
			}
			else
				Operate();
			break;
		case 5:
			drawBuyRecord();
			myTrade.getBuyRecord();
			Operate();
			break;
		case 6:
			drawSellRecord();
			myTrade.getSellRecord();
			Operate();
			break;
		case 7:
			quitFlag = true;//quitFlag==是标志位=false不退出,true推出
			break;
		default:
			system("cls");
			cout << "无效的选项!";
			Operate();
		}
	}
	myTrade.Save();
	return 0;
}