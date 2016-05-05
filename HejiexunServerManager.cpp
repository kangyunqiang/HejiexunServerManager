
// HejiexunServerManager.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "HejiexunServerManager.h"
#include "MainFrm.h"

#include "HejiexunServerManagerDoc.h"
#include "HejiexunServerManagerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CHejiexunServerManagerApp

BEGIN_MESSAGE_MAP(CHejiexunServerManagerApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CHejiexunServerManagerApp::OnAppAbout)
	// �����ļ��ı�׼�ĵ�����
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	ON_COMMAND(ID_SHOW_WINDOW, &CHejiexunServerManagerApp::OnShowWindow)
	ON_COMMAND(ID_AUTO_RUN, &CHejiexunServerManagerApp::OnAutoRun)
	ON_UPDATE_COMMAND_UI(ID_AUTO_RUN, &CHejiexunServerManagerApp::OnUpdateAutoRun)
END_MESSAGE_MAP()


// CHejiexunServerManagerApp ����

CHejiexunServerManagerApp::CHejiexunServerManagerApp()
{
	m_bHiColorIcons = TRUE;

	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// ���Ӧ�ó��������ù�����������ʱ֧��(/clr)�����ģ���:
	//     1) �����д˸������ã�������������������֧�ֲ�������������
	//     2) ��������Ŀ�У������밴������˳���� System.Windows.Forms ������á�
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: ������Ӧ�ó��� ID �ַ����滻ΪΨһ�� ID �ַ�����������ַ�����ʽ
	//Ϊ CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("HejiexunServerManager.AppID.NoVersion"));

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}

// Ψһ��һ�� CHejiexunServerManagerApp ����

CHejiexunServerManagerApp theApp;


// CHejiexunServerManagerApp ��ʼ��

BOOL CHejiexunServerManagerApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();


	// ��ʼ�� OLE ��
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// ʹ�� RichEdit �ؼ���Ҫ  AfxInitRichEdit2()	
	// AfxInitRichEdit2();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));
	LoadStdProfileSettings(4);  // ���ر�׼ INI �ļ�ѡ��(���� MRU)


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// ע��Ӧ�ó�����ĵ�ģ�塣�ĵ�ģ��
	// �������ĵ�����ܴ��ں���ͼ֮�������
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CHejiexunServerManagerDoc),
		RUNTIME_CLASS(CMainFrame),       // �� SDI ��ܴ���
		RUNTIME_CLASS(CHejiexunServerManagerView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// ������׼ shell ���DDE�����ļ�������������
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// ���á�DDE ִ�С�
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);


	// ��������������ָ����������
	// �� /RegServer��/Register��/Unregserver �� /Unregister ����Ӧ�ó����򷵻� FALSE��
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// Ψһ��һ�������ѳ�ʼ���������ʾ����������и���
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// �������к�׺ʱ�ŵ��� DragAcceptFiles
	//  �� SDI Ӧ�ó����У���Ӧ�� ProcessShellCommand ֮����
	// ������/��
	m_pMainWnd->DragAcceptFiles();
	return TRUE;
}

int CHejiexunServerManagerApp::ExitInstance()
{
	//TODO: �����������ӵĸ�����Դ
	AfxOleTerm(FALSE);

	return CWinAppEx::ExitInstance();
}

// CHejiexunServerManagerApp ��Ϣ�������


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// �������жԻ����Ӧ�ó�������
void CHejiexunServerManagerApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CHejiexunServerManagerApp �Զ������/���淽��

void CHejiexunServerManagerApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
	bNameValid = strName.LoadString(IDS_EXPLORER);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EXPLORER);
}

void CHejiexunServerManagerApp::LoadCustomState()
{
}

void CHejiexunServerManagerApp::SaveCustomState()
{
}

//��������
BOOL  CHejiexunServerManagerApp::CreateAutoRun()
{
	//������´���
	HKEY   hKey; 
	char pFileName[MAX_PATH] = {0}; 
	//�õ����������ȫ·�� 
	DWORD dwRet = GetModuleFileName(NULL, (LPSTR)pFileName, MAX_PATH); 
	//�ҵ�ϵͳ�������� 
	LPCTSTR lpRun = "Software\\Microsoft\\Windows\\CurrentVersion\\Run"; 
	//��������Key 
	long lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpRun, 0, KEY_WRITE, &hKey); 
	if (lRet == ERROR_SUCCESS)
	{
		//���ע��
		if (RegSetValueEx(hKey, "HejiexunServerManager", 0, REG_SZ, (const BYTE*)(LPCSTR)pFileName, MAX_PATH) != ERROR_SUCCESS)
		{
			RegCloseKey(hKey); 
			return FALSE;
		}
		RegCloseKey(hKey); 
	}
	return TRUE;
}
//ȡ����������
BOOL CHejiexunServerManagerApp::DeleteAutoRun()
{
	//������´���
	HKEY   hKey; 
	char pFileName[MAX_PATH] = {0}; 
	//�õ����������ȫ·�� 
	DWORD dwRet = GetModuleFileName(NULL, (LPSTR)pFileName, MAX_PATH); 
	//�ҵ�ϵͳ�������� 
	LPCTSTR lpRun = "Software\\Microsoft\\Windows\\CurrentVersion\\Run"; 
	//��������Key 
	long lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpRun, 0, KEY_WRITE, &hKey); 
	if (lRet == ERROR_SUCCESS)
	{
		//ɾ��ע��
		if (RegDeleteValue(hKey, "HejiexunServerManager") != ERROR_SUCCESS)
		{
			RegCloseKey(hKey); 
			return FALSE;
		}
		RegCloseKey(hKey); 
	}
	return TRUE;;
}
// CHejiexunServerManagerApp ��Ϣ�������


BOOL CHejiexunServerManagerApp::CheckAutoRun()
{
	DWORD dType, dSize = 512;
	HKEY hKey;
	BYTE byData[512];
	//�򿪹ر�ע���---------------------------------------------------------------
	LPCTSTR SubKey="SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run";
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, SubKey, 0, KEY_ALL_ACCESS, &hKey) != ERROR_SUCCESS)
	{
		return FALSE;
	}

	if (RegQueryValueEx(hKey, "HejiexunServerManager", NULL, &dType, byData, &dSize) != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return FALSE;
	}
	RegCloseKey(hKey);
	return TRUE;
}


void CHejiexunServerManagerApp::OnShowWindow()
{
	m_pMainWnd->ShowWindow(SW_NORMAL);
	m_pMainWnd->UpdateWindow();
}


void CHejiexunServerManagerApp::OnAutoRun()
{
	if (!CheckAutoRun())
	{
		CreateAutoRun();
	}
	else
	{
		DeleteAutoRun();
	}
}


void CHejiexunServerManagerApp::OnUpdateAutoRun(CCmdUI *pCmdUI)
{
	if (CheckAutoRun())
	{
		pCmdUI->SetCheck(TRUE);
	}
	else
	{
		pCmdUI->SetCheck(FALSE);
	}
}
