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
// ������� Read �Լ� ����


/// <summary>
/// �ܺ��� ���۸� mBufferPointer �� ����.
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
/// INT32 �� �����͸� �о�� ���� ���.
/// </summary>
/// <param name="data"></param>
/// <returns></returns>
BOOL CStream::ReadInt32(INT* data) 
{
	// CopyMemory �� �̿��ؼ� ������ ���ۿ� ���� ����.
	CopyMemory(data, mBufferPointer + mLength, sizeof(INT));

	// ��Ʈ�� ���̸� �������� ���̸�ŭ �����ݴϴ�. ������ ������ Read �Լ����� �̷� �������� �ۼ�.
	mLength += sizeof(INT);

	return TRUE;
}

/// <summary>
/// INT64 �� �����͸� ���� �� ���.
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
/// DWORD �� �����͸� ���� �� ���.
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
/// DWORD_PTR �� �����͸� ���� �� ���.
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
/// BYTE �� �����͸� ���� �� ���.
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
/// BYTE �� �����͸� length ��ŭ �о�� ���� ���.
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
/// FLOAT �� �����͸� ���� �� ���.
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
/// SHORT �� �����͸� ���� �� ���.
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
/// USHORT �� �����͸� ���� �� ���.
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
/// BOOL �� �����͸� ���� �� ���.
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
/// INT32 �� �����͸� �� ���� ���.
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
/// DWORD �� �����͸� �� ���� ���.
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
/// DWORD_PTR �� �����͸� �� ���� ���.
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
/// BYTE �� �����͸� �� ���� ���.
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
/// ���� ���� BYTE �� �����͸� �� ���� ���.
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
/// INT64 �� �����͸� �� �� ���.
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
/// SHORT �� �����͸� �� �� ���.
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
/// USHORT �� �����͸� �� �� ���.
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
/// BOOL �� �����͸� �� �� ���.
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
/// ���ۿ� ���� �Ҵ�� �������� ����(Byte ����)�� �޾ƿ� ���� ���.
/// </summary>
/// <param name=""></param>
/// <returns></returns>
DWORD CStream::GetLength(VOID) 
{
	return mLength;
}
#pragma endregion