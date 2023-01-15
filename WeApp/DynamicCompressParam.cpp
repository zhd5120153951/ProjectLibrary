// ************************************************************************
//  �ļ�����DynamicCompressParam.cpp
//
//  ͼ��Ҷȶ�̬ѹ������߶ȱ��������趨����ࣺ
//
// ************************************************************************

#include "stdafx.h"
#include "DIP_System.h"
#include "DynamicCompressParam.h"
#include "GrayProc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDynamicCompressParam dialog

CDynamicCompressParam::CDynamicCompressParam(CWnd* pParent /*=NULL*/) : CDialog(CDynamicCompressParam::IDD, pParent)
{
	m_nCValue = 100;
	m_pDoc = NULL;
}

void CDynamicCompressParam::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SPIN, m_ctrlSpin);
	DDX_Control(pDX, IDC_PARAMSET, m_ctrlSlider);
	DDX_Text(pDX, IDC_CVALUE, m_nCValue);
}

BEGIN_MESSAGE_MAP(CDynamicCompressParam, CDialog)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_PARAMSET, OnReleasedcaptureParamset)
	ON_BN_CLICKED(IDC_SET, OnSet)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN, OnDeltaposSpin)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDynamicCompressParam message handlers

BOOL CDynamicCompressParam::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// ��������ʼ����
	m_ctrlSlider.SetRange(100, 200);
	m_ctrlSlider.SetPos(m_nCValue);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDynamicCompressParam::OnReleasedcaptureParamset(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// ��ȡ��ǰ����
	m_nCValue = m_ctrlSlider.GetPos();
	
	// ���²���
	UpdateData(FALSE);

	// ��ʼ������ΪFALSE
 	m_pDoc->SetModifiedFlag(FALSE);
 
	// ����װ���ļ�
	m_pDoc->m_pDIB->ReLoad();

	// ����WindowTrans()�������лҶȴ��ڱ任
	CGrayProc clsGrayProc(m_pDoc->m_pDIB);
	clsGrayProc.DynamicCompress(m_nCValue);
	
	// ��������
	m_pDoc->SetModifiedFlag(TRUE);
	
	// ������ͼ
	m_pDoc->UpdateAllViews(NULL);

	*pResult = 0;
}

void CDynamicCompressParam::OnSet() 
{
	// ��ȡ����
	UpdateData(TRUE);

	// ���Խ�Ͻ�
	if (m_nCValue > m_ctrlSlider.GetRangeMax() || m_nCValue < m_ctrlSlider.GetRangeMin())
	{
		// �������û�����Χ
		m_ctrlSlider.SetRange(100 * (int)(m_nCValue / 100), 100 * (int)(m_nCValue / 100) + 100);
	}

	// �������û�����
	m_ctrlSlider.SetPos(m_nCValue);

	// ��ʼ������ΪFALSE
 	m_pDoc->SetModifiedFlag(FALSE);
 
	// ����װ���ļ�
	m_pDoc->m_pDIB->ReLoad();

	// ����WindowTrans()�������лҶȴ��ڱ任
	CGrayProc clsGrayProc(m_pDoc->m_pDIB);
	clsGrayProc.DynamicCompress(m_nCValue);
	
	// ��������
	m_pDoc->SetModifiedFlag(TRUE);
	
	// ������ͼ
	m_pDoc->UpdateAllViews(NULL);
}

void CDynamicCompressParam::OnDeltaposSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	
	// ΢������
	m_nCValue -= pNMUpDown->iDelta;

	// ���²�����ʾ
	UpdateData(FALSE);

	// �����趨
	OnSet();

	*pResult = 0;
}

