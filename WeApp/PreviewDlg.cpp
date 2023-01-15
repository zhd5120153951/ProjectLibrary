// PreviewDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PreviewDlg.h"
#include "MainFrm.h"
#include "DIP_SystemDoc.h"
#include "DIP_SystemView.h"

SCROLLINFO scroll;

static BOOL scrollSign;

static int yCurrentScroll;//��ֱ�����ϣ��������ĵ�ǰλ��
static int yScrollMax;//��ֱ�����ϣ��������������ϱ߽�
static int yScrollMin;//��ֱ�����ϣ��������������±߽�

static int xCurrentScroll;//ˮƽ�����ϣ��������ĵ�ǰλ��
static int xScrollMax;//ˮƽ�����ϣ��������������ұ߽�
static int xScrollMin;//ˮƽ�����ϣ���������������߽�

// CPreViewDlg �Ի���

IMPLEMENT_DYNAMIC(CPreviewDlg, CDialog)

CPreviewDlg::CPreviewDlg(CWnd* pParent /*=NULL*/) : CDialog(CPreviewDlg::IDD, pParent)
{
	m_nType = 0;
	m_bEnableClose = true;
	m_ptOrigin = CPoint();
	m_PaintRect = CRect();
	m_bScroll = false;
}

CPreviewDlg::~CPreviewDlg()
{
}

void CPreviewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPreviewDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CPreviewDlg ��Ϣ�������
//�Ի���ĳ�ʼ������
BOOL CPreviewDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CMDIFrameWnd* pFrame = (CMDIFrameWnd *)AfxGetApp()->GetMainWnd();
	CMDIChildWnd* pChild = (CMDIChildWnd *)pFrame->GetActiveFrame();
	CDIP_SystemDoc* pDoc = (CDIP_SystemDoc *)pChild->GetActiveDocument();

	m_nPicWidth = pDoc->m_pDIB->Width();
	m_nPicHeight = pDoc->m_pDIB->Height();
	m_nPicLength = pDoc->m_pDIB->Length();

	// ���ڴ�С
	int nDlgWidth = min(m_nPicWidth, GetSystemMetrics(SM_CXSCREEN) - 100);
	int nDlgHeight = min(m_nPicHeight, GetSystemMetrics(SM_CYSCREEN) - 100);
	::SetWindowPos(this->m_hWnd, HWND_TOP, 0, 0, nDlgWidth, nDlgHeight, SWP_SHOWWINDOW);

	GetClientRect(&m_PaintRect);
	return TRUE; 
}
void CPreviewDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CMDIFrameWnd* pFrame = (CMDIFrameWnd *)AfxGetApp()->GetMainWnd();
	CMDIChildWnd* pChild = (CMDIChildWnd *)pFrame->GetActiveFrame();
	CDIP_SystemDoc* pDoc = (CDIP_SystemDoc *)pChild->GetActiveDocument();

	if (m_nPicWidth-m_PaintRect.Width() <= 0 || m_nPicHeight-m_PaintRect.Height() <= 0)
		pDoc->DIBImage()->Draw(dc.GetSafeHdc(), CRect(0, 0, m_nPicWidth, m_nPicHeight));
	else 
		pDoc->DIBImage()->Draw(dc.GetSafeHdc(), CRect(0, 0, m_nPicWidth, m_nPicHeight), 
		CRect(m_ptOrigin.x, m_ptOrigin.y, m_ptOrigin.x + m_nPicWidth, m_ptOrigin.y + m_nPicHeight));
}

/******************************************************************************   
*ˮƽ������������
******************************************************************************/
void CPreviewDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	int errX = m_nPicWidth-m_PaintRect.Width();
	if (errX <= 0) return;

	m_bScroll = TRUE;
	int pos,min,max,thumbwidth;

	SCROLLINFO vinfo;
	GetScrollInfo(SB_HORZ,&vinfo);

	pos = vinfo.nPos;
	min = vinfo.nMin;
	max = vinfo.nMax;
	thumbwidth = vinfo.nPage;

	switch (nSBCode)
	{
	case SB_THUMBTRACK: //�϶�������
		ScrollWindow(-((int)nPos-pos),0);
		SetScrollPos(SB_HORZ,nPos);
		break;
	case SB_LINELEFT : //�������ͷ
		if (pos != 0)
		{
			ScrollWindow(1,0);
			SetScrollPos(SB_HORZ,pos-1);
		}
		break;
	case SB_LINERIGHT: //�����Ҽ�ͷ
		if (pos+thumbwidth <= max)
		{
			SetScrollPos(SB_HORZ,pos+1);
			ScrollWindow(-1,0); 
		}
		break;
	case SB_PAGELEFT: //�ڹ�������󷽿հ׹������򵥻�
		if (pos >= thumbwidth)
		{
			ScrollWindow(thumbwidth,0);
			SetScrollPos(SB_HORZ,pos-thumbwidth);
		}
		else
		{
			ScrollWindow(pos,0);
			SetScrollPos(SB_HORZ,0);
		}
		break;
	case SB_PAGERIGHT: //�ڹ�������ҷ��հ׹������򵥻�
		if (pos+thumbwidth <= max-thumbwidth)
		{
			ScrollWindow(-thumbwidth,0);
			SetScrollPos(SB_HORZ,pos+thumbwidth);
		}
		else
		{
			ScrollWindow(-(max-(pos+thumbwidth)),0);
			SetScrollPos(SB_HORZ,max-thumbwidth);
		}
		break;
	}
	m_ptOrigin.x = m_PaintRect.left + errX*pos/100.0;
	InvalidateRect(m_PaintRect);
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);}

/******************************************************************************   
*��ֱ��������������ʵ�ַ���ͬˮƽ������������
******************************************************************************/
void CPreviewDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	int errY = m_nPicHeight-m_PaintRect.Height();
	if (errY <= 0) return;

	m_bScroll = TRUE;
	int pos,min,max,thumbwidth;

	SCROLLINFO vinfo;
	GetScrollInfo(SB_VERT,&vinfo);

	pos = vinfo.nPos;
	min = vinfo.nMin;
	max = vinfo.nMax;
	thumbwidth = vinfo.nPage;

	switch (nSBCode)
	{
	case SB_THUMBTRACK:
		ScrollWindow(0,-((int)nPos-pos));
		SetScrollPos(SB_VERT,nPos); 
		break;
	case SB_LINELEFT:
		SetScrollPos(SB_VERT,pos-1);
		if (pos != 0)
			ScrollWindow(0,1);
		break;
	case SB_LINERIGHT:
		SetScrollPos(SB_VERT,pos+1);
		if (pos+thumbwidth < max)
			ScrollWindow(0,-1); 
		break;
	case SB_PAGELEFT: //�ڹ�������Ϸ��հ׹������򵥻�
		if (pos >= thumbwidth)
		{
			ScrollWindow(0,thumbwidth);
			SetScrollPos(SB_VERT,pos-thumbwidth);
		}
		else
		{
			ScrollWindow(0,pos);
			SetScrollPos(SB_VERT,0);
		}
		break;
	case SB_PAGERIGHT: //�ڹ�������·��հ׹������򵥻�
		if (pos+thumbwidth <= max-thumbwidth)
		{
			ScrollWindow(0,-thumbwidth);
			SetScrollPos(SB_VERT,pos+thumbwidth);
		}
		else
		{
			ScrollWindow(0,-(max-(pos+thumbwidth)));
			SetScrollPos(SB_VERT,max-thumbwidth);
		}
		break;
	}
	m_ptOrigin.y = m_PaintRect.top + errY*pos/100.0;
	InvalidateRect(m_PaintRect);
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CPreviewDlg::OnDestroy() 
{
	CDialog::OnDestroy();

	if (m_nType == 0)
		delete this;
}

BOOL CPreviewDlg::OnEraseBkgnd(CDC* pDC)
{
	if (m_bScroll == TRUE)
	{
		m_bScroll = FALSE;
		return TRUE;
	}
	return CDialog::OnEraseBkgnd(pDC);
}
