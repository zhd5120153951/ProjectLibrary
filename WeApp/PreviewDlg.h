#pragma once

// CPreViewDlg �Ի���

#include "resource.h"

class CPreviewDlg : public CDialog
{
	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(CPreviewDlg)

public:
	int					m_nType;
	bool				m_bEnableClose;
	CRect				m_PaintRect;   //��ͼ��
	CPoint              m_ptOrigin;	   //��ͼ���
	BOOL                m_bScroll;  //���ڹ�����ͼ
	int					m_nPicWidth;
	int					m_nPicHeight;
	LONG				m_nPicLength;

	CPreviewDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPreviewDlg();

	virtual BOOL OnInitDialog();
	
	void Refresh()
	{
		Invalidate();
	}

	afx_msg void OnPaint();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnDestroy();

	// �Ի�������
	enum { IDD = IDD_DLG_PREVIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
