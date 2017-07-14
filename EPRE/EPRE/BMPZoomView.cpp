// BMPZoomView.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "EPRE.h"
#include "BMPZoomView.h"
#include "memdc.h"	//include �ؾ� memdc ���� ���
#include "MainFrm.h"
// CBMPZoomView

IMPLEMENT_DYNCREATE(CBMPZoomView, CView)
using namespace Gdiplus; 
CBMPZoomView::CBMPZoomView() : m_create_canvas(FALSE)
{
	m_bSelectMode = FALSE;
	m_pSelectedImage = NULL;
	m_fResolution_H=0.0f;m_fResolution_W=0.0f;
	HDC hdc = ::GetDC(m_hWnd);	
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
	ON_WM_MOUSEWHEEL()
	ON_WM_DRAWITEM()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CBMPZoomView �׸����Դϴ�.

void CBMPZoomView::OnDraw(CDC* pDC)
{
	/*TRACE("DemoZoom::OnDraw\n");
	CDocument* pDoc = GetDocument();
	ASSERT_VALID(pDoc);*/
	
	//srand(0);
	/*int width = 2000/10;
	int height = 1000/10;
	int i = 0;
	for (i = 0; i < 10; i++)
	{
		pDC->FillSolidRect(CRect(i * width, i * height, 
								 i * width + width, i * height + height), RGB(rand() % 255, rand() % 255, rand() % 255));
	}*/
	
}


// CBMPZoomView �����Դϴ�.

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
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	return CZoomView::PreCreateWindow(cs);
}


void CBMPZoomView::OnInitialUpdate()
{
	CZoomView::OnInitialUpdate();
	CRect rt; GetClientRect( &rt );
	//MainFrame�� �ּҸ� �� MainFrame�� ���� �ִ� ListDlg�� PreviewDlg�ּҿ� ZoomView�� �ּ� ����
	CMainFrame* myFrame = (CMainFrame* )GetParentFrame();
	myFrame->m_pFormListDlg->m_cListCtrlRem.m_preViewDlg = this;

	if(m_create_canvas == FALSE){
		m_create_canvas= m_wndCanvas.Create( L"Image Preview Canvas", WS_VISIBLE | WS_CHILD | SS_OWNERDRAW, rt, this, 80 );
	}
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


// CBMPZoomView �޽��� ó�����Դϴ�.
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
	m_bSelectMode = TRUE;
	if (m_bSelectMode) 
	{		
		SetCapture();
		m_ptStart = m_ptEnd =point;
	}
	
	//CZoomView::OnLButtonDown(nFlags, point);
}


void CBMPZoomView::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(m_bSelectMode){
		CClientDC dc(this);
		CPen pen;
		CBrush brush;
		setColorStyle(dc, pen, brush);
		
		dc.SetROP2(R2_COPYPEN);
		dc.Rectangle(m_ptStart.x, m_ptStart.y, m_ptEnd.x, m_ptEnd.y);
		m_bSelectMode = FALSE;
		::ReleaseCapture();


	}
	/*m_bSelectMode = FALSE;
	Invalidate(FALSE);	
	SetZoomArea(m_rubberBand);*/
	CZoomView::OnLButtonUp(nFlags, point);
}
void CBMPZoomView::setColorStyle(CClientDC &dc, CPen &pen, CBrush &brush){
	pen.CreatePen(PS_SOLID, 1, RGB(0,0,0));
 	dc.SelectObject(&pen);
	dc.SelectStockObject(NULL_BRUSH);	
}

void CBMPZoomView::OnMouseMove(UINT nFlags, CPoint point)
{
	if(m_bSelectMode){
		CClientDC dc(this);	
		dc.SelectStockObject(NULL_BRUSH);
		CPen m_penDot(PS_DOT, 1, RGB(0, 0, 0));
		dc.SelectObject(&m_penDot);
		dc.SetROP2(R2_XORPEN);
		// ������ �׸� ������ �����.
		dc.Rectangle(m_ptStart.x,m_ptStart.y,m_ptEnd.x,m_ptEnd.y);
		// ���ο� ������ �׸���.
		dc.Rectangle(m_ptStart.x,m_ptStart.y,point.x,point.y);
		// ���� ������ ��ǥ�� �����Ѵ�.
		m_ptEnd = point; 
	}
	/*if (m_bSelectMode != FALSE) 
	{
		DPtoLP(&point);
		m_rubberBand.SetRect(m_ptStart, point);
		m_rubberBand.NormalizeRect();
		Invalidate(FALSE);
	}*/

	if(m_pSelectedImage!=NULL){		
		CPoint pt = point + GetScrollPosition();
		int m_nZoom= 1;//�̹��� Ȯ�� ����
		pt.x /= m_nZoom;
		pt.y /= m_nZoom;
		ShowImageInfo(pt);
		Invalidate(FALSE);
	}	
	CZoomView::OnMouseMove(nFlags, point);
}


void CBMPZoomView::ShowImageInfo(CPoint a_point)
{
	CMainFrame* pFrame = (CMainFrame *)AfxGetMainWnd();
	
	int w = m_pSelectedImage->GetWidth();
	int h = m_pSelectedImage->GetHeight();

	CString strText;
	//a_point.x < w+m_ImageDest.x && a_point.y < h+m_ImageDest.y &&
	// ���¹ٿ� ���콺 ��ǥ ǥ��
	if (a_point.x >= 0 && a_point.y >= 0 &&		
		a_point.x > m_ImageDest.x &&
		a_point.y > m_ImageDest.y &&
		a_point.x < m_ExpandDest.x+m_ImageDest.x &&
		a_point.y < m_ExpandDest.y+m_ImageDest.y)
	{
		int w = float(a_point.x-m_ImageDest.x)/m_fResolution_W; 
		int h = float(a_point.y-m_ImageDest.y)/m_fResolution_H;
		strText.Format(_T("(X:%d, y:%d)"), w, h);
		pFrame->m_wndStatusBar.SetPaneText(0, strText);
	}

	// ���¹ٿ� �̹��� ���� ǥ��	
	strText.Format(_T("w:%d  h:%d  c:16M"), w, h);	
	pFrame->m_wndStatusBar.SetPaneText(1, strText);
}


void CBMPZoomView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CZoomView::OnVScroll(nSBCode, nPos, pScrollBar);
}


void CBMPZoomView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CZoomView::OnHScroll(nSBCode, nPos, pScrollBar);
}

BOOL CBMPZoomView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
   return DoMouseWheel(nFlags, zDelta, pt);
	//return CZoomView::OnMouseWheel(nFlags, zDelta, pt);
}


void CBMPZoomView::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if( lpDrawItemStruct->itemAction & ODA_DRAWENTIRE )
	{
		m_Canvas_Rect = lpDrawItemStruct->rcItem;
		CMMemDC *pMemDC = NULL;
		pMemDC = new CMMemDC( lpDrawItemStruct->hDC );
		lpDrawItemStruct->hDC = pMemDC->m_hDC;
		//��� ���
		CRect rect; GetClientRect(rect);
		HBRUSH hBrush = ::CreateSolidBrush( RGB(255, 255, 255) );		
		::FillRect( lpDrawItemStruct->hDC, rect, hBrush );		
		DeleteObject( hBrush );
		
		if(!m_pSelectedImage)return;		
		
		if(m_pSelectedImage != NULL)
		{
			Graphics graphics(lpDrawItemStruct->hDC );
			p_graphics = &graphics;
			graphics.SetInterpolationMode(InterpolationModeHighQualityBicubic);
			//Rect ���� ��ȭ�ϸ� ��� ����� ���� �� �� �ִ�.?
			graphics.DrawImage(m_pSelectedImage,	
				                 Rect( lpDrawItemStruct->rcItem.left, 
									   lpDrawItemStruct->rcItem.top,
									   lpDrawItemStruct->rcItem.right - lpDrawItemStruct->rcItem.left,
									   lpDrawItemStruct->rcItem.bottom - lpDrawItemStruct->rcItem.top)); 
			CPen pen;
			CBrush brush;
			pen.CreatePen(PS_DOT, 1, RGB(0,0,0));
			pMemDC->SelectStockObject(NULL_BRUSH);
			pMemDC->SelectObject(&pen);
			//��ũ�ѹ� �̵��� ��ǥ����� ���� ����
			::Rectangle(lpDrawItemStruct->hDC, 
				m_ptStart.x-m_ImageDest.x,
				m_ptStart.y-m_ImageDest.y,
				m_ptEnd.x - m_ImageDest.x,
				m_ptEnd.y-m_ImageDest.y);

			delete pMemDC;
		}		
	}
	//CZoomView::OnDrawItem(nIDCtl, lpDrawItemStruct); ���ƾ� ���� ���� �ʽ��ϴ�.
}


void CBMPZoomView::OnSize(UINT nType, int cx, int cy)
{
	CZoomView::OnSize(nType, cx, cy);
	CRect rc; GetClientRect( rc );

	

	//CImageToolDoc* pDoc = (CImageToolDoc*)pFrame->GetActiveDocument();

	if(m_pSelectedImage==NULL)
	{
		//rc.SetRectEmpty();
		//m_wndCanvas.MoveWindow( rc );
	}
	else
	{
		const int   nWidth    = rc.Width();
		const int   nHeight   = rc.Height();
		const float fRatio    = (float)nHeight/nWidth;
		const float fImgRatio = (float)m_pSelectedImage->GetHeight()/m_pSelectedImage->GetWidth();
				
		if( fImgRatio > fRatio )
		{
			m_ExpandDest.x  = nHeight/fImgRatio;
			m_ExpandDest.y = nHeight;
			m_ImageDest.x       = (nWidth-m_ExpandDest.x)/2;//�̹��� �߾����� ���
			m_ImageDest.y       = 0;
			
		}
		else
		{			
			m_ExpandDest.x  = nWidth;
			m_ExpandDest.y = nWidth*fImgRatio;
			m_ImageDest.x       = 0;
			m_ImageDest.y       = (nHeight-m_ExpandDest.y)/2;
		}

		m_fResolution_W = float(m_ExpandDest.x)/float(m_pSelectedImage->GetWidth());
		m_fResolution_H = float(m_ExpandDest.y)/float(m_pSelectedImage->GetHeight());

		CRect rect(m_ImageDest.x, m_ImageDest.y, m_ImageDest.x+m_ExpandDest.x, m_ImageDest.y+m_ExpandDest.y);
		//Invalidate();
		m_wndCanvas.MoveWindow( rect );
		m_wndCanvas.Invalidate();
	}
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}
