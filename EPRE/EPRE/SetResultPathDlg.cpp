// SetResultPathDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EPRE.h"
#include "SetResultPathDlg.h"
#include "afxdialogex.h"


// CSetResultPathDlg 대화 상자입니다.

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


// CSetResultPathDlg 메시지 처리기입니다.
