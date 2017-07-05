// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "ImageTool.h"
#include "MainFrm.h"
#include "ImageToolView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame
IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE(                                                         )
	ON_COMMAND(           ID_VIEW_DIRECTORY_BAR, OnViewDirectoryBar       )
	ON_UPDATE_COMMAND_UI( ID_VIEW_DIRECTORY_BAR, OnUpdateViewDirectoryBar )
	ON_COMMAND(           ID_VIEW_PREVIEW_BAR,   OnViewPreviewBar         )
	ON_UPDATE_COMMAND_UI( ID_VIEW_PREVIEW_BAR,   OnUpdateViewPreviewBar   )
	ON_WM_CLOSE(                                                          )
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction
CMainFrame::CMainFrame()
{
	m_bShowPreviewBar = m_bShowDirectoryBar = TRUE;
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if( CFrameWnd::OnCreate(lpCreateStruct) == -1 )
	{
		return -1;
	}

	if( !m_wndToolBar.CreateEx( this,
		                        TBSTYLE_FLAT,
								WS_CHILD      | 
								WS_VISIBLE    |
								CBRS_TOP      |
								CBRS_GRIPPER  | 
								CBRS_TOOLTIPS |
								CBRS_FLYBY    | 
								CBRS_SIZE_DYNAMIC )
		|| !m_wndToolBar.LoadToolBar( IDR_MAINFRAME ) 
	  )
	{
		TRACE0("Failed to create toolbar\n");
		return -1;
	}

	if( !m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators( indicators,
		                               sizeof(indicators)/sizeof(UINT))
	  )
	{
		TRACE0("Failed to create status bar\n");
		return -1; 
	}

	m_wndToolBar.EnableDocking( CBRS_ALIGN_ANY );
	EnableDocking( CBRS_ALIGN_ANY );
	DockControlBar( &m_wndToolBar );

	return 0;
}

BOOL CMainFrame::PreCreateWindow( CREATESTRUCT& cs )
{
	cs.style &= ~FWS_ADDTOTITLE;
	cs.cx = 480;
	cs.cy = 740;
	cs.x  = 530;
	cs.y  = 10;

	if( !CFrameWnd::PreCreateWindow( cs ) )
	{
		return FALSE;
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics
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

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers
void CMainFrame::OnViewDirectoryBar() 
{
	CImageToolView* pView = (CImageToolView*)GetActiveView();

	if( m_bShowDirectoryBar )
	{
		::ShowWindow( pView->m_pFoldersDlg->m_hWnd, SW_HIDE );			
		m_bShowDirectoryBar = FALSE;

		RecalcLayout( TRUE );
	}
	else
	{
		::ShowWindow( pView->m_pFoldersDlg->m_hWnd, SW_SHOW );
		RecalcLayout( TRUE );
		m_bShowDirectoryBar = TRUE;
	}
}

void CMainFrame::OnUpdateViewDirectoryBar(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( m_bShowDirectoryBar );
}

void CMainFrame::OnViewPreviewBar() 
{
	CImageToolView* pView = (CImageToolView*)GetActiveView();

	if( m_bShowPreviewBar )
	{
		::ShowWindow( pView->m_pPreviewDlg->m_hWnd, SW_HIDE );			
		m_bShowPreviewBar = FALSE;

		RecalcLayout( TRUE );
	}
	else
	{
		::ShowWindow( pView->m_pPreviewDlg->m_hWnd, SW_SHOW );
		RecalcLayout( TRUE );
		m_bShowPreviewBar = TRUE;
	}
}

void CMainFrame::OnUpdateViewPreviewBar(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( m_bShowPreviewBar );	
}

void CMainFrame::OnClose() 
{
	CImageToolView* pView = (CImageToolView*)GetActiveView();

	if( pView->IsRunning() == TRUE ) 
	{
		pView->TerminateLoadThread();
	}
	else
	{
		CFrameWnd::OnClose();
	}
}