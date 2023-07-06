#pragma once
#include "afxdialogex.h"


// CSimulateKeyInput 对话框

class CSimulateKeyInput : public CDialog
{
	DECLARE_DYNAMIC(CSimulateKeyInput)

public:
	CSimulateKeyInput(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CSimulateKeyInput();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EDIT_INPUT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 输入框
	CEdit m_ctlEdit;
	afx_msg void OnClickedBtnInput();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
