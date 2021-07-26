#pragma once
#include "Global.h"
  
typedef struct _QUEUE_DATA
{
	VOID* Object;
	BYTE	Data[MAX_BUFFER_LENGTH];
	DWORD	DataLength;

	CHAR	RemoteAddress[32];
	USHORT	RemotePort;

	DWORD	Protocol;
} QUEUE_DATA;

class CCircularQueue : public CMultiThreadSync<CCircularQueue>
{
public:
	CCircularQueue(VOID);
	~CCircularQueue(VOID);

private:
	QUEUE_DATA	mQueue[MAX_QUEUE_LENGTH];
	DWORD		mQueueHead;
	DWORD		mQueueTail;

public:
	BOOL		Begin(VOID);
	BOOL		End(VOID);

	BYTE* Push(VOID* object, BYTE* data, DWORD dataLength);
	BYTE* Push(VOID* object, BYTE* data, DWORD dataLength, LPCSTR remoteAddress, USHORT remotePort);
	BYTE* Push(VOID* object, DWORD protocol, BYTE* data, DWORD dataLength);
	BYTE* Push(VOID* object, DWORD protocol, BYTE* data, DWORD dataLength, LPCSTR remoteAddress, USHORT remotePort);

	BOOL		Pop(VOID** object, BYTE* data, DWORD& dataLength);
	BOOL		Pop(VOID** object, BYTE* data, DWORD& dataLength, LPSTR remoteAddress, USHORT& remotePort);
	BOOL		Pop(VOID** object, DWORD& protocol, BYTE* data, DWORD& dataLength);
	BOOL		Pop(VOID** object, DWORD& protocol, BYTE* data, DWORD& dataLength, LPSTR remoteAddress, USHORT& remotePort);
	BOOL		Pop(VOID);

	BOOL		IsEmpty(VOID);
};


// // 예전 부분 주석
//template <class t>
//class CCircularQueue{
//public:
//	CCircularQueue(VOID) {
//		ZeroMemory(mQueue);
//	}
//
//private:
//	T mQueue[MAX_QUEUE_LENGTH];
//	DWORD mQueueHead;
//	DWORD mQueueTail;
//
//public:
//	BOOL Begin(VOID) {
//		ZeroMemory(mQueue, sizeof(mQueue));
//		mQueueHead = mQueueTail = 0;
//
//		return TRUE;
//	}
//
//	BOOL End(VOID) { // TODO: 그냥 표시용 용도신 것 같다. 전체적으로 책에 실행이 안되거나 잘못된 부분이 조금씩 있으니 넘어가고 나중에 재확인.
//		return TRUE;
//	}
//
//	BOOL Push(T data) {
//		DWORD TempTail = (mQueueTail + 1) % MAX_QUEUE_LENGTH;
//		if (TempTail == mQueueHead)
//			return FALSE;
//
//		CopyMemory(&mQueue[TempTail], &data, sizeof(T));
//
//		mQueueTail = TempTail;
//
//		return TRUE;
//	}
//
//	BOOL Pop(T& data) {
//		if (mQueueHead == mQueueTail)
//			return FALSE;
//
//		DWORD TempHead = (mQueueHead + 1) % MAX_QUEUE_LENGTH;
//
//		CopyMemory(&data, &mQueue[TempHead], sizeof(T));
//
//		mQueueHead = TempHead;
//
//		return TRUE;
//	}
//
//	BOOL IsEmpty(VOID) {
//		if (mQueueHea == mQueueTail)
//			return TRUE;
//		return FALSE;
//	}
//};