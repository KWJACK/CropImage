#pragma once


// CSetFileNameDlg 대화 상자입니다.

class CSetFileNameDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSetFileNameDlg)

public:
	CSetFileNameDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSetFileNameDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SAVE__IMAGE_DLG };
	CString m_fileName;
	BOOL m_IDOK;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:	
};
