#pragma once
// FormFolderDlg �� ���Դϴ�.
#include "MFCShellTreeCtrlEx.h"

class FormFolderDlg : public CFormView
{
	DECLARE_DYNCREATE(FormFolderDlg)

protected:
	FormFolderDlg();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~FormFolderDlg();
	CMFCShellTreeCtrlEx m_cTreeCtrl;
public:
	enum { IDD = IDD_FORM_FOLDER };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	CWnd* m_target;
	DECLARE_MESSAGE_MAP()
public:
	void SetTarget(CWnd* m_cwnd);	
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void OnInitialUpdate();
};


