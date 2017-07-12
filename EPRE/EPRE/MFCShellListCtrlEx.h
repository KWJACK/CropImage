#pragma once

// CMFCShellListCtrlEx
#include "BMPZoomView.h"

class CMFCShellListCtrlEx : public CMFCShellListCtrl
{
	DECLARE_DYNAMIC(CMFCShellListCtrlEx)

public:
	CMFCShellListCtrlEx();
	virtual ~CMFCShellListCtrlEx();

	DWORD_PTR GetItemDataEx(int nItem) const;
	BOOL SetItemDataEx(int nItem, DWORD_PTR dwData);
	BOOL CopyItems(const CMFCShellListCtrlEx& cSrcListCtrl, const CUIntArray& cItemPosArr);	
	
	int m_nItem;
	CString m_selFilePath;
	CBMPZoomView* m_preViewDlg;
protected:
	DECLARE_MESSAGE_MAP()

	CStringArray m_cCopyNamesArr;

	BOOL IsItemToCopy(LPCTSTR szFileName);
	virtual HRESULT EnumObjects(LPSHELLFOLDER pParentFolder, LPITEMIDLIST pidlParent);
	virtual void PreEnumObjects(LPCTSTR szFolderPath) {}
	virtual BOOL IncludeItem(LPCTSTR szFileName) { return TRUE; }
	virtual void FreeItemData(int nItem, DWORD_PTR dwItemData) {}
	virtual void OnItemInserted(int nItem) {}
public:
	afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);		
	afx_msg void OnDeleteitem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLvnKeydown(NMHDR *pNMHDR, LRESULT *pResult);
};


