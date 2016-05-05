#include "stdafx.h"
#include "ProcessManager.h"
#include <Tlhelp32.h>
// // CreateToolhelp32Snapshot 法            

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
	//必须设置启动目录，否则会导致程序启动时打印出错
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

		//关键步骤，CreateProcess函数参数意义请查阅MSDN
		if (!CreateProcess(NULL, processPath.GetBuffer(), NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, processStartDirectory, &si, &pi)) 
		{  
			return FALSE;
		}
		int len;
		DWORD dwRead;
		// 读管道，并显示wSpawn从管道中返回的输出信息
		if (!ReadFile( hRead, &len, sizeof(int), &dwRead, NULL) || dwRead == 0)
			return 0;

		while(len)
		{
			char *buf = new char[len + 1];
			memset(buf, 0, len + 1);
			if(!ReadFile( hRead, buf, len, &dwRead, NULL) || dwRead == 0)
				return 0;

			// 将返回信息中的"\n"替换为Edit Box可识别的"\r\n"
			CString tmp = buf;
			tmp.Replace("\n", "\r\n");

			// 将结果显示在Edit Box中，并刷新对话框
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
	////根据版本设置共享模式和安全属性
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
	////使用标准柄和显示窗口
	//s.hStdOutput =hConsoleRedirect;//将文件作为标准输出句柄
	//s.wShowWindow =SW_HIDE;//隐藏控制台窗口
	//PROCESS_INFORMATION pi={0};
	//if ( CreateProcess ( NULL,cmdLine,NULL,NULL,TRUE,NULL,NULL,NULL,&s,&pi) )
	//{
	//	//创建进程,执行Ping程序,测试网络是否连通
	//	WaitForSingleObject ( pi.hProcess ,INFINITE );
	//	//等待进程执行完毕
	//	CloseHandle ( pi.hProcess );
	//	CloseHandle ( pi.hThread );
	//	//关闭进程和主线程句柄
	//}
	//CloseHandle ( hConsoleRedirect );
	////关闭控制台定向输出文件句柄
	//CFile myFile ( "c://NetStatus.txt",CFile::modeRead );
	//ASSERT ( myFile.m_hFile!=NULL );
	//char * pszNetStatus=new char[myFile.GetLength () +1];
	//ZeroMemory ( pszNetStatus,myFile.GetLength () +1 );
	//myFile.Read ( pszNetStatus,myFile.GetLength () );
	//myFile.Close ();
	////打开文件,把它读到一个字符缓冲区
	//DeleteFile ( "c://NetStatus.txt" );
	////删除临时文件
	//AfxMessageBox(pszNetStatus);
	////把控制台程序输出信息写到编辑框中
	//delete pszNetStatus;
	return TRUE;
}

BOOL ProcessManager::StopProcess(CString processName) //窗口标题
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

