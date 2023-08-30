// CBitmapBtn.cpp: 实现文件
//

#include "pch.h"
#include "WeMFC.h"
#include "afxdialogex.h"
#include "CBitmapBtn.h"


// CBitmapBtn 对话框

IMPLEMENT_DYNAMIC(CBitmapBtn, CDialog)

CBitmapBtn::CBitmapBtn(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_BitmapBtn, pParent)
{

}

CBitmapBtn::~CBitmapBtn()
{
}

void CBitmapBtn::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_BMP, m_bitmapbutton);
}


BEGIN_MESSAGE_MAP(CBitmapBtn, CDialog)
	ON_BN_CLICKED(IDC_BTN_BMP, &CBitmapBtn::OnClickedBtnBmp)
	ON_BN_CLICKED(IDCANCEL, &CBitmapBtn::OnBnClickedCancel)
END_MESSAGE_MAP()


// CBitmapBtn 消息处理程序


void CBitmapBtn::OnClickedBtnBmp()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CBitmapBtn::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialog::OnCancel();
}


BOOL CBitmapBtn::OnInitDialog()
{
	// TODO: 在此处添加实现代码.
	CDialog::OnInitDialog();
	m_bitmapbutton.LoadBitmaps(IDB_BMP1, IDB_BMP2);				//载入按钮位图资源
	LOGFONT font;
	memset(&font, 0, sizeof(LOGFONT));				//为字体结构赋初值，默认值
	font.lfHeight = 20;								//字体高度为20
	font.lfWeight = 700;								//粗体
	font.lfWidth = 13;								//字体宽度为13
	font.lfItalic = TRUE;							//采用斜体
	strcpy_s(font.lfFaceName, "Times New Roman");		//字符集
	font.lfEscapement = -50;						//字体向下倾斜5度
	myfont.CreateFontIndirect(&font); 				//创建新字体
	GetDlgItem(IDCANCEL)->SetFont(&myfont);

	return TRUE;
}
