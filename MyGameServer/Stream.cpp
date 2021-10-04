#include "stdafx.h"
#include "CriticalSection.h"
#include "MultiThreadSync.h"
#include "MemoryPool.h"
#include "Stream.h"

CStream::CStream(VOID) 
{
	mBufferPointer = NULL;
	mLength = 0;
}

CStream::~CStream(VOID) 
{

}


#pragma region CStream Read Function
// 여기부터 Read 함수 시작


/// <summary>
/// 외부의 버퍼를 mBufferPointer 에 세팅.
/// </summary>
/// <param name="buffer"></param>
/// <returns></returns>
BOOL CStream::SetBuffer(BYTE* buffer) 
{
	if (!buffer) return FALSE;

	mBufferPointer = buffer;
	mLength = 0;

	return TRUE;
}

/// <summary>
/// INT32 형 데이터를 읽어올 때에 사용.
/// </summary>
/// <param name="data"></param>
/// <returns></returns>
BOOL CStream::ReadInt32(INT* data) 
{
	// CopyMemory 를 이용해서 지정된 버퍼에 값을 복사.
	CopyMemory(data, mBufferPointer + mLength, sizeof(INT));

	// 스트림 길이를 데이터의 길이만큼 더해줍니다. 앞으로 나오는 Read 함수들은 이런 형식으로 작성.
	mLength += sizeof(INT);

	return TRUE;
}

/// <summary>
/// INT64 형 데이터를 읽을 때 사용.
/// </summary>
/// <param name="data"></param>
/// <returns></returns>
BOOL CStream::ReadInt64(INT64* data) 
{
	CopyMemory(data, mBufferPointer + mLength, sizeof(INT64));
	mLength += sizeof(INT64);
	return TRUE;
}

/// <summary>
/// DWORD 형 데이터를 읽을 때 사용.
/// </summary>
/// <param name="data"></param>
/// <returns></returns>
BOOL CStream::ReadDWORD(DWORD* data) 
{
	CopyMemory(data, mBufferPointer + mLength, sizeof(DWORD));
	mLength += sizeof(DWORD);
	return TRUE;
}

/// <summary>
/// DWORD_PTR 형 데이터를 읽을 때 사용.
/// </summary>
/// <param name="data"></param>
/// <returns></returns>
BOOL CStream::ReadDWORD_PTR(DWORD_PTR* data) 
{
	CopyMemory(data, mBufferPointer + mLength, sizeof(DWORD_PTR));
	mLength += sizeof(DWORD_PTR);
	return TRUE;
}

/// <summary>
/// BYTE 형 데이터를 읽을 때 사용.
/// </summary>
/// <param name="data"></param>
/// <returns></returns>
BOOL CStream::ReadByte(BYTE* data) 
{
	CopyMemory(data, mBufferPointer + mLength, sizeof(BYTE));
	mLength += sizeof(BYTE);
	return TRUE;
}

/// <summary>
/// BYTE 형 데이터를 length 만큼 읽어올 때에 사용.
/// </summary>
/// <param name="data"></param>
/// <param name="length"></param>
/// <returns></returns>
BOOL CStream::ReadBytes(BYTE* data, DWORD length) 
{
	CopyMemory(data, mBufferPointer + mLength, length);
	mLength += length;
	return TRUE;
}

/// <summary>
/// FLOAT 형 데이터를 읽을 때 사용.
/// </summary>
/// <param name="data"></param>
/// <returns></returns>
BOOL CStream::ReadFloat(FLOAT* data) 
{
	CopyMemory(data, mBufferPointer + mLength, sizeof(FLOAT));
	mLength += sizeof(FLOAT);
	return TRUE;
}

/// <summary>
/// SHORT 형 데이터를 읽을 때 사용.
/// </summary>
/// <param name="data"></param>
/// <returns></returns>
BOOL CStream::ReadSHORT(SHORT* data) 
{
	CopyMemory(data, mBufferPointer + mLength, sizeof(SHORT));
	mLength += sizeof(SHORT);
	return TRUE;
}

/// <summary>
/// USHORT 형 데이터를 읽을 때 사용.
/// </summary>
/// <param name="data"></param>
/// <returns></returns>
BOOL CStream::ReadUSHORT(USHORT* data) 
{
	CopyMemory(data, mBufferPointer + mLength, sizeof(USHORT));
	mLength += sizeof(USHORT);
	return TRUE;
}

/// <summary>
/// BOOL 형 데이터를 읽을 때 사용.
/// </summary>
/// <param name="data"></param>
/// <returns></returns>
BOOL CStream::ReadBOOL(BOOL* data) 
{
	CopyMemory(data, mBufferPointer + mLength, sizeof(BOOL));
	mLength += sizeof(BOOL);
	return TRUE;
}

BOOL CStream::ReadWCHAR(WCHAR* data)
{
	memcpy(data, mBufferPointer + mLength, sizeof(WCHAR));

	mLength += sizeof(WCHAR);

	return TRUE;
}

BOOL CStream::ReadWCHARs(LPWSTR data, DWORD length)
{
	memcpy(data, mBufferPointer + mLength, length * sizeof(WCHAR));

	mLength += length * sizeof(WCHAR);

	return TRUE;
}

#pragma endregion


#pragma region CStream Write Function

/// <summary>
/// INT32 형 데이터를 쓸 때에 사용.
/// </summary>
/// <param name="data"></param>
/// <returns></returns>
BOOL CStream::WriteInt32(INT data) 
{
	CopyMemory(mBufferPointer + mLength, &data, sizeof(INT));
	mLength += sizeof(INT);
	return true;
}

/// <summary>
/// DWORD 형 데이터를 쓸 때에 사용.
/// </summary>
/// <param name="data"></param>
/// <returns></returns>
BOOL CStream::WriteDWORD(DWORD data) 
{
	CopyMemory(mBufferPointer + mLength, &data, sizeof(DWORD));
	mLength += sizeof(DWORD);
	return true;
}

/// <summary>
/// DWORD_PTR 형 데이터를 쓸 때에 사용.
/// </summary>
/// <param name="data"></param>
/// <returns></returns>
BOOL CStream::WriteDWORD_PTR(DWORD_PTR data) 
{
	CopyMemory(mBufferPointer + mLength, &data, sizeof(DWORD_PTR));
	mLength += sizeof(DWORD_PTR);
	return true;
}

/// <summary>
/// BYTE 형 데이터를 쓸 때에 사용.
/// </summary>
/// <param name="data"></param>
/// <returns></returns>
BOOL CStream::WriteByte(BYTE data) 
{
	CopyMemory(mBufferPointer + mLength, &data, sizeof(BYTE));
	mLength += sizeof(BYTE);
	return true;
}

/// <summary>
/// 여러 개의 BYTE 형 데이터를 쓸 때에 사용.
/// </summary>
/// <param name="data"></param>
/// <param name="length"></param>
/// <returns></returns>
BOOL CStream::WriteBytes(BYTE* data, DWORD length) 
{
	CopyMemory(mBufferPointer + mLength, data, length);
	mLength += length;
	return true;
}

/// <summary>
/// INT64 형 데이터를 쓸 때 사용.
/// </summary>
/// <param name="data"></param>
/// <returns></returns>
BOOL CStream::WriteInt64(INT64 data) 
{
	CopyMemory(mBufferPointer + mLength, &data, sizeof(INT64));
	mLength += sizeof(INT64);
	return true;
}

/// <summary>
/// SHORT 형 데이터를 쓸 때 사용.
/// </summary>
/// <param name="data"></param>
/// <returns></returns>
BOOL CStream::WriteSHORT(SHORT data) 
{
	CopyMemory(mBufferPointer + mLength, &data, sizeof(SHORT));
	mLength += sizeof(SHORT);
	return true;
}

/// <summary>
/// USHORT 형 데이터를 쓸 때 사용.
/// </summary>
/// <param name="data"></param>
/// <returns></returns>
BOOL CStream::WriteUSHORT(USHORT data) 
{
	CopyMemory(mBufferPointer + mLength, &data, sizeof(USHORT));
	mLength += sizeof(USHORT);
	return true;
}

/// <summary>
/// BOOL 형 데이터를 쓸 때 사용.
/// </summary>
/// <param name="data"></param>
/// <returns></returns>
BOOL CStream::WriteBOOL(BOOL data) 
{
	CopyMemory(mBufferPointer + mLength, &data, sizeof(BOOL));
	mLength += sizeof(BOOL);
	return true;
}

BOOL CStream::WriteWCHAR(WCHAR data)
{
	memcpy(mBufferPointer + mLength, &data, sizeof(WCHAR));

	mLength += sizeof(WCHAR);

	return TRUE;
}

BOOL CStream::WriteWCHARs(LPCWSTR data, DWORD length)
{
	memcpy(mBufferPointer + mLength, data, length * sizeof(WCHAR));

	mLength += length * sizeof(WCHAR);

	return TRUE;
}

/// <summary>
/// 버퍼에 현재 할당된 데이터의 길이(Byte 단위)를 받아올 때에 사용.
/// </summary>
/// <param name=""></param>
/// <returns></returns>
DWORD CStream::GetLength(VOID) 
{
	return mLength;
}
#pragma endregion