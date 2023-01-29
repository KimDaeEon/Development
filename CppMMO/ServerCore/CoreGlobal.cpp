#include "pch.h"
#include "CoreGlobal.h"
#include "ThreadManager.h"
#include "DeadLockDetector.h"

ThreadManager* GThreadManager = nullptr;
DeadLockDetector* GDeadLockDetector = nullptr;

// 여기서 Manager의 생성 순서를 조절
class CoreGlobal
{
public:
	CoreGlobal()
	{
		GThreadManager = new ThreadManager();
		GDeadLockDetector = new DeadLockDetector();
	}

	~CoreGlobal()
	{
		delete GThreadManager;
		delete GDeadLockDetector;
	}

} GCoreGlobal; // 전역변수를 통해 초기화 실행
