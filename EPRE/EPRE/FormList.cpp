// FormList.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "EPRE.h"
#include "FormList.h"


// FormList

IMPLEMENT_DYNCREATE(FormList, CFormView)

FormList::FormList()
	: CFormView(FormList::IDD)
{

}

FormList::~FormList()
{
}

void FormList::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(FormList, CFormView)
END_MESSAGE_MAP()


// FormList �����Դϴ�.

#ifdef _DEBUG
void FormList::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void FormList::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// FormList �޽��� ó�����Դϴ�.
