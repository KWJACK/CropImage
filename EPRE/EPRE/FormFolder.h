#pragma once
// FormFolder 폼 뷰입니다.

class FormFolder : public CFormView
{
	DECLARE_DYNCREATE(FormFolder)

protected:
	FormFolder();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~FormFolder();

public:
	enum { IDD = IDD_FORM_FOLDER };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	CWnd* m_target;
	DECLARE_MESSAGE_MAP()
public:
	void SetTarget(CWnd* m_cwnd);	
};


