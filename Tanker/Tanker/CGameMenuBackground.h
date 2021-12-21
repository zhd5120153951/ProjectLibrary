/*!
 * \file: CGameMenuBackground.h
 * \author: Daito
 * \date: 2021/09/09 16:53
 * \version: 1.0.0
 * \CSDN: https://blog.csdn.net/zhd5120153951
 * \GitHub: https://github.com/zhd5120153951
 * \e-mail: 2462491568@qq.com
 * \description:������Ϸ������µ�һ������ͼ��--����ʵ�ֱ���ͼ�Ļ���
 */

#pragma once
#include "CGameEntry.h"
class CGame;
class CGameMenuBackground :
	public CGameEntryMoveable
{
public:
	CGame* m_Parent = NULL;//��Ϸ��ָ��
	RectF m_rect{ 0,0,800,600 };//����ͼ��Χ
	Image* m_ImgBackground = NULL;//����ͼָ��
	CGameMenuBackground();
	~CGameMenuBackground();
	void SetParent(CGame *g);
	virtual void Draw(Graphics& gh)const;//���Ʊ���ͼ�ĺ���
};

