#pragma once

// ------------------------
//		BaseAllocator
// ------------------------

class BaseAllocator
{
public:
	static void* Alloc(int32 size);
	static void Release(void* ptr);
};

// -------------------------
//		StompAllocator
// -------------------------

// �޸� ������ �޸� overflow ���¸� üũ�ϱ� ���� �Ҵ���
class StompAllocator
{
	enum
	{
		PAGE_SIZE = 0x1000 // 4KB
	};
public:
	static void* Alloc(int32 size);
	static void Release(void* ptr);
};

// -------------------------
//		STL Allocator
// -------------------------

// STL���� �䱸�ϴ� ������� ��� ���� Custom Allocator
// STL �����̳ʿ��� ����� �����ϴ�.
// TODO: �ڵ崩�� ���Ǹ� ���� ���⿡ ������ ��Ұ� ����.. ���� �ʿ��ϸ� �߰�
template<typename T>
class StlAllocator
{
public:
	using value_type = T;

	StlAllocator() {}

	template<typename Other>
	StlAllocator(const StlAllocator<Other>&) {}

	T* allocate(size_t count)
	{
		const int32 size = static_cast<int32>(count * sizeof(T));
		return static_cast<T*>(myAlloc(size));
	}

	void deallocate(T* ptr, size_t count)
	{
		myRelease(ptr);
	}
};