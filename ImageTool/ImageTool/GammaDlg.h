#if !defined(AFX_GAMMADLG_H__76F1E6A1_D16E_4556_A3D4_D6149DF4438F__INCLUDED_)
#define AFX_GAMMADLG_H__76F1E6A1_D16E_4556_A3D4_D6149DF4438F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GammaDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGammaDlg dialog
class CGammaDlg : public CDialog
{
// Construction
public:
	CGammaDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CGammaDlg)
	enum { IDD = IDD_DIALOG_GAMMA };
	float	m_gamma;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGammaDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CGammaDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
#endif // !defined(AFX_GAMMADLG_H__76F1E6A1_D16E_4556_A3D4_D6149DF4438F__INCLUDED_)
