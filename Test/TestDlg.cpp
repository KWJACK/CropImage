
// TestDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "Test.h"
#include "TestDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTestDlg 대화 상자
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


// CTestDlg 메시지 처리기

BOOL CTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	m_sTmp = L"C:\\Users\\jaekeun\\Desktop\\job\\sampleImage(100)\\temp\\*.*";
	m_sPath = L"C:\\Users\\jaekeun\\Desktop\\job\\sampleImage(100)\\temp\\";
	m_FileClass = new FileOpenClass;
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CTestDlg::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(changeFileName() >0){
		MessageBox(L"파일 이름 수정 완료");
	}else{
		MessageBox(L"에러 발생");
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
				newName.TrimLeft(L"셔");
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
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(checkFileName() >0){
		MessageBox(L"유효성 검사 완료");
	}else{
		MessageBox(L"에러 발생");
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
		USHORT nShort = 0xfeff;  // 유니코드 바이트 오더마크.  
		if(m_fp.Open(L"errorPrint.txt", CFile::modeCreate |CFile::modeWrite | CStdioFileEx::modeWriteUnicode)){			
			do {
				if (fd.cFileName[0] == '.') {//current and parent path ignore						
					continue;
				}
				else {						
					errFile = fd.cFileName;
					FileName = fd.cFileName;					
					wcstok (FileName, L".");//확장자 이름으로 구분합니다. (파일 이름에 .이 있으면 안됨)				
					newName = FileName;	//이부분 수정 필요 
					wcstok(FileName, L"_");//맨 처음 ad는 사용안함					
					TCHAR* wWord = wcstok(NULL, L"_");	//글자 취득
					CString newWord = wWord;
					if(newWord!= wOldWord){//pOldWord랑 같다면					
						wOldWord = newWord;	//oldWord 변경
						m_wordLength=wcslen(wOldWord);	//글자 수 멤버 변수 변경
						oldIndex = -1;
					}
					oldIndex++;
					if(wcscmp(wWord, wOldWord) == 0){
						TCHAR* wIndex = wcstok(NULL, L"_");//글자 인덱스
						int newIndex = _wtoi(wIndex);

						//TCHAR* wChar = wcstok(NULL, L"_");//한 글자
						//if(wChar == NULL){							 
						//	m_fp.WriteString(L"[ErrNoChar] "+errFile+L"\r\n");
						//	continue;
						//}
						//if(wcslen(wChar)!=1){//한 글자 취득인데 한 글자가 아닐 경우에							 
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
						//if(wWord[newIndex] != wChar[0]){//해당 인덱스의 글자가 다를 때 							
						//	m_fp.WriteString(L"[ErrMatching] "+errFile+L"\r\n");
						//	continue;
						//}
						
					}

				}								
			} while (FindNextFile(hFind, &fd));		
			FindClose(hFind);//handle 반환
			m_fp.Close();	//파일 닫음
			return success;
		}
		else{
			MessageBox(L"errorPrint.txt 파일 생성 불가");
		}
	}
	return fail;
}

void CTestDlg::OnBnClickedButton3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(makeFileNmae() >0){
		MessageBox(L"temp폴더 내 파일 이름 모두 변경");
	}else{
		MessageBox(L"에러 발생");
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
				wcstok (FileName, L".");//확장자 이름으로 구분합니다. (파일 이름에 .이 있으면 안됨)				
				newName = FileName;	
				wcstok(FileName, L"_");//맨 처음 ad는 사용안함					
				TCHAR* wWord = wcstok(NULL, L"_");	//글자 취득
				CString newWord = wWord;										
				TCHAR* wIndex = wcstok(NULL, L"_");//글자 인덱스
				int newIndex = _wtoi(wIndex);												

				CString newChar(wWord[newIndex]);
				newChar = newName+L"_"+newChar+L".bmp";
				try{
					CFile::Rename(m_sPath+oldFileName, m_sPath+(TCHAR*)(LPCTSTR)newChar);//이름 바꿀 때는 전체 경로가 있어야함
				}catch(CFileException* pEx){					
					printf("File %20s not found, cause = %d\n", fd.cFileName, pEx->m_cause);
					pEx->Delete();
				}
			}								
		} while (FindNextFile(hFind, &fd));		
		FindClose(hFind);//handle 반환
		m_fp.Close();	//파일 닫음
		return success;
	}
	return fail;
}

void CTestDlg::OnBnClickedButton4()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.	
	if(makeBinary() >0){
		MessageBox(L"temp폴더 내 파일 이름 모두 변경");
	}else{
		MessageBox(L"에러 발생");
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
		USHORT nShort = 0xfeff;  // 유니코드 바이트 오더마크.  		
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
		FindClose(hFind);//handle 반환
		//m_fp.Close();	//파일 닫음
		return success;
	}
	return fail;
}


void CTestDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	delete m_FileClass;
	
}
