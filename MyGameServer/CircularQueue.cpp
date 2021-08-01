#include "stdafx.h"
#include "CriticalSection.h"
#include "MultiThreadSync.h"
#include "CircularQueue.h"

// TODO: 이 부분 바뀐 것이 너무 많아서 일단 복사했다. 나중에 코드 다시 보면서 이해하자.

CCircularQueue::CCircularQueue(VOID)
{
	memset(mQueue, 0, sizeof(mQueue));

	mQueueHead = 0;
	mQueueTail = 0;
}

CCircularQueue::~CCircularQueue(VOID)
{
}

BOOL CCircularQueue::Begin(VOID)
{
	memset(mQueue, 0, sizeof(mQueue));

	mQueueHead = 0;
	mQueueTail = 0;

	return TRUE;
}

BOOL CCircularQueue::End(VOID)
{
	return TRUE;
}

/// <summary>
/// 데이터 포인터를 받으면 그것을 복사해서 저장하고, 그 저장된 데이터의 포인터를 리턴한다.
/// 이로 인해 임시 변수의 데이터를 받아도 그것을 큐에 저장해놓을 수 있다.
/// </summary>
/// <param name="object"></param>
/// <param name="data"></param>
/// <param name="dataLength"></param>
/// <returns></returns>
BYTE* CCircularQueue::Push(VOID* object, BYTE* data, DWORD dataLength)
{
	CThreadSync Sync;

	//CLog::WriteLog(_T("Queue Status  : Head (%d) Tail (%d)\n"), mQueueHead, mQueueTail);

	if (!object || !data)
		return NULL;

	DWORD TempTail = (mQueueTail + 1) % MAX_QUEUE_LENGTH;

	if (TempTail == mQueueHead)
		return NULL;

	mQueue[TempTail].Object = object;
	mQueue[TempTail].DataLength = dataLength;

	// 여기서 복사를 해두기에 임시로 만든 데이터를 저장해둘 수 있는 것이다.
	memcpy(mQueue[TempTail].Data, data, dataLength);

	mQueueTail = TempTail;

	return mQueue[TempTail].Data;
}

BYTE* CCircularQueue::Push(VOID* object, DWORD protocol, BYTE* data, DWORD dataLength)
{
	CThreadSync Sync;

	//CLog::WriteLog(_T("Queue Status  : Head (%d) Tail (%d)\n"), mQueueHead, mQueueTail);

	if (!object || !data)
		return NULL;

	DWORD TempTail = (mQueueTail + 1) % MAX_QUEUE_LENGTH;

	if (TempTail == mQueueHead)
		return NULL;

	mQueue[TempTail].Object = object;
	mQueue[TempTail].DataLength = dataLength;
	mQueue[TempTail].Protocol = protocol;

	memset(mQueue[TempTail].Data, 0, sizeof(mQueue[TempTail].Data));
	memcpy(mQueue[TempTail].Data, data, dataLength);

	mQueueTail = TempTail;

	return mQueue[TempTail].Data;
}

BYTE* CCircularQueue::Push(VOID* object, BYTE* data, DWORD dataLength, LPCSTR remoteAddress, USHORT remotePort)
{
	CThreadSync Sync;

	//CLog::WriteLog(_T("Queue Status  : Head (%d) Tail (%d)\n"), mQueueHead, mQueueTail);

	if (!object || !data)
		return NULL;

	DWORD TempTail = (mQueueTail + 1) % MAX_QUEUE_LENGTH;

	if (TempTail == mQueueHead)
		return NULL;

	mQueue[TempTail].Object = object;
	mQueue[TempTail].DataLength = dataLength;
	mQueue[TempTail].RemotePort = remotePort;

	memset(mQueue[TempTail].RemoteAddress, 0, sizeof(mQueue[TempTail].RemoteAddress));
	strncpy(mQueue[TempTail].RemoteAddress, remoteAddress, sizeof(mQueue[TempTail].RemoteAddress));

	memset(mQueue[TempTail].Data, 0, sizeof(mQueue[TempTail].Data));
	memcpy(mQueue[TempTail].Data, data, dataLength);

	mQueueTail = TempTail;

	return mQueue[TempTail].Data;
}

BYTE* CCircularQueue::Push(VOID* object, DWORD protocol, BYTE* data, DWORD dataLength, LPCSTR remoteAddress, USHORT remotePort)
{
	CThreadSync Sync;

	//CLog::WriteLog(_T("Queue Status  : Head (%d) Tail (%d)\n"), mQueueHead, mQueueTail);

	if (!object || !data)
		return NULL;

	DWORD TempTail = (mQueueTail + 1) % MAX_QUEUE_LENGTH;

	if (TempTail == mQueueHead)
		return NULL;

	mQueue[TempTail].Object = object;
	mQueue[TempTail].DataLength = dataLength;
	mQueue[TempTail].RemotePort = remotePort;
	mQueue[TempTail].Protocol = protocol;

	memset(mQueue[TempTail].RemoteAddress, 0, sizeof(mQueue[TempTail].RemoteAddress));
	strncpy(mQueue[TempTail].RemoteAddress, remoteAddress, sizeof(mQueue[TempTail].RemoteAddress));

	memset(mQueue[TempTail].Data, 0, sizeof(mQueue[TempTail].Data));
	memcpy(mQueue[TempTail].Data, data, dataLength);

	mQueueTail = TempTail;

	return mQueue[TempTail].Data;
}

BOOL CCircularQueue::Pop(VOID)
{
	CThreadSync Sync;

	//CLog::WriteLog(_T("Queue Status  : Head (%d) Tail (%d)\n"), mQueueHead, mQueueTail);

	if (mQueueHead == mQueueTail)
		return FALSE;

	DWORD TempHead = (mQueueHead + 1) % MAX_QUEUE_LENGTH;

	mQueueHead = TempHead;

	return TRUE;
}

BOOL CCircularQueue::Pop(VOID** object, BYTE* data, DWORD& dataLength)
{
	CThreadSync Sync;

	//CLog::WriteLog(_T("Queue Status  : Head (%d) Tail (%d)\n"), mQueueHead, mQueueTail);

	if (!object || !data)
		return FALSE;

	if (mQueueHead == mQueueTail)
		return FALSE;

	DWORD TempHead = (mQueueHead + 1) % MAX_QUEUE_LENGTH;

	dataLength = mQueue[TempHead].DataLength;
	*object = mQueue[TempHead].Object;

	memcpy(data, mQueue[TempHead].Data, mQueue[TempHead].DataLength);

	mQueueHead = TempHead;

	return TRUE;
}

BOOL CCircularQueue::Pop(VOID** object, DWORD& protocol, BYTE* data, DWORD& dataLength)
{
	CThreadSync Sync;

	//CLog::WriteLog(_T("Queue Status  : Head (%d) Tail (%d)\n"), mQueueHead, mQueueTail);

	if (!object || !data)
		return FALSE;

	if (mQueueHead == mQueueTail)
		return FALSE;

	DWORD TempHead = (mQueueHead + 1) % MAX_QUEUE_LENGTH;

	dataLength = mQueue[TempHead].DataLength;
	*object = mQueue[TempHead].Object;
	protocol = mQueue[TempHead].Protocol;

	memcpy(data, mQueue[TempHead].Data, mQueue[TempHead].DataLength);

	mQueueHead = TempHead;

	return TRUE;
}

BOOL CCircularQueue::Pop(VOID** object, BYTE* data, DWORD& dataLength, LPSTR remoteAddress, USHORT& remotePort)
{
	CThreadSync Sync;

	//CLog::WriteLog(_T("Queue Status  : Head (%d) Tail (%d)\n"), mQueueHead, mQueueTail);

	if (!object || !data)
		return FALSE;

	if (mQueueHead == mQueueTail)
		return FALSE;

	DWORD TempHead = (mQueueHead + 1) % MAX_QUEUE_LENGTH;

	dataLength = mQueue[TempHead].DataLength;
	*object = mQueue[TempHead].Object;
	remotePort = mQueue[TempHead].RemotePort;

	strncpy(remoteAddress, mQueue[TempHead].RemoteAddress, sizeof(mQueue[TempHead].RemoteAddress));
	memcpy(data, mQueue[TempHead].Data, mQueue[TempHead].DataLength);

	mQueueHead = TempHead;

	return TRUE;
}

BOOL CCircularQueue::Pop(VOID** object, DWORD& protocol, BYTE* data, DWORD& dataLength, LPSTR remoteAddress, USHORT& remotePort)
{
	CThreadSync Sync;

	//CLog::WriteLog(_T("Queue Status  : Head (%d) Tail (%d)\n"), mQueueHead, mQueueTail);

	if (!object || !data)
		return FALSE;

	if (mQueueHead == mQueueTail)
		return FALSE;

	DWORD TempHead = (mQueueHead + 1) % MAX_QUEUE_LENGTH;

	dataLength = mQueue[TempHead].DataLength;
	*object = mQueue[TempHead].Object;
	remotePort = mQueue[TempHead].RemotePort;
	protocol = mQueue[TempHead].Protocol;

	strncpy(remoteAddress, mQueue[TempHead].RemoteAddress, sizeof(mQueue[TempHead].RemoteAddress));
	memcpy(data, mQueue[TempHead].Data, mQueue[TempHead].DataLength);

	mQueueHead = TempHead;

	return TRUE;
}

BOOL CCircularQueue::IsEmpty(VOID)
{
	CThreadSync Sync;

	if (mQueueHead == mQueueTail)
		return TRUE;

	return FALSE;
}
