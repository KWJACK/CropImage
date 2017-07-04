// SelRootFolderDlg.cpp : implementation file
//
#include "stdafx.h"
#include "MFCShellExtension.h"
#include "SelRootFolderDlg.h"
#include "afxdialogex.h"

// CSelRootFolderDlg dialog

IMPLEMENT_DYNAMIC(CSelRootFolderDlg, CDialog)

CSelRootFolderDlg::CSelRootFolderDlg(CWnd* pParent) :
	CDialog(CSelRootFolderDlg::IDD, pParent), m_cTreeCtrl(SHELLTREEEX_QUICK_CHLDDETECT)
{

}

CSelRootFolderDlg::~CSelRootFolderDlg()
{
}

void CSelRootFolderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE, m_cTreeCtrl);
}

BEGIN_MESSAGE_MAP(CSelRootFolderDlg, CDialog)
END_MESSAGE_MAP()

// CSelRootFolderDlg message handlers
BOOL CSelRootFolderDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CString cFolder = m_cRootFolder.IsEmpty() ? _T("C:\\") : m_cRootFolder;
	m_cTreeCtrl.SelectPath(cFolder);
	return TRUE;
}

void CSelRootFolderDlg::OnOK()
{
	m_cTreeCtrl.GetItemPath(m_cRootFolder);
	CDialog::OnOK();
}
