// PreviewDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"//
#include "Test.h"
#include "PreviewDlg.h"//
#include "afxdialogex.h"
#include "memdc.h"//

// CPreviewDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPreviewDlg, CDialog)

CPreviewDlg::CPreviewDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPreviewDlg::IDD, pParent)
{

}

CPreviewDlg::~CPreviewDlg()
{
}

void CPreviewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	
}


BEGIN_MESSAGE_MAP(CPreviewDlg, CDialog)
	ON_WM_CREATE()
	ON_WM_DRAWITEM()
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CPreviewDlg 메시지 처리기입니다.


int CPreviewDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	CRect rect; GetClientRect( &rect );

	BOOL tt= m_wndCanvas.Create( L"Image Preview Canvas", WS_VISIBLE | WS_CHILD | SS_OWNERDRAW, rect, this, 80 );
	return 0;
}


//비트맵 이미지를 새로운 창에서 만든다
void CPreviewDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if( lpDrawItemStruct->itemAction & ODA_DRAWENTIRE )
	{
		CMMemDC *pMemDC = NULL;
		pMemDC = new CMMemDC( lpDrawItemStruct->hDC );
		lpDrawItemStruct->hDC = pMemDC->m_hDC;

		CRect rect; GetClientRect( rect );
		HBRUSH hBrush = ::CreateSolidBrush( RGB(255, 255, 255) );
		
		::FillRect( lpDrawItemStruct->hDC, rect, hBrush );
		
		DeleteObject( hBrush );
		///*CImageToolDoc *pDoc = (CImageToolDoc*)((CMainFrame*)AfxGetMainWnd())->GetActiveDocument();
		//
		//if( pDoc->m_pSelectedImage != NULL )
		//{
		//	Graphics graphics( lpDrawItemStruct->hDC );
		//	graphics.SetInterpolationMode(InterpolationModeHighQualityBicubic);
		//	graphics.DrawImage( pDoc->m_pSelectedImage,	
		//		                 Rect( lpDrawItemStruct->rcItem.left, 
		//							   lpDrawItemStruct->rcItem.top,
		//							   lpDrawItemStruct->rcItem.right - lpDrawItemStruct->rcItem.left,
		//							   lpDrawItemStruct->rcItem.bottom - lpDrawItemStruct->rcItem.top)); 
		//}*/

		delete pMemDC;
	}

	CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

//사이즈 조절시 확대
void CPreviewDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	/*CRect rc; GetClientRect( rc );

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();

	if( pFrame == NULL )
	{
		return;
	}

	CImageToolDoc* pDoc = (CImageToolDoc*)pFrame->GetActiveDocument();

	if( pDoc->m_pSelectedImage == NULL )
	{
		rc.SetRectEmpty();
		m_wndCanvas.MoveWindow( rc );
	}
	else
	{
		const int   nWidth    = rc.Width();
		const int   nHeight   = rc.Height();
		const float fRatio    = (float)nHeight/nWidth;
		const float fImgRatio = (float)pDoc->m_pSelectedImage->GetHeight()/pDoc->m_pSelectedImage->GetWidth();

		int XDest, YDest, nDestWidth, nDestHeight;
	
		if( fImgRatio > fRatio )
		{
			nDestWidth  = nHeight/fImgRatio;
			XDest       = (nWidth-nDestWidth)/2;
			YDest       = 0;
			nDestHeight = nHeight;
		}
		else
		{
			XDest       = 0;
			nDestWidth  = nWidth;
			nDestHeight = nWidth*fImgRatio;
			YDest       = (nHeight-nDestHeight)/2;
		}

		CRect rect(XDest, YDest, XDest+nDestWidth, YDest+nDestHeight);
		m_wndCanvas.MoveWindow( rect );
		m_wndCanvas.Invalidate();
	}*/
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


HBRUSH CPreviewDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.
	switch (nCtlColor) 
	{
	case CTLCOLOR_DLG:
		pDC->SetBkColor( RGB(255,255,255) );
		return (HBRUSH)GetStockObject( WHITE_BRUSH );
	default:
		return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	}
	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}
