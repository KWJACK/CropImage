#include "stdafx.h"
#include "FileOpenClass.h"


FileOpenClass::FileOpenClass(void):
	m_inImg(nullptr)	
	, m_height(0)
	, m_width(0)
	, c_outBMP(NULL)
{

}


FileOpenClass::~FileOpenClass(void)
{
	if(m_inImg!=nullptr){
		delete m_inImg;			
		m_inImg = nullptr;
	}	
	if(c_outBMP!=NULL){
		delete c_outBMP;
		c_outBMP = nullptr;
	}
}


BOOL FileOpenClass::ReadBMP(IN CString a_fileName){
	int result = m_hFile.Open(a_fileName, CFile::modeRead| CFile::shareDenyWrite,NULL);		
	
	m_hFile.Read(&BMPHf, sizeof(BITMAPFILEHEADER));
	if(BMPHf.bfType != 0x4D42){
		AfxMessageBox(L"BMP파일이 아닙니다.");
		return FALSE;
	}
	m_hFile.Read(&BMPHi, sizeof(BITMAPINFOHEADER));
	if(BMPHi.biBitCount != 24){
		AfxMessageBox(L"24비트의 BMP파일만 읽을 수 있습니다.");
		return FALSE;
	}
		
	int m_imgSize = m_hFile.GetLength() - sizeof(BITMAPINFOHEADER) - sizeof(BITMAPFILEHEADER);
	m_inImg = new UCHAR[m_imgSize];
	m_hFile.Read(m_inImg, m_imgSize);//이미지 복사
	m_hFile.Close();
	m_height = BMPHi.biHeight;
	m_width = BMPHi.biWidth;
	
	return TRUE;
}

BOOL FileOpenClass::SaveBMP(IN CString a_fileName)
{
	//저장경로는 images (임시)
	if(!m_hFile.Open(L".\\images\\"+a_fileName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary)){
		AfxMessageBox(L"BMP 파일 생성 초기화 오류");
		return FALSE;
	}
	//생성자에서 바로 bit 생성
	c_outBMP = new BMPclass(m_height, m_width, m_inImg);
	// 파일에 그레이 스케일 이미지 작성(경로가 이진파일과 중복되서 그레이만 빼려면 경로 바꿔야합니다)
	/*m_hFile.Write(&c_outBMP->fh, sizeof(BITMAPFILEHEADER));
	m_hFile.Write(&c_outBMP->ih, sizeof(BITMAPINFOHEADER));
	m_hFile.Write(c_outBMP->rgb, sizeof(RGBQUAD)*256);
	m_hFile.Write(c_outBMP->m_pucBMP, c_outBMP->ih.biSizeImage);	
	m_hFile.Close();*/

	delete m_inImg;
	m_inImg=nullptr;

	c_outBMP->bpp1BMP();
	m_hFile.Write(&c_outBMP->fh, sizeof(BITMAPFILEHEADER));
	m_hFile.Write(&c_outBMP->ih, sizeof(BITMAPINFOHEADER));
	m_hFile.Write(c_outBMP->rgb2, sizeof(RGBQUAD) * 2);
	m_hFile.Write(c_outBMP->m_bin, c_outBMP->ih.biSizeImage);
	m_hFile.Close();

	delete c_outBMP;
	c_outBMP = nullptr;
	return TRUE;
}

BOOL FileOpenClass::PaletteChange(IN CString a_path, IN CString a_fileName){
	RGBQUAD rgb2[2];//추가
	int result = m_hFile.Open(a_path, CFile::modeRead| CFile::shareDenyWrite,NULL);		

	int dataSize = m_hFile.GetLength() - //전체 파일 크기 - (비트맵헤더+흑백 파레트)
		(sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+sizeof(RGBQUAD)*2);//
	
	m_hFile.Read(&BMPHf, sizeof(BITMAPFILEHEADER));
	if(BMPHf.bfType != 0x4D42){
		AfxMessageBox(L"BMP파일이 아닙니다.");
		return FALSE;
	}
	m_hFile.Read(&BMPHi, sizeof(BITMAPINFOHEADER));
	if(BMPHi.biBitCount != 1){
		AfxMessageBox(L"1비트의 BMP파일만 읽을 수 있습니다.");
		return FALSE;
	}	
	
	m_hFile.Read(rgb2, sizeof(rgb2));//파레트 정보 읽어서 반전
	for(int i=0;i<2;i++){
		rgb2[i].rgbRed =~rgb2[i].rgbRed;
		rgb2[i].rgbBlue =~rgb2[i].rgbBlue;
		rgb2[i].rgbGreen =~rgb2[i].rgbGreen;
	}

	m_inImg = new UCHAR[dataSize];
	m_hFile.Read(m_inImg, dataSize);//데이터 정보 읽어서 반전
	for(int i=0;i<dataSize;i++){
		m_inImg[i] =~ m_inImg[i];
	}
	m_hFile.Close();

	if(!m_hFile.Open(L".\\images\\"+a_fileName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary)){
		AfxMessageBox(L"BMP 파일 생성 초기화 오류");
		return FALSE;
	}
	m_hFile.Write(&BMPHf, sizeof(BITMAPFILEHEADER));
	m_hFile.Write(&BMPHi, sizeof(BITMAPINFOHEADER));
	m_hFile.Write(rgb2, sizeof(RGBQUAD) * 2);
	m_hFile.Write(m_inImg, dataSize);
	m_hFile.Close();
	delete m_inImg;
	m_inImg=nullptr;

	return TRUE;
}