#pragma once


// CSetResultPathDlg 대화 상자입니다.

class CSetResultPathDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSetResultPathDlg)

public:
	CSetResultPathDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSetResultPathDlg();
	CString m_reslut_path;
// 대화 상자 데이터입니다.
	enum { IDD = IDD_SAVE_RESULT_PATH_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
