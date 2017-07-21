#include "stdafx.h"
#include "MyBMPclass.h"


MyBMPclass::MyBMPclass(void):m_height(0), m_width(0), bmpWidth(0), m_imgSize(0)
{
	m_bin =m_pucBMP = m_inImg = nullptr;	
}


MyBMPclass::~MyBMPclass(void)
{
	if(m_inImg!=nullptr){
		delete m_inImg;			
		m_inImg = nullptr;
	}
	if(m_pucBMP){
		delete m_pucBMP;
		m_pucBMP = nullptr;
	}
}

BOOL MyBMPclass::readBMP(IN CString a_fileName, OPTIONAL int flag)
{
	int result = m_hFile.Open(a_fileName, CFile::modeRead| CFile::shareDenyWrite,NULL);			
	m_hFile.Read(&BMPHf, sizeof(BITMAPFILEHEADER));
	if(BMPHf.bfType != 0x4D42){
		AfxMessageBox(L"BMP������ �ƴմϴ�.");
		return FALSE;
	}
	m_hFile.Read(&BMPHi, sizeof(BITMAPINFOHEADER));	
	if(flag == bit24){
		if(BMPHi.biBitCount != 24){
			AfxMessageBox(L"24��Ʈ�� BMP���ϸ� ���� �� �ֽ��ϴ�.");
			return FALSE;
		}
		m_imgSize = m_hFile.GetLength() - sizeof(BITMAPINFOHEADER) - sizeof(BITMAPFILEHEADER);
	}
	else if(flag == bit1){
		if(BMPHi.biBitCount != 1){
			AfxMessageBox(L"1��Ʈ�� BMP���ϸ� ���� �� �ֽ��ϴ�.");
			return FALSE;
		}
		m_imgSize = m_hFile.GetLength() - (sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+sizeof(RGBQUAD)*2);
		m_hFile.Read(rgb2, sizeof(rgb2));//�ķ�Ʈ ���� �о ����
		for(int i=0;i<2;i++){
			rgb2[i].rgbRed =~rgb2[i].rgbRed;
			rgb2[i].rgbBlue =~rgb2[i].rgbBlue;
			rgb2[i].rgbGreen =~rgb2[i].rgbGreen;
		}
	}

	m_inImg = new UCHAR[m_imgSize];
	m_hFile.Read(m_inImg, m_imgSize);//�̹��� ����
	
	m_height = BMPHi.biHeight;
	m_width = BMPHi.biWidth;

	m_hFile.Close();
	return TRUE;
}
BOOL MyBMPclass::createBMP(OUT UCHAR* a_outImg)
{
	//calculate byte align
	bmpWidth = (m_width * 3 + 3) & ~3; //original ver
	int align = bmpWidth - (m_width*3);
	int Bialign = align;
	if(align ==1 || align==3) Bialign = 4 - align;

	bmpWidth = m_width + Bialign;	
	//initialize
	memset(&BMPHf, 0, sizeof(BITMAPFILEHEADER));
	memset(&BMPHi, 0, sizeof(BITMAPINFOHEADER));
	memset(&rgb, 0, sizeof(RGBQUAD) * 256);		
	//write header file
	BMPHf.bfOffBits = 1078; // RGBQUAD + InfoHeader + FileHeader only 8bit mode if 24bit == 54; 40+ 14; 
	BMPHf.bfSize = m_height * m_width + sizeof(RGBQUAD) * 256 + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	BMPHf.bfType = 19778;
	//write header info
	BMPHi.biBitCount = 8;
	BMPHi.biHeight = m_height;
	BMPHi.biWidth = m_width;
	BMPHi.biPlanes = 1;
	BMPHi.biSize = 40;
	BMPHi.biSizeImage = bmpWidth * m_height;	
	BMPHi.biXPelsPerMeter = 0;
	BMPHi.biYPelsPerMeter = 0;
	//write palette
	for(int i = 0; i < 256; i++)
	{
		rgb[i].rgbBlue = i;
		rgb[i].rgbGreen = i;
		rgb[i].rgbRed = i;
		rgb[i].rgbReserved = 0;
	}
   //copy image data info
	m_pucBMP = new UCHAR[BMPHi.biSizeImage];
	memset(m_pucBMP, 0, sizeof(UCHAR)*BMPHi.biSizeImage);

	int index = 0, oldindex=0;
	UINT R,G,B,GRAY;	
	for(int i=0; i<m_height; i++)
	{	
		for(int j=0; j<m_width; j++)
		{		
			B = m_inImg[index++];
			G = m_inImg[index++];
			R = m_inImg[index++];
			GRAY = (UCHAR)(0.299*R + 0.587*G + 0.114*B);//convert RGB->Gray
			m_pucBMP[oldindex++] = GRAY;
		}				
		for(int j=0;j<align;j++){//byte align
			index++;		
		}
		for(int j=0;j<Bialign;j++){			
			m_pucBMP[oldindex++] = 255;//In here palette is different. we think whitespace has 255 values
		}
	}
	return TRUE;
}

int	MyBMPclass::binarizationIterative()
{
	float hist[256] = { 0, };
	makeHistogram(hist);
	//�ʱ� �Ӱ谪 ���� - �׷��� ������ ���� ��ü ���
	int i, T, Told;
	float sum = 0.f;
	for (i = 0; i < 256; i++) {
		sum += (i*hist[i]);
	}
	T = static_cast<int>(sum + .5f);
	//�ݺ��� ���� �Ӱ谪 ����
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
void MyBMPclass::makeHistogram(OUT float histo[256])
{
	//������׷� ���
	int size = m_width * m_height;		
	int cnt[256] = { 0, };//256�� gray ��Ʈ����
	int Index=0;
	for (int i = 0; i < m_height; i++){
		for(int j=0;j<m_width;j++)
			cnt[m_pucBMP[Index++]]++;
		for(int j=0;j<bmpWidth-m_width;j++)
			Index++;
	}
	//������׷� ����ȭ
	for (int i = 0; i < 256; i++)
	{
		histo[i] = static_cast<float>(cnt[i]) / size;
	}
}


BOOL MyBMPclass::make1bpp()
{
	int widthBitAlign = (m_width + 7) & 0xFFFFFFF8; 
	int Bialign = widthBitAlign-m_width;
	int bbmpWidth = (widthBitAlign/8 + 3) & ~3; //original ver
	int align = bbmpWidth - widthBitAlign/8;//align = 0,1,2,3	

	memset(&BMPHf, 0, sizeof(BITMAPFILEHEADER));
	memset(&BMPHi, 0, sizeof(BITMAPINFOHEADER));
	memset(&rgb2, 0, sizeof(RGBQUAD) * 2);

	BMPHf.bfOffBits = 62; // RGBQUAD(8) + InfoHeader(40) + FileHeader(14) only 8bit mode if 24bit == 54; 40+ 14; 
	BMPHf.bfSize = m_height*bbmpWidth + sizeof(RGBQUAD) * 2 + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	BMPHf.bfType = 19778;

	BMPHi.biBitCount = 1;
	BMPHi.biHeight = m_height;
	BMPHi.biWidth = m_width;
	BMPHi.biPlanes = 1;
	BMPHi.biSize = 40;//bmp info��� ũ��(40)
	BMPHi.biSizeImage = bbmpWidth*m_height;
	BMPHi.biXPelsPerMeter = 0;
	BMPHi.biYPelsPerMeter = 0;
		
	rgb2[0].rgbBlue = 255;//���
	rgb2[0].rgbGreen = 255;
	rgb2[0].rgbRed = 255;
	rgb2[0].rgbReserved = 255;

	rgb2[1].rgbBlue = 0;//������
	rgb2[1].rgbGreen = 0;
	rgb2[1].rgbRed = 0;
	rgb2[1].rgbReserved = 255;
	
	m_bin = new UCHAR[BMPHi.biSizeImage];
	memset(m_bin, 0, sizeof(UCHAR)*BMPHi.biSizeImage);

	int index = 0, oldindex = 0, count=0;	
	for (int i = 0; i<m_height; i++)
	{		
		for (int j = 0; j< bmpWidth; j++)//8bit�� align��
		{		
			count++;
			m_bin[oldindex] = m_bin[oldindex]<<1;
			//���̸� �ι� ° �ķ�Ʈ �� �߰�. ����Ʈ�� ù��° �ķ�Ʈ ��
			if(m_pucBMP[index++]==0){
				m_bin[oldindex] |= 1;
			}

			if(count==8 || j == bmpWidth-1){
				count=0;
				oldindex++;
			}		
		}					
		for (int j = 0; j< align; j++) {//�е��� byte						
			oldindex++;
		}
	}
	return TRUE;
}

BOOL MyBMPclass::saveBMP(IN CString a_fileName, OPTIONAL int flag)
{
	if(!m_hFile.Open(L".\\images\\"+a_fileName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary)){
		AfxMessageBox(L"BMP ���� ���� �ʱ�ȭ ����");
		return FALSE;
	}
	m_hFile.Write(&BMPHf, sizeof(BITMAPFILEHEADER));
	m_hFile.Write(&BMPHi, sizeof(BITMAPINFOHEADER));
	m_hFile.Write(rgb2, sizeof(RGBQUAD) * 2);
	m_hFile.Write(m_bin, BMPHi.biSizeImage);
	m_hFile.Close();
}

BOOL MyBMPclass::runMake1bpp(IN CString a_fileName, IN CString a_newFileName, OPTIONAL int flag){
	if(!readBMP(a_fileName, flag))return FALSE;
	if(!createBMP(m_pucBMP))return FALSE;
	
	delete m_inImg;
	m_inImg=nullptr;

	int Threshold =binarizationIterative();
	for (int i = 0; i < m_height*bmpWidth; i++)
	{
		m_pucBMP[i] = (m_pucBMP[i] <= Threshold) ? 0 : 255;
	}
	if(!make1bpp())return FALSE;

	delete m_pucBMP;
	m_pucBMP=nullptr;

	if(!saveBMP(a_newFileName, flag))return FALSE;	

	return TRUE;
}