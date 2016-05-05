
// HejiexunServerManagerView.cpp : CHejiexunServerManagerView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
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

const char * TEXT_STOP = "ֹͣ";
const char * TEXT_RUN = "����";
const char * TEXT_MANUAL = "�ֶ�";
const char * TEXT_AUTO = "�Զ�";
const char * TEXT_SHOW = "ǰ̨";
const char * TEXT_HIDE = "��̨";

//////////////////////////////////////////////////////////////////////////
//Ӧ���߼������̺߳���
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

// CHejiexunServerManagerView ����/����
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
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CListView::PreCreateWindow(cs);
}

void CHejiexunServerManagerView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();

	//��ʼ���б�
	InitListCtrl();
	//����������Ϣ
	LoadConfigInfo();
	//�������
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


// CHejiexunServerManagerView ���

#ifdef _DEBUG
void CHejiexunServerManagerView::AssertValid() const
{
	CListView::AssertValid();
}

void CHejiexunServerManagerView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CHejiexunServerManagerDoc* CHejiexunServerManagerView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CHejiexunServerManagerDoc)));
	return (CHejiexunServerManagerDoc*)m_pDocument;
}


#endif //_DEBUG

void CHejiexunServerManagerView::InitListCtrl()
{
	//  �Ӷ������������ ListView��
	CListCtrl & listCtrl = GetListCtrl();

	LONG lStyle = GetWindowLong(listCtrl.GetSafeHwnd(), GWL_STYLE);//��ȡ��ǰ��������
	lStyle &= ~LVS_TYPEMASK; //�����ʾ��ʽλ
	lStyle |= LVS_REPORT; //������ʾ��ʽ
	SetWindowLong(listCtrl.GetSafeHwnd(), GWL_STYLE, lStyle);//���ô�������
	listCtrl.SetExtendedStyle( LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

	const char * arCols[] = {"����",	"����",	"ӳ������", "��������", "��ʾ��ʽ", "·��", "״̬"};
	const int colWidth[] = {100, 150, 100, 80, 80, 200, 100};
	for (int nCol = 0; nCol < 7; nCol++)
	{
		listCtrl.InsertColumn(nCol, (LPCTSTR)arCols[nCol], LVCFMT_CENTER, colWidth[nCol]);
	}
}

// CHejiexunServerManagerView ��Ϣ�������

void CHejiexunServerManagerView::OnButtonStart()
{
	CListCtrl &listCtrl = GetListCtrl();
	int iIndex = -1;
	POSITION pos = listCtrl.GetFirstSelectedItemPosition(); //posѡ�е�����λ��
	if (pos == NULL)
	{
		AfxMessageBox("��ѡ��Ҫ��������Ŀ!");
		return;
	}
	else
	{
		while (pos != NULL)   //�����ѡ�����
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
	POSITION pos = listCtrl.GetFirstSelectedItemPosition(); //posѡ�е�����λ��
	if (pos == NULL)
	{
		AfxMessageBox("��ѡ��Ҫֹͣ����Ŀ!");
		return;
	}
	else
	{
		while (pos != NULL)   //�����ѡ�����
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
	POSITION pos = listCtrl.GetFirstSelectedItemPosition(); //posѡ�е�����λ��
	if (pos == NULL)
	{
		AfxMessageBox("��ѡ��Ҫɾ���ļ�¼!");
		return;
	}
	else
	{
		if (AfxMessageBox("ȷ��Ҫɾ����", MB_OKCANCEL) != IDOK)
		{
			return;
		}
		while (pos != NULL)   //�����ѡ�����
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
	// TODO: �ڴ���������������
	CListCtrl &listCtrl = GetListCtrl();
	if (listCtrl.GetSelectedCount() != 1)
	{
		AfxMessageBox("��ѡ��һ�н����޸�!");
		return;
	}
	int iIndex = -1;
	POSITION pos = listCtrl.GetFirstSelectedItemPosition(); //posѡ�е�����λ��
	if (pos == NULL)
	{
		AfxMessageBox("��ѡ��һ�н����޸�!");
		return;
	}
	else
	{
		if (pos != NULL)   //�����ѡ�����
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
		AfxMessageBox("�ó����Ѿ�����");
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
		AfxMessageBox("��������ʧ�ܣ�����·���Ƿ���ȷ");
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
		AfxMessageBox("�ó����Ѿ�ֹͣ����");
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
		AfxMessageBox("ֹͣ����ʧ�ܣ�����·���Ƿ���ȷ");
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
		appName.Format("��¼%d", itemIndex++);
		GetPrivateProfileString(appName, "����", "", buffer, BUFFER_SIZE, configFilePath);
		if ((GetLastError() != ERROR_SUCCESS) || (strlen(buffer) <= 0))
		{
			break;
		}
		nItem = listCtrl.InsertItem(listCtrl.GetItemCount(), buffer);

		GetPrivateProfileString(appName, "����", "", buffer, BUFFER_SIZE, configFilePath);
		if ((GetLastError() != ERROR_SUCCESS) || (strlen(buffer) <= 0))
		{
			break;
		}
		listCtrl.SetItemText(nItem, 1, buffer);

		GetPrivateProfileString(appName, "ӳ������", "", buffer, BUFFER_SIZE, configFilePath);
		if ((GetLastError() != ERROR_SUCCESS) || (strlen(buffer) <= 0))
		{
			break;
		}
		listCtrl.SetItemText(nItem, 2, buffer);

		GetPrivateProfileString(appName, "��������", "", buffer, BUFFER_SIZE, configFilePath);
		if ((GetLastError() != ERROR_SUCCESS) || (strlen(buffer) <= 0))
		{
			break;
		}
		listCtrl.SetItemText(nItem, 3, buffer);

		GetPrivateProfileString(appName, "��ʾ��ʽ", "", buffer, BUFFER_SIZE, configFilePath);
		if ((GetLastError() != ERROR_SUCCESS) || (strlen(buffer) <= 0))
		{
			break;
		}
		listCtrl.SetItemText(nItem, 4, buffer);

		GetPrivateProfileString(appName, "·��", "", buffer, BUFFER_SIZE, configFilePath);
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
	//ɾ���ļ��Ӷ�����������ݣ��������ֱ�Ŵ���
	DeleteFile(configFilePath);
	//����д��������Ϣ
	int driverIndex = 1;
	CString appName;
	CListCtrl &listCtrl = GetListCtrl();
	char buffer[BUFFER_SIZE];
	for (int iIndex = 0; iIndex < listCtrl.GetItemCount(); iIndex++)
	{
		appName.Format("��¼%d", driverIndex++);
		listCtrl.GetItemText(iIndex, 0, buffer, BUFFER_SIZE);
		WritePrivateProfileString(appName, "����", buffer, configFilePath);
		listCtrl.GetItemText(iIndex, 1, buffer, BUFFER_SIZE);
		WritePrivateProfileString(appName, "����", buffer, configFilePath);
		listCtrl.GetItemText(iIndex, 2, buffer, BUFFER_SIZE);
		WritePrivateProfileString(appName, "ӳ������", buffer, configFilePath);
		listCtrl.GetItemText(iIndex, 3, buffer, BUFFER_SIZE);
		WritePrivateProfileString(appName, "��������", buffer, configFilePath);
		listCtrl.GetItemText(iIndex, 4, buffer, BUFFER_SIZE);
		WritePrivateProfileString(appName, "��ʾ��ʽ", buffer, configFilePath);
		listCtrl.GetItemText(iIndex, 5, buffer, BUFFER_SIZE);
		WritePrivateProfileString(appName, "·��", buffer, configFilePath);
	}
}

// CHejiexunServerMonitorView ��Ϣ�������
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
		AfxMessageBox("����ҵ���߳�ʧ�ܣ����Ժ�����!");
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
							message.Format("����%s����ʧ��", name);
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (nIDEvent == 1234)
	{
		OnStartMonitor();
		KillTimer(nIDEvent);
	}
	CListView::OnTimer(nIDEvent);
}
