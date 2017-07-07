#pragma once

#include "CZoomView.h"

// CMyScrollView ���Դϴ�.

class CMyScrollView : public CZoomView
{
	DECLARE_DYNCREATE(CMyScrollView)

protected:
	CMyScrollView();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
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
	virtual void OnDraw(CDC* pDC);      // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual void OnInitialUpdate();     // ������ �� ó���Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnViewZoomin();
	afx_msg void OnViewZoomout();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
};


