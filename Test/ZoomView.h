#pragma once

// ZoomView 뷰입니다.

class ZoomView : public CScrollView
{
	DECLARE_DYNCREATE(ZoomView)

protected:
	ZoomView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~ZoomView();

// Attributes
public:
	// Flicker free attributes
	CDC*		m_pdcMemory;
	CBitmap*	m_pBitmap;
	BOOL		m_bInitialSize;

protected:
	// Zooom attributes
	float		m_zoomFactor;
	float		m_minZoomFactor;
	float		m_maxZoomFactor;
	CSize		m_line;
	CSize		m_page;
	CSize		m_layout;
	CSize		m_displaySize;
	// Operations
public:
	// Display Operation /////////////////////////////////////////////////////
	int  SetMapMode(CDC* pDC);
	void SetDisplaySize(void);
	void SetZoomScale(float fScale);
	void SetZoomArea(CRect zoomRect);
	CPoint GetCenterScrollPosition(void);
	void LPtoDP(LPPOINT lpPoint);
	void LPtoDP(LPRECT lpRect);
	void LPtoDP(LPSIZE lpSize);
	void DPtoLP(LPPOINT lpPoint);
	void DPtoLP(LPRECT lpRect);
	void DPtoLP(LPSIZE lpSize);
	CSize GetDisplaySize() { return m_displaySize; };
	//////////////////////////////////////////////////////////////////////////
	int FloatToInt(float fNumber);
protected:
	void SetZoomFactor(float fZoomFactor);

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
};


