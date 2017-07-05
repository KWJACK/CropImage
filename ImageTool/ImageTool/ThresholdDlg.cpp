// ThresholdDlg.cpp : implementation file
//

#include "stdafx.h"
#include "imagetool.h"
#include "ThresholdDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CThresholdDlg dialog
CThresholdDlg::CThresholdDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CThresholdDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CThresholdDlg)
	m_level = 0;
	//}}AFX_DATA_INIT
}

void CThresholdDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CThresholdDlg)
	DDX_Text(pDX, IDC_EDIT1, m_level);
	DDV_MinMaxByte(pDX, m_level, 0, 255);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CThresholdDlg, CDialog)
	//{{AFX_MSG_MAP(CThresholdDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()