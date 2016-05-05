// EditInfoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HejiexunServerManager.h"
#include "EditInfoDlg.h"
#include "afxdialogex.h"


// CEditInfoDlg �Ի���

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


// CEditInfoDlg ��Ϣ�������


void CEditInfoDlg::OnBnClickedMfcbuttonOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if (name.Trim().GetLength() <= 0)
	{
		AfxMessageBox("����������");
		return;
	}
	if (description.Trim().GetLength() <= 0)
	{
		AfxMessageBox("����������");
		return;
	}
	if (startType.Trim().GetLength() <= 0)
	{
		AfxMessageBox("��ѡ����������");
		return;
	}
	if (showType.Trim().GetLength() <= 0)
	{
		AfxMessageBox("��ѡ����ʾ��ʽ");
		return;
	}
	if (path.Trim().GetLength() <= 0)
	{
		AfxMessageBox("���������·��");
		return;
	}
	CDialogEx::OnOK();
}


void CEditInfoDlg::OnBnClickedMfcbuttonCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}


void CEditInfoDlg::OnBnClickedMfcbuttonBrowse()
{
	CFileDialog fileDialog(TRUE, 
		NULL, 
		NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT | OFN_ENABLESIZING,
		_T("exe�ļ�|*.exe|�����ļ�|*.*||"),
		NULL);
	if (fileDialog.DoModal() == IDOK)
	{
		editPath.SetWindowTextA(fileDialog.GetPathName());
	}
}
