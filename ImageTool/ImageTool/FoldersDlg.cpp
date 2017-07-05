// FoldersDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FoldersDlg.h"
#include "MainFrm.h"
#include "ImageToolDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFoldersDlg dialog
CFoldersDlg::CFoldersDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFoldersDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFoldersDlg)
	//}}AFX_DATA_INIT
}

void CFoldersDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFoldersDlg)
	DDX_Control(pDX, IDC_BROWSE_TREE, m_browseTree);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFoldersDlg, CDialog)
	//{{AFX_MSG_MAP(CFoldersDlg)
	ON_NOTIFY(  TVN_SELCHANGED,    IDC_BROWSE_TREE, OnSelchangedBrowseTree    )
	ON_WM_SIZE(                                                               )
	ON_NOTIFY(  TVN_ITEMEXPANDING, IDC_BROWSE_TREE, OnItemexpandingBrowseTree )
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFoldersDlg message handlers
BOOL CFoldersDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_browseTree.EnableImages();
	m_browseTree.PopulateTree();

	return TRUE;
}

void CFoldersDlg::OnSelchangedBrowseTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	CString sPath; sPath.Empty();
	
	if( m_browseTree.OnFolderSelected( pNMHDR, pResult, sPath ) )
	{
		CImageToolDoc* pDoc=(CImageToolDoc*)((CMainFrame*)AfxGetMainWnd())->GetActiveDocument();
		pDoc->SelectFolder( sPath );
	}

	*pResult = 0;
}

void CFoldersDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	if( m_browseTree.m_hWnd != NULL )
	{
		CRect rc; GetClientRect(rc);
		m_browseTree.MoveWindow( rc );
	}
}

void CFoldersDlg::OnItemexpandingBrowseTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	m_browseTree.OnFolderExpanding(pNMHDR,pResult);
	
	*pResult = 0;
}
