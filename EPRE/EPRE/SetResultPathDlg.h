#pragma once


// CSetResultPathDlg ��ȭ �����Դϴ�.

class CSetResultPathDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSetResultPathDlg)

public:
	CSetResultPathDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CSetResultPathDlg();
	CString m_reslut_path;
// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_SAVE_RESULT_PATH_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};
