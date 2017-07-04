// ProjectListCtrl.cpp : implementation file
//
#include "stdafx.h"
#include "Test.h"
#include "ProjectListCtrl.h"

// CProjectListCtrl

IMPLEMENT_DYNAMIC(CProjectListCtrl, CMFCShellListCtrlEx)

CProjectListCtrl::CProjectListCtrl(LISTFILTER nFilter) : m_nFilter(nFilter), m_pProjFileMap(NULL)
{

}

CProjectListCtrl::~CProjectListCtrl()
{
}

BEGIN_MESSAGE_MAP(CProjectListCtrl, CMFCShellListCtrlEx)
END_MESSAGE_MAP()

// CProjectListCtrl message handlers
void CProjectListCtrl::PreEnumObjects(LPCTSTR szFolderPath)
{
	// Saving path to the current folder
	m_cCurFolder = szFolderPath;
}

BOOL CProjectListCtrl::IncludeItem(LPCTSTR szFileName)
{
	// Files filtering function
	if (m_nFilter == LISTFILTER_NONE)
		return FALSE;
	if (m_nFilter == LISTFILTER_ALL || m_pProjFileMap == NULL)
		return TRUE;
	BOOL bResult = (m_nFilter == LISTFILTER_SELECTED) ? FALSE : TRUE;
	CStringArray *pFilesArr = m_pProjFileMap->GetFiles(m_cCurFolder, FALSE);
	if (pFilesArr)
	{
		for (int i = 0; i < pFilesArr->GetSize(); i++)
		{
			CString cFileName = pFilesArr->GetAt(i);
			if (!cFileName.CompareNoCase(szFileName))
			{
				bResult = !bResult;
				break;
			}
		}
	}
	return bResult;
}


