// CMouseTip.cpp: 实现文件
//

#include "pch.h"
#include "WeMFC.h"
#include "afxdialogex.h"
#include "CMouseTip.h"


// CMouseTip 对话框

IMPLEMENT_DYNAMIC(CMouseTip, CDialog)

CMouseTip::CMouseTip(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_MOUSE_TIP, pParent)
{

	m_sName = _T("");
}

CMouseTip::~CMouseTip()
{
}

void CMouseTip::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SHOW, m_ctlList);
	DDX_Text(pDX, IDC_INPUT, m_sName);
}

BOOL CMouseTip::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_MOUSEMOVE)
	{
		POINT pt = pMsg->pt;
		ScreenToClient(&pt);//转换为客户区坐标
		m_toolTip.ShowToolTip((CPoint)pt);//显示提示框
	}
	return CDialog::PreTranslateMessage(pMsg);
}


BEGIN_MESSAGE_MAP(CMouseTip, CDialog)
	ON_BN_CLICKED(IDC_BTN_ADD, &CMouseTip::OnAdd)
	ON_BN_CLICKED(IDC_BTN_EXIT, &CMouseTip::OnExit)
END_MESSAGE_MAP()


// CMouseTip 消息处理程序

void CMouseTip::OnAdd()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	if (!m_sName.IsEmpty())
	{
		m_ctlList.AddString(m_sName);
		m_sName = "";
		UpdateData(false);
	}
}

BOOL CMouseTip::OnInitDialog()
{
	// TODO: 在此处添加实现代码.
	CDialog::OnInitDialog();

	m_toolTip.Create(this);//创建提示对象
	CStringArray straInfo;
	//“添加”按钮提示框
	straInfo.RemoveAll();
	straInfo.Add("向列表框中添加记录");
	m_toolTip.AddControlInfo(IDC_BTN_ADD, straInfo, RGB(220, 174, 208), RGB(0, 0, 162));
	//退出按钮提示框
	straInfo.RemoveAll();
	straInfo.Add("关闭窗口");
	m_toolTip.AddControlInfo(IDC_BTN_EXIT, straInfo, RGB(220, 174, 208), RGB(0, 0, 162));
	//编辑框提示框
	straInfo.RemoveAll();
	straInfo.Add("输入书籍名称");
	m_toolTip.AddControlInfo(IDC_INPUT, straInfo, RGB(220, 174, 208), RGB(0, 0, 162));
	//列表提示框
	straInfo.RemoveAll();
	straInfo.Add("显示添加的图书记录");
	straInfo.Add("实现自动排序");
	m_toolTip.AddControlInfo(IDC_LIST_SHOW, straInfo, RGB(220, 174, 208), RGB(0, 0, 162));

	return TRUE;
}



void CMouseTip::OnExit()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialog::OnCancel();
}
