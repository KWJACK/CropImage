// BMPZoomView.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "EPRE.h"
#include "BMPZoomView.h"
#include "memdc.h"	//include �ؾ� memdc ���� ���
#include "MainFrm.h"
#include <cmath>	//abs float
#include <algorithm>//swap
// CBMPZoomView
#include "SetFileNameDlg.h"
#include "SetImagePathDlg.h"

IMPLEMENT_DYNCREATE(CBMPZoomView, CView)
using namespace Gdiplus; 
CBMPZoomView::CBMPZoomView() : m_create_canvas(FALSE)
{
	m_bSelectMode = FALSE;
	m_pSelectedImage = NULL;
	m_fResolution_H=0.0f;m_fResolution_W=0.0f;
	HDC hdc = ::GetDC(m_hWnd);
	m_IDOK = FALSE;
	m_sPath = L"C:\\Users\\jaekeun\\Desktop\\job\\sampleImage(300)\\temp\\";//default
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
	ON_COMMAND(ID_32774, &CBMPZoomView::OnSaveCropImageFile)
	ON_COMMAND(ID_32777, &CBMPZoomView::OnSetImagePath)
END_MESSAGE_MAP()


// CBMPZoomView �׸����Դϴ�.

void CBMPZoomView::OnDraw(CDC* pDC)
{
	
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
	
	if (!m_bSelectMode && m_pSelectedImage!=NULL //�̹��� Ŭ�� Ȯ��
		&& point.x >= m_ImageDest.x && point.y >= m_ImageDest.y)//����Ȯ��
	{		
		m_bSelectMode = TRUE;
		SetCapture();
		m_ptStart = m_ptEnd =point;
	}
	
	//CZoomView::OnLButtonDown(nFlags, point);
}


void CBMPZoomView::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(m_bSelectMode && point.x <= m_ExpandDest.x+m_ImageDest.x // ���� ���� x,y Ȯ��
		&& point.y <= m_ExpandDest.y+m_ImageDest.y){		
		m_bSelectMode = FALSE;
		CClientDC dc(this);		
		::ReleaseCapture();

		float pStartW = float(m_ptStart.x-m_ImageDest.x)/m_fResolution_W;
		float pStartH = float(m_ptStart.y-m_ImageDest.y)/m_fResolution_H;
		float pEndW = float(point.x-m_ImageDest.x)/m_fResolution_W; 
		float pEndH = float(point.y-m_ImageDest.y)/m_fResolution_H;
		float absWidth = abs(pEndW-pStartW);
		float absHeight = abs(pEndH-pStartH);

		if( absWidth<3 || absHeight<5)return;//�簢���� �ʹ� ������ �������� �ʴ´�

		if(pStartW >= pEndW && pStartH >= pEndH){//��
			if(point.x < m_ptStart.x ||point.y <m_ptStart.y)return;
			
			std::swap(pStartW,pEndW);
			std::swap(pStartH,pEndH);
		}
		else if(pStartW <= pEndW && pStartH >= pEndH){//��
			//if(point.x < m_ptStart.x ||point.y <m_ptStart.y)return;
			std::swap(pStartH,pEndH);
		}
		else if(pStartW >= pEndW && pStartH <= pEndH){//��
			if(point.x < m_ptStart.x ||point.y <m_ptStart.y)return;
			std::swap(pStartW,pEndW);
		}
		else {//��
		}
		

		Gdiplus::RectF bmpRect(0.0f, 0.0f,absWidth,absHeight);		
		Bitmap *pCloneBmp = m_pSelectedImage->Clone(pStartW, pStartH, absWidth,absHeight, m_pSelectedImage->GetPixelFormat());
		Graphics *pGraphics = Graphics::FromImage(pCloneBmp);
		pGraphics->DrawImage(pCloneBmp, bmpRect);	

		OnSaveCropImageFile();		
		if(m_IDOK){//��ȭ���ڿ��� OK�� �������� üũ�ϴ� �ɼ� ��
			UINT num,size;
			ImageCodecInfo * pImageCodecInfo;
			GetImageEncodersSize(&num,&size);
			pImageCodecInfo =(ImageCodecInfo*)( malloc (size));
			GetImageEncoders(num,size,pImageCodecInfo);
			pCloneBmp->Save(m_sPath+m_fileName+L".bmp",&pImageCodecInfo[0].Clsid);
			free(pImageCodecInfo);
		}
		delete pCloneBmp;
		delete pGraphics;		
	}
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
			Gdiplus::Graphics graphics(lpDrawItemStruct->hDC );
			p_graphics = &graphics;
			graphics.SetInterpolationMode(InterpolationModeHighQualityBicubic);
			
			//Rect ���� ��ȭ�ϸ� ��� ����� ���� �� �� �ִ�.?
			graphics.DrawImage(m_pSelectedImage,	
				                 Gdiplus::Rect( lpDrawItemStruct->rcItem.left, 
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
}


void CBMPZoomView::OnSaveCropImageFile()
{
	CSetFileNameDlg dlg;
	dlg.m_fileName = m_oldFileName;//Dlg�� ���� �ֱ� �����̸� ���
	if (dlg.DoModal() == IDOK)
	{
		if(dlg.m_fileName.GetLength()<8)return;//adxxxx_ �� �߰��� �Է��Ѱ� �־�� ����

		m_fileName = m_oldFileName =dlg.m_fileName;//������ �ۼ��� �����̸��� ���߿��� ����մϴ�.
		CString Address = wcstok((TCHAR*)(LPCTSTR)m_fileName, L"_");
		CString Word = wcstok(NULL, L"_");		
		TCHAR* Index = wcstok(NULL, L"_");
		if(Index ==NULL)return;//����ڰ� �߸� �Է��� ��� ó��

		int tempIndex = _wtoi(Index);		
		CString newIndex;
		newIndex.Format(L"%d", tempIndex+1);		
		m_fileName  = Address+L"_"+Word+L"_"+Index+L"_"+Word.GetAt(tempIndex);
		m_oldFileName = Address+L"_"+Word+L"_"+ newIndex +L"_"+Word.GetAt(++tempIndex);		
		if(tempIndex  >= Word.GetLength()){
			m_oldFileName = Address+L"_";//�Է¹��ڿ��� �ʰ��ϸ� up)
		}
		
		m_IDOK = TRUE;
		//ó�� ���� �̸� �Է��� ��� �ε��� �� �ڵ����� �����ϰ� �ϸ� �ɵ�
	}
	else m_IDOK = FALSE;
}

//�ڸ� ���� ���� ��ġ ����
void CBMPZoomView::OnSetImagePath()
{
	SetImagePathDlg dlg;	
	if (dlg.DoModal() == IDOK)
	{
		m_sPath = dlg.m_path +L"\\";		
	}	
}
