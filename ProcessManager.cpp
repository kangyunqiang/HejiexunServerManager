#include "stdafx.h"
#include "ProcessManager.h"
#include <Tlhelp32.h>
// // CreateToolhelp32Snapshot ��            

ProcessManager::ProcessManager(void)
{

}

ProcessManager::~ProcessManager(void)
{

}

BOOL ProcessManager::StartProcess(CString processPath, BOOL bShow)
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi; 
	SECURITY_ATTRIBUTES sa;
	HANDLE hRead, hWrite;

	memset(&si, 0, sizeof(si));
	memset(&pi, 0, sizeof(pi));
	//������������Ŀ¼������ᵼ�³�������ʱ��ӡ����
	CString processStartDirectory = processPath.Left(processPath.ReverseFind('\\'));
	
	if (processPath.Trim().GetLength() <= 0)
	{
		return FALSE;
	}
	
	if (bShow)
	{
		si.cb = sizeof(STARTUPINFO);
		si.dwFlags=STARTF_USESHOWWINDOW;
		si.wShowWindow = bShow ? SW_SHOW : SW_HIDE;
		if( !CreateProcess( NULL,   // No module name (use command line)
			processPath.GetBuffer(),        // Command line
			NULL,           // Process handle not inheritable
			NULL,           // Thread handle not inheritable
			FALSE,          // Set handle inheritance to FALSE
			0,              // No creation flags
			NULL,           // Use parent's environment block
			processStartDirectory,           // Use parent's starting directory 
			&si,            // Pointer to STARTUPINFO structure
			&pi )           // Pointer to PROCESS_INFORMATION structure
			) 
		{
			return FALSE;
		}
	}
	else
	{
		sa.nLength = sizeof(SECURITY_ATTRIBUTES);
		sa.lpSecurityDescriptor = NULL;
		sa.bInheritHandle = TRUE;
		if (!CreatePipe(&hRead, &hWrite, &sa, 0)) 
		{
			return FALSE;
		} 
		si.cb = sizeof(STARTUPINFO);
		GetStartupInfo(&si); 
		si.hStdError = hWrite;
		si.hStdOutput = hWrite;
		
		si.wShowWindow = SW_HIDE;
		si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;

		//�ؼ����裬CreateProcess�����������������MSDN
		if (!CreateProcess(NULL, processPath.GetBuffer(), NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, processStartDirectory, &si, &pi)) 
		{  
			return FALSE;
		}
		int len;
		DWORD dwRead;
		// ���ܵ�������ʾwSpawn�ӹܵ��з��ص������Ϣ
		if (!ReadFile( hRead, &len, sizeof(int), &dwRead, NULL) || dwRead == 0)
			return 0;

		while(len)
		{
			char *buf = new char[len + 1];
			memset(buf, 0, len + 1);
			if(!ReadFile( hRead, buf, len, &dwRead, NULL) || dwRead == 0)
				return 0;

			// ��������Ϣ�е�"\n"�滻ΪEdit Box��ʶ���"\r\n"
			CString tmp = buf;
			tmp.Replace("\n", "\r\n");

			// �������ʾ��Edit Box�У���ˢ�¶Ի���
			//pDlg->m_edit1.SetWindowText(Output);
			//pDlg->InvalidateRect(NULL);
			//pDlg->UpdateWindow();
			AfxMessageBox(tmp);

			delete[] buf;
			if(!ReadFile( hRead, &len, sizeof(int), &dwRead, NULL) || dwRead == 0)
				return 0;
		}

		CloseHandle(hWrite);
		CloseHandle(hRead);
	}
	//char cmdLine[MAX_PATH];  
	//wsprintf ( cmdLine,"Ping.exe %d.%d.%d.%d",10,30,127,10 );
	//SECURITY_ATTRIBUTES sa={sizeof ( sa ),NULL,TRUE};
	//SECURITY_ATTRIBUTES *psa=NULL;
	//DWORD dwShareMode=FILE_SHARE_READ|FILE_SHARE_WRITE;
	//OSVERSIONINFO osVersion={0};
	//osVersion.dwOSVersionInfoSize =sizeof ( osVersion );
	//if ( GetVersionEx ( &osVersion ) )
	//{
	//	if ( osVersion.dwPlatformId ==VER_PLATFORM_WIN32_NT )
	//	{
	//		psa=&sa;
	//		dwShareMode|=FILE_SHARE_DELETE;
	//	}
	//}
	////���ݰ汾���ù���ģʽ�Ͱ�ȫ����
	//HANDLE hConsoleRedirect=CreateFile (
	//	"c://NetStatus.txt",
	//	GENERIC_WRITE,
	//	dwShareMode,
	//	psa,
	//	OPEN_ALWAYS,
	//	FILE_ATTRIBUTE_NORMAL,
	//	NULL );
	//ASSERT ( hConsoleRedirect!=INVALID_HANDLE_VALUE );
	//STARTUPINFO s={sizeof ( s ) };
	//s.dwFlags =STARTF_USESHOWWINDOW|STARTF_USESTDHANDLES;
	////ʹ�ñ�׼������ʾ����
	//s.hStdOutput =hConsoleRedirect;//���ļ���Ϊ��׼������
	//s.wShowWindow =SW_HIDE;//���ؿ���̨����
	//PROCESS_INFORMATION pi={0};
	//if ( CreateProcess ( NULL,cmdLine,NULL,NULL,TRUE,NULL,NULL,NULL,&s,&pi) )
	//{
	//	//��������,ִ��Ping����,���������Ƿ���ͨ
	//	WaitForSingleObject ( pi.hProcess ,INFINITE );
	//	//�ȴ�����ִ�����
	//	CloseHandle ( pi.hProcess );
	//	CloseHandle ( pi.hThread );
	//	//�رս��̺����߳̾��
	//}
	//CloseHandle ( hConsoleRedirect );
	////�رտ���̨��������ļ����
	//CFile myFile ( "c://NetStatus.txt",CFile::modeRead );
	//ASSERT ( myFile.m_hFile!=NULL );
	//char * pszNetStatus=new char[myFile.GetLength () +1];
	//ZeroMemory ( pszNetStatus,myFile.GetLength () +1 );
	//myFile.Read ( pszNetStatus,myFile.GetLength () );
	//myFile.Close ();
	////���ļ�,��������һ���ַ�������
	//DeleteFile ( "c://NetStatus.txt" );
	////ɾ����ʱ�ļ�
	//AfxMessageBox(pszNetStatus);
	////�ѿ���̨���������Ϣд���༭����
	//delete pszNetStatus;
	return TRUE;
}

BOOL ProcessManager::StopProcess(CString processName) //���ڱ���
{
	//HANDLE hProcessHandle;  
	//ULONG nProcessID;
	//HWND TheWindow;

	//TheWindow = ::FindWindow( NULL, pszWindowTitle);
	//::GetWindowThreadProcessId( TheWindow, &nProcessID );
	//hProcessHandle = ::OpenProcess( PROCESS_TERMINATE, FALSE, nProcessID );
	//return ::TerminateProcess( hProcessHandle, 4 );
	PROCESSENTRY32 pe32;
	pe32.dwSize =sizeof(pe32);
	HANDLE hpro = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	if(hpro == INVALID_HANDLE_VALUE)
	{  
		return 0;
	}
	BOOL nowrun=Process32First(hpro, &pe32);
	while(nowrun)
	{
		if (processName.Compare(pe32.szExeFile) == 0)
		{
			DWORD proid = pe32.th32ProcessID;
			HANDLE hprocess=::OpenProcess(PROCESS_ALL_ACCESS, FALSE, proid);
			if (hprocess != NULL)
			{
				::TerminateProcess(hprocess, 0);
				::CloseHandle(hprocess);
				return TRUE;
			}
			else
			{
				return FALSE;
			}
		}
		nowrun=::Process32Next(hpro,&pe32);
	}
	return FALSE;
}

ProcessState ProcessManager::GetProcessState(CString processName)
{
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32);
	ProcessState ps;
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPALL/*TH32CS_SNAPPROCESS*/, NULL);
	BOOL bRet = Process32First(hProcessSnap, &pe32);
	while (bRet)
	{
		bRet = Process32Next(hProcessSnap, &pe32);
		printf("%s\n",&pe32.szExeFile);
		if (processName.Compare(pe32.szExeFile) == 0)
		{
			CloseHandle(hProcessSnap);
			return STATE_RUN;
		}
	}
	CloseHandle(hProcessSnap);
	return STATE_STOP;
}

