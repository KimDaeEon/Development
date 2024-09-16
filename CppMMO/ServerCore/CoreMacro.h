#pragma once

// C#의 OUT이 없기에, 매크로로 표시
#define OUT 

// ---------------------------
//			Namespace
// ---------------------------
#define NAMESPACE_BEGIN(name)	namespace name {
#define NAMESPACE_END			}

// ------------------------
//			Type
// ------------------------
#define sizeof16(val)			static_cast<int16>(sizeof(val))
#define sizeof32(val)			static_cast<int32>(sizeof(val))
#define len16(arr)				static_cast<int16>(sizeof(arr)/sizeof(arr[0]))
#define len32(arr)				static_cast<int32>(sizeof(arr)/sizeof(arr[0]))


// ------------------------
//			Lock
// ------------------------
// 2024-09-15 std::mutex 보다 성능상 이슈가 있어서 폐기
//#define USE_MANY_LOCKS(count)	Lock _locks[count];
//#define USE_LOCK				USE_MANY_LOCKS(1)
//#define READ_LOCK_IDX(idx)		ReadLockGuard readLockGuard_##idx(_locks[idx], typeid(this).name());
//#define READ_LOCK				READ_LOCK_IDX(0)
//#define WRITE_LOCK_IDX(idx)		WriteLockGuard writeLockGuard_##idx(_locks[idx], typeid(this).name());
//#define WRITE_LOCK				WRITE_LOCK_IDX(0)


// ------------------------
//			Memory
// ------------------------
#ifdef _DEBUG
#define myAlloc(size) PoolAllocator::Alloc(size)
#define myRelease(ptr) PoolAllocator::Release(ptr)
#else
#define myAlloc(size) PoolAllocator::Alloc(size)
#define myRelease(ptr) PoolAllocator::Release(ptr)
#endif

// ------------------------
//			Crash
// ------------------------
// __analysis_assume을 써야지 nullptr 역참조에 대한 경고(Warning)가 사라진다.
#define CRASH(cause)							\
{												\
	uint32* crash = nullptr;					\
	__analysis_assume(crash != nullptr);		\
	*crash = 0xDEADBEEF;						\
}			

#define ASSERT_CRASH(expr)						\
{												\
	if(!(expr))									\
	{											\
		CRASH("ASSERT_CRASH");					\
		__analysis_assume(expr);				\
	}											\
}