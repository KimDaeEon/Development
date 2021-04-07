#include "Stream.h"

CStream::CStream(VOID) {
	mBufferPointer = NULL;
	mLength = 0;
}

CStream::~CStream(VOID) {

}

/// <summary>
/// �ܺ��� ���۸� mBufferPointer �� �����մϴ�.
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
/// INT32 �� �����͸� �о�� ���� ����մϴ�.
/// </summary>
/// <param name="data"></param>
/// <returns></returns>
BOOL CStream::ReadInt32(INT* data) {
	// CopyMemory �� �̿��ؼ� ������ ���ۿ� ���� �����մϴ�.
	CopyMemory(data, mBufferPointer + mLength, sizeof(INT));

	// ��Ʈ�� ���̸� �������� ���̸�ŭ �����ݴϴ�. ������ ������ Read �Լ����� �̷� �������� �ۼ��˴ϴ�.
	mLength += sizeof(INT);

	return TRUE;
}

/// <summary>
/// INT64 �� �����͸� ���� �� ����մϴ�.
/// </summary>
/// <param name="data"></param>
/// <returns></returns>
BOOL CStream::ReadInt64(INT64* data) {
	CopyMemory(data, mBufferPointer + mLength, sizeof(INT64));
	mLength += sizeof(INT64);
	return TRUE;
}

/// <summary>
/// DWORD �� �����͸� ���� �� ����մϴ�.
/// </summary>
/// <param name="data"></param>
/// <returns></returns>
BOOL CStream::ReadDWORD(DWORD* data) {
	CopyMemory(data, mBufferPointer + mLength, sizeof(DWORD));
	mLength += sizeof(DWORD);
	return TRUE;
}

/// <summary>
/// DWORD_PTR �� �����͸� ���� �� ����մϴ�.
/// </summary>
/// <param name="data"></param>
/// <returns></returns>
BOOL CStream::ReadDWORD_PTR(DWORD_PTR* data) {
	CopyMemory(data, mBufferPointer + mLength, sizeof(DWORD_PTR));
	mLength += sizeof(DWORD_PTR);
	return TRUE;
}

/// <summary>
/// BYTE �� �����͸� ���� �� ����մϴ�.
/// </summary>
/// <param name="data"></param>
/// <returns></returns>
BOOL CStream::ReadByte(BYTE* data) {
	CopyMemory(data, mBufferPointer + mLength, sizeof(BYTE));
	mLength += sizeof(BYTE);
	return TRUE;
}

/// <summary>
/// BYTE �� �����͸� length ��ŭ �о�� ���� ����մϴ�.
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
/// FLOAT �� �����͸� ���� �� ����մϴ�.
/// </summary>
/// <param name="data"></param>
/// <returns></returns>
BOOL CStream::ReadFloat(FLOAT* data) {
	CopyMemory(data, mBufferPointer + mLength, sizeof(FLOAT));
	mLength += sizeof(FLOAT);
	return TRUE;
}

/// <summary>
/// SHORT �� �����͸� ���� �� ����մϴ�.
/// </summary>
/// <param name="data"></param>
/// <returns></returns>
BOOL CStream::ReadSHORT(SHORT* data) {
	CopyMemory(data, mBufferPointer + mLength, sizeof(SHORT));
	mLength += sizeof(SHORT);
	return TRUE;
}

/// <summary>
/// USHORT �� �����͸� ���� �� ����մϴ�.
/// </summary>
/// <param name="data"></param>
/// <returns></returns>
BOOL CStream::ReadUSHORT(USHORT* data) {
	CopyMemory(data, mBufferPointer + mLength, sizeof(USHORT));
	mLength += sizeof(USHORT);
	return TRUE;
}

/// <summary>
/// BOOL �� �����͸� ���� �� ����մϴ�.
/// </summary>
/// <param name="data"></param>
/// <returns></returns>
BOOL CStream::ReadBOOL(BOOL* data) {
	CopyMemory(data, mBufferPointer + mLength, sizeof(BOOL));
	mLength += sizeof(BOOL);
	return TRUE;
}
