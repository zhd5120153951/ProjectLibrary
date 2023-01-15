// GaussianDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GaussianDlg.h"
#include "afxdialogex.h"

// CGaussianDlg �Ի���

IMPLEMENT_DYNAMIC(CGaussianDlg, CDialogEx)

CGaussianDlg::CGaussianDlg(CWnd* pParent /*=NULL*/) : CDialogEx(CGaussianDlg::IDD, pParent)
{
	m_dGaussianMean = 0;
	m_dGaussianStd = 0;
	m_String1 = L"��ֵ:";
	m_String2 = L"����:";
}

CGaussianDlg::~CGaussianDlg()
{
}

void CGaussianDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GAUSSIAN_MEAN, m_EditGaussianMean);
	DDX_Control(pDX, IDC_GAUSSIAN_STD, m_EditGaussianStd);
	DDX_Text(pDX, IDC_GAUSSIAN_MEAN, m_dGaussianMean);
	DDX_Text(pDX, IDC_GAUSSIAN_STD, m_dGaussianStd);
	DDX_Control(pDX, IDC_ITEM1, m_Static1);
	DDX_Control(pDX, IDC_ITEM2, m_Static2);
	DDX_Text(pDX, IDC_ITEM1, m_String1);
	DDX_Text(pDX, IDC_ITEM2, m_String2);
}

BEGIN_MESSAGE_MAP(CGaussianDlg, CDialogEx)
END_MESSAGE_MAP()

// CGaussianDlg ��Ϣ�������

