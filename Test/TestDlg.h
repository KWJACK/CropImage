
// TestDlg.h : 헤더 파일
//

#pragma once
#include <iostream>
#include "StdioFileEx.h"
#include "FileOpenClass.h"
enum { success= 1, fail= -1};
// CTestDlg 대화 상자
class CTestDlg : public CDialogEx
{

// 생성입니다.
public:
	CTestDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_TEST_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.
public:	
	int changeFileName();
	int checkFileName();
	int makeFileNmae();
	int makeBinary();
	int m_wordLength;
	CStdioFileEx m_fp;	//MFC 텍스트 파일 클래스
	FileOpenClass* m_FileClass;	//BMP 파일 관리 클래스
	CString m_sTmp;
	CString m_sPath;
// 구현입니다.

protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnDestroy();
};
