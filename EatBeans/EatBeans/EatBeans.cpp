// EatBeans.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "EatBeans.h"
#include "GObject.h"
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")

#define MAX_LOADSTRING 100

#define WLENTH 700
#define WHIGHT 740
#define STAGE_COUNT 3


// 全局变量:
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名
//游戏物体
BeanMan* p;//豆子人类型指针
GObject* e1;
GObject* e2;
GObject* e3;
GObject* e4;
//释放动态内存函数模板--模板头--函数体
template<class T>
void Realese(T t)
{
	if (t!=NULL)
	{
		delete t;
	}

}

//玩家胜利后,要么进入下一关,要么游戏结束,此时刷新对象
void ResetGObjects()
{
	p->SetPosition(PLAYROW, PLAYARRAY);
	e1->SetPosition(ENEMYROW, ENEMYARRAY);
	e2->SetPosition(ENEMYROW, ENEMYARRAY);
	e3->SetPosition(ENEMYROW, ENEMYARRAY);
	e4->SetPosition(ENEMYROW, ENEMYARRAY);
}

// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int,HWND&);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
//这是主函数入口
int WINAPI wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此处放置代码。
	int s_n = 0;//正在进行的关卡数
	p = new BeanMan(PLAYROW, PLAYARRAY);        //定义一个豆子人对象
	e1 = new RandEnemy(ENEMYROW, ENEMYARRAY);   //定义一个随机敌人
	e2 = new RandEnemy(ENEMYROW, ENEMYARRAY);   //随机敌人
	e3 = new DefendEnemy(ENEMYROW, ENEMYARRAY); //定义一个防御型敌人
	e4 = new AttackEnemy(ENEMYROW, ENEMYARRAY); //定义一个攻击性敌人
	GMap* MapArray[STAGE_COUNT] = { new stage_1(),new stage_2(),new stage_3() };//地图类类型的指针数组,存放所有的地图
	GObject::pStage = MapArray[s_n];//默认关卡为第一关s_n=0
	Enemy::mPlayer = p;//mPLayer是声明在Enemy类中的,这样的目的是让敌人有锁定的目标--即玩家,还可以设定多个玩家(后面自己升级看看)
	MSG msg; //定义一个消息变量
	HACCEL hAccelTable;

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_EATBEANS, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化:
	HWND hWnd;//必须要先定义一个窗口句柄,为后面调用函数传递参数
    if (!InitInstance (hInstance, nCmdShow,hWnd))
    {
        return FALSE;
    }

    hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_EATBEANS));
	DWORD t = 0;
	mciSendString(TEXT("open beanM2.mp3"), 0, 0, 0);
	mciSendString(TEXT("play beanM2.mp3 repeat"), 0, 0, 0);
    // 主消息循环:窗口的绘制也是在主函数中完成的
	while (p->GetTW()!=OVER&&s_n<3)
	{
		//玩家胜利
		if (p->IsWin())
		{
			HDC hdc = GetDC(hWnd);
			s_n++;//默认直接进入下一关
			ResetGObjects();
			if (s_n<3)
			{
				MessageBoxA(hWnd, "恭喜您通关", "吃豆子提示", MB_OK);
				GObject::pStage = MapArray[s_n];//地图变为下一关地图
				RECT screenRect;
				screenRect.left = 0;
				screenRect.top = 0;
				screenRect.right = WLENTH;
				screenRect.bottom = WHIGHT;//这是屏幕的绘制区域
				::FillRect(hdc, &screenRect, CreateSolidBrush(RGB(255, 255, 255)));
				GObject::pStage->DrawMap(hdc);
			}
			continue;
		}
		//玩家正在游戏中
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))//实时获取窗口的消息
		{
			TranslateMessage(&msg);//翻译窗口消息
			DispatchMessage(&msg);//进入消息队列
		}
		if (GetAsyncKeyState(VK_DOWN) & 0x8000)//在当前窗口按下了向下键(这是异步按键检测)
			p->SetTWCommand(DOWN);
		if (GetAsyncKeyState(VK_LEFT) & 0x8000)
			p->SetTWCommand(LEFT);
		if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
			p->SetTWCommand(RIGHT);
		if (GetAsyncKeyState(VK_UP) & 0x8000)
			p->SetTWCommand(UP);
		else
		{
			if (GetTickCount()-t>58)
			{
				HDC hdc = GetDC(hWnd);//获取窗口句柄
				e1->Action();
				e2->Action();
				e3->Action();
				e4->Action();
				p->Action();
				GObject::pStage->DrawBean(hdc);
				e1->DrawBlank(hdc);
				e2->DrawBlank(hdc);
				e3->DrawBlank(hdc);
				e4->DrawBlank(hdc);
				p->DrawBlank(hdc);
				e1->Draw(hdc);
				e2->Draw(hdc);
				e3->Draw(hdc);
				e4->Draw(hdc);
				p->Draw(hdc);
				DeleteDC(hdc);//创建窗口会用到画笔,用完后一定要释放
				t = GetTickCount();
			}
		}
	}
	Realese(e1);//释放对象
	Realese(e2);
	Realese(e3);
	Realese(e4);
	for (int i=0;i<STAGE_COUNT;i++)
	{
		Realese(MapArray[i]);//地图资源对象也要释放
	}
	if (p->GetTW()==OVER)
	{
		mciSendString(TEXT("close beanM2.mp3"), 0, 0, 0);
		mciSendString(TEXT("open defeat.mp3"), 0, 0, 0);
		mciSendString(TEXT("play defeat.mp3 repeat"), 0, 0, 0);
		//玩家得到结束指令
		MessageBoxA(hWnd, "通关失败", "吃豆子提示", MB_OK);
		

	}
	else
	{
		mciSendString(TEXT("close defeat.mp3"), 0, 0, 0);
		mciSendString(TEXT("open vectory.mp3"), 0, 0, 0);
		mciSendString(TEXT("play vectory.mp3 repeat"), 0, 0, 0);
		MessageBoxA(hWnd, "恭喜您,你获得了胜利","吃豆子提示", MB_OK);//MB_OK是鼠标的左键
	}
	Realese(p);
    return (int) msg.wParam;
}

//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_EATBEANS));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_EATBEANS);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow,HWND& hWnd)//默认的只有两个参数,需要更改
{
   hInst = hInstance; // 将实例句柄存储在全局变量中
   hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, 0, 0, WLENTH, WHIGHT, NULL, NULL, hInstance, NULL);
   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//回调函数==即窗口过程处理函数(一般主窗口和子窗口会共享这个函数)
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmld, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;    //以后可以用到的时候在声明,这样更便捷
    switch (message)
    {
    case WM_COMMAND:         //  WM_COMMAND  - 处理应用程序菜单
        wmld = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
        // 分析菜单选择:
        switch (wmld)
        {
        case IDM_ABOUT:  //处理关于选项
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);//About函数在后面去实现--回调函数
            break;
        case IDM_EXIT:   //处理退出选项
            DestroyWindow(hWnd);
            break;
        default:         //处理默认选项
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    break;
    case WM_PAINT:       //  WM_PAINT    - 绘制主窗口
        hdc = BeginPaint(hWnd, &ps);
        // TODO: 在此处添加使用 hdc 的任何绘图代码...
		GObject::pStage->DrawMap(hdc);
        EndPaint(hWnd, &ps);
        break;
    case WM_DESTROY:    //  WM_DESTROY  - 发送退出消息并返回
        PostQuitMessage(0);
		//::exit(0);
		exit(0);//::有没有都是一个意思,但是加上了其意义更明确
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;
    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
