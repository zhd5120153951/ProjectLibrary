// CSimulateKeyInput.cpp: 实现文件
//

#include "pch.h"
#include "WeMFC.h"
#include "afxdialogex.h"
#include "CSimulateKeyInput.h"


// CSimulateKeyInput 对话框

IMPLEMENT_DYNAMIC(CSimulateKeyInput, CDialog)

CSimulateKeyInput::CSimulateKeyInput(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_EDIT_INPUT, pParent)
{

}

CSimulateKeyInput::~CSimulateKeyInput()
{
}

void CSimulateKeyInput::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_INPUT, m_ctlEdit);
}


BEGIN_MESSAGE_MAP(CSimulateKeyInput, CDialog)
	ON_BN_CLICKED(ID_BTN_INPUT, &CSimulateKeyInput::OnClickedBtnInput)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CSimulateKeyInput 消息处理程序


void CSimulateKeyInput::OnClickedBtnInput()
{
	// TODO: 在此添加控件通知处理程序代码
	static BOOL bChangeFlag = TRUE;
	if (bChangeFlag)
	{
		SetTimer(1, 500, NULL);//设置定时器
		bChangeFlag = bChangeFlag ? FALSE : TRUE;
		GetDlgItem(ID_BTN_INPUT)->SetWindowText("停止输入");//设置文本
	}
	else
	{
		GetDlgItem(ID_BTN_INPUT)->SetWindowText("模拟输入");
		KillTimer(1);//取消定时器
		bChangeFlag = bChangeFlag ? FALSE : TRUE;
	}
}


void CSimulateKeyInput::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	static int count = 0;
	BYTE keyname[11]{ 72,69,76,76,79,32,87,76,68 };//按键序列的虚拟键码
	if (nIDEvent == 1)
	{
		m_ctlEdit.SetFocus();//编辑框的输入焦点
		keybd_event(keyname[count], 0, 0, 0);//按键按下
		keybd_event(keyname[count], 0, KEYEVENTF_KEYUP, 0);//按键抬起
		count++;
		if (count == 12)
		{
			m_ctlEdit.SetWindowText("");//编辑框
			UpdateData(FALSE);
			count = 0;
		}

	}
	CDialog::OnTimer(nIDEvent);
}
