#include "pch.h"
#include "CGame.h"

CGame::CGame()
{

}

CGame::~CGame()
{

}

void CGame::SetHandle(HWND hWnd)
{
	m_hWnd = hWnd;//把当前窗口给窗口变量
}
//进入游戏
bool CGame::EnterFrame(DWORD dwTime)
{
	//开始绘制对象
	GameRunDraw();//具体的实现功能由此函数完成
	GameRunDraw();//调用绘图方法
	return false;
}

void CGame::OnMouseMove(UINT nFlags, CPoint point)
{
	m_Menu.OnMouseMove(nFlags, point);//菜单页面--选择游戏类型--移动选择
}

void CGame::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_Menu.OnLButtonUp(nFlags, point);//菜单页面--选择游戏类型--抬起确定
}
//游戏绘制图形了
void CGame::GameRunDraw()
{
	HDC hdc = ::GetDC(m_hWnd);//得到当前窗口的句柄
	CRect rc;
	GetClientRect(m_hWnd, &rc);//得到当前窗口的大小

	CDC *dc = CClientDC::FromHandle(hdc);

	CDC m_dcMemory;          //双缓冲区,绘图用
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(dc, rc.Width(), rc.Height());
	m_dcMemory.CreateCompatibleDC(dc);
	CBitmap *pOldBitmap = m_dcMemory.SelectObject(&bmp);

	Graphics gh(m_dcMemory.GetSafeHdc());//构造一个图形对象gh
	gh.Clear(Color::White);//清楚背景
	gh.ResetClip();

	//画入内存中
	{
		m_Menu.Draw(gh);//画开始菜单
		m_Background.Draw(gh);//画背景
	}
	//复制到内存中
	::BitBlt(hdc, 0, 0, rc.Width(), rc.Height(), m_dcMemory.GetSafeHdc(), 0, 0, SRCCOPY);
	dc->DeleteDC();//释放
	return;
}

//画出FPS
void CGame::DrawFps(Graphics &gh)
{
	static int fps = 0;//静态变量,每次调用这个函数时都保存上次的值
	m_Fps++;    //帧数自加,记录已经画了多少帧
	static DWORD dwLast = GetTickCount();//记录上次输出的帧率时间,--是为了计算下次的时间差,并计算下次帧率
	if (GetTickCount()-dwLast>=1000)//帧率=帧数/时间(s)--所以这里要大于1000ms
	{
		fps = m_Fps;
		m_Fps = 0;//每秒内的帧数变量需要清0
		dwLast = GetTickCount();//记录下本次输出的时间
	}
	//输出fps
	{
		CString s;
		s.Format(_T("FPS:%d"), fps);//把fps格式化到字符串s
		//SolidBrush brush(Color(0x00, 0x00, 0xFF));//创建一个红色画刷
		SolidBrush brush(Color(rand() % 256, rand() % 256, rand() % 256));//创建一个随机颜色画刷
		Gdiplus::Font font(_T("楷体"), 10.5);//创建输出的字体
		CRect rc;
		GetClientRect(m_hWnd, &rc);//获取输出窗口的大小,用来定位文字的输出位置
		PointF origin(static_cast<float>(rc.right - 50), static_cast<float>(rc.top + 2));//在右上角画帧率
		//前面都是为图片的位置做准备,现在是开始画图了
		gh.DrawString(s.GetString(), -1, &font, origin, &brush);
	}
}
