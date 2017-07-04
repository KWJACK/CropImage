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
	BITMAPFILEHEADER m_BMPHf;    // ��Ʈ�� ������� ����ü
	BITMAPINFOHEADER m_BMPHi;    // ��Ʈ�� ������� ����ü

	BMPclass* c_outBMP;
	

	BOOL ReadBMP(CString a_fileName);	
	BOOL SaveBMP(CString a_fileName);
};

