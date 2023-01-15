#pragma once

// CSpaceFilterWnd

//
// *** class CSpaceFilterWnd ***
//

#include "DIP_SystemDoc.h"
#include "DIP_SystemView.h"
#include "resource.h"

#define MAX_FILTER_SIZE				7

class CFilter;

class CSpaceFilterWnd : public CWnd
{
	DECLARE_DYNAMIC(CSpaceFilterWnd)

public:
	CSpaceFilterWnd();
	virtual ~CSpaceFilterWnd();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

public://������Ա�뺯��
	CRect*                                m_pPaintRect;  //��ͼ��
	CImage*                                   m_pImage;  //ԭͼ��ָ��
	BYTE*                                    m_dataSrc;  //ԭͼ������
	CEdit*m_pElements[MAX_FILTER_SIZE*MAX_FILTER_SIZE];  //�༭��

	CDIP_SystemDoc*                             m_pDoc;  //��ĵ�
	CDIP_SystemView*                           m_pView;  //���ͼ

	//ͼ�������Ϣ
	int                                  m_nImageWidth;  //ͼ����
	int                                 m_nImageHeight;  //ͼ��߶�
	int                                     m_nChannel;  //ͼ��ͨ��
	long                                     m_nRowlen;  //���ֽ���
	long                                    m_nlenData;  //���ݳ���

	//��ť
	CMFCButton*                                m_pBnOK;  //ȷ��
	CMFCButton*                            m_pBnCancel;  //ȡ��
	CMFCButton*                             m_pBnReset;  //����
	CMFCButton*                      m_pBnMedianFilter;  //��ֵ�˲�
	CMFCButton*                    m_pBnAdaptiveFilter;  //����Ӧ��ֵ�˲�
	CMFCButton*                m_pBnAddPepperSaltNoise;  //��������
	CMFCButton*                  m_pBnAddGaussianNoise;  //��˹����
	CMFCButton*                             m_pBnApply;  //Ӧ�ò���
	CComboBox*                       m_pComboOperation;  //��������
	CComboBox*                      m_pComboFilterSize;  //�˲�����С
	CFont*                                  m_pAppFont;  //����
	CFont*                                m_pSmallFont;  //��С������

	//����
	CFilter*                             m_pPixelBlock;  //���ؿ�
	CFilter*                               m_pMyFilter;  //�˲���
	int                                  m_nFilterSize;  //�˲�������
	int                               m_nMaxFilterSize;  //������
	int                                   m_nOperation;  //ѡ�еĲ���

	//Gauss Noise
	double                                      m_Mean;  //��ֵ
	double                                       m_Std;  //����

	//Pepper & Salt Noise
	double                                    m_Pepper;  //���������к�������
	double                                 m_Pollution;  //ͼ����Ⱦ�̶�

	//*** ���� ***
	void Reshape(CWnd* pWnd, double Space, int nRow, int nCol = 1);
	                                                     //�ػ水ť
	BOOL AddPepperSaltNoise();                           //��ӽ�������
	BOOL AddGaussianNoise();                             //��Ӹ�˹����
	BOOL MedianFilter();                                 //��ֵ�˲�
	BOOL AdaptMedianFilter();                            //����Ӧ��ֵ�˲�
	int FilterProcessA(BYTE* dataSrc, int curChannel, int y, int x, int FilterSize);
	int FilterProcessB(int Zmin, int Zmax, int Zmed, int Zxy);
	void Convolute(CFilter* Filter);                     // ���

	afx_msg void OnCmdOK();
	afx_msg void OnCmdCancel();
	afx_msg void OnCmdReset();
	afx_msg void OnCmdMedianFilter();
	afx_msg void OnCmdAdaptiveFilter();
	afx_msg void OnCmdAddPepperSaltNoise();
	afx_msg void OnCmdAddGaussianNoise();
	afx_msg void OnCmdApply();
	afx_msg void OnCmdChooseOperation();
	afx_msg void OnCmdChangeFilterSize();
};

double ProduceGaussianNoise(double nMean = 0, double nStd = 1);   //������˹����:�������ļ��޶���
double ProduceGaussianNoise2(double nMean = 0, double nStd = 1);  //������˹����:����Box-Muller�취
int ProducePepperSaltNoise(double Probability = 0.05);            //��������������ProbabilityΪ��������

