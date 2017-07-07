
// MainFrm.cpp : CMainFrame Ŭ������ ����
//

#include "stdafx.h"
#include "EPRE.h"

#include "MainFrm.h"
#include "FormFolder.h"
#include "FormList.h"
#include "FormBMP.h"
#include "BMPZoomView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // ���� �� ǥ�ñ�
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame ����/�Ҹ�

CMainFrame::CMainFrame()
{
	// TODO: ���⿡ ��� �ʱ�ȭ �ڵ带 �߰��մϴ�.
}

CMainFrame::~CMainFrame()
{
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
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}

// CMainFrame ����

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


// CMainFrame �޽��� ó����

void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
	// �� â���� ��Ŀ���� �̵��մϴ�.
	
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// �信�� ù° ũ���� �ش� ��ɿ� ��Ÿ������ �մϴ�.	

	// �׷��� ������ �⺻ ó���մϴ�.
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}



BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{	
	// �������� ������.
	if(!m_wndSplitter.CreateStatic(this, 1, 2)) {
		TRACE0("Fail to left vertical splitter...");
		return FALSE;
	}

	int m_IDpane = m_wndSplitter.IdFromRowCol(0, 0);//���� ������ ���̵� ����
	int m_IDpane2 = m_wndSplitter.IdFromRowCol(0, 1);//������ ������ ���̵� ����
	//���ʿ��� ������ ����
	if(!m_wndSplitter2.CreateStatic(&m_wndSplitter, 2, 1, WS_CHILD|WS_VISIBLE, m_IDpane)) {
		TRACE0("Fail to left horizontal splitter...");
		return FALSE;
	}
	//�����ʿ��� ���� ����
	if(!m_wndSplitter3.CreateStatic(&m_wndSplitter, 2, 1, WS_CHILD|WS_VISIBLE, m_IDpane2)) {
		TRACE0("Fail to right horizontal splitter...");
		return FALSE;
	}
	// Main View (link ��� ��) �� ����(���� ���, �ϴ�)
	if(!m_wndSplitter2.CreateView(0, 0, RUNTIME_CLASS(FormFolder), CSize(800, 200), pContext)||
		!m_wndSplitter2.CreateView(1, 0, RUNTIME_CLASS(FormBMP), CSize(500, 10), pContext)) {
		TRACE0("Fail to create MainView...");
		return FALSE;
	}
	// Main View (link ��� ��) �� ����(������ ���, �ϴ�)
	if(!m_wndSplitter3.CreateView(0, 0, RUNTIME_CLASS(CBMPZoomView), CSize(800, 200), pContext)||
		!m_wndSplitter3.CreateView(1, 0, RUNTIME_CLASS(FormBMP), CSize(800, 10), pContext)) {
		TRACE0("Fail to create MainView...");
		return FALSE;
	}
	
	// ���� ���� �������� ũ��
	m_wndSplitter.SetColumnInfo( 0, 250, 20 );
	// ���� ���� ���� �������� ũ��
	m_wndSplitter2.SetRowInfo( 0, 300, 10 );
	m_wndSplitter3.SetRowInfo( 0, 400, 10 );
	
	// Ȱ��â ����
	SetActiveView((CView *)m_wndSplitter3.GetPane(0, 0));

	return TRUE;
}
