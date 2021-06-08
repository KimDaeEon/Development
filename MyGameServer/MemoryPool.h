#pragma once
#include "Global.h"

template <class T, int ALLOC_BLOCK_SIZE = 50>
class CMemoryPool 
{
public:
	static VOID* operator new(size_t allocLength) { // TODO: å������ std::size_t ���µ� �̷��� �ᵵ ���̰� ���ٰ� ���Ҵ�. Ȥ�� �𸣴� �ϴ� üũ.
		
		assert(sizeof(T) == allocLength); // allocLength �� sizeof(T) �� �ٸ��� ����.
		assert(sizeof(T) >= sizeof(UCHAR*)); // sizeof(T) �� 4����Ʈ ���� ������ ����. �޸� Ǯ�� �̻��� ũ��� �Ҵ��� �� ���̴�.

		// ���� �� �̻� �Ҵ��� �� �ִ� ������ ���� ��� ���� �޸� ���� �߰� �Ҵ�
		if (!mFreePointer)
			allockBlock();

		UCHAR* returnPointer = mFreePointer;

		// mPreePointer �� �� �ִ� ���� ���� �޸� ���� ���� �ּҸ� ����Ű�� �ִ�. �Ʒ��� ���� ������ �ϸ� ���� �޸� ���� ���� �ּҸ� ����Ű�� �ȴ�.
		// ���̳ʸ� ��(�ּ� ��)�� �����ϴ� retinerpret_cast �� �Ἥ ��� UCHAR* ���� �����ϸ鼭 ���� �Ҵ� ������ ���� �ּҸ� mFreePointer �� ����Ű���� �ϴ� ���̴�.
		mFreePointer = *reinterpret_cast<UCHAR**>(returnPointer);

		return returnPointer;
	}

	static VOID operator delete(VOID* deletePointer) { // ���α׷� ���� �ÿ��� �ڵ����� �޸𸮰� ��������, ��� ���� �ø� �޸𸮴� ���� ���� ���� ���� ���̶�..
		*reinterpret_cast<UCHAR**>(deletePointer) = mFreePointer; // ��ȯ�Ǵ� �޸� �� ���� ���� mFreePointer �� ���� �޸� ������ ����
		mFreePointer = static_cast<UCHAR*>(deletePointer); // mFreePointer �� ��ȯ�� �޸� ���� ����Ű���� ����, �̷��� �����ϸ� mFreePointer �� deletePointer �� �־ ���� ���� ó���� �ʿ䰡 ����!! 
	}

private:
	static VOID allockBlock() {
		mFreePointer = new UCHAR[sizeof(T) * ALLOC_BLOCK_SIZE]; // T �� Ÿ���� ũ�⸸ŭ 50���� �Ҵ�
		
		UCHAR** current	= reinterpret_cast<UCHAR**>(mFreePointer); // �Ҵ��� �޸� ���� �����͸� current �� �Ҵ�
		UCHAR* next		= mFreePointer; // ����� �Ҵ�� ù �޸� ���� �ּ�, �Ʒ� ���꿡�� ���� ���� �ּҸ� ����Ű�� �뵵�� ���ȴ�.

		for(INT i = 0; i < ALLOC_BLOCK_SIZE - 1; ++i) {
			next		+= sizeof(T); // ���� �� �ּҸ�ŭ �̵�.
			*current	= next; // current �� ���� ���� �ּҸ� ����Ű���� �ش� ���� �Ҵ�.
			current		= reinterpret_cast<UCHAR**>(next); // UCHAR* ������ �����ϸ鼭 �޸� ���� �� ù 4����Ʈ�� ���� ���� �ּ� ���� �Ҵ��ϱ� ���� UCHAR ** ������ ��ȯ.
		}

		*current = 0; // ������ ���� ���� ���� ���� �����Ƿ� ù 4����Ʈ�� NULL �� �Ҵ�.
	}

private:
	static UCHAR* mFreePointer;

protected:
	~CMemoryPool() { // �ı��� ��� �����ϰ� ����

	}
};

template <class T, int ALLOCK_BLOCK_SIZE>
UCHAR* CMemoryPool<T, ALLOCK_BLOCK_SIZE>::mFreePointer; // Ŭ���� ���� ����� �̷� ������ �ʱ�ȭ�� ���Ѿ� �Ѵ�. ���� �Է����� ������ �ڵ����� 0�� �ȴ�. (BSS)