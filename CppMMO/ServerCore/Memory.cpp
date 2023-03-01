#include "pch.h"
#include "Memory.h"
#include "MemoryPool.h"

// ---------------------------
//		MemoryPoolManager
// ---------------------------
MemoryPoolManager::MemoryPoolManager()
{
	int32 size = 0;
	
	// 0~32번 테이블은 0~32바이트 단위의 데이터를 할당할 때에 사용되고,
	// 이 때에 32바이트씩 메모리를 할당하는 MemoryPool이 사용된다.
	int32 tableIdx = 0; 

	for (size = SMALL_BYTES_INTERVAL; size <= SMALL_BYTES_SPAN; size += SMALL_BYTES_INTERVAL)
	{
		MemoryPool* pool = new MemoryPool(size);
		_pools.push_back(pool);

		while (tableIdx <= size)
		{
			_poolSearchTable[tableIdx] = pool;
			tableIdx++;
		}
	}

	for (; size <= SMALL_BYTES_SPAN + MEDIUM_BYTES_SPAN; size += MEDIUM_BYTES_INTERVAL)
	{
		MemoryPool* pool = new MemoryPool(size);
		_pools.push_back(pool);

		while (tableIdx <= size)
		{
			_poolSearchTable[tableIdx] = pool;
			tableIdx++;
		}
	}

	for (; size <= SMALL_BYTES_SPAN + MEDIUM_BYTES_SPAN + BIG_BYTES_SPAN; size += BIG_BYTES_INTERVAL)
	{
		MemoryPool* pool = new MemoryPool(size);
		_pools.push_back(pool);

		while (tableIdx <= size)
		{
			_poolSearchTable[tableIdx] = pool;
			tableIdx++;
		}
	}
}

MemoryPoolManager::~MemoryPoolManager()
{
	for (MemoryPool* pool : _pools)
		delete pool;

	_pools.clear();
}

void* MemoryPoolManager::Allocate(int32 size)
{
	MemoryHeader* header = nullptr;
	const int32 allocSize = size + sizeof(MemoryHeader); // 메모리 헤더도 추가해야하므로 총 사이즈는 이렇게 계산된다.
#ifdef _STOMP
	header = reinterpret_cast<MemoryHeader*>(StompAllocator::Alloc(allocSize));
#else
	if (allocSize > MAX_ALLOC_SIZE)
	{
		// 메모리 풀링 최대 크기를 벗어나면 일반 할당
		header = reinterpret_cast<MemoryHeader*>(::_aligned_malloc(allocSize, SLIST_ALIGNMENT));
	}
	else
	{
		// 크기가 작으면 메모리 풀에서 꺼내온다.
		header = _poolSearchTable[allocSize]->Pop();
	}
#endif
	

	return MemoryHeader::AttachHeader(header, allocSize);
}

void MemoryPoolManager::Release(void* ptr)
{
	MemoryHeader* header = MemoryHeader::DetachHeader(ptr);

	const int32 allocSize = header->allocSize;
	ASSERT_CRASH(allocSize > 0);

#ifdef _STOMP
	StompAllocator::Release(header);
#else
	if (allocSize > MAX_ALLOC_SIZE)
	{
		// 메모리 풀링 최대 크기를 벗어나면 일반 해제
		// 여기서 굳이 aligned_free를 쓸 필요는 없지만, 16바이트 자체가 CPU연산에 최적화 되어있다고 하니..
		::_aligned_free(header);
	}
	else
	{
		// 메모리 풀 객체라면 메모리 풀에 반납
		_poolSearchTable[allocSize]->Push(header);
	}
#endif
}
