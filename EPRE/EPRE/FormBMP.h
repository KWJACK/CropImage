#pragma once
#include "myGdiPlus.h" 
#include "afxwin.h"
// FormBMP 폼 뷰입니다.
using namespace Gdiplus;
class FormBMP : public CFormView
{
	DECLARE_DYNCREATE(FormBMP)

protected:
	FormBMP();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~FormBMP();

	Graphics* p_graphics;
	
	CStatic m_bmp_thumnail;
	CRect rt;
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
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);	
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	OPTIONAL int m_saveFlag;//BMPZoomView에서 사진이 바뀌었음을 나타내는플래그
	IN Bitmap  *m_pSelectedImage;
	afx_msg void OnPaint();
};


