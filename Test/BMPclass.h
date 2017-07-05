#pragma once
class BMPclass
{
public:
	BITMAPFILEHEADER fh;
	BITMAPINFOHEADER ih;
	RGBQUAD rgb[256];

	UCHAR*	m_pucBMP;	
	UCHAR*	m_bin;	//���߿� �ٲ���մϴ�. (8bit �ε� 1bit����)
	UINT	m_uiWidth;	//���Ͽ� ǥ�õǴ� ����, align ���̴� bmpWidth
	UINT	m_uiHeight;

	BMPclass(UINT m_height, UINT m_width, UCHAR* m_inImg);
	~BMPclass(void);

	void adaptiveThreshold(unsigned char* input, unsigned char* bin);	
};
