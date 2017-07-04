#include "stdafx.h"
#include "BMPclass.h"

#define WIDTHBYTES(bits) (((bits)+31)/32*4)

BMPclass::BMPclass(UINT m_height, UINT m_width, UCHAR* m_inImg):
	m_pucBMP(nullptr)	
{	
	int align = 4-(m_width & 0x03);	
	if(align ==4)align=0;

	int bmpWidth = (m_width + 3) / 4 * 4;	
	
	memset(&fh, 0, sizeof(BITMAPFILEHEADER));
	memset(&ih, 0, sizeof(BITMAPINFOHEADER));
	memset(&rgb, 0, sizeof(RGBQUAD) * 256);		

	fh.bfOffBits = 1078; // RGBQUAD + InfoHeader + FileHeader only 8bit mode if 24bit == 54; 40+ 14; 
	fh.bfSize = m_height * m_width + sizeof(RGBQUAD) * 256 + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	fh.bfType = 19778;

	ih.biBitCount = 8;
	ih.biHeight = m_height;
	ih.biWidth = m_width;
	ih.biPlanes = 1;
	ih.biSize = 40;
	ih.biSizeImage = bmpWidth * m_height;	
	ih.biXPelsPerMeter = 0;
	ih.biYPelsPerMeter = 0;

	for(int i = 0; i < 256; i++)
	{
		rgb[i].rgbBlue = i;
		rgb[i].rgbGreen = i;
		rgb[i].rgbRed = i;
		rgb[i].rgbReserved = 0;
	}
   
   //이미지 데이터 복사	
	m_pucBMP = new UCHAR[ih.biSizeImage];
	memset(m_pucBMP, 0, sizeof(UCHAR)*ih.biSizeImage);
	int index = 0, oldindex=0;
	UINT R,G,B,GRAY;	
	for(int i=0; i<m_height; i++)
	{	
		for(int j=0; j<bmpWidth; j++)
		{		
			B = m_inImg[index++];
			G = m_inImg[index++];
			R = m_inImg[index++];
			GRAY = (UCHAR)(0.299*R + 0.587*G + 0.114*B);
			m_pucBMP[oldindex++] = GRAY;
		}	
	}
}


BMPclass::~BMPclass(void)
{
	if(m_pucBMP) delete m_pucBMP;

}