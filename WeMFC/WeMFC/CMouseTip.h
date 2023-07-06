#pragma once
#include "afxdialogex.h"
#include "CMFECToolTip.h"

// CMouseTip 对话框

class CMouseTip : public CDialog
{
	DECLARE_DYNAMIC(CMouseTip)

public:
	CMouseTip(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CMouseTip();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MOUSE_TIP };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()
public:
	CString m_sName;
	// 提示列表
	CListBox m_ctlList;
protected:
	CMFECToolTip m_toolTip;

	afx_msg void OnAdd();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
};
