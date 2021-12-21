#include "pch.h"
#include "CGameMenuBackground.h"
#include "CGame.h"
CGameMenuBackground::CGameMenuBackground()
{
	m_ImgBackground = Image::FromFile(_T("menu_background.png"));//���ر���ͼ���ñ�����ĳ�Ա������
}

CGameMenuBackground::~CGameMenuBackground()
{

}

void CGameMenuBackground::SetParent(CGame *g)
{
	m_Parent = g;
}

void CGameMenuBackground::Draw(Graphics& gh) const
{
	gh.DrawImage(m_ImgBackground, m_rect);//����ͼ��Ϊ����800,600
}
