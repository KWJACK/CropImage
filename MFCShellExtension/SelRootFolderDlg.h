#pragma once
#include "afxcmn.h"
#include "MFCShellTreeCtrlEx.h"

// CSelRootFolderDlg dialog

class CSelRootFolderDlg : public CDialog
{
	DECLARE_DYNAMIC(CSelRootFolderDlg)

public:
	CSelRootFolderDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSelRootFolderDlg();

// Dialog Data
	enum { IDD = IDD_SELFOLDER_DLG };
	CString m_cRootFolder;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CMFCShellTreeCtrlEx m_cTreeCtrl;
	virtual BOOL OnInitDialog();
	virtual void OnOK();
};
