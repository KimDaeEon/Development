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
	// Lock-Free stack�� �����ϴ� �޸𸮰� �ִٸ� ����
	while (MemoryHeader* header = static_cast<MemoryHeader*>(::InterlockedPopEntrySList(&_header)))
	{
		::_aligned_free(header);
	}
}

void MemoryPool::Push(MemoryHeader* ptr)
{
	// 0�� ���� ��ȯ�� �޸𸮶� ���̴�. �ٽ� ���� ���� placement new�� ���� �ٽ� size�� �Ҵ�ȴ�.
	ptr->allocSize = 0; 

	// ���⼭ PLIST_ENTRY�� ����ȯ�� ������ ������ MemoryHeader�� SLIST_ENTRY�� �Ҵ�޾ұ⿡, 
	// �� ����ü�� �޸� �ּ� �� ó���� ���� �����Ͱ� SLIST_ENTRY�� �Ǿ �̰��� �����ϴ�.
	::InterlockedPushEntrySList(&_header, static_cast<PSLIST_ENTRY>(ptr)); // Pool�� �޸� �ݳ�

	_usedCount.fetch_sub(1);
	_reservedCount.fetch_add(1);
}

MemoryHeader* MemoryPool::Pop()
{
	MemoryHeader* header = static_cast<MemoryHeader*>(::InterlockedPopEntrySList(&_header));

	// ���ٸ� ������ �Ҵ�
	if (header == nullptr)
	{
		// ���⼭�� �޸� �Ҵ��� 16����Ʈ ������ �ȴٴ� ������ ���⿡ _aligned_malloc �Լ��� ���
		header = reinterpret_cast<MemoryHeader*>(::_aligned_malloc(_allocSize, SLIST_ALIGNMENT));
	}
	// ������ Pool�� ���� �ִ� �޸� �����ͼ� ���� ���
	else
	{
		// �̷� ���� �������� Ȥ�ó� �ؼ� üũ
		ASSERT_CRASH(header->allocSize == 0);
		_reservedCount.fetch_sub(1);
	}

	_usedCount.fetch_add(1);
	 
	return header;
}
