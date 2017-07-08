// FormListDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EPRE.h"
#include "FormListDlg.h"

#define SEND_LISTDLG WM_USER+1
// FormListDlg

IMPLEMENT_DYNCREATE(FormListDlg, CFormView)

FormListDlg::FormListDlg()
	: CFormView(FormListDlg::IDD)
{

}

FormListDlg::~FormListDlg()
{
}

void FormListDlg::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_REM, m_cListCtrlRem);
}

BEGIN_MESSAGE_MAP(FormListDlg, CFormView)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// FormListDlg 진단입니다.

#ifdef _DEBUG
void FormListDlg::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void FormListDlg::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG

void FormListDlg::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	// Not displaying subfolders	
	m_cListCtrlRem.SetItemTypes(SHCONTF_NONFOLDERS);
	// Passing project files map pointer to both shell list controls	
	m_cListCtrlRem.SetProjectFiles(&m_cProjFileMap);
	// Setting list control filters	
	m_cListCtrlRem.SetFilter(LISTFILTER_REMAINING);	 	
	//View의 스크롤 바 제거
	CSize scrollSize(0, 0);
	SetScrollSizes(MM_TEXT, scrollSize);

	//View 크기를 얻은 후 최대 크기로 확대
	GetClientRect(rt);
	m_cListCtrlRem.MoveWindow(rt);
	//해당 클래스의 인스턴스 주소를 MainFrame으로 보냄
	::SendMessage(AfxGetMainWnd()->m_hWnd, SEND_LISTDLG, (WPARAM)this,0);

}


void FormListDlg::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);
	if (rt.Width() !=0) {
		GetClientRect(rt);
		m_cListCtrlRem.MoveWindow(rt);
	}
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}
