#pragma once
#include "ProjectListCtrl.h"
#include "ProjFilesArray.h"
// FormListDlg 폼 뷰입니다.
class FormListDlg : public CFormView
{
	DECLARE_DYNCREATE(FormListDlg)

protected:
	FormListDlg();           // 동적 만들기에 사용되는 protected 생성자입니다.
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CProjectListCtrl m_cListCtrlRem;
	CProjFilesArray m_cProjFileMap;	
	virtual void OnInitialUpdate();

};


