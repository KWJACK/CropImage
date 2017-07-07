// BMPZoomView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EPRE.h"
#include "BMPZoomView.h"


// CBMPZoomView

IMPLEMENT_DYNCREATE(CBMPZoomView, CView)

CBMPZoomView::CBMPZoomView()
{
	m_bSelectMode = FALSE;
}

CBMPZoomView::~CBMPZoomView()
{
}

BEGIN_MESSAGE_MAP(CBMPZoomView, CView)
	ON_COMMAND(ID_VIEW_ZOOMIN, OnViewZoomin)
	ON_COMMAND(ID_VIEW_ZOOMOUT, OnViewZoomout)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_MOUSEHWHEEL()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()


// CBMPZoomView 그리기입니다.

void CBMPZoomView::OnDraw(CDC* pDC)
{
	/*TRACE("DemoZoom::OnDraw\n");
	CDocument* pDoc = GetDocument();
	ASSERT_VALID(pDoc);*/
	// TODO: 여기에 그리기 코드를 추가합니다.
	srand(0);
	int width = 2000/10;
	int height = 1000/10;
	int i = 0;
	for (i = 0; i < 10; i++)
	{
		pDC->FillSolidRect(CRect(i * width, i * height, 
								 i * width + width, i * height + height), RGB(rand() % 255, rand() % 255, rand() % 255));
	}
	if (m_bSelectMode == TRUE) 
	{
		CPen* pPen = pDC->GetCurrentPen();
		LOGPEN logPen;
		pPen->GetLogPen(&logPen);
		logPen.lopnColor = RGB(133,142,144);
		logPen.lopnStyle = PS_SOLID;

		LOGBRUSH logBrush;
		::ZeroMemory(&logBrush,sizeof(logBrush));
		logBrush.lbStyle = BS_HOLLOW;
		
		CPen newPen;
		newPen.CreatePenIndirect(&logPen);
		CBrush newBrush;
		newBrush.CreateBrushIndirect(&logBrush);
		CPen* oldPen = pDC->SelectObject(&newPen);
		CBrush* oldBrush = pDC->SelectObject(&newBrush);

		pDC->Rectangle(m_rubberBand);

		pDC->SelectObject(oldBrush);
		pDC->SelectObject(oldPen);
	}
}


// CBMPZoomView 진단입니다.

#ifdef _DEBUG
void CBMPZoomView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CBMPZoomView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG

BOOL CBMPZoomView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	return CZoomView::PreCreateWindow(cs);
}


void CBMPZoomView::OnInitialUpdate()
{
	CZoomView::OnInitialUpdate();
}


BOOL CBMPZoomView::OnPreparePrinting(CPrintInfo* pInfo)
{	
	return CZoomView::OnPreparePrinting(pInfo);
}


void CBMPZoomView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{	
	CZoomView::OnBeginPrinting(pDC, pInfo);
}


void CBMPZoomView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	CZoomView::OnEndPrinting(pDC, pInfo);
}


// CBMPZoomView 메시지 처리기입니다.
void CBMPZoomView::OnViewZoomin() 
{
	SetZoomScale(m_zoomFactor + 1.0f);
}

void CBMPZoomView::OnViewZoomout() 
{
	SetZoomScale(m_zoomFactor - 1.0f);
}

void CBMPZoomView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
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


void CBMPZoomView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_bSelectMode = FALSE;
	Invalidate(FALSE);

	SetZoomArea(m_rubberBand);
	CZoomView::OnLButtonUp(nFlags, point);
}


void CBMPZoomView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_bSelectMode != FALSE) 
	{
		DPtoLP(&point);
		m_rubberBand.SetRect(m_ptStart, point);
		m_rubberBand.NormalizeRect();
	}
	
	Invalidate(FALSE);
	CZoomView::OnMouseMove(nFlags, point);
}



void CBMPZoomView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CZoomView::OnVScroll(nSBCode, nPos, pScrollBar);
}


void CBMPZoomView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CZoomView::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CBMPZoomView::OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// 이 기능을 사용하려면 Windows Vista 이상이 있어야 합니다.
	// _WIN32_WINNT 기호는 0x0600보다 크거나 같아야 합니다.
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	DoMouseWheel(nFlags, zDelta, pt);
	//CZoomView::OnMouseHWheel(nFlags, zDelta, pt);
}


BOOL CBMPZoomView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	
   return DoMouseWheel(nFlags, zDelta, pt);
	//return CZoomView::OnMouseWheel(nFlags, zDelta, pt);
}
