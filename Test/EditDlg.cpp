// EditDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Test.h"
#include "EditDlg.h"
#include "afxdialogex.h"
#include "resource.h"

// CEditDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CEditDlg, CDialogEx)

CEditDlg::CEditDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEditDlg::IDD, pParent)
{

}

CEditDlg::~CEditDlg()
{
}

void CEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CEditDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CEditDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CEditDlg �޽��� ó�����Դϴ�.


void CEditDlg::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CEdit *pEdit = (CEdit*)GetDlgItem(IDC_ADDRESS_EDIT);	
	pEdit->GetWindowText(m_edit_str);
	CDialogEx::OnOK();
}
