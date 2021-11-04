
// Tanker.h: Tanker 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // 主符号


// CTankerApp:
// 有关此类的实现，请参阅 Tanker.cpp
//

class CTankerApp : public CWinApp
{
public:
	CTankerApp() noexcept;


// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 实现

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	//引入GDIPlus所需要的变量
	ULONG_PTR m_TokenGdiplus;
	Gdiplus::GdiplusStartupInput m_Input;//调用Gdiplus库中的GdiplusStartupInput类型定义变量
	Gdiplus::GdiplusStartupOutput m_Output;
};

extern CTankerApp theApp;
