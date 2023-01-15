
#pragma once

//
// Coding.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCoding window
#include "dib.h"
#include "DIP_SystemDoc.h"

// ������
#define DWORD_WBYTES(x)         ( (((x) + 31UL) >> 5) << 2 )
#define WORD_WBYTES(x)          ( (((x) + 15UL) >> 4) << 1 )
#define BYTE_WBYTES(x)          (  ((x) +  7UL) >> 3       )

// ����
#define MAX_BUFF_SIZE           32768 
#define MAX_HASH_SIZE            5051
#define MAX_TABLE_SIZE           4096
#define MAX_SUBBLOCK_SIZE         255

class CCoding : public CWnd
{
// Construction
public:
	CCoding();
	CCoding(CDIB* pDIB);

// Attributes
public:

	// PCX�ļ�ͷ�ṹ
	typedef struct{
		 BYTE bManufacturer;
		 BYTE bVersion;
		 BYTE bEncoding;
		 BYTE bBpp;
		 WORD wLeft;
		 WORD wTop;
		 WORD wRight;
		 WORD wBottom;
		 WORD wXResolution;
		 WORD wYResolution;
		 BYTE bPalette[48];
		 BYTE bReserved;
		 BYTE bPlanes;
		 WORD wLineBytes;
		 WORD wPaletteType;
		 WORD wSrcWidth;
		 WORD wSrcDepth;
		 BYTE bFiller[54];
	} PCXHEADER;

	// �ļ�ͷ�ṹ
	typedef struct tagGIF_HEADER{
			BYTE bySignature[3];		// GIF����
			BYTE byVersion[3];			// �汾��
	} GIFHEADER;	

	// �߼���Ļ��ʶ���ṹ
	typedef struct tagGIF_SCRDESC{
			WORD wWidth;				// �߼���Ļ���
			WORD wDepth;				// �߼���Ļ�߶�
			struct tagGLOBAL_FLAG{		
					BYTE PalBits   : 3;	// ָ��ȫ�ֵ�ɫ���λ��
					BYTE SortFlag  : 1;	// ָ��ȫ�ֵ�ɫ����RGB��ɫֵ�Ƿ񾭹�����
					BYTE ColorRes  : 3;	// ָ��ͼ��ɫ�ʷֱ���
					BYTE GlobalPal : 1;	// ָ���Ƿ����ȫ�ֵ�ɫ��
			} GlobalFlag;
			BYTE byBackground;			// ����ɫ
			BYTE byAspect;				// ���ؿ�߱�
	} GIFSCRDESC;

	// ͼ���ʶ���ṹ 
	typedef struct tagGIF_IMAGE{
			WORD wLeft;					// X����ƫ����
			WORD wTop;					// Y����ƫ����
			WORD wWidth;				// ͼ����
			WORD wDepth;				// ͼ��߶�
			struct tagLOCALFLAG{		
		            BYTE PalBits   : 3;	// �ֲ���ɫ�б��С
					BYTE Reserved  : 2;	// �����������ʼ��Ϊ0
					BYTE SortFlag  : 1;	// �����־
					BYTE Interlace : 1;	// ��֯��־
					BYTE LocalPal  : 1;	// �ֲ���ɫ�б��־
			} LocalFlag;
	} GIFIMAGE;

	// ͼ�ο�����չ�ṹ 
	typedef struct tagGIF_CONTROL{		 
			BYTE byBlockSize;					// ָ��ͼ�ο�����չ�ṹ�ĳ��ȣ��̶�Ϊ6
			struct tagFLAG{					
					BYTE Transparency   : 1;	// ͸����ɫ��־
					BYTE UserInput      : 1;	// �û������־
					BYTE DisposalMethod : 3;	// ���÷���
					BYTE Reserved       : 3;	// ����
			} Flag;
			WORD wDelayTime;					// ָ��������һ������ǰ�ӳٵ�ʱ��
			BYTE byTransparencyIndex;			// ָ��ͼ����͸��ɫ����ɫ����
			BYTE byTerminator;					// ���ս������̶�Ϊ0
	} GIFCONTROL;

	// ͼ���ı���չ�ṹ
	typedef struct tagGIF_PLAINTEXT{
			BYTE byBlockSize;		// �������ĳ���
			WORD wTextGridLeft;		// �ı������߼���Ļ����߽����
			WORD wTextGridTop;		// �ı������߼���Ļ���ϱ߽����
			WORD wTextGridWidth;	// �ı�����
			WORD wTextGridDepth;	// �ı���߶�
			BYTE byCharCellWidth;	// �ַ���Ԫ����
			BYTE byCharCellDepth;	// �ַ���Ԫ��߶�
			BYTE byForeColorIndex;	// �ı�ǰ��ɫ����
			BYTE byBackColorIndex;	// �ı�����ɫ����
	} GIFPLAINTEXT;

	// Ӧ�ó�����չ�ṹ
	typedef struct tagGIF_APPLICATION{
			BYTE byBlockSize;			// ���С
			BYTE byIdentifier[8];		// Ӧ�ó����ʶ��
			BYTE byAuthentication[3];	// Ӧ�ó��������
	} GIFAPPLICATION;

	typedef struct tagGIF_D_VAR{
			BYTE* lpDataBuff;
			BYTE* lpBgnBuff;
			BYTE* lpEndBuff;
			DWORD dwDataLen;
			WORD  wMemLen;
			WORD  wWidth;
			WORD  wDepth;
			WORD  wLineBytes;
			WORD  wBits;
			BOOL  bEOF;
			BOOL  bInterlace;	
	} GIFD_VAR, FAR* LPGIFD_VAR;

	typedef struct tagGIF_C_VAR{
			BYTE* lpDataBuff;
			BYTE* lpEndBuff;
			DWORD dwTempCode;
			WORD  wWidth;
			WORD  wDepth;
			WORD  wLineBytes;
			WORD  wBits;
			WORD  wByteCnt;
			WORD  wBlockNdx;
			BYTE  byLeftBits;
	} GIFC_VAR, FAR *LPGIFC_VAR;

// Implementation
public:
	void OutputCode(CFile &file, WORD wCode, BYTE* lpbySubBlock, BYTE* lpbyCurrentBits, LPGIFC_VAR lpGIFCVar);
	void LZW_Encode(BYTE* lpDIBBits, CFile& file, LPGIFC_VAR lpGIFCVar, WORD wWidthBytes, BOOL bInterlace);
	void LZW_Decode(CFile& file, BYTE* lpDIBBits, LPGIFD_VAR lpGIFDVar, WORD wWidthBytes);
	BOOL DIBToGIF(HDIB hDIB, CFile& file, BOOL bInterlace);
	HDIB GIFToDIB(CFile& file);
	void ReadGIF(CFile& file, LPWORD lpwMemLen, LPDWORD lpdwDataLen, BYTE* lpSrcBuff, LPBOOL lpbEOF);
	BOOL Huffman(CString strPath);
	BOOL Shannon_Fannon(CString strPath);
	BOOL DIBToPCX(CString strPath);
	HDIB PCXToDIB(CString strPath);
	void SaveBMP(CString fileName, BYTE * buf, UINT width, UINT height);
	BYTE* LoadBMP(CString fileName, UINT *width, UINT *height);
	void LoadJPG(CDIP_SystemDoc* pDoc, CString filePath);
	void SaveJPG(CDIP_SystemDoc* pDoc, CString filePath);
	virtual ~CCoding();

	// Generated message map functions
protected:
	CDIB* m_pDIB;
	DECLARE_MESSAGE_MAP()
};
