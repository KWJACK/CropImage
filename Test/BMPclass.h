#pragma once
class BMPclass
{
public:
	BITMAPFILEHEADER fh;
	BITMAPINFOHEADER ih;
	RGBQUAD rgb[256];
	RGBQUAD rgb2[2];

	UCHAR*	m_pucBMP;	
	UCHAR*	m_bin;	//���߿� �ٲ���մϴ�. (8bit �ε� 1bit����)
	UINT	m_uiWidth;	//���Ͽ� ǥ�õǴ� ����, align ���̴� bmpWidth
	UINT	m_uiHeight;

	UINT bmpWidth;	//align�� bmp ���� ũ��

	int m_nThreshold;
	BMPclass(IN UINT m_height, IN UINT m_width, IN UCHAR* m_inImg);
	~BMPclass(void);
	int BinarizationIterative();
	void IppHistogram(OUT float histo[256]);
	void bpp1BMP();
	//void adaptiveThreshold(unsigned char* input, unsigned char* bin);	
};
