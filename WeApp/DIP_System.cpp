// DIP_System.cpp : Defines the class behaviors for the application.

#include "stdafx.h"
#include "DIP_System.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "DIP_SystemDoc.h"
#include "DIP_SystemView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CDIP_SystemApp

BEGIN_MESSAGE_MAP(CDIP_SystemApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
	ON_COMMAND(ID_AUTHOR_BLOG, &CDIP_SystemApp::OnAuthorBlog)
	ON_COMMAND(ID_AUTHOR_MICRO_BLOG, &CDIP_SystemApp::OnAuthorMicroBlog)
	ON_COMMAND(ID_FILE_MYOPEN, &CDIP_SystemApp::OnFileMyopen)
	ON_COMMAND(ID_FILE_OPEN_PGM, &CDIP_SystemApp::OnFileOpenPgm)
	ON_COMMAND(ID_FILE_OPEN_PBM, &CDIP_SystemApp::OnFileOpenPbm)
	ON_COMMAND(ID_FILE_OPEN_PPM, &CDIP_SystemApp::OnFileOpenPpm)
END_MESSAGE_MAP()

// CDIP_SystemApp construction

CDIP_SystemApp::CDIP_SystemApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

CDIP_SystemApp theApp;//�������--���Ƶ�

// CDIP_SystemApp initialization

BOOL CDIP_SystemApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	// CWinApp::Enable3dControls is no longer needed. You should remove this call.
	//Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(L"Capital Normal University");

	LoadStdProfileSettings(MIX_FILE_NUMBER);  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(
		IDR_DIP_SYTYPE,
		RUNTIME_CLASS(CDIP_SystemDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CDIP_SystemView));
	AddDocTemplate(pDocTemplate);

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// ����ʱ���Զ���һ�����ĵ� 
	cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The main window has been initialized, so show and update it.
	pMainFrame->ShowWindow(SW_SHOWMAXIMIZED);
	pMainFrame->SetWindowText(L"Digital Image Processing System");
	pMainFrame->UpdateWindow();

	return TRUE;
}

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// App command to run the dialog
void CDIP_SystemApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CDIP_SystemApp message handlers

// CMFCVisualManager й¶ԭ��
int CDIP_SystemApp::ExitInstance()
{
	// TODO:  �ڴ����ר�ô����/����û���

	CMFCVisualManager::DestroyInstance(TRUE);
	return CWinApp::ExitInstance();
}

// github
void CDIP_SystemApp::OnAuthorBlog()
{
	ShellExecute(NULL, L"open", L"http://www.github.com/zhd5120153951", NULL, NULL, SW_SHOWNORMAL);
}

// csdn
void CDIP_SystemApp::OnAuthorMicroBlog()
{
	ShellExecute(NULL, L"open", L"http://blog.csdn/zhd5120153951", NULL, NULL, SW_SHOWNORMAL);
}

// �½�
void CDIP_SystemApp::OnFileMynew()
{
	CWinApp::OnFileNew();
}

// ��
void CDIP_SystemApp::OnFileMyopen()
{
	CString strFilter = L"����ͼ��|*.BMP;*.DIB;*.RLE;*.JPG;*.JPEG;*.JPE;*.JFIF;*.GIF;*.TIF;*.TIFF;*.PNG;*.ICO|BMP (*.BMP;*.DIB;*.RLE)|*.BMP;*.DIB;*.RLE|JPEG (*.JPG;*.JPEG;*.JPE;*.JFIF)|*.JPG;*.JPEG;*.JPE;*.JFIF|GIF (*.GIF)|*.GIF|ͼ�� (*.ICO)|*.ICO|�����ļ�|*.*||";

	CFileDialog hFileDlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_READONLY, strFilter, NULL);
	hFileDlg.m_ofn.nFilterIndex = 1;
	hFileDlg.m_ofn.lStructSize = sizeof(OPENFILENAME);
	hFileDlg.m_ofn.lpstrTitle = TEXT("��ͼ���ļ�...\0");
	hFileDlg.m_ofn.nMaxFile = MAX_PATH;
	if (hFileDlg.DoModal() != IDOK) return;
	CString path = hFileDlg.GetPathName();

	CWinApp::OnFileNew();
	CMDIFrameWnd* pFrame = (CMDIFrameWnd *)AfxGetApp()->GetMainWnd();
	CMDIChildWnd* pChild = (CMDIChildWnd *)pFrame->GetActiveFrame();
	CDIP_SystemDoc* pDoc = (CDIP_SystemDoc *)pChild->GetActiveDocument();
	if (pDoc->DIBImage() != NULL)
		pDoc = pDoc->CreateNewDocument();
	if (pDoc->OnOpenDocument(path))
		pChild->GetActiveView()->Invalidate();
}

// ��PGM��ʽͼ��
void CDIP_SystemApp::OnFileOpenPgm()
{
	CFileDialog hFileDlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_READONLY, 
		TEXT("PGM�ļ�(*.pgm)|*.pgm||"), NULL);
	hFileDlg.m_ofn.nFilterIndex = 2;
	hFileDlg.m_ofn.lStructSize = sizeof(OPENFILENAME);
	hFileDlg.m_ofn.lpstrTitle = TEXT("��PGM�ļ�...\0");
	hFileDlg.m_ofn.nMaxFile = MAX_PATH;
	if (hFileDlg.DoModal() != IDOK) return;
	CString path = hFileDlg.GetPathName();

	CWinApp::OnFileNew();
	CMDIFrameWnd* pFrame = (CMDIFrameWnd *)AfxGetApp()->GetMainWnd();
	CMDIChildWnd* pChild = (CMDIChildWnd *)pFrame->GetActiveFrame();
	CDIP_SystemDoc* pDoc = (CDIP_SystemDoc *)pChild->GetActiveDocument();
	if (pDoc->DIBImage() != NULL)
		pDoc = pDoc->CreateNewDocument();
	if (pDoc->OnOpenDocument(path))
		pChild->GetActiveView()->Invalidate();
}

// ��PBM��ʽͼ��
void CDIP_SystemApp::OnFileOpenPbm()
{
	CFileDialog hFileDlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_READONLY, 
		TEXT("PBM�ļ�(*.pbm)|*.pbm||"), NULL);
	hFileDlg.m_ofn.nFilterIndex = 2;
	hFileDlg.m_ofn.lStructSize = sizeof(OPENFILENAME);
	hFileDlg.m_ofn.lpstrTitle = TEXT("��PBM�ļ�...\0");
	hFileDlg.m_ofn.nMaxFile = MAX_PATH;
	if (hFileDlg.DoModal() != IDOK) return;
	CString path = hFileDlg.GetPathName();

	CWinApp::OnFileNew();
	CMDIFrameWnd* pFrame = (CMDIFrameWnd *)AfxGetApp()->GetMainWnd();
	CMDIChildWnd* pChild = (CMDIChildWnd *)pFrame->GetActiveFrame();
	CDIP_SystemDoc* pDoc = (CDIP_SystemDoc *)pChild->GetActiveDocument();
	if (pDoc->DIBImage() != NULL)
		pDoc = pDoc->CreateNewDocument();
	if (pDoc->OnOpenDocument(path))
		pChild->GetActiveView()->Invalidate();
}

// ��PPM��ʽͼ��
void CDIP_SystemApp::OnFileOpenPpm()
{
	CFileDialog hFileDlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_READONLY, 
		TEXT("PPM�ļ�(*.ppm)|*.ppm||"), NULL);
	hFileDlg.m_ofn.nFilterIndex = 2;
	hFileDlg.m_ofn.lStructSize = sizeof(OPENFILENAME);
	hFileDlg.m_ofn.lpstrTitle = TEXT("��PPM�ļ�...\0");
	hFileDlg.m_ofn.nMaxFile = MAX_PATH;
	if (hFileDlg.DoModal() != IDOK) return;
	CString path = hFileDlg.GetPathName();

	CWinApp::OnFileNew();
	CMDIFrameWnd* pFrame = (CMDIFrameWnd *)AfxGetApp()->GetMainWnd();
	CMDIChildWnd* pChild = (CMDIChildWnd *)pFrame->GetActiveFrame();
	CDIP_SystemDoc* pDoc = (CDIP_SystemDoc *)pChild->GetActiveDocument();
	if (pDoc->DIBImage() != NULL)
		pDoc = pDoc->CreateNewDocument();
	if (pDoc->OnOpenDocument(path))
		pChild->GetActiveView()->Invalidate();
}