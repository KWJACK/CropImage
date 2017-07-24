#pragma once
#include "myGdiPlus.h" 
#include "afxwin.h"
// FormBMP �� ���Դϴ�.
using namespace Gdiplus;
class FormBMP : public CFormView
{
	DECLARE_DYNCREATE(FormBMP)

protected:
	FormBMP();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);	
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	OPTIONAL int m_saveFlag;//BMPZoomView���� ������ �ٲ������ ��Ÿ�����÷���
	IN Bitmap  *m_pSelectedImage;
	afx_msg void OnPaint();
};


