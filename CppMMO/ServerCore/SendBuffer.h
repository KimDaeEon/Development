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
	BYTE*				_buffer; // SendBufferChunk에서 받아온 포인터만 들고 있는다.
	uint32				_allocSize = 0;
	uint32				_writeSize = 0;
	SendBufferChunkRef	_owner; // SendBufferChunk로 할당받은 메모리를 사용 중에 해제되지 않도록 하기 위한 참조
};

// ----------------------------------
//			SendBufferChunk
// ----------------------------------
// 엄청 큰 샌드 버퍼 덩어리를 할당받아놓고, 잘라서 사용한다.
// 해당 클래스는 Thread Local로 사용되기에 lock을 잡지 않는다.
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
	SendBufferRef		Open(uint32 size); // SendBuffer 덩어리를 잘라서 리턴한다.

private:
	SendBufferChunkRef	Pop();
	void				Push(SendBufferChunkRef buffer);

	static void			PushGlobal(SendBufferChunk* buffer);

private:
	Mutex							_mutex;
	myVector<SendBufferChunkRef>	_sendBufferChunks; // vector지만 stack의 용도로 쓰인다.
};