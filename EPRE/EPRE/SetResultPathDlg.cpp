// SetResultPathDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "EPRE.h"
#include "SetResultPathDlg.h"
#include "afxdialogex.h"


// CSetResultPathDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CSetResultPathDlg, CDialogEx)

CSetResultPathDlg::CSetResultPathDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSetResultPathDlg::IDD, pParent)
{

}

CSetResultPathDlg::~CSetResultPathDlg()
{
}

void CSetResultPathDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FILENAME_EDIT, m_reslut_path);
}


BEGIN_MESSAGE_MAP(CSetResultPathDlg, CDialogEx)
END_MESSAGE_MAP()


// CSetResultPathDlg �޽��� ó�����Դϴ�.
