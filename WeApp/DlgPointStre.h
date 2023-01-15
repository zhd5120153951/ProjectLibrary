
#pragma once

// DlgPointStre.h : header file
//

#include "DIP_SystemDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgPointStre dialog

class CDlgPointStre : public CDialog
{
// Construction
public:
	CDlgPointStre(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	enum { IDD = IDD_DLG_POINTSTRE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
public:
	CDIP_SystemDoc* m_pDoc;		// �ĵ�ָ��

	// �����Ҷ�ֵ�ļ���
	float m_fIntensity[256];

protected:
	// ��ǰ����϶�״̬��0��ʾδ�϶���1��ʾ�����϶���һ�㣬2��ʾ�����϶��ڶ��㡣
	int		m_iIsDraging;

	// ��Ӧ����¼��ľ�������
	CRect	m_MouseRect;
	
	// ����ת�۵�����
	BYTE	m_bX1;
	BYTE	m_bY1;
	BYTE	m_bX2;
	BYTE	m_bY2;

protected:

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
};
