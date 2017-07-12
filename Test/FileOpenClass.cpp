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


BOOL FileOpenClass::ReadBMP(CString a_fileName){
	BITMAPFILEHEADER BMPHf;    // 비트맵 파일헤드 구조체
	BITMAPINFOHEADER BMPHi;    // 비트맵 영상헤드 구조체
	int result = m_hFile.Open(a_fileName, CFile::modeRead| CFile::shareDenyWrite,NULL);	
	m_hFile.Read(&BMPHf, sizeof(BITMAPFILEHEADER));
	if(BMPHf.bfType != 0x4D42){
		AfxMessageBox(L"BMP파일이 아닙니다.");
		return FALSE;
	}
	m_hFile.Read(&BMPHi, sizeof(BITMAPINFOHEADER));
	if(BMPHi.biBitCount != 24){
		//AfxMessageBox(L"24비트의 BMP파일만 읽을 수 있습니다.");
		//return FALSE;
	}
	int m_imgSize = m_hFile.GetLength() - sizeof(BITMAPINFOHEADER) - sizeof(BITMAPFILEHEADER);
	m_inImg = new UCHAR[m_imgSize];	
	m_hFile.Read(m_inImg, m_imgSize);//이미지 복사
	m_hFile.Close();

	m_height = BMPHi.biHeight;
	m_width = BMPHi.biWidth;
	
	return TRUE;
}

BOOL FileOpenClass::SaveBMP(CString a_fileName)
{
	//저장경로는 images (임시)
	if(!m_hFile.Open(L".\\images\\"+a_fileName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary)){
		AfxMessageBox(L"BMP 파일 생성 초기화 오류");
		return FALSE;
	}
	//생성자에서 바로 bit 생성
	c_outBMP = new BMPclass(m_height, m_width, m_inImg);
	// 파일에 쓰기
	m_hFile.Write(&c_outBMP->fh, sizeof(BITMAPFILEHEADER));
	m_hFile.Write(&c_outBMP->ih, sizeof(BITMAPINFOHEADER));
	m_hFile.Write(c_outBMP->rgb, sizeof(RGBQUAD)*256);
	m_hFile.Write(c_outBMP->m_pucBMP, c_outBMP->ih.biSizeImage);
	m_hFile.Close();

	delete m_inImg;
	m_inImg=nullptr;

	delete c_outBMP;
	c_outBMP = nullptr;
	return TRUE;
}