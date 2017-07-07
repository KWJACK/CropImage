#pragma once
// CBMPZoomView 뷰입니다.
#include "ZoomView.h"
#include "afxwin.h"
#include "myGdiPlus.h" 
#include "resource.h"

using namespace Gdiplus; 
class CBMPZoomView : public CZoomView
{
	DECLARE_DYNCREATE(CBMPZoomView)

protected:
	CBMPZoomView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CBMPZoomView();
	//Attributes
public:
	//CDemoZoomDoc* GetDocument();
	BOOL m_bSelectMode;
	CRect m_rubberBand;
	CPoint m_ptStart;
	
	CStatic m_wndCanvas;
	CString m_filePath;
	//선택 이미지
	Bitmap  *m_pSelectedImage;	
	inline Bitmap *GetImage() { return m_pSelectedImage; }	

public:
	virtual void OnDraw(CDC* pDC);      // 이 뷰를 그리기 위해 재정의되었습니다.
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnViewZoomin();
	afx_msg void OnViewZoomout();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};

