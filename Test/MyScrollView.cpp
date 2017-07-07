// MyScrollView.cpp : 구현 파일입니다.
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


// CMyScrollView 그리기입니다.

void CMyScrollView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
}


// CMyScrollView 진단입니다.

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
	// TODO: 이 뷰의 전체 크기를 계산합니다.
	//sizeTotal.cx = sizeTotal.cy = 100;
	//SetScrollSizes(MM_TEXT, sizeTotal);
}

// CMyScrollView 메시지 처리기입니다.

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
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CZoomView::PreCreateWindow(cs);
}
