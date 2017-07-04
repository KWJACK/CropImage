#pragma once

#define SHELLTREEEX_QUICK_CHLDDETECT	0x00000001
#define SHELLTREEEX_KEEP_CHILDREN		0x00000002
#define SHELLTREEEX_EXPAND_ALL			0x00000004

// CMFCShellTreeCtrlEx
class CMFCShellTreeCtrlEx : public CMFCShellTreeCtrl
{
	DECLARE_DYNAMIC(CMFCShellTreeCtrlEx)

public:
	CMFCShellTreeCtrlEx(DWORD dwProp = 0);
	virtual ~CMFCShellTreeCtrlEx();

	void SetRootFolder(LPCTSTR szRootDir, BOOL bFullPath = FALSE, DWORD *pdwProp = NULL);
	void RefreshEx();
	void SetFlagsEx(DWORD dwFlags, BOOL bRefresh);
	DWORD_PTR GetItemDataEx(HTREEITEM hItem) const;
	BOOL SetItemDataEx(HTREEITEM hItem, DWORD_PTR dwData);


protected:
	afx_msg void OnDeleteitem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemexpanding(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void PreSubclassWindow();
	DECLARE_MESSAGE_MAP()

	CString m_cRootDir;
	DWORD m_dwProp;
	BOOL m_bFullRootPath;

	void InitTreeEx();
	BOOL GetRootItemsEx();
	BOOL IsCustomRoot() { return !m_cRootDir.IsEmpty(); }
	BOOL GetFullRootPIDL(LPSHELLFOLDER pParentFolder, CStringArray& cDirPartArr, int nIndex, LPAFX_SHELLITEMINFO pItem);
	HRESULT EnumObjects(HTREEITEM hParentItem, LPSHELLFOLDER pParentFolder, LPITEMIDLIST pidlParent);
	virtual void FreeItemData(HTREEITEM hItem, DWORD_PTR dwItemData){}
	virtual void OnItemInserted(HTREEITEM hItem, LPCTSTR szFolderPath){}

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


