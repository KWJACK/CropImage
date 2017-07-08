
// MainFrm.cpp : CMainFrame 클래스의 구현
//

#include "stdafx.h"
#include "EPRE.h"

#include "MainFrm.h"
#include "FormFolderDlg.h"
#include "FormListDlg.h"
#include "FormBMP.h"
#include "BMPZoomView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define SEND_LISTDLG WM_USER+1
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()	
	ON_MESSAGE(SEND_LISTDLG, &CMainFrame::OnSendListdlg)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 상태 줄 표시기
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 생성/소멸

CMainFrame::CMainFrame():m_bCustomFolder(FALSE)
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
}

CMainFrame::~CMainFrame()
{
}

void CMainFrame::DoDataExchange(CDataExchange* pDX)
{
	CFrameWnd::DoDataExchange(pDX);
	
	
	
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;	
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}

// CMainFrame 진단

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 메시지 처리기

void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
	// 뷰 창으로 포커스를 이동합니다.
	
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// 뷰에서 첫째 크랙이 해당 명령에 나타나도록 합니다.	

	// 그렇지 않으면 기본 처리합니다.
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}



BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{	
	// 수직선을 나눈다.
	if(!m_wndSplitter.CreateStatic(this, 1, 2)) {
		TRACE0("Fail to left vertical splitter...");
		return FALSE;
	}

	int m_IDpane = m_wndSplitter.IdFromRowCol(0, 0);//왼쪽 윈도우 아이디 접근
	int m_IDpane2 = m_wndSplitter.IdFromRowCol(0, 1);//오른쪽 윈도우 아이디 접근
	//왼쪽열에 수평선을 만듬
	if(!m_wndSplitter2.CreateStatic(&m_wndSplitter, 2, 1, WS_CHILD|WS_VISIBLE, m_IDpane)) {
		TRACE0("Fail to left horizontal splitter...");
		return FALSE;
	}
	//오른쪽열에 수평선 만듬
	if(!m_wndSplitter3.CreateStatic(&m_wndSplitter, 2, 1, WS_CHILD|WS_VISIBLE, m_IDpane2)) {
		TRACE0("Fail to right horizontal splitter...");
		return FALSE;
	}
	CRuntimeClass* t_FormFolderDlg = RUNTIME_CLASS(FormFolderDlg);	

	// Main View (link 출력 뷰) 를 만듬(왼쪽 하단, 상단) 하단 ForList를 만들어야  상단 FolderDlg 생성가능
	m_wndSplitter2.CreateView(1, 0, RUNTIME_CLASS(FormListDlg), CSize(200, 10), pContext);
 	
	if(!m_wndSplitter2.CreateView(0, 0, t_FormFolderDlg, CSize(200, 10), pContext)) {
		TRACE0("Fail to create MainView...");
		return FALSE;
	}

	// Main View (link 출력 뷰) 를 만듬(오른쪽 상단, 하단)
	if(!m_wndSplitter3.CreateView(0, 0, RUNTIME_CLASS(CBMPZoomView), CSize(800, 200), pContext)||
		!m_wndSplitter3.CreateView(1, 0, RUNTIME_CLASS(FormBMP), CSize(800, 10), pContext)) {
		TRACE0("Fail to create MainView...");
		return FALSE;
	}
	
	// 수직 분할 윈도우의 크기
	m_wndSplitter.SetColumnInfo( 0, 250, 20 );
	// 왼쪽 수평 분할 윈도우의 크기
	m_wndSplitter2.SetRowInfo( 0, 300, 10 );
	m_wndSplitter3.SetRowInfo( 0, 400, 10 );
	
	// 활성창 지정
	SetActiveView((CView *)m_wndSplitter3.GetPane(0, 0));
			
		
	return TRUE;
}


void CMainFrame::OnRootFolderChanged(UINT uID)
{
	/*BOOL bCustomFolder = (uID == IDC_RADIO_CUSTOM);
	if (bCustomFolder == m_bCustomFolder)
		return;
	CString cRootFolder = m_cRootFolder;
	if (uID == IDC_RADIO_CUSTOM)
	{
		CSelRootFolderDlg cDlg;
		cDlg.m_cRootFolder = m_cRootFolder;
		if (cDlg.DoModal() != IDOK)
		{
			int nID = bCustomFolder ? IDC_RADIO_DEFAULT : IDC_RADIO_CUSTOM;
			CheckRadioButton(IDC_RADIO_DEFAULT, IDC_RADIO_CUSTOM, nID);
			return;
		}
		cRootFolder = m_cRootFolder = cDlg.m_cRootFolder;
	}
	else
		cRootFolder.Empty();
	SetDlgItemText(IDC_EDIT_ROOTFOLDER, cRootFolder);
	m_cTreeCtrl.SetRootFolder(cRootFolder);
	m_bCustomFolder = bCustomFolder;*/
}

afx_msg LRESULT CMainFrame::OnSendListdlg(WPARAM wParam, LPARAM lParam)
{
	m_pFormListDlg = (FormListDlg *)wParam;
	return 0;
}
