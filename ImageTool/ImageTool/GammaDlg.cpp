// GammaDlg.cpp : implementation file
//

#include "stdafx.h"
#include "imagetool.h"
#include "GammaDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGammaDlg dialog
CGammaDlg::CGammaDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGammaDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGammaDlg)
	m_gamma = 0.0f;
	//}}AFX_DATA_INIT
}

void CGammaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGammaDlg)
	DDX_Text(pDX, IDC_EDIT_GAMMA, m_gamma);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CGammaDlg, CDialog)
	//{{AFX_MSG_MAP(CGammaDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
