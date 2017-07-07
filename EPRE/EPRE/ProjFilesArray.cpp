#include "stdafx.h"
#include "ProjFilesArray.h"

// This is a CString to CObject map, where CObject is a CStringArray containing selected file names
CProjFilesArray::CProjFilesArray()
{
}

CProjFilesArray::~CProjFilesArray()
{
	for (POSITION pos = GetStartPosition(); pos != NULL;)
	{
		CString cFolderPath;
		CStringArray *pArray = NULL;
		GetNextAssoc(pos, cFolderPath, (CObject*&)pArray);
		// Deleting string array
		if (pArray)
			delete pArray;
	}
}
// Getting the list of selected files (a CStringArray object) corresponding to the folder
// specified by szFolderPath parameters.
// If bAddIfNotFound is TRUE and the map does not contain the required key
// a new entry with an empty string array is added to the map.
CStringArray *CProjFilesArray::GetFiles(LPCTSTR szFolderPath, BOOL bAddIfNotFound)
{
	CStringArray *pArray = NULL;
	BOOL bFound = Lookup(szFolderPath, (CObject*&)pArray);
	if (!bFound && bAddIfNotFound)
	{
		pArray = new CStringArray;
		SetAt(szFolderPath, pArray);
	}
	return pArray;
}
