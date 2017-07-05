// ImageToolDoc.cpp : implementation of the CImageToolDoc class
//

#include "stdafx.h"
#include "ImageTool.h"
#include "ImageToolDoc.h"
#include "GammaDlg.h"
#include "MainFrm.h"
#include "ImageToolView.h"
#include "ThresholdDlg.h"
#include "FoldersDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

ColorMatrix	clrMatrix; 

/////////////////////////////////////////////////////////////////////////////
// CImageToolDoc
IMPLEMENT_DYNCREATE(CImageToolDoc, CDocument)

BEGIN_MESSAGE_MAP(CImageToolDoc, CDocument)
	//{{AFX_MSG_MAP(CImageToolDoc)
	ON_COMMAND(           ID_EDIT_UNDO,		   OnEditUndo              )
	ON_UPDATE_COMMAND_UI( ID_EDIT_UNDO,		   OnUpdateEditUndo        )
	ON_COMMAND(           ID_EDIT_REDO,		   OnEditRedo              )
	ON_UPDATE_COMMAND_UI( ID_EDIT_REDO,		   OnUpdateEditRedo        )
	ON_COMMAND(           ID_IMAGE_FLIP,	   OnImageFlip             )
	ON_UPDATE_COMMAND_UI( ID_IMAGE_FLIP,	   OnUpdateImageFlip       )
	ON_COMMAND(           ID_IMAGE_ROTATE,	   OnImageRotate           )
	ON_UPDATE_COMMAND_UI( ID_IMAGE_ROTATE,	   OnUpdateImageRotate     )
	ON_COMMAND(           ID_IMAGE_MIRROR,	   OnImageMirror           )
	ON_UPDATE_COMMAND_UI( ID_IMAGE_MIRROR,	   OnUpdateImageMirror     )
	ON_COMMAND(           ID_IMAGE_GAMMA,	   OnImageGamma            )
	ON_UPDATE_COMMAND_UI( ID_IMAGE_GAMMA,	   OnUpdateImageGamma      )
	ON_COMMAND(           ID_IMAGE_THRESHOLD,  OnImageThreshold        )
	ON_UPDATE_COMMAND_UI( ID_IMAGE_THRESHOLD,  OnUpdateImageThreshold  )
	ON_COMMAND(           ID_IMAGE_LIGHTEN,	   OnImageLighten          )
	ON_UPDATE_COMMAND_UI( ID_IMAGE_LIGHTEN,	   OnUpdateImageLighten    )
	ON_COMMAND(           ID_IMAGE_DARKEN,	   OnImageDarken           ) 
	ON_UPDATE_COMMAND_UI( ID_IMAGE_DARKEN,	   OnUpdateImageDarken     )
	ON_COMMAND(           ID_IMAGE_GRAYSCALE,  OnImageGrayscale        )
	ON_UPDATE_COMMAND_UI( ID_IMAGE_GRAYSCALE,  OnUpdateImageGrayscale  )
	ON_COMMAND(           ID_IMAGE_NEGATIVE,   OnImageNegative         )
	ON_UPDATE_COMMAND_UI( ID_IMAGE_NEGATIVE,   OnUpdateImageNegative   )
	ON_COMMAND(           ID_IMAGE_SHARPEN,	   OnImageSharpen          )
	ON_UPDATE_COMMAND_UI( ID_IMAGE_SHARPEN,	   OnUpdateImageSharpen    )
	ON_COMMAND(           ID_IMAGE_CONTRAST,   OnImageContrast         )
	ON_UPDATE_COMMAND_UI( ID_IMAGE_CONTRAST,   OnUpdateImageContrast   )
	ON_COMMAND(           ID_IMAGE_LROTATTE90, OnImageLrotatte90       )
	ON_UPDATE_COMMAND_UI( ID_IMAGE_LROTATTE90, OnUpdateImageLrotatte90 )
	ON_UPDATE_COMMAND_UI( ID_APP_EXIT,         OnUpdateAppExit         )
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImageToolDoc construction/destruction
CImageToolDoc::CImageToolDoc()
{
	m_pSelectedImage      = NULL;
	m_strCurrentDirectory = _T("");
	m_nSelectedItem       = -1;
	m_UndoLevel		      = 0;
	m_hThreadTools		  = NULL;

	for( int i=0; i < MAX_UNDO_LEVELS; i++ )
	{
		imageUndo[i] = NULL;
	}
}

CImageToolDoc::~CImageToolDoc()
{
	if( m_pSelectedImage != NULL )
	{
		delete m_pSelectedImage;
	}

	for( int i=0; i < MAX_UNDO_LEVELS; i++ )
	{
		delete imageUndo[i];
	}

	::CloseHandle( m_hThreadTools ); 
}

BOOL CImageToolDoc::OnNewDocument()
{
	if( !CDocument::OnNewDocument() )
	{
		return FALSE;
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CImageToolDoc serialization
void CImageToolDoc::Serialize(CArchive& ar)
{
	if( ar.IsStoring() ) {}
	else {}
}

/////////////////////////////////////////////////////////////////////////////
// CImageToolDoc diagnostics
#ifdef _DEBUG
void CImageToolDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CImageToolDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CImageToolDoc commands
BOOL CImageToolDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if( !CDocument::OnOpenDocument( lpszPathName ) )
	{
		return FALSE;
	}
	
	return TRUE;
}

void CImageToolDoc::SelectFolder( LPCSTR pstr )
{
	CImageToolView* pView = (CImageToolView*)((CMainFrame*)AfxGetMainWnd())->GetActiveView();

	if( pView->IsRunning() )
	{
		pView->TerminateLoadThread();
	}

	m_strCurrentDirectory = pstr;
	m_vFileName.clear();

	if( m_pSelectedImage != NULL )
	{
		delete m_pSelectedImage;
		m_pSelectedImage = NULL;
	}

	pView->m_pPreviewDlg->SendMessage( WM_SIZE );

	CFileFind finder;
	CString strWildCard( pstr );

	strWildCard += "\\*.*";

	BOOL bWorking = finder.FindFile( strWildCard );

	while( bWorking )
	{
		bWorking = finder.FindNextFile();

		if( finder.IsDots() || finder.IsDirectory() )
		{
			continue;
		}
		else
		{
			CString filePath = finder.GetFileName();

			if( IsImageGDIPLUSValid( m_strCurrentDirectory + _T("\\") + filePath ) )
			{
				filePath.MakeLower();
				m_vFileName.push_back( filePath );
			}
		}
	}

	finder.Close();
	pView->LoadThumbnailImages();
	m_nSelectedItem = -1;
}

BOOL CImageToolDoc::IsImageGDIPLUSValid( CString filePath )
{
	Bitmap image( filePath.AllocSysString() );

	if( image.GetFlags() == ImageFlagsNone ) return FALSE;
	else return TRUE;
}

void CImageToolDoc::SelectItem( int nIndex )
{
	if( m_nSelectedItem == nIndex )
	{
		return;
	}

	m_nSelectedItem = nIndex;

	if( m_pSelectedImage != NULL )
	{
		delete m_pSelectedImage;
		m_pSelectedImage = NULL;
	}
	
	if( nIndex < m_vFileName.size() )
	{
		CString filePath; filePath.Empty();
		filePath = m_strCurrentDirectory + "\\" + m_vFileName[ nIndex ];

		m_pSelectedImage  = Bitmap::FromFile( filePath.AllocSysString() );
		CImageToolView* pView = (CImageToolView*)((CMainFrame*)AfxGetMainWnd())->GetActiveView();
		pView->m_pPreviewDlg->SendMessage( WM_SIZE );
	}
}

unsigned long _stdcall RunImageToolsThread( LPVOID lpParam )
{
	CImageToolDoc *pDoc = (CImageToolDoc *)lpParam;
	
	if( pDoc == NULL || pDoc->m_pSelectedImage == NULL ) 
	{
		return 0;
	}
	
	pDoc->SubmitUndo();

	Graphics* imageGraphics = Graphics::FromImage( pDoc->GetImage() );
	ImageAttributes	imAtt; 

	int W = (int)( pDoc->m_pSelectedImage->GetWidth()  );
	int H = (int)( pDoc->m_pSelectedImage->GetHeight() );
	
	switch( pDoc->m_MenuCommand )
	{
	case ID_IMAGE_FLIP:
		pDoc->m_pSelectedImage->RotateFlip( RotateNoneFlipY );
		break;
	case ID_IMAGE_MIRROR:
		pDoc->m_pSelectedImage->RotateFlip( RotateNoneFlipX );
		break;
	case ID_IMAGE_ROTATE:							
		pDoc->m_pSelectedImage->RotateFlip( Rotate90FlipNone );
		break;
	case ID_IMAGE_LROTATTE90:						
		pDoc->m_pSelectedImage->RotateFlip( Rotate270FlipNone ); 
		break;
	case ID_IMAGE_NEGATIVE:
		{
			ColorMatrix matrix = {
					-1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
					0.0f,  -1.0f,  0.0f, 0.0f, 0.0f,
					0.0f,   0.0f, -1.0f, 0.0f, 0.0f,
					0.0f,   0.0f,  0.0f, 1.0f, 0.0f,
					1.0f,   1.0f,  1.0f, 0.0f, 1.0f };
					
			clrMatrix = matrix;
			imAtt.SetColorMatrix( &clrMatrix, ColorMatrixFlagsDefault, ColorAdjustTypeBitmap );
			imageGraphics->DrawImage( pDoc->m_pSelectedImage,
									  Rect(0,0,W,H),
									  0,0,W,H,
									  UnitPixel,
									  &imAtt );
		}
		break;
	case ID_IMAGE_GRAYSCALE:
		{
			ColorMatrix matrix = {
					0.299f, 0.299f,	0.299f,	0.0f, 0.0f,
					0.587f,	0.587f, 0.587f,	0.0f, 0.0f,
					0.114f,	0.114f,	0.114f, 0.0f, 0.0f,
					0.0f,	0.0f,	0.0f,	1.0f, 0.0f,
					0.0f,	0.0f,	0.0f,	0.0f, 1.0f };

			clrMatrix = matrix;
			imAtt.SetColorMatrix( &clrMatrix, ColorMatrixFlagsDefault, ColorAdjustTypeBitmap );
			imageGraphics->DrawImage( pDoc->m_pSelectedImage,
				                      Rect(0,0,W,H),	
									  0,0,W,H,					
									  UnitPixel,
									  &imAtt );
		}
		break;
	case ID_IMAGE_THRESHOLD:
		{
			float factor = 0.5f;
			imAtt.SetThreshold( factor, ColorAdjustTypeBitmap );
			imageGraphics->DrawImage( pDoc->m_pSelectedImage,
				                      Rect(0,0,W,H),	
									  0,0,W,H,	
									  UnitPixel,
									  &imAtt );
		}
		break;
	case ID_IMAGE_LIGHTEN:
		{
			float factor = 0.2f;
			ColorMatrix matrix =  {
					1.0f,   0.0f,   0.0f,   0.0f, 0.0f,
					0.0f,   1.0f,   0.0f,   0.0f, 0.0f,
					0.0f,   0.0f,   1.0f,   0.0f, 0.0f,
					0.0f,   0.0f,   0.0f,   1.0f, 0.0f,
					factor, factor, factor, 0.0f, 1.0f };

			clrMatrix = matrix;
			imAtt.SetColorMatrix( &clrMatrix, ColorMatrixFlagsDefault, ColorAdjustTypeBitmap );
			imageGraphics->DrawImage( pDoc->m_pSelectedImage,
				                      Rect(0,0,W,H),	
									  0,0,W,H,
									  UnitPixel,
									  &imAtt );
		}
		break;
	case ID_IMAGE_DARKEN:
		{
			float factor = -0.2f;

			ColorMatrix matrix = {
					1.0f,   0.0f,   0.0f,   0.0f, 0.0f,
					0.0f,   1.0f,   0.0f,   0.0f, 0.0f,
					0.0f,   0.0f,   1.0f,   0.0f, 0.0f,
					0.0f,   0.0f,   0.0f,   1.0f, 0.0f,
					factor, factor, factor, 0.0f, 1.0f };

			clrMatrix = matrix;
			imAtt.SetColorMatrix( &clrMatrix, ColorMatrixFlagsDefault, ColorAdjustTypeBitmap );
			imageGraphics->DrawImage( pDoc->m_pSelectedImage,
				                      Rect(0,0,W,H),
									  0,0,W,H,
									  UnitPixel,
									  &imAtt );
		}
		break;
	case ID_IMAGE_CONTRAST:
		{
			float factor  = 1.6f;
			float factorT = 0.5f*(1.0f - factor); 

			ColorMatrix matrix = {
					factor,  0.0f,    0.0f,    0.0f, 0.0f,
					0.0f,    factor,  0.0f,    0.0f, 0.0f,
					0.0f,    0.0f,	  factor,  0.0f, 0.0f,
					0.0f,    0.0f,	  0.0f,    1.0f, 0.0f,
					factorT, factorT, factorT, 0.0f, 1.0f };

			clrMatrix = matrix;
			imAtt.SetColorMatrix( &clrMatrix, ColorMatrixFlagsDefault, ColorAdjustTypeBitmap );
			imageGraphics->DrawImage( pDoc->m_pSelectedImage,
				                      Rect(0,0,W,H),
									  0,0,W,H,
									  UnitPixel,
									  &imAtt );
		}
		break;
	case ID_IMAGE_SHARPEN:
		{
			float factor  = 1.2f;
			float factorT = 0.5f*( 1.0f - factor ); 

			ColorMatrix matrix = {
					factor,  0.0f,	  0.0f,    0.0f, 0.0f,
					0.0f,    factor,  0.0f,    0.0f, 0.0f,
					0.0f,    0.0f,	  factor,  0.0f, 0.0f,
					0.0f,    0.0f,	  0.0f,    1.0f, 0.0f,
					factorT, factorT, factorT, 0.0f, 1.0f };

			clrMatrix = matrix;
			imAtt.SetColorMatrix( &clrMatrix, ColorMatrixFlagsDefault, ColorAdjustTypeBitmap );
			imageGraphics->DrawImage( pDoc->m_pSelectedImage,
				                      Rect(0,0,W,H),
									  0,0,W,H,
									  UnitPixel,
									  &imAtt );
		}
		break;
	case ID_IMAGE_GAMMA:
		{
			imAtt.SetGamma( (float)(long)pDoc->m_fp[0]/1000, ColorAdjustTypeBitmap );
			imageGraphics->DrawImage( pDoc->m_pSelectedImage,
				                      Rect(0,0,W,H),
									  0,0,W,H,
									  UnitPixel,
									  &imAtt );
		}
		break;
	default:
		break;
	}

	pDoc->m_hThreadTools = NULL;
	::CloseHandle( pDoc->m_hThreadTools ); 

	CImageToolView* pView = (CImageToolView*)((CMainFrame*)AfxGetMainWnd())->GetActiveView();
	pView->m_pPreviewDlg->SendMessage( WM_SIZE );

	delete imageGraphics;

	return 0;
}

void CImageToolDoc::SubmitUndo()
{
	if( m_UndoLevel >= MAX_UNDO_LEVELS )			
	{
		delete imageUndo[0];
		
		for( int i = 1; i < MAX_UNDO_LEVELS; i++ )
		{
			imageUndo[ i - 1 ] = imageUndo[ i ];
		}
		
		imageUndo[ MAX_UNDO_LEVELS - 1 ] = 0;			
		
		m_UndoLevel = MAX_UNDO_LEVELS - 1;				
	}
	
	for( int i = m_UndoLevel; i < MAX_UNDO_LEVELS; i++ )
	{
		if( imageUndo[ i ] )
		{
			delete imageUndo[ i ];
			imageUndo[ i ] = 0;
		}
	}
	
	imageUndo[ m_UndoLevel ] = m_pSelectedImage->Clone( Rect( 0,
		                                                      0,
		                                                      m_pSelectedImage->GetWidth(),
															  m_pSelectedImage->GetHeight() ),
		                                                PixelFormat32bppARGB );
	m_UndoLevel++;
}

void CImageToolDoc::OnEditUndo() 
{
	CImageToolView* pView    = (CImageToolView*)((CMainFrame*)AfxGetMainWnd())->GetActiveView();
	m_UndoLevel--;

	Bitmap *tmpImg           = m_pSelectedImage;
	m_pSelectedImage         = imageUndo[ m_UndoLevel ];
	imageUndo[ m_UndoLevel ] = tmpImg;
	m_MenuCommand            = ID_NO_MENU;

	pView->m_pPreviewDlg->SendMessage( WM_SIZE );
}

void CImageToolDoc::OnUpdateEditUndo(CCmdUI* pCmdUI) 
{
	if( m_pSelectedImage == 0 || m_hThreadTools )
	{
		pCmdUI->Enable(0);
	}
	else
	{
		pCmdUI->Enable( m_UndoLevel > 0 );
	}

	CString s; s.Format( "Undo (%d)", m_UndoLevel );
	
	pCmdUI->SetText( s );	
}

void CImageToolDoc::OnEditRedo() 
{
	Bitmap *tmpImg           = m_pSelectedImage;
	m_pSelectedImage         = imageUndo[ m_UndoLevel ];
	imageUndo[ m_UndoLevel ] = tmpImg;

	m_UndoLevel++;
	m_MenuCommand            = ID_NO_MENU;

	CImageToolView* pView    = (CImageToolView*)((CMainFrame*)AfxGetMainWnd())->GetActiveView();
	pView->m_pPreviewDlg->SendMessage( WM_SIZE );
}

void CImageToolDoc::OnUpdateEditRedo(CCmdUI* pCmdUI) 
{
	if( m_pSelectedImage == 0 || m_hThreadTools )
	{
		pCmdUI->Enable(0);
	}
	else
	{
		pCmdUI->Enable( (m_UndoLevel<(MAX_UNDO_LEVELS)) && (imageUndo[m_UndoLevel] !=0 ) );	
	}
}

void CImageToolDoc::OnImageFlip() 
{
	m_MenuCommand = ID_IMAGE_FLIP;

	m_hThreadTools = CreateThread( NULL,
		                           0,
								   RunImageToolsThread,
								   this,
								   0,
								   &dwThreadID );
}

void CImageToolDoc::OnUpdateImageFlip(CCmdUI* pCmdUI) 
{
	if( m_pSelectedImage == 0 || m_hThreadTools )
	{
		pCmdUI->Enable(0);
	}
}

void CImageToolDoc::OnImageRotate()      
{
	m_MenuCommand = ID_IMAGE_ROTATE;
		
	m_hThreadTools = CreateThread( NULL,
		                           0,
								   RunImageToolsThread,
								   this,
								   0,
								   &dwThreadID );
}

void CImageToolDoc::OnUpdateImageRotate(CCmdUI* pCmdUI) 
{
	if( m_pSelectedImage == 0 || m_hThreadTools )
	{
		pCmdUI->Enable(0);
	}
}

void CImageToolDoc::OnImageLrotatte90() 
{
	m_MenuCommand = ID_IMAGE_LROTATTE90;
		
	m_hThreadTools = CreateThread( NULL,
			                       0,
								   RunImageToolsThread,
								   this,
								   0,
								   &dwThreadID );
}

void CImageToolDoc::OnUpdateImageLrotatte90(CCmdUI* pCmdUI) 
{
	if( m_pSelectedImage == 0 || m_hThreadTools )
	{
		pCmdUI->Enable(0);
	}
}

void CImageToolDoc::OnImageMirror() 
{
	m_MenuCommand = ID_IMAGE_MIRROR;

	m_hThreadTools = CreateThread( NULL,
		                           0,
								   RunImageToolsThread,
								   this,
								   0,
								   &dwThreadID );
	
}

void CImageToolDoc::OnUpdateImageMirror(CCmdUI* pCmdUI) 
{
	if( m_pSelectedImage == 0 || m_hThreadTools )
	{
		pCmdUI->Enable(0);
	}
}

void CImageToolDoc::OnImageGamma()
{
	if( m_pSelectedImage == NULL )
	{
		return;
	}

	CGammaDlg dlg;
	dlg.m_gamma = (float)0.5;

	if( dlg.DoModal() == IDOK )
	{
		m_MenuCommand = ID_IMAGE_GAMMA;
		
		m_fp[0] = (void *)(long)( dlg.m_gamma * 1000 );
		
		m_hThreadTools = CreateThread( NULL,
			                           0,
									   RunImageToolsThread,
									   this,
									   0,
									   &dwThreadID );
	}	
}

void CImageToolDoc::OnUpdateImageGamma(CCmdUI* pCmdUI) 
{
	if( m_pSelectedImage == 0 || m_hThreadTools )
	{
		pCmdUI->Enable(0);
	}
}

void CImageToolDoc::OnImageThreshold() 
{
	if( m_pSelectedImage == NULL )
	{
		return;
	}
	
	CThresholdDlg dlg;

	if( dlg.DoModal() == IDOK )
	{
		m_MenuCommand = ID_IMAGE_THRESHOLD;
		m_fp[0] = (void *)(BYTE)( dlg.m_level );
		
		m_hThreadTools = CreateThread( NULL,
							           0,
									   RunImageToolsThread,
									   this,
									   0,
									   &dwThreadID );
	}
}

void CImageToolDoc::OnUpdateImageThreshold(CCmdUI* pCmdUI) 
{
	if( m_pSelectedImage == 0 || m_hThreadTools )
	{
		pCmdUI->Enable(0);
	}
}

void CImageToolDoc::OnImageLighten() 
{
	m_MenuCommand = ID_IMAGE_LIGHTEN;

	m_hThreadTools = CreateThread( NULL,
		                           0,
								   RunImageToolsThread,
								   this,
								   0,
								   &dwThreadID );	
}

void CImageToolDoc::OnUpdateImageLighten(CCmdUI* pCmdUI) 
{
	if( m_pSelectedImage == 0 || m_hThreadTools )
	{
		pCmdUI->Enable(0);
	}
}

void CImageToolDoc::OnImageDarken() 
{
	m_MenuCommand = ID_IMAGE_DARKEN;

	m_hThreadTools = CreateThread( NULL,
		                           0,
								   RunImageToolsThread,
								   this,
								   0,
								   &dwThreadID );		
}

void CImageToolDoc::OnUpdateImageDarken(CCmdUI* pCmdUI) 
{
	if( m_pSelectedImage == 0 || m_hThreadTools )
	{
		pCmdUI->Enable(0);
	}
}

void CImageToolDoc::OnImageGrayscale() 
{
	m_MenuCommand = ID_IMAGE_GRAYSCALE;

	m_hThreadTools = CreateThread( NULL,
		                           0,
								   RunImageToolsThread,
								   this,
								   0,
								   &dwThreadID );		
}

void CImageToolDoc::OnUpdateImageGrayscale(CCmdUI* pCmdUI) 
{
	if( m_pSelectedImage == 0 || m_hThreadTools )
	{
		pCmdUI->Enable(0);
	}
}

void CImageToolDoc::OnImageNegative() 
{
	m_MenuCommand = ID_IMAGE_NEGATIVE;

	m_hThreadTools = CreateThread( NULL,
		                           0,
								   RunImageToolsThread,
								   this,
								   0,
								   &dwThreadID );	
}

void CImageToolDoc::OnUpdateImageNegative(CCmdUI* pCmdUI) 
{
	if( m_pSelectedImage == 0 || m_hThreadTools )
	{
		pCmdUI->Enable(0);
	}
}

void CImageToolDoc::OnImageSharpen() 
{
	m_MenuCommand = ID_IMAGE_SHARPEN;

	m_hThreadTools = CreateThread( NULL,
		                           0,
								   RunImageToolsThread,
								   this,
								   0,
								   &dwThreadID );	
}

void CImageToolDoc::OnUpdateImageSharpen(CCmdUI* pCmdUI) 
{
	if( m_pSelectedImage == 0 || m_hThreadTools )
	{
		pCmdUI->Enable(0);
	}
}

void CImageToolDoc::OnImageContrast() 
{
	m_MenuCommand = ID_IMAGE_CONTRAST;

	m_hThreadTools = CreateThread( NULL,
		                           0,
								   RunImageToolsThread,
								   this,
								   0,
								   &dwThreadID );	
}

void CImageToolDoc::OnUpdateImageContrast(CCmdUI* pCmdUI) 
{
	if( m_pSelectedImage == 0 || m_hThreadTools )
	{
		pCmdUI->Enable(0);	
	}
}

void CImageToolDoc::OnUpdateAppExit(CCmdUI* pCmdUI) 
{
	if( m_hThreadTools )
	{
		pCmdUI->Enable(0);	
	}
}
