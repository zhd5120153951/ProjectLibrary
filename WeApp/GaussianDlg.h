#pragma once

#include "afxwin.h"
#include "resource.h"

// CGaussianDlg �Ի���

class CGaussianDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CGaussianDlg)

public:
	CGaussianDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CGaussianDlg();

// �Ի�������
	enum { IDD = IDD_DLG_GAUSSIAN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CEdit			m_EditGaussianMean;
	CEdit			m_EditGaussianStd;
	double			m_dGaussianMean;
	double			m_dGaussianStd;
	CStatic			m_Static1;
	CStatic			m_Static2;
	CString			m_String1;
	CString			m_String2;
};

