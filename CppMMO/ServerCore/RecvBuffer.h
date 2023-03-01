#pragma once

// ---------------------------
//		   RecvBuffer
// ---------------------------

// [r][][][w][][]�� ���� ������ r <= index < w ������ �����͵��� ���� ���� ���� ��ȿ�� �������̴�.
class RecvBuffer
{
	enum
	{
		BUFFER_COUNT = 10
	};

public:
	RecvBuffer(int32 bufferSize);
	~RecvBuffer();

	void			Relocate(); // _readPos, _writePos ���ġ
	bool			OnRead(int32 numOfBytes);
	bool			OnWrite(int32 numOfBytes);

	BYTE*			ReadPos() { return &_buffer[_readPos]; }
	BYTE*			WritePos() { return &_buffer[_readPos]; }
	int32			DataSize() { return _writePos - _readPos; }
	int32			FreeSize() { return _capacity - _writePos; }

private:
	int32			_capacity = 0; // �������� ���� ũ��, _writePos�� _readPos�� ��ĥ �� �ֵ��� ������ �α� ���� ��
	int32			_bufferSize = { 0 }; // �� ���� �� �� �ִ� ���� ũ��
	int32			_readPos = { 0 }; // ���� ������ �ε���
	int32			_writePos = { 0 }; // WSARecv�� �Էµ� ������ �������� ���� �ε��� (���� ������ �Է¹��� �� �ִ� ù �ε���)
	myVector<BYTE>	_buffer;
};

