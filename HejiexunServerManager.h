
// HejiexunServerManager.h : HejiexunServerManager Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CHejiexunServerManagerApp:
// �йش����ʵ�֣������ HejiexunServerManager.cpp
//

class CHejiexunServerManagerApp : public CWinAppEx
{
public:
	CHejiexunServerManagerApp();

// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnShowWindow();
	afx_msg void OnAutoRun();

	BOOL CheckAutoRun();
	BOOL CreateAutoRun();
	BOOL DeleteAutoRun();
	afx_msg void OnUpdateAutoRun(CCmdUI *pCmdUI);
};

extern CHejiexunServerManagerApp theApp;
