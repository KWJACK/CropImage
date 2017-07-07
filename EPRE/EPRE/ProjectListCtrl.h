#pragma once
#include "MFCShellListCtrlEx.h"
#include "ProjFilesArray.h"

// CProjectListCtrl
enum LISTFILTER
{
	LISTFILTER_ALL,
	LISTFILTER_SELECTED,
	LISTFILTER_REMAINING,
	LISTFILTER_NONE
};

class CProjectListCtrl : public CMFCShellListCtrlEx
{
	DECLARE_DYNAMIC(CProjectListCtrl)

public:
	CProjectListCtrl(LISTFILTER nFilter = LISTFILTER_NONE);
	virtual ~CProjectListCtrl();

	void SetFilter(LISTFILTER nFilter) { m_nFilter = nFilter; }
	void SetProjectFiles(CProjFilesArray *pProjFileMap) { m_pProjFileMap = pProjFileMap; }

protected:
	DECLARE_MESSAGE_MAP()

	LISTFILTER m_nFilter;
	CString m_cCurFolder;
	CProjFilesArray *m_pProjFileMap;
	void PreEnumObjects(LPCTSTR szFolderPath);
	BOOL IncludeItem(LPCTSTR szFileName);
};


