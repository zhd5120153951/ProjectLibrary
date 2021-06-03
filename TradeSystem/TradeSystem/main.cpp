#include "Commodity.h"
#include "Trade.h"
#include <iostream>
using namespace std;
void toBuy(int& id,int& count)
{
	cout << "�����빺����Ʒ��ţ�" << endl;
	cin >> id;
	getchar();
	cout << "�����빺����Ʒ����:" << endl;
	cin >> count;
	getchar();
}
void toSell(int& id,int& count)
{
	cout << "������������Ʒ��ţ�" << endl;
	cin >> id;
	getchar();
	cout << "������������Ʒ����:" << endl;
	cin >> count;
	getchar();
}
void Operate()
{
	cout << "�����������!";
	getchar();
	system("cls");
}
void drawWindows()
{
	cout << "======================================"<<"\n";
	cout << "       ********��ƷĿ¼************       " << "\n";
	cout << "======================================" << "\n";
}
void drawLine()
{
	cout << "======================================" << "\n";
}
void drawMainMenu()
{
	cout << "\t" << "=====================================" << "\n";
	cout << "\t" << "||        ��ӭʹ����Ʒ����ϵͳ     ||" << "\n";
	cout << "\t" << "=====================================" << "\n";
	cout << "\t" << "||             1-�����Ʒ          ||" << "\n";
	cout << "\t" << "||             2-������Ʒ          ||" << "\n";
	cout << "\t" << "||             3-������Ʒ          ||" << "\n";
	cout << "\t" << "||             4-�鿴��Ʒ��Ϣ      ||" << "\n";
	cout << "\t" << "||             5-�鿴�ɹ���¼      ||" << "\n";
	cout << "\t" << "||             6-�鿴���ۼ�¼      ||" << "\n";
	cout << "\t" << "||             7-�˳�ϵͳ          ||" << "\n";
	cout << "\t" << "=====================================" << "\n";
}
void drawBuyRecord()
{
	system("cls");
	cout << "\t" << "=====================================" << "\n";
	cout << "\t" << "         ******�ɹ���¼***********         " << "\n";
	cout << "\t" << "=====================================" << "\n";
}
void drawSellRecord()
{
	system("cls");
	cout << "\t" << "=====================================" << "\n";
	cout << "\t" << "         ******���ۼ�¼***********         " << "\n";
	cout << "\t" << "=====================================" << "\n";
}
int main(int argc,char* argv[])
{
	Trade myTrade;
	//�տ�ʼû��txt,��ʼ��һ��ʧ��
	if (!myTrade.Init())
		myTrade = Trade();//���Ǳ���Ҫ����myTrade
	bool quitFlag = false;
	while (!quitFlag)
	{
		drawMainMenu();
		cout << "���������ѡ��:";
		int choice;
		cin >> choice;
		getchar();
		int id, count;//������Ź���ͳ��۵ı�ź�����
		switch (choice)
		{
		case 1:
			drawWindows();
			myTrade.getIndex();
			drawLine();
			toBuy(id, count);//��ʵ��������id��count
			if (myTrade.Buy(id,count))
			{
				system("cls");
				cout << "����ɹ�";
			}
			else
			{
				system("cls");
				cout << "���ʧ��";
			}
			Operate();
			break;
		case 2:
			drawWindows();
			myTrade.getIndex();//�õ���Ʒ���,��һ��ʼû��,���Բ�����ʾ
			drawLine();
			toSell(id, count);
			if (myTrade.Sell(id,count))
			{
				system("cls");
				cout << "�����ɹ�";
			}
			else
			{
				system("cls");
				cout << "������������!";
			}
			Operate();
			break;
		case 3:
			char name[30];
			float iPrice, oPrice;
			system("cls");
			cout << "�����빺����Ʒ����:" << endl;
			cin >> name;
			getchar();
			cout << "�����빺����Ʒ�۸�:" << endl;
			cin >> iPrice;
			getchar();
			cout << "��������Ʒ���ۼ۸�:" << endl;
			cin >> oPrice;
			getchar();
			myTrade.addNew(name, iPrice, oPrice);
			system("cls");
			cout << "�����ɹ�";
			Operate();
			break;
		case 4:
			drawWindows();
			myTrade.getIndex();//�õ���Ʒ���,��һ��ʼû��,���Բ�����ʾ
			drawLine();
			cout << "��������Ʒ���:";
			int index;
			cin >> index;
			getchar();
			system("cls");
			if (!myTrade.getInfo(index))
			{
				cout << "��Ч����Ʒ���!";
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
			quitFlag = true;//quitFlag==�Ǳ�־λ=false���˳�,true�Ƴ�
			break;
		default:
			system("cls");
			cout << "��Ч��ѡ��!";
			Operate();
		}
	}
	myTrade.Save();
	return 0;
}