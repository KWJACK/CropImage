#include "stdafx.h"
#include "MFCShellUtils.h"

// Developed by Vladimir Misovsky
// January 2016

CMFCShellUtils::CMFCShellUtils()
{
}


CMFCShellUtils::~CMFCShellUtils()
{
}

CString CMFCShellUtils::GetDisplayName(IShellFolder *pshf, LPITEMIDLIST pidl, BOOL bFullPath)
{
	LPMALLOC lpMalloc = NULL;
	if (::SHGetMalloc(&lpMalloc) != NOERROR)
		return CString(_T(""));
	DWORD dwFlags = bFullPath ? SHGDN_FORPARSING : SHGDN_NORMAL;
	STRRET sStrRet;
	pshf->GetDisplayNameOf(pidl, dwFlags, &sStrRet);
	CString cName((char *)sStrRet.cStr);
	if (sStrRet.uType == STRRET_OFFSET)
	{
		LPCTSTR szBuffer = (LPCTSTR)&((LPCTSTR)pidl)[sStrRet.uOffset];
		cName = szBuffer;
	}
	else if (sStrRet.uType == STRRET_WSTR)
	{
		char szName[MAX_PATH + 1];
		::WideCharToMultiByte(CP_ACP, (DWORD)0, (LPCWSTR)sStrRet.pOleStr, -1,
			szName, MAX_PATH, NULL, NULL);
		cName = szName;
		lpMalloc->Free(sStrRet.pOleStr);
	}
	if (bFullPath && cName.IsEmpty())
		cName = GetDisplayName(pshf, pidl, FALSE);
	if (lpMalloc)
		lpMalloc->Release();
	return cName;
}
