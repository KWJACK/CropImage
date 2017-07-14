#pragma once


// CEditDlg 대화 상자입니다.

class CEditDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CEditDlg)

public:
	CEditDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CEditDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_EDIT_DIALOG };
	CString m_edit_str;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
