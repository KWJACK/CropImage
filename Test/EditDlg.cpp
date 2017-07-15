// EditDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Test.h"
#include "EditDlg.h"
#include "afxdialogex.h"
#include "TestDlg.h"
#include "resource.h"

// CEditDlg 대화 상자입니다.

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


// CEditDlg 메시지 처리기입니다.


void CEditDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CEdit *pEdit = (CEdit*)GetDlgItem(IDC_ADDRESS_EDIT);	
	pEdit->GetWindowText(m_edit_str);	
	CString check_ = L"_";
	CTestDlg* pParentDlg = (CTestDlg* )GetParent();
	if(m_edit_str.GetLength() >6){
		if(m_edit_str.GetAt(6) != check_ ){
			AfxMessageBox(L"예시 포맷에 맞춰서 작성해야합니다. adXXXX_");						
		}else{
			
			pParentDlg->PaletteChange1bpp(m_edit_str);
		}
	}else{
		AfxMessageBox(L"잘못입력하셨습니다");			
	}
	CDialogEx::OnOK();
}
