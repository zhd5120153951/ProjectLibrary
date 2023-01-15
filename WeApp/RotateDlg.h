#pragma once

#include "resource.h"
#include "PreviewDlg.h"
#include "DIP_SystemDoc.h"
#include "DIP_SystemView.h"

// CRotateDlg �Ի���

class CRotateDlg : public CDialog
{
	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(CRotateDlg)

public:
	int				m_nAngle;				// ��ת�Ƕ�
	int				m_nPos;					// ��ת�ǶȻ������
	int				m_nType;				// ��תģʽ
	UINT			m_nWidth;				// ͼ����
	UINT			m_nHeight;				// ͼ��߶�
	UINT			m_nChannel;				// ͼ��ͨ��
	LONG			m_nRowLen;				// ͼ���п�
	LONG			m_nLength;				// ͼ�����ֽ�
	BYTE*			m_pBuf;					// ָ�򻺴�����

	CRotateDlg(CWnd* pParent = NULL); 
	virtual ~CRotateDlg();

	void Refresh();
	virtual BOOL OnInitDialog();
	CDIP_SystemDoc* GetDocument();			// ��ȡ��ĵ�
	CDIP_SystemView* GetActiveView();		// ��ȡ���ͼ

	// ��Ϣ��Ӧ����
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedPrev();
	afx_msg void OnEnChangeAngle();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedSave();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

private:
	bool	m_bModify;	// ͼ���Ƿ��޸�

	// �Ի�������
	enum { IDD = IDD_DLG_ROTATE };
public:
	afx_msg void OnBnClickedCancel();
};