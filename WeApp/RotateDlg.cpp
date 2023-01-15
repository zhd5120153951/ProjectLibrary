// RotateDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DIP_System.h"
#include "RotateDlg.h"
#include "GeoOperator.h"
#include "Mainfrm.h"


// CRotateDlg �Ի���
#ifndef PI
#define PI 3.1415926535
#endif

IMPLEMENT_DYNAMIC(CRotateDlg, CDialog)

CRotateDlg::CRotateDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRotateDlg::IDD, pParent)
	, m_nAngle(0)
	, m_nPos(0)
	, m_nType(0)
{
	m_pBuf = NULL;
}

CRotateDlg::~CRotateDlg()
{
	SAFE_DELETE(m_pBuf);
}

void CRotateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ANGLE, m_nAngle);
	DDX_Slider(pDX, IDC_SLIDER, m_nPos);
	DDX_CBIndex(pDX, IDC_MODE, m_nType);
}

BEGIN_MESSAGE_MAP(CRotateDlg, CDialog)
	ON_BN_CLICKED(IDC_PREV, &CRotateDlg::OnBnClickedPrev)
	ON_EN_CHANGE(IDC_ANGLE, &CRotateDlg::OnEnChangeAngle)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_SAVE, &CRotateDlg::OnBnClickedSave)
	ON_BN_CLICKED(IDOK, &CRotateDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CRotateDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CRotateDlg ��Ϣ�������

CDIP_SystemDoc* CRotateDlg::GetDocument()
{
	CMDIFrameWnd* pFrame = (CMDIFrameWnd *)AfxGetApp()->GetMainWnd();
	CMDIChildWnd* pChild = (CMDIChildWnd *)pFrame->GetActiveFrame();
	return (CDIP_SystemDoc *)pChild->GetActiveDocument();
}

CDIP_SystemView* CRotateDlg::GetActiveView()
{
	CMDIFrameWnd* pFrame = (CMDIFrameWnd *)AfxGetApp()->GetMainWnd();
	CMDIChildWnd* pChild = (CMDIChildWnd *)pFrame->GetActiveFrame();
	return (CDIP_SystemView *)pChild->GetActiveView();
}

/******************************************************************************
*	����:		�Ի����ʼ��
******************************************************************************/
BOOL CRotateDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CDIP_SystemDoc* pDoc = GetDocument();

	// ͼ�����ת��Ϊ32λɫ
	pDoc->m_pDIB->PushCopy();
	pDoc->m_pDIB->ChangeBPP(32);

	m_nWidth = pDoc->m_pDIB->Width();
	m_nHeight = pDoc->m_pDIB->Height();
	m_nChannel = pDoc->m_pDIB->Channel();
	m_nRowLen = pDoc->m_pDIB->RowLen();
	m_nLength = pDoc->m_pDIB->Length();
	m_pBuf = new BYTE[m_nLength];
	memcpy(m_pBuf, pDoc->m_pDIB->FindAddress(), m_nLength);

	// �޸ġ���ת�Ƕȡ��ķ�Χ
	((CSliderCtrl*)GetDlgItem(IDC_SLIDER))->SetRange(0, 360);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

/******************************************************************************
*	����:		���Ƕȡ��ı�ʱ����Ӧ����
******************************************************************************/
void CRotateDlg::OnEnChangeAngle()
{
	UpdateData(TRUE);

	if (m_nAngle < 0)
		m_nAngle = 0;
	if (m_nAngle > 360)
		m_nAngle = 360;

	m_nPos = m_nAngle;

	UpdateData(FALSE);
}

/******************************************************************************
*	����:		ˮƽ�������Ӧ����
******************************************************************************/
void CRotateDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	UpdateData(TRUE);
	m_nAngle = m_nPos;
	UpdateData(FALSE);

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

/******************************************************************************
*	����:		���½���ͼ����ת
******************************************************************************/
void CRotateDlg::Refresh()
{
	UpdateData(TRUE);

	// ����Ԥ���Ի���

	BYTE* temp = NULL;
	UINT OutWidth = m_nWidth;
	UINT OutHeight = m_nHeight;

	if (m_nType == 0)
		RotateNormal(m_pBuf, temp, m_nWidth, m_nHeight, OutWidth, OutHeight, m_nAngle / 180.0 * PI);
	else
		RotateInterpolation(m_pBuf, temp, m_nWidth, m_nHeight, OutWidth, OutHeight, m_nAngle / 180.0 * PI);
	
	CDIP_SystemDoc *pDoc = GetDocument();
	pDoc->m_pDIB->SetBits(temp, 4, OutWidth, OutHeight);
	SAFE_DELETE(temp);
	GetDocument()->SetDocSize();
	GetActiveView()->Invalidate();
}

/******************************************************************************
*	����:		��Ԥ��Ч������ť����Ӧ����
******************************************************************************/
void CRotateDlg::OnBnClickedPrev()
{
	Refresh();
}

void CRotateDlg::OnBnClickedSave()
{
	Refresh();
}

void CRotateDlg::OnBnClickedOk()
{
	OnBnClickedSave();
	CDialog::OnOK();
}

void CRotateDlg::OnBnClickedCancel()
{
	// ��ԭͼ�񵯳���ջ
	GetDocument()->m_pDIB->Undo();
	GetDocument()->SetDocSize();
	GetActiveView()->Invalidate();
	CDialog::OnCancel();
}