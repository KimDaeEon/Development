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
/// INT32 형 데이터를 읽어올 때에 사용합니다.
/// </summary>
/// <param name="data"></param>
/// <returns></returns>
BOOL CStream::ReadInt32(INT* data) {
	// CopyMemory 를 이용해서 지정된 버퍼에 값을 복사합니다.
	CopyMemory(data, mBufferPointer + mLength, sizeof(INT));

	// 스트림 길이를 데이터의 길이만큼 더해줍니다. 앞으로 나오는 Read 함수들은 이런 형식으로 작성됩니다.
	mLength += sizeof(INT);

	return TRUE;
}

/// <summary>
/// INT64 형 데이터를 읽을 때 사용합니다.
/// </summary>
/// <param name="data"></param>
/// <returns></returns>
BOOL CStream::ReadInt64(INT64* data) {
	CopyMemory(data, mBufferPointer + mLength, sizeof(INT64));
	mLength += sizeof(INT64);
	return TRUE;
}

/// <summary>
/// DWORD 형 데이터를 읽을 때 사용합니다.
/// </summary>
/// <param name="data"></param>
/// <returns></returns>
BOOL CStream::ReadDWORD(DWORD* data) {
	CopyMemory(data, mBufferPointer + mLength, sizeof(DWORD));
	mLength += sizeof(DWORD);
	return TRUE;
}

/// <summary>
/// DWORD_PTR 형 데이터를 읽을 때 사용합니다.
/// </summary>
/// <param name="data"></param>
/// <returns></returns>
BOOL CStream::ReadDWORD_PTR(DWORD_PTR* data) {
	CopyMemory(data, mBufferPointer + mLength, sizeof(DWORD_PTR));
	mLength += sizeof(DWORD_PTR);
	return TRUE;
}

/// <summary>
/// BYTE 형 데이터를 읽을 때 사용합니다.
/// </summary>
/// <param name="data"></param>
/// <returns></returns>
BOOL CStream::ReadByte(BYTE* data) {
	CopyMemory(data, mBufferPointer + mLength, sizeof(BYTE));
	mLength += sizeof(BYTE);
	return TRUE;
}

/// <summary>
/// BYTE 형 데이터를 length 만큼 읽어올 때에 사용합니다.
/// </summary>
/// <param name="data"></param>
/// <param name="length"></param>
/// <returns></returns>
BOOL CStream::ReadBytes(BYTE* data, DWORD length) {
	CopyMemory(data, mBufferPointer + mLength, length);
	mLength += length;
	return TRUE;
}

/// <summary>
/// FLOAT 형 데이터를 읽을 때 사용합니다.
/// </summary>
/// <param name="data"></param>
/// <returns></returns>
BOOL CStream::ReadFloat(FLOAT* data) {
	CopyMemory(data, mBufferPointer + mLength, sizeof(FLOAT));
	mLength += sizeof(FLOAT);
	return TRUE;
}

/// <summary>
/// SHORT 형 데이터를 읽을 때 사용합니다.
/// </summary>
/// <param name="data"></param>
/// <returns></returns>
BOOL CStream::ReadSHORT(SHORT* data) {
	CopyMemory(data, mBufferPointer + mLength, sizeof(SHORT));
	mLength += sizeof(SHORT);
	return TRUE;
}

/// <summary>
/// USHORT 형 데이터를 읽을 때 사용합니다.
/// </summary>
/// <param name="data"></param>
/// <returns></returns>
BOOL CStream::ReadUSHORT(USHORT* data) {
	CopyMemory(data, mBufferPointer + mLength, sizeof(USHORT));
	mLength += sizeof(USHORT);
	return TRUE;
}

/// <summary>
/// BOOL 형 데이터를 읽을 때 사용합니다.
/// </summary>
/// <param name="data"></param>
/// <returns></returns>
BOOL CStream::ReadBOOL(BOOL* data) {
	CopyMemory(data, mBufferPointer + mLength, sizeof(BOOL));
	mLength += sizeof(BOOL);
	return TRUE;
}
