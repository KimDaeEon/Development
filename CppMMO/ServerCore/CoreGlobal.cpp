#include "pch.h"
#include "CoreGlobal.h"
#include "ThreadManager.h"
#include "DeadLockDetector.h"
#include "Memory.h"
#include "SocketUtils.h"

ThreadManager*			GThreadManager = nullptr;
MemoryPoolManager*		GMemoryPoolManager = nullptr;
SendBufferManager*		GSendBufferManager = nullptr;

DeadLockDetector*		GDeadLockDetector = nullptr;

// ���⼭ Manager�� ���� ������ ����
class CoreGlobal
{
public:
	CoreGlobal()
	{
		GThreadManager = new ThreadManager();
		GMemoryPoolManager = new MemoryPoolManager();
		GSendBufferManager = new SendBufferManager();

		GDeadLockDetector = new DeadLockDetector();
		SocketUtils::Init();
	}

	~CoreGlobal()
	{
		delete GThreadManager;
		delete GMemoryPoolManager;
		delete GSendBufferManager;

		delete GDeadLockDetector;
		SocketUtils::Clear();
	}

} GCoreGlobal; // ���������� ���� �ʱ�ȭ ����
