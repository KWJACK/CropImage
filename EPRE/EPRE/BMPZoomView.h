#pragma once
// CBMPZoomView ���Դϴ�.
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
	CBMPZoomView();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CBMPZoomView();
	//Attributes
public:
	//CDemoZoomDoc* GetDocument();
	BOOL m_bSelectMode;			//�׸��� ���
	CRect m_rubberBand, m_Canvas_Rect;
	CPoint m_ptStart, m_ptEnd;	//���콺 �巡�׿� ���� �׸� ������ ������
	BOOL m_create_canvas;
	CStatic m_wndCanvas;	

	CPoint m_ImageDest;//�̹��� ��� ������ cx = left, cy = top
	CPoint m_ExpandDest;//�þ �̹��� ũ��
	float m_fResolution_W, m_fResolution_H;	//�̹��� ����
	//���� �̹���
	Bitmap  *m_pSelectedImage;		
	Graphics* p_graphics;//GDI+ ��°�ü�� ����Ű�� ������
	//���� �̸� ���� ���� ����
	CString m_fileName, m_oldFileName;
	CString m_sPath;//�̹��� ���� ���
	BOOL m_IDOK;
	//����ȭ ��� �������
	CString m_result_path;
	MyBMPclass* m_BMPclass;
public:
	virtual void OnDraw(CDC* pDC);      // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	void ShowImageInfo(CPoint a_point);//���콺 Ŀ���� ���� ��Ʈ�� �ȼ� ���� ���
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

