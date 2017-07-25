// SetFileNameDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EPRE.h"
#include "SetFileNameDlg.h"
#include "afxdialogex.h"


// CSetFileNameDlg 대화 상자입니다.

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
	DDX_Text(pDX, IDC_FILENAME_EDIT, m_fileName);//Directly connect edit with CString variable
}


BEGIN_MESSAGE_MAP(CSetFileNameDlg, CDialogEx)
END_MESSAGE_MAP()




BOOL CSetFileNameDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetWindowPos(NULL, 300,700,0,0,SWP_NOSIZE);
	

	return TRUE;  // return TRUE unless you set the focus to a control
	
}
