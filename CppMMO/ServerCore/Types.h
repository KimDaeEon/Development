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
using Atomic		= std::atomic<T>; // using의 좋은 점은 template과도 함꼐 쓸 수 있다는 것!
using Mutex			= std::mutex; // 나중에 다른 mutex를 쓰고 싶으면 이를 통해 바꾼다.
using CondVar		= std::condition_variable;
using UniqueLock	= std::unique_lock<std::mutex>;
using LockGuard		= std::lock_guard <std::mutex>;  

// shared_ptr 전부 다 타이밍하기 귀찮으므로 아래와 같이 타입으로 만들어둔다.
// 객체이름Ref 형식으로 shared_ptr 축약형 타입이 만들어진다.
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


#define _STOMP // TODO: 추후 이거 해제하고 테스트 필요. MemoryPoolManager::Allocate에서 문제 발생 중임