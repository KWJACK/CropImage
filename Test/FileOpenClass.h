#pragma once

#include "BMPclass.h"
class FileOpenClass
{
private:
	BOOL ReadBMP(CString a_fileName);	
	BOOL SaveBMP(CString a_fileName);
public:
	FileOpenClass(void);
	~FileOpenClass(void);

	CFile m_hFile;	
	UCHAR* m_inImg;	

	UINT m_height;
	UINT m_width;
	BMPclass* c_outBMP;
	
};

