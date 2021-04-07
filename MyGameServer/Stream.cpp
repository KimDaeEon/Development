#include "Stream.h"

CStream::CStream(VOID) {
	mBufferPointer = NULL;
	mLength = 0;
}

CStream::~CStream(VOID) {

}

/// <summary>
/// 외부의 버퍼를 mBufferPointer 에 세팅합니다.
/// </summary>
/// <param name="buffer"></param>
/// <returns></returns>
BOOL CStream::SetBuffer(BYTE* buffer) {
	if (!buffer) return FALSE;

	mBufferPointer = buffer;
	mLength = 0;

	return TRUE;
}

/// <summary>
/// INT32형 데이터를 읽어올 때에 사용합니다.
/// </summary>
/// <param name="data"></param>
/// <returns></returns>
BOOL CStream::ReadInt32(INT* data) {
	// CopyMemory 를 이용해서 지정된 버퍼에 값을 복사합니다.
	CopyMemory(data, mBufferPointer + mLength, sizeof(INT));

	// 스트림 길이를 데이터의 길이만큼 더해줍니다.
	mLength += sizeof(INT);

	return TRUE;
}

/// <summary>
/// DWORD형 데이터를 읽을 때 사용합니다.
/// </summary>
/// <param name="data"></param>
/// <returns></returns>
BOOL CStream::ReadDWORD(DWORD* data) {
	CopyMemory(data, mBufferPointer + mLength, sizeof(DWORD));
	mLength += sizeof(DWORD_PTR);
	return TRUE;
}

