// ZoomDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"

#include "ZoomDlg.h"
#include "afxdialogex.h"
#include "DIP_SystemDoc.h"
#include "DIP_SystemView.h"
#include "MainFrm.h"

// CZoomDlg �Ի���

IMPLEMENT_DYNAMIC(CZoomDlg, CDialogEx)

CZoomDlg::CZoomDlg(CWnd* pParent /*=NULL*/) : CDialogEx(CZoomDlg::IDD, pParent)
{
	m_nComboSelected = 0;
	m_dWidthEdit = 512;
	m_dHeightEdit = 512;
	m_dImageRatio = 1;
	m_dWidthRatio = 1;
	m_dHeightRatio = 1;
	m_nMethod = 0;
	m_pDoc = NULL;
	m_pView = NULL;
	m_pImage = NULL;
	m_dataSrc = NULL;
	m_bModified = FALSE;
}

CZoomDlg::~CZoomDlg()
{
	SAFE_DELETE(m_dataSrc);
}

void CZoomDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ZOOM_DLG_COMBO, m_ZoomDlgCombo);
	DDX_Control(pDX, IDC_ZOOM_DLG_WIDTH, m_EditImageWidth);
	DDX_Control(pDX, IDC_ZOOM_DLG_HEIGHT, m_EditImageHeight);
	DDX_Control(pDX, IDC_ZOOM_DLG_RATIO, m_EditImageRatio);
	DDX_CBIndex(pDX, IDC_ZOOM_DLG_COMBO, m_nComboSelected);
	DDX_Text(pDX, IDC_ZOOM_DLG_WIDTH, m_dWidthEdit);
	DDX_Text(pDX, IDC_ZOOM_DLG_HEIGHT, m_dHeightEdit);
	DDX_Text(pDX, IDC_ZOOM_DLG_RATIO, m_dImageRatio);
	DDX_Control(pDX, IDC_ZOOM_DLG_METHOD, m_ComboMethod);
}

BEGIN_MESSAGE_MAP(CZoomDlg, CDialogEx)
	ON_BN_CLICKED(IDC_ZOOM_DLG_APPLY, &CZoomDlg::OnBnClickedZoomDlgApply)
	ON_CBN_SELCHANGE(IDC_ZOOM_DLG_COMBO, &CZoomDlg::OnCbnSelchangeZoomDlgCombo)
	ON_CBN_SELCHANGE(IDC_ZOOM_DLG_METHOD, &CZoomDlg::OnSelchangeZoomDlgMethod)
	ON_BN_CLICKED(IDOK, &CZoomDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CZoomDlg::OnBnClickedCancel)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

// CZoomDlg ��Ϣ�������

BOOL CZoomDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_ZoomDlgCombo.InsertString(0, L"����");
	m_ZoomDlgCombo.InsertString(1, L"���ű�");
	m_ZoomDlgCombo.InsertString(2, L"������");
	m_ZoomDlgCombo.SetCurSel(0);
	m_EditImageRatio.EnableWindow(FALSE);
	m_EditImageRatio.SetWindowText(L"1.00");

	m_ComboMethod.InsertString(0, L"���ٽ���ֵ");
	m_ComboMethod.InsertString(1, L"˫���Բ�ֵ");
	m_ComboMethod.SetCurSel(1);

	return TRUE;
}

// Ӧ��
void CZoomDlg::OnBnClickedZoomDlgApply()
{
	if (m_pImage != NULL)
	{
		UpdateData(TRUE);
		int nLastWidth = m_nWidthNew;
		int nLastHeight = m_nHeightNew;
		switch (m_nComboSelected)
		{
		case 0:
			m_nWidthNew = m_dWidthEdit;
			m_nHeightNew = m_dHeightEdit;
			m_dWidthRatio = 1.0*m_nWidthNew / m_nImageWidth;
			m_dHeightRatio = 1.0*m_nHeightNew / m_nImageHeight;
			break;
		case 1:
			m_dWidthRatio = m_dWidthEdit;
			m_dHeightRatio = m_dHeightEdit;
			m_nWidthNew = m_nImageWidth*m_dWidthRatio;
			m_nHeightNew = m_nImageHeight*m_dHeightRatio;
			break;
		case 2:
			m_nWidthNew = m_nImageWidth * m_dImageRatio;
			m_nHeightNew = m_nImageHeight * m_dImageRatio;
			m_dWidthRatio = 1.0*m_nWidthNew / m_nImageWidth;
			m_dHeightRatio = 1.0*m_nHeightNew / m_nImageHeight;
			break;
		default:
			break;
		}
		if (nLastWidth == m_nWidthNew && nLastHeight == m_nHeightNew)
			return ;
		switch (m_nMethod)
		{
		case 0://���ٽ���ֵ
			ZoomNormal(m_nWidthNew, m_nHeightNew);
			break;
		case 1://˫���Բ�ֵ
			ZoomInterpolation(m_nWidthNew, m_nHeightNew);
			break;
		default:
			ZoomNormal(m_nWidthNew, m_nHeightNew);
			break;
		}
		if (m_nImageWidth == m_nWidthNew && m_nImageHeight == m_nHeightNew)
			m_bModified = FALSE;
		else
			m_bModified = TRUE;
	}
}

BOOL CZoomDlg::ZoomNormal(int widthnew, int heightnew)
{
	double fx = m_dWidthRatio;
	double fy = m_dHeightRatio;
	long RowlenCur = WIDTHBYTES(widthnew * m_nChannel * 8);
	long DataLenCur = RowlenCur * m_nHeightNew;

	BYTE* tempPixel = new BYTE[DataLenCur];
	memset(tempPixel, 255, DataLenCur);

	BYTE*	copyPixel = NULL;	// ָ��ԭʼͼ������Ҫ�������ص���ʼλ��
	BYTE*	objPixel = NULL;	// ָ�򱻸������ص���ʼλ��
	int		x = 0;				// �任������غ�����
	int		y = 0;				// �任�������������
	long	tempY;				// �洢�м�ֵ,��ߺ����ٶ�
	long	tempJ;				// �洢�м�ֵ,��ߺ����ٶ�

	for (UINT j = 0; j < heightnew; j++)
	{
		// ����ٽ����ص�������
		y = (int)(j / fy + 0.5);

		// ��������
		if (y >= m_nImageHeight)
			y--;

		// ������i,x�޹ص��м�ֵ
		tempY = y * m_nRowLen;
		tempJ = j * RowlenCur;

		for (UINT i = 0; i < widthnew; i++) 
		{
			// ����ٽ����صĺ�����
			x = (int)(i / fx + 0.5);

			// ��������
			if (x >= m_nImageWidth)
				x--;
			copyPixel = m_dataSrc + tempY + x * m_nChannel;
			objPixel = tempPixel + tempJ + i * m_nChannel;
			memcpy(objPixel, copyPixel, m_nChannel);
		}
	}

	CImage* pImage = new CImage;
	if (m_nChannel == 1)//�Ҷ�ͼ�����⴦��
	{
		pImage->Create(widthnew, heightnew, 8);
		RGBQUAD ColorTab[256];
		for (int i = 0; i<256; i++)
		{
			ColorTab[i].rgbBlue = ColorTab[i].rgbGreen = ColorTab[i].rgbRed = i;
		}
		pImage->SetColorTable(0, 256, ColorTab);
	}
	else pImage->Create(widthnew, heightnew, m_nChannel * 8);
	BYTE* pBits = (BYTE *)pImage->GetBits() + (pImage->GetPitch()*(pImage->GetHeight() - 1));
	memcpy(pBits, tempPixel, DataLenCur);
	// ���ͼ������������
	m_pDoc->m_pDIB->Clear();
	m_pDoc->m_pDIB->m_UndoStack.SetTop(pImage);
	m_pDoc->m_pDIB->SetImage(pImage);
	m_pDoc->SetDocSize();
	m_pView->m_PaintRect.right = m_nWidthNew;
	m_pView->m_PaintRect.bottom = m_nHeightNew;
	m_pView->Invalidate(TRUE);
	
	delete [] tempPixel;
	return TRUE;
}

BOOL CZoomDlg::ZoomInterpolation(int widthnew, int heightnew)
{
	//��ȡԭͼ����
	BYTE* pBits = new BYTE[m_nDatalen];
	memcpy(pBits, m_dataSrc, m_nDatalen);

	//�������ź��ͼ��ߴ�����
	long WidthCur = widthnew;
	long HeightCur = heightnew;
	long RowlenCur = (widthnew*m_nChannel * 8 + 31) / 32 * 4;
	long DataLenCur = HeightCur * RowlenCur;
	//�������ű���
	float ratewidth = 1.0f * WidthCur / m_nImageWidth;
	float rateheight = 1.0f * HeightCur / m_nImageHeight;
	//����ռ����ڱ���Ŀ��ͼ�����ݼ��м���
	BYTE* dataStep1 = new BYTE[m_nImageHeight * RowlenCur];
	BYTE* dataDes = new BYTE[DataLenCur];

	//˫���β�ֵ��
	int r = 0, c = 0, k = 0;
	//�������ֵ
	for (r = 0; r < m_nImageHeight; r++)
	{
		BYTE* pDes = dataStep1 + r * RowlenCur;
		for (c = 0; c < WidthCur; c++)
		{
			int col = (int)(c / ratewidth);
			float fRateX = c / ratewidth - col;
			BYTE* pSrc = pBits + r * m_nRowLen + col * m_nChannel;
			if (col < m_nImageWidth - 1)
			{
				for (k = 0; k<m_nChannel; k++)
				{
					*(pDes + k) = (BYTE)(*(pSrc + k) * (1 - fRateX) + *(pSrc + m_nChannel + k) * fRateX);
				}
			}
			else
			{
				for (k = 0; k<m_nChannel; k++)
				{
					*(pDes + k) = (BYTE)*(pSrc + k);
				}
			}
			pDes += m_nChannel;
		}
	}
	delete[] pBits;
	pBits = NULL;
	//�������ֵ
	for (c = 0; c < WidthCur; c++)
	{
		BYTE* pDes = dataDes + c * m_nChannel;
		for (r = 0; r < HeightCur; r++)
		{
			int row = (int)(r / rateheight);
			float fRateY = r / rateheight - row;
			BYTE* pSrc = dataStep1 + row * RowlenCur + c * m_nChannel;
			if (row < m_nImageHeight - 1)
			{
				for (k = 0; k<m_nChannel; k++)
				{
					*(pDes + k) = (BYTE)(*(pSrc + k) * (1 - fRateY) + *(pSrc + RowlenCur + k) * fRateY);
				}
			}
			else
			{
				for (k = 0; k<m_nChannel; k++)
				{
					*(pDes + k) = (BYTE)*(pSrc + k);
				}
			}
			pDes += RowlenCur;
		}
	}
	CImage* pImage = new CImage;
	if (m_nChannel == 1)//�Ҷ�ͼ�����⴦��
	{
		pImage->Create(widthnew, heightnew, 8);
		RGBQUAD ColorTab[256];
		for (int i = 0; i<256; i++)
		{
			ColorTab[i].rgbBlue = ColorTab[i].rgbGreen = ColorTab[i].rgbRed = i;
		}
		pImage->SetColorTable(0, 256, ColorTab);
	}
	else pImage->Create(widthnew, heightnew, m_nChannel * 8);
	pBits = (BYTE *)pImage->GetBits() + (pImage->GetPitch()*(pImage->GetHeight() - 1));
	memcpy(pBits, dataDes, DataLenCur);
	// ���ͼ������������
	m_pDoc->m_pDIB->Clear();
	m_pDoc->m_pDIB->m_UndoStack.SetTop(pImage);
	m_pDoc->m_pDIB->SetImage(pImage);
	m_pDoc->SetDocSize();
	m_pView->m_PaintRect.right = m_nWidthNew;
	m_pView->m_PaintRect.bottom = m_nHeightNew;
	m_pView->Invalidate(TRUE);
	delete[] dataStep1;
	delete[] dataDes;
	return TRUE;
}

// ���¿ؼ���״̬
void CZoomDlg::OnCbnSelchangeZoomDlgCombo()
{
	m_EditImageWidth.EnableWindow(TRUE);
	m_EditImageHeight.EnableWindow(TRUE);
	m_EditImageRatio.EnableWindow(TRUE);
	m_nComboSelected = m_ZoomDlgCombo.GetCurSel();
	CString text1, text2;
	switch (m_nComboSelected)
	{
	case 0:
		text1.Format(L"%u", m_nWidthNew);
		text2.Format(L"%u", m_nHeightNew);
		m_EditImageWidth.SetWindowText(text1);
		m_EditImageHeight.SetWindowText(text2);
		m_EditImageRatio.EnableWindow(FALSE);
		break;
	case 1:
		text1.Format(L"%.2f", m_dWidthRatio);
		text2.Format(L"%.2f", m_dHeightRatio);
		m_EditImageWidth.SetWindowText(text1);
		m_EditImageHeight.SetWindowText(text2);
		m_EditImageRatio.EnableWindow(FALSE);
		break;
	case 2:
		text1.Format(L"%.2f", m_dImageRatio);
		m_EditImageRatio.SetWindowText(text1);
		m_EditImageWidth.EnableWindow(FALSE);
		m_EditImageHeight.EnableWindow(FALSE);
		break;
	default:
		break;
	}
}


// ��ȡ�û�ѡ��Ĳ�ֵ����
void CZoomDlg::OnSelchangeZoomDlgMethod()
{
	m_nComboSelected = m_ComboMethod.GetCurSel();
}

void CZoomDlg::OnBnClickedOk()
{
	CDialogEx::OnOK();
	OnBnClickedZoomDlgApply();
}

void CZoomDlg::OnBnClickedCancel()
{
	if (m_bModified)
	{
		m_dImageRatio = 1;
		m_dWidthRatio = 1;
		m_dHeightRatio = 1;
		m_nWidthNew = m_nImageWidth;
		m_nHeightNew = m_nImageHeight;
		ZoomNormal(m_nImageWidth, m_nImageHeight);
		m_bModified = FALSE;
	}
	CDialogEx::OnCancel();
}

void CZoomDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (bShow)
	{
		CMDIFrameWnd* pFrame = (CMDIFrameWnd *)AfxGetApp()->GetMainWnd();
		CMDIChildWnd* pChild = (CMDIChildWnd *)pFrame->GetActiveFrame();
		m_pDoc = (CDIP_SystemDoc *)pChild->GetActiveDocument();
		m_pView = (CDIP_SystemView *)pChild->GetActiveView();
		m_pImage = m_pDoc->DIBImage();
		m_nChannel = m_pImage->GetBPP() / 8;
		m_nImageWidth = m_pImage->GetWidth();
		m_nImageHeight = m_pImage->GetHeight();
		m_nRowLen = abs(m_pImage->GetPitch());
		m_nDatalen = m_nImageHeight*m_nRowLen;
		//������ʾ
		UpdateData(TRUE);
		OnCbnSelchangeZoomDlgCombo();

		SAFE_DELETE(m_dataSrc);
		m_dataSrc = new BYTE[m_nDatalen];
		BYTE* pBits = (BYTE *)m_pImage->GetBits() + (m_pImage->GetPitch()*(m_nImageHeight - 1));
		memcpy(m_dataSrc, pBits, m_nDatalen);
	}
}