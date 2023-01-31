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

// 메모리 오염과 메모리 overflow 상태를 체크하기 위한 할당자
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

// STL에서 요구하는 내용들을 담아 놓은 Custom Allocator
// STL 컨테이너에서 사용이 가능하다.
// TODO: 코드누리 강의를 보면 여기에 들어가야할 요소가 많다.. 추후 필요하면 추가
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