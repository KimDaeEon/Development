#include "pch.h"
#include "Memory.h"
#include "MemoryPool.h"

// ---------------------------
//		MemoryPoolManager
// ---------------------------
MemoryPoolManager::MemoryPoolManager()
{
	int32 size = 0;
	
	// 0~32�� ���̺��� 0~32����Ʈ ������ �����͸� �Ҵ��� ���� ���ǰ�,
	// �� ���� 32����Ʈ�� �޸𸮸� �Ҵ��ϴ� MemoryPool�� ���ȴ�.
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
	const int32 allocSize = size + sizeof(MemoryHeader); // �޸� ����� �߰��ؾ��ϹǷ� �� ������� �̷��� ���ȴ�.
#ifdef _STOMP
	header = reinterpret_cast<MemoryHeader*>(StompAllocator::Alloc(allocSize));
#else
	if (allocSize > MAX_ALLOC_SIZE)
	{
		// �޸� Ǯ�� �ִ� ũ�⸦ ����� �Ϲ� �Ҵ�
		header = reinterpret_cast<MemoryHeader*>(::_aligned_malloc(allocSize, SLIST_ALIGNMENT));
	}
	else
	{
		// ũ�Ⱑ ������ �޸� Ǯ���� �����´�.
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
		// �޸� Ǯ�� �ִ� ũ�⸦ ����� �Ϲ� ����
		// ���⼭ ���� aligned_free�� �� �ʿ�� ������, 16����Ʈ ��ü�� CPU���꿡 ����ȭ �Ǿ��ִٰ� �ϴ�..
		::_aligned_free(header);
	}
	else
	{
		// �޸� Ǯ ��ü��� �޸� Ǯ�� �ݳ�
		_poolSearchTable[allocSize]->Push(header);
	}
#endif
}
