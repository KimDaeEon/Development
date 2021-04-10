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

	// Read �Լ�
	BOOL ReadInt32(INT* data);
	BOOL ReadInt64(INT64* data);
	BOOL ReadDWORD(DWORD* data);
	BOOL ReadDWORD_PTR(DWORD_PTR* data);
	BOOL ReadByte(BYTE* data);
	BOOL ReadBytes(BYTE* data, DWORD length);
	BOOL ReadFloat(FLOAT* data);
	BOOL ReadSHORT(SHORT* data); 
	BOOL ReadUSHORT(USHORT* data); 
	BOOL ReadBOOL(BOOL* data); 

	// Write �Լ�
	BOOL WriteInt32(INT data);
	BOOL WriteInt64(INT64 data);
	BOOL WriteDWORD(DWORD data);
	BOOL WriteDWORD_PTR(DWORD_PTR data);
	BOOL WriteByte(BYTE data);
	BOOL WriteBytes(BYTE* data, DWORD length);
	BOOL WriteFloat(FLOAT data);
	BOOL WriteSHORT(SHORT data);
	BOOL WriteUSHORT(USHORT data);
	BOOL WriteBOOL(BOOL data);

	DWORD GetLength(VOID);
};

/// <summary>
/// CStream Ŭ������ ����Ʈ �����ͷ� �����ϱ� ���� ������ Ŭ�����Դϴ�.
/// ����Ʈ �����͸� �̿��Ͽ� ����ϱ� ���ؼ� -> �����ڸ� ������(override) �Ͽ����ϴ�.
/// </summary>
class CStreamSP {
public:
	CStreamSP(VOID){ Stream = new CStream(); }
	~CStreamSP(VOID) { delete Stream; }

	CStream* operator ->(VOID) { return Stream; }
	operator CStream* (VOID) { return Stream; }

private:
	CStream* Stream;
};