// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__6FF8C1FF_7D6C_4761_9D4F_F73B92B46A48__INCLUDED_)
#define AFX_MAINFRM_H__6FF8C1FF_7D6C_4761_9D4F_F73B92B46A48__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMainFrame : public CFrameWnd
{

protected:
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
	BOOL        m_bShowPreviewBar,
		        m_bShowDirectoryBar;

	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewDirectoryBar();
	afx_msg void OnUpdateViewDirectoryBar(CCmdUI* pCmdUI);
	afx_msg void OnViewPreviewBar();
	afx_msg void OnUpdateViewPreviewBar(CCmdUI* pCmdUI);
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
#endif // !defined(AFX_MAINFRM_H__6FF8C1FF_7D6C_4761_9D4F_F73B92B46A48__INCLUDED_)
