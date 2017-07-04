
// TestDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "Test.h"
#include "TestDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTestDlg ��ȭ ����
using namespace std;



CTestDlg::CTestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTestDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CTestDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CTestDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CTestDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CTestDlg::OnBnClickedButton4)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CTestDlg �޽��� ó����

BOOL CTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	m_sTmp = L"C:\\Users\\jaekeun\\Desktop\\job\\sampleImage(100)\\temp\\*.*";
	m_sPath = L"C:\\Users\\jaekeun\\Desktop\\job\\sampleImage(100)\\temp\\";
	m_FileClass = new FileOpenClass;
	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CTestDlg::OnBnClickedButton1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if(changeFileName() >0){
		MessageBox(L"���� �̸� ���� �Ϸ�");
	}else{
		MessageBox(L"���� �߻�");
	}
}

int CTestDlg::changeFileName(){

	CString m_sTmp = L".\\nameproblem\\*.*";	
	CString path = L"C:\\Users\\jaekeun\\Documents\\pro\\Test\\Test\\nameproblem\\";
	WIN32_FIND_DATA fd;
	HANDLE hFind = FindFirstFile(m_sTmp, &fd);
	CString newName =L"";
	if (INVALID_HANDLE_VALUE != hFind)
	{
		do {
			if (fd.cFileName[0] == '.') {//current and parent path ignore						
				continue;
			}
			else {				
				newName = fd.cFileName;
				newName.TrimLeft(L"��");
				try{
					CFile::Rename(path+fd.cFileName, path+(TCHAR*)(LPCTSTR)newName);
				}catch(CFileException* pEx){					
					printf("File %20s not found, cause = %d\n", fd.cFileName, pEx->m_cause);
					pEx->Delete();
				}
			}

		} while (FindNextFile(hFind, &fd));

		FindClose(hFind);
		return success;
	}
	return fail;

}

void CTestDlg::OnBnClickedButton2()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if(checkFileName() >0){
		MessageBox(L"��ȿ�� �˻� �Ϸ�");
	}else{
		MessageBox(L"���� �߻�");
	}

}

int CTestDlg::checkFileName()
{			
	WIN32_FIND_DATA fd;
	HANDLE hFind = FindFirstFile(m_sTmp, &fd);
	CString newName =L"";
	TCHAR* FileName= nullptr;
	CString wOldWord = L"_";
	CString errFile=L"";

	int oldIndex=-1;
	if (INVALID_HANDLE_VALUE != hFind)
	{
		USHORT nShort = 0xfeff;  // �����ڵ� ����Ʈ ������ũ.  
		if(m_fp.Open(L"errorPrint.txt", CFile::modeCreate |CFile::modeWrite | CStdioFileEx::modeWriteUnicode)){			
			do {
				if (fd.cFileName[0] == '.') {//current and parent path ignore						
					continue;
				}
				else {						
					errFile = fd.cFileName;
					FileName = fd.cFileName;					
					wcstok (FileName, L".");//Ȯ���� �̸����� �����մϴ�. (���� �̸��� .�� ������ �ȵ�)				
					newName = FileName;	//�̺κ� ���� �ʿ� 
					wcstok(FileName, L"_");//�� ó�� ad�� ������					
					TCHAR* wWord = wcstok(NULL, L"_");	//���� ���
					CString newWord = wWord;
					if(newWord!= wOldWord){//pOldWord�� ���ٸ�					
						wOldWord = newWord;	//oldWord ����
						m_wordLength=wcslen(wOldWord);	//���� �� ��� ���� ����
						oldIndex = -1;
					}
					oldIndex++;
					if(wcscmp(wWord, wOldWord) == 0){
						TCHAR* wIndex = wcstok(NULL, L"_");//���� �ε���
						int newIndex = _wtoi(wIndex);

						//TCHAR* wChar = wcstok(NULL, L"_");//�� ����
						//if(wChar == NULL){							 
						//	m_fp.WriteString(L"[ErrNoChar] "+errFile+L"\r\n");
						//	continue;
						//}
						//if(wcslen(wChar)!=1){//�� ���� ����ε� �� ���ڰ� �ƴ� ��쿡							 
						//	m_fp.WriteString(L"[ErrOverChar] "+errFile+L"\r\n");
						//	continue;
						//}						
						if(oldIndex != newIndex){							
							m_fp.WriteString(L"[ErrIndex] "+errFile+L"\r\n");
							continue;
						}						
						if(m_wordLength<oldIndex){
							m_fp.WriteString(L"[ErrShortWord] "+errFile+L"\r\n");
							continue;
						}
						//if(wWord[newIndex] != wChar[0]){//�ش� �ε����� ���ڰ� �ٸ� �� 							
						//	m_fp.WriteString(L"[ErrMatching] "+errFile+L"\r\n");
						//	continue;
						//}
						
					}

				}								
			} while (FindNextFile(hFind, &fd));		
			FindClose(hFind);//handle ��ȯ
			m_fp.Close();	//���� ����
			return success;
		}
		else{
			MessageBox(L"errorPrint.txt ���� ���� �Ұ�");
		}
	}
	return fail;
}

void CTestDlg::OnBnClickedButton3()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if(makeFileNmae() >0){
		MessageBox(L"temp���� �� ���� �̸� ��� ����");
	}else{
		MessageBox(L"���� �߻�");
	}
}


int CTestDlg::makeFileNmae(){	
	WIN32_FIND_DATA fd;
	HANDLE hFind = FindFirstFile(m_sTmp, &fd);
	CString newName =L"";
	TCHAR* FileName= nullptr;	
	CString oldFileName=L"";
	if (INVALID_HANDLE_VALUE != hFind)
	{		
		do {
			if (fd.cFileName[0] == '.') {//current and parent path ignore						
				continue;
			}
			else {		
				oldFileName = fd.cFileName;				
				FileName = fd.cFileName;					
				wcstok (FileName, L".");//Ȯ���� �̸����� �����մϴ�. (���� �̸��� .�� ������ �ȵ�)				
				newName = FileName;	
				wcstok(FileName, L"_");//�� ó�� ad�� ������					
				TCHAR* wWord = wcstok(NULL, L"_");	//���� ���
				CString newWord = wWord;										
				TCHAR* wIndex = wcstok(NULL, L"_");//���� �ε���
				int newIndex = _wtoi(wIndex);												

				CString newChar(wWord[newIndex]);
				newChar = newName+L"_"+newChar+L".bmp";
				try{
					CFile::Rename(m_sPath+oldFileName, m_sPath+(TCHAR*)(LPCTSTR)newChar);//�̸� �ٲ� ���� ��ü ��ΰ� �־����
				}catch(CFileException* pEx){					
					printf("File %20s not found, cause = %d\n", fd.cFileName, pEx->m_cause);
					pEx->Delete();
				}
			}								
		} while (FindNextFile(hFind, &fd));		
		FindClose(hFind);//handle ��ȯ
		m_fp.Close();	//���� ����
		return success;
	}
	return fail;
}

void CTestDlg::OnBnClickedButton4()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.	
	if(makeBinary() >0){
		MessageBox(L"temp���� �� ���� �̸� ��� ����");
	}else{
		MessageBox(L"���� �߻�");
	}
}

int CTestDlg::makeBinary()
{		
	WIN32_FIND_DATA fd;
	HANDLE hFind = FindFirstFile(m_sTmp, &fd);
	CString newName =L"";
	TCHAR* FileName= nullptr;		
	if (INVALID_HANDLE_VALUE != hFind)
	{
		USHORT nShort = 0xfeff;  // �����ڵ� ����Ʈ ������ũ.  		
		do {
			if (fd.cFileName[0] == '.') {//current and parent path ignore						
				continue;
			}
			else {
				CString tempFileName = fd.cFileName;
				m_FileClass->ReadBMP(m_sPath+tempFileName);
				m_FileClass->SaveBMP(tempFileName);
			}								
		} while (FindNextFile(hFind, &fd));		
		FindClose(hFind);//handle ��ȯ
		//m_fp.Close();	//���� ����
		return success;
	}
	return fail;
}


void CTestDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	delete m_FileClass;
	
}
