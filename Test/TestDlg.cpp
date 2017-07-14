
// TestDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "Test.h"
#include "TestDlg.h"
#include "afxdialogex.h"
#include "MyScrollView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
// CTestDlg ��ȭ ����
using namespace std;


#define RESULT_PATH L".\\images\\"

CTestDlg::CTestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTestDlg::IDD, pParent), m_bCustomFolder(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_pPreviewDlg = NULL;
}

void CTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FOLDERS_TREE, m_cTreeCtrl);
	DDX_Control(pDX, IDC_LIST_REM, m_cListCtrlRem);
}

BEGIN_MESSAGE_MAP(CTestDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CTestDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CTestDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CTestDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CTestDlg::OnBnClickedButton4)
	ON_WM_DESTROY()
	ON_NOTIFY(TVN_SELCHANGED, IDC_FOLDERS_TREE, OnTvnSelchanged)
	ON_WM_MOUSEHWHEEL()	
	ON_BN_CLICKED(IDC_BUTTON5, &CTestDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CTestDlg::OnBnClickedButton6)
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

	m_sTmp = L"C:\\Users\\jaekeun\\Desktop\\job\\sampleImage(200)\\temp\\*.*";
	//m_sTmp = L"C:\\Users\\jaekeun\\Desktop\\origin\\*.*";
	m_sPath = L"C:\\Users\\jaekeun\\Desktop\\job\\sampleImage(200)\\temp\\";
	//m_sPath = L"C:\\Users\\jaekeun\\Desktop\\origin\\";
	m_FileClass = new FileOpenClass;

	ShowWindow(SW_SHOWMAXIMIZED);//���̾�α� �ִ�ȭ

	// Not displaying subfolders	
	m_cListCtrlRem.SetItemTypes(SHCONTF_NONFOLDERS);
	// Passing project files map pointer to both shell list controls	
	m_cListCtrlRem.SetProjectFiles(&m_cProjFileMap);
	// Setting list control filters	
	m_cListCtrlRem.SetFilter(LISTFILTER_REMAINING);
	HTREEITEM hParentItem = m_cTreeCtrl.GetRootItem();
	m_cTreeCtrl.SelectItem(hParentItem);
	m_cTreeCtrl.Expand(hParentItem, TVE_EXPAND);		
	//PrevieweImageDlg ����
	//m_pPreviewDlg = new CPreviewDlg();
	//m_pPreviewDlg->Create(IDD_PREVIEW_DLG, this);
	
	//�ʱ� ��ġ�ϱ�
	CRect trt, prt,lrt;
	this->GetClientRect(prt);
	m_cTreeCtrl.GetClientRect(trt);
	m_cListCtrlRem.GetClientRect(lrt);

	m_cTreeCtrl.MoveWindow(0,0,trt.Width(), prt.Height());
	m_cListCtrlRem.MoveWindow(trt.right, 0, 300, prt.Height());
	//m_pPreviewDlg->SetWindowPos(&wndTopMost, 20, 320, 700, 630, SWP_DRAWFRAME);
	//m_pPreviewDlg->MoveWindow(lrt.Width()+trt.right, 0, 700, prt.Height());

	m_cListCtrlRem.m_preViewDlg = m_pPreviewDlg;
	
	//��ũ�Ѻ� ����
	/*CRuntimeClass *pObject;
	pObject = RUNTIME_CLASS(CMyScrollView);
	CMyScrollView* pView = (CMyScrollView*)pObject->CreateObject();

	if (!pView->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
		CRect(trt.right+400, 0, trt.right + 400+700, prt.Height()), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("Failed to create view window\n");
		return -1;
	}
	CSize sizeTotal;
	sizeTotal.cx = sizeTotal.cy = 100;
	pView->SetScrollSizes(MM_TEXT, sizeTotal);*/
	
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
		ShellExecute(NULL, L"open", L"notepad", L".\\errorPrint.txt", NULL, SW_SHOW);
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
		//m_fp.Close();	//���� ���� 
		return success;
	}
	return fail;
}

void CTestDlg::OnBnClickedButton4()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.	
	if(makeBinary() >0){
		MessageBox(L"����ȭ ����(1bpp)");
		ShellExecute(NULL, L"open", L"explorer", RESULT_PATH, NULL, SW_SHOW);
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
		return success;
	}
	return fail;
}


void CTestDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	if( m_FileClass ){
		delete m_FileClass;
	}
	if( m_pPreviewDlg )
	{
		delete m_pPreviewDlg;
	}	
}



void CTestDlg::OnBnClickedButton5()
{
	if(elimanate2Char() >0){
		MessageBox(L"temp���� �� ���� �� ���ڸ� ����");
	}else{
		MessageBox(L"���� �߻�");
	}
}

int CTestDlg::elimanate2Char(){	
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
				newName.Delete(newName.GetLength()-2,2);
				newName = newName+L".bmp";
				try{
					CFile::Rename(m_sPath+oldFileName, m_sPath+(TCHAR*)(LPCTSTR)newName);//�̸� �ٲ� ���� ��ü ��ΰ� �־����
				}catch(CFileException* pEx){					
					printf("File %20s not found, cause = %d\n", fd.cFileName, pEx->m_cause);
					pEx->Delete();
				}
			}								
		} while (FindNextFile(hFind, &fd));		
		FindClose(hFind);//handle ��ȯ
		//m_fp.Close();	//���� ���� 
		return success;
	}
	return fail;
}



void CTestDlg::OnRootFolderChanged(UINT uID)
{
	/*BOOL bCustomFolder = (uID == IDC_RADIO_CUSTOM);
	if (bCustomFolder == m_bCustomFolder)
		return;
	CString cRootFolder = m_cRootFolder;
	if (uID == IDC_RADIO_CUSTOM)
	{
		CSelRootFolderDlg cDlg;
		cDlg.m_cRootFolder = m_cRootFolder;
		if (cDlg.DoModal() != IDOK)
		{
			int nID = bCustomFolder ? IDC_RADIO_DEFAULT : IDC_RADIO_CUSTOM;
			CheckRadioButton(IDC_RADIO_DEFAULT, IDC_RADIO_CUSTOM, nID);
			return;
		}
		cRootFolder = m_cRootFolder = cDlg.m_cRootFolder;
	}
	else
		cRootFolder.Empty();
	SetDlgItemText(IDC_EDIT_ROOTFOLDER, cRootFolder);
	m_cTreeCtrl.SetRootFolder(cRootFolder);
	m_bCustomFolder = bCustomFolder;*/
}

void CTestDlg::OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	HTREEITEM hItem = m_cTreeCtrl.GetSelectedItem();
	CString cFolderPath;
	if (m_cTreeCtrl.GetItemPath(cFolderPath, hItem))
	{
		// Retrieving an existing (or adding a new: bAddIfNotFound = TRUE) project files array
		CStringArray *pFilesArr = m_cProjFileMap.GetFiles(cFolderPath, TRUE);
		// and using the resultant pointer as the selected tree item data
		m_cTreeCtrl.SetItemDataEx(hItem, (DWORD_PTR)pFilesArr);
		// Refreshing both list controls contents		
		m_cListCtrlRem.DisplayFolder(cFolderPath);
	}
}

void CTestDlg::OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// �� ����� ����Ϸ��� Windows Vista �̻��� �־�� �մϴ�.
	// _WIN32_WINNT ��ȣ�� 0x0600���� ũ�ų� ���ƾ� �մϴ�.
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	if (zDelta < 0) {//���콺�� down
		//_fView
	}
	else {//���콺�� up``

	}
	CDialogEx::OnMouseHWheel(nFlags, zDelta, pt);
}



//�ķ�Ʈ ����
void CTestDlg::OnBnClickedButton6()
{	 
	
	if(PaletteChange1bpp() >0){
		MessageBox(L"�ķ�Ʈ,������ ���� ����");
		ShellExecute(NULL, L"open", L"explorer", RESULT_PATH, NULL, SW_SHOW);
	}else{
		MessageBox(L"���� �߻�");
	}
}


int CTestDlg::PaletteChange1bpp()
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
				m_FileClass->PaletteChange(m_sPath+tempFileName, tempFileName);
			}								
		} while (FindNextFile(hFind, &fd));		
		FindClose(hFind);//handle ��ȯ		
		return success;
	}
	return fail;
}