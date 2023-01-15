#pragma once

// CCLAHEWnd

#include "DIP_SystemDoc.h"
#include "DIP_SystemView.h"
#include "resource.h"
#include <fstream>
using namespace std;

class CDIPSysDlg;

class CHistogramWnd : public CWnd
{
	DECLARE_DYNAMIC(CHistogramWnd)

public:
	CHistogramWnd();
	virtual ~CHistogramWnd();

protected:
	DECLARE_MESSAGE_MAP()

public://������Ա�뺯��
	CRect*                   m_pPaintRect;  //��ͼ��
	CRect*                  m_pClientRect;  //�ͻ���

	//��ť
	CMFCButton*                   m_pBnOK;  //ȷ��
	CMFCButton*               m_pBnCancel;  //ȡ��
	CMFCButton*                   m_pBnHE;  //HE���⻯
	CMFCButton*                  m_pBnLHE;  //LHE���⻯
	CMFCButton*                  m_pBnAHE;  //AHE���⻯
	CMFCButton*                m_pBnCLAHE;  //CLAHE���⻯
	CMFCButton*                m_pBnReset;  //����

	//����
	double*              m_pfHistogram[4];    //ֱ��ͼ:4*256
	double*              m_pfTotalHist[4];    //�ۼ�ֱ��ͼ
	int*                 m_V4Transform[4];    //�Ҷȱ任:4*m_nWidth
	BYTE*                       m_dataSrc;    //ͼ��Դ����
	BYTE*                       m_curData;    //ͼ��ǰ����

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

	//�ֲ�ֱ��ͼ�������
	int                     m_nRegionSize;  //�����С
	int                   m_nRegionLength;  //���򳤶�

	//��ȡ������λͼָ��
	CImage*                      m_pImage;  //λͼָ��

	//*** ���� ***
	BOOL GetImageInfo();                    //��ȡͼ����Ϣ
	CDIP_SystemDoc* GetDocument();          //��û�ĵ�
	CDIP_SystemView* GetActiveView();       //��û��ͼ
	BOOL GetHistogram();                    //ͨ�����㣬ȡ��ֱ��ͼ
	void HistogramEq(BYTE** Region, double Map[256]);//��ȡ�ֲ�ֱ��ͼ�����޸���������
	void MappingLHE();                      //�ֲ�ֱ��ͼ����
	void MappingHE();                       //����⻯ӳ�䣨��ͼ->��ͼ����ϵ����д����ͼ��
	void Reshape(CWnd* pWnd, double space, int Number);
	void GetBits(int channel, BYTE*);       //��ȡĳ��ͨ������
	void SetBits(int channel, BYTE*);       //����ĳ��ͨ������
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnCmdOK();
	afx_msg void OnCmdCancel();
	afx_msg void OnCmdHe();
	afx_msg void OnCmdLhe();
	afx_msg void OnCmdAhe();
	afx_msg void OnCmdClahe();
	afx_msg void OnCmdReset();
};

//*** ���� ***

COLORREF ColorSetting(int Channel, int Strength, int WhereUse = 0);//������ɫ
template<typename TYPE>
void Write2File(CString FilePath, TYPE* dataSrc, int Width, int Height);

//
/*********************** CLAHE ************************/
//

#define uiNR_OF_GREY (256)

int CLAHE(BYTE* pImage, unsigned int uiXRes, unsigned int uiYRes, BYTE Min, BYTE Max, unsigned int uiNrX, unsigned int uiNrY, unsigned int uiNrBins, float fCliplimit);

static void ClipHistogram (unsigned long*, unsigned int, unsigned long);

static void MakeHistogram (BYTE*, unsigned int, unsigned int, unsigned int, unsigned long*, unsigned int, BYTE*);

static void MapHistogram (unsigned long*, BYTE, BYTE, unsigned int, unsigned long);

static void MakeLut (BYTE*, BYTE, BYTE, unsigned int);

static void Interpolate (BYTE*, int, unsigned long*, unsigned long*, unsigned long*, unsigned long*, unsigned int, unsigned int, BYTE*);

