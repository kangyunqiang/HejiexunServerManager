
// HejiexunServerManager.cpp : 定义应用程序的类行为。
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
	// 基于文件的标准文档命令
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	ON_COMMAND(ID_SHOW_WINDOW, &CHejiexunServerManagerApp::OnShowWindow)
	ON_COMMAND(ID_AUTO_RUN, &CHejiexunServerManagerApp::OnAutoRun)
	ON_UPDATE_COMMAND_UI(ID_AUTO_RUN, &CHejiexunServerManagerApp::OnUpdateAutoRun)
END_MESSAGE_MAP()


// CHejiexunServerManagerApp 构造

CHejiexunServerManagerApp::CHejiexunServerManagerApp()
{
	m_bHiColorIcons = TRUE;

	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// 如果应用程序是利用公共语言运行时支持(/clr)构建的，则:
	//     1) 必须有此附加设置，“重新启动管理器”支持才能正常工作。
	//     2) 在您的项目中，您必须按照生成顺序向 System.Windows.Forms 添加引用。
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: 将以下应用程序 ID 字符串替换为唯一的 ID 字符串；建议的字符串格式
	//为 CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("HejiexunServerManager.AppID.NoVersion"));

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}

// 唯一的一个 CHejiexunServerManagerApp 对象

CHejiexunServerManagerApp theApp;


// CHejiexunServerManagerApp 初始化

BOOL CHejiexunServerManagerApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();


	// 初始化 OLE 库
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// 使用 RichEdit 控件需要  AfxInitRichEdit2()	
	// AfxInitRichEdit2();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));
	LoadStdProfileSettings(4);  // 加载标准 INI 文件选项(包括 MRU)


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// 注册应用程序的文档模板。文档模板
	// 将用作文档、框架窗口和视图之间的连接
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CHejiexunServerManagerDoc),
		RUNTIME_CLASS(CMainFrame),       // 主 SDI 框架窗口
		RUNTIME_CLASS(CHejiexunServerManagerView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// 分析标准 shell 命令、DDE、打开文件操作的命令行
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// 启用“DDE 执行”
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);


	// 调度在命令行中指定的命令。如果
	// 用 /RegServer、/Register、/Unregserver 或 /Unregister 启动应用程序，则返回 FALSE。
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// 唯一的一个窗口已初始化，因此显示它并对其进行更新
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// 仅当具有后缀时才调用 DragAcceptFiles
	//  在 SDI 应用程序中，这应在 ProcessShellCommand 之后发生
	// 启用拖/放
	m_pMainWnd->DragAcceptFiles();
	return TRUE;
}

int CHejiexunServerManagerApp::ExitInstance()
{
	//TODO: 处理可能已添加的附加资源
	AfxOleTerm(FALSE);

	return CWinAppEx::ExitInstance();
}

// CHejiexunServerManagerApp 消息处理程序


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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

// 用于运行对话框的应用程序命令
void CHejiexunServerManagerApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CHejiexunServerManagerApp 自定义加载/保存方法

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

//开机启动
BOOL  CHejiexunServerManagerApp::CreateAutoRun()
{
	//添加以下代码
	HKEY   hKey; 
	char pFileName[MAX_PATH] = {0}; 
	//得到程序自身的全路径 
	DWORD dwRet = GetModuleFileName(NULL, (LPSTR)pFileName, MAX_PATH); 
	//找到系统的启动项 
	LPCTSTR lpRun = "Software\\Microsoft\\Windows\\CurrentVersion\\Run"; 
	//打开启动项Key 
	long lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpRun, 0, KEY_WRITE, &hKey); 
	if (lRet == ERROR_SUCCESS)
	{
		//添加注册
		if (RegSetValueEx(hKey, "HejiexunServerManager", 0, REG_SZ, (const BYTE*)(LPCSTR)pFileName, MAX_PATH) != ERROR_SUCCESS)
		{
			RegCloseKey(hKey); 
			return FALSE;
		}
		RegCloseKey(hKey); 
	}
	return TRUE;
}
//取消开机启动
BOOL CHejiexunServerManagerApp::DeleteAutoRun()
{
	//添加以下代码
	HKEY   hKey; 
	char pFileName[MAX_PATH] = {0}; 
	//得到程序自身的全路径 
	DWORD dwRet = GetModuleFileName(NULL, (LPSTR)pFileName, MAX_PATH); 
	//找到系统的启动项 
	LPCTSTR lpRun = "Software\\Microsoft\\Windows\\CurrentVersion\\Run"; 
	//打开启动项Key 
	long lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpRun, 0, KEY_WRITE, &hKey); 
	if (lRet == ERROR_SUCCESS)
	{
		//删除注册
		if (RegDeleteValue(hKey, "HejiexunServerManager") != ERROR_SUCCESS)
		{
			RegCloseKey(hKey); 
			return FALSE;
		}
		RegCloseKey(hKey); 
	}
	return TRUE;;
}
// CHejiexunServerManagerApp 消息处理程序


BOOL CHejiexunServerManagerApp::CheckAutoRun()
{
	DWORD dType, dSize = 512;
	HKEY hKey;
	BYTE byData[512];
	//打开关闭注册表---------------------------------------------------------------
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
