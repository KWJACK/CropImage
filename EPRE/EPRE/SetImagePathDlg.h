#pragma once


// SetImagePathDlg ��ȭ �����Դϴ�.

class SetImagePathDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SetImagePathDlg)

public:
	SetImagePathDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~SetImagePathDlg();
	CString m_path;
// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_SAVE_PATH_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};
