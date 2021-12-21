#include "pch.h"
#include "CGameMenuBackground.h"
#include "CGame.h"
CGameMenuBackground::CGameMenuBackground()
{
	m_ImgBackground = Image::FromFile(_T("menu_background.png"));//加载背景图到该背景类的成员变量中
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
	gh.DrawImage(m_ImgBackground, m_rect);//背景图即为整个800,600
}
