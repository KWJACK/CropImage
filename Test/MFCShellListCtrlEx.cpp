// MFCShellListCtrlEx.cpp : implementation file
//
#include "stdafx.h"
#include "MFCShellUtils.h"
#include "MFCShellListCtrlEx.h"

// Developed by Vladimir Misovsky
// January 2016

// CMFCShellListCtrlEx

IMPLEMENT_DYNAMIC(CMFCShellListCtrlEx, CMFCShellListCtrl)

CMFCShellListCtrlEx::CMFCShellListCtrlEx()
{

}

CMFCShellListCtrlEx::~CMFCShellListCtrlEx()
{
}

BEGIN_MESSAGE_MAP(CMFCShellListCtrlEx, CMFCShellListCtrl)
	ON_NOTIFY_REFLECT(LVN_DELETEITEM, &CMFCShellListCtrlEx::OnDeleteitem)
END_MESSAGE_MAP()

// CMFCShellListCtrlEx message handlers
BOOL CMFCShellListCtrlEx::IsItemToCopy(LPCTSTR szFileName)
{
	for (int i = 0; i<m_cCopyNamesArr.GetSize(); i++)
	{
		if (!m_cCopyNamesArr[i].CompareNoCase(szFileName))
			return TRUE;
	}
	return FALSE;
}

HRESULT CMFCShellListCtrlEx::EnumObjects(LPSHELLFOLDER pParentFolder, LPITEMIDLIST pidlParent)
{
	BOOL bCopyItems = m_cCopyNamesArr.GetSize() > 0;
	LPSHELLFOLDER pDesktopFolder = NULL;
	if (SUCCEEDED(SHGetDesktopFolder(&pDesktopFolder)))
	{
		STRRET str;
		TCHAR szFolderPath[MAX_PATH] = { 0 };
		pDesktopFolder->GetDisplayNameOf(pidlParent, SHGDN_FORPARSING, &str);
		StrRetToBuf(&str, pidlParent, szFolderPath, sizeof(szFolderPath) / sizeof((szFolderPath)[0]));
		pDesktopFolder->Release();
		PreEnumObjects(szFolderPath);
	}
	ASSERT_VALID(this);
	ASSERT_VALID(afxShellManager);
	LPENUMIDLIST pEnum = NULL;
	HRESULT hRes = pParentFolder->EnumObjects(NULL, m_nTypes, &pEnum);
	if (SUCCEEDED(hRes) && pEnum != NULL)
	{
		LPITEMIDLIST pidlTemp;
		DWORD dwFetched = 1;
		//enumerate the item's PIDLs
		while (pEnum->Next(1, &pidlTemp, &dwFetched) == S_OK && dwFetched)
		{
			CString cFileName = CMFCShellUtils::GetDisplayName(pParentFolder, pidlTemp, FALSE);
			// To determine whether current item is to be inserted
			// calling either IsItemToCopy if items for another list view are being copied or IncludeItem otherwise
			BOOL bInclude = bCopyItems ? IsItemToCopy(cFileName) : IncludeItem(cFileName);
			if (!bInclude)
			{
				dwFetched = 0;
				continue;
			}
			LVITEM lvItem;
			ZeroMemory(&lvItem, sizeof(lvItem));
			//fill in the TV_ITEM structure for this item
			lvItem.mask = LVIF_PARAM | LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
			//AddRef the parent folder so it's pointer stays valid
			pParentFolder->AddRef();
			//put the private information in the lParam
			LPAFX_SHELLITEMINFOEX pItem = (LPAFX_SHELLITEMINFOEX)GlobalAlloc(GPTR, sizeof(AFX_SHELLITEMINFOEX));
			pItem->pidlRel = pidlTemp;
			pItem->pidlFQ = afxShellManager->ConcatenateItem(pidlParent, pidlTemp);
			pItem->pParentFolder = pParentFolder;
			lvItem.lParam = (LPARAM)pItem;
			lvItem.pszText = _T("");
			lvItem.iImage = OnGetItemIcon(GetItemCount(), (LPAFX_SHELLITEMINFO)pItem);
			//determine if the item is shared
			DWORD dwAttr = SFGAO_DISPLAYATTRMASK;
			pParentFolder->GetAttributesOf(1, (LPCITEMIDLIST*)&pidlTemp, &dwAttr);
			if (dwAttr & SFGAO_SHARE)
			{
				lvItem.mask |= LVIF_STATE;
				lvItem.stateMask |= LVIS_OVERLAYMASK;
				lvItem.state |= INDEXTOOVERLAYMASK(1); //1 is the index for the shared overlay image
			}
			if (dwAttr & SFGAO_GHOSTED)
			{
				lvItem.mask |= LVIF_STATE;
				lvItem.stateMask |= LVIS_CUT;
				lvItem.state |= LVIS_CUT;
			}
			if (bCopyItems)
			{
				lvItem.mask |= LVIF_STATE;
				lvItem.state |= LVIS_SELECTED;
				lvItem.stateMask |= LVIS_SELECTED;
			}
			int iItem = InsertItem(&lvItem);
			if (iItem >= 0)
			{
				// Set columns:
				const int nColumns = m_wndHeader.GetItemCount();
				for (int iColumn = 0; iColumn < nColumns; iColumn++)
				{
					SetItemText(iItem, iColumn, OnGetItemText(iItem, iColumn, (LPAFX_SHELLITEMINFO)pItem));
				}
			}
			OnItemInserted(iItem);
			dwFetched = 0;
		}
		pEnum->Release();
	}
	return hRes;
}

DWORD_PTR CMFCShellListCtrlEx::GetItemDataEx(int nItem) const
{
	LPAFX_SHELLITEMINFOEX pItem = (nItem >= 0 && nItem < GetItemCount()) ? (LPAFX_SHELLITEMINFOEX)GetItemData(nItem) : NULL;
	return pItem ? pItem->dwItemData : 0;
}

BOOL CMFCShellListCtrlEx::SetItemDataEx(int nItem, DWORD_PTR dwData)
{
	LPAFX_SHELLITEMINFOEX pItem = (nItem >= 0 && nItem < GetItemCount()) ? (LPAFX_SHELLITEMINFOEX)GetItemData(nItem) : NULL;
	if (!pItem)
		return FALSE;
	pItem->dwItemData = dwData;
	return TRUE;
}

void CMFCShellListCtrlEx::OnDeleteitem(NMHDR* pNMHDR, LRESULT* pResult)
{
	ASSERT_VALID(afxShellManager);
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	ENSURE(pNMListView != NULL);
	LPAFX_SHELLITEMINFOEX pItem = (LPAFX_SHELLITEMINFOEX)pNMListView->lParam;
	// Calling FreeItemData to free custom item data
	if (pItem)
		FreeItemData(pNMListView->iItem, pItem->dwItemData);
	CMFCShellListCtrl::OnDeleteitem(pNMHDR, pResult);
}

BOOL CMFCShellListCtrlEx::CopyItems(const CMFCShellListCtrlEx& cSrcListCtrl, const CUIntArray& cItemPosArr)
{
	if (!m_psfCurFolder)
		return FALSE;
	int nItemCount = GetItemCount();
	BOOL bResult = TRUE;
	// Check if non of the items to be copied is already in the list
	for (int i = 0; i < cItemPosArr.GetSize(); i++)
	{
		int nItem = cItemPosArr[i];
		LPAFX_SHELLITEMINFOEX pItem = (nItem >= 0 && nItem < cSrcListCtrl.GetItemCount()) ? (LPAFX_SHELLITEMINFOEX)cSrcListCtrl.GetItemData(nItem) : NULL;
		BOOL bRemove = pItem == NULL || pItem->pParentFolder == NULL;
		if (!bRemove)
		{
			CString cItemName = CMFCShellUtils::GetDisplayName(pItem->pParentFolder, pItem->pidlRel, FALSE);
			for (int j = 0; j < nItemCount; j++)
			{
				CString cName = GetItemText(j, AFX_ShellList_ColumnName);
				if (!cName.CompareNoCase(cItemName))
				{
					bRemove = TRUE;
					break;
				}
			}
			// If item already exists => remove the appropriate m_cCopyNamesArr element
			if (!bRemove)
				m_cCopyNamesArr.Add(cItemName);
		}
	}
	bResult = m_cCopyNamesArr.GetSize() > 0;
	// If copy items array isn't empty...
	if (bResult)
	{
		CWaitCursor wait;
		SetRedraw(FALSE);
		// call EnumObjects to add new files to the list
		bResult = SUCCEEDED(EnumObjects(m_psfCurFolder, m_pidlCurFQ));
		m_cCopyNamesArr.RemoveAll();
		// and re-sort the list
		if (bResult && (GetStyle() & LVS_REPORT))
			Sort(AFX_ShellList_ColumnName);
		SetRedraw(TRUE);
		RedrawWindow();
	}
	return bResult;
}
