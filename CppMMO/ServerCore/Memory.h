#pragma once
#include "Allocator.h"

class MemoryPool;

// ---------------------------
//		MemoryPoolManager
// ---------------------------
class MemoryPoolManager
{
	// 4096바이트를 넘는 데이터는 이미 큰 단위라서 메모리풀링 사용하지 않는다.
	enum
	{
		SMALL_BYTES_INTERVAL = 32,
		MEDIUM_BYTES_INTERVAL = 128,
		BIG_BYTES_INTERVAL = 256,

		SMALL_BYTES_SPAN = 1024, // 작은 크기 메모리풀 영역 길이
		MEDIUM_BYTES_SPAN = 1024, // 중간 크기 메모리풀 영역 길이
		BIG_BYTES_SPAN = 2048, // 가장 큰 크기 메모리풀 영역 길이

		// 1024 바이트까지 32바이트 단위
		// 2048 바이트까지 128바이트 단위
		// 4096 바이트까지 256바이트 단위
		POOL_COUNT = (SMALL_BYTES_SPAN / SMALL_BYTES_INTERVAL) + (MEDIUM_BYTES_SPAN / MEDIUM_BYTES_INTERVAL) + (BIG_BYTES_SPAN / BIG_BYTES_INTERVAL),
		MAX_ALLOC_SIZE = SMALL_BYTES_SPAN + MEDIUM_BYTES_SPAN + BIG_BYTES_SPAN
	};


public:
	MemoryPoolManager();
	~MemoryPoolManager();

	void* Allocate(int32 size);
	void Release(void* ptr);

private:
	vector<MemoryPool*> _pools; // 프로그램 종료 시에 pool들을 해제하는 용도

	// 할당하는 메모리 크기별로 pool이 존재하고 그것을 빠르게 찾기 위한 테이블.
	MemoryPool* _poolSearchTable[MAX_ALLOC_SIZE + 1];
};

// 가변 인자를 쓰면 인자가 없을 수도 있다.
// 기본적으로 PoolAllocator(MemoryPool)를 쓰고, 내부에서 MemoryPool을 쓸지 StompAllocator를 쓸지를 바꿀 수 있다.
template<typename Type, typename... Args>
Type* myNew(Args&&... args)
{
	Type* memory = static_cast<Type*>(myAlloc(sizeof(Type)));

	// placement new
	new(memory) Type(std::forward<Args>(args)...);

	return memory;
}

// myRelease를 상황에 따라 바꿔서 사용하고 싶은 메모리 할당 방식을 사용한다.
template<typename Type>
void myDelete(Type* obj)
{
	obj->~Type();
	myRelease(obj);
}

// 이게 있으면 아래의 shared_ptr<Type> myMakeShared(Args&&... args)로 추론이 안된다.. 
// 템플릿 추론 우선 순서 명심해두자.
//template<typename Type>
//shared_ptr<Type> myMakeShared()
//{
//	return shared_ptr<Type>{ myNew<Type>(), myDelete<Type> };
//}

template<typename Type, typename... Args>
shared_ptr<Type> myMakeShared(Args&&... args)
{
	return shared_ptr<Type>{ myNew<Type>(std::forward<Args>(args)...), myDelete<Type> };
}