// SetImagePathDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EPRE.h"
#include "SetImagePathDlg.h"
#include "afxdialogex.h"


// SetImagePathDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(SetImagePathDlg, CDialogEx)

SetImagePathDlg::SetImagePathDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(SetImagePathDlg::IDD, pParent)
{

}

SetImagePathDlg::~SetImagePathDlg()
{
}

void SetImagePathDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_IMAGEPATH_EDIT, m_path);
}


BEGIN_MESSAGE_MAP(SetImagePathDlg, CDialogEx)
END_MESSAGE_MAP()


// SetImagePathDlg 메시지 처리기입니다.
