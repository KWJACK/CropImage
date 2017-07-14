#include "stdafx.h"
#include "BMPclass.h"

#define WIDTHBYTES(bits) (((bits)+31)/32*4)
//#define T (0.03f)
//0.05이면 흐릿한 글자가 많이 출력됨
BMPclass::~BMPclass(void)
{
	if(m_pucBMP){
		delete m_pucBMP;
		m_pucBMP = nullptr;
	}
	if(m_bin){
		delete m_bin;
		m_bin = nullptr;
	}

}


BMPclass::BMPclass(IN UINT m_height, IN UINT m_width, IN UCHAR* m_inImg):
	m_pucBMP(nullptr), m_bin(nullptr)
{	
	//int align = 4-(m_width & 0x03);	
	//if(align ==4)align=0;

	//int bmpWidth = (m_width + 3) / 4 * 4;	//new ver
	//int align = bmpWidth - m_width;
	bmpWidth = (m_width * 3 + 3) & ~3; //original ver
	int align = bmpWidth - (m_width*3);
	int Bialign = align;
	if(align ==1 || align==3) Bialign = 4 - align;

	bmpWidth = m_width + Bialign;
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

	int index = 0, oldindex=0;
	UINT R,G,B,GRAY;	
	for(int i=0; i<m_height; i++)
	{	
		for(int j=0; j<m_uiWidth; j++)
		{		
			B = m_inImg[index++];
			G = m_inImg[index++];
			R = m_inImg[index++];
			GRAY = (UCHAR)(0.299*R + 0.587*G + 0.114*B);
			m_pucBMP[oldindex++] = GRAY;
		}				
		for(int j=0;j<align;j++){
			index++;		
		}
		for(int j=0;j<Bialign;j++){			
			m_pucBMP[oldindex++] = 255;//빈 여백은 흰색으로 채움. 나중에 이진화할 때 0을 검은색으로 보기때문
		}
	}	
	//히스토그램을 통해 임계값을 구함
	m_nThreshold = BinarizationIterative();

	for (int i = 0; i < m_uiHeight*bmpWidth; i++)
	{
		m_pucBMP[i] = (m_pucBMP[i] <= m_nThreshold) ? 0 : 255;
	}
	
	//adaptiveThreshold(m_pucBMP, m_bin);
}

int BMPclass::BinarizationIterative()
{
	float hist[256] = { 0, };
	IppHistogram(hist);
	//초기 임계값 설정 - 그레이 스케일 값의 전체 평균
	int i, T, Told;
	float sum = 0.f;
	for (i = 0; i < 256; i++) {
		sum += (i*hist[i]);
	}
	T = static_cast<int>(sum + .5f);
	//반복에 의한 임계값 결정
	float a1, b1, u1, a2, b2, u2;
	do {
		Told = T;

		a1 = b1 = u1 = 0.f;
		for (i = 0; i <= Told; i++)
		{
			a1 += (i*hist[i]);
			b1 += hist[i];
		}

		if (b1 != 0.f)
			u1 = a1 / b1;

		a2 = b2 = u2 = 0.f;
		for (i = Told + 1; i < 256; i++)
		{
			a2 += (i*hist[i]);
			b2 += hist[i];
		}

		if (b2 != 0.f)
			u2 = a2 / b2;

		T = static_cast<int>((u1 + u2) / 2 + 0.5f);
	} while (T != Told);
	return T;	
}

void BMPclass::IppHistogram(OUT float histo[256])
{
	//히스토그램 계산
	int size = m_uiWidth * m_uiHeight;		
	int cnt[256] = { 0, };//256은 gray 비트수준
	int Index=0;
	for (int i = 0; i < m_uiHeight; i++){
		for(int j=0;j<m_uiWidth;j++)
			cnt[m_pucBMP[Index++]]++;
		for(int j=0;j<bmpWidth-m_uiWidth;j++)
			Index++;
	}
	//히스토그램 정규화
	for (int i = 0; i < 256; i++)
	{
		histo[i] = static_cast<float>(cnt[i]) / size;
	}
	
}

void BMPclass::bpp1BMP()
{
 	int widthBitAlign = (m_uiWidth + 7) & 0xFFFFFFF8; 
	int Bialign = widthBitAlign-m_uiWidth;
	int bbmpWidth = (widthBitAlign/8 + 3) & ~3; //original ver
	int align = bbmpWidth - widthBitAlign/8;//align = 0,1,2,3
	

	memset(&fh, 0, sizeof(BITMAPFILEHEADER));
	memset(&ih, 0, sizeof(BITMAPINFOHEADER));
	memset(&rgb2, 0, sizeof(RGBQUAD) * 2);

	fh.bfOffBits = 62; // RGBQUAD(8) + InfoHeader(40) + FileHeader(14) only 8bit mode if 24bit == 54; 40+ 14; 
	fh.bfSize = m_uiHeight*bbmpWidth + sizeof(RGBQUAD) * 2 + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	fh.bfType = 19778;

	ih.biBitCount = 1;
	ih.biHeight = m_uiHeight;
	ih.biWidth = m_uiWidth;
	ih.biPlanes = 1;
	ih.biSize = 40;//bmp info헤더 크기(40)
	ih.biSizeImage = bbmpWidth*m_uiHeight;
	ih.biXPelsPerMeter = 0;
	ih.biYPelsPerMeter = 0;
		
	rgb2[0].rgbBlue = 255;//흰색
	rgb2[0].rgbGreen = 255;
	rgb2[0].rgbRed = 255;
	rgb2[0].rgbReserved = 255;

	rgb2[1].rgbBlue = 0;//검은색
	rgb2[1].rgbGreen = 0;
	rgb2[1].rgbRed = 0;
	rgb2[1].rgbReserved = 255;
	
	m_bin = new UCHAR[ih.biSizeImage];
	memset(m_bin, 0, sizeof(UCHAR)*ih.biSizeImage);

	int index = 0, oldindex = 0, count=0;	
	for (int i = 0; i<m_uiHeight; i++)
	{		
		for (int j = 0; j< bmpWidth; j++)//8bit의 alignㄷ
		{		
			count++;
			m_bin[oldindex] = m_bin[oldindex]<<1;
			//흑이면 두번 째 파레트 색 추가. 디폴트는 첫번째 파레트 색
			if(m_pucBMP[index++]==0){
				m_bin[oldindex] |= 1;
			}

			if(count==8 || j == bmpWidth-1){
				count=0;
				oldindex++;
			}		
		}					
		for (int j = 0; j< align; j++) {//패딩할 byte						
			oldindex++;
		}
	}
}

//
//void BMPclass::adaptiveThreshold(unsigned char* input, unsigned char* bin)
//{
//	unsigned long* integralImg = 0;
//	int i, j;
//	long sum=0;
//	int count=0;
//	int index;
//	int x1, y1, x2, y2;
//	int S = (m_uiWidth/8);
//	int s2 = S/2;
//
//	// create the integral image
//	integralImg = (unsigned long*)malloc(m_uiWidth*m_uiHeight*sizeof(unsigned long*));
//
//	for (i=0; i<m_uiWidth; i++)
//	{
//		// reset this column sum
//		sum = 0;
//		for (j=0; j<m_uiHeight; j++)
//		{
//			index = j*m_uiWidth+i;
//			sum += input[index];
//			if (i==0)
//				integralImg[index] = sum;
//			else
//				integralImg[index] = integralImg[index-1] + sum;
//		}
//	}
//
//	// perform thresholding
//	for (i=0; i<m_uiWidth; i++)
//	{
//		for (j=0; j<m_uiHeight; j++)
//		{
//			index = j*m_uiWidth+i;
//			// set the SxS region
//			x1=i-s2; x2=i+s2;
//			y1=j-s2; y2=j+s2;
//
//			// check the border
//			if (x1 < 0) x1 = 0;
//			if (x2 >= m_uiWidth) x2 = m_uiWidth-1;
//			if (y1 < 0) y1 = 0;
//			if (y2 >= m_uiHeight) y2 = m_uiHeight-1;
//			
//			count = (x2-x1)*(y2-y1);
//			// I(x,y)=s(x2,y2)-s(x1,y2)-s(x2,y1)+s(x1,x1)
//			sum = integralImg[y2*m_uiWidth+x2] -
//				  integralImg[y1*m_uiWidth+x2] -
//				  integralImg[y2*m_uiWidth+x1] +
//				  integralImg[y1*m_uiWidth+x1];
//			if ((long)(input[index]*count) < (long)(sum*(1.0-T)))
//				bin[index] = 0;
//			else
//				bin[index] = 255;
//		}
//	}
//
//	free (integralImg);
//
//	//복사( 수정 필요)
//	for(int i=0; i<ih.biSizeImage; i++)
//	{	
//		m_pucBMP[i]= bin[i];
//	}
//
//}



