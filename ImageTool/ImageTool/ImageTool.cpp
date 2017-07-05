// ImageTool.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "ImageTool.h"
#include "MainFrm.h"
#include "ImageToolDoc.h"
#include "ImageToolView.h"
#include "myGdiPlus.h" 

using namespace Gdiplus; 
#pragma comment(lib, "gdiplus.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

ULONG_PTR gdiplusToken;

/////////////////////////////////////////////////////////////////////////////
// CImageToolApp

BEGIN_MESSAGE_MAP(CImageToolApp, CWinApp)
	//{{AFX_MSG_MAP(CImageToolApp)
	//}}AFX_MSG_MAP
	ON_COMMAND( ID_FILE_NEW,         CWinApp::OnFileNew        )
	ON_COMMAND( ID_FILE_OPEN,        CWinApp::OnFileOpen       )
	ON_COMMAND( ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup )
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImageToolApp construction
CImageToolApp::CImageToolApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CImageToolApp object
CImageToolApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CImageToolApp initialization
BOOL CImageToolApp::InitInstance()
{
	AfxEnableControlContainer();

//begin: initialize GDI+  
	GdiplusStartupInput gdiplusStartupInput;
    VERIFY(GdiplusStartup( &gdiplusToken, &gdiplusStartupInput, NULL ) == Ok );
//end: initialize GDI+

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	LoadStdProfileSettings();

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate( IDR_MAINFRAME,
		                                   RUNTIME_CLASS(CImageToolDoc),
										   RUNTIME_CLASS(CMainFrame),
										   RUNTIME_CLASS(CImageToolView) );
	AddDocTemplate(pDocTemplate);

	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	if (!ProcessShellCommand(cmdInfo))
	{
		return FALSE;
	}

	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	return TRUE;
}

int CImageToolApp::ExitInstance() 
{
//begin: shutdown GDI+ 
	GdiplusShutdown(gdiplusToken);
//end: shutdown GDI+
	
	return CWinApp::ExitInstance();
}
