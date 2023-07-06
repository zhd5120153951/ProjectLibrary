#include "pch.h"
#include "CMFECToolTip.h"

CMFECToolTip::CMFECToolTip()
{
	m_nHeight = 0;
	m_nWidth = 0;
	m_nFontSize = 14;		// original size
	m_currentControlID = 0;
	m_pParentWnd = NULL;

	m_aControlInfo.RemoveAll();//清空所有消息
}

CMFECToolTip::~CMFECToolTip()
{
	ToolTipInfo* pInfo = NULL;
	int nSize = m_aControlInfo.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		pInfo = (ToolTipInfo*)m_aControlInfo.GetAt(i);
		delete pInfo;
	}
	m_aControlInfo.RemoveAll();
}

void CMFECToolTip::Create(CWnd* pWnd)
{
	m_pParentWnd = pWnd;
}

void CMFECToolTip::ErasePreviousToolTipDisplay(UINT nControlID)
{
	ASSERT(m_pParentWnd);
	if ((m_nHeight == 0) || (m_nWidth == 0))
	{
		return;
	}
	CRect oInfoRect(0,0,0,0);
	CalculateInfoBoxRect(nControlID, &oInfoRect);
	m_pParentWnd->InvalidateRect(&oInfoRect);
	m_pParentWnd->UpdateWindow();

	m_nHeight = 0;
	m_nWidth = 0;
}

void CMFECToolTip::ShowToolTip(UINT nControlID)
{
	ToolTipInfo* pToolTip = IsControlIDExisting(nControlID);
	if (!pToolTip)
	{
		return;
	}
	DisplayInfo(pToolTip);
}

void CMFECToolTip::ShowToolTip(CPoint& point)
{
	CWnd* pWnd = m_pParentWnd->ChildWindowFromPoint(point);
	if (pWnd)
	{
		UINT nControlID = (UINT)pWnd->GetDlgCtrlID();//得到某个控件id
		if (m_currentControlID != nControlID)
		{
			ErasePreviousToolTipDisplay(m_currentControlID);//把旧的擦除
			ShowToolTip(nControlID);                        //显示新的提示
		}
	}
}

BOOL CMFECToolTip::AddControlInfo(UINT controlID, CStringArray& straInfo, COLORREF back, COLORREF text)
{
	ToolTipInfo* pToolTip = new ToolTipInfo;
	ASSERT(pToolTip!=NULL);
	int nSize = straInfo.GetSize();
	if (pToolTip <= 0)
	{
		delete pToolTip;
		return FALSE;
	}

	pToolTip->nControlInfo.RemoveAll();
	pToolTip->nInfoSize = nSize;
	pToolTip->nControlID = controlID;
	for (register int i = 0; i < nSize; i++)
	{
		pToolTip->nControlInfo.Add(straInfo[i]);
	}
	//初始化颜色，否则使用默认
	pToolTip->nBackColor = back;
	pToolTip->nTextColor = text;

	//添加到控件提示消息列表
	m_aControlInfo.Add(pToolTip);
	return TRUE;
}

BOOL CMFECToolTip::RemoveControlInfo(UINT nControlID)
{
	ToolTipInfo* pToolTip = NULL;
	int nSize = m_aControlInfo.GetSize();
	for (register int i = 0; i < nSize; i++)
	{
		pToolTip = (ToolTipInfo*)m_aControlInfo.GetAt(i);
		if (pToolTip->nControlID == nControlID)	//找到该控件
		{
			if (m_currentControlID == nControlID)
			{
				ErasePreviousToolTipDisplay(m_currentControlID);
			}
			m_aControlInfo.RemoveAt(i);	//移除该控件
			delete pToolTip;
			break;		
		}
	}
	return TRUE;
}

void CMFECToolTip::SetFontSize(int size)
{
	m_nFontSize = size;
}

void CMFECToolTip::CalculateInfoBoxRect(UINT nControlID, CRect* pInfoRect)
{
	ASSERT(m_pParentWnd);

	CRect oRect(0, 0, 0, 0);
	CRect oParentWindowRect(0, 0, 0, 0);

	m_pParentWnd->GetWindowRect(&oParentWindowRect);
	m_pParentWnd->ScreenToClient(&oParentWindowRect);

	CWnd* pWnd = m_pParentWnd->GetDlgItem(nControlID);
	ASSERT(pWnd);
	pWnd->GetWindowRect(oRect);
	m_pParentWnd->ScreenToClient(&oRect);

	//如果提示框合适--检查
	SetFontSize(14);
	int nButton = oRect.bottom - (oRect.bottom - oRect.top) / 2;
	if (nButton <= oParentWindowRect.bottom)
	{
		pInfoRect->top = oRect.bottom - (oRect.bottom - oRect.top) / 2;
		pInfoRect->bottom = pInfoRect->top + m_nHeight;
	}
	else
	{
		// change the size of the font as well as the info box if all
		// info data cannot be viewed
		if (m_nHeight > (oParentWindowRect.bottom - oParentWindowRect.top))
		{
			SetFontSize(12);
			m_nHeight = 12 + m_nTotalLine * (m_nFontSize - 1);
			m_nWidth = 10 + (int)(7 * m_maxCharInLine);
		}
		// end
		pInfoRect->bottom = oParentWindowRect.bottom - 1;
		pInfoRect->top = pInfoRect->bottom - m_nHeight;
	}

	//check if the box fit in the parent dialog
	int nRight = (oRect.left + oRect.right) / 2 + m_nWidth;
	if (nRight <= oParentWindowRect.right)
	{
		pInfoRect->left = (oRect.left + oRect.right) / 2;
		pInfoRect->right = pInfoRect->left + m_nWidth;
	}
	else
	{
		int nLeft = (oRect.left + oRect.right) / 2 - m_nWidth;
		if (nLeft <= oParentWindowRect.left)
		{
			pInfoRect->left = oParentWindowRect.left + 1;
			pInfoRect->right = pInfoRect->left + m_nWidth;
		}
		else
		{
			pInfoRect->right = (oRect.left + oRect.right) / 2;
			pInfoRect->left = pInfoRect->right - m_nWidth;
		}
	}

	ASSERT(pInfoRect->top <= pInfoRect->bottom);
	ASSERT(pInfoRect->left <= pInfoRect->right);
}

void CMFECToolTip::CalculateHeightAndWidth(CStringArray& straInfos)
{
	ASSERT(m_pParentWnd);

	int nMaxLength = 0;
	int nLength;
	int nLine = straInfos.GetSize();
	m_nTotalLine = nLine; //提示消息的最大行
	for (int i = 0; i < nLine; i++)
	{
		nLength = (straInfos[i].GetLength());
		if (nLength > nMaxLength)
		{
			nMaxLength = nLength;
		}
	}
	m_maxCharInLine = nMaxLength; //最长提示字符
	m_nHeight = 12 + nLine * (m_nFontSize - 1);
	m_nWidth = 10 + (int)(7.5 * nMaxLength);//每个字符的最大像素
}

ToolTipInfo* CMFECToolTip::IsControlIDExisting(UINT controlID)
{
	ToolTipInfo* pToolTip = NULL;
	int nSize = m_aControlInfo.GetSize();
	for (register int i = 0; i < nSize; i++)
	{
		pToolTip = (ToolTipInfo*)m_aControlInfo.GetAt(i);
		if (pToolTip->nControlID == controlID)
		{
			m_currentControlID = controlID;//找到这个控件
			return pToolTip;
		}
	}
	m_currentControlID = 0;//没找到
	return NULL;
}

void CMFECToolTip::DisplayInfo(ToolTipInfo* pToolTip)
{
	if (pToolTip->nInfoSize <= 0)
	{
		return;
	}
	ASSERT(m_pParentWnd);
	CDC* pDC = m_pParentWnd->GetDC();
	CRect oInfoRect;
	CBrush oBrush,*pOldBrush, oBorderBrush;
	int nX, nY;
	TEXTMETRIC TM;
	int nTextHigh;
	CFont oFont, *pOldFont;
	CWnd* pWnd = NULL;

	pDC->SetBkColor(TRANSPARENT);

	oBrush.CreateSolidBrush(pToolTip->nBackColor);

	pOldBrush = pDC->SelectObject(&oBrush);
	pDC->SetTextColor(pToolTip->nTextColor);

	//动态计算提示文字的宽和高
	CalculateHeightAndWidth(pToolTip->nControlInfo);
	CalculateInfoBoxRect(pToolTip->nControlID, &oInfoRect);

	oFont.CreateFont(m_nFontSize, 0, 0, 0, FW_REGULAR, 0, 0, 0, 0, 0, 0, 0, 0, "Courier New");
	pOldFont = pDC->SelectObject(&oFont);

	pDC->FillRect(&oInfoRect, &oBrush);
	pDC->SelectObject(pOldBrush);
	oBrush.DeleteObject();

	oBorderBrush.CreateSolidBrush(pToolTip->nTextColor);
	pOldBrush = pDC->SelectObject(&oBorderBrush);
	DrawEdge(pDC->m_hDC, &oInfoRect, BDR_RAISEDINNER|BDR_SUNKENOUTER,BF_RECT);

	pDC->SetTextAlign(TA_LEFT);
	pDC->GetTextMetrics(&TM);
	nTextHigh = TM.tmHeight + TM.tmExternalLeading - 2;
	nX = oInfoRect.left + 8;
	nY = oInfoRect.top + 5;

	for (register UINT i = 0; i < pToolTip->nInfoSize; i++)
	{
		pDC->TextOut(nX, nY, pToolTip->nControlInfo[i]);
		nY += m_nFontSize - 1;
	}
	pDC->SelectObject(pOldBrush);
	oBorderBrush.DeleteObject();
	m_pParentWnd->ReleaseDC(pDC);
}
