//	zoomctrl.cpp
//---------------------------------------------------------------------
/*
	Child control with zoom and scroll

	Jim Dill [jim@jimdill.net]
	v 1.0, Mar 25 2008
*/
//---------------------------------------------------------------------
#include "stdafx.h"
#include "zoomctrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//---------------------------------------------------------------------
CZoomCtrl::CZoomCtrl()
{
    m_scrollHelper.AttachWnd(this);
	m_zoomFactor = 1.0;
}
//---------------------------------------------------------------------
CZoomCtrl::~CZoomCtrl()
{
}
//---------------------------------------------------------------------

BEGIN_MESSAGE_MAP(CZoomCtrl, CWnd)
	ON_WM_PAINT()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_MOUSEACTIVATE()
    ON_WM_MOUSEWHEEL()
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

//---------------------------------------------------------------------
void CZoomCtrl::Draw(CDC *pDC)
{
	// fill background
	COLORREF backgrColor = RGB(255,255,192);
	CRect rClient;
	GetClientRect(rClient);
	pDC->FillRect(&rClient, &CBrush(backgrColor));

	// define virtual drawing space -- change this to suit
	// if drawing must not distort, then create a rect same shape as rClient
	CRect rVirt(0, 0, 5000, 5000);
	PrepDC(pDC, rVirt, rClient);

	// do your drawing here, in virtual coords

	// here's our sample
	CRect rTest(2000, 2200, 3000, 2800);
	CPen penBrown(PS_SOLID, 0, RGB(192,110,0));
	CPen penGreen(PS_SOLID, 0, RGB(0,192,0));
	CPen* pOldPen = pDC->SelectObject(&penGreen);
	pDC->Ellipse(&rTest);

	pDC->SelectObject(&penBrown);
	pDC->MoveTo(0, 0);
	pDC->LineTo(5000, 5000);
	pDC->MoveTo(5000, 0);
	pDC->LineTo(0, 5000);

	pDC->SelectObject(pOldPen);
}
//---------------------------------------------------------------------
void CZoomCtrl::OnPaint()
{
	CPaintDC dc(this);
	Draw(&dc);
}
//---------------------------------------------------------------------
void CZoomCtrl::PrepDC(CDC *pDC, const CRect& rVirt, const CRect& rScreen)
{
	// maps virtual into client
	m_rVirt = rVirt;
	m_rScreen = rScreen;

	pDC->IntersectClipRect(&rScreen);

	pDC->SetMapMode(MM_ANISOTROPIC);
	pDC->SetWindowExt(rVirt.Width(), rVirt.Height());

	LONG wid = (LONG)(m_zoomFactor * (double)rScreen.Width());
	LONG hgt = (LONG)(m_zoomFactor * (double)rScreen.Height());
	pDC->SetViewportExt(wid, hgt);

	CSize scrollPos = m_scrollHelper.GetScrollPos();
	pDC->SetViewportOrg(-scrollPos.cx, -scrollPos.cy);
}
//---------------------------------------------------------------------
void CZoomCtrl::AdjustScrollbars()
{
	int xMax = (int)((double)m_rScreen.Width() * m_zoomFactor);
	int yMax = (int)((double)m_rScreen.Height() * m_zoomFactor);
	m_scrollHelper.SetPageSize(m_rScreen.Width(), m_rScreen.Height());
	m_scrollHelper.SetDisplaySize(xMax, yMax);
}
//---------------------------------------------------------------------
void CZoomCtrl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    m_scrollHelper.OnHScroll(nSBCode, nPos, pScrollBar);
	Invalidate();
}
//---------------------------------------------------------------------
void CZoomCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    m_scrollHelper.OnVScroll(nSBCode, nPos, pScrollBar);
	Invalidate();
}
//---------------------------------------------------------------------
BOOL CZoomCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
    return m_scrollHelper.OnMouseWheel(nFlags, zDelta, pt);
}
//---------------------------------------------------------------------
LRESULT CZoomCtrl::WindowProc(UINT msg, WPARAM wParam, LPARAM lParam) 
{
	// necessary for getting h/vscroll msgs
    if (msg == WM_NCHITTEST || msg == WM_NCLBUTTONDOWN || msg == WM_NCLBUTTONDBLCLK) 
        return ::DefWindowProc(m_hWnd, msg, wParam, lParam); 
    return CWnd::WindowProc(msg, wParam, lParam); 
} 
//---------------------------------------------------------------------
int CZoomCtrl::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
     // necessary for getting mousewheel messages
	int status = CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
    SetFocus();
	return status;
}
//---------------------------------------------------------------------
BOOL CZoomCtrl::OnEraseBkgnd(CDC* pDC)
{
	// prevent erase to reduce flashing
	// for smoothest resize, also set clipchildren style in parent dialog
    return TRUE;
}
//---------------------------------------------------------------------