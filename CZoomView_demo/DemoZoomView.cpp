// DemoZoomView.cpp : implementation of the CDemoZoomView class
//

#include "stdafx.h"
#include "DemoZoom.h"

#include "DemoZoomDoc.h"
#include "DemoZoomView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDemoZoomView

IMPLEMENT_DYNCREATE(CDemoZoomView, CZoomView)

BEGIN_MESSAGE_MAP(CDemoZoomView, CZoomView)
	//{{AFX_MSG_MAP(CDemoZoomView)
	ON_COMMAND(ID_VIEW_ZOOMIN, OnViewZoomin)
	ON_COMMAND(ID_VIEW_ZOOMOUT, OnViewZoomout)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CZoomView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CZoomView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CZoomView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDemoZoomView construction/destruction

CDemoZoomView::CDemoZoomView()
{
	m_bSelectMode = FALSE;
}

CDemoZoomView::~CDemoZoomView()
{
}

BOOL CDemoZoomView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CZoomView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CDemoZoomView drawing

void CDemoZoomView::OnDraw(CDC* pDC)
{
	TRACE("DemoZoom::OnDraw\n");
	CDemoZoomDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here

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

void CDemoZoomView::OnInitialUpdate()
{
	CZoomView::OnInitialUpdate();
}

/////////////////////////////////////////////////////////////////////////////
// CDemoZoomView printing

BOOL CDemoZoomView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CDemoZoomView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CDemoZoomView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CDemoZoomView diagnostics

#ifdef _DEBUG
void CDemoZoomView::AssertValid() const
{
	CZoomView::AssertValid();
}

void CDemoZoomView::Dump(CDumpContext& dc) const
{
	CZoomView::Dump(dc);
}

CDemoZoomDoc* CDemoZoomView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDemoZoomDoc)));
	return (CDemoZoomDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDemoZoomView message handlers

void CDemoZoomView::OnViewZoomin() 
{
	SetZoomScale(m_zoomFactor + 1.0f);
}

void CDemoZoomView::OnViewZoomout() 
{
	SetZoomScale(m_zoomFactor - 1.0f);
}

void CDemoZoomView::OnLButtonDown(UINT nFlags, CPoint point) 
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

void CDemoZoomView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_bSelectMode = FALSE;
	Invalidate(FALSE);

	SetZoomArea(m_rubberBand);
	
	CZoomView::OnLButtonUp(nFlags, point);
}

void CDemoZoomView::OnMouseMove(UINT nFlags, CPoint point) 
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

