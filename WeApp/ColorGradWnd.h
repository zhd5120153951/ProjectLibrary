#pragma once

#include "DIP_SystemDoc.h"
#include "DIP_SystemView.h"
#include "resource.h"
#include <vector>
using namespace std;

// CColorGradWnd

class CColorGradWnd : public CWnd
{
	DECLARE_DYNAMIC(CColorGradWnd)

public:
	CColorGradWnd();
	virtual ~CColorGradWnd();

protected:
	DECLARE_MESSAGE_MAP()

public://������Ա�뺯��
	CRect*                   m_pPaintRect;  //��ͼ��
	CRect*                  m_pClientRect;  //�ͻ���

	//��ť
	CMFCButton*                   m_pBnOK;  //ȷ��
	CMFCButton*               m_pBnCancel;  //ȡ��
	CButton*                m_pBnGroupBox;  //GroupBox
	CStatic*               m_pStaticInput;  //����ɫ��
	CComboBox*             m_pComboBoxRGB;  //RGB��Ͽ�
	CEdit*                 m_pEditLeftTop;  //���Ͻǵı༭��
	CEdit*                     m_pEditTop;  //���Ϸ��ı༭��
	CEdit*                m_pEditRightTop;  //���Ͻǵı༭��
	CEdit*              m_pEditLeftBottom;  //���½ǵı༭��
	CEdit*             m_pEditRightBottom;  //���½ǵı༭��
	CStatic*              m_pStaticOutput;  //���ɫ��

	//���Ƶ�
	CPoint                       m_ptDark;  //���Ϸ�
	CPoint                     m_ptMedian;  //�м�
	CPoint                     m_ptBright;  //���Ϸ�
	CPoint                        m_ptMin;  //���·�
	CPoint                        m_ptMax;  //���·�
	int                        m_nCurDrag;  //��ǰק���ĵ�
	int                       m_nLastDrag;  //�ϴ�ק���ĵ�
	double                    m_dRatio[5];  //���Ƶ���Ի�ͼ����ȵı���
	double                    m_dValue[5];  //CEdit��ʾ��ֵ
	double                      m_dMedian;  //�м���ʾ������������

	//����
	double*              m_pfHistogram[4];    //ֱ��ͼ:4*256
	int*                 m_V4Transform[4];    //�Ҷȱ任:4*m_nWidth
	BYTE*                       m_dataSrc;    //ͼ��Դ����
	BYTE*                       m_dataCur;    //ͼ��ǰ����

	//ͼ����Ϣ
	int                     m_nImageWidth;  //ͼ����
	int                    m_nImageHeight;  //ͼ��߶�
	int                        m_nChannel;  //ͼ��ͨ��
	LONG                        m_nRowlen;  //ÿ���ֽ�
	LONG                       m_nlenData;  //�ܵĳ���

	int                m_nChannelSelected;    //��ǰͨ��
	int                      m_nRectWidth;    //��ͼ�����
	int                     m_nRectHeight;    //��ͼ���߶�
	double                  m_fWidthRatio;    //������ű�
	double                 m_fHeightRatio;    //�߶����ű�

	//����
	CFont*                     m_pWndFont;  //��������
	CString                    m_FontName;  //��������
	int                       m_nFontSize;  //�����С

	//ָ������
	vector<CWnd*>             m_WndVector;  //����ָ������
	CPoint*                  m_ptArray[5];  //���Ƶ�����

	//����
	int                         m_nCurPos;  //��ǰλ��
	int                        m_nLastPos;  //��һ��λ��
	double                     m_dOldRate;  //�м��ռ�ı���

	//��ȡ������λͼָ��
	CImage*                      m_pImage;  //λͼָ��

	//*** ���� ***
	CDIP_SystemDoc* GetDocument();          //��û�ĵ�
	CDIP_SystemView* GetActiveView();       //��û��ͼ
	BOOL GetHistogram();                    //ͨ�����㣬ȡ��ֱ��ͼ
	BOOL GrayTransform();                   //����5�����Ƶ���лҶȱ任
	void SplineFunc();                      //����������ֵ����
	void ShowEditValue();                   //��ʾCEdit�ؼ���ֵ
	void UpdateControls(int number);        //���¿��Ƶ�

	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnChannelSelected();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnClickOK();
	afx_msg void OnClickCancel();
	afx_msg void OnEditLeftTop();
	afx_msg void OnEditTop();
	afx_msg void OnEditRightTop();
	afx_msg void OnEditLeftBottom();
	afx_msg void OnEditRightBottom();
	virtual void DoDataExchange(CDataExchange* pDX);

private://�����ӵ�˽�г�Ա����
	//
	//*** ���� ***
	//
	COLORREF ColorSetting(int Channel, int Strength, int WhereUse = 0);//������ɫ
	void DrawPoint(CPoint& pt, int size, CDC& MemDC);                  //���ƿ��Ƶ�
	//
	// *** ����������ֵ���� ***
	//
	void Spline(double* Node, double* Value, double* x0, double* s, int n, int m, double c1, double c2, int CASE = 2);
	void Chase(double* a, double* b, double* c, double* f, double* x, int n);
};

