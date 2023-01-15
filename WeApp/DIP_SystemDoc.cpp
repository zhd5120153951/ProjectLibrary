// DIP_SystemDoc.cpp : implementation of the CDIP_SystemDoc class

#include "stdafx.h"
#include "DIP_System.h"

#include "DIP_SystemDoc.h"
#include "DIP_SystemView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CDIP_SystemDoc

IMPLEMENT_DYNCREATE(CDIP_SystemDoc, CDocument)

BEGIN_MESSAGE_MAP(CDIP_SystemDoc, CDocument)
	ON_COMMAND(ID_FILE_REOPEN, OnFileReopen)
END_MESSAGE_MAP()

// CDIP_SystemDoc construction/destruction

CDIP_SystemDoc::CDIP_SystemDoc()
{
	// Ĭ�ϱ���ɫ����ɫ
	m_refColorBKG = 0x00808080;	
	
	// ��ʼ������
	m_hDIB = NULL;
	m_palDIB = NULL;
	m_sizeDoc = CSize(1, 1);
	m_ptPoint_I = CPoint(0, 0);
	m_ptPoint_II = CPoint(0, 0);
	m_ptLastPos = CPoint(0, 0);
	m_fZoomRatio = 2.0f;
	m_nRgnNum = 0;
	m_bIsFrequency = FALSE;
	m_strCurrentFile = "";

	m_Tracker.m_rect.SetRect (0, 0, 10, 10);
	m_Tracker.m_nStyle = CRectTracker::resizeInside | CRectTracker::dottedLine;

	m_pDIB = new CDIB;
	m_sFileName = "";
	m_sFilePath = "";
	m_bIsDecoding = FALSE;
	m_bJumpSecLine = FALSE;
}

CDIP_SystemDoc::~CDIP_SystemDoc()
{
	// �ж�DIB�����Ƿ����
	if (m_hDIB != NULL)
	{
		// ���DIB����
		::GlobalFree((HGLOBAL)m_hDIB);
	}
	// �����ɫ��
	if (m_palDIB != NULL)
		delete m_palDIB;
	if (m_pDIB != NULL)
		delete m_pDIB;
}

BOOL CDIP_SystemDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	m_strCurrentFile = "";

	return TRUE;
}

// CDIP_SystemDoc serialization

void CDIP_SystemDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

// CDIP_SystemDoc diagnostics

#ifdef _DEBUG
void CDIP_SystemDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDIP_SystemDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CDIP_SystemDoc commands

//	���´��ĵ�						 
void CDIP_SystemDoc::OnFileReopen() 
{
 	// ���´�ͼ�񣬷��������޸�
 
	// �жϵ�ǰͼ���Ƿ��Ѿ����Ķ�
	if (IsModified())
	{
		// �ò�������ʧ���е�ǰ���޸�
		if (MessageBox(NULL, L"���´�ͼ�񽫶�ʧ���иĶ����Ƿ������", L"ϵͳ��ʾ", MB_ICONQUESTION | MB_YESNO) == IDNO)
		{
			// �û�ȡ��������ֱ�ӷ���
			return;
		}
	}
	
	CFile file;
 	CFileException fe;
 
 	CString strPathName;
  	
	// ��ȡ��ǰ�ļ�·��
 	strPathName = GetPathName();
 
 	// ���´��ļ�
 	if (!file.Open(strPathName, CFile::modeRead | CFile::shareDenyWrite, &fe))
 	{
 		// ʧ��
 		ReportSaveLoadException(strPathName, &fe, FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
		
 		return;
 	}
 	
 	
 	BeginWaitCursor();
 	
 	// ���Ե���ReadDIBFile()��ȡͼ��
	BOOL Status = FALSE;
 	TRY
 	{
		Status = m_pDIB->Read(file);
 	}
 	CATCH (CFileException, eLoad)
 	{
 		// ��ȡʧ��
 		file.Abort();
 		
 		EndWaitCursor();
 		
 		// ����ʧ��
 		ReportSaveLoadException(strPathName, eLoad,	FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
 		
 		
 		return;
 	}
 	END_CATCH
 	
 	// ��ʼ��DIB
 	InitDIBData();
 	
 	// �ж϶�ȡ�ļ��Ƿ�ɹ�
	if (!Status)
 	{
 		// ʧ�ܣ����ܷ�BMP��ʽ
 		CString strMsg;
 		strMsg = "��ȡͼ��ʱ���������Ƿ�BMP��ʽ��ͼ���ļ���";
 		
 		// ��ʾ����
 		MessageBox(NULL, strMsg, L"ϵͳ��ʾ", MB_ICONINFORMATION | MB_OK);

 		EndWaitCursor();
 		
 		return;
 	}
 	
 	// ��ʼ������ΪFALSE
 	SetModifiedFlag(FALSE);
 	
 	// ˢ��
 	UpdateAllViews(NULL);
  	
 	EndWaitCursor();
 	
 	return;
}

//	���ĵ�							 
BOOL CDIP_SystemDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	m_strCurrentFile = CString(lpszPathName);
	if (   m_strCurrentFile.Right(4) == ".pgm" || m_strCurrentFile.Right(4) == ".PGM"
		|| m_strCurrentFile.Right(4) == ".pbm" || m_strCurrentFile.Right(4) == ".PBM"
		|| m_strCurrentFile.Right(4) == ".ppm" || m_strCurrentFile.Right(4) == ".PPM")
	{
		return OpenPXMFile(lpszPathName);
	}

	// ����ĵ������е�����
	DeleteContents();
	BeginWaitCursor();

	// ���Ե���Read()��ȡͼ��
	BOOL Status = TRUE;
	Status = m_pDIB->Read(m_strCurrentFile);
	// �ж϶�ȡ�ļ��Ƿ�ɹ�
	if (!Status)
	{
		CString strMsg;
		strMsg = "��ȡͼ��ʱ����";
		MessageBox(NULL, strMsg, L"ϵͳ��ʾ", MB_ICONINFORMATION | MB_OK);
		return FALSE;
	}
	// �����ĵ���С
	SetDocSize();
	EndWaitCursor();
	// �����ļ�·����ӵ����ʹ�ã�MRU�����ļ��б�
	if (!m_bIsDecoding)
	{
		SetPathName(lpszPathName);
	}
	// ��ʼ���ͱ��ΪFALSE
	SetModifiedFlag(FALSE);
	m_sFileName = m_strTitle;
	m_sFilePath = m_strPathName;

	CDIP_SystemView* pView = (CDIP_SystemView*)GetView(RUNTIME_CLASS(CDIP_SystemView));
	ASSERT(pView != NULL);
	pView->m_PaintRect = CRect(0, 0, m_sizeDoc.cx, m_sizeDoc.cy);

	return TRUE;
}

//	�����ĵ�							 
BOOL CDIP_SystemDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	m_strCurrentFile = CString(lpszPathName);

	// ���Ե���SaveDIB����ͼ��
	BOOL bSuccess = TRUE;
	
	BeginWaitCursor();

	// ���Ա���ͼ��
	bSuccess = m_pDIB->Save(m_strCurrentFile);

	EndWaitCursor();
	
	// ��������ΪFALSE
	SetModifiedFlag(FALSE);

	if (!bSuccess)
	{
		// ����ʧ�ܣ�������������ʽ��DIB�����Զ�ȡ���ǲ��ܱ���
		// ������SaveDIB��������

		CString strMsg;
		strMsg = "�޷�����BMPͼ��";
		
		// ��ʾ����
		MessageBox(NULL, strMsg, L"ϵͳ��ʾ", MB_ICONINFORMATION | MB_OK);
	}
	
	return bSuccess;
}

// HDIBת��ΪCImageͼ��
BOOL CDIP_SystemDoc::HDIB2CImage(HDIB hDIB)
{
	BYTE*  lpDIB = (BYTE*) ::GlobalLock((HGLOBAL)m_hDIB);
	BYTE* pSrc = m_pDIB->FindAddress(lpDIB);
	LONG Width = m_pDIB->Width(lpDIB);
	LONG Height = m_pDIB->Height(lpDIB);
	WORD nNumColors = m_pDIB->NumColors(lpDIB);
	WORD nBitCount = m_pDIB->BitCount(lpDIB);
	int Channel = nBitCount / 8;
	LONG Length = WIDTHBYTES(Width * nBitCount * Height);

	CImage* pImage = new CImage;
	if (Channel == 1)//�Ҷ�ͼ�����⴦��
	{
		pImage->Create(Width, Height, 8);
		RGBQUAD ColorTab[256];
		for (int i = 0; i<256; i++)
		{
			ColorTab[i].rgbBlue = ColorTab[i].rgbGreen = ColorTab[i].rgbRed = i;
		}
		pImage->SetColorTable(0, 256, ColorTab);
	}
	else pImage->Create(Width, Height, 24);
	BYTE* pDst = (BYTE*)pImage->GetBits() + (pImage->GetPitch()*(pImage->GetHeight() - 1));
	memcpy(pDst, pSrc, Length);
	m_pDIB->Clear();
	m_pDIB->m_UndoStack.SetTop(pImage);
	m_pDIB->SetImage(pImage);

	// �ж�ͼ���Ƿ����INT_MAX:2,147,483,647��
	if (m_pDIB->Width(lpDIB) > INT_MAX || m_pDIB->Height(lpDIB) > INT_MAX)
	{
		// ����DIB����
		::GlobalUnlock((HGLOBAL)m_hDIB);

		// �ͷ�DIB����
		::GlobalFree((HGLOBAL)m_hDIB);

		// ����DIBΪ��
		m_hDIB = NULL;

		CString strMsg;
		strMsg = "BMPͼ��̫��";
		
		MessageBox(NULL, strMsg, L"ϵͳ��ʾ", MB_ICONINFORMATION | MB_OK);
		
		return FALSE;
	}

	// �����ĵ���С
	m_sizeDoc = CSize((int)Width, (int)Height);

	// ����DIB����
	::GlobalUnlock((HGLOBAL)m_hDIB);
	return TRUE;
}

// ��������BMPλͼ
BOOL CDIP_SystemDoc::CreateTempFile(CString strFileName)
{
	::DeleteFile(strFileName);
	if (m_pDIB->m_pImage->Save(strFileName) == S_OK)
		return OnOpenDocument(strFileName, 0);
	return FALSE;
}

// ��PGM��ʽͼ��
BOOL CDIP_SystemDoc::OpenPXMFile(LPCTSTR lpszPathName)
{
	char *fStream;
	CFile file;
	ULONGLONG filelength;

	if (!file.Open(lpszPathName, CFile::modeRead | CFile::shareDenyWrite))
		return FALSE;

	//��ȡ�ļ�����
	filelength = file.GetLength();
	fStream = new char[filelength];
	file.Read(fStream, filelength);

	int i = 0, j = 0, k = 0, start = 0, speLine = -1, curLine = 0, tempnum = 0, arg[4] = {0, 0, 0, 0};

	// �����Ƿ���ע����
	bool note = false;
	for (i = 0; curLine<3 && i<filelength; i++)
	{
		if(fStream[i] == '\n')
		{
			curLine++;
			//�Ƿ�Ϊע����
			if (i+1 < filelength && fStream[i+1] == '#')
			{
				speLine = curLine + 1;
				note = true;
				break;
			}
		}
	}

	bool IsPBM = false;
	if (!note)
	{
		for(i = 0; i < filelength; i++)
		{
			//��ʱ�������
			if(fStream[i] >= '0' && fStream[i] <= '9')
				tempnum = tempnum * 10 + fStream[i] - '0';
			if(fStream[i] == '\n' || fStream[i] == '\t' || fStream[i] == ' ')
			{
				arg[j++] = tempnum;
				tempnum = 0;
			}
			if (arg[0] == 1 || arg[0] == 4)
				IsPBM = true;
			if(j == 4 - IsPBM)
				break;
		}
	}
	else
	{
		curLine = 0;
		for(i = 0; i < filelength; i++)
		{
			//��ʱ�������
			if(fStream[i] >= '0' && fStream[i] <= '9')
				tempnum = tempnum * 10 + fStream[i] - '0';
			if(curLine != speLine)
			{
				if(fStream[i] == '\n')
				{
					curLine++;
					//�Ƿ�Ϊע����
					if(curLine != speLine)
					{
						arg[j++] = tempnum;
						tempnum = 0;
					}
					else tempnum = 0;
				}
			}
			else if(fStream[i] == '\n' || fStream[i] == '\t' || fStream[i] == ' ')
			{
				arg[j++] = tempnum;
				tempnum = 0;
			}
			if (arg[0] == 1 || arg[0] == 4)
				IsPBM = true;
			if(j == 4 - IsPBM)
				break;
		}
	}
	start = i + 1;

	int nType = arg[0];
	int nImageWidth = arg[1];
	int nImageHeight = arg[2];
	int nBitCount;
	if (nImageWidth * nImageHeight == 0)
	{
		file.Close();
		delete fStream;
		fStream = NULL;
		return FALSE;
	}

	switch (nType)
	{
	case 1: nBitCount = 8; break;
	case 4: nBitCount = 1; break;
	case 2: case 5: nBitCount = 8; break;
	case 3: case 6: nBitCount = 24;break;
	default:
		AfxMessageBox(L"ͼ���Ƿ�Ϊ��׼��PGM��ʽ��", MB_OK | MB_ICONQUESTION);
		file.Close();
		delete fStream;
		fStream = NULL;
		return FALSE;
	}

	long Rowlen;
	if (nBitCount == 1)
		Rowlen = WIDTHBYTES(nImageWidth * 8);
	else
		Rowlen = WIDTHBYTES(nImageWidth * nBitCount);

	// ����CImage����
	CImage *pImage = new CImage;
	pImage->Create(nImageWidth, nImageHeight, nBitCount);
	BYTE* pBits = (BYTE*)pImage->GetBits() + pImage->GetPitch() * (nImageHeight - 1);

	int x = 0, y = 0;
	i = j = tempnum = 0;
	switch(nType)
	{
	case 4:
		{
			memcpy(pBits, fStream + start, filelength - start);
			break;
		}
	case 5:
	case 6:
		{
			BYTE *pCur = pBits + nImageHeight * Rowlen;
			for(i = start; i < filelength; ++i)
			{
				pCur--;
				*pCur = (BYTE)fStream[i];
			}
			break;
		}
	case 1:
		{
			for(i = start; i < filelength; ++i)
			{
				if(fStream[i] >= '0' && fStream[i] <= '9')
					tempnum = tempnum * 10 + fStream[i] - '0';
				if(fStream[i] == '\t' || fStream[i] == ' ')
				{
					*(pBits + x + y * Rowlen) = 255 * tempnum;
					x++;
					if (tempnum == 1)
					{
						tempnum = 0;
					}
					if (x == nImageWidth)
					{
						x = 0;
						y++;
					}
				}
			}
			break;
		}
	case 2:
	case 3:
		{
			BYTE *pCur = pBits + nImageHeight * Rowlen;
			for(i = start; i < filelength; ++i)
			{
				if(fStream[i] >= '0' && fStream[i] <= '9')
					tempnum = tempnum * 10 + fStream[i] - '0';
				if(fStream[i] == '\t' || fStream[i] == ' ')
				{
					pCur--;
					*pCur = (BYTE)tempnum;
					tempnum = 0;
				}
			}
			break;
		}
	}

	// ������ɫ��
	switch (nBitCount)
	{
	case 1:
		{
			RGBQUAD ColorTab[2];
			ColorTab[0].rgbRed = ColorTab[0].rgbGreen = ColorTab[0].rgbBlue = 0;
			ColorTab[0].rgbReserved = 0;
			ColorTab[1].rgbRed = ColorTab[1].rgbGreen = ColorTab[1].rgbBlue = 255;
			ColorTab[1].rgbReserved = 0;
			pImage->SetColorTable(0, 2, ColorTab);
			break;
		}
	case 8:
		{
			RGBQUAD ColorTab[256];
			for (i = 0; i < 256; i++)
			{
				ColorTab[i].rgbRed = ColorTab[i].rgbGreen = ColorTab[i].rgbBlue = i;
				ColorTab[i].rgbReserved = 0;
			}
			pImage->SetColorTable(0, 256, ColorTab);
			break;
		}
	default:
		break;
	}

	file.Close();
	delete fStream;
	fStream = NULL;

	// ��ָ��ѹ���ջ
	m_pDIB->m_UndoStack.Push(pImage);
	m_pDIB->SetImage(pImage);
	// ������λͼת��Ϊ8λɫͼ��
	if (nBitCount == 1)
		m_pDIB->BinaryMap2Other(8);
	SetDocSize();
	if (!m_bIsDecoding)
	{
		SetPathName(lpszPathName);
	}
	SetModifiedFlag(FALSE);
	m_sFileName = m_strTitle;
	m_sFilePath = m_strPathName;

	CDIP_SystemView* pView = (CDIP_SystemView*)GetView(RUNTIME_CLASS(CDIP_SystemView));
	ASSERT(pView != NULL);
	pView->m_PaintRect = CRect(0, 0, m_sizeDoc.cx, m_sizeDoc.cy);
	return TRUE;
}

///////////////////////////////////////////
//	��ʼ��DIB����						 //
///////////////////////////////////////////

// �����ĵ���С,Ĭ��Ϊͼ���С
void CDIP_SystemDoc::SetDocSize()
{
	m_sizeDoc = CSize((int) m_pDIB->Width(), (int) m_pDIB->Height());
	GetView(RUNTIME_CLASS(CDIP_SystemView))->SetPaintRect(m_sizeDoc);
	GetView(RUNTIME_CLASS(CDIP_SystemView))->SetScrollSizes(MM_TEXT, m_sizeDoc);
}

// ���أ������ĵ���С������cx��cy
void CDIP_SystemDoc::SetDocSize(int cx, int cy)
{
	m_sizeDoc.cx = cx;
	m_sizeDoc.cy = cy;
	GetView(RUNTIME_CLASS(CDIP_SystemView))->SetPaintRect(m_sizeDoc);
	GetView(RUNTIME_CLASS(CDIP_SystemView))->SetScrollSizes(MM_TEXT, m_sizeDoc);
}

///////////////////////////////////////////
//	��ʼ��DIB����						 //
///////////////////////////////////////////
void CDIP_SystemDoc::InitDIBData()
{
	// �жϵ�ɫ���Ƿ�Ϊ��
	if (m_palDIB != NULL)
	{
		// ɾ����ɫ�����
		delete m_palDIB;

		// ���õ�ɫ��Ϊ��
		m_palDIB = NULL;
	}

	// ���DIB����Ϊ�գ�ֱ�ӷ���
	if (m_hDIB == NULL)
	{
		
		return;
	}

	BYTE*  lpDIB = (BYTE* ) ::GlobalLock((HGLOBAL)m_hDIB);

	// �ж�ͼ���Ƿ����INT_MAX:2,147,483,647��
	if (m_pDIB->Width(lpDIB) > INT_MAX || m_pDIB->Height(lpDIB) > INT_MAX)
	{
		// ����DIB����
		::GlobalUnlock((HGLOBAL)m_hDIB);

		// �ͷ�DIB����
		::GlobalFree((HGLOBAL)m_hDIB);

		// ����DIBΪ��
		m_hDIB = NULL;

		
		return;
	}

	// ����DIB����
	::GlobalUnlock((HGLOBAL)m_hDIB);

	// �����µ�ɫ��
	m_palDIB = new CPalette;

	// �ж��Ƿ񴴽��ɹ�
	if (m_palDIB == NULL)
	{
		// ʧ�ܣ��������ڴ治��
		::GlobalFree((HGLOBAL)m_hDIB);

		// ����DIB����Ϊ��
		m_hDIB = NULL;

		
		return;
	}

	// ����CreateDIBPalette��������ɫ��
	if (m_pDIB->CreatePalette(m_hDIB, m_palDIB) == NULL)
	{
		// �գ����ܸ�DIB����û�е�ɫ��

		// ɾ��
		delete m_palDIB;

		// ����Ϊ��
		m_palDIB = NULL;

		
		return;
	}
}

///////////////////////////////////////////
//	�滻DIB���ڹ���ճ�����õ��ú���		 //
///////////////////////////////////////////
void CDIP_SystemDoc::ReplaceHDIB(HDIB hDIB)
{
	// �ж�DIB�Ƿ�Ϊ��
	if (m_hDIB != NULL)
	{
		// �ǿգ������
		::GlobalFree((HGLOBAL)m_hDIB);
	}

	// �滻���µ�DIB����
	m_hDIB = hDIB;
}

///////////////////////////////////////////
//	���ĵ�							 //
///////////////////////////////////////////
BOOL CDIP_SystemDoc::OnOpenDocument(LPCTSTR lpszPathName, BOOL bReserved)
{
	CFile file;
	CFileException fe;
	m_strCurrentFile = CString(lpszPathName);

	// ���ļ�
	if (!file.Open(lpszPathName, CFile::modeRead | CFile::shareDenyWrite, &fe))
	{
		ReportSaveLoadException(lpszPathName, &fe, FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
		return FALSE;
	}

	// ����ĵ������е�����
	DeleteContents();
	BeginWaitCursor();

	// ���Ե���ReadDIBFile()��ȡͼ�񣬲������쳣
	TRY
	{
		m_hDIB = m_pDIB->Read(file, 0);
	}
	CATCH(CFileException, eLoad)
	{
		// ��ȡʧ��
		file.Abort();
		EndWaitCursor();
		ReportSaveLoadException(lpszPathName, eLoad, FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
		m_hDIB = NULL;
		return FALSE;
	}
	END_CATCH

	InitDIBData();
	EndWaitCursor();
	if (m_hDIB == NULL)
		return FALSE;
	SetTitle(L"����λͼ");
	return TRUE;
}

///////////////////////////////////////////
//	�����ĵ�							 //
///////////////////////////////////////////
BOOL CDIP_SystemDoc::OnSaveDocument(LPCTSTR lpszPathName, BOOL bReserved)
{
	CFile file;
	CFileException fe;

	m_strCurrentFile = CString(lpszPathName);

	// ���ļ�
	if (!file.Open(lpszPathName, CFile::modeCreate |
		CFile::modeReadWrite | CFile::shareExclusive, &fe))
	{
		// ʧ��
		ReportSaveLoadException(lpszPathName, &fe, TRUE, AFX_IDP_INVALID_FILENAME);

		return FALSE;
	}

	// ���Ե���SaveDIB����ͼ��
	BOOL bSuccess = FALSE;
	TRY
	{
		
		BeginWaitCursor();

		// ���Ա���ͼ��
		bSuccess = m_pDIB->Save(m_hDIB, file);

		// �ر��ļ�
		file.Close();
	}
	CATCH(CException, eSave)
	{
		// ʧ��
		file.Abort();

		EndWaitCursor();
		ReportSaveLoadException(lpszPathName, eSave, TRUE, AFX_IDP_FAILED_TO_SAVE_DOC);

		return FALSE;
	}
	END_CATCH

	EndWaitCursor();

	// ��������ΪFALSE
	SetModifiedFlag(FALSE);

	if (!bSuccess)
	{
		// ����ʧ�ܣ�������������ʽ��DIB�����Զ�ȡ���ǲ��ܱ���
		// ������SaveDIB��������

		CString strMsg;
		strMsg = "�޷�����BMPͼ��";

		// ��ʾ����
		MessageBox(NULL, strMsg, L"ϵͳ��ʾ", MB_ICONINFORMATION | MB_OK);
	}

	return bSuccess;
}

// ���ĵ���ȡ��ͼָ��
// ʹ�����£� CYourView* pView=(CYourView*)GetView(RUNTIME_CLASS(CYourView));
CDIP_SystemView* CDIP_SystemDoc::GetView(CRuntimeClass* pClass)
{

	CView *pView = NULL; 
	POSITION pos = GetFirstViewPosition(); 
	while(pos != NULL) 
	{ 
		pView = GetNextView(pos); 
		if(!pView->IsKindOf(pClass)) 
			break; 
	} 
	if(!pView->IsKindOf(pClass)) 
	{ 
		AfxMessageBox(L"Can not locate the View."); 
		return NULL; 
	} 
	return (CDIP_SystemView*)pView; 
}

void CDIP_SystemDoc::OnCloseDocument()
{
	// TODO: �ڴ����ר�ô����/����û���

	CDocument::OnCloseDocument();
}