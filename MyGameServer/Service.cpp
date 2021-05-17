#include"Service.h"

CService* gOwner = NULL; // TODO: �̰� ���� ���� ���� ������?? ���Ŀ� �̰��� ��ӹ޾Ƽ� �� �����ͷ� ������ �ϴ� �� ����.

VOID WINAPI RunCallback(DWORD argumentCount, LPTSTR* arguments) {
	gOwner->RunCallback(argumentCount, arguments);
}

VOID WINAPI CtrlHandlerCallback(DWORD opCode) {
	gOwner->CtrlHandlerCallback(opCode);
}

CService::CService(VOID) {
	ZeroMemory(mServiceName, sizeof(mServiceName));

	gOwner = this;
}

CService::~CService(VOID) {

}

/// <summary>
/// ���񽺸� ����� ���� �̿��ϴ� �Լ�.
/// </summary>
/// <param name="serviceName"></param>
/// <returns></returns>
BOOL CService::Install(LPCTSTR serviceName) {
	TCHAR ServiceFIleName[MAX_PATH] = { 0, };
	SC_HANDLE ServiceControlManager = NULL;
	SC_HANDLE ServiceHandle = NULL;

	if (!serviceName)
		return FALSE;

	// ���� ���� �Է�
	ServiceFIleName[0] = '"'; // ù ���ڿ� " ����.
	GetModuleFileName(NULL, ServiceFIleName + 1, MAX_PATH - 1); // TODO: �� API ����� ���� �Ẹ��
	_tcscat(ServiceFIleName, _T("\"")); // ���ڿ� ���� " ����

	
	// ���� ���� �ɼ����� SCM �� Open
	// �̰� ���� �Ŵ����� ������ �δ� ���̴�. SQL �� DB Connection �� ����� ����. �׷��� �̰��� ���� �ϰ� CreateService �� �ϵ��� �ؾ��ϴ� ���̴�.
	// �տ� 2���� NULL �̸� ���� ��ǻ���� SERVICE_ACTIVE_DATABASE �ɼ����� connection �� ������. SC_MANAGER_CREATE_SERVICE�� �����ε�, �Ŵ����� �� ���� ���� ���� �ɼ��� �༭ �����Ѵ�.
	// �� ���ѿ� ���� ���� �������� ����ڰ� �� �� �ִ� ������ ��������. �ڼ��� ������ MSDN ����.
	ServiceControlManager = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE); 
	if (!ServiceControlManager)
		return FALSE;

	// ���񽺸� SERVICE_WIN32_OWN_PROCESS ���·� �����մϴ�.
	ServiceHandle = CreateService(ServiceControlManager, // TODO: �� API ����� ���� �Ẹ��
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
	// TODO: ���⼭ ServiceHandle �̶� ServiceControlManager 2���� �ڵ��� �ִµ�, ��Ȯ�� ���� �������� ����غ��� ����.
	CloseServiceHandle(ServiceHandle);
	CloseServiceHandle(ServiceControlManager);

	return TRUE;
}

/// <summary>
/// ���񽺸� ������ ���� ����ϴ� �Լ�.
/// </summary>
/// <param name="serviceName"></param>
/// <returns></returns>
BOOL CService::Uninstall(LPCTSTR serviceName) {
	SC_HANDLE ServiceControlManager = NULL;
	SC_HANDLE ServiceHandle = NULL;

	if (!serviceName)
		return FALSE;

	// �ν��� �� ���� ���������� SCManager �� Open
	// SCManager ���� �ΰ�(Open), ���� ���� �۾��ϰ�, �ݴ� ��(Close)�� ���� �۾� �⺻ ����Ŭ�̴�.
	ServiceControlManager = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);  // TODO: DELETE �ϴ� ������ OpenService ���� �ο��Ѵ�. SC_MANAGER_CREATE_SERVICE �� DELETE �ʹ� ū ����� ���� �� ����. ���� Ȯ��
	if (!ServiceControlManager)
		return FALSE;

	// DELETE �ϱ� ���ؼ� OpenService ����
	ServiceHandle = OpenService(ServiceControlManager, serviceName, DELETE);

	if (!ServiceHandle) {
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
BOOL CService::Begin(LPTSTR serviceName) {

	if (!serviceName)
		return FALSE;

	// StartServiceCtrlDispatcher ���� ����� ���� ȯ�� ����.
	SERVICE_TABLE_ENTRY DispatchTable[] = {
		{serviceName, ::RunCallback},  // ���� ������ �ִ� WINAPI RunCallback �� ȣ���ϱ� ���� :: �����ڸ� ���� ���̴�.
		{NULL, NULL}
	};

	_tcscpy(mServiceName, serviceName);  // ������ ��� ���� �̸��� ����ϹǷ� mServiceName �� �����մϴ�.

	OnStarted();


	if (!StartServiceCtrlDispatcher(DispatchTable)) {
		// ������ ���
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
BOOL CService::End(VOID) {
	OnStopped(); // End �Լ��� OnStopped �� �Բ� �۵�

	return TRUE;
}


/// <summary>
/// ���� ���� �ݹ� StartServiceCtrlDispatcher �� ���������� ����Ǿ��� ���� �Ѿ���� �ݹ�.
/// </summary>
/// <param name="argumentCount"></param>
/// <param name="arguments"></param>
VOID CService::RunCallback(DWORD argumentCount, LPTSTR* arguments) {
	DWORD Status;

	// ���� ����ϴ� ���� �����̹Ƿ� ����� mServiceStatus �� ���� ���� ����Ͽ� ���.
	// TODO: �� �Ʒ����� �� ����� ���鼭 �� �ٲٴ��� Ȯ���غ���.
	mServiceStatus.dwServiceType = SERVICE_WIN32;
	mServiceStatus.dwCurrentState = SERVICE_START_PENDING;
	mServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_PAUSE_CONTINUE;
	
	mServiceStatus.dwWin32ExitCode = 0;
	mServiceStatus.dwServiceSpecificExitCode = 0;
	mServiceStatus.dwCheckPoint = 0;
	mServiceStatus.dwWaitHint = 0;

	// ���� ���¸� SERVICE_START_PENDING ���� ��ȣ�� Control Callback ���
	mServiceStatusHandle = RegisterServiceCtrlHandler(mServiceName, ::CtrlHandlerCallback);

	// Control Callback ����� �����ϸ� ����
	// TODO: �Ʒ� SERVICE_STATUS_HANDLE ĳ��Ʈ ������ ��� ���ϴ��� �� �־�� �����غ���.
	if (mServiceStatusHandle == (SERVICE_STATUS_HANDLE)0) // 0, NULL �� ���� SERVICE_STATUS_HANDLE �� ĳ��Ʈ ������ �ϴ� ���̴�.
		return;

	Status = NO_ERROR; // ��� ���������Ƿ� NO_ERROR

	// ����� ���������Ƿ� SERVICE_RUNNING ���·� ����
	mServiceStatus.dwCurrentState = SERVICE_RUNNING;
	mServiceStatus.dwCheckPoint = 0; // TOOD: �̰� ������ 0���� �ߴµ� �� �� �ٲ���? �߰��� �ٲ?
	mServiceStatus.dwWaitHint = 0;

	// TODO: SetServiceStatus �� � ����� �ϴ��� ���������� ����.
	if (!SetServiceStatus(mServiceStatusHandle, &mServiceStatus))
		return;
}

VOID CService::CtrlHandlerCallback(DWORD opCode) {
	switch (opCode) {
	case SERVICE_CONTROL_PAUSE: // ���� �Ͻ� ������ ���
		mServiceStatus.dwCurrentState = SERVICE_PAUSED;
		break;
	case SERVICE_CONTROL_CONTINUE: // ���� �簳�� ���
		mServiceStatus.dwCurrentState = SERVICE_RUNNING;
		break;
	case SERVICE_CONTROL_STOP:	// ���� ������ ���
		mServiceStatus.dwWin32ExitCode = 0;
		mServiceStatus.dwCurrentState = SERVICE_STOPPED;
		mServiceStatus.dwCheckPoint = 0;
		mServiceStatus.dwWaitHint = 0;
		break;
	case SERVICE_CONTROL_INTERROGATE:  // ���� ��Ʈ�� �Ŵ������� ���� ���� ������ ������ �� ���
		break;
	} // switch

	// ���� ������ �Ǿ����Ƿ� ����
	if (!SetServiceStatus(mServiceStatusHandle, &mServiceStatus))
		return;
}