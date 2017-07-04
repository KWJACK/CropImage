// MFCShellTreeCtrlEx.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Test.h"
#include "MFCShellTreeCtrlEx.h"
#include "MFCShellUtils.h"
//#include "afxshellmanager.h"

// CMFCShellTreeCtrlEx

IMPLEMENT_DYNAMIC(CMFCShellTreeCtrlEx, CMFCShellTreeCtrl)

CMFCShellTreeCtrlEx::CMFCShellTreeCtrlEx(DWORD dwProp) : m_dwProp(dwProp), m_bFullRootPath(FALSE)
{

}

CMFCShellTreeCtrlEx::~CMFCShellTreeCtrlEx()
{
}


BEGIN_MESSAGE_MAP(CMFCShellTreeCtrlEx, CMFCShellTreeCtrl)
	ON_WM_DELETEITEM()
	ON_WM_CREATE()	
	ON_NOTIFY_REFLECT(TVN_DELETEITEM, &CMFCShellTreeCtrlEx::OnDeleteitem)
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDING, &CMFCShellTreeCtrlEx::OnItemexpanding)
END_MESSAGE_MAP()



// CMFCShellTreeCtrlEx 메시지 처리기입니다.




void CMFCShellTreeCtrlEx::OnDeleteItem(int nIDCtl, LPDELETEITEMSTRUCT lpDeleteItemStruct)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CMFCShellTreeCtrl::OnDeleteItem(nIDCtl, lpDeleteItemStruct);
}


void CMFCShellTreeCtrlEx::RefreshEx()
{
	ASSERT_VALID(this);
	DeleteAllItems();
	GetRootItemsEx();
	TreeView_SetScrollTime(GetSafeHwnd(), 100);
}

void CMFCShellTreeCtrlEx::InitTreeEx()
{
	TCHAR szWinDir[MAX_PATH + 1];
	if (GetWindowsDirectory(szWinDir, MAX_PATH) > 0)
	{
		SHFILEINFO sfi;
		SetImageList(CImageList::FromHandle((HIMAGELIST)SHGetFileInfo(szWinDir, 0, &sfi, sizeof(SHFILEINFO), SHGFI_SYSICONINDEX | SHGFI_SMALLICON)), 0);
	}
	RefreshEx();
}

int CMFCShellTreeCtrlEx::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMFCShellTreeCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	if (afxShellManager == NULL)
	{
		TRACE0("You need to initialize CShellManager first\n");
		return -1;
	}
	InitTreeEx();
	return 0;
}


void CMFCShellTreeCtrlEx::PreSubclassWindow()
{
	CTreeCtrl::PreSubclassWindow();
	_AFX_THREAD_STATE* pThreadState = AfxGetThreadState();
	if (pThreadState->m_pWndInit == NULL)
		InitTreeEx();
}

BOOL CMFCShellTreeCtrlEx::GetRootItemsEx()
{
	ASSERT_VALID(this);
	ENSURE(afxShellManager != NULL);
	ASSERT_VALID(afxShellManager);
	LPITEMIDLIST pidl;
	LPSHELLFOLDER pParentFolder = NULL;
	if (FAILED(SHGetSpecialFolderLocation(NULL, CSIDL_DESKTOP, &pidl)))
		return FALSE;
	if (FAILED(SHGetDesktopFolder(&pParentFolder)))
		return FALSE;
	LPAFX_SHELLITEMINFOEX pItem = (LPAFX_SHELLITEMINFOEX)GlobalAlloc(GPTR, sizeof(AFX_SHELLITEMINFOEX));
	ENSURE(pItem != NULL);
	pItem->pidlRel = pidl;
	pItem->pidlFQ = afxShellManager->CopyItem(pidl);
	// The desktop doesn't have a parent folder, so make this NULL:
	pItem->pParentFolder = NULL;
	CString cFolderPath = IsCustomRoot() ? m_cRootDir : CMFCShellUtils::GetDisplayName(pParentFolder, pidl, TRUE);
	// If a custim root folder was set...
	if (IsCustomRoot())
	{
		// Just going through the full path from desktop to the custom root folder
		// to get correct LPAFX_SHELLITEMINFOEX member values for our root item
		CStringArray cDirPartArr;
		// The first child item is "Computer", specified by its CSIDL
		CString cComputer;
		cComputer.Format(_T("%d*"), (int)CSIDL_DRIVES);
		cDirPartArr.Add(cComputer);
		// Tokenizing full folder path to get the remaining child items
		int nPos = 0;
		CString cDirPart;
		cDirPart = m_cRootDir.Tokenize(_T("\\"), nPos);
		while (cDirPart != _T(""))
		{
			// Appendig '\' to the drive item
			if (cDirPartArr.GetSize() == 1)
				cDirPart += _T("\\");
			cDirPartArr.Add(cDirPart);
			cDirPart = m_cRootDir.Tokenize(_T("\\"), nPos);
		}
		BOOL bResult = TRUE;
		int nCount = cDirPartArr.GetSize();
		// Updating pItem members...
		for (int i = 0; i < nCount; i++)
		{
			bResult = GetFullRootPIDL(pParentFolder, cDirPartArr, i, (LPAFX_SHELLITEMINFO)pItem);
			if (bResult)
			{
				bResult = SUCCEEDED(pItem->pParentFolder->BindToObject(pItem->pidlRel, NULL, IID_IShellFolder, (LPVOID*)&pParentFolder));
				// Releasing pItem->pParentFolder interface for each consequtive child item except for the last one (our root folder)
				if (i < nCount - 1 && pItem->pParentFolder != NULL)
					pItem->pParentFolder->Release();
			}
			if (!bResult)
				break;
		}
		// If failed to find all child items =>  free all allocated resources
		if (!bResult)
		{
			afxShellManager->FreeItem(pItem->pidlFQ);
			afxShellManager->FreeItem(pItem->pidlRel);
			if (pItem->pParentFolder != NULL)
				pItem->pParentFolder->Release();
			GlobalFree((HGLOBAL)pItem);
			return FALSE;
		}
	}
	// Performing default CMFCShellTreeCtrl logic to insert top level item
	TV_ITEM tvItem;
	tvItem.mask = TVIF_PARAM | TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_CHILDREN;
	tvItem.lParam = (LPARAM)pItem;
	CString strItem = (IsCustomRoot() && m_bFullRootPath) ? cFolderPath : OnGetItemText((LPAFX_SHELLITEMINFO)pItem);
	tvItem.pszText = strItem.GetBuffer(strItem.GetLength());
	tvItem.iImage = OnGetItemIcon((LPAFX_SHELLITEMINFO)pItem, FALSE);
	tvItem.iSelectedImage = OnGetItemIcon((LPAFX_SHELLITEMINFO)pItem, TRUE);
	// Assume the desktop has children:
	tvItem.cChildren = TRUE;
	// Fill in the TV_INSERTSTRUCT structure for this item:
	TV_INSERTSTRUCT tvInsert;
	tvInsert.item = tvItem;
	tvInsert.hInsertAfter = TVI_LAST;
	tvInsert.hParent = TVI_ROOT;
	HTREEITEM hParentItem = InsertItem(&tvInsert);
	if (!IsCustomRoot())
		pParentFolder->Release();
	OnItemInserted(hParentItem, cFolderPath);
	Expand(hParentItem, TVE_EXPAND);
	return TRUE;
}


void CMFCShellTreeCtrlEx::SetFlagsEx(DWORD dwFlags, BOOL bRefresh)
{
	ASSERT_VALID(this);
	m_dwFlags = dwFlags;
	if (bRefresh && GetSafeHwnd() != NULL)
		RefreshEx();
}

void CMFCShellTreeCtrlEx::SetRootFolder(LPCTSTR szRootDir, BOOL bFullPath, DWORD *pdwProp)
{
	m_bFullRootPath = bFullPath;
	if (szRootDir)
	{
		// Check if szRootDir is not an empty string and points to a valid folder patname
		if (lstrlen(szRootDir) != 0 && _taccess(szRootDir, 0) != 0)
			return;
		// If root folder didn't change => exit
		if (!m_cRootDir.CompareNoCase(szRootDir))
			return;
	}
	// If root folder didn't change => exit
	else if (m_cRootDir.IsEmpty())
		return;
	if (pdwProp)
		m_dwProp = *pdwProp;
	m_cRootDir = szRootDir ? szRootDir : _T("");
	if (m_hWnd)
	{
		// Re-populate tree items
		RefreshEx();
		HTREEITEM hRootItem = GetRootItem();
		if (hRootItem)
			SelectItem(hRootItem);
	}
}

DWORD_PTR CMFCShellTreeCtrlEx::GetItemDataEx(HTREEITEM hItem) const
{
	LPAFX_SHELLITEMINFOEX pItem = hItem ? (LPAFX_SHELLITEMINFOEX)GetItemData(hItem) : NULL;
	return pItem ? pItem->dwItemData : 0;
}


BOOL CMFCShellTreeCtrlEx::SetItemDataEx(HTREEITEM hItem, DWORD_PTR dwData)
{
	LPAFX_SHELLITEMINFOEX pItem = hItem ? (LPAFX_SHELLITEMINFOEX)GetItemData(hItem) : NULL;
	if (!pItem)
		return FALSE;
	pItem->dwItemData = dwData;
	return TRUE;
}


// Enumerating child objects of the folder specified by pParentFolder IShellFolder object
// and searching for the item specified by nIndex parameter.
// (item names are stored in cDirPartArr array)
BOOL CMFCShellTreeCtrlEx::GetFullRootPIDL(LPSHELLFOLDER pParentFolder, CStringArray& cDirPartArr, int nIndex, LPAFX_SHELLITEMINFO pItem)
{
	LPENUMIDLIST pEnum = NULL;
	HRESULT hr = pParentFolder->EnumObjects(NULL, m_dwFlags, &pEnum);
	if (FAILED(hr) || pEnum == NULL)
		return FALSE;
	LPITEMIDLIST pidlTemp;
	DWORD dwFetched = 1;
	CString cDirPart = cDirPartArr[nIndex];
	int nLen = cDirPart.GetLength();
	int nCSIDL = (cDirPart[nLen - 1] == _T('*')) ? _ttoi(cDirPart) : -1;
	BOOL bDrive = (nCSIDL >= 0) ? FALSE : (cDirPart[nLen - 1] == _T('\\'));
	BOOL bFound = FALSE;
	while (SUCCEEDED(pEnum->Next(1, &pidlTemp, &dwFetched)) && dwFetched)
	{
		// If the item is specified by its CSIDL ("Computer")...
		if (nCSIDL >= 0)
		{
			// check if current item is a special folder with the same CSIDL
			LPITEMIDLIST pidl = NULL;
			bFound = SUCCEEDED(SHGetSpecialFolderLocation(m_hWnd, nCSIDL, &pidl));
			if (bFound)
				bFound = (pidlTemp->mkid.cb == pidl->mkid.cb) ? !memcmp(pidlTemp->mkid.abID, pidl->mkid.abID, pidl->mkid.cb) : FALSE;
			afxShellManager->FreeItem(pidl);
		}
		else
		{
			// Otherwise compare item names (use full pathname for a drive folder)
			CString cFolderName = CMFCShellUtils::GetDisplayName(pParentFolder, pidlTemp, bDrive);
			bFound = !cFolderName.CompareNoCase(cDirPart);
		}
		// If item is found
		if (bFound)
		{
			// Use AddRef to create another instance of pParentFolder
			// (the original one will be released)
			pParentFolder->AddRef();
			pItem->pParentFolder = pParentFolder;
			pItem->pidlRel = pidlTemp;
			// concatinate current pidl to the root one
			pItem->pidlFQ = afxShellManager->ConcatenateItem(pItem->pidlFQ, pidlTemp);
			break;
		}
	}
	pParentFolder->Release();
	pEnum->Release();
	return bFound;
}

HRESULT CMFCShellTreeCtrlEx::EnumObjects(HTREEITEM hParentItem, LPSHELLFOLDER pParentFolder, LPITEMIDLIST pidlParent)
{
	ASSERT_VALID(this);
	ASSERT_VALID(afxShellManager);
	LPENUMIDLIST pEnum = NULL;
	HRESULT hr = pParentFolder->EnumObjects(NULL, m_dwFlags, &pEnum);
	if (FAILED(hr) || pEnum == NULL)
	{
		return hr;
	}
	LPITEMIDLIST pidlTemp;
	DWORD dwFetched = 1;
	// Enumerate the item's PIDLs:
	while (SUCCEEDED(pEnum->Next(1, &pidlTemp, &dwFetched)) && dwFetched)
	{
		TVITEM tvItem;
		ZeroMemory(&tvItem, sizeof(tvItem));
		// Fill in the TV_ITEM structure for this item:
		tvItem.mask = TVIF_PARAM | TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_CHILDREN;
		// AddRef the parent folder so it's pointer stays valid:
		pParentFolder->AddRef();
		// Put the private information in the lParam:
		LPAFX_SHELLITEMINFOEX pItem = (LPAFX_SHELLITEMINFOEX)GlobalAlloc(GPTR, sizeof(AFX_SHELLITEMINFOEX));
		ENSURE(pItem != NULL);
		pItem->pidlRel = pidlTemp;
		pItem->pidlFQ = afxShellManager->ConcatenateItem(pidlParent, pidlTemp);
		pItem->pParentFolder = pParentFolder;
		tvItem.lParam = (LPARAM)pItem;
		CString strItem = OnGetItemText((LPAFX_SHELLITEMINFO)pItem);
		tvItem.pszText = strItem.GetBuffer(strItem.GetLength());
		tvItem.iImage = OnGetItemIcon((LPAFX_SHELLITEMINFO)pItem, FALSE);
		tvItem.iSelectedImage = OnGetItemIcon((LPAFX_SHELLITEMINFO)pItem, TRUE);
		// Determine if the item has children:
		DWORD dwAttribs = SFGAO_HASSUBFOLDER | SFGAO_FOLDER | SFGAO_DISPLAYATTRMASK | SFGAO_CANRENAME | SFGAO_FILESYSANCESTOR | SFGAO_REMOVABLE;
		pParentFolder->GetAttributesOf(1, (LPCITEMIDLIST*)&pidlTemp, &dwAttribs);
		// If SHELLTREEEX_QUICK_CHLDDETECT was set ignoring SFGAO_FILESYSANCESTOR mask
		DWORD dwMask = (m_dwProp & SHELLTREEEX_QUICK_CHLDDETECT) != 0 ? SFGAO_HASSUBFOLDER | SFGAO_REMOVABLE : SFGAO_HASSUBFOLDER | SFGAO_FILESYSANCESTOR;
		tvItem.cChildren = (dwAttribs & dwMask) != 0;
		// Determine if the item is shared:
		if (dwAttribs & SFGAO_SHARE)
		{
			tvItem.mask |= TVIF_STATE;
			tvItem.stateMask |= TVIS_OVERLAYMASK;
			tvItem.state |= INDEXTOOVERLAYMASK(1); //1 is the index for the shared overlay image
		}
		// Fill in the TV_INSERTSTRUCT structure for this item:
		TVINSERTSTRUCT tvInsert;
		tvInsert.item = tvItem;
		tvInsert.hInsertAfter = TVI_LAST;
		tvInsert.hParent = hParentItem;
		HTREEITEM hItem = InsertItem(&tvInsert);
		CString cFolderPath = CMFCShellUtils::GetDisplayName(pParentFolder, pItem->pidlRel, TRUE);
		OnItemInserted(hItem, cFolderPath);
		// If SHELLTREEEX_EXPAND_ALL flag was set expand the folder
		if (IsCustomRoot() && tvItem.cChildren && ((m_dwProp & SHELLTREEEX_EXPAND_ALL) != 0))
		{
			Expand(hItem, TVE_EXPAND);
		}
		dwFetched = 0;
	}
	pEnum->Release();
	return S_OK;
}

