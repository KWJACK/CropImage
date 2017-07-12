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
	BITMAPFILEHEADER BMPHf;    // ��Ʈ�� ������� ����ü
	BITMAPINFOHEADER BMPHi;    // ��Ʈ�� ������� ����ü
	int result = m_hFile.Open(a_fileName, CFile::modeRead| CFile::shareDenyWrite,NULL);	
	m_hFile.Read(&BMPHf, sizeof(BITMAPFILEHEADER));
	if(BMPHf.bfType != 0x4D42){
		AfxMessageBox(L"BMP������ �ƴմϴ�.");
		return FALSE;
	}
	m_hFile.Read(&BMPHi, sizeof(BITMAPINFOHEADER));
	if(BMPHi.biBitCount != 24){
		//AfxMessageBox(L"24��Ʈ�� BMP���ϸ� ���� �� �ֽ��ϴ�.");
		//return FALSE;
	}
	int m_imgSize = m_hFile.GetLength() - sizeof(BITMAPINFOHEADER) - sizeof(BITMAPFILEHEADER);
	m_inImg = new UCHAR[m_imgSize];	
	m_hFile.Read(m_inImg, m_imgSize);//�̹��� ����
	m_hFile.Close();

	m_height = BMPHi.biHeight;
	m_width = BMPHi.biWidth;
	
	return TRUE;
}

BOOL FileOpenClass::SaveBMP(CString a_fileName)
{
	//�����δ� images (�ӽ�)
	if(!m_hFile.Open(L".\\images\\"+a_fileName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary)){
		AfxMessageBox(L"BMP ���� ���� �ʱ�ȭ ����");
		return FALSE;
	}
	//�����ڿ��� �ٷ� bit ����
	c_outBMP = new BMPclass(m_height, m_width, m_inImg);
	// ���Ͽ� ����
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