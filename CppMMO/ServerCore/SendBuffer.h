#pragma once

class SendBufferChunk;

// ----------------------------
//			SendBuffer
// ----------------------------
class SendBuffer
{
public:
	SendBuffer(SendBufferChunkRef owner, BYTE* buffer, uint32 allocSize);
	~SendBuffer();

	BYTE*		Buffer() { return _buffer; }
	uint32		AllocSize() { return _allocSize; }
	uint32		WriteSize() { return _writeSize; }
	void		Close(uint32 writeSize);

private:
	BYTE*				_buffer; // SendBufferChunk���� �޾ƿ� �����͸� ��� �ִ´�.
	uint32				_allocSize = 0;
	uint32				_writeSize = 0;
	SendBufferChunkRef	_owner; // SendBufferChunk�� �Ҵ���� �޸𸮸� ��� �߿� �������� �ʵ��� �ϱ� ���� ����
};

// ----------------------------------
//			SendBufferChunk
// ----------------------------------
// ��û ū ���� ���� ����� �Ҵ�޾Ƴ���, �߶� ����Ѵ�.
// �ش� Ŭ������ Thread Local�� ���Ǳ⿡ lock�� ���� �ʴ´�.
class SendBufferChunk : public enable_shared_from_this<SendBufferChunk>
{
	enum
	{
		SEND_BUFFER_CHUNK_SIZE = 0x10000 // 65536
	};

public:
	SendBufferChunk();
	~SendBufferChunk();

	void			Reset();
	SendBufferRef	Open(uint32 allocSize);
	void			Close(uint32 writeSize);

	bool			IsOpen() { return _isOpen; }
	BYTE*			Buffer() { return &_buffer[_usedSize]; }
	uint32			FreeSize() { return static_cast<uint32>(_buffer.size()) - _usedSize; }

private:
	myArray<BYTE, SEND_BUFFER_CHUNK_SIZE>	_buffer = {};
	bool									_isOpen = { false };
	uint32									_usedSize = { 0 };

};

// -----------------------------------
//			SendBufferManager
// -----------------------------------
class SendBufferManager
{
public:
	SendBufferRef		Open(uint32 size); // SendBuffer ����� �߶� �����Ѵ�.

private:
	SendBufferChunkRef	Pop();
	void				Push(SendBufferChunkRef buffer);

	static void			PushGlobal(SendBufferChunk* buffer);

private:
	Mutex							_mutex;
	myVector<SendBufferChunkRef>	_sendBufferChunks; // vector���� stack�� �뵵�� ���δ�.
};