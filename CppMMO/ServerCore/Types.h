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
#define MAKE_REF_TYPE(name) using name##Ref = std::shared_ptr<class name>;
MAKE_REF_TYPE(IocpCore);
MAKE_REF_TYPE(IocpObject);
MAKE_REF_TYPE(Session);
MAKE_REF_TYPE(PacketSession);
MAKE_REF_TYPE(Listener);
MAKE_REF_TYPE(ServerService);
MAKE_REF_TYPE(ClientService);
MAKE_REF_TYPE(SendBuffer);
MAKE_REF_TYPE(SendBufferChunk);
MAKE_REF_TYPE(Job);
MAKE_REF_TYPE(JobQueue);


#define _STOMP // TODO: ���� �̰� �����ϰ� �׽�Ʈ �ʿ�. MemoryPoolManager::Allocate���� ���� �߻� ����