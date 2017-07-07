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
	//�ش� Ŭ������ �ν��Ͻ� �ּҸ� MainFrame���� ����
	::SendMessage(AfxGetMainWnd()->m_hWnd, SEND_LISTDLG, (WPARAM)this,0);
}
