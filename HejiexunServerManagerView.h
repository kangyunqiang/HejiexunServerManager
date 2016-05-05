
// HejiexunServerManagerView.h : CHejiexunServerManagerView ��Ľӿ�
//

#pragma once
#include "HejiexunServerManagerDoc.h"

extern const char * TEXT_STOP;
extern const char * TEXT_RUN;
extern const char * TEXT_MANUAL;
extern const char * TEXT_AUTO;
extern const char * TEXT_SHOW;
extern const char * TEXT_HIDE;

class CHejiexunServerManagerView : public CListView
{
protected: // �������л�����
	CHejiexunServerManagerView();
	DECLARE_DYNCREATE(CHejiexunServerManagerView)

// ����
public:
	CHejiexunServerManagerDoc* GetDocument() const;
// ����
private:
	bool LoadConfigInfo();
	void SaveConfigInfo();
	void OnStartMonitor();

	HANDLE hThread;
	DWORD dwThread;
	CString configFilePath;

// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // ������һ�ε���

	void InitListCtrl();

// ʵ��
public:
	virtual ~CHejiexunServerManagerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnButtonStart();
	afx_msg void OnButtonStop();
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonModify();
	afx_msg void OnButtonDelete();
	void StartProcess(int iIndex);
	void StopProcess(int iIndex);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

#ifndef _DEBUG  // HejiexunServerManagerView.cpp �еĵ��԰汾
inline CHejiexunServerManagerDoc* CHejiexunServerManagerView::GetDocument() const
   { return reinterpret_cast<CHejiexunServerManagerDoc*>(m_pDocument); }
#endif

