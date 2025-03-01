#pragma once

extern class ThreadManager*			GThreadManager;
extern class MemoryPoolManager*		GMemoryPoolManager;
extern class SendBufferManager*		GSendBufferManager;

extern class DeadLockDetector*		GDeadLockDetector;

extern class GlobalQueue*			GGlobalQueue;
extern class JobTimer*				GJobTimer;

extern class DBConnectionPool*		GDBConnectionPool;

extern class ConsoleLog*			GConsoleLogger;