// MFCShellListCtrlEx.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "EPRE.h"
#include "MFCShellListCtrlEx.h"
#include "MFCShellUtils.h"
#include "resource.h"
#define filepathtopreview WM_USER+2
// Developed by Vladimir Misovsky
// January 2016
IMPLEMENT_DYNAMIC(CMFCShellListCtrlEx, CMFCShellListCtrl)
CMFCShellListCtrlEx::CMFCShellListCtrlEx()
{
	m_preViewDlg=nullptr;
	m_nItem=-1;//리스트 아이템 선택 초기화
}

CMFCShellListCtrlEx::~CMFCShellListCtrlEx()
{
}

BEGIN_MESSAGE_MAP(CMFCShellListCtrlEx, CMFCShellListCtrl)	
	ON_NOTIFY_REFLECT(NM_CLICK, &CMFCShellListCtrlEx::OnNMClick)
	ON_NOTIFY_REFLECT(NM_DBLCLK, &CMFCShellListCtrlEx::OnNMDblclk)	
	ON_NOTIFY_REFLECT(LVN_DELETEITEM, &CMFCShellListCtrlEx::OnDeleteitem)	
	ON_NOTIFY_REFLECT(LVN_KEYDOWN, &CMFCShellListCtrlEx::OnLvnKeydown)	
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

//리스트 아이템클릭시 해당 파일의 이름을 얻어옴
void CMFCShellListCtrlEx::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_nItem = pNMItemActivate->iItem;
	//int m_nSubItem = pNMItemActivate->iSubItem;//iItem= row, iSubItem= colum
	//CString cStrText = GetItemText(m_nItem, m_nSubItem); //선택 list의 파일 이름 얻음
	if(m_nItem==-1){
		return;
	}	
	//선택 아이템 경로 얻기		
	if(!GetItemPath(m_selFilePath, m_nItem)){
		AfxMessageBox(L"해당 파일의 경로를 찾을 수 없습니다.");
		return;
	}
	//preview에 전달
	if(m_preViewDlg){				
		if(m_selFilePath.Find(L".jpg") > 0 ){//jpg, bmp 파일만 읽어들임		
			m_preViewDlg->m_pSelectedImage  = Bitmap::FromFile(m_selFilePath.AllocSysString());		
			::SendMessage(m_preViewDlg->m_hWnd, WM_SIZE, 0, 0);	
			::PostMessage(m_preViewDlg->m_hWnd, LM_SEL_PATH_NAME, 0, LPARAM(&m_selFilePath));
		}
		else if( m_selFilePath.Find(L".bmp") > 0){
			m_preViewDlg->m_pSelectedImage  = Bitmap::FromFile(m_selFilePath.AllocSysString());		
			::SendMessage(m_preViewDlg->m_hWnd, WM_SIZE, 0,0);	
		}
		else if( m_selFilePath.Find(L".PNG") > 0){
			m_preViewDlg->m_pSelectedImage  = Bitmap::FromFile(m_selFilePath.AllocSysString());		
			::SendMessage(m_preViewDlg->m_hWnd, WM_SIZE, 0,0);	
		}
		//m_preViewDlg->m_filePath = m_selFilePath;
		
	}	
	*pResult = 0;
}


void CMFCShellListCtrlEx::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	//더블클릭 시 파일이 오픈되는데 막아버림
	//LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//*pResult = 0;
}



void CMFCShellListCtrlEx::OnLvnKeydown(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);
	int i = m_nItem;
	switch (pLVKeyDow->wVKey)
	{
	case VK_DOWN:		
		if(GetItemCount() > m_nItem+1)
			i = m_nItem + 1;		
		break;
	case VK_UP:	
		if (m_nItem != 0) i = m_nItem - 1;break;
	default:return;
	}
	//선택 아이템 경로 얻기	
	

	if (!GetItemPath(m_selFilePath, i)) {
		AfxMessageBox(L"해당 파일의 경로를 찾을 수 없습니다.");
		return;
	}
	//preview에 전달
	if (m_preViewDlg) {
		if (m_selFilePath.Find(L".jpg") > 0) {//jpg, bmp 파일만 읽어들임		
			m_preViewDlg->m_pSelectedImage = Bitmap::FromFile(m_selFilePath.AllocSysString());
			::SendMessage(m_preViewDlg->m_hWnd, WM_SIZE, 0, 0);
		}
		else if (m_selFilePath.Find(L".bmp") > 0) {
			m_preViewDlg->m_pSelectedImage = Bitmap::FromFile(m_selFilePath.AllocSysString());
			::SendMessage(m_preViewDlg->m_hWnd, WM_SIZE, 0, 0);
		}
	}
	switch (pLVKeyDow->wVKey)
	{
	case VK_DOWN:
		if (GetItemCount() > m_nItem+1)m_nItem++;
		break;
	case VK_UP:
		if(m_nItem!=0) m_nItem--;
		break;
	default:return;
	}
}

