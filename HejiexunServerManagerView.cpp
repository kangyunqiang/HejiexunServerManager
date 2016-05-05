
// HejiexunServerManagerView.cpp : CHejiexunServerManagerView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "HejiexunServerManager.h"
#endif

#include "HejiexunServerManagerDoc.h"
#include "HejiexunServerManagerView.h"
#include "EditInfoDlg.h"
#include "ProcessManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const char * TEXT_STOP = "停止";
const char * TEXT_RUN = "运行";
const char * TEXT_MANUAL = "手动";
const char * TEXT_AUTO = "自动";
const char * TEXT_SHOW = "前台";
const char * TEXT_HIDE = "后台";

//////////////////////////////////////////////////////////////////////////
//应用逻辑处理线程函数
DWORD WINAPI ThreadProc(LPVOID lpParam);
//////////////////////////////////////////////////////////////////////////

// CHejiexunServerManagerView

IMPLEMENT_DYNCREATE(CHejiexunServerManagerView, CListView)

BEGIN_MESSAGE_MAP(CHejiexunServerManagerView, CListView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_BUTTON_START, &CHejiexunServerManagerView::OnButtonStart)
	ON_COMMAND(ID_BUTTON_STOP, &CHejiexunServerManagerView::OnButtonStop)
	ON_COMMAND(ID_BUTTON_ADD, &CHejiexunServerManagerView::OnButtonAdd)
	ON_COMMAND(ID_BUTTON_DELETE, &CHejiexunServerManagerView::OnButtonDelete)
	ON_COMMAND(ID_BUTTON_MODIFY, &CHejiexunServerManagerView::OnButtonModify)
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CHejiexunServerManagerView 构造/析构
#define BUFFER_SIZE 1000
CHejiexunServerManagerView::CHejiexunServerManagerView()
{
	hThread = INVALID_HANDLE_VALUE;
	char buffer[BUFFER_SIZE];
	GetModuleFileName(NULL, buffer, BUFFER_SIZE);
	configFilePath = buffer;
	configFilePath = configFilePath.Left(configFilePath.ReverseFind('\\'));
	configFilePath += "\\HejiexunServerManagerConfig.ini";
}

CHejiexunServerManagerView::~CHejiexunServerManagerView()
{
}

BOOL CHejiexunServerManagerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CListView::PreCreateWindow(cs);
}

void CHejiexunServerManagerView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();

	//初始化列表
	InitListCtrl();
	//加载配置信息
	LoadConfigInfo();
	//启动监控
	SetTimer(1234, 60000, NULL);
}


void CHejiexunServerManagerView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CHejiexunServerManagerView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CHejiexunServerManagerView 诊断

#ifdef _DEBUG
void CHejiexunServerManagerView::AssertValid() const
{
	CListView::AssertValid();
}

void CHejiexunServerManagerView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CHejiexunServerManagerDoc* CHejiexunServerManagerView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CHejiexunServerManagerDoc)));
	return (CHejiexunServerManagerDoc*)m_pDocument;
}


#endif //_DEBUG

void CHejiexunServerManagerView::InitListCtrl()
{
	//  从而可以用项填充 ListView。
	CListCtrl & listCtrl = GetListCtrl();

	LONG lStyle = GetWindowLong(listCtrl.GetSafeHwnd(), GWL_STYLE);//获取当前窗口类型
	lStyle &= ~LVS_TYPEMASK; //清除显示方式位
	lStyle |= LVS_REPORT; //设置显示方式
	SetWindowLong(listCtrl.GetSafeHwnd(), GWL_STYLE, lStyle);//设置窗口类型
	listCtrl.SetExtendedStyle( LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

	const char * arCols[] = {"名称",	"描述",	"映像名称", "启动类型", "显示方式", "路径", "状态"};
	const int colWidth[] = {100, 150, 100, 80, 80, 200, 100};
	for (int nCol = 0; nCol < 7; nCol++)
	{
		listCtrl.InsertColumn(nCol, (LPCTSTR)arCols[nCol], LVCFMT_CENTER, colWidth[nCol]);
	}
}

// CHejiexunServerManagerView 消息处理程序

void CHejiexunServerManagerView::OnButtonStart()
{
	CListCtrl &listCtrl = GetListCtrl();
	int iIndex = -1;
	POSITION pos = listCtrl.GetFirstSelectedItemPosition(); //pos选中的首行位置
	if (pos == NULL)
	{
		AfxMessageBox("请选择要启动的条目!");
		return;
	}
	else
	{
		while (pos != NULL)   //如果你选择多行
		{
			iIndex = listCtrl.GetNextSelectedItem(pos);
			if (iIndex >=0 && iIndex < listCtrl.GetItemCount())
			{
				StartProcess(iIndex);
			}
		}
	}
}

void CHejiexunServerManagerView::OnButtonStop()
{
	CListCtrl &listCtrl = GetListCtrl();
	int iIndex = -1;
	POSITION pos = listCtrl.GetFirstSelectedItemPosition(); //pos选中的首行位置
	if (pos == NULL)
	{
		AfxMessageBox("请选择要停止的条目!");
		return;
	}
	else
	{
		while (pos != NULL)   //如果你选择多行
		{
			iIndex = listCtrl.GetNextSelectedItem(pos);
			if (iIndex >=0 && iIndex < listCtrl.GetItemCount())
			{
				StopProcess(iIndex);
			}
		}
	}
}

void CHejiexunServerManagerView::OnButtonAdd()
{
	CEditInfoDlg editInfoDlg;
	if (editInfoDlg.DoModal() == IDOK)
	{
		CListCtrl &listCtrl = GetListCtrl();
		int nItem = listCtrl.InsertItem(listCtrl.GetItemCount(), editInfoDlg.name);
		listCtrl.SetItemText(nItem, 1, editInfoDlg.description);
		listCtrl.SetItemText(nItem, 2, editInfoDlg.path.Right(editInfoDlg.path.GetLength() - editInfoDlg.path.ReverseFind('\\') - 1));
		listCtrl.SetItemText(nItem, 3, editInfoDlg.startType);
		listCtrl.SetItemText(nItem, 4, editInfoDlg.showType);
		listCtrl.SetItemText(nItem, 5, editInfoDlg.path);
		listCtrl.SetItemText(nItem, 6, TEXT_STOP);

		SaveConfigInfo();
	}
}

void CHejiexunServerManagerView::OnButtonDelete()
{
	CListCtrl &listCtrl = GetListCtrl();
	int iIndex = -1;
	POSITION pos = listCtrl.GetFirstSelectedItemPosition(); //pos选中的首行位置
	if (pos == NULL)
	{
		AfxMessageBox("请选择要删除的记录!");
		return;
	}
	else
	{
		if (AfxMessageBox("确定要删除吗？", MB_OKCANCEL) != IDOK)
		{
			return;
		}
		while (pos != NULL)   //如果你选择多行
		{
			iIndex = listCtrl.GetNextSelectedItem(pos);
			if (iIndex >=0 && iIndex < listCtrl.GetItemCount())
			{
				listCtrl.DeleteItem(iIndex);
			}
			pos = listCtrl.GetFirstSelectedItemPosition();
		}
		SaveConfigInfo();
	}
}


void CHejiexunServerManagerView::OnButtonModify()
{
	// TODO: 在此添加命令处理程序代码
	CListCtrl &listCtrl = GetListCtrl();
	if (listCtrl.GetSelectedCount() != 1)
	{
		AfxMessageBox("请选择一行进行修改!");
		return;
	}
	int iIndex = -1;
	POSITION pos = listCtrl.GetFirstSelectedItemPosition(); //pos选中的首行位置
	if (pos == NULL)
	{
		AfxMessageBox("请选择一行进行修改!");
		return;
	}
	else
	{
		if (pos != NULL)   //如果你选择多行
		{
			iIndex = listCtrl.GetNextSelectedItem(pos);
			if (iIndex >=0 && iIndex < listCtrl.GetItemCount())
			{
				CEditInfoDlg editInfoDlg;
				editInfoDlg.name = listCtrl.GetItemText(iIndex, 0);
				editInfoDlg.description = listCtrl.GetItemText(iIndex, 1);
				editInfoDlg.startType = listCtrl.GetItemText(iIndex, 3);
				editInfoDlg.showType = listCtrl.GetItemText(iIndex, 4);
				editInfoDlg.path = listCtrl.GetItemText(iIndex, 5);
				if (editInfoDlg.DoModal() == IDOK)
				{
					listCtrl.SetItemText(iIndex, 0, editInfoDlg.name);
					listCtrl.SetItemText(iIndex, 1, editInfoDlg.description);
					listCtrl.SetItemText(iIndex, 2, editInfoDlg.path.Right(editInfoDlg.path.GetLength() - editInfoDlg.path.ReverseFind('\\') - 1));
					listCtrl.SetItemText(iIndex, 3, editInfoDlg.startType);
					listCtrl.SetItemText(iIndex, 4, editInfoDlg.showType);
					listCtrl.SetItemText(iIndex, 5, editInfoDlg.path);

					SaveConfigInfo();
				}
			}
		}
	}
}

void CHejiexunServerManagerView::StartProcess(int iIndex)
{
	CListCtrl &listCtrl = GetListCtrl();
	char curState[10];
	listCtrl.GetItemText(iIndex, 6, curState, 10);
	if (strcmp(curState, TEXT_RUN) == 0)
	{
		AfxMessageBox("该程序已经运行");
		return;
	}

	CString processPath = listCtrl.GetItemText(iIndex, 5);
	CString processName = listCtrl.GetItemText(iIndex, 2).Trim();
	if (ProcessManager::GetProcessState(processName) == STATE_RUN)
	{
		listCtrl.SetItemText(iIndex, 6, TEXT_RUN);
		return;
	}
	char showType[100];
	listCtrl.GetItemText(iIndex, 4, showType, 100);
	BOOL bShow = TRUE;
	if (strcmp(showType, TEXT_SHOW) == 0)
	{ 
		bShow = TRUE;
	}
	else
	{
		bShow = FALSE;
	}
	if (ProcessManager::StartProcess(processPath, bShow))
	{
		listCtrl.SetItemText(iIndex, 6, TEXT_RUN);
	}
	else
	{
		AfxMessageBox("启动程序失败，请检查路径是否正确");
		return;
	}
}

void CHejiexunServerManagerView::StopProcess(int iIndex)
{
	CListCtrl &listCtrl = GetListCtrl();
	char curState[10];
	listCtrl.GetItemText(iIndex, 6, curState, 10);
	if (strcmp(curState, TEXT_STOP) == 0)
	{
		AfxMessageBox("该程序已经停止运行");
		return;
	}
	CString processName = listCtrl.GetItemText(iIndex, 2).Trim();
	if (ProcessManager::GetProcessState(processName) == STATE_STOP)
	{
		listCtrl.SetItemText(iIndex, 6, TEXT_STOP);
		return;
	}
	if (ProcessManager::StopProcess(processName))
	{
		listCtrl.SetItemText(iIndex, 6, TEXT_STOP);
		return;
	}
	else
	{
		AfxMessageBox("停止程序失败，请检查路径是否正确");
		return;
	}
}

bool CHejiexunServerManagerView::LoadConfigInfo()
{
	CListCtrl &listCtrl = GetListCtrl();

	CString appName;
	int itemIndex = 1;
	char buffer[1000];
	int nItem = 0;
	do 
	{
		appName.Format("记录%d", itemIndex++);
		GetPrivateProfileString(appName, "名称", "", buffer, BUFFER_SIZE, configFilePath);
		if ((GetLastError() != ERROR_SUCCESS) || (strlen(buffer) <= 0))
		{
			break;
		}
		nItem = listCtrl.InsertItem(listCtrl.GetItemCount(), buffer);

		GetPrivateProfileString(appName, "描述", "", buffer, BUFFER_SIZE, configFilePath);
		if ((GetLastError() != ERROR_SUCCESS) || (strlen(buffer) <= 0))
		{
			break;
		}
		listCtrl.SetItemText(nItem, 1, buffer);

		GetPrivateProfileString(appName, "映像名称", "", buffer, BUFFER_SIZE, configFilePath);
		if ((GetLastError() != ERROR_SUCCESS) || (strlen(buffer) <= 0))
		{
			break;
		}
		listCtrl.SetItemText(nItem, 2, buffer);

		GetPrivateProfileString(appName, "启动类型", "", buffer, BUFFER_SIZE, configFilePath);
		if ((GetLastError() != ERROR_SUCCESS) || (strlen(buffer) <= 0))
		{
			break;
		}
		listCtrl.SetItemText(nItem, 3, buffer);

		GetPrivateProfileString(appName, "显示方式", "", buffer, BUFFER_SIZE, configFilePath);
		if ((GetLastError() != ERROR_SUCCESS) || (strlen(buffer) <= 0))
		{
			break;
		}
		listCtrl.SetItemText(nItem, 4, buffer);

		GetPrivateProfileString(appName, "路径", "", buffer, BUFFER_SIZE, configFilePath);
		if ((GetLastError() != ERROR_SUCCESS) || (strlen(buffer) <= 0))
		{
			break;
		}
		listCtrl.SetItemText(nItem, 5, buffer);

	} while (GetLastError() == ERROR_SUCCESS && itemIndex <= 100);

	return listCtrl.GetItemCount() > 0;
}

void CHejiexunServerManagerView::SaveConfigInfo()
{
	//删除文件从而清空所有内容，否则会出现编号错误
	DeleteFile(configFilePath);
	//重新写入连接信息
	int driverIndex = 1;
	CString appName;
	CListCtrl &listCtrl = GetListCtrl();
	char buffer[BUFFER_SIZE];
	for (int iIndex = 0; iIndex < listCtrl.GetItemCount(); iIndex++)
	{
		appName.Format("记录%d", driverIndex++);
		listCtrl.GetItemText(iIndex, 0, buffer, BUFFER_SIZE);
		WritePrivateProfileString(appName, "名称", buffer, configFilePath);
		listCtrl.GetItemText(iIndex, 1, buffer, BUFFER_SIZE);
		WritePrivateProfileString(appName, "描述", buffer, configFilePath);
		listCtrl.GetItemText(iIndex, 2, buffer, BUFFER_SIZE);
		WritePrivateProfileString(appName, "映像名称", buffer, configFilePath);
		listCtrl.GetItemText(iIndex, 3, buffer, BUFFER_SIZE);
		WritePrivateProfileString(appName, "启动类型", buffer, configFilePath);
		listCtrl.GetItemText(iIndex, 4, buffer, BUFFER_SIZE);
		WritePrivateProfileString(appName, "显示方式", buffer, configFilePath);
		listCtrl.GetItemText(iIndex, 5, buffer, BUFFER_SIZE);
		WritePrivateProfileString(appName, "路径", buffer, configFilePath);
	}
}

// CHejiexunServerMonitorView 消息处理程序
void CHejiexunServerManagerView::OnStartMonitor()
{
	if (hThread != INVALID_HANDLE_VALUE)
	{
		TerminateThread(hThread, 0);
		CloseHandle(hThread);
	}

	hThread = CreateThread(NULL, 0, ThreadProc, (LPVOID)this, 0, &dwThread);

	if(hThread == NULL)
	{
		AfxMessageBox("启动业务线程失败，请稍后重试!");
		return;
	}
}

DWORD WINAPI ThreadProc(LPVOID lpParam)
{
	if (NULL == lpParam)
	{
		return -1;
	}

	CString processName;
	SYSTEMTIME systemTime;
	GetLocalTime(&systemTime);
	WORD wDay = systemTime.wDay;
	CListCtrl &listCtrl =  ((CHejiexunServerManagerView *)lpParam)->GetListCtrl();
	
	while (TRUE) 
	{
	//	GetLocalTime(&systemTime);
		for (int iIndex = 0; iIndex < listCtrl.GetItemCount(); iIndex++)
		{
			processName = listCtrl.GetItemText(iIndex, 2).Trim();
			ProcessState processState = ProcessManager::GetProcessState(processName);
			switch (processState)
			{
			case STATE_STOP:
				{
					listCtrl.SetItemText(iIndex, 6, TEXT_STOP);
					char startType[100];
					listCtrl.GetItemText(iIndex, 3, startType, 100);
					if (strcmp(startType, TEXT_AUTO) == 0)
					{
						char processPath[1000];
						listCtrl.GetItemText(iIndex, 5, processPath, 1000);
						char showType[100];
						listCtrl.GetItemText(iIndex, 4, showType, 100);
						BOOL bShow = TRUE;
						if (strcmp(showType, TEXT_SHOW) == 0)
						{ 
							bShow = TRUE;
						}
						else
						{
							bShow = FALSE;
						}
						if (!ProcessManager::StartProcess(processPath, bShow))
						{
							char name[100];
							listCtrl.GetItemText(iIndex, 0, name, 100);
							CString message;
							message.Format("启动%s程序失败", name);
							AfxMessageBox(message);
						}
						else
						{
							listCtrl.SetItemText(iIndex, 6, TEXT_RUN);
						}
					}
				}
				break;
			case STATE_RUN:
				listCtrl.SetItemText(iIndex, 6, TEXT_RUN);
				/*if (systemTime.wDay != wDay)
				{
				if (ProcessManager::StopProcess(processName))
				{
				listCtrl.SetItemText(iIndex, 6, TEXT_STOP);
				}
				}*/
				
				break;
			case STATE_SUSPEND:
				break;
			case STATE_UNKNOWN:
				break;
			default:
				break;
			}
		}
		wDay = systemTime.wDay;
		Sleep(5000);
	}
	return 0;
}



void CHejiexunServerManagerView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == 1234)
	{
		OnStartMonitor();
		KillTimer(nIDEvent);
	}
	CListView::OnTimer(nIDEvent);
}
