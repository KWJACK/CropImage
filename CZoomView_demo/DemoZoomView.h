// DemoZoomView.h : interface of the CDemoZoomView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEMOZOOMVIEW_H__FCB563F7_4DF6_4831_A1E5_8189273971AB__INCLUDED_)
#define AFX_DEMOZOOMVIEW_H__FCB563F7_4DF6_4831_A1E5_8189273971AB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ZoomView.h"

class CDemoZoomView : public CZoomView
{
protected: // create from serialization only
	CDemoZoomView();
	DECLARE_DYNCREATE(CDemoZoomView)

// Attributes
public:
	CDemoZoomDoc* GetDocument();
	BOOL m_bSelectMode;
	CRect m_rubberBand;
	CPoint m_ptStart;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDemoZoomView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDemoZoomView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CDemoZoomView)
	afx_msg void OnViewZoomin();
	afx_msg void OnViewZoomout();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in DemoZoomView.cpp
inline CDemoZoomDoc* CDemoZoomView::GetDocument()
   { return (CDemoZoomDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEMOZOOMVIEW_H__FCB563F7_4DF6_4831_A1E5_8189273971AB__INCLUDED_)
