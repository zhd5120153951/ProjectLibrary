/*!
 * \file DIP_SystemDoc.h
 * \date 2023/01/15 11:30
 *
 * \author Hedong
 * \Contact: zenghedong@outlook.com
 *
 * \brief: DIP_SystemDoc.h : interface of the CDIP_SystemDoc class
 *
 * \TODO: long description
 *
 * \note
*/

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "dib.h"

class CDIP_SystemView;

class CDIP_SystemDoc : public CDocument
{
protected: // create from serialization only
	CDIP_SystemDoc();
	virtual ~CDIP_SystemDoc();
	DECLARE_DYNCREATE(CDIP_SystemDoc)

// Attributes
public:
	CSize				m_sizeDoc;			//
	CDIB*				m_pDIB;				//
	CRectTracker		m_Tracker;			//
	int					m_nColorIndex;		//
	COLORREF			m_refColorBKG;		// ����ɫ
	CPoint				m_ptPoint_I;		// ���ӵ�1
	CPoint				m_ptPoint_II;		// ���ӵ�2
	CPoint				m_ptLastPos;		// ����ƶ���һ��λ��
	CString				m_strCurrentFile;	//
	BOOL				m_bIsFrequency;		//
	int					m_nRgnNum;			//
	CRect				m_rect[1000];		//
	float				m_fZoomRatio;		// ���ٷŴ����С�ı���

	// ԭ��
	CPalette*			m_palDIB;
	HDIB				m_hDIB;

	// ����
	CString				m_sFileName;		//�ļ�����
	CString				m_sFilePath;		//�ļ�ȫ��
	BOOL				m_bIsDecoding;		//�Ƿ�Ϊ����ͼ��
	BOOL				m_bJumpSecLine;		//�Ƿ������ڶ���

public:
	CImage* DIBImage() const
		{
			return m_pDIB->GetImage();
	}
	CSize GetDocSize() const
		{ return m_sizeDoc; }
	void SetDocSize();
	void SetDocSize(int cx, int cy);
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual void OnCloseDocument();
	//����
	BOOL HDIB2CImage(HDIB hDIB);
	BOOL CreateTempFile(CString strFileName = L"TempFile.bmp");
	CDIP_SystemDoc* CreateNewDocument()// �½��հ��ĵ�
		{
			return (CDIP_SystemDoc *)GetDocTemplate()->OpenDocumentFile(NULL);
		}
	BOOL OpenPXMFile(LPCTSTR lpszPathName);// ��PGM��ʽ
	//ԭ��
	CPalette* GetDocPalette() const
		{
			return m_palDIB;
		}
	HDIB GetHDIB() const
		{
			return m_hDIB;
		}
	void InitDIBData();
	void ReplaceHDIB(HDIB hDIB);
	BOOL OnOpenDocument(LPCTSTR lpszPathName, BOOL bReserved);
	BOOL OnSaveDocument(LPCTSTR lpszPathName, BOOL bReserved);
	CDIP_SystemView* GetView(CRuntimeClass* pClass);

// Generated message map functions
public:
	afx_msg void OnFileReopen();
	DECLARE_MESSAGE_MAP()

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

