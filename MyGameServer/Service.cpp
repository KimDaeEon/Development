#include"Service.h"

CService* gOwner = NULL; // TODO: 이거 뭐할 때에 쓰는 것이지?? 추후에 이것을 상속받아서 이 포인터로 무엇을 하는 것 같다.

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
/// 서비스를 등록할 때에 이용하는 함수.
/// </summary>
/// <param name="serviceName"></param>
/// <returns></returns>
BOOL CService::Install(LPCTSTR serviceName) {
	TCHAR ServiceFIleName[MAX_PATH] = { 0, };
	SC_HANDLE ServiceControlManager = NULL;
	SC_HANDLE ServiceHandle = NULL;

	if (!serviceName)
		return FALSE;

	// 실행 파일 입력
	ServiceFIleName[0] = '"'; // 첫 문자에 " 삽입.
	GetModuleFileName(NULL, ServiceFIleName + 1, MAX_PATH - 1); // TODO: 이 API 제대로 보고 써보기
	_tcscat(ServiceFIleName, _T("\"")); // 문자열 끝에 " 삽입

	
	// 서비스 생성 옵션으로 SCM 을 Open
	// 이게 서비스 매니저와 연결을 맺는 것이다. SQL 의 DB Connection 과 비슷한 느낌. 그래서 이것을 먼저 하고 CreateService 를 하든지 해야하는 것이다.
	// 앞에 2개가 NULL 이면 로컬 컴퓨터의 SERVICE_ACTIVE_DATABASE 옵션으로 connection 이 열린다. SC_MANAGER_CREATE_SERVICE는 권한인데, 매니저를 열 때에 권한 설정 옵션을 줘서 오픈한다.
	// 이 권한에 따라서 서비스 관리에서 사용자가 할 수 있는 범위가 정해진다. 자세한 사항은 MSDN 참조.
	ServiceControlManager = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE); 
	if (!ServiceControlManager)
		return FALSE;

	// 서비스를 SERVICE_WIN32_OWN_PROCESS 형태로 생성합니다.
	ServiceHandle = CreateService(ServiceControlManager, // TODO: 이 API 제대로 보고 써보기
		serviceName, // lpServiceName, 서비스 이름
		serviceName, // lpDisplayName, 보여지는 이름 (서비스 이름과 같게 설정)
		SERVICE_ALL_ACCESS, // dwDesiredAccess, 모든 엑세스가 가능하게 설정
		SERVICE_WIN32_OWN_PROCESS, // dwServiceType, Win32 서비스 타입
		SERVICE_DEMAND_START, // dwStartType,
		SERVICE_ERROR_NORMAL, // dwErrorControl,
		ServiceFIleName, // lpBinaryPathName, 경로 이름
		NULL, // lpLoadOrderGroup
		NULL, // lpdwTagId
		NULL, // lpDependencies
		NULL, // lpServiceStartName, 이 서비스가 실행되어야 하는 계정 이름, NULL 이면 로컬 시스템 계정이 사용된다. TODO: 추후 시간 되면 이 옵션 써보기
		NULL // lpPassword TODO: 추후 시간되면 이 옵션 써보기
	);

	// 세션이 성공했다면 넘어간다.
	if (!ServiceHandle) {
		// 실패
		CloseServiceHandle(ServiceControlManager);
		return FALSE;
	}

	// 사용한 핸들을 닫아준다.
	// TODO: 여기서 ServiceHandle 이랑 ServiceControlManager 2개의 핸들이 있는데, 정확히 무슨 차이인지 사용해보고 알자.
	CloseServiceHandle(ServiceHandle);
	CloseServiceHandle(ServiceControlManager);

	return TRUE;
}

/// <summary>
/// 서비스를 삭제할 때에 사용하는 함수.
/// </summary>
/// <param name="serviceName"></param>
/// <returns></returns>
BOOL CService::Uninstall(LPCTSTR serviceName) {
	SC_HANDLE ServiceControlManager = NULL;
	SC_HANDLE ServiceHandle = NULL;

	if (!serviceName)
		return FALSE;

	// 인스톨 할 때와 마찬가지로 SCManager 를 Open
	// SCManager 연결 맺고(Open), 서비스 관련 작업하고, 닫는 것(Close)이 서비스 작업 기본 사이클이다.
	ServiceControlManager = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);  // TODO: DELETE 하는 권한을 OpenService 에서 부여한다. SC_MANAGER_CREATE_SERVICE 는 DELETE 와는 큰 상관은 없는 것 같다. 추후 확인
	if (!ServiceControlManager)
		return FALSE;

	// DELETE 하기 위해서 OpenService 실행
	ServiceHandle = OpenService(ServiceControlManager, serviceName, DELETE);

	if (!ServiceHandle) {
		CloseServiceHandle(ServiceHandle);
		CloseServiceHandle(ServiceControlManager);

		return FALSE;
	}

	// 완료 후 사용한 핸들 값을 닫아줍니다.
	CloseServiceHandle(ServiceHandle);
	CloseServiceHandle(ServiceControlManager);

	return TRUE;
}

/// <summary>
/// 서비스 시작 함수.
/// </summary>
/// <param name="serviceName"></param>
/// <returns></returns>
BOOL CService::Begin(LPTSTR serviceName) {

	if (!serviceName)
		return FALSE;

	// StartServiceCtrlDispatcher 에서 등록할 서비스 환경 정보.
	SERVICE_TABLE_ENTRY DispatchTable[] = {
		{serviceName, ::RunCallback},  // 전역 범위에 있는 WINAPI RunCallback 을 호출하기 위해 :: 연산자를 붙인 것이다.
		{NULL, NULL}
	};

	_tcscpy(mServiceName, serviceName);  // 앞으로 계속 서비스 이름을 사용하므로 mServiceName 에 복사합니다.

	OnStarted();


	if (!StartServiceCtrlDispatcher(DispatchTable)) {
		// 실패인 경우
		_tprintf(_T("## Debug console mode ##\n"));
		getchar();
	}

	return TRUE;
}

/// <summary>
/// 서비스 종료 함수.
/// </summary>
/// <param name=""></param>
/// <returns></returns>
BOOL CService::End(VOID) {
	OnStopped(); // End 함수는 OnStopped 와 함께 작동

	return TRUE;
}


/// <summary>
/// 서비스 시작 콜백 StartServiceCtrlDispatcher 가 성공적으로 실행되었을 때에 넘어오는 콜백.
/// </summary>
/// <param name="argumentCount"></param>
/// <param name="arguments"></param>
VOID CService::RunCallback(DWORD argumentCount, LPTSTR* arguments) {
	DWORD Status;

	// 현재 사용하는 서비 승태이므로 멤버인 mServiceStatus 에 값을 직접 등록하여 사용.
	// TODO: 이 아래부터 다 디버그 찍어보면서 뭐 바꾸는지 확인해보자.
	mServiceStatus.dwServiceType = SERVICE_WIN32;
	mServiceStatus.dwCurrentState = SERVICE_START_PENDING;
	mServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_PAUSE_CONTINUE;
	
	mServiceStatus.dwWin32ExitCode = 0;
	mServiceStatus.dwServiceSpecificExitCode = 0;
	mServiceStatus.dwCheckPoint = 0;
	mServiceStatus.dwWaitHint = 0;

	// 서비스 상태를 SERVICE_START_PENDING 으로 내호고 Control Callback 등록
	mServiceStatusHandle = RegisterServiceCtrlHandler(mServiceName, ::CtrlHandlerCallback);

	// Control Callback 등록이 실패하면 종료
	// TODO: 아래 SERVICE_STATUS_HANDLE 캐스트 연산자 어떻게 변하는지 값 넣어보며 실험해보자.
	if (mServiceStatusHandle == (SERVICE_STATUS_HANDLE)0) // 0, NULL 에 대해 SERVICE_STATUS_HANDLE 로 캐스트 연산을 하는 것이다.
		return;

	Status = NO_ERROR; // 등록 성공했으므로 NO_ERROR

	// 등록이 성공했으므로 SERVICE_RUNNING 상태로 변경
	mServiceStatus.dwCurrentState = SERVICE_RUNNING;
	mServiceStatus.dwCheckPoint = 0; // TOOD: 이거 위에서 0으로 했는데 왜 또 바꾸지? 중간에 바뀌나?
	mServiceStatus.dwWaitHint = 0;

	// TODO: SetServiceStatus 도 어떤 기능을 하는지 개략적으로 알자.
	if (!SetServiceStatus(mServiceStatusHandle, &mServiceStatus))
		return;
}

VOID CService::CtrlHandlerCallback(DWORD opCode) {
	switch (opCode) {
	case SERVICE_CONTROL_PAUSE: // 서비스 일시 정지인 경우
		mServiceStatus.dwCurrentState = SERVICE_PAUSED;
		break;
	case SERVICE_CONTROL_CONTINUE: // 서비스 재개인 경우
		mServiceStatus.dwCurrentState = SERVICE_RUNNING;
		break;
	case SERVICE_CONTROL_STOP:	// 서비스 정지인 경우
		mServiceStatus.dwWin32ExitCode = 0;
		mServiceStatus.dwCurrentState = SERVICE_STOPPED;
		mServiceStatus.dwCheckPoint = 0;
		mServiceStatus.dwWaitHint = 0;
		break;
	case SERVICE_CONTROL_INTERROGATE:  // 서비스 컨트롤 매니저에게 현재 상태 정보를 보내야 할 경우
		break;
	} // switch

	// 상태 변경이 되었으므로 적용
	if (!SetServiceStatus(mServiceStatusHandle, &mServiceStatus))
		return;
}