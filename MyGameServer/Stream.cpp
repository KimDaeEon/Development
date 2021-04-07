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
/// INT32�� �����͸� �о�� ���� ����մϴ�.
/// </summary>
/// <param name="data"></param>
/// <returns></returns>
BOOL CStream::ReadInt32(INT* data) {
	// CopyMemory �� �̿��ؼ� ������ ���ۿ� ���� �����մϴ�.
	CopyMemory(data, mBufferPointer + mLength, sizeof(INT));

	// ��Ʈ�� ���̸� �������� ���̸�ŭ �����ݴϴ�.
	mLength += sizeof(INT);

	return TRUE;
}

/// <summary>
/// DWORD�� �����͸� ���� �� ����մϴ�.
/// </summary>
/// <param name="data"></param>
/// <returns></returns>
BOOL CStream::ReadDWORD(DWORD* data) {
	CopyMemory(data, mBufferPointer + mLength, sizeof(DWORD));
	mLength += sizeof(DWORD_PTR);
	return TRUE;
}

