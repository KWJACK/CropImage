
// EPRE.h : EPRE ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.


// CEPREApp:
// �� Ŭ������ ������ ���ؼ��� EPRE.cpp�� �����Ͻʽÿ�.
//

class CEPREApp : public CWinApp
{
public:
	CEPREApp();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// �����Դϴ�.

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CEPREApp theApp;
