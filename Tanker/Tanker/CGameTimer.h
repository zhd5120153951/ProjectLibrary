#pragma once
#include <windows.h>
//��Ϸ����ʱ�������
class CGameTimer
{
public:
	explicit CGameTimer(unsigned int timeout = 3000, unsigned int timeval = 300)
		:m_timeout(timeout), m_timeval(timeval) {
	}
	virtual ~CGameTimer(){}
	//����ʱ����
	void SetTimeval(unsigned int timeval);
	//������һ��ʱ��ֵ
	void SetLastTime();
	//������ʼʱ��Ϊ��ǰʱ��---�������Ϊ�ж���ӵ�,ÿ���ӵ�����ʼʱ�䲻һ��
	void SetStartTimer();
	//�����ܵĳ�ʱʱ��
	void SetAllTimeOut(unsigned int timeout);
	//�ж��Ƿ�ʱ
	bool IsTimeout()const;
	//�ж��ӵ��Ƿ�ﵽ����ʱ����,����-�Ƿ����Ϊ�ϴ�ʱ��ֵ
	bool IsTimeval(bool bUpdate = true);//Ĭ�ϸ���
private:
	unsigned int m_timeStart{ ::GetTickCount() };//��ʱ����ʼʱ��--ϵͳ��ʱ�������¼һ��ʱ��ֵ
	unsigned int m_timeout{ 3000 };//�ܹ���ʱʱ��ֵ
	unsigned int m_timeLast{ m_timeStart };//��һ�θ���ʱ��
	unsigned int m_timeval{ 300 };//ʱ����
};
//�ж�ʱ��������--��Ҫ�����ж�������ת��ʱ���
class CGameTimeval
{
public:
	explicit CGameTimeval(unsigned int timeval = 300):m_timeval(timeval)
	{}
	virtual ~CGameTimeval();
	void SetTimeval(unsigned int timeval);//����ʱ����
	void SetLastTime();//������ʼʱ��:��ǰʱ��
	bool IsTimeval(bool bUpdate = true);//�Ƿ�ﵽʱ����,����������ϴ�ʱ��Ϊ����ʱ��
private:
	unsigned int m_timeval{ 300 };//ʱ����
	unsigned int m_timeLast{ ::GetTickCount() };//�ϴθ���ʱ��
};