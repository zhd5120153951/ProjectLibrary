
// MainFrm.cpp: CMainFrame 类的实现
//

#include "pch.h"
#include "MainFrm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define GAME_WIN_W 800
#define GAME_WIN_H 600
//CMainFrame
IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd);
BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)  //此类中包含的功能函数图---这些函数是CMainFrm类中的各种消息函数(存放在消息映射表)
	ON_WM_CREATE()//创建一个窗口消息
	ON_WM_TIMER()//计时器消息

	ON_WM_MOUSEMOVE()//鼠标移动消息
	ON_WM_LBUTTONUP()//左键抬起消息
END_MESSAGE_MAP()
//CMainFrame窗口类的构造函数
CMainFrame::CMainFrame()
{
#define  MY_STYLE (WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX|FWS_ADDTOTITLE)
	//开始创建窗口
	Create(NULL, TEXT("tanker written by daito"), MY_STYLE, CRect(0, 0, GAME_WIN_W, GAME_WIN_H));
	//设置窗口的大小
	{
		CRect rcClient;
		GetClientRect(rcClient);
		RECT rcFrame = { 0,0,m_Width + m_Width - rcClient.right,m_Height + m_Height - rcClient.bottom };//计算窗口的大小
		MoveWindow(&rcFrame, TRUE);
	
	}
}
//CMainFrame主窗口的析构函数
CMainFrame::~CMainFrame()
{

}
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)return -1;//窗口创建失败,返回-1
	SetTimer(ETimeIdGameLoop, 0, NULL);//启动定时器每次都会进入游戏帧中
	m_Game.SetHandle(GetSafeHwnd());//通过获取窗口句柄函数,并传给对象的设置窗口句柄函数,达到设置游戏窗口
	return 0;//创建成功返回0
}
//OnTimer()函数用来判断上次定时和本次定时的时间差,防止时间差过小,相关的执行函数处理太快,影响游戏速度
void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case ETimeIdGameLoop:
		static DWORD dwLastUpdate = GetTickCount();//记录本次时刻
		if(GetTickCount()-dwLastUpdate>=20)
		{
			m_Game.EnterFrame(GetTickCount());//符合时间差,进入游戏帧处理
			dwLastUpdate = GetTickCount();//记录时间间隔
		 }
		//否则什么都不做
	default:
		break;
	}
	CFrameWnd::OnTimer(nIDEvent);
}

void CMainFrame::OnMouseMove(UINT nFlags, CPoint point)
{
	m_Game.OnMouseMove(nFlags, point);//直接把鼠标消息传给CGame对象
	CFrameWnd::OnMouseMove(nFlags, point);
}

void CMainFrame::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_Game.OnLButtonUp(nFlags, point);
	CFrameWnd::OnLButtonUp(nFlags, point);
}
