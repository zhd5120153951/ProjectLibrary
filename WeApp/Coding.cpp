// ************************************************************************
//  �ļ�����Coding.cpp
//
//  ͼ�����ѹ������ѹ�������⣺
//
//	LoadJPG()			- װ��JPEGͼ�󣨽�ѹ�ã�
//	SaveJPG()			- ����JPEGͼ��ѹ���ã�
//	SaveBMP()			- ����BMPͼ���ļ�����ѹ�ã�
//	LoadBMP()			- ���ļ�װ��BMPͼ��ѹ���ã�
//  DIBToPCX()			- ��DIB����ΪPCX�ļ�
//  PCXToDIB()			- ��ȡPCX�ļ�
//	Huffman()			- ����������
//	Shannon_Fannon()	- ��ũ-��ŵ����
//	DIBToGIF()			- ��DIB���浽GIF�ļ�
//	LZW_Encode()		- ��ͼ�����LZW����
//	OutputCode()		- ΪGIF-LZW�㷨���һ������
//	GIFToDIB()			- ��ȡGIF��DIB
//	LZW_Decode()		- ��LZW������н���
//	ReadGIF()			- ��ȡָ��GIF�ļ��е�ͼ�����
//
//*************************************************************************

#include "stdafx.h"
#include "dip_system.h"
#include "Coding.h"
#include "JPEGfile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//	Dib�ļ�ͷ��־���ַ���"BM"��дDIBʱ�õ��ó�����
#define DIB_HEADER_MARKER   ((WORD) ('M' << 8) | 'B')

/////////////////////////////////////////////////////////////////////////////
// CCoding

CCoding::CCoding()
{
}

CCoding::CCoding(CDIB* pDIB)
{
	m_pDIB = pDIB;
}

CCoding::~CCoding()
{
}

BEGIN_MESSAGE_MAP(CCoding, CWnd)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCoding message handlers

/*************************************************************************
 *
 * �������ƣ�
 *   SaveJPG()
 *
 * ����:
 *   CDIP_SystemDoc* pDoc	- �ĵ�ָ��
 *	 CString fileName		- �����ļ�·��
 *
 * ����ֵ:
 *   void			    - �޷���ֵ
 *
 * ˵��:
 *   �ú�����ͼ�����JPEG����ѹ��
 *
 ************************************************************************/
void CCoding::SaveJPG(CDIP_SystemDoc* pDoc, CString fileName)
{
	// ��ʱ����
	BYTE *buf;
	buf = NULL;

	// ��ȡ�ĵ��ߴ�
	CSize size = pDoc->GetDocSize();
	UINT width = size.cx;
	UINT height = size.cy;

	buf = LoadBMP(pDoc->m_strCurrentFile, &width, &height);

	// ѹ��λͼ��JPEG�ļ�������������������ѹ��������1 - 100��
	JpegFile::RGBToJpegFile(fileName, buf, width, height, TRUE, 50);

	delete[] buf;
}

/*************************************************************************
 *
 * �������ƣ�
 *   LoadJPG()
 *
 * ����:
 *   CDIP_SystemDoc* pDoc	- �ĵ�ָ��
 *	 CString fileName		- װ���ļ�·��
 *
 * ����ֵ:
 *   void			    - �޷���ֵ
 *
 * ˵��:
 *   �ú�����ͼ�����JPEG�����ѹ��
 *
 ************************************************************************/
void CCoding::LoadJPG(CDIP_SystemDoc* pDoc, CString filePath)
{
	// ����λͼ
	CString sNewPath = L"����ͼ��";

	// ��ʱ����
	BYTE *buf;
	buf = NULL;

	// JPEGͼ��߶ȡ����
	UINT width = 0;
	UINT height = 0;

	// ������ʱ����
	buf = JpegFile::JpegFileToRGB(filePath, &width, &height);

	// ������������
	JpegFile::BGRFromRGB(buf, width, height);

	// ��ֱ��ת��ʾ
	JpegFile::VertFlipBuf(buf, width * 3, height);

	// �����ѹ������ݵ���ʱλͼ�ļ�
	SaveBMP(sNewPath, buf, width, height);

	// ɾ��ָ��
	delete[] buf;

	// װ����ʱλͼ�ļ�
	pDoc->SetTitle(sNewPath);
	pDoc->m_bIsDecoding = TRUE;
	pDoc->OnOpenDocument(sNewPath);

	// ɾ����ʱ�ļ�
	::DeleteFile(sNewPath);

	// ��ʼ�����ΪFALSE
	pDoc->SetModifiedFlag(TRUE);
}

/*************************************************************************
 *
 * �������ƣ�
 *   SaveBMP()
 *
 * ����:
 *	 CString fileName		- �����ļ�·��
 *	 BYTE* buf				- BGR����
 *	 UINT width				- ���
 *	 UINT height			- �߶�
 *
 * ����ֵ:
 *   void			    - �޷���ֵ
 *
 * ˵��:
 *   �ú�����BMPͼ����ڴ汣�浽�ļ�
 *
 ************************************************************************/
void CCoding::SaveBMP(CString fileName,	BYTE * buf,	UINT width,	UINT height)
{
	// λͼ�ļ�ͷ
	char m1 = 'B';
	char m2 = 'M';
	short res1 = 0;
    short res2 = 0;
    long pixoff = 54;

	// ���
	DWORD widthDW = WIDTHBYTES(width * 24);

	// �ļ�����
	long bmfsize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + widthDW * height;	
	
	// �ļ�ָ��
	long byteswritten = 0;

	// λͼ��Ϣͷ
	BITMAPINFOHEADER header;
  	header.biSize = 40; 		   		    // ͷ��С
	header.biWidth = width;
	header.biHeight = height;
	header.biPlanes = 1;
	header.biBitCount = 24;					// 24λ
	header.biCompression = BI_RGB;			// ��ѹ��
	header.biSizeImage = 0;
	header.biXPelsPerMeter = 0;
	header.biYPelsPerMeter = 0;
	header.biClrUsed = 0;
	header.biClrImportant = 0;

	// ������д��ʽ���ļ�
	FILE *fp;	
	USES_CONVERSION;
	fp = fopen(W2A(fileName), "wb");

	// д�ļ�ͷ
	fwrite((BYTE*)&(m1), 1, 1, fp); 
	byteswritten += 1;
	fwrite((BYTE*)&(m2), 1, 1, fp); 
	byteswritten += 1;
	fwrite((long*)&(bmfsize), 4, 1, fp);
	byteswritten += 4;
	fwrite((int*)&(res1), 2, 1, fp);
	byteswritten += 2;
	fwrite((int*)&(res2), 2, 1, fp);
	byteswritten += 2;
	fwrite((long*)&(pixoff), 4, 1, fp);
	byteswritten += 4;

	// дλͼ��Ϣͷ
	fwrite((BITMAPINFOHEADER *)&header, sizeof(BITMAPINFOHEADER), 1, fp);
	
	// �ƶ�ָ��
	byteswritten += sizeof(BITMAPINFOHEADER);
	
	// ��ʱ����
	long row = 0;
	long rowidx;
	long row_size;
    long rc;

	// �д�С
	row_size = header.biWidth * 3;
	
	// ����λͼ����
	for (row = 0; row < header.biHeight; row++)
	{
		rowidx = (long unsigned)row * row_size;						      

		// дһ��
		rc = fwrite((void *)(buf + rowidx), row_size, 1, fp);
		if (rc != 1) 
			break;
		byteswritten += row_size;	

		for (DWORD count = row_size; count < widthDW; count++) 
		{
			char dummy = 0;
			fwrite(&dummy, 1, 1, fp);
			byteswritten++;							  
		}
	}

	// �ر��ļ�
	fclose(fp);
}

/*************************************************************************
 *
 * �������ƣ�
 *   LoadBMP()
 *
 * ����:
 *	 CString fileName		- װ��·��
 *	 UINT width				- ���
 *	 UINT height			- �߶�
 *
 * ����ֵ:
 *   BYTE*					- ��ȡͼ�󻺴��׵�ַ
 *
 * ˵��:
 *   �ú�����BMPͼ����ļ�װ�ص��ڴ�
 *
 ************************************************************************/
BYTE* CCoding::LoadBMP(CString fileName, UINT *width, UINT *height)
{
	BITMAP inBM;
	BYTE m1,m2;
    long filesize;
    short res1,res2;
    long pixoff;
    long bmisize;                    
    long compression;
    unsigned long sizeimage;
    long xscale, yscale;
    long colors;
    long impcol;
    
	// ��ʼ��
	BYTE *outBuf = NULL;
	*width = 0;
	*height = 0;
	DWORD m_bytesRead = 0;

	// �Զ����ƶ���ʽ���ļ�
	FILE *fp;
	USES_CONVERSION;
	fp = fopen(W2A(fileName), "rb");
	
	// ��ȡ�ļ�
	if (fp != NULL)
	{
		// ����BMP��־
	    long rc;
		rc = fread((BYTE*)&(m1), 1, 1,fp);
		m_bytesRead += 1;
		if (rc == -1) 
		{
			fclose(fp); 
			return NULL;
		}

		rc = fread((BYTE*)&(m2), 1, 1,fp);
		m_bytesRead += 1;
		if ((m1!='B') || (m2!='M')) 
		{
			fclose(fp);
			return NULL;
        }
        
		// ���ļ�ͷ��Ϣ
		rc = fread((long*)&(filesize), 4, 1, fp); 
		m_bytesRead += 4;

		if (rc != 1)
		{
			fclose(fp); 
			return NULL;
		}

		rc = fread((int*)&(res1), 2, 1, fp); 
		m_bytesRead += 2;

		if (rc != 1) 
		{
			fclose(fp); 
			return NULL;
		}

		rc = fread((int*)&(res2), 2, 1, fp); 
		m_bytesRead += 2;
		if (rc != 1)
		{
			fclose(fp); 
			return NULL;
		}

		rc = fread((long*)&(pixoff), 4, 1, fp);
		m_bytesRead += 4;
		if (rc != 1) 
		{ 
			fclose(fp); 
			return NULL;
		}

		rc = fread((long*)&(bmisize), 4, 1, fp);
		m_bytesRead += 4;
		if (rc != 1) 
		{
			fclose(fp); 
			return NULL;
		}

		rc = fread((long*)&(inBM.bmWidth), 4, 1, fp);
		m_bytesRead += 4;
		if (rc != 1) 
		{
			fclose(fp);
			return NULL;
		}

		rc = fread((long*)&(inBM.bmHeight), 4, 1, fp);
		m_bytesRead += 4;
		if (rc != 1)
		{
			fclose(fp); 
			return NULL;
		}

		rc = fread((int*)&(inBM.bmPlanes), 2, 1, fp); 
		m_bytesRead += 2;
		if (rc != 1) 
		{
			fclose(fp);
			return NULL;
		}

		rc = fread((int*)&(inBM.bmBitsPixel), 2, 1, fp);
		m_bytesRead += 2;
		if (rc != 1)
		{
			fclose(fp); 
			return NULL;
		}

		rc = fread((long*)&(compression), 4, 1, fp);
		m_bytesRead += 4;
		if (rc != 1) 
		{
			fclose(fp);
			return NULL;
		}

		rc = fread((long*)&(sizeimage), 4, 1, fp);
		m_bytesRead += 4;
		if (rc != 1) 
		{
			fclose(fp); 
			return NULL;
		}

		rc = fread((long*)&(xscale), 4, 1, fp);
		m_bytesRead += 4;

		if (rc != 1) 
		{
			fclose(fp);
			return NULL;
		}

		rc = fread((long*)&(yscale), 4, 1, fp); 
		m_bytesRead += 4;
		if (rc != 1)
		{
			fclose(fp);
			return NULL;
		}

		rc = fread((long*)&(colors), 4, 1, fp);
		m_bytesRead += 4;
		if (rc != 1)
		{
			fclose(fp); 
			return NULL;
		}

		rc = fread((long*)&(impcol), 4, 1, fp);
		m_bytesRead += 4;
		if (rc != 1)
		{
			fclose(fp);
			return NULL;
		}

		// ��RLE�ļ���������
		if (compression != BI_RGB) 
		{
	    	fclose(fp);
	    	return NULL;
	    }

		if (colors == 0) 
			colors = 1 << inBM.bmBitsPixel;
		
		// ��ȡ��ɫ��
		RGBQUAD *colormap = NULL;

		if ((long)m_bytesRead > pixoff) 
		{
			fclose(fp);
			delete [] colormap;
			fclose(fp);
			return NULL;
		}

		while ((long)m_bytesRead < pixoff) 
		{
			char dummy;
			fread(&dummy,1,1,fp);
			m_bytesRead++;
		}

		int w = inBM.bmWidth;
		int h = inBM.bmHeight;

		// �����������
		*width = w;
		*height = h;

		long row_size = w * 3;

		long bufsize = (long)w * 3 * (long)h;

		// �����ڴ�
		outBuf = (BYTE*) new BYTE [bufsize];
		if (outBuf != NULL) 
		{
			long row = 0;
			long rowOffset = 0;

			// ��ȡ��������
			for (row = inBM.bmHeight - 1; row >= 0;row--) 
			{
				rowOffset = (long unsigned)row*row_size;						      

				if (inBM.bmBitsPixel == 24)
				{
					for (int col = 0; col < w; col++) 
					{
						long offset = col * 3;
						char pixel[3];

						if (fread((void  *)(pixel), 1, 3, fp) == 3)
						{
							//����R��B��Ϣ
							*(outBuf + rowOffset + offset + 0) = pixel[2];		// r
							*(outBuf + rowOffset + offset + 1) = pixel[1];		// g
							*(outBuf + rowOffset + offset + 2) = pixel[0];		// b
						}

					}

					m_bytesRead += row_size;
					
					while ((m_bytesRead - pixoff) & 3)
					{
						char dummy;
						if (fread(&dummy, 1, 1, fp) != 1)
						{
							delete [] outBuf;
							fclose(fp);
							return NULL;
						}
						m_bytesRead++;
					}
				} 
			}
		}

		if (colormap) 
			delete [] colormap;

		fclose(fp);
    }
	return outBuf;
}

/*************************************************************************
 *
 * �������ƣ�
 *   DIBToPCX()
 *
 * ����:
 *   CString strPath	    - Ҫ������ļ�·��
 *
 * ����ֵ:
 *   BOOL               - �ɹ�����True�����򷵻�False��
 *
 * ˵��:
 *   �ú�����DIB����ΪPCX�ļ���
 *
 *************************************************************************/
BOOL CCoding::DIBToPCX(CString strPath)
{
	
	LONG	i;
	LONG	j;
	LONG	k;
	
	// �м����
	BYTE	bChar1;
	BYTE	bChar2;
	
	// ָ��Դͼ�����ص�ָ��
	BYTE*	lpSrc;
	
	// ָ������ͼ�����ݵ�ָ��
	BYTE*	lpDst;
	
	// �ظ����ؼ���
	int		iCount;
	
	// ����˳��B��G��R
	int	RGB[3] = {2, 1, 0};

	// ���ļ�
	CFile file;
	file.Open(strPath, CFile::modeCreate | CFile::modeReadWrite);
	
	// ��������ʹ�õ��ֽ���
	DWORD	dwBuffUsed;
	
	BYTE* lpDIBBits = m_pDIB->FindAddress();
	
	if (m_pDIB->Channel() < 3)
	{
		MessageBox(L"���Ƚ���ת��Ϊ24λɫ(��32λɫ)���ٽ��д���", L"ϵͳ��ʾ" , MB_ICONINFORMATION | MB_OK);
		return FALSE;
	}
	
	BeginWaitCursor();
	
	LONG lWidth = m_pDIB->Width();
	
	LONG lHeight = m_pDIB->Height();
	
	LONG lLineBytes = m_pDIB->RowLen();
	
	//*************************************************************************
	// PCX�ļ�ͷ
	PCXHEADER pcxHdr;
	
	// ���ļ�ͷ��ֵ
	
	// PCX��ʶ��
	pcxHdr.bManufacturer = 0x0A;
	
	// PCX�汾��
	pcxHdr.bVersion = 5;
	
	// PCX���뷽ʽ��1��ʾRLE���룩
	pcxHdr.bEncoding = 1;
	
	// ����λ����24λɫΪ8λ��
	pcxHdr.bBpp = 8;
	
	// ͼ���������Ļ�����Ͻ�X���꣨������Ϊ��λ��
	pcxHdr.wLeft = 0;
	
	// ͼ���������Ļ�����Ͻ�Y���꣨������Ϊ��λ��
	pcxHdr.wTop = 0;
	
	// ͼ���������Ļ�����½�X���꣨������Ϊ��λ��
	pcxHdr.wRight = (WORD)(lWidth - 1);
	
	// ͼ���������Ļ�����½�Y���꣨������Ϊ��λ��
	pcxHdr.wBottom = (WORD)(lHeight - 1);
		
	// ͼ���ˮƽ�ֱ���
	pcxHdr.wXResolution = (WORD)lWidth;
	
	// ͼ��Ĵ�ֱ�ֱ���
	pcxHdr.wYResolution = (WORD)lHeight;

	// �������趨Ϊ0��
	pcxHdr.bReserved = 0;
	
	// ͼ��ɫ��ƽ����Ŀ
	pcxHdr.bPlanes = 3;
	
	// ͼ��Ŀ�ȣ��ֽ�Ϊ��λ��������Ϊż����
	pcxHdr.wLineBytes = (WORD)lWidth;
	
	// ͼ���ɫ������ͣ�1��ʾ��ɫ���ߵ�ɫͼ��2��ʾͼ���ǻҶ�ͼ��
	pcxHdr.wPaletteType = 1;
	
	// ������ͼ�����Ļ��ȣ�����Ϊ��λ��
	pcxHdr.wSrcWidth = 0;
	
	// ������ͼ�����Ļ�߶ȣ�����Ϊ��λ��
	pcxHdr.wSrcDepth = 0;
	
	// ������ȡֵ�趨Ϊ0��
	for (i = 0; i < 54; i ++)
	{
		pcxHdr.bFiller[i] = 0;
	}
	
	// д���ļ�ͷ
	file.Write((BYTE*)&pcxHdr, sizeof(PCXHEADER));
	
	//*******************************************************************************
	// ��ʼ����
	
	// ����һƬ������(2��ԭʼͼ���С)�Ա��������
	lpDst = new BYTE[lHeight * lLineBytes * 2];
	
	// ָ����ǰ�Ѿ����˶��ٻ��������ֽ�����
	dwBuffUsed = 0;
	
	// ÿ��
	for (i = 0; i < lHeight; i++)
	{
		for (k = 0; k < 3; k++)
		{
			// ָ��DIB��i�У���0�����ص�ָ��
			lpSrc = (BYTE*)(lpDIBBits + lLineBytes * (lHeight - 1 - i) + RGB[k]);
			
			// ��bChar1��ֵ
			bChar1 = *lpSrc;
			
			// ����iCountΪ1
			iCount = 1;
		
			// ʣ����
			for (j = 3; j < lLineBytes; j += 3)
			{
				// ָ��DIB��i�У���j�����ص�ָ��
				bChar2 = *(lpSrc + j);

				// �ж��Ƿ��bChar1��ͬ����iCount < 63
				if ((bChar1 == bChar2) && (iCount < 63))
				{
					// ��ͬ��������1
					iCount ++;
				}	
				else// ��ͬ������iCount = 63
				{
					// д�뻺����
					if ((iCount > 1) || (bChar1 >= 0xC0))
					{
						// �����볤��Ϣ
						lpDst[dwBuffUsed] = iCount | 0xC0;
					
						// ����bChar1
						lpDst[dwBuffUsed + 1] = bChar1;
					
						// ����dwBuffUsed
						dwBuffUsed += 2;
					}
					else
					{
						// ֱ�ӱ����ֵ
						lpDst[dwBuffUsed] = bChar1;
					
						// ����dwBuffUsed
						dwBuffUsed ++;
					}
				
					// ���¸�bChar1��ֵ
					bChar1 = bChar2;
				
					// ����iCountΪ1
					iCount = 1;
				}
			}
	
			// ����ÿ�����һ���ֱ���
			if ((iCount > 1) || (bChar1 >= 0xC0))
			{
				// �����볤��Ϣ
				lpDst[dwBuffUsed] = iCount | 0xC0;
			
				// ����bChar1
				lpDst[dwBuffUsed + 1] = bChar1;
				
				// ����dwBuffUsed
				dwBuffUsed += 2;
			}
			else
			{
				// ֱ�ӱ����ֵ
				lpDst[dwBuffUsed] = bChar1;
			
				// ����dwBuffUsed
				dwBuffUsed ++;
			}
		}
	}

	// д�������
	file.Write((BYTE*)lpDst, dwBuffUsed);
	
	// �ͷ��ڴ�
	delete[] lpDst;
	
	// �ر��ļ�
	file.Close();

	EndWaitCursor();

	return TRUE;
}

/*************************************************************************
 *
 * �������ƣ�
 *   PCXToDIB()
 *
 * ����:
 *   CString strPath	    - Ҫװ�ص��ļ�·��
 *
 * ����ֵ:
 *   HDIB		            - �ɹ�����DIB�ľ�������򷵻�NULL��
 *
 * ˵��:
 *   �ú�������ȡָ����PCX�ļ�����ȡ�Ľ��������DIB�����С�
 *
 *************************************************************************/
HDIB CCoding::PCXToDIB(CString strPath)
{
	// PCX�ļ�ͷ
	PCXHEADER pcxHdr;
	
	// DIB��С���ֽ�����
	DWORD	dwDIBSize;
	
	// DIB���
	HDIB	hDIB;
	
	// DIBָ��
	BYTE*	pDIB;
	
	LONG	i;
	LONG	j;
	LONG	k;
	LONG    l;

	// �ظ����ؼ���
	int		iCount;
	
	// ����RGB˳��
	int	RGB[3] = {2, 1, 0};
	
	// �м����
	BYTE	bChar;
	
	// ָ��Դͼ�����ص�ָ��
	BYTE *	lpSrc;
	
	// ָ������ͼ�����ݵ�ָ��
	BYTE *	lpDst;
	
	// ��ʱָ��
	BYTE *	lpTemp;

	// ���ļ�
	CFile file;
	file.Open(strPath, CFile::modeReadWrite);
	
	// ���Զ�ȡPCX�ļ�ͷ
	if (file.Read((BYTE*)&pcxHdr, sizeof(PCXHEADER)) != sizeof(PCXHEADER))
	{
		// ��С���ԣ�����NULL��
		return NULL;
	}
	
	// �ж��Ƿ���24λɫPCX�ļ�������һ���ֽ��Ƿ���0x0A��
	if ((pcxHdr.bManufacturer != 0x0A) || (pcxHdr.bBpp != 8) || (pcxHdr.bPlanes != 3))
	{
		// ��24λɫPCX�ļ�������NULL��
		return NULL;
	}
	
	// ��ȡͼ��߶�
	LONG lHeight = pcxHdr.wBottom - pcxHdr.wTop + 1;
	
	// ��ȡͼ����
	LONG lWidth  = pcxHdr.wRight - pcxHdr.wLeft + 1;
	
	LONG lLineBytes = m_pDIB->RowLen();
	
	// ����DIB���ȣ��ֽڣ�
	dwDIBSize = sizeof(BITMAPINFOHEADER) + 1024 + lHeight * lLineBytes;
	
	// ΪDIB�����ڴ�
	hDIB = (HDIB) ::GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, dwDIBSize);
	if (hDIB == 0)
	{
		// �ڴ����ʧ�ܣ�����NULL��
		return NULL;
	}
	
	// ����
	pDIB = (BYTE*) ::GlobalLock((HGLOBAL) hDIB);
	
	// ָ��BITMAPINFOHEADER��ָ��
	LPBITMAPINFOHEADER lpBI;
	
	// ��ֵ
	lpBI = (LPBITMAPINFOHEADER) pDIB;
	
	// ��lpBI��Ա��ֵ
	lpBI->biSize = 40;
	lpBI->biWidth = lWidth;
	lpBI->biHeight = lHeight;
	lpBI->biPlanes = 1;
	lpBI->biBitCount = 24;
	lpBI->biCompression = BI_RGB;
	lpBI->biSizeImage = lHeight * lLineBytes;
	lpBI->biXPelsPerMeter = pcxHdr.wXResolution;
	lpBI->biYPelsPerMeter = pcxHdr.wYResolution;
	lpBI->biClrUsed = 0;
	lpBI->biClrImportant = 0;
	
	// �����ڴ��Զ�ȡ����������
	lpSrc = new BYTE[file.GetLength() - sizeof(PCXHEADER) - 769];
	lpTemp = lpSrc;
	
	// ��ȡ����������
	if (file.Read(lpSrc, file.GetLength() - sizeof(PCXHEADER) - 769) !=
		file.GetLength() - sizeof(PCXHEADER) - 769 )
	{
		// ��С���ԡ�
		
		// �ͷ��ڴ�
		::GlobalFree((HGLOBAL) hDIB);
		
		return NULL;
	}
	
	// ����DIB������λ��
	m_pDIB->m_pImage->Create(lWidth, lHeight, 24);
	lpDst = (BYTE *) m_pDIB->FindAddress();
	
	// һ��һ�н���
	for (j = 0; j < lHeight; j++)
	{
		for (k = 0; k < 3; k++)
		{
			i = 0;
			while (i < lWidth)
			{
				// ��ȡһ���ֽ�
				bChar = *lpTemp;
				lpTemp++;
				
				if ((bChar & 0xC0) == 0xC0)
				{
					// �г�
					iCount = bChar & 0x3F;
				
					// ��ȡ��һ���ֽ�
					bChar = *lpTemp;
					lpTemp++;
				
					// bChar�ظ�iCount�α���
					for (l = 0; l < iCount; l++)
						*(lpDst + (lHeight - j - 1) * lLineBytes + (i + l) * 3 + RGB[k]) = bChar;

					// �Ѿ���ȡ���صĸ�����iCount
					i += iCount;
				}
				else
				{
					// ���浱ǰ�ֽ�
					*(lpDst + (lHeight - j - 1) * lLineBytes + i * 3 + RGB[k]) = bChar;

					// �Ѿ���ȡ���صĸ�����1
					i += 1;
				}
			}
		}
	}
	
	// �ͷ��ڴ�
	delete lpSrc;
 
	// �ر��ļ�
	file.Close();

	return hDIB;
}

// ���ܴ����
BOOL CCoding::Huffman(CString strPath)
{
	return TRUE;
}

// ���ܴ����
BOOL CCoding::Shannon_Fannon(CString strPath)
{
	return TRUE;
}

/*************************************************************************
 *
 * �������ƣ�
 *   DIBToGIF()
 *
 * ����:
 *   HDIB hDIB	        - DIB���
 *   CFile& file        - Ҫ������ļ�
 *   BOOL bInterlace	- ���淽ʽ
 *
 * ����ֵ:
 *   BOOL               - �ɹ�����True�����򷵻�False��
 *
 * ˵��:
 *   �ú�����256ɫDIBλͼ����ΪGIF�ļ���
 *
 *************************************************************************/
BOOL CCoding::DIBToGIF(HDIB hDIB, CFile &file, BOOL bInterlace)
{
	
	LONG	i;
	LONG	j;

	// ����DIB
	LPBYTE lpDIB = (LPBYTE) ::GlobalLock((HGLOBAL)hDIB);

	// �ж��Ƿ���8-bppλͼ
	if (m_pDIB->BitCount(lpDIB) != 8)
	{
		
		MessageBox(L"���Ƚ���ת��Ϊ8λɫλͼ���ٽ��д���", L"ϵͳ��ʾ", MB_ICONINFORMATION | MB_OK);

		// �������
		::GlobalUnlock((HGLOBAL)hDIB);

		return FALSE;
	}

	// GIF�ļ�ͷ
	GIFHEADER          GIFH;

	// GIF�߼���Ļ������
	GIFSCRDESC         GIFS;

	// GIFͼ��������
	GIFIMAGE           GIFI;

	// GIF�������
	GIFC_VAR           GIFCVar;

	// ��ɫ��
	BYTE               byGIF_Pal[768];

	// �ֽڱ���
	BYTE               byChar;

	// ָ��BITMAPINFO�ṹ��ָ�루Win3.0��
	LPBITMAPINFO	   lpbmi;

	// ָ��BITMAPCOREINFO�ṹ��ָ��
	LPBITMAPCOREINFO   lpbmc;

	// �����Ƿ���Win3.0 DIB�ı��
	BOOL			   bWinStyleDIB;

	BeginWaitCursor();

	// ��ȡDIB�߶�
	LONG lHeight = m_pDIB->Height(lpDIB);

	// ��ȡDIB���
	LONG lWidth = m_pDIB->Width(lpDIB);

	LPBYTE lpDIBBits = m_pDIB->FindAddress(lpDIB);

	// ����ÿ���ֽ���
	LONG lWidthBytes = DWORD_WBYTES(lWidth * 8);

	// ��GIFCVar�ṹ��ֵ
	GIFCVar.wWidth = (WORD)lWidth;
	GIFCVar.wDepth = (WORD)lHeight;
	GIFCVar.wBits = m_pDIB->BitCount(lpDIB);
	GIFCVar.wLineBytes = (WORD)BYTE_WBYTES((DWORD)GIFCVar.wWidth * (DWORD)GIFCVar.wBits);

	// ������ɫ��Ŀ
	LONG lColors = 1 << GIFCVar.wBits;

	// ��ȡָ��BITMAPINFO�ṹ��ָ�루Win3.0��
	lpbmi = (LPBITMAPINFO)lpDIB;

	// ��ȡָ��BITMAPCOREINFO�ṹ��ָ��
	lpbmc = (LPBITMAPCOREINFO)lpDIB;

	// �ж��Ƿ���WIN3.0��DIB
	bWinStyleDIB = IS_WIN30_DIB(lpDIB);

	// ����ɫ�帳ֵ
	if (bWinStyleDIB)
	{
		j = 0;
		for (i = 0; i < lColors; i++)
		{
			// ��ȡ��ɫ����
			byGIF_Pal[j++] = lpbmi->bmiColors[i].rgbRed;

			// ��ȡ��ɫ����
			byGIF_Pal[j++] = lpbmi->bmiColors[i].rgbGreen;

			// ��ȡ��ɫ����
			byGIF_Pal[j++] = lpbmi->bmiColors[i].rgbBlue;
		}
	}
	else
	{
		j = 0;
		for (i = 0; i < lColors; i++)
		{
			// ��ȡ��ɫ����
			byGIF_Pal[j++] = lpbmc->bmciColors[i].rgbtRed;

			// ��ȡ��ɫ����
			byGIF_Pal[j++] = lpbmc->bmciColors[i].rgbtGreen;

			// ��ȡ��ɫ����
			byGIF_Pal[j++] = lpbmc->bmciColors[i].rgbtBlue;
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////
	// ��ʼдGIF�ļ�

	// дGIF�ļ�ͷ
	GIFH.bySignature[0] = 'G';
	GIFH.bySignature[1] = 'I';
	GIFH.bySignature[2] = 'F';
	GIFH.byVersion[0] = '8';
	GIFH.byVersion[1] = '9';
	GIFH.byVersion[2] = 'a';
	file.Write((LPBYTE)&GIFH, 6);

	// дGIF�߼���Ļ������
	GIFS.wWidth = GIFCVar.wWidth;
	GIFS.wDepth = GIFCVar.wDepth;
	GIFS.GlobalFlag.PalBits = GIFCVar.wBits - 1;
	GIFS.GlobalFlag.SortFlag = 0x00;
	GIFS.GlobalFlag.ColorRes = GIFCVar.wBits - 1;
	GIFS.GlobalFlag.GlobalPal = 0x01;
	GIFS.byBackground = 0x00;
	GIFS.byAspect = 0x00;
	file.Write(&GIFS, 7);

	// дGIFȫ�ֵ�ɫ��
	file.Write(byGIF_Pal, (lColors * 3));

	// дGIFͼ�����������
	byChar = 0x2C;
	file.Write(&byChar, 1);

	// дGIFͼ��������
	GIFI.wLeft = 0;
	GIFI.wTop = 0;
	GIFI.wWidth = GIFCVar.wWidth;
	GIFI.wDepth = GIFCVar.wDepth;
	GIFI.LocalFlag.PalBits = 0x00;
	GIFI.LocalFlag.Reserved = 0x00;
	GIFI.LocalFlag.SortFlag = 0x00;
	GIFI.LocalFlag.Interlace = bInterlace ? 0x01 : 0x00;
	GIFI.LocalFlag.LocalPal = 0x00;
	file.Write(&GIFI, 9);

	// дGIFͼ��ѹ������
	HANDLE hSrcBuff = GlobalAlloc(GHND, (DWORD)MAX_BUFF_SIZE);
	GIFCVar.lpDataBuff = (LPBYTE)GlobalLock(hSrcBuff);
	GIFCVar.lpEndBuff = GIFCVar.lpDataBuff;
	GIFCVar.dwTempCode = 0UL;
	GIFCVar.wByteCnt = 0;
	GIFCVar.wBlockNdx = 1;
	GIFCVar.byLeftBits = 0x00;

	// ����GIF_LZW����
	LZW_Encode(lpDIBBits, file, &GIFCVar, (WORD)lWidthBytes, bInterlace);

	// �ж��Ƿ����ɹ�
	if (GIFCVar.wByteCnt)
	{
		// д���ļ�
		file.Write(GIFCVar.lpDataBuff, GIFCVar.wByteCnt);
	}

	// �ͷ��ڴ�
	GlobalUnlock(hSrcBuff);
	GlobalFree(hSrcBuff);

	// дGIF Block Terminator
	byChar = 0x00;
	file.Write(&byChar, 1);

	// дGIF�ļ���β��
	byChar = 0x3B;
	file.Write(&byChar, 1);

	// �������
	::GlobalUnlock((HGLOBAL)hDIB);

	EndWaitCursor();

	return TRUE;
}

/*************************************************************************
 *
 * �������ƣ�
 *   LZW_Encode()
 *
 * ����:
 *   BYTE* lpDIBBits		- ָ��ԴDIBͼ��ָ��
 *   CFile& file			- Ҫ������ļ�
 *   LPGIFC_VAR lpGIFCVar	- ָ��GIFC_VAR�ṹ��ָ��
 *	 WORD wWidthBytes		- ÿ��ͼ���ֽ���
 *	 BOOL bInterlace		- �Ƿ��ս���ʽ����
 *
 * ����ֵ:
 *   ��
 *
 * ˵��:
 *   �ú�����ͼ�����GIF_LZW���롣
 *
 *************************************************************************/
void CCoding::LZW_Encode(BYTE* lpDIBBits, CFile &file, LPGIFC_VAR lpGIFCVar, WORD wWidthBytes, BOOL bInterlace)
{
	// �ڴ������
	HANDLE hTableNdx;
	HANDLE hPrefix;
	HANDLE hSuffix;
	
	// ָ���ִ���ָ��
	LPWORD lpwTableNdx;
	
	// �����ִ�������������
	LPWORD lpwPrefix;
	BYTE* lpbySuffix;
	
	// ָ��ǰ�������ص�ָ��
	BYTE*  lpImage;
	
	// ���㵱ǰ����ͼ���ƫ����
	DWORD  dwDataNdx;
	
	// LZW_CLEAR
	WORD   wLZW_CLEAR;
	
	// LZW_EOI
	WORD   wLZW_EOI;
	
	// LZW_MinCodeLen
	BYTE   byLZW_MinCodeLen;
	
	// �ִ�������
	WORD   wPreTableNdx;
	WORD   wNowTableNdx;
	WORD   wTopTableNdx;
	
	// ��ϣ������
	WORD   wHashNdx;
	WORD   wHashGap;
	WORD   wPrefix;
	WORD   wShiftBits;
	
	// ��ǰͼ�������
	WORD   wRowNum;
	
	WORD   wWidthCnt;
	
	WORD   wi;
	WORD   wj;
	
	// ����ʽ�洢ʱÿ�����ӵ�����
	WORD   wIncTable[5]  = {8, 8, 4, 2, 0}; 
	
	// ����ʽ�洢ʱ��ʼ����
	WORD   wBgnTable[5]  = {0, 4, 2, 1, 0 }; 
	
	BOOL   bStart;
	BYTE   bySuffix;
	BYTE   bySubBlock[256];
	BYTE   byCurrentBits;
	BYTE   byMask;
	BYTE   byChar;
	BYTE   byPass;
	
	// ��ʱ�ֽڱ���
	BYTE   byTemp;
	
	// ���ִ�������ڴ�
	hTableNdx        = GlobalAlloc(GHND,(DWORD)(MAX_HASH_SIZE<<1));
	hPrefix          = GlobalAlloc(GHND,(DWORD)(MAX_HASH_SIZE<<1));
	hSuffix          = GlobalAlloc(GHND,(DWORD)MAX_HASH_SIZE);
	
	// �����ڴ�
	lpwTableNdx      = (LPWORD)GlobalLock(hTableNdx);
	lpwPrefix        = (LPWORD)GlobalLock(hPrefix);
	lpbySuffix       = (BYTE*)GlobalLock(hSuffix);
	
	// ����LZW_MinCodeLen
	byLZW_MinCodeLen = (lpGIFCVar->wBits>1) ? lpGIFCVar->wBits : 0x02;
	
	// дGIF LZW��С�����С
	file.Write(&byLZW_MinCodeLen, 1);
	
	wRowNum          = 0;
	bStart           = TRUE;
	byPass           = 0x00;
	
	// ����LZW_CLEAR
	wLZW_CLEAR       = 1 << byLZW_MinCodeLen;
	
	// ����LZW_EOI
	wLZW_EOI         = wLZW_CLEAR + 1;
	
	// ��ʼ���ִ���
	byCurrentBits    = byLZW_MinCodeLen + 0x01;
	wNowTableNdx     = wLZW_CLEAR + 2;
	wTopTableNdx     = 1 << byCurrentBits;
	for(wi = 0; wi < MAX_HASH_SIZE; wi++)
	{
		// ��ʼ��Ϊ0xFFFF
		*(lpwTableNdx + wi) = 0xFFFF;
	}
	
	// ���LZW_CLEAR
	OutputCode(file, wLZW_CLEAR, bySubBlock, &byCurrentBits, lpGIFCVar);
	
	// ���б���
	for(wi=0; wi < lpGIFCVar->wDepth; wi++)
	{
		// ���㵱ǰƫ����
		dwDataNdx  = (DWORD)(lpGIFCVar->wDepth - 1 - wRowNum) * (DWORD)wWidthBytes;
		
		// ָ��ǰ��ͼ���ָ��
		lpImage    = lpDIBBits + dwDataNdx;
		
		wWidthCnt  = 0;
		wShiftBits = 8 - lpGIFCVar->wBits;
		byMask     = (lpGIFCVar->wBits == 1) ? 0x80 : 0xF0;
		
		if (bStart)
		{
			// �ж��Ƿ���256ɫλͼ��һ������һ�ֽڣ�
			if (lpGIFCVar->wBits == 8)
			{
				// 256ɫ��ֱ�Ӹ�ֵ����
				byTemp = *lpImage++;
			}
			else
			{
				// ��256ɫ����Ҫ��λ��ȡ����ֵ
				wShiftBits = 8 - lpGIFCVar->wBits;
				byMask = (lpGIFCVar->wBits == 1) ? 0x80 : 0xF0;
				byTemp = (*lpImage & byMask) >> wShiftBits;
				byMask >>= lpGIFCVar->wBits;
				wShiftBits -= lpGIFCVar->wBits;
			}
			wPrefix  = (WORD)byTemp;
			bStart = FALSE;
			wWidthCnt++;
		}
		
		// ÿ�б���
		while(wWidthCnt < lpGIFCVar->wWidth)
		{
			// �ж��Ƿ���256ɫλͼ��һ������һ�ֽڣ�
			if (lpGIFCVar->wBits == 8)
			{
				// 256ɫ��ֱ�Ӹ�ֵ����
				byTemp = *lpImage++;
			}
			else
			{
				// ��256ɫ����Ҫ��λ��ȡ����ֵ
				byChar = *lpImage;
				byTemp = (byChar & byMask) >> wShiftBits;
				if (wShiftBits)
				{
					byMask >>= lpGIFCVar->wBits;
					wShiftBits -= lpGIFCVar->wBits;
				}
				else
				{
					wShiftBits = 8 - lpGIFCVar->wBits;
					byMask = (lpGIFCVar->wBits==1) ? 0x80 : 0xF0;
					lpImage++;
				}
			}
			bySuffix = byTemp;
			wWidthCnt ++;
			
			// ���ҵ�ǰ�ַ����Ƿ�������ִ�����
			wHashNdx = wPrefix ^ ((WORD)bySuffix << 4);
			wHashGap = (wHashNdx ? (MAX_HASH_SIZE - wHashNdx) : 1);
			
			// �жϵ�ǰ�ַ����Ƿ����ִ�����
			while(TRUE)
			{
				// ��ǰ�ַ��������ִ�����
				if (*(lpwTableNdx + wHashNdx) == 0xFFFF)
				{
				   // ���ַ������˳�ѭ��
				   break;
				}
				
				// �ж��Ƿ��ҵ����ַ���
				if ((*(lpwPrefix+wHashNdx)  == wPrefix) &&
					(*(lpbySuffix+wHashNdx) == bySuffix))
				{
					// �ҵ����˳�ѭ��
					break;
				}
				
				// �ڶ���ϣ��
				if (wHashNdx < wHashGap)
				{
					wHashNdx += MAX_HASH_SIZE;
				}
				wHashNdx -= wHashGap;
			}
			
			// �ж��Ƿ������ַ���
			if (*(lpwTableNdx+wHashNdx) != 0xFFFF)
			{
				// �������ַ���
				wPrefix = *(lpwTableNdx + wHashNdx);
			}
			else
			{
				// ���ַ���
				
				// ����ñ���
				OutputCode(file, wPrefix, bySubBlock, &byCurrentBits, lpGIFCVar);
				
				// �������ַ�����ӵ��ִ�����
				wPreTableNdx = wNowTableNdx;
				
				// �ж��Ƿ�ﵽ����ִ����С
				if (wNowTableNdx < MAX_TABLE_SIZE)
				{
					*(lpwTableNdx+wHashNdx) = wNowTableNdx++;
					*(lpwPrefix+wHashNdx)   = wPrefix;
					*(lpbySuffix+wHashNdx)  = bySuffix;
				}
				
				if (wPreTableNdx == wTopTableNdx)
				{
					if (byCurrentBits<12)
					{
						byCurrentBits ++;
						wTopTableNdx <<= 1;
					}
					else
					{
						// �ִ�������󳤶�
						
						// ���LZW_CLEAR
						OutputCode(file, wLZW_CLEAR, bySubBlock, &byCurrentBits, lpGIFCVar);
						
						// ���³�ʼ���ִ���
						byCurrentBits    = byLZW_MinCodeLen + 0x01;
						wLZW_CLEAR       = 1 << byLZW_MinCodeLen;
						wLZW_EOI         = wLZW_CLEAR + 1;
						wNowTableNdx     = wLZW_CLEAR + 2;
						wTopTableNdx     = 1 << byCurrentBits;
						for(wj = 0; wj < MAX_HASH_SIZE; wj++)
						{
							// ��ʼ��Ϊ0xFFFF
							*(lpwTableNdx+wj) = 0xFFFF;
						}
					}
				}
				wPrefix = (WORD)bySuffix;
			}
		}
		
		// �ж��Ƿ��ǽ���ʽ
		if (bInterlace)
		{
			// ����ʽ��������һ��λ��
			wRowNum += wIncTable[byPass];
			if (wRowNum >= lpGIFCVar->wDepth)
			{
				byPass++;
				wRowNum = wBgnTable[byPass];
			}
		}
		else
		{
			// �ǽ���ʽ��ֱ�ӽ�������һ����
			wRowNum ++;
		}
	}
	
	// �����ǰ����
	OutputCode(file, wPrefix, bySubBlock, &byCurrentBits, lpGIFCVar);
	
	// ���LZW_EOI
	OutputCode(file, wLZW_EOI, bySubBlock, &byCurrentBits, lpGIFCVar);
	
	if (lpGIFCVar->byLeftBits)
	{
		// ������ַ�
		bySubBlock[lpGIFCVar->wBlockNdx++] = (BYTE)lpGIFCVar->dwTempCode;
		
		// �ж��Ƿ񳬳�MAX_SUBBLOCK_SIZE
		if (lpGIFCVar->wBlockNdx > MAX_SUBBLOCK_SIZE)
		{
			// �ж�wByteCnt + 256�Ƿ񳬹�MAX_BUFF_SIZE
			if ((lpGIFCVar->wByteCnt + 256) >= MAX_BUFF_SIZE)
			{
				// ���
				file.Write(lpGIFCVar->lpDataBuff, lpGIFCVar->wByteCnt);
				lpGIFCVar->lpEndBuff = lpGIFCVar->lpDataBuff;
				lpGIFCVar->wByteCnt  = 0;
			}
			bySubBlock[0]           = (lpGIFCVar->wBlockNdx - 1);
			memcpy(lpGIFCVar->lpEndBuff, bySubBlock, lpGIFCVar->wBlockNdx);
			lpGIFCVar->lpEndBuff   += lpGIFCVar->wBlockNdx;
			lpGIFCVar->wByteCnt    += lpGIFCVar->wBlockNdx;
			lpGIFCVar->wBlockNdx    = 1;
		}
		lpGIFCVar->dwTempCode = 0UL;
		lpGIFCVar->byLeftBits = 0x00;
	}
	
	if (lpGIFCVar->wBlockNdx > 1)
	{
		// �ж�wByteCnt + 256�Ƿ񳬹�MAX_BUFF_SIZE
		if ((lpGIFCVar->wByteCnt + 256) >= MAX_BUFF_SIZE)
		{
			// ���
			file.Write(lpGIFCVar->lpDataBuff, lpGIFCVar->wByteCnt);
			lpGIFCVar->lpEndBuff = lpGIFCVar->lpDataBuff;
			lpGIFCVar->wByteCnt  = 0;
		}
		bySubBlock[0]           = lpGIFCVar->wBlockNdx - 1;
		memcpy(lpGIFCVar->lpEndBuff, bySubBlock, lpGIFCVar->wBlockNdx);
		lpGIFCVar->lpEndBuff   += lpGIFCVar->wBlockNdx;
		lpGIFCVar->wByteCnt    += lpGIFCVar->wBlockNdx;
		lpGIFCVar->wBlockNdx    = 1;
	}
	
	GlobalUnlock(hTableNdx);
	GlobalUnlock(hPrefix);
	GlobalUnlock(hSuffix);
	
	// �ͷ��ڴ�
	GlobalFree(hTableNdx);
	GlobalFree(hPrefix);
	GlobalFree(hSuffix);
	
	// �˳�
	return;
}

/*************************************************************************
 *
 * �������ƣ�
 *   OutputCode()
 *
 * ����:
 *   CFile& file			- Ҫ������ļ�
 *	 WORD wCode				- Ҫ��ӵı���
 *   BYTE* lpbySubBlock	- �ӿ�
 *	 BYTE* lpbyCurrentBits	- ��ǰλ��
 *	 LPGIFC_VAR lpGIFCVar	- ָ��GIFC_VAR�ṹ��ָ��
 *
 * ����ֵ:
 *   ��
 *
 * ˵��:
 *   �ú����������һ�����롣
 *
 *************************************************************************/
void CCoding::OutputCode(CFile &file, WORD wCode, BYTE* lpbySubBlock, BYTE* lpbyCurrentBits, LPGIFC_VAR lpGIFCVar)
{
	// ����ñ���
	lpGIFCVar->dwTempCode |= ((DWORD)wCode << lpGIFCVar->byLeftBits);
	lpGIFCVar->byLeftBits += (*lpbyCurrentBits);
	
	while(lpGIFCVar->byLeftBits >= 0x08)
	{
		lpbySubBlock[lpGIFCVar->wBlockNdx++] = (BYTE)lpGIFCVar->dwTempCode;
      
		// �ж��Ƿ񳬳�MAX_SUBBLOCK_SIZE
		if (lpGIFCVar->wBlockNdx > MAX_SUBBLOCK_SIZE)
		{
			// �ж�wByteCnt + 256�Ƿ񳬹�MAX_BUFF_SIZE
			if ((lpGIFCVar->wByteCnt + 256) >= MAX_BUFF_SIZE)
			{
				// ���
				file.Write(lpGIFCVar->lpDataBuff, lpGIFCVar->wByteCnt);
	            lpGIFCVar->lpEndBuff = lpGIFCVar->lpDataBuff;
		        lpGIFCVar->wByteCnt  = 0;
			}
			lpbySubBlock[0] = lpGIFCVar->wBlockNdx - 1;
			memcpy(lpGIFCVar->lpEndBuff, lpbySubBlock, lpGIFCVar->wBlockNdx);
			lpGIFCVar->lpEndBuff += lpGIFCVar->wBlockNdx;
			lpGIFCVar->wByteCnt += lpGIFCVar->wBlockNdx;
			lpGIFCVar->wBlockNdx = 1;
		}
		lpGIFCVar->dwTempCode >>= 8;
		lpGIFCVar->byLeftBits -= 0x08;
	}
	
	return;
}

/*************************************************************************
 *
 * �������ƣ�
 *   GIFToDIB()
 *
 * ����:
 *   CFile& file        - Ҫ��ȡ���ļ�
 *
 * ����ֵ:
 *   HDIB               - �ɹ�����DIB�ľ�������򷵻�NULL��
 *
 * ˵��:
 *   �ú�����ȡGIF�ļ���DIB
 *
 *************************************************************************/
HDIB CCoding::GIFToDIB(CFile &file)
{	
	// GIF�ļ�ͷ
	GIFHEADER          GIFH;
	
	// GIF�߼���Ļ������
	GIFSCRDESC         GIFS;
	
	// GIFͼ��������
	GIFIMAGE           GIFI;
	
	// GIFͼ����������
	GIFCONTROL         GIFC;
	
	// GIFͼ��˵�������
	GIFPLAINTEXT       GIFP;
	
	// GIFӦ�ó��������
	GIFAPPLICATION     GIFA;
	
	// GIF�������
	GIFD_VAR           GIFDVar;
	
	// ��ɫ��Ŀ
	WORD               wColors;
	
	// ÿ���ֽ���
	WORD               wWidthBytes;
	
	// ��ɫ��
	BYTE               byGIF_Pal[768];
	
	// 16ɫϵͳ��ɫ��
	BYTE               bySysPal16[48] = {	0,   0,   0,   0,   0, 128,
											0, 128,   0,   0, 128, 128,
										  128,   0,   0, 128,   0, 128,
										  128, 128,   0, 128, 128, 128,
										  192, 192, 192,   0,   0, 255,
											0, 255,   0,   0, 255, 255,
										  255,   0,   0, 255,   0, 255,
										  255, 255,   0, 255, 255, 255};
	
	// DIB��С(�ֽ���)
	DWORD              dwDIB_Size;	
	
	// ��ɫ���С(�ֽ���)
	WORD               wPalSize;
	
	// �ֽڱ���
	BYTE               byTemp;
	
	// �ڴ���
	HANDLE			   hSrcBuff;
	HANDLE             hTemp;
	
	// �ڴ�ָ��
	BYTE*              lpTemp;
	
	// �ֱ���
	WORD               wTemp;
	
	WORD               wi;
	
	// ��ǩ
	BYTE               byLabel;
	
	// ���С
	BYTE               byBlockSize;
	
	// ��ȡGIF�ļ�ͷ
	file.Read((BYTE*)&GIFH, sizeof(GIFH));
	
	// �ж��Ƿ���GIF�ļ�
	if (memicmp((BYTE*)GIFH.bySignature,"GIF",3) != 0)
	{
		// ��GIF�ļ�������NULL
		return NULL;
	}
	
	// �жϰ汾���Ƿ���ȷ
	if ((memicmp((BYTE*)GIFH.byVersion,"87a",3) != 0) &&
		(memicmp((BYTE*)GIFH.byVersion,"89a",3) != 0))
	{
		// ��֧�ָð汾������NULL
		return NULL;
	}
	
	// ��ȡGIF�߼���Ļ������
	file.Read((BYTE*)&GIFS, 7);
	
	// ��ȡ��ɫ���λ��
	GIFDVar.wBits = (WORD)GIFS.GlobalFlag.PalBits + 1;
	
	// �ж��Ƿ���ȫ�ֵ�ɫ��
	if (GIFS.GlobalFlag.GlobalPal)
	{
		// ����ֵ
		memset((BYTE*)byGIF_Pal,0,768);
		
		// ȫ�ֵ�ɫ���С
		wPalSize = 3 * (1 << GIFDVar.wBits);
		
		// ��ȡȫ�ֵ�ɫ��
		file.Read((BYTE*)byGIF_Pal,wPalSize);
	}
	
	// ��ȡ��һ���ֽ�
	file.Read((BYTE*)&byTemp,1);
	
	// ��ÿһ��������ѭ��
	while(TRUE)
	{
		// �ж��Ƿ���ͼ��������
		if (byTemp == 0x2C)
		{
			// ��ͼ�������飬�˳�ѭ��
			break;
		}
		
		// �ж��Ƿ���GIF��չ��
		if (byTemp==0x21)
		{
			// ��GIF��չ��
			
			// �����ڴ�
			hTemp  = GlobalAlloc(GHND, (DWORD)MAX_BUFF_SIZE);
			
			// �����ڴ�
			lpTemp = (BYTE*) GlobalLock(hTemp);
			
			// ��ȡ��һ���ֽ�
			file.Read((BYTE*)&byLabel, 1);
			
			// ��Ը�������飬���зֱ���
			switch(byLabel)
			{
				case 0xF9:
				{
					// ͼ����������
					file.Read((BYTE*)&GIFC, 6);
					
					// ����
					break;
				}
				case 0x01:
				{
					// ͼ��˵�������
					file.Read((BYTE*)&GIFP, sizeof(GIFP));
					
					// ��ȡ������С
					file.Read((BYTE*)&byBlockSize, 1);
					
					// ��byBlockSize > 0ʱѭ����ȡ
					while(byBlockSize)
					{
						// ��ȡͼ��˵������飨����û�н����κδ���
						file.Read(lpTemp, byBlockSize);
						
						// ��ȡ������С
						file.Read((BYTE*)&byBlockSize, 1);
					}
					
					// ����
					break;
				}
				case 0xFE:
				{
					// ע��˵�������
					
					// ��ȡ������С
					file.Read((BYTE*)&byBlockSize, 1);
					
					// ��byBlockSize > 0ʱѭ����ȡ
					while(byBlockSize)
					{
						// ��ȡע��˵������飨����û�н����κδ���
						file.Read(lpTemp, byBlockSize);
					
						// ��ȡ������С
						file.Read((BYTE*)&byBlockSize, 1);
					}
					
					// ����
					break;
				}
				case 0xFF:
				{
					// Ӧ�ó��������
					file.Read((BYTE*)&GIFA, sizeof(GIFA));
					
					// ��ȡ������С
					file.Read((BYTE*)&byBlockSize, 1);
					
					// ��byBlockSize > 0ʱѭ����ȡ
					while(byBlockSize)
					{
						// ��ȡӦ�ó�������飨����û�н����κδ���
						file.Read(lpTemp, byBlockSize);
						
						// ��ȡ������С
						file.Read((BYTE*)&byBlockSize, 1);
					}
					
					// ����
					break;
				}
				default:
				{
					// ����δ֪�������
					
					// ��ȡ������С
					file.Read((BYTE*)&byBlockSize, 1);
					
					// ��byBlockSize > 0ʱѭ����ȡ
					while(byBlockSize)
					{
						// ��ȡδ֪������飨����û�н����κδ���
						file.Read(lpTemp, byBlockSize);
						
						// ��ȡ������С
						file.Read((BYTE*)&byBlockSize, 1);
					}
					
					// ����
					break;
				}
			
				// �ͷ��ڴ�
				GlobalUnlock(hTemp);
				GlobalFree(hTemp);
			}
		}
		
		// ��ȡ��һ���ֽ�
		file.Read((BYTE*)&byTemp, 1);
	}

	// ��ȡGIFͼ��������
	file.Read((BYTE*)&GIFI, 9);
	
	// ��ȡͼ����
	GIFDVar.wWidth         = GIFI.wWidth;
	
	// ��ȡͼ��߶�
	GIFDVar.wDepth         = GIFI.wDepth;
	
	// �ж��Ƿ��������ɫ��
	if (GIFI.LocalFlag.LocalPal)
	{
		// ����ֵ
		memset((BYTE*)byGIF_Pal, 0, 768);
		
		// ��ȡ�����ɫ��λ��
		GIFDVar.wBits = (WORD)GIFI.LocalFlag.PalBits + 1;
		
		// �����ɫ���С
		wPalSize      = 3 * (1 << GIFDVar.wBits);
		
		// ��ȡ�����ɫ��
		file.Read((BYTE*)byGIF_Pal,wPalSize);
	}
	
	// ��GIFDVar��Ա��ֵ
	GIFDVar.wBits = ((GIFDVar.wBits==1) ? 1 : (GIFDVar.wBits<=4) ? 4 : 8);
	
	GIFDVar.wLineBytes = (WORD)BYTE_WBYTES((DWORD)GIFDVar.wWidth * (DWORD)GIFDVar.wBits);
	GIFDVar.bEOF = FALSE;
	
	// ����ʽ
	GIFDVar.bInterlace = (GIFI.LocalFlag.Interlace ? TRUE : FALSE);
	
	// ÿ���ֽ���
	wWidthBytes	= (WORD)DWORD_WBYTES((DWORD)GIFDVar.wWidth * (DWORD)GIFDVar.wBits);
	
	// ��ɫ��Ŀ
	wColors = 1 << GIFDVar.wBits;
	
	// ��ɫ���С
	wPalSize = wColors * sizeof(RGBQUAD);
	
	// ����DIB���ȣ��ֽڣ�
	dwDIB_Size = sizeof(BITMAPINFOHEADER) + wPalSize + GIFDVar.wDepth * wWidthBytes;
	
	// ΪDIB�����ڴ�
	HDIB hDIB = (HDIB) ::GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, dwDIB_Size);
	
	if (hDIB == 0)
	{
		// �ڴ����ʧ�ܣ�����NULL��
		return NULL;
	}
	
	// ����
	BYTE* pDIB = (BYTE*) ::GlobalLock((HGLOBAL) hDIB);
	
	///////////////////////////////////////////////////////////////////////////
	// ����BITMAPINFOHEADER

	// ָ��BITMAPINFOHEADER��ָ��
	LPBITMAPINFOHEADER lpBIH = (LPBITMAPINFOHEADER) pDIB;
	
	// ָ��BITMAPINFO��ָ��
	LPBITMAPINFO lpBI = (LPBITMAPINFO) pDIB;	

	// ��lpBIH��Ա��ֵ
	lpBIH->biSize = (DWORD)sizeof(BITMAPINFOHEADER);
	lpBIH->biWidth = (DWORD)GIFDVar.wWidth;
	lpBIH->biHeight = (DWORD)GIFDVar.wDepth;
	lpBIH->biPlanes = 1;
	lpBIH->biBitCount = GIFDVar.wBits;
	lpBIH->biCompression = BI_RGB;
	lpBIH->biSizeImage = (DWORD)wWidthBytes * GIFDVar.wDepth;
	lpBIH->biXPelsPerMeter = 0;
	lpBIH->biYPelsPerMeter = 0;
	lpBIH->biClrUsed = wColors;
	lpBIH->biClrImportant = 0;
	
	///////////////////////////////////////////////////////////////////////////
	// ���õ�ɫ��
	
	// �ж��Ƿ�ָ��ȫ�ֻ������ɫ��
	if (GIFS.GlobalFlag.GlobalPal || GIFI.LocalFlag.LocalPal)
	{
		wTemp = 0;
		for(wi=0; wi<wColors; wi++)
		{
			lpBI->bmiColors[wi].rgbRed      = byGIF_Pal[wTemp++];
			lpBI->bmiColors[wi].rgbGreen    = byGIF_Pal[wTemp++];
			lpBI->bmiColors[wi].rgbBlue     = byGIF_Pal[wTemp++];
			lpBI->bmiColors[wi].rgbReserved = 0x00;
		}
	}
	else
	{
		// û��ָ��ȫ�ֺ������ɫ�壬����ϵͳ��ɫ��
		
		// ������ɫ��Ŀ���ֱ����ɫ�帳ֵ
		switch(wColors)
		{
			case   2:
			{
				// ��ɫλͼ
				
				lpBI->bmiColors[0].rgbRed          = 0x00;
				lpBI->bmiColors[0].rgbGreen        = 0x00;
				lpBI->bmiColors[0].rgbBlue         = 0x00;
				lpBI->bmiColors[0].rgbReserved     = 0x00;
				lpBI->bmiColors[1].rgbRed          = 0xFF;
				lpBI->bmiColors[1].rgbGreen        = 0xFF;
				lpBI->bmiColors[1].rgbBlue         = 0xFF;
				lpBI->bmiColors[1].rgbReserved     = 0x00;
				
				// ����
				break;
			}
			case  16:
			{
				// 16ɫλͼ
				
				wTemp = 0;
				for(wi=0;wi<wColors;wi++)
				{
					lpBI->bmiColors[wi].rgbRed      = bySysPal16[wTemp++];
					lpBI->bmiColors[wi].rgbGreen    = bySysPal16[wTemp++];
					lpBI->bmiColors[wi].rgbBlue     = bySysPal16[wTemp++];
					lpBI->bmiColors[wi].rgbReserved = 0x00;
				}
				
				// ����
				break;
			}
			case 256:
			{
				// 256ɫλͼ
				for(wi=0; wi<wColors; wi++)
				{
					lpBI->bmiColors[wi].rgbRed      = (BYTE)wi;
					lpBI->bmiColors[wi].rgbGreen    = (BYTE)wi;
					lpBI->bmiColors[wi].rgbBlue     = (BYTE)wi;
					lpBI->bmiColors[wi].rgbReserved = 0x00;
				}
				
				// ����
				break;
			}
		}
	}
	
	///////////////////////////////////////////////////////////////////////////
	// ����

	// ��ȡ�������ݳ���
	GIFDVar.dwDataLen  = (DWORD) (file.GetLength() - file.GetPosition());
	
	// �����ڴ��С����󲻳���MAX_BUFF_SIZE��
	GIFDVar.wMemLen    = ((GIFDVar.dwDataLen > (DWORD)MAX_BUFF_SIZE) ?
						  (WORD)MAX_BUFF_SIZE : (WORD)GIFDVar.dwDataLen);
	
	// �����ڴ�
	hSrcBuff = GlobalAlloc(GHND, (DWORD)GIFDVar.wMemLen);
	
	// �����ڴ�
	GIFDVar.lpDataBuff = (BYTE*)GlobalLock(hSrcBuff);
	
	// ��ȡ��������
	ReadGIF(file, &GIFDVar.wMemLen, &GIFDVar.dwDataLen, GIFDVar.lpDataBuff, &GIFDVar.bEOF);
	
	// ��������ʼλ��
	GIFDVar.lpBgnBuff  = GIFDVar.lpDataBuff;
	
	// ��������ֹλ��
	GIFDVar.lpEndBuff  = GIFDVar.lpBgnBuff + GIFDVar.wMemLen;
	
	// ����DIB������λ��
	BYTE* lpDIBBits = m_pDIB->FindAddress(pDIB);
	
	// ����
	LZW_Decode(file, lpDIBBits, &GIFDVar, wWidthBytes);

	// �ͷ��ڴ�
	GlobalUnlock(hSrcBuff);
	GlobalFree(hSrcBuff);
	
	return hDIB;
}

/*************************************************************************
 *
 * �������ƣ�
 *   ReadGIF()
 *
 * ����:
 *   CFile& file			- ԴGIF�ļ�
 *   LPWORD lpwMemLen		- ���������ȣ�ָ�룩
 *   LPDWORD lpdwDataLen	- ʣ�����ݳ��ȣ�ָ�룩
 *   BYTE* lpSrcBuff		- ������ָ��
 *	 LPBOOL lpbEOF			- ������־
 *
 * ����ֵ:
 *   ��
 *
 * ˵��:
 *   �ú���������ȡָ��GIF�ļ��е�ͼ�����
 *
 *************************************************************************/
void CCoding::ReadGIF(CFile &file, LPWORD lpwMemLen, LPDWORD lpdwDataLen, BYTE* lpSrcBuf, LPBOOL lpbEOF)
{
	// �������ݳ����Ƿ���Ȼ�����ڴ��С���жϽ����Ƿ�Ҫ���
	if ((*lpdwDataLen) > (DWORD)(*lpwMemLen))
		(*lpdwDataLen) -= (DWORD)(*lpwMemLen);
	else
	{
		// �ڴ��С����ʣ�����ݳ���
		(*lpwMemLen)    = (WORD)(*lpdwDataLen);
		
		// EOF��־����ΪTRUE���������
		(*lpbEOF)       = TRUE;
	}
	
	// ��ȡ��������
	file.Read(lpSrcBuf, (*lpwMemLen));
	
	return;
}

/*************************************************************************
 *
 * �������ƣ�
 *   LZW_Decode()
 *
 * ����:
 *   CFile& file			- ԴGIF�ļ�
 *   BYTE* lpDIBBits		- ָ��Ҫ�����DIBͼ��ָ��
 *   LPGIFD_VAR lpGIFDVar	- ָ��GIFC_VAR�ṹ��ָ��
 *	 WORD wWidthBytes		- ÿ��ͼ���ֽ���
 *
 * ����ֵ:
 *   ��
 *
 * ˵��:
 *   �ú�����LZW������н��롣
 *
 *************************************************************************/
void CCoding::LZW_Decode(CFile &file, BYTE* lpDIBBits, LPGIFD_VAR lpGIFDVar, WORD wWidthBytes)
{
	// ָ������ͼ�����ݵ�ָ��
	BYTE *	lpDst;
	
	// �ڴ������
	HANDLE hPrefix;
	HANDLE hSuffix;
	HANDLE hStack;
	HANDLE hImage;
	
	// �����ִ�������������
	LPWORD lpwPrefix;
	BYTE* lpbySuffix;
	BYTE* lpbyStack;
	BYTE* lpbyStackBgn;
	
	// ָ��ͼ��ǰ�н�������ָ��
	BYTE*  lpImageBgn;
	
	// ָ��ǰ�������ص�ָ��
	BYTE*  lpImage;
	
	// ���㵱ǰ����ͼ���ƫ����
	DWORD  dwDataNdx;
	
	// LZW_CLEAR
	WORD   wLZW_CLEAR;
	
	// LZW_EOI
	WORD   wLZW_EOI;
	
	// LZW_MinCodeLen
	BYTE   byLZW_MinCodeLen;
	
	// �ִ�������
	WORD   wNowTableNdx;
	WORD   wTopTableNdx;
	
	// ��ǰͼ�������
	WORD   wRowNum;
	
	// ����
	WORD   wWidthCnt;
	WORD   wBitCnt;
	WORD   wRowCnt;
	
	WORD   wi;
	
	// ����ʽ�洢ʱÿ�����ӵ�����
	WORD   wIncTable[5]  = { 8,8,4,2,0 }; 
	
	// ����ʽ�洢ʱ��ʼ����
	WORD   wBgnTable[5]  = { 0,4,2,1,0 }; 
	
	// ���С
	BYTE   byBlockSize;
	
	// ������
	BYTE   byBlockNdx;
	
	DWORD  dwData;
	
	// ��ǰ����
	WORD   wCode;
	
	// ��һ������
	WORD   wOldCode;
	
	// ��ʱ����
	WORD   wTempNdx;
	
	WORD   wCodeMask[13] = {0x0000, 0x0001, 0x0003, 0x0007, 0x000F,
							0x001F, 0x003F, 0x007F, 0x00FF, 0x01FF,
							0x03FF, 0x07FF, 0x0FFF};
	
	BYTE   byLeftBits;
	BYTE   byFirstChar;
	BYTE   byCode;
	BYTE   byCurrentBits;
	BYTE   byPass;
	
	// ��ʱ�ֽڱ���
	BYTE   byTempChar;
	
	// ���ִ�������ڴ�
	hPrefix          = GlobalAlloc(GHND,(DWORD)(MAX_TABLE_SIZE<<1));
	hSuffix          = GlobalAlloc(GHND,(DWORD)MAX_TABLE_SIZE);
	hStack           = GlobalAlloc(GHND,(DWORD)MAX_TABLE_SIZE);
	hImage           = GlobalAlloc(GHND,(DWORD)wWidthBytes);
	
	// �����ڴ�
	lpwPrefix        = (LPWORD)GlobalLock(hPrefix);
	lpbySuffix       = (BYTE*)GlobalLock(hSuffix);
	lpbyStack        = (BYTE*)GlobalLock(hStack);
	lpbyStackBgn     = lpbyStack;
	lpImage          = (BYTE*)GlobalLock(hImage);
	lpImageBgn       = lpImage;
	
	// ��ȡGIF LZW��С�����С
	byLZW_MinCodeLen = *lpGIFDVar->lpBgnBuff++;
	
	byCurrentBits    = byLZW_MinCodeLen + (BYTE)0x01;
	
	// ����LZW_CLEAR
	wLZW_CLEAR       = 1 << byLZW_MinCodeLen;
	
	// ����LZW_EOI
	wLZW_EOI         = wLZW_CLEAR + 1;
	
	// �����ִ�������
	wNowTableNdx     = wLZW_CLEAR + 2;
	wTopTableNdx     = 1 << byCurrentBits;
	
	// ����ֵ
	dwData           = 0UL;
	wBitCnt          = lpGIFDVar->wBits;
	wRowNum          = 0;
	wRowCnt          = 1;
	wWidthCnt        = 0;
	wCode            = 0;
	wOldCode         = 0xFFFF;
	byBlockSize      = 0x01;
	byBlockNdx       = 0x00;
	byLeftBits       = 0x00;
	byTempChar       = 0x00;
	byPass           = 0x00;
	
	// ��ȡ��һ������
	while(byLeftBits < byCurrentBits)
	{
		// ��ȡ��һ���ַ�
		
		// �ж��Ƿ����һ�����ݿ�
		if (++byBlockNdx == byBlockSize)
		{
			// ��ȡ��һ�����ݿ�
			byBlockSize = *lpGIFDVar->lpBgnBuff++;
			byBlockNdx  = 0x00;
			
			// �ж��Ƿ����
			if ((lpGIFDVar->lpBgnBuff == lpGIFDVar->lpEndBuff) &&
			    !lpGIFDVar->bEOF)
			{
				// ��ȡ��һ�����ݿ�
				ReadGIF(file, &lpGIFDVar->wMemLen, &lpGIFDVar->dwDataLen, lpGIFDVar->lpDataBuff, &lpGIFDVar->bEOF);
				
				// ָ�����¸�ֵ
				lpGIFDVar->lpBgnBuff = lpGIFDVar->lpDataBuff;
				lpGIFDVar->lpEndBuff = lpGIFDVar->lpBgnBuff + lpGIFDVar->wMemLen;
			}
		}
		
		// ��һ���ַ�
		byCode      = *lpGIFDVar->lpBgnBuff++;
		
		// ��λ
		dwData     |= ((DWORD)byCode << byLeftBits);
		byLeftBits += 0x08;
		
		// �ж��Ƿ����
		if ((lpGIFDVar->lpBgnBuff == lpGIFDVar->lpEndBuff) && !lpGIFDVar->bEOF)
		{
			// ��ȡ��һ�����ݿ�
			ReadGIF(file, &lpGIFDVar->wMemLen, &lpGIFDVar->dwDataLen, lpGIFDVar->lpDataBuff, &lpGIFDVar->bEOF);
			
			// ָ�����¸�ֵ
			lpGIFDVar->lpBgnBuff = lpGIFDVar->lpDataBuff;
			lpGIFDVar->lpEndBuff = lpGIFDVar->lpBgnBuff + lpGIFDVar->wMemLen;
		}
	}
	
	wCode       = (WORD)dwData & wCodeMask[byCurrentBits];
	dwData    >>= byCurrentBits;
	byLeftBits -= byCurrentBits;
	
	// ����
	while(wCode != wLZW_EOI)
	{
		// ��ǰ���벻��LZW_EOI��
		
		// �ж��Ƿ���LZW_CLEAR��
		if (wCode == wLZW_CLEAR)
		{
			// ��LZW_CLEAR������ִ���
			
			// ���³�ʼ���ִ���
			for(wi = 0; wi < wLZW_CLEAR; wi++)
			{
				*(lpwPrefix  + wi)  = 0xFFFF;
				*(lpbySuffix + wi) = (BYTE)wi;
			}
			
			for(wi = wNowTableNdx; wi < MAX_TABLE_SIZE; wi++)
			{
				*(lpwPrefix+wi)  = 0xFFFF;
				*(lpbySuffix+wi) = 0x00;
			}
			
			byCurrentBits = byLZW_MinCodeLen + (BYTE)0x01;
			wNowTableNdx  = wLZW_CLEAR + 2;
			wTopTableNdx  = 1 << byCurrentBits;
			wOldCode      = 0xFFFF;
			
			// ��ȡ��һ������
			while(byLeftBits < byCurrentBits)
			{
				// ��ȡ��һ���ַ�
				
				// �ж��Ƿ����һ�����ݿ�
				if (++byBlockNdx == byBlockSize)
				{
					// ��ȡ��һ�����ݿ�
					byBlockSize = *lpGIFDVar->lpBgnBuff++;
					byBlockNdx  = 0x00;
					
					// �ж��Ƿ����
					if ((lpGIFDVar->lpBgnBuff == lpGIFDVar->lpEndBuff) && !lpGIFDVar->bEOF)
					{
						// ��ȡ��һ�����ݿ�
						ReadGIF(file, &lpGIFDVar->wMemLen, &lpGIFDVar->dwDataLen, lpGIFDVar->lpDataBuff, &lpGIFDVar->bEOF);
						
						// ָ�����¸�ֵ
						lpGIFDVar->lpBgnBuff = lpGIFDVar->lpDataBuff;
						lpGIFDVar->lpEndBuff = lpGIFDVar->lpBgnBuff +
											   lpGIFDVar->wMemLen;
					}
				}
				byCode      = *lpGIFDVar->lpBgnBuff++;
				dwData     |= ((DWORD)byCode << byLeftBits);
				byLeftBits += 0x08;
				
				// �ж��Ƿ����
				if ((lpGIFDVar->lpBgnBuff == lpGIFDVar->lpEndBuff) && !lpGIFDVar->bEOF)
				{
					// ��ȡ��һ�����ݿ�
					ReadGIF(file, &lpGIFDVar->wMemLen, &lpGIFDVar->dwDataLen, lpGIFDVar->lpDataBuff, &lpGIFDVar->bEOF);
					
					// ָ�����¸�ֵ
					lpGIFDVar->lpBgnBuff = lpGIFDVar->lpDataBuff;
					lpGIFDVar->lpEndBuff = lpGIFDVar->lpBgnBuff + lpGIFDVar->wMemLen;
				}
			}
			wCode       = (WORD)dwData & wCodeMask[byCurrentBits];
			dwData    >>= byCurrentBits;
			byLeftBits -= byCurrentBits;
			
			// �жϱ����Ƿ�ΪLZW_EOI
			if (wCode!=wLZW_EOI)
			{
				// ����û���õ�lpbyStack[0]
				lpbyStack  ++;
				
				// ������ѹ���ջ
				while((*(lpwPrefix+wCode)) != 0xFFFF)
				{
					*lpbyStack++ = *(lpbySuffix+wCode);
					wCode        = *(lpwPrefix+wCode);
				}
				*lpbyStack  = *(lpbySuffix+wCode);
				byFirstChar = *lpbyStack;
				
				// �������
				while(lpbyStack>lpbyStackBgn)
				{
					byTempChar |= (*lpbyStack-- << (8-wBitCnt));
					
					if (wBitCnt==8)
					{
						*lpImage++ = byTempChar;
						byTempChar = 0x00;
						wBitCnt    = lpGIFDVar->wBits;
					}
					else
					{
						wBitCnt   += lpGIFDVar->wBits;
					}
					
					wWidthCnt  ++;
					
					if (wWidthCnt==lpGIFDVar->wWidth)
					{
						if (wBitCnt!=lpGIFDVar->wBits)
						{
							*lpImage   = byTempChar;
							byTempChar = 0x00;
							wBitCnt    = lpGIFDVar->wBits;
						}
						
						// ͼ��ǰ��ƫ����
						dwDataNdx = (DWORD)(lpGIFDVar->wDepth - 1 - wRowNum) * (DWORD)wWidthBytes;
						
						// ͼ��ǰ����ʼλ��
						lpDst = (BYTE *)lpDIBBits + dwDataNdx;
						
						// ��ֵ
						memcpy(lpDst, lpImageBgn, wWidthBytes);
						
						lpImage   = lpImageBgn;
						
						// �ж��Ƿ��ս���ʽ����
						if (lpGIFDVar->bInterlace)
						{
							// ����ʽ
							
							// ������һ�е��к�
							wRowNum += wIncTable[byPass];
							if (wRowNum >= lpGIFDVar->wDepth)
							{
								byPass ++;
								wRowNum = wBgnTable[byPass];
							}
						}
						else
						{
							// �ǽ���ʽ���к�ֱ�Ӽ�1
							wRowNum ++;
						}
						wWidthCnt = 0;
					}
				}
			}
		}
		else
		{
			// ����û���õ�lpbyStack[0]
			lpbyStack  ++;
			
			// �ж��ַ����Ƿ����ִ�����
			if (wCode < wNowTableNdx)
			{
				// �����ִ�����
				wTempNdx   = wCode;
			}
			else
			{
				// ���ִ�����
				wTempNdx     = wOldCode;
				*lpbyStack++ = byFirstChar;
			}
			
			// ������ѹ���ջ
			while((*(lpwPrefix+wTempNdx)) != 0xFFFF)
			{
				*lpbyStack++ = *(lpbySuffix+wTempNdx);
				wTempNdx     = *(lpwPrefix+wTempNdx);
			}
			*lpbyStack  = *(lpbySuffix+wTempNdx);
			byFirstChar = *lpbyStack;
			
			// ���ַ�����ӵ��ִ�����
			*(lpwPrefix+wNowTableNdx)  = wOldCode;
			*(lpbySuffix+wNowTableNdx) = byFirstChar;
			if (++wNowTableNdx==wTopTableNdx && byCurrentBits<12)
			{
				byCurrentBits ++;
				wTopTableNdx = 1 << byCurrentBits;
			}
			
			// �������
			while(lpbyStack>lpbyStackBgn)
			{
				byTempChar |= (*lpbyStack-- << (8 - wBitCnt));
				if (wBitCnt==8)
				{
					*lpImage++ = byTempChar;
					byTempChar = 0x00;
					wBitCnt = lpGIFDVar->wBits;
				}
				else
					wBitCnt += lpGIFDVar->wBits;
				
				wWidthCnt  ++;
				if (wWidthCnt==lpGIFDVar->wWidth)
				{
					if (wBitCnt!=lpGIFDVar->wBits)
					{
						*lpImage   = byTempChar;
						byTempChar = 0x00;
						wBitCnt    = lpGIFDVar->wBits;
					}
					
					// ͼ��ǰ��ƫ����
					dwDataNdx = (DWORD)(lpGIFDVar->wDepth - 1 - wRowNum) * (DWORD)wWidthBytes;
					
					// ͼ��ǰ����ʼλ��
					lpDst = (BYTE *)lpDIBBits + dwDataNdx;
					
					// ��ֵ
					memcpy(lpDst, lpImageBgn, wWidthBytes);
					
					lpImage   = lpImageBgn;
					
					// �ж��Ƿ��ս���ʽ����
					if (lpGIFDVar->bInterlace)
					{
						// ����ʽ
						
						// ������һ�е��к�
						wRowNum += wIncTable[byPass];
						if (wRowNum >= lpGIFDVar->wDepth)
						{
							byPass ++;
							wRowNum = wBgnTable[byPass];
						}
					}
					else	// �ǽ���ʽ���к�ֱ�Ӽ�1
						wRowNum ++;
					wWidthCnt = 0;
				}
			}
		}
		wOldCode = wCode;
		
		// ��ȡ��һ������
		while(byLeftBits < byCurrentBits)
		{
			// ��ȡ��һ���ַ�
			
			// �ж��Ƿ����һ�����ݿ�
			if (++byBlockNdx == byBlockSize)
			{
				// ��ȡ��һ�����ݿ�
				byBlockSize = *lpGIFDVar->lpBgnBuff++;
				byBlockNdx  = 0x00;
				
				// �ж��Ƿ����
				if ((lpGIFDVar->lpBgnBuff == lpGIFDVar->lpEndBuff) && !lpGIFDVar->bEOF)
				{
					// ��ȡ��һ�����ݿ�
					ReadGIF(file, &lpGIFDVar->wMemLen, &lpGIFDVar->dwDataLen, lpGIFDVar->lpDataBuff,&lpGIFDVar->bEOF);					
					
					// ָ�����¸�ֵ
					lpGIFDVar->lpBgnBuff = lpGIFDVar->lpDataBuff;
					lpGIFDVar->lpEndBuff = lpGIFDVar->lpBgnBuff + lpGIFDVar->wMemLen;
				}
			}
			byCode      = *lpGIFDVar->lpBgnBuff++;
			dwData     |= ((DWORD)byCode << byLeftBits);
			byLeftBits += 0x08;
			
			// �ж��Ƿ����
			if ((lpGIFDVar->lpBgnBuff == lpGIFDVar->lpEndBuff) && !lpGIFDVar->bEOF)
			{
				// ��ȡ��һ�����ݿ�
				ReadGIF(file, &lpGIFDVar->wMemLen, &lpGIFDVar->dwDataLen, lpGIFDVar->lpDataBuff, &lpGIFDVar->bEOF);
				
				// ָ�����¸�ֵ
				lpGIFDVar->lpBgnBuff = lpGIFDVar->lpDataBuff;
				lpGIFDVar->lpEndBuff = lpGIFDVar->lpBgnBuff + lpGIFDVar->wMemLen;
			}
		}
		wCode       = (WORD)dwData & wCodeMask[byCurrentBits];
		dwData    >>= byCurrentBits;
		byLeftBits -= byCurrentBits;
	}
	
	// �ͷ��ڴ�
	GlobalUnlock(hPrefix);
	GlobalUnlock(hSuffix);
	GlobalUnlock(hStack);
	GlobalFree(hPrefix);
	GlobalFree(hSuffix);
	GlobalFree(hStack);
	
	return;
}

