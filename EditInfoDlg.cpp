// EditInfoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "HejiexunServerManager.h"
#include "EditInfoDlg.h"
#include "afxdialogex.h"


// CEditInfoDlg 对话框

IMPLEMENT_DYNAMIC(CEditInfoDlg, CDialogEx)

CEditInfoDlg::CEditInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEditInfoDlg::IDD, pParent)
	, name(_T(""))
	, description(_T(""))
	, startType(_T(""))
	, path(_T(""))
	, showType(_T(""))
{

}

CEditInfoDlg::~CEditInfoDlg()
{
}

void CEditInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NAME, name);
	DDV_MaxChars(pDX, name, 100);
	DDX_Text(pDX, IDC_EDIT_DESCRIPTION, description);
	DDV_MaxChars(pDX, description, 100);
	DDX_CBString(pDX, IDC_COMBO_START_TYPE, startType);
	DDX_Text(pDX, IDC_EDIT_PATH, path);
	DDV_MaxChars(pDX, path, 1000);
	DDX_Control(pDX, IDC_EDIT_PATH, editPath);
	DDX_CBString(pDX, IDC_COMBO_SHOW_TYPE, showType);
}


BEGIN_MESSAGE_MAP(CEditInfoDlg, CDialogEx)
	ON_BN_CLICKED(IDC_MFCBUTTON_OK, &CEditInfoDlg::OnBnClickedMfcbuttonOk)
	ON_BN_CLICKED(IDC_MFCBUTTON_CANCEL, &CEditInfoDlg::OnBnClickedMfcbuttonCancel)
	ON_BN_CLICKED(IDC_MFCBUTTON_BROWSE, &CEditInfoDlg::OnBnClickedMfcbuttonBrowse)
END_MESSAGE_MAP()


// CEditInfoDlg 消息处理程序


void CEditInfoDlg::OnBnClickedMfcbuttonOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (name.Trim().GetLength() <= 0)
	{
		AfxMessageBox("请输入名称");
		return;
	}
	if (description.Trim().GetLength() <= 0)
	{
		AfxMessageBox("请输入描述");
		return;
	}
	if (startType.Trim().GetLength() <= 0)
	{
		AfxMessageBox("请选择启动类型");
		return;
	}
	if (showType.Trim().GetLength() <= 0)
	{
		AfxMessageBox("请选择显示方式");
		return;
	}
	if (path.Trim().GetLength() <= 0)
	{
		AfxMessageBox("请输入程序路径");
		return;
	}
	CDialogEx::OnOK();
}


void CEditInfoDlg::OnBnClickedMfcbuttonCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


void CEditInfoDlg::OnBnClickedMfcbuttonBrowse()
{
	CFileDialog fileDialog(TRUE, 
		NULL, 
		NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT | OFN_ENABLESIZING,
		_T("exe文件|*.exe|所有文件|*.*||"),
		NULL);
	if (fileDialog.DoModal() == IDOK)
	{
		editPath.SetWindowTextA(fileDialog.GetPathName());
	}
}
