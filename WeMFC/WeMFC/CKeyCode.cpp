// CKeyCode.cpp: 实现文件
//

#include "pch.h"
#include "WeMFC.h"
#include "afxdialogex.h"
#include "CKeyCode.h"


// CKeyCode 对话框

IMPLEMENT_DYNAMIC(CKeyCode, CDialog)

CKeyCode::CKeyCode(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_SHOW_KEYCODE, pParent)
{

}

CKeyCode::~CKeyCode()
{
}

void CKeyCode::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SHOW_KEYCODE, m_ctlframe);
}


BEGIN_MESSAGE_MAP(CKeyCode, CDialog)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()


// CKeyCode 消息处理程序


void CKeyCode::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	AfxMessageBox("hello");
	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}


BOOL CKeyCode::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此处添加实现代码.
	if (pMsg->message == WM_KEYDOWN)//按下任意按键
	{
		CString strwParam;
		strwParam.Format("用户按下按键的虚拟按键码为：%d ", pMsg->wParam);
		CDC* pDC = m_ctlframe.GetDC();//获得显示狂的DC(绘图设备)
		pDC->TextOut(2, 2, strwParam);//显示文本
		ReleaseDC(pDC);
	}
	return CDialog::PreTranslateMessage(pMsg);
}
