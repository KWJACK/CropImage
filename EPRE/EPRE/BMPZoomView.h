#pragma once
// CBMPZoomView 뷰입니다.
#include "ZoomView.h"
#include "afxwin.h"
#include "myGdiPlus.h" 
#include "resource.h"
#include "MyBMPclass.h"	//write&save BMP
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
	BOOL m_bSelectMode;			//그리기 모드
	CRect m_rubberBand, m_Canvas_Rect;
	CPoint m_ptStart, m_ptEnd;	//마우스 드래그에 따라 그릴 상자의 시작점
	BOOL m_create_canvas;
	CStatic m_wndCanvas;	

	CPoint m_ImageDest;//이미지 출력 시작점 cx = left, cy = top
	CPoint m_ExpandDest;//늘어난 이미지 크기
	float m_fResolution_W, m_fResolution_H;	//이미지 배율
	//선택 이미지
	Bitmap  *m_pSelectedImage;		
	Graphics* p_graphics;//GDI+ 출력개체를 가르키는 포인터
	//파일 이름 변수 저장 관련
	CString m_fileName, m_oldFileName;
	CString m_sPath;//이미지 저장 경로
	BOOL m_IDOK;
	//이진화 결과 저장관련
	CString m_result_path;
	MyBMPclass* m_BMPclass;
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
	void ShowImageInfo(CPoint a_point);//마우스 커서에 따른 비트맵 픽셀 정보 출력
	void setColorStyle(CClientDC &dc, CPen &pen, CBrush &brush);

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
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSaveCropImageFile();
	afx_msg void OnSetImagePath();
	afx_msg void On24bitBMPto1bitBinarization();
	afx_msg void OnSetResultPath();
	afx_msg void OnDestroy();
};

