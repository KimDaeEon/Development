#include "pch.h"
#include "CoreGlobal.h"
#include "ThreadManager.h"
#include "DeadLockDetector.h"
#include "Memory.h"

ThreadManager*		GThreadManager = nullptr;
MemoryPoolManager*	GMemoryPoolManager = nullptr;
DeadLockDetector*	GDeadLockDetector = nullptr;

// 여기서 Manager의 생성 순서를 조절
class CoreGlobal
{
public:
	CoreGlobal()
	{
		GThreadManager = new ThreadManager();
		GMemoryPoolManager = new MemoryPoolManager();
		GDeadLockDetector = new DeadLockDetector();
	}

	~CoreGlobal()
	{
		delete GThreadManager;
		delete GMemoryPoolManager;
		delete GDeadLockDetector;
	}

} GCoreGlobal; // 전역변수를 통해 초기화 실행
