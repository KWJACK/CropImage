// DemoZoomDoc.h : interface of the CDemoZoomDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEMOZOOMDOC_H__B573B658_A0DC_4A9E_9417_9D2424185342__INCLUDED_)
#define AFX_DEMOZOOMDOC_H__B573B658_A0DC_4A9E_9417_9D2424185342__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CDemoZoomDoc : public CDocument
{
protected: // create from serialization only
	CDemoZoomDoc();
	DECLARE_DYNCREATE(CDemoZoomDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDemoZoomDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDemoZoomDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CDemoZoomDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEMOZOOMDOC_H__B573B658_A0DC_4A9E_9417_9D2424185342__INCLUDED_)
