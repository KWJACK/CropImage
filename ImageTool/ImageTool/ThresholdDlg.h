#if !defined(AFX_THRESHOLDDLG_H__29E668E7_06AC_4536_A3A1_765D3F62C769__INCLUDED_)
#define AFX_THRESHOLDDLG_H__29E668E7_06AC_4536_A3A1_765D3F62C769__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ThresholdDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CThresholdDlg dialog
class CThresholdDlg : public CDialog
{
// Construction
public:
	CThresholdDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CThresholdDlg)
	enum { IDD = IDD_DIALOG_THRE };
	BYTE	m_level;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CThresholdDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CThresholdDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_THRESHOLDDLG_H__29E668E7_06AC_4536_A3A1_765D3F62C769__INCLUDED_)
