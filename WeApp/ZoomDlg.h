#pragma once

#include "afxwin.h"
#include "resource.h"
#include "DIP_SystemDoc.h"
#include "DIP_SystemView.h"

// CZoomDlg �Ի���

class CDIP_SystemDoc;

class CZoomDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CZoomDlg)

public:
	CZoomDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CZoomDlg();

	// �Ի�������
	enum { IDD = IDD_DLG_ZOOM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CComboBox					m_ZoomDlgCombo;
	CEdit						m_EditImageWidth;
	CEdit						m_EditImageHeight;
	CEdit						m_EditImageRatio;
	CComboBox					m_ComboMethod;
	int							m_nComboSelected;
	double						m_dWidthEdit;
	double						m_dHeightEdit;
	double						m_dImageRatio;
	int							m_nMethod;

	int							m_nWidthNew;
	int							m_nHeightNew;
	double						m_dWidthRatio;
	double						m_dHeightRatio;

	BYTE*						m_dataSrc;            //ԭʼ����
	CDIP_SystemDoc*				m_pDoc;               //��ĵ�
	CDIP_SystemView*            m_pView;              //���ͼ
	CImage*						m_pImage;             //��ǰͼ��
	int							m_nChannel;           //ͨ����
	long						m_nImageWidth;        //ͼ����
	long						m_nImageHeight;       //ͼ��߶�
	long						m_nRowLen;            //ÿ���ֽ���
	long						m_nDatalen;           //�ܳ���
	BOOL						m_bModified;          //ͼƬ������

	BOOL ZoomNormal(int widthnew, int heightnew);		//���ٽ���ֵ
	BOOL ZoomInterpolation(int widthnew, int heightnew);//˫���Բ�ֵ
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedZoomDlgApply();
	afx_msg void OnCbnSelchangeZoomDlgCombo();
	afx_msg void OnSelchangeZoomDlgMethod();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};

