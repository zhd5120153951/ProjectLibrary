#pragma once
#include "resource.h"

// CSpaceFilterDlg �Ի���

class CSpaceFilterWnd;

class CSpaceFilterDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSpaceFilterDlg)

public:
	CSpaceFilterDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSpaceFilterDlg();

// �Ի�������
	enum { IDD = IDD_DLG_SPACEFILTER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public://������Ա�뺯��
	CSpaceFilterWnd*      m_pSpaceFilterWnd;//2200
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
};