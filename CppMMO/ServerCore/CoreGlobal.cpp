#include "pch.h"
#include "CoreGlobal.h"
#include "ThreadManager.h"
#include "DeadLockDetector.h"
#include "GlobalQueue.h"
#include "JobTimer.h"
#include "Memory.h"
#include "SocketUtils.h"
#include "DBConnectionPool.h"
#include "ConsoleLog.h"

ThreadManager*			GThreadManager		= nullptr;
MemoryPoolManager*		GMemoryPoolManager	= nullptr;
SendBufferManager*		GSendBufferManager	= nullptr;

DeadLockDetector*		GDeadLockDetector	= nullptr;

GlobalQueue*			GGlobalQueue		= nullptr;
JobTimer*				GJobTimer			= nullptr;

DBConnectionPool*		GDBConnectionPool	= nullptr;
ConsoleLog*				GConsoleLogger = nullptr;


// 여기서 Manager 의 생성 순서를 조절
class CoreGlobal
{
public:
	CoreGlobal()
	{
		GMemoryPoolManager = new MemoryPoolManager();
		GThreadManager		= new ThreadManager();

		GSendBufferManager	= new SendBufferManager();

		GDeadLockDetector	= new DeadLockDetector();

		GGlobalQueue		= new GlobalQueue();
		GJobTimer			= new JobTimer();

		GDBConnectionPool	= new DBConnectionPool();

		GConsoleLogger		= new ConsoleLog();

		SocketUtils::Init();
	}

	~CoreGlobal()
	{
		delete GThreadManager;
		delete GSendBufferManager;

		delete GDeadLockDetector;

		delete GGlobalQueue;
		delete GJobTimer;

		delete GDBConnectionPool;

		delete GConsoleLogger;

		SocketUtils::Clear();
		delete GMemoryPoolManager;
	}

} GCoreGlobal; // 전역변수를 통해 초기화 실행
