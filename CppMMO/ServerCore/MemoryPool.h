#pragma once
// 요즘에는 메모리 할당기가 잘 되어있어서 MemoryPool을 안쓰는 경우도 있다고 한다.

enum
{
	SLIST_ALIGNMENT = 16
};

// -----------------------
//		MemoryHeader
// -----------------------
// DECLSPEC_ALIGH(16)을 통해 struct가 16바이트의 배수로 만들어지게 된다.
// 이렇게 하는 이유는 CPU관련 연산이 16바이트 배수의 자료구조에 대해 최적화가 되어 있고,
// 이에 맞춰서 Microsoft에서 만든 Lock Free 알고리즘도 작동하기 때문이다.
DECLSPEC_ALIGN(SLIST_ALIGNMENT)
struct MemoryHeader : public SLIST_ENTRY
{
	// [MemoryHeader][Data] 형식으로 메모리가 할당이 된다.
	MemoryHeader(int32 size) : allocSize(size) {}
	
	// 할당한 메모리에 헤더를 달아주고, 사용하는 데이터의 메모리 주소 리턴
	static void* AttachHeader(MemoryHeader* header, int32 size)
	{
		new(header)MemoryHeader(size);
		return reinterpret_cast<void*>(++header); // header*만큼 건너뛰어서 데이터 주소만 전달
	}

	static MemoryHeader* DetachHeader(void* ptr)
	{
		MemoryHeader* header = reinterpret_cast<MemoryHeader*>(ptr) - 1;
		return header;
	}

	int32 allocSize;
	// TODO: 추후에 필요한 정보 추가할 수 있음
};

// --------------------
//		MemoryPool
// --------------------
DECLSPEC_ALIGN(SLIST_ALIGNMENT)
class MemoryPool
{
public:
	MemoryPool(int32 allocSize);
	~MemoryPool();

	void			Push(MemoryHeader* ptr);
	MemoryHeader*	Pop();

private:
	//USE_LOCK;
	SLIST_HEADER			_header; // Lock-Free Stack을 관리하는 구조체
	int32					_allocSize = 0;
	atomic<int32>			_usedCount = 0; // 메모리풀에 할당된 메모리 블록 중 사용되는 갯수
	atomic<int32>			_reservedCount = 0; // 메모리풀에 할당된 메모리 블록 중 사용되지 않은 갯수
};

