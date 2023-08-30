
// WeMFC.h: WeMFC 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含 'pch.h' 以生成 PCH"
#endif

#include "resource.h"       // 主符号


// CWeMFCApp:
// 有关此类的实现，请参阅 WeMFC.cpp
//

class CWeMFCApp : public CWinApp
{
public:
	CWeMFCApp() noexcept;


// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 实现
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
		afx_msg void OnSimulateInput();
	afx_msg void OnShowKeycode();
	afx_msg void OnMouseTip();
	afx_msg void OnDoubleClickTitle();
	afx_msg void OnClickClose();
	afx_msg void OnBaseControl();
	afx_msg void OnBmp();
};

extern CWeMFCApp theApp;
