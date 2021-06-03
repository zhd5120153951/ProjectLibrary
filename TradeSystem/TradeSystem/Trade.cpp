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
			cout << "��Ʒ���:" << itor->getId() << "\n"
				<< "��Ʒ����:" << itor->getName() << "\n"
				<< "����۸�:" << itor->getBuyMoney() << "\n"
				<< "���ۼ۸�:" << itor->getSellMoney() << "\n"
				<< "ʣ����:" << itor->getStock() << "\n";
			return true;
		}
	}
	return false;
}
//�õ���ƷĿ¼������
void Trade::getIndex()
{
	for (auto itor=dataList.begin();itor!=dataList.end();itor++)
	{
		cout << "\t" << "��Ʒ���:" << itor->getId() << "   " << "��Ʒ����:" << itor->getName() << "\n";
	}

}

bool Trade::Init()
{
	ifstream file("stock.txt");//��ȡ�ļ�
	if (!file.fail())
	{
		char titles[CAP][TLEN] = { "ID:","NAME:","COST:","VALUE:","STOCK:" };
		char saves[CAP][ALEN] = {};
		int tIndex = 0;
		char buf[128];
		int kinds = 0;//��Ʒ�����������
		for (int i=0;i<3;i++)
		{
			file.getline(buf, 128);
		}
		while (!file.eof())//eof()������ʾ�ļ�ָ���Ƿ��ļ�β--û��(����false)--���ļ�β(true)
		{
			char temp[TLEN] = "";//��ȡ�ļ����ݵ��ַ�����
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
								return false;//�в�����β������Ч�ַ�����,��ʱʧЧ
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
		file << "       ********��Ʒ��Ϣ********       "<<"\n";
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
		cout << "���ۼ�¼�ļ�����ʧ��";
	}
	file.close();
	file.open("sellRecord.txt");
	if(!file.fail())
	{
		file << "====================================" << "\n";
		file << "     *********������Ϣ**********    " << "\n";
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
		cout << "���ۼ�¼�ļ�����ʧ��.";
	}
	file.close();
	file.open("buyRecord.txt");
	if (!file.fail())
	{
		file << "====================================" << "\n";
		file << "     *********������Ϣ**********    " << "\n";
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
		cout << "�����¼�ļ�����ʧ��.";
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
			strftime(temp, sizeof(temp), "%Y��%m��%d��%X%A", localtime(&t));
			buyRecord.push_back(record(itor->name, count, temp));
			return true;//����ɹ�
		}
	}
	return false;//����ʧ��
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
			strftime(temp, sizeof(temp), "%Y��%m��%d��%X%A", localtime(&t));
			sellRecord.push_back(record(itor->name, count, temp));
			return true;
		}
	}
	return false;
}

void Trade::addNew(char name[], float buyMoney, float sellMoney)
{
	dataList.push_back(Commodity(name,buyMoney,sellMoney));//ѹ��ı�����Commodity�����͵Ķ���,�����ǳ�ʼ��
}

void Trade::getBuyRecord()
{
	for (auto itor=buyRecord.begin();itor!=buyRecord.end();itor++)
	{
		cout << "\t������Ʒ���ƣ�" << itor->name << "\n";
		cout << "\t������Ʒ���ڣ�" << itor->sTime << "\n";
		cout << "\t������Ʒ������" << itor->count << "\n";
		cout << "\t===================================="<<"\n";
	}
}

void Trade::getSellRecord()
{
	for (auto itor = sellRecord.begin(); itor != sellRecord.end(); itor++)
	{
		cout << "\t������Ʒ���ƣ�" << itor->name << "\n";
		cout << "\t������Ʒ���ڣ�" << itor->sTime << "\n";
		cout << "\t������Ʒ������" << itor->count << "\n";
		cout << "\t===================================="<<"\n";
	}
}

