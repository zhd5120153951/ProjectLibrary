
// Tanker.cpp: 定义应用程序的类行为。
//

#include "pch.h"
#include "Tanker.h"
#include "MainFrm.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTankerApp

BEGIN_MESSAGE_MAP(CTankerApp, CWinApp)  //此类中包含的函数图
	ON_COMMAND(ID_APP_ABOUT, &CTankerApp::OnAppAbout)
END_MESSAGE_MAP()


// CTankerApp 构造

CTankerApp::CTankerApp() noexcept
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
#ifdef _MANAGED
	// 如果应用程序是利用公共语言运行时支持(/clr)构建的，则: 
	//     1) 必须有此附加设置，“重新启动管理器”支持才能正常工作。
	//     2) 在您的项目中，您必须按照生成顺序向 System.Windows.Forms 添加引用。
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: 将以下应用程序 ID 字符串替换为唯一的 ID 字符串；建议的字符串格式
	//为 CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("Tanker.AppID.NoVersion"));

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}

// 唯一的 CTankerApp 对象

CTankerApp theApp;

// CTankerApp 初始化,这里CTankerApp代表着整个游戏App,而Gdilib也需要初始化,而且是程序启动后,窗口显示前

BOOL CTankerApp::InitInstance()
{
	//在这里创建新的窗口对象,并将其设置为应用程序的主窗口对象
	CMainFrame* pFrame = new CMainFrame;
	if (!pFrame)return FALSE;
	m_pMainWnd = pFrame;
	pFrame->ShowWindow(SW_SHOW);//显示窗口
	pFrame->UpdateWindow();     //更新窗口
	Status s = GdiplusStartup(&m_TokenGdiplus, &m_Input, &m_Output);
	return TRUE;
}

int CTankerApp::ExitInstance()
{
	//TODO: 处理可能已添加的附加资源
	AfxOleTerm(FALSE);

	return CWinApp::ExitInstance();
}

// CTankerApp 消息处理程序


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg() noexcept;

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() noexcept : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// 用于运行对话框的应用程序命令
void CTankerApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CTankerApp 消息处理程序



