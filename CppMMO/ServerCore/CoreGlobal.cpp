#include "pch.h"
#include "CoreGlobal.h"
#include "ThreadManager.h"
#include "DeadLockDetector.h"
#include "Memory.h"

ThreadManager*		GThreadManager = nullptr;
MemoryPoolManager*	GMemoryPoolManager = nullptr;
DeadLockDetector*	GDeadLockDetector = nullptr;

// ���⼭ Manager�� ���� ������ ����
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

} GCoreGlobal; // ���������� ���� �ʱ�ȭ ����
