// SetFileNameDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "EPRE.h"
#include "SetFileNameDlg.h"
#include "afxdialogex.h"


// CSetFileNameDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CSetFileNameDlg, CDialogEx)

CSetFileNameDlg::CSetFileNameDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSetFileNameDlg::IDD, pParent)
{
	m_IDOK = FALSE;
}

CSetFileNameDlg::~CSetFileNameDlg()
{
}

void CSetFileNameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);		
	DDX_Text(pDX, IDC_FILENAME_EDIT, m_fileName);
}


BEGIN_MESSAGE_MAP(CSetFileNameDlg, CDialogEx)
END_MESSAGE_MAP()


