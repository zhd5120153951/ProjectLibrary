// DIP_SystemView.cpp : implementation of the CDIP_SystemView class
//

#include "stdafx.h"
#include "DIP_System.h"

#include "DIP_SystemDoc.h"
#include "DIP_SystemView.h"
#include "MainFrm.h"
#include "SplitProc.h"
#include "GrayProc.h"
#include "DlgIntensity.h"
#include "DlgPointStre.h"
#include "DlgPointWin.h"
#include "DlgPointDT.h"
#include "DlgPointDZ.h"
#include "DLGSetAngle.h"
#include "DynamicCompressParam.h"
#include "GeoTrans.h"
#include "FreqCalculate.h"
#include "SpaceFilter.h"
#include "FreqFilter.h"
#include "DLGSetFreq.h"
#include "Coding.h"
#include "Morph.h"
#include "Noise.h"
#include "WaterMarking.h"

// 2014��7������
#include "ZoomDlg.h"
#include "ColorGradDlg.h"
#include "CurveDlg.h"
#include "HistogramDlg.h"
#include "SpaceFilterDlg.h"
#include "EnhancedHistDlg.h"
#include "StretchDlg.h"
#include "ThresholdDlg.h"
#include "ChangeBppDlg.h"
#include "RotateDlg.h"
#include "GeoOperator.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ����ģʽ
static enum EmbossType
{
	EMBOSS_1 = 0,
	EMBOSS_2,
	EMBOSS_3,
	EMBOSS_4,
	EMBOSS_5,
	EMBOSS_6,
	EMBOSS_7,
	EMBOSS_8
};

/////////////////////////////////////////////////////////////////////////////
// CDIP_SystemView

IMPLEMENT_DYNCREATE(CDIP_SystemView, CScrollView)

BEGIN_MESSAGE_MAP(CDIP_SystemView, CScrollView)
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_COMMAND(ID_TOGRAY, OnTogray)
	ON_COMMAND(ID_RGN_GROW, OnRgnGrow)
	ON_COMMAND(ID_POINT_INVERT, OnPointInvert)
	ON_COMMAND(ID_POINT_EQUA, OnPointEqua)
	ON_COMMAND(ID_VIEW_INTENSITY, OnViewYIntensity)
	ON_COMMAND(ID_VIEW_R_INTENSITY, OnViewRIntensity)
	ON_COMMAND(ID_VIEW_G_INTENSITY, OnViewGIntensity)
	ON_COMMAND(ID_VIEW_B_INTENSITY, OnViewBIntensity)
	ON_COMMAND(ID_POINT_STRE, OnPointStre)
	ON_COMMAND(ID_POINT_WIND, OnPointWind)
	ON_COMMAND(ID_POINT_DT, OnPointDt)
	ON_COMMAND(ID_POINT_DZ, OnPointDz)
	ON_COMMAND(ID_SET_INTENSITY, OnSetIntensity)
	ON_COMMAND(ID_POINT_SML, OnPointSml)
	ON_COMMAND(ID_POINT_GML, OnPointGml)
	ON_COMMAND(ID_DYNAMIC_COMPRESS, OnDynamicCompress)
	ON_COMMAND(ID_GEOM_ZOOMIN, OnGeomZoomin)
	ON_COMMAND(ID_GEOM_ZOOMOUT, OnGeomZoomout)
	ON_COMMAND(ID_FREQ_FOUR, OnFreqFour)
	ON_COMMAND(ID_BUTTERWORTH_L, OnButterworthL)
	ON_COMMAND(IDM_EMBOSS_1, OnEmboss_1)
	ON_COMMAND(IDM_EMBOSS_2, OnEmboss_2)
	ON_COMMAND(IDM_EMBOSS_3, OnEmboss_3)
	ON_COMMAND(IDM_EMBOSS_4, OnEmboss_4)
	ON_COMMAND(IDM_EMBOSS_5, OnEmboss_5)
	ON_COMMAND(IDM_EMBOSS_6, OnEmboss_6)
	ON_COMMAND(IDM_EMBOSS_7, OnEmboss_7)
	ON_COMMAND(IDM_EMBOSS_8, OnEmboss_8)
	ON_COMMAND(ID_BUTTERWORTH_H, OnButterworthH)
	ON_COMMAND(ID_MUTUAL_FILTER, OnMutualFilter)
	ON_COMMAND(ID_LINE_SMOOTH_FILTER, OnLineSmoothFilter)
	ON_COMMAND(ID_LINE_QUICK_FILTER, OnLineQuickFilter)
	ON_COMMAND(ID_MEDIAN_FILTER, OnMedianFilter)
	ON_COMMAND(ID_JPEG_ENCODING, OnJpegEncoding)
	ON_COMMAND(ID_JPEG_DECODING, OnJpegDecoding)
	ON_COMMAND(ID_RETRORSE_FILTER, OnRetrorseFilter)
	ON_COMMAND(ID_WIENER_FILTER, OnWienerFilter)
	ON_COMMAND(ID_SOBEL, OnSobel)
	ON_COMMAND(ID_PREWITT, OnPrewitt)
	ON_COMMAND(ID_ROBERTS, OnRoberts)
	ON_COMMAND(ID_LAPLACIAN, OnLaplacian)
	ON_COMMAND(ID_GUASS__LAPLACIAN, OnGuassLaplacian)
	ON_COMMAND(ID_HOUGH, OnHough)
	ON_COMMAND(ID_KRISCH, OnKrisch)
	ON_COMMAND(ID_CONTOURDIB, OnContourdib)
	ON_COMMAND(ID_TRACEDIB, OnTracedib)
	ON_COMMAND(ID_ITERATE_THRESHOLD, OnIterateThreshold)
	ON_COMMAND(ID_FREQ_DCT, OnFreqDct)
	ON_COMMAND(ID_FREQ_WALH, OnFreqWalh)
	ON_COMMAND(ID_H_EROSION, OnHErosion)
	ON_COMMAND(ID_V_EROSION, OnVErosion)
	ON_COMMAND(ID_H_DILATION, OnHDilation)
	ON_COMMAND(ID_V_DILATION, OnVDilation)
	ON_COMMAND(ID_OPEN_OPERATE, OnOpenOperate)
	ON_COMMAND(ID_CLOSE_OPERATE, OnCloseOperate)
	ON_COMMAND(ID_VH_EROSION, OnVhErosion)
	ON_COMMAND(ID_VH_DILATION, OnVhDilation)
	ON_COMMAND(ID_THIN, OnThin)
	ON_COMMAND(ID_RANDOM_NOISE, OnRandomNoise)
	ON_COMMAND(ID_SALT_NOISE, OnSaltNoise)
	ON_COMMAND(ID_PCX_ENCODING, OnPCXEncoding)
	ON_COMMAND(ID_PCX_DECODING, OnPCXDecoding)
	ON_COMMAND(ID_LINE_SMOOTH_FILTER2, OnLineSmoothFilter2)
	ON_COMMAND(ID_LINE_SMOOTH_FILTER3, OnLineSmoothFilter3)
	ON_COMMAND(ID_OVERRUN_FILTER, OnOverrunFilter)
	ON_COMMAND(ID_CUTWAVE, OnCutWave)
	ON_COMMAND(ID_PSE_FILTER, OnPSE_Filter)
	ON_COMMAND(ID_MIN_POWER, OnMinPower)
	ON_COMMAND(ID_ROTATE, OnRotate)
	ON_COMMAND(ID_RECTINICATION, OnRectinication)
	ON_COMMAND(ID_THICK, OnThick)
	ON_COMMAND(ID_MAT, OnMat)
	ON_COMMAND(ID_HUFFMAN, OnHuffman)
	ON_COMMAND(ID_SHANNON_FANNON, OnShannonFannon)
	ON_COMMAND(ID_LZW_ENCODING, OnLzwEncoding)
	ON_COMMAND(ID_LZW_DECODING, OnLzwDecoding)
	ON_COMMAND(ID_SET_WATERMARKING, OnSetWatermarking)
	ON_COMMAND(ID_GET_WATERMARKING, OnGetWatermarking)
	ON_COMMAND(ID_DWT, OnDwt)
	ON_COMMAND(ID_IDWT, OnIdwt)
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
	ON_COMMAND(ID_CALL_ZOOM_DLG, &CDIP_SystemView::OnCallZoomDlg)
	ON_COMMAND(ID_CALL_COLORGRAD_DLG, &CDIP_SystemView::OnCallColorgradDlg)
	ON_COMMAND(ID_CALL_CURVE_DLG, &CDIP_SystemView::OnCallCurveDlg)
	ON_COMMAND(ID_CALL_HISTOGRAM_DLG, &CDIP_SystemView::OnCallHistogramDlg)
	ON_COMMAND(ID_CALL_SPACEFILTER_DLG, &CDIP_SystemView::OnCallSpacefilterDlg)
	ON_COMMAND(ID_TEMP_FILE, &CDIP_SystemView::OnTempFile)
	ON_UPDATE_COMMAND_UI(ID_TEMP_FILE, &CDIP_SystemView::OnUpdateTempFile)
	ON_COMMAND(ID_HISTOGRAM_II, &CDIP_SystemView::OnHistogramIi)
	ON_COMMAND(ID_CALL_STRETCH_DLG, &CDIP_SystemView::OnCallStretchDlg)
	ON_COMMAND(ID_CALL_THRESHOLD_DLG, &CDIP_SystemView::OnCallThresholdDlg)
	ON_COMMAND(ID_EDIT_UNDO, &CDIP_SystemView::OnEditUndo)
	ON_COMMAND(ID_EDIT_REDO, &CDIP_SystemView::OnEditRedo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, &CDIP_SystemView::OnUpdateEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, &CDIP_SystemView::OnUpdateEditRedo)
	ON_COMMAND(ID_VIEW_ZOOMIN, &CDIP_SystemView::OnViewZoomin)
	ON_COMMAND(ID_VIEW_ZOOMOUT, &CDIP_SystemView::OnViewZoomout)
	ON_COMMAND(ID_VIEW_FIT_WINDOW, &CDIP_SystemView::OnViewFitWindow)
	ON_COMMAND(ID_VIEW_SCREEN_SIZE, &CDIP_SystemView::OnViewScreenSize)
	ON_COMMAND(ID_VIEW_DEFAULT, &CDIP_SystemView::OnViewDefault)
	ON_COMMAND(ID_CALL_BPP_DLG, &CDIP_SystemView::OnCallBppDlg)
	ON_COMMAND(ID_CALL_ROTATE_DLG, &CDIP_SystemView::OnCallRotateDlg)
	ON_COMMAND(ID_FLIP_VERTICAL, &CDIP_SystemView::OnFlipVertical)
	ON_COMMAND(ID_FLIP_HORIZONTAL, &CDIP_SystemView::OnFlipHorizontal)
	ON_COMMAND(ID_TRANSPOSE, &CDIP_SystemView::OnTranspose)
	ON_COMMAND(ID_GET_CURRENT_PIXEL, &CDIP_SystemView::OnGetCurrentPixel)
	ON_UPDATE_COMMAND_UI(ID_GET_CURRENT_PIXEL, &CDIP_SystemView::OnUpdateGetCurrentPixel)
	ON_COMMAND(ID_VIEW_R_CHANNEL_IMAGE, &CDIP_SystemView::OnViewRChannelImage)
	ON_COMMAND(ID_VIEW_G_CHANNEL_IMAGE, &CDIP_SystemView::OnViewGChannelImage)
	ON_COMMAND(ID_VIEW_B_CHANNEL_IMAGE, &CDIP_SystemView::OnViewBChannelImage)
	ON_COMMAND(ID_CONVERT_RGB_2_RBG, &CDIP_SystemView::OnConvertRgb2Rbg)
	ON_COMMAND(ID_CONVERT_RGB_2_BRG, &CDIP_SystemView::OnConvertRgb2Brg)
	ON_COMMAND(ID_CONVERT_RGB_2_BGR, &CDIP_SystemView::OnConvertRgb2Bgr)
	ON_COMMAND(ID_CONVERT_RGB_2_GRB, &CDIP_SystemView::OnConvertRgb2Grb)
	ON_COMMAND(ID_CONVERT_RGB_2_GBR, &CDIP_SystemView::OnConvertRgb2Gbr)
	ON_COMMAND(ID_VIEW_ALL_CHANNEL_IMAGE, &CDIP_SystemView::OnViewAllChannelImage)
	ON_UPDATE_COMMAND_UI(ID_VIEW_R_CHANNEL_IMAGE, &CDIP_SystemView::OnUpdateViewRChannelImage)
	ON_UPDATE_COMMAND_UI(ID_VIEW_G_CHANNEL_IMAGE, &CDIP_SystemView::OnUpdateViewGChannelImage)
	ON_UPDATE_COMMAND_UI(ID_VIEW_B_CHANNEL_IMAGE, &CDIP_SystemView::OnUpdateViewBChannelImage)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ALL_CHANNEL_IMAGE, &CDIP_SystemView::OnUpdateViewAllChannelImage)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOMIN, &CDIP_SystemView::OnUpdateViewZoomin)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOMOUT, &CDIP_SystemView::OnUpdateViewZoomout)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SCREEN_SIZE, &CDIP_SystemView::OnUpdateViewScreenSize)
	ON_UPDATE_COMMAND_UI(ID_VIEW_FIT_WINDOW, &CDIP_SystemView::OnUpdateViewFitWindow)
	ON_UPDATE_COMMAND_UI(ID_VIEW_DEFAULT, &CDIP_SystemView::OnUpdateViewDefault)
	ON_COMMAND(ID_HOTKEY_ALT_Z, &CDIP_SystemView::OnHotkeyAltZ)
	ON_COMMAND(ID_HOTKEY_CTRL_0, &CDIP_SystemView::OnHotkeyCtrl0)
	ON_COMMAND(ID_HOTKEY_CTRL_C, &CDIP_SystemView::OnHotkeyCtrlC)
	ON_COMMAND(ID_HOTKEY_CTRL_F, &CDIP_SystemView::OnHotkeyCtrlF)
	ON_COMMAND(ID_HOTKEY_CTRL_H, &CDIP_SystemView::OnHotkeyCtrlH)
	ON_COMMAND(ID_HOTKEY_CTRL_L, &CDIP_SystemView::OnHotkeyCtrlL)
	ON_COMMAND(ID_HOTKEY_CTRL_M, &CDIP_SystemView::OnHotkeyCtrlM)
	ON_COMMAND(ID_HOTKEY_CTRL_N, &CDIP_SystemView::OnHotkeyCtrlN)
	ON_COMMAND(ID_HOTKEY_CTRL_O, &CDIP_SystemView::OnHotkeyCtrlO)
	ON_COMMAND(ID_HOTKEY_CTRL_P, &CDIP_SystemView::OnHotkeyCtrlP)
	ON_COMMAND(ID_HOTKEY_CTRL_Q, &CDIP_SystemView::OnHotkeyCtrlQ)
	ON_COMMAND(ID_HOTKEY_CTRL_R, &CDIP_SystemView::OnHotkeyCtrlR)
	ON_COMMAND(ID_HOTKEY_CTRL_S, &CDIP_SystemView::OnHotkeyCtrlS)
	ON_COMMAND(ID_HOTKEY_CTRL_V, &CDIP_SystemView::OnHotkeyCtrlV)
	ON_COMMAND(ID_HOTKEY_CTRL_W, &CDIP_SystemView::OnHotkeyCtrlW)
	ON_COMMAND(ID_HOTKEY_CTRL_Z, &CDIP_SystemView::OnHotkeyCtrlZ)
	ON_COMMAND(ID_HOTKEY_ZOOM_IN, &CDIP_SystemView::OnHotkeyZoomIn)
	ON_COMMAND(ID_HOTKEY_ZOOM_OUT, &CDIP_SystemView::OnHotkeyZoomOut)
	END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDIP_SystemView construction/destruction
CDIP_SystemView::CDIP_SystemView()
{
	m_PaintRect = CRect();
	m_nInteract = 0;
	m_bShowImage = TRUE;
	m_nViewType = ID_VIEW_DEFAULT;

	m_pZoomDlg = new CZoomDlg;
	m_pZoomDlg->Create(IDD_DLG_ZOOM, this);
	m_pZoomDlg->SETDLGICON(IDI_ZOOM_DLG);

	m_Color_I = m_Color_II = RGB(0, 0, 0);

	m_bGetCurPixel = false;
	m_bRNegativeImage = 0;
	m_bGNegativeImage = 0;
	m_bBNegativeImage = 0;
	m_nCurSeed = 1;
}

CDIP_SystemView::~CDIP_SystemView()
{
	DELETE_WND(m_pZoomDlg);
}

BOOL CDIP_SystemView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////[��������]/////////////////////////////////////////////////

///////////////////////////////////////////
//	��ȡ�����浱ǰ�����ɫֵ			 //	
///////////////////////////////////////////
void CDIP_SystemView::CurrentRGB(CPoint point)
{
	CDC* pDC = GetDC();
	m_Color_I = m_Color_II;
	m_Color_II = pDC->GetPixel(point);
	if (m_Color_I == m_Color_II)
		return;
	// ����ɫ����
	int R = GetRValue(m_Color_II);
	int G = GetGValue(m_Color_II);
	int B = GetBValue(m_Color_II);
	// ���浱ǰ����ɫֵ
	CString  strRGB;
	strRGB.Format(_T("R = %d\nG = %d\nB = %d"), R, G, B);
	m_ToolTip.UpdateTipText(strRGB, this);
}

// ���û�ͼ����С
void CDIP_SystemView::SetPaintRect(CSize size)
{
	m_PaintRect.right = size.cx;
	m_PaintRect.bottom = size.cy;
}

void CDIP_SystemView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	//���Ҽ��˳�ʰȡ����
	m_bGetCurPixel = false;
	m_nInteract = 0;
	SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
	ReleaseCapture();
	CScrollView::OnRButtonDown(nFlags, point);
}


void CDIP_SystemView::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CScrollView::OnRButtonUp(nFlags, point);
}



void CDIP_SystemView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_bGetCurPixel)
	{
		CDIP_SystemDoc* pDoc = GetDocument();
		CPoint curPos = pDoc->m_ptLastPos;
		pDoc->m_ptLastPos = point;
		// ����ƶ��˲Ÿ���tip
		if (curPos != pDoc->m_ptLastPos)
			CurrentRGB(point);
	}

	CScrollView::OnMouseMove(nFlags, point);
}


void CDIP_SystemView::OnLButtonUp(UINT nFlags, CPoint point)
{
	CDIP_SystemDoc* pDoc = GetDocument();
	if (m_bLeftButtonDown)
	{
		m_bLeftButtonDown = false;
		CDC* pDC = GetDC();
		if (m_nCurSeed == 1)
		{
			pDoc->m_ptPoint_I = point;
			// ��ȡ����һ��RGBֵ
			m_clrSeed_I = pDC->GetPixel(pDoc->m_ptPoint_I);
			m_nCurSeed = 2;
		}
		else
		{
			pDoc->m_ptPoint_II = point;
			// ��ȡ���Ӷ���RGBֵ
			m_clrSeed_II = pDC->GetPixel(pDoc->m_ptPoint_II);
			m_nCurSeed = 1;
		}
		ReleaseDC(pDC);
		Invalidate(TRUE);
	}

	CScrollView::OnLButtonUp(nFlags, point);
}

void CDIP_SystemView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_bLeftButtonDown = true;

	// ��ȡ�ĵ�
	CDIP_SystemDoc* pDoc = GetDocument();

	// �ڽ���״̬�¶Ե�ǰ�㻭Բ
	DrawCircle(point);

	// �ػ���ͼ
	Invalidate();

	// ��ȡ�豸�������
	CDC* pDC = GetDC();
	
	// �ж��Ƿ�ѡ����Ȧ����
	BOOL bResult = pDoc->m_Tracker.HitTest(point) != CRectTracker::hitNothing;
	
	// ���ѡ�о���
	if (bResult)
	{
		if(pDoc->m_bIsFrequency)
		{
			// ��ȡ��ǰ����λ��
			CPoint pt = GetScrollPosition();

			// ����ѡ�о�����ȫͼ�е�����
			pDoc->m_rect[pDoc->m_nRgnNum] = pDoc->m_Tracker.m_rect;
			pDoc->m_rect[pDoc->m_nRgnNum].top += pt.y;
			pDoc->m_rect[pDoc->m_nRgnNum].bottom += pt.y;
			pDoc->m_rect[pDoc->m_nRgnNum].left += pt.x;
			pDoc->m_rect[pDoc->m_nRgnNum].right += pt.x;
		
			// ��ȡ��ǰͼ��ߴ�
			CSize size = pDoc->GetDocSize();

			// Խ�籣��
			if (pDoc->m_rect[pDoc->m_nRgnNum].top < 0)
				pDoc->m_rect[pDoc->m_nRgnNum].top = 0;
			if (pDoc->m_rect[pDoc->m_nRgnNum].bottom > size.cy)
				pDoc->m_rect[pDoc->m_nRgnNum].bottom = size.cy;
			if (pDoc->m_rect[pDoc->m_nRgnNum].left < 0)
				pDoc->m_rect[pDoc->m_nRgnNum].left = 0;
			if (pDoc->m_rect[pDoc->m_nRgnNum].right > size.cx)
				pDoc->m_rect[pDoc->m_nRgnNum].right = size.cx;

			// ����ѡ���������ɫ
			pDoc->m_pDIB->SetRgnColor(pDoc->m_rect[pDoc->m_nRgnNum], RGB(0, 0, 0));
		
			// �ػ���ͼ
			InvalidateRect(pDoc->m_rect[pDoc->m_nRgnNum]);

			// ѡ�������������
			pDoc->m_nRgnNum++;
	
			// ��������
			pDoc->SetModifiedFlag(TRUE);
		}
	}
	// �������ѡ���������Ϸ�
	else
		pDoc->m_Tracker.TrackRubberBand(this, point, TRUE);

	// ��ѡ������߿�
	pDoc->m_Tracker.Draw(pDC);

	// �ͷ���Դ
	ReleaseDC(pDC);

	CScrollView::OnLButtonDown(nFlags, point);
}

///////////////////////////////////////////
//	�ڵ�ǰ�㻭Բ						 //
///////////////////////////////////////////
void CDIP_SystemView::DrawCircle(CPoint point)
{
	if (m_nInteract > 0)
	{
		// ��ȡ�ĵ�
		CDIP_SystemDoc* pDoc = GetDocument();

		// ��ȡ��ǰͼ��ߴ�
		CSize size = pDoc->GetDocSize();

		// ��ȡ��ǰ����λ��
		CPoint pt = GetScrollPosition();
		
		// �궨������
		CRect rect(point.x + pt.x + 8, point.y + pt.y + 8, point.x + pt.x + 24, point.y + pt.y + 24);

		// Խ�籣��
		if (rect.top < 0 || rect.bottom > size.cy + 8 || rect.left < -8 || rect.right > size.cx + 8)
			return;		
			
		// ��ȡ�豸�������
		CDC* pDC = GetDC();
		
		// ����ѡ���������ɫ
		COLORREF col;
		if (m_nInteract == 1)
			col = RGB(255, 0, 0);
		if (m_nInteract == 2)
			col = RGB(0, 0, 0);
		if (m_nInteract == 3)
			col = RGB(255, 255, 255);
		pDoc->m_pDIB->SetCircleColor(rect, col);
		
		// �ػ���ͼ
		InvalidateRect(rect);

		// �ͷ���Դ
		ReleaseDC(pDC);
	}
}

///////////////////////////////////////////
//	�ػ���ͼ							 //
///////////////////////////////////////////
void CDIP_SystemView::OnDraw(CDC* pDC)
{
	CDIP_SystemDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	// ��ȡָ��
	HDIB hDIB = pDoc->GetHDIB();
	CImage* pImage = pDoc->DIBImage();
	if (hDIB == NULL) m_bShowImage = TRUE;

	// ʹ��ͼ��Ӧ����
	if (m_nViewType == ID_VIEW_FIT_WINDOW)
	{
		CRect rect;
		GetClientRect(&rect);
		m_PaintRect = rect;
		pDoc->SetDocSize(m_PaintRect.right, m_PaintRect.bottom);
	}

	// ��ʾ��ǰͼ��
	if (m_bShowImage)
	{
		if (pImage != NULL)
		{
			int cxDIB = (int)pDoc->m_pDIB->Width();
			int cyDIB = (int)pDoc->m_pDIB->Height();
			CRect rcDIB(0, 0, cxDIB, cyDIB);
			CRect rcDest = m_PaintRect;
			// �ж��Ƿ��Ǵ�ӡ
			if (pDC->IsPrinting())
			{
				// �Ǵ�ӡ���������ͼ���λ�úʹ�С���Ա����ҳ��

				// ��ȡ��ӡҳ���ˮƽ���(����)
				int cxPage = pDC->GetDeviceCaps(HORZRES);

				// ��ȡ��ӡҳ��Ĵ�ֱ�߶�(����)
				int cyPage = pDC->GetDeviceCaps(VERTRES);

				// ��ȡ��ӡ��ÿӢ��������
				int cxInch = pDC->GetDeviceCaps(LOGPIXELSX);
				int cyInch = pDC->GetDeviceCaps(LOGPIXELSY);

				// �����ӡͼ���С�����ţ�����ҳ���ȵ���ͼ���С��
				rcDest.top = rcDest.left = 0;
				rcDest.bottom = (int)(((double)cyDIB * cxPage * cyInch) / ((double)cxDIB * cxInch));
				rcDest.right = cxPage;

				// �����ӡͼ��λ�ã���ֱ���У�
				int temp = cyPage - (rcDest.bottom - rcDest.top);
				rcDest.bottom += temp / 2;
				rcDest.top += temp / 2;
			}
			pDoc->m_pDIB->Paint(pDC->m_hDC, &rcDest, &rcDIB);
		}
	}
	//��ʾ����ͼ��
	else
	{
		if (hDIB != NULL)
		{
			LPBYTE lpDIB = (LPBYTE) ::GlobalLock((HGLOBAL)hDIB);
			int cxDIB = (int)pDoc->m_pDIB->Width(lpDIB);
			int cyDIB = (int)pDoc->m_pDIB->Height(lpDIB);
			::GlobalUnlock((HGLOBAL)hDIB);
			CRect rcDIB(0, 0, cxDIB, cyDIB);
			CRect rcDest = m_PaintRect;
			// �ж��Ƿ��Ǵ�ӡ
			if (pDC->IsPrinting())
			{
				// �Ǵ�ӡ���������ͼ���λ�úʹ�С���Ա����ҳ��

				// ��ȡ��ӡҳ���ˮƽ���(����)
				int cxPage = pDC->GetDeviceCaps(HORZRES);

				// ��ȡ��ӡҳ��Ĵ�ֱ�߶�(����)
				int cyPage = pDC->GetDeviceCaps(VERTRES);

				// ��ȡ��ӡ��ÿӢ��������
				int cxInch = pDC->GetDeviceCaps(LOGPIXELSX);
				int cyInch = pDC->GetDeviceCaps(LOGPIXELSY);

				// �����ӡͼ���С�����ţ�����ҳ���ȵ���ͼ���С��
				rcDest.top = rcDest.left = 0;
				rcDest.bottom = (int)(((double)cyDIB * cxPage * cyInch) / ((double)cxDIB * cxInch));
				rcDest.right = cxPage;

				// �����ӡͼ��λ�ã���ֱ���У�
				int temp = cyPage - (rcDest.bottom - rcDest.top);
				rcDest.bottom += temp / 2;
				rcDest.top += temp / 2;
			}
			pDoc->m_pDIB->Paint(pDC->m_hDC, &rcDest, pDoc->GetHDIB(), &rcDIB, pDoc->GetDocPalette());
		}
	}

	// �����ӵ�1
	if (pDoc->m_ptPoint_I != CPoint(0, 0))
	{
		pDC->Ellipse(pDoc->m_ptPoint_I.x - 3, pDoc->m_ptPoint_I.y - 3, 
			pDoc->m_ptPoint_I.x + 3, pDoc->m_ptPoint_I.y + 3);
	}

	// �����ӵ�2
	if (pDoc->m_ptPoint_II != CPoint(0, 0))
	{
		pDC->Ellipse(pDoc->m_ptPoint_II.x - 3, pDoc->m_ptPoint_II.y - 3, 
			pDoc->m_ptPoint_II.x + 3, pDoc->m_ptPoint_II.y + 3);
	}
}

void CDIP_SystemView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	// ref��http://bbs.csdn.net/topics/230047928
	m_bLeftButtonDown = false;
	m_ToolTip.Create(this, TTS_ALWAYSTIP);       
	m_ToolTip.AddTool(this, _T(""));   
	m_ToolTip.SetTipBkColor(RGB(0,222,0));
	m_ToolTip.SetMaxTipWidth(50);
	m_ToolTip.SetDelayTime(500);
	SetScrollSizes(MM_TEXT, GetDocument()->GetDocSize());
}

/////////////////////////////////////////////////////////////////////////////
//									 ͼ���ӡ							   //
/////////////////////////////////////////////////////////////////////////////
BOOL CDIP_SystemView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// ������ҳ��Ϊһ��
	pInfo->SetMaxPage(1);

	return DoPreparePrinting(pInfo);
}

void CDIP_SystemView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CDIP_SystemView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CDIP_SystemView diagnostics

#ifdef _DEBUG
void CDIP_SystemView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CDIP_SystemView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CDIP_SystemDoc* CDIP_SystemView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDIP_SystemDoc)));
	return (CDIP_SystemDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDIP_SystemView message handlers
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//																		   //
//								����λͼ����							   //
//																		   //
/////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////
//	�����Ӵ���Ĭ�ϱ���ɫΪm_refColorBKG  //
///////////////////////////////////////////
BOOL CDIP_SystemView::OnEraseBkgnd(CDC* pDC) 
{
	// ��ȡ�ĵ�
	CDIP_SystemDoc* pDoc = GetDocument();

	// ����һ��Brush
	CBrush brush(pDoc->m_refColorBKG);                                              
                                                                                  
	// ������ǰ��Brush
	CBrush* pOldBrush = pDC->SelectObject(&brush);
	
	// ��ȡ�ػ�����
	CRect rectClip;
	pDC->GetClipBox(&rectClip);
	
	if (m_nViewType != ID_VIEW_FIT_WINDOW)
	{
		// �ػ棬������ͼ����
		pDC->PatBlt(m_PaintRect.right, 0, rectClip.right - m_PaintRect.right, rectClip.bottom, PATCOPY);
		pDC->PatBlt(0, m_PaintRect.bottom, rectClip.right, rectClip.bottom - m_PaintRect.bottom, PATCOPY);
	}

	// �ָ���ǰ��Brush
	pDC->SelectObject(pOldBrush);                                                 
	
	return TRUE;
}

///////////////////////////////////////////
//	ʵ���µĵ�ɫ��						 //
///////////////////////////////////////////
void CDIP_SystemView::OnDoRealize(WPARAM wParam, LPARAM lParam)
{
	ASSERT(wParam != NULL);

	// ��ȡ�ĵ�
	CDIP_SystemDoc* pDoc = GetDocument();
	
	// �ж�DIB�Ƿ�Ϊ��
	if (pDoc->DIBImage() == NULL)
	{
		// ֱ�ӷ���
		return;
	}

	// ��ȡMainFrame
	CMainFrame* pAppFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	ASSERT_KINDOF(CMainFrame, pAppFrame);

	CClientDC appDC(pAppFrame);
}

///////////////////////////////////////////
//	���㴰����ͼ��С���Լ������������   //	
///////////////////////////////////////////
void CDIP_SystemView::CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType) 
{
	CScrollView::CalcWindowRect(lpClientRect, nAdjustType);
	ASSERT(GetDocument() != NULL);
	SetScrollSizes(MM_TEXT, GetDocument()->GetDocSize());
}

void CDIP_SystemView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	CScrollView::OnActivateView(bActivate, pActivateView, pDeactiveView);
	if (bActivate)
	{
		ASSERT(pActivateView == this);
		OnDoRealize((WPARAM)m_hWnd, 0);   // same as SendMessage(WM_DOREALIZE);
	}
}

///////////////////////////////////////////
//	���Ƶ�ǰDIB��������					 //	
///////////////////////////////////////////
void CDIP_SystemView::OnEditCopy()
{
	// ��ȡ�ĵ�
	CDIP_SystemDoc* pDoc = GetDocument();

	if (OpenClipboard())
	{
		if (pDoc->DIBImage() == NULL)
		{
			CloseClipboard();
			return;
		}
		CImage* pSrc = pDoc->m_pDIB->MakeCopy();
		CBitmap* pDst = new CBitmap;
		HBITMAP bi = pSrc->Detach();
		pDst->Attach(bi);
		int width = pDoc->m_pDIB->Width();
		int height = pDoc->m_pDIB->Height();

		//��õ�ǰDC
		CDC* pDC = GetDC();

		//��������λͼ
		CBitmap   cBmp;   
		cBmp.CreateCompatibleBitmap(pDC, width, height);   

		//�����뵱ǰDC������ڴ�DC
		CDC   cMemDC;   
		cMemDC.CreateCompatibleDC(pDC); 

		//Ϊ�ڴ�DC����λͼ
		CBitmap* pOldBitmap = cMemDC.SelectObject(&cBmp);

		//���ڴ�DC�����ĵ�����
		OnDraw(&cMemDC);

		AfxGetApp()->m_pMainWnd->OpenClipboard() ;
		EmptyClipboard();
		SetClipboardData(CF_BITMAP, cBmp.GetSafeHandle() ) ;
		CloseClipboard();
		cMemDC.SelectObject(pOldBitmap);
		cMemDC.DeleteDC();
		ReleaseDC(pDC);
		BITMAP bmp = {0};
		cBmp.GetBitmap(&bmp);
		cBmp.Detach();

		delete pSrc;
		delete pDst;
	}
}

void CDIP_SystemView::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
	// �����ǰDIB���󲻿գ����Ʋ˵�����Ч
	pCmdUI->Enable(GetDocument()->DIBImage() != NULL);
}

///////////////////////////////////////////
//	�Ӽ�����ճ��DIB����ǰ��ͼ			 //	
///////////////////////////////////////////
void CDIP_SystemView::OnEditPaste() 
{
	// ��ȡ�ĵ�
	CDIP_SystemDoc* pDoc = GetDocument();
	if (pDoc->DIBImage() != NULL)
		pDoc = pDoc->CreateNewDocument();
	if (OpenClipboard())
	{
		HBITMAP handle = (HBITMAP)GetClipboardData(CF_BITMAP);
		CBitmap* pBitmap = CBitmap::FromHandle(handle);
		CImage* pImage;
		// ȡ��Դ����
		BITMAP bmp = {0};
		pBitmap->GetBitmap(&bmp);
		long Length = bmp.bmHeight * bmp.bmWidthBytes;
		BYTE* pSrc = new BYTE[Length];
		pBitmap->GetBitmapBits(Length, pSrc);
		// ��Ŀ���ַ��������
		pImage = new CImage;
		pImage->Create(bmp.bmWidth, bmp.bmHeight, bmp.bmBitsPixel);
		BYTE* pDst = (BYTE*)pImage->GetBits() + (pImage->GetPitch()*(pImage->GetHeight() - 1));
		memcpy(pDst, pSrc, Length);
		pDoc->m_pDIB->m_UndoStack.Push(pImage);
		pDoc->m_pDIB->SetImage(pImage);
		pDoc->SetDocSize();
		pDoc->GetView(RUNTIME_CLASS(CDIP_SystemView))->m_PaintRect = CRect(0, 0, bmp.bmWidth, bmp.bmHeight);
		Invalidate();
		delete [] pSrc;
		CloseClipboard();
	}
}

void CDIP_SystemView::OnUpdateEditPaste(CCmdUI* pCmdUI) 
{
	// �����ǰ����������DIB����ճ���˵�����Ч
	pCmdUI->Enable(::IsClipboardFormatAvailable(CF_DIB));
}

///////////////////////////////////////////
//	�Ŵ�ǰ��ͼ						 //	
///////////////////////////////////////////
void CDIP_SystemView::OnGeomZoomin() 
{
	// ��ȡ�ĵ�
	CDIP_SystemDoc* pDoc = GetDocument();

	// ��ʼ������ΪFALSE
	pDoc->SetModifiedFlag(FALSE);
 
	if (pDoc->DIBImage() == NULL) return;
	pDoc->m_pDIB->PushCopy();

	// ����ZoomDIB()������ȡ�Ŵ���DIB
	CGeoTrans clsGeoTrans(pDoc->m_pDIB);
	clsGeoTrans.ZoomDIB(pDoc->m_fZoomRatio);

	// ��������
	pDoc->SetModifiedFlag(TRUE);

	// �������ù�����ͼ��С
	SetScrollSizes(MM_TEXT, pDoc->GetDocSize());

	// ������ͼ
	pDoc->UpdateAllViews(NULL);
}

///////////////////////////////////////////
//	��С��ǰ��ͼ						 //	
///////////////////////////////////////////
void CDIP_SystemView::OnGeomZoomout() 
{
	// ��ȡ�ĵ�
	CDIP_SystemDoc* pDoc = GetDocument();

	// ��ʼ������ΪFALSE
	pDoc->SetModifiedFlag(FALSE);
 
	if (pDoc->DIBImage() == NULL) return;
	pDoc->m_pDIB->PushCopy();

	// ����ZoomDIB()������ȡ��С���DIB
	CGeoTrans clsGeoTrans(pDoc->m_pDIB);
	clsGeoTrans.ZoomDIB(1.0f / pDoc->m_fZoomRatio);

	// ��������
	pDoc->SetModifiedFlag(TRUE);

	// �������ù�����ͼ��С
	SetScrollSizes(MM_TEXT, pDoc->GetDocSize());

	// ������ͼ
	pDoc->UpdateAllViews(NULL);
}

/////////////////////////////////////////////////////////////////////////////
//																		   //
//								ͼ���������							   //
//																		   //
/////////////////////////////////////////////////////////////////////////////

///////////////////////////[�Ҷȴ�����]////////////////////////////////////////////

///////////////////////////////////////////
//	����ɫλͼת��Ϊ�Ҷ�ͼ��			 //	
///////////////////////////////////////////
void CDIP_SystemView::OnTogray() 
{
	// ��ȡ�ĵ�
	CDIP_SystemDoc* pDoc = GetDocument();

	// ����ǰһ��״̬
	if (pDoc->DIBImage() == NULL) return;
	if (pDoc->m_pDIB->Channel() == 1) return;
	pDoc->m_pDIB->PushCopy();

	// ���лҶ�ת��
	CGrayProc clsGrayProc(pDoc->m_pDIB);
	clsGrayProc.ToGray();
	
	// �ػ���ͼ
	Invalidate();

	// ��������
	pDoc->SetModifiedFlag(TRUE);
}

///////////////////////////////////////////
//	��ͼ��ʵʩ��ɫ�任����				 //	
///////////////////////////////////////////
void CDIP_SystemView::OnPointInvert() 
{
	// ��ȡ�ĵ�
	CDIP_SystemDoc* pDoc = GetDocument();

	if (pDoc->DIBImage() == NULL) return;
	pDoc->m_pDIB->PushCopy();

	// ���з�ɫ����
	CGrayProc clsGrayProc(pDoc->m_pDIB);
	clsGrayProc.PointInvert();
	
	// �ػ���ͼ
	Invalidate();

	// ��������
	pDoc->SetModifiedFlag(TRUE);
}

///////////////////////////////////////////
//	��ͼ����лҶȷֲ����⻯����		 //	
///////////////////////////////////////////
void CDIP_SystemView::OnPointEqua() 
{
	// ��ȡ�ĵ�
	CDIP_SystemDoc* pDoc = GetDocument();

	if (pDoc->DIBImage() == NULL) return;
	pDoc->m_pDIB->PushCopy();

	// ���лҶȷֲ����⻯����
	CGrayProc clsGrayProc(pDoc->m_pDIB);
	clsGrayProc.PointEqua();
	
	// �ػ���ͼ
	Invalidate();

	// ��������
	pDoc->SetModifiedFlag(TRUE);
}

///////////////////////////////////////////
//	�鿴ͼ��ĻҶ�ֱ��ͼ				 //	
///////////////////////////////////////////
void CDIP_SystemView::OnViewYIntensity() 
{
	
	int i;

	// ��ȡ�ĵ�
	CDIP_SystemDoc* pDoc = GetDocument();
	if (pDoc->DIBImage() == NULL) return;

	// ����ɫ�����ĻҶȷֲ��ܶ�
	float fPs_Y[256];

	// ����Ҷȷֲ��ܶ�
	CGrayProc clsGrayProc(pDoc->m_pDIB);
	clsGrayProc.GetIntensity(fPs_Y);
	
	CDlgIntensity dlg;

	// ���ݻҶȷֲ��ܶ����ݸ������
	for (i = 0; i <256; i++)
		dlg.m_fIntensity[i] = fPs_Y[i];

	// ��ʾ���
	dlg.DoModal();
}

void CDIP_SystemView::OnViewRIntensity() 
{
	
	int i;

	// ��ȡ�ĵ�
	CDIP_SystemDoc* pDoc = GetDocument();
	if (pDoc->DIBImage() == NULL) return;

	// ����ɫ�����ĻҶȷֲ��ܶ�
	float fPs_R[256];
	float fPs_G[256];
	float fPs_B[256];

	// ����Ҷȷֲ��ܶ�
	CGrayProc clsGrayProc(pDoc->m_pDIB);
	clsGrayProc.GetIntensity(fPs_R, fPs_G, fPs_B);
	
	CDlgIntensity dlg;

	// ���ݻҶȷֲ��ܶ����ݸ������
	for (i = 0; i <256; i++)
		dlg.m_fIntensity[i] = fPs_R[i];

	// ��ʾ���
	dlg.DoModal();
}

void CDIP_SystemView::OnViewGIntensity() 
{
	
	int i;

	// ��ȡ�ĵ�
	CDIP_SystemDoc* pDoc = GetDocument();
	if (pDoc->DIBImage() == NULL) return;

	// ����ɫ�����ĻҶȷֲ��ܶ�
	float fPs_R[256];
	float fPs_G[256];
	float fPs_B[256];

	// ����Ҷȷֲ��ܶ�
	CGrayProc clsGrayProc(pDoc->m_pDIB);
	clsGrayProc.GetIntensity(fPs_R, fPs_G, fPs_B);
	
	CDlgIntensity dlg;

	// ���ݻҶȷֲ��ܶ����ݸ������
	for (i = 0; i <256; i++)
		dlg.m_fIntensity[i] = fPs_G[i];

	// ��ʾ���
	dlg.DoModal();
}

void CDIP_SystemView::OnViewBIntensity() 
{
	
	int i;

	// ��ȡ�ĵ�
	CDIP_SystemDoc* pDoc = GetDocument();
	if (pDoc->DIBImage() == NULL) return;

	// ����ɫ�����ĻҶȷֲ��ܶ�
	float fPs_R[256];
	float fPs_G[256];
	float fPs_B[256];

	// ����Ҷȷֲ��ܶ�
	CGrayProc clsGrayProc(pDoc->m_pDIB);
	clsGrayProc.GetIntensity(fPs_R, fPs_G, fPs_B);
	
	CDlgIntensity dlg;

	// ���ݻҶȷֲ��ܶ����ݸ������
	for (i = 0; i <256; i++)
		dlg.m_fIntensity[i] = fPs_B[i];

	// ��ʾ���
	dlg.DoModal();
}

///////////////////////////////////////////
//	�Ҷ����߱任						 //	
///////////////////////////////////////////
void CDIP_SystemView::OnPointStre() 
{
	
	int i;

	// ����ɫ�����ĻҶȷֲ��ܶ�
	float fPs_Y[256];

	// ��ȡ�ĵ�
	CDIP_SystemDoc* pDoc = GetDocument();

	if (pDoc->DIBImage() == NULL) return;
	pDoc->m_pDIB->PushCopy();

	// ����Ҷȷֲ��ܶ�
	CGrayProc clsGrayProc(pDoc->m_pDIB);
	clsGrayProc.GetIntensity(fPs_Y);
	
	// �����Ի���
	CDlgPointStre dlg;
	dlg.m_pDoc = pDoc;

	// ���ݻҶȷֲ��ܶ����ݸ������
	for (i = 0; i <256; i++)
		dlg.m_fIntensity[i] = fPs_Y[i];

	// ��ʾ�Ի������û����лҶ����߱任
	dlg.DoModal();
}

///////////////////////////////////////////
//	�Ҷȴ��ڱ任						 //	
///////////////////////////////////////////
void CDIP_SystemView::OnPointWind() 
{
	
	int i;

	// ����ɫ�����ĻҶȷֲ��ܶ�
	float fPs_Y[256];

	// ��ȡ�ĵ�
	CDIP_SystemDoc* pDoc = GetDocument();

	if (pDoc->DIBImage() == NULL) return;
	pDoc->m_pDIB->PushCopy();

	// ����Ҷȷֲ��ܶ�
	CGrayProc clsGrayProc(pDoc->m_pDIB);
	clsGrayProc.GetIntensity(fPs_Y);
	
	// �����Ի���
	CDlgPointWin dlg;
	dlg.m_pDoc = pDoc;

	// ���ݻҶȷֲ��ܶ����ݸ������
	for (i = 0; i <256; i++)
		dlg.m_fIntensity[i] = fPs_Y[i];

	// ��ʾ�Ի������û����лҶ����߱任
	dlg.DoModal();
}

///////////////////////////////////////////
//	�Ҷȴ�ͨ�任						 //	
///////////////////////////////////////////
void CDIP_SystemView::OnPointDt() 
{
	int i;

	// ����ɫ�����ĻҶȷֲ��ܶ�
	float fPs_Y[256];

	// ��ȡ�ĵ�
	CDIP_SystemDoc* pDoc = GetDocument();

	if (pDoc->DIBImage() == NULL) return;
	pDoc->m_pDIB->PushCopy();

	// ����Ҷȷֲ��ܶ�
	CGrayProc clsGrayProc(pDoc->m_pDIB);
	clsGrayProc.GetIntensity(fPs_Y);
	
	// �����Ի���
	CDlgPointDT dlg;
	dlg.m_pDoc = pDoc;

	// ���ݻҶȷֲ��ܶ����ݸ������
	for (i = 0; i <256; i++)
		dlg.m_fIntensity[i] = fPs_Y[i];

	// ��ʾ�Ի������û����лҶ����߱任
	dlg.DoModal();
}

///////////////////////////////////////////
//	�Ҷȴ���任						 //	
///////////////////////////////////////////
void CDIP_SystemView::OnPointDz() 
{
	int i;

	// ����ɫ�����ĻҶȷֲ��ܶ�
	float fPs_Y[256];

	// ��ȡ�ĵ�
	CDIP_SystemDoc* pDoc = GetDocument();

	if (pDoc->DIBImage() == NULL) return;
	pDoc->m_pDIB->PushCopy();

	// ����Ҷȷֲ��ܶ�
	CGrayProc clsGrayProc(pDoc->m_pDIB);
	clsGrayProc.GetIntensity(fPs_Y);
	
	// �����Ի���
	CDlgPointDZ dlg;
	dlg.m_pDoc = pDoc;

	// ���ݻҶȷֲ��ܶ����ݸ������
	for (i = 0; i <256; i++)
		dlg.m_fIntensity[i] = fPs_Y[i];

	// ��ʾ�Ի������û����лҶ����߱任
	dlg.DoModal();
}

///////////////////////////////////////////
//	�涨�Ҷ�ֱ��ͼ�ֲ�					 //	
///////////////////////////////////////////
void CDIP_SystemView::OnSetIntensity() 
{
	AfxMessageBox(L"���ܴ���ӣ�");
}

///////////////////////////////////////////
//	��ӳ��������ֱ��ͼ�涨��			 //	
///////////////////////////////////////////
void CDIP_SystemView::OnPointSml() 
{

//--------���Դ��� ��ʼ------------------------------------------
	int nu[64];
	float pu[64];

	float a=1.0f/(32.0f*63.0f);
	for(int i=0;i<64;i++)
	{
		nu[i]=i*4;
		pu[i]=a*i;
	}
//--------���Դ��� ����------------------------------------------

	// ��ȡ�ĵ�
	CDIP_SystemDoc* pDoc = GetDocument();

	if (pDoc->DIBImage() == NULL) return;
	pDoc->m_pDIB->PushCopy();

	// ����Ҷȷֲ��ܶ�
	CGrayProc clsGrayProc(pDoc->m_pDIB);
	clsGrayProc.PointSML(64,nu, pu);

	// �ػ���ͼ
	Invalidate();

	// ��������
	pDoc->SetModifiedFlag(TRUE);
}

///////////////////////////////////////////
//	��ӳ��������ֱ��ͼ�涨��			 //	
///////////////////////////////////////////
void CDIP_SystemView::OnPointGml() 
{

//--------���Դ��� ��ʼ------------------------------------------
	int nu[64];
	float pu[64];

	float a=1.0f/(32.0f*63.0f);
	for(int i=0;i<64;i++)
	{
		nu[i]=i*4;
		pu[i]=a*i;
	}
//--------���Դ��� ����------------------------------------------

	// ��ȡ�ĵ�
	CDIP_SystemDoc* pDoc = GetDocument();

	if (pDoc->DIBImage() == NULL) return;
	pDoc->m_pDIB->PushCopy();

	// ����Ҷȷֲ��ܶ�
	CGrayProc clsGrayProc(pDoc->m_pDIB);
	clsGrayProc.PointGML(64,nu, pu);

	// �ػ���ͼ
	Invalidate();

	// ��������
	pDoc->SetModifiedFlag(TRUE);
}

///////////////////////////////////////////
//	�ԻҶȽ��ж�̬��Χѹ��				 //	
///////////////////////////////////////////
void CDIP_SystemView::OnDynamicCompress() 
{
	// �����Ի���
	CDynamicCompressParam dlg;
	
	// ��ȡ�ĵ�
	CDIP_SystemDoc* pDoc = GetDocument();

	if (pDoc->DIBImage() == NULL) return;
	pDoc->m_pDIB->PushCopy();

	dlg.m_pDoc = pDoc;

	// ��ʾ�Ի������û����г߶ȱ��������趨
	dlg.DoModal();
}

///////////////////////////////////////////
//	�ԻҶȽ�����������					 //	
///////////////////////////////////////////
void CDIP_SystemView::OnCutWave() 
{
	// ��ȡ�ĵ�
	CDIP_SystemDoc* pDoc = GetDocument();

	if (pDoc->DIBImage() == NULL) return;
	pDoc->m_pDIB->PushCopy();

	// ��ͼ����а�����˹��ͨ�˲�
	CGrayProc clsGrayProc(pDoc->m_pDIB);
	clsGrayProc.CutWave();

	// �ػ���ͼ
	Invalidate();

	// ��������
	pDoc->SetModifiedFlag(TRUE);
}

///////////////////////////[ͼ��ָ��]////////////////////////////////////////////

///////////////////////////////////////////
//	��ͼ��ʵʩ������������				 //	
///////////////////////////////////////////
void CDIP_SystemView::OnRgnGrow() 
{
	// ��ȡ�ĵ�
	CDIP_SystemDoc* pDoc = GetDocument();

	if (pDoc->DIBImage() == NULL) return;
	pDoc->m_pDIB->PushCopy();

	// ��������
	if (pDoc->m_ptPoint_I == CPoint(0, 0))
	{
		MessageBox(L"����˫��ͼ�����������ӵ�һ��", L"ϵͳ��ʾ" , MB_ICONINFORMATION | MB_OK);
		return;
	}

	if (pDoc->m_ptPoint_II == CPoint(0, 0))
	{
		MessageBox(L"����˫��ͼ�����������ӵ����", L"ϵͳ��ʾ" , MB_ICONINFORMATION | MB_OK);
		return;
	}

	// ������������������ͼ����зָ�
	CSplitProc clsSplitProc(pDoc->m_pDIB);
	clsSplitProc.RgnGrow(m_clrSeed_I, m_clrSeed_II);
	
	// ������ӵ�
	pDoc->m_ptPoint_I = CPoint(0, 0);
	pDoc->m_ptPoint_II = CPoint(0, 0);

	// �ػ���ͼ
	Invalidate();

	// ��������
	pDoc->SetModifiedFlag(TRUE);
}

///////////////////////////////////////////
//	��ͼ�����������ȡ					 //		
///////////////////////////////////////////
void CDIP_SystemView::OnContourdib() 
{
	// ��ȡ�ĵ�
	CDIP_SystemDoc* pDoc = GetDocument();

	if (pDoc->DIBImage() == NULL) return;
	pDoc->m_pDIB->PushCopy();

	// ��������
	if (pDoc->m_ptPoint_I == CPoint(0, 0))
	{
		MessageBox(L"����˫��ͼ�����������ӵ�һ��", L"ϵͳ��ʾ" , MB_ICONINFORMATION | MB_OK);
		return;
	}

	if (pDoc->m_ptPoint_II == CPoint(0, 0))
	{
		MessageBox(L"����˫��ͼ�����������ӵ����", L"ϵͳ��ʾ" , MB_ICONINFORMATION | MB_OK);
		return;
	}
	
	// ����������ȡ 
	CSplitProc clsSplitProc(pDoc->m_pDIB);
	clsSplitProc.ContourDIB(m_clrSeed_I, m_clrSeed_II);
	
	// ������ӵ�
	pDoc->m_ptPoint_I = CPoint(0, 0);
	pDoc->m_ptPoint_II = CPoint(0, 0);

	// �ػ���ͼ
	Invalidate();

	// ��������
	pDoc->SetModifiedFlag(TRUE);
}

///////////////////////////////////////////
//	��ͼ����б߽����					 //		
///////////////////////////////////////////
void CDIP_SystemView::OnTracedib() 
{
	// ������������RGBֵ
	COLORREF clrSeed;

	// ��ȡ�ĵ�
	CDIP_SystemDoc* pDoc = GetDocument();

	if (pDoc->DIBImage() == NULL) return;
	pDoc->m_pDIB->PushCopy();

	// ��������
	if (pDoc->m_ptPoint_II == CPoint(0, 0))
	{
		MessageBox(L"����˫��Ŀ�����������ӵ㣡", L"ϵͳ��ʾ" , MB_ICONINFORMATION | MB_OK);
		return;
	}

	// ��ȡĿ�����ɫֵ
	if (pDoc->m_ptPoint_I == CPoint(0, 0))
		clrSeed = m_clrSeed_II;
	else
		clrSeed = m_clrSeed_I;

	// ����������ȡ 
	CSplitProc clsSplitProc(pDoc->m_pDIB);
	clsSplitProc.TraceDIB(clrSeed);

	// ������ӵ�
	pDoc->m_ptPoint_I = CPoint(0, 0);
	pDoc->m_ptPoint_II = CPoint(0, 0);

	// �ػ���ͼ
	Invalidate();

	// ��������
	pDoc->SetModifiedFlag(TRUE);
}

///////////////////////////////////////////
//	��ͼ��������������ӵ��ݶ��˲�		 //	
///////////////////////////////////////////
void CDIP_SystemView::OnSobel() 
{
	// ��ȡ�ĵ�
	CDIP_SystemDoc* pDoc = GetDocument();

	if (pDoc->DIBImage() == NULL) return;
	pDoc->m_pDIB->PushCopy();

	// ���������������ݶ��˲�
	CSplitProc clsSplitProc(pDoc->m_pDIB);
	clsSplitProc.Sobel();
	
	// �ػ���ͼ
	Invalidate();

	// ��������
	pDoc->SetModifiedFlag(TRUE);
}

///////////////////////////////////////////
//	��ͼ���������ά�����ӵ��ݶ��˲�	 //	
///////////////////////////////////////////
void CDIP_SystemView::OnPrewitt() 
{
	// ��ȡ�ĵ�
	CDIP_SystemDoc* pDoc = GetDocument();

	if (pDoc->DIBImage() == NULL) return;
	pDoc->m_pDIB->PushCopy();

	// ���������������ݶ��˲�
	CSplitProc clsSplitProc(pDoc->m_pDIB);
	clsSplitProc.Prewitt();
	
	// �ػ���ͼ
	Invalidate();

	// ��������
	pDoc->SetModifiedFlag(TRUE);
}

///////////////////////////////////////////
//	��ͼ������޲��ؽ������ӵ��ݶ��˲�	 //	
///////////////////////////////////////////
void CDIP_SystemView::OnRoberts() 
{
	// ��ȡ�ĵ�
	CDIP_SystemDoc* pDoc = GetDocument();

	if (pDoc->DIBImage() == NULL) return;
	pDoc->m_pDIB->PushCopy();

	// ���������������ݶ��˲�
	CSplitProc clsSplitProc(pDoc->m_pDIB);
	clsSplitProc.Roberts();
	
	// �ػ���ͼ
	Invalidate();

	// ��������
	pDoc->SetModifiedFlag(TRUE);
}

///////////////////////////////////////////
//	��ͼ�����Kirsch���ӵ��ݶ��˲�		 //	
///////////////////////////////////////////
void CDIP_SystemView::OnKrisch() 
{
	// ��ȡ�ĵ�
	CDIP_SystemDoc* pDoc = GetDocument();

	if (pDoc->DIBImage() == NULL) return;
	pDoc->m_pDIB->PushCopy();

	// ���������������ݶ��˲�
	CSplitProc clsSplitProc(pDoc->m_pDIB);
	clsSplitProc.Kirsch();
	
	// �ػ���ͼ
	Invalidate();

	// ��������
	pDoc->SetModifiedFlag(TRUE);
}

///////////////////////////////////////////
//	��ͼ�����������˹���ӵ��˲�		 //	
///////////////////////////////////////////
void CDIP_SystemView::OnLaplacian() 
{
	// ��ȡ�ĵ�
	CDIP_SystemDoc* pDoc = GetDocument();

	if (pDoc->DIBImage() == NULL) return;
	pDoc->m_pDIB->PushCopy();

	// ���������������ݶ��˲�
	CSplitProc clsSplitProc(pDoc->m_pDIB);
	clsSplitProc.Laplacian();
	
	// �ػ���ͼ
	Invalidate();

	// ��������
	pDoc->SetModifiedFlag(TRUE);
}

///////////////////////////////////////////
//	��ͼ����и�˹����������˹���ӵ��˲� //	
///////////////////////////////////////////
void CDIP_SystemView::OnGuassLaplacian() 
{
	// ��ȡ�ĵ�
	CDIP_SystemDoc* pDoc = GetDocument();

	if (pDoc->DIBImage() == NULL) return;
	pDoc->m_pDIB->PushCopy();

	// ���������������ݶ��˲�
	CSplitProc clsSplitProc(pDoc->m_pDIB);
	clsSplitProc.GuassLaplacian();
	
	// �ػ���ͼ
	Invalidate();

	// ��������
	pDoc->SetModifiedFlag(TRUE);
}

///////////////////////////////////////////
//	��ͼ����й���任�Լ��Բ��		 //	
///////////////////////////////////////////
void CDIP_SystemView::OnHough() 
{
	// ��ȡ�ĵ�
	CDIP_SystemDoc* pDoc = GetDocument();

	if (pDoc->DIBImage() == NULL) return;
	pDoc->m_pDIB->PushCopy();

	// ���������������ݶ��˲�
	CSplitProc clsSplitProc(pDoc->m_pDIB);
	clsSplitProc.Hough();
	
	// �ػ���ͼ
	Invalidate();

	// ��������
	pDoc->SetModifiedFlag(TRUE);
}

///////////////////////////////////////////
//	��ͼ����з�ֵ�����ķ�ֵ���ָ��	 //	
///////////////////////////////////////////
void CDIP_SystemView::OnIterateThreshold() 
{
	// ����ɫ�����ĻҶȷֲ��ܶ�
	int nNY[256];
	memset(nNY, 0, sizeof(nNY));

	// ��ȡ�ĵ�
	CDIP_SystemDoc* pDoc = GetDocument();

	if (pDoc->DIBImage() == NULL) return;
	pDoc->m_pDIB->PushCopy();

	// ����Ҷȷֲ��ܶ�
	CGrayProc clsGrayProc(pDoc->m_pDIB);
	clsGrayProc.GetIntensity(nNY);

	// ���ݷ�ֵ��ͼ����з�ֵ���ָ��
	CSplitProc clsSplitProc(pDoc->m_pDIB);
	clsSplitProc.Threshold(nNY);

	// �ػ���ͼ
	Invalidate();

	// ��������
	pDoc->SetModifiedFlag(TRUE);
}

///////////////////////////[ͼ������˲�����]////////////////////////////////////////////

///////////////////////////////////////////
//	��ͼ��ʵʩ���񻯴��������֣�		 //	
///////////////////////////////////////////
void CDIP_SystemView::OnEmboss_1() 
{
	CDIP_SystemDoc* pDoc = GetDocument();
	if (pDoc->DIBImage() == NULL) return;
	pDoc->m_pDIB->PushCopy();
	DoEmboss(EMBOSS_1);
}

void CDIP_SystemView::OnEmboss_2() 
{
	CDIP_SystemDoc* pDoc = GetDocument();
	if (pDoc->DIBImage() == NULL) return;
	pDoc->m_pDIB->PushCopy();
	DoEmboss(EMBOSS_2);
}

void CDIP_SystemView::OnEmboss_3() 
{
	CDIP_SystemDoc* pDoc = GetDocument();
	if (pDoc->DIBImage() == NULL) return;
	pDoc->m_pDIB->PushCopy();
	DoEmboss(EMBOSS_3);
}

void CDIP_SystemView::OnEmboss_4() 
{
	CDIP_SystemDoc* pDoc = GetDocument();
	if (pDoc->DIBImage() == NULL) return;
	pDoc->m_pDIB->PushCopy();
	DoEmboss(EMBOSS_4);
}

void CDIP_SystemView::OnEmboss_5() 
{
	CDIP_SystemDoc* pDoc = GetDocument();
	if (pDoc->DIBImage() == NULL) return;
	pDoc->m_pDIB->PushCopy();
	DoEmboss(EMBOSS_5);
}

void CDIP_SystemView::OnEmboss_6() 
{
	CDIP_SystemDoc* pDoc = GetDocument();
	if (pDoc->DIBImage() == NULL) return;
	pDoc->m_pDIB->PushCopy();
	DoEmboss(EMBOSS_6);
}

void CDIP_SystemView::OnEmboss_7() 
{
	CDIP_SystemDoc* pDoc = GetDocument();
	if (pDoc->DIBImage() == NULL) return;
	pDoc->m_pDIB->PushCopy();
	DoEmboss(EMBOSS_7);
}

void CDIP_SystemView::OnEmboss_8() 
{
	CDIP_SystemDoc* pDoc = GetDocument();
	if (pDoc->DIBImage() == NULL) return;
	pDoc->m_pDIB->PushCopy();
	DoEmboss(EMBOSS_8);
}

void CDIP_SystemView::DoEmboss(int nEmbossMode)
{
	// ��ȡ�ĵ�
	CDIP_SystemDoc* pDoc = GetDocument();

	if (pDoc->DIBImage() == NULL) return;
	pDoc->m_pDIB->PushCopy();

	// ���и��񻯴���
	CSpaceFilter clsSpaceFilter(pDoc->m_pDIB);
	clsSpaceFilter.MakeEmboss(nEmbossMode);

	// ����DIB��С�͵�ɫ��
	pDoc->InitDIBData();

	// ��������
	pDoc->SetModifiedFlag(TRUE);

	// �������ù�����ͼ��С
	SetScrollSizes(MM_TEXT, pDoc->GetDocSize());

	// ������ͼ
	pDoc->UpdateAllViews(NULL);
}

///////////////////////////////////////////
//	��ͼ��ʵʩ����ƽ���˲���G1��		 //	
///////////////////////////////////////////
void CDIP_SystemView::OnLineSmoothFilter() 
{
	// ��ȡ�ĵ�
	CDIP_SystemDoc* pDoc = GetDocument();

	if (pDoc->DIBImage() == NULL) return;
	pDoc->m_pDIB->PushCopy();

	// ��������ƽ���˲�����
	CSpaceFilter clsSpaceFilter(pDoc->m_pDIB);
	clsSpaceFilter.StencilFilter(&Line_Filter[0]);

	// ��������
	pDoc->SetModifiedFlag(TRUE);

	// ������ͼ
	Invalidate();
}

void CDIP_SystemView::OnLineSmoothFilter2() 
{
	// ��ȡ�ĵ�
	CDIP_SystemDoc* pDoc = GetDocument();

	if (pDoc->DIBImage() == NULL) return;
	pDoc->m_pDIB->PushCopy();

	// ��������ƽ���˲�����
	CSpaceFilter clsSpaceFilter(pDoc->m_pDIB);
	clsSpaceFilter.StencilFilter(&Line_Filter[1]);

	// ��������
	pDoc->SetModifiedFlag(TRUE);

	// ������ͼ
	Invalidate();
}

void CDIP_SystemView::OnLineSmoothFilter3() 
{
	// ��ȡ�ĵ�
	CDIP_SystemDoc* pDoc = GetDocument();

	if (pDoc->DIBImage() == NULL) return;
	pDoc->m_pDIB->PushCopy();

	// ��������ƽ���˲�����
	CSpaceFilter clsSpaceFilter(pDoc->m_pDIB);
	clsSpaceFilter.StencilFilter(&Line_Filter[2]);

	// ��������
	pDoc->SetModifiedFlag(TRUE);

	// ������ͼ
	Invalidate();
}

///////////////////////////////////////////
//	��ͼ��ʵʩ������ƽ���˲���G2��		 //	
///////////////////////////////////////////
void CDIP_SystemView::OnMedianFilter() 
{
	// ��ȡ�ĵ�
	CDIP_SystemDoc* pDoc = GetDocument();

	if (pDoc->DIBImage() == NULL) return;
	pDoc->m_pDIB->PushCopy();

	// ��������ƽ���˲�����
	CSpaceFilter clsSpaceFilter(pDoc->m_pDIB);
	clsSpaceFilter.MedianFilter();

	// ��������
	pDoc->SetModifiedFlag(TRUE);

	// ������ͼ
	Invalidate();
}

///////////////////////////////////////////
//	��ͼ��ʵʩ�������˲���G3��		 //	
///////////////////////////////////////////
void CDIP_SystemView::OnLineQuickFilter() 
{
	// ��ȡ�ĵ�
	CDIP_SystemDoc* pDoc = GetDocument();

	if (pDoc->DIBImage() == NULL) return;
	pDoc->m_pDIB->PushCopy();

	// ��������ƽ���˲�����
	CSpaceFilter clsSpaceFilter(pDoc->m_pDIB);
	clsSpaceFilter.StencilFilter(&Line_Filter[3]);

	// ��������
	pDoc->SetModifiedFlag(TRUE);

	// ������ͼ
	Invalidate();
}

///////////////////////////////////////////
//	��ͼ��ʵʩ�����˲�					 //	
///////////////////////////////////////////
void CDIP_SystemView::OnOverrunFilter() 
{
	// ��ȡ�ĵ�
	CDIP_SystemDoc* pDoc = GetDocument();

	if (pDoc->DIBImage() == NULL) return;
	pDoc->m_pDIB->PushCopy();

	// ��������ƽ���˲�����
	CSpaceFilter clsSpaceFilter(pDoc->m_pDIB);
	clsSpaceFilter.OverRun(&OverRun_Filter, 10);

	// ��������
	pDoc->SetModifiedFlag(TRUE);

	// ������ͼ
	Invalidate();
}

///////////////////////////[�����任����]////////////////////////////////////////////

///////////////////////////////////////////
//	��ͼ��ʵʩ����Ҷ�任����			 //	
///////////////////////////////////////////
void CDIP_SystemView::OnFreqFour() 
{
	// ��ȡ�ĵ�
	CDIP_SystemDoc* pDoc = GetDocument();

	if (pDoc->DIBImage() == NULL) return;
	pDoc->m_pDIB->PushCopy();
	
	CFreqCalculate clsFreqCalculate(pDoc->m_pDIB);

	// ����DIBFourier()�������и���Ҷ�任
	if (clsFreqCalculate.DIBFourier())
	{
		// ��ʶΪƵ��ͼ
		pDoc->m_bIsFrequency = TRUE;

		// ��������
		pDoc->SetModifiedFlag(TRUE);
		
		// ������ͼ
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		
		MessageBox(L"�����ڴ�ʧ�ܣ�", L"ϵͳ��ʾ" , MB_ICONINFORMATION | MB_OK);
	}
}

///////////////////////////////////////////
//	��ͼ��ʵʩ��ɢ���ұ任����			 //	
///////////////////////////////////////////
void CDIP_SystemView::OnFreqDct() 
{
	// ��ȡ�ĵ�
	CDIP_SystemDoc* pDoc = GetDocument();

	if (pDoc->DIBImage() == NULL) return;
	pDoc->m_pDIB->PushCopy();
	
	CFreqCalculate clsFreqCalculate(pDoc->m_pDIB);

	// ����DIBFourier()�������и���Ҷ�任
	if (clsFreqCalculate.DIBDCT())
	{
		// ��������
		pDoc->SetModifiedFlag(TRUE);
		
		// ������ͼ
		pDoc->UpdateAllViews(NULL);	
	}
	else
	{
		
		MessageBox(L"�����ڴ�ʧ�ܣ�", L"ϵͳ��ʾ" , MB_ICONINFORMATION | MB_OK);
	}
}

///////////////////////////////////////////
//	��ͼ��ʵʩ�ֶ�ʲ����������任����	 //	
///////////////////////////////////////////
void CDIP_SystemView::OnFreqWalh() 
{
	// ��ȡ�ĵ�
	CDIP_SystemDoc* pDoc = GetDocument();

	if (pDoc->DIBImage() == NULL) return;
	pDoc->m_pDIB->PushCopy();
	
	CFreqCalculate clsFreqCalculate(pDoc->m_pDIB);

	// ����DIBFourier()�������и���Ҷ�任
	if (clsFreqCalculate.DIBWalsh())
	{
		// ��������
		pDoc->SetModifiedFlag(TRUE);
		
		// ������ͼ
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		
		MessageBox(L"�����ڴ�ʧ�ܣ�", L"ϵͳ��ʾ" , MB_ICONINFORMATION | MB_OK);
	}
}

///////////////////////////[ͼ��Ƶ���˲�����]////////////////////////////////////////////

///////////////////////////////////////////
//	��ͼ��ʵʩ������˹��ͨ�˲�			 //	
///////////////////////////////////////////
void CDIP_SystemView::OnButterworthL() 
{
	// ��ȡ�ĵ�
	CDIP_SystemDoc* pDoc = GetDocument();

	if (pDoc->DIBImage() == NULL) return;
	pDoc->m_pDIB->PushCopy();

	// �ɶԻ��������ֹƵ��
	CDLGSetFreq dlg;
	dlg.m_fValue = 250.0f;
	if(dlg.DoModal() == IDOK)
	{
		// ��������
		UpdateData(TRUE);

		// ��ͼ����а�����˹��ͨ�˲�
		CFreqFilter clsFreqFilter(pDoc->m_pDIB);
		clsFreqFilter.ButterworthL(dlg.m_fValue);

		// �ػ���ͼ
		Invalidate();

		// ��������
		pDoc->SetModifiedFlag(TRUE);
	}
}

///////////////////////////////////////////
//	��ͼ��ʵʩ������˹��ͨ�˲�			 //	
///////////////////////////////////////////
void CDIP_SystemView::OnButterworthH() 
{
	// ��ȡ�ĵ�
	CDIP_SystemDoc* pDoc = GetDocument();

	if (pDoc->DIBImage() == NULL) return;
	pDoc->m_pDIB->PushCopy();

	// �ɶԻ��������ֹƵ��
	CDLGSetFreq dlg;
	dlg.m_fValue = 5.0f;
	if(dlg.DoModal() == IDOK)
	{
		// ��������
		UpdateData(TRUE);

		// ��ͼ����а�����˹��ͨ�˲�
		CFreqFilter clsFreqFilter(pDoc->m_pDIB);
		clsFreqFilter.ButterworthH(dlg.m_fValue);

		// �ػ���ͼ
		Invalidate();

		// ��������
		pDoc->SetModifiedFlag(TRUE);
	}
}

///////////////////////////////////////////
//	��ͼ��ʵʩ����ʽ�����˲�			 //	
///////////////////////////////////////////
void CDIP_SystemView::OnMutualFilter() 
{
	// ��ȡ�ĵ�
	CDIP_SystemDoc* pDoc = GetDocument();

	if (pDoc->DIBImage() == NULL) return;
	pDoc->m_pDIB->PushCopy();

    if (pDoc->m_bIsFrequency == FALSE)
	{
		
		MessageBox(L"������Ƶ��ͼָ���˲�����!", L"ϵͳ��ʾ" , MB_ICONINFORMATION | MB_OK);
		return;
	}
	
	pDoc->SetModifiedFlag(FALSE);

	// ��ͼ����н���ʽ�����˲�
	CFreqFilter clsFreqFilter(pDoc->m_pDIB);
	clsFreqFilter.MutualFilter(pDoc->m_rect, pDoc->m_nRgnNum);

	pDoc->m_nRgnNum = 0;

	// �ػ���ͼ
	Invalidate();

	// ��������
	pDoc->SetModifiedFlag(TRUE);
}

///////////////////////////////////////////
//	��ͼ��ʵʩ������˹��ͨ�˲������˲�   //	
///////////////////////////////////////////
void CDIP_SystemView::OnRetrorseFilter() 
{
	// ��ȡ�ĵ�
	CDIP_SystemDoc* pDoc = GetDocument();

	if (pDoc->DIBImage() == NULL) return;
	pDoc->m_pDIB->PushCopy();

	// �ɶԻ��������ֹƵ��
	CDLGSetFreq dlg;
	dlg.m_fValue = 250.0f;
	if(dlg.DoModal() == IDOK)
	{
		// ��������
		UpdateData(TRUE);

		// ��ͼ����а�����˹��ͨ�˲�
		CFreqFilter clsFreqFilter(pDoc->m_pDIB);
		clsFreqFilter.RetrorseFilter(dlg.m_fValue);

		// �ػ���ͼ
		Invalidate();

		// ��������
		pDoc->SetModifiedFlag(TRUE);
	}
}

///////////////////////////////////////////
//	��ͼ��ʵʩά���˲�					 //	
///////////////////////////////////////////
void CDIP_SystemView::OnWienerFilter() 
{
	// ��ȡ�ĵ�
	CDIP_SystemDoc* pDoc = GetDocument();

	if (pDoc->DIBImage() == NULL) return;
	pDoc->m_pDIB->PushCopy();

	// �ɶԻ��������ֹƵ��
	CDLGSetFreq dlg;
	dlg.m_fValue = 250.0f;
	if(dlg.DoModal() == IDOK)
	{
		// ��������
		UpdateData(TRUE);

		// ��ͼ����а�����˹��ͨ�˲�
		CFreqFilter clsFreqFilter(pDoc->m_pDIB);
		clsFreqFilter.WienerFilter(dlg.m_fValue);

		// �ػ���ͼ
		Invalidate();

		// ��������
		pDoc->SetModifiedFlag(TRUE);
	}
}

///////////////////////////////////////////
//	��ͼ��ʵʩ�����׾����˲�			 //	
///////////////////////////////////////////
void CDIP_SystemView::OnPSE_Filter() 
{
	// ��ȡ�ĵ�
	CDIP_SystemDoc* pDoc = GetDocument();

	if (pDoc->DIBImage() == NULL) return;
	pDoc->m_pDIB->PushCopy();

	// �ɶԻ��������ֹƵ��
	CDLGSetFreq dlg;
	dlg.m_fValue = 250.0f;
	if(dlg.DoModal() == IDOK)
	{
		// ��������
		UpdateData(TRUE);

		// ��ͼ����а�����˹��ͨ�˲�
		CFreqFilter clsFreqFilter(pDoc->m_pDIB);
		clsFreqFilter.PSE_Filter(dlg.m_fValue);

		// �ػ���ͼ
		Invalidate();

		// ��������
		pDoc->SetModifiedFlag(TRUE);
	}
}

///////////////////////////////////////////
//	��ͼ��ʵʩ��С�����˲�				 //	
///////////////////////////////////////////
void CDIP_SystemView::OnMinPower() 
{
	// ��ȡ�ĵ�
	CDIP_SystemDoc* pDoc = GetDocument();

	if (pDoc->DIBImage() == NULL) return;
	pDoc->m_pDIB->PushCopy();

	// �ɶԻ��������ֹƵ��
	CDLGSetFreq dlg;
	dlg.m_fValue = 250.0f;
	if(dlg.DoModal() == IDOK)
	{
		// ��������
		UpdateData(TRUE);

		// ��ͼ����а�����˹��ͨ�˲�
		CFreqFilter clsFreqFilter(pDoc->m_pDIB);
		clsFreqFilter.MinPower(dlg.m_fValue);

		// �ػ���ͼ
		Invalidate();

		// ��������
		pDoc->SetModifiedFlag(TRUE);
	}
}

///////////////////////////////////////////
//	�Կռ�ʧ��ͼ����н������ӽǶȣ�	 //	
///////////////////////////////////////////
void CDIP_SystemView::OnRotate() 
{
	// ��ȡ�ĵ�
	CDIP_SystemDoc* pDoc = GetDocument();

	if (pDoc->DIBImage() == NULL) return;
	pDoc->m_pDIB->PushCopy();

	// �ɶԻ���������ת�Ƕ�
	CDLGSetAngle dlg;
	dlg.m_fAngle = 0.0f;
	if(dlg.DoModal() == IDOK)
	{
		// ��������
		UpdateData(TRUE);

		// ��ͼ����нǶȽ���
		CGeoTrans clsGeoTrans(pDoc->m_pDIB);
		clsGeoTrans.Rotate(dlg.m_fAngle);

		// �ػ���ͼ
		Invalidate();

		// ��������
		pDoc->SetModifiedFlag(TRUE);
	}
}

///////////////////////////////////////////
//	�Կռ�ʧ��ͼ�����Уֱ���ӿռ�Ƕȣ� //	
///////////////////////////////////////////
void CDIP_SystemView::OnRectinication() 
{
	// ��ȡ�ĵ�
	CDIP_SystemDoc* pDoc = GetDocument();

	if (pDoc->DIBImage() == NULL) return;
	pDoc->m_pDIB->PushCopy();

	// �ɶԻ���������ת�Ƕ�
	CDLGSetAngle dlg;
	dlg.m_fAngle = 0.0f;
	if(dlg.DoModal() == IDOK)
	{
		// ��������
		UpdateData(TRUE);

		// ��ͼ����нǶȽ���
		CGeoTrans clsGeoTrans(pDoc->m_pDIB);
		clsGeoTrans.Rectinication(dlg.m_fAngle);

		// �ػ���ͼ
		Invalidate();

		// ��������
		pDoc->SetModifiedFlag(TRUE);
	}
}

///////////////////////////////////////////
//	��ͼ������������					 //	
///////////////////////////////////////////
void CDIP_SystemView::OnRandomNoise() 
{
	// ��ȡ�ĵ�
	CDIP_SystemDoc* pDoc = GetDocument();

	if (pDoc->DIBImage() == NULL) return;
	pDoc->m_pDIB->PushCopy();

	// ��ͼ����а�����˹��ͨ�˲�
	CNoise clsNoise(pDoc->m_pDIB);
	clsNoise.RandomNoise();

	// �ػ���ͼ
	Invalidate();

	// ��������
	pDoc->SetModifiedFlag(TRUE);
}

///////////////////////////////////////////
//	��ͼ�������������					 //	
///////////////////////////////////////////
void CDIP_SystemView::OnSaltNoise() 
{
	// ��ȡ�ĵ�
	CDIP_SystemDoc* pDoc = GetDocument();

	if (pDoc->DIBImage() == NULL) return;
	pDoc->m_pDIB->PushCopy();

	// ��ͼ����а�����˹��ͨ�˲�
	CNoise clsNoise(pDoc->m_pDIB);
	clsNoise.SaltNoise();

	// �ػ���ͼ
	Invalidate();

	// ��������
	pDoc->SetModifiedFlag(TRUE);
}

///////////////////////////[��ѧ��̬ѧ����]//////////////////////////////////////////

///////////////////////////////////////////
//	��ͼ��ʵʩˮƽ��ʴ����				 //	
///////////////////////////////////////////
void CDIP_SystemView::OnHErosion() 
{
	// ��ȡ�ĵ�
	CDIP_SystemDoc* pDoc = GetDocument();

	if (pDoc->DIBImage() == NULL) return;
	pDoc->m_pDIB->PushCopy();

	// ��ͼ����а�����˹��ͨ�˲�
	CMorph clsMorph(pDoc->m_pDIB);
	clsMorph.HErosion();

	// �ػ���ͼ
	Invalidate();

	// ��������
	pDoc->SetModifiedFlag(TRUE);
}

///////////////////////////////////////////
//	��ͼ��ʵʩ��ֱ��ʴ����				 //	
///////////////////////////////////////////
void CDIP_SystemView::OnVErosion() 
{
	// ��ȡ�ĵ�
	CDIP_SystemDoc* pDoc = GetDocument();

	if (pDoc->DIBImage() == NULL) return;
	pDoc->m_pDIB->PushCopy();

	// ��ͼ����а�����˹��ͨ�˲�
	CMorph clsMorph(pDoc->m_pDIB);
	clsMorph.VErosion();

	// �ػ���ͼ
	Invalidate();

	// ��������
	pDoc->SetModifiedFlag(TRUE);
}

///////////////////////////////////////////
//	��ͼ��ʵʩ��ֱ���ʹ���				 //	
///////////////////////////////////////////
void CDIP_SystemView::OnHDilation() 
{
	// ��ȡ�ĵ�
	CDIP_SystemDoc* pDoc = GetDocument();

	if (pDoc->DIBImage() == NULL) return;
	pDoc->m_pDIB->PushCopy();

	// ��ͼ����а�����˹��ͨ�˲�
	CMorph clsMorph(pDoc->m_pDIB);
	clsMorph.HDilation();

	// �ػ���ͼ
	Invalidate();

	// ��������
	pDoc->SetModifiedFlag(TRUE);
}

///////////////////////////////////////////
//	��ͼ��ʵʩ��ֱ���ʹ���				 //	
///////////////////////////////////////////
void CDIP_SystemView::OnVDilation() 
{
	// ��ȡ�ĵ�
	CDIP_SystemDoc* pDoc = GetDocument();

	if (pDoc->DIBImage() == NULL) return;
	pDoc->m_pDIB->PushCopy();

	// ��ͼ����а�����˹��ͨ�˲�
	CMorph clsMorph(pDoc->m_pDIB);
	clsMorph.VDilation();

	// �ػ���ͼ
	Invalidate();

	// ��������
	pDoc->SetModifiedFlag(TRUE);
}

///////////////////////////////////////////
//	��ͼ��ʵʩȫ����ʴ				 //	
///////////////////////////////////////////
void CDIP_SystemView::OnVhErosion() 
{
	// ��ȡ�ĵ�
	CDIP_SystemDoc* pDoc = GetDocument();

	if (pDoc->DIBImage() == NULL) return;
	pDoc->m_pDIB->PushCopy();

	// ��ͼ����а�����˹��ͨ�˲�
	CMorph clsMorph(pDoc->m_pDIB);
	clsMorph.VHErosion();

	// �ػ���ͼ
	Invalidate();

	// ��������
	pDoc->SetModifiedFlag(TRUE);
}

///////////////////////////////////////////
//	��ͼ��ʵʩȫ��������				 //	
///////////////////////////////////////////
void CDIP_SystemView::OnVhDilation() 
{
	// ��ȡ�ĵ�
	CDIP_SystemDoc* pDoc = GetDocument();

	if (pDoc->DIBImage() == NULL) return;
	pDoc->m_pDIB->PushCopy();

	// ��ͼ����а�����˹��ͨ�˲�
	CMorph clsMorph(pDoc->m_pDIB);
	clsMorph.VHDilation();

	// �ػ���ͼ
	Invalidate();

	// ��������
	pDoc->SetModifiedFlag(TRUE);
}

///////////////////////////////////////////
//	��ͼ��ʵʩ�����㴦��				 //	
///////////////////////////////////////////
void CDIP_SystemView::OnOpenOperate() 
{
	// ��ȡ�ĵ�
	CDIP_SystemDoc* pDoc = GetDocument();

	if (pDoc->DIBImage() == NULL) return;
	pDoc->m_pDIB->PushCopy();

	// ��ͼ����а�����˹��ͨ�˲�
	CMorph clsMorph(pDoc->m_pDIB);
	clsMorph.OpenOperate();

	// �ػ���ͼ
	Invalidate();

	// ��������
	pDoc->SetModifiedFlag(TRUE);
}

///////////////////////////////////////////
//	��ͼ��ʵʩ�����㴦��				 //	
///////////////////////////////////////////
void CDIP_SystemView::OnCloseOperate() 
{
	// ��ȡ�ĵ�
	CDIP_SystemDoc* pDoc = GetDocument();

	if (pDoc->DIBImage() == NULL) return;
	pDoc->m_pDIB->PushCopy();

	// ��ͼ����а�����˹��ͨ�˲�
	CMorph clsMorph(pDoc->m_pDIB);
	clsMorph.CloseOperate();

	// �ػ���ͼ
	Invalidate();

	// ��������
	pDoc->SetModifiedFlag(TRUE);
}

///////////////////////////////////////////
//	��ͼ��ʵʩϸ������					 //	
///////////////////////////////////////////
void CDIP_SystemView::OnThin() 
{
	// ��ȡ�ĵ�
	CDIP_SystemDoc* pDoc = GetDocument();

	if (pDoc->DIBImage() == NULL) return;
	pDoc->m_pDIB->PushCopy();

	// ��ͼ����а�����˹��ͨ�˲�
	CMorph clsMorph(pDoc->m_pDIB);
	clsMorph.Thin();

	// �ػ���ͼ
	Invalidate();

	// ��������
	pDoc->SetModifiedFlag(TRUE);
}

///////////////////////////////////////////
//	��ͼ��ʵʩ�ֻ�����					 //	
///////////////////////////////////////////
void CDIP_SystemView::OnThick() 
{
	// ��ȡ�ĵ�
	CDIP_SystemDoc* pDoc = GetDocument();

	if (pDoc->DIBImage() == NULL) return;
	pDoc->m_pDIB->PushCopy();

	// ��ͼ����а�����˹��ͨ�˲�
	CMorph clsMorph(pDoc->m_pDIB);
	clsMorph.Thick();

	// �ػ���ͼ
	Invalidate();

	// ��������
	pDoc->SetModifiedFlag(TRUE);
}

///////////////////////////////////////////
//	��ͼ��ʵʩ����任					 //	
///////////////////////////////////////////
void CDIP_SystemView::OnMat() 
{
	// ��ȡ�ĵ�
	CDIP_SystemDoc* pDoc = GetDocument();

	if (pDoc->DIBImage() == NULL) return;
	pDoc->m_pDIB->PushCopy();

	// ��ͼ����а�����˹��ͨ�˲�
	CMorph clsMorph(pDoc->m_pDIB);
	clsMorph.MAT();

	// �ػ���ͼ
	Invalidate();

	// ��������
	pDoc->SetModifiedFlag(TRUE);
}

///////////////////////////[ͼ����벿��]////////////////////////////////////////////

///////////////////////////////////////////
//	��ͼ��ʵʩJPEG����ѹ��				 //	
///////////////////////////////////////////
void CDIP_SystemView::OnJpegEncoding() 
{
	// ��ȡ�ĵ�
	CDIP_SystemDoc* pDoc = GetDocument();
	
	if (pDoc->m_strCurrentFile == "")
	{
		AfxMessageBox(L"���ȴ�BMPλͼ�ļ�����ѹ��!");
		return;
	}
	if (pDoc->m_strCurrentFile.Right(4) != ".bmp" || pDoc->m_strCurrentFile.Right(4) != ".BMP")
	{
		AfxMessageBox(L"��ȷ�ϵ�ǰͼ��ĸ�ʽΪBMPλͼ!");
		return;
	}
	if (pDoc->IsModified())
	{
		AfxMessageBox(L"���ȱ���ԭʼBMPλͼ�ļ�����ѹ��!");
		return;
	}

	// ѡ�񱣴��ļ�
    CFileDialog fileDlg(FALSE, L"*.JPG", L"*.JPG", NULL, L"JPEG �ļ�(*.JPG)|*.JPG||", this);
	fileDlg.m_ofn.Flags |= OFN_FILEMUSTEXIST;
	fileDlg.m_ofn.lpstrTitle = L"�ļ�����Ϊ...";

	// ����ѹ��ͼ��
	if (fileDlg.DoModal() == IDOK)
	{
		CCoding clsCoding(pDoc->m_pDIB);
		clsCoding.SaveJPG(pDoc, fileDlg.GetPathName());
	}            
}

///////////////////////////////////////////
//	��ͼ��ʵʩJPEG�����ѹ��			 //	
///////////////////////////////////////////
void CDIP_SystemView::OnJpegDecoding() 
{
	// ѡ����ļ�
	CFileDialog fileDlg(TRUE, L"*.JPG", L"*.JPG", NULL, L"JPEG �ļ�(*.JPG)|*.JPG||", this);
	fileDlg.m_ofn.Flags|=OFN_FILEMUSTEXIST;
	fileDlg.m_ofn.lpstrTitle = L"��ѡ��JPG�ļ�...";

	if (fileDlg.DoModal() == IDOK) 
	{
		CDIP_SystemDoc* pDoc = GetDocument();
		if(pDoc->DIBImage() != NULL) // �½��հ��ĵ�
			pDoc = pDoc->CreateNewDocument();

		// ��ѹͼ��
		CCoding clsCoding;
		clsCoding.LoadJPG(pDoc, fileDlg.GetPathName());

		// �ػ���ͼ
		Invalidate();
	}
}

///////////////////////////////////////////
//	��ͼ��ʵʩPCX����ѹ��				 //	
///////////////////////////////////////////
void CDIP_SystemView::OnPCXEncoding() 
{
	// ��ȡ�ĵ�
	CDIP_SystemDoc* pDoc = GetDocument();
	
	// ѡ�񱣴��ļ�
    CFileDialog fileDlg(FALSE, L"*.PCX", L"*.PCX", NULL, L"PCX �ļ�(*.PCX)|*.PCX||", this);
	fileDlg.m_ofn.Flags |= OFN_FILEMUSTEXIST;
	fileDlg.m_ofn.lpstrTitle = L"�ļ�����Ϊ...";

	// ����ѹ��ͼ��
	if (fileDlg.DoModal() == IDOK)
	{
		CCoding clsCoding(pDoc->m_pDIB);
		clsCoding.DIBToPCX(fileDlg.GetPathName());
	}            
}

///////////////////////////////////////////
//	��ͼ��ʵʩPCX�����ѹ��				 //	
///////////////////////////////////////////
void CDIP_SystemView::OnPCXDecoding() 
{
	// ������DIB
	HDIB hNewDIB = NULL;
	
	// ��ȡ�ĵ�
	CDIP_SystemDoc* pDoc = GetDocument();

	// ѡ����ļ�
	CFileDialog fileDlg(TRUE, L"*.PCX", L"*.PCX", NULL, L"PCX �ļ�(*.PCX)|*.PCX||", this);
	fileDlg.m_ofn.Flags|=OFN_FILEMUSTEXIST;
	fileDlg.m_ofn.lpstrTitle = L"��ѡ��PCX�ļ�...";

	if (fileDlg.DoModal() == IDOK) 
	{
		// ��ȡ�ĵ�
		CDIP_SystemDoc* pDoc = GetDocument();
		if (pDoc->DIBImage() != NULL)
			pDoc = pDoc->CreateNewDocument();
		// ��ʼ��ָ��
		pDoc->m_pDIB->m_pImage = new CImage;
		pDoc->m_pDIB->m_UndoStack.Push(pDoc->m_pDIB->m_pImage);

		// ��ѹͼ��
		CCoding clsCoding(pDoc->m_pDIB);
		hNewDIB = clsCoding.PCXToDIB(fileDlg.GetPathName());

		// �жϴ����Ƿ�ɹ�
		if (hNewDIB != NULL)
		{
			// ����DIB��С�͵�ɫ��
			pDoc->InitDIBData();
		
			// ��������
			pDoc->SetModifiedFlag(TRUE);
		
			// �������ù�����ͼ��С
			SetScrollSizes(MM_TEXT, pDoc->GetDocSize());

			// ������ͼ
			pDoc->UpdateAllViews(NULL);
		}
		else
		{
			
			MessageBox(L"�����ڴ�ʧ�ܣ�", L"ϵͳ��ʾ" , MB_ICONINFORMATION | MB_OK);
		}
	}
}

///////////////////////////////////////////
//	��ͼ��ʵʩ����������				 //	
///////////////////////////////////////////
void CDIP_SystemView::OnHuffman() 
{
	// ��ȡ�ĵ�
	CDIP_SystemDoc* pDoc = GetDocument();

	AfxMessageBox(L"���ܴ���ӣ�"); return;
	
	// ѡ�񱣴��ļ�
    CFileDialog fileDlg(FALSE, L"*.HUF", L"*.HUF", NULL, L"HUFFMAN �ļ�(*.HUF)|*.HUF||", this);
	fileDlg.m_ofn.Flags |= OFN_FILEMUSTEXIST;
	fileDlg.m_ofn.lpstrTitle = L"�ļ�����Ϊ...";

	// ����ѹ��ͼ��
	if (fileDlg.DoModal() == IDOK)
	{
	}            
}

///////////////////////////////////////////
//	��ͼ��ʵʩ��ũ-��ŵ����				 //	
///////////////////////////////////////////
void CDIP_SystemView::OnShannonFannon() 
{
	// ��ȡ�ĵ�
	CDIP_SystemDoc* pDoc = GetDocument();
	
	AfxMessageBox(L"���ܴ���ӣ�"); return;

	// ѡ�񱣴��ļ�
    CFileDialog fileDlg(FALSE, L"*.FAN", L"*.FAN", NULL, L"Shannon-Fannon �ļ�(*.FAN)|*.FAN||", this);
	fileDlg.m_ofn.Flags |= OFN_FILEMUSTEXIST;
	fileDlg.m_ofn.lpstrTitle = L"�ļ�����Ϊ...";

	// ����ѹ��ͼ��
	if (fileDlg.DoModal() == IDOK)
	{
	}            
}

///////////////////////////////////////////
//	��ͼ��ʵʩGIF����ѹ��				 //	
///////////////////////////////////////////
void CDIP_SystemView::OnLzwEncoding() 
{
	// ��ȡ�ĵ�
	CDIP_SystemDoc* pDoc = GetDocument();
	
	// ѡ�񱣴��ļ�
    CFileDialog fileDlg(FALSE, L"*.GIF", L"*.GIF", NULL, L"GIF �ļ�(*.GIF)|*.GIF||", this);
	fileDlg.m_ofn.Flags |= OFN_FILEMUSTEXIST;
	fileDlg.m_ofn.lpstrTitle = L"�ļ�����Ϊ...";

	// ����ѹ��ͼ��
	if (fileDlg.DoModal() == IDOK)
	{
		if (!pDoc->CreateTempFile())
		{
			AfxMessageBox(L"���������ļ�����");
			return;
		}
		CFile file;
		file.Open(fileDlg.GetPathName(), CFile::modeCreate|CFile::modeReadWrite);
		CCoding clsCoding;
		clsCoding.DIBToGIF(pDoc->GetHDIB(), file, TRUE);
		file.Close();
	}            
}

///////////////////////////////////////////
//	��ͼ��ʵʩGIF�����ѹ��				 //	
///////////////////////////////////////////
void CDIP_SystemView::OnLzwDecoding() 
{
	// ������DIB
	HDIB hNewDIB = NULL;
	
	// ��ȡ�ĵ�
	CDIP_SystemDoc* pDoc = GetDocument();

	// ѡ�񱣴��ļ�
    CFileDialog fileDlg(TRUE, L"*.GIF", L"*.GIF", NULL, L"GIF �ļ�(*.GIF)|*.GIF||", this);
	fileDlg.m_ofn.Flags |= OFN_FILEMUSTEXIST;
	fileDlg.m_ofn.lpstrTitle = L"��ѡ��GIF�ļ�...";

	// ����ѹ��ͼ��
	if (fileDlg.DoModal() == IDOK)
	{
		// ����ZoomDIB()������ȡ��С���DIB
		CCoding clsCoding(pDoc->m_pDIB);
		CFile file;
		file.Open(fileDlg.GetPathName(),CFile::modeReadWrite);
		hNewDIB = clsCoding.GIFToDIB(file);
		file.Close();

		// �ж������Ƿ�ɹ�
		if (hNewDIB != NULL)
		{		
			// �滻DIB��ͬʱ�ͷž�DIB����
			pDoc->ReplaceHDIB(hNewDIB);
	
			// ����DIB��С�͵�ɫ��
			pDoc->InitDIBData();
		
			pDoc->HDIB2CImage(hNewDIB);

			// �������ù�����ͼ��С
			SetScrollSizes(MM_TEXT, pDoc->GetDocSize());

			pDoc->SetModifiedFlag(TRUE);

			// ������ͼ
			pDoc->UpdateAllViews(NULL);
		}
		else
		{
			
			MessageBox(L"�����ڴ�ʧ�ܣ�", L"ϵͳ��ʾ" , MB_ICONINFORMATION | MB_OK);
		}
	}
}

///////////////////////////////////////////
//	Ƕ��ˮӡ							 //	
///////////////////////////////////////////
void CDIP_SystemView::OnSetWatermarking() 
{
	// ��ȡ�ĵ�
	CDIP_SystemDoc* pDoc = GetDocument();

	if (pDoc->DIBImage() == NULL) return;
	pDoc->m_pDIB->PushCopy();

	// ����ѹ��ͼ��
	CWaterMarking clsWaterMarking(pDoc->m_pDIB);
	clsWaterMarking.Set();
}

///////////////////////////////////////////
//	���ˮӡ							 //	
///////////////////////////////////////////
void CDIP_SystemView::OnGetWatermarking() 
{
	// ��ȡ�ĵ�
	CDIP_SystemDoc* pDoc = GetDocument();
	
	// ѡ�񱣴��ļ�
    CFileDialog fileDlg(FALSE, L"*.BMP", L"*.BMP", NULL, L"ˮӡ�ļ�(*.BMP)|*.BMP||", this);
	fileDlg.m_ofn.Flags |= OFN_FILEMUSTEXIST;
	fileDlg.m_ofn.lpstrTitle = L"�ļ�����Ϊ...";

	// ����ѹ��ͼ��
	if (fileDlg.DoModal() == IDOK)
	{
		CFile file;
		file.Open(fileDlg.GetPathName(), CFile::modeCreate | CFile::modeReadWrite);
		CWaterMarking clsWaterMarking(pDoc->m_pDIB);
		clsWaterMarking.Get(file);
		file.Close();
	}            
}

///////////////////////////////////////////
//	С���ֽ�							 //	
///////////////////////////////////////////
void CDIP_SystemView::OnDwt() 
{
	// ��ȡ�ĵ�
	CDIP_SystemDoc* pDoc = GetDocument();

	if (pDoc->DIBImage() == NULL) return;
	pDoc->m_pDIB->PushCopy();

	// ��ͼ����а�����˹��ͨ�˲�
	CFreqCalculate clsFreqCalculate(pDoc->m_pDIB);
	clsFreqCalculate.DIBDWT();

	// �ػ���ͼ
	Invalidate();

	// ��������
	pDoc->SetModifiedFlag(TRUE);
}

///////////////////////////////////////////
//	С���ع�							 //	
///////////////////////////////////////////
void CDIP_SystemView::OnIdwt() 
{
	// ��ȡ�ĵ�
	CDIP_SystemDoc* pDoc = GetDocument();

	if (pDoc->DIBImage() == NULL) return;
	pDoc->m_pDIB->PushCopy();

	// ��ͼ����а�����˹��ͨ�˲�
	CFreqCalculate clsFreqCalculate(pDoc->m_pDIB);
	clsFreqCalculate.DIBIDWT();

	// �ػ���ͼ
	Invalidate();

	// ��������
	pDoc->SetModifiedFlag(TRUE);
}

///////////////////////////////////////////
//	�����߼�ͼ����Ի���				 //	
///////////////////////////////////////////

// ����"�����任"�Ի���
void CDIP_SystemView::OnCallZoomDlg()
{
	// ��ȡ�ĵ�
	CDIP_SystemDoc* pDoc = GetDocument();
	CImage* pImage = pDoc->DIBImage();
	if (pImage == NULL) return;

	pDoc->m_pDIB->PushCopy();

	m_pZoomDlg->m_nWidthNew = m_pZoomDlg->m_dWidthEdit = pImage->GetWidth();
	m_pZoomDlg->m_nHeightNew = m_pZoomDlg->m_dHeightEdit = pImage->GetHeight();
	m_pZoomDlg->ShowWindow(SW_SHOW);
	pDoc->m_sizeDoc.cx = pImage->GetWidth();
	pDoc->m_sizeDoc.cy = pImage->GetHeight();
	// ��������
	if (m_pZoomDlg->m_bModified)
		pDoc->SetModifiedFlag(TRUE);
}

// ����"ɫ��"�Ի���
void CDIP_SystemView::OnCallColorgradDlg()
{
	CColorGradDlg Dlg(this);
	Dlg.DoModal();
}

// ����"����"�Ի���
void CDIP_SystemView::OnCallCurveDlg()
{
	CCurveDlg Dlg;
	Dlg.DoModal();
}

// ����"ֱ��ͼ"�Ի���
void CDIP_SystemView::OnCallHistogramDlg()
{
	CHECK_IMAGE_NOT_NULL;
	CHistogramDlg Dlg;
	Dlg.DoModal();
}

// ����"�ռ��˲�"�Ի���
void CDIP_SystemView::OnCallSpacefilterDlg()
{
	CHECK_IMAGE_NOT_NULL;
	CSpaceFilterDlg Dlg;
	Dlg.DoModal();
}

// ��ʾ�����ļ�
void CDIP_SystemView::OnTempFile()
{
	m_bShowImage = !m_bShowImage;
	Invalidate();
}

void CDIP_SystemView::OnUpdateTempFile(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(GetDocument()->GetHDIB() != NULL);
	pCmdUI->SetCheck(!m_bShowImage);
}

// �鿴��ǿ��ֱ��ͼ
void CDIP_SystemView::OnHistogramIi()
{
	CHECK_IMAGE_NOT_NULL;
	CEnhancedHistDlg dlg;
	dlg.DoModal();
}

// ����"�Ҷ�����"�Ի���
void CDIP_SystemView::OnCallStretchDlg()
{
	CHECK_IMAGE_NOT_NULL;
	CStretchDlg dlg;
	dlg.DoModal();
}

// ����"��ֵ�任"�Ի���
void CDIP_SystemView::OnCallThresholdDlg()
{
	CHECK_IMAGE_NOT_NULL;
	CThresholdDlg dlg;
	dlg.DoModal();
}

void CDIP_SystemView::ResetView()
{
	CDIP_SystemDoc* pDoc = GetDocument();
	m_PaintRect.right = pDoc->m_pDIB->Width();
	m_PaintRect.bottom = pDoc->m_pDIB->Height();
	m_bShowImage = ID_VIEW_DEFAULT;
	SetScrollSizes(MM_TEXT, CSize(m_PaintRect.right, m_PaintRect.bottom));
}

// ����
void CDIP_SystemView::OnEditUndo()
{
	CDIP_SystemDoc* pDoc = GetDocument();
	if (pDoc->DIBImage() == NULL) return;
	pDoc->m_pDIB->Undo();
	ResetView();
	Invalidate();
}

// �ָ�
void CDIP_SystemView::OnEditRedo()
{
	CDIP_SystemDoc* pDoc = GetDocument();
	if (pDoc->DIBImage() == NULL) return;
	pDoc->m_pDIB->Redo();
	ResetView();
	Invalidate();
}

void CDIP_SystemView::OnUpdateEditUndo(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(GetDocument()->m_pDIB->m_UndoStack.m_nSizeOfStack > 1);
}

void CDIP_SystemView::OnUpdateEditRedo(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!GetDocument()->m_pDIB->m_RedoStack.IsEmpty());
}

// �Ŵ���ͼ
void CDIP_SystemView::OnViewZoomin()
{
	m_nViewType = ID_VIEW_ZOOMIN;
	m_PaintRect.right = m_PaintRect.right * 2.0;
	m_PaintRect.bottom = m_PaintRect.bottom * 2.0;

	CDIP_SystemDoc *pDoc = GetDocument();
	if (pDoc->DIBImage() == NULL) return;
	if (m_PaintRect.right > VIEW_MAX_WIDTH)
	{
		m_PaintRect.right = VIEW_MAX_WIDTH;
		float rate = 1.0f * VIEW_MAX_WIDTH / pDoc->m_pDIB->Width();
		m_PaintRect.bottom = int(pDoc->m_pDIB->Height() * rate);
	}
	pDoc->SetDocSize(m_PaintRect.right, m_PaintRect.bottom);

	// ����ԭ��
	ScrollToPosition(POINT());
	Invalidate(TRUE);
}

// ��С��ͼ
void CDIP_SystemView::OnViewZoomout()
{
	m_nViewType = ID_VIEW_ZOOMOUT;
	m_PaintRect.right = m_PaintRect.right / 2.0;
	m_PaintRect.bottom = m_PaintRect.bottom / 2.0;

	CDIP_SystemDoc *pDoc = GetDocument();
	if (pDoc->DIBImage() == NULL) return;
	if (m_PaintRect.right < VIEW_MIN_WIDTH)
	{
		m_PaintRect.right = VIEW_MIN_WIDTH;
		float rate = 1.0f * VIEW_MIN_WIDTH / pDoc->m_pDIB->Width();
		m_PaintRect.bottom = int(pDoc->m_pDIB->Height() * rate);
	}
	pDoc->SetDocSize(m_PaintRect.right, m_PaintRect.bottom);

	// ����ԭ��
	ScrollToPosition(POINT());
	Invalidate(TRUE);
}

// ��Ӧ����
void CDIP_SystemView::OnViewFitWindow()
{
	m_nViewType = ID_VIEW_FIT_WINDOW;
	Invalidate(TRUE);
}

// ��������Ļ��С
void CDIP_SystemView::OnViewScreenSize()
{
	m_nViewType = ID_VIEW_SCREEN_SIZE;
	m_PaintRect.right = GetSystemMetrics(SM_CXSCREEN); //��Ļ���
	m_PaintRect.bottom = GetSystemMetrics(SM_CYSCREEN); //��Ļ�߶�
	GetDocument()->SetDocSize(m_PaintRect.right, m_PaintRect.bottom);
	Invalidate(TRUE);
}

// ����Ĭ�ϵ���ͼ
void CDIP_SystemView::OnViewDefault()
{
	m_nViewType = ID_VIEW_DEFAULT;
	CDIP_SystemDoc* pDoc = GetDocument();
	if (pDoc->DIBImage() == NULL) return;
	m_PaintRect.right = pDoc->m_pDIB->Width();
	m_PaintRect.bottom = pDoc->m_pDIB->Height();
	pDoc->SetDocSize(m_PaintRect.right, m_PaintRect.bottom);
	Invalidate(TRUE);
}

void CDIP_SystemView::OnUpdateViewZoomin(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_nViewType == ID_VIEW_ZOOMIN);
}

void CDIP_SystemView::OnUpdateViewZoomout(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_nViewType == ID_VIEW_ZOOMOUT);
}


void CDIP_SystemView::OnUpdateViewScreenSize(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_nViewType == ID_VIEW_SCREEN_SIZE);
}


void CDIP_SystemView::OnUpdateViewFitWindow(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_nViewType == ID_VIEW_FIT_WINDOW);
}


void CDIP_SystemView::OnUpdateViewDefault(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_nViewType == ID_VIEW_DEFAULT);
}


// ����"λ���"�Ի���
void CDIP_SystemView::OnCallBppDlg()
{
	CDIP_SystemDoc* pDoc = GetDocument();
	if (pDoc->DIBImage() == NULL) return;
	CChangeBppDlg dlg;
	dlg.DoModal();
}

// ����"��ת"�Ի���
void CDIP_SystemView::OnCallRotateDlg()
{
	CHECK_IMAGE_NOT_NULL;
	CRotateDlg dlg;
	dlg.DoModal();
}

// ��ֱ��ת
void CDIP_SystemView::OnFlipVertical()
{
	CDIP_SystemDoc* pDoc = GetDocument();

	if (pDoc->DIBImage() == NULL) return;
	pDoc->m_pDIB->PushCopy();

	int bpp = pDoc->m_pDIB->BitCount(), Sel = IDYES;
	if (bpp != 32)
	{
		Sel = MessageBox(L"��ǰͼ�񽫱�תΪ32λɫ�����Ƿ�ϣ��������", L"����", MB_ICONWARNING | MB_YESNO);
		pDoc->m_pDIB->ChangeBPP(32);
	}

	UINT nWidth = pDoc->m_pDIB->Width();
	UINT nHeight = pDoc->m_pDIB->Height();
	BYTE* pSrc = pDoc->m_pDIB->FindAddress();
	BYTE* temp = NULL;
	
	VMirrorTrans(pSrc, temp, nWidth, nHeight);
	pDoc->m_pDIB->SetBits(temp, 4, nWidth, nHeight);

	if (Sel != IDYES)
	{
		pDoc->m_pDIB->ChangeBPP(bpp);
	}

	SAFE_DELETE(temp);

	pDoc->SetDocSize();
	Invalidate();
}

// ˮƽ��ת
void CDIP_SystemView::OnFlipHorizontal()
{
	CDIP_SystemDoc* pDoc = GetDocument();

	if (pDoc->DIBImage() == NULL) return;
	pDoc->m_pDIB->PushCopy();

	int bpp = pDoc->m_pDIB->BitCount(), Sel = IDYES;
	if (bpp != 32)
	{
		Sel = MessageBox(L"��ǰͼ�񽫱�תΪ32λɫ�����Ƿ�ϣ��������", L"����", MB_ICONWARNING | MB_YESNO);
		pDoc->m_pDIB->ChangeBPP(32);
	}

	UINT nWidth = pDoc->m_pDIB->Width();
	UINT nHeight = pDoc->m_pDIB->Height();
	BYTE* pSrc = pDoc->m_pDIB->FindAddress();
	BYTE* temp = NULL;

	HMirrorTrans(pSrc, temp, nWidth, nHeight);
	pDoc->m_pDIB->SetBits(temp, 4, nWidth, nHeight);

	if (Sel != IDYES)
	{
		pDoc->m_pDIB->ChangeBPP(bpp);
	}

	SAFE_DELETE(temp);

	pDoc->SetDocSize();
	Invalidate();
}

// ת��ͼ��
void CDIP_SystemView::OnTranspose()
{
	CDIP_SystemDoc* pDoc = GetDocument();

	if (pDoc->DIBImage() == NULL) return;
	pDoc->m_pDIB->PushCopy();

	int bpp = pDoc->m_pDIB->BitCount(), Sel = IDYES;
	if (bpp != 32)
	{
		Sel = MessageBox(L"��ǰͼ�񽫱�תΪ32λɫ�����Ƿ�ϣ��������", L"����", MB_ICONWARNING | MB_YESNO);
		pDoc->m_pDIB->ChangeBPP(32);
	}

	UINT nWidth = pDoc->m_pDIB->Width();
	UINT nHeight = pDoc->m_pDIB->Height();
	UINT nTempWidth = nWidth;
	UINT nTempHeight = nHeight;

	BYTE* pSrc = pDoc->m_pDIB->FindAddress();
	BYTE* temp = NULL;

	Transpose(pSrc, temp, nWidth, nHeight, nTempWidth, nTempHeight);

	pDoc->m_pDIB->SetBits(temp, 4, nTempWidth, nTempHeight);

	if (Sel != IDYES)
	{
		pDoc->m_pDIB->ChangeBPP(bpp);
	}

	SAFE_DELETE(temp);

	pDoc->SetDocSize();
	Invalidate();
}


LRESULT CDIP_SystemView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if(message == WM_MOUSEMOVE && m_bGetCurPixel)
	{
		MSG msg;
		msg.hwnd = m_hWnd;
		msg.message = message;
		msg.wParam = wParam;
		msg.lParam = lParam;

		m_ToolTip.RelayEvent(&msg);
	}
	return CScrollView::WindowProc(message, wParam, lParam);
}


void CDIP_SystemView::OnGetCurrentPixel()
{
	m_bGetCurPixel = !m_bGetCurPixel;
}


void CDIP_SystemView::OnUpdateGetCurrentPixel(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bGetCurPixel);
}

void CDIP_SystemView::NegativeImage(int channel)
{
	// ��ȡ�ĵ�
	CDIP_SystemDoc* pDoc = GetDocument();
	CDIB* pDIB = pDoc->m_pDIB;
	int nChannel = pDIB->Channel();
	// �ǲ�ɫͼ�񲻴���
	if (nChannel < 3)
		return;
	long nWidth = pDIB->Width();
	long nHeight = pDIB->Height();
	long nRowLen = pDIB->RowLen();
	CImage* pImage = new CImage;
	pImage->Create(nWidth, nHeight, nChannel * 8, 0);
	BYTE* pDst = (BYTE*)pImage->GetBits() + (pImage->GetPitch()*(nHeight - 1));
	BYTE* pSrc = pDIB->FindAddress();
	memcpy(pDst, pSrc, nHeight * nRowLen);
	for (int i = 0; i < nHeight; ++i)
	{
		for (int j = 0; j < nWidth; ++j)
		{
			*(pDst + j * nChannel + 2 - channel + i * nRowLen) = 255 - *(pSrc + j * nChannel + 2 - channel + i * nRowLen);
		}
	}
	pDIB->Clear();
	pDIB->m_UndoStack.SetTop(pImage);
	pDIB->SetImage(pImage);
	Invalidate();
	return;
}

// �鿴��ɫͨ����Ƭͼ��
void CDIP_SystemView::OnViewRChannelImage()
{
	CHECK_IMAGE_NOT_NULL;
	m_bRNegativeImage = !m_bRNegativeImage;
	NegativeImage(0);
}

// �鿴��ɫͨ����Ƭͼ��
void CDIP_SystemView::OnViewGChannelImage()
{
	CHECK_IMAGE_NOT_NULL;
	m_bGNegativeImage = !m_bGNegativeImage;
	NegativeImage(1);
}

// �鿴��ɫͨ����Ƭͼ��
void CDIP_SystemView::OnViewBChannelImage()
{
	CHECK_IMAGE_NOT_NULL;
	m_bBNegativeImage = !m_bBNegativeImage;
	NegativeImage(2);
}

// �鿴����ͨ����Ƭ
void CDIP_SystemView::OnViewAllChannelImage()
{
	CHECK_IMAGE_NOT_NULL;
	if (!m_bRNegativeImage)
	{
		m_bRNegativeImage = true;
		NegativeImage(0);
	}
	if (!m_bGNegativeImage)
	{
		m_bGNegativeImage = true;
		NegativeImage(1);
	}
	if (!m_bBNegativeImage)
	{
		m_bBNegativeImage = true;
		NegativeImage(2);
	}
}

void CDIP_SystemView::OnUpdateViewRChannelImage(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bRNegativeImage);
}


void CDIP_SystemView::OnUpdateViewGChannelImage(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bGNegativeImage);
}


void CDIP_SystemView::OnUpdateViewBChannelImage(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bBNegativeImage);
}


void CDIP_SystemView::OnUpdateViewAllChannelImage(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bRNegativeImage && m_bGNegativeImage && m_bBNegativeImage);
}

// R-2, G-1, B-0.ת����RGB��Ӧ��ͨ��
void CDIP_SystemView::ConvertImage(int R, int G, int B)
{
	// ��ȡ�ĵ�
	CDIP_SystemDoc* pDoc = GetDocument();
	if (pDoc->DIBImage() == NULL) return;
	CDIB* pDIB = pDoc->m_pDIB;
	int nChannel = pDIB->Channel();
	// �ǲ�ɫͼ�񲻴���
	if (nChannel < 3)
		return;
	long nWidth = pDIB->Width();
	long nHeight = pDIB->Height();
	long nRowLen = pDIB->RowLen();
	CImage* pImage = new CImage;
	pImage->Create(nWidth, nHeight, nChannel * 8, 0);
	BYTE* pDst = (BYTE*)pImage->GetBits() + (pImage->GetPitch()*(nHeight - 1));
	BYTE* pSrc = pDIB->FindAddress();
	memcpy(pDst, pSrc, nHeight * nRowLen);
	for (int i = 0; i < nHeight; ++i)
	{
		for (int j = 0; j < nWidth; ++j)
		{
			*(pDst + j * nChannel + 2 + i * nRowLen) = *(pSrc + j * nChannel + R + i * nRowLen);
			*(pDst + j * nChannel + 1 + i * nRowLen) = *(pSrc + j * nChannel + G + i * nRowLen);
			*(pDst + j * nChannel     + i * nRowLen) = *(pSrc + j * nChannel + B + i * nRowLen);
		}
	}
	pDIB->Clear();
	pDIB->m_UndoStack.SetTop(pImage);
	pDIB->SetImage(pImage);
	Invalidate();
	return;
}

void CDIP_SystemView::OnConvertRgb2Rbg()
{
	ConvertImage(2, 0, 1);
}


void CDIP_SystemView::OnConvertRgb2Brg()
{
	ConvertImage(0, 2, 1);
}


void CDIP_SystemView::OnConvertRgb2Bgr()
{
	ConvertImage(0, 1, 2);
}


void CDIP_SystemView::OnConvertRgb2Grb()
{
	ConvertImage(1, 2, 0);
}


void CDIP_SystemView::OnConvertRgb2Gbr()
{
	ConvertImage(1, 0, 2);
}

// *** �����ǹ��ڿ�ݼ� ***

int CDIP_SystemView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;
	// [2] ���ؿ�ݼ���Դ
	m_hHotKey = ::LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_ACCELERATOR));
	return 0;
}


BOOL CDIP_SystemView::PreTranslateMessage(MSG* pMsg)
{
	// [3] �����ݼ���Ϣ
	if(::TranslateAccelerator(GetSafeHwnd(), m_hHotKey, pMsg))   
		return true;

	return CScrollView::PreTranslateMessage(pMsg);
}

// �����任
void CDIP_SystemView::OnHotkeyAltZ()
{
	OnCallZoomDlg();
}

// Ĭ����ͼ
void CDIP_SystemView::OnHotkeyCtrl0()
{
	OnViewDefault();
}

// ����
void CDIP_SystemView::OnHotkeyCtrlC()
{
	OnEditCopy();
}

// �ռ��˲�
void CDIP_SystemView::OnHotkeyCtrlF()
{
	OnCallSpacefilterDlg();
}

// ֱ��ͼ
void CDIP_SystemView::OnHotkeyCtrlH()
{
	OnCallHistogramDlg();
}

// ɫ��
void CDIP_SystemView::OnHotkeyCtrlL()
{
	OnCallColorgradDlg();
}

// ����
void CDIP_SystemView::OnHotkeyCtrlM()
{
	OnCallCurveDlg();
}

// �½�
void CDIP_SystemView::OnHotkeyCtrlN()
{
	CDIP_SystemApp* pMyApp = (CDIP_SystemApp*)AfxGetApp();
	pMyApp->OnFileMynew();
}

// ��
void CDIP_SystemView::OnHotkeyCtrlO()
{
	CDIP_SystemApp* pMyApp = (CDIP_SystemApp*)AfxGetApp();
	pMyApp->OnFileMyopen();
}

// ��ӡ
void CDIP_SystemView::OnHotkeyCtrlP()
{
	OnFilePrint();
}

// �˳�
void CDIP_SystemView::OnHotkeyCtrlQ()
{
	AfxGetMainWnd()->SendMessage(WM_CLOSE);
}

// ����
void CDIP_SystemView::OnHotkeyCtrlR()
{
	OnEditRedo();
}

// ����
void CDIP_SystemView::OnHotkeyCtrlS()
{
	CDIP_SystemDoc *pDoc = GetDocument();
	pDoc->OnSaveDocument(pDoc->m_strCurrentFile);
}

// ���
void CDIP_SystemView::OnHotkeyCtrlV()
{
	OnEditPaste();
}

// �ر�
void CDIP_SystemView::OnHotkeyCtrlW()
{
	CDIP_SystemDoc *pDoc = GetDocument();
	pDoc->OnCloseDocument();
}

// ����
void CDIP_SystemView::OnHotkeyCtrlZ()
{
	OnEditUndo();
}

// �Ŵ���ͼ
void CDIP_SystemView::OnHotkeyZoomIn()
{
	OnViewZoomin();
}

// ��С��ͼ
void CDIP_SystemView::OnHotkeyZoomOut()
{
	OnViewZoomout();
}