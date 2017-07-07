


// MainFrm.h : CMainFrame 클래스의 인터페이스
//#pragma once
#include "FormListDlg.h"
class CMainFrame : public CFrameWnd
{	
public:
	CMainFrame();
protected: 
	DECLARE_DYNAMIC(CMainFrame)

	//ListCtrl, TreeCtrl
	BOOL m_bCustomFolder;
	FormListDlg* m_pFormListDlg;
	
	CString m_cRootFolder;
// 재정의입니다.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

// 구현입니다.
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 컨트롤 모음이 포함된 멤버입니다.	
	CSplitterWnd m_wndSplitter, m_wndSplitter2, m_wndSplitter3;

// 생성된 메시지 맵 함수
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);	
	afx_msg void OnRootFolderChanged(UINT uID);
	DECLARE_MESSAGE_MAP()

	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual void DoDataExchange(CDataExchange* pDX);
	afx_msg LRESULT OnSendListdlg(WPARAM wParam, LPARAM lParam);
};


