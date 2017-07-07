#pragma once
// FormFolder �� ���Դϴ�.

class FormFolder : public CFormView
{
	DECLARE_DYNCREATE(FormFolder)

protected:
	FormFolder();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	CWnd* m_target;
	DECLARE_MESSAGE_MAP()
public:
	void SetTarget(CWnd* m_cwnd);	
};


