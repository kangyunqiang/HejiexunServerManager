#pragma once

enum ProcessState
{
	STATE_STOP,
	STATE_RUN,
	STATE_SUSPEND,
	STATE_UNKNOWN,
};


class ProcessManager
{
public:
	ProcessManager(void);
	~ProcessManager(void);
	static BOOL StartProcess(CString processPath, BOOL bShow);
	static BOOL StopProcess(CString processName);

	static ProcessState GetProcessState(CString processName);
};

