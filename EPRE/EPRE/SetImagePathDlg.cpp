// SetImagePathDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "EPRE.h"
#include "SetImagePathDlg.h"
#include "afxdialogex.h"


// SetImagePathDlg ��ȭ �����Դϴ�.

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


// SetImagePathDlg �޽��� ó�����Դϴ�.
