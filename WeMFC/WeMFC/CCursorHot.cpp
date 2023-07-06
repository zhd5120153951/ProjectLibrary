// CCursorHot.cpp: 实现文件
//

#include "pch.h"
#include "WeMFC.h"
#include "afxdialogex.h"
#include "CCursorHot.h"


// CCursorHot 对话框

IMPLEMENT_DYNAMIC(CCursorHot, CDialog)

CCursorHot::CCursorHot(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_CURSORHOT, pParent)
{

}

CCursorHot::~CCursorHot()
{
}

void CCursorHot::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCursorHot, CDialog)
	ON_WM_SETCURSOR()
END_MESSAGE_MAP()


// CCursorHot 消息处理程序


BOOL CCursorHot::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (pWnd->GetDlgCtrlID())//得到鼠标所在位置的控价ID
	{
	case IDC_BTN_HOT://鼠标位于按钮空间区域
	{
		SetCursor(AfxGetApp()->LoadCursor(IDC_MYHAND));
		//提示文字可见
		GetDlgItem(IDC_STATIC)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC)->SetWindowText("光标位置在按钮处");
		return TRUE;
	}
	break;
	case IDC_EDIT1://光标在编辑框处
	{
		SetCursor(AfxGetApp()->LoadCursor(IDC_MYHAND));
		//提示文字可见
		GetDlgItem(IDC_STATIC)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC)->SetWindowText("光标位置在编辑框处");
		return TRUE;
	}
	break;
	case IDC_MONTHCALENDAR1:// 鼠标位于日历控件区域内
	{
		// 设置鼠标指针为"手"形指针
		SetCursor(AfxGetApp()->LoadCursor(IDC_MYHAND));
		// 将提示文字可见
		GetDlgItem(IDC_STATIC)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC)->SetWindowText("光标所在位置为日历控件!");
		return TRUE;
	}
	break;
	default:// 鼠标不在或者离开了"热点"
	{
		// 将提示性文字隐藏
		GetDlgItem(IDC_STATIC)->ShowWindow(SW_HIDE);
		// 将鼠标指针设置为标准的鼠标指针
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
		return TRUE;
	}
	}
	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}
