// ImageTool.h : main header file for the IMAGETOOL application
//

#if !defined(AFX_IMAGETOOL_H__8465860F_D179_43D7_B7C4_EB66D21162FE__INCLUDED_)
#define AFX_IMAGETOOL_H__8465860F_D179_43D7_B7C4_EB66D21162FE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

#define	THUMBNAIL_WIDTH		100
#define	THUMBNAIL_HEIGHT	90

/////////////////////////////////////////////////////////////////////////////
// CImageToolApp:
// See ImageTool.cpp for the implementation of this class
//
class CImageToolApp : public CWinApp
{
public:
	CImageToolApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImageToolApp)
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CImageToolApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
#endif // !defined(AFX_IMAGETOOL_H__8465860F_D179_43D7_B7C4_EB66D21162FE__INCLUDED_)
