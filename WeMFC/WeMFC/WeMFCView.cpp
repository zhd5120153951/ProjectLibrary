
// WeMFCView.cpp: CWeMFCView 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "WeMFC.h"
#endif

#include "WeMFCView.h"
#include "MainFrm.h"
#include "CCursorHot.h"
#include "CMouseTip.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWeMFCView

IMPLEMENT_DYNCREATE(CWeMFCView, CView)

BEGIN_MESSAGE_MAP(CWeMFCView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_CHAR()
	ON_WM_SETFOCUS()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDBLCLK()
	ON_COMMAND(ID_CURSORHOT, &CWeMFCView::OnCursorhot)
	ON_COMMAND(ID_MOUSE_TIP, &CWeMFCView::OnMouseTip)
END_MESSAGE_MAP()

// CWeMFCView 构造/析构

CWeMFCView::CWeMFCView() noexcept
{
	// TODO: 在此处添加构造代码
	bShiftdown = bShiftup = bshiftB = FALSE;
	ptCharacter.x = ptCharacter.y = 0;

	bDrag = FALSE;
	ptDown.x = ptDown.y = 0;
	ptUp.x = ptUp.y = 0;
}
CWeMFCView::~CWeMFCView()
{

}

BOOL CWeMFCView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CWeMFCView 绘图

void CWeMFCView::OnDraw(CDC* pDC)
{
	CWeMFCDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	pDC->TextOut(100, 4, "按下鼠标左键，移动鼠标即可绘制椭圆，直到松开左键。");
	pDC->TextOut(100, 54, "双击左键和右键查看光标形状。");
	// TODO: 在此处为本机数据添加绘制代码
	if (bShiftdown)
	{
		pDC->TextOut(100, 100, "按下了shift");
	}
	if (bShiftup)
	{
		pDC->TextOut(100, 200, "松开了shift");
	}
	if (bshiftB)
	{
		pDC->TextOut(100, 300, "按下了B和Shift");
		bshiftB = FALSE;
	}
}


// CWeMFCView 打印

BOOL CWeMFCView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CWeMFCView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CWeMFCView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CWeMFCView 诊断

#ifdef _DEBUG
void CWeMFCView::AssertValid() const
{
	CView::AssertValid();
}

void CWeMFCView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CWeMFCDoc* CWeMFCView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWeMFCDoc)));
	return (CWeMFCDoc*)m_pDocument;
}
#endif //_DEBUG


// CWeMFCView 消息处理程序

//响应按键按下
void CWeMFCView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nChar == VK_SHIFT)
	{
		bShiftdown = TRUE;
		bShiftup = FALSE;
		Invalidate(TRUE);
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
	
}

//响应按键松开
void CWeMFCView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nChar == VK_SHIFT)
	{
		bShiftup = TRUE;
		Invalidate(TRUE);
		bShiftdown = FALSE;
	}
	CView::OnKeyUp(nChar, nRepCnt, nFlags);
}

//响应按键输入字符
void CWeMFCView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if ((nChar == 98) || (nChar == 66))
	{
		if (bShiftdown)
		{
			bshiftB = TRUE;
			bShiftdown = FALSE;
			Invalidate(TRUE);
		}
	}
	//输入字符
	if (nChar == 13)
	{
		//换行
		ptCharacter.x = 0;
		ptCharacter.y += 25;

		SetCaretPos(ptCharacter);
		ShowCaret();
	}
	else
	{
		CClientDC dc(this);
		HideCaret();
		dc.TextOut(ptCharacter.x, ptCharacter.y, (LPCTSTR)&nChar);
		CSize textSize;
		textSize = dc.GetTextExtent((LPCTSTR)&nChar);
		//前进到下一个字符位置
		ptCharacter.x = ptCharacter.x + textSize.cx;
		SetCaretPos(ptCharacter);
		ShowCaret();
	}

	//
	CView::OnChar(nChar, nRepCnt, nFlags);
}


void CWeMFCView::OnSetFocus(CWnd* pOldWnd)
{
	CView::OnSetFocus(pOldWnd);

	// TODO: 在此处添加消息处理程序代码
	CreateSolidCaret(3, 18);		//创建插入符
	SetCaretPos(ptCharacter);		//将插入符移到当前字符输入点
	ShowCaret();
}


// 响应鼠标按键--画圆
void CWeMFCView::DrawCircle()
{
	// TODO: 在此处添加实现代码.
	Invalidate(false);
	CClientDC dc(this);//获取某个窗口的句柄
	CRect rc;
	GetClientRect(rc);//获取客户区域
	CBrush brush(RGB(255, 255, 255));//定义画刷颜色
	dc.FillRect(rc, &brush);
	dc.Ellipse(ptDown.x, ptDown.y, ptUp.x, ptUp.y);//画椭圆---左上和右下，对应按下和抬起的x,y


}


void CWeMFCView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	bDrag = TRUE;//已经按下左键
	ptUp = ptDown = point;//记录鼠标位置
	CView::OnLButtonDown(nFlags, point);
}

//抬起时才画圆，按下只是记录point
void CWeMFCView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (bDrag)
	{
		ptUp = point;
		DrawCircle();
		bDrag = FALSE;
	}
	CView::OnLButtonUp(nFlags, point);
}


void CWeMFCView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (bDrag)
	{
		ptUp = point;//按下的点不变，移动后松开的点变
		DrawCircle();
	}
	CView::OnMouseMove(nFlags, point);
}

//双击左键
void CWeMFCView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CMainFrame* pMainframe;
	pMainframe = (CMainFrame*)AfxGetMainWnd();
	SetCursor(pMainframe->m_cursor); //设为自绘制光标
	CView::OnLButtonDblClk(nFlags, point);
}

//双击右键
void CWeMFCView::OnRButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CMainFrame* pMainframe;
	pMainframe = (CMainFrame*)AfxGetMainWnd();
	SetCursor(pMainframe->m_cursor);//设置光标为自绘光标
	CView::OnRButtonDblClk(nFlags, point);
}


void CWeMFCView::OnCursorhot()
{
	// TODO: 在此添加命令处理程序代码
	CCursorHot ch;
	ch.DoModal();
}


void CWeMFCView::OnMouseTip()
{
	// TODO: 在此添加命令处理程序代码
	CMouseTip mt;
	mt.DoModal();
}
