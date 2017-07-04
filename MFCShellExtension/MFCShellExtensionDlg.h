
// MFCShellExtensionDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include "ProjectListCtrl.h"
#include "ProjFilesArray.h"
#include "MFCShellTreeCtrlEx.h"

// CMFCShellExtensionDlg dialog
class CMFCShellExtensionDlg : public CDialogEx
{
// Construction
public:
	CMFCShellExtensionDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_MFCSHELLEXTENSION_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	BOOL m_bCustomFolder;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CMFCShellTreeCtrlEx m_cTreeCtrl;
	CProjectListCtrl m_cListCtrlSel;
	CProjectListCtrl m_cListCtrlRem;
	CProjFilesArray m_cProjFileMap;
	CString m_cRootFolder;
	void CopyFiles(BOOL bDelete);

	afx_msg void OnBnClickedDelFiles();
	afx_msg void OnBnClickedAddFiles();
	afx_msg void OnRootFolderChanged(UINT uID);
	afx_msg void OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult);
};
