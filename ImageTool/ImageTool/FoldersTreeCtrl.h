#if !defined(AFX_FOLDERSTREECTRL_H__B83A961E_B6F0_46D7_855D_E8720E8300A8__INCLUDED_)
#define AFX_FOLDERSTREECTRL_H__B83A961E_B6F0_46D7_855D_E8720E8300A8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FoldersTreeCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFoldersTreeCtrl window
class CFoldersTreeCtrl : public CTreeCtrl
{
public:
	CFoldersTreeCtrl();

	typedef struct tagLVID
	{
		LPSHELLFOLDER lpsfParent;
		LPITEMIDLIST  lpi;
		ULONG         ulAttribs;
	} LVITEMDATA, *LPLVITEMDATA;

	typedef struct tagID
	{
		LPSHELLFOLDER lpsfParent;
		LPITEMIDLIST  lpi;
		LPITEMIDLIST  lpifq;
	} TVITEMDATA, *LPTVITEMDATA;

	void PopulateTree();
	void OnFolderExpanding(NMHDR* pNMHDR, LRESULT* pResult);
	BOOL OnFolderSelected(NMHDR* pNMHDR, LRESULT* pResult, CString &szFolderPath);
	void EnableImages();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFoldersTreeCtrl)
	//}}AFX_VIRTUAL

	virtual ~CFoldersTreeCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CFoldersTreeCtrl)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	LPITEMIDLIST	ConcatPidls(LPCITEMIDLIST pidl1, LPCITEMIDLIST pidl2);
    LPITEMIDLIST	CopyITEMID(LPMALLOC lpMalloc, LPITEMIDLIST lpi);
    BOOL			GetName(LPSHELLFOLDER lpsf, LPITEMIDLIST  lpi, DWORD dwFlags, LPSTR lpFriendlyName);
    LPITEMIDLIST	CreatePidl(UINT cbSize);
    UINT			GetSize(LPCITEMIDLIST pidl);
    LPITEMIDLIST	Next(LPCITEMIDLIST pidl);
	int		        GetItemIcon(LPITEMIDLIST lpi, UINT uFlags);
	void			FillTreeView(LPSHELLFOLDER lpsf, LPITEMIDLIST  lpifq, HTREEITEM     hParent);

	static int CALLBACK TreeViewCompareProc(LPARAM, LPARAM, LPARAM);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
#endif // !defined(AFX_FOLDERSTREECTRL_H__B83A961E_B6F0_46D7_855D_E8720E8300A8__INCLUDED_)
