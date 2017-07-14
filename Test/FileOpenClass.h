#pragma once

#include "BMPclass.h"
class FileOpenClass
{
private:
public:
	FileOpenClass(void);
	~FileOpenClass(void);
	BOOL ReadBMP(IN CString a_fileName);	
	BOOL SaveBMP(IN CString a_fileName);
	BOOL PaletteChange(IN CString a_path, IN CString a_fileName);
	CFile m_hFile;	
	UCHAR* m_inImg;	

	UINT m_height;
	UINT m_width;
	BMPclass* c_outBMP;

	BITMAPFILEHEADER BMPHf;    // ��Ʈ�� ������� ����ü
	BITMAPINFOHEADER BMPHi;    // ��Ʈ�� ������� ����ü	
};

