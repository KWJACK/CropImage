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
		AfxMessageBox(L"BMP������ �ƴմϴ�.");
		return FALSE;
	}
	m_hFile.Read(&BMPHi, sizeof(BITMAPINFOHEADER));
	if(BMPHi.biBitCount != 24){
		AfxMessageBox(L"24��Ʈ�� BMP���ϸ� ���� �� �ֽ��ϴ�.");
		return FALSE;
	}
		
	int m_imgSize = m_hFile.GetLength() - sizeof(BITMAPINFOHEADER) - sizeof(BITMAPFILEHEADER);
	m_inImg = new UCHAR[m_imgSize];
	m_hFile.Read(m_inImg, m_imgSize);//�̹��� ����
	m_hFile.Close();
	m_height = BMPHi.biHeight;
	m_width = BMPHi.biWidth;
	
	return TRUE;
}

BOOL FileOpenClass::SaveBMP(IN CString a_fileName)
{
	//�����δ� images (�ӽ�)
	if(!m_hFile.Open(L".\\images\\"+a_fileName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary)){
		AfxMessageBox(L"BMP ���� ���� �ʱ�ȭ ����");
		return FALSE;
	}
	//�����ڿ��� �ٷ� bit ����
	c_outBMP = new BMPclass(m_height, m_width, m_inImg);
	// ���Ͽ� �׷��� ������ �̹��� �ۼ�(��ΰ� �������ϰ� �ߺ��Ǽ� �׷��̸� ������ ��� �ٲ���մϴ�)
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
	RGBQUAD rgb2[2];//�߰�
	int result = m_hFile.Open(a_path, CFile::modeRead| CFile::shareDenyWrite,NULL);		

	int dataSize = m_hFile.GetLength() - //��ü ���� ũ�� - (��Ʈ�����+��� �ķ�Ʈ)
		(sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+sizeof(RGBQUAD)*2);//
	
	m_hFile.Read(&BMPHf, sizeof(BITMAPFILEHEADER));
	if(BMPHf.bfType != 0x4D42){
		AfxMessageBox(L"BMP������ �ƴմϴ�.");
		return FALSE;
	}
	m_hFile.Read(&BMPHi, sizeof(BITMAPINFOHEADER));
	if(BMPHi.biBitCount != 1){
		AfxMessageBox(L"1��Ʈ�� BMP���ϸ� ���� �� �ֽ��ϴ�.");
		return FALSE;
	}	
	
	m_hFile.Read(rgb2, sizeof(rgb2));//�ķ�Ʈ ���� �о ����
	for(int i=0;i<2;i++){
		rgb2[i].rgbRed =~rgb2[i].rgbRed;
		rgb2[i].rgbBlue =~rgb2[i].rgbBlue;
		rgb2[i].rgbGreen =~rgb2[i].rgbGreen;
	}

	m_inImg = new UCHAR[dataSize];
	m_hFile.Read(m_inImg, dataSize);//������ ���� �о ����
	for(int i=0;i<dataSize;i++){
		m_inImg[i] =~ m_inImg[i];
	}
	m_hFile.Close();

	if(!m_hFile.Open(L".\\images\\"+a_fileName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary)){
		AfxMessageBox(L"BMP ���� ���� �ʱ�ȭ ����");
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