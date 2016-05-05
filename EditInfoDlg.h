#pragma once
#include "afxwin.h"


// CEditInfoDlg 对话框

class CEditInfoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CEditInfoDlg)

public:
	CEditInfoDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CEditInfoDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_EDIT_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedMfcbuttonOk();
	afx_msg void OnBnClickedMfcbuttonCancel();
		afx_msg void OnBnClickedMfcbuttonBrowse();

	CString name;
	CString description;
	CString startType;
	CString path;
	CString processName;
	CEdit editPath;
	CString showType;
};
