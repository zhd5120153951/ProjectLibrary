
// WeMFCView.h: CWeMFCView 类的接口
//

#pragma once
#include "WeMFCDoc.h"

class CWeMFCView : public CView
{
protected: // 仅从序列化创建
	CWeMFCView() noexcept;
	DECLARE_DYNCREATE(CWeMFCView)

// 特性
public:
	CWeMFCDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CWeMFCView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
protected:
	BOOL bShiftdown;
	BOOL bShiftup;
	BOOL bshiftB;
	CRect oldRC;
public:
	CPoint ptCharacter;
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	// 响应鼠标按键--画圆
	void DrawCircle();
	//是否拉动鼠标
	BOOL bDrag;
	//按键按下位置
	CPoint ptDown;
	//按键抬起位置
	CPoint ptUp;
	int m_radio;

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnCursorhot();
	afx_msg void OnMenuItemCommandRange(UINT nID);
	afx_msg void OnUpdateMenuItemCommandRange(CCmdUI *pCCmdUI);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
};

#ifndef _DEBUG  // WeMFCView.cpp 中的调试版本
inline CWeMFCDoc* CWeMFCView::GetDocument() const
   { return reinterpret_cast<CWeMFCDoc*>(m_pDocument); }
#endif

