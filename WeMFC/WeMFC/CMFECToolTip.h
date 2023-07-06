#pragma once
#include <afxwin.h>
#define IVORY RGB(255,255,220)
#define BLACK RGB(0,0,0)

//提示信息结构体
struct ToolTipInfo : public CObject
{
	UINT nControlID;//控件ID
	UINT nInfoSize; //消息行数
	CStringArray nControlInfo;//消息容器
	COLORREF nTextColor;//文字颜色
	COLORREF nBackColor;//背景颜色
};

class CMFECToolTip : public CWnd
{
public:
	CMFECToolTip();
	virtual ~CMFECToolTip();

	void Create(CWnd* pWnd);
	void ErasePreviousToolTipDisplay(UINT nControlID);
	void ShowToolTip(UINT nControlID);

	void ShowToolTip(CPoint& point);
	BOOL AddControlInfo(UINT controlID, CStringArray& straInfo, COLORREF back = IVORY,COLORREF text = BLACK);
	BOOL RemoveControlInfo(UINT nControlID);
	void SetFontSize(int size);
protected:
	CWnd* m_pParentWnd;
	int m_nHeight;
	int m_nWidth;
	int m_nFontSize;

	int m_nTotalLine;
	int m_maxCharInLine;

	virtual void CalculateInfoBoxRect(UINT nControlID, CRect* pInfoRect);
	virtual void CalculateHeightAndWidth(CStringArray& straInfos);

private:
	ToolTipInfo* IsControlIDExisting(UINT controlID);
	void DisplayInfo(ToolTipInfo* pToolTip);
private:
	CObArray m_aControlInfo;
	UINT m_currentControlID;
};

