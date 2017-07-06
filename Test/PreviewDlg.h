#pragma once
#include "afxwin.h"
#include "myGdiPlus.h" 
#include "resource.h"

using namespace Gdiplus; 
// CPreviewDlg ��ȭ �����Դϴ�.
class CPreviewDlg : public CDialog
{
	DECLARE_DYNAMIC(CPreviewDlg)

public:
	CPreviewDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CPreviewDlg();

	//���� �̹���
	Bitmap  *m_pSelectedImage;	
	inline Bitmap *GetImage() { return m_pSelectedImage; }	
	
// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_PREVIEW_DLG };	
	CStatic m_wndCanvas;
	CString m_filePath;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);			
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};
