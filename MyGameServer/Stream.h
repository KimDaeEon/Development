#pragma once
#include<Windows.h>
class CStream {
public:
	CStream(VOID);
	~CStream(VOID);

private:
	BYTE* mBufferPointer;
	DWORD mLength;

public:
	BOOL SetBuffer(BYTE* buffer);

	BOOL ReadInt32(INT* data);
	BOOL ReadDWORD(DWORD* data);
	BOOL ReadDWORD_PTR(DWORD_PTR* data);
	BOOL ReadByte(BYTE* data);
	BOOL ReadBytes(BYTE* data, DWORD length);
	BOOL ReadFloat(FLOAT* data);
	BOOL ReadInt64(INT64* data);
	BOOL ReadUSHORT(USHORT* data); // TODO: �Լ����� �� USHORT �����ϱ�
	BOOL ReadBOOL(BOOL data); // TODO: �Լ����� �� BOOL �����ϱ�

	DWORD GetLength(VOID);
};