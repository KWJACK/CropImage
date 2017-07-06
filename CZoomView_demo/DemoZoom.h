// DemoZoom.h : main header file for the DEMOZOOM application
//

#if !defined(AFX_DEMOZOOM_H__6C43CE57_3367_4F7E_BDF9_276054775ABD__INCLUDED_)
#define AFX_DEMOZOOM_H__6C43CE57_3367_4F7E_BDF9_276054775ABD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CDemoZoomApp:
// See DemoZoom.cpp for the implementation of this class
//

class CDemoZoomApp : public CWinApp
{
public:
	CDemoZoomApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDemoZoomApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CDemoZoomApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEMOZOOM_H__6C43CE57_3367_4F7E_BDF9_276054775ABD__INCLUDED_)
