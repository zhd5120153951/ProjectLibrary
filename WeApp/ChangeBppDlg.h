#pragma once
#include "afxwin.h"

// CChangeBppDlg �Ի���

class CChangeBppDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CChangeBppDlg)

public:
	CChangeBppDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CChangeBppDlg();

// �Ի�������
	enum { IDD = IDD_DLG_CHANGE_BPP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CStatic		m_StaticBpp;
	CComboBox	m_ComboBpp;
	int			m_nTargetBpp;
	afx_msg void OnSelchangeComboBpp();
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};

