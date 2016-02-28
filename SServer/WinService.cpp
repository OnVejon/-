#include "Log.h"
#include "socket.h"
#include "WinService.h"
#include <tchar.h>

SERVICE_STATUS ServiceStatus;
SERVICE_STATUS_HANDLE hStatus;

#define SERVICE_NAME _T("ServiceTest")

void WINAPI ControlHandler(DWORD fdwControl)
{
	switch (fdwControl)
	{
	case SERVICE_CONTROL_STOP:
	case SERVICE_CONTROL_SHUTDOWN:
		ServiceStatus.dwWin32ExitCode = 0;
		ServiceStatus.dwCurrentState = SERVICE_STOPPED;
	default: break;
	}
	SetServiceStatus(hStatus, &ServiceStatus);
}

void WINAPI ServiceMain(DWORD dwArgc, LPTSTR* lpszArgv)
{
	ZeroMemory(&ServiceStatus, sizeof(ServiceStatus));
	ServiceStatus.dwServiceType = SERVICE_WIN32;
	ServiceStatus.dwCurrentState = SERVICE_START_PENDING;
	ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;

	hStatus = RegisterServiceCtrlHandler(SERVICE_NAME, (LPHANDLER_FUNCTION)ControlHandler);
	if (!hStatus) {
		return;
	}
	// We report the running status to SCM.
	ServiceStatus.dwCurrentState = SERVICE_RUNNING;
	SetServiceStatus(hStatus, &ServiceStatus);

	//for the main code;
	//主要代码加在这里！
	MainService();

	ServiceStatus.dwCurrentState = SERVICE_STOPPED;
	ServiceStatus.dwWin32ExitCode = -1;
	SetServiceStatus(hStatus, &ServiceStatus);
}

void ServiceInit()
{
	SERVICE_TABLE_ENTRY DispatchTable[] = {
		{ SERVICE_NAME, (LPSERVICE_MAIN_FUNCTION)ServiceMain },
		{ NULL, NULL }
	};
	StartServiceCtrlDispatcher(DispatchTable);
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
	ServiceInit();
	return 0;
	
}

