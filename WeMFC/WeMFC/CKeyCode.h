#pragma once
#include "afxdialogex.h"


// CKeyCode 对话框

class CKeyCode : public CDialog
{
	DECLARE_DYNAMIC(CKeyCode)

public:
	CKeyCode(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CKeyCode();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SHOW_KEYCODE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 显示按键码
	CStatic m_ctlframe;
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
