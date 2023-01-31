#include "pch.h"
#include "CoreGlobal.h"
#include "ThreadManager.h"
#include "DeadLockDetector.h"

ThreadManager* GThreadManager = nullptr;
DeadLockDetector* GDeadLockDetector = nullptr;

// ���⼭ Manager�� ���� ������ ����
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

} GCoreGlobal; // ���������� ���� �ʱ�ȭ ����