#pragma once
#include<Windows.h>

#define MAX_QUEUE_LENGTH = 20; // TODO: 이것에 대한 언급이 없으셔서 일단 20으로.. 추후 확인하고 변경 필요하면 변경

template <class T>
class CCircularQueue {
public:
	CCircularQueue(VOID) {
		ZeroMemory(mQueue)
	}

private:
	T mQueue[MAX_QUEUE_LENGTH];
	DWORD mQueueHead;
	DWORD mQueueTail;

public:
	BOOL Begin(VOID) {
		ZeroMemory(mQueue, sizeof(mQueue));
		mQueueHead = mQueueTail = 0;

		return TRUE;
	}

	BOOL End(VOID) { // TODO: 그냥 표시용 용도신 것 같다. 전체적으로 책에 실행이 안되거나 잘못된 부분이 조금씩 있으니 넘어가고 나중에 재확인.
		return TRUE;
	}

	BOOL Push(T data) {
		DWORD TempTail = (mQueueTail + 1) % MAX_QUEUE_LENGTH;
		if (TempTail == mQueueHead)
			return FALSE;

		CopyMemory(&mQueue[TempTail], &data, sizeof(T));

		mQueueTail = TempTail;

		return TRUE;
	}

	BOOL Pop(T& data) {
		if (mQueueHead == mQueueTail)
			return FALSE;

		DWORD TempHead = (mQueueHead + 1) % MAX_QUEUE_LENGTH;

		CopyMemory(&data, &mQueue[TempHead], sizeof(T));

		mQueueHead = TempHead;

		return TRUE;
	}

	BOOL IsEmpty(VOID) {
		if (mQueueHea == mQueueTail)
			return TRUE;
		return FALSE;
	}
};