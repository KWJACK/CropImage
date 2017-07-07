#pragma once

#include "CZoomView.h"

// CMyScrollView 뷰입니다.

class CMyScrollView : public CZoomView
{
	DECLARE_DYNCREATE(CMyScrollView)

protected:
	CMyScrollView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CMyScrollView();

	// Attributes
public:
	//CDemoZoomDoc* GetDocument();
	BOOL m_bSelectMode;
	CRect m_rubberBand;
	CPoint m_ptStart;
public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void OnDraw(CDC* pDC);      // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual void OnInitialUpdate();     // 생성된 후 처음입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnViewZoomin();
	afx_msg void OnViewZoomout();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
};


