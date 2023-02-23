#pragma once
#include "Allocator.h"

class MemoryPool;

// ---------------------------
//		MemoryPoolManager
// ---------------------------
class MemoryPoolManager
{
	// 4096����Ʈ�� �Ѵ� �����ʹ� �̹� ū ������ �޸�Ǯ�� ������� �ʴ´�.
	enum
	{
		SMALL_BYTES_INTERVAL = 32,
		MEDIUM_BYTES_INTERVAL = 128,
		BIG_BYTES_INTERVAL = 256,

		SMALL_BYTES_SPAN = 1024, // ���� ũ�� �޸�Ǯ ���� ����
		MEDIUM_BYTES_SPAN = 1024, // �߰� ũ�� �޸�Ǯ ���� ����
		BIG_BYTES_SPAN = 2048, // ���� ū ũ�� �޸�Ǯ ���� ����

		// 1024 ����Ʈ���� 32����Ʈ ����
		// 2048 ����Ʈ���� 128����Ʈ ����
		// 4096 ����Ʈ���� 256����Ʈ ����
		POOL_COUNT = (SMALL_BYTES_SPAN / SMALL_BYTES_INTERVAL) + (MEDIUM_BYTES_SPAN / MEDIUM_BYTES_INTERVAL) + (BIG_BYTES_SPAN / BIG_BYTES_INTERVAL),
		MAX_ALLOC_SIZE = SMALL_BYTES_SPAN + MEDIUM_BYTES_SPAN + BIG_BYTES_SPAN
	};


public:
	MemoryPoolManager();
	~MemoryPoolManager();

	void* Allocate(int32 size);
	void Release(void* ptr);

private:
	vector<MemoryPool*> _pools; // ���α׷� ���� �ÿ� pool���� �����ϴ� �뵵

	// �Ҵ��ϴ� �޸� ũ�⺰�� pool�� �����ϰ� �װ��� ������ ã�� ���� ���̺�.
	MemoryPool* _poolSearchTable[MAX_ALLOC_SIZE + 1];
};

// ���� ���ڸ� ���� ���ڰ� ���� ���� �ִ�.
// �⺻������ PoolAllocator(MemoryPool)�� ����, ���ο��� MemoryPool�� ���� StompAllocator�� ������ �ٲ� �� �ִ�.
template<typename Type, typename... Args>
Type* myNew(Args&&... args)
{
	Type* memory = static_cast<Type*>(myAlloc(sizeof(Type)));

	// placement new
	new(memory) Type(std::forward<Args>(args)...);

	return memory;
}

// myRelease�� ��Ȳ�� ���� �ٲ㼭 ����ϰ� ���� �޸� �Ҵ� ����� ����Ѵ�.
template<typename Type>
void myDelete(Type* obj)
{
	obj->~Type();
	myRelease(obj);
}

// �̰� ������ �Ʒ��� shared_ptr<Type> myMakeShared(Args&&... args)�� �߷��� �ȵȴ�.. 
// ���ø� �߷� �켱 ���� ����ص���.
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