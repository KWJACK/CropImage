// FormBMP.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "EPRE.h"
#include "FormBMP.h"


// FormBMP

IMPLEMENT_DYNCREATE(FormBMP, CFormView)

FormBMP::FormBMP()
	: CFormView(FormBMP::IDD)
{

}

FormBMP::~FormBMP()
{
}

void FormBMP::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(FormBMP, CFormView)
END_MESSAGE_MAP()


// FormBMP �����Դϴ�.

#ifdef _DEBUG
void FormBMP::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void FormBMP::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// FormBMP �޽��� ó�����Դϴ�.
