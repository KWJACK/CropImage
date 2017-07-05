// ImageToolView.cpp : implementation of the CImageToolView class
//

#include "stdafx.h"
#include "ImageTool.h"
#include "ImageToolDoc.h"
#include "ImageToolView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CImageToolView
IMPLEMENT_DYNCREATE(CImageToolView, CListView)

BEGIN_MESSAGE_MAP(CImageToolView, CListView)
	//{{AFX_MSG_MAP(CImageToolView)
	//}}AFX_MSG_MAP
	ON_NOTIFY_REFLECT( LVN_ITEMCHANGED,       OnItemchanged                 )
	ON_COMMAND(        ID_FILE_PRINT,         CListView::OnFilePrint        )
	ON_COMMAND(        ID_FILE_PRINT_DIRECT,  CListView::OnFilePrint        )
	ON_COMMAND(        ID_FILE_PRINT_PREVIEW, CListView::OnFilePrintPreview )
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImageToolView construction/destruction
CImageToolView::CImageToolView()
{
	m_bTerminate  = FALSE;
	m_bRunning    = FALSE;
	m_hThreadLoad = NULL;
	m_pFoldersDlg = NULL;
	m_pPreviewDlg = NULL;
}

CImageToolView::~CImageToolView()
{
	if( m_pFoldersDlg )
	{
		delete m_pFoldersDlg;
	}

	if( m_pPreviewDlg )
	{
		delete m_pPreviewDlg;
	}

	::CloseHandle( m_hThreadLoad ); 
}

BOOL CImageToolView::PreCreateWindow( CREATESTRUCT& cs )
{
	return CListView::PreCreateWindow( cs );
}

/////////////////////////////////////////////////////////////////////////////
// CImageToolView drawing
void CImageToolView::OnDraw(CDC* pDC)
{
	CImageToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	BOOL   bPrinting = pDC->IsPrinting();
	Bitmap *imaView  = pDoc->GetImage();

	if( imaView && imaView->GetFlags() != 0 )
	{
		if( bPrinting )
		{
			int cxPage = pDC->GetDeviceCaps(HORZRES);
			int cyPage = pDC->GetDeviceCaps(VERTRES);
		
			int cxInch = pDC->GetDeviceCaps(LOGPIXELSX);
			int cyInch = pDC->GetDeviceCaps(LOGPIXELSY);

			int cx = cxPage-200;
			int cy = (int)(((double)imaView->GetHeight()*cyInch*cx)
						  /((double)imaView->GetWidth()*cxInch));
		
			Graphics graphics(pDC->GetSafeHdc());
			graphics.SetInterpolationMode(InterpolationModeHighQualityBicubic);
			graphics.DrawImage(imaView, Rect(100, 100, cx, cy));
		}
	}
}

void CImageToolView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();

	CListCtrl& ListCtrl = GetListCtrl();

	m_ImageListThumb.Create( THUMBNAIL_WIDTH,
		                     THUMBNAIL_HEIGHT,
							 ILC_COLOR32,
							 0,
							 1 );

	ListCtrl.SetImageList( &m_ImageListThumb, LVSIL_NORMAL );

	m_pPreviewDlg = new CPreviewDlg();
	m_pPreviewDlg->Create( IDD_PREVIEW_DLG );
	m_pPreviewDlg->SetWindowPos( &wndTopMost,
								 20, 320,
								 500, 430,
								 SWP_DRAWFRAME );

	m_pFoldersDlg = new CFoldersDlg();
	m_pFoldersDlg->Create( IDD_BROWSER_DLG );
	m_pFoldersDlg->SetWindowPos( &wndTopMost,
								 20, 10,
								 200, 300,
								 SWP_DRAWFRAME );
}

/////////////////////////////////////////////////////////////////////////////
// CImageToolView printing
BOOL CImageToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	pInfo->SetMaxPage(1);
	pInfo->m_bContinuePrinting = FALSE;

	return DoPreparePrinting( pInfo );
}

void CImageToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* pInfo )
{
}

void CImageToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* pInfo)
{
}

/////////////////////////////////////////////////////////////////////////////
// CImageToolView diagnostics
#ifdef _DEBUG
void CImageToolView::AssertValid() const
{
	CListView::AssertValid();
}

void CImageToolView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CImageToolDoc* CImageToolView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CImageToolDoc)));
	return (CImageToolDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CImageToolView message handlers
BOOL CImageToolView::Create( LPCTSTR        lpszClassName, 
							 LPCTSTR        lpszWindowName, 
							 DWORD          dwStyle, 
							 const RECT&    rect, 
							 CWnd           *pParentWnd,
							 UINT           nID, 
							 CCreateContext *pContext) 
{
	return CListView::Create( lpszClassName, 
		                      _T("ListView"),
							  dwStyle           | 
							  LVS_SHOWSELALWAYS | 
							  LVS_ALIGNTOP      |
							  LVS_ICON          |
							  LVS_SINGLESEL     |
							  LVS_AUTOARRANGE, 
							  rect, 
							  pParentWnd, 
							  nID,
							  pContext );
}

unsigned long _stdcall RunLoadThumbnailThread( LPVOID lpParam )
{
	int nIndex = 0;
	std::vector<CString>::iterator iter;

	CImageToolView *pView = (CImageToolView*)lpParam;
	CImageToolDoc  *pDoc  = pView->GetDocument();

	CListCtrl& ListCtrl  = pView->GetListCtrl();
	CImageList *pImgList = &pView->m_ImageListThumb;

	int iImageCount = pImgList->GetImageCount();

	for( int i=0; i < iImageCount; i++ )
	{
		pImgList->Remove(i);	
	}

	ListCtrl.DeleteAllItems();
	pImgList->SetImageCount( pDoc->m_vFileName.size() );
	
	ListCtrl.SetRedraw( FALSE );

	for( iter =  pDoc->m_vFileName.begin();
	     iter != pDoc->m_vFileName.end() && pView->m_bTerminate != TRUE; 
		 iter++, nIndex++ )
	{
		  HBITMAP hbmReturn = NULL; 
		  Bitmap  *bmPhoto  = NULL;
  		  CBitmap Bmp1;
	  
		  ListCtrl.InsertItem( nIndex, *iter, nIndex );
		  
		  CString path; path.Empty();
		  path.Format( "%s\\%s", pDoc->m_strCurrentDirectory, *iter);
		  
		  Bitmap image( path.AllocSysString() );
		  
		  int sourceWidth  = image.GetWidth();
		  int sourceHeight = image.GetHeight();

		  int destX = 0,
			  destY = 0; 
		  
		  float nPercent  = 0;
		  float nPercentW = ((float)THUMBNAIL_WIDTH/(float)sourceWidth);;
		  float nPercentH = ((float)THUMBNAIL_HEIGHT/(float)sourceHeight);
			  
		  if(nPercentH < nPercentW)
		  {
			  nPercent = nPercentH;
			  destX    = (int)((THUMBNAIL_WIDTH - (sourceWidth * nPercent))/2);
		  }
		  else
		  {
			  nPercent = nPercentW;
			  destY    = (int)((THUMBNAIL_HEIGHT - (sourceHeight * nPercent))/2);
		  }
			  
		  int destWidth  = (int)(sourceWidth * nPercent);
		  int destHeight = (int)(sourceHeight * nPercent);
			  
		  bmPhoto = new Bitmap( THUMBNAIL_WIDTH, THUMBNAIL_HEIGHT, PixelFormat24bppRGB );
		  bmPhoto->SetResolution( image.GetHorizontalResolution(), image.GetVerticalResolution() );
			  
		  Graphics *grPhoto = Graphics::FromImage( bmPhoto );
		  Color colorW(255, 255, 255, 255);
		  grPhoto->Clear( colorW );
		  grPhoto->SetInterpolationMode( InterpolationModeHighQualityBicubic );
		  grPhoto->DrawImage( &image, Rect(destX, destY, destWidth, destHeight) );
		  
		  bmPhoto->GetHBITMAP( colorW, &hbmReturn );
		  
		  Bmp1.Attach( hbmReturn );
		  pImgList->Replace( nIndex, &Bmp1, NULL );
		  
		  delete grPhoto;
		  delete bmPhoto;
		  Bmp1.Detach();
		  DeleteObject( hbmReturn );
	}

	ListCtrl.SetRedraw(TRUE);
	ListCtrl.Invalidate();
		  
	pView->m_bRunning   = FALSE;
	pView->m_bTerminate = FALSE;
		  
	pView->m_hThreadLoad = NULL;
	::CloseHandle( pView->m_hThreadLoad ); 
		  
	return 0;
}

void CImageToolView::OnItemchanged( NMHDR* pNMHDR, LRESULT* pResult ) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	CImageToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->SelectItem( pNMListView->iItem );
	*pResult = 0;
}

BOOL CImageToolView::TerminateLoadThread()
{
	if( m_bRunning == FALSE )
	{
		return TRUE;
	}

	m_bTerminate = TRUE;
	
	while( m_bRunning )
	{
		MSG message;
		while (::PeekMessage( &message, NULL, 0, 0, PM_REMOVE ) ) 
		{
			::TranslateMessage( &message );
			::DispatchMessage(  &message );
		}
	}
	
	::CloseHandle( m_hThreadLoad );

	return TRUE;
}

void CImageToolView::LoadThumbnailImages()
{
	m_hThreadLoad = CreateThread( NULL,
		                          0,
								  RunLoadThumbnailThread,
								  this,
								  0,
								  &m_dwThreadID );

	m_bRunning = TRUE;
}
