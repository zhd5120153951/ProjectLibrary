#pragma once
#include "pch.h"
#include "CGameMenuPanel.h"    //������Ϸ���м���˵��ͱ�������ͷ�ļ�
#include "CGameMenuBackground.h"
class CGame
{
public:
	CGame::CGame();
	CGame::~CGame();
	void SetHandle(HWND hWnd);//����������ڵľ��
	bool EnterFrame(DWORD dwTime);//������Ϸ֡
	void OnMouseMove(UINT nFlags, CPoint point);//��������ƶ��¼�
	void OnLButtonUp(UINT nFlags, CPoint point);//����������̧���¼�
private:
	HWND m_hWnd;//���ھ������--ȷ���Ǹ����ڵ�
	// �������˵������ر���
	CGameMenuPanel m_Menu;//��ʼ�˵�����
	CGameMenuBackground m_Background;//��ʼ�˵���������
	//��Ϸ��ͼ����
	void GameRunDraw();//��Ϸ������ƺ���
	void DrawFps(Graphics &gh);    //����֡��
	int m_Fps{ 0 };   //��¼ÿ�����֡
};

