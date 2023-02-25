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
using IocpCoreRef		= std::shared_ptr<class IocpCore>;
using IocpObjectRef		= std::shared_ptr<class IocpObject>;
using SessionRef		= std::shared_ptr<class Session>;
using ListenerRef		= std::shared_ptr<class Listener>;
using ServerServiceRef	= std::shared_ptr<class ServerService>;
using ClientServiceRef	= std::shared_ptr<class ClientService>;
