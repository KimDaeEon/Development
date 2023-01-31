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
	// ���� size�� Ŀ���� �� �ִ� ������ ���ڸ� ����Ѵ�. -1 �κ� �߿��ϴ� �� ��������.
	const int64 pageCount = (size + PAGE_SIZE - 1) / PAGE_SIZE;

	// �����͸� �� �ں��� �Ҵ�
	const int64 dataOffset = pageCount * PAGE_SIZE - size;

	// �� ������� �Ҵ��� �ؾ� ������ �޸𸮸� �ǵ帮���� �� ���� exception�� �ٷ� ȣ��ȴ�.
	void* baseAddress = ::VirtualAlloc(NULL, pageCount * PAGE_SIZE, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	return static_cast<void*>(static_cast<int8*>(baseAddress) + dataOffset);
}

void StompAllocator::Release(void* ptr)
{
	const int64 address = reinterpret_cast<int64>(ptr);
	const int64 baseAddress = address - (address % PAGE_SIZE);

	// MEM_RELEASE �ɼ��� �ָ� �Ʒ� �Լ� �� ��° ���ڴ� 0�� �־�� �Ѵ�.
	// MEM_DECOMMIT �ɼ��� �ָ� ��ó�� ���� �ּҺ��� �� ��° ����ũ�⸸ŭ �޸𸮸� ����
	::VirtualFree(reinterpret_cast<void*>(baseAddress), 0, MEM_RELEASE);
}
