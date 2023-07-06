#pragma once
#include "afxdialogex.h"


// CCursorHot 对话框

class CCursorHot : public CDialog
{
	DECLARE_DYNAMIC(CCursorHot)

public:
	CCursorHot(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CCursorHot();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CURSORHOT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
};
