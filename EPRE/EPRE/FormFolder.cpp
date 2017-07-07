// FormFolder.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EPRE.h"
#include "FormFolder.h"


// FormFolder

IMPLEMENT_DYNCREATE(FormFolder, CFormView)

FormFolder::FormFolder()
	: CFormView(FormFolder::IDD)
{

}

FormFolder::~FormFolder()
{
}

void FormFolder::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(FormFolder, CFormView)
END_MESSAGE_MAP()


// FormFolder 진단입니다.

#ifdef _DEBUG
void FormFolder::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void FormFolder::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG

BOOL FormFolder::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
	if(m_target)
	{		
		m_target->SendMessage(WM_COMMAND, wParam, lParam);
	}
	else
	{
		CFormView::OnCommand(wParam, lParam);
	}
	return true;	
}

void FormFolder::SetTarget(CWnd* m_cwnd)
{
	m_target = m_cwnd;
}



// FormFolder 메시지 처리기입니다.
