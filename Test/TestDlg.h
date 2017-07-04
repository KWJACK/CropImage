
// TestDlg.h : ��� ����
//

#pragma once
#include <iostream>
#include "StdioFileEx.h"
#include "FileOpenClass.h"
enum { success= 1, fail= -1};
// CTestDlg ��ȭ ����
class CTestDlg : public CDialogEx
{

// �����Դϴ�.
public:
	CTestDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_TEST_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.
public:	
	int changeFileName();
	int checkFileName();
	int makeFileNmae();
	int makeBinary();
	int m_wordLength;
	CStdioFileEx m_fp;	//MFC �ؽ�Ʈ ���� Ŭ����
	FileOpenClass* m_FileClass;	//BMP ���� ���� Ŭ����
	CString m_sTmp;
	CString m_sPath;
// �����Դϴ�.

protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
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
