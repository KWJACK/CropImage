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

	UINT bmpWidth;	//align�� bmp ���� ũ��

	int m_nThreshold;
	BMPclass(UINT m_height, UINT m_width, UCHAR* m_inImg);
	~BMPclass(void);
	int BinarizationIterative();
	void IppHistogram(float histo[256]);
	void bpp1BMP();
	//void adaptiveThreshold(unsigned char* input, unsigned char* bin);	
};
