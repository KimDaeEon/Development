#include "pch.h"
#include "CoreGlobal.h"
#include "ThreadManager.h"
#include "DeadLockDetector.h"
#include "Memory.h"
#include "SocketUtils.h"

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
		SocketUtils::Init();
	}

	~CoreGlobal()
	{
		delete GThreadManager;
		delete GMemoryPoolManager;
		delete GDeadLockDetector;
		SocketUtils::Clear();
	}

} GCoreGlobal; // ���������� ���� �ʱ�ȭ ����
