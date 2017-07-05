#if !defined(AFX_FOLDERSDLG_H__21D94DF2_1D23_4C8C_9D96_8216A8AB9C31__INCLUDED_)
#define AFX_FOLDERSDLG_H__21D94DF2_1D23_4C8C_9D96_8216A8AB9C31__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FoldersDlg.h : header file
//
#include "imagetool.h"
#include "FoldersTreeCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CFoldersDlg dialog
class CFoldersDlg : public CDialog
{
// Construction
public:
	CFoldersDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFoldersDlg)
	enum { IDD = IDD_BROWSER_DLG };
	//}}AFX_DATA

	CFoldersTreeCtrl m_browseTree;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFoldersDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CFoldersDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangedBrowseTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnItemexpandingBrowseTree(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FOLDERSDLG_H__21D94DF2_1D23_4C8C_9D96_8216A8AB9C31__INCLUDED_)
