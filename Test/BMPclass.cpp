#include "stdafx.h"
#include "BMPclass.h"

#define WIDTHBYTES(bits) (((bits)+31)/32*4)
#define T (0.03f)

BMPclass::~BMPclass(void)
{
	if(m_pucBMP){
		delete m_pucBMP;
		m_pucBMP = nullptr;
	}

}

BMPclass::BMPclass(UINT m_height, UINT m_width, UCHAR* m_inImg):
	m_pucBMP(nullptr)	
{	
	int align = 4-(m_width & 0x03);	
	if(align ==4)align=0;

	int bmpWidth = (m_width + 3) / 4 * 4;	

	m_uiHeight = m_height;
	m_uiWidth = m_width;
	
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
	
	//나중에 1bit으로 바꿔야합니다. 현재 8bit
	m_bin = new UCHAR[ih.biSizeImage];
	memset(m_bin, 0, sizeof(UCHAR)*ih.biSizeImage);

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

	//adaptiveThreshold(m_pucBMP, m_bin);
}



void BMPclass::adaptiveThreshold(unsigned char* input, unsigned char* bin)
{
	unsigned long* integralImg = 0;
	int i, j;
	long sum=0;
	int count=0;
	int index;
	int x1, y1, x2, y2;
	int S = (m_uiWidth/8);
	int s2 = S/2;

	// create the integral image
	integralImg = (unsigned long*)malloc(m_uiWidth*m_uiHeight*sizeof(unsigned long*));

	for (i=0; i<m_uiWidth; i++)
	{
		// reset this column sum
		sum = 0;
		for (j=0; j<m_uiHeight; j++)
		{
			index = j*m_uiWidth+i;
			sum += input[index];
			if (i==0)
				integralImg[index] = sum;
			else
				integralImg[index] = integralImg[index-1] + sum;
		}
	}

	// perform thresholding
	for (i=0; i<m_uiWidth; i++)
	{
		for (j=0; j<m_uiHeight; j++)
		{
			index = j*m_uiWidth+i;
			// set the SxS region
			x1=i-s2; x2=i+s2;
			y1=j-s2; y2=j+s2;

			// check the border
			if (x1 < 0) x1 = 0;
			if (x2 >= m_uiWidth) x2 = m_uiWidth-1;
			if (y1 < 0) y1 = 0;
			if (y2 >= m_uiHeight) y2 = m_uiHeight-1;
			
			count = (x2-x1)*(y2-y1);
			// I(x,y)=s(x2,y2)-s(x1,y2)-s(x2,y1)+s(x1,x1)
			sum = integralImg[y2*m_uiWidth+x2] -
				  integralImg[y1*m_uiWidth+x2] -
				  integralImg[y2*m_uiWidth+x1] +
				  integralImg[y1*m_uiWidth+x1];
			if ((long)(input[index]*count) < (long)(sum*(1.0-T)))
				bin[index] = 0;
			else
				bin[index] = 255;
		}
	}

	free (integralImg);

	//복사( 수정 필요)
	for(int i=0; i<ih.biSizeImage; i++)
	{	
		m_pucBMP[i]= bin[i];
	}

}



