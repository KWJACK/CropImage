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
	if(m_inImg!=nullptr) delete m_inImg;			
	
	if(c_outBMP!=NULL) delete c_outBMP;
}


BOOL FileOpenClass::ReadBMP(CString a_fileName){
	int result = m_hFile.Open(a_fileName, CFile::modeRead| CFile::shareDenyWrite,NULL);	
	m_hFile.Read(&m_BMPHf, sizeof(BITMAPFILEHEADER));
	if(m_BMPHf.bfType != 0x4D42){
		AfxMessageBox(L"BMP파일이 아닙니다.");
		return FALSE;
	}
	m_hFile.Read(&m_BMPHi, sizeof(BITMAPINFOHEADER));
	//if(m_BMPHi.biBitCount != 24){
//		AfxMessageBox(L"24비트의 BMP파일만 읽을 수 있습니다.");
		//return FALSE;
	//}
	m_imgSize = m_hFile.GetLength() - sizeof(BITMAPINFOHEADER) - sizeof(BITMAPFILEHEADER);
	m_inImg = new UCHAR[m_imgSize];	
	m_hFile.Read(m_inImg, m_imgSize);//이미지 복사
	m_hFile.Close();

	m_height = m_BMPHi.biHeight;
	m_width = m_BMPHi.biWidth;
	
	return TRUE;
}

BOOL FileOpenClass::SaveBMP(CString a_fileName)
{
	if(!m_hFile.Open(L".\\images\\"+a_fileName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary)){
		AfxMessageBox(L"BMP 파일 생성 초기화 오류");
		return FALSE;
	}
	c_outBMP = new BMPclass(m_height, m_width, m_inImg);
	// 파일에 쓰기
	m_hFile.Write(&c_outBMP->fh, sizeof(BITMAPFILEHEADER));
	m_hFile.Write(&c_outBMP->ih, sizeof(BITMAPINFOHEADER));
	m_hFile.Write(c_outBMP->rgb, sizeof(RGBQUAD)*256);
	m_hFile.Write(c_outBMP->m_pucBMP, c_outBMP->ih.biSizeImage);
	m_hFile.Close();

	delete m_inImg;
	m_inImg=nullptr;

	delete c_outBMP->m_pucBMP;
	c_outBMP = nullptr;

	delete c_outBMP;
	return TRUE;
}