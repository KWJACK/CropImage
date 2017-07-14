#pragma once
#include "myGdiPlus.h" 
// FormBMP 폼 뷰입니다.
using namespace Gdiplus;
class FormBMP : public CFormView
{
	DECLARE_DYNCREATE(FormBMP)

protected:
	FormBMP();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~FormBMP();

	Graphics* p_graphics;
public:
	enum { IDD = IDD_FORM_BMP };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};


