#pragma once
class BMPclass
{
public:
	BMPclass(UINT m_height, UINT m_width, UCHAR* m_inImg);
	~BMPclass(void);

	BITMAPFILEHEADER fh;
	BITMAPINFOHEADER ih;
	RGBQUAD rgb[256];

	UCHAR*	m_pucBMP;	
	UINT m_uiWidth;
	UINT m_uiHeight;
};
