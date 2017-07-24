// FormBMP.cpp : 구현 파일입니다.

#include "stdafx.h"
#include "EPRE.h"
#include "FormBMP.h"
#include "MainFrm.h"
#include "memdc.h"

// FormBMP
IMPLEMENT_DYNCREATE(FormBMP, CFormView)

FormBMP::FormBMP()
	: CFormView(FormBMP::IDD)
{
	m_saveFlag = 0;
	m_pSelectedImage = nullptr;
}

FormBMP::~FormBMP()
{
}

void FormBMP::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BMP_THUMNAIL, m_bmp_thumnail);
}

BEGIN_MESSAGE_MAP(FormBMP, CFormView)
	ON_WM_SIZE()	
	ON_WM_PAINT()
	ON_WM_CREATE()
END_MESSAGE_MAP()

// FormBMP 진단입니다.

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

// FormBMP 메시지 처리기입니다.

void FormBMP::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);
	if(rt.Width()){
		this->GetClientRect(rt);
		CStatic* pWnd = (CStatic *)GetDlgItem(IDC_BMP_THUMNAIL);
		pWnd->MoveWindow(rt);
	}	
}


int FormBMP::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;
	GetClientRect(rt);	
	CMainFrame* myFrame = (CMainFrame* )GetParentFrame();
	myFrame->m_pFormBMP = this;
	return 0;
}


void FormBMP::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	if(m_saveFlag){
			CMMemDC *pMemDC = NULL;
			HDC hdc = ::GetWindowDC(m_hWnd);
			pMemDC = new CMMemDC( hdc);
			hdc= pMemDC->m_hDC;
			//배경 흰색
			CRect rect; GetClientRect(rect);
			HBRUSH hBrush = ::CreateSolidBrush( RGB(255, 255, 255) );		
			::FillRect( hdc, rect, hBrush );		
			DeleteObject( hBrush );

			Gdiplus::Graphics graphics(hdc);
			p_graphics = &graphics;
			graphics.SetInterpolationMode(InterpolationModeHighQualityBicubic);
				
			graphics.DrawImage(m_pSelectedImage,	
				Gdiplus::Rect( rt.left, rt.top, rt.right -rt.left, rt.bottom - rt.top)); 
			m_saveFlag = 0;			
			delete pMemDC;
	}
}
