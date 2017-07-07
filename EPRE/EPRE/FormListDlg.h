#pragma once
#include "ProjectListCtrl.h"
#include "ProjFilesArray.h"
// FormListDlg �� ���Դϴ�.
class FormListDlg : public CFormView
{
	DECLARE_DYNCREATE(FormListDlg)

protected:
	FormListDlg();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~FormListDlg();
	
public:
	enum { IDD = IDD_FORM_LIST };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CProjectListCtrl m_cListCtrlRem;
	CProjFilesArray m_cProjFileMap;	
	virtual void OnInitialUpdate();

};


