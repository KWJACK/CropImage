// FormFolderDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EPRE.h"
#include "FormFolderDlg.h"
#include "MainFrm.h"
#define GET_LISTDLG WM_USER+2
// FormFolderDlg

IMPLEMENT_DYNCREATE(FormFolderDlg, CFormView)

FormFolderDlg::FormFolderDlg(): CFormView(FormFolderDlg::IDD)
{
	
}

FormFolderDlg::~FormFolderDlg()
{
}

void FormFolderDlg::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FOLDERS_TREE, m_cTreeCtrl);	
}

BEGIN_MESSAGE_MAP(FormFolderDlg, CFormView)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// FormFolderDlg 진단입니다.

#ifdef _DEBUG
void FormFolderDlg::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void FormFolderDlg::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG

BOOL FormFolderDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
	if(m_target)
	{		
		m_target->SendMessage(WM_COMMAND, wParam, lParam);
	}
	else
	{
		CFormView::OnCommand(wParam, lParam);
	}
	return true;	
}

void FormFolderDlg::SetTarget(CWnd* m_cwnd)
{
	m_target = m_cwnd;
}



// FormFolderDlg 메시지 처리기입니다.


int FormFolderDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	return 0;
}


void FormFolderDlg::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	HTREEITEM hParentItem = m_cTreeCtrl.GetRootItem();

	//MainFrame의 주소를 얻어서 MainFrame이 갖고 있는 ListDlg 값을 건네 준다
	CMainFrame* myFrame = (CMainFrame* )GetParentFrame();
	m_cTreeCtrl.m_pFormListDlg = myFrame->m_pFormListDlg;

	m_cTreeCtrl.SelectItem(hParentItem);
	m_cTreeCtrl.Expand(hParentItem, TVE_EXPAND);	

	CSize scrollSize(0, 0);//View의 스크롤 바 제거
	SetScrollSizes(MM_TEXT, scrollSize);

	GetClientRect(rt);//View 크기만큼 얻어서 컨트롤 크기 확대
	m_cTreeCtrl.MoveWindow(rt);
	
}


void FormFolderDlg::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);
	if (rt.Width() != 0) {
		GetClientRect(rt);
		m_cTreeCtrl.MoveWindow(rt);
	}
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}
