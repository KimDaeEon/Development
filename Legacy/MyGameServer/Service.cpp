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
/// 서비스를 등록할 때에 이용하는 함수.
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

	// 실행 파일 입력
	ServiceFIleName[0] = '"'; // 첫 문자에 " 삽입.
	GetModuleFileName(NULL, ServiceFIleName + 1, MAX_PATH - 1); // 맨앞 변수가 hModule, Module 의 Handle, 경로와 파일명을 구하려고 하는 Module 을 의미한다. 
	// 이 값이 NULL 이면 현재 저 함수를 호출한 프로세스의 exe 파일의 경로를 ServiceFileName 에 MAX_PATH -1 길이 만큼 저장한다. 즉, 현재 프로그램의 경로를 저장한다고 보면 된다.
	_tcscat(ServiceFIleName, _T("\"")); // 문자열 끝에 " 삽입

	
	// 서비스 생성 옵션으로 SCM 을 Open
	// 이게 서비스 매니저와 연결을 맺는 것이다. SQL 의 DB Connection 과 비슷한 느낌. 그래서 이것을 먼저 하고 CreateService 를 하든지 해야하는 것이다.
	// 앞에 2개가 NULL 이면 로컬 컴퓨터의 SERVICE_ACTIVE_DATABASE 옵션으로 connection 이 열린다. SC_MANAGER_CREATE_SERVICE는 권한인데, 매니저를 열 때에 권한 설정 옵션을 줘서 오픈한다.
	// 이 권한에 따라서 서비스 관리에서 사용자가 할 수 있는 범위가 정해진다. 자세한 사항은 MSDN 참조.
	ServiceControlManager = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE); 
	if (!ServiceControlManager)
		return FALSE;

	// 서비스를 SERVICE_WIN32_OWN_PROCESS 형태로 생성합니다.
	ServiceHandle = CreateService(ServiceControlManager,
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
	CloseServiceHandle(ServiceHandle);
	CloseServiceHandle(ServiceControlManager);

	return TRUE;
}

/// <summary>
/// 서비스를 삭제할 때에 사용하는 함수.
/// </summary>
/// <param name="serviceName"></param>
/// <returns></returns>
BOOL CService::Uninstall(LPCTSTR serviceName) 
{
	SC_HANDLE ServiceControlManager = NULL;
	SC_HANDLE ServiceHandle = NULL;

	if (!serviceName)
		return FALSE;

	// 인스톨 할 때와 마찬가지로 SCManager 를 Open
	// SCManager 연결 맺고(Open), 서비스 관련 작업하고, 닫는 것(Close)이 서비스 작업 기본 사이클이다.
	ServiceControlManager = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE); 
	if (!ServiceControlManager)
		return FALSE;

	// DELETE 하기 위해서 OpenService 실행
	ServiceHandle = OpenService(ServiceControlManager, serviceName, DELETE);

	if (!ServiceHandle) {
		CloseServiceHandle(ServiceHandle);
		CloseServiceHandle(ServiceControlManager);

		return FALSE;
	}

	// 실제 서비스를 삭제합니다.
	if (!DeleteService(ServiceHandle)) { // 못 지우면 FALSE 리턴
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
BOOL CService::Begin(LPTSTR serviceName) 
{
	if (!serviceName)
		return FALSE;

	// StartServiceCtrlDispatcher 에서 등록할 서비스 환경 정보.
	// Dispatcher: A piece of software responsible for assigning priorities and resources to tasks waiting to be scheduled.
	// Dispatcher 는 CPU 의 제어권을 STS (Short-Term Scheduling) 에 의하여 선택된 Process 에 넘겨주는 모듈이다.
	// Context Switching, User Mode 로 전환, 사용자 프로그램의 재시작을 위해 해당 주소로 이동, 3가지 기능을 한다.
	// Scheduler 와의 차이점은 해야할 일을 정하는 것 까지가 Scheduelr, 실제로 CPU 를 Process 에 할당하는 것이 Dispatcher 의 일이다.
	// 넓은 의미로 Scheduling 은 Scheduler 와 Dispatche 기능을 모두 내포한 말이다.
	SERVICE_TABLE_ENTRY DispatchTable[] = {
		{serviceName, ::RunCallback},  // 전역 범위에 있는 RunCallback 을 호출하기 위해 :: 연산자를 붙인 것이다.
		{NULL, NULL}
	};

	_tcscpy(mServiceName, serviceName); // 앞으로 계속 서비스 이름을 사용하므로 mServiceName 에 복사합니다.

	OnStarted();


	if (!StartServiceCtrlDispatcher(DispatchTable)) { // 서비스 프로세스의 메인 스레드를 SCM에 연결한다. 해당 작업을 메인으로 SCM 에서 처리하게 된다는 뜻.
		// StartServiceCtrlDispatcher 는 성공 후에 SERVICE_STOPPED 상태가 되지 않는 한 리턴을 하지 않는다.
		// 실패인 경우, 또는 디버그 모드인 경우
		// 서비스 형식은 디버깅이 어려워서 추후에 다른 스레드에서 서버 프로그램을 동작 시키고, getchar() 로 멈춰서 동작을 확인하는 식으로 하는 것이다.
		// TODO: 메인 스레드가 멈추면 다른 스레드들이 동작된다고 쓰여 있는데, 추후 가용한 시간에 스레드들의 동작 순서 및 자원 할당 관련해서 운영체제와 다른 스레드 프로그래밍 서적을 찾아봐야 할 것 같다.
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
BOOL CService::End(VOID) 
{
	OnStopped(); // End 함수는 OnStopped 와 함께 작동

	return TRUE;
}


/// <summary>
/// 서비스 시작 콜백 StartServiceCtrlDispatcher 가 성공적으로 실행되었을 때에 넘어오는 콜백.
/// </summary>
/// <param name="argumentCount"></param>
/// <param name="arguments"></param>
VOID CService::RunCallback(DWORD argumentCount, LPTSTR* arguments) 
{ // TODO: 이거 찾아보니 디버그로 서비스 작동 과정을 확인 못하는 거 같은데.. 일단 다른 것도 해야해서 넘어가고 추후 시간나면 확인.
	DWORD Status;

	// 현재 사용하고 있는 서비스 상태이므로 멤버인 mServiceStatus 에 값을 직접 등록하여 사용.
	mServiceStatus.dwServiceType				= SERVICE_WIN32;			// SERVICE_WIN32_OWN_PROCESS 와 같다. 프로세스 내에서 실행된다는 뜻. 다른 장치 드라이버에 있는 서비스도 있나보다.
	mServiceStatus.dwCurrentState				= SERVICE_START_PENDING;	// 서비스가 시작된다는 뜻이 아니라, 시작할 것인데 미뤄지고 있다는 의미다. 서비스는 시작 시나 정지 시에 특정 작업이 필요한데 그것을 하는 중이란 뜻이다.
																			// 초기화가 끝나고 SCM 의 Service Control 을 받을 수 있는 상태가 'SERVICE_RUNNING'이다. SERVICE_RUNNING 상태가 아니면 서비스 실행이 실패되었다고 SCM 은 표시한다.
	mServiceStatus.dwControlsAccepted			= SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_PAUSE_CONTINUE; //  정지나 일시정지, 재시작이 가능하다.
	
	mServiceStatus.dwWin32ExitCode				= 0;						// 시작 시나 실패 시에 리턴할 에러코드, 0은 NO_ERROR 와 같다. 서비스 별로 특별한 에러코드를 설정하지 않는 한, 보통 이렇게 세팅한다고 쓰여 있다.
	mServiceStatus.dwServiceSpecificExitCode	= 0;						// dwWin32ExitCode 값이 ERROR_SERVICE_SPECIFIC_ERROR 가 아닌 이상에야 이 값을 설정하는 것은 무의미.
	mServiceStatus.dwCheckPoint					= 0;						// 서비스가 어떤 작업을 진행할 때에 오래 걸리면 start, stop, pause, continue 에서 pending 상태가 되는데 이 때에 값을 추적하려고 설정한다. 추적하지 않으려면 0으로 설정.
	mServiceStatus.dwWaitHint					= 0;						// start, stop, pause, continue 작업이 끝날 시간을 예측해서 밀리 세컨드로 나타내는 것. 이 시간이 설정되면 이 시간을 넘게 되면 dwCheckPoint 등도 무시하고 서비스가 특정 작업 중 실패했다고 간주한다.

	// 서비스 상태를 SERVICE_START_PENDING 으로 해놓고 Control Callback 등록, SERVICE_START_PENDING 을 하는 이유는 구체적으로 이러한 작업을 해주어야 하기 때문이다.
	mServiceStatusHandle = RegisterServiceCtrlHandler(mServiceName, ::CtrlHandlerCallback); // 이 함수는 Service Control Request 를 처리하기 위한 CtrlHandlerCallback 함수를 서비스에 등록한다. 그리고 서비스 정보를 관리하는 핸들을 리턴한다.

	// Control Callback 등록이 실패하면 종료
	if (mServiceStatusHandle == (SERVICE_STATUS_HANDLE)0)	// 0, NULL 에 대해 SERVICE_STATUS_HANDLE 로 캐스트 연산을 하는 것이다.
		return;

	Status = NO_ERROR; // 등록 성공했으므로 NO_ERROR

	// 등록이 성공했으므로 SERVICE_RUNNING 상태로 변경
	mServiceStatus.dwCurrentState	= SERVICE_RUNNING;
	mServiceStatus.dwCheckPoint		= 0;					// TODO: 나중에 값 변하는 과정 확인하는 법 알면 확인해보자.
	mServiceStatus.dwWaitHint		= 0;

	if (!SetServiceStatus(mServiceStatusHandle, &mServiceStatus)) // Service Control Manager, SCM 가 관리하고 있는 Service 의 상태(Status) 정보를 업데이트 하는 함수. 성공하면 non-zero, 실패하면 0 리턴.
		return;
}

VOID CService::CtrlHandlerCallback(DWORD opCode) 
{ // SCM 의 request 를 처리하는 함수이다.
	switch (opCode) {
	case SERVICE_CONTROL_PAUSE:		// 서비스 일시 정지인 경우
		mServiceStatus.dwCurrentState = SERVICE_PAUSED;
		break;
	case SERVICE_CONTROL_CONTINUE:	// 서비스 재개인 경우
		mServiceStatus.dwCurrentState = SERVICE_RUNNING;
		break;
	case SERVICE_CONTROL_STOP:		// 서비스 정지인 경우
		mServiceStatus.dwWin32ExitCode	= 0;
		mServiceStatus.dwCurrentState	= SERVICE_STOPPED;
		mServiceStatus.dwCheckPoint		= 0;
		mServiceStatus.dwWaitHint		= 0;
		// 원래 가상 함수 OnStopped 가 있어야 하는 위치, 여기선 디버깅을 위해 위치 이동
		break;
	case SERVICE_CONTROL_INTERROGATE:  // 서비스 컨트롤 매니저에게 현재 상태 정보를 보내야 할 경우
		break;
	} // switch

	// 상태 변경이 되었으므로 적용
	if (!SetServiceStatus(mServiceStatusHandle, &mServiceStatus))
		return;
}