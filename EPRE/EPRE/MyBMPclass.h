#pragma once
enum {
	bit24=0,
	bit1=1
};

class MyBMPclass
{
	CFile m_hFile;//File pointer

	BITMAPFILEHEADER BMPHf;    // 비트맵 파일헤드 구조체
	BITMAPINFOHEADER BMPHi;    // 비트맵 영상헤드 구조체	
	RGBQUAD rgb2[2];//추가
	RGBQUAD rgb[256];//추가

	UCHAR* m_inImg;	//input
	UCHAR* m_pucBMP;//gray
	UCHAR* m_bin;	//result
	UINT m_height;
	UINT m_width;
	UINT bmpWidth; //align width
	UINT m_imgSize;

public:
	MyBMPclass(void);
	~MyBMPclass(void);

	BOOL readBMP(IN CString a_fileName, OPTIONAL int flag);
	BOOL createBMP(OUT UCHAR* a_outImg);
	int	binarizationIterative();
	void makeHistogram(OUT float histo[256]);
	BOOL saveBMP(IN CString a_fileName, OPTIONAL int flag);
	BOOL make1bpp();//1bit 

	BOOL runMake1bpp(IN CString a_fileName, IN CString a_newFileName, OPTIONAL int flag);
};

