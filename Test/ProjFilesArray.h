#pragma once
#include "afxcoll.h"

class CProjFilesArray : public CMapStringToOb
{
public:
	CProjFilesArray();
	~CProjFilesArray();

	CStringArray *GetFiles(LPCTSTR szFolderPath, BOOL bAddIfNotFound = FALSE);
};