#pragma once

#include "BMPclass.h"
class FileOpenClass
{
public:
	FileOpenClass(void);
	~FileOpenClass(void);
	CFile m_hFile;
	int m_imgSize;
	UCHAR* m_inImg;	

	UINT m_height;
	UINT m_width;
	BITMAPFILEHEADER m_BMPHf;    // 비트맵 파일헤드 구조체
	BITMAPINFOHEADER m_BMPHi;    // 비트맵 영상헤드 구조체

	BMPclass* c_outBMP;
	

	BOOL ReadBMP(CString a_fileName);	
	BOOL SaveBMP(CString a_fileName);
};

