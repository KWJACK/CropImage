#pragma once


// SetImagePathDlg 대화 상자입니다.

class SetImagePathDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SetImagePathDlg)

public:
	SetImagePathDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~SetImagePathDlg();
	CString m_path;
// 대화 상자 데이터입니다.
	enum { IDD = IDD_SAVE_PATH_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
