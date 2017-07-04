#pragma once

typedef struct _AFX_SHELLITEMINFOEX
{
	LPSHELLFOLDER  pParentFolder;
	LPITEMIDLIST   pidlFQ;
	LPITEMIDLIST   pidlRel;
	DWORD_PTR	   dwItemData;

	_AFX_SHELLITEMINFOEX()
	{
		pParentFolder = NULL;
		pidlFQ = NULL;
		pidlRel = NULL;
		dwItemData = NULL;
	}
}
AFX_SHELLITEMINFOEX, FAR *LPAFX_SHELLITEMINFOEX;

class CMFCShellUtils
{
public:
	CMFCShellUtils();
	~CMFCShellUtils();

	static CString GetDisplayName(IShellFolder *pshf, LPITEMIDLIST pidl, BOOL bFullPath = FALSE);
};

