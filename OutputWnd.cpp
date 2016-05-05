
#include "stdafx.h"

#include "OutputWnd.h"
#include "Resource.h"
#include "MainFrm.h"
#include "ProcessManager.h"
#include "HejiexunServerManagerView.h"
#include "map"
#include "MessagePipe.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////////////////////////////////////////////
//应用逻辑处理线程函数
DWORD WINAPI OutputThreadProc(LPVOID lpParam);
//////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// COutputBar

COutputWnd::COutputWnd()
{
}

COutputWnd::~COutputWnd()
{
}

BEGIN_MESSAGE_MAP(COutputWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

int COutputWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// 创建选项卡窗口:
	if (!m_wndTabs.Create(CMFCTabCtrl::STYLE_FLAT, rectDummy, this, 1))
	{
		TRACE0("未能创建输出选项卡窗口\n");
		return -1;      // 未能创建
	}

	// 创建输出窗格:
	const DWORD dwStyle = LBS_NOINTEGRALHEIGHT | WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL;

	if (!m_wndOutputState.Create(dwStyle, rectDummy, &m_wndTabs, 2) ||
		!m_wndOutputData.Create(dwStyle, rectDummy, &m_wndTabs, 3) ||
		!m_wndOutputLog.Create(dwStyle, rectDummy, &m_wndTabs, 4))
	{
		TRACE0("未能创建输出窗口\n");
		return -1;      // 未能创建
	}

	UpdateFonts();

	CString strTabName;
	BOOL bNameValid;

	// 将列表窗口附加到选项卡:
	bNameValid = strTabName.LoadString(IDS_BUILD_TAB);
	ASSERT(bNameValid);
	m_wndTabs.AddTab(&m_wndOutputState, strTabName, (UINT)0);
	bNameValid = strTabName.LoadString(IDS_DEBUG_TAB);
	ASSERT(bNameValid);
	m_wndTabs.AddTab(&m_wndOutputData, strTabName, (UINT)1);
	bNameValid = strTabName.LoadString(IDS_FIND_TAB);
	ASSERT(bNameValid);
	m_wndTabs.AddTab(&m_wndOutputLog, strTabName, (UINT)2);

	SetTimer(1, 5000, NULL);
	return 0;
}

void COutputWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// 选项卡控件应覆盖整个工作区:
	m_wndTabs.SetWindowPos (NULL, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
}

void COutputWnd::AdjustHorzScroll(CListBox& wndListBox)
{
	CClientDC dc(this);
	CFont* pOldFont = dc.SelectObject(&afxGlobalData.fontRegular);

	int cxExtentMax = 0;

	for (int i = 0; i < wndListBox.GetCount(); i ++)
	{
		CString strItem;
		wndListBox.GetText(i, strItem);

		cxExtentMax = max(cxExtentMax, (int)dc.GetTextExtent(strItem).cx);
	}

	wndListBox.SetHorizontalExtent(cxExtentMax);
	dc.SelectObject(pOldFont);
}

void COutputWnd::UpdateFonts()
{
	m_wndOutputState.SetFont(&afxGlobalData.fontRegular);
	m_wndOutputData.SetFont(&afxGlobalData.fontRegular);
	m_wndOutputLog.SetFont(&afxGlobalData.fontRegular);
}

/////////////////////////////////////////////////////////////////////////////
// COutputList1

COutputList::COutputList()
{
}

COutputList::~COutputList()
{
}

BEGIN_MESSAGE_MAP(COutputList, CListBox)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_COMMAND(ID_VIEW_OUTPUTWND, OnViewOutput)
	ON_WM_WINDOWPOSCHANGING()
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// COutputList 消息处理程序

void COutputList::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	CMenu menu;
	menu.LoadMenu(IDR_OUTPUT_POPUP);

	CMenu* pSumMenu = menu.GetSubMenu(0);

	if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx)))
	{
		CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;

		if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
			return;

		((CMDIFrameWndEx*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
		UpdateDialogControls(this, FALSE);
	}

	SetFocus();
}

void COutputList::OnEditCopy()
{
	MessageBox(_T("复制输出"));
}

void COutputList::OnEditClear()
{
	MessageBox(_T("清除输出"));
}

void COutputList::OnViewOutput()
{
	CDockablePane* pParentBar = DYNAMIC_DOWNCAST(CDockablePane, GetOwner());
	CMDIFrameWndEx* pMainFrame = DYNAMIC_DOWNCAST(CMDIFrameWndEx, GetTopLevelFrame());

	if (pMainFrame != NULL && pParentBar != NULL)
	{
		pMainFrame->SetFocus();
		pMainFrame->ShowPane(pParentBar, FALSE, FALSE, FALSE);
		pMainFrame->RecalcLayout();

	}
}

void COutputWnd::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (nIDEvent)
	{
	case 1:
		{
			OnStartMonitor();
			KillTimer(1);
		}
		break;
	default:
		break;
	}
	CDockablePane::OnTimer(nIDEvent);
}

// CHejiexunServerMonitorView 消息处理程序
void COutputWnd::OnStartMonitor()
{
	//if (hThread != INVALID_HANDLE_VALUE)
	//{
	//	TerminateThread(hThread, 0);
	//	CloseHandle(hThread);
	//}
	
	hThread = CreateThread(NULL, 0, OutputThreadProc, (LPVOID)this, 0, &dwThread);
	
	if(hThread == NULL)
	{
		AfxMessageBox("启动业务线程失败，请稍后重试!");
		return;
	}
}

DWORD WINAPI OutputThreadProc(LPVOID lpParam)
{
	if (NULL == lpParam)
	{
		return -1;
	}

	int rowIndex = 0;
	//获取主对话框对象
	COutputWnd * pOutputWnd = (COutputWnd *)lpParam;

	CMainFrame*   pFrame=(CMainFrame*)AfxGetApp()->GetMainWnd();
	CHejiexunServerManagerView* pMonitorView = (CHejiexunServerManagerView*)pFrame->GetActiveView();

	
	CString outputMsg;
	std::map<CString, ProcessState> processStateMap;
	CListCtrl &listCtrl = pMonitorView->GetListCtrl();
	for (int i = 0; i < listCtrl.GetItemCount(); i++)
	{			
		char processName[100];
		listCtrl.GetItemText(i, 0, processName, 100);
		processStateMap[processName] = STATE_STOP;
	}
	std::map<CString, ProcessState>::iterator iter;

	//MsgPipeHandle clientMsgPipeHandle = clientGetMsgPipe("HejiexunServerTest");
	//if (clientMsgPipeHandle==NULL)
	//{
	//	AfxMessageBox("Client Msg Pipe handle is NULL.");
	//}

	while (TRUE) 
	{
		for (int iIndex = 0; iIndex < listCtrl.GetItemCount(); iIndex++)
		{
			char processName[100];
			listCtrl.GetItemText(iIndex, 0, processName, 100);

			ProcessState processState = ProcessManager::GetProcessState(processName);
			iter = processStateMap.find(processName);
			if (iter != processStateMap.end())
			{
				if (iter->second != processState)
				{
					switch (processState)
					{
					case STATE_STOP:
						{
							outputMsg.Format("%s %s", processName, TEXT_STOP);
						}
						break;
					case STATE_RUN:
						{
							outputMsg.Format("%s %s", processName, TEXT_RUN);
						}
						break;
					case STATE_SUSPEND:
						break;
					case STATE_UNKNOWN:
						break;
					default:
						break;
					}

					pOutputWnd->m_wndOutputState.AddString(outputMsg);

					processStateMap[processName] = STATE_STOP;
				}
			}
			else
			{
				processStateMap[processName] = processState;
				pOutputWnd->m_wndOutputState.AddString(outputMsg);
			}
			
		}

		//unsigned int msgCount;
		//MsgInfo *pMsgInfo = NULL;
		//msgCount = clientRecv(clientMsgPipeHandle, &pMsgInfo);
		//for (unsigned index = 0; index < msgCount; ++index)
		//{
		//	switch (pMsgInfo[index].type)
		//	{
		//	case 1:
		//		{
		//			char msgBuffer[1024];
		//			memset(msgBuffer, 0, 1024);
		//			memcpy(msgBuffer,pMsgInfo[index].address, pMsgInfo[index].length );
		//			pOutputWnd->m_wndOutputState.AddString(msgBuffer);
		//		}
		//	}
		//}

		Sleep(1000);
	}
	return 0;
}

