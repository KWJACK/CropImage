


// MainFrm.h : CMainFrame Ŭ������ �������̽�
//#pragma once
#include "FormListDlg.h"
#include "FormBMP.h"
class CMainFrame : public CFrameWnd
{	
public:
	CMainFrame();
	virtual ~CMainFrame();
protected: 
	DECLARE_DYNAMIC(CMainFrame)

	//ListCtrl, TreeCtrl
	BOOL m_bCustomFolder;
	FormListDlg* m_pFormListDlg;
	FormBMP* m_pFormBMP;
	CString m_cRootFolder;
// �������Դϴ�.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
// �����Դϴ�.
public:
	CToolBar          m_wndToolBar;
	CStatusBar        m_wndStatusBar;//��Ʈ�� �ȼ� ���� ����� ���� status bar
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // ��Ʈ�� ������ ���Ե� ����Դϴ�.	
	CSplitterWnd m_wndSplitter, m_wndSplitter2, m_wndSplitter3;

// ������ �޽��� �� �Լ�
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);	
	afx_msg void OnRootFolderChanged(UINT uID);
	DECLARE_MESSAGE_MAP()

	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual void DoDataExchange(CDataExchange* pDX);
	afx_msg LRESULT OnSendListdlg(WPARAM wParam, LPARAM lParam);
};


