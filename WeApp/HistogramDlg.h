#pragma once
#include "resource.h"

// CCLAHEDLg �Ի���

class CHistogramWnd;

class CHistogramDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CHistogramDlg)

public:
	CHistogramDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CHistogramDlg();

// �Ի�������
	enum { IDD = IDD_DLG_HISTOGRAM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public://������Ա�뺯��
	CHistogramWnd*              m_pHistWnd;  //Ψһ�ĳ�Ա����
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
};
