#include "pch.h"
#include "MemoryPool.h"

// --------------------
//		MemoryPool
// --------------------

MemoryPool::MemoryPool(int32 allocSize) : _allocSize(allocSize)
{
	::InitializeSListHead(&_header);
}

MemoryPool::~MemoryPool()
{
	// Lock-Free stack에 존재하는 메모리가 있다면 해제
	while (MemoryHeader* header = static_cast<MemoryHeader*>(::InterlockedPopEntrySList(&_header)))
	{
		::_aligned_free(header);
	}
}

void MemoryPool::Push(MemoryHeader* ptr)
{
	// 0이 들어가면 반환돤 메모리란 뜻이다. 다시 사용될 때에 placement new를 통해 다시 size가 할당된다.
	ptr->allocSize = 0; 

	// 여기서 PLIST_ENTRY로 형변환이 가능한 이유는 MemoryHeader가 SLIST_ENTRY를 할당받았기에, 
	// 이 구조체의 메모리 주소 맨 처음에 오는 데이터가 SLIST_ENTRY가 되어서 이것이 가능하다.
	::InterlockedPushEntrySList(&_header, static_cast<PSLIST_ENTRY>(ptr)); // Pool에 메모리 반납

	_usedCount.fetch_sub(1);
	_reservedCount.fetch_add(1);
}

MemoryHeader* MemoryPool::Pop()
{
	MemoryHeader* header = static_cast<MemoryHeader*>(::InterlockedPopEntrySList(&_header));

	// 없다면 데이터 할당
	if (header == nullptr)
	{
		// 여기서도 메모리 할당이 16바이트 단위로 된다는 보장이 없기에 _aligned_malloc 함수를 사용
		header = reinterpret_cast<MemoryHeader*>(::_aligned_malloc(_allocSize, SLIST_ALIGNMENT));
	}
	// 기존에 Pool에 남아 있는 메모리 가져와서 쓰는 경우
	else
	{
		// 이럴 일은 없겠지만 혹시나 해서 체크
		ASSERT_CRASH(header->allocSize == 0);
		_reservedCount.fetch_sub(1);
	}

	_usedCount.fetch_add(1);
	 
	return header;
}
