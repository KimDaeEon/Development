#pragma once
#include <mutex>
#include <atomic>

using BYTE = unsigned char;
using int8 = __int8;
using int16 = __int16;
using int32 = __int32;
using int64 = __int64;
using uint8 = unsigned __int8;
using uint16 = unsigned __int16;
using uint32 = unsigned __int32;
using uint64 = unsigned __int64;

template<typename T>
using Atomic		= std::atomic<T>; // using�� ���� ���� template���� �Բ� �� �� �ִٴ� ��!
using Mutex			= std::mutex; // ���߿� �ٸ� mutex�� ���� ������ �̸� ���� �ٲ۴�.
using CondVar		= std::condition_variable;
using UniqueLock	= std::unique_lock<std::mutex>;
using LockGuard		= std::lock_guard <std::mutex>;  

// shared_ptr ���� �� Ÿ�̹��ϱ� �������Ƿ� �Ʒ��� ���� Ÿ������ �����д�.
// ��ü�̸�Ref �������� shared_ptr ����� Ÿ���� ���������.
#define MAKE_SHARED_PTR_TYPE(name) using name##Ref = std::shared_ptr<class name>;
MAKE_SHARED_PTR_TYPE(IocpCore);
MAKE_SHARED_PTR_TYPE(IocpObject);
MAKE_SHARED_PTR_TYPE(Session);
MAKE_SHARED_PTR_TYPE(PacketSession);
MAKE_SHARED_PTR_TYPE(Listener);
MAKE_SHARED_PTR_TYPE(ServerService);
MAKE_SHARED_PTR_TYPE(ClientService);
MAKE_SHARED_PTR_TYPE(SendBuffer);
MAKE_SHARED_PTR_TYPE(SendBufferChunk);
MAKE_SHARED_PTR_TYPE(Job);
MAKE_SHARED_PTR_TYPE(JobQueue);

//#define _STOMP // TODO: ���� �̰� �����ϰ� �׽�Ʈ �ʿ�. MemoryPoolManager::Allocate���� ���� �߻� ����