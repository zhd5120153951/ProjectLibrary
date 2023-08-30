#pragma once
#include "afxdialogex.h"


// CBitmapBtn 对话框

class CBitmapBtn : public CDialog
{
	DECLARE_DYNAMIC(CBitmapBtn)

public:
	CBitmapBtn(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CBitmapBtn();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BitmapBtn };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CBitmapButton m_bitmapbutton;
protected:
	CFont myfont;
public:
	afx_msg void OnClickedBtnBmp();
	afx_msg void OnBnClickedCancel();
protected:
	virtual BOOL OnInitDialog();
};
