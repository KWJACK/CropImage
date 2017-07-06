#pragma once
#include "afxwin.h"
#include "myGdiPlus.h" 
#include "resource.h"

using namespace Gdiplus; 
// CPreviewDlg 대화 상자입니다.
class CPreviewDlg : public CDialog
{
	DECLARE_DYNAMIC(CPreviewDlg)

public:
	CPreviewDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPreviewDlg();

	//선택 이미지
	Bitmap  *m_pSelectedImage;	
	inline Bitmap *GetImage() { return m_pSelectedImage; }	
	
// 대화 상자 데이터입니다.
	enum { IDD = IDD_PREVIEW_DLG };	
	CStatic m_wndCanvas;
	CString m_filePath;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);			
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};
