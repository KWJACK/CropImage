#pragma once



// FormList �� ���Դϴ�.

class FormList : public CFormView
{
	DECLARE_DYNCREATE(FormList)

protected:
	FormList();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~FormList();

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
};


