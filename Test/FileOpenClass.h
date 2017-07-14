#pragma once

#include "BMPclass.h"
class FileOpenClass
{
private:
public:
	FileOpenClass(void);
	~FileOpenClass(void);
	BOOL ReadBMP(CString a_fileName);	
	BOOL SaveBMP(CString a_fileName);
	BOOL PaletteChange(CString a_path, CString a_fileName);
	CFile m_hFile;	
	UCHAR* m_inImg;	

	UINT m_height;
	UINT m_width;
	BMPclass* c_outBMP;

	BITMAPFILEHEADER BMPHf;    // 비트맵 파일헤드 구조체
	BITMAPINFOHEADER BMPHi;    // 비트맵 영상헤드 구조체	
};

