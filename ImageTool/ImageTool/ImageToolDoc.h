// ImageToolDoc.h : interface of the CImageToolDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMAGETOOLDOC_H__4BD44B86_4DF2_4F17_BB4D_F2FBCFA4519A__INCLUDED_)
#define AFX_IMAGETOOLDOC_H__4BD44B86_4DF2_4F17_BB4D_F2FBCFA4519A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include "myGdiPlus.h" 
using namespace Gdiplus; 

#define MAX_UNDO_LEVELS 8
#define ID_NO_MENU 500

class CImageToolDoc : public CDocument
{
protected:
	CImageToolDoc();
	DECLARE_DYNCREATE( CImageToolDoc )

public:
	Bitmap  *m_pSelectedImage;
	HANDLE	m_hThreadTools;
	long	m_MenuCommand;
	void	*m_fp[4];

	inline Bitmap *GetImage() { return m_pSelectedImage; }
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImageToolDoc)
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

	void SubmitUndo();
	void SelectItem( int nIndex );
	void SelectFolder( LPCSTR pstr );
	BOOL IsImageGDIPLUSValid( CString filePath );
	
	CString m_strCurrentDirectory;
	std::vector<CString> m_vFileName;

	virtual ~CImageToolDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	Bitmap* imageUndo[ MAX_UNDO_LEVELS ];
	int		m_UndoLevel, m_nSelectedItem;
	DWORD	dwThreadID;

	//{{AFX_MSG(CImageToolDoc)
	afx_msg void OnEditUndo();
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnEditRedo();
	afx_msg void OnUpdateEditRedo(CCmdUI* pCmdUI);
	afx_msg void OnImageFlip();
	afx_msg void OnUpdateImageFlip(CCmdUI* pCmdUI);
	afx_msg void OnImageRotate();
	afx_msg void OnUpdateImageRotate(CCmdUI* pCmdUI);
	afx_msg void OnImageMirror();
	afx_msg void OnUpdateImageMirror(CCmdUI* pCmdUI);
	afx_msg void OnImageGamma();
	afx_msg void OnUpdateImageGamma(CCmdUI* pCmdUI);
	afx_msg void OnImageThreshold();
	afx_msg void OnUpdateImageThreshold(CCmdUI* pCmdUI);
	afx_msg void OnImageLighten();
	afx_msg void OnUpdateImageLighten(CCmdUI* pCmdUI);
	afx_msg void OnImageDarken();
	afx_msg void OnUpdateImageDarken(CCmdUI* pCmdUI);
	afx_msg void OnImageGrayscale();
	afx_msg void OnUpdateImageGrayscale(CCmdUI* pCmdUI);
	afx_msg void OnImageNegative();
	afx_msg void OnUpdateImageNegative(CCmdUI* pCmdUI);
	afx_msg void OnImageSharpen();
	afx_msg void OnUpdateImageSharpen(CCmdUI* pCmdUI);
	afx_msg void OnImageContrast();
	afx_msg void OnUpdateImageContrast(CCmdUI* pCmdUI);
	afx_msg void OnImageLrotatte90();
	afx_msg void OnUpdateImageLrotatte90(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAppExit(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
#endif // !defined(AFX_IMAGETOOLDOC_H__4BD44B86_4DF2_4F17_BB4D_F2FBCFA4519A__INCLUDED_)
