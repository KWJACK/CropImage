#pragma once
#include "afxwin.h"


// CPreviewDlg ��ȭ �����Դϴ�.

class CPreviewDlg : public CDialog
{
	DECLARE_DYNAMIC(CPreviewDlg)

public:
	CPreviewDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CPreviewDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_PREVIEW_DLG };

	CStatic m_wndCanvas;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);	
};
