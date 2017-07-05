// PreviewDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"//
#include "Test.h"
#include "PreviewDlg.h"//
#include "afxdialogex.h"
#include "memdc.h"//

// CPreviewDlg ��ȭ �����Դϴ�.

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


// CPreviewDlg �޽��� ó�����Դϴ�.


int CPreviewDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  ���⿡ Ư��ȭ�� �ۼ� �ڵ带 �߰��մϴ�.
	CRect rect; GetClientRect( &rect );

	BOOL tt= m_wndCanvas.Create( L"Image Preview Canvas", WS_VISIBLE | WS_CHILD | SS_OWNERDRAW, rect, this, 80 );
	return 0;
}


//��Ʈ�� �̹����� ���ο� â���� �����
void CPreviewDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
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

//������ ������ Ȯ��
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
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}


HBRUSH CPreviewDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ���⼭ DC�� Ư���� �����մϴ�.
	switch (nCtlColor) 
	{
	case CTLCOLOR_DLG:
		pDC->SetBkColor( RGB(255,255,255) );
		return (HBRUSH)GetStockObject( WHITE_BRUSH );
	default:
		return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	}
	// TODO:  �⺻���� �������� ������ �ٸ� �귯�ø� ��ȯ�մϴ�.
	return hbr;
}
