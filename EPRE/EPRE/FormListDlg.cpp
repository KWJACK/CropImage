// FormListDlg.cpp : ���� �����Դϴ�.
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


// FormListDlg �����Դϴ�.

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
	//View�� ��ũ�� �� ����
	CSize scrollSize(0, 0);
	SetScrollSizes(MM_TEXT, scrollSize);

	//View ũ�⸦ ���� �� �ִ� ũ��� Ȯ��
	GetClientRect(rt);
	m_cListCtrlRem.MoveWindow(rt);
	//�ش� Ŭ������ �ν��Ͻ� �ּҸ� MainFrame���� ����
	::SendMessage(AfxGetMainWnd()->m_hWnd, SEND_LISTDLG, (WPARAM)this,0);

}


void FormListDlg::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);
	if (rt.Width() !=0) {
		GetClientRect(rt);
		m_cListCtrlRem.MoveWindow(rt);
	}
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}
