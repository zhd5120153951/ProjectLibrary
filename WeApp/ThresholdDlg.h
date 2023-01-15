#pragma once

#include "PreviewDlg.h"
#include "afxcmn.h"
#include "DIP_SystemDoc.h"
#include "DIP_SystemView.h"

// CThresholdDlg �Ի���

class CThresholdDlg : public CDialog
{
	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(CThresholdDlg)

public:
	CSliderCtrl		m_SliderNum;			// ��ֵ�Ļ������
	int				m_nNum;					// ��ֵ
	int				m_nPos;					// ��������������
	int				m_nWidth;				// ͼ����
	int				m_nHeight;				// ͼ��߶�
	int				m_nChannel;				// ͼ��ͨ��
	LONG			m_nRowLen;				// ͼ���п�
	LONG			m_nLength;				// ͼ�����ֽ�
	BYTE*			m_pSrc;					// ָ��ԭʼ����
	BYTE*			m_pBuf;					// ָ�򻺴�����

	// ��׼�������������
	CThresholdDlg(CWnd* pParent = NULL);
	virtual ~CThresholdDlg();

	void Refresh();						// ˢ��Ԥ������

	CDIP_SystemDoc* GetDocument();		// ��ȡ��ĵ�
	CDIP_SystemView* GetActiveView();	// ��ȡ���ͼ
	virtual BOOL OnInitDialog();		// ��ʼ���Ի���
	afx_msg void OnEnChangeNum();		// ��ֵ���޸�ʱ����Ӧ����
	afx_msg void OnBnClickedSave();		// �����桱��ť��Ӧ���� 
	afx_msg void OnBnClickedOk();		// ��ȷ������ť����Ӧ���� 
	afx_msg void OnBnClickedCancel();	// ��ȡ������ť����Ӧ����
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
										// ��������Ӧ����

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

private:
	CPreviewDlg*	dlg;				// Ԥ���Ի���
	bool			m_bModify;			// ͼ���Ƿ��޸�

	// �Ի�������
	enum { IDD = IDD_DLG_THRESHOLD };
};

