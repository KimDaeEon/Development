#pragma once
#include "Global.h"

template <class T, int ALLOC_BLOCK_SIZE = 50>
class CMemoryPool 
{
public:
	static VOID* operator new(size_t allocLength) { // TODO: 책에서는 std::size_t 였는데 이렇게 써도 차이가 없다고 보았다. 혹시 모르니 일단 체크.
		
		assert(sizeof(T) == allocLength); // allocLength 와 sizeof(T) 가 다르면 중지.
		assert(sizeof(T) >= sizeof(UCHAR*)); // sizeof(T) 가 4바이트 보다 작으면 중지. 메모리 풀이 이상한 크기로 할당이 된 것이다.

		// 만약 더 이상 할당할 수 있는 공간이 없을 경우 새로 메모리 블럭을 추가 할당
		if (!mFreePointer)
			allockBlock();

		UCHAR* returnPointer = mFreePointer;

		// mPreePointer 에 들어가 있는 값은 다음 메모리 블럭의 시작 주소를 가리키고 있다. 아래와 같이 연산을 하면 다음 메모리 블럭의 시작 주소를 가리키게 된다.
		// 바이너리 값(주소 값)을 유지하는 retinerpret_cast 를 써서 계속 UCHAR* 형을 유지하면서 다음 할당 가능한 블럭의 주소를 mFreePointer 가 가리키도록 하는 것이다.
		mFreePointer = *reinterpret_cast<UCHAR**>(returnPointer);

		return returnPointer;
	}

	static VOID operator delete(VOID* deletePointer) { // 프로그램 종료 시에는 자동으로 메모리가 내려가고, 사실 여기 올린 메모리는 내릴 일이 거의 없는 것이라서..
		*reinterpret_cast<UCHAR**>(deletePointer) = mFreePointer; // 반환되는 메모리 블럭 기준 현재 mFreePointer 를 다음 메모리 블럭으로 설정
		mFreePointer = static_cast<UCHAR*>(deletePointer); // mFreePointer 가 반환된 메모리 블럭을 가리키도록 설정, 이렇게 구현하면 mFreePointer 를 deletePointer 에 넣어도 딱히 예외 처리할 필요가 없다!! 
	}

private:
	static VOID allockBlock() {
		mFreePointer = new UCHAR[sizeof(T) * ALLOC_BLOCK_SIZE]; // T 형 타입의 크기만큼 50개씩 할당
		
		UCHAR** current	= reinterpret_cast<UCHAR**>(mFreePointer); // 할당한 메모리 블럭의 포인터를 current 에 할당
		UCHAR* next		= mFreePointer; // 현재는 할당된 첫 메모리 블럭의 주소, 아래 연산에서 다음 블럭의 주소를 가리키는 용도로 사용된다.

		for(INT i = 0; i < ALLOC_BLOCK_SIZE - 1; ++i) {
			next		+= sizeof(T); // 다음 블럭 주소만큼 이동.
			*current	= next; // current 가 다음 블럭의 주소를 가리키도록 해당 값을 할당.
			current		= reinterpret_cast<UCHAR**>(next); // UCHAR* 형식을 유지하면서 메모리 블럭의 맨 첫 4바이트에 다음 블럭의 주소 값을 할당하기 위해 UCHAR ** 형으로 변환.
		}

		*current = 0; // 마지막 블럭의 경우는 다음 블럭이 없으므로 첫 4바이트를 NULL 로 할당.
	}

private:
	static UCHAR* mFreePointer;

protected:
	~CMemoryPool() { // 파괴자 상속 가능하게 구성

	}
};

template <class T, int ALLOCK_BLOCK_SIZE>
UCHAR* CMemoryPool<T, ALLOCK_BLOCK_SIZE>::mFreePointer; // 클래스 정적 멤버는 이런 식으로 초기화를 시켜야 한다. 값을 입력하지 않으면 자동으로 0이 된다. (BSS)