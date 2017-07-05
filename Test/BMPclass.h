#pragma once
class BMPclass
{
public:
	BITMAPFILEHEADER fh;
	BITMAPINFOHEADER ih;
	RGBQUAD rgb[256];

	UCHAR*	m_pucBMP;	
	UCHAR*	m_bin;	//나중에 바꿔야합니다. (8bit 인데 1bit으로)
	UINT	m_uiWidth;	//파일에 표시되는 길이, align 길이는 bmpWidth
	UINT	m_uiHeight;

	BMPclass(UINT m_height, UINT m_width, UCHAR* m_inImg);
	~BMPclass(void);

	void adaptiveThreshold(unsigned char* input, unsigned char* bin);	
};
