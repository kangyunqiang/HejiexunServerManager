
// HejiexunServerManagerView.h : CHejiexunServerManagerView 类的接口
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
protected: // 仅从序列化创建
	CHejiexunServerManagerView();
	DECLARE_DYNCREATE(CHejiexunServerManagerView)

// 特性
public:
	CHejiexunServerManagerDoc* GetDocument() const;
// 操作
private:
	bool LoadConfigInfo();
	void SaveConfigInfo();
	void OnStartMonitor();

	HANDLE hThread;
	DWORD dwThread;
	CString configFilePath;

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // 构造后第一次调用

	void InitListCtrl();

// 实现
public:
	virtual ~CHejiexunServerManagerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
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

#ifndef _DEBUG  // HejiexunServerManagerView.cpp 中的调试版本
inline CHejiexunServerManagerDoc* CHejiexunServerManagerView::GetDocument() const
   { return reinterpret_cast<CHejiexunServerManagerDoc*>(m_pDocument); }
#endif

