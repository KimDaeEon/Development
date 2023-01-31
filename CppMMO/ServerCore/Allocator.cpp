#include "pch.h"
#include "Allocator.h"

// ------------------------
//		BaseAllocator
// ------------------------

void* BaseAllocator::Alloc(int32 size)
{
	return ::malloc(size);
}

void BaseAllocator::Release(void* ptr)
{
	::free(ptr);
}


// -------------------------
//		StompAllocator
// -------------------------

void* StompAllocator::Alloc(int32 size)
{
	// 들어온 size를 커버할 수 있는 페이지 숫자를 계산한다. -1 부분 중요하니 잘 이해하자.
	const int64 pageCount = (size + PAGE_SIZE - 1) / PAGE_SIZE;

	// 데이터를 맨 뒤부터 할당
	const int64 dataOffset = pageCount * PAGE_SIZE - size;

	// 이 방식으로 할당을 해야 해제한 메모리를 건드리려고 할 때에 exception이 바로 호출된다.
	void* baseAddress = ::VirtualAlloc(NULL, pageCount * PAGE_SIZE, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	return static_cast<void*>(static_cast<int8*>(baseAddress) + dataOffset);
}

void StompAllocator::Release(void* ptr)
{
	const int64 address = reinterpret_cast<int64>(ptr);
	const int64 baseAddress = address - (address % PAGE_SIZE);

	// MEM_RELEASE 옵션을 주면 아래 함수 두 번째 인자는 0을 넣어야 한다.
	// MEM_DECOMMIT 옵션을 주면 맨처음 인자 주소부터 두 번째 인자크기만큼 메모리를 해제
	::VirtualFree(reinterpret_cast<void*>(baseAddress), 0, MEM_RELEASE);
}
