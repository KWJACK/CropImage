// ZoomCtrlDemoDlg.h : header file
//

#pragma once

#include "zoomctrl.h"

// CZoomCtrlDemoDlg dialog
class CZoomCtrlDemoDlg : public CDialog
{
// Construction
public:
	CZoomCtrlDemoDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_ZOOMCTRLDEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	CZoomCtrl m_zoomCtrl;
	double m_zoomFactor;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedZoom();
	afx_msg void OnBnClickedFit();
};
