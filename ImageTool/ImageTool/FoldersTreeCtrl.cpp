// FoldersTreeCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "FoldersTreeCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFoldersTreeCtrl
CFoldersTreeCtrl::CFoldersTreeCtrl()
{
}

CFoldersTreeCtrl::~CFoldersTreeCtrl()
{
}


BEGIN_MESSAGE_MAP(CFoldersTreeCtrl, CTreeCtrl)
	//{{AFX_MSG_MAP(CFoldersTreeCtrl)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFoldersTreeCtrl message handlers
LPITEMIDLIST CFoldersTreeCtrl::Next(LPCITEMIDLIST pidl)
{
	LPSTR lpMem = ( LPSTR )pidl;
	
	lpMem += pidl->mkid.cb;
	
	return ( LPITEMIDLIST )lpMem;
}

UINT CFoldersTreeCtrl::GetSize(LPCITEMIDLIST pidl)
{
    UINT cbTotal = 0;

    if( pidl )
    {
        cbTotal += sizeof( pidl->mkid.cb );
    
		while( pidl->mkid.cb )
        {
            cbTotal += pidl->mkid.cb;
            pidl    = Next( pidl );
        }
    }
	
    return cbTotal;
}

LPITEMIDLIST CFoldersTreeCtrl::CreatePidl( UINT cbSize )
{
    LPMALLOC     lpMalloc;
    HRESULT      hr;
    LPITEMIDLIST pidl=NULL;
	
    hr = SHGetMalloc(&lpMalloc);
	
    if( FAILED(hr) )
	{
		return 0;
	}
	
    pidl = (LPITEMIDLIST)lpMalloc->Alloc(cbSize);
	
    if( pidl )
	{
        memset(pidl, 0, cbSize);   
	}
	
    if( lpMalloc )
	{
		lpMalloc->Release();
	}
	
    return pidl;
}

LPITEMIDLIST CFoldersTreeCtrl::ConcatPidls(LPCITEMIDLIST pidl1, LPCITEMIDLIST pidl2)
{
    LPITEMIDLIST pidlNew;
    UINT         cb1;
    UINT         cb2;
	
    if( pidl1 ) 
	{
		cb1 = GetSize(pidl1) - sizeof(pidl1->mkid.cb);
	}
    else
	{
		cb1 = 0;
	}
	
    cb2 = GetSize(pidl2);
	
    pidlNew = CreatePidl(cb1 + cb2);
    
	if( pidlNew )
    {
        if( pidl1 )
		{
			memcpy(pidlNew, pidl1, cb1);
		}

        memcpy(((LPSTR)pidlNew) + cb1, pidl2, cb2);
    }
    return pidlNew;
}

LPITEMIDLIST CFoldersTreeCtrl::CopyITEMID(LPMALLOC lpMalloc, LPITEMIDLIST lpi)
{
	LPITEMIDLIST lpiTemp;
	lpiTemp = (LPITEMIDLIST)lpMalloc->Alloc(lpi->mkid.cb+sizeof(lpi->mkid.cb));
	CopyMemory((PVOID)lpiTemp, (CONST VOID *)lpi, lpi->mkid.cb+sizeof(lpi->mkid.cb));
	
	return lpiTemp;
}

BOOL CFoldersTreeCtrl::GetName( LPSHELLFOLDER lpsf,
						        LPITEMIDLIST  lpi,
								DWORD         dwFlags,
								LPSTR         lpFriendlyName )
{
	BOOL   bSuccess=TRUE;
	STRRET str;
	
	if( NOERROR == lpsf->GetDisplayNameOf( lpi,dwFlags, &str ) )
	{
		switch( str.uType )
		{
		case STRRET_WSTR:
            WideCharToMultiByte( CP_ACP, 
				                 0,		               
								 str.pOleStr,          
								 -1,                   
								 lpFriendlyName,       
								 MAX_PATH,
								 NULL,                   
								 NULL );                  
			break;
		case STRRET_OFFSET:
			lstrcpy( lpFriendlyName, (LPSTR)lpi+str.uOffset );
            break;
		case STRRET_CSTR:
			lstrcpy( lpFriendlyName, (LPSTR)str.cStr );
			break;
		default:
			bSuccess = FALSE;
			break;
		}
	}
	else
	{
		bSuccess = FALSE;
	}
	
	return bSuccess;
}

int CFoldersTreeCtrl::GetItemIcon(LPITEMIDLIST lpi, UINT uFlags)
{
	SHFILEINFO sfi;
	SHGetFileInfo( (LPCSTR)lpi, 0, &sfi, sizeof(SHFILEINFO), uFlags );
	
	return sfi.iIcon;
}

void CFoldersTreeCtrl::PopulateTree() 
{
    LPSHELLFOLDER lpsf = NULL;
    LPITEMIDLIST  lpi  = NULL;
    HRESULT       hr   = NULL;
	HTREEITEM	  hItem;
    TV_SORTCB     tvscb;
	
    hr = SHGetDesktopFolder( &lpsf );
	
    if( SUCCEEDED(hr) )
    {
		DeleteAllItems();
		
		FillTreeView(lpsf, NULL, TVI_ROOT);
		lpsf->Release();
    }
    tvscb.hParent     = TVI_ROOT;
    tvscb.lParam      = 0;
    tvscb.lpfnCompare = TreeViewCompareProc;
	
	SortChildrenCB( &tvscb/*, FALSE*/ );
    
	hItem = GetRootItem();
	Expand(hItem,TVE_EXPAND);
	Select(GetRootItem(),TVGN_CARET);
}

void CFoldersTreeCtrl::FillTreeView( LPSHELLFOLDER lpsf,
							         LPITEMIDLIST  lpifq,
									 HTREEITEM     hParent )
{
    TV_ITEM         tvi;                          
    TV_INSERTSTRUCT tvins;                       
    HTREEITEM       hPrev = NULL;                 
    LPSHELLFOLDER   lpsf2=NULL;
    LPENUMIDLIST    lpe=NULL;
    LPITEMIDLIST    lpi=NULL, lpiTemp=NULL, lpifqThisItem=NULL;
    LPTVITEMDATA    lptvid=NULL;
    LPMALLOC        lpMalloc=NULL;
    ULONG           ulFetched;
    UINT            uCount=0;
    HRESULT         hr;
    char            szBuff[256];
    HWND            hwnd=::GetParent(m_hWnd);
	
    hr=::SHGetMalloc(&lpMalloc);

    if( FAILED(hr) )
	{
		return;
	}
	
    if( SUCCEEDED(hr) )
    {
        hr=lpsf->EnumObjects(hwnd, SHCONTF_FOLDERS | SHCONTF_NONFOLDERS, &lpe);
		
        if( SUCCEEDED(hr) )
        {
            while (S_OK==lpe->Next(1, &lpi, &ulFetched))
            {
                ULONG ulAttrs = SFGAO_HASSUBFOLDER | SFGAO_FOLDER;
				
                lpsf->GetAttributesOf(1, (const struct _ITEMIDLIST **)&lpi, &ulAttrs);
				
                if( ulAttrs & (SFGAO_HASSUBFOLDER | SFGAO_FOLDER) )
                {
					if (ulAttrs & SFGAO_FOLDER)
					{
						tvi.mask= TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
						
						if (ulAttrs & SFGAO_HASSUBFOLDER)
						{
							tvi.cChildren=1;
							tvi.mask |= TVIF_CHILDREN;
						}
                        
						lptvid = (LPTVITEMDATA)lpMalloc->Alloc(sizeof(TVITEMDATA));
				
						if (!lptvid) goto Cleanup;
						if (!GetName(lpsf, lpi, SHGDN_NORMAL, szBuff)) goto Cleanup;
						
						tvi.pszText    = szBuff;
						tvi.cchTextMax = MAX_PATH;
						
						lpifqThisItem=ConcatPidls(lpifq, lpi);
						
						lptvid->lpi=CopyITEMID(lpMalloc, lpi);
						
						tvi.iImage = GetItemIcon(lpifqThisItem, SHGFI_PIDL | SHGFI_SYSICONINDEX | SHGFI_SMALLICON);
						tvi.iSelectedImage = GetItemIcon(lpifqThisItem, SHGFI_PIDL | SHGFI_SYSICONINDEX | SHGFI_SMALLICON | SHGFI_OPENICON);
						
						lptvid->lpsfParent=lpsf;
						lpsf->AddRef();
						
						lptvid->lpifq=ConcatPidls(lpifq, lpi);
						
						tvi.lParam = (LPARAM)lptvid;
						
						tvins.item         = tvi;
						tvins.hInsertAfter = hPrev;
						tvins.hParent      = hParent;
						
						hPrev = InsertItem(&tvins);
					}

					lpMalloc->Free(lpifqThisItem);  
					lpifqThisItem=0;
                }
				
                lpMalloc->Free(lpi);
                lpi=0;
            }
        }
	}
    else
	{
		return;
	}
	
Cleanup:
	if (lpe) lpe->Release();
    if (lpi && lpMalloc) lpMalloc->Free(lpi);
    if (lpifqThisItem && lpMalloc) lpMalloc->Free(lpifqThisItem);  
    if (lpMalloc) lpMalloc->Release();
}

int CALLBACK CFoldersTreeCtrl::TreeViewCompareProc( LPARAM lparam1, 
											        LPARAM lparam2,
													LPARAM lparamSort )
{
    LPTVITEMDATA lptvid1=(LPTVITEMDATA)lparam1;
    LPTVITEMDATA lptvid2=(LPTVITEMDATA)lparam2;
    HRESULT   hr;
	
    hr = lptvid1->lpsfParent->CompareIDs(0,lptvid1->lpi,lptvid2->lpi);
	
    if( FAILED(hr) ) return 0;
	
    return (short)SCODE_CODE(GetScode(hr));
}

void CFoldersTreeCtrl::OnFolderExpanding(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LPTVITEMDATA   lptvid; 
	HRESULT        hr;
	LPSHELLFOLDER  lpsf2=NULL;
	static char    szBuff[MAX_PATH];
	TV_SORTCB      tvscb;
	
	NM_TREEVIEW* pnmtv = (NM_TREEVIEW*)pNMHDR;
	
    if ((pnmtv->itemNew.state & TVIS_EXPANDEDONCE))
		return;
	
    lptvid=(LPTVITEMDATA)pnmtv->itemNew.lParam;

    if( lptvid )
	{
		hr=lptvid->lpsfParent->BindToObject( lptvid->lpi,
			                                 0,
											 IID_IShellFolder,
											 (LPVOID *)&lpsf2 );
		
		if( SUCCEEDED(hr) )
		{
			FillTreeView( lpsf2, lptvid->lpifq, pnmtv->itemNew.hItem );
		}
		
		tvscb.hParent     = pnmtv->itemNew.hItem;
		tvscb.lParam      = 0;
		tvscb.lpfnCompare = TreeViewCompareProc;
		
		SortChildrenCB(&tvscb /*, FALSE*/);
    }	
	
	*pResult = 0;
}

BOOL CFoldersTreeCtrl::OnFolderSelected(NMHDR* pNMHDR, LRESULT* pResult, CString &szFolderPath) 
{
	LPTVITEMDATA	lptvid; 
	LPSHELLFOLDER	lpsf2=NULL;
	static char		szBuff[MAX_PATH];
	HRESULT			hr;
	BOOL			bRet = FALSE;
	TV_SORTCB		tvscb;
	HTREEITEM		hItem=NULL;
	
	if((hItem = GetSelectedItem()))
	{
		lptvid=(LPTVITEMDATA)GetItemData(hItem);
		
		if (lptvid && lptvid->lpsfParent && lptvid->lpi)
		{
			hr=lptvid->lpsfParent->BindToObject(lptvid->lpi,
				0,IID_IShellFolder,(LPVOID *)&lpsf2);
			
			if (SUCCEEDED(hr))
			{
				ULONG ulAttrs = SFGAO_FILESYSTEM;
				
				lptvid->lpsfParent->GetAttributesOf(1, (const struct _ITEMIDLIST **)&lptvid->lpi, &ulAttrs);
				
				if (ulAttrs & (SFGAO_FILESYSTEM))
				{
					if(SHGetPathFromIDList(lptvid->lpifq,szBuff))
					{
						szFolderPath = szBuff;
						bRet = TRUE;
					}
				}
				
				NM_TREEVIEW* pnmtv = (NM_TREEVIEW*)pNMHDR;
				
				if ((pnmtv->itemNew.cChildren == 1) && !(pnmtv->itemNew.state & TVIS_EXPANDEDONCE)){
					FillTreeView(lpsf2,lptvid->lpifq,pnmtv->itemNew.hItem);
					
					tvscb.hParent     = pnmtv->itemNew.hItem;
					tvscb.lParam      = 0;
					tvscb.lpfnCompare = TreeViewCompareProc;
					SortChildrenCB(&tvscb);
					
					pnmtv->itemNew.state |= TVIS_EXPANDEDONCE;
					pnmtv->itemNew.stateMask |= TVIS_EXPANDEDONCE;
					pnmtv->itemNew.mask |= TVIF_STATE;
					SetItem(&pnmtv->itemNew);
				}
			}
			
		}
		if( lpsf2 ) lpsf2->Release();
		
	}	

	*pResult = 0;
	
	return bRet;
}

void CFoldersTreeCtrl::EnableImages()
{
    HIMAGELIST  hImageList;
    SHFILEINFO  sfi;
	
    hImageList = (HIMAGELIST)SHGetFileInfo( (LPCSTR)"C:\\", 
		                                    0,
											&sfi, 
											sizeof(SHFILEINFO), 
											SHGFI_SYSICONINDEX | SHGFI_SMALLICON );
	
    if( hImageList )
	{
        ::SendMessage( m_hWnd,TVM_SETIMAGELIST, (WPARAM) TVSIL_NORMAL, (LPARAM)hImageList );
	}
}
