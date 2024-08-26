#include "stdafx.h"
#include "Service.h"

CService* gOwner = NULL;

VOID WINAPI RunCallback(DWORD argumentCount, LPTSTR* arguments) 
{
	gOwner->RunCallback(argumentCount, arguments);
}

VOID WINAPI CtrlHandlerCallback(DWORD opCode) 
{
	gOwner->CtrlHandlerCallback(opCode);
}

CService::CService(VOID) 
{
	ZeroMemory(mServiceName, sizeof(mServiceName));
	gOwner = this;
}

CService::~CService(VOID) 
{
}

/// <summary>
/// ���񽺸� ����� ���� �̿��ϴ� �Լ�.
/// </summary>
/// <param name="serviceName"></param>
/// <returns></returns>
BOOL CService::Install(LPCTSTR serviceName) 
{
	TCHAR ServiceFIleName[MAX_PATH] = { 0, };
	SC_HANDLE ServiceControlManager = NULL;
	SC_HANDLE ServiceHandle = NULL;

	if (!serviceName)
		return FALSE;

	// ���� ���� �Է�
	ServiceFIleName[0] = '"'; // ù ���ڿ� " ����.
	GetModuleFileName(NULL, ServiceFIleName + 1, MAX_PATH - 1); // �Ǿ� ������ hModule, Module �� Handle, ��ο� ���ϸ��� ���Ϸ��� �ϴ� Module �� �ǹ��Ѵ�. 
	// �� ���� NULL �̸� ���� �� �Լ��� ȣ���� ���μ����� exe ������ ��θ� ServiceFileName �� MAX_PATH -1 ���� ��ŭ �����Ѵ�. ��, ���� ���α׷��� ��θ� �����Ѵٰ� ���� �ȴ�.
	_tcscat(ServiceFIleName, _T("\"")); // ���ڿ� ���� " ����

	
	// ���� ���� �ɼ����� SCM �� Open
	// �̰� ���� �Ŵ����� ������ �δ� ���̴�. SQL �� DB Connection �� ����� ����. �׷��� �̰��� ���� �ϰ� CreateService �� �ϵ��� �ؾ��ϴ� ���̴�.
	// �տ� 2���� NULL �̸� ���� ��ǻ���� SERVICE_ACTIVE_DATABASE �ɼ����� connection �� ������. SC_MANAGER_CREATE_SERVICE�� �����ε�, �Ŵ����� �� ���� ���� ���� �ɼ��� �༭ �����Ѵ�.
	// �� ���ѿ� ���� ���� �������� ����ڰ� �� �� �ִ� ������ ��������. �ڼ��� ������ MSDN ����.
	ServiceControlManager = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE); 
	if (!ServiceControlManager)
		return FALSE;

	// ���񽺸� SERVICE_WIN32_OWN_PROCESS ���·� �����մϴ�.
	ServiceHandle = CreateService(ServiceControlManager,
		serviceName, // lpServiceName, ���� �̸�
		serviceName, // lpDisplayName, �������� �̸� (���� �̸��� ���� ����)
		SERVICE_ALL_ACCESS, // dwDesiredAccess, ��� �������� �����ϰ� ����
		SERVICE_WIN32_OWN_PROCESS, // dwServiceType, Win32 ���� Ÿ��
		SERVICE_DEMAND_START, // dwStartType,
		SERVICE_ERROR_NORMAL, // dwErrorControl,
		ServiceFIleName, // lpBinaryPathName, ��� �̸�
		NULL, // lpLoadOrderGroup
		NULL, // lpdwTagId
		NULL, // lpDependencies
		NULL, // lpServiceStartName, �� ���񽺰� ����Ǿ�� �ϴ� ���� �̸�, NULL �̸� ���� �ý��� ������ ���ȴ�. TODO: ���� �ð� �Ǹ� �� �ɼ� �Ẹ��
		NULL // lpPassword TODO: ���� �ð��Ǹ� �� �ɼ� �Ẹ��
	);

	// ������ �����ߴٸ� �Ѿ��.
	if (!ServiceHandle) {
		// ����
		CloseServiceHandle(ServiceControlManager);
		return FALSE;
	}

	// ����� �ڵ��� �ݾ��ش�.
	CloseServiceHandle(ServiceHandle);
	CloseServiceHandle(ServiceControlManager);

	return TRUE;
}

/// <summary>
/// ���񽺸� ������ ���� ����ϴ� �Լ�.
/// </summary>
/// <param name="serviceName"></param>
/// <returns></returns>
BOOL CService::Uninstall(LPCTSTR serviceName) 
{
	SC_HANDLE ServiceControlManager = NULL;
	SC_HANDLE ServiceHandle = NULL;

	if (!serviceName)
		return FALSE;

	// �ν��� �� ���� ���������� SCManager �� Open
	// SCManager ���� �ΰ�(Open), ���� ���� �۾��ϰ�, �ݴ� ��(Close)�� ���� �۾� �⺻ ����Ŭ�̴�.
	ServiceControlManager = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE); 
	if (!ServiceControlManager)
		return FALSE;

	// DELETE �ϱ� ���ؼ� OpenService ����
	ServiceHandle = OpenService(ServiceControlManager, serviceName, DELETE);

	if (!ServiceHandle) {
		CloseServiceHandle(ServiceHandle);
		CloseServiceHandle(ServiceControlManager);

		return FALSE;
	}

	// ���� ���񽺸� �����մϴ�.
	if (!DeleteService(ServiceHandle)) { // �� ����� FALSE ����
		CloseServiceHandle(ServiceHandle);
		CloseServiceHandle(ServiceControlManager);

		return FALSE;
	}

	// �Ϸ� �� ����� �ڵ� ���� �ݾ��ݴϴ�.
	CloseServiceHandle(ServiceHandle);
	CloseServiceHandle(ServiceControlManager);

	return TRUE;
}

/// <summary>
/// ���� ���� �Լ�.
/// </summary>
/// <param name="serviceName"></param>
/// <returns></returns>
BOOL CService::Begin(LPTSTR serviceName) 
{
	if (!serviceName)
		return FALSE;

	// StartServiceCtrlDispatcher ���� ����� ���� ȯ�� ����.
	// Dispatcher: A piece of software responsible for assigning priorities and resources to tasks waiting to be scheduled.
	// Dispatcher �� CPU �� ������� STS (Short-Term Scheduling) �� ���Ͽ� ���õ� Process �� �Ѱ��ִ� ����̴�.
	// Context Switching, User Mode �� ��ȯ, ����� ���α׷��� ������� ���� �ش� �ּҷ� �̵�, 3���� ����� �Ѵ�.
	// Scheduler ���� �������� �ؾ��� ���� ���ϴ� �� ������ Scheduelr, ������ CPU �� Process �� �Ҵ��ϴ� ���� Dispatcher �� ���̴�.
	// ���� �ǹ̷� Scheduling �� Scheduler �� Dispatche ����� ��� ������ ���̴�.
	SERVICE_TABLE_ENTRY DispatchTable[] = {
		{serviceName, ::RunCallback},  // ���� ������ �ִ� RunCallback �� ȣ���ϱ� ���� :: �����ڸ� ���� ���̴�.
		{NULL, NULL}
	};

	_tcscpy(mServiceName, serviceName); // ������ ��� ���� �̸��� ����ϹǷ� mServiceName �� �����մϴ�.

	OnStarted();


	if (!StartServiceCtrlDispatcher(DispatchTable)) { // ���� ���μ����� ���� �����带 SCM�� �����Ѵ�. �ش� �۾��� �������� SCM ���� ó���ϰ� �ȴٴ� ��.
		// StartServiceCtrlDispatcher �� ���� �Ŀ� SERVICE_STOPPED ���°� ���� �ʴ� �� ������ ���� �ʴ´�.
		// ������ ���, �Ǵ� ����� ����� ���
		// ���� ������ ������� ������� ���Ŀ� �ٸ� �����忡�� ���� ���α׷��� ���� ��Ű��, getchar() �� ���缭 ������ Ȯ���ϴ� ������ �ϴ� ���̴�.
		// TODO: ���� �����尡 ���߸� �ٸ� ��������� ���۵ȴٰ� ���� �ִµ�, ���� ������ �ð��� ��������� ���� ���� �� �ڿ� �Ҵ� �����ؼ� �ü���� �ٸ� ������ ���α׷��� ������ ã�ƺ��� �� �� ����.
		_tprintf(_T("## Debug console mode ##\n"));
		getchar();
	}

	return TRUE;
}

/// <summary>
/// ���� ���� �Լ�.
/// </summary>
/// <param name=""></param>
/// <returns></returns>
BOOL CService::End(VOID) 
{
	OnStopped(); // End �Լ��� OnStopped �� �Բ� �۵�

	return TRUE;
}


/// <summary>
/// ���� ���� �ݹ� StartServiceCtrlDispatcher �� ���������� ����Ǿ��� ���� �Ѿ���� �ݹ�.
/// </summary>
/// <param name="argumentCount"></param>
/// <param name="arguments"></param>
VOID CService::RunCallback(DWORD argumentCount, LPTSTR* arguments) 
{ // TODO: �̰� ã�ƺ��� ����׷� ���� �۵� ������ Ȯ�� ���ϴ� �� ������.. �ϴ� �ٸ� �͵� �ؾ��ؼ� �Ѿ�� ���� �ð����� Ȯ��.
	DWORD Status;

	// ���� ����ϰ� �ִ� ���� �����̹Ƿ� ����� mServiceStatus �� ���� ���� ����Ͽ� ���.
	mServiceStatus.dwServiceType				= SERVICE_WIN32;			// SERVICE_WIN32_OWN_PROCESS �� ����. ���μ��� ������ ����ȴٴ� ��. �ٸ� ��ġ ����̹��� �ִ� ���񽺵� �ֳ�����.
	mServiceStatus.dwCurrentState				= SERVICE_START_PENDING;	// ���񽺰� ���۵ȴٴ� ���� �ƴ϶�, ������ ���ε� �̷����� �ִٴ� �ǹ̴�. ���񽺴� ���� �ó� ���� �ÿ� Ư�� �۾��� �ʿ��ѵ� �װ��� �ϴ� ���̶� ���̴�.
																			// �ʱ�ȭ�� ������ SCM �� Service Control �� ���� �� �ִ� ���°� 'SERVICE_RUNNING'�̴�. SERVICE_RUNNING ���°� �ƴϸ� ���� ������ ���еǾ��ٰ� SCM �� ǥ���Ѵ�.
	mServiceStatus.dwControlsAccepted			= SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_PAUSE_CONTINUE; //  ������ �Ͻ�����, ������� �����ϴ�.
	
	mServiceStatus.dwWin32ExitCode				= 0;						// ���� �ó� ���� �ÿ� ������ �����ڵ�, 0�� NO_ERROR �� ����. ���� ���� Ư���� �����ڵ带 �������� �ʴ� ��, ���� �̷��� �����Ѵٰ� ���� �ִ�.
	mServiceStatus.dwServiceSpecificExitCode	= 0;						// dwWin32ExitCode ���� ERROR_SERVICE_SPECIFIC_ERROR �� �ƴ� �̻󿡾� �� ���� �����ϴ� ���� ���ǹ�.
	mServiceStatus.dwCheckPoint					= 0;						// ���񽺰� � �۾��� ������ ���� ���� �ɸ��� start, stop, pause, continue ���� pending ���°� �Ǵµ� �� ���� ���� �����Ϸ��� �����Ѵ�. �������� �������� 0���� ����.
	mServiceStatus.dwWaitHint					= 0;						// start, stop, pause, continue �۾��� ���� �ð��� �����ؼ� �и� ������� ��Ÿ���� ��. �� �ð��� �����Ǹ� �� �ð��� �Ѱ� �Ǹ� dwCheckPoint � �����ϰ� ���񽺰� Ư�� �۾� �� �����ߴٰ� �����Ѵ�.

	// ���� ���¸� SERVICE_START_PENDING ���� �س��� Control Callback ���, SERVICE_START_PENDING �� �ϴ� ������ ��ü������ �̷��� �۾��� ���־�� �ϱ� �����̴�.
	mServiceStatusHandle = RegisterServiceCtrlHandler(mServiceName, ::CtrlHandlerCallback); // �� �Լ��� Service Control Request �� ó���ϱ� ���� CtrlHandlerCallback �Լ��� ���񽺿� ����Ѵ�. �׸��� ���� ������ �����ϴ� �ڵ��� �����Ѵ�.

	// Control Callback ����� �����ϸ� ����
	if (mServiceStatusHandle == (SERVICE_STATUS_HANDLE)0)	// 0, NULL �� ���� SERVICE_STATUS_HANDLE �� ĳ��Ʈ ������ �ϴ� ���̴�.
		return;

	Status = NO_ERROR; // ��� ���������Ƿ� NO_ERROR

	// ����� ���������Ƿ� SERVICE_RUNNING ���·� ����
	mServiceStatus.dwCurrentState	= SERVICE_RUNNING;
	mServiceStatus.dwCheckPoint		= 0;					// TODO: ���߿� �� ���ϴ� ���� Ȯ���ϴ� �� �˸� Ȯ���غ���.
	mServiceStatus.dwWaitHint		= 0;

	if (!SetServiceStatus(mServiceStatusHandle, &mServiceStatus)) // Service Control Manager, SCM �� �����ϰ� �ִ� Service �� ����(Status) ������ ������Ʈ �ϴ� �Լ�. �����ϸ� non-zero, �����ϸ� 0 ����.
		return;
}

VOID CService::CtrlHandlerCallback(DWORD opCode) 
{ // SCM �� request �� ó���ϴ� �Լ��̴�.
	switch (opCode) {
	case SERVICE_CONTROL_PAUSE:		// ���� �Ͻ� ������ ���
		mServiceStatus.dwCurrentState = SERVICE_PAUSED;
		break;
	case SERVICE_CONTROL_CONTINUE:	// ���� �簳�� ���
		mServiceStatus.dwCurrentState = SERVICE_RUNNING;
		break;
	case SERVICE_CONTROL_STOP:		// ���� ������ ���
		mServiceStatus.dwWin32ExitCode	= 0;
		mServiceStatus.dwCurrentState	= SERVICE_STOPPED;
		mServiceStatus.dwCheckPoint		= 0;
		mServiceStatus.dwWaitHint		= 0;
		// ���� ���� �Լ� OnStopped �� �־�� �ϴ� ��ġ, ���⼱ ������� ���� ��ġ �̵�
		break;
	case SERVICE_CONTROL_INTERROGATE:  // ���� ��Ʈ�� �Ŵ������� ���� ���� ������ ������ �� ���
		break;
	} // switch

	// ���� ������ �Ǿ����Ƿ� ����
	if (!SetServiceStatus(mServiceStatusHandle, &mServiceStatus))
		return;
}