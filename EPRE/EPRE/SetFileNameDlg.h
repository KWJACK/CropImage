#pragma once


// CSetFileNameDlg ��ȭ �����Դϴ�.

class CSetFileNameDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSetFileNameDlg)

public:
	CSetFileNameDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CSetFileNameDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_SAVE__IMAGE_DLG };
	CString m_fileName;
	BOOL m_IDOK;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:	
};
