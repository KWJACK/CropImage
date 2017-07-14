
// TestDlg.h : ��� ����
//

#pragma once
#include <iostream>
#include "StdioFileEx.h"
#include "FileOpenClass.h"
#include "MFCShellTreeCtrlEx.h"
#include "ProjectListCtrl.h"
#include "ProjFilesArray.h"
#include "PreviewDlg.h"
#include "afxwin.h"
#include "EditDlg.h"

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
	int elimanate2Char();
	int PaletteChange1bpp();
	int m_wordLength;
	
	CStdioFileEx m_fp;	//MFC �ؽ�Ʈ ���� Ŭ����
	FileOpenClass* m_FileClass;	//BMP ���� ���� Ŭ����
	CString m_sTmp;
	CString m_sPath;

	//ListCtrl, TreeCtrl
	BOOL m_bCustomFolder;
	CMFCShellTreeCtrlEx m_cTreeCtrl;
	CProjectListCtrl m_cListCtrlRem;
	CProjFilesArray m_cProjFileMap;
	CString m_cRootFolder;

	//PreviewImage
	CPreviewDlg*	m_pPreviewDlg;

	//EditDlg ���� 
	CButton m_name_check;	//üũ �ڽ�
	CEditDlg m_editDlg;
	//BOOL IsImageGDIPLUSValid( CString filePath );
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

	afx_msg void OnRootFolderChanged(UINT uID);
	afx_msg void OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();	
	
	
};
