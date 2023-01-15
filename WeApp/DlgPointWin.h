
#pragma once

// DlgPointWin.h : header file
//

#include "DIP_SystemDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgPointWin dialog

class CDlgPointWin : public CDialog
{
// Construction
public:
	CDlgPointWin(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	enum { IDD = IDD_DLG_POINTWIN };

// Overrides
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
public:
	CDIP_SystemDoc* m_pDoc; 

	// �����Ҷ�ֵ�ļ���
	float m_fIntensity[256];

protected:
	// ��ǰ����϶�״̬��0��ʾδ�϶���1��ʾ�����϶����ޣ�2��ʾ�����϶����ޡ�
	int		m_iIsDraging;
	
	// ��Ӧ����¼��ľ�������
	CRect	m_MouseRect;

	// ���ڵ�����
	BYTE	m_bLow;
	
	// ���ڵ�����
	BYTE	m_bUp;

protected:

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
};