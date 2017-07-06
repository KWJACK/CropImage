// DemoZoomDoc.cpp : implementation of the CDemoZoomDoc class
//

#include "stdafx.h"
#include "DemoZoom.h"

#include "DemoZoomDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDemoZoomDoc

IMPLEMENT_DYNCREATE(CDemoZoomDoc, CDocument)

BEGIN_MESSAGE_MAP(CDemoZoomDoc, CDocument)
	//{{AFX_MSG_MAP(CDemoZoomDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDemoZoomDoc construction/destruction

CDemoZoomDoc::CDemoZoomDoc()
{
	// TODO: add one-time construction code here

}

CDemoZoomDoc::~CDemoZoomDoc()
{
}

BOOL CDemoZoomDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CDemoZoomDoc serialization

void CDemoZoomDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CDemoZoomDoc diagnostics

#ifdef _DEBUG
void CDemoZoomDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDemoZoomDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDemoZoomDoc commands
