#pragma once


// CEditDlg ��ȭ �����Դϴ�.

class CEditDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CEditDlg)

public:
	CEditDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CEditDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_EDIT_DIALOG };
	CString m_edit_str;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
