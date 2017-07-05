
// Test.cpp : 응용 프로그램에 대한 클래스 동작을 정의합니다.
//

#include "stdafx.h"
#include "Test.h"
#include "TestDlg.h"
#include "myGdiPlus.h" 

using namespace Gdiplus; 
#pragma comment(lib, "gdiplus.lib")


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

ULONG_PTR gdiplusToken;

// CTestApp

BEGIN_MESSAGE_MAP(CTestApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CTestApp 생성

CTestApp::CTestApp()
{
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}


// 유일한 CTestApp 개체입니다.

CTestApp theApp;


// CTestApp 초기화

BOOL CTestApp::InitInstance()
{
	CWinApp::InitInstance();
	AfxEnableControlContainer();

	//begin: initialize GDI+  
	GdiplusStartupInput gdiplusStartupInput;
    VERIFY(GdiplusStartup( &gdiplusToken, &gdiplusStartupInput, NULL ) == Ok );
	//end: initialize GDI+


	// Create the shell manager, in case the dialog contains
	// any shell tree view or shell list view controls.
	CShellManager *pShellManager = new CShellManager;

	// Activate "Windows Native" visual manager for enabling themes in MFC controls
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	CTestDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();

	// Delete the shell manager created above.
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}
	return FALSE;
}



int CTestApp::ExitInstance()
{
//begin: shutdown GDI+ 
	GdiplusShutdown(gdiplusToken);
//end: shutdown GDI+
	return CWinApp::ExitInstance();
}
