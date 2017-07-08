// FormFolderDlg.cpp : ���� �����Դϴ�.
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


// FormFolderDlg �����Դϴ�.

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



// FormFolderDlg �޽��� ó�����Դϴ�.


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

	//MainFrame�� �ּҸ� �� MainFrame�� ���� �ִ� ListDlg ���� �ǳ� �ش�
	CMainFrame* myFrame = (CMainFrame* )GetParentFrame();
	m_cTreeCtrl.m_pFormListDlg = myFrame->m_pFormListDlg;

	m_cTreeCtrl.SelectItem(hParentItem);
	m_cTreeCtrl.Expand(hParentItem, TVE_EXPAND);	

	CSize scrollSize(0, 0);//View�� ��ũ�� �� ����
	SetScrollSizes(MM_TEXT, scrollSize);

	GetClientRect(rt);//View ũ�⸸ŭ �� ��Ʈ�� ũ�� Ȯ��
	m_cTreeCtrl.MoveWindow(rt);
	
}


void FormFolderDlg::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);
	if (rt.Width() != 0) {
		GetClientRect(rt);
		m_cTreeCtrl.MoveWindow(rt);
	}
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}
