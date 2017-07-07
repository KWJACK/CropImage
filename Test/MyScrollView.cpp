// MyScrollView.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Test.h"
#include "MyScrollView.h"


// CMyScrollView

IMPLEMENT_DYNCREATE(CMyScrollView, CScrollView)

CMyScrollView::CMyScrollView()
{

}

CMyScrollView::~CMyScrollView()
{
}


BEGIN_MESSAGE_MAP(CMyScrollView, CScrollView)
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_VIEW_ZOOMIN, OnViewZoomin)
	ON_COMMAND(ID_VIEW_ZOOMOUT, OnViewZoomout)
END_MESSAGE_MAP()


// CMyScrollView �׸����Դϴ�.

void CMyScrollView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: ���⿡ �׸��� �ڵ带 �߰��մϴ�.
}


// CMyScrollView �����Դϴ�.

#ifdef _DEBUG
void CMyScrollView::AssertValid() const
{
	CScrollView::AssertValid();
}

#ifndef _WIN32_WCE
void CMyScrollView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif
#endif //_DEBUG


void CMyScrollView::OnInitialUpdate()
{
	CZoomView::OnInitialUpdate();
	//CScrollView::OnInitialUpdate();
	//CSize sizeTotal;
	// TODO: �� ���� ��ü ũ�⸦ ����մϴ�.
	//sizeTotal.cx = sizeTotal.cy = 100;
	//SetScrollSizes(MM_TEXT, sizeTotal);
}

// CMyScrollView �޽��� ó�����Դϴ�.

void CMyScrollView::OnViewZoomin()
{
	SetZoomScale(m_zoomFactor + 1.0f);
}

void CMyScrollView::OnViewZoomout()
{
	SetZoomScale(m_zoomFactor - 1.0f);
}


void CMyScrollView::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_bSelectMode = FALSE;
	Invalidate(FALSE);

	SetZoomArea(m_rubberBand);

	CZoomView::OnLButtonUp(nFlags, point);
}


void CMyScrollView::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_bSelectMode == FALSE)
	{
		m_bSelectMode = TRUE;
		m_ptStart = point;
		DPtoLP(&m_ptStart);
		m_rubberBand.SetRect(m_ptStart, m_ptStart);
		Invalidate(FALSE);
	}

	CZoomView::OnLButtonDown(nFlags, point);
}


void CMyScrollView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_bSelectMode != FALSE)
	{
		DPtoLP(&point);
		m_rubberBand.SetRect(m_ptStart, point);
		m_rubberBand.NormalizeRect();
	}

	Invalidate(FALSE);
	CZoomView::OnMouseMove(nFlags, point);
}


BOOL CMyScrollView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	return CZoomView::PreCreateWindow(cs);
}
